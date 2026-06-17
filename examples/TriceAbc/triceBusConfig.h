#ifndef TRICE_BUS_CONFIG_H_
#define TRICE_BUS_CONFIG_H_

/*
 * triceBusConfig.h
 *
 * Bus-wide settings for the TriceAbc example nodes.
 *
 * Every node-specific triceConfig.h includes this file and maps the values below
 * to the normal Trice configuration macros it needs. Settings placed here must
 * be identical for all nodes that participate in the same demonstration bus.
 */

/*
 * Framing mode used on the demonstration bus.
 *
 * Individual nodes map this value to their direct or deferred output framing
 * macro. Keeping the framing choice here prevents accidentally building nodes
 * that talk different byte-stream dialects.
 */
#ifndef TRICE_BUS_FRAMING
#define TRICE_BUS_FRAMING TRICE_FRAMING_TCOBS
#endif

/*
 * Prepared switch for a doubled-16-bit-ID demonstration variant.
 *
 * The initial demo keeps this disabled. It is defined here because all bus
 * participants must agree if this special mode is enabled later.
 */
#ifndef TRICE_BUS_D16_ENABLE
#define TRICE_BUS_D16_ENABLE 0
#endif

/*
 * Prepared switch for an encrypted bus demonstration variant.
 *
 * The initial demo keeps this disabled. Enabling encryption later must be a
 * bus-wide decision because every participant has to encode/decode the same
 * stream format.
 */
#ifndef TRICE_BUS_XTEA_ENABLE
#define TRICE_BUS_XTEA_ENABLE 0
#endif

#endif /* TRICE_BUS_CONFIG_H_ */
