/*! \file triceCodeSizeOptimization.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

// Reduce trice code size by selectively setting 0 to unused functions.
// This maybe needs not to be done with specific linker optimization: 
//
// Some compiler support so called "linker feedback"
// by compiling twice to get rid of unused functions.
// Check your compiler for the "linker feedback" option.
//
// In ARM uVision, when using ARMCC compiler v5, there is a 
// check box Project -> Options -> Target -> "Cross Module Optimization".
// This increases the build time but reduces the image size significantly.
// When using the "linker feedback" option or program memory is not full
// there is no need for changes here and simply let all values be 1.
// In ARMCC this works also with the lite version.
//
// In ARM uVision, when using ARMCLANG compiler v6, 
// the "Cross Module Optimization" is not visible.
// Instead check box Project -> Options -> C/C++(AC6) -> "Link-Time Optimization"
// is usable to set the CLI -flto switch. LTO is not possible with ARMCLANG6 lite:
// https://developer.arm.com/documentation/ka004054/latest.
//
// With GCC probably simply the -flto CLI switch is needed.

#ifndef TRICE_CODE_SIZE_OPTIMIZATION_H_
#define TRICE_CODE_SIZE_OPTIMIZATION_H_

// without stamp 8-bit values functions
#define ENABLE_trice8fn_1  1
#define ENABLE_trice8fn_2  1
#define ENABLE_trice8fn_3  1
#define ENABLE_trice8fn_4  1
#define ENABLE_trice8fn_5  1
#define ENABLE_trice8fn_6  1
#define ENABLE_trice8fn_7  1
#define ENABLE_trice8fn_8  1
#define ENABLE_trice8fn_9  1
#define ENABLE_trice8fn_10 1
#define ENABLE_trice8fn_11 1
#define ENABLE_trice8fn_12 1

// with 16-bit stamp 8-bit values functions
#define ENABLE_Trice8fn_1  1
#define ENABLE_Trice8fn_2  1
#define ENABLE_Trice8fn_3  1
#define ENABLE_Trice8fn_4  1
#define ENABLE_Trice8fn_5  1
#define ENABLE_Trice8fn_6  1
#define ENABLE_Trice8fn_7  1
#define ENABLE_Trice8fn_8  1
#define ENABLE_Trice8fn_9  1
#define ENABLE_Trice8fn_10 1
#define ENABLE_Trice8fn_11 1
#define ENABLE_Trice8fn_12 1

// with 32-bit stamp 8-bit values functions
#define ENABLE_TRice8fn_1  1
#define ENABLE_TRice8fn_2  1
#define ENABLE_TRice8fn_3  1
#define ENABLE_TRice8fn_4  1
#define ENABLE_TRice8fn_5  1
#define ENABLE_TRice8fn_6  1
#define ENABLE_TRice8fn_7  1
#define ENABLE_TRice8fn_8  1
#define ENABLE_TRice8fn_9  1
#define ENABLE_TRice8fn_10 1
#define ENABLE_TRice8fn_11 1
#define ENABLE_TRice8fn_12 1

// without stamp 16-bit values functions
#define ENABLE_trice16fn_1  1
#define ENABLE_trice16fn_2  1
#define ENABLE_trice16fn_3  1
#define ENABLE_trice16fn_4  1
#define ENABLE_trice16fn_5  1
#define ENABLE_trice16fn_6  1
#define ENABLE_trice16fn_7  1
#define ENABLE_trice16fn_8  1
#define ENABLE_trice16fn_9  1
#define ENABLE_trice16fn_10 1
#define ENABLE_trice16fn_11 1
#define ENABLE_trice16fn_12 1

// with 16-bit stamp 16-bit values functions
#define ENABLE_Trice16fn_1  1
#define ENABLE_Trice16fn_2  1
#define ENABLE_Trice16fn_3  1
#define ENABLE_Trice16fn_4  1
#define ENABLE_Trice16fn_5  1
#define ENABLE_Trice16fn_6  1
#define ENABLE_Trice16fn_7  1
#define ENABLE_Trice16fn_8  1
#define ENABLE_Trice16fn_9  1
#define ENABLE_Trice16fn_10 1
#define ENABLE_Trice16fn_11 1
#define ENABLE_Trice16fn_12 1

// with 32-bit stamp 16-bit values functions
#define ENABLE_TRice16fn_1  1
#define ENABLE_TRice16fn_2  1
#define ENABLE_TRice16fn_3  1
#define ENABLE_TRice16fn_4  1
#define ENABLE_TRice16fn_5  1
#define ENABLE_TRice16fn_6  1
#define ENABLE_TRice16fn_7  1
#define ENABLE_TRice16fn_8  1
#define ENABLE_TRice16fn_9  1
#define ENABLE_TRice16fn_10 1
#define ENABLE_TRice16fn_11 1
#define ENABLE_TRice16fn_12 1

// without stamp 32-bit values functions
#define ENABLE_trice32fn_1  1
#define ENABLE_trice32fn_2  1
#define ENABLE_trice32fn_3  1
#define ENABLE_trice32fn_4  1
#define ENABLE_trice32fn_5  1
#define ENABLE_trice32fn_6  1
#define ENABLE_trice32fn_7  1
#define ENABLE_trice32fn_8  1
#define ENABLE_trice32fn_9  1
#define ENABLE_trice32fn_10 1
#define ENABLE_trice32fn_11 1
#define ENABLE_trice32fn_12 1

// with 16-bit stamp 32-bit values functions
#define ENABLE_Trice32fn_1  1
#define ENABLE_Trice32fn_2  1
#define ENABLE_Trice32fn_3  1
#define ENABLE_Trice32fn_4  1
#define ENABLE_Trice32fn_5  1
#define ENABLE_Trice32fn_6  1
#define ENABLE_Trice32fn_7  1
#define ENABLE_Trice32fn_8  1
#define ENABLE_Trice32fn_9  1
#define ENABLE_Trice32fn_10 1
#define ENABLE_Trice32fn_11 1
#define ENABLE_Trice32fn_12 1

// with 32-bit stamp 32-bit values functions
#define ENABLE_TRice32fn_1  1
#define ENABLE_TRice32fn_2  1
#define ENABLE_TRice32fn_3  1
#define ENABLE_TRice32fn_4  1
#define ENABLE_TRice32fn_5  1
#define ENABLE_TRice32fn_6  1
#define ENABLE_TRice32fn_7  1
#define ENABLE_TRice32fn_8  1
#define ENABLE_TRice32fn_9  1
#define ENABLE_TRice32fn_10 1
#define ENABLE_TRice32fn_11 1
#define ENABLE_TRice32fn_12 1

// without stamp 64-bit values functions
#define ENABLE_trice64fn_1  1
#define ENABLE_trice64fn_2  1
#define ENABLE_trice64fn_3  1
#define ENABLE_trice64fn_4  1
#define ENABLE_trice64fn_5  1
#define ENABLE_trice64fn_6  1
#define ENABLE_trice64fn_7  1
#define ENABLE_trice64fn_8  1
#define ENABLE_trice64fn_9  1
#define ENABLE_trice64fn_10 1
#define ENABLE_trice64fn_11 1
#define ENABLE_trice64fn_12 1

// with 16-bit stamp 64-bit values functions
#define ENABLE_Trice64fn_1  1
#define ENABLE_Trice64fn_2  1
#define ENABLE_Trice64fn_3  1
#define ENABLE_Trice64fn_4  1
#define ENABLE_Trice64fn_5  1
#define ENABLE_Trice64fn_6  1
#define ENABLE_Trice64fn_7  1
#define ENABLE_Trice64fn_8  1
#define ENABLE_Trice64fn_9  1
#define ENABLE_Trice64fn_10 1
#define ENABLE_Trice64fn_11 1
#define ENABLE_Trice64fn_12 1

// with 32-bit stamp 64-bit values functions
#define ENABLE_TRice64fn_1  1
#define ENABLE_TRice64fn_2  1
#define ENABLE_TRice64fn_3  1
#define ENABLE_TRice64fn_4  1
#define ENABLE_TRice64fn_5  1
#define ENABLE_TRice64fn_6  1
#define ENABLE_TRice64fn_7  1
#define ENABLE_TRice64fn_8  1
#define ENABLE_TRice64fn_9  1
#define ENABLE_TRice64fn_10 1
#define ENABLE_TRice64fn_11 1
#define ENABLE_TRice64fn_12 1

#endif /* TRICE_CODE_SIZE_OPTIMIZATION_H_ */
