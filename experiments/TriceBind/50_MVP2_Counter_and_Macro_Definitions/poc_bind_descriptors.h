// SPDX-License-Identifier: MIT

// This header models generator-owned sidecar output for the two independent PoCs.
#ifndef POC_BIND_MVP2_DESCRIPTORS_H_
#define POC_BIND_MVP2_DESCRIPTORS_H_

#if defined(POC_COUNTER_DESCRIPTORS) == defined(POC_WRAPPER_DESCRIPTORS)
#error "Select exactly one PoC descriptor set before including this header."
#endif

#ifdef POC_COUNTER_DESCRIPTORS

#undef TRICE_BIND_FILE_KEY
#define TRICE_BIND_FILE_KEY K000000000000C001
#define TRICE_BIND_ROUTE_K000000000000C001 BIND

// A generator/preprocessor pass observed counter values 0, 1, and 2 on line 100.
#define TRICE_BIND_SITE_K000000000000C001_L100_C0 TRICE_BIND_AUTO, iD(4101u)
#define TRICE_BIND_SITE_K000000000000C001_L100_C1 TRICE_BIND_AUTO, iD(4102u)
#define TRICE_BIND_SITE_K000000000000C001_L100_C2 TRICE_BIND_AUTO, iD(4103u)

// Counter value 3 belongs to an unrelated expansion; the next Trice sites use 4 and 5.
#define TRICE_BIND_SITE_K000000000000C001_L200_C4 TRICE_BIND_AUTO, iD(4104u)
#define TRICE_BIND_SITE_K000000000000C001_L200_C5 TRICE_BIND_AUTO, iD(4105u)

// A plain helper has only one descriptor for line 250, so both calls collide.
#define TRICE_BIND_SITE_K000000000000C001_L250 TRICE_BIND_AUTO, iD(4199u)

// A generated shadow source can carry explicit ordinals without using __COUNTER__.
#define TRICE_BIND_SITE_K000000000000C001_L260_O0 TRICE_BIND_AUTO, iD(4201u)
#define TRICE_BIND_SITE_K000000000000C001_L260_O1 TRICE_BIND_AUTO, iD(4202u)

#endif

#ifdef POC_WRAPPER_DESCRIPTORS

#undef TRICE_BIND_FILE_KEY
#define TRICE_BIND_FILE_KEY K000000000000A001
#define TRICE_BIND_ROUTE_K000000000000A001 BIND

// Every wrapper invocation expands both switch branches during preprocessing.
// IDs 5101 and 5102 belong to the two Trice definitions and remain stable.
#define TRICE_BIND_SITE_K000000000000A001_L300_C0 TRICE_BIND_AUTO, iD(5101u)
#define TRICE_BIND_SITE_K000000000000A001_L300_C1 TRICE_BIND_AUTO, iD(5102u)
#define TRICE_BIND_SITE_K000000000000A001_L301_C2 TRICE_BIND_AUTO, iD(5101u)
#define TRICE_BIND_SITE_K000000000000A001_L301_C3 TRICE_BIND_AUTO, iD(5102u)

#endif

#endif
