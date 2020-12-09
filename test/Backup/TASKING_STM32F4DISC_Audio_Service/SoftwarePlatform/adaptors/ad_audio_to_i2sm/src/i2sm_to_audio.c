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
#include <drv_i2sm.h>
#include "ad_i2sm_to_audio_cfg_instance.h"


static bool i2sm_to_audio_tx_accept( audiostream_t* stream )
{
    return i2sm_tx_avail(stream->data_dev);
}


int i2sm_to_audio_set_format( audiostream_t * stream, uint32_t samplerate, uint8_t samplesize, uint8_t channels )
{
    i2sm_set_samplerate((i2sm_t*)stream->data_dev, samplerate);
    i2sm_set_samplesize((i2sm_t*)stream->data_dev, samplesize, channels);
    return 0;
}


bool i2sm_to_audio_rx_inuse( void* drv, const void *buf )
{
    return false;
}


bool i2sm_to_audio_set_max_buf_size(audiostream_t * stream)
{
    stream->max_buf_size = 0xfffe;
    return true;
}


void i2sm_to_audio_init( void )
{
    int id;

    for ( int adaptor_id = 0; adaptor_id < AD_I2SM_TO_AUDIO_INSTANCE_COUNT; adaptor_id++ )
    {
        id = ad_i2sm_to_audio_instance_table[adaptor_id].ad_audiostream;

        audio_table[id].audiostream_id               = ad_i2sm_to_audio_instance_table[adaptor_id].drv_i2sm;
        audio_table[id].audiostream.open             = (void*)i2sm_open;
        audio_table[id].audiostream.set_format       = i2sm_to_audio_set_format;
        audio_table[id].audiostream.tx_accept        = i2sm_to_audio_tx_accept;
        audio_table[id].audiostream.tx_inuse         = (void*)i2sm_tx_inuse;
        audio_table[id].audiostream.tx_start         = (void*)i2sm_tx_start;
        audio_table[id].audiostream.tx_stop          = (void*)i2sm_tx_stop;
        audio_table[id].audiostream.tx_write         = (void*)i2sm_write;
        audio_table[id].audiostream.tx_flush         = (void*)i2sm_tx_flush;
        audio_table[id].audiostream.rx_avail         = (void*)i2sm_rx_avail;
        audio_table[id].audiostream.rx_inuse         = i2sm_to_audio_rx_inuse;
        audio_table[id].audiostream.rx_start         = (void*)i2sm_rx_start;
        audio_table[id].audiostream.rx_stop          = (void*)i2sm_rx_stop;
        audio_table[id].audiostream.rx_read          = (void*)i2sm_read;
        audio_table[id].audiostream.rx_flush         = (void*)i2sm_rx_flush;
        audio_table[id].audiostream.set_callback     = (void*)i2sm_set_callback;
        audio_table[id].audiostream.tx_pending       = (void*)i2sm_tx_pending;
        audio_table[id].audiostream.rx_pending       = (void*)i2sm_rx_pending;
        audio_table[id].audiostream.set_max_buf_size = i2sm_to_audio_set_max_buf_size;

        // calculate max buffer size
    }

}

