// SPDX-License-Identifier: MIT

package scripts_test

import (
	"archive/zip"
	"bytes"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestManualEPUBGeneration checks publication, tool preflight and conversion
// failures through the public script without invoking a real package installer.
func TestManualEPUBGeneration(t *testing.T) {
	for _, tc := range []struct {
		name, failure, version string
	}{
		{"both_books", "", "3.8.3"},
		{"release_tag", "", "3.8.3"},
		{"missing_converter", "missing", "3.8.3"},
		{"failed_version_command", "version", "3.8.3"},
		{"wrong_converter", "", "3.0"},
		{"failed_preparation", "prepare", "3.8.3"},
		{"failed_conversion", "convert", "3.8.3"},
		{"failed_second_book", "convert_reference", "3.8.3"},
		{"empty_output", "empty", "3.8.3"},
		{"unreadable_output", "read", "3.8.3"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_325_generate_trice_manual_epubs.sh", "scripts/_manual_epub.css")
			// Fixed Git facts distinguish the source date from generation time
			// and keep metadata assertions independent of the developer checkout.
			writeFixture(t, root, "bin/git", `#!/bin/sh
case "$*" in
  'describe --tags --always --dirty') echo v2.0.0-beta-4-2-g123456789abc-dirty ;;
  'rev-parse --short=12 HEAD') echo 123456789abc ;;
  'log -1 --format=%cs') echo 2026-11-05 ;;
  *) exit 94 ;;
esac
`)
			// Model only the shared preparation boundary; the PDF preparation
			// itself has separate tests for real chapter and link processing.
			writeFixture(t, root, "scripts/_320_generate_trice_user_manual_pdf.sh", `#!/bin/sh
set -eu
[ "$1" = prepare ] || exit 91
touch preparation-called
[ "$EPUB_FAILURE" != prepare ] || exit 23
mkdir -p temp/manual-pdf/chapters
printf '<h1>User manual</h1>\n' > temp/manual-pdf/chapters/TriceUserManual.html
printf '<h1>Reference manual</h1>\n' > temp/manual-pdf/TriceReferenceManual.html
`)
			writeFixture(t, root, "bin/pandoc", `#!/bin/sh
set -eu
if [ "$1" = --version ]; then
  [ "$EPUB_FAILURE" != version ] || exit 26
  printf 'pandoc %s\n' "$EPUB_VERSION"
  exit 0
fi
output=
mode=convert
for argument do
  case "$argument" in --output=*) output="${argument#--output=}" ;; --from=epub) mode=read ;; esac
done
[ -n "$output" ] || exit 92
printf '%s\n' "$*" >> "$EPUB_CALLS"
if [ "$mode" = convert ]; then
  [ "$EPUB_FAILURE" != convert ] || exit 24
  if [ "$EPUB_FAILURE" = convert_reference ] && [ "$1" = TriceReferenceManual.html ]; then exit 24; fi
  [ "$EPUB_FAILURE" != empty ] || exit 0
  printf 'generated %s\n' "$1" > "$output"
else
  [ -s "$1" ] || exit 93
  [ "$EPUB_FAILURE" != read ] || exit 25
  printf 'read %s\n' "$1" > "$output"
fi
`)
			for _, manual := range []string{"TriceUserManual", "TriceReferenceManual"} {
				writeFixture(t, root, "docs/"+manual+".epub", "previous "+manual)
			}
			command := "bash scripts/_325_generate_trice_manual_epubs.sh"
			if tc.failure == "missing" {
				// Hide the host converter too, so a local installation cannot
				// accidentally satisfy the missing-tool fixture.
				command = `command() { if [ "$1" = -v ] && [ "$2" = pandoc ]; then return 1; fi; builtin command "$@"; }; source scripts/_325_generate_trice_manual_epubs.sh`
			}
			releaseTag := ""
			if tc.name == "release_tag" {
				releaseTag = "v2.0.0-beta-4"
			}
			out, err := runFixture(t, root, command, map[string]string{
				"EPUB_FAILURE": tc.failure, "EPUB_VERSION": tc.version,
				"GORELEASER_CURRENT_TAG": releaseTag,
				"EPUB_CALLS":             filepath.Join(root, "converter-calls"),
			})
			if tc.failure != "" || tc.version != "3.8.3" {
				assert.Error(t, err, out)
				for _, manual := range []string{"TriceUserManual", "TriceReferenceManual"} {
					data, readErr := os.ReadFile(filepath.Join(root, "docs", manual+".epub"))
					require.NoError(t, readErr)
					assert.Equal(t, "previous "+manual, string(data), "failed generation preserves prior release assets")
				}
				if tc.version != "3.8.3" {
					assert.NoFileExists(t, filepath.Join(root, "preparation-called"))
					assert.Contains(t, out, "expected pandoc 3.8.3")
				}
				return
			}
			assert.NoError(t, err, out)
			calls, readErr := os.ReadFile(filepath.Join(root, "converter-calls"))
			require.NoError(t, readErr)
			assert.Equal(t, 4, strings.Count(string(calls), "--from="), "both books are converted and read back")
			assert.Equal(t, 2, strings.Count(string(calls), "--to=epub3"))
			assert.Equal(t, 2, strings.Count(string(calls), "--split-level=2"))
			expectedVersion := "v2.0.0-beta-4-2-g123456789abc-dirty"
			if releaseTag != "" {
				expectedVersion = releaseTag
			}
			assert.Contains(t, string(calls), "Trice User Manual ("+expectedVersion+")")
			assert.Contains(t, string(calls), "Trice Reference Manual ("+expectedVersion+")")
			assert.Equal(t, 2, strings.Count(string(calls), "Source commit: 123456789abc (2026-11-05). Generated:"))
			assert.Regexp(t, `--metadata=date:\d{4}-\d{2}-\d{2}( |\n)`, string(calls))
			assert.Regexp(t, `Generated: \d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z`, string(calls))
			for _, manual := range []string{"TriceUserManual", "TriceReferenceManual"} {
				data, readErr := os.ReadFile(filepath.Join(root, "docs", manual+".epub"))
				require.NoError(t, readErr)
				assert.Equal(t, "generated "+manual+".html\n", string(data))
			}
		})
	}
}

// TestPublishedEPUBContainers rejects missing, damaged and incorrectly labelled
// downloads through the same shell step used by the post-publication audit.
func TestPublishedEPUBContainers(t *testing.T) {
	for _, failure := range []string{"", "missing", "damaged", "wrong_mimetype"} {
		t.Run("failure="+failure, func(t *testing.T) {
			root := t.TempDir()
			for _, manual := range []string{"TriceUserManual", "TriceReferenceManual"} {
				if manual == "TriceReferenceManual" && failure == "missing" {
					continue
				}
				var data bytes.Buffer
				book := zip.NewWriter(&data)
				entry, err := book.CreateHeader(&zip.FileHeader{Name: "mimetype", Method: zip.Store})
				require.NoError(t, err)
				mimetype := "application/epub+zip"
				if manual == "TriceReferenceManual" && failure == "wrong_mimetype" {
					mimetype = "text/plain"
				}
				_, err = entry.Write([]byte(mimetype))
				require.NoError(t, err)
				require.NoError(t, book.Close())
				content := data.String()
				if manual == "TriceReferenceManual" && failure == "damaged" {
					content = "not a ZIP file"
				}
				writeFixture(t, root, "artifacts/"+manual+".epub", content)
			}
			out, err := runFixture(t, root, workflowShellStep(t, "release-audit.yml", "Verify release EPUB containers"), nil)
			if failure == "" {
				assert.NoError(t, err, out)
			} else {
				assert.Error(t, err, out)
				assert.Contains(t, out, "::error::")
				assert.Contains(t, out, "TriceReferenceManual.epub")
			}
		})
	}
}
