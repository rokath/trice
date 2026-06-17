#ifndef TRICE_BUS_CONFIG_H_
#define TRICE_BUS_CONFIG_H_

/*
 * triceBusConfig.h
 *
 * Settings that must be identical for all nodes participating in the same
 * Trice ABC demonstration bus.
 *
 * Node-specific triceConfig.h files include this file and map these bus settings
 * to the normal Trice configuration macros they need.
 */

/* The initial Trice ABC bus demo shall use TCOBS framing. */
#ifndef TRICE_BUS_FRAMING
#define TRICE_BUS_FRAMING TRICE_FRAMING_TCOBS
#endif

/* Prepared common bus variants. They are intentionally disabled for now. */
#ifndef TRICE_BUS_D16_ENABLE
#define TRICE_BUS_D16_ENABLE 0
#endif

#ifndef TRICE_BUS_XTEA_ENABLE
#define TRICE_BUS_XTEA_ENABLE 0
#endif

#endif /* TRICE_BUS_CONFIG_H_ */
