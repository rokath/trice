/*! \file tcobsInternal.h
\author Thomas.Hoehenleitner [at] seerose.net
\details See ./TCOBSv2Specification.md. Only internal usage.
*******************************************************************************/

#ifndef TCOBS_INTERNAL_H_
#define TCOBS_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define N  0x00 //!< N  is the sigil byte 0x101ooooo, without operation with offsets 1-31 to keep the sigil chain linked.
//         0x10 is inside N
#define Z0 0x20 //!< Z0 is the sigil byte 0x001ooooo, a CCQN zeroCount cipher 0 with offsets 0-31.
//         0x30 is inside Z0
#define R1 0x40 //!< R1 is the sigil byte 0x0100oooo, a CCTN repeatCount cipher 1 with offsets 0-15.
#define Z2 0x50 //!< Z2 is the sigil byte 0x0101oooo, a CCQN zeroCount cipher 2 with offsets 0-15.
#define Z1 0x60 //!< Z1 is the sigil byte 0x011ooooo, a CCQN zeroCount cipher 1 with offsets 0-31.
//         0x70 is inside Z1
#define R0 0x80 //!< R0 is the sigil byte 0x100ooooo, a CCTN repeatCount cipher 0 with offsets 0-31.
//         0x90 is inside R0
#define R2 0xA0 //!< R2 is the sigil byte 0x1010oooo, a CCTN repeatCount cipher 2 with offsets 0-15.
#define Z3 0xB0 //!< Z3 is the sigil byte 0x0001oooo, a CCQN zeroCount cipher 3 with offsets 0-15.
#define F1 0xC0 //!< F1 is the sigil byte 0x110ooooo, a CCQN fullCount cipher 1 with offsets 0-31.
//         0xD0 is inside F1
#define F2 0xE0 //!< F2 is the sigil byte 0x1110oooo, a CCQN fullCount cipher 2 with offsets 0-15.
#define F3 0xF0 //!< F3 is the sigil byte 0x1111oooo, a CCQN fullCount cipher 3 with offsets 0-14.
#define F0 0xFF //!< F0 is the sigil byte 0x11111111, a CCQN fullCount cipher 0 with offset 0.

#ifdef __cplusplus
}
#endif

#endif // TCOBS_INTERNAL_H_
