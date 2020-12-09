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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <drv_i2sm.h>
#include <drv_stm32_i2sm_internal.h>


/**
 * @brief Set callback-funtion
 *
 * This function sets the address of the callback-function.
 * If set, the callback-function is called eacht time a buffer has finisched playing.
 * As the callback-function is called from inside the interrupt routine, the callback-function needs to be fast,
 * otherwise the sound may get interrupted.
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 * @param callback    Pointer to callback-function
 * @param context     Pointer to context for callback function
 *
 * @return Nothing
 */

void i2sm_set_callback( i2sm_t * restrict drv, void (*callback)(void *), void *context )
{
    drv->callback = callback;
    drv->cb_context = context;
}

/**
 * @brief Write samples to the I2S peripheral
 *
 * Send audio data through the I2S interface. How the data is formatted does not matter for this function.
 *
 * Since the I2S peripheral is limited to stereo signals, the software should make sure the first sample
 * is meant for the left channel and the size should be a multiple of 2 (1 channel left,1 channel right).
 *
 * Note: this is a non-blocking function. It is up to you to make sure the I2S is done with this buffer
 * before writing to or otherwise invalidating it!
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 * @param buffer    Pointer to sample data (left channel first)
 * @param samples   Size of data buffer (in samples)
 *
 * @return Number of written samples
 */

size_t i2sm_write(i2sm_t * restrict drv, void * buffer, size_t samples )
{
	if (samples > 0xfffe) samples = 0xfffe;
    assert( (samples & 1) == 0 );
    assert( buffer != NULL );
    drv->dma_nextbuf_size = (uint16_t)samples;
    drv->dma_nextbuf = (uint32_t) buffer;
    return samples;
}

/**
 * @brief Read samples from the I2S peripheral
 *
 * This function removes data from the I2S interface. The data is read into the previously scheduled
 * buffer
 *
 * Note: this is a non-blocking function. You should call i2sm_rx_avail() to make sure
 * the entire buffer is filled before calling this function.
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 * @param buffer    Pointer to sample data (ignored)
 * @param samples   Size of data buffer (in samples)
 *
 * @return Nothing
 */

void i2sm_read(i2sm_t * restrict drv, void * buffer, unsigned int samples)
{
    drv->rx_current -= samples;
}

/**
 * @brief Stop the I2S transmitter
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 *
 * @return Nothing
 */

void i2sm_tx_stop( i2sm_t * restrict drv )
{
    DMA_Cmd( drv->dma_stream, DISABLE );
    DMA_ITConfig( drv->dma_stream, DMA_IT_TC, DISABLE );
    I2S_Cmd( drv->i2sx, DISABLE );
    drv->dma_curbuf = 0;
    drv->dma_nextbuf = 0;
}

/**
 * @brief Check if there is space is available in the device driver for further transmission
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 *
 * @return True if there's enough space to store data, false otherwise
 */

bool i2sm_tx_avail(i2sm_t * restrict drv)
{
    return ( drv->dma_nextbuf == 0 );
}

/**
 * @brief Retrieve audio word length settings from the hardware
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 *
 * @return the audio word length as currently being used by the transmitter
 */

int i2sm_get_wwidth(i2sm_t * restrict drv)
{
    switch( drv->i2s_init.I2S_DataFormat )
    {
    case I2S_DataFormat_16b : return 16;
    case I2S_DataFormat_24b : return 24;
    default : return 32;
    }
}

/**
 * @brief Stop the I2S receiver
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 *
 * @return Nothing
 */

void i2sm_rx_stop( i2sm_t * restrict drv )
{
    I2S_Cmd( drv->i2sx, DISABLE );
    DMA_Cmd( drv->dma_stream, DISABLE );
    DMA_ITConfig( drv->dma_stream, DMA_IT_TC, DISABLE );
    drv->dma_nextbuf = 0;
}

/**
 * @brief Schedule next buffer for receiving
 *
 * This function sets the next buffer to start receiving in. The receiver
 * will switch to this buffer as soon as either the current DMA run is finished
 * or (if no DMA is running yet) as soon as i2sm_rx_start() is called.
 *
 * For proper operation, make sure the buffer is large enough to
 * store n x channels x bytes_sample bytes.
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 * @param buffer    Pointer to next buffer
 * @param n         Size of next buffer (in samples)
 *
 * @return Nothing
 */

void i2sm_rx_setbuf( i2sm_t * restrict drv, void * buffer, unsigned int n )
{
    drv->dma_nextbuf_size = (uint16_t)n;
    drv->dma_nextbuf = (uint32_t) buffer;
}

/**
 * @brief Retrieve the number of samples available in the receiver buffers
 *
 * This function retrieves the number of samples available in the receiver
 * buffers. You should make sure to always read() complete buffers, otherwise
 * this function will fail.
 *
 * @param  drv   Pointer to device driver context as returned by i2sm_open()
 *
 * @return the number of samples available in the buffer
 */

uint32_t i2sm_rx_avail( i2sm_t * restrict drv )
{
    return drv->rx_current - DMA_GetCurrDataCounter( drv->dma_stream );
}

/**
 * @brief Flush the receiver buffer
 *
 * @param  drv   Pointer to device driver context as returned by i2sm_open()
 *
 * @return Nothing
 */

void i2sm_rx_flush( i2sm_t * restrict drv )
{
    drv->dma_nextbuf = 0;
    drv->rx_current = 0;
    drv->rx_done = 0;
}


/**
 * @brief Test whether it's safe to write to a buffer
 *
 * This function tests whether or not a buffer is currently waiting to be scheduled
 * or is being played from right now.
 *
 * @param  drv   Pointer to device driver context as returned by i2sm_open()
 * @param buffer Pointer to buffer to be tested for
 *
 * @return false if the buffer is not in use (and thus is safe to be written into), true otherwise
 */

bool i2sm_tx_inuse( i2sm_t * restrict drv, void * buffer )
{
    return ((uintptr_t)buffer == drv->dma_nextbuf) || ((uintptr_t)buffer == drv->dma_curbuf);
}


/**
 * @brief Test the number of samples pending in the transmit chain
 *
 * This function tests the number of samples pending in the current buffer and in the next buffer, if set.
 *
 * @param  drv   Pointer to device driver context as returned by i2sm_open()
 *
 * @return the number of pending samples
 */

size_t i2sm_tx_pending(i2sm_t * restrict drv)
{
    size_t retval = DMA_GetCurrDataCounter( drv->dma_stream );
    if (drv->dma_nextbuf) retval += drv->dma_nextbuf_size;
    return retval;
}


/**
 * @brief Test the number of samples pending in the transmit chain
 *
 * This function tests the number of samples pending in the current buffer and in the next buffer, if set.
 *
 * @param  drv   Pointer to device driver context as returned by i2sm_open()
 *
 * @return the number of pending samples
 */

void i2sm_tx_flush(i2sm_t * restrict drv)
{
    drv->dma_nextbuf = 0;
}


/**
 * @brief Test the number of samples pending in the receive chain
 *
 * This function tests the number of samples pending in the current buffer and in the next buffer, if set.
 *
 * @param  drv   Pointer to device driver context as returned by i2sm_open()
 *
 * @return the number of pending samples
 */

size_t i2sm_rx_pending(i2sm_t * restrict drv)
{
    size_t retval = DMA_GetCurrDataCounter( drv->dma_stream );
    return retval;
}



