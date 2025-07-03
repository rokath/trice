/*! \file triceCheckAliases.c
*******************************************************************************/
#if 0 // !TRICE_OFF - We do not exclude this file globally for TRICE_OFF == 1, to check if TRICE_OFF works well.

#include <float.h>
#include "trice.h"

// The strings behind "//exp:" are the expected result for each line (-color=none)
// Lines ending with a comment starting with `//exp:` are executed during the automatic Go tests.
// clang-format off

void TriceCheckAliases(int index); // Avoid noise with option -Wmissing-prototypes.

//! TriceCheckAliases(n) performs trice code sequence n. This function is also called from Go for tests.
void TriceCheckAliases(int n) {
        switch( n ){ 
                default:

                break; case __LINE__: TRice("Hello World!\n" );                                              //exp: time:feed3322default: Hello World!

                // The following line is what is expected but does not occure: 
                //break; case __LINE__: who ("att:'-alias' & '-salias' contributor is %s.\n", "@srgg" );     //xxx: time:        default: att:'-alias' & '-salias' contributor is @srgg.
                // This is, what happens instead - a better understanding is needed.
                break; case __LINE__: who ("att:'-alias' & '-salias' contributor is %s.\n", "@srgg" );       //exp: time:        default: @srgg

                // 'tell' is declared as -alias in ./trice_environment.sh and defined (here in ./_test/*/triceConfig.h, these files are excluded from parsing in ./trice_environment.sh)
                break; case __LINE__: tell("A user cmd.\n");                                                             //exp: time:        default: A user cmd.
                break; case __LINE__: tell("A user cmd: %d * %d * %d = %u = %.1f\n", 2, -3, -7, 2*-3*-7, aFloat(42.0));  //exp: time:        default: A user cmd: 2 * -3 * -7 = 42 = 42.0
        }
}

#endif
