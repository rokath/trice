package id_test

import (
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// TestInsertKeepsLessThanInTil ensures that a trice insert run does not
// rewrite '<' characters inside the TIL JSON file into '\u003c'.
func TestInsertKeepsLessThanInTil(t *testing.T) {
	// Setup test environment and ensure cleanup runs afterward.
	defer id.Setup(t)()

	// existing li.json (location info)
	exsLI := `{
	"707": {
		"File": "file1.c",
		"Line": 3
	}
}`
	assert.Nil(t, id.FSys.WriteFile(id.LIFnJSON, []byte(exsLI), 0o777))

	// existing til.json with "<" in the format string
	exsTIL := `{
	"707": {
		"Type": "TRice",
		"Strg": "modem:<<+QNBIOTEVENT: \\\"ENTER PSM\\\""
	}
}`
	assert.Nil(t, id.FSys.WriteFile(id.FnJSON, []byte(exsTIL), 0o777))

	// src file1: Trice with known ID
	src1 := `
	TRice(id(707),"modem:<<+QNBIOTEVENT: \"ENTER PSM\"" );
	`
	assert.Nil(t, id.FSys.WriteFile("file1.c", []byte(src1), 0o777))

	// action: single trice insert
	argsSlice := []string{
		"trice", "insert",
		"-src", "file1.c",
		"-til", id.FnJSON,
		"-li", id.LIFnJSON,
	}
	assert.Nil(t, args.Handler(id.W, id.FSys, argsSlice))

	// check: source file must not be modified by insert
	actSrc1, e := id.FSys.ReadFile("file1.c")
	assert.Nil(t, e)
	assert.Equal(t, src1, string(actSrc1))

	// check: til.json still contains "<" and does not contain \u003c
	actTIL, e := id.FSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	s := string(actTIL)

	assert.Contains(t, s, `modem:<<+QNBIOTEVENT:`)
	assert.NotContains(t, s, `\u003c`, "unexpected unicode escaping for '<' in til.json")

	// cleanup
	id.FSys.Remove(id.FnJSON)
	id.FSys.Remove(id.LIFnJSON)
	id.FSys.RemoveAll(id.UserHomeDir)
}

// TestInsertLessThanIdempotent ensures that running trice insert twice
// does not change the TIL JSON file between runs and does not introduce
// unicode escaping for '<' characters.
func TestInsertLessThanIdempotent(t *testing.T) {
	// Setup test environment and ensure cleanup runs afterward.
	defer id.Setup(t)()

	// existing li.json (location info)
	exsLI := `{
	"707": {
		"File": "file1.c",
		"Line": 3
	}
}`
	assert.Nil(t, id.FSys.WriteFile(id.LIFnJSON, []byte(exsLI), 0o777))

	// existing til.json with "<" in the format string
	exsTIL := `{
	"707": {
		"Type": "TRice",
		"Strg": "modem:<<+QNBIOTEVENT: \\\"ENTER PSM\\\""
	}
}`
	assert.Nil(t, id.FSys.WriteFile(id.FnJSON, []byte(exsTIL), 0o777))

	// src file1: Trice with known ID
	src1 := `
	TRice(id(707),"modem:<<+QNBIOTEVENT: \"ENTER PSM\"" );
	`
	assert.Nil(t, id.FSys.WriteFile("file1.c", []byte(src1), 0o777))

	argsSlice := []string{
		"trice", "insert",
		"-src", "file1.c",
		"-til", id.FnJSON,
		"-li", id.LIFnJSON,
	}

	// first run
	err := args.Handler(id.W, id.FSys, argsSlice)
	assert.Nil(t, err)
	after1, err := id.FSys.ReadFile(id.FnJSON)
	assert.Nil(t, err)

	// second run: in a full package test run, the global tag/ID-range
	// configuration may already contain an entry for tag "e". In that
	// case args.Handler can legitimately return a specific error about
	// an already assigned ID range. We treat that as acceptable here,
	// as long as the TIL file itself remains unchanged and does not
	// contain unicode-escaped '<' characters.
	err = args.Handler(id.W, id.FSys, argsSlice)
	if err != nil {
		assert.EqualError(t, err, "tagName e has already an assigned ID range. Please check your command line")
	}

	after2, err := id.FSys.ReadFile(id.FnJSON)
	assert.Nil(t, err)

	// the complete TIL file must be identical between runs
	assert.Equal(t, string(after1), string(after2), "til.json changed between two identical insert runs")

	// safety check: no unicode escaping for '<'
	assert.NotContains(t, string(after2), `\u003c`, "unexpected unicode escaping for '<' in til.json")

	// cleanup
	id.FSys.Remove(id.FnJSON)
	id.FSys.Remove(id.LIFnJSON)
	id.FSys.RemoveAll(id.UserHomeDir)
}
