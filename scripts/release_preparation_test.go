// SPDX-License-Identifier: MIT

package scripts_test

import (
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"gopkg.in/yaml.v3"
)

// TestReleaseAuditSelectsRequestedTag executes the real resolver/download step.
// Its GH double refuses an omitted/different tag, so latest fallback cannot pass.
func TestReleaseAuditSelectsRequestedTag(t *testing.T) {
	for _, tc := range []struct {
		name, requested, published string
		prerelease, draft, missing bool
	}{
		{name: "beta_selected_explicitly", requested: "v2.0.0-beta-1", published: "v2.0.0-beta-1", prerelease: true},
		{name: "stable_selected_explicitly", requested: "v2.0.0", published: "v2.0.0"},
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
    filename=$(printf '%s' "$pattern" | sed 's/\*/fixture/g')
    printf 'artifact\n' > "$directory/$filename"
    ;;
  *) exit 98 ;;
esac
`)
			outputFile := filepath.Join(root, "outputs")
			env := map[string]string{
				"RELEASE_TAG": tc.requested, "PUBLISHED_TAG": tc.published,
				"PRERELEASE": booleanText(tc.prerelease), "DRAFT": booleanText(tc.draft), "MISSING_RELEASE": booleanText(tc.missing),
				"GITHUB_OUTPUT": outputFile, "GITHUB_REPOSITORY": "fixture/trice",
			}
			out, err := runFixture(t, root, workflowShellStep(t, "release-audit.yml", "Resolve and download selected release assets"), env)
			if tc.name == "beta_selected_explicitly" || tc.name == "stable_selected_explicitly" {
				assert.NoError(t, err, out)
				outputs, readErr := os.ReadFile(outputFile)
				assert.NoError(t, readErr)
				assert.Equal(t, "tag="+tc.requested+"\nprerelease="+booleanText(tc.prerelease)+"\n", string(outputs))
				calls, readErr := os.ReadFile(filepath.Join(root, "gh.calls"))
				assert.NoError(t, readErr)
				assert.Contains(t, string(calls), "release view "+tc.requested+" --repo fixture/trice")
				assert.Equal(t, 9, strings.Count(string(calls), "release download "+tc.requested+" "))
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
