# Building and Releasing the Tools

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Build Trice tool from Go sources](#build-trice-tool-from-go-sources)
* [2. Prepare A Release](#prepare-a-release)
* [3. Publish a Release Manually from a Selected Tag](#publish-a-release-manually-from-a-selected-tag)
  * [3.1. Verify the Candidate Commit](#verify-the-candidate-commit)
  * [3.2. Create and Transfer the Release Tag](#create-and-transfer-the-release-tag)
  * [3.3. Start the Release Workflow](#start-the-release-workflow)
  * [3.4. Inspect the Published Assets](#inspect-the-published-assets)
  * [3.5. Audit That Exact Release](#audit-that-exact-release)
  * [3.6. Install a Beta or a Stable Release](#install-a-beta-or-a-stable-release)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## 1. <a id="build-trice-tool-from-go-sources"></a>Build Trice tool from Go sources

* Install [Go](https://go.dev/) at the version required by [go.mod](../../go.mod), and Bash (Git Bash on Windows).
* From the repository root, run:

  ```bash
  ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (main)
  $ bash ./scripts/buildTriceTool.sh # installs ./cmd/trice and ./cmd/tlog locally
  ```

* The script prints the installation paths for both `trice` and `tlog` (`.exe` on Windows). It uses `GOBIN` when set; otherwise it uses the first `GOPATH` entry plus `/bin`, normally `~/go/bin`.
* Add that directory to `PATH`, or copy both binaries to a directory already in `PATH`. Verify with `trice version` and `tlog --version`.
* An existing tool is protected against accidental replacement. An interactive run asks before overwriting; unattended runs require `--force`. Backups are enabled by default; `--no-backup` disables them.
* Check:

  ```bash
  ms@PaulPCWin11 MINGW64 ~/repos/trice (main)
  $ ./scripts/buildTriceTool.sh
  ----------------------------------------
  Building trice with embedded Git metadata:
    origin:     git@github.com:rokath/trice.git
    branch:     main
    version:    branch dirty
    commit:     f7edcc51
    date:       2025-11-27T13:59:50+01:00
    git_state:  dirty
    git_status:  M .vscode/launch.json  M docs/TriceReferenceManual.md  M internal/emitter/lineComposer.go
  ----------------------------------------
  Build complete.
  
  ms@PaulPCWin11 MINGW64 ~/repos/trice (main)
  $ trice version
  no version, branch=git@github.com:rokath/trice.git - main (local modifications at build time), commit=f7edcc51,   built at 2025-11-27T13:59:50+01:00
  
  ms@PaulPCWin11 MINGW64 ~/repos/trice (main)
  ```

* Hints
  * For a release build, check out the desired release tag first. Development branches may contain unreleased changes.
  * When using [./scripts/buildTriceTool.sh](../../scripts/buildTriceTool.sh), the generated Trice image is significant smaller (about 30%), because the build script removes debugging information from the Trice binary. The Trice release images contain this additionally information for a more verbose error reporting, just in case.  
  * `tlog` is a separate binary for the `trice log` runtime path. It accepts the log flags directly, for example `tlog -port HEX ...`, and release archives/packages ship it next to `trice`.
  * Give each Trice binary its own name when using different images. Otherwise you always get what is found first in the **$PATH**.
  * Use GoReleaser if you wish to create releases on your forked Trice repository.
  * For Windows CGO and host C tests, follow the [compiler selection and runtime checks](development_environment_setup.md#choose-the-right-windows-compiler). Run the commands from the Trice directory in Git Bash.
  * Use [Running the Tests](target_tests.md) for native compiler prerequisites, local checks and the full/quick selections.

```bash
ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (main)
$ go install ./cmd/trice ./cmd/tlog
```

This direct local command also builds both tools, using Go's `GOBIN`/`GOPATH` installation directory.

Prefer `bash ./scripts/buildTriceTool.sh` for metadata and overwrite protection. It includes the current repository state in both binaries, shown by `trice version` and `tlog --version`.

## 2. <a id="prepare-a-release"></a>Prepare A Release

Follow [GoReleaser and release-package testing](target_tests.md#goreleaser-and-release-package-testing) for configuration checks, disposable local snapshots, native package installation and published-release audits. That chapter lists prerequisites and explains what each level proves.

Use this checklist for each selected release. Acceptance checks do not add features or require general documentation changes; a failure needs a focused correction and renewed evidence for the affected checks.

| Before creating the tag | Required evidence |
| --- | --- |
| Candidate | Approved commit, intended branch history, clean worktree and recorded commit ID. |
| Native tests | Successful full runs on Linux, macOS and Windows for that candidate; distinguish Intel/Apple Silicon results and any accepted platform limits. Quick PR checks remain useful separate evidence. |
| CI and coverage | Required Actions pass for the candidate; inspect skipped/incomplete jobs and compare coverage with the target-branch baseline using the declared package selection. |
| Installation and snapshot | Both tools, target sources, complete licenses, platform archives, Linux packages and both manual PDFs checked through the existing snapshot/installation jobs. |
| Notes and known limits | The selected version has nonempty English release notes, correct links and an explicit record of accepted deviations. |
| Relevant manual checks | For changed TCP behavior, check Windows for a delayed firewall dialog after process exit. Record RTT hardware checks separately with board, transport and tool versions; compiler tests do not establish hardware operation. |

| After publication | Required evidence |
| --- | --- |
| Selected release | The expected tag/commit and prerelease/stable status are visible. |
| Downloads | Actual published archives, packages, checksums, target sources and both PDFs are present and usable at their final URLs. |
| Release audit | The manually selected tag passes archive, package-installation and shipped-feature checks. Retain the run URL and any accepted limits. |

Local full runs and Actions cover the candidate. The post-publication checks cover the files users actually download. Preserve the distinction when recording results; older successful runs remain evidence for their original revisions.

## 3. <a id="publish-a-release-manually-from-a-selected-tag"></a>Publish a Release Manually from a Selected Tag

The [release workflow](../../.github/workflows/goreleaser.yml) runs only when started manually. Publishing a Git tag makes the selected source snapshot available; the separate workflow start publishes its release assets.

### 3.1. <a id="verify-the-candidate-commit"></a>Verify the Candidate Commit

Review the release notes and commit the intended changes. Inspect the remote branches before choosing the candidate:

```sh
git fetch origin --tags
git status --short
git branch -vv
git log --oneline --left-right origin/main...origin/wip
git log --oneline -5 HEAD
```

Confirm which reviewed changes belong in the release and complete their normal branch/PR integration. A tag identifies a commit, not all changes present on another branch. On the chosen clean checkout, record and test the exact candidate:

```sh
release_commit=$(git rev-parse HEAD)
printf 'Release candidate: %s\n' "$release_commit"
./scripts/testAll.sh full
git status --short
```

Follow the [release-candidate checks](target_tests.md#before-accepting-a-release-candidate) on this same fixed commit, including source restoration. The full code suite does not generate snapshot packages; package construction and native installation are separate checks.

### 3.2. <a id="create-and-transfer-the-release-tag"></a>Create and Transfer the Release Tag

Use an annotated tag on the approved commit. In one Bash session, select an unused version and the branch containing the approved workflow. This example uses the next beta after beta 3; change it to the version actually being released:

```sh
release_tag=v2.0.0-beta-4
workflow_branch=main
git rev-parse "$release_commit^{commit}"
git show-ref --tags
git ls-remote --tags origin "refs/tags/$release_tag" "refs/tags/$release_tag^{}"
```

The selected tag must be absent locally and remotely. After transferring the approved branch through its normal PR/push workflow, verify that the remote branch contains the candidate, then create and transfer only that tag:

```sh
git fetch origin "$workflow_branch"
git merge-base --is-ancestor "$release_commit" "origin/$workflow_branch" &&
  git tag -a "$release_tag" "$release_commit" -m "Trice $release_tag" &&
  git push origin "refs/tags/$release_tag"
git rev-parse "$release_tag^{}"
git ls-remote --tags origin "refs/tags/$release_tag^{}"
```

The local commit and the remote annotated tag's peeled commit must agree. Keep the tag fixed after publishing so its sources, tool metadata and documentation continue to describe the same release. A failed ancestry check means branch integration is incomplete; resolve it before tagging.

### 3.3. <a id="start-the-release-workflow"></a>Start the Release Workflow

In GitHub Actions, open **goreleaser**, choose **Run workflow**, select the branch containing the approved pipeline, and enter the selected release tag in the required `tag` field.

The workflow checks out `refs/tags/<tag>`, checks its commit, and explicitly passes that tag to GoReleaser. The workflow branch selects the pipeline; the tag selects the sources and release version. In the same Bash session:

```sh
gh workflow run goreleaser.yml --repo rokath/trice --ref "$workflow_branch" -f tag="$release_tag"
gh run list --repo rokath/trice --workflow goreleaser.yml --limit 5
```

The workflow uses the selected version's section in [CHANGELOG.md](../../CHANGELOG.md) as the release notes; a missing or empty section stops publication. Wait for the **Run GoReleaser** and **Verify generated release artifacts** steps to succeed. During the v2 beta phase, [the release configuration](../../.goreleaser.yaml) marks releases as prereleases.

### 3.4. <a id="inspect-the-published-assets"></a>Inspect the Published Assets

Open the release with the selected tag and check:

- Platform archives contain both `trice` and `tlog`, plus the complete `LICENSE.md`.
- The target-source ZIP contains `src`, `src/default_conf`, the repository license and the SEGGER license.
- Linux packages, checksums and both `TriceUserManual.pdf` and `TriceReferenceManual.pdf` are present.
- The host tools report the expected version and commit.

### 3.5. <a id="audit-that-exact-release"></a>Audit That Exact Release

Start [release-audit](../../.github/workflows/release-audit.yml) manually with the same `tag`. For example:

```sh
gh release view "$release_tag" --repo rokath/trice
gh workflow run release-audit.yml --repo rokath/trice --ref "$workflow_branch" -f tag="$release_tag"
gh run list --repo rokath/trice --workflow release-audit.yml --limit 5
```

The audit downloads that release explicitly and tests its archives, packages, target sources and PDFs. The host archive jobs also compile the PC feature tour against the unpacked target library, then verify text, JSON and KV with both shipped tools. Missing artifacts, changed licenses, incorrect fields or non-record JSON stdout fail the audit.

### 3.6. <a id="install-a-beta-or-a-stable-release"></a>Install a Beta or a Stable Release

For a beta, download the matching platform archive from its tagged release and place its tools in `PATH`, or build from a checkout of that tag. Use the matching target sources and manuals.

The module remains `github.com/rokath/trice`. Versioned remote installation with a v2 tag is not supported without a `/v2` module path; use the archive or local checkout build described above. See [Go major version suffixes](https://go.dev/ref/mod#major-version-suffixes).

Homebrew remains the stable-release route. `skip_upload: auto` prevents prerelease tags from updating the normal tap, and beta audits skip the Homebrew job. Before publishing a stable v2 release, review `release.prerelease` and the stable installation checks explicitly.

