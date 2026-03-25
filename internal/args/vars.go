// SPDX-License-Identifier: MIT

package args

import (
	"flag"
)

var (
	// Version is injected at build time (typically by goreleaser).
	Version string

	// Commit is the VCS revision hash injected at build time.
	Commit string

	// Date is the build timestamp injected at build time.
	Date string

	// Verbose enables additional informational output.
	Verbose bool

	// Branch is the Git branch name at build time.
	Branch string

	// GitState indicates repository state at build time ("clean" or "dirty").
	GitState string

	// GitStatus contains a '|' separated summary of modified files at build time.
	GitStatus string

	// BuiltBy is an optional builder identifier (see .goreleaser.yaml).
	BuiltBy string

	// fsScAdd is flag set for sub command 'add' for updating ID list without touching the sources.
	fsScAdd *flag.FlagSet

	// fsScGenerate is flag set for sub command 'generate' for creating support files without touching the sources.
	fsScGenerate *flag.FlagSet

	// fsScUpdate is flag set for sub command 'update' for updating ID list.
	//fsScUpdate *flag.FlagSet

	// fsScInsert is flag set for sub command 'insert' for updating src tree and ID list.
	fsScInsert *flag.FlagSet

	// fsScHelp is flag set for sub command 'help'.
	fsScHelp *flag.FlagSet

	// fsScLog is flag set for sub command 'log'.
	fsScLog *flag.FlagSet

	// fsScSv is flag set for sub command 'displayServer'.
	fsScSv *flag.FlagSet

	// fsScVersion is flag set for sub command 'version'.
	fsScVersion *flag.FlagSet

	// fsScScan is flag set for sub command 'scan'.
	fsScScan *flag.FlagSet

	// fsScSdSv is flag set for sub command 'shutdownServer'.
	fsScSdSv *flag.FlagSet

	// fsScClean is flag set for sub command 'clean' for clearing IDs in source tree.
	fsScClean *flag.FlagSet

	allHelp           bool // flag for partial help
	displayServerHelp bool // flag for partial help
	helpHelp          bool // flag for partial help
	logHelp           bool // flag for partial help
	addHelp           bool // flag for partial help
	generateHelp      bool // flag for partial help
	scanHelp          bool // flag for partial help
	shutdownHelp      bool // flag for partial help
	insertIDsHelp     bool // flag for partial help
	versionHelp       bool // flag for partial help
	cleanIDsHelp      bool // flag for partial help
)
