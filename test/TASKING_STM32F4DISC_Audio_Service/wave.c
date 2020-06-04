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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <audio.h>
#include <util_endian.h>

#include "wave.h"

static uint8_t wavbuf1[2048];
static uint8_t wavbuf2[2048];

#define WAV_RIFF        0x46464952          // "RIFF"
#define WAV_WAVE        0x45564157          // "WAVE"
#define WAV_FMT         0x20746D66          // "fmt "
#define WAV_DATA        0x61746164          // "data"



// fix up endianness and play buffer through the audio service
static void decode_and_play( audio_t * restrict audio, wav_header_t info, void *buffer, unsigned size )
{
    uint8_t * buf8;
    uint16_t * buf16;
    uint32_t * buf32;
    unsigned i;

    if ( info.samplesize == 8 )
    {
        buf8 = buffer;
        for( i = 0; i < size; i++ )
        {
            // Convert sample to signed int
            buf8[i] = (((int)buf8[i]) - 0x80) & 0xFF;
        }
        while( size )
        {
            i = audio_tx_write( audio, buf8, size );
            buf8 += i;
            size -= i;
        }
    }
    else if ( info.samplesize == 16 )
    {
        size /= 2;
        buf16 = (uint16_t*)buffer;
        for( i = 0; i < size; i++ )
        {
            // Convert to native endianess. Do we need to convert from unsigned to signed?
            buf16[i] = little16( buf16[i] );
        }
        while( size )
        {
            while (!audio_tx_accept(audio));
            i = audio_tx_write( audio, buf16, size );
            buf16 += i;
            size -= i;
        }
    }
    else if ( info.samplesize == 32 )
    {
        size /= 4;
        buf32 = (uint32_t*)buffer;
        for( i = 0; i < size; i++ )
        {
            // Convert to native endianess. Do we need to convert from unsigned to signed?
            buf32[i] = little32( buf32[i] );
        }
        while( size )
        {
            i = audio_tx_write( audio, buf32, size );
            buf32 += i;
            size -= i;
        }
    }
}

/**********************************************************************
|*
|*  FUNCTION    : wav_playfile
|*
|*  PARAMETERS  : info = wave file information structure
|*                filename = name of file to be played
|*
|*  RETURNS     : None
|*
|*  DESCRIPTION : Play a wav file using the generic audio service
 */
void wav_playfile( audio_t * audio, wav_header_t info, uintptr_t bufstart, uintptr_t bufend )
{
    uint32_t  chunksize;
    uint32_t  chunktype;
    uint32_t  bufsize;
    uint32_t  minsize;
    uint8_t   *wavbuf = NULL;

    audio_set_format( audio, info.samplerate, (uint8_t)info.samplesize, (uint8_t)info.channels );

    bufsize = bufend - bufstart;
    minsize = 3 * sizeof(uint32_t); // size of header
    if ( bufsize < minsize ) return;

    if ( read_little32from8( (void*)bufstart ) == WAV_RIFF  && read_little32from8( (void*)(bufstart + 8) ) == WAV_WAVE )
    {
        bufstart += minsize;
        bufsize  -= minsize;

        minsize = 2 * sizeof(uint32_t); // size of chunk

        while (bufstart < bufend)
        {
            if ( bufsize < minsize ) break;

            chunktype = read_little32from8( (void*)bufstart );
            chunksize = read_little32from8( (void*)(bufstart + 4) );

            bufstart += minsize;
            bufsize  -= minsize;

            if ( chunktype == WAV_DATA )
            {
                printf( "Playing %d samples...\n", chunksize * 8 / info.samplesize );
                audio_tx_start(audio);
                while( chunksize )
                {
                    uint32_t size = (chunksize < sizeof(wavbuf1)) ? chunksize : sizeof(wavbuf1);

                    wavbuf = (wavbuf == wavbuf1) ? wavbuf2 : wavbuf1;
                    while (audio_tx_inuse(audio, wavbuf));
                    memcpy (wavbuf, (void*) bufstart, size);
                    bufstart += size;
                    bufsize  -= size;

                    decode_and_play (audio, info, wavbuf, size);
                    chunksize -= size;
                }

                while( audio_tx_pending(audio) != 0);
                audio_tx_stop(audio);
                puts( "done" );
            }
            else
            {
                bufstart += chunksize;
                bufsize  -= chunksize;
            }
        }
    }
}

/**********************************************************************
|*
|*  FUNCTION    : wav_parseheader
|*
|*  PARAMETERS  : filename = name of file to be checked
|*
|*  RETURNS     : WAVE information
|*
|*  DESCRIPTION : Get WAVE information of said file
 */
extern wav_header_t wav_parseheader( uintptr_t bufstart, uintptr_t bufend )
{
    wav_header_t wavinfo = { 0 } ;
    uint32_t chunksize;
    uint32_t chunktype;
    uint32_t bufsize;
    uint32_t minsize;

    bufsize = 1 + bufend - bufstart;
    minsize = 3 * sizeof(uint32_t); // // uint32_t hdr[3]

    if ( bufsize < minsize ) return wavinfo;

    if ( read_little32from8( (void*)bufstart ) == WAV_RIFF  && read_little32from8( (void*)(bufstart + 8) ) == WAV_WAVE )
    {
        wavinfo.playtime = read_little32from8( (void*)(bufstart + 4) );

        bufstart += minsize;
        bufsize  -= minsize;

        minsize = 2 * sizeof(uint32_t); // uint32_t chunk[2]

        while (bufstart < bufend)
        {
            if ( bufsize < minsize ) break;

            chunktype = read_little32from8( (void*)bufstart );
            chunksize = read_little32from8( (void*)(bufstart + 4) );

            bufstart += minsize;
            bufsize  -= minsize;

            if ( chunktype == WAV_FMT )
            {
                minsize = 7 * sizeof(uint16_t); // uint16_t fmt[7]
                if ( bufsize < minsize ) break;

                wavinfo.format     = read_little16from8( (void*)(bufstart + 0) ); // fmt[0]
                wavinfo.channels   = read_little16from8( (void*)(bufstart + 2) ); // fmt[1]
                wavinfo.samplerate = (uint16_t)read_little32from8( (void*)(bufstart + 4) ); // fmt[2]
                wavinfo.samplesize = read_little16from8( (void*)(bufstart + 12) ) * 8 / wavinfo.channels; // fmt[6]
                break;
            }
            else
            {
                bufstart += chunksize;
                bufsize  -= chunksize;
            }
        }
    }
    if ( wavinfo.channels && wavinfo.samplerate && wavinfo.samplesize )
    {
        wavinfo.playtime /= wavinfo.channels * wavinfo.samplerate * wavinfo.samplesize / 8;
    }
    else
    {
        wavinfo.playtime = 0;
    }
    return wavinfo;
}

/**********************************************************************
|*
|*  FUNCTION    : wav_testsupported
|*
|*  PARAMETERS  : header = wave file information structure
|*
|*  RETURNS     : False if not supported, true otherwise
|*
|*  DESCRIPTION : Check if this is a file we support (PCM encoded WAV, mono or stereo, 8 or 16 bits per sample only)
 */
bool wav_testsupported( wav_header_t header )
{
    bool supported = true;
    if ( header.format != WAVE_FORMAT_PCM ) supported = false;
    if ( header.channels != 1 && header.channels != 2 ) supported = false;
    if ( header.samplesize != 8 && header.samplesize != 16 ) supported = false;
    return supported;
}


