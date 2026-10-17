// SPDX-License-Identifier: MIT

package printf

import (
	"errors"
	"fmt"
	"go/ast"
	"go/constant"
	"go/token"
	"math"
	"math/big"
	"strings"

	"github.com/rokath/trice/internal/decoder"
)

// compiler carries context restrictions and registers every independent observer.
// The nesting flags prevent stateful phases and nested delta baselines.
type compiler struct {
	bits       int
	allowDelta bool
	inPhase    bool
	inDelta    bool
	unwraps    []*unwrapNode
	deltas     []*deltaNode
}

// compileCall admits stateful operations only in explicit stamp contexts.
func (c *compiler) compileCall(call *ast.CallExpr, refs *References) (expressionNode, bool, error) {
	name, ok := call.Fun.(*ast.Ident)
	if c.bits == 0 {
		return nil, false, fmt.Errorf("expression form %T is not supported", call)
	}
	if !ok || call.Ellipsis.IsValid() || c.inPhase {
		return nil, false, fmt.Errorf("unsupported function or stateful unwrap phase")
	}
	switch name.Name {
	case "unwrap":
		if len(call.Args) != 2 && len(call.Args) != 3 {
			return nil, false, fmt.Errorf("unwrap requires phase, modulus, and optional initial")
		}
		modulus, err := exactUint(call.Args[1])
		if err != nil || modulus == 0 {
			return nil, false, fmt.Errorf("unwrap modulus must be an integer constant in 1..UINT64_MAX")
		}
		var initial uint64
		if len(call.Args) == 3 {
			initial, err = exactUint(call.Args[2])
			if err != nil {
				return nil, false, fmt.Errorf("unwrap initial must be an integer constant in 0..UINT64_MAX: %w", err)
			}
		}
		c.inPhase = true
		phase, _, err := compileExpressionNode(call.Args[0], refs, c)
		c.inPhase = false
		if err != nil {
			return nil, false, err
		}
		node := &unwrapNode{phase: phase, modulus: modulus, initial: initial, explicit: len(call.Args) == 3}
		c.unwraps = append(c.unwraps, node)
		return node, false, nil
	case "delta":
		if !c.allowDelta || c.inDelta || len(call.Args) != 1 {
			return nil, false, fmt.Errorf("delta requires one operand in a delta column; nested delta is unsupported")
		}
		c.inDelta = true
		operand, _, err := compileExpressionNode(call.Args[0], refs, c)
		c.inDelta = false
		if err != nil {
			return nil, false, err
		}
		node := &deltaNode{operand: operand}
		c.deltas = append(c.deltas, node)
		return node, false, nil
	default:
		return nil, false, fmt.Errorf("unsupported function %q", name.Name)
	}
}

// exactConstant evaluates only constant arithmetic, preserving integers above 2^53.
// Division uses exact rational arithmetic even when both literals are integers.
func exactConstant(expr ast.Expr) (constant.Value, error) {
	switch n := expr.(type) {
	case *ast.ParenExpr:
		return exactConstant(n.X)
	case *ast.BasicLit:
		if n.Kind == token.INT || n.Kind == token.FLOAT {
			value := constant.MakeFromLiteral(n.Value, n.Kind, 0)
			if value.Kind() != constant.Unknown {
				return value, nil
			}
		}
	case *ast.UnaryExpr:
		if n.Op == token.SUB {
			value, err := exactConstant(n.X)
			if err != nil {
				return nil, err
			}
			return constant.UnaryOp(n.Op, value, 0), nil
		}
	case *ast.BinaryExpr:
		if n.Op != token.ADD && n.Op != token.SUB && n.Op != token.MUL && n.Op != token.QUO {
			break
		}
		left, err := exactConstant(n.X)
		if err != nil {
			return nil, err
		}
		right, err := exactConstant(n.Y)
		if err != nil {
			return nil, err
		}
		if n.Op == token.QUO {
			if constant.Sign(right) == 0 {
				return nil, fmt.Errorf("division by zero in constant")
			}
			left, right = constant.ToFloat(left), constant.ToFloat(right)
		}
		return constant.BinaryOp(left, n.Op, right), nil
	}
	return nil, fmt.Errorf("expected a numeric constant expression")
}

// exactUint rejects fractional, negative, or oversized constants before conversion.
func exactUint(expr ast.Expr) (uint64, error) {
	value, err := exactConstant(expr)
	if err != nil {
		return 0, err
	}
	value = constant.ToInt(value)
	if value.Kind() == constant.Int {
		if integer, ok := constant.Uint64Val(value); ok {
			return integer, nil
		}
	}
	return 0, fmt.Errorf("constant must be an integer in 0..UINT64_MAX")
}

// outcome caches both values and failures, so rendering cannot re-observe a sample.
type outcome struct {
	value Scalar
	err   error
}

// errFirstDelta propagates an undefined value without inventing a numeric zero.
// Other valid operands and independent runtime failures must still be evaluated.
var errFirstDelta = errors.New("first delta has no previous emission")

// evaluation is an immutable raw-event snapshot plus per-render delta operands.
type evaluation struct {
	decoder.VisRecord
	unwrapped   map[*unwrapNode]outcome
	differences map[*deltaNode]outcome
	operands    map[*deltaNode]Scalar
	missing     bool
}

// unwrapNode owns one exact tick history. Overflow remains latched until Reset.
type unwrapNode struct {
	phase    expressionNode
	modulus  uint64
	initial  uint64
	explicit bool
	previous uint64
	total    uint64
	started  bool
	overflow bool
}

// Evaluate reads the saved observation; it never changes counter history.
func (n *unwrapNode) Evaluate(e *evaluation) (Scalar, error) {
	value, ok := e.unwrapped[n]
	if !ok {
		return Scalar{}, fmt.Errorf("unwrap has no observed sample")
	}
	return value.value, value.err
}

// Infer keeps reconstructed ticks integral through direct formatting and delta.
func (*unwrapNode) Infer(*Shape) (Kind, error) { return Unsigned, nil }

// observe advances only from a valid phase and checks addition before committing it.
func (n *unwrapNode) observe(e *evaluation) outcome {
	if n.overflow {
		return outcome{err: fmt.Errorf("unwrap accumulator overflow; start a new tracking session")}
	}
	value, err := n.phase.Evaluate(e)
	if err != nil {
		return outcome{err: err}
	}
	phase, err := phaseUint(value)
	if err != nil {
		return outcome{err: err}
	}
	if phase >= n.modulus {
		return outcome{err: fmt.Errorf("unwrap phase %d is outside 0..%d", phase, n.modulus-1)}
	}
	if !n.started {
		n.total = phase
		if n.explicit {
			n.total = n.initial
		}
		n.started = true
	} else {
		var step uint64
		if phase >= n.previous {
			step = phase - n.previous
		} else {
			step = n.modulus - n.previous + phase
		}
		if step > math.MaxUint64-n.total {
			n.overflow = true
			return outcome{err: fmt.Errorf("unwrap accumulator overflow; start a new tracking session")}
		}
		n.total += step
	}
	n.previous = phase
	return outcome{value: Scalar{Kind: Unsigned, Bits: 64, Unsigned: n.total}}
}

// phaseUint admits integral finite arithmetic results without unsafe conversion.
func phaseUint(value Scalar) (uint64, error) {
	switch value.Kind {
	case Unsigned:
		return value.Unsigned, nil
	case Signed:
		if value.Signed >= 0 {
			return uint64(value.Signed), nil
		}
	case Float:
		if !math.IsNaN(value.Floating) && !math.IsInf(value.Floating, 0) &&
			value.Floating >= 0 && value.Floating < 18446744073709551616.0 && math.Trunc(value.Floating) == value.Floating {
			return uint64(value.Floating), nil
		}
	}
	return 0, fmt.Errorf("unwrap phase must be finite, nonnegative, integral, and representable by uint64")
}

// deltaNode retains the operand of the previous successful field emission.
type deltaNode struct {
	operand  expressionNode
	previous Scalar
	started  bool
}

// Evaluate caches the current operand and computes a difference without committing it.
func (n *deltaNode) Evaluate(e *evaluation) (Scalar, error) {
	if value, ok := e.differences[n]; ok {
		return value.value, value.err
	}
	current, err := n.operand.Evaluate(e)
	if err == nil {
		err = finite(current)
	}
	result := Scalar{Kind: Unsigned, Bits: 64}
	if err == nil {
		e.operands[n] = current
		if !n.started {
			e.missing = true
			err = errFirstDelta
		} else {
			result, err = subtract(current, n.previous)
		}
	}
	e.differences[n] = outcome{value: result, err: err}
	return result, err
}

// Infer defers the signedness of an integer difference until its operands are known.
func (n *deltaNode) Infer(shape *Shape) (Kind, error) { return n.operand.Infer(shape) }

// subtract preserves integral differences before any requested unit conversion.
// big.Int avoids intermediate signed overflow for mixed signed/unsigned operands.
func subtract(current, previous Scalar) (Scalar, error) {
	if current.Kind == Float || previous.Kind == Float {
		left, err := scalarAsFloat64(current)
		if err != nil {
			return Scalar{}, err
		}
		right, err := scalarAsFloat64(previous)
		if err != nil {
			return Scalar{}, err
		}
		value := Scalar{Kind: Float, Bits: 64, Floating: left - right}
		return value, finite(value)
	}
	integer := func(value Scalar) *big.Int {
		if value.Kind == Unsigned {
			return new(big.Int).SetUint64(value.Unsigned)
		}
		return big.NewInt(value.Signed)
	}
	diff := integer(current)
	diff.Sub(diff, integer(previous))
	if diff.Sign() < 0 && diff.IsInt64() {
		return Scalar{Kind: Signed, Bits: 64, Signed: diff.Int64()}, nil
	}
	if diff.Sign() >= 0 && diff.IsUint64() {
		return Scalar{Kind: Unsigned, Bits: 64, Unsigned: diff.Uint64()}, nil
	}
	return Scalar{}, fmt.Errorf("delta result is not representable by int64 or uint64")
}

// finite rejects unavailable arithmetic rather than printing a misleading unit.
func finite(value Scalar) error {
	if value.Kind == Float && (math.IsNaN(value.Floating) || math.IsInf(value.Floating, 0)) {
		return fmt.Errorf("nonfinite expression result")
	}
	return nil
}

// Sample preserves one event's reconstructed values and repeatable field rendering.
// Reading or measuring a placeholder never commits a visible-delta baseline.
type Sample struct {
	program          *Program
	event            evaluation
	rendered         bool
	text             string
	missing          bool
	err              error
	observationError error
}

// Observe updates every registered unwrap before filters or output selection run.
// A missing stamp must be excluded by the caller; a present zero is a valid sample.
func (p *Program) Observe(record decoder.VisRecord) *Sample {
	s := &Sample{program: p, event: evaluation{VisRecord: record, unwrapped: make(map[*unwrapNode]outcome)}}
	for _, node := range p.unwraps {
		value := node.observe(&s.event)
		s.event.unwrapped[node] = value
		if value.err != nil && s.observationError == nil {
			s.observationError = value.err
		}
	}
	return s
}

// ObservationError reports unusable phases even when their application event is hidden.
func (s *Sample) ObservationError() error { return s.observationError }

// Render evaluates the complete field, then commits all delta baselines together.
// Undefined first deltas initialize valid operands; runtime failures commit nothing.
// The boolean distinguishes first-delta omission from a runtime-error placeholder.
func (s *Sample) Render() (string, bool, error) {
	if s.rendered {
		return s.text, s.missing, s.err
	}
	s.rendered = true
	e := &s.event
	e.differences = make(map[*deltaNode]outcome)
	e.operands = make(map[*deltaNode]Scalar)
	for _, node := range s.program.deltas {
		if _, err := node.Evaluate(e); err != nil && !errors.Is(err, errFirstDelta) && s.err == nil {
			s.err = err
		}
	}
	args := make([]any, len(s.program.Expressions))
	for i, expression := range s.program.Expressions {
		value, err := expression.Root.Evaluate(e)
		if errors.Is(err, errFirstDelta) {
			continue
		}
		if err == nil {
			err = finite(value)
		}
		if err == nil {
			args[i], err = ArgumentForVerb(value, s.program.Verbs[i])
		}
		if err != nil && s.err == nil {
			s.err = fmt.Errorf("expression %d (%s): %w", i+1, expression.Source, err)
		}
	}
	if s.err == nil && !e.missing {
		s.text = fmt.Sprintf(s.program.Format, args...)
		if strings.Contains(s.text, "%!") {
			s.err = fmt.Errorf("formatter produced a Go diagnostic")
		}
	}
	if s.err != nil {
		s.text = s.program.Placeholder("?")
		return s.text, false, s.err
	}
	for node, value := range e.operands {
		node.previous, node.started = value, true
	}
	s.missing = e.missing
	if s.missing {
		s.text = s.program.Placeholder("-")
	}
	return s.text, s.missing, nil
}

// Placeholder substitutes markers using literal widths, without evaluating any node.
func (p *Program) Placeholder(marker string) string {
	var out strings.Builder
	position := 0
	for _, verb := range p.Verbs {
		out.WriteString(strings.ReplaceAll(p.Format[position:verb.start], "%%", "%"))
		directive := p.Format[verb.start+1 : verb.end-1]
		i := 0
		for i < len(directive) && strings.ContainsRune("+#- 0", rune(directive[i])) {
			i++
		}
		j := i
		for j < len(directive) && directive[j] >= '0' && directive[j] <= '9' {
			j++
		}
		align := ""
		if strings.Contains(directive[:i], "-") {
			align = "-"
		}
		out.WriteString(fmt.Sprintf("%"+align+directive[i:j]+"s", marker))
		position = verb.end
	}
	out.WriteString(strings.ReplaceAll(p.Format[position:], "%%", "%"))
	return out.String()
}

// Reset starts a new explicit tracking session without changing the compiled format.
func (p *Program) Reset() {
	for _, node := range p.unwraps {
		node.previous, node.total, node.started, node.overflow = 0, 0, false, false
	}
	for _, node := range p.deltas {
		node.previous, node.started = Scalar{}, false
	}
}
