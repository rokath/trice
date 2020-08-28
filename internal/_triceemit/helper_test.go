// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package triceemit

import (
	"fmt"
	"log"
	"path/filepath"
	"reflect"
	"runtime"
	"testing"
)

// test helper ///////////////////////////////////////////////////////////////////////
//

// assertEqual fails the test if exp is not equal to act.
func assertEqual(tb testing.TB, exp, act interface{}) {
	if !reflect.DeepEqual(exp, act) {
		_, file, line, _ := runtime.Caller(1)
		log.Println("expect:", exp)
		log.Println("actual:", act)
		fmt.Println(filepath.Base(file), line)
		tb.FailNow()
	}
}

//
// test helper ///////////////////////////////////////////////////////////////////////
