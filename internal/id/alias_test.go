// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"fmt"
	"io"
	"path/filepath"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/emitter"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestAliasFrozenNames protects the persisted meaning, including cross-group
// exclusions and partially overlapping longer positive occurrences.
func TestAliasFrozenNames(t *testing.T) {
	positiveCount, exclusionCount := 0, 0
	for _, rule := range aliasTagRules {
		positiveCount += len(strings.Fields(rule.positives))
		exclusionCount += len(strings.Fields(rule.exclusions))
	}
	assert.Equal(t, 18, positiveCount)
	assert.Equal(t, 144, exclusionCount)
	for _, rule := range aliasTagRules {
		for _, word := range strings.Fields(rule.positives) {
			t.Run(word, func(t *testing.T) {
				tag, _ := aliasNameTag("Log_" + strings.ToUpper(word))
				assert.Equal(t, rule.tag, tag)
			})
		}
		for _, word := range strings.Fields(rule.exclusions) {
			t.Run(word, func(t *testing.T) { tag, _ := aliasNameTag(word); assert.Empty(t, tag) })
		}
	}
	for name, expected := range map[string]string{"Log_Error_Warning": "WARNING", "LogErrWrn": "ERROR", "CriticalLog": "CRITICAL", "CriticalSectionError": "ERROR", "InfrastructureInfo": "INFO", "ReadbackDebug": "DEBUG", "TransferTrace": "TRACE", "TrialTrace": "TRACE", "TrialMode": "", "nonfatalFatal": "FATAL"} {
		tag, _ := aliasNameTag(name)
		assert.Equal(t, expected, tag, name)
	}
}

// TestAliasCacheCannotRestoreObsoleteIdentity plants warm timestamp cache
// entries. Alias commands deliberately reparse without migrating old caches.
func TestAliasCacheCannotRestoreObsoleteIdentity(t *testing.T) {
	const source = `A("ready");`
	defer prepareBindTest(t, map[string]string{"main.c": source})()
	TriceCacheEnabled = true
	TriceAliases = ArrayFlag{"A"}
	require.NoError(t, ProcessAliases())
	fullPath, err := filepath.Abs(Srcs[0])
	require.NoError(t, err)
	fullPath = strings.TrimPrefix(fullPath, filepath.VolumeName(fullPath))
	plant := func(folder, content string) {
		name := filepath.Join(UserHomeDir, ".trice/cache", folder, fullPath)
		require.NoError(t, FSys.MkdirAll(filepath.Dir(name), 0o755))
		require.NoError(t, FSys.WriteFile(name, []byte(content), 0o644))
		info, err := FSys.Stat(Srcs[0])
		require.NoError(t, err)
		require.NoError(t, FSys.Chtimes(name, info.ModTime(), info.ModTime()))
	}
	plant(insertedCacheFolderName, source)
	plant(cleanedCacheFolderName, source)
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	lu := NewLut(io.Discard, FSys, FnJSON)
	require.Len(t, lu, 1)
	for _, raw := range lu {
		assert.Equal(t, "A", raw.Type)
	}
	// A matching cleaned-cache timestamp used to skip the actual ID removal.
	plant(cleanedCacheFolderName, source)
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	cleaned, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, source, string(cleaned))
}

// TestAliasBuildTagRanges uses name/override classification rather than a
// conflicting format prefix, and retains original tags outside identity keys.
func TestAliasBuildTagRanges(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"main.c": `MyWrn("err:failed");`})()
	TriceAliases = ArrayFlag{"MyWrn"}
	require.NoError(t, ProcessAliases())
	require.NoError(t, emitter.AddUserTagNames())
	IDRange = ArrayFlag{"wrn:300,309"}
	require.NoError(t, EvaluateIDRangeStrings())
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	lu := NewLut(io.Discard, FSys, FnJSON)
	require.Len(t, lu, 1)
	assert.Equal(t, "MyWrn", lu[300].Type)
	policy := idData{TagList: []TagEntry{{tagName: "untagged", min: 100, max: 110}, {tagName: "ERROR", min: 200, max: 210}}}
	raw := TriceFmt{Type: "A", Strg: "err:post CE"}
	assert.True(t, policy.idAllowedForTrice(100, raw, "untagged"))
	assert.False(t, policy.idAllowedForTrice(100, raw))
}

// TestAliasSourceIdentityAndReaders exercises both source workflows against a
// mixed historical table, including ID reuse, exact spelling and a kind change.
func TestAliasSourceIdentityAndReaders(t *testing.T) {
	for _, bind := range []bool{false, true} {
		t.Run(fmt.Sprint("bind=", bind), func(t *testing.T) {
			const source = "void f(void) { A(\"err:value=%d\", 7); FooS(\"value=%d\", 8); My32(\"value=%d\", 9); MyAssert(\"value=%d\", 10); S(\"wrn:value=%s\", text); }\n"
			defer prepareBindTest(t, map[string]string{"main.c": source})()
			TriceAliases, TriceSAliases = ArrayFlag{"A", "FooS", "My32", "MyAssert"}, ArrayFlag{"S"}
			require.NoError(t, ProcessAliases())
			require.NoError(t, emitter.AddUserTagNames())
			old := TriceIDLookUp{100: {Type: "trice", Strg: "err:value=%d"}}
			require.NoError(t, old.toFile(FSys.Fs, FnJSON))
			run := SubCmdIdInsert
			if bind {
				run = SubCmdIdBind
			}
			require.NoError(t, run(io.Discard, FSys))
			first := NewLut(io.Discard, FSys, FnJSON)
			assert.Equal(t, old[100], first[100])
			require.Len(t, first, 6)
			seen := map[string]bool{}
			for n, raw := range first {
				if n == 100 {
					continue
				}
				seen[raw.Type] = true
				assert.Empty(t, raw.Alias)
				payload, err := PayloadFormat(raw)
				require.NoError(t, err)
				if raw.Type == "S" {
					assert.Equal(t, "triceS", payload.Type)
				} else {
					assert.Equal(t, "trice", payload.Type)
				}
			}
			assert.Equal(t, map[string]bool{"A": true, "FooS": true, "My32": true, "MyAssert": true, "S": true}, seen)
			selected, selectErr := selectCurrentLogEntries(io.Discard, FSys, first)
			require.NoError(t, selectErr)
			require.Len(t, selected, 5)
			require.NoError(t, run(io.Discard, FSys))
			assert.Equal(t, first, NewLut(io.Discard, FSys, FnJSON))
			// Readers must use the native view without changing raw exports or keys.
			for _, raw := range first {
				_, _, _ = computeValues(raw, 32)
				_, _, _ = computeLogValues(raw, 32)
			}
			c, err := first.toListTilC("til.c")
			require.NoError(t, err)
			assert.Contains(t, string(c), "TRICE_LOG_PARAM_COUNT_DYNAMIC_STRING")
			assert.NotContains(t, string(c), SAliasStrgPrefix)
			require.NoError(t, SubCmdIdClean(io.Discard, FSys))
			cleaned, err := FSys.ReadFile(Srcs[0])
			require.NoError(t, err)
			if !bind {
				assert.Equal(t, source, string(cleaned))
			}
		})
	}
}

// TestAliasIdentityChangesPreserveHistory keeps case renames and payload-family
// changes distinct even when their visible format or arguments happen to agree.
func TestAliasIdentityChangesPreserveHistory(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"main.c": `A("same");`})()
	TriceAliases = ArrayFlag{"A", "a"}
	require.NoError(t, ProcessAliases())
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	first := NewLut(io.Discard, FSys, FnJSON)
	require.NoError(t, FSys.WriteFile(Srcs[0], []byte(`a("same");`), 0o644))
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	second := NewLut(io.Discard, FSys, FnJSON)
	require.Len(t, second, len(first)+1)
	for n, raw := range first {
		assert.Equal(t, raw, second[n])
	}
	TriceAliases, TriceSAliases = nil, ArrayFlag{"a"}
	require.NoError(t, ProcessAliases())
	require.NoError(t, FSys.WriteFile(Srcs[0], []byte(`a("same");`), 0o644))
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	third := NewLut(io.Discard, FSys, FnJSON)
	require.Len(t, third, len(second)+1)
	for n, raw := range second {
		assert.Equal(t, raw, third[n])
	}
}

// TestAliasValidationAndVerbose tests reserved boundaries before publication
// and verifies that unused options create diagnostics only when requested.
func TestAliasValidationAndVerbose(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"main.c": `A("SAlias_Strg('reserved");`})()
	TriceAliases = ArrayFlag{"A", "Unused:sensor"}
	require.NoError(t, ProcessAliases())
	before, _ := FSys.ReadFile(Srcs[0])
	var diagnostic bytes.Buffer
	assert.Error(t, SubCmdIdInsert(&diagnostic, FSys))
	assert.Contains(t, diagnostic.String(), "reserved SAlias boundary")
	after, _ := FSys.ReadFile(Srcs[0])
	assert.Equal(t, before, after)
	var output bytes.Buffer
	ExplainAliases(&output, nil)
	assert.Empty(t, output.String())
	Verbose = true
	ExplainAliases(&output, TriceIDLookUp{1: {Type: "MyWrn", Strg: "err:x"}, 2: {Type: "InfrastructureInfo", Strg: "x"}})
	assert.Contains(t, output.String(), "valid unused registration")
	assert.Contains(t, output.String(), "conflicting static prefix")
	assert.Contains(t, output.String(), "excluded:")
}

// TestAliasRegistration rejects invalid definitions before scanning and merges
// equivalent assignments independently of the argument order.
func TestAliasRegistration(t *testing.T) {
	oldA, oldS := TriceAliases, TriceSAliases
	t.Cleanup(func() { TriceAliases, TriceSAliases = oldA, oldS; _ = ProcessAliases(); _ = emitter.AddUserLabels() })
	for _, values := range [][]string{{"A", "A:err", "A:ERROR"}, {"A:sensor", "A:SENSOR", "A"}} {
		TriceAliases, TriceSAliases = values, nil
		assert.NoError(t, ProcessAliases())
		assert.NoError(t, emitter.AddUserLabels(AliasTagNames()...))
		assert.True(t, matchAnyTriceStart.MatchString(`A("x")`))
		assert.False(t, matchAnyTriceStart.MatchString(`a("x")`))
	}
	for _, values := range [][]string{{""}, {"bad-name"}, {"TriceWarning"}, {"A:"}, {"A:all"}, {"A:err", "A:wrn"}} {
		TriceAliases, TriceSAliases = values, nil
		assert.Error(t, ProcessAliases(), values)
	}
	TriceAliases, TriceSAliases = []string{"FooS"}, []string{"FooS"}
	assert.Error(t, ProcessAliases())
}

// TestAliasPayloadAndPrecedence separates raw identity, payload and event tags.
func TestAliasPayloadAndPrecedence(t *testing.T) {
	oldA, oldS := TriceAliases, TriceSAliases
	t.Cleanup(func() { TriceAliases, TriceSAliases = oldA, oldS; _ = ProcessAliases(); _ = emitter.AddUserLabels() })
	TriceAliases, TriceSAliases = []string{"A:sensor", "MyWrn:untagged"}, nil
	assert.NoError(t, ProcessAliases())
	assert.NoError(t, emitter.AddUserLabels(AliasTagNames()...))
	for _, test := range []struct {
		raw          TriceFmt
		payload, tag string
	}{
		{TriceFmt{Type: "A", Strg: "err:x"}, "trice", "SENSOR"},
		{TriceFmt{Type: "MyWrn", Strg: "err:x"}, "trice", "untagged"},
		{TriceFmt{Type: "FooS", Strg: "err:x"}, "trice", "ERROR"},
		{TriceFmt{Type: "My32", Strg: "ctx:err:x"}, "trice", "untagged"},
		{TriceFmt{Type: "MyAssert", Strg: SAliasStrgPrefix + `condition, "err:x"` + SAliasStrgSuffix}, "triceS", "untagged"},
		{TriceFmt{Type: "S", Strg: SAliasStrgPrefix + `"err:x"` + SAliasStrgSuffix}, "triceS", "ERROR"},
	} {
		payload, err := PayloadFormat(test.raw)
		assert.NoError(t, err)
		assert.Equal(t, test.payload, payload.Type)
		assert.Equal(t, test.tag, ResolveAliasEvent(test.raw).Tag)
		assert.Equal(t, test.raw, storedTriceFormat(payload))
	}
	for _, format := range []string{SAliasStrgPrefix + "x", "x" + SAliasStrgSuffix} {
		_, err := PayloadFormat(TriceFmt{Type: "A", Strg: format})
		assert.Error(t, err)
	}
}
