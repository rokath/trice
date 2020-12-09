/*
 *  Software Platform Generated File
 *  --------------------------------
 */

#ifndef _SWP_AUDIO_CFG_INSTANCE_H
#define _SWP_AUDIO_CFG_INSTANCE_H

#include "audio_cfg.h"

#include <stdint.h>
#include "ad_i2sm_to_audio_cfg_instance.h"
#include "ad_cs4322_to_audio_cfg_instance.h"

#define AUDIO_INSTANCE_CHANNELS_MONO  1
#define AUDIO_INSTANCE_CHANNELS_STEREO  2

#define AUDIO_INSTANCE_SAMPLESIZE_8  8
#define AUDIO_INSTANCE_SAMPLESIZE_16  16
#define AUDIO_INSTANCE_SAMPLESIZE_20  20
#define AUDIO_INSTANCE_SAMPLESIZE_24  24

#define AUDIO_INSTANCE_SAMPLERATE_MIN  22050
#define AUDIO_INSTANCE_SAMPLERATE_MAX  22050

#define AUDIO_INSTANCE_CHANNELS_MONO_USED  0
#define AUDIO_INSTANCE_CHANNELS_STEREO_USED  1
#define AUDIO_INSTANCE_CHANNELS_OPTIONS_USED  1
#define AUDIO_INSTANCE_CHANNELS_MIN  AUDIO_INSTANCE_CHANNELS_STEREO
#define AUDIO_INSTANCE_CHANNELS_MAX  AUDIO_INSTANCE_CHANNELS_STEREO

#define AUDIO_INSTANCE_SAMPLESIZE_8_USED  0
#define AUDIO_INSTANCE_SAMPLESIZE_16_USED  1
#define AUDIO_INSTANCE_SAMPLESIZE_20_USED  0
#define AUDIO_INSTANCE_SAMPLESIZE_24_USED  0
#define AUDIO_INSTANCE_SAMPLESIZE_OPTIONS_USED  1
#define AUDIO_INSTANCE_SAMPLESIZE_MIN  AUDIO_INSTANCE_SAMPLESIZE_16
#define AUDIO_INSTANCE_SAMPLESIZE_MAX  AUDIO_INSTANCE_SAMPLESIZE_16


// instance data
typedef struct
{
        uint32_t samplerate;
        uint8_t channels;
        uint8_t samplesize;
    int ad_audiostream;
    struct { const int *instances; size_t size; } ad_codec;
} audio_cfg_instance_t;

extern const audio_cfg_instance_t audio_instance_table[]; 

#endif /* _SWP_AUDIO_CFG_INSTANCE_H */
