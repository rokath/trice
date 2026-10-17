// SPDX-License-Identifier: MIT

package printf

import (
	"fmt"
	"math"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// stampProgram compiles a complete field so tests exercise the public language.
func stampProgram(t *testing.T, text string, bits int, delta bool) Program {
	t.Helper()
	program, err := ParseStamp(text, bits, delta)
	require.NoError(t, err)
	return program
}

// stampSample supplies one usable raw stamp, including the valid value zero.
func stampSample(program *Program, raw uint64, bits int) *Sample {
	return program.Observe(decoder.VisRecord{Stamp: raw, StampBits: bits})
}

// TestStampArithmeticAndReconstruction covers the published tables and exact
// integers, using complete printf fields rather than testing implementation nodes.
func TestStampArithmeticAndReconstruction(t *testing.T) {
	tests := []struct {
		name       string
		expression string
		bits       int
		raw        []uint64
		want       []string
	}{
		{"fractional phase", `printf("%.3f",(47999-ts)/48)`, 16, []uint64{47999, 47951, 23999, 0}, []string{"0.000", "1.000", "500.000", "999.979"}},
		{"truncate and round", `printf("%d/%.0f",(47999-ts)/48,(47999-ts)/48)`, 16, []uint64{0}, []string{"999/1000"}},
		{"raw and scaled", `printf("%d/%.3f",ts16,(47999-ts)/48)`, 16, []uint64{23999}, []string{"23999/500.000"}},
		{"two wraps", `printf("%d/%.3f",unwrap(47999-ts,48000),unwrap(47999-ts,48000)/48)`, 16, []uint64{47999, 23999, 0, 47999, 23999, 47999}, []string{"0/0.000", "24000/500.000", "47999/999.979", "48000/1000.000", "72000/1500.000", "96000/2000.000"}},
		{"default origin", `printf("%d",unwrap(47999-ts,48000))`, 16, []uint64{23999, 0, 47999}, []string{"24000", "47999", "48000"}},
		{"zero origin and equal phase", `printf("%d",unwrap(47999-ts,48000,0))`, 16, []uint64{23999, 23999, 0, 47999}, []string{"0", "0", "23999", "24000"}},
		{"16-bit up", `printf("%d",unwrap(ts,65536))`, 16, []uint64{65534, 65535, 0, 1}, []string{"65534", "65535", "65536", "65537"}},
		{"16-bit down", `printf("%d",unwrap(65535-ts,65536))`, 16, []uint64{1, 0, 65535, 65534}, []string{"65534", "65535", "65536", "65537"}},
		{"nonbinary modulus", `printf("%d",unwrap(ts,10000))`, 16, []uint64{9998, 9999, 0, 1}, []string{"9998", "9999", "10000", "10001"}},
		{"32-bit boundary", `printf("%d",unwrap(ts32,4294967296))`, 32, []uint64{math.MaxUint32, 0, 1}, []string{"4294967295", "4294967296", "4294967297"}},
		{"exact large origin", `printf("%d",unwrap(ts,65536,9007199254740993))`, 16, []uint64{0, 1}, []string{"9007199254740993", "9007199254740994"}},
		{"exact integral floating constant", `printf("%d",unwrap(ts,48000.0,9007199254740993.0))`, 16, []uint64{0, 1}, []string{"9007199254740993", "9007199254740994"}},
		{"exact constant arithmetic", `printf("%d",unwrap(ts,2*32768,9007199254740992+1))`, 16, []uint64{0}, []string{"9007199254740993"}},
		{"maximum initial", `printf("%x",unwrap(ts,18446744073709551615,18446744073709551615))`, 16, []uint64{0}, []string{"ffffffffffffffff"}},
		{"raw delta has no implicit modulo", `printf("%d",delta(ts))`, 16, []uint64{65535, 0}, []string{"-", "-65535"}},
		{"integral difference before scaling", `printf("%.9f",delta(unwrap(ts,65536,9007199254740993))/48)`, 16, []uint64{0, 1, 2}, []string{"-", "0.020833333", "0.020833333"}},
		{"arithmetic around delta", `printf("%.1f",2*delta(ts+0.5)-1)`, 16, []uint64{10, 11}, []string{"-", "1.0"}},
		{"undefined reciprocal initializes baseline", `printf("%.1f",1/delta(ts))`, 16, []uint64{10, 12}, []string{"-", "0.5"}},
		{"literal printf prefix", `printf("printf(%d)",ts)`, 16, []uint64{7}, []string{"printf(7)"}},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			program := stampProgram(t, tt.expression, tt.bits, true)
			for i, raw := range tt.raw {
				text, _, err := stampSample(&program, raw, tt.bits).Render()
				require.NoError(t, err)
				assert.Equal(t, tt.want[i], text)
			}
		})
	}
}

// TestStampConfigurationErrors rejects syntax, names, contexts, nesting, and
// nonexact constants before any observation or input-channel side effect.
func TestStampConfigurationErrors(t *testing.T) {
	for _, text := range []string{
		`printf("%d",ticks)`, `printf("%d",ts32)`, `printf("%d",v0)`, `printf("%d",id)`,
		`printf("%d",unwrap(ts,0))`, `printf("%d",unwrap(ts,-1))`,
		`printf("%d",unwrap(ts,48000.5))`, `printf("%d",unwrap(ts,ts))`,
		`printf("%d",unwrap(ts,18446744073709551616))`,
		`printf("%d",unwrap(ts,65536,9007199254740993.5))`,
		`printf("%d",unwrap(ts,65536,-1))`, `printf("%d",unwrap(ts,65536,18446744073709551616))`,
		`printf("%d",unwrap(ts,65536,1/2))`, `printf("%d",unwrap(ts,1/0))`,
		`printf("%d",unwrap(unwrap(ts,65536),65536))`,
		`printf("%d",unwrap(delta(ts),65536))`, `printf("%d",delta(delta(ts)))`,
		`printf("%d",unwrap(ts))`, `printf("%d",delta(ts,ts))`,
		`printf("%d",sqrt(ts))`, `printf("%d",ts...)`,
		`printf("%d",ts) trailing`, `printf("%d",`, `printf("%d")`,
		`printf("%s",ts)`, `printf("%t",ts)`, `printf("%*d",ts,ts)`,
	} {
		t.Run(text, func(t *testing.T) {
			_, err := ParseStamp(text, 16, true)
			require.Error(t, err)
		})
	}
	_, err := ParseStamp(`printf("%d",delta(ts))`, 16, false)
	require.ErrorContains(t, err, "delta")
	_, err = ParseStamp(`printf("%d",ts16)`, 32, false)
	require.ErrorContains(t, err, "32-bit")
	_, err = ParseStamp(`printf("%d",ts)`, 8, false)
	require.Error(t, err)
	for _, expression := range []string{`printf("%d",unwrap(ts,65536))`, `printf("%d",delta(ts))`} {
		_, err := Parse(expression)
		require.ErrorContains(t, err, "expression form")
	}
}

// TestInvisibleObservationsAndRepeatableRendering separates counter capture
// from delta emission, including saved snapshots and placeholder width queries.
func TestInvisibleObservationsAndRepeatableRendering(t *testing.T) {
	program := stampProgram(t, `printf("dt:%10.3f us",delta(unwrap(47999-ts,48000))/48)`, 16, true)
	assert.Equal(t, "dt:         - us", program.Placeholder("-"))
	first := stampSample(&program, 47999, 16)
	text, missing, err := first.Render()
	require.NoError(t, err)
	assert.True(t, missing)
	assert.Equal(t, "dt:         - us", text)
	for _, raw := range []uint64{23999, 47999, 23999, 47999, 23999} {
		stampSample(&program, raw, 16)
	}
	last := stampSample(&program, 47999, 16)
	// A later observation cannot overwrite the earlier event's reconstructed ticks.
	stampSample(&program, 23999, 16)
	for i := 0; i < 3; i++ {
		text, missing, err = last.Render()
		require.NoError(t, err)
		assert.False(t, missing)
		assert.Equal(t, "dt:  3000.000 us", text)
	}
	text, _, err = stampSample(&program, 47999, 16).Render()
	require.NoError(t, err)
	assert.Equal(t, "dt:  1000.000 us", text)
}

// TestRuntimeFailuresPreserveHistory verifies atomic delta commits, phase rejection,
// overflow latching, and explicit reset without contaminating another source.
func TestRuntimeFailuresPreserveHistory(t *testing.T) {
	t.Run("invalid phases retain last usable observation", func(t *testing.T) {
		program := stampProgram(t, `printf("%d",unwrap(ts/2,10))`, 16, false)
		for _, tt := range []struct {
			raw  uint64
			want string
			bad  bool
		}{{0, "0", false}, {3, "?", true}, {20, "?", true}, {18, "9", false}, {0, "10", false}} {
			text, _, err := stampSample(&program, tt.raw, 16).Render()
			assert.Equal(t, tt.want, text)
			assert.Equal(t, tt.bad, err != nil)
		}
	})
	t.Run("later arithmetic failure retains all baselines", func(t *testing.T) {
		program := stampProgram(t, `printf("%d/%d",delta(unwrap(ts,10))/(ts-2),delta(unwrap(ts,10,0)))`, 16, true)
		for _, tt := range []struct {
			raw     uint64
			want    string
			bad     bool
			missing bool
		}{{0, "-/-", false, true}, {1, "-1/1", false, false}, {2, "?/?", true, false}, {3, "2/2", false, false}} {
			text, missing, err := stampSample(&program, tt.raw, 16).Render()
			assert.Equal(t, tt.want, text)
			assert.Equal(t, tt.bad, err != nil)
			assert.Equal(t, tt.missing, missing)
		}
	})
	t.Run("invalid first operand does not establish baseline", func(t *testing.T) {
		program := stampProgram(t, `printf("%d",delta(1/ts))`, 16, true)
		_, _, err := stampSample(&program, 0, 16).Render()
		require.ErrorContains(t, err, "division by zero")
		_, missing, err := stampSample(&program, 1, 16).Render()
		require.NoError(t, err)
		assert.True(t, missing)
	})
	t.Run("unsafe formatting preserves every delta baseline", func(t *testing.T) {
		program := stampProgram(t, `printf("%d/%d",delta(unwrap(ts,10)),ts*(2-ts)*1e30)`, 16, true)
		for _, tt := range []struct {
			raw  uint64
			want string
			bad  bool
		}{{0, "-/-", false}, {1, "?/?", true}, {2, "2/0", false}} {
			text, _, err := stampSample(&program, tt.raw, 16).Render()
			assert.Equal(t, tt.want, text)
			assert.Equal(t, tt.bad, err != nil)
		}
	})
	t.Run("unsafe and nonfinite conversions", func(t *testing.T) {
		for _, expr := range []string{`printf("%d",ts*1e30)`, `printf("%f",ts*1e308)`, `printf("%d",unwrap(-ts,65536))`, `printf("%d",unwrap(ts*1e308,65536))`} {
			program := stampProgram(t, expr, 16, false)
			text, _, err := stampSample(&program, 2, 16).Render()
			require.Error(t, err)
			assert.Equal(t, "?", text)
		}
	})
	t.Run("overflow remains unavailable until reset", func(t *testing.T) {
		program := stampProgram(t, `printf("%d",unwrap(ts,65536,18446744073709551615))`, 16, false)
		other := stampProgram(t, `printf("%d",unwrap(ts,65536,0))`, 16, false)
		_, _, err := stampSample(&program, 0, 16).Render()
		require.NoError(t, err)
		for _, raw := range []uint64{1, 0, 5} {
			_, _, err = stampSample(&program, raw, 16).Render()
			require.ErrorContains(t, err, "overflow")
		}
		text, _, err := stampSample(&other, 5, 16).Render()
		require.NoError(t, err)
		assert.Equal(t, "0", text)
		program.Reset()
		text, _, err = stampSample(&program, 8, 16).Render()
		require.NoError(t, err)
		assert.Equal(t, "18446744073709551615", text)
	})
}

// TestResetStartsOnlyTheSelectedSession verifies counter origins and visible
// delta baselines reset together while another logger continues its own history.
func TestResetStartsOnlyTheSelectedSession(t *testing.T) {
	first := stampProgram(t, `printf("%d",delta(unwrap(ts,10,0)))`, 16, true)
	second := stampProgram(t, `printf("%d",delta(unwrap(ts,10,0)))`, 16, true)
	_, missing, err := stampSample(&first, 8, 16).Render()
	require.NoError(t, err)
	assert.True(t, missing)
	_, missing, err = stampSample(&second, 2, 16).Render()
	require.NoError(t, err)
	assert.True(t, missing)
	text, _, err := stampSample(&first, 9, 16).Render()
	require.NoError(t, err)
	assert.Equal(t, "1", text)
	first.Reset()
	_, missing, err = stampSample(&first, 0, 16).Render()
	require.NoError(t, err)
	assert.True(t, missing)
	text, _, err = stampSample(&first, 1, 16).Render()
	require.NoError(t, err)
	assert.Equal(t, "1", text)
	text, _, err = stampSample(&second, 5, 16).Render()
	require.NoError(t, err)
	assert.Equal(t, "3", text)
}

// TestIntegerDeltaRange checks exact mixed-sign differences without wrapping.
func TestIntegerDeltaRange(t *testing.T) {
	for _, tt := range []struct {
		current, previous Scalar
		want              string
		bad               bool
	}{
		{Scalar{Kind: Unsigned, Unsigned: math.MaxUint64}, Scalar{Kind: Unsigned, Unsigned: math.MaxUint64 - 1}, "1", false},
		{Scalar{Kind: Signed, Signed: math.MinInt64}, Scalar{Kind: Signed, Signed: math.MinInt64 + 1}, "-1", false},
		{Scalar{Kind: Unsigned, Unsigned: 0}, Scalar{Kind: Unsigned, Unsigned: 1 << 63}, "-9223372036854775808", false},
		{Scalar{Kind: Unsigned, Unsigned: 0}, Scalar{Kind: Unsigned, Unsigned: math.MaxUint64}, "", true},
		{Scalar{Kind: Unsigned, Unsigned: math.MaxUint64}, Scalar{Kind: Signed, Signed: -1}, "", true},
	} {
		value, err := subtract(tt.current, tt.previous)
		assert.Equal(t, tt.bad, err != nil)
		if err == nil {
			assert.Equal(t, tt.want, fmtScalar(value))
		}
	}
}

// fmtScalar uses the same checked display layer as real fields.
func fmtScalar(value Scalar) string {
	arg, _ := ArgumentForVerb(value, Verb{Verb: 'd', Class: IntegerVerb})
	return fmt.Sprint(arg)
}
