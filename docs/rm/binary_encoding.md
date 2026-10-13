# Binary Encoding

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Symbols](#symbols)
* [2. Package Format](#package-format)
  * [2.1. typeX0 Records](#typex0-records)
  * [2.2. Framing - NONE or with COBS or TCOBS encoding](#framing---none-or-with-cobs-or-tcobs-encoding)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="symbols"></a>Symbols

| Symbol  | Meaning                                                                      |
|:-------:|------------------------------------------------------------------------------|
|   `i`   | ID bit                                                                       |
|   `I`   | `iiiiiiii` = ID byte                                                         |
|   `n`   | number bit                                                                   |
|   `z`   | count selector bit                                                           |
|   `s`   | stamp selector bit                                                           |
|   `N`   | `znnnnnnnn` = count selector bit plus 7-bit number byte                      |
|   `c`   | cycle counter bit                                                            |
|   `C`   | z==0 ? `cccccccc` : `nnnnnnnn` = cycle counter byte or number byte extension |
|   `t`   | (time)stamp bit                                                              |
|   `T`   | `tttttttt` = (time)stamp byte                                                |
|   `d`   | data bit                                                                     |
|   `D`   | `dddddddd` = data byte                                                       |
|  `...`  | 0 to 32767 data bytes                                                        |
| `"..."` | format string                                                                |
|   `W`   | bit width 8, 16, 32 or 64 (uW stands for u8, u16, or u64)                    |
|   `x`   | unspecified bit                                                              |
|   `X`   | =`xxxxxxxx` unspecified byte                                                 |

The long count field can represent up to 32767 payload bytes. This is the binary format limit, not the amount that the current target buffer macros can write. `TRICE_N`, `TRICE_S`, and the buffer macros built on `TRICE_N` cap the payload at `TRICE_SINGLE_MAX_SIZE - 12` bytes. `TRICE_SINGLE_MAX_SIZE` is the reserved size of one aligned target-side record and cannot exceed 32764 bytes; therefore the largest payload through these macros is 32752 bytes. The limit for a runtime string counts its characters, excluding the terminating zero byte.

The 12-byte allowance covers up to 8 bytes for the ID, a 32-bit timestamp, and the count field, plus one 4-byte word for alignment and output scratch space. The target rounds the stored payload up to a 32-bit boundary; some output paths also need space after the record, for example when padding for XTEA encryption. At the macro's maximum payload size, the aligned record uses at most `TRICE_SINGLE_MAX_SIZE - 4` bytes, leaving that word inside the reserved size. Package framing can add further bytes on the transport.

## 2. <a id="package-format"></a>Package Format

* Because of **TCOBS** or **COBS** package framing, package sizes are detectable by the Trice tool without additional length information from the payload itself.
* A decoded frame of 0 bytes is ignored. A 1-byte frame is unsupported. Frames with 2 or 3 bytes are selector-0 candidates when their selector bits are `00`; valid counted `typeX0` handling then still depends on the embedded count. Otherwise they are unsupported short user data.

  | bytes       | Comment                                                                                                       |
  |:------------|---------------------------------------------------------------------------------------------------------------|
  | ` `         | This is an empty package, which can have also a meaning. It is detectable by 2 consecutive 0-delimiter bytes. |
  | `X`         | 1-byte message, unsupported and reserved for extensions or user data                                          |
  | `X` `X`     | 2-byte message, counted `typeX0` if selector `00`, otherwise unsupported and reserved                         |
  | `X` `X` `X` | 3-byte message, counted `typeX0` if selector `00`, otherwise unsupported and reserved                         |

* In decoded frames with at least 2 bytes, the first 2 bytes contain 2 selector bits at the most significant position in the known endianness.
* The `0` selector is usable for any user encoding. The Trice tool handles such packages according to the CLI switch `-typeX0`.
* The `1`, `2` and `3` selector bits are followed by the 14-bit ID.

  | 16-bit groups                      | Selector (2 msb) | Comment                                                                                   | Endianness sizes                |
  |:-----------------------------------|:----------------:|-------------------------------------------------------------------------------------------|:--------------------------------|
  | _________ `00xxxxxxX ...`          |        0         | [typeX0 record](#typex0-records), >= 2-byte message, reserved for extensions or user data | ___ `u16 ?...?`                 |
  | _________ `01iiiiiiI NC  ...`      |        1         | >= 4-byte message, Trice format without     stamp                                         | ___ `u16 u16 [uW] ... [uW]`     |
  | _________ `10iiiiiiI TT NC ...`    |        2         | >= 4-byte message, Trice format with 16-bit stamp                                         | ___ `u16 u16 u16 [uW] ... [uW]` |
  | `10iiiiiiI 10iiiiiiI TT NC ...`    |        2         | First 16bit are doubled. Info over `-d16` trice switch.                                   | `u16 u16 u16 u16 [uW] ... [uW]` |
  | _________ `11iiiiiiI TT TT NC ...` |        3         | >= 4-byte message, Trice format with 32-bit stamp                                         | ___ `u16 u32 u16 [uW] ... [uW]` |

* The selector `2` encoding has 2 possibilities. When using `TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE` or encryption, for alignment reasons the first 16bit ID field is doubled. The Trice tool discards these 2 doubled bytes when the CLI switch `-d16` is given or encryption is active.
* Default endianness is little endian as most MCUs use little endianness. Otherwise the `-triceEndianness=bigEndian` CLI switch is needed.
* The receiving tool first evaluates the 2 selector bits and follows these rules:
  * 0: handle it according to `-typeX0` or report an error and ignore the whole package when no X0 handling is selected.
  * 1:                                    next 14 bits are the ID                                                              followed by 2 bytes u16=NC and optional parameter values. Package size is >= 4 bytes.
  * 2 and `-d16` CLI switch not provided: next 14 bits are the ID                            and convert then u16=TT=stamp16   followed by 2 bytes u16=NC and optional parameter values. Package size is >= 6 bytes.
  * 2 and `-d16` CLI switch     provided: next 14 bits are the ID, discard 2 following bytes and convert then u16=TT=stamp16   followed by 2 bytes u16=NC and optional parameter values. Package size is >= 8 bytes.
  * 3:                                    next 14 bits are the ID                            and convert then u32=TTTT=stamp32 followed by 2 bytes u16=NC and optional parameter values. Package size is >= 8 bytes.
* Use the ID to get parameter width `W`=8,16,32,64 and parameter count from file *til.json*, then convert the payload accordingly.
  * Within one trice message the parameter bit width `W` does not change.

> Example for Trices without timestamps

* The ([T]COBS decoded) binary Trice data normally starts with a little endian u16 Trice ID value.
* All following values are encoded in the known endianness.

| value   | byte offset | type              | comment                                                                                                                                                                                                                                                                                                                        |
|---------|------------:|-------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| IdLo    |           0 | byte              | The first byte is always the Trice ID lower 8 bits.                                                                                                                                                                                                                                                                            |
| IdHi    |           1 | byte              | The second byte 2 most significant bits are `01` and the 6 least significant bits are the Trice ID upper 6 bits.                                                                                                                                                                                                               |
| NC      |           2 | u16               | The most significant bit is the count selector bit `z` and usually **0**, telling, that the following 7 bits are the payload byte count and that the 8 least significant bits are the cycle counter. If `z` is **1**, the current Trice contains no cycle counter and has a 15-bit payload count instead (for payloads > 127). |
| payload |           4 | u8\|u16\|u32\|u64 | The payload contains a number of equal size values.                                                                                                                                                                                                                                                                            |

### 2.1. <a id="typex0-records"></a>typeX0 Records

The user can insert any data with a well-defined structure into the Trice data stream. When interpreting the Trice binary data, the Trice tool handles selector-0/typeX0 records according to the CLI switch `-typeX0`.

One possible use case is to have user **printi** statements parallel to Trices (see [Legacy User Code Option Print Buffer Wrapping and Framing](trice_and_legacy_user_code.md#legacy-user-code-option-print-buffer-wrapping-and-framing)). For the counted typeX0 variant, the user prepends a generated **printi** buffer with its payload size as a 16-bit count smaller than `16384`. See [./_test/userprint_dblB_de_tcobs_ua/TargetActivity.c](../../_test/userprint_dblB_de_tcobs_ua/TargetActivity.c) for an implementation option. See chapter [20. typeX0 User Packets](typex0_user_packets.md) for further details.

### 2.2. <a id="framing---none-or-with-cobs-or-tcobs-encoding"></a>Framing - NONE or with COBS or TCOBS encoding

> Summary Information for Trice Data Parsing

* With COBS or TCOBS framing (TCOBS includes compression):
  * 1-3 package delimiter zeroes possible between 2 packages.
  * One or more Trices packed together and only at the package end are 0-3(7) padding zero bytes possible.
  * Counted `typeX0` records can occur together with normal Trices in one package because their length is checkable. X0 formats without length information need their own package framing.

* With NONE framing:
  * With XTEA encryption and `-pf=none` or `-pf=none64` 64-bit alignment: 0-7 zero bytes after a single Trice.
  * Without encryption the stream is compact or 32-bit aligned. That does not change for one session and is detectable.
    * `-pf=none` -> detect stream (deprecated, only for backward compatibility)
    * `-pf=none8` -> stream is compact
    * `-pf=none32` -> stream is 32-bit aligned
  * A stream with alignment is allowed to have only a single Trice between two alignments. An alignment is just a multiple of 4(8)-bytes distance.
  * These combinations are forbidden, because we cannot safely know the actual padding count: `|TriceATriceB0|TriceC00|` <- Is the `0` after `TriceB` a padding zero or part of `TriceC`?
    * Framing NONE && TRICE_MULTI_PACK_MODE && 32-bit write
    * Framing NONE && TRICE_MULTI_PACK_MODE && XTEA encryption

> Details

* For maximum storage speed each Trice message starts at a 32-bit boundary and has 1-3 padding bytes inside the target device RAM.
* The macro `TRICE_LEAVE` and/or function `TriceTransfer` ([Trice Target Code Implementation](trice_target_code_implementation.md)) are the Trice data output.
  * In **direct mode** each single message gets its own transfer buffer.
  * In **deferred mode** any count of Trice messages is in the transfer buffer.
  * Additional counted **typeX0 records** can share a transfer buffer with normal Trices. Non-counted selector-0 user data needs a separate transfer buffer because its length is not checkable.

<!--
* To create the transfer buffer, there are different policies possible:
  1. **TRICE_MULTI_PACK_MODE**: Compact RAM buffer by removing all padding bytes, encode it as a single (T)COBS package, append one 0-delimiter and transmit. This allows to reduce the transmitted data amount by paying the price of possibly more data loss in case of an error. Also the Trice tool internal data interpretation can perform less checks. TRICE_MULTI_PACK_MODE makes only sense for deferred output.
     * When Trice is used without framing, the data stream interpretation is possible, but not 100% secure for 32-bit writes, because in some cases we do not know, if 1-3 zeroes after a Trice, part of those are padding bytes or not. We allow such configuration, but will issue a warning: *Combination TRICE_MULTI_PACK_MODE && framing NONE && 32-bit write is depreachiated.*
  2. **TRICE_SINGLE_PACK_MODE**: Encode each Trice separate as (T)COBS, append a 0-delimiter for each and pack them all together before transmitting. This increases the transmit data slightly but minimizes the amount of lost data in case of a data disruption.
     * When Trice is used without framing, the data stream interpretation is possible, but not 100% secure for 32-bit writes, because in some cases we do not know, if 1-3 zeroes after a Trice, part of those are padding bytes or not. To avoid that specify `-pf=none32`. We allow such configuration, because even with `pf=none` a one-time detection will work well in almost every case.
  3. Additional **typeX0 records**: Those messages are not mixed into (T)COBS packages. They get their own (T)COBS packages.
-->

*Framing NONE Overview Table:*

| *mode* | *packed* | `-pf=`   | encr | *wr* | *use* | pad | stream  | remark   |
|--------|----------|----------|------|------|-------|-----|---------|----------|
| *di*   | *single* | `none32` | NONE | *32* | *32*  | 0-3 | aligned | done     |
| *de*   | *single* | `none8`  | NONE | *8*  | *8*   | 0   | compact | done     |
| *de*   | *single* | `none32` | NONE | *8*  | *32*  | 0   | aligned | **plan** |
| *de*   | *multi*  | `none8`  | NONE | *8*  | *8*   | 0   | compact | done     |
| *de*   | *multi*  | `none32` | NONE | *8*  | *32*  | 0   | unknown | forbid   |
| *de*   | *multi*  | `none`   | NONE | *8*  | *32*  | 0   | unknown | forbid   |
| *di*   | *single* | `none64` | XTEA | *32* | *32*  | 0-7 | aligned | done     |
| *de*   | *single* | `none64` | XTEA | *8*  | *8*   | 0-7 | aligned | done     |
| *de*   | *single* | `none64` | XTEA | *8*  | *32*  | 0-7 | aligned | **plan** |
| *de*   | *multi*  | `none`   | XTEA | *8*  | *8*   | 0-7 | unknown | forbid   |
| *de*   | *multi*  | `none`   | XTEA | *8*  | *32*  | 0-7 | unknown | forbid   |

* wr: The internal write function bit width.
* use: The possible user write function bit width (auxiliary write)



<p align="right">(<a href="#binary-encoding">back to top</a>)</p>

