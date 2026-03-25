package translator

import (
	"fmt"
	"io"
	"log"
	"os"
	"os/signal"
	"path/filepath"
	"strconv"
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
		log.Fatal(fmt.Sprintln("unknown endianness", TriceEndianness, "- accepting litteEndian or bigEndian."))
	}
	switch strings.ToUpper(Encoding) {
	case "TREX":
		dec = trexDecoder.New(w, lut, m, li, rwc, endian)
	case "CHAR":
		dec = charDecoder.New(w, lut, m, li, rwc, endian)
	case "DUMP":
		dec = dumpDecoder.New(w, lut, m, li, rwc, endian)
	default:
		log.Fatal(fmt.Sprintln("unknown encoding ", Encoding))
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

type targetStampState struct {
	prev16    uint16
	prev32    uint32
	hasPrev16 bool
	hasPrev32 bool
}

func prepareTargetStampFormats() error {
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
	if strings.HasPrefix(decoder.TargetStamp16Delta, "epoch") {
		return fmt.Errorf(`invalid value '%s' for CLI switch -ts16delta. "epoch..." is only supported for -ts32 absolute timestamps`, decoder.TargetStamp16Delta)
	}
	if strings.HasPrefix(decoder.TargetStamp32Delta, "epoch") {
		return fmt.Errorf(`invalid value '%s' for CLI switch -ts32delta. "epoch..." is only supported for -ts32 absolute timestamps`, decoder.TargetStamp32Delta)
	}
	autoTargetStamp0Delta()
	return nil
}

func autoTargetStamp0Delta() {
	if decoder.ShowTargetStamp0DeltaPassed || decoder.TargetStamp0Delta != "" {
		return
	}
	widths := make([]int, 0, 2)
	for _, cfg := range []struct {
		size   int
		format string
	}{
		{2, decoder.TargetStamp16Delta},
		{4, decoder.TargetStamp32Delta},
	} {
		if cfg.format == "" {
			continue
		}
		widths = append(widths, targetStampDisplayWidth(cfg.size, cfg.format))
	}
	if len(widths) == 0 {
		return
	}
	width := widths[0]
	for _, w := range widths[1:] {
		if w > width {
			width = w
		}
	}
	decoder.TargetStamp0Delta = strings.Repeat(" ", width)
}

func targetStampWidth(size int, format string) int {
	return len(formatTargetStamp(size, format, 0))
}

func targetStampDisplayWidth(size int, format string) int {
	s := formatTargetStamp(size, format, 0)
	tag, rest, found := strings.Cut(s, ":")
	if found && tag != "" {
		return len(rest)
	}
	return len(s)
}

func commonTargetDeltaPlaceholder() string {
	if decoder.TargetStamp0Delta != "" {
		return decoder.TargetStamp0Delta
	}
	width := 0
	for _, cfg := range []struct {
		size   int
		format string
	}{
		{2, decoder.TargetStamp16Delta},
		{4, decoder.TargetStamp32Delta},
	} {
		if cfg.format == "" {
			continue
		}
		w := targetStampDisplayWidth(cfg.size, cfg.format)
		if w > width {
			width = w
		}
	}
	if width == 0 {
		return ""
	}
	return strings.Repeat(" ", width)
}

func targetStampFormats(size int) (absoluteFormat, deltaFormat string) {
	switch size {
	case 4:
		return decoder.TargetStamp32, decoder.TargetStamp32Delta
	case 2:
		return decoder.TargetStamp16, decoder.TargetStamp16Delta
	case 0:
		return decoder.TargetStamp0, decoder.TargetStamp0Delta
	default:
		return "", ""
	}
}

func renderTargetStamp(size int, timestamp uint64) string {
	absoluteFormat, _ := targetStampFormats(size)
	return formatTargetStamp(size, absoluteFormat, timestamp)
}

func renderTargetDelta(size int, timestamp uint64, state *targetStampState) string {
	_, deltaFormat := targetStampFormats(size)
	if deltaFormat == "" {
		return commonTargetDeltaPlaceholder()
	}
	return formatTargetDelta(size, deltaFormat, timestamp, state)
}

func formatTargetDelta(size int, format string, timestamp uint64, state *targetStampState) string {
	if format == "" {
		return ""
	}
	switch size {
	case 0:
		return formatTargetStamp(0, format, 0)
	case 2:
		current := uint16(timestamp)
		if !state.hasPrev16 {
			state.prev16 = current
			state.hasPrev16 = true
			return formatMissingTargetDelta(size, format)
		}
		delta := uint16(current - state.prev16)
		state.prev16 = current
		return formatTargetStamp(size, format, uint64(delta))
	case 4:
		current := uint32(timestamp)
		if !state.hasPrev32 {
			state.prev32 = current
			state.hasPrev32 = true
			return formatMissingTargetDelta(size, format)
		}
		delta := uint32(current - state.prev32)
		state.prev32 = current
		return formatTargetStamp(size, format, uint64(delta))
	default:
		return ""
	}
}

func formatTargetDeltaValue(size int, timestampFormat string, timestamp uint64) string {
	switch size {
	case 4:
		switch timestampFormat {
		case "ms", "hh:mm:ss,ms":
			ms := timestamp % 1000
			sec := (timestamp - ms) / 1000 % 60
			min := (timestamp - ms - 1000*sec) / 60000 % 60
			hour := (timestamp - ms - 1000*sec - 60000*min) / 3600000
			return fmt.Sprintf("%2d:%02d:%02d,%03d", hour, min, sec, ms)
		case "us", "Âµs", "ssss,ms_Âµs":
			us := timestamp % 1000
			ms := (timestamp - us) / 1000 % 1000
			sd := (timestamp - 1000*ms) / 1000000
			return fmt.Sprintf("%4d,%03d_%03d", sd, ms, us)
		default:
			return fmt.Sprintf(timestampFormat, timestamp)
		}
	case 2:
		switch timestampFormat {
		case "ms", "s,ms":
			ms := timestamp % 1000
			sec := (timestamp - ms) / 1000
			return fmt.Sprintf("      %2d,%03d", sec, ms)
		case "us", "Âµs", "ms_Âµs":
			us := timestamp % 1000
			ms := (timestamp - us) / 1000 % 1000
			return fmt.Sprintf("      %2d_%03d", ms, us)
		default:
			return fmt.Sprintf(timestampFormat, timestamp)
		}
	case 0:
		if timestampFormat != "" {
			return fmt.Sprint(timestampFormat)
		}
	}
	return ""
}

func formatTargetStamp(size int, format string, timestamp uint64) string {
	switch size {
	case 4:
		return formatTargetStamp32(format, timestamp)
	case 2:
		return formatTargetStamp16(format, timestamp)
	case 0:
		if format != "" {
			return fmt.Sprint(format)
		}
	}
	return ""
}

func formatTargetStamp32(format string, timestamp uint64) string {
	switch format {
	case "ms", "hh:mm:ss,ms":
		ms := timestamp % 1000
		sec := (timestamp - ms) / 1000 % 60
		min := (timestamp - ms - 1000*sec) / 60000 % 60
		hour := (timestamp - ms - 1000*sec - 60000*min) / 3600000
		return fmt.Sprintf("time:%2d:%02d:%02d,%03d", hour, min, sec, ms)
	case "us", "µs", "ssss,ms_µs":
		us := timestamp % 1000
		ms := (timestamp - us) / 1000 % 1000
		sd := (timestamp - 1000*ms) / 1000000
		return fmt.Sprintf("time:%4d,%03d_%03d", sd, ms, us)
	case "epoch":
		t := time.Unix(int64(timestamp), 0).UTC()
		s := t.Format("2006-01-02 15:04:05 UTC")
		c := correctWrappedTimestamp(uint32(timestamp))
		if !t.Equal(c) {
			s += "-->" + c.Format("2006-01-02 15:04:05 UTC")
		}
		return s
	case "":
		return ""
	default:
		after, found := strings.CutPrefix(format, "epoch")
		if found {
			t := time.Unix(int64(timestamp), 0).UTC()
			s := t.Format(after)
			c := correctWrappedTimestamp(uint32(timestamp))
			if !t.Equal(c) {
				s += "-->" + c.Format(after)
			}
			return s
		}
		return fmt.Sprintf(format, timestamp)
	}
}

func formatTargetStamp16(format string, timestamp uint64) string {
	switch format {
	case "ms", "s,ms":
		ms := timestamp % 1000
		sec := (timestamp - ms) / 1000
		return fmt.Sprintf("time:      %2d,%03d", sec, ms)
	case "us", "µs", "ms_µs":
		us := timestamp % 1000
		ms := (timestamp - us) / 1000 % 1000
		return fmt.Sprintf("time:      %2d_%03d", ms, us)
	case "":
		return ""
	default:
		return fmt.Sprintf(format, timestamp)
	}
}

func formatMissingTargetDelta(size int, format string) string {
	prefix, width, leftAlign, suffix, ok := splitSingleFormatDirective(format)
	if !ok {
		if isBuiltinTargetStampFormat(size, format) {
			return strings.Repeat(" ", targetStampDisplayWidth(size, format))
		}
		return formatTargetStamp(size, format, 0)
	}
	marker := "-"
	if width > 0 {
		if leftAlign {
			marker = fmt.Sprintf("%-*s", width, marker)
		} else {
			marker = fmt.Sprintf("%*s", width, marker)
		}
	}
	return prefix + marker + suffix
}

func isBuiltinTargetStampFormat(size int, format string) bool {
	switch size {
	case 4:
		switch format {
		case "ms", "hh:mm:ss,ms", "us", "Âµs", "ssss,ms_Âµs":
			return true
		}
	case 2:
		switch format {
		case "ms", "s,ms", "us", "Âµs", "ms_Âµs":
			return true
		}
	}
	return false
}

func splitSingleFormatDirective(format string) (prefix string, width int, leftAlign bool, suffix string, ok bool) {
	for i := 0; i < len(format); i++ {
		if format[i] != '%' {
			continue
		}
		if i+1 < len(format) && format[i+1] == '%' {
			i++
			continue
		}
		j := i + 1
		for j < len(format) && strings.ContainsRune("+-# 0", rune(format[j])) {
			if format[j] == '-' {
				leftAlign = true
			}
			j++
		}
		widthStart := j
		for j < len(format) && format[j] >= '0' && format[j] <= '9' {
			j++
		}
		if widthStart != j {
			width, _ = strconv.Atoi(format[widthStart:j])
		}
		if j < len(format) && format[j] == '.' {
			j++
			for j < len(format) && format[j] >= '0' && format[j] <= '9' {
				j++
			}
		}
		if j >= len(format) {
			return "", 0, false, "", false
		}
		if !strings.ContainsRune("bdoOxXUeEfFgGcqpvTt", rune(format[j])) {
			return "", 0, false, "", false
		}
		return format[:i], width, leftAlign, format[j+1:], true
	}
	return "", 0, false, "", false
}

// decodeAndComposeLoop does not return.
func decodeAndComposeLoop(w io.Writer, sw *emitter.TriceLineComposer, dec decoder.Decoder, lut id.TriceIDLookUp, li id.TriceIDLookUpLI) error {
	b := make([]byte, decoder.DefaultSize) // intermediate trice string buffer
	bufferReadStartTime := time.Now()
	sleepCounter := 0
	if err := prepareTargetStampFormats(); err != nil {
		return err
	}
	state := targetStampState{}
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

		// b contains here a single trice message with printed values or a single error message.
		start := time.Now()

		// Filtering is done here to suppress the loc, timestamp and id display as well for the filtered items.
		n = emitter.BanOrPickFilter(b[:n])

		if n > 0 { // s.th. to write out
			var logLineStart bool // logLineStart is a helper flag for log line start detection
			if len(sw.Line) == 0 {
				logLineStart = true
			}

			if logLineStart && id.LIFnJSON != "off" && id.LIFnJSON != "none" {
				s := locationInformation(decoder.LastTriceID, li)
				_, err := sw.Write([]byte(s)) // todo: sw.WriteString(s) ?
				msg.OnErr(err)
			}

			var s string
			if logLineStart {
				s = renderTargetStamp(decoder.TargetTimestampSize, decoder.TargetTimestamp)
				if s != "" {
					_, err := sw.Write([]byte(s))
					msg.OnErr(err)
					_, err = sw.Write([]byte("default: "))
					msg.OnErr(err)
				}
				s = renderTargetDelta(decoder.TargetTimestampSize, decoder.TargetTimestamp, &state)
				if s != "" {
					_, err := sw.Write([]byte(s))
					msg.OnErr(err)
					_, err = sw.Write([]byte("default: "))
					msg.OnErr(err)
				}
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
		if duration > 100000 {
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
