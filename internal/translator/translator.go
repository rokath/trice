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
						s = fmt.Sprintf("time:%2d:%02d:%02d,%03d", hour, min, sec, ms)
					case "us", "µs", "ssss,ms_µs":
						us := decoder.TargetTimestamp % 1000
						ms := (decoder.TargetTimestamp - us) / 1000 % 1000
						sd := (decoder.TargetTimestamp - 1000*ms) / 1000000
						s = fmt.Sprintf("time:%4d,%03d_%03d", sd, ms, us)
					case "epoch":
						t := time.Unix(int64(decoder.TargetTimestamp), 0).UTC()
						s = t.Format("2006-01-02 15:04:05 UTC")
						c := correctWrappedTimestamp(uint32(decoder.TargetTimestamp))
						if !t.Equal(c) {
							s += "-->" + c.Format("2006-01-02 15:04:05 UTC")
						}
					case "":
						// Suppressing ts32 output is desired.
					default:
						after, found := strings.CutPrefix(decoder.TargetStamp32, "epoch")
						if found { // Assume a -ts32="epoch2006-01-02 15:04:05 UTC" like value.
							t := time.Unix(int64(decoder.TargetTimestamp), 0).UTC()
							s = t.Format(after) // examples for after:
							// s = t.Format("Mon Jan _2 15:04:05 2006")            //ANSIC
							// s = t.Format("Mon Jan _2 15:04:05 MST 2006")        //UnixDate
							// s = t.Format("Mon Jan 02 15:04:05 -0700 2006")      //RubyDate
							// s = t.Format("02 Jan 06 15:04 MST")                 //RFC822
							// s = t.Format("02 Jan 06 15:04 -0700")               //RFC822Z     (RFC822 with numeric zone)
							// s = t.Format("Monday, 02-Jan-06 15:04:05 MST")      //RFC850
							// s = t.Format("Mon, 02 Jan 2006 15:04:05 MST")       //RFC1123
							// s = t.Format("Mon, 02 Jan 2006 15:04:05 -0700")     //RFC1123Z    (RFC1123 with numeric zone)
							// s = t.Format("2006-01-02T15:04:05Z07:00")           //RFC3339
							// s = t.Format("2006-01-02T15:04:05.999999999Z07:00") //RFC3339Nano
							// s = t.Format("3:04PM")                              //Kitchen
							// Assumed usage example: trice log -ts32='epoch"Mon, 02 Jan 2006 15:04:05 MST"'
							c := correctWrappedTimestamp(uint32(decoder.TargetTimestamp))
							if !t.Equal(c) {
								s += "-->" + c.Format(after)
							}

						} else { // Assume a string containing a single %d like format specification.
							s = fmt.Sprintf(decoder.TargetStamp32, decoder.TargetTimestamp)
						}
					}
				case 2:
					switch decoder.TargetStamp16 {
					case "ms", "s,ms":
						ms := decoder.TargetTimestamp % 1000
						sec := (decoder.TargetTimestamp - ms) / 1000
						s = fmt.Sprintf("time:      %2d,%03d", sec, ms)
					case "us", "µs", "ms_µs":
						us := decoder.TargetTimestamp % 1000
						ms := (decoder.TargetTimestamp - us) / 1000 % 1000
						s = fmt.Sprintf("time:      %2d_%03d", ms, us)
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
				_, err = sw.Write([]byte("default: "))
				msg.OnErr(err)
			}
			// write ID only if enabled and line start.
			if logLineStart && decoder.ShowID != "" {
				s := fmt.Sprintf(decoder.ShowID, decoder.LastTriceID)
				_, err := sw.Write([]byte(s))
				msg.OnErr(err)
				_, err = sw.Write([]byte("default: ")) // add space as separator
				msg.OnErr(err)
			}
			_, err := sw.Write(b[:n])
			msg.OnErr(err)
		}

		duration := time.Since(start).Milliseconds()
		if duration > 1000 {
			fmt.Fprintln(w, "TriceLineComposer.Write duration =", duration, "ms.")
		}
	}
}

// correctWrappedTimestamp checks whether a 32-bit timestamp falls outside the valid range
// and virtually sets a 33rd bit by adding 2^32 seconds to it
func correctWrappedTimestamp(ts32 uint32) time.Time {

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

// locationInformation returns optional location information for id.
func locationInformation(tid id.TriceID, li id.TriceIDLookUpLI) string {
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
