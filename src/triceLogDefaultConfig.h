// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceLogDefaultConfig.h
//! \brief Default configuration for optional target-side Trice formatting.
//!
//! Applications override these values in their existing triceConfig.h before
//! including trice.h. This file deliberately contains no formatter-library
//! include and no NANOPRINTF_* dependency: UserTriceLogPrintfFn may point to
//! snprintf, nanoprintf, another third-party implementation, or application
//! code with compatible return semantics.

#ifndef TRICE_LOG_DEFAULT_CONFIG_H_
#define TRICE_LOG_DEFAULT_CONFIG_H_

//! TRICE_LOCAL_LOG enables target-side consumption and text formatting of
//! records accumulated in a deferred ring or double buffer.
//!
//! TriceLog and TriceTransfer are alternative consumers of that buffer. A
//! local-log application must not enable a binary deferred output channel for
//! the same records. Keeping this switch at 0 removes the complete local-log
//! implementation independently of all more detailed switches below.
#ifndef TRICE_LOCAL_LOG
#define TRICE_LOCAL_LOG 0
#endif

//! TRICE_LOCAL_LOG_USE_PRINTF_HOOK enables the snprintf-compatible
//! UserTriceLogPrintfFn runtime hook.
//!
//! The hook is invoked once for each ordinary scalar conversion. It is not
//! used for bounded dynamic strings or for Trice-specific conversions whose
//! semantics are not portable across printf implementations. Setting this to
//! 0 removes the hook variable and dispatch path; literal-only, dynamic-string,
//! and internally formatted records can still work.
#ifndef TRICE_LOCAL_LOG_USE_PRINTF_HOOK
#define TRICE_LOCAL_LOG_USE_PRINTF_HOOK 1
#endif

//! TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER links the separately compiled exact
//! %d/%x fallback.
//!
//! When both this switch and TRICE_LOCAL_LOG_USE_PRINTF_HOOK are 1, a non-null
//! hook takes precedence and the minimal formatter is used only while the hook
//! is null. Set this switch to 0 when the application always installs a hook;
//! the fallback then contributes no code without requiring LTO.
#ifndef TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER
#define TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER 1
#endif

//! TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS accepts fixed field widths
//! such as %8d, %02x, and %-12s.
//!
//! Dynamic field width with '*' remains unsupported because it would consume
//! an additional transported value. Disabling this feature rejects formats
//! containing a fixed width and lets matching generated format strings be
//! removed from the target image.
#ifndef TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS
#define TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#endif

//! TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS accepts fixed precision such
//! as %.3f and %.5s.
//!
//! Dynamic precision with '*' remains unsupported. The option is independent
//! from float support because bounded dynamic strings also use precision.
#ifndef TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS
#define TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS 1
#endif

//! TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS accepts %e, %E, %f, %F, %g,
//! and %G for transported aFloat values and, when 64-bit values are enabled,
//! aDouble values.
//!
//! Trice reconstructs the floating value but delegates its decimal rendering
//! to UserTriceLogPrintfFn. The selected hook must therefore provide the same
//! enabled float capability. This switch itself never includes or configures a
//! third-party printf library.
#ifndef TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS
#define TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS 1
#endif

//! TRICE_LOCAL_LOG_USE_64_BIT_VALUES accepts transported 64-bit scalar and
//! buffer elements.
//!
//! Together with TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS it also enables
//! aDouble rendering. Disable it on small targets to remove 64-bit decode and
//! formatting paths. Literal-only TRICE64_0 records remain usable because they
//! transport no 64-bit value.
#ifndef TRICE_LOCAL_LOG_USE_64_BIT_VALUES
#define TRICE_LOCAL_LOG_USE_64_BIT_VALUES 1
#endif

//! TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS accepts the Trice %b integer
//! conversion.
//!
//! Binary formatting is implemented inside Trice because %b is not portable
//! across snprintf implementations. Fixed field width, precision, zero fill,
//! left alignment, and the '#' alternative prefix follow the corresponding
//! feature switches.
#ifndef TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS
#define TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS 0
#endif

//! TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG accepts the '#' flag.
//!
//! For ordinary printf conversions the flag is passed to the configured hook.
//! Trice applies it itself to internally rendered conversions such as %b. This
//! switch corresponds conceptually to NANOPRINTF_USE_ALT_FORM_FLAG but has no
//! compile-time dependency on nanoprintf.
#ifndef TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG
#define TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG 1
#endif

//! TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS accepts Trice-specific %O,
//! %t, %p, and %q conversions.
//!
//! Their semantics intentionally follow the host Trice decoder rather than a
//! platform printf implementation: %O is octal with a 0o prefix, %t is true or
//! false, %p is the transported value in lowercase hexadecimal without a C
//! pointer cast, and %q quotes a character or dynamic string. Trice formats or
//! normalizes these conversions internally.
#ifndef TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS
#define TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS 0
#endif

//! TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES accepts bounded triceS payloads
//! and string-form triceN payloads.
//!
//! The encoded record length, not a presumed trailing NUL, bounds the source.
//! Exactly one %s is supported; %q is additionally accepted when extended
//! format specifiers are enabled. Width and precision are controlled by their
//! dedicated switches and formatting never calls the printf hook.
#ifndef TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES
#define TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES 1
#endif

//! TRICE_LOCAL_LOG_USE_BUFFER_TRICES accepts TRICE8_B, TRICE16_B, TRICE32_B,
//! and TRICE64_B records.
//!
//! A buffer format must contain exactly one scalar conversion. Trice repeats
//! its item portion for every aligned payload element and writes a trailing
//! newline only once, matching the host decoder's buffer semantics. The
//! element conversion still needs every corresponding scalar feature.
#ifndef TRICE_LOCAL_LOG_USE_BUFFER_TRICES
#define TRICE_LOCAL_LOG_USE_BUFFER_TRICES 0
#endif

//! TRICE_LOCAL_LOG_USE_PREFIX_HOOK enables the optional
//! UserTriceLogPrefixFn callback before each formatted record.
//!
//! The callback can add a project-specific timestamp, route, or diagnostic
//! prefix from the parsed ID and stamp fields. Trice itself does not assume
//! that a stamp is time and does not duplicate host color or display policy.
//! Keeping this switch at 0 removes the callback and preserves the current
//! message-body-only output.
#ifndef TRICE_LOCAL_LOG_USE_PREFIX_HOOK
#define TRICE_LOCAL_LOG_USE_PREFIX_HOOK 0
#endif

//! TRICE_LOCAL_LOG_KEEP_DISABLED_IDS keeps compact metadata rows for generated
//! IDs whose format strings require disabled local-log features.
//!
//! Such rows contain a null format pointer, allowing TriceLog to return the
//! distinct TRICE_LOG_ERR_FEATURE_DISABLED result. Set this to 0 for maximum
//! table-size reduction; an emitted disabled ID is then indistinguishable from
//! an obsolete or otherwise missing generated table entry.
#ifndef TRICE_LOCAL_LOG_KEEP_DISABLED_IDS
#define TRICE_LOCAL_LOG_KEEP_DISABLED_IDS 1
#endif

// Every public local-log switch is Boolean. Validate values here so misspelled
// numeric settings fail at the configuration boundary rather than silently
// selecting surprising preprocessor branches in several translation units.
#if (TRICE_LOCAL_LOG != 0) && (TRICE_LOCAL_LOG != 1)
#error configuration: TRICE_LOCAL_LOG must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_PRINTF_HOOK != 0) && (TRICE_LOCAL_LOG_USE_PRINTF_HOOK != 1)
#error configuration: TRICE_LOCAL_LOG_USE_PRINTF_HOOK must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER != 0) && (TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER != 1)
#error configuration: TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS != 0) && (TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS != 1)
#error configuration: TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS != 0) && (TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS != 1)
#error configuration: TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS != 0) && (TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS != 1)
#error configuration: TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_64_BIT_VALUES != 0) && (TRICE_LOCAL_LOG_USE_64_BIT_VALUES != 1)
#error configuration: TRICE_LOCAL_LOG_USE_64_BIT_VALUES must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS != 0) && (TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS != 1)
#error configuration: TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG != 0) && (TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG != 1)
#error configuration: TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS != 0) && (TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS != 1)
#error configuration: TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES != 0) && (TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES != 1)
#error configuration: TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_BUFFER_TRICES != 0) && (TRICE_LOCAL_LOG_USE_BUFFER_TRICES != 1)
#error configuration: TRICE_LOCAL_LOG_USE_BUFFER_TRICES must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_USE_PREFIX_HOOK != 0) && (TRICE_LOCAL_LOG_USE_PREFIX_HOOK != 1)
#error configuration: TRICE_LOCAL_LOG_USE_PREFIX_HOOK must be 0 or 1
#endif
#if (TRICE_LOCAL_LOG_KEEP_DISABLED_IDS != 0) && (TRICE_LOCAL_LOG_KEEP_DISABLED_IDS != 1)
#error configuration: TRICE_LOCAL_LOG_KEEP_DISABLED_IDS must be 0 or 1
#endif

#endif // TRICE_LOG_DEFAULT_CONFIG_H_
