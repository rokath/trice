// SPDX-License-Identifier: MIT

package args

import (
	"encoding/json"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// nativePathSource sends real binary records rather than relying on a compiler
// accepting the source. Windows stdout is binary so CRLF translation cannot
// corrupt the wire capture. UTF-8 remains dictionary text, not payload bytes.
const nativePathSource = `// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <stdio.h>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
#include "trice.h"
NATIVE_BIND_INCLUDE
static void capture(const uint8_t *data, size_t size) {
    fwrite(data, 1, size, stdout);
}
int main(void) {
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_BINARY);
#endif
    UserNonBlockingDirectWrite8AuxiliaryFn = capture;
    trice32("info:Grüße value=%u\n", 7u);
    return 0;
}
`

// TestNativeProjectPathsAndLineEndings exercises real filesystem paths, the
// public CLI, both compiler families and the real decoder on the current OS.
// The integration switch is mandatory in testAll/CI and keeps plain go test
// usable without downloading compilers or modifying a developer's environment.
func TestNativeProjectPathsAndLineEndings(t *testing.T) {
	if os.Getenv("TRICE_BIND_INTEGRATION") != "1" {
		t.Skip("set TRICE_BIND_INTEGRATION=1 for native filesystem/compiler integration")
	}
	root, err := filepath.Abs("../..")
	require.NoError(t, err)
	binary := filepath.Join(t.TempDir(), "Werkzeuge Grüße with spaces", "trice")
	require.NoError(t, os.MkdirAll(filepath.Dir(binary), 0o755))
	if runtime.GOOS == "windows" {
		binary += ".exe"
	}
	build := exec.Command("go", "build", "-o", binary, "./cmd/trice")
	build.Dir = root
	out, err := build.CombinedOutput()
	require.NoError(t, err, "%s", out)
	separators := []string{"native"}
	if runtime.GOOS == "windows" {
		separators = append(separators, "forward_slashes")
	}
	for _, workflow := range []string{"bind", "insert"} {
		for _, ending := range []string{"LF", "CRLF"} {
			for _, separator := range separators {
				t.Run(workflow+"/"+ending+"/"+separator, func(t *testing.T) {
					project := filepath.Join(t.TempDir(), "Projekt Grüße with spaces")
					caller := filepath.Join(project, "caller")
					generated := filepath.Join(project, "generated headers")
					require.NoError(t, os.MkdirAll(caller, 0o755))
					// Explicit empty dictionaries satisfy the CLI's safety contract;
					// a missing user-supplied dictionary must never be auto-created.
					for _, name := range []string{"til.json", "li.json"} {
						require.NoError(t, os.WriteFile(filepath.Join(project, name), []byte("{}\n"), 0o644))
					}
					source := strings.ReplaceAll(nativePathSource, "NATIVE_BIND_INCLUDE", "")
					if workflow == "bind" {
						source = strings.ReplaceAll(nativePathSource, "NATIVE_BIND_INCLUDE", `#include "trice_sidecar_main_c_1111111111111111.h" // trice-bind: keep as last include`)
					}
					if ending == "CRLF" {
						source = strings.ReplaceAll(source, "\n", "\r\n")
					}
					path := filepath.Join(project, "main.c")
					require.NoError(t, os.WriteFile(path, []byte(source), 0o644))
					config := strings.ReplaceAll(contextTargetConfig, "#define TRICE_RX_LOG_SUPPORT 1", "#define TRICE_RX_LOG_SUPPORT 0")
					require.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(config), 0o644))
					// Every CLI path is relative to a different working directory.
					// On Windows the two cases test actual backslash and slash argv.
					relative := func(path string) string {
						value, err := filepath.Rel(caller, path)
						require.NoError(t, err)
						if separator == "forward_slashes" {
							value = filepath.ToSlash(value)
						}
						return value
					}
					base := []string{"-src", relative(path), "-til", "../til.json", "-li", "../li.json", "-liRoot", relative(project)}
					run := func(arguments ...string) []byte {
						cmd := exec.Command(binary, arguments...)
						cmd.Dir = caller
						output, err := cmd.CombinedOutput()
						require.NoError(t, err, "%v\n%s", arguments, output)
						return output
					}
					arguments := append([]string{workflow}, base...)
					arguments = append(arguments, "-genDir", relative(generated), "-IDMin", "1000", "-IDMax", "1999", "-IDMethod", "upward")
					run(arguments...)
					instrumented, err := os.ReadFile(path)
					require.NoError(t, err)
					if workflow == "bind" {
						assert.Equal(t, []byte(source), instrumented, "Bind must preserve authored bytes")
					}
					run(arguments...)
					repeated, err := os.ReadFile(path)
					require.NoError(t, err)
					assert.Equal(t, instrumented, repeated, "repeated instrumentation is byte-stable")
					// Only a successful probe may allow an alternative LI spelling.
					caseInsensitiveSource := false
					if ending == "LF" {
						// Detect this volume's semantics instead of assuming every
						// macOS volume ignores case or every Linux volume preserves it.
						alternate := filepath.Join(project, "MAIN.c")
						alternateInfo, statErr := os.Stat(alternate)
						caseSensitive := os.IsNotExist(statErr)
						t.Logf("case-sensitive filesystem: %v", caseSensitive)
						if !caseSensitive {
							require.NoError(t, statErr)
							originalInfo, err := os.Stat(path)
							require.NoError(t, err)
							require.True(t, os.SameFile(originalInfo, alternateInfo))
							caseInsensitiveSource = true
						}
						caseArguments := append([]string{}, arguments...)
						caseArguments[2] = relative(alternate) // Value of the declared -src flag.
						originalTIL, err := os.ReadFile(filepath.Join(project, "til.json"))
						require.NoError(t, err)
						output := run(caseArguments...)
						if caseSensitive {
							assert.Contains(t, string(output), "does not exist")
						} else {
							assert.NotContains(t, string(output), "does not exist")
						}
						// Selecting both aliases must still process one physical site.
						run(append(caseArguments, "-src", relative(path))...)
						afterTIL, err := os.ReadFile(filepath.Join(project, "til.json"))
						require.NoError(t, err)
						assert.Equal(t, originalTIL, afterTIL, "case spelling cannot allocate duplicate IDs")
						afterCase, err := os.ReadFile(path)
						require.NoError(t, err)
						assert.Equal(t, instrumented, afterCase, "case spelling cannot rewrite an already instrumented source")
					}
					var locations map[string]struct {
						File string
						Line int
					}
					li, err := os.ReadFile(filepath.Join(project, "li.json"))
					require.NoError(t, err)
					require.NoError(t, json.Unmarshal(li, &locations))
					require.Len(t, locations, 1)
					for _, location := range locations {
						assert.True(t, location.File == "main.c" || (caseInsensitiveSource && location.File == "MAIN.c"), "LI uses the source spelling accepted by the native filesystem: %s", location.File)
						assert.Equal(t, 1+strings.Count(source[:strings.Index(source, "    trice32(")], "\n"), location.Line)
					}
					library, err := filepath.Glob(filepath.Join(root, "src", "[a-z]*.c"))
					require.NoError(t, err)
					for _, compiler := range []string{"gcc", "clang"} {
						t.Run(compiler, func(t *testing.T) {
							// Keep the Unicode project path in compiler inputs, but use a
							// relative linker output: Windows GNU ld cannot reliably open
							// this absolute Unicode path. Separate outputs also avoid
							// overwriting a just-run executable while Windows releases it.
							executable := filepath.Join(project, compiler+"-native-target.exe")
							args := []string{"-std=c11", "-O0", "-Wall", "-Wextra", "-Werror", "-I", project, "-I", generated, "-I", filepath.Join(root, "src"), "-I", filepath.Join(root, "src/default_conf"), path}
							args = append(args, library...)
							args = append(args, "-o", filepath.Base(executable))
							compile := exec.Command(compiler, args...)
							compile.Dir = project
							compiled, err := compile.CombinedOutput()
							require.NoError(t, err, "%s", compiled)
							wire, err := exec.Command(executable).Output()
							require.NoError(t, err)
							require.NotEmpty(t, wire)
							capture := filepath.Join(project, "capture.bin")
							require.NoError(t, os.WriteFile(capture, wire, 0o644))
							decoded := run("log", "-p", "FILEBUFFER", "-args", relative(capture), "-pf", "none", "-til", "../til.json", "-li", "off", "-hs", "off", "-ts", "off", "-prefix", "", "-suffix", "", "-color", "none", "-logFormat", "json")
							var event struct {
								Message string `json:"message"`
							}
							require.NoError(t, json.Unmarshal(decoded, &event), "%s", decoded)
							assert.Equal(t, "Grüße value=7\n", event.Message)
						})
					}
					if workflow == "insert" {
						run(append([]string{"clean"}, base...)...)
						cleaned, err := os.ReadFile(path)
						require.NoError(t, err)
						assert.Equal(t, []byte(source), cleaned, "Clean must restore UTF-8 and authored line endings")
					}
				})
			}
		}
	}
}
