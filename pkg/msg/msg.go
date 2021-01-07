package msg

import (
	"log"
	"runtime"
)

// OnErr logs a common error message with location info when err is not nil.
func OnErr(err error) {
	if nil == err {
		return
	}
	// notice that we're using 1, so it will actually log the where
	// the error happened, 0 = this function, we don't want that.
	pc, fn, line, _ := runtime.Caller(1)
	log.Printf("[error] in %s[%s:%d] %v", runtime.FuncForPC(pc).Name(), fn, line, err)
}
