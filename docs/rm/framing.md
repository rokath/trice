# Framing

* Trice messages are framed binary data, if framing is not disabled.
* Framing is important for data disruption cases and is done with [TCOBS](https://github.com/rokath/tcobs) (has included data compression) but the user can force to use [COBS](https://github.com/rokath/COBS), what makes it easier to write an own decoder in some cases or disable framing at all.
  * Set `TRICE_DIRECT_OUT_FRAMING` and/or `TRICE_DEFERRED_OUT_FRAMING` inside `triceConfig.h` and use the Trice tool `-packageFraming` switch accordingly.
* For robustness each Trice can get its own (T)COBS package (`TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE`). That is configurable for transfer data reduction. Use `#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE` inside `triceConfig.h` (is now default). This allows to reduce the data size a bit by avoiding many 0-delimiter bytes but results in some more data loss in case of data disruptions.

Encoder dependencies follow the **active outputs**. Select direct framing with `TRICE_DIRECT_OUT_FRAMING` and deferred framing with `TRICE_DEFERRED_OUT_FRAMING`. An inactive output's framing setting adds no dependency. For example, a direct-only COBS application does not need TCOBS despite the deferred framing default.

Active output framing | Required encoder source | Encoder sources that can be omitted
--- | --- | ---
TCOBS only | `src/tcobsv1Encode.c` | `src/cobsEncode.c`
COBS only | `src/cobsEncode.c` | `src/tcobsv1Encode.c`
NONE only | None | Both encoder sources
Direct COBS and deferred TCOBS, or the reverse | Both encoder sources | Neither

These selective builds work without LTO, optimization or linker removal of unused sections. Encryption-disabled builds need no `src/xtea.c`; builds without RTT need no `src/SEGGER_RTT.c` or project RTT configuration. With the Trice backend disabled (`TRICE_OFF` or `TRICE_CLEAN`, without counted typeX0 transmission), retained UART settings do not require `triceUart.h` or UART hardware hooks. The [minimal demos](examples.md#minimal-pc-demos-direct-and-deferred) still compile `src/*.c` for simplicity; removing unreferenced code from such a complete-library build remains a separate [image-size optimization](code_size.md).

`TriceEncode(encrypt, framing, dst, buf, len)` keeps its runtime framing argument. By default, `TRICE_COBS_ENCODE_SUPPORT` and `TRICE_TCOBS_ENCODE_SUPPORT` include exactly the encoders needed by active outputs. If your own code calls this API with additional framing methods, enable the corresponding support in `triceConfig.h` and link its encoder source. For example, add runtime COBS to an otherwise TCOBS-only application with:

```c
#define TRICE_COBS_ENCODE_SUPPORT 1
```

Use `TRICE_TCOBS_ENCODE_SUPPORT 1` to add runtime TCOBS instead. Both switches accept only `0` or `1`; disabling an encoder required by an active output is a compile-time error. A request for a framing method not included in the build returns `0`, leaves the source and destination untouched, and produces no fallback frame. Unknown framing values also return `0`. NONE remains available without either encoder; its return value is the copied byte count, without a delimiter. Check the return value before sending the destination buffer.

The [encoder dependency tests](../../internal/id/encoder_dependencies_test.go) link isolated native projects at `-O0`, decode their real output, and check disabled outputs and runtime opt-ins. They also inspect ARM GCC objects when that toolchain is installed; no test relies on LTO or unused-section removal.

<p align="right">(<a href="#framing">back to top</a>)</p>

