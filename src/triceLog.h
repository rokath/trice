// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceLog.h
//! \brief Optional target-side formatting of buffered Trice records.

#ifndef TRICE_LOG_H_
#define TRICE_LOG_H_

#include <stddef.h>

#if TRICE_LOCAL_LOG == 1

#ifdef __cplusplus
extern "C" {
#endif

//! TriceLogPrintfFn_t is the optional snprintf-compatible formatter hook.
//!
//! TriceLog invokes the hook once for every scalar conversion. The function
//! must use snprintf return semantics: the returned count excludes the final
//! NUL and reports the required size even when the destination is too small.
typedef int (*TriceLogPrintfFn_t)(char* buffer, size_t size, const char* format, ...);

//! UserTriceLogPrintfFn selects an application or third-party formatter.
//!
//! With TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER == 1, leave it zero to use the
//! separately linked fallback for exact %d and %x plus literal %%. With that
//! switch at zero, a zero hook makes TriceLog return TRICE_LOG_ERR_PRINTF.
extern TriceLogPrintfFn_t UserTriceLogPrintfFn;

//! Stable negative results returned by TriceLog.
enum {
	TRICE_LOG_ERR_ARGUMENT = -100,           //!< outbuf is NULL or maxlen is zero; no record is consumed.
	TRICE_LOG_ERR_OUTPUT_TOO_SMALL = -101,   //!< The current record was consumed but did not fit including its final NUL.
	TRICE_LOG_ERR_RECORD = -102,             //!< The buffered record was malformed; the buffer was cleared to guarantee progress.
	TRICE_LOG_ERR_ID = -103,                 //!< The current ID was absent from the generated log table.
	TRICE_LOG_ERR_FORMAT = -104,             //!< The format uses a conversion unsupported by local scalar logging.
	TRICE_LOG_ERR_PRINTF = -105,             //!< The configured printf hook reported an error.
	TRICE_LOG_ERR_BUFFER = -106,             //!< The selected deferred buffer rejected a peek or release operation.
};

//! \brief Formats and consumes the next buffered Trice record.
//! \param outbuf Application-owned text destination.
//! \param maxlen Complete destination capacity including the final NUL.
//! \retval >0 Number of visible bytes in outbuf, excluding its final NUL.
//! \retval 0 No complete printable record is currently available.
//! \retval <0 One of the documented TRICE_LOG_ERR_* values.
//!
//! For every valid outbuf/maxlen pair, outbuf is NUL-terminated. A complete
//! record is released after success and after record-local errors. Empty
//! messages are consumed internally so zero remains unambiguously "no data".
//! TriceLog and TriceTransfer are alternative consumers and must not be called
//! for the same buffer configuration.
int TriceLog(char* outbuf, size_t maxlen);

#ifdef __cplusplus
}
#endif

#endif // TRICE_LOCAL_LOG == 1

#endif // TRICE_LOG_H_
