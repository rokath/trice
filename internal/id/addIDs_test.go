// SPDX-License-Identifier: MIT

package id_test

import (
	"encoding/json"
	"os"
	"path/filepath"
	"runtime"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// assertLocationJSONEqual compares generated location entries and rejects obsolete Path fields.
// Older behavioral fixtures name only the expected source suffix; dedicated
// Issue 712 tests above verify the complete root-relative path semantics.
func assertLocationJSONEqual(t *testing.T, expected string, actual []byte) {
	t.Helper()
	var expectedLI TriceIDLookUpLI
	var actualLI TriceIDLookUpLI
	require.NoError(t, json.Unmarshal([]byte(expected), &expectedLI))
	require.NoError(t, json.Unmarshal(actual, &actualLI))
	for triceID, expectedLocation := range expectedLI {
		actualLocation, ok := actualLI[triceID]
		if !ok || expectedLocation.File == actualLocation.File {
			continue
		}
		expectedFile := strings.TrimPrefix(filepath.ToSlash(expectedLocation.File), "./")
		actualFile := filepath.ToSlash(actualLocation.File)
		if strings.HasSuffix(actualFile, "/"+expectedFile) {
			expectedLocation.File = actualLocation.File
			expectedLI[triceID] = expectedLocation
		}
	}
	assert.Equal(t, expectedLI, actualLI)
	assert.NotContains(t, string(actual), `"Path"`)
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
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "add", "-v", "-src", sFn, "-til", FnJSON, "-li", LIFnJSON, "-liRoot", "."}))

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

// TestToLIFile verifies default and explicit location roots.
func TestToLIFile(t *testing.T) {
	defer Setup(t)()

	LIFnJSON = filepath.Join(Proj, "build", "demoLI.json")
	sourcePath := filepath.Join(Proj, "examples", "TriceABC", "src", "main.c")
	LIRoot = ""
	assert.Equal(t, "../examples/TriceABC/src/main.c", ToLIFile(sourcePath))

	LIRoot = Proj
	assert.Equal(t, "examples/TriceABC/src/main.c", ToLIFile(sourcePath))

	workingDirectory, err := os.Getwd()
	require.NoError(t, err)
	LIRoot = "."
	assert.Equal(t, "nested/main.c", ToLIFile(filepath.Join(workingDirectory, "nested", ".", "main.c")))

	if runtime.GOOS == "windows" {
		LIRoot = `C:\project`
		assert.Equal(t, "D:/sources/main.c", ToLIFile(`D:\sources\main.c`))
		assert.Equal(t, "//server/share/sources/main.c", ToLIFile(`\\server\share\sources\main.c`))
	}
}

// TestIssue712GeneratedPaths verifies that all ID commands retain canonical paths.
func TestIssue712GeneratedPaths(t *testing.T) {
	defer Setup(t)()

	firstSource := filepath.Join(Proj, "one", "main.c")
	secondSource := filepath.Join(Proj, "two", "main.c")
	require.NoError(t, FSys.MkdirAll(filepath.Dir(firstSource), 0o700))
	require.NoError(t, FSys.MkdirAll(filepath.Dir(secondSource), 0o700))
	require.NoError(t, FSys.WriteFile(firstSource, []byte(`trice(iD(101), "one");`), 0o600))
	require.NoError(t, FSys.WriteFile(secondSource, []byte(`trice(iD(102), "two");`), 0o600))

	commands := []struct {
		name string
	}{
		{name: "add"},
		{name: "clean"},
		{name: "insert"},
	}
	for _, command := range commands {
		require.NoError(t, args.Handler(W, FSys, []string{
			"trice", command.name,
			"-src", firstSource,
			"-src", secondSource,
			"-til", FnJSON,
			"-li", LIFnJSON,
			"-liRoot", Proj,
		}))

		encodedLocations, err := FSys.ReadFile(LIFnJSON)
		require.NoError(t, err)
		var locations TriceIDLookUpLI
		require.NoError(t, json.Unmarshal(encodedLocations, &locations))
		assert.Equal(t, TriceLI{File: "one/main.c", Line: 1}, locations[101])
		assert.Equal(t, TriceLI{File: "two/main.c", Line: 1}, locations[102])
		assert.NotContains(t, string(encodedLocations), `"Path"`)
	}
}

// TestIssue712LocationFileLimits verifies trailing-directory selection.
func TestIssue712LocationFileLimits(t *testing.T) {
	defer Setup(t)()

	location := TriceLI{File: "examples/TriceABC/src/main.c", Line: 7}
	expected := []string{
		"main.c",
		"src/main.c",
		"TriceABC/src/main.c",
		"examples/TriceABC/src/main.c",
	}
	for maxDirs, want := range expected {
		LIMaxDirs = maxDirs
		assert.Equal(t, want, LocationFile(location))
	}

	LIMaxDirs = 3
	assert.Equal(t, "src/main.c", LocationFile(TriceLI{File: "src/main.c"}))
	assert.Equal(t, "examples/TriceABC/src/main.c", LocationFile(TriceLI{File: "../../../examples/TriceABC/src/main.c"}))
	assert.Equal(t, "examples/TriceABC/src/main.c", LocationFile(TriceLI{File: "/checkout/examples/TriceABC/src/main.c"}))
	assert.Equal(t, "examples/TriceABC/src/main.c", LocationFile(TriceLI{File: `D:\checkout\examples\TriceABC\src\main.c`}))

	LIMaxDirs = -1
	assert.Equal(t, "main.c", LocationFile(location))
}

// TestIssue712LocationJSONHasOnlyFileAndLine verifies the serialized schema.
func TestIssue712LocationJSONHasOnlyFileAndLine(t *testing.T) {
	encoded, err := json.Marshal(TriceLI{File: "src/main.c", Line: 42})
	require.NoError(t, err)
	assert.JSONEq(t, `{"File":"src/main.c","Line":42}`, string(encoded))
	assert.False(t, strings.Contains(string(encoded), "Path"))
}

// TestIssue712ObsoletePathMigration verifies rewriting without source changes.
func TestIssue712ObsoletePathMigration(t *testing.T) {
	defer Setup(t)()

	const oldLocationJSON = `{
	"17": {
		"File": "src/main.c",
		"Path": "src/main.c",
		"Line": 42
	}
}`
	require.NoError(t, FSys.WriteFile(LIFnJSON, []byte(oldLocationJSON), 0o600))
	emptySource := filepath.Join(Proj, "empty.c")
	require.NoError(t, FSys.WriteFile(emptySource, nil, 0o600))

	require.NoError(t, args.Handler(W, FSys, []string{
		"trice", "add",
		"-src", emptySource,
		"-til", FnJSON,
		"-li", LIFnJSON,
	}))

	actual, err := FSys.ReadFile(LIFnJSON)
	require.NoError(t, err)
	assertLocationJSONEqual(t, `{"17":{"File":"src/main.c","Line":42}}`, actual)
}
