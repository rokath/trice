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

#if defined(STM32F2XX) || defined(STM32F4XX)

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#include <interrupts.h>
#ifdef STM32F2XX
#include <per_stm32f2xx_i2s_cfg_instance.h>
#endif
#ifdef STM32F4XX
#include <per_stm32f4xx_i2s_cfg_instance.h>
#endif
#include <drv_stm32_i2sm_cfg_instance.h>
#include <drv_i2sm.h>

#include <stm32_chip_config.h>


static i2sm_t drv_stm32_i2sm_table[DRV_STM32_I2SM_INSTANCE_COUNT];


static void pll_coeff(int fin, int fout, int *N, int *R)
{
    int tmp, tmp_div, tmp_err;
    int best_n = 0, best_r = 0, best_err = INT_MAX;

    for (int tmp_n = 192; tmp_n <= 432; tmp_n++)
    {
        tmp = fin * tmp_n / fout;
        for (int tmp_r = 2; tmp_r <= 7; tmp_r++)
        {
            tmp_div = tmp / tmp_r;
            if (tmp_div < 4) tmp_div = 4;
            if (tmp_div > 511) tmp_div = 511;
            tmp_err = abs(fout * tmp_r * tmp_div - fin * tmp_n);
            if (tmp_err < best_err)
            {
                best_err = tmp_err;
                best_n = tmp_n;
                best_r = tmp_r;
            }
            if (best_err == 0) break;
        }
        if (best_err == 0) break;
    }
    *N = best_n;
    *R = best_r;
}


/**
 * Switch to the next transmit- or receive buffer. Since this action is exactly the same
 * for transmit and receive, it is shared for RX and TX
 */

static void switch_to_next_buf( i2sm_t * restrict drv )
{
    drv->dma_curbuf = drv->dma_nextbuf;
    drv->dma_init.DMA_Memory0BaseAddr = drv->dma_nextbuf;
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

__INTERRUPT_NATIVE void stm32f2xx_i2sm_handler_native( void )
{
    int num = interrupt_get_current();
    i2sm_t * restrict drv = (i2sm_t *) interrupt_native_context( num );

    // acknowledge as soon as possible
    interrupt_acknowledge( num );

    switch( num )
    {
    case DMA1_Stream3_IRQn: // DMA I2S2 RX
        DMA_ClearFlag( drv->dma_stream, DMA_FLAG_TCIF3 );
        break;
    case DMA1_Stream4_IRQn: // DMA I2S2 TX
        DMA_ClearFlag( drv->dma_stream, DMA_FLAG_TCIF4 );
        break;
    case DMA1_Stream0_IRQn: // DMA I2S3 RX
        DMA_ClearFlag( drv->dma_stream, DMA_FLAG_TCIF0 );
        break;
    case DMA1_Stream5_IRQn: // DMA I2S3 TX
        DMA_ClearFlag( drv->dma_stream, DMA_FLAG_TCIF5 );
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

    // Assert for validity check on id should be here
    assert( id < DRV_STM32_I2SM_INSTANCE_COUNT );

    i2sm_t * drv;
    const drv_stm32_i2sm_cfg_instance_t * restrict drv_cfg;
#ifdef STM32F2XX
    const per_stm32f2xx_i2s_cfg_instance_t * restrict per_cfg;
    drv_cfg = &drv_stm32_i2sm_instance_table[id];
    per_cfg = &per_stm32f2xx_i2s_instance_table[drv_cfg->per_stm32_i2s];
    drv = &drv_stm32_i2sm_table[id];
#endif
#ifdef STM32F4XX
    const per_stm32f4xx_i2s_cfg_instance_t * restrict per_cfg;
    drv_cfg = &drv_stm32_i2sm_instance_table[id];
    per_cfg = &per_stm32f4xx_i2s_instance_table[drv_cfg->per_stm32_i2s];
    drv = &drv_stm32_i2sm_table[id];
#endif

    if ( drv->i2sx == NULL )
    {
        I2S_StructInit( &drv->i2s_init );
        drv->i2s_init.I2S_Mode = drv_cfg->i2s_mode;
        drv->i2s_init.I2S_Standard = drv_cfg->i2s_standard;
        drv->i2s_init.I2S_DataFormat = drv_cfg->i2s_dataformat;
        drv->i2s_init.I2S_AudioFreq = drv_cfg->i2s_audiofreq;
        drv->i2s_init.I2S_CPOL = drv_cfg->i2s_cpol;

        drv->dma_stream = (drv_cfg->i2s_mode == I2S_Mode_MasterTx) ? per_cfg->dma_stream_tx : per_cfg->dma_stream_rx;

        drv->dma_init.DMA_Memory0BaseAddr = 0;
        drv->dma_init.DMA_DIR = (drv_cfg->i2s_mode == I2S_Mode_MasterTx) ? DMA_DIR_MemoryToPeripheral : DMA_DIR_PeripheralToMemory;
        drv->dma_init.DMA_BufferSize = 0xFFFE;
        drv->dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        drv->dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
        drv->dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        drv->dma_init.DMA_MemoryDataSize = (drv_cfg->i2s_dataformat == I2S_DataFormat_16b) ?  DMA_MemoryDataSize_HalfWord : DMA_MemoryDataSize_Word;    // DMA_MemoryDataSize_Byte;
        drv->dma_init.DMA_Mode = DMA_Mode_Normal;
        drv->dma_init.DMA_Priority = drv_cfg->dma_priority;
        drv->dma_init.DMA_FIFOMode = DMA_FIFOMode_Enable;
        drv->dma_init.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
        drv->dma_init.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        drv->dma_init.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

        drv->dma_nextbuf = 0;
        drv->dma_nextbuf_size = 0xFFFE;


        switch(per_cfg->baseaddress)
        {
        case SPI2_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
            drv->i2sx = SPI2;
#if defined(PINCFG_I2S2_MCK)
            drv->i2s_init.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
#else
            drv->i2s_init.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
#endif
            break;

        case SPI3_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
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
        DMA_DeInit( drv->dma_stream );

        I2S_Init( drv->i2sx, &drv->i2s_init );
        RCC_PLLI2SCmd(ENABLE);
        drv->dma_init.DMA_Channel = DMA_Channel_0; // All possible streams use channel 0
        drv->dma_init.DMA_PeripheralBaseAddr = (uint32_t) &drv->i2sx->DR;
        DMA_Init( drv->dma_stream, &drv->dma_init );
        SPI_I2S_DMACmd( drv->i2sx, (drv->dma_init.DMA_DIR == DMA_DIR_MemoryToPeripheral) ? SPI_I2S_DMAReq_Tx : SPI_I2S_DMAReq_Rx, ENABLE );
        drv->running = false;
    }

    if (drv->dma_stream == DMA1_Stream3)
    { irq = DMA1_Stream3_IRQn; }
    else if (drv->dma_stream == DMA1_Stream4)
    { irq = DMA1_Stream4_IRQn; }
    else if (drv->dma_stream == DMA1_Stream0)
    { irq = DMA1_Stream0_IRQn; }
    else if (drv->dma_stream == DMA1_Stream5)
    { irq = DMA1_Stream5_IRQn; }
    else
    {
        // invalid DMA stream
        assert(0);
        return NULL;
    }

    // Install the rx or tx DMA interrupt handler
    interrupt_register_native(irq, (void*) drv, stm32f2xx_i2sm_handler_native);
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
    int plln, pllr, pllm, i2s_clk;

    if (samplerate != drv->i2s_init.I2S_AudioFreq)
    {
        if (samplerate >= I2S_AudioFreq_8k && samplerate <= I2S_AudioFreq_192k)
        {
            drv->i2s_init.I2S_AudioFreq = samplerate;

            pllm = (int)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM);
            if (drv->i2s_init.I2S_MCLKOutput == I2S_MCLKOutput_Enable)
            {
                i2s_clk = samplerate * 256;
            }
            else if (drv->i2s_init.I2S_DataFormat == I2S_DataFormat_16b)
            {
                i2s_clk = samplerate * 32;
            }
            else
            {
                i2s_clk = samplerate * 32 * 2;
            }

            pll_coeff( HSE_VALUE / pllm, i2s_clk, &plln, &pllr);
            RCC_PLLI2SCmd(DISABLE);
#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
            RCC_PLLI2SConfig(plln, 2, pllr);
#else
            RCC_PLLI2SConfig(plln, pllr);
#endif
            RCC_PLLI2SCmd(ENABLE);
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
    SPI_I2S_DMACmd( drv->i2sx, (drv->dma_init.DMA_DIR == DMA_DIR_MemoryToPeripheral) ? SPI_I2S_DMAReq_Tx : SPI_I2S_DMAReq_Rx, ENABLE );
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

