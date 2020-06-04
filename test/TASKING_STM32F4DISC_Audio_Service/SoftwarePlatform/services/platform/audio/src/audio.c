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
**  Service for audio routines.
**
** ------------------------------------------------------------
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <timing.h>
#include <audio.h>
#include <audio_i.h>
#include "audio_cfg_instance.h"


// runtime driver table
audio_t audio_table[AUDIO_INSTANCE_COUNT];


/**
 * @brief    Get max accepted buffersize
 *
 * Get the max size of the buffer to be accepted.
 *
 * @param  srv          Pointer to audio context
 *
 * @return max size of the accepted buffer.
 */

size_t audio_get_max_bufsize( audio_t * srv )
{
    return srv->audiostream.max_buf_size;
}


/**
 * @brief    Set callback function
 *
 * Set the callback function. This function is called duriong interrupts
 * Be sure the time spent in the callback function will be very short.
 * This function returns false if the driver doesn't accept callback functions.
 *
 * Callback function prototype: void callback(void *);
 *
 * @param  srv          Pointer to audio context
 * @param  callback     Pointer to callback function
 * @param  context      The callback function will be called with this context as parameter
 *
 * @return true if callback function is accepted, false otherwise
 */

bool audio_set_callback( audio_t * srv, void (* callback)( void *), void * context)
{
    if (srv->audiostream.set_callback)
    {
        srv->audiostream.set_callback(srv->audiostream.data_dev, callback, context);

        return true;
    }
    return false;
}


/**
 * @brief    See if buffer will be accepted
 *
 * See if the driver will accept another buffer. The size of the buffer should not exceed the maximum buffer size as
 * returned by audio_get_max_bufsize().
 *
 * @param  srv          Pointer to audio context
 *
 * @return true if new buffer can be accepted, false otherwise
 */

bool audio_tx_accept( audio_t * srv )
{
    return srv->audiostream.tx_accept(&srv->audiostream);
}


/**
 * @brief    See if tx buffer is in use
 *
 * See if the given buffer is in use by the driver.
 *
 * @param  srv          Pointer to audio context
 *
 * @return true if buffer is in use, false otherwise
 */

bool audio_tx_inuse( audio_t * srv, const void* buf )
{
    return srv->audiostream.tx_inuse(srv->audiostream.data_dev, buf);
}


/**
 * @brief    Start transmitter
 *
 * Start the transmitter. Transmitter will be enabled till audio_tx_stop is called.
 *
 * @param  srv          Pointer to audio context
 *
 * @return nothing
 */

void audio_tx_start( audio_t * srv )
{
    if (srv->codec.codec) srv->codec.play(srv->codec.codec);
    srv->audiostream.tx_start(srv->audiostream.data_dev);
    srv->playing = true;
}


/**
 * @brief    Flush transmitter
 *
 * Flush the transmitter. This flushes the software buffer(s). Flushing the hardware buffers depends on the hardware.
 *
 * @param  srv          Pointer to audio context
 *
 * @return nothing
 */

void audio_tx_flush( audio_t * srv )
{
    srv->audiostream.tx_flush(srv->audiostream.data_dev);
}


/**
 * @brief    See if receiver buffer is available
 *
 * See if a new buffer with received data is available
 *
 * @param  srv          Pointer to audio context
 *
 * @return true if new buffer is available, false otherwise
 */

bool audio_rx_avail( audio_t * srv )
{
    return srv->audiostream.rx_avail(srv->audiostream.data_dev);
}


/**
 * @brief    See if rx buffer is in use
 *
 * See if the given buffer is in use by the driver.
 *
 * @param  srv          Pointer to audio context
 *
 * @return true if buffer is in use, false otherwise
 */

bool audio_rx_inuse( audio_t * srv, const void * buf )
{
    return srv->audiostream.rx_inuse(srv->audiostream.data_dev, buf);
}


/**
 * @brief    Start receiver
 *
 * Start the receiver. receiver will be enabled till audio_rx_stop is called.
 *
 * @param  srv          Pointer to audio context
 *
 * @return nothing
 */

void audio_rx_start( audio_t * srv )
{
    if (srv->codec.codec) srv->codec.record(srv->codec.codec);
    srv->audiostream.rx_start(srv->audiostream.data_dev);
    srv->recording = true;
}


/**
 * @brief    Set record level
 *
 * Set the record level
 *
 * @param  srv          Pointer to audio context
 * @param level         new record level
 *
 * @return nothing
 */

void audio_rx_set_level( audio_t * srv, uint16_t level )
{
    if (srv->codec.codec) srv->codec.record_level(srv->codec.codec, level);
}


/**
 * @brief    Flush receiver
 *
 * Flush the receiver. This flushes the software buffer(s). Flushing the hardware buffers depends on the hardware.
 *
 * @param  srv          Pointer to audio context
 *
 * @return nothing
 */

void audio_rx_flush( audio_t * srv )
{
    srv->audiostream.rx_flush(srv->audiostream.data_dev);
}



/**
 * @brief    Open a context of the audio service
 *
 * The first time the function is called, it initializes bus and audio device drivers.
 * Further attempts to open an already successfully opened context simply return the same pointer.
 *
 * @param  id  valid context id
 *
 * @return context pointer if successful initialized, NULL otherwise
 */

audio_t *audio_open(const unsigned int id)
{
    // Copy configuration data to driver table
    audio_t *  srv = &audio_table[id];
    const audio_cfg_instance_t * restrict context_cfg = &audio_instance_table[id];

    // If opened before, just return the pointer
    if ( srv->audiostream.data_dev == NULL)
    {
        srv->audiostream.data_dev = srv->audiostream.open(srv->audiostream_id);
        if (srv->audiostream.data_dev == NULL)
        {
            return NULL;
        }
        if (context_cfg->ad_codec.size == 1)
        {
            // codec_id has been set by the adaptor plugin
            srv->codec.codec = srv->codec.open(srv->codec_id);
            if (srv->codec.codec == NULL)
            {
                srv->audiostream.data_dev = NULL;
                return NULL;
            }
        }
        else
        {
            srv->codec.codec = NULL;
        }

        srv->audiostream.set_max_buf_size(&srv->audiostream);
        srv->samplerate = context_cfg->samplerate;
        srv->channels = context_cfg->channels;
        srv->samplesize = context_cfg->samplesize;
        srv->audiostream.set_format(&srv->audiostream, srv->samplerate, srv->samplesize, srv->channels);
        if (srv->codec.codec) srv->codec.set_format(srv->codec.codec, srv->samplerate, srv->samplesize, srv->channels);

        // And we're done!
    }
    return srv;
}


/**
 * @brief    Set audio format
 *
 * Set the audio format on the hardware as well as the driver
 *
 * @param  srv          Pointer to audio context
 * @param  samplerate   Sample rate to be set, in samples per second
 * @param  channels     Number of channels (1 for mono, 2 for stero)
 * @param  samplesize   Size of a single channel's sample, in bits (8, 16, 24 or 32)
 *
 * @return 0 on success, failure otherwise
 */

int audio_set_format( audio_t * restrict srv, uint32_t samplerate, uint8_t samplesize, uint8_t channels )
{
    if ((samplerate != srv->samplerate) || (samplesize != srv->samplesize) || (channels != srv->channels))
    {
        srv->samplerate = samplerate;
        srv->channels = channels;
        srv->samplesize = samplesize;
        if (srv->codec.codec) srv->codec.set_format(srv->codec.codec,  samplerate, samplesize, channels );
        srv->audiostream.set_format(&srv->audiostream, samplerate, samplesize, channels);
    }
    return 0;
}


/**
 * @brief    Sets the volume of the audio
 *
 * @param  srv  pointer to audio context
 * @param  volume volume to set, 0 is softest, 255 is max
 *
 * @return 0 on success, failure otherwise
 */

void audio_tx_set_volume( audio_t * srv, uint16_t volume )
{
    if (srv->codec.codec) srv->codec.playback_volume(srv->codec.codec, volume);
}


/**
 * @brief    Sets the mute
 *
 * @param  srv  pointer to audio context
 * @param  mute 0 - mute is disabled, 1 - mute is enabled
 *
 * @return 0 on success, failure otherwise
 */

void audio_tx_set_mute( audio_t * srv, bool mute )
{
    if (srv->codec.codec) srv->codec.mute(srv->codec.codec, mute);
}



/**
 * @brief   Play sample
 *
 * This functions starts playing data from memory
 *
 * @param   srv     Pointer to context as returned by audio_open
 * @param   buf     Pointer to audio to be played
 * @param   buflen  Number of samples in the buffer to be played
 *
 * @return  number of samples written, < 0 : error
 */

size_t audio_tx_write( audio_t * srv, const void * buf, size_t buflen )
{
    size_t result;

    result = srv->audiostream.tx_write(srv->audiostream.data_dev, buf, buflen);
    if (srv->playing) srv->audiostream.tx_start(srv->audiostream.data_dev);
    return result;
}

/**
 * @brief   Record sample
 *
 * This functions records audio data in memory
 *
 * @param   srv     Pointer to context as returned by audio_open
 * @param   buf     Pointer to audio to be recorded
 * @param   buflen  Maximum number of samples in the buffer to be recorded
 *
 * @return  number of samples read, < 0 : error
 */

size_t audio_rx_read( audio_t * srv, void * buf, unsigned int buflen )
{
    return srv->audiostream.rx_read(srv->audiostream.data_dev, buf, buflen);
//    srv->codec.record(srv->codec.codec);
}


/**
 * @brief   Stop playing
 *
 * This functions stops audio play
 *
 * @param   srv   Pointer to context as returned by audio_open
 *
 * @return  0 on success, failure otherwise
 */

void audio_tx_stop( audio_t * srv )
{
    if (srv->codec.codec) srv->codec.stop_playing(srv->codec.codec);
    srv->audiostream.tx_stop(srv->audiostream.data_dev);
    srv->playing = false;
}

/**
 * @brief   Stop recording
 *
 * This functions stops audio record
 *
 * @param   srv   Pointer to context as returned by audio_open
 *
 * @return  0 on success, failure otherwise
 */

void audio_rx_stop( audio_t * srv )
{
    srv->audiostream.rx_stop(srv->audiostream.data_dev);
    if (srv->codec.codec) srv->codec.stop_record(srv->codec.codec);
    srv->recording = false;
}


/**
 * @brief Get sample rate
 *
 * This function returns the current sample rate used
 *
 * @param srv   Pointer to context as returned by audio_open
 *
 * @return Samplefrequency, in Hz
 */

uint32_t audio_get_samplerate( audio_t * srv )
{
    return srv->samplerate;
}

/**
 * @brief Get number of channels
 *
 * This function returns the current number of channels in use
 *
 * @param srv   Pointer to context as returned by audio_open
 *
 * @return Number of channels
 */

uint8_t audio_get_channels( audio_t * srv )
{
    return srv->channels;
}

/**
 * @brief Get samplesize
 *
 * This function returns the current samplesize, in bits per channel
 *
 * @param srv   Pointer to context as returned by audio_open
 *
 * @return Samplesize
 */

uint8_t audio_get_samplesize( audio_t * srv )
{
    return srv->samplesize;
}



/**
 * @brief    Get transmitter pending samples
 *
 * Get the number of pending samples in the transmitter.
 *
 * @param  srv          Pointer to audio context
 *
 * @return number of pending samples
 */

size_t audio_tx_pending( audio_t * srv)
{
    return srv->audiostream.tx_pending(srv->audiostream.data_dev);
}
