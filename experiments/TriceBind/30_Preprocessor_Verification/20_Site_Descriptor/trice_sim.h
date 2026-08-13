#ifndef POC2_TRICE_SIM_H
#define POC2_TRICE_SIM_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

/* Minimal TID encoding used only by this PoC. */
#define iD(n) (0xA0000000u | (uint32_t)(n))
#define id(n) (0x00000000u | (uint32_t)(n))
#define Id(n) (0x10000000u | (uint32_t)(n))
#define ID(n) (0x20000000u | (uint32_t)(n))

#define TRICE_BIND_SITE_I(key, line) TRICE_BIND_SITE_##key##_L##line
#define TRICE_BIND_SITE(key, line) TRICE_BIND_SITE_I(key, line)
#define TRICE_BIND_SITE_HERE() TRICE_BIND_SITE(TRICE_BIND_FILE_KEY, __LINE__)

/*
 * TRICE_BIND_SITE_HERE() expands to two comma-separated tokens:
 *     processing-mode, complete-TID-expression
 * The extra macro layer makes those tokens arguments of TRICE_BIND_APPLY_I.
 */
#define TRICE_BIND_APPLY(...) TRICE_BIND_APPLY_I(__VA_ARGS__)
#define TRICE_BIND_APPLY_I(mode, tid, ...) mode(tid, __VA_ARGS__)

#define TRICE_BIND_AUTO(tid, ...) poc2_emit("AUTO", (tid), __VA_ARGS__)
#define TRICE_BIND_REPLACE(tid, ignoredTid, ...) poc2_emit("REPLACE", (tid), __VA_ARGS__)

#define TRICE(...) TRICE_BIND_APPLY(TRICE_BIND_SITE_HERE(), __VA_ARGS__)
#define trice(...) TRICE_BIND_APPLY(TRICE_BIND_SITE_HERE(), __VA_ARGS__)

static inline const char* poc2_stamp_name(uint32_t tid) {
	switch (tid & 0xF0000000u) {
	case 0x00000000u:
		return "id";
	case 0x10000000u:
		return "Id";
	case 0x20000000u:
		return "ID";
	case 0xA0000000u:
		return "iD";
	default:
		return "unknown";
	}
}

static inline void poc2_emit(const char* mode, uint32_t tid, const char* format, ...) {
	va_list ap;
	printf("mode=%s stamp=%s id=%u text=", mode, poc2_stamp_name(tid), (unsigned)(tid & 0x00FFFFFFu));
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	putchar('\n');
}

#endif
