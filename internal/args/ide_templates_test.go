// SPDX-License-Identifier: MIT

package args

import (
	"encoding/json"
	"encoding/xml"
	"flag"
	"io"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// ideLaunch models the shared fields we can validate without starting an IDE,
// opening a serial port or requiring debugger hardware.
type ideLaunch struct {
	Configurations []struct {
		Name, Type, Request, Mode, Program, Cwd string
		Args                                    []string
	}
	Inputs []struct {
		ID, Type, Description, Default string
	}
}

// readIDELaunch keeps validation tied to the actual shipped templates.
func readIDELaunch(t *testing.T) ideLaunch {
	t.Helper()
	data, err := os.ReadFile("../../.vscode/launch.json")
	require.NoError(t, err)
	var launch ideLaunch
	require.NoError(t, json.Unmarshal(data, &launch))
	require.NotEmpty(t, launch.Configurations)
	return launch
}

// ideArguments resolves the same checkout and prompt references as VS Code.
// Empty prompt defaults receive a realistic serial port without opening it.
func ideArguments(t *testing.T, launch ideLaunch, args []string, root string) []string {
	t.Helper()
	resolved := make([]string, len(args))
	for index, arg := range args {
		arg = strings.ReplaceAll(arg, "${workspaceFolder}/", root)
		for _, input := range launch.Inputs {
			value := input.Default
			if value == "" {
				value = "COM3"
			}
			arg = strings.ReplaceAll(arg, "${input:"+input.ID+"}", value)
		}
		require.NotContains(t, arg, "${", "unresolved VS Code variable")
		resolved[index] = arg
	}
	return resolved
}

// TestIDELaunchUsesCurrentCommandsAndExistingInputs rejects removed flags,
// stale checkout paths and missing prompt declarations without executing live
// logging or source-changing commands. Parsing uses the production flag values
// but ContinueOnError prevents a broken template from exiting the test process.
func TestIDELaunchUsesCurrentCommandsAndExistingInputs(t *testing.T) {
	launch := readIDELaunch(t)
	root, err := filepath.Abs("../..")
	require.NoError(t, err)
	names := make(map[string]bool)
	inputs := make(map[string]bool)
	for _, input := range launch.Inputs {
		assert.False(t, inputs[input.ID], "duplicate input %q", input.ID)
		inputs[input.ID] = true
		assert.Equal(t, "promptString", input.Type)
		assert.NotEmpty(t, input.Description)
	}
	t.Cleanup(func() {
		id.Srcs, id.ExcludeSrcs, id.TriceAliases, id.TriceSAliases = nil, nil, nil, nil
		FlagsInit()
	})
	for _, config := range launch.Configurations {
		t.Run(config.Name, func(t *testing.T) {
			require.False(t, names[config.Name], "duplicate launch name")
			names[config.Name] = true
			assert.Equal(t, "go", config.Type)
			assert.Equal(t, "launch", config.Request)
			program := strings.ReplaceAll(config.Program, "${workspaceFolder}", root)
			info, err := os.Stat(program)
			require.NoError(t, err, "debug package must exist")
			require.True(t, info.IsDir())
			args := ideArguments(t, launch, config.Args, root+"/")
			require.NotEmpty(t, args)
			if config.Mode == "test" {
				require.Len(t, args, 2)
				require.Equal(t, "-test.run", args[0])
				selection, err := regexp.Compile(args[1])
				require.NoError(t, err)
				files, err := filepath.Glob(filepath.Join(program, "*_test.go"))
				require.NoError(t, err)
				found := false
				for _, file := range files {
					source, err := os.ReadFile(file)
					require.NoError(t, err)
					for _, name := range regexp.MustCompile(`func (Test\w+)\(t \*testing\.T\)`).FindAllSubmatch(source, -1) {
						found = found || selection.Match(name[1])
					}
				}
				assert.True(t, found, "selected test must exist in the debug package")
				return
			}
			require.Equal(t, "auto", config.Mode)
			assert.Equal(t, "${workspaceFolder}", config.Cwd, "CLI paths resolve from the repository root")
			FlagsInit()
			id.Srcs, id.ExcludeSrcs, id.TriceAliases, id.TriceSAliases = nil, nil, nil, nil
			commands := map[string]*flag.FlagSet{
				"generate": fsScGenerate, "log": fsScLog, "ver": fsScVersion,
				"help": fsScHelp, "ds": fsScSv, "sd": fsScSdSv,
				"insert": fsScInsert, "clean": fsScClean,
			}
			original, ok := commands[args[0]]
			require.True(t, ok, "unreviewed or removed command %q", args[0])
			flags := flag.NewFlagSet(args[0], flag.ContinueOnError)
			flags.SetOutput(io.Discard)
			original.VisitAll(func(option *flag.Flag) {
				flags.Var(option.Value, option.Name, option.Usage)
			})
			require.NoError(t, flags.Parse(args[1:]), "template must use current CLI flags")
			assert.Empty(t, flags.Args(), "no stray positional arguments")
			for _, arg := range args {
				if strings.HasPrefix(arg, root+"/") && arg != root+"/generated" {
					_, err := os.Stat(filepath.FromSlash(arg))
					assert.NoError(t, err, "referenced input %q must exist", arg)
				}
			}
		})
	}
}

// ideMemoryInputs copies only a template's real source and metadata into a
// disposable filesystem. Running instrumentation cannot change the checkout.
func ideMemoryInputs(t *testing.T, paths ...string) *afero.Afero {
	t.Helper()
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	for _, path := range paths {
		data, err := os.ReadFile(filepath.Join("../..", path))
		require.NoError(t, err)
		require.NoError(t, fs.MkdirAll(filepath.Dir(path), 0o755))
		require.NoError(t, fs.WriteFile(path, data, 0o644))
	}
	return fs
}

// TestIDEDumpDecodesItsOwnCapture proves that a merely existing JSON file is
// not enough: the shipped bytes must decode with the shipped fixture table.
// The historical capture omits cycle 194, and that diagnostic stays enabled.
func TestIDEDumpDecodesItsOwnCapture(t *testing.T) {
	launch := readIDELaunch(t)
	for _, config := range launch.Configurations {
		if config.Name != "trice l -p DUMP" {
			continue
		}
		fs := ideMemoryInputs(t, "_test/testdata/testTIL.json", "_test/testdata/testLI.json")
		args := ideArguments(t, launch, config.Args, "")
		args = append(args, "-color", "off", "-hs", "off", "-prefix", "off")
		output, err := runContextCLI(t, fs, args...)
		require.NoError(t, err, output)
		for _, message := range []string{
			"NUCLEO-G0B1RE", "TRICE_DIRECT_OUTPUT == 1, TRICE_DEFERRED_OUTPUT == 1",
			"_SINGLE_MAX_SIZE=180, _BUFFER_SIZE=248, _DEFERRED_BUFFER_SIZE=6000",
		} {
			assert.Contains(t, output, message)
		}
		assert.Equal(t, 1, strings.Count(output, "CYCLE_ERROR:"))
		assert.Contains(t, output, "195 != 194")
		assert.NotContains(t, output, "unknown ID")
		return
	}
	t.Fatal("hardware-free DUMP entry is missing")
}

// TestIDEGenerateUsesCurrentG0B1Sites exercises the real launch arguments after
// bind prepares the corresponding sidecar in memory, just as the documented
// example build does. No arbitrary substitute source or TIL is introduced.
func TestIDEGenerateUsesCurrentG0B1Sites(t *testing.T) {
	launch := readIDELaunch(t)
	for _, config := range launch.Configurations {
		if config.Name != "generate" {
			continue
		}
		source := "examples/G0B1_inst/Core/Src/main.c"
		fs := ideMemoryInputs(t, source, "demoTIL.json", "demoLI.json")
		output, err := runContextCLI(t, fs, "bind", "-src", source,
			"-i", "demoTIL.json", "-li", "demoLI.json", "-genDir", "generated")
		require.NoError(t, err, output)
		output, err = runContextCLI(t, fs, ideArguments(t, launch, config.Args, "")...)
		require.NoError(t, err, output)
		table, err := fs.ReadFile("generated/til.c")
		require.NoError(t, err)
		assert.Contains(t, string(table), "StartDefaultTask")
		assert.Contains(t, string(table), "Fun %x!")
		return
	}
	t.Fatal("C table generator entry is missing")
}

// TestIDEIgnoresPersonalStateButKeepsSharedSettings verifies Git's actual ignore
// semantics. In particular, inline comments in a pattern would silently stop
// matching local workspace files. All Git writes stay in a temporary repository.
func TestIDEIgnoresPersonalStateButKeepsSharedSettings(t *testing.T) {
	if _, err := exec.LookPath("git"); err != nil {
		t.Skip("git is required to check IDE ignore behavior")
	}
	root := t.TempDir()
	require.NoError(t, os.Mkdir(filepath.Join(root, ".idea"), 0o755))
	data, err := os.ReadFile("../../.idea/.gitignore")
	require.NoError(t, err)
	require.NoError(t, os.WriteFile(filepath.Join(root, ".idea", ".gitignore"), data, 0o644))
	output, err := exec.Command("git", "init", "-q", root).CombinedOutput()
	require.NoError(t, err, string(output))
	for _, tc := range []struct {
		name    string
		ignored bool
	}{
		{"workspace.xml", true}, {"tasks.xml", true}, {"usage.statistics.xml", true},
		{"shelf/personal.patch", true}, {"caches/index", true},
		{"libraries/local.xml", true}, {"artifacts/local.xml", true},
		{"misc.xml", false}, {"modules.xml", false}, {"trice.iml", false},
		{"vcs.xml", false}, {"dictionaries/project.xml", false},
		{"dictionaries/personal.xml", true},
		{"inspectionProfiles/Project_Default.xml", false},
		{"codeStyles/Project.xml", false}, {"editor.xml", false}, {"go.imports.xml", false},
	} {
		t.Run(tc.name, func(t *testing.T) {
			// Personal global excludes must not mask the repository rule being checked.
			cmd := exec.Command("git", "-c", "core.excludesFile=", "-C", root, "check-ignore", "--no-index", ".idea/"+tc.name)
			output, err := cmd.CombinedOutput()
			if tc.ignored {
				assert.NoError(t, err, string(output))
				assert.Contains(t, string(output), tc.name)
			} else {
				var exit *exec.ExitError
				require.ErrorAs(t, err, &exit, string(output))
				assert.Equal(t, 1, exit.ExitCode(), "shared setting must not be ignored")
				assert.Empty(t, string(output))
			}
		})
	}
}

// TestIDEG0B1CompilerDiscovery checks portable compiler selection, existing
// include directories and the absence of stale compiler-version overrides.
// If ARM GCC is available, its actual queried defines must describe Cortex-M0+.
func TestIDEG0B1CompilerDiscovery(t *testing.T) {
	for _, variant := range []string{"bare", "inst", "log", "features"} {
		t.Run(variant, func(t *testing.T) {
			workspace := filepath.Join("../../examples", "G0B1_"+variant)
			data, err := os.ReadFile(filepath.Join(workspace, ".vscode/c_cpp_properties.json"))
			require.NoError(t, err)
			var properties struct {
				Configurations []struct {
					CompilerPath                       string
					IntelliSenseMode                   string
					CompilerArgs, Defines, IncludePath []string
				}
			}
			require.NoError(t, json.Unmarshal(data, &properties))
			require.NotEmpty(t, properties.Configurations)
			for _, config := range properties.Configurations {
				require.NotEmpty(t, config.CompilerPath)
				assert.Equal(t, filepath.Base(config.CompilerPath), config.CompilerPath,
					"compiler must be found through PATH, not an installation directory")
				assert.NotContains(t, config.CompilerPath, ":")
				assert.Regexp(t, `^(macos-|linux-|windows-)?gcc-arm$`, config.IntelliSenseMode,
					"IntelliSense must model ARM GCC rather than the host architecture")
				assert.Contains(t, config.Defines, "USE_HAL_DRIVER")
				assert.Contains(t, config.Defines, "STM32G0B1xx")
				for _, define := range config.Defines {
					assert.False(t, strings.HasPrefix(define, "__"),
						"query compiler builtins instead of pinning %q", define)
				}
				for _, include := range config.IncludePath {
					path := strings.ReplaceAll(strings.TrimSuffix(include, "/**"), "${workspaceFolder}", workspace)
					info, err := os.Stat(filepath.FromSlash(path))
					require.NoError(t, err, "include path %q must exist", include)
					assert.True(t, info.IsDir())
				}
				compiler, err := exec.LookPath(config.CompilerPath)
				if err != nil {
					t.Log("ARM GCC unavailable: compiler query requires an installed toolchain")
					continue
				}
				args := append(append([]string{}, config.CompilerArgs...), "-dM", "-E", "-x", "c", "-")
				output, err := exec.Command(compiler, args...).CombinedOutput()
				require.NoError(t, err, string(output))
				assert.Contains(t, string(output), "#define __ARM_ARCH_6M__ 1",
					"compiler query must use the board's CPU rather than GCC's default CPU")
			}
		})
	}
}

// TestIDEJetBrainsModuleReferencesExist checks the active module list rather
// than protecting an unused duplicate filename. Shared preferences stay intact.
func TestIDEJetBrainsModuleReferencesExist(t *testing.T) {
	data, err := os.ReadFile("../../.idea/modules.xml")
	require.NoError(t, err)
	var project struct {
		Modules []struct {
			File string `xml:"filepath,attr"`
		} `xml:"component>modules>module"`
	}
	require.NoError(t, xml.Unmarshal(data, &project))
	require.NotEmpty(t, project.Modules)
	for _, module := range project.Modules {
		path := strings.ReplaceAll(module.File, "$PROJECT_DIR$/", "../../")
		_, err := os.Stat(filepath.FromSlash(path))
		assert.NoError(t, err, "active module reference must resolve")
	}
}
