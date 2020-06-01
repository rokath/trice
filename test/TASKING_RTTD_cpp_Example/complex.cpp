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
        printf("re = %2d, im = %2d\n", re_part, im_part);
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

        printf( "Complex number c1: " );
        c1.Print();
        printf( "Complex number c2: " );
        c2.Print();

        printf( "Calculate c1 + c2: " );
        c3 = c1 + c2;
        c3.Print();

        printf( "Calculate c2 - c1: " );
        c3 = c2 - c1;
        c3.Print();

        printf( "Negate previous  : " );
        c3 = -c3;
        c3.Print();

        printf( "End of Program\n" );
        return 0;
}
