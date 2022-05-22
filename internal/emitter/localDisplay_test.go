// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// white-box test for package emitter.
package emitter

import "os"

func Example_newLocalDisplay() {
	p := newLocalDisplay(os.Stdout)
	l1 := []string{"This is ", "the 1st ", "line"}
	l2 := []string{"This is ", "the 2nd ", "line"}
	p.WriteLine(l1)
	p.WriteLine(l2)
	// Output:
	// This is the 1st line
	// This is the 2nd line
}

func Example_newColorDisplay() {
	p := newColorDisplay(os.Stdout, "none")
	l1 := []string{"msg:This is ", "the 1st ", "line"}
	l2 := []string{"MSG:This is ", "the 2nd ", "line"}
	p.WriteLine(l1)
	p.WriteLine(l2)
	// Output:
	// This is the 1st line
	// MSG:This is the 2nd line
}
