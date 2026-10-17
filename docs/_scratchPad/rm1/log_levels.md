# Trice Log-Level Control

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Trice version 1.0 Compile-time Log-level Control](#trice-version-10-compile-time-log-level-control)
* [2. Trice version 1.0 Run-time Log-level Control](#trice-version-10-run-time-log-level-control)
* [3. Trice Version 1.0 Compile-time - Run-time Log-level Control](#trice-version-10-compile-time---run-time-log-level-control)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="trice-version-10-compile-time-log-level-control"></a>Trice version 1.0 Compile-time Log-level Control

In Trice version 1.0 is no compile-time log-level control. You can only disable **all** Trice logs 

* on file level by adding a `#define TRICE_OFF` line before `#include "trice.h"`
* or project level by using `-DTRICE_OFF` as compiler switch.

## 2. <a id="trice-version-10-run-time-log-level-control"></a>Trice version 1.0 Run-time Log-level Control

Because the target Trice code is so fast and generates only a few bytes per log, in Trice version 1.0 is no direct run-time log-level control inside the target code.  The user has the Trice CLI switches `-ban`, `-pick` and `-logLevel`, to control, which Trice messages are displayed by  the Trice tool.

## 3. <a id="trice-version-10-compile-time---run-time-log-level-control"></a>Trice Version 1.0 Compile-time - Run-time Log-level Control

During compilation the developer can control which Trice tags, like *info* in `trice( "info:...\n");` get which ID range. Look for `-IDRange` in `trice h -i` output. By defining values like `TRICE_UARTA_MIN_ID` in the project specific *triceConfig.h* during compile-time is controllable, which Trice tags get routed to an output device or not.

<p align="right">(<a href="#trice-log-level-control">back to top</a>)</p>

