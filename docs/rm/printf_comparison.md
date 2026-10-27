# Writing Trice Messages

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Printf-like functions](#printf-like-functions)
* [2. Trice IDs](#trice-ids)
* [3. Trice values bit width](#trice-values-bit-width)
* [4. Many value parameters](#many-value-parameters)
* [5. Floating Point Values](#floating-point-values)
* [6. Runtime Generated 0-terminated Strings Transfer with triceS](#runtime-generated-0-terminated-strings-transfer-with-trices)
* [7. Runtime Generated counted Strings Transfer with triceN](#runtime-generated-counted-strings-transfer-with--tricen)
* [8. Runtime Generated Buffer Transfer with triceB](#runtime-generated-buffer-transfer-with-triceb)
* [9. Extended format specifier possibilities](#extended-format-specifier-possibilities)
  * [9.1. Trice format specifier](#trice-format-specifier)
  * [9.2. Length modifier support](#length-modifier-support)
  * [9.3. Overview Table](#overview-table)
* [10. Unsupported printf format features](#unsupported-printf-format-features)
  * [10.1. Dynamic field width with *](#dynamic-field-width-with-)
  * [10.2. Dynamic precision with *](#dynamic-precision-with-)
  * [10.3. Dynamic width and precision together](#dynamic-width-and-precision-together)
  * [10.4. Wide character and wide string formats: %lc and %ls](#wide-character-and-wide-string-formats-lc-and-ls)
  * [10.5. The special %n conversion specifier](#the-special-n-conversion-specifier)
  * [10.6. Security implications of %n](#security-implications-of-n)
  * [10.7. Why Trice does not support %n](#why-trice-does-not-support-n)
* [11. UTF-8 Support](#utf-8-support)
* [12. Switch the language without changing a bit inside the target code](#switch-the-language-without-changing-a-bit-inside-the-target-code)
* [13. Format tags prototype specifier examples](#format-tags-prototype-specifier-examples)
* [14. Target Macros](#target-macros)
* [15. Trice Parameter Bit Widths](#trice-parameter-bit-widths)
* [16. Buffer Macros](#buffer-macros)
* [17. Adapting printf calls](#adapting-printf-calls)
  * [17.1. User Code Adaptation](#user-code-adaptation)
* [18. Special Care Demands](#special-care-demands)
  * [18.1. More than 12 printf parameters](#more-than-12-printf-parameters)
  * [18.2. Float Numbers](#float-numbers)
  * [18.3. Double numbers](#double-numbers)
  * [18.4. Runtime Generated Strings](#runtime-generated-strings)
* [19. UTF-8 example](#utf-8-example)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## 1. <a id="printf-like-functions"></a>Printf-like functions

Printf formats text on the target; Trice transfers an ID and values and formats them on the host. This reduces target work and keeps format strings out of the firmware. For timing-sensitive code, choose the [buffer and output mode](performance.md) carefully and configure [critical sections](target_configuration.md#critical-sections-and-checks) for concurrent calls.

For existing logging wrappers, see [Integrating Existing Logging Macros](legacy_user_code.md).

## 2. <a id="trice-ids"></a>Trice IDs

Write calls without numeric IDs, for example `trice("ready\n");`. [Bind](bind.md) assigns the 14-bit IDs and generates compiler headers before compilation. The format string and ID are stored in the [dictionary](id_management.md), not in the firmware.

## 3. <a id="trice-values-bit-width"></a>Trice values bit width

* No need to explicit express the value bit width.
* The default parameter width for the Trice macro is 32 bit. It is changeable to 8, 16 or 64-bit:
  * Adapt `TRICE_DEFAULT_PARAMETER_BIT_WIDTH` inside `triceConfig.h`. It influences ![./ref/DefaultBitWidth.PNG](../ref/DefaultBitWidth.PNG)
  * Use `-defaultTRICEBitwidth` switch during logging when changing this value.
* The macros `trice8`, `trice16`, `trice32`, `trice64` are usable too, to define the bit width explicit.
  * This leads for the smaller bit widths to less needed space and bandwidth. But when using the default package framing TCOBS, the influence is marginal because of the implicit compression.
* The fastest Trice macro execution is, when MCU bit width matches the macro bit width.
* The implicit TCOBS compression compacts the binary Trice data during the framing.

## 4. <a id="many-value-parameters"></a>Many value parameters

* No need to explicit express the values count.
* Up to 12 values are supported directly. Example:
  * `trice( "%p | %04x %04x %04x %04x %04x %04x %04x %04x %04x | %f\n", p, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], aFloat(x));
  * The long count field can encode up to 32767 payload bytes, but the current target buffer macros impose a lower limit; see [Binary Encoding](binary_encoding.md). Supporting more than 12 values per Trice macro would require extending the target and host code.
* Each macro can be prolonged with the used parameter count, for example `TRICE8_3` or `TRICE_2` to intense compile time checks.
  * This length code extension can be done automatically using `trice u -addParamCount`. This is not needed anymore:
* The _Trice_ tool compares the number of given format specifiers with the written parameters in a precimpile step to minimize the risk of runtime errors.
* There is no variadic values scanning during runtime. The C preprocessor does the work.

## 5. <a id="floating-point-values"></a>Floating Point Values

Use `aFloat()` with 32-bit calls and `aDouble()` with 64-bit calls. All values in a call share the selected width; convert a float to double as well when placing it in a 64-bit call. Integers can share either call.

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
   trice64( "float %f and double %f", aDouble(x), aDouble(y));
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

## 6. <a id="runtime-generated-0-terminated-strings-transfer-with-trices"></a>Runtime Generated 0-terminated Strings Transfer with triceS

* The `%s` format specifier is supported by the Trice macro too but needs specific treatment.
* Strings, known at compile time should be a part of a format string to reduce runtime overhead.
* Strings created at runtime need a special `TRICE_S` (or `triceS`, `TriceS`, `TRiceS`) macro, which accepts exactly one `%s` format specifier. It transfers at most `TRICE_SINGLE_MAX_SIZE - 12` string bytes, excluding the terminating zero byte: 92 bytes by default or at most 32752 bytes with the largest supported configuration. Longer strings are truncated.
  * Example:

  ```c
   char s[] = "Hello again!";
   triceS("A runtime string %20s\n", s);
  ```

Trice was designed mainly for speed. An universal `trice` like `printf` would cost too much runtime and destroy this main Trice advantage.

If you need several `%s`, like in 

```C
char* n = "Ann";
char* f = "Fox";
uint8_t dd = 22;
uint8_t mm = 11;
uint16_t yyyy = 1988;
// ...
print( "Name: %12s, Family: %s, Birthday %2u-%02u-%4u\n",  n, f, dd, mm, yyyy );
```
you can do:

```C
// ...
triceS( "Name: %12s, ",  n );
triceS( "Family: %s, ", f );
trice( "Birthday %2u-%02u-%4u\n", dd, mm, yyyy );
```
 or also

```C
// ...
triceS( "Name: %12s, ",  n ); triceS( "Family: %s, ", f ); trice( "Birthday %2u-%02u-%4u\n", dd, mm, yyyy );
```

## 7. <a id="runtime-generated-counted-strings-transfer-with--tricen"></a>Runtime Generated counted Strings Transfer with  triceN

* It is also possible to transfer a buffer with length n using the `TRICE_N` (or `triceN`, `TriceN`, `TRiceN`) macro.
* This becomes handy for example, when a possibly not 0-terminated string in FLASH memory needs transmission: `triceN( "msg: FLASH string is %s", addr, 16 );
* There are also specific macros like `trice32B`. Please look into [triceCheck.c](../../_test/testdata/triceCheck.c) for usage or see the following.

## 8. <a id="runtime-generated-buffer-transfer-with-triceb"></a>Runtime Generated Buffer Transfer with triceB

* A buffer is transmittable with `TRICE_B` (or `triceB`, `TriceB`, `TRiceB`) and specifying just one format specifier, which is then repeated. Example:

```code
  s = "abcde 12345"; // assume this as runtime generated string
  triceS( "msg:Show s with triceS: %s\n", s );
  len = strlen(s);
  triceN( "sig:Show s with triceN:%s\n", s, len );
  triceB( "dbg: %02x\n", s, len ); // Show s as colored code sequence in hex code.
  triceB( "msg: %4d\n", s, len ); // Show s as colored code sequence in decimal code.
```

  This gives output similar to: ![./ref/TRICE_B.PNG](../ref/TRICE_B.PNG)

  Channel specifier within the `TRICE_B` format string are supported in Trice versions >= v0.66.0.

 If the buffer is not 8 but 16, 32 or 32 bits wide, the macros `TRICE8_B`, `TRICE16_B`, `TRICE32_B` and  `TRICE64_B`, are usable in the same manner.



## 9. <a id="extended-format-specifier-possibilities"></a>Extended format specifier possibilities

* Because the format string is interpreted by the Trice tool written in [Go](https://en.wikipedia.org/wiki/Go_(programming_language)), the **Go** capabilities partial usable.

### 9.1. <a id="trice-format-specifier"></a>Trice format specifier

* The Trice macros are used in **C** code.
* The format strings are interpreted by the Trice tool, which is written in **Go**.
* The **C** and **Go** format specifier are not equal but similar.
* Therefore, a **T**rice adaptation is internally performed.

### 9.2. <a id="length-modifier-support"></a>Length modifier support

* Trice now accepts the common C length modifiers `hh`, `h`, `l`, `ll`, `j`, `z`, `t`, and `L` together with the corresponding supported conversion specifiers.
* This is useful for ordinary Trice macros as well as for buffer macros such as `TRICE8_B`, `TRICE16_B`, `TRICE32_B`, and `TRICE64_B`.
* The original C format string stays unchanged in the lookup data. Internally, the Trice tool normalizes only a temporary working copy so that the host side can format the values with Go.
* Therefore, these examples are accepted and decoded as expected:
  * `%4ld` behaves like `%4d`
  * `%zu` behaves like `%u`
  * `%02zx` behaves like `%02x`
  * `%02llx` behaves like `%02x`
  * `%04Lf` behaves like `%04f`
* The normalization keeps flags, field width, and precision. Only the C length modifier itself is removed from the host side working copy.
* Invalid combinations are not legalized by Trice. For example, `%LX` is not treated as a valid integer format because `L` belongs to floating-point conversions such as `%Lf`, not to `%X`.

### 9.3. <a id="overview-table"></a>Overview Table

In the remarks, ✅ means supported and ❌ means unsupported. The format-specifier columns retain their literal spellings.

Format Specifier Type                                           | C | Go | T | (T =Trice) \| remark
-----------------------------------------------------------------|---|----|---|-----------------------------------------------------------------------------
signed decimal integer                                          | d | d  | d | ✅ Supported.
unsigned decimal integer                                        | u | -  | u | The Trice tool changes %u into %d and treats value as unsigned.
signed decimal integer                                          | i | d  | i | The Trice tool changes %i into %d and treats value as signed.
signed octal integer                                            | - | o  | o | With `trice log -unsigned=false` value is treated as signed.
unsigned octal integer                                          | o | -  | o | With `trice log` value is treated as unsigned.
signed octal integer with 0o prefix                             | - | O  | O | With `trice log -unsigned=false` value is treated as signed.
unsigned octal integer with 0o prefix                           | - | -  | O | With `trice log` value is treated as unsigned.
signed hexadecimal integer lowercase                            | - | x  | x | With `trice log -unsigned=false` value is treated as signed.
unsigned hexadecimal integer lowercase                          | x | -  | x | With `trice log` value is treated as unsigned.
signed hexadecimal integer uppercase                            | - | X  | X | With `trice log -unsigned=false` value is treated as signed.
unsigned hexadecimal integer uppercase                          | X | -  | X | With `trice log` value is treated as unsigned.
signed binary integer                                           | - | b  | b | With `trice log -unsigned=false` value is treated as signed.
unsigned binary integer                                         | - | -  | b | With `trice log` value is treated as unsigned.
decimal floating point, lowercase                               | f | f  | f | `aFloat(value)`\|`aDouble(value)`
decimal floating point, uppercase                               | - | F  | F | `aFloat(value)`\|`aDouble(value)`
scientific notation (mantissa/exponent), lowercase              | e | e  | e | `aFloat(value)`\|`aDouble(value)`
scientific notation (mantissa/exponent), uppercase              | E | E  | E | `aFloat(value)`\|`aDouble(value)`
the shortest representation of %e or %f                         | g | g  | g | `aFloat(value)`\|`aDouble(value)`
the shortest representation of %E or %F                         | G | G  | G | `aFloat(value)`\|`aDouble(value)`
a character as byte                                             | c | -  | c | Value can contain ASCII character.
a character represented by the corresponding Unicode code point | c | c  | c | Value can contain UTF-8 characters if the C-File is edited in UTF-8 format.
a quoted character                                              | - | q  | q | ✅ Supported.
the word true or false                                          | - | t  | t | ✅ Supported.
a string                                                        | s | s  | s | Use `triceS` macro with one and only one runtime generated string.
pointer address                                                 | p | p  | p | ✅ Supported.
a double %% prints a single %                                   | % | %  | % | ✅ Supported.
Unicode escape sequence                                         | - | U  | - | ❌ **Not supported.**
value in default format                                         | - | v  | - | ❌ **Not supported.**
Go-syntax representation of the value                           | - | #v | - | ❌ **Not supported.**
a Go-syntax representation of the type of the value             | - | T  | - | ❌ **Not supported.**
nothing printed                                                 | n | -  | - | ❌ **Not supported.**

* [x] Long story short: Use the `-unsigned=false` switch when you like to see hex numbers and the like as signed values.
* [x] Look in [triceCheck.c](../../_test/testdata/triceCheck.c) for exampe code producing this:

[![TriceCheck output with numeric and string values](../ref/TriceCheckOutput-preview.png)](../ref/TriceCheckOutput.gif)

Select the preview to open the full animation.

## 10. <a id="unsupported-printf-format-features"></a>Unsupported `printf` format features

Trice supports the common `printf`-style format specifiers used for embedded logging. Some less common `printf` features are intentionally not supported yet, because they do not fit well into the current lightweight Trice argument handling model or because they introduce side effects that are unsuitable for logging.

This mainly concerns:

* dynamic field width with `*`, for example `%*d`
* dynamic precision with `*`, for example `%.*s` or `%*.*f`
* wide character and wide string formats such as `%lc` and `%ls`
* the `%n` conversion specifier

### 10.1. <a id="dynamic-field-width-with-"></a>Dynamic field width with `*`

In standard `printf`, a field width can either be fixed inside the format string or supplied dynamically.

Example with fixed width:

```c
printf("%10d", value);
```

Example with dynamic width:

```c
printf("%*d", width, value);
```

The `*` is not the value to be printed. It tells `printf` to consume an additional `int` argument from the argument list and to use that value as the field width. Therefore:

```c
printf("%*d", 10, value);
```

behaves like:

```c
printf("%10d", value);
```

A negative dynamic width has a special meaning and implies left-aligned output, similar to the `-` flag.

### 10.2. <a id="dynamic-precision-with-"></a>Dynamic precision with `*`

The same principle exists for precision.

Example with fixed precision:

```c
printf("%.3f", value);
```

Example with dynamic precision:

```c
printf("%.*f", precision, value);
```

Again, the `*` consumes an additional `int` argument. For strings this is often used to limit the maximum number of emitted characters:

```c
printf("%.*s", maxLen, text);
```

### 10.3. <a id="dynamic-width-and-precision-together"></a>Dynamic width and precision together

Both dynamic field width and dynamic precision can be used in the same conversion:

```c
printf("%*.*f", width, precision, value);
```

This consumes three arguments:

```c
int width;
int precision;
double value;
```

The important point is that each `*` consumes an additional `int` argument before the actual value argument. This means that `%*.*f` does not correspond to one runtime value only. It corresponds to `int, int, double`.

Trice is designed so that a format string usually maps to a compact and predictable sequence of transmitted values. Dynamic width and dynamic precision break that simple mapping because the `*` tokens are not visible output conversions, but they still consume additional arguments.

For that reason, Trice currently does not support these forms. Supporting them correctly would require the Trice format analysis to count and encode the hidden width and precision arguments in addition to the visible value arguments.

### 10.4. <a id="wide-character-and-wide-string-formats-lc-and-ls"></a>Wide character and wide string formats: `%lc` and `%ls`

The `%lc` and `%ls` conversions are valid C `printf` forms for wide character and wide string data.

They are not equivalent to `%c` and `%s`:

* `%c` and `%s` operate on narrow character data
* `%lc` and `%ls` operate on wide character data such as `wchar_t`

For Trice this matters because `triceS` and related string handling transport runtime buffers, but the Trice tool does not automatically know how a target represents wide characters internally.

Without additional target-specific metadata, the host side would not know:

* whether `wchar_t` on the target is 16 bit or 32 bit
* which byte order is used
* which encoding semantics should be assumed for the transported code units

Therefore, simply removing the `l` and treating `%lc` like `%c` or `%ls` like `%s` would not be correct. That would silently change the meaning of the original C format string and could decode the payload differently from what the target-side C code actually describes.

For that reason, Trice currently does not support `%lc` and `%ls`. Proper support would require extra target-side type or encoding information so that the host can decode the transported data in an unambiguous and portable way.

### 10.5. <a id="the-special-n-conversion-specifier"></a>The special `%n` conversion specifier

The `%n` specifier is fundamentally different from ordinary `printf` conversions.

Most conversions produce output:

```c
printf("%d", value);
printf("%s", text);
printf("%f", number);
```

The `%n` specifier prints nothing. Instead, it writes the number of characters printed so far into the object pointed to by the corresponding argument.

Example:

```c
int count = 0;

printf("abc%nxyz", &count);
```

The visible output is:

```text
abcxyz
```

After the call, `count` contains `3`, because three characters were printed before `%n` was reached.

Depending on the length modifier, `%n` expects different pointer types:

```c
printf("%n",   &i);   // int *
printf("%hn",  &s);   // short *
printf("%hhn", &c);   // signed char *
printf("%ln",  &l);   // long *
printf("%lln", &ll);  // long long *
```

This means `%n` is not a pure logging conversion. It has a side effect because it writes to memory.

### 10.6. <a id="security-implications-of-n"></a>Security implications of `%n`

The `%n` specifier is also relevant for format-string security.

This is unsafe when `userInput` is not trusted:

```c
printf(userInput);
```

If `userInput` contains format specifiers, `printf` interprets them. If it contains `%n`, `printf` expects a pointer argument and writes through it. If no valid pointer was actually passed, this can cause undefined behavior, memory corruption, or a crash. In more serious cases, uncontrolled format strings can become a security vulnerability.

The correct way to print uncontrolled text is:

```c
printf("%s", userInput);
```

Because `%n` writes to memory and is strongly associated with format-string vulnerabilities, many coding standards and safety-oriented code bases discourage or forbid it.

### 10.7. <a id="why-trice-does-not-support-n"></a>Why Trice does not support `%n`

Trice is a logging and tracing system. Its purpose is to transfer compact log information from the target to the host, where it is decoded into readable text.

The `%n` specifier does not fit this model for several reasons:

1. `%n` produces no log output.
2. `%n` writes to target memory through a pointer argument.
3. The written value depends on the number of characters formatted so far.
4. Trice intentionally avoids full target-side formatting in order to remain small and fast.
5. Supporting `%n` would introduce a side effect into what should be a side-effect-free logging operation.
6. `%n` has known security implications when format strings are not fully controlled.

For these reasons, Trice currently does not support `%n`.

This is intentional. Trice log statements should describe data to be logged, not modify application memory as a side effect of formatting.

## 11. <a id="utf-8-support"></a>UTF-8 Support

This is gratis, if you edit your source files containing the format strings in UTF-8:

![./ref/UTF-8Example.PNG](../ref/UTF-8Example.PNG)

The target does not even "know" about that, because it gets only the Trice IDs.

## 12. <a id="switch-the-language-without-changing-a-bit-inside-the-target-code"></a>Switch the language without changing a bit inside the target code

Once the [til.json](../../demoTIL.json) list is done the user can translate it in any language and exchanging the list switches to another language. This is nowadays a simple AI agent task.

## 13. <a id="format-tags-prototype-specifier-examples"></a>Format tags prototype specifier examples

This syntax is supported: `%[flags][width][.precision][length]`

* Because the interpretation is done inside the Trice tool written in Go these all should work:
  * `%-d`
  * `%064b`
  * `%+9.3f`
  * `%+#012.12g`
  * `%+'#012.12E`
  * `%e`
  * `%9.f`

## 14. <a id="target-macros"></a>Target Macros

Macro spelling selects the timestamp; parameter widths follow the rules above:

```C
trice("Hello world!"); // without     timestamp
Trice("Hello world!"); // with 16-bit timestamp
TRice("Hello world!"); // with 32-bit timestamp
```

See [Stamps](stamps.md) for the clock definitions and host display.

The additional macros

* `trice8`, `trice16`, `trice32`, `trice64`
* `Trice8`, `Trice16`, `Trice32`, `Trice64`
* `TRice8`, `TRice16`, `TRice32`, `TRice64`

are always usable and the number 8, 16, 32, 64 specifies the parameter width, which is equal for all values within one macro. Trice macros are partially disabled, when the value TRICE_SINGLE_MAX_SIZE is defined to be smaller than 104. For example with TRICE_SINGLE_MAX_SIZE == 8, `TRice32` can have no parameter value (4 byte Trice header, 4 byte stamp) and `trice8` can have up to 4 parameter values (4 byte Trice header, 4 byte values) That's mainly to get compiler errors rather than runtime errors.

More examples:

Trice     | Header | Stamp | max. Values  | Trice Size
-----------|--------|-------|--------------|------------
`trice8`  | 4      | 0     | 0 \*1 byte   | 4
...       | ...    | ...   | ...          | ...
`trice8`  | 4      | 0     | 12 \*1 byte  | 16
`Trice8`  | 4      | 2     | 0 \*1 byte   | 6
...       | ...    | ...   | ...          | ...
`Trice8`  | 4      | 2     | 12 \*1 byte  | 18
`TRice8`  | 4      | 4     | 0  \*1 byte  | 8
...       | ...    | ...   | ...          | ...
`TRice8`  | 4      | 4     | 12 \*1 byte  | 20
`trice16` | 4      | 0     | 2  \*2 byte  | 8
`Trice16` | 4      | 2     | 1  \*2 byte  | 8
`trice32` | 4      | 0     | 1  \*4 byte  | 8
`Trice32` | 4      | 2     | 1  \*4 byte  | 10
`TRice32` | 4      | 4     | 2  \*4 byte  | 16
`trice64` | 4      | 0     | 1  \*8 byte  | 12
`TRice64` | 4      | 4     | 1  \*8 byte  | 16
...       | ...    | ...   | ...          | ...
`TRice64` | 4      | 4     | 12  \*8 byte | 104

On smaller machines, a 16-bit default can reduce payload size; match the host default-width setting.

The fully uppercase `TRICE` macros use inline code. Because the main design aim was speed, this was the original design. Then it became clear, that several hundred of Trice macros increase the needed code amount too much and that it is better to have just a function call instead of having inline macros. If speed matters use `TRICE(id(0)`, `TRICE(Id(0)`, `TRICE(ID(0)` else use `trice(iD(0)`, `Trice(iD(0)`, `TRice(iD(0)` or mix usage as you like. The lower case macros internally use Trice like code but each is only a function call and therefore needs less space.

## 15. <a id="trice-parameter-bit-widths"></a>Trice Parameter Bit Widths

* The macros `trice`, `Trice`, `TRice` and `TRICE` use 32-bit parameter values per default. See `TRICE_DEFAULT_PARAMETER_BIT_WIDTH` inside [src/triceDefaultConfig.h](../../src/triceDefaultConfig.h) to change that.
* If for example the bit width of all trice parameters is 8-bit, it is writable as trice8 macro, reducing the transmitted byte count per parameter from 4 to 1:

  ```C
  char b[8] = {1,2,3,4,5,6,7,8};

  // 36 bytes: 4 bytes plus 32 (8 times 4) bytes payload
  trice( "%02x %02x %02x %02x %02x %02x %02x %02x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);

  // 12 bytes: 4 bytes plus 8 (8 times 1) bytes payload
  trice8( " %02x %02x %02x %02x %02x %02x %02x %02x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);

  // 12 bytes: 4 bytes plus 8 (8 times 1) bytes payload in short notation.
  triceB( "deb: %02x\n", &b, sizeof(b) );
  ```

Hint: With the default TCOBS framing 8-bit values as 32-bit parameters typically occupy only 2-bytes during transmission.

## 16. <a id="buffer-macros"></a>Buffer Macros

(Examples in [../_test/testdata/triceCheck.c](../../_test/testdata/triceCheck.c))

Macro Name                                      | Description
-------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
`triceS`  \|`TriceS`  \|`TRiceS`  \|`TRICE_S`   | Output of runtime generated 0-terminated strings.
`triceN`  \|`TriceN`  \|`TRiceN`  \|`TRICE_N`   | Is for byte buffer output as string until the specified size. It allows limiting the string size to a specific value and does not rely on a terminating 0. If for example len = 7 is given and "Hello\0World\n" is in the buffer, the byte sequence "Hello\0W" is transmitted but the trice tool probably shows only "Hello".
`triceB`  \|`TriceB`  \|`TRiceB`  \|`TRICE_B`   | Is buffer output according to the given format specifier for a default unit according to configuration (8\|16\|32\|64-bit value) - default is `#define TRICE_B TRICE8_B`.
`trice8B` \|`Trice8B` \|`TRice8B` \|`TRICE8_B`  | Is for byte buffer output according to the given format specifier for a single byte.
`trice16B`\|`Trice16B`\|`TRice16B`\|`TRICE16_B` | Is for 16-bit buffer output according to the given format specifier for a 16-bit value.
`trice32B`\|`Trice32B`\|`TRice32B`\|`TRICE32_B` | Is for 32-bit buffer output according to the given format specifier for a 32-bit value.
|                                                 | See chapter [Trice ABC - Asynchronous Broadcast Commands](abc_asynchronous_broadcast_commands.md) for the following lines.
`triceC`  \|`TriceC`  \|`TRiceC`  \|`TRICE_C`   | Is for ABC buffer output according to the given function handler for a default unit according to configuration (8\|16\|32\|64-bit value) - default is `#define TRICE_C TRICE8_C`.
`trice8C` \|`Trice8C` \|`TRice8C` \|`TRICE8_C`  | Is for ABC byte buffer output according to the given function handler.
`trice16C`\|`Trice16C`\|`TRice16C`\|`TRICE16_C` | Is for ABC 16-bit buffer output according to the given function handler for a 16-bit wide buffer.
`trice32C`\|`Trice32C`\|`TRice32C`\|`TRICE32_C` | Is for ABC 32-bit buffer output according to the given function handler for a 32-bit wide buffer.

## 17. <a id="adapting-printf-calls"></a>Adapting printf calls

### 17.1. <a id="user-code-adaptation"></a>User Code Adaptation

* Replace all strings `puts` with the string `trice`, when the string follows immediately. For runtime generated strings see `triceS`.
* Replace all strings `printf` with the string `trice`, when the format string follows immediately.
* Check for float and double format specifiers in the format strings. The appropriate parameters need to be covered with `aFloat()` or `aDouble()`. Example:

    ```c
    printf( "%d, %3.2f EUR, %g rate\n", i, price, change );
    ```

    ```c
    trice64( "%d, %3.2f EUR, %g rate\n", i, aDouble(price), aDouble(change) );
    ```

  * Because double needs 8 bytes the trice macro in this case needs to be trice64 (see <a href="printf_comparison.md#trice-parameter-bit-widths">Trice Parameter Bit Widths</a>).

* Check for string format specifiers in the format strings. Put each in a separate trice message. Example:

    ```c
    printf( "name: %16s, surname: %32s, birthday: %4u-%02u-%02u\n", n, s, y, m, d);
    ```

    ```c
    triceS( "name: %16s, ", n);
    triceS( "surname: %32s, ", s );
    trice( "birthday: %4u-%02u-%02u\n", y, m, d);
    ```

The Trice macros are designed for maximal execution speed and therefore we have to pay the price for their limited capabilities.

* Optionally add tags to get color. Example:

    ```c
    puts( "A message");
    ```

    ```c
   trice( "msg:A message");
    ```

* Add `#include "trice.h"` to all user files using trice.

## 18. <a id="special-care-demands"></a>Special Care Demands

### 18.1. <a id="more-than-12-printf-parameters"></a>More than 12 printf parameters

* Split the values across several Trice calls.
* Use triceB and its relatives

### 18.2. <a id="float-numbers"></a>Float Numbers

* surround each with `aFloat()`

### 18.3. <a id="double-numbers"></a>Double numbers

* surround each with `aDouble()` and use the `trice64` macro and relatives

### 18.4. <a id="runtime-generated-strings"></a>Runtime Generated Strings

* Each needs its own `triceS` macro, example:
  * Legacy code:

    ```C
    printf( "Entered name is %20s %30s, favorite numbers %d, %f\n", "Paul", "Luap", 42, 3.14159 );
    ```

  * Trice code:

    ```C
    name = "Paul"; triceS( "Entered name is %20s", name );
    surname = "Luap";  triceS( " %30s, ", surname );
    trice( "favorite numbers %d, %f\n", 42, aFloat(3.14159) );
    ```

The `triceS` macro is ment to be used with strings not known at compile time.

*Usage intention and recommendation:* (given by [@escherstair](https://github.com/escherstair))

```C
char runtime_string[50];
fillRuntimeStringFromSomewhere(runtime_string); // the content of runtime_string is filled at run time
triceS( "msg:This part of the string is known at compile time. This part is dynamic: %s\n", runtime_string);
```

All the string literals (i.e. compile-time known strings) should be put inside the format string.
Only the runtime strings should be used as variables in triceS macro for best performance.

## 19. <a id="utf-8-example"></a>UTF-8 example

```c
trice("Hello! 👋🙂");
int a = -4;
float x = 3.14159265f;
trice("info:π/%d is %f with the bit pattern %032b\n", a, aFloat(x / a), aFloat(x));
const char *s = "world";
triceS("msg:A runtime generated string: %s", s);
```
