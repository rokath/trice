
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

* For X=8|16|32|64 and N=0...12 selectively set `#define ENABLE_triceXfn_N` to `0` for unused functions in project specific file `triceConfig.h`.
* For X=8|16|32|64 and N=0...12 selectively set `#define ENABLE_TriceXfn_N` to `0` for unused functions in project specific file `triceConfig.h`.
* For X=8|16|32|64 and N=0...12 selectively set `#define ENABLE_TRiceXfn_N` to `0` for unused functions in project specific file `triceConfig.h`.

When having lots of program memory or using specific linker optimization you can simply let all values be `1`.

##  4. <a name='LinkerOption--split-sectionsifsupported'></a>Linker Option `--split-sections` (if supported)

* In uVision `Project -> Options -> C/C++ -> "One EFL section for each function"`, allows good optimization and getting rid of unused code without additional linker optimization. This leeds to a faster build process and is fine for most cases. It allows excluding unused functions.

##  5. <a name='LinkerOptimization-fltoifsupported'></a>Linker Optimization `-flto` (if supported)

* To get the smallest possible image, do _not_ use option `--split sections`.
* Use linker optimization alone.
* This increases the build time but reduces the image size significantly.

###  5.1. <a name='ARMCCcompilerv5LinkerFeedback'></a>ARMCC compiler v5 "Linker Feedback"

* In ARM uVision, when using ARMCC compiler v5, there is a check box `Project -> Options -> Target -> "Cross Module Optimization"`.
* In ARMCC this works also with the lite version.

###  5.2. <a name='ARMCLANGcompilerv6Link-TimeOptimization'></a>ARMCLANG compiler v6 "Link-Time Optimization"

* In ARM uVision, when using ARMCLANG compiler v6, the check box `Project -> Options -> C/C++(AC6) -> "Link-Time Optimization"` is usable to set the CLI `-flto` switch. 
* LTO is not possible with ARMCLANG6 lite: https://developer.arm.com/documentation/ka004054/latest.

###  5.3. <a name='GCC'></a>GCC

With GCC use the `-flto` CLI switch directly.

###  5.4. <a name='OtherIDEsandcompilers'></a>Other IDE´s and compilers

Please check the manuals and create a pull request or simply let me know.