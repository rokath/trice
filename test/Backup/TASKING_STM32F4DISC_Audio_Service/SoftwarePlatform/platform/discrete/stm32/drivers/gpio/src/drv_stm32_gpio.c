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

#include <assert.h>
#include <stddef.h>
#include <drv_gpio.h>
#include <drv_gpio_internal.h>
#include <drv_stm32_gpio_cfg_instance.h>

#include <stm32_chip_config.h>


struct  drv_gpio_s
{
    GPIO_TypeDef * port;
    uint32_t mask;
};

static gpio_t gpio_drv_table[DRV_STM32_GPIO_INSTANCE_COUNT];

/**
 * @brief Open an instance of the driver
 *
 * @param id Instance ID as provided in the software platform document
 *
 * @return Pointer to a private device driver table to be used for future reference or NULL on error
 */

gpio_t * gpio_open( unsigned int id )
{
    assert( id < DRV_STM32_GPIO_INSTANCE_COUNT );

    gpio_t *drv = &gpio_drv_table[id];
    const drv_stm32_gpio_cfg_instance_t * restrict drv_cfg = &drv_stm32_gpio_instance_table[id];
#if defined(SWP_STM32F0XX)
    const per_stm32f0xx_gpio_cfg_instance_t * restrict per_cfg = &per_stm32f0xx_gpio_instance_table[drv_cfg->per_stm32_gpio];
#elif defined(STM32F10X)
    const per_stm32f10x_gpio_cfg_instance_t * restrict per_cfg = &per_stm32f10x_gpio_instance_table[drv_cfg->per_stm32_gpio];
#elif defined(STM32F2XX)
    const per_stm32f2xx_gpio_cfg_instance_t * restrict per_cfg = &per_stm32f2xx_gpio_instance_table[drv_cfg->per_stm32_gpio];
#elif defined(STM32F30X)
    const per_stm32f30x_gpio_cfg_instance_t * restrict per_cfg = &per_stm32f30x_gpio_instance_table[drv_cfg->per_stm32_gpio];
#elif defined(STM32F37X)
    const per_stm32f37x_gpio_cfg_instance_t * restrict per_cfg = &per_stm32f37x_gpio_instance_table[drv_cfg->per_stm32_gpio];
#elif defined(STM32F4XX)
    const per_stm32f4xx_gpio_cfg_instance_t * restrict per_cfg = &per_stm32f4xx_gpio_instance_table[drv_cfg->per_stm32_gpio];
#elif defined(STM32L1XX)
    const per_stm32l1xx_gpio_cfg_instance_t * restrict per_cfg = &per_stm32l1xx_gpio_instance_table[drv_cfg->per_stm32_gpio];
#else
#error unknown processor
#endif

    if ( drv->port == NULL )
    {
        switch (per_cfg->baseaddress)
        {
#if defined(PINCFG_GPIOA)
        case GPIOA_BASE:
            drv->mask = PINCFG_GPIOA_MASK;
            break;
#endif
#if defined(PINCFG_GPIOB)
        case GPIOB_BASE:
            drv->mask = PINCFG_GPIOB_MASK;
            break;
#endif
#if defined(PINCFG_GPIOC)
        case GPIOC_BASE:
            drv->mask = PINCFG_GPIOC_MASK;
            break;
#endif
#if defined(PINCFG_GPIOD)
        case GPIOD_BASE:
            drv->mask = PINCFG_GPIOD_MASK;
            break;
#endif
#if defined(PINCFG_GPIOE)
        case GPIOE_BASE:
            drv->mask = PINCFG_GPIOE_MASK;
            break;
#endif
#if defined(PINCFG_GPIOF)
        case GPIOF_BASE:
            drv->mask = PINCFG_GPIOF_MASK;
            break;
#endif
#if defined(PINCFG_GPIOG)
        case GPIOG_BASE:
            drv->mask = PINCFG_GPIOG_MASK;
            break;
#endif
#if defined(PINCFG_GPIOH)
        case GPIOH_BASE:
            drv->mask = PINCFG_GPIOH_MASK;
            break;
#endif
#if defined(PINCFG_GPIOI)
        case GPIOI_BASE:
            drv->mask = PINCFG_GPIOI_MASK;
            break;
#endif
#if defined(PINCFG_GPIOJ)
        case GPIOJ_BASE:
            drv->mask = PINCFG_GPIOJ_MASK;
            break;
#endif
#if defined(PINCFG_GPIOK)
        case GPIOK_BASE:
            drv->mask = PINCFG_GPIOK_MASK;
            break;
#endif
        }

        per_cfg->pinconfig();

        drv->port = (GPIO_TypeDef *) per_cfg->baseaddress;

        gpio_write_bits(drv, drv->mask, drv_cfg->initvalue );
    }

    return drv;
}

/**
 * @brief Set pins in output port to 1
 *
 * @param drv Pointer to device driver context as returned by gpio_open()
 * @param mask Pins to be set to '1'
 *
 * @return Nothing
 */

void gpio_set_bits( gpio_t * drv, uint32_t mask )
{
    assert((mask & ~drv->mask) == 0);

    GPIO_SetBits( drv->port, (uint16_t) mask );
}

/**
 * @brief Set pins in output port to 0
 *
 * @param drv Pointer to device driver context as returned by gpio_open()
 * @param mask Pins to be set to '0'
 *
 * @return Nothing
 */

void gpio_reset_bits( gpio_t * drv, uint32_t mask )
{
    assert((mask & ~drv->mask) == 0);

    GPIO_ResetBits( drv->port, (uint16_t) mask );
}

/**
 * @brief Set pins in output port to a specified value
 *
 * Set the pins on an output port to the specified value. The action basically sets the pins defined
 * by the mask to the bits in the specified value. Thus if you want pin 0..3 to be 1010, you can
 * call this function with mask = 0x000F (pins 0..3) and value 0x000A
 *
 * @param drv Pointer to device driver context as returned by gpio_open()
 * @param mask Pins to be changed
 * @param value Value to be set on the masked pins
 *
 * @return Nothing
 */


void gpio_write_bits( gpio_t * drv, uint32_t mask, uint32_t value )
{
    assert((mask & ~drv->mask) == 0);

    uint16_t mask16 = (uint16_t) mask & 0xFFFF;
    uint16_t val16 = (uint16_t) value & 0xFFFF;

    if ( mask16 == 0xFFFF )
    {
        GPIO_Write( drv->port, val16 );
    }
    else
    {
        GPIO_Write( drv->port, (GPIO_ReadOutputData( drv->port ) & ~mask16) | (val16 & mask16));
    }
}

/**
 * @brief Read a number of bits from the input port
 *
 * @param drv Pointer to device driver context as returned by gpio_open()
 * @param mask Pins to be read
 *
 * @return Value as read from said pins
 */

uint32_t gpio_read_inbits( gpio_t * drv, uint32_t mask )
{
    assert((mask & ~drv->mask) == 0);

    return GPIO_ReadInputData( drv->port ) & mask;
}

/**
 * @brief Read a number of bits from the output port
 *
 * @param drv Pointer to device driver context as returned by gpio_open()
 * @param mask Pins to be read
 *
 * @return Value as read from the internal flipflops of the port
 */

uint32_t gpio_read_outbits( gpio_t * drv, uint32_t mask )
{
    assert((mask & !drv->mask) == 0);

    return GPIO_ReadOutputData( drv->port ) & mask;
}

