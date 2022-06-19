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
#define TRICE_FILE Id(43036)


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
	TRICE( Id(38498),"Button press at, %d\n", system_millis);
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
		TRICE( Id(33008),"Hello, TRICE, %d\n", 42);

		// TRICE() with a string parameter only is problematic.
		// See discussion on https://github.com/rokath/trice/issues/279
		// TRICE0() works in either case
		#ifdef __STRICT_ANSI__
		// if compiled with e.g. --std=c99
		TRICE0( Id(60901),"Hello, TRICE\n");
		#else
		TRICE( Id(63042),"Hello, TRICE\n");
		TRICE0( Id(65406),"Hello, TRICE0()\n");
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

