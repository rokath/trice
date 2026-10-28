// SPDX-License-Identifier: MIT

// Command testcheck validates native test evidence and writes the suite report.
// It uses only the standard library so the test bootstrap needs no extra runtime.
package main

import (
	"bufio"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strconv"
	"strings"
	"time"
)

// testEvent is the stable subset of go test -json needed to distinguish an
// executed test from an empty selection, a skipped test or a failed package.
type testEvent struct {
	Action, Package, Test, Output, ImportPath string
}

// allowedSkip documents platform capability exceptions, never missing tools.
// Native Windows process/restore tests cover the normal worker paths; only
// POSIX process-group delivery is inapplicable. Windows symlink rejection is
// additionally covered by the ordinary object/path checks without elevation.
func allowedSkip(platform, name, reason string) bool {
	if platform == "windows" {
		for _, test := range []string{
			"TestManagedWorkflowRepeatedInterrupts", "TestPCWorkerCancellationReachesDescendants",
			"TestL432CancellationStopsCompilerDescendants", "TestHandleSIGTERMExitsAndCloses",
		} {
			if name == test && (strings.Contains(reason, "POSIX process-group") || strings.Contains(reason, "SIGTERM subprocess")) {
				return true
			}
		}
		if (strings.HasPrefix(name, "TestPCCacheOverlayTracksContents/") || name == "TestBindReportDoesNotFollowArtifactSymlinks") &&
			(strings.Contains(reason, "symlink") || strings.Contains(reason, "symbolic link")) {
			return true
		}
	}
	// PowerShell/cmd syntax is required on Windows. Other hosts exercise Bash;
	// an installed PowerShell still runs and must pass on those hosts as well.
	return platform != "windows" && strings.HasPrefix(name, "TestStampShellQuoting/") &&
		!strings.HasSuffix(name, "/Bash_and_Git_Bash") && strings.Contains(reason, "is not installed")
}

// audit consumes the complete JSON stream, including package completion.
// A partial stream, unknown skip or missing required test is incomplete (2),
// while a complete stream containing test/package failures returns failure (1).
func audit(input io.Reader, output io.Writer, platform string, required []string) int {
	decoder := json.NewDecoder(input)
	passed := make(map[string]bool)
	failedTests := make(map[string]bool)
	running := make(map[string]bool)
	reasons := make(map[string]string)
	packages := 0
	failed, incomplete := false, false
	for {
		var event testEvent
		if err := decoder.Decode(&event); err != nil {
			if !errors.Is(err, io.EOF) {
				fmt.Fprintf(output, "INCOMPLETE: invalid Go test evidence: %v\n", err)
				incomplete = true
			}
			break
		}
		key := event.Package + "/" + event.Test
		switch event.Action {
		case "start":
			running[event.Package] = true
		case "output":
			fmt.Fprint(output, event.Output)
			// Retain the last diagnostic, not an entire verbose compiler log.
			if strings.TrimSpace(event.Output) != "" {
				reasons[key] += event.Output
				if len(reasons[key]) > 4096 {
					reasons[key] = reasons[key][len(reasons[key])-4096:]
				}
			}
		case "pass":
			if event.Test != "" {
				passed[event.Test] = true
			} else {
				delete(running, event.Package)
				packages++
			}
		case "fail":
			failed = true
			failedTests[event.Test] = true
			if event.Test == "" {
				delete(running, event.Package)
				packages++
			}
		case "build-output":
			fmt.Fprint(output, event.Output)
		case "build-fail":
			// Go 1.25 reports compilation errors before a package can start.
			// Preserve this as a real failure, including its compiler diagnostic.
			failed = true
			packages++
			delete(running, event.ImportPath)
		case "skip":
			if event.Test == "" {
				delete(running, event.Package)
				packages++ // Packages without tests legitimately report skip.
			} else if allowedSkip(platform, event.Test, reasons[key]) {
				fmt.Fprintf(output, "NOT APPLICABLE: %s (%s); documented platform exception\n", event.Test, platform)
			} else {
				fmt.Fprintf(output, "INCOMPLETE: required native test skipped: %s/%s\n", event.Package, event.Test)
				incomplete = true
			}
		}
	}
	if packages == 0 || len(running) != 0 {
		fmt.Fprintln(output, "INCOMPLETE: no completed Go packages or an interrupted JSON stream")
		incomplete = true
	}
	for _, name := range required {
		if !passed[name] && !failedTests[name] {
			fmt.Fprintf(output, "INCOMPLETE: required test did not pass: %s\n", name)
			incomplete = true
		}
	}
	if incomplete {
		return 2
	}
	if failed {
		return 1
	}
	return 0
}

// stepResult keeps each group attributable to its log and measured duration.
type stepResult struct {
	Name     string `json:"name"`
	Result   string `json:"result"`
	Duration int    `json:"duration_seconds"`
	Log      string `json:"log"`
}

// suiteReport describes this host only; it never claims other OS runs passed.
type suiteReport struct {
	Schema    int          `json:"schema"`
	Result    string       `json:"result"`
	Selection string       `json:"selection"`
	OS        string       `json:"os"`
	Arch      string       `json:"arch"`
	Commit    string       `json:"commit"`
	Dirty     bool         `json:"dirty"`
	Started   time.Time    `json:"started"`
	Duration  int64        `json:"duration_seconds"`
	Context   string       `json:"tool_context"`
	Steps     []stepResult `json:"steps"`
}

// writeReport publishes JSON atomically in the current run directory. A
// malformed record is an error rather than silently missing test evidence.
func writeReport(dir, selection, result string, started int64) error {
	file, err := os.Open(filepath.Join(dir, "steps.tsv"))
	if err != nil {
		return err
	}
	defer file.Close()
	report := suiteReport{Schema: 1, Result: result, Selection: selection,
		OS: runtime.GOOS, Arch: runtime.GOARCH, Started: time.Unix(started, 0).UTC(),
		Duration: time.Now().Unix() - started, Steps: []stepResult{}}
	// Git context is evidence too: a failed lookup must not look like a clean run.
	commit, err := exec.Command("git", "rev-parse", "HEAD").Output()
	if err != nil {
		return fmt.Errorf("record commit: %w", err)
	}
	report.Commit = strings.TrimSpace(string(commit))
	status, err := exec.Command("git", "status", "--porcelain").Output()
	if err != nil {
		return fmt.Errorf("record worktree: %w", err)
	}
	report.Dirty = len(status) != 0
	context, err := os.ReadFile(filepath.Join(dir, "tools.log"))
	if err != nil {
		return fmt.Errorf("record tool context: %w", err)
	}
	report.Context = string(context)
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		fields := strings.SplitN(scanner.Text(), "\t", 4)
		if len(fields) != 4 {
			return fmt.Errorf("invalid step record: %q", scanner.Text())
		}
		seconds, err := strconv.Atoi(fields[2])
		if err != nil || seconds < 0 {
			return fmt.Errorf("invalid step duration: %q", fields[2])
		}
		report.Steps = append(report.Steps, stepResult{fields[0], fields[1], seconds, fields[3]})
	}
	if err := scanner.Err(); err != nil {
		return err
	}
	data, err := json.MarshalIndent(report, "", "  ")
	if err != nil {
		return err
	}
	temporary := filepath.Join(dir, "results.json.tmp")
	if err := os.WriteFile(temporary, append(data, '\n'), 0o644); err != nil {
		return err
	}
	return os.Rename(temporary, filepath.Join(dir, "results.json"))
}

// main uses stable exit codes shared by the shell runner and native CI.
func main() {
	if len(os.Args) == 4 && os.Args[1] == "identity" {
		if err := checkToolIdentity(os.Args[2], os.Args[3], ""); err != nil {
			fmt.Fprintf(os.Stderr, "INCOMPLETE: %v\n", err)
			os.Exit(2)
		}
		return
	}
	if len(os.Args) > 1 && (os.Args[1] == "tools" || os.Args[1] == "bundle") {
		// The first pass may reuse or freeze installed tools but cannot download,
		// compile or unpack missing ones. Exit 3 asks the shell for one approval.
		arguments, allowInstall := os.Args[2:], true
		if len(arguments) > 0 && arguments[0] == "--no-install" {
			arguments, allowInstall = arguments[1:], false
		}
		var bin string
		var err error
		switch {
		case os.Args[1] == "tools" && len(arguments) >= 3:
			bin, err = installTools(arguments[0], arguments[1], arguments[2:], allowInstall)
		case os.Args[1] == "bundle" && len(arguments) == 3:
			bin, err = installBundle(arguments[0], arguments[1], arguments[2], allowInstall)
		default:
			fmt.Fprintln(os.Stderr, "INCOMPLETE: tool setup needs config, cache and tool names")
			os.Exit(2)
		}
		if err != nil {
			if errors.Is(err, errInstallRequired) {
				fmt.Fprintln(os.Stderr, "SETUP REQUIRED:", err)
				os.Exit(3)
			}
			fmt.Fprintln(os.Stderr, "INCOMPLETE:", err)
			os.Exit(2)
		}
		fmt.Println(bin)
		return
	}
	if len(os.Args) >= 3 && os.Args[1] == "audit" {
		file, err := os.Open(os.Args[2])
		if err != nil {
			fmt.Fprintln(os.Stderr, "INCOMPLETE:", err)
			os.Exit(2)
		}
		defer file.Close()
		os.Exit(audit(file, os.Stdout, runtime.GOOS, os.Args[3:]))
	}
	if len(os.Args) == 6 && os.Args[1] == "report" {
		started, err := strconv.ParseInt(os.Args[5], 10, 64)
		if err == nil {
			err = writeReport(os.Args[2], os.Args[3], os.Args[4], started)
		}
		if err != nil {
			fmt.Fprintln(os.Stderr, "INCOMPLETE: cannot write test report:", err)
			os.Exit(2)
		}
		return
	}
	fmt.Fprintln(os.Stderr, "Usage: testcheck audit events.json [required-test ...] | report directory selection result started-unix | tools [--no-install] config cache names... | bundle [--no-install] config cache name | identity binary version")
	os.Exit(2)
}
