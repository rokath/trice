// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net

package args

import (
	"io"
	"log"
	"strconv"
	"strings"
	"time"

	"github.com/rokath/trice/cgo/write"
	"github.com/rokath/trice/internalStim/com"
)

// scWrite is an example for sending out trice messages with the trice tool.
func scWrite(w io.Writer) error {
	var e error
	distributeArgs(w)

	p := com.NewPort(w, com.SerialPort, verbose)
	if p.Open() {
		defer p.Close()
	} else {
		log.Fatal("cannot open ", com.SerialPort)
	}

	hexBytesStrings := strings.Fields(hexBytes)
	b := make([]byte, len(hexBytesStrings))
	for i, c := range hexBytesStrings {
		value, err := strconv.ParseInt(c, 16, 16)
		if err != nil {
			log.Fatalf("Conversion failed: %s\n", err)
		} else {
			b[i] = byte(value)
		}
	}
	delay := 5000 * time.Millisecond
	//_, _ = p.Write(b)
	_, _ = p.Write(write.Message0(0))
	time.Sleep(delay)
	_, _ = p.Write(write.Message1(0xa6a6, 1))
	time.Sleep(delay)
	_, _ = p.Write(write.Message2(0xe2e2e2e2, 2))
	time.Sleep(delay)
	_, _ = p.Write(write.Message3(0x9494949494949494, 3))
	time.Sleep(delay)
	return e
}
