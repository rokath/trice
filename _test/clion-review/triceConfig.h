/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA
#define TRICE_CYCLE_COUNTER 0

// NOTE (Windows/MinGW):
// TRICE uses TRICE_WEAK (= __attribute__((weak))) for default backend functions so user code can override them.
// On Windows with MinGW (COFF/PE), weak function symbols are implemented via COMDAT/linkonce sections and
// are not always reliably used by the linker to satisfy external references (can lead to "undefined reference"
// even though nm shows the weak symbols in the .obj).
// Therefore we disable TRICE_WEAK for this host build on MinGW to force strong definitions.
// See: https://sourceware.org/binutils/docs/ld/Options.html (COFF/COMDAT behavior is different from ELF/Mach-O)
#if defined(__MINGW32__) || defined(__MINGW64__)
#undef TRICE_WEAK
#define TRICE_WEAK
#endif

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
