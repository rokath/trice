//! \file triceDoubleBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "cobs.h"
#include "tcobs.h"
#include "trice.h"

#if TRICE_BUFFER == TRICE_DOUBLE_BUFFER && TRICE_OFF == 0

static void TriceOut(uint32_t* tb, size_t tLen);

//! triceBuffer is a double buffer for better write speed.
//! halfBufferStart     writePosition
//! ^-TRICE_DATA_OFFSET-^-restOf_TRICE_DEFERRED_BUFFER_SIZE-^-2ndBuf...
//! ^-TRICE_DATA_OFFSET-^-restOf_TRICE_DEFERRED_BUFFER_SIZE-Limit
static uint32_t triceBuffer[2][TRICE_DEFERRED_BUFFER_SIZE >> 3] = {0};

static uint32_t* const triceWriteStartHalfBuffer0 = &triceBuffer[0][TRICE_DATA_OFFSET >> 2];
static uint32_t* const triceWriteLimitHalfBuffer0 = &triceBuffer[0][TRICE_DEFERRED_BUFFER_SIZE >> 3];
static uint32_t* const triceWriteStartHalfBuffer1 = &triceBuffer[1][TRICE_DATA_OFFSET >> 2];
static uint32_t* const triceWriteLimitHalfBuffer1 = &triceBuffer[1][TRICE_DEFERRED_BUFFER_SIZE >> 3];

//! triceActiveHalfBuffer is the index of the active write buffer. !triceActiveHalfBuffer is the active read buffer index.
static int triceActiveHalfBuffer = 0;

//! TriceBufferWritePosition is the active write position and is used by TRICE_PUT macros.
uint32_t* TriceBufferWritePosition = triceWriteStartHalfBuffer0;

//! TriceBufferWritePositionStart is the begin of the active write buffer.
uint32_t* TriceBufferWritePositionStart = triceWriteStartHalfBuffer0;

#if TRICE_PROTECT == 1
//! TriceBufferWritePositionLimit is the first not usable address of the current written half buffer.
static uint32_t* TriceBufferWritePositionLimit = triceWriteLimitHalfBuffer0;

//! TriceEnoughSpace checks, if at least TRICE_SINGLE_MAX_SIZE bytes available for the next trice.
//! \retval 0, when not enough space
//! \retval 1, when enough space
int TriceEnoughSpace(void) {
	// space32 is the writable 32-bit value count in the current write buffer.
	int space32 = TriceBufferWritePositionLimit - TriceBufferWritePosition;
	// there need to be at least TRICE_SINGLE_MAX_SIZE bytes space in the current write buffer.
	if (space32 >= (TRICE_SINGLE_MAX_SIZE >> 2)) {
		return 1;
	} else {
#if TRICE_DIAGNOSTICS == 1
		TriceDeferredOverflowCount++;
#endif
		return 0;
	}
}

#endif // #if TRICE_PROTECT == 1

//! triceBufferSwap swaps the trice double buffer and returns the read buffer address.
static uint32_t* triceBufferSwap(void) {
	if (triceActiveHalfBuffer == 0) {
		triceActiveHalfBuffer = 1;
		TriceBufferWritePositionStart = triceWriteStartHalfBuffer1;
		TriceBufferWritePosition = TriceBufferWritePositionStart;
#if TRICE_PROTECT == 1
		TriceBufferWritePositionLimit = triceWriteLimitHalfBuffer1;
#endif
		return triceWriteStartHalfBuffer0;
	} else {
		triceActiveHalfBuffer = 0;
		TriceBufferWritePositionStart = triceWriteStartHalfBuffer0;
		TriceBufferWritePosition = TriceBufferWritePositionStart;
#if TRICE_PROTECT == 1
		TriceBufferWritePositionLimit = triceWriteLimitHalfBuffer0;
#endif
		return triceWriteStartHalfBuffer1;
	}
}

#if TRICE_DIAGNOSTICS == 1
//! TriceHalfBufferDepthMax is a diagnostics value usable to optimize buffer size.
unsigned TriceHalfBufferDepthMax = 0;
#endif

//! TriceTransfer, if possible, swaps the double buffer and initiates a write.
//! It is the responsibility of the app to call this function once every 10-100 milliseconds.
void TriceTransfer(void) {
	if (0 == TriceOutDepth()) { // transmission done for slowest output channel, so a swap is possible.
		uint32_t* readBuf;
		size_t tLen32;
		TRICE_ENTER_CRITICAL_SECTION
		tLen32 = TriceBufferWritePosition - TriceBufferWritePositionStart;
		if (tLen32) { // Some Trice data are available.
			readBuf = triceBufferSwap();
			readBuf -= (TRICE_DATA_OFFSET >> 2);
		}
		TRICE_LEAVE_CRITICAL_SECTION
		if (tLen32) {
			TriceOut(readBuf, tLen32 << 2);
		}
	}
}

//! TriceNext expects at *buf 32-bit aligned trice messages and returns the next one in pStart and pLen.
//! \todo: use this function only when MULTI
//! \param buf points to 32-bit aligned trice messages and is filled with the advanced buf.
//! \param pSize contains the total size of all following trices including padding bytes to 32-bit alignments and gets the by next trice reduced value.
//! \param pStart points to the net start of the next trice.
//! \param pLen contains the net length of the first trice.
//! \retval is the trice ID on success or negative on error.
//! The trices inside the double buffer start 32-bit aligned with maybe 1-3 padding bytes in-between.
//! \li |    ... |    trice   | trice | ... |  trice |
//! \li before:  ^- buf
//! \li before:  <-------------- pSize -------------->
//! \li after:                 ^- buf
//! \li after:                 <-------- pSize ------>
//! \li after:     ^- pStart    (is input buf or input buf + 2)
//! \li after:     <- pLen ->   (maybe 1-3 bytes shorter)
static int TriceNext(uint8_t** buf, size_t* pSize, const uint8_t** pStart, size_t* pLen) {
	uint16_t* pTID = (uint16_t*)*buf;  // lint !e826, get TID address
	unsigned TID = TRICE_TTOHS(*pTID); // type and id
	int triceID = 0x3FFF & TID;
	int triceType = TID >> 14;
	unsigned offset;
	size_t size = *pSize;
	size_t triceSize;
	size_t len;
	switch (triceType) {
	case TRICE_TYPE_S0: // S0 = no stamp
		*pStart = *buf;
		offset = 0;
		len = 4 + triceDataLen(*pStart + 2); // tyId
		break;
	case TRICE_TYPE_S2: // S2 = 16-bit stamp
		*pStart = *buf; // see Id(n) macro definition
		*pStart += 2;   // see Id(n) macro definition
		offset = 2;
		len = 6 + triceDataLen(*pStart + 4); // tyId ts16
		break;
	case TRICE_TYPE_S4: // S4 = 32-bit stamp
		*pStart = *buf;
		offset = 0;
		len = 8 + triceDataLen(*pStart + 6); // tyId ts32
		break;
	default: // impossible
	         // fallthrugh
	case TRICE_TYPE_X0:
		TriceErrorCount++;
		*pStart = 0;
		*pLen = 0;
		return -__LINE__; // extended trices not supported (yet)
	}
	triceSize = (len + offset + 3) & ~3;
	// S16 case example:            triceSize  len   t-0-3   t-o
	// 80id 80id 1616 00cc                8     6      3      6
	// 80id 80id 1616 01cc dd            12     7      7     10
	// 80id 80id 1616 02cc dd dd         12     8      7     10
	// 80id 80id 1616 03cc dd dd dd      12     9      7     10
	// 80id 80id 1616 04cc dd dd dd dd   12    10      7     10
	if (!(triceSize - (offset + 3) <= len && len <= triceSize - offset)) { // corrupt data
		TriceErrorCount++;
		return -__LINE__;
	}
	size -= triceSize;
	*buf += triceSize;
	*pSize = size;
	*pLen = len;
	return triceID;
}

uint8_t* firstNotModifiedAddress;
int distance;
int triceDataOffsetDepth;

//! TriceOut encodes trices and writes them in one step to the output.
//! This function is called only, when the slowest deferred output device has finished its last buffer.
//! At the half buffer start tb are TRICE_DATA_OFFSET bytes space followed by a number of trice messages which all contain
//! 0-3 padding bytes and therefore have a length of a multiple of 4. There is no additional space between these trice messages.
//! When XTEA enabled, only (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE) is allowed, because the 4 bytes behind a trice messages
//! are changed, when the trice length is not a multiple of 8, but only of 4. (XTEA can encrypt only multiple of 8 lenth packages.)
//! \param tb is start of uint32_t* trice buffer. The space TRICE_DATA_OFFSET at the tb start is for in-buffer encoding of the trice data.
//! \param tLen is total length of several trice data. It is always a multiple of 4 because of 32-bit alignment and padding bytes.
static void TriceOut(uint32_t* tb, size_t tLen) {
	uint8_t* enc = (uint8_t*)tb;            // This is the later encoded data starting address.
	uint8_t* dat = enc + TRICE_DATA_OFFSET; // Thid is the start of      32-bit aligned trices.
	uint8_t* nxt = dat;                     // Thid is the start of next 32-bit aligned trices.
	size_t encLen = 0;
	uint8_t* dst = enc; // This value dst must not get > nxt to avoid overwrites.
	int triceID = 0;    // This assignment is only needed to silence compiler complains about being uninitialized.
#if TRICE_DIAGNOSTICS == 1
	unsigned depth = tLen + TRICE_DATA_OFFSET;
	TriceHalfBufferDepthMax = depth < TriceHalfBufferDepthMax ? TriceHalfBufferDepthMax : depth;
#endif
	// do it
	while (tLen) {
#if TRICE_DIAGNOSTICS == 1
		firstNotModifiedAddress = enc + encLen;
		distance = nxt - firstNotModifiedAddress;
		triceDataOffsetDepth = TRICE_DATA_OFFSET - distance; // distance can get > TRICE_DATA_OFFSET, but that is no problem.
		TriceDataOffsetDepthMax = triceDataOffsetDepth < TriceDataOffsetDepthMax ? TriceDataOffsetDepthMax : triceDataOffsetDepth;
#endif // #if TRICE_DIAGNOSTICS == 1
		const uint8_t* triceNettoStart;
		size_t triceNettoLen; // This is the trice netto length (without padding bytes).
#if (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING != TRICE_FRAMING_NONE) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE)
		uint8_t* crypt = nxt - 4; // only 8-byte groups are encryptable
#endif
		triceID = TriceNext(&nxt, &tLen, &triceNettoStart, &triceNettoLen);
		if (triceID <= 0) { // on data error
			TriceErrorCount++;
			break; // ignore following data
		}
#if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE
		uint8_t* dst = enc + encLen;
#if (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS)
		memmove(crypt, triceNettoStart, triceNettoLen);
		size_t len8 = (triceNettoLen + 7) & ~7;                   // Only multiple of 8 encryptable, so we adjust len.
		memset((crypt) + triceNettoLen, 0, len8 - triceNettoLen); // Clear padding space.
		XTEAEncrypt((uint32_t*)crypt, len8 >> 2);
		encLen += (size_t)TCOBSEncode(dst, crypt, len8); // encLen is re-used here
		dst[encLen++] = 0;                               // Add zero as package delimiter.
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS)
		memmove(crypt, triceNettoStart, triceNettoLen);
		size_t len8 = (triceNettoLen + 7) & ~7;                   // Only multiple of 8 encryptable, so we adjust len.
		memset((crypt) + triceNettoLen, 0, len8 - triceNettoLen); // Clear padding space.
		XTEAEncrypt((uint32_t*)crypt, len8 >> 2);
		encLen += (size_t)COBSEncode(dst, crypt, len8); // encLen is re-used here
		dst[encLen++] = 0;                              // Add zero as package delimiter.
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE)
#if TRICE_CONFIG_WARNINGS == 1
#error configuration: The Trice tool does not support encryted data without COBS or TCOBS framing.
#endif
		// memmove(dst, triceNettoStart, triceNettoLen );
		// size_t len8 = (triceNettoLen + 7) & ~7; // Only multiple of 8 encryptable, so we adjust len.
		// memset((enc)+triceNettoLen, 0, len8 - triceNettoLen); // Clear padding space.
		// XTEAEncrypt( (uint32_t *)enc, len8>>2 );
		// encLen += len8; // encLen is re-used here
		// nextPosForEncoding = dst+encLen;
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS)
		size_t len = (size_t)TCOBSEncode(dst, triceNettoStart, triceNettoLen);
		dst[len++] = 0; // Add zero as package delimiter.
		encLen += len;
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS)
		size_t len = (size_t)COBSEncode(dst, triceNettoStart, triceNettoLen);
		dst[len++] = 0; // Add zero as package delimiter.
		encLen += len;
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE)
		memmove(dst, triceNettoStart, triceNettoLen);
		encLen += triceNettoLen;
#else
#error configuration: unexpected
#endif
#elif TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE
		// pack data
		uint8_t* packed = dat + encLen;                  // After the loop, the packed data start at dat.
		memmove(packed, triceNettoStart, triceNettoLen); // This action removes all padding bytes of the trices, compacting their sequence this way
		encLen += triceNettoLen;
#endif // #elif  TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE

		dst = enc + encLen; // When several Trices in the double buffer, with each encoding the new dst could drift a bit closer towards triceNettoStart.

#if (TRICE_PROTECT == 1) || (TRICE_DIAGNOSTICS == 1)
		int triceDataOffsetSpaceRemained = nxt - dst; // THe begin of unprocessed data MINUS next dst must not be negative.
#endif
#if (TRICE_PROTECT == 1)
		if (triceDataOffsetSpaceRemained < 0) {
			TriceErrorCount++;
			return; // discard broken data to avoid buffer overflow
		}
#endif
#if TRICE_DIAGNOSTICS == 1
		int triceDataOffsetDepth = TRICE_DATA_OFFSET - triceDataOffsetSpaceRemained;
		TriceDataOffsetDepthMax = triceDataOffsetDepth < TriceDataOffsetDepthMax ? TriceDataOffsetDepthMax : triceDataOffsetDepth;
#endif
	}
#if TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE
// At this point the compacted trice messages start TRICE_DATA_OFFSET bytes after tb (now dat) and the encLen is their total netto length.
// Behind this up to 7 bytes can be used as scratch pad when XTEA is active. That is ok, because the half buffer should not get totally filled.
// encLen = TriceEncode( TRICE_DEFERRED_XTEA_ENCRYPT, TRICE_DEFERRED_OUT_FRAMING, enc, dat, encLen );
#if (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS) // && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)
	// special case: The data are at dat and can be big, are compacted and behind them is space. So we can encrypt them in space
	size_t len8 = (encLen + 7) & ~7;                    // Only multiple of 8 encryptable, so we adjust len.
	memset(((uint8_t*)dat) + encLen, 0, len8 - encLen); // clear padding space: ATTENTION! OK only for this compiler switch setting.
	XTEAEncrypt((uint32_t*)dat, len8 >> 2);
	size_t eLen = (size_t)TCOBSEncode(enc, dat, len8);                                          // encLen is re-used here
	enc[eLen++] = 0;                                                                            // Add zero as package delimiter.
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS)  // && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)
	// special case: The data are at dat and can be big, are compacted and behind them is space. So we can encrypt them in space
	size_t len8 = (encLen + 7) & ~7;                    // Only multiple of 8 encryptable, so we adjust len.
	memset(((uint8_t*)dat) + encLen, 0, len8 - encLen); // clear padding space: ATTENTION! OK only for this compiler switch setting.
	XTEAEncrypt((uint32_t*)dat, len8 >> 2);
	size_t eLen = (size_t)COBSEncode(enc, dat, len8); // encLen is re-used here
	enc[eLen++] = 0;                                  // Add zero as package delimiter.
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE)  // && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)
	size_t eLen = TriceEncode(TRICE_DEFERRED_XTEA_ENCRYPT, TRICE_DEFERRED_OUT_FRAMING, enc, dat, encLen);
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS) // && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)
	size_t eLen = TCOBSEncode(enc, dat, encLen);
	enc[eLen++] = 0; // Add zero as package delimiter.
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS)  // && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)
	size_t eLen = (size_t)COBSEncode(enc, dat, encLen);
	enc[eLen++] = 0; // Add zero as package delimiter.
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE)  // && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)
	enc = dat;
	size_t eLen = encLen;
#else
#error configuration:
#endif
#if TRICE_DIAGNOSTICS
	// before: space = enc[TRICE_DATA_OFFSET], data = dat[encLen]
	// after:  date  = enc[eLen], (dat [encLen])
	// Mostly eLen < encLen, but it could be eLen = encLen + 1 + (encLen>>5) in TCOBS worst case.
	// dat - enc = TRICE_DATA_OFFSET
	// if eLen > enclen, then TriceDataOffsetDepth = eLen - encLen
	int triceDataOffsetDepth = eLen - encLen; // usually negative
	TriceDataOffsetDepthMax = triceDataOffsetDepth < TriceDataOffsetDepthMax ? TriceDataOffsetDepthMax : triceDataOffsetDepth;
#endif
	encLen = eLen;
#endif

	// Reaching here means all trice data in the current half buffer are encoded
	// into a single continuous buffer having 0-delimiters between them or not but at the ent is a 0-delimiter.
	//
	// output
	TRICE_ENTER_CRITICAL_SECTION
	TriceNonBlockingDeferredWrite8(triceID, enc, encLen); // lint !e771 Info 771: Symbol 'triceID' conceivably not initialized. Comment: tLen is always > 0.
	TRICE_LEAVE_CRITICAL_SECTION
}

#endif // #if TRICE_BUFFER == TRICE_DOUBLE_BUFFER && TRICE_OFF == 0
