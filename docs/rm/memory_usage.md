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
* [7. TRICESTACKBUFFER could cause stack overflow with -o0 optimization](#tricestackbuffer-could-cause-stack-overflow-with--o0-optimization)
* [8. Recorded memory measurements](#recorded-memory-measurements)
  * [8.1. F030bare Size](#f030bare-size)
  * [8.2. F030inst Size with TRICEOFF=1](#f030inst-size-with-triceoff1)
  * [8.3. F030inst with ring buffer](#f030inst-with-ring-buffer)
  * [8.4. F030inst with ring buffer](#f030inst-with-ring-buffer-1)
  * [8.5. A developer setting, only enabling SEGGERRTT](#a-developer-setting-only-enabling-seggerrtt)
  * [8.6. A developer setting, only enabling SEGGERRTT and without deferred output gives after running ./build.sh TRICE_DIAGNOSTICS=0 TRICE_PROTECT=0:](#a-developer-setting-only-enabling-seggerrtt-and-without-deferred-output-gives-after-running-buildsh-trice_diagnostics0-trice_protect0)
  * [8.7. Settings Conclusion](#settings-conclusion)
  * [8.8. Legacy Trice Space Example (Old Version)](#legacy-trice-space-example-old-version)
  * [8.9. Memory Needs for Old Example 1](#memory-needs-for-old-example-1)
  * [8.10. Memory Needs for Old Example 2](#memory-needs-for-old-example-2)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

Choose RAM and Flash settings separately. Buffer mode, maximum record size and active outputs affect RAM; enabled backends and linker garbage collection affect Flash. See [performance](performance.md) for execution-time tradeoffs.

## 1. <a id="no-dynamic-memory-management-needed"></a>No Dynamic Memory Management needed

Trice's target logging path needs no heap allocator: it does not call `malloc`, `calloc`, `realloc` or `free`. Buffer sizes are selected in the target configuration. Depending on the chosen mode, storage includes static buffers and automatic local buffers on the stack; a stack buffer is not a static object. RAM requirements therefore depend on the configuration, record sizes and buffering strategy; see [Trice memory needs](memory_usage.md).

This statement covers Trice's own target logging code, including its CE adapters. It does not cover the Go host tool, an RTOS, a transport callback, an application's local-formatting sink, or a function invoked by a CE expression. For example, a user-supplied `read_context()` that allocates memory still allocates when called through `-ce`. Choose non-allocating application functions when the complete log call must avoid the heap.

You can compile the complete `src/*.c` library instead of maintaining a list of individual files. With unused-section removal enabled, only the library functions and data needed by your application remain in the final executable or firmware image. Configuration already excludes many disabled features during preprocessing; the linker removes the remaining unreferenced sections. Compiling every source can still take longer than compiling a selected subset.

The PC example scripts enable this explicitly: `-ffunction-sections -fdata-sections` separates functions and data, then `-Wl,--gc-sections` removes unused sections with GCC/Clang on Linux and MinGW on Windows. On macOS they use `-Wl,-dead_strip` instead. For MSVC, use `/Gy /Gw` and `/OPT:REF`. Embedded examples use the corresponding section-removal settings in their build configuration. The minimal demos omit these optional flags to keep their build commands simple. Without these settings, compiling `src/*.c` alone does **not** guarantee that unused code disappears.

Search project and generated header directories first, then `src`, then `src/default_conf` last. The last directory supplies a fallback `SEGGER_RTT_Conf.h`, so PC demos need no additional RTT header or J-Link installation. A project-specific RTT header takes precedence and remains appropriate for boards that need different buffer sizes or locking. If RTT is enabled, Trice inherits the selected SEGGER buffer sizes and channel counts automatically; explicit `TRICE_*` overrides must still agree with them. See [Trice over RTT](rtt.md).

## 2. <a id="code-optimization--o3-or--oz-if-supported"></a>Code Optimization -o3 or -oz (if supported)

For debugging it could be helpful to switch off code optimization what increases the code size. A good choice is `-o1`. See also
[TRICE_STACK_BUFFER could cause stack overflow with -o0 optimization](memory_usage.md#tricestackbuffer-could-cause-stack-overflow-with--o0-optimization).


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

Compiler | Linker         | Result                                          | Comment
----------|----------------|-------------------------------------------------|-----------------------------------
o0       |                | Code=46942 RO-data=266 RW-data=176 ZI-data=4896 | very big
o1       |                | Code=22582 RO-data=258 RW-data=168 ZI-data=4896 | |
o3       |                | Code=21646 RO-data=258 RW-data=168 ZI-data=4896 | |
o0       | split sections | Code= 7880 RO-data=268 RW-data=156 ZI-data=4892 | for debugging
o1       | split sections | Code= 5404 RO-data=260 RW-data=148 ZI-data=4892 | **for debugging**
o3       | split sections | Code= 4996 RO-data=260 RW-data=148 ZI-data=4892 | **good balance**
o0       | flto           | Code= 8150 RO-data=266 RW-data=176 ZI-data=4896 | builds slower
o1       | flto           | Code= 5210 RO-data=258 RW-data=148 ZI-data=4892 | builds slower
o3       | flto           | Code= 4818 RO-data=258 RW-data=148 ZI-data=4892 | builds slower, **smallest image**

## 7. <a id="tricestackbuffer-could-cause-stack-overflow-with--o0-optimization"></a>TRICE_STACK_BUFFER could cause stack overflow with -o0 optimization

As discussed in [issue #294](https://github.com/rokath/trice/issues/294) it can happen, that several TRICE macros within one function call increase the stack usage more than expected, when compiler optimization is totally switched off.

## 8. <a id="recorded-memory-measurements"></a>Recorded memory measurements

These measurements describe the firmware, compiler and configuration shown in each record. They are reference examples, not current size guarantees. Rebuild your own configuration to measure it.

Depending on your target configuration the needed space can differ:

### 8.1. <a id="f030bare-size"></a>F030_bare Size

* `./build.sh`:

```bash
arm-none-eabi-size build/F030_bare.elf
   text    data     bss     dec     hex filename
   2428      12    1564    4004     fa4 build/F030_bare.elf
```

That is the basic size of an empty generated project just containing some drivers.

### 8.2. <a id="f030inst-size-with-triceoff1"></a>F030_inst Size with TRICE_OFF=1

* `./build.sh TRICE_OFF=1` :

```bash
arm-none-eabi-size build/F030_inst.elf
   text    data     bss     dec     hex filename
   2428      12    1564    4004     fa4 build/F030_inst.elf
```

This is exactly the same result, proofing that `TRICE_OFF 1` is working correctly.

### 8.3. <a id="f030inst-with-ring-buffer"></a>F030_inst with ring buffer

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

### 8.4. <a id="f030inst-with-ring-buffer-1"></a>F030_inst with ring buffer

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

### 8.5. <a id="a-developer-setting-only-enabling-seggerrtt"></a>A developer setting, only enabling SEGGER_RTT

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

### 8.6. <a id="a-developer-setting-only-enabling-seggerrtt-and-without-deferred-output-gives-after-running-buildsh-trice_diagnostics0-trice_protect0"></a>A developer setting, only enabling SEGGER_RTT and without deferred output gives after running `./build.sh TRICE_DIAGNOSTICS=0 TRICE_PROTECT=0`:

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

### 8.7. <a id="settings-conclusion"></a>Settings Conclusion

* 4-8 KB Flash and 1.2 KB RAM needed for the Trice library.
* The RAM size is mainly influenced by the configured buffer sizes.
* Switching off diagnostics and/or protection is ok for less memory needs and faster Trice execution after getting some experience with the project.

### 8.8. <a id="legacy-trice-space-example-old-version"></a>Legacy Trice Space Example (Old Version)

* STM32CubeMX generated empty default project: `Program Size: Code=2208 RO-data=236 RW-data=4 ZI-data=1636`
* Same project with default `Trice` instrumentation: `Program Size: Code=2828 RO-data=236 RW-data=44 ZI-data=1836`
* Needed [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory): 620 Bytes
* Needed [RAM](https://en.wikipedia.org/wiki/Random-access_memory): 40 Bytes plus 200 Bytes for the 2 times 100 Bytes double buffer
* With increased/decreased buffers also more/less [RAM](https://en.wikipedia.org/wiki/Random-access_memory) is needed.
* With each additional Trice macro a few additional [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory) bytes, like 10 assembler instructions, are needed.
* No printf-like library code is used anymore.
* No format strings go into the target code anymore.
* In general Trice instrumentation **reduces** the needed memory compared to a printf-like implementation.

### 8.9. <a id="memory-needs-for-old-example-1"></a>Memory Needs for Old Example 1

The following numbers are measured with a legacy encoding, showing that the instrumentation code can be even smaller.

Program Size (STM32-F030R8 demo project)      | trice instrumentation | buffer size | compiler optimize for time | comment
-----------------------------------------------|-----------------------|-------------|----------------------------|---------------------------------
Code=1592 RO-data=236 RW-data= 4 ZI-data=1028 | none                  | 0           | off                        | CubeMX generated, no trice
Code=1712 RO-data=240 RW-data=24 ZI-data=1088 | core                  | 64          | off                        | core added without trices
Code=3208 RO-data=240 RW-data=36 ZI-data=1540 | TriceCheckSet()       | 512         | off                        | TRICE_SHORT_MEMORY is 1 (small)
Code=3808 RO-data=240 RW-data=36 ZI-data=1540 | TriceCheckSet()       | 512         | on                         | TRICE_SHORT_MEMORY is 0 (fast)

* The core instrumentation needs less 150 bytes FLASH and about 100 bytes RAM when buffer size is 64 bytes.
* The about 50 trices in TriceCheckSet() allocate roughly 2100 (fast mode) or 1500 (small mode) bytes.
* trices are removable without code changes with `#define TRICE_OFF 1` before `incude "trice.h"` on file level or generally on project level.

### 8.10. <a id="memory-needs-for-old-example-2"></a>Memory Needs for Old Example 2

Project                        | Compiler    | Optimization | Link-Time-Optimization | Result                                        | Remark
--------------------------------|-------------|--------------|------------------------|-----------------------------------------------|--------------------------------------------------------------------
MDK-ARM_STM32F030_bareerated   | CLANG v6.19 | -Oz          | yes                    | Code=1020 RO-data=196 RW-data=0 ZI-data=1024  | This is the plain generated project without trice instrumentation.
MDK-ARM_STM32F030_instrumented | CLANG v6.19 | -Oz          | yes                    | Code=4726 RO-data=238 RW-data=16 ZI-data=4608 | This is with full trice instrumentation with example messages.

* The size need is less than 4 KB. See also [Trice Project Image Size Optimization](memory_usage.md).

