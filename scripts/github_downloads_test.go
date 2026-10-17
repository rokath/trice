// SPDX-License-Identifier: MIT

package scripts_test

import (
	"encoding/json"
	"encoding/xml"
	"io"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"gopkg.in/yaml.v3"
)

// downloadRelease models public release metadata without user identities or API tokens.
func downloadRelease(id int, tag, published string, counts ...int) map[string]any {
	assets := make([]map[string]int, 0, len(counts))
	for _, count := range counts {
		assets = append(assets, map[string]int{"download_count": count})
	}
	return map[string]any{"id": id, "tag_name": tag, "published_at": published, "assets": assets}
}

// downloadJSON preserves the paginated shape emitted by gh api --paginate --slurp.
func downloadJSON(t *testing.T, pages ...[]map[string]any) string {
	t.Helper()
	data, err := json.Marshal(pages)
	require.NoError(t, err)
	return string(data)
}

// runDownloadReport executes the production generator without network access or Git writes.
func runDownloadReport(t *testing.T, input, directory, observedAt string) (string, error) {
	t.Helper()
	python, err := exec.LookPath("python3")
	if err != nil {
		python, err = exec.LookPath("python")
	}
	if err != nil {
		t.Skip("Python 3 is not installed; download charts use its standard library")
	}
	file := filepath.Join(t.TempDir(), "releases.json")
	require.NoError(t, os.WriteFile(file, []byte(input), 0600))
	command := exec.Command(python, "github_downloads.py", file, directory, "--observed-at", observedAt)
	output, err := command.CombinedOutput()
	return string(output), err
}

// downloadFile reads an expected artifact and fails at the caller on missing output.
func downloadFile(t *testing.T, directory, name string) string {
	t.Helper()
	data, err := os.ReadFile(filepath.Join(directory, name))
	require.NoError(t, err)
	return string(data)
}

// TestDownloadReleaseCharts covers pagination, publication state, totals and escaped labels.
func TestDownloadReleaseCharts(t *testing.T) {
	first := downloadRelease(1, "v1", "2026-01-01T00:00:00Z", 4, 7)
	second := downloadRelease(2, `v2<&"`, "2026-02-01T00:00:00Z", 5)
	second["prerelease"] = true
	draft := downloadRelease(3, "draft", "2026-03-01T00:00:00Z", 100)
	draft["draft"] = true
	input := downloadJSON(t,
		[]map[string]any{first, draft, downloadRelease(4, "unpublished", "", 200)},
		[]map[string]any{second, first, downloadRelease(5, "empty", "2025-01-01T00:00:00Z")},
	)
	directory := t.TempDir()
	output, err := runDownloadReport(t, input, directory, "2026-10-17T23:43:00Z")
	require.NoError(t, err, output)
	assert.Contains(t, output, "16 downloads, 1 observed days")

	var snapshot struct {
		ObservedAt string `json:"observed_at"`
		Releases   []struct {
			Tag       string
			Downloads int
		}
	}
	require.NoError(t, json.Unmarshal([]byte(downloadFile(t, directory, "snapshots/2026-10-17.json")), &snapshot))
	assert.Equal(t, "2026-10-17T23:43:00Z", snapshot.ObservedAt)
	require.Len(t, snapshot.Releases, 3)
	assert.Equal(t, `v2<&"`, snapshot.Releases[0].Tag)
	assert.Equal(t, 5, snapshot.Releases[0].Downloads)
	assert.Equal(t, 11, snapshot.Releases[1].Downloads)
	assert.Zero(t, snapshot.Releases[2].Downloads)
	chart := downloadFile(t, directory, "report/by-release.svg")
	assert.Contains(t, chart, "v2&lt;&amp;&quot;")
	assert.NotContains(t, chart, ">draft<")
	for _, name := range []string{"by-release.svg", "over-time.svg"} {
		decoder := xml.NewDecoder(strings.NewReader(downloadFile(t, directory, "report/"+name)))
		for {
			_, err := decoder.Token()
			if err == io.EOF {
				break
			}
			require.NoError(t, err, name)
		}
	}
	html := downloadFile(t, directory, "report/index.html")
	assert.Contains(t, html, "16 downloads</strong> across 3 published releases")
	assert.Contains(t, html, "2026-10-17T23:43:00Z")
	assert.NotContains(t, html, "<script")
}

// TestDownloadDailyHistory checks same-day replacement, immutable older days, gaps and decreases.
func TestDownloadDailyHistory(t *testing.T) {
	directory := t.TempDir()
	observe := func(day string, count int) {
		t.Helper()
		input := downloadJSON(t, []map[string]any{downloadRelease(1, "v1", "2026-01-01T00:00:00Z", count)})
		output, err := runDownloadReport(t, input, directory, day)
		require.NoError(t, err, output)
	}
	observe("2026-10-17T01:00:00Z", 10)
	observe("2026-10-17T23:43:00Z", 12)
	files, err := filepath.Glob(filepath.Join(directory, "snapshots", "*.json"))
	require.NoError(t, err)
	assert.Len(t, files, 1, "reruns replace the current day's observation")
	firstDay := downloadFile(t, directory, "snapshots/2026-10-17.json")
	observe("2026-10-19T23:43:00Z", 19)
	observe("2026-10-20T23:43:00Z", 15)
	assert.Equal(t, firstDay, downloadFile(t, directory, "snapshots/2026-10-17.json"))
	chart := downloadFile(t, directory, "report/over-time.svg")
	assert.Equal(t, 3, strings.Count(chart, "<circle "))
	assert.Equal(t, 1, strings.Count(chart, `<path class="series"`), "only consecutive observed days are joined")
	assert.NotContains(t, chart, `data-date="2026-10-18"`)
	assert.Contains(t, chart, "2026-10-20: 15 downloads")
	assert.Contains(t, downloadFile(t, directory, "report/index.html"), "15 downloads</strong>")
}

// TestDownloadInvalidInputPreservesHistory rejects bad API data before any good artifact changes.
func TestDownloadInvalidInputPreservesHistory(t *testing.T) {
	valid := downloadJSON(t, []map[string]any{downloadRelease(1, "v1", "2026-01-01T00:00:00Z", 9)})
	for _, tc := range []struct{ name, input, stamp string }{
		{"malformed_json", "[", "2026-10-17T23:43:00Z"},
		{"api_error_object", `{"message":"API rate limit exceeded"}`, "2026-10-17T23:43:00Z"},
		{"unpaginated_response", `[{}]`, "2026-10-17T23:43:00Z"},
		{"negative_count", downloadJSON(t, []map[string]any{downloadRelease(1, "v1", "2026-01-01T00:00:00Z", -1)}), "2026-10-17T23:43:00Z"},
		{"missing_count", `[[{"id":1,"tag_name":"v1","published_at":"2026-01-01T00:00:00Z","assets":[{}]}]]`, "2026-10-17T23:43:00Z"},
		{"fractional_count", `[[{"id":1,"tag_name":"v1","published_at":"2026-01-01T00:00:00Z","assets":[{"download_count":1.5}]}]]`, "2026-10-17T23:43:00Z"},
		{"missing_timezone", valid, "2026-10-17T23:43:00"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			directory := t.TempDir()
			output, err := runDownloadReport(t, valid, directory, "2026-10-17T01:00:00Z")
			require.NoError(t, err, output)
			before := downloadFile(t, directory, "snapshots/2026-10-17.json")
			report := downloadFile(t, directory, "report/index.html")
			output, err = runDownloadReport(t, tc.input, directory, tc.stamp)
			assert.Error(t, err, output)
			assert.Contains(t, output, "Download report failed for")
			assert.Equal(t, before, downloadFile(t, directory, "snapshots/2026-10-17.json"))
			assert.Equal(t, report, downloadFile(t, directory, "report/index.html"))
		})
	}
}

// TestDownloadEmptyRepository renders an honest zero without inventing past observations.
func TestDownloadEmptyRepository(t *testing.T) {
	directory := t.TempDir()
	output, err := runDownloadReport(t, "[[]]", directory, "2026-10-17T23:43:00Z")
	require.NoError(t, err, output)
	assert.Contains(t, output, "0 downloads, 1 observed days")
	assert.Contains(t, downloadFile(t, directory, "report/by-release.svg"), "No published releases.")
	assert.Equal(t, 1, strings.Count(downloadFile(t, directory, "report/over-time.svg"), "<circle "))
}

// TestDownloadPagesIntegration runs the actual publication step against local Git/tar fixtures.
func TestDownloadPagesIntegration(t *testing.T) {
	data, err := os.ReadFile("../.github/workflows/pages.yml")
	require.NoError(t, err)
	var workflow struct {
		Jobs map[string]struct {
			Steps []struct{ Name, Run string }
		}
	}
	require.NoError(t, yaml.Unmarshal(data, &workflow))
	var script string
	for _, step := range workflow.Jobs["build"].Steps {
		if step.Name == "Include repository statistics in the existing site" {
			script = step.Run
		}
	}
	require.NotEmpty(t, script)
	for _, tc := range []struct {
		name, missing string
		downloads     bool
	}{
		{name: "before_first_download_collection"},
		{name: "complete_report", downloads: true},
		{name: "missing_chart", downloads: true, missing: "over-time.svg"},
		{name: "missing_page", downloads: true, missing: "index.html"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t)
			writeFixture(t, root, "include-statistics.sh", script)
			writeFixture(t, root, "_site/docs/keep.html", "existing documentation")
			writeFixture(t, root, "rokath/trice/latest-report/report.html", "unchanged upstream report")
			writeFixture(t, root, "bin/git", `#!/bin/sh
case "$1" in
  ls-remote) echo 'fixture refs/heads/github-repo-stats' ;;
  fetch) exit 0 ;;
  cat-file) [ -d rokath/trice/downloads/report ] ;;
  archive) shift 2; tar -cf - "$@" ;;
  *) echo "unexpected Git operation: $*" >&2; exit 93 ;;
esac
`)
			if tc.downloads {
				for _, name := range []string{"index.html", "by-release.svg", "over-time.svg"} {
					if name != tc.missing {
						writeFixture(t, root, "rokath/trice/downloads/report/"+name, "download artifact "+name)
					}
				}
			}
			output, err := runFixture(t, root, "bash include-statistics.sh", nil)
			assert.Equal(t, "existing documentation", downloadFile(t, root, "_site/docs/keep.html"))
			if tc.missing != "" {
				assert.Error(t, err, output)
				assert.Contains(t, output, "Expected a nonempty _statistics/rokath/trice/downloads/report/"+tc.missing)
				return
			}
			require.NoError(t, err, output)
			assert.Equal(t, "unchanged upstream report", downloadFile(t, root, "_site/stats/report/report.html"))
			index := downloadFile(t, root, "_site/stats/index.html")
			if tc.downloads {
				assert.Contains(t, index, `href="downloads/"`)
				for _, name := range []string{"index.html", "by-release.svg", "over-time.svg"} {
					assert.Equal(t, "download artifact "+name, downloadFile(t, root, "_site/stats/downloads/"+name))
				}
			} else {
				assert.Contains(t, index, "after the first download collection")
				assert.NotContains(t, index, `href="downloads/"`)
			}
		})
	}
}
