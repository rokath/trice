// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implemets the commandline interface and calls the appropriate commands.
package args

import (
	"errors"
	"flag"
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
		msg.InfoOnErr("", fsScHelp.Parse(subArgs))
		distributeArgs()
		return scHelp(fsScLog, fsScZero, fsScVersion, fsScSv, fsScSdSv)
	case "s", "sc", "scan":
		msg.InfoOnErr("", fsScScan.Parse(subArgs))
		distributeArgs()
		_, err := com.GetSerialPorts()
		return err
	case "v", "ver", "version":
		msg.InfoOnErr("", fsScVersion.Parse(subArgs))
		distributeArgs()
		return scVersion()
	case "u", "update":
		msg.InfoOnErr("", fsScUpdate.Parse(subArgs))
		distributeArgs()
		return id.SubCmdUpdate()
	case "zeroSourceTreeIds":
		msg.InfoOnErr("", fsScZero.Parse(subArgs))
		distributeArgs()
		return id.ScZero(*pSrcZ, fsScZero)
	case "sd", "sdds", "sdrds", "shutdownRemoteDisplayServer":
		msg.InfoOnErr("", fsScSdSv.Parse(subArgs))
		distributeArgs()
		return emitter.ScShutdownRemoteDisplayServer(1)
	case "ds", "displayServer":
		msg.InfoOnErr("", fsScSv.Parse(subArgs))
		distributeArgs()
		return emitter.ScDisplayServer() // endless loop
	case "l", "log":
		msg.InfoOnErr("", fsScLog.Parse(subArgs))
		distributeArgs()
		logLoop() // endless loop
		return nil
	}
}

// scHelp is subcommand help. It prits usage to stdout.
func scHelp(
	l *flag.FlagSet,
	z *flag.FlagSet,
	v *flag.FlagSet,
	sv *flag.FlagSet,
	scSdSv *flag.FlagSet,
) error {
	if verbose {
		fmt.Printf("\n*** https://github.com/rokath/trice ***\n\n")
		fmt.Printf("If a non-multi parameter is used more than one times the last value wins.\n")
	}
	cage.Enable()
	defer cage.Disable()

	fmt.Println("syntax: 'trice subcommand' [params]")

	fmt.Println(`subcommand 'ds|displayServer': Starts a display server. 
	Use in a separate console. On Windows use wt (https://github.com/microsoft/terminal) or a linux shell like git-bash to avoid ANSI color issues. 
	Running "trice ds" inside a console opens a display server to be used for displaying the TRICE logs remotely.
	Several instances of 'trice l -ds -port ...' (for different ports) will send output there in parallel.`)
	sv.SetOutput(os.Stdout)
	sv.PrintDefaults()

	fmt.Println(`subcommand 'h|help': For command line usage.
	"trice h" will print this help text as a whole.`)

	fsScHelp.SetOutput(os.Stdout)
	fsScHelp.PrintDefaults()

	fmt.Println(`subcommand 'l|log': For displaying trice logs coming from port. With "trice log" the trice tool display mode is activated.`)
	l.SetOutput(os.Stdout)
	l.PrintDefaults()

	fmt.Println("subcommand 'sd|shutdownServer': Ends display server at IPA:IPP, works also on a remote mashine.")
	scSdSv.SetOutput(os.Stdout)
	scSdSv.PrintDefaults()

	fmt.Println(`subcommand 'u|upd|update': For updating ID list and source files.
	"trice update" will parse source tree(s) for new or changed TRICE macros, modify them appropirate and update/generate the JSON list.
	The "update" subcommand has no mantadory switches. Omitted optional switches are used with their default parameters.`)

	fsScUpdate.SetOutput(os.Stdout)
	fsScUpdate.PrintDefaults()

	fmt.Println(`subcommand 'v|ver|version': For displaying version information.
	"trice v" will print the version information. In trice is unversioned the build time will be displayed instead.`)
	v.SetOutput(os.Stdout)
	v.PrintDefaults()

	fmt.Println(`subcommand 'zeroSourceTreeIds': Set all Id(n) inside source tree dir to Id(0). 
	Avoid using this subcommand normally. The switch "-src" is mantadory and no multi-flag here.
	This subcommand is mainly for testing. For several source directories you need several runs.`)
	z.SetOutput(os.Stdout)
	z.PrintDefaults()

	fmt.Println("examples:")
	fmt.Println("    'trice update -src ../A -src ../../B': Parse ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json")
	fmt.Println("    'trice l -p COM15 -baud 38400': Display trice log messages from serial port COM15")
	fmt.Println("    'trice l': Display bare data format trice log messages from default source JLINK over Segger RTT protocol.")
	fmt.Println("    'trice zeroSourceTreeIds -dir ../A': Sets all TRICE IDs to 0 in ./A. Use with care!")
	fmt.Println("    'trice l -port STLINK -v -s': Shows verbose version information and also the received raw bytes.")
	return nil
}

// logLoop prepares writing and lut and provides a retry mechanism for unplugged UART.
func logLoop() {
	msg.InfoOnErr("", cipher.SetUp()) // does nothing when -password is "none"
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
		defer func() { msg.InfoOnErr("", rc.Close()) }()
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
