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
 * Interface to I2C control port of CS4322 stereo audio CODEC.
 */

#ifndef _DRV_CS4322_H
#define _DRV_CS4322_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

// CS4322 registers
#define CS4322_REG_ID           0x01
#define CS4322_REG_PWR_CTRL1    0x02
#define CS4322_REG_PWR_CTRL2    0x04
#define CS4322_REG_CLK_CTRL     0x05
#define CS4322_REG_IF_CTRL1     0x06
#define CS4322_REG_IF_CTRL2     0x07
#define CS4322_REG_PT_SEL_A     0x08
#define CS4322_REG_PT_SEL_B     0x09
#define CS4322_REG_AN_ZC_SR     0x0A
#define CS4322_REG_PS_GANG_CTRL 0x0C
#define CS4322_REG_PLAY_CTRL1   0x0D
#define CS4322_REG_MISC_CTRL    0x0E
#define CS4322_REG_PLAY_CTRL2   0x0F
#define CS4322_REG_PTA_VOL      0x14
#define CS4322_REG_PTB_VOL      0x15
#define CS4322_REG_PCMA_VOL     0x1A
#define CS4322_REG_PCMB_VOL     0x1B
#define CS4322_REG_BEEPFREQ     0x1D
#define CS4322_REG_BEEPTONE     0x1E
#define CS4322_REG_TONE_CTRL    0x1F
#define CS4322_REG_MASTER_VOL_A 0x20
#define CS4322_REG_MASTER_VOL_B 0x21
#define CS4322_REG_HDPHONE_VOL_A 0x22
#define CS4322_REG_HDPHONE_VOL_B 0x23
#define CS4322_REG_SPKR_VOL_A   0x24
#define CS4322_REG_SPKR_VOL_B   0x25
#define CS4322_REG_CHMIX_SWAP   0x26
#define CS4322_REG_LIMIT_CTRL1  0x27
#define CS4322_REG_LIMIT_CTRL2  0x28
#define CS4322_REG_LIMIT_ATTACK 0x29
#define CS4322_REG_OVF_CLK_STAT 0x2E
#define CS4322_REG_BAT_COMP     0x2F
#define CS4322_REG_VP_BATT_LVL  0x30
#define CS4322_REG_SPKR_STAT    0x31
#define CS4322_REG_CHRGPUMP_FREQ 0x34

typedef struct cs4322_s cs4322_t;

// Standard audio control interface
extern cs4322_t * cs4322_open( unsigned int id );
extern void cs4322_init( cs4322_t * restrict drv );
extern void cs4322_set_speed( cs4322_t * restrict drv, unsigned int clockspeed );
extern void cs4322_set_format( cs4322_t * restrict drv, uint8_t format );
extern void cs4322_set_volume( cs4322_t * restrict drv, uint8_t volume );
extern void cs4322_play( cs4322_t * restrict drv );
extern void cs4322_record( cs4322_t * restrict drv );
extern void cs4322_stop( cs4322_t * restrict drv );

#endif	// _DRV_CS4322_H
