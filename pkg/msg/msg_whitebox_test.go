// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package msg

import (
	"errors"
	"fmt"
	"testing"
)

func TestFatalInfoOnFalse(t *testing.T) {
	origLogFatalf := logFatalf
	// After this test, replace the original fatal function
	defer func() { logFatalf = origLogFatalf }()

	e := []string{}
	logFatalf = func(format string, args ...interface{}) {
		if len(args) > 0 {
			e = append(e, fmt.Sprintf(format, args))
		} else {
			e = append(e, format)
		}
	}

	err := errors.New("wrong")

	FatalOnErr(nil)
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalOnErr(err)
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

	FatalInfoOnErr(nil, "just in case")
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalInfoOnErr(err, "just in case")
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

	FatalOnTrue(false)
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalOnTrue(true)
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

	FatalOnFalse(true)
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalOnFalse(false)
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

	FatalInfoOnTrue(false, "its is true")
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalInfoOnTrue(true, "its is true")
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

	FatalInfoOnFalse(true, "info")
	if len(e) != 0 {
		t.Errorf("excepted no error, actual %v", len(e))
	}
	FatalInfoOnFalse(false, "info")
	if len(e) != 1 {
		t.Errorf("excepted one error, actual %v", len(e))
	}
	e = e[:0] // clear

}
