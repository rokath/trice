/*! \file triceBareFifo.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_BARE_FIFO_H_
#define TRICE_BARE_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

//! triceServeBareFifoOut must be called cyclically like every 1-100 ms for examle in main loop or a background task.
void triceServeBareFifoOut(void);

extern int triceBareFifoMaxDepthTrices; //!< usabble for diagnostics

//! TRICE_BARE_SYNC_LEVEL is the max amount of trices bulk transmitted without a sync trice injected.
//! Assuming triceServeBareFifoOut() is called every ms this is also the max ms abount without a sync trice.
//! Bigger numbers decrease the bandwidth needs but increase the possible loss of trices in case of
//! re-sync need. A number of 10-100 is recommended: every 10-100 ms or 10-100 trices a sync trice is transmitted
//! increasing the bandwidth need by 10-1% ans loosing 10 trices in the worst case.
#define TRICE_BARE_SYNC_LEVEL 10


#ifdef __cplusplus
}
#endif

#endif /* TRICE_BARE_FIFO_H_ */
