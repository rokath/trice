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

;
#if defined (SWP_STM32F0XX) || defined (STM32F30X) || defined(STM32F37X)

#include <assert.h>
#include <stdint.h>
#include <drv_i2cm.h>
#include <time.h>
#include <timing.h>
#include <devices.h>
#include "drv_stm32_i2cm_cfg_instance.h"
#include "stm32_chip_config.h"
#if defined(SWP_STM32F0XX)
#include "per_stm32f0xx_i2c_cfg_instance.h"
#include "stm32f0xx_i2c.h"
#elif defined(STM32F30X)
#include "per_stm32f30x_i2c_cfg_instance.h"
#include "stm32f30x_i2c.h"
#elif defined(STM32F37X)
#include "per_stm32f37x_i2c_cfg_instance.h"
#include "stm32f37x_i2c.h"
#endif
#include "../internal/drv_stm32_i2cm_internal.h"


i2cm_t i2cm_drv_table[DRV_STM32_I2CM_INSTANCE_COUNT];

/**
 * Configures the different clocks & GPIO ports and reset the peripheral and bus
 *
 * param per_cfg pointer to peripheral configuration
 * param drv pointer to driver context
 *
 * return 0 = okay, -1 = clock-line error, -2 = data-line error, -3 = unconnected pin(s), -4 = unknown id.
 */

#if defined(SWP_STM32F0XX)
static int init_bus_fxxx( const per_stm32f0xx_i2c_cfg_instance_t * per_cfg, i2cm_t * drv)
#elif defined(STM32F30X)
static int init_bus_fxxx( const per_stm32f30x_i2c_cfg_instance_t * per_cfg, i2cm_t * drv)
#elif defined(STM32F37X)
static int init_bus_fxxx( const per_stm32f37x_i2c_cfg_instance_t * per_cfg, i2cm_t * drv)
#endif
{
    GPIO_TypeDef *scl_port;
    GPIO_TypeDef *sda_port;
    uint16_t     scl_pin;
    uint16_t     sda_pin;

    switch(per_cfg->instance_id)
    {
    case 0:
#if !(defined(PINCFG_I2C1_SDA) && defined(PINCFG_I2C1_SCL))
        return -3;
#else
        sda_port = PINCFG_I2C1_SDA_PORT;
        sda_pin  = PINCFG_I2C1_SDA_PIN;
        scl_port = PINCFG_I2C1_SCL_PORT;
        scl_pin  = PINCFG_I2C1_SCL_PIN;
        PINCFG_I2C1_SCL_PORT_CLOCK_ENABLE;
        PINCFG_I2C1_SDA_PORT_CLOCK_ENABLE;
        break;
#endif
    case 1:
#if !(defined(PINCFG_I2C2_SDA) && defined(PINCFG_I2C2_SCL))
        return -3;
#else
        sda_port = PINCFG_I2C2_SDA_PORT;
        sda_pin  = PINCFG_I2C2_SDA_PIN;
        scl_port = PINCFG_I2C2_SCL_PORT;
        scl_pin  = PINCFG_I2C2_SCL_PIN;
        PINCFG_I2C2_SCL_PORT_CLOCK_ENABLE;
        PINCFG_I2C2_SDA_PORT_CLOCK_ENABLE;
        break;
#endif
    default:
        return -4;
    }


    GPIO_InitTypeDef gpio_init_struct;
    clock_t t;
    const clock_t timeout = CLOCKS_PER_SEC / 65536; // Actual speed should be 16384 kHz?

    // reset bus

    GPIO_WriteBit( scl_port, scl_pin, Bit_SET );
    GPIO_WriteBit( sda_port, sda_pin, Bit_SET );

    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz; // 50 MHz
    gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init_struct.GPIO_OType = GPIO_OType_OD;
    gpio_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;

    gpio_init_struct.GPIO_Pin = scl_pin; // I2C: SCL
    GPIO_Init( scl_port, &gpio_init_struct );
    gpio_init_struct.GPIO_Pin = sda_pin; // I2C: SDA
    GPIO_Init( sda_port, &gpio_init_struct );

    // Generate 9 stop conditions, without start:
    //
    //      ___ ___     ___     ___     ___     ___     ___     ___     ___     ___     __________
    //  SCL ___|   |___| ' |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|
    //                   '
    //      _________    '___     ___     ___     ___     ___     ___     ___     ___     ________
    //  SDA _________|___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|
    //                   '
    //                   stop

    for ( int i = 0; i < 9; i++ )
    {
        // Drop clock
        GPIO_WriteBit( scl_port, scl_pin, Bit_RESET );
        for ( t = clock() + timeout; clock() < t; ) __nop();

        // Drop data
        GPIO_WriteBit( sda_port, sda_pin, Bit_RESET );
        for ( t = clock() + timeout; clock() < t; ) __nop();

        // Assert clock
        GPIO_WriteBit( scl_port, scl_pin, Bit_SET );
        for ( t = clock() + timeout; clock() < t; ) __nop();
        if ( GPIO_ReadInputDataBit( scl_port, scl_pin ) == RESET ) return -1;   // Clock is kept low by a slave :-(

        GPIO_WriteBit( sda_port, sda_pin, Bit_SET );  // Assert data (hopefully)
        for ( t = clock() + timeout; clock() < t; ) __nop();

    }

    if ( GPIO_ReadInputDataBit( sda_port, sda_pin ) == RESET ) return -2;   // Did not finish, data kept low by slave

    switch(per_cfg->instance_id)
    {
    case 0:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        drv->i2cx = I2C1;
        break;

    case 1:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
        drv->i2cx = I2C2;
        break;

    }

    per_cfg->pinconfig();

    // Reset the I2C peripheral - it is not happy with unexpected stop conditions
    I2C_SoftwareResetCmd( drv->i2cx );

    return 0;
}


static uint32_t calc_timing(uint32_t freq)
{
    uint32_t result = 0;
    uint32_t presc, scll, sclh, sdadel, scldel;

    if (freq < 100000)
    {
        /* set to 10 kHz */
        presc = 1;
        scll = 199;
        sclh = 195;
        sdadel = 2;
        scldel = 4;
    }
    else if (freq < 400000)
    {
        /*set to 100 kHz */
        presc = 1;
        scll = 19;
        sclh = 15;
        sdadel = 2;
        scldel = 4;
    }
    else if (freq < 500000)
    {
        /*set to 400 kHz */
        presc = 0;
        scll = 9;
        sclh = 3;
        sdadel = 1;
        scldel = 3;
    }
    else
    {
        /* set to 500 kHz*/
        presc = 0;
        scll = 6;
        sclh = 3;
        sdadel = 1;
        scldel = 1;
    }

    result =  (scll & 0xff);
    result |= (sclh & 0xff) << 8;
    result |= (sdadel & 0x0f) << 16;
    result |= (scldel & 0x0f) << 20;
    result |= (presc & 0x0f) << 28;

    return result;
}


/**
 * @brief Open the I2CM device driver and initializes the hardware
 *
 * This function opens the device driver for an I2C peripheral on the STM32 core. You should call
 * this function once per instantiation.
 *
 * @param id Instance identifier as generated by the software platform
 *
 * @return NULL on error, pointer to instance context otherwise
 */

i2cm_t * i2cm_open( unsigned int id )
{
    // Assert for validity check on id should be here
//    assert( id < DRV_I2CM_INSTANCE_COUNT );

    i2cm_t * drv;
    drv = &i2cm_drv_table[id];

    const drv_stm32_i2cm_cfg_instance_t * restrict drv_cfg;
    drv_cfg = &drv_stm32_i2cm_instance_table[id];

#if defined(SWP_STM32F0XX)
    const per_stm32f0xx_i2c_cfg_instance_t * restrict per_cfg;
    per_cfg = &per_stm32f0xx_i2c_instance_table[drv_cfg->per_stm32_i2c];
#elif defined(STM32F30X)
    const per_stm32f30x_i2c_cfg_instance_t * restrict per_cfg;
    per_cfg = &per_stm32f30x_i2c_instance_table[drv_cfg->per_stm32_i2c];
#elif defined(STM32F37X)
    const per_stm32f37x_i2c_cfg_instance_t * restrict per_cfg;
    per_cfg = &per_stm32f37x_i2c_instance_table[drv_cfg->per_stm32_i2c];
#endif

    if ( drv->i2cx == NULL )
    {
        drv->i2c_frequency = drv_cfg->i2c_frequency;

        // Initialize the I2C peripheral
        I2C_StructInit( &drv->init );
        drv->init.I2C_Mode = I2C_Mode_I2C;
        drv->init.I2C_OwnAddress1 = 0x00;
        drv->init.I2C_Ack = I2C_Ack_Enable;
        drv->init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        drv->init.I2C_Timing = calc_timing(drv_cfg->i2c_frequency);

        if ( init_bus_fxxx( per_cfg, drv ) ) return NULL;

        I2C_Cmd( drv->i2cx, ENABLE );
        I2C_Init( drv->i2cx, &drv->init );
    }
    return drv;
}

#endif
