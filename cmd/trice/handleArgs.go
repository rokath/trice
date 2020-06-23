// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package main implemets the commandline interface and calls the appropriate commands
package main

import (
	"errors"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
	"time"

	"github.com/rokath/trice/internal/cmd"
	"github.com/rokath/trice/internal/disp"
	"github.com/rokath/trice/internal/emit"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/receiver/com"
	"github.com/rokath/trice/internal/receiver/direct"
	"github.com/rokath/trice/internal/receiver/http"
	"github.com/rokath/trice/internal/receiver/jlinkrttlogger"
	"github.com/rokath/trice/internal/receiver/randomdummy"
	"github.com/rokath/trice/internal/receiver/rttfile"
	"github.com/rokath/trice/internal/receiver/segger"
	"github.com/rokath/trice/internal/trice"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/inputdummy"
	"github.com/rokath/trice/pkg/lib"
)

/*
var (
	scLog *flag.FlagSet
)

func init() {
	// example with short version for long flag
	flag.StringVar(scLog, "l", "dada", "short for log") // short flag
}

func init() {
	// example with short version for long flag
	// flag.StringVar(strFlag, "s", "", "Description")
}
*/
// HandleArgs evaluates the arguments slice of strings
func HandleArgs(args []string) error {

	cage.DefaultLogfileName = "2006-01-02_1504-05_trice.log"

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

	hCmd := flag.NewFlagSet("help", flag.ContinueOnError)                                   // subcommand
	pHlf := hCmd.String("logfile", "off", "write output to logfile when set to a filename") // flag
	hCmd.StringVar(pHlf, "lg", "off", "short for -logfile")                                 // short flag

	vCmd := flag.NewFlagSet("version", flag.ContinueOnError)                                             // subcommand
	pVlf := vCmd.String("logfile", "off", "append all output to logfile, set to a filename for logging") // flag
	vCmd.StringVar(pVlf, "lg", "off", "short for -logfile")                                              // short flag

	scLog := flag.NewFlagSet("log", flag.ExitOnError)                                                                                   // subcommand
	pJSON := scLog.String("idlist", "til.json", "trice ID list path")                                                                   // flag
	scLog.StringVar(pJSON, "i", "til.json", "short for -idlist")                                                                        // short flag
	pKey := scLog.String("key", "none", "decrypt passphrase")                                                                           // flag
	scLog.StringVar(pKey, "k", "none", "short for -key")                                                                                // short flag
	pShow := scLog.Bool("show", false, "show passphrase")                                                                               // flag
	pLlf := scLog.String("logfile", cage.DefaultLogfileName, "Append all output to logfile. Set to \"off\" or \"none\" to switch off.") // flag
	scLog.StringVar(pLlf, "lg", cage.DefaultLogfileName, "short for -logfile")                                                          // short flag
	pTs := scLog.String("ts", "LOCmicro", "PC timestamp for logs and logfile name, options: off|UTCmicro")                              // flag
	pCol := scLog.String("color", "default", "color set, options: off|alternate")                                                       // flag
	pLpre := scLog.String("prefix", "COMport:", "prepend prefix to all lines, set to \"off\"")                                          // flag
	pLpost := scLog.String("postfix", "\n", "append postfix to all lines")                                                              // flag

	pPort := scLog.String("port", "COMscan", "COM port, options: COM1|...|COM999")                                      // flag
	pBaud := scLog.Int("baud", 115200, "COM baudrate")                                                                  // flag
	pLdev := scLog.String("device", "COM", "receiver device, options: JLinkRTTLogger, HTTP, RTT, RTTD, SIM, RND, RTTF") // flag
	//pParam := scLog.String("param", "default", "parameters: input device specific string. The \"default\" value is device specific")            // flag

	scCl := flag.NewFlagSet("receiver", flag.ExitOnError)                           // subcommand
	pClPort := scCl.String("port", "COMscan", "COM port, options: COM1|...|COM999") // flag
	pClBaud := scCl.Int("baud", 115200, "COM baudrate")                             // flag
	//pClJSON := scCl.String("list", "til.json", "trice ID list path")                                                                        // flag
	//pClKey := scCl.String("key", "none", "decrypt passphrase")                                                                              // flag
	//pClShow := scCl.Bool("show", false, "show passphrase")                                                                                  // flag
	//pClIPA := scCl.String("ipa", "localhost", "ip address")                                                                                 // flag (127.0.0.1)
	pClIPP := scCl.String("ipp", "61497", "16 bit ip port number") // flag
	//pClTs := scCl.String("ts", "LOCmicro", "PC timestamp for logs, options: off|UTCmicro")                                                  // flag
	pClSrv := scCl.Bool("ds", false, "start display server ") // flag
	//pRpre := scCl.String("prefix", "COMport:", "prepend prefix to all lines, set to \"off\"")                                               // flag
	//pRpost := scCl.String("postfix", "\n", "append postfix to all lines")                                                                   // flag
	//pClLf := scCl.String("lf", cage.DefaultLogfileName, "If '-ds' append all output to logfile. Set to \"off\" or \"none\" to switch off.") // flag
	pRdev := scCl.String("device", "COM", "receiver device, options: JLinkRTTLogger, HTTP, RTT, RTTD, SIM, RND, RTTF") // flag

	scSv := flag.NewFlagSet("displayServer", flag.ExitOnError)                                                                           // subcommand
	pSvIPA := scSv.String("ipa", "localhost", "ip address")                                                                              // flag (127.0.0.1)
	pSvIPP := scSv.String("ipp", "61497", "16 bit port number")                                                                          // flag
	pSvCol := scSv.String("color", "default", "color set, options: off|alternate")                                                       // flag
	pSvLlf := scSv.String("logfile", cage.DefaultLogfileName, "Append all output to logfile. Set to \"off\" or \"none\" to switch off.") // flag
	scLog.StringVar(pSvLlf, "lg", cage.DefaultLogfileName, "short for -logfile")                                                         // short flag
	//pSvTs := scSv.String("ts", "LOCmicro", "timestamp for logfile name, options: off|UTCmicro") // flag

	sCmd := flag.NewFlagSet("scan", flag.ContinueOnError) // subcommand

	scSdSv := flag.NewFlagSet("shutdownServer", flag.ExitOnError)   // subcommand
	pSdSvIPA := scSdSv.String("ipa", "localhost", "ip address")     // flag (127.0.0.1)
	pSdSvIPP := scSdSv.String("ipp", "61497", "16 bit port number") // flag

	// Verify that a subcommand has been provided
	// os.Arg[0] is the main command
	// os.Arg[1] will be the subcommand
	if len(os.Args) < 2 {
		msg := "no args, try: 'trice help'"
		fmt.Println(msg)
		return errors.New(msg)
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
		cage.Name = *pHlf
		return scHelp(hCmd, scUpd, scChk, scLog, scZero, vCmd, scSv, scCl)

	case "s", "sc", "scan":
		sCmd.Parse(subArgs)
		return scScan()

	case "v", "ver", "version":
		vCmd.Parse(subArgs)
		cage.Name = *pVlf
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
		receiver.Device = *pLdev
		com.Port = *pPort
		com.Baud = *pBaud
		setPrefix(*pLpre)
		id.FnJSON = lib.Assign(*pJSON)
		lib.TimeStampFormat = *pTs
		disp.ColorPalette = *pCol
		trice.Password = *pKey
		trice.ShowPassword = *pShow
		cage.Name = *pLlf
		jlinkrttlogger.Param = "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0"

		/*
		   // trice.ScLog is the subcommand log and connects to COM port and displays traces
		   func ScLog() error {
		   	cage.Enable()
		   	defer cage.Disable()

		   	return DoReceive()
		   }*/
		cage.Enable()
		defer cage.Disable()
		//trice.SetUp()
		//trice.DoReceive()
		receiving()
		//return trice.ScLog()

	case "r", "rec", "receiver":
		scCl.Parse(subArgs)
		disp.IPAddr = *pClIPA
		disp.IPPort = *pClIPP
		receiver.Device = *pRdev
		com.Port = *pClPort
		com.Baud = *pClBaud
		setPrefix(*pRpre)
		emit.Postfix = *pRpost
		id.FnJSON = lib.Assign(*pClJSON)
		lib.TimeStampFormat = *pClTs
		trice.Password = *pClKey
		trice.ShowPassword = *pClShow
		cage.Name = *pClLf
		//if true == *pClSrv {
		//	return trice.ScReceive(args[0])
		//}
		//return trice.ScReceive("")

		if true == *pClSrv {
			disp.StartServer(args[0])
		}
		err := trice.Connect("")
		if err != nil {
			fmt.Println(err)
			return err
		}
		cmd.KeyboardInput()
		receiving()
	case "ds", "displayServer":
		scSv.Parse(subArgs)
		disp.ColorPalette = *pSvCol
		disp.IPAddr = *pSvIPA
		disp.IPPort = *pSvIPP
		cage.Name = *pSvLlf
		//lib.TimeStampFormat = *pSvTs
		return disp.ScDisplayServer()

	case "sd", "shutdownRemoteDisplayServer":
		scSdSv.Parse(subArgs)
		disp.IPAddr = *pSdSvIPA
		disp.IPPort = *pSdSvIPP
		return disp.ScShutdownRemoteDisplayServer(1)

	default:
		fmt.Println("try: 'trice help|h'")
		return nil
	}
	return nil
}

/*

// trice.ScLog is the subcommand log and connects to COM port and displays traces
func ScLog() error {
	cage.Enable()
	defer cage.Disable()

	return DoReceive()
}



//// ScReceive is the subcommand remoteDisplay and acts as client connecting to the displayServer
//// sv is the executable name to be started as remote display server (typically arg[0] == trice)
//func ScReceive(sv string) error {
//	err := NewConnection(sv)
//	if err != nil {
//		fmt.Println(err)
//		return err
//	}
//	cmd.KeyboardInput()
//	DoReceive() // does not return
//	return nil
//}
//
//*/
//
///*func do() {
//	/* TODO: Introduce new command line option for choosing between
//	   1) Serial receiver(port name, baudrate, parity bit etc. )
//	   2) TCP receiver (IP, port, Protocol (i.e JSON,XML))
//	   3) HTTP/Websocket receiver (may be the simplest form in Golang)
//	*/
//	switch receiver.Device {
//	case "COM":
//		comPort.DoSerial()
//		/*
//// DoSerial is the endless loop for trice logging over serial port
//func comPort.DoSerial() {
//	err := conditionalComPortScan()
//	if err != nil {
//		return
//	}
//	sR := newSerialReceiver(Port, Baud)
//
//	if sR.setUp() == false {
//		fmt.Println("Could not set up serial port", Port)
//		fmt.Println("try -port COMscan")
//		return
//	}
//	fmt.Println("Opened serial port", Port)
//
//	sR.Start()
//	defer sR.cleanUp()
//
//	sR.DoReceive()
//}
//*/
//	case "RTT":
//		seggerRTT.DoSeggerRTT()
///*
//
//// DoSeggerRTT is the endless loop for trice logging
//func DoSeggerRTT() {
//	rtt := NewSeggerRTTReceiver()
//
//	for nil != rtt.setUp() {
//		fmt.Println("Could not set up", rtt.name, ", trying again...")
//		<-time.After(2 * time.Second)
//		continue
//	}
//	fmt.Println("Opened", rtt.name)
//	defer rtt.cleanUp()
//
//	rtt.Start()
//	defer rtt.Stop()
//
//	rtt.DoReceive()
//}
//*/
//	default:
//		fmt.Println("Unknown receiver device", receiver.Device)
//	}
//}
//*/

func receiving() {
	var n int
	err := trice.SetUp()
	if nil != err {
		return
	}
	var r io.ReadCloser
	switch receiver.Device {
	case "JLinkRTTLogger":
		l := jlinkrttlogger.New(jlinkrttlogger.Param) // yes
		if nil != l.Open() {
			return
		}
		defer l.Close()
		r = l
	case "HTTP":
		h := http.New()
		if false == h.Open() {
			return
		}
		r = h
	case "RND":
		n = 19 // you will see n-7 discarded bytes because 7 bytes are held internally to try to sync a wrap
		innerReader := randomdummy.New(randomdummy.ZeroRandomSeed, randomdummy.ChaosMode, randomdummy.NoDelay, n)
		r = ioutil.NopCloser(innerReader) // https://stackoverflow.com/questions/28158990/golang-io-ioutil-nopcloser
	case "COM":
		c := com.New()
		if false == c.Open() {
			return
		}
		r = c
	case "SIM":
		n = 50 // you will see about n/8 lines
		i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', '\n', 235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0, 235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 1, 1}
		//i := []byte{235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0}
		s := inputdummy.New(i, time.Millisecond, 50)
		receiver.DiscardByte = receiver.DiscardASCII
		r = ioutil.NopCloser(s) // https://stackoverflow.com/questions/28158990/golang-io-ioutil-nopcloser
	case "RTT":
		receiver.DiscardByte = receiver.DiscardASCII
		s := segger.New()
		if nil != s.Open() {
			return
		}
		r = s
	case "RTTD":
		receiver.DiscardByte = receiver.DiscardASCII
		d := direct.New()
		if nil != d.Open() {
			return
		}
		r = d
	case "RTTF":
		s := rttfile.New()
		fn := "c:/repos/trice/rttfile.bin"
		err := s.Open(fn)
		if nil != err {
			fmt.Println(err)
			return
		}
		r = s
	default:
		fmt.Println(receiver.Device, "is unknown as device")
		return
	}
	t := receiver.New(r)

	t.Start()
	/*go func() {
		for {
			time.Sleep(time.Second)
		}
	}()*/
}

func scScan() error {
	com.Port = "COMscan"
	_, err := com.GetSerialPorts()
	return err
}

func scVersion() error {
	cage.Enable()
	defer cage.Disable()
	log.SetFlags(0)
	if "" != version {
		log.Printf("version=%v, commit=%v, built at %v\n", version, commit, date)
	} else {
		log.Printf("version=devel, built %s\n", linkTime)
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

	cage.Enable()
	defer cage.Disable()

	/* debug code:
	Problem:
	cage.Enable() re-direkted os.Stderr und dupliziert so Ausgaben an os.Sterr in trice.log
	hCmd.Output() usw. geben os.Stderr zurück. Das ist auch wirklich die re-directed Adresse.
	Der Paketest von cage funktioniert, auch TestScHelp() geht fehlerfrei durch.
	Wenn aber über die Kommandozeile "trice h" erfolgt, landen Ausgaben an os.Stderr NICHT in trice.log obwohl os.Stderr re-directed ist.
	*/
	/*
		log.SetFlags(0)
		fmt.Println("0: os.Stderr     addr:", os.Stderr) // ok not in trice.log
		cage.Enable()
		defer cage.Disable()
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
	case "off", "none":
		emit.Prefix = ""
	case "COMport:":
		emit.Prefix = receiver.Device + " " // com.Port + "  " //
	default:
		emit.Prefix = s
	}
}
