package src

import (
	"testing"

	"github.com/tj/assert"
)

type t0 []struct {
	id  int
	fmt string
	b   []byte
}

// Cannot check cycle counter because of parallel test execution.

//  func Test_trice0i(t *testing.T) {
//  	tt := t0{
//  		{34481, "", []byte{0, 136, 53, 132}},
//  		{34481, "", []byte{1, 136, 53, 132}},
//  	}
//  	for _, x := range tt {
//  		b := trice0i(len(x.b), x.id, x.fmt)
//
//  		assert.Equal(t, b[1:], x.b[1:]) // ignore cycle counter here
//  	}
//  }

func _Test_trice0(t *testing.T) { // flexl
	tt := t0{
		{34481, "", []byte{0, 136, 53, 132}},
		{34481, "", []byte{1, 136, 53, 132}},
	}
	for _, x := range tt {
		b := trice0(len(x.b), x.id, x.fmt)
		assert.Equal(t, b[1:], x.b[1:]) // ignore cycle counter
	}
}

type t81 []struct {
	id  int
	fmt string
	b0  byte
	b   []byte
}

//  func Test_trice8_1i(t *testing.T) {
//  	tt := t81{
//  		{34481, "", 81, []byte{0, 137, 53, 132, 81, 0, 0, 0}},
//  		{34481, "", 81, []byte{1, 137, 53, 132, 81, 0, 0, 0}},
//  	}
//  	for _, x := range tt {
//  		b := trice8_1i(len(x.b), x.id, x.fmt, x.b0)
//  		assert.Equal(t, x.b[1:], b[1:]) // ignore cycle counter here
//  	}
//  }

func _Test_trice8_1(t *testing.T) { // flexl
	tt := t81{
		{34481, "", 81, []byte{0, 137, 53, 132, 81, 0, 0, 0}},
		{34481, "", 81, []byte{1, 137, 53, 132, 81, 0, 0, 0}},
	}
	for _, x := range tt {
		b := trice8_1(len(x.b), x.id, x.fmt, x.b0)
		assert.Equal(t, x.b[1:], b[1:]) // ignore cycle counter here
	}
}

type t82 []struct {
	id     int
	fmt    string
	b0, b1 byte
	b      []byte
}

//  func Test_trice8_2i(t *testing.T) {
//  	tt := t82{
//  		{34481, "", 81, 82, []byte{0, 138, 53, 132, 82, 81, 0, 0}},
//  		{34481, "", 81, 82, []byte{1, 138, 53, 132, 82, 81, 0, 0}},
//  	}
//  	for _, x := range tt {
//  		b := trice8_2i(len(x.b), x.id, x.fmt, x.b0, x.b1)
//  		assert.Equal(t, x.b[1:], b[1:]) // ignore cycle counter here
//  	}
//  }

func _Test_trice8_2(t *testing.T) { // flexl
	tt := t82{
		{34481, "", 81, 82, []byte{0, 138, 53, 132, 82, 81, 0, 0}},
		{34481, "", 81, 82, []byte{1, 138, 53, 132, 82, 81, 0, 0}},
	}
	for _, x := range tt {
		b := trice8_2(len(x.b), x.id, x.fmt, x.b0, x.b1)
		assert.Equal(t, x.b[1:], b[1:]) // ignore cycle counter here
	}
}
