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
	"os"
	"strings"
	"time"

	"github.com/rokath/trice/internal/cmd"
	"github.com/rokath/trice/internal/disp"
	"github.com/rokath/trice/internal/emit"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/receiver/com"
	"github.com/rokath/trice/internal/receiver/direct"
	"github.com/rokath/trice/internal/receiver/http"
	"github.com/rokath/trice/internal/receiver/jlink"
	"github.com/rokath/trice/internal/receiver/randomdummy"
	"github.com/rokath/trice/internal/receiver/rttfile"
	"github.com/rokath/trice/internal/receiver/segger"
	"github.com/rokath/trice/internal/trice"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/inputdummy"
	"github.com/rokath/trice/pkg/lib"
)

var (
	// rndMode is flag value for random mode
	rndMode string

	// rndLimit is flag vaue for byte count limit in random mode
	rndLimit int

	// displayserver if set, sends trice lines over TCP
	displayserver bool
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
	var vLgOff string // for default logfile=off cases (version & help)
	cage.DefaultLogfileName = "2006-01-02_1504-05_trice.log"

	scUpdate := flag.NewFlagSet("update", flag.ExitOnError)                                       // subcommand
	scUpdate.BoolVar(&id.DryRun, "dry-run", false, "no changes are applied")                      // flag
	scUpdate.StringVar(&id.FnJSON, "idlist", "til.json", "trice ID list path, \"none\" possible") // flag
	scUpdate.StringVar(&id.FnJSON, "i", "til.json", "trice ID list path, \"none\" possible")      // flag
	scUpdate.BoolVar(&id.Verbose, "v", false, "verbose")                                          // flag
	scUpdate.Var(&lib.Srcs, "src", "source dir or file, multi use possible (default \"./\")")     // multi flag

	scCheck := flag.NewFlagSet("check", flag.ExitOnError)                                          // subcommand
	pSet := scCheck.String("dataset", "position", "parameters, option: negative")                  // flag
	scCheck.StringVar(&id.FnJSON, "idlist", "til.json", "trice ID list path")                      // flag
	scCheck.StringVar(&id.FnJSON, "i", "til.json", "trice ID list path")                           // flag
	scCheck.StringVar(&disp.ColorPalette, "color", "default", "color set, options: off|alternate") // flag
	scCheck.BoolVar(&id.Verbose, "v", false, "verbose")                                            // flag

	scZero := flag.NewFlagSet("zeroSourceTreeIds", flag.ContinueOnError)                  // subcommand (during development only)
	pSrcZ := scZero.String("src", "", "zero all Id(n) inside source tree dir (required)") // flag
	scZero.BoolVar(&id.DryRun, "dry-run", false, "no changes are applied")                // flag

	hCmd := flag.NewFlagSet("help", flag.ContinueOnError)                                       // subcommand
	hCmd.StringVar(&vLgOff, "logfile", "off", "write output to logfile when set to a filename") // flag
	hCmd.StringVar(&vLgOff, "lg", "off", "short for -logfile")                                  // short flag

	vCmd := flag.NewFlagSet("version", flag.ContinueOnError)                                                 // subcommand
	vCmd.StringVar(&vLgOff, "logfile", "off", "append all output to logfile, set to a filename for logging") // flag
	vCmd.StringVar(&vLgOff, "lg", "off", "short for -logfile")                                               // short flag

	scLog := flag.NewFlagSet("log", flag.ExitOnError)                                                                                                                    // subcommand
	scLog.StringVar(&id.FnJSON, "idlist", "til.json", "trice ID list path")                                                                                              // short flag
	scLog.StringVar(&id.FnJSON, "i", "til.json", "short for -idlist")                                                                                                    // short flag
	scLog.StringVar(&trice.Password, "key", "none", "decrypt passphrase")                                                                                                // flag
	scLog.StringVar(&trice.Password, "k", "none", "short for -key")                                                                                                      // short flag
	scLog.BoolVar(&trice.ShowPassword, "show", false, "show passphrase")                                                                                                 // flag
	scLog.StringVar(&cage.Name, "logfile", "off" /*cage.DefaultLogfileName*/, "Append all output to logfile. Set to \"off\" or \"none\" to switch off.")                 // flag
	scLog.StringVar(&cage.Name, "lg", "off" /*cage.DefaultLogfileName*/, "short for -logfile")                                                                           // short flag
	scLog.StringVar(&lib.TimeStampFormat, "ts", "LOCmicro", "PC timestamp for logs and logfile name, options: off|UTCmicro")                                             // flag
	scLog.StringVar(&disp.ColorPalette, "color", "default", "color set, options: off|alternate")                                                                         // flag
	scLog.StringVar(&emit.Prefix, "prefix", "source:", "prepend prefix to all lines, set to \"off\"")                                                                    // flag
	scLog.StringVar(&emit.Postfix, "postfix", "\n", "append postfix to all lines")                                                                                       // flag
	scLog.BoolVar(&id.Verbose, "v", false, "verbose")                                                                                                                    // flag
	scLog.StringVar(&receiver.Source, "source", "JLINK", "receiver device, options: COMn, JLINK, STLINK, filename, SIM, RND")                                            //HTTP, RTT, RTTD, RTTF")                                             // flag
	scLog.StringVar(&receiver.Source, "s", "JLINK", "short for -source")                                                                                                 // short flag
	scLog.IntVar(&com.Baud, "baud", 115200, "Only for -s COMn, COM baudrate")                                                                                            // flag flag
	scLog.StringVar(&jlink.Param, "jlink", "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0", "Only for -s JLRTT, see JLinkRTTLogger in SEGGER UM08001_JLink.pdf") // JLRTT flag
	scLog.StringVar(&rndMode, "rndMode", "WrapModeWithValidCrc", "Only for -s RND, see randomdummy.go, Options are ChaosMode, BareModeNoSync")
	scLog.IntVar(&rndLimit, "rndLimit", randomdummy.NoLimit, "Only for -s RND, see randomdummy.go, Options are count of bytes, 0 for unlimited count")
	scLog.StringVar(&cage.Name, "remote", "off" /*cage.DefaultLogfileName*/, "Append all output to logfile. Set to \"off\" or \"none\" to switch off.") // flag
	scLog.StringVar(&cage.Name, "r", "off", "short for -remote")                                                                                        // short flag
	scLog.BoolVar(&displayserver, "ds", false, "send trice lines to displayserver @ IPA:IPP")
	scLog.BoolVar(&displayserver, "displayserver", false, "send trice lines to displayserver @ ipa:ipp")
	scLog.StringVar(&disp.IPAddr, "ipa", "localhost", "ip address")     // flag (127.0.0.1)
	scLog.StringVar(&disp.IPPort, "ipp", "61497", "16 bit port number") // flag

	scSv := flag.NewFlagSet("displayServer", flag.ExitOnError)                                                                                // subcommand
	scSv.StringVar(&disp.IPAddr, "ipa", "localhost", "ip address")                                                                            // flag (127.0.0.1)
	scSv.StringVar(&disp.IPPort, "ipp", "61497", "16 bit port number")                                                                        // flag
	scSv.StringVar(&disp.ColorPalette, "color", "default", "color set, options: off|alternate")                                               // flag
	scSv.StringVar(&cage.Name, "logfile", cage.DefaultLogfileName, "Append all output to logfile. Set to \"off\" or \"none\" to switch off.") // flag
	scSv.StringVar(&cage.Name, "lg", cage.DefaultLogfileName, "short for -logfile")                                                           // short flag

	sCmd := flag.NewFlagSet("scan", flag.ContinueOnError) // subcommand

	scSdSv := flag.NewFlagSet("shutdownServer", flag.ExitOnError)        // subcommand
	scSdSv.StringVar(&disp.IPAddr, "ipa", "localhost", "ip address")     // flag (127.0.0.1)
	scSdSv.StringVar(&disp.IPPort, "ipp", "61497", "16 bit port number") // flag

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
		cage.Name = vLgOff
		return scHelp(hCmd, scUpdate, scCheck, scLog, scZero, vCmd, scSv)

	case "s", "sc", "scan":
		sCmd.Parse(subArgs)
		return scScan()

	case "v", "ver", "version":
		vCmd.Parse(subArgs)
		cage.Name = vLgOff
		return scVersion()

	case "u", "update":
		scUpdate.Parse(subArgs)
		id.FnJSON = lib.ConditinalFilePath(id.FnJSON)
		return id.ScUpdate()

	case "check":
		scCheck.Parse(subArgs)
		id.FnJSON = lib.ConditinalFilePath(id.FnJSON)
		return emit.ScCheckList(*pSet)

	case "zeroSourceTreeIds":
		scZero.Parse(subArgs)
		return id.ScZero(*pSrcZ, scZero)

	case "l", "log":
		scLog.Parse(subArgs)
		jlink.Verbose = id.Verbose
		cage.Verbose = id.Verbose
		// adjust settings
		if "source:" == emit.Prefix {
			emit.Prefix = receiver.Source + ":"
		}
		setPrefix(emit.Prefix)
		if strings.HasPrefix(receiver.Source, "COM") {
			com.Port = receiver.Source // set COM port number
			receiver.Source = "COM"
		}
		id.FnJSON = lib.ConditinalFilePath(id.FnJSON)

		if false == displayserver {
			cage.Enable()
			defer cage.Disable()
			receiving()
		} else {
			//if true == *pClSrv {
			//	disp.StartServer(args[0])
			//}
			err := trice.Connect("")
			if err != nil {
				fmt.Println(err)
				return err
			}
			cmd.KeyboardInput()
			receiving()
		}
	case "ds", "displayServer":
		scSv.Parse(subArgs)
		return disp.ScDisplayServer()

	case "sd", "shutdownRemoteDisplayServer":
		scSdSv.Parse(subArgs)
		return disp.ScShutdownRemoteDisplayServer(1)

	default:
		fmt.Println("try: 'trice help|h'")
		return nil
	}
	return nil
}

func receiving() {
	//var n int

	// prepare
	err := trice.SetUp()
	if nil != err {
		return
	}
	var r io.ReadCloser
	switch receiver.Source {
	case "JLINK":
		l := jlink.New(jlink.Param) // yes
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
		// rndLimit = 19: you will see n-7 discarded bytes because 7 bytes are held internally to try to sync a wrap
		innerReader := randomdummy.New(randomdummy.ZeroRandomSeed, rndMode, randomdummy.DefaultDelay, rndLimit)
		r = ioutil.NopCloser(innerReader) // https://stackoverflow.com/questions/28158990/golang-io-ioutil-nopcloser
	case "COM":
		c := com.New()
		if false == c.Open() {
			return
		}
		r = c
	case "SIM":
		n := 50 // you will see about n/8 lines
		i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', '\n',
			235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0,
			235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 1, 1}
		//i := []byte{235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0}
		s := inputdummy.New(i, time.Millisecond, n)
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
		fmt.Println(receiver.Source, "is unknown as device")
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
	//log.SetFlags(0) // changed to fmt, because log got not captured by
	if "" != version {
		fmt.Printf("version=%v, commit=%v, built at %v\n", version, commit, date)
	} else {
		fmt.Printf("version=devel, built %s\n", linkTime)
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
	sv *flag.FlagSet) error {

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
		emit.Prefix = receiver.Source + " " // com.Port + "  " //
	default:
		emit.Prefix = s + " "
	}
}

// LEFTOVERS

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
