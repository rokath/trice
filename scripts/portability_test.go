// SPDX-License-Identifier: MIT

package scripts_test

import (
	"encoding/json"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

// writeFixture confines executable tool doubles and input files to one test's
// disposable repository. No test runs the real compiler matrices or formatter.
func writeFixture(t *testing.T, root, name, content string) {
	t.Helper()
	path := filepath.Join(root, name)
	if !assert.NoError(t, os.MkdirAll(filepath.Dir(path), 0755)) {
		t.FailNow()
	}
	if !assert.NoError(t, os.WriteFile(path, []byte(content), 0755)) {
		t.FailNow()
	}
}

// scriptFixture copies only the production scripts under test; their normal
// relative-root discovery then resolves into the isolated fixture repository.
func scriptFixture(t *testing.T, scripts ...string) string {
	t.Helper()
	root := t.TempDir()
	for _, name := range scripts {
		data, err := os.ReadFile(filepath.Join("..", name))
		if !assert.NoError(t, err) {
			t.FailNow()
		}
		writeFixture(t, root, name, string(data))
	}
	return root
}

// runFixture preserves the host's basic shell utilities while replacing tools
// explicitly. Environment overrides are unique, including case-insensitive
// Windows names, so a user's LOG_DIR or tool selection cannot escape the fixture.
func runFixture(t *testing.T, root, command string, overrides map[string]string) (string, error) {
	t.Helper()
	bash, err := exec.LookPath("bash")
	if err != nil {
		t.Skip("bash not installed; shell portability tests require a POSIX-compatible environment")
	}
	env := map[string]string{
		"LOG_DIR": filepath.Join(root, "temp", "log"), "TRICE_TMP_DIR": filepath.Join(root, "temp"),
		"TRICE_BIN_DIR": filepath.Join(root, "bin"), "PATH": filepath.Join(root, "bin") + string(os.PathListSeparator) + os.Getenv("PATH"),
		"GOCACHE": filepath.Join(root, "cache"), "NO_COLOR": "1",
	}
	for key, value := range overrides {
		env[key] = value
	}
	cmd := exec.Command(bash, "-c", command)
	cmd.Dir = root
	for _, entry := range os.Environ() {
		key := strings.SplitN(entry, "=", 2)[0]
		keep := true
		for override := range env {
			if strings.EqualFold(key, override) {
				keep = false
				break
			}
		}
		if keep {
			cmd.Env = append(cmd.Env, entry)
		}
	}
	for key, value := range env {
		cmd.Env = append(cmd.Env, key+"="+value)
	}
	output, err := cmd.CombinedOutput()
	return string(output), err
}

// initGitFixture gives runner tests a real index while keeping every Git write
// inside their disposable repository. The tracked file may then start dirty.
func initGitFixture(t *testing.T, root string) {
	t.Helper()
	writeFixture(t, root, "tracked.txt", "committed state\n")
	for _, args := range [][]string{{"init", "-q", root}, {"-C", root, "add", "tracked.txt"}} {
		out, err := exec.Command("git", args...).CombinedOutput()
		if !assert.NoError(t, err, string(out)) {
			t.FailNow()
		}
	}
}

// TestRunnerLogsAndCancellation checks cleanup boundaries, quiet skip reporting,
// fail-fast/no-stop behavior and cancellation without launching the real suite.
func TestRunnerLogsAndCancellation(t *testing.T) {
	for _, tc := range []struct {
		name, args, status, result string
		later                      bool
	}{
		{"pass", "quick", "0", "PASS", true},
		{"fail_fast", "quick", "1", "FAIL", false},
		{"no_stop", "quick --no-stop", "1", "FAIL", true},
		{"interrupt", "quick --no-stop", "130", "ABORTED", false},
		{"terminate", "quick --no-stop", "143", "ABORTED", false},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_110_test_runner.sh")
			initGitFixture(t, root)
			for _, name := range []string{"_testAll_07_GoCoverage.log", "_640_test_pc_targets_bind.log", "_600_test_gcc_bind.log.standalone.PC_log", "coverage.out"} {
				writeFixture(t, root, "temp/log/"+name, "stale failure")
			}
			for _, name := range []string{"notes.txt", "application.log", "trice-state.saved/recovery.tar"} {
				writeFixture(t, root, "temp/log/"+name, "retain")
			}
			writeFixture(t, root, "scripts/_410_test_first.sh", "#!/usr/bin/env bash\nsource scripts/_100_test_common.sh\ninit_logfile\nlog 'SKIP: unsupported tool version'\nexit "+tc.status+"\n")
			if tc.status == "1" {
				writeFixture(t, root, "scripts/_410_test_first.sh", "#!/usr/bin/env bash\nsource scripts/_100_test_common.sh\ninit_logfile\nlog 'EXPECTATION FAILURE triceCheck.c:123 channel=direct'\nlog 'want: hello; got: wrong'\nexit 1\n")
			}
			writeFixture(t, root, "scripts/_430_test_later.sh", "#!/usr/bin/env bash\nsource scripts/_100_test_common.sh\ninit_logfile\nlog executed\n")
			out, err := runFixture(t, root, "source scripts/_110_test_runner.sh; build_test_plan() { add_plan_step _410_test_first.sh; add_plan_step _430_test_later.sh; }; main "+tc.args, nil)
			if tc.status == "0" {
				assert.NoError(t, err, out)
				assert.Contains(t, out, "WARN")
				assert.Contains(t, out, "SKIP: unsupported tool version")
			} else {
				assert.Error(t, err, out)
			}
			assert.Contains(t, out, "Result: "+tc.result)
			if tc.status == "1" {
				assert.Contains(t, out, "Details:")
				assert.Contains(t, out, "_410_test_first.log")
				assert.Contains(t, out, "EXPECTATION FAILURE triceCheck.c:123 channel=direct")
				assert.Contains(t, out, "want: hello; got: wrong")
			}
			_, err = os.Stat(filepath.Join(root, "temp/log/_430_test_later.log"))
			assert.Equal(t, tc.later, err == nil, out)
			for _, name := range []string{"_testAll_07_GoCoverage.log", "_640_test_pc_targets_bind.log", "_600_test_gcc_bind.log.standalone.PC_log", "coverage.out"} {
				_, err := os.Stat(filepath.Join(root, "temp/log", name))
				assert.True(t, os.IsNotExist(err), name)
			}
			for _, name := range []string{"notes.txt", "application.log", "trice-state.saved/recovery.tar"} {
				data, err := os.ReadFile(filepath.Join(root, "temp/log", name))
				assert.NoError(t, err)
				assert.Equal(t, "retain", string(data))
			}
		})
	}
}

// TestRunnerComparesActualTrackedBytes demonstrates why comparing only Git
// status is insufficient: both runs start with a dirty tracked file, but only
// the case that changes its bytes again must fail and name the affected path.
func TestRunnerComparesActualTrackedBytes(t *testing.T) {
	for _, tc := range []struct {
		name, stepBody, wantResult string
	}{
		{"unchanged_dirty_file", "true", "PASS"},
		{"changed_dirty_file", "printf 'changed by test\\n' > tracked.txt", "FAIL"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_110_test_runner.sh")
			initGitFixture(t, root)
			writeFixture(t, root, "tracked.txt", "dirty before test\n")
			writeFixture(t, root, "scripts/_410_test_change.sh", "#!/usr/bin/env bash\nsource scripts/_100_test_common.sh\ninit_logfile\n"+tc.stepBody+"\n")
			out, err := runFixture(t, root, "source scripts/_110_test_runner.sh; build_test_plan() { add_plan_step _410_test_change.sh; }; main quick", nil)
			if tc.wantResult == "PASS" {
				assert.NoError(t, err, out)
				assert.NotContains(t, out, "tracked.txt")
			} else {
				assert.Error(t, err, out)
				assert.Contains(t, out, "tracked.txt")
				assert.Contains(t, out, "initial state was not restored")
			}
			assert.Contains(t, out, "Result: "+tc.wantResult)
		})
	}
}

// TestManagedExampleStateRestoration checks that the same snapshot protects
// pre-existing local data and initially absent paths after success, a worker
// error, and interruption. A fake Bind preparation and worker mutate distinct
// central and example paths so each part of the restore contract is observable.
func TestManagedExampleStateRestoration(t *testing.T) {
	for _, tc := range []struct {
		name, workerEnd, wantStatus string
	}{
		{"success", "exit 0", "end status: 0"},
		{"worker_failure", "exit 7", "end status: 7"},
		{"signal", "kill -TERM \"$PPID\"; exit 0", "end status: 143"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_120_setup_trice_environment.sh", "scripts/_130_trice_id_workflow.sh", "scripts/_140_trice_test_state.sh")
			initial := map[string]string{
				"sample.c":                                 "dirty source\n",
				"demoTIL.json":                             "dirty central TIL\n",
				"demoLI.json":                              "dirty central LI\n",
				"generated/user-selection.h":               "user selection\n",
				"examples/PC_log/til.json":                 "old PC TIL\n",
				"examples/PC_log/li.json":                  "old PC LI\n",
				"examples/PC_log/generated/user-choice.h":  "PC user choice\n",
				"examples/PC_log/build/user-object.o":      "PC build bytes\n",
				"examples/G0B1_log/li.json":                "old MCU LI\n",
				"examples/G0B1_log/out.gcc/user-object.o":  "MCU build bytes\n",
				"examples/G0B1_inst/out.gcc/user-object.o": "other target bytes\n",
			}
			for path, contents := range initial {
				writeFixture(t, root, path, contents)
			}
			writeFixture(t, root, "bin/trice", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "trice_bindIDs_in_examples_and_test_folder.sh", "#!/bin/bash\nprintf 'bound source\\n' > sample.c\nprintf 'bound central TIL\\n' > demoTIL.json\nprintf 'bound central LI\\n' > demoLI.json\nprintf 'new selection\\n' > generated/user-selection.h\n")
			writeFixture(t, root, "worker.sh", "#!/bin/bash\nprintf 'updated PC TIL\\n' > examples/PC_log/til.json\nprintf 'updated PC LI\\n' > examples/PC_log/li.json\nprintf 'updated MCU LI\\n' > examples/G0B1_log/li.json\nprintf 'new MCU TIL\\n' > examples/G0B1_log/til.json\nmkdir -p examples/G0B1_log/generated\nprintf 'new header\\n' > examples/G0B1_log/generated/sidecar.h\nprintf 'overwrite\\n' > examples/PC_log/generated/user-choice.h\nprintf 'overwrite\\n' > examples/PC_log/build/user-object.o\nprintf 'overwrite\\n' > examples/G0B1_log/out.gcc/user-object.o\nprintf 'overwrite\\n' > examples/G0B1_inst/out.gcc/user-object.o\n"+tc.workerEnd+"\n")
			out, err := runFixture(t, root, "source scripts/_140_trice_test_state.sh; trice_test_run_managed_workflow bind example ./worker.sh", map[string]string{"TRICE_PRJ_FILES": "-src ./sample.c"})
			if tc.name == "success" {
				assert.NoError(t, err, out)
			} else {
				assert.Error(t, err, out)
			}
			assert.Contains(t, out, tc.wantStatus)
			assert.Contains(t, out, "exact initial files restored")
			for path, contents := range initial {
				data, readErr := os.ReadFile(filepath.Join(root, path))
				assert.NoError(t, readErr, path)
				assert.Equal(t, contents, string(data), path)
			}
			for _, path := range []string{"examples/G0B1_log/til.json", "examples/G0B1_log/generated"} {
				_, statErr := os.Lstat(filepath.Join(root, path))
				assert.True(t, os.IsNotExist(statErr), path)
			}
		})
	}
}

// TestRuntimePreparationIsTransactional ensures step 490 does not leave its
// canonical Bind setup in the checkout, even if the preparatory bind fails.
func TestRuntimePreparationIsTransactional(t *testing.T) {
	for _, tc := range []struct {
		name, bindExit string
	}{
		{"success", "0"},
		{"bind_failure", "9"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t,
				"scripts/_100_test_common.sh", "scripts/_120_setup_trice_environment.sh",
				"scripts/_130_trice_id_workflow.sh", "scripts/_140_trice_test_state.sh",
				"scripts/_490_test_runtime_prepare.sh")
			initial := map[string]string{
				"sample.c":              "initial source\n",
				"demoTIL.json":          "initial TIL\n",
				"demoLI.json":           "initial LI\n",
				"generated/selection.h": "initial selection\n",
			}
			for path, contents := range initial {
				writeFixture(t, root, path, contents)
			}
			writeFixture(t, root, "bin/trice", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "trice_bindIDs_in_examples_and_test_folder.sh", "#!/bin/sh\nprintf 'changed source\\n' > sample.c\nprintf 'changed TIL\\n' > demoTIL.json\nprintf 'changed LI\\n' > demoLI.json\nprintf 'changed selection\\n' > generated/selection.h\nexit "+tc.bindExit+"\n")
			out, err := runFixture(t, root, "bash scripts/_490_test_runtime_prepare.sh", map[string]string{"TRICE_PRJ_FILES": "-src ./sample.c"})
			if tc.bindExit == "0" {
				assert.NoError(t, err, out)
			} else {
				assert.Error(t, err, out)
				assert.Contains(t, out, "preparing canonical Bind state failed")
			}
			assert.Contains(t, out, "exact initial files restored")
			for path, contents := range initial {
				data, readErr := os.ReadFile(filepath.Join(root, path))
				assert.NoError(t, readErr, path)
				assert.Equal(t, contents, string(data), path)
			}
		})
	}
}

// TestExampleCleanupPreservesFailureStatus executes each affected build script
// in a disposable checkout. The fake build and ID helpers fail independently,
// making it observable whether clean errors replace success or mask the
// original build or signal status. No production sources are touched.
func TestExampleCleanupPreservesFailureStatus(t *testing.T) {
	scripts := []string{
		"examples/L432_inst/build.sh",
		"examples/F030_inst/build.sh",
		"examples/G0B1_inst/build_with_clang.sh",
		"examples/G0B1_features/build_with_clang.sh",
	}
	cases := []struct {
		name, buildStatus, cleanStatus, signal, wantStatus string
		wantWarning                                        bool
	}{
		{"successful_build_and_clean", "0", "0", "", "0", false},
		{"clean_failure_after_successful_build", "0", "23", "", "23", true},
		{"build_failure_before_failed_clean", "17", "23", "", "17", true},
		{"interrupt_before_failed_clean", "0", "23", "INT", "130", true},
		{"terminate_before_failed_clean", "0", "23", "TERM", "143", true},
	}
	for _, script := range scripts {
		t.Run(strings.ReplaceAll(script, "/", "_"), func(t *testing.T) {
			for _, tc := range cases {
				t.Run(tc.name, func(t *testing.T) {
					root := scriptFixture(t, script)
					writeFixture(t, root, "examples/prepareTriceBind.sh", "prepare_trice_bind_build() { :; }\n")
					writeFixture(t, root, "scripts/_150_setup_build_environment.sh", "MAKE_JOBS=-j1\n")
					writeFixture(t, root, "scripts/_230_legacy_insert_ids.sh", "#!/bin/sh\nexit 0\n")
					writeFixture(t, root, "scripts/_240_legacy_clean_ids.sh", "#!/bin/sh\ncount_file=\"$FIXTURE_ROOT/clean-count\"\ncount=0\nif [ -f \"$count_file\" ]; then read -r count < \"$count_file\"; fi\ncount=$((count + 1))\nprintf '%s\\n' \"$count\" > \"$count_file\"\nif [ \"$count\" -eq 2 ]; then exit \"$CLEAN_STATUS\"; fi\nexit 0\n")
					writeFixture(t, root, "bin/make", "#!/bin/sh\nif [ \"${1:-}\" = clean ]; then exit 0; fi\nif [ -n \"$BUILD_SIGNAL\" ]; then kill -s \"$BUILD_SIGNAL\" \"$PPID\"; exit 0; fi\nexit \"$BUILD_STATUS\"\n")
					env := map[string]string{
						"FIXTURE_ROOT": root, "BUILD_STATUS": tc.buildStatus,
						"CLEAN_STATUS": tc.cleanStatus, "BUILD_SIGNAL": tc.signal,
						"TRICE_ID_WORKFLOW_OWNER": "0",
					}
					command := "bash " + script
					out, err := runFixture(t, root, command, env)
					if tc.wantStatus == "0" {
						assert.NoError(t, err, out)
					} else if assert.Error(t, err, out) {
						assert.Equal(t, "exit status "+tc.wantStatus, err.Error(), out)
					}
					if tc.wantWarning {
						assert.Contains(t, out, "trice clean failed with exit code 23")
					} else {
						assert.NotContains(t, out, "trice clean failed")
					}
					count, readErr := os.ReadFile(filepath.Join(root, "clean-count"))
					assert.NoError(t, readErr)
					assert.Equal(t, "2\n", string(count), "pre-clean and final clean must each run once")
				})
			}
		})
	}
}

// TestManualGenerateExamplesUseCurrentCLI validates the copyable CE and ABC
// examples against a freshly built host tool in an isolated project directory.
// It also verifies that the obsolete C# generator flag is unavailable.
func TestManualGenerateExamplesUseCurrentCLI(t *testing.T) {
	root := t.TempDir()
	suffix := ""
	if runtime.GOOS == "windows" {
		suffix = ".exe"
	}
	tool := filepath.Join(root, "trice"+suffix)
	build := exec.Command("go", "build", "-o", tool, "./cmd/trice")
	build.Dir = ".."
	output, err := build.CombinedOutput()
	if !assert.NoError(t, err, string(output)) {
		t.FailNow()
	}
	writeFixture(t, root, "sample.c", "void f(void) {\n  trice32(\"info:Moving sample={sample}\\n\", 3);\n  trice16C(\"cmd:set_pwm\", payload, 1);\n}\n")
	writeFixture(t, root, "til.json", "{}\n")
	writeFixture(t, root, "li.json", "{}\n")
	runCLI := func(directory string, args ...string) (string, error) {
		command := exec.Command(tool, args...)
		command.Dir = directory
		result, runErr := command.CombinedOutput()
		return string(result), runErr
	}

	ceRule := `info:", x={}, y={}, m/s=%f", pos.x, pos.y, aFloat(velocity)`
	outputText, bindErr := runCLI(root, "bind", "-src", "sample.c", "-i", "til.json", "-li", "li.json", "-ce", ceRule)
	if !assert.NoError(t, bindErr, outputText) {
		t.FailNow()
	}
	tilBytes, readErr := os.ReadFile(filepath.Join(root, "til.json"))
	if !assert.NoError(t, readErr) {
		t.FailNow()
	}
	var til map[string]struct{ Strg string }
	if !assert.NoError(t, json.Unmarshal(tilBytes, &til)) {
		t.FailNow()
	}
	var foundCE, foundABC bool
	for _, entry := range til {
		foundCE = foundCE || entry.Strg == `info:Moving sample={sample}, x={pos.x}, y={pos.y}, m/s=%f\n`
		foundABC = foundABC || entry.Strg == "cmd:set_pwm"
	}
	assert.True(t, foundCE, "the documented CE rule must produce the named position fields and speed")
	assert.True(t, foundABC, "the command record must be available to ABC generation")

	outputText, generateErr := runCLI(root, "generate", "-i", "til.json", "-li", "off", "-abc=deviceX")
	assert.NoError(t, generateErr, outputText)
	for _, path := range []string{"generated/deviceX.h", "generated/deviceX.c"} {
		_, statErr := os.Stat(filepath.Join(root, path))
		assert.NoError(t, statErr, path)
	}
	outputText, generateErr = runCLI(root, "generate", "-i", "til.json", "-li", "off", "-abc=custom/deviceY")
	assert.NoError(t, generateErr, outputText)
	for _, path := range []string{"custom/deviceY.h", "custom/deviceY.c"} {
		_, statErr := os.Stat(filepath.Join(root, path))
		assert.NoError(t, statErr, path)
	}
	outputText, generateErr = runCLI(root, "generate", "-tilCS")
	assert.Error(t, generateErr)
	assert.Contains(t, outputText, "flag provided but not defined: -tilCS")
}

// TestClangFormatCompatibility ensures only the canonical version can report a
// formatting regression. An override, missing executable or newer/older version
// never rewrites source files, and real canonical formatting errors still fail.
func TestClangFormatCompatibility(t *testing.T) {
	for _, tc := range []struct {
		version, replacements string
		failure               bool
	}{
		{"19.1.3", "1", false}, {"19.1.70", "1", false}, {"20.1.0", "1", false}, {"unknown", "1", false},
		{"19.1.7", "0", false}, {"19.1.7", "1", true}, {"missing", "0", false},
	} {
		t.Run(tc.version+"_"+tc.replacements, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_430_test_clang_format.sh", "scripts/_280_format_c_code.sh")
			writeFixture(t, root, "bin/git", "#!/bin/sh\nprintf 'sample.c\\n'\n")
			writeFixture(t, root, "bin/go", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "bin/formatter", "#!/bin/sh\nif [ \"$1\" = --version ]; then echo \"clang-format version $VERSION\"; elif [ \"$REPLACEMENTS\" = 1 ]; then echo '<replacement offset=\"0\" length=\"1\"> </replacement>'; fi\n")
			writeFixture(t, root, "sample.c", "int main(void){return 0;}\n")
			formatter := "./bin/formatter"
			if tc.version == "missing" {
				formatter = "./bin/not-installed"
			}
			out, err := runFixture(t, root, "bash scripts/_430_test_clang_format.sh", map[string]string{"CLANG_FORMAT_BIN": formatter, "VERSION": tc.version, "REPLACEMENTS": tc.replacements, "CLANG_FILTER_CMD": "cat"})
			assert.Equal(t, tc.failure, err != nil, out)
			if tc.version != "19.1.7" {
				assert.Contains(t, out, "SKIP:")
			}
			data, err := os.ReadFile(filepath.Join(root, "sample.c"))
			assert.NoError(t, err)
			assert.Equal(t, "int main(void){return 0;}\n", string(data))
		})
	}
}

// clangBootstrapFixture simulates both venv layouts and tool installation. The
// doubles record setup calls, so cache reuse and offline checks cannot silently
// pass by using Python or clang-format installed on the developer's machine.
func clangBootstrapFixture(t *testing.T, platform string) (string, map[string]string) {
	t.Helper()
	root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_430_test_clang_format.sh", "scripts/_280_format_c_code.sh")
	writeFixture(t, root, "bin/git", "#!/bin/sh\necho sample.c\n")
	writeFixture(t, root, "bin/go", "#!/bin/sh\nexit 0\n")
	writeFixture(t, root, "bin/uname", "#!/bin/sh\nif [ \"$1\" = -s ]; then echo \"$PLATFORM\"; else echo x86_64; fi\n")
	writeFixture(t, root, "bin/clang-format", "#!/bin/sh\nif [ \"$1\" = --version ]; then echo 'clang-format version 19.1.3'; else echo wrong-system-formatter; exit 29; fi\n")
	writeFixture(t, root, "sample.c", "int value=1;\n")
	writeFixture(t, root, "seed/formatter", `#!/bin/sh
if [ "$1" = --version ]; then
  echo "clang-format version ${INSTALLED_VERSION:-19.1.7}"
else
  case "$*" in
    *-dump-config*) exit 0 ;;
    *-output-replacements-xml*) echo '<replacements/>' ;;
    *) echo 'int value = 1;' ;;
  esac
fi
`)
	writeFixture(t, root, "seed/venv-python", `#!/bin/sh
printf 'pip %s\n' "$*" >> "$SETUP_LOG"
[ "${FAIL_STAGE:-}" != pip ] || exit 23
cp "$SEED_FORMATTER" "$(dirname "$0")/$FORMATTER_NAME"
`)
	// A launcher and both usual Python names prevent a missing-Python test from
	// falling through to a real interpreter and contacting the package index.
	for _, name := range []string{"py", "python3", "python"} {
		writeFixture(t, root, "bin/"+name, `#!/bin/sh
[ "${FAIL_STAGE:-}" != python ] || exit 1
if [ "$1" = -3 ]; then shift; fi
if [ "$1" = -c ]; then exit 0; fi
printf 'venv %s\n' "$*" >> "$SETUP_LOG"
[ "${FAIL_STAGE:-}" != venv ] || exit 23
mkdir -p "$3/$VENV_BIN"
cp "$SEED_PYTHON" "$3/$VENV_BIN/$VENV_PYTHON"
`)
	}
	env := map[string]string{
		"PLATFORM": platform, "CLANG_FORMAT_BIN": "", "CLANG_FILTER_CMD": "cat",
		"VENV_BIN": "bin", "VENV_PYTHON": "python", "FORMATTER_NAME": "clang-format",
		"SETUP_LOG": filepath.Join(root, "setup.log"), "SEED_PYTHON": filepath.Join(root, "seed/venv-python"),
		"SEED_FORMATTER": filepath.Join(root, "seed/formatter"),
	}
	if strings.HasPrefix(platform, "MINGW") {
		env["VENV_BIN"], env["VENV_PYTHON"], env["FORMATTER_NAME"] = "Scripts", "python.exe", "clang-format.exe"
	}
	return root, env
}

// TestClangFormatAutomaticSetup checks first-run installation, native Windows
// layout, offline cache reuse and honoring an explicit user tool selection.
func TestClangFormatAutomaticSetup(t *testing.T) {
	for _, platform := range []string{"Darwin", "Linux", "MINGW64_NT"} {
		t.Run(platform, func(t *testing.T) {
			root, env := clangBootstrapFixture(t, platform)
			out, err := runFixture(t, root, "bash scripts/_280_format_c_code.sh format", env)
			assert.NoError(t, err, out)
			assert.Contains(t, out, "Using clang-format version 19.1.7")
			data, err := os.ReadFile(filepath.Join(root, "sample.c"))
			assert.NoError(t, err)
			assert.Equal(t, "int value = 1;\n", string(data))
			setup, err := os.ReadFile(filepath.Join(root, "setup.log"))
			assert.NoError(t, err)
			assert.Equal(t, 1, strings.Count(string(setup), "pip -m pip install "))
			assert.Contains(t, string(setup), "clang-format==19.1.7")
			assert.Contains(t, string(setup), "--only-binary=:all:")
			assert.Contains(t, string(setup), "--no-input")

			// A cached formatter must work even when bootstrap Python is absent.
			env["FAIL_STAGE"] = "python"
			for _, command := range []string{"bash scripts/_280_format_c_code.sh format", "bash scripts/_430_test_clang_format.sh"} {
				out, err = runFixture(t, root, command, env)
				assert.NoError(t, err, out)
				assert.NotContains(t, out, "SKIP:")
			}
			after, err := os.ReadFile(filepath.Join(root, "setup.log"))
			assert.NoError(t, err)
			assert.Equal(t, string(setup), string(after))

			env["CLANG_FORMAT_BIN"] = "./bin/clang-format"
			out, err = runFixture(t, root, "bash scripts/_280_format_c_code.sh format", env)
			assert.Error(t, err, out)
			assert.Contains(t, out, "Explicit CLANG_FORMAT_BIN")
		})
	}
}

// TestClangFormatSetupFailures preserves source bytes when prerequisites,
// downloads or post-install validation fail. Checks never initiate setup.
func TestClangFormatSetupFailures(t *testing.T) {
	for _, stage := range []string{"python", "venv", "pip", "version", "check"} {
		t.Run(stage, func(t *testing.T) {
			root, env := clangBootstrapFixture(t, "Linux")
			env["FAIL_STAGE"] = stage
			mode := "format"
			if stage == "version" {
				env["INSTALLED_VERSION"] = "18.0.0"
			} else if stage == "check" {
				mode = "check"
			}
			out, err := runFixture(t, root, "bash scripts/_280_format_c_code.sh "+mode, env)
			assert.Equal(t, stage != "check", err != nil, out)
			data, err := os.ReadFile(filepath.Join(root, "sample.c"))
			assert.NoError(t, err)
			assert.Equal(t, "int value=1;\n", string(data))
			if stage == "check" {
				assert.Contains(t, out, "SKIP:")
				_, err = os.Stat(filepath.Join(root, "setup.log"))
				assert.True(t, os.IsNotExist(err))
			}
		})
	}
}

// TestShfmtCompatibility ensures test runs use an available canonical formatter
// without installing anything, including the native Windows .exe suffix.
func TestShfmtCompatibility(t *testing.T) {
	for _, tc := range []struct {
		version, suffix, needs string
		failure                bool
	}{
		{"v3.8.0", "", "0", false}, {"v3.8.0", ".exe", "0", false},
		{"v3.8.0", "", "1", true}, {"v3.12.0", "", "1", false},
	} {
		t.Run(tc.version+tc.suffix+"_"+tc.needs, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_400_test_shell_format.sh", "scripts/_260_format_shell_scripts.sh")
			writeFixture(t, root, "bin/git", "#!/bin/sh\necho sample.sh\n")
			writeFixture(t, root, "bin/go", "#!/bin/sh\necho unexpected-install; exit 19\n")
			writeFixture(t, root, "bin/shfmt"+tc.suffix, "#!/bin/sh\nif [ \"$1\" = --version ]; then echo \"$VERSION\"; elif [ \"$NEEDS\" = 1 ]; then echo sample.sh; fi\n")
			writeFixture(t, root, "sample.sh", "#!/bin/sh\nexit 0\n")
			out, err := runFixture(t, root, "bash scripts/_400_test_shell_format.sh", map[string]string{"SHFMT_BIN": "./bin/shfmt", "VERSION": tc.version, "NEEDS": tc.needs})
			assert.Equal(t, tc.failure, err != nil, out)
			assert.NotContains(t, out, "unexpected-install")
			if tc.version != "v3.8.0" {
				assert.Contains(t, out, "SKIP:")
			}
			data, err := os.ReadFile(filepath.Join(root, "temp/log/_400_test_shell_format.log"))
			assert.NoError(t, err)
			assert.Contains(t, string(data), "Starting _400_test_shell_format")
		})
	}
}

// TestShellCheckDialectSelection skips unsupported grammars rather than forcing
// Bash parsing, while retaining failures in scripts ShellCheck can actually lint.
func TestShellCheckDialectSelection(t *testing.T) {
	for _, status := range []string{"0", "1"} {
		t.Run(status, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_410_test_shellcheck.sh")
			writeFixture(t, root, "bin/git", "#!/bin/sh\nprintf 'sample.sh\\nzshell.sh\\n'\n")
			writeFixture(t, root, "bin/shellcheck", "#!/bin/sh\nprintf 'checked: %s\\n' \"$@\"; exit \"$STATUS\"\n")
			writeFixture(t, root, "sample.sh", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "zshell.sh", "#!/bin/zsh\nexit 0\n")
			out, err := runFixture(t, root, "bash scripts/_410_test_shellcheck.sh", map[string]string{"STATUS": status})
			assert.Equal(t, status != "0", err != nil, out)
			assert.Contains(t, out, "SKIP: ShellCheck does not support this interpreter: zshell.sh")
			assert.Contains(t, out, "checked: sample.sh")
			assert.NotContains(t, out, "checked: zshell.sh")
		})
	}
}

// TestManagedPrerequisites verifies that a missing prerequisite skips before
// any source snapshot/mutation, whereas a real preparation failure stays fatal.
func TestManagedPrerequisites(t *testing.T) {
	for _, tc := range []struct {
		name, missing, cgo, compiler, envStatus string
		skipped                                 bool
	}{
		{"missing_go", "go", "1", "cc", "0", true},
		{"missing_trice", "trice", "1", "cc", "0", true},
		{"missing_tar", "tar", "1", "cc", "0", true},
		{"cgo_disabled", "", "0", "cc", "0", true},
		{"missing_compiler", "missing-cc", "1", "missing-cc", "0", true},
		{"go_env_error", "", "1", "cc", "23", false},
		{"snapshot_failure", "", "1", "cc", "0", false},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_140_trice_test_state.sh")
			writeFixture(t, root, "scripts/_130_trice_id_workflow.sh", "# No workflow setup is needed before prerequisite checks.\n")
			// Override command discovery rather than deleting any installed tools.
			command := `source scripts/_140_trice_test_state.sh
command() { if [ "$1" = -v ]; then [ "$2" != "$MISSING" ]; else builtin command "$@"; fi; }
go() { if [ "$ENV_STATUS" != 0 ]; then return "$ENV_STATUS"; fi; case "$2" in CGO_ENABLED) echo "$CGO";; CC) echo "$COMPILER";; esac; }
trice_test_state_snapshot() { echo unexpected-snapshot; return 1; }
trice_test_finish() { exit "$1"; }
trice_test_run_managed_workflow bind pc unused-worker
`
			out, err := runFixture(t, root, command, map[string]string{"MISSING": tc.missing, "CGO": tc.cgo, "COMPILER": tc.compiler, "ENV_STATUS": tc.envStatus})
			assert.Equal(t, !tc.skipped, err != nil, out)
			if tc.skipped {
				assert.Contains(t, out, "SKIP:")
				assert.NotContains(t, out, "unexpected-snapshot")
			} else {
				assert.NotContains(t, out, "SKIP:")
			}
		})
	}
}

// TestGoReleaserCompatibility exercises numeric minor versions and keeps schema
// failures distinct from unsupported tool versions, including snapshot preflight.
func TestGoReleaserCompatibility(t *testing.T) {
	for _, version := range []string{"v2.9.0", "2.10.0", "v2.100.0", "3.0.0", "unknown"} {
		t.Run(version, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_460_test_goreleaser_config.sh", "scripts/_610_test_goreleaser_snapshot.sh")
			writeFixture(t, root, "bin/goreleaser", "#!/bin/sh\nif [ \"$1\" = --version ]; then echo \"GitVersion: $VERSION\"; else echo schema-check; exit 17; fi\n")
			out, err := runFixture(t, root, "bash scripts/_460_test_goreleaser_config.sh", map[string]string{"VERSION": version})
			unsupported := version == "v2.9.0" || version == "unknown"
			assert.Equal(t, !unsupported, err != nil, out)
			if unsupported {
				assert.Contains(t, out, "SKIP:")
				assert.NotContains(t, out, "schema-check")
				out, err = runFixture(t, root, "bash scripts/_610_test_goreleaser_snapshot.sh", map[string]string{"VERSION": version})
				assert.NoError(t, err, out)
				assert.Contains(t, out, "SKIP:")
			} else {
				assert.Contains(t, out, "schema-check")
				assert.Contains(t, out, "FAIL:")
			}
		})
	}
}

// TestStandaloneExamplesKeepWorkflowOwnership guards against invoking Bind-only
// entry points from an Insert transaction; both still execute their GCC matrix.
func TestStandaloneExamplesKeepWorkflowOwnership(t *testing.T) {
	for _, workflow := range []string{"insert", "bind"} {
		t.Run(workflow, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_200_gcc_example_build_worker.sh")
			writeFixture(t, root, "scripts/_150_setup_build_environment.sh", "# Fixture has no toolchain environment overrides.\n")
			writeFixture(t, root, "examples/cleanAllTargets.sh", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "examples/buildAllTargets_TRICE_ON.sh", "#!/bin/sh\necho matrix-$TRICE_ID_WORKFLOW\n")
			for _, tool := range []string{"arm-none-eabi-gcc", "cmake", "cc"} {
				writeFixture(t, root, "bin/"+tool, "#!/bin/sh\nexit 0\n")
			}
			for _, entry := range []string{"DemoData_Trice/build.sh", "PC_log/build_and_run.sh", "G0B1_log/build.sh", "F030_inst/build.sh", "G0B1_inst/build.sh", "L432_inst/build.sh"} {
				writeFixture(t, root, "examples/"+entry, "#!/bin/sh\nif [ -z \"${TRICE_ID_WORKFLOW:-}\" ]; then echo 'INFO: standalone'; else echo managed-$TRICE_ID_WORKFLOW; fi\nexit \"${BUILD_STATUS:-0}\"\n")
			}
			writeFixture(t, root, "build.log", "warning: tool-version diagnostic\n")
			out, err := runFixture(t, root, "bash scripts/_200_gcc_example_build_worker.sh on full", map[string]string{"TRICE_ID_WORKFLOW": workflow, "LOGFILE": filepath.Join(root, "build.log")})
			assert.NoError(t, err, out)
			assert.Contains(t, out, "matrix-"+workflow)
			assert.Contains(t, out, "Hint:")
			if workflow == "bind" {
				assert.Equal(t, 6, strings.Count(out, "INFO: standalone"), out)
			} else {
				assert.NotContains(t, out, "INFO: standalone")
			}
			// The same worker must propagate a real failed build even if its log
			// also contains warnings that are intentionally no longer fatal.
			out, err = runFixture(t, root, "bash scripts/_200_gcc_example_build_worker.sh on quick", map[string]string{"TRICE_ID_WORKFLOW": workflow, "LOGFILE": filepath.Join(root, "build.log"), "BUILD_STATUS": "23"})
			assert.Error(t, err, out)
		})
	}
}

// TestPCLogSourcesAreLocaleIndependent records the real build command on each
// supported OS path without requiring SEGGER software or a native compiler.
func TestPCLogSourcesAreLocaleIndependent(t *testing.T) {
	for _, platform := range []string{"Darwin", "Linux", "MINGW64_NT"} {
		for _, locale := range []string{"C", "de_DE.UTF-8"} {
			t.Run(platform+"_"+locale, func(t *testing.T) {
				root := scriptFixture(t, "examples/PC_log/build_and_run.sh")
				for _, source := range []string{"trice.c", "triceLog.c", "SEGGER_RTT.c"} {
					writeFixture(t, root, "src/"+source, "// Fixture source.\n")
				}
				writeFixture(t, root, "bin/cc", "#!/bin/sh\nprintf '%s\\n' \"$@\" > compiler-args.txt\n")
				writeFixture(t, root, "bin/trice", "#!/bin/sh\nexit 0\n")
				writeFixture(t, root, "bin/uname", "#!/bin/sh\necho \"$PLATFORM\"\n")
				for _, executable := range []string{"pc_log", "pc_log.exe"} {
					writeFixture(t, root, "examples/PC_log/build/"+executable, "#!/bin/sh\nexit 0\n")
				}
				out, err := runFixture(t, root, "sh examples/PC_log/build_and_run.sh", map[string]string{"PLATFORM": platform, "LC_ALL": locale})
				assert.NoError(t, err, out)
				assert.Contains(t, out, "SEGGER_RTT_Conf.h and J-Link tools are not required")
				data, err := os.ReadFile(filepath.Join(root, "examples/PC_log/compiler-args.txt"))
				assert.NoError(t, err)
				assert.NotContains(t, string(data), "SEGGER_RTT.c")
				assert.Contains(t, string(data), "../../src/trice.c")
				assert.Contains(t, string(data), "../../src/triceLog.c")
			})
		}
	}
}
