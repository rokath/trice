/*
 *  Software Platform Generated File
 *  --------------------------------
 */

#ifndef _SWP_PER_STM32F4XX_I2C_CFG_INSTANCE_H
#define _SWP_PER_STM32F4XX_I2C_CFG_INSTANCE_H

#include "per_stm32f4xx_i2c_cfg.h"

#include <stdint.h>
#include <stddef.h>

#define PER_STM32F4XX_I2C_INSTANCE_BASEADDRESS_MIN  0x40005400
#define PER_STM32F4XX_I2C_INSTANCE_BASEADDRESS_MAX  0x40005400


// instance data
typedef struct
{
    int instance_id;
        uint32_t baseaddress;
        void (*pinconfig)(void);
        int8_t interruptev;
        int8_t interrupter;
} per_stm32f4xx_i2c_cfg_instance_t;

extern const per_stm32f4xx_i2c_cfg_instance_t per_stm32f4xx_i2c_instance_table[]; 

#endif /* _SWP_PER_STM32F4XX_I2C_CFG_INSTANCE_H */
