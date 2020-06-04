/*
 *  Software Platform Generated File
 *  --------------------------------
 */

#ifndef _SWP_AD_I2SM_TO_AUDIO_CFG_INSTANCE_H
#define _SWP_AD_I2SM_TO_AUDIO_CFG_INSTANCE_H

#include "ad_i2sm_to_audio_cfg.h"

#include "drv_stm32_i2sm_cfg_instance.h"


// instance data
typedef struct
{
    int ad_audiostream;
    int drv_i2sm;
} ad_i2sm_to_audio_cfg_instance_t;

extern const ad_i2sm_to_audio_cfg_instance_t ad_i2sm_to_audio_instance_table[]; 

#endif /* _SWP_AD_I2SM_TO_AUDIO_CFG_INSTANCE_H */
