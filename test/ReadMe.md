# Examples and Testing the `trice` C-code

## Examples


| Project Name | Description |
| - | - |
| [MDK-ARM_STM32F030R8_generated](./MDK-ARM_STM32F030R8_generated) | For the [NUCLEO-STM32F030R8](https://www.st.com/en/evaluation-tools/nucleo-f030r8.html) with the free [CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) generated empty project as compare base for the trice instrumentation. |
| [MDK-ARM_STM32F030R8](./MDK-ARM_STM32F030R8) | The [MDK-ARM_STM32F030R8_generated](./MDK-ARM_STM32F030R8_generated) project was trice instrumented for parallel usage of RTT and UART transfer. |
| [OpenCM3_STM32F411_Nucleo](./OpenCM3_STM32F411_Nucleo) | See the [Readme.md](./OpenCM3_STM32F411_Nucleo/Readme.md) in this folder.

## Important to know

The [ARM-Keil µVision IDE](https://www2.keil.com/mdk5/uvision/) does sometimes not recognize external file modifications. That means for example: After editing `main.c` by adding a `trice( "Hi!\n" )` and executing `trice u` as pre-compile step it could happen, that an updated `trice( iD(12345), "Hi!\n" )`  was inserted and correct compiled but the update in `main.c` is not shown. Simply close and reopen `main.c` before editing again. This seem to be a [ARM-Keil µVision IDE](https://www2.keil.com/mdk5/uvision/) "feature".

## Tests

The `cgo_...` folders are serving for target code testing in different configuration variants on the host machine. The file [./testdata/triceCheck.c.txt](./testdata/triceCheck.c.txt) is the master file for editing. After changing it, [./updateTestData.sh](./updateTestData.sh) needs to run. It copies [./testdata/triceCheck.c.txt](./testdata/triceCheck.c.txt) into [./testdata/triceCheck.c](./testdata/triceCheck.c) and runs `trice u` on it regenerating [./testdata/til.json](./testdata/til.json). It then distributes some file copies. [./testdata/cgoPackage.go](./testdata/cgoPackage.go) is the common master for the `cgo_.../generated_cgoPackage.go` files and contains the common test code. The folders `cgo_...` are Go packages just for tests. They all have the same package name`cgot` and are not included ito the trice tool. The `cgo_.../triceConfig.h` differ and correspondent to the `cgo_.../cgo_test.go` files. On test execution, the `./testdata/*.c` files are compiled into the trice test executable using the `cgo_.../triceConfig.h` file. The test collects the expected results (`//exp: result`) together with the line numbers into a slice to execute the test loop on it. The `triceLogTest` function gets the `triceLog` function as parameter. `triceLogTest` iterates over the results slice and calls for each line the C-function `triceCheck`. Then the line specific binary data buffer is passed to the `triceLog` parameter function which "logs" the passed buffer into an actual result string which in turn is compared with the expected result. The whole process is relatively slow, but allows automated tests in many different configuration variants.

Also the `[MDK-ARM_STM32F030R8](./MDK-ARM_STM32F030R8)` includes [./testdata/triceCheck.c](./testdata/triceCheck.c) and performs a subset of line calls on it.
