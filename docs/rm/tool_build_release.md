# Building and Releasing the Trice Tools

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Build Trice tool from Go sources](#build-trice-tool-from-go-sources)
* [2. Prepare A Release](#prepare-a-release)
  * [2.1. Check a GoReleaser Release before Publishing](#check-a-goreleaser-release-before-publishing)
* [3. Trigger a real Trice release via CI (with git tag)](#trigger-a-real-trice-release-via-ci-with-git-tag)
  * [3.1. Make sure your workflow reacts to tags](#make-sure-your-workflow-reacts-to-tags)
  * [3.2. Final checks before tagging](#final-checks-before-tagging)
  * [3.3. Choose a version and create a git tag](#choose-a-version-and-create-a-git-tag)
  * [3.4. Push the tag to GitHub (this triggers CI)](#push-the-tag-to-github-this-triggers-ci)
  * [3.5. Watch the CI release run on GitHub](#watch-the-ci-release-run-on-github)
  * [3.6. Check the GitHub Release](#check-the-github-release)

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

## 3. <a id="trigger-a-real-trice-release-via-ci-with-git-tag"></a>Trigger a **real** Trice release via CI (with `git tag`)

Letting CI build and publish an **official release**.

### 3.1. <a id="make-sure-your-workflow-reacts-to-tags"></a>Make sure your workflow reacts to tags

In [.github/workflows/goreleaser.yml](../../.github/workflows/goreleaser.yml), you need `on:   workflow_dispatch:   push:     tags:       - 'v*'`.

* `workflow_dispatch` = you can still run it manually from the Actions tab.
* `push -> tags: 'v*'` = whenever you push a tag like `v0.44.0`, this workflow will start automatically.

Commit & push this change (if you haven’t already):

```sh
git add .github/workflows/goreleaser.yml
git commit -m "Configure GoReleaser workflow to run on tags"
git push origin main
```

### 3.2. <a id="final-checks-before-tagging"></a>Final checks before tagging

In your local `trice` repo:

* Update to latest main:
  * `git checkout main`
  * `git pull origin main`
* Run your tests: `go test ./...` or, for the full compiler matrix, `./scripts/testAll.sh full`.
* Optionally run the snapshot dry run again: `goreleaser release --clean --snapshot --skip=publish`.
    
If all of that is green, you’re ready to “bless” a version.

### 3.3. <a id="choose-a-version-and-create-a-git-tag"></a>Choose a version and create a `git tag`

Decide on a version, for example:

* `v0.44.0`
* `v1.0.0`  
    (Important: GoReleaser expects **SemVer-style tags** like `vX.Y.Z`.)

Create an **annotated tag**:

`git tag -a v0.44.0 -m "Trice v0.44.0"`

Check your tags:

`git tag`

You should see `v0.44.0` in the list.

> 💡 The **tag** is what GoReleaser uses as the release version (`.Tag`, `.Version`, etc.) in your `.goreleaser.yaml`.  
> Your `ldflags` like `-X main.version={{ .Version }}` will use this.

### 3.4. <a id="push-the-tag-to-github-this-triggers-ci"></a>Push the tag to GitHub (this triggers CI)

Now push the tag:

`git push origin v0.44.0`

This does **not** push all tags, only `v0.44.0`.

Because of your workflow’s `on: push: tags: 'v*'`, this **automatically starts** the GoReleaser workflow in GitHub Actions.

### 3.5. <a id="watch-the-ci-release-run-on-github"></a>Watch the CI release run on GitHub

1.  Open your browser and go to your repo:
    
    `https://github.com/rokath/trice`
    
2.  Click the **“Actions”** tab at the top.
    
3.  In the list of workflows, click on **“goreleaser”**.
    
4.  You should see a new run with something like:
    
* Event: `push`
* Ref: `refs/tags/v0.44.0`
* Status: in progress → green (hopefully)
        
5.  Click on that run, then on the job (e.g. `goreleaser`):
    
* Step “Check out repository”
* Step “Set up Go”
* Step “Run GoReleaser”

If “Run GoReleaser” is green ✔, the CI release has succeeded.

### 3.6. <a id="check-the-github-release"></a>Check the GitHub Release

Finally, verify the published release:

1.  In your repo, click the **“Releases”** section (right side or under “Code”).
    
2.  You should see a new release **`v0.44.0`** created by GoReleaser.
    
3.  Inside it you’ll find:
    
* the generated archives (tar.gz/zip), 
* checksums,
* etc., as defined in your `.goreleaser.yaml`.
        
This is now your **official Trice release built by CI**.

<p align="right">(<a href="#building-and-releasing-the-trice-tools">back to top</a>)</p>

