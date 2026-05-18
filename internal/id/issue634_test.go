// SPDX-License-Identifier: MIT

package id_test

import (
	"strings"
	"testing"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestIssue634InsertAndCleanRespectMarkers(t *testing.T) {
	defer Setup(t)()

	source := strings.Join([]string{
		`trice("before");`,
		`// TRICE_INSERT_OFF`,
		`TRice("bad %d", 1, 2);`,
		`TRice(iD(77), "skip id");`,
		`// TRICE_INSERT_ON`,
		`trice("after");`,
		``,
	}, "\n")
	src := "issue634.c"
	require.NoError(t, FSys.WriteFile(src, []byte(source), 0o777))

	insertArgs := []string{
		"trice", "insert",
		"-src", src,
		"-IDMin", "1200",
		"-IDMax", "1299",
		"-IDMethod", "upward",
		"-til", FnJSON,
		"-li", LIFnJSON,
	}
	require.NoError(t, args.Handler(W, FSys, insertArgs))

	inserted, err := FSys.ReadFile(src)
	require.NoError(t, err)
	wantInserted := strings.Join([]string{
		`trice(iD(1200), "before");`,
		`// TRICE_INSERT_OFF`,
		`TRice("bad %d", 1, 2);`,
		`TRice(iD(77), "skip id");`,
		`// TRICE_INSERT_ON`,
		`trice(iD(1201), "after");`,
		``,
	}, "\n")
	assert.Equal(t, wantInserted, string(inserted))

	til, err := FSys.ReadFile(FnJSON)
	require.NoError(t, err)
	assert.Contains(t, string(til), "before")
	assert.Contains(t, string(til), "after")
	assert.NotContains(t, string(til), "skip")
	assert.NotContains(t, string(til), `"77"`)

	cleanArgs := []string{
		"trice", "clean",
		"-src", src,
		"-til", FnJSON,
		"-li", LIFnJSON,
	}
	require.NoError(t, args.Handler(W, FSys, cleanArgs))

	cleaned, err := FSys.ReadFile(src)
	require.NoError(t, err)
	assert.Equal(t, source, string(cleaned))
}

func TestIssue634AddRespectsMarkers(t *testing.T) {
	defer Setup(t)()

	source := strings.Join([]string{
		`trice(iD(11), "active");`,
		`// TRICE_INSERT_OFF`,
		`trice(iD(22), "skip");`,
		`// TRICE_INSERT_ON`,
		`trice(iD(33), "after");`,
		``,
	}, "\n")
	src := "issue634_add.c"
	require.NoError(t, FSys.WriteFile(src, []byte(source), 0o777))

	addArgs := []string{
		"trice", "add",
		"-src", src,
		"-til", FnJSON,
		"-li", LIFnJSON,
	}
	require.NoError(t, args.Handler(W, FSys, addArgs))

	til, err := FSys.ReadFile(FnJSON)
	require.NoError(t, err)
	assert.Contains(t, string(til), "active")
	assert.Contains(t, string(til), "after")
	assert.NotContains(t, string(til), "skip")
	assert.NotContains(t, string(til), `"22"`)
}
