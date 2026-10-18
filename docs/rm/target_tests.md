# Testing Trice

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. What the tests prove](#what-the-tests-prove)
* [2. General info](#general-info)
* [3. How to run the tests](#how-to-run-the-tests)
  * [3.1. CLion review build](#clion-review-build)
* [4. Tests Details](#tests-details)
* [5. How to add new test cases](#how-to-add-new-test-cases)
* [6. Test Internals](#test-internals)
  * [6.1. Comparing test duration between computers](#comparing-test-duration-between-computers)
* [7. Test Results](#test-results)
* [8. Test Cases](#test-cases)
  * [8.1. Folder Naming Convention](#folder-naming-convention)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## 1. <a id="what-the-tests-prove"></a>What the tests prove

CGO is Go's interface to compiled C. These tests compile the real Trice C library with a host C compiler, call it from Go and pass the resulting bytes through the real host decoder.

```text
triceCheck.c + triceConfig.h + Trice library
                    │ host C compiler / CGO
                    ▼
            real binary records
                    │ host logger + matching TIL
                    ▼
             decoded log text
                    │ compare with //exp: on each test site
                    ▼
              PASS or mismatch
```

| Layer | Evidence | Boundary |
| --- | --- | --- |
| Go unit tests | Parser, dictionary, decoder and host behavior. | Not target timing or C coverage. |
| PC/CGO matrix | Actual C records decoded under many configurations. | Host execution, not MCU peripheral behavior. |
| Firmware build matrix | Code compiles/links for target configurations. | Does not flash or execute a board. |
| Hardware checks | Actual transport, interrupt behavior and timing. | Require the corresponding board and setup. |

## 2. <a id="general-info"></a>General info

The `_test` directory reduces VS Code indexing work and excludes the slow C matrices from `go test ./...`. Run `./scripts/testAll.sh` to include them.

The matrices compile and run target code under different compiler switches, automating configuration checks.

`scripts/testAll.sh quick` performs the standard Bind compiler matrices and focused CE/SL checks for both Bind and Insert/Clean. `scripts/testAll.sh full` also runs the complete legacy Insert/Clean and extended compiler matrices; its duration depends strongly on the host. The runner orders short checks before long matrices and shows a hardware-independent percentage of expected relative test work. On an interactive terminal, a spinner changes in place every few seconds during a long step; it does not add repeated log lines or claim a time-based ETA.

Each result line shows the elapsed time for that script before its name, with right-aligned minutes and seconds: `[24/26 | ~ 76.7%] (  4m 30s) _620_test_l432_configs.sh: PASS`. The format is `(%3dm%3ds)`; four hours appear as `(240m  0s)`. This measures elapsed time including any preparation and restoration performed by the script, not accumulated CPU time or time since the entire suite started. The same column appears for `WARN`, `FAIL` and `ABORTED`, and is saved in `temp/log/testAll_summary.log`. The interactive spinner updates the current script's elapsed time in place.

Both selections include [step 515](../../scripts/_515_test_logging_features.sh), which runs the existing compiler-to-decoder CE/SL checks and the feature examples:

Check | Evidence
--- | ---
`TestContextEnrichmentTargetToDecoder` and `TestContextInsertCleanTargetToDecoder` | Real C/C++ records, text/JSON/KV output, structured fields, stamps, reversible Insert/Clean, disabled logging and exactly-once argument evaluation.
`TestContextEnrichmentPoC` and `TestContextEnrichmentPoCRebaseScopeBoundary` | Retained proofs for direct callsites and the scope limitation of the existing Rebase dispatcher.
[PC feature example](../../examples/PC_features/check_output.sh) | Runtime string, fields, stamps/deltas, CE, tag filtering and KV output from the built example.
[G0B1 feature example](../../examples/G0B1_features/check_build.sh) | Firmware build, task-context adapter, runtime-string and structured-field metadata, nonempty ELF/HEX/BIN artifacts. This does not execute the firmware on an MCU.

The compiler/decoder checks require Go, Clang, Clang++ and clangd. The examples require the current repository's `trice` tool, Git and tar; the PC example needs `cc` or `gcc`, and G0B1 needs Make and ARM GNU GCC/objcopy/size with its bare-metal libraries. In `quick`, an unavailable group is explicitly skipped and the runner shows `WARN`. In `full`, missing tools make this step fail. A selected Go test that reports `SKIP`, or fails to report its named `PASS`, is also an error. This prevents an empty test selection from appearing to validate the features. The shared Library CI workflow invokes this same step in `full` mode.

The example checks run in a fresh copy under `temp/log/logging-features.*`. It contains the current bytes of tracked sources, including uncommitted source edits, and the same relative layout as the checkout. Existing captures, generated directories and object files are not reused or modified. Successful copies are removed; failed copies remain for inspection. Details are written to `temp/log/_515_test_logging_features.log`. Run just this acceptance step from the repository root with:

```bash
./scripts/_480_test_build_trice_tool.sh
./scripts/_515_test_logging_features.sh full
```

The larger experimental CE Rebase/Wrapper proof remains a separate, explicit investigation. It does not establish productive support for those constructs. To rerun it with the locally available compiler variants:

```bash
TRICE_BIND_INTEGRATION=1 go test ./internal/id -run '^TestContextEnrichmentRebasePoC$' -count=1 -v
```

It reports unavailable compiler variants and optional clangd evidence; missing variants are not platform acceptance. Ordinary Go unit/coverage runs continue to cover SL parser/renderer behavior; step 515 selects only the additional compiler checks instead of repeating those suites.

The PC matrix uses four independent configuration processes by default. Set `TRICE_PC_TEST_JOBS=1` for serial execution or choose another positive limit. ID preparation and source restoration remain sequential. A failing configuration stops at its first mismatch. `testAll.sh` continues with the remaining configurations and test steps by default (`--no-stop`), but the final result remains `FAIL` if any check failed. Use `--stop` to stop after the first failure; cancellation always stops the run. Failure summaries include source references, expected/actual output and the detailed log path.

For example, from the repository root:

```bash
TRICE_PC_TEST_JOBS=4 ./scripts/_640_test_pc_targets_bind.sh full
TRICE_PC_TEST_JOBS=1 TRICE_PC_TEST_MODE=line-by-line ./scripts/_630_test_pc_targets_insert.sh full
```

The second command explicitly selects the diagnostic single-expectation path. Normal runs use `TRICE_PC_TEST_MODE=auto`: bulk where packet boundaries are preserved, single-expectation decoding for unframed or special configurations. All expectations remain enabled.

The L432 matrix builds all 101 configurations (`CONFIGURATION=0` through `100`). It prepares the shared Bind state once and builds configurations concurrently, each with `make -j1`. The default concurrency follows the online CPU count; on Windows it uses the bounded budget from the shared build setup, which prefers physical cores. If detection fails, the matrix uses four jobs. Set `TRICE_L432_TEST_JOBS=1` for serial execution or choose another positive limit; this limits the total number of simultaneous compiler/linker commands, even when `MAKE_JOBS` normally requests unlimited parallelism. For example, to limit a run to four jobs from the repository root:

```bash
TRICE_L432_TEST_JOBS=4 ./scripts/_620_test_l432_configs.sh
```

Every invocation uses fresh, separate object directories for every configuration. All code-generation options, source files and ELF/HEX/BIN targets remain enabled; there is no reuse of potentially stale objects after a header, configuration, workflow or compiler change. The matrix skips the expensive assembler `.lst` text listings (`GCC_LISTINGS=0`); compiler warnings and errors remain enabled. Ordinary `build.sh CONFIGURATION=N` builds still generate listings for manual inspection. Existing `examples/L432_inst/out.gcc` builds remain untouched by the matrix. Full compiler logs stay in `temp/log/l432.*/config-N.log`. Successful temporary build outputs are removed to save disk space; failed or interrupted outputs are retained beside their logs.

The matrix reports each configuration's result, prints compiler error excerpts and gives the command to reproduce a failure. Under `testAll.sh`, the selected `--no-stop` or `--stop` policy applies. A directly invoked L432 matrix stops after a failed batch by default; use `TRICE_TEST_NO_STOP=1` to finish the remaining configurations. Already started jobs finish before source restoration. Cancellation terminates the compiler processes too and prevents further configurations from starting. The managed wrapper above restores the initial source and metadata state on success, failure and cancellation; a direct `examples/L432_inst/all_configs_build.sh` invocation performs the same Bind preparation as `build.sh` and leaves sources in Bind state.

* Partial tests:
  * In `./examples` you can build the target examples with `./buildAllTargets_TRICE_ON.sh` or `./buildAllTargets_TRICE_OFF.sh`.
  * In `./examples/L432_inst` the script `all_configs_build.sh` translates many different configurations.

For the user it could be helpful to start with a `triceConfig.h`file from here and to adapt the Trice tool command line from the matching `cgo_test.go` if no close match in the `examples` folder was found.

## 3. <a id="how-to-run-the-tests"></a>How to run the tests

* CGO requires a working host C compiler and runtime. Follow the [Windows compiler selection and runtime checks](development_environment_setup.md#choose-the-right-windows-compiler), including checks for visible Clang installations. On every platform, keep `C_INCLUDE_PATH` unset globally so ARM headers do not leak into host and CGO builds.
* For direct `go test` calls outside the managed PC worker, execute `go clean -cache` from the repository root after editing externally included C files if CGO appears to reuse precompiled files. The managed PC workflows described below handle these repository inputs automatically.
* Normal tests use an overlay for the shared CGO test files. For ordinary Bind maintenance use `./trice_bindIDs_in_examples_and_test_folder.sh`; it retains ID history. `./scripts/_330_renew_ids_and_refresh_tests.sh` is deliberate legacy maintenance: it runs Clean/Insert/Clean, copies the canonical CGO harness files and cleans example build outputs. Without `keepHistory` it discards the shared TIL/LI history and reseeds IDs. Do not run either maintenance command alongside tests.
* To run direct Go tests from the repository root, use a repo-local Go cache if needed: `GOCACHE="$PWD/.gocache" go test ./...` on POSIX shells, or `$env:GOCACHE = "$PWD/.gocache"; go test ./...` in PowerShell. The `.gocache/` folder is ignored by Git.
* Start with `./scripts/testAll.sh` from the repository root: the default checks use Bind. `./scripts/testAll.sh full` also runs Legacy-Insert/Clean and the full compiler matrices.
* For target-code tests only, use `./scripts/_640_test_pc_targets_bind.sh quick` (representative configurations) or `./scripts/_640_test_pc_targets_bind.sh full` (all configurations). The corresponding legacy entry is `./scripts/_630_test_pc_targets_insert.sh quick` or `full`.
* These managed wrappers own ID preparation and restore the initial sources, TIL, LI and generated-header state after success, failure or cancellation. Do not run competing preparation scripts while they are active.
* Direct `cd _test && go test ./...` calls need preparation first. For Bind, run `./trice_bindIDs_in_examples_and_test_folder.sh` from the root and provide its generated include directory to CGO, as the [PC worker](../../scripts/_160_pc_target_test_worker.sh) does. For Legacy-Insert/Clean, use the corresponding managed wrapper; Insert skips Bind-owned files, so simply running Insert over a bound checkout is insufficient. Direct calls do not provide automatic state restoration or the worker's C-input cache overlay.
* `scripts/testAll.sh` creates its local helper artifacts inside the ignored `./temp/log` folder. The versioned `demoTIL.json` and `demoLI.json` files in the repository root stay available as example reference files. When `GOCACHE` is unset, `scripts/testAll.sh` also uses the ignored repo-local cache folder `./.gocache`.
* A script name beginning with `_` marks an internal helper or an individually runnable test step. Its three-digit prefix groups the flat `scripts` folder: `100`--`250` are shared workflow helpers, `260`--`330` are maintenance helpers, and `400`--`640` are tests ordered broadly from short checks to long compiler matrices.
* It is possible to start the tests individually, but for some the default `-timeout 30s` maybe too short.

### 3.1. <a id="clion-review-build"></a>CLion review build

The [CMake review project](../../_test/clion-review/CMakeLists.txt) builds a host executable for indexing and inspections. With `trice`, CMake and a host C compiler available:

```sh
cmake -S _test/clion-review -B temp/clion-review
cmake --build temp/clion-review
```

If needed, select the tool with `-DTRICE_EXECUTABLE=path/to/trice`. CMake copies the shared test source and TIL into the build directory, prepares private Bind headers, and refreshes them before compilation. The canonical source and shared dictionaries remain unchanged. This is a host review build, not a board test or a replacement for the CGO matrix.

Step `_515` checks the real CLion build and all nine ABC nodes with both ID workflows in isolated checkout paths containing spaces. To run just this check, use `TRICE_BIND_INTEGRATION=1 go test ./scripts -run '^TestBindEntrypointIntegration$' -count=1`. Ordinary script tests use tool doubles; the GCC example matrix additionally exercises the two small F030 entry points and the G0B1 Clang entry. Step `_515` builds the G0B1 feature tour through its public GCC entry, and also through its Clang entry in `full` mode.

## 4. <a id="tests-details"></a>Tests Details

The `_test` tree contains independent configuration packages and shared `testdata`. A folder beginning with `_` is omitted from recursive Go package discovery. Use the managed wrappers for normal selection rather than renaming test folders during a run.

[TriceCheck.c](../../_test/testdata/triceCheck.c) supplies the C sites and their expected text. [Test Internals](#test-internals) describes the bridge and comparison loop; this section covers execution strategy, diagnostics and cache handling.

The bulk path still executes each C test site. It collects binary output and starts the host logger once per output channel, then compares every expected text range with its source line. This avoids repeated logger initialization. Finite replay inputs finish when their buffered records are drained; they do not wait for a fixed timeout after each expectation.

Framed direct and deferred channels are collected separately. Configurations that previously transferred after every test site retain that transfer schedule, so small target buffers cannot overflow merely because host decoding is batched. Existing deferred bulk tests retain their multi-site transfer schedule to exercise buffering. Unframed configurations keep the single-expectation path because concatenation can lose packet boundaries or change padding interpretation. Successful bulk runs are not repeated completely line by line.

Each configuration has its own `output.log` below `temp/log/pc-<workflow>.<run>/`. On a bulk mismatch, the original binary and text output are saved there, and the worker reruns that configuration line by line. A passing diagnostic rerun does not clear the bulk failure: it points to an interaction involving framing, buffering or state. The reported source line is the first divergent expectation, which may follow the actual cause. Expected and actual strings show escaped control characters; nearby text helps identify shifts. The failure report also gives a reproduction command, which requires the same prepared ID state and compiler include paths. Run directories are retained for diagnosis; subsequent runs use a new directory.

The managed PC worker preserves Go's build cache between runs. Before testing, [pc_cache_overlay.go](../../scripts/pc_cache_overlay.go) hashes file names and contents in `src`, `_test/testdata`, each configuration directory and the applicable Bind sidecar directory, including a custom generation directory. It also records the ID workflow and compiler/header-search settings. The worker appends the resulting SHA-256 signature to temporary copies of the CGO Go sources and supplies them through a Go overlay. External C or header changes therefore become visible to Go's build cache without changing the original sources or discarding cached Go dependencies. Added, removed or renamed input files change the signature too; timestamps alone do not. A local configuration change rebuilds that configuration, while shared input changes rebuild all affected configurations. The ABC host bridges participate as well as the generated harnesses.

Every selected test still executes with `-count=1`; only compiled build artifacts are reused. The preceding normal Go and coverage steps also preserve the build cache and use `-count=1`, so a complete `testAll.sh` invocation does not discard the PC builds from the previous run. Overlay JSON and stamped sources remain beside the logs in `overlay.json` and `cache-inputs/` for diagnosis. The temporary Go filenames start with `_`, keeping them out of ordinary package discovery. Unreadable inputs or symbolic links inside the input trees stop preparation instead of silently running a potentially stale binary. This protection covers the repository's PC input trees, not independently modified external C libraries or SDK headers at an unchanged location. After such external changes, explicitly run `go clean -cache`. Go itself accounts for compiler identity and build flags; see [Go's build-cache documentation](https://pkg.go.dev/cmd/go#hdr-Build_and_test_caching).

The behavioral checks in [pc_cache_overlay_test.go](../../scripts/pc_cache_overlay_test.go) include real cold and warm CGO builds. They verify current C runtime values after source, header, sidecar, configuration and compiler-option changes, verify that unchanged configurations avoid C recompilation, and independently count actual test executions.

The `testdata\cgoPackage.go` file contains a variable `testLines = n`, which limits the amount of performed trices for each test case to `n`. Changing this value will heavily influence the test duration. The value `-1` is reserved for testing all test lines.

## 5. <a id="how-to-add-new-test-cases"></a>How to add new test cases

- Choose a test folder similar to the intended test and copy it under a new descriptive name like `newTest`.
- Add the new package to the test-folder list in `scripts/_330_renew_ids_and_refresh_tests.sh` if it should receive refreshed generated test files.
- Edit files `newTest/triceConfig.h` and `newTest/cgo_test.go` in a matching way.
- Use the managed PC wrapper with the new configuration selected; direct `go test ./_test/newTest` requires the ID/include/overlay preparation described above.

## 6. <a id="test-internals"></a>Test Internals

Each configuration is an independent `cgot` package. Its `triceConfig.h` selects the C behavior; `cgo_test.go` supplies matching host decoder options. The shared [cgoPackage.go](../../_test/testdata/cgoPackage.go) contains the CGO bridge and test loop. Normal workers overlay it rather than rewriting all generated copies.

The harness finds `//exp:` expectations and their source line numbers in [triceCheck.c](../../_test/testdata/triceCheck.c). It calls the C switch case for each site, collects direct and deferred output through the test writers, decodes it, then compares the text. The capture supports multiple records; the old claim that its shared buffer allowed only one Trice per line no longer describes this harness. Bind's separate [source-site limits](bind.md#bind-limits) still apply.

| Mode | C execution | Host decoding |
| --- | --- | --- |
| Bulk | Execute every selected site and retain its expected output boundary. | Start the logger once per output channel. |
| Line by line | Execute one expectation at a time. | Decode separately; useful for unframed inputs and diagnosis. |
| Auto | Choose according to the configuration. | Bulk where framing permits it, otherwise line by line. |

A passing diagnostic rerun does not erase a bulk failure. Both modes must preserve the configured transfer schedule and test coverage. See the cache and artifact details above.

### 6.1. <a id="comparing-test-duration-between-computers"></a>Comparing test duration between computers

Compare the same commit, workflow, configuration count, compiler versions and job limits. Keep the distinction between cold compilation, warm compilation and actual test execution:

| Observation | Check |
| --- | --- |
| First run slower | Go/tool downloads, compiler setup and empty build caches. |
| Every warm run slow | Verify cache directory reuse and overlay input signatures. |
| One platform much slower | Compare per-step times, compiler processes, storage and antivirus scanning; do not infer a C/Go defect from total time alone. |
| Line mode slower than bulk | Repeated logger initialization is deliberate diagnostic overhead. |
| High parallelism slower | Reduce `TRICE_PC_TEST_JOBS` or `TRICE_L432_TEST_JOBS` and compare memory/CPU pressure. |

Keep `-count=1` and all expectations enabled. A cached test result is not evidence that this run executed the tests. The Insert/Clean `-cache` option is unrelated to Go's compiled-artifact cache.

## 7. <a id="test-results"></a>Test Results

The following is a **historical December 2024 run**, not a current acceptance result or a duration target. The current runner, bulk path and cache handling differ.


```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice (main)
$ ./scripts/testAll.sh
Thu, Dec 12, 2024  4:51:26 PM
This can take several minutes ...
?       github.com/rokath/trice/internal/decoder        [no test files]
?       github.com/rokath/trice/internal/do     [no test files]
?       github.com/rokath/trice/internal/translator     [no test files]
?       github.com/rokath/trice/pkg/ant [no test files]
ok      github.com/rokath/trice/cmd/trice       1.392s
ok      github.com/rokath/trice/internal/args   0.415s
ok      github.com/rokath/trice/internal/charDecoder    0.298s
ok      github.com/rokath/trice/internal/com    15.845s
ok      github.com/rokath/trice/internal/dumpDecoder    0.339s
ok      github.com/rokath/trice/internal/emitter        0.326s
ok      github.com/rokath/trice/internal/id     3.088s
ok      github.com/rokath/trice/internal/keybcmd        0.233s
ok      github.com/rokath/trice/internal/link   0.196s
ok      github.com/rokath/trice/internal/receiver       0.246s
?       github.com/rokath/trice/internal/translator     [no test files]
?       github.com/rokath/trice/pkg/ant [no test files]
ok      github.com/rokath/trice/cmd/trice       1.392s
ok      github.com/rokath/trice/internal/args   0.415s
ok      github.com/rokath/trice/internal/charDecoder    0.298s
ok      github.com/rokath/trice/internal/com    15.845s
ok      github.com/rokath/trice/internal/dumpDecoder    0.339s
ok      github.com/rokath/trice/internal/emitter        0.326s
ok      github.com/rokath/trice/internal/id     3.088s
ok      github.com/rokath/trice/internal/keybcmd        0.233s
ok      github.com/rokath/trice/internal/link   0.196s
ok      github.com/rokath/trice/internal/receiver       0.246s
ok      github.com/rokath/trice/internal/trexDecoder    0.264s
ok      github.com/rokath/trice/pkg/cipher      0.230s
ok      github.com/rokath/trice/pkg/endian      0.161s
ok      github.com/rokath/trice/internal/args   0.415s
ok      github.com/rokath/trice/internal/charDecoder    0.298s
ok      github.com/rokath/trice/internal/com    15.845s
ok      github.com/rokath/trice/internal/dumpDecoder    0.339s
ok      github.com/rokath/trice/internal/emitter        0.326s
ok      github.com/rokath/trice/internal/id     3.088s
ok      github.com/rokath/trice/internal/keybcmd        0.233s
ok      github.com/rokath/trice/internal/link   0.196s
ok      github.com/rokath/trice/internal/receiver       0.246s
ok      github.com/rokath/trice/internal/trexDecoder    0.264s
ok      github.com/rokath/trice/pkg/cipher      0.230s
ok      github.com/rokath/trice/pkg/endian      0.161s
ok      github.com/rokath/trice/internal/id     3.088s
ok      github.com/rokath/trice/internal/keybcmd        0.233s
ok      github.com/rokath/trice/internal/link   0.196s
ok      github.com/rokath/trice/internal/receiver       0.246s
ok      github.com/rokath/trice/internal/trexDecoder    0.264s
ok      github.com/rokath/trice/pkg/cipher      0.230s
ok      github.com/rokath/trice/pkg/endian      0.161s
ok      github.com/rokath/trice/pkg/msg 0.157s
ok      github.com/rokath/trice/pkg/tst 0.261s
ok      github.com/rokath/trice/_test/be_dblB_de_tcobs_ua       123.142s
ok      github.com/rokath/trice/_test/be_staticB_di_xtea_cobs_rtt32     123.159s
ok      github.com/rokath/trice/internal/trexDecoder    0.264s
ok      github.com/rokath/trice/pkg/cipher      0.230s
ok      github.com/rokath/trice/pkg/endian      0.161s
ok      github.com/rokath/trice/pkg/msg 0.157s
ok      github.com/rokath/trice/pkg/tst 0.261s
ok      github.com/rokath/trice/_test/be_dblB_de_tcobs_ua       123.142s
ok      github.com/rokath/trice/_test/be_staticB_di_xtea_cobs_rtt32     123.159s
ok      github.com/rokath/trice/pkg/msg 0.157s
ok      github.com/rokath/trice/pkg/tst 0.261s
ok      github.com/rokath/trice/_test/be_dblB_de_tcobs_ua       123.142s
ok      github.com/rokath/trice/_test/be_staticB_di_xtea_cobs_rtt32     123.159s
ok      github.com/rokath/trice/_test/dblB_de_cobs_ua   122.964s
ok      github.com/rokath/trice/_test/dblB_de_multi_cobs_ua     123.308s
ok      github.com/rokath/trice/_test/be_dblB_de_tcobs_ua       123.142s
ok      github.com/rokath/trice/_test/be_staticB_di_xtea_cobs_rtt32     123.159s
ok      github.com/rokath/trice/_test/dblB_de_cobs_ua   122.964s
ok      github.com/rokath/trice/_test/dblB_de_multi_cobs_ua     123.308s
ok      github.com/rokath/trice/_test/dblB_de_cobs_ua   122.964s
ok      github.com/rokath/trice/_test/dblB_de_multi_cobs_ua     123.308s
ok      github.com/rokath/trice/_test/dblB_de_multi_cobs_ua     123.308s
ok      github.com/rokath/trice/_test/dblB_de_multi_nopf_ua     123.244s
ok      github.com/rokath/trice/_test/dblB_de_multi_nopf_ua     123.244s
ok      github.com/rokath/trice/_test/dblB_de_multi_tcobs_ua    123.109s
ok      github.com/rokath/trice/_test/dblB_de_multi_tcobs_ua    123.109s
ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_cobs_ua        123.213s
ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_tcobs_ua       123.001s
ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_cobs_ua        123.213s
ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_tcobs_ua       123.001s
ok      github.com/rokath/trice/_test/dblB_de_nopf_ua   123.092s
ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_tcobs_ua       123.001s
ok      github.com/rokath/trice/_test/dblB_de_nopf_ua   123.092s
ok      github.com/rokath/trice/_test/dblB_de_tcobs_ua  122.324s
ok      github.com/rokath/trice/_test/dblB_de_nopf_ua   123.092s
ok      github.com/rokath/trice/_test/dblB_de_tcobs_ua  122.324s
ok      github.com/rokath/trice/_test/dblB_de_tcobs_ua  122.324s
ok      github.com/rokath/trice/_test/dblB_de_xtea_cobs_ua      123.149s
ok      github.com/rokath/trice/_test/dblB_de_xtea_tcobs_ua     122.883s
ok      github.com/rokath/trice/_test/dblB_de_xtea_cobs_ua      123.149s
ok      github.com/rokath/trice/_test/dblB_de_xtea_tcobs_ua     122.883s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_cobs_ua    246.703s
ok      github.com/rokath/trice/_test/dblB_de_xtea_tcobs_ua     122.883s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_cobs_ua    246.703s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_cobs_ua    246.703s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_multi_cobs_ua      247.125s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_multi_tcobs_ua     246.862s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_tcobs_ua   246.531s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_xtea_cobs_ua       247.072s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_cobs_ua     246.639s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_multi_cobs_ua       246.599s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_multi_tcobs_ua      247.114s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_tcobs_ua    246.851s
ok      github.com/rokath/trice/_test/ringB_de_cobs_ua  123.578s
ok      github.com/rokath/trice/_test/ringB_de_multi_tcobs_ua   123.517s
ok      github.com/rokath/trice/_test/ringB_de_multi_xtea_cobs_ua       123.497s
ok      github.com/rokath/trice/_test/ringB_de_multi_xtea_tcobs_ua      123.379s
ok      github.com/rokath/trice/_test/ringB_de_nopf_ua  123.555s
ok      github.com/rokath/trice/_test/ringB_de_tcobs_ua 123.300s
ok      github.com/rokath/trice/_test/ringB_de_xtea_cobs_ua     123.487s
ok      github.com/rokath/trice/_test/ringB_de_xtea_tcobs_ua    123.846s
ok      github.com/rokath/trice/_test/ringB_di_cobs_rtt32__de_tcobs_ua  247.400s
ok      github.com/rokath/trice/_test/ringB_di_cobs_rtt8__de_tcobs_ua   247.202s
ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt32__de_tcobs_ua  247.204s
ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt32__de_xtea_cobs_ua      246.818s
ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt8__de_tcobs_ua   247.006s
ok      github.com/rokath/trice/_test/ringB_di_tcobs_rtt32__de_tcobs_ua 247.000s
ok      github.com/rokath/trice/_test/ringB_di_xtea_cobs_rtt32__de_xtea_cobs_ua 246.872s
ok      github.com/rokath/trice/_test/special_protect_dblB_de_tcobs_ua  0.444s
ok      github.com/rokath/trice/_test/stackB_di_nopf_aux32      123.819s
ok      github.com/rokath/trice/_test/stackB_di_nopf_aux8       123.830s
ok      github.com/rokath/trice/_test/stackB_di_nopf_rtt32      123.912s
ok      github.com/rokath/trice/_test/stackB_di_nopf_rtt8       123.976s
ok      github.com/rokath/trice/_test/stackB_di_xtea_cobs_rtt8  123.719s
ok      github.com/rokath/trice/_test/staticB_di_nopf_aux32     123.553s
ok      github.com/rokath/trice/_test/staticB_di_nopf_aux8      123.551s
ok      github.com/rokath/trice/_test/staticB_di_nopf_rtt32     123.596s
ok      github.com/rokath/trice/_test/staticB_di_nopf_rtt8      123.618s
ok      github.com/rokath/trice/_test/staticB_di_tcobs_rtt32    123.177s
ok      github.com/rokath/trice/_test/staticB_di_tcobs_rtt8     123.353s
ok      github.com/rokath/trice/_test/staticB_di_xtea_cobs_rtt32        123.126s

real    10m31.130s
user    0m0.000s
sys     0m0.015s

ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice (main)
$
```

## 8. <a id="test-cases"></a>Test Cases

### 8.1. <a id="folder-naming-convention"></a>Folder Naming Convention

Folder Name Part | Meaning
:----------------:|----------------------------------------------------------------------------------------------------------
`testdata`    | This is no test folder. It contains data common to all tests.
`_...`      | Folder starting with an undescore `_` are excluded when `go test ./...` is executed.
`_di_`      | direct mode
`_de_`      | deferred mode
`special_`    | a test, not using `./testdata/triceCheck.c`
`staticB_`    | static buffer, direct mode only possible
`stackB_`     | stack buffer, direct mode only possible
`ringB_`     | ring buffer, deferred mode and optional parallel direct mode
`dblB_`      | double buffer, deferred mode and optional parallel direct mode
`_rtt8_`     | (simulated) SEGGER_RTT byte transfer
`_rtt32_`     | (simulated) SEGGER_RTT word transfer
`__`       | direct and deferred mode together
`_xtea_`     | with encryption, otherwise without encryption
`_tcobs`     | TCOBS package framing
`_cobs`      | COBS package framing
`_nopf`      | no package framing
`_multi_`     | Usually each Trice is handled separately. In multi mode, groups of available Trices are framed together.
`_ua`       | simulated UART A output (for deferred modes)

