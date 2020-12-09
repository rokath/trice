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
#include <fcntl.h>
#include <errno.h>

#include <stdbool.h>
#include <stdint.h>

#include <assert.h>


#include <audio_i.h>
#include <drv_cs4322.h>
#include "ad_cs4322_to_audio_cfg_instance.h"



int cs4322_to_audio_set_format( void * codec, uint32_t samplefreq, uint8_t samplesize, uint8_t channels )
{
    cs4322_set_format((cs4322_t*)codec, channels);
    return 0;
}



int cs4322_to_audio_mute(  void * codec, bool mute )
{
    if (mute)
    {
        cs4322_stop((cs4322_t*)codec);
    }
    else
    {
        cs4322_play((cs4322_t*)codec);
    }
    return 0;
}



void cs4322_to_audio_init( void )
{
    int id;

    for ( int adaptor_id = 0; adaptor_id < AD_CS4322_TO_AUDIO_INSTANCE_COUNT; adaptor_id++ )
    {
        id = ad_cs4322_to_audio_instance_table[adaptor_id].ad_codec;

        audio_table[id].codec_id              = ad_cs4322_to_audio_instance_table[adaptor_id].drv_cs4322;
        audio_table[id].codec.open            = (void*)cs4322_open;
        audio_table[id].codec.set_format      = cs4322_to_audio_set_format;
        audio_table[id].codec.mute            = cs4322_to_audio_mute;
        audio_table[id].codec.playback_volume = (void*)cs4322_set_volume;
        audio_table[id].codec.play            = (void*)cs4322_play;
        audio_table[id].codec.stop_playing    = (void*)cs4322_stop;
        audio_table[id].codec.record          = (void*)cs4322_record;
        audio_table[id].codec.stop_record     = (void*)cs4322_stop;
        audio_table[id].codec.stop            = (void*)cs4322_stop;
    }
}




