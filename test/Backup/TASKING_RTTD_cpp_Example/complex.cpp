/*************************************************************************
**
**  VERSION:            @(#)complex.cpp	1.4
**
**  IN PACKAGE:         ARM
**
**  COPYRIGHT:          Copyright 2005-2015 Altium BV
**
**  DESCRIPTION:        C++ example program with operator overloading
**
**  NOTE:               This program uses FSS (File System Simulation) to
**                      send output to the debugger.
**
**************************************************************************/

#include <stdio.h>

#include "SEGGER_RTT.h"
#include "trice.h"

class   Complex
{
        int     re_part;
        int     im_part;

public:
        Complex(int re, int im) { re_part=re; im_part = im; }
        Complex(int re) { re_part = re; im_part = 0; }
        Complex() { re_part = 0; im_part = 0; }

        void    Print();

        friend  Complex operator+(Complex, Complex);
        friend  Complex operator-(Complex, Complex);
        friend  Complex operator-(Complex);
};

void    Complex::Print(void)
{
	TRICE_P("re = %2d, im = %2d\n", re_part, im_part);
}

Complex operator+(Complex c1, Complex c2)
{
        return Complex(c1.re_part + c2.re_part, c1.im_part + c2.im_part);
}

Complex operator-(Complex c1, Complex c2)
{
        return Complex(c1.re_part - c2.re_part, c1.im_part - c2.im_part);
}

Complex operator-(Complex c1)
{
        return Complex(-c1.re_part, -c1.im_part);
}


//
// Use complex class
//

int     main(void)
{
        Complex c1(1,2);
        Complex c2(2);
        Complex c3;

        TRICE_RTT_SYNC;

        TRICE0( Id(18398), "Complex number c1: " );
        c1.Print();
        TRICE0( Id(43499), "Complex number c2: " );
        c2.Print();

        TRICE0( Id(32731), "Calculate c1 + c2: " );
        c3 = c1 + c2;
        c3.Print();

        TRICE0( Id(38802), "Calculate c2 - c1: " );
        c3 = c2 - c1;
        c3.Print();

        TRICE0( Id(40045), "Negate previous  : " );
        c3 = -c3;
        c3.Print();

    	// initialize systick
    	*(uint32_t*)0xE000E010 = 0;          // SysTick->CTRL = 0;   // Disable SysTick
    	*(uint32_t*)0xE000E014 = 0x00FFFFFF; // SysTick->LOAD = 0x00ffffff; // Count down from 999 to 0
    	*(uint32_t*)0xE000E018 = 0;          // SysTick->VAL = 0;    // Clear current value to 0
    	*(uint32_t*)0xE000E010 = 0x5;        // SysTick->CTRL = 0x7; // Enable SysTick, and use processor clock
    										 // no exception


        TRICE16_1( Id(14522), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
        TRICE0( Id(24326), "att:TASKING_RTTD_cpp_Example\n" );
        TRICE16_1( Id(14522), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
        TRICE16_1( Id(14522), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
        TRICE16_1( Id(14522), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
        TRICE16_1( Id(14522), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
        triceCheckSet(0);

        printf( "End of Program\n" );
        return 0;
}
