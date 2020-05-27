/*****************************************************
 *
 * TASKING_GenericSTMF030R8_RTTD.c
 *
 * Description : Hello World in C, ANSI-style
 *
 */
#include "core_m0.h"
#include "SEGGER_RTT.h"
#include "trice.h"

int main(void)
{
	SEGGER_RTT_printf( 0, "Hello world\n" );

    TRICE_RTTD_SYNC;
    TRICE16_1( Id(45373), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
    TRICE0( Id(40356), "att:TASKING_GenericSTMF030R8_RTTD\n" );
    TRICE16_1( Id(45373), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
    TRICE16_1( Id(45373), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
    TRICE16_1( Id(45373), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
    TRICE16_1( Id(45373), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );

}
