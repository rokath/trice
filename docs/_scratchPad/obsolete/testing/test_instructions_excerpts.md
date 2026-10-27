# Retired test instructions

> Replaced by [Running the Tests](../../../rm/target_tests.md) and [Test Internals](../../../rm/target_test_internals.md). These excerpts preserve the removed instructions exactly; commands and links below describe the former documentation and may be outdated.

## Maintenance: workflows, coverage and links

## 2. <a id="github-workflows"></a>GitHub workflows

The workflow files are the authority for current triggers. Local checks use the same repository scripts where possible; [Testing Trice](target_tests.md) explains test scope and state restoration.

| Workflow | Purpose |
| --- | --- |
| [trice_lib_ci.yml](../../.github/workflows/trice_lib_ci.yml) | **Full** library matrix for PRs into `main`; quick nightly/manual checks. The existing check name remains “Heavy CI (quick)” even for a full PR run. |
| [trice_lib_ci_full.yml](../../.github/workflows/trice_lib_ci_full.yml) | Full nightly/manual library matrix. |
| [trice_lib_reusable.yml](../../.github/workflows/trice_lib_reusable.yml) | Shared library build/test implementation. |
| [go.yml](../../.github/workflows/go.yml) | Manually triggered Go build, tests and coverage upload. |
| [coverage.yml](../../.github/workflows/coverage.yml) | Go coverage on PRs, pushes to `main`, monthly and manual runs; upload to Coveralls. |
| [codeql.yml](../../.github/workflows/codeql.yml) | Static analysis; separate from compiling and executing tests. |
| [clang-format.yml](../../.github/workflows/clang-format.yml) | Check C formatting without rewriting sources. |
| [shfmt.yml](../../.github/workflows/shfmt.yml), [shellcheck.yml](../../.github/workflows/shellcheck.yml) | Shell formatting and shell diagnostics. |
| [superlinter.yml](../../.github/workflows/superlinter.yml) | YAML and Markdown linting. |
| [link-check.yml](../../.github/workflows/link-check.yml) | Links, including source-file links handled by the local wrapper. |
| [install-checks.yml](../../.github/workflows/install-checks.yml) | Build and exercise checkout installations and release-shaped packages. |
| [goreleaser.yml](../../.github/workflows/goreleaser.yml), [release-audit.yml](../../.github/workflows/release-audit.yml) | Publish/audit release artifacts; see [Building and Releasing](tool_build_release.md). |
| [pages.yml](../../.github/workflows/pages.yml) | Build and deploy the documentation website and the statistics reports described below. |
| [label.yml](../../.github/workflows/label.yml), [stale.yml](../../.github/workflows/stale.yml) | Repository labeling and inactive-issue handling. |
| [statistics.yml](../../.github/workflows/statistics.yml) | Repository activity reports; not a Trice logging feature. |

A push to `wip` alone does not trigger the library matrix. Once a PR into `main` exists, a new commit updates that PR and starts the full PR checks again. The nightly activity guard applies only to scheduled runs, not PRs or explicit manual runs. Branch-protection settings determine whether a failed check blocks merging.

### 2.1. <a id="coverage"></a>Coverage

Use the same package selection as the coverage workflow:

```sh
mkdir -p temp/log
go test ./... -covermode=atomic -coverprofile=./temp/log/coverage.out -coverpkg=./...
go tool cover -func=./temp/log/coverage.out
go tool cover -html=./temp/log/coverage.out
```

Go coverage measures Go code, not the C target library. The CGO and firmware matrices provide separate evidence for C configurations. Compare coverage with the target branch; a rounded statement percentage is not identical to Coveralls covered-line coverage.

### 2.2. <a id="link-checks"></a>Link checks

Run `./scripts/_530_test_links.sh`. It uses [lychee.toml](../../lychee.toml), checks source references as well as documentation and writes details to `temp/log/_530_test_links.log`. It maps the CLI-help source URL to the local checkout. Missing Lychee is reported as a skip. `GITHUB_TOKEN` or `GH_TOKEN` can reduce GitHub API throttling.


## Tool build: historical test transcript and local checks

  * Historical test transcript; package names and results describe the recorded checkout. Use the current test entry points below:

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

Use `./scripts/testAll.sh` for the standard Bind checks. For target-code tests only, use `./scripts/_640_test_pc_targets_bind.sh quick` or `full`; `./scripts/_630_test_pc_targets_insert.sh` provides the legacy Insert/Clean variant. These wrappers prepare IDs and restore the starting source and metadata state. Direct `go test` calls inside `_test` require a matching prepared state; see [target-test instructions](target_tests.md#how-to-run-the-tests). The full matrix can take many minutes.


## Tool build: local release snapshot

## 2. <a id="prepare-a-release"></a>Prepare A Release

Prerequisite: Installed `goreleaser`.

### 2.1. <a id="check-a-goreleaser-release-before-publishing"></a>Check a GoReleaser Release before Publishing

From the repository root, clone into a new disposable directory. The clone command stops if `temp/release-check` already contains files; choose another empty directory for a later run.

```bash
mkdir -p temp &&
git clone https://github.com/rokath/trice.git temp/release-check &&
cd temp/release-check &&
goreleaser release --clean --snapshot --skip=publish
```

This generates artifacts locally in `temp/release-check/dist`, using the clone's [.goreleaser.yaml](../../.goreleaser.yaml). Check out the intended candidate in that clone before building if it differs from the default branch.

Alternatively, clean an existing disposable clone first. `git clean -xfd` deletes all untracked and ignored files, including local configuration and build data. Preview with `git clean -xfdn` and continue only when that list can be deleted:

```bash
git status
git clean -xfdn
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

A successful snapshot checks local builds and packaging. It does not check publication credentials, remote uploads or installation on other hosts; those are covered by the release and audit workflows below.
If it fails, fix the reported local problem before proceeding.


## Tool build: former candidate acceptance claim

The test summary must pass, including source-state restoration. Check any skips explicitly. This run builds the local snapshot packages, verifies their licenses, and exercises Bind, Context Enrichment and structured output using the unpacked host tools and target sources. Run the [installation workflow](../../.github/workflows/install-checks.yml) on the same candidate to check Linux, macOS and Windows.

## Contribution test instructions

## Tests

Use `github.com/stretchr/testify/assert` for new assertion-style Go tests to keep the test style consistent across the repository.

Run the regular repository checks with:

```sh
./scripts/testAll.sh
```

Use `./scripts/testAll.sh full` before a final release-level change that
affects target C code or compiler configurations. The regular suite already
includes Go tests and coverage. Its logs and coverage profile are written below
`temp/log/`; no browser-based test viewer or additional Go test framework is
required.


## ID migration: former test default

- **Repository automation:** helper/test scripts use numbered filenames, for example `scripts/_530_test_links.sh` replaces `_testAll_06_LinkCheck.sh`. Update hard-coded helper and documentation paths. `./scripts/testAll.sh` defaults to the quick Bind workflow; use `full` for broad Insert/Clean coverage and `--stop` for early exit. See [target tests](target_tests.md).


## Development environment: historical Debian test transcript

* Logout, login and compile Trice:

    ```bash
    th@P51-DebianKDE:~/repos$ go version
    go version go1.23.4 linux/amd64
    th@P51-DebianKDE:~/repos$ cd trice
    th@P51-DebianKDE:~/repos/trice$ go install ./cmd/...
    go: downloading github.com/spf13/afero v1.9.5
    go: downloading github.com/kr/pretty v0.1.0
    go: downloading go.bug.st/serial v1.6.0
    go: downloading github.com/mgutz/ansi v0.0.0-20200706080929-d51e80ef957d
    go: downloading github.com/rokath/cobs v0.0.0-20230425030040-4ebbe9b903b9
    go: downloading github.com/rokath/tcobs v0.9.1
    go: downloading golang.org/x/crypto v0.31.0
    go: downloading github.com/fsnotify/fsnotify v1.6.0
    go: downloading github.com/pkg/errors v0.9.1
    go: downloading golang.org/x/sys v0.28.0
    go: downloading golang.org/x/text v0.21.0
    go: downloading github.com/kr/text v0.1.0
    go: downloading github.com/mattn/go-colorable v0.1.13
    go: downloading github.com/creack/goselect v0.1.2
    go: downloading github.com/mattn/go-isatty v0.0.19
    th@P51-DebianKDE:~/repos/trice$ trice version
    version=devel, built 2025-01-04 16:29:30.51921408 +0100 CET
    th@P51-DebianKDE:~/repos/trice$ go test ./...
    go: downloading github.com/tj/assert v0.0.3
    go: downloading github.com/stretchr/testify v1.8.4
    go: downloading github.com/udhos/equalfile v0.3.0
    go: downloading github.com/pmezard/go-difflib v1.0.0
    go: downloading gopkg.in/yaml.v3 v3.0.1
    go: downloading github.com/davecgh/go-spew v1.1.1
    ?       github.com/rokath/trice/internal/do     [no test files]
    ?       github.com/rokath/trice/internal/translator     [no test files]
    ?       github.com/rokath/trice/pkg/ant [no test files]
    ok      github.com/rokath/trice/cmd/trice       1.014s
    ok      github.com/rokath/trice/internal/args   0.009s
    ok      github.com/rokath/trice/internal/charDecoder    0.005s
    ok      github.com/rokath/trice/internal/com    0.005s
    ok      github.com/rokath/trice/internal/decoder        0.005s
    ok      github.com/rokath/trice/internal/dumpDecoder    0.006s
    ok      github.com/rokath/trice/internal/emitter        0.006s
    ok      github.com/rokath/trice/internal/id     2.744s
    ok      github.com/rokath/trice/internal/keybcmd        0.006s
    ok      github.com/rokath/trice/internal/link   0.006s
    ok      github.com/rokath/trice/internal/receiver       0.007s
    ok      github.com/rokath/trice/internal/trexDecoder    0.008s
    ok      github.com/rokath/trice/pkg/cipher      0.006s
    ok      github.com/rokath/trice/pkg/endian      0.002s
    ok      github.com/rokath/trice/pkg/msg 0.005s
    ok      github.com/rokath/trice/pkg/tst 0.003s
    th@P51-DebianKDE:~/repos/trice$ 
    th@P51-DebianKDE:~/repos/trice$ gcc --version
    gcc (Debian 12.2.0-14) 12.2.0
    Copyright (C) 2022 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    th@P51-DebianKDE:~/repos/trice$ ./scripts/testAll.sh 
    Sa 4. Jan 16:33:57 CET 2025
    This can take several minutes ...
    ?       github.com/rokath/trice/internal/do     [no test files]
    ok      github.com/rokath/trice/cmd/trice       1.013s
    ok      github.com/rokath/trice/internal/args   0.007s
    ok      github.com/rokath/trice/internal/charDecoder    0.004s
    ok      github.com/rokath/trice/internal/com    0.004s
    ok      github.com/rokath/trice/internal/decoder        0.003s
    ok      github.com/rokath/trice/internal/dumpDecoder    0.004s
    ok      github.com/rokath/trice/internal/emitter        0.003s
    ?       github.com/rokath/trice/internal/translator     [no test files]
    ?       github.com/rokath/trice/pkg/ant [no test files]
    ok      github.com/rokath/trice/internal/id     2.742s
    ok      github.com/rokath/trice/internal/keybcmd        0.004s
    ok      github.com/rokath/trice/internal/link   0.004s
    ok      github.com/rokath/trice/internal/receiver       0.005s
    ok      github.com/rokath/trice/internal/trexDecoder    0.005s
    ok      github.com/rokath/trice/pkg/cipher      0.004s
    ok      github.com/rokath/trice/pkg/endian      0.002s
    ok      github.com/rokath/trice/pkg/msg 0.004s
    ok      github.com/rokath/trice/pkg/tst 0.004s
    ok      github.com/rokath/trice/_test/be_dblB_de_tcobs_ua       144.000s
    ok      github.com/rokath/trice/_test/be_staticB_di_xtea_cobs_rtt32     143.927s
    ok      github.com/rokath/trice/_test/dblB_de_cobs_ua   144.006s
    ok      github.com/rokath/trice/_test/dblB_de_multi_cobs_ua     143.824s
    ok      github.com/rokath/trice/_test/dblB_de_multi_nopf_ua     144.087s
    ok      github.com/rokath/trice/_test/dblB_de_multi_tcobs_ua    144.100s
    ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_cobs_ua        143.928s
    ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_tcobs_ua       144.034s
    ok      github.com/rokath/trice/_test/dblB_de_nopf_ua   142.399s
    ok      github.com/rokath/trice/_test/dblB_de_tcobs_ua  142.520s
    ok      github.com/rokath/trice/_test/dblB_de_xtea_cobs_ua      142.526s
    ok      github.com/rokath/trice/_test/dblB_de_xtea_tcobs_ua     142.246s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_cobs_ua    281.643s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_multi_cobs_ua      281.201s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_multi_tcobs_ua     281.518s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_tcobs_ua   281.536s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_xtea_cobs_ua       279.814s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_cobs_ua     280.173s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_multi_cobs_ua       280.095s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_multi_tcobs_ua      279.693s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_tcobs_ua    291.037s
    ok      github.com/rokath/trice/_test/ringB_de_cobs_ua  140.861s
    ok      github.com/rokath/trice/_test/ringB_de_multi_tcobs_ua   140.802s
    ok      github.com/rokath/trice/_test/ringB_de_multi_xtea_cobs_ua       141.037s
    ok      github.com/rokath/trice/_test/ringB_de_multi_xtea_tcobs_ua      149.046s
    ok      github.com/rokath/trice/_test/ringB_de_nopf_ua  149.114s
    ok      github.com/rokath/trice/_test/ringB_de_tcobs_ua 149.121s
    ok      github.com/rokath/trice/_test/ringB_de_xtea_cobs_ua     149.089s
    ok      github.com/rokath/trice/_test/ringB_de_xtea_tcobs_ua    149.164s
    ok      github.com/rokath/trice/_test/ringB_di_cobs_rtt32__de_tcobs_ua  288.603s
    ok      github.com/rokath/trice/_test/ringB_di_cobs_rtt8__de_tcobs_ua   288.569s
    ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt32__de_tcobs_ua  278.409s
    ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt32__de_xtea_cobs_ua      278.493s
    ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt8__de_tcobs_ua   278.301s
    ok      github.com/rokath/trice/_test/ringB_di_tcobs_rtt32__de_tcobs_ua 278.462s
    ok      github.com/rokath/trice/_test/ringB_di_xtea_cobs_rtt32__de_xtea_cobs_ua 278.590s
    ok      github.com/rokath/trice/_test/special_for_debug 0.129s
    ok      github.com/rokath/trice/_test/special_protect_dblB_de_tcobs_ua  0.130s
    ok      github.com/rokath/trice/_test/stackB_di_nopf_aux32      139.984s
    ok      github.com/rokath/trice/_test/stackB_di_nopf_aux8       139.358s
    ok      github.com/rokath/trice/_test/stackB_di_nopf_rtt32      139.366s
    ok      github.com/rokath/trice/_test/stackB_di_nopf_rtt8       143.107s
    ok      github.com/rokath/trice/_test/stackB_di_xtea_cobs_rtt8  141.896s
    ok      github.com/rokath/trice/_test/staticB_di_nopf_aux32     141.398s
    ok      github.com/rokath/trice/_test/staticB_di_nopf_aux8      141.677s
    ok      github.com/rokath/trice/_test/staticB_di_nopf_rtt32     141.779s
    ok      github.com/rokath/trice/_test/staticB_di_nopf_rtt8      141.609s
    ok      github.com/rokath/trice/_test/staticB_di_tcobs_rtt32    141.487s
    ok      github.com/rokath/trice/_test/staticB_di_tcobs_rtt8     141.559s
    ok      github.com/rokath/trice/_test/staticB_di_xtea_cobs_rtt32        139.048s
    Script run 1300 seconds.
    th@P51-DebianKDE:~/repos/trice$ 
    ```


## Context Enrichment: former focused command

Repeat the focused acceptance tests from the repository root:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id ./internal/args -run '^(TestBindContext|TestContextEnrichment|TestInsertCleanContext|TestSourceContext|TestContextInsertClean)' -count=1
```



