// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implemets the commandline interface and calls the appropriate commands
package args

import (
	"bytes"
	"errors"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
	"os/signal"
	"path/filepath"
	"runtime"
	"strings"
	"syscall"
	"time"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/keybcmd"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/translator"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/cipher"
)

var (
	// Version is set in main package
	Version string

	// Commit is set in main package
	Commit string

	// Date is set in main package
	Date string

	// Verbose gives mor information on output if set. This variable is copied into the appropriate packages.
	Verbose bool

	// Source is the trice receiver to use
	Port string

	// Encoding describes the way the byte stream is coded. TODO: Change to MultiArgs
	Encoding string

	// rndMode is flag value for random mode
	//rndMode string

	// rndLimit is flag vaue for byte count limit in random mode
	//rndLimit int

	// displayRemote if set, sends trice lines over TCP
	displayRemote bool

	// autostart if set, starts an additional trice instamce as displayserver
	autostart bool

	// fsScCheck is flag set for sub command check for evaluating ID list
	// fsScCheck *flag.FlagSet

	// pSet is a string pointer to the data set used for scCheck
	pSet *string

	// fsScUpdate is flag set for sub command update for updating ID list
	fsScUpdate *flag.FlagSet

	// fsScZero is flag set for sub command zero for clearing IDs in source tree
	fsScZero *flag.FlagSet

	// pSrcZ is a string pointer to the saftets string for scZero
	pSrcZ *string

	// fsScHelp is flag set for sub command 'help'
	fsScHelp *flag.FlagSet

	// fsScLog is flag set for sub command 'log'
	fsScLog *flag.FlagSet

	// fsScSv is flag set for sub command 'displayServer'
	fsScSv *flag.FlagSet

	// fsScVerseion is flag set for sub command 'version'
	fsScVersion *flag.FlagSet

	// fsScScan is flag set for sub command 'scan'
	fsScScan *flag.FlagSet

	// fsScSdSv is flag set for sub command 'shutdownServer'
	fsScSdSv *flag.FlagSet

	fnJSON string
)

func flagLogfile(p *flag.FlagSet) {
	p.StringVar(&cage.Name, "logfile", "off", "append all output to logfile, options: 'none|filename|auto', 'auto' for \"2006-01-02_1504-05_trice.log\" with actual time") // flag
	p.StringVar(&cage.Name, "lg", "off", "short for -logfile")                                                                                                             // short flag
}

func flagVerbosity(p *flag.FlagSet) {
	p.BoolVar(&Verbose, "verbose", false, "verbose, more informal output if used") // flag
	p.BoolVar(&Verbose, "v", false, "short for verbose")                           // flag
}

func flagIDList(p *flag.FlagSet) {
	p.StringVar(&fnJSON, "idlist", "til.json", "trice ID list path, 'none' possible") // flag
	p.StringVar(&fnJSON, "i", "til.json", "short for '-idlist'")                      // flag
}

func flagIPAddress(p *flag.FlagSet) {
	p.StringVar(&emitter.IPAddr, "ipa", "localhost", "ip address like '127.0.0.1'") // flag
	p.StringVar(&emitter.IPPort, "ipp", "61497", "16 bit port number")              // flag
}

// scHelp is subcommand help
func scHelp(
	//c *flag.FlagSet,
	l *flag.FlagSet,
	z *flag.FlagSet,
	v *flag.FlagSet,
	sv *flag.FlagSet,
	scSdSv *flag.FlagSet,
) error {
	if Verbose {
		fmt.Printf("\n*** https://github.com/rokath/trice ***\n\n")
		fmt.Printf("If a non-multi parameter is used more than one times the last value wins.\n")
	}
	cage.Enable()
	defer cage.Disable()

	fmt.Println("syntax: 'trice subcommand' [params]")
	fmt.Println("subcommand 'help', 'h' for command line usage")
	fsScHelp.SetOutput(os.Stdout)
	fsScHelp.PrintDefaults()
	fmt.Println("subcommand 'u', 'upd', 'update' for updating ID list and source files")
	fsScUpdate.SetOutput(os.Stdout)
	fsScUpdate.PrintDefaults()
	//fmt.Fprintln(c.Output(), "subcommand 'check' for dispaying the ID list in trice log format")
	//c.PrintDefaults()
	fmt.Println("subcommand 'l', 'log' for displaying trice logs coming from source")
	l.SetOutput(os.Stdout)
	l.PrintDefaults()
	fmt.Println("subcommand 'zeroSourceTreeIds' for setting all TRICE IDs to 0 in source tree, avoid using this subcommand normally")
	z.SetOutput(os.Stdout)
	z.PrintDefaults()
	fmt.Println("subcommand 'v', 'ver', 'version' for displaying version information")
	v.SetOutput(os.Stdout)
	v.PrintDefaults()
	fmt.Println("subcommand 'ds', 'displayServer' starts a display server, use in a separate console, on Windows use wt or a linux shell like git-bash to avoid color issues, several instances of 'trice l -ds' will send output there")
	sv.SetOutput(os.Stdout)
	sv.PrintDefaults()
	fmt.Println("subcommand 'sd', 'shutdownServer' ends display server at IPA:IPP, works also on a remote mashine")
	scSdSv.SetOutput(os.Stdout)
	scSdSv.PrintDefaults()
	fmt.Println("examples:")
	fmt.Println("    'trice update -src ../A -src ../../B' parses ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json")
	fmt.Println("    'trice l -s COM15 -baud 38400 -d wrap display wrap data format trice log messages from COM15")
	fmt.Println("    'trice l display bare data format trice log messages from default source")
	fmt.Println("    'trice zeroSourceTreeIds -dir ../A' sets all TRICE IDs to 0 in ./A")
	fmt.Println("    'trice v -v' shows verbose version information")
	return nil
}

// func init() {
// 	fsScCheck = flag.NewFlagSet("check", flag.ExitOnError)                                           // subcommand
// 	pSet = fsScCheck.String("dataset", "position", "parameters, option: 'negative'")                 // flag
// 	fsScCheck.StringVar(&emitter.ColorPalette, "color", "default", "color set, options: 'off|none'") // flag
// 	flagLogfile(fsScCheck)
// 	flagVerbosity(fsScCheck)
// 	flagIDList(fsScCheck)
// }

func init() {
	fsScUpdate = flag.NewFlagSet("update", flag.ExitOnError)                                // subcommand
	fsScUpdate.BoolVar(&id.DryRun, "dry-run", false, "no changes are applied")              // flag
	fsScUpdate.Var(&id.Srcs, "src", "source dir or file, multi use possible, default './'") // multi flag
	flagVerbosity(fsScUpdate)
	flagIDList(fsScUpdate)
}

func init() {
	fsScZero = flag.NewFlagSet("zeroSourceTreeIds", flag.ContinueOnError)                 // subcommand (during development only)
	pSrcZ = fsScZero.String("src", "", "zero all Id(n) inside source tree dir, required") // flag
	fsScZero.BoolVar(&id.DryRun, "dry-run", false, "no changes are applied")              // flag
}

func init() {
	fsScHelp = flag.NewFlagSet("help", flag.ContinueOnError) // subcommand
	flagLogfile(fsScHelp)
	flagVerbosity(fsScHelp)
}

func init() {
	fsScVersion = flag.NewFlagSet("version", flag.ContinueOnError) // subcommand
	flagLogfile(fsScVersion)
	flagVerbosity(fsScVersion)
}

func init() {
	fsScLog = flag.NewFlagSet("log", flag.ExitOnError)                                                                                                                                                          // subcommand
	fsScLog.StringVar(&Encoding, "encoding", "default", "trice transmit data format type, options: 'ascii|bare|esc|wrap'. For -p JLINK default is bare, for -p COMn default is bare")                           // flag
	fsScLog.StringVar(&Encoding, "e", "default", "short for -encoding")                                                                                                                                         // short flag
	fsScLog.StringVar(&cipher.Password, "password", "none", "decrypt passphrase")                                                                                                                               // flag
	fsScLog.StringVar(&cipher.Password, "pw", "none", "short for -password")                                                                                                                                    // short flag
	fsScLog.BoolVar(&cipher.ShowKey, "key", false, "show encryption key")                                                                                                                                       // flag
	fsScLog.StringVar(&emitter.TimeStampFormat, "ts", "LOCmicro", "PC timestamp for logs and logfile name, options: 'off|none|UTCmicro|zero'")                                                                  // flag
	fsScLog.StringVar(&emitter.ColorPalette, "color", "default", "color set, 'off' disables color handling (\"w:x\"->\"w:x\"), 'none' disables channels color (\"w:x\"->\"x\"), options: 'off|none'")           // flag
	fsScLog.StringVar(&emitter.Prefix, "prefix", "source: ", "line prefix, options: any string or 'off|none' or 'source:' followed by 0-12 spaces, 'source:' will be replaced by source value e.g., 'COM17:'")  // flag
	fsScLog.StringVar(&emitter.Suffix, "suffix", "", "append suffix to all lines, options: any string")                                                                                                         // flag
	fsScLog.StringVar(&Port, "port", "JLINK", "receiver device, options: 'COMn|JLINK|STLINK|filename|SIM|RND|HTTP'")                                                                                            // flag
	fsScLog.StringVar(&Port, "p", "JLINK", "short for -port")                                                                                                                                                   // short flag
	fsScLog.IntVar(&com.Baud, "baud", 115200, "COM baudrate, valid only for '-source COMn'")                                                                                                                    // flag flag
	fsScLog.StringVar(&link.Args, "args", "-Device STM32F070RB -if SWD -Speed 4000 -RTTChannel 0", "passed parameter string, valid only for '-p JLINK|STLINK', see JLinkRTTLogger in SEGGER UM08001_JLink.pdf") // JLRTT flag
	//fsScLog.StringVar(&rndMode, "rndMode", "WrapModeWithValidCrc", "valid only for '-source RND', see randomdummy.go, options: 'ChaosMode|BareModeNoSync'")
	//fsScLog.IntVar(&rndLimit, "rndLimit", randomdummy.NoLimit, "valid only for '-source RND', see randomdummy.go, options: 'n|0', 'n' is count of bytes, '0' for unlimited count")
	fsScLog.BoolVar(&displayRemote, "displayserver", false, "send trice lines to displayserver @ ipa:ipp")
	fsScLog.BoolVar(&displayRemote, "ds", false, "short for '-displayserver'")
	fsScLog.BoolVar(&autostart, "autostart", false, "autostart displayserver @ ipa:ipp (works not good with windows, because of cmd and powershell color issues and missing cli params in wt and gitbash)")
	fsScLog.BoolVar(&autostart, "a", false, "short for '-autostart'")
	flagLogfile(fsScLog)
	flagVerbosity(fsScLog)
	flagIDList(fsScLog)
	flagIPAddress(fsScLog)
}

func init() {
	fsScSv = flag.NewFlagSet("displayServer", flag.ExitOnError)                                   // subcommand
	fsScSv.StringVar(&emitter.ColorPalette, "color", "default", "color set, options: 'off|none'") // flag
	flagLogfile(fsScSv)
	flagIPAddress(fsScSv)
}

func init() {
	fsScScan = flag.NewFlagSet("scan", flag.ContinueOnError) // subcommand
}

func init() {
	fsScSdSv = flag.NewFlagSet("shutdownServer", flag.ExitOnError) // subcommand
	flagIPAddress(fsScSdSv)
}

// injectValues is distibuting values used in several packages.
// It must not be called before the appropriate arg parsing.
func injectValues() {
	id.Verbose = Verbose
	emitter.Verbose = Verbose
	link.Verbose = Verbose
	cage.Verbose = Verbose
	receiver.Verbose = Verbose
	translator.Verbose = Verbose
	link.Port = Port
}

// Handler evaluates the arguments slice of strings
func Handler(args []string) error {

	// buildTime is a helper for displaying version as time during development.
	buildTime := "unknown"
	fi, err := os.Stat(os.Args[0])
	if nil == err { // on running main tests fileinfo is invalid
		buildTime = fi.ModTime().String()
	}

	cage.DefaultLogfileName = "2006-01-02_1504-05_trice.log"

	// Verify that a subcommand has been provided: os.Arg[0] is the main command, os.Arg[1] will be the subcommand.
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

	case "h", "help":
		fsScHelp.Parse(subArgs)
		injectValues()
		//return scHelp(fsScCheck, fsScLog, fsScZero, fsScVerseion, fsScSv, fsScSdSv)
		return scHelp(fsScLog, fsScZero, fsScVersion, fsScSv, fsScSdSv)

	case "s", "sc", "scan":
		fsScScan.Parse(subArgs)
		injectValues()
		_, err := com.GetSerialPorts()
		return err

	case "v", "ver", "version":
		fsScVersion.Parse(subArgs)
		injectValues()
		return scVersion(buildTime)

	case "u", "update":
		fsScUpdate.Parse(subArgs)
		injectValues()
		fnJSON = id.ConditionalFilePath(fnJSON)
		return id.ScUpdate(fnJSON)

	//case "check":
	//	fsScCheck.Parse(subArgs)
	//	id.FnJSON = id.ConditionalFilePath(id.FnJSON)
	//	cage.Enable()
	//	defer cage.Disable()
	//	return emitter.ScCheckList(*pSet)

	case "zeroSourceTreeIds":
		fsScZero.Parse(subArgs)
		injectValues()
		return id.ScZero(*pSrcZ, fsScZero)

	case "l", "log":
		fsScLog.Parse(subArgs)
		injectValues()
		return receiving()

	case "ds", "displayServer":
		fsScSv.Parse(subArgs)
		injectValues()
		return emitter.ScDisplayServer()

	case "sd", "shutdownRemoteDisplayServer":
		fsScSdSv.Parse(subArgs)
		injectValues()
		return emitter.ScShutdownRemoteDisplayServer(1)

	default:
		fmt.Println("try: 'trice help|h'")
		return nil
	}
}

// errorFatal ends in osExit(1) if err not nil.
func errorFatal(err error) {
	if nil == err {
		return
	}
	if Verbose {
		_, file, line, _ := runtime.Caller(1)
		log.Fatal(err, " "+filepath.Base(file)+" ", line)
	}
	log.Fatal(err)
}

// newReadCloser uses variable Port and tries to return a valid io.ReadCloser.
func newReadCloser() (r io.ReadCloser, e error) {
	if strings.HasPrefix(Port, "COM") {
		var c com.COMport // interface type
		//c = com.NewCOMPortGoBugSt(Port)
		c = com.NewCOMPortTarm(Port)
		if false == c.Open() {
			e = fmt.Errorf("Can not open %s", Port)
		}
		r = c
		return
	}
	switch Port {
	case "JLINK", "STLINK":
		l := link.NewReadCloser(link.Args) // yes
		if nil != l.Open() {
			e = fmt.Errorf("Can not open LINK %s", link.Args)
		}
		r = l
	case "DUMMY":
		rd := bytes.NewReader([]byte{2, 1, 1, 1, 0x89, 0xab, 0xcd, 0xef, 2, 2, 2, 0, 3, 3, 3, 3, 4, 4})
		r = ioutil.NopCloser(rd)
	default:
		e = fmt.Errorf("Unknown input port %s", Port)
	}
	return
}

// receiving uses internally these settings:
// Port
// Encoding
func receiving() error {
	translatePrefix()
	fnJSON = id.ConditionalFilePath(fnJSON)
	if true == displayRemote {
		var p *emitter.RemoteDisplay
		if true == autostart {
			p = emitter.NewRemoteDisplay(os.Args[0], "-logfile "+cage.Name)
		} else {
			p = emitter.NewRemoteDisplay()
		}
		p.ErrorFatal()
		keybcmd.ReadInput()
	} else {
		cage.Enable()
		defer cage.Disable()
	}
	portReader, e := newReadCloser()
	errorFatal(e)

	var p translator.Translator // interface type

	if "default" == Encoding {
		switch Port {
		case "JLINK", "STLINK":
			Encoding = "bare"
		default:
			if strings.HasPrefix(Port, "COM") {
				Encoding = "bare"
			}
		}
	}
	switch Encoding {
	//	case "sim":
	//		p = simNewSimpleTriceInterpreterWithAnsi(r)
	case "bare":
		p = receiveBareSimpleTricesAndDisplayAnsiColor(portReader, fnJSON)
	case "esc":
		p = receiveEscTricesAndDisplayAnsiColor(portReader, fnJSON)
	case "wrap", "wrapped":
		p = receiveWrapSimpleTricesAndDisplayAnsiColor(portReader, fnJSON)

	case "bareXTEACrypted", "wrapXTEACrypted":
		errorFatal(cipher.SetUp())
		fallthrough
	case "ascii":
		fallthrough
	default:
		return fmt.Errorf("unknown encoding: %s", Encoding)
	}

	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)

	for nil == p.SavedError() { // endless loop
		select {
		case sig := <-sigs:
			fmt.Println("####################################   END OF LIFE!   ####################################")
			fmt.Println(sig)
			goto end
		case <-time.After(10 * time.Millisecond):
		}
	}
end:
	errorFatal(portReader.Close())
	return p.SavedError()
}

func receiveWrapSimpleTricesAndDisplayAnsiColor(rd io.Reader, fnJSON string) *translator.BareTranslator {
	// triceAtomsReceiver uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	triceAtomsReceiver := receiver.NewTricesfromBare(receiver.NewBareReaderFromWrap(rd))

	// NewColorDisplay creates a ColorlDisplay. It provides a Linewriter.
	// It uses internally a local display combined with a line transformer.
	lwD := emitter.NewColorDisplay(emitter.ColorPalette)

	// lineComposer implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	sw := emitter.NewLineComposer(lwD, emitter.TimeStampFormat, emitter.Prefix, emitter.Suffix)

	list := id.NewList(fnJSON)
	list.ReadListFile()
	go list.FileWatcher()

	// uses triceAtomsReceiver for reception and the io.StringWriter interface sw for writing.
	// collects trice atoms to a complete trice, generates the appropriate string using list and writes it to the provided io.StringWriter
	return translator.NewSimpleTrices(sw, list, triceAtomsReceiver)
}

func receiveBareSimpleTricesAndDisplayAnsiColor(rd io.Reader, fnJSON string) *translator.BareTranslator {
	// triceAtomsReceiver uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	triceAtomsReceiver := receiver.NewTricesfromBare(rd)

	// NewColorDisplay creates a ColorlDisplay. It provides a Linewriter.
	// It uses internally a local display combined with a line transformer.
	lwD := emitter.NewColorDisplay(emitter.ColorPalette)

	// lineComposer implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	sw := emitter.NewLineComposer(lwD, emitter.TimeStampFormat, emitter.Prefix, emitter.Suffix)

	list := id.NewList(fnJSON)
	list.ReadListFile()
	go list.FileWatcher()

	// uses triceAtomsReceiver for reception and the io.StringWriter interface sw for writing.
	// collects trice atoms to a complete trice, generates the appropriate string using list and writes it to the provided io.StringWriter
	return translator.NewSimpleTrices(sw, list, triceAtomsReceiver)
}

func receiveEscTricesAndDisplayAnsiColor(rd io.Reader, fnJSON string) *translator.EscTranslator {
	// NewColorDisplay creates a ColorlDisplay. It provides a Linewriter.
	// It uses internally a local display combined with a line transformer.
	lwD := emitter.NewColorDisplay(emitter.ColorPalette)

	// lineComposer implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	sw := emitter.NewLineComposer(lwD, emitter.TimeStampFormat, emitter.Prefix, emitter.Suffix)

	list := id.NewList(fnJSON)
	list.ReadListFile()
	go list.FileWatcher()

	// uses rd for reception and the io.StringWriter interface sw for writing.
	// collects trice bytes to a complete esc trice message, generates the appropriate string using list and writes it to the provided io.StringWriter
	return translator.NewEscTrices(sw, list, rd)
}

//  // There is a small chance this test fails because of unexpected ordering of 'ignoring bytes' message.
//  // This is no error.
//  func simNewSimpleTriceInterpreterWithAnsi(rd io.Reader) *translator.TriceTranslator {
//
//  	// tai uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
//  	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
//  	triceAtomsReceiver := receiver.NewTricesfromBare(rd)
//
//  	// NewColorDisplay creates a ColorlDisplay. It provides a Linewriter.
//  	// It uses internally a local display combined with a line transformer.
//  	lwD := emitter.NewColorDisplay(emitter.ColorPalette)
//
//  	// lineComposer r implements the io.StringWriter interface and uses the Linewriter provided.
//  	// The line composer scans the trice strings and composes lines out of them according to its properies.
//  	sw := emitter.NewLineComposer(lwD, emitter.TimeStampFormat, emitter.Prefix, emitter.Suffix)
//
//  	// sti uses triceAtomsReceiver for reception and the io.StringWriter interface (r) for writing.
//  	// sti collects trice atoms to a complete trice, generates the appropriate string with list and writes it to the provided io.StringWriter
//  	return translator.NewSimpleTrices(sw, list,  triceAtomsReceiver)
//  }
//
//  // There is a small chance this test fails because of unexpected ordering of 'ignoring bytes' message.
//  // This is no error.
//  func simNewSimpleTriceInterpreterWithAnsi0(rd io.Reader) *translator.TriceTranslator {
//
//  	// tai uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
//  	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
//  	triceAtomsReceiver := receiver.NewTricesfromBare(rd)
//
//  	// NewColorDisplay creates a ColorlDisplay. It provides a Linewriter.
//  	// It uses internally a local display combined with a line transformer.
//  	lwD := emitter.NewColorDisplay(emitter.ColorPalette)
//
//  	// lineComposer r implements the io.StringWriter interface and uses the Linewriter provided.
//  	// The line composer scans the trice strings and composes lines out of them according to its properies.
//  	sw := emitter.NewLineComposer(lwD, emitter.TimeStampFormat, emitter.Prefix, emitter.Suffix)
//
//  	id.List = []id.Item{
//  		{ID: 257, FmtType: "TRICE8_2", FmtStrg: "att:Hello, %d+%d=", Created: 0, Removed: 0},
//  		{ID: 514, FmtType: "TRICE16_1", FmtStrg: "att:%d, ok?\n", Created: 0, Removed: 0},
//  		{ID: 771, FmtType: "TRICE0", FmtStrg: "msg:Yes!\n", Created: 0, Removed: 0},
//  		{ID: 5654, FmtType: "TRICE0", FmtStrg: "%s", Created: 0, Removed: 0},
//  	}
//
//  	// sti uses triceAtomsReceiver for reception and the io.StringWriter interface (r) for writing.
//  	// sti collects trice atoms to a complete trice, generates the appropriate string with list and writes it to the provided io.StringWriter
//  	return translator.NewSimpleTrices(sw, triceAtomsReceiver)
//  }

//switch Encoding {
//case "bare":
//	// rd implements the io.Reader interface needed by TriceReceiver.
//	// It is the input source.
//	rd := bytes.NewReader([]byte{1, 1, 1, 1, 0x16, 0x16, 0x16, 0x16, 2, 2, 2, 0, 3, 2, 0, 0, 3, 3, 3, 3, 4, 4})
//	receiveBareSimpleTricesAndDisplayAnsiColor(rd)
//	return nil
//default:
//	return errors.New("unexpected Encoding")
//}

// // connect starts a display server sv if sv is not empty, otherwise it assumes a running display server.
// //
// // It connects then to the running display server.
// func connect(sv string) error {
// 	//if "" != sv {
// 	//	disp.StartServer(sv)
// 	//}
// 	//
// 	//err := disp.Connect()
// 	//disp.WriteLine = disp.RemoteOut // re-direct output
// 	//if nil != err {
// 	//	return err
// 	//}
// 	//
// 	//disp.PtrRPC.Call("Server.Out", []string{""}, nil)
// 	//disp.PtrRPC.Call("Server.Out", []string{""}, nil)
// 	//disp.PtrRPC.Call("Server.Out", []string{""}, nil)
// 	//disp.PtrRPC.Call("Server.Out", []string{"att:new connection from ", "read:" + Source, "..."}, nil)
// 	//disp.PtrRPC.Call("Server.Out", []string{""}, nil)
// 	//disp.PtrRPC.Call("Server.Out", []string{""}, nil)
// 	return nil
// }

/*
// receiving TODO better design
func receiving() {


	source := Source
	if strings.HasPrefix(source, "COM") {
		source = "COM" // overwrite "COMn"
	}

	switch source {

	//case "HTTP":
	//	h := http.New()
	//	if false == h.Open() {
	//		return
	//	}
	//	r = h
	case "RND":
		// rndLimit = 19: you will see n-7 discarded bytes because 7 bytes are held internally to try to sync a wrap
		innerReader := randomdummy.New(randomdummy.ZeroRandomSeed, rndMode, randomdummy.DefaultDelay, rndLimit)
		r = ioutil.NopCloser(innerReader) // https://stackoverflow.com/questions/28158990/golang-io-ioutil-nopcloser
	case "COM":
		c := com.New(Source)
		if false == c.Open() {
			return
		}
		r = c
		// case "SIM":
		// 	n := 50 // you will see about n/8 lines
		// 	i := []byte{'g', 'a', 'r', 'b', 'a', 'g', 'e', '\n',
		// 		235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 0, 0,
		// 		235, 96, 96, 235 ^ 10 ^ 172, 10, 172, 1, 1}
		// 	s := inputdummy.New(i, time.Millisecond, n)
		// 	emit.DiscardByte = emit.DiscardASCII
		// 	r = ioutil.NopCloser(s) // https://stackoverflow.com/questions/28158990/golang-io-ioutil-nopcloser
		case "RTT":
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
			r = d
		//case "RTTF":
		// default: // assume source is a filename
		// 	s := rttfile.New()
		// 	//fn := "c:/repos/trice/rttfile.bin"
		// 	err := s.Open(Source)
		// 	if nil != err {
		// 		fmt.Println(err)
		// 		return
		// 	}
		// 	r = s
	}
	//t := receiver.New(r)

	//t.Start()
}
*/
//  func scScan() error {
//  	com.Port = "COMscan"
//  	_, err := com.GetSerialPorts()
//  	return err
//  }

// scVersion is subcommand 'version'
func scVersion(buildTime string) error {
	cage.Enable()
	defer cage.Disable()
	if Verbose {
		fmt.Println("https://github.com/rokath/trice")
	}
	if "" != Version {
		fmt.Printf("version=%v, commit=%v, built at %v\n", Version, Commit, Date)
	} else {
		fmt.Printf("version=devel, built %s\n", buildTime)
	}
	return nil
}

// translatePrefix changes "source:" to e.g., "JLINK:".
// todo: use strings.Split()
func translatePrefix() {
	switch emitter.Prefix {
	case "source:":
		emitter.Prefix = Port + ":"
	case "source: ":
		emitter.Prefix = Port + ": "
	case "source:  ":
		emitter.Prefix = Port + ":  "
	case "source:   ":
		emitter.Prefix = Port + ":   "
	case "source:    ":
		emitter.Prefix = Port + ":    "
	case "source:     ":
		emitter.Prefix = Port + ":     "
	case "source:      ":
		emitter.Prefix = Port + ":      "
	case "source:       ":
		emitter.Prefix = Port + ":       "
	case "source:        ":
		emitter.Prefix = Port + ":        "
	case "source:         ":
		emitter.Prefix = Port + ":         "
	case "source:          ":
		emitter.Prefix = Port + ":          "
	case "source:           ":
		emitter.Prefix = Port + ":           "
	case "source:            ":
		emitter.Prefix = Port + ":            "
	case "off", "none":
		emitter.Prefix = ""
	}
}
