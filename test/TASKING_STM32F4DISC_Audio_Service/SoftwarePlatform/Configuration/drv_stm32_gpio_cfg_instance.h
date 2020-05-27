/*
 *  Software Platform Generated File
 *  --------------------------------
 */

#ifndef _SWP_DRV_STM32_GPIO_CFG_INSTANCE_H
#define _SWP_DRV_STM32_GPIO_CFG_INSTANCE_H

#include "drv_stm32_gpio_cfg.h"

#include <stdint.h>
#include "per_stm32f4xx_gpio_cfg_instance.h"

#define DRV_STM32_GPIO_INSTANCE_INITVALUE_MIN  0x0000
#define DRV_STM32_GPIO_INSTANCE_INITVALUE_MAX  0x0000


// instance data
typedef struct
{
        uint16_t initvalue;
    int per_stm32_gpio;
} drv_stm32_gpio_cfg_instance_t;

extern const drv_stm32_gpio_cfg_instance_t drv_stm32_gpio_instance_table[]; 

#endif /* _SWP_DRV_STM32_GPIO_CFG_INSTANCE_H */
