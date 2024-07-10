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

 ...have a lot of things to do: Copy format string from FLASH memory into a RAM buffer and parse it for format specifiers. Also parse the variadic parameter list and convert each parameter according to its format specifier into a character sequences, what includes several divisions - costly function calls. Concatenate the parts to a new string and deliver it to the output, what often means copying again. A full-featured printf library consumes plenty space and time and many open source projects try to make it better in this or that way. Never ever call a printf-like function in time critical code, like an interrupt - it would crash your target in most cases.
The *trice* calls are usable inside interrupts. 


##  2. <a name='TriceIDs'></a>*Trice* IDs

* Each *Trice* caries an [ID](./TriceUserGuide.md) as runtime replacement for the format string.
* This ID is automatically generated (controllable) and the source code takes it as first parameter inside the `TRICE` macro after the optional target timestamp and context.
* The format specifier string is **not** compiled into the target code. It goes together with the ID into a reference list file [til.json](../test/testdata/til.json)

##  3. <a name='Tricevaluesbitwidth'></a>*Trice* values bit width

* No need to explicit express the value bit width.
* The default parameter width for the `TRICE` macro is 32 bit. It is adaptable for 8- or 16-bit MCUs:
  * Adapt settings inside `triceConfig.h`: `TRICE_DEFAULT_PARAMETER_BIT_WIDTH`. It influences ![./ref/DefaultBitWidth.PNG](./ref/DefaultBitWidth.PNG)
  * Use `-defaultTRICEBitwidth` switch during logging when changing this value.
* The macros `TRICE8`, `TRICE16`, `TRICE32`, `TRICE64` are usable too, to define the bit width explicit.
  * This leads for the smaller bit widths to a data packing and less needed space and bandwidth.
* The fastest `TRICE` macro execution is, when MCU bit width matches the `TRICE`macro bit width.

##  4. <a name='Manyvalueparameters'></a>Many value parameters

* No need to explicit express the values count.
* Up to 12 values are supported directly. Example:
  * `TRICE( "%p | %04x %04x %04x %04x %04x %04x %04x %04x %04x | %f\n", p, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], aFloat(x));`
  * To support more than 12 values for each `TRICE` macro, the *Trice* code on target and host is straightforward extendable up to a total payload of 32767 bytes.
* Each macro can be prolonged with the used parameter count, for example `TRICE8_3` or `TRICE_2` to improve compile time checks.
  * This length code extension can be done automatically using `trice u -addParamCount`.
* There is no variadic values scanning during runtime. The C preprocessor does the work.

##  5. <a name='floatanddoublevalues'></a>`float` and `double` values

These types are mixable with integer types but need to be covered by converter function.

* *float* types use the `aFloat()` function and need a minimal value bit width of 32, to secure correct data transfer.
  * Example:
  
  ```c
   float x = 7.2;
   TRICE32( "%f", aFloat(x));
  ```

* *double* types use the `aDouble()` function and need a value bit width of 64, to secure correct data transfer.
  * Example:

  ```c
   double y = 7.2;
   TRICE64( "float %f and double %f", aFloat(x), aDouble(y));
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

##  6. <a name='RuntimeGenerated0-terminatedStringsTransferwithTRICE_S'></a>Runtime Generated 0-terminated Strings Transfer with `TRICE_S`

* The `%s` format specifier is not directly supported by the `TRICE` macro.
* Strings, known at compile time should be a part of a format string to reduce runtime overhead.
* Strings created at runtime, need a special `TRICE_S` macro, which accepts exactly one type `%s` format specifier. Generated strings are allowed to a size of 32767 bytes each, if the configured *Trice* buffer size matches.
  * Example:

  ```c
   char s[] = "Hello again!";
   TRICE_S( "A runtime string %20s\n", s;
  ```

##  7. <a name='RuntimeGeneratedcountedStringsTransferwithTRICE_N'></a>Runtime Generated counted Strings Transfer with `TRICE_N`

* It is also possible to transfer a buffer with length n using the `TRICE_N` macro.
* This becomes handy for example, when a possibly not 0-terminated string in FLASH memory needs transmission: `TRICE_N( Id(0), "msg: FLASH string is %s", addr, 16 );`

##  8. <a name='RuntimeGeneratedBufferTransferwithTRICE_B'></a>Runtime Generated Buffer Transfer with `TRICE_B`

* A buffer is transmittable with `TRICE_B` and specifying just one format specifier, which is then repeated. Example:

```code
  s = "abcde 12345";
  TRICE_S( Id(65209), "msg:With TRICE_S:%s\n", s );
  len = strlen(s);
  TRICE_N( Id(55770), "sig:With TRICE_N:%s\n", s, len );
  TRICE32( Id(33585), "att:len=%u:With TRICE_B:\n", len);
  TRICE_B( Id(59113), "  %02x", s, len );
  TRICE( Id(40249), "\n" );
  TRICE_B( Id(58119), "%4d", s, len );
  TRICE( Id(65448), "\n" );
```

 This gives: ![./ref/TRICE_B.PNG](./ref/TRICE_B.PNG)
 Channel specifier within the `TRICE_B` format string are not supported.

 If the buffer is not 8 but 16, 32 or 32 bits wide, the macros `TRICE8_B`, `TRICE16_B`, `TRICE32_B` and  `TRICE64_B`, are usable in the same manner.

##  9. <a name='RemotefunctioncallsyntaxsupportwithTRICE_F'></a>Remote function call syntax support with `TRICE_F`

The `TRICE8_F`, `TRICE16_F`, `TRICE32_F`, `TRICE64_F`, macros expect a string without format specifiers which is usable later as a function call. Example:

```code
  TRICE8_F(  Id(51520), "info:FunctionNameW",  b8, count );
  TRICE16_F( Id(57243), "sig:FunctionNameX", b16, count );
  TRICE32_F( Id(34450), "diag:FunctionNameY", b32, count );
  TRICE64_F( Id(37668), "notice:FunctionNameZ", b64, count );
```

(needs further clarification)

* Future extensions are possible:
  * `TRICE_D( Id(0), "dump:32", addr, 160 );` -> The **trice** tool dumps in 32 byte rows.

##  10. <a name='Extendedformatspecifierpossibilities'></a>Extended format specifier possibilities

* Because the format string is interpreted by the **trice** tool written in [Go](https://en.wikipedia.org/wiki/Go_(programming_language)), the **Go** capabilities partial usable.

###  10.1. <a name='Triceformatspecifier'></a>*Trice* format specifier

* The `TRICE` macros are used in **C** code.
* The format strings are interpreted by the **trice** tool, which is written in **Go**.
* The **C** and **Go** format specifier are not equal but similar.
* Therefore, a **T**rice adaption is internally performed.

###  10.2. <a name='OverviewTable'></a>Overview Table

|Format Specifier Type                                           | C | Go| T | remark                                                                      |
|-                                                               | - | - | - | -                                                                           |
|  signed decimal integer                                        | d | d | d | Supported.                                                                  |
|unsigned decimal integer                                        | u | - | u | The **trice** tool changes %u into %d and treats value as unsigned.         |
|  signed decimal integer                                        | i | d | i | The **trice** tool changes %i into %d and treats value as signed.           |
|  signed octal integer                                          | - | o | o | With `trice log -unsigned=false` value is treated as signed.                |
|unsigned octal integer                                          | o | - | o | With `trice log` value is treated as unsigned.                              |
|  signed octal integer with 0o prefix                           | - | O | O | With `trice log -unsigned=false` value is treated as signed.                |
|unsigned octal integer with 0o prefix                           | - | - | O | With `trice log` value is treated as unsigned.                              |
|  signed hexadecimal integer lowercase                          | - | x | x | With `trice log -unsigned=false` value is treated as signed.                |
|unsigned hexadecimal integer lowercase                          | x | - | x | With `trice log` value is treated as unsigned.                              |
|  signed hexadecimal integer uppercase                          | - | X | X | With `trice log -unsigned=false` value is treated as signed.                |
|unsigned hexadecimal integer uppercase                          | X | - | X | With `trice log` value is treated as unsigned.                              |
|  signed binary integer                                         | - | b | b | With `trice log -unsigned=false` value is treated as signed.                |
|unsigned binary integer                                         | - | - | b | With `trice log` value is treated as unsigned.                              |
|decimal floating point, lowercase                               | f | f | f | `aFloat(value)`\|`aDouble(value)`                                           |
|decimal floating point, uppercase                               | - | F | F | `aFloat(value)`\|`aDouble(value)`                                           |
|scientific notation (mantissa/exponent), lowercase              | e | e | e | `aFloat(value)`\|`aDouble(value)`                                           |
|scientific notation (mantissa/exponent), uppercase              | E | E | E | `aFloat(value)`\|`aDouble(value)`                                           |
|the shortest representation of %e or %f                         | g | g | g | `aFloat(value)`\|`aDouble(value)`                                           |
|the shortest representation of %E or %F                         | G | G | G | `aFloat(value)`\|`aDouble(value)`                                           |
|a character as byte                                             | c | - | c | Value can contain ASCII character.                                          |
|a character represented by the corresponding Unicode code point | c | c | c | Value can contain UTF-8 characters if the C-File is edited in UTF-8 format. |
|a quoted character                                              | - | q | q | Supported.                                                                  |
|Unicode escape sequence                                         | - | U | - | Not supported (yet) by *Trice*.                                             |
|the word true or false                                          | - | t | t | Supported.                                                                  |
|a string                                                        | s | s | s | Use `TRICE_S` macro with one and only one runtime generated string.         |
|value in default format                                         | - | v | - | Not supported.                                                              |
|Go-syntax representation of the value                           | - | #v| - | Not supported.                                                              |
|a Go-syntax representation of the type of the value             | - | T | - | Not supported.                                                              |
|pointer address                                                 | p | p | p | Supported.                                                                  |
|a double %% prints a single %                                   | % | % | % | Supported.                                                                  |
| nothing printed                                                | n | - | - | Not supported.                                                              |

* [x] Long story short: Use the `-unsigned=false` switch when you like to see hex numbers and the like as signed values.
* [x] Look in [triceCheck.c](../test/testata/triceCheck.c) for exampe code producing this:

![./ref/TriceCheckOutput.gif](./ref/TriceCheckOutput.gif)

##  11. <a name='UTF-8Support'></a>UTF-8 Support

This is gratis, if you edit your source files containing the format strings in UTF-8:

![./ref/UTF-8Example.PNG](./ref/UTF-8Example.PNG)

The target does not even "know" about that, because it gets only the *Trice* IDs.

##  12. <a name='Switchthelanguagewithoutchangingabitinsidethetargetcode'></a>Switch the language without changing a bit inside the target code

Once the [til.json](../til.json) list is done the user can translate it in any language and exchanging the list switches to another language.

##  13. <a name='Formattagsprototypeflagswidth.precisionlengthspecifierexamples'></a>Format tags prototype `%[flags][width][.precision][length]` specifier examples

* Because the interpretation is done inside the **trice** tool written in Go these all should work:
  * `%-d`
  * `%064b`
  * `%+9.3f`
  * `%+#012.12g`
  * `%+'#012.12E`
  * `%e`
  * `%9.f`
* Anyway tests are needed (Issue #211).
