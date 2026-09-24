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

	"github.com/rokath/trice/internal/fmtspec"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
)

// structuredTestFile reads an artifact and reports failures at the caller.
func structuredTestFile(t *testing.T, path string) []byte {
	t.Helper()
	content, err := FSys.ReadFile(path)
	assert.NoError(t, err)
	return content
}

// structuredTestTIL verifies that no hidden schema extension has been introduced.
func structuredTestTIL(t *testing.T) TriceIDLookUp {
	t.Helper()
	content := structuredTestFile(t, FnJSON)
	var raw map[string]map[string]json.RawMessage
	assert.NoError(t, json.Unmarshal(content, &raw))
	for _, entry := range raw {
		assert.Len(t, entry, 2, "TIL entries contain only Type and Strg")
	}
	var til TriceIDLookUp
	assert.NoError(t, json.Unmarshal(content, &til))
	return til
}

// TestStructuredInsertCleanRoundTrip checks source reversibility separately
// from canonical dictionary identity and repeatable registry generation.
func TestStructuredInsertCleanRoundTrip(t *testing.T) {
	source := "#include \"trice.h\"\nvoid log_values(void) {\n    trice(\"info:Motor {}: {} C\\n\", motor_id, aFloat(temperature_c));\n    trice(\"{motor.} %d {last:%u}\", state->value, unused, last);\n}\n"
	defer prepareBindTest(t, map[string]string{"module.c": source})()
	oldDir := FieldsDir
	FieldsDir = BindDir
	t.Cleanup(func() { FieldsDir = oldDir })
	if !assert.NoError(t, SubCmdIdInsert(io.Discard, FSys)) {
		return
	}
	inserted := structuredTestFile(t, Srcs[0])
	tilBytes := structuredTestFile(t, FnJSON)
	til := structuredTestTIL(t)
	assert.Equal(t, "info:Motor {motor_id}: {temperature_c:%f} C\\n", til[100].Strg)
	assert.Equal(t, "{motor.state.value} %d {last:%u}", til[101].Strg)
	assert.Contains(t, string(inserted), `"info:Motor {}: {} C\n"`, "source short forms remain intact")
	registryPath := filepath.Join(FieldsDir, "trice-fields.txt")
	registry := structuredTestFile(t, registryPath)
	assert.Equal(t, "       1 last\n       1 motor.state.value\n       1 motor_id\n       1 temperature_c\n", string(registry))
	assert.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	assert.Equal(t, inserted, structuredTestFile(t, Srcs[0]), "second insert is byte-identical")
	assert.Equal(t, tilBytes, structuredTestFile(t, FnJSON), "second insert allocates no schema")
	assert.Equal(t, registry, structuredTestFile(t, registryPath), "registry counts do not accumulate")
	assert.NoError(t, SubCmdIdClean(io.Discard, FSys))
	assert.Equal(t, source, string(structuredTestFile(t, Srcs[0])), "clean(insert(source)) equals source")
	assert.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	assert.Equal(t, inserted, structuredTestFile(t, Srcs[0]), "insert after clean restores the same IDs")
}

// TestStructuredBindSchemaLifecycle covers stable IDs for equivalent member
// spelling, new IDs after a field rename, and preservation of historical schemas.
func TestStructuredBindSchemaLifecycle(t *testing.T) {
	source := "#include \"trice.h\"\nvoid log_value(void) {\n    trice(\"info:{}\", motor->temperature);\n}\n"
	defer prepareBindTest(t, map[string]string{"module.c": source})()
	if !assert.NoError(t, SubCmdIdBind(io.Discard, FSys)) {
		return
	}
	first := structuredTestFile(t, Srcs[0])
	firstTIL := structuredTestFile(t, FnJSON)
	assert.Equal(t, "info:{motor.temperature}", structuredTestTIL(t)[100].Strg)
	assert.NoError(t, SubCmdIdBind(io.Discard, FSys))
	assert.Equal(t, first, structuredTestFile(t, Srcs[0]))
	assert.Equal(t, firstTIL, structuredTestFile(t, FnJSON))
	assert.NoError(t, FSys.WriteFile(Srcs[0], []byte(strings.ReplaceAll(string(first), "motor->temperature", "motor.temperature")), 0o644))
	assert.NoError(t, SubCmdIdBind(io.Discard, FSys))
	assert.Equal(t, firstTIL, structuredTestFile(t, FnJSON), "equivalent canonical name retains schema and ID")
	current := string(structuredTestFile(t, Srcs[0]))
	assert.NoError(t, FSys.WriteFile(Srcs[0], []byte(strings.ReplaceAll(current, "info:{}", "info:{temperature_c}")), 0o644))
	assert.NoError(t, SubCmdIdBind(io.Discard, FSys))
	til := structuredTestTIL(t)
	assert.Len(t, til, 2)
	assert.Equal(t, "info:{motor.temperature}", til[100].Strg, "historical firmware remains decodable")
	assert.Equal(t, "info:{temperature_c}", til[101].Strg)
	assert.Equal(t, "       1 temperature_c\n", string(structuredTestFile(t, filepath.Join(BindDir, "trice-fields.txt"))), "registry excludes historical dictionary fields")
}

// TestStructuredInvalidSourcesFailBeforeWrites checks a whole multi-file run:
// a valid neighbor must not be rewritten when another file has a schema error.
func TestStructuredInvalidSourcesFailBeforeWrites(t *testing.T) {
	for _, operation := range []string{"insert", "bind"} {
		for _, tt := range []struct{ name, call, errorText string }{
			{"duplicate", `trice("{motor->id} {motor.id}", a, b);`, "duplicate structured field"},
			{"missing explicit name", `trice("{}", values[0]);`, "explicit field name"},
			{"unbalanced", `trice("value={x", x);`, "unclosed"},
			{"two conversions", `trice("{x:%d %d}", x, y);`, "exactly one"},
			{"named buffer", `triceB("{bytes:%02x}", buffer, count);`, "does not support named structured fields"},
			{"string field cannot claim an integer type", `triceS("{bytes:%x}", text);`, "string fields require %s"},
			{"float wrapper must match a double payload", `trice64("{temperature}", aFloat(temperature));`, "requires aDouble()"},
			{"missing value", `trice("{x}");`, "parameter count"},
			{"too many values", `trice("{x}", x, y);`, "parameter count"},
		} {
			t.Run(operation+"/"+tt.name, func(t *testing.T) {
				defer prepareBindTest(t, map[string]string{"a.c": `trice("{valid}", value);`, "z.c": tt.call})()
				oldDir := FieldsDir
				FieldsDir = BindDir
				t.Cleanup(func() { FieldsDir = oldDir })
				before := make(map[string][]byte)
				for _, path := range append(append([]string{}, Srcs...), FnJSON, LIFnJSON) {
					before[path] = structuredTestFile(t, path)
				}
				var err error
				var diagnostics bytes.Buffer
				if operation == "insert" {
					err = SubCmdIdInsert(&diagnostics, FSys)
				} else {
					err = SubCmdIdBind(&diagnostics, FSys)
				}
				assert.Error(t, err)
				assert.Contains(t, diagnostics.String(), tt.errorText)
				for path, content := range before {
					assert.Equal(t, content, structuredTestFile(t, path), path+" must remain unchanged")
				}
				_, err = FSys.Stat(filepath.Join(BindDir, "trice-fields.txt"))
				assert.True(t, os.IsNotExist(err), "failed runs must not publish a registry")
			})
		}
	}
}

// TestStructuredRegistryCountsCurrentSites checks rarity sorting, alphabetical
// ties, multiple sites with the same name, and removal of no-longer-used fields.
func TestStructuredRegistryCountsCurrentSites(t *testing.T) {
	source := "trice(\"{z} {motor.id}\", z, id);\ntrice(\"{a} {motor.id}\", a, id);\n"
	defer prepareBindTest(t, map[string]string{"module.c": source})()
	if !assert.NoError(t, SubCmdIdBind(io.Discard, FSys)) {
		return
	}
	path := filepath.Join(BindDir, "trice-fields.txt")
	assert.Equal(t, "       1 a\n       1 z\n       2 motor.id\n", string(structuredTestFile(t, path)))
	current := string(structuredTestFile(t, Srcs[0]))
	current = strings.ReplaceAll(current, `trice("{z} {motor.id}", z, id);`, `trice("no fields");`)
	current = strings.ReplaceAll(current, `trice("{a} {motor.id}", a, id);`, `trice("also no fields");`)
	assert.NoError(t, FSys.WriteFile(Srcs[0], []byte(current), 0o644))
	assert.NoError(t, SubCmdIdBind(io.Discard, FSys))
	assert.Empty(t, structuredTestFile(t, path), "successful field-free run clears the registry")
}

// TestStructuredGeneratedLocalFormat derives classic printf metadata without
// storing a second schema or leaking field names to the target-side formatter.
func TestStructuredGeneratedLocalFormat(t *testing.T) {
	til := TriceIDLookUp{4711: {Type: "TRICE32_2", Strg: "info:Motor {motor_id}: {temperature:%.1f C}"}}
	generated, err := til.toListTilC("til.c")
	assert.NoError(t, err)
	assert.Contains(t, string(generated), `"info:Motor %d: %.1f C"`)
	assert.NotContains(t, string(generated), "{motor_id}")
	assert.Equal(t, "info:Motor {motor_id}: {temperature:%.1f C}", til[4711].Strg)
}

// TestStructuredLegacyMigrationKeepsHistoricalIDs converts both active source
// and historical dictionary-only entries. Braces already occurring as pairs
// were two old literal characters and must therefore become four characters.
func TestStructuredLegacyMigrationKeepsHistoricalIDs(t *testing.T) {
	source := `trice(iD(100), "set={1,2}, pair={{, close=}");` + "\n"
	defer prepareBindTest(t, map[string]string{"module.c": source})()
	oldMigration := MigrateBraces
	t.Cleanup(func() { MigrateBraces = oldMigration })
	MigrateBraces = true
	assert.NoError(t, FSys.WriteFile(FnJSON, []byte(`{"100":{"Type":"trice","Strg":"set={1,2}, pair={{, close=}"},"99":{"Type":"TRICE32_1","Strg":"old={value:%d}"}}`), 0o644))
	beforeTIL := structuredTestFile(t, FnJSON)
	DryRun = true
	assert.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	assert.Equal(t, source, string(structuredTestFile(t, Srcs[0])))
	assert.Equal(t, beforeTIL, structuredTestFile(t, FnJSON))
	DryRun = false
	assert.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	assert.Equal(t, `trice(iD(100), "set={{1,2}}, pair={{{{, close=}}");`+"\n", string(structuredTestFile(t, Srcs[0])))
	til := structuredTestTIL(t)
	assert.Equal(t, "set={{1,2}}, pair={{{{, close=}}", til[100].Strg)
	assert.Equal(t, "old={{value:%d}}", til[99].Strg)
	assert.Len(t, til, 2, "migration does not allocate or discard IDs")
	for _, tid := range []TriceID{99, 100} {
		template, err := fmtspec.ParseTemplate(til[tid].Strg, nil)
		assert.NoError(t, err)
		assert.Empty(t, template.Fields, "old braces must not create structured fields")
	}
}

// TestStructuredTargetTemplatesCompile runs the real target macros with the
// generated IDs in both instrumentation workflows. Field syntax remains host
// information; no target parser, new API, or transmitted field name is needed.
func TestStructuredTargetTemplatesCompile(t *testing.T) {
	compiler := firstAvailableCompiler("cc", "clang", "gcc")
	if compiler == "" {
		t.Skip("a C compiler is required for the target integration check")
	}
	for _, operation := range []string{"insert", "bind"} {
		t.Run(operation, func(t *testing.T) {
			project := t.TempDir()
			fileSystem, teardown := prepareOSBindProject(t, project)
			defer teardown()
			oldDir := FieldsDir
			FieldsDir = BindDir
			t.Cleanup(func() { FieldsDir = oldDir })
			writeBindIntegrationFile(t, project, "triceConfig.h", `#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_CLEAN 0
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DEFERRED_OUTPUT 0
#define TRICE_DIRECT_AUXILIARY8 1
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_TCOBS
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0
#endif
`)
			source := writeBindIntegrationFile(t, project, "module.c", `#include "trice.h"
void report(int motor_id, float temperature_c, double voltage) {
    trice("info:Motor {}: {} C\n", motor_id, aFloat(temperature_c));
    trice8("{small:%u}", 255);
    trice16("{small:%d}", -32768);
    trice64("{voltage:%.3f}", aDouble(voltage));
    triceS("info:{message:%s}", "motor ready");
    triceN("info:{message:%s}", "motor ready", 11);
    trice("set={{1,2}}, value={value}", motor_id);
}
`)
			var err error
			if operation == "insert" {
				err = SubCmdIdInsert(io.Discard, fileSystem)
			} else {
				err = SubCmdIdBind(io.Discard, fileSystem)
			}
			if !assert.NoError(t, err) {
				return
			}
			compileBindFixture(t, compiler, "c11", source, filepath.Join(project, "module.o"), BindDir, project, filepath.Join(bindRepositoryRoot(t), "src"))
		})
	}
}

// TestStructuredMigrationRollback prevents a failed source replacement from
// leaving historical dictionary formats migrated ahead of the source corpus.
func TestStructuredMigrationRollback(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"module.c": `trice("literal={old}");`})()
	assert.NoError(t, FSys.WriteFile(FnJSON, []byte(`{"100":{"Type":"trice","Strg":"literal={old}"}}`), 0o644))
	beforeSource, beforeTIL := structuredTestFile(t, Srcs[0]), structuredTestFile(t, FnJSON)
	failing := &bindFailOnceRenameFs{Fs: FSys.Fs, destination: Srcs[0]}
	err := migrateLiteralBraces(io.Discard, &afero.Afero{Fs: failing})
	assert.ErrorContains(t, err, "injected rename failure")
	assert.True(t, failing.failed, "source commit must be reached after the TIL replacement")
	assert.Equal(t, beforeSource, structuredTestFile(t, Srcs[0]))
	assert.Equal(t, beforeTIL, structuredTestFile(t, FnJSON), "rollback restores the dictionary, including historical meanings")
}

// TestStructuredRegistryDryRunPreservesPublishedCounts covers a changed source
// after a successful run. A preview must leave the last published registry alone.
func TestStructuredRegistryDryRunPreservesPublishedCounts(t *testing.T) {
	for _, operation := range []string{"insert", "bind"} {
		t.Run(operation, func(t *testing.T) {
			defer prepareBindTest(t, map[string]string{"module.c": `trice("{old}", value);`})()
			oldDir := FieldsDir
			FieldsDir = BindDir
			t.Cleanup(func() { FieldsDir = oldDir })
			run := SubCmdIdInsert
			if operation == "bind" {
				run = SubCmdIdBind
			}
			assert.NoError(t, run(io.Discard, FSys))
			path := filepath.Join(BindDir, "trice-fields.txt")
			assert.Equal(t, "       1 old\n", string(structuredTestFile(t, path)))
			current := strings.ReplaceAll(string(structuredTestFile(t, Srcs[0])), "{old}", "{new}")
			assert.NoError(t, FSys.WriteFile(Srcs[0], []byte(current), 0o644))
			DryRun = true
			assert.NoError(t, run(io.Discard, FSys))
			assert.Equal(t, "       1 old\n", string(structuredTestFile(t, path)), "preview must not publish new counts")
			DryRun = false
			assert.NoError(t, run(io.Discard, FSys))
			assert.Equal(t, "       1 new\n", string(structuredTestFile(t, path)), "successful run replaces old counts")
		})
	}
}
