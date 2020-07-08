// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for decoder
package decoder

import (
	"bytes"
	"testing"

	"github.com/rokath/trice/internal/bare"
	"github.com/rokath/trice/pkg/lib"
)

func Test1_stringsFromASCIIDecode(t *testing.T) {
	r := bytes.NewReader([]byte("m:Hello\nyou\t\a\a\asig:Gophers!\r\n"))
	enc := []string{"ascii"}
	var (
		trice   chan []bare.Item
		command chan Command
		dropped chan byte
		err     chan error
	)
	ascii := make(chan string, 2)

	p, e := New(r, enc, trice, command, ascii, dropped, err)
	lib.Equals(t, nil, e)
	p.decode()
	var act []string
	count := 2
	for count > 0 {
		select {
		case s := <-ascii:
			act = append(act, s)
		case <-trice:
			t.Fail()
		case <-command:
			t.Fail()
		case <-dropped:
			t.Fail()
		case <-err:
			t.Fail()
		}
		count--
	}
	exp := []string{"m:Hello", "you\t\a\a\asig:Gophers!"}
	lib.Equals(t, exp, act)
}

func Test1_bareDecode(t *testing.T) {
	r := bytes.NewReader([]byte{'j', 'a', 'r', 1, 1, 1, 1, 0x16, 0x16, 0x16, 0x16, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4})
	enc := []string{"bare"}
	var (
		ascii   chan string
		command chan Command
		dropped chan byte
		err     chan error
	)
	trice := make(chan []bare.Item, 200)
	p, e := New(r, enc, trice, command, ascii, dropped, err)
	lib.Equals(t, nil, e)
	p.decode()
	var act []bare.Item
	select {
	case act = <-trice:
	case <-ascii:
		t.Fail()
	case <-command:
		t.Fail()
	case <-dropped:
		t.Fail()
	case <-err:
		t.Fail()
	}
	exp := []bare.Item{
		{ID: 0x0101, Value: [2]byte{0x01, 0x01}},
		{ID: 0x1616, Value: [2]byte{0x16, 0x16}},
		{ID: 0x0202, Value: [2]byte{0x02, 0x02}},
		{ID: 0x0303, Value: [2]byte{0x03, 0x03}},
	}
	lib.Equals(t, exp, act)
}

func Test1_wrapDecode(t *testing.T) {
	r := bytes.NewReader([]byte{'j', 'a', 'r', 0xC0, 0x60, 0x60, 0xC0, 1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 0xC0, 0x60, 0x60, 0xC0, 8, 8, 5, 5})
	enc := []string{"wrap"}
	var (
		ascii   chan string
		command chan Command
		dropped chan byte
		err     chan error
	)
	trice := make(chan []bare.Item, 200)
	p, e := New(r, enc, trice, command, ascii, dropped, err)
	lib.Equals(t, nil, e)
	p.decode()
	var act []bare.Item
	select {
	case act = <-trice:
	case <-ascii:
		t.Fail()
	case <-command:
		t.Fail()
	case <-dropped:
		t.Fail()
	case <-err:
		t.Fail()
	}
	exp := []bare.Item{
		{ID: 0x0101, Value: [2]byte{0x02, 0x02}},
		{ID: 0x0808, Value: [2]byte{0x05, 0x05}},
	}
	lib.Equals(t, exp, act)
}
