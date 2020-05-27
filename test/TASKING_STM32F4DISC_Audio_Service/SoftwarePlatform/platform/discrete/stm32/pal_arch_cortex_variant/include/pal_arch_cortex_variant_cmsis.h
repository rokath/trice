/* ------------------------------------------------------------
**
**  Copyright (c) 2013-2015 Altium Limited
**
**  This software is the proprietary, copyrighted property of
**  Altium Ltd. All Right Reserved.
**
**  SVN revision information:
**  $Rev: 14907 $:
**  $Date: 2015-01-19 13:30:51 +0100 (Mon, 19 Jan 2015) $:
**
** ------------------------------------------------------------
*/

#ifndef _CMCSIS_ARCH_H
#define _CMCSIS_ARCH_H

#if STM32F2XX
 #include <stm32f2xx.h>
#elif STM32L1XX
 #include <stm32l1xx.h>
#elif STM32F4XX
 #include <stm32f4xx.h>
#elif STM32F30X
 #include <stm32f30x.h>
#elif STM32F37X
 #include <stm32f37x.h>
#elif STM32F10X
 #include <stm32f10x.h>
#elif SWP_STM32F0XX
 #include <stm32f0xx.h>
#else
 #error no processor defined
#endif

/* clock source override for PAL */
//#define PAL_ARCH_CLOCKHZ    SystemCoreClock

static inline void pal_architecture_variant_init(void)
{
#if STM32F10X_CONF_CUSTOM_INIT // NOTE: not #ifdef -- JH
    STM32F10X_CONF_CUSTOM_INIT_CALLBACK();
#endif
    SystemCoreClockUpdate();
}

#endif /* _CMCSIS_ARCH_H */
