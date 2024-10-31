//! \file triceRingBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "cobs.h"
#include "tcobs.h"
#include "trice.h"

#if TRICE_BUFFER == TRICE_RING_BUFFER && TRICE_OFF == 0

#if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE
static void triceSingleDeferredOut(int* wordCount);
#else
static void triceMultiDeferredOut(int* triceCount, int* wordCount);
#endif

#if TRICE_RING_BUFFER_OVERFLOW_WATCH == 1

#define TRICE_RING_BUFFER_LOWER_MARGIN 8 //!< 32-bit units just for debugging > 0
#define TRICE_RING_BUFFER_UPPER_MARGIN 8 //!< 32-bit units just for debugging > 0
#define TRICE_RING_BUFFER_MARGIN_FILL_VALUE 0xfee4deb

#else

#define TRICE_RING_BUFFER_LOWER_MARGIN 0 //!< 32-bit units just for debugging > 0
#define TRICE_RING_BUFFER_UPPER_MARGIN 0 //!< 32-bit units just for debugging > 0
#define TRICE_RING_BUFFER_FILL_VALUE 0

#endif

//! triceRingBuffer is a kind of heap for trice messages. It needs to be initialized with 0.
//! Initial:
//! |<-LM->|<----------------  triceRingBuffer  ------------------------------------------------------->|<-UM->|
//! |<-LM->|<--TRICE_DATA_OFFSET-->|<---------------- TRICE_RING_BUFFER_SIZE -------------------------->|<-UM->|
//! |<-LM->|<--TRICE_DATA_OFFSET-->|<--        writable        -->|<-- TRICE_RING_BUFFER_MIN_SPACE32 -->|<-UM->|
//!                                |                              |                                     ^--------- TriceRingBufferLimit
//!                                |                              ^----------------------------------------------- TriceRingBufferProtectLimit
//!                                ^------------------------------------------------------------------------------ TriceBufferWritePosition
//!                                ^------------------------------------------------------------------------------ TriceRingBufferReadPosition
//!                                ^------------------------------------------------------------------------------ TriceRingBufferStart
//!
//! After some writes:
//! |<-LM->|<----------------  triceRingBuffer  ------------------------------------------------------->|<-UM->|
//! |<-LM->|<--TRICE_DATA_OFFSET-->|<---------------- TRICE_RING_BUFFER_SIZE -------------------------->|<-UM->|
//! |<-LM->|<--TRICE_DATA_OFFSET-->|<-T->|<-T->|<--  writable  -->|<-- TRICE_RING_BUFFER_MIN_SPACE32 -->|<-UM->|
//!                                |           |                  |                                     ^--------- TriceRingBufferLimit
//!                                |           |                  ^----------------------------------------------- TriceRingBufferProtectLimit
//!                                |           ^------------------------------------------------------------------ TriceBufferWritePosition
//!                                ^------------------------------------------------------------------------------ TriceRingBufferReadPosition
//!                                ^------------------------------------------------------------------------------ TriceRingBufferStart
//!
//! After some reads:
//! |<-LM->|<----------------  triceRingBuffer  ------------------------------------------------------->|<-UM->|
//! |<-LM->|<--TRICE_DATA_OFFSET-->|<---------------- TRICE_RING_BUFFER_SIZE -------------------------->|<-UM->|
//! |<-LM->|<--TRICE_DATA_OFFSET-->|<writable->|<--  writable  -->|<-- TRICE_RING_BUFFER_MIN_SPACE32 -->|<-UM->|
//!                                |           |                  |                                     ^--------- TriceRingBufferLimit
//!                                |           |                  ^----------------------------------------------- TriceRingBufferProtectLimit
//!                                |           ^------------------------------------------------------------------ TriceBufferWritePosition
//!                                |           ^-------------------------------------------------------------------TriceRingBufferReadPosition
//!                                ^------------------------------------------------------------------------------ TriceRingBufferStart
//!
//! After some time:
//! |<-LM->|<----------------  triceRingBuffer  ------------------------------------------------------->|<-UM->|
//! |<-LM->|<--TRICE_DATA_OFFSET-->|<---------------- TRICE_RING_BUFFER_SIZE -------------------------->|<-UM->|
//! |<-LM->|<--TRICE_DATA_OFFSET-->|                              |<-- TRICE_RING_BUFFER_MIN_SPACE32 -->|<-UM->|
//!                                |                              |                                     ^--------- TriceRingBufferLimit
//!                                |                              ^----------------------------------------------- TriceRingBufferProtectLimit
//!                                |<-- writable -->|<-T->|<-T->|<-T->|<-------- not writable --------->|<-UM->|
//!                                |                |                 ^------------------------------------------- TriceBufferWritePosition, no more TRICE_RING_BUFFER_MIN_SPACE32!
//!                                |                ^------------------------------------------------------------- TriceRingBufferReadPosition
//!                                ^------------------------------------------------------------------------------ TriceRingBufferStart
//! With next Trice, TriceBufferWritePosition has to wrap firstly:
//! |<-LM->|<----------------  triceRingBuffer  ------------------------------------------------------->|<-UM->|
//! |<-LM->|<--TRICE_DATA_OFFSET-->|<---------------- TRICE_RING_BUFFER_SIZE -------------------------->|<-UM->|
//! |<-LM->|<--TRICE_DATA_OFFSET-->|                              |<-- TRICE_RING_BUFFER_MIN_SPACE32 -->|<-UM->|
//!                                |                              |                                     ^--------- TriceRingBufferLimit
//!                                |                              ^----------------------------------------------- TriceRingBufferProtectLimit
//!                                |<-- writable -->|<-T->|<-T->|<-T->|<-------- not writable --------->|<-UM->|   When depth32(==writable) < TRICE_RING_BUFFER_MIN_SPACE32, no write is possible.
//!                                |                ^------------------------------------------------------------- TriceRingBufferReadPosition
//!                                ^------------------------------------------------------------------------------ TriceBufferWritePosition (wrapped)
//!                                ^------------------------------------------------------------------------------ TriceRingBufferStart
uint32_t triceRingBuffer[TRICE_RING_BUFFER_LOWER_MARGIN + (TRICE_DATA_OFFSET >> 2) + (TRICE_RING_BUFFER_SIZE >> 2) + TRICE_RING_BUFFER_UPPER_MARGIN] = {0};

//! TRICE_RING_BUFFER_MIN_SPACE32 is the needed space before a new Trice is allowed to be written.
//! The TRICE_DATA_OFFSET guaranties, that in front of TriceRingBufferReadPosition is always a scratch pad.
#define TRICE_RING_BUFFER_MIN_SPACE32 ((TRICE_SINGLE_MAX_SIZE + TRICE_DATA_OFFSET) >> 2)

//! TRICE_RING_BUFFER_START is a helper definition to avoid warning "expression must have a constant value".
#define TRICE_RING_BUFFER_START (triceRingBuffer + TRICE_RING_BUFFER_LOWER_MARGIN + (TRICE_DATA_OFFSET >> 2))

//! TriceRingBufferStart is an immutable pointer to the begin of the triceRingBuffer area.
uint32_t* const TriceRingBufferStart = TRICE_RING_BUFFER_START;

//! triceBufferWriteLimit is the first address behind triceRingBuffer area and immutable.
//! With encryption it can happen that 4 bytes following TriceRingBufferLimit are used as scratch pad.
//! We use the value of TRICE_DEFERRED_XTEA_ENCRYPT (0 or 1) here to respect that
//! See also comment inside triceSingleDeferredOut.
uint32_t* const TriceRingBufferLimit = TRICE_RING_BUFFER_START + (TRICE_RING_BUFFER_SIZE >> 2);

//! TricesCountRingBuffer holds the readable trices count inside triceRingBuffer.
unsigned TricesCountRingBuffer = 0;

//! TriceBufferWritePosition is used by the TRICE_PUT macros.
uint32_t* TriceBufferWritePosition = TRICE_RING_BUFFER_START;

//! TriceRingBufferProtectLimit is the first address not allowed to be written to in any circumstances.
//! If TriceBufferWritePosition >= TriceRingBufferProtectLimit, TriceBufferWritePosition needs to wrap.
uint32_t* const TriceRingBufferProtectLimit = TRICE_RING_BUFFER_START + (TRICE_RING_BUFFER_SIZE >> 2) - TRICE_RING_BUFFER_MIN_SPACE32;

//! TriceRingBufferReadPosition points to a valid trice message when singleTricesRingCount > 0.
//! This is first the TRICE_DATA_OFFSET byte space followed by the trice data.
//! Initially this value is set to TriceRingBufferStart minus TRICE_DATA_OFFSET byte space
//! to get a correct value for the very first call of triceNextRingBufferRead
// uint32_t* TriceRingBufferReadPosition = TriceRingBufferStart - (TRICE_DATA_OFFSET>>2); //lint !e428 Warning 428: negative subscript (-4) in operator 'ptr-int'
uint32_t* TriceRingBufferReadPosition = TRICE_RING_BUFFER_START;

#if TRICE_DIAGNOSTICS == 1

//! TricesCountRingBufferMax holds the max count of trices occurred inside the ring buffer.
//! This value is only informal, because the length of the trice messages is not known.
// unsigned TricesCountRingBufferMax = 0;

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
	// When trice data read out takes place, the TriceRingBufferReadPosition is incremented. That makes the depth32 value smaller again and gives write space.
	// Before a new trice write operation, TriceBufferWritePosition is reset to TriceRingBufferStart if less than TRICE_SINGLE_MAX_SIZE bytes left to the TriceRingBufferLimit.
	// So, even there is a only 4 bytes Trice, it cannot be written, when no full TRICE_SINGLE_MAX_SIZE space is left, because we do not know its size in advance.
	// The TriceRingBufferReadPosition jumps in the same way as the TriceBufferWritePosition. It wraps according the same rules.

	int depth32 = TriceBufferWritePosition - TriceRingBufferReadPosition;
	if (depth32 == 0 && TricesCountRingBuffer > 0) {
		goto noSpace;
	}
	if (depth32 < 0) { // After a TriceBufferWritePosition reset the difference is negative and needs correction to get the correct value.
		depth32 += (TRICE_RING_BUFFER_SIZE >> 2);
	}
	// This fn is called before an intended trice data write ande therefore additional at least TRICE_SINGLE_MAX_SIZE>>2 32-bit words need to fit in the buffer.
	// There must be left TRICE_DATA_OFFSET space behind the last Trice to be usable as scratch pad in front of TriceRingBufferReadPosition.
	if (depth32 <= (TRICE_RING_BUFFER_SIZE >> 2) - TRICE_RING_BUFFER_MIN_SPACE32) {
		return 1;
	} else {
	noSpace:
#if TRICE_DIAGNOSTICS == 1
		TriceDeferredOverflowCount++;
#endif
		return 0;
	}
}

#endif // #if TRICE_PROTECT == 1

// triceIncrementRingBufferReadPosition sets TriceRingBufferReadPosition forward by wordCount.
//! \param lastWordCount is the u32 count of the last read trice including padding bytes.
TRICE_INLINE void triceIncrementRingBufferReadPosition(int wordCount) {
	TriceRingBufferReadPosition += wordCount;
	if (TriceRingBufferReadPosition > TriceRingBufferProtectLimit) {
		TriceRingBufferReadPosition = TriceRingBufferStart;
	}
}

// triceRingBufferDiagnostics computes and tracks TriceRingBufferDepthMax
TRICE_INLINE void triceRingBufferDiagnostics(void) {
#if TRICE_DIAGNOSTICS == 1
	int depth = (TriceBufferWritePosition - TriceRingBufferReadPosition) << 2; // lint !e845 Info 845: The left argument to operator '<<' is certain to be 0
	if (depth < 0) {
		depth += TRICE_DEFERRED_BUFFER_SIZE;
	}
	TriceRingBufferDepthMax = (depth > TriceRingBufferDepthMax) ? depth : TriceRingBufferDepthMax;
#endif // #if TRICE_DIAGNOSTICS == 1
}

#if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE

//! triceTransferSingleFraming transfers a single Trice from the Ring Buffer.
//! Implicit assumed is, that the pre-condition "TricesCountRingBuffer > 0" is fulfilled.
void triceTransferSingleFraming(void) {
	triceRingBufferDiagnostics(); // We need to measure before the RingBufferReadPosition increment.

	static int lastWordCount = 0; // lastWordCount is needed to increment TriceRingBufferReadPosition accordingly after transfer is done.
	triceIncrementRingBufferReadPosition(lastWordCount);
	lastWordCount = 0;

	if (TricesCountRingBuffer == 0) { // no data
		return;
	}
	TRICE_ENTER_CRITICAL_SECTION
	TricesCountRingBuffer--; // We decrement, even the Trice is not out yet.
	TRICE_LEAVE_CRITICAL_SECTION

	// The trice data are starting at byte offset TRICE_DATA_OFFSET from TriceRingBufferReadPosition.
	triceSingleDeferredOut(&lastWordCount);
}

#endif // #if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE

#if TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE

//! triceTransferMultiFraming transfers several, but not necessarily all, Trices from the Ring Buffer.
void triceTransferMultiFraming(void) {
	triceRingBufferDiagnostics(); // We need to measure before the RingBufferReadPosition increment.

	// The Ring Buffer can contain a fair amount of trices and we do not want them copy in a separate buffer for less RAM usage.
	// Therefore we pack all Trices until the Ring Buffer end (where it wraps) together.
	// We know here, that at least one Trice is inside the Ring Buffer.
	static int triceCount = 0; // triceCount it the count of Trices from the last call.
	TRICE_ENTER_CRITICAL_SECTION
	// It is known here, that the previous transmission is finished and we can advance.
	TricesCountRingBuffer -= triceCount;
	TRICE_LEAVE_CRITICAL_SECTION
	triceCount = 0;

	static int multiWordCount = 0;                        // wordCount is the Ring Buffer space which is now free after the last transfer is finished.
	triceIncrementRingBufferReadPosition(multiWordCount); // We can safely increment here, because we know, that the last transmission is done.
	multiWordCount = 0;

	triceMultiDeferredOut(&triceCount, &multiWordCount);
	return;
}

#endif // #if TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE

//! TriceTransfer needs to be called cyclically to read out the Ring Buffer.
void TriceTransfer(void) {
#if TRICE_CGO == 0             // In automated tests we assume last transmission is finished, so we do not test depth to be able to test multiple Trices in deferred mode.
	if (TriceOutDepth() > 0) { // Last transmission not finished. todo: Write TriceOutDepth() as dummy for TRICE_CGO.
		return;
	}
#endif
#if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE
	triceTransferSingleFraming();
#else
	triceTransferMultiFraming();
#endif
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

#if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE

//! triceSingleDeferredOut expects a single trice at TriceRingBufferReadPosition and returns the wordCount of this trice which includes 1-3 padding bytes.
//! The space from TriceRingBufferReadPosition-TRICE_DATA_OFFSET to TriceRingBufferReadPosition is assumed to be usable as scratch pad.
//! This function is specific to the ring buffer, because the wordCount value needs to be reconstructed.
//! \retval The returned value tells how many words where used by the transmitted trice and is usable for the memory management. See RingBuffer for example.
//! The returned value is typically (TRICE_DATA_OFFSET/4) plus 1 (4 bytes) to 3 (9-12 bytes) but could go up to ((TRICE_DATA_OFFSET/4)+(TRICE_BUFFER_SIZE/4)).
//! Return values <= 0 signal an error.
//! TriceRingBufferReadPosition points to the begin of a single trice.
//! The data at TriceRingBufferReadPosition are getting destroyed, because buffer is used as scratch pad.
static void triceSingleDeferredOut(int* wordCount) {
	uint8_t* enc = ((uint8_t*)TriceRingBufferReadPosition) - TRICE_DATA_OFFSET; // TRICE_DATA_OFFSET bytes are usable in front of TriceRingBufferReadPosition.
	uint8_t* pTriceNetStart;                                                    // Trice starts here. That is maybe a 2 bytes offset from TriceRingBufferReadPosition.
	size_t triceNetLength;                                                      // Trice length without padding bytes.
	int triceID = TriceIDAndBuffer(TriceRingBufferReadPosition, wordCount, &pTriceNetStart, &triceNetLength);
	// We can let TRICE_DATA_OFFSET only in front of the ring buffer and pack the Trices without offset space.
	// And if we allow as max depth only ring buffer size minus TRICE_DATA_OFFSET, we can use space in front of each Trice.

#if (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE)
#if TRICE_CONFIG_WARNINGS == 1
#warning configuration: The Trice tool does not support encryption without COBS (or TCOBS) framing.
#endif
	size_t encLen = TriceEncode(TRICE_DEFERRED_XTEA_ENCRYPT, TRICE_DEFERRED_OUT_FRAMING, enc, pTriceNetStart, triceNetLength);
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 1)
	// comment: The following 2 steps could be done in an incremental way within one singe loop.
	// Behind the trice brutto length (with padding bytes), 4 bytes needed as scratch pad when XTEA is active.
	// After TriceIDAndBuffer pTriceNetStart could have a 2 bytes offset.
	uint8_t* tmp = ((uint8_t*)TriceRingBufferReadPosition) - 4;
	memmove(tmp, pTriceNetStart, triceNetLength);           // Make 4 bytes space for scratch pad beind Trice message.
	size_t len8 = (triceNetLength + 7) & ~7;                // Only multiple of 8 are possible to encrypt, so we adjust len.
	memset(tmp + triceNetLength, 0, len8 - triceNetLength); // clear padding space
	XTEAEncrypt((uint32_t*)tmp, len8 >> 2);
#if TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS
	size_t encLen = (size_t)TCOBSEncode(enc, tmp, len8);
#elif TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS
	size_t encLen = COBSEncode(enc, tmp, len8);
#else
#error configuration
#endif
	enc[encLen++] = 0; // Add zero as package delimiter.
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS)
	size_t len = (size_t)TCOBSEncode(enc, pTriceNetStart, triceNetLength);
	enc[len++] = 0; // Add zero as package delimiter.
	size_t encLen = len;
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS)
	size_t len = (size_t)COBSEncode(enc, pTriceNetStart, triceNetLength);
	enc[len++] = 0; // Add zero as package delimiter.
	size_t encLen = len;
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE)
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
}

#endif // #if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE

#if TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE

// triceMultiDeferredOut packs Trices until the Ring Buffer end and returns their count and total length in words.
// These 2 values are used later, after the transmission is finished, for advancing.
// The value TricesCountRingBuffer is evaluated internally but not modified.
static void triceMultiDeferredOut(int* triceCount, int* multiWordCount) {
	*triceCount = 0;
	*multiWordCount = 0;
	if (TricesCountRingBuffer == 0) {
		return; // Nothing to do.
	}
	// At least one Trice message is ready for transfer.
	if (TriceRingBufferReadPosition > TriceRingBufferLimit - TRICE_RING_BUFFER_MIN_SPACE32) {
		for (;;)
			; // unexpected
	}

	// We can start at TriceRingBufferReadPosition and go to the TriceRingBufferLimit OR go to the TriceBufferWritePosition using TricesCountRingBuffer.
	uint8_t* enc = ((uint8_t*)TriceRingBufferReadPosition) - TRICE_DATA_OFFSET; // TRICE_DATA_OFFSET bytes are usable in front of TriceRingBufferReadPosition.
	uint8_t* tmp = enc + TRICE_DATA_OFFSET / 2;                                 // todo
	uint8_t* nextEnc = tmp;
	int multiLen = 0;
	uint32_t* nextTriceRingBufferReadPosition = TriceRingBufferReadPosition;
	do {
		int wordCount = 0;
		uint8_t* pTriceNetStart; // Trice starts here.
		size_t triceNetLength;   // Trice length without padding bytes.
		TriceIDAndBuffer(nextTriceRingBufferReadPosition, &wordCount, &pTriceNetStart, &triceNetLength);
		memmove(nextEnc, pTriceNetStart, triceNetLength);
		nextEnc += triceNetLength;
		multiLen += triceNetLength;
		*multiWordCount += wordCount;
		(*triceCount)++;
		nextTriceRingBufferReadPosition += wordCount;

	} while (*triceCount < TricesCountRingBuffer && nextTriceRingBufferReadPosition <= TriceRingBufferLimit - TRICE_RING_BUFFER_MIN_SPACE32);

#if (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE)
#if TRICE_CONFIG_WARNINGS == 1
#warning configuration: The Trice tool does not support encryption without COBS (or TCOBS) framing.
#endif
	size_t encLen = TriceEncode(TRICE_DEFERRED_XTEA_ENCRYPT, TRICE_DEFERRED_OUT_FRAMING, enc, enc, multiLen);
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 1)
	size_t len8 = (multiLen + 7) & ~7;          // Only multiple of 8 are possible to encrypt, so we adjust len.
	memset(tmp + multiLen, 0, len8 - multiLen); // clear padding space
	XTEAEncrypt((uint32_t*)tmp, len8 >> 2);
#if TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS
	size_t encLen = (size_t)TCOBSEncode(enc, tmp, len8);
	enc[encLen++] = 0;
#elif TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS
	size_t encLen = (size_t)COBSEncode(enc, tmp, len8);
	enc[encLen++] = 0;
#else
#error configuration
#endif
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS)
	size_t len = (size_t)TCOBSEncode(enc, tmp, multiLen);
	enc[len++] = 0; // Add zero as package delimiter.
	size_t encLen = len;
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS)
	size_t len = (size_t)COBSEncode(enc, tmp, multiLen);
	enc[len++] = 0; // Add zero as package delimiter.
	size_t encLen = len;
#elif (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE)
	size_t encLen = multiLen;
#else
#error configuration: TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE for ring buffer not implemented yet
#endif
#if TRICE_DIAGNOSTICS == 1
// todo ...
#endif // #if TRICE_DIAGNOSTICS == 1
	TriceNonBlockingDeferredWrite8(0, enc, encLen);
}

#endif // #if TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE

#if TRICE_RING_BUFFER_OVERFLOW_WATCH == 1

void TriceInitRingBufferMargins(void) {
	for (int i = 0; i < TRICE_RING_BUFFER_LOWER_MARGIN; i++) {
		triceRingBuffer[i] = TRICE_RING_BUFFER_MARGIN_FILL_VALUE;
	}
	for (int i = 0; i < TRICE_RING_BUFFER_UPPER_MARGIN; i++) {
		*(TriceRingBufferLimit + i) = TRICE_RING_BUFFER_MARGIN_FILL_VALUE;
	}
}

void WatchRingBufferMargins(void) {
	for (int i = 0; i < TRICE_RING_BUFFER_LOWER_MARGIN; i++) {
		if (triceRingBuffer[i] != TRICE_RING_BUFFER_MARGIN_FILL_VALUE) {
			for (;;)
				;
		}
	}

	for (int i = 0; i < TRICE_RING_BUFFER_UPPER_MARGIN; i++) {
		if (*(TriceRingBufferLimit + i) != TRICE_RING_BUFFER_MARGIN_FILL_VALUE) {
			for (;;)
				;
		}
	}
}

#endif // #if TRICE_RING_BUFFER_OVERFLOW_WATCH == 1

#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER && TRICE_OFF == 0
