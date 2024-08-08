// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implements the commandline interface and calls the appropriate commands.
package args

import (
	"fmt"
	"io"

	"github.com/rokath/trice/pkg/msg"
)

// scHelp is sub-command help. It prints usage to stdout.
func scHelp(w io.Writer) error {
	if Verbose {
		fmt.Fprintf(w, "\n*** https://github.com/rokath/trice ***\n\n")
		fmt.Fprintf(w, "If a non-multi parameter is used more than one times the last value wins.\n")
	}

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
		{allHelp || insertIDsHelp, insertIDsInfo},
		{allHelp || zeroIDsHelp, zeroIDsInfo},
		{allHelp || cleanIDsHelp, cleanIDsInfo},
	}
	for _, z := range x {
		if z.flag {
			msg.FatalOnErr(z.info(w))
			ok = true
		}
	}
	if !ok {
		fmt.Fprintln(w, "Example 'trice h -help': Print help for help.")
	}
	return nil
}

func versionInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'ver|version': For displaying version information.
	"trice v" will print the version information. If trice is not versioned the build time will be displayed instead.`)
	fsScVersion.SetOutput(w)
	fsScVersion.PrintDefaults()
	return e
}

func helpInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'h|help': For command line usage.
#	"trice h" will print this help text as a whole.
#	Example 'trice h': Print short help.
#	Example 'trice h -all': Print all help.
#	Example 'trice h -log': Print log help.`)
	fsScHelp.SetOutput(w)
	fsScHelp.PrintDefaults()
	return e
}

func logInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'l|log': For displaying trice logs coming from port. With "trice log" the trice tool display mode is activated.
#	Example: 'trice l -p COM15 -baud 38400': Display trice log messages from serial port COM15
#	Example: 'trice l': Display flexL data format trice log messages from default source J-LINK over Segger RTT protocol.
#	Example: 'trice l -port ST-LINK -v -s': Shows verbose version information and also the received raw bytes.`)
	fsScLog.SetOutput(w)
	fsScLog.PrintDefaults()
	return e
}

func scanInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 's|scan': Shows available serial ports)
#	Example: 'trice s': Show COM ports.`)
	fsScScan.SetOutput(w)
	fsScScan.PrintDefaults()
	return e
}

func displayServerInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'ds|displayServer': Starts a display server. 
#	Use in a separate console. On Windows use wt (https://github.com/microsoft/terminal) or a linux shell like git-bash to avoid ANSI color issues. 
#	Running "trice ds" inside a console opens a display server to be used for displaying the TRICE logs remotely.
#	Several instances of 'trice l -ds -port ...' (for different ports) will send output there in parallel.
#	Example: 'trice ds': Start display server.`)
	fsScSv.SetOutput(w)
	fsScSv.PrintDefaults()
	return e
}

func shutdownInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'sd|shutdown': Ends display server at IPA:IPP, works also on a remote machine.
#	Example: 'trice sd': Shut down remote display server.`)
	fsScSdSv.SetOutput(w)
	fsScSdSv.PrintDefaults()
	return e
}

func insertIDsInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'i|insert': For updating til.json and inserting IDs into source files.
#	"trice insert" will parse source tree(s) for new or changed TRICE macros, modify them appropriate and extend the JSON list.
#	This command relies on existing til.json and li.json files. The li.json file is used as reference and generated new during insert.
#	Without li.json the insert command assigns new IDs to all found trice macros, because it cannot assign files to them, and extends 
#	the til.json file accordingly. That is for safety, because the insert command acts in multiple Go routines per file parallel. 
#	If you lost the li.json file, you can run "trice clean|zero" first, to re-generate a new li.json and then execute "trice insert".
#	With an empty til.json file, the insert command re-creates a new til.json (and a fresh li.json) from the source code.
#	If the insert command finds an ID for a trice inside the sources, used already for an other trice inside til.json, it reports that
#	and assigns a new ID into the source file, adding it to til.json as well.
#	The "insert" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
#	The switch "-src" is optional (default is "./") and a multi-flag here. So you can use the "-src" flag several times.
#	Example: 'trice i -src ../A -src ../../B': Parse ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json`)
	fsScUpdate.SetOutput(w)
	fsScUpdate.PrintDefaults()
	return e
}

func cleanIDsInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'c|clean': Set all [id|Id|ID](n) inside source tree dir to [id|Id|ID](0). Remove all iD(n).
#	All in source code found IDs are added to til.json if not already there. Inside til.json differently used IDs are 
#	reported and just cleaned inside the source files. The existing li.json is not used. A new li.json is generated in place. 
#	The switch "-src" is optional (default is "./") and a multi-flag here. So you can use the "-src" flag several times.
#	Example: 'trice clean -src ../A -src B/x.c': Sets all TRICE IDs to 0, or removes them, in folder ../A. and file B/x.c
#	EXPERIMENTAL! The command itself works reliable, but a sophisticated editor will detect inconsistencies with removed IDs.
#	EXPERIMENTAL! With "#define TRICE_CLEAN 1" inside "triceConfig.h" these displayed "errors" are suppressable.
#	EXPERIMENTAL! The user needs then to set "#define TRICE_CLEAN 0" inside "triceConfig.h" after "trice insert" manually before compiling.
#	EXPERIMENTAL! That could be automated, but right now not sure about maybe unwanted implications. So we do not touch it currently.
#	In difference to "trice zero", Trice function calls get iD(n) removed. Example: "TRice( iD(88), "hi);" -> "TRice("hi);"`)
	fsScClean.SetOutput(w)
	fsScClean.PrintDefaults()
	return e
}

func zeroIDsInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'z|zero': Set all [id|Id|ID|iD](n) inside source tree dir to [id|Id|ID|iD](0).
#	All in source code found IDs are added to til.json if not already there. Inside til.json differently used IDs are 
#	reported and just zeroed inside the source files. The existing li.json is not used. A new li.json is generated in place. 
#	The switch "-src" is optional (default is "./") and a multi-flag here. So you can use the "-src" flag several times.
#	Example: 'trice zero -src ../A -src B/x.c': Sets all TRICE IDs to 0 in folder ../A. and file B/x.c
#	In difference to "trice clean", Trice function calls keep the ID as zero IDs. Example: "TRice( iD(88), "hi);" -> "TRice( iD(0), "hi);"`)
	fsScZero.SetOutput(w)
	fsScZero.PrintDefaults()
	return e
}

func updateInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'u|update': DEPRECIATED! Will be removed in the future.
#	Use "trice i|insert" instead.
#	DEPRECIATED! For updating ID list and source files.
#	DEPRECIATED! "trice update" will parse source tree(s) for new or changed TRICE macros, modify them appropriate and update/generate the JSON list.
#	DEPRECIATED! The "update" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
#	DEPRECIATED! Example: 'trice update -src ../A -src ../../B': Parse ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json.
#	Use command "insert" instead.`)
	fsScUpdate.SetOutput(w)
	fsScUpdate.PrintDefaults()
	return e
}

func refreshInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'r|refresh': DEPRECIATED! Will be removed in the future.
#	Use "trice z|zero" and "trice i|insert" instead.
#	DEPRECIATED! For updating ID list from source files but does not change the source files.
#	DEPRECIATED! "trice refresh" will parse source tree(s) for TRICE macros, and refresh/generate the JSON list.
#	DEPRECIATED! This command should be run on adding source files to the project before the first time "trice update" is called.
#	DEPRECIATED! If the new source files contain TRICE macros with IDs these are added to til.json if not already used.
#	DEPRECIATED! Already used IDs are reported, so you have the chance to remove them from til.son and then do "trice u" again.
#	DEPRECIATED! This way you can make sure to get the new sources unchanged in your list.
#	DEPRECIATED! Already used IDs are replaced by new IDs during the next "trice update", so the old IDs in the list will survive.
#	DEPRECIATED! If you do not refresh the list after adding source files and perform an "trice update" new generated IDs could be equal to 
#	DEPRECIATED! IDs used in the added sources with the result that IDs in the added sources could get changed what you may not want.
#	DEPRECIATED! Using "trice u -IDMethod random" (default) makes the chance for such conflicts very low.
#	DEPRECIATED! The "refresh" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
#	DEPRECIATED! Example: 'trice refresh': Update ID list from source tree.`)
	fsScRefresh.SetOutput(w)
	fsScRefresh.PrintDefaults()
	return e
}

func renewInfo(w io.Writer) error {
	_, e := fmt.Fprintln(w, `sub-command 'renew': DEPRECIATED! Will be removed in the future. 
#	Use "trice z|zero" and "trice i|insert" instead (empty til.json manually first).
#	DEPRECIATED! It is like refresh, but til.json is cleared first, so all 'old' trices are removed. Use with care.
#	DEPRECIATED! Example: 'trice renew': Rebuild ID list from source tree, discard old IDs.`)
	fsScRenew.SetOutput(w)
	fsScRenew.PrintDefaults()
	return e
}
