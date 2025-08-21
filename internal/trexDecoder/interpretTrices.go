package trexDecoder

import (
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"math"
	"strings"
	"time"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/msg"
)

// interpretTrices prints p.trices.
func (p *trexDec) interpretTrices(b []byte) (n int, err error) {
	for i := range p.tcount {
		p.t = &p.trices[i]
		var m int
		m, err = p.interpretTrice(b[n:])
		n += m
		if err != nil {
			return n, err
		}
	}
	return
}

// interpretTrice emits Trice p.t into b and returns with n the written byte count.
func (p *trexDec) interpretTrice(b []byte) (n int, err error) {
	t := p.t
	n = p.checkReceivedCycle(b)
	//size := t.headerSize + t.paramSize

	var ok bool
	t.Trice, ok = p.Lut[t.id] // t.Trice is field Trice id.TriceFmt <-- That works!

	if !ok {
		n += copy(b[n:], fmt.Sprintln("WARNING:\aunknown ID ", t.id, "- ignoring trice:"))
		n += copy(b[n:], fmt.Sprintln(hex.Dump(t.v)))
		n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		return
	}
	// pick or ban filter here
	// Filtering is done here to suppress the loc, timestamp and id display as well for the filtered items.
	//n = emitter.BanOrPickFilter(b[:n]) // todo: b can contain several trices - handle that!

	if AddNewlineToEachTriceMessage {
		t.Trice.Strg += `\n` // this adds a newline to each single Trice message
	}

	var logLineStart bool // logLineStart is a helper flag for log line start detection
	if len(p.Sw.Line) == 0 {
		logLineStart = true
	}

	if logLineStart && id.LIFnJSON != "off" && id.LIFnJSON != "none" {
		s := decoder.LocationInformation(decoder.LastTriceID, p.Li)
		_, err := p.Sw.Write([]byte(s))
		msg.OnErr(err)
		// TODO: Check is this is a good idea
		//  if len(s) > 0 { // Add a color-less space after the target stamp only if a stamp was written.
		//  	_, err = p.Sw.Write([]byte("default: "))
		//  	msg.OnErr(err)
		//  }

	}

	var s string
	if logLineStart {
		switch p.t.stampSize {
		case 4:
			switch decoder.TargetStamp32 {
			case "ms", "hh:mm:ss,ms":
				ms := p.t.stamp % 1000
				sec := (p.t.stamp - ms) / 1000 % 60
				min := (p.t.stamp - ms - 1000*sec) / 60000 % 60
				hour := (p.t.stamp - ms - 1000*sec - 60000*min) / 3600000
				s = fmt.Sprintf("time:%2d:%02d:%02d,%03d", hour, min, sec, ms)
			case "us", "µs", "ssss,ms_µs":
				us := p.t.stamp % 1000
				ms := (p.t.stamp - us) / 1000 % 1000
				sd := (p.t.stamp - 1000*ms) / 1000000
				s = fmt.Sprintf("time:%4d,%03d_%03d", sd, ms, us)
			case "epoch":
				t := time.Unix(int64(p.t.stamp), 0).UTC()
				s = t.Format("2006-01-02 15:04:05 UTC")
				c := decoder.CorrectWrappedTimestamp(uint32(p.t.stamp))
				if !t.Equal(c) {
					s += "-->" + c.Format("2006-01-02 15:04:05 UTC")
				}
			case "":
				// Suppressing ts32 output is desired.
			default:
				after, found := strings.CutPrefix(decoder.TargetStamp32, "epoch")
				if found { // Assume a -ts32="epoch2006-01-02 15:04:05 UTC" like value.
					t := time.Unix(int64(p.t.stamp), 0).UTC()
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
					c := decoder.CorrectWrappedTimestamp(uint32(p.t.stamp))
					if !t.Equal(c) {
						s += "-->" + c.Format(after)
					}

				} else { // Assume a string containing a single %d like format specification.
					s = fmt.Sprintf(decoder.TargetStamp32, p.t.stamp)
				}
			}
		case 2:
			switch decoder.TargetStamp16 {
			case "ms", "s,ms":
				ms := p.t.stamp % 1000
				sec := (p.t.stamp - ms) / 1000
				s = fmt.Sprintf("time:      %2d,%03d", sec, ms)
			case "us", "µs", "ms_µs":
				us := p.t.stamp % 1000
				ms := (p.t.stamp - us) / 1000 % 1000
				s = fmt.Sprintf("time:      %2d_%03d", ms, us)
			case "":
			default:
				s = fmt.Sprintf(decoder.TargetStamp16, p.t.stamp)
			}
		case 0:
			if decoder.TargetStamp0 != "" {
				s = fmt.Sprintf(decoder.TargetStamp0)
			}
		}
		_, err := p.Sw.Write([]byte(s))
		msg.OnErr(err)
		if len(s) > 0 { // Add a color-less space after the target stamp only if a stamp was written.
			_, err = p.Sw.Write([]byte("default: "))
			msg.OnErr(err)
		}
	}
	// write ID only if enabled and line start.
	if logLineStart && decoder.ShowID != "" {
		s := fmt.Sprintf(decoder.ShowID, decoder.LastTriceID)
		_, err := p.Sw.Write([]byte(s))
		msg.OnErr(err)
		_, err = p.Sw.Write([]byte("default: ")) // add space as separator
		msg.OnErr(err)
	}
	n += p.sprintTrice(b[n:]) // use param info
	return
}

// checkReceivedCycle uses p.t, evaluates p.receivedCycle and updates p.receivedCycle.
// On error it write a message into and returns in n the message length.
// It uses and updates decoder.InitialCycle.
func (p *trexDec) checkReceivedCycle(b []byte) (n int) {
	t := p.t
	// cycle counter automatic & check
	if t.receivedCycle == 0xc0 && t.expectedCycle != 0xc0 && decoder.InitialCycle { // with cycle counter and seems to be a target reset
		n += copy(b[n:], fmt.Sprintln("warning:\a   Target Reset?   "))
		t.expectedCycle = t.receivedCycle + 1 // adjust cycle
		decoder.InitialCycle = false
	}
	if t.receivedCycle == 0xc0 && t.expectedCycle != 0xc0 && !decoder.InitialCycle { // with cycle counter and seems to be a target reset
		//n += copy(b[n:], fmt.Sprintln("info:   Target Reset?   ")) // todo: This line is ok with cycle counter but not without cycle counter
		t.expectedCycle = t.receivedCycle + 1 // adjust cycle
	}
	if t.receivedCycle == 0xc0 && t.expectedCycle == 0xc0 && decoder.InitialCycle { // with or without cycle counter and seems to be a target reset
		//n += copy(b[n:], fmt.Sprintln("warning:   Restart?   "))
		t.expectedCycle = t.receivedCycle + 1 // adjust cycle
		decoder.InitialCycle = false
	}
	if t.receivedCycle == 0xc0 && t.expectedCycle == 0xc0 && !decoder.InitialCycle { // with or without cycle counter and seems to be a normal case
		t.expectedCycle = t.receivedCycle + 1 // adjust cycle
	}
	if t.receivedCycle != 0xc0 && !DisableCycleErrors { // with cycle counter and s.th. lost
		if t.receivedCycle != t.expectedCycle { // no cycle check for 0xc0 to avoid messages on every target reset and when no cycle counter is active
			n += copy(b[n:], fmt.Sprintln("CYCLE_ERROR:\a", t.receivedCycle, "!=", t.expectedCycle, " (count=", emitter.TagEvents("CYCLE_ERROR")+1, ")"))
			n += copy(b[n:], "                                         ") // len of location information plus stamp: 41 spaces - see NewlineIndent below - todo: make it generic
			t.expectedCycle = t.receivedCycle                             // adjust cycle
		}
		decoder.InitialCycle = false
		t.expectedCycle++
	}
	return
}

// sprintTrice writes the p.t trice string or appropriate message into b and returns that len.
//
// p.t.Trice.Type is the received trice, in fact the name from til.json.
func (p *trexDec) sprintTrice(b []byte) (n int) {
	t := p.t
	isSAlias := strings.HasPrefix(t.Trice.Strg, id.SAliasStrgPrefix) && strings.HasSuffix(t.Trice.Strg, id.SAliasStrgSuffix)
	if isSAlias { // A SAlias Strg is covered with id.SAliasStrgPrefix and id.SAliasStrgSuffix in til.json and it needs to be replaced with "%s" here.
		t.Trice.Strg = "%s" // See appropriate comment inside insertTriceIDs().
	}

	t.pFmt, t.u = decoder.UReplaceN(t.Trice.Strg)
	t.Trice.Type = strings.TrimSuffix(t.Trice.Type, "AssertTrue")
	t.Trice.Type = strings.TrimSuffix(t.Trice.Type, "AssertFalse")
	triceType, err := id.ConstructFullTriceInfo(t.Trice.Type, len(t.u))

	if err != nil {
		n += copy(b[n:], fmt.Sprintln("err:ConstructFullTriceInfo failed with:", t.Trice.Type, len(p.I), "- ignoring package:"))
		return
	}
	ucTriceTypeReceived := strings.ToUpper(t.Trice.Type)   // examples: TRICE_S,   TRICE,   TRICE32,   TRICE16_2
	ucTriceTypeReconstructed := strings.ToUpper(triceType) // examples: TRICE32_S, TRICE0,  TRICE32_4, TRICE16_2
	for _, s := range cobsFunctionPtrList {                // walk through the list and try to find a match for execution
		if s.triceType == ucTriceTypeReconstructed || s.triceType == ucTriceTypeReceived { // match list entry "TRICE..."
			if len(t.v) < t.paramSize {
				n += copy(b[n:], fmt.Sprintln("err:len(t.v) =", len(t.v), "< t.paramSize = ", t.paramSize, "- ignoring package:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(t.v)))
				n += copy(b[n:], fmt.Sprintln(decoder.Hints))
				return
			}
			if t.paramSize != (s.bitWidth>>3)*s.paramCount {
				specialCases := []string{
					"TRICES", "TRICEN", "TRICEB", "TRICEF",
					"TRICE8B", "TRICE16B", "TRICE32B", "TRICE64B",
					"TRICE8F", "TRICE16F", "TRICE32F", "TRICE64F",

					"TRICE_S", "TRICE_N", "TRICE_B", "TRICE_F",
					"TRICE8_B", "TRICE16_B", "TRICE32_B", "TRICE64_B",
					"TRICE8_F", "TRICE16_F", "TRICE32_F", "TRICE64_F",
				}
				tt := strings.ToUpper(s.triceType)
				for _, casus := range specialCases {
					if tt == casus {
						goto ignoreSpecialCase
					}
				}
				n += copy(b[n:], fmt.Sprintln("err:s.triceType =", s.triceType, "ParamSpace =", t.paramSize, "not matching with bitWidth ", s.bitWidth, "and paramCount", s.paramCount, "- ignoring package:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(t.v)))
				n += copy(b[n:], fmt.Sprintln(decoder.Hints))
				return
			}
		ignoreSpecialCase:
			ss := strings.Split(t.pFmt, `\n`)
			if len(ss) >= 3 { // at least one "\n" before "\n" line end
				if decoder.NewlineIndent == -1 { // auto sense
					decoder.NewlineIndent = 12 + 1 // todo: strings.SplitN & len(decoder.TargetStamp0) // 12
					if !(id.LIFnJSON == "off" || id.LIFnJSON == "none") {
						decoder.NewlineIndent += 28 // todo: length(decoder.LocationInformationFormatString), see https://stackoverflow.com/questions/32987215/find-numbers-in-string-using-golang-regexp
						// todo: split channel info with format specifiers too, example: ["msg:%d\nsignal:%x %u\n", p0, p1, p2] -> ["msg:%d\n", p0] && ["signal:%x %u\n", p1, p2]
					}
					if decoder.ShowID != "" {
						decoder.NewlineIndent += 5 // todo: automatic
					}
				}
				skip := `\n`
				spaces := decoder.NewlineIndent
				for spaces > 0 {
					skip += " "
					spaces--
				}
				t.pFmt = strings.Join(ss[:], skip)
				t.pFmt = strings.TrimRight(t.pFmt, " ")
			}

			n += s.triceFn(p, b, s.bitWidth, s.paramCount) // match found, call handler
			return
		}
	}
	n += copy(b[n:], fmt.Sprintln("err:Unknown trice.Type:", t.Trice.Type, "and", triceType, "not matching - ignoring trice data:"))
	n += copy(b[n:], fmt.Sprintln(hex.Dump(p.I)))
	n += copy(b[n:], fmt.Sprintln(decoder.Hints))
	return
}

// triceTypeFn is the type for cobsFunctionPtrList elements.
type triceTypeFn struct {
	triceType  string                                              // triceType describes if parameters, the parameter bit width or if the parameter is a string.
	triceFn    func(p *trexDec, b []byte, bitwidth, count int) int // triceFn performs the conversion to the output string.
	ParamSpace int                                                 // ParamSpace is the count of bytes allocated for the parameters.
	bitWidth   int                                                 // bitWidth is the individual parameter width.
	paramCount int                                                 // paramCount is the amount pf parameters for the format string, which must match the count of format specifiers.
}

// cobsFunctionPtrList is a function pointer list.
var cobsFunctionPtrList = [...]triceTypeFn{
	{"TRICE_0", (*trexDec).trice0, 0, 0, 0},
	{"TRICE8_1", (*trexDec).unSignedOrSignedOut, 1, 8, 1},
	{"TRICE8_2", (*trexDec).unSignedOrSignedOut, 2, 8, 2},
	{"TRICE8_3", (*trexDec).unSignedOrSignedOut, 3, 8, 3},
	{"TRICE8_4", (*trexDec).unSignedOrSignedOut, 4, 8, 4},
	{"TRICE8_5", (*trexDec).unSignedOrSignedOut, 5, 8, 5},
	{"TRICE8_6", (*trexDec).unSignedOrSignedOut, 6, 8, 6},
	{"TRICE8_7", (*trexDec).unSignedOrSignedOut, 7, 8, 7},
	{"TRICE8_8", (*trexDec).unSignedOrSignedOut, 8, 8, 8},
	{"TRICE8_9", (*trexDec).unSignedOrSignedOut, 9, 8, 9},
	{"TRICE8_10", (*trexDec).unSignedOrSignedOut, 10, 8, 10},
	{"TRICE8_11", (*trexDec).unSignedOrSignedOut, 11, 8, 11},
	{"TRICE8_12", (*trexDec).unSignedOrSignedOut, 12, 8, 12},
	{"TRICE16_1", (*trexDec).unSignedOrSignedOut, 2, 16, 1},
	{"TRICE16_2", (*trexDec).unSignedOrSignedOut, 4, 16, 2},
	{"TRICE16_3", (*trexDec).unSignedOrSignedOut, 6, 16, 3},
	{"TRICE16_4", (*trexDec).unSignedOrSignedOut, 8, 16, 4},
	{"TRICE16_5", (*trexDec).unSignedOrSignedOut, 10, 16, 5},
	{"TRICE16_6", (*trexDec).unSignedOrSignedOut, 12, 16, 6},
	{"TRICE16_7", (*trexDec).unSignedOrSignedOut, 14, 16, 7},
	{"TRICE16_8", (*trexDec).unSignedOrSignedOut, 16, 16, 8},
	{"TRICE16_9", (*trexDec).unSignedOrSignedOut, 18, 16, 9},
	{"TRICE16_10", (*trexDec).unSignedOrSignedOut, 20, 16, 10},
	{"TRICE16_11", (*trexDec).unSignedOrSignedOut, 22, 16, 11},
	{"TRICE16_12", (*trexDec).unSignedOrSignedOut, 24, 16, 12},
	{"TRICE32_1", (*trexDec).unSignedOrSignedOut, 4, 32, 1},
	{"TRICE32_2", (*trexDec).unSignedOrSignedOut, 8, 32, 2},
	{"TRICE32_3", (*trexDec).unSignedOrSignedOut, 12, 32, 3},
	{"TRICE32_4", (*trexDec).unSignedOrSignedOut, 16, 32, 4},
	{"TRICE32_5", (*trexDec).unSignedOrSignedOut, 20, 32, 5},
	{"TRICE32_6", (*trexDec).unSignedOrSignedOut, 24, 32, 6},
	{"TRICE32_7", (*trexDec).unSignedOrSignedOut, 28, 32, 7},
	{"TRICE32_8", (*trexDec).unSignedOrSignedOut, 32, 32, 8},
	{"TRICE32_9", (*trexDec).unSignedOrSignedOut, 36, 32, 9},
	{"TRICE32_10", (*trexDec).unSignedOrSignedOut, 40, 32, 10},
	{"TRICE32_11", (*trexDec).unSignedOrSignedOut, 44, 32, 11},
	{"TRICE32_12", (*trexDec).unSignedOrSignedOut, 48, 32, 12},
	{"TRICE64_1", (*trexDec).unSignedOrSignedOut, 8, 64, 1},
	{"TRICE64_2", (*trexDec).unSignedOrSignedOut, 16, 64, 2},
	{"TRICE64_3", (*trexDec).unSignedOrSignedOut, 24, 64, 3},
	{"TRICE64_4", (*trexDec).unSignedOrSignedOut, 32, 64, 4},
	{"TRICE64_5", (*trexDec).unSignedOrSignedOut, 40, 64, 5},
	{"TRICE64_6", (*trexDec).unSignedOrSignedOut, 48, 64, 6},
	{"TRICE64_7", (*trexDec).unSignedOrSignedOut, 56, 64, 7},
	{"TRICE64_8", (*trexDec).unSignedOrSignedOut, 64, 64, 8},
	{"TRICE64_9", (*trexDec).unSignedOrSignedOut, 72, 64, 9},
	{"TRICE64_10", (*trexDec).unSignedOrSignedOut, 80, 64, 10},
	{"TRICE64_11", (*trexDec).unSignedOrSignedOut, 88, 64, 11},
	{"TRICE64_12", (*trexDec).unSignedOrSignedOut, 96, 64, 12},

	{"TRICES", (*trexDec).triceS, -1, 0, 0},
	{"TRICEN", (*trexDec).triceN, -1, 0, 0},

	{"TRICE8F", (*trexDec).trice8F, -1, 0, 0},
	{"TRICE16F", (*trexDec).trice16F, -1, 0, 0},
	{"TRICE32F", (*trexDec).trice32F, -1, 0, 0},
	{"TRICE64F", (*trexDec).trice64F, -1, 0, 0},

	{"TRICE8B", (*trexDec).trice8B, -1, 0, 0},
	{"TRICE16B", (*trexDec).trice16B, -1, 0, 0},
	{"TRICE32B", (*trexDec).trice32B, -1, 0, 0},
	{"TRICE64B", (*trexDec).trice64B, -1, 0, 0},
}

// triceN converts dynamic strings.
func (p *trexDec) triceN(b []byte, _ int, _ int) int {
	s := string(p.t.v[:p.t.paramSize])
	// todo: evaluate t.Trice.Strg, use p.SLen and do whatever should be done
	return copy(b, fmt.Sprintf(p.t.Trice.Strg, s))
}

// triceS converts dynamic strings.
func (p *trexDec) triceS(b []byte, _ int, _ int) int {
	s := string(p.t.v)
	return copy(b, fmt.Sprintf(p.t.Trice.Strg, s))
}

// triceB converts dynamic buffers.
func (p *trexDec) trice8B(b []byte, _ int, _ int) (n int) {
	t := p.t
	if decoder.DebugOut {
		fmt.Fprintln(p.W, string(p.t.v))
	}
	s := p.t.v
	before, after, found := strings.Cut(t.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = t.Trice.Strg
	}

	noNewline := strings.TrimSuffix(after, `\n`)

	for i := 0; i < len(s); i++ {
		n += copy(b[n:], fmt.Sprintf(noNewline, s[i]))
	}
	if len(noNewline) < len(after) { // strings.TrimSuffix removed a newline
		n += copy(b[n:], fmt.Sprintln()) // so add it finally
	}
	return
}

// trice16B converts dynamic buffers.
func (p *trexDec) trice16B(b []byte, _ int, _ int) (n int) {
	t := p.t
	if decoder.DebugOut {
		fmt.Fprintln(p.W, string(p.t.v))
	}
	s := p.t.v

	before, after, found := strings.Cut(t.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = t.Trice.Strg
	}

	noNewline := strings.TrimSuffix(after, `\n`)

	for i := 0; i < len(s); i += 2 {
		n += copy(b[n:], fmt.Sprintf(noNewline, binary.LittleEndian.Uint16(s[i:])))
	}
	if len(noNewline) < len(after) { // strings.TrimSuffix removed a newline
		n += copy(b[n:], fmt.Sprintln()) // so add it finally
	}

	return
}

// trice32B converts dynamic buffers.
func (p *trexDec) trice32B(b []byte, _ int, _ int) (n int) {
	t := p.t
	if decoder.DebugOut {
		fmt.Fprintln(p.W, string(p.t.v))
	}
	s := p.t.v

	before, after, found := strings.Cut(t.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = t.Trice.Strg
	}

	noNewline := strings.TrimSuffix(after, `\n`)

	for i := 0; i < len(s); i += 4 {
		n += copy(b[n:], fmt.Sprintf(noNewline, binary.LittleEndian.Uint32(s[i:])))
	}
	if len(noNewline) < len(after) { // strings.TrimSuffix removed a newline
		n += copy(b[n:], fmt.Sprintln()) // so add it finally
	}
	return
}

// trice64B converts dynamic buffers.
func (p *trexDec) trice64B(b []byte, _ int, _ int) (n int) {
	t := p.t
	if decoder.DebugOut {
		fmt.Fprintln(p.W, string(p.t.v))
	}
	s := p.t.v

	before, after, found := strings.Cut(t.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = t.Trice.Strg
	}

	noNewline := strings.TrimSuffix(after, `\n`)

	for i := 0; i < len(s); i += 8 {
		n += copy(b[n:], fmt.Sprintf(noNewline, binary.LittleEndian.Uint64(s[i:])))
	}
	if len(noNewline) < len(after) { // strings.TrimSuffix removed a newline
		n += copy(b[n:], fmt.Sprintln()) // so add it finally
	}
	return
}

// trice8F display function call with 8-bit parameters.
func (p *trexDec) trice8F(b []byte, _ int, _ int) (n int) {
	t := p.t
	if decoder.DebugOut {
		fmt.Fprintln(p.W, string(p.t.v))
	}
	s := p.t.v
	n += copy(b[n:], fmt.Sprintf(t.Trice.Strg))
	for i := 0; i < len(s); i++ {
		n += copy(b[n:], fmt.Sprintf("(%02x)", s[i]))
	}
	n += copy(b[n:], fmt.Sprintln())
	return
}

// trice16F display function call with 16-bit parameters.
func (p *trexDec) trice16F(b []byte, _ int, _ int) (n int) {
	t := p.t
	if decoder.DebugOut {
		fmt.Fprintln(p.W, string(p.t.v))
	}
	s := p.t.v
	n += copy(b[n:], fmt.Sprintf(t.Trice.Strg))
	for i := 0; i < len(s); i += 2 {
		n += copy(b[n:], fmt.Sprintf("(%04x)", binary.LittleEndian.Uint16(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintln())
	return
}

// trice32F display function call with 32-bit parameters.
func (p *trexDec) trice32F(b []byte, _ int, _ int) (n int) {
	t := p.t
	if decoder.DebugOut {
		fmt.Fprintln(p.W, string(p.t.v))
	}
	s := p.t.v
	n += copy(b[n:], fmt.Sprintf(t.Trice.Strg))
	for i := 0; i < len(s); i += 4 {
		n += copy(b[n:], fmt.Sprintf("(%08x)", binary.LittleEndian.Uint32(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintln())
	return
}

// trice64F display function call with 64-bit parameters.
func (p *trexDec) trice64F(b []byte, _ int, _ int) (n int) {
	t := p.t
	if decoder.DebugOut {
		fmt.Fprintln(p.W, string(p.t.v))
	}
	s := p.t.v
	n += copy(b[n:], fmt.Sprintf(t.Trice.Strg))
	for i := 0; i < len(s); i += 8 {
		n += copy(b[n:], fmt.Sprintf("(%016x)", binary.LittleEndian.Uint64(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintln())
	return
}

// trice0 prints the trice format string.
func (p *trexDec) trice0(b []byte, _ int, _ int) int {
	t := p.t
	return copy(b, fmt.Sprintf(t.pFmt))
}

// unSignedOrSignedOut prints p.t.v according to the format string.
func (p *trexDec) unSignedOrSignedOut(b []byte, bitwidth, count int) int {
	t := p.t
	if len(t.u) != count {
		return copy(b, fmt.Sprintln("ERROR: Invalid format specifier count inside", t.Trice.Type, t.Trice.Strg))
	}
	v := make([]interface{}, 32768) // theoretical 2^15 bytes could arrive
	switch bitwidth {
	case 8:
		for i, f := range t.u {
			switch f {
			case decoder.UnsignedFormatSpecifier, decoder.PointerFormatSpecifier: // see comment inside decoder.UReplaceN
				v[i] = p.t.v[i]
			case decoder.SignedFormatSpecifier:
				v[i] = int8(p.t.v[i])
			case decoder.BooleanFormatSpecifier:
				v[i] = p.t.v[i] != 0
			default:
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier (float?) inside", t.Trice.Type, t.Trice.Strg))
			}
		}
	case 16:
		for i, f := range t.u {
			n := p.ReadU16(p.t.v[2*i:])
			switch f {
			case decoder.UnsignedFormatSpecifier, decoder.PointerFormatSpecifier: // see comment inside decoder.UReplaceN
				v[i] = n
			case decoder.SignedFormatSpecifier:
				v[i] = int16(n)
			case decoder.BooleanFormatSpecifier:
				v[i] = n != 0
			default:
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier (float?) inside", t.Trice.Type, t.Trice.Strg))
			}
		}
	case 32:
		for i, f := range t.u {
			n := p.ReadU32(p.t.v[4*i:])
			switch f {
			case decoder.UnsignedFormatSpecifier, decoder.PointerFormatSpecifier: // see comment inside decoder.UReplaceN
				v[i] = n
			case decoder.SignedFormatSpecifier:
				v[i] = int32(n)
			case decoder.FloatFormatSpecifier:
				v[i] = math.Float32frombits(n)
			case decoder.BooleanFormatSpecifier:
				v[i] = n != 0
			default:
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier inside", t.Trice.Type, t.Trice.Strg))
			}
		}
	case 64:
		for i, f := range t.u {
			n := p.ReadU64(p.t.v[8*i:])
			switch f {
			case decoder.UnsignedFormatSpecifier, decoder.PointerFormatSpecifier: // see comment inside decoder.UReplaceN
				v[i] = n
			case decoder.SignedFormatSpecifier:
				v[i] = int64(n)
			case decoder.FloatFormatSpecifier:
				v[i] = math.Float64frombits(n)
			case decoder.BooleanFormatSpecifier:
				v[i] = n != 0
			default:
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier inside", t.Trice.Type, t.Trice.Strg))
			}
		}
	}
	return copy(b, fmt.Sprintf(t.pFmt, v[:len(t.u)]...))
}
