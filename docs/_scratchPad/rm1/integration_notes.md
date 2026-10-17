# Trice Integration Notes

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

- [1. Trice on LibOpenCM3](#trice-on-libopencm3)
  - [1.1. Prerequisites](#prerequisites)
  - [1.2. triceConfig.h](#triceconfigh)
  - [1.3. main.c](#mainc)
  - [1.4. nucleo-f411re.ld](#nucleo-f411reld)
  - [1.5. Makefile](#makefile)
  - [1.6. Usage](#usage)
- [2. Get all project files containing Trice messages](#get-all-project-files-containing-trice-messages)
- [3. Building a trice library?](#building-a-trice-library)
- [4. Possible Compiler Issue when using Trice macros without parameters on old compiler or with strict-C settings](#possible-compiler-issue-when-using-trice-macros-without-parameters-on-old-compiler-or-with-strict-c-settings)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="trice-on-libopencm3"></a>Trice on LibOpenCM3

* This is a OpenCM3_STM32F411_Nucleo Contribution from [kraiskil](https://github.com/kraiskil).
* See also pull request [\#269](https://github.com/rokath/trice/pull/269).
* It is here because the code need some re-work to be compatible with Trice version 1.0.

[LibOpenCM3](https://libopencm3.org/) is a hardware abstraction library for many microcontrollers.

This is an exampe using STM's [STM32F411 Nucleo](https://www.st.com/en/evaluation-tools/nucleo-f411re.html) board.

```diff
--> This code uses a legacy Trice version and needs adaptation!
```

### 1.1. <a id="prerequisites"></a>Prerequisites

- Suitable ARM GCC cross compiler (`arm-none-eabi-gcc`) found in your system's PATH
- GNU Make, or compatible
- Environment variable `OPENCM3_DIR` points to the base install of libopencm3.
  This is e.g. the libopencm3 source directory, if you also built it in the source directory.
- OpenOCD

### 1.2. <a id="triceconfigh"></a>triceConfig.h

```C
/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
LibOpenCM3 adapatation by Kalle Raiskila.
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

// Local (to this demo) time keeping functions
#include "time.h"

#define TRICE_UART USART2 //!< Enable and set UART for serial output.
// The alternative, TRICE_RTT_CHANNEL is not available with OpenCM3
// #define TRICE_RTT_CHANNEL 0

// Timestamping function to be provided by user. In this demo from time.h
#define TRICE_TIMESTAMP wallclock_ms() // todo: replace with TRICE_TREX_ENCODING stuff

// Enabling next 2 lines results in XTEA TriceEncryption  with the key.
// #define TRICE_ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret
// #define TRICE_DECRYPT //!< TRICE_DECRYPT is usually not needed. Enable for checks.

// #define TRICE_BIG_ENDIANNESS //!< TRICE_BIG_ENDIANNESS needs to be defined for TRICE64 macros on big endian devices. (Untested!)

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Predefined trice modes: Adapt or creeate your own trice mode.
//
#ifndef TRICE_MODE
#error Define TRICE_MODE to 0, 200 or 201
#endif

//! Direct output to UART or RTT with cycle counter. Trices inside interrupts forbidden. Direct TRICE macro execution.
//! This mode is mainly for a quick tryout start or if no timing constrains for the TRICE macros exist.
//! Only a putchar() function is required - look for triceBlockingPutChar().
//! UART Command line similar to: `trice log -p COM1 -baud 115200`
//! RTT needs additional tools installed - see RTT documentation.
//! J-LINK Command line similar to: `trice log -args="-Device STM32G071RB -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"`
//! ST-LINK Command line similar to: `trice log -p ST-LINK -args="-Device STM32G071RB -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"`
#if TRICE_MODE == 0                     // must not use TRICE_ENCRYPT!
#define TRICE_STACK_BUFFER_MAX_SIZE 128 //!< This  minus TRICE_DATA_OFFSET the max allowed single trice size. Usually ~40 is enough.
#ifndef TRICE_ENTER
#define TRICE_ENTER                                                                          \
	{                                                  /*! Start of TRICE macro */           \
		uint32_t co[TRICE_STACK_BUFFER_MAX_SIZE >> 2]; /* Check TriceDepthMax at runtime. */ \
		uint32_t* TriceBufferWritePosition = co + (TRICE_DATA_OFFSET >> 2);
#endif
#ifndef TRICE_LEAVE
#define TRICE_LEAVE                                                                 \
	{ /*! End of TRICE macro */                                                     \
		unsigned tLen = ((TriceBufferWritePosition - co) << 2) - TRICE_DATA_OFFSET; \
		TriceOut(co, tLen);                                                         \
	}                                                                               \
	}
#endif
#endif // #if TRICE_MODE == 0

//! Double Buffering output to RTT or UART with cycle counter. Trices inside interrupts allowed. Fast TRICE macro execution.
//! UART Command line similar to: `trice log -p COM1 -baud 115200`
//! RTT Command line similar to: `trice l -args="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"`
#if TRICE_MODE == 200
#ifndef TRICE_ENTER
#define TRICE_ENTER TRICE_ENTER_CRITICAL_SECTION //! TRICE_ENTER is the start of TRICE macro. The TRICE macros are a bit slower. Inside interrupts TRICE macros allowed.
#endif
#ifndef TRICE_LEAVE
#define TRICE_LEAVE TRICE_LEAVE_CRITICAL_SECTION //! TRICE_LEAVE is the end of TRICE macro.
#endif
#define TRICE_HALF_BUFFER_SIZE 1000 //!< This is the size of each of both buffers. Must be able to hold the max TRICE burst count within TRICE_TRANSFER_INTERVAL_MS or even more, if the write out speed is small. Must not exceed SEGGER BUFFER_SIZE_UP
#define TRICE_SINGLE_MAX_SIZE 100   //!< must not exeed TRICE_HALF_BUFFER_SIZE!
#endif                              // #if TRICE_MODE == 200

//! Double Buffering output to UART without cycle counter. No trices inside interrupts allowed. Fastest TRICE macro execution.
//! Command line similar to: `trice log -p COM1 -baud 115200`
#if TRICE_MODE == 201
#define TRICE_CYCLE_COUNTER 0       //! Do not add cycle counter, The TRICE macros are a bit faster. Lost TRICEs are not detectable by the trice tool.
#define TRICE_ENTER                 //! TRICE_ENTER is the start of TRICE macro. The TRICE macros are a bit faster. Inside interrupts TRICE macros forbidden.
#define TRICE_LEAVE                 //! TRICE_LEAVE is the end of TRICE macro.
#define TRICE_HALF_BUFFER_SIZE 2000 //!< This is the size of each of both buffers. Must be able to hold the max TRICE burst count within TRICE_TRANSFER_INTERVAL_MS or even more, if the write out speed is small. Must not exceed SEGGER BUFFER_SIZE_UP
#define TRICE_SINGLE_MAX_SIZE 800   //!< must not exeed TRICE_HALF_BUFFER_SIZE!
#endif                              // #if TRICE_MODE == 201

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Headline info
//

#ifdef TRICE_HALF_BUFFER_SIZE
#define TRICE_BUFFER_INFO                                                              \
	do {                                                                               \
		TRICE32(Id(0), "att: Trice 2x half buffer size:%4u ", TRICE_HALF_BUFFER_SIZE); \
	} while (0)
#else
#define TRICE_BUFFER_INFO                                                                                 \
	do {                                                                                                  \
		TRICE32(Id(0), "att:Single Trice Stack buf size:%4u", TRICE_SINGLE_MAX_SIZE + TRICE_DATA_OFFSET); \
	} while (0)
#endif

//! This is usable as the very first trice sequence after restart. Adapt and use it or ignore it.
#define TRICE_HEADLINE                                                           \
	TRICE0(Id(0), "s:                                          \n");             \
	TRICE8(Id(0), "s:     NUCLEO-F411RE     TRICE_MODE %3u     \n", TRICE_MODE); \
	TRICE0(Id(0), "s:                                          \n");             \
	TRICE0(Id(0), "s:     ");                                                    \
	TRICE_BUFFER_INFO;                                                           \
	TRICE0(Id(0), "s:     \n");                                                  \
	TRICE0(Id(0), "s:                                          \n");

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Compiler Adaptation
//

#if defined(__GNUC__) /* gnu compiler ###################################### */

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4                                 //!< align to 4 byte boundary preamble
#define ALIGN4_END __attribute__((aligned(4))) //!< align to 4 byte boundary post declaration

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
#define TRICE_ENTER_CRITICAL_SECTION               \
	{                                              \
		uint32_t old_mask = cm_mask_interrupts(1); \
		{

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
#define TRICE_LEAVE_CRITICAL_SECTION \
	}                                \
	cm_mask_interrupts(old_mask);    \
	}

#else
#error unknown compliler
#endif // compiler adaptations ##################################################

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Optical feedback: Adapt to your device.
//

TRICE_INLINE void ToggleOpticalFeedbackLED(void) {
	// The only user controllable LED available on the
	// Nucleo is LD2, on port A5. This is set up in main.c
	gpio_toggle(GPIOA, GPIO5);
}

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// UART interface: Adapt to your device.
//

#ifdef TRICE_UART

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmpty(void) {
	uint32_t reg = USART_SR(TRICE_UART);
	return (reg & USART_SR_TXE);
}

//! Write value v into trice transmit register.
//! \param v byte to transmit
//! User must provide this function.
TRICE_INLINE void triceTransmitData8(uint8_t v) {
	usart_send_blocking(TRICE_UART, v);
	ToggleOpticalFeedbackLED();
}

//! Allow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceEnableTxEmptyInterrupt(void) {
	usart_enable_tx_interrupt(TRICE_UART);
}

//! Disallow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceDisableTxEmptyInterrupt(void) {
	usart_disable_tx_interrupt(TRICE_UART);
}

#endif // #ifdef TRICE_UART

///////////////////////////////////////////////////////////////////////////////
// Default TRICE macro bitwidth: 32 (optionally adapt to MCU bit width)
//

#define TRICE_1 TRICE32_1   //!< Default parameter bit width for 1  parameter count TRICE is 32, change for a different value.
#define TRICE_2 TRICE32_2   //!< Default parameter bit width for 2  parameter count TRICE is 32, change for a different value.
#define TRICE_3 TRICE32_3   //!< Default parameter bit width for 3  parameter count TRICE is 32, change for a different value.
#define TRICE_4 TRICE32_4   //!< Default parameter bit width for 4  parameter count TRICE is 32, change for a different value.
#define TRICE_5 TRICE32_5   //!< Default parameter bit width for 5  parameter count TRICE is 32, change for a different value.
#define TRICE_6 TRICE32_6   //!< Default parameter bit width for 6  parameter count TRICE is 32, change for a different value.
#define TRICE_7 TRICE32_7   //!< Default parameter bit width for 7  parameter count TRICE is 32, change for a different value.
#define TRICE_8 TRICE32_8   //!< Default parameter bit width for 8  parameter count TRICE is 32, change for a different value.
#define TRICE_9 TRICE32_9   //!< Default parameter bit width for 9  parameter count TRICE is 32, change for a different value.
#define TRICE_10 TRICE32_10 //!< Default parameter bit width for 10 parameter count TRICE is 32, change for a different value.
#define TRICE_11 TRICE32_11 //!< Default parameter bit width for 11 parameter count TRICE is 32, change for a different value.
#define TRICE_12 TRICE32_12 //!< Default parameter bit width for 12 parameter count TRICE is 32, change for a different value.

//
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */

```

### 1.3. <a id="mainc"></a>main.c

```C
/*
 * Demo to test/show TRICE usage in a libopencm3
 * environment.
 */

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>

#include <stdint.h>
void msleep(uint32_t delay);
uint32_t wallclock_ms(void);

#include "trice.h"

static void hardware_setup(void)
{
	/* Set device clocks from opencm3 provided preset.*/
	const struct rcc_clock_scale *clocks = &rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ];
	rcc_clock_setup_pll( clocks );

	/* Set up driving the LED connected to port A, pin 5. */
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);

	/* User-button is connected to port C, pin 13. Set up button push
	 * to cause an interrupt. */
	gpio_mode_setup(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO13);
	rcc_periph_clock_enable(RCC_SYSCFG);  // clock for the EXTI handler
	nvic_enable_irq(NVIC_EXTI15_10_IRQ);
	exti_select_source(EXTI13, GPIOC);
	exti_set_trigger(EXTI13, EXTI_TRIGGER_FALLING);
	exti_enable_request(EXTI13);

	/* USART2 is connected to the nucleo's onboard ST-Link, which forwards
	 * it as a serial terminal over the ST-Link USB connection.
	 * This UART is given to the Trice data. */
	rcc_periph_clock_enable(RCC_USART2);
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	// Enable UART2 interrupts in the system's interrupt controller
	// but do NOT enable generating interrupts in the UART at this
	// time. Let Trice enable them with triceEnableTxEmptyInterrupt()
	nvic_enable_irq(NVIC_USART2_IRQ);
	//usart_enable_tx_interrupt(USART2);
	usart_enable(USART2);

	/* Configure USART TX pin only. We don't get any input via the TRICE
	 * channel, so the RX pin can be left unconnected to the USART2 */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);

	/* Enable systick at a 1mS interrupt rate */
	systick_set_reload(84000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();
	systick_interrupt_enable();
}

//////////////////////////
// Time handling utilities
static volatile uint32_t system_millis;

/* "sleep" for delay milliseconds */
void msleep(uint32_t delay)
{
	uint32_t wake = system_millis + delay;
	while (wake > system_millis);
}

uint32_t wallclock_ms(void)
{
	return system_millis;
}

//////////////////////////
// Interupt handlers
// These are weak symbols in libopencm3
// that get overridden here.

// Trice USART
void usart2_isr(void)
{
	#if TRICE_MODE == 200
	triceServeTransmit();
	#endif
}

// External interrupts on pins 10-15, all ports.
// Only PC13 (User button on Nucleo) is enabled in this program.
void exti15_10_isr(void)
{
	exti_reset_request(EXTI13);
	#if TRICE_MODE == 200
	TRICE(Id(0), "Button press at, %d\n", system_millis);
	#endif
}

// Systick timer set to 1ms
void sys_tick_handler(void)
{
	system_millis++;
	#if TRICE_MODE == 200
	// Start sending what is currently in the Trice transmit buffer
	triceTriggerTransmit();
	#endif
}

int main(void)
{
	hardware_setup();
	TRICE_HEADLINE;
	while (1) {
		msleep(1000);

		// Depending on mode, either print this string to
		// UART (mode 0), or the Trice write buffer (mode 200).
		TRICE(Id(0), "Hello, TRICE, %d\n", 42);

		// TRICE("") with a string parameter only is problematic.
		// See discussion on https://github.com/rokath/trice/issues/279
		// TRICE0("") works in either case
		#ifdef __STRICT_ANSI__
		// if compiled with e.g. --std=c99
		TRICE0(Id(0), "Hello, TRICE\n");
		#else
		TRICE(Id(0), "Hello, TRICE\n");
		TRICE0(Id(0), "Hello, TRICE0()\n");
		#endif

		#if TRICE_MODE == 200
		// Swap Trice transmit/write ping-pong buffers.
		// Stuff printed with TRICE() since the last
		// call to TriceTransfer() will be sent once
		// triceTriggerTransmit() is called.
		TriceTransfer();
		#endif
	}

	return 0;
}

```

### 1.4. <a id="nucleo-f411reld"></a>nucleo-f411re.ld

```ld
/* Use the LibOpenCM3-provided defaults for the linker details.
 */
MEMORY
{
	rom (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

INCLUDE cortex-m-generic.ld
```

### 1.5. <a id="makefile"></a>Makefile

```mak
# Makefile for compiling the Trice demo on LibOpenCM3
# for STM32F411-Nucleo boards
CC=arm-none-eabi-gcc
C_FLAGS=-O0 -std=c99 -ggdb3
C_FLAGS+=-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
C_FLAGS+=-Wextra -Wshadow -Wimplicit-function-declaration -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
C_FLAGS+=-fno-common -ffunction-sections -fdata-sections  -MD -Wall -Wundef
C_FLAGS+=-DSTM32F4 -I/home/kraiskil/stuff/libopencm3/include
# These two are for trice.h and triceConfig.h
C_FLAGS+=-I. -I../../src -I../../src/default_conf

LFLAGS=-L${OPENCM3_DIR}/lib -lopencm3_stm32f4 -lm -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
LFLAGS+=-T nucleo-f411re.ld
LFLAGS+=--static -nostartfiles
LFLAGS+=-Wl,-Map=memorymap.txt

all: direct_mode.elf irq_mode.elf
.PHONY: flash clean

# Annotate Trice-enabled code.
# trice does this annotation in-place, so here we take
# a copy before running trice.
# I.e. write TRICE macros in foo.c, and this will generate
# the TRICE( Id(1234) .. ) macros into foo.trice.c
%.trice.c: %.c til.json
	cp -f $< $<.bak
	trice update
	cp -f $< $@
	cp -f $<.bak $<

# trice expects this file to exist, can be empty.
til.json:
	touch til.json

direct_mode.elf: main.trice.c ../../pkg/src/trice.c
	${CC} ${C_FLAGS} $^ -o $@ ${LFLAGS} -DTRICE_MODE=0

flash_direct_mode: direct_mode.elf
	openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -c "program direct_mode.elf verify reset exit"

irq_mode.elf: main.trice.c ../../pkg/src/trice.c
	${CC} ${C_FLAGS} $^ -o $@ ${LFLAGS} -DTRICE_MODE=200

flash_irq_mode: irq_mode.elf
	openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -c "program irq_mode.elf verify reset exit"


clean:
	@rm -f *.elf til.json main.trice.c
```

### 1.6. <a id="usage"></a>Usage

- Run `make direct_mode.elf` to compile with Trice mode 0.
- Run `make flash_direct_mode` to program the board.
- Run trice: `trice l -p /dev/ttyACM0`.

## 2. <a id="get-all-project-files-containing-trice-messages"></a>Get all project files containing Trice messages

We check the location information file. Every Trice is registered here.

```bash
cat demoLI.json | grep '"File":' | sort | uniq
		"File": "_test/_ringB_protect_de_tcobs_ua/TargetActivity.c",
		"File": "_test/special_dblB_de_tcobs_ua/TargetActivity.c",
		"File": "_test/special_for_debug/TargetActivity.c",
		"File": "_test/special_protect_dblB_de_tcobs_ua/TargetActivity.c",
		"File": "_test/testdata/triceCheck.c",
		"File": "examples/F030_inst/Core/Src/stm32f0xx_it.c",
		"File": "examples/G0B1_inst/Core/Src/main.c",
		"File": "examples/G0B1_inst/Core/Src/stm32g0xx_it.c",
		"File": "examples/L432_inst/Core/Inc/triceConfig.h",
		"File": "examples/L432_inst/Core/Src/main.c",
		"File": "examples/L432_inst/Core/Src/stm32l4xx_it.c",
		"File": "examples/exampleData/triceExamples.c",
		"File": "examples/exampleData/triceLogDiagData.c",
```

## 3. <a id="building-a-trice-library"></a>Building a trice library?

The triceConfig.h is mandatory for the trice code. It controls which parts of the trice code are included. There is no big advantage having a trice library, because it would work only with unchanged settings in the project specific triceConfig.h. Once the trice source files are translated, their objects are rebuilt automatically and only when the triceConfig.h is changed. So only the linker has a bit less to do when it finds a trice library compared to a bunch of trice objects. But does that influence the build time heavily?

The triceConfig.h is the only part of the trice sources which should be modified by the users. It is ment to be a individual part of the user projects. The examples folder shows the usage.

## 4. <a id="possible-compiler-issue-when-using-trice-macros-without-parameters-on-old-compiler-or-with-strict-c-settings"></a>Possible Compiler Issue when using Trice macros without parameters on old compiler or with strict-C settings

If you encounter a compilation error on `trice( "hi");` for example, but not on `trice( "%u stars", 5 );`, this is probably caused by the way your compiler interprets variadic macros. Simply change to `trice0( "hi");` or change your compiler settings. See issue [\#279](https://github.com/rokath/trice/issues/279) for more details. If your project needs to be translated with strict-C settings for some reason, you have to use the `trice0` macros when no values exist for the Trice macros.

<p align="right">(<a href="#trice-integration-notes">back to top</a>)</p>

