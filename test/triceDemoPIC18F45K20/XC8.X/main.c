#if 1
/*
 * File:   main.c
 * Author: Kell
 *
 * Created on den 20 januari 2018, 11:21
 * Config information:
 * file:///C:/Program%20Files%20(x86)/Microchip/xc8/v1.42/docs/chips/18f45k20.html
 */
#pragma config FOSC = INTIO7, FCMEN = ON, IESO = ON//CONFIG1H @ 0x300001
#pragma config PWRT = OFF, BOREN = OFF, BORV = 30//CONFIG2L @ 0x300002
#pragma config WDTEN = OFF, WDTPS =	32768//CONFIG2H @ 0x300003
#pragma config CCP2MX = PORTC, PBADEN =	OFF, LPT1OSC = OFF, HFOFST = OFF, MCLRE = ON//CONFIG3H @ 0x300005
#pragma config STVREN = ON, LVP = OFF, XINST = OFF//CONFIG4L @ 0x300006
// DEBUG = OFF "should not be specified in source code configuration settings"
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF//CONFIG5L @ 0x300008
#pragma config CPB = OFF, CPD = OFF//CONFIG5H @ 0x300009
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF//CONFIG6L @ 0x30000A
#pragma config WRTC = OFF, WRTB = OFF, WRTD = OFF//CONFIG6H @ 0x30000B
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF//CONFIG7L @ 0x30000C
#pragma config EBTRB=OFF//Register: CONFIG7H @ 0x30000D


#include <xc.h>
#include <stdio.h>
#include <stdint.h>
 interrupt high_priority void isrHigh() {
    
}
void interrupt low_priority isrLow() {
    
}
void setupUART() {
    //Based on Fosc = 1MHz,default on Reset
    TRISC = 0b10111111;//RC6 is TX is output
    //TXSTA Register
    TXEN = 1;
    SYNC = 0;
    TX9 = 0; //8-bit mode
    //RCSTA Register
    RX9 = 0;//9-bit Receive Enable bit
    SPEN = 1;//Serial Port Enable bit
    CREN = 1;//Continuous Receive Enable bit
    //BAUDCON reg
    BRGH = 1;
    BRG16 = 1;//8-bit Baud Rate Generator is used (SPBRG)
    SPBRGH = 0;
    SPBRG = 25;//9600 bits/s
}
void setupADC() {
    TRISA = TRISA | 0b00000001;//RA0 is input
    ANS0 = 1; //RA0 is analog
    ADCON0 = 1;//Channel AN0, ADC enabled
    ADCON1 = 0;//Neg Ref = VSS, Pos ref = VDD
    ADCON2 = 0b10111110;//ADFM = Left, ACQT = 20TAD, ADCS = Fosc/64
}
uint8_t readADC() {
    ADCON0bits.GO = 1;
    while (ADCON0bits.NOT_DONE);
    return ADRESL;
}
void putch(char data)
{
    while( ! TXIF);//Wait for TXIF
    TXREG = data;
}
void main(void) {
    setupUART();
    setupADC();
    printf("The quick brown fox jumps over the lazy dog\n\r");
    uint8_t adLow;
    float voltage;
    while(1) {
        adLow = readADC();//returns with ADRESL when ADRESL & ADRESH is avail.
        voltage = 5 * (ADRESH * 256 + adLow) / 1023.0;
        printf("[%.2f], ", voltage);
        _delay(125000);
        //Sleep(500);//Power down
    }
    return;
}
#endif
 