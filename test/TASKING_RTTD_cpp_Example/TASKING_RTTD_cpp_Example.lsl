///////////////////////////////////////////////////////////////////////////
//
//  File        :  TASKING_RTTD_cpp_Example.lsl
//
//  Version     :  @(#)TASKING_RTTD_cpp_Example.lsl	1.3
//
//  Description :  Project specific Linker Script.
//
//  Copyright 2008-2015 Altium BV
//
////////////////////////////////////////////////////////////////////////////

#if defined(__PROC_ARMV7_M__)
#include "cm3.lsl"
#elif defined(__PROC_ARMV6_M__)
#include "cm0.lsl"
#elif defined(__PROC_STM32F030R8__)
#include "stm32f0xx.lsl"
#else
#include <device.lsl>
#endif
