# Memory Usage and Code Size

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. No Dynamic Memory Management needed](#no-dynamic-memory-management-needed)
* [2. Code Optimization -o3 or -oz (if supported)](#code-optimization--o3-or--oz-if-supported)
* [3. Compiler Independent Setting (a bit outdated)](#compiler-independent-setting-a-bit-outdated)
* [4. Linker Option --split-sections (if supported)](#linker-option---split-sections-if-supported)
* [5. Linker Optimization -flto (if supported)](#linker-optimization--flto-if-supported)
  * [5.1. ARMCC Compiler v5 Linker Feedback](#armcc-compiler-v5-linker-feedback)
  * [5.2. ARMCLANG Compiler v6 Link-Time Optimization](#armclang-compiler-v6-link-time-optimization)
  * [5.3. GCC](#gcc)
  * [5.4. LLVM ARM Clang](#llvm-arm-clang)
  * [5.5. Other IDE´s and compilers](#other-ides-and-compilers)
* [6. Legacy STM32F030 Example Project - Different Build Sizes](#legacy-stm32f030-example-project---different-build-sizes)
  * [6.1. ARMCC compiler v5](#armcc-compiler-v5)
* [7. TRICE_STACK_BUFFER could cause stack overflow with -o0 optimization](#trice_stack_buffer-could-cause-stack-overflow-with--o0-optimization)
* [8. Reproducible F030 comparison](#reproducible-f030-comparison)
  * [8.1. CubeMX baseline and synchronized build settings](#cubemx-baseline-and-synchronized-build-settings)
  * [8.2. Comparison with LTO](#comparison-with-lto)
  * [8.3. Reference without LTO](#reference-without-lto)
  * [8.4. Where the additional bytes go](#where-the-additional-bytes-go)
  * [8.5. Buffer configurations](#buffer-configurations)
  * [8.6. Rebuild and track differences](#rebuild-and-track-differences)
* [9. Recorded memory measurements](#recorded-memory-measurements)
  * [9.1. F030_bare Size](#f030_bare-size)
  * [9.2. F030_inst Size with TRICE_OFF=1](#f030_inst-size-with-trice_off1)
  * [9.3. F030_inst with ring buffer](#f030_inst-with-ring-buffer)
  * [9.4. F030_inst with ring buffer](#f030_inst-with-ring-buffer-1)
  * [9.5. A developer setting, only enabling SEGGER_RTT](#a-developer-setting-only-enabling-segger_rtt)
  * [9.6. A developer setting, only enabling SEGGER_RTT and without deferred output gives after running `./build.sh TRICE_DIAGNOSTICS=0 TRICE_PROTECT=0`:](#a-developer-setting-only-enabling-segger_rtt-and-without-deferred-output-gives-after-running-buildsh-trice_diagnostics0-trice_protect0)
  * [9.7. Settings Conclusion](#settings-conclusion)
  * [9.8. Legacy Trice Space Example (Old Version)](#legacy-trice-space-example-old-version)
  * [9.9. Memory Needs for Old Example 1](#memory-needs-for-old-example-1)
  * [9.10. Memory Needs for Old Example 2](#memory-needs-for-old-example-2)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

Choose RAM and Flash settings separately. Buffer mode, maximum record size and active outputs affect RAM; enabled backends and linker garbage collection affect Flash. See [performance](performance.md) for execution-time tradeoffs.

## 1. <a id="no-dynamic-memory-management-needed"></a>No Dynamic Memory Management needed

Trice's target logging path needs no heap allocator: it does not call `malloc`, `calloc`, `realloc` or `free`. Buffer sizes are selected in the target configuration. Depending on the chosen mode, storage includes static buffers and automatic local buffers on the stack; a stack buffer is not a static object. RAM requirements therefore depend on the configuration, record sizes and buffering strategy; see [Trice memory needs](memory_usage.md).

This statement covers Trice's own target logging code, including its CE adapters. It does not cover the Go host tool, an RTOS, a transport callback, an application's local-formatting sink, or a function invoked by a CE expression. For example, a user-supplied `read_context()` that allocates memory still allocates when called through `-ce`. Choose non-allocating application functions when the complete log call must avoid the heap.

You can compile the complete `src/*.c` library instead of maintaining a list of individual files. With unused-section removal enabled, only the library functions and data needed by your application remain in the final executable or firmware image. Configuration already excludes many disabled features during preprocessing; the linker removes the remaining unreferenced sections. Compiling every source can still take longer than compiling a selected subset.

The PC example scripts enable this explicitly: `-ffunction-sections -fdata-sections` separates functions and data, then `-Wl,--gc-sections` removes unused sections with GCC/Clang on Linux and MinGW on Windows. On macOS they use `-Wl,-dead_strip` instead. For MSVC, use `/Gy /Gw` and `/OPT:REF`. Embedded examples use the corresponding section-removal settings in their build configuration. The minimal demos omit these optional flags to keep their build commands simple. Without these settings, compiling `src/*.c` alone does **not** guarantee that unused code disappears.

Follow the [RTT include-order and configuration rules](rtt.md). The fallback lets PC demos compile without an additional RTT header or J-Link installation; boards can provide their own buffer and locking settings.

## 2. <a id="code-optimization--o3-or--oz-if-supported"></a>Code Optimization -o3 or -oz (if supported)

For debugging it could be helpful to switch off code optimization what increases the code size. A good choice is `-o1`. See also
[TRICE_STACK_BUFFER could cause stack overflow with -o0 optimization](memory_usage.md#trice_stack_buffer-could-cause-stack-overflow-with--o0-optimization).


## 3. <a id="compiler-independent-setting-a-bit-outdated"></a>Compiler Independent Setting (a bit outdated)

Maybe the following is a bit unhandy but it decreases the code amount, build time and the image size.

* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_trice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.
* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_Trice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.
* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_TRice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.

When having lots of program memory simply let all values be `1`. With specific linker optimization unused functions can get stripped out automatically.

It is possible to `#define TRICE_SINGLE_MAX_SIZE 12` for example in *triceConfig.h*. This automaticaly disables all Trice messages with payloads > 8 bytes (Trice size is 4 bytes).

## 4. <a id="linker-option---split-sections-if-supported"></a>Linker Option --split-sections (if supported)

In ARM-MDK uVision `Project -> Options -> C/C++ -> "One EFL section for each function"` allows good optimization and getting rid of unused code without additional linker optimization. This leads to a faster build process and is fine for most cases. It allows excluding unused functions.

## 5. <a id="linker-optimization--flto-if-supported"></a>Linker Optimization -flto (if supported)

* To get the smallest possible image, do _not_ use option `--split sections`.
* Use linker optimization alone.
* This increases the build time but reduces the image size significantly.

### 5.1. <a id="armcc-compiler-v5-linker-feedback"></a>ARMCC Compiler v5 Linker Feedback

* In ARM-MDK uVision, when using ARMCC compiler v5, there is a check box `Project -> Options -> Target -> "Cross Module Optimization"`.
* In ARMCC this works also with the lite version.

### 5.2. <a id="armclang-compiler-v6-link-time-optimization"></a>ARMCLANG Compiler v6 Link-Time Optimization

* In ARM-MDK uVision, when using ARMCLANG compiler v6, the check box `Project -> Options -> C/C++(AC6) -> "Link-Time Optimization"` is usable to set the CLI `-flto` switch.
* LTO is not possible with ARMCLANG6 lite: https://developer.arm.com/documentation/ka004054/latest.

### 5.3. <a id="gcc"></a>GCC

With GCC use the `-flto` CLI switch directly.

### 5.4. <a id="llvm-arm-clang"></a>LLVM ARM Clang

This compiler is much faster and creates the smallest images. Right now it uses the GCC libs and linker.

### 5.5. <a id="other-ides-and-compilers"></a>Other IDE´s and compilers

Please check the manuals and create a pull request or simply let me know.

## 6. <a id="legacy-stm32f030-example-project---different-build-sizes"></a>Legacy STM32F030 Example Project - Different Build Sizes

### 6.1. <a id="armcc-compiler-v5"></a>ARMCC compiler v5

| Compiler | Linker         | Result                                          | Comment                           |
| -------- | -------------- | ----------------------------------------------- | --------------------------------- |
| o0       |                | Code=46942 RO-data=266 RW-data=176 ZI-data=4896 | very big                          |
| o1       |                | Code=22582 RO-data=258 RW-data=168 ZI-data=4896 |                                   |
| o3       |                | Code=21646 RO-data=258 RW-data=168 ZI-data=4896 |                                   |
| o0       | split sections | Code= 7880 RO-data=268 RW-data=156 ZI-data=4892 | for debugging                     |
| o1       | split sections | Code= 5404 RO-data=260 RW-data=148 ZI-data=4892 | **for debugging**                 |
| o3       | split sections | Code= 4996 RO-data=260 RW-data=148 ZI-data=4892 | **good balance**                  |
| o0       | flto           | Code= 8150 RO-data=266 RW-data=176 ZI-data=4896 | builds slower                     |
| o1       | flto           | Code= 5210 RO-data=258 RW-data=148 ZI-data=4892 | builds slower                     |
| o3       | flto           | Code= 4818 RO-data=258 RW-data=148 ZI-data=4892 | builds slower, **smallest image** |

## 7. <a id="trice_stack_buffer-could-cause-stack-overflow-with--o0-optimization"></a>TRICE_STACK_BUFFER could cause stack overflow with -o0 optimization

As discussed in [issue #294](https://github.com/rokath/trice/issues/294) it can happen, that several TRICE macros within one function call increase the stack usage more than expected, when compiler optimization is totally switched off.

## 8. <a id="reproducible-f030-comparison"></a>Reproducible F030 comparison

**For small projects, about 2 KB Flash and 200 B RAM can be enough for Trice instrumentation.** This assumes small buffers and a compact output configuration. The Trice kernel itself can fit in roughly 1 KB Flash; each log call adds a few Flash bytes for the call and argument handling.

For a rough buffer estimate, assume about 8 B per typical message. Even a 64 B buffer may suffice for short bursts if the output drains it fast enough. Framing, buffer bookkeeping, transport state and automatic stack storage also consume RAM and must fit the chosen budget. These are planning values for a suitably configured small project; the 1 KB RTT and ring buffers used below are not minimum requirements.

Compare each instrumented example with `F030_bare` from the same compiler, optimization and LTO series. The differences include the three log calls, transport and required integration in these examples; they are not a universal Trice overhead.

### 8.1. <a id="cubemx-baseline-and-synchronized-build-settings"></a>CubeMX baseline and synchronized build settings

`F030_bare` retains the CubeMX-generated application: it initializes the clock, GPIO and USART2 even without Trice. Its absolute size therefore includes those drivers. When comparing a newly generated project, match the MCU, initialization and linker reservations first.

The three comparison Makefiles deliberately share these changes to the generated build settings:

* LTO is enabled by default with `LTO=1`; use `LTO=0` for the reference without LTO. `-flto` and `-Og` are passed during compilation and linking.
* `-ffunction-sections -fdata-sections` are also repeated at link time. GCC's LTO code generation needs them to preserve removal of unused functions and data with `--gc-sections`.
* The existing `-g -gdwarf-2` debug options are also passed at link time so GCC's LTO stage keeps the same debug format instead of defaulting to DWARF 5.
* `TRICE_FLAGS` accepts the same optional framing definition in all three projects. It has no effect on bare or direct RTT. GCC assembly listings can be disabled through `LISTING_FLAGS=` for Clang's integrated assembler; assembly files do not receive `-flto`.

Each linker script also defines `_estack = ORIGIN(RAM) + LENGTH(RAM)` **after** its `MEMORY` block. GNU ld accepts CubeMX's earlier forward reference; LLD requires the RAM region to exist first. This reorder changes no region address, length, section placement or heap/stack reservation. Reapply it to all three projects after CubeMX regeneration when using LLD.

The `._user_heap_stack` section is explicitly `(NOLOAD)`. It reserves RAM without storing initialization bytes in the ELF load image. GNU ld already treated this generated section as uninitialized; LLD otherwise creates a load image for it. This preserves the same 512 B heap and 1024 B stack minima for both linkers.

The generated `.ioc`, application C files and startup assembly are unchanged by this comparison setup. Keep the common Makefile/linker settings synchronized when regenerating or changing any of the three projects. The default remains GCC with `-Og`; Clang is selected explicitly through the measurement command below. Clang/LLD use the same pinned GNU ARM startup objects and Newlib-nano/libgcc libraries as GCC.

Clean and rebuild all three projects when switching compiler, framing or LTO manually; Make does not detect changes to command-line flags in existing object files. The measurement runner always builds fresh snapshots.

### 8.2. <a id="comparison-with-lto"></a>Comparison with LTO

Measured on Windows with GCC 15.2.1 and Clang/LLD 23.1.3, at `-Og` with LTO and section garbage collection. Each delta uses bare from the same compiler. Static RAM excludes the linker heap/stack reserve and automatic stack buffers.

Compiler | Example / output | Delta Flash [B] | Delta static RAM [B] | Flash [B] | Static RAM [B]
-------- | ---------------- | --------------: | -------------------: | --------: | -------------:
GCC      | bare             | 0               | 0                    | 2372      | 32
GCC      | direct RTT       | 1008            | 1132                 | 3380      | 1164
GCC      | UART / TCOBS     | 2616            | 1080                 | 4988      | 1112
GCC      | UART / COBS      | 1292            | 1080                 | 3664      | 1112
Clang    | bare             | 0               | 0                    | 2288      | 32
Clang    | direct RTT       | 1028            | 1132                 | 3316      | 1164
Clang    | UART / TCOBS     | 2256            | 1076                 | 4544      | 1108
Clang    | UART / COBS      | 1124            | 1076                 | 3412      | 1108

COBS saves 1324 B Flash with GCC and 1132 B with Clang in these UART configurations. The linked encoder sizes are 1400/78 B for TCOBS/COBS with GCC and 1230/100 B with Clang; section alignment accounts for another 2 B of each difference. Clang LTO eliminates the 4 B `TriceErrorCount` object, explaining its smaller static RAM increase. Bare and RTT have identical sizes across the two framing builds for each compiler.

Enabling GCC LTO changes bare from 2376 to 2372 B Flash; static RAM remains 32 B. The UART/TCOBS ELF changes from 5020 to 4988 B. The GCC encoder itself remains 1400 B at `-Og`. Keeping function/data sections during LTO linking is essential: otherwise unused syscall stubs remain together and pull in another 396 B of Newlib state in bare.

These measurements use base commit `64bd22439a1d76c4e8c073e6fbe5c6c208ff4e15` plus the synchronized Makefile/linker changes, recorded as source-patch SHA-256 `16c3c108ff8a45e275b71d6d5fb371e0e7065b9f095a42625669997b9c01e086`. Each series records its compiler, linker, ARM runtime, source patch and ELF hashes. CI uses the existing Linux Clang/LLD 23.1.4 pin; those results belong to that compiler version rather than the Windows 23.1.3 record above.

MemBrowse 1.2.18 matches the Flash/RAM region totals and encoder symbol sizes, but its **GCC-LTO source-file labels can be wrong**. For example, it labels `TCOBSEncode()` as `stm32f0xx_ll_exti.c`; `arm-none-eabi-addr2line` correctly resolves that address to `tcobsv1Encode.c:77`. This persists with consistent DWARF 2. MemBrowse's declaration lookup can use the current compilation unit's file table for a declaration referenced from another unit. Use symbol names/sizes for this GCC-LTO comparison and verify file ownership with binutils; the measured Clang encoder file labels are correct. This limitation does not change the table's byte counts.

### 8.3. <a id="reference-without-lto"></a>Reference without LTO

The following table records the initial GCC comparison before LTO was enabled:

Example        | Delta Flash [B] | Delta static RAM [B] | Flash [B] | Static RAM [B]
-------------- | --------------: | -------------------: | --------: | -------------:
F030_bare      | +0              | +0                   | 2376      | 32
F030_inst_rtt  | +1012           | +1132                | 3388      | 1164
F030_inst_uart | +2644           | +1080                | 5020      | 1112

UART uses 1632 B more Flash and 52 B less static RAM than RTT. RTT also needs an automatic record buffer on the stack, which the static RAM column does not measure.

Measured on Windows at commit `64bd22439a1d76c4e8c073e6fbe5c6c208ff4e15`, with xPack GNU Arm Embedded GCC 15.2.1 (`15.2.1-1.1`), `-Og`, `-ffunction-sections`, `-fdata-sections` and `--gc-sections`. All three ELFs were built fresh from the same committed sources, without `triceCheck.c`. Local MemBrowse 1.2.18 analysis of those ELFs, maps and linker scripts confirmed the region totals below.

Flash counts loadable section bytes at their Flash load addresses, including the initialization image of `.data`. Static RAM counts allocated RAM sections excluding `._user_heap_stack`. Debug information is excluded. All examples reserve 512 B heap and 1024 B stack; RTT has another 4 B of alignment in that section. These reservations are excluded from the table. MemBrowse's RAM-region total includes them: 1568 B bare, 2704 B RTT and 2648 B UART. Its RTT RAM difference is therefore 1136 B, including the 4 B alignment difference.

### 8.4. <a id="where-the-additional-bytes-go"></a>Where the additional bytes go

The retained input sections in the linker maps account for every additional Flash byte in the reference without LTO below, including constants, initialized variables and alignment. This reference uses `-Og` and section garbage collection.

Flash contribution above bare                             | RTT [B]  | UART [B]
--------------------------------------------------------- | -------: | -------:
TCOBS framing encoder (`tcobsv1Encode.c`)                 | 0        | 1400
Ring buffering and deferred framing (`triceRingBuffer.c`) | 0        | 590
Record-writing functions (`trice32.c`)                    | 160      | 304
Trice core (`trice.c`)                                    | 238      | 50
SEGGER RTT library (`SEGGER_RTT.c`)                       | 552      | 0
Trice UART backend (`triceUart.c`)                        | 0        | 196
Log/transfer calls in `main.c`                            | 36       | 40
USART2 interrupt handler (`stm32f0xx_it.c`)               | 0        | 26
C library: `memcpy` for RTT, `memmove` for UART           | 18       | 38
Alignment                                                 | 8        | 0
**Total**                                                 | **1012** | **2644**

`TCOBSEncode()` alone occupies 1400 B, about 53% of the UART Flash increase. Deferred output inherits TCOBS framing and multi-pack mode. The ring-buffer object contributes 590 B in total, including transfer, framing coordination, diagnostics and 16 B of constants/initialized pointers. Its largest functions are `triceMultiDeferredOut()` (184 B), `TriceIDAndBuffer()` (148 B) and `triceTransferMultiFraming()` (96 B).

TCOBSv1 tracks runs of zero bytes, `0xFF` bytes and other repeated bytes, plus the current block offset. It has separate paths for the main loop, run limits and the last two bytes. The encoder expands `OUTB` 12 times, zero/FF output six times each and repeat output four times. Its `ASSERT` macro is empty, so assertions and diagnostic strings contribute no bytes. The Cortex-M0 `-Og` disassembly contains 700 Thumb instructions, including 59 conditional branches and 68 explicit stack load/store instructions.

The decoder is not linked into this firmware: `.text.TCOBSDecode` (336 B) and `.text.COBSDecode` (62 B) appear under `Discarded input sections` in the reference map and are absent from the final ELF symbol table. The full 1400 B belongs to `TCOBSEncode()` itself, also confirmed by compiling that encoder alone.

Isolated encoder objects built with GCC 15.2.1, Cortex-M0/Thumb, function/data sections and no LTO show the optimization effect separately from the rest of the firmware:

Encoder       | `-Og` [B] | `-Os` [B]
------------- | --------: | --------:
`TCOBSEncode` | 1400      | 936
`COBSEncode`  | 78        | 70

The smaller COBS encoder has one short loop for zero-delimited blocks; it does not implement TCOBS's repeated-byte compression. This comparison keeps firmware builds at `-Og`; `-Os` above is an isolated diagnostic, not a different setting for one of the three projects.

The vendor USART driver contributes 216 B in **each** ELF, already present in bare. Clock/GPIO drivers and the board's UART initialization are also unchanged. The UART increase therefore comes from Trice's encoding and transport path, including the interrupt-handler extension, rather than additional vendor driver code.

Additional static RAM above bare        | RTT [B]  | UART [B]
--------------------------------------- | -------: | -------:
RTT up buffer / Trice ring allocation   | 1024     | 1024
RTT down buffer                         | 16       | 0
RTT control block (`_SEGGER_RTT`)       | 72       | 0
Ring-buffer state, seven 32-bit words   | 0        | 28
Trice core counters, four 32-bit words  | 16       | 16
Stack-buffer write-position pointer     | 4        | 0
UART transmit state, three 32-bit words | 0        | 12
**Total**                               | **1132** | **1080**

These RAM differences are the named objects in the local MemBrowse reports. The common bare RAM is unchanged. The automatic RTT record buffer and linker reservations remain separate as described above.

### 8.5. <a id="buffer-configurations"></a>Buffer configurations

Example        | Output                         | Buffer storage
-------------- | ------------------------------ | ----------------------------------------------------------------------------------------
F030_bare      | No Trice                       | No Trice buffers
F030_inst_rtt  | Direct RTT, 32-bit writes      | 1024 B RTT up buffer, 16 B down buffer, one channel each; automatic 172 B record buffer
F030_inst_uart | Deferred USART2, multi framing | 1024 B static ring allocation, including 64 B offset; `TriceTransfer()` in the main loop

Both instrumented examples inherit `TRICE_SINGLE_MAX_SIZE=104`, `TRICE_DATA_OFFSET=64`, diagnostics, protection and the cycle counter from the defaults. RTT's configured automatic record buffer is `64 + 104 + 4 = 172 B`; the dynamic stack maximum, including call frames and interrupts, needs a separate stack analysis. UART does not link RTT buffers. See the project configurations for [RTT](../../examples/F030_inst_rtt/Core/Inc/triceConfig.h) and [UART](../../examples/F030_inst_uart/Core/Inc/triceConfig.h).

### 8.6. <a id="rebuild-and-track-differences"></a>Rebuild and track differences

From the repository root, with Go, Python 3, Git, tar, GNU Make and a POSIX shell available:

```sh
arm_bin="$(go run ./scripts/testcheck bundle scripts/test-tools.env temp/tools/bundles arm)"
if command -v cygpath >/dev/null 2>&1; then arm_bin="$(cygpath -u "$arm_bin")"; fi
PATH="$arm_bin:$PATH" python3 scripts/memory_usage.py
```

The command selects the existing pinned ARM package and rebuilds **committed `HEAD`** in a temporary snapshot, including its Trice CLI. Uncommitted source changes are excluded. For a local measurement of tracked edits, add `--worktree`; the JSON and summary then record their patch hash. Untracked source files are excluded. The synchronized Makefiles/linker scripts must be committed or included through `--worktree`; the script rejects build plans that omit the requested options.

Select the compiler and framing explicitly to repeat all four CI series:

```sh
python3 scripts/memory_usage.py --compiler gcc --framing tcobs
python3 scripts/memory_usage.py --compiler gcc --framing cobs
python3 scripts/memory_usage.py --compiler clang --framing tcobs
python3 scripts/memory_usage.py --compiler clang --framing cobs
```

For Clang, first select the native pinned Clang and matching LLD from the [test tool setup](target_tests.md). Windows uses WinLibs 23.1.3; Linux/macOS use 23.1.4. Clang also requires the pinned ARM package for headers, startup objects and libraries. The runner passes the same CPU, optimization, LTO and framing options to all three projects; it selects Clang's integrated assembler and LLD only for Clang builds. To reproduce a reference without LTO, add `--lto off` to the same command.

The example directories remain untouched. Each series writes its table, JSON metadata, ELF hashes, section/symbol listings, build plans, ELF files, maps and linker scripts to its own directory, for example `temp/memory-usage/gcc-tcobs-lto/`. Flash and RAM are calculated from `arm-none-eabi-objdump -h`; the JSON records the exact compiler and commit.

The [F030 memory workflow](../../.github/workflows/membrowse.yml) builds all three projects in each of the four compiler/framing combinations. It runs on pushes to `main`, pull requests and manual runs, publishing `f030-memory-comparison-<compiler>-<framing>` artifacts for reviewing and updating this table; it does not commit documentation. Eight stable MemBrowse targets use those same ELFs, maps and linker scripts:

Example / output | GCC target                     | Clang target
---------------- | ------------------------------ | --------------------------------
bare             | `F030_bare_gcc_lto`            | `F030_bare_clang_lto`
direct RTT       | `F030_inst_rtt_gcc_lto`        | `F030_inst_rtt_clang_lto`
UART / TCOBS     | `F030_inst_uart_gcc_lto_tcobs` | `F030_inst_uart_clang_lto_tcobs`
UART / COBS      | `F030_inst_uart_gcc_lto_cobs`  | `F030_inst_uart_clang_lto_cobs`

Bare and RTT are uploaded once per compiler because deferred framing does not change them. Their builds and comparison rows remain in both framing artifacts. View the [Trice MemBrowse project](https://membrowse.com/project/09c7019d-4ca6-4cc0-9d89-7d813d5f526c).

Pushes and manual runs use the repository secret `MEMBROWSE_API_KEY`. Without it, the comparison artifact remains available and the upload is skipped. Public-repository PRs use MemBrowse's tokenless upload; this requires marking the project as open source in the MemBrowse Portal. No PR build receives the project key. Same-repository PRs also receive a combined comment from a separate job; fork PRs retain their reports and artifacts without a write token.

## 9. <a id="recorded-memory-measurements"></a>Recorded memory measurements

These measurements describe the firmware, compiler and configuration shown in each record. They are reference examples, not current size guarantees. Rebuild your own configuration to measure it.

Depending on your target configuration the needed space can differ:

### 9.1. <a id="f030_bare-size"></a>F030_bare Size

* `./build.sh`:

```bash
arm-none-eabi-size build/F030_bare.elf
   text    data     bss     dec     hex filename
   2428      12    1564    4004     fa4 build/F030_bare.elf
```

That is the basic size of an empty generated project just containing some drivers.

### 9.2. <a id="f030_inst-size-with-trice_off1"></a>F030_inst Size with TRICE_OFF=1

* `./build.sh TRICE_OFF=1` :

```bash
arm-none-eabi-size build/F030_inst.elf
   text    data     bss     dec     hex filename
   2428      12    1564    4004     fa4 build/F030_inst.elf
```

This is exactly the same result, proofing that `TRICE_OFF 1` is working correctly.

### 9.3. <a id="f030_inst-with-ring-buffer"></a>F030_inst with ring buffer

* `./build.sh`:

```bash
arm-none-eabi-size out/F030_inst.elf
   text    data     bss     dec     hex filename
   9416      28    2692   12136    2f68 out/F030_inst.elf
```

This is about 7 KB Flash and 1.2 KB RAM size for the Trice library and we see:

```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice_wt_devel/examples/F030_inst (devel)
$ trice l -p com5 -ts16 "time:     #%6d" -hs off
com5:       triceExamples.c    12      # 65535  Hello! 👋🙂
com5:
com5:         ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
com5:         🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-F030R8   🎈🎈🎈🎈
com5:         🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃
com5:
com5:
com5:       triceExamples.c    61              TRICE_DIRECT_OUTPUT == 0, TRICE_DEFERRED_OUTPUT == 1
com5:       triceExamples.c    67              TRICE_DOUBLE_BUFFER, TRICE_MULTI_PACK_MODE
com5:       triceExamples.c    76              _CYCLE == 1, _PROTECT == 1, _DIAG == 1, XTEA == 0
com5:       triceExamples.c    77              _SINGLE_MAX_SIZE=104, _BUFFER_SIZE=172, _DEFERRED_BUFFER_SIZE=1024
com5:       triceExamples.c    29    0,031_804 🐁 Speedy Gonzales a  32-bit timestamp
com5:       triceExamples.c    30    0,031_646 🐁 Speedy Gonzales b  32-bit timestamp
com5:       triceExamples.c    31    0,031_488 🐁 Speedy Gonzales c  32-bit timestamp
com5:       triceExamples.c    32    0,031_330 🐁 Speedy Gonzales d  32-bit timestamp
com5:       triceExamples.c    33      # 31172 🐁 Speedy Gonzales e  16-bit timestamp
com5:       triceExamples.c    34      # 31012 🐁 Speedy Gonzales f  16-bit timestamp
com5:       triceExamples.c    35      # 30852 🐁 Speedy Gonzales g  16-bit timestamp
com5:       triceExamples.c    36      # 30692 🐁 Speedy Gonzales h  16-bit timestamp
com5:       triceExamples.c    42      # 30224 2.71828182845904523536 <- float number as string
com5:       triceExamples.c    43      # 29517 2.71828182845904509080 (double with more ciphers than precision)
com5:       triceExamples.c    44      # 29322 2.71828174591064453125 (float  with more ciphers than precision)
com5:       triceExamples.c    45      # 29145 2.718282 (default rounded float)
com5:       triceExamples.c    46      # 28969 A Buffer:
com5:       triceExamples.c    47      # 28790 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
com5:       triceExamples.c    48      # 28076 31372e32  31383238  34383238  34303935  35333235
...
com5:       triceExamples.c    50              5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
com5:       triceExamples.c    52      # 26554 i=44444400 aaaaaa00
com5:       triceExamples.c    52      # 26342 i=44444401 aaaaaa01
com5:       triceExamples.c    52      # 26130 i=44444402 aaaaaa02
com5:       triceExamples.c    52      # 25918 i=44444403 aaaaaa03
com5:       triceExamples.c    52      # 25706 i=44444404 aaaaaa04
com5:       triceExamples.c    29    0,031_790 🐁 Speedy Gonzales a  32-bit timestamp
com5:       triceExamples.c    30    0,031_632 🐁 Speedy Gonzales b  32-bit timestamp
com5:       triceExamples.c    31    0,031_474 🐁 Speedy Gonzales c  32-bit timestamp
com5:       triceExamples.c    32    0,031_316 🐁 Speedy Gonzales d  32-bit timestamp
com5:       triceExamples.c    33      # 31158 🐁 Speedy Gonzales e  16-bit timestamp
com5:       triceExamples.c    34      # 30998 🐁 Speedy Gonzales f  16-bit timestamp
com5:       triceExamples.c    35      # 30838 🐁 Speedy Gonzales g  16-bit timestamp
com5:       triceExamples.c    36      # 30678 🐁 Speedy Gonzales h  16-bit timestamp
com5:       triceExamples.c    42      # 30210 2.71828182845904523536 <- float number as string
com5:       triceExamples.c    43      # 29503 2.71828182845904509080 (double with more ciphers than precision)
com5:       triceExamples.c    44      # 29308 2.71828174591064453125 (float  with more ciphers than precision)
com5:       triceExamples.c    45      # 29131 2.718282 (default rounded float)
com5:       triceExamples.c    46      # 28955 A Buffer:
com5:       triceExamples.c    47      # 28776 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
com5:       triceExamples.c    48      # 28062 31372e32  31383238  34383238  34303935  35333235
...
com5:       triceExamples.c    50              5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
com5:       triceExamples.c    52      # 26540 i=44444400 aaaaaa00
com5:       triceExamples.c    52      # 26328 i=44444401 aaaaaa01
com5:       triceExamples.c    52      # 26116 i=44444402 aaaaaa02
com5:       triceExamples.c    52      # 25904 i=44444403 aaaaaa03
com5:       triceExamples.c    52      # 25692 i=44444404 aaaaaa04
com5:    triceLogDiagData.c    44              triceSingleDepthMax = 108 of 172 (TRICE_BUFFER_SIZE)
com5:    triceLogDiagData.c    67              TriceHalfBufferDepthMax = 388 of  512
com5:       triceExamples.c    29    0,031_344 🐁 Speedy Gonzales a  32-bit timestamp
com5:       triceExamples.c    30    0,031_186 🐁 Speedy Gonzales b  32-bit timestamp
```

### 9.4. <a id="f030_inst-with-ring-buffer-1"></a>F030_inst with ring buffer

* `./build.sh`:

We need 600 bytes more Flash but could have less RAM used:

```bash
   text    data     bss     dec     hex filename
  10060      24    2688   12772    31e4 out/F030_inst.elf
```

```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice_wt_devel/examples/F030_inst (devel)
$ trice l -p com5 -ts16 "time:     #%6d" -hs off
com5:       triceExamples.c    12      # 65535  Hello! 👋🙂
com5:
com5:         ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
com5:         🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-F030R8   🎈🎈🎈🎈
com5:         🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃
com5:
com5:
com5:       triceExamples.c    61              TRICE_DIRECT_OUTPUT == 0, TRICE_DEFERRED_OUTPUT == 1
com5:       triceExamples.c    69              TRICE_RING_BUFFER, TRICE_MULTI_PACK_MODE
com5:       triceExamples.c    76              _CYCLE == 1, _PROTECT == 1, _DIAG == 1, XTEA == 0
com5:       triceExamples.c    77              _SINGLE_MAX_SIZE=104, _BUFFER_SIZE=172, _DEFERRED_BUFFER_SIZE=1024
com5:       triceExamples.c    29    0,031_732 🐁 Speedy Gonzales a  32-bit timestamp
com5:       triceExamples.c    30    0,031_531 🐁 Speedy Gonzales b  32-bit timestamp
com5:       triceExamples.c    31    0,031_330 🐁 Speedy Gonzales c  32-bit timestamp
com5:       triceExamples.c    32    0,031_129 🐁 Speedy Gonzales d  32-bit timestamp
com5:       triceExamples.c    33      # 30928 🐁 Speedy Gonzales e  16-bit timestamp
com5:       triceExamples.c    34      # 30725 🐁 Speedy Gonzales f  16-bit timestamp
com5:       triceExamples.c    35      # 30522 🐁 Speedy Gonzales g  16-bit timestamp
com5:       triceExamples.c    36      # 30319 🐁 Speedy Gonzales h  16-bit timestamp
com5:       triceExamples.c    42      # 29808 2.71828182845904523536 <- float number as string
com5:       triceExamples.c    43      # 29058 2.71828182845904509080 (double with more ciphers than precision)
com5:       triceExamples.c    44      # 28821 2.71828174591064453125 (float  with more ciphers than precision)
com5:       triceExamples.c    45      # 28602 2.718282 (default rounded float)
com5:       triceExamples.c    46      # 28383 A Buffer:
com5:       triceExamples.c    47      # 28162 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
com5:       triceExamples.c    48      # 27406 31372e32  31383238  34383238  34303935  35333235
...
com5:       triceExamples.c    50              5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
com5:       triceExamples.c    52      # 25757 i=44444400 aaaaaa00
com5:       triceExamples.c    52      # 25502 i=44444401 aaaaaa01
com5:       triceExamples.c    52      # 25247 i=44444402 aaaaaa02
com5:       triceExamples.c    52      # 24992 i=44444403 aaaaaa03
com5:       triceExamples.c    52      # 24737 i=44444404 aaaaaa04
com5:       triceExamples.c    29    0,031_746 🐁 Speedy Gonzales a  32-bit timestamp
com5:       triceExamples.c    30    0,031_545 🐁 Speedy Gonzales b  32-bit timestamp
com5:       triceExamples.c    31    0,031_344 🐁 Speedy Gonzales c  32-bit timestamp
com5:       triceExamples.c    32    0,031_143 🐁 Speedy Gonzales d  32-bit timestamp
com5:       triceExamples.c    33      # 30942 🐁 Speedy Gonzales e  16-bit timestamp
com5:       triceExamples.c    34      # 30739 🐁 Speedy Gonzales f  16-bit timestamp
com5:       triceExamples.c    35      # 30536 🐁 Speedy Gonzales g  16-bit timestamp
com5:       triceExamples.c    36      # 30333 🐁 Speedy Gonzales h  16-bit timestamp
com5:       triceExamples.c    42      # 29822 2.71828182845904523536 <- float number as string
com5:       triceExamples.c    43      # 29072 2.71828182845904509080 (double with more ciphers than precision)
com5:       triceExamples.c    44      # 28835 2.71828174591064453125 (float  with more ciphers than precision)
com5:       triceExamples.c    45      # 28616 2.718282 (default rounded float)
com5:       triceExamples.c    46      # 28397 A Buffer:
com5:       triceExamples.c    47      # 28176 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
com5:       triceExamples.c    48      # 27420 31372e32  31383238  34383238  34303935  35333235
...
com5:       triceExamples.c    50              5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
com5:       triceExamples.c    52      # 25771 i=44444400 aaaaaa00
com5:       triceExamples.c    52      # 25516 i=44444401 aaaaaa01
com5:       triceExamples.c    52      # 25261 i=44444402 aaaaaa02
com5:       triceExamples.c    52      # 25006 i=44444403 aaaaaa03
com5:       triceExamples.c    52      # 24751 i=44444404 aaaaaa04
com5:    triceLogDiagData.c    44              triceSingleDepthMax = 108 of 172 (TRICE_BUFFER_SIZE)
com5:    triceLogDiagData.c    75              triceRingBufferDepthMax = 324 of 1024
com5:       triceExamples.c    29    0,031_188 🐁 Speedy Gonzales a  32-bit timestamp
com5:       triceExamples.c    30    0,030_987 🐁 Speedy Gonzales b  32-bit timestamp
```

### 9.5. <a id="a-developer-setting-only-enabling-segger_rtt"></a>A developer setting, only enabling SEGGER_RTT

* `./build.sh`:

```bash
arm-none-eabi-size out/F030_inst.elf
   text    data     bss     dec     hex filename
   6656      16    2768    9440    24e0 out/F030_inst.elf
```

About 4 KB Flash needed and we see:

```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice_wt_devel/examples/F030_inst (devel)
$ trice l -p jlink -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0" -pf none -d16 -showID "deb:%5d"
Dec  6 16:14:38.276356  jlink:       triceExamples.c    12       65_535 16369  Hello! 👋🙂
Dec  6 16:14:38.276356  jlink:
Dec  6 16:14:38.276356  jlink:         ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
Dec  6 16:14:38.276356  jlink:         🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-F030R8   🎈🎈🎈🎈
Dec  6 16:14:38.276356  jlink:         🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃
Dec  6 16:14:38.276356  jlink:
Dec  6 16:14:38.276356  jlink:
Dec  6 16:14:38.276356  jlink:       triceExamples.c    61              16334 TRICE_DIRECT_OUTPUT == 1, TRICE_DEFERRED_OUTPUT == 0
Dec  6 16:14:38.276356  jlink:       triceExamples.c    63              16333 TRICE_STACK_BUFFER, TRICE_MULTI_PACK_MODE
Dec  6 16:14:38.276920  jlink:       triceExamples.c    76              16327 _CYCLE == 1, _PROTECT == 1, _DIAG == 1, XTEA == 0
Dec  6 16:14:38.277424  jlink:       triceExamples.c    77              16326 _SINGLE_MAX_SIZE=104, _BUFFER_SIZE=172, _DEFERRED_BUFFER_SIZE=1024
Dec  6 16:14:39.181228  jlink:       triceExamples.c    29    0,031_848 16356 🐁 Speedy Gonzales a  32-bit timestamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    30    0,031_292 16355 🐁 Speedy Gonzales b  32-bit timestamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    31    0,030_736 16354 🐁 Speedy Gonzales c  32-bit timestamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    32    0,030_180 16353 🐁 Speedy Gonzales d  32-bit timestamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    33       29_624 16352 🐁 Speedy Gonzales e  16-bit timestamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    34       29_066 16351 🐁 Speedy Gonzales f  16-bit timestamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    35       28_508 16350 🐁 Speedy Gonzales g  16-bit timestamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    36       27_950 16349 🐁 Speedy Gonzales h  16-bit timestamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    42       27_086 16344 2.71828182845904523536 <- float number as string
Dec  6 16:14:39.181228  jlink:       triceExamples.c    43       25_906 16343 2.71828182845904509080 (double with more ciphers than precision)
Dec  6 16:14:39.181798  jlink:       triceExamples.c    44       25_305 16342 2.71828174591064453125 (float  with more ciphers than precision)
Dec  6 16:14:39.181798  jlink:       triceExamples.c    45       24_727 16341 2.718282 (default rounded float)
Dec  6 16:14:39.181798  jlink:       triceExamples.c    46       24_148 16340 A Buffer:
Dec  6 16:14:39.181798  jlink:       triceExamples.c    47       23_578 16339 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
Dec  6 16:14:39.181798  jlink:       triceExamples.c    48       22_394 16338 31372e32  31383238  34383238  34303935  35333235
...
Dec  6 16:14:39.181798  jlink:       triceExamples.c    50              16200 5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
Dec  6 16:14:39.182303  jlink:       triceExamples.c    52       19_555 16335 i=44444400 aaaaaa00
Dec  6 16:14:39.182303  jlink:       triceExamples.c    52       18_941 16335 i=44444401 aaaaaa01
Dec  6 16:14:39.182303  jlink:       triceExamples.c    52       18_327 16335 i=44444402 aaaaaa02
Dec  6 16:14:39.182834  jlink:       triceExamples.c    52       17_713 16335 i=44444403 aaaaaa03
Dec  6 16:14:39.182834  jlink:       triceExamples.c    52       17_099 16335 i=44444404 aaaaaa04
Dec  6 16:14:40.187121  jlink:       triceExamples.c    29    0,031_855 16356 🐁 Speedy Gonzales a  32-bit timestamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    30    0,031_299 16355 🐁 Speedy Gonzales b  32-bit timestamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    31    0,030_743 16354 🐁 Speedy Gonzales c  32-bit timestamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    32    0,030_187 16353 🐁 Speedy Gonzales d  32-bit timestamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    33       29_631 16352 🐁 Speedy Gonzales e  16-bit timestamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    34       29_073 16351 🐁 Speedy Gonzales f  16-bit timestamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    35       28_515 16350 🐁 Speedy Gonzales g  16-bit timestamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    36       27_957 16349 🐁 Speedy Gonzales h  16-bit timestamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    42       27_093 16344 2.71828182845904523536 <- float number as string
Dec  6 16:14:40.187121  jlink:       triceExamples.c    43       25_913 16343 2.71828182845904509080 (double with more ciphers than precision)
Dec  6 16:14:40.187121  jlink:       triceExamples.c    44       25_310 16342 2.71828174591064453125 (float  with more ciphers than precision)
Dec  6 16:14:40.187121  jlink:       triceExamples.c    45       24_730 16341 2.718282 (default rounded float)
Dec  6 16:14:40.187121  jlink:       triceExamples.c    46       24_149 16340 A Buffer:
Dec  6 16:14:40.187121  jlink:       triceExamples.c    47       23_577 16339 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
Dec  6 16:14:40.187630  jlink:       triceExamples.c    48       22_391 16338 31372e32  31383238  34383238  34303935  35333235
...
Dec  6 16:14:40.187690  jlink:       triceExamples.c    50              16200 5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
Dec  6 16:14:40.187690  jlink:       triceExamples.c    52       19_546 16335 i=44444400 aaaaaa00
Dec  6 16:14:40.188195  jlink:       triceExamples.c    52       18_930 16335 i=44444401 aaaaaa01
Dec  6 16:14:40.188195  jlink:       triceExamples.c    52       18_314 16335 i=44444402 aaaaaa02
Dec  6 16:14:40.188195  jlink:       triceExamples.c    52       17_698 16335 i=44444403 aaaaaa03
Dec  6 16:14:40.188195  jlink:       triceExamples.c    52       17_082 16335 i=44444404 aaaaaa04
Dec  6 16:14:41.191648  jlink:    triceLogDiagData.c    21              16382 RTT0_writeDepthMax=325 (BUFFER_SIZE_UP=1024)
Dec  6 16:14:41.191648  jlink:    triceLogDiagData.c    44              16378 triceSingleDepthMax = 108 of 172 (TRICE_BUFFER_SIZE)
Dec  6 16:14:41.191648  jlink:       triceExamples.c    29    0,030_628 16356 🐁 Speedy Gonzales a  32-bit timestamp
Dec  6 16:14:41.191648  jlink:       triceExamples.c    30    0,030_072 16355 🐁 Speedy Gonzales b  32-bit timestamp
```

"🐁 Speedy Gonzales" needs about 500 MCU clocks.

### 9.6. <a id="a-developer-setting-only-enabling-segger_rtt-and-without-deferred-output-gives-after-running-buildsh-trice_diagnostics0-trice_protect0"></a>A developer setting, only enabling SEGGER_RTT and without deferred output gives after running `./build.sh TRICE_DIAGNOSTICS=0 TRICE_PROTECT=0`:

```bash
arm-none-eabi-size out/F030_inst.elf
   text    data     bss     dec     hex filename
   5796      16    2736    8548    2164 out/F030_inst.elf
```

That is nearly 1 KB less Flash needs.

The output:

```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice_wt_devel/examples/F030_inst (devel)
$ trice l -p jlink -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0" -pf none -d16 -showID "deb:%5d"
Dec  6 16:20:10.545274  jlink:       triceExamples.c    12       65_535 16369  Hello! 👋🙂
Dec  6 16:20:10.545274  jlink:
Dec  6 16:20:10.545274  jlink:         ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
Dec  6 16:20:10.545274  jlink:         🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-F030R8   🎈🎈🎈🎈
Dec  6 16:20:10.545274  jlink:         🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃
Dec  6 16:20:10.545274  jlink:
Dec  6 16:20:10.545274  jlink:
Dec  6 16:20:10.545274  jlink:       triceExamples.c    61              16334 TRICE_DIRECT_OUTPUT == 1, TRICE_DEFERRED_OUTPUT == 0
Dec  6 16:20:10.545274  jlink:       triceExamples.c    63              16333 TRICE_STACK_BUFFER, TRICE_MULTI_PACK_MODE
Dec  6 16:20:10.545890  jlink:       triceExamples.c    76              16327 _CYCLE == 1, _PROTECT == 0, _DIAG == 0, XTEA == 0
Dec  6 16:20:10.546396  jlink:       triceExamples.c    77              16326 _SINGLE_MAX_SIZE=104, _BUFFER_SIZE=172, _DEFERRED_BUFFER_SIZE=1024
Dec  6 16:20:11.448885  jlink:       triceExamples.c    29    0,031_859 16356 🐁 Speedy Gonzales a  32-bit timestamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    30    0,031_661 16355 🐁 Speedy Gonzales b  32-bit timestamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    31    0,031_463 16354 🐁 Speedy Gonzales c  32-bit timestamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    32    0,031_265 16353 🐁 Speedy Gonzales d  32-bit timestamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    33       31_067 16352 🐁 Speedy Gonzales e  16-bit timestamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    34       30_867 16351 🐁 Speedy Gonzales f  16-bit timestamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    35       30_667 16350 🐁 Speedy Gonzales g  16-bit timestamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    36       30_467 16349 🐁 Speedy Gonzales h  16-bit timestamp
Dec  6 16:20:11.549660  jlink:       triceExamples.c    42       29_961 16344 2.71828182845904523536 <- float number as string
Dec  6 16:20:11.549660  jlink:       triceExamples.c    43       29_141 16343 2.71828182845904509080 (double with more ciphers than precision)
Dec  6 16:20:11.549660  jlink:       triceExamples.c    44       28_897 16342 2.71828174591064453125 (float  with more ciphers than precision)
Dec  6 16:20:11.549660  jlink:       triceExamples.c    45       28_675 16341 2.718282 (default rounded float)
Dec  6 16:20:11.549660  jlink:       triceExamples.c    46       28_452 16340 A Buffer:
Dec  6 16:20:11.550166  jlink:       triceExamples.c    47       28_238 16339 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
Dec  6 16:20:11.550247  jlink:       triceExamples.c    48       27_412 16338 31372e32  31383238  34383238  34303935  35333235
...
Dec  6 16:20:11.550247  jlink:       triceExamples.c    50              16200 5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
Dec  6 16:20:11.550754  jlink:       triceExamples.c    52       25_646 16335 i=44444400 aaaaaa00
Dec  6 16:20:11.550754  jlink:       triceExamples.c    52       25_389 16335 i=44444401 aaaaaa01
Dec  6 16:20:11.550754  jlink:       triceExamples.c    52       25_132 16335 i=44444402 aaaaaa02
Dec  6 16:20:11.551285  jlink:       triceExamples.c    52       24_875 16335 i=44444403 aaaaaa03
Dec  6 16:20:11.551285  jlink:       triceExamples.c    52       24_618 16335 i=44444404 aaaaaa04
Dec  6 16:20:12.453968  jlink:       triceExamples.c    29    0,031_859 16356 🐁 Speedy Gonzales a  32-bit timestamp
Dec  6 16:20:12.453968  jlink:       triceExamples.c    30    0,031_661 16355 🐁 Speedy Gonzales b  32-bit timestamp
```

"🐁 Speedy Gonzales" direct outout needs about 200 MCU clocks and not 500 as before.

### 9.7. <a id="settings-conclusion"></a>Settings Conclusion

* 4-8 KB Flash and 1.2 KB RAM needed for the Trice library.
* The RAM size is mainly influenced by the configured buffer sizes.
* Switching off diagnostics and/or protection is ok for less memory needs and faster Trice execution after getting some experience with the project.

### 9.8. <a id="legacy-trice-space-example-old-version"></a>Legacy Trice Space Example (Old Version)

* STM32CubeMX generated empty default project: `Program Size: Code=2208 RO-data=236 RW-data=4 ZI-data=1636`
* Same project with default `Trice` instrumentation: `Program Size: Code=2828 RO-data=236 RW-data=44 ZI-data=1836`
* Needed [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory): 620 Bytes
* Needed [RAM](https://en.wikipedia.org/wiki/Random-access_memory): 40 Bytes plus 200 Bytes for the 2 times 100 Bytes double buffer
* With increased/decreased buffers also more/less [RAM](https://en.wikipedia.org/wiki/Random-access_memory) is needed.
* With each additional Trice macro a few additional [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory) bytes, like 10 assembler instructions, are needed.
* No printf-like library code is used anymore.
* No format strings go into the target code anymore.
* In general Trice instrumentation **reduces** the needed memory compared to a printf-like implementation.

### 9.9. <a id="memory-needs-for-old-example-1"></a>Memory Needs for Old Example 1

The following numbers are measured with a legacy encoding, showing that the instrumentation code can be even smaller.

Program Size (STM32-F030R8 demo project)      | trice instrumentation | buffer size | compiler optimize for time | comment
--------------------------------------------- | --------------------- | ----------- | -------------------------- | -------------------------------
Code=1592 RO-data=236 RW-data= 4 ZI-data=1028 | none                  | 0           | off                        | CubeMX generated, no trice
Code=1712 RO-data=240 RW-data=24 ZI-data=1088 | core                  | 64          | off                        | core added without trices
Code=3208 RO-data=240 RW-data=36 ZI-data=1540 | TriceCheckSet()       | 512         | off                        | TRICE_SHORT_MEMORY is 1 (small)
Code=3808 RO-data=240 RW-data=36 ZI-data=1540 | TriceCheckSet()       | 512         | on                         | TRICE_SHORT_MEMORY is 0 (fast)

* The core instrumentation needs less 150 bytes FLASH and about 100 bytes RAM when buffer size is 64 bytes.
* The about 50 trices in TriceCheckSet() allocate roughly 2100 (fast mode) or 1500 (small mode) bytes.
* trices are removable without code changes with `#define TRICE_OFF 1` before `incude "trice.h"` on file level or generally on project level.

### 9.10. <a id="memory-needs-for-old-example-2"></a>Memory Needs for Old Example 2

Project                        | Compiler    | Optimization | Link-Time-Optimization | Result                                        | Remark
------------------------------ | ----------- | ------------ | ---------------------- | --------------------------------------------- | ------------------------------------------------------------------
MDK-ARM_STM32F030_bareerated   | CLANG v6.19 | -Oz          | yes                    | Code=1020 RO-data=196 RW-data=0 ZI-data=1024  | This is the plain generated project without trice instrumentation.
MDK-ARM_STM32F030_instrumented | CLANG v6.19 | -Oz          | yes                    | Code=4726 RO-data=238 RW-data=16 ZI-data=4608 | This is with full trice instrumentation with example messages.

* The size need is less than 4 KB. See also [Trice Project Image Size Optimization](memory_usage.md).

