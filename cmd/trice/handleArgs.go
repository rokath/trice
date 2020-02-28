// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package main

import (
	"errors"
	"flag"
	"fmt"
	"log"
	"os"
	"path/filepath"

	"github.com/rokath/trice/pkg/emit"
	"github.com/rokath/trice/pkg/id"
	"github.com/rokath/trice/pkg/receiver"
)

// Key is used to decrypt trices, use empty Key for unencrypted trices
var Key string

// HandleArgs evaluates the arguments slice of strings und uses wd as working directory
func HandleArgs(wd string, args []string) error {
	list := make(id.List, 0, 65536) // for 16 bit IDs enough
	pList := &list

	uCmd := flag.NewFlagSet("update", flag.ExitOnError)                                        // subcommand
	pSrcU := uCmd.String("src", wd, "source dir or file (optional, default is ./)")            // flag
	pDryR := uCmd.Bool("dry-run", false, "no changes are applied (optional)")                  // flag
	pLU := uCmd.String("list", "til.json", "trice ID list path (optional), \"none\" possible") // flag
	pVerb := uCmd.Bool("v", false, "verbose (optional)")                                       // flag

	lCmd := flag.NewFlagSet("log", flag.ExitOnError)                                // subcommand
	pPort := lCmd.String("port", "", "subcommand (required, try COMscan)")          // flag
	pBaud := lCmd.Int("baud", 38400, "COM baudrate (optional, default is 38400")    // flag
	pL := lCmd.String("list", "til.json", "trice ID list path (optional)")          // flag
	pCol := lCmd.String("color", "default", "color set (optional), off, alternate") // flag
	pKey := lCmd.String("key", "", "decrypt passphrase, (optional)")                // flag

	cCmd := flag.NewFlagSet("check", flag.ExitOnError)                              // subcommand
	pSet := cCmd.String("dataset", "position", "parameters (optional), negative")   // flag
	pC := cCmd.String("list", "til.json", "trice ID list path (optional)")          // flag
	pPal := cCmd.String("color", "default", "color set (optional), off, alternate") // flag

	zCmd := flag.NewFlagSet("zeroSourceTreeIds", flag.ContinueOnError)                  // subcommand (during development only)
	pSrcZ := zCmd.String("src", "", "zero all Id(n) inside source tree dir (required)") // flag
	pRunZ := zCmd.Bool("dry-run", false, "no changes are applied (optional)")           // flag

	hCmd := flag.NewFlagSet("help", flag.ContinueOnError) // subcommand

	vCmd := flag.NewFlagSet("version", flag.ContinueOnError) // subcommand

	// Verify that a subcommand has been provided
	// os.Arg[0] is the main command
	// os.Arg[1] will be the subcommand
	if len(os.Args) < 2 {
		return errors.New("no args, try: 'trice help'")
	}

	// Switch on the subcommand
	// Parse the flags for appropriate FlagSet
	// FlagSet.Parse() requires a set of arguments to parse as input
	// os.Args[2:] will be all arguments starting after the subcommand at os.Args[1]
	subCmd := args[1]
	subArgs := args[2:]
	var err error
	switch subCmd { // Check which subcommand is invoked.
	case "h":
		fallthrough
	case "help":
		err = hCmd.Parse(subArgs)
	case "version":
		err = vCmd.Parse(subArgs)
	case "u":
		fallthrough
	case "update":
		err = uCmd.Parse(subArgs)
	case "check":
		err = cCmd.Parse(subArgs)
	case "log":
		err = lCmd.Parse(subArgs)
	case "zeroSourceTreeIds":
		err = zCmd.Parse(subArgs)
	default:
		fmt.Println("try: 'trice help|h'")
		return nil
	}
	if nil != err {
		return fmt.Errorf("failed to parse %s: %v", subArgs, err)
	}
	// Check which subcommand was Parsed using the FlagSet.Parsed() function. Handle each case accordingly.
	// FlagSet.Parse() will evaluate to false if no flags were parsed (i.e. the user did not provide any flags)
	if hCmd.Parsed() {
		return help(hCmd, uCmd, cCmd, lCmd, zCmd, vCmd)
	}
	if uCmd.Parsed() {
		lU, err := filepath.Abs(*pLU)
		// @basti? fmt.Errorf("%s", lU)
		if nil != err {
			return fmt.Errorf("failed to parse %s: %v", *pLU, err)
		}
		srcU, err := filepath.Abs(*pSrcU)
		if nil != err {
			return fmt.Errorf("failed to parse %s: %v", *pSrcU, err)
		}
		return update(*pDryR, srcU, lU, pList, *pVerb)
	}
	if cCmd.Parsed() {
		return checkList(*pC, *pSet, pList, *pPal)
	}
	if lCmd.Parsed() {
		Key = *pKey
		return logTraces(lCmd, *pPort, *pBaud, *pL, pList, *pCol, *pKey)
	}
	if zCmd.Parsed() {
		return zeroIds(*pRunZ, *pSrcZ, zCmd)
	}
	if vCmd.Parsed() {
		return ver()
	}
	return nil
}

func ver() error {
	if "" != version {
		fmt.Printf("version=%v, commit=%v, built at %v", version, commit, date)
		return nil
	}
	fmt.Printf("version=devel, commit=unknown, built after 2020-02-10-1800")
	return errors.New("No goreleaser generated executable")
}

func help(hCmd *flag.FlagSet,
	uCmd *flag.FlagSet,
	cCmd *flag.FlagSet,
	lCmd *flag.FlagSet,
	zCmd *flag.FlagSet,
	vCmd *flag.FlagSet) error {
	fmt.Println("syntax: 'trice subcommand' [params]")
	fmt.Println("subcommand 'help', 'h'")
	hCmd.PrintDefaults()
	fmt.Println("subcommand 'update', 'upd', 'u'")
	uCmd.PrintDefaults()
	fmt.Println("subcommand 'check'")
	cCmd.PrintDefaults()
	fmt.Println("subcommand 'log', 'l'")
	lCmd.PrintDefaults()
	fmt.Println("subcommand 'zeroSourceTreeIds' (avoid using this subcommand normally)")
	zCmd.PrintDefaults()
	fmt.Println("subcommand 'version', 'ver'. 'v'")
	vCmd.PrintDefaults()
	fmt.Println("examples:")
	fmt.Println("    'trice update [-src sourcerootdir]', default sourcerootdir is ./")
	fmt.Println("    'trice log [-port COMn] [-baud m]', default port is COMscan, default m is 38400, fixed to 8N1")
	fmt.Println("    'trice zeroSourceTreeIds -dir sourcerootdir]'")
	fmt.Println("    'trice version'")
	return ver()
}

// parse source tree, update IDs and is list
func update(dryRun bool, dir, fn string, p *id.List, verbose bool) error {
	err := p.Update(dir, fn, !dryRun, verbose)
	if nil != err {
		return fmt.Errorf("failed update on %s with %s: %v", dir, fn, err)
	}
	fmt.Println(len(*p), "ID's in list", fn)
	return nil
}

// log the id list with dataset
func checkList(fn, dataset string, p *id.List, palette string) error {
	err := p.Read(fn)
	if nil != err {
		fmt.Println("ID list " + fn + " not found, exit")
		return nil
	}
	emit.Check(*p, dataset, palette)
	return nil
}

// connect to port and display traces
func logTraces(cmd *flag.FlagSet, port string, baud int, fn string, p *id.List, palette, password string) error {
	if "" == port {
		cmd.PrintDefaults()
		return nil
	}

	if "none" != fn {
		// setup ip list
		err := p.Read(fn)
		if nil != err {
			fmt.Println("ID list " + fn + " not found, exit")
			return nil
		}
	}

	/* TODO: Introduce new command line option for choosing between
	   1) Serial receiver(port name, baudrate, parity bit etc. )
	   2) TCP receiver (IP, port, Protocol (i.e JSON,XML))
	   3) HTTP/Websocket receiver (may be the simplest form in Golang)
	*/

	if port == "COMscan" {
		log.Println("Scan for serial ports...")

		ports, err := receiver.GetSerialPorts()

		if err != nil {
			log.Fatal("Error during serial port search", err)
		}

		if len(ports) > 0 {
			log.Println("Take serial port", ports[0])
			port = ports[0]
		} else {
			log.Fatal("Could not find serial port on system")
			return nil
		}
	}

	serialReceiver := receiver.NewSerialReceiver(port, baud, password)

	if serialReceiver.SetUp() == false {
		fmt.Println("Could not set up serial port", port)
		fmt.Println("try -port COMscan")
		return nil
	} else {
		log.Println("Opened serial port", port)
	}

	log.Println("using id list file", fn, "with", len(*p), "items")

	serialReceiver.Start()
	defer serialReceiver.CleanUp()

	for {
		bytesReceived := <-(*serialReceiver.GetReceiveChannel())

		err := emit.Trice(bytesReceived, *p, palette)
		if nil != err {
			fmt.Println("trice.Log error", err, bytesReceived)
		}
	}

	//return nil // cmd\trice\handleArgs.go:240:2: unreachable code
}

// replace all ID's in sourc tree with 0
func zeroIds(dryRun bool, SrcZ string, cmd *flag.FlagSet) error {
	if SrcZ == "" {
		cmd.PrintDefaults()
		return errors.New("no source tree root specified")
	}
	id.ZeroSourceTreeIds(SrcZ, !dryRun)
	return nil
}
