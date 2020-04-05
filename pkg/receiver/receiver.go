// Copyright 2020 basti@blackoutcloud.de
//                Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package receiver is responsible for getting and handling the bytes from the communication port.
// Currently only serial port is supported.
// It is activated by the trice package and call emit.Trice() on received bytes chunks.
package receiver

import (
	"errors"
	"fmt"
	"log"

	"github.com/rokath/trice/pkg/emit"
	"github.com/rokath/trice/pkg/id"
)

/*
type TriceReceiver interface {
	SetUp() bool
	CleanUp()
	Start()
	Stop()
	GetTriceChannel() *chan []byte
	GetBufferChannel() *chan []byte
}
*/

var (
	// Port is the COMport name like COM1
	Port string

	// Baud is the configured baudrate of the serial port
	Baud int
)

type receiver struct {
	name          string
	receivingData bool
	triceChannel  chan []byte
	bufferChannel chan []byte
}

func (p *receiver) GetTriceChannel() *chan []byte {
	return &p.triceChannel
}

func (p *receiver) GetBufferChannel() *chan []byte {
	return &p.bufferChannel
}

// DoSerial is the endless loop for trice logging
func DoSerial() error {
	err := conditionalComPortScan()
	if err != nil {
		return err
	}
	serialReceiver := NewSerialReceiver(Port, Baud)

	if serialReceiver.SetUp() == false {
		fmt.Println("Could not set up serial port", Port)
		fmt.Println("try -port COMscan")
		return nil
	}
	fmt.Println("Opened serial port", Port)

	serialReceiver.Start()
	defer serialReceiver.CleanUp()

	//keyboardInput()

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
			b, err = emit.Trice(t, b, id.List)
			if nil != err {
				log.Println("trice.Log error", err, t, b)
			}
		}
	}
}

// conditionalComPortScan scans for COM ports if -port was specified as COMscan, it tries to use first found COM port.
func conditionalComPortScan() error {
	if "COMscan" != Port {
		return nil
	}
	log.Println("Scan for serial ports...")
	ports, err := GetSerialPorts()
	if err != nil {
		return err
	}
	if len(ports) > 0 {
		log.Println("Take serial port", ports[0])
		Port = ports[0]
		return nil
	}
	return errors.New("Could not find serial port on system")
}
