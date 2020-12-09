#line 1 "src/main.c"
#line 1 "src/main.c"



#line 5 "src/main.c"

#line 8 "src/main.c"
#line 51 "src/main.c"
#line 52 "src/main.c"

#line 55 "src/main.c"
#line 107 "src/main.c"
#line 108 "src/main.c"

#line 115 "src/main.c"
 
#pragma config FOSC = INTIO7, FCMEN = ON, IESO = ON
#pragma config PWRT = OFF, BOREN = OFF, BORV = 30
#pragma config WDTEN = OFF, WDTPS =	32768
#pragma config CCP2MX = PORTC, PBADEN =	OFF, LPT1OSC = OFF, HFOFST = OFF, MCLRE = ON
#pragma config STVREN = ON, LVP = OFF, XINST = OFF

#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF, CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF
#pragma config WRTC = OFF, WRTB = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF
#pragma config EBTRB=OFF



#line 1 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdio.h"

#line 3 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdio.h"

#line 1 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdarg.h"
 


#line 5 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdarg.h"

typedef void* va_list;
#line 8 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdarg.h"
#line 9 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdarg.h"
#line 10 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdarg.h"
#line 11 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdarg.h"
#line 12 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdarg.h"
#line 4 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdio.h"

#line 1 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stddef.h"
 

#line 4 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stddef.h"

typedef unsigned char wchar_t;


#line 10 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stddef.h"
 
typedef signed short int ptrdiff_t;
typedef signed short int ptrdiffram_t;
typedef signed short long int ptrdiffrom_t;


#line 20 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stddef.h"
 
typedef unsigned short int size_t;
typedef unsigned short int sizeram_t;
typedef unsigned short long int sizerom_t;


#line 34 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stddef.h"
 
#line 36 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stddef.h"


#line 41 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stddef.h"
 
#line 43 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stddef.h"

#line 45 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stddef.h"
#line 5 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdio.h"



#line 9 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdio.h"
 
#line 11 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdio.h"

#line 13 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdio.h"


typedef unsigned char FILE;

 
#line 19 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdio.h"
#line 20 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdio.h"

extern FILE *stderr;
extern FILE *stdout;


int putc (auto char c, auto FILE *f);
int vsprintf (auto char *buf, auto const far  rom char *fmt, auto va_list ap);
int vprintf (auto const far  rom char *fmt, auto va_list ap);
int sprintf (auto char *buf, auto const far  rom char *fmt, ...);
int printf (auto const far  rom char *fmt, ...);
int fprintf (auto FILE *f, auto const far  rom char *fmt, ...);
int vfprintf (auto FILE *f, auto const far  rom char *fmt, auto va_list ap);
int puts (auto const far  rom char *s);
int fputs (auto const far  rom char *s, auto FILE *f);

#line 36 "C:/Program Files (x86)/Microchip/mplabc18/v3.47/bin/../h/stdio.h"
#line 131 "src/main.c"


void interrupt high_priority isrHigh() {
    
}
void interrupt low_priority isrLow() {
    
}
void setupUART() {
    
    TRISC = 0b10111111;
    
    TXEN = 1;
    SYNC = 0;
    TX9 = 0; 
    
    RX9 = 0;
    SPEN = 1;
    CREN = 1;
    
    BRGH = 1;
    BRG16 = 1;
    SPBRGH = 0;
    SPBRG = 25;
}
void setupADC() {
    TRISA = TRISA | 0b00000001;
    ANS0 = 1; 
    ADCON0 = 1;
    ADCON1 = 0;
    ADCON2 = 0b10111110;
}
uint8_t readADC() {
    ADCON0bits.GO = 1;
    while (ADCON0bits.NOT_DONE);
    return ADRESL;
}
void putch(char data)
{
    while( ! TXIF);
    TXREG = data;
}
void main(void) {
    setupUART();
    setupADC();
    printf("The quick brown fox jumps over the lazy dog\n\r");
    uint8_t adLow;
    float voltage;
    while(1) {
        adLow = readADC();
        voltage = 5 * (ADRESH * 256 + adLow) / 1023.0;
        printf("[%.2f], ", voltage);
        _delay(125000);
        
    }
    return;
}
#line 189 "src/main.c"
 