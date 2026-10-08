# Target Integration Sources for Trice

This folder contains the target-side Trice instrumentation sources.

These files belong together. They are not unrelated utility files. A product that
uses Trice on the target typically integrates this whole set together with a
project-specific `triceConfig.h`.

If you found this folder while inspecting an existing product, the most likely
interpretation is:

- The firmware uses [Trice](https://github.com/rokath/trice) for target-side logging and instrumentation.
- The host-side decoding tool is not in this folder. It belongs to the same
  Trice project, but is built from the Go sources of the full repository.
- The target-side configuration is usually provided by the product itself in a
  local `triceConfig.h`.

## Essential Overview

- Include [trice.h](./trice.h) in target source files that use Trice macros.
- Including [trice.h](./trice.h) pulls in the local `triceConfig.h`
  implicitly and also includes
  [triceDefaultConfig.h](./triceDefaultConfig.h), which supplies the defaults
  for all options not overridden by the product.
- The files in this folder are library files and should normally not be changed
  inside a product. The project-specific settings belong into `triceConfig.h`.
- [triceDefaultConfig.h](./triceDefaultConfig.h) is not only a defaults file
  but also an important compact reference for the available configuration
  options.
- The host-side workflow additionally needs a project-specific `til.json`, and
  optionally `li.json` when location information is used.
- Default target stamp examples defined in
  [triceDefaultConfig.h](./triceDefaultConfig.h) should get user-specific
  definitions.

Project-level documentation:

- Repository: [github.com/rokath/trice](https://github.com/rokath/trice)
- Main README: [README.md](../README.md)
- Reference Manual: [TriceReferenceManual.md](../docs/TriceReferenceManual.md)
  - Generated release asset: `TriceReferenceManual.pdf`

## Configuration Model

The normal pattern is:

1. The product provides its own `triceConfig.h`.
2. `triceConfig.h` overrides only the settings that differ from the defaults.
3. The remaining defaults come from [triceDefaultConfig.h](./triceDefaultConfig.h).

Minimal example:

```c
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#define TRICE_DEFERRED_UARTA 1 // 1: use TRICE_UARTA for deferred output
#define TRICE_UARTA USART2     // Use the hardware-specific device USART2 as TRICE_UARTA.
                               // The symbol is typically provided by a user-specific triceUart.h file.
                               // The file triceUart.h is automatically included from the library file triceUart.c.

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER

#ifdef __cplusplus
}
#endif

#endif // TRICE_CONFIG_H_
```

Important:

- Define your project-specific overrides only in `triceConfig.h`.
- Do not edit [triceDefaultConfig.h](./triceDefaultConfig.h) for product-specific settings unless you intentionally want to fork the defaults.

## Notes

- `SEGGER_RTT.*` is included here because RTT is a common direct-output path for Trice.
  The unmodified vendor files `SEGGER_RTT.c`, `SEGGER_RTT.h`, and
  `SEGGER_RTT_ConfDefaults.h` come from
  [SEGGERMicro/RTT commit 4d8feab3150f86f37a9d323ddc88d6cdf5673072](https://github.com/SEGGERMicro/RTT/tree/4d8feab3150f86f37a9d323ddc88d6cdf5673072)
  (2026-06-03). Their [SEGGER license](./SEGGER_RTT_LICENSE.md) must accompany
  source redistributions, including target-source release archives.
  This source snapshot is independent of the installed J-Link host-tool version.
  Keep project and generated include directories first, then `src`, then
  `src/default_conf` last. The small fallback configuration selects Trice's
  channel counts and C implementation; SEGGER's defaults supply the remaining
  settings. A project's `SEGGER_RTT_Conf.h` takes precedence.
- `xtea.*` is optional and only relevant when encrypted Trice transport is enabled.
- `cobs*` and `tcobs*` are framing helpers and may or may not be used by a product.

If this folder appears in a third-party firmware tree, that firmware is very
likely carrying a local copy of the Trice target instrumentation library.
