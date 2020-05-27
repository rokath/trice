/*
 *  Software Platform Generated File
 *  --------------------------------
 */

#ifndef _SWP_PER_STM32F4XX_GPIO_CFG_INSTANCE_H
#define _SWP_PER_STM32F4XX_GPIO_CFG_INSTANCE_H

#include "per_stm32f4xx_gpio_cfg.h"

#include <stdint.h>
#include <stddef.h>

#define PER_STM32F4XX_GPIO_INSTANCE_BASEADDRESS_MIN  0x40020C00
#define PER_STM32F4XX_GPIO_INSTANCE_BASEADDRESS_MAX  0x40020C00


// instance data
typedef struct
{
    int instance_id;
        uint32_t baseaddress;
        void (*pinconfig)(void);
} per_stm32f4xx_gpio_cfg_instance_t;

extern const per_stm32f4xx_gpio_cfg_instance_t per_stm32f4xx_gpio_instance_table[]; 

#endif /* _SWP_PER_STM32F4XX_GPIO_CFG_INSTANCE_H */
