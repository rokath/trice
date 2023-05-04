//! \file triceModbusBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if 0

#ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
void TriceWriteDeviceModbus( uint8_t *buf, size_t len ){
    TriceNonBlockingWriteModbusBuffer( buf, len ); 
}
#endif

void TriceNonBlockingWriteModbusBuffer( uint8_t const * buf, unsigned len );
size_t TriceModbusAlsoFetch( int index, uint8_t* tBuf );
size_t TriceModbusOnlyFetch( int index, uint8_t* tBuf );

//#if defined(TRICE_LOG_OVER_MODBUS_FUNC24) && (TRICE_MODE != TRICE_STREAM_BUFFER)
//#error TRICE_LOG_OVER_MODBUS_FUNC24 works only with TRICE_MODE == TRICE_STREAM_BUFFER.
//#endif

///////////////////////////////////////////////////////////////////////////////
// Modbus support s not working yet.
// 
//  
//  //! TRICE_LOG_OVER_MODBUS_FUNC24_ALSO allows to access the trice messages over modbus.
//  //! TRICE_LOG_OVER_MODBUS_FUNC24_ONLY works with all TRICE_MODE settings.
//  //! Other trice output channels are supported in this mode.
//  //! See comment on TriceModbusAlsoFetch() for more details.
//  #define TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
//  
//  //! TRICE_LOG_OVER_MODBUS_FUNC24_ONLY allows to access the trice messages over modbus.
//  //! TRICE_LOG_OVER_MODBUS_FUNC24_ONLY works only with TRICE_MODE == TRICE_STREAM_BUFFER.
//  //! Other trice output channels are not supported in this mode.
//  //! See comment on TriceModbusOnlyFetch() for more details.
//  //#define TRICE_LOG_OVER_MODBUS_FUNC24_ONLY
//  
//  //! TRICE_LOG_FIFO_MODBUS_START_ADDRESS is the used virtual modbus address for modbus trice fifo read out.
//  //! The trice tool assumes 47400 as default value. The limit is 47400+(TRICE_FIFO_ELEMENTS>>1).
//  //! If you change this here you need to use the appropriate trice tool CLI switch.
//  #define TRICE_LOG_FIFO_MODBUS_START_ADDRESS 47400
//  
//  
//  #ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
//  #define TRICE_MODBUS_MIN_ID 1           //!< TRICE_MODBUS_BUFFER_MIN_ID is the smallest ID transferred to MODBUS_BUFFER.
//  #define TRICE_MODBUS_MAX_ID ((1<<14)-1) //!< TRICE_MODBUS_BUFFER_MAX_ID is the biggest  ID transferred to MODBUS_BUFFER.
//  
//  #define TRICE_MODBUS_BUFFER_SIZE 240
//  #define TRICE_MODBUS_FIFO_ELEMENTS 32 //!< Must be a power of 2. The half number is the amount of bufferable trices before they go out.
//  #endif // #ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO

//
///////////////////////////////////////////////////////////////////////////////


#ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
#error
#if TRICE_MODBUS_BUFFER_SIZE < 2*(TRICE_SINGLE_MAX_SIZE+TRICE_DATA_OFFSET)
#error
#endif

static uint32_t triceModbusBufferHeap[TRICE_MODBUS_BUFFER_SIZE>>2] = {0}; //!< triceModbusBufferHeap is a kind of heap for trice messages for modbus.
static uint32_t* triceModbusBufferWritePosition = triceModbusBufferHeap; //!< TriceModbusBufferWritePosition is the active write position.
static uint32_t* triceModbusBufferWriteLimit  =  &triceModbusBufferHeap[TRICE_MODBUS_BUFFER_SIZE>>2]; //!< triceModbusBufferWriteLimit is the triceBuffer written limit. 

// TRICE_MODBUS_FIFO_MAX_DEPTH is the max possible count of values the triceFifo can hold.
#define TRICE_MODBUS_FIFO_MAX_DEPTH (TRICE_MODBUS_FIFO_ELEMENTS-1)

//! triceModbusFifo holds up to TRICE_MODBUS_FIFO_MAX_DEPTH uint32_t* values.
static uint32_t* triceModbusFifo[TRICE_MODBUS_FIFO_ELEMENTS];

//! triceModbusFifoWriteIndex indexes the next write position.
static unsigned triceModbusFifoWriteIndex = 0;

//! triceModbusFifoReadIndex indexes the next read position.
static unsigned triceModbusFifoReadIndex = 0;

//! triceModbusFifoDepth returns the count of elements stored inside triceFifo.
static int triceModbusFifoDepth( void ){
    return TRICE_MODBUS_FIFO_MAX_DEPTH & (triceModbusFifoWriteIndex - triceModbusFifoReadIndex);
}

//! triceModbusFifoPush stores v in triceFifo.
//! There is no depth check.
void TriceModbusFifoPush( void* v ){
    triceModbusFifo[triceModbusFifoWriteIndex++] = v;
    triceModbusFifoWriteIndex &= TRICE_MODBUS_FIFO_MAX_DEPTH;
}

//  //! triceModbusFifoPop reads next value from triceFifo.
//  //! There is no depth check.
//  static uint32_t* triceModbusFifoPop( void ){
//      uint32_t* v = triceModbusFifo[triceModbusFifoReadIndex++];
//      triceModbusFifoReadIndex &= TRICE_MODBUS_FIFO_MAX_DEPTH;
//      return v;
//  }

//! triceModbusFifoDiscard2 dismisses next 2 values from triceModbusFifo.
//! There is no depth check.
static void triceModbusFifoDiscard2( void ){
    triceModbusFifoReadIndex++;
    triceModbusFifoReadIndex++;
    triceModbusFifoReadIndex &= TRICE_MODBUS_FIFO_MAX_DEPTH;
}

//! triceNextModbsuBuffer returns a usable address with at least len space.
static uint32_t* triceNextModbusBuffer( size_t len ){
    
    // todo: This check here is wrong! Need to be checked when Modbus support is implemented.
    //if( TriceBufferWritePosition > triceModbusBufferWriteLimit ){
    //    for(;;); // trice modbus buffer overflow
    //}
    
    if( (int)triceModbusBufferWriteLimit - (int)TriceBufferWritePosition > len ){
        return triceModbusBufferWritePosition; // enough space at buffer end
    }else{
        return triceModbusBufferHeap; // buffer wrap
    }
}

//  //! triceLen returns the trice byte count ready for transfer including padding bytes.
//  //! The trice data start at tb + TRICE_DATA_OFFSET.
//  //! The returned depth is without the TRICE_DATA_OFFSET offset.
//  static size_t triceDepth( uint32_t const* tBuf ){
//      uint32_t* limit = triceFifoPop(); 
//      size_t depth = (limit - tBuf)<<2;
//      return depth - TRICE_DATA_OFFSET;
//  }

static size_t triceModbusFifoDepthMax = 0; //!< triceModbusFifoDepthMax is used for diagnostics.
//static size_t triceModbusBufferDepthMax = 0; //!< triceModbusBufferDepth is used for diagnostics.

//  static size_t modbusBufferDepth( uint32_t* tBuf ){
//      int depth;
//      if( tBuf < triceModbusBufferWritePosition ){
//          depth = (int)triceModbusBufferWritePosition - (int)tBuf;
//      }else{
//          int top  = (int)triceModbusBufferWritePosition - (int)triceModbusBufferHeap;
//          int bottom = (int)triceModbusBufferWriteLimit - (int)tBuf;
//          depth = top + bottom;
//      }
//      return (size_t)depth;
//  }

//  //! TriceTransfer, if possible, initiates a write.
//  //! It is the responsibility of the app to call this function.
//  void TriceTransfer( void ){
//      if( TriceOutDepth() == 0 ){ // transmission done, so a new is possible
//          size_t depth = triceFifoDepth();
//          triceFifoDepthMax = depth < triceFifoDepthMax ? triceFifoDepthMax : depth; // diagnostics
//          if( depth >= 2 ){ // data in triceFifo
//              uint32_t* tBuf = triceFifoPop(); 
//              size_t tLen = triceDepth(tBuf); // calls internally triceFifoPop(), tlen is always a multiple of 4
//              size_t triceStreamBufferDepth = streamBufferDepth(tBuf);
//              TriceOut( tBuf, tLen );
//              triceStreamBufferDepthMax = triceStreamBufferDepth < triceStreamBufferDepthMax ? triceStreamBufferDepthMax : triceStreamBufferDepth; // diagnostics
//          } // else: nothing to transfer
//      } // else: transmission not done yet
//  }


void TriceNonBlockingWriteModbusBuffer( uint8_t const * buf, unsigned len ){
    uint32_t* dest = triceNextModbusBuffer(len);
    memcpy(dest, buf, len);
    
    if( triceModbusFifoDepth() >= triceModbusFifoDepthMax ){
        triceModbusFifoDiscard2();
    }
    TriceModbusFifoPush(dest);
    TriceModbusFifoPush(dest + ((len+3)>>2) );
}


#if TRICE_CYCLE_COUNTER == 0
#error TRICE_CYCLE_COUNTER is needed for TRICE_LOG_OVER_MODBUS_FUNC24
#endif

//! triceCountInsideModbusBuffer delivers the cout of trices despite of their length inside stream buffer.
static int triceCountInsideModbusBuffer( void ){
    return triceModbusFifoDepth() >> 1;
}

//! triceModbusFifoFetch returns the nth value from trice modbus fifo. There is no check concerning triceModbusFifo depth.
//! The fifo is not changed.
static uint32_t* triceModbusFifoFetch( unsigned n ){
    unsigned fi = (triceModbusFifoReadIndex + n) & TRICE_MODBUS_FIFO_MAX_DEPTH;
    uint32_t* v = triceModbusFifo[fi];
    return v;
}

//! TriceModbusAlsoFetch returns size of indexed trice message inside TriceModbusFifo and copies its values to tBuf.
//! When a modbus FC24 read request at index arrives, TriceFetch is needs to be called by the user.
//! The host uses the modbus fifo address offset to TRICE_LOG_FIFO_MODBUS_START_ADDRESS to select the index.
//! When TriceFetch returned 0, the host needs to read the previous index, if >=0, to
//! check if the data are unchanged there. For that the TRICE_CYCLE_COUNTER value is used.
//! When TriceFetch returned >0, the host needs to read the next index < (TRICE_MODBUS_FIFO_MAX_DEPTH>>1).
//! to check if more data arrived. This way the host has to poll the last 2 index fifos cyclically.
//! The modbus interface itself is not part of the trice kernel and is let to the users reponsibility.
//! Anyway, the trice tool will be able (hopefully soon) to read trice messages from modbus.
size_t TriceModbusAlsoFetch( int index, uint8_t* tBuf ){
    int tCnt = triceCountInsideModbusBuffer();
    if( index >= tCnt ){
        return 0;
    }
    unsigned n = index << 1;
    uint32_t* triceModbusBufferStart = triceModbusFifoFetch( n );
    uint32_t* triceModbusBufferLimit = triceModbusFifoFetch( n + 1 );
    size_t len = (triceModbusBufferLimit - triceModbusBufferStart) << 2;
    memcpy(tBuf, triceModbusBufferStart, len);
    return len;
}

#endif // #ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO

#endif
