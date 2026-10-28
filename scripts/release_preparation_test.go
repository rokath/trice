// SPDX-License-Identifier: MIT

package scripts_test

import (
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"runtime"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"gopkg.in/yaml.v3"
)

// checkoutBuildFixture runs the real installer with metadata/tool doubles. Go
// records local package arguments without installing into any host directory.
func checkoutBuildFixture(t *testing.T) string {
	t.Helper()
	root := scriptFixture(t, "scripts/buildTriceTool.sh")
	writeFixture(t, root, "bin/git", `#!/bin/sh
case "$1" in
  remote) echo 'fixture/trice' ;;
  rev-parse) echo 'fixture' ;;
  describe) exit 1 ;;
  diff) exit 0 ;;
  *) exit 94 ;;
esac
`)
	writeFixture(t, root, "bin/date", "#!/bin/sh\necho 20261014-120000\n")
	writeFixture(t, root, "bin/go", `#!/bin/sh
set -eu
case "$1" in
  env)
    case "$2" in
      GOEXE) printf '%s\n' "$FIXTURE_EXE" ;;
      GOHOSTOS) printf '%s\n' "$FIXTURE_HOST" ;;
      GOBIN) printf '%s\n' "${FIXTURE_GOBIN:-}" ;;
      GOPATH) printf '%s\n' "$FIXTURE_GOPATH" ;;
      *) exit 95 ;;
    esac
    ;;
  install) printf '%s\n' "$@" > go-install.args ;;
  *) exit 96 ;;
esac
`)
	return root
}

// TestCheckoutBuildInstallTargets covers native path-list rules independently
// of the test host. Phantom paths are inspected but never created or installed.
func TestCheckoutBuildInstallTargets(t *testing.T) {
	for _, tc := range []struct {
		name, host, gobin, envGobin, gopath, destination, suffix string
	}{
		{"linux_first_gopath", "linux", "", "", "/r14-fixture/first path:/r14-fixture/second", "/r14-fixture/first path/bin", ""},
		{"macos_first_gopath", "darwin", "", "", "/r14-fixture/first:/r14-fixture/second", "/r14-fixture/first/bin", ""},
		{"windows_single_drive", "windows", "", "", "C:/r14-fixture/Go workspace", "C:/r14-fixture/Go workspace/bin", ".exe"},
		{"windows_drive_list", "windows", "", "", "C:/r14-fixture/first path;D:/r14-fixture/second", "C:/r14-fixture/first path/bin", ".exe"},
		{"windows_backslashes", "windows", "", "", `C:\r14-fixture\first path;D:\r14-fixture\second`, `C:\r14-fixture\first path/bin`, ".exe"},
		{"windows_unc_list", "windows", "", "", `\\r14-fixture\share\first;\\r14-fixture\share\second`, `\\r14-fixture\share\first/bin`, ".exe"},
		{"configured_gobin", "linux", "", "/r14-fixture/configured tools", "/r14-fixture/unused", "/r14-fixture/configured tools", ""},
		{"environment_gobin_takes_precedence", "windows", "C:/r14-fixture/environment tools", "C:/r14-fixture/unused", "C:/r14-fixture/unused", "C:/r14-fixture/environment tools", ".exe"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := checkoutBuildFixture(t)
			out, err := runFixture(t, root, "bash scripts/buildTriceTool.sh", map[string]string{
				"GOBIN": tc.gobin, "GOROOT": "", "FIXTURE_GOBIN": tc.envGobin,
				"FIXTURE_GOPATH": tc.gopath, "FIXTURE_HOST": tc.host, "FIXTURE_EXE": tc.suffix,
			})
			assert.NoError(t, err, out)
			for _, tool := range []string{"trice", "tlog"} {
				assert.Regexp(t, `(?m)^Installed `+tool+` target: +`+regexp.QuoteMeta(tc.destination+"/"+tool+tc.suffix)+`$`, out)
			}
			data, readErr := os.ReadFile(filepath.Join(root, "go-install.args"))
			assert.NoError(t, readErr)
			assert.True(t, strings.HasSuffix(string(data), "\n./cmd/trice\n./cmd/tlog\n"), "both tools must install from the current checkout: %s", data)
		})
	}
}

// TestCheckoutBuildProtectsBothTools proves that GOPATH resolution protects an
// existing trice or tlog before either build starts, including a Windows drive.
func TestCheckoutBuildProtectsBothTools(t *testing.T) {
	for _, tool := range []string{"trice", "tlog"} {
		t.Run(tool, func(t *testing.T) {
			root := checkoutBuildFixture(t)
			suffix := ""
			if runtime.GOOS == "windows" {
				suffix = ".exe"
			}
			name := "Go workspace/bin/" + tool + suffix
			writeFixture(t, root, name, "previous installation\n")
			out, err := runFixture(t, root, "bash scripts/buildTriceTool.sh", map[string]string{
				"GOBIN": "", "GOROOT": "", "FIXTURE_GOBIN": "",
				"FIXTURE_GOPATH": filepath.ToSlash(filepath.Join(root, "Go workspace")),
				"FIXTURE_HOST":   runtime.GOOS, "FIXTURE_EXE": suffix,
			})
			assert.Error(t, err, out)
			assert.Contains(t, out, "Refusing to overwrite existing target in non-interactive mode:")
			assert.Contains(t, out, "Use --force to overwrite")
			assert.NoFileExists(t, filepath.Join(root, "go-install.args"), "neither tool may be installed before overwrite approval")
			data, readErr := os.ReadFile(filepath.Join(root, name))
			assert.NoError(t, readErr)
			assert.Equal(t, "previous installation\n", string(data))
		})
	}
}

// TestReleaseAuditSelectsRequestedTag executes the real resolver/download step.
// Its GH double refuses an omitted/different tag, so latest fallback cannot pass.
func TestReleaseAuditSelectsRequestedTag(t *testing.T) {
	for _, tc := range []struct {
		name, requested, published, missingAsset    string
		prerelease, draft, missing, extraAssets, ok bool
	}{
		{name: "beta_selected_explicitly", requested: "v2.0.0-beta-1", published: "v2.0.0-beta-1", prerelease: true, ok: true},
		{name: "stable_selected_explicitly", requested: "v2.0.0", published: "v2.0.0", ok: true},
		{name: "multiple_assets_with_spaces", requested: "v2.0.0", published: "v2.0.0", extraAssets: true, ok: true},
		{name: "missing_downloaded_sources", requested: "v2.0.0", published: "v2.0.0", missingAsset: "trice_target_sources_*.zip"},
		{name: "missing_downloaded_manual", requested: "v2.0.0", published: "v2.0.0", missingAsset: "TriceUserManual.pdf"},
		{name: "missing_tag_stops_before_api"},
		{name: "invalid_tag_stops_before_api", requested: "bad tag"},
		{name: "missing_release_stops_before_downloads", requested: "v2.0.0-beta-1", missing: true},
		{name: "draft_stops_before_downloads", requested: "v2.0.0-beta-1", published: "v2.0.0-beta-1", prerelease: true, draft: true},
		{name: "wrong_release_stops_before_downloads", requested: "v2.0.0-beta-1", published: "v1.3.0"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			if _, err := exec.LookPath("jq"); err != nil {
				t.Skip("jq is required to execute the release audit resolver")
			}
			root := t.TempDir()
			writeFixture(t, root, "bin/gh", `#!/bin/sh
set -eu
printf '%s\n' "$*" >> gh.calls
[ "$3" = "$RELEASE_TAG" ] || { echo unexpected-release >&2; exit 97; }
case "$1 $2" in
  'release view')
    [ "$MISSING_RELEASE" = false ] || { echo unavailable >&2; exit 23; }
    printf '{"tagName":"%s","isPrerelease":%s,"isDraft":%s}\n' "$PUBLISHED_TAG" "$PRERELEASE" "$DRAFT"
    ;;
  'release download')
    shift 3
    pattern= directory=
    while [ "$#" -gt 0 ]; do
      case "$1" in
        --pattern) pattern=$2; shift 2 ;;
        --dir) directory=$2; shift 2 ;;
        *) shift ;;
      esac
    done
    mkdir -p "$directory"
    # Simulate an incomplete successful download so validation must catch it.
    [ "$pattern" != "$MISSING_ASSET" ] || exit 0
    filename=$(printf '%s' "$pattern" | sed 's/\*/fixture/g')
    printf 'artifact\n' > "$directory/$filename"
    if [ "$EXTRA_ASSETS" = true ] && [ "$pattern" = 'trice_target_sources_*.zip' ]; then
      printf 'artifact\n' > "$directory/trice_target_sources_fixture extra.zip"
    fi
    ;;
  *) exit 98 ;;
esac
`)
			outputFile := filepath.Join(root, "outputs")
			env := map[string]string{
				"RELEASE_TAG": tc.requested, "PUBLISHED_TAG": tc.published,
				"PRERELEASE": booleanText(tc.prerelease), "DRAFT": booleanText(tc.draft), "MISSING_RELEASE": booleanText(tc.missing),
				"MISSING_ASSET": tc.missingAsset, "EXTRA_ASSETS": booleanText(tc.extraAssets),
				"GITHUB_OUTPUT": outputFile, "GITHUB_REPOSITORY": "fixture/trice",
			}
			// Keep the macOS Bash 3.2 regression observable on newer Bash hosts too.
			command := "enable -n mapfile 2>/dev/null || true\n" + workflowShellStep(t, "release-audit.yml", "Resolve and download selected release assets")
			out, err := runFixture(t, root, command, env)
			if tc.ok {
				assert.NoError(t, err, out)
				outputs, readErr := os.ReadFile(outputFile)
				assert.NoError(t, readErr)
				assert.Equal(t, "tag="+tc.requested+"\nprerelease="+booleanText(tc.prerelease)+"\n", string(outputs))
				calls, readErr := os.ReadFile(filepath.Join(root, "gh.calls"))
				assert.NoError(t, readErr)
				assert.Contains(t, string(calls), "release view "+tc.requested+" --repo fixture/trice")
				assert.Equal(t, 9, strings.Count(string(calls), "release download "+tc.requested+" "))
				assert.Contains(t, out, "OK downloaded: trice_target_sources_*.zip")
				if tc.extraAssets {
					assert.Contains(t, out, "  release-assets/trice_target_sources_fixture.zip\n")
					assert.Contains(t, out, "  release-assets/trice_target_sources_fixture extra.zip\n")
				}
			} else if tc.missingAsset != "" {
				assert.Error(t, err, out)
				assert.Contains(t, out, "::error::no downloaded release asset matches: "+tc.missingAsset)
				assert.Contains(t, out, "release-assets/trice_tool_linux_amd64.tar.gz", "list nearby downloaded assets on failure")
			} else {
				assert.Error(t, err, out)
				assert.Contains(t, out, "::error::")
				assert.NoFileExists(t, outputFile)
				assert.NoDirExists(t, filepath.Join(root, "release-assets"))
				if tc.requested == "" || tc.requested == "bad tag" {
					assert.NoFileExists(t, filepath.Join(root, "gh.calls"))
				}
			}
		})
	}
}

// releasePDFSetupFixture isolates the real setup and snapshot scripts. Tool
// doubles refuse rendering and stop the release after checking a captured hook.
func releasePDFSetupFixture(t *testing.T, installed bool) string {
	t.Helper()
	root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_320_generate_trice_user_manual_pdf.sh", "scripts/_610_test_goreleaser_snapshot.sh")
	writeFixture(t, root, "docs/TriceUserManual.md", "# User Manual\n")
	writeFixture(t, root, "docs/rm/README.md", "# Reference Manual\n")
	writeFixture(t, root, "docs/ref/fixture.txt", "reference\n")
	writeFixture(t, root, "docs/TriceUserManual.pdf", "existing user PDF\n")
	writeFixture(t, root, "docs/TriceReferenceManual.pdf", "existing reference PDF\n")
	writeFixture(t, root, "fixture-stitchmd", "#!/bin/sh\n[ \"$1\" = -version ] || { touch render-called; exit 96; }\necho 'stitchmd 0.9.0'\n")
	if installed {
		writeFixture(t, root, "bin/stitchmd", "#!/bin/sh\n[ \"$1\" = -version ] || { touch render-called; exit 96; }\necho 'stitchmd 0.9.0'\n")
	} else {
		// Shadow any host installation so a version mismatch always needs consent.
		writeFixture(t, root, "bin/stitchmd", "#!/bin/sh\necho 'stitchmd 0.8.0'\n")
	}
	writeFixture(t, root, "bin/npx", "#!/bin/sh\n[ \"$1\" = --version ] && { echo fixture; exit 0; }\ntouch render-called\nexit 96\n")
	// PDF setup verifies Node before preparing captured release hooks. Model
	// the selected runtime so this fixture never attempts a real download.
	writeFixture(t, root, "bin/node", "#!/bin/sh\necho v26.12.1\n")
	for _, tool := range []string{"tar", "unzip"} {
		writeFixture(t, root, "bin/"+tool, "#!/bin/sh\necho fixture\n")
	}
	writeFixture(t, root, "bin/go", `#!/bin/sh
set -eu
[ "$1" = install ] || exit 97
printf '%s\n' "$2" > go-installed
[ "${FAIL_INSTALL:-0}" = 0 ] || exit 23
cp fixture-stitchmd "$GOBIN/stitchmd"
chmod +x "$GOBIN/stitchmd"
`)
	writeFixture(t, root, "bin/goreleaser", `#!/bin/sh
set -eu
if [ "$1" = --version ]; then echo 'GitVersion: 2.18.3'; exit 0; fi
[ "$*" = 'release --snapshot --clean' ] || exit 98
touch release-called
# A captured hook has no stdin; the caller must already have prepared its tools.
bash scripts/_320_generate_trice_user_manual_pdf.sh setup </dev/null
echo 'captured hook found prepared PDF tools'
exit 23
`)
	return root
}

// TestReleasePDFSetupPreservesOutputs verifies that preflight never assembles
// or renders either manual and rejects invalid modes before changing artifacts.
func TestReleasePDFSetupPreservesOutputs(t *testing.T) {
	for _, mode := range []string{"setup", "unexpected"} {
		t.Run(mode, func(t *testing.T) {
			root := releasePDFSetupFixture(t, true)
			out, err := runFixture(t, root, "bash scripts/_320_generate_trice_user_manual_pdf.sh "+mode, nil)
			if mode == "setup" {
				assert.NoError(t, err, out)
				assert.Contains(t, out, "Manual PDF prerequisites ready: stitchmd 0.9.0 and npx.")
			} else {
				assert.Error(t, err, out)
				assert.Contains(t, out, "Usage:")
			}
			for name, want := range map[string]string{"TriceUserManual": "existing user PDF\n", "TriceReferenceManual": "existing reference PDF\n"} {
				data, readErr := os.ReadFile(filepath.Join(root, "docs", name+".pdf"))
				assert.NoError(t, readErr)
				assert.Equal(t, want, string(data))
			}
			assert.NoFileExists(t, filepath.Join(root, "render-called"))
			assert.NoFileExists(t, filepath.Join(root, "go-installed"))
		})
	}
}

// TestReleaseSnapshotPreparesToolsBeforeHooks covers existing tools, consent,
// EOF, CI refusal and install failure before any release hooks can mutate sources.
func TestReleaseSnapshotPreparesToolsBeforeHooks(t *testing.T) {
	for _, tc := range []struct {
		name, answer, failInstall string
		installed, ci, proceeds   bool
	}{
		{name: "already_installed", installed: true, proceeds: true},
		{name: "approved_local_install", answer: "yes\n", proceeds: true},
		{name: "declined_install", answer: "no\n"},
		{name: "no_input"},
		{name: "failed_install", answer: "yes\n", failInstall: "1"},
		{name: "ci_does_not_install", answer: "yes\n", ci: true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := releasePDFSetupFixture(t, tc.installed)
			out, err := runFixture(t, root, "printf '%s' \"$TOOL_REPLY\" | bash scripts/_610_test_goreleaser_snapshot.sh", map[string]string{
				"TOOL_REPLY": tc.answer, "CI": booleanText(tc.ci), "FAIL_INSTALL": tc.failInstall,
			})
			assert.Error(t, err, "the release double deliberately stops before building artifacts: %s", out)
			if tc.proceeds {
				assert.FileExists(t, filepath.Join(root, "release-called"))
				assert.Contains(t, out, "captured hook found prepared PDF tools")
			} else {
				assert.NoFileExists(t, filepath.Join(root, "release-called"))
				assert.Contains(t, out, "FAIL: manual PDF prerequisites unavailable; snapshot build was not started")
			}
			if tc.answer == "yes\n" && !tc.ci {
				data, readErr := os.ReadFile(filepath.Join(root, "go-installed"))
				assert.NoError(t, readErr)
				assert.Equal(t, "go.abhg.dev/stitchmd@v0.9.0\n", string(data))
			} else {
				assert.NoFileExists(t, filepath.Join(root, "go-installed"))
			}
			assert.NoFileExists(t, filepath.Join(root, "render-called"))
		})
	}
}

// TestReleaseSnapshotKeepsSetupPromptVisible exercises the interactive runner
// with a child awaiting consent. Spinner carriage returns must not erase it.
func TestReleaseSnapshotKeepsSetupPromptVisible(t *testing.T) {
	root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_110_test_runner.sh")
	writeFixture(t, root, "scripts/_610_test_goreleaser_snapshot.sh", "#!/bin/sh\nprintf 'Install fixture? [y/N] '\nsleep 1\nIFS= read -r answer\nprintf 'Received: %s\\n' \"$answer\"\n")
	out, err := runFixture(t, root, `printf 'yes\n' | {
source scripts/_110_test_runner.sh
SUMMARY_LOG="$LOG_DIR/testAll_summary.log"
TEST_ALL_INTERACTIVE=1
TEST_ALL_STEP_NUMBER=1 TEST_ALL_STEP_COUNT=1 TEST_ALL_STEP_END_TENTHS=1000
run_step _610_test_goreleaser_snapshot.sh
}`, nil)
	assert.NoError(t, err, out)
	assert.Contains(t, out, "Install fixture? [y/N] Received: yes\n")
	assert.NotContains(t, out, "\r", "no spinner frame may overwrite the prompt")
	assert.Contains(t, out, "_610_test_goreleaser_snapshot.sh: PASS")
}

// TestSelectedReleaseNotes keeps beta-1 separate from beta-10 and rejects missing
// or empty descriptions before publishing a Git-generated list in their place.
func TestSelectedReleaseNotes(t *testing.T) {
	for _, tc := range []struct {
		name, tag, content string
		valid              bool
	}{
		{"exact_beta_and_tagged_doc_links", "v2.0.0-beta-1", "\ufeff# Changelog\n\n## v2.0.0-beta-10\n\nOther beta\n\n## v2.0.0-beta-1 (unreleased)\n\nHuman notes [Guide](docs/TriceUserManual.md#start).\n\n## v1.3.0\n\nOld notes\n", true},
		{"stable_description", "v2.0.0", "# Changelog\n\n## v2.0.0\n\nStable notes\n", true},
		{"empty_tag", "", "# Changelog\n", false},
		{"missing_version", "v2.0.0-beta-1", "# Changelog\n\n## v2.0.0-beta-10\n\nOther beta\n", false},
		{"empty_description", "v2.0.0-beta-1", "# Changelog\n\n## v2.0.0-beta-1\n\n## v1.3.0\n\nOld notes\n", false},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := t.TempDir()
			writeFixture(t, root, "CHANGELOG.md", tc.content)
			out, err := runFixture(t, root, workflowShellStep(t, "goreleaser.yml", "Prepare selected release notes"), map[string]string{
				"RELEASE_TAG": tc.tag, "GITHUB_REPOSITORY": "fixture/trice", "GITHUB_SERVER_URL": "https://github.example",
			})
			if tc.valid {
				assert.NoError(t, err, out)
				notes, readErr := os.ReadFile(filepath.Join(root, "temp", "release-notes.md"))
				assert.NoError(t, readErr)
				assert.True(t, strings.HasPrefix(string(notes), "## "+tc.tag+"\n\n"))
				assert.NotContains(t, string(notes), "unreleased")
				assert.NotContains(t, string(notes), "Old notes")
				assert.NotContains(t, string(notes), "Other beta")
				if tc.tag == "v2.0.0-beta-1" {
					assert.Contains(t, string(notes), "https://github.example/fixture/trice/blob/v2.0.0-beta-1/docs/TriceUserManual.md#start")
				}
			} else {
				assert.Error(t, err, out)
				assert.Contains(t, out, "::error::")
				assert.NoFileExists(t, filepath.Join(root, "temp", "release-notes.md"))
			}
		})
	}
}

// booleanText keeps API fixtures valid JSON rather than shell-specific booleans.
func booleanText(value bool) string {
	if value {
		return "true"
	}
	return "false"
}

// TestReleaseTagCheckout rejects a missing ref or a different checked-out commit.
// Only the Git identity is doubled; the production guard runs unchanged.
func TestReleaseTagCheckout(t *testing.T) {
	for _, tc := range []struct {
		name, tag, head string
		missing         bool
	}{
		{"matching_beta_commit", "v2.0.0-beta-1", "candidate", false},
		{"matching_stable_commit", "v2.0.0", "candidate", false},
		{"wrong_commit", "v2.0.0-beta-1", "older", false},
		{"missing_tag", "v2.0.0-beta-1", "candidate", true},
		{"empty_tag", "", "candidate", false},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := t.TempDir()
			writeFixture(t, root, "bin/git", `#!/bin/sh
set -eu
case "$1" in
  check-ref-format) [ "$2" != refs/tags/ ] ;;
  rev-parse)
    if [ "$2" = HEAD ]; then printf '%s\n' "$CHECKOUT_COMMIT"; exit 0; fi
    [ "$3" = "refs/tags/$RELEASE_TAG^{commit}" ] || exit 97
    [ "$MISSING_TAG" = false ] || exit 23
    echo candidate
    ;;
  *) exit 98 ;;
esac
`)
			out, err := runFixture(t, root, workflowShellStep(t, "goreleaser.yml", "Verify requested release tag"), map[string]string{
				"RELEASE_TAG": tc.tag, "CHECKOUT_COMMIT": tc.head, "MISSING_TAG": booleanText(tc.missing),
			})
			if tc.tag != "" && tc.head == "candidate" && !tc.missing {
				assert.NoError(t, err, out)
				assert.Contains(t, out, "Building release tag "+tc.tag+" at candidate")
			} else {
				assert.Error(t, err, out)
				assert.Contains(t, out, "::error::")
				assert.NotContains(t, out, "Building release tag")
			}
		})
	}
}

// TestReleaseWorkflowBindings checks platform-side invariants that shell tests
// cannot execute: required inputs, exact checkout/version binding and beta gating.
func TestReleaseWorkflowBindings(t *testing.T) {
	for _, name := range []string{"goreleaser.yml", "release-audit.yml"} {
		data, err := os.ReadFile(filepath.Join("..", ".github", "workflows", name))
		assert.NoError(t, err)
		var workflow struct {
			On map[string]struct {
				Inputs map[string]struct {
					Required bool
					Type     string
				}
			}
			Jobs map[string]struct {
				If    string
				Steps []struct {
					Uses string
					With map[string]string
					Env  map[string]string
				}
			}
		}
		assert.NoError(t, yaml.Unmarshal(data, &workflow))
		input := workflow.On["workflow_dispatch"].Inputs["tag"]
		assert.True(t, input.Required, name)
		assert.Equal(t, "string", input.Type, name)
		if name == "goreleaser.yml" {
			for _, step := range workflow.Jobs["goreleaser"].Steps {
				if strings.HasPrefix(step.Uses, "actions/checkout@") {
					assert.Equal(t, "refs/tags/${{ inputs.tag }}", step.With["ref"])
				}
				if strings.HasPrefix(step.Uses, "goreleaser/goreleaser-action@") {
					assert.Equal(t, "${{ inputs.tag }}", step.Env["GORELEASER_CURRENT_TAG"])
				}
			}
		} else {
			assert.Equal(t, "needs.download-release-assets.outputs.prerelease == 'false'", workflow.Jobs["audit-homebrew"].If)
			for _, job := range []string{"audit-linux", "audit-macos", "audit-windows", "audit-target-sources"} {
				for _, step := range workflow.Jobs[job].Steps {
					if strings.HasPrefix(step.Uses, "actions/checkout@") {
						assert.Equal(t, "refs/tags/${{ needs.download-release-assets.outputs.tag }}", step.With["ref"], job)
					}
				}
			}
		}
	}
}

// TestLibraryWorkflowRetainsPreparationLogs keeps failed setup diagnosable even
// before compiler artifacts exist or when their optional upload is disabled.
func TestLibraryWorkflowRetainsPreparationLogs(t *testing.T) {
	data, err := os.ReadFile(filepath.Join("..", ".github", "workflows", "trice_lib_reusable.yml"))
	if !assert.NoError(t, err) {
		t.FailNow()
	}
	var workflow struct {
		Jobs map[string]struct {
			Steps []struct {
				Name, If, Uses string
				With           map[string]string
			}
		}
	}
	if !assert.NoError(t, yaml.Unmarshal(data, &workflow)) {
		t.FailNow()
	}
	found := false
	for _, step := range workflow.Jobs["build"].Steps {
		if step.Name == "Upload test logs" {
			found = true
			assert.Equal(t, "${{ always() }}", step.If)
			assert.Equal(t, "actions/upload-artifact@v7", step.Uses)
			assert.Equal(t, "temp/log/", step.With["path"])
			assert.Equal(t, "trice-lib-test-logs", step.With["name"])
		}
	}
	assert.True(t, found, "the reusable Library job must retain preparation logs")
}

// TestLibraryWorkflowTriggers protects the pre-merge matrix and existing check
// identity. Ordinary branch pushes must not create a second expensive run.
func TestLibraryWorkflowTriggers(t *testing.T) {
	for _, filename := range []string{"trice_lib_ci.yml", "trice_lib_ci_full.yml"} {
		t.Run(filename, func(t *testing.T) {
			data, err := os.ReadFile(filepath.Join("..", ".github", "workflows", filename))
			if !assert.NoError(t, err) {
				t.FailNow()
			}
			// Typed YAML fields keep GitHub event bindings distinct from strings
			// embedded in comments or shell examples elsewhere in the workflow.
			var workflow struct {
				Name string
				On   map[string]yaml.Node
				Jobs map[string]struct {
					Name, If, Needs, Uses string
					With                  map[string]interface{}
				}
			}
			if !assert.NoError(t, yaml.Unmarshal(data, &workflow)) {
				t.FailNow()
			}
			assert.NotContains(t, workflow.On, "push", "a wip push without a PR must stay cheap")
			assert.Contains(t, workflow.On, "schedule")
			assert.Contains(t, workflow.On, "workflow_dispatch")
			assert.Equal(t, "github.event_name == 'schedule'", workflow.Jobs["guard_activity"].If)
			build := workflow.Jobs["heavy_ci"]
			assert.Equal(t, "guard_activity", build.Needs)
			assert.Equal(t, "${{ !cancelled() && (github.event_name != 'schedule' || needs.guard_activity.outputs.should_run == 'true') }}", build.If,
				"PR/manual checks must run after a skipped guard, while cancellation still stops them")
			assert.Equal(t, "./.github/workflows/trice_lib_reusable.yml", build.Uses)
			for _, compiler := range []string{"toolchain_gcc_trice_on", "toolchain_gcc_trice_off", "toolchain_clang"} {
				assert.Equal(t, true, build.With[compiler], compiler)
			}
			if filename == "trice_lib_ci.yml" {
				assert.Equal(t, "TRICE Library CI (Nightly Quick)", workflow.Name,
					"keep existing branch-protection check identities")
				assert.Equal(t, "Heavy CI (quick)", build.Name)
				// Decode only the PR mapping; schedule is a YAML sequence and
				// workflow_dispatch is an empty mapping in the same event list.
				var pullRequest struct {
					Branches []string
					Types    []string
				}
				prEvent, exists := workflow.On["pull_request"]
				assert.True(t, exists)
				assert.NoError(t, prEvent.Decode(&pullRequest))
				assert.Equal(t, []string{"main"}, pullRequest.Branches)
				assert.Empty(t, pullRequest.Types,
					"default PR activities include synchronization after a corrective push")
				assert.Equal(t, "${{ github.event_name == 'pull_request' && 'full' || 'quick' }}", build.With["configs_mode"])
				assert.Equal(t, 0, build.With["configs_start"])
				assert.Equal(t, 10, build.With["configs_end"])
			} else {
				assert.NotContains(t, workflow.On, "pull_request", "one full library matrix per PR update")
				assert.Equal(t, "full", build.With["configs_mode"])
			}
		})
	}
}

// TestLibraryNightlyActivityGuard executes each unchanged schedule decision
// against controlled Git history. Idle nights skip; Git failures fail visibly.
func TestLibraryNightlyActivityGuard(t *testing.T) {
	for _, filename := range []string{"trice_lib_ci.yml", "trice_lib_ci_full.yml"} {
		for _, tc := range []struct {
			name, history, want string
			gitFailure          bool
		}{
			{name: "idle_main_skips", want: "should_run=false\n"},
			{name: "recent_main_runs", history: "abc123 merged change\n", want: "should_run=true\n"},
			{name: "git_error_is_not_a_skip", gitFailure: true},
		} {
			t.Run(filename+"/"+tc.name, func(t *testing.T) {
				root := t.TempDir()
				writeFixture(t, root, "bin/git", `#!/bin/sh
set -eu
[ "$*" = 'log --since=24 hours ago --pretty=oneline origin/main' ] || exit 98
[ "$TEST_GIT_FAILURE" = false ] || { echo 'fixture Git history is unavailable' >&2; exit 23; }
printf '%s' "$TEST_GIT_HISTORY"
`)
				outputFile := filepath.Join(root, "github-output")
				out, err := runFixture(t, root, workflowShellStep(t, filename, "Decide whether to run heavy CI"), map[string]string{
					"GITHUB_OUTPUT": outputFile, "TEST_GIT_HISTORY": tc.history,
					"TEST_GIT_FAILURE": booleanText(tc.gitFailure),
				})
				if tc.gitFailure {
					assert.Error(t, err, out)
					assert.Contains(t, out, "fixture Git history is unavailable")
					assert.NoFileExists(t, outputFile)
				} else {
					assert.NoError(t, err, out)
					decision, err := os.ReadFile(outputFile)
					assert.NoError(t, err)
					assert.Equal(t, tc.want, string(decision))
				}
			})
		}
	}
}

// TestReleaseFeatureOutputValidation tests malformed records, lost CE/stamps,
// stray stdout and changed/truncated notices independently of the real packages.
func TestReleaseFeatureOutputValidation(t *testing.T) {
	node, err := exec.LookPath("node")
	if err != nil {
		t.Skip("Node is required for the existing release environment")
	}
	helper, err := filepath.Abs("_verify_release_features.cjs")
	assert.NoError(t, err)
	root := t.TempDir()
	command := exec.Command(node, "-e", `
const assert = require('node:assert/strict');
const fs = require('node:fs');
const helper = require(process.argv[1]);
// The fixture represents all records emitted by the shipped PC tour.
const events = [
  {message:'PC feature tour starts\n',tag:'INFO'},
  {message:'Device pump A\n',fields:{device:'pump A'}},
  {message:'Phase 7\n',ts32:'0:00:00,100',fields:{phase:7}},
  {message:'Supply 3300 mV, cycle=7\n',fields:{voltage_mv:3300,cycle:7}},
  {message:'Retry 2\n',tag:'WARNING'},
  {message:'Humidity 55 percent\n',ts16:'7',fields:{humidity_pct:55}},
  {message:'Supply 3250 mV, cycle=11\n',ts32Delta:'0:00:00,025',fields:{voltage_mv:3250,cycle:11}},
  {message:'Raw register=0000002a\n'},
  {message:'A message without a tag\n',tag:'untagged'},
  {message:'41 00 ff ',tag:'RECEIVE'}
];
const encoded = data => data.map(event => JSON.stringify(event)).join('\n')+'\n';
helper.verifyEvents(encoded(events),'fixture');
const text = events.map(event => event.message.replace(/\n/g,'')).join('\n')+'\n';
helper.verifyReadable(text,'text','fixture');
assert.throws(() => helper.verifyReadable(text.replace('Device pump A','Device other'),'text','fixture'));
const kvLines = events.map(event => 'tag=INFO level=INFO message='+JSON.stringify(event.message));
kvLines[3] += ' field.voltage_mv=3300 field.cycle=7';
kvLines[6] += ' field.voltage_mv=3250 field.cycle=11';
const kv = kvLines.join('\n')+'\n';
helper.verifyReadable(kv,'kv','fixture');
assert.throws(() => helper.verifyReadable(kv.replace('field.cycle=11','field.other=11'),'kv','fixture'));
const diagnostic = [...kvLines]; diagnostic[0]='startup diagnostic';
assert.throws(() => helper.verifyReadable(diagnostic.join('\n'),'kv','fixture'));
for (const output of ['banner\n'+encoded(events), encoded(events)+'diagnostic\n', encoded(events.slice(1)), encoded([...events,events[0]]), '{}\n']) {
  assert.throws(() => helper.verifyEvents(output,'fixture'));
}
for (const mutate of [data => delete data[3].fields.cycle, data => data[6].fields.cycle=7, data => data[6].ts32Delta='0:00:00,000', data => data[1].fields.device='other', data => data[5].ts16='0']) {
  const changed = JSON.parse(JSON.stringify(events));
  mutate(changed);
  assert.throws(() => helper.verifyEvents(encoded(changed),'fixture'));
}
fs.writeFileSync('expected-license','full copyright and permission notice\n');
fs.writeFileSync('license','full copyright and permission notice\n');
helper.verifyLicense('license','expected-license');
fs.writeFileSync('license','full copyright\n');
assert.throws(() => helper.verifyLicense('license','expected-license'), /license content differs/);
assert.throws(() => helper.verifyLicense('absent','expected-license'), /missing or empty/);
assert.throws(() => helper.verifyFeatures('','archive.zip'), /missing unpacked/);
assert.throws(() => helper.verifyFeatures('tools',''), /missing target-source archive/);
const selected = helper.toolEnvironment('shipped-tools', {Path:'developer-tools',PATH:'other-tools',KEEP:'value'});
assert.equal(Object.keys(selected).filter(key => key.toLowerCase()==='path').length,1);
assert.equal(selected.Path,'shipped-tools'+require('node:path').delimiter+'developer-tools');
assert.equal(selected.KEEP,'value');
console.log('PASS: application records, CE/stamps, stdout separation and complete licenses checked');
`, helper)
	command.Dir = root
	out, err := command.CombinedOutput()
	assert.NoError(t, err, string(out))
	assert.Contains(t, string(out), "PASS:")
}
