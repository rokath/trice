// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"io/ioutil"
	"math/rand"
	"os"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/assert"
	"github.com/rokath/trice/pkg/file"
)

func doUpdate(t *testing.T, s, sExp []string) {

	rand.Seed(0)
	p := id.NewList(file.Random("{}", "", "til*.json"))

	dir0, err := ioutil.TempDir("", "*")
	assert.ErrorNil(t, err)
	dir1, err := ioutil.TempDir(dir0, "*")
	assert.ErrorNil(t, err)
	dir2, err := ioutil.TempDir(dir0, "*")
	assert.ErrorNil(t, err)
	dir3, err := ioutil.TempDir(dir1, "*")
	assert.ErrorNil(t, err)

	n0 := file.Random(s[0], dir1, "*.c")
	err = p.Update(dir0, true, true)
	assert.ErrorNil(t, err)

	n1 := file.Random(s[1], dir2, "*.cpp")
	err = p.Update(dir0, true, true)
	assert.ErrorNil(t, err)

	n2 := file.Random(s[2], dir2, "*.cc")
	err = p.Update(dir0, true, true)
	assert.ErrorNil(t, err)

	n3 := file.Random(s[3], dir3, "*.c")
	err = p.Update(dir0, true, true)
	assert.ErrorNil(t, err)

	p.ZeroTimestampCreated()
	p.WriteListFile()

	sAct := make([]string, 0, 4)
	sAct = append(sAct, file.ReadString(n0))
	sAct = append(sAct, file.ReadString(n1))
	sAct = append(sAct, file.ReadString(n2))
	sAct = append(sAct, file.ReadString(n3))

	for i := range sExp {
		assert.EqualLines(t, sExp[i], sAct[i])
	}

	os.RemoveAll(dir0)
}

func TestUpdateZeroId(t *testing.T) {
	s0 := []string{
		`
TRICE0 (Id(0), "---------------------------------------\n" );
TRICE0 (Id(0), "---------------------------------------\n" );
`,
		`
TRICE8_1( Id(0), "tst:TRICE8 %d\n", 1 );
TRICE8_2( Id(0), "tst:TRICE8 %d %d\n", 1, 2 );
TRICE8_3( Id(0), "tst:TRICE8 %d %d %d\n", 1, 2, 3 );
TRICE8_4( Id(0), "tst:TRICE8 %d %d %d %d\n", 1, 2, 3, 4 );
TRICE8_5( Id(0), "tst:TRICE8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 );
TRICE8_6( Id(0), "tst:TRICE8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 );
TRICE8_7( Id(0), "tst:TRICE8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 );
TRICE8_8( Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
`,
		`	
TRICE16_1( Id(0), "tst:TRICE16 %d\n", 1 );
TRICE16_2( Id(0), "tst:TRICE16 %d %d\n", 1, 2 );
TRICE16_3( Id(0), "tst:TRICE16 %d %d %d\n", 1, 2, 3 );
TRICE16_4( Id(0), "tst:TRICE16 %d %d %d %d\n", 1, 2, 3, 4 );
`,
		`
TRICE32_2( Id(0), "tst:TRICE32   %%09x ->     %09x     %09x", 1, 0x7fffffff );
TRICE32_2( Id(0), "tst:     %09x     %09x\n", 0x80000000, 0xffffffff );
TRICE32_2( Id(0), "tst:TRICE32   %%11d ->   %11d   %11d", 1, 0x7fffffff );
TRICE32_2( Id(0), "tst:   %11d   %11d\n", 0x80000000, 0xffffffff  );
TRICE32_2( Id(0), "tst:TRICE32   %%12o ->  %12o  %12o", 1, 0x7fffffff );
TRICE32_2( Id(0), "tst:  %12o  %12o\n", 0x80000000, 0xffffffff );
`,
	}

	sExp := []string{
		`    
TRICE0 (Id(43274), "---------------------------------------\n" );
TRICE0 (Id( 9089), "---------------------------------------\n" );
`,
		`
TRICE8_1( Id(46083), "tst:TRICE8 %d\n", 1 );
TRICE8_2( Id(50226), "tst:TRICE8 %d %d\n", 1, 2 );
TRICE8_3( Id(18460), "tst:TRICE8 %d %d %d\n", 1, 2, 3 );
TRICE8_4( Id(55041), "tst:TRICE8 %d %d %d %d\n", 1, 2, 3, 4 );
TRICE8_5( Id(60612), "tst:TRICE8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 );
TRICE8_6( Id(16507), "tst:TRICE8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 );
TRICE8_7( Id(59378), "tst:TRICE8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 );
TRICE8_8( Id(62543), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
`,
		`	
TRICE16_1( Id(34028), "tst:TRICE16 %d\n", 1 );
TRICE16_2( Id(  472), "tst:TRICE16 %d %d\n", 1, 2 );
TRICE16_3( Id( 1669), "tst:TRICE16 %d %d %d\n", 1, 2, 3 );
TRICE16_4( Id(21923), "tst:TRICE16 %d %d %d %d\n", 1, 2, 3, 4 );
`,
		`
TRICE32_2( Id(42242), "tst:TRICE32   %%09x ->     %09x     %09x", 1, 0x7fffffff );
TRICE32_2( Id(60761), "tst:     %09x     %09x\n", 0x80000000, 0xffffffff );
TRICE32_2( Id(16891), "tst:TRICE32   %%11d ->   %11d   %11d", 1, 0x7fffffff );
TRICE32_2( Id(53650), "tst:   %11d   %11d\n", 0x80000000, 0xffffffff  );
TRICE32_2( Id(15285), "tst:TRICE32   %%12o ->  %12o  %12o", 1, 0x7fffffff );
TRICE32_2( Id(61124), "tst:  %12o  %12o\n", 0x80000000, 0xffffffff );
`,
	}

	doUpdate(t, s0, sExp)
}

/*
func TestUpdateAllEqual(t *testing.T) {
	doUpdate(t, "/testdata/UpdateAllEqual/")
}

func TestUpdateSameId(t *testing.T) {
	doUpdate(t, "/testdata/UpdateSameId/")
}

func TestUpdatePrintfAdapter(t *testing.T) {
	doUpdate(t, "/testdata/AutoParamCount/")
}
*/
