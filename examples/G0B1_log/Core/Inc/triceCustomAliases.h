// SPDX-License-Identifier: MIT

//! \file triceCustomAliases.h

#ifndef TRICE_CUSTOM_ALIASES_H_
#define TRICE_CUSTOM_ALIASES_H_

#include "stm32g0xx_ll_usart.h"
#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"
#include "trice.h"

// Helper macro to extract the filename from the full path
#ifndef FILENAME
#define FILENAME(path) (strrchr(path, '/') ? strrchr(path, '/') + 1 : (strrchr(path, '\\') ? strrchr(path, '\\') + 1 : path))
#endif

// Argument counting dispatch (up to 16 arguments)
#define GET_17TH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, ...) _17
#define COUNT_ARGS_3(...) GET_17TH_ARG(__VA_ARGS__, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0)
#define COUNT_ARGS_4(...) GET_17TH_ARG(__VA_ARGS__, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0)
#define _DISPATCH_IMPL(name, N) name##_##N
#define _DISPATCH_EXPAND(name, N) _DISPATCH_IMPL(name, N)

// Build an assertion message without a user-provided format string.
#define ASSERT_MESSAGE_HELPER(out_var, condition_str, file_path, line_number) \
	char full_msg[512];                                                       \
	npf_snprintf(full_msg, sizeof(full_msg), "[ASSERT] %s:%d: %s\n",          \
	             FILENAME(file_path), line_number, condition_str);            \
	char* out_var = full_msg

// Build an assertion message from a mandatory format string and optional values.
// The format string is part of __VA_ARGS__, so the variadic list is never empty.
#define ASSERT_FORMATTED_MESSAGE_HELPER(out_var, condition_str, file_path, line_number, ...) \
	char user_msg[256];                                                                      \
	npf_snprintf(user_msg, sizeof(user_msg), __VA_ARGS__);                                   \
	char full_msg[512];                                                                      \
	npf_snprintf(full_msg, sizeof(full_msg), "[ASSERT] %s:%d: %s -> %s\n",                   \
	             FILENAME(file_path), line_number, condition_str, user_msg);                 \
	char* out_var = full_msg

#if (defined(TRICE_CLEAN) && TRICE_CLEAN == 0) || !defined(TRICE_OFF) || TRICE_OFF == 0
// ALL calls have ID as first parameter.

#define CUSTOM_ASSERT_IMPL(id, condition, condition_str, file, line)      \
	do {                                                                  \
		if (!(condition)) {                                               \
			ASSERT_MESSAGE_HELPER(assert_msg, condition_str, file, line); \
			TRICE_INSERT_triceS(id, "%s", assert_msg);                    \
		}                                                                 \
	} while (0)

#define CUSTOM_ASSERT_FORMAT_IMPL(id, condition, condition_str, file, line, ...)                 \
	do {                                                                                         \
		if (!(condition)) {                                                                      \
			ASSERT_FORMATTED_MESSAGE_HELPER(assert_msg, condition_str, file, line, __VA_ARGS__); \
			TRICE_INSERT_triceS(id, "%s", assert_msg);                                           \
		}                                                                                        \
	} while (0)

#define CUSTOM_PRINT(id, ...) trice(id, __VA_ARGS__)

// Assert implementations
#define CUSTOM_ASSERT_1(id) \
	static_assert(0, "CUSTOM_ASSERT requires at least 2 arguments: id, condition")

#define CUSTOM_ASSERT_2(id, condition) \
	CUSTOM_ASSERT_IMPL(id, condition, #condition, __FILE__, __LINE__)

#define CUSTOM_ASSERT_3(id, condition, ...) \
	CUSTOM_ASSERT_FORMAT_IMPL(id, condition, #condition, __FILE__, __LINE__, __VA_ARGS__)

#define CUSTOM_ASSERT_4(id, condition, ...) \
	CUSTOM_ASSERT_FORMAT_IMPL(id, condition, #condition, __FILE__, __LINE__, __VA_ARGS__)

// Route the registered simple alias at its invocation site. Insert-owned calls
// already carry an ID, while bound calls receive the same argument from the
// generated sidecar before the existing assertion implementation is expanded.
#define CUSTOM_ASSERT_WITH_ID(...) \
	_DISPATCH_EXPAND(CUSTOM_ASSERT, COUNT_ARGS_4(__VA_ARGS__))(__VA_ARGS__)
#define CUSTOM_ASSERT(...) \
	TRICE_BIND_DISPATCH(TRICE_BIND_ROUTE(TRICE_BIND_FILE_KEY), CUSTOM_ASSERT_WITH_ID, __VA_ARGS__)

#else
static inline void uart2_putc(int c, void* ctx) {
	(void)ctx; // unused
	while (!LL_USART_IsActiveFlag_TXE(USART2)) {
	}
	LL_USART_TransmitData8(USART2, (uint8_t)c);
	while (!LL_USART_IsActiveFlag_TC(USART2)) {
	}
}

// NO calls have ID, fallback to printf.
#define CUSTOM_ASSERT_IMPL(condition, condition_str, file, line)          \
	do {                                                                  \
		if (!(condition)) {                                               \
			ASSERT_MESSAGE_HELPER(assert_msg, condition_str, file, line); \
			npf_pprintf(&uart2_putc, NULL, "%s", assert_msg);             \
		}                                                                 \
	} while (0)

#define CUSTOM_ASSERT_FORMAT_IMPL(condition, condition_str, file, line, ...)                     \
	do {                                                                                         \
		if (!(condition)) {                                                                      \
			ASSERT_FORMATTED_MESSAGE_HELPER(assert_msg, condition_str, file, line, __VA_ARGS__); \
			npf_pprintf(&uart2_putc, NULL, "%s", assert_msg);                                    \
		}                                                                                        \
	} while (0)

#define CUSTOM_PRINT(...) npf_pprintf(&uart2_putc, NULL, __VA_ARGS__)

#define CUSTOM_ASSERT_1(condition) \
	CUSTOM_ASSERT_IMPL(condition, #condition, __FILE__, __LINE__)

#define CUSTOM_ASSERT_2(condition, ...) \
	CUSTOM_ASSERT_FORMAT_IMPL(condition, #condition, __FILE__, __LINE__, __VA_ARGS__)

#define CUSTOM_ASSERT_3(condition, ...) \
	CUSTOM_ASSERT_FORMAT_IMPL(condition, #condition, __FILE__, __LINE__, __VA_ARGS__)

#define CUSTOM_ASSERT(...) _DISPATCH_EXPAND(CUSTOM_ASSERT, COUNT_ARGS_3(__VA_ARGS__))(__VA_ARGS__)
#endif

#endif
