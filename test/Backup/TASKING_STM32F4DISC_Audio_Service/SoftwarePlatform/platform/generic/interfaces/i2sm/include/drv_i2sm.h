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
 * Device driver interface for I2S peripherals.
 */

#ifndef _DRV_I2SM_H
#define _DRV_I2SM_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief    I2SM context
 *
 * A pointer to the I2SM context is returned by the i2sm_open() function and must be passed to other functions.
 * The content of the context is not available to the application.
 */

typedef struct i2sm_s i2sm_t;

extern i2sm_t * i2sm_open( unsigned int id );

//extern bool i2sm_set_format( i2sm_t * restrict drv, unsigned int samplerate, unsigned short samplesize );
extern int i2sm_set_samplerate( i2sm_t * restrict drv, unsigned int samplerate );
extern bool i2sm_set_samplesize( i2sm_t * restrict drv, uint8_t samplesize, uint8_t channels  );
extern void i2sm_set_callback( i2sm_t * restrict drv, void (*callback)(void *), void *ctx );

extern void i2sm_tx_start(i2sm_t * restrict drv);
extern void i2sm_tx_stop(i2sm_t * restrict drv);
extern size_t i2sm_write(i2sm_t * restrict drv, void * buffer, size_t samples);
extern bool i2sm_tx_avail( i2sm_t * restrict drv );
extern bool i2sm_tx_inuse( i2sm_t * restrict drv, void * buffer );
extern void i2sm_tx_flush( i2sm_t * restrict drv);
extern size_t i2sm_tx_pending(i2sm_t * restrict drv);

extern void i2sm_rx_start(i2sm_t * restrict drv);
extern void i2sm_rx_stop(i2sm_t * restrict drv);
extern void i2sm_read(i2sm_t * restrict drv, void * buffer, size_t samples);
extern uint32_t i2sm_rx_avail( i2sm_t * restrict drv );
extern void i2sm_rx_flush( i2sm_t * restrict drv);
extern size_t i2sm_rx_pending(i2sm_t * restrict drv);
extern void i2sm_rx_setbuf( i2sm_t * restrict drv, void * buffer, unsigned int n );

#ifdef  __cplusplus
}
#endif

#endif /* _DRV_I2SM_H */

