// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implements the commandline interface and calls the appropriate commands.
package args

import (
	"fmt"
	"io"

	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/msg"
)

// scHelp is sub-command help. It prints usage to stdout.
func scHelp(w io.Writer) error {
	if verbose {
		fmt.Fprintf(w, "\n*** https://github.com/rokath/trice ***\n\n")
		fmt.Fprintf(w, "If a non-multi parameter is used more than one times the last value wins.\n")
	}
	cage.Enable(w)
	defer cage.Disable(w)

	fmt.Fprintln(w, "syntax: 'trice sub-command' [params]")
	var ok bool
	x := []selector{
		{allHelp || displayServerHelp, displayServerInfo},
		{allHelp || helpHelp, helpInfo},
		{allHelp || logHelp, logInfo},
		{allHelp || refreshHelp, refreshInfo},
		{allHelp || renewHelp, renewInfo},
		{allHelp || scanHelp, scanInfo},
		{allHelp || shutdownHelp, shutdownInfo},
		{allHelp || versionHelp, versionInfo},
		{allHelp || updateHelp, updateInfo},
		{allHelp || zeroIDsHelp, zeroIDsInfo},
	}
	for _, z := range x {
		if z.flag {
			msg.FatalOnErr(z.info(w))
			ok = true
		}
	}
	if !ok {
		fmt.Fprintln(w, "example 'trice h -help': Print help for help.")
	}
	return nil
}

func displayServerInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'ds|displayServer': Starts a display server. 
	Use in a separate console. On Windows use wt (https://github.com/microsoft/terminal) or a linux shell like git-bash to avoid ANSI color issues. 
	Running "trice ds" inside a console opens a display server to be used for displaying the TRICE logs remotely.
	Several instances of 'trice l -ds -port ...' (for different ports) will send output there in parallel.`)
	fsScSv.SetOutput(w)
	fsScSv.PrintDefaults()
	_, _ = fmt.Fprintln(w, "example: 'trice ds': Start display server.")
	return e
}

func helpInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'h|help': For command line usage.
	"trice h" will print this help text as a whole.`)
	fsScHelp.SetOutput(w)
	fsScHelp.PrintDefaults()
	fmt.Fprintln(w, "example 'trice h': Print short help.")
	fmt.Fprintln(w, "example 'trice h -all': Print all help.")
	fmt.Fprintln(w, "example 'trice h -log': Print log help.")
	return e
}

func logInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'l|log': For displaying trice logs coming from port. With "trice log" the trice tool display mode is activated.`)
	fsScLog.SetOutput(w)
	fsScLog.PrintDefaults()
	fmt.Fprintln(w, "example: 'trice l -p COM15 -baud 38400': Display trice log messages from serial port COM15")
	fmt.Fprintln(w, "example: 'trice l': Display flexL data format trice log messages from default source J-LINK over Segger RTT protocol.")
	fmt.Fprintln(w, "example: 'trice l -port ST-LINK -v -s': Shows verbose version information and also the received raw bytes.")
	return e
}

func refreshInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'r|refresh': For updating ID list from source files but does not change the source files.
	"trice refresh" will parse source tree(s) for TRICE macros, and refresh/generate the JSON list.
	This command should be run on adding source files to the project before the first time "trice update" is called.
	If the new source files contain TRICE macros with IDs these are added to til.json if not already used.
	Already used IDs are reported, so you have the chance to remove them from til.son and then do "trice u" again.
	This way you can make sure to get the new sources unchanged in your list.
	Already used IDs are replaced by new IDs during the next "trice update", so the old IDs in the list will survive.
	If you do not refresh the list after adding source files and perform an "trice update" new generated IDs could be equal to 
	IDs used in the added sources with the result that IDs in the added sources could get changed what you may not want.
	Using "trice u -IDMethod random" (default) makes the chance for such conflicts very low.
	The "refresh" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.`)
	fsScRefresh.SetOutput(w)
	fsScRefresh.PrintDefaults()
	fmt.Fprintln(w, "example: 'trice refresh': Update ID list from source tree.")
	return e
}

func renewInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'renew': It is like refresh, but til.json is cleared first, so all 'old' trices are removed. Use with care.`)
	fsScRenew.SetOutput(w)
	fsScRenew.PrintDefaults()
	fmt.Fprintln(w, "example: 'trice renew': Rebuild ID list from source tree, discard old IDs.")
	return e
}

func scanInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 's|scan': Shows available serial ports)`)
	fsScScan.SetOutput(w)
	fsScScan.PrintDefaults()
	fmt.Fprintln(w, "example: 'trice s': Show COM ports.")
	return e
}

func shutdownInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, "sub-command 'sd|shutdown': Ends display server at IPA:IPP, works also on a remote machine.")
	fsScSdSv.SetOutput(w)
	fsScSdSv.PrintDefaults()
	fmt.Fprintln(w, "example: 'trice sd': Shut down remote display server.")
	return e
}

func updateInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'u|update': For updating ID list and source files.
	"trice update" will parse source tree(s) for new or changed TRICE macros, modify them appropriate and update/generate the JSON list.
	The "update" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.`)
	fsScUpdate.SetOutput(w)
	fsScUpdate.PrintDefaults()
	fmt.Fprintln(w, "example: 'trice update -src ../A -src ../../B': Parse ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json")
	return e
}

func versionInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'ver|version': For displaying version information.
	"trice v" will print the version information. If trice is not versioned the build time will be displayed instead.`)
	fsScVersion.SetOutput(w)
	fsScVersion.PrintDefaults()
	return e
}

func zeroIDsInfo(w io.Writer) error {
	fmt.Fprintln(w, `sub-command 'zeroSourceTreeIds': Set all Id(n) inside source tree dir to Id(0). 
	Avoid using this sub-command normally. The switch "-src" is mandatory and no multi-flag here.
	This sub-command is mainly for testing. For several source directories you need several runs.`)
	fsScZero.SetOutput(w)
	fsScZero.PrintDefaults()
	_, e := fmt.Fprintln(w, "example: 'trice zeroSourceTreeIds -src ../A': Sets all TRICE IDs to 0 in ../A. Use with care!")
	return e
}
