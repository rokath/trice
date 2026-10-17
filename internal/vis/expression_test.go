// SPDX-License-Identifier: MIT

package vis

import (
	"math"
	"reflect"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/printf"

	"github.com/rokath/trice/internal/id"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestArgumentForVerbAppliesDocumentedConversions verifies type preservation and checked truncation.
func TestArgumentForVerbAppliesDocumentedConversions(t *testing.T) {
	integerVerb := printf.Verb{Verb: 'd', Class: printf.IntegerVerb}
	floatVerb := printf.Verb{Verb: 'f', Class: printf.FloatVerb}
	genericVerb := printf.Verb{Verb: 'v', Class: printf.GenericVerb}
	boolVerb := printf.Verb{Verb: 't', Class: printf.BoolVerb}

	argument, err := printf.ArgumentForVerb(printf.Scalar{Kind: printf.Signed, Bits: 16, Signed: -7}, integerVerb)
	require.NoError(t, err)
	assert.IsType(t, int16(0), argument)
	assert.Equal(t, int16(-7), argument)

	argument, err = printf.ArgumentForVerb(printf.Scalar{Kind: printf.Unsigned, Bits: 32, Unsigned: 0xf0000000}, integerVerb)
	require.NoError(t, err)
	assert.IsType(t, uint32(0), argument)
	assert.Equal(t, uint32(0xf0000000), argument)

	argument, err = printf.ArgumentForVerb(printf.Scalar{Kind: printf.Float, Bits: 64, Floating: 3.9}, integerVerb)
	require.NoError(t, err)
	assert.Equal(t, int64(3), argument)
	argument, err = printf.ArgumentForVerb(printf.Scalar{Kind: printf.Float, Bits: 64, Floating: -3.9}, integerVerb)
	require.NoError(t, err)
	assert.Equal(t, int64(-3), argument)

	argument, err = printf.ArgumentForVerb(printf.Scalar{Kind: printf.Unsigned, Bits: 64, Unsigned: math.MaxUint64}, floatVerb)
	require.NoError(t, err)
	assert.Equal(t, float64(uint64(math.MaxUint64)), argument)

	argument, err = printf.ArgumentForVerb(printf.Scalar{Kind: printf.Float, Bits: 32, Floating: 1.25}, genericVerb)
	require.NoError(t, err)
	assert.IsType(t, float32(0), argument)
	assert.Equal(t, float32(1.25), argument)

	argument, err = printf.ArgumentForVerb(printf.Scalar{Kind: printf.Bool, Bits: 8, Boolean: true}, boolVerb)
	require.NoError(t, err)
	assert.Equal(t, true, argument)
}

// TestArgumentForVerbRejectsUnsafeIntegerConversions verifies NaN, infinities, and both int64 bounds.
func TestArgumentForVerbRejectsUnsafeIntegerConversions(t *testing.T) {
	integerVerb := printf.Verb{Verb: 'd', Class: printf.IntegerVerb}
	values := []float64{
		math.NaN(),
		math.Inf(1),
		math.Inf(-1),
		9223372036854775808.0,
		-9223372036854775808.0 - 2048,
	}
	for _, value := range values {
		_, err := printf.ArgumentForVerb(printf.Scalar{Kind: printf.Float, Bits: 64, Floating: value}, integerVerb)
		require.Error(t, err)
		assert.Contains(t, err.Error(), "cannot be converted safely")
	}
}

// TestArithmeticExpressionsAreFloat64 verifies every supported operator, parentheses, literals, and reordering.
func TestArithmeticExpressionsAreFloat64(t *testing.T) {
	fileSystem := testFileSystem()
	lut := id.TriceIDLookUp{
		1: {Type: "TRICE32_4", Strg: "math:a=%d,b=%d,c=%d,d=%d\n"},
	}
	router, err := NewRouter(
		nil,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{`math:printf("%v,%v,%v,%v,%v,%v",ts32/10,v0*0.5,v1+100,20.0*v2-1000,-(v3),0x20+1)@math.txt`},
		false,
	)
	require.NoError(t, err)
	t.Cleanup(func() { require.NoError(t, router.Close()) })

	record := testRecord(
		1,
		"TRICE32_4",
		"math:a=%d,b=%d,c=%d,d=%d\n",
		125,
		32,
		signedValue(32, 3),
		signedValue(32, 4),
		signedValue(32, 5),
		signedValue(32, 5),
	)
	assert.False(t, router.Process(record, false))
	assert.Equal(t, "12.5,1.5,104,-900,-5,33", readTestFile(t, fileSystem, "math.txt"))
}

// TestDirectFieldsAndLiteralsRetainEvaluatorCategories verifies the distinction from arithmetic results.
func TestDirectFieldsAndLiteralsRetainEvaluatorCategories(t *testing.T) {
	rule, err := parseRule(`msg:printf("%v,%v,%v,%v,%v",id,ts32,v0,10,10.0)@types.txt`)
	require.NoError(t, err)
	record := testRecord(
		3,
		"TRICE32_1",
		"msg:v=%u\n",
		7,
		32,
		unsignedValue(32, 9),
	)
	wantKinds := []printf.Kind{printf.Unsigned, printf.Unsigned, printf.Unsigned, printf.Signed, printf.Float}
	for i, expression := range rule.expressions {
		value, err := expression.Evaluate(record)
		require.NoError(t, err)
		assert.Equal(t, wantKinds[i], value.Kind)
	}

	arithmetic, err := parseRule(`msg:printf("%v",v0+0)@type.txt`)
	require.NoError(t, err)
	value, err := arithmetic.expressions[0].Evaluate(record)
	require.NoError(t, err)
	assert.Equal(t, printf.Float, value.Kind)
	assert.Equal(t, reflect.TypeOf(float64(0)), reflect.TypeOf(printf.NativeScalar(value)))
}

// TestBoolCandidateCompatibilityIsDeferred verifies that %t is accepted syntactically but classified by the LUT type.
func TestBoolCandidateCompatibilityIsDeferred(t *testing.T) {
	rule, err := parseRule(`msg:printf("%t",v0)@bool.txt`)
	require.NoError(t, err)

	boolShape, err := fixedNumericShape(id.TriceFmt{Type: "TRICE32_1", Strg: "msg:v=%t\n"})
	require.NoError(t, err)
	require.NoError(t, printf.ValidateExpressionVerb(rule.expressions[0], rule.verbs[0], &boolShape))

	signedShape, err := fixedNumericShape(id.TriceFmt{Type: "TRICE32_1", Strg: "msg:v=%d\n"})
	require.NoError(t, err)
	require.Error(t, printf.ValidateExpressionVerb(rule.expressions[0], rule.verbs[0], &signedShape))
}

// TestNoFormattingDiagnosticReachesASink verifies representative widths, precision, Boolean, and literal percent.
func TestNoFormattingDiagnosticReachesASink(t *testing.T) {
	fileSystem := testFileSystem()
	lut := id.TriceIDLookUp{
		1: {Type: "TRICE32_3", Strg: "fmt:u=%u,f=%f,b=%t\n"},
	}
	router, err := NewRouter(
		nil,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{`fmt:printf("%08x,%0.2f,%t,%%",v0,v1,v2)@format.txt`},
		false,
	)
	require.NoError(t, err)
	t.Cleanup(func() { require.NoError(t, router.Close()) })

	record := testRecord(
		1,
		"TRICE32_3",
		"fmt:u=%u,f=%f,b=%t\n",
		0,
		0,
		unsignedValue(32, 15),
		floatValue(32, 1.25),
		boolValue(32, true),
	)
	assert.False(t, router.Process(record, false))
	output := readTestFile(t, fileSystem, "format.txt")
	assert.Equal(t, "0000000f,1.25,true,%", output)
	assert.NotContains(t, output, "%!")
}
