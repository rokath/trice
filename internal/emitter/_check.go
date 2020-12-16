// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// The check functions are used not only for testing but also for evaluating id lists.

// package emitter

/*

// checkValuePosition is a consistency check for positive values and their position.
// It should be moved to the tests.
func checkValuePosition(l id.ListT, s []byte) error {
	var b []byte // check data
	for i := range l {
		it := l[i]
		switch it.FmtType {
		case "TRICE0":
			b = append(b, 0, 0) // 2 bytes padding like inside Trace()
		case "TRICE8_1":
			b = append(b, 1, 0) // 1 byte padding like inside Trace()
		case "TRICE8_2":
			b = append(b, 1, 2)
		case "TRICE8_3":
			b = append(b, 1, 2, 3, 0) // 1 byte padding like inside Trace()
		case "TRICE8_4":
			b = append(b, 1, 2, 3, 4)
		case "TRICE8_5":
			b = append(b, 1, 2, 3, 4, 5, 0) // 1 byte padding like inside Trace()
		case "TRICE8_6":
			b = append(b, 1, 2, 3, 4, 5, 6)
		case "TRICE8_7":
			b = append(b, 1, 2, 3, 4, 5, 6, 7, 0) // 1 byte padding like inside Trace()
		case "TRICE8_8":
			b = append(b, 1, 2, 3, 4, 5, 6, 7, 8)
		case "TRICE16_1":
			b = append(b, 1, 0)
		case "TRICE16_2":
			b = append(b, 1, 0, 2, 0)
		case "TRICE16_3":
			b = append(b, 1, 0, 2, 0, 3, 0)
		case "TRICE16_4":
			b = append(b, 1, 0, 2, 0, 3, 0, 4, 0)
		case "TRICE32_1":
			b = append(b, 1, 0, 0, 0)
		case "TRICE32_2":
			b = append(b, 1, 0, 0, 0, 2, 0, 0, 0)
		case "TRICE32_3":
			b = append(b, 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0)
		case "TRICE32_4":
			b = append(b, 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0)
		case "TRICE64_1":
			b = append(b, 1, 0, 0, 0, 0, 0, 0, 0)
		case "TRICE64_2":
			b = append(b, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0)
		}
		if "%s" == it.FmtStrg {
			b[0] = s[6] // set simulated pix value equal
		}
		s, _, err := Emitter(it, b, s)
		if nil != err {
			return err
		}
		b = b[:0] // empty d for next trice
		LineCollect(s)
	}
	return nil
}

// checkNegativeValues is a consistency check for negative values.
// It should be moved to the tests.
func checkNegativeValues(l id.ListT, s []byte) error {
	var b []byte // check data
	for i := range l {
		it := l[i]
		switch it.FmtType {
		case "TRICE0":
			b = append(b, 0, 0) // 2 bytes padding like inside Trace()
		case "TRICE8_1":
			b = append(b, 0x80, 0) // 1 byte padding like inside Trace()
		case "TRICE8_2":
			b = append(b, 0x80, 0x80)
		case "TRICE8_3":
			b = append(b, 0x80, 0x80, 0x80, 0) // 1 byte padding like inside Trace()
		case "TRICE8_4":
			b = append(b, 0x80, 0x80, 0x80, 0x80)
		case "TRICE8_5":
			b = append(b, 0x80, 0x80, 0x80, 0x80, 0x80, 0) // 1 byte padding like inside Trace()
		case "TRICE8_6":
			b = append(b, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80)
		case "TRICE8_7":
			b = append(b, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0) // 1 byte padding like inside Trace()
		case "TRICE8_8":
			b = append(b, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80)
		case "TRICE16_1":
			b = append(b, 0, 0x80)
		case "TRICE16_2":
			b = append(b, 0, 0x80, 0x00, 0x80)
		case "TRICE16_3":
			b = append(b, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80)
		case "TRICE16_4":
			b = append(b, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80)
		case "TRICE32_1":
			b = append(b, 0, 0, 0, 0x80)
		case "TRICE32_2":
			b = append(b, 0, 0, 0, 0x80, 0, 0, 0, 0x80)
		case "TRICE32_3":
			b = append(b, 0, 0, 0, 0x80, 0, 0, 0, 0x80, 0, 0, 0, 0x80)
		case "TRICE32_4":
			b = append(b, 0, 0, 0, 0x80, 0, 0, 0, 0x80, 0, 0, 0, 0x80, 0, 0, 0, 0x80)
		case "TRICE64_1":
			b = append(b, 0, 0, 0, 0, 0, 0, 0, 0x80)
		case "TRICE64_2":
			b = append(b, 0, 0, 0, 0, 0, 0, 0, 0x80, 0, 0, 0, 0, 0, 0, 0, 0x80)
		}
		if "%s" == it.FmtStrg {
			b[0] = s[6] // set simulated pix value equal
		}
		s, _, err := Emitter(it, b, s)
		if nil != err {
			return err
		}
		b = b[:0] // empty d for next trice
		LineCollect(s)
	}
	return nil
}

// checkFix is a consistency check for fixed values.
// It should NOT be moved to the tests because it is helpful for til.json checks.
func checkFix(l id.ListT, s []byte) error {
	b := []byte{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'} // dummy data
	for i := range l {
		it := l[i]
		switch it.FmtType {
		case "TRICE0", "TRICE8_1", "TRICE8_2", "TRICE16_1":
			b = b[:2]
		case "TRICE8_3", "TRICE8_4", "TRICE16_2", "TRICE32_1":
			b = b[:4]
		case "TRICE8_5", "TRICE8_6", "TRICE16_3":
			b = b[:6]
		case "TRICE8_7", "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
			b = b[:8]
		case "TRICE32_3":
			b = b[:12]
		case "TRICE32_4", "TRICE64_2":
			b = b[:16]
		}
		if "%s" == it.FmtStrg {
			b[0] = s[6] // set simulated pix value equal
		}
		s, _, err := Emitter(it, b, s)
		if nil != err {
			return err
		}
		b = b[:0] // empty d for next trice
		LineCollect(s)
	}
	return nil
}

// Check evaluates the l output with specifiesd dataSet.
func Check(l id.ListT, dataSet string) error {
	s := make([]byte, 0, 12)
	// this is a dummy buffer for a simulated runtime string "RS" with pix = 1 (for easy crc8)
	s = append(s, 0xc0, 0, 0, 0xc0, 0xff, 0xff, 1, 1, 1, 0, 'R', 'S')
	if "position" == dataSet {
		return checkValuePosition(l, s)
	}
	if "negative" == dataSet {
		return checkNegativeValues(l, s)
	}
	fmt.Println("dataset", dataSet, "unknown, using internal values")
	return checkFix(l, s)
}

// ScCheckList does log the id list with a dataset
func ScCheckList(dataset string) error {
	err := id.List.Read(id.FnJSON)
	if nil != err {
		id.ListNotFoundMsg(id.FnJSON)
		return nil
	}
	Check(id.List, dataset)
	return nil
}
*/
