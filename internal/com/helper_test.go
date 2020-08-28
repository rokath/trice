// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package com_test is a blackbox test.
package com_test

import (
	"fmt"
	"path/filepath"
	"runtime"
	"testing"
)

// test helper ///////////////////////////////////////////////////////////////////////
//

// errorFail fails the test if err is not nil.
func errorFail(tb testing.TB, err error) {
	if err != nil {
		_, file, line, _ := runtime.Caller(1)
		fmt.Println(err.Error(), filepath.Base(file), line)
		tb.FailNow()
	}
}

//
// test helper ///////////////////////////////////////////////////////////////////////
