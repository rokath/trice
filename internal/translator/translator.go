package translator

import (
	"fmt"
	"io"
	"log"
	"os"
	"os/signal"
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
	"github.com/rokath/trice/internal/tleDecoder"
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
	case "TLE", "COBS":
		dec = tleDecoder.New(w, lut, m, li, rwc, endian)
		//cobsVariantDecode = cobs.Decode
		//  case "COBSFF":
		//  	dec = newCOBSDecoder(w, lut, m, rc, endian)
		//  	cobsVariantDecode = cobsFFDecode
	case "TREX":
		dec = trexDecoder.New(w, lut, m, li, rwc, endian)
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
			if Verbose {
				fmt.Fprintln(w, "####################################", sig, "####################################")
			}
			emitter.PrintColorChannelEvents(w)
			msg.FatalOnErr(rc.Close())
			os.Exit(0) // end
		case <-ticker.C:
		}
	}
}

// decodeAndComposeLoop does not return.
func decodeAndComposeLoop(w io.Writer, sw *emitter.TriceLineComposer, dec decoder.Decoder, lut id.TriceIDLookUp, li id.TriceIDLookUpLI) error {
	b := make([]byte, decoder.DefaultSize) // intermediate trice string buffer
	bufferReadStartTime := time.Now()
	sleepCounter := 0
	if decoder.TargetStamp == "" || decoder.TargetStamp == "off" || decoder.TargetStamp == "none" {
		if !decoder.ShowTargetStamp0Passed {
			decoder.TargetStamp0 = ""
		}
		if !decoder.ShowTargetStamp16Passed {
			decoder.TargetStamp16 = ""
		}
		if !decoder.ShowTargetStamp32Passed {
			decoder.TargetStamp32 = ""
		}
	}
	if decoder.TargetStamp == "ms" {
		if !decoder.ShowTargetStamp0Passed {
			decoder.TargetStamp0 = "             "
		}
		if !decoder.ShowTargetStamp16Passed {
			decoder.TargetStamp16 = "ms"
		}
		if !decoder.ShowTargetStamp32Passed {
			decoder.TargetStamp32 = "ms"
		}
	}
	if decoder.TargetStamp == "us" || decoder.TargetStamp == "µs" {
		if !decoder.ShowTargetStamp0Passed {
			decoder.TargetStamp0 = "             "
		}
		if !decoder.ShowTargetStamp16Passed {
			decoder.TargetStamp16 = "us"
		}
		if !decoder.ShowTargetStamp32Passed {
			decoder.TargetStamp32 = "us"
		}
	}
	for {
		n, err := dec.Read(b) // Code to measure, dec.Read can return n=0 in some cases and then wait.

		if err != io.EOF && err != nil {
			log.Fatal(err)
		}

		if n == 0 {
			if (receiver.Port == "FILEBUFFER" || receiver.Port == "BUFFER") /*&& err == io.EOF*/ && time.Since(bufferReadStartTime) > 100*time.Millisecond { // do not wait if a predefined buffer
				if len(sw.Line) > 0 {
					_, _ = sw.Write([]byte(`\n`)) // add newline as line end to display any started line
				}
				msg.OnErr(err)
				return io.EOF
			}
			//  if Verbose {
			//  	fmt.Fprintln(w, err, "-> WAITING...")
			//  }
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

		// Filtering is done here to suppress the loc, timestamp and id display as well for the filtered items.
		n = emitter.BanOrPickFilter(b[:n]) // todo: b can contain several trices - handle that!

		if n > 0 { // s.th. to write out
			var logLineStart bool // logLineStart is a helper flag for log line start detection
			if len(sw.Line) == 0 {
				logLineStart = true
			}

			if logLineStart && id.LIFnJSON != "off" && id.LIFnJSON != "none" {
				s := locationInformation(decoder.LastTriceID, li)
				_, err := sw.Write([]byte(s))
				msg.OnErr(err)
			}

			/////////////
			// obsolete
			//
			// // If target location & enabled and line start, write target location.
			// if logLineStart && decoder.TargetLocationExists && decoder.LocationInformationFormatString != "off" && decoder.LocationInformationFormatString != "none" {
			// 	targetFileID := id.TriceID(decoder.TargetLocation >> 16)
			// 	t := lut[targetFileID]
			// 	targetFile := t.Strg
			// 	s := fmt.Sprintf(decoder.LocationInformationFormatString, targetFile, 0xffff&decoder.TargetLocation)
			// 	_, err := sw.Write([]byte(s))
			// 	msg.OnErr(err)
			// }

			var s string
			if logLineStart {
				switch decoder.TargetTimestampSize {
				case 4:
					switch decoder.TargetStamp32 {
					case "ms", "hh:mm:ss,ms":
						ms := decoder.TargetTimestamp % 1000
						sec := (decoder.TargetTimestamp - ms) / 1000 % 60
						min := (decoder.TargetTimestamp - ms - 1000*sec) / 60000 % 60
						hour := (decoder.TargetTimestamp - ms - 1000*sec - 60000*min) / 3600000
						s = fmt.Sprintf("tim:%2d:%02d:%02d,%03d ", hour, min, sec, ms)
					case "us", "µs", "ssss,ms_µs":
						us := decoder.TargetTimestamp % 1000
						ms := (decoder.TargetTimestamp - us) / 1000 % 1000
						sd := (decoder.TargetTimestamp - 1000*ms) / 1000000
						s = fmt.Sprintf("tim:%4d,%03d_%03d ", sd, ms, us)
					case "":
					default:
						s = fmt.Sprintf(decoder.TargetStamp32, decoder.TargetTimestamp)
					}

				case 2:
					switch decoder.TargetStamp16 {
					case "ms", "s,ms":
						ms := decoder.TargetTimestamp % 1000
						sec := (decoder.TargetTimestamp - ms) / 1000
						s = fmt.Sprintf("tim:      %2d,%03d ", sec, ms)
					case "us", "µs", "ms_µs":
						us := decoder.TargetTimestamp % 1000
						ms := (decoder.TargetTimestamp - us) / 1000 % 1000
						s = fmt.Sprintf("tim:      %2d_%03d ", ms, us)
					case "":
					default:
						s = fmt.Sprintf(decoder.TargetStamp16, decoder.TargetTimestamp)
					}

				case 0:
					if decoder.TargetStamp0 != "" {
						s = fmt.Sprintf(decoder.TargetStamp0)
					}
				}
				_, err := sw.Write([]byte(s))
				msg.OnErr(err)
			}
			// write ID only if enabled and line start.
			if logLineStart && decoder.ShowID != "" {
				s := fmt.Sprintf(decoder.ShowID, decoder.LastTriceID)
				_, err := sw.Write([]byte(s))
				msg.OnErr(err)
			}
			_, err := sw.Write(b[:n])
			msg.OnErr(err)
		}

		duration := time.Since(start).Milliseconds()
		if duration > 100 {
			fmt.Fprintln(w, "TriceLineComposer.Write duration =", duration, "ms.")
		}
		//msg.InfoOnErr(err, fmt.Sprintln("sw.Write wrote", m, "bytes"))
	}
}

// locationInformation returns optional location information for id.
func locationInformation(tid id.TriceID, li id.TriceIDLookUpLI) string {
	if li != nil && decoder.LocationInformationFormatString != "off" && decoder.LocationInformationFormatString != "none" {
		if li, ok := li[tid]; ok {
			return fmt.Sprintf(decoder.LocationInformationFormatString, li.File, li.Line)
		} else {
			return fmt.Sprintf(decoder.LocationInformationFormatString, "", 0)
		}
	} else {
		if Verbose {
			return fmt.Sprintf("wrn:no li ")
		}
	}
	return ""
}
