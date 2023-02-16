
# `triceConfig.h` Project Image Size Optimization

##  1. <a name='TableofContence'></a>Table of Contence

<details><summary>Table of Contents</summary><ol>
<!-- vscode-markdown-toc -->

- [`triceConfig.h` Project Image Size Optimization](#triceconfigh-project-image-size-optimization)
  - [1. Table of Contence](#1-table-of-contence)
  - [2. Code Optimization `-o3` or `-oz` (if supported)](#2-code-optimization--o3-or--oz-if-supported)
  - [3. Compiler Independent Setting](#3-compiler-independent-setting)
  - [4. Linker Option `--split-sections` (if supported)](#4-linker-option---split-sections-if-supported)
  - [5. Linker Optimization `-flto` (if supported)](#5-linker-optimization--flto-if-supported)
    - [5.1. ARMCC compiler v5 "Linker Feedback"](#51-armcc-compiler-v5-linker-feedback)
    - [5.2. ARMCLANG compiler v6 "Link-Time Optimization"](#52-armclang-compiler-v6-link-time-optimization)
    - [5.3. GCC](#53-gcc)
    - [5.4. Other IDE´s and compilers](#54-other-ides-and-compilers)
  - [6. STM32F030 Example Project - Different Builds](#6-stm32f030-example-project---different-builds)
    - [6.1. ARMCC compiler v5](#61-armcc-compiler-v5)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

</ol></details>

> ---
> You *can* reduce trice code size by different means.

##  2. <a name='CodeOptimization-o3or-ozifsupported'></a>Code Optimization `-o3` or `-oz` (if supported)

* For debugging it could be helpful to switch off temporary code optimization what increases the code size.

##  3. <a name='CompilerIndependentSetting'></a>Compiler Independent Setting

Maybe it´s a bit unhandy but it decreases the code amount, build time and the image size.

* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_trice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.
* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_Trice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.
* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_TRice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.

When having lots of program memory or using specific linker optimization you can simply let all values be `1`.

##  4. <a name='LinkerOption--split-sectionsifsupported'></a>Linker Option `--split-sections` (if supported)

* In ARM-MDK uVision `Project -> Options -> C/C++ -> "One EFL section for each function"` allows good optimization and getting rid of unused code without additional linker optimization. This leeds to a faster build process and is fine for most cases. It allows excluding unused functions.

##  5. <a name='LinkerOptimization-fltoifsupported'></a>Linker Optimization `-flto` (if supported)

* To get the smallest possible image, do _not_ use option `--split sections`.
* Use linker optimization alone.
* This increases the build time but reduces the image size significantly.

###  5.1. <a name='ARMCCcompilerv5LinkerFeedback'></a>ARMCC compiler v5 "Linker Feedback"

* In ARM-MDK uVision, when using ARMCC compiler v5, there is a check box `Project -> Options -> Target -> "Cross Module Optimization"`.
* In ARMCC this works also with the lite version.

###  5.2. <a name='ARMCLANGcompilerv6Link-TimeOptimization'></a>ARMCLANG compiler v6 "Link-Time Optimization"

* In ARM-MDK uVision, when using ARMCLANG compiler v6, the check box `Project -> Options -> C/C++(AC6) -> "Link-Time Optimization"` is usable to set the CLI `-flto` switch. 
* LTO is not possible with ARMCLANG6 lite: https://developer.arm.com/documentation/ka004054/latest.

###  5.3. <a name='GCC'></a>GCC

With GCC use the `-flto` CLI switch directly.

###  5.4. <a name='OtherIDEsandcompilers'></a>Other IDE´s and compilers

Please check the manuals and create a pull request or simply let me know.

##  6. <a name='STM32F030ExampleProject-DifferentBuilds'></a>STM32F030 Example Project - Different Builds

###  6.1. <a name='ARMCCcompilerv5'></a>ARMCC compiler v5

| Compiler | Linker         | Result                                          | Comment       |
| -        | -              | -                                               | -             |
|  o0      |                | Code=46942 RO-data=266 RW-data=176 ZI-data=4896 | very big      |
|  o1      |                | Code=22582 RO-data=258 RW-data=168 ZI-data=4896 |               |
|  o3      |                | Code=21646 RO-data=258 RW-data=168 ZI-data=4896 |               |
|  o0      | split sections | Code= 7880 RO-data=268 RW-data=156 ZI-data=4892 | for debugging |
|  o1      | split sections | Code= 5404 RO-data=260 RW-data=148 ZI-data=4892 | **for debugging** |
|  o3      | split sections | Code= 4996 RO-data=260 RW-data=148 ZI-data=4892 | **good balance**  |
|  o0      | flto           | Code= 8150 RO-data=266 RW-data=176 ZI-data=4896 | builds slower |
|  o1      | flto           | Code= 5210 RO-data=258 RW-data=148 ZI-data=4892 | builds slower |
|  o3      | flto           | Code= 4818 RO-data=258 RW-data=148 ZI-data=4892 | builds slower, **smallest image** |
