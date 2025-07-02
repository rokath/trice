/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DIRECT_OUTPUT 1 // trice l -p jlink -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0" -showID "deb:%04x" -hs off -pf cobs -d16=true
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_COBS
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_DEFERRED_OUTPUT 1 // trice l -p com4 -d16=false -pw MySecret -pf COBS
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA
#define TRICE_CGO 1
#define TRICE_CYCLE_COUNTER 0

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
