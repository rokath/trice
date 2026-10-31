// SPDX-License-Identifier: MIT

package scripts_test

import (
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

// pairedBuildFixture runs the real entry points with complete prerequisite
// doubles. No compiler, installer, source rewrite or real firmware build runs.
func pairedBuildFixture(t *testing.T, project, entry string) (string, map[string]string) {
	t.Helper()
	root := l432ClangFixture(t)
	name := "examples/" + project + "/" + entry
	data, err := os.ReadFile(filepath.Join("..", name))
	if !assert.NoError(t, err) {
		t.FailNow()
	}
	writeFixture(t, root, name, string(data))
	writeFixture(t, root, "generated/probe.h", "/* managed Bind fixture */\n")
	writeFixture(t, root, "bin/llvm-size", "#!/bin/sh\nexit 99\n")
	writeFixture(t, root, "bin/clang", `#!/bin/sh
case "$1" in
  --print-targets)
    printf 'Registered Targets:\n'
    [ "${CLANG_NO_ARM:-0}" = 1 ] || printf '    thumb - Thumb\n'
    ;;
  --print-prog-name=llvm-size) printf 'llvm-size\n' ;;
  *) exit 99 ;;
esac
`)
	writeFixture(t, root, "bin/make", `#!/bin/sh
if [ "${1:-}" = --version ]; then printf '%s\n' "${MAKE_VERSION:-GNU Make 4.4.1}"; exit 0; fi
[ "${1:-}" != clean ] || exit 0
printf '%s\n' "$@" > ../../build.args
printf '%s\n' "$*" >> ../../build.calls
[ "${BUILD_STATUS:-0}" = 0 ] || exit "$BUILD_STATUS"
for argument; do compiler=$argument; done
case "$PWD" in */L432_inst) image=L432KC ;; *) image=G0B1 ;; esac
mkdir -p "out.$compiler"
for extension in elf hex bin; do printf 'fixture image\n' > "out.$compiler/$image.$extension"; done
`)
	return root, map[string]string{
		"TRICE_ID_WORKFLOW_OWNER": "1", "TRICE_ID_WORKFLOW": "bind",
		"TRICE_BIND_INCLUDE_DIR": filepath.Join(root, "generated"),
	}
}

// TestPairedBuildCLI checks the externally visible define arguments for all six
// entry points, including defaults, equivalent profile spellings and rejections
// before source preparation. Feature profiles are deliberately unavailable.
func TestPairedBuildCLI(t *testing.T) {
	for _, project := range []string{"G0B1_inst", "L432_inst", "G0B1_features"} {
		for _, entry := range []string{"build.sh", "build_with_clang.sh"} {
			cases := []struct {
				arguments, flags string
				reject           bool
			}{
				{"", "", false}, {"EXTRA=7 ENABLED", "-DEXTRA=7 -DENABLED ", false},
				{"98 EXTRA=7", "-DCONFIGURATION=98 -DEXTRA=7 ", project == "G0B1_features"},
				{"CONFIGURATION=98 EXTRA=7", "-DCONFIGURATION=98 -DEXTRA=7 ", project == "G0B1_features"},
				{"100", "-DCONFIGURATION=100 ", project != "L432_inst"},
				{"0", "-DCONFIGURATION=0 ", project == "G0B1_features"},
				{"098", "", true}, {"101", "", true}, {"-1", "", true},
				{"98 0", "", true}, {"CONFIGURATION=98 98", "", true},
				{"--check EXTRA=1", "", true}, {"--help 98", "", true},
				{"--x0-matrix EXTRA=1", "", true}, {"--unknown", "", true},
				{"bad-name=1", "", true}, {"=value", "", true},
			}
			for _, tc := range cases {
				t.Run(project+"/"+entry+"/"+tc.arguments, func(t *testing.T) {
					root, env := pairedBuildFixture(t, project, entry)
					out, err := runFixture(t, root, "bash examples/"+project+"/"+entry+" "+tc.arguments, env)
					assert.Equal(t, tc.reject, err != nil, out)
					if tc.reject {
						assert.Contains(t, out, "error:")
						assert.NoFileExists(t, filepath.Join(root, "build.args"))
						assert.NoFileExists(t, filepath.Join(root, "preparation.calls"))
						return
					}
					args, readErr := os.ReadFile(filepath.Join(root, "build.args"))
					assert.NoError(t, readErr)
					assert.Contains(t, string(args), "TRICE_FLAGS="+tc.flags+"\n")
					compiler := "gcc"
					if entry == "build_with_clang.sh" {
						compiler = "clang"
					}
					assert.True(t, strings.HasSuffix(string(args), "\n"+compiler+"\n"), string(args))
				})
			}
		}
	}
}

// TestPairedBuildControlOptions verifies help/check have no preparation or build
// side effects, and every matrix reaches all four cases with its own compiler.
func TestPairedBuildControlOptions(t *testing.T) {
	for _, project := range []string{"G0B1_inst", "L432_inst", "G0B1_features"} {
		for _, entry := range []string{"build.sh", "build_with_clang.sh"} {
			for _, option := range []string{"--help", "-h", "--check", "--x0-matrix"} {
				t.Run(project+"/"+entry+"/"+option, func(t *testing.T) {
					root, env := pairedBuildFixture(t, project, entry)
					out, err := runFixture(t, root, "bash examples/"+project+"/"+entry+" "+option, env)
					assert.NoError(t, err, out)
					assert.NoFileExists(t, filepath.Join(root, "preparation.calls"))
					if option != "--x0-matrix" {
						assert.NoFileExists(t, filepath.Join(root, "build.args"))
						if option == "--check" {
							assert.Contains(t, out, "Prerequisites OK.")
						} else {
							assert.Contains(t, out, "--help | --check | --x0-matrix")
						}
						return
					}
					calls, readErr := os.ReadFile(filepath.Join(root, "build.calls"))
					assert.NoError(t, readErr)
					compiler := "gcc"
					if entry == "build_with_clang.sh" {
						compiler = "clang"
					}
					for _, off := range []string{"0", "1"} {
						for _, x0 := range []string{"0", "1"} {
							assert.Contains(t, string(calls), "TRICE_FLAGS=-DTRICE_OFF="+off+" -DTRICE_TX_X0_COUNTED_BUFFER_SUPPORT="+x0+" ")
						}
					}
					for _, call := range strings.Split(strings.TrimSpace(string(calls)), "\n") {
						assert.True(t, strings.HasSuffix(call, " "+compiler), call)
					}
				})
			}
		}
	}
}

// TestPairedBuildControlFailures rejects unsuitable tools without preparation
// and stops the matrix at the first failing build, retaining its exit status.
func TestPairedBuildControlFailures(t *testing.T) {
	for _, entry := range []string{"build.sh", "build_with_clang.sh"} {
		for _, mode := range []string{"non_GNU_make", "matrix_failure", "missing_Thumb"} {
			if entry == "build.sh" && mode == "missing_Thumb" {
				continue
			}
			t.Run(entry+"/"+mode, func(t *testing.T) {
				root, env := pairedBuildFixture(t, "G0B1_inst", entry)
				option := "--check"
				switch mode {
				case "non_GNU_make":
					env["MAKE_VERSION"] = "BSD Make"
				case "missing_Thumb":
					env["CLANG_NO_ARM"] = "1"
				case "matrix_failure":
					option = "--x0-matrix"
					env["BUILD_STATUS"] = "43"
				}
				out, err := runFixture(t, root, "bash examples/G0B1_inst/"+entry+" "+option, env)
				if !assert.Error(t, err, out) {
					return
				}
				assert.NoFileExists(t, filepath.Join(root, "preparation.calls"))
				if mode == "matrix_failure" {
					assert.Contains(t, err.Error(), "exit status 43")
					calls, readErr := os.ReadFile(filepath.Join(root, "build.calls"))
					assert.NoError(t, readErr)
					assert.Equal(t, 1, strings.Count(string(calls), "\n"))
				} else {
					assert.Contains(t, out, "error:")
					assert.NoFileExists(t, filepath.Join(root, "build.args"))
				}
			})
		}
	}
}

// TestFeatureBuildDefinesStayPrivate checks standalone define builds keep the
// feature project's CE/ID preparation and propagate its failure unchanged.
func TestFeatureBuildDefinesStayPrivate(t *testing.T) {
	for _, entry := range []string{"build.sh", "build_with_clang.sh"} {
		t.Run(entry, func(t *testing.T) {
			root, env := pairedBuildFixture(t, "G0B1_features", entry)
			env["TRICE_ID_WORKFLOW_OWNER"] = "0"
			writeFixture(t, root, "examples/G0B1_features/demo_build.sh", `#!/bin/sh
printf '%s\n' "$TRICE_FLAGS" "$@" > "$(dirname "$0")/feature.args"
exit 29
`)
			out, err := runFixture(t, root, "bash examples/G0B1_features/"+entry+" EXTRA=7 ENABLED", env)
			if !assert.Error(t, err, out) {
				return
			}
			assert.Contains(t, err.Error(), "exit status 29")
			args, readErr := os.ReadFile(filepath.Join(root, "examples/G0B1_features/feature.args"))
			assert.NoError(t, readErr)
			want := "-DEXTRA=7 -DENABLED \n"
			if entry == "build_with_clang.sh" {
				want += "clang\n"
			}
			assert.Equal(t, want, string(args))
			assert.NoFileExists(t, filepath.Join(root, "preparation.calls"))
			assert.NoFileExists(t, filepath.Join(root, "build.args"))
		})
	}
}

// TestFeatureBuildDefineTransitions exercises the real private build helper.
// Warm builds may reuse objects only for the last successfully built defines.
func TestFeatureBuildDefineTransitions(t *testing.T) {
	for _, entry := range []string{"build.sh", "build_with_clang.sh"} {
		t.Run(entry, func(t *testing.T) {
			root := scriptFixture(t, "examples/G0B1_features/"+entry, "examples/G0B1_features/demo_build.sh")
			writeFixture(t, root, "scripts/_150_setup_build_environment.sh", "# fixture tools are already configured\n")
			for _, tool := range []string{"trice", "arm-none-eabi-gcc", "clang"} {
				writeFixture(t, root, "bin/"+tool, "#!/bin/sh\nexit 0\n")
			}
			writeFixture(t, root, "bin/make", `#!/bin/sh
printf '%s|%s\n' "$*" "$TRICE_FLAGS" >> calls
exit "${MAKE_FAIL:-0}"
`)
			compiler := "gcc"
			if entry == "build_with_clang.sh" {
				compiler = "clang"
			}
			want := ""
			for _, tc := range []struct {
				arguments   string
				force, fail bool
			}{
				{"", true, false}, {"EXTRA=7", true, false}, {"EXTRA=7", false, false},
				{"", true, false}, {"TRICE_OFF=1", true, true}, {"", true, false},
				{"TRICE_OFF=1", true, false}, {"TRICE_OFF=1", false, false},
			} {
				env := map[string]string{"TRICE_ID_WORKFLOW_OWNER": "0", "MAKE_FAIL": "0"}
				if tc.fail {
					env["MAKE_FAIL"] = "43"
				}
				out, err := runFixture(t, root, "bash examples/G0B1_features/"+entry+" "+tc.arguments, env)
				assert.Equal(t, tc.fail, err != nil, out)
				if tc.force {
					want += "-B "
				}
				want += compiler + "|"
				if tc.arguments != "" {
					want += "-D" + tc.arguments + " "
				}
				want += "\n"
				calls, readErr := os.ReadFile(filepath.Join(root, "examples/G0B1_features/calls"))
				assert.NoError(t, readErr)
				assert.Equal(t, want, string(calls))
			}
		})
	}
}
