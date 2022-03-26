/*
 * Demo to test/show TRICE usage in a libopencm3
 * environment.
 */

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>

#include "trice.h"

#include "time.h"

static void hardware_setup(void)
{
	/* Set device clocks from opencm3 provided preset.*/
	const struct rcc_clock_scale *clocks = &rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ];
	rcc_clock_setup_pll( clocks );

	/* Set up driving the LED connected to port A, pin 5. */
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);

	/* USART2 is connected to the nucleo's onboard ST-Link, which forwards
	 * it as a serial terminal over the ST-Link USB connection. */
	rcc_periph_clock_enable(RCC_USART2);
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
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

int main(void)
{
	hardware_setup();

	while (1) {
		msleep(1000);
		// TODO: how to print a string only?
		// This causes .trice.c compilation to fail
		//TRICE( Hello, TRICE\n");
		TRICE("Hello, TRICE, %d\n", 42);
	}

	return 0;
}
