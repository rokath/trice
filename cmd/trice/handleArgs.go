// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"bufio"
	"crypto/sha1"
	"errors"
	"flag"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"runtime"
	"strings"
	"sync"

	"github.com/fatih/color"
	"github.com/rokath/trice/pkg/disp"
	"github.com/rokath/trice/pkg/emit"
	"github.com/rokath/trice/pkg/id"
	"github.com/rokath/trice/pkg/lgf"
	"github.com/rokath/trice/pkg/receiver"
	"golang.org/x/crypto/xtea"
)

type arrayFlag []string // slice type for multi flag

func (i *arrayFlag) String() string {
	return "my string representation"
}

func (i *arrayFlag) Set(value string) error {
	*i = append(*i, value)
	return nil
}

// local config values
var (
	srcs         arrayFlag // gets multiple files or directories
	port         string
	baud         int
	fnJSON       string
	pList        *id.List
	password     string
	showPassword bool
)

// HandleArgs evaluates the arguments slice of strings und uses wd as working directory
func HandleArgs(wd string, args []string) error {
	list := make(id.List, 0, 65536) // for 16 bit IDs enough
	pList = &list

	scUpd := flag.NewFlagSet("update", flag.ExitOnError)                               // subcommand
	pDryR := scUpd.Bool("dry-run", false, "no changes are applied")                    // flag
	pLU := scUpd.String("list", "til.json", "trice ID list path, \"none\" possible")   // flag
	pVerb := scUpd.Bool("v", false, "verbose")                                         // flag
	scUpd.Var(&srcs, "src", "source dir or file, multi use possible (default \"./\")") // multi flag

	scChk := flag.NewFlagSet("check", flag.ExitOnError)                           // subcommand
	pSet := scChk.String("dataset", "position", "parameters, option: negative")   // flag
	pC := scChk.String("list", "til.json", "trice ID list path")                  // flag
	pPal := scChk.String("color", "default", "color set, options: off|alternate") // flag

	scZero := flag.NewFlagSet("zeroSourceTreeIds", flag.ContinueOnError)                  // subcommand (during development only)
	pSrcZ := scZero.String("src", "", "zero all Id(n) inside source tree dir (required)") // flag
	pRunZ := scZero.Bool("dry-run", false, "no changes are applied")                      // flag

	hCmd := flag.NewFlagSet("help", flag.ContinueOnError) // subcommand
	//pHlf := hCmd.String("lf", "trice.log", "append output to logfile, set to \"off\" to avoid this") // flag

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
		return scHelp( /**pHlf,*/ hCmd, scUpd, scChk, scLog, scZero, vCmd, scSv, scCl)
	case "v", "ver", "version":
		vCmd.Parse(subArgs)
		return scVersion(*pVlf)
	case "u", "update":
		scUpd.Parse(subArgs)
		return scUpdate(*pDryR, *pLU, *pVerb)
	case "check":
		scChk.Parse(subArgs)
		return scCheckList(*pC, *pSet, *pPal)
	case "l", "log":
		scLog.Parse(subArgs)
		return scLogging(*pLpre, *pLpost, *pPort, *pBaud, *pJSON, *pTs, *pCol, *pKey, *pShow, *pLlf)
	case "zeroSourceTreeIds":
		scZero.Parse(subArgs)
		return zeroIds(*pRunZ, *pSrcZ, scZero)
	case "ds", "displayServer":
		scSv.Parse(subArgs)
		return disp.ScServer(*pSvCol, *pSvIPA, *pSvIPP, *pSvLlf)
	case "r", "rec", "receiver":
		scCl.Parse(subArgs)
		return scReceiver(*pRpre, *pRpost, *pClIPA, *pClIPP, *pClPort, *pClBaud, *pClJSON, *pClTs, *pClKey, *pClShow, *pClSrv)
	default:
		fmt.Println("try: 'trice help|h'")
		return nil
	}
}

func assign(fn string) string {
	if "none" == fn {
		return fn
	}
	s, err := filepath.Abs(fn)
	if nil != err {
		_ = fmt.Errorf("failed to parse %s: %v", fn, err)
	}
	return s
}

func scVersion(lfn string) error {
	lgf.Name = lfn
	lgf.Enable()
	defer lgf.Disable()

	if "" != version {
		color.New().Printf("color.version=%v, commit=%v, built at %v\n", version, commit, date)
	} else {
		s := "version=devel, commit=unknown, built after 2020-03-29-1437"
		color.New().Printf("color %s\n", s)
		log.Printf("log %s\n", s) // starts with tab in terminal
		log.Printf("log %s\n", s) // this is ok
		fmt.Printf("fmt %s\n", s) // goes not into logfile!
	}
	return nil
}

// scHelp is subcommand help
func scHelp( /*lfn string,*/
	hCmd *flag.FlagSet,
	u *flag.FlagSet,
	c *flag.FlagSet,
	l *flag.FlagSet,
	z *flag.FlagSet,
	v *flag.FlagSet,
	sv *flag.FlagSet,
	cl *flag.FlagSet) error {

	// flag.go uses fmt.Print, so first solve of.File = io.Writer riddle
	// lgf.Name = lfn
	// lgf.Enable()
	// defer lgf.Disable()

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

// scUpdate is subcommand update
func scUpdate(dry bool, fn string, v bool) error {
	fnJSON = assign(fn)
	if 0 == len(srcs) {
		srcs = append(srcs, "./") // default value
	}
	for i := range srcs {
		s := srcs[i]
		srcU := assign(s)
		if _, err := os.Stat(srcU); err == nil { // path exists
			err = update(dry, srcU, fnJSON, v)
			if nil != err {
				return err
			}
		} else if os.IsNotExist(err) { // path does *not* exist
			fmt.Println(s, " -> ", srcU, "does not exist!")
		} else {
			fmt.Println(s, "Schrodinger: file may or may not exist. See err for details.")
			// Therefore, do *NOT* use !os.IsNotExist(err) to test for file existence
			// https://stackoverflow.com/questions/12518876/how-to-check-if-a-file-exists-in-go
		}
	}
	return nil
}

// update does parse source tree, update IDs and is list
func update(dryRun bool, dir, fn string, verbose bool) error {
	err := pList.Update(dir, fn, !dryRun, verbose)
	if nil != err {
		return fmt.Errorf("failed update on %s with %s: %v", dir, fn, err)
	}
	fmt.Println(len(*pList), "ID's in list", fn)
	return nil
}

// scCheckList does log the id list with a dataset
func scCheckList(fn, dataset, palette string) error {
	emit.TimeStampFormat = "off"
	disp.ColorPalette = palette
	fnJSON = assign(fn)
	err := pList.Read(fnJSON)
	if nil != err {
		fmt.Println("ID list " + fnJSON + " not found, exit")
		return nil
	}
	emit.Check(*pList, dataset)
	return nil
}

// createCipher prepares decryption, with password "none" the encryption flag is set false, otherwise true
func createCipher() (*xtea.Cipher, bool, error) {
	h := sha1.New() // https://gobyexample.com/sha1-hashes
	h.Write([]byte(password))
	key := h.Sum(nil)
	key = key[:16] // only first 16 bytes needed as key

	c, err := xtea.NewCipher(key)
	if err != nil {
		return nil, false, errors.New("NewCipher returned error")
	}
	var e bool
	if "none" != password {
		e = true
		if true == showPassword {
			fmt.Printf("% 20x is XTEA encryption key\n", key)
		}
	} else if true == showPassword {
		fmt.Printf("no encryption\n")
	}
	return c, e, nil
}

func setPrefix(s string) {
	switch s {
	case "off":
		emit.Prefix = ""
	case "COMport:":
		emit.Prefix = port + "  "
	default:
		emit.Prefix = s
	}
}

// scLogging is the subcommand log and connects to COM port and displays traces
func scLogging(pre, post, prt string, bd int, fn, ts, col, pw string, show bool, lfn string) error {
	lgf.Name = lfn
	lgf.Enable()
	defer lgf.Disable()

	port = prt
	baud = bd
	setPrefix(pre)
	emit.Postfix = post
	fnJSON = assign(fn)
	emit.TimeStampFormat = ts
	disp.ColorPalette = col
	password = pw
	showPassword = show
	return logTrices()
}

// logTrices connects to COM port and displays traces
func logTrices() error {
	if "none" != fnJSON {
		// setup ip list
		err := pList.Read(fnJSON)
		if nil != err {
			fmt.Println("ID list " + fnJSON + " not found, exit")
			return nil
		}
	}

	var err error
	receiver.Cipher, receiver.Crypto, err = createCipher()
	if nil != err {
		return err
	}

	/* TODO: Introduce new command line option for choosing between
	   1) Serial receiver(port name, baudrate, parity bit etc. )
	   2) TCP receiver (IP, port, Protocol (i.e JSON,XML))
	   3) HTTP/Websocket receiver (may be the simplest form in Golang)
	*/

	fmt.Println("id list file", fnJSON, "with", len(*pList), "items")
	return doSerialReceive()
}

// conditionalComPortScan scans for COM ports if -port was specified as COMscan, it tries to use first found COM port.
func conditionalComPortScan() error {
	if "COMscan" != port {
		return nil
	}
	log.Println("Scan for serial ports...")
	ports, err := receiver.GetSerialPorts()
	if err != nil {
		return err
	}
	if len(ports) > 0 {
		log.Println("Take serial port", ports[0])
		port = ports[0]
		return nil
	}
	return errors.New("Could not find serial port on system")
}

// keyboardInput expects user input from terminal
func keyboardInput() { // https://tutorialedge.net/golang/reading-console-input-golang/
	reader := bufio.NewReader(os.Stdin)
	fmt.Println("Simple Shell")
	fmt.Println("------------")

	go func() {
		for {
			fmt.Print("-> ")
			text, _ := reader.ReadString('\n')
			// convert CRLF to LF
			e := "\n"
			if runtime.GOOS == "windows" {
				e = "\r\n"
			}
			text = strings.Replace(text, e, "", -1) // Linux "\n" !

			switch text {
			case "q", "quit":
				os.Exit(0)
			case "h", "help":
				fmt.Println("h|help    - this text")
				fmt.Println("q|quit    - end program")
			default:
				fmt.Printf("Unknown command '%s' - use 'help'\n", text)
			}
		}
	}() // https://stackoverflow.com/questions/16008604/why-add-after-closure-body-in-golang
}

// doSerialReceive is the endless loop for trice logging
func doSerialReceive() error {
	err := conditionalComPortScan()
	if err != nil {
		return err
	}
	serialReceiver := receiver.NewSerialReceiver(port, baud)

	if serialReceiver.SetUp() == false {
		fmt.Println("Could not set up serial port", port)
		fmt.Println("try -port COMscan")
		return nil
	}
	fmt.Println("Opened serial port", port)

	serialReceiver.Start()
	defer serialReceiver.CleanUp()

	keyboardInput()

	var t, b []byte
	for {
		select {
		case c := <-(*serialReceiver.GetBufferChannel()):
			if len(c) > 0 {
				//log.Println("from buffer channel:", c)
				b = append(b, c...)
			}
		case t = <-(*serialReceiver.GetTriceChannel()):
			//log.Println("from trice channel:", t)
			b, err = emit.Trice(t, b, *pList)
			if nil != err {
				log.Println("trice.Log error", err, t, b)
			}
		}
	}
}

// zeroIds does replace all ID's in sourc tree with 0
func zeroIds(dryRun bool, SrcZ string, cmd *flag.FlagSet) error {
	if SrcZ == "" {
		cmd.PrintDefaults()
		return errors.New("no source tree root specified")
	}
	id.ZeroSourceTreeIds(SrcZ, !dryRun)
	return nil
}

// scReceiver is the subcommand remoteDisplay and acts as client connecting to the displayServer
func scReceiver(pre, post, ipa, ipp, prt string, bd int, fn, ts, pw string, show, sv bool) error {
	var wg sync.WaitGroup
	disp.IpAddr = ipa
	disp.IpPort = ipp
	port = prt
	baud = bd
	setPrefix(pre)
	emit.Postfix = post
	fnJSON = assign(fn)
	emit.TimeStampFormat = ts
	password = pw
	showPassword = show
	if true == sv {
		disp.StartServer()
	}
	wg.Add(1)
	err := disp.Connect()
	if nil != err {
		return err
	}
	var result int64

	/*err = pRPC.Call("Server.Adder", [2]int64{10, 20}, &result)
	if err != nil {
		fmt.Println(err)
	} else {
		fmt.Println("Server.Adder(10,20) =", result)
	}*/
	err = disp.PtrRpc.Call("Server.Visualize", "att:\n\n\nnew connection....\n\n\n", &result)
	if err != nil {
		fmt.Println(err)
	} else {
		fmt.Println("len is", result)
	}
	logTrices() // does not return
	wg.Wait()
	fmt.Println("...done")
	return nil
}
