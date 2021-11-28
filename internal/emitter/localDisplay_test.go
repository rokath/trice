// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import "os"

func ExampleNewLocalDisplay() {
	p := NewLocalDisplay(os.Stdout)
	l1 := []string{"This is ", "the 1st ", "line"}
	l2 := []string{"This is ", "the 2nd ", "line"}
	p.writeLine(l1)
	p.writeLine(l2)
	// Output:
	// This is the 1st line
	// This is the 2nd line
}

func ExampleNewColorDisplay() {
	p := NewColorDisplay(os.Stdout, "none")
	l1 := []string{"msg:This is ", "the 1st ", "line"}
	l2 := []string{"MSG:This is ", "the 2nd ", "line"}
	p.writeLine(l1)
	p.writeLine(l2)
	// Output:
	// This is the 1st line
	// MSG:This is the 2nd line
}
