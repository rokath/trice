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

#ifndef DRV_STM32_I2SM_INTERNAL_H
#define DRV_STM32_I2SM_INTERNAL_H

#ifdef STM32F10X
#include <stm32f10x.h>

struct  i2sm_s
{
    int                     interrupt0;
    SPI_TypeDef *           i2sx;
    I2S_InitTypeDef         i2s_init;
    DMA_InitTypeDef         dma_init;
    DMA_Channel_TypeDef *   dma_stream;
    uintptr_t               dma_curbuf;             // Points to buffer currently being sent
    uintptr_t               dma_nextbuf;            // Points to next buffer to be sent. If 0, it's not valid.
    volatile uint16_t       dma_nextbuf_size;       // Size of next buffer (in samples)
    volatile uint32_t       rx_done;                // Bytes already read in previous calls
    volatile uint16_t       rx_current;             // Availability of samples
    void (*callback)(void *);
    void *                  cb_context;
} ;

#endif

#ifdef STM32F2XX
#include <stm32f2xx.h>
#include <stdbool.h>

struct  i2sm_s
{
    int                     interrupt0;
    SPI_TypeDef *           i2sx;
    I2S_InitTypeDef         i2s_init;
    DMA_InitTypeDef         dma_init;
    DMA_Stream_TypeDef *    dma_stream;
    uintptr_t               dma_curbuf;             // Points to buffer currently being sent
    uintptr_t               dma_nextbuf;            // Points to next buffer to be sent. If 0, it's not valid.
    volatile uint16_t       dma_nextbuf_size;       // Size of next buffer (in samples)
    volatile uint32_t       rx_done;                // Bytes already read in previous calls
    volatile uint16_t       rx_current;             // Availability of samples
    volatile bool           running;
    void (*callback)(void *);
    void *                  cb_context;
};

#endif

#ifdef STM32F30X
#include <stm32f30x.h>
#include <stdbool.h>

struct  i2sm_s
{
    int                     interrupt0;
    SPI_TypeDef *           i2sx;
    I2S_InitTypeDef         i2s_init;
    DMA_InitTypeDef         dma_init;
    DMA_Channel_TypeDef *   dma_stream;
    uintptr_t               dma_curbuf;             // Points to buffer currently being sent
    uintptr_t               dma_nextbuf;            // Points to next buffer to be sent. If 0, it's not valid.
    volatile uint16_t       dma_nextbuf_size;       // Size of next buffer (in samples)
    volatile uint32_t       rx_done;                // Bytes already read in previous calls
    volatile uint16_t       rx_current;             // Availability of samples
    volatile bool           running;
    void (*callback)(void *);
    void *                  cb_context;
};

#endif

#ifdef STM32F37X
#include <stm32f37x.h>
#include <stdbool.h>

struct  i2sm_s
{
    int                     interrupt0;
    SPI_TypeDef *           i2sx;
    I2S_InitTypeDef         i2s_init;
    DMA_InitTypeDef         dma_init;
    DMA_Channel_TypeDef *   dma_stream;
    uintptr_t               dma_curbuf;             // Points to buffer currently being sent
    uintptr_t               dma_nextbuf;            // Points to next buffer to be sent. If 0, it's not valid.
    volatile uint16_t       dma_nextbuf_size;       // Size of next buffer (in samples)
    volatile uint32_t       rx_done;                // Bytes already read in previous calls
    volatile uint16_t       rx_current;             // Availability of samples
    volatile bool           running;
    void (*callback)(void *);
    void *                  cb_context;
};

#endif

#ifdef STM32F4XX
#include <stm32f4xx.h>
#include <stdbool.h>

struct  i2sm_s
{
    int                     interrupt0;
    SPI_TypeDef *           i2sx;
    I2S_InitTypeDef         i2s_init;
    DMA_InitTypeDef         dma_init;
    DMA_Stream_TypeDef *    dma_stream;
    uintptr_t               dma_curbuf;             // Points to buffer currently being sent
    uintptr_t               dma_nextbuf;            // Points to next buffer to be sent. If 0, it's not valid.
    volatile uint16_t       dma_nextbuf_size;       // Size of next buffer (in samples)
    volatile uint32_t       rx_done;                // Bytes already read in previous calls
    volatile uint16_t       rx_current;             // Availability of samples
    volatile bool           running;
    void (*callback)(void *);
    void *                  cb_context;
};

#endif


#endif
