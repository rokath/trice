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

#ifndef _DRV_STM32_I2CM_INTERNAL_H_
#define _DRV_STM32_I2CM_INTERNAL_H_

#if defined(SWP_STM32F0XX)
#include "stm32f0xx_i2c.h"
#elif defined(STM32F10X)
#include "stm32f10x_i2c.h"
#elif defined(STM32F2XX)
#include "stm32f2xx_i2c.h"
#elif defined(STM32F30X)
#include "stm32f30x_i2c.h"
#elif defined(STM32F37X)
#include "stm32f37x_i2c.h"
#elif defined(STM32F4XX)
#include "stm32f4xx_i2c.h"
#elif defined(STM32L1XX)
#include "stm32l1xx_i2c.h"
#else
#error no processor defined
#endif

extern i2cm_t i2cm_drv_table[];

struct  drv_i2cm_s
{
    I2C_TypeDef *           i2cx;
    I2C_InitTypeDef         init;
    uint32_t                i2c_frequency;
    uint8_t                 occupied;
};



#endif
