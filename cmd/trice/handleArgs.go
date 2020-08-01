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
	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/disp"
	"github.com/rokath/trice/internal/emit"
	"github.com/rokath/trice/internal/global"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/jlink"
	"github.com/rokath/trice/internal/randomdummy"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/receiver/http"
	"github.com/rokath/trice/internal/receiver/rttfile"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/cipher"
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

	// autostart if set, starts an additional trice instamce as displayserver
	autostart bool
)

func flagLogfile(p *flag.FlagSet) {
	p.StringVar(&cage.Name, "logfile", "off", "append all output to logfile, options: 'none|filename|auto', 'auto' for \"2006-01-02_1504-05_trice.log\" with actual time") // flag
	p.StringVar(&cage.Name, "lg", "off", "short for -logfile")                                                                                                             // short flag
}

func flagVerbosity(p *flag.FlagSet) {
	p.BoolVar(&global.Verbose, "v", false, "verbose, more informal output if used") // flag
}

func flagIDList(p *flag.FlagSet) {
	p.StringVar(&id.FnJSON, "idlist", "til.json", "trice ID list path, 'none' possible") // flag
	p.StringVar(&id.FnJSON, "i", "til.json", "short for '-idlist'")                      // flag
}

func flagIPAddress(p *flag.FlagSet) {
	p.StringVar(&disp.IPAddr, "ipa", "localhost", "ip address like '127.0.0.1'") // flag
	p.StringVar(&disp.IPPort, "ipp", "61497", "16 bit port number")              // flag
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
	scSdSv *flag.FlagSet,
) error {
	if global.Verbose {
		fmt.Printf("\n*** https://github.com/rokath/trice ***\n\n")
		fmt.Printf("If a non-multi parameter is used more than one times the last value wins.\n")
	}
	cage.Enable()
	defer cage.Disable()

	fmt.Fprintln(hCmd.Output(), "syntax: 'trice subcommand' [params]")
	fmt.Fprintln(hCmd.Output(), "subcommand 'help', 'h' for command line usage")
	hCmd.PrintDefaults()
	fmt.Fprintln(u.Output(), "subcommand 'u', 'upd', 'update' for updating ID list and source files")
	u.PrintDefaults()
	fmt.Fprintln(c.Output(), "subcommand 'check' for dispaying the ID list in trice log format")
	c.PrintDefaults()
	fmt.Fprintln(l.Output(), "subcommand 'l', 'log' for displaying trice logs coming from source")
	l.PrintDefaults()
	fmt.Fprintln(z.Output(), "subcommand 'zeroSourceTreeIds' for setting all TRICE IDs to 0 in source tree, avoid using this subcommand normally")
	z.PrintDefaults()
	fmt.Fprintln(v.Output(), "subcommand 'v', 'ver', 'version' for displaying version information")
	v.PrintDefaults()
	fmt.Fprintln(sv.Output(), "subcommand 'ds', 'displayServer' starts a display server, use in a separate console, on Windows use wt or a linux shell like git-bash to avoid color issues, several instances of 'trice l -ds' will send output there")
	sv.PrintDefaults()
	fmt.Fprintln(scSdSv.Output(), "subcommand 'sd', 'shutdownServer' ends display server at IPA:IPP, works also on a remote mashine")
	scSdSv.PrintDefaults()
	fmt.Fprintln(hCmd.Output(), "examples:")
	fmt.Fprintln(hCmd.Output(), "    'trice update -src ../A -src ../../B' parses ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json")
	fmt.Fprintln(hCmd.Output(), "    'trice l -s COM15 -baud 38400 -d wrap display wrap data format trice log messages from COM15")
	fmt.Fprintln(hCmd.Output(), "    'trice l display bare data format trice log messages from default source")
	fmt.Fprintln(hCmd.Output(), "    'trice zeroSourceTreeIds -dir ../A' sets all TRICE IDs to 0 in ./A")
	fmt.Fprintln(hCmd.Output(), "    'trice v -v' shows verbose version information")
	return nil
}

// Encoding describes the way the byte stream is coded. TODO: Change to MultiArgs
var Encoding string

// HandleArgs evaluates the arguments slice of strings
func HandleArgs(args []string) error {
	cage.DefaultLogfileName = "2006-01-02_1504-05_trice.log"

	scCheck := flag.NewFlagSet("check", flag.ExitOnError)                                       // subcommand
	pSet := scCheck.String("dataset", "position", "parameters, option: 'negative'")             // flag
	scCheck.StringVar(&disp.ColorPalette, "color", "default", "color set, options: 'off|none'") // flag
	flagLogfile(scCheck)
	flagVerbosity(scCheck)
	flagIDList(scCheck)

	scUpdate := flag.NewFlagSet("update", flag.ExitOnError)                                // subcommand
	scUpdate.BoolVar(&id.DryRun, "dry-run", false, "no changes are applied")               // flag
	scUpdate.Var(&lib.Srcs, "src", "source dir or file, multi use possible, default './'") // multi flag
	flagVerbosity(scUpdate)
	flagIDList(scUpdate)

	scZero := flag.NewFlagSet("zeroSourceTreeIds", flag.ContinueOnError)                 // subcommand (during development only)
	pSrcZ := scZero.String("src", "", "zero all Id(n) inside source tree dir, required") // flag
	scZero.BoolVar(&id.DryRun, "dry-run", false, "no changes are applied")               // flag

	hCmd := flag.NewFlagSet("help", flag.ContinueOnError) // subcommand
	flagLogfile(hCmd)
	flagVerbosity(hCmd)

	vCmd := flag.NewFlagSet("version", flag.ContinueOnError) // subcommand
	flagLogfile(vCmd)
	flagVerbosity(vCmd)

	scLog := flag.NewFlagSet("log", flag.ExitOnError)                                                                                                                                                          // subcommand
	scLog.StringVar(&Encoding, "encoding", "bare", "trice transmit data format type, options: 'ascii|wrap'")                                                                                                   // flag
	scLog.StringVar(&Encoding, "e", "bare", "short for -encoding")                                                                                                                                             // short flag
	scLog.StringVar(&cipher.Password, "password", "none", "decrypt passphrase")                                                                                                                                // flag
	scLog.StringVar(&cipher.Password, "pw", "none", "short for -password")                                                                                                                                     // short flag
	scLog.BoolVar(&cipher.ShowKey, "key", false, "show encryption key")                                                                                                                                        // flag
	scLog.StringVar(&decoder.TimeStampFormat, "ts", "LOCmicro", "PC timestamp for logs and logfile name, options: 'off|UTCmicro'")                                                                             // flag
	scLog.StringVar(&disp.ColorPalette, "color", "default", "color set, 'off' disables color handling (\"w:x\"->\"w:x\"), 'none' disables channels color (\"w:x\"->\"x\"), options: 'off|none'")               // flag
	scLog.StringVar(&emit.Prefix, "prefix", "source: ", "prepend prefix to all lines, options: any string or 'off|none' or 'source:' followed by 0-12 spaces, source will be replaced")                        // flag
	scLog.StringVar(&emit.Postfix, "postfix", "\n", "append postfix to all lines, options: any string")                                                                                                        // flag
	scLog.StringVar(&global.Source, "source", "JLINK", "receiver device, options: 'COMn|JLINK|STLINK|filename|SIM|RND|HTTP'")                                                                                  //HTTP, RTT, RTTD, RTTF")                                             // flag
	scLog.StringVar(&global.Source, "s", "JLINK", "short for -source")                                                                                                                                         // short flag
	scLog.IntVar(&com.Baud, "baud", 115200, "COM baudrate, valid only for '-source COMn'")                                                                                                                     // flag flag
	scLog.StringVar(&jlink.Param, "jlink", "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0", "passed parameter string, valid only for '-source JLRTT', see JLinkRTTLogger in SEGGER UM08001_JLink.pdf") // JLRTT flag
	//scLog.StringVar(&jlink.Exec, "jlinkExec", jlink.Exec, "JLinkRTTLogger executable with full path")                                                                                                          // JLRTT flag
	scLog.StringVar(&rndMode, "rndMode", "WrapModeWithValidCrc", "valid only for '-source RND', see randomdummy.go, options: 'ChaosMode|BareModeNoSync'")
	scLog.IntVar(&rndLimit, "rndLimit", randomdummy.NoLimit, "valid only for '-source RND', see randomdummy.go, options: 'n|0', 'n' is count of bytes, '0' for unlimited count")
	scLog.BoolVar(&displayserver, "displayserver", false, "send trice lines to displayserver @ ipa:ipp")
	scLog.BoolVar(&displayserver, "ds", false, "short for '-displayserver'")
	scLog.BoolVar(&autostart, "autostart", false, "autostart displayserver @ ipa:ipp (works not good with windows, because of cmd and powershell color issues and missing cli params in wt and gitbash)")
	scLog.BoolVar(&autostart, "a", false, "short for '-autostart'")
	flagLogfile(scLog)
	flagVerbosity(scLog)
	flagIDList(scLog)
	flagIPAddress(scLog)

	scSv := flag.NewFlagSet("displayServer", flag.ExitOnError)                               // subcommand
	scSv.StringVar(&disp.ColorPalette, "color", "default", "color set, options: 'off|none'") // flag
	flagLogfile(scSv)
	flagIPAddress(scSv)

	sCmd := flag.NewFlagSet("scan", flag.ContinueOnError) // subcommand

	scSdSv := flag.NewFlagSet("shutdownServer", flag.ExitOnError) // subcommand
	flagIPAddress(scSdSv)

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
		return scHelp(hCmd, scUpdate, scCheck, scLog, scZero, vCmd, scSv, scSdSv)

	case "s", "sc", "scan":
		sCmd.Parse(subArgs)
		_, err := com.GetSerialPorts()
		return err

	case "v", "ver", "version":
		vCmd.Parse(subArgs)
		return scVersion()

	case "u", "update":
		scUpdate.Parse(subArgs)
		id.FnJSON = lib.ConditinalFilePath(id.FnJSON)
		return id.ScUpdate()

	case "check":
		scCheck.Parse(subArgs)
		id.FnJSON = lib.ConditinalFilePath(id.FnJSON)
		cage.Enable()
		defer cage.Disable()
		return emit.ScCheckList(*pSet)

	case "zeroSourceTreeIds":
		scZero.Parse(subArgs)
		return id.ScZero(*pSrcZ, scZero)

	case "l", "log":
		scLog.Parse(subArgs)
		lib.TimeStampFormat = decoder.TimeStampFormat // todo
		setPrefix(emit.Prefix)
		//if strings.HasPrefix(global.Source, "COM") {
		//com.Port = global.Source // set COM port number
		//global.Source = "COM" // overwrite "COMn"
		//}
		id.FnJSON = lib.ConditinalFilePath(id.FnJSON)

		if false == displayserver {
			cage.Enable()
			defer cage.Disable()
			receiving()
		} else {
			if true == autostart {
				disp.StartServer(args[0])
			}
			err := disp.Connect()
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

// Connect starts a display server sy if sv is not empty, otherwise it assumes a running display server.
//
// It connects then to the running display server.
func connect(sv string) error {
	if "" != sv {
		disp.StartServer(sv)
	}

	err := disp.Connect()
	disp.Out = disp.RemoteOut // re-direct output
	if nil != err {
		return err
	}

	disp.PtrRPC.Call("Server.Out", []string{""}, nil)
	disp.PtrRPC.Call("Server.Out", []string{""}, nil)
	disp.PtrRPC.Call("Server.Out", []string{""}, nil)
	disp.PtrRPC.Call("Server.Out", []string{"att:new connection from ", "read:" + global.Source, "..."}, nil)
	disp.PtrRPC.Call("Server.Out", []string{""}, nil)
	disp.PtrRPC.Call("Server.Out", []string{""}, nil)
	return nil
}

func receiving() {
	switch Encoding {
	case "bare", "raw":
		if "none" != cipher.Password {
			Encoding = "bareXTEACrypted"
		}
	case "wrap", "wrapped":
		if "none" != cipher.Password {
			Encoding = "wrapXTEACrypted"
		}
	case "ascii":
		fallthrough
	default:
		fmt.Println("unknown encoding:", Encoding)
		return
	}

	id.ReadListFile()

	// prepare
	err := cipher.SetUp()
	if nil != err {
		return
	}
	var r io.ReadCloser

	source := global.Source
	if strings.HasPrefix(source, "COM") {
		source = "COM" // overwrite "COMn"
	}

	switch source {
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
		c := com.New(global.Source)
		if false == c.Open() {
			return
		}
		r = c
	case "SIM":
		n := 50 // you will see about n/8 lines
		i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', '\n',
			235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0,
			235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 1, 1}
		s := inputdummy.New(i, time.Millisecond, n)
		emit.DiscardByte = emit.DiscardASCII
		r = ioutil.NopCloser(s) // https://stackoverflow.com/questions/28158990/golang-io-ioutil-nopcloser
	/*case "RTT":
		emit.DiscardByte = emit.DiscardASCII
		s := segger.New()
		if nil != s.Open() {
			return
		}
		r = s
	case "RTTD":
		emit.DiscardByte = emit.DiscardASCII
		d := direct.New()
		if nil != d.Open() {
			return
		}
		r = d*/
	//case "RTTF":
	default: // assume source is a filename
		s := rttfile.New()
		//fn := "c:/repos/trice/rttfile.bin"
		err := s.Open(global.Source)
		if nil != err {
			fmt.Println(err)
			return
		}
		r = s
	}
	t := receiver.New(r)

	t.Start()
}

//  func scScan() error {
//  	com.Port = "COMscan"
//  	_, err := com.GetSerialPorts()
//  	return err
//  }

func scVersion() error {
	cage.Enable()
	defer cage.Disable()
	if global.Verbose {
		fmt.Println("https://github.com/rokath/trice")
	}
	if "" != version {
		fmt.Printf("version=%v, commit=%v, built at %v\n", version, commit, date)
	} else {
		fmt.Printf("version=devel, built %s\n", linkTime)
	}
	return nil
}

func setPrefix(s string) {
	switch s {
	case "source:":
		emit.Prefix = global.Source + ":"
	case "source: ":
		emit.Prefix = global.Source + ": "
	case "source:  ":
		emit.Prefix = global.Source + ":  "
	case "source:   ":
		emit.Prefix = global.Source + ":   "
	case "source:    ":
		emit.Prefix = global.Source + ":    "
	case "source:     ":
		emit.Prefix = global.Source + ":     "
	case "source:      ":
		emit.Prefix = global.Source + ":      "
	case "source:       ":
		emit.Prefix = global.Source + ":       "
	case "source:        ":
		emit.Prefix = global.Source + ":        "
	case "source:         ":
		emit.Prefix = global.Source + ":         "
	case "source:          ":
		emit.Prefix = global.Source + ":          "
	case "source:           ":
		emit.Prefix = global.Source + ":           "
	case "source:            ":
		emit.Prefix = global.Source + ":            "
	case "off", "none":
		emit.Prefix = ""
	default:
		emit.Prefix = s
	}
}
