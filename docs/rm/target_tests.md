# Testing Trice — Running the Tests

Run `./scripts/testAll.sh` from the repository root for the complete local test suite. It tests the current checkout on the current operating system. Use GitHub's native Windows, Linux and macOS jobs to check the same commit on all three systems.

This chapter covers everyday use, GitHub Actions and release-package checks. [Test Internals](target_test_internals.md) explains the CGO bridge, every PC configuration, cache handling and adding tests.

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [How to run the tests](#how-to-run-the-tests)
  * [Prepare Windows](#prepare-windows)
  * [Prepare Linux](#prepare-linux)
  * [Prepare macOS](#prepare-macos)
  * [Shared tool rules](#shared-tool-rules)
* [What full and quick cover](#what-full-and-quick-cover)
* [Read results and diagnose failures](#read-results-and-diagnose-failures)
* [Run a focused local check](#run-a-focused-local-check)
  * [Inspect Go coverage](#inspect-go-coverage)
  * [CLion review build](#clion-review-build)
* [GitHub Actions](#github-actions)
  * [Native acceptance on all three systems](#native-acceptance-on-all-three-systems)
  * [Other test workflows](#other-test-workflows)
* [GoReleaser and release-package testing](#goreleaser-and-release-package-testing)
  * [Check a local snapshot](#check-a-local-snapshot)
  * [Understand the installation and release audit jobs](#understand-the-installation-and-release-audit-jobs)
  * [Before accepting a release candidate](#before-accepting-a-release-candidate)
* [Documentation history](#documentation-history)

<!-- numbering=false min=2 max=3 slug=github anchor=false link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## How to run the tests

Use Bash on Linux and macOS, and Git Bash on Windows:

```sh
./scripts/testAll.sh
```

The default is **full**. The suite checks installed tools and `temp/tools` first. If a project-local tool needs installation or repair, it asks once during preparation; later runs with valid tools need no answer. Empty input or EOF declines installation. System package changes require their own approval. Some host prerequisites and operating-system SDKs still need to be installed separately. Allow plenty of time: a full run includes hundreds of compiler configurations and may take about an hour, depending on the machine and cache.

Every run lists the declared tool versions before preparation. Installation questions name the requesting tool group or cache. Caches belong to each worktree. Standalone-tool caches use a hash of the complete `scripts/test-tools.env`, so merging a changed manifest can require a new cache even for tools whose versions stayed the same. npm packages use a separate lockfile hash. The next run reuses a completed, valid cache.

| Command | Use |
| --- | --- |
| `./scripts/testAll.sh` | Full native suite; normal final check before sharing a change. |
| `./scripts/testAll.sh quick` | Shorter feedback while editing; still includes real C execution with GCC and Clang. |
| `./scripts/testAll.sh full` | Explicit spelling of the default. |
| `./scripts/testAll.sh quick --stop` | Stop after the first failed step. |
| `./scripts/testAll.sh --no-stop` | Explicitly continue after failures; this is already the default. |

From PowerShell, use `bash ./scripts/testAll.sh` to start the same runner. Direct Go commands also work in PowerShell; the compiler and runtime still need to be available to Go.

Do not run two suites, ID-maintenance scripts or the repository formatter in the same checkout at the same time. The suite holds a checkout lock. Separate Git worktrees can run independently. Ctrl+C stops the active work and waits for source restoration; let that finish before starting another run.

### Prepare Windows

Use Git for Windows with Git Bash, native Go, Make and Python support. The suite can install the declared Node.js version into `temp/tools` after approval. Have `tar`, `unzip`, `curl` and the usual shell utilities available in Git Bash.

The test environment uses a matching **GCC/Clang/MinGW-w64** compiler and runtime set. When the global compilers do not match the declared set, preparation can install the complete project-local bundle under `temp/tools`. An arbitrary `clang.exe` on `PATH` is insufficient: both C and C++ must compile, link and run with matching headers and libraries. See [Windows compiler selection](development_environment_setup.md#choose-the-right-windows-compiler) for diagnosing a mixed installation.

Keep shell scripts in LF form as specified by [.gitattributes](../../.gitattributes). The tests themselves create both LF and CRLF source files, so Git cannot silently normalize away the cases being tested. Native Windows tests also exercise `/` and `\`, PowerShell/cmd argument passing, local TCP, and rollback when a source file is held open with incompatible sharing permissions.

The suite selects CMake's `MinGW Makefiles` generator and the compiler bundle's `mingw32-make`, so Visual Studio and NMake are not required.

### Prepare Linux

Install the basic tools listed above and a working C/C++ development environment, including `xz` support for archive extraction. The suite prepares the declared native GCC and LLVM packages in its project cache and can install the declared Node.js version after approval. System headers and runtime libraries must be present. The [native CI setup](../../.github/workflows/native-tests-reusable.yml) shows the Ubuntu package prerequisites.

Linux checks run as Linux processes on the native filesystem. They include shell signals, case-sensitive filenames, real compiler output, local TCP/UDP and file replacement. WSL or a Linux container supplies Linux evidence even when its host is Windows or macOS.

### Prepare macOS

Install the Xcode Command Line Tools with their SDK, the basic tools listed above, and the declared genuine GCC version. Apple `gcc` is normally another name for Clang and does not count as the GCC compiler family. The suite finds a matching versioned `gcc-N`/`g++-N` pair and creates run-local aliases; it does not replace Apple's tools.

The suite selects pinned upstream LLVM for Clang, Clang++ and clangd, while retaining Apple's SDK. Apple Silicon uses the official binary package. Upstream provides no Intel macOS binary for this release, so Intel Macs build the verified source package locally using the SDK compilers, Make, Python and pinned CMake. This first build can take many minutes or longer and needs several GB of free space. The terminal shows each build phase and a status message every 30 seconds; `tools.log` retains the full compiler output. Later runs reuse the completed compiler in this worktree's cache. A new worktree, a new LLVM version or a deleted cache can require another build. Exact versions live in [test-tools.env](../../scripts/test-tools.env); `tools.log` records the selected SDK and executables. Missing SDKs and system packages are reported before the test steps and are not installed with administrator rights by the suite.

The native filesystem test detects whether the actual volume is case-sensitive. Two spellings of the same source must retain IDs; two distinct case-sensitive files must remain separate. macOS ARM and Intel are separate execution targets. An ARM build or a cross-built Intel archive alone is not proof of an Intel run.

### Shared tool rules

All three systems use [test-tools.env](../../scripts/test-tools.env) for tool versions and archive checksums, and the [npm lockfile](../../scripts/testcheck/package-lock.json) for the existing Node tools. The suite also prepares the complete ARM cross-toolchain, formatters and analyzers. CMake is prepared as a complete pinned package under `temp/tools/bundles`; no system CMake is required. Each worktree has its own cache, so a fresh worktree may need downloads even when another checkout already passes. Network access is needed when a required cache entry is absent. A failed download or incompatible prerequisite makes the run INCOMPLETE.

Use the preparation log's diagnosis rather than installing a random newer tool. Keep `C_INCLUDE_PATH` unset globally: ARM headers must not leak into native CGO builds. Go, Node, system SDKs and basic build utilities remain explicit prerequisites; project-local tool preparation does not make every possible host installation supported.

The October 2026 tool refresh uses LLVM 23.1.4 on Linux/macOS and the latest complete WinLibs runtime with Clang 23.1.3 on Windows. These platform pins deliberately follow available complete packages. Formatting uses the separately published clang-format 23.1.3 wheel. Go, shfmt, jq, the managed Windows Python runtime, Markdown/PDF tools and lychee are also pinned to their verified stable releases; tools already current retain their pins. CI reads the same manifest and npm lockfile.

Clang diagnostics remain enabled. Trice suppresses the intentional `__COUNTER__` extension only during its own counter expansion. The G0B1 example suppresses `unused-but-set-global` only for FreeRTOS `tasks.c`, whose trace counter is updated even when tracing is disabled; vendor sources remain unchanged.

## What full and quick cover

| Check | Quick | Full |
| --- | --- | --- |
| Shell/C/Go/Markdown/manual formatting, ShellCheck, Actionlint, Cppcheck, GoReleaser configuration | Required | Required |
| Build the current tools; probe C/C++ compilers by compiling, linking and executing | Required | Required |
| Go package tests and Go coverage | Required | Required |
| Native CLI paths, Unicode/spaces, LF/CRLF, filesystem aliases and platform-specific checks | Required | Required |
| Bind behavior; CE/SL compiler-to-decoder checks; PC, ABC and firmware feature examples | Required | Required; additional G0B1 Clang build |
| Clang firmware/example matrix | Bind | Insert and Bind |
| GCC firmware/example matrix | Selected Bind examples | TRICE_OFF, Insert and Bind |
| PC/CGO configurations, for **each** of GCC and Clang | Three representative packages, Bind | All 63 packages, Insert and Bind |
| L432 firmware configurations | Not selected | All 101, numbered 0 through 100 |
| External HTTP links and release snapshot packages | Separate checks | Separate checks |

Quick's PC packages are `ringB_de_multi_cobs_ua`, `abc_rx_host` and `abc_tx_host`. Quick still exercises both ID workflows in focused feature/integration checks; it does not run the complete legacy PC Insert matrix. The [runner](../../scripts/_110_test_runner.sh) owns the actual step selection.

| Test layer | What a pass tells you |
| --- | --- |
| Go tests | Host parsing, dictionaries, decoding, command behavior and script orchestration work for the exercised cases. |
| PC/CGO | Real Trice C code emits records that the host decoder understands under the selected configuration. |
| Firmware builds | The selected MCU code compiles and links, including the expected output artifacts. |
| Native OS checks | The exercised file, path, process and network operations work on that runner. |
| Hardware tests | Actual UART/RTT, interrupt timing and board behavior; require the corresponding hardware and are not supplied by a host build. |

## Read results and diagnose failures

Each suite invocation keeps its own `temp/log/testAll.XXXXXX` directory. The console prints the exact path. Earlier logs and recovery directories are retained.

| Result | Exit | Meaning |
| --- | --- | --- |
| PASS | 0 | All required checks for the selected scope ran and passed. |
| FAIL | 1 | A check found a failure. Later successful steps do not erase it. |
| INCOMPLETE | 2 | Preparation or required test evidence is missing. |
| ABORTED | 130 / 143 | Interrupted by INT / TERM; inspect restoration messages before resuming work. |

Start with the first failed step and its log, rather than the last line of a long compiler trace. The displayed time belongs to that step, including its preparation and restoration. The percentage estimates relative test work, not remaining wall-clock time.

| File in the run directory | Contents |
| --- | --- |
| `testAll_summary.log` | Selected scope, step results, duration and final state check. |
| `tools.log` | Tool selection, versions, compiler/runtime probes, SDK and preparation errors. |
| `results.json` | Machine-readable scope, OS/architecture, commit, dirty flag, tool context and step results/log paths. |
| `_NNN_test_*.log` | Detailed output for one test step. |
| `*.go.*` | Original Go JSON events used to check actual execution and skips. |
| `coverage.out` | Go coverage profile belonging to this run. |
| `pc-*` and `clang/pc-*` | Per-configuration PC logs, overlays and retained failure data. |

PASS is checked against Go's test events: an unknown skip or a required test that never ran prevents acceptance. A small [explicit exception list](../../scripts/testcheck/main.go) handles genuine platform differences, such as POSIX signal tests on Windows and PowerShell/cmd tests on other systems. It does not allow a missing compiler to count as success.

The suite restores initial source, TIL, LI and generated-header bytes and verifies the checkout at the end, including relevant new inputs. This also protects pre-existing local edits. Recovery data is retained when restoration cannot complete. A forced process kill or power loss cannot run shell cleanup; inspect the recorded recovery directory in that case.

For a PC mismatch, open the configuration's `output.log`. It identifies the source expectation, expected and actual text, compiler context and reproduction command. A bulk mismatch also retains binary/text output and triggers a line-by-line diagnostic run. Even if that rerun passes, the original bulk failure remains a failure. See [CGO execution and diagnostics](target_test_internals.md#execute-and-compare-records).

## Run a focused local check

Run commands below from the repository root. Direct steps use the same shared helpers, but do not all perform the suite's complete preparation and result audit. Read `SKIP` and `MISSING TOOL` messages; a standalone exit 0 can still mean a check was skipped.

| Command | Checks |
| --- | --- |
| `go test -count=1 ./...` | Ordinary Go packages; `_test` is excluded by Go's directory discovery rule. Optional compiler integration remains opt-in. |
| `TRICE_BIND_INTEGRATION=1 go test -count=1 ./internal/args ./internal/id ./scripts` | Go packages with their native compiler/integration checks enabled. |
| `./scripts/_500_test_bind.sh` | Bind-focused behavior and generated-code checks. |
| `./scripts/_515_test_logging_features.sh full` | CE/SL compiler-to-decoder checks and feature examples. Build the current tool with `_480_test_build_trice_tool.sh` first if needed. |
| `./scripts/_640_test_pc_targets_bind.sh quick` | Managed representative CGO run with Bind. Use `full` for every configuration. |
| `./scripts/_630_test_pc_targets_insert.sh full` | Managed complete CGO run with legacy Insert/Clean. |
| `./scripts/_645_test_pc_targets_clang.sh full` | Complete PC selection with Clang, Insert and Bind. |
| `./scripts/_620_test_l432_configs.sh` | Managed L432 firmware matrix. |
| `./scripts/_530_test_links.sh` | Lychee file/web links and their `#anchor` destinations; needs Lychee. |
| `./scripts/_460_test_goreleaser_config.sh` | Declared GoReleaser configuration contract. |

POSIX-style `NAME=value command` examples also work in Git Bash. In PowerShell set the variable first, for example `$env:TRICE_BIND_INTEGRATION = '1'`, then run the Go command. The full suite enables native integration, CGO and Windows TCP checks itself; a plain `go test` does not provide the same acceptance claim.

| Control | Default | When useful |
| --- | --- | --- |
| `TRICE_PC_TEST_JOBS` | 4 | Limit independent configuration processes; use 1 for serial diagnosis. |
| `TRICE_PC_TEST_MODE` | `auto` | Use `line-by-line` to diagnose an individual expectation path. |
| `TRICE_L432_TEST_JOBS` | Detected CPU budget; fallback 4 | Bound concurrent compiler/linker commands. Each configuration uses `make -j1`. |
| `GOCACHE` | Existing value; otherwise suite-local `.gocache` | Reuse compiled Go/C artifacts. Tests still execute with `-count=1`. |

For example:

```sh
TRICE_PC_TEST_JOBS=1 TRICE_PC_TEST_MODE=line-by-line ./scripts/_640_test_pc_targets_bind.sh full
TRICE_L432_TEST_JOBS=4 ./scripts/_620_test_l432_configs.sh
```

The managed wrappers own ID preparation and restoration. A direct `cd _test && go test ./...` requires matching IDs, Bind includes, metadata and compiler settings and lacks the worker's C-input cache overlay. Prefer the wrappers. The [internals chapter](target_test_internals.md#cgo-build-cache) explains when an explicit `go clean -cache` is needed.

### Inspect Go coverage

For the suite, open its `coverage.out`. To reproduce the standalone [coverage workflow](../../.github/workflows/coverage.yml):

```sh
mkdir -p temp/log
go test -count=1 ./... -covermode=atomic -coverprofile=./temp/log/coverage.out -coverpkg=./...
go tool cover -func=./temp/log/coverage.out
go tool cover -html=./temp/log/coverage.out
```

Go coverage measures Go statements, not execution coverage of the Trice C library. Compare the same package selection and target-branch baseline. Coveralls covered-line percentages and Go's rounded statement percentage are different measures; neither replaces the C configuration matrices.

### CLion review build

With the current `trice` tool, CMake and a native C compiler available:

```sh
cmake -S _test/clion-review -B temp/clion-review
cmake --build temp/clion-review
```

Use `-DTRICE_EXECUTABLE=path/to/trice` if tool selection is needed. This project prepares private sources, metadata and Bind headers in the build directory for indexing and inspection. Step 515 also verifies the real build. It does not replace the CGO matrix or execute an MCU board.

## GitHub Actions

Open the repository's **Actions** tab, choose a workflow and inspect the run's commit, OS/architecture and selection. For a manual run, select the intended branch and inputs. A workflow name alone does not tell you whether its checks were skipped or which commit its artifacts belong to.

### Native acceptance on all three systems

The [native workflow](../../.github/workflows/native-tests.yml) calls the [shared runner workflow](../../.github/workflows/native-tests-reusable.yml). That runner installs prerequisites, reads the central tool versions, restores project caches and invokes the same `testAll.sh` as local development.

| Event | Native systems | Scope |
| --- | --- | --- |
| Pull request | `ubuntu-24.04`, `windows-2025`, `macos-26` (ARM) | Explicit `quick`. |
| Nightly | Same three plus `macos-15-intel` | `full`, plus separate race checks. |
| Manual | Same three; Intel always full | Chosen `full` or `quick`; default full, plus race checks. |

Each platform runs independently; one failure does not cancel the others. Race checks cover `internal/decoder`, `internal/emitter`, `internal/translator` and `internal/vis`. The workflow attempts to upload `temp/log` even after failures, under `native-<runner>-<selection>` artifacts.

Use a successful native job on each required OS at the same commit as platform evidence. Linux containers help isolate Linux packages; they do not exercise Windows file locking or macOS process behavior. This matrix uses actual native runners.

### Other test workflows

These jobs still serve distinct purposes. They are not all replaced by the native suite.

| Workflow | Trigger and role |
| --- | --- |
| [Library Quick](../../.github/workflows/trice_lib_ci.yml) | PRs into `main`: **full** library build/configuration matrix on Linux. Nightly/manual: quick. The historical check name `Heavy CI (quick)` is retained. |
| [Library Full](../../.github/workflows/trice_lib_ci_full.yml) | Nightly/manual complete Linux library build matrix. Both library workflows use [the reusable implementation](../../.github/workflows/trice_lib_reusable.yml), including feature checks. Only their scheduled runs may skip after 24 hours without `main` activity. |
| [Script portability](../../.github/workflows/script-portability.yml) | Push, PR and manual: `go test ./scripts` on native Windows/Linux/macOS, mostly disposable fixtures and tool doubles. No complete firmware matrix. |
| [Coverage](../../.github/workflows/coverage.yml) | PR, push to `main`, monthly and manual: Go coverage artifact and Coveralls upload. |
| [Go](../../.github/workflows/go.yml) | Manual Go build/tests and coverage integration. |
| [Clang-Format](../../.github/workflows/clang-format.yml), [shfmt](../../.github/workflows/shfmt.yml), [ShellCheck](../../.github/workflows/shellcheck.yml), [Super-Linter](../../.github/workflows/superlinter.yml) | Source/document/configuration checks; each YAML defines its event filters. |
| [Links](../../.github/workflows/link-check.yml) | Separate local/external link check, including fragment destinations. Both the local script and CI enable `--include-fragments`. Server availability and rate limits can affect it; inspect the actual failing URL. `GITHUB_TOKEN`/`GH_TOKEN` can reduce GitHub throttling locally. |
| [CodeQL](../../.github/workflows/codeql.yml) | Go/C/C++ security analysis; complements executable tests. |
| [Future tool compatibility](../../.github/workflows/tool-compatibility.yml) | Weekly/manual: current stable Go and latest GoReleaser. A maintenance signal for future upgrades, separate from acceptance with declared versions. |

A push to `wip` alone does not start the library PR matrix; an open PR into `main` does. Other push-triggered checks may still run. GitHub branch protection decides which checks block merging. Adding a workflow does not automatically make its jobs required; select the native platform checks explicitly in repository settings. YAML describes intended execution, not proof that a particular remote run has passed.

## GoReleaser and release-package testing

There are four useful levels. Keep the distinction between checking source, constructing packages, running unpacked packages, and auditing a published release.

| Level | Entry | Evidence |
| --- | --- | --- |
| Configuration | `_460_test_goreleaser_config.sh` | `goreleaser check` accepts [.goreleaser.yaml](../../.goreleaser.yaml) with the declared GoReleaser version. Included in quick/full; does not build packages. |
| Local snapshot | `_610_test_goreleaser_snapshot.sh` | Builds release-shaped artifacts and performs the checks below. Separate from `testAll.sh`. |
| CI installation | [install-checks.yml](../../.github/workflows/install-checks.yml) | PR/manual: builds snapshots and executes unpacked tools natively on Windows, Linux, macOS ARM and macOS Intel; also checks checkout installation. |
| Published release | [release-audit.yml](../../.github/workflows/release-audit.yml) | Manual audit of one explicitly selected published tag, using downloaded assets. |

### Check a local snapshot

Use a disposable checkout of the intended candidate, because snapshot hooks refresh manuals and `--clean` replaces `dist`. A local clone includes committed content, not uncommitted edits. From the repository root, choose a fresh destination:

```sh
git clone --no-hardlinks . temp/release-check
cd temp/release-check
./scripts/_610_test_goreleaser_snapshot.sh
```

Step 610 uses the pinned GoReleaser and runs its snapshot pipeline. It also needs the PDF toolchain: Node/npm, the declared manual tools and a working headless browser with the required system libraries. Its setup phase may need downloads and standalone installation input; prepare those prerequisites before leaving it unattended.

The check verifies archive/package presence, both manual PDFs, complete repository and SEGGER license notices, default configuration headers and compilation of unpacked target sources. On the current host it unpacks the matching tool archive, runs `trice`/`tlog` help and version commands, decodes a known record, then builds and decodes a feature example using the **shipped tools and shipped C sources**. Bind, Context Enrichment and structured output are therefore checked through the package contents.

Read the final log, `temp/log/_610_test_goreleaser_snapshot.log`, and inspect `dist`. This standalone script can skip work when prerequisites are missing; a skip is not release acceptance. A raw `goreleaser release --snapshot --clean --skip=publish` builds packages without the script's additional unpack/compile/runtime checks.

Snapshot builds do not publish a release. Building foreign-platform archives does not execute their contents; the native CI installation jobs provide that next level of evidence.

### Understand the installation and release audit jobs

`install-checks.yml` first verifies checkout installations through both `GOBIN` and `GOPATH/bin`. It builds snapshot assets on Linux, then passes the same archives to native OS jobs. These check both tools, a known decoded record, and the shipped Bind/CE/structured-logging feature example. The macOS aggregate check requires both native architecture jobs. Linux DEB/RPM/APK installation jobs check their respective package formats separately.

`release-audit.yml` downloads the assets of the exact supplied tag and checks their versions against it. It audits Windows/Linux/macOS archives, Linux DEB/RPM/APK installation, target-source compilation and manual PDF presence/size. Homebrew is checked for stable releases; that route is intentionally skipped for prereleases. The current published-release audit selects the macOS AMD64 archive; the two-architecture native package check belongs to `install-checks.yml`.

The [publishing workflow](../../.github/workflows/goreleaser.yml) is a separate manual operation. It checks out the selected tag, validates notes, publishes and checks generated artifacts. A successful snapshot does not prove publication credentials, package-manager availability or successful remote installation. See [Building and Releasing](tool_build_release.md#publish-a-release-manually-from-a-selected-tag) for the publication procedure.

### Before accepting a release candidate

Use one fixed candidate commit for the native full jobs on all required systems, the complete compiler matrices, external links, snapshot/installation checks and the release notes. Inspect retained artifacts and any skipped jobs. After publication, audit the exact release tag. Keep hardware checks separate and name the board/setup when recording their result.

## Documentation history

This chapter and [Test Internals](target_test_internals.md) replace the former target-test chapter, scattered test instructions and the native-test design draft. Their original texts are preserved in [the test documentation archive](../_scratchPad/obsolete/testing/README.md). Historical transcripts there describe old runs, not current acceptance or performance targets.
