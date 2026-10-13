# Trice Decoding

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Trice ID list til.json](#trice-id-list-tiljson)
* [2. Trice location information file li.json](#trice-location-information-file-lijson)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

The 14-bit IDs are used to display the log strings. These IDs are pointing in two reference files.

## 1. <a id="trice-id-list-tiljson"></a>Trice ID list til.json

* This file integrates all firmware variants and versions and is the key to display the message strings. With the latest version of this file all previous deployed firmware images are usable without the need to know the actual firmware version.
* A compact C metadata table can be generated to help write a decoder or format buffered Trices on the target. First run `trice insert` or `trice bind`, then use `trice generate -src <source> -logC[=<output.c>]`. That can be interesting in environments where Go-compiled binaries are not executable, such as [PCs running QNX OS](https://github.com/rokath/trice/discussions/263#discussioncomment-4180692). See also chapter [Trice Generate](trice_generate.md).

## 2. <a id="trice-location-information-file-lijson"></a>Trice location information file li.json

* If the generated `li.json` is available, the Trice tool automatically displays file name and line number. But that is accurate only with the exact matching firmware version. That usually is the case right after compiling and of most interest at the developers table.
* The Trice tool will silently not display location information, if the `li.json` file is not found. For in-field logging, the option `-showID "inf:%5d"` could be used. This allows later an easy location of the relevant source code.
* Another option is to record the binary trice messages (`trice log -p com1 -blf aFileName`) and to play them later with the Trice tool using the correct `li.json` (`trice log -p FILEBUFFER -args aFileName` ).
* Each entry contains only `File` and `Line`. `File` retains the normalized source path relative to `-liRoot`; the default root is the directory containing `li.json`.
* Use an explicit project root when `li.json` lives in a build directory but stable project-relative paths are wanted: `trice insert -li build/demoLI.json -liRoot .`.
* `File` uses `/` separators on every platform. Required leading `..` components are preserved. If Windows cannot make a source path relative across volumes, the normalized absolute path is stored.
* `trice log` and `tlog` show only the filename by default. `-liMaxDirs=N` adds at most `N` immediately preceding parent directories. Leading `..` components are never displayed or counted.
* The v1.3.0 option `-liPath` is replaced by `-liRoot` for storage and `-liMaxDirs` for display. Both v1.3.0 and the current location files use `File` and `Line`.
* The Trice repository uses `-liRoot .` while generating the shared `demoLI.json`, so all stored paths remain relative to the repository root across supported platforms.
* Active demo and example LI files, including `demoLI.json`, are generated locally rather than versioned. Build and ID-workflow scripts initialize a missing file and retain existing LI evidence. The historical `_test/testdata/testLI.json` fixture remains versioned.
* Keep local LI between ID-processing runs even if source locations are unwanted in logs. `trice log -li off` suppresses their loading and display while Bind and Insert can continue using LI for ID ownership and reuse. See [Location Information](trice_command_line_interface_examples.md#location-information).

<p align="right">(<a href="#trice-decoding">back to top</a>)</p>

