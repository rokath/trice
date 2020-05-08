// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver

import (
	"encoding/binary"
	"errors"
	"fmt"
	"log"

	"go.bug.st/serial"
)

var (
	locAddr = byte(0x60) // local trice address
	remAddr = byte(0x60) // remote trice address
)

// serialReceiver is a serial device trice receiver
type serialReceiver struct {
	*triceReceiver // compose

	portName    string
	readTimeout int

	// serial port configuration
	serialHandle serial.Port
	serialMode   serial.Mode
}

// newTriceReceiver creates an instance of the common trice receiver part for a new receiver device
func newTriceReceiver(r triceReceiverInterface) *triceReceiver {
	return &triceReceiver{
		triceReceiverInterface: r,
		name:                   "trice receiver",
		receivingData:          false,
		triceChannel:           make(chan []byte),
		bufferChannel:          make(chan []byte),
	}
}

// newSerialReceiver creates an instance of a serial device type trice receiver
func newSerialReceiver(portIdentifier string, baudrate int) *serialReceiver {
	r := &serialReceiver{
		//triceReceiver: //newTriceReceiver(r),
		portName:    portIdentifier,
		readTimeout: 1,
		serialMode: serial.Mode{
			BaudRate: baudrate,
			DataBits: 8,
			Parity:   serial.NoParity,
			StopBits: serial.OneStopBit},
	}
	r.triceReceiver = newTriceReceiver(r)
	return r
}

// SetUp opens a serial port
func (p *serialReceiver) SetUp() bool {
	var err error

	p.serialHandle, err = serial.Open(p.portName, &p.serialMode)

	if err != nil {
		fmt.Println("Error: Could not open serial port:", err)
		return false
	}
	return true
}

// Start starts receiving of serial data
func (p *serialReceiver) Start() {
	p.receivingData = true
	go p.receiving()
}

// Stop stops receiving of serial data
func (p *serialReceiver) Stop() {
	p.receivingData = false
}

// CleanUp makes clean
func (p *serialReceiver) CleanUp() {
	p.Stop()
	p.serialHandle.Close()
}

// receiving: ReadEndless expects a pointer to a filled COM port configuration
func (p *serialReceiver) receiving() {
	for p.receivingData == true {
		b, err := p.readHeader()

		if nil != err {
			fmt.Println("Could not read serial header: ", err)
			continue
		}

		if 0xeb == b[0] { // traceLog startbyte, no further data
			//fmt.Println("to trice channel:", b)
			p.triceChannel <- b // send to process trace log channel

		} else if 0xc0 == b[0] {
			switch b[6] & 0xc0 {
			case 0xc0:
				fmt.Println("reCal command expecting an answer", b)
			case 0x80:
				fmt.Println("reCal message (not expecting an answer)", b)
			case 0x40:
				fmt.Println("answer to a reCal command")
			case 0x00:
				if (0xff != b[4]) || (0xff != b[5]) || (1 != b[7]) {
					fmt.Println("ERR:wrong format")
				} else {
					// int(b[6]) contains string identificator for verification
					d, _ := p.readAtLeastBytes(2) // len of buffer (only one buffer)
					if nil != err {
						fmt.Println("Could not read serial len: ", err)
						continue
					}
					len := int(binary.LittleEndian.Uint16(d[:2]))
					s, _ := p.readAtLeastBytes(len + 1) // len is len-1 value
					if nil != err {
						fmt.Println("Could not read buffer: ", err)
						continue
					}
					b = append(b, d...) // len is redundant here and usable as check
					b = append(b, s...) // the buffer (string) data
					//log.Println("to buffer channel:", b)
					p.bufferChannel <- b // send to process trace log channel
				}
			}
		} else {
			fmt.Println("Got unknown header on serial console. Discarding...", b)
		}
	}
}

// ClosePort releases port
func (p *serialReceiver) ClosePort() {
	p.serialHandle.Close()
}

func (p *serialReceiver) Read(buf []byte) (int, error) {
	return p.serialHandle.Read(buf)
}

// GetSerialPorts scans for serial ports
func GetSerialPorts() ([]string, error) {
	ports, err := serial.GetPortsList()

	if err != nil {
		log.Println(err)
		return ports, err
	}
	if len(ports) == 0 {
		log.Println("No serial ports found!")
		return ports, err
	}
	for _, port := range ports {
		log.Println("Found port: ", port)
	}

	return ports, err
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

// DoSerial is the endless loop for trice logging
func DoSerial() {
	err := conditionalComPortScan()
	if err != nil {
		return
	}
	sR := newSerialReceiver(Port, Baud)

	if sR.SetUp() == false {
		fmt.Println("Could not set up serial port", Port)
		fmt.Println("try -port COMscan")
		return
	}
	fmt.Println("Opened serial port", Port)

	sR.Start()
	defer sR.CleanUp()

	sR.doReceive()
}

/*
BACKUP

// SetReadTimeOut sets timeout
func (p *serialReceiver) SetReadTimeOut(timeout int) {
	p.readTimeout = timeout
}

// SetParity sets transmit parity
func (p *serialReceiver) SetParity(parity serial.Parity) {
	p.serialMode.Parity = parity
}

// SetDataBits sets bit count
func (p *serialReceiver) SetDataBits(databits int) {
	p.serialMode.DataBits = databits
}

// SetBaudrate sets speed
func (p *serialReceiver) SetBaudrate(baudrate int) {
	p.serialMode.BaudRate = baudrate
}

// SetStopBits sets stop condition
func (p *serialReceiver) SetStopBits(stopbits serial.StopBits) {
	p.serialMode.StopBits = stopbits
}

*/
