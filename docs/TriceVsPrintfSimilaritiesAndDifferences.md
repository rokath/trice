# *Trice* Similarities and differences to `printf` usage

> _(Read only you are interested in)_

<!-- vscode-markdown-toc -->
* 1. [Printf-like functions](#Printf-likefunctions)
* 2. [*Trice* IDs](#TriceIDs)
* 3. [*Trice* values bit width](#Tricevaluesbitwidth)
* 4. [Many value parameters](#Manyvalueparameters)
* 5. [`float` and `double` values](#floatanddoublevalues)
* 6. [Runtime Generated 0-terminated Strings Transfer with `TRICE_S`](#RuntimeGenerated0-terminatedStringsTransferwithTRICE_S)
* 7. [Runtime Generated counted Strings Transfer with `TRICE_N`](#RuntimeGeneratedcountedStringsTransferwithTRICE_N)
* 8. [Runtime Generated Buffer Transfer with `TRICE_B`](#RuntimeGeneratedBufferTransferwithTRICE_B)
* 9. [Remote function call syntax support with `TRICE_F`](#RemotefunctioncallsyntaxsupportwithTRICE_F)
* 10. [Extended format specifier possibilities](#Extendedformatspecifierpossibilities)
	* 10.1. [*Trice* format specifier](#Triceformatspecifier)
	* 10.2. [Overview Table](#OverviewTable)
* 11. [UTF-8 Support](#UTF-8Support)
* 12. [Switch the language without changing a bit inside the target code](#Switchthelanguagewithoutchangingabitinsidethetargetcode)
* 13. [Format tags prototype `%[flags][width][.precision][length]` specifier examples](#Formattagsprototypeflagswidth.precisionlengthspecifierexamples)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  1. <a name='Printf-likefunctions'></a>Printf-like functions

 ...have a lot of things to do: Copy format string from FLASH memory into a RAM buffer and parse it for format specifiers. Also parse the variadic parameter list and convert each parameter according to its format specifier into a character sequences, what includes several divisions - costly function calls. Concatenate the parts to a new string and deliver it to the output, what often means copying again. A full-featured printf library consumes plenty space and processing time and several open source projects try to make it better in this or that way. Never ever call a printf-like function in time critical code, like an interrupt - it would crash your target in most cases.
The *trice* calls are usable inside interrupts, because they only need a few MCU clocks for execution. Porting legacy code to use it with the Trice library, means mainly to replace Printf-like function calls with `trice` function calls.


##  2. <a name='TriceIDs'></a>*Trice* IDs

* Each *Trice* caries a 14-bit nuber ID as replacement for the format string.
* This ID is automatically generated (controllable) and in the source code it is the first parameter inside the `trice` macro followed by the format string and optional values.
* The user can decide not to spoil the code by having the IDs permanently in its source code, by just inserting them as a pre-compile step with `trice insert` and removing them as a post-compile step with `trice clean`.
  * The Trice cache makes this invisible to the build system, allowing full translation speed.
* The format string is **not** compiled into the target code. It goes together with the ID into a project specific reference list file [til.json](../_test/testdata/til.json) (example).

##  3. <a name='Tricevaluesbitwidth'></a>*Trice* values bit width

* No need to explicit express the value bit width.
* The default parameter width for the `trice` macro is 32 bit. It is changeable to 8, 16 or 64-bit:
  * Adapt settings inside `triceConfig.h`: `TRICE_DEFAULT_PARAMETER_BIT_WIDTH`. It influences ![./ref/DefaultBitWidth.PNG](./ref/DefaultBitWidth.PNG)
  * Use `-defaultTRICEBitwidth` switch during logging when changing this value.
* The macros `trice8`, `trice16`, `trice32`, `trice64` are usable too, to define the bit width explicit.
  * This leads for the smaller bit widths to less needed space and bandwidth. But when using the default package framing TCOBS, the influence is marginal because of the implicit compression.
* The fastest `trice` macro execution is, when MCU bit width matches the `trice`macro bit width.

##  4. <a name='Manyvalueparameters'></a>Many value parameters

* No need to explicit express the values count.
* Up to 12 values are supported directly. Example:
  * `trice( "%p | %04x %04x %04x %04x %04x %04x %04x %04x %04x | %f\n", p, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], aFloat(x));`
  * To support more than 12 values for each `trice` macro, the *Trice* code on target and host is straightforward extendable up to a total payload of 32764 bytes.
* Each macro can be prolonged with the used parameter count, for example `TRICE8_3` or `TRICE_2` to intense compile time checks.
  * This length code extension can be done automatically using `trice u -addParamCount`.
* The _Trice_ tool compares the number of given format specifiers with the written parameters in a precimpile step to minimize the risk of runtime errors.
* There is no variadic values scanning during runtime. The C preprocessor does the work.

##  5. <a name='floatanddoublevalues'></a>`float` and `double` values

These types are mixable with integer types but need to be covered by converter function.

* *float* types use the `aFloat()` function and need a minimal value bit width of 32, to secure correct data transfer.
  * Example:
  
  ```c
   float x = 7.2;
   trice( "%f", aFloat(x));
  ```

* *double* types use the `aDouble()` function and need a value bit width of 64, to secure correct data transfer.
  * Example:

  ```c
   double y = 7.2;
   trice64( "float %f and double %f", aFloat(x), aDouble(y));
  ```

* Both functions are simple and fast:

```C

// aFloat returns passed float value x as bit pattern in a uint32_t type.
static inline uint32_t aFloat( float x ){
    union {
        float f;
        uint32_t u;
    } t;
    t.f = x;
    return t.u;
}

// aDouble returns passed double value x as bit pattern in a uint64_t type.
static inline uint64_t aDouble( double x ){
    union {
        double d;
        uint64_t u;
    } t;
    t.d = x;
    return t.u;
}
```

##  6. <a name='RuntimeGenerated0-terminatedStringsTransferwithTRICE_S'></a>Runtime Generated 0-terminated Strings Transfer with `triceS`, `TriceS`, `TRiceS`

* The `%s` format specifier is supported by the `trice` macro too but needs specific treatment.
* Strings, known at compile time should be a part of a format string to reduce runtime overhead.
* Strings created at runtime, need a special `TRICE_S` (or `triceS`, `TriceS`, `TRiceS`) macro, which accepts exactly one type `%s` format specifier. Generated strings are allowed to a size of 32764 bytes each, if the configured *Trice* buffer size matches.
  * Example:

  ```c
   char s[] = "Hello again!";
   triceS( "A runtime string %20s\n", s;
  ```

##  7. <a name='RuntimeGeneratedcountedStringsTransferwithTRICE_N'></a>Runtime Generated counted Strings Transfer with  `triceN`, `TriceN`, `TRiceN`

* It is also possible to transfer a buffer with length n using the `TRICE_N` (or `triceN`, `TriceN`, `TRiceN`) macro.
* This becomes handy for example, when a possibly not 0-terminated string in FLASH memory needs transmission: `triceN( "msg: FLASH string is %s", addr, 16 );`
* There are also specific macros like `trice32B` or `trice16F`. Please look into [triceCheck.c](../_test/testdata/triceCheck.c) for usage or see the following.

##  8. <a name='RuntimeGeneratedBufferTransferwithTRICE_B'></a>Runtime Generated Buffer Transfer with `triceB`, `TriceB`, `TRiceB`

* A buffer is transmittable with `TRICE_B` (or `triceB`, `TriceB`, `TRiceB`) and specifying just one format specifier, which is then repeated. Example:

```code
  s = "abcde 12345"; // assume this as runtime generated string
  triceS( "msg:Show s with triceS: %s\n", s );
  len = strlen(s);
  triceN( "sig:Show s with triceN:%s\n", s, len );
  triceB( "dbg: %02x\n", s, len ); // Show s as colored code sequence in hex code.
  triceB( "msg: %4d\n", s, len ); // Show s as colored code sequence in decimal code.
```

 This gives output similar to: ![./ref/TRICE_B.PNG](./ref/TRICE_B.PNG)
 Channel specifier within the `TRICE_B` format string are supported in Trice versions >= v0.66.0.

 If the buffer is not 8 but 16, 32 or 32 bits wide, the macros `TRICE8_B`, `TRICE16_B`, `TRICE32_B` and  `TRICE64_B`, are usable in the same manner.

##  9. <a name='RemotefunctioncallsyntaxsupportwithTRICE_F'></a>Remote function call syntax support with `TRICE_F`, `trice8F`, ...

The `TRICE8_F`, `TRICE16_F`, `TRICE32_F`, `TRICE64_F`, macros expect a string without format specifiers which is usable later as a function call. Examples:

```code
trice8F(   "call:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time:            default: call:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
TRICE16_F( "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time: 842,150_450default: info:FunctionNameX(0000)(ffff)(fffe)(3344) 
TRice16F(  "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time: 842,150_450default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
Trice16F(  "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:       5_654default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
trice16F(  "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:            default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
TRICE32_F( "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time: 842,150_450default: info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
TRice32F(  "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time: 842,150_450default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
Trice32F(  "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:       5_654default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
trice32F(  "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:            default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
TRICE64_F( "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time: 842,150_450default: info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
TRice64F(  "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time: 842,150_450default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
Trice64F(  "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:       5_654default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
trice64F(  "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:            default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
```

The Trice tool displays the parameter buffer in the shown manner. It is planned to code a FunctionPointerList Generator (See [issue #303](https://github.com/rokath/trice/issues/303), which generates mainly a function pointer list with associated IDs. This list can get part of the source code of a remote device. Then, when receiving a Trice message, the remote device can execute the assigned function call using the transferred parameters. This way several devices can communicate in an easy and reliable way.

* Future extensions are possible:
  * `triceD( "dump:32", addr, 160 );` -> The **trice** tool dumps in 32 byte rows.
  * An appropriate syntax is needed.

##  10. <a name='Extendedformatspecifierpossibilities'></a>Extended format specifier possibilities

* Because the format string is interpreted by the **trice** tool written in [Go](https://en.wikipedia.org/wiki/Go_(programming_language)), the **Go** capabilities partial usable.

###  10.1. <a name='Triceformatspecifier'></a>*Trice* format specifier

* The `TRICE` macros are used in **C** code.
* The format strings are interpreted by the **trice** tool, which is written in **Go**.
* The **C** and **Go** format specifier are not equal but similar.
* Therefore, a **T**rice adaption is internally performed.

###  10.2. <a name='OverviewTable'></a>Overview Table

| Format Specifier Type                                           | C | Go | T | remark                                                                      |
|-----------------------------------------------------------------|---|----|---|-----------------------------------------------------------------------------|
| signed decimal integer                                          | d | d  | d | Supported.                                                                  |
| unsigned decimal integer                                        | u | -  | u | The **trice** tool changes %u into %d and treats value as unsigned.         |
| signed decimal integer                                          | i | d  | i | The **trice** tool changes %i into %d and treats value as signed.           |
| signed octal integer                                            | - | o  | o | With `trice log -unsigned=false` value is treated as signed.                |
| unsigned octal integer                                          | o | -  | o | With `trice log` value is treated as unsigned.                              |
| signed octal integer with 0o prefix                             | - | O  | O | With `trice log -unsigned=false` value is treated as signed.                |
| unsigned octal integer with 0o prefix                           | - | -  | O | With `trice log` value is treated as unsigned.                              |
| signed hexadecimal integer lowercase                            | - | x  | x | With `trice log -unsigned=false` value is treated as signed.                |
| unsigned hexadecimal integer lowercase                          | x | -  | x | With `trice log` value is treated as unsigned.                              |
| signed hexadecimal integer uppercase                            | - | X  | X | With `trice log -unsigned=false` value is treated as signed.                |
| unsigned hexadecimal integer uppercase                          | X | -  | X | With `trice log` value is treated as unsigned.                              |
| signed binary integer                                           | - | b  | b | With `trice log -unsigned=false` value is treated as signed.                |
| unsigned binary integer                                         | - | -  | b | With `trice log` value is treated as unsigned.                              |
| decimal floating point, lowercase                               | f | f  | f | `aFloat(value)`\|`aDouble(value)`                                           |
| decimal floating point, uppercase                               | - | F  | F | `aFloat(value)`\|`aDouble(value)`                                           |
| scientific notation (mantissa/exponent), lowercase              | e | e  | e | `aFloat(value)`\|`aDouble(value)`                                           |
| scientific notation (mantissa/exponent), uppercase              | E | E  | E | `aFloat(value)`\|`aDouble(value)`                                           |
| the shortest representation of %e or %f                         | g | g  | g | `aFloat(value)`\|`aDouble(value)`                                           |
| the shortest representation of %E or %F                         | G | G  | G | `aFloat(value)`\|`aDouble(value)`                                           |
| a character as byte                                             | c | -  | c | Value can contain ASCII character.                                          |
| a character represented by the corresponding Unicode code point | c | c  | c | Value can contain UTF-8 characters if the C-File is edited in UTF-8 format. |
| a quoted character                                              | - | q  | q | Supported.                                                                  |
| the word true or false                                          | - | t  | t | Supported.                                                                  |
| a string                                                        | s | s  | s | Use `triceS` macro with one and only one runtime generated string.         |
| pointer address                                                 | p | p  | p | Supported.                                                                  |
| a double %% prints a single %                                   | % | %  | % | Supported.                                                                  |
| Unicode escape sequence                                         | - | U  | - | **Not supported.**                                                          |
| value in default format                                         | - | v  | - | **Not supported.**                                                          |
| Go-syntax representation of the value                           | - | #v | - | **Not supported.**                                                          |
| a Go-syntax representation of the type of the value             | - | T  | - | **Not supported.**                                                          |
| nothing printed                                                 | n | -  | - | **Not supported.**                                                          |

* [x] Long story short: Use the `-unsigned=false` switch when you like to see hex numbers and the like as signed values.
* [x] Look in [triceCheck.c](../_test/testdata/triceCheck.c) for exampe code producing this:

![./ref/TriceCheckOutput.gif](./ref/TriceCheckOutput.gif)

##  11. <a name='UTF-8Support'></a>UTF-8 Support

This is gratis, if you edit your source files containing the format strings in UTF-8:

![./ref/UTF-8Example.PNG](./ref/UTF-8Example.PNG)

The target does not even "know" about that, because it gets only the *Trice* IDs.

##  12. <a name='Switchthelanguagewithoutchangingabitinsidethetargetcode'></a>Switch the language without changing a bit inside the target code

Once the [til.json](../examples/F030R8_inst/til.json) list is done the user can translate it in any language and exchanging the list switches to another language.

##  13. <a name='Formattagsprototypeflagswidth.precisionlengthspecifierexamples'></a>Format tags prototype `%[flags][width][.precision][length]` specifier examples

* Because the interpretation is done inside the **trice** tool written in Go these all should work:
  * `%-d`
  * `%064b`
  * `%+9.3f`
  * `%+#012.12g`
  * `%+'#012.12E`
  * `%e`
  * `%9.f`

