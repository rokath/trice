// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implemets the commandline interface and calls the appropriate commands.
package args

import (
	"io"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/translator"
)

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

/*
func xxxmain() {
	watcher, err := fsnotify.NewWatcher()
	if err != nil {
		log.Fatal(err)
	}
	defer watcher.Close()

	done := make(chan bool)
	go func() {
		for {
			select {
			case event, ok := <-watcher.Events:
				if !ok {
					return
				}
				log.Println("event:", event)
				if event.Op&fsnotify.Write == fsnotify.Write {
					log.Println("modified file:", event.Name)
				}
			case err, ok := <-watcher.Errors:
				if !ok {
					return
				}
				log.Println("error:", err)
			}
		}
	}()

	err = watcher.Add("/tmp/foo")
	if err != nil {
		log.Fatal(err)
	}
	<-done
}
*/

//case "check":
//	fsScCheck.Parse(subArgs)
//	id.FnJSON = id.ConditionalFilePath(id.FnJSON)
//	cage.Enable()
//	defer cage.Disable()
//	return emitter.ScCheckList(*pSet)

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

// rndMode is flag value for random mode
//rndMode string

// rndLimit is flag vaue for byte count limit in random mode
//rndLimit int

// fsScCheck is flag set for sub command check for evaluating ID list
// fsScCheck *flag.FlagSet

// pSet is a string pointer to the data set used for scCheck
//pSet *string

// func init() {
// 	fsScCheck = flag.NewFlagSet("check", flag.ExitOnError)                                           // subcommand
// 	pSet = fsScCheck.String("dataset", "position", "parameters, option: 'negative'")                 // flag
// 	fsScCheck.StringVar(&emitter.ColorPalette, "color", "default", "color set, options: 'off|none'") // flag
// 	flagLogfile(fsScCheck)
// 	flagVerbosity(fsScCheck)
// 	flagIDList(fsScCheck)
// }

//fsScLog.StringVar(&rndMode, "rndMode", "WrapModeWithValidCrc", "valid only for '-source RND', see randomdummy.go, options: 'ChaosMode|BareModeNoSync'")
//fsScLog.IntVar(&rndLimit, "rndLimit", randomdummy.NoLimit, "valid only for '-source RND', see randomdummy.go, options: 'n|0', 'n' is count of bytes, '0' for unlimited count")

//fmt.Fprintln(c.Output(), "subcommand 'check' for dispaying the ID list in trice log format")
//c.PrintDefaults()
