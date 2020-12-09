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

#ifndef _AUDIO_H
#define _AUDIO_H

#ifdef  __cplusplus
extern "C" {
#endif

/** @file
 * Interface to audio controller services.
 */

#include <stdint.h>
#include <stdbool.h>


/**
 * @name Error codes
 */

/** @{ */

#define AUDIO_ERR_OK           0       /**<  No error */
#define AUDIO_ERR_INVALID      -1      /**<  Could not get valid value */
/** @} */


typedef struct ctxt_audio_s audio_t;

extern audio_t *audio_open(unsigned int id);

extern int audio_set_format( audio_t * srv, uint32_t samplerate, uint8_t samplesize, uint8_t channels );
extern uint32_t audio_get_samplerate( audio_t * srv );
extern uint8_t audio_get_channels( audio_t * srv );
extern uint8_t audio_get_samplesize( audio_t * srv );
extern size_t audio_get_max_bufsize( audio_t * srv );

extern bool audio_set_callback( audio_t * srv, void (* callback)( void *), void * context);

extern bool audio_tx_accept( audio_t * srv );
extern bool audio_tx_inuse( audio_t * srv, const void* buf );
extern size_t audio_tx_write( audio_t * srv, const void* buf, size_t samples );
extern void audio_tx_start( audio_t * srv );
extern void audio_tx_stop( audio_t * srv );
extern void audio_tx_set_volume( audio_t * srv, uint16_t volume );
extern void audio_tx_set_mute( audio_t * srv, bool mute );
extern void audio_tx_flush( audio_t * srv );
extern size_t audio_tx_pending( audio_t * srv );

extern bool audio_rx_avail( audio_t * srv );
extern bool audio_rx_inuse( audio_t * srv, const void * buf );
extern size_t audio_rx_read( audio_t * srv, void * buf, size_t buflen );
extern void audio_rx_start( audio_t * srv );
extern void audio_rx_stop( audio_t * srv );
extern void audio_rx_set_level( audio_t * srv, uint16_t level );
extern void audio_rx_flush( audio_t * srv );
extern size_t audio_rx_pending( audio_t * srv );

#ifdef  __cplusplus
}
#endif

#endif /* _AUDIO_H */
