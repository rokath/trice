
<!-- TABLE OF CONTENTS -->
<details><summary>Table of Contents</summary><ol>

<!-- steps: 
- Generate TOC for Markdown -> This makes correct numbering for the headlines.
- Clear TOC, Set cursor and Markdown All In Once: Create TOC -> This makes correct links.
- The links only work inside the browser and with up-folded sections
-->
<!-- vscode-markdown-toc -->
- [*Trice*  Version 1.0 Specification (Draft)](#trice--version-10-specification-draft)
  - [1.  Trice User Interface - Quick Start](#1--trice-user-interface---quick-start)
    - [1.1. User Code Adaption](#11-user-code-adaption)
    - [1.2. Limitations](#12-limitations)
    - [1.3. Trice (Time) Stamps](#13-trice-time-stamps)
    - [1.4. Trice Parameter Bit Widths](#14-trice-parameter-bit-widths)
  - [2. Trice Binary Data Format](#2-trice-binary-data-format)
    - [2.1. Framing](#21-framing)
    - [2.2. Optional XTEA Encryption](#22-optional-xtea-encryption)
    - [2.3. Endianness](#23-endianness)
    - [2.4. `TRICE` (Time)Stamps](#24-trice-timestamps)
    - [2.5. Binary Encoding](#25-binary-encoding)
      - [2.5.1. Symbols](#251-symbols)
      - [2.5.2. Package Format](#252-package-format)
  - [3. Trice Decoding](#3-trice-decoding)
    - [3.1. *Trice* ID list `til.json`](#31-trice-id-list-tiljson)
    - [3.2. *Trice* location information file `li.json`](#32-trice-location-information-file-lijson)
  - [4. *Trice* ID Numbers](#4-trice-id-numbers)
    - [4.1. ID number selection](#41-id-number-selection)
    - [4.2. ID number usage](#42-id-number-usage)
    - [4.3. ID number stability](#43-id-number-stability)
    - [4.4. *Trice* ID 0](#44-trice-id-0)
  - [5. Trice ID management](#5-trice-id-management)
    - [5.1. User Code Patching (`trice update`)](#51-user-code-patching-trice-update)
      - [5.1.1. User Code Patching Examples](#511-user-code-patching-examples)
    - [5.2. User Code Un-Patching](#52-user-code-un-patching)
    - [5.3. ID Usage Options](#53-id-usage-options)
    - [5.4. General ID Management Information](#54-general-id-management-information)
    - [5.5. Option 1: Patching the User Code once and let the inserted Irice ID b a part of the User Code](#55-option-1-patching-the-user-code-once-and-let-the-inserted-irice-id-b-a-part-of-the-user-code)
    - [5.6. Option 2: Patching the User Code in a pre-build process and Un-patching in a post-build process](#56-option-2-patching-the-user-code-in-a-pre-build-process-and-un-patching-in-a-post-build-process)
    - [5.7. Option 3: Patching the User Code on Repository Check-Out and un-patching on Check-In](#57-option-3-patching-the-user-code-on-repository-check-out-and-un-patching-on-check-in)
  - [6. Changelog](#6-changelog)
<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->
<div id="top"></div></ol></details>

# *Trice*  Version 1.0 Specification (Draft)

<!-- 🟢✅🟡⛔🔴🔵💧❓↩෴⚓🛑❗🌡⏱∑✳‼♦♣🚫⚠🎥📷🌊🆘🧷🐢➡☕ -->

##  1. <a name='TriceUserInterface-QuickStart'></a> Trice User Interface - Quick Start

###  1.1. <a name='UserCodeAdaption'></a>User Code Adaption

- Replace all strings `puts` with the string `trice`.
- Replace all strings `printf` with the string `trice`.
- Check for float and double format specifiers in the format strings. The appropriate parameters need to be covered with `aFloat()` or `a double()`. Example:

    ```c
    printf( "%d, %3.2f EUR, %g rate\n", i, price, change );
    ```

    ```c
    trice64( "%d, %3.2f EUR, %g rate\n", i, aFloat(price), aDouble(change) ); 
    ```

  - Because double needs 8 bytes the trice macro in this case needs to be trice64 (see [1.4. Trice Parameter Bit Widths](#14-trice-parameter-bit-widths)).

- Check for string format specifiers in the format strings. Put each in a separate trice message. Example:

    ```c
    printf( "name: %16s, surname: %32s, birthday: %4u-%02u-%02u\n", n, s, y, m, d);
    ```

    ```c
    trice( "name: %16s, ", n); trice( "surname: %32s, ", s ); trice( "birthday: %4u-%02u-%02u\n" y, m, d);
    ```

- Optionally add channel specifiers to get color. Example:

    ```c
    puts( "A message");
    ```

    ```c
   trice( "msg:A message");
    ```

- Add `#include trice.h` to all user files using trice.

###  1.2. <a name='Limitations'></a>Limitations

- The maximum parameter count per trice is 12.
- Each trice must fit into a single line.
  - Not ok:

    ```c
    trice( "hello %u\n", 
            year);
    ```

- But several trices ca be in one line.
  - Ok:

    ```c
    trice( "hello %u\n", year); trice( "good time");
    ```

- Strings directly as parameter are forbidden.
  - Not ok:

    ```c
    trice( "hello %s\n", "world" );
    ```

  - Ok:

    ```c
    s = "world"; trice( "hello %s\n", s );
    #define WORLD "world"
    trice( "hello %s\n", WORLD );
    ```

- Excluded trices are seen by the trice update process.
  - Example: The following code will be patched and get an id as well:

    ```c
    // trice( "Hi!" );
    ```

- All parameters inside one trice have the same bit width. If for example there are a single double and 10 bytes values, the needed trice macro is `trice64` providing 8 bytes space for all parameter values, therefore increasing the transmit overhead, but this can be handled by splitting into 2 trices:

  ```C
  // 92 bytes: 4 bytes header plus 11 times 8 bytes
  trice64( "%g: %c%c%c%c%c%c%c%c%c%c", aDouble(3.14159), 61, 62, 63, 64, 65, 66, 67, 68, 69, 10 );
  
  // 24 bytes: 4 bytes header plus 1 times 8 bytes plus 4 bytes header plus 8 times 1 byte
  trice64( "%g: ", aDouble(3.14159)); trice8( "%c%c%c%c%c%c%c%c%c%c", 61, 62, 63, 64, 65, 66, 67, 68, 69, 10 );
  ```

###  1.3. <a name='TriceTimeStamps'></a>Trice (Time) Stamps

- Trice messages can have no or 16-bit or 32-bit (time) stamps.
  - recommended:

      ```c
      trice( "hello %u\n", year); // no (time) stamp
      Trice( "hello %u\n", year); // 16-bit (time) stamp
      TRice( "hello %u\n", year); // 32-bit (time) stamp
      ```

  - legacy (inlining) syntax:

      ```c
      TRICE( id(0), "hello %u\n", year); // no (time) stamp
      TRICE( Id(0), "hello %u\n", year); // 16-bit (time) stamp
      TRICE( ID(0), "hello %u\n", year); // 32-bit (time) stamp
      ```

- The user is asked to provide the appropriate 2 functions.
  - Example for µs time stamps
  
    ```C
    TRICE_INLINE uint16_t modulo10000( x uint32_t ) {
        while( x - 10000 >= 0{
            x -= 10000;
        }
        return x;
    }
    ```
    
    ```c
    uint16_t TriceStamp16( void ){ // wraps after 10ms
        return modulo10000(ReadUs32());
    }

    uint32_t TriceStamp32( void ){
        return ReadUs32();
    }
    ```

  - Example for ms time stamps

    ```c
    uint16_t TriceStamp16( void ){ // wraps after 10s 
        return modulo10000(milliSecond);
    }

    uint32_t TriceStamp32( void ){
        return milliSecond;
    }
    ```

- Using different timestamp bit width parallel allows to reduce the transmitted data size.
- Example showing one trice without, 6 with 16-bit and 2 with 32-bit (time)stamps:

  ![x](./ref/0-16-32BitTimeStamps.jpg)

- Check the trice tool `tsu` CLI switch to make life easy.
- It is also possible to use the (time) stamp option not for timestamps but for any values, like addresses or a voltage.

###  1.4. <a name='TriceParameterBitWidths'></a>Trice Parameter Bit Widths

- The macros `trice`, `Trice`, `TRice` and `TRICE` use 32-bit parameter values per default. See `TRICE_DEFAULT_PARAMETER_BIT_WIDTH` inside `triceConfig.h` to change that.
- If for example the bit width of all trice parameters is 8-bit, it is writable as trice8 macro, reducing the transmitted byte count per parameter from 4 to 1:

  ```C
  char b[8] = {1,2,3,4,5,6,7,8};

  // 36 bytes: 4 bytes plus 32 (8 times 4) bytes payload 
  trice( "%02x %02x %02x %02x %02x %02x %02x %02x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);`

  // 12 bytes: 4 bytes plus 8 (8 times 1) bytes payload 
  trice8( " %02x %02x %02x %02x %02x %02x %02x %02x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);`

  // 16 bytes: 4 bytes plus 8 (8 times 1) bytes payload in short notation plus 4 bytes
  TRICE8_B( id( 6468), " %02x ", &b, sizeof(b) ); TRICE( id( 2822), "\n" );
  ```

<p align="right">(<a href="#top">back to top</a>)</p>

##  2. <a name='TriceBinaryDataFormat'></a>Trice Binary Data Format

###  2.1. <a name='Framing'></a>Framing

- *Trice* messages are framed binary data, if framing is not disabled.
- Framing is important for data disruption cases and is done with [TCOBS](./TCOBSSpecification.md) (has included data reduction) but the user can force to use [COBS](https://github.com/rokath/COBS), what makes it easier to write an own decoder in some cases od disable framing at all. 
  - Change the setting `TRICE_FRAMING` inside `triceConfig.h` and use the **trice** tool `-packageFraming` switch accordingly.
- For robustness each *Trice* gets its own (T)COBS package per default. That is changeable for transfer data reduction. Use `#define TRICE_TRANSFER_MODE TRICE_PACK_MULTI_MODE.` inside `triceConfig.h`. This allows to reduce the data size a bit by avoiding many 0-delimiter bytes but results in some more data loss in case of data disruptions.

###  2.2. <a name='OptionalXTEAEncryption'></a>Optional XTEA Encryption

- If XTEA is used, the encrypted packages have a multiple-of-8 byte length containing 1-7 padding bytes.
- The optional decryption is the next step after unpacking a data frame.
- Enabling XTEA automatically switches to COBS framing. There is no need to use the **trice** tool `-packageFraming` switch in that case.

###  2.3. <a name='Endianness'></a>Endianness

- To interpret a decoded package, it´s endianness needs to be known.
- For efficiency binary trice data are stored and transmitted in MCU endianness and the **trice** tool expects binary data in little endian format as most MCUs are little endian.
- On big endian MCUs the compiler switch `TRICE_MCU_IS_BIG_ENDIAN` needs to be defined and the **trice** tool has a CLI switch "triceEndianness" which needs to be set to "bigEndian" then.
- If trice transmit data are needed to be not in MCU order for some reason, the macro `TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN` is needed. This increases the critical trice storage time and target code amount.

###  2.4. <a name='TRICETimeStamps'></a>`TRICE` (Time)Stamps

- Each `TRICE` message can carry stamp bits, which are free usable like for time, addressing or filtering.
- By selecting the ID letter case `id(0)`, `Id(0)` or `ID(0)` the user can decide for each single `TRICE` macro about the stamp size.

  | notation                    | stamp size | remark |
  | -                           | -          | -      |
  | `TRICE( id(0), "...", ...)` | 0-bit      | no stamp at all, shortest footprint |
  | `TRICE( Id(0), "...", ...)` | 16-bit     | calls internally `uint16_t TriceStamp16( void )` for trice message stamping |
  | `TRICE( ID(0), "...", ...)` | 32-bit     | calls internally `uint32_t TriceStamp32( void )` for trice message stamping |

It is up to the user to provide the functions `TriceStamp16()` and/or `TriceStamp32()`. Normally they return a tick count.

###  2.5. <a name='BinaryEncoding'></a>Binary Encoding

####  2.5.1. <a name='Symbols'></a>Symbols

| Symbol  | Meaning |
| :-:     | - | 
| `i`     | ID bit |
| `I`     | `iiiiiiii` = ID byte |
| `n`     | number bit |
| `s`     | selector bit
| `N`     | `snnnnnnnn` = 7-bit number byte |
| `c`     | cycle counter bit |
| `C`     | s==0 ? `cccccccc` : `nnnnnnnn` = cycle counter byte or number byte extension |
| `t`     | (time)stamp bit |
| `T`     | `tttttttt` = (time)stamp byte |
| `d`     | data bit |
| `D`     | `dddddddd` = data byte |
| `...`   | 0 to 32767 data bytes |
| `"..."` | format string |
| `x`     | unspecified bit |
| `X`     | =`xxxxxxxx` unspecified byte |

####  2.5.2. <a name='PackageFormat'></a>Package Format

- All decoded frames of 0-, 1-, 2- and 3-byte size are considered as user data and ignored by the **trice** tool.
- In decoded frames >= 4-byte the first 2 bytes are the 14-bit ID with 2 selector bits at the most significant position.
- The `0` selector is usable for any user encoding. The **trice** tool ignores such packages.

  | 16-bit groups            | Selector (2 msb)| *Trice* code                 | Comment                              |
  | :-                       | :-:             | -                            | -                                    |
  | `00xxxxxxX ...`          | 0               |                              | reserved for extensions or user data |
  | `01iiiiiiI NC  ...`      | 1               | `TRICE( id(n), "...", ...);` | *Trice* format without     stamp     |
  | `10iiiiiiI TT NC ...`    | 2               | `TRICE( Id(n), "...", ...);` | *Trice* format with 16-bit stamp     |
  | `11iiiiiiI TT TT NC ...` | 3               | `TRICE( ID(n), "...", ...);` | *Trice* format with 32-bit stamp     |

<p align="right">(<a href="#top">back to top</a>)</p>

##  3. <a name='TriceDecoding'></a>Trice Decoding

The 14-bit IDs are used to display the log strings. These IDs are pointing in two reference files.

###  3.1. <a name='TriceIDlisttil.json'></a>*Trice* ID list `til.json`

- This file integrates all firmware variants and versions and is the key to display the message strings. With the latest version of this file all previous deployed firmware images are usable without the need to know the actual firmware version.
- The files `til.json.h` and `til.json.c` are generated to help writing an own trice decoder tool.

###  3.2. <a name='Tricelocationinformationfileli.json'></a>*Trice* location information file `li.json`

- If the generated `li.json` is available, the **trice** tool automatically displays file name and line number. But that is accurate only with the exact matching firmware version. That usually is the case right after compiling and of most interest at the developers table.
- The **trice** tool will silently not display location information, if the `li.json` file is not found. For in-field logging, the option `-showID string` could be used. This allows later an easy location of the relevant source code.
- An other option is to record the binary trice messages and to play them later with the **trice** tool using the correct `li.json`.

<p align="right">(<a href="#top">back to top</a>)</p>

##  4. <a name='TriceIDNumbers'></a>*Trice* ID Numbers
###  4.1. <a name='IDnumberselection'></a>ID number selection

- The default encoding TREX supports 14-bit IDs, so over 16000 IDs possible. Other encodings can work with other ID sizes.
- `trice( "Hi!\n");` ➡ `trice update` ➡ `trice( 12345, "Hi!\n");` 
- The **ID** `12345` is a number assigned to `TRICE( "Hi!\n");` in the above example.
  - It is a so far unused number, according to rules you can control:
    - The `-IDMethod` switch allows a selection method for new IDs.
      - Per default new IDs determined randomly to keep the chance low, that several developers grab the same ID.
      - Example: `trice update -IDMin 1000 -IDMethod upward` will choose the smallest free ID >= 1000.
        - This allows to use the ID space without wholes.
    - The `-IDMin` and `-IDMax` switches are usable to control the ID range, a new ID is selected from, making it possible to divide the ID space. Each developer can gets it region.
      - Example: `trice update -IDMin 6000 -IDMax 6999` will choose new randomly IDs only between 6000 and 6999.
- In a future **trice** tool it can be possible to give each *trice* channel an **ID** range making it possible to implement *Trice* channel specific runtime on/off on the target side if that is needed. This could be interesting for routing purposes also.
  - To stay compatible with previous **trice** tool versions such implementation would use the `-args` switch, which then contains the relevant channels like `trice u -args "err:20:99,wrn:200:300"`. This needs to be specified in more detail, especially the error handling.

###  4.2. <a name='IDnumberusage'></a>ID number usage

- If you write `TRICE( "Hi!\n");` again on a 2nd location, it gets a different **ID**.
- `TRICE8( "msg:%d", 1);` and `TRICE16( "msg:%d", 1);` are different *Trices* even the format strings identical.

###  4.3. <a name='IDnumberstability'></a>ID number stability

- IDs stay constant and get only changed to solve conflicts.
- To make sure, a single ID will not be changed, you could change it manually to a hexadecimal syntax.
  - This lets the `trice update` command ignore such `TRICE` macros and therefore a full [til.json](../til.json) rebuild will not add them anymore. Generally this should not be done, because this could cause future bugs.
  - It is possible to assign an ID manually as decimal number. It will be added to the ID list automatically during the next `trice u`.
- If a *Trice* was deleted inside the source tree (or file removal) the appropriate ID stays inside the ID list.
- If the same string appears again this ID is active again.
- If a trice occurs more than one time, each occurance gets a different ID. If then 2 of them disappear, their ID numbers stay in `til.json`. If then one of them comes back, it gets a new ID, or not?

How could that work?
- First read all source files and map

###  4.4. <a name='TriceID0'></a>*Trice* ID 0

- The trice ID 0 is a placeholder for "no ID", which is replaced automatically during the next `trice update` according to the used trice switches `-IDMethod`, `-IDMin` and `IDMax`.
  - It is sufficient to write the TRICE macros just without the `id(0),` `Id(0),` `ID(0),`. It will be inserted automatically according the `-stamp` switch.
- With `trice zeroSourceTreeIds` all IDs in the given source tree are set to 0. This gives the option afterwards to set-up a new `til.json` according to a different `-IDMethod`, `-IDMin` and `IDMax`.

##  5. <a name='TriceIDmanagement'></a>Trice ID management

###  5.1. <a name='UserCodePatchingtriceupdate'></a>User Code Patching (`trice update`)

- A *Trice* **ID** is inserted by `trice update` as shown in the table:

  | Unpatched User Code       | After `trice update`          | Remark                                               |
  |---------------------------|-------------------------------|------------------------------------------------------|
  | `trice( "Hi!\n");`        | `trice( 12345, "Hi!\n");`     | no stamps                                            |
  | `Trice( "Hi!\n");`        | `Trice( 12345, "Hi!\n");`     | 16-bit stamps                                        |
  | `TRice( "Hi!\n");`        | `TRice( 12345, "Hi!\n");`     | 32-bit stamps                                        |

- Legacy code is handled this way:

  | Unpatched User Code       | After `trice update`          | Remark                                               |
  |---------------------------|-------------------------------|------------------------------------------------------|
  | `TRICE( "Hi!\n");`        | `TRICE( id(12345), "Hi!\n");` | no stamps after `trice u -defaultStampSize 0`        |
  | `TRICE( "Hi!\n");`        | `TRICE( Id(12345), "Hi!\n");` | 16-bit stamps after `trice u -defaultStampSize 16`   |
  | `TRICE( "Hi!\n");`        | `TRICE( ID(12345), "Hi!\n");` | 32-bit stamps after `trice u -defaultStampSize 32`   |
  | `TRICE( id(0), "Hi!\n");` | `TRICE( id(12345), "Hi!\n");` | no stamps                                            |
  | `TRICE( Id(0), "Hi!\n");` | `TRICE( ID(12345), "Hi!\n");` | 16-bit stamps                                        |
  | `TRICE( ID(0), "Hi!\n");` | `TRICE( ID(12345), "Hi!\n");` | 32-bit stamps                                        |

- A pre-build step `trice update` generates the `Id(12345)` part. Examples:
  - `trice u` in your project root expects a til.json file there and checks sources and **til.json** for changes to update.
  - `trice u -v -i ../../../til.json -src ../src -src ../lib/src -src ./` is a typical case as automated pre-build step in your project settings telling **trice** to scan the project dir and two external directories. Even `trice u` is fast, it is generally quicker to search only relevant places.

####  5.1.1. <a name='UserCodePatchingExamples'></a>User Code Patching Examples

- A *Trice* **ID** is modified as shown in these cases:
  - Previously updated (patched) user code copied to a different location:

    ```C
    trice(12345, "Hi!\n"); // copied
    trice(12345, "Hi!\n"); // original
    trice(12345, "Hi!\n"); // copied
    ```

  - After updating (patching) again:

    ```C
    trice(12345, "Hi!\n");
    trice( 1233, "Hi!\n"); // re-patched
    trice( 1234, "Hi!\n"); // re-patched
    ```
  
    - If the code is copied inside the same file, the first occurrence after the copy stays unchanged and the following are modified.
    - If the code is copied to other files only, one first occurrence stays unchanged. In which file is not defined.
  - Previously updated (patched) user code copied and modified:

    ```C
    trice(12345, "Ha!\n"); // copied and modified
    trice(12345, "Hi!\n"); // original
    trice(12345, "Ha!\n"); // copied and modified
    ```

  - After updating (patching) again:

    ```C
    trice( 2333, "Ha!\n"); // re-patched
    trice(12345, "Hi!\n"); // unchanged
    trice( 1234, "Ha!\n"); // re-patched
    ```

  - If the code is copied to other files, it is re-patched.
- A *Trice* **ID** is stays the same if the stamp size is changed. Example:

  ```C
  trice( 12345, "Hi!" ); // original
  ```

  ```C
  TRice( 12345, "Hi!" ); // manually changed stamp size and then "trice u" performed.
  ```

###  5.2. <a name='UserCodeUn-Patching'></a>User Code Un-Patching

###  5.3. <a name='IDUsageOptions'></a>ID Usage Options

- Per default the `trice update` command chooses randomly a so far unused ID for new format strings.
- 
  
###  5.4. <a name='GeneralIDManagementInformation'></a>General ID Management Information

- The trice ID-instead-of-String idea lives from pre-compile patching of the user code.
- The user has full control how to deal with that.
- There are 3 options and the user has to decide which fits best for him.
- Each format string gets its unique trice ID. If the same format string is used on different source code locations it gets different trice IDs this way allowing a reliable location information.


###  5.5. <a name='Option1:PatchingtheUserCodeonceandlettheinsertedIriceIDbapartoftheUserCode'></a>Option 1: Patching the User Code once and let the inserted Irice ID b a part of the User Code



###  5.6. <a name='Option2:PatchingtheUserCodeinapre-buildprocessandUn-patchinginapost-buildprocess'></a>Option 2: Patching the User Code in a pre-build process and Un-patching in a post-build process

###  5.7. <a name='Option3:PatchingtheUserCodeonRepositoryCheck-Outandun-patchingonCheck-In'></a>Option 3: Patching the User Code on Repository Check-Out and un-patching on Check-In

<p align="right">(<a href="#top">back to top</a>)</p>

##  6. <a name='Changelog'></a>Changelog

<details><summary>Details</summary><ol>

| Date        | Version | Comment |
| -           | -       | - |
| 2022-MAR-15 |  0.0.0  | Initial Draft |
| 2022-MAR-15 |  0.1.0  | Minor corrections applied. |
| 2022-MAR-15 |  0.2.0  | Sigil byte encoding clarified. |
| 2022-MAR-15 |  0.3.0  | Forward versus backward COBS encoding discussion inserted. |
| 2022-MAR-15 |  0.4.0  | Forward versus backward COBS encoding reworked. Disruption detection added. |
| 2022-MAR-15 |  0.5.0  | Minor corrections |
| 2022-MAR-16 |  0.6.0  | TCOBS prime number comment added, simplified |
| 2022-MAR-17 |  0.7.0  | TCOBS move into a separate [TCOBS Specification](./TCOBSSpecification.md), Framing more detailed. |
| 2022-MAR-20 |  0.7.1  | Contributive *Trice* extension remark added. |
| 2022-APR-12 |  0.8.0  | TREX mainstream format changed to timestamps immediate after ID. |
| 2022-MAY-20 |  0.8.1  | Formatting, Spelling |
| 2022-JUN-19 |  0.9.0  | Implementation hint added to chapter Framing. |
| 2022-AUG-14 | 0.10.0  | Chapter ID Management added |
| 2022-AUG-19 | 0.11.0  | Chapter Main Stream Logs changed/extended |
| 2022-SEP-15 | 0.11.1  | TS32, TS16, NOTS, MOD7 added |
| 2022-OCT-08 | 0.11.2  | S0...X3 added |
| 2022-NOV-28 | 0.11.3  | +[#337](https://github.com/rokath/trice/issues/337) in [Framing](#Framing)|
| 2022-DEC-11 | 0.12.0  | restructured |
| 2022-DEC-13 | 0.13.0  | unneeded text removed, some clarifications |
| 2023-JAN-14 | 0.14.0  | Formatting improved, [1.  Trice User Interface - Quick Start](#1--trice-user-interface---quick-start) added. |
|             |         | |

<p align="right">(<a href="#top">back to top</a>)</p></ol></details>

 <!-- 
 
-##  3. <a name='TREXTRiceEXtendableencoding'></a>TREX (*TR*ice *EX*tendable) encoding

* The extendable encoding leaves options for user specific data mixable with trice data. This allows filtering of the framed data by just checking a bit pattern.

<p align="right">(<a href="#top">back to top</a>)</p>

-##  4. <a name='IDManagement'></a>ID Management

* The IDs inside the source code are a "dealbreaker" as [bora](https://community.memfault.com/u/bora) mentioned in his [comment](https://interrupt.memfault.com/blog/trice). In fact it is not acceptable for library code used in several projects. An improved approach could look like this:


```c
TRICE( id(0), "...", ...); // a trice without stamp
TRICE( Id(0), "...", ...); // a trice with a 16-bit stamp
TRICE( ID(0), "...", ...); // a trice with a 32-bit stamp
```
 For the implementation of the optional *Trice* extensions (see below), a `til.json` format extension is needed because several files are unhandy. Both `til.json` formats will be accepted in the future. -->
<!-- With [TREX](#TREXTriceextendableencoding) encoding the location information needs no transmission anymore but goes not into the `til.json` file. In the field the location information is normally useless and probably outdated. The software developer is the one, mostly interested in the location information.  When the firmware left the developer table, only the file `til.json` is of interest.--> 
 <!-- Also the planned `-binaryLogfile` option is possible. See [issue #267 Add `-binaryLogfile` option](https://github.com/rokath/trice/issues/267). It allows a replay of the logs and the developer can provide the right version of the `li.json` file.-->
<!--


* [x] New *Trice* macros are writable without the ID, so when `trice u` is executed, a CLI switch controls the ID type selection:
  * The update switch `-stamp 32` defaults new ID´s to `ID`.
  * The update switch `-stamp 16` defaults new ID´s to `Id`.
  * The update switch `-stamp 0`  defaults new ID´s to `id`.
  * The update switch `-stamp to32` converts all `id` and `Id` to `ID`.
  * The update switch `-stamp to16` converts all `id` and `ID` to `Id`.
  * The update switch `-stamp to0`  converts all `ID` and `Id` to `id`.
* [x] `trice log`:
  * The log switch `-ttsf` is the same as `-ttsf32`.
  * There is a new log switch `ttsf16` for the 16 bit timestamps.
  * The `trice` tool alignes *Trice* messages with different timestamp sizes.

* The **trice** tool supports several encodings, old projects should be usable with newer **trice** tool versions by applying the `-encoding TLE` switch.


* The [TREX (**TR**ice **EX**tendable) encoding](#TREXTriceextendableencoding) format is planned to be stable.
The with name "COBS" branded [*Trice* v0.48.0 encoding](./TriceMessagesEncoding.md) is not optimal concerning the generated data amount:
* See discussion [#253 Save trice COBS encoded data on target and view it later on PC](https://github.com/rokath/trice/discussions/253).
* The location information is transmitted as 16 bit file ID plus 16 bit line number. It is possible to generate during `trice update` an additional file `li.json` containing the location information for each *Trice* ID avoiding the additional 4 bytes this way. But this could cause assignment issues, when the same *Trice* ID is used at different locations (see [https://github.com/rokath/trice/discussions/264](https://github.com/rokath/trice/discussions/264)). But it is possible to drop the option `trice u -sharedIDs`.
* The 32-bit "COBS" package descriptor is overkill for allowing user data and dropped in TREX.
* The additional padding bytes to achieve 32 bit sizes are not needed and dropped in TREX. The user could add them by himself if really needed.
* The 4 timestamp bytes in front of each *Trice* demand the "COBS" package descriptor. The timestamp should go inside the *Trice* message and be optionally smaller. That is done in TREX.


<p align="right">(<a href="#top">back to top</a>)</p>

-##  2. <a name='Compatibility'></a>Compatibility

* There is no guaranty for compatibility with legacy target trice versions but the aim is to provide it.


* The *Trice* v0.48.0 user syntax will remain mainly unchanged. The letter case of the ID codes the target timestamp size. (see below)
* The as "COBS" branded legacy [v0.48.0 *Trice* encoding](.TriceMessageEncoding.md) will stay unchanged as an option for compatibility. But it will not be the default encoding anymore. To use newer **trice** tool versions with legacy projects the CLI switch `-encoding TLE` needs to be used.
* The option `-sharedIDs` will be further available but depreciated to avoid location assignment issues.
* Legacy projects which used the option `-sharedIDs` will still work even with a `li.json` file. A several times used ID will get an assignment of one of the locations.
* The issue [#242 Add target context option](https://github.com/rokath/trice/issues/242) could get the label "wontfix". When a task ID is needed, it could be also a data value in such cases.
* The same user source files usable with the legacy *Trice* "COBS" encoding and the proposed additional [TREX](#TREXTriceextendableencoding) encoding. They will have 16 bit stamps instead of 32-bits if you keep the sub macros`Id(n)`.
* Exchange individually to `ID(n)` to get 32-bit timestamps back.
* Exchange individually to `id(n)` to avoid timestamps.

* User data are in separate TCOBS packages encoded. When *Trices* are accumulated in a double half buffer, their separation in TCOBS packages is possible until the first extended *Trice*. Because of the generally unknown extended *Trice* length from this point, all following *Trices* in this half buffer need to go in one TCOBS package (including optional padding bytes) what is ok. The only disadvantage with this is, that in case of a data disruption at this place, several *Trice* messages can get lost.

Possible better implementation: See issue [#290](https://github.com/rokath/trice/issues/290)

COBS: See [#337](https://github.com/rokath/trice/issues/337)


```c
TRICE( S0, "...", ...); // a trice without stamp
TRICE( S2, "...", ...); // a trice with a 16-bit stamp
TRICE( S4, "...", ...); // a trice with a 32-bit stamp
TRICE( S8, "...", ...); // a trice with a 64-bit stamp
```

```c
TRICE( X0, "...", ...); // an extended type 0 trice 
TRICE( X1, "...", ...); // an extended type 1 trice 
TRICE( X2, "...", ...); // an extended type 2 trice 
TRICE( X3, "...", ...); // an extended type 3 trice 
```

* When editing, the user needs to write only `TRICE( "...", ...);` and the trice tool inserts a S0, S2, S4 or S8 automatically according to the used `-stamp` switch parameter.
* After repository check-out and before compiling, following substitutions are done using `trice -u`:
  * `TRICE( S0, "...", ...);` → `TRICE( id(0), "...", ...);` → `TRICE( id(12345), "...", ...);`
  * `TRICE( S2, "...", ...);` → `TRICE( Id(0), "...", ...);` → `TRICE( Id(12345), "...", ...);`
  * `TRICE( S4, "...", ...);` → `TRICE( ID(0), "...", ...);` → `TRICE( ID(12345), "...", ...);`
  * `TRICE( S8, "...", ...);` → `TRICE( iD(0), "...", ...);` → `TRICE( ID(12345), "...", ...);`
* After compiling and before repository check-in, following substitutions are done using `trice -z`:
  * `TRICE( id(12345), "...", ...);` → `TRICE( id(0), "...", ...);` → `TRICE( S0, "...", ...);`
  * `TRICE( Id(12345), "...", ...);` → `TRICE( Id(0), "...", ...);` → `TRICE( S2, "...", ...);`
  * `TRICE( ID(12345), "...", ...);` → `TRICE( ID(0), "...", ...);` → `TRICE( S4, "...", ...);`
  * `TRICE( iD(12345), "...", ...);` → `TRICE( iD(0), "...", ...);` → `TRICE( S8, "...", ...);`
* The project specific `til.json` contains all IDs and during `trice u` the same IDs are used again for the same **trice** statement. For new or modified **trices** new IDs a chosen and `til.json` is extended as usual.
* Identical **trices** should have different IDs for the correctness of the location information. The switch `-sharedIDs` is obsolete and depreciated.
* There is no guaranty each **trice** gets its old ID back, if for example 5 identical **trices** with different IDs exist, but the probability for an exact restore can made high using the previous `li.json` file. Proposed method:
  * When `trice -u` is executed, the previous `li.json` is read into an internal `li_1.map` and `li.json` is reset to be an empty file and that is red into `li.map`.
  * The `til.json` is read into a `lu` as already done, but the reversal `tflu` list format gets an ID slice assigned to each *trice*.
  * **Trices** occurring only once, what are probably the most, contain an ID slice of length 1.
  * If a trice occurs for example 5 times its ID slice has length 5 containing 5 different IDs.
  * When the `trice -u` command finds a **trice** with ID slice length > 1, it looks into `li_1.map` for all possible IDs and compares the location information with the actual location:
    * If no matching file name is found, a new ID is generated.
    * If file name is identical, the ID with the minimum difference to the line number is chosen if not already used.
    * Because all assigned IDs go into the `li.map` this is possible to check.
    * If all IDs in the slice with identical file name are used, a new ID is generated.
    * Of course there are cases possible, where some unwanted ID "shift" happens. But we have to consider, that first we are talking about rare identical **trices** and that such case, if, only happens once with the result, that the `til.json` file adds a bit data garbage. A `til.json` cleaning is always possible, but you loose history then.

####  6.1. <a name='Triceformat'></a>*Trice* format

* Parameter data bytes start after the optional timestamp.
* N is the parameter data bytes count. Padding bytes are not counted.
* Usually N is < 127 but for buffer or string transfer N can get up to 32767 (15 bits).
* When N > 127 (s==1) `NC` is replaced by `1nnnnnnn nnnnnnnn`. C is incremented with each *Trice* but not transmitted when:
  * N > 127
  * extended *Trice* without C

-####  3.2. <a name='FramingwithTCOBSencoding'></a>Framing with TCOBS encoding

* For maximum storage speed each **trice** message starts at a 32-bit boundary and has 1-3 padding bytes.
* In direct mode only a single message needs handling.
* In deferred mode after half buffer swap any count of trice messages is in the buffer.
* There are different policies possible:
  1. **TRICE_FAST_MULTI_MODE**: Compact buffer by removing all padding bytes, encode it as a single package, append one 0-delimiter and transmit. This allows to reduce the transmitted data amount by paying the price of possibly more data lost in case of an error. Also the data interpretation can perform less checks.
  2. **TRICE_SAFE_SINGLE_MODE**: Encode each buffer separate, append a 0-delimiter for each and pack them all together before transmitting. This increases the transmit data slightly but minimizes the amount of lost data in case of a data disruption.

<!-- ###  6.3. <a name='ExtendedTricesasfutureoption'></a>Extended *Trices* as future option

If for special cases, the main stream encoding is not sufficient, the user can add its own encoding.

See issue #313 for an other option. The user can write `TRICE16( 7, "%d,%d,%d", x, y, z);` For example 
the iD7 macro Packs the 3 16-bit values x, y, z as packed 12-bit, 12-bit, 8-bit into a 32-bit payload.
Additionally only 2 IDs (1 bit) are needed without cycle and count:
`00sssii dddddddd dddddddd dddddddd dddddddd`
* `00...` sub-options `TRICEX0`, `TRICEX1`, `TRICEX2`, `TRICEX3`
  * `-ex0 pos -ex1 pos -ex2 pos -ex3 pos`  Select position in extendable table for TRICEXn, 4 coding's selectable in one shot.
  * The table is creatable and extendable on demand.
  * For each line an appropriate target and host code needs to be done.
  * Then the target configuration must match the CLI switches.
  * Table example:
    |Position | Encoding                          | Remarks                                       |
    | -   | -                                     | -                                             |
    | pos | `00nniiii I D D`                      | 12 bit ID, no timestamp, 1x 16 bit data       |
    | pos | `00nniiii I D D`                      | 12 bit ID, no timestamp, 2x 8 bit data        |
    | pos | `00nniiii I D D D D`                  | 12 bit ID, no timestamp, 2x 16 bit data       |
    | pos | `00nniiii I T T  D D`                 | 12 bit ID, 16 bit timestamp,1x16 bit data     |
    | pos | `00nniiii I T T  D D D D`             | 12 bit ID, 16 bit timestamp, 1x32 bit data    |
    | pos | `00nniidd dddddddd`                   | 2 bit ID & 1x 10 bit data                     |
    |   6 | `00nndddd dddddddd`                   | no ID, 12 bit data as a 5 and a 7 bit value   |
    | pos | `00nndddd`                            | no ID, 4x 1 bit data                          |
    | pos | `00nniiii I`                          | 12 bit ID, no data                            |
    | pos | `00nniiii D D`                        | 4 bit ID, 1x 16 bit data                      |
    | pos | `00nniiii I D D`                      | 12 bit ID, 2x 8 bit data                      |
    | pos | `00nniiii I T T`                      | 12 bit ID, 16 bit timestamp, no data          |
    | pos | `00nniiii D D T`                      | ...                                           |
    |  13 | `00nniiii C T T  D D D D`             | 8 bit cyle counter, 16 bit timestamp, a float |
    | pos | `00nniiii C D D  T T T T`             | ...                                           |
    | pos | `00nniiii I D D  T`                   | ...                                           |
    | pos | `00nniiii tttttttt ttttdddd dddddddd` | 4 bit ID, 12 bit timestamp, 12 bit data       |
    | ... | `00nn...`                             | ...                                           |
  * Examples:
    * `-ex0 13` means TRICEX0 = `0000iiii C T T  D D D D`.
      * Usage: `TRICEX0( "result %f\n", aFloat(v));`
    * `-ex2 6`  means TRICEX2 = `0010dddd dddddddd`.
      * Usage: `TRICEX2( "point %x,%d\n", a, b);`
  * *Trice* extensions without cycle counter are counted as well.
  * Each TRICEXn has its own ID space.
* Several different *Trice* extensions are contributive.

- ###  6.4. <a name='Unknownuserdata'></a>Unknown user data

* Unknown user data are possible as part of the *Trice* extensions.
  * Without the `-ex0` switch, `0000...` packages are ignored as unknown user data.
  * Without the `-ex1` switch, `0001...` packages are ignored as unknown user data.
  * Without the `-ex2` switch, `0010...` packages are ignored as unknown user data.
  * Without the `-ex3` switch, `0011...` packages are ignored as unknown user data.
* So, if *Trice* extensions not used, all `00...` packages are ignored as unknown user data.
* Unknown user data have an unknown length. Therefore they cannot share a COBS packet with *Trices*.
* Unknown user data packets do not affect the cycle counter. The can have their own cycle counter.
-->
