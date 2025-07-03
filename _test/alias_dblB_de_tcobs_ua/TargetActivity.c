#include "trice.h"

// The strings behind "//exp:" are the expected result for each line (-color=off)
// Lines ending with a comment starting with `//exp:` are executed during the automatic Go tests.
// clang-format off

void TriceCheckSpecial( int n ) {

        switch(n){
                default:

                break; case __LINE__: TRice("Hello World!\n" );                                            //exp: time:feed3322default: Hello World!

                // The following line is what is expected but does not occure: 
                //break; case __LINE__: who ("att:'-alias' & '-salias' contributor is %s.\n", "@srgg" );     //xxx: time:        default: att:'-alias' & '-salias' contributor is @srgg.
                // This is, what happens instead - a better understanding is needed.
                break; case __LINE__: who ("att:'-alias' & '-salias' contributor is %s.\n", "@srgg" );       //exp: time:        default: @srgg

                // 'tell' is declared as -alias in ./trice_environment.sh and defined (here in ./_test/*/triceConfig.h, these files are excluded from parsing in ./trice_environment.sh)
                break; case __LINE__: tell("A user cmd.\n");                                                             //exp: time:        default: A user cmd.
                break; case __LINE__: tell("A user cmd: %d * %d * %d = %u = %.1f\n", 2, -3, -7, 2*-3*-7, aFloat(42.0));  //exp: time:        default: A user cmd: 2 * -3 * -7 = 42 = 42.0

        }
}

// clang-format on
