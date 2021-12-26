
# Quick start guide

- Download latest release assets for your system: Source code and compressed binaries.
- Place the **trice** binary somewhere in your [PATH](https://en.wikipedia.org/wiki/PATH_(variable)).
- In a console type `trice help` 
- Copy 3 files to your embedded project:
  - `./pkg/src/trice.h`
  - `./pkg/src/trice.c`
  - `./test/.../triceConfig.h`
- In your source.c: `#include "trice.h"`
- In a function: `TRICE( "Coming soon: %d!\n", 2022 );`
- In project root:
  - Create empty file: `touch til.json`.
  - Run `trice u` should:
    - patch source.c to `TRICE( Id(12345), "Coming soon: %d!\n", 2022 );`
    - extend `til.json`
- Modify `triceConfig.h` acording your needs.
  - With `#define TRICE_MODE 0` (direct mode) just provide a **putchar()** function.
  - Recommended is an indirect mode which allows to use `TRICE` macros also inside interrupts.
    - Compare 
      - the **not** instrumented test project [./test/MDK-ARM_STM32F030R8_generated]([./test/MDK-ARM_STM32F030R8_generated) 
      - with the instrumented test project [./test/MDK-ARM_STM32F030R8]([./test/MDK-ARM_STM32F030R8) to see an implementation.
- Compile, load and start your app.
- In project root command like `trice l -p COM3 -baud 57600` should show `Coming soon: 2022!` after app start.
- Look in `./pkg/src/triceCheck.c` for examples.
- The used serial Go driver package is Linux & Windows tested.

<!--

## Embedded system code setup

- It is sufficient for most cases just to use the `TRICE` macro with max 0 to 12 parameters as a replacement for `printf` and to use the default settings.
- **Or** follow these steps for instrumentation information even your target processor is not an ARM (any bit width will do):
  - Install the free [STCubeMX](https://www.st.com/en/development-tools/stm32cubemx.html).
  - Choose from [test examples](https://github.com/rokath/trice/tree/master/test) the for you best fitting project `MyExample`.
  - Open the `MyExample.ioc` file with [STCubeMX](https://www.waveshare.com/wiki/STM32CubeMX_Tutorial_Series:_Overview) and generate without changing any setting.
  - Make an empty directory `MyProject` inside the `test` folder and copy the `MyExample.ioc` there and rename it to `MyProject.ioc`.
  - Open `MyProject.ioc` with [STCubeMX](https://www.waveshare.com/wiki/STM32CubeMX_Tutorial_Series:_Overview), change in projects settings `MyExample` to `MyProject` and generate.
  - Now compare the directories `MyExample` and `MyProject` to see the *trice* instrumentation as differences.
- For compiler adaption see [triceConfigCompiler.h](./pkg/src/intern/triceConfigCompiler.h).
- For hardware adaption see [triceUART_LL_STM32](./pkg/src/intern/triceUART_LL_STM32.h)

-->

## Option: build `trice` tool from Go sources

- Install [Go](https://golang.org/).
- On Windows you need to install [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/) - recommendation: Minimal online installer.
  - GCC is only needed for [./pkg/src/src.go](https://github.com/rokath/trice/blob/master/pkg/src/src.go), what gives the option to test the C-code on the host.
  - Make sure TDM-GCC is found first in the path.
  - Other gcc variants could work also but not tested.
- Open a console inside the `trice` directory.
- Check and install:

```b
go vet ./...
go test ./...
go install ./...
```

Afterwards you should find an executable `trice` inside $GOPATH/bin/


## Target configuration and options

* Each project gets its own `triceConfig.h` file.
* Choose the *trice* mode here:
  * Direct mode: Straight output inside `TRICE` macro at the cost of the time it takes.
  * Indirect mode: Background output outside `TRICE` macro at the cost of RAM buffer needed.
* Set Options:
  * Target timestamps and their time base
  * Cycle counter
  * Allow `TRICE` usage inside interrupts
  * Buffer size
