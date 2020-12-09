/*
 *  Software Platform Generated File
 *  --------------------------------
 */

void framework_init(void) 
{
    extern void pal_init(void);
    pal_init();

    extern void cs4322_to_audio_init(void);
    cs4322_to_audio_init();

    extern void i2sm_to_audio_init(void);
    i2sm_to_audio_init();
}
