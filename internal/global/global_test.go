// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package global_test is a blackbox test
package global_test

/*
func TestEvaluateWrap(t *testing.T) {
	// check start byte
	b := []byte{0xe0, global.SourceAddr, global.SourceAddr, 0xe0, 1, 1, 2, 2}
	x := global.EvaluateWrap(b)
	lib.Equals(t, false, x)

	// check crc
	b = []byte{0xc0, global.SourceAddr, global.SourceAddr, 0xe0, 1, 1, 2, 2}
	x = global.EvaluateWrap(b)
	lib.Equals(t, false, x)

	// check ok
	b = []byte{0xc0, global.SourceAddr, global.SourceAddr, 0xc0, 1, 1, 2, 2}
	x = global.EvaluateWrap(b)
	lib.Equals(t, true, x)
}

func TestCheck(t *testing.T) {
	err := errors.New("an error")
	b := global.Check(err)
	lib.Equals(t, true, b)
	c := global.Check(nil)
	lib.Equals(t, false, c)
}
*/
