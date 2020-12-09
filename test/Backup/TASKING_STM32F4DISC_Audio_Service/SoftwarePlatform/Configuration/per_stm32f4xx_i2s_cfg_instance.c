/*
 *  Software Platform Generated File
 *  --------------------------------
 */

#include "per_stm32f4xx_i2s_cfg_instance.h"

extern void pincfg_i2s3_init(void);


const per_stm32f4xx_i2s_cfg_instance_t per_stm32f4xx_i2s_instance_table[1] = 
{
    {
        1,
            0x40003C00,
            pincfg_i2s3_init,
            51,
            PER_STM32F4XX_I2S_INSTANCE_DMA_STREAM_RX_DMA1_STREAM0,
            PER_STM32F4XX_I2S_INSTANCE_DMA_STREAM_TX_DMA1_STREAM5,
    },

};
