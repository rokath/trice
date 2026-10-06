// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"errors"
	"io"
	"os"
	"path/filepath"
	"testing"
	"time"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// reportReadOnlyFs counts attempted mutations, including ones whose errors a
// caller might ignore. Reads retain the underlying filesystem's behavior.
type reportReadOnlyFs struct {
	afero.Fs
	writes int
}

// deny records an attempted mutation without allowing it to reach the fixture.
func (f *reportReadOnlyFs) deny() error { f.writes++; return os.ErrPermission }

// Create, directory operations, metadata changes and writable opens cover the
// filesystem mutation surface, not just the atomic writer used by Bind.
func (f *reportReadOnlyFs) Create(string) (afero.File, error)          { return nil, f.deny() }
func (f *reportReadOnlyFs) Mkdir(string, os.FileMode) error            { return f.deny() }
func (f *reportReadOnlyFs) MkdirAll(string, os.FileMode) error         { return f.deny() }
func (f *reportReadOnlyFs) Remove(string) error                        { return f.deny() }
func (f *reportReadOnlyFs) RemoveAll(string) error                     { return f.deny() }
func (f *reportReadOnlyFs) Rename(string, string) error                { return f.deny() }
func (f *reportReadOnlyFs) Chmod(string, os.FileMode) error            { return f.deny() }
func (f *reportReadOnlyFs) Chown(string, int, int) error               { return f.deny() }
func (f *reportReadOnlyFs) Chtimes(string, time.Time, time.Time) error { return f.deny() }

// OpenFile permits only read-only opens, so a discarded write error is visible.
func (f *reportReadOnlyFs) OpenFile(name string, flag int, perm os.FileMode) (afero.File, error) {
	if flag&(os.O_WRONLY|os.O_RDWR|os.O_CREATE|os.O_TRUNC|os.O_APPEND) != 0 {
		return nil, f.deny()
	}
	return f.Fs.OpenFile(name, flag, perm)
}

// reportFixture creates valid generated descriptors alongside handwritten and
// deliberately unreferenced files. There are no TIL or LI dictionaries at all.
func reportFixture(t *testing.T) (*afero.Afero, *reportReadOnlyFs, string) {
	t.Helper()
	BindDir = "generated"
	Srcs = ArrayFlag{"src"}
	ExcludeSrcs = nil
	GenerateBindReport = true
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	const owner = "trice_main_c_K1111111111111111.h"
	files := map[string]string{
		"src/main.c":         "#include \"" + owner + "\"\n#include \"" + owner + "\"\ntrice(\"hi\");\n",
		"generated/" + owner: "#define TRICE_BIND_FILE_KEY K1111111111111111\n#define TRICE_BIND_ROUTE_K1111111111111111 BIND\n",
		"generated/trice_triceCheck_c_K2222222222222222.h": "#define TRICE_BIND_FILE_KEY K2222222222222222\n#define TRICE_BIND_ROUTE_K2222222222222222 BIND\n",
		"generated/device_abc.h":                           "/* user-selected commands; never prune this */\n",
		"generated/trice-fields.txt":                       "temperature\n",
		"generated/subdir/kept.txt":                        "nested content is not inspected\n",
		"src/.hidden/ignored.c":                            "#include \"" + owner + "\"\n",
	}
	for path, content := range files {
		require.NoError(t, fs.MkdirAll(filepath.Dir(path), 0o755))
		require.NoError(t, fs.WriteFile(path, []byte(content), 0o640))
	}
	guard := &reportReadOnlyFs{Fs: fs.Fs}
	return fs, guard, owner
}

// reportTree snapshots names, bytes and modes; it independently catches changed
// and newly created files or directories even if a write bypasses the guard.
func reportTree(t *testing.T, fs *afero.Afero) map[string]string {
	t.Helper()
	state := make(map[string]string)
	require.NoError(t, fs.Walk(".", func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}
		state[path] = info.Mode().String()
		if !info.IsDir() {
			content, err := fs.ReadFile(path)
			if err != nil {
				return err
			}
			state[path] += ":" + string(content)
		}
		return nil
	}))
	return state
}

// TestBindReportMapsReferencesWithoutMutatingOrLoadingDictionaries proves the
// central contract, repeatability, deduplication and cautious unknown ownership.
func TestBindReportMapsReferencesWithoutMutatingOrLoadingDictionaries(t *testing.T) {
	defer Setup(t)()
	fs, guard, owner := reportFixture(t)
	before := reportTree(t, fs)
	var output bytes.Buffer
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Contains(t, output.String(), owner+" [sidecar]\n  File Key: K1111111111111111; owner sidecar: "+owner+"\n  Referenced in scan: src/main.c\n")
	assert.Contains(t, output.String(), "trice_triceCheck_c_K2222222222222222.h [sidecar]")
	assert.Contains(t, output.String(), "Not referenced in selected scan; owner source not established")
	assert.Contains(t, output.String(), "device_abc.h [unclassified]")
	assert.Contains(t, output.String(), "may be user-managed")
	assert.Contains(t, output.String(), "subdir [directory (not inspected recursively)]")
	assert.NotContains(t, output.String(), "kept.txt")
	assert.Contains(t, output.String(), "Scanned 1 source file(s)")
	assert.NotContains(t, output.String(), "src/main.c, src/main.c")
	first := output.String()
	output.Reset()
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Equal(t, first, output.String(), "reports must be stable across repeated inspection")
	assert.Equal(t, 0, guard.writes)
	assert.Equal(t, before, reportTree(t, fs))
}

// TestBindReportSeparatesHelperReferencesFromOwnerReferences distinguishes a
// helper still included in source from an old helper merely sharing an owner.
func TestBindReportSeparatesHelperReferencesFromOwnerReferences(t *testing.T) {
	defer Setup(t)()
	fs, guard, owner := reportFixture(t)
	for _, kind := range []string{"begin", "end"} {
		artifact := renderBindRebaseArtifact(owner, "K1111111111111111", "K1111111111111111_R0", kind)
		require.NoError(t, fs.WriteFile(filepath.Join(BindDir, artifact.name), artifact.content, 0o644))
	}
	begin := renderBindRebaseArtifact(owner, "K1111111111111111", "K1111111111111111_R0", "begin")
	source, err := fs.ReadFile("src/main.c")
	require.NoError(t, err)
	source = append(source, []byte("#include \""+begin.name+"\" // trice-bind: generated rebase begin K1111111111111111_R0\n")...)
	require.NoError(t, fs.WriteFile("src/main.c", source, 0o644))
	var output bytes.Buffer
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Contains(t, output.String(), begin.name+" [rebase begin]")
	assert.Contains(t, output.String(), "owner sidecar: "+owner+"\n  Referenced in scan: src/main.c\n  Owner sidecar referenced by: src/main.c")
	assert.Contains(t, output.String(), "Owner sidecar referenced by: src/main.c")
	assert.Contains(t, output.String(), "Not referenced in selected scan")
	assert.NotContains(t, output.String(), "UNVERIFIED")
	assert.Equal(t, 0, guard.writes)

	// Hand editing one helper must not make its filename trusted metadata.
	require.NoError(t, fs.WriteFile(filepath.Join(BindDir, begin.name), []byte("user changes\n"), 0o644))
	output.Reset()
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Contains(t, output.String(), "content differs from the generated helper; ownership unverified")
}

// TestBindReportShowsConflictsMissingFilesAndInvalidDeclarations preserves all
// evidence even when an ordinary Bind run would reject the project.
func TestBindReportShowsConflictsMissingFilesAndInvalidDeclarations(t *testing.T) {
	defer Setup(t)()
	fs, guard, owner := reportFixture(t)
	const missing = "trice_missing_c_K3333333333333333.h"
	require.NoError(t, fs.WriteFile("src/other.c", []byte("#include \""+owner+"\"\n#include \""+missing+"\"\n"), 0o644))
	require.NoError(t, fs.WriteFile("generated/"+owner, []byte("// #define TRICE_BIND_FILE_KEY K1111111111111111\n#define TRICE_BIND_ROUTE_K1111111111111111 BIND\n"), 0o644))
	artifact := renderBindRebaseArtifact(missing, "K3333333333333333", "K3333333333333333_R2", "end")
	require.NoError(t, fs.WriteFile(filepath.Join(BindDir, artifact.name), artifact.content, 0o644))
	var output bytes.Buffer
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Contains(t, output.String(), "AMBIGUOUS: File Key claimed by multiple sources: src/main.c, src/other.c")
	assert.Contains(t, output.String(), "no valid TRICE_BIND_FILE_KEY definition")
	assert.Contains(t, output.String(), missing+" [missing]")
	assert.Contains(t, output.String(), "owner sidecar is missing or has invalid declarations")
	assert.Equal(t, 0, guard.writes)
}

// TestBindReportUsesScanScopeRatherThanGuessingCompilerUsage demonstrates why
// partial scans, comments and excluded files cannot establish global ownership.
func TestBindReportUsesScanScopeRatherThanGuessingCompilerUsage(t *testing.T) {
	defer Setup(t)()
	fs, guard, owner := reportFixture(t)
	require.NoError(t, fs.WriteFile("src/comment.c", []byte("// #include \""+owner+"\"\n#include \"ordinary.h\" // \""+owner+"\"\n#include SELECT(\""+owner+"\")\n"), 0o644))
	Srcs = ArrayFlag{"src", "src/main.c"}
	ExcludeSrcs = ArrayFlag{"src/main.c"}
	var output bytes.Buffer
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Contains(t, output.String(), "Exclusions: src/main.c")
	assert.NotContains(t, output.String(), "Referenced in scan:")
	assert.NotContains(t, output.String(), "AMBIGUOUS")
	assert.Contains(t, output.String(), "preprocessor conditions and compiler use are not evaluated")
	assert.Equal(t, 0, guard.writes)
	// A physical include inside #if 0 still belongs to this lexical scan;
	// the report explicitly does not claim that a compiler activates it.
	require.NoError(t, fs.WriteFile("src/comment.c", []byte("#if 0\n#include <"+owner+">\n#endif\n"), 0o644))
	output.Reset()
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Contains(t, output.String(), "Referenced in scan: src/comment.c")
}

// TestBindReportRejectsWriteModesAndIncompleteInputs checks actionable errors
// before inventory publication and zero writes, including to missing folders.
func TestBindReportRejectsWriteModesAndIncompleteInputs(t *testing.T) {
	for _, name := range []string{"logC", "onelineJSON", "abc", "colors", "empty directory", "missing directory", "directory is file", "missing source", "unreadable artifact", "unreadable source", "unreadable directory"} {
		t.Run(name, func(t *testing.T) {
			defer Setup(t)()
			fs, guard, owner := reportFixture(t)
			var expected string
			var inspected afero.Fs = guard
			switch name {
			case "logC":
				GenerateLogC = true
				expected = "cannot be combined"
			case "onelineJSON":
				GenerateOneLineJSON = true
				expected = "cannot be combined"
			case "abc":
				GenerateABC = "device"
				expected = "cannot be combined"
			case "colors":
				WriteAllColors = true
				expected = "cannot be combined"
			case "empty directory":
				BindDir = " "
				expected = "-genDir must not be empty"
			case "missing directory":
				BindDir = "absent"
				expected = "cannot read generated directory absent"
			case "directory is file":
				BindDir = "src/main.c"
				expected = "cannot read generated directory src/main.c"
			case "missing source":
				Srcs = ArrayFlag{"absent.c"}
				expected = "cannot inspect source root absent.c"
			case "unreadable artifact":
				inspected = &bindMetadataReadFailFs{Fs: guard, path: "generated/" + owner}
				expected = "cannot read artifact"
			case "unreadable source":
				inspected = &bindMetadataReadFailFs{Fs: guard, path: "src/main.c"}
				expected = "incomplete source scan"
			case "unreadable directory":
				inspected = &bindMetadataReadFailFs{Fs: guard, path: "generated"}
				expected = "cannot read generated directory"
			}
			before := reportTree(t, fs)
			var output bytes.Buffer
			err := SubCmdGenerate(&output, &afero.Afero{Fs: inspected})
			require.ErrorContains(t, err, expected)
			assert.Empty(t, output.String(), "a failed inspection must not publish a misleading partial inventory")
			assert.Equal(t, 0, guard.writes)
			assert.Equal(t, before, reportTree(t, fs))
		})
	}
}

// reportFailWriter models a closed pipe; callers must receive output errors.
type reportFailWriter struct{}

// Write always fails rather than pretending that the report reached its sink.
func (reportFailWriter) Write([]byte) (int, error) { return 0, io.ErrClosedPipe }

// TestBindReportEmptyInventoryAndOutputErrors covers useful empty reports,
// default source selection, a custom directory and propagation of sink errors.
func TestBindReportEmptyInventoryAndOutputErrors(t *testing.T) {
	defer Setup(t)()
	fs, guard, _ := reportFixture(t)
	BindDir = "custom-output"
	require.NoError(t, fs.MkdirAll(BindDir, 0o755))
	Srcs = nil
	var output bytes.Buffer
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Contains(t, output.String(), "Bind artifact report: custom-output")
	assert.Contains(t, output.String(), "Source roots: .")
	assert.Contains(t, output.String(), "0 directory entry/entries")
	assert.Nil(t, Srcs, "default selection must not alter the caller's flags")
	require.NoError(t, SubCmdGenerate(nil, &afero.Afero{Fs: guard}), "a nil output writer must safely discard the report")
	assert.True(t, errors.Is(SubCmdGenerate(reportFailWriter{}, &afero.Afero{Fs: guard}), io.ErrClosedPipe))
	assert.Equal(t, 0, guard.writes)
}

// TestBindReportReadsActualBindOutput makes the producer and the report agree
// on owner identity without needing to rerun Bind or modify any artifact.
func TestBindReportReadsActualBindOutput(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"main.c": "#include \"trice.h\"\nvoid log_event(void) { trice(\"msg:hello\"); }\n"})()
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	GenerateBindReport = true
	var output bytes.Buffer
	guard := &reportReadOnlyFs{Fs: FSys.Fs}
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Contains(t, output.String(), "trice_main_c_K1111111111111111.h [sidecar]")
	assert.Contains(t, output.String(), "Referenced in scan: "+filepath.ToSlash(Srcs[0]))
	assert.NotContains(t, output.String(), "UNVERIFIED")
	assert.NotContains(t, output.String(), "AMBIGUOUS")
	assert.Equal(t, 0, guard.writes)
}

// TestBindReportDeduplicatesRootsAndTracksMovedOwners keeps physical source
// identity independent of scan-root spelling and the filename in an old sidecar.
func TestBindReportDeduplicatesRootsAndTracksMovedOwners(t *testing.T) {
	defer Setup(t)()
	fs, guard, owner := reportFixture(t)
	Srcs = ArrayFlag{"src", "src/main.c", "./src"}
	var output bytes.Buffer
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Contains(t, output.String(), "Scanned 1 source file(s)")
	assert.NotContains(t, output.String(), "AMBIGUOUS")
	require.NoError(t, fs.Rename("src/main.c", "src/renamed.c"))
	Srcs = ArrayFlag{"src"}
	output.Reset()
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Contains(t, output.String(), owner+" [sidecar]")
	assert.Contains(t, output.String(), "Referenced in scan: src/renamed.c")
	assert.NotContains(t, output.String(), "src/main.c")
	assert.Equal(t, 0, guard.writes)
}

// TestBindReportChecksSidecarDeclarations prevents a convincing filename from
// masking mismatched identity, routes or declarations that exist only in comments.
func TestBindReportChecksSidecarDeclarations(t *testing.T) {
	for _, tc := range []struct{ name, content, problem string }{
		{"wrong key", "#define TRICE_BIND_FILE_KEY K9999999999999999\n#define TRICE_BIND_ROUTE_K1111111111111111 BIND\n", "sidecar declares key K9999999999999999"},
		{"wrong route key", "#define TRICE_BIND_FILE_KEY K1111111111111111\n#define TRICE_BIND_ROUTE_K9999999999999999 BIND\n", "sidecar declares route for key K9999999999999999"},
		{"missing route", "#define TRICE_BIND_FILE_KEY K1111111111111111\n", "existing Trice bind sidecar has no valid BIND route definition"},
		{"commented declarations", "/*\n#define TRICE_BIND_FILE_KEY K1111111111111111\n#define TRICE_BIND_ROUTE_K1111111111111111 BIND\n*/\n", "existing Trice bind sidecar has no valid TRICE_BIND_FILE_KEY definition"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			defer Setup(t)()
			fs, guard, owner := reportFixture(t)
			require.NoError(t, fs.WriteFile(filepath.Join(BindDir, owner), []byte(tc.content), 0o644))
			var output bytes.Buffer
			require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
			assert.Contains(t, output.String(), "UNVERIFIED: "+tc.problem)
			assert.Contains(t, output.String(), "Referenced in scan: src/main.c", "invalid declarations must not hide the physical include")
			assert.Equal(t, 0, guard.writes)
		})
	}
}

// TestBindReportDoesNotFollowArtifactSymlinks checks a real directory entry;
// a dangling target proves the report cannot silently read through the link.
func TestBindReportDoesNotFollowArtifactSymlinks(t *testing.T) {
	defer Setup(t)()
	root := t.TempDir()
	BindDir = filepath.Join(root, "generated")
	Srcs = ArrayFlag{filepath.Join(root, "src")}
	GenerateBindReport = true
	fs := &afero.Afero{Fs: afero.NewOsFs()}
	require.NoError(t, fs.MkdirAll(BindDir, 0o755))
	require.NoError(t, fs.MkdirAll(Srcs[0], 0o755))
	const name = "trice_main_c_K1111111111111111.h"
	if err := os.Symlink("absent-target.h", filepath.Join(BindDir, name)); err != nil {
		t.Skipf("artifact symlink check requires local symlink support: %v", err)
	}
	var output bytes.Buffer
	guard := &reportReadOnlyFs{Fs: fs.Fs}
	require.NoError(t, SubCmdGenerate(&output, &afero.Afero{Fs: guard}))
	assert.Contains(t, output.String(), name+" [non-regular entry (not followed)]")
	assert.NotContains(t, output.String(), "[sidecar]")
	assert.Equal(t, 0, guard.writes)
}
