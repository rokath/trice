// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceVariadic.h
//! \brief trice Variadic declarations and macros.

#ifndef TRICE_VARIADIC_H
#define TRICE_VARIADIC_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The variadic macros need to be defined before `#include "triceOff.h"`.
// When TRICE OFF == 1, the  variadic macros cannot simply be defined as empty macros, because then unused parameter warnings pop up.
//

//! TRICE_NTH_ARGUMENT just evaluates to the 14th argument. It is extendable until a 32764 bytes payload.
//! Variadic macros (https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms)
//! See for more explanation https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/preprocessor/macros/__VA_ARGS__/count-arguments
#define TRICE_NTH_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, ...) a14

//! TRICE_COUNT_ARGUMENTS builds upon TRICE_NTH_ARGUMENT and counts 1 to 13 arguments.
//! Its callers must pass at least one argument. The trailing sentinel keeps
//! TRICE_NTH_ARGUMENT's variadic part non-empty in strict C99 and later modes.
#define TRICE_COUNT_ARGUMENTS(...) TRICE_NTH_ARGUMENT(__VA_ARGS__, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

//! TRICE_COUNT_VALUE_ARGUMENTS counts the optional values following a mandatory format argument.
//! Public convenience macros pass their format string as the first variadic argument, so the
//! argument list is never empty and no compiler-specific comma swallowing extension is needed.
#define TRICE_COUNT_VALUE_ARGUMENTS(...) TRICE_NTH_ARGUMENT(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0)

//! TRICE_CONCAT concatenates the 2 arguments a and b (helper macro).
#define TRICE_CONCAT(a, b) a##b

//! TRICE_CONCAT2 concatenates the 2 arguments a and b (helper macro).
#define TRICE_CONCAT2(a, b) TRICE_CONCAT(a, b)

#endif
