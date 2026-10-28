# Testing Trice — Internals

Use [Running the Tests](target_tests.md) for commands, prerequisites, GitHub Actions and release checks. This part explains how the tests produce their evidence and where to make a focused change.

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [Where the tests live](#where-the-tests-live)
* [From C call to checked text](#from-c-call-to-checked-text)
  * [Execute and compare records](#execute-and-compare-records)
  * [Shared and additional Go test entry points](#shared-and-additional-go-test-entry-points)
* [CGO configuration catalogue](#cgo-configuration-catalogue)
  * [Deferred double buffer](#deferred-double-buffer)
  * [Double buffer with simultaneous direct and deferred output](#double-buffer-with-simultaneous-direct-and-deferred-output)
  * [Deferred ring buffer](#deferred-ring-buffer)
  * [Ring buffer with simultaneous direct and deferred output](#ring-buffer-with-simultaneous-direct-and-deferred-output)
  * [Direct stack and static buffers](#direct-stack-and-static-buffers)
  * [Special configurations](#special-configurations)
  * [ABC case details](#abc-case-details)
* [CGO build cache](#cgo-build-cache)
* [How the suite is assembled](#how-the-suite-is-assembled)
  * [Feature checks and firmware matrices](#feature-checks-and-firmware-matrices)
* [Add or change a test](#add-or-change-a-test)

<!-- numbering=false min=2 max=3 slug=github anchor=false link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## Where the tests live

| Location | Responsibility |
| --- | --- |
| [internal](../../internal), [cmd](../../cmd), [pkg](../../pkg) | Go unit and integration tests beside their implementation: CLI/IDs, parsing, encoding/decoding, transports, filtering, output and shared helpers. |
| [scripts](../../scripts) | Behavioral tests of public scripts, cancellation, source restoration, tool setup, release preparation and cache invalidation. Most use isolated fixtures; explicitly enabled integration builds real examples. |
| [scripts/testcheck](../../scripts/testcheck) | Tool identity/download/cache checks, Go-event auditing and structured suite reports. |
| [_test](../../_test) | 63 independently compiled CGO configuration packages, catalogued below. Go does not discover this underscore-prefixed tree from `go test ./...`. |
| [examples](../../examples) | Native demos and MCU projects built through their public entry points. The managed wrappers restore shared state. |

The native CLI tests in [native_platform_test.go](../../internal/args/native_platform_test.go) build the current tools in temporary paths with spaces and Unicode. They check LF/CRLF, LI lines, relative paths, Bind byte preservation, Insert/Clean round trips, filesystem case aliases, and C records decoded with both compiler families. [The Windows test](../../internal/args/native_platform_windows_test.go) adds a real sharing-lock failure and checks preservation/retry. These are additional integration tests, not substitutes for the configuration matrix.

## From C call to checked text

CGO is Go's interface to compiled C. A PC configuration compiles the real Trice sources for the current host and calls them from Go. UART/RTT writers are simulated; the record creation and host decoding are real.

```text
triceConfig.h + Trice C library + C test sites
                 | native C compiler / CGO
                 v
       captured direct/deferred binary records
                 | real host decoder + matching TIL
                 v
             decoded text
                 | compare with the site's //exp: expectation
                 v
          pass or source-linked mismatch
```

Each ordinary configuration uses these pieces:

| File | Role |
| --- | --- |
| Its `triceConfig.h` | Selects buffer, direct/deferred outputs, framing, encryption, transfer mode and simulated writer. |
| Its `cgo_test.go` | Chooses matching host decoder flags and test strategy; special packages define their own assertions here. |
| [testdata/cgoPackage.go](../../_test/testdata/cgoPackage.go) | Canonical CGO bridge, C source includes, capture/transfer helpers and comparison loops. |
| [testdata/cgoPackage_test.go](../../_test/testdata/cgoPackage_test.go) | Shared test entry points, state setup and diagnostic regression tests. |
| [testdata/cgoTrice.c](../../_test/testdata/cgoTrice.c) | Simulated output writers, capture buffers and framing-capability probes. |
| [testdata/triceCheck.c](../../_test/testdata/triceCheck.c) | Shared C calls and their expected text. |
| `generated_cgoPackage*.go` | Package-local bridge/test copies. Normal runs overlay the current canonical files; do not hand-edit these copies. |

The bridge includes the C implementation files directly, so each package receives its own compile-time configuration. Its CGO include order is the configuration directory, `src`, then `src/default_conf`; Bind preparation adds the generated sidecar include paths. Each configuration runs in a separate process, keeping its C state and Go decoder globals separate from other configurations.

### Execute and compare records

The harness reads the `//exp:` markers in `triceCheck.c`, remembers their line numbers and calls `TriceCheck(line)` for each selected site. C uses `case __LINE__` to select the corresponding code. The expected string therefore stays beside the call that produces it. The file covers stamped and unstamped calls, integer widths, numeric formats, floats/doubles, strings and buffers, assertions, structured calls, legacy F calls and counted X0 payloads. Exact expected output belongs in the source rather than in a second prose list.

`testLines = -1` means all expectations. Positive values are a local debugging limit, not a complete test result. A selected line may emit multiple records; the capture is not limited to one Trice per source line.

| Strategy | Behavior |
| --- | --- |
| `auto` | The worker asks the compiled C configuration whether framed channels support batched decoding. Unframed and special cases keep their suitable existing path. |
| Bulk | Executes all sites, retains source/output boundaries, and starts the host logger once per channel. Direct and deferred streams are kept separate. |
| Line by line | Captures and decodes one expectation at a time; needed where concatenation loses unframed packet boundaries and useful for diagnosis. |

Batched **decoding** does not mean changing every target's transfer schedule. Configurations that drain after each site retain that schedule; the existing deferred bulk cases retain their grouped transfers to test buffering. Finite replay finishes when the recorded input is drained.

The first mismatch stops that configuration's comparison loop, avoiding thousands of follow-on errors. Bulk diagnostics show the first divergent expectation, channel, escaped expected/actual text and nearby output. That line may follow the original cause of a corrupted stream. Original `.bin`/`.txt` files remain in the configuration's artifact directory. The worker may rerun line by line, but it never clears the original failure.

### Shared and additional Go test entry points

| Test | What it checks |
| --- | --- |
| `TestTriceLog` | Runs the shared expectation list using the configuration's direct, deferred, combined or special path. |
| `TestBulkOutputDiagnostics` | Checks source mapping and diagnostics for exact, empty and multiline output, changed values, missing records, unexpected prefixes and trailing records. This is a harness check, repeated by the shared package template. |
| `TestLogs` in the three alias/user-print packages | Runs their dedicated `TargetActivity.c` expectations through the actual decoder; see their catalogue rows. |
| `TestStructuredTriceExamples` in `ringB_de_multi_cobs_ua` | Separately identifies the canonical structured examples: 8/16/32/64-bit values, member fields mixed with classic placeholders, float/double wrappers, strings, bounded strings and a manual JSON field name. |
| `TestPCStopsAtFirstMismatch` in the same package | Deliberately fails child processes in line, bulk and combined modes; requires one source-linked diagnostic, no subsequent comparison, and retained bulk input/output. |
| `TestTriceAbcReceiveRuntime` | Runs 15 C receiver/resolver scenarios; see the ABC cases below. |
| `TestTriceAbcTransmitMacros` | Compares six ABC macro calls with fixed wire bytes; see the ABC cases below. |

`TestMain` and per-test setup save/restore shared Go settings. They are lifecycle helpers, not extra C coverage claims.

## CGO configuration catalogue

Every independently selected package appears once below. A row describes its compile-time variation; ordinary rows run the common expectation list just explained. Follow the package link for `triceConfig.h`, decoder options and any custom fixture.

Read names as a guide, not as a replacement for the configuration: `di` means direct, `de` deferred, `__` joins two output paths, `rtt8`/`rtt32` are byte/word RTT writer simulations, and `ua` is simulated UART A. `aux8`/`aux32` use the auxiliary writer. COBS/TCOBS frame records; `nopf` means unframed; XTEA adds encryption. Single-pack frames individual records; multi-pack frames groups. Double/ring buffers support deferred output; stack/static buffers serve direct output.

### Deferred double buffer

All these packages use simulated UART A. The single/multi difference remains active even when the host decoder batches input.

| Package | Variation checked |
| --- | --- |
| [dblB_de_multi_cobs_ua](../../_test/dblB_de_multi_cobs_ua) | Multi-pack COBS. |
| [dblB_de_multi_nopf_ua](../../_test/dblB_de_multi_nopf_ua) | Multi-pack without framing. |
| [dblB_de_multi_tcobs_ua](../../_test/dblB_de_multi_tcobs_ua) | Multi-pack TCOBS. |
| [dblB_de_multi_xtea_cobs_ua](../../_test/dblB_de_multi_xtea_cobs_ua) | Multi-pack XTEA + COBS. |
| [dblB_de_multi_xtea_tcobs_ua](../../_test/dblB_de_multi_xtea_tcobs_ua) | Multi-pack XTEA + TCOBS. |
| [dblB_de_single_cobs_ua](../../_test/dblB_de_single_cobs_ua) | Single-pack COBS. |
| [dblB_de_single_nopf_ua](../../_test/dblB_de_single_nopf_ua) | Single-pack without framing. |
| [dblB_de_single_tcobs_ua](../../_test/dblB_de_single_tcobs_ua) | Single-pack TCOBS. |
| [dblB_de_single_xtea_cobs_ua](../../_test/dblB_de_single_xtea_cobs_ua) | Single-pack XTEA + COBS. |
| [dblB_de_single_xtea_tcobs_ua](../../_test/dblB_de_single_xtea_tcobs_ua) | Single-pack XTEA + TCOBS. |

### Double buffer with simultaneous direct and deferred output

Each site must decode correctly on both paths. Deferred output uses UART A; the columns distinguish only what changes between configurations. “None” means no framing, not no output.

| Package | Direct path | Deferred path |
| --- | --- | --- |
| [dblB_di_cobs_rtt32__de_xtea_cobs_ua](../../_test/dblB_di_cobs_rtt32__de_xtea_cobs_ua) | RTT32, COBS | XTEA + COBS |
| [dblB_di_nopf_rtt32__de_cobs_ua](../../_test/dblB_di_nopf_rtt32__de_cobs_ua) | RTT32, none | COBS |
| [dblB_di_nopf_rtt32__de_multi_cobs_ua](../../_test/dblB_di_nopf_rtt32__de_multi_cobs_ua) | RTT32, none | Explicit multi-pack COBS |
| [dblB_di_nopf_rtt32__de_multi_tcobs_ua](../../_test/dblB_di_nopf_rtt32__de_multi_tcobs_ua) | RTT32, none | Explicit multi-pack TCOBS |
| [dblB_di_nopf_rtt32__de_tcobs_ua](../../_test/dblB_di_nopf_rtt32__de_tcobs_ua) | RTT32, none | TCOBS |
| [dblB_di_nopf_rtt32__de_xtea_cobs_ua](../../_test/dblB_di_nopf_rtt32__de_xtea_cobs_ua) | RTT32, none | XTEA + COBS |
| [dblB_di_nopf_rtt8__de_cobs_ua](../../_test/dblB_di_nopf_rtt8__de_cobs_ua) | RTT8, none | COBS |
| [dblB_di_nopf_rtt8__de_multi_cobs_ua](../../_test/dblB_di_nopf_rtt8__de_multi_cobs_ua) | RTT8, none | Explicit multi-pack COBS |
| [dblB_di_nopf_rtt8__de_multi_tcobs_ua](../../_test/dblB_di_nopf_rtt8__de_multi_tcobs_ua) | RTT8, none | Explicit multi-pack TCOBS |
| [dblB_di_nopf_rtt8__de_tcobs_ua](../../_test/dblB_di_nopf_rtt8__de_tcobs_ua) | RTT8, none | TCOBS |

Unspecified deferred transfer mode currently defaults to multi-pack. The explicitly named multi-pack packages retain that setting in their own header; both sets remain independently selected.

### Deferred ring buffer

These use UART A, like the deferred double-buffer cases, but exercise the ring-buffer implementation and wrap/transfer behavior.

| Package | Variation checked |
| --- | --- |
| [ringB_de_multi_cobs_ua](../../_test/ringB_de_multi_cobs_ua) | Multi-pack COBS; also owns the structured-example and first-mismatch tests. |
| [ringB_de_multi_nopf_ua](../../_test/ringB_de_multi_nopf_ua) | Multi-pack without framing; explicitly enlarged deferred buffer. |
| [ringB_de_multi_tcobs_ua](../../_test/ringB_de_multi_tcobs_ua) | Multi-pack TCOBS. |
| [ringB_de_multi_xtea_cobs_ua](../../_test/ringB_de_multi_xtea_cobs_ua) | Multi-pack XTEA + COBS. |
| [ringB_de_multi_xtea_tcobs_ua](../../_test/ringB_de_multi_xtea_tcobs_ua) | Multi-pack XTEA + TCOBS. |
| [ringB_de_single_cobs_ua](../../_test/ringB_de_single_cobs_ua) | Single-pack COBS. |
| [ringB_de_single_nopf_ua](../../_test/ringB_de_single_nopf_ua) | Single-pack without framing. |
| [ringB_de_single_tcobs_ua](../../_test/ringB_de_single_tcobs_ua) | Single-pack TCOBS. |
| [ringB_de_single_xtea_cobs_ua](../../_test/ringB_de_single_xtea_cobs_ua) | Single-pack XTEA + COBS. |
| [ringB_de_single_xtea_tcobs_ua](../../_test/ringB_de_single_xtea_tcobs_ua) | Single-pack XTEA + TCOBS. |

### Ring buffer with simultaneous direct and deferred output

Deferred output again uses UART A and the default multi-pack mode.

| Package | Direct path | Deferred path |
| --- | --- | --- |
| [ringB_di_cobs_rtt32__de_tcobs_ua](../../_test/ringB_di_cobs_rtt32__de_tcobs_ua) | RTT32, COBS | TCOBS |
| [ringB_di_cobs_rtt32__de_xtea_cobs_ua](../../_test/ringB_di_cobs_rtt32__de_xtea_cobs_ua) | RTT32, COBS | XTEA + COBS |
| [ringB_di_cobs_rtt8__de_tcobs_ua](../../_test/ringB_di_cobs_rtt8__de_tcobs_ua) | RTT8, COBS | TCOBS |
| [ringB_di_nopf_rtt32__de_tcobs_ua](../../_test/ringB_di_nopf_rtt32__de_tcobs_ua) | RTT32, none | TCOBS |
| [ringB_di_nopf_rtt32__de_xtea_cobs_ua](../../_test/ringB_di_nopf_rtt32__de_xtea_cobs_ua) | RTT32, none | XTEA + COBS |
| [ringB_di_nopf_rtt8__de_tcobs_ua](../../_test/ringB_di_nopf_rtt8__de_tcobs_ua) | RTT8, none | TCOBS |
| [ringB_di_tcobs_rtt32__de_tcobs_ua](../../_test/ringB_di_tcobs_rtt32__de_tcobs_ua) | RTT32, TCOBS | TCOBS |
| [ringB_di_xtea_cobs_rtt32__de_cobs_ua](../../_test/ringB_di_xtea_cobs_rtt32__de_cobs_ua) | RTT32, XTEA + COBS | COBS |
| [ringB_di_xtea_cobs_rtt32__de_tcobs_ua](../../_test/ringB_di_xtea_cobs_rtt32__de_tcobs_ua) | RTT32, XTEA + COBS | TCOBS |
| [ringB_di_xtea_cobs_rtt32__de_xtea_cobs_ua](../../_test/ringB_di_xtea_cobs_rtt32__de_xtea_cobs_ua) | RTT32, XTEA + COBS | XTEA + COBS |

### Direct stack and static buffers

These have no deferred UART path. “Big-endian” selects wire transfer order on the native host; it does not turn the machine into a big-endian CPU.

| Package | Variation checked |
| --- | --- |
| [stackB_di_nopf_aux8](../../_test/stackB_di_nopf_aux8) | Stack buffer, unframed auxiliary byte writer. |
| [stackB_di_nopf_aux32](../../_test/stackB_di_nopf_aux32) | Stack buffer, unframed auxiliary word writer. |
| [stackB_di_nopf_rtt8](../../_test/stackB_di_nopf_rtt8) | Stack buffer, unframed RTT byte writer. |
| [stackB_di_nopf_rtt32](../../_test/stackB_di_nopf_rtt32) | Stack buffer, unframed RTT word writer. |
| [stackB_di_xtea_cobs_rtt8](../../_test/stackB_di_xtea_cobs_rtt8) | Stack buffer, XTEA + COBS, RTT byte writer. |
| [staticB_di_nopf_aux8](../../_test/staticB_di_nopf_aux8) | Static buffer, unframed auxiliary byte writer. |
| [staticB_di_nopf_aux32](../../_test/staticB_di_nopf_aux32) | Static buffer, unframed auxiliary word writer. |
| [staticB_di_nopf_rtt8](../../_test/staticB_di_nopf_rtt8) | Static buffer, unframed RTT byte writer. |
| [staticB_di_nopf_rtt32](../../_test/staticB_di_nopf_rtt32) | Static buffer, unframed RTT word writer. |
| [staticB_di_tcobs_rtt8](../../_test/staticB_di_tcobs_rtt8) | Static buffer, TCOBS, RTT byte writer. |
| [staticB_di_tcobs_rtt32](../../_test/staticB_di_tcobs_rtt32) | Static buffer, TCOBS, RTT word writer. |
| [staticB_di_xtea_cobs_rtt32](../../_test/staticB_di_xtea_cobs_rtt32) | Static buffer, XTEA + COBS, RTT word writer. |
| [be_staticB_di_xtea_cobs_rtt32](../../_test/be_staticB_di_xtea_cobs_rtt32) | Same encrypted static-buffer path with big-endian transfer order. |

### Special configurations

These use custom assertions or add an important independent variation. Some folder names are historical: the alias/user-print headers actually select **stack, direct, unframed auxiliary32**, not the double-buffer/deferred/TCOBS combination suggested by their names.

| Package | What it checks |
| --- | --- |
| [be_dblB_de_tcobs_ua](../../_test/be_dblB_de_tcobs_ua) | Shared expectations with big-endian transfer order, double buffer, deferred TCOBS/UART A. |
| [dblB_de_protect_tcobs_ua](../../_test/dblB_de_protect_tcobs_ua) | Deliberately small double buffer: already accepted records survive, and the next record that cannot safely fit is suppressed. |
| [ringB_de_protect_tcobs_ua](../../_test/ringB_de_protect_tcobs_ua) | Corresponding protected ring-buffer overflow case, with its own capacity/transfer behavior. |
| [modify_for_debug](../../_test/modify_for_debug) | Small dedicated double-buffer fixture emits the same three-byte buffer twice and checks both decoded records. |
| [stackB_di_nopf_aux32_specific](../../_test/stackB_di_nopf_aux32_specific) | Dedicated legacy F-call fixture for 8/16/32/64-bit arrays and stamp spellings; checks line and bulk decoding. |
| [alias_dblB_de_tcobs_ua](../../_test/alias_dblB_de_tcobs_ua) | User logging aliases, including wrapper macros, preserve their expected decoded output. |
| [aliasassert_dblB_de_tcobs_ua](../../_test/aliasassert_dblB_de_tcobs_ua) | Custom assertion wrappers, condition text, file/line and formatted messages. |
| [userprint_dblB_de_tcobs_ua](../../_test/userprint_dblB_de_tcobs_ua) | Variadic user `print` wrapper produces the expected Trice records. |
| [abc_rx_host](../../_test/abc_rx_host) | Real C ABC parser, resolver and dispatch behavior, with synthetic input records. |
| [abc_tx_host](../../_test/abc_tx_host) | ABC transmit macro wire bytes, captured by custom write hooks; no receiver table or full Trice runtime is needed. |

### ABC case details

The receiver's [C fixture](../../_test/abc_rx_host/triceAbcReceiveTest.c) returns a failure count to Go for each case:

| Case | Check |
| --- | --- |
| 1 | No-payload command with a 16-bit stamp; logical record length excludes padding. |
| 2 | 16-bit payload with a 32-bit stamp, including unaligned input. |
| 3 | 32-bit payload without a stamp. |
| 4 | 64-bit payload with a 16-bit stamp; padding is excluded from consumed length. |
| 5 | Unknown ABC ID is consumed without calling a handler. |
| 6 | Selector 0 is rejected when counted X0 support is disabled. |
| 7 | Payload length not aligned to its 32-bit width is rejected before dispatch. |
| 8 | Truncated selected record is rejected before dispatch. |
| 9 | Nested dispatch preserves the outer handler's stamp/context. |
| 10 | Long-count byte payload and logical length without appended padding. |
| 11 | Default bit width for log names without an explicit width. |
| 12 | No-width ABC command names remain payload-free. |
| 13 | Explicit log widths override the no-width default. |
| 14 | Unknown log IDs, truncated input and wrong parameter counts; no ABC dispatch side effects. The successful known-ID consumed length is currently not asserted. |
| 15 | A stream caller advances by the consumed length; one receive call handles one record. |

The transmitter's [C fixture](../../_test/abc_tx_host/triceAbcTransmitTest.c) compares exact bytes, including headers and padding:

| Case | Macro and variation |
| --- | --- |
| 1 | `triceC`: no payload, no stamp. |
| 2 | `TriceC`: no payload, 16-bit stamp. |
| 3 | `TRiceC`: no payload, 32-bit stamp. |
| 4 | `trice8C`: three payload bytes, no stamp. |
| 5 | `Trice16C`: two 16-bit values, 16-bit stamp. |
| 6 | `TRice32C`: one 32-bit value, 32-bit stamp. |

The nine-node [ABC demo](../../examples/TriceAbc/demo.sh) supplies separate process-level integration evidence in step 515: all receivers become ready before transmission, remain alive until requests/replies are drained, and are reaped on failure or interruption. Its start markers are private to each run. The CGO receiver/transmitter cases isolate wire/runtime behavior without that process orchestration.

## CGO build cache

Go's build cache does not reliably notice arbitrary C files included from outside a CGO package. [pc_cache_overlay.go](../../scripts/pc_cache_overlay.go) makes those inputs visible without flushing cached Go dependencies.

Before compilation it hashes file names and contents in `src`, `_test/testdata`, the configuration directory and the applicable Bind sidecar directory. It also records workflow and compiler/header-search settings. It appends that signature to temporary copies of the CGO Go sources and supplies a Go overlay mapping them to their package locations. Shared harness copies are overlaid with the current canonical files. The ABC bridges participate too.

A shared header change invalidates affected packages; a local configuration change stays local. Added/removed/renamed files change the signature; timestamps alone do not. Overlay JSON and stamped copies remain beside the run logs in `overlay.json` and `cache-inputs/`. Temporary Go source names start with `_` so ordinary Go discovery ignores them.

Each selected package still executes with `-count=1`. Reusing a compiled binary is useful; reusing a previous test result would not prove execution in this run. Unreadable repository inputs or symlinks in the scanned trees stop overlay preparation rather than silently using stale binaries.

The overlay does not hash independently changed external libraries or SDK headers at the same external path. After those changes, or when using direct CGO commands without the worker, use an explicit `go clean -cache` when stale external C inputs are suspected. The Insert/Clean `-cache` feature is unrelated.

[pc_cache_overlay_test.go](../../scripts/pc_cache_overlay_test.go) verifies cold/warm **real CGO builds**, changed source/header/sidecar/configuration/compiler settings, unchanged-package reuse, and actual test-execution counts. When comparing machines, use the same commit, compiler, selection and job limits, and distinguish tool setup, cold compilation, warm compilation and execution time.

## How the suite is assembled

[testAll.sh](../../scripts/testAll.sh) starts [the runner](../../scripts/_110_test_runner.sh), using an available sleep inhibitor. The runner captures initial input bytes, takes the checkout lock, prepares declared tools, builds one ordered plan and records every result. [testcheck](../../scripts/testcheck) audits Go JSON events and writes the final report. Unknown skips, missing required tests and failed restoration prevent PASS.

Preparation exposes the report helper's path only after its build succeeds and the file is nonempty and executable. A failed or partial build cannot be used by later checks. `TestReportHelperPublication` in [portability_test.go](../../scripts/portability_test.go) checks missing, partial and empty build output, plus successful publication, through both standalone preparation and the runner. Its runner success case executes one real fixture step and verifies the report; failure cases verify that neither the step nor the broken helper runs.

The numbered files are stable entry points. The table maps each current suite step to its main responsibility; selection differences are listed only in [the usage chapter](target_tests.md#what-full-and-quick-cover).

| Step | Responsibility |
| --- | --- |
| 400 | shfmt shell formatting. |
| 410 | ShellCheck diagnostics. |
| 420 | Unwanted `.DS_Store` files. |
| 430 | C/C++ formatting with the pinned clang-format. |
| 440 | Manual contents/heading formatting with mdtoc. |
| 450 | Markdown linting. |
| 460 | GoReleaser configuration check. |
| 470 | GitHub workflow validation with Actionlint. |
| 480 | Build the current Trice tool. |
| 490 | Native runtime/compiler preparation checks. |
| 500 | Bind behavior and compiler integration. |
| 510 | Public Bind workflows and transactional restoration. |
| 515 | Productive CE/SL target-to-decoder tests and feature/example integration. |
| 520 | Exhaustive target-code analysis with Cppcheck. |
| 540 | Go formatting and package tests. |
| 550 | Go tests with coverage and coverage summary. |
| 560 / 570 | Clang example builds with Insert / Bind. |
| 580 / 590 / 600 | GCC examples with logging off / Insert / Bind. |
| 620 | All L432 firmware configurations. |
| 630 / 640 | GCC PC/CGO configurations with Insert / Bind. |
| 645 | Clang PC/CGO configurations for the selected ID workflows. |

Step 530 (links) and 610 (snapshot packages) remain separately runnable checks, as explained in [release testing](target_tests.md#goreleaser-and-release-package-testing).

### Feature checks and firmware matrices

Step 515 selects the compiler-to-decoder CE/SL tests for C/C++, text/JSON/KV, structured fields, stamps, exactly-once argument evaluation, disabled logging and reversible Insert/Clean. It also checks the PC feature tour, G0B1 firmware artifacts, ABC demo and CLion build through public scripts. Its isolated checkout copies contain current tracked source bytes, including uncommitted edits; existing build directories and captures are not reused. Failed copies remain under `temp/log/logging-features.*`.

`TestContextEnrichmentPoC` and `TestContextEnrichmentPoCRebaseScopeBoundary` retain narrowly scoped compiler proofs. The larger experimental `TestContextEnrichmentRebasePoC` is explicitly excluded from ordinary suite Go/coverage runs; it does not establish productive Rebase/wrapper support. Run that investigation separately with `TRICE_BIND_INTEGRATION=1 go test ./internal/id -run '^TestContextEnrichmentRebasePoC$' -count=1 -v`. Available compiler variants and skips must be read as such. See [CE testing](context_enrichment.md#test-coverage) for the feature's limits.

The L432 wrapper prepares Bind once, then builds configurations 0–100 in separate fresh object directories. It keeps all code-generation options and ELF/HEX/BIN targets, while disabling only expensive assembler text listings (`GCC_LISTINGS=0`). Each configuration uses `make -j1`; the outer job budget limits total concurrency. Normal `build.sh CONFIGURATION=N` still creates listings for manual inspection.

Compiler warnings/errors and `temp/log/l432.*/config-N.log` are retained. Successful temporary build outputs are removed; failed/interrupted outputs remain next to the logs. Started jobs finish or are terminated before source restoration. The managed wrapper restores initial state; directly running `examples/L432_inst/all_configs_build.sh` leaves its prepared Bind state in place.

## Add or change a test

For a shared C behavior case, add a call and `//exp:` expectation in [triceCheck.c](../../_test/testdata/triceCheck.c). Keep the source expression and expected decoded value easy to compare. Include the relevant boundary/error case, then run a representative managed matrix followed by the affected full configuration set.

For a new configuration:

1. Copy the closest package under `_test` with a descriptive name.
2. Adjust `triceConfig.h` and matching decoder options in `cgo_test.go` together.
3. Use a separate `TargetActivity.c`/test function only when the shared expectation loop cannot express the behavior.
4. Run the managed full wrapper: its package discovery includes the new package automatically. Check both compiler families and the applicable ID workflows.
5. Add the package and its unique purpose to the catalogue above. If deliberate legacy copy refresh should include it, also update the folder list in `_330_renew_ids_and_refresh_tests.sh`.

For harness changes, edit `testdata/cgoPackage.go` or `testdata/cgoPackage_test.go`; normal workers use them immediately through overlays. Do not edit the generated copies or run ID reseeding merely to execute tests. Ordinary Bind maintenance uses `trice_bindIDs_in_examples_and_test_folder.sh` and retains history. `_330_renew_ids_and_refresh_tests.sh` is deliberate legacy Clean/Insert/Clean maintenance; without `keepHistory` it discards shared TIL/LI history. Neither maintenance command belongs beside an active suite.

For host behavior, add tests beside the relevant Go implementation. For orchestration/setup, use the isolated script fixtures so failure and interruption can be exercised without changing a developer's installed tools. Keep actual compiler/file/network integration separately identifiable, assert the result rather than just executing a line, and document any genuinely platform-specific exception in the event auditor.

Shared shell helpers and their fixtures must also work with macOS Bash 3.2. For example, expanding an empty array under `set -u` can fail there while succeeding with modern Bash. Give runner success fixtures a nonempty plan and assert that its steps ran. Give compiler cases separate executable paths, so Windows never has to overwrite a just-executed program.

After changing shared test infrastructure, check its direct callers, suite, CI and release hooks, then run the complete affected Go packages after the final edit. Use native Windows, Linux and macOS checks for the same revision and record each result separately. Mocked OS selection and a passing test on one host do not establish a pass on the other hosts. See [native acceptance](target_tests.md#native-acceptance-on-all-three-systems) for the CI entry points.
