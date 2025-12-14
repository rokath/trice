/*! \file triceVariadic.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_VARIADIC_H
#define TRICE_VARIADIC_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The variadic macros need to be defined before `#include "triceOff.h"`.
// When TRICE OFF == 1, the  variadic macros cannot simply be defined as empty macros, because then unused parameter warnings pop up.
//

//! TRICE_NTH_ARGUMENT just evaluates to the 15th argument. It is extendable until a 32764 bytes payload.
//! Variadic macros (https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms)
//! See for more explanation https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/preprocessor/macros/__VA_ARGS__/count-arguments
#define TRICE_NTH_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, ...) a14

//! TRICE_COUNT_ARGUMENTS builds upon TRICE_NTH_ARGUMENT. The more arguments that are passed to TRICE_COUNT_ARGUMENTS,
//! the more the »counting arguments« (12, 11, 10, 9, 8, 7…) are pushed to the right.
//! Thus the macro evaluates to the number of arguments that are passed to the macro.
//! If you set the C language to strict C (C90, C99, C11 or C17) the `##` operator doesn't remove the comma before it when `__VA_ARGS__` expands to nothing.
//! In this case, the TRICE macro doesn't work with no parameters. You must then explicitly use TRICE0 instead of TRICE for a no parameter value TRICE.
//! For more details see closed Issue #279. Special thanks to [@escherstair](https://github.com/escherstair).
//! If for example using CLANG 6.18 set C-language to gnu11, gnu99 or std to avoid the comma issue when no parameters are in a TRICE  macro.
//! In case you have to set the C-Language to c11 or c99 you can use the TRICE0 macro directly instead of TRICE when no value parameters.
#define TRICE_COUNT_ARGUMENTS(...) TRICE_NTH_ARGUMENT(dummy, ##__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

//! TRICE_CONCAT concatenates the 2 arguments a and b (helper macro).
#define TRICE_CONCAT(a, b) a##b

//! TRICE_CONCAT2 concatenates the 2 arguments a and b (helper macro).
#define TRICE_CONCAT2(a, b) TRICE_CONCAT(a, b)

#endif
