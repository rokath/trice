// SPDX-License-Identifier: MIT

package scripts_test

import (
	"os"
	"os/exec"
	"path/filepath"
	"strconv"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestMissingPrerequisiteKeepsMenu blocks continuation before any installer,
// while keeping system upgrades available under the same c/u/a menu.
func TestMissingPrerequisiteKeepsMenu(t *testing.T) {
	root := scriptFixture(t)
	writeFixture(t, root, "bin/node", "#!/bin/sh\necho v26.12.1\n")
	for _, name := range []string{"npm", "npx"} {
		writeFixture(t, root, "bin/"+name, "#!/bin/sh\nexit 92\n")
	}
	writeFixture(t, root, "bin/df", "#!/bin/sh\nprintf 'Filesystem 1024-blocks Used Available Capacity Mounted\\nfixture 99999999 1 9999999 1%% /\\n'\n")
	out, err := runFixture(t, root, `source scripts/_105_setup_test_tools.sh
source scripts/_116_test_tool_plan.sh
# Model a missing host command without hiding Bash's own utilities.
command() {
  if [ "$1" = -v ] && [ "${2:-}" = tar ]; then return 1; fi
  builtin command "$@"
}
trice_system_tool_setup() { echo system-option-reached; return 4; }
printf 'c\nu\na\n' | trice_test_tool_plan node`, map[string]string{"CI": "false"})
	assert.Error(t, err, out)
	assert.Regexp(t, `(?m)^tar +required +-- +-- +System +--$`, out)
	assert.Contains(t, out, "Missing prerequisite: tar")
	assert.Contains(t, out, "Cannot continue: tar")
	assert.Contains(t, out, "system-option-reached")
	assert.NotContains(t, out, "No installation needed.")
	assert.GreaterOrEqual(t, strings.Count(out, "Choose [c/u/a] (default: a):"), 3)
	assert.NoDirExists(t, filepath.Join(root, "temp", "tools"))
}

// TestToolPlanSizeLabels keeps approximate capacity labels consistent between
// installation rows and disk messages, including the switch to gigabytes.
func TestToolPlanSizeLabels(t *testing.T) {
	root := scriptFixture(t)
	out, err := runFixture(t, root, `source scripts/_105_setup_test_tools.sh
for size in 150 1024 4500; do trice_plan_size "$size"; printf '\n'; done`, nil)
	assert.NoError(t, err, out)
	assert.Equal(t, "~150 MB\n~1.0 GB\n~4.4 GB\n", out)
}

// TestToolPlanInventorySeparatesSystemAndCache uses real discovery and rendering
// on each platform branch. An older host, a usable cache and an incompatible
// cache must stay distinct; cancellation may never execute an installer.
func TestToolPlanInventorySeparatesSystemAndCache(t *testing.T) {
	for _, platform := range []string{"Linux", "Darwin", "MINGW64_NT-10.0"} {
		for _, cached := range []string{"26.12.1", "25.0.0"} {
			t.Run(platform+"/"+cached, func(t *testing.T) {
				root := scriptFixture(t)
				// Discovery must not reuse this Mac's real Homebrew Node. The
				// Windows branch needs a path-conversion double on non-Windows hosts.
				writeFixture(t, root, "bin/brew", "#!/bin/sh\nexit 1\n")
				writeFixture(t, root, "bin/cygpath", "#!/bin/sh\nfor arg do result=$arg; done\nprintf '%s\\n' \"$result\"\n")
				writeFixture(t, root, "bin/uname", "#!/bin/sh\ncase \"$1\" in -s) echo \"$FIXTURE_PLATFORM\" ;; -m) echo x86_64 ;; esac\n")
				writeFixture(t, root, "bin/node", "#!/bin/sh\necho v24.14.0\n")
				cache := "temp/tools/node/" + cached + "/node-v" + cached + "/bin/"
				binary := "node"
				if strings.HasPrefix(platform, "MINGW") {
					cache = "temp/tools/node/" + cached + "/node-v" + cached + "/"
					binary = "node.exe"
				}
				writeFixture(t, root, cache+binary, "#!/bin/sh\necho v"+cached+"\n")
				for _, tool := range []string{"npm", "npx"} {
					writeFixture(t, root, "bin/"+tool, "#!/bin/sh\nexit 92\n")
					writeFixture(t, root, cache+tool, "#!/bin/sh\nexit 92\n")
				}
				writeFixture(t, root, "bin/df", "#!/bin/sh\nprintf 'Filesystem 1024-blocks Used Available Capacity Mounted\\nfixture 99999999 1 9999999 1%% /\\n'\n")
				out, err := runFixture(t, root, `source scripts/_105_setup_test_tools.sh
source scripts/_116_test_tool_plan.sh
PATH="$PWD/$FIXTURE_CACHE:$PATH"
printf 'a\n' | trice_test_tool_plan node`, map[string]string{"CI": "false", "FIXTURE_PLATFORM": platform, "FIXTURE_CACHE": cache})
				assert.Error(t, err, out)
				assert.Regexp(t, `(?m)^Tool +Required +System +Cache +Location +Size$`, out)
				assert.Regexp(t, `(?m)^-{79}$`, out)
				assert.Regexp(t, `(?m)^Node[.]js +26[.]12[.]1 +24[.]14[.]0 +`+strings.ReplaceAll(cached, ".", `[.]`)+` +Cache +`, out)
				assert.NotContains(t, out, "Selected ")
				assert.NotContains(t, out, "Peak")
				if cached == "26.12.1" {
					assert.Contains(t, out, "No installation needed.")
				} else {
					assert.Contains(t, out, "~180 MB")
				}
				assert.Contains(t, out, "[u] Update system tools (up to pinned versions)")
				assert.Contains(t, out, "Install missing pinned tool versions into temp/tools.")
				assert.Contains(t, out, "Leave newer system versions unchanged.")
				assert.Contains(t, out, "Install remaining missing pinned tool versions into temp/tools.")
			})
		}
	}
}

// TestApprovedPlanRejectsAdditionalTools exercises the shared installer boundary
// without a real helper binary, network or compiler. Neither absent tools nor
// a vanished previously reusable runtime may expand the approved installation.
func TestApprovedPlanRejectsAdditionalTools(t *testing.T) {
	root := scriptFixture(t, "scripts/_115_prepare_native_tests.sh")
	writeFixture(t, root, "plan.tsv", "Go\t1.27.3\t1.27.3\treuse\tinstalled\ttemp/tools\t0\t0\n")
	out, err := runFixture(t, root, `source scripts/_105_setup_test_tools.sh
source scripts/_115_prepare_native_tests.sh
TRICE_PLAN_FILE="$PWD/plan.tsv"
TRICE_TEST_REPORT_BIN=must-not-execute
prepare_pinned_test_selection selected tools temp/tools/native shfmt`, nil)
	assert.Error(t, err, out)
	assert.Contains(t, out, "shfmt was not included")
	out, err = runFixture(t, root, `source scripts/_105_setup_test_tools.sh
TRICE_PLAN_FILE="$PWD/plan.tsv"
trice_plan_allows_install Go`, nil)
	assert.Error(t, err, out)
	assert.Contains(t, out, "installation of Go was not in the approved plan")
}

// TestLintEntryPlansBeforePreparation executes the direct script with the real
// planner. Only runtime probing and the final installers are doubles: preparation
// verifies that all its tool rows exist before marking the linter ready.
func TestLintEntryPlansBeforePreparation(t *testing.T) {
	root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_450_test_markdownlint.sh")
	common, err := os.ReadFile(filepath.Join(root, "scripts", "_100_test_common.sh"))
	require.NoError(t, err)
	writeFixture(t, root, "scripts/_100_test_common.sh", string(common)+`
source scripts/_116_test_tool_plan.sh
trice_find_go() { echo "$PWD/bin/go"; }
trice_find_node() { echo "$PWD/bin/node"; }
trice_plan_npm() {
  trice_plan_row markdownlint-cli "$TRICE_TOOL_MARKDOWNLINT_VERSION" missing install locked temp/tools 24 80
}

ensure_native_report_tool() { test -s "$TRICE_PLAN_FILE"; }
prepare_pinned_native_tools() {
  for name in Go Node.js goreleaser actionlint shellcheck jq shfmt mdtoc stitchmd markdownlint-cli; do
    awk -F '\t' -v name="$name" '$1==name {ok=1} END {exit !ok}' "$TRICE_PLAN_FILE" || return 91
  done
  echo complete-plan > prepared
}
`)
	writeFixture(t, root, "bin/markdownlint", "#!/bin/sh\nexit 0\n")
	writeFixture(t, root, "bin/df", "#!/bin/sh\nprintf 'Filesystem 1024-blocks Used Available Capacity Mounted\\nfixture 999999999 1 999999000 1%% /\\n'\n")
	out, err := runFixture(t, root, "printf 'c\\n' | bash scripts/_450_test_markdownlint.sh", map[string]string{"CI": "false"})
	assert.NoError(t, err, out)
	assert.FileExists(t, filepath.Join(root, "prepared"))
}

// TestBrowserPlanUsesCheckoutCache checks discovery's environment in a fresh
// process, before any browser installer can set or change its destination.
func TestBrowserPlanUsesCheckoutCache(t *testing.T) {
	root := scriptFixture(t)
	writeFixture(t, root, "scripts/testcheck/package-lock.json", "{}\n")
	writeFixture(t, root, "bin/node", `#!/bin/sh
echo "$PUPPETEER_CACHE_DIR" > discovered-cache
if [ "$2" = inspect ]; then
  printf 'md-to-pdf\t5.2.5\t5.2.5\treuse\t%s\n' "$PWD/global"
else echo browser-present; fi
`)
	writeFixture(t, root, "bin/npm", "#!/bin/sh\nexit 1\n")
	out, err := runFixture(t, root, `source scripts/_105_setup_test_tools.sh
TRICE_PLAN_FILE="$PWD/plan.tsv"
TRICE_PLAN_SCOPE=pdf
TRICE_PLAN_OS=linux
TRICE_PLAN_NODE="$PWD/bin/node"
unset PUPPETEER_CACHE_DIR
trice_plan_npm`, nil)
	assert.NoError(t, err, out)
	cache, err := os.ReadFile(filepath.Join(root, "discovered-cache"))
	require.NoError(t, err)
	assert.True(t, strings.HasSuffix(strings.TrimSpace(string(cache)), "/temp/tools/puppeteer"), string(cache))
	assert.NoDirExists(t, filepath.Join(root, "temp", "tools"))
}

// TestToolPlanConsentAndSpace executes the startup boundary without calling an
// installer. Platform doubles model df/uname only; version checks, cancellation,
// cache discovery and the displayed requirement calculation remain real.
func TestToolPlanConsentAndSpace(t *testing.T) {
	for _, platform := range []string{"Linux", "Darwin", "MINGW64_NT-10.0"} {
		for _, tc := range []struct {
			name, reply string
			matching    bool
			freeMiB     int
			wantSuccess bool
		}{
			{"matching_no_install", "c", true, 0, true},
			{"missing_approved", "c", false, 4096, true},
			{"missing_declined", "a", false, 4096, false},
			{"missing_EOF", "", false, 4096, false},
			{"insufficient_space", "c", false, 1, false},
			{"invalid_then_continue", "invalid\nc", false, 4096, true},
		} {
			t.Run(platform+"/"+tc.name, func(t *testing.T) {
				root := scriptFixture(t, "scripts/_116_test_tool_plan.sh")
				writeFixture(t, root, "bin/brew", "#!/bin/sh\nexit 1\n")
				writeFixture(t, root, "bin/cygpath", "#!/bin/sh\nfor arg do result=$arg; done\nprintf '%s\\n' \"$result\"\n")
				writeFixture(t, root, "bin/uname", "#!/bin/sh\ncase \"$1\" in -s) echo \"$FIXTURE_PLATFORM\" ;; -m) echo x86_64 ;; *) exit 90 ;; esac\n")
				writeFixture(t, root, "bin/df", "#!/bin/sh\nprintf 'Filesystem 1024-blocks Used Available Capacity Mounted\\nfixture 99999999 1 %s 1%% /\\n' \"$FIXTURE_FREE_KIB\"\n")
				version := "0.0.0"
				if tc.matching {
					version = "26.12.1"
				}
				writeFixture(t, root, "bin/node", "#!/bin/sh\n[ \"$1\" = --version ] || exit 91\necho v"+version+"\n")
				for _, tool := range []string{"npm", "npx", "curl"} {
					writeFixture(t, root, "bin/"+tool, "#!/bin/sh\necho unexpected-tool-execution >&2\ntouch unexpected-tool-execution\nexit 92\n")
				}
				body := "source scripts/_105_setup_test_tools.sh\nsource scripts/_116_test_tool_plan.sh\ntrice_test_tool_plan node\n"
				if tc.reply != "" {
					body = "printf '%s\\n' '" + tc.reply + "' | {\n" + body + "}\n"
				} else {
					body = "{\n" + body + "} </dev/null\n"
				}
				out, err := runFixture(t, root, body, map[string]string{
					"FIXTURE_PLATFORM": platform, "FIXTURE_FREE_KIB": strconv.Itoa(tc.freeMiB * 1024),
					"CI": "false", "TRICE_TEST_SETUP_APPROVED": "0", "TRICE_TEST_PLAN_SCOPES": "", "TRICE_TEST_PLAN_ACTIVE": "0",
				})
				assert.Equal(t, tc.wantSuccess, err == nil, out)
				assert.Contains(t, out, "26.12.1")
				assert.Contains(t, out, "free")
				if tc.matching {
					assert.Contains(t, out, "No installation needed.")
				} else {
					assert.Contains(t, out, "Install: Node.js (~180 MB)")
				}
				if tc.name == "insufficient_space" {
					assert.Contains(t, out, "Free another")
					assert.Contains(t, out, "Cannot continue: insufficient disk space.")
					assert.Contains(t, out, "[u] Update system tools (up to pinned versions)")
				}
				assert.NoFileExists(t, filepath.Join(root, "unexpected-tool-execution"))
				assert.NoDirExists(t, filepath.Join(root, "temp", "tools"), "planning must never install or extract a tool")
			})
		}
	}
}

// TestToolPlanListsOnlyMissingTools checks the user's two-mismatch scenario on
// all three platform branches. Complete compiler/npm selections are modeled;
// their real resource and module checks have separate behavioral tests.
func TestToolPlanListsOnlyMissingTools(t *testing.T) {
	for _, platform := range []string{"Linux", "Darwin", "MINGW64_NT-10.0"} {
		t.Run(platform, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_116_test_tool_plan.sh")
			writeFixture(t, root, "bin/uname", "#!/bin/sh\ncase \"$1\" in -s) echo \"$FIXTURE_PLATFORM\" ;; -m) echo x86_64 ;; esac\n")
			writeFixture(t, root, "bin/df", "#!/bin/sh\nprintf 'Filesystem 1024-blocks Used Available Capacity Mounted\\nfixture 999999999 1 999999000 1%% /\\n'\n")
			writeFixture(t, root, "bin/python3", "#!/bin/sh\nif [ \"$1\" = --version ]; then echo 'Python 3.14.0'; fi\nexit 0\n")
			gcc, clang := "16.2.0", "23.1.4"
			if platform == "Linux" {
				gcc = "15.2.0"
			}
			if strings.HasPrefix(platform, "MINGW") {
				clang = "23.1.3"
			}
			versions := map[string]string{
				"node": "26.12.1", "gcc": gcc, "clang": clang, "arm-none-eabi-gcc": "15.2.1", "cmake": "4.4.4",
				"goreleaser": "2.19.0", "actionlint": "1.7.12", "shellcheck": "0.11.0", "mdtoc": "1.0.3", "stitchmd": "0.9.0",
				"cppcheck": "2.22.0", "clang-format": "23.1.3", "go": "1.27.3", "jq": "0.0.0", "shfmt": "0.0.0",
			}
			for name, version := range versions {
				writeFixture(t, root, "bin/"+name, "#!/bin/sh\necho 'fixture "+version+"'\n")
			}
			for _, name := range []string{"npm", "npx", "xcrun"} {
				writeFixture(t, root, "bin/"+name, "#!/bin/sh\necho fixture\n")
			}
			body := `printf 'c\n' | {
source scripts/_105_setup_test_tools.sh
source scripts/_116_test_tool_plan.sh
trice_find_go() { printf '%s/bin/go\n' "$PWD"; }
trice_find_bundle() { printf '%s/complete-compiler\n' "$PWD"; }
trice_bundle_complete() { return 0; }
trice_plan_npm() {
  trice_plan_row markdownlint-cli "$TRICE_TOOL_MARKDOWNLINT_VERSION" "$TRICE_TOOL_MARKDOWNLINT_VERSION" reuse available temp/tools 0 0
  trice_plan_row md-to-pdf "$TRICE_TOOL_MD_TO_PDF_VERSION" "$TRICE_TOOL_MD_TO_PDF_VERSION" reuse available temp/tools 0 0
  trice_plan_row puppeteer "$TRICE_TOOL_PUPPETEER_VERSION" "$TRICE_TOOL_PUPPETEER_VERSION" reuse available temp/tools 0 0
}
trice_test_tool_plan native || exit $?
awk -F '\t' '$4=="install" {print "INSTALL=" $1}' "$TRICE_PLAN_FILE"
}
`
			out, err := runFixture(t, root, body, map[string]string{"FIXTURE_PLATFORM": platform, "CI": "false"})
			assert.NoError(t, err, out)
			assert.Equal(t, 2, strings.Count(out, "INSTALL="), out)
			assert.Contains(t, out, "INSTALL=jq")
			assert.Contains(t, out, "INSTALL=shfmt")
			assert.Contains(t, out, "Install: jq (~32 MB), shfmt (~32 MB)")
			assert.NoDirExists(t, filepath.Join(root, "temp", "tools"))
		})
	}
}

// TestToolPlanChecksEachFilesystem prevents a large free worktree volume from
// hiding a nearly full separate destination/cache filesystem.
func TestToolPlanChecksEachFilesystem(t *testing.T) {
	root := scriptFixture(t, "scripts/_116_test_tool_plan.sh")
	writeFixture(t, root, "disk-a/.keep", "")
	writeFixture(t, root, "disk-b/.keep", "")
	writeFixture(t, root, "space.tsv", "one\t1.0.0\tmissing\tinstall\trelease\tdisk-a\t5\t10\ntwo\t1.0.0\tmissing\tinstall\trelease\tdisk-b\t20\t50\n")
	writeFixture(t, root, "bin/df", `#!/bin/sh
case "$2" in *disk-b) device=disk-b; free=1024 ;; *) device=disk-a; free=99999999 ;; esac
printf 'Filesystem 1024-blocks Used Available Capacity Mounted\n%s 999999999 1 %s 1%% /\n' "$device" "$free"
`)
	out, err := runFixture(t, root, "source scripts/_105_setup_test_tools.sh\nTRICE_PLAN_FILE=space.tsv\nTRICE_TEST_DISK_RESERVE_MIB=10\ntrice_plan_space\n", nil)
	assert.Error(t, err, out)
	assert.Contains(t, out, "Disk [disk-b]: ~1 MB free; insufficient.")
	assert.Contains(t, out, "Free another ~59 MB before setup.")
}

// TestNpmSelectionAndLockSubset uses real module resolution and the maintained
// dependency graph. Correct globals and independent cache roots stay usable;
// a nested incompatible Puppeteer must not be mistaken for the top-level pin.
func TestNpmSelectionAndLockSubset(t *testing.T) {
	node, err := exec.LookPath("node")
	require.NoError(t, err, "npm selection checks require the existing Node runtime")
	root := scriptFixture(t, "scripts/_test_npm_tools.cjs", "scripts/testcheck/package-lock.json")
	writeFixture(t, root, "check.cjs", `const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const helper = require('./scripts/_test_npm_tools.cjs');
// Synthetic installations have real package resolution but no npm/network use.
function pkg(root, name, version) {
  const directory = path.join(root, 'node_modules', name);
  fs.mkdirSync(directory, {recursive:true});
  fs.writeFileSync(path.join(directory,'package.json'),JSON.stringify({name,version,main:'index.cjs',bin:'index.cjs'}));
  fs.writeFileSync(path.join(directory,'index.cjs'),'if(process.argv.includes("--version")) console.log('+JSON.stringify(version)+'); module.exports = {};');
  if(name==='md-to-pdf') {
    fs.mkdirSync(path.join(directory,'lib'),{recursive:true});
    fs.writeFileSync(path.join(directory,'lib/get-marked-with-highlighter.js'),'exports.getMarked = () => ({});');
  }
  return directory;
}

const installed = path.resolve('global'), cache = path.resolve('cache');
const versions = ['0.50.0','5.2.5','25.16.0'];
pkg(installed,'markdownlint-cli','0.50.0');
const renderer = pkg(installed,'md-to-pdf','5.2.5');
pkg(installed,'puppeteer','25.16.0');
let selected = helper.inspect([installed,cache],versions,{write(){}});
assert.equal(selected[0].root,installed);
assert.equal(selected[1].root,installed);
// A satisfying top-level version cannot hide an incompatible nested dependency.
const brokenRoot = path.resolve('broken-global');
pkg(brokenRoot,'markdownlint-cli','0.50.0');
const brokenRenderer = pkg(brokenRoot,'md-to-pdf','5.2.5');
pkg(brokenRoot,'puppeteer','25.16.0');
pkg(brokenRenderer,'puppeteer','99.0.0');
pkg(cache,'md-to-pdf','5.2.5');
pkg(cache,'puppeteer','25.16.0');
fs.writeFileSync(path.join(cache,'.ready'),'cache');
selected = helper.inspect([brokenRoot,cache],versions,{write(){}});
assert.equal(selected[0].root,brokenRoot);
assert.equal(selected[1].root,cache);
// Inventory keeps the host and cache separate even when selection needs only
// one of them. A host-only linter must not be attributed to the checkout cache.
helper.inspect([brokenRoot,cache],versions,{write(){}},'inventory.tsv');
const inventory = fs.readFileSync('inventory.tsv','utf8');
assert.ok(inventory.includes('markdownlint-cli\t0.50.0\t--\n'));
assert.ok(inventory.includes('md-to-pdf\t5.2.5\t5.2.5\n'));
assert.ok(inventory.includes('puppeteer\t99.0.0\t25.16.0\n'));
const full = JSON.parse(fs.readFileSync('scripts/testcheck/package-lock.json','utf8'));
const markdown = helper.subsetLock(full,['markdownlint-cli']);
assert.equal(markdown.packages[''].dependencies['markdownlint-cli'],'0.50.0');
assert.ok(!markdown.packages['node_modules/md-to-pdf']);
assert.ok(!markdown.packages['node_modules/puppeteer']);
const pdf = helper.subsetLock(full,['md-to-pdf','puppeteer']);
assert.equal(pdf.packages['node_modules/puppeteer'].version,'25.16.0');
assert.ok(!pdf.packages['node_modules/markdownlint-cli']);
// Every retained package preserves the exact original integrity and resolution.
for(const [name,pkg] of Object.entries(pdf.packages)) if(name) assert.deepEqual(pkg,full.packages[name]);
assert.throws(()=>helper.subsetLock(full,['not-declared']),/no direct dependency/);
const broken = {packages:{'':{dependencies:{fixture:'1.0.0'}},'node_modules/fixture':{dependencies:{absent:'1.0.0'}}}};
assert.throws(()=>helper.subsetLock(broken,['fixture']),/lacks dependency absent/);
console.log('selection and dependency closure passed');
`)
	command := exec.Command(node, "check.cjs")
	command.Dir = root
	command.Env = append(os.Environ(), "CI=false")
	output, err := command.CombinedOutput()
	assert.NoError(t, err, string(output))
	assert.True(t, strings.Contains(string(output), "selection and dependency closure passed"), string(output))
	// Let the installed npm validate a generated graph without fetching or
	// extracting packages. This catches lockfile consistency rules beyond our
	// own traversal assertions, including the renderer's peer dependencies.
	out, npmErr := runFixture(t, root, "node scripts/_test_npm_tools.cjs lock scripts/testcheck/package-lock.json subset md-to-pdf puppeteer\nnpm ci --prefix subset --cache \"$PWD/npm-cache\" --dry-run --offline --ignore-scripts --no-audit --no-fund\n", nil)
	assert.NoError(t, npmErr, out)
	assert.NoFileExists(t, filepath.Join(root, "subset", "node_modules", "md-to-pdf", "package.json"), "dry-run must not install a real tool")
}

// TestPinnedSystemToolUpdates never invokes a real package manager or sudo.
// Only older managed tools may be upgraded to an available exact pin. A newer,
// absent or unidentifiable installation stays unchanged for local preparation.
func TestPinnedSystemToolUpdates(t *testing.T) {
	for _, manager := range []string{"apt", "brew", "winget"} {
		for _, tc := range []struct {
			name, reply string
			available   bool
			changed     bool
			freeMiB     int
			wantInstall bool
		}{
			{"exact_pin_approved", "y", true, false, 8192, true},
			{"exact_pin_declined", "n", true, false, 8192, false},
			{"different_pin_only", "y", false, false, 8192, false},
			{"prerelease_is_not_stable_pin", "y", false, false, 8192, false},
			{"available_subset", "y", true, false, 8192, true},
			{"manual_prerequisite", "y", true, false, 32768, true},
			{"insufficient_space", "y", true, false, 1, false},
			{"brew_catalog_changed", "y", true, true, 8192, false},
			{"already_pinned", "y", true, false, 8192, false},
			{"newer_system", "y", true, false, 8192, false},
			{"newer_inactive_copy", "y", true, false, 8192, false},
			{"missing_system", "y", true, false, 8192, false},
			{"unknown_system", "y", true, false, 8192, false},
			{"upgrade_failed", "y", true, false, 8192, false},
			{"installed_changed", "y", true, false, 8192, false},
		} {
			if tc.changed && manager != "brew" {
				continue
			}
			if tc.name == "manual_prerequisite" && manager != "brew" {
				continue
			}
			// Multiple installed kegs are a Homebrew case, not a skipped
			// required test on the other package-manager branches.
			if tc.name == "newer_inactive_copy" && manager != "brew" {
				continue
			}
			t.Run(manager+"/"+tc.name, func(t *testing.T) {
				root := scriptFixture(t, "scripts/_116_test_tool_plan.sh", "scripts/_117_system_test_tools.sh")
				plan := "Node.js\t26.12.1\t24.0.0\tinstall\tofficial archive\ttemp/tools\t180\t300\n"
				if tc.name == "manual_prerequisite" {
					plan = "gcc\t16.2.0\t15.0.0\tmanual\tinstall matching native GCC\ttemp/tools\t0\t0\n"
				}
				if tc.name == "available_subset" {
					plan += "mdtoc\t1.0.3\tmissing\tinstall\trelease\ttemp/tools\t32\t64\n"
				}
				writeFixture(t, root, "plan.tsv", plan)
				writeFixture(t, root, "system-cache/.keep", "")
				writeFixture(t, root, "bin/df", "#!/bin/sh\nprintf 'Filesystem 1024-blocks Used Available Capacity Mounted\\nfixture 99999999 1 %s 1%% /\\n' \"$FIXTURE_FREE_KIB\"\n")
				writeFixture(t, root, "bin/id", "#!/bin/sh\necho 1000\n")
				writeFixture(t, root, "bin/sudo", "#!/bin/sh\nexec \"$@\"\n")
				for _, tool := range []string{"npm", "npx"} {
					writeFixture(t, root, "bin/"+tool, "#!/bin/sh\nexit 91\n")
				}
				writeFixture(t, root, "bin/apt-config", "#!/bin/sh\nprintf 'Dir::Cache \"%s/system-cache\";\\n' \"$PWD\"\n")
				// A second identity query models another process upgrading the
				// package while the user reviews the proposed commands.
				versionQuery := `version="$FIXTURE_INSTALLED"
if [ "$FIXTURE_INSTALLED_CHANGED" = true ] && { [ "$1" = list ] || [ "$1" = -W ]; }; then
  if [ -f version-seen ]; then version=26.12.2; fi
  touch version-seen
fi
`
				writeFixture(t, root, "bin/dpkg-query", "#!/bin/sh\n"+versionQuery+"[ -n \"$version\" ] || exit 1\nprintf 'install ok installed\\t%s-1\\n' \"$version\"\n")
				writeFixture(t, root, "bin/apt-cache", "#!/bin/sh\nprintf 'nodejs | %s-1 | fixture-repository\\n' \"$FIXTURE_VERSION\"\n")
				writeFixture(t, root, "bin/apt-get", "#!/bin/sh\n[ \"$FIXTURE_FAILED\" != true ] || exit 91\nprintf '%s\\n' \"$@\" > installation.args\n")
				writeFixture(t, root, "bin/brew", "#!/bin/sh\n"+versionQuery+`
case "$1" in
  --prefix) printf '%s\n' "$PWD" ;;
  --cache) printf '%s/system-cache\n' "$PWD" ;;
  list) [ -z "$version" ] || printf '%s %s\n' "$4" "$version" ;;
  info)
    version="$FIXTURE_VERSION"
    if [ "$FIXTURE_CHANGED" = true ] && [ -f brew-info-seen ]; then version=26.12.2; fi
    touch brew-info-seen
    printf '{"formulae":[{"versions":{"stable":"%s"}}]}\n' "$version"
    ;;
  upgrade) [ "$FIXTURE_FAILED" != true ] || exit 91; printf '%s\n' "$@" > installation.args ;;
  *) exit 92 ;;
esac
`)
				writeFixture(t, root, "bin/winget", "#!/bin/sh\n"+versionQuery+`
case "$1" in
  show) [ "$FIXTURE_VERSION" = 26.12.1 ] ;;
  list) [ -z "$version" ] || printf 'Node.js %s %s winget\n' "$3" "$version" ;;
  upgrade) [ "$FIXTURE_FAILED" != true ] || exit 91; printf '%s\n' "$@" > installation.args ;;
  *) exit 93 ;;
esac
`)
				platform := map[string]string{"apt": "linux", "brew": "darwin", "winget": "windows"}[manager]
				version := "26.12.1"
				installed := "24.0.0"
				switch tc.name {
				case "already_pinned":
					installed = "26.12.1"
				case "newer_system":
					installed = "26.12.10"
				case "newer_inactive_copy":
					installed = "24.0.0 26.12.10"
				case "missing_system":
					installed = ""
				case "unknown_system":
					installed = "Unknown"
				}
				if tc.name == "manual_prerequisite" {
					version = "16.2.0"
					installed = "15.0.0"
				}
				if !tc.available {
					version = "26.12.10"
				}
				if tc.name == "prerelease_is_not_stable_pin" {
					version = "26.12.1~rc1"
				}
				out, err := runFixture(t, root, "printf '%s\\n' '"+tc.reply+"' | {\nsource scripts/_105_setup_test_tools.sh\nTRICE_PLAN_FILE=\"$PWD/plan.tsv\"\nTRICE_PLAN_OS=\"$FIXTURE_OS\"\ntrice_system_tool_setup\n}\n", map[string]string{
					"FIXTURE_OS": platform, "FIXTURE_VERSION": version, "FIXTURE_CHANGED": strconv.FormatBool(tc.changed),
					"FIXTURE_INSTALLED": installed, "FIXTURE_FAILED": strconv.FormatBool(tc.name == "upgrade_failed"),
					"FIXTURE_INSTALLED_CHANGED": strconv.FormatBool(tc.name == "installed_changed"),
					"FIXTURE_FREE_KIB":          strconv.Itoa(tc.freeMiB * 1024), "CI": "false",
					"PROGRAMFILES": root, "LOCALAPPDATA": root, "TEMP": filepath.Join(root, "system-cache"),
				})
				if tc.wantInstall {
					assert.NoError(t, err, out)
					arguments, readErr := os.ReadFile(filepath.Join(root, "installation.args"))
					require.NoError(t, readErr, out)
					switch manager {
					case "apt":
						assert.Contains(t, string(arguments), "nodejs=26.12.1-1")
						assert.Contains(t, string(arguments), "--only-upgrade")
					case "brew":
						if tc.name == "manual_prerequisite" {
							assert.Equal(t, "upgrade\n--formula\ngcc\n", string(arguments))
							details, detailsErr := os.ReadFile(filepath.Join(root, "plan.tsv.system.details"))
							require.NoError(t, detailsErr)
							assert.Contains(t, string(details), "1500 MiB additional installed data")
							assert.Contains(t, string(details), "separate dependency and package-cache allowances")
						} else {
							assert.Equal(t, "upgrade\n--formula\nnode\n", string(arguments))
						}
					case "winget":
						assert.Contains(t, string(arguments), "upgrade\n")
						assert.Contains(t, string(arguments), "--version\n26.12.1\n")
					}
					assert.Contains(t, out, "Rechecking tools")
					assert.Contains(t, out, "Updating system")
					assert.Contains(t, out, "... done")
				} else {
					assert.Error(t, err, out)
					assert.NoFileExists(t, filepath.Join(root, "installation.args"), out)
					if tc.name == "installed_changed" {
						assert.Contains(t, out, "changed since the upgrade plan")
					}
				}
				assert.NotContains(t, out, "Checking exact system packages")
				assert.NotContains(t, out, "not installed, not older, or version unknown")
			})
		}
	}
}

// TestSystemUpgradeVersionBounds keeps numeric ordering portable and refuses
// ambiguous identities instead of guessing whether a downgrade would occur.
func TestSystemUpgradeVersionBounds(t *testing.T) {
	root := scriptFixture(t)
	out, err := runFixture(t, root, `source scripts/_105_setup_test_tools.sh
for pair in '1.7.1 1.8.2' '1.8.2 1.8.2' '1.10.0 1.8.2' '15.3 15.2.1' '1:1.7.1-2 1.8.2' '1.7.1_3 1.8.2' 'Unknown 1.8.2' '1.8.2~rc1 1.8.2'; do
  read -r current pin <<<"$pair"
  if trice_system_version_is_older "$current" "$pin"; then echo "$current: upgrade"; else echo "$current: unchanged"; fi
done`, nil)
	require.NoError(t, err, out)
	assert.Equal(t, "1.7.1: upgrade\n1.8.2: unchanged\n1.10.0: unchanged\n15.3: unchanged\n1:1.7.1-2: upgrade\n1.7.1_3: upgrade\nUnknown: unchanged\n1.8.2~rc1: unchanged\n", out)
}

// TestUpdateChoiceContinuesLocalPreparation covers the combined startup path.
// Replanning sees changed tools, preserves remaining local work and starts no
// second menu after a confirmed update. Failed/refused upgrades grant no consent.
func TestUpdateChoiceContinuesLocalPreparation(t *testing.T) {
	for _, tc := range []struct {
		name, replies, updateBody, action string
		wantSuccess                       bool
	}{
		{"updated_system", "u", `printf '#!/bin/sh\necho v26.12.1\n' > bin/node; return 0`, "reuse", true},
		{"unavailable_uses_local", "u", "return 3", "install", true},
		{"declined_then_continue", "u\nc", "return 4", "install", true},
		{"declined_then_abort", "u\na", "return 4", "", false},
		{"failed_stops", "u\nc", "return 2", "", false},
		{"old_choice_rejected", "s\na", "return 0", "", false},
		{"missing_prerequisite", "u", "return 3", "", false},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t)
			writeFixture(t, root, "bin/brew", "#!/bin/sh\nexit 1\n")
			writeFixture(t, root, "bin/node", "#!/bin/sh\necho v24.0.0\n")
			for _, tool := range []string{"npm", "npx"} {
				writeFixture(t, root, "bin/"+tool, "#!/bin/sh\nexit 91\n")
			}
			writeFixture(t, root, "bin/df", "#!/bin/sh\nprintf 'Filesystem 1024-blocks Used Available Capacity Mounted\\nfixture 99999999 1 9999999 1%% /\\n'\n")
			body := `source scripts/_105_setup_test_tools.sh
source scripts/_116_test_tool_plan.sh
trice_system_tool_setup() { echo upgrade-attempt; ` + tc.updateBody + `; }
if [ "$FIXTURE_MISSING" = true ]; then
  command() {
    if [ "$1" = -v ] && [ "${2:-}" = tar ]; then return 1; fi
    builtin command "$@"
  }
fi
trice_test_tool_plan node || exit $?
test "$TRICE_TEST_SETUP_APPROVED" = 1 || exit 92
awk -F '\t' '$1=="Node.js" {print "NODE_ACTION=" $4}' "$TRICE_PLAN_FILE"
echo local-preparation-reached
`
			out, err := runFixture(t, root, "printf '%s\\n' '"+tc.replies+"' | {\n"+body+"}\n", map[string]string{
				"CI": "false", "FIXTURE_MISSING": strconv.FormatBool(tc.name == "missing_prerequisite"),
			})
			assert.Equal(t, tc.wantSuccess, err == nil, out)
			assert.Equal(t, 1, strings.Count(out, "Tool             Required"), "tool rechecks must not repeat the inventory")
			if tc.wantSuccess {
				assert.Contains(t, out, "NODE_ACTION="+tc.action)
				assert.Contains(t, out, "local-preparation-reached")
				if tc.name != "declined_then_continue" {
					assert.Equal(t, 1, strings.Count(out, "Choose [c/u/a]"))
				}
			} else {
				assert.NotContains(t, out, "local-preparation-reached")
			}
			if tc.name == "old_choice_rejected" {
				assert.Contains(t, out, "Please enter c, u, or a.")
				assert.NotContains(t, out, "upgrade-attempt")
			}
			assert.NoDirExists(t, filepath.Join(root, "temp", "tools"), "the plan itself must not install tools")
		})
	}
}

// TestSetupProgress preserves parent-shell selections and stdout protocols,
// reports long-running work, and retains the original install failure status.
func TestSetupProgress(t *testing.T) {
	for _, tc := range []struct {
		name, work string
		wantStatus int
		heartbeat  bool
	}{
		{"success", "export FIXTURE_SELECTED=ready; printf selected-path", 0, false},
		{"failure", "return 23", 23, false},
		{"long_install", "sleep 2; export FIXTURE_SELECTED=ready; printf selected-path", 0, true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t)
			out, err := runFixture(t, root, `source scripts/_105_setup_test_tools.sh
prepare_fixture() { `+tc.work+`; }
status=0
trice_setup_with_progress 'Installing fixture 1.2.3 into temp/tools' prepare_fixture >selection.txt || status=$?
echo "STATUS=$status"
echo "SELECTION=${FIXTURE_SELECTED:-missing}"
`, map[string]string{"TRICE_TEST_PROGRESS_INTERVAL_SECONDS": "1", "FIXTURE_SELECTED": ""})
			require.NoError(t, err, out)
			assert.Contains(t, out, "STATUS="+strconv.Itoa(tc.wantStatus))
			assert.Contains(t, out, "Installing fixture 1.2.3 into temp/tools...")
			if tc.wantStatus == 0 {
				assert.Contains(t, out, "SELECTION=ready", "the progress wrapper must preserve installer environment changes")
				assert.Contains(t, out, "... done")
				selection, readErr := os.ReadFile(filepath.Join(root, "selection.txt"))
				require.NoError(t, readErr)
				assert.Equal(t, "selected-path", string(selection), "status lines must not contaminate returned tool paths")
			} else {
				assert.Contains(t, out, "FAILED (exit 23)")
				assert.NotContains(t, out, "... done")
			}
			if tc.heartbeat {
				assert.Contains(t, out, "Still working: Installing fixture")
			}
		})
	}
}
