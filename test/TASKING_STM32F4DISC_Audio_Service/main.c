/* ------------------------------------------------------------
**
**  Copyright (c) 2014-2015 Altium Limited
**
**  This software is the proprietary, copyrighted property of
**  Altium Ltd. All Right Reserved.
**
**  SVN revision information:
**  $Rev: 14907 $:
**  $Date: 2015-01-19 13:30:51 +0100 (Mon, 19 Jan 2015) $:
**
**  Shows how to use the generic Audio driver
**
** ------------------------------------------------------------
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Application Stack interface
#include "swplatform.h"
#include "wave.h"

// Audio file 'sound.wav' is linked at compiletime:
extern uint8_t _lc_ub_sound_wav[];
extern uint8_t _lc_ue_sound_wav[];


void main( void )
{
    wav_header_t info;
    audio_t * audio;

    // Say hello to the user
    puts( "Audio service example, " __FILE__ " compiled " __DATE__ ", " __TIME__ );

    gpio_t *gpio = gpio_open(DRV_STM32_GPIO_1);
    gpio_set_bits( gpio, 1 << 4 );

    audio  = audio_open(AUDIO_1);
    if ( audio == NULL )
    {
        puts( "Failed to initialize audio service" );
        abort();
    }
    puts( "Successfully initialized audio service\n" );

    info = wav_parseheader( (uintptr_t) _lc_ub_sound_wav, (uintptr_t) _lc_ue_sound_wav );
    if (wav_testsupported( info ))
    {
        printf( "Play 'sound.wav'\n-> " );
        audio_tx_start(audio);

        for (;;)
        {
            wav_playfile( audio, info, (uintptr_t) _lc_ub_sound_wav, (uintptr_t) _lc_ue_sound_wav );
        }
    }
    else
    {
        puts( "Invalid wav file\n" );
    }
}

