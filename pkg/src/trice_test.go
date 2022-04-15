package src

import (
	"fmt"
	"io"
	"testing"

	"github.com/tj/assert"
)

// TRICE32_1( Id(58755), "rd:TRICE32_1 line %d (%%d)\n", -1 );
//
//  "58755": {
//  	"Type": "TRICE32_1",
//  	"Strg": "rd:TRICE32_1 line %d (%%d)\\n"
//  },
//
// []byte{0x02, 0x03, 0x01, 0x01, 0x02, 0x1a, 0x0f, 0x37, 0xcb, 0x11, 0x11, 0x11, 0x11, 0xc0, 0x01, 0x83, 0xe5, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00}
//
// "rd:TRICE32_1 line -1 (%d)\n"

// dump prints the byte slice as hex in one line
func dump(w io.Writer, b []byte) {
	fmt.Fprint(w, "exp := []byte{ ")
	for _, x := range b {
		fmt.Fprintf(w, "0x%02x, ", x)
	}
	fmt.Fprintln(w, "}")
}

func TestN(t *testing.T) {
	act := make([]byte, 1024)
	setTriceBuffer(act)

	for _, x := range triceTests {
		act = act[:x.tfn()]
		//dump(os.Stdout, act)
		assert.Equal(t, x.exp, act)
	}

}
