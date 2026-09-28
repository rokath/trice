// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"io"
	"path/filepath"
	"strings"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// prepareSourceContextTest keeps generated fields inside the isolated project.
func prepareSourceContextTest(t *testing.T, sources map[string]string, rules ...string) func() {
	t.Helper()
	teardown := prepareBindTest(t, sources)
	FieldsDir = BindDir
	ContextEnrichment = append(ArrayFlag{}, rules...)
	return teardown
}

// TestInsertCleanContextRoundTrip asserts persisted output, repeatability,
// exact source restoration and stable reuse of the final enriched schema.
func TestInsertCleanContextRoundTrip(t *testing.T) {
	for _, tc := range []struct {
		name, call, rule, physical string
		want                       TriceFmt
	}{
		{"clock example retains selector only in source", `trice("msg:ctx7:hi\n");`, `ctx7:", clock={}", clock`, `"msg:ctx7:hi, clock={}\n", clock)`, TriceFmt{Type: "trice", Strg: `msg:hi, clock={clock}\n`}},
		{"existing fields and whitespace survive cleaning", "trice( \"MSG:ctx:value={}\\n\", value );", `ctx:", count={count}", counter`, `"MSG:ctx:value={}, count={count}\n", value , counter)`, TriceFmt{Type: "trice", Strg: `MSG:value={value}, count={count}\n`}},
		{"fixed arity grows and then returns", `TRICE16_1(Id(0), "msg:ctx:{v}", v);`, `ctx:", x={}", x`, `TRICE16_2`, TriceFmt{Type: "TRICE16_2", Strg: "msg:{v}, x={x}"}},
		{"fixed zero becomes one", `trice0("msg:ctx:ready");`, `ctx:" {x}", x`, `trice_1`, TriceFmt{Type: "trice_1", Strg: "msg:ready {x}"}},
		{"literal extension works on string records", `triceS("msg:ctx:{device:%s}", "pump");`, `ctx:" online"`, `"msg:ctx:{device:%s} online", "pump")`, TriceFmt{Type: "triceS", Strg: "msg:{device:%s} online"}},
		{"float transport remains explicit", `trice32("msg:ctx:speed");`, `ctx:", m/s=%f", aFloat(velocity)`, `aFloat(velocity))`, TriceFmt{Type: "trice32", Strg: "msg:speed, m/s=%f"}},
		{"double field inference survives restoration", `trice64("msg:ctx:speed");`, `ctx:", speed={}", aDouble(velocity)`, `aDouble(velocity))`, TriceFmt{Type: "trice64", Strg: "msg:speed, speed={velocity:%f}"}},
		{"opening parenthesis character is data", `trice("msg:ctx:ready");`, `ctx:" {character}", '('`, `, '(')`, TriceFmt{Type: "trice", Strg: "msg:ready {character}"}},
		{"closing parenthesis character is data", `trice("msg:ctx:ready");`, `ctx:" {character}", ')'`, `, ')')`, TriceFmt{Type: "trice", Strg: "msg:ready {character}"}},
		{"escaped backslash n stays literal", `trice("msg:ctx:ready\\n");`, `ctx:" {x}", x`, `"msg:ctx:ready\\n {x}"`, TriceFmt{Type: "trice", Strg: `msg:ready\\n {x}`}},
		{"mixed-case selector remains visible", `trice("MSG:Ctx:ready");`, `ctx:" {x}", x`, `"MSG:Ctx:ready {x}"`, TriceFmt{Type: "trice", Strg: "MSG:Ctx:ready {x}"}},
		{"known alias retains normal tag policy", `trice("warn:ready");`, `WARNING:" {x}", x`, `"warn:ready {x}"`, TriceFmt{Type: "trice", Strg: "warn:ready {x}"}},
		{"literal context needs no arguments", `trice("msg:ctx:ready\n");`, `ctx:" online"`, `"msg:ctx:ready online\n")`, TriceFmt{Type: "trice", Strg: `msg:ready online\n`}},
		{"comments cannot close the call", `trice("msg:ctx:{x}", x /* ) */);`, `ctx:" {y}", y`, `x /* ) */, y)`, TriceFmt{Type: "trice", Strg: "msg:{x} {y}"}},
		{"CE expression comments survive safely", `trice("msg:ctx:ready");`, `ctx:" {x}", x /* ) */`, `, x /* ) */)`, TriceFmt{Type: "trice", Strg: "msg:ready {x}"}},
		{"empty extension still applies selector policy", `trice("msg:ctx:ready");`, `ctx:""`, `"msg:ctx:ready")`, TriceFmt{Type: "trice", Strg: "msg:ready"}},
	} {
		t.Run(tc.name, func(t *testing.T) {
			source := "#include \"trice.h\"\nvoid f(void) {\n    " + tc.call + "\n}\n"
			defer prepareSourceContextTest(t, map[string]string{"main.c": source}, tc.rule)()
			var output bytes.Buffer
			err := SubCmdIdInsert(&output, FSys)
			require.NoError(t, err, output.String())
			inserted, err := FSys.ReadFile(Srcs[0])
			require.NoError(t, err)
			assert.Contains(t, string(inserted), tc.physical)
			assert.Equal(t, 1, strings.Count(string(inserted), insertContextPrefix))
			assert.Equal(t, TriceIDLookUp{100: tc.want}, contextTestTIL(t))
			first := contextTestSnapshot(t)
			require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
			assert.Equal(t, first, contextTestSnapshot(t), "second insert must not append anything or change an ID")
			selected, err := selectCurrentLogEntries(io.Discard, FSys, contextTestTIL(t))
			require.NoError(t, err)
			assert.Equal(t, contextTestTIL(t), selected, "generate uses the proven final schema")
			require.NoError(t, SubCmdIdClean(io.Discard, FSys))
			cleaned, err := FSys.ReadFile(Srcs[0])
			require.NoError(t, err)
			assert.Equal(t, source, string(cleaned), "clean restores field spelling, arity, whitespace and newline")
			afterClean := contextTestSnapshot(t)
			require.NoError(t, SubCmdIdClean(io.Discard, FSys))
			assert.Equal(t, afterClean, contextTestSnapshot(t), "second clean is harmless")
			require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
			assert.Equal(t, first, contextTestSnapshot(t), "reinsertion reuses the enriched schema's ID")
		})
	}
}

// TestSourceContextSelectionAndOwnership distinguishes actual generated data
// from identical hand-written suffixes, and checks source/CLI group ordering.
func TestSourceContextSelectionAndOwnership(t *testing.T) {
	source := "trice(\"msg:ctxb:ctxa:CTXA:ready\\n\");\ntrice(\"msg:manual, x={x}\", x);\n"
	defer prepareSourceContextTest(t, map[string]string{"main.c": source}, `ctxa:", a={first}", a`, `ctxb:", b={second}", b`, `ctxa:", c={third}", c`)()
	var output bytes.Buffer
	require.NoError(t, SubCmdIdInsert(&output, FSys))
	assert.Contains(t, output.String(), "duplicate CE selector")
	assert.Equal(t, TriceFmt{Type: "trice", Strg: `msg:CTXA:ready, b={second}, a={first}, c={third}\n`}, contextTestTIL(t)[100])
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	cleaned, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, source, string(cleaned))
	// Even a matching selector and suffix do not prove that clean owns the text.
	manual := `trice("msg:ctx:manual, x={x}", x);`
	require.NoError(t, FSys.WriteFile(Srcs[0], []byte(manual), 0o644))
	ContextEnrichment = ArrayFlag{`ctx:", x={x}", x`}
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	cleaned, err = FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, manual, string(cleaned))
	before := contextTestSnapshot(t)
	assert.ErrorContains(t, SubCmdIdInsert(io.Discard, FSys), "duplicate structured field")
	assert.Equal(t, before, contextTestSnapshot(t), "manual data is not guessed to be prior CE output")
}

// TestSourceContextRejectsBeforePublishing covers complete-project preflight,
// including invalid unselected rules and a later file's unsupported record.
func TestSourceContextRejectsBeforePublishing(t *testing.T) {
	for _, tc := range []struct{ name, rule, badCall, diagnostic string }{
		{"malformed unused rule", `other:"%d"`, `trice("msg:plain");`, "format conversion"},
		{"duplicate field", `ctx:" {v}", extra`, `trice("msg:ctx:{v}", value);`, "duplicate structured field"},
		{"too many scalar values", `ctx:"%d%d%d%d%d%d%d%d%d%d%d%d", x,x,x,x,x,x,x,x,x,x,x,x`, `trice("msg:ctx:%d", x);`, "no more than 12"},
		{"buffer cannot gain arguments", `ctx:"%d", x`, `triceB("msg:ctx:%x", buffer, size);`, "cannot append runtime"},
		{"string cannot gain arguments", `ctx:"%d", x`, `triceS("msg:ctx:%s", "device");`, "cannot append runtime"},
		{"wrong float transport", `ctx:"%f", aFloat(velocity)`, `trice64("msg:ctx:ready");`, "requires aDouble"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			defer prepareSourceContextTest(t, map[string]string{"a.c": `trice("msg:ctx:valid");`, "z.c": tc.badCall}, tc.rule)()
			before := contextTestSnapshot(t)
			assert.ErrorContains(t, SubCmdIdInsert(io.Discard, FSys), tc.diagnostic)
			assert.Equal(t, before, contextTestSnapshot(t), "no file changes on preflight failure")
		})
	}
}

// TestSourceContextRejectsRuleChangesAndEdits prevents accidental destruction of
// a user's changes, even if the old generated marker is still next to the call.
func TestSourceContextRejectsRuleChangesAndEdits(t *testing.T) {
	for _, change := range []string{"different rule", "different argument", "different message", "broken marker", "detached marker", "reordered rules"} {
		t.Run(change, func(t *testing.T) {
			defer prepareSourceContextTest(t, map[string]string{"main.c": `trice("msg:ctx:ready");`}, `ctx:" {x}", x`, `unused:" text"`)()
			require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
			content, err := FSys.ReadFile(Srcs[0])
			require.NoError(t, err)
			switch change {
			case "different rule":
				ContextEnrichment[0] = `ctx:" {x}", other`
			case "different argument":
				content = bytes.Replace(content, []byte(", x)"), []byte(", y)"), 1)
			case "different message":
				content = bytes.Replace(content, []byte("ready"), []byte("edited"), 1)
			case "broken marker":
				content = bytes.Replace(content, []byte(insertContextPrefix), []byte(insertContextPrefix+"?"), 1)
			case "detached marker":
				content = bytes.Replace(content, []byte(insertContextPrefix), []byte("; "+insertContextPrefix), 1)
			case "reordered rules":
				ContextEnrichment[0], ContextEnrichment[1] = ContextEnrichment[1], ContextEnrichment[0]
			}
			require.NoError(t, FSys.WriteFile(Srcs[0], content, 0o644))
			before := contextTestSnapshot(t)
			for _, command := range []func(io.Writer, *afero.Afero) error{SubCmdIdInsert, SubCmdIdClean} {
				assert.Error(t, command(io.Discard, FSys))
				assert.Equal(t, before, contextTestSnapshot(t), "rejection must preserve the user's edited state")
			}
		})
	}
}

// TestSourceContextMovesWithItsCall does not rely on source paths, line numbers,
// or build files to recognize its own output. Legacy ID-only clean is harmless.
func TestSourceContextMovesWithItsCall(t *testing.T) {
	defer prepareSourceContextTest(t, map[string]string{"main.c": `trice("msg:ctx:ready\n");`}, `ctx:" {x}", x`)()
	TriceCacheEnabled = true
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	inserted, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	moved := filepath.Join(Proj, t.Name(), "moved.c")
	require.NoError(t, FSys.WriteFile(moved, append([]byte("// Moved call.\n\n"), inserted...), 0o644))
	require.NoError(t, FSys.Remove(Srcs[0]))
	require.NoError(t, FSys.RemoveAll(FieldsDir))
	Srcs = ArrayFlag{moved}
	rules := ContextEnrichment
	ContextEnrichment = nil
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	partiallyCleaned, err := FSys.ReadFile(moved)
	require.NoError(t, err)
	assert.NotContains(t, string(partiallyCleaned), "iD(")
	assert.Contains(t, string(partiallyCleaned), insertContextPrefix, "ID-only clean must retain ownership")
	ContextEnrichment = rules
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	assert.Len(t, contextTestTIL(t), 1, "moving a call and deleting build output do not create a new schema")
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	cleaned, err := FSys.ReadFile(moved)
	require.NoError(t, err)
	assert.Equal(t, "// Moved call.\n\ntrice(\"msg:ctx:ready\\n\");", string(cleaned))
}

// TestSourceContextRespectsSelection checks commented and excluded regions,
// multiline calls, macro definitions, repeated rules, and current field counts.
func TestSourceContextRespectsSelection(t *testing.T) {
	source := "// trice(\"msg:ctx:comment\");\n/* trice(\"msg:ctx:block comment\"); */\n" +
		"// TRICE_INSERT_OFF\ntrice(\"msg:ctx:excluded region\");\n// TRICE_INSERT_ON\n" +
		"#define WRAP() do { trice(\"msg:ctx:one\"); trice(\"msg:ctx:two\"); } while (0)\n" +
		"trice(\n  \"msg:ctx:multiline\\n\"\n);\nWRAP();\n"
	defer prepareSourceContextTest(t, map[string]string{"main.c": source, "excluded.c": `trice("msg:ctx:excluded file");`}, `ctx:" {x}", x`)()
	excluded := filepath.Join(Proj, t.Name(), "excluded.c")
	ExcludeSrcs = ArrayFlag{excluded}
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	path := filepath.Join(Proj, t.Name(), "main.c")
	inserted, err := FSys.ReadFile(path)
	require.NoError(t, err)
	assert.Equal(t, 3, strings.Count(string(inserted), insertContextPrefix))
	assert.Contains(t, string(inserted), `trice("msg:ctx:excluded region");`)
	registry, err := FSys.ReadFile(filepath.Join(FieldsDir, "trice-fields.txt"))
	require.NoError(t, err)
	assert.Equal(t, "       3 x\n", string(registry))
	excludedContent, err := FSys.ReadFile(excluded)
	require.NoError(t, err)
	assert.Equal(t, `trice("msg:ctx:excluded file");`, string(excludedContent))
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	cleaned, err := FSys.ReadFile(path)
	require.NoError(t, err)
	assert.Equal(t, source, string(cleaned), "commented calls retain ordinary ID behavior without nested ownership comments")
}

// TestSourceContextAtomicPublish exercises real rollback after one replacement
// has succeeded, and dry-run isolation for both insertion and cleanup.
func TestSourceContextAtomicPublish(t *testing.T) {
	defer prepareSourceContextTest(t, map[string]string{"a.c": `trice("msg:ctx:one");`, "b.c": `trice("msg:ctx:two");`}, `ctx:" {x}", x`)()
	for _, clean := range []bool{false, true} {
		command := SubCmdIdInsert
		if clean {
			require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
			command = SubCmdIdClean
		}
		before := contextTestSnapshot(t)
		DryRun = true
		require.NoError(t, command(io.Discard, FSys))
		assert.Equal(t, before, contextTestSnapshot(t), "dry run must not publish any source or metadata")
		DryRun = false
		failing := &bindFailOnceRenameFs{Fs: FSys.Fs, destination: filepath.Join(Proj, t.Name(), "b.c")}
		assert.ErrorContains(t, command(io.Discard, &afero.Afero{Fs: failing}), "injected rename failure")
		assert.True(t, failing.failed, "the failure must reach the publishing stage")
		assert.Equal(t, before, contextTestSnapshot(t), "rollback must restore every already published file")
	}
}
