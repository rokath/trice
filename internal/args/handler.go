// SPDX-License-Identifier: MIT

package args

import (
	"errors"
	"flag"
	"fmt"
	"io"
	"os"
	"runtime/debug"
	"strings"
	"sync"
	"time"

	"github.com/kr/pretty"
	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/do"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/translator"
	"github.com/rokath/trice/internal/vis"
	"github.com/rokath/trice/pkg/cipher"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// implicitBindSourceHint explains the recursive default only after it caused a
// failed bind. Successful commands remain silent, and an explicitly selected
// "./" does not receive a redundant hint.
const implicitBindSourceHint = `
============================ TRICE BIND HINT ============================
No -src was specified, so trice bind recursively scanned "./".
If this directory contains independent projects or verification sources,
restrict the selection with one or more -src or -exclude options.
=========================================================================`

// cliUsageError identifies invalid arguments without treating runtime failures
// as usage errors. Public entry points add the help command for their executable.
type cliUsageError struct {
	command string // canonical subcommand, or empty when no command was selected
	cause   error  // original diagnostic, retained for errors.Is and errors.As
}

// Error preserves the underlying diagnostic until the public entry point adds its hint.
func (e *cliUsageError) Error() string { return e.cause.Error() }

// Unwrap retains the original error for callers inspecting a failed invocation.
func (e *cliUsageError) Unwrap() error { return e.cause }

// withCLIHelp adds exactly one actionable hint to usage errors. Runtime errors
// pass through unchanged, and tlog never advertises unavailable trice subcommands.
func withCLIHelp(err error, loggingOnly bool) error {
	var usage *cliUsageError
	if !errors.As(err, &usage) {
		return err
	}
	helpCommand := "trice help"
	if loggingOnly {
		helpCommand = "tlog help"
	} else if usage.command != "" {
		helpCommand += " -" + strings.ToLower(usage.command)
	}
	return fmt.Errorf("%w\nUse %s for CLI documentation.", err, helpCommand)
}

// parseCommandFlags reports errors through the caller instead of printing the
// flag package's automatic usage dump. Explicit help prints full usage and stops
// successfully; errors and unexpected positional arguments stop before any work.
func parseCommandFlags(w io.Writer, flags *flag.FlagSet, arguments []string) (stop bool, err error) {
	// Help renderers also use these flag sets, so restore their output after parsing.
	previousOutput := flags.Output()
	flags.SetOutput(io.Discard)
	defer flags.SetOutput(previousOutput)
	err = flags.Parse(arguments)
	if errors.Is(err, flag.ErrHelp) {
		flags.SetOutput(w)
		flags.Usage()
		return true, nil
	}
	if err != nil {
		return true, &cliUsageError{command: flags.Name(), cause: err}
	}
	if flags.NArg() != 0 {
		return true, &cliUsageError{command: flags.Name(), cause: fmt.Errorf("unexpected argument %q", flags.Arg(0))}
	}
	return false, nil
}

// Handler parses args and executes the selected sub-command.
//
// args is expected in os.Args form where args[0] is the executable name and
// args[1] is the sub-command.
func Handler(w io.Writer, fSys *afero.Afero, args []string) (err error) {
	defer func() { err = withCLIHelp(err, false) }()

	// Trim leading and trailing whitespace
	//for i := range args {
	//	args[i] = strings.TrimSpace(args[i])
	//}

	ensureDate(fSys)

	// Verify that a sub-command has been provided: os.Arg[0] is the main command (trice), os.Arg[1] will be the sub-command.
	if len(args) < 2 {
		return &cliUsageError{cause: errors.New("no sub-command provided")}
	}
	if isVersionSwitch(args[1]) {
		return versionHandler(w, fSys, args[2:])
	}

	// Switch on the sub-command. Parse the flags for appropriate FlagSet.
	// FlagSet.Parse() requires a set of arguments to parse as input.
	// os.Args[2:] will be all arguments starting after the sub-command at os.Args[1]
	subCmd := args[1]
	subArgs := args[2:]
	switch subCmd { // Check which sub-command is invoked.
	default:
		return &cliUsageError{cause: fmt.Errorf("unknown sub-command '%s'", subCmd)}
	case "-h", "-help", "--help":
		if len(subArgs) != 0 {
			return &cliUsageError{cause: fmt.Errorf("unexpected argument %q", subArgs[0])}
		}
		return helpInfo(w)
	case "h", "help":
		if stop, err := parseCommandFlags(w, fsScHelp, subArgs); stop {
			return err
		}
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return scHelp(w)
	case "s", "scan":
		if stop, err := parseCommandFlags(w, fsScScan, subArgs); stop {
			return err
		}
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		_, err := com.GetSerialPorts(w)
		return err
	case "ver", "version":
		return versionHandler(w, fSys, subArgs)
	case "a", "add":
		if stop, err := parseCommandFlags(w, fsScAdd, subArgs); stop {
			return err
		}
		id.Verbose = Verbose
		id.CompactSrcs()
		id.ProcessAliases()
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return id.SubCmdIdAdd(w, fSys)
	case "generate":
		if stop, err := parseCommandFlags(w, fsScGenerate, normalizeGenerateArgs(subArgs)); stop {
			return err
		}
		id.Verbose = Verbose
		id.CompactSrcs()
		id.ProcessAliases()
		// Inventory inspection bypasses output setup, which can open a logfile
		// or connect to a sink. Its only output is the caller-provided writer.
		if id.GenerateBindReport {
			return id.SubCmdGenerate(w, fSys)
		}
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return id.SubCmdGenerate(w, fSys)
	case "i", "insert":
		if stop, err := parseCommandFlags(w, fsScInsert, subArgs); stop {
			return err
		}
		id.Verbose = Verbose
		id.CompactSrcs()
		id.ProcessAliases()
		if err := emitter.AddUserLabels(); err != nil {
			return &cliUsageError{command: "insert", cause: err}
		}
		err := id.EvaluateIDRangeStrings()
		if err != nil {
			return &cliUsageError{command: "insert", cause: err}
		}
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		//fmt.Println(id.StructuredLoggingFormatString)
		//fmt.Println(id.StructuredLoggingValuesString)
		//return err
		return id.SubCmdIdInsert(w, fSys)
	case "b", "bind":
		if stop, err := parseCommandFlags(w, fsScBind, subArgs); stop {
			return err
		}
		id.Verbose = Verbose
		// Remember why CompactSrcs adds "./". Once expanded, the default is
		// indistinguishable from a deliberate `-src ./` selection.
		implicitSourceSelection := len(id.Srcs) == 0
		id.CompactSrcs()
		id.ProcessAliases()
		if err := emitter.AddUserLabels(); err != nil {
			return &cliUsageError{command: "bind", cause: err}
		}
		if err := id.EvaluateIDRangeStrings(); err != nil {
			return &cliUsageError{command: "bind", cause: err}
		}
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		err := id.SubCmdIdBind(w, fSys)
		if err != nil && implicitSourceSelection {
			fmt.Fprintln(w, implicitBindSourceHint)
		}
		return err
	case "c", "clean":
		if stop, err := parseCommandFlags(w, fsScClean, subArgs); stop {
			return err
		}
		id.CompactSrcs()
		id.ProcessAliases()
		if err := emitter.AddUserLabels(); err != nil {
			return &cliUsageError{command: "clean", cause: err}
		}
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return id.SubCmdIdClean(w, fSys)
	case "sd", "shutdown":
		if stop, err := parseCommandFlags(w, fsScSdSv, subArgs); stop {
			return err
		}
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		return emitter.ScShutdownRemoteDisplayServer(w, 0) // 0|1: 0=no 1=with shutdown timestamp in display server
	case "ds", "displayServer":
		if stop, err := parseCommandFlags(w, fsScSv, subArgs); stop {
			return err
		}
		w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
		// The server applies its own palette to incoming, uncolored log fragments.
		if err := emitter.AddUserLabels(); err != nil {
			return &cliUsageError{command: "displayServer", cause: err}
		}
		return emitter.ScDisplayServer(w) // endless loop
	case "l", "log":
		return handleLog(w, fSys, append([]string{args[0]}, subArgs...))
	}
}

// LogHandler parses log args and executes the log command.
//
// args is expected in os.Args form where args[0] is the executable name and
// args[1:] are the log command flags. This makes `tlog ...` use the same code
// path as `trice log ...` without exposing the other trice sub-commands.
func LogHandler(w io.Writer, fSys *afero.Afero, args []string) error {
	return withCLIHelp(handleLog(w, fSys, args), true)
}

// handleLog shares log, help and version handling without choosing an executable's help hint.
func handleLog(w io.Writer, fSys *afero.Afero, args []string) error {
	ensureDate(fSys)
	if len(args) == 0 {
		args = []string{"tlog"}
	}
	if len(args) > 1 {
		switch args[1] {
		case "ver", "version":
			return versionHandler(w, fSys, args[2:])
		case "h", "help":
			if len(args) > 2 {
				return &cliUsageError{command: "log", cause: fmt.Errorf("unexpected argument %q", args[2])}
			}
			return logInfo(w)
		default:
			if isVersionSwitch(args[1]) {
				return versionHandler(w, fSys, args[2:])
			}
		}
	}
	return runLog(w, fSys, args[1:])
}

func normalizeGenerateArgs(args []string) []string {
	out := make([]string, 0, len(args))
	for i := 0; i < len(args); i++ {
		if args[i] == "-logC" && i+1 < len(args) && !strings.HasPrefix(args[i+1], "-") {
			out = append(out, "-logC="+args[i+1])
			i++
			continue
		}
		out = append(out, args[i])
	}
	return out
}

func isVersionSwitch(arg string) bool {
	return arg == "--version" || arg == "-version"
}

func versionHandler(w io.Writer, fSys *afero.Afero, subArgs []string) error {
	if stop, err := parseCommandFlags(w, fsScVersion, subArgs); stop {
		return err
	}
	w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
	if Verbose {
		bi, ok := debug.ReadBuildInfo()
		if !ok {
			fmt.Println("buildInfo not ok")
		} else {
			pretty.Println(bi)
		}
	}
	return scVersion(w)
}

func ensureDate(fSys *afero.Afero) {
	if Date != "" { // goreleaser will set Date, otherwise use file info.
		return
	}
	path, err := os.Executable()
	if err != nil {
		Date = fmt.Sprint(err) // just in case, simply show the error
		return
	}
	fi, err := fSys.Stat(path)
	if err == nil { // On running main tests file-info is invalid, so do not use it in that case.
		Date = fi.ModTime().String()
		return
	}
	Date = fmt.Sprint(err) // just in case, simply show the error
}

func runLog(w io.Writer, fSys *afero.Afero, subArgs []string) error {
	// A later invocation in the same process starts with the documented default.
	decoder.LogFormat = "text"
	if stop, err := parseCommandFlags(w, fsScLog, subArgs); stop {
		return err
	}
	// Downstream presentation uses canonical names for its format comparisons.
	switch strings.ToLower(decoder.LogFormat) {
	case "text", "json", "kv":
		decoder.LogFormat = strings.ToLower(decoder.LogFormat)
	case "key-value":
		decoder.LogFormat = "kv"
	default:
		return &cliUsageError{command: "log", cause: fmt.Errorf("invalid -logFormat %q: expected text, json, kv, or key-value", decoder.LogFormat)}
	}
	if decoder.LogFormat != "text" && !strings.EqualFold(translator.Encoding, "TREX") {
		return &cliUsageError{command: "log", cause: fmt.Errorf("-logFormat %s requires -encoding TREX because CHAR/DUMP have no event boundaries", decoder.LogFormat)}
	}
	if decoder.LogFormat != "text" && (emitter.DisplayRemote || decoder.TestTableMode) {
		return &cliUsageError{command: "log", cause: fmt.Errorf("-logFormat %s cannot use the text-only remote display or test-table output", decoder.LogFormat)}
	}
	if isLogFlagPassed("pick") && isLogFlagPassed("ban") {
		return &cliUsageError{command: "log", cause: errors.New("switches -pick and -ban cannot be used together")}
	}
	id.Logging = true
	id.ProcessAliases()
	if err := emitter.AddUserLabels(); err != nil {
		return &cliUsageError{command: "log", cause: err}
	}
	if err := emitter.ResolveFilterSelectors(); err != nil {
		return &cliUsageError{command: "log", cause: err}
	}
	decoder.TargetTimeStampUnitPassed = isLogFlagPassed("ts")
	decoder.ShowTargetStamp32Passed = isLogFlagPassed("ts32")
	decoder.ShowTargetStamp16Passed = isLogFlagPassed("ts16")
	decoder.ShowTargetStamp0Passed = isLogFlagPassed("ts0")
	decoder.ShowTargetStamp32DeltaPassed = isLogFlagPassed("ts32delta")
	decoder.ShowTargetStamp16DeltaPassed = isLogFlagPassed("ts16delta")
	decoder.ShowTargetStamp0DeltaPassed = isLogFlagPassed("ts0delta")
	if err := translator.PrepareTargetStampFormats(); err != nil {
		return &cliUsageError{command: "log", cause: err}
	}
	w = do.DistributeArgs(w, fSys, LogfileName, Verbose)
	return startLogLoop(w, fSys) // endless loop until EOF or a returned failure
}

// startLogLoop starts the input/output path after all command-line validation.
// Tests replace it to prove invalid options cannot open an input channel.
var startLogLoop = logLoop

// https://stackoverflow.com/questions/35809252/check-if-flag-was-provided-in-go
func isLogFlagPassed(name string) bool {
	found := false
	fsScLog.Visit(func(f *flag.Flag) {
		if f.Name == name {
			found = true
		}
	})
	return found
}

type selector struct {
	flag bool
	info func(io.Writer) error
}

// logLoop prepares writing and lut and provides a retry mechanism for unplugged UART.
func logLoop(w io.Writer, fSys *afero.Afero) error {
	applicationOutput := w
	if decoder.LogFormat != "text" {
		w = os.Stderr
	}
	msg.FatalOnErr(cipher.SetUp(w)) // does nothing when -password is ""
	if decoder.TestTableMode {
		// set switches if they not set already
		// trice l -ts off -prefix " }, ``" -suffix "\n``}," -color off
		if emitter.HostStamp == "LOCmicro" {
			emitter.HostStamp = "off"
		}
		if defaultPrefix == emitter.Prefix {
			emitter.Prefix = " }, `"
		}
		if emitter.Suffix == "" {
			emitter.Suffix = "`},"
		}
		if emitter.ColorPalette == "default" || emitter.ColorPalette == "dark" {
			emitter.ColorPalette = "off"
		}
		decoder.TargetStamp32 = "" // todo: justify this line
	}

	var ilu id.TriceIDLookUp
	if id.FnJSON == "emptyFile" { // reserved name for tests only
		ilu = make(id.TriceIDLookUp)
	} else {
		ilu = id.NewLut(w, fSys, id.FnJSON) // lut is a map, that means a pointer
		decoder.IDLUT = ilu
	}
	m := new(sync.RWMutex) // m is a pointer to a read write mutex for lu
	m.Lock()
	ilu.AddFmtCount(w)
	m.Unlock()

	if len(visRules) > 0 && !strings.EqualFold(translator.Encoding, "TREX") {
		msg.FatalOnErr(fmt.Errorf("-vis requires -encoding TREX, got %q", translator.Encoding))
	}
	visRouter, err := vis.NewRouter(w, fSys, ilu, m, []string(visRules), Verbose)
	msg.FatalOnErr(err)
	if visRouter != nil {
		defer func() { msg.OnErr(visRouter.Close()) }()
	}
	var li id.TriceIDLookUpLI // nil

	if id.LIFnJSON == "emptyFile" { // reserved name for tests only
		li = make(id.TriceIDLookUpLI) // li is not nil, but empty
	} else {
		if _, err := fSys.Stat(id.LIFnJSON); errors.Is(err, os.ErrNotExist) {
			if id.LIFnJSON != "off" && id.LIFnJSON != "none" && id.LIFnJSON != "no" {
				if Verbose {
					fmt.Fprintf(w, "path/to/ %s does not exist: li is nil\n", id.LIFnJSON)
				}
			}
		} else {
			li = id.NewLutLI(w, fSys, id.LIFnJSON) // lut is a map, that means a pointer
		}
	}
	// Decoder, presentation, and statistics share one lock for live table reads.
	decoder.IDLUT, decoder.LILUT, decoder.LUTMutex = ilu, li, m
	// Watcher diagnostics always use stderr, including text mode, so asynchronous
	// warnings cannot corrupt application records or write into an unsafe sink.
	stopWatching, watchErr := id.WatchLookUpTables(os.Stderr, fSys, ilu, li, m)
	defer stopWatching()
	if watchErr != nil {
		fmt.Fprintf(os.Stderr, "warning: automatic table reload unavailable: %v; restart trice log to load affected tables\n", watchErr)
	}

	sw := emitter.New(applicationOutput)
	var interrupted bool
	var counter int

	for {
		rwc, e := receiver.NewReadWriteCloser(w, fSys, Verbose, receiver.Port, receiver.PortArguments)
		if e != nil {
			if !interrupted && decoder.LogFormat != "text" {
				return e // Machine commands report a failed input setup to their caller.
			}
			fmt.Fprintln(w, e)
			if !interrupted {
				return nil // preserve the existing text command's diagnostic-only result
			}
			time.Sleep(1000 * time.Millisecond) // retry interval
			fmt.Fprintf(w, "\rsig:(re-)setup input port...%d", counter)
			counter++
			continue
		}
		interrupted = true
		if receiver.ShowInputBytes {
			rwc = receiver.NewBytesViewer(w, rwc)
		}
		if receiver.BinaryLogfileName != "off" && receiver.BinaryLogfileName != "none" {
			rwc = receiver.NewBinaryLogger(w, fSys, rwc)
		}
		e = translator.Translate(applicationOutput, sw, ilu, m, li, rwc, visRouter)
		// Close the final wrapper chain before retrying or returning; a defer in
		// this loop retained every previous input and never closed wrapper files.
		closeErr := rwc.Close()
		if closeErr != nil {
			if e == io.EOF {
				e = nil
			}
			return errors.Join(e, closeErr)
		}
		if io.EOF == e {
			return nil // end of predefined buffer
		}
		if e != nil {
			return e
		}
	}
}

// scVersion is sub-command 'version'. It prints version information.
func scVersion(w io.Writer) error {
	if Verbose {
		fmt.Fprintln(w, "https://github.com/rokath/trice")
	}

	// Fallback if nothing has been set (e.g., go install without ldflags)
	if Version == "" && Commit == "" && Date == "" && Branch == "" {
		fmt.Fprintln(w, "version=dev (no build info)")
		return nil
	}

	dirtyText := ""
	if GitState == "dirty" {
		dirtyText = " (local modifications at build time)"
	}

	// If version is set (GoReleaser or local build)
	if Version != "" {
		// GoReleaser builds usually don't set Branch/GitState/GitStatus.
		// For those, we omit the branch to avoid "branch=" being empty.
		if Branch != "" {
			fmt.Fprintf(w, "no version, branch=%s%s, commit=%s, built at %s", Branch, dirtyText, Commit, Date)
		} else {
			fmt.Fprintf(w, "version=%s, commit=%s, built at %s",
				Version, Commit, Date)
		}

		// Optional: append builtBy if present.
		if BuiltBy != "" {
			fmt.Fprintf(w, " (built by %s)", BuiltBy)
		}
		fmt.Fprintln(w)

	} else {
		// Fallback to Branch/Commit for local dev builds with missing version
		if Branch != "" {
			fmt.Fprintf(w, "branch=%s%s, ", Branch, dirtyText)
		}
		if Commit != "" {
			fmt.Fprintf(w, "commit=%s, ", Commit)
		}
		fmt.Fprintf(w, "built at %s\n", Date)
	}

	// Only with verbose + dirty: display list of files that were modified at build time.
	if Verbose && GitState == "dirty" && GitStatus != "" {
		fmt.Fprintln(w, "modified files at build time:")
		for _, line := range strings.Split(GitStatus, "|") {
			line = strings.TrimSpace(line)
			if line == "" {
				continue
			}
			fmt.Fprintf(w, "  %s\n", line)
		}
	}

	return nil
}
