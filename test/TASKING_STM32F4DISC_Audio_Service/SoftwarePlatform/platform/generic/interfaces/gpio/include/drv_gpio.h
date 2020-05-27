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

/**
 * @file
 *
 * General purpose I/O interface.
 */
 
#ifndef _DRV_GPIO_H
#define _DRV_GPIO_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief    GPIO context
 *
 * A pointer to the GPIO context is returned by the gpio_open() function and must be passed to other functions.
 * The content of the context is not available to the application.
 */

typedef struct drv_gpio_s gpio_t;

extern gpio_t *gpio_open(unsigned int id);
extern void gpio_set_bits(gpio_t *drv, uint32_t mask);
extern void gpio_reset_bits(gpio_t *drv, uint32_t mask);
extern void gpio_write_bits(gpio_t *drv, uint32_t mask, uint32_t value);
extern uint32_t gpio_read_inbits(gpio_t *drv, uint32_t mask);
extern uint32_t gpio_read_outbits(gpio_t *drv, uint32_t mask);

#ifdef  __cplusplus
}
#endif

#endif /* _DRV_GPIO_H */
