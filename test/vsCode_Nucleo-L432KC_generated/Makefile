

MAKEFILE = Makefile mcu.mak flags.mak gcc0.mak gcc1.mak clang0.mak clang1.mak
VERBOSE = # -v

######################################
# target
######################################
TARGET = L432KC


######################################
# building variables
######################################
# debug build?
DEBUG = 1



######################################
# source
######################################
# C sources
C_SOURCES =  \
Core/Src/main.c \
Core/Src/freertos.c \
Core/Src/stm32l4xx_it.c \
Core/Src/stm32l4xx_hal_msp.c \
Core/Src/stm32l4xx_hal_timebase_tim.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_utils.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_exti.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_gpio.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usart.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_rcc.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_dma.c \
Core/Src/system_stm32l4xx.c \
Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
Middlewares/Third_Party/FreeRTOS/Source/list.c \
Middlewares/Third_Party/FreeRTOS/Source/queue.c \
Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
Middlewares/Third_Party/FreeRTOS/Source/timers.c \
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c  

# ASM sources
ASM_SOURCES =  \
startup_stm32l432xx.s

include gcc0.mak
include clang0.mak
include mcu.mak

#######################################
# CFLAGS
#######################################


# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_FULL_LL_DRIVER \
-DSTM32L432xx \
-DUSE_HAL_DRIVER


# AS includes
AS_INCLUDES =  \
-ICore/Inc

# C includes
C_INCLUDES =  \
-ICore/Inc \
-IDrivers/STM32L4xx_HAL_Driver/Inc \
-IDrivers/STM32L4xx_HAL_Driver/Inc/Legacy \
-IMiddlewares/Third_Party/FreeRTOS/Source/include \
-IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS \
-IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-IDrivers/CMSIS/Device/ST/STM32L4xx/Include \
-IDrivers/CMSIS/Include

include flags.mak

.PHONY: all gcc clean
.SECONDARY: main-build

all: pre-build main-build

pre-build:
	@#echo PRE ...
	@#echo PRE ... done
	@#echo -e

post-build:
	@#echo POST ...
	@#echo POST ... done
	@#echo -e

main-build: clang

clang: $(CLANG_BUILD)/$(TARGET).elf $(CLANG_BUILD)/$(TARGET).hex $(CLANG_BUILD)/$(TARGET).bin
	@$(MAKE) --no-print-directory post-build

gcc:     $(GCC_BUILD)/$(TARGET).elf   $(GCC_BUILD)/$(TARGET).hex   $(GCC_BUILD)/$(TARGET).bin
	@$(MAKE) --no-print-directory post-build

#all:      gcc clang
#	@ls -l build/*.o
#	@ls -l build/$(TARGET).*
#	@ls -l $(GCC_OBJECTS) 
#	@ls -l $(GCC_BUILD)/$(TARGET).*
#	@ls -l $(CLANG_OBJECTS)
#	@ls -l $(CLANG_BUILD)/$(TARGET).*

#######################################
# build the application
#######################################
include gcc1.mak
include clang1.mak

clean:
	@-rm -fR $(GCC_BUILD) $(CLANG_BUILD) 
  
# openocdflashgcc: all
# 	openocd -f interface/jlink.cfg -f target/stm32g0x.cfg -c "program $(GCC_BUILD)/$(TARGET).elf verify reset exit"
# 
# openocdflashclang: all
# 	openocd -f interface/jlink.cfg -f target/stm32g0x.cfg -c "program $(CLANG_BUILD)/$(TARGET).elf verify reset exit"
# 
# seggerflashgcc: all
# 	JFlash -openprj G0B1.jflash -open $(GCC_BUILD)/G0B1.hex,0x08000000 -auto -startapp -exit
# 
# seggerflashclang: all
# 	JFlash -openprj G0B1.jflash -open $(CLANG_BUILD)/G0B1.hex,0x08000000 -auto -startapp -exit
# 
# # See UM0892
# stflashgcc: all
# 	ST-LINK_CLI.exe -c SWD FREQ=4000 UR -P $(GCC_BUILD)/G0B1.hex -V
# 
# # See UM0892
# stflashclang: all
# 	ST-LINK_CLI.exe -c SWD FREQ=4000 UR -P $(CLANG_BUILD)/G0B1.hex -V
