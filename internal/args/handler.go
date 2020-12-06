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

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/translator"
	"github.com/rokath/trice/pkg/cage"
)

// Handler is called in main, evaluates args and calls the appropriate functions.
// It returns for program exit.
func Handler(args []string) error {

	if "" == Date { // goreleaser will set Date, otherwise use file info.
		fi, err := os.Stat(os.Args[0])
		if nil == err { // On running main tests fileinfo is invalid, so do not use in that case.
			Date = fi.ModTime().String()
		}
	}
	cage.DefaultLogfileName = "2006-01-02_1504-05_trice.log"

	// Verify that a subcommand has been provided: os.Arg[0] is the main command (trice), os.Arg[1] will be the subcommand.
	if len(os.Args) < 2 {
		msg := "no args, try: 'trice help'"
		fmt.Println(msg)
		return errors.New(msg)
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
		fsScHelp.Parse(subArgs)
		distributeArgs()
		return scHelp(fsScLog, fsScZero, fsScVersion, fsScSv, fsScSdSv)
	case "s", "sc", "scan":
		fsScScan.Parse(subArgs)
		distributeArgs()
		_, err := com.GetSerialPorts()
		return err
	case "v", "ver", "version":
		fsScVersion.Parse(subArgs)
		distributeArgs()
		return scVersion()
	case "u", "update":
		fsScUpdate.Parse(subArgs)
		distributeArgs()
		fnJSON = id.ConditionalFilePath(fnJSON)
		return id.ScUpdate(fnJSON)
	case "zeroSourceTreeIds":
		fsScZero.Parse(subArgs)
		distributeArgs()
		return id.ScZero(*pSrcZ, fsScZero)
	case "sd", "sdds", "sdrds", "shutdownRemoteDisplayServer":
		fsScSdSv.Parse(subArgs)
		distributeArgs()
		return emitter.ScShutdownRemoteDisplayServer(1)
	case "ds", "displayServer":
		fsScSv.Parse(subArgs)
		distributeArgs()
		return emitter.ScDisplayServer() // endless loop
	case "l", "log":
		fsScLog.Parse(subArgs)
		distributeArgs()
		doReceive() // endless loop
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

	fmt.Println("subcommand 'help', 'h' for command line usage")
	fsScHelp.SetOutput(os.Stdout)
	fsScHelp.PrintDefaults()

	fmt.Println("subcommand 'u', 'upd', 'update' For updating ID list and source files.")
	fsScUpdate.SetOutput(os.Stdout)
	fsScUpdate.PrintDefaults()

	fmt.Println("subcommand 'l', 'log' For displaying trice logs coming from port.")
	l.SetOutput(os.Stdout)
	fmt.Println("args is used as optional port parameters. The \"default\" value is replaced with:")
	fmt.Printf("\t\"COMn\" port: \"%s\", Option for args is \"TARM\" to use a different driver. (For baud rate settings see -baud.)\n", defaultCOMArgs)
	fmt.Printf(`%s"JLINK" or "STLINK" port: "%s", For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf. (The -RTTSearchRanges "..." need to be written without "" and with _ istead of space.)%s`, "\t", defaultLinkArgs, "\n")
	fmt.Printf("\t\"BUFFER\" port: \"%s\", Option for args is any byte sequence.\n", defaultBUFFERArgs)
	l.PrintDefaults()

	fmt.Println("subcommand 'zeroSourceTreeIds' For setting all TRICE IDs to 0 in source tree.")
	z.SetOutput(os.Stdout)
	z.PrintDefaults()

	fmt.Println("subcommand 'v', 'ver', 'version' For displaying version information.")
	v.SetOutput(os.Stdout)
	v.PrintDefaults()

	fmt.Println("subcommand 'ds', 'displayServer' Starts a display server, use in a separate console, on Windows use wt or a linux shell like git-bash to avoid color issues, several instances of 'trice l -ds' will send output there")
	sv.SetOutput(os.Stdout)
	sv.PrintDefaults()

	fmt.Println("subcommand 'sd', 'shutdownServer' Ends display server at IPA:IPP, works also on a remote mashine.")
	scSdSv.SetOutput(os.Stdout)
	scSdSv.PrintDefaults()

	fmt.Println("examples:")
	fmt.Println("    'trice update -src ../A -src ../../B' parses ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json")
	fmt.Println("    'trice l -p COM15 -baud 38400' Display trice log messages from serial port COM15")
	fmt.Println("    'trice l' Display bare data format trice log messages from default source JLINK over Segger RTT protocol.")
	fmt.Println("    'trice zeroSourceTreeIds -dir ../A' Sets all TRICE IDs to 0 in ./A.")
	fmt.Println("    'trice v -v' Shows verbose version information.")
	return nil
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

// replaceDefaultArgs assigns port specific default strings.
func replaceDefaultArgs() {
	if strings.HasPrefix(port, "COM") {
		portArguments = defaultCOMArgs
	} else {
		switch port {
		case "JLINK", "STLINK":
			portArguments = defaultLinkArgs
		case "BUFFER":
			portArguments = defaultBUFFERArgs
		}
	}
}

// distributeArgs is distibuting values used in several packages.
// It must not be called before the appropriate arg parsing.
func distributeArgs() {
	replaceDefaultArgs()
	com.Verbose = verbose
	id.Verbose = verbose
	emitter.Verbose = verbose
	link.Verbose = verbose
	cage.Verbose = verbose
	receiver.Verbose = verbose
	translator.Verbose = verbose
}
