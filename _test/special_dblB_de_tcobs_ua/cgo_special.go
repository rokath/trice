package cgot

// For some reason inside the trice_test.go an 'import "C"' is not possible.

// void TriceCheckSpecial( int n );
import "C"
import (
	"fmt"
	"strings"
	"testing"

	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func targetSpecialActivity(n int) {
	C.TriceCheckSpecial(C.int(n))
}

// triceLogSpecialTest creates a list of expected results from  path.Join(triceDir, "./_test/tf/TargetActivity.c").
// It loops over the result list and executes for each result the compiled C-code.
// It passes the received binary data as buffer to the triceLog function of type logF.
// This function is test package specific defined. The file cgoPackage.go is
// copied into all specific test packages and compiled there together with the
// triceConfig.h, which holds the test package specific target code configuration.
// limit is the count of executed test lines starting from the beginning. -1 ist for all.
func triceLogSpecialTest(t *testing.T, triceLog logF, limit int) {

	osFSys := &afero.Afero{Fs: afero.NewOsFs()}

	// CopyFileIntoFSys(t, mmFSys, "til.json", osFSys, td+"./til.json") // needed for the trice log
	out := make([]byte, 32768)
	setTriceBuffer(out)

	result := getExpectedResults(osFSys, "./TargetActivity.c")

	var count int
	for i, r := range result {

		count++
		if limit >= 0 && count >= limit {
			return
		}

		fmt.Println(i, r)

		// target activity
		targetSpecialActivity(r.line) // triceCheck would compile but not wat we want here

		triceTransfer() // This is only for deferred modes needed, but direct modes contain this as empty function.

		length := triceOutDepth()
		bin := out[:length] // bin contains the binary trice data of trice message i in r.line

		buf := fmt.Sprint(bin)
		buffer := buf[1 : len(buf)-1]

		act := triceLog(t, osFSys, buffer)
		triceClearOutBuffer()

		assert.Equal(t, r.exps, strings.TrimSuffix(act, "\n"))
	}
}
