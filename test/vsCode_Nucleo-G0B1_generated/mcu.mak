

 
# fpu

# float-abi
#MCU = $(CPU) $(FPU) $(FLOAT-ABI)

# cpu
CPU =
 CPU +=  -mcpu=cortex-m0plus # https://gcc.gnu.org/legacy-ml/gcc-patches/2014-10/msg02932.html
 CPU += -mtune=cortex-m0plus # For some ARM implementations better performance can be obtained by using this option. (https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html)
#CPU +=  -mcpu=cortex-m0plus.small-multiply # https://gcc.gnu.org/legacy-ml/gcc-patches/2014-10/msg02932.html
#CPU += -mtune=cortex-m0plus.small-multiply # For some ARM implementations better performance can be obtained by using this option. (https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html)
#CPU += -mthumb #

# fpu
FPU =
# NONE for Cortex-M0/M0+/M3

# float-abi
FLOAT-ABI =

# mcu
MCU = $(CPU) $(FPU) $(FLOAT-ABI)
 ASFLAGS += -mcpu=cortex-m0plus

 C_FLAGS += -mcpu=cortex-m0plus

 LDSCRIPT = STM32G0B1RETx_FLASH.ld
 