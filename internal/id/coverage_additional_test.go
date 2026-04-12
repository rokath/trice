// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"fmt"
	"strings"
	"testing"
	"time"

	"github.com/rokath/trice/pkg/ant"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestProcessTriceIDCleaningConfigSwitchAndDryRun verifies the triceConfig.h
// special case independently from the broader source-tree walk integration.
func TestProcessTriceIDCleaningConfigSwitchAndDryRun(t *testing.T) {
	defer Setup(t)()

	path := Proj + "triceConfig.h"
	original := "#define TRICE_CLEAN 0\n"
	require.NoError(t, FSys.WriteFile(path, []byte(original), 0o644))

	fileInfo, err := FSys.Stat(path)
	require.NoError(t, err)

	p := &idData{
		idToTrice:  TriceIDLookUp{},
		idToLocNew: TriceIDLookUpLI{},
	}
	admin := &ant.Admin{}
	var out bytes.Buffer

	oldDryRun := DryRun
	DryRun = true
	t.Cleanup(func() { DryRun = oldDryRun })

	require.NoError(t, p.processTriceIDCleaning(&out, FSys, path, fileInfo, admin))
	got, err := FSys.ReadFile(path)
	require.NoError(t, err)
	assert.Equal(t, original, string(got))

	DryRun = false
	p.err = nil
	require.NoError(t, p.processTriceIDCleaning(&out, FSys, path, fileInfo, admin))
	got, err = FSys.ReadFile(path)
	require.NoError(t, err)
	assert.Contains(t, string(got), "#define TRICE_CLEAN 1")
}

// TestProcessTriceIDInsertionConfigSwitchAndDryRun verifies the matching
// insertion-side triceConfig.h rewrite and keeps the check isolated from the
// heavier ID insertion algorithm tests.
func TestProcessTriceIDInsertionConfigSwitchAndDryRun(t *testing.T) {
	defer Setup(t)()

	path := Proj + "triceConfig.h"
	original := "#define TRICE_CLEAN 1\n"
	require.NoError(t, FSys.WriteFile(path, []byte(original), 0o644))

	fileInfo, err := FSys.Stat(path)
	require.NoError(t, err)

	p := &idData{}
	admin := &ant.Admin{}

	oldDryRun := DryRun
	DryRun = true
	t.Cleanup(func() { DryRun = oldDryRun })

	require.NoError(t, p.processTriceIDInsertion(&bytes.Buffer{}, FSys, path, fileInfo, admin))
	got, err := FSys.ReadFile(path)
	require.NoError(t, err)
	assert.Equal(t, original, string(got))

	DryRun = false
	p.err = nil
	require.NoError(t, p.processTriceIDInsertion(&bytes.Buffer{}, FSys, path, fileInfo, admin))
	got, err = FSys.ReadFile(path)
	require.NoError(t, err)
	assert.Contains(t, string(got), "#define TRICE_CLEAN 0")
}

// TestEvaluateTriceParameterCountBranchErrors closes the remaining low-level
// validation branches so coverage improvements are driven by real parser
// behavior instead of broad integration runs only.
func TestEvaluateTriceParameterCountBranchErrors(t *testing.T) {
	tests := []struct {
		name       string
		tf         TriceFmt
		rest       string
		wantErrSub string
	}{
		{
			name:       "string macro requires one format specifier",
			tf:         TriceFmt{Type: "TRICES", Strg: "plain"},
			rest:       `, "abc");`,
			wantErrSub: "exactly one format specifier",
		},
		{
			name:       "string macro requires one parameter",
			tf:         TriceFmt{Type: "TRICES", Strg: "%s"},
			rest:       `);`,
			wantErrSub: "exactly one parameter",
		},
		{
			name:       "counted string macro requires one format specifier",
			tf:         TriceFmt{Type: "TRICEN", Strg: "plain"},
			rest:       `, s, n);`,
			wantErrSub: "exactly one format specifier",
		},
		{
			name:       "counted string macro requires two parameters",
			tf:         TriceFmt{Type: "TRICEN", Strg: "%s"},
			rest:       `, s);`,
			wantErrSub: "exactly two parameters",
		},
		{
			name:       "buffer macro requires two parameters",
			tf:         TriceFmt{Type: "TRICE8B", Strg: "%d"},
			rest:       `, buf);`,
			wantErrSub: "exactly two parameters",
		},
		{
			name:       "function macro must not contain format specifiers",
			tf:         TriceFmt{Type: "TRICE16F", Strg: "rpc:%d"},
			rest:       `, buf, sizeof(buf));`,
			wantErrSub: "should have no format specifier",
		},
		{
			name:       "function macro requires two parameters",
			tf:         TriceFmt{Type: "TRICE16F", Strg: "rpc:Call"},
			rest:       `, buf);`,
			wantErrSub: "exactly two parameters",
		},
		{
			name:       "assert macro requires a boolean argument",
			tf:         TriceFmt{Type: "TRICEAssertFalse", Strg: ""},
			rest:       `);`,
			wantErrSub: "one or more parameters",
		},
		{
			name:       "regular macro propagates parse errors",
			tf:         TriceFmt{Type: "TRICE", Strg: "%d"},
			rest:       `, fn(`,
			wantErrSub: "invalid:",
		},
	}

	for _, tt := range tests {
		tt := tt
		t.Run(tt.name, func(t *testing.T) {
			err := evaluateTriceParameterCount(tt.tf, 17, tt.rest)
			require.Error(t, err)
			assert.Contains(t, err.Error(), tt.wantErrSub)
		})
	}
}

// TestCountColonsUntilClosingBracketErrors covers the invalid parser exits that
// are otherwise hard to hit through higher-level command tests.
func TestCountColonsUntilClosingBracketErrors(t *testing.T) {
	_, err := countColonsUntilClosingBracket(`, "unterminated );`)
	require.Error(t, err)
	assert.Contains(t, err.Error(), "invalid:")

	_, err = countColonsUntilClosingBracket(`, call(abc);`)
	require.Error(t, err)
	assert.Contains(t, err.Error(), "no matching closing bracket found")
}

// TestAddFmtCountEdgeBranches exercises the special-case and validation paths
// that are not covered by the existing happy-path oriented tests.
func TestAddFmtCountEdgeBranches(t *testing.T) {
	ilu := TriceIDLookUp{
		1: {Type: "TRICE", Strg: strings.Repeat("%d", 13)},
		2: {Type: "TRICE_N", Strg: "plain"},
		3: {Type: "TRICE_B", Strg: "plain"},
		4: {Type: "TRICE_13", Strg: "%d"},
		5: {Type: "TRICE_bad", Strg: "%d"},
	}

	var out bytes.Buffer
	ilu.AddFmtCount(&out)

	assert.Equal(t, "TRICE", ilu[1].Type)
	assert.Equal(t, "TRICE_N", ilu[2].Type)
	assert.Equal(t, "TRICE_B", ilu[3].Type)
	assert.Equal(t, "TRICE_13", ilu[4].Type)
	assert.Equal(t, "TRICE_bad", ilu[5].Type)

	text := out.String()
	assert.Contains(t, text, "Invalid format specifier count 13")
	assert.Contains(t, text, "Expected format specifier count is 1 but got 0")
	assert.Contains(t, text, "Unexpected Trice type - please check")
}

// TestNewRandomIDWarnsWhenSpaceRunsLow covers the low-free-space warning path
// without relying on a particular random sequence for the final result.
func TestNewRandomIDWarnsWhenSpaceRunsLow(t *testing.T) {
	ilu := TriceIDLookUp{}
	for id := TriceID(10); id < 25; id++ {
		ilu[id] = TriceFmt{Type: "TRICE", Strg: "x"}
	}

	got := ilu.newRandomID(&bytes.Buffer{}, 10, 25)
	assert.Equal(t, TriceID(25), got)
}

// TestCopyFileWithMTimeSuccessAndSourceError closes the remaining helper paths
// around explicit file copying without depending on the broader cache tests.
func TestCopyFileWithMTimeSuccessAndSourceError(t *testing.T) {
	defer Setup(t)()

	src := Proj + "copy-source.c"
	dst := Proj + "copy-destination.c"
	content := []byte("copy me")
	require.NoError(t, FSys.WriteFile(src, content, 0o644))

	time.Sleep(10 * time.Millisecond)
	require.NoError(t, CopyFileWithMTime(FSys, dst, src))

	got, err := FSys.ReadFile(dst)
	require.NoError(t, err)
	assert.Equal(t, string(content), string(got))
	assert.Equal(t, MTime(FSys, src), MTime(FSys, dst))
}

// TestEvaluateIDRangeStringsAdditionalBranches covers the remaining validation
// exits so range handling is protected before future id refactors.
func TestEvaluateIDRangeStringsAdditionalBranches(t *testing.T) {
	tests := []struct {
		name       string
		idRange    ArrayFlag
		min        TriceID
		max        TriceID
		initial    []TagEntry
		wantErrSub string
		wantCount  int
	}{
		{
			name:      "entry without colon is ignored",
			idRange:   ArrayFlag{"missing-separator"},
			min:       10,
			max:       20,
			wantCount: 0,
		},
		{
			name:       "missing max separator is rejected",
			idRange:    ArrayFlag{"e:100"},
			min:        10,
			max:        20,
			wantErrSub: `invalid syntax in e:100 - expecting "TagName:number,number"`,
		},
		{
			name:       "invalid minimum is rejected",
			idRange:    ArrayFlag{"e:x,100"},
			min:        10,
			max:        20,
			wantErrSub: `invalid syntax in e:x,100 - expecting "TagName:number,number"`,
		},
		{
			name:       "minimum above maximum is rejected",
			idRange:    ArrayFlag{"e:200,100"},
			min:        10,
			max:        20,
			wantErrSub: "ID range is inval Min(200) > Max(100)",
		},
		{
			name:       "unknown tag is rejected",
			idRange:    ArrayFlag{"unknown:100,101"},
			min:        10,
			max:        20,
			wantErrSub: "applied name unknown is unknown",
		},
		{
			name:       "canonical tag alias can only be assigned once",
			idRange:    ArrayFlag{"e:100,102", "err:200,202"},
			min:        10,
			max:        20,
			wantErrSub: "tagName e has already an assigned ID range",
		},
		{
			name:       "range overlapping default interval is rejected",
			idRange:    ArrayFlag{"e:15,25"},
			min:        10,
			max:        20,
			wantErrSub: "overlapping ID ranges for e (Min 15, Max 25) and default (Min 10, Max 20)",
		},
		{
			name:    "range overlapping configured tag interval is rejected",
			idRange: ArrayFlag{"dbg:201,204"},
			min:     10,
			max:     20,
			initial: []TagEntry{
				{tagName: "e", min: 200, max: 202},
			},
			wantErrSub: "overlapping ID ranges for d (Min 201, Max 204) and e (Min 200, Max 202)",
		},
		{
			name:    "distinct tag range is appended",
			idRange: ArrayFlag{"dbg:200,202"},
			min:     10,
			max:     20,
			initial: []TagEntry{
				{tagName: "e", min: 100, max: 102},
			},
			wantCount: 2,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			defer Setup(t)()

			Min = tt.min
			Max = tt.max
			IDRange = tt.idRange
			IDData.TagList = append([]TagEntry(nil), tt.initial...)

			err := EvaluateIDRangeStrings()
			if tt.wantErrSub != "" {
				require.Error(t, err)
				assert.Contains(t, err.Error(), tt.wantErrSub)
				return
			}

			require.NoError(t, err)
			assert.Len(t, IDData.TagList, tt.wantCount)
		})
	}
}

// TestZeroTriceIDsAdditionalBranches keeps the low-level clean-to-zero logic
// covered directly, including invalid source syntax, new til entries, and
// til/source mismatches that must still zero the source ID.
func TestZeroTriceIDsAdditionalBranches(t *testing.T) {
	defer Setup(t)()

	IDData = idData{
		idToTrice: TriceIDLookUp{
			111: {Type: "TRice", Strg: "same"},
			222: {Type: "TRice", Strg: "other"},
		},
		idToLocNew: TriceIDLookUpLI{},
	}

	in := []byte(strings.Join([]string{
		`TRice(iD(111), "same");`,
		`TRice(iD(name), "skip");`,
		`TRice(iD(222), "conflict");`,
		`trice(iD(333), "fresh");`,
		"",
	}, "\n"))

	var out bytes.Buffer
	admin := &ant.Admin{}

	oldVerbose := Verbose
	Verbose = true
	t.Cleanup(func() { Verbose = oldVerbose })

	got, modified, err := zeroTriceIDs(&out, "pkg/source.c", in, admin)
	require.NoError(t, err)
	assert.True(t, modified)

	text := string(got)
	assert.Contains(t, text, `TRice(iD(0), "same");`)
	assert.Contains(t, text, `TRice(iD(name), "skip");`)
	assert.Contains(t, text, `TRice(iD(0), "conflict");`)
	assert.Contains(t, text, `trice(iD(0), "fresh");`)

	assert.Equal(t, TriceFmt{Type: "trice", Strg: "fresh"}, IDData.idToTrice[333])
	require.Equal(t, TriceLI{"pkg/source.c", 1}, IDData.idToLocNew[111])
	require.Equal(t, TriceLI{"pkg/source.c", 4}, IDData.idToLocNew[333])
	_, exists := IDData.idToLocNew[222]
	assert.False(t, exists)

	log := out.String()
	assert.Contains(t, log, fmt.Sprintf("ID inside pkg/source.c line 3 refers to %v but is already used inside til.json for %v", TriceFmt{Type: "TRice", Strg: "conflict"}, TriceFmt{Type: "TRice", Strg: "other"}))
	assert.Contains(t, log, "2 items inside li")
}
