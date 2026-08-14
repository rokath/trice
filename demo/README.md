# Minimal Trice Demos

These two small PC programs demonstrate the same Trice output channel in two
modes:

- `direct`: Each Trice call writes its binary data immediately to
  `build/log.bin`.
- `deferred`: The Trice calls first write to a ring buffer. `TriceTransfer()`
  subsequently transfers the data to `build/log.bin`.

Both programs use the Trice sources from `../src` directly. No library is
copied, and no build system is required.

## Requirements

Only the following tools are needed:

- `trice` in `PATH`,
- a C compiler available as `cc` or `gcc`,
- Bash or a comparable POSIX shell.

The concise, commented-out prerequisite checks at the beginning of `demo.sh`
can be enabled if needed. The script does not install anything automatically.

## Running the Demos

Run the central script from the `demo` directory:

```sh
cd demo
./demo.sh
```

It runs `trice bind` once for both programs, then builds and starts `deferred`
followed by `direct`. Finally, it displays both log files in sequence.

Each demo has its own `build` directory. These directories contain generated
files only:

```text
demo/build/triceIDs/       shared generated bind headers
demo/deferred/build/       demo_deferred and log.bin
demo/direct/build/         demo_direct and log.bin
```

On Windows, the application name additionally has the `.exe` suffix. The script
automatically starts it under the correct name.

`trice bind` runs without options from the demo directory and therefore uses
`til.json`, `li.json`, and `build/triceIDs` directly. `trice log` also runs from
this directory and needs only `FILEBUFFER` and the path to the respective
`log.bin`. These demos do not require `tlog`.

On the first run, `trice bind` automatically adds the initially unfamiliar line
`#include "trice_main_c_K...h"` to each `main.c`. The file name is generated; the
user neither writes nor maintains it. The included header is then located under
`build/triceIDs`.

## Shared and Local Files

`til.json` and `li.json` are shared in this directory. They contain the IDs and
source locations of all executed demos and belong to the demo project.

The compiler glob `../src/[a-z]*.c` selects all regular Trice sources. The unused
vendor file `SEGGER_RTT.c` starts with an uppercase letter and is therefore
excluded. A dummy SEGGER RTT configuration is not required.

Each subdirectory contains only its own application configuration:

```text
direct/ or deferred/
├── main.c
├── triceConfig.h
└── build/                 generated and not versioned
```

The shared workflow is defined only in `demo.sh`.
