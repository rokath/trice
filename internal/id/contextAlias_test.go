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
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// contextWarningOrderFS observes actual publication rather than private
// planning writes, which must already have emitted the fallback diagnostic.
type contextWarningOrderFS struct {
	afero.Fs
	beforeRename func()
}

// Rename checks the visible diagnostic before a final file becomes visible.
func (fs *contextWarningOrderFS) Rename(oldName, newName string) error {
	fs.beforeRename()
	return fs.Fs.Rename(oldName, newName)
}

// TestAliasContextWarningBeforePublication guards the ordering for both source
// workflows, including dictionary and generated-header publication.
func TestAliasContextWarningBeforePublication(t *testing.T) {
	for _, command := range []string{"bind", "insert"} {
		t.Run(command, func(t *testing.T) {
			defer prepareSourceContextTest(t, map[string]string{"main.c": "printi(\"ctx:err:failed\");\n"}, `ctx:" [checked]"`)()
			TriceAliases = ArrayFlag{"printi"}
			require.NoError(t, ProcessAliases())
			require.NoError(t, emitter.AddUserTagNames())
			var output bytes.Buffer
			publications := 0
			fs := &afero.Afero{Fs: &contextWarningOrderFS{Fs: FSys.Fs, beforeRename: func() { publications++; assert.Equal(t, 1, strings.Count(output.String(), "CE fallback changes")) }}}
			run := SubCmdIdInsert
			if command == "bind" {
				run = SubCmdIdBind
			}
			require.NoError(t, run(&output, fs), output.String())
			assert.Positive(t, publications)
		})
	}
}

// TestAliasContextPartialScan retains IDs and source bytes belonging to an
// unselected alias site while keeping old formats available for old firmware.
func TestAliasContextPartialScan(t *testing.T) {
	defer prepareSourceContextTest(t, map[string]string{"a.c": "MyWrn(\"value=%d\\n\", 1);\n", "b.c": "MyWrn(\"value=%d\\n\", 2);\n"}, `wrn:" {context}", context`)()
	TriceAliases = ArrayFlag{"MyWrn"}
	require.NoError(t, ProcessAliases())
	require.NoError(t, emitter.AddUserTagNames())
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	first := contextTestTIL(t)
	a := filepath.Join(Proj, t.Name(), "a.c")
	b := filepath.Join(Proj, t.Name(), "b.c")
	excluded, err := FSys.ReadFile(b)
	require.NoError(t, err)
	Srcs = ArrayFlag{a}
	require.NoError(t, FSys.WriteFile(a, []byte("MyWrn(\"changed=%d\\n\", 3);\n"), 0o644))
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	after, err := FSys.ReadFile(b)
	require.NoError(t, err)
	assert.Equal(t, excluded, after)
	current := contextTestTIL(t)
	require.Len(t, current, 3)
	for n, entry := range first {
		assert.Equal(t, entry, current[n])
	}
}

// TestAliasContextVirtualTagSuffix keeps an initially empty format reversible
// when fixed CE text itself introduces the host's first registered prefix.
func TestAliasContextVirtualTagSuffix(t *testing.T) {
	for _, command := range []string{"bind", "insert"} {
		t.Run(command, func(t *testing.T) {
			const source = "printi(\"\");\n"
			defer prepareSourceContextTest(t, map[string]string{"main.c": source}, `untagged:"err:checked"`)()
			TriceAliases = ArrayFlag{"printi"}
			require.NoError(t, ProcessAliases())
			require.NoError(t, emitter.AddUserTagNames())
			IDRange = ArrayFlag{"untagged:300,309", "err:400,409"}
			require.NoError(t, EvaluateIDRangeStrings())
			run := SubCmdIdInsert
			if command == "bind" {
				run = SubCmdIdBind
			}
			for repeat := 0; repeat < 2; repeat++ {
				var output bytes.Buffer
				require.NoError(t, run(&output, FSys), output.String())
				assert.Equal(t, 1, strings.Count(output.String(), "CE fallback changes"))
				assert.Equal(t, TriceIDLookUp{300: {Type: "printi", Strg: "err:checked"}}, contextTestTIL(t))
			}
			if command == "insert" {
				var output bytes.Buffer
				require.NoError(t, SubCmdIdClean(&output, FSys))
				assert.Equal(t, 1, strings.Count(output.String(), "CE fallback changes"))
				restored, err := FSys.ReadFile(Srcs[0])
				require.NoError(t, err)
				assert.Equal(t, source, string(restored))
			}
		})
	}
}

// TestAliasContextSelectionOrder locks virtual-first selection and canonical
// deduplication while retaining conflicting known prefixes as message text.
func TestAliasContextSelectionOrder(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"main.c": ""})()
	TriceAliases = ArrayFlag{"MyWrn", "A:sensor"}
	require.NoError(t, ProcessAliases())
	require.NoError(t, emitter.AddUserTagNames(AliasTagNames()...))
	rules, err := parseContextRules([]string{`ctx:" C"`, `err:" E"`, `wrn:" W1"`, `WARNING:" W2"`, `SENSOR:" S"`})
	require.NoError(t, err)
	format, selected, duplicates := selectTriceContextRules(TriceFmt{Type: "MyWrn", Strg: "err:ctx:ready"}, rules)
	assert.Equal(t, "err:ready", format)
	assert.Empty(t, duplicates)
	var extensions []string
	for _, rule := range selected {
		extensions = append(extensions, rule.format)
	}
	assert.Equal(t, []string{" W1", " W2", " E", " C"}, extensions)
	format, selected, duplicates = selectTriceContextRules(TriceFmt{Type: "A", Strg: "Sensor:SENSOR:sensor:ready"}, rules)
	assert.Equal(t, "Sensor:SENSOR:sensor:ready", format)
	require.Len(t, selected, 1)
	assert.Equal(t, " S", selected[0].format)
	assert.Len(t, duplicates, 2)
}

// TestAliasContextWorkflows covers the accepted warning table through fresh,
// repeated and dry-run Bind/Insert, then matching Clean with cache enabled.
func TestAliasContextWorkflows(t *testing.T) {
	for _, command := range []string{"bind", "insert"} {
		for _, cache := range []bool{false, true} {
			for _, test := range []struct {
				name, format, registration, stored, buildTag, hostTag string
				registered                                            bool
				warning                                               bool
			}{
				{"fallback divergence", "ctx:err:failed", "printi", "err:failed [checked]", "untagged", "ERROR", false, true},
				{"already exposed tag", "err:failed [checked]", "printi", "err:failed [checked]", "ERROR", "ERROR", false, false},
				{"enduring prefix", "untagged:ctx:err:failed", "printi", "untagged:err:failed [checked]", "untagged", "untagged", false, false},
				{"no exposed tag", "ctx:failed", "printi", "failed [checked]", "untagged", "untagged", false, false},
				{"explicit override", "ctx:err:failed", "printi:untagged", "err:failed [checked]", "untagged", "untagged", false, false},
				{"name match", "ctx:err:failed", "MyWrn", "err:failed [checked]", "WARNING", "WARNING", false, false},
				{"registered first tag", "ctx:err:failed", "printi", "ctx:err:failed [checked]", "CTX", "CTX", true, false},
			} {
				t.Run(fmt.Sprintf("%s/cache=%t/%s", command, cache, test.name), func(t *testing.T) {
					name, _, _ := strings.Cut(test.registration, ":")
					source := "#include \"trice.h\"\n" + name + `("` + test.format + `");` + "\n"
					defer prepareSourceContextTest(t, map[string]string{"main.c": source}, `ctx:" [checked]"`)()
					TriceCacheEnabled = cache
					TriceAliases = ArrayFlag{test.registration}
					require.NoError(t, ProcessAliases())
					names := AliasTagNames()
					if test.registered {
						names = append(names, "ctx")
					}
					require.NoError(t, emitter.AddUserTagNames(names...))
					IDRange = ArrayFlag{"untagged:300,309", "err:400,409", "wrn:500,509"}
					if test.registered {
						IDRange = append(IDRange, "ctx:600,609")
					}
					require.NoError(t, EvaluateIDRangeStrings())
					legacy := TriceFmt{Type: "trice", Strg: "legacy"}
					require.NoError(t, (TriceIDLookUp{100: legacy}).toFile(FSys.Fs, FnJSON))
					run := SubCmdIdInsert
					if command == "bind" {
						run = SubCmdIdBind
					}
					checkWarning := func(output string) {
						t.Helper()
						count := 0
						if test.warning {
							count = 1
							line := 2
							if command == "bind" {
								line = 3
							}
							assert.Contains(t, output, fmt.Sprintf("main.c:%d:", line))
							assert.Contains(t, output, "alias "+name)
							assert.Contains(t, output, "build tag "+test.buildTag+" to host tag "+test.hostTag)
							assert.Contains(t, output, "untagged:")
							assert.Contains(t, output, "build and logging")
						}
						assert.Equal(t, count, strings.Count(output, "CE fallback changes"))
					}
					before := contextTestSnapshot(t)
					DryRun = true
					var output bytes.Buffer
					require.NoError(t, run(&output, FSys), output.String())
					checkWarning(output.String())
					assert.Equal(t, before, contextTestSnapshot(t))
					DryRun = false
					output.Reset()
					require.NoError(t, run(&output, FSys), output.String())
					checkWarning(output.String())
					til := contextTestTIL(t)
					require.Len(t, til, 2)
					assert.Equal(t, legacy, til[100])
					wantID := map[string]TriceID{"untagged": 300, "ERROR": 400, "WARNING": 500, "CTX": 600}[test.buildTag]
					want := TriceFmt{Type: name, Strg: test.stored}
					assert.Equal(t, want, til[wantID])
					assert.Equal(t, test.hostTag, ResolveAliasEvent(til[wantID]).Tag)
					first := contextTestSnapshot(t)
					output.Reset()
					require.NoError(t, run(&output, FSys))
					checkWarning(output.String())
					assert.Equal(t, first, contextTestSnapshot(t))
					current, err := selectCurrentLogEntries(io.Discard, FSys, til)
					require.NoError(t, err)
					assert.Equal(t, TriceIDLookUp{wantID: want}, current)
					if command == "insert" {
						output.Reset()
						require.NoError(t, SubCmdIdClean(&output, FSys), output.String())
						checkWarning(output.String())
						cleaned, err := FSys.ReadFile(Srcs[0])
						require.NoError(t, err)
						assert.Equal(t, source, string(cleaned))
						assert.Equal(t, til, contextTestTIL(t))
						output.Reset()
						require.NoError(t, SubCmdIdClean(&output, FSys))
						assert.NotContains(t, output.String(), "CE fallback changes")
					}
				})
			}
		}
	}
}

// TestAliasContextRejectsStringAliases verifies virtual and first-argument
// selection, full-project preflight and preservation of ordinary SAlias calls.
func TestAliasContextRejectsStringAliases(t *testing.T) {
	for _, command := range []string{"bind", "insert", "clean"} {
		for _, test := range []struct {
			alias, call, rule string
			rejected          bool
		}{
			{"MyWrn", `MyWrn("value=%d", 7);`, `WARNING:" [fixed]"`, true},
			{"MyWrn", `MyWrn("value=%d", 7);`, `wrn:" {count}", count`, true},
			{"A", `A("wrn:value=%d", 7);`, `wrn:" [fixed]"`, true},
			{"A", `A(condition, "wrn:value=%d", 7);`, `wrn:" [fixed]"`, false},
			{"A", `A("value=%d", 7);`, `ctx:" [fixed]"`, false},
		} {
			t.Run(command+"/"+test.call+test.rule, func(t *testing.T) {
				defer prepareSourceContextTest(t, map[string]string{"a.c": "trice(\"ctx:valid\");\n", "z.c": test.call + "\n"}, test.rule)()
				bindRandomReader = bytes.NewReader(append(bytes.Repeat([]byte{0x11}, 8), bytes.Repeat([]byte{0x22}, 8)...))
				require.NoError(t, FSys.WriteFile(FnJSON, []byte("{}"), 0o644))
				TriceSAliases = ArrayFlag{test.alias}
				require.NoError(t, ProcessAliases())
				require.NoError(t, emitter.AddUserTagNames())
				before := contextTestSnapshot(t)
				var output bytes.Buffer
				run := SubCmdIdInsert
				if command == "bind" {
					run = SubCmdIdBind
				} else if command == "clean" {
					run = SubCmdIdClean
				}
				err := run(&output, FSys)
				if test.rejected {
					require.Error(t, err)
					assert.Contains(t, err.Error()+output.String(), "-salias "+test.alias+" cannot receive CE")
					assert.Equal(t, before, contextTestSnapshot(t))
				} else {
					require.NoError(t, err, output.String())
					for _, entry := range contextTestTIL(t) {
						if entry.Type == test.alias {
							assert.True(t, strings.HasPrefix(entry.Strg, SAliasStrgPrefix))
							assert.True(t, strings.HasSuffix(entry.Strg, SAliasStrgSuffix))
							assert.NotContains(t, entry.Strg, "[fixed]")
						}
					}
				}
			})
		}
	}
}

// TestAliasContextCanonicalGroupAndRollback checks custom grouping, independent
// scalar fields, unchanged Bind calls, and rollback after a publication failure.
func TestAliasContextCanonicalGroupAndRollback(t *testing.T) {
	const source = "A(\"Sensor:SENSOR:sensor:value={value}\\n\", value);\n"
	defer prepareSourceContextTest(t, map[string]string{"main.c": source}, `SENSOR:" {extra}", extra`)()
	TriceAliases = ArrayFlag{"A:sensor"}
	require.NoError(t, ProcessAliases())
	require.NoError(t, emitter.AddUserTagNames(AliasTagNames()...))
	var output bytes.Buffer
	require.NoError(t, SubCmdIdInsert(&output, FSys), output.String())
	assert.Equal(t, TriceFmt{Type: "A", Strg: `Sensor:SENSOR:sensor:value={value} {extra}\n`}, contextTestTIL(t)[100])
	assert.Equal(t, 1, strings.Count(output.String(), "duplicate CE selector"))
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	cleaned, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, source, string(cleaned))
	// The existing failure-injection filesystem targets final source publication.
	before := contextTestSnapshot(t)
	fs := &afero.Afero{Fs: &bindFailOnceRenameFs{Fs: FSys.Fs, destination: Srcs[0]}}
	err = SubCmdIdInsert(io.Discard, fs)
	require.Error(t, err)
	assert.Equal(t, before, contextTestSnapshot(t))
}

// TestAliasContextStringRules rejects string CE even when the rule is unused,
// but permits literal percent text and original string arguments.
func TestAliasContextStringRules(t *testing.T) {
	for _, format := range []string{`ctx:" %s", text`, `ctx:" {text:%s}", text`, `unused:" %.3s", text`} {
		_, err := parseContextRules([]string{format})
		assert.ErrorContains(t, err, "added runtime strings")
	}
	_, err := parseContextRules([]string{`ctx:" %%s {{text}}"`})
	assert.NoError(t, err)
}

// TestAliasContextBindFallback reports divergence before publication and keeps
// original-source ID policy rather than selecting again from the host result.
func TestAliasContextBindFallback(t *testing.T) {
	for _, test := range []struct {
		format, alias, stored string
		warning               bool
	}{
		{"ctx:err:failed", "printi", "err:failed [checked]", true},
		{"err:failed [checked]", "printi", "err:failed [checked]", false},
		{"untagged:ctx:err:failed", "printi", "untagged:err:failed [checked]", false},
		{"ctx:failed", "printi", "failed [checked]", false},
		{"ctx:err:failed", "MyWrn", "err:failed [checked]", false},
	} {
		t.Run(test.format+test.alias, func(t *testing.T) {
			defer prepareBindTest(t, map[string]string{"main.c": test.alias + `("` + test.format + `");` + "\n"})()
			TriceAliases = ArrayFlag{test.alias}
			require.NoError(t, ProcessAliases())
			require.NoError(t, emitter.AddUserTagNames())
			ContextEnrichment = ArrayFlag{`ctx:" [checked]"`, `ERROR:" [error]"`}
			var output bytes.Buffer
			require.NoError(t, SubCmdIdBind(&output, FSys), output.String())
			// A known ERROR source prefix selects its own group independently of the
			// event tag. A host-based second pass would incorrectly reorder it.
			want := test.stored
			if strings.Contains(test.format, "err:") {
				want += " [error]"
			}
			assert.Equal(t, want, contextTestTIL(t)[100].Strg)
			if test.warning {
				assert.Equal(t, 1, strings.Count(output.String(), "CE fallback changes"))
				assert.Contains(t, output.String(), "build tag untagged to host tag ERROR")
			} else {
				assert.NotContains(t, output.String(), "CE fallback changes")
			}
			before := contextTestSnapshot(t)
			require.NoError(t, SubCmdIdBind(io.Discard, FSys))
			assert.Equal(t, before, contextTestSnapshot(t))
		})
	}
}
