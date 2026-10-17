# Project Resources

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

- [1. Download and install Trice](#download-and-install-trice)
- [2. Alternative projects and related approaches](#alternative-projects-and-related-approaches)
  - [2.1. Logging and deferred formatting](#logging-and-deferred-formatting)
  - [2.2. Tracing and visualization](#tracing-and-visualization)
  - [2.3. Other techniques and background reading](#other-techniques-and-background-reading)
- [3. Community and contributions](#community-and-contributions)
- [4. Support and sponsoring](#support-and-sponsoring)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="download-and-install-trice"></a>Download and install Trice

Choose a release or a source checkout, then use its matching target sources, host tools, and documentation. An older release binary may not contain features documented by a newer checkout. See [firmware and host-tool compatibility](id_reference_list_til_json.md#compatibility-with-firmware-and-host-tool-versions).

- [GitHub Releases](https://github.com/rokath/trice/releases) provides published host-tool archives and source archives. Choose the operating system and architecture of your host computer; extract both `trice` and `tlog` and add their directory to `PATH`.
- For a checkout build, run the existing script from the repository root with Go and Bash installed:

  ```sh
  ./scripts/buildTriceTool.sh
  trice --version
  tlog --version
  ```

  The script builds `trice` and `tlog` and prints the installation paths. Use the Go version required by [go.mod](../../go.mod); see [building the host tools](tool_build_release.md#build-trice-tool-from-go-sources) for details.
- For v2, use a release archive or build a checkout of its tag. Remote `go install github.com/rokath/trice/cmd/trice@v2.0.0` is not supported while the module path has no `/v2` suffix. A local checkout build does not require that suffix.
- The macOS Homebrew tap provides stable releases: `brew install rokath/tap/trice`. For a beta, use its tagged release archive or build from its source checkout. Confirm the installed version rather than assuming it includes the current development checkout.

For the first hardware-free experiment, follow the [User Manual](../TriceUserManual.md#get-lib-and-tools). Its PC examples additionally require a native C compiler. Shell examples use Bash; Windows users can run them in Git Bash with a Windows host compiler. For target integration, choose a [firmware quickstart](quickstarts.md).

## 2. <a id="alternative-projects-and-related-approaches"></a>Alternative projects and related approaches

Trice combines printf-like source calls, ID-based host decoding, and configurable binary transport. Other projects emphasize different languages, runtime environments, tracing models, or integrations. These links are starting points for comparison, not a claim that all projects provide the same features or interchangeable formats.

### 2.1. <a id="logging-and-deferred-formatting"></a>Logging and deferred formatting

- [defmt](https://github.com/knurling-rs/defmt): deferred formatting for embedded Rust.
- [Zephyr dictionary-based logging](https://docs.zephyrproject.org/latest/services/logging/index.html#dictionary-based-logging).
- [Memfault Compact Logs](https://docs.memfault.com/docs/mcu/compact-logs).
- [Postform](https://github.com/Javier-varez/Postform).
- [Embedded Logger (elog)](https://github.com/martinribelotta/elog).
- [McuLog](https://mcuoneclipse.com/2020/06/01/mculog-logging-framework-for-small-embedded-microcontroller-systems/).
- [uLog](https://github.com/rdpoor/ulog).
- [NanoLog](https://github.com/PlatformLab/NanoLog).
- [Diagnostic Log and Trace (DLT)](https://github.com/COVESA/dlt-daemon).

### 2.2. <a id="tracing-and-visualization"></a>Tracing and visualization

- [Baical UP7](https://baical.net/up7.html).
- [Pigweed tokenized tracing](https://pigweed.dev/pw_trace_tokenized/).
- [SEGGER SystemView](https://www.segger.com/products/development-tools/systemview/technology/what-is-systemview/).
- [Percepio Tracealyzer](https://percepio.com/tracealyzer/).
- [QP/Spy](https://www.state-machine.com/qtools/qpspy.html).
- [MCUViewer](https://github.com/klonyyy/MCUViewer).
- [RTEdbg](https://github.com/RTEdbg/RTEdbg).
- [Tonbandgerät](https://github.com/schilkp/Tonbandgeraet).
- [Traces](https://github.com/yotamr/traces).

### 2.3. <a id="other-techniques-and-background-reading"></a>Other techniques and background reading

- Plain `printf` over UART, or a project-specific binary logger.
- Hardware trace through Arm ITM/SWO, and [Arm Keil Event Recorder](https://arm-software.github.io/CMSIS-View/latest/evr.html).
- [SEGGER RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) as a transport; Trice can also [use RTT](rtt.md).
- [Logging with symbols in the Embedonomicon](https://docs.rust-embedded.org/embedonomicon/logging.html).
- [Call-stack logging through function instrumentation](https://dev.to/taugustyn/call-stack-logger-function-instrumentation-as-a-way-to-trace-programs-flow-of-execution-419a).
- [Dynamic printf breakpoints](https://mcuoneclipse.com/2022/02/09/debugging-with-dynamic-printf-breakpoints/).
- [Minimal Structured Logging for Autonomous Vehicles](https://youtu.be/FyJI4Z6jD4w), a talk about another approach.

## 3. <a id="community-and-contributions"></a>Community and contributions

Use [GitHub Discussions](https://github.com/rokath/trice/discussions) for questions, ideas, and integration experience. Search [open and closed issues](https://github.com/rokath/trice/issues?q=is%3Aissue) for known problems and previous resolutions. For a reproducible bug, include the tool version, relevant configuration, command, expected result, and actual output.

Examples, documentation improvements, platform recipes, fixes, and reproducible performance measurements are welcome. [CONTRIBUTING](../../CONTRIBUTING.md) describes the development workflow, [CHANGELOG](../../CHANGELOG.md) records release changes, and the [MIT License](../../LICENSE.md) governs the project. [AUTHORS](../../AUTHORS.md) lists contributors.

## 4. <a id="support-and-sponsoring"></a>Support and sponsoring

If Trice helps your work, you can support it by sharing an example, improving the documentation, reporting a reproducible problem, contributing a fix, or starring the repository. Financial support for continued maintenance is also welcome:

- [GitHub Sponsors](https://github.com/sponsors/rokath/).
- [Buy Me a Coffee](https://buymeacoffee.com/rokath).
- [PayPal](https://www.paypal.com/paypalme/rolfkarlthomas).

These are the support routes configured by the project in [FUNDING.yml](../../.github/FUNDING.yml). Sponsoring is optional; using Trice does not require a donation.

