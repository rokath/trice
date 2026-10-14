# Building and Releasing the Trice Tools

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Build Trice tool from Go sources](#build-trice-tool-from-go-sources)
* [2. Prepare A Release](#prepare-a-release)
  * [2.1. Check a GoReleaser Release before Publishing](#check-a-goreleaser-release-before-publishing)
* [3. Publish a Release Manually from a Selected Tag](#publish-a-release-manually-from-a-selected-tag)
  * [3.1. Verify the Candidate Commit](#verify-the-candidate-commit)
  * [3.2. Create and Transfer the Release Tag](#create-and-transfer-the-release-tag)
  * [3.3. Start the Release Workflow](#start-the-release-workflow)
  * [3.4. Inspect the Published Assets](#inspect-the-published-assets)
  * [3.5. Audit That Exact Release](#audit-that-exact-release)
  * [3.6. Install a Beta or a Stable Release](#install-a-beta-or-a-stable-release)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="build-trice-tool-from-go-sources"></a>Build Trice tool from Go sources

* Install [Go](https://go.dev/).
* Run:

  ```bash
  ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (main)
  $ bash ./scripts/buildTriceTool.sh # internally runs go install ./cmd/trice/...
  ```

* Afterwards you should find executables `trice` and `tlog` inside `~/go/bin`.
* Extend PATH variable with `~/go/bin` **OR** copy the Trice binaries from there into a folder of your path.
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
  * Use only the main branch. Other branches may be inconsistent.
  * When using [./scripts/buildTriceTool.sh](../../scripts/buildTriceTool.sh), the generated Trice image is significant smaller (about 30%), because the build script removes debugging information from the Trice binary. The Trice release images contain this additionally information for a more verbose error reporting, just in case.  
  * `tlog` is a separate binary for the `trice log` runtime path. It accepts the log flags directly, for example `tlog -port HEX ...`, and release archives/packages ship it next to `trice`.
  * Give each Trice binary its own name when using different images. Otherwise you always get what is found first in the **$PATH**.
  * Use GoReleaser if you wish to create releases on your forked Trice repository.
  * On Windows, install [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/) or another compatible MinGW-w64 GCC distribution if you wish to execute the CGO and host C-code tests.
    * Match the compiler architecture to Go, normally 64-bit Go with a 64-bit host compiler.
    * The minimal TDM-GCC online installation is sufficient for these host tests.
    * Put the selected host compiler first in `PATH` when several GCC variants are installed.
    * If `clang` is also in `PATH`, the regression tests can execute it in addition to GCC. Clang must therefore have a usable host C runtime configuration; merely finding `clang.exe` is not enough.
    * A Windows Clang using the GNU target can reuse the installed MinGW-w64 runtime. A Clang using the MSVC target instead requires the Visual Studio C++ Build Tools and a Windows SDK.
    * Verify each visible host compiler can include `string.h` before running `go test ./...`.
  * Open a console inside the Trice directory, recommended is the git-bash, when using Windows.
  * Tests:

  ```b
  ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (main)
  $ go clean -cache
  
  ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (main)
  $ go vet ./...
  
  ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (main)
  $ go test ./...
  ?       github.com/rokath/trice/cmd/cui [no test files]
  ok      github.com/rokath/trice/cmd/stim        0.227s
  ok      github.com/rokath/trice/cmd/trice       0.577s
  ok      github.com/rokath/trice/internal/args   0.232s
  ok      github.com/rokath/trice/internal/charDecoder    0.407s
  ok      github.com/rokath/trice/internal/com    1.148s
  ok      github.com/rokath/trice/internal/decoder        0.412s [no tests to run]
  ?       github.com/rokath/trice/internal/do     [no test files]
  ok      github.com/rokath/trice/internal/dumpDecoder    0.388s
  ok      github.com/rokath/trice/internal/emitter        0.431s
  ok      github.com/rokath/trice/internal/id     0.421s
  ok      github.com/rokath/trice/internal/keybcmd        0.431s
  ok      github.com/rokath/trice/internal/link   0.404s
  ok      github.com/rokath/trice/internal/receiver       0.409s
  ok      github.com/rokath/trice/internal/tleDecoder     0.398s
  ?       github.com/rokath/trice/internal/translator     [no test files]
  ok      github.com/rokath/trice/internal/trexDecoder    0.391s
  ok      github.com/rokath/trice/pkg/cipher      0.377s
  ok      github.com/rokath/trice/pkg/endian      0.302s
  ok      github.com/rokath/trice/pkg/msg 0.299s
  ok      github.com/rokath/trice/pkg/tst 0.406s
  ```

To execute the target code tests, you can run `scripts/testAll.sh` or `cd` into `_test` and run `go test ./...` from there. ATTENTION: These tests run a significant long time (many minutes depending on your machine), because the **Go** - **C** border is crossed very often.
The last tests can last quite a while, depending on your machine.

```bash
ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (main)
$ go install ./cmd/trice/
```

Afterwards you should find an executable `trice` inside $GOPATH/bin/ and you can modify its source code.

After installing Go, in your home folder should exist a folder ./go/bin. Please add it to your path variable. OR: Copy the Trice binaries from there into a folder of your path after creating them with `go install ./cmd/trice/... ./cmd/tlog/...`. There is now a remommended script `./scripts/buildTriceTool.sh`. Using it, depending on your system you may need to enter `bash ./scripts/buildTriceTool.sh`, includes the actual Trice repository state into the Trice binaries, which is shown with `trice version` and `tlog --version` then - useful in case of issues.

## 2. <a id="prepare-a-release"></a>Prepare A Release

Prerequisite: Installed `goreleaser`.

### 2.1. <a id="check-a-goreleaser-release-before-publishing"></a>Check a GoReleaser Release before Publishing

By cloning the Trice repo into an empty folder, you make sure no other files exist in the Trice folder.

```bash
mkdir ./tmp
cd /tmp
git clone https://github.com/rokath/trice.git
cd trice
goreleaser release --clean --snapshot --skip=publish
```

This just generates the artifacts locally in `/tmp/trice/dist` using the [./trice/.goreleaser.yaml](../../.goreleaser.yaml) copy in `./temp/trice`.

Alternatively you can do in your local Trice clone directly, by removing everything not a part of the Trice repo, if you are sure not to loose any data:

```bash
git status
git clean -xfd
goreleaser release --clean --snapshot --skip=publish
```

Explanation:

* `release` – run the normal release pipeline (builds, archives, checksums, changelog, etc.).
* `--clean` – delete the `dist/` folder first so no old artefacts are reused. 
* `--snapshot` – build as a “snapshot” version, not a real tagged release.
* `--skip=publish` – **do not upload** anything (no GitHub Releases, no Homebrew, no Docker, etc.).

What you should see:

* GoReleaser building all your `builds:` targets from `.goreleaser.yaml`, including the `trice` and `tlog` host binaries.
* Creating archives in `dist/`.
* Generating checksums.
* No attempts to call the GitHub API for a real release.

If this **succeeds**, you’ve already tested 90% of what CI will do for a real release.
If it **fails**, fix the problem locally first (missing files, bad paths, etc.) – it would fail the same way in CI.

## 3. <a id="publish-a-release-manually-from-a-selected-tag"></a>Publish a Release Manually from a Selected Tag

The [release workflow](../../.github/workflows/goreleaser.yml) runs only when started manually. Publishing a Git tag makes the selected source snapshot available; the separate workflow start publishes its release assets.

### 3.1. <a id="verify-the-candidate-commit"></a>Verify the Candidate Commit

Review the release notes, commit the intended changes and run the final checks on that fixed state:

```sh
git status
./scripts/testAll.sh full
git status
```

The test summary must pass, including source-state restoration. Check any skips explicitly. This run builds the local snapshot packages, verifies their licenses, and exercises Bind, Context Enrichment and structured output using the unpacked host tools and target sources. Run the [installation workflow](../../.github/workflows/install-checks.yml) on the same candidate to check Linux, macOS and Windows.

### 3.2. <a id="create-and-transfer-the-release-tag"></a>Create and Transfer the Release Tag

Use an annotated tag on the approved commit. For the first v2 beta:

```sh
git tag -a v2.0.0-beta-1 -m "Trice v2.0.0-beta-1"
git push origin v2.0.0-beta-1
```

Transfer the branch containing the updated workflows as well. Keep the tag fixed after publishing so its sources, tool metadata and documentation continue to describe the same release.

### 3.3. <a id="start-the-release-workflow"></a>Start the Release Workflow

In GitHub Actions, open **goreleaser**, choose **Run workflow**, select the branch containing the approved pipeline, and enter `v2.0.0-beta-1` in the required `tag` field.

The workflow checks out `refs/tags/<tag>`, checks its commit, and explicitly passes that tag to GoReleaser. The workflow branch selects the pipeline; the tag selects the sources and release version. An equivalent CLI invocation, using `wip` as the workflow branch, is:

```sh
gh workflow run goreleaser.yml --ref wip -f tag=v2.0.0-beta-1
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
gh workflow run release-audit.yml --ref wip -f tag=v2.0.0-beta-1
```

The audit downloads that release explicitly and tests its archives, packages, target sources and PDFs. The host archive jobs also compile the PC feature tour against the unpacked target library, then verify text, JSON and KV with both shipped tools. Missing artifacts, changed licenses, incorrect fields or non-record JSON stdout fail the audit.

### 3.6. <a id="install-a-beta-or-a-stable-release"></a>Install a Beta or a Stable Release

For a beta, download the matching platform archive from its tagged release and place its tools in `PATH`, or build from a checkout of that tag. Use the matching target sources and manuals.

Homebrew remains the stable-release route. `skip_upload: auto` prevents prerelease tags from updating the normal tap, and beta audits skip the Homebrew job. Before publishing a stable v2 release, review `release.prerelease` and the stable installation checks explicitly.

<p align="right">(<a href="#building-and-releasing-the-trice-tools">back to top</a>)</p>

