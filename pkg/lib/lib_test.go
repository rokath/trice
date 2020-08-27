// +build move
// whitebox tests
package lib

/*
func TestAssert(t *testing.T) {
	Assert(t, true, "message")
}

func TestEqualFileContent2(t *testing.T) {
	EqualFiles2(t, os.Args[0], os.Args[0])
}

func TestEqualFileContent3(t *testing.T) {
	s := "one"
	EqualFile(t, one, s)
}
*/
/*
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
*/
