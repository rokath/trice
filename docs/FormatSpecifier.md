# *Trice* format specifier

- The `TRICE` macros are used in **C** code.
- The format strings are interpreted by the **trice** tool, which is written in **Go**.
- The **C** and **Go** format specifier are not equal but similar.
- Therefore a **T**rice adaption is internally performed.

## Overview Table

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
- [x] Look in [triceCheck.c](../pkg/src/triceCheck.c) for examples.

## Format tags prototype %[flags][width][.precision][length]specifier examples

- `%-d`
- `%064b`
- `%+9.3f`
- `%+#012.12g`
- `%+'#012.12E`
- `%e`
- `%9.f`
