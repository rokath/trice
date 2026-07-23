// SPDX-License-Identifier: MIT

package vis

import (
	"fmt"
	"go/ast"
	"go/token"
	"math"
	"strconv"
	"strings"

	"github.com/rokath/trice/internal/decoder"
)

// scalarKind is the deliberately small type system used by -vis expressions.
type scalarKind uint8

const (
	// scalarUnknown defers a positional value's type until a LUT candidate is classified.
	scalarUnknown scalarKind = iota
	// scalarSigned represents a signed integer.
	scalarSigned
	// scalarUnsigned represents an unsigned integer.
	scalarUnsigned
	// scalarFloat represents a floating-point value.
	scalarFloat
	// scalarBool represents a Boolean value.
	scalarBool
)

// scalar carries one evaluated expression value without discarding its source category or width.
type scalar struct {
	kind     scalarKind
	bits     int
	signed   int64
	unsigned uint64
	floating float64
	boolean  bool
}

// candidateShape describes the typed positional values of one fixed-width numeric LUT entry.
type candidateShape struct {
	values [decoder.VisValueCapacity]scalarKind
	count  int
}

// expressionReferences records fields that affect candidate and Target-Stamp eligibility.
type expressionReferences struct {
	values    [decoder.VisValueCapacity]bool
	genericTS bool
	ts16      bool
	ts32      bool
}

// merge adds another expression's field references without changing their meaning.
func (r *expressionReferences) merge(other expressionReferences) {
	for i := range r.values {
		r.values[i] = r.values[i] || other.values[i]
	}
	r.genericTS = r.genericTS || other.genericTS
	r.ts16 = r.ts16 || other.ts16
	r.ts32 = r.ts32 || other.ts32
}

// compiledExpression is a validated AST subset ready for candidate inference and runtime evaluation.
type compiledExpression struct {
	root        expressionNode
	directField bool
	references  expressionReferences
	source      string
}

// expressionNode is implemented only by the whitelisted expression forms.
type expressionNode interface {
	evaluate(decoder.VisRecord) (scalar, error)
	infer(*candidateShape) (scalarKind, error)
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
	kind  fieldKind
	index int
	name  string
}

// evaluate reads a field from the current typed decoder record.
func (n fieldNode) evaluate(record decoder.VisRecord) (scalar, error) {
	switch n.kind {
	case fieldID:
		return scalar{kind: scalarUnsigned, bits: 16, unsigned: uint64(record.ID)}, nil
	case fieldTS, fieldTS16, fieldTS32:
		return scalar{kind: scalarUnsigned, bits: record.StampBits, unsigned: record.Stamp}, nil
	case fieldValue:
		if n.index < 0 || n.index >= record.ValueCount {
			return scalar{}, fmt.Errorf("%s is unavailable in ID %d", n.name, record.ID)
		}
		return scalarFromVisValue(record.Values[n.index])
	default:
		return scalar{}, fmt.Errorf("unsupported field %q", n.name)
	}
}

// infer resolves a field category using a candidate when the field is positional.
func (n fieldNode) infer(shape *candidateShape) (scalarKind, error) {
	if n.kind != fieldValue {
		return scalarUnsigned, nil
	}
	if shape == nil {
		return scalarUnknown, nil
	}
	if n.index < 0 || n.index >= shape.count {
		return scalarUnknown, fmt.Errorf("%s is unavailable", n.name)
	}
	return shape.values[n.index], nil
}

// literalNode retains one direct numeric literal and its integer or floating category.
type literalNode struct {
	value scalar
}

// evaluate returns the immutable literal value.
func (n literalNode) evaluate(decoder.VisRecord) (scalar, error) {
	return n.value, nil
}

// infer returns the literal's statically known category.
func (n literalNode) infer(*candidateShape) (scalarKind, error) {
	return n.value.kind, nil
}

// unaryNode evaluates the supported unary minus operation as float64.
type unaryNode struct {
	child expressionNode
}

// evaluate applies unary minus after the documented float64 conversion.
func (n unaryNode) evaluate(record decoder.VisRecord) (scalar, error) {
	value, err := n.child.evaluate(record)
	if err != nil {
		return scalar{}, err
	}
	floating, err := scalarAsFloat64(value)
	if err != nil {
		return scalar{}, err
	}
	return scalar{kind: scalarFloat, bits: 64, floating: -floating}, nil
}

// infer rejects Boolean operands and otherwise yields the arithmetic float64 category.
func (n unaryNode) infer(shape *candidateShape) (scalarKind, error) {
	kind, err := n.child.infer(shape)
	if err != nil {
		return scalarUnknown, err
	}
	if kind == scalarBool {
		return scalarUnknown, fmt.Errorf("Boolean values cannot participate in arithmetic")
	}
	return scalarFloat, nil
}

// binaryNode evaluates one of the four supported arithmetic operators as float64.
type binaryNode struct {
	operator token.Token
	left     expressionNode
	right    expressionNode
}

// evaluate performs checked division and ordinary float64 arithmetic.
func (n binaryNode) evaluate(record decoder.VisRecord) (scalar, error) {
	left, err := n.left.evaluate(record)
	if err != nil {
		return scalar{}, err
	}
	right, err := n.right.evaluate(record)
	if err != nil {
		return scalar{}, err
	}
	l, err := scalarAsFloat64(left)
	if err != nil {
		return scalar{}, err
	}
	r, err := scalarAsFloat64(right)
	if err != nil {
		return scalar{}, err
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
			return scalar{}, fmt.Errorf("division by zero")
		}
		result = l / r
	default:
		return scalar{}, fmt.Errorf("unsupported arithmetic operator %q", n.operator)
	}
	return scalar{kind: scalarFloat, bits: 64, floating: result}, nil
}

// infer verifies both arithmetic operands and yields the float64 result category.
func (n binaryNode) infer(shape *candidateShape) (scalarKind, error) {
	left, err := n.left.infer(shape)
	if err != nil {
		return scalarUnknown, err
	}
	right, err := n.right.infer(shape)
	if err != nil {
		return scalarUnknown, err
	}
	if left == scalarBool || right == scalarBool {
		return scalarUnknown, fmt.Errorf("Boolean values cannot participate in arithmetic")
	}
	return scalarFloat, nil
}

// compileExpression validates one Go expression AST and lowers it into the small -vis evaluator.
func compileExpression(node ast.Expr, source string) (compiledExpression, error) {
	var references expressionReferences
	root, directField, err := compileExpressionNode(node, &references)
	if err != nil {
		return compiledExpression{}, err
	}
	return compiledExpression{
		root:        root,
		directField: directField,
		references:  references,
		source:      source,
	}, nil
}

// compileExpressionNode accepts only identifiers, numeric literals, parentheses, unary minus, and basic arithmetic.
func compileExpressionNode(node ast.Expr, references *expressionReferences) (expressionNode, bool, error) {
	switch n := node.(type) {
	case *ast.ParenExpr:
		return compileExpressionNode(n.X, references)
	case *ast.Ident:
		field, err := compileField(n.Name, references)
		return field, err == nil, err
	case *ast.BasicLit:
		literal, err := compileLiteral(n)
		return literal, false, err
	case *ast.UnaryExpr:
		if n.Op != token.SUB {
			return nil, false, fmt.Errorf("only unary minus is supported")
		}
		child, _, err := compileExpressionNode(n.X, references)
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
		left, _, err := compileExpressionNode(n.X, references)
		if err != nil {
			return nil, false, err
		}
		right, _, err := compileExpressionNode(n.Y, references)
		if err != nil {
			return nil, false, err
		}
		return binaryNode{operator: n.Op, left: left, right: right}, false, nil
	default:
		return nil, false, fmt.Errorf("expression form %T is not supported", node)
	}
}

// compileField validates one identifier and records its eligibility implications.
func compileField(name string, references *expressionReferences) (expressionNode, error) {
	switch name {
	case "id":
		return fieldNode{kind: fieldID, name: name}, nil
	case "ts":
		references.genericTS = true
		return fieldNode{kind: fieldTS, name: name}, nil
	case "ts16":
		references.ts16 = true
		return fieldNode{kind: fieldTS16, name: name}, nil
	case "ts32":
		references.ts32 = true
		return fieldNode{kind: fieldTS32, name: name}, nil
	}
	if !strings.HasPrefix(name, "v") || len(name) < 2 {
		return nil, fmt.Errorf("unknown field %q", name)
	}
	index, err := strconv.Atoi(name[1:])
	if err != nil || index < 0 || index >= decoder.VisValueCapacity || name != fmt.Sprintf("v%d", index) {
		return nil, fmt.Errorf("unknown field %q; only v0 through v11 are supported", name)
	}
	references.values[index] = true
	return fieldNode{kind: fieldValue, index: index, name: name}, nil
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
			return literalNode{value: scalar{kind: scalarSigned, bits: 64, signed: int64(value)}}, nil
		}
		return literalNode{value: scalar{kind: scalarUnsigned, bits: 64, unsigned: value}}, nil
	case token.FLOAT:
		value, err := strconv.ParseFloat(clean, 64)
		if err != nil {
			return nil, fmt.Errorf("invalid floating literal %q: %w", literal.Value, err)
		}
		return literalNode{value: scalar{kind: scalarFloat, bits: 64, floating: value}}, nil
	default:
		return nil, fmt.Errorf("only integer and floating-point literals are supported")
	}
}

// scalarFromVisValue converts the public decoder union into the evaluator's private scalar form.
func scalarFromVisValue(value decoder.VisValue) (scalar, error) {
	switch value.Kind {
	case decoder.VisValueSigned:
		return scalar{kind: scalarSigned, bits: value.Bits, signed: value.Signed}, nil
	case decoder.VisValueUnsigned:
		return scalar{kind: scalarUnsigned, bits: value.Bits, unsigned: value.Unsigned}, nil
	case decoder.VisValueFloat:
		return scalar{kind: scalarFloat, bits: value.Bits, floating: value.Float}, nil
	case decoder.VisValueBool:
		return scalar{kind: scalarBool, bits: value.Bits, boolean: value.Bool}, nil
	default:
		return scalar{}, fmt.Errorf("unsupported decoder value kind %d", value.Kind)
	}
}

// scalarAsFloat64 applies the documented arithmetic conversion and rejects Boolean operands.
func scalarAsFloat64(value scalar) (float64, error) {
	switch value.kind {
	case scalarSigned:
		return float64(value.signed), nil
	case scalarUnsigned:
		return float64(value.unsigned), nil
	case scalarFloat:
		return value.floating, nil
	case scalarBool:
		return 0, fmt.Errorf("Boolean values cannot participate in arithmetic")
	default:
		return 0, fmt.Errorf("unknown expression value type")
	}
}

// validateExpressionVerb rejects expression/formatter combinations known without a LUT candidate.
func validateExpressionVerb(expression compiledExpression, verb formatVerb, shape *candidateShape) error {
	kind, err := expression.root.infer(shape)
	if err != nil {
		return err
	}
	switch verb.class {
	case verbInteger, verbFloat:
		if kind == scalarBool {
			return fmt.Errorf("Boolean expression %q cannot use %%%c", expression.source, verb.verb)
		}
	case verbGeneric:
		return nil
	case verbBool:
		if !expression.directField {
			return fmt.Errorf("%%%c requires a direct Boolean field", verb.verb)
		}
		if kind != scalarUnknown && kind != scalarBool {
			return fmt.Errorf("expression %q is not Boolean", expression.source)
		}
	default:
		return fmt.Errorf("unsupported formatter verb %%%c", verb.verb)
	}
	return nil
}

// argumentForVerb performs the small set of documented format-driven conversions.
func argumentForVerb(value scalar, verb formatVerb) (interface{}, error) {
	switch verb.class {
	case verbInteger:
		switch value.kind {
		case scalarSigned, scalarUnsigned:
			return nativeScalar(value), nil
		case scalarFloat:
			// 2^63 is exactly representable while MaxInt64 is not, so use an exclusive upper bound.
			const int64Limit = 9223372036854775808.0
			if math.IsNaN(value.floating) || math.IsInf(value.floating, 0) ||
				value.floating < -int64Limit || value.floating >= int64Limit {
				return nil, fmt.Errorf("value %v cannot be converted safely to int64", value.floating)
			}
			return int64(math.Trunc(value.floating)), nil
		default:
			return nil, fmt.Errorf("%%%c requires a numeric value", verb.verb)
		}
	case verbFloat:
		floating, err := scalarAsFloat64(value)
		if err != nil {
			return nil, fmt.Errorf("%%%c requires a numeric value: %w", verb.verb, err)
		}
		return floating, nil
	case verbGeneric:
		return nativeScalar(value), nil
	case verbBool:
		if value.kind != scalarBool {
			return nil, fmt.Errorf("%%t requires a Boolean value")
		}
		return value.boolean, nil
	default:
		return nil, fmt.Errorf("unsupported formatter verb %%%c", verb.verb)
	}
}

// nativeScalar reconstructs the source-width Go type for direct formatting and %v.
func nativeScalar(value scalar) interface{} {
	switch value.kind {
	case scalarSigned:
		switch value.bits {
		case 8:
			return int8(value.signed)
		case 16:
			return int16(value.signed)
		case 32:
			return int32(value.signed)
		default:
			return value.signed
		}
	case scalarUnsigned:
		switch value.bits {
		case 8:
			return uint8(value.unsigned)
		case 16:
			return uint16(value.unsigned)
		case 32:
			return uint32(value.unsigned)
		default:
			return value.unsigned
		}
	case scalarFloat:
		if value.bits == 32 {
			return float32(value.floating)
		}
		return value.floating
	case scalarBool:
		return value.boolean
	default:
		return nil
	}
}
