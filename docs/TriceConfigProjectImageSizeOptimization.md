
# `triceConfig.h` Project Image Size Optimization

<details><summary>Table of Contents</summary><ol><!-- TABLE OF CONTENTS START -->

<!-- 
Table of Contents Generation:
- Install vsCode extension "Markdown TOC" from dumeng 
- Use Shift-Ctrl-P "markdownTOC:generate" to get the automatic numbering.
- replace "<a id=" with "<a id=" 
-->

<!-- vscode-markdown-toc -->
* 1. [Code Optimization `-o3` or `-oz` (if supported)](#code-optimization-`-o3`-or-`-oz`-(if-supported))
* 2. [Compiler Independent Setting](#compiler-independent-setting)
* 3. [Linker Option `--split-sections` (if supported)](#linker-option-`--split-sections`-(if-supported))
* 4. [Linker Optimization `-flto` (if supported)](#linker-optimization-`-flto`-(if-supported))
  * 4.1. [ARMCC compiler v5 "Linker Feedback"](#armcc-compiler-v5-"linker-feedback")
  * 4.2. [ARMCLANG compiler v6 "Link-Time Optimization"](#armclang-compiler-v6-"link-time-optimization")
  * 4.3. [GCC](#gcc)
  * 4.4. [LLVM ARM Clang](#llvm-arm-clang)
  * 4.5. [Other IDE´s and compilers](#other-ide´s-and-compilers)
* 5. [STM32F030 Example Project - Different Builds](#stm32f030-example-project---different-builds)
  * 5.1. [ARMCC compiler v5](#armcc-compiler-v5)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

<div id="top"></div></ol></details><!-- TABLE OF CONTENTS END -->

> ---
> You *can* reduce trice code size by different means.

##  1. <a id='code-optimization-`-o3`-or-`-oz`-(if-supported)'></a>Code Optimization `-o3` or `-oz` (if supported)

* For debugging it could be helpful to switch off code optimization what increases the code size.

##  2. <a id='compiler-independent-setting'></a>Compiler Independent Setting

Maybe the following is a bit unhandy but it decreases the code amount, build time and the image size.

* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_trice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.
* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_Trice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.
* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_TRice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.

When having lots of program memory simply let all values be `1`. With specific linker optimization unused functions can get stripped out automatically. 

##  3. <a id='linker-option-`--split-sections`-(if-supported)'></a>Linker Option `--split-sections` (if supported)

* In ARM-MDK uVision `Project -> Options -> C/C++ -> "One EFL section for each function"` allows good optimization and getting rid of unused code without additional linker optimization. This leads to a faster build process and is fine for most cases. It allows excluding unused functions.

##  4. <a id='linker-optimization-`-flto`-(if-supported)'></a>Linker Optimization `-flto` (if supported)

* To get the smallest possible image, do _not_ use option `--split sections`.
* Use linker optimization alone.
* This increases the build time but reduces the image size significantly.

###  4.1. <a id='armcc-compiler-v5-"linker-feedback"'></a>ARMCC compiler v5 "Linker Feedback"

* In ARM-MDK uVision, when using ARMCC compiler v5, there is a check box `Project -> Options -> Target -> "Cross Module Optimization"`.
* In ARMCC this works also with the lite version.

###  4.2. <a id='armclang-compiler-v6-"link-time-optimization"'></a>ARMCLANG compiler v6 "Link-Time Optimization"

* In ARM-MDK uVision, when using ARMCLANG compiler v6, the check box `Project -> Options -> C/C++(AC6) -> "Link-Time Optimization"` is usable to set the CLI `-flto` switch. 
* LTO is not possible with ARMCLANG6 lite: https://developer.arm.com/documentation/ka004054/latest.

###  4.3. <a id='gcc'></a>GCC

With GCC use the `-flto` CLI switch directly.

###  4.4. <a id='llvm-arm-clang'></a>LLVM ARM Clang

This compiler is much faster and creates the smallest images. Right now it uses the GCC libs and linker.

###  4.5. <a id='other-ide´s-and-compilers'></a>Other IDE´s and compilers

Please check the manuals and create a pull request or simply let me know.

##  5. <a id='stm32f030-example-project---different-builds'></a>STM32F030 Example Project - Different Builds

###  5.1. <a id='armcc-compiler-v5'></a>ARMCC compiler v5

| Compiler | Linker         | Result                                          | Comment                           |
|----------|----------------|-------------------------------------------------|-----------------------------------|
| o0       |                | Code=46942 RO-data=266 RW-data=176 ZI-data=4896 | very big                          |
| o1       |                | Code=22582 RO-data=258 RW-data=168 ZI-data=4896 |                                   |
| o3       |                | Code=21646 RO-data=258 RW-data=168 ZI-data=4896 |                                   |
| o0       | split sections | Code= 7880 RO-data=268 RW-data=156 ZI-data=4892 | for debugging                     |
| o1       | split sections | Code= 5404 RO-data=260 RW-data=148 ZI-data=4892 | **for debugging**                 |
| o3       | split sections | Code= 4996 RO-data=260 RW-data=148 ZI-data=4892 | **good balance**                  |
| o0       | flto           | Code= 8150 RO-data=266 RW-data=176 ZI-data=4896 | builds slower                     |
| o1       | flto           | Code= 5210 RO-data=258 RW-data=148 ZI-data=4892 | builds slower                     |
| o3       | flto           | Code= 4818 RO-data=258 RW-data=148 ZI-data=4892 | builds slower, **smallest image** |

<p align="right">(<a href="#top">back to top</a>)</p>
