package cage_test

import (
	"fmt"
	"log"
	"os"
	"testing"

	"github.com/rokath/trice/pkg/tst"
	"github.com/stretchr/testify/assert"

	"github.com/rokath/trice/pkg/cage"
)

func TestStartVerbose(t *testing.T) {
	cage.Verbose = true
	TestStart(t)
}

func TestStart(t *testing.T) {
	afn := "testdata/actCage.log"
	efn := "testdata/expCage.log"
	_ = os.Remove(afn)
	_ = os.Remove(efn)

	efh, err := os.OpenFile(efn, os.O_RDWR|os.O_CREATE, 0666)
	assert.Nil(t, err)
	_, err = fmt.Fprintln(efh, "testLog00\ntestOutOrErr01\ntestOutOrErr01")
	assert.Nil(t, err)
	log.SetFlags(0) // switch off log timestamp

	c := cage.Start(os.Stdout, afn)

	log.Println("testLog00")
	fmt.Println("testOutOrErr01")
	_, err = fmt.Fprintln(os.Stderr, "testOutOrErr01")
	assert.Nil(t, err)

	cage.Stop(os.Stdout, c)
	tst.AssertEqualFiles(t, afn, efn)

	efh, err = os.OpenFile(efn, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	assert.Nil(t, err)
	_, err = fmt.Fprintln(efh, "testLog10\ntestOutOrErr11\ntestOutOrErr11")
	assert.Nil(t, err)

	d := cage.Start(os.Stdout, afn)

	log.Println("testLog10")
	fmt.Println("testOutOrErr11")
	_, err = fmt.Fprintln(os.Stderr, "testOutOrErr11")
	assert.Nil(t, err)

	cage.Stop(os.Stdout, d)

	tst.EqualTextFiles(t, afn, efn)

	assert.Nil(t, os.Remove(afn))
	assert.Nil(t, os.Remove(efn))
}
