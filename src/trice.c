//! \file trice.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////

#include "trice.h"
#include "tcobs.h"
#include "cobs.h"
#include "xtea.h"

// check configuration:

#ifndef TRICE_DEFERRED_TRANSFER_MODE
#error configuration: TRICE_DEFERRED_TRANSFER_MODE is not defined. Add "#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE" to your triceConfig.h.
#endif

#if (TRICE_DEFERRED_TRANSFER_MODE != TRICE_SINGLE_PACK_MODE) && (TRICE_DEFERRED_TRANSFER_MODE != TRICE_MULTI_PACK_MODE)
#error configuration: Check TRICE_DEFERRED_TRANSFER_MODE value.
#endif

#if (TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE) && (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DATA_OFFSET < 64)
#warning configuration: Because each Trice is encoded separately, several Trices can easy "eat" the TRICE_DATA_OFFSET, so make this value not too small.
#endif

#if ( (TRICE_XTEA_DIRECT_ENCRYPT == 1) || (TRICE_XTEA_DEFERRED_ENCRYPT == 1) ) && !defined(XTEA_ENCRYPT_KEY)
#error configuration: TRICE_XTEA_DIRECT_ENCRYPT and TRICE_XTEA_DEFERRED_ENCRYPT need a defined XTEA_ENCRYPT_KEY.
#endif

#if ( (TRICE_XTEA_DIRECT_ENCRYPT == 0) && (TRICE_XTEA_DEFERRED_ENCRYPT == 0) ) && defined(XTEA_ENCRYPT_KEY)
#warning configuration: TRICE_XTEA_DIRECT_ENCRYPT and TRICE_XTEA_DEFERRED_ENCRYPT are 0, so no need for a defined XTEA_ENCRYPT_KEY.
#endif

#if (TRICE_DIRECT_OUTPUT == 0) && (TRICE_DEFERRED_OUTPUT == 0)
#error configuration: need at east one output mode - (TRICE_DIRECT_OUTPUT == 1) and/or (TRICE_DDEFERRED_OUTPUT == 0)
#endif

#if (TRICE_DEFERRED_OUTPUT == 0) && (TRICE_BUFFER == TRICE_RING_BUFFER)
#error configuration: (TRICE_BUFFER == TRICE_RING_BUFFER) needs (TRICE_DEFERRED_OUTPUT == 1)
#endif

#if (TRICE_DEFERRED_OUTPUT == 0) && (TRICE_BUFFER == TRICE_DOUBLE_BUFFER)
#error configuration: (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) needs (TRICE_DEFERRED_OUTPUT == 1)
#endif

#if (TRICE_DEFERRED_OUTPUT == 1) && (TRICE_BUFFER == TRICE_STACK_BUFFER)
#error configuration: (TRICE_BUFFER == TRICE_STACK_BUFFER) needs (TRICE_DEFERRED_OUTPUT == 0)
#endif

#if (TRICE_DEFERRED_OUTPUT == 1) && (TRICE_BUFFER == TRICE_STATIC_BUFFER)
#error configuration: (TRICE_BUFFER == TRICE_STATIC_BUFFER) needs (TRICE_DEFERRED_OUTPUT == 0)
#endif

#if defined (TRICE_CGO) && (TRICE_CYCLE_COUNTER == 1)
#warning configuration: TRICE_CGO needs TRICE_CYCLE_COUNTER == 0 for successful tests
#endif

#if (TRICE_DIRECT_OUTPUT == 1) && (TRICE_DIRECT_AUXILIARY == 0) && (TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 0) && (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 0)
#error configuration: TRICE_DIRECT_OUTPUT == 1 needs specified output channel
#endif

#if (TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING == 1) && (TRICE_DEFERRED_OUTPUT == 0)
#error configuration: TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING == 1 needs TRICE_DEFERRED_OUTPUT == 1
#endif

#if (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1) && (TRICE_DIRECT_OUTPUT == 0)
#error configuration: TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1 needs TRICE_DIRECT_OUTPUT == 1
#endif

#if (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) && (TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1)
#error configuration: only one direct output channel is possible
#endif

#if (TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1) && (TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1)
#error configuration: only one direct output channel is possible
#endif

#if (TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1) && (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1)
#error configuration: only one RTT output channel is possible
#endif

#if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DEFERRED_BUFFER_SIZE/2 < TRICE_BUFFER_SIZE)
#error configuration: TRICE_DEFERRED_BUFFER_SIZE too small
#endif

#if (TRICE_BUFFER == TRICE_RING_BUFFER) && (TRICE_DEFERRED_BUFFER_SIZE < TRICE_BUFFER_SIZE)
#error configuration: TRICE_DEFERRED_BUFFER_SIZE too small
#endif

#if (TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1) && (TRICE_DIRECT_OUTPUT == 0)
#error configuration: TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1 needs TRICE_DIRECT_OUTPUT == 1
#endif

#if (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) && (TRICE_DIRECT_OUTPUT == 0)
#error configuration: TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1 needs TRICE_DIRECT_OUTPUT == 1
#endif

#if defined(SEGGER_RTT) && (TRICE_BUFFER_SIZE > BUFFER_SIZE_UP)
#error configuration: BUFFER_SIZE_UP too small
#endif

#if defined( TRICE_UARTA ) && ( TRICE_BUFFER != TRICE_RING_BUFFER) && ( TRICE_BUFFER != TRICE_DOUBLE_BUFFER)
#error configuration: direct-only mode needs no TRICE_UARTA
#endif

#if defined( TRICE_UARTB ) && ( TRICE_BUFFER != TRICE_RING_BUFFER) && ( TRICE_BUFFER != TRICE_DOUBLE_BUFFER)
#error configuration: direct-only mode needs no TRICE_UARTA
#endif

#if ( TRICE_BUFFER == TRICE_STACK_BUFFER) && (TRICE_DIRECT_OUTPUT == 0)
#error configuration: direct-only mode needs TRICE_DIRECT_OUTPUT == 1
#endif

#if ( TRICE_BUFFER == TRICE_STATIC_BUFFER) && (TRICE_DIRECT_OUTPUT == 0)
#error configuration: direct-only mode needs TRICE_DIRECT_OUTPUT == 1
#endif

#if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUTPUT == 0) && !defined(TRICE_UARTA) && !defined(TRICE_UARTB) && !defined(TRICE_DEFERRED_AUXILIARY)
#error configuration: deferred-only mode needs TRICE_UART or TRICE_DEFERRED_AUXILIARY
#endif

#if (TRICE_BUFFER == TRICE_RING_BUFFER) && (TRICE_DIRECT_OUTPUT == 0) && !defined(TRICE_UARTA) && !defined(TRICE_UARTB) && !defined(TRICE_DEFERRED_AUXILIARY)
#error configuration: deferred-only mode needs TRICE_UART or TRICE_DEFERRED_AUXILIARY
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

#if (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1)  && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
#error "configuration: TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1 makes only sense, when TRICE_DIRECT_OUT_FRAMING != TRICE_FRAMING_NONE"
#endif

#if (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 0)  && (TRICE_DIRECT_OUT_FRAMING != TRICE_FRAMING_NONE)
#error "configuration: TRICE_DIRECT_OUT_FRAMING != TRICE_FRAMING_NONE demands TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1"
#endif

#if (TRICE_DIRECT_AUXILIARY == 1) && (TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1)
#error configuration: only one direct output channel is possible
#endif

#if (TRICE_DIRECT_AUXILIARY == 1) && (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1)
#error configuration: only one direct output channel is possible
#endif

// function prototypes:

#if defined( TRICE_UARTA )
static void triceNonBlockingWriteUartA( const void * buf, size_t nByte );
#endif

#if defined( TRICE_UARTB )
static void triceNonBlockingWriteUartB( const void * buf, size_t nByte );
#endif

#if TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1
static void SEGGER_Write_RTT0_NoCheck32( const uint32_t* pData, unsigned NumW );
#endif

// global variables:

//! TriceErrorCount is incremented, when data inside the internal trice buffer are corrupted.
//! That could happen, when the buffer wrapped before data are sent. The user app should check this diagnostic value.
unsigned TriceErrorCount = 0; 

//! triceCommand is the command receive buffer.
char triceCommandBuffer[TRICE_COMMAND_SIZE_MAX+1]; // with terminating 0

//! triceCommandFlag ist set, when a command was received completely.
int triceCommandFlag = 0; // updated

#if TRICE_CYCLE_COUNTER == 1

//! TriceCycle is increased and transmitted with each trice message, if enabled.
uint8_t  TriceCycle = 0xc0;

#endif

#if TRICE_DIAGNOSTICS == 1

//! TriceDataOffsetDepthMax measures the max used offset space.
int TriceDataOffsetDepthMax = 0;

//! TriceSingleMaxWordCount is a diagnostics value usable to optimize buffer space TRICE_BUFFER_SIZE for a single trice message.
unsigned TriceSingleMaxWordCount = 0;

//! TriceDynBufTruncateCount couts how often a dynamic buffer was truncated because it was too long.
unsigned TriceDynBufTruncateCount = 0;

#ifdef TRICE_PROTECT

unsigned TriceDirectOverflowCount = 0;
unsigned TriceDeferredOverflowCount = 0;

#endif

#endif

//! TriceInit needs to run before the first trice macro is executed.
//! Not neseecary for all configurations.
void TriceInit( void ){

    #ifdef SEGGER_RTT
        // This is just to force the INIT() call inside SEGGER_RTT.c what allows to use
        // SEGGER_RTT_WriteNoLock or SEGGER_Write_RTT0_NoCheck32 instead of SEGGER_RTT_Write.
        #ifndef TRICE_CGO
            SEGGER_RTT_Write(0, 0, 0 ); //lint !e534
        #endif
    #endif

    #ifdef XTEA_ENCRYPT_KEY
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
size_t triceDataLen( const uint8_t * p ){
    uint16_t nc = TRICE_TTOHS(*(uint16_t*)p); // lint !e826
    size_t n = nc>>8;
    if( n < 128 ){
        return n;
    }
    return nc & 0x7fff;
}

#if (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1) 

//! triceIDAndLen expects at buf a trice message and returns the ID for routing.
//! \param pBuf is where the trice message starts.
//! \param ppStart return the trice data start address. This is pBuf or 2 bytes later.
//! \param triceID is filled positive ID value on success or negative on error.
//! \retval is the netto trice length (without padding bytes), 0 on error.
static size_t triceIDAndLen( uint32_t* pBuf, uint8_t** ppStart, int* triceID ){
    uint16_t* pTID = (uint16_t*)pBuf; // get TID address
    uint16_t TID = TRICE_TTOHS( *pTID ); // type and id
    *triceID = 0x3FFF & TID;
    int triceType = TID >> 14;
    size_t len;
    uint8_t* pStart = (uint8_t*)pBuf;
    switch( triceType ){
        case TRICE_TYPE_S0: // S0 = no stamp
            len = 4 + triceDataLen(pStart + 2); // tyId
            break;
        case TRICE_TYPE_S2: // S2 = 16-bit stamp
            pStart += 2; // see Id(n) macro definition
            len = 6 + triceDataLen(pStart + 4); // tyId ts16
            break;
        case TRICE_TYPE_S4: // S4 = 32-bit stamp
            len = 8 + triceDataLen(pStart + 6); // tyId ts32
            break;
        default:
            //lint -fallthrugh
        case TRICE_TYPE_X0:
            TriceErrorCount++;
            *triceID = -__LINE__; // extended trices not supported (yet)
            return 0;
    }
    *ppStart = pStart;
    return len;
}

#endif // #if (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1)


//! TriceEncode expects at buf trice netto data with netto length len.
//! It fills dst with the next trice data, which are encoded and framed or not, according the selected switches.
//! The areas of dst and buf are allowed to overlap.
//! \param encrypt, when 0, then without encryption, when 1, then with XTEA encryption.
//! \param framing selects if and which framing is used.
//! \param dst is the destination. It must be 32-bit aligned.
//! \param buf is the source. This can be not 32-bit aligned.
//! \param len is the source len.
//! \retval is the encoded len with 0-delimiter byte.
size_t TriceEncode( unsigned encrypt, unsigned framing, uint8_t* dst, const uint8_t * buf, size_t len ){ 
#if TRICE_DIAGNOSTICS == 1
    int distance = buf - dst;
    if(distance < 4 ){
        TriceErrorCount++;
    }
    int TriceDataOffsetDepth = TRICE_DATA_OFFSET - distance;
    TriceDataOffsetDepthMax = TriceDataOffsetDepth < TriceDataOffsetDepthMax ? TriceDataOffsetDepthMax : TriceDataOffsetDepth;
#endif

    size_t encLen;
    const uint8_t * dat;
    if( encrypt ){
        #ifdef XTEA_ENCRYPT_KEY
            // Only multiple of 8 encryptable, but trice data are 32-bit aligned.
            // A 64-bit trice data aligning would waste RAM and costs a bit time during trice generation.
            // We need additional 4 bytes after each trice for the XTEA encryption.
            // Also, the framing could make the trice message a bit longer.
            // Therefore we copy the trice data to a place, we can use.
            // The location for XTEAEncrypt must lay on a 32-bit boundary.
            #if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)
                // special case: The data can be big, are compacted and behind them is space. So we can encrypt them in space
                dat = buf; // That is also for the framing afterwards.
                size_t len8 = (len + 7) & ~7; // Only multiple of 8 encryptable, so we adjust len.
                memset(((uint8_t *)buf)+len, 0, len8 -len); // clear padding space: ATTENTION! OK only for this compiler switch setting. 
                len = len8;
                XTEAEncrypt( (uint32_t *)dat, len8>>2 );
            #else // #if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)

                // Let space in front for framing, free 4 bytes behind do a 32-bit align backwards.
                // uint32_t * loc = (uint32_t *)(((unsigned)dst + TRICE_DATA_OFFSET - 4) & ~3); 
                // The computing above does not work, because, when several Trices, this "free" location
                // drifts into the unprocessed Trice data. So we create a buffer.
                // Not on the stack, because small stacks could be a problem here.
                static uint32_t loc[TRICE_SINGLE_MAX_SIZE>>2];

                // Because dst + TRICE_DATA_OFFSET could be the buf value itself, we need to move at least 4 bytes.
                memmove( loc, buf, len ); // We use not memcpy here, because dst and buf allowed to overlap.
                dat = (const uint8_t *)loc; // That is also for the framing afterwards.
                size_t len8 = (len + 7) & ~7; // Only multiple of 8 encryptable, so we adjust len.
                memset(((uint8_t*)loc)+len, 0, len8 -len); // clear padding space
                len = len8;
                XTEAEncrypt( loc, len8>>2 );
            #endif // #else // #if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE)
        #endif // #ifdef XTEA_ENCRYPT_KEY
    }else{
        dat = buf;
    }
    switch( framing ){
        case TRICE_FRAMING_TCOBS:
            encLen = (size_t)TCOBSEncode(dst, dat, len);
            dst[encLen++] = 0; // Add zero as package delimiter.
            return encLen;
        case TRICE_FRAMING_COBS:
            encLen = (size_t)COBSEncode(dst, dat, len);
            dst[encLen++] = 0; // Add zero as package delimiter.
            return encLen;
        case TRICE_FRAMING_NONE:
            memmove( dst, dat, len );
            encLen = len;
            return encLen;
    }
    return 0; // unexpected
}

#if (TRICE_DIAGNOSTICS ==1) && defined(SEGGER_RTT)

unsigned RTT0_writeDepthMax = 0; //!< RTT0_writeDepthMax is usable for diagnostics.

static void triceSeggerRTTDiagnostics( void ){
    #ifdef TRICE_CGO
        unsigned space = TRICE_BUFFER;
    #else
        unsigned space = SEGGER_RTT_GetAvailWriteSpace (0);
    #endif
    int writeDepth = BUFFER_SIZE_UP - space;
    #ifdef TRICE_PROTECT
        if( writeDepth < 0 ){ // unexpected
            for(;;); 
        }
    #endif
    unsigned wd = (unsigned)writeDepth;
    RTT0_writeDepthMax = wd > RTT0_writeDepthMax ? wd : RTT0_writeDepthMax;
}

#endif // #if (TRICE_DIAGNOSTICS ==1) && defined(SEGGER_RTT)

#if TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1

static void TriceWriteDeviceRtt0( const uint8_t * enc, size_t encLen ){
    #if defined(TRICE_CGO) // automated tests
        TriceWriteDeviceCgo( enc, encLen );
    #else // #if defined(TRICE_CGO)
        SEGGER_RTT_WriteNoLock(0, enc, encLen );
        #if TRICE_DIAGNOSTICS == 1
            triceSeggerRTTDiagnostics(); // todo: maybe not needed
        #endif
    #endif // else // #if defined(TRICE_CGO)
}

#endif // (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) || (TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1)


#if (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) || (TRICE_DIRECT_AUXILIARY == 1)

static void TriceDirectWrite8( const uint8_t * enc, size_t encLen ){
    #if defined(TRICE_CGO) // automated tests
        TriceWriteDeviceCgo( enc, encLen );
    #else // #if defined(TRICE_CGO)
        #if TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1
            SEGGER_RTT_WriteNoLock(0, enc, encLen );
            #if TRICE_DIAGNOSTICS == 1
                triceSeggerRTTDiagnostics(); // todo: maybe not needed
            #endif
        #endif
        #if TRICE_DIRECT_AUXILIARY
            TriceNonBlockingDirectWriteAuxiliary( enc, encLen );
        #endif
    #endif // else // #if defined(TRICE_CGO)
}

#endif // #if (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) || (TRICE_DIRECT_AUXILIARY == 1)

#if TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1
//! SEGGER_Write_RTT0_NoCheck32 was derived from SEGGER_RTT.c version 7.60g function _WriteNoCheck for speed reasons. If using a different version please review the code first.
static void SEGGER_Write_RTT0_NoCheck32( const uint32_t* pData, unsigned NumW ) {
    #if defined(TRICE_CGO) // automated tests
        TriceWriteDeviceCgo( (uint8_t*)pData, NumW<<2 );
    #else // #if defined(TRICE_CGO)
        unsigned NumWordsAtOnce;
        unsigned WrOff;
        unsigned RemW;
        #ifdef TRICE_PROTECT
            unsigned space = SEGGER_RTT_GetAvailWriteSpace (0);
            if( space < NumW<<2 ){
                for(;;);
            }
        #endif
        // Get "to-host" ring buffer.
        static SEGGER_RTT_BUFFER_UP * const pRingUp0 = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[0] + SEGGER_RTT_UNCACHED_OFF);  // Access uncached to make sure we see changes made by the J-Link side and all of our changes go into HW directly
        WrOff = pRingUp0->WrOff;
        RemW = (pRingUp0->SizeOfBuffer - WrOff)>>2;
        volatile uint32_t* pDstW = (uint32_t*)((pRingUp0->pBuffer + WrOff) + SEGGER_RTT_UNCACHED_OFF); //lint !e826
        if (RemW > NumW) { // All data fits before wrap around
            WrOff += NumW<<2;
            while (NumW--) {
                *pDstW++ = *pData++;
            };
            RTT__DMB();                     // Force data write to be complete before writing the <WrOff>, in case CPU is allowed to change the order of memory accesses
            pRingUp0->WrOff = WrOff;
        } else { // We reach the end of the buffer, so need to wrap around
            NumWordsAtOnce = RemW;
            while (NumWordsAtOnce--) {
                *pDstW++ = *pData++;
            };
            pDstW = (uint32_t*)(pRingUp0->pBuffer + SEGGER_RTT_UNCACHED_OFF); //lint !e826
            NumWordsAtOnce = NumW - RemW;
            while (NumWordsAtOnce--) {
                *pDstW++ = *pData++;
            };
            RTT__DMB();                     // Force data write to be complete before writing the <WrOff>, in case CPU is allowed to change the order of memory accesses
            pRingUp0->WrOff = (NumW - RemW)<<2;
        }
        #if TRICE_DIAGNOSTICS == 1
            triceSeggerRTTDiagnostics();
        #endif
    #endif // #else // #if defined(TRICE_CGO)
}
#endif // #if TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1

#if TRICE_DIRECT_OUTPUT == 1

//! TriceNonBlockingDirectWrite copies a single trice from triceStart to output.
//! This is the time critical part, executed inside TRICE_LEAVE.
//! The trice data start at triceStart and include wordCount values with 1-3 padding bytes at the end.
//! In front of triceStart are TRICE_DATA_OFFSET bytes space usable for optional in-buffer encoding.
//! This is NOT the case, when using direct and deferred modes parallel, because for efficient RAM usage
//! there is no gap between the Trices in double or ring buffer. Therefore, when enabling 
//! TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE together with a deferred mode, for efficiency the RTT output can only be
//! unframed, what should not be a problem.
//! The space behind the trice (at triceStart + wordCount) is usable because in direct mode this is writable.
//! Also in combined modes (direct plus deferred) this is allowed, because the next trice is not started yet.
void TriceNonBlockingDirectWrite( uint32_t* triceStart, unsigned wordCount ){

    // The 16-bit stamped trices start with 2-times 16-bit ID for align and speed reasons.
    // The trice tool knows and expects that when switch -packageFraming = NONE was applied.
    // The 2 additional transmit bytes are avoidable then but that would need a 2nd NONE option for the trice tool, what makes usage more confusing.
    // That the TRICE_FRAMING_NONE does not remove the 2 additional bytes for 16-bit stamped trices has the 
    // main reason in the TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE option for the fast 32-bit transfer, what probably will be a common use case.

    // What happens here, is similar to TriceEncode but this is time critical code and we can do in-place encoding too.
    #if (TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 0) && (TRICE_BUFFER != TRICE_RING_BUFFER) && (TRICE_BUFFER != TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    // direct-only mode without routing & framing
        uint32_t * dat;
        unsigned wc;
        #if TRICE_XTEA_DIRECT_ENCRYPT
            wc = ((wordCount + 1) & ~1); // only multiple of 8 can be encrypted 
            XTEAEncrypt( triceStart, wc ); // in-buffer encryption (in direct-only mode is usable space bedind the Trice message.)
        #else
            wc = wordCount;
        #endif
        dat = triceStart;
        #ifdef TRICE_PROTECT
            #ifdef TRICE_CGO
                unsigned space = TRICE_BUFFER;
            #else
                unsigned space = SEGGER_RTT_GetAvailWriteSpace (0);
            #endif
                if( space >= wc<<2 ){
                    SEGGER_Write_RTT0_NoCheck32( dat, wc );
                }else{
                    TriceDirectOverflowCount++;
                }
        #else // #ifdef TRICE_PROTECT
            SEGGER_Write_RTT0_NoCheck32( dat, wc );
        #endif // #else // #ifdef TRICE_PROTECT
        return;

    #elif (TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1) && (TRICE_BUFFER != TRICE_RING_BUFFER) && (TRICE_BUFFER != TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    #error configuration: TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE does not support routing

    #elif (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 0) && (TRICE_BUFFER != TRICE_RING_BUFFER) && (TRICE_BUFFER != TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    // direct-only mode without routing & framing
        unsigned wc;
        unsigned bc;
        #if TRICE_XTEA_DIRECT_ENCRYPT
            wc = ((wordCount + 1) & ~1); // only multiple of 8 can be encrypted 
            XTEAEncrypt( triceStart, wc ); // in-buffer encryption (in direct-only mode is usable space bedind the Trice message.)
            bc = wc<<2;
        #else
            bc = wordCount<<2;
        #endif
        #ifdef TRICE_PROTECT
            #ifdef TRICE_CGO
                unsigned space = TRICE_BUFFER;
            #else
                unsigned space = SEGGER_RTT_GetAvailWriteSpace (0);
            #endif
            if( space >= bc ){
                TriceDirectWrite8( (const uint8_t *)triceStart, bc );
            }else{
                TriceDirectOverflowCount++;
            }
        #else // #ifdef TRICE_PROTECT
            TriceDirectWrite8( (const uint8_t *)triceStart, bc );
        #endif // #else // #ifdef TRICE_PROTECT
        return;

    #elif (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1) && (TRICE_BUFFER != TRICE_RING_BUFFER) && (TRICE_BUFFER != TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    #error configuration: TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE does not support routing

    #elif (TRICE_DIRECT_AUXILIARY == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 0) && (TRICE_BUFFER != TRICE_RING_BUFFER) && (TRICE_BUFFER != TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    // direct-only mode without routing & framing
        unsigned wc;
        unsigned bc;
        #if TRICE_XTEA_DIRECT_ENCRYPT
            wc = ((wordCount + 1) & ~1); // only multiple of 8 can be encrypted 
            XTEAEncrypt( triceStart, wc ); // in-buffer encryption (in direct-only mode is usable space bedind the Trice message.)
            bc = wc<<2;
        #else
            bc = wordCount<<2;
        #endif
        TriceNonBlockingDirectWriteAuxiliary( (const uint8_t *)triceStart, bc );
        return;

    #elif (TRICE_DIRECT_AUXILIARY == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1) && (TRICE_BUFFER != TRICE_RING_BUFFER) && (TRICE_BUFFER != TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    #error configuration: TRICE_DIRECT_AUXILIARY does not support routing

    #elif (TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1)  && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 0) && ((TRICE_BUFFER == TRICE_RING_BUFFER) || (TRICE_BUFFER == TRICE_DOUBLE_BUFFER)) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    // In this twin mode (direct and deferred output) for runtime efficiency, for direct output no encryption is allowed and only package framing NONE is possible (for now).
        #ifdef TRICE_PROTECT
            #ifdef TRICE_CGO
                unsigned space = TRICE_BUFFER;
            #else
                unsigned space = SEGGER_RTT_GetAvailWriteSpace (0);
            #endif
            if( space >= wordCount<<2 ){
                SEGGER_Write_RTT0_NoCheck32( triceStart, wordCount );
            }else{
                TriceDirectOverflowCount++;
            }
        #else // #ifdef TRICE_PROTECT
            SEGGER_Write_RTT0_NoCheck32( triceStart, wordCount );
        #endif // #else // #ifdef TRICE_PROTECT
        return;

    #elif (TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1) && ((TRICE_BUFFER == TRICE_RING_BUFFER) || (TRICE_BUFFER == TRICE_DOUBLE_BUFFER)) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    #error configuration: TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE does not support routing

    #elif (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 0) && ((TRICE_BUFFER == TRICE_RING_BUFFER) || (TRICE_BUFFER == TRICE_DOUBLE_BUFFER)) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    // In this twin mode (direct and deferred output) for runtime efficiency, for direct output no encryption is allowed and only package framing NONE is possible (for now).
        unsigned bc = wordCount<<2;
        #ifdef TRICE_PROTECT
            #ifdef TRICE_CGO
                unsigned space = TRICE_BUFFER;
            #else
                unsigned space = SEGGER_RTT_GetAvailWriteSpace (0);
            #endif
            if( space >= wordCount<<2 ){
                TriceDirectWrite8( (const uint8_t *)triceStart, bc );
            }else{
                TriceDirectOverflowCount++;
            }
        #else // #ifdef TRICE_PROTECT
            TriceDirectWrite8( (const uint8_t *)triceStart, bc );
        #endif // #else // #ifdef TRICE_PROTECT
        return;

    #elif (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1) && ((TRICE_BUFFER == TRICE_RING_BUFFER) || (TRICE_BUFFER == TRICE_DOUBLE_BUFFER)) && (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    #error configuration: In this twin mode (direct and deferred output) no support for direct out routing yet.

    
//  #else
//        #error "invalid configuration"
//    #endif



        //  #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
        //      dat = triceStart;
        //  #else // #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
        //      #if (TRICE_BUFFER == TRICE_RING_BUFFER) || (TRICE_BUFFER == TRICE_DOUBLE_BUFFER)
        //          //! There is no gap between the Trices in double or ring buffer for efficient RAM usage. Therefore, when enabling 
        //          //! TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE together with a deferred mode, for speed the RTT output can only be
        //          //! unframed, what should not be a problem.
        //          #error "Wrong configuration"
        //      #endif
        //      uint8_t* enc = ((uint8_t*)triceStart) - TRICE_DATA_OFFSET;
        //      #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_COBS)
        //      unsigned encLen = COBSEncode(enc, triceStart, wc<<2);
        //      #endif
        //      #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS)
        //      unsigned encLen = TCOBSEncode(enc, triceStart, wc<<2);
        //      #endif
        //      do{
        //          enc[encLen++] = 0; // add 0-delimiter and optional padding zeroes
        //      }while( (encLen & 3) != 0 ); 
        //      dat = (uint32_t *)enc;
        //      wc = encLen>>2;
        //  #endif // #else // #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)

        //  #if defined(TRICE_CGO) // automated tests
        //      TriceWriteDeviceCgo( (uint8_t*)triceStart, wordCount<<2 );
        //  #else // #if defined(TRICE_CGO)
        //      #ifdef TRICE_PROTECT
        //          #ifdef TRICE_CGO
        //              unsigned space = TRICE_BUFFER;
        //          #else
        //              unsigned space = SEGGER_RTT_GetAvailWriteSpace (0);
        //          #endif
        //          if( space >= wc<<2 ){
        //              SEGGER_Write_RTT0_NoCheck32( dat, wc );
        //          }else{
        //              TriceErrorCount++;
        //          }
        //      #else // #ifdef TRICE_PROTECT
        //          SEGGER_Write_RTT0_NoCheck32( dat, wc );
        //      #endif // #else // #ifdef TRICE_PROTECT
        //  #endif // #else // #if defined(TRICE_CGO)
        //  return;
    //#endif // #if TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1 // fast SEGGER RTT
    //  #elif TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1 // normal SEGGER RTT /////////////////////////////////////////////////////////////
    //      // What happens here, is similar to TriceEncode but this is time critical code and we can do in-place encoding too.
    //      uint8_t * dat;
    //      unsigned bc;
    //      #if TRICE_XTEA_DIRECT_ENCRYPT
    //          unsigned wc = ((wordCount + 1) & ~1); // only multiple of 8 can be encrypted 
    //          XTEAEncrypt( triceStart, wc ); // in-buffer encryption
    //          bc = wc << 2;
    //      #else
    //          bc = wordCount << 2;
    //      #endif
//  
    //      #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    //          dat = (uint8_t *)triceStart;
    //      #else // #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
    //          uint8_t* enc = ((uint8_t*)triceStart) - TRICE_DATA_OFFSET;
    //          #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_COBS)
    //          unsigned encLen = COBSEncode(enc, triceStart, bc);
    //          #endif
    //          #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS)
    //          unsigned encLen = TCOBSEncode(enc, triceStart, bc);
    //          #endif
//  
    //          enc[encLen++] = 0; // add 0-delimiter
    //          dat = enc;
    //          bc = encLen;
    //      #endif // #else // #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)
//  
    //      #if defined(TRICE_CGO) // automated tests
    //          TriceWriteDeviceCgo( (uint8_t*)triceStart, wordCount<<2 );
    //      #else
    //          TriceWriteDeviceRtt0( dat, bc );
    //      #endif
    //      return;
    //#endif // #if TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1// normal SEGGER RTT without framing
    #else //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE) 
            #if (TRICE_DIRECT_AUXILIARY == 1)
                TriceNonBlockingDirectWriteAuxiliary( (uint8_t*)triceStart, wordCount<<2 );
                return;
            #endif

            #if defined(TRICE_CGO)
                TriceWriteDeviceCgo( (uint8_t*)triceStart, wordCount<<2 );
            #endif
        #else // #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE) 
            #if (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1) \
                && (    (TRICE_DIRECT_AUXILIARY == 1) \
                    || ((TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1)) \
                    || defined( TRICE_CGO ) )

                uint8_t* triceStart2;
                int triceID;
                size_t len = triceIDAndLen( triceStart, &triceStart2, &triceID );

                #if TRICE_DEFERRED_OUTPUT
                    // When using deferred and direct mode parallel, we need to provide a temporary buffer here.
                    // That's, because here in the direct processing, we cannot use deferred buffer space, which is not read out yet.
                    static uint8_t enc[TRICE_BUFFER_SIZE];
                #else
                    uint8_t* enc = triceStart2 - TRICE_DATA_OFFSET;
                #endif

                size_t encLen = TriceEncode(TRICE_XTEA_DIRECT_ENCRYPT, TRICE_DIRECT_OUT_FRAMING, enc, triceStart2, len );

                #if TRICE_DIRECT_AUXILIARY == 1
                    #if defined(TRICE_DIRECT_AUXILIARY_MIN_ID) && defined(TRICE_DIRECT_AUXILIARY_MAX_ID)
                    if( (TRICE_DIRECT_AUXILIARY_MIN_ID < triceID) && (triceID < TRICE_DIRECT_AUXILIARY_MAX_ID) )
                    #endif
                    { TriceNonBlockingDirectWriteAuxiliary( enc, encLen ); }
                #endif
                
                #if ((TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) && (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1))
                    #if defined(TRICE_SEGGER_RTT_ROUTED_8BIT_DIRECT_WRITE_MIN_ID) && defined(TRICE_SEGGER_RTT_ROUTED_8BIT_DIRECT_WRITE_MAX_ID)
                    if( (TRICE_SEGGER_RTT_ROUTED_8BIT_DIRECT_WRITE_MIN_ID < triceID) && (triceID < TRICE_SEGGER_RTT_ROUTED_8BIT_DIRECT_WRITE_MAX_ID) )
                    #endif
                    { TriceWriteDeviceRtt0( enc, encLen ); }
                #endif
                
                #ifdef TRICE_CGO
                    TriceWriteDeviceCgo( enc, encLen );
                #endif

            #else // #if (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1) && ...
                #error unexpected configuration
            #endif // #else // #if (TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1)
        #endif // #else // #if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE)

        #endif //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif // #if TRICE_DIRECT_OUTPUT == 1

#if TRICE_DEFERRED_OUTPUT

// TriceNonBlockingDeferredWrite8 routes trice data to output channels.
void TriceNonBlockingDeferredWrite8( int triceID, const uint8_t * enc, size_t encLen ){
    
    #if defined(TRICE_CGO) // automated tests
        TriceWriteDeviceCgo( enc, encLen );
    #else // #if defined(TRICE_CGO) // automated tests
        #if defined( TRICE_UARTA )
            #if defined(TRICE_UARTA_MIN_ID) && defined(TRICE_UARTA_MAX_ID)
            if( (TRICE_UARTA_MIN_ID < triceID) && (triceID < TRICE_UARTA_MAX_ID) )
            #endif
            { triceNonBlockingWriteUartA( enc, encLen ); }
        #endif
        #if defined( TRICE_UARTB )
            #if defined(TRICE_UARTB_MIN_ID) && defined(TRICE_UARTB_MAX_ID)
            if( (TRICE_UARTB_MIN_ID < triceID) && (triceID < TRICE_UARTB_MAX_ID) )
            #endif
            { triceNonBlockingWriteUartB( enc, encLen ); }
        #endif
        #if ( TRICE_DEFERRED_AUXILIARY == 1)
            #if defined(TRICE_DEFERRED_AUXILIARY_MIN_ID) && defined(TRICE_DEFERRED_AUXILIARY_MAX_ID)
            if( (TRICE_DEFERRED_AUXILIARY_MIN_ID < triceID) && (triceID < TRICE_DEFERRED_AUXILIARY_MAX_ID) )
            #endif
            { TriceNonBlockingDeferredWrite8Auxiliary( enc, encLen ); }
        #endif
        #ifdef TRICE_CGO
            
        #endif
        #if (TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1)
            #if defined(TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE_MIN_ID) && defined(TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE_MAX_ID)
            if( (TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE_MIN_ID < triceID) && (triceID < TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE_MAX_ID) )
            #endif
            TriceWriteDeviceRtt0( enc, encLen );
        #endif
        //  #ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
        //      #if defined(TRICE_MODBUS_MIN_ID) && defined(TRICE_MODBUS_MAX_ID)
        //      if( (TRICE_MODBUS_MIN_ID < triceID) && (triceID < TRICE_MODBUS_MAX_ID) )
        //      #endif
        //      TriceWriteDeviceModbus( enc, encLen );
        //  #endif
    #endif //  #else // #if defined(TRICE_CGO) // automated tests
} //lint !e715 Info 715: Symbol 'triceID' not referenced

#endif // #if TRICE_DEFERRED_OUTPUT

#if defined( TRICE_UARTA ) // deferred out to UART

//! triceOutBufferUartA points into the double or ring buffer to the next (encoded) trice package.
static const uint8_t * triceOutBufferUartA;

//! triceOutCountUartA is the not yet transmitted byte count after a triceNonBlockingWriteUartA() call.
static size_t triceOutCountUartA = 0;

//! triceOutIndexUartA is the triceOutBufferUartA offset to the next to transmit byte.
static unsigned triceOutIndexUartA = 0;

//! triceNonBlockingWriteUartA registers a buffer for TRICE_UARTA transmission.
//! \param buf is byte buffer start.
//! \param nByte is the number of bytes to transfer
static void triceNonBlockingWriteUartA( const void * buf, size_t nByte ){
    triceOutBufferUartA = buf;
    triceOutIndexUartA = 0;
    triceOutCountUartA = nByte;
}

//! TriceOutDepthUartA returns the amount of bytes not written yet to UARTB.
unsigned TriceOutDepthUartA( void ){
    unsigned depth = triceOutCountUartA - triceOutIndexUartA;
    return depth;
}

//! TriceNextUint8UartA returns the next trice byte for transmission to TRICE_UARTA.
uint8_t TriceNextUint8UartA( void ){
    #if 0 // debugging
        uint8_t * d = (uint8_t *)&(triceOutBufferUartA[triceOutIndexUartA]);
        uint8_t b = triceOutBufferUartA[triceOutIndexUartA++];
        *d = 0xdd;
        return b;
    #else
        return triceOutBufferUartA[triceOutIndexUartA++];
    #endif
}

//! triceServeTransmitUartA must be called cyclically to proceed ongoing write out.
//! A good place is UARTA ISR.
void triceServeTransmitUartA(void) {
    triceTransmitData8UartA(TriceNextUint8UartA());
    if (0 == TriceOutDepthUartA()) { // no more bytes
        triceDisableTxEmptyInterruptUartA();
    }
}

// triceTriggerTransmitUartA must be called cyclically to initialize write out.
void triceTriggerTransmitUartA(void){
    if( TriceOutDepthUartA() && triceTxDataRegisterEmptyUartA() ){
        triceEnableTxEmptyInterruptUartA(); // next bytes
    }
}

#endif // #if defined( TRICE_UARTA )

#if defined( TRICE_UARTB ) // deferred out to UART

//! triceOutBufferUartB holds the uart out buffer address.
static const uint8_t * triceOutBufferUartB;

//! triceOutCountUartB holds th uarts out buffer size.
static size_t triceOutCountUartB = 0;

//! triceOutIndexUartB is the next to transmit byte index.
static unsigned triceOutIndexUartB = 0;

//! triceNonBlockingWriteUartB registers a buffer for TRICE_UARTB transmission.
//! \param buf is byte buffer start.
//! \param nByte is the number of bytes to transfer
static void triceNonBlockingWriteUartB( const void * buf, size_t nByte ){
    triceOutBufferUartB = buf;
    triceOutIndexUartB = 0;
    triceOutCountUartB = nByte;
}

//! TriceOutDepthUartB returns the amount of bytes not written yet to UARTB.
unsigned TriceOutDepthUartB( void ){
    // unsigned depthRtt0 = 0; -> assuming RTT is fast enough
    unsigned depth = triceOutCountUartB - triceOutIndexUartB;
    return depth;
}

//! TriceNextUint8UartB returns the next trice byte for transmission to TRICE_UARTA.
uint8_t TriceNextUint8UartB( void ){
    return triceOutBufferUartB[triceOutIndexUartB++];
}

//! triceServeTransmitUartB must be called cyclically to proceed ongoing write out.
//! A good place is UARTA ISR.
void triceServeTransmitUartB(void) {
    triceTransmitData8UartB(TriceNextUint8UartB());
    if (0 == TriceOutDepthUartB()) { // no more bytes
        triceDisableTxEmptyInterruptUartB();
    }
}

// triceTriggerTransmitUartB must be called cyclically to initialize write out.
void triceTriggerTransmitUartB(void){
    if( TriceOutDepthUartB() && triceTxDataRegisterEmptyUartB() ){
        triceEnableTxEmptyInterruptUartB(); // next bytes
    }
}

#endif // #if defined( TRICE_UARTB )

//! TriceOutDepth returns the amount of bytes not written yet from the slowest device.
unsigned TriceOutDepth( void ){
    unsigned d = 0, depth = 0;
    #ifdef SEGGER_RTT
        // When no RTT host is connected, the RTT buffer runs full.
        // If a RTT host is connected, it is assumed to be the fastest device.
    #endif
    #if defined( TRICE_UARTA ) 
        d = TriceOutDepthUartA(); //lint !e838
        depth = d > depth ? d : depth;
    #endif
    #if defined( TRICE_UARTB )
        d = TriceOutDepthUartB();
        depth = d > depth ? d : depth;
    #endif
    #ifdef TRICE_CGO
        d = TriceOutDepthCGO();
        depth = d > depth ? d : depth;
    #endif
    depth = d > depth ? d : depth;
    return depth;
}

#ifdef TRICE_CLEAN
#else // #ifdef TRICE_CLEAN

//! TRICE_ASSERT writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! This is a helper macro and should not be used inuser code.
#define TRICE_ASSERT( tid ) \
    TRICE_ENTER tid; TRICE_CNTC(0); \
    TRICE_LEAVE

void triceAssertTrue( int idN, char* msg, int flag ){
    if( !flag ){
        TRICE_ASSERT( id( idN ) );
    }
}

void TriceAssertTrue( int idN, char* msg, int flag ){
    if( !flag ){
         TRICE_ASSERT( Id( idN ) );
    }
}

void TRiceAssertTrue( int idN, char* msg, int flag ){
    if( !flag ){
         TRICE_ASSERT( ID( idN ) );
    }
}

void triceAssertFalse( int idN, char* msg, int flag ){
    if( flag ){
        TRICE_ASSERT( id( idN ) );
    }
}

void TriceAssertFalse( int idN, char* msg, int flag ){
    if( flag ){
         TRICE_ASSERT( Id( idN ) );
    }
}

void TRiceAssertFalse( int idN, char* msg, int flag ){
    if( flag ){
         TRICE_ASSERT( ID( idN ) );
    }
}

#endif // #else // #ifdef TRICE_CLEAN
