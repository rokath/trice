// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"testing"

	"github.com/rokath/trice/internal/emitter"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestIDPolicyMatchesCommonAndTagSpecificRanges verifies inclusive boundaries
// and the fallback from an unknown or unconfigured tag to the common range.
func TestIDPolicyMatchesCommonAndTagSpecificRanges(t *testing.T) {
	savedMin, savedMax, savedTags := Min, Max, emitter.Tags
	t.Cleanup(func() {
		Min, Max, emitter.Tags = savedMin, savedMax, savedTags
	})
	Min, Max = 100, 199
	p := &idData{TagList: []TagEntry{{tagName: "ERROR", min: 10, max: 19}, {min: Min, max: Max}}}

	assert.True(t, p.idAllowedForTrice(10, TriceFmt{Strg: "err:failure"}))
	assert.True(t, p.idAllowedForTrice(19, TriceFmt{Strg: "err:failure"}))
	assert.False(t, p.idAllowedForTrice(20, TriceFmt{Strg: "err:failure"}))
	assert.True(t, p.idAllowedForTrice(100, TriceFmt{Strg: "info:status"}))
	assert.False(t, p.idAllowedForTrice(99, TriceFmt{Strg: "info:status"}))
	assert.True(t, p.idAllowedForTrice(150, TriceFmt{Strg: "unknown:status"}))
}

// TestHistoricalPolicyWarningIsDeterministicAndVerboseOnly verifies one
// summary, the smallest offending example, and silence when verbose is off.
func TestHistoricalPolicyWarningIsDeterministicAndVerboseOnly(t *testing.T) {
	savedMin, savedMax, savedVerbose := Min, Max, Verbose
	t.Cleanup(func() { Min, Max, Verbose = savedMin, savedMax, savedVerbose })
	Min, Max = 100, 199
	p := &idData{
		idToTrice: TriceIDLookUp{
			250: {Type: "TRICE", Strg: "err:old"},
			300: {Type: "TRICE", Strg: "err:old2"},
			150: {Type: "TRICE", Strg: "info:valid"},
		},
		TagList: []TagEntry{
			{tagName: "ERROR", min: 10, max: 99},
			{min: Min, max: Max},
		},
	}
	var out bytes.Buffer
	Verbose = false
	p.reportHistoricalPolicyViolations(&out)
	assert.Empty(t, out.String())

	Verbose = true
	p.reportHistoricalPolicyViolations(&out)
	got := out.String()
	assert.Contains(t, got, "2 historical ID(s)")
	assert.Contains(t, got, "example ID 250")
	assert.Contains(t, got, "expected range 10..99")
	assert.Equal(t, 1, bytes.Count([]byte(got), []byte("WARNING:")))

	var noViolations bytes.Buffer
	p.idToTrice = TriceIDLookUp{10: {Type: "TRICE", Strg: "err:valid"}}
	p.reportHistoricalPolicyViolations(&noViolations)
	assert.Empty(t, noViolations.String())
	require.NotNil(t, p.idToTrice)
}
