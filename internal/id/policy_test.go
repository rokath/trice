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

// TestIDRangesUseTheSameTagRecognitionAsCallsites checks CLI range parsing and
// source format classification together. Built-in spellings share one range;
// free labels have independent ranges and an unknown spelling uses the common
// source range, but is rejected when supplied as a CLI range selector.
func TestIDRangesUseTheSameTagRecognitionAsCallsites(t *testing.T) {
	for _, tc := range []struct {
		name, rangeName string
	}{
		{"short lowercase alias", "rx"},
		{"unlisted mixed-case alias", "rX"},
		{"unlisted mixed-case canonical name", "ReCeIvE"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			defer Setup(t)()
			oldTags, oldLabels := emitter.Tags, emitter.UserLabel
			t.Cleanup(func() { emitter.Tags, emitter.UserLabel = oldTags, oldLabels })
			emitter.UserLabel = []string{"new:300", "NEW:650"}
			require.NoError(t, emitter.AddUserLabels())
			Min, Max = 100, 199
			IDData.TagList = nil
			IDRange = ArrayFlag{tc.rangeName + ":10,19", "new:20,29", "NEW:30,39"}
			require.NoError(t, EvaluateIDRangeStrings())
			assert.Equal(t, []TagEntry{
				{tagName: "RECEIVE", min: 10, max: 19},
				{tagName: "new", min: 20, max: 29},
				{tagName: "NEW", min: 30, max: 39},
			}, IDData.TagList)
			for _, spelling := range []string{"rx", "RX", "rX", "ReCeIvE"} {
				format := TriceFmt{Strg: spelling + ":ready"}
				assert.True(t, IDData.idAllowedForTrice(10, format), spelling)
				assert.True(t, IDData.idAllowedForTrice(19, format), spelling)
				assert.False(t, IDData.idAllowedForTrice(20, format), spelling)
				assert.False(t, IDData.idAllowedForTrice(100, format), "recognized tag must not fall back to common range: %s", spelling)
			}
			assert.True(t, IDData.idAllowedForTrice(20, TriceFmt{Strg: "new:ready"}))
			assert.False(t, IDData.idAllowedForTrice(30, TriceFmt{Strg: "new:ready"}))
			assert.True(t, IDData.idAllowedForTrice(30, TriceFmt{Strg: "NEW:ready"}))
			assert.False(t, IDData.idAllowedForTrice(20, TriceFmt{Strg: "NEW:ready"}))
			assert.True(t, IDData.idAllowedForTrice(100, TriceFmt{Strg: "NeW:ready"}))
			assert.False(t, IDData.idAllowedForTrice(20, TriceFmt{Strg: "NeW:ready"}))
			for _, invalid := range []struct{ rule, reason string }{
				{"Rx:40,49", "already an assigned ID range"},
				{"NeW:40,49", "applied name NeW is unknown"},
			} {
				before := append([]TagEntry(nil), IDData.TagList...)
				IDRange = ArrayFlag{invalid.rule}
				err := EvaluateIDRangeStrings()
				require.Error(t, err)
				assert.Contains(t, err.Error(), invalid.reason)
				assert.Equal(t, before, IDData.TagList, "a rejected rule must not change previously configured ranges")
			}
		})
	}
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
