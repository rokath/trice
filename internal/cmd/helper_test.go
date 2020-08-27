// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package cmd

import (
	"fmt"
	"log"
	"math/rand"
	"reflect"
	"runtime"
	"testing"
	"time"
)

// test helper ///////////////////////////////////////////////////////////////////////
//

// equals fails the test if exp is not equal to act.
func equals(tb testing.TB, exp, act interface{}) {
	if !reflect.DeepEqual(exp, act) {
		// notice that we're using 1, so it will actually log the where
		// the error happened, 0 = this function, we don't want that.
		pc, fn, line, _ := runtime.Caller(1)
		log.Printf("[delta] in %s[%s:%d]", runtime.FuncForPC(pc).Name(), fn, line)
		log.Println("expect:", exp)
		log.Println("actual:", act)
		tb.FailNow()
	}
}

func randomDynIPPort() (s string) {
	rand.Seed(time.Now().UTC().UnixNano())
	min := 49152
	max := 65535
	s = fmt.Sprint(rand.Intn(max-min) + min)
	return
}

func TestEquals(t *testing.T) {
	equals(t, nil, nil)
}

//
// test helper ///////////////////////////////////////////////////////////////////////
