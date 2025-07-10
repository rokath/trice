//! \file TargetActivity.c
//! \brief This file gets compiled into the cgot package only because it exists in the cgot folder.
//! \details Go func triceLogSpecialTest parses additionally this file to collect the expected results line by line for later test execution.

#include "trice.h"

// The strings behind "//exp:" are the expected result for each line (-color=off)
// Lines ending with a comment starting with `//exp:` are executed during the automatic Go tests.
// clang-format off

void TriceCheckSpecial( int n ) {

  // The custom macros cause compiler warnings, when the IDs are still inserted. Therefore a 'trice clean' is needed before building the binary.

  /* Some Custom Trice Alias Examples */  
  const int theRightAnswer = 42;
  const int theFastFoundAnswer = 24;
  const char* theQuestion = "What could be the answer to the Ultimate Question of Life, the Universe, and Everything?";

  switch(n){
    default:
    break; case __LINE__: TRice("Hello World!\n" ); //exp: Time:feed3322default: Hello World!
    
    // Some Trice custom alias examples
    break; case __LINE__: CUSTOM_PRINT("CUSTOM_PRINT example: the right answer is: %d\n", theRightAnswer); //exp: Time:        default: CUSTOM_PRINT example: the right answer is: 42
      
    // Assert with condition 
    break; case __LINE__: CUSTOM_ASSERT(theFastFoundAnswer == theRightAnswer); //exp: Time:        default: [ASSERT] TargetActivity.c:28: theFastFoundAnswer == theRightAnswer
      
    // Assert with condition and a message: This works too, but triggers a clang compiler warning, we cannot suppress. (https://stackoverflow.com/questions/52692564/how-can-i-disable-format-security-error-with-clang)
    break; case __LINE__: CUSTOM_ASSERT(theFastFoundAnswer == theRightAnswer, (char*)theQuestion ); //exp: Time:        default: [ASSERT] TargetActivity.c:31: theFastFoundAnswer == theRightAnswer -> What could be the answer to the Ultimate Question of Life, the Universe, and Everything?
      
    // Assert with condition and a message and some extra message arguments
    break; case __LINE__: CUSTOM_ASSERT(theFastFoundAnswer == theRightAnswer, (char*)"'%s' Am, it is %d", (char*)theQuestion, theRightAnswer); //exp: Time:        default: [ASSERT] TargetActivity.c:34: theFastFoundAnswer == theRightAnswer -> 'What could be the answer to the Ultimate Question of Life, the Universe, and Everything?' Am, it is 42
  }

}

// clang-format on
