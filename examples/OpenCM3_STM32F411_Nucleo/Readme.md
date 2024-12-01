<div id="top">

# Trice on LibOpenCM3

<details><summary>Table of Contents</summary><ol><!-- TABLE OF CONTENTS START -->

<!-- 
Table of Contents Generation:
- Install vsCode extension "Markdown TOC" from dumeng 
- Use Shift-Ctrl-P "markdownTOC:generate" to get the automatic numbering.
- replace "<a id=" with "<a id=" 
-->

<!-- vscode-markdown-toc -->
* 1. [Description](#description)
* 2. [Setting Up](#setting-up)
* 3. [Build](#build)
* 4. [Deferred Mode for max Speed](#deferred-mode-for-max-speed)
* 5. ["Hardware" Changes](#"hardware"-changes)
* 6. [Using RTT with on-board J-Link and JLinkRTTLogger](#using-rtt-with-on-board-j-link-and-jlinkrttlogger)
* 7. [Using RTT with on-board J-Link and OpenOCD](#using-rtt-with-on-board-j-link-and-openocd)
  * 7.1. [With Windows not possible](#with-windows-not-possible)
  * 7.2. [Darwin](#darwin)
* 8. [Using RTT with on-board ST-Link and OpenOCD](#using-rtt-with-on-board-st-link-and-openocd)
* 9. [Using On-board ST-Link and VS-Code Cortex-Debug Extension](#using-on-board-st-link-and-vs-code-cortex-debug-extension)
  * 9.1. [Fail](#fail)
  * 9.2. [OK](#ok)

<!-- vscode-markdown-toc-config
  numbering=true
  autoSave=true
  /vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

</div></ol></details><!-- TABLE OF CONTENTS END -->

## About

[LibOpenCM3](https://libopencm3.org/) is a hardware abstraction library for many microcontrollers.

This folder is an exampe using STM's [STM32F411 Nucleo](https://www.st.com/en/evaluation-tools/nucleo-f411re.html) board.

## Prerequisites

- Suitable ARM GCC cross compiler (`arm-none-eabi-gcc`) found in your system's PATH
- GNU Make, or compatible
- Environment variable `OPENCM3_DIR` points to the base install of libopencm3.
  This is e.g. the libopencm3 source directory, if you also built it in the source directory.
- OpenOCD

## Usage

- Run `make direct_mode.elf` to compile with Trice mode 0.
- Run `make flash_direct_mode` to program the board.
- Run trice: `trice l -p /dev/ttyACM0`.

## Hint

- This project is an older contribution and not maintained right now.
- To use it, copy for example [../F030_inst/Core/Inc/triceConfig.h](./F030_inst/Core/Inc/triceConfig.h) and replace the compiler adaption and hardware specific parts from the [./triceConfig.h](./triceConfig.h) here.
- The trice instrumention code resides now in ../src/ instead of ../pkg/src/ and is divided into several files. This needs to be adapted too to activate this project again.
- Pull requests are welcome.

<p align="right">(<a href="#top">back to top</a>)</p>
