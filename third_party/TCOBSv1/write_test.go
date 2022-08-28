package tcobs_test

import (
	"bytes"
	"testing"

	"github.com/rokath/tcobs"
	"github.com/stretchr/testify/assert"
)

func TestWrite(t *testing.T) {
	var w bytes.Buffer                        // create inner writer
	p := tcobs.NewEncoder(&w, testBufferSize) // https://stackoverflow.com/questions/23454940/getting-bytes-buffer-does-not-implement-io-writer-error-message
	for _, x := range testData {
		n, e := p.Write(x.dec)
		assert.Nil(t, e)
		assert.True(t, n == len(x.dec))
		exp := append(x.enc, 0)
		act := w.Bytes()
		assert.Equal(t, exp, act)
		w.Reset()
	}
}
