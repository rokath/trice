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
                break; case __LINE__: TRice("Hello World!\n" );                              //exp: Time:feed3322default: Hello World!
                break; case __LINE__: print("A user print call without values.");            //exp: Time:        default: A user print call without values.
                break; case __LINE__: print("A user print call with integer and float values: %u, %d, %x, %2.1f", 42, -8, 0xc0de, -42.0);                          //exp: Time:        default: A user print call with integer and float values: 42, -8, c0de, -42.0
                break; case __LINE__: print("A user... with string, integer and float values: %u, %d, %x, %2.1f, %s and %s", 42, -8, 0xc0de, -42.0, "up", "down"); //exp: Time:        default: A user... with string, integer and float values: 42, -8, c0de, -42.0, up and down
                break; case __LINE__: Trice("ok!\n" );                                       //exp: Time:    be16default: ok!
        }
}

// clang-format on
