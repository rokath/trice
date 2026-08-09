// SPDX-License-Identifier: MIT

// This file models output generated and maintained by a future `trice bind`.
// It intentionally has no include guard because SETUP, BEGIN, and END are
// separate inclusion modes used at different source positions.

#if (defined(TRICE_BIND_REBASE_SETUP) + defined(TRICE_BIND_REBASE_BEGIN) + \
     defined(TRICE_BIND_REBASE_END)) != 1
#error "Select exactly one generated rebase-helper mode."
#endif

#if defined(TRICE_BIND_REBASE_SETUP)

// The generated file key normally uses the regular line-based bind route.
#undef TRICE_BIND_FILE_KEY
#define TRICE_BIND_FILE_KEY K000000000000B101
#define TRICE_BIND_ROUTE_K000000000000B101 BIND

// These two IDs belong to the two Trice definitions inside LOG_ERROR.
#define POC_REBASE_COUNT_S_CASE0 2
#define POC_REBASE_COUNT_S_CASE7 2
#define POC_REBASE_COUNT_S_DEFAULT 2
#define POC_REBASE_COUNT_S_SAME_LINE 4

// Generated constant expressions map local expansion ordinals to stable IDs.
// They require no lookup table, RAM, dynamic allocation, or runtime mutation.
#define POC_REBASE_PAIR_ID(index) (((index) & 1) == 0 ? 6101u : 6102u)
#define POC_REBASE_ID_S_CASE0(index) POC_REBASE_PAIR_ID(index)
#define POC_REBASE_ID_S_CASE7(index) POC_REBASE_PAIR_ID(index)
#define POC_REBASE_ID_S_DEFAULT(index) POC_REBASE_PAIR_ID(index)
#define POC_REBASE_ID_S_SAME_LINE(index) POC_REBASE_PAIR_ID(index)

// Two-step concatenation expands a generated scope token before joining it.
#define POC_REBASE_CAT_I(left, right) left##right
#define POC_REBASE_CAT(left, right) POC_REBASE_CAT_I(left, right)
#define POC_REBASE_BASE_NAME(scope) POC_REBASE_CAT(triceBindCounterBase_, scope)
#define POC_REBASE_END_NAME(scope) POC_REBASE_CAT(triceBindCounterEnd_, scope)
#define POC_REBASE_COUNT_NAME(scope) POC_REBASE_CAT(POC_REBASE_COUNT_, scope)
#define POC_REBASE_ID_NAME(scope) POC_REBASE_CAT(POC_REBASE_ID_, scope)
#define POC_REBASE_SCOPE_COUNT(scope) POC_REBASE_COUNT_NAME(scope)
#define POC_REBASE_SCOPE_ID(scope, index) POC_REBASE_ID_NAME(scope)(index)

// The assertion spelling is the only C/C++ language difference in the helper.
#if defined(__cplusplus)
#define POC_REBASE_STATIC_ASSERT(condition, message) static_assert((condition), message)
#else
#define POC_REBASE_STATIC_ASSERT(condition, message) _Static_assert((condition), message)
#endif

// A captured counter becomes a number before it is reused in several checks.
#define POC_REBASE_LOCAL_INDEX(counter) ((counter) - POC_REBASE_ACTIVE_BASE - 1)
#define POC_REBASE_CAPTURE(implementation, counter, ...) \
    POC_REBASE_APPLY(implementation, counter, __VA_ARGS__)
#define POC_REBASE_APPLY(implementation, counter, ...)                                      \
    do {                                                                                    \
        POC_REBASE_STATIC_ASSERT(                                                           \
            POC_REBASE_LOCAL_INDEX(counter) >= 0 &&                                         \
                POC_REBASE_LOCAL_INDEX(counter) < POC_REBASE_ACTIVE_COUNT,                  \
            "trice bind: unexpected __COUNTER__ expansion inside an advanced scope; "      \
            "use trice insert/clean or change the macro");                                 \
        implementation(iD(POC_REBASE_ACTIVE_ID(POC_REBASE_LOCAL_INDEX(counter))),           \
                       __VA_ARGS__);                                                         \
    } while (0)

// The extra dispatch route is selected only between generated BEGIN and END.
#define TRICE_BIND_ROUTE_REBASE(implementation, ...) \
    POC_REBASE_CAPTURE(implementation, __COUNTER__, __VA_ARGS__)

#elif defined(TRICE_BIND_REBASE_BEGIN)

#if defined(POC_FORCE_NO_COUNTER) || !defined(__COUNTER__)
#error "trice bind: target preprocessor has no __COUNTER__; use trice insert/clean or change the macro"
#elif !defined(TRICE_BIND_REBASE_SCOPE)
#error "The generated rebase BEGIN requires TRICE_BIND_REBASE_SCOPE."
#elif defined(POC_REBASE_ACTIVE)
#error "Nested generated rebase scopes are not supported."
#else

// The base absorbs every unrelated counter expansion that happened earlier.
enum { POC_REBASE_BASE_NAME(TRICE_BIND_REBASE_SCOPE) = __COUNTER__ };
POC_REBASE_STATIC_ASSERT(POC_REBASE_SCOPE_COUNT(TRICE_BIND_REBASE_SCOPE) > 0,
                         "A generated rebase scope must contain at least one Trice site");

// Active macros deliberately refer to the scope token until the END include.
#define POC_REBASE_ACTIVE 1
#define POC_REBASE_ACTIVE_BASE POC_REBASE_BASE_NAME(TRICE_BIND_REBASE_SCOPE)
#define POC_REBASE_ACTIVE_COUNT POC_REBASE_SCOPE_COUNT(TRICE_BIND_REBASE_SCOPE)
#define POC_REBASE_ACTIVE_ID(index) POC_REBASE_SCOPE_ID(TRICE_BIND_REBASE_SCOPE, index)

#undef TRICE_BIND_ROUTE_K000000000000B101
#define TRICE_BIND_ROUTE_K000000000000B101 REBASE

#endif

#elif defined(TRICE_BIND_REBASE_END)

#if !defined(TRICE_BIND_REBASE_SCOPE) || !defined(POC_REBASE_ACTIVE)
#error "The generated rebase END has no matching active BEGIN."
#else

// The final snapshot detects missing sites and foreign counter use anywhere
// in the protected segment, including after its last Trice expansion.
enum { POC_REBASE_END_NAME(TRICE_BIND_REBASE_SCOPE) = __COUNTER__ };
POC_REBASE_STATIC_ASSERT(
    POC_REBASE_END_NAME(TRICE_BIND_REBASE_SCOPE) - POC_REBASE_ACTIVE_BASE - 1 ==
        POC_REBASE_ACTIVE_COUNT,
    "trice bind: generated scope consumed an unexpected number of __COUNTER__ values; "
    "use trice insert/clean or change the macro");

#undef TRICE_BIND_ROUTE_K000000000000B101
#define TRICE_BIND_ROUTE_K000000000000B101 BIND
#undef POC_REBASE_ACTIVE_ID
#undef POC_REBASE_ACTIVE_COUNT
#undef POC_REBASE_ACTIVE_BASE
#undef POC_REBASE_ACTIVE
#undef TRICE_BIND_REBASE_SCOPE

#endif

#endif
