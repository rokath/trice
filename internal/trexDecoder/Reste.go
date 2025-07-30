package trexDecoder

/*
// InterpretCobsFramedData analyzes next COBS buffer in framed buffer p.B, returs the result in b[:n] and removes the interpreted COBS data from p.B including the following 0-delimiter byte.
// If not enough data in p.B (no 0-delimiter) nothing happens and n=0, nil is returned. If an empty COBS package was found (just a 0-delimiter), it is removed from p.B and n=0, nil is returned.
func (p *trexDec) InterpretCobsFramedData(b []byte) (n int, err error) {
	m, err := cobs.Decode(p.ScratchPad, p.B)
	if err != nil {
		return
	}
	p.B = p.Scratchpad[:m]
	err = p.Decrypt()
	if err != nil {
		return
	}
	return p.InterpretDecodedFrame(p.ScratchPad[:m])
}

// InterpretTCobsFramedData analyzes next TCOBS buffer in framed buffer p.B, returs the result in b[:n] and removes the interpreted TCOBS data from p.B including the following 0-delimiter byte.
// If not enough data in p.B (no 0-delimiter) nothing happens and n=0, nil is returned. If an empty TCOBS package was found (just a 0-delimiter), it is removed from p.B and n=0, nil is returned.
func (p *trexDec) InterpretTCobsFramedData(b []byte) (n int, err error) {
	return
}

// InterpretTCobsV2FramedData analyzes next TCOBSv2 buffer in framed buffer p.B, returs the result in b[:n] and removes the interpreted TCOBv2 data from p.B including the following 0-delimiter byte.
// If not enough data in p.B (no 0-delimiter) nothing happens and n=0, nil is returned. If an empty TCOBSv2 package was found (just a 0-delimiter), it is removed from p.B and n=0, nil is returned.
func (p *trexDec) InterpretTCobsV2FramedData(b []byte) (n int, err error) {
	return
}
*/

/*
// nextFrame reads with an inner reader a (T)COBSv1 encoded byte stream.
//
// When no terminating 0 is found in the incoming bytes, nextFrame returns without action.
// That means the incoming data stream is exhausted and a next try should be started a bit later.
// Some arrived bytes are kept internally and concatenated with the following bytes in a next Read.
// When a terminating 0 is found in the incoming bytes ReadFromCOBS decodes the COBS package
// and returns it in b and its len in n. If more data arrived after the first terminating 0,
// these are kept internally and concatenated with the following bytes in a next Read.
func (p *trexDec) nextFrame() {
	// Here p.IBuf contains none or available bytes, what can be several trice messages.
	// So first try to process p.IBuf.
	index := bytes.IndexByte(p.IBuf, 0) // find terminating 0
	if index == -1 {                    // p.IBuf has no complete COBS data, so try to read more input
		m, err := p.In.Read(p.InnerBuffer)            // use p.InnerBuffer as bytes read buffer
		p.IBuf = append(p.IBuf, p.InnerBuffer[:m]...) // merge with leftovers
		if err != nil && err != io.EOF {              // some serious error
			log.Fatal("ERROR:internal reader error\a", err) // exit
		}
		index = bytes.IndexByte(p.IBuf, 0) // find terminating 0
		if index == -1 {                   // p.IBuf has no complete COBS data, so leave
			// Even err could be io.EOF, some valid data possibly in p.iBUf.
			// In case of file input (J-LINK usage) a plug off is not detectable here.
			return // no terminating 0, nothing to do
		}
	}
	if decoder.TestTableMode {
		p.printTestTableLine(index + 1)
	}
	// here a complete COBS or TCOBS package exists
	if decoder.DebugOut { // Debug output
		fmt.Printf("%s: ", decoder.PackageFraming)
		decoder.Dump(os.Stdout, p.IBuf[:index+1])
		// fmt.Fprintf(p.W, "%s: ", decoder.PackageFraming)
		// decoder.Dump(p.W, p.IBuf[:index+1])
	}

	frame := p.IBuf[:index]

	switch p.packageFraming {

	case packageFramingCOBS:
		p.B = p.B0                      // make([]byte, decoder.DefaultSize) // todo: avoid allocation
		n, e := cobs.Decode(p.B, frame) // if index is 0, an empty buffer is decoded
		p.IBuf = p.IBuf[index+1:]       // step forward (next package data in p.IBuf now, if any)
		if e != nil {
			if decoder.Verbose {
				fmt.Println("\ainconsistent COBS buffer!") // show also terminating 0
			}
		}
		p.B = p.B[:n]

	case packageFramingTCOBS:
	repeat:
		p.B = p.B0                       // make([]byte, decoder.DefaultSize) // todo: avoid allocation
		n, e := tcobs.Decode(p.B, frame) // if index is 0, an empty buffer is decoded
		// from merging: p.IBuf = p.IBuf[index+1:]        // step forward (next package data in p.IBuf now, if any)
		if e != nil {
			fmt.Println("\ainconsistent TCOBSv1 buffer!")

			// remove 3 lines if they exist
			s := strings.SplitN(strings.ReplaceAll(string(frame), "\r\n", "\n"), "\n", 4)
			var bytesCount int
			if len(s) >= 3 {
				var newLines int
				fmt.Println(s[0])
				fmt.Println(s[1])
				fmt.Println(s[2])
				for _, b := range frame {
					frame = frame[1:]
					bytesCount++
					if b == 10 {
						newLines++
						if newLines == 3 {
							break
						}
					}
					continue
				}
				index -= bytesCount
				goto repeat
			}
			if decoder.Verbose {
				fmt.Println(e, "\ainconsistent TCOBSv1 buffer:")
				fmt.Println(e, hex.Dump(frame)) // show also terminating 0
			}
			e = nil
			p.B = p.B[:0]
			p.IBuf = p.IBuf[index+1:] // step forward (next package data in p.IBuf now, if any) // from merging:
		} else {
			p.B = p.B[len(p.B)-n:]    // buffer is filled from the end
			p.IBuf = p.IBuf[index+1:] // step forward (next package data in p.IBuf now, if any) // from merging:
		}
	default:
		log.Fatalln("unexpected execution path", p.packageFraming)
	}

	if decoder.DebugOut { // Debug output
		fmt.Print("->TRICE: ")
		decoder.Dump(os.Stdout, p.B)
		// fmt.Fprint(p.W, "->TRICE: ")
		// decoder.Dump(p.W, p.B)
	}

	if cipher.Password != "" { // encrypted
		cipher.Decrypt(p.B, p.B)
		if decoder.DebugOut { // Debug output
			fmt.Print("->DEC: ")
			decoder.Dump(os.Stdout, p.B)
			//fmt.Fprint(p.W, "-> DEC:  ")
			//decoder.Dump(p.W, p.B)
		}
	}
}
*/

/*
func (p *trexDec) removeZeroHiByte(s []byte) (r []byte) {
	// The package interpreter does not know the number of padding zeroes, so it needs to discard them one by one.
	// If they are not zero, this is an error.
	switch p.Endian {
	case decoder.BigEndian:
		// Big endian case: 00 00 AA AA C0 00 -> 00 AA AA C0 00 -> still typeX0 -> AA AA C0 00 -> ok next package
		if s[0] != 0 {
			fmt.Println("unexpected case 273740233487", string(s))
		}
		r = s[1:]
	case decoder.LittleEndian:
		// Little endian case: 00 00 AA AA C0 00 -> 00 AA AA C0 00 -> AA00 signals a valid Trice, but it is not! -> We need to remove the HI byte!
		if s[1] != 0 {
			//log.Fatal("unexpected case", s)
			// todo: This needs to be disabled for successfully running all test cases.
			// BUT: deferred package framing NONE does not work
		}
		r = append(s[:1], s[2:]...)
	default:
		fmt.Println("unexpected case 927346193377", string(s))
	}
	return
}
*/


//nc := p.ReadU16(p.B) // n = number of data bytes (without timestamp), most significant bit is the count encoding, c = cycle

//decoder.TargetTimestamp = uint64(p.ReadU32(p.B))

//		/* TODO
//	default: //case typeX0: // extended trice type X0
//		len := triceID
//		if len == 0 {
//			// We assume padding zeroes here. The user print, if active, should not transmit buffers with len 0.
//			// p.B = p.removeZeroHiByte(p.B0)
//			return
//		}
//		decoder.TargetTimestampSize = 0
//		switch TypeX0Handler {
//		case "countedString":
//			//n += copy(b[n:], "vintage:")
//			n += copy(b[n:], fmt.Sprintln(string(p.B[:len])))
//			p.B = p.B[len:]
//			return
//		case "noTypeX0Handler":
//			if p.packageFraming == packageFramingNone {
//				if decoder.Verbose {
//					n += copy(b[n:], fmt.Sprintln("wrn:\aTo try to resync removing zero HI byte from:"))
//					n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B0)))
//				}
//				p.B = p.removeZeroHiByte(p.B0)
//				return
//			}
//			n += copy(b[n:], fmt.Sprintln("ERROR:\aUnexpected triceType typeX0"))
//			fallthrough
//		default:
//			n += copy(b[n:], fmt.Sprintln("ERROR:\aNo handler for triceType typeX0"))
//			n += copy(b[n:], fmt.Sprintln("wrn:We can reach here in target TRICE_MULTI_PACK_MODE, when a trice message is followed by several zeroes (up to 7 possible with encryption)."))
//			p.B = p.removeZeroHiByte(packed)
//			return
//		}	return true

// NextTriceComplete returns true, if in p.B next Trice is complete including optional following padding bytes.
//  func (p *trexDec) NextTriceComplete() bool {
//  	return false
//  }

/*
// InterpretBuffer analyzes next Trices in buffer p.B, returs the result in b[:n] and removes the interpreted bytes from p.B including optional 0-7 padding bytes.
// Hint: In unframed32 and unframed64 the MULTI_PACK_MODE is forbidden, because we would not know if a 0-byte in front of | is an alignment byte or part of next Trice.
// unframed32 (unframed64) Examples, legend: z = 0-3(7) padding zeroes, x = 0-3(7) at least one !=0 bytes, | = alignment border:
// |TriceA | A Trice is complete, but ends not at an | and z not arrived yet: return to wait for z.
// |TriceAz| A Trice is complete with z: Interpret and remove.
// |TriceAx| Report error.
func (p *trexDec) InterpretBuffer(b []byte) (n int, err error) {
	switch p.packageFraming {
	case packageFramingCOBS, packageFramingTCOBS, packageFramingTCOBSv2:
		// framed:     Any         Trices, 0-7 padding bytes allowed at buffer end, current p.B buffer end consistent.

	case packageFramingNone8:
		// unframed8:  Any         Trices,  no padding bytes allowed,               current p.B buffer end open. Unaligned Trices are ok.

	case packageFramingNone32:
		// unframed32: Any aligned Trices, 0-3 padding bytes allowed at Trice  end, current p.B buffer end open. Unaligned Trices are an error.
	case packageFramingNone64:
		// unframed64: Any aligned Trices, 0-7 padding bytes allowed at Trice  end, current p.B buffer end open. Unaligned Trices are an error.
	case packageFramingNone0:
	default:
		log.Fatal("Invalid framing switch:\a", p.packageFraming)
		return
	}
}
*/

/*
// Read is the provided read method for TREX decoding and provides next string as byte slice.
//
// It uses inner reader p.In and internal id look-up table to fill b with a string.
// b is a slice of bytes with a len for the max expected string size.
// n is the count of read bytes inside b.
// Read returns usually one complete trice string or nothing but can return concatenated
// trice strings, each ending with a newline despite the last one, when messages added.
// Read does not process all internally read complete trice packages to be able later to
// separate Trices within one line to keep them separated for color processing.
// Therefore, Read needs to be called cyclically even after returning io.EOF to process internal data.
// When Read returns n=0, all processable complete trice packages are done,
// but the start of a following trice package can be already inside the internal buffer.
// In case of a not matching cycle, a warning message in trice format is prefixed.
// In case of invalid package data, error messages in trice format are returned and the package is dropped.
func (p *trexDec) Read(b []byte) (n int, err error) {

	switch p.packageFraming {
	case packageFramingNone, packageFramingNone0:
		p.nextData() // returns all unprocessed data inside p.B
		return p.InterpretUnframedData0(b)
	case packageFramingNone8:
		p.nextData() // returns all unprocessed data inside p.B
		return p.InterpretUnframedData8(b)
	case packageFramingNone32:
		p.nextData() // returns all unprocessed data inside p.B
		return p.InterpretUnframedData32(b)
	case packageFramingNone64: // XTEA is active
		p.nextData() // returns all unprocessed data inside p.B
		return p.InterpretUnframedData64(b)
	case packageFramingCOBS:
		p.nextFrame()
		return p.InterpretCobsFramedData(b)
	case packageFramingTCOBS: //v1
		p.nextFrame()
		return p.InterpretTCobsFramedData(b)
	case packageFramingTCOBSv2: //v2
		p.nextFrame()
		return p.InterpretTCobsV2FramedData(b)

	}
	if p.packageFraming == packageFramingNone {
		p.nextData() // returns all unprocessed data inside p.B
		p.B0 = p.B   // keep data for re-sync
		return p.InterpretUnframedData(b)
	} else {
		if cipher.Password != "" && len(p.B) < 8 && allZero(p.B) {
			if decoder.Verbose {
				fmt.Println("Discard", len(p.B), "trailing zeroes. ATTENTION: incomplete trice messages containing many zeroes could be problematic here!")
			}
			p.B = p.B[:0]
		}
		//if len(p.B) == 1 { // last decoded package exhausted
		//	if decoder.Verbose {
		//		fmt.Println("Inconsistent data, discarding last single byte", p.B[0], "from:")
		//		fmt.Println(hex.Dump(p.B))
		//	}
		//	p.B = p.B[:0]
		//}
		//if len(p.B) == 0 {
		//	fmt.Println("--> last decoded package exhausted")
		p.nextFrame() // returns one decoded package inside p.B
		//}
	}
	decoder.Verbose = true
	packageSize := len(p.B)
	switch packageSize {
	case 0:
		if decoder.Verbose {
			fmt.Println("--> package size zero")
		}
		time.Sleep(100 * time.Millisecond)
		decoder.Verbose = false
		return
	case 1:
		if p.B[0] == 0 {
			p.B = p.B[:0]
			if decoder.Verbose {
				fmt.Println("--> discard 1 padding zero")
			}
		} else {
			if decoder.Verbose {
				fmt.Printf("--> %02x in p.B", p.B[0])
			}
		}
		decoder.Verbose = false
		return
	case 2:
		if p.B[0] == 0 && p.B[1] == 0 {
			p.B = p.B[:0]
			if decoder.Verbose {
				fmt.Println("--> discard 2 padding zeroes")
			}
		} else {
			if decoder.Verbose {
				fmt.Printf("--> %02x %02x in p.B", p.B[0], p.B[1])
			}
		}
		decoder.Verbose = false
		return
	case 3:
		if p.B[0] == 0 && p.B[1] == 0 && p.B[2] == 0 {
			p.B = p.B[:0]
			if decoder.Verbose {
				fmt.Println("--> discard 3 padding zeroes")
			}
		} else {
			if decoder.Verbose {
				fmt.Printf("--> %02x %02x %02x in p.B", p.B[0], p.B[1], p.B[2])
			}
		}
		decoder.Verbose = false
		return
	}

	packed := p.B
	tyId := p.ReadU16(p.B)
	p.B = p.B[tyIdSize:]

	triceType := int(tyId >> decoder.IDBits) // most significant bit are the triceType
	triceID := id.TriceID(0x3FFF & tyId)     // 14 least significant bits are the ID
	decoder.LastTriceID = triceID            // used for showID
	decoder.RecordForStatistics(triceID)     // This is for the "trice log -stat" flag

	switch triceType {
	case typeS0: // no stamp
		decoder.TargetTimestampSize = 0
		if packageSize < tyIdSize+decoder.TargetTimestampSize+ncSize { // for non typeEX trices
			return // not enough data
		}
		decoder.TargetTimestamp = 0
	case typeS2: // 16-bit stamp
		decoder.TargetTimestampSize = 2
		if Doubled16BitID { // p.packageFraming == packageFramingNone || cipher.Password != "" {
			if len(p.B) < 2 {
				return // wait for more data
			}

			// Without encoding it needs to be done here.
			// Also encrypted trice messages carry a double 16-bit ID.
			p.B = p.B[tyIdSize:] // When target encoding is done, it removes the double 16-bit ID at the 16-bit timestamp trices.
		}
		if packageSize < tyIdSize+decoder.TargetTimestampSize+ncSize { // for non typeEX trices
			return // not enough data
		}
		decoder.TargetTimestamp = uint64(p.ReadU16(p.B))
	case typeS4: // 32-bit stamp
		decoder.TargetTimestampSize = 4
		if packageSize < tyIdSize+decoder.TargetTimestampSize+ncSize { // for non typeEX trices
			return // not enough data
		}
		decoder.TargetTimestamp = uint64(p.ReadU32(p.B))

	default: //case typeX0: // extended trice type X0
		len := triceID
		if len == 0 {
			// We assume padding zeroes here. The user print, if active, should not transmit buffers with len 0.
			p.B = p.removeZeroHiByte(p.B0)
			return
		}
		decoder.TargetTimestampSize = 0
		switch TypeX0Handler {
		case "countedString":
			//n += copy(b[n:], "vintage:")
			n += copy(b[n:], fmt.Sprintln(string(p.B[:len])))
			p.B = p.B[len:]
			return
		case "noTypeX0Handler":
			if p.packageFraming == packageFramingNone {
				if decoder.Verbose {
					n += copy(b[n:], fmt.Sprintln("wrn:\aTo try to resync removing zero HI byte from:"))
					n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B0)))
				}
				p.B = p.removeZeroHiByte(p.B0)
				return
			}
			n += copy(b[n:], fmt.Sprintln("ERROR:\aUnexpected triceType typeX0"))
			fallthrough
		default:
			n += copy(b[n:], fmt.Sprintln("ERROR:\aNo handler for triceType typeX0"))
			n += copy(b[n:], fmt.Sprintln("wrn:We can reach here in target TRICE_MULTI_PACK_MODE, when a trice message is followed by several zeroes (up to 7 possible with encryption)."))
			p.B = p.removeZeroHiByte(packed)
			return
		}
	}

	// try to interpret

	p.B = p.B[decoder.TargetTimestampSize:]
	if len(p.B) < 2 {
		return // wait for more data
	}
	nc := p.ReadU16(p.B) // n = number of data bytes (without timestamp), most significant bit is the count encoding, c = cycle
	p.B = p.B[ncSize:]

	var cycle uint8
	if nc>>15 == 1 { // special case: more than data 127 bytes
		// C code: #define TRICE_LCNT(count) TRICE_PUT16( (0x8000 | (count)) );
		cycle = p.expectedCycle                 // cycle is not transmitted, so set expected value
		p.ParamSpace = int(0x7FFF & nc) // 15 bit for data byte count excluding timestamp
	} else {
		// C code: #define TRICE_CNTC(count) TRICE_PUT16( ((count)<<8) | TRICE_CYCLE )
		cycle = uint8(nc)           // low byte is cycle
		p.ParamSpace = int(nc >> 8) // high byte is 7 bit number of bytes for data count excluding timestamp
	}

	p.TriceSize = tyIdSize + decoder.TargetTimestampSize + ncSize + p.ParamSpace

	// Here, when p.TriceSize < packageSize and pf!=none

	if p.TriceSize > packageSize { //  '>' for multiple trices in one package (case TriceOutMultiPackMode), todo: discuss all possible variants
		if p.packageFraming == packageFramingNone {
			if decoder.Verbose {
				n += copy(b[n:], fmt.Sprintln("wrn:\adiscarding first byte", p.B0[0], "from:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B0)))
			}
			p.B0 = p.B0[1:] // discard first byte and try again
			p.B = p.B0
			return
		}
		if decoder.Verbose {
			n += copy(b[n:], fmt.Sprintln("ERROR:\apackage size", packageSize, "is <", p.TriceSize, " - ignoring package:"))
			n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B)))
			n += copy(b[n:], fmt.Sprintln("tyIdSize=", tyIdSize, "tsSize=", decoder.TargetTimestampSize, "ncSize=", ncSize, "ParamSpae=", p.ParamSpace))
			n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		}
		p.B = p.B[len(p.B):] // discard buffer
	}
	if SingleFraming && p.TriceSize != packageSize {
		if decoder.Verbose {
			n += copy(b[n:], fmt.Sprintln("ERROR:\asingle framed package size", packageSize, "is !=", p.TriceSize, " - ignoring package:"))
			n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B)))
			n += copy(b[n:], fmt.Sprintln("tyIdSize=", tyIdSize, "tsSize=", decoder.TargetTimestampSize, "ncSize=", ncSize, "ParamSpae=", p.ParamSpace))
			n += copy(b[n:], fmt.Sprintln(decoder.Hints))
		}
		p.B = p.B[len(p.B):] // discard buffer
	}

	// cycle counter automatic & check
	if cycle == 0xc0 && p.expectedCycle != 0xc0 && decoder.InitialCycle { // with cycle counter and seems to be a target reset
		n += copy(b[n:], fmt.Sprintln("warning:\a   Target Reset?   "))
		p.expectedCycle = cycle + 1 // adjust cycle
		decoder.InitialCycle = false
	}
	if cycle == 0xc0 && p.expectedCycle != 0xc0 && !decoder.InitialCycle { // with cycle counter and seems to be a target reset
		//n += copy(b[n:], fmt.Sprintln("info:   Target Reset?   ")) // todo: This line is ok with cycle counter but not without cycle counter
		p.expectedCycle = cycle + 1 // adjust cycle
	}
	if cycle == 0xc0 && p.expectedCycle == 0xc0 && decoder.InitialCycle { // with or without cycle counter and seems to be a target reset
		//n += copy(b[n:], fmt.Sprintln("warning:   Restart?   "))
		p.expectedCycle = cycle + 1 // adjust cycle
		decoder.InitialCycle = false
	}
	if cycle == 0xc0 && p.expectedCycle == 0xc0 && !decoder.InitialCycle { // with or without cycle counter and seems to be a normal case
		p.expectedCycle = cycle + 1 // adjust cycle
	}
	if cycle != 0xc0 && !DisableCycleErrors { // with cycle counter and s.th. lost
		if cycle != p.expectedCycle { // no cycle check for 0xc0 to avoid messages on every target reset and when no cycle counter is active
			n += copy(b[n:], fmt.Sprintln("CYCLE_ERROR:\a", cycle, "!=", p.expectedCycle, " (count=", emitter.TagEvents("CYCLE_ERROR")+1, ")"))
			n += copy(b[n:], "                                         ") // len of location information plus stamp: 41 spaces - see NewlineIndent below - todo: make it generic
			p.expectedCycle = cycle                                               // adjust cycle
		}
		decoder.InitialCycle = false
		p.expectedCycle++
	}

	var ok bool
	p.LutMutex.RLock()
	p.Trice, ok = p.Lut[triceID]
	if AddNewlineToEachTriceMessage {
		p.Trice.Strg += `\n` // this adds a newline to each single Trice message
	}
	p.LutMutex.RUnlock()
	if !ok {
		if p.packageFraming == packageFramingNone {
			if decoder.Verbose {
				n += copy(b[n:], fmt.Sprintln("wrn:\adiscarding first byte", p.B0[0], "from:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B0)))
			}
			p.B0 = p.B0[1:] // discard first byte and try again
			p.B = p.B0
		} else {
			n += copy(b[n:], fmt.Sprintln("WARNING:\aunknown ID ", triceID, "- ignoring trice ending with:"))
			n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B)))
			n += copy(b[n:], fmt.Sprintln(decoder.Hints))
			p.B = p.B[:0] // discard all
		}
		return
	}

	n += p.sprintTrice(b[n:]) // use param info
	if len(p.B) < p.ParamSpace {
		if p.packageFraming == packageFramingNone {
			if decoder.Verbose {
				n += copy(b[n:], fmt.Sprintln("wrn:discarding first byte", p.B0[0], "from:"))
				n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B0)))
			}
			p.B0 = p.B0[1:] // discard first byte and try again
			p.B = p.B0
		} else {
			n += copy(b[n:], fmt.Sprintln("ERROR:ignoring data garbage:"))
			n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B)))
			n += copy(b[n:], fmt.Sprintln(decoder.Hints))
			p.B = p.B[:0] // discard all
		}
	} else {
		if p.packageFraming != packageFramingNone { // COBS | TCOBS are exact
			//p.B=p.B[:0] // This is WRONG! There could be more Trices in p.B
			p.B = p.B[p.ParamSpace:] // drop param info HERE we drop one of 3 zero bytes
			// if len(p.B) > 0 {
			// 	fmt.Println(p.ParamSpace, p.B )
			// 	p.B = p.B[:0]
			// }
		} else { // no package framing
			padding := (p.ParamSpace + 3) & ^3
			if padding <= len(p.B) {
				p.B = p.B[padding:] // drop param info with padding bytes
				// When no packagge framing and multi pack mode, there are 0-3 padding zero bytes after each Trice.
			} else {
				p.B = p.B[p.ParamSpace:] // drop param info
				// n += copy(b[n:], fmt.Sprintln("wrn: cannot discard padding bytes", ))
			}
		}
	}
	return
}
*/

	//  if len(p.B) < p.ParamSpace {
	//  	if p.packageFraming == packageFramingNone {
	//  		if decoder.Verbose {
	//  			n += copy(b[n:], fmt.Sprintln("wrn:discarding first byte", p.B0[0], "from:"))
	//  			n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B0)))
	//  		}
	//  		p.B0 = p.B0[1:] // discard first byte and try again
	//  		p.B = p.B0
	//  	} else {
	//  		n += copy(b[n:], fmt.Sprintln("ERROR:ignoring data garbage:"))
	//  		n += copy(b[n:], fmt.Sprintln(hex.Dump(p.B)))
	//  		n += copy(b[n:], fmt.Sprintln(decoder.Hints))
	//  		p.B = p.B[:0] // discard all
	//  	}
	//  } else {
	//  	if p.packageFraming != packageFramingNone { // COBS | TCOBS are exact
	//  		//p.B=p.B[:0] // This is WRONG! There could be more Trices in p.B
	//  		p.B = p.B[p.ParamSpace:] // drop param info HERE we drop one of 3 zero bytes
	//  		// if len(p.B) > 0 {
	//  		// 	fmt.Println(p.ParamSpace, p.B )
	//  		// 	p.B = p.B[:0]
	//  		// }
	//  	} else { // no package framing
	//  		padding := (p.ParamSpace + 3) & ^3
	//  		if padding <= len(p.B) {
	//  			p.B = p.B[padding:] // drop param info with padding bytes
	//  			// When no packagge framing and multi pack mode, there are 0-3 padding zero bytes after each Trice.
	//  		} else {
	//  			p.B = p.B[p.ParamSpace:] // drop param info
	//  			// n += copy(b[n:], fmt.Sprintln("wrn: cannot discard padding bytes", ))
	//  		}
	//  	}
	//  }

	/*
// InterpretUnframedData0 analyzes next Trice in compact or aligned p.B, returs the result in b[:n] and removes the interpreted bytes from p.B including optional adding bytes.
// If not enough data in p.B including optional padding bytes nothing happens and n=0, nil is returned.
// It tries to detect if the data stream is compact or aligned and sets p.packageFraming accordingly.
func (p *trexDec) InterpretUnframedData0(b []byte) (n int, err error) {
	// todo
	return
}

// InterpretUnframedData8 analyzes next Trice in compact buffer p.B, returs the result in b[:n] and removes the interpreted bytes from p.B.
// If not enough data in p.B nothing happens and n=0, nil is returned.
func (p *trexDec) InterpretUnframedData8(b []byte) (n int, err error) {
	for len(p.B) > 0 {
		if !p.TriceComplete() {
			return
		}
	}
	n, err = p.printTrice(b)
	if
	p.B = p.B[p.ntlen:]
	return
}

// InterpretUnframedData32 analyzes next Trice in 32-bit aligned buffer p.B, returs the result in b[:n] and removes the interpreted bytes from p.B including optional 0-3 adding bytes.
// If not enough data in p.B including optional padding bytes nothing happens and n=0, nil is returned.
func (p *trexDec) InterpretUnframedData32(b []byte) (n int, err error) {
	if !p.TriceComplete() {
		return
	}
	n, err = p.printTrice(b)
	//p.B = p.B[p.ntlen:]
	//
	//switch len(p.B){
	//case 0:
	//	return
	//case
	//
	//}

	return
}

// InterpretUnframedData64 analyzes next Trice in 64-bit aligned buffer p.B, returs the result in b[:n] and removes the interpreted bytes from p.B including optional 0-7 adding bytes.
// If not enough data in p.B including optional padding bytes nothing happens and n=0, nil is returned.
func (p *trexDec) InterpretUnframedData64(b []byte) (n int, err error) {
	if !p.TriceComplete() {
		return
	}
	return
}
*/
