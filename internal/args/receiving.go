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

// newInputPort uses variable Port and tries to return a valid io.ReadCloser.
func newInputPort() (r io.ReadCloser, e error) {
	switch Port {
	case "JLINK", "STLINK":
		l := link.NewDevice()
		if nil != l.Open() {
			e = fmt.Errorf("Can not open link device %s", link.Args)
		}
		r = l
	default: // assuming serial port
		var c com.COMport // interface type
		c = com.NewCOMPortGoBugSt(Port)
		//c = com.NewCOMPortTarm(Port)
		if false == c.Open() {
			e = fmt.Errorf("Can not open %s", Port)
		}
		r = c
		return
	}
	return
}

// receiving performs the trice log task, uses internally Port and encoding and returns on program end.
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
	portReader, e := newInputPort()
	errorFatal(e)
	defer portReader.Close()

	var p translator.Translator // interface type

	switch encoding {
	//	case "sim":
	//		p = simNewSimpleTriceInterpreterWithAnsi(r)
	case "bare":
		p = receiveBareSimpleTricesAndDisplayAnsiColor(portReader, fnJSON)
	case "esc":
		p = receiveEscTricesAndDisplayAnsiColor(portReader, fnJSON)
	//case "wrap", "wrapped":
	//p = receiveWrapSimpleTricesAndDisplayAnsiColor(portReader, fnJSON)

	case "bareXTEACrypted", "wrapXTEACrypted":
		errorFatal(cipher.SetUp())
		fallthrough
	case "ascii":
		fallthrough
	default:
		return fmt.Errorf("unknown encoding: %s", encoding)
	}

	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
	sig := <-sigs // wait for a signal
	if verbose {
		fmt.Println("####################################", sig, "####################################")
	}
	p.Done() <- 0         // end translator
	return p.SavedError() // back to main
}

func receiveBareSimpleTricesAndDisplayAnsiColor(rd io.ReadCloser, fnJSON string) *translator.BareTranslator {
	// NewColorDisplay creates a ColorlDisplay. It provides a Linewriter.
	// It uses internally a local display combined with a line transformer.
	lwD := emitter.NewColorDisplay(emitter.ColorPalette)

	// lineComposer implements the io.StringWriter interface and uses the Linewriter provided.
	// The line composer scans the trice strings and composes lines out of them according to its properies.
	sw := emitter.NewLineComposer(lwD, emitter.TimeStampFormat, emitter.Prefix, emitter.Suffix)

	list := id.NewList(fnJSON)
	list.ReadListFile()
	go list.FileWatcher()

	// triceAtomsReceiver uses the io.Reader interface from s and implements the TriceAtomsReceiver interface.
	// It scans the raw input byte stream and decodes the trice atoms it transmits to the TriceAtomsReceiver interface.
	triceAtomsReceiver := receiver.NewTricesfromBare(rd)

	// uses triceAtomsReceiver for reception and the io.StringWriter interface sw for writing.
	// collects trice atoms to a complete trice, generates the appropriate string using list and writes it to the provided io.StringWriter
	return translator.NewSimpleTrices(sw, list, triceAtomsReceiver)
}

func receiveEscTricesAndDisplayAnsiColor(rd io.ReadCloser, fnJSON string) *translator.EscTranslator {
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
