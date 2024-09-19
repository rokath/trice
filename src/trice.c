//! \file trice.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////

#include "trice.h"
#include "cobs.h"
#include "tcobs.h"
#include "xtea.h"

#if TRICE_OFF == 0

// check configuration:

#ifndef TRICE_DATA_OFFSET
#error configuration: Check triceDefaultConfig.h or add "#define TRICE_DATA_OFFSET 16" to your triceConfig.h.
#endif

#if (TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 1) && (TRICE_DIRECT_AUXILIARY32 == 1)
#error configuration: Only one bit width for direct output is supported.
#endif

#if (TRICE_DIRECT_AUXILIARY8 == 1) && (TRICE_DIRECT_AUXILIARY32 == 1)
#error configuration: Only one bit width for direct output is supported.
#endif

#if (TRICE_DIRECT_AUXILIARY8 == 1) && (TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1)
#error configuration: Only one bit width for direct output is supported.
#endif

#if (TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1)
#error configuration: TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE not implemented: (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1)
#endif

#if (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS) && (TRICE_CONFIG_WARNINGS == 1)
#pragma message("configuration: XTEA with TCOBS is possible but not recommended. Encrypted data are more effective framed with COBS.")
#endif

#if (TRICE_DIRECT_XTEA_ENCRYPT == 1) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS) && (TRICE_CONFIG_WARNINGS == 1)
#pragma message("configuration: XTEA with TCOBS is possible but not recommended. Encrypted data are more effective framed with COBS.")
#endif

#ifndef TRICE_DEFERRED_TRANSFER_MODE
#error configuration: Check triceDefaultConfig.h or add "#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE" to your triceConfig.h.
#endif

#if (TRICE_DEFERRED_TRANSFER_MODE != TRICE_SINGLE_PACK_MODE) && (TRICE_DEFERRED_TRANSFER_MODE != TRICE_MULTI_PACK_MODE)
#error configuration: Check TRICE_DEFERRED_TRANSFER_MODE value.
#endif

#if (TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE) && (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DATA_OFFSET < 64) && (TRICE_CONFIG_WARNINGS == 1)
#warning configuration: Because each Trice is encoded separately, several Trices can easy "eat" the TRICE_DATA_OFFSET, so make this value not too small.
#endif

#if (TRICE_BUFFER == TRICE_RING_BUFFER) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)
#error configuration: (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE) not implemented for (TRICE_BUFFER == TRICE_RING_BUFFER)
#endif

#if (TRICE_DIRECT_XTEA_ENCRYPT == 1) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE) && (TRICE_CONFIG_WARNINGS == 1)
#warning configuration: The Trice tool needs COBS (or TCOBS) framing for encrypted data.
#endif

#if (TRICE_DEFERRED_XTEA_ENCRYPT == 1) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE) && (TRICE_CONFIG_WARNINGS == 1)
#warning configuration: The Trice tool needs COBS (or TCOBS) framing for encrypted data.
#endif

#if (TRICE_DEFERRED_XTEA_ENCRYPT == 0) && (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE) && (TRICE_CONFIG_WARNINGS == 1)
#warning configuration: (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE) is depreciated and not fully tested. Will probably not run error free.
#endif

#if (TRICE_DIRECT_OUTPUT == 0) && (TRICE_DEFERRED_OUTPUT == 0)
#error configuration: need at east one output mode - (TRICE_DIRECT_OUTPUT == 1) and/or (TRICE_DEFERRED_OUTPUT == 0)
#endif

#if (TRICE_DEFERRED_OUTPUT == 0) && (TRICE_BUFFER == TRICE_RING_BUFFER)
#error configuration: (TRICE_BUFFER == TRICE_RING_BUFFER) needs (TRICE_DEFERRED_OUTPUT == 1) or consider (TRICE_BUFFER == TRICE_STACK_BUFFER) or (TRICE_BUFFER == TRICE_STATIC_BUFFER)
#endif

#if (TRICE_DEFERRED_OUTPUT == 0) && (TRICE_BUFFER == TRICE_DOUBLE_BUFFER)
#error configuration: (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) needs (TRICE_DEFERRED_OUTPUT == 1) or consider (TRICE_BUFFER == TRICE_STACK_BUFFER) or (TRICE_BUFFER == TRICE_STATIC_BUFFER)
#endif

#if (TRICE_DEFERRED_OUTPUT == 1) && (TRICE_BUFFER == TRICE_STACK_BUFFER)
#error configuration: (TRICE_BUFFER == TRICE_STACK_BUFFER) needs (TRICE_DEFERRED_OUTPUT == 0)
#endif

#if (TRICE_DEFERRED_OUTPUT == 1) && (TRICE_BUFFER == TRICE_STATIC_BUFFER)
#error configuration: (TRICE_BUFFER == TRICE_STATIC_BUFFER) needs (TRICE_DEFERRED_OUTPUT == 0)
#endif

#if (TRICE_CGO == 1) && (TRICE_CYCLE_COUNTER == 1) && (TRICE_CONFIG_WARNINGS == 1)
#warning configuration: TRICE_CGO == 1 needs TRICE_CYCLE_COUNTER == 0 for successful tests.
#endif

#if (TRICE_DIRECT_OUTPUT == 1) && (TRICE_DIRECT_AUXILIARY8 == 0) && (TRICE_DIRECT_AUXILIARY32 == 0) && (TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 0) && (TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 0)
#error configuration: TRICE_DIRECT_OUTPUT == 1 needs specified output channel
#endif

#if (TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING == 1) && (TRICE_DEFERRED_OUTPUT == 0)
#error configuration: TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING == 1 needs TRICE_DEFERRED_OUTPUT == 1
#endif

#if (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1) && (TRICE_DIRECT_OUTPUT == 0)
#error configuration: TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1 needs TRICE_DIRECT_OUTPUT == 1
#endif

#if (TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 1) && (TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1)
#error configuration: only one RTT output channel is possible
#endif

#if (TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE == 1) && (TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1)
#error configuration: only one RTT output channel is possible
#endif

#if (TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE == 1) && (TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 1)
#error configuration: only one RTT output channel is possible
#endif

#if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DEFERRED_BUFFER_SIZE / 2 < TRICE_BUFFER_SIZE)
#error configuration: TRICE_DEFERRED_BUFFER_SIZE too small
#endif

#if (TRICE_BUFFER == TRICE_RING_BUFFER) && (TRICE_DEFERRED_BUFFER_SIZE < TRICE_BUFFER_SIZE)
#error configuration: TRICE_DEFERRED_BUFFER_SIZE too small
#endif

#if (TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1) && (TRICE_DIRECT_OUTPUT == 0)
#error configuration: TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1 needs TRICE_DIRECT_OUTPUT == 1
#endif

#if (TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 1) && (TRICE_DIRECT_OUTPUT == 0)
#error configuration: TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 1 needs TRICE_DIRECT_OUTPUT == 1
#endif

#if defined(SEGGER_RTT) && (TRICE_BUFFER_SIZE > BUFFER_SIZE_UP)
#error configuration: BUFFER_SIZE_UP too small
#endif

#if (TRICE_BUFFER == TRICE_STACK_BUFFER) && (TRICE_DIRECT_OUTPUT == 0)
#error configuration: direct-only mode needs TRICE_DIRECT_OUTPUT == 1
#endif

#if (TRICE_BUFFER == TRICE_STATIC_BUFFER) && (TRICE_DIRECT_OUTPUT == 0)
#error configuration: direct-only mode needs TRICE_DIRECT_OUTPUT == 1
#endif

#if (TRICE_DEFERRED_OUTPUT == 1) && (TRICE_DEFERRED_UARTA == 0) && (TRICE_DEFERRED_UARTB == 0) && (TRICE_DEFERRED_AUXILIARY8 == 0) && (TRICE_DEFERRED_AUXILIARY32 == 0)
#error configuration: deferred output needs TRICE_DFERRED_UARTx or TRICE_DEFERRED_AUXILIARYx
#endif

#if (TRICE_DEFERRED_UARTA == 1) && !defined(TRICE_UARTA)
#error configuration: deferred TRICE_DEFERRED_UARTA == 1 needs a defined TRICE_UARTA.
#endif

#if (TRICE_DEFERRED_UARTB == 1) && !defined(TRICE_UARTA)
#error configuration: deferred TRICE_DEFERRED_UARTB == 1 needs a defined TRICE_UARTB.
#endif

#if TRICE_DATA_OFFSET & 3
#error All size values must be a multiple of 4!
#endif

#if TRICE_SINGLE_MAX_SIZE & 3
#error All size values must be a multiple of 4!
#endif

#if TRICE_DEFERRED_BUFFER_SIZE & 3
#error All size values must be a multiple of 4!
#endif

#if (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1)
#warning configuration: TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING is experimental
#endif

#if (TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING == 1)
#warning configuration: TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING is experimental
#endif

#if (TRICE_DIRECT_AUXILIARY8 == 1) && (TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1)
#error configuration: (TRICE_DIRECT_AUXILIARY8 == 1) allows (TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 1) but not (TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1)
#endif

// function prototypes:

#if TRICE_DEFERRED_UARTA == 1
void TriceNonBlockingWriteUartA(const void* buf, size_t nByte);
#endif

#if TRICE_DEFERRED_UARTB == 1
void TriceNonBlockingWriteUartB(const void* buf, size_t nByte);
#endif

#if TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1
static void SEGGER_Write_RTT0_NoCheck32(const uint32_t* pData, unsigned NumW);
#endif

// global variables:

//! TriceErrorCount is incremented, when data inside the internal trice buffer are corrupted.
//! That could happen, when the buffer wrapped before data are sent. The user app should check this diagnostic value.
unsigned TriceErrorCount = 0;

//! triceCommand is the command receive buffer.
char triceCommandBuffer[TRICE_COMMAND_SIZE_MAX + 1]; // with terminating 0

//! triceCommandFlag ist set, when a command was received completely.
int triceCommandFlag = 0; // updated

#if TRICE_CYCLE_COUNTER == 1

//! TriceCycle is increased and transmitted with each trice message, if enabled.
uint8_t TriceCycle = 0xc0;

#endif

#if TRICE_DIAGNOSTICS == 1

//! TriceDataOffsetDepthMax measures the max used offset space.
int TriceDataOffsetDepthMax = 0;

//! TriceSingleMaxWordCount is a diagnostics value usable to optimize buffer space TRICE_BUFFER_SIZE for a single trice message.
unsigned TriceSingleMaxWordCount = 0;

//! TriceDynBufTruncateCount couts how often a dynamic buffer was truncated because it was too long.
unsigned TriceDynBufTruncateCount = 0;

#if TRICE_PROTECT == 1

unsigned TriceDirectOverflowCount = 0;
unsigned TriceDeferredOverflowCount = 0;

#endif

#endif

//! TriceInit needs to run before the first trice macro is executed.
//! Not neseecary for all configurations.
void TriceInit(void) {
#if defined(SEGGER_RTT) && (TRICE_CGO == 0)
	// This is just to force the INIT() call inside SEGGER_RTT.c what allows to use
	// SEGGER_RTT_WriteNoLock or SEGGER_Write_RTT0_NoCheck32 instead of SEGGER_RTT_Write.
	SEGGER_RTT_Write(0, 0, 0); // lint !e534
#endif

#if (TRICE_DIRECT_XTEA_ENCRYPT == 1) || (TRICE_DEFERRED_XTEA_ENCRYPT == 1)
	XTEAInitTable();
#endif
}

//! triceDataLen returns encoded len.
//! \param p points to nc
//! To avoid alignment issues, the optional payload needs to start at a 32-bit boundary.
//! The provided buffer starts also at a 32-bit boundary.
//! To ensure, the first 16-bit value is ssiiiiiiI we do the following:
//! -       v__________________ v___________ v__ (32-bit alignment positions)
//! - *da = 11iiiiiiI TT        TT        NC ... | ID(n): After writing 11iiiiiiI write the 32-bit TTTT value in 2 16-bit write operations.
//! - *da = 10iiiiiiI 10iiiiiiI TT        NC ... | Id(n): Write 10iiiiiiI as doubled value in one 32-bit operation into the trice buffer. The first 16-bit will be removed just before sending to the out channel.
//! - *da =                     01iiiiiiI NC ... | id(n): Just write 01iiiiiiI as 16-bit operation.
//! - *da = 00xxxxxxX extended trices are not used yet, unspecified length >= 2
//! - This way, after writing the 16-bit NC value the payload starts always at a 32-bit boundary.
//! - With framing, user 1-byte messages allowed and ignored by the trice tool.
size_t triceDataLen(const uint8_t* p) {
	uint16_t nc = TRICE_TTOHS(*(uint16_t*)p); // lint !e826
	size_t n = nc >> 8;
	if (n < 128) {
		return n;
	}
	return nc & 0x7fff;
}

#if TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1

//! triceIDAndLen expects at buf a trice message and returns the ID for routing.
//! \param pBuf is where the trice message starts.
//! \param ppStart return the trice data start address. This is pBuf or 2 bytes later.
//! \param triceID is filled positive ID value on success or negative on error.
//! \retval is the netto trice length (without padding bytes), 0 on error.
static size_t triceIDAndLen(uint32_t* pBuf, uint8_t** ppStart, int* triceID) {
	uint16_t* pTID = (uint16_t*)pBuf;  // get TID address
	uint16_t TID = TRICE_TTOHS(*pTID); // type and id
	*triceID = 0x3FFF & TID;
	int triceType = TID >> 14;
	size_t len;
	uint8_t* pStart = (uint8_t*)pBuf;
	switch (triceType) {
	case TRICE_TYPE_S0:                     // S0 = no stamp
		len = 4 + triceDataLen(pStart + 2); // tyId
		break;
	case TRICE_TYPE_S2:                     // S2 = 16-bit stamp
		pStart += 2;                        // see Id(n) macro definition
		len = 6 + triceDataLen(pStart + 4); // tyId ts16
		break;
	case TRICE_TYPE_S4:                     // S4 = 32-bit stamp
		len = 8 + triceDataLen(pStart + 6); // tyId ts32
		break;
	default:
		// lint -fallthrugh
	case TRICE_TYPE_X0:
		TriceErrorCount++;
		*triceID = -__LINE__; // extended trices not supported (yet)
		return 0;
	}
	*ppStart = pStart;
	return len;
}

#endif // #if TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1

//! TriceEncode expects at buf trice netto data with netto length len.
//! It fills dst with the next trice data, which are encoded and framed or not, according the selected switches.
//! The areas of dst and buf are allowed to overlap.
//! \param encrypt, when 0, then without encryption, when 1, then with XTEA encryption.
//! \param framing selects if and which framing is used.
//! \param dst is the destination. It must be 32-bit aligned.
//! \param buf is the source. This can be not 32-bit aligned.
//! \param len is the source len.
//! \retval is the encoded len with 0-delimiter byte.
size_t TriceEncode(unsigned encrypt, unsigned framing, uint8_t* dst, const uint8_t* buf, size_t len) {

#if TRICE_DIAGNOSTICS == 1
	int distance = buf - dst;
	if (distance < 4) {
		TriceErrorCount++;
	}
	int TriceDataOffsetDepth = TRICE_DATA_OFFSET - distance;
	TriceDataOffsetDepthMax = TriceDataOffsetDepth < TriceDataOffsetDepthMax ? TriceDataOffsetDepthMax : TriceDataOffsetDepth;
#endif

	size_t encLen;
	const uint8_t* dat = buf;

	if (encrypt) {

#if (TRICE_DIRECT_XTEA_ENCRYPT == 1) || (TRICE_DEFERRED_XTEA_ENCRYPT == 1)

// Only multiple of 8 encryptable, but trice data are 32-bit aligned.
// A 64-bit trice data aligning would waste RAM and costs a bit time during trice generation.
// We need additional 4 bytes after each trice for the XTEA encryption.
// Also, the framing could make the trice message a bit longer.
// Therefore we copy the trice data to a place, we can use.
// The location for XTEAEncrypt must lay on a 32-bit boundary.
#if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)

		// special case: The data can be big, are compacted and behind them is space. So we can encrypt them in space
		dat = buf;                                    // That is also for the framing afterwards.
		size_t len8 = (len + 7) & ~7;                 // Only multiple of 8 encryptable, so we adjust len.
		memset(((uint8_t*)buf) + len, 0, len8 - len); // clear padding space: ATTENTION! OK only for this compiler switch setting.
		len = len8;
		XTEAEncrypt((uint32_t*)dat, len8 >> 2);

#else // #if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)

		// Let space in front for framing, free 4 bytes behind do a 32-bit align backwards.
		// uint32_t * loc = (uint32_t *)(((unsigned)dst + TRICE_DATA_OFFSET - 4) & ~3);
		// The computing above does not work, because, when several Trices, this "free" location
		// drifts into the unprocessed Trice data. So we create a buffer.
		// Not on the stack, because small stacks could be a problem here.
		static uint32_t loc[TRICE_SINGLE_MAX_SIZE >> 2];

		// Because dst + TRICE_DATA_OFFSET could be the buf value itself, we need to move at least 4 bytes.
		memmove(loc, buf, len);                       // We use not memcpy here, because dst and buf allowed to overlap.
		dat = (const uint8_t*)loc;                    // That is also for the framing afterwards.
		size_t len8 = (len + 7) & ~7;                 // Only multiple of 8 encryptable, so we adjust len.
		memset(((uint8_t*)loc) + len, 0, len8 - len); // clear padding space
		len = len8;
		XTEAEncrypt(loc, len8 >> 2);

#endif // #else // #if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)

#endif // #if (TRICE_DIRECT_XTEA_ENCRYPT == 1) || (TRICE_DEFERRED_XTEA_ENCRYPT == 1)
	}

	switch (framing) {
	case TRICE_FRAMING_TCOBS:
		encLen = (size_t)TCOBSEncode(dst, dat, len);
		dst[encLen++] = 0; // Add zero as package delimiter.
		return encLen;
	case TRICE_FRAMING_COBS:
		encLen = (size_t)COBSEncode(dst, dat, len);
		dst[encLen++] = 0; // Add zero as package delimiter.
		return encLen;
	case TRICE_FRAMING_NONE:
		memmove(dst, dat, len);
		encLen = len;
		return encLen;
	}

	return 0; // unexpected
}

#if (TRICE_DIAGNOSTICS == 1) && defined(SEGGER_RTT)

unsigned RTT0_writeDepthMax = 0; //!< RTT0_writeDepthMax is usable for diagnostics.

static void triceSeggerRTTDiagnostics(void) {
	unsigned space;
#if TRICE_CGO == 1 // automated tests
	space = TRICE_BUFFER_SIZE;
#else
	space = SEGGER_RTT_GetAvailWriteSpace(0);
#endif
	int writeDepth = BUFFER_SIZE_UP - space;
#if (TRICE_PROTECT == 1)
	if (writeDepth < 0) { // unexpected
		for (;;)
			;
	}
#endif
	unsigned wd = (unsigned)writeDepth;
	RTT0_writeDepthMax = wd > RTT0_writeDepthMax ? wd : RTT0_writeDepthMax;
}

#endif // #if (TRICE_DIAGNOSTICS ==1) && defined(SEGGER_RTT)

#define TRICE_DIRECT32 ((TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1) || (TRICE_DIRECT_AUXILIARY32 == 1))
#define TRICE_DIRECT8 ((TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 1) || (TRICE_DIRECT_AUXILIARY8 == 1))

#if TRICE_DIRECT32 && TRICE_DIRECT8
#error configuration
#endif

#define TRICE_DIRECT32_ONLY TRICE_DIRECT32 && ((TRICE_BUFFER == TRICE_STACK_BUFFER) || (TRICE_BUFFER == TRICE_STATIC_BUFFER)) //!< direct only output
#define TRICE_DIRECT32_ALSO TRICE_DIRECT32 && ((TRICE_BUFFER == TRICE_RING_BUFFER) || (TRICE_BUFFER == TRICE_DOUBLE_BUFFER))  //!< direct and deferred output

#define TRICE_DIRECT8_ONLY TRICE_DIRECT8 && ((TRICE_BUFFER == TRICE_STACK_BUFFER) || (TRICE_BUFFER == TRICE_STATIC_BUFFER)) //!< direct only output
#define TRICE_DIRECT8_ALSO TRICE_DIRECT8 && ((TRICE_BUFFER == TRICE_RING_BUFFER) || (TRICE_BUFFER == TRICE_DOUBLE_BUFFER))  //!< direct and deferred output

#if TRICE_DIRECT32_ONLY && TRICE_DIRECT8_ONLY
#error configuration
#endif

#if TRICE_DIRECT32_ALSO && TRICE_DIRECT8_ALSO
#error configuration
#endif

#if TRICE_DIRECT32_ONLY && TRICE_DIRECT8_ALSO
#error configuration
#endif

#if TRICE_DIRECT32_ALSO && TRICE_DIRECT8_ONLY
#error configuration
#endif

#if TRICE_DIRECT32

#if TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1

//! SEGGER_Write_RTT0_NoCheck32 was derived from SEGGER_RTT.c version 7.60g function _WriteNoCheck for speed reasons. If using a different version please review the code first.
static void SEGGER_Write_RTT0_NoCheck32(const uint32_t* pData, unsigned NumW) {
#if TRICE_CGO == 1 // automated tests
	TriceWriteDeviceCgo(pData, NumW << 2);
#else // #if TRICE_CGO == 1

	unsigned NumWordsAtOnce;
	unsigned WrOff;
	unsigned RemW;

#if TRICE_PROTECT == 1
	unsigned space = SEGGER_RTT_GetAvailWriteSpace(0);
	if (space < NumW << 2) {
		for (;;)
			;
	}
#endif

	// Get "to-host" ring buffer.
	static SEGGER_RTT_BUFFER_UP* const pRingUp0 = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[0] + SEGGER_RTT_UNCACHED_OFF); // Access uncached to make sure we see changes made by the J-Link side and all of our changes go into HW directly
	WrOff = pRingUp0->WrOff;
	RemW = (pRingUp0->SizeOfBuffer - WrOff) >> 2;
	volatile uint32_t* pDstW = (uint32_t*)((pRingUp0->pBuffer + WrOff) + SEGGER_RTT_UNCACHED_OFF); // lint !e826

	if (RemW > NumW) { // All data fits before wrap around
		WrOff += NumW << 2;
		while (NumW--) {
			*pDstW++ = *pData++;
		};
		RTT__DMB(); // Force data write to be complete before writing the <WrOff>, in case CPU is allowed to change the order of memory accesses
		pRingUp0->WrOff = WrOff;
	} else { // We reach the end of the buffer, so need to wrap around
		NumWordsAtOnce = RemW;
		while (NumWordsAtOnce--) {
			*pDstW++ = *pData++;
		};
		pDstW = (uint32_t*)(pRingUp0->pBuffer + SEGGER_RTT_UNCACHED_OFF); // lint !e826
		NumWordsAtOnce = NumW - RemW;
		while (NumWordsAtOnce--) {
			*pDstW++ = *pData++;
		};
		RTT__DMB(); // Force data write to be complete before writing the <WrOff>, in case CPU is allowed to change the order of memory accesses
		pRingUp0->WrOff = (NumW - RemW) << 2;
	}

#if TRICE_DIAGNOSTICS == 1
	triceSeggerRTTDiagnostics();
#endif

#endif // #else // #if TRICE_CGO == 1
}

#endif // #if TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1

#if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_COBS) || (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS)

//! directXEncode32 transforms buf to enc and adds a 0-delimiter and padding zeroes to the next uint32 boundary.
//! \retval count of enc values
static unsigned directXEncode32(uint32_t* enc, const void* buf, unsigned count) {

#if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_COBS)
	size_t lenX = COBSEncode(enc, buf, count << 2);
#elif (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS)
	size_t lenX = TCOBSEncode(enc, buf, count << 2);
#else
#error configuration
#endif

	size_t len4 = (lenX + 3 + 1) & ~3; // size with padding and with packet 0-delimiter
	int zeroesCount = len4 - lenX;
	memset(((uint8_t*)enc) + lenX, 0, zeroesCount);
	return len4 >> 2;
}

#endif // #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_COBS) ||  (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS)

static void TriceDirectWrite32(const uint32_t* buf, unsigned count) {

#if TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1

#if TRICE_PROTECT == 1

#if TRICE_CGO == 1               // automated tests
	unsigned space = count << 2; // always enough space
#else
	unsigned space = SEGGER_RTT_GetAvailWriteSpace(0);
#endif

	if (space >= count << 2) {
		SEGGER_Write_RTT0_NoCheck32(buf, count);
	} else {
		TriceDirectOverflowCount++;
	}

#else  // #if TRICE_PROTECT == 1
	SEGGER_Write_RTT0_NoCheck32(buf, count);
#endif // #else // #if TRICE_PROTECT == 1

#if TRICE_DIAGNOSTICS == 1
	triceSeggerRTTDiagnostics(); // todo: maybe not needed
#endif

#endif // #if TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1

#if TRICE_DIRECT_AUXILIARY32 == 1
	TriceNonBlockingDirectWrite32Auxiliary(buf, count);
#endif // #if TRICE_DIRECT_AUXILIARY32 == 1
}

#endif // #if TRICE_DIRECT32

#if TRICE_DIRECT8

#if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_COBS) || (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS)

//! directXEncode transforms buf to enc and adds a 0-delimiter and padding zeroes to the next uint32 boundary.
static size_t directXEncode8(void* enc, const void* buf, unsigned len) {

#if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_COBS)
	size_t lenX = COBSEncode(enc, buf, len);
#elif (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS)
	size_t lenX = TCOBSEncode(enc, buf, len);
#else
#error configuration
#endif

	size_t len4 = (lenX + 1 + 3) & ~3;
	memset(enc + lenX, 0, len4 - lenX);
	return lenX + 1;
}

#endif // #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_COBS) ||  (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS)

#if (TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 1) || (TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE == 1)

static void TriceWriteDeviceRtt0(const uint8_t* enc, size_t encLen) {

#if TRICE_CGO == 1 // automated tests
	TriceWriteDeviceCgo(enc, encLen);
#else // #if TRICE_CGO == 1

#if TRICE_PROTECT == 1
	unsigned space = SEGGER_RTT_GetAvailWriteSpace(0);
	if (encLen <= space) {
		SEGGER_RTT_WriteNoLock(0, enc, encLen);
	} else {
		TriceDirectOverflowCount++;
	}
#else  // #if TRICE_PROTECT == 1
	SEGGER_RTT_WriteNoLock(0, enc, encLen);
#endif // #else // #if TRICE_PROTECT == 1

#endif // #else // #if TRICE_CGO == 1

#if TRICE_DIAGNOSTICS == 1
	triceSeggerRTTDiagnostics(); // todo: maybe not needed
#endif
}

#endif // #if (TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 1) || (TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE == 1)

static void TriceDirectWrite8(const uint8_t* enc, size_t encLen) {
#if TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 1
	TriceWriteDeviceRtt0(enc, encLen);
#endif

#if TRICE_DIRECT_AUXILIARY8
	TriceNonBlockingDirectWrite8Auxiliary(enc, encLen);
#endif
}

#endif // #if TRICE_DIRECT8

#if TRICE_DIRECT_OUTPUT == 1

//! TriceNonBlockingDirectWrite copies a single trice from triceStart to output.
//! This is the time critical part, executed inside TRICE_LEAVE.
//! The trice data start at triceStart and include wordCount values with 1-3 padding bytes at the end.
//! In front of triceStart are TRICE_DATA_OFFSET bytes space usable for optional in-buffer encoding. ??????? todo
//! This is NOT the case, when using direct and deferred modes parallel, because for efficient RAM usage
//! there is no gap between the Trices in double or ring buffer. Therefore, when enabling
//! TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE together with a deferred mode, for efficiency the RTT output can only be unframed.
//! The space behind the trice (at triceStart + wordCount) is usable because in direct mode this is writable.
//! Also in combined modes (direct plus deferred) this is allowed, under certain cirumstances:
//! - TRICE_DOUBLE_BUFFER: The current Trice could be the last one and could have filled the double buffer to the end. So additional 4 bytes at the end are needed as scratc pad.
//! - TRICE_RING_BUFFER: The max depth is not allowed and at the end is 4 bytes space needed.
void TriceNonBlockingDirectWrite(uint32_t* triceStart, unsigned wordCount) {

	// The 16-bit stamped trices start with 2-times 16-bit ID for align and speed reasons.
	// The trice tool knows and expects that when switch -packageFraming = NONE was applied.
	// The 2 additional transmit bytes are avoidable then but that would need a 2nd NONE option for the trice tool, what makes usage more confusing.
	// That the TRICE_FRAMING_NONE does not remove the 2 additional bytes for 16-bit stamped trices has the
	// main reason in the TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE option for the fast 32-bit transfer, what probably will be a common use case.

	// What happens here, is similar to TriceEncode but this is time critical code and we can do in-place encoding too.

#if TRICE_DIRECT32_ONLY // Space at triceStart + wordCount is usable and we can destroy the data.

#if (TRICE_DIRECT_XTEA_ENCRYPT == 1)
	triceStart[wordCount++] = 0;        // clear padding space
	wordCount &= ~1;                    // only multiple of 8 can be encrypted
	XTEAEncrypt(triceStart, wordCount); // in-buffer encryption (in direct-only mode is usable space bedind the Trice message.)
#endif

#if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
	uint32_t* enc = triceStart;
	unsigned count = wordCount;
#else
	static uint32_t enc[TRICE_BUFFER_SIZE >> 2]; // static buffer!
	unsigned count = directXEncode32(enc, triceStart, wordCount);
#endif

	TriceDirectWrite32(enc, count);
	return;

#elif TRICE_DIRECT8_ONLY // Space at triceStart + wordCount is usable and we can destroy the data.

#if (TRICE_DIRECT_XTEA_ENCRYPT == 1)
	triceStart[wordCount++] = 0;        // clear padding space
	wordCount &= ~1;                    // only multiple of 8 can be encrypted
	XTEAEncrypt(triceStart, wordCount); // in-buffer encryption (in direct-only mode is usable space bedind the Trice message.)
#endif

#if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
	uint8_t* enc = (uint8_t*)triceStart;
	unsigned len = wordCount << 2;
#else
	static uint8_t enc[TRICE_BUFFER_SIZE];                          // stack buffer!
	unsigned len = directXEncode8(enc, triceStart, wordCount << 2); // Up to 3 trailing zeroes are packed as well here.
#endif

	TriceDirectWrite8(enc, len);
	return;

#elif TRICE_DIRECT32_ALSO // Space at triceStart + wordCount is NOT usable and we can NOT destroy the data.

#if (TRICE_DIRECT_XTEA_ENCRYPT == 1) || (TRICE_DIRECT_OUT_FRAMING != TRICE_FRAMING_NONE)
	static uint32_t enc[TRICE_BUFFER_SIZE >> 2]; // stack buffer!
#endif

#if (TRICE_DIRECT_XTEA_ENCRYPT == 1)
	uint32_t* dat = enc + (TRICE_DATA_OFFSET >> 2);
	memcpy(dat, triceStart, wordCount << 2); // Trice data are 32-bit aligned.
	dat[wordCount++] = 0;                    // clear padding space
	wordCount &= ~1;                         // only multiple of 8 can be encrypted
	XTEAEncrypt(dat, wordCount);             // in-buffer encryption (in direct-only mode is usable space bedind the Trice message.)
#else
	uint32_t* dat = triceStart;
#endif

#if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
	TriceDirectWrite32(dat, wordCount);
#else
	unsigned count = directXEncode32(enc, dat, wordCount); // Up to 3 trailing zeroes are packed as well here.
	TriceDirectWrite32(enc, count);
#endif

	return;

#elif TRICE_DIRECT8_ALSO // Space at triceStart + wordCount is NOT usable and we can NOT destroy the data.

	static uint32_t enc[TRICE_BUFFER_SIZE >> 2]; // stack buffer!

#if (TRICE_DIRECT_XTEA_ENCRYPT == 1)
	uint32_t* dat = enc + (TRICE_DATA_OFFSET >> 2);
	memcpy(dat, triceStart, wordCount << 2); // Trice data are 32-bit aligned.
	dat[wordCount++] = 0;                    // clear padding space
	wordCount &= ~1;                         // only multiple of 8 can be encrypted
	XTEAEncrypt(dat, wordCount);             // in-buffer encryption (in direct-only mode is usable space bedind the Trice message.)
#else
	uint32_t* dat = triceStart;
#endif

#if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
	TriceDirectWrite8((uint8_t*)dat, wordCount << 2);
#else
	unsigned len = directXEncode8(enc, dat, wordCount << 2); // Up to 3 trailing zeroes are packed as well here.
	TriceDirectWrite8((uint8_t*)enc, len);
#endif

	return;

#else //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#error configuration
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif // #if TRICE_DIRECT_OUTPUT == 1

#if TRICE_DEFERRED_OUTPUT == 1

// TriceNonBlockingDeferredWrite8 routes trice data to output channels.
void TriceNonBlockingDeferredWrite8(int triceID, const uint8_t* enc, size_t encLen) {

#if TRICE_DEFERRED_UARTA == 1
#if (TRICE_UARTA_MIN_ID != 0) || (TRICE_UARTA_MAX_ID != 0)
	if ((TRICE_UARTA_MIN_ID < triceID) && (triceID < TRICE_UARTA_MAX_ID))
#else
	TRICE_UNUSED(triceID)
#endif
	{
		TriceNonBlockingWriteUartA(enc, encLen);
	}
#endif

#if TRICE_DEFERRED_UARTB == 1
#if (TRICE_UARTB_MIN_ID != 0) && (TRICE_UARTB_MAX_ID != 0)
	if ((TRICE_UARTB_MIN_ID < triceID) && (triceID < TRICE_UARTB_MAX_ID))
#else
	TRICE_UNUSED(triceID)
#endif
	{
		TriceNonBlockingWriteUartB(enc, encLen);
	}
#endif

#if (TRICE_DEFERRED_AUXILIARY8 == 1)
#if defined(TRICE_DEFERRED_AUXILIARY8_MIN_ID) && defined(TRICE_DEFERRED_AUXILIARY8_MAX_ID)
	if ((TRICE_DEFERRED_AUXILIARY8_MIN_ID < triceID) && (triceID < TRICE_DEFERRED_AUXILIARY8_MAX_ID))
#else
	TRICE_UNUSED(triceID)
#endif
	{
		TriceNonBlockingDeferredWrite8Auxiliary(enc, encLen);
	}
#endif

#if (TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE == 1)
#if defined(TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE_MIN_ID) && defined(TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE_MAX_ID)
	if ((TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE_MIN_ID < triceID) && (triceID < TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE_MAX_ID))
#else
	TRICE_UNUSED(triceID)
#endif
	{
		TriceWriteDeviceRtt0(enc, encLen);
	}
#endif

	//  #if TRICE_LOG_OVER_MODBUS_FUNC24_ALSO == 1
	//      #if defined(TRICE_MODBUS_MIN_ID) && defined(TRICE_MODBUS_MAX_ID)
	//      if( (TRICE_MODBUS_MIN_ID < triceID) && (triceID < TRICE_MODBUS_MAX_ID) )
	//      #else
	//          TRICE_UNUSED(triceID)
	//      #endif
	//      TriceWriteDeviceModbus( enc, encLen );
	//  #endif
} // lint !e715 Info 715: Symbol 'triceID' not referenced

#endif // #if TRICE_DEFERRED_OUTPUT == 1

//! TriceOutDepth returns the amount of bytes not written yet from the slowest device.
unsigned TriceOutDepth(void) {
	unsigned d = 0, depth = 0;
	TRICE_ENTER_CRITICAL_SECTION

#ifdef SEGGER_RTT
	// When no RTT host is connected, the RTT buffer runs full.
	// If a RTT host is connected, it is assumed to be the fastest device.
	depth = d > depth ? d : depth;
#endif

#if TRICE_DEFERRED_UARTA == 1
	d = TriceOutDepthUartA(); // lint !e838
	depth = d > depth ? d : depth;
#endif

#if TRICE_DEFERRED_UARTB == 1
	d = TriceOutDepthUartB();
	depth = d > depth ? d : depth;
#endif

#if TRICE_CGO == 1 // automated tests
	d = TriceOutDepthCGO();
	depth = d > depth ? d : depth;
#endif

	TRICE_LEAVE_CRITICAL_SECTION
	return depth;
}

#if TRICE_OFF == 1 || TRICE_CLEAN == 1
#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

//! TRICE_ASSERT writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! This is a helper macro and should not be used in user code.
#define TRICE_ASSERT(tid) \
	TRICE_ENTER tid;      \
	TRICE_CNTC(0);        \
	TRICE_LEAVE

void triceAssertTrue(int idN, char* msg, int flag) {
	TRICE_UNUSED(msg)
	if (!flag) {
		TRICE_ASSERT(id(idN));
	}
}

void TriceAssertTrue(int idN, char* msg, int flag) {
	TRICE_UNUSED(msg)
	if (!flag) {
		TRICE_ASSERT(Id(idN));
	}
}

void TRiceAssertTrue(int idN, char* msg, int flag) {
	TRICE_UNUSED(msg)
	if (!flag) {
		TRICE_ASSERT(ID(idN));
	}
}

void triceAssertFalse(int idN, char* msg, int flag) {
	TRICE_UNUSED(msg)
	if (flag) {
		TRICE_ASSERT(id(idN));
	}
}

void TriceAssertFalse(int idN, char* msg, int flag) {
	TRICE_UNUSED(msg)
	if (flag) {
		TRICE_ASSERT(Id(idN));
	}
}

void TRiceAssertFalse(int idN, char* msg, int flag) {
	TRICE_UNUSED(msg)
	if (flag) {
		TRICE_ASSERT(ID(idN));
	}
}

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#ifdef TRICE_N

void triceN(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE_N(id(tid), fmt, buf, n);
}

void TriceN(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE_N(Id(tid), fmt, buf, n);
}

void TRiceN(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE_N(ID(tid), fmt, buf, n);
}

void trice8B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE8_B(id(tid), fmt, buf, n);
}

void Trice8B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE8_B(Id(tid), fmt, buf, n);
}

void TRice8B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE8_B(ID(tid), fmt, buf, n);
}

void trice16B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE16_B(id(tid), fmt, buf, n);
}

void Trice16B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE16_B(Id(tid), fmt, buf, n);
}

void TRice16B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE16_B(ID(tid), fmt, buf, n);
}

void trice32B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE32_B(id(tid), fmt, buf, n);
}

void Trice32B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE32_B(Id(tid), fmt, buf, n);
}

void TRice32B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE32_B(ID(tid), fmt, buf, n);
}

void trice64B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE64_B(id(tid), fmt, buf, n);
}

void Trice64B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE64_B(Id(tid), fmt, buf, n);
}

void TRice64B(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE64_B(ID(tid), fmt, buf, n);
}

void trice8F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE8_F(id(tid), fmt, buf, n);
}

void Trice8F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE8_F(Id(tid), fmt, buf, n);
}

void TRice8F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE8_F(ID(tid), fmt, buf, n);
}

void trice16F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE16_F(id(tid), fmt, buf, n);
}

void Trice16F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE16_F(Id(tid), fmt, buf, n);
}

void TRice16F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE16_F(ID(tid), fmt, buf, n);
}

void trice32F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE32_F(id(tid), fmt, buf, n);
}

void Trice32F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE32_F(Id(tid), fmt, buf, n);
}

void TRice32F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE32_F(ID(tid), fmt, buf, n);
}

void trice64F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE64_F(id(tid), fmt, buf, n);
}

void Trice64F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE64_F(Id(tid), fmt, buf, n);
}

void TRice64F(int tid, char* fmt, void* buf, uint32_t n) {
	TRICE64_F(ID(tid), fmt, buf, n);
}

#endif // #ifdef TRICE_N

#ifdef TRICE_S

void triceS(int tid, char* fmt, char* runtimeGeneratedString) {
	TRICE_S(id(tid), fmt, runtimeGeneratedString);
}

void TriceS(int tid, char* fmt, char* runtimeGeneratedString) {
	TRICE_S(Id(tid), fmt, runtimeGeneratedString);
}

void TRiceS(int tid, char* fmt, char* runtimeGeneratedString) {
	TRICE_S(ID(tid), fmt, runtimeGeneratedString);
}

#endif // #ifdef TRICE_N

#endif // #if TRICE_OFF == 0
