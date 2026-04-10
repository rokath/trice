// SPDX-License-Identifier: MIT

package id

import (
	"path/filepath"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestInsertIsDeterministicAcrossRuns exposes that insert currently depends on
// goroutine scheduling when multiple files contain the same trice and IDs are
// assigned from an empty list.
func _TestInsertIsDeterministicAcrossRuns(t *testing.T) {
	defer Setup(t)()

	root := filepath.Join(Proj, t.Name())
	require.NoError(t, FSys.MkdirAll(root, 0o755))

	aFn := filepath.Join(root, "a.c")
	bFn := filepath.Join(root, "b.c")

	const srcA = "void a(void){\n\tTRice(iD(0), \"same\\n\");\n}\n"
	const srcB = "void b(void){\n\tTRice(iD(0), \"same\\n\");\n}\n"

	Srcs = nil
	Srcs = append(Srcs, root)
	Min = 100
	Max = 101
	SearchMethod = "upward"

	var firstA, firstB string
	for i := 0; i < 40; i++ {
		require.NoError(t, FSys.WriteFile(aFn, []byte(srcA), 0o644))
		require.NoError(t, FSys.WriteFile(bFn, []byte(srcB), 0o644))
		require.NoError(t, FSys.WriteFile(FnJSON, []byte("{}"), 0o644))
		require.NoError(t, FSys.WriteFile(LIFnJSON, []byte("{}"), 0o644))

		require.NoError(t, SubCmdIdInsert(W, FSys))

		aData, err := FSys.ReadFile(aFn)
		require.NoError(t, err)
		bData, err := FSys.ReadFile(bFn)
		require.NoError(t, err)

		aText := string(aData)
		bText := string(bData)
		if i == 0 {
			firstA = aText
			firstB = bText
			continue
		}
		assert.Equalf(t, firstA, aText, "iteration %d changed a.c output\nfirst:\n%s\ncurrent:\n%s", i, firstA, aText)
		assert.Equalf(t, firstB, bText, "iteration %d changed b.c output\nfirst:\n%s\ncurrent:\n%s", i, firstB, bText)
		if t.Failed() {
			tilData, tilErr := FSys.ReadFile(FnJSON)
			liData, liErr := FSys.ReadFile(LIFnJSON)
			require.NoError(t, tilErr)
			require.NoError(t, liErr)
			t.Fatalf("insert is nondeterministic on iteration %d\na.c:\n%s\nb.c:\n%s\ntil.json:\n%s\nli.json:\n%s", i, aText, bText, string(tilData), string(liData))
		}
	}

	assert.NotEmpty(t, firstA)
	assert.NotEmpty(t, firstB)
}
