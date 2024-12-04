
<!--
There are several types of trice statements. All trice statements can have an additional letter 'i'. This means **i**nside critical section. \
You can use these when it is sure not to get interrupted by other trices. If for example an interrupt contains a trice statement this can be \
an i-trice but other trices not allowed to be an i-trice, they need to be normal trices, which are protected against interruption. \
If you are not sure it is always safe to use normal trices (without ending 'i'). The i-trices are a bit faster what is not relevant in most cases because of the general speed.

* Mixed case `Trice0`, `Trice8_1`, `Trice16_1` and `Trice8_2` are so called short trice macros.\
They use internal a smaller encoding and have only a 15-bit ID size, means ID's 1-32767 are usable.\
These are the fastest trices and with them the speed limit is reached.\
![x](./ref/Trice16_1-Code.PNG)\
![x](./ref/Trice16_1i-Code.PNG)\
![x](./ref/Trice16_1i.PNG)\
The number in the blue lines is the current processor tick. For `Trice16_1i` the difference between neighbors is about 13 clocks. \
Short trices need 'id(0)' instead 'Id(0)' as important difference to normal trices. The Trice tool will handle that for you.
* Upper case `TRICE0`, `TRICE8_1`, ... `TRICE8_8`, `TRICE16_1`, ... `TRICE16_4`, `TRICE32_1`, ... `TRICE32_4`, `TRICE64_1`, `TRICE64_2` are normal trice macros. \
They insert code directly (no function call) for better performance but the drawback is the rising code amount when many trices are used.
* Lower case `trice0`, `trice8_1`, ... `trice8_8`, `trice16_1`, ... `trice16_4`, `trice32_1`, ... `trice32_4`, `trice64_1`, `trice64_2` are normal trice functions. \
The function call overhead is reasonable and the advantage is significant less code amount when many trices are used.
* For most flexibility the code for each trice function can be enabled or not inside the triceConfig.h.

-->

<!---
_### <a id='Checkthetricebinary'></a>Check the Trice binary
* Copy command trice into a path directory.
* Run inside a shell `trice check -list path/to/trice/examples/triceDemoF030R8/MDK-ARM/`[til.json](../examples/triceDemoF030R8/MDK-ARM/til.json). You should see output like this:
![](./ref/Check.PNG)

_### <a id='InstrumentatargetsourcecodeprojectHowtousetriceinyourproject'></a>Instrument a target source code project (How to use trice in your project)

Look at one of the appropriate test projects as example. In general:

* Copy [triceConfig.h](https://github.com/rokath/trice/tree/master/pkg/src/intern/triceConfig.h) and adapt to your needs.
* Make sure the [trice.h](https://github.com/rokath/trice/blob/master/pkg/src/trice.h) header file is found by your compiler.
* Include [trice.c](https://github.com/rokath/trice/blob/master/pkg/src/trice.c)

Next steps:

* Add `#include "trice.h"` to your project files where to use TRICE macros and put `TRICE0( "msg:Hello world!\n" );` after your initialization code.
* Run `trice i` at the root of your source code. Afterwards:
  * It should have changed into `TRICE0( Id(12345), "msg:Hello world!\n" );` as example. (The `12345` stays here for a 20-bit non-zero random number).
  * A file [til.json](https://github.com/rokath/trice/blob/master/til.json)  (**t**race **i**d **l**ist) should have been generated.
* Set up timer and UART interrupt and main loop in the right way. Analyze the test example projects for advice.
* For help have a look at the differences between these 2 projects:
  * `../_test/MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64` - It is just the STM32 CubeMX generated code.
  * `../_test/MDK-ARM_LL_UART_RTT0_FLEX_STM32F030R8-NUCLEO-64` - It is a copy of the above enhanced with trice check code.

* Add your compiler definitions to `trice/pkg/src/intern/triceConfigCompiler.h`
* Make a copy of `trice/pkg/src/intern/triceUART_LL_STM32.h`, rename the copy appropriate an fill these 4 functions with the hardware specific code:

```C
TRICE_INLINE uint32_t triceTxDataRegisterEmpty(void){
}

TRICE_INLINE void triceTransmitData8(uint8_t v) {
}

TRICE_INLINE void triceEnableTxEmptyInterrupt(void) {
}

TRICE_INLINE void triceDisableTxEmptyInterrupt(void) {
}
```

Quick workaround:

```C
* Leave these definitions empty:
  * triceTxDataRegisterEmpty()
  * triceEableTxEmptyInterrupt()
  * triceDisableTxEmptyInterrupt()
* Use:
  * void triceTransmitData8( uint8_t d ){
    my_putchar( (char)d); // your code
  }
  Call TxStart();TxContinue(); cyclically in sufficient long intervals like 1 ms
```

* After compiling and flashing run `trice l -port COMn -baud m` with n and m set to correct values
* Now start your device and you should see the hello world message coming from your target. In fact the hello-world string never went to the embedded device, only the ID comes from  there and the string is found in the [til.json](https://github.com/rokath/trice/blob/master/til.json) file of your project.
* If you use a legacy project containing `printf()` statements you can simply transform them to **TRICE** statements. TRICE32 will do in most cases but for better performance take **TRICE8** or **TRICE16** where possible.
* `printf(...)` statements containing string format specifier are quickly portable by using `TRICE_P(...)` but without the trice space and speed advantage. The TRICE_P() is intended only for the few dynamic strings in a ported  project.  Enable `TRICE_PRINTF_ADAPTER` increases the needed code size by a few KB.
* It could be helpful to add `trice i ...` as prebuild step into your toolchain for each file or for the project as a whole.
  This way you cannot forget the insert step, it performs automatically.
-->


<!--#### <a id='SetallIDsinadirectorytreeto0'></a>Set all IDs in a directory tree to 0

```bash
trice zero -src ./
```

![./ref/ZeroIDsExample.PNG](./ref/ZeroIDsExample.PNG)

* If you intend to integrate some existing sources into a project using [Trice ID management](#trice-id-management) options, this could be a need.
  * From v0.61.0 is possible to restore the IDs after a `trice c|z` during the next `trice i` exactly even there are seveal identical tice messages in a file. That will allow to have the IDs inside the source code only during compiliation if you configure `trice c|z` as a post compilation step.
* Calling `trice i` afterwards will assign the same IDs.
-->

<!--

The most likely case is code like that:

```C
// f1 does s.th. but don't forget about s.th. else
void f1( void ){
    Trice( "Hello" );
}

// f2 does s.th. but don't forget about s.th. else
void f2( void ){
    Trice( "Hi" );
}
```

The problem here is the tick `'` in the `f1` comment. This will cause trouble for the Trice tool internals parser. It will ignore all text until the next `'` or to the end of the file. In this case the `Trice( "Hello" );` will get no ID during `trice i` or the ID will not be removed with `trice c`.

However, ticks `'` inside Trice format strings are allowed:

```C
// f0 does s.th.
void f0( void ){
    Trice( "Alice's message" ); // The tick inside the string is ok.
}

```
 THIS SEEMS TO BE NO MORE A PROBLEM NOW.
-->


<!--
_## <a id='Usingadifferentencoding'></a>Using a different encoding

It is possible to exchange the code behind the Trice macros with a different encoding and to add an appropriate decoder to the Trice tool.
The ID assignment is adjustable with `-IDMin` and `-IDMax`.


<p align="right">(<a href="#top">back to top</a>)</p>

_## <a id='Sub-commandcheckNotimplemented'></a>Sub-command `check` (Not implemented!)

* `trice check` will check the JSON list and emit all TRICE statements inside the list once with a dataset.

_### <a id='checkswitch-datasetNotimplemented'></a>`check` switch '-dataset' (Not implemented!)

* This is a `string` switch. It has one parameter. Its default value is `position`. That means each parameter has a different value. This is useful for testing.
* The `negative` value is uses a dataset with negative values for testing.
* Running `trice check` should show your message, indicating everything is fine so far.

_## <a id='Testing'></a>Testing


_### <a id='Folderinformation'></a>Folder information

* The folders in [./_test/](../test), despite `testdata`, are helper "projects" serving as examples and for testing the target C-code located in `./src/`.

* Some folders are hardware specific implementations and some are Go packages. The Go packages can have all the same name, only the folder names are not equal.
* In each Go package a different triceConfig.h is used, this way allowing to check all modes automatically, including encryption.
* The file `./testdata/triceCheck.c.txt` is the master test pattern for all CGO tests and edited manually. It has the extension `.txt` to avoid accidentally modification by the `trice i` command.
* After editing and before executing the tests, `./updateTestData.sh` needs to be executed. It copies into `./testdate/triceCheck.c`, and a `trice i -src triceCheck.c` is executed. Than the modified `./testdata/triceCheck.c` is compiled into the test executables in the `./cgo_*` folders.
* The file `./testdata/triceCheck.c` is copied into the memory filesystem and used there to extract the expected results (//exp: comments).
* The fresh `./testdata/til.json` is used inside the memory filesystem during the tests.
* They execute `cgo.TriceCheck(i)` this way activating the target code which writes into a buffer. The buffer is copied into a FILEBUFFER inside the memory file system and the trice tool is reading it.


* In a post-compile step a `trice z` should restore the `triceCheck.c` unmodified state to be identical to `./testdata/triceCheck_EditThisFile_NotTheTriceCheckDotC.txt`.
* Unfortunately this has to be done on the os filesystem.

_### <a id='Packagespecificinformation'></a>Package specific information

* Each C function gets a Go wrapper which ist tested in appropriate test functions.
* For some reason inside the trice_test.go an 'import "C"' is not possible.
* All C-files in the packages folder referring to the trice sources this way avoiding code duplication.
* The Go functions defined in the packages are not exported. They are called by the Go test functions in this package.
* This way the package test functions are executing the trice C-code compiled with the triceConfig.h there.

_### 13.3. <a id='todo'></a>todo

* repair cgo0_tcobs
* repair cgo1_tcobs
* cgo2_tcobs: ref_cobs.c_?
* Parse "C:\repos\trice\cgo\test\cgo2_tcobs\triceCheck.c" and complete TestTriceCheck

<p align="right">(<a href="#top">back to top</a>)</p>

_## 11. <a id='Tricedemotestprojectinfo'></a>Trice demo & test project info

Many demo projects are ARMKeil IDE STM32 here but the TRICE tool is easy adaptable to 8 - 64-bit architectures.

_### 11.1. <a id='Backupfolder'></a>Backup folder

* This folder contains unmaintained legacy test projects.
* They are only for reference and need some corrections to work with the current release.
* See also [../docs/TestExamples.md](../docs/TestExamples.md)

_### 11.2. <a id='Terminalinfo'></a>Terminal info

* Open an escape sequence capable terminal in trice root `C:\repos\trice\`opr where you put it.
  * Any directory will do as well but the `til.json` file needs to be found.
  * git-bash will do or also windows-terminal from Microsoft store.
    * Under Windows the DOS or powershell could have display issues with the escaped ASCII codes. Search the internet for answers.

_### 11.3. <a id='RTTinfo'></a>RTT info

* The example projects support both, RTT and UART just for demonstration.
* RTT = Real Time Transfer is a technique developed by SEGGER for background memory access during processor runtime. This is possible for ARM cores over the JTAG or SWD interface if a debug probe is connected.
* Some STM development boards contain a debug probe you can use for the board itself or for another board.
* These are so called on-board ST-LINK debug probes. To use RTT use the `-p STLINK` switch.
  * `stRttLogger.exe` and `libusb-1.0.dll` must be in the PATH.
* You can also flash the on-board debug probe with J-LINK firmware. To use RTT use the `-p JLINK` switch.
  * `JLinkRTTLogger.exe` and `JLinkARM.dll` must be in the PATH.

_### 11.4. <a id='UART'></a>UART

* The default baud rate is 115200 for all test projects and the trice tool assumes that baud rate automatically. Use the trice tool `-baud` switch for other settings.

_### 11.5. <a id='MDK-ARMprojects'></a>MDK-ARM projects

The projects are generated with necessary library files *as reference* to keep them smaller. Therefore, the direct compilation will fail, probably. Projects should compile if you follow these steps:

* Install latest ARMKeil IDE.
  * STM32 M0 cores with up to 32 KB FLASH memory are usable directly.
  * You can get a free ST Microelectronics license with a 256 KB FLASH limitation.
* Install latest STM32CubeMX with the latest target firmware library.
  * It is free of charge.
* Open the project ioc file with STM32CubeMX and re-generate.
* Open the project with ARMKeil IDE and build.

_### 11.6. <a id='Details'></a>Details


* [MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64\ReadMe.md](MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64\ReadMe.md)
* [MDK-ARM_LL_UART_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md)
* [MDK-ARM_LL_UART_RTT0_BARE_STM32F070RB_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_BARE_STM32F070RB_NUCLEO-64/ReadMe.md)
* [MDK-ARM_LL_UART_RTT0_BARE_STM32F091_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_BARE_STM32F091_NUCLEO-64/ReadMe.md)
* [MDK-ARM_LL_UART_RTT0_ESC_STM32F030R8_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_ESC_STM32F030R8_NUCLEO-64/ReadMe.md)
* [MDK-ARM_LL_UART_RTT0_ESC_STM32F070RB_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_ESC_STM32F070RB_NUCLEO-64/ReadMe.md)
* [MDK-ARM_LL_UART_RTT0_WRAP_STM32F030R8-NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_WRAP_STM32F030R8-NUCLEO-64/ReadMe.md)
* [MDK-ARM_RTT0_BARE_STM32F0308-DISCO/ReadMe.md](MDK-ARM_RTT0_BARE_STM32F0308-DISCO/ReadMe.md)
* [MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md](MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md)
* [MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md](MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md)
* [MDK-ARM_LL_UART_WRAP_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_WRAP_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md)


<p align="right">(<a href="#top">back to top</a>)</p>

_## 15. <a id='Furtherinfo'></a>Further info

<p align="right">(<a href="#top">back to top</a>)</p>

_## 16. <a id='ThirdpartySoftware'></a>Third party Software

<p align="right">(<a href="#top">back to top</a>)</p>

_## 17. <a id='alacritty'></a>alacritty

* A fast, cross-platform, OpenGL terminal emulator

_### 17.1. <a id='goST'></a>goST

* see Segger RTT over STLINK

_### 17.2. <a id='keil.com'></a>keil.com

* Tooling info

_## microchip.com

* Planned 8 bit example

-->

<!--- ### nxp.com

* Planned NXP example


_### <a id='segger.com'></a>segger.com

* Tooling around Segger RTT, Download latest version from SEGGER web site.

_#### <a id='SEGGERdownloadedSoftware'></a>SEGGER downloaded Software

* Check in the Internet for newer versions.

_#### <a id='JLink'></a>JLink

* Download and install [J-LinkSoftwareAndDocumentationPack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack) or simply use `JLinkRTTLogger.exe` and accompanying `JLinkARM.dll` copied from default install location `C:\Program Files (x86)\SEGGER\JLink`. Both files are inside `JLinkRTTLogger.zip` You need to put to a location in \$PATH or extend \$PATH.

_#### <a id='SEGGER_RTT'></a>SEGGER_RTT

* Target code is expected inside SEGGER_RTT. This is the extracted SEGGER_RTT_V....zip.
* Optionally check for a newer version.

_#### <a id='STLinkReflash_190812.zip'></a>STLinkReflash_190812.zip

* Tool for exchanging ST-LINK and J-LINK software on STM32 evaluation boards.
  * Works not for v3 Hardware but well for v2 Hardware.
  * In case of not accepting the ST-Link firmware use [../st.com/en.stsw-link007_V2-37-26.zip](../st.com/en.stsw-link007_V2-37-26.zip) for updating the ST-Link firmware first. It could be you need to exchange the ST-Link firmware variant into the variant with mass storage.

<p align="right">(<a href="#top">back to top</a>)</p>

_## <a id='st.com'></a>st.com

* STMicroelectronics

<p align="right">(<a href="#top">back to top</a>)</p>

_## <a id='TriceVersion1.0SpecificationDraft'></a>Trice  Version 1.0 Specification (Draft)

_## <a id='TriceUserInterface-QuickStart'></a> Trice User Interface - Quick Start

-->


<!--
* With `trice zero` all IDs in the given source tree are set to 0. This gives the option afterwards to set-up a new `til.json` according to a different `-IDMethod`, `-IDMin` and `IDMax`.
-->


<!--
_#### <a id='Addinglegacysourceswithtricerefresh'></a>Adding legacy sources with `trice refresh`

> depreciated, functionality will be removed in favor for no-ids functionality

When including legacy library code in several different projects, each with its own **til.json** ID reference list you probably do not want the IDs inside the library code be changed. If you missed the option to use prober ID ranges starting your project  `trice refresh` could help:

```bash
sub-command 'r|refresh': For updating ID list from source files but does not change the source files.
        "trice refresh" will parse source tree(s) for TRICE macros, and refresh/generate the JSON list.
        This command should be run on adding source files to the project before the first time "trice insert" is called.
        If the new source files contain TRICE macros with IDs these are added to til.json if not already used.
        Already used IDs are reported, so you have the chance to remove them from til.son and then do "trice i" again.
        This way you can make sure to get the new sources unchanged in your list.
        Already used IDs are replaced by new IDs during the next "trice insert", so the old IDs in the list will survive.
        If you do not refresh the list after adding source files and perform an "trice insert" new generated IDs could be equal to
        IDs used in the added sources with the result that IDs in the added sources could get changed what you may not want.
        Using "trice i -IDMethod random" (default) makes the chance for such conflicts very low.
        The "refresh" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
```

-->

<!---

      * If there, it points to an id slice, because f could be n times in src.
      * In most cases the slice contains only one ID. Only if the same f is used several time there are several IDs in the appropriate slice. For each i in id slice check oli for a fitting file and the closest match and that this i is not yet inside li.
        * If success patch id into source and extend li.
        * If no success, that means, that all i of that file inside id slice are used, so create new id and patch id into source and extend lu and li.
        * If only i unused with different file names, assume them to be used in this moment and create a new ID.
          * This way IDs in not parsed src get the chance to get their old ID back.
          * After file renaming all f get new i, but this happens not often.
        * For example after adding sources with IDs from a different project and not executing `trice refresh` before.
  * If the found f src ID != 0, check if found id->ilu->f is identical to src f.
    * If yes check li.
      * If yes (duplicate) create new id and extend lu and li and overwrite src ID.
      * If not, extend li.
    * If src ID not in lu, it cannot be in li, extend lu & li
-->

 <!--

_### <a id='TREXTRiceEXtendableencoding'></a>TREX (*TR*ice *EX*tendable) encoding

* The extendable encoding leaves options for user specific data mixable with trice data. This allows filtering of the framed data by just checking a bit pattern.

<p align="right">(<a href="#top">back to top</a>)</p>

_### <a id='IDManagement'></a>ID Management

* The IDs inside the source code are a "dealbreaker" as [bora](https://community.memfault.com/u/bora) mentioned in his [comment](https://interrupt.memfault.com/blog/trice). In fact it is not acceptable for library code used in several projects. An improved approach could look like this:


```c
TRICE( id(0), "...", ...); // a trice without stamp
TRICE( Id(0), "...", ...); // a trice with a 16-bit stamp
TRICE( ID(0), "...", ...); // a trice with a 32-bit stamp
```
 For the implementation of the optional Trice extensions (see below), a `til.json` format extension is needed because several files are unhandy. Both `til.json` formats will be accepted in the future. -->
<!-- With [TREX](#TREXTriceextendableencoding) encoding the location information needs no transmission anymore but goes not into the `til.json` file. In the field the location information is normally useless and probably outdated. The software developer is the one, mostly interested in the location information.  When the firmware left the developer table, only the file `til.json` is of interest.-->
 <!-- Also the planned `-binaryLogfile` option is possible. See [issue #267 Add `-binaryLogfile` option](https://github.com/rokath/trice/issues/267). It allows a replay of the logs and the developer can provide the right version of the `li.json` file.-->
<!--


* [x] New Trice macros are writable without the ID, so when `trice i` is executed, a CLI switch controls the ID type selection:
  * The update switch `-stamp 32` defaults new ID´s to `ID`.
  * The update switch `-stamp 16` defaults new ID´s to `Id`.
  * The update switch `-stamp 0`  defaults new ID´s to `id`.
  * The update switch `-stamp to32` converts all `id` and `Id` to `ID`.
  * The update switch `-stamp to16` converts all `id` and `ID` to `Id`.
  * The update switch `-stamp to0`  converts all `ID` and `Id` to `id`.
* [x] `trice log`:
  * The log switch `-ttsf` is the same as `-ttsf32`.
  * There is a new log switch `ttsf16` for the 16 bit timestamps.
  * The Trice tool alignes Trice messages with different timestamp sizes.

* The Trice tool supports several encodings, old projects should be usable with newer Trice tool versions by applying the `-encoding TLE` switch.

* The [TREX (**TR**ice **EX**tendable) encoding](#TREXTriceextendableencoding) format is planned to be stable.
The with name "COBS" branded [Trice v0.48.0 encoding](./TriceMessagesEncoding.md) is not optimal concerning the generated data amount:
* See discussion [#253 Save trice COBS encoded data on target and view it later on PC](https://github.com/rokath/trice/discussions/253).
* The location information is transmitted as 16 bit file ID plus 16 bit line number. It is possible to generate during `trice insert` an additional file `li.json` containing the location information for each Trice ID avoiding the additional 4 bytes this way. But this could cause assignment issues, when the same Trice ID is used at different locations (see [https://github.com/rokath/trice/discussions/264](https://github.com/rokath/trice/discussions/264)). But it is possible to drop the option `trice i -sharedIDs`.
* The 32-bit "COBS" package descriptor is overkill for allowing user data and dropped in TREX.
* The additional padding bytes to achieve 32 bit sizes are not needed and dropped in TREX. The user could add them by himself if really needed.
* The 4 timestamp bytes in front of each Trice demand the "COBS" package descriptor. The timestamp should go inside the Trice message and be optionally smaller. That is done in TREX.

<p align="right">(<a href="#top">back to top</a>)</p>

_### <a id='Compatibility'></a>Compatibility

* There is no guaranty for compatibility with legacy target trice versions but the aim is to provide it.

* The Trice v0.48.0 user syntax will remain mainly unchanged. The letter case of the ID codes the target timestamp size. (see below)
* The as "COBS" branded legacy [v0.48.0 Trice encoding](.TriceMessageEncoding.md) will stay unchanged as an option for compatibility. But it will not be the default encoding anymore. To use newer Trice tool versions with legacy projects the CLI switch `-encoding TLE` needs to be used.
* The option `-sharedIDs` will be further available but depreciated to avoid location assignment issues.
* Legacy projects which used the option `-sharedIDs` will still work even with a `li.json` file. A several times used ID will get an assignment of one of the locations.
* The issue [#242 Add target context option](https://github.com/rokath/trice/issues/242) could get the label "wontfix". When a task ID is needed, it could be also a data value in such cases.
* The same user source files usable with the legacy Trice "COBS" encoding and the proposed additional [TREX](#TREXTriceextendableencoding) encoding. They will have 16 bit stamps instead of 32-bits if you keep the sub macros`Id(n)`.
* Exchange individually to `ID(n)` to get 32-bit timestamps back.
* Exchange individually to `id(n)` to avoid timestamps.
* User data are in separate TCOBS packages encoded. When *Trices* are accumulated in a double half buffer, their separation in TCOBS packages is possible until the first extended Trice. Because of the generally unknown extended Trice length from this point, all following *Trices* in this half buffer need to go in one TCOBS package (including optional padding bytes) what is ok. The only disadvantage with this is, that in case of a data disruption at this place, several Trice messages can get lost.

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
* The project specific `til.json` contains all IDs and during `trice i` the same IDs are used again for the same Trice statement. For new or modified **trices** new IDs a chosen and `til.json` is extended as usual.
* Identical **trices** should have different IDs for the correctness of the location information. The switch `-sharedIDs` is obsolete and depreciated.
* There is no guaranty each Trice gets its old ID back, if for example 5 identical **trices** with different IDs exist, but the probability for an exact restore can made high using the previous `li.json` file. Proposed method:
  * When `trice -u` is executed, the previous `li.json` is read into an internal `li_1.map` and `li.json` is reset to be an empty file and that is red into `li.map`.
  * The `til.json` is read into a `lu` as already done, but the reversal `flu` list format gets an ID slice assigned to each Trice.
  * **Trices** occurring only once, what are probably the most, contain an ID slice of length 1.
  * If a trice occurs for example 5 times its ID slice has length 5 containing 5 different IDs.
  * When the `trice -u` command finds a Trice with ID slice length > 1, it looks into `li_1.map` for all possible IDs and compares the location information with the actual location:
    * If no matching file name is found, a new ID is generated.
    * If file name is identical, the ID with the minimum difference to the line number is chosen if not already used.
    * Because all assigned IDs go into the `li.map` this is possible to check.
    * If all IDs in the slice with identical file name are used, a new ID is generated.
    * Of course there are cases possible, where some unwanted ID "shift" happens. But we have to consider, that first we are talking about rare identical **trices** and that such case, if, only happens once with the result, that the `til.json` file adds a bit data garbage. A `til.json` cleaning is always possible, but you loose history then.

_##### <a id='Triceformat'></a>Trice format

* Parameter data bytes start after the optional timestamp.
* N is the parameter data bytes count. Padding bytes are not counted.
* Usually N is < 127 but for buffer or string transfer N can get up to 32767 (15 bits).
* When N > 127 (s==1) `NC` is replaced by `1nnnnnnn nnnnnnnn`. C is incremented with each Trice but not transmitted when:
  * N > 127
  * extended Trice without C

<!-- #### <a id='ExtendedTricesasfutureoption'></a>Extended *Trices* as future option

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
    | Position | Encoding                              | Remarks                                       |
    |----------|---------------------------------------|-----------------------------------------------|
    | pos      | `00nniiii I D D`                      | 12 bit ID, no timestamp, 1x 16 bit data       |
    | pos      | `00nniiii I D D`                      | 12 bit ID, no timestamp, 2x 8 bit data        |
    | pos      | `00nniiii I D D D D`                  | 12 bit ID, no timestamp, 2x 16 bit data       |
    | pos      | `00nniiii I T T  D D`                 | 12 bit ID, 16 bit timestamp,1x16 bit data     |
    | pos      | `00nniiii I T T  D D D D`             | 12 bit ID, 16 bit timestamp, 1x32 bit data    |
    | pos      | `00nniidd dddddddd`                   | 2 bit ID & 1x 10 bit data                     |
    | 6        | `00nndddd dddddddd`                   | no ID, 12 bit data as a 5 and a 7 bit value   |
    | pos      | `00nndddd`                            | no ID, 4x 1 bit data                          |
    | pos      | `00nniiii I`                          | 12 bit ID, no data                            |
    | pos      | `00nniiii D D`                        | 4 bit ID, 1x 16 bit data                      |
    | pos      | `00nniiii I D D`                      | 12 bit ID, 2x 8 bit data                      |
    | pos      | `00nniiii I T T`                      | 12 bit ID, 16 bit timestamp, no data          |
    | pos      | `00nniiii D D T`                      | ...                                           |
    | 13       | `00nniiii C T T  D D D D`             | 8 bit cyle counter, 16 bit timestamp, a float |
    | pos      | `00nniiii C D D  T T T T`             | ...                                           |
    | pos      | `00nniiii I D D  T`                   | ...                                           |
    | pos      | `00nniiii tttttttt ttttdddd dddddddd` | 4 bit ID, 12 bit timestamp, 12 bit data       |
    | ...      | `00nn...`                             | ...                                           |
  * Examples:
    * `-ex0 13` means TRICEX0 = `0000iiii C T T  D D D D`.
      * Usage: `TRICEX0( "result %f\n", aFloat(v));`
    * `-ex2 6`  means TRICEX2 = `0010dddd dddddddd`.
      * Usage: `TRICEX2( "point %x,%d\n", a, b);`
  * Trice extensions without cycle counter are counted as well.
  * Each TRICEXn has its own ID space.
* Several different Trice extensions are contributive.

* ### <a id='Unknownuserdata'></a>Unknown user data

* Unknown user data are possible as part of the Trice extensions.
  * Without the `-ex0` switch, `0000...` packages are ignored as unknown user data.
  * Without the `-ex1` switch, `0001...` packages are ignored as unknown user data.
  * Without the `-ex2` switch, `0010...` packages are ignored as unknown user data.
  * Without the `-ex3` switch, `0011...` packages are ignored as unknown user data.
* So, if Trice extensions not used, all `00...` packages are ignored as unknown user data.
* Unknown user data have an unknown length. Therefore they cannot share a COBS packet with *Trices*.
* Unknown user data packets do not affect the cycle counter. The can have their own cycle counter.
-->

