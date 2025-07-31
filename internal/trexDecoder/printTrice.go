package trexDecoder

import (
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"math"
	"strings"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
)

// sprintTrice writes a trice string or appropriate message into b and returns that len.
//
// p.Trice.Type is the received trice, in fact the name from til.json.
func (p *trexDec) sprintTrice(b []byte) (n int) {

	isSAlias := strings.HasPrefix(p.Trice.Strg, id.SAliasStrgPrefix) && strings.HasSuffix(p.Trice.Strg, id.SAliasStrgSuffix)
	if isSAlias { // A SAlias Strg is covered with id.SAliasStrgPrefix and id.SAliasStrgSuffix in til.json and it needs to be replaced with "%s" here.
		p.Trice.Strg = "%s" // See appropriate comment inside insertTriceIDs().
	}

	p.pFmt, p.u = decoder.UReplaceN(p.Trice.Strg)
	p.Trice.Type = strings.TrimSuffix(p.Trice.Type, "AssertTrue")
	p.Trice.Type = strings.TrimSuffix(p.Trice.Type, "AssertFalse")
	triceType, err := id.ConstructFullTriceInfo(p.Trice.Type, len(p.u))

	if err != nil {
		n += copy(b[n:], fmt.Sprintln("err:ConstructFullTriceInfo failed with:", p.Trice.Type, len(p.I), "- ignoring package:"))
		return
	}
	ucTriceTypeReceived := strings.ToUpper(p.Trice.Type)   // examples: TRICE_S,   TRICE,   TRICE32,   TRICE16_2
	ucTriceTypeReconstructed := strings.ToUpper(triceType) // examples: TRICE32_S, TRICE0,  TRICE32_4, TRICE16_2
	for _, s := range cobsFunctionPtrList {                // walk through the list and try to find a match for execution
		if s.triceType == ucTriceTypeReconstructed || s.triceType == ucTriceTypeReceived { // match list entry "TRICE..."
			if len(p.V) < p.ParamSpace {
				n += copy(b[n:], fmt.Sprintln("err:len(p.V) =", len(p.V), "< p.ParamSpace = ", p.ParamSpace, "- ignoring package:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(p.V)))
				n += copy(b[n:], fmt.Sprintln(decoder.Hints))
				return
			}
			if p.ParamSpace != (s.bitWidth>>3)*s.paramCount {
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
				n += copy(b[n:], fmt.Sprintln("err:s.triceType =", s.triceType, "ParamSpace =", p.ParamSpace, "not matching with bitWidth ", s.bitWidth, "and paramCount", s.paramCount, "- ignoring package:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(p.V)))
				n += copy(b[n:], fmt.Sprintln(decoder.Hints))
				return
			}
		ignoreSpecialCase:
			ss := strings.Split(p.pFmt, `\n`)
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
				p.pFmt = strings.Join(ss[:], skip)
				p.pFmt = strings.TrimRight(p.pFmt, " ")
			}

			n += s.triceFn(p, b, s.bitWidth, s.paramCount) // match found, call handler
			return
		}
	}
	n += copy(b[n:], fmt.Sprintln("err:Unknown trice.Type:", p.Trice.Type, "and", triceType, "not matching - ignoring trice data:"))
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
	s := string(p.V[:p.ParamSpace])
	// todo: evaluate p.Trice.Strg, use p.SLen and do whatever should be done
	return copy(b, fmt.Sprintf(p.Trice.Strg, s))
}

// triceS converts dynamic strings.
func (p *trexDec) triceS(b []byte, _ int, _ int) int {
	s := string(p.V)
	return copy(b, fmt.Sprintf(p.Trice.Strg, s))
}

// triceB converts dynamic buffers.
func (p *trexDec) trice8B(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.V)
	}
	s := p.V
	before, after, found := strings.Cut(p.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = p.Trice.Strg
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
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.V)
	}
	s := p.V

	before, after, found := strings.Cut(p.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = p.Trice.Strg
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
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.V)
	}
	s := p.V

	before, after, found := strings.Cut(p.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = p.Trice.Strg
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
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.V)
	}
	s := p.V

	before, after, found := strings.Cut(p.Trice.Strg, ":")
	if found {
		n += copy(b[n:], fmt.Sprint(before+":")) // print channel
	} else {
		after = p.Trice.Strg
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
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.V)
	}
	s := p.V
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i++ {
		n += copy(b[n:], fmt.Sprintf("(%02x)", s[i]))
	}
	n += copy(b[n:], fmt.Sprintln())
	return
}

// trice16F display function call with 16-bit parameters.
func (p *trexDec) trice16F(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.V)
	}
	s := p.V
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i += 2 {
		n += copy(b[n:], fmt.Sprintf("(%04x)", binary.LittleEndian.Uint16(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintln())
	return
}

// trice32F display function call with 32-bit parameters.
func (p *trexDec) trice32F(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.V)
	}
	s := p.V
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i += 4 {
		n += copy(b[n:], fmt.Sprintf("(%08x)", binary.LittleEndian.Uint32(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintln())
	return
}

// trice64F display function call with 64-bit parameters.
func (p *trexDec) trice64F(b []byte, _ int, _ int) (n int) {
	if decoder.DebugOut {
		fmt.Fprintln(p.W, p.V)
	}
	s := p.V
	n += copy(b[n:], fmt.Sprintf(p.Trice.Strg))
	for i := 0; i < len(s); i += 8 {
		n += copy(b[n:], fmt.Sprintf("(%016x)", binary.LittleEndian.Uint64(s[i:])))
	}
	n += copy(b[n:], fmt.Sprintln())
	return
}

// trice0 prints the trice format string.
func (p *trexDec) trice0(b []byte, _ int, _ int) int {
	return copy(b, fmt.Sprintf(p.pFmt))
}

// unSignedOrSignedOut prints p.V according to the format string.
func (p *trexDec) unSignedOrSignedOut(b []byte, bitwidth, count int) int {
	if len(p.u) != count {
		return copy(b, fmt.Sprintln("ERROR: Invalid format specifier count inside", p.Trice.Type, p.Trice.Strg))
	}
	v := make([]interface{}, 32768) // theoretical 2^15 bytes could arrive
	switch bitwidth {
	case 8:
		for i, f := range p.u {
			switch f {
			case decoder.UnsignedFormatSpecifier, decoder.PointerFormatSpecifier: // see comment inside decoder.UReplaceN
				v[i] = p.V[i]
			case decoder.SignedFormatSpecifier:
				v[i] = int8(p.V[i])
			case decoder.BooleanFormatSpecifier:
				v[i] = p.V[i] != 0
			default:
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier (float?) inside", p.Trice.Type, p.Trice.Strg))
			}
		}
	case 16:
		for i, f := range p.u {
			n := p.ReadU16(p.V[2*i:])
			switch f {
			case decoder.UnsignedFormatSpecifier, decoder.PointerFormatSpecifier: // see comment inside decoder.UReplaceN
				v[i] = n
			case decoder.SignedFormatSpecifier:
				v[i] = int16(n)
			case decoder.BooleanFormatSpecifier:
				v[i] = n != 0
			default:
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier (float?) inside", p.Trice.Type, p.Trice.Strg))
			}
		}
	case 32:
		for i, f := range p.u {
			n := p.ReadU32(p.V[4*i:])
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
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier inside", p.Trice.Type, p.Trice.Strg))
			}
		}
	case 64:
		for i, f := range p.u {
			n := p.ReadU64(p.V[8*i:])
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
				return copy(b, fmt.Sprintln("ERROR: Invalid format specifier inside", p.Trice.Type, p.Trice.Strg))
			}
		}
	}
	return copy(b, fmt.Sprintf(p.pFmt, v[:len(p.u)]...))
}
