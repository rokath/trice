// SPDX-License-Identifier: MIT

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

// Store short binary records quickly in the producer context. TriceLog reads
// them later without first copying their payload out of the ring buffer.
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 1024
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_LOCAL_LOG 1
#define TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER 0

// The example uses the application-provided snprintf hook and needs no binary
// UART, RTT, auxiliary, framing, or encryption output.
#define TRICE_DIRECT_OUTPUT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_CONFIG_WARNINGS 0

#endif // TRICE_CONFIG_H_
