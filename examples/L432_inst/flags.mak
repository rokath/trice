
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




 C_FLAGS += -Wall                             # -Wall enables a base set of warnings generally agreed upon as being useful and easy to fix.
 C_FLAGS += -Walloca                          # This option warns on all uses of "alloca" in the source. 
 C_FLAGS += -Wno-builtin-declaration-mismatch # Warn if a built-in function is declared with an incompatible signature.
 C_FLAGS += -Wunknown-pragmas                 # Warn when a pragma() is encountered that is not understood by gdc. 

 C_FLAGS += -Wextra                # -Wextra enables an additional set of flags not covered by -Wall
 C_FLAGS += -Wno-unused-parameter  # Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c:...
 C_FLAGS += -Wconversion           # Warn for implicit conversions that may alter a value.
 C_FLAGS += -Wno-sign-conversion   # Drivers/CMSIS/Include/core_cm4.h
 C_FLAGS += -Wno-conversion        # Drivers/CMSIS/Device/ST/STM32L4xx/Include/stm32l4xx.h
#C_FLAGS += -Werror                # -Werror causes all enabled warnings to cause compilation errors. 
                                   # -Wno-<warning> to disable the warning altogether
                                   # -Wno-error=<warning> to keep the warning enabled but not trigger an error.
 C_FLAGS += -Wshadow               # -Wshadow detects shadowing variables.
 C_FLAGS += -Wdouble-promotion     # -Wdouble-promotion catches implicit double promotion takes place. (https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags#-wdouble-promotion)
 C_FLAGS += -Wformat=2             # Enable -Wformat plus format checks not included in -Wformat. Currently equivalent to `-Wformat -Wformat-nonliteral -Wformat-security -Wformat-y2k'. 
 C_FLAGS += -Wformat-overflow      #
 C_FLAGS += -Wformat-truncation    #
 C_FLAGS += -Wundef                # -Wundef warns, if an undefined macro silently evaluating as 0 and causing unexpected behavior.
 C_FLAGS += -Wstack-usage=400      # -Wstack-usage=<limit> emits a warning when stack usage exceeds limit bytes.
 C_FLAGS += -Wstack-protector
 C_FLAGS += -Wmissing-include-dirs # Warn if a user-supplied include directory does not exist
 C_FLAGS += -Wswitch-default       # Warn whenever a switch statement does not have a default case.
#C_FLAGS += -Wswitch-enum          # Warn whenever a switch statement has an index of enumerated type and lacks a case for one or more of the named codes of that enumeration. case labels outside the enumeration range also provoke warnings when this option is used. 
#C_FLAGS += -Wunused-parameter     # Warn whenever a function parameter is unused aside from its declaration. To suppress this warning use the `unused' attribute (see Variable Attributes). 
 C_FLAGS += -Wunknown-pragmas
 C_FLAGS += -fstrict-aliasing
#C_FLAGS += -fstrict-overflow
#C_FLAGS += -Wstrict-overflow=5    # TODO: enable
 C_FLAGS += -Wno-sign-compare      # TODO: disable
 C_FLAGS += -Wfloat-equal          # Warn if floating point values are used in equality comparisons.
 C_FLAGS += -Wundef                # Warn if an undefined identifier is evaluated in an `#if' directive. 
#C_FLAGS += -Wlarger-than=512      # Warn whenever an object of larger than len bytes is defined.
 C_FLAGS += -Wframe-larger-than=400#Warn if the size of a function frame is larger than len bytes. The computation done to determine the stack frame size is approximate and not conservative. The actual requirements may be somewhat greater than len even if you do not get a warning. In addition, any space allocated via alloca, variable-length arrays, or related constructs is not included by the compiler when determining whether or not to issue a warning. 
 C_FLAGS += -Wunsafe-loop-optimizations # Warn if the loop cannot be optimized because the compiler could not assume anything on the bounds of the loop indices. With -funsafe-loop-optimizations warn if the compiler made such assumptions. 
 C_FLAGS += -funsafe-loop-optimizations
#C_FLAGS += -pedantic
 C_FLAGS += -Wpointer-arith        # Warn about anything that depends on the “size of” a function type or of void. GNU C assigns these types a size of 1, for convenience in calculations with void * pointers and pointers to functions. In C++, warn also when an arithmetic operation involves NULL. This warning is also enabled by -pedantic.
 C_FLAGS += -Wbad-function-cast    # Warn whenever a function call is cast to a non-matching type. For example, warn if int malloc() is cast to anything *. 
#C_FLAGS += -Wc++-compat           # Warn about ISO C constructs that are outside of the common subset of ISO C and ISO C++, e.g. request for implicit conversion from void * to a pointer to non-void type. 
#C_FLAGS += -Wcast-qual            # Warn whenever a pointer is cast so as to remove a type qualifier from the target type. For example, warn if a const char * is cast to an ordinary char *. 
#C_FLAGS += -Wcast-align           # Warn whenever a pointer is cast such that the required alignment of the target is increased. For example, warn if a char * is cast to an int * on machines where integers can only be accessed at two- or four-byte boundaries. 
 C_FLAGS += -Wwrite-strings        # When compiling C, give string constants the type const char[length] so that copying the address of one into a non-const char * pointer will get a warning.
 C_FLAGS += -Waddress              # Warn about suspicious uses of memory addresses.
 C_FLAGS += -Wlogical-op           # Warn about suspicious uses of logical operators in expressions
#C_FLAGS += -Waggregate-return     # Warn if any functions that return structures or unions are defined or called. (In languages where you can return an array, this also elicits a warning.) 
 C_FLAGS += -Wno-attributes        # Do not warn if an unexpected __attribute__ is used, such as unrecognized attributes, function attributes applied to variables, etc.
 C_FLAGS += -Wstrict-prototypes    # Warn if a function is declared or defined without specifying the argument types.
 C_FLAGS += -Wold-style-definition # Warn if an old-style function definition is used. A warning is given even if there is a previous prototype. 
 C_FLAGS += -Wmissing-prototypes   # Warn if a global function is defined without a previous prototype declaration. This warning is issued even if the definition itself provides a prototype. The aim is to detect global functions that fail to be declared in header files.
 C_FLAGS += -Wmissing-declarations # Warn if a global function is defined without a previous declaration. Do so even if the definition itself provides a prototype. Use this option to detect global functions that are not declared in header files. In C++, no warnings are issued for function templates, or for inline functions, or for functions in anonymous namespaces. 
 C_FLAGS += -Wmissing-noreturn     # Warn about functions which might be candidates for attribute noreturn. Note these are only possible candidates, not absolute ones. Care should be taken to manually verify functions actually do not ever return before adding the noreturn attribute, otherwise subtle code generation bugs could be introduced. You will not get a warning for main in hosted C environments.
 C_FLAGS += -Wmissing-format-attribute # Warn about function pointers which might be candidates for format attributes. Note these are only possible candidates, not absolute ones.
 C_FLAGS += -Wnormalized=nfc       # In ISO C and ISO C++, two identifiers are different if they are different sequences of characters.
 C_FLAGS += -Wpacked               # Warn if a structure is given the packed attribute, but the packed attribute has no effect on the layout or size of the structure.
#C_FLAGS += -Wredundant-decls      # Warn if anything is declared more than once in the same scope, even in cases where multiple declaration is valid and changes nothing. Drivers/CMSIS/Device/ST/STM32L4xx/Include/system_stm32l4xx.h
 C_FLAGS += -Wunreachable-code     # Warn if the compiler detects that code will never be executed
#C_FLAGS += -Winline               # Warn if a function can not be inlined and it was declared as inline. Even with this option, the compiler will not warn about failures to inline functions declared in system headers. 
 C_FLAGS += -Winvalid-pch          # Warn if a precompiled header (see Precompiled Headers) is found in the search path but can't be used. 
 C_FLAGS += -Wvla                  # Warn if variable length array is used in the code. -Wno-vla will prevent the -pedantic warning of the variable length array. 
 C_FLAGS += -Wdisabled-optimization# Warn if a requested optimization pass is disabled.
#C_FLAGS += -Wstack-protector      # This option is only active when -fstack-protector is active. It warns about functions that will not be protected against stack smashing. 
#C_FLAGS += -fstack-protector
 C_FLAGS += -Wno-discarded-qualifiers
 C_FLAGS += -Wno-suggest-attribute=noreturn

#C_FLAGS += -Wnested-externs       # Warn if an extern declaration is encountered within a function. 
#C_FLAGS += -Wpadded               # Warn if padding is included in a structure, either to align an element of the structure or to align the whole structure. Sometimes when this happens it is possible to rearrange the fields of the structure to reduce the padding and so make the structure smaller. 


#C_FLAGS += -Wl,--no-warn-rwx-segments
#C_FLAGS += -Wl,--no-warn-execstack # https://www.redhat.com/en/blog/linkers-warnings-about-executable-stacks-and-segments

#C_FLAGS += -mfloat-abi=hard       #
#C_FLAGS += -mfpu=fpv4-sp-d16      #

#C_FLAGS += -c $<                  # the input file
#C_FLAGS += -o $@                  # the output file

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
