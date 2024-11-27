# TRICE Target Code Implementation

<details><summary>Table of Contents</summary><ol><!-- TABLE OF CONTENTS START -->

<!-- 
Table of Contents Generation:
- Install vsCode extension "Markdown TOC" from dumeng 
- Use Shift-Ctrl-P "markdownTOC:generate" to get the automatic numbering.
- replace "<a id=" with "<a id=" 
-->

<!-- vscode-markdown-toc -->
* 1. [TRICE Macro structure](#trice-macro-structure)
  * 1.1. [`TRICE_ENTER`](#`trice_enter`)
  * 1.2. [`TRICE_PUT`](#`trice_put`)
  * 1.3. [`TRICE_LEAVE`](#`trice_leave`)
* 2. [`TRICE_STACK_BUFFER`](#`trice_stack_buffer`)
* 3. [`TRICE_STATIC_BUFFER`](#`trice_static_buffer`)
* 4. [`TRICE_DOUBLE_BUFFER`](#`trice_double_buffer`)
* 5. [`TRICE_RING_BUFFER`](#`trice_ring_buffer`)
* 6. [Deferred Out](#deferred-out)
  * 6.1. [Double Buffer](#double-buffer)
  * 6.2. [Ring Buffer](#ring-buffer)
* 7. [Direct Transfer](#direct-transfer)
* 8. [Some Thoughts](#some-thoughts)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

<div id="top"></div></ol></details><!-- TABLE OF CONTENTS END -->

##  1. <a id='trice-macro-structure'></a>TRICE Macro structure

###  1.1. <a id='`trice_enter`'></a>`TRICE_ENTER`

- Optionally disable interrupts.
- Prepare `TriceBufferWritePosition` and keep its initial value.

###  1.2. <a id='`trice_put`'></a>`TRICE_PUT`

- Use and increment `TriceBufferWritePosition`.

###  1.3. <a id='`trice_leave`'></a>`TRICE_LEAVE`

- Use `TriceBufferWritePosition` and its initial value for data transfer
- Optionally restore interrupt state.

##  2. <a id='`trice_stack_buffer`'></a>`TRICE_STACK_BUFFER`

- `TRICE_ENTER`: Allocate stack
- `TRICE_LEAVE`: Call TriceDirectOut()

##  3. <a id='`trice_static_buffer`'></a>`TRICE_STATIC_BUFFER`

- This is like `TRICE_STACK_BUFFER` but avoids stack allocation, what is better for many stacks.
- `TRICE_ENTER`: Set TriceBufferWritePosition to buffer start.
- `TRICE_LEAVE`: Call TriceDirectOut().

##  4. <a id='`trice_double_buffer`'></a>`TRICE_DOUBLE_BUFFER`

- `TRICE_ENTER`: Keep TriceBufferWritePosition.
- `TRICE_LEAVE`: Optionally call TriceDirectOut().

##  5. <a id='`trice_ring_buffer`'></a>`TRICE_RING_BUFFER`

- `TRICE_ENTER`: Keep or wrap TriceBufferWritePosition and add offset.
- `TRICE_LEAVE`: Optionally call TriceDirectOut().

The `TRICE_RING_BUFFER` allocates incremental ring buffer space and each trice location is read by a deferred task.

##  6. <a id='deferred-out'></a>Deferred Out

###  6.1. <a id='double-buffer'></a>Double Buffer

- TriceTransfer
  - TriceOut
  - TriceNonBlockingWrite( triceID, enc, encLen );
  
###  6.2. <a id='ring-buffer'></a>Ring Buffer

- TriceTransfer
  - lastWordCount = TriceSingleDeferredOut(addr);
    - int triceID = TriceIDAndBuffer( pData, &wordCount, &pStart, &Length );
    - TriceNonBlockingWrite( triceID, pEnc, encLen );

##  7. <a id='direct-transfer'></a>Direct Transfer

- TRICE_LEAVE
  - TriceDirectWrite(triceSingleBufferStartWritePosition, wordCount);
    - optional RTT32 with optional XTEAwithCOBS
    - optional RTT8  with optional XTEAwithCOBS
    - optional
      - triceIDAndLen
      - triceDirectEncode
      - triceNonBlockingDirectWrite

##  8. <a id='some-thoughts'></a>Some Thoughts

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

<p align="right">(<a href="#top">back to top</a>)</p>