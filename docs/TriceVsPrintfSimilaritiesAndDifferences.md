# *Trice* Similarities and differences to `printf` usage

##  1. <a name='TableofContents'></a>Table of Contents

<!-- vscode-markdown-toc -->
* 1. [Table of Contents](#TableofContents)
* 2. [Printf-like functions](#Printf-likefunctions)
* 3. [*Trice* values bit width](#Tricevaluesbitwidth)
* 4. [Many value parameters](#Manyvalueparameters)
* 5. [`float` and `double` values](#floatanddoublevalues)
* 6. [Runtime generated strings transfer](#Runtimegeneratedstringstransfer)
* 7. [Extended format specifier possibilities](#Extendedformatspecifierpossibilities)
* 8. [Overview Table](#OverviewTable)
* 9. [Format tags prototype %[flags][width][.precision][length]specifier examples](#Formattagsprototypeflagswidth.precisionlengthspecifierexamples)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  2. <a name='Printf-likefunctions'></a>Printf-like functions

 ...have a lot of things to do: Copy format string from FLASH memory into a RAM buffer and parse it for format specifiers. Also parse the variadic parameter list and convert each parameter according to its format specifier into a character sequences, what includes several divisions - costly function calls. Concatenate the parts to a new string and deliver it to the output, what often means copying again. A full featured printf library consumes plenty space and time and many open source projects try to make it better in this or that way. Never ever call a printf-like function in time critical code, like an interrupt - it would crash your target in most cases.

![./ref/TriceCheckOutput.gif](./ref/TriceCheckOutput.gif)

* Each *Trice* caries an [ID](./TriceIDManagement.md) as runtime replacement for the format string.

##  3. <a name='Tricevaluesbitwidth'></a>*Trice* values bit width

* No need to explicit express the value bit width.
* The default parameter width for the `TRICE` macro is 32 bit. It is changeable and recommended for 16-bit MCUs.
  * Adapt settings inside `triceConfig.h` and use `-defaultTRICEBitwidth` switch during logging.
* The macros `TRICE8`, `TRICE16`, `TRICE32`, `TRICE64` are usable too, to define the bit width explicit.
  * This leads for the smaller bit-widths to a data packing and less needed space and bandwidth.
* The fastest `TRICE` macro execution is, when MCU bit width matches the `TRICE`macro bit width.

##  4. <a name='Manyvalueparameters'></a>Many value parameters

* No need to explicit express the values count.
* Up to 12 values are supported directly. Example:
  * `TRICE( "%p | %04x %04x %04x %04x %04x %04x %04x %04x %04x | %f\n", p, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], aFloat(x));`
  * To support more than 12 values for each `TRICE` macro the *Trice* code on target and host is straightforward extendable up to a total payload of 1008 bytes.
* Each macro can be prolonged with the used parameter count, for example `TRICE8_3` or `TRICE_2` to improve compile time checks.
  * It can be done automatically using `trice u -addParamCount`.
* There is no variadic values scanning during runtime. The C preprocessor does the work.

##  5. <a name='floatanddoublevalues'></a>`float` and `double` values

These types are mixable with integer types but need to be covered by converter function.

* *float* types use the `aFloat()` function and need a minimal value bit width of 32, to secure correct data transfer.
  * Example:
  
  ```c
   float x = 7.2;
   TRICE32( "%f", aFloat(x));
  ```

* *double* types use the `aDouble()` function and need need a value bit width of 64, to secure correct data transfer.
  * Example:

  ```c
   double y = 7.2;
   TRICE64( "float %f and double %f", aFloat(x), aDouble(y));
  ```

##  6. <a name='Runtimegeneratedstringstransfer'></a>Runtime generated strings transfer

* The `%s` format specifier is not directly supported by the `TRICE` macro.
* Strings, known at compile time should be a part of a format string to reduce runtime overhead.
* Strings created at runtime, need a special `TRICE_S` macro, which accepts exactly one type `%s` format specifier. Generated strings are allowed to a size of 1000 bytes each, if the configured *Trice* buffer size matches.
  * Example:

  ```c
   char s[] = "Hello again!";
   TRICE_S( "A runtime string %20s\n", s;
  ```

##  7. <a name='Extendedformatspecifierpossibilities'></a>Extended format specifier possibilities

* Because the format string is interpreted by the **trice** tool written in [Go](https://en.wikipedia.org/wiki/Go_(programming_language)), the **Go** capabilities partial usable.

# *Trice* format specifier

- The `TRICE` macros are used in **C** code.
- The format strings are interpreted by the **trice** tool, which is written in **Go**.
- The **C** and **Go** format specifier are not equal but similar.
- Therefore a **T**rice adaption is internally performed.

##  8. <a name='OverviewTable'></a>Overview Table

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

- [x] Long story short: Use the `-unsigned=false` switch when you like to see hex numbers and the like as signed values.
- [x] Look in [triceCheck.c](../pkg/src/triceCheck.c) for exampe code producing this:

![./ref/TriceCheckOutput.gif](./ref/TriceCheckOutput.gif)

##  9. <a name='Formattagsprototypeflagswidth.precisionlengthspecifierexamples'></a>Format tags prototype %[flags][width][.precision][length]specifier examples

- `%-d`
- `%064b`
- `%+9.3f`
- `%+#012.12g`
- `%+'#012.12E`
- `%e`
- `%9.f`
