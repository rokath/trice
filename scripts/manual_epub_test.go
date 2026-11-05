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

// TestManualEPUBPreparationSharesBrowserSetup executes both real wrappers.
// Tool doubles expose missing cache propagation without installing a browser or
// borrowing the developer's existing npm modules and browser cache.
func TestManualEPUBPreparationSharesBrowserSetup(t *testing.T) {
	for _, tc := range []struct {
		name                          string
		warm, afterPDF, custom, fails bool
	}{
		{name: "standalone_cold_cache"},
		{name: "standalone_warm_cache", warm: true},
		{name: "separate_PDF_and_EPUB_hooks", afterPDF: true},
		{name: "explicit_cache_with_spaces", custom: true},
		{name: "failed_browser_preparation", fails: true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_320_generate_trice_user_manual_pdf.sh", "scripts/_325_generate_trice_manual_epubs.sh")
			// Preserve the real entry-point ordering; replace only preparation
			// and external tools so a cold-cache case cannot download anything.
			setup, err := os.ReadFile(filepath.Join(root, "scripts", "_105_setup_test_tools.sh"))
			require.NoError(t, err)
			writeFixture(t, root, "scripts/_105_setup_test_tools.sh", string(setup)+`
trice_test_tool_plan() {
  [ "$1" = pdf ] || return 91
  export PUPPETEER_CACHE_DIR="${PUPPETEER_CACHE_DIR:-$PWD/temp/tools/puppeteer}"
  export TRICE_TEST_NPM_PLAN="$PWD/renderer-plan"
}
ensure_node_test_tool() { return 0; }
prepare_selected_npm_tools() {
  [ "$FIXTURE_SETUP_FAILURE" != true ] || return 23
  if [ ! -f "$PUPPETEER_CACHE_DIR/browser" ]; then
    mkdir -p "$PUPPETEER_CACHE_DIR" || return 24
    printf 'prepared browser\n' > "$PUPPETEER_CACHE_DIR/browser"
    printf 'install\n' >> browser-installs
  fi
  export TRICE_TEST_NPM_DIR="$PWD/renderer"
  export TRICE_TEST_PDF_CLI=scripts/fixture-pdf.cjs
}
`)
			writeFixture(t, root, "docs/TriceUserManual.md", "# User Manual\n")
			writeFixture(t, root, "docs/rm/README.md", "# Reference Manual\n")
			writeFixture(t, root, "docs/ref/fixture.txt", "reference\n")
			for _, manual := range []string{"TriceUserManual", "TriceReferenceManual"} {
				writeFixture(t, root, "docs/"+manual+".pdf", "previous PDF\n")
				writeFixture(t, root, "docs/"+manual+".epub", "previous EPUB\n")
			}
			writeFixture(t, root, "bin/git", `#!/bin/sh
case "$*" in
  'describe --tags --always --dirty') echo v2.0.0-beta-4 ;;
  'rev-parse --short=12 HEAD') echo 123456789abc ;;
  'log -1 --format=%cs') echo 2026-11-05 ;;
  *) exit 92 ;;
esac
`)
			writeFixture(t, root, "bin/stitchmd", `#!/bin/sh
if [ "$1" = -version ]; then echo 'stitchmd 0.9.0'; exit 0; fi
printf '# Reference Manual\n' > temp/manual-pdf/TriceReferenceManual.md
`)
			writeFixture(t, root, "bin/node", `#!/bin/sh
set -eu
if [ "$1" = --version ]; then echo v26.12.1; exit 0; fi
[ -s "${PUPPETEER_CACHE_DIR:-missing-cache}/browser" ] || { echo 'missing prepared browser' >&2; exit 93; }
[ "$(cd "$PUPPETEER_CACHE_DIR" && pwd -P)" = "$(cd "$FIXTURE_BROWSER_CACHE" && pwd -P)" ] || exit 94
case "$1" in
  scripts/_315_prepare_reference_manual.cjs)
    mkdir -p temp/manual-pdf/chapters
    case "${2:-}" in
      '') printf '# Reference Manual\n' > temp/manual-pdf/SUMMARY.md ;;
      --verify) printf '<h1>Reference Manual</h1>\n' > temp/manual-pdf/TriceReferenceManual.html ;;
      --epub)
        printf '<h1>User Manual</h1>\n' > temp/manual-pdf/chapters/TriceUserManual.html
        printf 'prepared EPUB images\n' >> epub-images
        ;;
      *) exit 95 ;;
    esac
    ;;
  scripts/fixture-pdf.cjs)
    shift
    printf 'rendered PDF\n' > "${1%.md}.pdf"
    ;;
  *) exit 96 ;;
esac
`)
			writeFixture(t, root, "bin/npx", "#!/bin/sh\necho 'unexpected npm installation' >&2\nexit 97\n")
			writeFixture(t, root, "bin/pandoc", `#!/bin/sh
set -eu
if [ "$1" = --version ]; then echo 'pandoc 3.8.3'; exit 0; fi
for argument do
  case "$argument" in --output=*) printf 'converted book\n' > "${argument#--output=}" ;; esac
done
`)
			cache := filepath.Join(root, "temp", "tools", "puppeteer")
			if tc.custom {
				cache = filepath.Join(root, "custom browser cache")
			}
			if tc.warm {
				writeFixture(t, root, "temp/tools/puppeteer/browser", "existing browser\n")
			}
			environment := map[string]string{
				"FIXTURE_BROWSER_CACHE": filepath.ToSlash(cache),
				"FIXTURE_SETUP_FAILURE": booleanText(tc.fails),
			}
			if tc.custom {
				environment["PUPPETEER_CACHE_DIR"] = filepath.ToSlash(cache)
			}
			command := "set -e\n"
			if tc.afterPDF {
				// Separate shells must rediscover the same browser; exports from
				// the first GoReleaser hook cannot initialize the second hook.
				command += "bash scripts/_320_generate_trice_user_manual_pdf.sh\n"
			}
			command += "bash scripts/_325_generate_trice_manual_epubs.sh"
			out, err := runFixture(t, root, command, environment)
			if tc.fails {
				assert.Error(t, err, out)
				assert.Contains(t, out, "cannot prepare the selected PDF toolchain")
				assert.NoFileExists(t, filepath.Join(root, "epub-images"))
			} else {
				require.NoError(t, err, out)
				images, readErr := os.ReadFile(filepath.Join(root, "epub-images"))
				require.NoError(t, readErr)
				assert.Equal(t, "prepared EPUB images\n", string(images))
				if tc.warm {
					assert.NoFileExists(t, filepath.Join(root, "browser-installs"))
				} else {
					installs, readErr := os.ReadFile(filepath.Join(root, "browser-installs"))
					require.NoError(t, readErr)
					assert.Equal(t, "install\n", string(installs), "a second hook reuses the first hook's browser")
				}
			}
			for _, manual := range []string{"TriceUserManual", "TriceReferenceManual"} {
				epub, readErr := os.ReadFile(filepath.Join(root, "docs", manual+".epub"))
				require.NoError(t, readErr)
				wantEPUB := "converted book\n"
				if tc.fails {
					wantEPUB = "previous EPUB\n"
				}
				assert.Equal(t, wantEPUB, string(epub))
				pdf, readErr := os.ReadFile(filepath.Join(root, "docs", manual+".pdf"))
				require.NoError(t, readErr)
				wantPDF := "previous PDF\n"
				if tc.afterPDF {
					wantPDF = "rendered PDF\n"
				}
				assert.Equal(t, wantPDF, string(pdf), "EPUB preparation never renders or replaces PDFs")
			}
		})
	}
}

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
