# Changelog

| date        | version | comment                                                                                                                                                                                                                                          |
|-------------|--------:|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------     |
| 2020-02-11  |     0.1 | initial version                                                                                                                                                                                                                                  |
| 2020-02-14  |     0.2 | SHORT_MEMORY switch added                                                                                                                                                                                                                        |
| 2020-02-18  |     0.3 | C macros renamed into TRICE                                                                                                                                                                                                                      |
| 2020-02-20  |     0.4 | trice printf() adapter added                                                                                                                                                                                                                     |
| 2020-02-22  |     0.5 | support for auto param count                                                                                                                                                                                                                     |
| 2020-03-01  |     0.6 | encryption support, TRICE64_2 added                                                                                                                                                                                                              |
| 2020-03-12  |     0.7 | `trice u -src dir1 -src file -src dir2` any now possible, -ts added, effective runtime strings                                                                                                                                                   |
| 2020-03-29  |     0.8 | auto-logfile, remoteDisplay added, check issues!                                                                                                                                                                                                 |
| 2020-04-07  |   0.8.2 | display server supports many receiver-clients, no line mixes, code partially restructured, line prefix                                                                                                                                           |
| 2020-04-22  |     0.9 | fileWatcher added (no need to re-load til.json anymore), now `trice s` possible for COMport scan, NUCLEO-F070 demo project added, now with ANSI color (faster), default log filename with timestamp, remote displayServer shutdown possible      |
| 2020-05-12  |   0.9.1 | `SeggerRTT` & `SeggerRTTD` added, docs extended, more test examples, TRICE16_1 critical section bugfix                                                                                                                                           |
| 2020-06-29  |   0.9.2 | CLI slightly changed (no more 'r'), doc extended, added: Tests, Examples, JLINK, RND, SIM, FILE, TRICE_SYNC, TRICE_VARIANT, TRICE_ASSERT, now no semi-colon after `TRICE` is tolerated (macros), several `TRICE` in one code line are now ok     |
| ...         |         |                                                                                                                                                                                                                                                  |
| 2022-FEB-11 |  0.48.0 | FILE & TCP4 input ports added                                                                                                                                                                                                                    |
| 2022-APR-27 |  0.49.1 | FILEBUFFER support added                                                                                                                                                                                                                         |
| 2022-APR-27 |  0.49.1 | Darwin binaries added                                                                                                                                                                                                                            |
| 2022-MAY-14 |  0.49.2 | Legacy comport Tarm driver code deactivated, TRICE_S usage clarification in [TriceUserGuide.md](./doc/TriceUserGuide.md), `\t` support inside format strings added, `TRICE` macro in one line demand added., FILEBUFFER issue fixed (workaround) |
| 2022-MAY-16 |  0.50.0 | Switch `-tcp` added allowing trice TCP output for reception with 3rd party tools.                                                                                                                                                                |
| 2022-MAY-17 |  0.51.0 | Switch `-binaryLogfile`added.                                                                                                                                                                                                                    |
| 2022-MAY-22 |  0.51.1 | Display server works again. Simple Shell added.                                                                                                                                                                                                  |
| 2022-JUN-05 |  0.52.0 | UART Command option added (MDK-ARM_STM32F030R8 example), Colors extended.                                                                                                                                                                        |
| 2022-JUN-14 |  0.53.0 | Macros added: TRICE_B, TRICE8_B, TRICE16_B, TRICE32_B, TRICE64_B, TRICE8_F, TRICE16_F, TRICE32_F, TRICE64_F.                                                                                                                                     |
| 2022-JUL-06 |  0.54.0 | Location information file `li.json` now generated with `trice u`. This makes the 4 bytes location information inside the binary data obsolete and reduces the transmitted data. You should adapt your `triceConfig.h` - disable `TRICE_LOCATION` there. |
| 2022-JUL-07 |  0.54.1 | Test data adapted |
| 2022-AUG-25 |  0.55.1 | Switching to TREX (see ./doc/TriceV1.0Specification.md) as default encoding. Legacy projects need "trice log -enc TLE" now. In project specific `triceConfig.h` file switch between `TRICE_TREX_ENCODING` and `TRICE_LEGACY_ENCODING`. For TREX framing TCOBSv1 is active. As default the `Id(n)  sub-macro uses a 16-bit timestamp, so be careful changing legacy projects. See issue #317. |
| 2023-FEB-19 |  0.56.0 | Many changes - see below |
| 2023-FEB-21 |  0.56.2 | TriceUserManual.md changed |
| 2023-FEB-21 |  0.56.3 | Target Modbus Code deactivated (anyway not usable yet) |
| 2023-MAR-05 |  0.56.4 | Maintenance release: all 3 modes work parallel over RTT0 and UARTA in MDK-ARM_STM32F030R8. |
| 2023-MAY-04 |  0.57.0 | Target code reworked. Now 4 buffer types: `STACK`,`STATIC` for direct mode only and `DOUBLE`, `RING` for deferred plus optional direct mode. A RTT 32-bit write function allows direct SEGGER_RTT output within less 100 MCU clocks (^=1.5µs@64MHz). |
| 2023-MAY-05 |  0.58.0 | Target files all now direct in src folder and need to be included all. Direct XTEA with RTT32 (experimental). |
| 2023-MAY-10 |  0.59.0 | Target code better structured. Automatic target tests working and now extendable with more configuration variants. Auxiliary support for direct and deferred. Common RTT Diagnostics for all RTT Modes as part of TRICE_DIAGNOSTICS. |


## v0.56.0 Changes

This is a major release hopefully not breaking too much!

### Legacy `TRICE*` macros

* Using the `TRICE*` macros generally is not recommended anymore, but they are still supported.
* `TRICE*` macros with an `Id(n)` inside are now with 16-bit stamps. To have 32-bit stamps use `ID(n)` instead and `id(n)` is for no stamps. The ID numbers can stay the same.
* The `TRICE*` macros are inlining the trice code, what could lead to significant memory usage if you use these MACROs heavily.
* The very little time advantage of code inlining will not count in most cases.
* The `TRICE_*` macros, like `TRICE_S` are still needed.

### Function call macros `TRice`, `Trice` and `trice`

* `TRice*` macros insert a 32-bit stamp.
* `Trice*` macros insert a 16-bit stamp.
* `trice*` macros insert no stamp.
* ATTENTION: For some reason these macros cause with CGO issues when used with parameters, but they work fine with ARM6 and ARM CLANG6 (Select the g11 option). If your compiler has issues, use the bit width extension: Write `trice32( "%u", 1)` instead of `trice( "%u", 1)` for example or stay with `TRICE*`.
* These macros are function call macros. They call internally a trice function incrementing the code amount like a usual function call.
* You can write them down like the `TRICE*` macros, but do not insert `ID(0)`, `Id(0)` or `id(0)`! After executing `trice u` they get an `iD(n)` inserted automatically.
* To save space and bandwidth you can use the `*8` and `*16`, macro extensions like with the `TRICE*` macros.
* The default bitwidth is 32-bit but that is changeable in the configuration.
* The `*64` macro extensions are usable for 64-bit values, like `double`.
* See `test\testdata\triceCheck.c` for examples.

### Common improvements

- Target time stamps now better formatted, Default is `µs`, `ms` is selectable.
- A new document TriceConfigProjectImageSizeOptimization.md was written.
- Trice messages containing several `\n` are better formatted.
- Multiple physical channels parallel usable now, like RTT and UART.
- A space after ShowID inserted, also after target stamp for better visibility.
- `triceConfig.h` was heavily restructured.
- Target code split in several files now, but only `trice.c` needs to be added to the target project.

### CLI changes

- `ts` ist now target stamp. Host stamps are switchable with `-hs`.
- `-packageFraming` is new.
- Switch `-newlineIndent` added.
- `li off` is now possible.
- `trice s` now shows if a port is used already.
- `my/path/auto` now possible with log files.
### Changes under the hood

* MemFs `afero.Afero` is used now, what is for better tests.
* A `til.json.c|h` is generated when `trice u` is called with `-v` switch.
* Trices over Modbus are prepared (not usable yet).
* Better protection against panic, when data garbage is fed to the trice tool (wrong til.json for example).
* RTT log file file access rights in Windows11 solved. They are now collected in a project temp folder (you can play them later again).
* Complete target code test over CGO.
* `cobs` & `tcobs` now external packages.
* False positive windows vet warning removed.
* Endianness updated and tested.
