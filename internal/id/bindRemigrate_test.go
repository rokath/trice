// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// bindFailRemoveFs injects a sidecar deletion failure after source replacements
// so the shared transaction rollback can be verified.
type bindFailRemoveFs struct {
	afero.Fs
	destination string
}

// Remove rejects the selected sidecar and delegates all other removals.
func (f *bindFailRemoveFs) Remove(name string) error {
	if name == f.destination {
		return fmt.Errorf("injected remove failure for %s", name)
	}
	return f.Fs.Remove(name)
}

// writeValidBindSidecar creates the minimum generated identity required by the
// production sidecar validator.
func writeValidBindSidecar(t *testing.T, name, key string) string {
	t.Helper()
	path := filepath.Join(BindDir, name)
	require.NoError(t, FSys.MkdirAll(BindDir, 0o755))
	content := "#define TRICE_BIND_FILE_KEY " + key + "\n#define TRICE_BIND_ROUTE_" + key + " BIND\n"
	require.NoError(t, FSys.WriteFile(path, []byte(content), 0o644))
	return path
}

// TestBindRemigrationRemovesValidatedArtifacts verifies exact source-byte
// preservation, repeated owner reactivation, sidecar deletion, and idempotence.
func TestBindRemigrationRemovesValidatedArtifacts(t *testing.T) {
	key := "K0123456789ABCDEF"
	name := "trice_module_c_" + key + ".h"
	include := "#include \"" + name + "\" // trice-bind: generated"
	source := "#include \"trice.h\"\r\n" + include + "\r\ntrice(\"first\");\r\n#include \"module.h\"\r\n" + include + "\r\ntrice(id(0), \"second\");\r\n"
	expected := "#include \"trice.h\"\r\ntrice(\"first\");\r\n#include \"module.h\"\r\ntrice(id(0), \"second\");\r\n"
	defer prepareBindTest(t, map[string]string{"module.c": source})()
	sidecarPath := writeValidBindSidecar(t, name, key)

	require.NoError(t, SubCmdIdRemigrateBindToClean(io.Discard, FSys))
	after, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, expected, string(after))
	_, err = FSys.Stat(sidecarPath)
	assert.ErrorIs(t, err, os.ErrNotExist)

	require.NoError(t, SubCmdIdRemigrateBindToClean(io.Discard, FSys))
	afterSecondRun, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, after, afterSecondRun)
}

// TestBindRemigrationRejectsAmbiguousOwnership ensures one generated sidecar
// can never be deleted when two managed physical files claim it.
func TestBindRemigrationRejectsAmbiguousOwnership(t *testing.T) {
	key := "K0123456789ABCDEF"
	name := "trice_shared_c_" + key + ".h"
	include := "#include \"" + name + "\"\n"
	defer prepareBindTest(t, map[string]string{
		"a.c": include + "trice(\"a\");\n",
		"b.c": include + "trice(\"b\");\n",
	})()
	sidecarPath := writeValidBindSidecar(t, name, key)
	before := make(map[string][]byte)
	for _, path := range Srcs {
		content, err := FSys.ReadFile(path)
		require.NoError(t, err)
		before[path] = content
	}

	var output bytes.Buffer
	err := SubCmdIdRemigrateBindToClean(&output, FSys)
	require.Error(t, err)
	assert.Contains(t, output.String(), "multiple owner files")
	for path, expected := range before {
		actual, readErr := FSys.ReadFile(path)
		require.NoError(t, readErr)
		assert.Equal(t, expected, actual)
	}
	_, statErr := FSys.Stat(sidecarPath)
	require.NoError(t, statErr)
}

// TestBindRemigrationRejectsUnverifiableArtifacts covers missing and corrupt
// sidecars while proving that neither case changes its owning source.
func TestBindRemigrationRejectsUnverifiableArtifacts(t *testing.T) {
	tests := []struct {
		name           string
		createSidecar  bool
		sidecarContent string
		expected       string
	}{
		{name: "missing", expected: "does not exist"},
		{name: "corrupt", createSidecar: true, sidecarContent: "#define SOMETHING_ELSE 1\n", expected: "no valid TRICE_BIND_FILE_KEY"},
	}
	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			key := "K0123456789ABCDEF"
			name := "trice_owner_c_" + key + ".h"
			source := "#include \"" + name + "\"\ntrice(\"unchanged\");\n"
			defer prepareBindTest(t, map[string]string{"owner.c": source})()
			if test.createSidecar {
				require.NoError(t, FSys.MkdirAll(BindDir, 0o755))
				require.NoError(t, FSys.WriteFile(filepath.Join(BindDir, name), []byte(test.sidecarContent), 0o644))
			}

			var output bytes.Buffer
			err := SubCmdIdRemigrateBindToClean(&output, FSys)
			require.Error(t, err)
			assert.Contains(t, output.String(), test.expected)
			after, readErr := FSys.ReadFile(Srcs[0])
			require.NoError(t, readErr)
			assert.Equal(t, source, string(after))
		})
	}
}

// TestBindRemigrationRollsBackDeleteFailure proves that a late sidecar removal
// error restores every source already replaced by the transaction.
func TestBindRemigrationRollsBackDeleteFailure(t *testing.T) {
	key := "K0123456789ABCDEF"
	name := "trice_rollback_c_" + key + ".h"
	source := "#include \"" + name + "\"\ntrice(\"rollback\");\n"
	defer prepareBindTest(t, map[string]string{"rollback.c": source})()
	sidecarPath := writeValidBindSidecar(t, name, key)
	failing := &bindFailRemoveFs{Fs: FSys.Fs, destination: sidecarPath}
	failingFileSystem := &afero.Afero{Fs: failing}

	err := SubCmdIdRemigrateBindToClean(io.Discard, failingFileSystem)
	require.Error(t, err)
	after, readErr := FSys.ReadFile(Srcs[0])
	require.NoError(t, readErr)
	assert.Equal(t, source, string(after))
	_, statErr := FSys.Stat(sidecarPath)
	require.NoError(t, statErr)
}
