/* ------------------------------------------------------------
**
**  Copyright (c) 2013-2015 Altium Limited
**
**  This software is the proprietary, copyrighted property of
**  Altium Ltd. All Right Reserved.
**
**  SVN revision information:
**  $Rev: 15177 $:
**  $Date: 2015-06-17 11:24:57 +0200 (Wed, 17 Jun 2015) $:
**
** ------------------------------------------------------------
*/

;   // avoid "empty source file" warning

#if defined(STM32F30X)


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#include <stm32f30x_rcc.h>

#include <interrupts.h>
#include <per_stm32f30x_i2s_cfg_instance.h>
#include <drv_stm32_i2sm_cfg_instance.h>
#include <drv_i2sm.h>

#include <stm32_chip_config.h>

static i2sm_t drv_stm32_i2sm_table[DRV_STM32_I2SM_INSTANCE_COUNT];


/**
 * Switch to the next transmit- or receive buffer. Since this action is exactly the same
 * for transmit and receive, it is shared for RX and TX
 */

static void switch_to_next_buf( i2sm_t * restrict drv )
{
    drv->dma_curbuf = drv->dma_nextbuf;
    drv->dma_init.DMA_MemoryBaseAddr = drv->dma_nextbuf;
    drv->dma_init.DMA_BufferSize = drv->dma_nextbuf_size;
    drv->rx_done += drv->rx_current;
    drv->rx_current = drv->dma_nextbuf_size;
    DMA_Cmd( drv->dma_stream, DISABLE );
    DMA_Init( drv->dma_stream, &drv->dma_init );
    drv->dma_nextbuf = 0;
    DMA_Cmd( drv->dma_stream, ENABLE );
}

/**
 * This is the interrupt handler. It just switches from one buffer to the next and stops transmission
 * it there is no buffer waiting. Since this action is the same for receive and transmit, the function
 * is shared for RX and TX.
 */

__INTERRUPT_NATIVE void stm32f30x_i2sm_handler_native( void )
{
    int num = interrupt_get_current();
    i2sm_t * restrict drv = (i2sm_t *) interrupt_native_context( num );

    // acknowledge as soon as possible
    interrupt_acknowledge( num );

    switch( num )
    {
    case DMA1_Channel4_IRQn: // I2S2 RX
        DMA_ClearFlag( DMA1_FLAG_TC4 );
        break;
    case DMA1_Channel5_IRQn: // I2S2 TX
        DMA_ClearFlag( DMA1_FLAG_TC5 );
        break;
    case DMA2_Channel1_IRQn: // I2S3 RX
        DMA_ClearFlag( DMA2_FLAG_TC1 );
        break;
    case DMA2_Channel2_IRQn: // I2S3 TX
        DMA_ClearFlag( DMA2_FLAG_TC2 );
        break;
    default :   // Should never happen!
        ;
    }

    if ( drv->dma_nextbuf )
    {
        switch_to_next_buf( drv );
        if (drv->callback != NULL)
        {
            drv->callback(drv->cb_context);
        }
    }
    else
    {
        // Disable the interrupt
        drv->dma_curbuf = 0;
        DMA_Cmd( drv->dma_stream, DISABLE );
        DMA_ITConfig( drv->dma_stream, DMA_IT_TC, DISABLE );
        I2S_Cmd( drv->i2sx, DISABLE );
        drv->running = false;
    }
}


/**
 * @brief Initialize the I2S device driver and the hardware
 *
 * This function initializes an instance of both, the device driver and the underlying
 * hardware. You should call this function only once per instance.
 *
 * @param Device driver instance number
 *
 * @return Pointer to device driver context structure or NULL on failure
 *
 */
i2sm_t * i2sm_open( unsigned int id )
{
    uint32_t irq;

    assert( id < DRV_STM32_I2SM_INSTANCE_COUNT );

    i2sm_t * drv;
    const drv_stm32_i2sm_cfg_instance_t * restrict drv_cfg;
    const per_stm32f30x_i2s_cfg_instance_t * restrict per_cfg;
    drv_cfg = &drv_stm32_i2sm_instance_table[id];
    per_cfg = &per_stm32f30x_i2s_instance_table[drv_cfg->per_stm32_i2s];
    drv = &drv_stm32_i2sm_table[id];

    if ( drv->i2sx == NULL )
    {
        I2S_StructInit( &drv->i2s_init );
        drv->i2s_init.I2S_Mode = drv_cfg->i2s_mode;
        drv->i2s_init.I2S_Standard = drv_cfg->i2s_standard;
        drv->i2s_init.I2S_DataFormat = drv_cfg->i2s_dataformat;
        drv->i2s_init.I2S_AudioFreq = drv_cfg->i2s_audiofreq;
        drv->i2s_init.I2S_CPOL = drv_cfg->i2s_cpol;

        drv->dma_stream = (drv_cfg->i2s_mode == I2S_Mode_MasterTx) ? per_cfg->dma_channel_tx : per_cfg->dma_channel_rx;

        drv->dma_init.DMA_MemoryBaseAddr = 0;
        drv->dma_init.DMA_DIR = (drv_cfg->i2s_mode == I2S_Mode_MasterTx) ? DMA_DIR_PeripheralDST : DMA_DIR_PeripheralSRC;
        drv->dma_init.DMA_BufferSize = 0xFFFE;
        drv->dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        drv->dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
        drv->dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        drv->dma_init.DMA_MemoryDataSize = (drv_cfg->i2s_dataformat == I2S_DataFormat_16b) ?  DMA_MemoryDataSize_HalfWord : DMA_MemoryDataSize_Word;    // DMA_MemoryDataSize_Byte;
        drv->dma_init.DMA_Mode = DMA_Mode_Normal;
        drv->dma_init.DMA_Priority = drv_cfg->dma_priority;

        drv->dma_nextbuf = 0;
        drv->dma_nextbuf_size = 0xFFFE;

        switch(per_cfg->baseaddress)
        {
        case SPI2_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
            drv->i2sx = SPI2;
#if defined(PINCFG_I2S2_MCK)
            drv->i2s_init.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
#else
            drv->i2s_init.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
#endif
            break;

        case SPI3_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
            drv->i2sx = SPI3;
#if defined(PINCFG_I2S3_MCK)
            drv->i2s_init.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
#else
            drv->i2s_init.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
#endif
            break;

        default:
            return NULL; // unsupported

        }

        per_cfg->pinconfig();

        DMA_Cmd(drv->dma_stream, DISABLE);
        DMA_DeInit(drv->dma_stream);

        I2S_Init( drv->i2sx, &drv->i2s_init );
        RCC_I2SCLKConfig(RCC_I2S2CLKSource_SYSCLK);
        drv->dma_init.DMA_PeripheralBaseAddr = (uint32_t) &drv->i2sx->DR;
        DMA_Init( drv->dma_stream, &drv->dma_init );
        SPI_I2S_DMACmd( drv->i2sx, (drv->dma_init.DMA_DIR == DMA_DIR_PeripheralDST) ? SPI_I2S_DMAReq_Tx : SPI_I2S_DMAReq_Rx, ENABLE );
        drv->running = false;
    }

    if (drv->dma_stream == DMA1_Channel4)
    { irq = DMA1_Channel4_IRQn; }
    else if (drv->dma_stream == DMA1_Channel5)
    { irq = DMA1_Channel5_IRQn; }
    else if (drv->dma_stream == DMA2_Channel1)
    { irq = DMA2_Channel1_IRQn; }
    else if (drv->dma_stream == DMA2_Channel2)
    { irq = DMA2_Channel2_IRQn; }
    else
    {
        // invalid DMA channel
        assert(0);
        return NULL;
    }

    // Install the rx or tx DMA interrupt handler
    interrupt_register_native(irq, (void*) drv, stm32f30x_i2sm_handler_native);
    interrupt_acknowledge(irq);
    interrupt_enable(irq);


    return drv;
}

/**
 * @brief Set I2S samplerate
 *
 * This function sets the audio samplerate in sample slices per second (e.g. 44100 for CD quality).
 *
 * Due to limitations in the STM library, the samplerates that can be set are restricted to a predefined
 * set of most used frequencies. The frequency used is chosen to be as close to the requested frequency
 * as possible.
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 * @param samplerate Samplerate to be used for communication.
 *
 * @return Nothing
 */

int i2sm_set_samplerate( i2sm_t * restrict drv, unsigned int samplerate )
{
    if (samplerate != drv->i2s_init.I2S_AudioFreq)
    {
        if (samplerate >= I2S_AudioFreq_8k && samplerate <= I2S_AudioFreq_192k)
        {
            drv->i2s_init.I2S_AudioFreq = samplerate;

            I2S_Init( drv->i2sx, &drv->i2s_init );
        }
    }
    return samplerate;
}

/**
 * @brief Set I2S sample size
 *
 * This function changes the audio word length. STM32 supports 16- and 32 bit transfers only, but
 * data can be 16, 24 or 32 bit long. This function switches transfer to 16 bit in case of a 16-bit
 * data length and 32 bit for 24- and 32bit data width.
 *
 * Note: switching this setting requires both, I2S and DMA to stop before re-initializing. Therefore,
 * any existing data is truncated!
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 * @param wwidth    Requested word width (note: 20 bit not supported by hardware)
 *
 * @return false on error (not supported value), true otherwise
 */

bool i2sm_set_samplesize( i2sm_t * restrict drv, uint8_t samplesize, uint8_t channels )
{
    bool retval = true;

    drv->dma_nextbuf = 0;
    drv->dma_nextbuf_size = 0;

    DMA_DeInit( drv->dma_stream );
    SPI_I2S_DeInit( drv->i2sx );
    switch( samplesize )
    {
    case 16 :
        drv->i2s_init.I2S_DataFormat = I2S_DataFormat_16b;
        drv->dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        break;
    case 24 :
        drv->i2s_init.I2S_DataFormat = I2S_DataFormat_24b;
        drv->dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
        break;
    case 32 :
        drv->i2s_init.I2S_DataFormat = I2S_DataFormat_32b;
        drv->dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
        break;
    default :
        retval = false;
    }

    I2S_Init( drv->i2sx, &drv->i2s_init );
    DMA_Init( drv->dma_stream, &drv->dma_init );
    SPI_I2S_DMACmd( drv->i2sx, (drv->dma_init.DMA_DIR == DMA_DIR_PeripheralDST) ? SPI_I2S_DMAReq_Tx : SPI_I2S_DMAReq_Rx, ENABLE );
    drv->running = false;
    return retval;
}

/**
 * @brief Start the I2S transmitter
 *
 * This functions (re-)starts the I2S transmission. It is safe to call if there's a transmission underway.
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 *
 * @return Nothing
 */

void i2sm_tx_start( i2sm_t * restrict drv )
{
    if ((drv->running == false) && (drv->dma_nextbuf != 0))
    {
        switch_to_next_buf( drv );
        DMA_ITConfig( drv->dma_stream, DMA_IT_TC, ENABLE );
        I2S_Cmd( drv->i2sx, ENABLE );
        DMA_Cmd( drv->dma_stream, ENABLE );
        drv->running = true;
    }
}

/**
 * @brief Start the I2S receiver
 *
 * This functions (re-)starts the I2S receiver. It is safe to call if there's a transmission underway.
 *
 * @param drv   Pointer to device driver context as returned by i2sm_open()
 *
 * @return Nothing
 */

void i2sm_rx_start( i2sm_t * restrict drv )
{
    if ( drv->dma_nextbuf && DMA_GetCurrDataCounter( drv->dma_stream ) == 0 )
    {
        switch_to_next_buf( drv );
        DMA_ITConfig( drv->dma_stream, DMA_IT_TC, ENABLE );
        DMA_Cmd( drv->dma_stream, ENABLE );
        I2S_Cmd( drv->i2sx, ENABLE );
    }
}

#endif

