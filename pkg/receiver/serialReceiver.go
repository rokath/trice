// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver

import (
	"errors"
	"fmt"
	"log"

	"go.bug.st/serial"
)

var (
	locAddr = byte(0x60) // local trice address
	remAddr = byte(0x60) // remote trice address

	// Port is the COMport name like COM1
	Port string

	// Baud is the configured baudrate of the serial port
	Baud int
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

// CleanUp makes clean
//
// It stops reception and closes port (handle release)
func (p *serialReceiver) CleanUp() {
	p.Stop()
	p.serialHandle.Close()
}

// ClosePort releases port
func (p *serialReceiver) ClosePort() {
	p.serialHandle.Close()
}

// Stores data received from the serial port into the provided byte array
// buffer. The function returns the number of bytes read.
//
// The Read function blocks until (at least) one byte is received from
// the serial port or an error occurs.
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

// DoSerial is the endless loop for trice logging over serial port
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
BACKUP: unsused functions

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
