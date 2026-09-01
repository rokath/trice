// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceLogInternal.h
//! \brief Private contracts shared by local logging and deferred buffers.

#ifndef TRICE_LOG_INTERNAL_H_
#define TRICE_LOG_INTERNAL_H_

#include <stddef.h>
#include <stdint.h>

#include "triceLog.h"

#if TRICE_LOCAL_LOG == 1

//! triceLogBufferPeek exposes the next record without copying it.
//! The returned byte count may include later records, but always contains the
//! complete first record. Exactly one consumer may own the view at a time.
int triceLogBufferPeek(const uint8_t** record, size_t* available);

//! triceLogBufferRelease releases one padded record previously returned by
//! triceLogBufferPeek. storageBytes must be a non-zero multiple of four.
int triceLogBufferRelease(size_t storageBytes);

//! triceLogBufferDiscardAll restores progress after structural corruption made
//! the first record length unknowable. This intentionally drops queued data.
void triceLogBufferDiscardAll(void);

#if TRICE_LOCAL_LOG_USE_ANSI_COLORS == 1 || TRICE_LOCAL_LOG_STRIP_LOWER_CASE_TAGS == 1

//! triceLogApplyAnsiAndTagPolicy applies optional presentation to one body.
//!
//! The caller-provided body is already formatted and NUL-terminated. The
//! function may remove one known lower-case tag and may insert one ANSI color
//! sequence plus a reset. It returns the new visible byte count or a public
//! TRICE_LOG_ERR_* value without allocating memory.
int triceLogApplyAnsiAndTagPolicy(char* body, size_t bodySize, size_t bodyBytes);

#endif // ANSI colors or lower-case tag stripping

#if TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER == 1

//! triceLogMinimalSigned appends one base-10 signed value without libc printf.
#if TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 1
int triceLogMinimalSigned(char* dst, size_t size, int64_t value);

//! triceLogMinimalHex appends one lowercase base-16 unsigned value.
int triceLogMinimalHex(char* dst, size_t size, uint64_t value);
#else
int triceLogMinimalSigned(char* dst, size_t size, int32_t value);

//! triceLogMinimalHex appends one lowercase base-16 unsigned value.
int triceLogMinimalHex(char* dst, size_t size, uint32_t value);
#endif

#endif // TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER == 1

#endif // TRICE_LOCAL_LOG == 1

#endif // TRICE_LOG_INTERNAL_H_
