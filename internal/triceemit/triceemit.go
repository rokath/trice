// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package triceemit is responsible for generating a string slice for each line.
// The substrings are optionally prefix, timestamp, several content substrings and postfix.
// Each substring can contain its own color channel as prefix ("col:").
// The colors are converted later inside the disp.Print() function.

package triceemit

const (
	// triceSize is the count of bytes in a byte stream used for one Trice.
	triceSize = 4

	// triceChannelCapacity is the max possible trice slice count hold in channel
	triceChannelCapacity = 10

	// ignoredChannelCapacity is the max count of ignored bytes
	ignoredChannelCapacity = 10
)

var (
	// Prefix is a (configurable) string added to each line at the beginning
	Prefix = ""

	// Postfix is a (configurable) string added to each line end
	Postfix = "\n"
)

// Trice is the bare Trice data type for a Trice atom.
// A Trice starts with zero or several Trice atoms with ID==0 carrying parts of the Trice data payload.
// The last Trice atom of a Trice contains the Trice ID!=0 and the last part of the data payload.
type Trice struct {
	ID    uint16  // 2^16 ^= more than 65500 different trice IDs possible
	Value [2]byte // max 2 byte data payload inside a TriceAtom
}

// TriceAtomsReceiver is the interface a trice receicer has to provide for a trice interpreter.
// The provided channels are read only channels
type TriceAtomsReceiver interface {
	TriceAtomsChannel() <-chan []Trice
	IgnoredBytesChannel() <-chan []byte
}

/*
func NewTriceInterpreter(r *TriceReceiver) {
	go func() {
		for {
			select {
			case atoms := <-r.atoms:
				p.atoms = append(p.atoms, atoms)
			case ignored := <-r.ignored:

			}
		}
	}()
	go func() {

		for {
			atomsSlice := <-r.atoms // next trice atoms slice
			// 			p.atomsBuffer = append(p.atomsBuffer, atomsSlice)
			// 			p.item.ID = int(trice.ID)
			// 			p.values = append(p.values, trice.Value[0], trice.Value[1])
			// 			//p.values = append(p.values, trice.Value...)
			fmt.Println(atomsSlice)

			if 0 < len(r.atoms) {
				atom := r.atoms[0]
				p.atoms = p.atoms[1:]
				if 0 == atom.ID {

				}
			}
		}
	}()
}
*/

// // TrimBackslashes handles special chars in s
// func TrimBackslashes(s string) string {
// 	s = strings.ReplaceAll(s, "\\a", "\a")
// 	s = strings.ReplaceAll(s, "\\b", "\b")
// 	s = strings.ReplaceAll(s, "\\t", "\t")
// 	s = strings.ReplaceAll(s, "\\n", "\n")
// 	s = strings.ReplaceAll(s, "\\f", "\f")
// 	s = strings.ReplaceAll(s, "\\r", "\r")
// 	s = strings.ReplaceAll(s, "\\v", "\v")
// 	// There four more formats to handle: (TODO)
// 	//   \x followed by exactly two hexadecimal digits,
// 	//   \ followed by exactly three octal digits,
// 	//   \u followed by exactly four hexadecimal digits,
// 	//   \U followed by exactly eight hexadecimal digits.
// 	return s
// }

// // LineCollect collects s into an internal line substring slice
// // When s ends with a newline it is trimmed and the slice goes to Out and is discarded afterwards
// func (p *TriceInterpreter) LineCollect(s string) {
// 	s = TrimBackslashes(s)
//
// 	a := func(su string) {
// 		p.css = append(p.css, su)
// 	}
// 	if 0 == len(p.css) {
// 		a(Prefix)
// 		a(timestamp())
// 	}
// 	if false == strings.HasSuffix(s, "\n") {
// 		a(s)
// 		return
// 	}
// 	s = strings.TrimSuffix(s, "\n")
// 	a(s)
// 	a(Postfix)
// 	//lineComplete = true
// 	//disp.Out(css)
// 	//css = css[:0] // discard slice data
// }

// // triceItem tries to create a full trice item from the receiver data.
// func (p *TriceReceiver) triceItem() {
// 	for 0 == p.item.ID {
// 		select {
// 		case atomsSlice := <-p.atoms: // next trice atoms slice
// 			p.atomsBuffer = append(p.atomsBuffer, atomsSlice)
// 			p.item.ID = int(trice.ID)
// 			p.values = append(p.values, trice.Value[0], trice.Value[1])
// 			//p.values = append(p.values, trice.Value...)
// 		case dropped := <-p.ignored:
// 			fmt.Println(dropped)
// 		}
// 	}
// }
//
// // TriceItem tries to create a full trice item from the receiver channels.
// func (p *TriceReceiver) TriceItem() {
// 	go func() {
// 		for {
// 			if nil != p.err || io.EOF == p.err {
// 				return
// 			}
// 			p.triceItem()
// 		}
// 	}()
// }

/*func xy(){
	var t Trice // chanel 'trices'
	//var b []byte // channel 'ignored'
	item, values, err := TriceCollector(t)
	if item (
		//s, b, _ := Emitter( item, values, b)
		LineCollect(s)
	)
}*/

/* NewTriceReceiverFromWrap creates a TriceReceiver using r as internal reader.
func NewTriceReceiverFromWrap(r io.Reader) *TriceReceiver {
	p := &TriceReceiver{}
	p.r = r
	p.atoms = make(chan []Trice, triceChannelCapacity)
	p.ignored = make(chan []byte, ignoredChannelCapacity)
	go func() {

	}()
	return p
}*/

//type TriceReadFunc func Read( []Trice) (int, error)

// type LineWriter interface{
// 	func LineWrite( s []string ) (int, error)
// }

// // Receiver provides a TriceReadMethod
// type Receiver struct {
// 	inner    io.Reader // used inner byte reader
// 	encoding string    // provided outer trice reader
// 	//list id.ListT
// 	//w io.Writer // byte writer interface for strings
// 	//item Bare // trice item
// 	//data []byte // data collector
// }

// // BareRead uses the underlying byte reader and returns a count of trice atoms inside t.
// func (p *Receiver) bareRead(t []Trice) (int, error) {
// 	b := make([]byte, 1000)
// 	n, e := p.inner.Read(b)
// 	b = b[:n]
// 	return 0, e
// }

// // WrapRead uses the underlying byte reader and returns a count of trice atoms inside t.
// func (p *Receiver) wrapRead(t []Trice) (int, error) {
// 	return 0, nil
// }

// // TriceRead uses the underlying byte reader and returns a count of trice atoms inside t.
// func (p *Receiver) TriceRead(t []Trice) (int, error) {
// 	switch p.encoding {
// 	case "bare":
// 		return p.bareRead(t)
// 	case "wrap":
// 		return p.wrapRead(t)
// 	}
// 	return 0, nil
// }

// // NewTriceReceiver returns a pointer to a trice receiver instance using r as inner byte reader.
// func NewTriceReceiver(inner io.Reader, encoding string) *Receiver {
// 	p := &Receiver{}
// 	p.inner = inner
// 	p.encoding = encoding
// 	return p
// }

// func io.WriteString(w Writer, s string) (n int, err error)

// // RemoteCommand is the data type for commands.
// //
// // The data package count is given by len([]Pkg)
// type RemoteCommand struct {
// 	PackageID uint // package id (cycle counter)
// 	PackageType byte // message, command, answer
// 	FunctionType byte // type identifyer byte
// 	FunctionID byte // function identifyer byte
// 	DataPackages [][]byte // DataPackageCount is len(DataPackages)
// }
