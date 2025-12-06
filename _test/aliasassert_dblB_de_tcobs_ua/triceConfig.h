/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_AUXILIARY32 1
#define TRICE_CGO 1
#define TRICE_CYCLE_COUNTER 0
#define TRICE_SINGLE_MAX_SIZE (104 + 100)

//////////////////////////////////////////////////////////////////////////////
// aliases - we have to exclude this in trice_environment.sh
//
#include "nanoprintf.h"

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

// Portable pragma macros for disabling nonnull warnings
#if defined(__GNUC__) || defined(__clang__)
// GCC and Clang
#define DISABLE_NONNULL_WARNING_BEGIN \
	_Pragma("GCC diagnostic push")    \
	    _Pragma("GCC diagnostic ignored \"-Wnonnull\"")
#define DISABLE_NONNULL_WARNING_END \
	_Pragma("GCC diagnostic pop")
#elif defined(_MSC_VER)
// Microsoft Visual C++
#define DISABLE_NONNULL_WARNING_BEGIN \
	__pragma(warning(push))           \
	    __pragma(warning(disable : 6387))
#define DISABLE_NONNULL_WARNING_END \
	__pragma(warning(pop))
#else
// Unknown compiler - no-op
#define DISABLE_NONNULL_WARNING_BEGIN
#define DISABLE_NONNULL_WARNING_END
#endif

#define ASSERT_MESSAGE_HELPER_VAR(out_var, condition_str, file_path, line_number, fmt, ...) \
	char full_msg[512];                                                                     \
	if (fmt != NULL) {                                                                      \
		DISABLE_NONNULL_WARNING_BEGIN                                                       \
		char user_msg[256];                                                                 \
		npf_snprintf(user_msg, sizeof(user_msg), fmt, ##__VA_ARGS__);                       \
		DISABLE_NONNULL_WARNING_END                                                         \
		npf_snprintf(full_msg, sizeof(full_msg), "[ASSERT] %s:%d: %s -> %s\n",              \
		             FILENAME(file_path), line_number, condition_str, user_msg);            \
	} else {                                                                                \
		npf_snprintf(full_msg, sizeof(full_msg), "[ASSERT] %s:%d: %s\n",                    \
		             FILENAME(file_path), line_number, condition_str);                      \
	}                                                                                       \
	char* out_var = full_msg

#define CUSTOM_ASSERT_IMPL(id, condition, condition_str, file, line, fmt, ...)                    \
	do {                                                                                          \
		if (!(condition)) {                                                                       \
			ASSERT_MESSAGE_HELPER_VAR(assert_msg, condition_str, file, line, fmt, ##__VA_ARGS__); \
			triceS(id, "%s", assert_msg);                                                         \
		}                                                                                         \
	} while (0)

#define CUSTOM_PRINT(id, ...) trice(id, ##__VA_ARGS__)

// Assert implementations
#define CUSTOM_ASSERT_1(id) \
	static_assert(0, "CUSTOM_ASSERT requires at least 2 arguments: id, condition")

#define CUSTOM_ASSERT_2(id, condition) \
	CUSTOM_ASSERT_IMPL(id, condition, #condition, __FILE__, __LINE__, NULL)

#define CUSTOM_ASSERT_3(id, condition, fmt) \
	CUSTOM_ASSERT_IMPL(id, condition, #condition, __FILE__, __LINE__, fmt)

#define CUSTOM_ASSERT_4(id, condition, fmt, ...) \
	CUSTOM_ASSERT_IMPL(id, condition, #condition, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define CUSTOM_ASSERT(...) \
	_DISPATCH_EXPAND(CUSTOM_ASSERT, COUNT_ARGS_4(__VA_ARGS__))(__VA_ARGS__)

//
//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
