// SPDX-License-Identifier: MIT

package printf

import (
	"errors"
	"fmt"
	"go/ast"
	"go/token"
	"math"
	"strconv"
	"strings"

	"github.com/rokath/trice/internal/decoder"
)

// scalarKind is the deliberately small type system used by -vis expressions.
type Kind uint8

const (
	// scalarUnknown defers a positional value's type until a LUT candidate is classified.
	Unknown Kind = iota
	// scalarSigned represents a signed integer.
	Signed
	// scalarUnsigned represents an unsigned integer.
	Unsigned
	// scalarFloat represents a floating-point value.
	Float
	// scalarBool represents a Boolean value.
	Bool
)

// scalar carries one evaluated expression value without discarding its source category or width.
type Scalar struct {
	Kind     Kind
	Bits     int
	Signed   int64
	Unsigned uint64
	Floating float64
	Boolean  bool
}

// candidateShape describes the typed positional values of one fixed-width numeric LUT entry.
type Shape struct {
	Values [decoder.VisValueCapacity]Kind
	Count  int
}

// expressionReferences records fields that affect candidate and Target-Stamp eligibility.
type References struct {
	Values    [decoder.VisValueCapacity]bool
	GenericTS bool
	TS16      bool
	TS32      bool
}

// merge adds another expression's field references without changing their meaning.
func (r *References) Merge(other References) {
	for i := range r.Values {
		r.Values[i] = r.Values[i] || other.Values[i]
	}
	r.GenericTS = r.GenericTS || other.GenericTS
	r.TS16 = r.TS16 || other.TS16
	r.TS32 = r.TS32 || other.TS32
}

// compiledExpression is a validated AST subset ready for candidate inference and runtime evaluation.
type Expression struct {
	Root        expressionNode
	DirectField bool
	References  References
	Source      string
}

// expressionNode is implemented only by the whitelisted expression forms.
type expressionNode interface {
	Evaluate(*evaluation) (Scalar, error)
	Infer(*Shape) (Kind, error)
}

// Evaluate runs a stateless visualization expression without stamp observers.
func (e Expression) Evaluate(record decoder.VisRecord) (Scalar, error) {
	return e.Root.Evaluate(&evaluation{VisRecord: record})
}

// fieldKind distinguishes fixed metadata fields from positional TREX values.
type fieldKind uint8

const (
	// fieldID exposes the unsigned Trice ID.
	fieldID fieldKind = iota
	// fieldTS exposes a generic raw Target-Stamp.
	fieldTS
	// fieldTS16 exposes only a raw 16-bit Target-Stamp.
	fieldTS16
	// fieldTS32 exposes only a raw 32-bit Target-Stamp.
	fieldTS32
	// fieldValue exposes one positional typed TREX value.
	fieldValue
)

// fieldNode evaluates one supported identifier.
type fieldNode struct {
	Kind  fieldKind
	index int
	name  string
}

// evaluate reads a field from the current typed decoder record.
func (n fieldNode) Evaluate(record *evaluation) (Scalar, error) {
	switch n.Kind {
	case fieldID:
		return Scalar{Kind: Unsigned, Bits: 16, Unsigned: uint64(record.ID)}, nil
	case fieldTS, fieldTS16, fieldTS32:
		return Scalar{Kind: Unsigned, Bits: record.StampBits, Unsigned: record.Stamp}, nil
	case fieldValue:
		if n.index < 0 || n.index >= record.ValueCount {
			return Scalar{}, fmt.Errorf("%s is unavailable in ID %d", n.name, record.ID)
		}
		return scalarFromVisValue(record.Values[n.index])
	default:
		return Scalar{}, fmt.Errorf("unsupported field %q", n.name)
	}
}

// infer resolves a field category using a candidate when the field is positional.
func (n fieldNode) Infer(shape *Shape) (Kind, error) {
	if n.Kind != fieldValue {
		return Unsigned, nil
	}
	if shape == nil {
		return Unknown, nil
	}
	if n.index < 0 || n.index >= shape.Count {
		return Unknown, fmt.Errorf("%s is unavailable", n.name)
	}
	return shape.Values[n.index], nil
}

// literalNode retains one direct numeric literal and its integer or floating category.
type literalNode struct {
	value Scalar
}

// evaluate returns the immutable literal value.
func (n literalNode) Evaluate(*evaluation) (Scalar, error) {
	return n.value, nil
}

// infer returns the literal's statically known category.
func (n literalNode) Infer(*Shape) (Kind, error) {
	return n.value.Kind, nil
}

// unaryNode evaluates the supported unary minus operation as float64.
type unaryNode struct {
	child expressionNode
}

// evaluate applies unary minus after the documented float64 conversion.
func (n unaryNode) Evaluate(record *evaluation) (Scalar, error) {
	value, err := n.child.Evaluate(record)
	if err != nil {
		return Scalar{}, err
	}
	floating, err := scalarAsFloat64(value)
	if err != nil {
		return Scalar{}, err
	}
	return Scalar{Kind: Float, Bits: 64, Floating: -floating}, nil
}

// infer rejects Boolean operands and otherwise yields the arithmetic float64 category.
func (n unaryNode) Infer(shape *Shape) (Kind, error) {
	kind, err := n.child.Infer(shape)
	if err != nil {
		return Unknown, err
	}
	if kind == Bool {
		return Unknown, fmt.Errorf("Boolean values cannot participate in arithmetic")
	}
	return Float, nil
}

// binaryNode evaluates one of the four supported arithmetic operators as float64.
type binaryNode struct {
	operator token.Token
	left     expressionNode
	right    expressionNode
}

// evaluate performs checked division and ordinary float64 arithmetic.
func (n binaryNode) Evaluate(record *evaluation) (Scalar, error) {
	left, err := n.left.Evaluate(record)
	right, rightErr := n.right.Evaluate(record)
	if err != nil || rightErr != nil {
		if err != nil && !errors.Is(err, errFirstDelta) {
			return Scalar{}, err
		}
		if rightErr != nil && !errors.Is(rightErr, errFirstDelta) {
			return Scalar{}, rightErr
		}
		// An independently known zero divisor remains an error even if the
		// numerator is the undefined first delta.
		if n.operator == token.QUO && rightErr == nil {
			divisor, conversionErr := scalarAsFloat64(right)
			if conversionErr != nil {
				return Scalar{}, conversionErr
			}
			if divisor == 0 {
				return Scalar{}, fmt.Errorf("division by zero")
			}
		}
		return Scalar{}, errFirstDelta
	}
	l, err := scalarAsFloat64(left)
	if err != nil {
		return Scalar{}, err
	}
	r, err := scalarAsFloat64(right)
	if err != nil {
		return Scalar{}, err
	}

	var result float64
	switch n.operator {
	case token.ADD:
		result = l + r
	case token.SUB:
		result = l - r
	case token.MUL:
		result = l * r
	case token.QUO:
		if r == 0 {
			return Scalar{}, fmt.Errorf("division by zero")
		}
		result = l / r
	default:
		return Scalar{}, fmt.Errorf("unsupported arithmetic operator %q", n.operator)
	}
	return Scalar{Kind: Float, Bits: 64, Floating: result}, nil
}

// infer verifies both arithmetic operands and yields the float64 result category.
func (n binaryNode) Infer(shape *Shape) (Kind, error) {
	left, err := n.left.Infer(shape)
	if err != nil {
		return Unknown, err
	}
	right, err := n.right.Infer(shape)
	if err != nil {
		return Unknown, err
	}
	if left == Bool || right == Bool {
		return Unknown, fmt.Errorf("Boolean values cannot participate in arithmetic")
	}
	return Float, nil
}

// compileExpression validates one Go expression AST and lowers it into the small -vis evaluator.
func CompileExpression(node ast.Expr, source string) (Expression, error) {
	return compileInContext(node, source, &compiler{})
}

// compileInContext applies the caller's field and stateful-function whitelist.
func compileInContext(node ast.Expr, source string, context *compiler) (Expression, error) {
	var references References
	root, directField, err := compileExpressionNode(node, &references, context)
	if err != nil {
		return Expression{}, err
	}
	return Expression{
		Root:        root,
		DirectField: directField,
		References:  references,
		Source:      source,
	}, nil
}

// compileExpressionNode accepts only identifiers, numeric literals, parentheses, unary minus, and basic arithmetic.
func compileExpressionNode(node ast.Expr, references *References, context *compiler) (expressionNode, bool, error) {
	switch n := node.(type) {
	case *ast.ParenExpr:
		return compileExpressionNode(n.X, references, context)
	case *ast.Ident:
		if context.bits != 0 && n.Name != "ts" && n.Name != fmt.Sprintf("ts%d", context.bits) {
			return nil, false, fmt.Errorf("unknown field %q in %d-bit stamp context; use ts or ts%d", n.Name, context.bits, context.bits)
		}
		field, err := compileField(n.Name, references)
		return field, err == nil, err
	case *ast.BasicLit:
		literal, err := compileLiteral(n)
		return literal, false, err
	case *ast.UnaryExpr:
		if n.Op != token.SUB {
			return nil, false, fmt.Errorf("only unary minus is supported")
		}
		child, _, err := compileExpressionNode(n.X, references, context)
		if err != nil {
			return nil, false, err
		}
		return unaryNode{child: child}, false, nil
	case *ast.BinaryExpr:
		switch n.Op {
		case token.ADD, token.SUB, token.MUL, token.QUO:
		default:
			return nil, false, fmt.Errorf("operator %q is not supported", n.Op)
		}
		left, _, err := compileExpressionNode(n.X, references, context)
		if err != nil {
			return nil, false, err
		}
		right, _, err := compileExpressionNode(n.Y, references, context)
		if err != nil {
			return nil, false, err
		}
		return binaryNode{operator: n.Op, left: left, right: right}, false, nil
	case *ast.CallExpr:
		return context.compileCall(n, references)
	default:
		return nil, false, fmt.Errorf("expression form %T is not supported", node)
	}
}

// compileField validates one identifier and records its eligibility implications.
func compileField(name string, references *References) (expressionNode, error) {
	switch name {
	case "id":
		return fieldNode{Kind: fieldID, name: name}, nil
	case "ts":
		references.GenericTS = true
		return fieldNode{Kind: fieldTS, name: name}, nil
	case "ts16":
		references.TS16 = true
		return fieldNode{Kind: fieldTS16, name: name}, nil
	case "ts32":
		references.TS32 = true
		return fieldNode{Kind: fieldTS32, name: name}, nil
	}
	if !strings.HasPrefix(name, "v") || len(name) < 2 {
		return nil, fmt.Errorf("unknown field %q", name)
	}
	index, err := strconv.Atoi(name[1:])
	if err != nil || index < 0 || index >= decoder.VisValueCapacity || name != fmt.Sprintf("v%d", index) {
		return nil, fmt.Errorf("unknown field %q; only v0 through v11 are supported", name)
	}
	references.Values[index] = true
	return fieldNode{Kind: fieldValue, index: index, name: name}, nil
}

// compileLiteral parses Go integer and floating literals without accepting strings, runes, or imaginary values.
func compileLiteral(literal *ast.BasicLit) (expressionNode, error) {
	clean := strings.ReplaceAll(literal.Value, "_", "")
	switch literal.Kind {
	case token.INT:
		value, err := strconv.ParseUint(clean, 0, 64)
		if err != nil {
			return nil, fmt.Errorf("invalid integer literal %q: %w", literal.Value, err)
		}
		if value <= math.MaxInt64 {
			return literalNode{value: Scalar{Kind: Signed, Bits: 64, Signed: int64(value)}}, nil
		}
		return literalNode{value: Scalar{Kind: Unsigned, Bits: 64, Unsigned: value}}, nil
	case token.FLOAT:
		value, err := strconv.ParseFloat(clean, 64)
		if err != nil {
			return nil, fmt.Errorf("invalid floating literal %q: %w", literal.Value, err)
		}
		return literalNode{value: Scalar{Kind: Float, Bits: 64, Floating: value}}, nil
	default:
		return nil, fmt.Errorf("only integer and floating-point literals are supported")
	}
}

// scalarFromVisValue converts the public decoder union into the evaluator's private scalar form.
func scalarFromVisValue(value decoder.VisValue) (Scalar, error) {
	switch value.Kind {
	case decoder.VisValueSigned:
		return Scalar{Kind: Signed, Bits: value.Bits, Signed: value.Signed}, nil
	case decoder.VisValueUnsigned:
		return Scalar{Kind: Unsigned, Bits: value.Bits, Unsigned: value.Unsigned}, nil
	case decoder.VisValueFloat:
		return Scalar{Kind: Float, Bits: value.Bits, Floating: value.Float}, nil
	case decoder.VisValueBool:
		return Scalar{Kind: Bool, Bits: value.Bits, Boolean: value.Bool}, nil
	default:
		return Scalar{}, fmt.Errorf("unsupported decoder value kind %d", value.Kind)
	}
}

// scalarAsFloat64 applies the documented arithmetic conversion and rejects Boolean operands.
func scalarAsFloat64(value Scalar) (float64, error) {
	switch value.Kind {
	case Signed:
		return float64(value.Signed), nil
	case Unsigned:
		return float64(value.Unsigned), nil
	case Float:
		return value.Floating, nil
	case Bool:
		return 0, fmt.Errorf("Boolean values cannot participate in arithmetic")
	default:
		return 0, fmt.Errorf("unknown expression value type")
	}
}

// validateExpressionVerb rejects expression/formatter combinations known without a LUT candidate.
func ValidateExpressionVerb(expression Expression, verb Verb, shape *Shape) error {
	kind, err := expression.Root.Infer(shape)
	if err != nil {
		return err
	}
	switch verb.Class {
	case IntegerVerb, FloatVerb:
		if kind == Bool {
			return fmt.Errorf("Boolean expression %q cannot use %%%c", expression.Source, verb.Verb)
		}
	case GenericVerb:
		return nil
	case BoolVerb:
		if !expression.DirectField {
			return fmt.Errorf("%%%c requires a direct Boolean field", verb.Verb)
		}
		if kind != Unknown && kind != Bool {
			return fmt.Errorf("expression %q is not Boolean", expression.Source)
		}
	default:
		return fmt.Errorf("unsupported formatter verb %%%c", verb.Verb)
	}
	return nil
}

// argumentForVerb performs the small set of documented format-driven conversions.
func ArgumentForVerb(value Scalar, verb Verb) (interface{}, error) {
	switch verb.Class {
	case IntegerVerb:
		switch value.Kind {
		case Signed, Unsigned:
			return NativeScalar(value), nil
		case Float:
			// 2^63 is exactly representable while MaxInt64 is not, so use an exclusive upper bound.
			const int64Limit = 9223372036854775808.0
			if math.IsNaN(value.Floating) || math.IsInf(value.Floating, 0) ||
				value.Floating < -int64Limit || value.Floating >= int64Limit {
				return nil, fmt.Errorf("value %v cannot be converted safely to int64", value.Floating)
			}
			return int64(math.Trunc(value.Floating)), nil
		default:
			return nil, fmt.Errorf("%%%c requires a numeric value", verb.Verb)
		}
	case FloatVerb:
		floating, err := scalarAsFloat64(value)
		if err != nil {
			return nil, fmt.Errorf("%%%c requires a numeric value: %w", verb.Verb, err)
		}
		return floating, nil
	case GenericVerb:
		return NativeScalar(value), nil
	case BoolVerb:
		if value.Kind != Bool {
			return nil, fmt.Errorf("%%t requires a Boolean value")
		}
		return value.Boolean, nil
	default:
		return nil, fmt.Errorf("unsupported formatter verb %%%c", verb.Verb)
	}
}

// nativeScalar reconstructs the source-width Go type for direct formatting and %v.
func NativeScalar(value Scalar) interface{} {
	switch value.Kind {
	case Signed:
		switch value.Bits {
		case 8:
			return int8(value.Signed)
		case 16:
			return int16(value.Signed)
		case 32:
			return int32(value.Signed)
		default:
			return value.Signed
		}
	case Unsigned:
		switch value.Bits {
		case 8:
			return uint8(value.Unsigned)
		case 16:
			return uint16(value.Unsigned)
		case 32:
			return uint32(value.Unsigned)
		default:
			return value.Unsigned
		}
	case Float:
		if value.Bits == 32 {
			return float32(value.Floating)
		}
		return value.Floating
	case Bool:
		return value.Boolean
	default:
		return nil
	}
}
