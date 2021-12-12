# *Trice* format specifier

- The `TRICE` macros are used in **C** code.
- The format strings are interpreted by the **trice** tool, which is written in **Go**.
- The **C** and **Go** format specifier are not equal but similar.
- Therefore an adaption is needed.
- Some **Go** behavior 

|Format Specifier Type                                           | C | Go| trice, *trice* value                 |
|-                                                               | - | - | -                                    |
|  signed decimal integer                                        | d | d | d                                    |
|  signed decimal integer                                        | i | d | i, i→d signed                        |
|  signed octal integer                                          | - | o | o with `trice log -unsigned=false`   |
|  signed octal integer with 0o prefix                           | - | O | O with `trice log -unsigned=false`   |
|  signed hexadecimal integer lowercase                          | - | x | x with `trice log -unsigned=false`   |
|  signed hexadecimal integer uppercase                          | - | X | X with `trice log -unsigned=false`   |
|  signed binary integer                                         | - | b | b with `trice log -unsigned=false`   |
|unsigned decimal integer                                        | u | - | u, u→d unsigned                      |
|unsigned octal integer                                          | o | - | o                                    |
|unsigned octal integer with 0o prefix                           | - | - | O                                    |
|unsigned hexadecimal integer lowercase                          | x | - | x                                    |
|unsigned hexadecimal integer uppercase                          | X | - | X                                    |
|unsigned binary integer                                         | - | - | b                                    |
|decimal floating point, lowercase                               | f | f | f, `aFloat(value)`\|`aDouble(value)` |
|decimal floating point, uppercase                               | - | F | F, `aFloat(value)`\|`aDouble(value)` |
|scientific notation (mantissa/exponent), lowercase              | e | e | e, `aFloat(value)`\|`aDouble(value)` |
|scientific notation (mantissa/exponent), uppercase              | E | E | E, `aFloat(value)`\|`aDouble(value)` |
|the shortest representation of %e or %f                         | g | g | g, `aFloat(value)`\|`aDouble(value)` |
|the shortest representation of %E or %F                         | G | G | G, `aFloat(value)`\|`aDouble(value)` |
|a character as byte                                             | c | - | -                                    |
|a character represented by the corresponding Unicode code point | c | c | c, Format strings can contain UTF-8 characters if the C-File is edited in UTF-8 format.|
|a quoted character                                              | - | q | -                                    |
|Unicode escape sequence                                         | - | U | -                                    |
|the word true or false                                          | - | t | -                                    |
|a string                                                        | s | s | s                                    |
|value in default format                                         | - | v | -                                    |
|Go-syntax representation of the value                           | - | #v| -                                    |
|a Go-syntax representation of the type of the value             | - | T | -                                    |
|pointer address                                                 | p | p | p                                    |
|a double %% prints a single %                                   | % | % | %                                    |
| nothing printed                                                | n | - | -                                    |

## Format tags prototype %[flags][width][.precision][length]specifier examples

- `%-d`
- `%064b`
- `%+9.3f`
- `%+#012.12g`
- `%+'#012.12E`
- `%e`
- `%9.f`   width 9, precision 0
