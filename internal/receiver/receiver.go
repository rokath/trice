// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"path/filepath"
	"runtime"
	"time"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/cipher"
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// ShowInputBytes displays incoming bytes if set true
	ShowInputBytes bool

	// Port is the trice receiver to use.
	Port string

	// PortArguments are the trice receiver device specific arguments.
	PortArguments string
)

// NewInputPort returns a ReadCloser for the specified port and its args.
// err is nil on successful open.
// When port is "COMn" args can be used to be "TARM" to use a different driver for dynamic testing.
// When port is "BUFFER", args is expected to be a byte sequence in the same format as for example coming from one of the other ports.
// When port is "JLINK" args contains JLinkRTTLogger.exe specific parameters described inside UM08001_JLink.pdf.
// When port is "STLINK" args has the same format as for "JLINK"
func NewInputPort(port, args string) (r io.ReadCloser, err error) {
	switch port {
	case "JLINK", "STLINK":
		l := link.NewDevice(port, args)
		if nil != l.Open() {
			err = fmt.Errorf("can not open link device %s with args %s", port, args)
		}
		r = l
	default: // assuming serial port
		var c com.COMport   // interface type
		if "TARM" == args { // for comparing dynamic behaviour
			c = com.NewCOMPortTarm(port)
		} else {
			c = com.NewCOMPortGoBugSt(port)
		}
		if false == c.Open() {
			err = fmt.Errorf("can not open %s", port)
		}
		r = c
		return
	case "BUFFER":
		r = ioutil.NopCloser(bytes.NewBufferString(args))
	}
	return
}

// Loop prepares writing and list and provides a retry mechanism for unplugged UART.
func Loop() {
	if !emitter.DisplayRemote {
		cage.Enable()
		defer cage.Disable()
	}
	translatePrefix()

	lwD := emitter.NewLineWriter()
	list := NewList()

	// lineComposer implements the io.StringWriter interface and uses the line writer provided.
	// The line composer scans the trice strings and composes lines out of them according to its properties.
	sw := emitter.NewLineComposer(lwD)

	// over this channel read errors despite io.EOF reported
	hardReadError := make(chan bool)
	for {
		f := receiving(sw, list, hardReadError)
		if false == f {
			return
		}
		time.Sleep(100 * time.Millisecond) // retry interval
	}
}

// receiving performs the trice log task, uses internally Port and encoding and
// returns false on program end signal or true on hard read error.
func receiving(sw *emitter.TriceLineComposer, list *id.List, hardReadError chan bool) bool {

	// (re-)setup input port
	portReader, e := NewInputPort(Port, PortArguments)
	if nil != e {
		if Verbose {
			fmt.Println(e)
		}
		return true
	}
	defer portReader.Close()
	if ShowInputBytes {
		portReader = newBytesViewer(portReader)
	}

	// activate selected encoding
	//var p translator.Translator // interface type
	switch decoder.Encoding {
	case "esc":
		dec := decoder.NewEsc(list.ItemList, portReader)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = decoder.NewEsc(list.ItemList, portReader) // read list again - it could have changed
			}
		}
	case "bare":
		dec := decoder.NewBare(list.ItemList, portReader)
		for {
			err := run(sw, dec)
			if nil != err {
				time.Sleep(2 * time.Second)
				dec = decoder.NewBare(list.ItemList, portReader) // read list again - it could have changed
			}
		}

	//case "bare":
	//	p = receiveBareSimpleTricesAndDisplayAnsiColor(sw, portReader, list, hardReadError)
	//case "esc":
	//p = receiveEscTricesAndDisplayAnsiColor(sw, portReader, list, hardReadError)
	//case "wrap", "wrapped":
	//p = receiveWrapSimpleTricesAndDisplayAnsiColor(portReader, fnJSON)
	//	case "sim":
	//		p = simNewSimpleTriceInterpreterWithAnsi(r)
	case "bareXTEACrypted", "wrapXTEACrypted":
		errorFatal(cipher.SetUp())
		fallthrough
	case "ascii":
		fallthrough
	default:
		fmt.Println("unknown encoding ", decoder.Encoding)
		return false
	}

	//// prepare CTRL-C shutdown reaction
	//sigs := make(chan os.Signal, 1)
	//signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
	//select {
	//case <-hardReadError:
	//	if verbose {
	//		//fmt.Println("####################################", p.SavedError(), "####################################")
	//	}
	//	// p.Done() <- 0 // end translator
	//	return true
	//case sig := <-sigs: // wait for a signal
	//	if verbose {
	//		fmt.Println("####################################", sig, "####################################")
	//	}
	//	//p.Done() <- 0 // end translator
	//	return false // back to main
	//}
}

// NewList returns a pointer to a list struct which stays up-to-date in case the til.json file changes.
func NewList() (l *id.List) {
	l = id.NewList(id.FnJSON)
	l.ReadListFile()
	go l.FileWatcher()
	return
}

func run(sw *emitter.TriceLineComposer, dec decoder.StringsReader) error {
	ss := make([]string, 100)
	n, err := dec.StringsRead(ss)
	if nil != err && io.EOF != err {
		return err
	}
	for i := range ss[:n] {
		sw.WriteString(ss[i])
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// dynamic debug helper
//

type bytesViewer struct {
	r io.ReadCloser
}

// newBytesViewer returns a ReadCloser `in` which is internally using reader `from`.
// Calling the `in` Read method leads to internally calling the `from` Read method
// but lets to do some additional action like logging
func newBytesViewer(from io.ReadCloser) (in io.ReadCloser) {
	return &bytesViewer{from}
}

func (p *bytesViewer) Read(buf []byte) (count int, err error) {
	count, err = p.r.Read(buf)
	if 0 < count || (nil != err && io.EOF != err) {
		log.Println("input bytes:", err, count, buf[:count])
	}
	return
}

// Close is needed to satify the ReadCloser interface.
func (p *bytesViewer) Close() error { return nil }

//
///////////////////////////////////////////////////////////////////////////////////////////////////

// not needed
//func runEsc2(sw *emitter.TriceLineComposer, list *id.List) {
//again: // (re-)setup input port
//	portReader, e := decoder.NewInputPort(Port, "")
//	if nil != e {
//		if verbose {
//			fmt.Println(e)
//		}
//		time.Sleep(1000 * time.Millisecond)
//		goto again
//	}
//	defer portReader.Close()
//	if showInputBytes {
//		portReader = newBytesViewer(portReader)
//	}
//	//var dec decoder.StringsReader = decoder.NewEsc(list, portReader)
//}
//
//
// This works well but is not well refactored
//func receiveEscTricesAndDisplayAnsiColor(
//	sw *emitter.TriceLineComposer,
//	rd io.ReadCloser,
//	list *id.List,
//	hardReadError chan bool) (et *translator.EscTranslator) {
//
//	// uses rd for reception and the io.StringWriter interface sw for writing.
//	// collects trice bytes to a complete esc trice message, generates the appropriate string using list and writes it to the provided io.StringWriter
//	et = translator.NewEscTrices(sw, list, rd, hardReadError)
//	return
//}
//

/*
func receiveBareSimpleTricesAndDisplayAnsiColor(
	sw *emitter.TriceLineComposer,
	rd io.ReadCloser,
	list *id.List,
	hardReadError chan bool) (bt *translator.BareTranslator) {

	// triceAtomsReceiver uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	triceAtomsReceiver := receiver.NewTricesfromBare(rd, hardReadError)

	// bt uses triceAtomsReceiver for reception and the io.StringWriter interface sw for writing.
	// It collects trice atoms to a complete trice, generates the appropriate string using list and writes it to the provided io.StringWriter
	bt = translator.NewSimpleTrices(sw, list, triceAtomsReceiver)
	return
}
*/
