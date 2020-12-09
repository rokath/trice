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
 * Device driver for CS4322 24-bit, 192 kHz Stereo Audio CODEC
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <drv_i2cm.h>
#include <drv_cs4322.h>
#include <drv_cs4322_cfg_instance.h>

struct cs4322_s
{
    uint8_t i2c_address;
    i2cm_t * i2cm;
    uint8_t if_ctrl1;
} ;

static cs4322_t drv_table[DRV_CS4322_INSTANCE_COUNT];

/**
 * @brief Open an instance of the device driver and initialize
 *
 * This function initializes an instance of both, the device driver and the underlying
 * hardware. You should call this function only once per instance.
 *
 * Note: you must make sure the device is properly reset before you call this function.
 * On the STM3210C-EVAL evaluation board, the reset is tied to pin IN2 from the second
 * I/O expander (I2C address 0x88). You must raise this pin before calling cs4322_open()!
 *
 * @param id Device driver instance number
 *
 * @return Pointer to device driver context structure or NULL on failure
 */

cs4322_t * cs4322_open( unsigned int id )
{
    assert( id < DRV_CS4322_INSTANCE_COUNT );

    cs4322_t * restrict drv = &drv_table[id];
    const drv_cs4322_cfg_instance_t * restrict drv_cfg = &drv_cs4322_instance_table[id];
    i2cm_t * i2cm;
    uint8_t val;

    if ( !drv->i2cm )
    {
        drv->i2c_address = drv_cfg->i2c_address;
        drv->if_ctrl1 = drv_cfg->i2s_standard | drv_cfg->i2s_dataformat | drv_cfg->i2s_cpol;
        if (i2cm = i2cm_open( drv_cfg->drv_i2cm ), !i2cm )
        {
            drv = NULL;
        }
        else if ( (i2cm_read_reg8( i2cm, drv->i2c_address, CS4322_REG_ID, &val ) != 1) || (val & 0xF8) != 0xE0 )
        {
            drv = NULL;
        }
        else
        {
            drv->i2cm = i2cm;
        }
        if ( drv )
        {
            cs4322_init( drv );
        }
    }

    return drv;
}

/**
 * @brief Initialize the CS43L22
 *
 * This function initializes the CODEC to something useful. You must reset CODEC first through
 * it's RESET line to make sure the initialization is correct.
 *
 * Initialization sequence: power down, auto detect headphone & speakers, I2S standard Phillips,
 * master volume 0 dB and power up
 *
 * @param drv Pointer to device driver context as returned from cs4322_open()
 */

void cs4322_init( cs4322_t * restrict drv )
{

    i2cm_write_reg8( drv->i2cm, drv->i2c_address, CS4322_REG_PWR_CTRL1, 0x01 );         // Power down
    i2cm_write_reg8( drv->i2cm, drv->i2c_address, CS4322_REG_PWR_CTRL2, 0x05 );         // Detect headphone & speaker channels automagically
    i2cm_write_reg8( drv->i2cm, drv->i2c_address, CS4322_REG_CLK_CTRL, 0x81 );          // Speed auto detect
    i2cm_write_reg8( drv->i2cm, drv->i2c_address, CS4322_REG_IF_CTRL1, drv->if_ctrl1 );
    cs4322_set_volume( drv, 208 );                                                       // Set master volume to 0 dB
    i2cm_write_reg8( drv->i2cm, drv->i2c_address, CS4322_REG_PWR_CTRL1, 0x9E );         // Power up
}

/**
 * @brief This function is an empty stub, the CS43L22 uses automatic speed detection through MCLK
 *
 * @param drv Pointer to device driver context as returned from cs4322_open()
 */

void cs4322_set_speed( cs4322_t * restrict drv, unsigned int clockspeed )
{
}

/**
 * @brief Set I2S data format. To be implemented.
 *
 * @param drv Pointer to device driver context as returned from cs4322_open()
 * @param format I2S data format
 */

void cs4322_set_format( cs4322_t * restrict drv, uint8_t format )
{
}

/**
 * @brief Controls the master volume
 *
 * Master volume can be set from +12dB to -102dB in steps of 0.5dB. 0xFF = +12dB, 0x00 = -120dB.
 *
 * @param drv Pointer to device driver context as returned from cs4322_open()
 * @param volume Volume to be set
 */

void cs4322_set_volume( cs4322_t * restrict drv, uint8_t volume )
{
    /* Set the Master volume */
    volume += 0x18 - 0xFF;
    i2cm_write_reg8( drv->i2cm, drv->i2c_address, CS4322_REG_MASTER_VOL_A, volume );
    i2cm_write_reg8( drv->i2cm, drv->i2c_address, CS4322_REG_MASTER_VOL_B, volume );
}

/**
 * @brief Start playing
 *
 * This function un-mutes the CODEC
 *
 * @param drv Pointer to device driver context as returned from cs4322_open()
 */

void cs4322_play( cs4322_t * restrict drv )
{
    // Unmute
    i2cm_write_reg8( drv->i2cm, drv->i2c_address, CS4322_REG_PLAY_CTRL1, 0x60 );
    i2cm_write_reg8( drv->i2cm, drv->i2c_address, CS4322_REG_PLAY_CTRL2, 0x08 );
}

/**
 * @brief This function is a stub, the CS43L22 does not provide audio input
 *
 * @param drv Pointer to device driver context as returned from cs4322_open()
 */

void cs4322_record( cs4322_t * restrict drv )
{
    // Not implemented
}

/**
 * @brief Stop playing
 *
 * This function mutes the CODEC by muting master playback in the playback control registers
 *
 * @param drv Pointer to device driver context as returned from cs4322_open()
 */

void cs4322_stop( cs4322_t * restrict drv )
{
    // Play control "Mute"
    i2cm_write_reg8( drv->i2cm, drv->i2c_address, CS4322_REG_PLAY_CTRL1, 0x63 );
    i2cm_write_reg8( drv->i2cm, drv->i2c_address, CS4322_REG_PLAY_CTRL2, 0xF8 );
}


