// SPDX-License-Identifier: MIT

package scripts_test

import (
	"bytes"
	"compress/gzip"
	"encoding/json"
	"encoding/xml"
	"io"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"runtime"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestExampleCoverageInventory makes a newly added project an explicit coverage
// decision. Only tracked projects count; local build directories and archived
// material must not become test targets. exampleData is shared build input.
func TestExampleCoverageInventory(t *testing.T) {
	covered := map[string]string{
		"demo/direct": "TestHostDemoEntrypointIntegration", "demo/deferred": "TestHostDemoEntrypointIntegration",
		"demo/live":               "TestHostDemoEntrypointIntegration",
		"examples/DemoData_CSV":   "_600_test_gcc_bind.sh and TestHostDemoEntrypointIntegration",
		"examples/DemoData_Trice": "_600_test_gcc_bind.sh and TestHostDemoEntrypointIntegration",
		"examples/F030_bare":      "_600_test_gcc_bind.sh", "examples/G0B1_bare": "_600_test_gcc_bind.sh",
		"examples/L432_bare": "_600_test_gcc_bind.sh", "examples/F030_inst": "_600_test_gcc_bind.sh",
		"examples/G0B1_inst":      "_570_test_clang_bind.sh and _600_test_gcc_bind.sh",
		"examples/L432_inst":      "_620_test_l432_configs.sh",
		"examples/F030_inst_rtt":  "_600_test_gcc_bind.sh standalone build",
		"examples/F030_inst_uart": "_600_test_gcc_bind.sh standalone build",
		"examples/G0B1_log":       "_600_test_gcc_bind.sh standalone build",
		"examples/G0B1_features":  "_515_test_logging_features.sh GCC/Clang builds",
		"examples/PC_features":    "_515_test_logging_features.sh output checks",
		"examples/PC_log":         "_600_test_gcc_bind.sh standalone build and execution",
		"examples/TriceAbc":       "TestBindEntrypointIntegration: Bind and Insert/Clean",
		"examples/LabPlotDemo":    "TestLabPlotLaunchers: headless launch and project XML",
		"examples/LabPlotUser":    "documentation-only LabPlot walkthrough",
		"examples/exampleData":    "shared sources built by the MCU examples",
	}
	command := exec.Command("git", "ls-files", "-z", "--", "demo", "examples")
	command.Dir = ".."
	tracked, err := command.Output()
	require.NoError(t, err)
	// Report an uncovered vendor-heavy project once rather than once per file.
	seen := map[string]bool{}
	for _, name := range strings.Split(string(tracked), "\x00") {
		parts := strings.Split(name, "/")
		if len(parts) < 3 || parts[1] == "obsolete" {
			continue
		}
		project := strings.Join(parts[:2], "/")
		if seen[project] {
			continue
		}
		seen[project] = true
		assert.Contains(t, covered, project, "register the project and its real build/run coverage before declaring the suite complete")
	}
	for name := range covered {
		assert.DirExists(t, filepath.Join("..", filepath.FromSlash(name)))
	}
	// The full firmware checks must retain every target and both supported
	// standalone Clang entries; the host inventory alone cannot prove this.
	for source, entries := range map[string][]string{
		"examples/buildAllTargets_TRICE_ON.sh":     {"F030_bare", "G0B1_bare", "L432_bare", "F030_inst", "G0B1_inst", "L432_inst", "DemoData_CSV", "DemoData_Trice"},
		"scripts/_200_gcc_example_build_worker.sh": {"F030_inst_rtt/build.sh", "F030_inst_uart/build.sh", "G0B1_log/build.sh", "PC_log/build_and_run.sh", "G0B1_inst/build_with_clang.sh", "L432_inst/build_with_clang.sh"},
	} {
		data, err := os.ReadFile(filepath.Join("..", source))
		require.NoError(t, err)
		for _, entry := range entries {
			assert.Contains(t, string(data), "\""+entry+"\"", "full coverage must retain %s", entry)
		}
	}
	assert.FileExists(t, "../examples/TriceAbc/BcSimChk/demo.sh", "the nested transport demo is included in the native host integration")
}

// exampleShellPath preserves executable paths containing spaces in Git Bash as
// well as POSIX shells; the value is a path, never executable shell input.
func exampleShellPath(path string) string {
	return "'" + strings.ReplaceAll(filepath.ToSlash(path), "'", "'\"'\"'") + "'"
}

// TestDisabledRxTranslationUnit compiles the actual TX-only configuration with
// strict ISO C warnings. It catches empty library files even in normal Go runs.
func TestDisabledRxTranslationUnit(t *testing.T) {
	for _, compiler := range []string{"gcc", "clang"} {
		path, err := exec.LookPath(compiler)
		if err != nil {
			t.Logf("%s not installed; native step 515 requires both compilers", compiler)
			continue
		}
		for _, node := range []string{"N1_tx", "N2_tx"} {
			t.Run(compiler+"/"+node, func(t *testing.T) {
				object := filepath.Join(t.TempDir(), "disabled-rx.o")
				command := exec.Command(path, "-std=c99", "-Wall", "-Wextra", "-pedantic", "-Werror",
					"-I../examples/TriceAbc/"+node, "-I../src", "-I../src/default_conf", "-c", "../src/triceRx.c", "-o", object)
				output, err := command.CombinedOutput()
				require.NoError(t, err, string(output))
			})
		}
	}
}

// TestHostDemoEntrypointIntegration runs public host build/run scripts with the
// real CLI and both native compilers. Every case owns its binaries and metadata;
// a bounded process adapter stops the unchanged live demo after its first flush.
func TestHostDemoEntrypointIntegration(t *testing.T) {
	if os.Getenv("TRICE_BIND_INTEGRATION") != "1" {
		t.Skip("set TRICE_BIND_INTEGRATION=1 for real host demo builds and execution")
	}
	binary := filepath.Join(t.TempDir(), "trice")
	if runtime.GOOS == "windows" {
		binary += ".exe"
	}
	build := exec.Command("go", "build", "-o", binary, "./cmd/trice")
	build.Dir = ".."
	output, err := build.CombinedOutput()
	require.NoError(t, err, string(output))
	// Execute the RM's actual sender and CLI snippets. A one-ID test range makes
	// the illustrative TIL entry reproducible without changing the public example.
	t.Run("ABC_key_idea", func(t *testing.T) {
		manual, err := os.ReadFile("../docs/rm/abc_asynchronous_broadcast_commands.md")
		require.NoError(t, err)
		section := regexp.MustCompile("(?s)## [^\\n]*Key idea[^\\n]*\\n(.*?)\\n## ").FindSubmatch(manual)
		require.Len(t, section, 2)
		blocks := regexp.MustCompile("(?s)```(c|sh|json)\\n(.*?)\\n```").FindAllSubmatch(section[1], -1)
		require.Len(t, blocks, 6)
		root := scriptFixture(t)
		writeFixture(t, root, "sender.c", string(blocks[0][2])+"\n")
		writeFixture(t, root, "bin/trice", "#!/bin/sh\nexec "+exampleShellPath(binary)+" \"$@\"\n")
		for _, index := range []int{1, 3} {
			command := string(blocks[index][2])
			if index == 1 {
				command = strings.Replace(command, "trice bind", "trice bind -IDMin 5150 -IDMax 5150", 1)
			}
			out, err := runFixture(t, root, command, nil)
			require.NoError(t, err, out)
		}
		metadata, err := os.ReadFile(filepath.Join(root, "til.json"))
		require.NoError(t, err)
		assert.JSONEq(t, string(blocks[2][2]), string(metadata))
		var entries map[string]struct{ Type, Strg string }
		require.NoError(t, json.Unmarshal(metadata, &entries))
		assert.Equal(t, "trice8C", entries["5150"].Type)
		for _, file := range []string{"device_abc.h", "device_abc.c"} {
			generated, err := os.ReadFile(filepath.Join(root, "generated", file))
			require.NoError(t, err)
			assert.Contains(t, string(generated), "setLeds")
		}
	})
	for _, compiler := range []string{"gcc", "clang"} {
		cc, err := exec.LookPath(compiler)
		require.NoError(t, err, "step 515 requires both native compilers")
		for _, project := range []string{"demo/direct", "demo/deferred", "demo/live", "examples/DemoData_CSV", "examples/DemoData_Trice", "examples/TriceAbc", "examples/TriceAbc/BcSimChk"} {
			t.Run(compiler+"/"+project, func(t *testing.T) {
				root := copyEntrypointCheckout(t, project, "src", "examples/_020_prepare_trice_bind.sh",
					"examples/TriceAbc/BcSim", "trice_bindIDs_in_examples_and_test_folder.sh",
					"scripts/_120_setup_trice_environment.sh", "scripts/_130_trice_id_workflow.sh", "demoTIL.json")
				writeFixture(t, root, "bin/trice", "#!/bin/sh\nexec "+exampleShellPath(binary)+" \"$@\"\n")
				writeFixture(t, root, "bin/cc", "#!/bin/sh\nset -e\n"+exampleShellPath(cc)+` -Wall -Wextra -pedantic "$@"
if [ "${FIXTURE_LIVE:-0}" = 1 ]; then
  mv build/demo_live.exe build/demo_live_native.exe
  cp "$FIXTURE_ROOT/live-once.sh" build/demo_live.exe
  chmod +x build/demo_live.exe
fi
`)
				writeFixture(t, root, "live-once.sh", `#!/bin/sh
./demo_live_native.exe &
producer=$!
cleanup() { kill -TERM "$producer" 2>/dev/null || true; wait "$producer" 2>/dev/null || true; }
trap cleanup EXIT INT TERM
attempt=0
until [ -s log.bin ]; do
  kill -0 "$producer" 2>/dev/null || exit 91
  attempt=$((attempt + 1))
  [ "$attempt" -lt 1000 ] || exit 92
  sleep 0.01
done
`)
				environment := map[string]string{
					"CC": cc, "CFLAGS": "-Wall -Wextra -pedantic", "FIXTURE_ROOT": root, "TRICE_BIN": binary,
					"TRICE_DEFAULTS": "", "TRICE_PRJ_FILES": "-src ./examples/DemoData_Trice",
					"TRICE_TIL_JSON": "./demoTIL.json", "TRICE_LI_JSON": "./demoLI.json", "TRICE_LI_ROOT": ".",
					"TRICE_ID_WORKFLOW_OWNER": "0", "TRICE_ID_WORKFLOW": "", "TRICE_BIND_INCLUDE_DIR": "",
					"C_INCLUDE_PATH": "", "CPLUS_INCLUDE_PATH": "", "CPATH": "",
				}
				command, want := "", ""
				suffix := ""
				if runtime.GOOS == "windows" {
					suffix = ".exe"
				}
				switch project {
				case "demo/direct", "demo/deferred":
					command = "sh " + project + "/run.sh"
					want = "Hello from " + filepath.Base(project) + " mode."
				case "demo/live":
					environment["FIXTURE_LIVE"] = "1"
					command = "sh demo/live/run.sh\ncd demo/live\ntrice log -p FILEBUFFER -args build/log.bin"
					want = "Live counter=0."
				case "examples/DemoData_CSV":
					command = "sh " + project + "/build.sh\n" + project + "/bin/DemoData_CSV" + suffix + " --samples 3 --no-delay --header"
					want = "time_s,x,y,z"
				case "examples/DemoData_Trice":
					command = "sh " + project + "/build.sh\n" + project + "/bin/DemoData_Trice" + suffix + " --samples 3 --no-delay --output capture.bin\ntrice log -p FILEBUFFER -args capture.bin -i demoTIL.json"
					want = "Hello LabPlot!"
				case "examples/TriceAbc/BcSimChk":
					command = "bash " + project + "/demo.sh"
					want = "Starting four instances"
				case "examples/TriceAbc":
					environment["TRICE_ID_WORKFLOW"] = "bind"
					command = "bash " + project + "/demo.sh"
					want = "abc:DivideResult"
				}
				out, err := runFixture(t, root, command, environment)
				require.NoError(t, err, out)
				assert.Contains(t, out, want)
				assert.NotContains(t, out, "warning:", "host examples must not conceal compiler warnings")
			})
		}
	}
}

// TestLabPlotLaunchers checks the compressed project and both public launchers
// without opening a GUI or reserving fixed UDP ports. Native producer builds and
// decoding are covered separately above; actual plot rendering remains manual.
func TestLabPlotLaunchers(t *testing.T) {
	data, err := os.ReadFile("../examples/LabPlotDemo/LabPlotDemo.lml")
	require.NoError(t, err)
	reader, err := gzip.NewReader(bytes.NewReader(data))
	require.NoError(t, err)
	defer reader.Close()
	decoder := xml.NewDecoder(reader)
	for {
		_, err := decoder.Token()
		if err == io.EOF {
			break
		}
		require.NoError(t, err, "the shipped LabPlot project must remain valid XML")
	}
	for _, mode := range []string{"csv", "trice"} {
		t.Run(mode, func(t *testing.T) {
			root := scriptFixture(t, "examples/LabPlotDemo/run_"+mode+".sh", "examples/LabPlotDemo/LabPlotDemo.lml")
			writeFixture(t, root, "bin/labplot", "#!/bin/sh\n[ -f \"$1\" ] || exit 95\nprintf '%s\\n' \"$*\" > \"$FIXTURE_ROOT/plot-called\"\n")
			writeFixture(t, root, "bin/lsof", "#!/bin/sh\nexit 0\n")
			writeFixture(t, root, "bin/tlog", `#!/bin/sh
printf '%s\n' "$*" > "$FIXTURE_ROOT/decoder-called"
attempt=0
until [ -f "$FIXTURE_ROOT/producer-called" ]; do
  attempt=$((attempt + 1)); [ "$attempt" -lt 1000 ] || exit 91
  sleep 0.01
done
`)
			producer := "DemoData_CSV"
			if mode == "trice" {
				producer = "DemoData_Trice"
			}
			writeFixture(t, root, "examples/"+producer+"/bin/"+producer, `#!/bin/sh
attempt=0
until [ -f "$FIXTURE_ROOT/plot-called" ]; do
  attempt=$((attempt + 1)); [ "$attempt" -lt 1000 ] || exit 91
  sleep 0.01
done
printf '%s\n' "$*" > "$FIXTURE_ROOT/producer-called"
`)
			out, err := runFixture(t, root, "sh examples/LabPlotDemo/run_"+mode+".sh", map[string]string{
				"LABPLOT": filepath.Join(root, "bin/labplot"), "TLOG": filepath.Join(root, "bin/tlog"), "FIXTURE_ROOT": root,
			})
			require.NoError(t, err, out)
			plot, err := os.ReadFile(filepath.Join(root, "plot-called"))
			require.NoError(t, err)
			assert.Contains(t, string(plot), "LabPlotDemo.lml")
			arguments, err := os.ReadFile(filepath.Join(root, "producer-called"))
			require.NoError(t, err)
			assert.Contains(t, string(arguments), "--udp 127.0.0.1 900")
			if mode == "trice" {
				arguments, err = os.ReadFile(filepath.Join(root, "decoder-called"))
				require.NoError(t, err)
				assert.Contains(t, string(arguments), "-p UDP4")
				assert.Contains(t, string(arguments), "@udp://127.0.0.1:9000")
			}
		})
	}
}
