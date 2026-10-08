// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"flag"
	"fmt"
	"io"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

type versionState struct {
	Version   string
	Commit    string
	Date      string
	Branch    string
	GitState  string
	GitStatus string
	BuiltBy   string
	Verbose   bool
}

// snapshotVersionState captures version-related global state so tests can restore it afterwards.
func snapshotVersionState() versionState {
	return versionState{
		Version:   Version,
		Commit:    Commit,
		Date:      Date,
		Branch:    Branch,
		GitState:  GitState,
		GitStatus: GitStatus,
		BuiltBy:   BuiltBy,
		Verbose:   Verbose,
	}
}

// restoreVersionState restores version-related global state from a saved snapshot.
func restoreVersionState(s versionState) {
	Version = s.Version
	Commit = s.Commit
	Date = s.Date
	Branch = s.Branch
	GitState = s.GitState
	GitStatus = s.GitStatus
	BuiltBy = s.BuiltBy
	Verbose = s.Verbose
}

// TestScVersionDevFallback verifies the expected behavior.
func TestScVersionDevFallback(t *testing.T) {
	old := snapshotVersionState()
	defer restoreVersionState(old)

	Version = ""
	Commit = ""
	Date = ""
	Branch = ""
	GitState = ""
	GitStatus = ""
	BuiltBy = ""
	Verbose = false

	var out bytes.Buffer
	err := scVersion(&out)
	assert.Nil(t, err)
	assert.Equal(t, "version=dev (no build info)\n", out.String())
}

// TestScVersionReleaseOutput verifies the expected behavior.
func TestScVersionReleaseOutput(t *testing.T) {
	old := snapshotVersionState()
	defer restoreVersionState(old)

	Version = "1.2.3"
	Commit = "abc1234"
	Date = "2026-02-21T12:00:00Z"
	Branch = ""
	GitState = "clean"
	GitStatus = ""
	BuiltBy = "ci"
	Verbose = false

	var out bytes.Buffer
	err := scVersion(&out)
	assert.Nil(t, err)
	assert.Equal(t, "version=1.2.3, commit=abc1234, built at 2026-02-21T12:00:00Z (built by ci)\n", out.String())
}

// TestScVersionBranchFallbackAndDirtyList verifies the expected behavior.
func TestScVersionBranchFallbackAndDirtyList(t *testing.T) {
	old := snapshotVersionState()
	defer restoreVersionState(old)

	Version = ""
	Commit = "abc1234"
	Date = "2026-02-21T12:00:00Z"
	Branch = "main"
	GitState = "dirty"
	GitStatus = "M internal/args/handler.go|?? internal/args/new_test.go|"
	BuiltBy = ""
	Verbose = true

	var out bytes.Buffer
	err := scVersion(&out)
	assert.Nil(t, err)
	s := out.String()
	assert.Contains(t, s, "https://github.com/rokath/trice\n")
	assert.Contains(t, s, "branch=main (local modifications at build time), commit=abc1234, built at 2026-02-21T12:00:00Z\n")
	assert.Contains(t, s, "modified files at build time:\n")
	assert.Contains(t, s, "  M internal/args/handler.go\n")
	assert.Contains(t, s, "  ?? internal/args/new_test.go\n")
}

// TestIsLogFlagPassed verifies the expected behavior.
func TestIsLogFlagPassed(t *testing.T) {
	FlagsInit()
	err := fsScLog.Parse([]string{"-ts32", "ms", "-encoding", "CHAR"})
	assert.Nil(t, err)
	assert.True(t, isLogFlagPassed("ts32"))
	assert.True(t, isLogFlagPassed("encoding"))
	assert.False(t, isLogFlagPassed("ts16"))
}

// TestRunLogValidatesPickAndBanBeforeStartingInput verifies that the conflict
// is based on option presence, including empty or unknown selector values.
func TestRunLogValidatesPickAndBanBeforeStartingInput(t *testing.T) {
	originalStartLogLoop := startLogLoop
	t.Cleanup(func() { startLogLoop = originalStartLogLoop })

	tests := []struct {
		name    string
		args    []string
		wantErr bool
	}{
		{name: "pick then ban", args: []string{"-pick", "err", "-ban", "dbg"}, wantErr: true},
		{name: "ban then pick", args: []string{"-ban", "dbg", "-pick", "err"}, wantErr: true},
		{name: "empty pick still conflicts", args: []string{"-pick", "", "-ban", "dbg"}, wantErr: true},
		{name: "unknown ban still conflicts", args: []string{"-ban", "misspelled", "-pick", "err"}, wantErr: true},
		{name: "pick with log level", args: []string{"-pick", "err", "-logLevel", "all"}},
		{name: "ban with log level", args: []string{"-ban", "dbg", "-logLevel", "all"}},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			FlagsInit()
			started := false
			startLogLoop = func(io.Writer, *afero.Afero) error { started = true; return nil }

			err := runLog(io.Discard, &afero.Afero{Fs: afero.NewMemMapFs()}, tt.args)
			if tt.wantErr {
				require.Error(t, err)
				assert.Contains(t, err.Error(), "-pick and -ban")
				assert.False(t, started)
				return
			}

			require.NoError(t, err)
			assert.True(t, started)
			assert.True(t, len(emitter.Pick) > 0 || len(emitter.Ban) > 0)
		})
	}
}

// TestLogEntryPointsResolveUserSelectorsAfterRegistration verifies option-order
// independence for Pick and Ban through both supported log entry points.
func TestLogEntryPointsResolveUserSelectorsAfterRegistration(t *testing.T) {
	originalStartLogLoop := startLogLoop
	t.Cleanup(func() { startLogLoop = originalStartLogLoop })
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	tests := []struct {
		name string
		args []string
		run  func([]string) error
		pick bool
	}{
		{name: "trice pick before label", args: []string{"-pick", "motor", "-ulabel", "motor"}, run: func(args []string) error {
			return Handler(io.Discard, fSys, append([]string{"trice", "log"}, args...))
		}, pick: true},
		{name: "tlog label before pick", args: []string{"-ulabel", "motor", "-pick", "motor"}, run: func(args []string) error {
			return LogHandler(io.Discard, fSys, append([]string{"tlog"}, args...))
		}, pick: true},
		{name: "trice ban before label", args: []string{"-ban", "motor", "-ulabel", "motor"}, run: func(args []string) error {
			return Handler(io.Discard, fSys, append([]string{"trice", "log"}, args...))
		}},
		{name: "tlog label before ban", args: []string{"-ulabel", "motor", "-ban", "motor"}, run: func(args []string) error {
			return LogHandler(io.Discard, fSys, append([]string{"tlog"}, args...))
		}},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			FlagsInit()
			started := false
			startLogLoop = func(io.Writer, *afero.Afero) error { started = true; return nil }

			require.NoError(t, tt.run(tt.args))
			assert.True(t, started)
			if tt.pick {
				assert.Contains(t, emitter.Pick, "motor")
				assert.Nil(t, emitter.Ban)
			} else {
				assert.Contains(t, emitter.Ban, "motor")
				assert.Nil(t, emitter.Pick)
			}
		})
	}
}

// TestLogEntryPointsRejectInvalidSelectorsBeforeStartingInput verifies the
// shared validation path used by trice log and tlog.
func TestLogEntryPointsRejectInvalidSelectorsBeforeStartingInput(t *testing.T) {
	originalStartLogLoop := startLogLoop
	t.Cleanup(func() { startLogLoop = originalStartLogLoop })
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	entryPoints := []struct {
		name string
		run  func([]string) error
	}{
		{name: "trice log", run: func(args []string) error {
			return Handler(io.Discard, fSys, append([]string{"trice", "log"}, args...))
		}},
		{name: "tlog", run: func(args []string) error {
			return LogHandler(io.Discard, fSys, append([]string{"tlog"}, args...))
		}},
	}
	invalid := []struct {
		name     string
		args     []string
		contains string
	}{
		{name: "unknown pick", args: []string{"-pick", "missing"}, contains: "unknown tag"},
		{name: "empty pick", args: []string{"-pick", ""}, contains: "empty tag name"},
		{name: "double separator", args: []string{"-ban", "err::wrn"}, contains: "empty tag name"},
		{name: "unknown level", args: []string{"-logLevel", "missing"}, contains: "unknown tag"},
		{name: "negative level", args: []string{"-logLevel", "-1"}, contains: "range 0..999"},
		{name: "large level", args: []string{"-logLevel", "1000"}, contains: "range 0..999"},
	}

	for _, entryPoint := range entryPoints {
		for _, tt := range invalid {
			t.Run(entryPoint.name+" "+tt.name, func(t *testing.T) {
				FlagsInit()
				started := false
				startLogLoop = func(io.Writer, *afero.Afero) error { started = true; return nil }

				err := entryPoint.run(tt.args)
				require.Error(t, err)
				assert.Contains(t, err.Error(), tt.contains)
				assert.False(t, started)
			})
		}
	}
}

// decimalByteArguments formats raw input for the BUFFER receiver without
// changing or interpreting any byte values.
func decimalByteArguments(data []byte) string {
	values := make([]string, len(data))
	for i, value := range data {
		values[i] = fmt.Sprint(value)
	}
	return strings.Join(values, " ")
}

// TestBinaryRecordingPrecedesHostFiltering verifies that every host filter
// records the same received bytes and that replay can select a hidden event.
func TestBinaryRecordingPrecedesHostFiltering(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	raw := []byte("dbg:hidden during capture\n")
	common := []string{
		"-port", "BUFFER",
		"-args", decimalByteArguments(raw),
		"-encoding", "CHAR",
		"-til", "emptyFile",
		"-li", "emptyFile",
		"-hs", "off",
		"-prefix", "off",
		"-color", "off",
	}
	tests := []struct {
		name   string
		filter []string
	}{
		{name: "pick", filter: []string{"-pick", "err"}},
		{name: "ban", filter: []string{"-ban", "dbg"}},
		{name: "log level all", filter: []string{"-logLevel", "all"}},
		{name: "log level off", filter: []string{"-logLevel", "off"}},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			emitter.Pick = nil
			emitter.Ban = nil
			FlagsInit()
			logfile := strings.ReplaceAll(tt.name, " ", "-") + ".bin"
			args := append([]string{"trice"}, common...)
			args = append(args, "-binaryLogfile", logfile)
			args = append(args, tt.filter...)

			require.NoError(t, LogHandler(io.Discard, fSys, args))
			logged, err := fSys.ReadFile(logfile)
			require.NoError(t, err)
			assert.Equal(t, raw, logged)
		})
	}

	// The capture hid the dbg event, but replay applies a new selection to the
	// unchanged raw file and does not record back into its own input.
	emitter.Pick = nil
	emitter.Ban = nil
	FlagsInit()
	beforeReplay, err := fSys.ReadFile("pick.bin")
	require.NoError(t, err)
	var replay bytes.Buffer
	require.NoError(t, LogHandler(&replay, fSys, []string{
		"trice",
		"-port", "FILEBUFFER",
		"-args", "pick.bin",
		"-encoding", "CHAR",
		"-til", "emptyFile",
		"-li", "emptyFile",
		"-hs", "off",
		"-prefix", "off",
		"-color", "off",
		"-binaryLogfile", "off",
		"-pick", "dbg",
	}))
	assert.Contains(t, replay.String(), "dbg:hidden during capture")
	afterReplay, err := fSys.ReadFile("pick.bin")
	require.NoError(t, err)
	assert.Equal(t, beforeReplay, afterReplay)
}

// TestVisFlagIsRepeatableAndResetWithLogFlags verifies CLI collection without leaking rules across parses.
func TestVisFlagIsRepeatableAndResetWithLogFlags(t *testing.T) {
	FlagsInit()
	err := fsScLog.Parse([]string{
		"-vis", `msg:printf("%d",v0)@one.txt`,
		"-vis", `imu:printf("%f",v0)@two.txt`,
	})
	assert.Nil(t, err)
	assert.Equal(t, []string{
		`msg:printf("%d",v0)@one.txt`,
		`imu:printf("%f",v0)@two.txt`,
	}, []string(visRules))
	assert.True(t, isLogFlagPassed("vis"))

	FlagsInit()
	assert.Empty(t, visRules)
	assert.False(t, isLogFlagPassed("vis"))
}

// TestInfoHelpersWriteText verifies the expected behavior.
func TestInfoHelpersWriteText(t *testing.T) {
	FlagsInit()
	tt := []struct {
		name   string
		fn     func(io.Writer) error
		marker string
	}{
		{"versionInfo", versionInfo, "sub-command 'ver|version'"},
		{"helpInfo", helpInfo, "sub-command 'h|help'"},
		{"logInfo", logInfo, "sub-command 'l|log'"},
		{"scanInfo", scanInfo, "sub-command 's|scan'"},
		{"displayServerInfo", displayServerInfo, "sub-command 'ds|displayServer'"},
		{"shutdownInfo", shutdownInfo, "sub-command 'sd|shutdown'"},
		{"insertIDsInfo", insertIDsInfo, "sub-command 'i|insert'"},
		{"bindIDsInfo", bindIDsInfo, "sub-command 'b|bind'"},
		{"cleanIDsInfo", cleanIDsInfo, "sub-command 'c|clean'"},
		{"addInfo", addInfo, "sub-command 'a|add'"},
		{"generateInfo", generateInfo, "sub-command 'g|gen|generate'"},
	}
	for _, tc := range tt {
		t.Run(tc.name, func(t *testing.T) {
			var out bytes.Buffer
			err := tc.fn(&out)
			assert.Nil(t, err)
			assert.Contains(t, out.String(), tc.marker)
		})
	}
}

// TestScHelpWithSelectedSections verifies the expected behavior.
func TestScHelpWithSelectedSections(t *testing.T) {
	FlagsInit()
	allHelp = false
	displayServerHelp = true
	helpHelp = true
	defer func() {
		allHelp = false
		displayServerHelp = false
		helpHelp = false
	}()

	var out bytes.Buffer
	err := scHelp(&out)
	assert.Nil(t, err)
	s := out.String()
	assert.Contains(t, s, "syntax: 'trice sub-command' [params]")
	assert.Contains(t, s, "sub-command 'ds|displayServer'")
	assert.Contains(t, s, "sub-command 'h|help'")
}

// TestHandlerVersionSubcommands verifies the expected behavior.
func TestHandlerVersionSubcommands(t *testing.T) {
	old := snapshotVersionState()
	defer restoreVersionState(old)

	FlagsInit()
	Version = "1.0.0"
	Commit = "abc1234"
	Date = "2026-02-21T12:00:00Z"
	Branch = ""
	GitState = "clean"
	GitStatus = ""
	BuiltBy = ""
	Verbose = false

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	for _, cmd := range []string{"ver", "version", "--version", "-version"} {
		var out bytes.Buffer
		err := Handler(&out, fSys, []string{"trice", cmd})
		assert.Nil(t, err)
		assert.True(t, strings.Contains(out.String(), "version=1.0.0"))
	}
}

// TestHandlerGenerateWithoutParameters verifies the expected behavior.
func TestHandlerGenerateWithoutParameters(t *testing.T) {
	FlagsInit()
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	var out bytes.Buffer
	err := Handler(&out, fSys, []string{"trice", "generate"})
	assert.Nil(t, err)
	assert.Contains(t, out.String(), `The "trice generate" command needs at least one parameter.`)
}

// TestHandlerGenerateOneLineJSON verifies that the public generate command
// accepts both dictionary paths and writes their views under the shared default.
func TestHandlerGenerateOneLineJSON(t *testing.T) {
	FlagsInit()
	t.Cleanup(FlagsInit)
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	til := []byte("{\"123\":{\"Type\":\"trice\",\"Strg\":\"msg:hello\"}}")
	li := []byte("{\"123\":{\"File\":\"src/main.c\",\"Line\":17}}")
	require.NoError(t, fSys.WriteFile("custom-til.json", til, 0o600))
	require.NoError(t, fSys.WriteFile("custom-li.json", li, 0o600))

	var output bytes.Buffer
	require.NoError(t, Handler(&output, fSys, []string{"trice", "generate", "-onelineJSON", "-til", "custom-til.json", "-li", "custom-li.json"}))
	tilView, err := fSys.ReadFile("generated/custom-til.oneline.json")
	require.NoError(t, err)
	liView, err := fSys.ReadFile("generated/custom-li.oneline.json")
	require.NoError(t, err)
	legacyViewExists, err := fSys.Exists("custom-til.oneline.json")
	require.NoError(t, err)
	assert.False(t, legacyViewExists)
	assert.Contains(t, string(tilView), "\"123\": {\"Type\":\"trice\",\"Strg\":\"msg:hello\"}")
	assert.Contains(t, string(liView), "\"123\": {\"Line\":17,\"File\":\"src/main.c\"}")
	unchangedTIL, err := fSys.ReadFile("custom-til.json")
	require.NoError(t, err)
	unchangedLI, err := fSys.ReadFile("custom-li.json")
	require.NoError(t, err)
	assert.Equal(t, til, unchangedTIL)
	assert.Equal(t, li, unchangedLI)
}

// TestHandlerBindReportKeepsOutputReadOnly exercises public flags on a read-only
// filesystem, including custom scope and dictionaries that do not exist.
func TestHandlerBindReportKeepsOutputReadOnly(t *testing.T) {
	FlagsInit()
	previousSrcs, previousExclusions := id.Srcs, id.ExcludeSrcs
	previousLogfile := LogfileName
	t.Cleanup(func() {
		FlagsInit()
		id.Srcs, id.ExcludeSrcs = previousSrcs, previousExclusions
		LogfileName = previousLogfile
	})
	id.Srcs, id.ExcludeSrcs = nil, nil
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, fs.MkdirAll("app", 0o755))
	require.NoError(t, fs.MkdirAll("artifacts", 0o755))
	const owner = "trice_main_c_K1111111111111111.h"
	require.NoError(t, fs.WriteFile("app/main.c", []byte("#include \""+owner+"\"\n"), 0o644))
	require.NoError(t, fs.WriteFile("app/excluded.c", []byte("#include \""+owner+"\"\n"), 0o644))
	require.NoError(t, fs.WriteFile("artifacts/"+owner, []byte("#define TRICE_BIND_FILE_KEY K1111111111111111\n#define TRICE_BIND_ROUTE_K1111111111111111 BIND\n"), 0o644))
	// A previous command may have selected a logfile. Report mode must not open
	// that sink or invoke general output setup, even with verbosity enabled.
	LogfileName = "must-not-be-created.log"
	var output bytes.Buffer
	require.NoError(t, Handler(&output, &afero.Afero{Fs: afero.NewReadOnlyFs(fs.Fs)}, []string{
		"trice", "generate", "-bindReport", "-genDir", "artifacts", "-src", "app", "-exclude", "app/excluded.c", "-til", "absent-til.json", "-li", "absent-li.json", "-v",
	}))
	assert.Contains(t, output.String(), "Bind artifact report: artifacts")
	assert.Contains(t, output.String(), "Referenced in scan: app/main.c")
	assert.NotContains(t, output.String(), "AMBIGUOUS")
	for _, path := range []string{"must-not-be-created.log", "absent-til.json", "absent-li.json", "generated"} {
		exists, err := fs.Exists(path)
		require.NoError(t, err)
		assert.False(t, exists, "report mode must not create %s", path)
	}
}

// TestHandlerBindReportRejectsOtherModes verifies that the CLI cannot silently
// turn the promised read-only report into a generating command.
func TestHandlerBindReportRejectsOtherModes(t *testing.T) {
	for _, option := range []string{"-logC", "-onelineJSON", "-abc=device", "-colors"} {
		t.Run(option, func(t *testing.T) {
			FlagsInit()
			t.Cleanup(FlagsInit)
			fs := &afero.Afero{Fs: afero.NewReadOnlyFs(afero.NewMemMapFs())}
			var output bytes.Buffer
			err := Handler(&output, fs, []string{"trice", "generate", "-bindReport", option})
			require.ErrorContains(t, err, "cannot be combined")
			assert.Empty(t, output.String())
		})
	}
}

// TestNormalizeGenerateLogCPath verifies both documented optional-path forms
// without retaining the removed -tilC compatibility spelling.
func TestNormalizeGenerateLogCPath(t *testing.T) {
	assert.Equal(t, []string{"-logC=build/til.c", "-src", "src"}, normalizeGenerateArgs([]string{"-logC", "build/til.c", "-src", "src"}))
	assert.Equal(t, []string{"-logC=build/til.c"}, normalizeGenerateArgs([]string{"-logC=build/til.c"}))
	assert.Equal(t, []string{"-tilC", "old.c"}, normalizeGenerateArgs([]string{"-tilC", "old.c"}))
}

// TestGeneratedDirectoryFlagsUseOneDefault ensures every ID-producing command
// exposes the same path and the superseded spelling is unavailable.
func TestGeneratedDirectoryFlagsUseOneDefault(t *testing.T) {
	FlagsInit()
	t.Cleanup(FlagsInit)
	for _, command := range []struct {
		name string
		set  *flag.FlagSet
	}{
		{"bind", fsScBind},
		{"insert", fsScInsert},
		{"generate", fsScGenerate},
	} {
		t.Run(command.name, func(t *testing.T) {
			generated := command.set.Lookup("genDir")
			require.NotNil(t, generated)
			assert.Equal(t, id.DefaultGenDir, generated.DefValue)
			assert.Nil(t, command.set.Lookup("buildDir"))
		})
	}
}

// TestHandlerAddInsertCleanOnMissingSource verifies the expected behavior.
func TestHandlerAddInsertCleanOnMissingSource(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	assert.Nil(t, fSys.WriteFile("til.json", []byte("{}"), 0o644))
	assert.Nil(t, fSys.WriteFile("li.json", []byte("{}"), 0o644))
	id.FnJSON = "til.json"
	id.LIFnJSON = "li.json"
	tests := [][]string{
		{"trice", "add", "-src", "missing-source-tree"},
		{"trice", "insert", "-src", "missing-source-tree"},
		{"trice", "bind", "-src", "missing-source-tree"},
		{"trice", "clean", "-src", "missing-source-tree"},
	}
	for _, args := range tests {
		FlagsInit() // reset global flag sets before each parse
		var out bytes.Buffer
		err := Handler(&out, fSys, args)
		assert.Nil(t, err)
		assert.Contains(t, out.String(), "missing-source-tree does not exist!")
	}
}

// TestLogLocationPathFlags verifies the log-specific directory display limit.
func TestLogLocationPathFlags(t *testing.T) {
	oldMaxDirs := id.LIMaxDirs
	t.Cleanup(func() {
		id.LIMaxDirs = oldMaxDirs
	})

	FlagsInit()
	assert.Nil(t, fsScLog.Lookup("liPath"))
	assert.Nil(t, fsScLog.Lookup("liRoot"))
	assert.Equal(t, "0", fsScLog.Lookup("liMaxDirs").DefValue)
	assert.NoError(t, fsScLog.Parse([]string{"-liMaxDirs", "3"}))
	assert.Equal(t, 3, id.LIMaxDirs)
}

// TestGenerationLocationRootFlags verifies the shared root flag on all ID commands.
func TestGenerationLocationRootFlags(t *testing.T) {
	oldRoot := id.LIRoot
	t.Cleanup(func() { id.LIRoot = oldRoot })

	FlagsInit()
	for _, flagSet := range []*flag.FlagSet{fsScAdd, fsScInsert, fsScBind, fsScClean} {
		assert.Nil(t, flagSet.Lookup("liPath"))
		assert.Equal(t, "", flagSet.Lookup("liRoot").DefValue)
	}
	assert.NoError(t, fsScInsert.Parse([]string{"-liRoot", "project"}))
	assert.Equal(t, "project", id.LIRoot)
}

// TestHandlerBindAndGenerateShareGenDir checks that bind and generate use the
// same selected directory for sidecars, the field registry, and the C table.
func TestHandlerBindAndGenerateShareGenDir(t *testing.T) {
	FlagsInit()
	t.Cleanup(FlagsInit)
	oldSources := append(id.ArrayFlag(nil), id.Srcs...)
	id.Srcs = nil
	t.Cleanup(func() { id.Srcs = oldSources })
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	assert.NoError(t, fSys.MkdirAll("project", 0o755))
	assert.NoError(t, fSys.WriteFile("til.json", []byte("{}\n"), 0o644))
	assert.NoError(t, fSys.WriteFile("li.json", []byte("{}\n"), 0o644))
	const source = "trice(\"msg:handler=%d\\n\", 1);\n"
	assert.NoError(t, fSys.WriteFile("project/module.c", []byte(source), 0o644))

	var output bytes.Buffer
	err := Handler(&output, fSys, []string{
		"trice", "bind",
		"-src", "project/module.c",
		"-til", "til.json",
		"-li", "li.json",
		"-genDir", "generated/triceIDs",
		"-IDMin", "100",
		"-IDMax", "199",
		"-IDMethod", "upward",
		"-defaultStampSize", "16",
	})
	assert.NoError(t, err)
	bound, readErr := fSys.ReadFile("project/module.c")
	assert.NoError(t, readErr)
	assert.Contains(t, string(bound), `#include "trice_module_c_K`)
	entries, readErr := fSys.ReadDir("generated/triceIDs")
	assert.NoError(t, readErr)
	assert.Len(t, entries, 2, "sidecar plus the current-run field registry")
	fields, readErr := fSys.ReadFile("generated/triceIDs/trice-fields.txt")
	assert.NoError(t, readErr)
	assert.Empty(t, fields, "classic printf logs introduce no user fields")

	// Repeatable source selections live in package state, so model a fresh CLI call.
	id.Srcs = nil
	FlagsInit()
	output.Reset()
	err = Handler(&output, fSys, []string{"trice", "generate", "-src", "project/module.c", "-til", "til.json", "-genDir", "generated/triceIDs", "-logC"})
	require.NoError(t, err)
	generated, readErr := fSys.ReadFile("generated/triceIDs/til.c")
	require.NoError(t, readErr)
	assert.Contains(t, string(generated), "msg:handler=%d\\n", "generate resolves the bind sidecar in the selected generated directory")

	// An explicit output path overrides -genDir without changing sidecar lookup.
	id.Srcs = nil
	FlagsInit()
	output.Reset()
	err = Handler(&output, fSys, []string{"trice", "generate", "-src", "project/module.c", "-til", "til.json", "-genDir", "generated/triceIDs", "-logC=custom/trace.c"})
	require.NoError(t, err)
	explicit, readErr := fSys.ReadFile("custom/trace.c")
	require.NoError(t, readErr)
	assert.Contains(t, string(explicit), "msg:handler=%d\\n")
	assert.Contains(t, string(explicit), "File: custom/trace.c")
}

// TestHandlerBindRejectsOldDirectoryFlags confirms obsolete spellings fail
// before a source, dictionary, or generated artifact can be changed.
func TestHandlerBindRejectsOldDirectoryFlags(t *testing.T) {
	FlagsInit()
	t.Cleanup(FlagsInit)
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, fSys.MkdirAll("project", 0o755))
	const source = `trice("{motor_id}", motor_id);`
	require.NoError(t, fSys.WriteFile("project/module.c", []byte(source), 0o644))
	for _, obsolete := range []string{"-bindDir", "-buildDir"} {
		var output bytes.Buffer
		err := Handler(&output, fSys, []string{"trice", "bind", "-src", "project/module.c", obsolete, "generated/triceIDs"})
		assert.ErrorContains(t, err, "flag provided but not defined: "+obsolete)
	}
	actual, readErr := fSys.ReadFile("project/module.c")
	assert.NoError(t, readErr)
	assert.Equal(t, source, string(actual))
	for _, path := range []string{"generated/triceIDs", "til.json", "li.json"} {
		exists, statErr := fSys.Exists(path)
		assert.NoError(t, statErr)
		assert.False(t, exists, "%s must not be created after an invalid flag", path)
	}
}

// TestHandlerInsertGenDirPreservesRegistryOnDryRun checks the public option
// path and ensures a preview cannot replace the last published field counts.
func TestHandlerInsertGenDirPreservesRegistryOnDryRun(t *testing.T) {
	FlagsInit()
	t.Cleanup(FlagsInit)
	oldSources := append(id.ArrayFlag(nil), id.Srcs...)
	t.Cleanup(func() { id.Srcs = oldSources })
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, fSys.MkdirAll("project", 0o755))
	require.NoError(t, fSys.WriteFile("til.json", []byte("{}\n"), 0o644))
	require.NoError(t, fSys.WriteFile("li.json", []byte("{}\n"), 0o644))
	require.NoError(t, fSys.WriteFile("project/module.c", []byte(`trice("{old}", value);`), 0o644))
	options := []string{"trice", "insert", "-src", "project/module.c", "-til", "til.json", "-li", "li.json", "-genDir", "generated/triceIDs", "-IDMin", "100", "-IDMax", "199", "-IDMethod", "upward"}
	run := func(dryRun bool) error {
		// Repeated -src values are process-global; each run models a fresh CLI invocation.
		id.Srcs = nil
		FlagsInit()
		args := append([]string{}, options...)
		if dryRun {
			args = append(args, "-dry-run")
		}
		return Handler(io.Discard, fSys, args)
	}
	require.NoError(t, run(false))
	registryPath := "generated/triceIDs/trice-fields.txt"
	registry, err := fSys.ReadFile(registryPath)
	require.NoError(t, err)
	assert.Equal(t, "       1 old\n", string(registry))
	assert.NoError(t, fSys.WriteFile("project/module.c", []byte(`trice("{new}", value);`), 0o644))
	require.NoError(t, run(true))
	registry, err = fSys.ReadFile(registryPath)
	require.NoError(t, err)
	assert.Equal(t, "       1 old\n", string(registry))
	require.NoError(t, run(false))
	registry, err = fSys.ReadFile(registryPath)
	require.NoError(t, err)
	assert.Equal(t, "       1 new\n", string(registry))
}

// TestHandlerBindImplicitSourceHint verifies that a failed recursive default
// scan explains its scope, while the same failure under an explicit source
// selection does not add the hint.
func TestHandlerBindImplicitSourceHint(t *testing.T) {
	oldSrcs := append(id.ArrayFlag(nil), id.Srcs...)
	oldLogfileName := LogfileName
	t.Cleanup(func() {
		id.Srcs = oldSrcs
		LogfileName = oldLogfileName
		FlagsInit()
	})
	LogfileName = "off"

	for _, test := range []struct {
		name       string
		sourceArgs []string
		wantHint   bool
	}{
		{name: "implicit recursive root", wantHint: true},
		{name: "explicit source", sourceArgs: []string{"-src", "broken.c"}, wantHint: false},
	} {
		t.Run(test.name, func(t *testing.T) {
			id.Srcs = nil
			FlagsInit()
			fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
			assert.NoError(t, fSys.WriteFile("til.json", []byte("{}\n"), 0o644))
			assert.NoError(t, fSys.WriteFile("li.json", []byte("{}\n"), 0o644))
			assert.NoError(t, fSys.WriteFile("broken.c", []byte("trice(value);\n"), 0o644))

			args := []string{"trice", "bind", "-til", "til.json", "-li", "li.json"}
			args = append(args, test.sourceArgs...)
			var output bytes.Buffer
			err := Handler(&output, fSys, args)
			assert.Error(t, err)
			if test.wantHint {
				assert.Contains(t, output.String(), implicitBindSourceHint)
			} else {
				assert.NotContains(t, output.String(), implicitBindSourceHint)
			}
		})
	}
}

// TestHandlerBindHelp verifies that both public names and both standard help flags exit cleanly.
func TestHandlerBindHelp(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	for _, subcommand := range []string{"bind", "b"} {
		for _, helpFlag := range []string{"-h", "--help"} {
			FlagsInit()
			var output bytes.Buffer
			err := Handler(&output, fSys, []string{"trice", subcommand, helpFlag})
			assert.NoError(t, err)
			assert.Contains(t, output.String(), "Usage of bind:")
			assert.NotContains(t, output.String(), "flag: help requested")
		}
	}
}
