// whitebox tests
package lib

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

var (
	one = "testdata/dir/one.txt"
	two = "./testdata/dir/two.txt"
)

// Test is just to have a test
func TestDummy(t *testing.T) {
	Ok(t, nil)
}

func TestEquals(t *testing.T) {
	Equals(t, nil, nil)
}

func TestAssert(t *testing.T) {
	Assert(t, true, "message")
}

func TestArrayFlag(t *testing.T) {
	p := &Srcs
	Equals(t, "my string representation", p.String())

	p.Set("ab")
	p.Set("xyz")
	fmt.Println(Srcs)
	var af ArrayFlag
	af = []string{"ab", "xyz"}
	Equals(t, af, Srcs)
}

func TestConditinalFilePath(t *testing.T) {
	s := ConditinalFilePath("off")
	Equals(t, "off", s)
	s = ConditinalFilePath("none")
	Equals(t, "none", s)
	s = ConditinalFilePath("/tatue/tata")
	b := filepath.Base(s)
	Equals(t, b, "tata")
}

func TestEqualFileContent0(t *testing.T) {
	EqualFiles(t, os.Args[0], os.Args[0])
}

func TestEqualFileContent2(t *testing.T) {
	EqualFiles2(t, os.Args[0], os.Args[0])
}

func TestEqualFileContent3(t *testing.T) {
	s := "one"
	EqualFile(t, one, s)
}

func TestReadLines(t *testing.T) {
	s, err := ReadLines(two)
	Equals(t, nil, err)
	x := strings.NewReplacer("\r\n", "\n")
	cleared := make([]string, len(s))
	for i := range s {
		cleared[i] = x.Replace(s[i])
	}
	exp := []string{"one\n", "two\n"}
	Equals(t, exp, cleared)
}

func TestWriteLines(t *testing.T) {
	rf := two
	ef := two + ".expected.txt"
	ss := []string{"one\n", "two\n"}
	err := WriteLines(ef, ss)
	Equals(t, nil, err)
	CleanFile(rf)
	EqualFiles(t, ef, rf+".clean")
	err = os.Remove(ef)
	Equals(t, nil, err)
	err = os.Remove(rf + ".clean")
	Equals(t, nil, err)
}

func TestCopyFiles(t *testing.T) {
	src := two
	dest := two + ".copied"
	err := CopyFile(src, dest)
	Equals(t, nil, err)
	EqualFiles(t, src, dest)
	err = os.Remove(dest)
	Equals(t, nil, err)
}

func TestDir(t *testing.T) {
	src := "./testdata/dir"
	dest := src + ".copied"
	err := CopyDir(src, dest)
	Equals(t, nil, err)
	//Equals(t, src, dest)
	err = os.RemoveAll(dest)
	Equals(t, nil, err)
}
