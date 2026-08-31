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
