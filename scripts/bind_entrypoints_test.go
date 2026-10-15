// SPDX-License-Identifier: MIT

package scripts_test

import (
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

// TestLocalBindBuilds checks public entry points without invoking a compiler
// matrix. Tool doubles verify ordering, refreshed headers, and private metadata.
func TestLocalBindBuilds(t *testing.T) {
	for _, project := range []string{"F030_inst_rtt", "F030_inst_uart", "G0B1_features"} {
		for _, state := range []string{"missing", "current", "stale", "bind_failure", "compiler_failure"} {
			t.Run(project+"/"+state, func(t *testing.T) {
				entry := "build.sh"
				scripts := []string{"examples/" + project + "/" + entry}
				if project == "G0B1_features" {
					scripts = append(scripts, "examples/"+project+"/demo_build.sh")
				}
				root := scriptFixture(t, scripts...)
				projectDir := "examples/" + project
				// Preexisting ownership and shared dictionaries must survive each path.
				writeFixture(t, root, projectDir+"/li.json", "local ownership\n")
				writeFixture(t, root, "demoTIL.json", "global TIL\n")
				writeFixture(t, root, "demoLI.json", "global LI\n")
				generated := "out/sidecars"
				if project == "G0B1_features" {
					generated = "generated"
				}
				if state == "current" || state == "stale" {
					writeFixture(t, root, projectDir+"/"+generated+"/probe.h", state+"\n")
				}
				writeFixture(t, root, "bin/arm-none-eabi-gcc", "#!/bin/sh\nexit 0\n")
				writeFixture(t, root, "bin/trice", `#!/bin/sh
set -eu
[ "$1" = bind ]
[ "$(cat li.json)" = 'local ownership' ]
printf '%s\n' "$@" > arguments
[ "$TEST_STATE" != bind_failure ] || exit 37
mkdir -p "$TEST_GENERATED"
printf 'refreshed\n' > "$TEST_GENERATED/probe.h"
`)
				writeFixture(t, root, "bin/make", `#!/bin/sh
set -eu
[ "$(cat "$TEST_GENERATED/probe.h")" = refreshed ]
printf '%s\n' "$@" > make-called
[ "$TEST_STATE" != compiler_failure ] || exit 41
`)
				out, err := runFixture(t, root, "bash examples/"+project+"/build.sh", map[string]string{
					"TEST_STATE": state, "TEST_GENERATED": generated,
				})
				if state == "bind_failure" || state == "compiler_failure" {
					assert.Error(t, err, out)
				} else {
					assert.NoError(t, err, out)
				}
				if state == "bind_failure" {
					assert.NoFileExists(t, filepath.Join(root, projectDir, "make-called"))
				} else {
					assert.FileExists(t, filepath.Join(root, projectDir, "make-called"))
				}
				args, readErr := os.ReadFile(filepath.Join(root, projectDir, "arguments"))
				assert.NoError(t, readErr)
				assert.Contains(t, string(args), "-genDir\n"+generated+"\n")
				assert.NotContains(t, string(args), "demoTIL.json")
				for file, want := range map[string]string{
					"demoTIL.json": "global TIL\n", "demoLI.json": "global LI\n",
					projectDir + "/li.json": "local ownership\n",
				} {
					got, readErr := os.ReadFile(filepath.Join(root, file))
					assert.NoError(t, readErr)
					assert.Equal(t, want, string(got), file)
				}
			})
		}
	}
}

// TestFeatureClangUsesPrivateBind ensures the Clang wrapper reaches the same
// CE-aware preparation as the public GCC build, rather than the global ID scan.
func TestFeatureClangUsesPrivateBind(t *testing.T) {
	root := scriptFixture(t, "examples/G0B1_features/build_with_clang.sh")
	writeFixture(t, root, "scripts/_150_setup_build_environment.sh", "# fixture: toolchain already configured\n")
	writeFixture(t, root, "examples/G0B1_features/demo_build.sh", "#!/bin/sh\nprintf '%s\\n' \"$@\" > \"$(dirname \"$0\")/called\"\nexit 29\n")
	out, err := runFixture(t, root, "bash examples/G0B1_features/build_with_clang.sh", map[string]string{"TRICE_ID_WORKFLOW_OWNER": "0"})
	assert.Error(t, err, out)
	called, err := os.ReadFile(filepath.Join(root, "examples/G0B1_features/called"))
	assert.NoError(t, err)
	assert.Equal(t, "clang\n", string(called))
}

// TestClangBuildRespectsPreparationOwner distinguishes a direct Bind build from
// managed Bind/Insert, including preparation failure and compiler cancellation.
func TestClangBuildRespectsPreparationOwner(t *testing.T) {
	for _, workflow := range []string{"standalone", "bind", "insert"} {
		for _, outcome := range []string{"success", "bind_failure", "compiler_failure", "termination"} {
			t.Run(workflow+"/"+outcome, func(t *testing.T) {
				root := scriptFixture(t, "examples/G0B1_inst/build_with_clang.sh", "examples/prepareTriceBind.sh")
				writeFixture(t, root, "scripts/_150_setup_build_environment.sh", "MAKE_JOBS=-j1\n")
				writeFixture(t, root, "trice_bindIDs_in_examples_and_test_folder.sh", `#!/bin/sh
printf 'bind\n' >> "$FIXTURE_ROOT/preparation"
[ "$OUTCOME" != bind_failure ] || exit 37
mkdir -p "$FIXTURE_ROOT/generated"
`)
				writeFixture(t, root, "bin/trice", "#!/bin/sh\nexit 0\n")
				writeFixture(t, root, "bin/make", `#!/bin/sh
set -eu
[ "$TRICE_ID_WORKFLOW_OWNER" = 1 ]
[ "$1" != clean ] || exit 0
printf 'compile\n' > "$FIXTURE_ROOT/compiled"
case "$OUTCOME" in
  compiler_failure) exit 43 ;;
  termination) kill -TERM "$PPID" ;;
esac
`)
				// Managed workflows must never delegate preparation or cleanup back
				// to the legacy scripts; the outer wrapper owns exact restoration.
				for _, name := range []string{"_230_legacy_insert_ids.sh", "_240_legacy_clean_ids.sh"} {
					writeFixture(t, root, "scripts/"+name, "#!/bin/sh\nexit 89\n")
				}
				env := map[string]string{
					"FIXTURE_ROOT": root, "OUTCOME": outcome, "TRICE_ID_WORKFLOW_OWNER": "0",
					"TRICE_ID_WORKFLOW": "", "TRICE_BIND_INCLUDE_DIR": "",
				}
				if workflow != "standalone" {
					env["TRICE_ID_WORKFLOW_OWNER"] = "1"
					env["TRICE_ID_WORKFLOW"] = workflow
					env["TRICE_BIND_INCLUDE_DIR"] = filepath.Join(root, "generated")
					assert.NoError(t, os.MkdirAll(env["TRICE_BIND_INCLUDE_DIR"], 0755))
				}
				out, err := runFixture(t, root, "bash examples/G0B1_inst/build_with_clang.sh", env)
				wantFailure := outcome == "compiler_failure" || outcome == "termination" || (outcome == "bind_failure" && workflow == "standalone")
				if wantFailure {
					assert.Error(t, err, out)
				} else {
					assert.NoError(t, err, out)
				}
				if workflow == "standalone" {
					prepared, err := os.ReadFile(filepath.Join(root, "preparation"))
					assert.NoError(t, err)
					assert.Equal(t, "bind\n", string(prepared))
				} else {
					assert.NoFileExists(t, filepath.Join(root, "preparation"))
				}
				if outcome == "bind_failure" && workflow == "standalone" {
					assert.NoFileExists(t, filepath.Join(root, "compiled"))
				} else {
					assert.FileExists(t, filepath.Join(root, "compiled"))
				}
			})
		}
	}
}

// copyEntrypointCheckout copies tracked, current inputs into a disposable path
// containing spaces. Local build products and user notes cannot leak into it.
func copyEntrypointCheckout(t *testing.T, paths ...string) string {
	t.Helper()
	root := filepath.Join(t.TempDir(), "checkout with spaces")
	cmd := exec.Command("git", append([]string{"ls-files", "-z", "--"}, paths...)...)
	cmd.Dir = ".."
	files, err := cmd.Output()
	if !assert.NoError(t, err) {
		t.FailNow()
	}
	for _, name := range strings.Split(strings.TrimSuffix(string(files), "\x00"), "\x00") {
		data, err := os.ReadFile(filepath.Join("..", name))
		if !assert.NoError(t, err, name) {
			t.FailNow()
		}
		writeFixture(t, root, name, string(data))
	}
	return root
}

// TestBindEntrypointIntegration exercises real CMake and all nine ABC nodes.
// It is opt-in because ordinary script tests use tool doubles, not C builds.
func TestBindEntrypointIntegration(t *testing.T) {
	if os.Getenv("TRICE_BIND_INTEGRATION") != "1" {
		t.Skip("set TRICE_BIND_INTEGRATION=1 for real ABC and CMake builds")
	}
	for _, tool := range []string{"git", "go", "cmake"} {
		if _, err := exec.LookPath(tool); !assert.NoError(t, err, "integration requires "+tool) {
			t.FailNow()
		}
	}
	// Match the public script's supported host compilers, including MinGW GCC.
	compiler := ""
	for _, candidate := range []string{"cc", "gcc", "clang"} {
		if _, err := exec.LookPath(candidate); err == nil {
			compiler = candidate
			break
		}
	}
	if !assert.NotEmpty(t, compiler, "integration requires a host C compiler") {
		t.FailNow()
	}
	// Build the current CLI once; no installed version or stale temp binary is used.
	binary := filepath.Join(t.TempDir(), "trice")
	if runtime.GOOS == "windows" {
		binary += ".exe"
	}
	cmd := exec.Command("go", "build", "-o", binary, "./cmd/trice")
	cmd.Dir = ".."
	out, err := cmd.CombinedOutput()
	if !assert.NoError(t, err, string(out)) {
		t.FailNow()
	}

	t.Run("ABC_Bind_and_Insert", func(t *testing.T) {
		root := copyEntrypointCheckout(t, "examples/TriceAbc", "src", "scripts/_120_setup_trice_environment.sh", "demoTIL.json")
		original, err := os.ReadFile(filepath.Join(root, "examples/TriceAbc/N1_tx/main.c"))
		assert.NoError(t, err)
		selection, err := os.ReadFile(filepath.Join(root, "examples/TriceAbc/NodeLib/nodeAbc.h"))
		assert.NoError(t, err)
		til, err := os.ReadFile(filepath.Join(root, "demoTIL.json"))
		assert.NoError(t, err)
		// Isolated metadata proves generation honors the configured table instead
		// of silently reverting to demoTIL.json. Shared reference files stay intact.
		writeFixture(t, root, "metadata/til.json", string(til))
		writeFixture(t, root, "metadata/li.json", "")
		for _, workflow := range []string{"bind", "insert"} {
			t.Run(workflow, func(t *testing.T) {
				out, err := runFixture(t, root, "bash examples/TriceAbc/demo.sh", map[string]string{
					"TRICE_BIN": binary, "TRICE_ID_WORKFLOW": workflow, "CC": compiler,
					"TRICE_TIL_JSON": "metadata/til.json",
					"TRICE_LI_JSON":  "metadata/li.json", "TRICE_LI_ROOT": ".",
				})
				if !assert.NoError(t, err, out) {
					t.FailNow()
				}
				assert.Equal(t, 9, strings.Count(out, "build: N"), out)
				assert.Contains(t, out, "N6_rx: log:(from node=N1_tx) text=N1 says hi")
				assert.Contains(t, out, "abc:DivideResult")
				assert.Contains(t, out, "N1_tx: TX-> x0 3 bytes")
				assert.NotContains(t, out, "rx decode error")
				for _, node := range []string{"N1_tx", "N2_tx", "N3_bi", "N4_rx", "N5_rx", "N6_rx", "N7_bi", "N8_bi", "N9_bi"} {
					suffix := ""
					if runtime.GOOS == "windows" {
						suffix = ".exe"
					}
					assert.FileExists(t, filepath.Join(root, "examples/TriceAbc/build", node+suffix))
				}
				for file, want := range map[string][]byte{
					"examples/TriceAbc/N1_tx/main.c":      original,
					"examples/TriceAbc/NodeLib/nodeAbc.h": selection, "demoTIL.json": til,
				} {
					got, err := os.ReadFile(filepath.Join(root, file))
					assert.NoError(t, err)
					assert.Equal(t, want, got, file)
				}
			})
			for _, failure := range []string{"compiler_failure", "termination"} {
				t.Run(workflow+"/"+failure, func(t *testing.T) {
					// A failed compiler and a delivered TERM both leave original
					// sources intact; Insert still cleans its private source copy.
					compiler := "#!/bin/sh\nexit 43\n"
					wantStatus := "exit status 43"
					if failure == "termination" {
						compiler = "#!/bin/sh\nkill -TERM \"$PPID\"\nexit 0\n"
						wantStatus = "exit status 143"
					}
					writeFixture(t, root, "bin/failing-cc", compiler)
					out, err := runFixture(t, root, "bash examples/TriceAbc/build.sh", map[string]string{
						"TRICE_BIN": binary, "TRICE_ID_WORKFLOW": workflow, "CC": filepath.Join(root, "bin/failing-cc"),
						"TRICE_TIL_JSON": filepath.Join(root, "metadata/til.json"),
						"TRICE_LI_JSON":  filepath.Join(root, "metadata/li.json"), "TRICE_LI_ROOT": root,
					})
					if assert.Error(t, err, out) {
						assert.Equal(t, wantStatus, err.Error(), out)
					}
					preserved, err := os.ReadFile(filepath.Join(root, "examples/TriceAbc/N1_tx/main.c"))
					assert.NoError(t, err)
					assert.Equal(t, original, preserved)
					if workflow == "insert" {
						assert.Contains(t, out, "cleanup: trice clean")
					}
				})
			}
		}
	})

	t.Run("CLion_private_headers_are_rebuilt", func(t *testing.T) {
		root := copyEntrypointCheckout(t, "_test/clion-review", "_test/testdata/triceCheck.c", "src", "demoTIL.json")
		build := filepath.Join(root, "review build")
		// The source hash comparison also covers automatic CMake reconfiguration.
		original, err := os.ReadFile(filepath.Join(root, "_test/testdata/triceCheck.c"))
		assert.NoError(t, err)
		for _, args := range [][]string{
			{"-S", filepath.Join(root, "_test/clion-review"), "-B", build, "-DTRICE_EXECUTABLE=" + binary},
			{"--build", build, "--parallel", "2"},
		} {
			out, err := exec.Command("cmake", args...).CombinedOutput()
			if !assert.NoError(t, err, string(out)) {
				t.FailNow()
			}
		}
		headers, err := filepath.Glob(filepath.Join(build, "sidecars", "*.h"))
		assert.NoError(t, err)
		if !assert.NotEmpty(t, headers) {
			t.FailNow()
		}
		// Missing headers are regenerated; changed source text triggers CMake's
		// configure dependency and replaces valid but outdated Bind metadata.
		assert.NoError(t, os.Remove(headers[0]))
		out, err := exec.Command("cmake", "--build", build, "--parallel", "2").CombinedOutput()
		assert.NoError(t, err, string(out))
		assert.FileExists(t, headers[0])
		changed := strings.ReplaceAll(string(original), "err:file not found (error code 7)", "err:updated review message")
		assert.NotEqual(t, string(original), changed)
		assert.NoError(t, os.WriteFile(filepath.Join(root, "_test/testdata/triceCheck.c"), []byte(changed), 0644))
		out, err = exec.Command("cmake", "--build", build, "--parallel", "2").CombinedOutput()
		assert.NoError(t, err, string(out))
		metadata, err := os.ReadFile(filepath.Join(build, "til.json"))
		assert.NoError(t, err)
		assert.Contains(t, string(metadata), "err:updated review message")
		preserved, err := os.ReadFile(filepath.Join(root, "_test/testdata/triceCheck.c"))
		assert.NoError(t, err)
		assert.Equal(t, changed, string(preserved))
		assert.NoFileExists(t, filepath.Join(root, "demoLI.json"))
		// A manually edited generated helper is deliberately rejected by Bind;
		// CMake must preserve the diagnostic rather than silently overwrite it.
		if len(headers) > 1 {
			assert.NoError(t, os.WriteFile(headers[1], []byte("#error modified sidecar\n"), 0644))
			out, err = exec.Command("cmake", "--build", build, "--parallel", "2").CombinedOutput()
			assert.Error(t, err, string(out))
			assert.Contains(t, string(out), "was modified")
		}
	})
}
