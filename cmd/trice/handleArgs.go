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

// HandleArgs evaluates the arguments slice of strings und uses wd as working directory
func HandleArgs(wd string, args []string) error {
	list := make(id.List, 0, 65536) // for 16 bit IDs enough
	pList = &list

	uCmd := flag.NewFlagSet("update", flag.ExitOnError)                                        // subcommand
	pDryR := uCmd.Bool("dry-run", false, "no changes are applied (optional)")                  // flag
	pLU := uCmd.String("list", "til.json", "trice ID list path (optional), \"none\" possible") // flag
	pVerb := uCmd.Bool("v", false, "verbose (optional)")                                       // flag
	uCmd.Var(&srcs, "src", "source dir or file (optional, default is ./), multi use possible") // multi flag

	cCmd := flag.NewFlagSet("check", flag.ExitOnError)                              // subcommand
	pSet := cCmd.String("dataset", "position", "parameters (optional), negative")   // flag
	pC := cCmd.String("list", "til.json", "trice ID list path (optional)")          // flag
	pPal := cCmd.String("color", "default", "color set (optional), off, alternate") // flag

	zCmd := flag.NewFlagSet("zeroSourceTreeIds", flag.ContinueOnError)                  // subcommand (during development only)
	pSrcZ := zCmd.String("src", "", "zero all Id(n) inside source tree dir (required)") // flag
	pRunZ := zCmd.Bool("dry-run", false, "no changes are applied (optional)")           // flag

	hCmd := flag.NewFlagSet("help", flag.ContinueOnError) // subcommand

	vCmd := flag.NewFlagSet("version", flag.ContinueOnError) // subcommand

	lCmd := flag.NewFlagSet("log", flag.ExitOnError)                                // subcommand
	pPort := lCmd.String("port", "", "subcommand (required, try COMscan)")          // flag
	pBaud := lCmd.Int("baud", 115200, "COM baudrate (optional, default is 115200")  // flag
	pJSON := lCmd.String("list", "til.json", "trice ID list path (optional)")       // flag
	pTs := lCmd.String("ts", "LOCmicro", "timestamp (optional), off, UTCmicro")     // flag
	pCol := lCmd.String("color", "default", "color set (optional), off, alternate") // flag
	pKey := lCmd.String("key", "none", "decrypt passphrase, (optional)")            // flag
	pShow := lCmd.Bool("show", false, "show passphrase (optional)")                 // flag

	clCmd := flag.NewFlagSet("remoteDisplay", flag.ExitOnError)                               // subcommand
	pClSrv := clCmd.Bool("ds", false, "start display server (optional)")                      // flag
	pClIPA := clCmd.String("ipa", "localhost", "ip address (optional, default is localhost)") // flag (127.0.0.1)
	pClIPP := clCmd.String("ipp", "61497", "ip port number (required, a 16 bit number)")      // flag
	pClPort := clCmd.String("port", "", "subcommand (required, try COMscan)")                 // flag
	pClBaud := clCmd.Int("baud", 115200, "COM baudrate (optional, default is 115200")         // flag
	pClJSON := clCmd.String("list", "til.json", "trice ID list path (optional)")              // flag
	pClTs := clCmd.String("ts", "LOCmicro", "timestamp (optional), off, UTCmicro")            // flag
	pClKey := clCmd.String("key", "none", "decrypt passphrase, (optional)")                   // flag
	pClShow := clCmd.Bool("show", false, "show passphrase (optional)")                        // flag

	svCmd := flag.NewFlagSet("displayServer", flag.ExitOnError)                               // subcommand
	pSvIPA := svCmd.String("ipa", "localhost", "ip address (optional, default is localhost)") // flag (127.0.0.1)
	pSvIPP := svCmd.String("ipp", "61497", "port number (required, a 16 bit number)")         // flag
	pSvCol := svCmd.String("color", "default", "color set (optional), off, alternate")        // flag
	pSvTs := svCmd.String("ts", "LOCmicro", "timestamp (optional), off, UTCmicro")            // flag

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
		err = uCmd.Parse(subArgs)
	case "check":
		err = cCmd.Parse(subArgs)
	case "l", "log":
		err = lCmd.Parse(subArgs)
	case "zeroSourceTreeIds":
		err = zCmd.Parse(subArgs)
	case "ds", "displayServer":
		err = svCmd.Parse(subArgs)
	case "rd", "remoteDisplay":
		err = clCmd.Parse(subArgs)
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
		return help(hCmd, uCmd, cCmd, lCmd, zCmd, vCmd, svCmd, clCmd)
	}
	if uCmd.Parsed() {
		lU, err := filepath.Abs(*pLU)
		// @basti? fmt.Errorf("%s", lU)
		if nil != err {
			return fmt.Errorf("failed to parse %s: %v", *pLU, err)
		}
		for i := range srcs {
			s := srcs[i]
			srcU, err := filepath.Abs(s)
			if nil != err {
				return fmt.Errorf("failed to parse %s: %v", srcU, err)
			}
			if _, err := os.Stat(srcU); err == nil { // path exists
				err = update(*pDryR, srcU, lU /*pList, */, *pVerb)
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
	if cCmd.Parsed() {
		return checkList(*pC, *pSet, *pPal)
	}
	if lCmd.Parsed() {
		emit.TimeStampFormat = *pTs
		emit.ColorPalette = *pCol
		baud = *pBaud
		port = *pPort
		fnJSON = *pJSON
		password = *pKey
		showPassword = *pShow
		return logTrices()
	}
	if zCmd.Parsed() {
		return zeroIds(*pRunZ, *pSrcZ, zCmd)
	}
	if vCmd.Parsed() {
		return ver()
	}
	if svCmd.Parsed() {
		emit.TimeStampFormat = *pSvTs
		emit.ColorPalette = *pSvCol
		ipAddr = *pSvIPA
		ipPort = *pSvIPP
		return displayServer()
	}
	if clCmd.Parsed() {
		ipAddr = *pClIPA
		ipPort = *pClIPP
		port = *pClPort
		baud = *pClBaud
		fnJSON = *pClJSON
		emit.TimeStampFormat = *pClTs
		password = *pClKey
		showPassword = *pClShow
		if true == *pClSrv {
			clip := "/c start trice displayServer -ipa " + ipAddr + " -ipp " + ipPort
			cmd := exec.Command("cmd", clip)
			err := cmd.Run()
			if err != nil {
				log.Println(clip)
				log.Fatal(err)
			}
		}
		return remoteDisplay()
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
	return ver()
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
func checkList(fn, dataset string /*p *id.List,*/, palette string) error {
	err := pList.Read(fn)
	if nil != err {
		fmt.Println("ID list " + fn + " not found, exit")
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

// connect to port and display traces
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
	fmt.Println("---------------------")

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
			case "hi":
				fmt.Println("privet")
			case "hallo":
				fmt.Println("ahoi")
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

// Display is the exported server function for string display, if trice tool acts as display server.
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
func displayServer() error {
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
func remoteDisplay() error {
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
