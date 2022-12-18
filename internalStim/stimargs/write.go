// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net

package stimargs

import (
	"io"
	"log"
	"strconv"
	"strings"
	"time"

	"github.com/rokath/trice/cgo/write"
	"github.com/rokath/trice/internalStim/stimcom"
)

// scWrite is an example for sending out trice messages with the trice tool.
func scWrite(w io.Writer) error {
	var e error
	distributeArgs(w)

	p := stimcom.NewPort(w, stimcom.SerialPort, verbose)
	if p.Open() {
		defer p.Close()
	} else {
		log.Fatal("cannot open ", stimcom.SerialPort)
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
	return e
}
