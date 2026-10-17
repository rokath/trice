# Transports and Custom Writers

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Other byte transports](#other-byte-transports)
* [2. Custom writers and SD-card recording](#custom-writers-and-sd-card-recording)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

Choose a path that can carry the encoded bytes without changing their order. The [UM firmware outline](../TriceUserManual.md#bring-trice-into-your-firmware) introduces the basic writer; [integration recipes](quickstarts.md) specify the configuration.

| Path | Typical use | Details |
| --- | --- | --- |
| UART / USB virtual COM | Standalone serial connection, often deferred output. | [UART recipe](quickstarts.md#quickstart-uart-or-usb-vcom-deferred-output). |
| RTT | Probe access to target memory. | [RTT configuration and tools](rtt.md). |
| Existing writer or file | Application transmit queue, SD-card recording, custom transport. | Writer requirements below. |

## 1. <a id="other-byte-transports"></a>Other byte transports

A very performant output path is RTT, if your MCU supports background memory access like the ARM-M ones.

Because the Trice tool needs only to receive, a single target UART-TX pin will do. But it is also possible to use a GPIO-Pin for Trice messages without occupying a UART resource.

* This slow path is usable because a Trice needs only few bytes for transmission.
* You can transmit small records (for example, 4 bytes without a stamp or payload) as bare message over one pin:

  <img src="../ref/manchester1.PNG" width="400">
  <img src="../ref/manchester2.PNG" width="480">

* The 2 images are taken from [https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/). See there for more information.
* As Trice dongle you can use any spare MCU board with an UART together with an FTDI USB converter.
  * This allowes also any other data path - method does'nt matter:\
  [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter),\
  [I²C](https://en.wikipedia.org/wiki/I%C2%B2C),\
  [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface),\
  [GPIO](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/),\
  [CAN](https://en.wikipedia.org/wiki/CAN_bus),\
  [LIN](https://en.wikipedia.org/wiki/Local_Interconnect_Network), ...
* [RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) is also a possible path to use - see [Trice over RTT](rtt.md) for options.

## 2. <a id="custom-writers-and-sd-card-recording"></a>Custom writers and SD-card recording

* Enable `TRICE_DEFERRED_AUXILIARY8` in your project specific _triceConfig.h_ file. 
* Enabling `TRICE_DEFERRED_AUXILIARY8` is possible parallel to any direct and/or deferred output.
* The `TRICE_DEFERRED_OUT_FRAMING` value is used also for the deferred auxiliary writes.
* Consider the value for `TRICE_DEFERRED_TRANSFER_MODE`. The `TRICE_SINGLE_PACK_MODE` would trigger a file write function on each single Trice message. 
* Provide a self-made function like this:

    ```C
    /// mySDcardWrite performs SD-card writing by appending to file myTriceLogs.bin.
    static void mySDcardWrite(const uint8_t* buf, size_t bufLen) {
        // Copy the complete chunk into an application-owned queue here.
        // Report queue overflow; do not retain buf after returning.
    }

    // Assign during initialization, before the first transfer.
    UserNonBlockingDeferredWrite8AuxiliaryFn = mySDcardWrite; 
    ```

* If the SD-card write is more effective using 32-bits chunks, consider `TRICE_DEFERRED_AUXILIARY32`, what is recommended also if you use the encryption option.
  * `TRICE_DEFERRED_AUXILIARY32` writes whole 32-bit words and pads the last word with zero bytes when needed.
* There maybe use cases for `TRICE_DIRECT_AUXILIARY8` or `TRICE_DIRECT_AUXILIARY32`, but consider the max write time.
* Archive the matching TIL and LI with the recording; see [dictionary ownership](id_management.md).
* The callback must finish within the caller's timing budget. For slow storage, queue a copy and write it in another context; handle partial writes and queue overflow there.
* To decode *myTriceLogs.bin* later (select matching framing and dictionaries)

    ```bash
    trice log -port FILEBUFFER -args myTriceLogs.bin -hs off
    ```

Related issues/discussions:
[\#253](https://github.com/rokath/trice/discussions/253)
[\#405](https://github.com/rokath/trice/discussions/405)
[\#425](https://github.com/rokath/trice/issues/425)
[\#447](https://github.com/rokath/trice/discussions/447)
[\#537](https://github.com/rokath/trice/discussions/537)

