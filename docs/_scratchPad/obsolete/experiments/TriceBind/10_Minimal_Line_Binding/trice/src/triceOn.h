// SPDX-License-Identifier: MIT
// Reduced PoC extract based on the dispatcher idea in rokath/trice/src/triceOn.h.
#ifndef TRICE_ON_H_
#define TRICE_ON_H_

#define TRICE_BIND_JOIN_I(a, b) a##b
#define TRICE_BIND_JOIN(a, b) TRICE_BIND_JOIN_I(a, b)
#define TRICE_BIND_ID_AT_LINE(line) TRICE_BIND_JOIN(TRICE_ID_L, line)
#define TRICE_BIND_ID_HERE() TRICE_BIND_ID_AT_LINE(__LINE__)

#if TRICE_BIND
// The user writes trice("..."). The generated sidecar supplies TRICE_ID_L<line>.
// The format argument is intentionally not passed to target code.
#define trice(format) trice32fn_0((uint16_t)iD(TRICE_BIND_ID_HERE()))
#else
// Existing explicit-ID spelling for comparison and compatibility.
#define trice(tid, format) trice32fn_0((uint16_t)(tid))
#endif

#endif
