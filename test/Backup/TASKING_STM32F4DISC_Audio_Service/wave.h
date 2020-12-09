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

#ifndef _WAVE_H
#define _WAVE_H

#include <stdint.h>
#include <stdbool.h>

#define WAVE_FORMAT_PCM         0x0001      // File format is pulse code modulation
#define WAVE_FORMAT_IBM_MULAW   0x0101      // File format is IBM mu-law
#define WAVE_FORMAT_IBM_ALAW    0x0102      // File format is IBM a-law
#define WAVE_FORMAT_IBM_ADPCM   0x0103      // File format is IBM adaptive differential PCM

typedef struct
{
    uint16_t    format;                     // One of the file formats described above
    uint16_t    channels;                   // 1 = mono, 2 = stereo
    uint16_t    samplerate;                 // in Hz
    uint16_t    samplesize;                 // 8 or 16 bits per sample
    unsigned    playtime;                   // Play time in seconds
} wav_header_t;

extern void wav_playfile( audio_t * audio, wav_header_t info, uintptr_t bufstart, uintptr_t bufend );
extern wav_header_t wav_parseheader( uintptr_t bufstart, uintptr_t bufend );
extern bool wav_testsupported( wav_header_t header );

#endif // _WAVE_H
