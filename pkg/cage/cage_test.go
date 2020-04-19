package cage

import (
	"fmt"
	"log"
	"os"
	"testing"
	"time"

	"github.com/rokath/trice/pkg/lib"
)

func TestStart(t *testing.T) {
	lfn := "testdata/actCage.log"
	efn := "testdata/expCage.log"
	os.Remove(lfn)
	os.Remove(efn)

	efh, err := os.OpenFile(efn, os.O_RDWR|os.O_CREATE, 0666)
	lib.Ok(t, err)
	_, err = fmt.Fprintln(efh, "testLog00\ntestOut01\ntestErr02")
	lib.Ok(t, err)

	log.SetFlags(0) // switch off log timestamp

	c := Start(lfn)

	log.Println("testLog00")
	time.Sleep(1 * time.Millisecond) // needed to avoid takeover
	fmt.Println("testOut01")
	time.Sleep(1 * time.Millisecond) // needed to avoid takeover
	fmt.Fprintln(os.Stderr, "testErr02")

	Stop(c)
	lib.EqualFiles(t, lfn, efn)

	efh, err = os.OpenFile(efn, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	lib.Ok(t, err)
	_, err = fmt.Fprintln(efh, "testLog10\ntestOut11\ntestErr12")
	lib.Ok(t, err)

	d := Start(lfn)

	log.Println("testLog10")
	time.Sleep(1 * time.Millisecond) // needed to avoid takeover
	fmt.Println("testOut11")
	time.Sleep(1 * time.Millisecond) // needed to avoid takeover
	fmt.Fprintln(os.Stderr, "testErr12")

	Stop(d)

	lib.EqualFiles(t, lfn, efn)

	err = os.Remove(lfn)
	lib.Ok(t, err)
	err = os.Remove(efn)
	lib.Ok(t, err)
}
