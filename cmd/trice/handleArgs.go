// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package main

import (
	"bufio"
	"crypto/sha1"
	"errors"
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"net/rpc"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"sync"

	"github.com/rokath/trice/pkg/emit"
	"github.com/rokath/trice/pkg/id"
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

var srcs arrayFlag // gets multiple files or directories

// local config values
var port string
var baud int
var fnJSON string
var pList *id.List
var ipAddr string
var ipPort string
var password string
var showPassword bool
var logFile = "off"

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

	vCmd := flag.NewFlagSet("version", flag.ContinueOnError)                               // subcommand
	pVlf := vCmd.String("lf", "trice.log", "append all output to logfile, set to \"off\"") // flag

	scLog := flag.NewFlagSet("log", flag.ExitOnError)                                       // subcommand
	pPort := scLog.String("port", "COMscan", "COM port, options: COM1|...|COM999")          // flag
	pBaud := scLog.Int("baud", 115200, "COM baudrate")                                      // flag
	pJSON := scLog.String("list", "til.json", "trice ID list path")                         // flag
	pTs := scLog.String("ts", "LOCmicro", "timestamp, options: off|UTCmicro")               // flag
	pCol := scLog.String("color", "default", "color set, options: off|alternate")           // flag
	pKey := scLog.String("key", "none", "decrypt passphrase")                               // flag
	pShow := scLog.Bool("show", false, "show passphrase")                                   // flag
	pLlf := scLog.String("lf", "trice.log", "append all output to logfile, set to \"off\"") // flag

	scCl := flag.NewFlagSet("remoteDisplay", flag.ExitOnError)                      // subcommand
	pClPort := scCl.String("port", "COMscan", "COM port, options: COM1|...|COM999") // flag
	pClBaud := scCl.Int("baud", 115200, "COM baudrate")                             // flag
	pClJSON := scCl.String("list", "til.json", "trice ID list path")                // flag
	pClKey := scCl.String("key", "none", "decrypt passphrase")                      // flag
	pClShow := scCl.Bool("show", false, "show passphrase")                          // flag
	pClIPA := scCl.String("ipa", "localhost", "ip address")                         // flag (127.0.0.1)
	pClIPP := scCl.String("ipp", "61497", "16 bit ip port number")                  // flag
	pClTs := scCl.String("ts", "LOCmicro", "timestamp, options: off|UTCmicro")      // flag
	pClSrv := scCl.Bool("ds", false, "start display server ")                       // flag

	scSv := flag.NewFlagSet("displayServer", flag.ExitOnError)                     // subcommand
	pSvIPA := scSv.String("ipa", "localhost", "ip address")                        // flag (127.0.0.1)
	pSvIPP := scSv.String("ipp", "61497", "16 bit port number")                    // flag
	pSvCol := scSv.String("color", "default", "color set, options: off|alternate") // flag
	pSvTs := scSv.String("ts", "LOCmicro", "timestampm options: off|UTCmicro")     // flag

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
	case "h", "help":
		err = hCmd.Parse(subArgs)
	case "v", "ver", "version":
		err = vCmd.Parse(subArgs)
	case "u", "update":
		err = scUpd.Parse(subArgs)
	case "check":
		err = scChk.Parse(subArgs)
	case "l", "log":
		err = scLog.Parse(subArgs)
	case "zeroSourceTreeIds":
		err = scZero.Parse(subArgs)
	case "ds", "displayServer":
		err = scSv.Parse(subArgs)
	case "rd", "remoteDisplay":
		err = scCl.Parse(subArgs)
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
		return scHelp(hCmd, scUpd, scChk, scLog, scZero, vCmd, scSv, scCl)
	}
	if scUpd.Parsed() {
		return scUpdate(*pDryR, *pLU, *pVerb)
	}
	if scChk.Parsed() {
		return scCheckList(*pC, *pSet, *pPal)
	}
	if scLog.Parsed() {
		return scLogging(*pPort, *pBaud, *pJSON, *pTs, *pCol, *pKey, *pShow, *pLlf)
	}
	if scZero.Parsed() {
		return zeroIds(*pRunZ, *pSrcZ, scZero)
	}
	if vCmd.Parsed() {
		return scVersion(*pVlf)
	}
	if scSv.Parsed() {
		return scDisplayServer(*pSvTs, *pSvCol, *pSvIPA, *pSvIPP)
	}
	if scCl.Parsed() {
		return scRemoteDisplay(*pClIPA, *pClIPP, *pClPort, *pClBaud, *pClJSON, *pClTs, *pClKey, *pClShow, *pClSrv)
	}
	return nil
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

var tee io.Writer

func setTee() (*os.File, *os.File) {
	var err error
	old := os.Stdout
	tee = os.Stdout
	var lfHandle *os.File
	if "off" != logFile {
		lfHandle, err = os.OpenFile(logFile, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
		if err != nil {
			log.Fatalf("error opening file: %v", err)
		}

		tee = io.MultiWriter(os.Stdout, lfHandle)
		log.SetOutput(tee)
	}
	return old, lfHandle
}

func resetTee(lfHandle, old *os.File) {
	if nil != lfHandle {
		os.Stdout = old
		lfHandle.Close()
	}
}

/*
// https://medium.com/@hau12a1/golang-capturing-log-println-and-fmt-println-output-770209c791b4
func captureOutput(f func()) string {
	reader, writer, err := os.Pipe()
	if err != nil {
		panic(err)
	}
	stdout := os.Stdout
	stderr := os.Stderr
	defer func() {
		os.Stdout = stdout
		os.Stderr = stderr
		log.SetOutput(os.Stderr)
	}()
	os.Stdout = writer
	os.Stderr = writer
	log.SetOutput(writer)
	out := make(chan string)
	wg := new(sync.WaitGroup)
	wg.Add(1)
	go func() {
		var buf bytes.Buffer
		wg.Done()
		io.Copy(&buf, reader)
		out <- buf.String()
	}()
	wg.Wait()
	f()
	writer.Close()
	return <-out
}

func scVersion1(lfn string) error {
	var err error
	re := captureOutput(func() {
		err = scVersion0(lfn)
	})
	old, lfHandle := setTee(lfn)
	fmt.Fprint(tee, re)
	resetTee(old, lfHandle)
	return err
}

func scVersion0(fn string) error {
	logFile = fn
	if "" != version {
		fmt.Printf("version=%v, commit=%v, built at %v\n", version, commit, date)
		return nil
	}
	fmt.Printf("version=devel, commit=unknown, built after 2020-03-22-1815\n")
	return errors.New("No goreleaser generated executable")
}

// this works too
func scVersion00(fn string) error {
	logFile = fn
	var tee io.Writer
	tee = os.Stdout
	if "off" != logFile {
		f, err := os.OpenFile(logFile, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
		if err != nil {
			log.Fatalf("error opening file: %v", err)
		}
		defer f.Close()
		tee = io.MultiWriter(os.Stdout, f)
		log.SetOutput(tee)
	}

	if "" != version {
		fmt.Fprintf(tee, "version=%v, commit=%v, built at %v\n", version, commit, date)
		return nil
	}
	fmt.Fprintf(tee, "version=devel, commit=unknown, built after 2020-03-22-1815\n")
	return errors.New("No goreleaser generated executable")
}*/

// this works too
func scVersion(lfn string) error {
	logFile = lfn
	old, lfHandle := setTee()
	defer resetTee(old, lfHandle)

	if "" != version {
		fmt.Fprintf(tee, "version=%v, commit=%v, built at %v\n", version, commit, date)
	} else {
		fmt.Fprintf(tee, "version=devel, commit=unknown, built after 2020-03-22-2305\n")
	}
	return nil
}

func scHelp(hCmd *flag.FlagSet,
	u *flag.FlagSet,
	c *flag.FlagSet,
	l *flag.FlagSet,
	z *flag.FlagSet,
	v *flag.FlagSet,
	sv *flag.FlagSet,
	cl *flag.FlagSet) error {
	fmt.Println("syntax: 'trice subcommand' [params]")
	fmt.Println("subcommand 'help', 'h'")
	hCmd.PrintDefaults()
	fmt.Println("subcommand 'u', 'upd', 'update'")
	u.PrintDefaults()
	fmt.Println("subcommand 'check'")
	c.PrintDefaults()
	fmt.Println("subcommand 'l', 'log'")
	l.PrintDefaults()
	fmt.Println("subcommand 'zeroSourceTreeIds' (avoid using this subcommand normally)")
	z.PrintDefaults()
	fmt.Println("subcommand 'v', 'ver', 'version'")
	v.PrintDefaults()
	fmt.Println("subcommand 'ds', 'displayServer'")
	sv.PrintDefaults()
	fmt.Println("subcommand 'rd', 'remoteDisplay'")
	cl.PrintDefaults()
	fmt.Println("examples:")
	fmt.Println("    'trice update [-src sourcerootdir]', default sourcerootdir is ./")
	fmt.Println("    'trice log [-port COMn] [-baud m]', default port is COMscan, default m is 38400, fixed to 8N1")
	fmt.Println("    'trice zeroSourceTreeIds -dir sourcerootdir]'")
	fmt.Println("    'trice version'")
	return scVersion(logFile)
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

// parse source tree, update IDs and is list
func update(dryRun bool, dir, fn string, verbose bool) error {
	err := pList.Update(dir, fn, !dryRun, verbose)
	if nil != err {
		return fmt.Errorf("failed update on %s with %s: %v", dir, fn, err)
	}
	fmt.Println(len(*pList), "ID's in list", fn)
	return nil
}

// log the id list with dataset
func scCheckList(fn, dataset, palette string) error {
	emit.TimeStampFormat = "off"
	emit.ColorPalette = palette
	fnJSON = assign(fn)
	err := pList.Read(fnJSON)
	if nil != err {
		fmt.Println("ID list " + fnJSON + " not found, exit")
		return nil
	}
	emit.Check(*pList, dataset)
	return nil
}

// with password "none" the encryption flag is set false, otherwise true
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

// scLog connects to COM port and displays traces
func scLogging(prt string, bd int, fn, ts, col, pw string, show bool, lfn string) error {
	logFile = lfn
	old, lfHandle := setTee()
	defer resetTee(old, lfHandle)

	port = prt
	baud = bd
	fnJSON = assign(fn)
	emit.TimeStampFormat = ts
	emit.ColorPalette = col
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
	} else {
		return errors.New("Could not find serial port on system")
	}
}

// https://tutorialedge.net/golang/reading-console-input-golang/
func keyboardInput() {
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

// replace all ID's in sourc tree with 0
func zeroIds(dryRun bool, SrcZ string, cmd *flag.FlagSet) error {
	if SrcZ == "" {
		cmd.PrintDefaults()
		return errors.New("no source tree root specified")
	}
	id.ZeroSourceTreeIds(SrcZ, !dryRun)
	return nil
}

// This code was derived from the information in:
// https://stackoverflow.com/questions/37122401/execute-another-go-program-from-within-a-golang-program/37123869#37123869
// "4 - another way is using "net/rpc", this is best way for calling another function from another program."
//

// Server is the RPC struct for registered server dunctions
type Server struct{}

// Visualize is the exported server function for string display, if trice tool acts as display server.
// By declaring is as a Server struct method it is registered as RPC destination.
func (p *Server) Visualize(s string, reply *int64) error {
	*reply = int64(len(s))
	return emit.Visualize(s) // this function pointer has its default value on server side
}

/*
// Adder is a demo for a 2nd function
func (p *Server) Adder(u [2]int64, reply *int64) error {
	*reply = u[0] + u[1]
	return nil
}
*/

// displayServer is the endless function called when trice tool acts as remote display.
// All in Server struct registered RPC functions are reachable, when displayServer runs.
func scDisplayServer(ts, pal, ipa, ipp string) error {
	emit.TimeStampFormat = ts
	emit.ColorPalette = pal
	ipAddr = ipa
	ipPort = ipp
	a := fmt.Sprintf("%s:%s", ipAddr, ipPort)
	fmt.Println("displayServer @", a)
	rpc.Register(new(Server))

	ln, err := net.Listen("tcp", a)
	if err != nil {
		fmt.Println(err)
		return err
	}
	for {
		c, err := ln.Accept()
		if err != nil {
			continue
		}
		go rpc.ServeConn(c)
	}
}

var wg sync.WaitGroup
var pRPC *rpc.Client

func remoteVisualize(s string) error {
	var result int64
	err := pRPC.Call("Server.Visualize", s, &result)
	if err != nil {
		return err
	}
	//fmt.Println("result is", result)
	return nil
}

// client
func scRemoteDisplay(ipa, ipp, prt string, bd int, fn, ts, pw string, show, sv bool) error {
	ipAddr = ipa
	ipPort = ipp
	port = prt
	baud = bd
	fnJSON = assign(fn)
	emit.TimeStampFormat = ts
	password = pw
	showPassword = show
	if true == sv {
		var shell string
		var clip string
		if runtime.GOOS == "windows" {
			shell = "cmd"
			clip = "/c start trice displayServer -ipa " + ipAddr + " -ipp " + ipPort
		}
		cmd := exec.Command(shell, clip)
		err := cmd.Run()
		if err != nil {
			log.Println(clip)
			log.Fatal(err)
		}
	}
	wg.Add(1)
	a := fmt.Sprintf("%s:%s", ipAddr, ipPort)
	fmt.Println("remoteDisplay@", a)
	var err error
	pRPC, err = rpc.Dial("tcp", a)
	if err != nil {
		fmt.Println(err)
		return err
	}
	fmt.Println("Connected...")
	var result int64
	emit.Visualize = remoteVisualize // re-direct output
	/*err = pRPC.Call("Server.Adder", [2]int64{10, 20}, &result)
	if err != nil {
		fmt.Println(err)
	} else {
		fmt.Println("Server.Adder(10,20) =", result)
	}*/
	err = pRPC.Call("Server.Visualize", "att:\n\n\nnew connection....\n\n\n", &result)
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
