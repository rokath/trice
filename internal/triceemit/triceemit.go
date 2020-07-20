// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package emit is responsible for generating a string slice for each line.
// The substrings are optionally prefix, timestamp, several content substrings and postfix.
// Each substring can contain its own color channel as prefix ("col:").
// The colors are converted later inside the disp.Print() function.


// TriceAtomReader is interface needed
type TriceReader interface{
	func TriceRead( []Trice) (int, error)
}

//type TriceReadFunc func Read( []Trice) (int, error)

// TriceAtom is the bare trice data type
//
// A trice consists of one or several TriceAtom(s).
// It has only one ID. Te other trice atoms have ID==0 and only additional data bytes. 
type Trice struct {
	ID    uint16 // 2^16 ^= 65536 different trice IDs possible
	Value [2]byte // max 2 byte data payload inside a TriceAtom
}



// type LineWriter interface{
// 	func LineWrite( s []string ) (int, error)
// }



type Receiver struct{
	r io.Reader // inner byte reader 
	Read TriceReader // 
	//list id.ListT 
	//w io.Writer // byte writer interface for strings
	//item Bare // trice item
	//data []byte // data collector
}

// AtomRead uses the underlying byte reader
(p *Trice)RawRead() []Trice) (int, error){

}

// AtomRead uses the underlying byte reader
(p *Trice)WrapRead() []Trice) (int, error){

}

func NewBareReceiver( r io.Reader) *Receiver{
	p := &Receiver{}
	Read = RawRead
	p.r = r 
	return p
}

func NewWrapReceiver( r io.Reader) *Receiver{
	p := &Receiver{}
	Read = WrapRead
	p.r = r 
	return p
}


// func io.WriteString(w Writer, s string) (n int, err error)

func x(){
	var r io.Reader 
	p := NewBareReader( r )
	n,e := p.Read([]Bare)
}