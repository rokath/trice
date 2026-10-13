# Quickstarts

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

- [1. Quickstart: Existing non-blocking byte writer, deferred auxiliary 8-bit](#quickstart-existing-non-blocking-byte-writer-deferred-auxiliary-8-bit)
  - [1.1. Add Trice target sources](#add-trice-target-sources)
  - [1.2. Configure deferred auxiliary 8-bit output](#configure-deferred-auxiliary-8-bit-output)
  - [1.3. Assign your writer function](#assign-your-writer-function)
  - [1.4. Insert IDs before compiling](#insert-ids-before-compiling)
  - [1.5. Decode on the PC](#decode-on-the-pc)
  - [1.6. Common first checks](#common-first-checks)
  - [1.7. Why this quickstart matters](#why-this-quickstart-matters)
- [2. Quickstart: SEGGER RTT direct mode with J-Link](#quickstart-segger-rtt-direct-mode-with-j-link)
  - [2.1. Install tools](#install-tools)
  - [2.2. Add target sources](#add-target-sources)
  - [2.3. Configure direct RTT](#configure-direct-rtt)
  - [2.4. Add a first Trice call](#add-a-first-trice-call)
  - [2.5. Insert, build, flash](#insert-build-flash)
  - [2.6. Log through J-Link RTT](#log-through-j-link-rtt)
  - [2.7. When this path is ideal](#when-this-path-is-ideal)
  - [2.8. When this path is not ideal](#when-this-path-is-not-ideal)
- [3. Quickstart: UART or USB-VCOM deferred output](#quickstart-uart-or-usb-vcom-deferred-output)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## 1. <a id="quickstart-existing-non-blocking-byte-writer-deferred-auxiliary-8-bit"></a>Quickstart: Existing non-blocking byte writer, deferred auxiliary 8-bit

<!--
Existing manual coverage:
[Writing the Trice logs into an SD-card (or a user specific output)](writing_the_trice_logs_into_an_sd_card_or_a_user_specific_output.md) already documents `TRICE_DEFERRED_AUXILIARY8`, `TRICE_DEFERRED_AUXILIARY32`, and `UserNonBlockingDeferredWrite8AuxiliaryFn`.
This section reframes that information as a first-use quickstart, not as an SD-card special case.

README legacy placement:
This should replace the README's first-time dependence on SEGGER RTT when the user already has a byte output path.
-->

Use this path when your project already has a tested output function, for example:

- a non-blocking UART TX queue,
- a USB CDC/VCOM TX queue,
- a DMA-backed byte stream,
- a socket or pipe in a host-native test program,
- a file writer in a PC-side demo.

This is often the most universal first integration because Trice does not need to know your peripheral driver.
It only needs a function that accepts a byte buffer and length.

### 1.1. <a id="add-trice-target-sources"></a>Add Trice target sources

Add the complete [`src`](../../src) folder to your target project unchanged, including its `default_conf` subdirectory.
Search project and generated header directories first, followed by `src` and finally `src/default_conf`, for example `-Iproject -Igenerated -Isrc -Isrc/default_conf`.
Create a project-specific `triceConfig.h` in your application include path.

### 1.2. <a id="configure-deferred-auxiliary-8-bit-output"></a>Configure deferred auxiliary 8-bit output

Minimal `triceConfig.h` starting point:

```c
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_AUXILIARY8 1

/* Adapt these to your target if Trice can run from interrupts or multiple contexts. */
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }

#endif /* TRICE_CONFIG_H_ */
```

Notes:

- `TRICE_RING_BUFFER` is a balanced default.
- `TRICE_DOUBLE_BUFFER` can be faster for the Trice call itself, at the cost of more RAM and different buffer behavior.
- Keep the writer non-blocking or at least tightly bounded. A blocking writer moves the latency problem into `TriceTransfer()`.

### 1.3. <a id="assign-your-writer-function"></a>Assign your writer function

Example:

```c
#include "trice.h"

static void MyNonBlockingByteWrite(const uint8_t* data, size_t len) {
    /* Replace this with your project's existing writer. */
    ExistingTxQueueWrite(data, len);
}

void AppInit(void) {
    BoardInit();
    TriceInit(); // normally only needed when SEGGER_RTT is used. Otherwise it is an empty function.

    UserNonBlockingDeferredWrite8AuxiliaryFn = MyNonBlockingByteWrite;

    trice("boot\n");
}

void AppMainLoop(void) {
    for (;;) {
        AppRun();
        TriceTransfer();
    }
}
```

`TriceTransfer()` moves accumulated Trice records from the deferred buffer to your writer. Call it cyclically from the main loop, a low-priority task, or another context that is safe for your output driver.

### 1.4. <a id="insert-ids-before-compiling"></a>Insert IDs before compiling

From your project root:

```bash
touch til.json li.json
trice insert -src ./ -i ./til.json -li ./li.json
```

Then build and flash your target.

### 1.5. <a id="decode-on-the-pc"></a>Decode on the PC

For a serial or USB virtual COM port:

```bash
trice log -p COM15 -baud 921600 -i ./til.json -li ./li.json
```

On Linux/macOS, adapt the port:

```bash
trice log -p /dev/ttyACM0 -baud 921600 -i ./til.json -li ./li.json
```

If your writer produces a file, pipe, TCP stream, or another source, use the matching `trice log -p ...` input port.

### 1.6. <a id="common-first-checks"></a>Common first checks

- If you see no output, first confirm that your writer is called from `TriceTransfer()`.
- If raw data appears but does not decode, check framing settings on target and host.
- If output stops under burst load, increase `TRICE_DEFERRED_BUFFER_SIZE`, call `TriceTransfer()` more often, or improve the non-blocking writer queue.
- If interrupts or multiple tasks can call Trice, provide real critical-section macros.

### 1.7. <a id="why-this-quickstart-matters"></a>Why this quickstart matters

The old README led many first-time readers toward SEGGER RTT because it is convenient and fast.
That path is still valuable, but it also implies J-Link hardware and SEGGER tooling.
The auxiliary writer path is more portable: many projects already have a byte-stream output, and Trice can reuse it.

## 2. <a id="quickstart-segger-rtt-direct-mode-with-j-link"></a>Quickstart: SEGGER RTT direct mode with J-Link

<!--
Existing manual coverage:
See [Trice over RTT](trice_over_rtt.md), especially the RTT/J-Link sections and the notes around `TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE`.

README legacy placement:
This was previously the dominant README quickstart. It should remain a strong quickstart, but no longer be the only obvious first path.
-->

Use this path when you have a SEGGER J-Link and want the smallest amount of target-specific transport code.
See [Convert Evaluation Board onboard ST-Link to J-Link](trice_over_rtt.md#convert-evaluation-board-onboard-st-link-to-j-link) for a cheap option.

### 2.1. <a id="install-tools"></a>Install tools

- Install the `trice` host tool.
- Install the SEGGER J-Link software package so that `JLinkRTTLogger` or the relevant J-Link tools are in `PATH`.

### 2.2. <a id="add-target-sources"></a>Add target sources

Add the complete [`src`](../../src) folder to your target project unchanged, including its `default_conf` subdirectory.
Search project and generated header directories first, followed by `src` and finally `src/default_conf`, for example `-Iproject -Igenerated -Isrc -Isrc/default_conf`.

### 2.3. <a id="configure-direct-rtt"></a>Configure direct RTT

Minimal `triceConfig.h`:

```c
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

#endif /* TRICE_CONFIG_H_ */
```

### 2.4. <a id="add-a-first-trice-call"></a>Add a first Trice call

```c
#include "trice.h"

int main(void) {
    BoardInit();
    TriceInit(); // normally only needed when SEGGER_RTT is used. Otherwise it is an empty function.

    trice("Hello RTT\n");

    for (;;) {
        AppRun();
    }
}
```

Direct RTT does not require `TriceTransfer()` for the normal direct output path.

### 2.5. <a id="insert-build-flash"></a>Insert, build, flash

```bash
touch til.json li.json
trice insert -src ./ -i ./til.json -li ./li.json
```

Build and flash the target.

### 2.6. <a id="log-through-j-link-rtt"></a>Log through J-Link RTT

Example command; adapt the device name and speed:

```bash
trice log -p JLINK \
  -args "-Device STM32G0B1RE -if SWD -Speed 4000 -RTTChannel 0" \
  -pf none -prefix off -hs off -d16 \
  -i ./til.json -li ./li.json
```

Alternative file-based workflow:

```bash
rm -f ./temp/trice.bin
mkdir -p ./temp
touch ./temp/trice.bin
JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin
```

In a second terminal:

```bash
trice log -p FILE -args ./temp/trice.bin \
  -pf none -prefix off -hs off -d16 \
  -i ./til.json -li ./li.json
```

### 2.7. <a id="when-this-path-is-ideal"></a>When this path is ideal

Direct RTT is excellent for lab development because the target writes to RTT memory and the probe drains it. It avoids UART setup and usually feels close to `printf` debugging.

### 2.8. <a id="when-this-path-is-not-ideal"></a>When this path is not ideal

It depends on J-Link/RTT infrastructure. If that hardware or closed host tooling is a blocker, start with the [Quickstart: Existing non-blocking byte writer, deferred auxiliary 8-bit](#quickstart-existing-non-blocking-byte-writer-deferred-auxiliary-8-bit) or a UART/VCOM deferred path.

<a id="quickstart-uart-vcom-deferred"></a>

## 3. <a id="quickstart-uart-or-usb-vcom-deferred-output"></a>Quickstart: UART or USB-VCOM deferred output

See also [Communication Ports](start_with_trice.md#communication-ports), the example projects, and the UART-related configuration examples. This section is intentionally short because UART setup is MCU/vendor-specific.

Use this path when the target has a UART, USB CDC/VCOM, or board-specific serial path and you want Trice to use the built-in UART backend rather than an auxiliary writer.

Minimal shape of `triceConfig.h`:

```c
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#include "main.h" /* or your MCU/vendor header */

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2 /* adapt to your project */

#endif /* TRICE_CONFIG_H_ */
```

Application shape:

```c
#include "trice.h"

int main(void) {
    BoardInit();
    UartInit();
    TriceInit(); // normally only needed when SEGGER_RTT is used. Otherwise it is an empty function.

    trice("boot\n");

    for (;;) {
        AppRun();
        TriceTransfer();
    }
}
```

Host side:

```bash
trice log -p COM15 -baud 921600 -i ./til.json -li ./li.json
```

On Linux/macOS:

```bash
trice log -p /dev/ttyACM0 -baud 921600 -i ./til.json -li ./li.json
```

For a quick first success, the [Quickstart: Existing non-blocking byte writer, deferred auxiliary 8-bit](#quickstart-existing-non-blocking-byte-writer-deferred-auxiliary-8-bit) may be easier if your project already has a working serial or USB write function.

<p align="right">(<a href="#quickstarts">back to top</a>)</p>

