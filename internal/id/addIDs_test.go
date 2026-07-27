// SPDX-License-Identifier: MIT

package id_test

import (
	"encoding/json"
	"path/filepath"
	"testing"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// assertLocationJSONEqual compares legacy location fields while allowing generated Path metadata.
func assertLocationJSONEqual(t *testing.T, expected string, actual []byte) {
	t.Helper()
	var expectedLI TriceIDLookUpLI
	var actualLI TriceIDLookUpLI
	require.NoError(t, json.Unmarshal([]byte(expected), &expectedLI))
	require.NoError(t, json.Unmarshal(actual, &actualLI))
	for triceID, location := range expectedLI {
		location.Path = ""
		expectedLI[triceID] = location
	}
	for triceID, location := range actualLI {
		location.Path = ""
		actualLI[triceID] = location
	}
	assert.Equal(t, expectedLI, actualLI)
}

// TestAddWithLIExtension verifies the expected behavior.
func TestAddWithLIExtension(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create non-empty li.json
	li := `{
	"16200": {
		"File": "stm32f0xx_it.c",
		"Line": 160
	},
	"16202": {
		"File": "main.c",
		"Line": 71
	}
}`
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(li), 0777))

	// create src file
	sFn := t.Name() + "file.c"
	src := `break; case __LINE__: trice( iD(999), "msg:value=%d\n", -1  );`
	assert.Nil(t, FSys.WriteFile(sFn, []byte(src), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "add", "-v", "-src", sFn, "-til", FnJSON, "-li", LIFnJSON}))

	// check un-modified src file
	expSrc := src
	actSrc, e := FSys.ReadFile(sFn)
	assert.Nil(t, e)
	assert.Equal(t, expSrc, string(actSrc))

	// check extended location indormation file
	expLI := `{
	"16200": {
		"File": "stm32f0xx_it.c",
		"Line": 160
	},
	"16202": {
		"File": "main.c",
		"Line": 71
	},
	"999": {
		"File": "TestAddWithLIExtensionfile.c",
		"Line": 1
	}
}`
	actLI, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)
	assertLocationJSONEqual(t, expLI, actLI)
}

// TestToLIPath verifies the expected behavior.
func TestToLIPath(t *testing.T) {
	defer Setup(t)()

	LIPathKind = "base"
	assert.Equal(t, "demo.c", ToLIPath(filepath.Join("dir", "demo.c")))

	LIPathKind = filepath.Join("root", "relative")
	assert.Equal(t, filepath.ToSlash(filepath.Join("sub", "demo.c")), ToLIPath(filepath.Join("root", "sub", "demo.c")))

	LIPathKind = "full"
	got := ToLIPath("demo.c")
	assert.True(t, filepath.IsAbs(got))
	assert.Equal(t, "demo.c", filepath.Base(got))

	LIPathKind = "weird"
	assert.Equal(t, "demo.c", ToLIPath(filepath.Join("dir", "demo.c")))
}

// TestIssue708GeneratedPaths verifies that duplicate basenames retain distinct portable paths.
func TestIssue708GeneratedPaths(t *testing.T) {
	defer Setup(t)()

	firstSource := filepath.Join(Proj, "one", "main.c")
	secondSource := filepath.Join(Proj, "two", "main.c")
	require.NoError(t, FSys.MkdirAll(filepath.Dir(firstSource), 0o700))
	require.NoError(t, FSys.MkdirAll(filepath.Dir(secondSource), 0o700))
	require.NoError(t, FSys.WriteFile(firstSource, []byte(`trice(iD(101), "one");`), 0o600))
	require.NoError(t, FSys.WriteFile(secondSource, []byte(`trice(iD(102), "two");`), 0o600))

	commands := []struct {
		name       string
		pathKind   string
		firstFile  string
		secondFile string
	}{
		{name: "add", pathKind: "base", firstFile: "main.c", secondFile: "main.c"},
		{name: "clean", pathKind: "base", firstFile: "main.c", secondFile: "main.c"},
		{name: "insert", pathKind: filepath.Join(Proj, "relative"), firstFile: "one/main.c", secondFile: "two/main.c"},
	}
	for _, command := range commands {
		require.NoError(t, args.Handler(W, FSys, []string{
			"trice", command.name,
			"-src", firstSource,
			"-src", secondSource,
			"-til", FnJSON,
			"-li", LIFnJSON,
			"-liPath", command.pathKind,
		}))

		encodedLocations, err := FSys.ReadFile(LIFnJSON)
		require.NoError(t, err)
		var locations TriceIDLookUpLI
		require.NoError(t, json.Unmarshal(encodedLocations, &locations))
		assert.Equal(t, TriceLI{File: command.firstFile, Path: "one/main.c", Line: 1}, locations[101])
		assert.Equal(t, TriceLI{File: command.secondFile, Path: "two/main.c", Line: 1}, locations[102])
	}
}

// TestIssue708LocationFileSelectors verifies new files and backward-compatible old entries.
func TestIssue708LocationFileSelectors(t *testing.T) {
	defer Setup(t)()

	location := TriceLI{File: "stored/legacy.c", Path: "src/feature/main.c", Line: 7}
	LIDisplayPathKind = "legacy"
	assert.Equal(t, "stored/legacy.c", LocationFile(location))

	LIDisplayPathKind = "base"
	assert.Equal(t, "main.c", LocationFile(location))

	LIDisplayPathKind = "relative"
	assert.Equal(t, "src/feature/main.c", LocationFile(location))
	assert.Equal(t, "old/source.c", LocationFile(TriceLI{File: "old/source.c"}))

	LIDisplayPathKind = "full"
	LIFnJSON = filepath.Join(Proj, "logs", "li.json")
	LIRoot = ""
	assert.Equal(t, filepath.Join(Proj, "logs", "src", "feature", "main.c"), LocationFile(location))

	LIRoot = filepath.Join(Proj, "checkout")
	assert.Equal(t, filepath.Join(Proj, "checkout", "src", "feature", "main.c"), LocationFile(location))
	assert.Equal(t, filepath.Join(Proj, "checkout", "old", "source.c"), LocationFile(TriceLI{File: "old/source.c"}))
	assert.Equal(t, "source.c", LocationFile(TriceLI{File: "source.c"}))
}
