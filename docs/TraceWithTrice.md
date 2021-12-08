# Trace with `TRICE()` and get `printf()` comfort inside interrupts and everywhere

## Description

`TRICE()` is a comfortable macro, generating tiny C-code for getting PC `printf()` comfort at "speed-of-light" for any micro-controller. It is supported by a **Go** written powerful **trice** PC tool running on many platforms and comes with sample implementations. Features:
- super fast: a `TRICE()` macro is executable in less than 10 clocks
- target and host timestamps
- runtime filterable colored channels
- encryption option
- enable/disable `TRICE()` on file level 
- integrate several target devices in one log file
- transmit over TCP/IP to a remote server 
- any byte capable connection usable
- compared to `printf()` usage, less needed FLASH memory

## Author: Thomas Höhenleitner
---

<!--A little bit of background of why the reader should read this post.-->

## Abstract

If you develop software for an embedded system, you always need some kind of system feedback, like an LED on/off. Debuggers are awesome tools, but when it comes to analyze dynamic behavior in the field, they are not usable.

The first choice is to use logging then, usually done with `printf` like functions. Getting quick a result after having a `putchar()` implemented, it turns out to be an expensive way in terms of processor clocks and needed FLASH memory, when you regard the library code and all the strings needing FLASH memory space. For small micro-controllers that´s it.

Bigger micro-controllers are coming with embedded trace hardware. To use it, an expensive tool is needed. Very useful for analyzing complex multi-tasking systems but for field related issues they at least unhandy.

Unhappy with this situation the developer starts thinking of using a spare DAC (digital-analog.converter), using GPIO`s with a logic-analyzer or oscilloscope (both expensive too but usually available) or starts emitting some proprietary LED blinking codes or byte sequences, only understandable for him and only in this year.

The *trice* technique tries to fill this gap trying to be minimal invasive and as comfortable as possible.

## Simplicity is Complicated ...

... we learned from [Rob Pike](https://www.youtube.com/watch?v=rFejpH_tAHM), one of the **Go** inventors and indeed [Go](https://en.wikipedia.org/wiki/Go_(programming_language)) is easy to learn but a very powerful language. The **trice** tool was started in C++ but focusing on content was difficult because of all the C++ nuts and bolts to deal with. After restarting the project in **Go** the implementation started to get amusing.

The same perception exists when using *trice* in an embedded project instead of some other technique. One can simply write `TRICE( "Hello %d, hello, ..., hello %d again!", 88, -88 );` or whatever everywhere without thinking about execution time and FLASH memory size.

This may sound like a miracle, but it is not. *Trice* is the result of a long-year dissatisfaction and several attempts to find a loophole to make embedded programming more fun and this way more effective.



## Conclusion



<!-- Interrupt Keep START -->
<!--
{% include newsletter.html %}
{% include toc.html %}
{% include submit-pr.html %}
{:.no_toc}
-->
<!-- Interrupt Keep END -->

## References

<!-- prettier-ignore-start -->
<!--[^reference_key]:-->
 [*Trice* on Github](https://github.com/rokath/trice)
<!-- prettier-ignore-end -->



## Typical memory regions in an embedded program

For more background, see these references on linker scripts:

- [From Zero to main(): Demystifying Firmware Linker Scripts]({% post_url 2019-06-25-how-to-write-linker-scripts-for-firmware %})
- [Stargirl Flower's _outstanding_ "The most thoroughly commented linker script
  (probably)"
  post](https://blog.thea.codes/the-most-thoroughly-commented-linker-script/)
- [Elecia White's (of embedded.fm !) excellent Memory Map talk](https://www.youtube.com/watch?v=XRXLUcbJIxY)

A typical small embedded device (running bare-metal or an RTOS) will usually
have two types of memory available:

- read-only memory (usually flash)
- read-write memory (usually SRAM)

For example, the STM32F407VG chip has:

- 1 megabyte of flash
- 192 kilobytes of SRAM

On that chip, flash and SRAM are mapped to the following addresses:

```plaintext
┌───────────────────┐0x0800 0000
│Flash (1024kB)     │
└────────┬──────────┘0x080F FFFF
        ...
┌────────┴──────────┐0x1000 0000
│CCM data RAM (64kB)│
└────────┬──────────┘0x1000 FFFF
        ...
┌────────┴──────────┐0x2000 0000
│SRAM (128kB)       │
└───────────────────┘0x2001 FFFF
```

## Memory sections in an embedded program

A simple embedded application targeting the STM32F407VG would usually have these
output sections in its memory map:

```plaintext
❯ arm-none-eabi-size -Ax build/main.elf
build/main.elf  :
section                size         addr
.text                0x1314    0x8000000
.data                  0x78   0x20000000
.bss                   0xdc   0x20000078
```

Where:

- `.text` contains **read-only** data, such as executable code or `const` data
- `.data` contains _statically initialized_ **read-write** data (variables that
  have a non-zero initialization value)
- `.bss` contains _zero-initialized_ **read-write** data

When our program starts, the `.data` section will be loaded from the _load
address_ (LMA), and the `.bss` section will be set to all zeros, as part of the
program's startup code.

## Implementing a `.noinit` section

If we want a section of RAM that is _not_ initialized on startup, we can specify
a dedicated region and output section for it. The following example is for GNU
`ld`-compatible linker scripts (applies to GNU `ld` and LLVM `lld`, and
toolchains based on those).

```c
MEMORY
{
  FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 1M
  RAM    (rwx) : ORIGIN = 0x20000000, LENGTH = 128K - 0x100
  /* Put a noinit region at the top 256 bytes of RAM  */
  NOINIT (rwx) : ORIGIN = 0x20000000 + 128K - 0x100, LENGTH = 0x100
}

SECTIONS
{
  ...
  .noinit (NOLOAD):
  {
    /* place all symbols in input sections that start with .noinit */
    KEEP(*(*.noinit*))
  } > NOINIT
  ...
}
```

> Note that we're using the arithmetic support in ld to compute the origin of
> the `NOINIT` region; purely optional, the start positions could easily be
> hard-coded too (and might be preferable, since it would be more explicit! see
> <https://www.python.org/dev/peps/pep-0020/>)

Now any symbol that's placed into a `.noinit*` section will be located in the
specified region, and _will not_ be initialized by our program's startup code!

To place a symbol into a `.noinit` region, see the following C code examples:

```c
// For GCC or Clang or derived toolchains, use the "section" __attribute__ .
__attribute__((section(".noinit")) volatile int my_non_initialized_integer;

// for IAR EWARM, it varies, but typically:
__no_init volatile int my_non_initialized_integer @ ".noinit";
```

> Note that it may be necessary to mark the variables as "volatile" (as in the
> example above) to ensure the compiler doesn't optimize away the "store"
> instructions! when in doubt, check the generated assembly where the variable
> is accessed (for example, by running gdb with the .elf, and using the
> `disassemble /s <function>` command) If your system does require volatile, it
> might be worth adding a comment explaining why, since it's usually quite rare
> that volatile is needed.

We can verify that our symbol ended up in the correct location by looking at the
`.map` file (add `-Wl,-Map=app.map` to the linker flags):

```plaintext
.noinit         0x000000002001ff00       0x04
 *(*.noinit*)
 .noinit        0x000000002001ff00       0x04 build/src/main.o
                0x000000002001ff00                my_non_initialized_integer
```

We can also look at our binary with the `size` binutil and see the new section:

```bash
❯ arm-none-eabi-size -Ax build/main.elf
build/bootloader.elf  :
section                size         addr
.text                0x1314    0x8000000
.data                  0x78   0x20000000
.bss                   0xdc   0x20000078
.noinit                0x10   0x2001ff00
```

## What about bootloaders

See some background information on bootloader operation here:

- [How to write a bootloader from scratch (The Interrupt)]({% post_url
  2019-08-13-how-to-write-a-bootloader-from-scratch %})

Since a bootloader usually will use the same RAM regions as the application, we
need to make sure that the `NOINIT` region in the bootloader is similarly
reserved from its normal `RAM` region. This can be done by simply matching the
same `REGIONS` as in the application, ensuring nothing is placed into the
`NOINIT` regions:

```c
MEMORY
{
  FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 1M
  RAM    (rwx) : ORIGIN = 0x20000000, LENGTH = 128K - 0x100
  /* the application relies on this noinit region being left alone! */
  NOINIT (rwx) : ORIGIN = 0x20000000 + 128K - 0x100, LENGTH = 0x100
}
```

## Some practical examples

Let's take a look at some simple examples showcasing how a `.noinit` section can
be implemented and used

### No bootloader, just application

This system just has a single application that immediately starts when the chip
is powered up. There's two variables located in the `.noinit` section:

```c
// Two non-initialized variables, used to demonstrate keeping information
// through chip reset. Marked 'volatile' to ensure the compiler doesn't optimize
// away the STR's to these addresses
#define RESET_MAGIC 0xDEADBEEF
// magic value used to check if the variables are initialized
__attribute__((section(".noinit"))) volatile uint32_t reset_count_magic;
// reset counter, incremented on every warm reset
__attribute__((section(".noinit"))) volatile uint32_t reset_count;
```

When the chip is initially powered on, the contents of SRAM is unknown. To
handle this, the `reset_count_magic` variable contains a special value when
initialized. To use it, we might do something like this:

```c
if (reset_count_magic != RESET_MAGIC) {
  reset_count_magic = RESET_MAGIC;
  reset_count = 0;

  printf("First reset!\n");
}

printf("Reset count: %lu\n", ++reset_count);
```

After a cold power on, the `reset_count_magic` should persist through warm
resets (eg if a Hard Fault happens, or the system intentionally reboots), and
the `reset_count` should increment.

You can see more details in the implementation here:

<https://github.com/noahp/cortex-m-bootloader-sample/tree/app-only>

### Bootloader and application, separate linker scripts

This system has a bootloader and application, placed into separate pages of
flash memory:

```c
MEMORY
{
  BOOTLOADER_FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 16K
  APP_FLASH (rx) : ORIGIN = 0x08004000, LENGTH = 1M - 16K
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K - 0x100
  NOINIT (rwx) : ORIGIN = 0x20000000 + 128K - 0x100, LENGTH = 0x100
}
```

Each linker script specifies the correct region to place read-only code and
data, for example, for the bootloader:

```c
SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)
        KEEP(*(.init))
        KEEP(*(.fini))
        *(.rodata*)
    } > BOOTLOADER_FLASH
}
```

On chip power up, the bootloader runs first (since it's placed at the lowest
flash address). In this example, the bootloader and app share a source file
containing the non-initialized variable:

```c
// noinit.c

#include <stdint.h>

// marked 'volatile' to ensure the compiler doesn't optimize away the STR's to
// these addresses
__attribute__((section(".noinit"))) volatile uint32_t mailbox[4];
```

The bootloader can set values into that variable, and the application can read
them:

```c
// bootloader main.c:
  extern volatile uint32_t mailbox[4];
  mailbox[0] = get_random_number();
  printf("Set random value to mailbox: 0x%08" PRIx32 "\n", mailbox[0]);

// app main.c:
  extern volatile uint32_t mailbox[4];
  printf("mailbox was: 0x%08" PRIx32 "\n", mailbox[0]);
```

The application could also set values into the mailbox, then jump to the
bootloader (eg via reset). This might be used to command the bootloader to
reflash the application, for example in a "dual-bank" (aka A/B) partition
scheme.

You can see more details in the implementation here:

<https://github.com/noahp/cortex-m-bootloader-sample/tree/two-linker-scripts>

### Bootloader and application, shared linker script

This system is very similar to the one above, except the bootloader and
application use the same linker script:

```c
MEMORY
{
  BOOTLOADER_FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 16K
  APP_FLASH (rx) : ORIGIN = 0x08004000, LENGTH = 1M - 16K
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K - 0x100
  NOINIT (rwx) : ORIGIN = 0x20000000 + 128K - 0x100, LENGTH = 0x100
}

SECTIONS
{
  .text :
  {
        KEEP(*(.isr_vector))
        *(.text*)
        KEEP(*(.init))
        KEEP(*(.fini))
        *(.rodata*)
  } > FLASH__  /* placeholder region identifier */
}
```

To link the bootloader or application, the shared linker script is run through
the C preprocessor to generate the linker script used for linking. The `FLASH__`
placeholder is replaced with the appropriate `BOOTLOADER_FLASH`/`APP_FLASH`
region for the application being linked:

```bash
❯ gcc -DFLASH__=APP_FLASH -E -P -C -x c-header src/common/stm32f407.ld > build/app.ld
```

(A similar technique is used by the Zephyr RTOS build system to derive the
correct linker script for different targets/memory configurations).

You can see more details in the implementation here:

<https://github.com/noahp/cortex-m-bootloader-sample/tree/shared-linker-script>

## Other considerations

### Backup RAM

Some chips will have a "backup RAM" or dedicated USB or Ethernet memory banks.
These memories can be a convenient spot to place noinit data (if the memory is
not being used for another purpose).

For example, on the STM32F407VG, there's a 4kB "Backup RAM" intended for very
low power sleep data retention; however, the memory can be used for any purpose.

```c
MEMORY
{
  /* on this chip, there's 4kB of "Backup RAM" mapped to this address */
  NOINIT (rwx) : ORIGIN = 0x40024000, LENGTH = 4k
}
```

On this chip, the backup RAM does need to be powered up to be used:

```c
//! Using the stm32f407xx.h CMSIS register structures to enable the backup SRAM
static void enable_backup_sram(void) {
  // enable power interface clock
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;

  // enable backup SRAM clock
  RCC->AHB1ENR |= RCC_AHB1ENR_BKPSRAMEN;

  // enable backup SRAM
  PWR->CR |= PWR_CR_DBP;
}
```

Then we can read/write variables that are placed in that region! Since this RAM
may not be typically used for the main application, this could be considered
"free" memory for various purposes (such as non-initialized data!)!

### Retrofitting a noinit region when you can’t update the bootloader

This scenario could happen if the application needs a `.noinit` region, but the
bootloader is not updateable (either due to external requirements, permanently
enabling write protect on the bootloader flash pages, or avoiding the risk of
potentially bricking a device if the bootloader update fails).

If the bootloader is set up to use all of RAM, you'll need to examine which
locations are actually used. For example, stack may start at the top of RAM and
grow down; if there's a known limit to the stack, a `.noinit` region could be
added below the stack region, and the bootloader won't overrwrite it.

To illustrate this, here's a typical RAM layout for an embedded system,
including stack and heap, from lowest to highest memory address:

```plaintext
.data
.bss
.heap (end of .bss, grows up)
<possible spot for retrofitted .noinit>
.stack (end of RAM, grows down)
```

Of course, if you're lucky, there may be a small reserve of RAM ([backup
RAM](#backup-ram)) that can be used instead!

### Gotchas: Watch out for ROM bootloaders (they use RAM too)

Some chips have ROM bootloaders (also referred to "ISP", In-System Programming),
that can:

- run before the user application is started
- run on-demand, depending on boot pins, or programmatically launched from
  software

These ROM programs will often use RAM for their own purposes! which can impact
`.noinit` regions. Consult the chip documentation, but be wary. Some chips don't
have good documentation on the ROM bootloader.

Some chips use routines stored in ROM for other purposes (flash programming,
built-in USB operations, etc); for example, the LPC15xx series documents this:

[![lpc iap example](/img/noinit/lpc15xx-isp-iap-ram-usage.png)](/img/noinit/lpc15xx-isp-iap-ram-usage.png)

### Test implementation

Some chips can lose SRAM contents when resetting (some chips wire up `NVIC`
reset to pulse the physical reset line!), which can make these schemes unstable
and a pain to debug.

When bringing up a new chip, it's worth consulting the docs on this topic, as
well as doing a quick check that SRAM contents persist through chip reset. This
can be done by:

1. writing a pattern to entire reserved area (eg `0x01234567`)
2. reset the chip and confirm the pattern is still valid across the region

{:.no_toc}

## Further Reading

- <https://mcuoneclipse.com/2014/04/19/gnu-linker-can-you-not-initialize-my-variable/>
- <https://mcuoneclipse.com/2013/04/14/text-data-and-bss-code-and-data-size-explained/>
- <https://atadiat.com/en/e-how-to-preserve-a-variable-in-ram-between-software-resets/>

<!-- Interrupt Keep START -->

{% include newsletter.html %}

{% include submit-pr.html %}

<!-- Interrupt Keep END -->
