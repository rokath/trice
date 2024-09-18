//! \file triceRingBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "cobs.h"
#include "tcobs.h"
#include "trice.h"

// lint -e715 Info 715: Symbol 'pFmt' (line 854, file ..\..\..\src\trice.h) not referenced
// lint -e528 Warning 528: Symbol 'Trice0(unsigned short, const char *)'  not referenced
// lint -e438 Warning 438: Last value assigned to variable 'singleTricesRingCount' not used.

#if TRICE_BUFFER == TRICE_RING_BUFFER && TRICE_OFF == 0

static int TriceSingleDeferredOut(uint32_t* addr);

#if TRICE_RING_BUFFER_OVERFLOW_WATCH == 1

#define TRICE_RING_BUFFER_LOWER_MARGIN 8 //!< 32-bit units just for debugging > 0
#define TRICE_RING_BUFFER_UPPER_MARGIN 8 //!< 32-bit units just for debugging > 0
#define TRICE_RING_BUFFER_MARGIN_FILL_VALUE 0xfee4deb

#else

#define TRICE_RING_BUFFER_LOWER_MARGIN 0 //!< 32-bit units just for debugging > 0
#define TRICE_RING_BUFFER_UPPER_MARGIN 0 //!< 32-bit units just for debugging > 0
#define TRICE_RING_BUFFER_FILL_VALUE 0

#endif

//! TriceRingBuffer is a kind of heap for trice messages. It needs to be initialized with 0.
uint32_t TriceRingBuffer[TRICE_RING_BUFFER_LOWER_MARGIN + (TRICE_DATA_OFFSET >> 2) + (TRICE_DEFERRED_BUFFER_SIZE >> 2) + TRICE_RING_BUFFER_UPPER_MARGIN] = {0};

uint32_t* const TriceRingBufferStart = TriceRingBuffer + TRICE_RING_BUFFER_LOWER_MARGIN + (TRICE_DATA_OFFSET >> 2);

//! triceBufferWriteLimit is the first address behind TriceRingBuffer.
//! With encryption it can happen that 4 bytes following triceRingBufferLimit are used as scratch pad.
//! We use the value of TRICE_DEFERRED_XTEA_ENCRYPT (0 or 1) here to respect that
//! See also comment inside TriceSingleDeferredOut.
uint32_t* const triceRingBufferLimit = TriceRingBufferStart + (TRICE_DEFERRED_BUFFER_SIZE >> 2) - TRICE_DEFERRED_XTEA_ENCRYPT;

//! SingleTricesRingCount holds the readable trices count inside TriceRingBuffer.
unsigned SingleTricesRingCount = 0;

//! TriceBufferWritePosition is used by the TRICE_PUT macros.
uint32_t* TriceBufferWritePosition = TriceRingBufferStart;

// ARM5 #pragma push
// ARM5 #pragma diag_suppress=170 //warning:  #170-D: pointer points outside of underlying object
//! TriceRingBufferReadPosition points to a valid trice message when singleTricesRingCount > 0.
//! This is first the TRICE_DATA_OFFSET byte space followed by the trice data.
//! Initially this value is set to TriceRingBufferStart minus TRICE_DATA_OFFSET byte space
//! to get a correct value for the very first call of triceNextRingBufferRead
// uint32_t* TriceRingBufferReadPosition = TriceRingBufferStart - (TRICE_DATA_OFFSET>>2); //lint !e428 Warning 428: negative subscript (-4) in operator 'ptr-int'
uint32_t* TriceRingBufferReadPosition = TriceRingBufferStart;
// ARM5 #pragma  pop

#if TRICE_DIAGNOSTICS == 1

//! SingleTricesRingCountMax holds the max count of trices occurred inside the ring buffer.
//! This value is only informal, because the length of the trice messages is not known.
// unsigned SingleTricesRingCountMax = 0;

//! TriceRingBufferDepthMax holds the max occurred ring buffer depth.
int TriceRingBufferDepthMax = 0;

#endif // #if TRICE_DIAGNOSTICS == 1

#if TRICE_PROTECT == 1

//! TriceEnoughSpace checks, if enough bytes available for the next trice.
//! \retval 0, when not enough space
//! \retval 1, when enough space
int TriceEnoughSpace(void) {
	// depth32 is the used buffer space in 32-bit words. After reset TriceBufferWritePosition and TriceRingBufferReadPosition are equal and depth32 is 0.
	// After some trice data writing, TriceBufferWritePosition is > TriceRingBufferReadPosition and depth32 has a positive value.
	// When trice data read out takes place, the TriceRingBufferReadPosition is incremented. That makes the depth32 value smaller again and
	// gives write space. During the trice write operation TriceBufferWritePosition gets incremented by max TRICE_SINGLE_MAX_SIZE bytes.
	// If afterwards the Trice BufferWritePosition is less TRICE_SINGLE_MAX_SIZE away from the buffer end, it is reset to the buffer start.
	// That means, there are max TRICE_SINGLE_MAX_SIZE-4 bytes wasted in the worst case. So the needed safety space is 2*TRICE_SINGLE_MAX_SIZE-4.
	// So, even there is a only 4 bytes Trice, it cannot be written, when no full safety space is left, because we do not know its size in advance.
	// The TriceRingBufferReadPosition jumps in the same way as the TriceBufferWritePosition. It wraps according the same rules.
	const int neededSafetySpace32 = (TRICE_DATA_OFFSET + ((2 * TRICE_SINGLE_MAX_SIZE) - 4)) >> 2;
	// Because a Trice can have size TRICE_SINGLE_MAX_SIZE, in that case, after writing it, we still need TRICE_DATA_OFFSET for the framing.
	int depth32 = TriceBufferWritePosition - TriceRingBufferReadPosition; // lint !e845 Info 845: The left argument to operator '<<' is certain to be 0
	if (depth32 < 0) {                                                    // After a TriceBufferWritePosition reset the difference is negative and needs correction to get the correct value.
		depth32 += (TRICE_DEFERRED_BUFFER_SIZE >> 2);
	}
	// This fn is called before an intended trice data write ande therefore additional at least neededSafetySpace32 32-bit words need to fit in the buffer.
	if (depth32 + neededSafetySpace32 <= (TRICE_DEFERRED_BUFFER_SIZE >> 2)) {
		return 1;
	} else {
#if TRICE_DIAGNOSTICS == 1
		TriceDeferredOverflowCount++;
#endif
		return 0;
	}
}

#endif // #if TRICE_PROTECT == 1

//! triceNextRingBufferRead returns a single trice data buffer address. The trice data are starting at byte offset TRICE_DATA_OFFSET from this address.
//! Implicit assumed is, that the pre-condition "SingleTricesRingCount > 0" is fulfilled.
//! \param lastWordCount is the u32 count of the last read trice including padding bytes.
//! The value lastWordCount is needed to increment TriceRingBufferReadPosition accordingly.
//! \retval is the address of the next trice data buffer.
static uint32_t* triceNextRingBufferRead(int lastWordCount) {
	TriceRingBufferReadPosition += /*(TRICE_DATA_OFFSET>>2) +*/ lastWordCount;
	if ((TriceRingBufferReadPosition + (TRICE_BUFFER_SIZE >> 2)) > triceRingBufferLimit) {
		TriceRingBufferReadPosition = TriceRingBufferStart;
	}

#if TRICE_DIAGNOSTICS == 1
	int depth = (TriceBufferWritePosition - TriceRingBufferReadPosition) << 2; // lint !e845 Info 845: The left argument to operator '<<' is certain to be 0
	if (depth < 0) {
		depth += TRICE_DEFERRED_BUFFER_SIZE;
	}
	TriceRingBufferDepthMax = (depth > TriceRingBufferDepthMax) ? depth : TriceRingBufferDepthMax;
#endif // #if TRICE_DIAGNOSTICS == 1

	return TriceRingBufferReadPosition; // lint !e674 Warning 674: Returning address of auto through variable 'TriceRingBufferReadPosition'
}

//! TriceTransfer needs to be called cyclically to read out the Ring Buffer.
void TriceTransfer(void) {
	if (SingleTricesRingCount == 0) { // no data
		return;
	}
#if TRICE_CGO == 0         // In automated tests we assume last transmission is finished, so we do not test depth to be able to test multiple Trices in deferred mode.
	if (TriceOutDepth()) { // last transmission not finished
		return;
	}
#endif
	TRICE_ENTER_CRITICAL_SECTION
	SingleTricesRingCount--;
	TRICE_LEAVE_CRITICAL_SECTION
	static int lastWordCount = 0;
	uint32_t* addr = triceNextRingBufferRead(lastWordCount);
	lastWordCount = TriceSingleDeferredOut(addr);
}

//! TriceIDAndBuffer evaluates a trice message and returns the ID for routing.
//! \param pData is where the trice message starts.
//! \param pWordCount is filled with the word count the trice data occupy from pData.
//! \param ppTriceNetStart is filled with the trice net data start. That is maybe a 2 bytes offset from pData.
//! \param pTriceNetLength is filled with the net trice length (without padding bytes), 0 on error.
//! \retval is the triceID, a positive value on success or error information.
static int TriceIDAndBuffer(const uint32_t* const pData, int* pWordCount, uint8_t** ppTriceNetStart, size_t* pTriceNetLength) {
	uint16_t TID = TRICE_TTOHS(*(uint16_t*)pData); // type and id
	int triceID = 0x3FFF & TID;
	int triceType = TID >> 14; // 2 bits
	unsigned offset;
	size_t len;
	uint8_t* pStart = (uint8_t*)pData;
	switch (triceType) {
	case TRICE_TYPE_S0: // S0 = no stamp
		offset = 0;
		len = 4 + triceDataLen(pStart + 2); // tyId
		break;
	case TRICE_TYPE_S2:                     // S2 = 16-bit stamp
		len = 6 + triceDataLen(pStart + 6); // tyId ts16
		offset = 2;
#if TRICE_DEFERRED_XTEA_ENCRYPT
		// move trice to start at a uint32_t alignment border
		memmove(pStart, pStart + 2, len); // https://stackoverflow.com/questions/1201319/what-is-the-difference-between-memmove-and-memcpy
#else                                     // #if TRICE_DEFERRED_XTEA_ENCRYPT
		// Like for UART transfer no uint32_t alignment is needed.
		pStart += 2; // see Id(n) macro definition
#endif                                    // #else // #if TRICE_DEFERRED_XTEA_ENCRYPT
		break;
	case TRICE_TYPE_S4: // S4 = 32-bit stamp
		offset = 0;
		len = 8 + triceDataLen(pStart + 6); // tyId ts32
		break;
	default: // impossible case (triceType has only 2 bits)
	         // fall thru
	case TRICE_TYPE_X0:
		TriceErrorCount++;
		*ppTriceNetStart = pStart;
		*pTriceNetLength = 0;
		return -__LINE__; // extended trices not supported (yet)
	}
	// S16 case example:            triceSize  len   t-0-3   t-o
	// 80id 80id 1616 00cc                8     6      3      6
	// 80id 80id 1616 01cc dd            12     7      7     10
	// 80id 80id 1616 02cc dd dd         12     8      7     10
	// 80id 80id 1616 03cc dd dd dd      12     9      7     10
	// 80id 80id 1616 04cc dd dd dd dd   12    10      7     10
	*pWordCount = (len + offset + 3) >> 2;
	*ppTriceNetStart = pStart;
	*pTriceNetLength = len;
	return triceID;
}

//! TriceSingleDeferredOut expects a single trice at addr and returns the wordCount of this trice which includes 1-3 padding bytes.
//! The space from addr-TRICE_DATA_OFFSET to add is assumed to be usable as scratch pad.
//! This function is specific to the ring buffer, because the wordCount value needs to be reconstructed.
//! \param addr points to the begin of a single trice.
//! \retval The returned value tells how many words where used by the transmitted trice and is usable for the memory management. See RingBuffer for example.
//! The returned value is typically (TRICE_DATA_OFFSET/4) plus 1 (4 bytes) to 3 (9-12 bytes) but could go up to ((TRICE_DATA_OFFSET/4)+(TRICE_BUFFER_SIZE/4)).
//! Return values <= 0 signal an error.
//! The data at addr are getting destroyed, because buffer is used as scratch pad.
static int TriceSingleDeferredOut(uint32_t* addr) {
	uint8_t* enc = ((uint8_t*)addr) - TRICE_DATA_OFFSET; // TRICE_DATA_OFFSET bytes are usable in front of addr.
	int wordCount;
	uint8_t* pTriceNetStart;
	size_t triceNetLength; // without padding bytes
	int triceID = TriceIDAndBuffer(addr, &wordCount, &pTriceNetStart, &triceNetLength);
	// We can let TRICE_DATA_OFFSET only in front of the ring buffer and pack the Trices without offset space.
	// And if we allow as max depth only ring buffer size minus TRICE_DATA_OFFSET, we can use space in front of each Trice.

#if (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE)
#if TRICE_CONFIG_WARNINGS == 1
#warning configuration: The Trice tool does not support encryption without COBS (or TCOBS) framing.
#endif
	size_t encLen = TriceEncode(TRICE_DEFERRED_XTEA_ENCRYPT, TRICE_DEFERRED_OUT_FRAMING, enc, pTriceNetStart, triceNetLength);
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE)
	// comment: The following 2 steps could be done in an incremental way within one singe loop.
	// Behind the trice brutto length (with padding bytes), 4 bytes needed as scratch pad when XTEA is active.
	// After TriceIDAndBuffer pTriceNetStart could have a 2 bytes offset.
	uint8_t* tmp = ((uint8_t*)addr) - 4;
	memmove(tmp, pTriceNetStart, triceNetLength);
	size_t len8 = (triceNetLength + 7) & ~7;                // Only multiple of 8 are possible to encrypt, so we adjust len.
	memset(tmp + triceNetLength, 0, len8 - triceNetLength); // clear padding space
	XTEAEncrypt((uint32_t*)tmp, len8 >> 2);
#if TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS
	size_t encLen = (size_t)TCOBSEncode(enc, tmp, len8);
#elif TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS
	size_t encLen = (size_t)COBSEncode(enc, tmp, len8);
#else
#error configuration
#endif
	enc[encLen++] = 0; // Add zero as package delimiter.
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE)
	size_t len = (size_t)TCOBSEncode(enc, pTriceNetStart, triceNetLength);
	enc[len++] = 0; // Add zero as package delimiter.
	size_t encLen = len;
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE)
	size_t len = (size_t)COBSEncode(enc, pTriceNetStart, triceNetLength);
	enc[len++] = 0; // Add zero as package delimiter.
	size_t encLen = len;
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE)
	enc = pTriceNetStart;
	size_t encLen = triceNetLength;
#else
#error configuration: TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE for ring buffer not implemented yet
#endif
#if TRICE_DIAGNOSTICS == 1
	// enc                 addr  pTriceNetStart           nextData
	// ^-TRICE_DATA_OFFSET-^-0|2-^-triceNetLength+(0...3)-^
	// ^-encLen->firstNotModifiedAddress
	uint8_t* nextData = (uint8_t*)(((uintptr_t)(pTriceNetStart + triceNetLength + 3)) & ~3);
	uint8_t* firstNotModifiedAddress = enc + encLen;
	int distance = nextData - firstNotModifiedAddress;
	int triceDataOffsetDepth = TRICE_DATA_OFFSET - distance; // distance could get > TRICE_DATA_OFFSET, so TriceDataOffsetDepthMax stays unchanged then.
	TriceDataOffsetDepthMax = triceDataOffsetDepth < TriceDataOffsetDepthMax ? TriceDataOffsetDepthMax : triceDataOffsetDepth;
#endif // #if TRICE_DIAGNOSTICS == 1
	TriceNonBlockingDeferredWrite8(triceID, enc, encLen);
	return wordCount;
}

#if TRICE_RING_BUFFER_OVERFLOW_WATCH == 1

void TriceInitRingBufferMargins(void) {
	for (int i = 0; i < TRICE_RING_BUFFER_LOWER_MARGIN; i++) {
		TriceRingBuffer[i] = TRICE_RING_BUFFER_MARGIN_FILL_VALUE;
	}
	for (int i = 0; i < TRICE_RING_BUFFER_UPPER_MARGIN; i++) {
		*(triceRingBufferLimit + i) = TRICE_RING_BUFFER_MARGIN_FILL_VALUE;
	}
}

void WatchRingBufferMargins(void) {
	for (int i = 0; i < TRICE_RING_BUFFER_LOWER_MARGIN; i++) {
		if (TriceRingBuffer[i] != TRICE_RING_BUFFER_MARGIN_FILL_VALUE) {
			for (;;)
				;
		}
	}

	for (int i = 0; i < TRICE_RING_BUFFER_UPPER_MARGIN; i++) {
		if (*(triceRingBufferLimit + i) != TRICE_RING_BUFFER_MARGIN_FILL_VALUE) {
			for (;;)
				;
		}
	}
}

#endif // #if TRICE_RING_BUFFER_OVERFLOW_WATCH == 1

#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER && TRICE_OFF == 0
