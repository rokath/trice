package cage

import (
	"fmt"
	"log"
	"os"
	"testing"

	"github.com/rokath/trice/pkg/lib"
)

/*
github.com/stretchrcom/testify
github.com/onsi/ginko behaivioral tests
goconvey.co view test results in browser based display
httpexpect
code.google.com/p/gomock
go-sqlmock
packagename      -> whitebox
packagename_test -> blackbox testing
go help test
go help testflag
go test -coverprofile cover.out
go tool cover -func cover.out
go tool cover -html cover.out makes a website
go test -coverprofile cover.out -covermode count

*/
func TestStart(t *testing.T) {
	afn := "testdata/actCage.log"
	efn := "testdata/expCage.log"
	os.Remove(afn)
	os.Remove(efn)

	efh, err := os.OpenFile(efn, os.O_RDWR|os.O_CREATE, 0666)
	lib.Ok(t, err)
	_, err = fmt.Fprintln(efh, "testLog00\ntestOutOrErr01\ntestOutOrErr01")
	lib.Ok(t, err)

	log.SetFlags(0) // switch off log timestamp

	c := Start(afn)

	log.Println("testLog00")
	fmt.Println("testOutOrErr01")
	fmt.Fprintln(os.Stderr, "testOutOrErr01")

	Stop(c)
	lib.EqualFiles(t, afn, efn)

	efh, err = os.OpenFile(efn, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	lib.Ok(t, err)
	_, err = fmt.Fprintln(efh, "testLog10\ntestOutOrErr11\ntestOutOrErr11")
	lib.Ok(t, err)

	d := Start(afn)

	log.Println("testLog10")
	fmt.Println("testOutOrErr11")
	fmt.Fprintln(os.Stderr, "testOutOrErr11")

	Stop(d)

	lib.EqualFiles(t, afn, efn)

	lib.Ok(t, os.Remove(afn))
	lib.Ok(t, os.Remove(efn))
}
