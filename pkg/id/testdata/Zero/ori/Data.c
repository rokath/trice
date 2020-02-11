/*! \file SoftwareChecks.c
\copyright [BaumTec GmbH](http://www.baumtec.de/)
\brief 
\details 
\author Thomas Hoehenleitner
*******************************************************************************/


#include "config.h" // should be first include file, at least befor other project specific files

#ifdef TRACELOG
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <CriticalSection.h>
#include "Tr16.h"
#include "TraceUtilities.h"
#include "Fifo.h"
#include "LeftRightFifo.h"
#endif // #ifdef TRACELOG

#include "DefaultMacros.h"
#include "main.h"
#include "msTicks.h"
#include "MicroSeconds.h"
#include "Os.h"
#include "SoftwareChecks.h"
#include "SysTicks.h"
#include "usTicks.h"


/*! write out all types of logs
*/
void TraceLogChecks( void )
{
    #ifdef TRACELOG
    WaitForTraceLogTransmissionDone();
    /* test partial traces */
    TL0   (Id(46187), "\r\n\n\nwrn:a" );
    TL0   (Id(18678), "wrn:A" );
    TL0   (Id(20972), "rd_:b" );
    TL0   (Id(14372), "wr_:B" );
    TL0   (Id(14329), "msg:c" );
    TL0   (Id(14720), "tim:C\n" );
    WaitForTraceLogTransmissionDone();

    /* test normal traces */
    TL8_1( Id(19900), "WRN: TL8-1 %d\n", -1 );
    TL8_2( Id(58493), "ERR: TL8-2 %d +%d\n", -1, +2 );
    TL8_3( Id(59201), "TIM: TL8-3 +%d %d +%d\n", +1, -2, +3 );
    TL8_4( Id(30161), "RD_: TL8-4 %d +%d %d +%d\n", -1, +2, -3, +4 );
    TL8_5( Id(63974), "WR_: TL8-5 +%d %d +%d %d +%d\n", +1, -2, +3, -4, +5 );
    TL8_6( Id(52742), "ATT: TL8-6 %d %d +%d +%d %d %d \n", -1, -2, +3, +4, -5, -6 );
    TL8_7( Id(52464), "DBG: TL8-7 +%d +%d %d %d +%d +%d %d\n", +1, +2, -3, -4, +5, +6, -7 );
    TL8_8( Id(58219), "DIA: TL8-8 %d %d +%d +%d +%d +%d %d %d\n", -1, -2, +3, +4, +5, +6, -7, -8 );
    WaitForTraceLogTransmissionDone();

    TL16_1( Id(21988), "ISR: TL16-1 %x\n", 0x1311 );
    TL16_1( Id(34841), "SIG: TL16-1 %d\n", -1 );
    TL16_2( Id(11696), "TST: TL16-2 +%d %d\n", +1, -2 );
    TL16_3( Id(42785), "MSG: TL16-3 %d +%d %d\n", -1, +2, -3 );
    TL16_4( Id(49211), "MSG: TL16-4 +%d %d %d +%d\n", +1, -2, -3, +4 );
    TL32_1( Id(43555), "DBG: TL32-1 %x\r\n", 0x44332211 );
    TL32_2( Id(27431), "DBG: TL32-2 %d +0x%x\r\n", -1, +2 ); // ok
    TL32_1( Id(1079), "DBG: TL32-1 %x\r\n", 0x88332211 );
    TL32_2( Id(15943), "att:TL32-2 %08x%08x\r\n", 0xfedcba98, 0x76543210 ); // ok
    TL32_1( Id(6059), "DBG: TL32-1 %x\r\n", 0xaa332211 );
    TL8_1( Id(10), "SIG:BZA %X you can write a very long message here...\n", 0 );
    WaitForTraceLogTransmissionDone();
    #endif
    
    #ifdef TRACELOG
    WaitForTraceLogTransmissionDone();
    /* test partial traces */
    TL0   (Id(60762), "\r\n\n\nwrn:a" );
    TL0   (Id(16372), "wrn:A" );
    TL0   (Id(38406), "rd_:b" );
    TL0   (Id(57240), "wr_:B" );
    TL0   (Id(2897), "msg:c" );
    TL0   (Id(50504), "tim:C\n" );
    WaitForTraceLogTransmissionDone();

    /* test normal traces */
    TL8_1( Id(5872), "WRN: TL8-1 %d\n", -1 );
    TL8_2( Id(30484), "ERR: TL8-2 %d +%d\n", -1, +2 );
    TL8_3( Id(2004), "TIM: TL8-3 +%d %d +%d\n", +1, -2, +3 );
    TL8_4( Id(60382), "RD_: TL8-4 %d +%d %d +%d\n", -1, +2, -3, +4 );
    TL8_5( Id(28207), "WR_: TL8-5 +%d %d +%d %d +%d\n", +1, -2, +3, -4, +5 );
    TL8_6( Id(25277), "ATT: TL8-6 %d %d +%d +%d %d %d \n", -1, -2, +3, +4, -5, -6 );
    TL8_7( Id(40040), "DBG: TL8-7 +%d +%d %d %d +%d +%d %d\n", +1, +2, -3, -4, +5, +6, -7 );
    TL8_8( Id(44810), "DIA: TL8-8 %d %d +%d +%d +%d +%d %d %d\n", -1, -2, +3, +4, +5, +6, -7, -8 );
    WaitForTraceLogTransmissionDone();

    TL16_1( Id(24536), "ISR: TL16-1 %x\n", 0x1311 );
    TL16_1( Id(58705), "SIG: TL16-1 %d\n", -1 );
    TL16_2( Id(41076), "TST: TL16-2 +%d %d\n", +1, -2 );
    TL16_3( Id(57861), "MSG: TL16-3 %d +%d %d\n", -1, +2, -3 );
    TL16_4( Id(14724), "MSG: TL16-4 +%d %d %d +%d\n", +1, -2, -3, +4 );
    TL32_1( Id(49014), "DBG: TL32-1 %x\r\n", 0x44332211 );
    TL32_2( Id(37420), "DBG: TL32-2 %d +0x%x\r\n", -1, +2 ); // ok
    TL32_1( Id(20985), "DBG: TL32-1 %x\r\n", 0x88332211 );
    TL32_2( Id(4373), "att:TL32-2 %08x%08x\r\n", 0xfedcba98, 0x76543210 ); // ok
    TL32_1( Id(8838), "DBG: TL32-1 %x\r\n", 0xaa332211 );
    TL8_1( Id(10), "SIG:BZA %X you can write a very long message here...\n", 0 );
    WaitForTraceLogTransmissionDone();
    #endif    
    
}


/*! write time measurements of logs
*/
void TraceLogTimeChecks( void )
{
    #ifdef TRACELOG
    static uint32_t Timer = 0u;
    uint32_t s, Ticks, Ticks2, Ticks3;

    // shortest trace
    WaitForTraceLogTransmissionDone();
    StartSysTickTimer( &Timer );
    TL0( Id(39987), "dbg:Shortest " );
    TL0( Id(18517), "msg:Trace " ); // <-- this one is measured
    TL0( Id(18511), "dbg:takes " );
    Ticks3 = ElapsedSysTicks( &Timer );
    
    StartSysTickTimer( &Timer );
    TL0( Id(23700), "sig:                      " );
    TL0( Id(6273), "err:         " );
    Ticks2 = ElapsedSysTicks( &Timer );

    Ticks = SysTickDelta(Ticks3, Ticks2 );
    s = TICKS_TO_MICROSECONDS( Ticks ); 
    TL32_2( Id(56764), "tim:%3u SysTicks =%2u us\r\n", Ticks, s );

    // longest trace
    WaitForTraceLogTransmissionDone();
    StartSysTickTimer( &Timer );
    TL0( Id(13013), "dbg:Longest " );
    TL32_2( Id(24551), "msg:Trace (%x %x) ", 0xffffeeee, 0xeeeeffff ); // <-- this one is measured
    TL0( Id(60822), "dbg:takes " );
    Ticks3 = ElapsedSysTicks( &Timer );
    
    StartSysTickTimer( &Timer );
    TL0( Id(22194), "dia:     " );
    TL0( Id(6569), "isr:       " );
    Ticks2 = ElapsedSysTicks( &Timer );

    Ticks = SysTickDelta(Ticks3, Ticks2 );
    s = TICKS_TO_MICROSECONDS( Ticks ); 
    TL32_2( Id(32412), "tim:%3u SysTicks =%2u us\r\n", Ticks, s );

    WaitForTraceLogTransmissionDone();
    #endif
}


/*! Check Timing functions
*/
void DelayChecks( void )
{
    #ifdef TRACELOG
    uint32_t Timer, Ticks, s;

    WaitForTraceLogTransmissionDone();
    StartSysTickTimer( &Timer );
    DelaySysTicks( 1000u, NoopUsTicks );
    Ticks = ElapsedSysTicks( &Timer );
    s = TICKS_TO_MICROSECONDS( Ticks );
    TL32_2( Id(17575), "tim:1000 SysTicks Delay duration: %u SysTicks = %u us\r\n", Ticks, s );

    WaitForTraceLogTransmissionDone();
    StartUsTimer( &Timer );
    DelayUs( 100u, NoopUsTicks ); // max 994 us with 1ms SysTick period
    s = ElapsedUs( &Timer );
    TL32_1( Id(198), "tim:100us Delay duration: = %u us\r\n", s );

    WaitForTraceLogTransmissionDone();
    StartSysTickTimer( &Timer );
    DelayUs( 100u, NoopUsTicks ); // 993us is the absolute maximum, but consider interrupts!
    Ticks = ElapsedSysTicks( &Timer );
    s = TICKS_TO_MICROSECONDS( Ticks );
    TL32_2( Id(32209), "tim:500us Delay duration: %u us = %u SysTicks\r\n", s, Ticks );

    WaitForTraceLogTransmissionDone();
    StartSysTickTimer( &Timer );
    DelayUs( 200u, NoopUsTicks ); // 993us is the absolute maximum, but consider interrupts!
    Ticks = ElapsedSysTicks( &Timer );
    s = TICKS_TO_MICROSECONDS( Ticks );
    TL32_2( Id(39271), "tim:500us Delay duration: %u us = %u SysTicks\r\n", s, Ticks );

    WaitForTraceLogTransmissionDone();
    StartSysTickTimer( &Timer );
    DelayUs( 400u, NoopUsTicks ); // 993us is the absolute maximum, but consider interrupts!
    Ticks = ElapsedSysTicks( &Timer );
    s = TICKS_TO_MICROSECONDS( Ticks );
    TL32_2( Id(59967), "tim:500us Delay duration: %u us = %u SysTicks\r\n", s, Ticks );

    WaitForTraceLogTransmissionDone();
    StartSysTickTimer( &Timer );
    DelayUs( 500u, NoopUsTicks ); // 993us is the absolute maximum, but consider interrupts!
    Ticks = ElapsedSysTicks( &Timer );
    s = TICKS_TO_MICROSECONDS( Ticks );
    TL32_2( Id(29340), "tim:500us Delay duration: %u us = %u SysTicks\r\n", s, Ticks );

    WaitForTraceLogTransmissionDone();
    StartMsTimer( &Timer ); // works only with 1ms interrupt
    DelayMs( 100, Pause );
    s = ElapsedMs( &Timer );
    TL32_1( Id(40053), "tim:100ms Delay duration: %u ms\r\n", s );
    WaitForTraceLogTransmissionDone();
    #endif // #ifdef TRACELOG
}


/*! write out all registers of a register set not containing 0
\param pName register set name
\param SetSize size of resister set area
\param BaseAddress start address of register set
*/
void TraceRegisterSet( char* pName, uint32_t SetSize, uint32_t BaseAddress )
{
    #ifdef TRACELOG
    static uint8_t n = 0;
    int i;
    int RegCount = SetSize>>2;
    TL32_2( Id(57464), "msg:    // BaseAddress=0x%08x, RegCount=%2u, ", BaseAddress, RegCount );
    TraceString( pName );
    TL0( Id(22807), "\r\n" );
    for( i = 0; i < RegCount; i++ )
    {
        uint32_t* RegisterAddress = (uint32_t*)BaseAddress + i;
        uint32_t RegisterValue = *RegisterAddress;
        if( RegisterValue )
        {
            TL32_2( Id(26290), "wr_:    {  (uint32_t*) 0x%08x, 0x%08xu  },", RegisterAddress, RegisterValue );
            TL8_2(  Id(47698), "msg: // %3u = 0x%02x  ", n, n );
            n++;
            TraceString( pName );
            TL0( Id(35971), "\r\n" );
            WaitForTraceLogTransmissionDone();
        }
    }
    #endif // #ifdef TRACELOG
}

/*! write out all configured registers (not containing 0)
\details This output is used for filling struct RegisterInfo_t RegisterInfo ind RegisterInfo.c.
\li some registers cannot written as DBGMCU and OB
\li some register bits need time for initialization
\li to reduce the initialization code amount check registers individually
*/
void TraceRegisterAll( void )
{
    TraceRegisterSet(  "TIM3",         sizeof(TIM_TypeDef),  TIM3_BASE );
#ifdef TIM6_BASE
    TraceRegisterSet(  "TIM6",         sizeof(TIM_TypeDef),  TIM6_BASE );
#endif
    TraceRegisterSet(  "TIM14",        sizeof(TIM_TypeDef),  TIM14_BASE );
    TraceRegisterSet(  "RTC",          sizeof(RTC_TypeDef),  RTC_BASE );
    TraceRegisterSet(  "WWDG",         sizeof(WWDG_TypeDef),  WWDG_BASE );
    TraceRegisterSet(  "IWDG",         sizeof(IWDG_TypeDef),  IWDG_BASE );
#ifdef USART2_BASE
    TraceRegisterSet(  "USART2",       sizeof(USART_TypeDef),  USART2_BASE );
#endif
    TraceRegisterSet(  "I2C1",         sizeof(I2C_TypeDef),  I2C1_BASE );
#ifdef I2C2_BASE
    TraceRegisterSet(  "I2C2",         sizeof(I2C_TypeDef),  I2C2_BASE );
#endif
    TraceRegisterSet(  "PWR",          sizeof(PWR_TypeDef),  PWR_BASE );
    TraceRegisterSet(  "SYSCFG",       sizeof(SYSCFG_TypeDef),  SYSCFG_BASE );
    TraceRegisterSet(  "EXTI",         sizeof(EXTI_TypeDef),  EXTI_BASE );
    TraceRegisterSet(  "ADC1",         sizeof(ADC_TypeDef),  ADC1_BASE );
    TraceRegisterSet(  "ADC1_COMMON",  sizeof(ADC_Common_TypeDef),  ADC1_BASE );
    TraceRegisterSet(  "ADC",         sizeof(ADC_Common_TypeDef),  ADC_BASE );
    TraceRegisterSet(  "TIM1",         sizeof(TIM_TypeDef),  TIM1_BASE );
    TraceRegisterSet(  "SPI1",         sizeof(SPI_TypeDef),  SPI1_BASE );
#ifdef SPI2_BASE
    TraceRegisterSet(  "SPI2",         sizeof(SPI_TypeDef),  SPI2_BASE );
#endif
    TraceRegisterSet(  "USART1",       sizeof(USART_TypeDef),  USART1_BASE );
#ifdef TIM15_BASE
    TraceRegisterSet(  "TIM15",        sizeof(TIM_TypeDef),  TIM15_BASE );
#endif
    TraceRegisterSet(  "TIM16",        sizeof(TIM_TypeDef),  TIM16_BASE );
    TraceRegisterSet(  "TIM17",        sizeof(TIM_TypeDef),  TIM17_BASE );
  //TraceRegisterSet(  "DBGMCU",       sizeof(DBGMCU_TypeDef),  DBGMCU_BASE );
    TraceRegisterSet(  "DMA1",         sizeof(DMA_TypeDef),  DMA1_BASE );
    TraceRegisterSet(  "DMA1_Channel1",sizeof(DMA_Channel_TypeDef),  DMA1_Channel1_BASE );
    TraceRegisterSet(  "DMA1_Channel2",sizeof(DMA_Channel_TypeDef),  DMA1_Channel2_BASE );
    TraceRegisterSet(  "DMA1_Channel3",sizeof(DMA_Channel_TypeDef),  DMA1_Channel3_BASE );
    TraceRegisterSet(  "DMA1_Channel4",sizeof(DMA_Channel_TypeDef),  DMA1_Channel4_BASE );
    TraceRegisterSet(  "DMA1_Channel5",sizeof(DMA_Channel_TypeDef),  DMA1_Channel5_BASE );
    TraceRegisterSet(  "FLASH",        sizeof(FLASH_TypeDef),  FLASH_R_BASE );
  //TraceRegisterSet(  "OB",           sizeof(OB_TypeDef),  OB_BASE );
    TraceRegisterSet(  "RCC",          sizeof(RCC_TypeDef),  RCC_BASE );
    TraceRegisterSet(  "CRC",          sizeof(CRC_TypeDef),  CRC_BASE );
    TraceRegisterSet(  "GPIOA",        sizeof(GPIO_TypeDef),  GPIOA_BASE );
    TraceRegisterSet(  "GPIOB",        sizeof(GPIO_TypeDef),  GPIOB_BASE );
    TraceRegisterSet(  "GPIOC",        sizeof(GPIO_TypeDef),  GPIOC_BASE );
    TraceRegisterSet(  "GPIOD",        sizeof(GPIO_TypeDef),  GPIOD_BASE );
    TraceRegisterSet(  "GPIOF",        sizeof(GPIO_TypeDef),  GPIOF_BASE );
}


/*! dump memory
\param Address start
\param Size amount of bytes
*/
void DumpMemory( uint32_t Address, uint32_t Size )
{
    int i;
    int Count = Size>>2;
    TL32_2( Id(49624), "att:    // Memory Dump: Address=0x%08x, Count=%2u\r\n", Address, Count );
    for( i = 0; i < Count; i++ )
    {
        uint32_t* pAddress = (uint32_t*)Address + i;
        uint32_t Value = *pAddress;
        if( Value )
        {
            TL32_2( Id(26063), "rd_:    {  (uint32_t*) 0x%08x, 0x%08xu  },\r\n", pAddress, Value );
            WaitForTraceLogTransmissionDone();
        }
    }
}
