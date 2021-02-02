/*****************************************************
 *
 * TASKING_GenericSTMF030R8_RTTB.c
 *
 * Description : Hello World in C, ANSI-style
 *
 */

#include "SEGGER_RTT.h"
#include "trice.h"

int main(void)
{
	SEGGER_RTT_printf( 0, "Hello world\n" );
	TRICE_RTT_SYNC;

	// initialize systick
	*(uint32_t*)0xE000E010 = 0;          // SysTick->CTRL = 0;   // Disable SysTick
	*(uint32_t*)0xE000E014 = 0x00FFFFFF; // SysTick->LOAD = 0x00ffffff; // Count down from 999 to 0
	*(uint32_t*)0xE000E018 = 0;          // SysTick->VAL = 0;    // Clear current value to 0
	*(uint32_t*)0xE000E010 = 0x5;        // SysTick->CTRL = 0x7; // Enable SysTick, and use processor clock
										 // no exception

	TRICE16_1( Id(14522), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
    TRICE0( Id(44414), "att:TASKING_GenericSTMF030R8_RTTB\n" );
    TRICE16_1( Id(14522), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
    TRICE16_1( Id(14522), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
    TRICE16_1( Id(14522), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
    TRICE16_1( Id(14522), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
    for(;;){
    	triceServe();
    }
}
