// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package main implemets the commandline interface and calls the appropriate commands
package main

import (
	"errors"
	"flag"
	"fmt"
	"log"
	"os"

	"github.com/rokath/trice/pkg/disp"
	"github.com/rokath/trice/pkg/emit"
	"github.com/rokath/trice/pkg/id"
	"github.com/rokath/trice/pkg/lgf"
	"github.com/rokath/trice/pkg/lib"
	"github.com/rokath/trice/pkg/receiver"
	"github.com/rokath/trice/pkg/trice"
)

// HandleArgs evaluates the arguments slice of strings
func HandleArgs(args []string) error {

	//pList = &list

	scUpd := flag.NewFlagSet("update", flag.ExitOnError)                                   // subcommand
	pDryR := scUpd.Bool("dry-run", false, "no changes are applied")                        // flag
	pLU := scUpd.String("list", "til.json", "trice ID list path, \"none\" possible")       // flag
	pVerb := scUpd.Bool("v", false, "verbose")                                             // flag
	scUpd.Var(&lib.Srcs, "src", "source dir or file, multi use possible (default \"./\")") // multi flag

	scChk := flag.NewFlagSet("check", flag.ExitOnError)                           // subcommand
	pSet := scChk.String("dataset", "position", "parameters, option: negative")   // flag
	pClist := scChk.String("list", "til.json", "trice ID list path")              // flag
	pPal := scChk.String("color", "default", "color set, options: off|alternate") // flag

	scZero := flag.NewFlagSet("zeroSourceTreeIds", flag.ContinueOnError)                  // subcommand (during development only)
	pSrcZ := scZero.String("src", "", "zero all Id(n) inside source tree dir (required)") // flag
	pRunZ := scZero.Bool("dry-run", false, "no changes are applied")                      // flag

	hCmd := flag.NewFlagSet("help", flag.ContinueOnError)                                            // subcommand
	pHlf := hCmd.String("lf", "trice.log", "append output to logfile, set to \"off\" to avoid this") // flag

	vCmd := flag.NewFlagSet("version", flag.ContinueOnError)                               // subcommand
	pVlf := vCmd.String("lf", "trice.log", "append all output to logfile, set to \"off\"") // flag

	scLog := flag.NewFlagSet("log", flag.ExitOnError)                                          // subcommand
	pPort := scLog.String("port", "COMscan", "COM port, options: COM1|...|COM999")             // flag
	pBaud := scLog.Int("baud", 115200, "COM baudrate")                                         // flag
	pJSON := scLog.String("list", "til.json", "trice ID list path")                            // flag
	pTs := scLog.String("ts", "LOCmicro", "timestamp, options: off|UTCmicro")                  // flag
	pCol := scLog.String("color", "default", "color set, options: off|alternate")              // flag
	pKey := scLog.String("key", "none", "decrypt passphrase")                                  // flag
	pShow := scLog.Bool("show", false, "show passphrase")                                      // flag
	pLlf := scLog.String("lf", "trice.log", "append all output to logfile, set to \"off\"")    // flag
	pLpre := scLog.String("prefix", "COMport:", "prepend prefix to all lines, set to \"off\"") // flag
	pLpost := scLog.String("postfix", "\n", "append postfix to all lines")                     // flag

	scCl := flag.NewFlagSet("receiver", flag.ExitOnError)                                     // subcommand
	pClPort := scCl.String("port", "COMscan", "COM port, options: COM1|...|COM999")           // flag
	pClBaud := scCl.Int("baud", 115200, "COM baudrate")                                       // flag
	pClJSON := scCl.String("list", "til.json", "trice ID list path")                          // flag
	pClKey := scCl.String("key", "none", "decrypt passphrase")                                // flag
	pClShow := scCl.Bool("show", false, "show passphrase")                                    // flag
	pClIPA := scCl.String("ipa", "localhost", "ip address")                                   // flag (127.0.0.1)
	pClIPP := scCl.String("ipp", "61497", "16 bit ip port number")                            // flag
	pClTs := scCl.String("ts", "LOCmicro", "timestamp, options: off|UTCmicro")                // flag
	pClSrv := scCl.Bool("ds", false, "start display server ")                                 // flag
	pRpre := scCl.String("prefix", "COMport:", "prepend prefix to all lines, set to \"off\"") // flag
	pRpost := scCl.String("postfix", "\n", "append postfix to all lines")                     // flag

	scSv := flag.NewFlagSet("displayServer", flag.ExitOnError)                               // subcommand
	pSvIPA := scSv.String("ipa", "localhost", "ip address")                                  // flag (127.0.0.1)
	pSvIPP := scSv.String("ipp", "61497", "16 bit port number")                              // flag
	pSvCol := scSv.String("color", "default", "color set, options: off|alternate")           // flag
	pSvLlf := scSv.String("lf", "trice.log", "append all output to logfile, set to \"off\"") // flag
	//pSvTs := scSv.String("ts", "LOCmicro", "timestampm options: off|UTCmicro")               // flag

	sCmd := flag.NewFlagSet("scan", flag.ContinueOnError) // subcommand

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
	switch subCmd { // Check which subcommand is invoked.

	case "h", "help":
		hCmd.Parse(subArgs)
		lgf.Name = *pHlf
		return scHelp(hCmd, scUpd, scChk, scLog, scZero, vCmd, scSv, scCl)

	case "s", "sc", "scan":
		sCmd.Parse(subArgs)
		return scScan()

	case "v", "ver", "version":
		vCmd.Parse(subArgs)
		lgf.Name = *pVlf
		return scVersion()

	case "u", "update":
		scUpd.Parse(subArgs)
		id.FnJSON = lib.Assign(*pLU)
		id.Verbose = *pVerb
		id.DryRun = *pDryR
		return id.ScUpdate()

	case "check":
		scChk.Parse(subArgs)
		disp.ColorPalette = *pPal
		id.FnJSON = lib.Assign(*pClist)
		return emit.ScCheckList(*pSet)

	case "zeroSourceTreeIds":
		scZero.Parse(subArgs)
		id.DryRun = *pRunZ
		return id.ScZero(*pSrcZ, scZero)

	case "l", "log":
		scLog.Parse(subArgs)
		emit.Postfix = *pLpost
		receiver.Port = *pPort
		receiver.Baud = *pBaud
		setPrefix(*pLpre)
		id.FnJSON = lib.Assign(*pJSON)
		emit.TimeStampFormat = *pTs
		disp.ColorPalette = *pCol
		trice.Password = *pKey
		trice.ShowPassword = *pShow
		lgf.Name = *pLlf
		return trice.ScLog()

	case "r", "rec", "receiver":
		scCl.Parse(subArgs)
		disp.IPAddr = *pClIPA
		disp.IPPort = *pClIPP
		receiver.Port = *pClPort
		receiver.Baud = *pClBaud
		setPrefix(*pRpre)
		emit.Postfix = *pRpost
		id.FnJSON = lib.Assign(*pClJSON)
		emit.TimeStampFormat = *pClTs
		trice.Password = *pClKey
		trice.ShowPassword = *pClShow
		return trice.ScReceive(*pClSrv)

	case "ds", "displayServer":
		scSv.Parse(subArgs)
		disp.ColorPalette = *pSvCol
		disp.IPAddr = *pSvIPA
		disp.IPPort = *pSvIPP
		lgf.Name = *pSvLlf
		return disp.ScDisplayServer()

	default:
		fmt.Println("try: 'trice help|h'")
		return nil
	}
}

func scScan() error {
	receiver.Port = "COMscan"
	_, err := receiver.GetSerialPorts()
	return err
}

func scVersion() error {
	lgf.Enable()
	defer lgf.Disable()
	//log.SetFlags(0)
	if "" != version {
		log.Printf("version=%v, commit=%v, built at %v\n", version, commit, date)
	} else {
		s := "version=devel, commit=unknown, built after 2020-04-15-2206"
		log.Printf("fmt %s\n", s)
	}
	return nil
}

// scHelp is subcommand help
func scHelp(
	hCmd *flag.FlagSet,
	u *flag.FlagSet,
	c *flag.FlagSet,
	l *flag.FlagSet,
	z *flag.FlagSet,
	v *flag.FlagSet,
	sv *flag.FlagSet,
	cl *flag.FlagSet) error {

	lgf.Enable()
	defer lgf.Disable()

	/* debug code:
	Problem:
	lgf.Enable() re-direkted os.Stderr und dupliziert so Ausgaben an os.Sterr in trice.log
	hCmd.Output() usw. geben os.Stderr zurück. Das ist auch wirklich die re-directed Adresse.
	Der Paketest von cage funktioniert, auch TestScHelp() geht fehlerfrei durch.
	Wenn aber über die Kommandozeile "trice h" erfolgt, landen Ausgaben an os.Stderr NICHT in trice.log obwohl os.Stderr re-directed ist.
	*/
	/*
		log.SetFlags(0)
		fmt.Println("0: os.Stderr     addr:", os.Stderr) // ok not in trice.log
		lgf.Enable()
		defer lgf.Disable()
		fmt.Println("1: os.Stderr     addr:", os.Stderr)     // ok in trice.log (os.Stdout)
		fmt.Println("2: hCmd.Output() addr:", hCmd.Output()) // ok in trice.log (os.Stdout)

		fmt.Fprintln(os.Stderr, "3:      OS.STDERR")       // NOT in trice.log
		fmt.Fprintln(hCmd.Output(), "4:    hCmd.Output()") // NOT in trice.log
		log.Println("5: TryIt")                            // ok in trice.log (log out)
		fmt.Println("6: TryIt2")                           // ok in trice.log (os.Stdout)
		fmt.Fprintln(os.Stdout, "7:      OS.STDOUT")       // ok in trice.log (os.Stdout)
	*/
	fmt.Fprintln(hCmd.Output(), "syntax: 'trice subcommand' [params]")
	fmt.Fprintln(hCmd.Output(), "subcommand 'help', 'h'")
	hCmd.PrintDefaults()
	fmt.Fprintln(u.Output(), "subcommand 'u', 'upd', 'update'")
	u.PrintDefaults()
	fmt.Fprintln(c.Output(), "subcommand 'check'")
	c.PrintDefaults()
	fmt.Fprintln(l.Output(), "subcommand 'l', 'log'")
	l.PrintDefaults()
	fmt.Fprintln(z.Output(), "subcommand 'zeroSourceTreeIds' (avoid using this subcommand normally)")
	z.PrintDefaults()
	fmt.Fprintln(v.Output(), "subcommand 'v', 'ver', 'version'")
	v.PrintDefaults()
	fmt.Fprintln(sv.Output(), "subcommand 'ds', 'displayServer'")
	sv.PrintDefaults()
	fmt.Fprintln(cl.Output(), "subcommand 'r', 'rec', 'receiver'")
	cl.PrintDefaults()
	fmt.Fprintln(hCmd.Output(), "examples:")
	fmt.Fprintln(hCmd.Output(), "    'trice update [-src sourcerootdir]', default sourcerootdir is ./")
	fmt.Fprintln(hCmd.Output(), "    'trice log [-port COMn] [-baud m]', default port is COMscan, default m is 38400, fixed to 8N1")
	fmt.Fprintln(hCmd.Output(), "    'trice zeroSourceTreeIds -dir sourcerootdir]'")
	fmt.Fprintln(hCmd.Output(), "    'trice version'")
	return nil
}

func setPrefix(s string) {
	switch s {
	case "off":
		emit.Prefix = ""
	case "COMport:":
		emit.Prefix = receiver.Port + "  "
	default:
		emit.Prefix = s
	}
}
