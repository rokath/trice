// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net

package args

import (
	"fmt"
	"io"

	"github.com/rokath/trice/pkg/msg"
)

// scHelp is sub-command help. It prints usage to stdout.
func scHelp(w io.Writer) error {
	distributeArgs(w)
	if verbose {
		fmt.Fprintf(w, "\n*** github.com/rokath/trice/cmd/stim ***\n\n")
		fmt.Fprintf(w, "If a non-multi parameter is used more than one times the last value wins.\n")
	}

	fmt.Fprintln(w, "syntax: 'stim sub-command' [params]")
	var ok bool
	x := []selector{
		{allHelp || helpHelp, helpInfo},
		{allHelp || scanHelp, scanInfo},
		{allHelp || versionHelp, versionInfo},
		{allHelp || writeHelp, writeInfo},
	}
	for _, z := range x {
		if z.flag {
			msg.FatalOnErr(z.info(w))
			ok = true
		}
	}
	if !ok {
		fmt.Fprintln(w, "example 'stim h -help': Print help for help.")
	}
	return nil
}

type selector struct {
	flag bool
	info func(io.Writer) error
}

func helpInfo(w io.Writer) error {
	fmt.Fprintln(w, "")
	_, e := fmt.Fprintln(w, `sub-command 'h|help': For command line usage.
	"stim h" will print this help text as a whole.`)
	fsScHelp.SetOutput(w)
	fsScHelp.PrintDefaults()
	fmt.Fprintln(w, "example 'stim h': Print short help.")
	fmt.Fprintln(w, "example 'stim h -all': Print all help.")
	fmt.Fprintln(w, "example 'stim h -ver': Print version help.")
	return e
}

func scanInfo(w io.Writer) error {
	fmt.Fprintln(w, "")
	_, e := fmt.Fprintln(w, `sub-command 's|scan': Shows available serial ports)
example: 'stim s': Show COM ports.`)
	fsScScan.SetOutput(w)
	fsScScan.PrintDefaults()
	return e
}

func versionInfo(w io.Writer) error {
	fmt.Fprintln(w, "")
	_, e := fmt.Fprintln(w, `sub-command 'ver|version': For displaying version information.
	"stim ver" will print the version information. If stim is not versioned the build time will be displayed instead.`)
	fsScVersion.SetOutput(w)
	fsScVersion.PrintDefaults()
	return e
}

func writeInfo(w io.Writer) error {
	fmt.Fprintln(w, "")
	_, e := fmt.Fprintln(w, `sub-command 'w|write': Write to serial port according provided switches)`)
	fsScWrite.SetOutput(w)
	fsScWrite.PrintDefaults()
	fmt.Fprintln(w, "example: 'stim w -p COM7 -baud 4800 -parity none -b 0': Write a 0-byte to COM7.")
	return e
}
