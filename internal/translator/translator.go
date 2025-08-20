package translator

import (
	"fmt"
	"io"
	"log"
	"os"
	"os/signal"
	"path/filepath"
	"strings"
	"sync"
	"syscall"
	"time"

	"github.com/rokath/trice/internal/charDecoder"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/dumpDecoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/keybcmd"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/trexDecoder"
	"github.com/rokath/trice/pkg/msg"
)

var (
	// Encoding describes the way the byte stream is coded.
	Encoding string

	// TriceEndianness if bigEndian assumes a big endian encoded trice stream from the target.
	// To keep target load small, the encoded trice stream from the target matches the target endian, what us usually littleEndian.
	TriceEndianness string

	Verbose bool
)

// Translate performs the trice log task.
//
// Bytes are read with rc. Then according decoder.Encoding they are translated into strings.
// Each read returns the amount of bytes for one trice. rc is called on every
// Translate returns true on io.EOF or false on hard read error or sigterm.
func Translate(w io.Writer, sw *emitter.TriceLineComposer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, rwc io.ReadWriteCloser) error {
	//var dec Decoder //io.Reader
	if Verbose {
		fmt.Fprintln(w, "Encoding is", Encoding)
	}
	var endian bool
	var dec decoder.Decoder
	switch TriceEndianness {
	case "littleEndian":
		endian = decoder.LittleEndian
	case "bigEndian":
		endian = decoder.BigEndian
	default:
		log.Fatalf(fmt.Sprintln("unknown endianness ", TriceEndianness, "-accepting litteEndian or bigEndian."))
	}
	switch strings.ToUpper(Encoding) {
	case "TREX":
		dec = trexDecoder.New(w, sw, lut, m, li, rwc, endian)
	case "CHAR":
		dec = charDecoder.New(w, lut, m, li, rwc, endian)
	case "DUMP":
		dec = dumpDecoder.New(w, lut, m, li, rwc, endian)
	default:
		log.Fatalf(fmt.Sprintln("unknown encoding ", Encoding))
	}
	if emitter.DisplayRemote {
		keybcmd.ReadInput(rwc)
	} else {
		go handleSIGTERM(w, rwc)
	}
	dec.sw =sw
	return decodeAndComposeLoop(w, sw, dec, lut, li)
}

// handleSIGTERM is called on CTRL-C shutdown.
func handleSIGTERM(w io.Writer, rc io.ReadCloser) {
	// prepare CTRL-C shutdown reaction
	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
	ticker := time.NewTicker(50 * time.Millisecond)
	for {
		select {
		case sig := <-sigs: // wait for a signal
			time.Sleep(250 * time.Millisecond)
			if Verbose {
				fmt.Fprintln(w, "####################################", sig, "####################################")
			} else {
				fmt.Fprintln(w)
			}
			emitter.PrintTagStatistics(w)
			decoder.PrintTriceStatistics(w)
			msg.FatalOnErr(rc.Close())
			os.Exit(0) // end
		case <-ticker.C:
		}
	}
}

const DefaultTargetStamp0 = "time:            "

// decodeAndComposeLoop does not return.
func decodeAndComposeLoop(w io.Writer, sw *emitter.TriceLineComposer, dec decoder.Decoder, lut id.TriceIDLookUp, li id.TriceIDLookUpLI) error {
	b := make([]byte, decoder.DefaultSize) // intermediate trice string buffer
	bufferReadStartTime := time.Now()
	sleepCounter := 0
	switch decoder.TargetStamp {
	case "", "off", "none":
		if !decoder.ShowTargetStamp0Passed {
			decoder.TargetStamp0 = ""
		}
		if !decoder.ShowTargetStamp16Passed {
			decoder.TargetStamp16 = ""
		}
		if !decoder.ShowTargetStamp32Passed {
			decoder.TargetStamp32 = ""
		}
	case "ms":
		if !decoder.ShowTargetStamp0Passed {
			decoder.TargetStamp0 = DefaultTargetStamp0
		}
		if !decoder.ShowTargetStamp16Passed {
			decoder.TargetStamp16 = "ms"
		}
		if !decoder.ShowTargetStamp32Passed {
			decoder.TargetStamp32 = "ms"
		}
	case "us", "µs":
		if !decoder.ShowTargetStamp0Passed {
			decoder.TargetStamp0 = DefaultTargetStamp0
		}
		if !decoder.ShowTargetStamp16Passed {
			decoder.TargetStamp16 = "us"
		}
		if !decoder.ShowTargetStamp32Passed {
			decoder.TargetStamp32 = "us"
		}
	default:
		return fmt.Errorf("invalid value '%s' for CLI switch -ts. Please run 'trice help -log' and check the -ts options", decoder.TargetStamp)
	}
	for {
		n, err := dec.Read(b) // Code to measure, dec.Read can return n=0 in some cases and then wait.

		if err != io.EOF && err != nil {
			log.Fatal(err)
		}
		if n == 0 {
			if (receiver.Port == "FILEBUFFER" ||
				receiver.Port == "TCP4BUFFER" ||
				receiver.Port == "HEX" ||
				receiver.Port == "DUMP" ||
				receiver.Port == "DEC" ||
				receiver.Port == "BUFFER") /*&& err == io.EOF*/ && time.Since(bufferReadStartTime) > 100*time.Millisecond { // do not wait if a predefined buffer
				if len(sw.Line) > 0 {
					_, _ = sw.Write([]byte(`\n`)) // add newline as line end to display any started line
				}
				msg.OnErr(err)
				return io.EOF
			}

			sleepCounter++
			if sleepCounter > 100 {
				time.Sleep(100 * time.Millisecond)
				sleepCounter = 0
			}
			continue // read again
		}

		// b contains here none or several complete trice strings.
		// If several, they end with a newline each, despite the last one which optionally ends with a newline.
		start := time.Now()

		if n > 0 { // s.th. to write out
			_, err := sw.Write(b[:n])
			msg.OnErr(err)
		}

		duration := time.Since(start).Milliseconds()
		if duration > 1000 {
			fmt.Fprintln(w, "TriceLineComposer.Write duration =", duration, "ms.")
		}
	}
}

// CorrectWrappedTimestamp checks whether a 32-bit timestamp falls outside the valid range
// and virtually sets a 33rd bit by adding 2^32 seconds to it
func CorrectWrappedTimestamp(ts32 uint32) time.Time {

	const (
		minValidYear = 2000
		maxValidYear = 2038
		wrapOffset   = 1 << 32 // 2^32 seconds
	)

	// Interpret the timestamp as time.Time
	t := time.Unix(int64(ts32), 0).UTC()

	if t.Year() >= minValidYear && t.Year() <= maxValidYear {
		return t
	}

	// Apply wraparound correction by adding 2^32 seconds
	tWrapped := time.Unix(int64(ts32)+wrapOffset, 0).UTC()

	// If the corrected timestamp is plausible, return it
	if tWrapped.Year() > maxValidYear && tWrapped.Year() <= maxValidYear+100 {
		return tWrapped
	}

	// Fallback: return the original timestamp and print a warning
	fmt.Printf("WARNING: Timestamp %v (%d) is outside the expected year range\n", t, ts32)
	return t
}

// LocationInformation returns optional location information for id.
func LocationInformation(tid id.TriceID, li id.TriceIDLookUpLI) string {
	if li != nil && decoder.LocationInformationFormatString != "off" && decoder.LocationInformationFormatString != "none" {
		if li, ok := li[tid]; ok {
			return fmt.Sprintf(decoder.LocationInformationFormatString, filepath.Base(li.File), li.Line)
		} else {
			return fmt.Sprintf(decoder.LocationInformationFormatString, "", 0)
		}
	} else {
		if Verbose {
			return "no li"
		}
	}
	return ""
}
