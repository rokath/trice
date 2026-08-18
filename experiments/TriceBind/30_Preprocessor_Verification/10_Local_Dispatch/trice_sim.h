#ifndef POC1_TRICE_SIM_H
#define POC1_TRICE_SIM_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

/* Minimal TID encoding used only by this PoC. */
#define iD(n) (0xA0000000u | (uint32_t)(n))

/*
 * Expansion-time route selection.
 * If TRICE_BIND_FILE_KEY is not defined, it remains an identifier token.
 * The fallback token maps to INSERT.
 */
#define TRICE_BIND_ROUTE_TRICE_BIND_FILE_KEY INSERT
#define TRICE_BIND_ROUTE_I(key) TRICE_BIND_ROUTE_##key
#define TRICE_BIND_ROUTE(key) TRICE_BIND_ROUTE_I(key)

#define TRICE_DISPATCH_I(route, ...) TRICE_ROUTE_##route(__VA_ARGS__)
#define TRICE_DISPATCH(route, ...) TRICE_DISPATCH_I(route, __VA_ARGS__)

#define trice(...) TRICE_DISPATCH(TRICE_BIND_ROUTE(TRICE_BIND_FILE_KEY), __VA_ARGS__)

#define TRICE_ROUTE_INSERT(tid, ...) poc1_emit("INSERT", (tid), __VA_ARGS__)
#define TRICE_ROUTE_BIND(...) poc1_emit("BIND", TRICE_BIND_TID_HERE(), __VA_ARGS__)

static inline void poc1_emit(const char* route, uint32_t tid, const char* format, ...) {
	va_list ap;
	printf("route=%s id=%u text=", route, (unsigned)(tid & 0x00FFFFFFu));
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	putchar('\n');
}

#endif
