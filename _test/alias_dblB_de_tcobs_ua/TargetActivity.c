//! \file TargetActivity.c
//! \brief This file gets compiled into the cgot package only because it exists in the cgot folder.
//! \details Go func triceLogSpecialTest parses additionally this file to collect the expected results line by line for later test execution.




#include "trice.h"

// The strings behind "//exp:" are the expected result for each line (-color=off)
// Lines ending with a comment starting with `//exp:` are executed during the automatic Go tests.
// clang-format off

void TriceCheckSpecial( int n ) {

        switch(n){
                default:

                break; case __LINE__: TRice("Hello World!\n" );                                            //exp: Time:feed3322default: Hello World!
                // 'printi' is declared as -alias  in ./trice_environment.sh and defined here in triceConfig.h which is excluded from parsing in ./trice_environment.sh.
                // 'prints' is declared as -salias in ./trice_environment.sh and defined here in triceConfig.h which is excluded from parsing in ./trice_environment.sh.
                break; case __LINE__: prints("att:'-alias' & '-salias' contributor is %s.\n", "@srgg" );  //exp: Time:        default: att:'-alias' & '-salias' contributor is @srgg.
                break; case __LINE__: printi("A user printi call without values.\n");                     //exp: Time:        default: A user printi call without values.
                break; case __LINE__: prints("A user prints call without values.\n");                     //exp: Time:        default: A user prints call without values.
                break; case __LINE__: printi("A user printi call with integer and float values: %u, %d, %x, %2.1f\n", 42, -8, 0xc0de, aFloat(-42.0) );                 //exp: Time:        default: A user printi call with integer and float values: 42, -8, c0de, -42.0
                break; case __LINE__: prints("A user prints call with integer and float values: %u, %d, %x, %2.1f\n", 42, -8, 0xc0de, -42.0);                          //exp: Time:        default: A user prints call with integer and float values: 42, -8, c0de, -42.0
                break; case __LINE__: prints("A user ... with string, integer and float values: %u, %d, %x, %2.1f, %s and %s\n", 42, -8, 0xc0de, -42.0, "up", "down"); //exp: Time:        default: A user ... with string, integer and float values: 42, -8, c0de, -42.0, up and down

        }
}

// clang-format on
