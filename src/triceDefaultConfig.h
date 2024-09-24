/*! \file triceDefaultConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_DEFAULT_CONFIG_H_
#define TRICE_DEFAULT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRICE_CLEAN
// TRICE_CLEAN, when found in triceConfig.h is set to 0 with command "trice insert".
// and set to 1 with command "trice clean" to get rid of
// potential editor warnings in the trice clean state.
#define TRICE_CLEAN 0
#endif

#ifndef TRICE_OFF
// TRICE_OFF, when defined to 1 before including
// trice.h disables trice code for the file.
// When defined in the project settings, it influences
// the whole trice code generation.
#define TRICE_OFF 0
#endif

#ifndef TRICE_UARTA_MIN_ID
#define TRICE_UARTA_MIN_ID 0 //!< TRICE_UARTA_MIN_ID, if > 0, is the smalles ID routed to UARTA.
#endif

#ifndef TRICE_UARTA_MAX_ID
#define TRICE_UARTA_MAX_ID 0 //!< TRICE_UARTA_MAX_ID, if > 0, is the largest ID routed to UARTA.
#endif

#ifndef TRICE_UARTB_MIN_ID
#define TRICE_UARTB_MIN_ID 0 //!< TRICE_UARTB_MIN_ID, if > 0, is the smalles ID routed to UARTB.
#endif

#ifndef TRICE_UARTB_MAX_ID
#define TRICE_UARTB_MAX_ID 0 //!< TRICE_UARTB_MAX_ID, if > 0, is the largest ID routed to UARTA.
#endif

#ifndef TRICE_DEFERRED_UARTA
#define TRICE_DEFERRED_UARTA 0 //!< TRICE_DEFERRED_UARTA enables a user defined deferred trice write over TRICE_UARTA.
#endif

#ifndef TRICE_DEFERRED_UARTB
#define TRICE_DEFERRED_UARTB 0 //!< TRICE_DEFERRED_UARTB enables a user defined deferred trice write over TRICE_UARTB.
#endif

#ifndef TRICE_DIRECT_AUXILIARY8
#define TRICE_DIRECT_AUXILIARY8 0 //!< TRICE_DIRECT_AUXILIARY8 enables a user defined direct trice write.
#endif

#ifndef TRICE_DEFERRED_AUXILIARY8
#define TRICE_DEFERRED_AUXILIARY8 0 //!< TRICE_DEFERRED_AUXILIARY8 enables a user defined deferred trice write.
#endif

#ifndef TRICE_DIRECT_AUXILIARY32
#define TRICE_DIRECT_AUXILIARY32 0 //!< TRICE_DIRECT_AUXILIARY32 enables a user defineddirect trice write.
#endif

#ifndef TRICE_DEFERRED_AUXILIARY32
#define TRICE_DEFERRED_AUXILIARY32 0 //!< TRICE_DEFERRED_AUXILIARY32 enables a user defined deferred trice write.
#endif

#ifndef TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING
//! TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING allows to send an ID range of trices directly to an output.
//! The called output function usually is executed inside an interrupt and should therefore be non-blocking and fast.
#define TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING 0
#endif

#ifndef TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING
//! TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING allows to send an ID range of trices directly to an output.
//! The called output function should be non-blocking.
#define TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING 0
#endif

#ifndef TRICE_B
//! TRICE_B is a shortcut for TRICE8_B, TRICE16_B, TRICE32_B or TRICE64_B usable in your project.
#define TRICE_B TRICE8_B
#endif

#ifndef TRICE_F
//! TRICE_F is a shortcut for TRICE8_F, TRICE16_F, TRICE32_F or TRICE64_F usable in your project.
#define TRICE_F TRICE8_F
#endif

#ifndef TRICE_COMMAND_SIZE_MAX
//! TRICE_COMMAND_SIZE_MAX is the length limit for command strings to target.
#define TRICE_COMMAND_SIZE_MAX 8
#endif

#ifndef TRICE_DEFAULT_PARAMETER_BIT_WIDTH
//! TRICE_DEFAULT_PARAMETER_BIT_WIDTH is the default parameter bit width for TRICE macros not specifying the parameter bit width: 8, 16, 32 or 64.
//! If for example the majority of your values is 16 bit, it makes sense to set this value to 16 to use TRICE for them and to use TRICE32 explicitely for 32-bit values.
//! The trice tool CLI switch "-defaultTRICEBitwidth" needs to be set to the same bit width, default is 32.
#define TRICE_DEFAULT_PARAMETER_BIT_WIDTH 32
#endif

#ifndef TRICE_CONFIG_WARNINGS
#define TRICE_CONFIG_WARNINGS 1 //!< TRICE_CONFIG_WARNINGS == 0 can suppress some unwanted configuration warnings. Set to 0 only if you know what you are doing.
#endif

#ifndef TriceStamp16
//! TriceStamp16 returns a 16-bit value to stamp `Id` TRICE and `Trice` macros. Usually it is a timestamp, but could also be a destination address or a counter for example.
//! The user has to provide this value. Defining a macro here, instead if providing a fuction has significant speed impact.
#define TriceStamp16 0x1616
#endif

#ifndef TriceStamp32
//! TriceStamp32 returns a 32-bit value to stamp `ID` TRICE and `TRice` macros. Usually it is a timestamp, but could also be a destination address or a counter for example.
//! The user has to provide this function. Defining a macro here, instead a function if providing has significant speed impact.
#define TriceStamp32 0x32323232
#endif

#ifndef TRICE_PROTECT
//! The TRICE_PROTECT switch is only relevant for the deferred trice modes TRICE_DOUBLE_BUFFER and TRICE_RING_BUFFER.
//! The trice library works well, when less data are produced in the average than transmittable and when in the double buffer case the TriceTransfer
//! function is called, before too much data in a half buffer according to a good configuration. If that is guarantied you do not need to enable TRICE_PROTECT.
//! If because of an potential error this is not guarantied, you should enable TRICE_PROTECT. This slows down the TRICE macros a bit, but makes buffer overflows impossible.
//! A ring buffer cannot overflow in a first assumption, because old, not yet transmitted, trices are overwritten by newer ones.
//! But that can happen only to parts of trices. The ring buffer read out function relies on consistent data. If it gets data garbage, wrong values
//! for the trice lengths are possible and buffer overruns not avoidable. When enabling TRICE_PROTECT, new trices are only written into the deferred buffer,
//! if there is enough space. That guaranties data consistency. Because a suppressed trice cannot cause a cycle error, there is TriceOverflowCount as diagnostic value.
//! Long story short: If you sure, not to produce too much data, than set TRICE_PROTECT to 0 and get a bit more speed.
#define TRICE_PROTECT 1
#endif

#ifndef TRICE_BUFFER
//!  TRICE_BUFFER selects, where the TRICE macros accumulate the trice data during a single TRICE execution. Selectable options:
//! - TRICE_STACK_BUFFER: No additional buffer is needed, what makes sense for single task systems with direct output only.
//! - TRICE_STATIC_BUFFER: A single trice is stored in a separate static buffer, what makes sense for multi-tasking systems with direct output only.
//! - TRICE_DOUBLE_BUFFER: TRICE macros write direct into a double buffer without any additional management action.
//!   This is the fastest execution option for TRICE macros but needs more RAM. Used for deferred output and optional additional direct output.
//! - TRICE_RING_BUFFER: TRICE macros write direct into a ring buffer without any additional management action.
//!   This is a fast but not the fastest execution option for TRICE macros and needs less RAM. Used for deferred output and optional additional direct output.
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#endif

#ifndef TRICE_DEFERRED_TRANSFER_MODE
//! TRICE_DEFERRED_TRANSFER_MODE is the selected deferred trice transfer method. Options:
//! - TRICE_SINGLE_PACK_MODE packs each Trice message separately and adds a 0-delimiter byte. This reduces the transmit byte count. In case of a lost package only one Trice can get lost.
//! - TRICE_MULTI_PACK_MODE packs several trice messages before adding a 0-delimiter byte. This reduces the transmit byte count. In case of a lost package several Trices can get lost.
//! - When using encryption, the TRICE_MULTI_PACK_MODE can significantly reduce the transmit byte count, because in TRICE_SINGLE_PACK_MODE each Trice message gets extended
//! with 1 to 7 padding bytes before encryption. TRICE_MULTI_PACK_MODE is implemented for TRICE_DOUBLE_BUFFER but not yet for TRICE_RING_BUFFER and not possible for TRICE_STACK_BUFFER and TRICE_STATIC_BUFFER.
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#endif

#ifndef TRICE_RING_BUFFER_OVERFLOW_WATCH
//! TRICE_RING_BUFFER_OVERFLOW_WATCH is used to investigate, if the ring buffer margins are really not touched.
//! This value usually can stay 0.
#define TRICE_RING_BUFFER_OVERFLOW_WATCH 0
#endif

#ifndef TRICE_DIRECT_OUTPUT
//! TRICE_DIRECT_OUTPUT == 0: only deferred output, usually UART output only
//! TRICE_DIRECT_OUTPUT == 1: with direct output, SEGGER_RTT output and/or TRICE_DIRECT_AUXILIARY8 output
//! Setting TRICE_BUFFER to TRICE_STACK_BUFFER or TRICE_STATIC_BUFFER demands TRICE_DIRECT_OUTPUT == 1, no deferred output at all.
//! When TRICE_BUFFER == TRICE_RING_BUFFER or TRICE_BUFFER == TRICE_DOUBLE_BUFFER for deferred output, additional direct output can be switched on here.
//! For example it is possible to have direct 32-bit wise RTT TRICE_FRAMING_NONE output and deferred UART TRICE_FRAMING_TCOBS output.
#define TRICE_DIRECT_OUTPUT 0
#endif

#ifndef TRICE_DEFERRED_OUTPUT
//! TRICE_DEFERRED_OUTPUT == 0: no deferred output
//! TRICE_DEFERRED_OUTPUT == 1: with deferred output, usually UART output and/or auxiliary output
//! Setting TRICE_BUFFER to TRICE_RING_BUFFER or TRICE_DOUBLE_BUFFER demands TRICE_DEFERRED_OUTPUT == 1.
//! TRICE_BUFFER == TRICE_STACK_BUFFER or TRICE_BUFFER == TRICE_STATIC_BUFFER needs TRICE_DEFERRED_OUTPUT == 0.
//! When TRICE_BUFFER == TRICE_RING_BUFFER or TRICE_BUFFER == TRICE_DOUBLE_BUFFER for deferred output, additional direct output can be on.
#define TRICE_DEFERRED_OUTPUT 0
#endif

#ifndef TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING
//! TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1 makes only sense, when TRICE_DIRECT_OUTPUT is 1.
//! Enable this only, if you want only a specific ID range for direct Trice output.
//! If you use different direct output channels, you can change this only for all together.
#define TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING 0 // (experimental)
#endif

#ifndef TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING
//! TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING == 1 makes only sense, when TRICE_DEFERRED_OUTPUT is 1.
//! Enable this only, if you want only a specific ID range for direct Trice output.
//! If you use different direct output channels, you can change this only for all together.
#define TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING 0 // (experimental)
#endif

#ifndef TRICE_DATA_OFFSET
//! TRICE_DATA_OFFSET is the space in front of single trice data for in-buffer (T)COBS encoding.
//! - When using real big buffers, 16 may be not enough.
//! - When having only short trices but lots of trice bursts, it may make sense to reduce this value to 4.
//! - Without encoding/framing this value can be 0.
//! - When using XTEA, this value should incorporate additional 4 bytes, because of the 64-bit encryption units.
//! With TRICE_BUFFER == TRICE_RING_BUFFER, this amount of space is allocated in front of the ring buffer.
//! With TRICE_BUFFER == TRICE_DOUBLE_BUFFER, this amount of space is allocated once in front of each half buffer.
//! Must be a multiple of 4. 64 is a _very_ safe value. In most cases you can reduce this to 16 or less.
#define TRICE_DATA_OFFSET 64
#endif

#ifndef TRICE_SINGLE_MAX_SIZE
//! TRICE_SINGLE_MAX_SIZE is used to truncate long runtime generated strings, to detect the need of a ring buffer wrap or to protect against overflow.
//! - Be careful with this value: When using 12 64-bit values with a 32-bit stamp the trice size is 2(id) + 4(stamp) + 2(count) + 12*8(values) = 104 bytes.
//! - In direct mode, and also when you enabled TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE, this plus TRICE_DATA_OFFSET plus 4 is the max allocation size on the target
//!   stack with TRICE_BUFFER == TRICE_STACK_BUFFER.
//! - When short of memory and, for example, max 2 32-bit values with a 32-bit stamp are used, the max trice size is 2 + 4 + 2 + 2*4 = 16 bytes.
//! - When not using target timestamps and not more than one 32-bit or two 16-bit or four 8-bit values or using target timestamp without carried values, the TRICE_SINGLE_MAX_SIZE should be 8 bytes only.
//! - With a value < 104 automatically all then forbidden trices are disabled to avoid mistakes. This also reduces the needed flash size and compile time depending on the used compiler.
//! - Example: TRrice( "%d %d %d", 1, 2, 3)` would cause a compiler error with `#define TRICE_SINGLE_MAX_SIZE 16` then.
//! - When NOT using dynamic string or buffer transfer, bigger values than 104 make no sense here and just spoiling RAM.
//! - When USING dynamic string (or buffer) transfer, this value limits the max length of a trice. 2^15-4=32764 is the max possible value if you have enough RAM.
#define TRICE_SINGLE_MAX_SIZE 104 // must be a multiple of 4
#endif

#ifndef TRICE_DEFERRED_BUFFER_SIZE
//! TRICE_DEFERRED_BUFFER_SIZE needs to be capable to hold trice bursts until they are transmitted.
//! When TRICE_BUFFER == TRICE_STACK_BUFFER this value is not used.
//! When TRICE_BUFFER == TRICE_STATIC_BUFFER this value is not used.
//! When TRICE_BUFFER == TRICE_DOUBLE_BUFFER, this is the sum of both half buffers.
//! When TRICE_BUFFER == TRICE_RING_BUFFER, this is the whole buffer.
#define TRICE_DEFERRED_BUFFER_SIZE 1024 // must be a multiple of 4
#endif

#ifndef TRICE_MCU_IS_BIG_ENDIAN
//! TRICE_MCU_IS_BIG_ENDIAN needs to be 1 for TRICE64 macros on big endian MCUs for correct 64-bit values and 32-bit timestamp encoding.
#define TRICE_MCU_IS_BIG_ENDIAN 0 // todo: Set this value automatically thru the used compiler.
#endif

#ifndef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN
//! TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN can be defined to 1 on little endian MCUs if the trice data are needed in network order,
//! or on big endian MCUs if the trice data are needed in little endian order. You should avoid setting this to 1 because
//! it increases the trice storage time and the needed code amount. The default transfer order is little endian as most targets are
//! little endian machines. Do not change this value for big endian MCUs, you better apply the `-triceEndianness bigEndian` CLI switch
//! when using the `trice log` command then. Please consider also the TRICE_MCU_IS_BIG_ENDIAN compiler switch for big endian MCUs.
//! This switch is mainly intended to be used for automated tests.
#define TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN 0
#endif

#ifndef TRICE_DIRECT_OUT_FRAMING
//! TRICE_DIRECT_OUT_FRAMING defines the framing method of the binary trice data stream for direct output. Options:
//! - TRICE_FRAMING_TCOBS: Recommended for internal transfer and trice tool visualization.
//! - TRICE_FRAMING_COBS: The trice tool needs switch `-pf COBS`. Useful with XTEA or to decode the binary trice data with a user tool.
//! - TRICE_FRAMING_NONE: The trice tool needs switch `-pf none`. TRICE_FRAMING_NONE is recommended for fast RTT (32-bit access).
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_NONE
#endif

#ifndef TRICE_DEFERRED_OUT_FRAMING
//! TRICE_DEFERRED_OUT_FRAMING defines the framing method of the binary trice data stream for deferred output. Options:
//! - TRICE_FRAMING_TCOBS: Recommended for UART transfer and trice tool visualization.
//! - TRICE_FRAMING_COBS: The trice tool needs switch `-pf COBS`. Useful with XTEA or to decode the binary trice date with Python or an other language.
//! - TRICE_FRAMING_NONE: The trice tool needs switch `-pf none`. This mode may be helpful if you write your own trice viewer without a decoder.
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_TCOBS
#endif

#ifndef XTEA_ENCRYPT_KEY
//! XTEA_ENCRYPT_KEY allows XTEA TriceEncryption with the key.
//! To get your private XTEA_KEY, call just once "trice log -port ... -password YourSecret -showKey".
//! The byte sequence you see then, copy and use it in your trice.Config.h file.
#define XTEA_ENCRYPT_KEY XTEA_KEY(ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54); //!< -password MySecret
#endif

#ifndef XTEA_DECRYPT
//! XTEA_DECRYPT, when defined, enables device local decryption. Usable for checks or if you use a trice reception capable node to read XTEA encrypted messages.
//! One possible application is, receiving trices, decoding them, finding an ID match in a (from triceF generated) function pointer list and executing the dedicated function.
//! This allows kind of RPC (without immediate return) also with encryption.
#define XTEA_DECRYPT 0
#endif

#ifndef TRICE_DIRECT_XTEA_ENCRYPT
//! TRICE_DIRECT_XTEA_ENCRYPT enables encryption for direct output. Encrypting direct output costs significant computing time in the time critical path!
#define TRICE_DIRECT_XTEA_ENCRYPT 0
#endif

#ifndef TRICE_DEFERRED_XTEA_ENCRYPT
//! TRICE_DEFERRED_XTEA_ENCRYPT enables encryption for deferred output.
#define TRICE_DEFERRED_XTEA_ENCRYPT 0
#endif

#ifndef TRICE_DIAGNOSTICS
//! With TRICE_DIAGNOSTICS == 0, additional trice diagnostics code is removed.
//! During development TRICE_DIAGNOSTICS == 1 helps to optimize the trice buffer sizes.
#define TRICE_DIAGNOSTICS 1
#endif

#ifndef TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE
//! TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE==1 uses standard RTT transfer by using function SEGGER_RTT_WriteNoLock and needs ((TRICE_DIRECT_OUTPUT == 1).
//! - This setting results in unframed RTT trice packages and requires the `-packageFraming none` switch for the appropriate trice tool instance.
//! - Not that fast as with TRICE_SEGGER_RTT_32BIT_WRITE == 1 but still fast and uses pure SEGGER functionality only.
#define TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE 0
#endif

#ifndef TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE
//! TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1 speeds up RTT transfer by using function SEGGER_Write_RTT0_NoCheck32.
//! - This setting results in unframed RTT trice packages and requires the `-packageFraming none` switch for the appropriate trice tool instance.
//!   This squeezes the whole TRICE macro into about 100 processor clocks leaving the data already inside the SEGGER _acUpBuffer.
//! - If you do not wish RTT, or wish RTT with framing, simply set this value to 0.
//! - The trice tool CLI switch -d16 is needed too, because for alignment reasons the 16bit ID field is doubled for 16bit timestamp trice messages.
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 0
#endif

#ifndef TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE
//! TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE == 1 enables channel number 0 for SeggerRTT usage. Only channel 0 works right now for some reason.
//! Than the RTT trice packages can be framed according to the set TRICE_DIRECT_OUT_FRAMING.
//! Not useable together with TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE or TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE
//! Switch this on, if you wish deferred Segger RTT output. Usually RTT is used for direct mode only.
#define TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE 0
#endif

#ifndef TRICE_DIRECT_AUXILIARY8
//! TRICE_DIRECT_AUXILIARY8 enables a user defined direct trice 8bit char write.
#define TRICE_DIRECT_AUXILIARY8 0
#endif

#ifndef TRICE_DIRECT_AUXILIARY32
//! TRICE_DIRECT_AUXILIARY32 enables a user defined direct trice 32bit word write.
#define TRICE_DIRECT_AUXILIARY32 0
#endif

#ifndef TRICE_DEFERRED_AUXILIARY8
//! TRICE_DEFERRED_AUXILIARY8 enables a user defined deferred trice 8bit char write.
#define TRICE_DEFERRED_AUXILIARY8 0
#endif

#ifndef TRICE_DEFERRED_AUXILIARY32
//! TRICE_DEFERRED_AUXILIARY32 enables a user defined deferred trice 32bit word write.
#define TRICE_DEFERRED_AUXILIARY32 0
#endif

#ifndef TRICE_CGO
//! CGO interface for testing the target code with Go only, do not enable normally. Usage examples can be found in the trice/test folder.
#define TRICE_CGO 0
#endif

#ifndef TRICE_CYCLE_COUNTER
//! TRICE_CYCLE_COUNTER adds a cycle counter to each trice message.
//! If 0, do not add cycle counter. The TRICE macros are a bit faster. Lost TRICEs are not detectable by the trice tool. The cycle counter byte ist statically 0xC0.
//! If 1, add an 8-bit cycle counter. The TRICE macros are a bit slower. Lost TRICEs are detectable by the trice tool. The cycle counter byte changes (recommended).
#define TRICE_CYCLE_COUNTER 1
#endif

// Compiler Adaption:

#ifndef USE_SEGGER_RTT_LOCK_UNLOCK_MACROS
//! USE_SEGGER_RTT_LOCK_UNLOCK_MACROS == 1 includes SEGGER_RTT header files even SEGGER_RTT is not used. THis allows SEGGER code for critical sections.
#define USE_SEGGER_RTT_LOCK_UNLOCK_MACROS 0
#endif

#ifndef TRICE_ENTER_CRITICAL_SECTION
//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
//! If trices are used only outside critical sections or interrupts, you can leave this macro empty for more speed. Use only '{' in that case.
//! Examples:
//! - #define TRICE_ENTER_CRITICAL_SECTION { SEGGER_RTT_LOCK() { - does the job for many compilers.
//! - #define TRICE_ENTER_CRITICAL_SECTION {
//! - #define TRICE_ENTER_CRITICAL_SECTION { uint32_t old_mask = cm_mask_interrupts(1); { // copied from test/OpenCM3_STM32F411_Nucleo/triceConfig.h
//! - #define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {
#define TRICE_ENTER_CRITICAL_SECTION {
#endif

#ifndef TRICE_LEAVE_CRITICAL_SECTION
//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
//! If trices are used only outside critical sections or interrupts, you can leave this macro empty for more speed. Use only '}' in that case.
//! Examples:
//! - #define TRICE_LEAVE_CRITICAL_SECTION } SEGGER_RTT_UNLOCK() } - does the job for many compilers.
//! - #define TRICE_LEAVE_CRITICAL_SECTION }
//! - #define TRICE_LEAVE_CRITICAL_SECTION } cm_mask_interrupts(old_mask); } // copied from test/OpenCM3_STM32F411_Nucleo/triceConfig.h
//! - #define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }
#define TRICE_LEAVE_CRITICAL_SECTION }
#endif

#ifndef TRICE_INLINE
#define TRICE_INLINE static inline //! TRICE_INLINE is used for inlining trice code.
#endif

// code space optimization

#ifndef TRICE_8_BIT_SUPPORT
#define TRICE_8_BIT_SUPPORT 1 //!< TRICE_8_BIT_SUPPORT enables/disables all 8-bit Trice macros.
#endif

#ifndef TRICE_16_BIT_SUPPORT
#define TRICE_16_BIT_SUPPORT 1 //!< TRICE_16_BIT_SUPPORT enables/disables all 16-bit Trice macros.
#endif

#ifndef TRICE_32_BIT_SUPPORT
#define TRICE_32_BIT_SUPPORT 1 //!< TRICE_32_BIT_SUPPORT enables/disables all 32-bit Trice macros.
#endif

#ifndef TRICE_64_BIT_SUPPORT
#define TRICE_64_BIT_SUPPORT 1 //!< TRICE_64_BIT_SUPPORT enables/disables all 16-bit Trice macros.
#endif

// According to TRICE_SINGLE_MAX_SIZE the Trice functions are enabled/disabled automatically.
// To force that manually, the user can for example `#define ENABLE_trice32fn_12 0` or `#define ENABLE_trice32fn_12 1` for all variations. See trice8.c trice16.c, ... .

//
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* TRICE_DEFAULT_CONFIG_H_ */
