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
#include "trice.h"
#include "SEGGER_RTT.h"

// Audio file 'sound.wav' is linked at compiletime:
extern uint8_t _lc_ub_sound_wav[];
extern uint8_t _lc_ue_sound_wav[];


void main( void )
{
    wav_header_t info;
    audio_t * audio;

    TRICE_RTT_SYNC;
    // Say hello to the user
    //puts( "Audio service example, " __FILE__ " compiled " __DATE__ ", " __TIME__ );
    TRICE_P("Audio service example, %s compiled %s %s" __FILE__, __DATE__, __TIME__ );
    gpio_t *gpio = gpio_open(DRV_STM32_GPIO_1);
    gpio_set_bits( gpio, 1 << 4 );

    audio  = audio_open(AUDIO_1);
    if ( audio == NULL )
    {
        TRICE0( Id(63724), "Failed to initialize audio service" );
        abort();
    }
    TRICE0( Id(60540), "Successfully initialized audio service\n" );

    info = wav_parseheader( (uintptr_t) _lc_ub_sound_wav, (uintptr_t) _lc_ue_sound_wav );
    if (wav_testsupported( info ))
    {
    	TRICE0( Id(29299), "Play 'sound.wav'\n-> " );
        audio_tx_start(audio);
    	/*
        // initialize systick
    	*(uint32_t*)0xE000E010 = 0;          // SysTick->CTRL = 0;   // Disable SysTick
    	*(uint32_t*)0xE000E014 = 0x00FFFFFF; // SysTick->LOAD = 0x00ffffff; // Count down from 999 to 0
    	*(uint32_t*)0xE000E018 = 0;          // SysTick->VAL = 0;    // Clear current value to 0
    	*(uint32_t*)0xE000E010 = 0x5;        // SysTick->CTRL = 0x7; // Enable SysTick, and use processor clock
    										 // no exception
    	*/

        TRICE16_1( Id(51876), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
        TRICE0( Id(44230), "att:TASKING_STM32F4DISC_Audio_Service_RTTD\n" );
        TRICE16_1( Id( 1808), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
        TRICE16_1( Id( 4080), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
        TRICE16_1( Id(13209), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
        TRICE16_1( Id(29575), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
        triceCheckSet(0);

        for (;;)
        {
            wav_playfile( audio, info, (uintptr_t) _lc_ub_sound_wav, (uintptr_t) _lc_ue_sound_wav );
        }
    }
    else
    {
    	TRICE0( Id(34072), "Invalid wav file\n" );
    }
}

