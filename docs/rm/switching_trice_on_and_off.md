# Switching Trice ON and OFF

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Target side compile-time Trice On-Off](#target-side-compile-time--trice-on-off)
* [2. Host side Trice On-Off](#host-side-trice-on-off)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

<div id="Target side Trice On-Off"></div>

## 1. <a id="target-side-compile-time--trice-on-off"></a>Target side compile-time  Trice On-Off

* If your code works well after checking, you can add `#define TRICE_OFF 1` just before the `#include "trice.h"` line and no Trice code is generated anymore for that file, so no need to delete or comment out Trice macros: <!-- ![./ref/TRICE_OFF.PNG](./ref/TRICE_OFF.PNG) -->

```C
#define TRICE_OFF 1
#include "trice.h"
void fn(void) {
    trice( iD(123), "Hi"); // Will generate code only, when TRICE_OFF == 0.
    trice( "Lo");          // Will generate code only, when TRICE_OFF == 0.
}
```

With `#define TRICE_OFF 1`, macros in this file are ignored completely by the compiler, but not by the Trice tool. In case of reconstructing the [**T**rice **ID** **L**ist](../../demoTIL.json) these no code generating macros are regarded and go into (or stay inside) the ID reference list.

* Hint from [@escherstair](https://github.com/escherstair): With `-D TRICE_OFF=1` as compiler option, the trice code diappears completely from the binary.
* No runtime On-Off switch is implemented for several reasons:
  * Would need a control channel to the target.
  * Would add little performance and code overhead.
  * Would sligtly change target timing (testing).
  * User can add its own switches anywhere.
  * The short Trice macro code is negligible.
  * The trice output is encryptable, if needed.
* Because of the low Trice bandwidth needs and to keep the target code as clear as possible the runtime On-Off decision should be done by the Trice tool.

<p align="right">(<a href="#switching-trice-on-and-off">back to top</a>)</p>

## 2. <a id="host-side-trice-on-off"></a>Host side Trice On-Off

* The PC Trice tool offers command line switches to `-pick` or `-ban` for Trice tags and will be extended with display switches.
* A Trice tool `-logLevel` switch is usable too.

<p align="right">(<a href="#switching-trice-on-and-off">back to top</a>)</p>

