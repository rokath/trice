// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This is possibly obsolete information!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Package receiver is responsible for getting and handling the bytes from the communication port.
// There ae different communication ports possible: COM, RTT, TCP...
// It is activated by the trice package and calls emit.Trice() on received bytes chunks.
//
// Implementation
// ==============
// - trice.DoReceive() is called when trice ist started in log or receiver mode
// - In dependence on receiverDevice DoSerial() or DoSeggerRTT() or ... is activated (named DoDevice() from now)
// - DoDevice()
//   - performs device specific initialization
//   - performs Start() which starts the go routine receiving(), responsible for raw data (see below)
//   - and ends in device.doReceive()
// - device is a composed type from triceReceiver (common functionality) and deviceReceiver (device specific functionality)
// - The newDevice(...) function creates the device specific instance and links to it the triceReceiver instance created by
// device.triceReceiver = newTriceReceiver(device). The device parameter for newTriceReceiver() is the linked device interface
// into the triceReceiver instance so that the this way composed device instance can use the common triceReceiver parts as well.
// - receiving()
//  - scans the incoming data stream for valid headers,
//  - and puts these into the triceChannel, when trice headers
//  - or puts them together with the following package data into the buffer channel if it is a valid package
//  - It is up to the trice implementation to add more channels here for other protocols
// - doReceive() is the endless loop waiting for data in the channels filled in by receiving()
//  - All incoming buffer packets are collected into a byte slice for later usage (case `TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT_EXPERIMENTAL`)
//  - If a trice header comes out of the triceChannel emit.Trice() is called
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

package receiver

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"strings"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/pkg/msg"
)

var (
	// ShowInputBytes displays incoming bytes if set true.
	ShowInputBytes bool

	// Port is the trice receiver to use.
	Port string

	// PortArguments are the trice receiver device specific arguments.
	PortArguments string
)

// scanBytes assumes in s whitespace separated decimal numbers between 0 and 255 and returns them in buf
func scanBytes(s string) (buf []byte) {
	s = strings.ReplaceAll(s, ",", " ")
	s = strings.ReplaceAll(s, "\t", " ")
	s = strings.ReplaceAll(s, "\r", " ")
	s = strings.ReplaceAll(s, "\n", " ")
	s = strings.ReplaceAll(s, "  ", " ")
	as := strings.Split(s, " ")
	buf = make([]byte, 0)
	for _, a := range as {
		var b byte
		n, e := fmt.Sscan(a, &b)
		msg.FatalOnFalse(1 == n)
		msg.FatalOnErr(e)
		buf = append(buf, b)
	}
	return
}

// NewReadCloser returns a ReadCloser for the specified port and its args.
// err is nil on successful open.
// When port is "COMn" args can be used to be "TARM" to use a different driver for dynamic testing.
// When port is "BUFFER", args is expected to be a byte sequence in the same format as for example coming from one of the other ports.
// When port is "JLINK" args contains JLinkRTTLogger.exe specific parameters described inside UM08001_JLink.pdf.
// When port is "STLINK" args has the same format as for "JLINK"
func NewReadCloser(port, args string) (r io.ReadCloser, err error) {
	switch port {
	case "JLINK", "STLINK", "J-LINK", "ST-LINK":
		l := link.NewDevice(port, args)
		if nil != l.Open() {
			err = fmt.Errorf("can not open link device %s with args %s", port, args)
		}
		r = l
	case "BUFFER":
		buf := scanBytes(args)
		r = ioutil.NopCloser(bytes.NewBuffer(buf))
	default: // assuming serial port
		var c com.COMport   // interface type
		if "TARM" == args { // for comparing dynamic behaviour
			c = com.NewCOMPortTarm(port)
		} else {
			c = com.NewCOMPortGoBugSt(port)
		}
		if !c.Open() {
			err = fmt.Errorf("can not open %s", port)
		}
		r = c
		return
	}
	return
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// dynamic debug                                                                                 //
//                                                                                               //
type bytesViewer struct {
	r io.ReadCloser
}

// NewBytesViewer returns a ReadCloser `in` which is internally using reader `from`.
// Calling the `in` Read method leads to internally calling the `from` Read method
// but lets to do some additional action like logging
func NewBytesViewer(from io.ReadCloser) (in io.ReadCloser) {
	return &bytesViewer{from}
}

func (p *bytesViewer) Read(buf []byte) (count int, err error) {
	count, err = p.r.Read(buf)
	if 0 < count || (nil != err && io.EOF != err) {
		fmt.Print("Input( ")
		for _, x := range buf[:count] {
			fmt.Printf("%02x ", x)
		}
		fmt.Println(")")
	}
	return
}

// Close is needed to satify the ReadCloser interface.
func (p *bytesViewer) Close() error { return nil }

//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This is possibly obsolete code
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  // Loop prepares writing and list and provides a retry mechanism for unplugged UART.
//  func Loop() {
//
//  	list := id.New()
//  	sw := emitter.New(emitter.Prefix)
//
//  	for {
//
//  		// (re-)setup input port
//  		rc, e := NewReader(Port, PortArguments)
//  		if nil != e {
//  			if Verbose {
//  				fmt.Println(e)
//  			}
//  			return // true
//  		}
//  		defer rc.Close()
//  		if ShowInputBytes {
//  			rc = newBytesViewer(rc)
//  		}
//
//  		f := decoder.Translate(sw, list, rc)
//  		if false == f {
//  			return
//  		}
//
//  		time.Sleep(100 * time.Millisecond) // retry interval
//  	}
//  }
//
//  // errorFatal ends in osExit(1) if err not nil.
//  func errorFatal(err error) {
//  	if nil == err {
//  		return
//  	}
//  	if Verbose {
//  		_, file, line, _ := runtime.Caller(1)
//  		log.Fatal(err, " "+filepath.Base(file)+" ", line)
//  	}
//  	log.Fatal(err)
//
//  // not needed
//  func runEsc2(sw *emitter.TriceLineComposer, list *id.List) {
//  again: // (re-)setup input port
//  	portReader, e := decoder.NewReader(Port, "")
//  	if nil != e {
//  		if verbose {
//  			fmt.Println(e)
//  		}
//  		time.Sleep(1000 * time.Millisecond)
//  		goto again
//  	}
//  	defer portReader.Close()
//  	if showInputBytes {
//  		portReader = newBytesViewer(portReader)
//  	}
//  	//var dec decoder.StringsReader = decoder.NewEsc(list, portReader)
//  }
//
//
//  // This works well but is not well refactored
//  func receiveEscTricesAndDisplayAnsiColor(
//  	sw *emitter.TriceLineComposer,
//  	rd io.ReadCloser,
//  	list *id.List,
//  	hardReadError chan bool) (et *translator.EscTranslator) {
//
//  	// uses rd for reception and the io.StringWriter interface sw for writing.
//  	// collects trice bytes to a complete esc trice message, generates the appropriate string using list and writes it to the provided io.StringWriter
//  	et = translator.NewEscTrices(sw, list, rd, hardReadError)
//  	return
//  }
//
//  func receiveBareSimpleTricesAndDisplayAnsiColor(
//  	sw *emitter.TriceLineComposer,
//  	rd io.ReadCloser,
//  	list *id.List,
//  	hardReadError chan bool) (bt *translator.BareTranslator) {
//
//  	// triceAtomsReceiver uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
//  	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
//  	triceAtomsReceiver := receiver.NewTricesfromBare(rd, hardReadError)
//
//  	// bt uses triceAtomsReceiver for reception and the io.StringWriter interface sw for writing.
//  	// It collects trice atoms to a complete trice, generates the appropriate string using list and writes it to the provided io.StringWriter
//  	bt = translator.NewSimpleTrices(sw, list, triceAtomsReceiver)
//  	return
//  }
