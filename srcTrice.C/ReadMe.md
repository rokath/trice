# C sources (instrumentation code)
## Files

 name                              | usage
:----------------------------------|----------------------------------------------------------------------
trice.c                            | needed core instrumentation, add to project
trice.h                            | needed interface, include in project files using TRICE macros
triceConfig.h                      | needed configuration file, copy in project directory and adapt
triceConfigCompiler.h              | needed target compiler specific definitions, extend for project compiler
triceCConfigriticalSection.h       | needed target specific definitions for interrupt handling
triceConfigTx.h                    | only needed for single byte transfer
triceConfigTxInterrupt.h           | only needed for single byte transfer with interrupts
triceWrite.c                       | only needed if no write function can be used - do not use if `triceWriteSeggerRTT.c` is used
triceWrite.h                       | used internally
triceWriteSeggerRTT.c              | can be used instead of normal write - do not use if `triceWrite.c`is used
triceRuntimeGeneratedStrings.c     | only needed for `triceRuntimeGeneratedString()` implementation, when `TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT` is enabled
tricePrintfAdapter.c               | only needed for easy adaption of legacy projects. 
xteaCrypto.c                       | only for trice encryption, does not work with triceRuntimeGeneratedStringsMany.c
xteaCrypto.h                       | only internal used
triceCheck.c                       | only demo code
triceCheck.h                       | only demo code header

### Interface
todo
## Needed stuff
- trice.h must be included in source files using TRICE macros.
- trice.c needs to be added to your project if trices are enabled.
- triceConfig.h should be copied separately in each project. It contains project specific settings.
## Add Ons
- triceCheck.* is just for demonstration purposes and not needed.
- xteaCrypto.c is needed as part of your project only if you wich to use the encryption option.
## Software structure
### Trices
- A TRICE macro consists of one ore more basic trices.
  - A basic trice is 4 bytes long: a 2-byte id and 2 bytes value or padding.
  - A trice macro with a 64 bit data value consists therefore of 4 basic trices, for example.
- All basic trices are buffered in a 32 bit wide trice fifo (size must be a power of 2).
  - This is the least memory consuming option for storing trices.
  - The needed power of 2 size results fom the maximum trice burst rate.
- The 4-byte basic trices are transmitted as 8-byte trice packages.
```
    trice package: header without data packages
    |--------------------------------- fixed packet start0 byte 0xeb 
    |   |----------------------------- client address (local address byte)
    |   |   |------------------------- server address (destination)
    |   |   |   |--------------------- exclusive-or checksum byte
    |   |   |   |   |----------------- ID low part
    |   |   |   |   |   |------------- ID high part
    |   |   |   |   |   |   |--------- Value Low part
    |   |   |   |   |   |   |   |----- Value High part
    v   v   v   v   v   v   v   v
    0xeb cad sad cr8 idL idH vaL  vaH
```
- The transmission is performed using a function `int triceWrite( void* p, int nbytes );` which behaves like an ordinary of file write funktion without file handler.
  - The `triceWrite()` needs to be provided externally for a successful port to a new environment.
  - The triceConfig.h must set `TRICE_BYTES_PER_SECOND` to a proper value to avoid write buffer overflow.
    - For example with 9600 Baud theoretically this value is about 960 but in reality may be only 800 because of processor overhead.
    - This value decreases also if the write port has to transmit other data as well.
    - `TRICE_BYTES_PER_SECOND` is used to compute the repetition rate for calls of `triceWrite`.
      - Example: TRICE_BYTES_PER_SECOND*1000/8bytes gives a repetition rate of 10 ms for a value of 800.
      - So a read from trice fifo and the acompanying write call should happen not more often than every 10 ms in this example.

### Runtime generated strings (`#define TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT`)
- This option is used only if strings unknown at compile time are to be transfered. 
- The `TRICE_P` and `TRICE_S` macros both are using finally the `triceRuntimeGeneratedString()` function.
  - triceRuntimeGeneratedStringsMany.c implements this function for many such strings and transmits them buffers, what is more efficient but needs more code.
  - triceRuntimeGeneratedStringsRare.c implements this function for few such strings and transmits them as trice messages.

### `printf()` adapter
- This option is for easy adaption of legacy projects. 

#### `triceWrite()` implementation
- The size of the internal `triceWrite()` buffer must have sufficient space.



