# Testing the Trice Target Library

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. General info](#general-info)
* [2. How to run the tests](#how-to-run-the-tests)
  * [2.1. CLion review build](#clion-review-build)
* [3. Tests Details](#tests-details)
* [4. How to add new test cases](#how-to-add-new-test-cases)
* [5. Test Internals](#test-internals)
* [6. Test Results](#test-results)
* [7. Special tests](#special-tests)
* [8. Test Cases](#test-cases)
  * [8.1. Folder Naming Convention](#folder-naming-convention)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="general-info"></a>General info

This folder is per default named to `_test` to avoid VS Code slow down. Also, when running `go test ./...`,  the tests in the `_test` folder are excluded, because they take a long time. Run `./scripts/testAll.sh` to include them.

The main aim of these tests is to automatic compile and run the target code in different compiler switch variants avoiding manual testing this way. 

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

## 2. <a id="how-to-run-the-tests"></a>How to run the tests

* Host compiler prerequisites:
  * CGO and host-side target-code tests need a working host C compiler, not only a compiler executable in `PATH`.
  * On Windows, TDM-GCC or another matching MinGW-w64 GCC installation can provide the host compiler and C runtime.
  * Some Go regression tests execute every supported compiler found in `PATH`, including `clang`. If Clang is visible, verify it first with `printf '#include <string.h>\n' | clang -std=c99 -fsyntax-only -x c -`.
  * Keep `C_INCLUDE_PATH` unset globally so ARM cross-compiler headers do not leak into host and CGO builds.
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

### 2.1. <a id="clion-review-build"></a>CLion review build

The [CMake review project](../../_test/clion-review/CMakeLists.txt) builds a host executable for indexing and inspections. With `trice`, CMake and a host C compiler available:

```sh
cmake -S _test/clion-review -B temp/clion-review
cmake --build temp/clion-review
```

If needed, select the tool with `-DTRICE_EXECUTABLE=path/to/trice`. CMake copies the shared test source and TIL into the build directory, prepares private Bind headers, and refreshes them before compilation. The canonical source and shared dictionaries remain unchanged. This is a host review build, not a board test or a replacement for the CGO matrix.

Step `_515` checks the real CLion build and all nine ABC nodes with both ID workflows in isolated checkout paths containing spaces. To run just this check, use `TRICE_BIND_INTEGRATION=1 go test ./scripts -run '^TestBindEntrypointIntegration$' -count=1`. Ordinary script tests use tool doubles; the GCC example matrix additionally exercises the two small F030 entry points and the G0B1 Clang entry. Step `_515` builds the G0B1 feature tour through its public GCC entry, and also through its Clang entry in `full` mode.

## 3. <a id="tests-details"></a>Tests Details

All folders despite `testdata` are test folders and the name `tf` is used as a place holder for them in this document.

To exclude a specific folder temporary, simply rename it to start with an underscore `_tf`.

The `tf` are serving for target code testing in different configuration variants on the host machine. The file [./testdata/triceCheck.c](../../_test/testdata/triceCheck.c) is the main file for most tests and serves also as example usage.

[_test/testdata/cgoPackage.go](../../_test/testdata/cgoPackage.go) is the common main for the `generated_cgoPackage.go` files and contains the common test code. 

The folders `tf` are Go packages just for tests. They all have the same package name `cgot` and are not included into the trice tool. The different `cgot` packages are independent and could have any names. They do not see each other and are used for target code testing independently. When the tests are executed for each package, a separate test binary is build and these run parallel.

The `tf/triceConfig.h` files differ and correspondent to the `tf/cgo_test.go` files in the same folder. On test execution, the `./testdata/*.c` files are compiled into the trice test executable together with the trice sources `../src` using the `tf/triceConfig.h` file.

The individual tests collect the expected results (`//exp: result`) together with the line numbers into a slice to execute the test loop on it. The `triceLogTest` function gets the `triceLog` function as parameter.

`triceLogTest` iterates over the results slice and calls for each line the C-function `triceCheck`. Then the line specific binary data buffer is passed to the `triceLog` parameter function which "logs" the passed buffer into an actual result string which in turn is compared with the expected result.

The bulk path still executes each C test site. It collects binary output and starts the host logger once per output channel, then compares every expected text range with its source line. This avoids repeated logger initialization. Finite replay inputs finish when their buffered records are drained; they do not wait for a fixed timeout after each expectation.

Framed direct and deferred channels are collected separately. Configurations that previously transferred after every test site retain that transfer schedule, so small target buffers cannot overflow merely because host decoding is batched. Existing deferred bulk tests retain their multi-site transfer schedule to exercise buffering. Unframed configurations keep the single-expectation path because concatenation can lose packet boundaries or change padding interpretation. Successful bulk runs are not repeated completely line by line.

Each configuration has its own `output.log` below `temp/log/pc-<workflow>.<run>/`. On a bulk mismatch, the original binary and text output are saved there, and the worker reruns that configuration line by line. A passing diagnostic rerun does not clear the bulk failure: it points to an interaction involving framing, buffering or state. The reported source line is the first divergent expectation, which may follow the actual cause. Expected and actual strings show escaped control characters; nearby text helps identify shifts. The failure report also gives a reproduction command, which requires the same prepared ID state and compiler include paths. Run directories are retained for diagnosis; subsequent runs use a new directory.

The managed PC worker preserves Go's build cache between runs. Before testing, [pc_cache_overlay.go](../../scripts/pc_cache_overlay.go) hashes file names and contents in `src`, `_test/testdata`, each configuration directory and the applicable Bind sidecar directory, including a custom generation directory. It also records the ID workflow and compiler/header-search settings. The worker appends the resulting SHA-256 signature to temporary copies of the CGO Go sources and supplies them through a Go overlay. External C or header changes therefore become visible to Go's build cache without changing the original sources or discarding cached Go dependencies. Added, removed or renamed input files change the signature too; timestamps alone do not. A local configuration change rebuilds that configuration, while shared input changes rebuild all affected configurations. The ABC host bridges participate as well as the generated harnesses.

Every selected test still executes with `-count=1`; only compiled build artifacts are reused. The preceding normal Go and coverage steps also preserve the build cache and use `-count=1`, so a complete `testAll.sh` invocation does not discard the PC builds from the previous run. Overlay JSON and stamped sources remain beside the logs in `overlay.json` and `cache-inputs/` for diagnosis. The temporary Go filenames start with `_`, keeping them out of ordinary package discovery. Unreadable inputs or symbolic links inside the input trees stop preparation instead of silently running a potentially stale binary. This protection covers the repository's PC input trees, not independently modified external C libraries or SDK headers at an unchanged location. After such external changes, explicitly run `go clean -cache`. Go itself accounts for compiler identity and build flags; see [Go's build-cache documentation](https://pkg.go.dev/cmd/go#hdr-Build_and_test_caching).

The behavioral checks in [pc_cache_overlay_test.go](../../scripts/pc_cache_overlay_test.go) include real cold and warm CGO builds. They verify current C runtime values after source, header, sidecar, configuration and compiler-option changes, verify that unchanged configurations avoid C recompilation, and independently count actual test executions.

The `testdata\cgoPackage.go` file contains a variable `testLines = n`, which limits the amount of performed trices for each test case to `n`. Changing this value will heavily influence the test duration. The value `-1` is reserved for testing all test lines.

## 4. <a id="how-to-add-new-test-cases"></a>How to add new test cases

- Choose a test folder similar to the intended test and copy it under a new descriptive name like `newTest`.
- Add the new package to the test-folder list in `scripts/_330_renew_ids_and_refresh_tests.sh` if it should receive refreshed generated test files.
- Edit files `newTest/triceConfig.h` and `newTest/cgo_test.go` in a matching way.
- Run `go test ./_test/newTest` from the repository root.

## 5. <a id="test-internals"></a>Test Internals

The `./trice/_test/testdata/*.c` and `./trice/src/*.c` are compiled together with the actual cgot package into one single Trice test binary, resulting in as many test binaries as there are test folders. Calling its TeCEFunction(s) causes the activation of the Trice statement(s) inside *triceCheck.c*. The ususally into an embedded device compiled Trice code generates a few bytes according to the configuration into a buffer. These bytes are transmitted usually in real life over a (serial) port or RTT. In the tests here, this buffer is then read out by the Trice tool handler function according to the used CLI switches and processed to a log string using the *til.json* file. This string is then compared to the expected string for the activated line.

Each `tf` is a **Go** package, which is not part of any **Go** application. They are all named `cgot` and are only used independently for testing different configurations. The `tf/generated_cgoPackage.go` file is identical in all `tf`. Its master is `_test/testdata/cgoPackage.go`. The test worker overlays the master during normal test runs. The maintenance script `./scripts/_330_renew_ids_and_refresh_tests.sh` copies the master into the listed packages, but also renews IDs and clears ID history by default; use `keepHistory` only when you deliberately run that wider maintenance workflow.

The test specific target code configuration is inside `tf/trice.Config.h` and the appropriate Trice tool CLI switches are in `tf/cgo_test.go`.

When running `go test ./tf`, a Trice tool test executable is build, using the Trice tool packages and the `tf` package `cgot`, and the function `TestLogs` is executed. Its internal closure `triceLog` contains the Trice tool CLI switches and is passed to the `ccgot` package function `triceLogTest` together with the number of testLines and the trice mode (`directTransfer` or `deferrerdTransfer`).

During the test, the file `triceCheck.c` is scanned for lines like

```C
break; case __LINE__: TRice( iD(3537), "info:This is a message without values and a 32-bit stamp.\n" ); //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
```

Some C-code lines contain Trice statements and comments starting with `//exp: ` followed by the expected Trice tool output for that specific line. The **Go** teCEFunction collects these outputs in a slice together with the line numbers. Then for each found line number the execution of the **Go** function `func triceCheck(n int)` takes part, which in turn calls the CGO compiled C-function `TriceCheck(n)`. The now activated Trice C-code writes the generated trice bytes in a between **C** and **Go** shared buffer using the C-function `TriceWriteDeviceCgo`. After returning from the **Go** function `func triceCheck(n int)` and optionally calling `TriceTransfer` in deferred mode the Trice tool `triceLog()` function converts the Trice buffer bytes to the log string and compares the result with the expected data. The between **Go** and **C** shared buffer limits the executed Trices per line to one, because they use the same buffer from the beginning. This could be done better with an increment to allow several trices in one single line.

Because each test runs a different configuration, all possible combinations are testable.

## 6. <a id="test-results"></a>Test Results

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

## 7. <a id="special-tests"></a>Special tests

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

<p align="right">(<a href="#testing-the-trice-target-library">back to top</a>)</p>

