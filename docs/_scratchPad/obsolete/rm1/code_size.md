# Trice Code Size Optimization

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Code Optimization -o3 or -oz (if supported)](#code-optimization--o3-or--oz-if-supported)
* [2. Compiler Independent Setting (a bit outdated)](#compiler-independent-setting-a-bit-outdated)
* [3. Linker Option --split-sections (if supported)](#linker-option---split-sections-if-supported)
* [4. Linker Optimization -flto (if supported)](#linker-optimization--flto-if-supported)
  * [4.1. ARMCC Compiler v5 Linker Feedback](#armcc-compiler-v5-linker-feedback)
  * [4.2. ARMCLANG Compiler v6 Link-Time Optimization](#armclang-compiler-v6-link-time-optimization)
  * [4.3. GCC](#gcc)
  * [4.4. LLVM ARM Clang](#llvm-arm-clang)
  * [4.5. Other IDE´s and compilers](#other-ides-and-compilers)
* [5. Legacy STM32F030 Example Project - Different Build Sizes](#legacy-stm32f030-example-project---different-build-sizes)
  * [5.1. ARMCC compiler v5](#armcc-compiler-v5)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

You can compile the complete `src/*.c` library instead of maintaining a list of individual files. With unused-section removal enabled, only the library functions and data needed by your application remain in the final executable or firmware image. Configuration already excludes many disabled features during preprocessing; the linker removes the remaining unreferenced sections. Compiling every source can still take longer than compiling a selected subset.

The PC example scripts enable this explicitly: `-ffunction-sections -fdata-sections` separates functions and data, then `-Wl,--gc-sections` removes unused sections with GCC/Clang on Linux and MinGW on Windows. On macOS they use `-Wl,-dead_strip` instead. For MSVC, use `/Gy /Gw` and `/OPT:REF`. Embedded examples use the corresponding section-removal settings in their build configuration. The minimal demos omit these optional flags to keep their build commands simple. Without these settings, compiling `src/*.c` alone does **not** guarantee that unused code disappears.

Search project and generated header directories first, then `src`, then `src/default_conf` last. The last directory supplies a fallback `SEGGER_RTT_Conf.h`, so PC demos need no additional RTT header or J-Link installation. A project-specific RTT header takes precedence and remains appropriate for boards that need different buffer sizes or locking. If RTT is enabled, Trice inherits the selected SEGGER buffer sizes and channel counts automatically; explicit `TRICE_*` overrides must still agree with them. See [Trice over RTT](rtt.md).

## 1. <a id="code-optimization--o3-or--oz-if-supported"></a>Code Optimization -o3 or -oz (if supported)

For debugging it could be helpful to switch off code optimization what increases the code size. A good choice is `-o1`. See also
[TRICE_STACK_BUFFER could cause stack overflow with -o0 optimization](usage_tips.md#trice_stack_buffer-could-cause-stack-overflow-with--o0-optimization).


## 2. <a id="compiler-independent-setting-a-bit-outdated"></a>Compiler Independent Setting (a bit outdated)

Maybe the following is a bit unhandy but it decreases the code amount, build time and the image size.

* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_trice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.
* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_Trice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.
* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_TRice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.

When having lots of program memory simply let all values be `1`. With specific linker optimization unused functions can get stripped out automatically.

It is possible to `#define TRICE_SINGLE_MAX_SIZE 12` for example in *triceConfig.h*. This automaticaly disables all Trice messages with payloads > 8 bytes (Trice size is 4 bytes).

## 3. <a id="linker-option---split-sections-if-supported"></a>Linker Option --split-sections (if supported)

In ARM-MDK uVision `Project -> Options -> C/C++ -> "One EFL section for each function"` allows good optimization and getting rid of unused code without additional linker optimization. This leads to a faster build process and is fine for most cases. It allows excluding unused functions.

## 4. <a id="linker-optimization--flto-if-supported"></a>Linker Optimization -flto (if supported)

* To get the smallest possible image, do _not_ use option `--split sections`.
* Use linker optimization alone.
* This increases the build time but reduces the image size significantly.

### 4.1. <a id="armcc-compiler-v5-linker-feedback"></a>ARMCC Compiler v5 Linker Feedback

* In ARM-MDK uVision, when using ARMCC compiler v5, there is a check box `Project -> Options -> Target -> "Cross Module Optimization"`.
* In ARMCC this works also with the lite version.

### 4.2. <a id="armclang-compiler-v6-link-time-optimization"></a>ARMCLANG Compiler v6 Link-Time Optimization

* In ARM-MDK uVision, when using ARMCLANG compiler v6, the check box `Project -> Options -> C/C++(AC6) -> "Link-Time Optimization"` is usable to set the CLI `-flto` switch.
* LTO is not possible with ARMCLANG6 lite: https://developer.arm.com/documentation/ka004054/latest.

### 4.3. <a id="gcc"></a>GCC

With GCC use the `-flto` CLI switch directly.

### 4.4. <a id="llvm-arm-clang"></a>LLVM ARM Clang

This compiler is much faster and creates the smallest images. Right now it uses the GCC libs and linker.

### 4.5. <a id="other-ides-and-compilers"></a>Other IDE´s and compilers

Please check the manuals and create a pull request or simply let me know.

## 5. <a id="legacy-stm32f030-example-project---different-build-sizes"></a>Legacy STM32F030 Example Project - Different Build Sizes

### 5.1. <a id="armcc-compiler-v5"></a>ARMCC compiler v5

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

<p align="right">(<a href="#trice-code-size-optimization">back to top</a>)</p>

