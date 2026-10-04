// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"encoding/json"
	"io"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/emitter"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// contextTestTIL reads the persisted dictionary, not the allocator's internal state.
func contextTestTIL(t *testing.T) TriceIDLookUp {
	t.Helper()
	content, err := FSys.ReadFile(FnJSON)
	require.NoError(t, err)
	var til TriceIDLookUp
	require.NoError(t, json.Unmarshal(content, &til))
	return til
}

// contextTestSnapshot captures all files so rejected changes and repeated runs
// must preserve source, dictionaries, sidecars, and field counts byte for byte.
func contextTestSnapshot(t *testing.T) map[string]string {
	t.Helper()
	files := make(map[string]string)
	require.NoError(t, afero.Walk(FSys.Fs, Proj, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}
		if !info.IsDir() {
			content, err := FSys.ReadFile(path)
			if err != nil {
				return err
			}
			files[path] = string(content)
		}
		return nil
	}))
	return files
}

// TestBindContextFinalSchemas checks externally stored templates and actual
// adapters for source-order selection, field inference, and macro families.
func TestBindContextFinalSchemas(t *testing.T) {
	for _, tt := range []struct {
		name, call string
		rules      ArrayFlag
		want       TriceFmt
		adapter    string
		warning    string
	}{
		{"position fields precede the terminal newline", `trice("MSG:pos:ready\n");`, ArrayFlag{`pos:", x={}, y={}", pos.x, pos.y`}, TriceFmt{Type: "trice", Strg: `MSG:ready, x={pos.x}, y={pos.y}\n`}, "(pos.x), (pos.y)", ""},
		{"source selectors precede CLI order between groups", `trice("msg:ctxb:ctxa:ready");`, ArrayFlag{`ctxa:", a={first}", x`, `ctxb:", b={second}", y`, `ctxa:", c={third}", z`}, TriceFmt{Type: "trice", Strg: "msg:ready, b={second}, a={first}, c={third}"}, "(y), (x), (z)", ""},
		{"mixed case free selector stays visible", `trice("MSG:PoS:ready");`, ArrayFlag{`pos:", x={}", x`}, TriceFmt{Type: "trice", Strg: "MSG:PoS:ready, x={x}"}, "(x)", ""},
		{"known tag alias retains its original prefix", `trice("warn:ready");`, ArrayFlag{`WARNING:", x={}", x`}, TriceFmt{Type: "trice", Strg: "warn:ready, x={x}"}, "(x)", ""},
		{"duplicate selector applies the rule group once", `trice("msg:ctx:CTX:ctx:ready");`, ArrayFlag{`ctx:", x={}", x`}, TriceFmt{Type: "trice", Strg: "msg:CTX:ready, x={x}"}, "(x)", "duplicate CE selector"},
		{"an unconfigured prefix stays intact", `trice("msg:other:ready");`, ArrayFlag{`ctx:", x={}", x`}, TriceFmt{Type: "trice", Strg: "msg:other:ready"}, "", ""},
		{"fixed arity and explicit stamp are extended", `TRICE16_1(Id(0), "msg:ctx:value={v}", v);`, ArrayFlag{`ctx:", x={}", x`}, TriceFmt{Type: "TRICE16_2", Strg: "msg:value={v}, x={x}"}, "TRICE_INSERT_TRICE16_2(trice_ce_tid, trice_ce_format, trice_ce_arg0, (x))", ""},
		{"fixed zero gains a scalar implementation", `trice0("msg:ctx:ready");`, ArrayFlag{`ctx:", x={}", x`}, TriceFmt{Type: "trice_1", Strg: "msg:ready, x={x}"}, "TRICE_INSERT_trice_1(trice_ce_tid, trice_ce_format, (x))", ""},
		{"classic floating extension uses explicit transport", `trice32("msg:speed:ready");`, ArrayFlag{`speed:", m/s=%f", aFloat(velocity)`}, TriceFmt{Type: "trice32", Strg: "msg:ready, m/s=%f"}, "(aFloat(velocity))", ""},
		{"manual position and speed rule combines named values and a float", `trice32("info:Moving sample={sample}\n", 3);`, ArrayFlag{`info:", x={}, y={}, m/s=%f", pos.x, pos.y, aFloat(velocity)`}, TriceFmt{Type: "trice32", Strg: `info:Moving sample={sample}, x={pos.x}, y={pos.y}, m/s=%f\n`}, "(aFloat(velocity))", ""},
		{"double field infers display from its expression", `TRice64("msg:ctx:ready");`, ArrayFlag{`ctx:", energy={}", aDouble(energy)`}, TriceFmt{Type: "TRice64", Strg: "msg:ready, energy={energy:%f}"}, "(aDouble(energy))", ""},
		{"a string record accepts a literal extension", `triceS("msg:ctx:{device:%s}", "pump");`, ArrayFlag{`ctx:" online"`}, TriceFmt{Type: "triceS", Strg: "msg:{device:%s} online"}, "TRICE_INSERT_triceS(trice_ce_tid, trice_ce_format, trice_ce_arg0)", ""},
		{"escaped quotes and literal braces remain C escaped", `trice("msg:ctx:ready");`, ArrayFlag{`ctx:", \"{{x}}\"={x}", x`}, TriceFmt{Type: "trice", Strg: `msg:ready, \"{{x}}\"={x}`}, "(x)", ""},
		{"literal backslash n is not a newline", `trice("msg:ctx:ready\\n");`, ArrayFlag{`ctx:", x={}", x`}, TriceFmt{Type: "trice", Strg: `msg:ready\\n, x={x}`}, "(x)", ""},
		{"closing parenthesis character is data", `trice32("msg:ctx:%d", 7);`, ArrayFlag{`ctx:", c={character}", ')'`}, TriceFmt{Type: "trice32", Strg: "msg:%d, c={character}"}, "(')')", ""},
		{"opening parenthesis character is data", `trice32("msg:ctx:%d", 7);`, ArrayFlag{`ctx:", c={character}", '('`}, TriceFmt{Type: "trice32", Strg: "msg:%d, c={character}"}, "('(')", ""},
		{"exactly twelve values fit the scalar limit", `trice8_0("msg:ctx:values:");`, ArrayFlag{`ctx:"` + strings.Repeat("%d", 12) + `", ` + strings.TrimSuffix(strings.Repeat("x,", 12), ",")}, TriceFmt{Type: "trice8_12", Strg: "msg:values:" + strings.Repeat("%d", 12)}, "TRICE_INSERT_trice8_12", ""},
	} {
		t.Run(tt.name, func(t *testing.T) {
			source := "#include \"trice.h\"\nvoid f(void) {\n    " + tt.call + "\n}\n"
			defer prepareBindTest(t, map[string]string{"main.c": source})()
			ContextEnrichment = tt.rules
			var output bytes.Buffer
			require.NoError(t, SubCmdIdBind(&output, FSys), output.String())
			assert.Equal(t, TriceIDLookUp{100: tt.want}, contextTestTIL(t))
			bound, err := FSys.ReadFile(Srcs[0])
			require.NoError(t, err)
			assert.Contains(t, string(bound), tt.call, "CE never rewrites the user's call")
			sidecar, err := FSys.ReadFile(filepath.Join(BindDir, "trice_main_c_K1111111111111111.h"))
			require.NoError(t, err)
			if tt.adapter == "" {
				assert.NotContains(t, string(sidecar), "trice-ce:")
			} else {
				assert.Contains(t, string(sidecar), tt.adapter)
				assert.NotContains(t, string(sidecar), "__COUNTER__")
			}
			if tt.warning != "" {
				assert.Contains(t, output.String(), tt.warning)
				assert.Equal(t, 1, strings.Count(output.String(), "warning:"))
			} else {
				assert.NotContains(t, output.String(), "warning:")
			}
			before := contextTestSnapshot(t)
			require.NoError(t, SubCmdIdBind(io.Discard, FSys))
			assert.Equal(t, before, contextTestSnapshot(t), "repeated CE bind is byte-stable")
			selected, err := selectCurrentLogEntries(io.Discard, FSys, contextTestTIL(t))
			require.NoError(t, err)
			assert.Equal(t, contextTestTIL(t), selected, "logC consumes the final schema without repeating -ce")
		})
	}
}

// TestBindContextRejectsInvalidRulesBeforeWrites also covers unmatched invalid
// options, so errors cannot disappear merely because a selector was misspelled.
func TestBindContextRejectsInvalidRulesBeforeWrites(t *testing.T) {
	for _, tt := range []struct{ name, rule, diagnostic string }{
		{"missing selector", `:"x"`, "expected selector"},
		{"whitespace in selector", `bad tag:"x"`, "expected selector"},
		{"missing closing quote", `ctx:"x`, "unclosed format"},
		{"missing argument separator", `ctx:"%d" x`, "expected comma"},
		{"missing expression", `ctx:"%d"`, "1 format conversion(s) for 0"},
		{"extra expression", `ctx:"plain", x`, "0 format conversion(s) for 1"},
		{"empty expression", `ctx:"%d",`, "nonempty, comma-free"},
		{"comma in function call", `ctx:"%d", fn(x, y)`, "comma-free"},
		{"comma operator", `ctx:"%d", (x++, x)`, "comma-free"},
		{"trailing expression tokens", `ctx:"%d", x) + f(`, "balanced parentheses"},
		{"inference needs a simple name", `ctx:"{}", x + 1`, "explicit field name"},
		{"duplicate rule fields", `ctx:"{x}{x}", a, b`, "duplicate structured field"},
		{"literal closing brace must be escaped", `ctx:"}"`, "unmatched closing brace"},
		{"multiline expression cannot become a macro", "ctx:\"%d\", x\n+1", "expected selector"},
	} {
		t.Run(tt.name, func(t *testing.T) {
			defer prepareBindTest(t, map[string]string{"main.c": "trice(\"msg:untouched\");\n"})()
			ContextEnrichment = ArrayFlag{tt.rule}
			before := contextTestSnapshot(t)
			err := SubCmdIdBind(io.Discard, FSys)
			assert.ErrorContains(t, err, tt.diagnostic)
			assert.Equal(t, before, contextTestSnapshot(t))
		})
	}
}

// TestBindContextRejectsUnsupportedSitesWithoutPartialWrites verifies concrete
// failure causes, file/line attribution, the UM hint, and the whole transaction.
func TestBindContextRejectsUnsupportedSitesWithoutPartialWrites(t *testing.T) {
	for _, tt := range []struct {
		name, source string
		rules        ArrayFlag
		diagnostic   string
	}{
		{"two calls on a line", "trice(\"ctx:a\"); trice(\"msg:b\");\n", ArrayFlag{`ctx:"{x}", x`}, "direct, line-addressable"},
		{"another call on a multiline boundary is ambiguous", "trice(\n\"ctx:a\"\n); trice(\"msg:b\");\n", ArrayFlag{`ctx:"{x}", x`}, "direct, line-addressable"},
		{"single call wrapper is outside the first CE stage", "#define LOG() trice(\"ctx:a\")\nvoid f(void) { LOG(); }\n", ArrayFlag{`ctx:"{x}", x`}, "direct, line-addressable"},
		{"insert-owned call cannot be enriched by bind", "trice(iD(110), \"ctx:a\");\n", ArrayFlag{`ctx:"{x}", x`}, "direct, line-addressable"},
		{"source and CE cannot duplicate a field", "trice(\"ctx:{x}\", x);\n", ArrayFlag{`ctx:"{x}", x`}, "duplicate structured field"},
		{"two rules cannot duplicate a field", "trice(\"ctx:a\");\n", ArrayFlag{`ctx:"{x}", x`, `ctx:"{x}", y`}, "duplicate structured field"},
		{"fixed original arity is checked before expansion", "trice32_2(\"ctx:%d\", x);\n", ArrayFlag{`ctx:"{y}", y`}, "does not match format specifier count"},
		{"string cannot gain scalar runtime arguments", "triceS(\"ctx:%s\", text);\n", ArrayFlag{`ctx:"{x}", x`}, "cannot append runtime CE arguments"},
		{"buffer cannot gain scalar runtime arguments", "triceB(\"ctx:%02x\", data, size);\n", ArrayFlag{`ctx:"{x}", x`}, "cannot append runtime CE arguments"},
		{"float must use explicit transport", "trice(\"ctx:a\");\n", ArrayFlag{`ctx:"%f", velocity`}, "requires aFloat() or aDouble()"},
		{"eight bit cannot transport a float", "trice8(\"ctx:a\");\n", ArrayFlag{`ctx:"%f", aFloat(velocity)`}, "below 32 bit"},
		{"float32 bits cannot silently become float64", "trice64(\"ctx:a\");\n", ArrayFlag{`ctx:"%f", aFloat(velocity)`}, "requires aDouble(), not aFloat()"},
		{"float64 requires a 64 bit macro", "trice32(\"ctx:a\");\n", ArrayFlag{`ctx:"%f", aDouble(velocity)`}, "non-64-bit"},
		{"scalar record cannot append a string", "trice(\"ctx:a\");\n", ArrayFlag{`ctx:"%s", text`}, "string format specifier outside"},
		{"expanded arity must fit the target", "trice(\"ctx:a\");\n", ArrayFlag{`ctx:"` + strings.Repeat("%d", 13) + `", ` + strings.TrimSuffix(strings.Repeat("x,", 13), ",")}, "no more than 12"},
	} {
		t.Run(tt.name, func(t *testing.T) {
			defer prepareBindTest(t, map[string]string{"main.c": "#include \"trice.h\"\n" + tt.source})()
			ContextEnrichment = tt.rules
			before := contextTestSnapshot(t)
			var output bytes.Buffer
			require.Error(t, SubCmdIdBind(&output, FSys))
			assert.Contains(t, output.String(), tt.diagnostic)
			assert.Contains(t, output.String(), "main.c:")
			assert.Contains(t, output.String(), bindLimitsHint)
			assert.Equal(t, before, contextTestSnapshot(t), "rejection must not publish any partial CE state")
		})
	}
}

// TestBindContextConfigurationChanges covers disabling CE, stable expression-only
// changes, schema changes, field counts, dry-run, and stale logC metadata.
func TestBindContextConfigurationChanges(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"main.c": "#include \"trice.h\"\nvoid f(void) {\ntrice(\"msg:ctx:ready\");\ntrice(\"msg:other\");\n}\n"})()
	ContextEnrichment = ArrayFlag{`ctx:", x={context}", pos.x`}
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	originalTIL := contextTestTIL(t)
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	sidecarPath := filepath.Join(BindDir, "trice_main_c_K1111111111111111.h")
	sidecar, err := FSys.ReadFile(sidecarPath)
	require.NoError(t, err)
	fields, err := FSys.ReadFile(filepath.Join(BindDir, "trice-fields.txt"))
	require.NoError(t, err)
	assert.Contains(t, string(fields), "context")

	ContextEnrichment = ArrayFlag{`ctx:", x={context}", pos.y`}
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	assert.Equal(t, originalTIL, contextTestTIL(t), "only an expression changed, so schema and IDs stay the same")
	updated, err := FSys.ReadFile(sidecarPath)
	require.NoError(t, err)
	assert.NotEqual(t, sidecar, updated)
	assert.Contains(t, string(updated), "(pos.y)")

	ContextEnrichment = ArrayFlag{`ctx:", y={new_field}", pos.y`}
	DryRun = true
	before := contextTestSnapshot(t)
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	assert.Equal(t, before, contextTestSnapshot(t), "dry-run does not publish the changed schema")
	DryRun = false
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	assert.Equal(t, originalTIL[100], contextTestTIL(t)[100], "old firmware keeps its historical template")
	selected, err := selectCurrentLogEntries(io.Discard, FSys, contextTestTIL(t))
	require.NoError(t, err)
	assert.NotContains(t, selected, TriceID(100), "changed schema has a new current ID")

	// A source edit without bind must never make logC trust stale CE facts.
	changedSource := bytes.Replace(bound, []byte("msg:ctx:ready"), []byte("msg:ctx:changed"), 1)
	require.NoError(t, FSys.WriteFile(Srcs[0], changedSource, 0o644))
	var output bytes.Buffer
	_, err = selectCurrentLogEntries(&output, FSys, contextTestTIL(t))
	require.Error(t, err)
	assert.Contains(t, output.String(), "stale or inconsistent CE metadata")
	require.NoError(t, FSys.WriteFile(Srcs[0], bound, 0o644))

	ContextEnrichment = nil
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	selected, err = selectCurrentLogEntries(io.Discard, FSys, contextTestTIL(t))
	require.NoError(t, err)
	assert.Contains(t, selected, TriceID(101), "the unrelated log keeps its ID")
	assert.Contains(t, selected, TriceID(103), "turning CE off restores the source template as its own schema")
	assert.Equal(t, "msg:ctx:ready", selected[103].Strg)
	fields, err = FSys.ReadFile(filepath.Join(BindDir, "trice-fields.txt"))
	require.NoError(t, err)
	assert.NotContains(t, string(fields), "new_field")
	updated, err = FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, bound, updated)
}

// TestBindContextFieldRegistryCountsCurrentSites protects counts for repeated
// schemas and when old CE schemas remain in the historical TIL.
func TestBindContextFieldRegistryCountsCurrentSites(t *testing.T) {
	const source = `#include "trice.h"
void f(void) {
    trice32("info:pos:first={source}", 1);
    trice32("info:pos:second");
    trice32("info:pos:second");
    trice32("info:plain={direct}", 7);
}
`
	defer prepareBindTest(t, map[string]string{"main.c": source})()
	ContextEnrichment = ArrayFlag{`pos:", x={}, y={}", pos.x, pos.y`}
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	fieldsPath := filepath.Join(BindDir, "trice-fields.txt")
	fields, err := FSys.ReadFile(fieldsPath)
	require.NoError(t, err)
	assert.Equal(t, "       1 direct\n       1 source\n       3 pos.x\n       3 pos.y\n", string(fields))
	assert.Len(t, contextTestTIL(t), 4, "Bind keeps location-specific IDs even for identical schemas")
	before := contextTestSnapshot(t)
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	assert.Equal(t, before, contextTestSnapshot(t), "reused IDs must not lose field counts")

	ContextEnrichment = nil
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	fields, err = FSys.ReadFile(fieldsPath)
	require.NoError(t, err)
	assert.Equal(t, "       1 direct\n       1 source\n", string(fields), "historical CE entries never contribute to the current field registry")
}

// TestBindContextKeepsUnselectedRebaseAndUserTags verifies that the first-stage
// restriction belongs to selected sites, not to every file using -ce.
func TestBindContextKeepsUnselectedRebaseAndUserTags(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"main.c": "#include \"trice.h\"\nvoid f(void) {\ntrice(\"msg:a\"); trice(\"msg:b\");\ntrice(\"motor:ready\");\n}\n"})()
	oldLabels := emitter.UserLabel
	t.Cleanup(func() { emitter.UserLabel = oldLabels; require.NoError(t, emitter.AddUserLabels()) })
	emitter.UserLabel = []string{"motor"}
	require.NoError(t, emitter.AddUserLabels())
	ContextEnrichment = ArrayFlag{`MOTOR:", x={}", x`}
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	assert.Equal(t, "motor:ready, x={x}", contextTestTIL(t)[102].Strg, "registered user tags keep their text prefix")
	selected, err := selectCurrentLogEntries(io.Discard, FSys, contextTestTIL(t))
	require.NoError(t, err)
	assert.Len(t, selected, 3)
	before := contextTestSnapshot(t)
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	assert.Equal(t, before, contextTestSnapshot(t))
}

// TestBindContextMetadataRejectsStaleFacts verifies that logC cannot publish
// a schema detached from the exact source call and authoritative descriptor ID.
func TestBindContextMetadataRejectsStaleFacts(t *testing.T) {
	for _, corruption := range []string{"source argument", "malformed JSON", "wrong descriptor ID", "duplicate metadata", "wrong final schema", "missing metadata"} {
		t.Run(corruption, func(t *testing.T) {
			defer prepareBindTest(t, map[string]string{"main.c": "trice(\"msg:ctx:sample=%d\", 7);\n"})()
			ContextEnrichment = ArrayFlag{`ctx:", x={context}", x`}
			require.NoError(t, SubCmdIdBind(io.Discard, FSys))
			path := filepath.Join(BindDir, "trice_main_c_K1111111111111111.h")
			content, err := FSys.ReadFile(path)
			require.NoError(t, err)
			lines := strings.Split(string(content), "\n")
			for i, line := range lines {
				if !strings.HasPrefix(line, "// trice-ce: ") {
					continue
				}
				switch corruption {
				case "source argument":
					source, err := FSys.ReadFile(Srcs[0])
					require.NoError(t, err)
					require.NoError(t, FSys.WriteFile(Srcs[0], bytes.Replace(source, []byte(", 7)"), []byte(", 8)"), 1), 0o644))
				case "malformed JSON":
					lines[i] = "// trice-ce: {"
				case "wrong descriptor ID":
					lines[i] = strings.Replace(line, `"id":100`, `"id":101`, 1)
				case "duplicate metadata":
					lines = append(lines, line)
				case "wrong final schema":
					lines[i] = strings.Replace(line, "{context}", "{other}", 1)
				case "missing metadata":
					lines[i] = ""
				}
				break
			}
			require.NoError(t, FSys.WriteFile(path, []byte(strings.Join(lines, "\n")), 0o644))
			before := contextTestSnapshot(t)
			selected, err := selectCurrentLogEntries(io.Discard, FSys, contextTestTIL(t))
			assert.Error(t, err)
			assert.Nil(t, selected)
			assert.Equal(t, before, contextTestSnapshot(t))
		})
	}
}

// TestBindContextRollsBackAWriteFailure protects the complete CE transaction,
// including the old adapter and schema when publishing the new TIL fails.
func TestBindContextRollsBackAWriteFailure(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"main.c": "trice(\"msg:ctx:ready\");\n"})()
	ContextEnrichment = ArrayFlag{`ctx:", x={x}", x`}
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	before := contextTestSnapshot(t)
	ContextEnrichment = ArrayFlag{`ctx:", y={y}", y`}
	failing := &bindFailOnceRenameFs{Fs: FSys.Fs, destination: FnJSON}
	err := SubCmdIdBind(io.Discard, &afero.Afero{Fs: failing})
	assert.ErrorContains(t, err, "injected rename failure")
	assert.True(t, failing.failed, "the test must reach the intended publication failure")
	assert.Equal(t, before, contextTestSnapshot(t))
}
