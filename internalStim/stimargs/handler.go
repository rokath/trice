// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net

// Package args implements the command line interface and calls the appropriate commands.
package stimargs

import (
	"errors"
	"fmt"
	"io"
	"os"

	"github.com/rokath/trice/pkg/msg"
)

// Handler is called in main, evaluates args and calls the appropriate functions.
// It returns for program exit.
func Handler(w io.Writer, args []string) error {

	if Date == "" { // goreleaser will set Date, otherwise use file info.
		fi, err := os.Stat(os.Args[0])
		if nil == err { // On running main tests file-info is invalid, so do not use in that case.
			Date = fi.ModTime().String()
		}
	}

	// Verify that a sub-command has been provided: os.Arg[0] is the main command (stim), os.Arg[1] will be the sub-command.
	if len(args) < 2 {
		m := "no args, try: 'stim help'"
		return errors.New(m)
	}

	// Switch on the sub-command. Parse the flags for appropriate FlagSet.
	// FlagSet.Parse() requires a set of arguments to parse as input.
	// os.Args[2:] will be all arguments starting after the sub-command at os.Args[1]
	subCmd := args[1]
	subArgs := args[2:]
	switch subCmd { // Check which sub-command is invoked.
	default:
		return fmt.Errorf("unknown sub-command '%s'. try: 'stim help|h'", subCmd)
	case "h", "help":
		msg.OnErr(fsScHelp.Parse(subArgs))
		distributeArgs(w)
		return scHelp(w)
	case "ver", "version":
		msg.OnErr(fsScVersion.Parse(subArgs))
		return scVersion(w)
	case "s", "scan":
		msg.OnErr(fsScScan.Parse(subArgs))
		return scScan(w)
	case "w", "write":
		msg.OnErr(fsScWrite.Parse(subArgs))
		return scWrite(w)
	}
}
