# TRICE Target Code Implementation

## TRICE Macro structure

### `TRICE_ENTER`

- Optionally disable interrupts.
- Prepare `TriceBufferWritePosition` and keep its initial value.

### `TRICE_PUT`

- Use and increment `TriceBufferWritePosition`.

### `TRICE_LEAVE`

- Use `TriceBufferWritePosition` and its initial value for data transfer
- Optionally restore interrupt state.

## `TRICE_STACK_BUFFER`

- `TRICE_ENTER`: Allocate stack
- `TRICE_LEAVE`: Call TriceDirectOut()

## `TRICE_STATIC_BUFFER`

- This is like `TRICE_STACK_BUFFER` but avoids stack allocation, what is better for many stacks.
- `TRICE_ENTER`: Set TriceBufferWritePosition to buffer start.
- `TRICE_LEAVE`: Call TriceDirectOut().

## `TRICE_DOUBLE_BUFFER`

- `TRICE_ENTER`: Keep TriceBufferWritePosition.
- `TRICE_LEAVE`: Optionally call TriceDirectOut().

## `TRICE_RING_BUFFER`

- `TRICE_ENTER`: Keep or wrap TriceBufferWritePosition and add offset.
- `TRICE_LEAVE`: Optionally call TriceDirectOut().

The `TRICE_RING_BUFFER` allocates incremental ring buffer space and each trice location is read by a deferred task.

## Deferred Out

### Double Buffer

- TriceTransfer
  - TriceOut
  - TriceNonBlockingWrite( triceID, enc, encLen );
  
### Ring Buffer

- TriceTransfer
  - lastWordCount = TriceSingleDeferredOut(addr);
    - int triceID = TriceIDAndBuffer( pData, &wordCount, &pStart, &Length );
    - TriceNonBlockingWrite( triceID, pEnc, encLen );

## Direct Transfer

- TRICE_LEAVE
  - TriceDirectWrite(triceSingleBufferStartWritePosition, wordCount);
    - optional RTT32 with optional XTEAwithCOBS
    - optional RTT8  with optional XTEAwithCOBS
    - optional
      - triceIDAndLen
      - triceDirectEncode
      - triceNonBlockingDirectWrite

## Some Thoughs

There have been 3 similar implementations for trice encode

```C
static size_t triceDirectEncode(   uint8_t* enc, const uint8_t * buf, size_t len );
       size_t TriceDeferredEncode( uint8_t* enc, const uint8_t * buf, size_t len );

unsigned TriceEncryptAndCobsFraming32( uint32_t * const triceStart, unsigned wordCount ){
```

Now:

```C
size_t TriceEncode( unsigned encrypt, unsigned framing, uint8_t* dst, const uint8_t * buf, size_t len ){ 
unsigned TriceEncryptAndCobsFraming32( uint32_t * const triceStart, unsigned wordCount ){
```

Currently there are 3 similar implementations for trice buffer reads

```C
static size_t triceIDAndLen(    uint32_t* pBuf,               uint8_t** ppStart, int*      triceID );
static int    TriceNext(        uint8_t** buf,                size_t* pSize,     uint8_t** pStart,    size_t* pLen );
static int    TriceIDAndBuffer( uint32_t const * const pData, int* pWordCount,   uint8_t** ppStart,   size_t* pLength );
```

- The TriceID is only needed for routing and can co in a global variable just for speed.
- The source buffer should be `uint32_t const * const`.
- The destination should be given with `uint32_t * const` and the return value is the trice netto size. For efficiency the result should be ready encoded.

```C
//! \param pTriceID is filled with ID for routing
//! \param pCount is used for double or ring buffer to advance inside the buffer
//! \param dest provides space for the encoded trice
//! \param src is the location of the trice message we want encode
//! \retval is the netto size of the encoded trice data
size_t TriceEncode(int* pTriceID, unsigned int pCount, uint32_t * const dest, uint32_t const * const src );
```

- This function interface is used for all cases.
- First we use the existing code for implementation and then we clean the code.

