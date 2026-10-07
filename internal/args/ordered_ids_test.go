// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// orderedTargetSource emits real unframed scalar records, with two identical
// sites on one line. Its Windows stdout must preserve the binary wire bytes.
const orderedTargetSource = `// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <stdio.h>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
#include "trice.h"
void earlier(void);
static void capture(const uint8_t *data, size_t size) {
    fwrite(data, 1, size, stdout);
}
int main(void) {
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_BINARY);
#endif
    UserNonBlockingDirectWrite8AuxiliaryFn = capture;
    earlier();
    trice32("msg:value={value}\n", 7); trice32("msg:value={value}\n", 9);
    return 0;
}
`

// TestOrderedIDsTargetRecordsMatchSourceAndCatalog verifies the complete public
// Insert/Bind -> C/C++ target -> Go JSON decoder path. Adding an earlier file
// must change the two old runtime IDs as well as LI, not merely sort metadata.
func TestOrderedIDsTargetRecordsMatchSourceAndCatalog(t *testing.T) {
	if os.Getenv("TRICE_BIND_INTEGRATION") != "1" {
		t.Skip("set TRICE_BIND_INTEGRATION=1 for the ordered-ID target/decoder test")
	}
	_, filename, _, ok := runtime.Caller(0)
	require.True(t, ok)
	root := filepath.Clean(filepath.Join(filepath.Dir(filename), "..", ".."))
	library, err := filepath.Glob(filepath.Join(root, "src", "[a-z]*.c"))
	require.NoError(t, err)
	require.NotEmpty(t, library)
	// Exercise installed compiler families without making an optional second
	// family a new prerequisite of the regular feature integration step.
	var families []struct{ name, c, cpp string }
	for _, family := range []struct{ name, c, cpp string }{{"clang", "clang", "clang++"}, {"gcc", "gcc", "g++"}} {
		cc, cErr := exec.LookPath(family.c)
		cpp, cppErr := exec.LookPath(family.cpp)
		if cErr != nil || cppErr != nil {
			t.Logf("compiler family unavailable, not tested: %s / %s", family.c, family.cpp)
			continue
		}
		families = append(families, struct{ name, c, cpp string }{family.name, cc, cpp})
	}
	require.NotEmpty(t, families, "target integration requires an installed C/C++ compiler pair")
	for _, command := range []string{"insert", "bind"} {
		t.Run(command, func(t *testing.T) {
			project := t.TempDir()
			fs := &afero.Afero{Fs: afero.NewOsFs()}
			til, li := filepath.Join(project, "til.json"), filepath.Join(project, "li.json")
			main, earlier := filepath.Join(project, "main.c"), filepath.Join(project, "a.c")
			generated := filepath.Join(project, "generated")
			source := orderedTargetSource
			if command == "insert" {
				// Deliberately descending explicit IDs must be reordered too.
				source = strings.Replace(source, `trice32("msg:value={value}\n", 7)`, `trice32(iD(1002), "msg:value={value}\n", 7)`, 1)
				source = strings.Replace(source, `trice32("msg:value={value}\n", 9)`, `trice32(iD(1001), "msg:value={value}\n", 9)`, 1)
			}
			require.NoError(t, fs.WriteFile(main, []byte(source), 0o644))
			require.NoError(t, fs.WriteFile(earlier, []byte("void earlier(void) {}\n"), 0o644))
			config := strings.Replace(contextTargetConfig, "#define TRICE_RX_LOG_SUPPORT 1", "#define TRICE_RX_LOG_SUPPORT 0", 1)
			require.NoError(t, fs.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(config), 0o644))
			format := id.TriceFmt{Type: "trice32", Strg: `msg:value={value}\n`}
			catalog, err := json.Marshal(id.TriceIDLookUp{1001: format, 1002: format, 1003: format})
			require.NoError(t, err)
			require.NoError(t, fs.WriteFile(til, catalog, 0o644))
			require.NoError(t, fs.WriteFile(li, []byte(`{"1001":{"File":"main.c","Line":21},"1002":{"File":"main.c","Line":21},"1003":{"File":"a.c","Line":3}}`), 0o644))
			options := []string{command, "-src", project, "-til", til, "-li", li, "-liRoot", project, "-genDir", generated, "-IDMin", "1000", "-IDMax", "1999", "-IDMethod", "upward"}
			// A file selected through absolute and relative roots is one site
			// set. Different Windows drives cannot form a relative path.
			workingDirectory, err := os.Getwd()
			require.NoError(t, err)
			if relative, err := filepath.Rel(workingDirectory, main); err == nil {
				options = append(options, "-src", relative, "-src", main)
			}
			for _, added := range []bool{false, true} {
				phase := "two_sites_on_one_line"
				wantIDs, wantValues := []uint16{1001, 1002}, []int{7, 9}
				if added {
					phase = "third_site_in_earlier_file"
					call := `trice32("msg:value={value}\n", 11);`
					if command == "insert" {
						call = `trice32(iD(1003), "msg:value={value}\n", 11);`
					}
					require.NoError(t, fs.WriteFile(earlier, []byte("#include \"trice.h\"\nvoid earlier(void) {\n"+call+"\n}\n"), 0o644))
					wantIDs, wantValues = []uint16{1001, 1002, 1003}, []int{11, 7, 9}
				}
				output, err := runContextCLI(t, fs, options...)
				require.NoError(t, err, output)
				locationData, err := fs.ReadFile(li)
				require.NoError(t, err)
				var locations id.TriceIDLookUpLI
				require.NoError(t, json.Unmarshal(locationData, &locations))
				assert.Equal(t, "main.c", locations[id.TriceID(wantIDs[len(wantIDs)-1])].File)
				if added {
					// Bind adds its owner include. Check the actual final source
					// position rather than assuming the uninstrumented line number.
					instrumented, err := fs.ReadFile(earlier)
					require.NoError(t, err)
					callStart := bytes.Index(instrumented, []byte("trice32("))
					require.GreaterOrEqual(t, callStart, 0)
					line := bytes.Count(instrumented[:callStart], []byte("\n")) + 1
					assert.Equal(t, id.TriceLI{File: "a.c", Line: line}, locations[1001])
				}
				// Both front ends must produce the same actual records for each
				// installed compiler pair; absent families have no PASS subtest.
				for _, family := range families {
					t.Run(phase+"/"+family.name, func(t *testing.T) {
						common := []string{"-I", project, "-I", generated, "-I", filepath.Join(root, "src"), "-I", filepath.Join(root, "src", "default_conf")}
						compile := exec.Command(family.c, append(append([]string{"-std=c11", "-c"}, common...), library...)...)
						compile.Dir = project
						buildOutput, err := compile.CombinedOutput()
						require.NoError(t, err, "%s", buildOutput)
						objects, err := filepath.Glob(filepath.Join(project, "*.o"))
						require.NoError(t, err)
						for _, language := range []struct{ name, standard, compiler string }{{"c", "c11", family.c}, {"c++", "c++17", family.cpp}} {
							t.Run(language.name, func(t *testing.T) {
								executable := filepath.Join(project, "ordered-target")
								if runtime.GOOS == "windows" {
									executable += ".exe"
								}
								arguments := append([]string{"-x", language.name, "-std=" + language.standard}, common...)
								arguments = append(arguments, main, earlier, "-x", "none")
								arguments = append(arguments, objects...)
								arguments = append(arguments, "-o", executable)
								buildOutput, err := exec.Command(language.compiler, arguments...).CombinedOutput()
								require.NoError(t, err, "%s", buildOutput)
								wire, err := exec.Command(executable).Output()
								require.NoError(t, err)
								require.Len(t, wire, len(wantIDs)*8, "each unstamped scalar record contains a 4-byte header and value")
								for index, want := range wantIDs {
									assert.Equal(t, want, binary.LittleEndian.Uint16(wire[index*8:])&0x3fff, "actual target ID at record %d", index)
								}
								capture := filepath.Join(project, "capture.bin")
								require.NoError(t, fs.WriteFile(capture, wire, 0o644))
								decoded, err := runContextCLI(t, fs, "log", "-p", "FILEBUFFER", "-args", capture, "-pf", "none", "-til", til, "-li", "off", "-hs", "off", "-ts", "off", "-prefix", "", "-suffix", "", "-color", "none", "-logFormat", "json")
								require.NoError(t, err, decoded)
								decoder := json.NewDecoder(bytes.NewBufferString(decoded))
								for _, value := range wantValues {
									var event struct {
										Message string         `json:"message"`
										Fields  map[string]int `json:"fields"`
									}
									require.NoError(t, decoder.Decode(&event), decoded)
									assert.Equal(t, map[string]int{"value": value}, event.Fields)
									assert.Equal(t, "value="+fmt.Sprint(value)+"\n", event.Message)
								}
								assert.False(t, decoder.More(), "no extra decoded records")
							})
						}
					})
				}
			}
		})
	}
}
