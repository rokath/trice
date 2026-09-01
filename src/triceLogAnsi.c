// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceLogAnsi.c
//! \brief Optional record-local ANSI colors and lower-case tag removal.

#include "trice.h"
#include "triceLogInternal.h"

#if TRICE_LOCAL_LOG == 1 && (TRICE_LOCAL_LOG_USE_ANSI_COLORS == 1 || TRICE_LOCAL_LOG_STRIP_LOWER_CASE_TAGS == 1)

#include <limits.h>
#include <string.h>

//! triceLogTagStyle_t groups aliases which share one host palette color.
//!
//! This intentionally small target table mirrors the built-in Go tag hints in
//! internal/emitter/lineTransformerANSI.go. It is not generated and neither
//! file is authoritative for the other: maintainers may manually synchronize
//! the optional target aliases and colors when the host list changes. The Go
//! file contains the reciprocal maintenance note.
typedef struct {
	const char* aliases; //!< Pipe-separated exact aliases; first table match wins.
#if TRICE_LOCAL_LOG_USE_ANSI_COLORS == 1
	const char* ansi;    //!< ANSI Select Graphic Rendition sequence, or empty.
#endif
} triceLogTagStyle_t;

#if TRICE_LOCAL_LOG_USE_ANSI_COLORS == 1
#define TRICE_LOG_TAG_STYLE(aliases, ansi) {aliases, ansi}
#else
// Discarding the ANSI macro argument at preprocessing time keeps every escape
// string out of a strip-only object file, even without linker optimization.
#define TRICE_LOG_TAG_STYLE(aliases, ansi) {aliases}
#endif

// The order is significant and follows the Go table. Some historic aliases
// occur in more than one group (notably w, rx, tx, and s); matching the first
// group preserves the host's established precedence.
static const triceLogTagStyle_t triceLogTagStyles[] = {
	TRICE_LOG_TAG_STYLE("Fatal|fatal|FATAL", "\x1b[0;1;35;41m"),
	TRICE_LOG_TAG_STYLE("crit|Critical|critical|CRITICAL|Crit|CRIT", "\x1b[0;7;31;109m"),
	TRICE_LOG_TAG_STYLE("em|Emergency|emergency|EMERGENCY", "\x1b[0;7;31;44m"),
	TRICE_LOG_TAG_STYLE("e|Error|err|error|E|ERR|ERROR", "\x1b[0;38;5;11;41m"),
	TRICE_LOG_TAG_STYLE("w|wrn|Warning|warning|W|WRN|WARNING|Warn|warn|WARN", "\x1b[0;7;38;5;11;41m"),
	TRICE_LOG_TAG_STYLE("att|attention|Attention|ATT|ATTENTION", "\x1b[0;38;5;11;42m"),
	TRICE_LOG_TAG_STYLE("i|inf|info|Info|informal|I|INF|INFO|INFORMAL", "\x1b[0;1;36;109m"),
	TRICE_LOG_TAG_STYLE("d|db|Debug|dbg|deb|debug|D|DB|DBG|DEBUG", "\x1b[0;7;38;5;130m"),
	TRICE_LOG_TAG_STYLE("tr|Trace|trace|TRACE", "\x1b[0;7;39;109m"),
	TRICE_LOG_TAG_STYLE("tim|time|Time|TIM|TIME|TIMESTAMP|timestamp|Timestamp", "\x1b[0;7;34;103m"),
	TRICE_LOG_TAG_STYLE("m|msg|message|M|MSG|MESSAGE|OK", "\x1b[0;92;40m"),
	TRICE_LOG_TAG_STYLE("r|rx|rd|read|rd_|RD|RD_|READ", "\x1b[0;7;30;103m"),
	TRICE_LOG_TAG_STYLE("w|tx|wr|write|wr_|WR|WR_|WRITE", "\x1b[0;4;30;103m"),
	TRICE_LOG_TAG_STYLE("rx|receive|RECEIVE|Receive|RX", "\x1b[0;90;40m"),
	TRICE_LOG_TAG_STYLE("tx|transmit|TRANSMIT|Transmit|TX", "\x1b[0;30;100m"),
	TRICE_LOG_TAG_STYLE("dia|diag|Diag|DIA|DIAG", "\x1b[0;7;33;109m"),
	TRICE_LOG_TAG_STYLE("int|isr|ISR|INT|interrupt|Interrupt|INTERRUPT", "\x1b[0;7;35;109m"),
	TRICE_LOG_TAG_STYLE("s|sig|signal|S|SIG|SIGNAL", "\x1b[0;7;38;5;118m"),
	TRICE_LOG_TAG_STYLE("t|tst|test|T|TST|TEST", "\x1b[0;93;40m"),
	TRICE_LOG_TAG_STYLE("def|Default|DEFAULT|default", ""),
	TRICE_LOG_TAG_STYLE("note|Notice|NOTICE|notice|Note|NOTE", "\x1b[0;34;107m"),
	TRICE_LOG_TAG_STYLE("Alert|alert|ALERT", "\x1b[0;35;105m"),
	TRICE_LOG_TAG_STYLE("Assert|assert|ASSERT", "\x1b[0;7;33;44m"),
	TRICE_LOG_TAG_STYLE("a|Alarm|alarm|ALARM", "\x1b[0;7;31;107m"),
	TRICE_LOG_TAG_STYLE("CYCLE_ERROR", "\x1b[0;7;35;103m"),
	TRICE_LOG_TAG_STYLE("v|Verbose|verbose|VERBOSE", "\x1b[0;34;49m"),
	TRICE_LOG_TAG_STYLE("cfg|config", ""),
	TRICE_LOG_TAG_STYLE("us|\xC2\xB5s|uS|\xC2\xB5S|uSec|\xC2\xB5Sec|uSEC|\xC2\xB5SEC|MicroSec|Microsecond|Microseconds|ms|mS|mSec|mSEC|MSEC|MilliSec|Millisecond|Milliseconds|s|S|Sec|SEC|SECOND|SECONDS|Second|Seconds|dt|delta|dT|deltaTime|delta-time", "\x1b[0;7;34;103m"),
};

#undef TRICE_LOG_TAG_STYLE

#if TRICE_LOCAL_LOG_USE_ANSI_COLORS == 1
static const char triceLogAnsiReset[] = "\x1b[0m";
#endif

//! triceLogAliasListContains performs an allocation-free exact token lookup.
static int triceLogAliasListContains(const char* aliases, const char* tag, size_t tagBytes) {
	const char* candidate = aliases;
	for (;;) {
		const char* separator = strchr(candidate, '|');
		size_t candidateBytes = separator == 0 ? strlen(candidate) : (size_t)(separator - candidate);
		if (candidateBytes == tagBytes && memcmp(candidate, tag, tagBytes) == 0) {
			return 1;
		}
		if (separator == 0) {
			return 0;
		}
		candidate = separator + 1;
	}
}

//! triceLogFindTag returns the first matching built-in style.
static const triceLogTagStyle_t* triceLogFindTag(const char* tag, size_t tagBytes) {
	for (size_t index = 0u; index < sizeof(triceLogTagStyles) / sizeof(triceLogTagStyles[0]); ++index) {
		if (triceLogAliasListContains(triceLogTagStyles[index].aliases, tag, tagBytes)) {
			return &triceLogTagStyles[index];
		}
	}
	return 0;
}

#if TRICE_LOCAL_LOG_STRIP_LOWER_CASE_TAGS == 1
//! triceLogTagIsLowerCase mirrors the relevant host rule for known aliases.
//!
//! Every non-ASCII alias byte is part of a table spelling such as UTF-8 µs.
//! Rejecting ASCII upper-case letters is therefore sufficient for this fixed
//! list and retains punctuation such as rd_ and delta-time.
static int triceLogTagIsLowerCase(const char* tag, size_t tagBytes) {
	for (size_t index = 0u; index < tagBytes; ++index) {
		if ('A' <= tag[index] && tag[index] <= 'Z') {
			return 0;
		}
	}
	return 1;
}
#endif

int triceLogApplyAnsiAndTagPolicy(char* body, size_t bodySize, size_t bodyBytes) {
	if (body == 0 || bodySize == 0u) {
		return TRICE_LOG_ERR_ARGUMENT;
	}
	if (bodyBytes >= bodySize || bodyBytes > (size_t)INT_MAX) {
		body[0] = 0;
		return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
	}

	const char* colon = (const char*)memchr(body, ':', bodyBytes);
	if (colon == 0 || colon == body) {
		return (int)bodyBytes;
	}
	size_t tagBytes = (size_t)(colon - body);
	const triceLogTagStyle_t* style = triceLogFindTag(body, tagBytes);
	if (style == 0) {
		return (int)bodyBytes;
	}

	size_t stripBytes = 0u;
#if TRICE_LOCAL_LOG_STRIP_LOWER_CASE_TAGS == 1
	if (triceLogTagIsLowerCase(body, tagBytes)) {
		stripBytes = tagBytes + 1u;
	}
#endif
	size_t visibleBytes = bodyBytes - stripBytes;
	size_t ansiBytes = 0u;
	size_t resetBytes = 0u;
#if TRICE_LOCAL_LOG_USE_ANSI_COLORS == 1
	if (visibleBytes != 0u && style->ansi[0] != 0) {
		ansiBytes = strlen(style->ansi);
		resetBytes = sizeof(triceLogAnsiReset) - 1u;
	}
#endif
	if (visibleBytes > SIZE_MAX - ansiBytes || visibleBytes + ansiBytes > SIZE_MAX - resetBytes) {
		body[0] = 0;
		return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
	}
	size_t finalBytes = visibleBytes + ansiBytes + resetBytes;
	if (finalBytes >= bodySize || finalBytes > (size_t)INT_MAX) {
		body[0] = 0;
		return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
	}

	if (stripBytes != 0u) {
		memmove(body, body + stripBytes, visibleBytes + 1u);
	}
#if TRICE_LOCAL_LOG_USE_ANSI_COLORS == 0
	// A strip-only build has no ANSI member or escape strings. Returning here
	// also lets the compiler prove that the color machinery is absent.
	return (int)visibleBytes;
#else
	if (ansiBytes == 0u) {
		return (int)visibleBytes;
	}

	// Keep terminal state local to one Trice record. The reset precedes trailing
	// CR/LF bytes so the terminal never starts the following line in this color.
	size_t trailingBytes = 0u;
	while (trailingBytes < visibleBytes) {
		char value = body[visibleBytes - trailingBytes - 1u];
		if (value != '\r' && value != '\n') {
			break;
		}
		trailingBytes++;
	}
	memmove(body + ansiBytes, body, visibleBytes + 1u);
	memcpy(body, style->ansi, ansiBytes);
	size_t resetAt = ansiBytes + visibleBytes - trailingBytes;
	memmove(body + resetAt + resetBytes, body + resetAt, trailingBytes + 1u);
	memcpy(body + resetAt, triceLogAnsiReset, resetBytes);
	return (int)finalBytes;
#endif
}

#endif // TRICE_LOCAL_LOG == 1 && (ANSI colors or lower-case tag stripping)
