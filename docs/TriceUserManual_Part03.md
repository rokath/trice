

##  34. <a id='trice-similarities-and-differences-to-printf-usage'></a>Trice Similarities and Differences to printf Usage

###  34.1. <a id='printf-like-functions'></a>Printf-like functions

 ...have a lot of things to do: Copy format string from FLASH memory into a RAM buffer and parse it for format specifiers. Also parse the variadic parameter list and convert each parameter according to its format specifier into a character sequences, what includes several divisions - costly function calls. Concatenate the parts to a new string and deliver it to the output, what often means copying again. A full-featured printf library consumes plenty space and processing time and several open source projects try to make it better in this or that way. Never ever call a printf-like function in time critical code, like an interrupt - it would crash your target in most cases.
The *trice* calls are usable inside interrupts, because they only need a few MCU clocks for execution. Porting legacy code to use it with the Trice library, means mainly to replace Printf-like function calls with `trice` function calls. See also chapter [Legacy User Code Option Print Buffer Wrapping and Framing](#legacy-user-code-option-print-buffer-wrapping-and-framing).

###  34.2. <a id='trice-ids'></a>Trice IDs

* Each Trice caries a 14-bit nuber ID as replacement for the format string.
* This ID is automatically generated (controllable) and in the source code it is the first parameter inside the Trice macro followed by the format string and optional values.
* The user can decide not to spoil the code by having the IDs permanently in its source code, by just inserting them as a pre-compile step with `trice insert` and removing them as a post-compile step with `trice clean`.
  * The Trice cache makes this invisible to the build system, allowing full translation speed.
* The format string is **not** compiled into the target code. It goes together with the ID into a project specific reference list file [til.json](https://github.com/rokath/trice/blob/main/demoTIL.json) (example).

###  34.3. <a id='trice-values-bit-width'></a>Trice values bit width

* No need to explicit express the value bit width.
* The default parameter width for the Trice macro is 32 bit. It is changeable to 8, 16 or 64-bit:
  * Adapt `TRICE_DEFAULT_PARAMETER_BIT_WIDTH` inside `triceConfig.h`. It influences ![./ref/DefaultBitWidth.PNG](./ref/DefaultBitWidth.PNG)
  * Use `-defaultTRICEBitwidth` switch during logging when changing this value.
* The macros `trice8`, `trice16`, `trice32`, `trice64` are usable too, to define the bit width explicit.
  * This leads for the smaller bit widths to less needed space and bandwidth. But when using the default package framing TCOBS, the influence is marginal because of the implicit compression.
* The fastest Trice macro execution is, when MCU bit width matches the macro bit width.
* The implicit TCOBS compression compacts the binary Trice data during the framing.

###  34.4. <a id='many-value-parameters'></a>Many value parameters

* No need to explicit express the values count.
* Up to 12 values are supported directly. Example:
  * `trice( "%p | %04x %04x %04x %04x %04x %04x %04x %04x %04x | %f\n", p, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], aFloat(x));`
  * To support more than 12 values for each Trice macro, the Trice code on target and host is straightforward extendable up to a total payload of 32764 bytes.
* Each macro can be prolonged with the used parameter count, for example `TRICE8_3` or `TRICE_2` to intense compile time checks.
  * This length code extension can be done automatically using `trice u -addParamCount`. This is not needed anymore:
* The _Trice_ tool compares the number of given format specifiers with the written parameters in a precimpile step to minimize the risk of runtime errors.
* There is no variadic values scanning during runtime. The C preprocessor does the work.

###  34.5. <a id='floating-point-values'></a>Floating Point Values

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

###  34.6. <a id='runtime-generated-0-terminated-strings-transfer-with-trices'></a>Runtime Generated 0-terminated Strings Transfer with triceS

* The `%s` format specifier is supported by the Trice macro too but needs specific treatment.
* Strings, known at compile time should be a part of a format string to reduce runtime overhead.
* Strings created at runtime, need a special `TRICE_S` (or `triceS`, `TriceS`, `TRiceS`) macro, which accepts exactly one type `%s` format specifier. Generated strings are allowed to a size of 32764 bytes each, if the configured Trice buffer size is sufficient.
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

###  34.7. <a id='runtime-generated-counted-strings-transfer-with-tricen'></a>Runtime Generated counted Strings Transfer with  triceN

* It is also possible to transfer a buffer with length n using the `TRICE_N` (or `triceN`, `TriceN`, `TRiceN`) macro.
* This becomes handy for example, when a possibly not 0-terminated string in FLASH memory needs transmission: `triceN( "msg: FLASH string is %s", addr, 16 );`
* There are also specific macros like `trice32B` or `trice16F`. Please look into [triceCheck.c](https://github.com/rokath/trice/blob/main/_test/testdata/triceCheck.c) for usage or see the following.

###  34.8. <a id='runtime-generated-buffer-transfer-with-triceb'></a>Runtime Generated Buffer Transfer with triceB

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

###  34.9. <a id='remote-function-call-syntax-support-with-tricef'></a>Remote function call syntax support with triceF

The `TRICE8_F`, `TRICE16_F`, `TRICE32_F`, `TRICE64_F`, macros expect a string without format specifiers which is usable later as a function call. Examples:

```C
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

The Trice tool displays the parameter buffer in the shown manner. There is a [FunctionPointerList Generator](#generating-a-rpc-function-pointer-list), which generates mainly a function pointer list with associated IDs. This list can get part of the source code of a remote device. Then, when receiving a Trice message, the remote device can execute the assigned function call using the transferred parameters. This way several devices can communicate in an easy and reliable way.

With `#define TRICE_F TRICE16_F` in the project specific _triceConfig.h_ file the user can specify which should be the bitwidth (16 in this example) for `triceF` macros. The default value is 8.

**Hint:** If you add for example `"rpc"` as [tag](#explpore-and-modify-tags-and-their-colors) and call `trice log -ban "rpc"`, the Trice tool will not display the RPC Trices, but all others. That could be helpful, if you have frequent RPCs and do not wish to spoil your log output with them.

* Future extensions are possible:
  * `triceD( "dump:32", addr, 160 );` -> The Trice tool dumps in 32 byte rows.
  * An appropriate syntax is needed.

###  34.10. <a id='extended-format-specifier-possibilities'></a>Extended format specifier possibilities

* Because the format string is interpreted by the Trice tool written in [Go](https://en.wikipedia.org/wiki/Go_(programming_language)), the **Go** capabilities partial usable.

####  34.10.1. <a id='trice-format-specifier'></a>Trice format specifier

* The Trice macros are used in **C** code.
* The format strings are interpreted by the Trice tool, which is written in **Go**.
* The **C** and **Go** format specifier are not equal but similar.
* Therefore, a **T**rice adaption is internally performed.

####  34.10.2. <a id='overview-table'></a>Overview Table

| Format Specifier Type                                           | C | Go | T | (T =Trice) \| remark                                                        |
|-----------------------------------------------------------------|---|----|---|-----------------------------------------------------------------------------|
| signed decimal integer                                          | d | d  | d | Supported.                                                                  |
| unsigned decimal integer                                        | u | -  | u | The Trice tool changes %u into %d and treats value as unsigned.             |
| signed decimal integer                                          | i | d  | i | The Trice tool changes %i into %d and treats value as signed.               |
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
| a string                                                        | s | s  | s | Use `triceS` macro with one and only one runtime generated string.          |
| pointer address                                                 | p | p  | p | Supported.                                                                  |
| a double %% prints a single %                                   | % | %  | % | Supported.                                                                  |
| Unicode escape sequence                                         | - | U  | - | **Not supported.**                                                          |
| value in default format                                         | - | v  | - | **Not supported.**                                                          |
| Go-syntax representation of the value                           | - | #v | - | **Not supported.**                                                          |
| a Go-syntax representation of the type of the value             | - | T  | - | **Not supported.**                                                          |
| nothing printed                                                 | n | -  | - | **Not supported.**                                                          |

* [x] Long story short: Use the `-unsigned=false` switch when you like to see hex numbers and the like as signed values.
* [x] Look in [triceCheck.c](https://github.com/rokath/trice/blob/main/_test/testdata/triceCheck.c) for exampe code producing this:

![./ref/TriceCheckOutput.gif](./ref/TriceCheckOutput.gif)

###  34.11. <a id='utf-8-support'></a>UTF-8 Support

This is gratis, if you edit your source files containing the format strings in UTF-8:

![./ref/UTF-8Example.PNG](./ref/UTF-8Example.PNG)

The target does not even "know" about that, because it gets only the Trice IDs.

###  34.12. <a id='switch-the-language-without-changing-a-bit-inside-the-target-code'></a>Switch the language without changing a bit inside the target code

Once the [til.json](https://github.com/rokath/trice/blob/main/demoTIL.json) list is done the user can translate it in any language and exchanging the list switches to another language.

###  34.13. <a id='format-tags-prototype-specifier-examples'></a>Format tags prototype specifier examples

This syntax is supported: `%[flags][width][.precision][length]`

* Because the interpretation is done inside the Trice tool written in Go these all should work:
  * `%-d`
  * `%064b`
  * `%+9.3f`
  * `%+#012.12g`
  * `%+'#012.12E`
  * `%e`
  * `%9.f`

<p align="right">(<a href="#top">back to top</a>)</p>

##  35. <a id='development-environment-setup'></a>Development Environment Setup

* Trice is usable with any C-compiler for any processor type, bit width and endianness. The example projects here are STM32 ones but illustrate how to setup Trice.
* The [examples](https://github.com/rokath/trice/blob/main/examples) folder contains some instrumented example projects together with bare counterparts. Comparing a bare project with its intrumented counterpart gives a quick overview what needs to be done to get started.

###  35.1. <a id='common-information-1'></a>Common Information

- All used tools are **Open Source** (despite the [ARM-Keil µVision IDE](https://www2.keil.com/mdk5/uvision/), for new projects vsCode is a better choice).
- All provided information is just as example and needs adaption to your needs.
- There is no need to setup the environment in the given order.

###  35.2. <a id='important-to-know'></a>Important to know

The [ARM-Keil µVision IDE](https://www2.keil.com/mdk5/uvision/) does sometimes not recognize external file modifications. That means for example: After editing `main.c` by adding a `trice( "Hi!\n" )` and executing `trice insert` as pre-compile step it could happen, that an updated `trice( iD(12345), "Hi!\n" )`  was inserted and correct compiled but the update in `main.c` is not shown. Simply close and reopen `main.c` before editing again. This seems to be a [ARM-Keil µVision IDE](https://www2.keil.com/mdk5/uvision/) "feature" or be caused Windows not signaling a file change.

###  35.3. <a id='animation'></a>Animation

(The trice IDs occur just during the compilation.)

  <img src="./ref/Animation.gif" width="1200">

###  35.4. <a id='setup-linux-pc---example-with-debian12---kde-desktop'></a>Setup Linux PC - Example with Debian12 - KDE Desktop

####  35.4.1. <a id='basic-setup'></a>Basic setup

* Add yourself to the sudo group:

```bash
su
apt install sudo
adduser <your_user_name> sudo
exit
```

* Logout and login.
* Install and verify:

```bash
groups
sudo apt update
sudo apt upgrade
sudo apt install build-essential
make --version
gcc --version
git --version
git config --global user.email "you@example.com"
git config --global user.name "Your Name"
```

####  35.4.2. <a id='github'></a>Github

* Create github account.
* Create ssh pair:

    ```bash
    ssh-keygen -t ed25519
    ```

* Add ssh key to your github account.
* Clone Trice repository:

    ```bash
    cd ~
    mkdir repos
    cd repos
    git clone git@github.com:rokath/trice.git
    ```

####  35.4.3. <a id='vscode'></a>vsCode

* Download vsCode from https://code.visualstudio.com/download.
* Install vsCode (adapt to downloaded version) and start it inside the Trice folder:

    ```bash
    sudo apt update
    sudo apt upgrade
    sudo apt install ~/Downloads/code_1.96.2-1734607745_amd64.deb
    code .
    ```

####  35.4.4. <a id='go'></a>Go

* Download the **Go** language from https://go.dev/doc/install and install:

    ```bash
    cd ~/Downloads
    sudo rm -rf /usr/local/go && sudo tar -C /usr/local -xzf go1.23.4.linux-amd64.tar.gz
    ```

    Extend PATH variable with `/usr/local/go/bin:~/go/bin` for example by by adding a file like `/etc/profile.d/gopath.sh`:

    ```bash
    su
    sudo echo export PATH='$PATH':/usr/local/go/bin:/home/<your_user_name>/go/bin > /etc/profile.d/gopath.sh
    exit
    ```

* Logout, login and compile Trice:

    ```bash
    th@P51-DebianKDE:~/repos$ go version
    go version go1.23.4 linux/amd64
    th@P51-DebianKDE:~/repos$ cd trice
    th@P51-DebianKDE:~/repos/trice$ go install ./cmd/...
    go: downloading github.com/spf13/afero v1.9.5
    go: downloading github.com/kr/pretty v0.1.0
    go: downloading go.bug.st/serial v1.6.0
    go: downloading github.com/mgutz/ansi v0.0.0-20200706080929-d51e80ef957d
    go: downloading github.com/rokath/cobs v0.0.0-20230425030040-4ebbe9b903b9
    go: downloading github.com/rokath/tcobs v0.9.1
    go: downloading golang.org/x/crypto v0.31.0
    go: downloading github.com/fsnotify/fsnotify v1.6.0
    go: downloading github.com/pkg/errors v0.9.1
    go: downloading golang.org/x/sys v0.28.0
    go: downloading golang.org/x/text v0.21.0
    go: downloading github.com/kr/text v0.1.0
    go: downloading github.com/mattn/go-colorable v0.1.13
    go: downloading github.com/creack/goselect v0.1.2
    go: downloading github.com/mattn/go-isatty v0.0.19
    th@P51-DebianKDE:~/repos/trice$ trice version
    version=devel, built 2025-01-04 16:29:30.51921408 +0100 CET
    th@P51-DebianKDE:~/repos/trice$ go test ./...
    go: downloading github.com/tj/assert v0.0.3
    go: downloading github.com/stretchr/testify v1.8.4
    go: downloading github.com/udhos/equalfile v0.3.0
    go: downloading github.com/pmezard/go-difflib v1.0.0
    go: downloading gopkg.in/yaml.v3 v3.0.1
    go: downloading github.com/davecgh/go-spew v1.1.1
    ?       github.com/rokath/trice/internal/do     [no test files]
    ?       github.com/rokath/trice/internal/translator     [no test files]
    ?       github.com/rokath/trice/pkg/ant [no test files]
    ok      github.com/rokath/trice/cmd/trice       1.014s
    ok      github.com/rokath/trice/internal/args   0.009s
    ok      github.com/rokath/trice/internal/charDecoder    0.005s
    ok      github.com/rokath/trice/internal/com    0.005s
    ok      github.com/rokath/trice/internal/decoder        0.005s
    ok      github.com/rokath/trice/internal/dumpDecoder    0.006s
    ok      github.com/rokath/trice/internal/emitter        0.006s
    ok      github.com/rokath/trice/internal/id     2.744s
    ok      github.com/rokath/trice/internal/keybcmd        0.006s
    ok      github.com/rokath/trice/internal/link   0.006s
    ok      github.com/rokath/trice/internal/receiver       0.007s
    ok      github.com/rokath/trice/internal/trexDecoder    0.008s
    ok      github.com/rokath/trice/pkg/cipher      0.006s
    ok      github.com/rokath/trice/pkg/endian      0.002s
    ok      github.com/rokath/trice/pkg/msg 0.005s
    ok      github.com/rokath/trice/pkg/tst 0.003s
    th@P51-DebianKDE:~/repos/trice$ 
    th@P51-DebianKDE:~/repos/trice$ gcc --version
    gcc (Debian 12.2.0-14) 12.2.0
    Copyright (C) 2022 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    th@P51-DebianKDE:~/repos/trice$ ./testAll.sh 
    Sa 4. Jan 16:33:57 CET 2025
    This can take several minutes ...
    ?       github.com/rokath/trice/internal/do     [no test files]
    ok      github.com/rokath/trice/cmd/trice       1.013s
    ok      github.com/rokath/trice/internal/args   0.007s
    ok      github.com/rokath/trice/internal/charDecoder    0.004s
    ok      github.com/rokath/trice/internal/com    0.004s
    ok      github.com/rokath/trice/internal/decoder        0.003s
    ok      github.com/rokath/trice/internal/dumpDecoder    0.004s
    ok      github.com/rokath/trice/internal/emitter        0.003s
    ?       github.com/rokath/trice/internal/translator     [no test files]
    ?       github.com/rokath/trice/pkg/ant [no test files]
    ok      github.com/rokath/trice/internal/id     2.742s
    ok      github.com/rokath/trice/internal/keybcmd        0.004s
    ok      github.com/rokath/trice/internal/link   0.004s
    ok      github.com/rokath/trice/internal/receiver       0.005s
    ok      github.com/rokath/trice/internal/trexDecoder    0.005s
    ok      github.com/rokath/trice/pkg/cipher      0.004s
    ok      github.com/rokath/trice/pkg/endian      0.002s
    ok      github.com/rokath/trice/pkg/msg 0.004s
    ok      github.com/rokath/trice/pkg/tst 0.004s
    ok      github.com/rokath/trice/_test/be_dblB_de_tcobs_ua       144.000s
    ok      github.com/rokath/trice/_test/be_staticB_di_xtea_cobs_rtt32     143.927s
    ok      github.com/rokath/trice/_test/dblB_de_cobs_ua   144.006s
    ok      github.com/rokath/trice/_test/dblB_de_multi_cobs_ua     143.824s
    ok      github.com/rokath/trice/_test/dblB_de_multi_nopf_ua     144.087s
    ok      github.com/rokath/trice/_test/dblB_de_multi_tcobs_ua    144.100s
    ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_cobs_ua        143.928s
    ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_tcobs_ua       144.034s
    ok      github.com/rokath/trice/_test/dblB_de_nopf_ua   142.399s
    ok      github.com/rokath/trice/_test/dblB_de_tcobs_ua  142.520s
    ok      github.com/rokath/trice/_test/dblB_de_xtea_cobs_ua      142.526s
    ok      github.com/rokath/trice/_test/dblB_de_xtea_tcobs_ua     142.246s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_cobs_ua    281.643s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_multi_cobs_ua      281.201s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_multi_tcobs_ua     281.518s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_tcobs_ua   281.536s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_xtea_cobs_ua       279.814s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_cobs_ua     280.173s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_multi_cobs_ua       280.095s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_multi_tcobs_ua      279.693s
    ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_tcobs_ua    291.037s
    ok      github.com/rokath/trice/_test/ringB_de_cobs_ua  140.861s
    ok      github.com/rokath/trice/_test/ringB_de_multi_tcobs_ua   140.802s
    ok      github.com/rokath/trice/_test/ringB_de_multi_xtea_cobs_ua       141.037s
    ok      github.com/rokath/trice/_test/ringB_de_multi_xtea_tcobs_ua      149.046s
    ok      github.com/rokath/trice/_test/ringB_de_nopf_ua  149.114s
    ok      github.com/rokath/trice/_test/ringB_de_tcobs_ua 149.121s
    ok      github.com/rokath/trice/_test/ringB_de_xtea_cobs_ua     149.089s
    ok      github.com/rokath/trice/_test/ringB_de_xtea_tcobs_ua    149.164s
    ok      github.com/rokath/trice/_test/ringB_di_cobs_rtt32__de_tcobs_ua  288.603s
    ok      github.com/rokath/trice/_test/ringB_di_cobs_rtt8__de_tcobs_ua   288.569s
    ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt32__de_tcobs_ua  278.409s
    ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt32__de_xtea_cobs_ua      278.493s
    ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt8__de_tcobs_ua   278.301s
    ok      github.com/rokath/trice/_test/ringB_di_tcobs_rtt32__de_tcobs_ua 278.462s
    ok      github.com/rokath/trice/_test/ringB_di_xtea_cobs_rtt32__de_xtea_cobs_ua 278.590s
    ok      github.com/rokath/trice/_test/special_for_debug 0.129s
    ok      github.com/rokath/trice/_test/special_protect_dblB_de_tcobs_ua  0.130s
    ok      github.com/rokath/trice/_test/stackB_di_nopf_aux32      139.984s
    ok      github.com/rokath/trice/_test/stackB_di_nopf_aux8       139.358s
    ok      github.com/rokath/trice/_test/stackB_di_nopf_rtt32      139.366s
    ok      github.com/rokath/trice/_test/stackB_di_nopf_rtt8       143.107s
    ok      github.com/rokath/trice/_test/stackB_di_xtea_cobs_rtt8  141.896s
    ok      github.com/rokath/trice/_test/staticB_di_nopf_aux32     141.398s
    ok      github.com/rokath/trice/_test/staticB_di_nopf_aux8      141.677s
    ok      github.com/rokath/trice/_test/staticB_di_nopf_rtt32     141.779s
    ok      github.com/rokath/trice/_test/staticB_di_nopf_rtt8      141.609s
    ok      github.com/rokath/trice/_test/staticB_di_tcobs_rtt32    141.487s
    ok      github.com/rokath/trice/_test/staticB_di_tcobs_rtt8     141.559s
    ok      github.com/rokath/trice/_test/staticB_di_xtea_cobs_rtt32        139.048s
    Script run 1300 seconds.
    th@P51-DebianKDE:~/repos/trice$ 
    ```

####  35.4.5. <a id='gitkraken-(or-other-gui-for-git)'></a>Gitkraken (or other GUI for git)

* Gitkraken download from https://www.gitkraken.com/download and Install:

  ```bash
  mv ./gitkraken-amd64.deb /tmp; sudo apt install /tmp/gitkraken-amd64.deb
  ```

####  35.4.6. <a id='arm-none-eabi-toolchain-(or-other-target-system-compiler)'></a>arm-none-eabi toolchain (or other target system compiler)

```bash
sudo apt install gcc-arm-none-eabi
sudo apt install binutils-arm-none-eabi
sudo apt install gdb-arm-none-eabi
sudo apt install openocd
arm-none-eabi-gcc --version
arm-none-eabi-gcc (15:12.2.rel1-1) 12.2.1 20221205
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
 * See installed toolchain:

  ```bash
  ls -l /usr/bin/ | grep arm-none-eabi
  -rwxr-xr-x 1 root root     1033504 Feb 28  2023 arm-none-eabi-addr2line
  -rwxr-xr-x 2 root root     1066088 Feb 28  2023 arm-none-eabi-ar
  -rwxr-xr-x 2 root root     2095024 Feb 28  2023 arm-none-eabi-as
  -rwxr-xr-x 2 root root     1514496 Dec 22  2022 arm-none-eabi-c++
  -rwxr-xr-x 1 root root     1032992 Feb 28  2023 arm-none-eabi-c++filt
  -rwxr-xr-x 1 root root     1514496 Dec 22  2022 arm-none-eabi-cpp
  -rwxr-xr-x 1 root root       43640 Feb 28  2023 arm-none-eabi-elfedit
  -rwxr-xr-x 2 root root     1514496 Dec 22  2022 arm-none-eabi-g++
  -rwxr-xr-x 2 root root     1514496 Dec 22  2022 arm-none-eabi-gcc
  -rwxr-xr-x 2 root root     1514496 Dec 22  2022 arm-none-eabi-gcc-12.2.1
  -rwxr-xr-x 1 root root       35376 Dec 22  2022 arm-none-eabi-gcc-ar
  -rwxr-xr-x 1 root root       35376 Dec 22  2022 arm-none-eabi-gcc-nm
  -rwxr-xr-x 1 root root       35376 Dec 22  2022 arm-none-eabi-gcc-ranlib
  -rwxr-xr-x 1 root root      749664 Dec 22  2022 arm-none-eabi-gcov
  -rwxr-xr-x 1 root root      585688 Dec 22  2022 arm-none-eabi-gcov-dump
  -rwxr-xr-x 1 root root      610328 Dec 22  2022 arm-none-eabi-gcov-tool
  -rwxr-xr-x 1 root root     1104256 Feb 28  2023 arm-none-eabi-gprof
  -rwxr-xr-x 4 root root     1709968 Feb 28  2023 arm-none-eabi-ld
  -rwxr-xr-x 4 root root     1709968 Feb 28  2023 arm-none-eabi-ld.bfd
  -rwxr-xr-x 1 root root    24982344 Dec 22  2022 arm-none-eabi-lto-dump
  -rwxr-xr-x 2 root root     1054720 Feb 28  2023 arm-none-eabi-nm
  -rwxr-xr-x 2 root root     1180744 Feb 28  2023 arm-none-eabi-objcopy
  -rwxr-xr-x 2 root root     1867744 Feb 28  2023 arm-none-eabi-objdump
  -rwxr-xr-x 2 root root     1066120 Feb 28  2023 arm-none-eabi-ranlib
  -rwxr-xr-x 2 root root      973400 Feb 28  2023 arm-none-eabi-readelf
  -rwxr-xr-x 1 root root     1033280 Feb 28  2023 arm-none-eabi-size
  -rwxr-xr-x 1 root root     1037504 Feb 28  2023 arm-none-eabi-strings
  -rwxr-xr-x 2 root root     1180744 Feb 28  2023 arm-none-eabi-strip
  ```

* For some reason `sudo apt install gdb-arm-none-eabi` gives the message `Note, selecting 'gdb-multiarch' instead of 'gdb-arm-none-eabi'` and *arm-none-eabi-gdb* is not installed afterwards.
* To try the newest version, download it from https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads.
  * Setup:
    * Unpack and extend $PATH at the beginning:
      
    ```bash
    cd ~/Downloads
    tar xf arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz
    su
    echo export PATH=/home/th/Downloads/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin/:'$PATH' > /etc/profile.d/arm14.2path.sh
    exit
    ```

    * Logout and login

    ```bash
    arm-none-eabi-gcc --version
    arm-none-eabi-gcc (Arm GNU Toolchain 14.2.Rel1 (Build arm-14.52)) 14.2.1 20241119
    Copyright (C) 2024 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    ```

    ```bash
    arm-none-eabi-gdb --version
    GNU gdb (Arm GNU Toolchain 14.2.Rel1 (Build arm-14.52)) 15.2.90.20241130-git
    Copyright (C) 2024 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
    This is free software: you are free to change and redistribute it.
    There is NO WARRANTY, to the extent permitted by law.
    ```

  * Remove: Delete `/etc/profile.d/arm14.2path.sh` and `~/Downloads/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi`

####  35.4.7. <a id='j-link-(if-needed)'></a>J-Link (if needed)

* Download and install from https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack

```bash
sudo apt install ~/Downloads/JLink_Linux_V812_x86_64.deb
```

* Logout & login & check:

```bash
th@P51-DebianKDE:~/Downloads$ JLinkRTTLogger -?
SEGGER J-Link RTT Logger
Compiled Dec 18 2024 15:48:21
(c) 2016-2017 SEGGER Microcontroller GmbH, www.segger.com
         Solutions for real time microcontroller applications

Default logfile path: /home/th/.config/SEGGER

------------------------------------------------------------ 

Available options:
-Device <devicename>
-If <ifname>
-Speed <speed>
-USB <SN>
-IP <SN>
-RTTAddress <RTTAddress>
-RTTSearchRanges "<Rangestart> <RangeSize>[, <Range1Start> <Range1Size>, ...]
"-RTTChannel <RTTChannel>
-JLinkScriptFile <PathToScript>
<OutFilename>

Shutting down... Done.th@P51-DebianKDE:~/Downloads$ 
```


####  35.4.8. <a id='beyond-compare-(if-no-other-diff-tool)'></a>Beyond Compare (if no other diff tool)

* Download and install from https://www.scootersoftware.com

###  35.5. <a id='setup-windows-pc-example'></a>Setup Windows PC Example

Setting up a PC is for Linux mostly straightforward but Windows PCs are more problematic. The steps shown here are just one example.

- Create folder `repos` in your home directory.
  - Clone all repositories here.
- Create `C:\bin` folder.
  - When installing toolchains, put them here then and avoid spaces in created paths.
- Add `C:\bin` to PATH variable at the beginning.
  - This allows to copy tools like `trice.exe` simply into `C:\bin`.
- Install "Git for windows" from https://git-scm.com/downloads to get the neat git bash.
  - Select the Standalone Installer. This gives you useful context menu entries in the Windows explorer.
- BTW: For managing git repositories I like https://www.gitkraken.com/. Its free of charge for open source programs.
- Install VS-Code
  - This is my favorite editor with many optional Add-Ons. It is used for debugging as well.
- Install Go if you wish to compile Go programs.
  - `go test ./...` should succeed in a terminal window.
  - When GCC is not installed you get some strange errors with `TestAll.sh`:
    ```bash
    ms@PaulPCWin11 MINGW64 ~/repos/trice (devel)
    $ ./testAll.sh
    Fri Jan 10 11:19:39 WEST 2025
    This can take several minutes ...

    ...

    ok      github.com/rokath/trice/pkg/tst 0.601s
    # github.com/rokath/trice/_test/dblB_de_cobs_ua [github.com/rokath/trice/_test/dblB_de_cobs_ua.test]
    dblB_de_cobs_ua\cgo_test.go:20:110: undefined: triceDir
    dblB_de_cobs_ua\cgo_test.go:24:2: undefined: triceLogTest
    dblB_de_cobs_ua\cgo_test.go:24:28: undefined: testLines
    # github.com/rokath/trice/_test/dblB_de_tcobs_ua [github.com/rokath/trice/_test/dblB_de_tcobs_ua.test]
    dblB_de_tcobs_ua\cgo_test.go:20:92: undefined: triceDir
    dblB_de_tcobs_ua\cgo_test.go:24:2: undefined: triceLogTest
    dblB_de_tcobs_ua\cgo_test.go:24:28: undefined: testLines
    ```
  - Download and install latest GCC then, from https://winlibs.com for example.
    - Extract to `C:\bin\mingw64` and extend PATH with `C:\bin\mingw64\bin`
    - Open terminal and execute `gcc --version` to check success.
- Setup J-Link if you use this debug probe as hardware or software (see below).
  - Install SEGGER [J-Link Software and Documentation Pack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
- Install [Make for Windows](#install-make) and add its installation bin folder location to the PATH variable.

####  35.5.1. <a id='setup-trice'></a>Setup Trice

- from inside folder `repos` clone trice repo with `git clone https://github.com/rokath/trice.git`.
- Run `go install ./cmd/trice/...` from folder `repos/trice`.

OR

- Download the latest release archive and extract.
- Put trice binary into C:\bin.
- Put trice/src into `repos` if you want access the trice library code from several projects and have it only once.
  - Alternatively copy it into your project.

####  35.5.2. <a id='setup-arm-environment-example'></a>Setup ARM Environment Example

<a id='install-make'></a><h5>Install make</h5>

* Fast lane: Go to https://sourceforge.net/projects/ezwinports/files/, download ans extract **make-4.4.1-without-guile-w32-bin.zip**. Put the *make.exe* file somewhere in you $PATH.

* OR open the Windows Powershell and:

```bash
ms@PaulPCWin11 MINGW64 ~/repos/trice/examples (devel)
$ winget install ezwinports.make
The `msstore` source requires that you view the following agreements before using.
Terms of Transaction: https://aka.ms/microsoft-store-terms-of-transaction
The source requires the current machine's 2-letter geographic region to be sent to the backend service to function properly (ex. "US").

Do you agree to all the source agreements terms?
[Y] Yes  [N] No: Y
Found ezwinports: make [ezwinports.make] Version 4.4.1
This application is licensed to you by its owner.
Microsoft is not responsible for, nor does it grant any licenses to, third-party packages.
Downloading https://downloads.sourceforge.net/project/ezwinports/make-4.4.1-without-guile-w32-bin.zip
  ██████████████████████████████   383 KB /  383 KB
Successfully verified installer hash
Extracting archive...
Successfully extracted archive
Starting package install...
Path environment variable modified; restart your shell to use the new value.
Command line alias added: "make"
Successfully installed

ms@PaulPCWin11 MINGW64 ~/repos/trice/examples (devel)
```

* Check:

```bash
$ make --version
GNU Make 4.4.1
Built for Windows32
Copyright (C) 1988-2023 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
```

<a id='install-arm-gcc'></a><h5>Install ARM GCC</h5>

- Uninstall existing ARM GCC compilers optionally.
- Check if $PATH is clean.
  - In fact you can leave it as it is.
- Check if `C_INCLUDE_PATH` is not defined.
  - It would cause issues performing Go tests with CGO.
  - The environment variable `C_INCLUDE_PATH` is needed by Clang, but is set temporarily inside the Makefile.
- Open a console.
  - `which arm-none-eabi-gcc` should return `no arm-none-eabi-gcc` if no legacy compilers installed.
- Download latest version from https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads.
- Install exactly into `C:\bin\ArmGNUToolchain` and add `C:\bin\ArmGNUToolchain\bin` it to the **beginning** of the path variable.
  - The path is temporarily extendable inside the manually edited Makefiles as well, but for debugging the Windows path is used directly.
  - The path is used directly also in generated Makefiles.
  - Extending the path variable at the beginning avoids accidentally usage of installed legacy compilers with the same name.
  - To use exactly `C:\bin\ArmGNUToolchain` as install location allows a more easy project collaboration.
  - Check path:
  ```bash
  $ echo $PATH | tr : '\n'
  ...
  /c/bin
  /c/bin/ArmGNUToolchain/bin
  /c/bin/mingw64/bin
  /c/Program Files/Go/bin
  /c/Users/ms/go/bin
  /c/Users/ms/AppData/Local/Microsoft/WinGet/Packages/ezwinports.make_Microsoft.Winget.Source_8wekyb3d8bbwe/bin
  ...
  ```

<a id='macos'></a><h5>MacOS</h5>

- In terminal `brew install arm-none-eabi-gcc`
- Restart terminal
- In teminal `arm-non-eabi-gcc --version` delivers `arm-none-eabi-gcc (GCC) 14.2.0`
- In terminal `brew install arm-none-eabi-clang`
- Restart terminal
- In teminal `clang -target arm-none-eabi --version` delivers:
    ```bash
    Apple clang version 15.0.0 (clang-1500.3.9.4)
    Target: arm-none-unknown-eabi
    Thread model: posix
    InstalledDir: /Library/Developer/CommandLineTools/usr/bin
    ```
- In terminal `brew install arm-none-eabi-gdb`
- In terminal `brew install --cask gcc-arm-embedded`
- In terminal to get objcopy:

  ```bash
  brew install binutils
  echo 'export PATH="/usr/local/opt/binutils/bin:$PATH"' >> ~/.zshrc
  source ~/.zshrc
  ```

<a id='install-arm-clang-(optional)'></a><h5>Install ARM Clang (optional)</h5>

With the ARM Clang you get quicker compilation runs and smaller images.

- You need to install ARM GCC as well to use ARM Clang.
  - ARM Clang uses the GCC libraries. For that it looks for `C_INCLUDE_PATH`.
  - ARM Clang uses the GCC debugger. For that it looks into the Windows path variable directly.
- Uninstall existing ARM clang compilers or make sure they are hidden.
- Check if $PATH is clean.
  - In fact you can leave is as it is.
- Check if `C_INCLUDE_PATH` is not defined.
  - It would cause issues performing Go tests with CGO.
- Open a console.
  - `which clang` should return `no clang`.
  - If you have other clang compilers installed, do not touch them.
- Download latest version from https://github.com/llvm/llvm-project/releases.
- Install exactly into `C:\bin\ArmClang` and do **not** add it to path variable.
  - The path is extended temporarily inside the Makefile for the compiler run.

<a id='check-project-makefile-(if-it-already-exists)'></a><h5>Check Project Makefile (if it already exists)</h5>

- The Makefile should start with these lines:

```mak
# Put ARM Clang first in path temporary to avoid compiler variants issues.
export PATH := C:\bin\ArmClang\bin:$(PATH)

# ARM Clang uses the ARM GNU toolchain libraries and finds them over C_INCLUDE_PATH.
export C_INCLUDE_PATH := C:\bin\ArmGNUToolchain\arm-none-eabi\include
```

The `C:\bin\ArmGNUToolchain\bin:` is in fact not needed, because it must be in the path anyway for debugging.

- `make version` should give output like that:

```bash
$ make version
/c/bin/ArmGNUToolchain/bin/arm-none-eabi-gcc
arm-none-eabi-gcc (Arm GNU Toolchain 12.3.Rel1 (Build arm-12.35)) 12.3.1 20230626
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/c/bin/ArmClang/bin/clang
clang version 17.0.0
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: C:\bin\ArmClang\bin
```

The paths must match with the installation locations.

####  35.5.3. <a id='setup-stm32'></a>Setup STM32

<a id='generate-base-project'></a><h5>Generate Base Project</h5>

- Install and start STM32CubeMX code generator.
- Board-Selector -> STM32G0B1KE` or `STM32L432KC` or ...
- (Auto-)Initialize with default values.
- Clock-Generation -> Change PLL *N from "X 16" to "X 32" to get 64 MHz clocks.
  - Running at max clock speed and using `WFE` instructions in wait loops is slightly more energy efficient.
- Project Manager
  - Project
    - Set Project Name
    - Select Project Location
    - Toolchain / IDE -> Select Makefile
  - Code Generator
    - Select "Copy only the necessary library files".
  - Advanced Settings
    - Switch from HAL to LL at least for UART
- Generate Code as Makefile project

<a id='update-nucleo-onboard-debugger-(other-st-evaluation-boards-too)'></a><h5>Update NUCLEO Onboard Debugger (other ST evaluation boards too)</h5>

(https://www.st.com/en/development-tools/stsw-link007.html)

This step is recommended before re-flashing with the J-Link onboard debugger software.

- Connect STM evaluation board over USB
- Start ST-Link Upgrade (trice\third_party\st.com or look for a newer version at STM.).
  - Device Connect
  - Upgrade Firmware (select version **with** mass storage option)
    - Selecting the other option, would not allow to update with the SEGGER STLinkReflash tool.
  - Close

####  35.5.4. <a id='setup-onboard-j-link-on-nucleo-(other-st-evaluation-boards-too)'></a>Setup Onboard J-Link on NUCLEO (other ST evaluation boards too)

(https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/)

Using the J-Link onboard debugger software allows parallel debugging and RTT usage.

Unfortunately this is not possible with **v3** onboard debugger hardware! But you can use a J-Link hardware instead. Also it is possible to use a v2 onboard debugger from a different evaluation board or a "Bluepill" Development Board Module with ARM Cortex M3 processor".

- Start STLinkReflash (trice\third_party\segger.com)
  - Accept and Accept
  - 1: Upgrade to J-Link
  - 0: Quit
- Download, extract & start https://github.com/rokath/trice/blob/main/third_party/segger.com/STLinkReflash_190812.zip
  - Re-Flash onboard debugger.
    - You can undo this step anytime.

####  35.5.5. <a id='setup-vs-code'></a>Setup VS-Code

- Start VS Code
  - Install Go rich language support if you want to use Go as well (not needed for ARM debugging).
  - Install "Cortex Debug" extension.
  - Open the generated project directory.
  - Click on Run and Debug.
    - Click Generate launch.json and select "Cortex Debug"
  - Open and edit .vscode/launch.json
    - change "executable" value into: "./build/STM32G0B1KE_generated.elf" (example)
  - add lines:
    - `"device": "STM32G0B1KE",` or `"STM32L432KC"` or ...
    - `"svdFile": "./STM32G0B1KE.svd",` or `"./STM32L4x2.svd"` or ...
    - `"runToMain": true`
  - Set the commas right.
- Latest SVD Files can be found here: https://www.st.com/content/st_com/en/search.html#q=svd-t=resources-page=1
- Download file `STM32G0B1.svd` from https://www.st.com/resource/en/svd/stm32G0_svd.zip (example)
  - Alternatively copy it from `"C:\ST\STM32CubeIDE_1.13.1\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.productdb.debug_2.1.0.202306151215\resources\cmsis\STMicroelectronics_CMSIS_SVD\STM32G0B1.svd"` if you have the STM32CubeIDE installed.
  - Download file `STM32L4x2.svd` from https://www.st.com/resource/en/svd/stm32l4_svd.zip (example)
- Installing the **Cortex Debug** extension allow you to debug the target code.

###  35.6. <a id='makefile-with-clang-too'></a>Makefile with Clang too

- After STM32 CubeMX code generation the Makefile was edited and spitted.
- STM32 CubeMX code generation accepts the edited Makefile, so re-generation is no issue.
  - It modifies the settings according to the changes.

###  35.7. <a id='download-locations'></a>Download Locations

####  35.7.1. <a id='clang'></a>Clang

https://releases.llvm.org/download.html -> https://github.com/llvm/llvm-project/releases/tag/llvmorg-16.0.0 (example)

####  35.7.2. <a id='gcc-1'></a>GCC

https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain -> https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads (example))

###  35.8. <a id='install-locations'></a>Install Locations

Do not use locations containing spaces, like `C:\Program Files`. Take `C:\bin` for example. This avoids trouble caused by spaces inside path names.

###  35.9. <a id='environment-variables'></a>Environment Variables

Extend the path variable:

- PATH += `C:\bin\ArmGNUToolchain\bin`
- PATH += `C:\Program Files\SEGGER\JLink`. (may be C:\Program Files\SEGGER\JLink_V812a or similar)

###  35.10. <a id='build-command'></a>Build command

- Clang: `make` or to get it faster `make -j`.
- GCC: `make GCC`.

###  35.11. <a id='run-&-debug'></a>Run & Debug

- In terminal after `make` click Run&Debug & click green triangle.

###  35.12. <a id='logging'></a>Logging

- In terminal type `make log`. This executes the command in project folder:

`trice l -p JLINK -args="-Device STM32G0B1RE -if SWD -Speed 4000 -RTTChannel 0" -pf none -ts ms -d16` (example)

  <img src="./ref/Animation.gif" width="1000">

###  35.13. <a id='setting-up-a-new-project'></a>Setting up a new project

- Copy this project folder under a new name like `myAwesomeNewProject` or name it as you like.
- Make a temporary folder `myTemp` and generate with STM CubeMX the base project.
- Copy the *.ioc file from `myTemp` to `myAwesomeNewProject` and name it to the project name.
- Compare `myTemp\Makefile` with `myAwesomeNewProject\Makefile` and overwrite/extend in `myAwesomeNewProject\Makefile` the relevant settings, mainly the filenames, include path settings and DEFINES.
- Replace all generated files in `myAwesomeNewProject` with the ones in `myTemp`
- Replace the *.svd file if the MCU is different. You can find it in the internet.
- Run `make -j8` inside `myAwesomeNewProject` to check if all is ok.
- Open the copied *ioc file inside `myAwesomeNewProject` and re-generate and re-build to check.
- Compare the relevant files like `main.c` with the starting project and edit accordingly.
- Adapt `.vscode/launch.json` to the used MCU.
- Than the awesome new project should be ready to go for development.

<p align="right">(<a href="#top">back to top</a>)</p>

##  36. <a id='example-projects-without-and-with-trice-instrumentation'></a>Example Projects without and with Trice Instrumentation

| Project Name                                                              | Description                                                                                                                                                                                                                                                                                                             |
|---------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                                           |                                                                                                                                                                                                                                                                                                                         |
| [F030_bare](https://github.com/rokath/trice/blob/main/examples/F030_bare) | This is a minimal STM32CubeMX generated Makefile project adapted to Clang and GCC. It serves as a reference for diff to [F030_inst](https://github.com/rokath/trice/blob/main/examples/F030_inst) so see quickly the needed instrumentation steps you need for your own project.                                        |
| [F030_inst](https://github.com/rokath/trice/blob/main/examples/F030_inst) | This is a minimal STM32CubeMX generated Makefile project adapted to Clang and GCC and afterward instrumented with the Trice library. Compare it with [F030_bare](https://github.com/rokath/trice/blob/main/examples/F030_bare) to see quickly how to instrument your project.                                           |
|                                                                           |                                                                                                                                                                                                                                                                                                                         |
| [G0B1_bare](https://github.com/rokath/trice/blob/main/examples/G0B1_bare) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project adapted to Clang and GCC.                                                                                                                                                                                                                             |
| [G0B1_inst](https://github.com/rokath/trice/blob/main/examples/G0B1_inst) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project adapted to Clang and GCC and afterward instrumented with the Trice library.                                                                                                                                                                           |
|                                                                           |                                                                                                                                                                                                                                                                                                                         |
| [L432_bare](https://github.com/rokath/trice/blob/main/examples/L432_bare) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project extended to compile also with Clang trying to perform minimal changes. It produces some warnings, because it is not finetuned. The [L432_inst](https://github.com/rokath/trice/blob/main/examples/L432_inst) project is then a next step performable. |
| [L432_inst](https://github.com/rokath/trice/blob/main/examples/L432_inst) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project adapted to Clang and GCC and afterward instrumented with the Trice library.                                                                                                                                                                           |
|                                                                           |                                                                                                                                                                                                                                                                                                                         |

<p align="right">(<a href="#top">back to top</a>)</p>

###  36.1. <a id='nucleo-f030r8-examples'></a>Nucleo-F030R8 Examples

<img src="https://cdn1.botland.de/67242-pdt_540/stm32-nucleo-F030r8-stm32F030r8t6-arm-cortex-m0.jpg">

####  36.1.1. <a id='f030_bare'></a>F030_bare

Folder: [../examples/F030_bare/](https://github.com/rokath/trice/blob/main/examples/F030_bare/)

This is a STMCubeMX generated project without Trice instrumentation for easy compare with [F030_inst](https://github.com/rokath/trice/blob/main/examples/F030_inst) to figure out the needed changes to set up trice.

<h6>Steps performed as potential guide:</h6>

- Install STM32CubeMX to `C:\SMT32SubeMX`.
- Select NUCLEO-F030R8 board.
- Initialize with default values.
- Optionally set system clock to 32MHz for faster target timestamps.
- Optionally set UART baud rate to 115200.
- Mantadory set UART data bits including parity to **9**.
- Enable USART2 global interrupt.
- In Project Manager *Project*:
  - Set toolchain folder location to `E:\repos\trice\examples\F030_bare\`.
  - Set project name to `F030_bare`.
  - Set toolchain / IDE to `Makefile`.
- In Project Manager *Code Generator*:
  - Select "Copy only the necessary library files".
- In Project Manager *Advanced Settings*:
  - In Driver Selector change all to *LL*.
- Generate Code
- Start vsCode and open folder F030_bare with it.
- Start a terminal and type `make`. The output should be similar to:

```bash
PS E:\repos\trice\examples\F030_bare> make -j
mkdir build
arm-none-eabi-gcc -c -mcpu=cortex-m0 -mthumb   -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=0 -DDATA_CACHE_ENABLE=0 -DSTM32F030x8 -ICore/Inc -IDrivers/STM32F0xx_HAL_Driver/Inc -IDrivers/CMSIS/Device/ST/STM32F0xx/Include -IDrivers/CMSIS/Include -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/main.d" -Wa,-a,-ad,-alms=build/main.lst Core/Src/main.c -o build/main.o

...

arm-none-eabi-gcc -x assembler-with-cpp -c -mcpu=cortex-m0 -mthumb   -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=0 -DDATA_CACHE_ENABLE=0 -DSTM32F030x8 -ICore/Inc -IDrivers/STM32F0xx_HAL_Driver/Inc -IDrivers/CMSIS/Device/ST/STM32F0xx/Include -IDrivers/CMSIS/Include -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/startup_stm32F030x8.d" startup_stm32F030x8.s -o build/startup_stm32F030x8.o
arm-none-eabi-gcc build/main.o build/stm32f0xx_it.o build/stm32f0xx_ll_gpio.o build/stm32f0xx_ll_pwr.o build/stm32f0xx_ll_exti.o build/stm32f0xx_ll_usart.o build/stm32f0xx_ll_rcc.o build/stm32f0xx_ll_dma.o build/stm32f0xx_ll_utils.o build/system_stm32f0xx.o build/sysmem.o build/syscalls.o build/startup_stm32F030x8.o  -mcpu=cortex-m0 -mthumb   -specs=nano.specs -TSTM32F030R8Tx_FLASH.ld  -lc -lm -lnosys  -Wl,-Map=build/F030_bare.map,--cref -Wl,--gc-sections -o build/F030_bare.elf
C:/bin/ArmGNUToolchain/bin/../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/bin/ld.exe: warning: build/F030_bare.elf has a LOAD segment with RWX permissions
arm-none-eabi-size build/F030_bare.elf
   text    data     bss     dec     hex filename
   2428      12    1564    4004     fa4 build/F030_bare.elf
arm-none-eabi-objcopy -O ihex build/F030_bare.elf build/F030_bare.hex
arm-none-eabi-objcopy -O binary -S build/F030_bare.elf build/F030_bare.bin
PS E:\repos\trice\examples\F030_bare>
```

- Install vsCode Cortex-Debug extension.
- Create a launch.json file inside the *.vscode* subfolder and edit it to get

```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Cortex Debug",
            "cwd": "${workspaceFolder}",
            "executable": "./build/F030_bare.elf",
            "request": "launch",
            "type": "cortex-debug",
            "runToEntryPoint": "main",
            "servertype": "jlink",
            "device": "STM32F030R8",
            "svdFile": "./STM32F030R8.svd",
            "runToMain": true

        }
    ]
}
```

- Download [STM32G030.svd](https://github.com/fullyautomated/st-svd/blob/main/STM32G030.svd) or get it from the STMCubeIDE installation folder if you want to install this Eclipse IDE as well, but IMHO you do not need it.
- You may need to extract and install the [STM32 USB drivers](https://www.st.com/en/development-tools/stsw-link009.html). You can find them also in `./third_party/st.com/en.stsw-link009_v2.0.2.zip`.
- It is assumed, that you converted the OB ST-Link to an OB J-Link already. See [Convert Evaluation Board onboard ST-Link to J-Link](#convert-evaluation-board-onboard-st-link-to-j-link) for details.
- Press the Debug-Button or "CTRL+SHIFT+D" and start debugging.

<h6>Hint</h6>

- During the code generation, the CubeMX tool did not copy `syscalls.c` and `sysmem.c` but added them to the Makefile. This seems to be a STM32CubeMX "feature".
  - You do not need these files for the example project, but you can add them manually to avoid some warnings or extend the code with:
  ```C
  __weak int _close(void) { return -1; }
  __weak int _lseek(void) { return -1; }
  __weak int _read (void) { return -1; }
  __weak int _write(void) { return -1; }
  ```

####  36.1.2. <a id='f030_inst'></a>F030_inst

Folder: [../examples/F030_inst/](https://github.com/rokath/trice/blob/main/examples/F030_inst/)

This is a working example with deferred encrypted out over UART. By uncommenting 2 lines in [triceConfig.h](https://github.com/rokath/trice/blob/main/examples/F030_inst/Core/Inc/triceConfig.h), you get also parallel direct out over RTT. For setup see [Trice over RTT](#trice-over-rtt) and adapt steps from [F030_bare](https://github.com/rokath/trice/blob/main/examples/F030_bare/).

<h6>Intrumenting:</h6>

- Extend the Makefile with the information you get from comparing the *Makefile* here and in [../F030_bare/](https://github.com/rokath/trice/blob/main/examples/F030_bare/).
- Add *build.sh* and *clean.sh*.
- Copy file *SEGGER_RTT_Conf.h* from *trice/third_party/segger.com/SEGGER_RTT_V760g.zip* to [./Core/Inc/](https://github.com/rokath/trice/blob/main/examples/F030_inst/Core/Inc/). Yu could also look for a newer version.
- Copy and adapt a file [triceConfig.h](https://github.com/rokath/trice/blob/main/examples/F030_inst/Core/Inc/triceConfig.h) to [./Core/Inc/](https://github.com/rokath/trice/blob/main/examples/F030_inst/Core/Inc/). You can choose from an other example project or one of the test folders.
- Create 2 empty files: `touch til.json li.json`inside [./](./)
- Run `build.sh`. This should build all.
- Add `#include "trice.h"` to *main.c* and to *stm32f0xx_it.c* and edit these files according to diff.
- Add to `int main( void )` some `Trice( "..." );` messages.

- Run `trice s` to determine the relevant comport.
- You can have this output:

  <img src="./ref/G0B1_2024-07-22.png" width="1000">

- The Trices with 16-bit timestamps are about 150 clocks away from each other. @32MHz this is a time of less 5 µs.

<p align="right">(<a href="#top">back to top</a>)</p>

###  36.2. <a id='nucleo-g0b1-examples'></a>Nucleo-G0B1 Examples

<img src="https://docs.zephyrproject.org/latest/_images/nucleo_g0b1re.jpg">

####  36.2.1. <a id='g0b1_bare'></a>G0B1_bare

Folder: [../examples/G0B1_bare/](https://github.com/rokath/trice/blob/main/examples/G0B1_bare/)

<a id='g0b1_gen-description'></a><h5>G0B1_bare Description</h5>

- This is a working example with CLang and also GCC.
- This is a STMCubeMX generated project. It was then manually adapted to Clang.
- It is without TRICE instrumentation for easy compare with [../G0B1_inst](https://github.com/rokath/trice/blob/main/examples/G0B1_inst) to figure out the needed changes to set up trice.

<a id='setting-up-g0b1_gen'></a><h5>Setting Up G0B1_bare</h5>

- See and adapt steps from [F030_bare](#f030_bare).
- Then add/modify the files to reach this folder layot.

####  36.2.2. <a id='g0b1_inst'></a>G0B1_inst

Folder: [../examples/G0B1_inst/](https://github.com/rokath/trice/blob/main/examples/G0B1_inst/)

This is an example with direct out without framing over RTT and deferred out in TCOBS framing over UART.

<a id='setting-up-1'></a><h5>Setting Up</h5>

- See and adapt steps from [G0B1_bare](#g0b1_bare).

<a id='instrumenting'></a><h5>Instrumenting</h5>

- The steps are similar to the steps in [F030_bare](#f030_bare).
- See comments in [triceConfig.h](https://github.com/rokath/trice/blob/main/examples/G0B1_inst/Core/Inc/triceConfig.h) and commandlines in screenshot.

<img src="./ref/2024-07-22.png" width="1000">

<p align="right">(<a href="#top">back to top</a>)</p>

###  36.3. <a id='nucleo-l432kc-examples'></a>Nucleo-L432KC Examples

<img src="https://cdn-reichelt.de/bilder/web/xxl_ws/A300/NUCLEO_L432KC_01.png" width=400>

####  36.3.1. <a id='l432_bare'></a>L432_bare

Folder: [../examples/L432_bare/](https://github.com/rokath/trice/blob/main/examples/L432_bare/)

<!-- * [NUCLEO L432 User Manual](https://github.com/rokath/trice/blob/main/../ref/dm00231744-stm32-nucleo32-boards-mb1180-stmicroelectronics.pdf) (example) -->
* This example is without Trice istrumentation and serves for comparing with [L432_inst](https://github.com/rokath/trice/blob/main/examples/L432_inst/) to see the needed instrumentation steps quickly.
* This is a STMCubeMX generated project.
* See and adapt steps from [F030_bare](https://github.com/rokath/trice/blob/main/examples/F030_bare) example.
* It was then manually adapted additionally to Clang.
* It was additionally configured for FreeRTOS.

####  36.3.2. <a id='l432_inst'></a>L432_inst

Folder: [../examples/L432_inst/](https://github.com/rokath/trice/blob/main/examples/L432_inst/)

* This is the with Trice instrumented example project [L432_bare](https://github.com/rokath/trice/blob/main/examples/L432_bare).
* It is for easy compare to figure out the needed setup changes.
* See and adapt steps in [F030_bare](https://github.com/rokath/trice/blob/main/examples/F030_bare).
* Then add/modify the files to reach this folder layout.

<h5>Build:</h5>

Run `./build.sh` for configuration 0 or `./build.sh CONFIGURATION=34` for example.

<h6>Deferred Mode for max Speed</h6>

The stamps are MCU clocks here, so `🐁 Speedy Gonzales` lasts 9 processor clocks here.

```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice_wt_devel/examples/L432_inst (devel)
$ trice l -p com8 -hs off -prefix off
      triceExamples.c    10        0_272  Hello! 👋🙂

        ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
        🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-L432KC   🎈🎈🎈🎈
        🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃


        triceConfig.h   369              CONFIGURATION == 34 - UART, no cycle counter, no critical sections.
      triceExamples.c    45              TRICE_DIRECT_OUTPUT == 0, TRICE_DEFERRED_OUTPUT == 1
      triceExamples.c    51              TRICE_DOUBLE_BUFFER, TRICE_MULTI_PACK_MODE
      triceExamples.c    60              _CYCLE == 0, _PROTECT == 0, _DIAG == 0, XTEA == 0
      triceExamples.c    61              _SINGLE_MAX_SIZE=512, _BUFFER_SIZE=580, _DEFERRED_BUFFER_SIZE=4096
      triceExamples.c    15    0,000_731 🐁 Speedy Gonzales
      triceExamples.c    16    0,000_745 🐁 Speedy Gonzales
      triceExamples.c    17    0,000_754 🐁 Speedy Gonzales
      triceExamples.c    18    0,000_763 🐁 Speedy Gonzales
      triceExamples.c    19    0,000_772 🐁 Speedy Gonzales
      triceExamples.c    20    0,000_781 🐁 Speedy Gonzales
      triceExamples.c    21    0,000_790 🐁 Speedy Gonzales
      triceExamples.c    22    0,000_799 🐁 Speedy Gonzales
      triceExamples.c    24        0_981 2.71828182845904523536 <- float number as string
      triceExamples.c    25        1_230 2.71828182845904509080 (double with more ciphers than precision)
      triceExamples.c    26        1_268 2.71828174591064453125 (float  with more ciphers than precision)
      triceExamples.c    27        1_296 2.718282 (default rounded float)
      triceExamples.c    28        1_310 A Buffer:
      triceExamples.c    29        1_348 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
      triceExamples.c    30        1_603 31372e32  31383238  34383238  34303935  35333235
      triceExamples.c    31        1_799 ARemoteFunctionName(2e32)(3137)(3238)(3138)(3238)(3438)(3935)(3430)(3235)(3533)(3633)
      triceExamples.c    32              10 times a 16 byte long Trice messages, which not all will be written because of the TRICE_PROTECT:
      triceExamples.c    34        2_072 i=44444400 aaaaaa00
      triceExamples.c    34        2_119 i=44444401 aaaaaa01
      triceExamples.c    34        2_166 i=44444402 aaaaaa02
```

<a id='"hardware"-changes'></a><h5>"Hardware" Changes</h5>

* The used evaluation board is delivered with an on-board ST-Link software for debugging.
* This was changed to an on-board J-Link software for better debugging and RTT support.
* See [Trice over RTT](#trice-over-rtt) about that.

<a id='using-rtt-with-on-board-j-link-and-jlinkrttlogger'></a><h5>Using RTT with on-board J-Link and JLinkRTTLogger</h5>

* You need to install the "J-Link Software and Documentation pack" for yout OS.
* [./Core/Inc/triceConfig.h](https://github.com/rokath/trice/blob/main/examples/L432_inst/Core/Inc/triceConfig.h) contains example Trice log commands.

<a id='using-rtt-with-on-board-j-link-and-openocd'></a><h5>Using RTT with on-board J-Link and OpenOCD</h5>

<a id='with-windows-not-possible'></a><h6>With Windows not possible</h6>

* OpenOCD does not support the installed JLink driver.
![./ref/JLinkConfig0.png](./ref/JLinkConfig0.png)
* Changing to the WinUSB buld device driver is here not supported :-(

<a id='darwin'></a><h6>Darwin (MacOS)</h6>

* See **OpenOCD with Darwin** in [Trice over RTT](#trice-over-rtt)

<a id='using-rtt-with-on-board-st-link-and-openocd'></a><h5>Using RTT with on-board ST-Link and OpenOCD</h5>

**Terminal 1:**

```bash
ms@LenovoP51Win11 MINGW64 /e/repos/trice/examples/L432_inst (devel)
$ openocd -f STLinkOpenOCD.cfg
Open On-Chip Debugger 0.12.0 (2024-09-16) [https://github.com/sysprogs/openocd]
Licensed under GNU GPL v2
libusb1 d52e355daa09f17ce64819122cb067b8a2ee0d4b
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : The selected transport took over low-level target control. The results might differ compared to plain JTAG/SWD
Info : clock speed 100 kHz
Info : STLINK V2J24M11 (API v2) VID:PID 0483:374B
Info : Target voltage: 72.811768
Info : [stm32l4x.cpu] Cortex-M4 r0p1 processor detected
Info : [stm32l4x.cpu] target has 6 breakpoints, 4 watchpoints
Info : [stm32l4x.cpu] Examination succeed
Info : [stm32l4x.cpu] starting gdb server on 3333
Info : Listening on port 3333 for gdb connections
Info : rtt: Searching for control block 'SEGGER RTT'
Info : rtt: Control block found at 0x2000145c
Info : Listening on port 9090 for rtt connections
Channels: up=1, down=3
Up-channels:
0: Terminal 1024 0
Down-channels:
0: Terminal 16 0
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
```

**Terminal2:**

```bash
ms@LenovoP51Win11 MINGW64 /e/repos/trice/examples/L432_inst (devel)
$ trice l -p TCP4 -args localhost:9090  -pf none -d16
Nov 16 20:38:12.376056  TCP4:       triceExamples.c    10        1_595  Hello! 👋🙂
Nov 16 20:38:12.376056  TCP4:
Nov 16 20:38:12.376056  TCP4:         ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
Nov 16 20:38:12.376056  TCP4:         🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-L432KC   🎈🎈🎈🎈
Nov 16 20:38:12.376056  TCP4:         🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃
Nov 16 20:38:12.376056  TCP4:
Nov 16 20:38:12.376056  TCP4:
Nov 16 20:38:13.891033  TCP4:       triceExamples.c    16       43_439 2.71828182845904523536 <- float number as string
Nov 16 20:38:14.874024  TCP4:       triceExamples.c    17       44_949 2.71828182845904509080 (double with more ciphers than precision)
Nov 16 20:38:15.692614  TCP4:       triceExamples.c    18       45_802 2.71828174591064453125 (float  with more ciphers than precision)
Nov 16 20:38:16.323665  TCP4:       triceExamples.c    19       46_536 2.718282 (default rounded float)
```

<a id='using-on-board-st-link-and-vs-code-cortex-debug-extension'></a><h5>Using On-board ST-Link and VS-Code Cortex-Debug Extension</h5>

<a id='fail'></a><h6>Fail</h6>

* [https://www.st.com/resource/en/user_manual/um2576-stm32cubeide-stlink-gdb-server-stmicroelectronics.pdf](https://www.st.com/resource/en/user_manual/um2576-stm32cubeide-stlink-gdb-server-stmicroelectronics.pdf)
* Downloaded and installed
  * [en.stm32cubeprg-win64-v2-17-0.zip]()
  * [en.st-link-server-v2-1-1.zip]()
    * PATH variable extended with `C:\Program Files (x86)\STMicroelectronics\stlink_server`
    * Copied
      * From: "C:\Program Files (x86)\STMicroelectronics\stlink_server\stlinkserver.exe"
      * To: "C:\Program Files (x86)\STMicroelectronics\stlink_server\ST-LINK_gdbserver.exe"

<a id='ok'></a><h6>OK</h6>

* Download st-util from github.com
* Unpack to `C:\bin\stlink-1.8.0-win32` and add `C:\bin\stlink-1.8.0-win32\bin` to path
* Copy `C:\bin\stlink-1.8.0-win32\Program Files (x86)\stlink` to `C:\Program Files (x86)\stlink`
* Get `C:\bin\libusb-1.0.27`
* Copy `C:\bin\libusb-1.0.27\MinGW64\dll\libusb-1.0.dll` to `C:\bin\stlink-1.8.0-win32\bin\libusb-1.0.dll`
```bash
ms@LenovoP51Win11 MINGW64 /e/repos/trice/examples/L432_inst (devel)
$ st-util.exe
st-util 1.8.0
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_056A&PID_5105\5&1140C04&0&10'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_056A&PID_5105&MI_01\6&13339912&0&0001'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_058F&PID_9540\5&1140C04&0&11'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_8087&PID_0A2B\5&1140C04&0&14'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\ROOT_HUB30\4&20F1DF2E&0&0'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_0765&PID_5010\5&1140C04&0&13'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_0483&PID_374B&MI_01\6&224DEA1D&0&0001'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_5986&PID_111C&MI_00\6&104790C2&0&0000'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_046D&PID_C534\5&1140C04&0&6'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_0483&PID_374B&MI_02\6&224DEA1D&0&0002'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_0483&PID_374B\066CFF515570514867145144'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_138A&PID_0097\72FA8C531499'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_056A&PID_5105&MI_00\6&13339912&0&0000'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_046D&PID_C534&MI_01\6&C944391&0&0001'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_046D&PID_C534&MI_00\6&C944391&0&0000'
libusb: info [get_guid] no DeviceInterfaceGUID registered for 'USB\VID_5986&PID_111C\200901010001'
2024-11-17T22:20:05 INFO common.c: STM32L41x_L42x: 48 KiB SRAM, 256 KiB flash in at least 2 KiB pages.
2024-11-17T22:20:05 INFO gdb-server.c: Listening at *:4242...
Receive signal 0. Exiting...
```
(Last line after `CTRL-C`)

<p align="right">(<a href="#top">back to top</a>)</p>

##  37. <a id='trice-generate'></a>Trice Generate

###  37.1. <a id='colors'></a>Colors

Support for finding a color style:

![generateColors.PNG](./ref/generateColors.png)

See [Check Alternatives](#check-alternatives) chapter.

###  37.2. <a id='c-code'></a>C-Code

If you intend to get the `trice log` functionality full or partially as a `tlog` C-Source and do not wish to parse the _til.json_ file, you can run `trice generate -tilH --tilC` to create a C-file with header as starting point. That could be interesting for compiling the log functionality into a small separate microcontroller board.

```C
//! \file til.c
//! ///////////////////////////////////////////////////////////////////////////

//! Trice generated code - do not edit!

#include "til.h"

//! triceFormatStringList contains all trice format strings together with id and parameter information.
//!
//! The bitWidth value is not transmitted in the binary data stream and needed for its decoding.
//! The paramCount is de-facto not needed. It is derivable from the received data, see docs/TriceUserManual.md#binary-encoding.
//! It is recommended to check if both values are matching. A negative paramCount indicates, that its value is unknown at compile time.
const triceFormatStringList_t triceFormatStringList[] = {
	/* Trice type (  extended  ) */  //  id, bitWidth, paramCount, format-string
	/*      trice (  trice32_9 ) */ { 14016,  32,  9, "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d\n" },
	/*      trice (     trice0 ) */ { 14224,  32,  0, "\n" },
	/*    trice32 (  trice32_1 ) */ { 14337,  32,  1, "msg:%u (%%u)\n" },
	/*     TRICE8 (  TRICE8_10 ) */ { 15063,   8, 10, "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n" },
	/*     Trice8 (   Trice8_9 ) */ { 15124,   8,  9, "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n" },
	/*     TRICE8 (   TRICE8_5 ) */ { 15058,   8,  5, "rd:TRICE8 %d, %d, %d, %d, %d\n" },
	/*      TRice (     TRice0 ) */ { 14885,  32,  0, "TEST:yellow+h:black\n" },
	/*    Trice64 (  Trice64_1 ) */ { 15560,  64,  1, "rd:Trice64 %d\n" },
	/*      trice (  trice32_1 ) */ { 15860,  32,  1, "rd:TRICE float %9.f (%%9.f)\n" },
...
	/*  TRICE64_0 (  TRICE64_0 ) */ { 16157,  64,  0, "w: Hello! 👋🙂 \a\n" },
	/*      TRICE (     TRICE0 ) */ { 14658,  32,  0, "interrupt:magenta+i:default+h\n" },
};

//! triceFormatStringListElements holds the compile time computed count of list elements.
const unsigned triceFormatStringListElements = sizeof(triceFormatStringList) / sizeof(triceFormatStringList_t);

```

###  37.3. <a id='c#-code'></a>C#-Code

With `trice generate -tilCS` a starting point for a C-Sharp application is generated:

```cs
//! \file til.cs 

// Trice generated code - do not edit!

// There is still a need to exchange the format specifier from C to C# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// See https://stackoverflow.com/questions/33432341/how-to-use-c-language-format-specifiers-in-c-sharp
// and https://www.codeproject.com/Articles/19274/A-printf-implementation-in-C for possible help.

namespace TriceIDList;

	public class TilItem
	{
		public TilItem(int bitWidth, int paramCount, string strg)
		{
			BitWidth = bitWidth;
			ParamCount = paramCount;
			Strg = strg;
		}

		public int BitWidth { get; init; }
		public int ParamCount { get; init; }
		public string Strg { get; init; }
	}

	//! Til contains all trice format strings together with id and parameter information.
	//!
	//! The bitWidth value is not transmitted in the binary data stream and needed for its decoding.
	//! The paramCount is de-facto not needed. It is derivable from the received data, see docs/TriceUserManual.md#binary-encoding.
	//! It is recommended to check if both values are matching. A negative paramCount indicates, that its value is unknown at compile time.
	public static class Til
	{
		public static readonly Dictionary<int, TilItem> TilList= new Dictionary<int, TilItem>
		{ /* triceType ( extended ) */ //   id,     TilItem( bitWidth, paramCount, Strg )
		/*   TRICE_12 ( TRICE32_12 )*/ { 14991, new TilItem( 32, 12, "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n" ) },
		/*      TRICE (  TRICE32_1 )*/ { 15636, new TilItem( 32,  1, "WR:write        message, SysTick is %6u\n" ) },
		/*    TRICE_S (    TRICE_S )*/ { 14178, new TilItem( 32, -1, "msg:With TRICE_S:%s\n" ) },
...
		/*    TRICE16 (  TRICE16_2 )*/ { 16056, new TilItem( 16,  2, "rd:TRICE16 %p, %p\n" ) },
    };
}
```

###  37.4. <a id='generating-a-rpc-function-pointer-list'></a>Generating a RPC Function Pointer List

When several embedded devices are going to communicate, `trice generate -rpcH -rpcC` could be helpful.

You will get 2 files similar to:

```C
//! \file tilRpc.h
//! ///////////////////////////////////////////////////////////////////////////

//! Trice generated code - do not edit!

#include <stdint.h>

typedef void (*triceRpcHandler_t)(void* buffer, int count);

typedef struct{
    int id;
    triceRpcHandler_t fn;
} triceRpc_t;

extern triceRpc_t triceRpc[];
extern int triceRpcCount;

/*  TRICE16_F */ void FunctionNameXa( int16_t* p, int cnt );
/*  TRICE32_F */ void FunctionNameYa( int32_t* p, int cnt );
/*   TRICE8_F */ void TryoutBufferFunction( int8_t* p, int cnt );
/*   TRice16F */ void FunctionNameXb( int16_t* p, int cnt );
/*   trice32F */ void FunctionNameYd( int32_t* p, int cnt );
/*    TRice8F */ void FunctionNameWb( int8_t* p, int cnt );
/*   trice64F */ void FunctionNameZd( int64_t* p, int cnt );
/*   Trice16F */ void ARemoteFunctionName( int16_t* p, int cnt );
/*    Trice8F */ void FunctionNameWc( int8_t* p, int cnt );
/*   Trice16F */ void FunctionNameXc( int16_t* p, int cnt );
/*   TRICE8_F */ void TryoutStructFunction( int8_t* p, int cnt );
/*   Trice64F */ void FunctionNameZc( int64_t* p, int cnt );
/*    trice8F */ void FunctionNameWd( int8_t* p, int cnt );
/*   TRice64F */ void FunctionNameZb( int64_t* p, int cnt );
/*  TRICE64_F */ void FunctionNameZa( int64_t* p, int cnt );
/*   trice16F */ void FunctionNameXd( int16_t* p, int cnt );
/*   TRICE8_F */ void FunctionNameWa( int8_t* p, int cnt );
/*   TRice32F */ void FunctionNameYb( int32_t* p, int cnt );
/*   Trice32F */ void FunctionNameYc( int32_t* p, int cnt );

// End of file

```

```C
//! \file tilRpc.c
//! ///////////////////////////////////////////////////////////////////////////

//! Trice generated code - do not edit!

#include <stdio.h> // needed for __attribute__((weak)) 
#include "tilRpc.h"

//! triceRpc contains all rpc IDs together with their function pointer address.
const triceRpc_t triceRpc[] = {
	/* Trice type */  //  id, function pointer
	/*    TRice8F */ { 14227, FunctionNameWb },
	/*  TRICE32_F */ { 14234, FunctionNameYa },
	/*   TRICE8_F */ { 16179, TryoutBufferFunction },
	/*   Trice16F */ { 14232, FunctionNameXc },
	/*   Trice64F */ { 14240, FunctionNameZc },
	/*   TRice64F */ { 14239, FunctionNameZb },
	/*  TRICE16_F */ { 14230, FunctionNameXa },
	/*   TRICE8_F */ { 16178, TryoutStructFunction },
	/*    Trice8F */ { 14228, FunctionNameWc },
	/*   trice16F */ { 14233, FunctionNameXd },
	/*   trice64F */ { 14241, FunctionNameZd },
	/*   trice32F */ { 14237, FunctionNameYd },
	/*   TRICE8_F */ { 14226, FunctionNameWa },
	/*   TRice16F */ { 14231, FunctionNameXb },
	/*   TRice32F */ { 14235, FunctionNameYb },
	/*   Trice16F */ { 16337, ARemoteFunctionName },
	/*    trice8F */ { 14229, FunctionNameWd },
	/*   Trice32F */ { 14236, FunctionNameYc },
	/*  TRICE64_F */ { 14238, FunctionNameZa }
};

//! triceRpcListElements holds the compile time computed count of list elements.
const unsigned triceRpcElements = sizeof(triceRpc) / sizeof(triceRpc_t);

void TryoutBufferFunction( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXc( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameZc( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameZb( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXa( int16_t* p, int cnt) __attribute__((weak)) {}
void TryoutStructFunction( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWc( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXd( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameZd( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYd( int32_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWa( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXb( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYb( int32_t* p, int cnt) __attribute__((weak)) {}
void ARemoteFunctionName( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWd( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYc( int32_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameZa( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWb( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYa( int32_t* p, int cnt) __attribute__((weak)) {}

// End of file
```

Assume a project with several devices. You can add these 2 files to all targets and if a special target should execute any functions, simply implement them. These functions on their own can execute other Trice statements to transmit results. If a client executes a RPC function this way, the request is transmitted with the Trice speed. Several target devices (servers) can receive and respond and the client can wait for the first or some of them. That server receiving and client waiting functionality is not part of the Trice library. 

<p align="right">(<a href="#top">back to top</a>)</p>

##  38. <a id='testing-the-trice-library-c-code-for-the-target'></a>Testing the Trice Library C-Code for the Target

###  38.1. <a id='general-info'></a>General info

This folder is per default named to `_test` to avoid vsCode slow down. Also, when running `go test ./...`,  the tests in the `_test` folder are excluded, because they take a long time. Run `./testAll.sh` to include them.

The main aim of these tests is to automatic compile and run the target code in different compiler switch variants avoiding manual testing this way. 

`testAll.sh quick` performs just a short test. `testAll.sh full` runs all tests. That can take hours on a Windows PC because make is executed only as single thread there for stability reasons. On Darwin and Linux systems about an hour test duration should be expected. 

* Partial tests:
  * In `./examples` you can translate all examples with `./buildAllTargets.sh`.
  * In `./examples/L432_inst` the script `all_configs_build.sh` translates many different configurations.

For the user it could be helpful to start with a `triceConfig.h`file from here and to adapt the Trice tool command line from the matching `cgo_test.go` if no close match in the `examples` folder was found.

###  38.2. <a id='how-to-run-the-tests'></a>How to run the tests

* In `_trice` folder first execute `go clean -cache` after editing C-files. Cleaning the **Go** cache is recommended, because the CGO tests keep pre-compiled files and when editing C-files, this can led to confusing results.
* Execute `./renewIDs_in_examples_and_test_folder.sh` after you edited files in the `./examples` or `_test` folder.
* To run the tests manually `cd` into `_test` and execute `trice insert -i ../demoTIL.json -li ../demoLI.json` and then `go test ./...` fom there. It is more handy to run `trice_insertIDs_in_examples_and_test_folder.sh` from the Trice root folder.
* It is convenient to run  `testAll.sh` from the Trice root folder to perform this.
* It is possible to start the tests individually, but for some the default `-timeout 30s` maybe too short.

###  38.3. <a id='tests-details'></a>Tests Details

All folders despite `testdata` are test folders and the name `tf` is used as a place holder for them in this document.

To exclude a specific folder temporary, simply rename it to start with an underscore `_tf`.

The `tf` are serving for target code testing in different configuration variants on the host machine. The file [./testdata/triceCheck.c](https://github.com/rokath/trice/blob/main/_test/testdata/triceCheck.c) is the main file for most tests and serves also as example usage.

[_test/testdata/cgoPackage.go](https://github.com/rokath/trice/blob/main/_test/testdata/cgoPackage.go) is the common main for the `generated_cgoPackage.go` files and contains the common test code. 

The folders `tf` are Go packages just for tests. They all have the same package name `cgot` and are not included into the trice tool. The different `cgot` packages are independent and could have any names. They do not see each other and are used for target code testing independently. When the tests are executed for each package, a separate test binary is build and these run parallel.

The `tf/triceConfig.h` files differ and correspondent to the `tf/cgo_test.go` files in the same folder. On test execution, the `./testdata/*.c` files are compiled into the trice test executable together with the trice sources `../src` using the `tf/triceConfig.h` file.

The individual tests collect the expected results (`//exp: result`) together with the line numbers into a slice to execute the test loop on it. The `triceLogTest` function gets the `triceLog` function as parameter.

`triceLogTest` iterates over the results slice and calls for each line the C-function `triceCheck`. Then the line specific binary data buffer is passed to the `triceLog` parameter function which "logs" the passed buffer into an actual result string which in turn is compared with the expected result.

The whole process is relatively slow because of the often passed Go - C barrier, but allows automated tests in different configuration variants in one shot.

The `testdata\cgoPackage.go` file contains a variable `testLines = n`, which limits the amount of performed trices for each test case to `n`. Changing this value will heavily influence the test duration. The value `-1` is reserved for testing all test lines.

###  38.4. <a id='how-to-add-new-test-cases'></a>How to add new test cases

- Choose a test folder similar to the intended test and copy it under a new descriptive name like `newTest`.
- Extend file `./renewIDs_in_examples_and_test_folder.sh` accordingly.
- Edit files `newTest/triceConfig.h` and `newTest/cgo_test.go` in a matching way.
- Run command `go test test/newTest/...`

###  38.5. <a id='test-internals'></a>Test Internals

The `./trice/_test/testdata/*.c` and `./trice/src/*.c` are compiled together with the actual cgot package into one singe Trice test binary, resulting in as many test binaries as there are test folders. Calling its TestFunction(s) causes the activation of the Trice statement(s) inside *triceCheck.c*. The ususally into an embedded device compiled Trice code generates a few bytes according to the configuration into a buffer. These bytes are transmitted usually in real life over a (serial) port or RTT. In the tests here, this buffer is then read out by the Trice tool handler function according to the used CLI switches and processed to a log string using the *til.json* file. This string is then compared to the expected string for the activated line.

Each `tf` is a **Go** package, which is not part of any **Go** application. They all named `cgot` and are only used independently for testing different configurations. The `tf/generated_cgoPackage.go` file is identical in all `tf`. Its master is `testdata/cgoPackage.go`. After editing the master, running the command `./renewIDs_in_examples_and_test_folder.sh` copies the master to all `tf` and renames it to `generated_cgoPackage.go`.

The test specific target code configuration is inside `tf/trice.Config.h` and the appropriate Trice tool CLI switches are in `tf/cgo_test.go`.

When running `go test ./tf`, a Trice tool test executable is build, using the Trice tool packages and the `tf` package `cgot`, and the function `TestLogs` is executed. Its internal closure `triceLog` contains the Trice tool CLI switches and is passed to the `ccgot` package function `triceLogTest` together with the number of testLines and the trice mode (`directTransfer` or `deferrerdTransfer`).

During the test, the file `triceCheck.c` is scanned for lines like

```C
break; case __LINE__: TRice( iD(3537), "info:This is a message without values and a 32-bit stamp.\n" ); //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
```

Some C-code lines contain Trice statements and comments starting with `//exp: ` followed by the expected Trice tool output for that specific line. The **Go** testfunction collects these outputs in a slice together with the line numbers. Then for each found line number the execution of the **Go** function `func triceCheck(n int)` takes part, which in turn calls the CGO compiled C-function `TriceCheck(n)`. The now activated Trice C-code writes the generated trice bytes in a between **C** and **Go** shared buffer using the C-function `TriceWriteDeviceCgo`. After returning from the **Go** function `func triceCheck(n int)` and optionally calling `TriceTransfer` in deferred mode the Trice tool `triceLog()` function converts the Trice buffer bytes to the log string and compares the result with the expected data. The between **Go** and **C** shared buffer limits the executed Trices per line to one, because they use the same buffer from the beginning. This could be done better with an increment to allow several trices in one single line.

Because each test runs a different configuration, all possible combinations are testable.

###  38.6. <a id='test-results'></a>Test Results

```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice (main)
$ ./testAll.sh
Thu, Dec 12, 2024  4:51:26 PM
This can take several minutes ...
?       github.com/rokath/trice/internal/decoder        [no test files]
?       github.com/rokath/trice/internal/do     [no test files]
?       github.com/rokath/trice/internal/translator     [no test files]
?       github.com/rokath/trice/pkg/ant [no test files]
ok      github.com/rokath/trice/cmd/trice       1.392s
ok      github.com/rokath/trice/internal/args   0.415s
ok      github.com/rokath/trice/internal/charDecoder    0.298s
ok      github.com/rokath/trice/internal/com    15.845s
ok      github.com/rokath/trice/internal/dumpDecoder    0.339s
ok      github.com/rokath/trice/internal/emitter        0.326s
ok      github.com/rokath/trice/internal/id     3.088s
ok      github.com/rokath/trice/internal/keybcmd        0.233s
ok      github.com/rokath/trice/internal/link   0.196s
ok      github.com/rokath/trice/internal/receiver       0.246s
?       github.com/rokath/trice/internal/translator     [no test files]
?       github.com/rokath/trice/pkg/ant [no test files]
ok      github.com/rokath/trice/cmd/trice       1.392s
ok      github.com/rokath/trice/internal/args   0.415s
ok      github.com/rokath/trice/internal/charDecoder    0.298s
ok      github.com/rokath/trice/internal/com    15.845s
ok      github.com/rokath/trice/internal/dumpDecoder    0.339s
ok      github.com/rokath/trice/internal/emitter        0.326s
ok      github.com/rokath/trice/internal/id     3.088s
ok      github.com/rokath/trice/internal/keybcmd        0.233s
ok      github.com/rokath/trice/internal/link   0.196s
ok      github.com/rokath/trice/internal/receiver       0.246s
ok      github.com/rokath/trice/internal/trexDecoder    0.264s
ok      github.com/rokath/trice/pkg/cipher      0.230s
ok      github.com/rokath/trice/pkg/endian      0.161s
ok      github.com/rokath/trice/internal/args   0.415s
ok      github.com/rokath/trice/internal/charDecoder    0.298s
ok      github.com/rokath/trice/internal/com    15.845s
ok      github.com/rokath/trice/internal/dumpDecoder    0.339s
ok      github.com/rokath/trice/internal/emitter        0.326s
ok      github.com/rokath/trice/internal/id     3.088s
ok      github.com/rokath/trice/internal/keybcmd        0.233s
ok      github.com/rokath/trice/internal/link   0.196s
ok      github.com/rokath/trice/internal/receiver       0.246s
ok      github.com/rokath/trice/internal/trexDecoder    0.264s
ok      github.com/rokath/trice/pkg/cipher      0.230s
ok      github.com/rokath/trice/pkg/endian      0.161s
ok      github.com/rokath/trice/internal/id     3.088s
ok      github.com/rokath/trice/internal/keybcmd        0.233s
ok      github.com/rokath/trice/internal/link   0.196s
ok      github.com/rokath/trice/internal/receiver       0.246s
ok      github.com/rokath/trice/internal/trexDecoder    0.264s
ok      github.com/rokath/trice/pkg/cipher      0.230s
ok      github.com/rokath/trice/pkg/endian      0.161s
ok      github.com/rokath/trice/pkg/msg 0.157s
ok      github.com/rokath/trice/pkg/tst 0.261s
ok      github.com/rokath/trice/_test/be_dblB_de_tcobs_ua       123.142s
ok      github.com/rokath/trice/_test/be_staticB_di_xtea_cobs_rtt32     123.159s
ok      github.com/rokath/trice/internal/trexDecoder    0.264s
ok      github.com/rokath/trice/pkg/cipher      0.230s
ok      github.com/rokath/trice/pkg/endian      0.161s
ok      github.com/rokath/trice/pkg/msg 0.157s
ok      github.com/rokath/trice/pkg/tst 0.261s
ok      github.com/rokath/trice/_test/be_dblB_de_tcobs_ua       123.142s
ok      github.com/rokath/trice/_test/be_staticB_di_xtea_cobs_rtt32     123.159s
ok      github.com/rokath/trice/pkg/msg 0.157s
ok      github.com/rokath/trice/pkg/tst 0.261s
ok      github.com/rokath/trice/_test/be_dblB_de_tcobs_ua       123.142s
ok      github.com/rokath/trice/_test/be_staticB_di_xtea_cobs_rtt32     123.159s
ok      github.com/rokath/trice/_test/dblB_de_cobs_ua   122.964s
ok      github.com/rokath/trice/_test/dblB_de_multi_cobs_ua     123.308s
ok      github.com/rokath/trice/_test/be_dblB_de_tcobs_ua       123.142s
ok      github.com/rokath/trice/_test/be_staticB_di_xtea_cobs_rtt32     123.159s
ok      github.com/rokath/trice/_test/dblB_de_cobs_ua   122.964s
ok      github.com/rokath/trice/_test/dblB_de_multi_cobs_ua     123.308s
ok      github.com/rokath/trice/_test/dblB_de_cobs_ua   122.964s
ok      github.com/rokath/trice/_test/dblB_de_multi_cobs_ua     123.308s
ok      github.com/rokath/trice/_test/dblB_de_multi_cobs_ua     123.308s
ok      github.com/rokath/trice/_test/dblB_de_multi_nopf_ua     123.244s
ok      github.com/rokath/trice/_test/dblB_de_multi_nopf_ua     123.244s
ok      github.com/rokath/trice/_test/dblB_de_multi_tcobs_ua    123.109s
ok      github.com/rokath/trice/_test/dblB_de_multi_tcobs_ua    123.109s
ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_cobs_ua        123.213s
ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_tcobs_ua       123.001s
ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_cobs_ua        123.213s
ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_tcobs_ua       123.001s
ok      github.com/rokath/trice/_test/dblB_de_nopf_ua   123.092s
ok      github.com/rokath/trice/_test/dblB_de_multi_xtea_tcobs_ua       123.001s
ok      github.com/rokath/trice/_test/dblB_de_nopf_ua   123.092s
ok      github.com/rokath/trice/_test/dblB_de_tcobs_ua  122.324s
ok      github.com/rokath/trice/_test/dblB_de_nopf_ua   123.092s
ok      github.com/rokath/trice/_test/dblB_de_tcobs_ua  122.324s
ok      github.com/rokath/trice/_test/dblB_de_tcobs_ua  122.324s
ok      github.com/rokath/trice/_test/dblB_de_xtea_cobs_ua      123.149s
ok      github.com/rokath/trice/_test/dblB_de_xtea_tcobs_ua     122.883s
ok      github.com/rokath/trice/_test/dblB_de_xtea_cobs_ua      123.149s
ok      github.com/rokath/trice/_test/dblB_de_xtea_tcobs_ua     122.883s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_cobs_ua    246.703s
ok      github.com/rokath/trice/_test/dblB_de_xtea_tcobs_ua     122.883s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_cobs_ua    246.703s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_cobs_ua    246.703s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_multi_cobs_ua      247.125s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_multi_tcobs_ua     246.862s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_tcobs_ua   246.531s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt32__de_xtea_cobs_ua       247.072s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_cobs_ua     246.639s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_multi_cobs_ua       246.599s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_multi_tcobs_ua      247.114s
ok      github.com/rokath/trice/_test/dblB_di_nopf_rtt8__de_tcobs_ua    246.851s
ok      github.com/rokath/trice/_test/ringB_de_cobs_ua  123.578s
ok      github.com/rokath/trice/_test/ringB_de_multi_tcobs_ua   123.517s
ok      github.com/rokath/trice/_test/ringB_de_multi_xtea_cobs_ua       123.497s
ok      github.com/rokath/trice/_test/ringB_de_multi_xtea_tcobs_ua      123.379s
ok      github.com/rokath/trice/_test/ringB_de_nopf_ua  123.555s
ok      github.com/rokath/trice/_test/ringB_de_tcobs_ua 123.300s
ok      github.com/rokath/trice/_test/ringB_de_xtea_cobs_ua     123.487s
ok      github.com/rokath/trice/_test/ringB_de_xtea_tcobs_ua    123.846s
ok      github.com/rokath/trice/_test/ringB_di_cobs_rtt32__de_tcobs_ua  247.400s
ok      github.com/rokath/trice/_test/ringB_di_cobs_rtt8__de_tcobs_ua   247.202s
ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt32__de_tcobs_ua  247.204s
ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt32__de_xtea_cobs_ua      246.818s
ok      github.com/rokath/trice/_test/ringB_di_nopf_rtt8__de_tcobs_ua   247.006s
ok      github.com/rokath/trice/_test/ringB_di_tcobs_rtt32__de_tcobs_ua 247.000s
ok      github.com/rokath/trice/_test/ringB_di_xtea_cobs_rtt32__de_xtea_cobs_ua 246.872s
ok      github.com/rokath/trice/_test/special_protect_dblB_de_tcobs_ua  0.444s
ok      github.com/rokath/trice/_test/stackB_di_nopf_aux32      123.819s
ok      github.com/rokath/trice/_test/stackB_di_nopf_aux8       123.830s
ok      github.com/rokath/trice/_test/stackB_di_nopf_rtt32      123.912s
ok      github.com/rokath/trice/_test/stackB_di_nopf_rtt8       123.976s
ok      github.com/rokath/trice/_test/stackB_di_xtea_cobs_rtt8  123.719s
ok      github.com/rokath/trice/_test/staticB_di_nopf_aux32     123.553s
ok      github.com/rokath/trice/_test/staticB_di_nopf_aux8      123.551s
ok      github.com/rokath/trice/_test/staticB_di_nopf_rtt32     123.596s
ok      github.com/rokath/trice/_test/staticB_di_nopf_rtt8      123.618s
ok      github.com/rokath/trice/_test/staticB_di_tcobs_rtt32    123.177s
ok      github.com/rokath/trice/_test/staticB_di_tcobs_rtt8     123.353s
ok      github.com/rokath/trice/_test/staticB_di_xtea_cobs_rtt32        123.126s

real    10m31.130s
user    0m0.000s
sys     0m0.015s

ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice (main)
$
```

###  38.7. <a id='special-tests'></a>Special tests

###  38.8. <a id='test-cases'></a>Test Cases

####  38.8.1. <a id='folder-naming-convention'></a>Folder Naming Convention

| Folder Name Part | Meaning                                                                                                  |
|:----------------:|----------------------------------------------------------------------------------------------------------|
|    `testdata`    | This is no test folder. It contains data common to all tests.                                            |
|      `_...`      | Folder starting with an undescore `_` are excluded when `go test ./...` is executed.                     |
|      `_di_`      | direct mode                                                                                              |
|      `_de_`      | deferred mode                                                                                            |
|    `special_`    | a test, not using `./testdata/triceCheck.c`                                                              |
|    `staticB_`    | static buffer, direct mode only possible                                                                 |
|    `stackB_`     | stack buffer, direct mode only possible                                                                  |
|     `ringB_`     | ring buffer, deferred mode and optional parallel direct mode                                             |
|     `dblB_`      | double buffer, deferred mode and optional parallel direct mode                                           |
|     `_rtt8_`     | (simulated) SEGGER_RTT byte transfer                                                                     |
|    `_rtt32_`     | (simulated) SEGGER_RTT word transfer                                                                     |
|       `__`       | direct and deferred mode together                                                                        |
|     `_xtea_`     | with encryption, otherwise without encryption                                                            |
|     `_tcobs`     | TCOBS package framing                                                                                    |
|     `_cobs`      | COBS package framing                                                                                     |
|     `_nopf`      | no package framing                                                                                       |
|    `_multi_`     | Usually each Trice is handled separately. In multi mode, groups of available Trices are framed together. |
|      `_ua`       | simulated UART A output (for deferred modes)                                                             |

<p align="right">(<a href="#top">back to top</a>)</p>

##  39. <a id='test-issues'></a>Test Issues

Test folders starting with `ERROR_` have issues. These cases are **usable** on the target. These tests fail for an unknown reason. Probably it is a test implementation issue. Especially when XTEA is used in one output but not in the other, the tests fail.

<p align="right">(<a href="#top">back to top</a>)</p>
