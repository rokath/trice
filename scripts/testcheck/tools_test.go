// SPDX-License-Identifier: MIT

package main

import (
	"archive/zip"
	"bytes"
	"crypto/sha256"
	"encoding/json"
	"fmt"
	"net/http"
	"net/http/httptest"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"sync/atomic"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestPinnedDownloadValidatesCache checks cold/warm/corrupt downloads against
// independent expected bytes. A failed checksum cannot become an installation.
func TestPinnedDownloadValidatesCache(t *testing.T) {
	data := []byte("verified publisher archive")
	digest := fmt.Sprintf("%x", sha256.Sum256(data))
	var requests atomic.Int32
	server := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		requests.Add(1)
		_, _ = w.Write(data)
	}))
	defer server.Close()
	archive := filepath.Join(t.TempDir(), "tool.zip")
	require.NoError(t, downloadTool(server.URL, archive, digest))
	require.NoError(t, downloadTool(server.URL, archive, digest))
	assert.Equal(t, int32(1), requests.Load(), "warm verified cache performs no HTTP request")
	require.NoError(t, os.WriteFile(archive, []byte("interrupted or changed cache"), 0o644))
	require.NoError(t, downloadTool(server.URL, archive, digest))
	assert.Equal(t, int32(2), requests.Load())
	assert.ErrorContains(t, downloadTool(server.URL, archive, fmt.Sprintf("%064d", 0)), "SHA-256 mismatch")
	got, err := os.ReadFile(archive)
	require.NoError(t, err)
	assert.Equal(t, data, got, "failed download must retain the last verified archive")
}

// TestExecutableExtractionCannotEscape verifies that only executable content
// is copied, even when an archive contains a traversal-shaped member name.
func TestExecutableExtractionCannotEscape(t *testing.T) {
	root := t.TempDir()
	var buffer bytes.Buffer
	writer := zip.NewWriter(&buffer)
	entry, err := writer.Create("../../tool")
	require.NoError(t, err)
	_, err = entry.Write([]byte("selected executable"))
	require.NoError(t, err)
	require.NoError(t, writer.Close())
	archive := filepath.Join(root, "release.zip")
	require.NoError(t, os.WriteFile(archive, buffer.Bytes(), 0o644))
	require.NoError(t, os.Mkdir(filepath.Join(root, "stage"), 0o755))
	destination := filepath.Join(root, "stage", "tool")
	require.NoError(t, extractTool(archive, "tool", destination))
	got, err := os.ReadFile(destination)
	require.NoError(t, err)
	assert.Equal(t, "selected executable", string(got))
	assert.NoFileExists(t, filepath.Join(root, "tool"))
	assert.ErrorContains(t, extractTool(archive, "missing", destination), "lacks executable")
}

// TestToolConfigRejectsExecutableSyntax keeps the shared manifest data-only in
// the Go loader and reports malformed definitions before any download starts.
func TestToolConfigRejectsExecutableSyntax(t *testing.T) {
	file := filepath.Join(t.TempDir(), "tools.env")
	require.NoError(t, os.WriteFile(file, []byte("# comment\nTRICE_TOOL_EXAMPLE_VERSION=1.2.3\n"), 0o644))
	values, _, err := readToolConfig(file)
	require.NoError(t, err)
	assert.Equal(t, "1.2.3", values["TRICE_TOOL_EXAMPLE_VERSION"])
	require.NoError(t, os.WriteFile(file, []byte("run a command\n"), 0o644))
	_, _, err = readToolConfig(file)
	assert.ErrorContains(t, err, "invalid tool configuration")
}

// TestInstallerIgnoresWrongGlobalAndIncompleteCache uses a real native helper
// executable. A mismatched PATH entry and abandoned staging files must not
// change cold/warm selection or allow a corrupted publication to be reused.
func TestInstallerIgnoresWrongGlobalAndIncompleteCache(t *testing.T) {
	root := t.TempDir()
	name := "trice-fixture-tool"
	filename := name
	if runtime.GOOS == "windows" {
		filename += ".exe"
	}
	source := filepath.Join(root, "fixture.go")
	require.NoError(t, os.WriteFile(source, []byte("package main\nimport \"fmt\"\nfunc main() { fmt.Println(\"fixture 1.2.3\") }\n"), 0o644))
	compiled := filepath.Join(root, filename)
	command := exec.Command("go", "build", "-o", compiled, source)
	output, err := command.CombinedOutput()
	require.NoError(t, err, string(output))
	data, err := os.ReadFile(compiled)
	require.NoError(t, err)
	globalDir := filepath.Join(root, "global")
	require.NoError(t, os.Mkdir(globalDir, 0o755))
	// The test executable rejects --version, unlike the declared tool.
	testExecutable, err := os.Executable()
	require.NoError(t, err)
	wrong, err := os.ReadFile(testExecutable)
	require.NoError(t, err)
	require.NoError(t, os.WriteFile(filepath.Join(globalDir, filename), wrong, 0o755))
	t.Setenv("PATH", globalDir+string(os.PathListSeparator)+os.Getenv("PATH"))
	var requests atomic.Int32
	server := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		requests.Add(1)
		_, _ = w.Write(data)
	}))
	defer server.Close()
	platform := strings.ToUpper(runtime.GOOS + "_" + runtime.GOARCH)
	config := filepath.Join(root, "tools.env")
	manifest := fmt.Sprintf("TRICE_TOOL_TRICE_FIXTURE_TOOL_VERSION=1.2.3\nTRICE_TOOL_TRICE_FIXTURE_TOOL_URL_%s=%s/tool\nTRICE_TOOL_TRICE_FIXTURE_TOOL_SHA_%s=%x\n", platform, server.URL, platform, sha256.Sum256(data))
	require.NoError(t, os.WriteFile(config, []byte(manifest), 0o644))
	cache := filepath.Join(root, "cache")
	_, err = installTools(config, cache, []string{name}, false)
	assert.ErrorIs(t, err, errInstallRequired)
	assert.Zero(t, requests.Load(), "declined setup cannot download")
	bin, err := installTools(config, cache, []string{name}, true)
	require.NoError(t, err)
	selected := filepath.Join(bin, filename)
	assert.True(t, toolIdentity(selected, "1.2.3", ""))
	assert.False(t, toolIdentity(filepath.Join(globalDir, filename), "1.2.3", ""))
	require.NoError(t, os.Mkdir(filepath.Join(filepath.Dir(bin), ".stage-interrupted"), 0o755))
	warm, err := installTools(config, cache, []string{name}, false)
	require.NoError(t, err)
	assert.Equal(t, bin, warm)
	assert.Equal(t, int32(1), requests.Load())
	require.NoError(t, os.WriteFile(selected, []byte("incomplete executable"), 0o755))
	_, err = installTools(config, cache, []string{name}, false)
	assert.ErrorIs(t, err, errInstallRequired)
	assert.Equal(t, int32(1), requests.Load(), "repair requires consent even with a retained archive")
	repaired, err := installTools(config, cache, []string{name}, true)
	require.NoError(t, err)
	assert.Equal(t, bin, repaired)
	assert.True(t, toolIdentity(selected, "1.2.3", ""))
	assert.Equal(t, int32(1), requests.Load(), "repair uses the retained verified archive")
	// Matching installed executables can be frozen without a new installation.
	require.NoError(t, os.WriteFile(filepath.Join(globalDir, filename), data, 0o755))
	frozen, err := installTools(config, filepath.Join(root, "fresh-cache"), []string{name}, false)
	require.NoError(t, err)
	assert.True(t, toolIdentity(filepath.Join(frozen, filename), "1.2.3", ""))
	assert.Equal(t, int32(1), requests.Load())

	// Complete compiler packages need consent only when the verified package
	// cannot be reused. This fixture never invokes an archive extractor.
	digest := fmt.Sprintf("%x", sha256.Sum256(data))
	bundleManifest := fmt.Sprintf("TRICE_BUNDLE_FIXTURE_URL_%s=%s/package.tar.gz\nTRICE_BUNDLE_FIXTURE_SHA_%s=%s\nTRICE_BUNDLE_FIXTURE_EXECUTABLE=%s\nTRICE_BUNDLE_FIXTURE_VERSION_KEY=FIXTURE_VERSION\nFIXTURE_VERSION=1.2.3\n", platform, server.URL, platform, digest, name)
	bundleConfig := filepath.Join(root, "bundle.env")
	require.NoError(t, os.WriteFile(bundleConfig, []byte(bundleManifest), 0o644))
	bundleCache := filepath.Join(root, "bundles")
	_, err = installBundle(bundleConfig, bundleCache, "fixture", false)
	assert.ErrorIs(t, err, errInstallRequired)
	assert.NoDirExists(t, bundleCache, "cold probe must not extract or publish a package")
	bundleRoot := filepath.Join(bundleCache, "fixture", runtime.GOOS+"-"+runtime.GOARCH, digest[:16])
	bundleBin := filepath.Join(bundleRoot, "package", "bin")
	require.NoError(t, os.MkdirAll(bundleBin, 0o755))
	require.NoError(t, os.WriteFile(filepath.Join(bundleBin, filename), data, 0o755))
	require.NoError(t, os.WriteFile(filepath.Join(bundleRoot, ".ready"), []byte(digest), 0o644))
	warmBundle, err := installBundle(bundleConfig, bundleCache, "fixture", false)
	require.NoError(t, err)
	assert.Equal(t, bundleBin, warmBundle)
	require.NoError(t, os.WriteFile(filepath.Join(bundleRoot, ".ready"), []byte("incomplete"), 0o644))
	_, err = installBundle(bundleConfig, bundleCache, "fixture", false)
	assert.ErrorIs(t, err, errInstallRequired)
	assert.Equal(t, int32(1), requests.Load())
	_, err = installBundle(config, bundleCache, "unsupported", false)
	assert.ErrorContains(t, err, "no pinned unsupported package")
	assert.NotErrorIs(t, err, errInstallRequired, "invalid declarations are setup errors, not installation requests")
}

// TestToolSetupCommandDefersInstallation verifies the exit codes consumed by
// the shell: only missing installations request consent; setup errors do not.
func TestToolSetupCommandDefersInstallation(t *testing.T) {
	root := t.TempDir()
	binary := filepath.Join(root, "testcheck")
	if runtime.GOOS == "windows" {
		binary += ".exe"
	}
	output, err := exec.Command("go", "build", "-o", binary, ".").CombinedOutput()
	require.NoError(t, err, string(output))
	platform := strings.ToUpper(runtime.GOOS + "_" + runtime.GOARCH)
	config := filepath.Join(root, "tools.env")
	manifest := fmt.Sprintf("TRICE_TOOL_TRICE_CONSENT_FIXTURE_VERSION=1.2.3\nTRICE_TOOL_TRICE_CONSENT_FIXTURE_URL_%s=https://example.invalid/tool\nTRICE_TOOL_TRICE_CONSENT_FIXTURE_SHA_%s=%064d\nTRICE_BUNDLE_FIXTURE_URL_%s=https://example.invalid/bundle\nTRICE_BUNDLE_FIXTURE_SHA_%s=%064d\n", platform, platform, 0, platform, platform, 0)
	require.NoError(t, os.WriteFile(config, []byte(manifest), 0o644))
	for _, tc := range []struct {
		name, operation, tool, message string
		code                           int
	}{
		{"missing_tool", "tools", "trice-consent-fixture", "SETUP REQUIRED:", 3},
		{"missing_bundle", "bundle", "fixture", "SETUP REQUIRED:", 3},
		{"unknown_tool", "tools", "undeclared", "no pinned version", 2},
		{"unsupported_bundle", "bundle", "undeclared", "no pinned undeclared package", 2},
	} {
		t.Run(tc.name, func(t *testing.T) {
			command := exec.Command(binary, tc.operation, "--no-install", config, filepath.Join(root, tc.name), tc.tool)
			var stdout, stderr bytes.Buffer
			command.Stdout, command.Stderr = &stdout, &stderr
			err := command.Run()
			var exit *exec.ExitError
			require.ErrorAs(t, err, &exit, stderr.String())
			assert.Equal(t, tc.code, exit.ExitCode(), stderr.String())
			assert.Empty(t, stdout.String(), "failure must not be mistaken for a selected executable directory")
			assert.Contains(t, stderr.String(), tc.message)
		})
	}
}

// TestSharedNpmPinsAgree prevents the shell manifest and npm's direct and
// transitive lock from silently selecting different renderer/linter versions.
func TestSharedNpmPinsAgree(t *testing.T) {
	values, _, err := readToolConfig("../test-tools.env")
	require.NoError(t, err)
	data, err := os.ReadFile("package.json")
	require.NoError(t, err)
	var manifest struct{ Dependencies map[string]string }
	require.NoError(t, json.Unmarshal(data, &manifest))
	data, err = os.ReadFile("package-lock.json")
	require.NoError(t, err)
	var lock struct {
		Packages map[string]struct {
			Version      string
			Dependencies map[string]string
		}
	}
	require.NoError(t, json.Unmarshal(data, &lock))
	for name, key := range map[string]string{
		"markdownlint-cli": "TRICE_TOOL_MARKDOWNLINT_VERSION",
		"md-to-pdf":        "TRICE_TOOL_MD_TO_PDF_VERSION",
		"puppeteer":        "TRICE_TOOL_PUPPETEER_VERSION",
	} {
		assert.Equal(t, values[key], manifest.Dependencies[name], name)
		assert.Equal(t, values[key], lock.Packages[""].Dependencies[name], name)
		assert.Equal(t, values[key], lock.Packages["node_modules/"+name].Version, name)
	}
}
