/*
 *  Software Platform Generated File
 *  --------------------------------
 */

#ifndef _SWP_DRV_STM32_I2CM_CFG_INSTANCE_H
#define _SWP_DRV_STM32_I2CM_CFG_INSTANCE_H

#include "drv_stm32_i2cm_cfg.h"

#include <stdint.h>
#include "per_stm32f4xx_i2c_cfg_instance.h"

#define DRV_STM32_I2CM_INSTANCE_I2C_FREQUENCY_MIN  100000
#define DRV_STM32_I2CM_INSTANCE_I2C_FREQUENCY_MAX  100000


// instance data
typedef struct
{
        uint32_t i2c_frequency;
    int per_stm32_i2c;
} drv_stm32_i2cm_cfg_instance_t;

extern const drv_stm32_i2cm_cfg_instance_t drv_stm32_i2cm_instance_table[]; 

#endif /* _SWP_DRV_STM32_I2CM_CFG_INSTANCE_H */
