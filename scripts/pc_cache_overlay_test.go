// SPDX-License-Identifier: MIT

package scripts_test

import (
	"encoding/json"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"
	"time"

	"github.com/stretchr/testify/assert"
)

// cacheOverlayTool builds the real internal helper once for a group of cases.
// Its ignored build tag keeps the executable out of the scripts test package.
func cacheOverlayTool(t *testing.T) string {
	t.Helper()
	tool := filepath.Join(t.TempDir(), "pc-cache-overlay.exe")
	out, err := exec.Command("go", "build", "-o", tool, "pc_cache_overlay.go").CombinedOutput()
	if !assert.NoError(t, err, string(out)) {
		t.FailNow()
	}
	return tool
}

// cacheOverlayFixture includes shared C, a transitive header, package-local
// configuration and a generated sidecar. The second package uses an ABC-style
// bridge, so invalidation is not accidentally limited to generated harnesses.
func cacheOverlayFixture(t *testing.T) string {
	t.Helper()
	root := t.TempDir()
	// macOS aliases /var to /private/var; match Go's native overlay keys.
	root, err := filepath.EvalSymlinks(root)
	if !assert.NoError(t, err) {
		t.FailNow()
	}
	writeFixture(t, root, "go.mod", "module cacheprobe\n\ngo 1.20\n")
	writeFixture(t, root, "src/shared.h", "#define SHARED_VALUE 10\n")
	writeFixture(t, root, "src/value.c", "#include \"shared.h\"\n#include \"config.h\"\n#include \"sidecar.h\"\n#ifndef EXTRA\n#define EXTRA 0\n#endif\n#ifndef SOURCE_OFFSET\n#define SOURCE_OFFSET 0\n#endif\nint cache_value(void) { return SHARED_VALUE + LOCAL_VALUE + SIDECAR_VALUE + EXTRA + SOURCE_OFFSET; }\n")
	writeFixture(t, root, "generated/sidecar.h", "#define SIDECAR_VALUE 100\n")
	writeFixture(t, root, "_test/testdata/triceCheck.c", "#include \"../../src/value.c\"\n")
	source := "package cacheprobe\n\n/*\n#cgo CFLAGS: -I../../src -I. -I../../generated\n#include \"../testdata/triceCheck.c\"\n*/\nimport \"C\"\n\nfunc value() int { return int(C.cache_value()) }\n"
	writeFixture(t, root, "_test/testdata/cgoPackage.go", source)
	testSource := `package cacheprobe
import ("os"; "strconv"; "testing")
func TestCurrentCValue(t *testing.T) {
  want, err := strconv.Atoi(os.Getenv("WANT_VALUE"))
  if err != nil { t.Fatal(err) }
  if got := value(); got != want { t.Fatalf("C value: got %d, want %d", got, want) }
  f, err := os.OpenFile(os.Getenv("EXECUTION_LOG"), os.O_CREATE|os.O_APPEND|os.O_WRONLY, 0600)
  if err != nil { t.Fatal(err) }
  defer f.Close()
  if _, err := f.WriteString("executed\n"); err != nil { t.Fatal(err) }
}
`
	writeFixture(t, root, "_test/testdata/cgoPackage_test.go", testSource)
	writeFixture(t, root, "_test/a/generated_cgoPackage.go", source)
	writeFixture(t, root, "_test/a/generated_cgoPackage_test.go", testSource)
	writeFixture(t, root, "_test/a/config.h", "#define LOCAL_VALUE 1\n")
	writeFixture(t, root, "_test/b/cgo_bridge.go", source)
	writeFixture(t, root, "_test/b/cgo_bridge_test.go", testSource)
	writeFixture(t, root, "_test/b/config.h", "#define LOCAL_VALUE 2\n")
	return root
}

// runCacheOverlay retains each run in a different directory, exactly as the
// worker does. Stable cache identities must depend on content, not these paths.
func runCacheOverlay(t *testing.T, tool, root, workflow string, overrides ...map[string]string) (string, map[string]string, string, error) {
	t.Helper()
	dir := t.TempDir()
	overlay := filepath.Join(dir, "overlay.json")
	cmd := exec.Command(tool, "-out", overlay, "-inputs", filepath.Join(dir, "inputs"), "-workflow", workflow)
	cmd.Dir = filepath.Join(root, "_test")
	// Isolate header-search settings from the user's ARM or SDK environment.
	settings := map[string]string{"TRICE_BIND_INCLUDE_DIR": "", "CPATH": "", "C_INCLUDE_PATH": "", "CPLUS_INCLUDE_PATH": "", "SDKROOT": ""}
	for _, override := range overrides {
		for name, value := range override {
			settings[name] = value
		}
	}
	cmd.Env = cacheProbeEnv(settings)
	out, err := cmd.CombinedOutput()
	if err != nil {
		return overlay, nil, string(out), err
	}
	data, err := os.ReadFile(overlay)
	if !assert.NoError(t, err) {
		t.FailNow()
	}
	var result struct{ Replace map[string]string }
	if !assert.NoError(t, json.Unmarshal(data, &result)) {
		t.FailNow()
	}
	return overlay, result.Replace, string(out), nil
}

// cacheProbeEnv replaces settings rather than appending duplicate environment
// keys, including case-insensitive Windows names.
func cacheProbeEnv(settings map[string]string) []string {
	var result []string
	for _, entry := range os.Environ() {
		name := strings.SplitN(entry, "=", 2)[0]
		keep := true
		for key := range settings {
			if strings.EqualFold(name, key) {
				keep = false
			}
		}
		if keep {
			result = append(result, entry)
		}
	}
	for key, value := range settings {
		result = append(result, key+"="+value)
	}
	return result
}

// overlaySource reads the effective Go input rather than comparing temporary
// replacement filenames, which intentionally differ between invocations.
func overlaySource(t *testing.T, replacements map[string]string, root, file string) string {
	t.Helper()
	path, ok := replacements[filepath.Join(root, "_test", filepath.FromSlash(file))]
	if !assert.True(t, ok, "CGO input %s must be overlaid", file) {
		t.FailNow()
	}
	data, err := os.ReadFile(path)
	if !assert.NoError(t, err) {
		t.FailNow()
	}
	return string(data)
}

// TestPCCacheOverlayTracksContents covers all external input categories, and
// distinguishes content changes from mtime-only edits and per-package changes.
func TestPCCacheOverlayTracksContents(t *testing.T) {
	tool := cacheOverlayTool(t)
	for _, tc := range []struct {
		name, file, content, workflow string
		remove, changeA, changeB      bool
	}{
		{"identical_inputs_in_new_run_directory", "", "", "bind", false, false, false},
		{"mtime_only_does_not_rebuild", "src/shared.h", "", "bind", false, false, false},
		{"external_triceCheck_changes", "_test/testdata/triceCheck.c", "// changed switch\n", "bind", false, true, true},
		{"transitive_shared_header_changes", "src/shared.h", "#define SHARED_VALUE 20\n", "bind", false, true, true},
		{"generated_sidecar_changes", "generated/sidecar.h", "#define SIDECAR_VALUE 200\n", "bind", false, true, true},
		{"added_header_changes_manifest", "src/new.inc", "// new include\n", "bind", false, true, true},
		{"removed_header_changes_manifest", "src/shared.h", "", "bind", true, true, true},
		{"local_configuration_only_rebuilds_its_package", "_test/a/config.h", "#define LOCAL_VALUE 3\n", "bind", false, true, false},
		{"workflow_is_part_of_identity", "", "", "insert", false, true, true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := cacheOverlayFixture(t)
			_, before, out, err := runCacheOverlay(t, tool, root, "bind")
			if !assert.NoError(t, err, out) {
				t.FailNow()
			}
			if tc.file != "" {
				path := filepath.Join(root, filepath.FromSlash(tc.file))
				if tc.remove {
					assert.NoError(t, os.Remove(path))
				} else if tc.content != "" {
					writeFixture(t, root, tc.file, tc.content)
				} else {
					assert.NoError(t, os.Chtimes(path, time.Unix(1, 0), time.Unix(1, 0)))
				}
			}
			_, after, out, err := runCacheOverlay(t, tool, root, tc.workflow)
			if !assert.NoError(t, err, out) {
				t.FailNow()
			}
			for _, probe := range []struct {
				file    string
				changed bool
			}{{"a/generated_cgoPackage.go", tc.changeA}, {"b/cgo_bridge.go", tc.changeB}} {
				assert.Equal(t, probe.changed, overlaySource(t, before, root, probe.file) != overlaySource(t, after, root, probe.file), probe.file)
			}
			assert.Equal(t, filepath.Join(root, "_test/testdata/cgoPackage_test.go"), after[filepath.Join(root, "_test/a/generated_cgoPackage_test.go")])
		})
	}
	for _, missing := range []string{"src", "_test/testdata/cgoPackage.go", "_test/testdata/cgoPackage_test.go"} {
		t.Run("missing_input_aborts_"+missing, func(t *testing.T) {
			root := cacheOverlayFixture(t)
			assert.NoError(t, os.RemoveAll(filepath.Join(root, filepath.FromSlash(missing))))
			overlay, _, out, err := runCacheOverlay(t, tool, root, "bind")
			assert.Error(t, err)
			assert.Contains(t, out, "FAIL: PC cache overlay:")
			_, err = os.Stat(overlay)
			assert.True(t, os.IsNotExist(err), "never publish a partial overlay after an input error")
		})
	}
	t.Run("no_CGO_importers_aborts", func(t *testing.T) {
		root := cacheOverlayFixture(t)
		assert.NoError(t, os.RemoveAll(filepath.Join(root, "_test/a")))
		assert.NoError(t, os.RemoveAll(filepath.Join(root, "_test/b")))
		_, _, out, err := runCacheOverlay(t, tool, root, "bind")
		assert.Error(t, err)
		assert.Contains(t, out, "no PC-test CGO importers")
	})
	t.Run("malformed_CGO_importer_aborts", func(t *testing.T) {
		root := cacheOverlayFixture(t)
		writeFixture(t, root, "_test/b/cgo_bridge.go", "package cacheprobe\nimport (\n")
		_, _, out, err := runCacheOverlay(t, tool, root, "bind")
		assert.Error(t, err)
		assert.Contains(t, out, "read CGO importer")
	})
	t.Run("symbolic_link_does_not_hide_external_inputs", func(t *testing.T) {
		root := cacheOverlayFixture(t)
		if err := os.Symlink(filepath.Join(root, "generated/sidecar.h"), filepath.Join(root, "src/linked.h")); err != nil {
			t.Skipf("creating a symbolic link is not permitted: %v", err)
		}
		overlay, _, out, err := runCacheOverlay(t, tool, root, "bind")
		assert.Error(t, err)
		assert.Contains(t, out, "unsupported symbolic link")
		_, err = os.Stat(overlay)
		assert.True(t, os.IsNotExist(err))
	})
}

// TestPCCacheOverlayCustomSidecarsAndEnvironment prevents a false sense of
// safety when Bind uses a non-default generation directory or search flags.
func TestPCCacheOverlayCustomSidecarsAndEnvironment(t *testing.T) {
	tool := cacheOverlayTool(t)
	root := cacheOverlayFixture(t)
	writeFixture(t, root, "custom generated/sidecar.h", "#define SIDECAR_VALUE 300\n")
	settings := map[string]string{"TRICE_BIND_INCLUDE_DIR": filepath.Join(root, "custom generated")}
	_, before, out, err := runCacheOverlay(t, tool, root, "bind", settings)
	if !assert.NoError(t, err, out) {
		t.FailNow()
	}
	writeFixture(t, root, "custom generated/sidecar.h", "#define SIDECAR_VALUE 400\n")
	_, after, out, err := runCacheOverlay(t, tool, root, "bind", settings)
	if !assert.NoError(t, err, out) {
		t.FailNow()
	}
	assert.NotEqual(t, overlaySource(t, before, root, "a/generated_cgoPackage.go"), overlaySource(t, after, root, "a/generated_cgoPackage.go"))
	for _, variable := range []string{"CGO_CFLAGS", "CGO_CPPFLAGS", "C_INCLUDE_PATH", "CPATH", "SDKROOT"} {
		t.Run(variable, func(t *testing.T) {
			override := map[string]string{"TRICE_BIND_INCLUDE_DIR": settings["TRICE_BIND_INCLUDE_DIR"], variable: "changed setting"}
			_, changed, out, err := runCacheOverlay(t, tool, root, "bind", override)
			if !assert.NoError(t, err, out) {
				t.FailNow()
			}
			assert.NotEqual(t, overlaySource(t, after, root, "a/generated_cgoPackage.go"), overlaySource(t, changed, root, "a/generated_cgoPackage.go"))
		})
	}
	assert.NoError(t, os.RemoveAll(filepath.Join(root, "custom generated")))
	overlay, _, out, err := runCacheOverlay(t, tool, root, "bind", settings)
	assert.Error(t, err)
	assert.Contains(t, out, "read Bind PC cache inputs")
	_, err = os.Stat(overlay)
	assert.True(t, os.IsNotExist(err), "missing configured sidecars must not fall back to the default tree")
}

// TestPCCacheOverlayArtifactsStayOutOfGoDiscovery checks retained overlays
// inside the repository, where ordinary go list ./... must not see extra Go
// packages. Mixed CGO package names make an accidental discovery fail visibly.
func TestPCCacheOverlayArtifactsStayOutOfGoDiscovery(t *testing.T) {
	tool := cacheOverlayTool(t)
	root := cacheOverlayFixture(t)
	writeFixture(t, root, "_test/b/cgo_bridge.go", "package anotherprobe\nimport \"C\"\n")
	dir := filepath.Join(root, "temp", "log", "pc-run", "cache-inputs")
	cmd := exec.Command(tool, "-out", filepath.Join(root, "overlay.json"), "-inputs", dir, "-workflow", "insert")
	cmd.Dir = filepath.Join(root, "_test")
	cmd.Env = cacheProbeEnv(map[string]string{"TRICE_BIND_INCLUDE_DIR": ""})
	out, err := cmd.CombinedOutput()
	if !assert.NoError(t, err, string(out)) {
		t.FailNow()
	}
	writeFixture(t, root, "active/active.go", "package active\n")
	cmd = exec.Command("go", "list", "./...")
	cmd.Dir = root
	cmd.Env = cacheProbeEnv(map[string]string{"GOFLAGS": "", "GOENV": "off"})
	out, err = cmd.CombinedOutput()
	assert.NoError(t, err, string(out))
	assert.Equal(t, "cacheprobe/active\n", string(out), "only real source packages may be discovered")
}

// TestPCCacheOverlayRealCGOBuilds proves both sides of the contract: unchanged
// builds reuse compiled C, but every -count=1 run actually executes its test.
// Mutations keep original mtimes to rule out accidental timestamp invalidation.
func TestPCCacheOverlayRealCGOBuilds(t *testing.T) {
	if out, err := exec.Command("go", "env", "CGO_ENABLED").Output(); err != nil || strings.TrimSpace(string(out)) != "1" {
		t.Skip("real CGO cache proof requires CGO_ENABLED=1 and a working host compiler")
	}
	tool := cacheOverlayTool(t)
	root := cacheOverlayFixture(t)
	// Use a fresh cache for the cold/warm proof, shared by all its runs.
	cache := filepath.Join(t.TempDir(), "build-cache")
	executionLog := filepath.Join(root, "executions.txt")
	for index, tc := range []struct {
		name, file, content, workflow, flags string
		wantA, wantB                         int
		compileA, compileB                   bool
	}{
		{"cold_build", "", "", "bind", "", 111, 112, true, true},
		{"warm_build_still_executes_tests", "", "", "bind", "", 111, 112, false, false},
		{"C_source_change_is_not_stale", "_test/testdata/triceCheck.c", "#define SOURCE_OFFSET 5\n#include \"../../src/value.c\"\n", "bind", "", 116, 117, true, true},
		{"shared_header_change_reaches_both_packages", "src/shared.h", "#define SHARED_VALUE 20\n", "bind", "", 126, 127, true, true},
		{"sidecar_change_reaches_both_packages", "generated/sidecar.h", "#define SIDECAR_VALUE 200\n", "bind", "", 226, 227, true, true},
		{"configuration_change_is_local", "_test/a/config.h", "#define LOCAL_VALUE 3\n", "bind", "", 228, 227, true, false},
		{"compiler_option_changes_runtime_value", "", "", "bind", "-DEXTRA=7", 235, 234, true, true},
		{"warm_build_with_same_options", "", "", "bind", "-DEXTRA=7", 235, 234, false, false},
		{"workflow_change_rebuilds", "", "", "insert", "-DEXTRA=7", 235, 234, true, true},
		{"warm_insert_build_executes_again", "", "", "insert", "-DEXTRA=7", 235, 234, false, false},
	} {
		if !t.Run(tc.name, func(t *testing.T) {
			if tc.file != "" {
				path := filepath.Join(root, filepath.FromSlash(tc.file))
				info, err := os.Stat(path)
				if !assert.NoError(t, err) {
					t.FailNow()
				}
				writeFixture(t, root, tc.file, tc.content)
				assert.NoError(t, os.Chtimes(path, info.ModTime(), info.ModTime()))
			}
			overlay, _, out, err := runCacheOverlay(t, tool, root, tc.workflow)
			if !assert.NoError(t, err, out) {
				t.FailNow()
			}
			for _, probe := range []struct {
				pkg, generatedC string
				want            int
				compile         bool
			}{{"a", "generated_cgoPackage.cgo2.c", tc.wantA, tc.compileA}, {"b", "cgo_bridge.cgo2.c", tc.wantB, tc.compileB}} {
				cmd := exec.Command("go", "test", "-x", "-v", "-count=1", "-overlay="+overlay, "./_test/"+probe.pkg)
				cmd.Dir = root
				cmd.Env = cacheProbeEnv(map[string]string{"GOCACHE": cache, "GOFLAGS": "", "GOENV": "off", "CGO_CFLAGS": tc.flags, "CPATH": "", "C_INCLUDE_PATH": "", "CPLUS_INCLUDE_PATH": "", "WANT_VALUE": fmt.Sprint(probe.want), "EXECUTION_LOG": executionLog})
				output, err := cmd.CombinedOutput()
				if !assert.NoError(t, err, string(output)) {
					t.FailNow()
				}
				assert.Equal(t, probe.compile, strings.Contains(string(output), probe.generatedC), "compiler invocation for %s; output:\n%s", probe.pkg, output)
			}
			data, err := os.ReadFile(executionLog)
			assert.NoError(t, err)
			assert.Equal(t, 2*(index+1), strings.Count(string(data), "executed\n"), "both tests must execute again, even on a warm build")
		}) {
			break // Later expectations depend on earlier mutations; avoid cascading failures.
		}
	}
}
