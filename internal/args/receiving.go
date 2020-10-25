// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package args implemets the commandline interface and calls the appropriate commands.
package args

import (
	"fmt"
	"io"
	"log"
	"os"
	"os/signal"
	"path/filepath"
	"runtime"
	"syscall"
	"time"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/translator"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/cipher"
)

// newInputPort uses variable Port and tries to return a valid io.ReadCloser.
func newInputPort() (r io.ReadCloser, e error) {
	switch Port {
	case "JLINK", "STLINK":
		l := link.NewDevice()
		if nil != l.Open() {
			e = fmt.Errorf("can not open link device %s with args %s", Port, link.Args)
		}
		r = l
	default: // assuming serial port
		var c com.COMport // interface type
		c = com.NewCOMPortGoBugSt(Port)
		//c = com.NewCOMPortTarm(Port)
		if false == c.Open() {
			e = fmt.Errorf("can not open %s", Port)
		}
		r = c
		return
	}
	return
}

// NewList returns a list struct which stays up-to-date in case the til.json file changes.
func NewList() (l *id.List) {
	l = id.NewList(fnJSON)
	l.ReadListFile()
	go l.FileWatcher()
	return
}

// doReceive prepares writing and list and provides a retry mechanism for unplugged UART.
func doReceive() {
	if !displayRemote {
		cage.Enable()
		defer cage.Disable()
	}
	translatePrefix()
	fnJSON = id.ConditionalFilePath(fnJSON)
	lwD := emitter.NewLineWriter(displayRemote, autostart)
	list := NewList()

	// lineComposer implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
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

func runEsc2(sw *emitter.TriceLineComposer, list *id.List) {
again: // (re-)setup input port
	portReader, e := newInputPort()
	if nil != e {
		if verbose {
			fmt.Println(e)
		}
		time.Sleep(1000 * time.Millisecond)
		goto again
	}
	defer portReader.Close()
	if showInputBytes {
		portReader = newBytesViewer(portReader)
	}
	//var dec decoder.StringsReader = decoder.NewEsc(list, portReader)
}

// receiving performs the trice log task, uses internally Port and encoding and
// returns false on program end signal or true on hard read error.
func receiving(sw *emitter.TriceLineComposer, list *id.List, hardReadError chan bool) bool {

	// (re-)setup input port
	portReader, e := newInputPort()
	if nil != e {
		if verbose {
			fmt.Println(e)
		}
		return true
	}
	defer portReader.Close()
	if showInputBytes {
		portReader = newBytesViewer(portReader)
	}

	// activate selected encoding
	var p translator.Translator // interface type
	switch encoding {
	case "esc2":
		var dec decoder.StringsReader = decoder.NewEsc(list, portReader)
		run(sw, dec)

	case "bare":
		p = receiveBareSimpleTricesAndDisplayAnsiColor(sw, portReader, list, hardReadError)
	case "esc":
		p = receiveEscTricesAndDisplayAnsiColor(sw, portReader, list, hardReadError)
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
		fmt.Println("unknown encoding ", encoding)
		return false
	}

	// prepare CTRL-C shutdown reaction
	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
	select {
	case <-hardReadError:
		if verbose {
			fmt.Println("####################################", p.SavedError(), "####################################")
		}
		p.Done() <- 0 // end translator
		return true
	case sig := <-sigs: // wait for a signal
		if verbose {
			fmt.Println("####################################", sig, "####################################")
		}
		p.Done() <- 0 // end translator
		return false  // back to main
	}
}

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

func receiveEscTricesAndDisplayAnsiColor(
	sw *emitter.TriceLineComposer,
	rd io.ReadCloser,
	list *id.List,
	hardReadError chan bool) (et *translator.EscTranslator) {

	// uses rd for reception and the io.StringWriter interface sw for writing.
	// collects trice bytes to a complete esc trice message, generates the appropriate string using list and writes it to the provided io.StringWriter
	et = translator.NewEscTrices(sw, list, rd, hardReadError)
	return
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
	if verbose {
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

func (p *bytesViewer) Close() error { return nil } // todo: Why is Close() method needed here?

//
///////////////////////////////////////////////////////////////////////////////////////////////////
