// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implemets the commandline interface and calls the appropriate commands.
package args

import (
	"fmt"
	"os"

	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/msg"
)

// scHelp is subcommand help. It prits usage to stdout.
func scHelp() error {
	if verbose {
		fmt.Printf("\n*** https://github.com/rokath/trice ***\n\n")
		fmt.Printf("If a non-multi parameter is used more than one times the last value wins.\n")
	}
	cage.Enable()
	defer cage.Disable()

	fmt.Println("syntax: 'trice subcommand' [params]")
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
			msg.FatalOnErr(z.info())
			ok = true
		}
	}
	if !ok {
		fmt.Println("example 'trice h -help': Print help for help.")
	}
	return nil
}

func displayServerInfo() error {
	_, e := fmt.Println(`subcommand 'ds|displayServer': Starts a display server. 
	Use in a separate console. On Windows use wt (https://github.com/microsoft/terminal) or a linux shell like git-bash to avoid ANSI color issues. 
	Running "trice ds" inside a console opens a display server to be used for displaying the TRICE logs remotely.
	Several instances of 'trice l -ds -port ...' (for different ports) will send output there in parallel.`)
	fsScSv.SetOutput(os.Stdout)
	fsScSv.PrintDefaults()
	fmt.Println("example: 'trice ds': Start display server.")
	return e
}

func helpInfo() error {
	_, e := fmt.Println(`subcommand 'h|help': For command line usage.
	"trice h" will print this help text as a whole.`)
	fsScHelp.SetOutput(os.Stdout)
	fsScHelp.PrintDefaults()
	fmt.Println("example 'trice h': Print short help.")
	fmt.Println("example 'trice h -all': Print all help.")
	fmt.Println("example 'trice h -log': Print log help.")
	return e
}

func logInfo() error {
	_, e := fmt.Println(`subcommand 'l|log': For displaying trice logs coming from port. With "trice log" the trice tool display mode is activated.`)
	fsScLog.SetOutput(os.Stdout)
	fsScLog.PrintDefaults()
	fmt.Println("example: 'trice l -p COM15 -baud 38400': Display trice log messages from serial port COM15")
	fmt.Println("example: 'trice l': Display flexL data format trice log messages from default source J-LINK over Segger RTT protocol.")
	fmt.Println("example: 'trice l -port ST-LINK -v -s': Shows verbose version information and also the received raw bytes.")
	return e
}

func refreshInfo() error {
	_, e := fmt.Println(`subcommand 'r|refresh': For updating ID list from source files but does not change the source files.
	"trice refresh" will parse source tree(s) for TRICE macros, and refresh/generate the JSON list.
	This command should be run on adding souce files to the project before the first time "trice update" is called.
	If the new source files contain TRICE macros with IDs these are added to til.json if not already used.
	Already used IDs are reported, so you have the chance to remnove them from til.son and then do "trice u" again.
	This way you can make sure to get the new sources unchanged in your list. (to do: -force switch)
	Already used IDs are replaced by new IDs during the next "trice update", so the old IDs in the list will survive.
	If you do not refresh the list after adding source files and perform an "trice update" new generated IDs could be equal to 
	IDs used in the added sources with the result that IDs in the added sources could get changed what you may not want.
	Using "trice u -IDMethod random" (default) makes the chance for such conflicts very low.
	The "refresh" subcommand has no mantadory switches. Omitted optional switches are used with their default parameters.`)
	fsScRefresh.SetOutput(os.Stdout)
	fsScRefresh.PrintDefaults()
	fmt.Println("example: 'trice refresh': Update ID list from source tree.")
	return e
}

func renewInfo() error {
	_, e := fmt.Println(`subcommand 'renew': It is like refresh, but til.json is cleared first, so all 'old' trices are removed. Use with care.`)
	fsScRenew.SetOutput(os.Stdout)
	fsScRenew.PrintDefaults()
	fmt.Println("example: 'trice renew': Rebuild ID list from source tree, discard old IDs.")
	return e
}

func scanInfo() error {
	_, e := fmt.Println(`subcommand 's|scan': Shows available serial ports)`)
	fsScScan.SetOutput(os.Stdout)
	fsScScan.PrintDefaults()
	fmt.Println("example: 'trice s': Show COM ports.")
	return e
}

func shutdownInfo() error {
	_, e := fmt.Println("subcommand 'sd|shutdown': Ends display server at IPA:IPP, works also on a remote mashine.")
	fsScSdSv.SetOutput(os.Stdout)
	fsScSdSv.PrintDefaults()
	fmt.Println("example: 'trice sd': Shut down remote display server.")
	return e
}

func updateInfo() error {
	_, e := fmt.Println(`subcommand 'u|update': For updating ID list and source files.
	"trice update" will parse source tree(s) for new or changed TRICE macros, modify them appropriate and update/generate the JSON list.
	The "update" subcommand has no mantadory switches. Omitted optional switches are used with their default parameters.`)
	fsScUpdate.SetOutput(os.Stdout)
	fsScUpdate.PrintDefaults()
	fmt.Println("example: 'trice update -src ../A -src ../../B': Parse ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json")
	return e
}

func versionInfo() error {
	_, e := fmt.Println(`subcommand 'ver|version': For displaying version information.
	"trice v" will print the version information. In trice is unversioned the build time will be displayed instead.`)
	fsScVersion.SetOutput(os.Stdout)
	fsScVersion.PrintDefaults()
	return e
}

func zeroIDsInfo() error {
	fmt.Println(`subcommand 'zeroSourceTreeIds': Set all Id(n) inside source tree dir to Id(0). 
	Avoid using this subcommand normally. The switch "-src" is mantadory and no multi-flag here.
	This subcommand is mainly for testing. For several source directories you need several runs.`)
	fsScZero.SetOutput(os.Stdout)
	fsScZero.PrintDefaults()
	_, e := fmt.Println("example: 'trice zeroSourceTreeIds -src ../A': Sets all TRICE IDs to 0 in ../A. Use with care!")
	return e
}
