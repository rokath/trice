// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implemets the commandline interface and calls the appropriate commands.
package args

import (
	"errors"
	"fmt"
	"os"
	"strings"
	"time"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/cipher"
	"github.com/rokath/trice/pkg/msg"
)

// Handler is called in main, evaluates args and calls the appropriate functions.
// It returns for program exit.
func Handler(args []string) error {

	id.FnJSON = id.ConditionalFilePath(id.FnJSON)

	if "" == Date { // goreleaser will set Date, otherwise use file info.
		fi, err := os.Stat(os.Args[0])
		if nil == err { // On running main tests fileinfo is invalid, so do not use in that case.
			Date = fi.ModTime().String()
		}
	}
	cage.DefaultLogfileName = "2006-01-02_1504-05_trice.log"

	// Verify that a subcommand has been provided: os.Arg[0] is the main command (trice), os.Arg[1] will be the subcommand.
	if len(os.Args) < 2 {
		m := "no args, try: 'trice help'"
		fmt.Println(m)
		return errors.New(m)
	}

	// Switch on the subcommand. Parse the flags for appropriate FlagSet.
	// FlagSet.Parse() requires a set of arguments to parse as input.
	// os.Args[2:] will be all arguments starting after the subcommand at os.Args[1]
	subCmd := args[1]
	subArgs := args[2:]
	switch subCmd { // Check which subcommand is invoked.
	default:
		fmt.Println("try: 'trice help|h'")
		return nil
	case "h", "help":
		msg.OnErr(fsScHelp.Parse(subArgs))
		distributeArgs()
		return scHelp()
	case "s", "scan":
		msg.OnErr(fsScScan.Parse(subArgs))
		distributeArgs()
		_, err := com.GetSerialPorts()
		return err
	case "ver", "version":
		msg.OnErr(fsScVersion.Parse(subArgs))
		distributeArgs()
		return scVersion()
	case "r", "refresh":
		msg.OnErr(fsScRefresh.Parse(subArgs))
		distributeArgs()
		return id.SubCmdRefreshList()
	case "u", "update":
		msg.OnErr(fsScUpdate.Parse(subArgs))
		distributeArgs()
		return id.SubCmdUpdate()
	case "zeroSourceTreeIds":
		msg.OnErr(fsScZero.Parse(subArgs))
		distributeArgs()
		return id.ScZero(*pSrcZ, fsScZero)
	case "sd", "shutdown":
		msg.OnErr(fsScSdSv.Parse(subArgs))
		distributeArgs()
		return emitter.ScShutdownRemoteDisplayServer(1)
	case "ds", "displayServer":
		msg.OnErr(fsScSv.Parse(subArgs))
		distributeArgs()
		return emitter.ScDisplayServer() // endless loop
	case "l", "log":
		msg.OnErr(fsScLog.Parse(subArgs))
		distributeArgs()
		logLoop() // endless loop
		return nil
	}
}

// scHelp is subcommand help. It prits usage to stdout.
func scHelp() error {
	if verbose {
		fmt.Printf("\n*** https://github.com/rokath/trice ***\n\n")
		fmt.Printf("If a non-multi parameter is used more than one times the last value wins.\n")
	}
	cage.Enable()
	defer cage.Disable()

	fmt.Println("syntax: 'trice subcommand' [params]")
	if false == (allHelp ||
		displayServerHelp ||
		helpHelp ||
		logHelp ||
		refreshHelp ||
		scanHelp ||
		shutdownHelp ||
		updateHelp ||
		versionHelp ||
		zeroIDsHelp) {
		fmt.Println("example 'trice h -help': Print help for help.")
	}

	if allHelp || displayServerHelp {
		fmt.Println(`subcommand 'ds|displayServer': Starts a display server. 
	Use in a separate console. On Windows use wt (https://github.com/microsoft/terminal) or a linux shell like git-bash to avoid ANSI color issues. 
	Running "trice ds" inside a console opens a display server to be used for displaying the TRICE logs remotely.
	Several instances of 'trice l -ds -port ...' (for different ports) will send output there in parallel.`)
		fsScSv.SetOutput(os.Stdout)
		fsScSv.PrintDefaults()
		fmt.Println("example: 'trice ds': Start display server.")
	}

	if allHelp || helpHelp {
		fmt.Println(`subcommand 'h|help': For command line usage.
	"trice h" will print this help text as a whole.`)
		fsScHelp.SetOutput(os.Stdout)
		fsScHelp.PrintDefaults()
		fmt.Println("example 'trice h': Print short help.")
		fmt.Println("example 'trice h -all': Print all help.")
		fmt.Println("example 'trice h -log': Print log help.")
	}

	if allHelp || logHelp {
		fmt.Println(`subcommand 'l|log': For displaying trice logs coming from port. With "trice log" the trice tool display mode is activated.`)
		fsScLog.SetOutput(os.Stdout)
		fsScLog.PrintDefaults()
		fmt.Println("example: 'trice l -p COM15 -baud 38400': Display trice log messages from serial port COM15")
		fmt.Println("example: 'trice l': Display pack2L data format trice log messages from default source J-LINK over Segger RTT protocol.")
		fmt.Println("example: 'trice l -port ST-LINK -v -s': Shows verbose version information and also the received raw bytes.")
	}

	if allHelp || refreshHelp {
		fmt.Println(`subcommand 'r|refresh': For updating ID list from source files but does not change the source files.
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
	}

	if allHelp || scanHelp {
		fmt.Println(`subcommand 's|scan': Shows availiable serial ports)`)
		fsScScan.SetOutput(os.Stdout)
		fsScScan.PrintDefaults()
		fmt.Println("example: 'trice s': Show COM ports.")
	}

	if allHelp || shutdownHelp {
		fmt.Println("subcommand 'sd|shutdown': Ends display server at IPA:IPP, works also on a remote mashine.")
		fsScSdSv.SetOutput(os.Stdout)
		fsScSdSv.PrintDefaults()
		fmt.Println("example: 'trice sd': Shut down remote display server.")
	}

	if allHelp || updateHelp {
		fmt.Println(`subcommand 'u|update': For updating ID list and source files.
	"trice update" will parse source tree(s) for new or changed TRICE macros, modify them appropirate and update/generate the JSON list.
	The "update" subcommand has no mantadory switches. Omitted optional switches are used with their default parameters.`)
		fsScUpdate.SetOutput(os.Stdout)
		fsScUpdate.PrintDefaults()
		fmt.Println("example: 'trice update -src ../A -src ../../B': Parse ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json")
	}

	if allHelp || versionHelp {
		fmt.Println(`subcommand 'ver|version': For displaying version information.
	"trice v" will print the version information. In trice is unversioned the build time will be displayed instead.`)
		fsScVersion.SetOutput(os.Stdout)
		fsScVersion.PrintDefaults()
	}

	if allHelp || zeroIDsHelp {
		fmt.Println(`subcommand 'zeroSourceTreeIds': Set all Id(n) inside source tree dir to Id(0). 
	Avoid using this subcommand normally. The switch "-src" is mantadory and no multi-flag here.
	This subcommand is mainly for testing. For several source directories you need several runs.`)
		fsScZero.SetOutput(os.Stdout)
		fsScZero.PrintDefaults()
		fmt.Println("example: 'trice zeroSourceTreeIds -src ../A': Sets all TRICE IDs to 0 in ../A. Use with care!")
	}
	return nil
}

// logLoop prepares writing and lut and provides a retry mechanism for unplugged UART.
func logLoop() {
	msg.OnErr(cipher.SetUp()) // does nothing when -password is "none"
	if decoder.TestTableMode {
		// set switches if they not set already
		// trice l -ts off -prefix " }, ``" -suffix "\n``}," -color off
		if "LOCmicro" == emitter.TimestampFormat {
			emitter.TimestampFormat = "off"
		}
		if defaultPrefix == emitter.Prefix {
			//emitter.Prefix = " }, \""
			emitter.Prefix = " }, `"
		}
		if "" == emitter.Suffix {
			//emitter.Suffix = "\\\\n\"},"
			emitter.Suffix = "`},"
		}
		if "default" == emitter.ColorPalette {
			emitter.ColorPalette = "off"
		}
	}
	c := cage.Start(cage.Name)
	lu := id.NewLut(id.FnJSON) // lut is a map, that means a pointer

	// Just in case the id list file FnJSON gets updated, the file watcher updates lut.
	// This way trice needs not to be restarted during development process.
	lu.FileWatcher()

	sw := emitter.New()
	var interrupted bool
	var counter int

	for {
		rc, e := receiver.NewReadCloser(receiver.Port, receiver.PortArguments)
		if nil != e {
			if verbose {
				fmt.Println(e)
			}
			if !interrupted {
				cage.Stop(c)
				return // hopeless
			}
			time.Sleep(1000 * time.Millisecond) // retry interval
			fmt.Printf("\rsig:(re-)setup input port...%d", counter)
			counter++
			continue
		}
		defer func() { msg.OnErr(rc.Close()) }()
		if receiver.ShowInputBytes {
			rc = receiver.NewBytesViewer(rc)
		}

		f := decoder.Translate(sw, lu, rc)
		if false == f {
			cage.Stop(c)
			return
		}
		interrupted = true
	}
}

// scVersion is subcommand 'version'. It prints version information.
func scVersion() error {
	cage.Enable()
	defer cage.Disable()
	if verbose {
		fmt.Println("https://github.com/rokath/trice")
	}
	if "" != Version {
		fmt.Printf("version=%v, commit=%v, built at %v\n", Version, Commit, Date)
	} else {
		fmt.Printf("version=devel, built %s\n", Date)
	}
	return nil
}

// distributeArgs is distibuting values used in several packages.
// It must not be called before the appropriate arg parsing.
func distributeArgs() {
	replaceDefaultArgs()
	com.Verbose = verbose
	id.Verbose = verbose
	link.Verbose = verbose
	cage.Verbose = verbose
	decoder.Verbose = verbose
	emitter.Verbose = verbose
	emitter.TestTableMode = decoder.TestTableMode
}

// replaceDefaultArgs assigns port specific default strings.
func replaceDefaultArgs() {
	if strings.HasPrefix(receiver.Port, "COM") {
		receiver.PortArguments = defaultCOMArgs
	} else {
		switch receiver.Port {
		case "JLINK", "STLINK", "J-LINK", "ST-LINK":
			receiver.PortArguments = defaultLinkArgs
		case "BUFFER":
			receiver.PortArguments = defaultBUFFERArgs
		}
	}
}
