

 
# fpu

# float-abi
#MCU = $(CPU) $(FPU) $(FLOAT-ABI)

# cpu
CPU =
 CPU +=  -mcpu=cortex-m4 # https://gcc.gnu.org/legacy-ml/gcc-patches/2014-10/msg02932.html
 CPU += -mtune=cortex-m4 # For some ARM implementations better performance can be obtained by using this option. (https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html)
#CPU +=  -mcpu=cortex-m4.small-multiply # https://gcc.gnu.org/legacy-ml/gcc-patches/2014-10/msg02932.html
#CPU += -mtune=cortex-m4.small-multiply # For some ARM implementations better performance can be obtained by using this option. (https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html)
#CPU += -mthumb #

# fpu
FPU = -mfpu=fpv4-sp-d16
# NONE for Cortex-M0/M0+/M3

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) $(FPU) $(FLOAT-ABI)


 ASFLAGS += -mcpu=cortex-m4

 C_FLAGS += -mcpu=cortex-m4
 C_FLAGS += -mthumb
 C_FLAGS += -mfpu=fpv4-sp-d16
 C_FLAGS += -mfloat-abi=hard 

 # link script
LDSCRIPT = STM32L432KCUx_FLASH.ld