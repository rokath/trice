package trexDecoder

import (
	"bytes"
	"fmt"
	"io"
	"log"

	cobs "github.com/rokath/cobs/go"
	"github.com/rokath/tcobs/v1"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/pkg/cipher"
)

// nextData reads with an inner reader a raw byte stream and appends it to p.I.
func (p *trexDec) nextData() {
	m, err := p.In.Read(p.InnerBuffer) // use p.InnerBuffer as destination read buffer
	if err != nil && err != io.EOF {   // some serious error
		fmt.Fprint(p.W, "ERROR:internal reader error\a", err)
	}
	p.I = append(p.I, p.InnerBuffer[:m]...) // merge with leftovers
}

// nextFrame reads with an inner reader a raw byte stream, appends it to p.Last and returns in p.B a complete decoded frame or p.B stays empty.
func (p *trexDec) nextFrame() {
	m, err := p.In.Read(p.InnerBuffer) // use p.InnerBuffer as destination read buffer
	if err != nil && err != io.EOF {   // some serious error
		fmt.Fprint(p.W, "ERROR:internal reader error\a", err)
	}
	p.Last = append(p.Last, p.InnerBuffer[:m]...)
	index := bytes.IndexByte(p.Last, 0) // find terminating 0
	if index == -1 {                    // p.Last has no complete (T)COBS data
		if len(p.I) != 0 {
			fmt.Fprint(p.W, "ERROR:unexpected data in p.I\a")
			decoder.Dump(p.W, p.I)
			p.I = p.I[:0]
		}
		return
	}
	err = p.decodeFrame(p.Last[:index]) // exclude terminating 0, put frame into p.I

	// Here we have read but unprocessed data in p.Last[index+1:]
	n := copy(p.Last, p.Last[index+1:])
	p.Last = p.Last[:n]
}

// decodeFrame expects a full frame without 0-delimiter in src, and decodes it into p.I.
func (p *trexDec) decodeFrame(src []byte) (err error) {
	var n int
	p.I = p.B
	switch p.packageFraming {
	case packageFramingCOBS:
		n, err = cobs.Decode(p.I, src)
		p.I = p.I[:n]
	case packageFramingTCOBS:
		n, err = tcobs.Decode(p.I, src)
		p.I = p.I[len(p.I)-n:]
	case packageFramingTCOBSv2:
		fallthrough
	default:
		log.Fatalln("unexpected p.packageFraming", p.packageFraming)
	}
	if err != nil {
		return err
	}
	return p.decryptIfEncrypted()
}

// decryptIfEncrypted decrypts p.I in place, when a Cipher.Password is set.
func (p *trexDec) decryptIfEncrypted() (err error) {
	if decoder.DebugOut { // Debug output
		//fmt.Print("->TRICE: ")
		//decoder.Dump(os.Stdout, p.B)
		fmt.Fprint(p.W, "->TRICE: ")
		decoder.Dump(p.W, p.I)
	}
	if cipher.Password != "" { // encrypted
		if len(p.I)%8 != 0 {
			fmt.Fprint(p.W, "-> ERR:  ")
			decoder.Dump(p.W, p.I)
			err = fmt.Errorf("decoded frame length %d not a multiple of 8, discarding", len(p.I))
			p.I = p.I[:0]
			return err
		}
		cipher.Decrypt(p.I, p.I)
		if decoder.DebugOut { // Debug output
			//fmt.Print("->DEC: ")
			//decoder.Dump(os.Stdout, p.B)
			fmt.Fprint(p.W, "-> DEC:  ")
			decoder.Dump(p.W, p.I)
		}
	}
	return nil
}
