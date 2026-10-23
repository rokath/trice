// SPDX-License-Identifier: MIT

package scripts_test

import (
	"encoding/json"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"runtime"
	"strconv"
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

// TestIDWorkflowsInitializeLocalLI covers fresh checkouts, retained ownership
// evidence, disabled LI, and initialization errors before ID processing starts.
func TestIDWorkflowsInitializeLocalLI(t *testing.T) {
	t.Parallel()
	for _, action := range []string{"bind", "insert", "clean"} {
		for _, tc := range []struct {
			name, path, initial string
			existing, blocked   bool
		}{
			{name: "missing_default", path: "demoLI.json"},
			{name: "missing_nested", path: "metadata/local.json"},
			{name: "retained_empty", path: "demoLI.json", existing: true},
			{name: "retained_ownership", path: "demoLI.json", initial: "{\"42\":{\"File\":\"sample.c\",\"Line\":3}}\n", existing: true},
			{name: "disabled_off", path: "off"},
			{name: "disabled_none", path: "none"},
			{name: "parent_is_file", path: "metadata/local.json", blocked: true},
		} {
			t.Run(action+"/"+tc.name, func(t *testing.T) {
				// Each case owns its scripts, metadata, and tool double in a temp tree.
				root := scriptFixture(t, "scripts/_120_setup_trice_environment.sh", "scripts/_130_trice_id_workflow.sh")
				if tc.existing {
					writeFixture(t, root, tc.path, tc.initial)
				}
				if tc.blocked {
					writeFixture(t, root, "metadata", "not a directory\n")
				}
				writeFixture(t, root, "bin/trice", "#!/bin/sh\nset -eu\ncase \"$TRICE_LI_JSON\" in off|none) ;; *) [ -f \"$TRICE_LI_JSON\" ] || exit 91 ;; esac\nprintf '%s\\n' \"$1\" > called\n")
				// Merely loading the environment must not initialize the local table.
				_, err := runFixture(t, root, "source scripts/_120_setup_trice_environment.sh", map[string]string{"TRICE_LI_JSON": tc.path})
				assert.NoError(t, err)
				if !tc.existing && !tc.blocked {
					assert.NoFileExists(t, filepath.Join(root, tc.path))
				}
				// The workflow must initialize LI before invoking the tool, or stop.
				output, err := runFixture(t, root, "source scripts/_130_trice_id_workflow.sh\ntrice_id_workflow "+action, map[string]string{"TRICE_LI_JSON": tc.path})
				if tc.blocked {
					assert.Error(t, err, output)
					assert.NoFileExists(t, filepath.Join(root, "called"))
					return
				}
				assert.NoError(t, err, output)
				called, err := os.ReadFile(filepath.Join(root, "called"))
				assert.NoError(t, err)
				assert.Equal(t, action+"\n", string(called))
				if tc.path == "off" || tc.path == "none" {
					assert.NoFileExists(t, filepath.Join(root, tc.path))
					return
				}
				contents, err := os.ReadFile(filepath.Join(root, tc.path))
				assert.NoError(t, err)
				assert.Equal(t, tc.initial, string(contents), "initialization must preserve existing ID evidence")
			})
		}
	}
}

// scriptFixture copies only the production scripts under test; their normal
// relative-root discovery then resolves into the isolated fixture repository.
func scriptFixture(t *testing.T, scripts ...string) string {
	t.Helper()
	root := t.TempDir()
	// Tool setup is shared by standalone checks as well as suite entry points.
	scripts = append(scripts, "scripts/_105_setup_test_tools.sh")
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
		// A surrounding testAll full run must not silently enable recovery or
		// compiler matrices in fixtures. Full-mode cases opt in via overrides.
		"SELECTED": "quick",
		// A private runtime from the outer suite must not bypass tool doubles.
		"TRICE_TEST_PYTHON": "",
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

// TestPinnedToolConfirmation checks explicit consent, refusal and CI behavior
// without downloading packages or changing the developer's toolchain.
func TestPinnedToolConfirmation(t *testing.T) {
	for _, tc := range []struct {
		name, input, ci string
		accepted        bool
	}{
		{"yes", "yes", "false", true},
		{"windows_yes", "yes\\r", "false", true},
		{"no", "no", "false", false},
		{"empty", "", "false", false},
		{"ci", "yes", "true", false},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t)
			command := "printf '" + tc.input + "\\n' | { source scripts/_105_setup_test_tools.sh; confirm_tool_install 'shfmt v3.8.0'; }"
			out, err := runFixture(t, root, command, map[string]string{"CI": tc.ci})
			assert.Equal(t, tc.accepted, err == nil, out)
			if tc.ci == "true" {
				assert.Contains(t, out, "CI cannot prompt")
			} else {
				assert.Contains(t, out, "Install shfmt v3.8.0 locally under temp/tools?")
			}
			assert.NoDirExists(t, filepath.Join(root, "temp/tools"))
		})
	}
}

// TestCppcheckPinnedVersion verifies that the exact analyzer version is accepted
// and an older version requires consent instead of silently reducing checks.
func TestCppcheckPinnedVersion(t *testing.T) {
	t.Run("version_probe_preserves_installation_answer", func(t *testing.T) {
		root := scriptFixture(t)
		// Native tools must not consume the input reserved for the consent prompt.
		writeFixture(t, root, "bin/cppcheck", "#!/bin/sh\nread -r ignored || true\necho 'Cppcheck 2.21.0'\n")
		command := `printf 'yes\n' | {
source scripts/_105_setup_test_tools.sh
install_cppcheck_test_tool() { echo installation-reached; return 23; }
ensure_cppcheck_test_tool
}`
		out, err := runFixture(t, root, command, map[string]string{"CI": "false"})
		assert.Error(t, err)
		assert.Contains(t, out, "installation-reached")
	})
	for _, ending := range []string{"\\n", "\\r\\n"} {
		t.Run("pinned_version_"+ending, func(t *testing.T) {
			root := scriptFixture(t)
			writeFixture(t, root, "bin/cppcheck", "#!/bin/sh\nprintf 'Cppcheck 2.22.0"+ending+"'\n")
			output, err := runFixture(t, root, "source scripts/_105_setup_test_tools.sh; ensure_cppcheck_test_tool", nil)
			assert.NoError(t, err, output)
			assert.NotContains(t, output, "Install Cppcheck")
		})
	}
	t.Run("older_version_requires_confirmation", func(t *testing.T) {
		root := scriptFixture(t)
		writeFixture(t, root, "bin/cppcheck", "#!/bin/sh\nprintf 'Cppcheck 2.10\\n'\n")
		output, err := runFixture(t, root, "printf 'no\\n'; source scripts/_105_setup_test_tools.sh; ensure_cppcheck_test_tool", map[string]string{"CI": "false"})
		assert.Error(t, err, output)
		assert.Contains(t, output, "Install Cppcheck 2.22.0 locally under temp/tools?")
		assert.Contains(t, output, "Cppcheck 2.22.0 is required")
	})
}

// nativeDownloadFixture records downloads and substitutes archive verification.
// Failure cases cannot reach the network or extract unverified fixture bytes.
func nativeDownloadFixture(t *testing.T, root string) {
	t.Helper()
	writeFixture(t, root, "bin/curl", `#!/bin/sh
printf '%s\n' "$*" >> download.calls
[ "${NATIVE_FAIL:-}" != download ] || exit 23
while [ "$#" -gt 0 ]; do
  if [ "$1" = -o ]; then printf 'fixture archive\n' > "$2"; exit 0; fi
  shift
done
exit 91
`)
	writeFixture(t, root, "bin/sha256sum", `#!/bin/sh
if [ "${NATIVE_FAIL:-}" = checksum ]; then echo 'invalid archive hash'; else printf '%s  %s\n' "$PINNED_DIGEST" "$1"; fi
`)
}

// TestCppcheckLocalCache checks both installation layouts with an older system
// version first in PATH. Cached tools must also be available to child profiles.
func TestCppcheckLocalCache(t *testing.T) {
	for _, layout := range []string{"bin/cppcheck", "PFiles/Cppcheck/cppcheck.exe"} {
		t.Run(layout, func(t *testing.T) {
			root := scriptFixture(t)
			writeFixture(t, root, "bin/cppcheck", "#!/bin/sh\necho 'Cppcheck 2.21.0'\n")
			writeFixture(t, root, "temp/tools/cppcheck/2.22.0/"+layout, "#!/bin/sh\nprintf 'Cppcheck 2.22.0\\r\\n'\n")
			// Unix shells do not resolve `cppcheck` to `cppcheck.exe`. Emulate
			// Windows executable lookup beside the Windows-layout tool double.
			if runtime.GOOS != "windows" && strings.HasSuffix(layout, ".exe") {
				writeFixture(t, root, "temp/tools/cppcheck/2.22.0/"+strings.TrimSuffix(layout, ".exe"), "#!/bin/sh\nprintf 'Cppcheck 2.22.0\\r\\n'\n")
			}
			out, err := runFixture(t, root, "source scripts/_105_setup_test_tools.sh; ensure_cppcheck_test_tool && bash -c 'cppcheck --version'", map[string]string{"CI": "true"})
			assert.NoError(t, err, out)
			assert.Contains(t, out, "Cppcheck 2.22.0")
			assert.NotContains(t, out, "Install Cppcheck")
		})
	}
}

// TestCppcheckWindowsSetup verifies local MSI extraction, exact-version and hash
// rejection, quiet setup logs and reuse without asking or downloading again.
func TestCppcheckWindowsSetup(t *testing.T) {
	for _, failure := range []string{"", "download", "checksum", "extract", "runtime_missing", "version"} {
		t.Run("failure_"+failure, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_520_test_target_code_lint.sh")
			nativeDownloadFixture(t, root)
			writeFixture(t, root, "bin/uname", "#!/bin/sh\nif [ \"$1\" = -s ]; then echo MINGW64_NT-10.0; else echo x86_64; fi\n")
			writeFixture(t, root, "bin/cppcheck", "#!/bin/sh\necho 'Cppcheck 2.21.0'\n")
			writeFixture(t, root, "bin/cygpath", "#!/bin/sh\nprintf '%s\\n' \"$2\"\n")
			writeFixture(t, root, "bin/cmake", "#!/bin/sh\necho unexpected-cmake; exit 91\n")
			// Native Windows supplies this extension lookup itself; only Unix
			// hosts need a second name for the extracted shell tool double.
			unixAlias := ""
			if runtime.GOOS != "windows" {
				unixAlias = "cp seed/cppcheck temp/tools/cppcheck/2.22.0/PFiles/Cppcheck/cppcheck\n"
			}
			writeFixture(t, root, "bin/msiexec.exe", `#!/bin/sh
printf '%s\n' "$*" >> extract.calls
[ "$MSYS_NO_PATHCONV" = 1 ] || exit 91
echo native-msi-extraction
[ "${NATIVE_FAIL:-}" != extract ] || exit 23
mkdir -p temp/tools/cppcheck/2.22.0/PFiles/Cppcheck
cp seed/cppcheck temp/tools/cppcheck/2.22.0/PFiles/Cppcheck/cppcheck.exe
if [ "${NATIVE_FAIL:-}" != runtime_missing ]; then
  mkdir -p temp/tools/cppcheck/2.22.0/PFiles/System64
  for dll in vcruntime140.dll msvcp140.dll; do printf 'bundled runtime\n' > "temp/tools/cppcheck/2.22.0/PFiles/System64/$dll"; done
fi
`+unixAlias)
			writeFixture(t, root, "seed/cppcheck", `#!/bin/sh
[ -f "${0%/*}/vcruntime140.dll" ] && [ -f "${0%/*}/msvcp140.dll" ] || exit 91
if [ "$1" = --help ]; then echo --check-level; else printf 'Cppcheck %s\r\n' "$INSTALLED_VERSION"; fi
`)
			writeFixture(t, root, "scripts/_290_lint_c_code.sh", "#!/bin/sh\ncppcheck --version | tr -d '\\r' | grep -qx 'Cppcheck 2.22.0' || exit 91\necho profile >> profiles.calls\n")
			env := map[string]string{
				"CI": "false", "NATIVE_FAIL": failure, "INSTALLED_VERSION": "2.22.0",
				"PINNED_DIGEST": "3929162bab7e71d1b9b18708ec054fb0188bfc8ea1660979ed95837f5a9f547b",
			}
			if failure == "version" {
				env["INSTALLED_VERSION"] = "2.21.0"
			}
			out, err := runFixture(t, root, "printf 'yes\\r\\n' | bash scripts/_520_test_target_code_lint.sh --quiet", env)
			assert.Equal(t, failure != "", err != nil, out)
			logBytes, readErr := os.ReadFile(filepath.Join(root, "temp/log/_520_test_target_code_lint.log"))
			assert.NoError(t, readErr)
			logText := string(logBytes)
			assert.NotContains(t, logText, "unexpected-cmake")
			if failure == "download" || failure == "checksum" {
				assert.NoFileExists(t, filepath.Join(root, "extract.calls"))
			} else {
				calls, readErr := os.ReadFile(filepath.Join(root, "extract.calls"))
				assert.NoError(t, readErr)
				assert.Contains(t, string(calls), "/a ")
				assert.Contains(t, string(calls), " /qn TARGETDIR=")
				assert.Contains(t, string(calls), "/L*v ")
				assert.Contains(t, logText, "native-msi-extraction")
			}
			if failure != "" {
				assert.Contains(t, logText, "FAIL:")
				assert.NoFileExists(t, filepath.Join(root, "profiles.calls"))
				if failure == "checksum" {
					assert.Contains(t, logText, "SHA-256 mismatch")
				}
				return
			}
			profiles, readErr := os.ReadFile(filepath.Join(root, "profiles.calls"))
			assert.NoError(t, readErr)
			assert.Equal(t, 10, strings.Count(string(profiles), "profile\n"))
			assert.FileExists(t, filepath.Join(root, "temp/tools/cppcheck/2.22.0/PFiles/Cppcheck/vcruntime140.dll"))
			assert.FileExists(t, filepath.Join(root, "temp/tools/cppcheck/2.22.0/PFiles/Cppcheck/msvcp140.dll"))
			downloads, readErr := os.ReadFile(filepath.Join(root, "download.calls"))
			assert.NoError(t, readErr)
			env["CI"], env["NATIVE_FAIL"] = "true", "download"
			out, err = runFixture(t, root, "bash scripts/_520_test_target_code_lint.sh --quiet", env)
			assert.NoError(t, err, out)
			after, readErr := os.ReadFile(filepath.Join(root, "download.calls"))
			assert.NoError(t, readErr)
			assert.Equal(t, string(downloads), string(after))
		})
	}
}

// TestPinnedFormatterCheckInstallation proves that an approved check installs
// the exact version into a disposable cache and then checks unchanged sources.
func TestPinnedFormatterCheckInstallation(t *testing.T) {
	t.Run("clang-format", func(t *testing.T) {
		root, env := clangBootstrapFixture(t, "Linux")
		env["CI"] = "false"
		out, err := runFixture(t, root, "printf 'yes\\n' | bash scripts/_280_format_c_code.sh check", env)
		assert.NoError(t, err, out)
		assert.Contains(t, out, "Using clang-format version 23.1.2")
		setup, readErr := os.ReadFile(filepath.Join(root, "setup.log"))
		assert.NoError(t, readErr)
		assert.Contains(t, string(setup), "clang-format==23.1.2")
		source, readErr := os.ReadFile(filepath.Join(root, "sample.c"))
		assert.NoError(t, readErr)
		assert.Equal(t, "int value=1;\n", string(source))
	})
	t.Run("shfmt", func(t *testing.T) {
		root := scriptFixture(t, "scripts/_260_format_shell_scripts.sh")
		writeFixture(t, root, "sample.sh", "#!/bin/sh\nexit 0\n")
		writeFixture(t, root, "bin/git", "#!/bin/sh\nprintf 'sample.sh\\n'\n")
		writeFixture(t, root, "bin/shfmt", "#!/bin/sh\nif [ \"$1\" = --version ]; then echo v3.12.0; fi\n")
		writeFixture(t, root, "seed/shfmt", "#!/bin/sh\nif [ \"$1\" = --version ]; then echo v3.8.0; fi\n")
		writeFixture(t, root, "bin/go", "#!/bin/sh\nprintf '%s\\n' \"$*\" > installed-package.txt\ncp seed/shfmt \"$GOBIN/shfmt\"\n")
		out, err := runFixture(t, root, "printf 'yes\\n' | bash scripts/_260_format_shell_scripts.sh check", map[string]string{"CI": "false"})
		assert.NoError(t, err, out)
		installed, readErr := os.ReadFile(filepath.Join(root, "installed-package.txt"))
		assert.NoError(t, readErr)
		assert.Equal(t, "install mvdan.cc/sh/v3/cmd/shfmt@v3.8.0\n", string(installed))
		assert.FileExists(t, filepath.Join(root, "temp/tools/shfmt"))
		source, readErr := os.ReadFile(filepath.Join(root, "sample.sh"))
		assert.NoError(t, readErr)
		assert.Equal(t, "#!/bin/sh\nexit 0\n", string(source))
	})
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

// TestScratchpadExcludedFromGo verifies the underscore directory convention
// without requiring any marker file in the user's notes. Broken archived Go
// sources stay untouched while active tests and global coverage still run.
func TestScratchpadExcludedFromGo(t *testing.T) {
	t.Run("repository_discovery_keeps_active_packages_and_excludes_scratchpad", func(t *testing.T) {
		cmd := exec.Command("go", "list", "./...")
		cmd.Dir = ".."
		output, err := cmd.CombinedOutput()
		if !assert.NoError(t, err, string(output)) {
			t.FailNow()
		}
		packages := strings.Fields(string(output))
		for _, name := range []string{"cmd/trice", "internal/id", "pkg/tst", "scripts"} {
			assert.Contains(t, packages, "github.com/rokath/trice/"+name,
				"the archive exclusion must retain active tests")
		}
		for _, name := range packages {
			assert.NotContains(t, name, "/docs/_scratchPad",
				"drafts and obsolete sources must not become root-module test targets")
		}
	})

	for _, tc := range []struct {
		name     string
		args     []string
		coverage bool
	}{
		{"normal_tests_ignore_uncompilable_archived_tests", []string{"test", "-v", "./..."}, false},
		{"global_coverage_ignores_uncompilable_archived_tests", []string{"test", "-v", "./...", "-covermode=atomic", "-coverpkg=./...", "-coverprofile=coverage.out"}, true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := t.TempDir()
			writeFixture(t, root, "go.mod", "module example.com/active\n\ngo 1.25.0\n")
			writeFixture(t, root, "active.go", "package active\nfunc Value() int { return 7 }\n")
			writeFixture(t, root, "active_test.go", "package active\nimport \"testing\"\nfunc TestActiveValue(t *testing.T) { if Value() != 7 { t.Fatal(\"active behavior changed\") } }\n")
			// This reproduces the archived update tests' missing helper. If the
			// underscore exclusion stops working, both invocations fail to compile.
			archived := "package historical\nimport \"testing\"\nfunc TestRetiredUpdate(t *testing.T) { setupTest() }\n"
			writeFixture(t, root, "docs/_scratchPad/obsolete/cmd/trice/main_update_test.go", archived)
			cmd := exec.Command("go", tc.args...)
			cmd.Dir = root
			output, err := cmd.CombinedOutput()
			if !assert.NoError(t, err, string(output)) {
				t.FailNow()
			}
			assert.Contains(t, string(output), "--- PASS: TestActiveValue",
				"success must include executing the active test, not an empty selection")
			assert.NotContains(t, string(output), "TestRetiredUpdate")
			preserved, err := os.ReadFile(filepath.Join(root, "docs/_scratchPad/obsolete/cmd/trice/main_update_test.go"))
			assert.NoError(t, err)
			assert.Equal(t, archived, string(preserved), "historical sources remain unchanged")
			if tc.coverage {
				profile, err := os.ReadFile(filepath.Join(root, "coverage.out"))
				assert.NoError(t, err)
				assert.Contains(t, string(profile), "example.com/active/active.go")
				assert.NotContains(t, string(profile), "scratchPad",
					"archived packages must also stay out of global coverage targets")
			}
		})
	}
}

// TestRepositoryFormattingCheckPreservesOutputs exercises wrapper dispatch with
// destructive helper doubles confined to a fixture. Check mode must not call
// Bind or cleanup, even after an explicit earlier format argument.
func TestRepositoryFormattingCheckPreservesOutputs(t *testing.T) {
	for _, tc := range []struct {
		name, arguments, failed string
		format                  bool
	}{
		{name: "check", arguments: "check"},
		{name: "verbose_check", arguments: "check --verbose"},
		{name: "last_mode_is_check", arguments: "format check"},
		{name: "default_formats_and_cleans", format: true},
		{name: "explicit_format", arguments: "format", format: true},
		{name: "last_mode_is_format", arguments: "check format", format: true},
		{name: "failed_check_stops_without_cleanup", arguments: "check", failed: "_270_format_go_code.sh"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/format_repo.sh")
			for _, name := range []string{
				"scripts/_310_refresh_trice_user_manual.sh", "scripts/_260_format_shell_scripts.sh",
				"scripts/_270_format_go_code.sh", "scripts/_280_format_c_code.sh",
				"trice_bindIDs_in_examples_and_test_folder.sh", "examples/cleanAllTargets.sh",
				"scripts/_300_clean_dsstore.sh",
			} {
				writeFixture(t, root, name, `#!/bin/sh
name=${0##*/}
printf '%s\n' "$name" >> helpers.calls
[ "$name" != "$FAILED_HELPER" ] || exit 19
case "$name" in
  trice_bindIDs*) printf 'updated IDs\n' > demoTIL.json ;;
  cleanAllTargets.sh) rm -f examples/fixture/build/output ;;
  _300_clean_dsstore.sh) rm -f .DS_Store ;;
esac
`)
			}
			for _, name := range []string{"demoTIL.json", ".DS_Store", "examples/fixture/build/output"} {
				writeFixture(t, root, name, "preserve this\n")
			}
			out, err := runFixture(t, root, "bash scripts/format_repo.sh "+tc.arguments, map[string]string{"FAILED_HELPER": tc.failed})
			assert.Equal(t, tc.failed != "", err != nil, out)
			calls, readErr := os.ReadFile(filepath.Join(root, "helpers.calls"))
			assert.NoError(t, readErr)
			expected := "_310_refresh_trice_user_manual.sh\n_260_format_shell_scripts.sh\n_270_format_go_code.sh\n"
			if tc.failed == "" {
				expected += "_280_format_c_code.sh\n"
			}
			if tc.format {
				expected += "trice_bindIDs_in_examples_and_test_folder.sh\n_280_format_c_code.sh\ncleanAllTargets.sh\n_300_clean_dsstore.sh\n"
				assert.NoFileExists(t, filepath.Join(root, ".DS_Store"))
				assert.NoFileExists(t, filepath.Join(root, "examples/fixture/build/output"))
			} else {
				for _, name := range []string{"demoTIL.json", ".DS_Store", "examples/fixture/build/output"} {
					data, err := os.ReadFile(filepath.Join(root, name))
					assert.NoError(t, err)
					assert.Equal(t, "preserve this\n", string(data), name)
				}
			}
			assert.Equal(t, expected, string(calls))
		})
	}
}

// TestLegacyIDRefreshChecksInputsAndStopsOnFailure preserves the deliberate
// history-reset contract while keeping incomplete inputs and failed tools from
// triggering later source rewrites, generated copies or build cleanup.
func TestLegacyIDRefreshChecksInputsAndStopsOnFailure(t *testing.T) {
	for _, tc := range []struct {
		name, arguments, missing, failed string
	}{
		{name: "explicit_legacy_reseed"},
		{name: "retain_existing_history", arguments: "keepHistory"},
		{name: "missing_canonical_template", missing: "_test/testdata/cgoPackage_test.go"},
		{name: "missing_configured_package", missing: "_test/alias_dblB_de_tcobs_ua"},
		{name: "failed_clean_stops_refresh", arguments: "keepHistory", failed: "clean"},
		{name: "failed_insert_stops_refresh", arguments: "keepHistory", failed: "insert"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_330_renew_ids_and_refresh_tests.sh", "scripts/_120_setup_trice_environment.sh")
			// Mirror directory names only; the fixture never touches real CGO sources.
			entries, err := os.ReadDir(filepath.Join("..", "_test"))
			if !assert.NoError(t, err) {
				return
			}
			for _, entry := range entries {
				if entry.IsDir() {
					writeFixture(t, root, "_test/"+entry.Name()+"/generated_cgoPackage.go", "original harness\n")
				}
			}
			for _, name := range []string{"cgoPackage.go", "cgoPackage_test.go"} {
				writeFixture(t, root, "_test/testdata/"+name, "canonical "+name+"\n")
			}
			for _, name := range []string{"demoTIL.json", "demoLI.json"} {
				writeFixture(t, root, name, "retained history\n")
			}
			writeFixture(t, root, "bin/trice", `#!/bin/sh
printf '%s\n' "$*" >> trice.calls
[ "$1" != "$FAILED_ACTION" ] || exit 19
`)
			writeFixture(t, root, "examples/cleanAllTargets.sh", "#!/bin/sh\ntouch ../cleanup.called\n")
			if tc.missing != "" {
				assert.NoError(t, os.RemoveAll(filepath.Join(root, tc.missing)))
			}
			out, err := runFixture(t, root, "bash scripts/_330_renew_ids_and_refresh_tests.sh "+tc.arguments, map[string]string{"FAILED_ACTION": tc.failed})
			assert.Equal(t, tc.missing != "" || tc.failed != "", err != nil, out)
			if tc.missing != "" {
				assert.Contains(t, out, "ERROR:")
				assert.Contains(t, out, filepath.Base(tc.missing))
				assert.NoFileExists(t, filepath.Join(root, "trice.calls"))
			} else {
				calls, readErr := os.ReadFile(filepath.Join(root, "trice.calls"))
				assert.NoError(t, readErr)
				lines := strings.Split(strings.TrimSpace(string(calls)), "\n")
				expectedCalls := 9 // Initial clean, seven ordered inserts, final clean.
				if tc.failed == "clean" {
					expectedCalls = 1
				} else if tc.failed == "insert" {
					expectedCalls = 2
				}
				assert.Len(t, lines, expectedCalls)
				assert.True(t, strings.HasPrefix(lines[0], "clean "), string(calls))
			}
			if tc.missing != "" || tc.failed != "" {
				assert.NoFileExists(t, filepath.Join(root, "cleanup.called"))
			} else {
				assert.FileExists(t, filepath.Join(root, "cleanup.called"))
				data, readErr := os.ReadFile(filepath.Join(root, "_test/alias_dblB_de_tcobs_ua/generated_cgoPackage.go"))
				assert.NoError(t, readErr)
				assert.Equal(t, "canonical cgoPackage.go\n", string(data))
			}
			for _, name := range []string{"demoTIL.json", "demoLI.json"} {
				data, readErr := os.ReadFile(filepath.Join(root, name))
				assert.NoError(t, readErr)
				expected := "retained history\n"
				if tc.missing == "" && tc.arguments == "" {
					expected = ""
				}
				assert.Equal(t, expected, string(data), name)
			}
		})
	}
}

// TestSourceChecksExcludeScratchpad covers explicit Git file enumeration, which
// otherwise bypasses Go's underscore rule and would format or lint archived code.
func TestSourceChecksExcludeScratchpad(t *testing.T) {
	for _, tc := range []struct {
		script, tool, file, args string
	}{
		{"_260_format_shell_scripts.sh", "shfmt", "active.sh", "check"},
		{"_260_format_shell_scripts.sh", "shfmt", "active.sh", "format"},
		{"_270_format_go_code.sh", "gofmt", "active.go", "check"},
		{"_270_format_go_code.sh", "gofmt", "active.go", "format"},
		{"_410_test_shellcheck.sh", "shellcheck", "active.sh", ""},
	} {
		t.Run(tc.script+"/"+tc.args, func(t *testing.T) {
			root := scriptFixture(t, "scripts/"+tc.script, "scripts/_100_test_common.sh")
			archived := "docs/_scratchPad/obsolete/" + tc.file
			writeFixture(t, root, tc.file, "#!/bin/sh\n")
			writeFixture(t, root, archived, "deliberately invalid historical source\n")
			writeFixture(t, root, "bin/git", "#!/bin/sh\nprintf '%s\\n' '"+tc.file+"' '"+archived+"'\n")
			writeFixture(t, root, "bin/"+tc.tool, `#!/bin/sh
if [ "$1" = --version ]; then echo v3.8.0; exit 0; fi
printf '%s\n' "$@" >> tool.calls
for arg; do case "$arg" in docs/_scratchPad/*) exit 19 ;; esac; done
`)
			out, err := runFixture(t, root, "bash scripts/"+tc.script+" "+tc.args, map[string]string{"SHFMT_BIN": "./bin/shfmt"})
			assert.NoError(t, err, out)
			calls, readErr := os.ReadFile(filepath.Join(root, "tool.calls"))
			assert.NoError(t, readErr)
			assert.Contains(t, string(calls), tc.file)
			assert.NotContains(t, string(calls), "scratchPad")
			data, readErr := os.ReadFile(filepath.Join(root, archived))
			assert.NoError(t, readErr)
			assert.Equal(t, "deliberately invalid historical source\n", string(data))
			if tc.tool == "shfmt" && tc.args == "check" {
				assert.NoDirExists(t, filepath.Join(root, "temp/tools"), "checks must not create a tool installation directory")
			}
		})
	}
}

// TestRunnerLogsAndCancellation checks cleanup boundaries, quiet skip reporting,
// default continuation, explicit stopping and cancellation without launching the
// real suite. Later successful checks must never hide an earlier failure.
func TestRunnerLogsAndCancellation(t *testing.T) {
	for _, tc := range []struct {
		name, args, status, result, policy string
		later                              bool
	}{
		{"no_arguments_success", "", "0", "PASS", "1", true},
		{"no_arguments_continue_after_failure", "", "1", "FAIL", "1", true},
		{"quick_continues_after_failure_by_default", "quick", "1", "FAIL", "1", true},
		{"full_continues_after_failure_by_default", "full", "1", "FAIL", "1", true},
		{"explicit_stop_after_selection", "quick --stop", "1", "FAIL", "0", false},
		{"explicit_stop_before_selection", "--stop full", "1", "FAIL", "0", false},
		{"explicit_no_stop_remains_supported", "quick --no-stop", "1", "FAIL", "1", true},
		{"last_policy_option_enables_stopping", "--no-stop --stop", "1", "FAIL", "0", false},
		{"last_policy_option_restores_continuation", "--stop --no-stop", "1", "FAIL", "1", true},
		{"interrupt_stops_even_with_default_continuation", "quick", "130", "ABORTED", "1", false},
		{"terminate_stops_even_with_explicit_continuation", "quick --no-stop", "143", "ABORTED", "1", false},
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
			// A real child script observes the exported policy, just as PC workers
			// do. This guards against changing only the top-level progress output.
			firstStep := "#!/usr/bin/env bash\nsource scripts/_100_test_common.sh\ninit_logfile\nlog \"child continuation policy: $TRICE_TEST_NO_STOP\"\n"
			if tc.status == "1" {
				firstStep += "log 'EXPECTATION FAILURE triceCheck.c:123 channel=direct'\nlog 'want: hello; got: wrong'\n"
			} else {
				firstStep += "log 'SKIP: unsupported tool version'\n"
			}
			writeFixture(t, root, "scripts/_410_test_first.sh", firstStep+"exit "+tc.status+"\n")
			writeFixture(t, root, "scripts/_430_test_later.sh", "#!/usr/bin/env bash\nsource scripts/_100_test_common.sh\ninit_logfile\nlog executed\n")
			out, err := runFixture(t, root, "source scripts/_110_test_runner.sh; build_test_plan() { add_plan_step _410_test_first.sh; add_plan_step _430_test_later.sh; }; main "+tc.args, nil)
			if tc.status == "0" {
				assert.NoError(t, err, out)
				assert.Contains(t, out, "WARN")
				assert.Contains(t, out, "SKIP: unsupported tool version")
			} else {
				assert.Error(t, err, out)
				if exitErr, ok := err.(*exec.ExitError); assert.True(t, ok, out) {
					assert.Equal(t, tc.status, strconv.Itoa(exitErr.ExitCode()), out)
				}
			}
			assert.Contains(t, out, "Result: "+tc.result)
			// Every completed step, including WARN/FAIL/ABORTED, retains the
			// same time-prefixed line in stdout and the persistent summary.
			summary, readErr := os.ReadFile(filepath.Join(root, "temp/log/testAll_summary.log"))
			assert.NoError(t, readErr)
			resultLines := regexp.MustCompile(`(?m)^\[ *\d+/ *\d+ \| ~ *\d+\.\d%\] \( *\d+m *\d+s\) _\w+\.sh: (PASS|WARN|FAIL|ABORTED)$`)
			consoleLines := resultLines.FindAllString(out, -1)
			wantLines := 1
			if tc.later {
				wantLines = 2
			}
			assert.Len(t, consoleLines, wantLines, out)
			assert.Equal(t, consoleLines, resultLines.FindAllString(string(summary), -1))
			assert.NotContains(t, string(summary), ": running")
			if tc.policy == "1" {
				assert.Contains(t, out, "Failure policy: continue after failures (--no-stop, default)")
			} else {
				assert.Contains(t, out, "Failure policy: stop after the first failure (--stop)")
			}
			firstLog, readErr := os.ReadFile(filepath.Join(root, "temp/log/_410_test_first.log"))
			assert.NoError(t, readErr)
			assert.Contains(t, string(firstLog), "child continuation policy: "+tc.policy)
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

// TestRunnerDurationAlignment checks the exact public layout without waiting
// for minutes or hours. Large minute counts grow rather than wrap or truncate.
func TestRunnerDurationAlignment(t *testing.T) {
	root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_110_test_runner.sh")
	for _, tc := range []struct {
		name, seconds, display string
	}{
		{"instant_step", "0", "(  0m  0s)"},
		{"single_digit_seconds", "9", "(  0m  9s)"},
		{"last_second_before_minute", "59", "(  0m 59s)"},
		{"whole_minute", "60", "(  1m  0s)"},
		{"minute_and_one_second", "61", "(  1m  1s)"},
		{"last_second_before_hour", "3599", "( 59m 59s)"},
		{"whole_hour_remains_minutes", "3600", "( 60m  0s)"},
		{"four_hour_windows_step", "14400", "(240m  0s)"},
		{"three_digit_minute_limit", "59999", "(999m 59s)"},
		{"longer_runs_are_not_truncated", "60000", "(1000m  0s)"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			out, err := runFixture(t, root, `source scripts/_110_test_runner.sh
TEST_ALL_STEP_NUMBER=3 TEST_ALL_STEP_COUNT=25 TEST_ALL_STEP_END_TENTHS=14
step_progress_prefix _420_test_dsstore.sh `+tc.seconds, nil)
			assert.NoError(t, err, out)
			assert.Equal(t, "[ 3/25 | ~  1.4%] "+tc.display+" _420_test_dsstore.sh: ", out)
		})
	}
}

// TestRunnerTimesEachStepIndependently exercises real child execution. Starting
// the shell counter far above zero exposes accidental reporting of runner
// uptime instead of the elapsed time for each script. Advancing it between
// steps also catches reuse of the first start time. Only one second is spent
// sleeping; the format boundaries above use fixed inputs instead.
func TestRunnerTimesEachStepIndependently(t *testing.T) {
	root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_110_test_runner.sh")
	writeFixture(t, root, "scripts/_410_test_delayed.sh", "#!/usr/bin/env bash\nsleep 1\n")
	writeFixture(t, root, "scripts/_430_test_immediate.sh", "#!/usr/bin/env bash\nexit 0\n")
	out, err := runFixture(t, root, `source scripts/_110_test_runner.sh
mkdir -p "$LOG_DIR"
SUMMARY_LOG="$LOG_DIR/testAll_summary.log"
TEST_ALL_STEP_COUNT=2 TEST_ALL_STEP_NUMBER=1 TEST_ALL_STEP_END_TENTHS=500
SECONDS=500000
run_step _410_test_delayed.sh
SECONDS=900000
TEST_ALL_STEP_NUMBER=2 TEST_ALL_STEP_END_TENTHS=1000
run_step _430_test_immediate.sh`, nil)
	assert.NoError(t, err, out)
	results := regexp.MustCompile(`\( *(\d+)m *(\d+)s\) (_\w+\.sh): PASS`).FindAllStringSubmatch(out, -1)
	if !assert.Len(t, results, 2, out) {
		return
	}
	for i, result := range results {
		minutes, err := strconv.Atoi(result[1])
		assert.NoError(t, err)
		seconds, err := strconv.Atoi(result[2])
		assert.NoError(t, err)
		assert.Less(t, seconds, 60)
		assert.Less(t, minutes*60+seconds, 400000, "must subtract this step's own starting counter")
		if i == 0 {
			assert.GreaterOrEqual(t, minutes*60+seconds, 1, "must include the real child wait")
		}
	}
}

// TestRunnerSpinnerUsesTheSameTimeColumn renders one frame without a terminal
// or a two-second delay. Freezing the shell counter makes the frame deterministic.
func TestRunnerSpinnerUsesTheSameTimeColumn(t *testing.T) {
	root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_110_test_runner.sh")
	out, err := runFixture(t, root, `source scripts/_110_test_runner.sh
TEST_ALL_STEP_NUMBER=23 TEST_ALL_STEP_COUNT=25 TEST_ALL_STEP_END_TENTHS=765
unset SECONDS
SECONDS=1270
sleep() { exit 0; }
progress_spinner _620_test_l432_configs.sh 1000`, nil)
	assert.NoError(t, err, out)
	assert.Equal(t, "\r[23/25 | ~ 76.5%] (  4m 30s) _620_test_l432_configs.sh: |", out)
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
				"examples/F030_inst_rtt/Core/Src/main.c":   "local RTT source\n",
				"examples/F030_inst_rtt/til.json":          "local RTT TIL\n",
				"examples/F030_inst_rtt/li.json":           "local RTT LI\n",
				"examples/F030_inst_rtt/out/user.o":        "local RTT build\n",
				"examples/F030_inst_uart/Core/Inc/conf.h":  "local UART header\n",
				"examples/F030_inst_uart/til.json":         "local UART TIL\n",
			}
			for path, contents := range initial {
				writeFixture(t, root, path, contents)
			}
			writeFixture(t, root, "bin/trice", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "trice_bindIDs_in_examples_and_test_folder.sh", "#!/bin/bash\nprintf 'bound source\\n' > sample.c\nprintf 'bound central TIL\\n' > demoTIL.json\nprintf 'bound central LI\\n' > demoLI.json\nprintf 'new selection\\n' > generated/user-selection.h\n")
			writeFixture(t, root, "worker.sh", "#!/bin/bash\nprintf 'updated PC TIL\\n' > examples/PC_log/til.json\nprintf 'updated PC LI\\n' > examples/PC_log/li.json\nprintf 'updated MCU LI\\n' > examples/G0B1_log/li.json\nprintf 'new MCU TIL\\n' > examples/G0B1_log/til.json\nmkdir -p examples/G0B1_log/generated\nprintf 'new header\\n' > examples/G0B1_log/generated/sidecar.h\nprintf 'overwrite\\n' > examples/PC_log/generated/user-choice.h\nprintf 'overwrite\\n' > examples/PC_log/build/user-object.o\nprintf 'overwrite\\n' > examples/G0B1_log/out.gcc/user-object.o\nprintf 'overwrite\\n' > examples/G0B1_inst/out.gcc/user-object.o\n"+tc.workerEnd+"\n")
			// The added local examples must restore their private ownership data,
			// including paths that did not exist before the standalone build.
			worker, err := os.ReadFile(filepath.Join(root, "worker.sh"))
			assert.NoError(t, err)
			localChanges := "printf 'bound RTT source\\n' > examples/F030_inst_rtt/Core/Src/main.c\nprintf 'updated RTT TIL\\n' > examples/F030_inst_rtt/til.json\nprintf 'updated RTT LI\\n' > examples/F030_inst_rtt/li.json\nprintf 'overwrite\\n' > examples/F030_inst_rtt/out/user.o\nprintf 'bound UART header\\n' > examples/F030_inst_uart/Core/Inc/conf.h\nprintf 'updated UART TIL\\n' > examples/F030_inst_uart/til.json\nprintf 'new UART LI\\n' > examples/F030_inst_uart/li.json\nmkdir -p examples/F030_inst_uart/out/sidecars\nprintf 'new UART sidecar\\n' > examples/F030_inst_uart/out/sidecars/generated.h\n"
			// Preparation may read caller input, but the build worker must receive EOF.
			writeFixture(t, root, "worker.sh", strings.Replace(string(worker), "#!/bin/bash\n", "#!/bin/bash\nif IFS= read -r unexpected; then echo unexpected-worker-input; exit 91; fi\necho worker-input-is-EOF\n", 1))
			worker, err = os.ReadFile(filepath.Join(root, "worker.sh"))
			assert.NoError(t, err)
			writeFixture(t, root, "worker.sh", strings.Replace(string(worker), tc.workerEnd, localChanges+tc.workerEnd, 1))
			preparation, err := os.ReadFile(filepath.Join(root, "trice_bindIDs_in_examples_and_test_folder.sh"))
			assert.NoError(t, err)
			writeFixture(t, root, "trice_bindIDs_in_examples_and_test_folder.sh", strings.Replace(string(preparation), "#!/bin/bash\n", "#!/bin/bash\nIFS= read -r answer\n[ \"$answer\" = preparation-input ] || exit 92\necho preparation-received-input\n", 1))
			out, err := runFixture(t, root, "printf '%s\\n' preparation-input reserved-input | (source scripts/_140_trice_test_state.sh; trice_test_run_managed_workflow bind example ./worker.sh)", map[string]string{"TRICE_PRJ_FILES": "-src ./sample.c"})
			if tc.name == "success" {
				assert.NoError(t, err, out)
			} else {
				assert.Error(t, err, out)
			}
			assert.Contains(t, out, tc.wantStatus)
			assert.Contains(t, out, "preparation-received-input")
			assert.Contains(t, out, "worker-input-is-EOF")
			assert.Contains(t, out, "exact initial files restored")
			for path, contents := range initial {
				data, readErr := os.ReadFile(filepath.Join(root, path))
				assert.NoError(t, readErr, path)
				assert.Equal(t, contents, string(data), path)
			}
			for _, path := range []string{"examples/G0B1_log/til.json", "examples/G0B1_log/generated", "examples/F030_inst_uart/li.json", "examples/F030_inst_uart/out"} {
				_, statErr := os.Lstat(filepath.Join(root, path))
				assert.True(t, os.IsNotExist(statErr), path)
			}
		})
	}
}

// TestExampleMatricesStopOnCancellation checks both signal exit codes and actual
// signals while preserving the documented continuation after ordinary failures.
func TestExampleMatricesStopOnCancellation(t *testing.T) {
	for _, mode := range []string{"ON", "OFF"} {
		for _, outcome := range []string{"0", "7", "130", "143", "INT", "TERM"} {
			t.Run(mode+"/"+outcome, func(t *testing.T) {
				script := "examples/buildAllTargets_TRICE_" + mode + ".sh"
				root := scriptFixture(t, script)
				writeFixture(t, root, "examples/prepareTriceBind.sh", "prepare_trice_bind_build() { :; }\n")
				first := "F030_bare"
				if mode == "ON" {
					first = "DemoData_CSV"
				}
				for _, target := range []string{"F030_bare", "G0B1_bare", "L432_bare", "F030_inst", "G0B1_inst", "L432_inst", "DemoData_CSV", "DemoData_Trice"} {
					writeFixture(t, root, "examples/"+target+"/build.sh", `#!/bin/bash
name=${PWD##*/}
printf '%s\n' "$name" >> "$FIXTURE_ROOT/build.calls"
if [ "$name" = "$FIRST_TARGET" ]; then
  case "$OUTCOME" in
    INT | TERM) kill -s "$OUTCOME" "$PPID"; exit 0 ;;
    *) exit "$OUTCOME" ;;
  esac
fi
`)
				}
				out, err := runFixture(t, root, "bash "+script, map[string]string{"FIXTURE_ROOT": root, "FIRST_TARGET": first, "OUTCOME": outcome})
				want := outcome
				switch outcome {
				case "7":
					want = "1"
				case "INT":
					want = "130"
				case "TERM":
					want = "143"
				}
				if want == "0" {
					assert.NoError(t, err, out)
				} else if assert.Error(t, err, out) {
					assert.Equal(t, "exit status "+want, err.Error(), out)
				}
				calls, readErr := os.ReadFile(filepath.Join(root, "build.calls"))
				assert.NoError(t, readErr)
				if want == "130" || want == "143" {
					assert.Equal(t, first+"\n", string(calls), "cancellation must not start another target")
				} else {
					assert.Contains(t, string(calls), "L432_inst\n", "ordinary failure must still test later targets")
				}
			})
		}
	}
}

// TestBatchWorkersPropagateCancellation prevents --no-stop from swallowing an
// interrupted child or diagnostic and starting another PC/L432 configuration.
func TestBatchWorkersPropagateCancellation(t *testing.T) {
	for _, status := range []string{"130", "143"} {
		for _, phase := range []string{"pc", "diagnostic", "l432"} {
			t.Run(phase+"/"+status, func(t *testing.T) {
				var root, command string
				env := map[string]string{"TRICE_TEST_NO_STOP": "1", "CANCEL_STATUS": status}
				if phase == "l432" {
					root = l432Fixture(t)
					writeFixture(t, root, "bin/make", "#!/bin/sh\necho called >> ../../events/build.calls\nexit \"$CANCEL_STATUS\"\n")
					command = "bash examples/L432_inst/all_configs_build.sh"
					env["TRICE_L432_TEST_JOBS"] = "1"
				} else {
					root = pcWorkerFixture(t)
					data, err := os.ReadFile(filepath.Join(root, "bin/go"))
					assert.NoError(t, err)
					// Keep real package discovery and overlay setup; only test execution
					// returns a controlled cancellation code at the selected phase.
					writeFixture(t, root, "bin/go", strings.Replace(string(data), "for package; do :; done", `if [ "$CANCEL_PHASE" = pc ]; then exit "$CANCEL_STATUS"; fi
if [ "$TRICE_PC_TEST_MODE" = line-by-line ]; then exit "$CANCEL_STATUS"; fi
for package; do :; done`, 1))
					command = "mkdir -p temp/log; bash scripts/_160_pc_target_test_worker.sh full"
					env["TRICE_PC_TEST_JOBS"] = "1"
					env["TRICE_PC_TEST_MODE"] = "auto"
					env["CANCEL_PHASE"] = phase
					env["FAIL_B"] = "1"
				}
				out, err := runFixture(t, root, command, env)
				if assert.Error(t, err, out) {
					assert.Equal(t, "exit status "+status, err.Error(), out)
				}
				assert.NoFileExists(t, filepath.Join(root, "events/c.auto.args"))
				if phase == "l432" {
					calls, readErr := os.ReadFile(filepath.Join(root, "events/build.calls"))
					assert.NoError(t, readErr)
					assert.Equal(t, "called\n", string(calls))
				}
			})
		}
	}
}

// TestBatchCancellationEscalatesBeforeWaiting models an uncooperative child:
// waiting is allowed only after KILL, so a blocked wait cannot hide the fallback.
func TestBatchCancellationEscalatesBeforeWaiting(t *testing.T) {
	for _, tc := range []struct{ script, setup, cancel string }{
		{"scripts/_160_pc_target_test_worker.sh", "PC_PIDS=(424242)", "cancel_pc_jobs"},
		{"examples/L432_inst/all_configs_build.sh", "L432_PIDS=(424242); L432_RUN_DIR=fixture", "cancel_l432_jobs"},
	} {
		t.Run(tc.cancel, func(t *testing.T) {
			root := scriptFixture(t, tc.script)
			data, err := os.ReadFile(filepath.Join(root, tc.script))
			assert.NoError(t, err)
			// Load the actual cancellation function without launching a build matrix.
			writeFixture(t, root, tc.script, strings.Replace(string(data), "\nmain \"$@\"\n", "\n", 1))
			out, err := runFixture(t, root, "source "+tc.script+"\n"+tc.setup+`
terminated=0
kill() {
  case "$1" in
    -TERM) echo TERM; return 0 ;;
    -0) [ "$terminated" -eq 0 ]; return $? ;;
    -KILL) echo KILL; terminated=1; return 0 ;;
    *) exit 93 ;;
  esac
}
sleep() { :; }
wait() {
  [ "$terminated" -eq 1 ] || { echo premature-wait; exit 94; }
  echo reaped
}
`+tc.cancel+" 143", nil)
			if assert.Error(t, err, out) {
				assert.Equal(t, "exit status 143", err.Error(), out)
			}
			assert.Contains(t, out, "TERM\nKILL\nreaped\n")
			assert.NotContains(t, out, "premature-wait")
		})
	}
}

// TestFeatureBuildsDetachInput covers the isolated feature-build path, which
// intentionally does not use the managed ID-workflow worker boundary.
func TestFeatureBuildsDetachInput(t *testing.T) {
	root, _ := loggingFeatureFixture(t)
	for _, script := range []string{"examples/PC_features/check_output.sh", "examples/G0B1_features/check_build.sh"} {
		data, err := os.ReadFile(filepath.Join(root, script))
		assert.NoError(t, err)
		writeFixture(t, root, script, strings.Replace(string(data), "set -eu\n", "set -eu\nif IFS= read -r unexpected; then echo unexpected-build-input; exit 91; fi\necho feature-input-is-EOF\n", 1))
	}
	out, err := runFixture(t, root, "printf '%s\\n' caller-input | ./scripts/_515_test_logging_features.sh full", nil)
	assert.NoError(t, err, out)
	assert.Equal(t, 3, strings.Count(out, "feature-input-is-EOF"), "PC, GCC and Clang builds must each see EOF")
	assert.NotContains(t, out, "unexpected-build-input")
}

// TestManagedWorkflowRepeatedInterrupts signals the complete test process group,
// as terminal Ctrl-C does. A handshake holds real tar extraction open so later
// signals deterministically reach both the restoring shell and its child tool.
func TestManagedWorkflowRepeatedInterrupts(t *testing.T) {
	if runtime.GOOS == "windows" {
		t.Skip("POSIX process-group signals; ordinary state restoration is tested on Windows")
	}
	tar, err := exec.LookPath("tar")
	if err != nil {
		t.Skip("tar is required for managed workflow restoration")
	}
	for _, firstSignal := range []string{"INT", "TERM"} {
		for _, entry := range []string{"standalone", "testAll", "testAll.sh"} {
			t.Run(firstSignal+"/"+entry, func(t *testing.T) {
				root := scriptFixture(t,
					"scripts/testAll.sh", "scripts/_100_test_common.sh", "scripts/_110_test_runner.sh",
					"scripts/_120_setup_trice_environment.sh", "scripts/_130_trice_id_workflow.sh",
					"scripts/_140_trice_test_state.sh")
				initGitFixture(t, root)
				initial := map[string]string{
					"sample.c":            "#include \"trice_sidecar_sample.h\"\n// local user edit\ntrice(\"msg:hello\\n\");\n",
					"demoTIL.json":        "local TIL bytes\n",
					"demoLI.json":         "local LI bytes\n",
					"generated/sidecar.h": "local sidecar bytes\n",
				}
				for path, contents := range initial {
					writeFixture(t, root, path, contents)
				}
				writeFixture(t, root, "bin/trice", "#!/bin/sh\nexit 0\n")
				writeFixture(t, root, "trice_bindIDs_in_examples_and_test_folder.sh", "#!/bin/sh\nexit 0\n")
				// Only extraction pauses; snapshot creation still uses the host tar.
				writeFixture(t, root, "bin/tar", `#!/usr/bin/env bash
set -eu
if [ "$1" = -xf ]; then
  touch events/restore-ready
  attempts=0
  until [ -f events/restore-release ]; do
    sleep 0.01
    attempts=$((attempts+1))
    [ "$attempts" -le 500 ] || exit 91
  done
fi
exec "$REAL_TAR" "$@"
`)
				writeFixture(t, root, "worker.sh", `#!/usr/bin/env bash
printf 'trice(iD(1234), "msg:hello\\n");\n' > sample.c
printf 'changed TIL\n' > demoTIL.json
printf 'changed LI\n' > demoLI.json
rm -f generated/sidecar.h
touch generated/new-sidecar.h events/worker-ready
while :; do sleep 0.05; done
`)
				writeFixture(t, root, "scripts/_590_test_fixture.sh", `#!/usr/bin/env bash
source scripts/_100_test_common.sh
init_logfile
source scripts/_140_trice_test_state.sh
trice_test_run_managed_workflow bind none ./worker.sh
`)
				// Replace only the production plan, retaining the public wrapper,
				// actual runner, and one real managed step in the same process group.
				runner, readErr := os.ReadFile(filepath.Join(root, "scripts/_110_test_runner.sh"))
				if !assert.NoError(t, readErr) {
					return
				}
				writeFixture(t, root, "scripts/runner-implementation.sh", string(runner))
				writeFixture(t, root, "scripts/_110_test_runner.sh", `#!/usr/bin/env bash
source scripts/runner-implementation.sh
# Exercise terminal progress even though the test captures stdout.
TEST_ALL_INTERACTIVE=1
build_test_plan() {
  add_plan_step _590_test_fixture.sh
  add_plan_step _600_test_later.sh
}
main "$@"
`)
				writeFixture(t, root, "scripts/_600_test_later.sh", "#!/bin/sh\ntouch events/later-step\n")
				// Linux CI need not have a running systemd or change power settings.
				writeFixture(t, root, "bin/systemd-inhibit", "#!/bin/sh\nshift 4\nexec \"$@\"\n")
				writeFixture(t, root, "run-tests.sh", `#!/usr/bin/env bash
if [ "$ENTRY" = standalone ]; then
  exec bash scripts/_590_test_fixture.sh
fi
if [ "$ENTRY" = testAll.sh ]; then
  exec bash scripts/testAll.sh full
fi
exec bash scripts/_110_test_runner.sh full
`)
				// Job control gives the disposable job its own process group and
				// keeps SIGINT enabled even though the driver starts it in the background.
				out, err := runFixture(t, root, `set -m
mkdir -p events
git add -- sample.c demoTIL.json demoLI.json
bash run-tests.sh > runner.out 2>&1 &
job=$!
trap 'kill -KILL -- "-$job" 2>/dev/null || true; wait "$job" 2>/dev/null || true' EXIT
wait_for() {
  local attempts=0
  until [ -f "$1" ]; do
    if ! kill -0 "$job" 2>/dev/null || [ "$attempts" -ge 500 ]; then
      cat runner.out
      return 1
    fi
    sleep 0.01
    attempts=$((attempts+1))
  done
}
wait_for events/worker-ready || exit 1
kill -s "$FIRST_SIGNAL" -- "-$job"
wait_for events/restore-ready || exit 1
kill -s "$FIRST_SIGNAL" -- "-$job"
kill -s "$FIRST_SIGNAL" -- "-$job"
touch events/restore-release
status=0
wait "$job" || status=$?
cat runner.out
printf 'WorkflowExit:%s\n' "$status"
trap - EXIT
`, map[string]string{
					"TRICE_PRJ_FILES": "-src ./sample.c", "REAL_TAR": tar,
					"ENTRY": entry, "FIRST_SIGNAL": firstSignal,
				})
				assert.NoError(t, err, out)
				wantStatus := "130"
				if firstSignal == "TERM" {
					wantStatus = "143"
				}
				assert.Contains(t, out, "WorkflowExit:"+wantStatus)
				assert.Equal(t, 1, strings.Count(out, "Cancellation requested. Restoring the initial test state; please wait..."), out)
				if entry != "standalone" {
					assert.Contains(t, out, "Result: ABORTED (signal exit status "+wantStatus+")")
					assert.NotContains(t, out, "initial state was not restored")
				}
				assert.NoFileExists(t, filepath.Join(root, "events/later-step"), "cancellation never starts another test")
				log, readErr := os.ReadFile(filepath.Join(root, "temp/log/_590_test_fixture.log"))
				assert.NoError(t, readErr)
				assert.Contains(t, string(log), "exact initial files restored")
				assert.Contains(t, string(log), "end status: "+wantStatus)
				for path, contents := range initial {
					data, readErr := os.ReadFile(filepath.Join(root, path))
					assert.NoError(t, readErr, path)
					assert.Equal(t, contents, string(data), path)
				}
				assert.NoFileExists(t, filepath.Join(root, "generated/new-sidecar.h"))
				snapshots, globErr := filepath.Glob(filepath.Join(root, "temp/trice-state.*"))
				assert.NoError(t, globErr)
				assert.Empty(t, snapshots, "successful restoration removes its recovery snapshot")
			})
		}
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
			hostCompilerFixture(t, root)
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

// TestRuntimePreparationChecksPython exercises the actual full-mode entry point.
// Installation, checksums and runtimes are isolated doubles; no download or
// system Python installation can make a missing-prerequisite case pass.
func TestRuntimePreparationChecksPython(t *testing.T) {
	for _, tc := range []struct {
		name, failure, answer, machine  string
		installed, launcher, cached, ci bool
	}{
		{name: "installed", installed: true},
		{name: "launcher_only", launcher: true},
		{name: "install_amd64", answer: "yes\n"},
		{name: "install_win32", answer: "yes\n", machine: "i686"},
		{name: "install_arm64", answer: "yes\n", machine: "aarch64"},
		{name: "cached", cached: true},
		{name: "damaged_cache_with_system_fallback", cached: true, installed: true, failure: "version"},
		{name: "refused", answer: "no\n", failure: "refused"},
		{name: "no_input", failure: "refused"},
		{name: "ci", answer: "yes\n", ci: true, failure: "refused"},
		{name: "download", answer: "yes\n", failure: "download"},
		{name: "checksum", answer: "yes\n", failure: "checksum"},
		{name: "extract", answer: "yes\n", failure: "extract"},
		{name: "wrong_version", answer: "yes\n", failure: "version"},
		{name: "missing_runtime", answer: "yes\n", failure: "missing_runtime"},
		{name: "unsupported_arch", answer: "yes\n", machine: "armv7", failure: "architecture"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := hostPreparationFixture(t, "MINGW64_NT-10.0")
			nativeDownloadFixture(t, root)
			machine := tc.machine
			if machine == "" {
				machine = "x86_64"
			}
			writeFixture(t, root, "bin/uname", "#!/bin/sh\nif [ \"$1\" = -s ]; then echo MINGW64_NT-10.0; else echo "+machine+"; fi\n")
			for _, name := range []string{"python3", "python", "py"} {
				writeFixture(t, root, "bin/"+name, "#!/bin/sh\nexit 23\n")
			}
			if tc.installed {
				writeFixture(t, root, "bin/python3", "#!/bin/sh\nexit 0\n")
			}
			if tc.launcher {
				writeFixture(t, root, "bin/py", "#!/bin/sh\n[ \"$1\" = -3 ] && [ \"$2\" = -c ]\n")
			}
			writeFixture(t, root, "seed/python.exe", "#!/bin/sh\n[ \"${NATIVE_FAIL:-}\" != version ]\n")
			writeFixture(t, root, "bin/unzip", `#!/bin/sh
printf '%s\n' "$*" >> extract.calls
[ "${NATIVE_FAIL:-}" != extract ] || exit 23
while [ "$#" -gt 0 ]; do
  if [ "$1" = -d ]; then
    mkdir -p "$2" || exit 23
    [ "${NATIVE_FAIL:-}" != missing_runtime ] || exit 0
    cp seed/python.exe "$2/python.exe"
    exit $?
  fi
  shift
done
exit 91
`)
			arch := "amd64"
			digest := "9e35319bffedab8b36ad44d8f377cb4f1733bcec257a0b6d04d94f01486ddb38"
			if machine == "i686" {
				arch, digest = "win32", "4afd37bc88ed1391209a4c58deb902cd0741a476692412b23d340aff6e9285e7"
			} else if machine == "aarch64" {
				arch, digest = "arm64", "7b58787a047b400edbfcb539aa3cdb4e2ed77bf0f22cae42b7c01cedd7dd6614"
			}
			cache := "temp/tools/python/3.13.17-" + arch
			if tc.cached {
				data, err := os.ReadFile(filepath.Join(root, "seed/python.exe"))
				assert.NoError(t, err)
				writeFixture(t, root, cache+"/python.exe", string(data))
				writeFixture(t, root, cache+"/.ready", "")
			}
			env := map[string]string{"SELECTED": "full", "CI": booleanText(tc.ci), "TOOL_REPLY": tc.answer, "NATIVE_FAIL": tc.failure, "PINNED_DIGEST": digest}
			out, err := runFixture(t, root, "printf '%s' \"$TOOL_REPLY\" | bash scripts/_490_test_runtime_prepare.sh --quiet", env)
			success := tc.failure == "" || tc.installed
			assert.Equal(t, success, err == nil, out)
			logBytes, readErr := os.ReadFile(filepath.Join(root, "temp/log/_490_test_runtime_prepare.log"))
			assert.NoError(t, readErr)
			if !success {
				assert.NoFileExists(t, filepath.Join(root, cache, ".ready"))
				assert.NoFileExists(t, filepath.Join(root, "workflow.called"))
				assert.Contains(t, string(logBytes), "FAIL: Python preparation failed")
				assert.Contains(t, string(logBytes), "FAIL:")
				return
			}
			assert.FileExists(t, filepath.Join(root, "workflow.called"))
			if tc.installed || tc.launcher {
				assert.NoFileExists(t, filepath.Join(root, "download.calls"))
				assert.NoFileExists(t, filepath.Join(root, cache, ".ready"))
				return
			}
			assert.FileExists(t, filepath.Join(root, cache, ".ready"))
			if !tc.cached {
				downloads, err := os.ReadFile(filepath.Join(root, "download.calls"))
				assert.NoError(t, err)
				assert.Contains(t, string(downloads), "https://www.python.org/ftp/python/3.13.17/python-3.13.17-embed-"+arch+".zip")
				assert.Equal(t, 1, strings.Count(string(downloads), "\n"))
			}
			// A separately started step must use the private runtime, without
			// exposing the embeddable package as a general Python/venv provider.
			out, err = runFixture(t, root, `source scripts/_100_test_common.sh
python_test_tool_works "$TRICE_TEST_PYTHON"
command -v python3
`, map[string]string{"CI": "true", "NATIVE_FAIL": tc.failure})
			assert.NoError(t, err, out)
			assert.Contains(t, filepath.ToSlash(out), "/bin/python3")
			assert.NotContains(t, out, "Install Python")
		})
	}
}

// TestGoStepsAllowSlowNativeScriptFixtures checks the real command paths,
// including coverage, without compiling packages or invoking compiler matrices.
func TestGoStepsAllowSlowNativeScriptFixtures(t *testing.T) {
	for _, step := range []string{"_540_test_go.sh", "_550_test_go_coverage.sh"} {
		t.Run(step, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/"+step)
			writeFixture(t, root, "scripts/_270_format_go_code.sh", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "scripts/buildTriceTool.sh", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "bin/gofmt", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "bin/go", `#!/bin/sh
printf '%s\n' "$*" >> go.calls
case "$1" in
  env) [ "$2" != GOVERSION ] || echo go1.25.0 ;;
  list) echo github.com/rokath/trice/scripts ;;
  test)
    mkdir -p temp/log
    printf 'mode: atomic\ngithub.com/rokath/trice/scripts/fixture.go:1.1,1.2 1 1\n' > temp/log/coverage.out
    ;;
esac
`)
			out, err := runFixture(t, root, "bash scripts/"+step, nil)
			assert.NoError(t, err, out)
			calls, err := os.ReadFile(filepath.Join(root, "go.calls"))
			assert.NoError(t, err)
			assert.Contains(t, string(calls), "test -count=1 github.com/rokath/trice/scripts -timeout=15m")
		})
	}
}

// hostCompilerFixture isolates compiler probes from every host frontend in PATH.
// Its output can independently simulate a compile, link or runtime failure.
func hostCompilerFixture(t *testing.T, root string) {
	t.Helper()
	// Compiler-specific fixtures already satisfy the independent Python check.
	writeFixture(t, root, "bin/python3", "#!/bin/sh\nexit 0\n")
	for _, compiler := range []string{"cc", "gcc", "clang", "c++", "g++", "clang++"} {
		writeFixture(t, root, "bin/"+compiler, `#!/bin/sh
if [ "$1" = --version ]; then echo fixture-compiler; exit 0; fi
if [ "$1" = -dM ]; then
  echo '#define __GNUC__ 14'
  if [ "${GCC_IS_CLANG:-0}" = 1 ]; then echo '#define __clang__ 1'; fi
  exit 0
fi
printf '%s %s\n' "${0##*/}" "$*" >> compiler.calls
cat > probe-source.txt
if [ "${GCC_IS_CLANG:-0}" = 1 ] && grep -q 'full requires genuine GCC' probe-source.txt; then
  echo 'full requires genuine GCC, not a Clang compatibility alias' >&2; exit 23
fi
if [ "${CLANG_IS_GCC:-0}" = 1 ] && grep -q 'full requires genuine Clang' probe-source.txt; then
  echo 'full requires genuine Clang' >&2; exit 23
fi
if [ "${0##*/}" = "${FAILED_COMPILER:-}" ]; then echo 'missing host header or library' >&2; exit 23; fi
while [ "$#" -gt 0 ]; do
  if [ "$1" = -o ]; then
    printf '#!/bin/sh\nexit %s\n' "${RUNTIME_STATUS:-0}" > "$2"
    chmod +x "$2"
    exit 0
  fi
  shift
done
exit 91
`)
	}
}

// TestRuntimePreparationChecksHostCompilers detects a broken C or C++ frontend
// before any ID workflow and reports linker/runtime failures with setup hints.
func TestRuntimePreparationChecksHostCompilers(t *testing.T) {
	// Reproduce testAll's exported mode even when this test is run directly.
	// Basic probe cases must remain isolated while explicit full cases retain it.
	t.Setenv("SELECTED", "full")
	for _, failure := range []string{"", "clang", "clang++", "runtime"} {
		t.Run("failure_"+failure, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_490_test_runtime_prepare.sh")
			hostCompilerFixture(t, root)
			writeFixture(t, root, "bin/go", "#!/bin/sh\necho fixture-go\n")
			writeFixture(t, root, "bin/trice", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "scripts/_140_trice_test_state.sh", "trice_test_run_managed_workflow() { touch workflow.called; }\n")
			env := map[string]string{"FAILED_COMPILER": failure, "CI": "true"}
			if failure == "runtime" {
				env["RUNTIME_STATUS"] = "23"
			}
			out, err := runFixture(t, root, "bash scripts/_490_test_runtime_prepare.sh --quiet", env)
			assert.Equal(t, failure != "", err != nil, out)
			calls, readErr := os.ReadFile(filepath.Join(root, "compiler.calls"))
			assert.NoError(t, readErr)
			assert.Equal(t, 6, strings.Count(string(calls), "\n"))
			assert.Contains(t, string(calls), "clang++ -std=c++11")
			logBytes, readErr := os.ReadFile(filepath.Join(root, "temp/log/_490_test_runtime_prepare.log"))
			assert.NoError(t, readErr)
			if failure == "" {
				assert.FileExists(t, filepath.Join(root, "workflow.called"))
			} else {
				assert.NoFileExists(t, filepath.Join(root, "workflow.called"))
				assert.Contains(t, string(logBytes), "FAIL:")
				assert.Contains(t, string(logBytes), "Hint: on Windows, check both clang and clang++")
			}
		})
	}
	t.Run("full_requirements", testFullHostCompilerRequirements)
	t.Run("versioned_GCC", testVersionedHostGCC)
	t.Run("Homebrew_GCC", testHomebrewHostGCC)
	t.Run("local_Windows_bundle", testWindowsHostCompilerBundle)
}

// hostPreparationFixture isolates the real preparation step from ID workflows,
// downloads and the developer's installed tools on every test host OS.
func hostPreparationFixture(t *testing.T, platform string) string {
	t.Helper()
	root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_490_test_runtime_prepare.sh")
	hostCompilerFixture(t, root)
	writeFixture(t, root, "bin/uname", "#!/bin/sh\nif [ \"$1\" = -s ]; then echo '"+platform+"'; else echo x86_64; fi\n")
	writeFixture(t, root, "bin/go", "#!/bin/sh\nif [ \"$1\" = env ]; then echo \"${HOST_GOARCH:-amd64}\"; else echo fixture-go; fi\n")
	// Never inspect or mutate the real Homebrew installation from a fixture.
	writeFixture(t, root, "bin/brew", "#!/bin/sh\nexit 23\n")
	writeFixture(t, root, "bin/trice", "#!/bin/sh\nexit 0\n")
	writeFixture(t, root, "scripts/_140_trice_test_state.sh", "trice_test_run_managed_workflow() { touch workflow.called; }\n")
	return root
}

// testFullHostCompilerRequirements ensures a full run cannot pass through a
// missing family, a missing C++ frontend, or Apple's GCC-compatible Clang name.
func testFullHostCompilerRequirements(t *testing.T) {
	for _, platform := range []string{"Linux", "Darwin", "MINGW64_NT-10.0"} {
		for _, missing := range []string{"", "gcc", "g++", "clang", "clang++"} {
			t.Run(platform+"/missing_"+missing, func(t *testing.T) {
				root := hostPreparationFixture(t, platform)
				command := `command() {
  if [ "$1:$2" = "-v:$MISSING_COMPILER" ]; then return 1; fi
  builtin command "$@"
}
export -f command
bash scripts/_490_test_runtime_prepare.sh --quiet`
				out, err := runFixture(t, root, command, map[string]string{
					"SELECTED": "full", "CI": "true", "MISSING_COMPILER": missing,
				})
				assert.Equal(t, missing != "", err != nil, out)
				logBytes, readErr := os.ReadFile(filepath.Join(root, "temp/log/_490_test_runtime_prepare.log"))
				assert.NoError(t, readErr)
				if missing == "" {
					assert.FileExists(t, filepath.Join(root, "workflow.called"))
				} else {
					assert.Contains(t, string(logBytes), "full requires the native "+missing+" frontend")
					assert.NoFileExists(t, filepath.Join(root, "workflow.called"))
				}
				assert.NoFileExists(t, filepath.Join(root, "download.calls"))
			})
		}
	}
	t.Run("Clang_alias_is_not_GCC", func(t *testing.T) {
		root := hostPreparationFixture(t, "Darwin")
		// The test must still reject the aliases on Linux hosts with a real
		// versioned GCC in PATH; that installation is not part of this fixture.
		writeFixture(t, root, "bin/ln", "#!/bin/sh\nexit 23\n")
		out, err := runFixture(t, root, "bash scripts/_490_test_runtime_prepare.sh", map[string]string{
			"SELECTED": "full", "CI": "true", "GCC_IS_CLANG": "1",
		})
		assert.Error(t, err)
		assert.Contains(t, out, "full requires genuine GCC")
		assert.NoFileExists(t, filepath.Join(root, "workflow.called"))
	})
	t.Run("GCC_alias_is_not_Clang", func(t *testing.T) {
		root := hostPreparationFixture(t, "Linux")
		writeFixture(t, root, "bin/ln", "#!/bin/sh\nexit 23\n")
		out, err := runFixture(t, root, "bash scripts/_490_test_runtime_prepare.sh", map[string]string{
			"SELECTED": "full", "CI": "true", "CLANG_IS_GCC": "1",
		})
		assert.Error(t, err)
		assert.Contains(t, out, "full requires genuine Clang")
		assert.NoFileExists(t, filepath.Join(root, "workflow.called"))
	})
}

// testVersionedHostGCC verifies discovery and child-step reuse without assuming
// a package-manager prefix. Shell tool doubles keep this portable to Windows CI.
func testVersionedHostGCC(t *testing.T) {
	for _, platform := range []string{"Linux", "Darwin"} {
		t.Run(platform, func(t *testing.T) {
			root := hostPreparationFixture(t, platform)
			for _, compiler := range []string{"gcc", "g++"} {
				source, err := os.ReadFile(filepath.Join(root, "bin", compiler))
				assert.NoError(t, err)
				writeFixture(t, root, "bin/"+compiler+"-14", strings.ReplaceAll(string(source), "${GCC_IS_CLANG:-0}", "0"))
			}
			out, err := runFixture(t, root, "bash scripts/_490_test_runtime_prepare.sh", map[string]string{
				"SELECTED": "full", "CI": "true", "GCC_IS_CLANG": "1",
			})
			assert.NoError(t, err, out)
			assert.Contains(t, out, "using checkout-local aliases for versioned GCC")
			assert.FileExists(t, filepath.Join(root, "temp/tools/host-gcc/.ready"))
			out, err = runFixture(t, root, "source scripts/_100_test_common.sh; command -v gcc; command -v g++", nil)
			assert.NoError(t, err, out)
			assert.Contains(t, out, "/temp/tools/host-gcc/bin/gcc")
			assert.Contains(t, out, "/temp/tools/host-gcc/bin/g++")
		})
	}
}

// testHomebrewHostGCC exercises system-install consent and post-install probes
// in a disposable prefix containing spaces, without invoking real Homebrew.
func testHomebrewHostGCC(t *testing.T) {
	for _, tc := range []struct {
		name, answer, ci, failure           string
		installed, wantInstall, wantSuccess bool
	}{
		{name: "install", answer: "yes", wantInstall: true, wantSuccess: true},
		{name: "installed_unlinked", installed: true, wantSuccess: true},
		{name: "declined", answer: "no"},
		{name: "empty_answer", answer: ""},
		{name: "EOF", failure: "eof"},
		{name: "CI", answer: "yes", ci: "true"},
		{name: "install_failure", answer: "yes", failure: "install", wantInstall: true},
		{name: "missing_pair", answer: "yes", failure: "pair", wantInstall: true},
		{name: "broken_CXX", answer: "yes", failure: "g++", wantInstall: true},
		{name: "broken_runtime", answer: "yes", failure: "runtime", wantInstall: true},
		{name: "broken_installed", installed: true, failure: "g++"},
		{name: "Clang_failure_does_not_install_GCC", answer: "yes", failure: "clang"},
		{name: "no_Homebrew", answer: "yes", failure: "no_brew"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := hostPreparationFixture(t, "Darwin")
			for _, compiler := range []string{"gcc", "g++"} {
				source, err := os.ReadFile(filepath.Join(root, "bin", compiler))
				assert.NoError(t, err)
				installed := strings.ReplaceAll(string(source), "${GCC_IS_CLANG:-0}", "0")
				// An unknown future version must work without updating the script.
				writeFixture(t, root, "brew seed/"+compiler+"-99", installed)
			}
			writeFixture(t, root, "bin/brew", `#!/bin/sh
case "$*" in
  '--prefix --installed gcc')
    [ -f brew.installed ] || exit 1
    printf '%s/brew prefix\n' "$PWD"
    ;;
  'install gcc')
    printf '%s\n' "$*" >> brew.calls
    [ "$HOMEBREW_NO_INSTALL_UPGRADE:$HOMEBREW_NO_INSTALL_CLEANUP:$HOMEBREW_NO_INSTALLED_DEPENDENTS_CHECK" = 1:1:1 ] || exit 92
    echo 'fixture installation progress'
    [ "$BREW_FAILURE" != install ] || exit 23
    mkdir -p 'brew prefix/bin'
    cp 'brew seed/'* 'brew prefix/bin/'
    if [ "$BREW_FAILURE" = pair ]; then rm 'brew prefix/bin/g++-99'; fi
    touch brew.installed
    ;;
  *) exit 91 ;;
esac
`)
			if tc.installed {
				writeFixture(t, root, "brew.installed", "")
				// A previous successful run cannot keep a now-broken selection
				// marked as ready for the independently started child steps.
				writeFixture(t, root, "temp/tools/host-gcc/.ready", "")
				for _, compiler := range []string{"gcc", "g++"} {
					source, err := os.ReadFile(filepath.Join(root, "brew seed", compiler+"-99"))
					assert.NoError(t, err)
					writeFixture(t, root, "brew prefix/bin/"+compiler+"-99", string(source))
				}
			}
			// Suppress unrelated versioned compilers from the machine running the
			// test, but allow the selected fixture pair to create its local aliases.
			command := `ln() {
  case "$2" in *'brew prefix/'*) command ln "$@" ;; *) return 1 ;; esac
}
export -f ln
`
			if tc.failure == "no_brew" {
				command += `command() {
  if [ "$1:$2" = '-v:brew' ]; then return 1; fi
  builtin command "$@"
}
export -f command
`
			}
			if tc.failure == "eof" {
				command += "bash scripts/_490_test_runtime_prepare.sh --quiet </dev/null"
			} else {
				command += "printf '" + tc.answer + "\\n' | bash scripts/_490_test_runtime_prepare.sh --quiet"
			}
			env := map[string]string{"SELECTED": "full", "CI": "false", "GCC_IS_CLANG": "1", "BREW_FAILURE": tc.failure}
			if tc.ci != "" {
				env["CI"] = tc.ci
			}
			if tc.failure == "g++" {
				env["FAILED_COMPILER"] = "g++"
			}
			if tc.failure == "runtime" {
				env["RUNTIME_STATUS"] = "23"
			}
			if tc.failure == "clang" {
				env["GCC_IS_CLANG"] = "0"
				env["FAILED_COMPILER"] = "clang"
			}
			out, err := runFixture(t, root, command, env)
			logBytes, readErr := os.ReadFile(filepath.Join(root, "temp/log/_490_test_runtime_prepare.log"))
			assert.NoError(t, readErr)
			assert.Equal(t, tc.wantSuccess, err == nil, out+string(logBytes))
			if tc.wantInstall {
				calls, readErr := os.ReadFile(filepath.Join(root, "brew.calls"))
				assert.NoError(t, readErr)
				assert.Equal(t, "install gcc\n", string(calls))
				assert.Contains(t, out, "outside this checkout")
				assert.Contains(t, out, "fixture installation progress")
				assert.NotContains(t, out, "locally under temp/tools?")
			} else {
				assert.NoFileExists(t, filepath.Join(root, "brew.calls"))
			}
			if tc.wantSuccess {
				assert.FileExists(t, filepath.Join(root, "workflow.called"))
				assert.FileExists(t, filepath.Join(root, "temp/tools/host-gcc/.ready"))
				out, err = runFixture(t, root, "source scripts/_100_test_common.sh; command -v gcc; command -v g++", nil)
				assert.NoError(t, err, out)
				assert.Contains(t, out, "/temp/tools/host-gcc/bin/gcc")
				assert.Contains(t, out, "/temp/tools/host-gcc/bin/g++")
			} else {
				assert.NoFileExists(t, filepath.Join(root, "workflow.called"))
				assert.NoFileExists(t, filepath.Join(root, "temp/tools/host-gcc/.ready"))
				if tc.failure == "install" {
					assert.Contains(t, string(logBytes), "brew install gcc exited with status 23")
				}
			}
		})
	}
}

// testWindowsHostCompilerBundle validates architecture selection, verified
// downloads, refusal/failure handling and reuse in independently started steps.
// The archive and compilers are doubles; these tests never install a real SDK.
func testWindowsHostCompilerBundle(t *testing.T) {
	for _, tc := range []struct{ name, arch, failure, answer string }{
		{"amd64", "amd64", "", "yes"}, {"386", "386", "", "yes"},
		{"unsupported_arch", "arm64", "architecture", "yes"},
		{"declined", "amd64", "declined", "no"},
		{"download", "amd64", "download", "yes"},
		{"checksum", "amd64", "checksum", "yes"},
		{"extract", "amd64", "extract", "yes"},
		{"missing_frontend", "amd64", "missing_frontend", "yes"},
		{"broken_bundle", "amd64", "probe", "yes"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := hostPreparationFixture(t, "MINGW64_NT-10.0")
			nativeDownloadFixture(t, root)
			// Installed tools are separate from the intentionally broken system Clang.
			for _, compiler := range []string{"cc", "gcc", "clang", "c++", "g++", "clang++"} {
				source, err := os.ReadFile(filepath.Join(root, "bin", compiler))
				assert.NoError(t, err)
				installed := strings.ReplaceAll(string(source), "${FAILED_COMPILER:-}", "${INSTALLED_FAILED_COMPILER:-}")
				writeFixture(t, root, "seed/"+compiler, installed)
				writeFixture(t, root, "seed/"+compiler+".exe", installed)
			}
			writeFixture(t, root, "seed/clangd.exe", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "seed/mingw32-make.exe", "#!/bin/sh\necho 'GNU Make 4.4.1'\n")
			writeFixture(t, root, "bin/unzip", `#!/bin/sh
printf '%s\n' "$*" >> extract.calls
[ "${NATIVE_FAIL:-}" != extract ] || exit 23
while [ "$#" -gt 0 ]; do
  if [ "$1" = -d ]; then
    mkdir -p "$2/$PACKAGE_ROOT/bin"
    cp seed/* "$2/$PACKAGE_ROOT/bin/" || exit 23
    if [ "${NATIVE_FAIL:-}" = missing_frontend ]; then rm "$2/$PACKAGE_ROOT/bin/clang++.exe"; fi
    exit 0
  fi
  shift
done
exit 91
`)
			packageRoot, packageArch := "mingw64", "x86_64-posix-seh"
			digest := "1f04e482797dc5f71fe2875cf2b743bfb9f5fd1de0955fdd3118438c0432ba5f"
			if tc.arch == "386" {
				packageRoot, packageArch = "mingw32", "i686-posix-dwarf"
				digest = "9353f386f7a678ab41b0746db43211c4b6b8ab2811d2edb289f0496711fb1c9b"
			}
			packageDir := "temp/tools/host/winlibs-" + packageArch + "-gcc-16.2.0-llvm-23.1.3-mingw-w64ucrt-14.0.0-r3"
			env := map[string]string{
				"SELECTED": "full", "CI": "false", "FAILED_COMPILER": "clang", "HOST_GOARCH": tc.arch,
				"PINNED_DIGEST": digest, "NATIVE_FAIL": tc.failure, "PACKAGE_ROOT": packageRoot,
			}
			if tc.failure == "probe" {
				env["INSTALLED_FAILED_COMPILER"] = "clang"
			}
			out, err := runFixture(t, root, "printf '"+tc.answer+"\\n' | bash scripts/_490_test_runtime_prepare.sh --quiet", env)
			logBytes, readErr := os.ReadFile(filepath.Join(root, "temp/log/_490_test_runtime_prepare.log"))
			assert.NoError(t, readErr)
			assert.Equal(t, tc.failure != "", err != nil, out+string(logBytes))
			if tc.failure != "" {
				assert.NoFileExists(t, filepath.Join(root, packageDir, ".ready"))
				assert.NoFileExists(t, filepath.Join(root, "workflow.called"))
				if tc.failure == "declined" || tc.failure == "architecture" {
					assert.NoFileExists(t, filepath.Join(root, "download.calls"))
				}
				if tc.failure == "checksum" || tc.failure == "download" {
					assert.NoFileExists(t, filepath.Join(root, "extract.calls"))
				}
				return
			}
			assert.FileExists(t, filepath.Join(root, packageDir, ".ready"))
			assert.FileExists(t, filepath.Join(root, packageDir, packageRoot, "bin/make.exe"))
			assert.FileExists(t, filepath.Join(root, "workflow.called"))
			downloads, readErr := os.ReadFile(filepath.Join(root, "download.calls"))
			assert.NoError(t, readErr)
			assert.Contains(t, string(downloads), "winlibs-"+packageArch)
			env["CI"] = "true"
			out, err = runFixture(t, root, "bash scripts/_490_test_runtime_prepare.sh --quiet && source scripts/_100_test_common.sh && command -v clang", env)
			assert.NoError(t, err, out)
			assert.NotContains(t, out, "Install native GCC")
			assert.Contains(t, out, "/"+packageDir+"/"+packageRoot+"/bin/clang")
			after, readErr := os.ReadFile(filepath.Join(root, "download.calls"))
			assert.NoError(t, readErr)
			assert.Equal(t, string(downloads), string(after), "a verified cache requires neither confirmation nor another download")
		})
	}
}

// TestClangWorkerDetachesBuildInput keeps noninteractive Make recipes away from
// the caller's terminal while preserving build failures and compiler diagnostics.
func TestClangWorkerDetachesBuildInput(t *testing.T) {
	for _, tc := range []struct {
		name                     string
		cleanStatus, buildStatus int
		warning                  bool
	}{
		{name: "successful_build"},
		{name: "compiler_warning", warning: true},
		{name: "clean_failure", cleanStatus: 17},
		{name: "build_failure", buildStatus: 23},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_180_clang_translation_worker.sh")
			writeFixture(t, root, "scripts/_150_setup_build_environment.sh", ":\n")
			writeFixture(t, root, "bin/clang", "#!/bin/sh\necho fixture-clang\n")
			writeFixture(t, root, "bin/make", `#!/bin/sh
# Any readable caller input means the worker failed to detach this build phase.
if IFS= read -r unexpected; then
  echo "unexpected build input: $unexpected"
  exit 91
fi
echo "EOF for $1"
case "$1" in
  clean) exit "$CLEAN_STATUS" ;;
  probe-build) exit "$BUILD_STATUS" ;;
  *) exit 92 ;;
esac
`)
			writeFixture(t, root, "examples/G0B1_inst/build_with_clang.sh", "#!/bin/sh\nexec make probe-build\n")
			logText := ""
			if tc.warning {
				logText = "warning: fixture compiler diagnostic\n"
			}
			writeFixture(t, root, "worker.log", logText)
			// The sentinel remains available to the caller even if either build fails.
			out, err := runFixture(t, root, `export LOGFILE="$PWD/worker.log"
printf '%s\n' caller-input | (
  bash scripts/_180_clang_translation_worker.sh
  status=$?
  IFS= read -r remaining
  printf 'remaining input: %s\n' "$remaining"
  exit "$status"
)
`, map[string]string{"CLEAN_STATUS": strconv.Itoa(tc.cleanStatus), "BUILD_STATUS": strconv.Itoa(tc.buildStatus)})
			wantStatus := tc.cleanStatus
			if wantStatus == 0 {
				wantStatus = tc.buildStatus
			}
			if wantStatus == 0 {
				assert.NoError(t, err, out)
			} else if assert.Error(t, err, out) {
				assert.Equal(t, "exit status "+strconv.Itoa(wantStatus), err.Error(), out)
			}
			assert.Contains(t, out, "EOF for clean")
			assert.Contains(t, out, "remaining input: caller-input")
			assert.NotContains(t, out, "unexpected build input")
			if tc.cleanStatus == 0 {
				assert.Contains(t, out, "EOF for probe-build")
			} else {
				assert.NotContains(t, out, "EOF for probe-build")
			}
			assert.Equal(t, tc.warning, strings.Contains(out, "Hint: clang translation completed with compiler warnings"), out)
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

// manualMaintenanceFixture supplies managed chapters alongside navigation,
// unmanaged notes, nested drafts and the retained monolith. The tool double
// records actual arguments and never rewrites the documentation sources.
func manualMaintenanceFixture(t *testing.T) string {
	t.Helper()
	root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_310_refresh_trice_user_manual.sh", "scripts/_440_test_manual_format.sh")
	for _, path := range []string{
		"docs/TriceUserManual.md",
		"docs/rm/README.md", "docs/rm/HEADER.md", "docs/rm/FOOTER.md", "docs/rm/INDEX.md",
		"docs/rm/overview.md", "docs/rm/binary encoding.md", "docs/rm/rtt.md",
		"docs/rm/nested/draft.md", "docs/_TriceReferenceManual.md",
	} {
		writeFixture(t, root, path, "# Fixture\n\n<!-- mdtoc -->\n<!-- /mdtoc -->\n")
	}
	writeFixture(t, root, "docs/rm/notes.md", "# Unmanaged note\n")
	writeFixture(t, root, "docs/rm/short.md", "# Short chapter\n\n```md\n## Example heading\n```\n\n<!--\n## Retired heading\n-->\n")
	writeFixture(t, root, "docs/rm/README.md", "# Reference\n\n- [Overview](overview.md)\n- [Encoding](binary encoding.md)\n- [RTT](rtt.md)\n- [Short](short.md)\n- [Introduction](HEADER.md)\n- [Footer](FOOTER.md)\n")
	writeFixture(t, root, "bin/mdtoc", `#!/bin/sh
if [ "$1" = --version ]; then printf 'mdtoc 1.0.3\n'; exit 0; fi
[ "$#" -eq 3 ] || exit 91
printf '%s --file %s\n' "$1" "$3" >> mdtoc.calls
if [ "$3" = "$REJECTED" ]; then
  echo "invalid manual: $3" >&2
  exit 9
fi
`)
	return root
}

// TestManualPinnedMdtoc rejects an incompatible formatter before processing a
// chapter and accepts a tagged Go install only after explicit consent.
func TestManualPinnedMdtoc(t *testing.T) {
	for _, tc := range []struct {
		name, version, moduleVersion, answer string
		accepted, installed                  bool
	}{
		{"release", "mdtoc 1.0.3", "", "", true, false},
		{"old_release_declined", "mdtoc 0.3.3", "", "no", false, false},
		{"unverified_dev_declined", "mdtoc dev", "v0.3.3", "no", false, false},
		{"pinned_dev", "mdtoc dev", "v1.0.3", "", true, false},
		{"old_release_approved", "mdtoc 0.3.3", "v1.0.3", "yes", true, true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := manualMaintenanceFixture(t)
			writeFixture(t, root, "bin/mdtoc", `#!/bin/sh
if [ "$1" = --version ]; then printf '%s\n' "$MDTOC_FIXTURE_VERSION"; exit 0; fi
printf '%s --file %s\n' "$1" "$3" >> mdtoc.calls
`)
			writeFixture(t, root, "seed/mdtoc", "#!/bin/sh\nif [ \"$1\" = --version ]; then echo 'mdtoc dev'; else printf '%s --file %s\\n' \"$1\" \"$3\" >> mdtoc.calls; fi\n")
			writeFixture(t, root, "bin/go", `#!/bin/sh
if [ "$1" = install ]; then
  printf '%s\n' "$2" > installed-package.txt
  cp seed/mdtoc "$GOBIN/mdtoc"
  exit 0
fi
[ "$1" = version ] && [ "$2" = -m ] && [ "$3" = "$(command -v mdtoc)" ] || exit 91
printf 'fixture: go1.25\n\tpath\tgithub.com/rokath/mdtoc/cmd/mdtoc\n\tmod\tgithub.com/rokath/mdtoc\t%s\th1:fixture\n' "$MDTOC_MODULE_VERSION"
`)
			command := "bash scripts/_310_refresh_trice_user_manual.sh check"
			if tc.answer != "" {
				command = "printf '" + tc.answer + "\\n' | " + command
			}
			out, err := runFixture(t, root, command, map[string]string{
				"MDTOC_FIXTURE_VERSION": tc.version, "MDTOC_MODULE_VERSION": tc.moduleVersion, "CI": "false",
			})
			assert.Equal(t, tc.accepted, err == nil, out)
			calls, readErr := os.ReadFile(filepath.Join(root, "mdtoc.calls"))
			if tc.accepted {
				assert.NoError(t, readErr)
				assert.Contains(t, string(calls), "check --file docs/rm/rtt.md")
			} else {
				assert.True(t, os.IsNotExist(readErr), out)
				assert.Contains(t, out, "mdtoc v1.0.3 is required")
			}
			installed, readErr := os.ReadFile(filepath.Join(root, "installed-package.txt"))
			if tc.installed {
				assert.NoError(t, readErr)
				assert.Equal(t, "github.com/rokath/mdtoc/cmd/mdtoc@v1.0.3\n", string(installed))
			} else {
				assert.True(t, os.IsNotExist(readErr), out)
			}
			if tc.name == "old_release_declined" {
				out, err = runFixture(t, root, "bash scripts/_440_test_manual_format.sh", map[string]string{"MDTOC_FIXTURE_VERSION": tc.version, "CI": "true"})
				assert.Error(t, err, out)
				assert.Contains(t, out, "FAIL: _310_refresh_trice_user_manual.sh check failed")
			}
		})
	}
}

// TestManualMaintenanceIncludesChapters verifies check/format dispatch, automatic
// discovery and propagation of formatter failures without using the old monolith.
func TestManualMaintenanceIncludesChapters(t *testing.T) {
	for _, tc := range []struct {
		name, arguments, operation, rejected string
	}{
		{name: "check_user_manual_and_all_managed_chapters", arguments: "check", operation: "check"},
		{name: "format_user_manual_and_all_managed_chapters", arguments: "format", operation: "generate"},
		{name: "default_mode_still_formats", operation: "generate"},
		{name: "verbose_check_preserves_selection", arguments: "check --verbose", operation: "check"},
		{name: "invalid_user_manual_fails_check", arguments: "check", operation: "check", rejected: "docs/TriceUserManual.md"},
		{name: "invalid_chapter_fails_check", arguments: "check", operation: "check", rejected: "docs/rm/binary encoding.md"},
		{name: "chapter_generation_failure_stops_formatting", arguments: "format", operation: "generate", rejected: "docs/rm/binary encoding.md"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := manualMaintenanceFixture(t)
			out, err := runFixture(t, root, "bash scripts/_310_refresh_trice_user_manual.sh "+tc.arguments, map[string]string{"REJECTED": tc.rejected})
			if tc.rejected != "" {
				assert.Error(t, err, out)
				assert.Contains(t, out, "invalid manual: "+tc.rejected)
			} else {
				assert.NoError(t, err, out)
			}
			calls, readErr := os.ReadFile(filepath.Join(root, "mdtoc.calls"))
			if !assert.NoError(t, readErr) {
				return
			}
			// Exact calls also prove navigation, drafts and unmanaged files are skipped
			// and that a chapter filename containing spaces remains one argument.
			var expected strings.Builder
			for _, path := range []string{"docs/TriceUserManual.md", "docs/rm/binary encoding.md", "docs/rm/overview.md", "docs/rm/rtt.md"} {
				expected.WriteString(tc.operation + " --file " + path + "\n")
				if path == tc.rejected {
					break
				}
			}
			assert.Equal(t, expected.String(), string(calls))
			if strings.Contains(tc.arguments, "--verbose") {
				assert.Contains(t, out, "Checking docs/rm/rtt.md TOC, numbering, and anchors with mdtoc...")
			}
		})
	}
}

// TestManualMaintenanceValidatesSourcesBeforeFormatting ensures missing or empty
// entry points and chapters fail before mdtoc can modify any otherwise valid file.
func TestManualMaintenanceValidatesSourcesBeforeFormatting(t *testing.T) {
	for _, tc := range []struct {
		name, path, message string
		empty               bool
	}{
		{name: "missing_user_manual", path: "docs/TriceUserManual.md", message: "manual file not found or empty: docs/TriceUserManual.md"},
		{name: "empty_user_manual", path: "docs/TriceUserManual.md", empty: true, message: "manual file not found or empty: docs/TriceUserManual.md"},
		{name: "missing_reference_directory", path: "docs/rm", message: "reference manual directory not found: docs/rm"},
		{name: "missing_reference_entry_point", path: "docs/rm/README.md", message: "manual file not found or empty: docs/rm/README.md"},
		{name: "empty_reference_entry_point", path: "docs/rm/README.md", empty: true, message: "manual file not found or empty: docs/rm/README.md"},
		{name: "empty_chapter_prevents_partial_format", path: "docs/rm/rtt.md", empty: true, message: "manual file not found or empty: docs/rm/rtt.md"},
		{name: "missing_linked_chapter_prevents_partial_format", path: "docs/rm/rtt.md", message: "manual file not found or empty: docs/rm/rtt.md"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := manualMaintenanceFixture(t)
			if tc.empty {
				writeFixture(t, root, tc.path, "")
			} else if !assert.NoError(t, os.RemoveAll(filepath.Join(root, tc.path))) {
				return
			}
			out, err := runFixture(t, root, "bash scripts/_310_refresh_trice_user_manual.sh format", nil)
			assert.Error(t, err, out)
			assert.Contains(t, out, tc.message)
			assert.NoFileExists(t, filepath.Join(root, "mdtoc.calls"), "preflight must finish before invoking mdtoc")
		})
	}
}

// TestManualMaintenanceRejectsIncompleteChapterLists checks omissions in both
// directions before generation can modify even the User Manual.
func TestManualMaintenanceRejectsIncompleteChapterLists(t *testing.T) {
	for _, tc := range []struct {
		name, path, content, message string
	}{
		{"linked_chapter_without_container", "docs/rm/rtt.md", "# RTT\n\n## Setup\n", "missing complete mdtoc container in reference chapter: docs/rm/rtt.md"},
		{"unclosed_container", "docs/rm/rtt.md", "# RTT\n\n<!-- mdtoc -->\n", "missing complete mdtoc container in reference chapter: docs/rm/rtt.md"},
		{"unlisted_managed_chapter", "docs/rm/new.md", "# New\n\n<!-- mdtoc -->\n<!-- /mdtoc -->\n", "mdtoc-managed chapter is not linked from docs/rm/README.md: docs/rm/new.md"},
		{"duplicate_link", "docs/rm/README.md", "# Reference\n\n- [RTT](rtt.md)\n- [RTT again](./rtt.md)\n", "duplicate chapter link in docs/rm/README.md: docs/rm/rtt.md"},
		{"nested_link", "docs/rm/README.md", "# Reference\n\n- [Draft](nested/draft.md)\n", "expected a flat RM chapter link in docs/rm/README.md: nested/draft.md"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := manualMaintenanceFixture(t)
			writeFixture(t, root, tc.path, tc.content)
			out, err := runFixture(t, root, "bash scripts/_310_refresh_trice_user_manual.sh format", nil)
			assert.Error(t, err, out)
			assert.Contains(t, out, tc.message)
			assert.NoFileExists(t, filepath.Join(root, "mdtoc.calls"))
		})
	}
}

// TestManualMaintenanceRequiresManagedChapters rejects unmatched globs and a
// directory containing only navigation or unmanaged files instead of real chapters.
func TestManualMaintenanceRequiresManagedChapters(t *testing.T) {
	for _, withNotes := range []bool{false, true} {
		name := "navigation_only"
		if withNotes {
			name = "navigation_and_unmanaged_notes"
		}
		t.Run(name, func(t *testing.T) {
			root := manualMaintenanceFixture(t)
			writeFixture(t, root, "docs/rm/README.md", "# Reference\n\n- [Introduction](HEADER.md)\n- [Footer](FOOTER.md)\n")
			for _, path := range []string{"docs/rm/overview.md", "docs/rm/binary encoding.md", "docs/rm/rtt.md"} {
				assert.NoError(t, os.Remove(filepath.Join(root, path)))
			}
			if !withNotes {
				assert.NoError(t, os.Remove(filepath.Join(root, "docs/rm/notes.md")))
			}
			out, err := runFixture(t, root, "bash scripts/_310_refresh_trice_user_manual.sh check", nil)
			assert.Error(t, err, out)
			assert.Contains(t, out, "no mdtoc-managed reference chapters found: docs/rm/*.md")
			assert.Contains(t, out, "expected <!-- mdtoc -->")
			assert.NoFileExists(t, filepath.Join(root, "mdtoc.calls"))
		})
	}
}

// TestManualPDFGenerationPreservesBookIdentity runs the real wrapper against a
// renderer double. Stale PDFs must never conceal missing inputs or failed output.
func TestManualPDFGenerationPreservesBookIdentity(t *testing.T) {
	for _, tc := range []struct {
		name, missing, failed, empty, stitcher, toolError string
	}{
		{"distinct_artifacts_and_headers_for_both_books", "", "", "", "", ""},
		{"missing_guide_preserves_existing_pdfs", "TriceUserManual", "", "", "", ""},
		{"missing_reference_preserves_existing_pdfs", "TriceReferenceManual", "", "", "", ""},
		{"guide_renderer_failure_is_visible", "", "TriceUserManual", "", "", ""},
		{"reference_renderer_failure_is_visible", "", "TriceReferenceManual", "", "", ""},
		{"empty_guide_output_is_not_success", "", "", "TriceUserManual", "", ""},
		{"empty_reference_output_is_not_success", "", "", "TriceReferenceManual", "", ""},
		{"pinned_go_install_dev_build", "", "", "", "pinned", ""},
		{"pinned_go_install_crlf_metadata", "", "", "", "crlf", ""},
		{"approved_missing_stitcher", "", "", "", "install_missing", ""},
		{"missing_stitcher", "", "", "", "missing", "stitchmd 0.9.0 is required; install with:"},
		{"wrong_release", "", "", "", "wrong_release", "stitchmd 0.9.0 is required for reproducible PDF assembly"},
		{"failed_version_command", "", "", "", "failed_version", "cannot read stitchmd version"},
		{"missing_go_for_dev_build", "", "", "", "missing_go", "Go is required to verify stitchmd dev"},
		{"failed_build_information", "", "", "", "failed_metadata", "cannot read stitchmd build information"},
		{"empty_build_information", "", "", "", "empty_metadata", "stitchmd dev must be built from"},
		{"unversioned_dev_build", "", "", "", "unversioned", "stitchmd dev must be built from"},
		{"wrong_module_version", "", "", "", "wrong_module", "stitchmd dev must be built from"},
		{"wrong_main_package", "", "", "", "wrong_package", "stitchmd dev must be built from"},
		{"pinned_dependency_is_not_the_main_module", "", "", "", "dependency", "stitchmd dev must be built from"},
		{"replaced_module_is_not_pinned", "", "", "", "replacement", "stitchmd dev must be built from"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_320_generate_trice_user_manual_pdf.sh", "scripts/_manual_pdf_config.cjs", "scripts/_prepare_reference_manual.cjs")
			writeFixture(t, root, "docs/ref/asset.txt", "fixture image directory\n")
			for _, manual := range []string{"TriceUserManual", "TriceReferenceManual"} {
				if manual != tc.missing {
					if manual == "TriceUserManual" {
						writeFixture(t, root, "docs/TriceUserManual.md", "# TriceUserManual\n")
					} else {
						writeFixture(t, root, "docs/rm/README.md", "# Reference\n\n- [Chapter](chapter.md)\n")
						writeFixture(t, root, "docs/rm/chapter.md", "# Chapter\n")
					}
				}
				writeFixture(t, root, "docs/"+manual+".pdf", "previous "+manual)
			}
			writeFixture(t, root, "bin/node", "#!/bin/sh\necho fixture-node\n")
			writeFixture(t, root, "bin/npm", "#!/bin/sh\necho fixture-npm\n")
			writeFixture(t, root, "bin/stitchmd", `#!/bin/sh
if [ "$1" = -version ]; then
  case "$STITCHMD_FIXTURE" in
    failed_version) echo 'version command failed'; exit 19 ;;
    wrong_release) echo 'stitchmd 0.8.0' ;;
    '') echo 'stitchmd 0.9.0' ;;
    *) echo 'stitchmd dev' ;;
  esac
  exit 0
fi
printf '# Reference\n' > temp/manual-pdf/TriceReferenceManual.md
`)
			// Metadata doubles model go install and reject nearby but unpinned
			// identities. The selected binary must be the one inspected by Go.
			writeFixture(t, root, "bin/go", `#!/bin/sh
[ "$1" != install ] || {
  printf '%s\n' "$2" > installed-package.txt
  cp bin/stitchmd "$GOBIN/stitchmd"
  exit 0
}
[ "$1" = version ] && [ "$2" = -m ] && [ "$3" = "$(command -v stitchmd)" ] || exit 93
case "$STITCHMD_FIXTURE" in
  failed_metadata) echo 'unreadable executable'; exit 18 ;;
  empty_metadata) exit 0 ;;
esac
package=go.abhg.dev/stitchmd
module=go.abhg.dev/stitchmd
version=v0.9.0
case "$STITCHMD_FIXTURE" in
  unversioned) version='(devel)' ;;
  wrong_module) version=v0.8.0 ;;
  wrong_package) package=example.com/other ;;
  dependency) package=example.com/other; module=example.com/other ;;
esac
{
  printf 'fixture: go1.26.1\n\tpath\t%s\n\tmod\t%s\t%s\th1:fixture\n' "$package" "$module" "$version"
  case "$STITCHMD_FIXTURE" in
    dependency) printf '\tdep\tgo.abhg.dev/stitchmd\tv0.9.0\th1:fixture\n' ;;
    replacement) printf '\t=>\t../modified-stitchmd\t(devel)\n' ;;
  esac
} | if [ "$STITCHMD_FIXTURE" = crlf ]; then awk '{printf "%s\r\n", $0}'; else cat; fi
`)
			writeFixture(t, root, "bin/npx", `#!/bin/sh
if [ "$1" = --version ]; then echo fixture-npx; exit 0; fi
case "$*" in
  *'node scripts/_prepare_reference_manual.cjs --verify'*) exit 0 ;;
  *'node scripts/_prepare_reference_manual.cjs'*)
    mkdir -p temp/manual-pdf
    printf '# Reference\n' > temp/manual-pdf/SUMMARY.md
    exit 0 ;;
esac
manual=
for argument do
  case "$argument" in *'
'*) echo 'fixture rejects multiline renderer arguments' >&2; exit 92 ;; esac
  case "$argument" in docs/*.md|temp/manual-pdf/*.md) manual="$argument" ;; esac
done
printf '%s\n' "$*" >> renderer-calls.txt
if [ "${manual##*/}" = "$FAILED.md" ]; then echo "fixture renderer failed: $manual" >&2; exit 23; fi
if [ "${manual##*/}" = "$EMPTY.md" ]; then exit 0; fi
printf 'rendered %s\n' "$manual" > "${manual%.md}.pdf"
`)
			command := "bash scripts/_320_generate_trice_user_manual_pdf.sh"
			if tc.stitcher == "missing" || tc.stitcher == "missing_go" {
				// Hide only the selected command; host utilities remain available
				// so an installed developer tool cannot mask a missing-tool case.
				hidden := "stitchmd"
				if tc.stitcher == "missing_go" {
					hidden = "go"
				}
				command = "command() { if [ \"$1\" = -v ] && [ \"$2\" = " + hidden + " ]; then return 1; fi; builtin command \"$@\"; }; source scripts/_320_generate_trice_user_manual_pdf.sh"
			}
			if tc.stitcher == "install_missing" {
				command = "printf 'yes\\n' | { command() { if [ \"$1\" = -v ] && [ \"$2\" = stitchmd ] && [ ! -f temp/tools/bin/stitchmd ]; then return 1; fi; builtin command \"$@\"; }; source scripts/_320_generate_trice_user_manual_pdf.sh; }"
			}
			out, err := runFixture(t, root, command, map[string]string{
				"FAILED": tc.failed, "EMPTY": tc.empty, "STITCHMD_FIXTURE": tc.stitcher, "CI": "false",
			})
			if tc.stitcher == "install_missing" {
				installed, readErr := os.ReadFile(filepath.Join(root, "installed-package.txt"))
				assert.NoError(t, readErr)
				assert.Equal(t, "go.abhg.dev/stitchmd@v0.9.0\n", string(installed))
			}
			if tc.missing != "" {
				assert.Error(t, err, out)
				missing := "docs/TriceUserManual.md"
				if tc.missing == "TriceReferenceManual" {
					missing = "docs/rm/README.md"
				}
				assert.Contains(t, out, "missing or empty manual file: "+missing)
				_, statErr := os.Stat(filepath.Join(root, "renderer-calls.txt"))
				assert.True(t, os.IsNotExist(statErr), "missing input must be detected before rendering")
				for _, manual := range []string{"TriceUserManual", "TriceReferenceManual"} {
					data, readErr := os.ReadFile(filepath.Join(root, "docs", manual+".pdf"))
					assert.NoError(t, readErr)
					assert.Equal(t, "previous "+manual, string(data))
				}
			} else if tc.toolError != "" {
				assert.Error(t, err, out)
				assert.Contains(t, out, tc.toolError)
				assert.Contains(t, out, "PDF generation context:")
				assert.NoFileExists(t, filepath.Join(root, "temp/manual-pdf/SUMMARY.md"), "unverified tools must fail before staging")
				assert.NoFileExists(t, filepath.Join(root, "renderer-calls.txt"))
				for _, manual := range []string{"TriceUserManual", "TriceReferenceManual"} {
					data, readErr := os.ReadFile(filepath.Join(root, "docs", manual+".pdf"))
					assert.NoError(t, readErr)
					assert.Equal(t, "previous "+manual, string(data))
				}
			} else if tc.failed != "" {
				assert.Error(t, err, out)
				assert.Contains(t, out, "fixture renderer failed:")
				assert.Contains(t, out, tc.failed+".md")
				assert.Contains(t, out, "ERROR: md-to-pdf failed")
				assert.NotContains(t, out, "Generated manual PDF: docs/"+tc.failed+".pdf")
			} else if tc.empty != "" {
				assert.Error(t, err, out)
				assert.Contains(t, out, "missing or empty generated PDF:")
				assert.Contains(t, out, tc.empty+".pdf")
			} else {
				assert.NoError(t, err, out)
				calls, readErr := os.ReadFile(filepath.Join(root, "renderer-calls.txt"))
				assert.NoError(t, readErr)
				assert.Contains(t, string(calls), "--config-file scripts/_manual_pdf_config.cjs", "the renderer must apply portable links and visible contents")
				for _, manual := range []string{"TriceUserManual", "TriceReferenceManual"} {
					data, readErr := os.ReadFile(filepath.Join(root, "docs", manual+".pdf"))
					assert.NoError(t, readErr)
					input := "docs/TriceUserManual.md"
					if manual == "TriceReferenceManual" {
						input = "temp/manual-pdf/TriceReferenceManual.md"
					}
					assert.Equal(t, "rendered "+input+"\n", string(data))
					assert.Contains(t, string(calls), "<span>"+manual+".md</span>", "each PDF must identify its own Markdown source")
				}
				assert.NoFileExists(t, filepath.Join(root, "docs/TriceReferenceManual.md"), "the generated monolith belongs only in temp")
			}
		})
	}
}

// TestReferencePDFPreparation exercises real Markdown tokenization, namespace
// mapping and staging cleanup without rendering PDFs or fetching npm packages.
// It runs inside the existing offline renderer environment when available.
func TestReferencePDFPreparation(t *testing.T) {
	if _, err := exec.LookPath("npx"); err != nil {
		t.Skip("npx not installed; reference preparation uses the PDF renderer runtime")
	}
	root := scriptFixture(t, "scripts/_prepare_reference_manual.cjs")
	writeFixture(t, root, "docs/rm/README.md", "# Reference\n\n- [Introduction](HEADER.md)\n- [Beta](b.md)\n- [Alpha](a.md)\n- [Footer](FOOTER.md)\n")
	writeFixture(t, root, "docs/rm/HEADER.md", "# Introduction\n\nBook introduction.\n")
	writeFixture(t, root, "docs/rm/FOOTER.md", "# Footer\n\nBook footer.\n")
	writeFixture(t, root, "docs/ref/image.png", "fixture image\n")
	writeFixture(t, root, "docs/rm/a.md", "# Alpha\n\n<details markdown=\"1\">\n<summary>Contents</summary>\n\n<!-- mdtoc -->\n\n- [Example](#example)\n\n<!-- /mdtoc -->\n\n</details>\n\n## 1. <a id=\"example\"></a>Example\n\n<!-- Preserve [fake](missing.md) and <a id=\"comment-only\"></a>. -->\n\n[Beta](b.md#example)\n\n![Image](../ref/image.png)\n\n[Code](../../src/example.c)\n\n```md\n[Do not rewrite](b.md#example)\n```\n")
	// Exercise deep nesting, sibling resets and a skipped heading level without
	// letting generated local numbers replace the README-ordered book hierarchy.
	writeFixture(t, root, "docs/rm/b.md", "# Beta\n\n## 1. <a id=\"example\"></a>Example\n\n<a href=\"a.md#example\">Alpha</a>\n\nCode with a blank line:\n\n```text\nfirst line\n\nlast line\n```\n"+`
### 1.1. <a id="detail"></a>Detail *formatting*

#### 1.1.1. <a id="nested"></a>Nested

##### Deep

###### Deepest

### 1.2. <a id="sibling"></a>Sibling

## 2. <a id="next"></a>Next

#### Skipped level

## 2.0 protocol

### 32-bit packets

<!--
## 99. Commented heading
-->

`)
	writeFixture(t, root, "temp/manual-pdf/stale.txt", "must be removed\n")
	writeFixture(t, root, "temp/unrelated.txt", "must survive\n")
	writeFixture(t, root, "check.cjs", `
const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const helper = require('./scripts/_prepare_reference_manual.cjs');
let marked;
for (const directory of process.env.PATH.split(path.delimiter)) {
  try {
    const entry = require.resolve('md-to-pdf', {paths:[path.resolve(directory,'..')]});
    marked = require(path.join(path.dirname(entry),'lib/get-marked-with-highlighter.js')).getMarked({},[]);
    break;
  } catch (error) { if(error.code !== 'MODULE_NOT_FOUND') throw error; }
}
assert.ok(marked, 'the pinned renderer must supply its parser');
const before = ['docs/rm/README.md','docs/rm/HEADER.md','docs/rm/a.md','docs/rm/b.md','docs/rm/FOOTER.md'].map(file => [file,fs.readFileSync(file)]);
helper.prepare(marked, process.cwd());
assert.equal(fs.existsSync('temp/manual-pdf/stale.txt'), false);
assert.equal(fs.readFileSync('temp/unrelated.txt','utf8'), 'must survive\n');
const summary = fs.readFileSync('temp/manual-pdf/SUMMARY.md','utf8');
assert.ok(summary.indexOf('[1. Beta]') < summary.indexOf('[2. Alpha]'), 'README order controls book numbering');
assert.ok(summary.includes('[Introduction]') && summary.includes('[Footer]'), 'framing documents stay unnumbered');
const contents = fs.readFileSync('temp/manual-pdf/_contents.md','utf8');
assert.ok(contents.includes('>1. Beta</a>') && contents.includes('>2. Alpha</a>'), 'global contents use the same chapter numbers');
assert.ok(contents.includes('>1.1. Example</a>') && contents.includes('>1.1.1. Detail formatting</a>'), 'contents use full book subsection numbers');
assert.ok(contents.includes('>2.1. Example</a>'), 'subsection counters restart under the next main chapter');
const alpha = fs.readFileSync('temp/manual-pdf/chapters/a.md','utf8');
assert.ok(alpha.startsWith('# <a id="rm-a--chapter" class="rm-main-chapter"></a>2. Alpha\n'), 'chapter headings carry PDF-only numbers and page-break hooks');
assert.ok(!alpha.includes('<summary>Contents</summary>'), 'local contents are removed');
assert.ok(alpha.includes('href="#rm-b--anchor-example"'), 'chapter links become PDF destinations');
assert.ok(alpha.includes('src="../../../docs/ref/image.png"'), 'images remain local');
assert.match(alpha, /https:\/\/github.com\/rokath\/trice\/blob\/[^/]+\/src\/example.c/);
assert.ok(alpha.includes('<!-- Preserve [fake](missing.md) and <a id="comment-only"></a>. -->'));
assert.ok(alpha.replace(/<[^>]+>/g,'').includes('[Do not rewrite](b.md#example)'), 'highlighted code example text is preserved');
const beta = fs.readFileSync('temp/manual-pdf/chapters/b.md','utf8');
const headingLabels = [...beta.matchAll(/<h[3-6]\b[^>]*>(.*?)<\/h[3-6]>/gs)].map(match => match[1].replace(/<[^>]*>/g,''));
assert.deepEqual(headingLabels, [
  '1.1. Example',
  '1.1.1. Detail formatting',
  '1.1.1.1. Nested',
  '1.1.1.1.1. Deep',
  '1.1.1.1.1.1. Deepest',
  '1.1.2. Sibling',
  '1.2. Next',
  '1.2.1. Skipped level',
  '1.3. 2.0 protocol',
  '1.3.1. 32-bit packets',
], 'all levels follow the visible hierarchy, reset siblings and preserve literal title numbers');
assert.ok(beta.includes('<em>formatting</em>'), 'numbering retains inline heading markup');
assert.ok(beta.includes('## 99. Commented heading'), 'numbering must not rewrite commented headings');
assert.ok(!contents.includes('Commented heading'), 'comments must not enter the hierarchy');
assert.ok(beta.includes('href="#rm-a--anchor-example"'), 'HTML links are remapped too');
assert.ok(alpha.includes('id="rm-a--anchor-example"') && beta.includes('id="rm-b--anchor-example"'));
for(const [file,data] of before) assert.deepEqual(fs.readFileSync(file),data,'sources must stay untouched');
// Group labels affect global contents only; ungrouped sources above keep working.
fs.writeFileSync('docs/rm/README.md', '# Reference\n\n- [Introduction](HEADER.md)\n\n## Normal use\n\n- [Beta](b.md)\n\n## Advanced features\n\n- [Alpha](a.md)\n\n## Empty group\n\n## Appendices\n\n- [Appendix](c.md)\n- [Footer](FOOTER.md)\n');
fs.writeFileSync('docs/rm/c.md', '# Appendix\n\nAppendix text.\n');
helper.prepare(marked, process.cwd());
const groupedContents = fs.readFileSync('temp/manual-pdf/_contents.md','utf8');
const groups = [...groupedContents.matchAll(/class="toc-group"[^>]*><strong>(.*?)<\/strong>/g)].map(match => match[1]);
assert.deepEqual(groups, ['Normal use','Advanced features','Appendices'], 'only groups with chapters appear, in README order');
for (const [group, chapter] of [['Normal use','1. Beta'],['Advanced features','2. Alpha'],['Appendices','3. Appendix']]) {
  assert.ok(groupedContents.indexOf('>'+group+'</strong>') < groupedContents.indexOf('>'+chapter+'</a>'), 'each group precedes its chapters');
}
assert.equal(fs.readFileSync('temp/manual-pdf/chapters/a.md','utf8'), alpha, 'grouping preserves chapter content and numbering');
assert.equal(fs.readFileSync('temp/manual-pdf/chapters/b.md','utf8'), beta, 'grouping preserves existing subsection numbers');
const groupedSummary = fs.readFileSync('temp/manual-pdf/SUMMARY.md','utf8');
assert.ok(groupedSummary.includes('[3. Appendix]') && !groupedSummary.includes('Normal use') && !groupedSummary.includes('Appendices'), 'groups add no book chapters or divider pages');
fs.writeFileSync('temp/manual-pdf/TriceReferenceManual.md', '# <a id="rm-start"></a>Reference\n\n' + beta + '\n\n' + alpha);
helper.verify(marked,process.cwd());
const compiled = fs.readFileSync('temp/manual-pdf/TriceReferenceManual.html','utf8');
assert.ok(compiled.includes('first line\n\nlast line\n</code></pre>'), 'blank code lines survive final Markdown parsing');
assert.ok(compiled.includes('1.1.1.1.1.1. Deepest') && compiled.includes('2.1. '), 'final parsing preserves book numbering');
fs.writeFileSync('temp/manual-pdf/TriceReferenceManual.md','[Broken](#missing)\n');
assert.throws(() => helper.verify(marked,process.cwd()), /missing generated PDF anchor/);
fs.writeFileSync('docs/rm/b.md','# Beta\n\n[Broken](a.md#missing)\n');
assert.throws(() => helper.prepare(marked,process.cwd()), /unknown reference destination/);
fs.writeFileSync('docs/rm/b.md','# Beta\n\n![Broken](missing.png)\n');
assert.throws(() => helper.prepare(marked,process.cwd()), /missing reference image/);
fs.rmSync('docs/rm/b.md');
assert.throws(() => helper.prepare(marked,process.cwd()), /missing or empty reference source/);
console.log('PASS: README order, cleanup, sources, code, comments, images, anchors and rejected broken inputs');
`)
	out, err := runFixture(t, root, "npx --offline -y --package=md-to-pdf@5.2.4 --package=puppeteer@24.31.0 -c 'node check.cjs'", nil)
	if err != nil && (strings.Contains(out, "ENOTCACHED") || strings.Contains(out, "cache mode is 'only-if-cached'")) {
		t.Skip("pinned PDF renderer is not cached; this test never downloads npm packages")
	}
	assert.NoError(t, err, out)
	assert.Contains(t, out, "PASS:")
}

// workflowShellStep extracts one named run block without executing GitHub jobs.
// The strict indentation match fails visibly if the workflow layout changes.
func workflowShellStep(t *testing.T, workflow, step string) string {
	t.Helper()
	data, err := os.ReadFile(filepath.Join("..", ".github", "workflows", workflow))
	if !assert.NoError(t, err) {
		t.FailNow()
	}
	pattern := `(?m)^      - name: ` + regexp.QuoteMeta(step) + `\n(?:        [^\n]*\n)*?        run: \|\n((?:          [^\n]*\n|\n)+)`
	match := regexp.MustCompile(pattern).FindStringSubmatch(string(data))
	if !assert.Len(t, match, 2, "workflow must contain the shell step %q", step) {
		t.FailNow()
	}
	return strings.ReplaceAll(match[1], "\n          ", "\n")[10:]
}

// runPagesFixture keeps the actual workflow command while supplying the same
// verified interpreter used by download tests, including Windows' py -3.
func runPagesFixture(t *testing.T, root string) (string, error) {
	t.Helper()
	python := python3Command(t)
	quoted := make([]string, len(python))
	for index, arg := range python {
		quoted[index] = "'" + strings.ReplaceAll(filepath.ToSlash(arg), "'", "'\"'\"'") + "'"
	}
	writeFixture(t, root, "bin/python3", "#!/bin/sh\nexec "+strings.Join(quoted, " ")+" \"$@\"\n")
	return runFixture(t, root, workflowShellStep(t, "pages.yml", "Prepare documentation for GitHub Pages"), nil)
}

// TestManualPagesLinksKeepReadersOnTheSite executes the actual workflow step
// on tiny documents. Source links and images must retain their distinct roles.
func TestManualPagesLinksKeepReadersOnTheSite(t *testing.T) {
	for _, tc := range []struct{ name, newline string }{{"LF", "\n"}, {"CRLF", "\r\n"}} {
		t.Run(tc.name, func(t *testing.T) {
			checkManualPagesLinks(t, tc.newline)
		})
	}
}

// checkManualPagesLinks exercises the same real workflow with both authored
// newline styles; link rewriting must preserve code, comments and UTF-8 bytes.
func checkManualPagesLinks(t *testing.T, newline string) {
	t.Helper()
	lineEndings := func(text string) string { return strings.ReplaceAll(text, "\n", newline) }
	root := scriptFixture(t, "scripts/_prepare_pages.py", "index.md", "_config.yml")
	writeFixture(t, root, "README.md", "[Guide](./docs/TriceUserManual.md#see-your-first-log)\n[Reference](./docs/rm/README.md)\n[Speed](docs/rm/performance.md)\n")
	writeFixture(t, root, "docs/ref/example.png", "image bytes")
	writeFixture(t, root, "docs/ref/example.gif", "animation bytes")
	writeFixture(t, root, "docs/README.md", lineEndings("[Guide](TriceGuide.md#entry-points)\n"))
	for _, manual := range []string{"TriceUserManual", "TriceGuide"} {
		writeFixture(t, root, "docs/"+manual+".md", lineEndings("[Guide](./TriceUserManual.md#see-your-first-log)\n[Reference](./rm/bind.md#bind-limits)\n[Code](../src/trice.h)\n[Help](./ref/trice-help-all.txt)\n![Image](./ref/example.png)\n[Local](#local)\n"))
	}
	writeFixture(t, root, "docs/rm/README.md", lineEndings("[Bind](bind.md#bind-limits)\n"))
	// Nested chapters resolve parent documentation and source files differently.
	// Image previews, commented tests and code examples must survive unchanged.
	chapter := "[User](../TriceUserManual.md#see-your-first-log)\n[Code](../../src/trice.h)\n[`Header`](../../src/trice.h)\n[Help](../ref/trice-help-all.txt)\n![Image](../ref/example.png)\n[![Preview](../ref/example.png)](../ref/example.gif)\n[Local](#local)\n[External](https://example.com/guide?q=1#chapter)\n<a href=\"../TriceGuide.md#entry-points\">Guide</a>\n```md\n[Example](../../src/trice.h)\n```\n<!--\n[Comment](../../src/trice.h)\n-->\n`[Inline](../../src/trice.h)`\n"
	chapter = lineEndings(chapter + "UTF-8: café, µs.\n")
	writeFixture(t, root, "docs/rm/bind.md", chapter)
	writeFixture(t, root, "docs/rm/performance.md", lineEndings("# Trice Performance\n"))
	out, err := runPagesFixture(t, root)
	assert.NoError(t, err, out)
	site := filepath.Join(root, "temp", "pages-source")
	index, err := os.ReadFile(filepath.Join(site, "README.md"))
	assert.NoError(t, err)
	assert.Contains(t, string(index), "./docs/TriceUserManual.html#see-your-first-log")
	assert.Contains(t, string(index), "./docs/rm/index.html")
	assert.Contains(t, string(index), "docs/rm/performance.html")
	for _, manual := range []string{"TriceUserManual", "TriceGuide"} {
		data, err := os.ReadFile(filepath.Join(site, "docs", manual+".md"))
		assert.NoError(t, err)
		assert.Contains(t, string(data), "[Guide](./TriceUserManual.html#see-your-first-log)")
		assert.Contains(t, string(data), "[Reference](./rm/bind.html#bind-limits)")
		assert.Contains(t, string(data), "[Code](https://github.com/rokath/trice/blob/main/src/trice.h)")
		assert.Contains(t, string(data), "[Help](https://github.com/rokath/trice/blob/main/docs/ref/trice-help-all.txt)")
		assert.Contains(t, string(data), "![Image](./ref/example.png)")
		assert.Contains(t, string(data), "[Local](#local)")
	}
	data, err := os.ReadFile(filepath.Join(site, "docs/rm/bind.md"))
	assert.NoError(t, err)
	for _, expected := range []string{
		"[User](../TriceUserManual.html#see-your-first-log)",
		"[Code](https://github.com/rokath/trice/blob/main/src/trice.h)",
		"[`Header`](https://github.com/rokath/trice/blob/main/src/trice.h)",
		"[Help](https://github.com/rokath/trice/blob/main/docs/ref/trice-help-all.txt)",
		"![Image](../ref/example.png)",
		"[![Preview](../ref/example.png)](../ref/example.gif)",
		"[Local](#local)",
		"[External](https://example.com/guide?q=1#chapter)",
		"<a href=\"../TriceGuide.html#entry-points\">Guide</a>",
		"```md\n[Example](../../src/trice.h)\n```",
		"<!--\n[Comment](../../src/trice.h)\n-->",
		"`[Inline](../../src/trice.h)`",
		"UTF-8: café, µs.",
	} {
		assert.Contains(t, string(data), lineEndings(expected))
	}
	assert.Equal(t, strings.Count(chapter, "\n"), strings.Count(string(data), "\n"), "link rewriting must retain line boundaries")
	assert.Equal(t, strings.Count(chapter, "\r\n"), strings.Count(string(data), "\r\n"), "authored line endings must survive on every host")
	assert.NoFileExists(t, filepath.Join(root, "docs/TriceReferenceManual.md"))
	original, err := os.ReadFile(filepath.Join(root, "docs/rm/bind.md"))
	assert.NoError(t, err)
	assert.Equal(t, chapter, string(original), "the checkout remains authored Markdown")
	for _, path := range []string{"index.md", "_config.yml", "docs/ref/example.png", "docs/ref/example.gif"} {
		source, err := os.ReadFile(filepath.Join(root, path))
		assert.NoError(t, err)
		staged, err := os.ReadFile(filepath.Join(site, path))
		assert.NoError(t, err)
		assert.Equal(t, source, staged, path)
	}
}

// TestManualPagesRequiresActiveSources verifies missing inputs are reported
// before the workflow can partially rewrite its remaining documentation.
func TestManualPagesRequiresActiveSources(t *testing.T) {
	paths := []string{"index.md", "_config.yml", "README.md", "docs/README.md", "docs/TriceUserManual.md", "docs/TriceGuide.md", "docs/rm/README.md"}
	for _, missing := range paths {
		t.Run(missing, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_prepare_pages.py")
			for _, path := range paths {
				if path != missing {
					writeFixture(t, root, path, "[Reference](./rm/bind.md#bind-limits)\n")
				}
			}
			// Even byte-sensitive input must survive a failed preflight unchanged.
			chapter := "\xef\xbb\xbf[User](../TriceUserManual.md)\r\n"
			writeFixture(t, root, "docs/rm/bind.md", chapter)
			out, err := runPagesFixture(t, root)
			assert.Error(t, err, out)
			assert.Contains(t, out, "ERROR: missing or empty Pages source: "+missing)
			data, err := os.ReadFile(filepath.Join(root, "docs/rm/bind.md"))
			assert.NoError(t, err)
			assert.Equal(t, chapter, string(data), "preflight failure must not rewrite surviving chapters")
		})
	}
}

// TestManualPagesPublicationScope checks observable staging boundaries and links,
// including repeat builds after a resource disappears from the documentation.
func TestManualPagesPublicationScope(t *testing.T) {
	root := scriptFixture(t, "scripts/_prepare_pages.py", "index.md", "_config.yml")
	for _, path := range []string{"README.md", "docs/README.md", "docs/TriceUserManual.md", "docs/TriceGuide.md", "docs/rm/README.md"} {
		writeFixture(t, root, path, "# Documentation\n")
	}
	readme := "[![License](https://example.com/badge.svg)](LICENSE.md)\n[![Animation](docs/ref/preview.png)](docs/ref/demo.gif)\n<img src=\"docs/ref/space%20image.png\" alt=\"Image\">\n[Manual](https://github.com/rokath/trice/blob/main/docs/TriceUserManual.md?q=1#start)\n[Download](third_party/package.zip)\n[Archive](docs/obsolete/old.md)\n"
	writeFixture(t, root, "README.md", readme)
	writeFixture(t, root, "docs/rm/templates.md", "# Templates\n\n`{{name}}` and `{{1,2}}`\n```text\n{% endraw %}\n```\n")
	for _, path := range []string{"docs/ref/preview.png", "docs/ref/demo.gif", "docs/ref/space image.png", "docs/ref/unused.png", "third_party/package.zip", "src/trice.h", "docs/obsolete/old.md", "docs/_scratchPad/note.md"} {
		writeFixture(t, root, path, "original bytes")
	}
	out, err := runPagesFixture(t, root)
	assert.NoError(t, err, out)
	site := filepath.Join(root, "temp/pages-source")
	data, err := os.ReadFile(filepath.Join(site, "README.md"))
	assert.NoError(t, err)
	assert.Contains(t, string(data), "](https://github.com/rokath/trice/blob/main/LICENSE.md)")
	assert.Contains(t, string(data), "[Manual](docs/TriceUserManual.html?q=1#start)")
	assert.Contains(t, string(data), "[Download](https://github.com/rokath/trice/blob/main/third_party/package.zip)")
	assert.Contains(t, string(data), "[Archive](https://github.com/rokath/trice/blob/main/docs/obsolete/old.md)")
	templates, err := os.ReadFile(filepath.Join(site, "docs/rm/templates.md"))
	assert.NoError(t, err)
	assert.Contains(t, string(templates), "`{% raw %}{{{% endraw %}name}}` and `{% raw %}{{{% endraw %}1,2}}`")
	assert.Contains(t, string(templates), "```text\n{% raw %}{%{% endraw %} endraw %}\n```", "even literal Liquid delimiters remain documentation")
	for _, path := range []string{"docs/ref/preview.png", "docs/ref/demo.gif", "docs/ref/space image.png"} {
		copied, err := os.ReadFile(filepath.Join(site, path))
		assert.NoError(t, err)
		assert.Equal(t, "original bytes", string(copied), "referenced image bytes must survive")
	}
	for _, path := range []string{"docs/ref/unused.png", "third_party/package.zip", "src/trice.h", "docs/obsolete/old.md", "docs/_scratchPad/note.md", "scripts/_prepare_pages.py"} {
		assert.NoFileExists(t, filepath.Join(site, path), "unselected files must not be published")
	}
	writeFixture(t, root, "README.md", "# Updated homepage\n")
	out, err = runPagesFixture(t, root)
	assert.NoError(t, err, out)
	assert.NoFileExists(t, filepath.Join(site, "docs/ref/demo.gif"), "a repeat build removes stale staged resources")
	assert.FileExists(t, filepath.Join(root, "docs/ref/demo.gif"), "source assets are never deleted")
}

// TestManualPagesRejectsBrokenResources requires a useful diagnostic and keeps
// the previous staging tree intact when a new input cannot be published.
func TestManualPagesRejectsBrokenResources(t *testing.T) {
	for _, tc := range []struct{ name, link, resource, diagnostic string }{
		{"missing_image", "![Missing](docs/ref/missing.png)", "", "missing or empty Pages source: docs/ref/missing.png"},
		{"empty_image", "![Empty](docs/ref/empty.png)", "docs/ref/empty.png", "missing or empty Pages source: docs/ref/empty.png"},
		{"escaping_link", "[Outside](../outside.md)", "", "link escapes repository in README.md"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_prepare_pages.py", "index.md", "_config.yml")
			for _, path := range []string{"docs/README.md", "docs/TriceUserManual.md", "docs/TriceGuide.md", "docs/rm/README.md"} {
				writeFixture(t, root, path, "# Documentation\n")
			}
			writeFixture(t, root, "README.md", tc.link)
			writeFixture(t, root, "temp/pages-source/previous.txt", "previous build")
			if tc.resource != "" {
				writeFixture(t, root, tc.resource, "")
			}
			out, err := runPagesFixture(t, root)
			assert.Error(t, err, out)
			assert.Contains(t, out, tc.diagnostic)
			previous, err := os.ReadFile(filepath.Join(root, "temp/pages-source/previous.txt"))
			assert.NoError(t, err)
			assert.Equal(t, "previous build", string(previous))
		})
	}
}

// TestManualPDFNavigationOutlivesTheRenderer executes the actual browser script
// on representative links. PDFs must not retain a dead localhost URL or hide TOCs.
func TestManualPDFNavigationOutlivesTheRenderer(t *testing.T) {
	node, err := exec.LookPath("node")
	if err != nil {
		t.Skip("node not installed; PDF navigation requires the PDF renderer's runtime")
	}
	root := scriptFixture(t, "scripts/_manual_pdf_config.cjs")
	writeFixture(t, root, "check.cjs", `
const assert = require('node:assert/strict');
const vm = require('node:vm');
const childProcess = require('node:child_process');
const path = require('node:path');
const configPath = require.resolve('./scripts/_manual_pdf_config.cjs');
const originalExecFileSync = childProcess.execFileSync;
// Exercise a reproducible checkout and an exported tree without requiring Git
// metadata in the fixture or launching platform-specific executable doubles.
for (const ref of ['0123456789abcdef0123456789abcdef01234567', 'main']) {
  delete require.cache[configPath];
  childProcess.execFileSync = (command, args, options) => {
    assert.equal(command, 'git');
    assert.deepEqual(args, ['rev-parse', 'HEAD']);
    assert.equal(options.cwd, path.resolve(path.dirname(configPath), '..'));
    if (ref === 'main') throw new Error('no Git metadata in exported sources');
    return ref + '\n';
  };
  let config;
  try {
    config = require(configPath);
  } finally {
    childProcess.execFileSync = originalExecFileSync;
  }
const repositoryBase = 'https://github.com/rokath/trice/blob/' + ref;
const cases = [
  ['#get-the-tools', '#get-the-tools'],
  ['#chapter-start', '#manual-start', 'back to top'],
  ['#chapter-section', '#manual-start', '  BACK to TOP  '],
  ['./TriceUserManual.md#see-your-first-log', '#see-your-first-log'],
  ['./TriceUserManual.md#chapter-start', '#manual-start', 'go to top'],
  ['./rm/bind.md#bind-limits', repositoryBase + '/docs/rm/bind.md#bind-limits'],
  ['../examples/PC_features/main.c', repositoryBase + '/examples/PC_features/main.c'],
  ['https://example.com/guide?q=1#chapter', 'https://example.com/guide?q=1#chapter'],
  ['https://example.com/guide#top', 'https://example.com/guide#top', 'back to top'],
  ['mailto:maintainer@example.com', 'mailto:maintainer@example.com'],
];
const links = cases.map(([href, expected, text]) => ({
  href,
  textContent: text || '',
  getAttribute() { return this.href; },
  setAttribute(name, value) { assert.equal(name, 'href'); this.href = value; },
}));
const details = [{open:false}];
const document = {
  title: 'temporary renderer address',
  location: new URL('http://localhost:12345/docs/TriceUserManual.md'),
  querySelector(selector) {
    assert.equal(selector, 'h1[id]');
    return {id:'manual-start', textContent:'  Trice User Manual  '};
  },
  querySelectorAll(selector) {
    if (selector === 'details') return details;
    assert.equal(selector, 'a[href]', 'image sources must not be rewritten');
    return links;
  },
};
for (const script of config.script) vm.runInNewContext(script.content, {document, URL});
cases.forEach(([before, expected], index) => assert.equal(links[index].href, expected, before));
assert.equal(details[0].open, true, 'the printed contents must be visible');
assert.equal(document.title, 'Trice User Manual', 'PDF metadata must identify the book rather than localhost');
document.querySelector = () => null;
document.title = 'Existing book title';
links[1].href = '#chapter-start';
for (const script of config.script) vm.runInNewContext(script.content, {document, URL});
assert.equal(links[1].href, '#chapter-start', 'an absent book heading must not create a broken destination');
assert.equal(document.title, 'Existing book title', 'an absent book heading must preserve the existing title');
}
console.log('PASS: local destinations, repository links, external URLs and printed contents');
`)
	command := exec.Command(node, "check.cjs")
	command.Dir = root
	out, err := command.CombinedOutput()
	assert.NoError(t, err, string(out))
	assert.Contains(t, string(out), "PASS:")
}

// TestReleasePDFChecksRequireEachBook executes both CI verification steps with
// a deliberately smaller guide and exercises missing and truncated artifacts.
func TestReleasePDFChecksRequireEachBook(t *testing.T) {
	for _, workflow := range []string{"install-checks.yml", "release-audit.yml"} {
		for _, tc := range []struct {
			name, damaged string
			size          int
		}{
			{"both_books_present", "", 0},
			{"missing_user_manual", "TriceUserManual", -1},
			{"missing_reference_manual", "TriceReferenceManual", -1},
			{"empty_user_manual", "TriceUserManual", 0},
			{"truncated_reference_manual", "TriceReferenceManual", 100},
		} {
			t.Run(workflow+"/"+tc.name, func(t *testing.T) {
				root := t.TempDir()
				for _, manual := range []string{"TriceUserManual", "TriceReferenceManual"} {
					size := 100001
					if manual == "TriceUserManual" {
						size = 10001
					}
					if manual == tc.damaged {
						size = tc.size
					}
					if size >= 0 {
						writeFixture(t, root, "artifacts/"+manual+".pdf", strings.Repeat("x", size))
					}
				}
				out, err := runFixture(t, root, workflowShellStep(t, workflow, "Verify release PDF"), nil)
				if tc.damaged == "" {
					assert.NoError(t, err, out)
					assert.Contains(t, out, "TriceUserManual.pdf")
					assert.Contains(t, out, "TriceReferenceManual.pdf")
				} else {
					assert.Error(t, err, out)
					assert.Contains(t, out, "::error::")
					assert.Contains(t, out, tc.damaged+".pdf")
				}
			})
		}
	}
}

// TestClangFormatSkipsDeletedPaths exercises Git's unstaged-deletion behavior
// and verifies both temporary lists are removed, including an empty selection.
func TestClangFormatSkipsDeletedPaths(t *testing.T) {
	for _, mode := range []string{"check", "format"} {
		for _, present := range []bool{true, false} {
			t.Run(mode+"/present="+strconv.FormatBool(present), func(t *testing.T) {
				root := scriptFixture(t, "scripts/_280_format_c_code.sh")
				writeFixture(t, root, "bin/git", "#!/bin/sh\nprintf '%s\\n' 'active file.c' removed.c\n")
				writeFixture(t, root, "bin/formatter", `#!/bin/sh
if [ "$1" = --version ]; then echo 'clang-format version 23.1.2'; exit 0; fi
for arg; do
  case "$arg" in
    --files=*)
      while IFS= read -r file; do
        [ -f "$file" ] || { echo "missing source: $file"; exit 19; }
        printf '%s\n' "$file" >> formatter.calls
      done < "${arg#--files=}"
      ;;
  esac
done
`)
				if present {
					writeFixture(t, root, "active file.c", "int value;\n")
				}
				tmp := filepath.Join(root, "owned-temp")
				assert.NoError(t, os.Mkdir(tmp, 0755))
				out, err := runFixture(t, root, "bash scripts/_280_format_c_code.sh "+mode, map[string]string{"CLANG_FORMAT_BIN": "./bin/formatter", "TMPDIR": tmp})
				assert.NoError(t, err, out)
				if present {
					calls, readErr := os.ReadFile(filepath.Join(root, "formatter.calls"))
					assert.NoError(t, readErr)
					assert.Equal(t, "active file.c\n", string(calls))
				} else {
					assert.NoFileExists(t, filepath.Join(root, "formatter.calls"))
				}
				entries, readErr := os.ReadDir(tmp)
				assert.NoError(t, readErr)
				assert.Empty(t, entries, "formatter-owned temporary files must be removed")
			})
		}
	}
}

// TestClangFormatCompatibility ensures only the canonical version can report a
// formatting regression. An override, missing executable or newer/older version
// never rewrites source files, and real canonical formatting errors still fail.
func TestClangFormatCompatibility(t *testing.T) {
	for _, tc := range []struct {
		version, replacements string
		failure               bool
	}{
		{"19.1.7", "1", false}, {"23.1.20", "1", false}, {"24.1.0", "1", false}, {"unknown", "1", false},
		{"23.1.2", "0", false}, {"23.1.2", "1", true}, {"missing", "0", false},
	} {
		t.Run(tc.version+"_"+tc.replacements, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_430_test_clang_format.sh", "scripts/_280_format_c_code.sh")
			writeFixture(t, root, "bin/git", "#!/bin/sh\nprintf 'sample.c\\n'\n")
			writeFixture(t, root, "bin/go", "#!/bin/sh\necho unexpected-go; exit 99\n")
			writeFixture(t, root, "bin/formatter", `#!/bin/sh
if [ "$1" = --version ]; then
  echo "clang-format version $VERSION"
elif [ "$1" = --dry-run ] && [ "$REPLACEMENTS" = 1 ]; then
  echo 'sample.c:1:10: error: code should be clang-formatted'
  exit 1
fi
`)
			writeFixture(t, root, "sample.c", "int main(void){return 0;}\n")
			formatter := "./bin/formatter"
			if tc.version == "missing" {
				formatter = "./bin/not-installed"
			}
			out, err := runFixture(t, root, "bash scripts/_430_test_clang_format.sh", map[string]string{"CLANG_FORMAT_BIN": formatter, "VERSION": tc.version, "REPLACEMENTS": tc.replacements})
			assert.Equal(t, tc.failure, err != nil, out)
			assert.NotContains(t, out, "unexpected-go")
			if tc.version != "23.1.2" {
				assert.Contains(t, out, "SKIP:")
			}
			if tc.failure {
				assert.Contains(t, out, "sample.c:1:10: error:")
				assert.Contains(t, out, "::error::")
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
	writeFixture(t, root, "bin/go", "#!/bin/sh\necho unexpected-go; exit 99\n")
	writeFixture(t, root, "bin/uname", "#!/bin/sh\nif [ \"$1\" = -s ]; then echo \"$PLATFORM\"; else echo x86_64; fi\n")
	writeFixture(t, root, "bin/clang-format", "#!/bin/sh\nif [ \"$1\" = --version ]; then echo 'clang-format version 19.1.3'; else echo wrong-system-formatter; exit 29; fi\n")
	writeFixture(t, root, "sample.c", "int value=1;\n")
	writeFixture(t, root, "seed/formatter", `#!/bin/sh
if [ "$1" = --version ]; then
  echo "clang-format version ${INSTALLED_VERSION:-23.1.2}"
else
  if [ "$1" = -i ]; then echo 'int value = 1;' > sample.c; fi
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
		"PLATFORM": platform, "CLANG_FORMAT_BIN": "",
		"VENV_BIN": "bin", "VENV_PYTHON": "python", "FORMATTER_NAME": "clang-format",
		"SETUP_LOG": filepath.Join(root, "setup.log"), "SEED_PYTHON": filepath.Join(root, "seed/venv-python"),
		"SEED_FORMATTER": filepath.Join(root, "seed/formatter"),
	}
	if strings.HasPrefix(platform, "MINGW") || strings.HasPrefix(platform, "MSYS") || strings.HasPrefix(platform, "CYGWIN") {
		env["VENV_BIN"], env["VENV_PYTHON"], env["FORMATTER_NAME"] = "Scripts", "python.exe", "clang-format.exe"
	}
	return root, env
}

// TestClangFormatAutomaticSetup checks first-run installation, native Windows
// layout, offline cache reuse and honoring an explicit user tool selection.
func TestClangFormatAutomaticSetup(t *testing.T) {
	for _, platform := range []string{"Darwin", "Linux", "MINGW64_NT-10.0-26300", "MSYS_NT-10.0", "CYGWIN_NT-10.0"} {
		t.Run(platform, func(t *testing.T) {
			root, env := clangBootstrapFixture(t, platform)
			out, err := runFixture(t, root, "bash scripts/_280_format_c_code.sh format", env)
			assert.NoError(t, err, out)
			assert.Contains(t, out, "Using clang-format version 23.1.2")
			data, err := os.ReadFile(filepath.Join(root, "sample.c"))
			assert.NoError(t, err)
			assert.Equal(t, "int value = 1;\n", string(data))
			setup, err := os.ReadFile(filepath.Join(root, "setup.log"))
			assert.NoError(t, err)
			assert.Equal(t, 1, strings.Count(string(setup), "pip -m pip install "))
			assert.Contains(t, string(setup), "clang-format==23.1.2")
			assert.Contains(t, string(setup), "--only-binary=:all:")
			assert.Contains(t, string(setup), "--no-input")
			if env["VENV_BIN"] == "Scripts" {
				assert.Contains(t, string(setup), "windows-x86_64")
				// A Windows update must reuse the cache despite a new uname suffix.
				env["PLATFORM"] = "MINGW64_NT-10.0-99999"
			}

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

// TestClangFormatWindowsWithoutPython exercises the verified native wheel path,
// failure boundaries, CRLF version output, architectures and offline cache reuse.
func TestClangFormatWindowsWithoutPython(t *testing.T) {
	for _, machine := range []string{"x86_64", "aarch64"} {
		for _, failure := range []string{"", "download", "checksum", "extract", "version"} {
			t.Run(machine+"/failure_"+failure, func(t *testing.T) {
				root, env := clangBootstrapFixture(t, "MINGW64_NT-10.0")
				nativeDownloadFixture(t, root)
				env["CI"], env["FAIL_STAGE"], env["NATIVE_FAIL"], env["MACHINE"] = "false", "python", failure, machine
				env["PINNED_DIGEST"] = "955dd1c09c60a69645e4a21be514d5153b29b9e6f159d7501ad566d7d0dd64bc"
				wheelPlatform := "win_amd64"
				if machine == "aarch64" {
					env["PINNED_DIGEST"] = "bcb607957977ea1504cc1424170a8bb2caaf1b7c8d30b03fbac7e11260cba067"
					wheelPlatform = "win_arm64"
				}
				writeFixture(t, root, "bin/uname", "#!/bin/sh\nif [ \"$1\" = -s ]; then echo \"$PLATFORM\"; else echo \"$MACHINE\"; fi\n")
				writeFixture(t, root, "bin/unzip", `#!/bin/sh
printf '%s\n' "$*" >> extract.calls
[ "${NATIVE_FAIL:-}" != extract ] || exit 23
mkdir -p "$5/clang_format/data/bin"
cp "$SEED_FORMATTER" "$5/clang_format/data/bin/clang-format.exe"
`)
				writeFixture(t, root, "seed/formatter", `#!/bin/sh
if [ "$1" = --version ]; then printf 'clang-format version %s\r\n' "${INSTALLED_VERSION:-23.1.2}"; fi
`)
				if failure == "version" {
					env["INSTALLED_VERSION"] = "23.1.20"
				}
				out, err := runFixture(t, root, "printf 'yes\\r\\n' | bash scripts/_280_format_c_code.sh check", env)
				assert.Equal(t, failure != "", err != nil, out)
				assert.NoFileExists(t, filepath.Join(root, "setup.log"), "Python setup must not run")
				data, readErr := os.ReadFile(filepath.Join(root, "sample.c"))
				assert.NoError(t, readErr)
				assert.Equal(t, "int value=1;\n", string(data))
				if failure == "download" || failure == "checksum" {
					assert.NoFileExists(t, filepath.Join(root, "extract.calls"))
				}
				if failure != "" {
					assert.Contains(t, out, "FAIL:")
					return
				}
				assert.Contains(t, out, "Using clang-format version 23.1.2")
				downloads, readErr := os.ReadFile(filepath.Join(root, "download.calls"))
				assert.NoError(t, readErr)
				assert.Contains(t, string(downloads), "clang_format-23.1.2-py2.py3-none-"+wheelPlatform+".whl")
				env["CI"], env["NATIVE_FAIL"] = "true", "download"
				out, err = runFixture(t, root, "bash scripts/_280_format_c_code.sh check", env)
				assert.NoError(t, err, out)
				assert.NotContains(t, out, "SKIP:")
				after, readErr := os.ReadFile(filepath.Join(root, "download.calls"))
				assert.NoError(t, readErr)
				assert.Equal(t, string(downloads), string(after))
			})
		}
	}
}

// TestClangFormatSetupOnly ensures CI can bootstrap without normalizing sources
// before the check, which would otherwise hide a formatting regression.
func TestClangFormatSetupOnly(t *testing.T) {
	root, env := clangBootstrapFixture(t, "Linux")
	out, err := runFixture(t, root, "bash scripts/_280_format_c_code.sh setup", env)
	assert.NoError(t, err, out)
	assert.Contains(t, out, "Ready: clang-format version 23.1.2")
	data, err := os.ReadFile(filepath.Join(root, "sample.c"))
	assert.NoError(t, err)
	assert.Equal(t, "int value=1;\n", string(data))
}

// TestClangFormatNativeCache verifies that broken launchers and unavailable
// Python cannot prevent reusing the real binary inside a relocated wheel.
func TestClangFormatNativeCache(t *testing.T) {
	for _, layout := range []struct{ platform, native, launcher string }{
		{"Linux", "Linux-x86_64/lib/python3.13/site-packages/clang_format/data/bin/clang-format", "Linux-x86_64/bin/clang-format"},
		{"Darwin", "Darwin-x86_64/clang_format/data/bin/clang-format", "Darwin-x86_64/bin/clang-format"},
		{"MINGW64_NT-10.0", "windows-x86_64/Lib/site-packages/clang_format/data/bin/clang-format.exe", "windows-x86_64/Scripts/clang-format.exe"},
		{"MSYS_NT-10.0", "windows-x86_64/clang_format/data/bin/clang-format.exe", "windows-x86_64/Scripts/clang-format.exe"},
	} {
		t.Run(layout.platform, func(t *testing.T) {
			root, env := clangBootstrapFixture(t, layout.platform)
			data, err := os.ReadFile(filepath.Join(root, "seed/formatter"))
			assert.NoError(t, err)
			// Observe execution instead of its displayed path: MSYS can launch an
			// .exe through the same path without the suffix. The launcher must
			// never substitute for this native tool, even when Python is absent.
			env["NATIVE_FORMATTER_LOG"] = filepath.Join(root, "native-formatter.log")
			native := strings.Replace(string(data), "#!/bin/sh\n", "#!/bin/sh\nprintf '%s\\n' \"$*\" >> \"$NATIVE_FORMATTER_LOG\"\n", 1)
			writeFixture(t, root, "temp/tools/clang-format-23.1.2/"+layout.native, native)
			writeFixture(t, root, "temp/tools/clang-format-23.1.2/"+layout.launcher, "#!/bin/sh\necho broken-launcher; exit 99\n")
			env["FAIL_STAGE"] = "python"
			out, err := runFixture(t, root, "bash scripts/_280_format_c_code.sh setup", env)
			assert.NoError(t, err, out)
			nativeCalls, err := os.ReadFile(env["NATIVE_FORMATTER_LOG"])
			assert.NoError(t, err)
			assert.Contains(t, string(nativeCalls), "--version\n", "the cached native tool must actually execute")
			assert.NotContains(t, out, "broken-launcher")
			_, err = os.Stat(filepath.Join(root, "setup.log"))
			assert.True(t, os.IsNotExist(err))
		})
	}
}

// TestClangFormatBatch checks the one-process contract, unchanged check inputs,
// list cleanup, empty repositories, and failures before or during the batch.
func TestClangFormatBatch(t *testing.T) {
	for _, scenario := range []string{"check", "format", "empty", "git-failure", "config-failure", "batch-failure"} {
		t.Run(scenario, func(t *testing.T) {
			root, env := clangBootstrapFixture(t, "Linux")
			env["CLANG_FORMAT_BIN"] = "./bin/formatter"
			env["SCENARIO"] = scenario
			writeFixture(t, root, "path with spaces/ü.hpp", "int declaration;\n")
			writeFixture(t, root, "bin/git", `#!/bin/sh
case "$SCENARIO" in
  empty) exit 0 ;;
  git-failure) echo 'cannot read index' >&2; exit 1 ;;
esac
printf 'sample.c\npath with spaces/ü.hpp\n'
`)
			writeFixture(t, root, "bin/formatter", `#!/bin/sh
if [ "$1" = --version ]; then echo 'clang-format version 23.1.2'; exit 0; fi
case "$*" in
  *-dump-config*) [ "$SCENARIO" != config-failure ]; exit $? ;;
esac
printf '%s\n' "$*" >> calls.log
for arg in "$@"; do
  case "$arg" in
    --files=*) list=${arg#--files=}; cat "$list" > received.txt; printf '%s' "$list" > list-path.txt ;;
  esac
done
[ "$SCENARIO" != batch-failure ] || exit 1
if [ "$1" = -i ]; then echo 'int value = 1;' > sample.c; fi
`)
			mode := "check"
			if scenario == "format" {
				mode = "format"
			}
			out, err := runFixture(t, root, "bash scripts/_280_format_c_code.sh "+mode+" --verbose", env)
			assert.Equal(t, strings.HasSuffix(scenario, "failure"), err != nil, out)
			calls, readErr := os.ReadFile(filepath.Join(root, "calls.log"))
			if scenario == "empty" || scenario == "git-failure" || scenario == "config-failure" {
				assert.True(t, os.IsNotExist(readErr), out)
			} else {
				assert.NoError(t, readErr)
				assert.Equal(t, 1, strings.Count(string(calls), "\n"))
				assert.Contains(t, string(calls), "--verbose")
				if mode == "check" {
					assert.Contains(t, string(calls), "--dry-run --Werror")
				} else {
					assert.Contains(t, string(calls), "-i ")
				}
				list, err := os.ReadFile(filepath.Join(root, "received.txt"))
				assert.NoError(t, err)
				assert.Equal(t, "sample.c\npath with spaces/ü.hpp\n", string(list))
				path, err := os.ReadFile(filepath.Join(root, "list-path.txt"))
				assert.NoError(t, err)
				_, err = os.Stat(string(path))
				assert.True(t, os.IsNotExist(err), "temporary file list must be removed even after failure")
			}
			data, err := os.ReadFile(filepath.Join(root, "sample.c"))
			assert.NoError(t, err)
			want := "int value=1;\n"
			if mode == "format" {
				want = "int value = 1;\n"
			}
			assert.Equal(t, want, string(data))
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
			for _, tool := range []string{"arm-none-eabi-gcc", "cmake", "cc", "clang"} {
				writeFixture(t, root, "bin/"+tool, "#!/bin/sh\nexit 0\n")
			}
			for _, entry := range []string{"DemoData_Trice/build.sh", "PC_log/build_and_run.sh", "G0B1_log/build.sh", "F030_inst/build.sh", "F030_inst_rtt/build.sh", "F030_inst_uart/build.sh", "G0B1_inst/build.sh", "G0B1_inst/build_with_clang.sh", "L432_inst/build.sh"} {
				writeFixture(t, root, "examples/"+entry, "#!/bin/sh\nif [ -z \"${TRICE_ID_WORKFLOW:-}\" ]; then echo 'INFO: standalone'; else echo managed-$TRICE_ID_WORKFLOW; fi\nexit \"${BUILD_STATUS:-0}\"\n")
			}
			writeFixture(t, root, "build.log", "warning: tool-version diagnostic\n")
			out, err := runFixture(t, root, "bash scripts/_200_gcc_example_build_worker.sh on full", map[string]string{"TRICE_ID_WORKFLOW": workflow, "LOGFILE": filepath.Join(root, "build.log")})
			assert.NoError(t, err, out)
			assert.Contains(t, out, "matrix-"+workflow)
			assert.Contains(t, out, "Hint:")
			if workflow == "bind" {
				assert.Equal(t, 9, strings.Count(out, "INFO: standalone"), out)
			} else {
				assert.NotContains(t, out, "INFO: standalone")
			}
			// The same worker must propagate a real failed build even if its log
			// also contains warnings that are intentionally no longer fatal.
			out, err = runFixture(t, root, "bash scripts/_200_gcc_example_build_worker.sh on quick", map[string]string{"TRICE_ID_WORKFLOW": workflow, "LOGFILE": filepath.Join(root, "build.log"), "BUILD_STATUS": "23"})
			assert.Error(t, err, out)
			if workflow == "bind" {
				for _, status := range []string{"130", "143"} {
					out, err = runFixture(t, root, "bash scripts/_200_gcc_example_build_worker.sh on full", map[string]string{"TRICE_ID_WORKFLOW": workflow, "LOGFILE": filepath.Join(root, "build.log"), "BUILD_STATUS": status})
					if assert.Error(t, err, out) {
						assert.Equal(t, "exit status "+status, err.Error(), out)
					}
					assert.NotContains(t, out, "standalone examples/PC_log", "cancellation must stop before the second standalone entry")
					assert.NotContains(t, out, "matrix-bind")
				}
			}
		})
	}
}

// TestPCLogSourcesAreLocaleIndependent records the real build command on each
// supported OS path without requiring SEGGER software or a native compiler.
// All library sources are compiled with project headers before RTT defaults;
// the platform linker removes unused transport code.
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
				assert.Contains(t, out, "no project RTT header or J-Link installation is needed")
				data, err := os.ReadFile(filepath.Join(root, "examples/PC_log/compiler-args.txt"))
				assert.NoError(t, err)
				assert.Contains(t, string(data), "../../src/SEGGER_RTT.c")
				assert.Contains(t, string(data), "../../src/trice.c")
				assert.Contains(t, string(data), "../../src/triceLog.c")
				assert.Contains(t, string(data), "-I.\n-Igenerated\n-I../../src\n-I../../src/default_conf\n")
				assert.Contains(t, string(data), "-ffunction-sections\n-fdata-sections\n")
				// Darwin uses a different linker switch; executable suffixes must
				// still follow the selected platform in both locale settings.
				if platform == "Darwin" {
					assert.Contains(t, string(data), "-Wl,-dead_strip\n")
					assert.NotContains(t, string(data), "-Wl,--gc-sections")
				} else {
					assert.Contains(t, string(data), "-Wl,--gc-sections\n")
					assert.NotContains(t, string(data), "-Wl,-dead_strip")
				}
				if platform == "MINGW64_NT" {
					assert.Contains(t, string(data), "-o\nbuild/pc_log.exe\n")
				} else {
					assert.Contains(t, string(data), "-o\nbuild/pc_log\n")
				}
			})
		}
	}
}

// TestBindStepRunsCurrentProductChecks exercises step 500 in a repository that
// contains only the active scripts. Tool doubles make compiler availability and
// Go failures deterministic without compiling historical demonstration projects.
func TestBindStepRunsCurrentProductChecks(t *testing.T) {
	for _, tc := range []struct {
		name, tools, status, diagnostic string
		called                          bool
		success                         bool
	}{
		{"current_C_and_CPP_integration_without_CMake_or_experiments", "native", "0", "PASS: Bind generation", true, true},
		{"GCC_fallback_compilers_are_accepted", "gcc", "0", "PASS: Bind generation", true, true},
		{"Go_failure_preserves_diagnostics_and_fails_the_step", "native", "7", "fixture: generated header compilation failed", true, false},
		{"missing_Go_is_reported_without_running_tests", "no_go", "0", "SKIP: Trice bind tests require Go", false, true},
		{"missing_C_frontend_is_reported_without_running_tests", "no_c", "0", "SKIP: Trice bind target integration requires", false, true},
		{"missing_CPP_frontend_is_reported_without_running_tests", "no_cpp", "0", "SKIP: Trice bind target integration requires", false, true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_500_test_bind.sh")
			// Override discovery in the disposable common helper so installed
			// host compilers cannot accidentally satisfy a missing-tool case.
			commonPath := filepath.Join(root, "scripts/_100_test_common.sh")
			common, err := os.ReadFile(commonPath)
			if !assert.NoError(t, err) {
				t.FailNow()
			}
			writeFixture(t, root, "scripts/_100_test_common.sh", string(common)+`
has_command() {
  case "$MOCK_TOOLS:$1" in
    no_go:go | no_c:cc | no_c:gcc | no_c:clang | no_cpp:c++ | no_cpp:g++ | no_cpp:clang++)
      return 1 ;;
    gcc:cc | gcc:c++)
      return 1 ;;
    *:go | *:cc | *:gcc | *:clang | *:c++ | *:g++ | *:clang++)
      return 0 ;;
    *)
      command -v "$1" >/dev/null 2>&1 ;;
  esac
}
`)
			writeFixture(t, root, "bin/go", `#!/bin/sh
set -eu
printf '%s\n' "$@" > "$LOG_DIR/go-arguments"
printf '%s\n' "${TRICE_BIND_INTEGRATION:-}" > "$LOG_DIR/integration-enabled"
if [ "$MOCK_GO_STATUS" != 0 ]; then
  echo 'fixture: generated header compilation failed'
fi
exit "$MOCK_GO_STATUS"
`)
			// CMake is deliberately available but must not be invoked: Bind's
			// real compiler regressions belong to the selected Go integration tests.
			writeFixture(t, root, "bin/cmake", "#!/bin/sh\necho unexpected-CMake-invocation\nexit 99\n")
			output, err := runFixture(t, root, "bash scripts/_500_test_bind.sh", map[string]string{
				"MOCK_TOOLS": tc.tools, "MOCK_GO_STATUS": tc.status,
			})
			if tc.success {
				assert.NoError(t, err, output)
			} else {
				assert.Error(t, err, output)
				assert.Contains(t, output, "FAIL: Trice bind integration failed")
				assert.NotContains(t, output, "PASS: Bind generation")
			}
			assert.Contains(t, output, tc.diagnostic)
			assert.NotContains(t, output, "unexpected-CMake-invocation")
			argsPath := filepath.Join(root, "temp/log/go-arguments")
			if !tc.called {
				assert.NoFileExists(t, argsPath, "missing tools must stop before invoking Go")
				return
			}
			args, err := os.ReadFile(argsPath)
			if !assert.NoError(t, err) {
				t.FailNow()
			}
			lines := strings.Split(strings.TrimSpace(string(args)), "\n")
			if !assert.Len(t, lines, 5, "one focused Go invocation must run without cached results") {
				t.FailNow()
			}
			assert.Equal(t, []string{"test", "./internal/id", "-run"}, lines[:3])
			// Check the selected behavioral contracts individually so a missing
			// compilation, rejection, canonical-source or runtime check is visible.
			for _, name := range []string{
				"TestBindGeneratedTargetCompilesCAndCPP",
				"TestBindCanonicalTriceCheckGeneratesCompleteSidecar",
				"TestBindMVP2RebaseCompilesCAndCPP",
				"TestBindMVP2CounterGuardsAndGeneratedInvariants",
				"TestBindMVP2RebaseEmitsStableRuntimeIDs",
			} {
				assert.Regexp(t, lines[3], name, "step 500 must include this production regression")
			}
			assert.Equal(t, "-count=1", lines[4])
			enabled, err := os.ReadFile(filepath.Join(root, "temp/log/integration-enabled"))
			assert.NoError(t, err)
			assert.Equal(t, "1\n", string(enabled), "target integration must not silently skip")
		})
	}
}
