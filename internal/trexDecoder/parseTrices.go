package trexDecoder

import (
	"encoding/hex"
	"fmt"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/cipher"
)

// readAndParseTrices gets the next Trices into p.trices.
// To achieve that, it reads next bytes temporary into p.I and parses them.
func (p *trexDec) readAndParseTrices() (err error) {
	p.tcount = 0
	switch p.packageFraming {
	case packageFramingNone, packageFramingNone0:
		p.nextData()           // appends next data to p.I
		p.parseUnframedData0() // removes processed data from p.I
	case packageFramingNone8:
		p.nextData()           // appends next data to p.I
		p.parseUnframedData8() // removes processed data from p.I
	case packageFramingNone32:
		p.nextData()            // appends next data to p.I
		p.parseUnframedData32() // removes processed data from p.I
	case packageFramingNone64: // XTEA is active
		p.nextData()            // appends next data to p.I
		p.parseUnframedData64() // removes processed data from p.I
	default:
		p.nextFrame()
		p.parseDecodedFrame()
	}
	return
}

// nextTriceComplete tries to interpret next data in p.I. p.I is not altered. If not enough bytes, false is returned.
// On success, true is returned and in p.Trices[p.tcount] these values get filled already:
// id, ilk, headerSize, paramSize,
// When ilk != 0 also stampSize, stamp, p.Trices[p.tcount+1].expectedCycle are set too.
func (p *trexDec) nextTriceComplete() bool {
	t := &p.trices[p.tcount]
	t.headerSize = 2
	if len(p.I) < t.headerSize { // not enough data
		return false
	}
	//  if cipher.Password != "" {
	//  	p.headerSize = 8
	//  	if len(p.I) < p.headerSize { // not enough data
	//  		return false
	//  	}
	//  	// todo:
	//  	// Decrypt 8 bytes and optionally next 8 bytes and so on, until the first Trice is complete.
	//  	// We may need to wait for more data
	//  }
	tyId := id.TriceID(p.ReadU16(p.I))
	t.ilk = tyId >> decoder.IDBits // 2 most significant bits are the ilk.
	t.id = 0x3FFF & tyId           // 14 least significant bits are the ID.

	switch t.ilk {
	case typeS0: // no stamp
		return p.triceIlkS0Complete()
	case typeS2: // 16-bit stamp
		return p.triceIlkS2Complete()
	case typeS4: // 32-bit stamp
		return p.triceIlkS4Complete()
	case typeX0:
		return p.triceIlkX0Complete()
	}
	return false
}

// ParseDecodedFrame expects in p.I a decoded frame for interpretation.
func (p *trexDec) parseDecodedFrame() {
	maxRestSize := 3
	if cipher.Password != "" { // with XTEA
		maxRestSize = 7
	}
	for p.nextTriceComplete() {
		p.parseNextTriceInFramedBuffer()
	}
	if len(p.I) >= maxRestSize {
		s := fmt.Sprintln("WARNING:discarding unexpected bytes", hex.Dump(p.I))
		p.W.Write([]byte(s))
	} else {
		if !allZero(p.I) {
			s := fmt.Sprintln("WARNING:non-zero padding bytes:", hex.Dump(p.I))
			p.W.Write([]byte(s))
		}
	}
	p.I = p.I[:0] // Discard any leftovers at end of frame.
}

// parseNextTriceInFramedBuffer expects in p.I enough data for parsing a Trice. After parsing a Trice its bytes are removed.
// We do not know here, if padding zeroes are follwing, so we remove just the Trice data.
// Element p.Trices[p.tcount] gets filled here. Its fields headerSize, ilk, id, receivedCycle,
// paramSize, stampSize, stamp are valid already. p.tcount is incremented at the end.
// Here values are only referenced because the buffer is a frame.
func (p *trexDec) parseNextTriceInFramedBuffer() {
	t := &p.trices[p.tcount]
	t.v = p.I[t.headerSize:]             // values space with optional padding bytes starts here.
	t.v = t.v[:t.paramSize]              // exclude values paddings bytes
	p.I = p.I[t.headerSize+t.paramSize:] // remove evaluated data
	if t.ilk != 0 {
		decoder.RecordForStatistics(t.id) // This is for the "trice log -stat" flag
		decoder.LastTriceID = t.id        // used for showID TODO: That can get wrong! That is needed during print.
		p.tcount++
	} else {
		// todo: handle typeX0
	}
}

// parseNextTriceInUnframedBuffer expects in p.I enough data for parsing a Trice. After parsing a Trice its bytes are removed.
// We do not know here, if padding zeroes are follwing, so we remove just the Trice data.
// Element p.Trices[p.tcount] gets filled here. Its fields headerSize, ilk, id, receivedCycle,
// paramSize, stampSize, stamp are valid already. p.tcount is incremented at the end.
// Here a deep value copy is done, because we do not know how long the unframed buffer might be.
func (p *trexDec) parseNextTriceInUnframedBuffer() {
	t := &p.trices[p.tcount]
	// todo: n = p.checkReceivedCycle(b)
	//var ok bool
	//p.LutMutex.RLock()
	//p.Trice, ok = p.Lut[p.id]
	//p.LutMutex.RUnlock()
	//if ok {
	//if AddNewlineToEachTriceMessage {
	//	p.Trice.Strg += `\n` // this adds a newline to each single Trice message
	//}
	v := p.I[t.headerSize:]      // values space with optional padding bytes starts here.
	v = v[:t.paramSize]          // exclude values paddings bytes
	t.v = append([]byte{}, v...) // deep copy, needed for unframed cases, https://stackoverflow.com/questions/68966356/slice-copy-mutating-original-slice
	// TODO: explain situation. For framed cases: t.v = v

	//n += p.sprintTrice(b[n:])        // use param info
	//} else {
	//	n += copy(b[n:], fmt.Sprintln("WARNING:\aunknown ID ", p.id, "- ignoring trice:"))
	//	n += copy(b[n:], fmt.Sprintln(hex.Dump(p.I[:p.size])))
	//	n += copy(b[n:], fmt.Sprintln(decoder.Hints))
	//}
	p.tcount++
	if t.ilk != 0 {
		decoder.RecordForStatistics(t.id) // This is for the "trice log -stat" flag
		decoder.LastTriceID = t.id        // used for showID TODO: That can get wrong! That is needed during print.
	} else {
		// todo: handle typeX0
	}
	p.I = p.I[t.headerSize+t.paramSize:] // remove evaluated data
}

// parseUnframedData0 analyzes next Trice in compact or aligned p.I, returns the result in p.Trices and removes the interpreted bytes from p.I including optional padding bytes.
// If not enough data in p.I including optional padding bytes nothing happens.
// It tries to detect, if the data stream is compact or aligned and sets p.packageFraming accordingly.
// We do not need to check none64 cases. We forbid for now, that typeX0 Trices start with 00.
// Aligned streams are not allowed to have more than a single Trice between 2 alignment borders (|).
// The data stream options:
// case 0: |Trice|    -> p.paddingSpace = 0. No decision possible.
//
//	|Trice |   -> p.paddingSpace = 1. No decision possible because the padding space did not arrived yet.
//	|Trice  |  -> p.paddingSpace = 2. No decision possible because the padding space did not arrived yet.
//	|Trice   | -> p.paddingSpace = 3. No decision possible because the padding space did not arrived yet.
//
// case 1: |TriceN|   -> If N   is 0   { No decision possible. The 0 could be a padding zero (none32) or part of the next Trice (none8). } else{ framing is none8 }
// case 2: |TriceNN|  -> If NN  is 00  { framing is none32. The 00  could be a typeX0 start but we forbid that for now. }                  else{ framing is none8 }
// case 3: |TriceNNN| -> If NNN is 000 { framing is none32. The 000 could be a typeX0 start but we forbid that for now. }                  else{ framing is none8 }
func (p *trexDec) parseUnframedData0() {
	for {
		switch p.paddingSpace { // p.paddingSpace is initially 0
		case 3:
			if len(p.I) < 3 {
				return
			}
			if allZero(p.I[:p.paddingSpace]) {
				p.I = p.I[p.paddingSpace:] // remove padding zeroes
				p.packageFraming = packageFramingNone32
				return
			}
		case 2:
			if len(p.I) < 2 {
				return
			}
			if allZero(p.I[:p.paddingSpace]) {
				p.I = p.I[p.paddingSpace:] // remove padding zeroes
				p.packageFraming = packageFramingNone32
				return
			}
		case 1:
			if len(p.I) < 1 {
				return
			}
			if p.I[0] != 0 {
				p.packageFraming = packageFramingNone8
				return
			} else {
				// If we modify the ID generation to skip all IDs with lower byte =0 we will not arrive here. With big endian there are the ilk bits.
				// But legacy projects with fixed IDs could have IDs with lower byte =0 and use the pf=none switch. So we do our best here.
				// Here we have |Trice0|Trice and we do not know, if the 0 is part of the next Trice.
				// So let us check, if the ID is known:
				if len(p.I) < 2 {
					return
				}
				_, ok := p.Lut[id.TriceID(p.ReadU16(p.I))]
				if !ok { // no match, so the 0 is a padding zero (if no other error)
					p.I = p.I[1:] // remove padding zero
					p.packageFraming = packageFramingNone32
					return
				}
				if len(p.I) < 3 {
					return
				}
				// The id with low part 0 is known but that is no guaranty, that the 0 is no padding byte.
				_, ok = p.Lut[id.TriceID(p.ReadU16(p.I[1:]))]
				if !ok { // now we have the guaranty, because with padding byte we get an error
					p.I = p.I[1:] // remove padding zero
					p.packageFraming = packageFramingNone32
					return
				}
				p.W.Write([]byte(`WARNING:Here we have, the case that both variants give valid IDs.
We could evaluate further to check param count and cycle counter but a 100% guaranty we will not get.
Let it be for now...
`))
			}
		}
		if !p.nextTriceComplete() {
			// Leaving here with p.paddingSpace !=0 and p.packageFraming = packageFramingNone0 is ok, because we put some more bytes into p.I later.
			break
		}
		p.parseNextTriceInUnframedBuffer() // removes Trice from p.I but not optional padding bytes
		switch p.trices[p.tcount].headerSize % 4 {
		case 0:
			p.paddingSpace = 0
		case 1:
			p.paddingSpace = 3
		case 2:
			p.paddingSpace = 2
		case 3:
			p.paddingSpace = 1
		}
	}
}

// parseUnframedData8 parses next Trices in compact buffer p.I, returns the result in p.Trices[p.tcount++] and removes the interpreted bytes from p.I.
// If not enough data in p.I nothing happens.
func (p *trexDec) parseUnframedData8() {
	for p.nextTriceComplete() {
		p.parseNextTriceInUnframedBuffer()
	}
}

// parseUnframedData32 parses next Trices in 32-bit aligned buffer p.I, returns the result in p.Trices[p.tcount++] and removes the interpreted bytes from p.I including optional 0-3 adding bytes.
// If not enough data in p.I including optional padding bytes nothing happens. Non-zero padding bytes are reported.
func (p *trexDec) parseUnframedData32() {
	for p.nextTriceComplete() {
		t := &p.trices[p.tcount]
		size := t.headerSize + t.paramSize
		alignCount := 3 & (-(size % 4))
		if len(p.I) < size+alignCount {
			return // not all expected alignment bytes arrived
		}
		p.parseNextTriceInUnframedBuffer()
		if !allZero(p.I[:alignCount]) {
			s := fmt.Sprintln("WARNING:non-zero padding bytes:", hex.Dump(p.I[:alignCount]))
			p.W.Write([]byte(s))
		}
		p.I = p.I[alignCount:]
	}
}

// parseUnframedData64 parses next Trices in 64-bit aligned buffer p.I, returns the result in p.Trices[p.tcount++] and removes the interpreted bytes from p.I including optional 0-7 adding bytes.
// If not enough data in p.I including optional padding bytes nothing happens. Non-zero padding bytes are reported.
func (p *trexDec) parseUnframedData64() {
	for p.nextTriceComplete() {
		t := &p.trices[p.tcount]
		size := t.headerSize + t.paramSize
		alignCount := 7 & (-(size % 8))
		if len(p.I) < size+alignCount {
			return // not all expected alignment bytes arrived
		}
		p.parseNextTriceInUnframedBuffer()
		if !allZero(p.I[:alignCount]) {
			s := fmt.Sprintln("WARNING:non-zero padding bytes:", hex.Dump(p.I[:alignCount]))
			p.W.Write([]byte(s))
		}
		p.I = p.I[alignCount:]
	}
}

func (p *trexDec) getCycleAndParamSize(i []byte) {
	t := &p.trices[p.tcount]
	nc := p.ReadU16(i) // n = number of data bytes (without timestamp), most significant bit is the count encoding, c = cycle
	if nc>>15 == 1 {   // special case: more than data 127 bytes
		// C code: #define TRICE_LCNT(count) TRICE_PUT16( (0x8000 | (count)) );
		t.receivedCycle = t.expectedCycle // cycle is not transmitted, so set expected value
		t.paramSize = int(0x7FFF & nc)    // 15 bit for data byte count excluding timestamp
	} else {
		// C code: #define TRICE_CNTC(count) TRICE_PUT16( ((count)<<8) | TRICE_CYCLE )
		t.receivedCycle = uint8(nc) // low byte is cycle
		t.paramSize = int(nc >> 8)  // high byte is 7 bit number of bytes for data count excluding timestamp
	}
	p.trices[p.tcount+1].expectedCycle = t.receivedCycle // set expected cycle for next Triec
}

func (p *trexDec) triceIlkS0Complete() bool {
	t := &p.trices[p.tcount]
	t.stampSize = 0
	t.headerSize += ncSize
	if len(p.I) < t.headerSize {
		return false
	}
	p.getCycleAndParamSize(p.I[tyIdSize:])
	size := t.headerSize + t.paramSize
	return len(p.I) >= size
}

func (p *trexDec) triceIlkS2Complete() bool {
	t := &p.trices[p.tcount]
	t.stampSize = 2
	if Doubled16BitID { // p.packageFraming == packageFramingNone || cipher.Password != "" {
		t.headerSize += tyIdSize + 2 + ncSize
	} else {
		t.headerSize += 2 + ncSize
	}
	if len(p.I) < t.headerSize {
		return false
	}
	t.stamp = uint64(p.ReadU16(p.I[t.headerSize-4:]))
	p.getCycleAndParamSize(p.I[t.headerSize-2:])
	size := t.headerSize + t.paramSize
	return len(p.I) >= size
}

func (p *trexDec) triceIlkS4Complete() bool {
	t := &p.trices[p.tcount]
	t.stampSize = 4
	t.headerSize += 4 + ncSize
	if len(p.I) < t.headerSize {
		return false
	}
	t.stamp = uint64(p.ReadU32(p.I[tyIdSize:]))
	p.getCycleAndParamSize(p.I[tyIdSize+4:])
	size := t.headerSize + t.paramSize
	return len(p.I) >= size
}

func (p *trexDec) triceIlkX0Complete() bool {
	t := &p.trices[p.tcount]
	switch TypeX0Handler {
	case "countedString": // The first 2 bytes are the count.
		t.paramSize += int(t.id)
		size := t.headerSize + t.paramSize
		if len(p.I) < size {
			return false // not enough data
		}
	default:
		return false
	}
	return true
}

// allZero returns true, when all values in bytes are 0.
// If bytes is empty, true is returned as well.
func allZero(bytes []byte) bool {
	b := byte(0)
	for _, s := range bytes {
		b |= s
	}
	return b == 0
}
