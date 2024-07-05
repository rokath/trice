# Empty example vsCode_NucleoF030R8_instrumented

This is a STMCubeMX instrumented project without TRICE instrumentation for easy compare with [../vsCode_NucleoF030R8_instrumented](../vsCode_NucleoF030R8_instrumented) to figure out the needed changes to set up trice.

## Steps performed as potential guide on a Windows machine

- Install STM32CubeMX to `C:\SMT32SubeMX`.
- Select NUCLEO-F030R8 board.
- Initialize with default values.
- Optionally set system clock to 32MHz for faster target timestamps.
- Optionally set UART baud rate to 115200.
- Mantadory set UART data bits including parity to 9.
- Enable USART2 global interrupt.
- In Project Manager *Project*:
  - Set toolchain folder location to `E:\repos\trice\examples\vsCode_NucleoF030R8_instrumented\`.
  - Set project name to `vsCode_NucleoF030R8_instrumented`.
  - Set toolchain / IDE to `Makefile`.
- In Project Manager *Code Generator*:
  - Select "Copy only the necessary library files".
- In Project Manager *Advanced Settings*:
  - In Driver Selector change all to *LL*.
- Generate Code
- Start vsCode and open instrumented with it.
- Start a terminal and type `make`. The output should be similar to:

```bash
PS E:\repos\trice\examples\vsCode_NucleoF030R8_instrumented> make -j8  
mkdir build
arm-none-eabi-gcc -c -mcpu=cortex-m0 -mthumb   -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=0 -DDATA_CACHE_ENABLE=0 -DSTM32F030x8 -ICore/Inc -IDrivers/STM32F0xx_HAL_Driver/Inc -IDrivers/CMSIS/Device/ST/STM32F0xx/Include -IDrivers/CMSIS/Include -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/main.d" -Wa,-a,-ad,-alms=build/main.lst Core/Src/main.c -o build/main.o

...

arm-none-eabi-gcc -x assembler-with-cpp -c -mcpu=cortex-m0 -mthumb   -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=0 -DDATA_CACHE_ENABLE=0 -DSTM32F030x8 -ICore/Inc -IDrivers/STM32F0xx_HAL_Driver/Inc -IDrivers/CMSIS/Device/ST/STM32F0xx/Include -IDrivers/CMSIS/Include -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/startup_stm32f030x8.d" startup_stm32f030x8.s -o build/startup_stm32f030x8.o
arm-none-eabi-gcc build/main.o build/stm32f0xx_it.o build/stm32f0xx_ll_gpio.o build/stm32f0xx_ll_pwr.o build/stm32f0xx_ll_exti.o build/stm32f0xx_ll_usart.o build/stm32f0xx_ll_rcc.o build/stm32f0xx_ll_dma.o build/stm32f0xx_ll_utils.o build/system_stm32f0xx.o build/sysmem.o build/syscalls.o build/startup_stm32f030x8.o  -mcpu=cortex-m0 -mthumb   -specs=nano.specs -TSTM32F030R8Tx_FLASH.ld  -lc -lm -lnosys  -Wl,-Map=build/vsCode_NucleoF030R8_instrumented.map,--cref -Wl,--gc-sections -o build/vsCode_NucleoF030R8_instrumented.elf
C:/bin/ArmGNUToolchain/bin/../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/bin/ld.exe: warning: build/vsCode_NucleoF030R8_instrumented.elf has a LOAD segment with RWX permissions
arm-none-eabi-size build/vsCode_NucleoF030R8_instrumented.elf
   text    data     bss     dec     hex filename
   2428      12    1564    4004     fa4 build/vsCode_NucleoF030R8_instrumented.elf
arm-none-eabi-objcopy -O ihex build/vsCode_NucleoF030R8_instrumented.elf build/vsCode_NucleoF030R8_instrumented.hex
arm-none-eabi-objcopy -O binary -S build/vsCode_NucleoF030R8_instrumented.elf build/vsCode_NucleoF030R8_instrumented.bin
PS E:\repos\trice\examples\vsCode_NucleoF030R8_instrumented>
```

- If not done yet, install vsCode Cortex-Debug extension.
- Create a launch.json file inside the *.vscode* subfolder and edit it to get

```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Cortex Debug",
            "cwd": "${workspaceFolder}",
            "executable": "./build/vsCode_NucleoF030R8_instrumented.elf",
            "request": "launch",
            "type": "cortex-debug",
            "runToEntryPoint": "main",
            "servertype": "jlink",
            "device": "STM32F030R8",
            "svdFile": "./STM32F030R8.svd",
            "runToMain": true

        }
    ]
}
```

- Download [STM32G030.svd](https://github.com/fullyautomated/st-svd/blob/main/STM32G030.svd) or get it from the STMCubeIDE installation folder if you want to install this Eclipse IDE as well, but IMHO you do not need it.
- You may need to extract and install the STM32 USB drivers *"trice/third_party/st.com/en.stsw-link009_v2.0.2.zip"*. You can get them also from the STM site directly.
- It is assumed, that you converted the OB ST-Link to an OB J-Link already. See *trice/docs/TRICEOverRTT.md#J-Linkoption* for details.

- Press the Debug-Button or "CTRL+SHIFT+D" and start debugging.

## Intrumenting

- The above steps are de-facto identical to the steps performed in *trice/examples/vsCode_NucleoF030R8_generated*
- During the code generation, the CubeMX tool did not copy `syscalls.c` and `sysmem.c` but added them to the Makefile. This seems to be a STM32CubeMX bug.
  - You do not need these files for the example project, but you can add them manually to avois some warnings.

- Extend the Makefile with:

```mak
C_SOURCES += \
../../src/cobsDecode.c \
../../src/cobsEncode.c \
../../src/SEGGER_RTT.c \
../../src/tcobsv1Decode.c \
../../src/tcobsv1Encode.c \
../../src/trice.c \
../../src/trice16.c \
../../src/trice32.c \
../../src/trice64.c \
../../src/trice8.c \
../../src/triceAuxiliary.c \
../../src/triceDoubleBuffer.c \
../../src/triceRingBuffer.c \
../../src/triceStackBuffer.c \
../../src/triceStaticBuffer.c \
../../src/xtea.c 

C_INCLUDES += \
-I../../src

```


- Add `pre-build:` and `post-build:` before the default action to get:

```mak
pre-build:
	@echo "PRE ..."; \
	trice insert -src ../../src -src ./ -src ../exampleData; \
	echo "PRE ... done"; \
	make -j $(nproc) all

post-build:
	@echo POST ...
	@trice clean -src ../../src -src ./ -src ../exampleData
	@echo POST ... done
	@#echo -e

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin
```

- Copy file *SEGGER_RTT_Conf.h* from *trice/third_party/segger.com/SEGGER_RTT_V760g.zip* to *trice/examples/vsCode_NucleoF030R8_instrumented/Core/Inc/SEGGER_RTT_Conf.h*.
- Copy file *trice/examples/vsCode_Nucleo-L432KC_instrumented/Core/Inc/triceConfig.h* to *trice/examples/vsCode_NucleoF030R8_instrumented/Core/Inc/triceConfig.h*. You can also choose from an other example project.
- Create 2 empty files: `touch til.json li.json`
- Execute `make`. This should build all.
- Add `#include "trice.h"` to *main.c* and to *stm32f0xx_it.c*.
- Add `uint32_t milliSecond( void );` to *triceCondig.h*.
- Add to `int main( void )`:

```C
    TriceInit();
    //! This is usable as the very first trice sequence after restart. Adapt it. Use a UTF-8 capable editor like VS-Code or use pure ASCII.
    trice( "\n\n        ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨        \n        🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-F030R8   🎈🎈🎈🎈\n        🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃        \n\n\n");

    TRice( "w: Hello! 👋🙂 \a\n" ); // with sound!
    TRice( "w: Hello! 👋🙂 \a\n" ); // with sound!
    Trice( "w: Hello! 👋🙂 \a\n" ); // with sound!
    Trice( "w: Hello! 👋🙂 \a\n" ); // with sound!
    trice( "w: Hello! 👋🙂 \a\n" ); // with sound!
    trice( "w: Hello! 👋🙂 \a\n" ); // with sound!
```

- Extend *stm32f0xx_it.c* with:

```C
// ms32 is a 32-bit millisecond counter, counting circular in steps of 1 every ms.
uint32_t ms32 = 0;

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
    ms32++;
  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */
#if defined( TRICE_UARTA ) && ((TRICE_BUFFER == TRICE_DOUBLE_BUFFER) || (TRICE_BUFFER == TRICE_RING_BUFFER) ) // buffered out to UART
    triceTriggerTransmitUartA();
#endif
#if defined( TRICE_UARTB ) && ((TRICE_BUFFER == TRICE_DOUBLE_BUFFER) || (TRICE_BUFFER == TRICE_RING_BUFFER) ) // buffered out to UART
    triceTriggerTransmitUartB();
#endif
  /* USER CODE END SysTick_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
#if defined( TRICE_UARTA )
    if (LL_USART_IsActiveFlag_RXNE(TRICE_UARTA) ) { // Read Data Register Not Empty Flag 
        static char rxBuf[TRICE_COMMAND_SIZE_MAX+1]; // with terminating 0
        static int index = 0;
        uint8_t v;
        if( LL_USART_IsActiveFlag_ORE(TRICE_UARTA) ){
            TRICE( Id(0), "WARNING:USARTq OverRun Error Flag is set!\n" );
        }
        v = LL_USART_ReceiveData8(TRICE_UARTA); // implicit clears the flag
        rxBuf[index] = (char)v;
        index += index < TRICE_COMMAND_SIZE_MAX ? 1 : 0; 
        if( v == 0 ){ // command end
            TRICE_S( Id(0), "rx:received command:%s\n", rxBuf );
            strcpy(triceCommandBuffer, rxBuf );
            triceCommandFlag = 1;
            index = 0;
        }
        return;
    }
#endif // #if defined( TRICE_UARTA )
    // If both flags active and only one was served, the IRQHandler gets activated again.

#if defined( TRICE_UARTA ) && ((TRICE_BUFFER == TRICE_DOUBLE_BUFFER) || (TRICE_BUFFER == TRICE_RING_BUFFER) ) // buffered out to UARTA
    if( LL_USART_IsActiveFlag_TXE(TRICE_UARTA) ){ // Transmit Data Register Empty Flag
        triceServeTransmitUartA();
        return;
    }
#endif
}
```

- Put into your *triceConfig.h* for the target time stamps:

```C
#include "stm32f0xx_ll_system.h"

//! TriceStamp16 returns a 16-bit value to stamp `Id` TRICE macros. Usually it is a timestamp, but could also be a destination address or a counter for example.
//! The user has to provide this function. Defining a macro here, instead if providing `int16_t TriceStamp16( void );` has significant speed impact.
#define TriceStamp16 (SysTick->VAL) // Counts from 31999 -> 0 in each ms. 

// ms32 is a 32-bit millisecond counter, counting circular in steps of 1 every ms.
extern uint32_t ms32;

//! TriceStamp32 returns a 32-bit value to stamp `ID` TRICE macros. Usually it is a timestamp, but could also be a destination address or a counter for example.
//! The user has to provide this function. Defining a macro here, instead if providing `int32_t TriceStamp32( void );` has significant speed impact.
#define TriceStamp32 ms32 
```

- Run `trice s` to determine the relevant comport.
- You can have 2 outputs:
  - In a terminal run `trice l -p com5 -baud 1000000`.
  - In a terminal run `trice l -p jlink -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000" -d16 -pf none`

## With XTEA

` trice l -p com5 -baud 1000000 -pw MySecret -pf COBS`
