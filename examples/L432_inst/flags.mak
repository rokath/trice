
FLAGS = # common flags for compiler, assembler and linker
#FLAGS += -mcpu=cortex-m0plus #
###FLAGS += -mthumb
###FLAGS += -std=gnu11
#FLAGS += -flto#                  # -flto  ist link time optimization. It should be viewed as a last resort optimization. (https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags#-flto)

###  ASFLAGS = #
###  ASFLAGS += $(FLAGS) #
###  ASFLAGS += -g #
### ### ASFLAGS += -gdwarf-4
###  ASFLAGS += -Wa #
### #ASFLAGS += -defsym,__MICROLIB=1
### #ASFLAGS += -c $< 
### #ASFLAGS += -o $@   

#C_FLAGS = #

 C_FLAGS += $(FLAGS)
#C_FLAGS += -x c #                   # "since -x c tells the compiler that "no matter what extension I use, the following files are C source files (until '-x none' )" (https://stackoverflow.com/questions/22283940/return-to-native-file-treatment-after-x-c)
#C_FLAGS += -fno-rtti #               # command-line option '-fno-rtti' is valid for C++/D/ObjC++ but not for C
### C_FLAGS += -funsigned-char        #
### C_FLAGS += -fshort-wchar          #
 C_FLAGS += -fdata-sections #        # https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags#-ffunction-sections--fdata-sections----gc-sections
 C_FLAGS += -ffunction-sections #    # With -ffunction-sections, each function gets its own section.
### C_FLAGS += -fno-common            # -fno-common disables the ability for tentative definitions to be merged into a pre-existing definition. (https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags#-fno-common)
### C_FLAGS += -fstack-usage          # -fstack-usage emit .su files for each c file you compile. 
 C_FLAGS += -fshort-enums          # (https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags#-fshort-enum) (C_FLAGS += -fno-short-enums))
#C_FLAGS += -fpack-struct=n        # (https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags#-fpack-structn)
#C_FLAGS += -ffast-math            # -ffast-math �[breaks] IEEE or ISO rules/specifications for math function� in the name of faster code.

 C_FLAGS += -MMD #                   # Generate dependency information
 C_FLAGS += -MP #                    # Generate dependency information
 C_FLAGS += -MF"$(@:%.o=%.d)" #      # Generate dependency information

#C_FLAGS += --sysroot=.
#C_FLAGS += -O0                    # -O0 means: 'do not optimize at all'.
#C_FLAGS += -O1                    # -O1 means: 'optimize reasomable'.
#C_FLAGS += -O2                    # -O2 means: 'optimize better'.
#C_FLAGS += -O3                    # -O3 means: 'the target machine has effectively unlimited RAM and icache, spew all the assembly you want if it has any benefit'.
C_FLAGS += -Ofast                 # -Ofast optimizes for speed.
#C_FLAGS += -Oz                    # -Oz optimizes for size.
###C_FLAGS += -Os                    # -Os enables a good balance of flags which optimize for size as well as speed. 
#C_FLAGS += -Og#                   # -Og

#C_FLAGS += -g #                     # -g includes debug information into the ELF.
 C_FLAGS += -g3 #                   # -g3 include, a couple extra goodies such as macro definitions used in your application.

#C_FLAGS += -Wunused-parameter

 C_FLAGS += -Wall #                 # -Wall enables a base set of warnings generally agreed upon as being useful and easy to fix.
### C_FLAGS += -Wextra                # -Wextra enables an additional set of flags not covered by -Wall
#C_FLAGS += -Werror                # -Werror causes all enabled warnings to cause compilation errors. 
#C_FLAGS += -Wdocumentation        # -Wdocumentation can catch errors in comments.
                                   # -Wno-<warning> to disable the warning altogether
                                   # -Wno-error=<warning> to keep the warning enabled but not trigger an error.
### C_FLAGS += -Wshadow               # -Wshadow detects shadowing variables.
### C_FLAGS += -Wdouble-promotion     # -Wdouble-promotion catches implicit double promotion takes place. (https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags#-wdouble-promotion)
#C_FLAGS += -Wformat               #
#C_FLAGS += -Wformat=2             #
#C_FLAGS += -Wformat-security      #
#C_FLAGS += -Wformat-overflow      #
#C_FLAGS += -Wformat-truncation    #
### C_FLAGS += -Wundef                # -Wundef warns, if an undefined macro silently evaluating as 0 and causing unexpected behavior.
#C_FLAGS += -Wstack-usage=256      # -Wstack-usage=<limit> emits a warning when stack usage exceeds limit bytes.
### C_FLAGS += -Wstack-protector
##C_FLAGS += -Wconversion           # -Wconversion generates warnings when implicit conversions that are likely to change the underlying value take place. (https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags#-wconversion)
#C_FLAGS += -Wpadded               # -Wpadded warns if padding is added to a structure due to alignment requirements.
### C_FLAGS += -Wunused-parameter     # -Wunused-parameter will detect when a parameter passed to a function is never used in the function. 
#C_FLAGS += -Wl,--no-warn-rwx-segments
#C_FLAGS += -Wl,--no-warn-execstack # https://www.redhat.com/en/blog/linkers-warnings-about-executable-stacks-and-segments

#C_FLAGS += -mfloat-abi=hard       #
#C_FLAGS += -mfpu=fpv4-sp-d16      #

#C_FLAGS += -c $<                  # the input file
#C_FLAGS += -o $@                  # the output file

#C_FLAGS += -pedantic              #

#ifeq ($(DEBUG), 1)
#C_FLAGS += -g #
 C_FLAGS += -gdwarf-4 #
#endif

# libraries
LIBS = #
#LIBS += -nodefaultlibs# You disable the important libgcc library when you pass -nodefaultlibs (implied by -nostdlib). The compiler needs this library for many operations that it cannot do itself or that is more efficient to put into a shared function. 
                       # disables the default setting "-lc -lm -lclang_rt.builtins-arm"
#LIBS += -lgcc# You must pass this library near the end of the link line, after all the other object files and libraries, or the linker won't use it and you get strange linker errors.
 LIBS += -lc #
 LIBS += -lm #
 LIBS += -lnosys #

LIB_DIR = #
#LIB_DIR +="-LC:\\LLVM\\lib\\clang\\16\\lib\\baremetal"
#LIB_DIR +="-LC:\\LLVM\\bin\\../lib/clang-runtimes\\arm-none-eabi\\lib" 
#########LIB_DIR +="-LC:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\12.2 mpacbti-rel1\lib\gcc\arm-none-eabi\12.2.1" #

 LFLAGS += $(FLAGS) #
 LFLAGS += -T $(LDSCRIPT) #
 LFLAGS += $(LIBS) #
 LFLAGS += $(LIB_DIR) #
 LFLAGS += -nostdlib # -nostdlib is used to link your own libc implementation.
#LFLAGS += -flto
#LFLAGS += --library_type=microlib 
#LFLAGS += --diag_suppress 170  
#LFLAGS += --strict                         # generated witth STM32CubMX as ARM-MDK project and copied from there, but "clang: error: unsupported option '--strict'"
#LFLAGS += --cpu Cortex-M0+                 # generated witth STM32CubMX as ARM-MDK project and copied from there, but "clang: error: unsupported option '--cpu'"
#LFLAGS += -specs=nano.specs
#LFLAGS += -Wl #
#LFLAGS += -Wl,--no-warn-rwx-segments #     # https://stackoverflow.com/questions/73429929/gnu-linker-elf-has-a-load-segment-with-rwx-permissions-embedded-arm-project
#LFLAGS += -Map=$(BUILD_DIR)/$(TARGET).map
#LFLAGS += --cref 
#LFLAGS += --gc-sections# --gc-sections drops unused sections. (https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags#-ffunction-sections--fdata-sections----gc-sections)
#LFLAGS += --print-gc-sections# --print-gc-sections prints diagnostic logs about the sections dropped. 
#LFLAGS += -L$(ARM_CORTEXM_SYSROOT)/lib/$(ARM_CORTEXM_MULTI_DIR)

# LFLAGS (https://clang.llvm.org/docs/CrossCompilation.html)

LFLAGS += -Wl,--no-warn-rwx-segments # https://www.redhat.com/en/blog/linkers-warnings-about-executable-stacks-and-segments
LFLAGS += -Wl,--no-warn-execstack    # https://www.redhat.com/en/blog/linkers-warnings-about-executable-stacks-and-segments
