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

#ifndef _AUDIO_I_H
#define _AUDIO_I_H

#include <stdint.h>
#include <stdbool.h>
#include <timing.h>

#include <audio.h>
#include "audio_cfg.h"
#include "audio_cfg_instance.h"


/* audio codec settings */
typedef struct
{
    void* codec;
    void* (*open) (unsigned int);
    int   (*set_format) ( void *, uint32_t, uint8_t, uint8_t );
    int   (*mute)(void *, bool);
    int   (*playback_volume)(void *, unsigned short);    // playback volume
    int   (*record_level)(void *, unsigned short);     // record level
    void  (*play)  (void *);
    void  (*stop_playing)  (void *);
    void  (*record) (void *);
    void  (*stop_record)  (void *);
    void  (*stop)  (void *);
}codec_t;


typedef struct audiostream_s audiostream_t;

/* datastream settings */
struct audiostream_s
{
    void*   data_dev ;           // Bus device driver pointer
    void*   (*open)(unsigned int);
    int     (*set_format)( audiostream_t *, uint32_t, uint8_t, uint8_t );
    bool    (*tx_accept)(audiostream_t *);
    bool    (*tx_inuse)(void*, const void*);
    void    (*tx_start)(void*);
    void    (*tx_stop)(void*);
    size_t  (*tx_write)(void*, const void*, size_t);
    void    (*tx_flush)(void*);
    bool    (*rx_avail)(void*);
    bool    (*rx_inuse)(void*, const void*);
    void    (*rx_start)(void*);
    void    (*rx_stop)(void*);
    size_t  (*rx_read)(void*, void*, size_t);
    void    (*rx_flush)(void*);
    void    (*set_callback)(void *, void (* callback)( void *), void *);
    size_t  (*tx_pending)(void *);
    size_t  (*rx_pending)(void *);
    bool    (*set_max_buf_size)(audiostream_t *);         // Max buffer size that can be handled by the datastrean device
    size_t  max_buf_size;
};


struct ctxt_audio_s
{
    uint32_t      audiostream_id;
    audiostream_t audiostream;
    uint32_t      codec_id;
    codec_t       codec;                // audio device driver pointer
    uint32_t      samplerate;           // Sample frequency (in samples/sec)
    uint8_t       channels;             // Number of channels (1 or 2)
    uint8_t       samplesize;           // Number of bytes in a single sample (8, 16, 20 or 24)
    bool          playing;
    bool          recording;

#if AUDIO_USE_MUTEX
    pthread_mutex_t mutex;
#endif
};


extern audio_t audio_table[];


#endif  // _AUDIO_I_H
