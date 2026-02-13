

##  20. <a id='trice-decoding'></a>Trice Decoding

The 14-bit IDs are used to display the log strings. These IDs are pointing in two reference files.

###  20.1. <a id='trice-id-list-til.json'></a>Trice ID list til.json

* This file integrates all firmware variants and versions and is the key to display the message strings. With the latest version of this file all previous deployed firmware images are usable without the need to know the actual firmware version.
* The files `til.h` and `til.c` are generated to help writing an own trice decoder tool in your preferred language. Use `trice generate -tilH -tilC` for creation. That can be interesting in environments, where Go compiled binaries not executable, like [PCs running QNX OS](https://github.com/rokath/trice/discussions/263#discussioncomment-4180692). See also chapter [Trice Generate](#trice-generate).

###  20.2. <a id='trice-location-information-file-li.json'></a>Trice location information file li.json

* If the generated `li.json` is available, the Trice tool automatically displays file name and line number. But that is accurate only with the exact matching firmware version. That usually is the case right after compiling and of most interest at the developers table.
* The Trice tool will silently not display location information, if the `li.json` file is not found. For in-field logging, the option `-showID "inf:%5d"` could be used. This allows later an easy location of the relevant source code.
* An other option is to record the binary trice messages (`trice log -p com1 -blf aFileName`) and to play them later with the Trice tool using the correct `li.json` (`trice log -p FILEBUFFER -args aFileName` ).
* The user has the possibility to change the default kind of the location information path. Normally it is "base" and just the source code file names, containing Trice macros occur inside the _li.json_ file. That is sufficient as long there are no files with identical names on different places possibly containing identical Trices. Then it is not guarantied that the same IDs are always assigned. For such cases "relative" or "full" is selectable for the location information path kind (CLI -liPath). But both have weaknesses: The "full" path can differ between different machines and "relative" can differ between different projects on the same PC. Again: we are talking here about identical Trice maessges in files with identical names and the worst happening is that these "exchange" their IDs between `trice insert && trice clean && trice insert`. When using the Trice cache even less inconsistencies are expected.
  * The Trice folder itself is an example, how to deal with several projects and user library code. The "user libraries" here are inside `./_test/testdata` and `./examples/exampleData`. The "user projects" are the folders `./examples/*_inst/`. You can take the `*.sh` scripts as examples how to deal with such case.

<p align="right">(<a href="#top">back to top</a>)</p>

##  21. <a id='trice-id-numbers'></a>Trice ID Numbers

###  21.1. <a id='id-number-selection'></a>ID number selection

* The default encoding TREX supports 14-bit IDs, so over 16000 IDs possible. Other encodings can work with other ID sizes.
* `trice("Hi!\n");` ➡ `trice i` ➡ `trice( iD(12345), "Hi!\n");` ➡ `trice c` ➡ `trice("Hi!\n");`
* The **ID** `12345` is a number assigned to `trice( "Hi!\n");` in the above example.
  * It is a so far unused number, according to rules you can control:
    * The `-IDMethod` switch allows a selection method for new IDs.
      * Per default new IDs determined randomly to keep the chance low, that several developers grab the same ID.
      * Example: `trice insert -IDMin 1000 -IDMethod upward` will choose the smallest free ID >= 1000.
        * This allows to use the ID space without wholes.
    * The `-IDMin` and `-IDMax` switches are usable to control the ID range, a new ID is selected from, making it possible to divide the ID space. Each developer can gets it region.
      * Example: `trice insert -IDMin 6000 -IDMax 6999` will choose new randomly IDs only between 6000 and 6999.
* It is possible to give each Trice tag an **ID** range making it possible to implement Trice tag specific runtime on/off on the target side if that is needed. This could be interesting for routing purposes also. Please run `trice help -insert` and read about the `-IDRange` switch for more details.

####  21.1.1. <a id='trice-tool-internal-method-to-get-fast-a-random-id'></a>Trice tool internal Method to get fast a random ID

* Create Slice with numbers 1...16383
* Remove all used ID numbers from this slice
* Get random number between 0 and len(slice)
* remove this ID from slice and use it as new ID

###  21.2. <a id='id-number-usage-and-stability'></a>ID number usage and stability

* If you write `trice( "msg:%d", 1);` again on a 2nd location, the copy gets a different **ID**, because each Trice gets its own **ID**.
* If you change `trice( "msg:%d", 1);` to `trice8( "msg:%d", 1);`, to reduce the needed parameter space, a new **ID** is assigned. That is because the parameter bit width is implicit a part of the now changed Trice. If you change that back, the previous **ID** is assigned again.
* If you change `trice( "msg:%d", 1);` to `TRice8( "msg:%d", 1);`, to get a 32-bit stamp, the associated **ID** remains unchanged. That is because the optional stamp is not a part of the Trice itself.
* IDs stay constant and get only changed to solve conflicts.
* To make sure, a single ID will not be changed, you could change it manually to a hexadecimal syntax.
  * This lets the `trice insert` command ignore such Trice macros and therefore a full [til.json](https://github.com/rokath/trice/blob/main/demoTIL.json) rebuild will not add them anymore. Generally this should not be done, because this could cause future bugs.
  * It is possible to assign an ID manually as decimal number. It will be added to the ID list automatically during the next `trice i|c` if no conflicts occur.
* If a Trice was deleted inside the source tree (or file removal) the appropriate ID stays inside the ID list.
* If the same string appears again in the same file this ID is active again.
* If a trice occurs more than one time, each occurrence gets a different ID. If then 2 of them disappear, their ID numbers stay in `til.json`. If then one of them comes back, it gets its ID back.

###  21.3. <a id='trice-id-0'></a>Trice ID 0

* The trice ID 0 is a placeholder for "no ID", which is replaced automatically during the next `trice insert` according to the used trice switches `-IDMethod`, `-IDMin` and `IDMax`.
  * It is sufficient to write the `TRICE` macros just without the `id(0),` `Id(0),` `ID(0),`. It will be inserted automatically according the `-defaultStampSize` switch. With `trice clean` these stay with 0-values in the source code to encode the intended stamp size.
  * It is recommended to use the `trice`, `Trice` and `TRice` macros instead of `TRICE`. They encode the stamp size in their names already. There may be cases, where the user prefers to use the code inserting macros `TRICE` to get maximum performnce.

<p align="right">(<a href="#top">back to top</a>)</p>

##  22. <a id='trice-id-management'></a>Trice ID management

###  22.1. <a id='trice-inside-source-code'></a>Trice inside source code

####  22.1.1. <a id='trice-in-source-code-comments'></a>Trice in source code comments

* Trice macros commented out, are **visible** for the `trice insert` command and therefore regarded.
  * Example: `// trice("Hi!\n");` is still regarded by the `trice i`.
* During `trice insert` commented out Trice macros, are treated in the same way as active Trice macros. Even after deletion their content stays inside til.json. This is intensionally to get best stability across several firmware versions or variants.
* The trice tool does treat trice statements inside comments or excluded by compiler switches also.

####  22.1.2. <a id='different-ids-for-same-trices'></a>Different IDs for same Trices

* When the same Trice is used several times with identical IDs, after copying, and `trice insert` is called, only one ID survives in the source code. The other Trices get assigned new IDs. Otherwise the location information would not be correct everywhere.

####  22.1.3. <a id='same-ids-for-different-trices'></a>Same IDs for different Trices

* If duplicate ID's with different format strings found inside the source tree (case several developers or source code merging) one ID is replaced by a new ID. The probability for such case is low, because of the default random ID generation.
* Also you can simply copy a Trice statement and modify it without dealing with the ID.
* The Trice tool will detect the 2nd (or 3rd) usage of this ID and assign a new one, also extending the ID list.
* That is done silently for you during the next `trice insert`.
* When you use the [Trice Cache](#trice-cache), the IDs are invisible and all happens in the background automatically.

####  22.1.4. <a id='id-routing'></a>ID Routing

With the Trice insert CLI switch `-IDRange` each Trice [tag](#tags,-color-and-log-levels) can get a specific ID range assigned and inside the project specific *triceConfig.h* the user can control, which ID range is routed to specific output channels. Search for `_MIN_ID` inside **triceDefaultConfig.h** and extend your search than for example `TRICE_UARTA_MIN_ID` to explore how to use.

####  22.1.5. <a id='possibility-to-create-new-tags-without-modifying-trice-tool-source'></a>Possibility to create new tags without modifying trice tool source

According to the demand in [541](https://github.com/rokath/trice/issues/541) a CLI switch `-ulabel` exists now.

Use `-ulabel` for additional user labels. Try this example in an empty folder:

* File *main.c*:

```C
#include "trice.h"

int main(void){
    trice("msg:hi\n");
    trice("man:hi\n");
    trice("wife:hi\n");
    trice("any:hi\n");
}
```

* Bash:

```bash
touch til.json li.json
trice i -IDMin 1004 -IDMax 6999 -IDRange wife:16000,16009 -IDRange man:1000,1003 -ulabel man -ulabel wife
```

* File *main.c*:

```C
#include "trice.h"

int main(void){
    trice(iD(5778), "msg:hi\n");
    trice(iD(1002), "man:hi\n");
    trice(iD(16004), "wife:hi\n");
    trice(iD(2184), "any:hi\n");
}
```

<p align="right">(<a href="#top">back to top</a>)</p>

##  23. <a id='trice-version-1.0-log-level-control'></a>Trice version 1.0 Log-level Control

###  23.1. <a id='trice-version-1.0-compile-time-log-level-control'></a>Trice version 1.0 Compile-time Log-level Control

In Trice version 1.0 is no compile-time log-level control. You can only disable **all** Trice logs 

* on file level by adding a `#define TRICE_OFF` line before `#include "trice.h"`
* or project level by using `-DTRICE_OFF` as compiler switch.

###  23.2. <a id='trice-version-1.0-run-time-log-level-control'></a>Trice version 1.0 Run-time Log-level Control

Because the target Trice code is so fast and generates only a few bytes per log, in Trice version 1.0 is no direct run-time log-level control inside the target code.  The user has the Trice CLI switches `-ban`, `-pick` and `-logLevel`, to control, which Trice messages are displayed by  the Trice tool.

###  23.3. <a id='trice-version-1.0-compile-time---run-time-log-level-control'></a>Trice Version 1.0 Compile-time - Run-time Log-level Control

During compilation the developer can control which Trice tags, like *info* in `trice( "info:...\n");` get get which ID range. Look for `-IDRange` in `trice h -i` output. By defining values like `TRICE_UARTA_MIN_ID` in the project specific *triceConfig.h* during compile-time is controllable, which Trice tags get routed to an output device or not.

<p align="right">(<a href="#top">back to top</a>)</p>

##  24. <a id='id-reference-list-til.json'></a>ID reference list til.json

* The `trice insert` command demands a **til.json** file - it will not work without it. That is a safety feature to avoid unwanted file generations. If you are sure to create a new **til.json** file, create an empty one: `touch til.json`.
* The name **til.json** is a default one. With the command line parameter `-i` you can use any filename.
* It is possible to use several **til.json** files - for example one for each target project but it is easier to maintain only one **til.json** file for all projects.
* The ID reference list keeps all obsolete IDs with their format strings allowing compatibility to former firmware versions.
* One can delete the ID reference list when IDs inside the code. It will be reconstructed automatically from the source tree with the next `trice clean` command, but history is lost then.
* Keeping obsolete IDs makes it more comfortable during development to deal with different firmware variants at the same time.

###  24.1. <a id='til.json-version-control'></a>til.json Version control

* The ID list should go into the version control repository of your project.
* To keep it clean from the daily development garbage one could `git restore til.json`, and re-build just before check-in.

```diff
--> Deleting til.json should not not be done when the sources are without IDs. 
--> That would result in a loss of the complete ID history and a assignment of a complete new set of IDs.
```

You could write a small bash script similar to this (untested):

```bash
trice insert -cache # Insert the IDs into the source code.
git restore til.json # Forget the todays garbage.

# Add the todays IDs to the restored til.json and clean the code.
# We have to deactivate the cache to force the file processing to get the new IDs into til.json.
trice clean # Remove the IDs from the source code with deactivated cache.  
```

###  24.2. <a id='long-time-availability'></a>Long Time Availability

* You could place a download link for the Trice tool and the used **til.json** list.
* Optionally add the (compressed/encrypted) ID reference list as resource into the target FLASH memory to be sure not to loose it in the next 200 years.

<p align="right">(<a href="#top">back to top</a>)</p>

##  25. <a id='the-trice-insert-algorithm'></a>The Trice Insert Algorithm

###  25.1. <a id='starting-conditions'></a>Starting Conditions

```diff
@@ To understand this chapter you should look into the Trice tool source code. @@
```

* Before `trice i` is executed on a source tree, the starting conditions are partially undefined:
  * A trice ID list file `til.json` file must exist, but it is allowed to be empty.
    * The `til.json` is a serialized key-value map, where
      * the keys are the IDs i and
      * the values are Trice format string structs (bit width plus format string) named f.
      * When de-serializing, it is not impossible, that an ID is used more than one times. This can only happen, when **til.json** was edited manually, what normally is not done. But could be the result of a `git merge`.
        * The trice tool will report that as error and stop. The user then has to correct the error manually, for example by deleting one of the doubled keys.
      * This ID look-up is the key-value map `idToFmt TriceIDLookUp` as `map[TriceID]TriceFmt`.
        * Each ID i as key, points to one and only one f.
        * The TriceFmt structs contains the parameter width and the format string.
      * The idToFmt is reverted then into `fmtToId triceFmtLookUp` as map[TriceFmt]TriceIDs.
        * `TriceIDs` is a triceID slice because the identical f can have several ids (no shared IDs).
        * The format struct f look-up map fmtToId is used internally for faster access and always in sync with idToFmt.
    * idToFmt and fmtToId together are named lu.
  * A location information file `li.json` may exist or not.
    * The `li.json` is a serialized key-value map `idToLocRef TriceIDLookUpLI`, a `map[TriceID]TriceLI`, where
      * the keys are the IDs i and
      * the values are the location information (filename, line and position in line) structs.
    * Each ID as key points to one and only one location information.
* The `til.json` IDs may occur in the source tree not at all, once or several times. Also it is not guarantied, that the source tree Trices match the `til.json` value.
  * That is possible after code edit, for example or code copied or modified.
  * One and only one position is used and relevant, all others are ignored. If no `til.json` exists on the expected location the user must provide one, at least an empty file.
* The `li.json` IDs may occur in the source tree not at all, once or several times. Also it is not guarantied, that the source tree Trices match the `li.json` value.
  * One and only one position is used and relevant, all others are ignored. If no `li.json` exists on the expected location trice insert creates one there.
* The src tree can contain IDs not present inside `til.json`. This state is seldom, for example after adding sources containing IDs. <!-- To keep `trice i` short in execution. `trice refresh` could be run in such cases. -->

###  25.2. <a id='aims'></a>Aims

* The `trice insert` main aim is to have a consistent state between `til.json`, `li.json` and the source tree with no **ID** used twice.
* Also the changes should be minimal.
* As a general rule lu is only extendable.
* li is rebuild from scratch.
* For faster operation files will be processed parallel.
* To keep the [Trice ID management](#trice-id-management) simple, the `insert` operation acts "per file". That means, that in case a file is renamed or code containing trice statements is copied to an other file, new IDs are generated for the affectes trices.
  * File name changes occur are not that often, so that should be acceptable.

###  25.3. <a id='method'></a>Method

####  25.3.1. <a id='trice-insert-initialization'></a>Trice Insert Initialization

```Go
// insertIDsData holds the insert run specific data.
type insertIDsData struct {
    idToFmt    TriceIDLookUp     // idToFmt is a trice ID lookup map and is generated from existing til.json file at the begin of SubCmdIdInsert. This map is only extended during SubCmdIdInsert and goes back into til.json afterwards.
    fmtToId    triceFmtLookUp    // fmtToId is a trice fmt lookup map (reversed idToFmt for faster operation) and kept in sync with idToFmt. Each fmt can have several trice IDs (slice).
    idToLocRef TriceIDLookUpLI   // idToLocInf is the trice ID location information as reference generated from li.json (if exists) at the begin of SubCmdIdInsert and is not modified at all. At the end of SubCmdIdInsert a new li.json is generated from itemToId.
    itemToId   TriceItemLookUpID // itemToId is a trice item lookup ID map, extended from source tree during SubCmdIdInsert after each found and maybe modified trice item.
    idToItem   TriceIDLookupItem // idToItem is a trice ID lookup item map (reversed itemToId for faster operation) and kept in sync with itemToId.
}
```

* Create an `insertIDsData` instance.
* De-serialize `til.json` into `idToFmt` and `fmtToId`. On error abort and report for manual correction. One result is a slice with used IDs.
* De-serialize `li.json` into `idToLocRef`. On error abort and report for manual correction. As result the slice with used IDs is extended.
  * If `li.json` contains IDs not already inside `til.json`, these are reported as warning.
  * `idToLocRef` stays untouched and is used only in cases when identical f are found.
* Create a slice `IDSpace` with numbers IDMin ... IDMax (1 ... 16383, or 1000 ... 1999 if specified in the command line that way)
* Remove all used IDs from `IDSpace`.
  * If used IDs outside IDMin and IDmax, for example IDMin=1000, IDmax=1999 and some used IDs are bigger or smaller these are not removable from IDroom what is ok.
* Create empty `itemToId` and `idToItem`.
* Walk the src and create a **s**ource **t**ree **m**ap STM with
  * key=`Trice+LI` and
  * value=**ID**.
* During STM creation use these rules:
  * If the next found f src ID == n != 0:
    * If ID n already inside STM set ID = 0 (that is brutal but ok)
    * Otherwise extend STM with ID n and remove n from ID space
      * It is possible, f is used n times with different IDs, so that is no problem.
      * It is possible, f is used n times with the same ID, so the first occurrence is the winner.
  * If the next found f src ID == 0 (normal case after trice z):
    * Look in flu
      * If not there, create new id and extend STM.
        * The new ID is "new", so forbidden to be inside ilu.
        * If it is accidentally somewhere in the so far unparsed src, we do not know that and therefore do not care about.
          * That is a seldom case and not worth to parse the source tree twice all the time.
        * Patch id into source and extend STM.
      * If the ID slice has len 1 (usually the case), take that n, extend STM and remove f from flu.
        * That is important because f could be copied before.
      * If the ID slice has a len > 1 (several IDs on the same string) check li
        * If li is empty, just remove the first id from the slice and extend STM
        * Loop over slice IDs
          * If a file matches, take the first occurrence, extend STM and remove id from the ID slice
          * If no file matches do the same as when li is empty.
          * That means, after file renaming or code copying between files during trice z state, new IDs are generated for that parts.
            * That is only for same f with several IDs cases
          * File changes during trice i state are ok, because STM is generated with the IDs inside the sources.

Until here the algorithm seem to be ok.

* STM is not needed but maybe helpful during debugging.
* STM than is usable to regenerate li.json and to extend til.json

* If after `trice i` a `trice c` and a `trice i` again is executed, all IDs are expected to be at the same place again. If in between `trice i`, an optional `trice c`and a `trice i` src was edited, most IDs are expected to be at the same place again.

###  25.4. <a id='user-code-patching-(trice-insert)'></a>User Code Patching (trice insert)

* A Trice **ID** is inserted by `trice insert` as shown in the table:

  | Unpatched User Code | After `trice insert`          | Remark        |
  |---------------------|-------------------------------|---------------|
  | `trice( "Hi!\n");`  | `trice( iD(12345), "Hi!\n");` | no stamps     |
  | `Trice( "Hi!\n");`  | `Trice( iD(12345), "Hi!\n");` | 16-bit stamps |
  | `TRice( "Hi!\n");`  | `TRice( iD(12345), "Hi!\n");` | 32-bit stamps |

* Legacy code is handled this way:

  | Unpatched User Code       | After `trice insert`          | Remark                                             |
  |---------------------------|-------------------------------|----------------------------------------------------|
  | `TRICE( "Hi!\n");`        | `TRICE( id(12345), "Hi!\n");` | no stamps after `trice i -defaultStampSize 0`      |
  | `TRICE( "Hi!\n");`        | `TRICE( Id(12345), "Hi!\n");` | 16-bit stamps after `trice i -defaultStampSize 16` |
  | `TRICE( "Hi!\n");`        | `TRICE( ID(12345), "Hi!\n");` | 32-bit stamps after `trice i -defaultStampSize 32` |
  | `TRICE( id(0), "Hi!\n");` | `TRICE( id(12345), "Hi!\n");` | no stamps                                          |
  | `TRICE( Id(0), "Hi!\n");` | `TRICE( Id(12345), "Hi!\n");` | 16-bit stamps                                      |
  | `TRICE( ID(0), "Hi!\n");` | `TRICE( ID(12345), "Hi!\n");` | 32-bit stamps                                      |

* A pre-build step `trice insert` generates the `Id(12345)` part. Examples:
  * `trice i` in your project root expects a til.json file there and checks sources and **til.json** for changes to insert.
  * `trice i -v -i ../../../til.json -src ../src -src ../lib/src -src ./` is a typical case as automated pre-build step in your project settings telling Trice to scan the project dir and two external directories. Even `trice i` is fast, it is generally quicker to search only relevant places.

###  25.5. <a id='user-code-patching-examples'></a>User Code Patching Examples

* A Trice **ID** is modified as shown in these cases:
  * Previously inserted (patched) user code copied to a different location:

    ```C
    trice(iD(12345), "Hi!\n"); // copied
    trice(iD(12345), "Hi!\n"); // original
    trice(iD(12345), "Hi!\n"); // copied
    ```

  * After updating (patching) again:

    ```C
    trice(iD(12345), "Hi!\n");
    trice(iD( 1233), "Hi!\n"); // re-patched
    trice(iD( 1234), "Hi!\n"); // re-patched
    ```

    * If the code is copied inside the same file, the first occurrence after the copy stays unchanged and the following are modified.
    * If the code is copied to other files only, the copies get new IDs.
  * Previously inserted (patched) user code copied and modified:

    ```C
    trice(iD(12345), "Ha!\n"); // copied and modified
    trice(iD(12345), "Hi!\n"); // original
    trice(iD(12345), "Ha!\n"); // copied and modified
    ```

  * After updating (patching) again:

    ```C
    trice(iD( 2333), "Ha!\n"); // re-patched
    trice(iD(12345), "Hi!\n"); // unchanged
    trice(iD( 1234), "Ha!\n"); // re-patched
    ```

  * If the code is copied to other files, it is re-patched.
* A Trice **ID** is stays the same if the stamp size is changed. Example:

  ```C
  trice( iD(12345), "Hi!" ); // original
  ```

  ```C
  TRice( iD(12345), "Hi!" ); // manually changed stamp size and then "trice i" performed.
  ```

###  25.6. <a id='exclude-folders-&-files-from-being-parsed-(pull-request-529)'></a>Exclude folders & files from being parsed (pull request 529)

The pull request [\#529](https://github.com/rokath/trice/pull/529) introduces key enhancement:

```b
    -exclude Flag
    Introduces a command-line flag -exclude that allows users to specify one or more source addresses to be omitted from scanning or processing. This improves flexibility in environments with known noisy or irrelevant sources.
```

-exclude Flag Example

The -exclude flag can be used multiple times to omit specific files or directories from scanning. Wildcards are not supported.

trice insert -v -src ./_test/ -exclude _test/src/trice.h -exclude _test/generated/

###  25.7. <a id='id-usage-options'></a>ID Usage Options

* Per default the `trice insert` command chooses randomly a so far unused ID for new format strings and extends `til.json`.
* After `trice c` all src IDs are removed or 0. In this state the src should go into the version management system.

###  25.8. <a id='general-id-management-information'></a>General ID Management Information

* Each format string gets its unique trice ID. If the same format string is used on different source code locations it gets different trice IDs this way allowing a reliable location information.
* The trice ID-instead-of-String idea lives from pre-compile patching of the user code.
* The user has full control how to deal with that.
* There are the 3 following options and the user has to decide which fits best for him. The [Trice Cache](#trice-cache) is probably the best fitting setup for many users.

####  25.8.1. <a id='option-cleaning-in-a-post-build-process'></a>Option Cleaning in a Post-build process

* The code is visually free of IDs all the time.

####  25.8.2. <a id='option-let-the-inserted-trice-id-be-a-part-of-the-user-code'></a>Option Let the inserted Trice ID be a Part of the User Code

* This is the legacy method. It allows unchanged src translation into code without using the trice tool.
* It is very robust and maybe needed in nasty debugging situations.
* It allows to reconstruct lost til.json information.
* Recommendet for small projects.

####  25.8.3. <a id='option-cleaning-on-repository-check-in'></a>Option Cleaning on Repository Check-In

* The code is visually free of IDs only inside the repository.

<p align="right">(<a href="#top">back to top</a>)</p>

##  26. <a id='trice-speed'></a>Trice Speed

 A Trice macro execution can be as cheap like **3 Assembler instructions or 6 processor clocks**:

* Disassembly: ![./ref/MEASURE_executionCode.PNG](./ref/MEASURE_executionCode.PNG)
* Measurement: The blue SYSTICK clock counts backwards 6 clocks for each Trice macro (on an ARM M0+), what is less than 100 ns @64 MHz MCU clock: ![./ref/MEASURE_executionClocks.PNG](./ref/MEASURE_executionClocks.PNG)

A more realistic (typical) timing with target location and µs timestamps, critical section and parameters is shown here with the STM32F030 M0 core:

![./ref/F030FullTiming.PNG](./ref/F030FullTiming.PNG)

The MCU is clocked with 48 MHz and a Trice duration is about 2 µs, where alone the internal ReadUs() call is already nearly 1 µs long:

![./ref/ReadUsF030.PNG](./ref/ReadUsF030.PNG)

###  26.1. <a id='target-implementation-options'></a>Target Implementation Options

All trice macros use internally this sub-macro:

```C
#define TRICE_PUT(x) do{ *TriceBufferWritePosition++ = TRICE_HTOTL(x); }while(0); //! PUT copies a 32 bit x into the TRICE buffer.
```

The usual case is `#define TRICE_HTOTL(x) (x)`. The `uint32_t* TriceBufferWritePosition` points to a buffer, which is codified and used with the Trice framing sub-macros `TRICE_ENTER` and `TRICE_LEAVE` in dependence of the use case.

####  26.1.1. <a id='trice-use-cases-trice_static_buffer-and-trice_stack_buffer---direct-mode-only'></a>Trice Use Cases TRICE_STATIC_BUFFER and TRICE_STACK_BUFFER - direct mode only

1. Each singe Trice is build inside a common buffer and finally copied inside the sub-macro `TRICE_LEAVE`.
2. Disabled relevant interrupts between `TRICE_ENTER` and `TRICE_LEAVE` are mantadory for `TRICE_STATIC_BUFFER`.
3. Usable for multiple non-blocking physical Trice channels but **not** recommended for some time blocking channels.
4. A copy call is executed inside `TRICE_LEAVE`.

* With appropriate mapping a direct write to physical output(s) is possible:
  * RTT0 without extra copy.
    * With `TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE` about 100 MCU clocks do the whole work, what is within 1.5 us @ 64 MHz.
  * AUX without extra copy.
  * Not (yet) supported UART transfer loop with polling. With 1MBit baud rate, 4-12 bytes would last 40-120 µs.

####  26.1.2. <a id='trice-use-case-trice_double_buffer---deferred-mode,-fastest-trice-execution,-more-ram-needed'></a>Trice Use Case TRICE_DOUBLE_BUFFER - deferred mode, fastest Trice execution, more RAM needed

1. Several *trices* are build in a half buffer.
1. No stack used.
1. Disabled interrupts between `TRICE_ENTER` and `TRICE_LEAVE`.
1. Usable for multiple blocking and non-blocking physical Trice channels.
1. No copy call inside `TRICE_LEAVE` but optionally an additional direct mode is supported.

####  26.1.3. <a id='trice-use-case-trice_ring_buffer---deferred-mode,-balanced-trice-execution-time-and-needed-ram'></a>Trice Use Case TRICE_RING_BUFFER - deferred mode, balanced Trice execution time and needed RAM

1. Each single *trices* is build in a ring buffer segment.
1. No stack used.
1. Disabled interrupts between `TRICE_ENTER` and `TRICE_LEAVE`.
1. Usable for multiple blocking and non-blocking physical Trice channels.
1. No copy call inside `TRICE_LEAVE` but optionally an additional direct mode is supported.
1. Allocation call inside `TRICE_ENTER`

###  26.2. <a id='a-configuration-for-maximum-trice-execution-speed-with-the-l432_inst-example'></a>A configuration for maximum Trice execution speed with the L432_inst example

* To not loose any clocks, the function `SomeExampleTrices` in [triceExamples.c](https://github.com/rokath/trice/blob/main/examples/exampleData/triceExamples.c) uses the upper case macro `TRICE` for the first "🐁 Speedy Gonzales" Trices.

* The `triceConfig.h` settings are

```C
#define TriceStamp16 (*DWT_CYCCNT) // @64MHz wraps after a bit more than 1ms (MCU clocks)
#define TriceStamp32 (*DWT_CYCCNT) // @64MHz -> 1 µs, wraps after 2^32 µs ~= 1.2 hours

#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER

#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0
```

* Both time stamps use the debug watchdog counter running with the 64 MHz MCU clock.
* No direct output to not loose time during the Trice macro execution.
* Critical sections are disabled (default), so be careful where Trices are used.
* The Trice double buffer allows the Trice macros to write without checks.
* The Trice protection, diagnostics and cycle counter are disabled to not perform unneeded clocks.
* Additionally in file [flags.mak](https://github.com/rokath/trice/blob/main/examples/L432_inst/flags.mak) the optimization is set to`C_FLAGS += -Ofast`.

After running [./build.sh](https://github.com/rokath/trice/blob/main/examples/L432_inst/build.sh), executing ` arm-none-eabi-objdump.exe -D -S -l out.clang/triceExamples.o` shows:

```bash
out.clang/triceExamples.o:     file format elf32-littlearm


Disassembly of section .text.TriceHeadLine:

00000000 <TriceHeadLine>:
TriceHeadLine():
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:10
#include "trice.h"

//! TriceHeadLine emits a decorated name. The name length should be 18 characters.
void TriceHeadLine(char * name) {
	//! This is usable as the very first trice sequence after restart. Adapt it. Use a UTF-8 capable editor like VS-Code or use pure ASCII.
	TriceS("w: Hello! 👋🙂\n\n        ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨        \n        🎈🎈🎈🎈%s🎈🎈🎈🎈\n        🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃        \n\n\n", name);
   0:	f240 0100 	movw	r1, #0
   4:	4602      	mov	r2, r0
   6:	f2c0 0100 	movt	r1, #0
   a:	f643 70f1 	movw	r0, #16369	@ 0x3ff1
   e:	f7ff bffe 	b.w	0 <TriceS>

Disassembly of section .ARM.exidx.text.TriceHeadLine:

00000000 <.ARM.exidx.text.TriceHeadLine>:
   0:	00000000 	andeq	r0, r0, r0
   4:	00000001 	andeq	r0, r0, r1

Disassembly of section .text.SomeExampleTrices:

00000000 <SomeExampleTrices>:
SomeExampleTrices():
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:14
}

//! SomeExampleTrices generates a few Trice example logs and a burst of Trices.
void SomeExampleTrices(int burstCount) {
   0:	b5f0      	push	{r4, r5, r6, r7, lr}
   2:	af03      	add	r7, sp, #12
   4:	e92d 0700 	stmdb	sp!, {r8, r9, sl}
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:15
	TRICE(ID(0), "att:🐁 Speedy Gonzales A  32-bit time stamp\n");
   8:	f240 0100 	movw	r1, #0
   c:	f2c0 0100 	movt	r1, #0
  10:	680d      	ldr	r5, [r1, #0]
  12:	f240 0800 	movw	r8, #0
  16:	f2c0 0800 	movt	r8, #0
  1a:	4604      	mov	r4, r0
  1c:	6829      	ldr	r1, [r5, #0]
  1e:	f8d8 0000 	ldr.w	r0, [r8]
  22:	f06f 0212 	mvn.w	r2, #18
  26:	8041      	strh	r1, [r0, #2]
  28:	0c09      	lsrs	r1, r1, #16
  2a:	1cd3      	adds	r3, r2, #3
  2c:	8081      	strh	r1, [r0, #4]
  2e:	21c0      	movs	r1, #192	@ 0xc0
  30:	8003      	strh	r3, [r0, #0]
  32:	80c1      	strh	r1, [r0, #6]
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:16
	TRICE(ID(0), "att:🐁 Speedy Gonzales B  32-bit time stamp\n");
  34:	682b      	ldr	r3, [r5, #0]
  36:	1c96      	adds	r6, r2, #2
  38:	8143      	strh	r3, [r0, #10]
  3a:	0c1b      	lsrs	r3, r3, #16
  3c:	8106      	strh	r6, [r0, #8]
  3e:	8183      	strh	r3, [r0, #12]
  40:	81c1      	strh	r1, [r0, #14]
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:17
	TRICE(ID(0), "att:🐁 Speedy Gonzales C  32-bit time stamp\n");
  42:	682b      	ldr	r3, [r5, #0]
  44:	1c56      	adds	r6, r2, #1
  46:	8243      	strh	r3, [r0, #18]
  48:	0c1b      	lsrs	r3, r3, #16
  4a:	8206      	strh	r6, [r0, #16]
  4c:	8283      	strh	r3, [r0, #20]
  4e:	82c1      	strh	r1, [r0, #22]
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:18
	TRICE(ID(0), "att:🐁 Speedy Gonzales D  32-bit time stamp\n");
  50:	682b      	ldr	r3, [r5, #0]
  52:	8302      	strh	r2, [r0, #24]
  54:	0c1a      	lsrs	r2, r3, #16
  56:	8343      	strh	r3, [r0, #26]
  58:	8382      	strh	r2, [r0, #28]
  5a:	83c1      	strh	r1, [r0, #30]
  5c:	f64b 7ad4 	movw	sl, #49108	@ 0xbfd4
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:19
	TRICE(Id(0), "att:🐁 Speedy Gonzales E  16-bit time stamp\n");
  60:	682a      	ldr	r2, [r5, #0]
  62:	f6cb 7ad4 	movt	sl, #49108	@ 0xbfd4
  66:	f10a 1318 	add.w	r3, sl, #1572888	@ 0x180018
  6a:	6203      	str	r3, [r0, #32]
  6c:	8482      	strh	r2, [r0, #36]	@ 0x24
  6e:	84c1      	strh	r1, [r0, #38]	@ 0x26
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:20
	TRICE(Id(0), "att:🐁 Speedy Gonzales F  16-bit time stamp\n");
  70:	682a      	ldr	r2, [r5, #0]
  72:	f10a 1317 	add.w	r3, sl, #1507351	@ 0x170017
  76:	6283      	str	r3, [r0, #40]	@ 0x28
  78:	8582      	strh	r2, [r0, #44]	@ 0x2c
  7a:	85c1      	strh	r1, [r0, #46]	@ 0x2e
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:21
	TRICE(Id(0), "att:🐁 Speedy Gonzales G  16-bit time stamp\n");
  7c:	682a      	ldr	r2, [r5, #0]
  7e:	f10a 1316 	add.w	r3, sl, #1441814	@ 0x160016
  82:	6303      	str	r3, [r0, #48]	@ 0x30
  84:	8682      	strh	r2, [r0, #52]	@ 0x34
  86:	86c1      	strh	r1, [r0, #54]	@ 0x36
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:22
	TRICE(Id(0), "att:🐁 Speedy Gonzales H  16-bit time stamp\n");
  88:	682a      	ldr	r2, [r5, #0]
  8a:	f10a 1315 	add.w	r3, sl, #1376277	@ 0x150015
  8e:	8782      	strh	r2, [r0, #60]	@ 0x3c
  90:	f647 72e8 	movw	r2, #32744	@ 0x7fe8
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:23
	TRICE(id(0), "att:🐁 Speedy Gonzales I without time stamp\n");
  94:	f8a0 2040 	strh.w	r2, [r0, #64]	@ 0x40
  98:	f647 72e7 	movw	r2, #32743	@ 0x7fe7
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:24
	TRICE(id(0), "att:🐁 Speedy Gonzales J without time stamp\n");
  9c:	f8a0 2044 	strh.w	r2, [r0, #68]	@ 0x44
  a0:	f647 72e6 	movw	r2, #32742	@ 0x7fe6
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:25
	TRICE(id(0), "att:🐁 Speedy Gonzales K without time stamp\n");
  a4:	f8a0 2048 	strh.w	r2, [r0, #72]	@ 0x48
  a8:	f647 72e5 	movw	r2, #32741	@ 0x7fe5
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:22
	TRICE(Id(0), "att:🐁 Speedy Gonzales H  16-bit time stamp\n");
  ac:	6383      	str	r3, [r0, #56]	@ 0x38
  ae:	87c1      	strh	r1, [r0, #62]	@ 0x3e
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:23
	TRICE(id(0), "att:🐁 Speedy Gonzales I without time stamp\n");
  b0:	f8a0 1042 	strh.w	r1, [r0, #66]	@ 0x42
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:24
	TRICE(id(0), "att:🐁 Speedy Gonzales J without time stamp\n");
  b4:	f8a0 1046 	strh.w	r1, [r0, #70]	@ 0x46
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:25
	TRICE(id(0), "att:🐁 Speedy Gonzales K without time stamp\n");
  b8:	f8a0 104a 	strh.w	r1, [r0, #74]	@ 0x4a
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../exampleData/triceExamples.c:26
	TRICE(id(0), "att:🐁 Speedy Gonzales L without time stamp\n");
  bc:	f8a0 204c 	strh.w	r2, [r0, #76]	@ 0x4c
  c0:	f8a0 104e 	strh.w	r1, [r0, #78]	@ 0x4e
TRice0():
C:\Users\ms\repos\trice_wt_devel\examples\L432_inst/../../src/trice.h:741
	Trice32m_0(tid);
	TRICE_UNUSED(pFmt)
}

...
```

* There are only 7 assembler instructions between two `TRICE` macros(around line 17).
* The log output is:

![2024-12-05_L432_inst_maxSpeed.png](./ref/2024-12-05_L432_inst_maxSpeed.png)

As you can see in the highlighted blue timestamp bar, typical 8-10 clocks are needed for one Trice macro. One clock duration @64MHz is 15.625 ns, so we need about 150 ns for a Trice. Light can travel about 50 meter in that time.

###  26.3. <a id='a-configuration-for-normal-trice-execution-speed-with-the-g0b1_inst-example'></a>A configuration for normal Trice execution speed with the G0B1_inst example

* The `triceConfig.h` settings are

```C
// hardware specific trice lib settings
#include "main.h"
#define TriceStamp16 TIM17->CNT     // 0...999 us
#define TriceStamp32 HAL_GetTick()  // 0...2^32-1 ms (wraps after 49.7 days)

#define TRICE_BUFFER TRICE_RING_BUFFER

// trice l -p JLINK -args="-Device STM32G0B1RE -if SWD -Speed 4000 -RTTChannel 0" -pf none  -d16 -ts ms
//#define TRICE_DIRECT_OUTPUT 1
//#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

// trice log -p com7 -pw MySecret -pf COBS
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2

#include "cmsis_gcc.h"
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }
```

* The 16-bit time stamp counts the microseconds within 1 millisecond.
* The 32-bit time stamp counts the milliseconds.
* The ring buffer uses the RAM more effectively for the price of a bit speed.
* The encryption and framing has no influence on the Trice execution speed becuse this is done in the background.
* The critical section protects Trices in different tasks from each-other interruption and allows Trices inside interrupts parallel to normal usage.
* Per default are Trice protection, diagnostics and cycle counter active.

![2024-12-05_G0B1_inst_normalSpeed.png](./ref/2024-12-05_G0B1_inst_normalSpeed.png)

* A typical Trice duration is here 4 microseconds (with `-Ospeed`)
* Switcing the optimization to `-Oz` can result in typical 4-5 µs Trice execution time.
* Additionally enabling the Trice direct out over Segger RTT has this impact:

![2024-12-05_G0B1_inst_slowSpeed.png](./ref/2024-12-05_G0B1_inst_slowSpeed.png)

> 🛑 The Trice execution time is now over 20 microseconds❗

Still fast enough for many cases but you hopefully have a good knowledge now how to tune Trice best for your application.

<p align="right">(<a href="#top">back to top</a>)</p>

##  27. <a id='trice-memory-needs'></a>Trice memory needs

Depending on your target configuration the needed space can differ:

###  27.1. <a id='f030_bare-size'></a>F030_bare Size

* `./build.sh`:

```bash
arm-none-eabi-size build/F030_bare.elf
   text    data     bss     dec     hex filename
   2428      12    1564    4004     fa4 build/F030_bare.elf
```

That is the basic size of an empty generated project just containing some drivers.

###  27.2. <a id='f030_inst-size-with-trice_off=1'></a>F030_inst Size with TRICE_OFF=1

* `./build.sh TRICE_OFF=1` :

```bash
arm-none-eabi-size build/F030_inst.elf
   text    data     bss     dec     hex filename
   2428      12    1564    4004     fa4 build/F030_inst.elf
```

This is exactly the same result, proofing that `TRICE_OFF 1` is working correctly.

###  27.3. <a id='f030_inst-with-ring-buffer'></a>F030_inst with ring buffer

* `./build.sh`:

```bash
arm-none-eabi-size out/F030_inst.elf
   text    data     bss     dec     hex filename
   9416      28    2692   12136    2f68 out/F030_inst.elf
```

This is about 7 KB Flash and 1.2 KB RAM size for the Trice library and we see:

```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice_wt_devel/examples/F030_inst (devel)
$ trice l -p com5 -ts16 "time:     #%6d" -hs off
com5:       triceExamples.c    12      # 65535  Hello! 👋🙂
com5:
com5:         ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
com5:         🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-F030R8   🎈🎈🎈🎈
com5:         🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃
com5:
com5:
com5:       triceExamples.c    61              TRICE_DIRECT_OUTPUT == 0, TRICE_DEFERRED_OUTPUT == 1
com5:       triceExamples.c    67              TRICE_DOUBLE_BUFFER, TRICE_MULTI_PACK_MODE
com5:       triceExamples.c    76              _CYCLE == 1, _PROTECT == 1, _DIAG == 1, XTEA == 0
com5:       triceExamples.c    77              _SINGLE_MAX_SIZE=104, _BUFFER_SIZE=172, _DEFERRED_BUFFER_SIZE=1024
com5:       triceExamples.c    29    0,031_804 🐁 Speedy Gonzales a  32-bit time stamp
com5:       triceExamples.c    30    0,031_646 🐁 Speedy Gonzales b  32-bit time stamp
com5:       triceExamples.c    31    0,031_488 🐁 Speedy Gonzales c  32-bit time stamp
com5:       triceExamples.c    32    0,031_330 🐁 Speedy Gonzales d  32-bit time stamp
com5:       triceExamples.c    33      # 31172 🐁 Speedy Gonzales e  16-bit time stamp
com5:       triceExamples.c    34      # 31012 🐁 Speedy Gonzales f  16-bit time stamp
com5:       triceExamples.c    35      # 30852 🐁 Speedy Gonzales g  16-bit time stamp
com5:       triceExamples.c    36      # 30692 🐁 Speedy Gonzales h  16-bit time stamp
com5:       triceExamples.c    42      # 30224 2.71828182845904523536 <- float number as string
com5:       triceExamples.c    43      # 29517 2.71828182845904509080 (double with more ciphers than precision)
com5:       triceExamples.c    44      # 29322 2.71828174591064453125 (float  with more ciphers than precision)
com5:       triceExamples.c    45      # 29145 2.718282 (default rounded float)
com5:       triceExamples.c    46      # 28969 A Buffer:
com5:       triceExamples.c    47      # 28790 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
com5:       triceExamples.c    48      # 28076 31372e32  31383238  34383238  34303935  35333235
com5:       triceExamples.c    49      # 27430 ARemoteFunctionName(2e32)(3137)(3238)(3138)(3238)(3438)(3935)(3430)(3235)(3533)(3633)
com5:       triceExamples.c    50              5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
com5:       triceExamples.c    52      # 26554 i=44444400 aaaaaa00
com5:       triceExamples.c    52      # 26342 i=44444401 aaaaaa01
com5:       triceExamples.c    52      # 26130 i=44444402 aaaaaa02
com5:       triceExamples.c    52      # 25918 i=44444403 aaaaaa03
com5:       triceExamples.c    52      # 25706 i=44444404 aaaaaa04
com5:       triceExamples.c    29    0,031_790 🐁 Speedy Gonzales a  32-bit time stamp
com5:       triceExamples.c    30    0,031_632 🐁 Speedy Gonzales b  32-bit time stamp
com5:       triceExamples.c    31    0,031_474 🐁 Speedy Gonzales c  32-bit time stamp
com5:       triceExamples.c    32    0,031_316 🐁 Speedy Gonzales d  32-bit time stamp
com5:       triceExamples.c    33      # 31158 🐁 Speedy Gonzales e  16-bit time stamp
com5:       triceExamples.c    34      # 30998 🐁 Speedy Gonzales f  16-bit time stamp
com5:       triceExamples.c    35      # 30838 🐁 Speedy Gonzales g  16-bit time stamp
com5:       triceExamples.c    36      # 30678 🐁 Speedy Gonzales h  16-bit time stamp
com5:       triceExamples.c    42      # 30210 2.71828182845904523536 <- float number as string
com5:       triceExamples.c    43      # 29503 2.71828182845904509080 (double with more ciphers than precision)
com5:       triceExamples.c    44      # 29308 2.71828174591064453125 (float  with more ciphers than precision)
com5:       triceExamples.c    45      # 29131 2.718282 (default rounded float)
com5:       triceExamples.c    46      # 28955 A Buffer:
com5:       triceExamples.c    47      # 28776 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
com5:       triceExamples.c    48      # 28062 31372e32  31383238  34383238  34303935  35333235
com5:       triceExamples.c    49      # 27416 ARemoteFunctionName(2e32)(3137)(3238)(3138)(3238)(3438)(3935)(3430)(3235)(3533)(3633)
com5:       triceExamples.c    50              5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
com5:       triceExamples.c    52      # 26540 i=44444400 aaaaaa00
com5:       triceExamples.c    52      # 26328 i=44444401 aaaaaa01
com5:       triceExamples.c    52      # 26116 i=44444402 aaaaaa02
com5:       triceExamples.c    52      # 25904 i=44444403 aaaaaa03
com5:       triceExamples.c    52      # 25692 i=44444404 aaaaaa04
com5:    triceLogDiagData.c    44              triceSingleDepthMax = 108 of 172 (TRICE_BUFFER_SIZE)
com5:    triceLogDiagData.c    67              TriceHalfBufferDepthMax = 388 of  512
com5:       triceExamples.c    29    0,031_344 🐁 Speedy Gonzales a  32-bit time stamp
com5:       triceExamples.c    30    0,031_186 🐁 Speedy Gonzales b  32-bit time stamp
```

###  27.4. <a id='f030_inst-with-ring-buffer-1'></a>F030_inst with ring buffer

* `./build.sh`:

We need 600 bytes more Flash but could have less RAM used:

```bash
   text    data     bss     dec     hex filename
  10060      24    2688   12772    31e4 out/F030_inst.elf
```

```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice_wt_devel/examples/F030_inst (devel)
$ trice l -p com5 -ts16 "time:     #%6d" -hs off
com5:       triceExamples.c    12      # 65535  Hello! 👋🙂
com5:
com5:         ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
com5:         🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-F030R8   🎈🎈🎈🎈
com5:         🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃
com5:
com5:
com5:       triceExamples.c    61              TRICE_DIRECT_OUTPUT == 0, TRICE_DEFERRED_OUTPUT == 1
com5:       triceExamples.c    69              TRICE_RING_BUFFER, TRICE_MULTI_PACK_MODE
com5:       triceExamples.c    76              _CYCLE == 1, _PROTECT == 1, _DIAG == 1, XTEA == 0
com5:       triceExamples.c    77              _SINGLE_MAX_SIZE=104, _BUFFER_SIZE=172, _DEFERRED_BUFFER_SIZE=1024
com5:       triceExamples.c    29    0,031_732 🐁 Speedy Gonzales a  32-bit time stamp
com5:       triceExamples.c    30    0,031_531 🐁 Speedy Gonzales b  32-bit time stamp
com5:       triceExamples.c    31    0,031_330 🐁 Speedy Gonzales c  32-bit time stamp
com5:       triceExamples.c    32    0,031_129 🐁 Speedy Gonzales d  32-bit time stamp
com5:       triceExamples.c    33      # 30928 🐁 Speedy Gonzales e  16-bit time stamp
com5:       triceExamples.c    34      # 30725 🐁 Speedy Gonzales f  16-bit time stamp
com5:       triceExamples.c    35      # 30522 🐁 Speedy Gonzales g  16-bit time stamp
com5:       triceExamples.c    36      # 30319 🐁 Speedy Gonzales h  16-bit time stamp
com5:       triceExamples.c    42      # 29808 2.71828182845904523536 <- float number as string
com5:       triceExamples.c    43      # 29058 2.71828182845904509080 (double with more ciphers than precision)
com5:       triceExamples.c    44      # 28821 2.71828174591064453125 (float  with more ciphers than precision)
com5:       triceExamples.c    45      # 28602 2.718282 (default rounded float)
com5:       triceExamples.c    46      # 28383 A Buffer:
com5:       triceExamples.c    47      # 28162 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
com5:       triceExamples.c    48      # 27406 31372e32  31383238  34383238  34303935  35333235
com5:       triceExamples.c    49      # 26718 ARemoteFunctionName(2e32)(3137)(3238)(3138)(3238)(3438)(3935)(3430)(3235)(3533)(3633)
com5:       triceExamples.c    50              5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
com5:       triceExamples.c    52      # 25757 i=44444400 aaaaaa00
com5:       triceExamples.c    52      # 25502 i=44444401 aaaaaa01
com5:       triceExamples.c    52      # 25247 i=44444402 aaaaaa02
com5:       triceExamples.c    52      # 24992 i=44444403 aaaaaa03
com5:       triceExamples.c    52      # 24737 i=44444404 aaaaaa04
com5:       triceExamples.c    29    0,031_746 🐁 Speedy Gonzales a  32-bit time stamp
com5:       triceExamples.c    30    0,031_545 🐁 Speedy Gonzales b  32-bit time stamp
com5:       triceExamples.c    31    0,031_344 🐁 Speedy Gonzales c  32-bit time stamp
com5:       triceExamples.c    32    0,031_143 🐁 Speedy Gonzales d  32-bit time stamp
com5:       triceExamples.c    33      # 30942 🐁 Speedy Gonzales e  16-bit time stamp
com5:       triceExamples.c    34      # 30739 🐁 Speedy Gonzales f  16-bit time stamp
com5:       triceExamples.c    35      # 30536 🐁 Speedy Gonzales g  16-bit time stamp
com5:       triceExamples.c    36      # 30333 🐁 Speedy Gonzales h  16-bit time stamp
com5:       triceExamples.c    42      # 29822 2.71828182845904523536 <- float number as string
com5:       triceExamples.c    43      # 29072 2.71828182845904509080 (double with more ciphers than precision)
com5:       triceExamples.c    44      # 28835 2.71828174591064453125 (float  with more ciphers than precision)
com5:       triceExamples.c    45      # 28616 2.718282 (default rounded float)
com5:       triceExamples.c    46      # 28397 A Buffer:
com5:       triceExamples.c    47      # 28176 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
com5:       triceExamples.c    48      # 27420 31372e32  31383238  34383238  34303935  35333235
com5:       triceExamples.c    49      # 26732 ARemoteFunctionName(2e32)(3137)(3238)(3138)(3238)(3438)(3935)(3430)(3235)(3533)(3633)
com5:       triceExamples.c    50              5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
com5:       triceExamples.c    52      # 25771 i=44444400 aaaaaa00
com5:       triceExamples.c    52      # 25516 i=44444401 aaaaaa01
com5:       triceExamples.c    52      # 25261 i=44444402 aaaaaa02
com5:       triceExamples.c    52      # 25006 i=44444403 aaaaaa03
com5:       triceExamples.c    52      # 24751 i=44444404 aaaaaa04
com5:    triceLogDiagData.c    44              triceSingleDepthMax = 108 of 172 (TRICE_BUFFER_SIZE)
com5:    triceLogDiagData.c    75              triceRingBufferDepthMax = 324 of 1024
com5:       triceExamples.c    29    0,031_188 🐁 Speedy Gonzales a  32-bit time stamp
com5:       triceExamples.c    30    0,030_987 🐁 Speedy Gonzales b  32-bit time stamp
```

###  27.5. <a id='a-developer-setting,-only-enabling-segger_rtt'></a>A developer setting, only enabling SEGGER_RTT

* `./build.sh`:

```bash
arm-none-eabi-size out/F030_inst.elf
   text    data     bss     dec     hex filename
   6656      16    2768    9440    24e0 out/F030_inst.elf
```

About 4 KB Flash needed and we see:

```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice_wt_devel/examples/F030_inst (devel)
$ trice l -p jlink -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0" -pf none -d16 -showID "deb:%5d"
Dec  6 16:14:38.276356  jlink:       triceExamples.c    12       65_535 16369  Hello! 👋🙂
Dec  6 16:14:38.276356  jlink:
Dec  6 16:14:38.276356  jlink:         ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
Dec  6 16:14:38.276356  jlink:         🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-F030R8   🎈🎈🎈🎈
Dec  6 16:14:38.276356  jlink:         🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃
Dec  6 16:14:38.276356  jlink:
Dec  6 16:14:38.276356  jlink:
Dec  6 16:14:38.276356  jlink:       triceExamples.c    61              16334 TRICE_DIRECT_OUTPUT == 1, TRICE_DEFERRED_OUTPUT == 0
Dec  6 16:14:38.276356  jlink:       triceExamples.c    63              16333 TRICE_STACK_BUFFER, TRICE_MULTI_PACK_MODE
Dec  6 16:14:38.276920  jlink:       triceExamples.c    76              16327 _CYCLE == 1, _PROTECT == 1, _DIAG == 1, XTEA == 0
Dec  6 16:14:38.277424  jlink:       triceExamples.c    77              16326 _SINGLE_MAX_SIZE=104, _BUFFER_SIZE=172, _DEFERRED_BUFFER_SIZE=1024
Dec  6 16:14:39.181228  jlink:       triceExamples.c    29    0,031_848 16356 🐁 Speedy Gonzales a  32-bit time stamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    30    0,031_292 16355 🐁 Speedy Gonzales b  32-bit time stamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    31    0,030_736 16354 🐁 Speedy Gonzales c  32-bit time stamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    32    0,030_180 16353 🐁 Speedy Gonzales d  32-bit time stamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    33       29_624 16352 🐁 Speedy Gonzales e  16-bit time stamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    34       29_066 16351 🐁 Speedy Gonzales f  16-bit time stamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    35       28_508 16350 🐁 Speedy Gonzales g  16-bit time stamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    36       27_950 16349 🐁 Speedy Gonzales h  16-bit time stamp
Dec  6 16:14:39.181228  jlink:       triceExamples.c    42       27_086 16344 2.71828182845904523536 <- float number as string
Dec  6 16:14:39.181228  jlink:       triceExamples.c    43       25_906 16343 2.71828182845904509080 (double with more ciphers than precision)
Dec  6 16:14:39.181798  jlink:       triceExamples.c    44       25_305 16342 2.71828174591064453125 (float  with more ciphers than precision)
Dec  6 16:14:39.181798  jlink:       triceExamples.c    45       24_727 16341 2.718282 (default rounded float)
Dec  6 16:14:39.181798  jlink:       triceExamples.c    46       24_148 16340 A Buffer:
Dec  6 16:14:39.181798  jlink:       triceExamples.c    47       23_578 16339 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
Dec  6 16:14:39.181798  jlink:       triceExamples.c    48       22_394 16338 31372e32  31383238  34383238  34303935  35333235
Dec  6 16:14:39.181798  jlink:       triceExamples.c    49       21_295 16337 ARemoteFunctionName(2e32)(3137)(3238)(3138)(3238)(3438)(3935)(3430)(3235)(3533)(3633)
Dec  6 16:14:39.181798  jlink:       triceExamples.c    50              16200 5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
Dec  6 16:14:39.182303  jlink:       triceExamples.c    52       19_555 16335 i=44444400 aaaaaa00
Dec  6 16:14:39.182303  jlink:       triceExamples.c    52       18_941 16335 i=44444401 aaaaaa01
Dec  6 16:14:39.182303  jlink:       triceExamples.c    52       18_327 16335 i=44444402 aaaaaa02
Dec  6 16:14:39.182834  jlink:       triceExamples.c    52       17_713 16335 i=44444403 aaaaaa03
Dec  6 16:14:39.182834  jlink:       triceExamples.c    52       17_099 16335 i=44444404 aaaaaa04
Dec  6 16:14:40.187121  jlink:       triceExamples.c    29    0,031_855 16356 🐁 Speedy Gonzales a  32-bit time stamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    30    0,031_299 16355 🐁 Speedy Gonzales b  32-bit time stamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    31    0,030_743 16354 🐁 Speedy Gonzales c  32-bit time stamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    32    0,030_187 16353 🐁 Speedy Gonzales d  32-bit time stamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    33       29_631 16352 🐁 Speedy Gonzales e  16-bit time stamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    34       29_073 16351 🐁 Speedy Gonzales f  16-bit time stamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    35       28_515 16350 🐁 Speedy Gonzales g  16-bit time stamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    36       27_957 16349 🐁 Speedy Gonzales h  16-bit time stamp
Dec  6 16:14:40.187121  jlink:       triceExamples.c    42       27_093 16344 2.71828182845904523536 <- float number as string
Dec  6 16:14:40.187121  jlink:       triceExamples.c    43       25_913 16343 2.71828182845904509080 (double with more ciphers than precision)
Dec  6 16:14:40.187121  jlink:       triceExamples.c    44       25_310 16342 2.71828174591064453125 (float  with more ciphers than precision)
Dec  6 16:14:40.187121  jlink:       triceExamples.c    45       24_730 16341 2.718282 (default rounded float)
Dec  6 16:14:40.187121  jlink:       triceExamples.c    46       24_149 16340 A Buffer:
Dec  6 16:14:40.187121  jlink:       triceExamples.c    47       23_577 16339 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
Dec  6 16:14:40.187630  jlink:       triceExamples.c    48       22_391 16338 31372e32  31383238  34383238  34303935  35333235
Dec  6 16:14:40.187690  jlink:       triceExamples.c    49       21_290 16337 ARemoteFunctionName(2e32)(3137)(3238)(3138)(3238)(3438)(3935)(3430)(3235)(3533)(3633)
Dec  6 16:14:40.187690  jlink:       triceExamples.c    50              16200 5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
Dec  6 16:14:40.187690  jlink:       triceExamples.c    52       19_546 16335 i=44444400 aaaaaa00
Dec  6 16:14:40.188195  jlink:       triceExamples.c    52       18_930 16335 i=44444401 aaaaaa01
Dec  6 16:14:40.188195  jlink:       triceExamples.c    52       18_314 16335 i=44444402 aaaaaa02
Dec  6 16:14:40.188195  jlink:       triceExamples.c    52       17_698 16335 i=44444403 aaaaaa03
Dec  6 16:14:40.188195  jlink:       triceExamples.c    52       17_082 16335 i=44444404 aaaaaa04
Dec  6 16:14:41.191648  jlink:    triceLogDiagData.c    21              16382 RTT0_writeDepthMax=325 (BUFFER_SIZE_UP=1024)
Dec  6 16:14:41.191648  jlink:    triceLogDiagData.c    44              16378 triceSingleDepthMax = 108 of 172 (TRICE_BUFFER_SIZE)
Dec  6 16:14:41.191648  jlink:       triceExamples.c    29    0,030_628 16356 🐁 Speedy Gonzales a  32-bit time stamp
Dec  6 16:14:41.191648  jlink:       triceExamples.c    30    0,030_072 16355 🐁 Speedy Gonzales b  32-bit time stamp
```

"🐁 Speedy Gonzales" needs about 500 MCU clocks.

###  27.6. <a id='a-developer-setting,-only-enabling-segger_rtt-and-without-deferred-output-gives-after-running-`./build.sh-trice_diagnostics=0-trice_protect=0`:'></a>A developer setting, only enabling SEGGER_RTT and without deferred output gives after running `./build.sh TRICE_DIAGNOSTICS=0 TRICE_PROTECT=0`:

```bash
arm-none-eabi-size out/F030_inst.elf
   text    data     bss     dec     hex filename
   5796      16    2736    8548    2164 out/F030_inst.elf
```

That is nearly 1 KB less Flash needs.

The output:

```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice_wt_devel/examples/F030_inst (devel)
$ trice l -p jlink -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0" -pf none -d16 -showID "deb:%5d"
Dec  6 16:20:10.545274  jlink:       triceExamples.c    12       65_535 16369  Hello! 👋🙂
Dec  6 16:20:10.545274  jlink:
Dec  6 16:20:10.545274  jlink:         ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
Dec  6 16:20:10.545274  jlink:         🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-F030R8   🎈🎈🎈🎈
Dec  6 16:20:10.545274  jlink:         🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃
Dec  6 16:20:10.545274  jlink:
Dec  6 16:20:10.545274  jlink:
Dec  6 16:20:10.545274  jlink:       triceExamples.c    61              16334 TRICE_DIRECT_OUTPUT == 1, TRICE_DEFERRED_OUTPUT == 0
Dec  6 16:20:10.545274  jlink:       triceExamples.c    63              16333 TRICE_STACK_BUFFER, TRICE_MULTI_PACK_MODE
Dec  6 16:20:10.545890  jlink:       triceExamples.c    76              16327 _CYCLE == 1, _PROTECT == 0, _DIAG == 0, XTEA == 0
Dec  6 16:20:10.546396  jlink:       triceExamples.c    77              16326 _SINGLE_MAX_SIZE=104, _BUFFER_SIZE=172, _DEFERRED_BUFFER_SIZE=1024
Dec  6 16:20:11.448885  jlink:       triceExamples.c    29    0,031_859 16356 🐁 Speedy Gonzales a  32-bit time stamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    30    0,031_661 16355 🐁 Speedy Gonzales b  32-bit time stamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    31    0,031_463 16354 🐁 Speedy Gonzales c  32-bit time stamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    32    0,031_265 16353 🐁 Speedy Gonzales d  32-bit time stamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    33       31_067 16352 🐁 Speedy Gonzales e  16-bit time stamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    34       30_867 16351 🐁 Speedy Gonzales f  16-bit time stamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    35       30_667 16350 🐁 Speedy Gonzales g  16-bit time stamp
Dec  6 16:20:11.448885  jlink:       triceExamples.c    36       30_467 16349 🐁 Speedy Gonzales h  16-bit time stamp
Dec  6 16:20:11.549660  jlink:       triceExamples.c    42       29_961 16344 2.71828182845904523536 <- float number as string
Dec  6 16:20:11.549660  jlink:       triceExamples.c    43       29_141 16343 2.71828182845904509080 (double with more ciphers than precision)
Dec  6 16:20:11.549660  jlink:       triceExamples.c    44       28_897 16342 2.71828174591064453125 (float  with more ciphers than precision)
Dec  6 16:20:11.549660  jlink:       triceExamples.c    45       28_675 16341 2.718282 (default rounded float)
Dec  6 16:20:11.549660  jlink:       triceExamples.c    46       28_452 16340 A Buffer:
Dec  6 16:20:11.550166  jlink:       triceExamples.c    47       28_238 16339 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
Dec  6 16:20:11.550247  jlink:       triceExamples.c    48       27_412 16338 31372e32  31383238  34383238  34303935  35333235
Dec  6 16:20:11.550247  jlink:       triceExamples.c    49       26_671 16337 ARemoteFunctionName(2e32)(3137)(3238)(3138)(3238)(3438)(3935)(3430)(3235)(3533)(3633)
Dec  6 16:20:11.550247  jlink:       triceExamples.c    50              16200 5 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
Dec  6 16:20:11.550754  jlink:       triceExamples.c    52       25_646 16335 i=44444400 aaaaaa00
Dec  6 16:20:11.550754  jlink:       triceExamples.c    52       25_389 16335 i=44444401 aaaaaa01
Dec  6 16:20:11.550754  jlink:       triceExamples.c    52       25_132 16335 i=44444402 aaaaaa02
Dec  6 16:20:11.551285  jlink:       triceExamples.c    52       24_875 16335 i=44444403 aaaaaa03
Dec  6 16:20:11.551285  jlink:       triceExamples.c    52       24_618 16335 i=44444404 aaaaaa04
Dec  6 16:20:12.453968  jlink:       triceExamples.c    29    0,031_859 16356 🐁 Speedy Gonzales a  32-bit time stamp
Dec  6 16:20:12.453968  jlink:       triceExamples.c    30    0,031_661 16355 🐁 Speedy Gonzales b  32-bit time stamp
```

"🐁 Speedy Gonzales" direct outout needs about 200 MCU clocks and not 500 as before.

###  27.7. <a id='settings-conclusion'></a>Settings Conclusion

* 4-8 KB Flash and 1.2 KB RAM needed for the Trice library.
* The RAM size is mainly influenced by the configured buffer sizes.
* Switching off diagnostics and/or protection is ok for less memory needs and faster Trice execution after getting some experience with the project.

###  27.8. <a id='legacy-trice-space-example-(old-version)'></a>Legacy Trice Space Example (Old Version)

* STM32CubeMX generated empty default project: `Program Size: Code=2208 RO-data=236 RW-data=4 ZI-data=1636`
* Same project with default `Trice` instrumentation: `Program Size: Code=2828 RO-data=236 RW-data=44 ZI-data=1836`
* Needed [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory): 620 Bytes
* Needed [RAM](https://en.wikipedia.org/wiki/Random-access_memory): 40 Bytes plus 200 Bytes for the 2 times 100 Bytes double buffer
* With increased/decreased buffers also more/less [RAM](https://en.wikipedia.org/wiki/Random-access_memory) is needed.
* With each additional Trice macro a few additional [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory) bytes, like 10 assembler instructions, are needed.
* No printf-like library code is used anymore.
* No format strings go into the target code anymore.
* In general Trice instrumentation **reduces** the needed memory compared to a printf-like implementation.

###  27.9. <a id='memory-needs-for-old-example-1'></a>Memory Needs for Old Example 1

The following numbers are measured with a legacy encoding, showing that the instrumentation code can be even smaller.

| Program Size (STM32-F030R8 demo project)      | trice instrumentation | buffer size | compiler optimize for time | comment                         |
|-----------------------------------------------|-----------------------|-------------|----------------------------|---------------------------------|
| Code=1592 RO-data=236 RW-data= 4 ZI-data=1028 | none                  | 0           | off                        | CubeMX generated, no trice      |
| Code=1712 RO-data=240 RW-data=24 ZI-data=1088 | core                  | 64          | off                        | core added without trices       |
| Code=3208 RO-data=240 RW-data=36 ZI-data=1540 | TriceCheckSet()       | 512         | off                        | TRICE_SHORT_MEMORY is 1 (small) |
| Code=3808 RO-data=240 RW-data=36 ZI-data=1540 | TriceCheckSet()       | 512         | on                         | TRICE_SHORT_MEMORY is 0 (fast)  |

* The core instrumentation needs less 150 bytes FLASH and about 100 bytes RAM when buffer size is 64 bytes.
* The about 50 trices in TriceCheckSet() allocate roughly 2100 (fast mode) or 1500 (small mode) bytes.
* trices are removable without code changes with `#define TRICE_OFF 1` before `incude "trice.h"` on file level or generally on project level.

###  27.10. <a id='memory-needs-for-old-example-2'></a>Memory Needs for Old Example 2

| Project                        | Compiler    | Optimization | Link-Time-Optimization | Result                                        | Remark                                                             |
|--------------------------------|-------------|--------------|------------------------|-----------------------------------------------|--------------------------------------------------------------------|
| MDK-ARM_STM32F030_bareerated   | CLANG v6.19 | -Oz          | yes                    | Code=1020 RO-data=196 RW-data=0 ZI-data=1024  | This is the plain generated project without trice instrumentation. |
| MDK-ARM_STM32F030_instrumented | CLANG v6.19 | -Oz          | yes                    | Code=4726 RO-data=238 RW-data=16 ZI-data=4608 | This is with full trice instrumentation with example messages.     |

* The size need is less than 4 KB. See also [Trice Project Image Size Optimization](#trice-project-image-size-optimization).

<p align="right">(<a href="#top">back to top</a>)</p>

##  28. <a id='trice-project-image-size-optimization'></a>Trice Project Image Size Optimization

Modern compilers are optimizing out unused code automatically, but you can help to reduce trice code size if your compiler is not perfect.

###  28.1. <a id='code-optimization--o3-or--oz-(if-supported)'></a>Code Optimization -o3 or -oz (if supported)

For debugging it could be helpful to switch off code optimization what increases the code size. A good choice is `-o1`. See also
[TRICE_STACK_BUFFER could cause stack overflow with -o0 optimization](#trice_stack_buffer-could-cause-stack-overflow-with--o0-optimization).


###  28.2. <a id='compiler-independent-setting-(a-bit-outdated)'></a>Compiler Independent Setting (a bit outdated)

Maybe the following is a bit unhandy but it decreases the code amount, build time and the image size.

* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_trice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.
* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_Trice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.
* For **X=8|16|32|64** and **N=0...12** selectively set `#define ENABLE_TRice`**X**`fn_`**N**` 1` to ` 0` for unused functions in project specific file `triceConfig.h`.

When having lots of program memory simply let all values be `1`. With specific linker optimization unused functions can get stripped out automatically.

It is possible to `#define TRICE_SINGLE_MAX_SIZE 12` for example in *triceConfig.h*. This automaticaly disables all Trice messages with payloads > 8 bytes (Trice size is 4 bytes).

###  28.3. <a id='linker-option---split-sections-(if-supported)'></a>Linker Option --split-sections (if supported)

In ARM-MDK uVision `Project -> Options -> C/C++ -> "One EFL section for each function"` allows good optimization and getting rid of unused code without additional linker optimization. This leads to a faster build process and is fine for most cases. It allows excluding unused functions.

###  28.4. <a id='linker-optimization--flto-(if-supported)'></a>Linker Optimization -flto (if supported)

* To get the smallest possible image, do _not_ use option `--split sections`.
* Use linker optimization alone.
* This increases the build time but reduces the image size significantly.

####  28.4.1. <a id='armcc-compiler-v5-linker-feedback'></a>ARMCC Compiler v5 Linker Feedback

* In ARM-MDK uVision, when using ARMCC compiler v5, there is a check box `Project -> Options -> Target -> "Cross Module Optimization"`.
* In ARMCC this works also with the lite version.

####  28.4.2. <a id='armclang-compiler-v6-link-time-optimization'></a>ARMCLANG Compiler v6 Link-Time Optimization

* In ARM-MDK uVision, when using ARMCLANG compiler v6, the check box `Project -> Options -> C/C++(AC6) -> "Link-Time Optimization"` is usable to set the CLI `-flto` switch.
* LTO is not possible with ARMCLANG6 lite: https://developer.arm.com/documentation/ka004054/latest.

####  28.4.3. <a id='gcc'></a>GCC

With GCC use the `-flto` CLI switch directly.

####  28.4.4. <a id='llvm-arm-clang'></a>LLVM ARM Clang

This compiler is much faster and creates the smallest images. Right now it uses the GCC libs and linker.

####  28.4.5. <a id='other-ide´s-and-compilers'></a>Other IDE´s and compilers

Please check the manuals and create a pull request or simply let me know.

###  28.5. <a id='legacy-stm32f030-example-project---different-build-sizes'></a>Legacy STM32F030 Example Project - Different Build Sizes

####  28.5.1. <a id='armcc-compiler-v5'></a>ARMCC compiler v5

| Compiler | Linker         | Result                                          | Comment                           |
|----------|----------------|-------------------------------------------------|-----------------------------------|
| o0       |                | Code=46942 RO-data=266 RW-data=176 ZI-data=4896 | very big                          |
| o1       |                | Code=22582 RO-data=258 RW-data=168 ZI-data=4896 |                                   |
| o3       |                | Code=21646 RO-data=258 RW-data=168 ZI-data=4896 |                                   |
| o0       | split sections | Code= 7880 RO-data=268 RW-data=156 ZI-data=4892 | for debugging                     |
| o1       | split sections | Code= 5404 RO-data=260 RW-data=148 ZI-data=4892 | **for debugging**                 |
| o3       | split sections | Code= 4996 RO-data=260 RW-data=148 ZI-data=4892 | **good balance**                  |
| o0       | flto           | Code= 8150 RO-data=266 RW-data=176 ZI-data=4896 | builds slower                     |
| o1       | flto           | Code= 5210 RO-data=258 RW-data=148 ZI-data=4892 | builds slower                     |
| o3       | flto           | Code= 4818 RO-data=258 RW-data=148 ZI-data=4892 | builds slower, **smallest image** |

<p align="right">(<a href="#top">back to top</a>)</p>

##  29. <a id='trice-tags-and-color'></a>Trice Tags and Color

###  29.1. <a id='how-to-get'></a>How to get

* Add a tag name as color descriptor in front of each Trice format string like `"wrn:Peng!"`.
* In file [../internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/main/internal/emitter/lineTransformerANSI.go) the colors are changeable and additional color tags definable.
* It is possible to concatenate single colorized letters to get output like this:

![./ref/COLOR_output.PNG](./ref/COLOR_output.PNG)

* [../_test/testdata/triceCheck.c](https://github.com/rokath/trice/blob/main/_test/testdata/triceCheck.c) contains the code for this example.
* The Trice tool, if knowing `wrn:` as pattern, prepends the appropriate color code. It removes the sequence `wrn:`, if it is known and completely lower case.
  * The Trice tool will strip full lowercase tag descriptors from the format string after setting the appropriate color, making it possible to give even each letter in a message its color.

    `"wrn:fox"` will display colored "fox"
    `"Wrn:fox"` will display colored "Wrn:fox"

* The user can define any pattern with any color code to create colored output with the Trice tool.
* There is no tag enable switch inside the target code. It would need a back channel and add overhead.
* An option using tag specific ID ranges with optional routing exists.
* The Trice tool offers the 2 command line switches `-pick` and `-ban` to control tag visualization during runtime.

####  29.1.1. <a id='output-options'></a>Output options

![./ref/ColorOptions.PNG](./ref/ColorOptions.PNG)

####  29.1.2. <a id='check-alternatives'></a>Check Alternatives

There are over 1000 possibilities:

![./ref/ColorAlternatives.PNG](./ref/ColorAlternatives.PNG)

To see them all run `trice generate -color`. Only file [../internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/main/internal/emitter/lineTransformerANSI.go) needs to be changed and the Trice tool needs to be rebuild afterwards: `go install ./...`. If you design a good looking flavour, feel free to propose it. 

###  29.2. <a id='color-issues-under-windows'></a>Color issues under Windows

**Currently console colors are not enabled by default in Win10**, so if you see no color but escape sequences on your powershell or cmd window, please refer to [Windows console with ANSI colors handling](https://superuser.com/questions/413073/windows-console-with-ansi-colors-handling/1050078#1050078) or simply use a Linux like terminal under windows, like git-bash. One option is also to install Microsoft *Windows Terminal (Preview)* from inside the Microsoft store and to start the Trice tool inside there. Unfortunately this can not be done automatically right now because of missing command line switches. [Alacritty](https://github.com/rokath/trice/blob/main/third_party/alacritty/ReadMe.md) is one of other alternatives.

<p align="right">(<a href="#top">back to top</a>)</p>

##  30. <a id='trice-without-uart'></a>Trice without UART

A very performant output path is RTT, if your MCU supports background memory access like the ARM-M ones.

Because the Trice tool needs only to receive, a single target UART-TX pin will do. But it is also possible to use a GPIO-Pin for Trice messages without occupying a UART resource.

* This slow path is usable because a Trice needs only few bytes for transmission.
* You can transmit each basic trice (4 or 8 bytes) as bare message over one pin:

  <img src="./ref/manchester1.PNG" width="400">
  <img src="./ref/manchester2.PNG" width="480">

* The 2 images are taken from [https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/). See there for more information.
* As Trice dongle you can use any spare MCU board with an UART together with an FTDI USB converter.
  * This allowes also any other data path - method does'nt matter:\
  [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter),\
  [I²C](https://en.wikipedia.org/wiki/I%C2%B2C),\
  [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface),\
  [GPIO](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/),\
  [CAN](https://en.wikipedia.org/wiki/CAN_bus),\
  [LIN](https://en.wikipedia.org/wiki/Local_Interconnect_Network), ...
* [RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) is also a possible path to use - see [Trice over RTT](#trice-over-rtt) for options.

<p align="right">(<a href="#top">back to top</a>)</p>


##  31. <a id='trice-over-rtt'></a>Trice over RTT

> Allows Trice over the debug probe without using a pin or UART.

* RTT works good with a SEGGER J-Link debug probe but needs some closed source software components.
* Also ST-Link is usable for Trice logs, but maybe not parallel with debugging.
* Most investigations where done with a [NUCLEO64-STM32F030R8 evaluation board](https://www.st.com/en/evaluation-tools/nucleo-F030r8.html) which contains an on-board debug probe reflashed with a SEGGER J-Link OB software (see below).
  * When using very high Trice loads over RTT for a long time, sometimes an on-board J-Link (re-flashed ST-Link) could get internally into an inconsistent state (probably internal buffer overrun), what needs a power cycle then.
* You could consider RTT over open-OCD as an alternative.
* The default SEGGER up-buffer size is 1024 bytes, good for most cases. If not, adapt it in your *triceConfig.h* file **AND** in the *SEGGER_RTT_Conf.h* file:
  You need only one up-channel for Trice:

  ```C
  #define BUFFER_SIZE_UP (128)  // "TRICE_DIRECT_BUFFER_SIZE"
  ```
* Inside the [triceDefaultConfig.h](https://github.com/rokath/trice/blob/main/src/triceDefaultConfig.h) you can find some other settings recommended for the *SEGGER_RTT_Conf.h* file. You have to set them manually in the *SEGGER_RTT_Conf.h* because the SEGGER target sources do not include *trice.h* (and implicit [triceDefaultConfig.h](https://github.com/rokath/trice/blob/main/src/triceDefaultConfig.h) and *triceConfig.h*).
* **Possible:** Parallel usage of RTT direct mode with UART deferred mode. You can define `TRICE_UARTA_MIN_ID` and `TRICE_UARTA_MAX_ID` inside triceConfig.h to log only a specific ID range over UARTA in deferred mode for example. ([\#446](https://github.com/rokath/trice/issues/446))

<p align="right">(<a href="#top">back to top</a>)</p>

###  31.1. <a id='for-the-impatient-(2-possibilities)'></a>For the impatient (2 possibilities)

The default SEGGER tools only suport RTT channel 0.

####  31.1.1. <a id='start-jlink-commander-and-connect-over-tcp'></a>Start JLink commander and connect over TCP

* JLink.exe → `connect ⏎ ⏎ S ⏎` and keep it active.
  * You can control the target with `r[eset], g[o], h[alt]` and use other commands too.
  * ![./ref/JLink.exe.PNG](./ref/JLink.exe.PNG)
* Start in Git-Bash or s.th. similar: `trice l -p TCP4 -args localhost:19021`
* You may need a Trice tool restart after firmware reload.


<a id='setup-tcp4-server-providing-the-trace-data'></a><h5>Setup TCP4 server providing the trace data</h5>

This is just the SEGGER J-Link server here for demonstration, but if your target device has an TCP4 interface, you can replace this with your target server.

```bash
ms@DESKTOP-7POEGPB MINGW64 ~/repos/trice (main)
$ jlink
SEGGER J-Link Commander V7.92g (Compiled Sep 27 2023 15:36:46)
DLL version V7.92g, compiled Sep 27 2023 15:35:10

Connecting to J-Link via USB...O.K.
Firmware: J-Link STLink V21 compiled Aug 12 2019 10:29:20
Hardware version: V1.00
J-Link uptime (since boot): N/A (Not supported by this model)
S/N: 770806762
VTref=3.300V


Type "connect" to establish a target connection, '?' for help
J-Link>connect
Please specify device / core. <Default>: STM32G0B1RE
Type '?' for selection dialog
Device>
Please specify target interface:
  J) JTAG (Default)
  S) SWD
  T) cJTAG
TIF>s
Specify target interface speed [kHz]. <Default>: 4000 kHz
Speed>
Device "STM32G0B1RE" selected.


Connecting to target via SWD
InitTarget() start
SWD selected. Executing JTAG -> SWD switching sequence.
DAP initialized successfully.
InitTarget() end - Took 36.3ms
Found SW-DP with ID 0x0BC11477
DPv0 detected
CoreSight SoC-400 or earlier
Scanning AP map to find all available APs
AP[1]: Stopped AP scan as end of AP map has been reached
AP[0]: AHB-AP (IDR: 0x04770031)
Iterating through AP map to find AHB-AP to use
AP[0]: Core found
AP[0]: AHB-AP ROM base: 0xF0000000
CPUID register: 0x410CC601. Implementer code: 0x41 (ARM)
Found Cortex-M0 r0p1, Little endian.
FPUnit: 4 code (BP) slots and 0 literal slots
CoreSight components:
ROMTbl[0] @ F0000000
[0][0]: E00FF000 CID B105100D PID 000BB4C0 ROM Table
ROMTbl[1] @ E00FF000
[1][0]: E000E000 CID B105E00D PID 000BB008 SCS
[1][1]: E0001000 CID B105E00D PID 000BB00A DWT
[1][2]: E0002000 CID B105E00D PID 000BB00B FPB
Memory zones:
  Zone: "Default" Description: Default access mode
Cortex-M0 identified.
J-Link>
```

Now the TCP4 server is running and you can start the Trice tool as TCP4 client, which connects to the TCP4 server to receive the binary log data:

```bash
$ trice l -p TCP4 -args="127.0.0.1:19021" -til ../examples/G0B1_inst/til.json -li ../examples/G0B1_inst/li.json -d16 -pf none
```

In this **G0B1_inst** example we use the additional `-d16` and `-pf none` switches to decode the RTT data correctly.

**This is a demonstration and test for the `-port TCP4` usage possibility**. Using RTT with J-Link is more easy possible as shown in the next point.

####  31.1.2. <a id='start-using-jlinkrttlogger'></a>Start using JLinkRTTLogger

* Start inside Git-Bash or s.th. similar: `trice l -p JLINK -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0"`
  * Replace CLI details with your settings.
  * For **G0B1_inst**: `trice l -p JLINK -args "-Device STM32G0B1RE -if SWD -Speed 4000 -RTTChannel 0" -d16 -pf none`
  * You can add the `-verbose` CLI switch for more details.
* You may **not** need a Trice tool restart after firmware reload.

####  31.1.3. <a id='jlinkrttlogger-issue'></a>JLinkRTTLogger Issue

* For some reason the RTT technique does not work well with Darwin (MacOS) and also Linux right now. The problem seems to be that the JLinkRTTLogger app cannot work correctly in the background. But there is a workaround:
  * Example 1:
    * In one terminal run `JLinkRTTLogger -Device STM32G0B1RE -if SWD -Speed 4000 -RTTChannel 0 myLogFile.bin`
    * and in an other terminal execute `trice l -p FILE -args myLogFile.bin -pf none -d16`.
  * Example 2:
    * Flash, start debugger and run to main()
    * Terminal 1: `rm ./temp/trice.bin && JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin`
    * Terminal 2: `touch ./temp/trice.bin && trice log -p FILE -args ./temp/trice.bin -prefix off -hs off -d16 -ts ms -i ../../demoTIL.json -li ../../demoLI.json -pf none`
    * Continue to run in debugger
    * Terminal 1:
    
      ```bash
      th@P51-DebianKDE:~/repos/trice/examples/G0B1_inst$ rm ./temp/trice.bin && JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin
      SEGGER J-Link RTT Logger
      Compiled Dec 18 2024 15:48:21
      (c) 2016-2017 SEGGER Microcontroller GmbH, www.segger.com
              Solutions for real time microcontroller applications

      Default logfile path: /home/th/.config/SEGGER

      ------------------------------------------------------------ 


      ------------------------------------------------------------ 
      Connected to:
        SEGGER J-Link ST-LINK
        S/N: 779220206

      Searching for RTT Control Block...OK.
      1 up-channels found:
      0: Terminal
      Selected RTT Channel description: 
        Index: 0
        Name:  Terminal
        Size:  1024 bytes.

      Output file: ./temp/trice.bin

      Getting RTT data from target. Press any key to quit.
      ------------------------------------------------------------ 

      Transfer rate: 0 Bytes/s Data written: 15.71 KB
      ```

    * Terminal 2:
    
      ```bash
      th@P51-DebianKDE:~/repos/trice/examples/G0B1_inst$ touch ./temp/trice.bin && trice log -p FILE -args ./temp/trice.bin -prefix off -hs off -d16 -ts ms  -i ../../demoTIL.json -li ../../demoLI.json -pf none 
            triceExamples.c    12        0,000  Hello! 👋🙂
              ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨        
              🎈🎈🎈🎈  NUCLEO-G0B1RE   🎈🎈🎈🎈        
              🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃        


            triceExamples.c    61              TRICE_DIRECT_OUTPUT == 1, TRICE_DEFERRED_OUTPUT == 1
            triceExamples.c    69              TRICE_RING_BUFFER, TRICE_MULTI_PACK_MODE
            triceExamples.c    76              _CYCLE == 1, _PROTECT == 1, _DIAG == 1, XTEA == 1
            triceExamples.c    77              _SINGLE_MAX_SIZE=104, _BUFFER_SIZE=172, _DEFERRED_BUFFER_SIZE=2000
            triceExamples.c    29  0:00:00,003 🐁 Speedy Gonzales a  32-bit time stamp
            triceExamples.c    30  0:00:00,003 🐁 Speedy Gonzales b  32-bit time stamp
            triceExamples.c    31  0:00:00,003 🐁 Speedy Gonzales c  32-bit time stamp
            triceExamples.c    32  0:00:00,003 🐁 Speedy Gonzales d  32-bit time stamp
            triceExamples.c    33        0,310 🐁 Speedy Gonzales e  16-bit time stamp
            triceExamples.c    34        0,328 🐁 Speedy Gonzales f  16-bit time stamp
            triceExamples.c    35        0,347 🐁 Speedy Gonzales g  16-bit time stamp
            triceExamples.c    36        0,365 🐁 Speedy Gonzales h  16-bit time stamp
            triceExamples.c    42        0,394 2.71828182845904523536 <- float number as string
            triceExamples.c    43        0,436 2.71828182845904509080 (double with more ciphers than precision)
            triceExamples.c    44        0,458 2.71828174591064453125 (float  with more ciphers than precision)
            triceExamples.c    45        0,479 2.718282 (default rounded float)
            triceExamples.c    46        0,500 A Buffer:
            triceExamples.c    47        0,520 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36 
            triceExamples.c    48        0,562 31372e32  31383238  34383238  34303935  35333235  
            triceExamples.c    49        0,601 ARemoteFunctionName(2e32)(3137)(3238)(3138)(3238)(3438)(3935)(3430)(3235)(3533)(3633)
            triceExamples.c    50              3 times a 16 byte long Trice messages, which may not be written all if the buffer is too small:
            triceExamples.c    52        0,664 i=44444400 aaaaaa00
            triceExamples.c    52        0,687 i=44444401 aaaaaa01
            triceExamples.c    52        0,709 i=44444402 aaaaaa02
                    main.c   312  0:00:00,003 StartDefaultTask
                    main.c   339  0:00:00,003 StartTask02:Diagnostics and TriceTransfer
        triceLogDiagData.c    21              RTT0_writeDepthMax=365 (BUFFER_SIZE_UP=1024)
        triceLogDiagData.c    44              triceSingleDepthMax =  96 of 172 (TRICE_BUFFER_SIZE)
        triceLogDiagData.c    75              triceRingBufferDepthMax =   0 of 2000
              triceCheck.c    57               line 57
              triceCheck.c    59  0:00:02,325 Hello World!
              triceCheck.c    61  0:00:02,405 This is a message without values and a 32-bit stamp.
              triceCheck.c    62        0,306 This is a message without values and a 16-bit stamp.
              triceCheck.c    63              This is a message without values and without stamp.
      ```
      * See also the configuration in [./examples/G0B1_inst/Core/Inc/triceConfig.h](https://github.com/rokath/trice/blob/main/examples/G0B1_inst/Core/Inc/triceConfig.h)

* If you install the `tmux` command your life gets esier by using a shell script like [./examples/G0B1_inst/RTTLogTmux.sh](https://github.com/rokath/trice/blob/main/examples/G0B1_inst/RTTLogTmux.sh):

```bash
mkdir -p ./temp
rm -f ./temp/trice.bin
touch ./temp/trice.bin
tmux new -s "tricerttlog" -d "JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin"
trice log -p FILE -args ./temp/trice.bin -pf none -prefix off -hs off -d16 -ts16 "time:offs:%4d µs" -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json -stat
tmux kill-session -t "tricerttlog"
```

  * Usage:

    ```bash
    th@PaulPCdeb128KDE:~/repos/trice/examples/G0B1_inst$ ./RTTLogUnix.sh 
        triceExamples.c    12        0,000  Hello! 👋🙂
          ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨        
          🎈🎈🎈🎈  NUCLEO-G0B1RE   🎈🎈🎈🎈
          🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃        


        triceExamples.c    61              TRICE_DIRECT_OUTPUT == 1, TRICE_DEFERRED_OUTPUT == 1
        triceExamples.c    69              TRICE_RING_BUFFER, TRICE_MULTI_PACK_MODE
        triceExamples.c    76              _CYCLE == 1, _PROTECT == 1, _DIAG == 1, XTEA == 1
        triceExamples.c    77              _SINGLE_MAX_SIZE=104, _BUFFER_SIZE=172, _DEFERRED_BUFFER_SIZE=2000
        triceExamples.c    29  0:00:00,002 🐁 Speedy Gonzales a  32-bit time stamp
    ```

* **Hint:** If you use *RTTLogTmux.sh* with Darwin (MacOS), the "control-C" key combination seems not to work immediately. That is simply because the keyboard focus switches away after script start. Simply click into the terminal window again and then use "control-C" to terminate the Trice logging.

<p align="right">(<a href="#top">back to top</a>)</p>

###  31.2. <a id='segger-real-time-transfer-(rtt)'></a>Segger Real Time Transfer (RTT)

* Prerequisite is a processor with memory background access support like ARM Cortex-M cores.
* If you can use a Segger J-Link or an STM ST-Link debug probe (ST Microelectronics eval boards have it) this is an easy and fast way to use Trice without any UART or other port.
* Detailed description can be found in document [UM08001_JLink.pdf](https://github.com/rokath/trice/blob/main/third_party/segger.com/UM08001_JLink.pdf) in chapter 16 which is part of [https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack).
* Following examples are for Windows, but should work similar also on Linux and Darwin (MacOS).
* Trice can use the Segger RTT protocol in different ways.
  * Hardware paths:
    * Use [J-Link](https://www.segger.com/products/debug-probes/j-link/) or [J-Link OB (on-board)](https://www.segger.com/products/debug-probes/j-link/models/j-link-ob/).
      J-Link OB can be flashed to many ST Microelectronics evaluation boards (v2.0 link hardware) and for example is also usable with NXP and Atmel. For that you can also use a spare STM32 evaluation board (10 EUR) with jumper changes and breakout wires.
    * Use ST-Link with [gostlink](https://github.com/rokath/trice/blob/main/third_party/goST/ReadMe.md).
      It uses only one USB endpoint so debugging and Trice output in parallel is not possible.
    * Use some other Debug-Probe with target memory access (support welcome)
  * RTT channel selection (on target and on host)
    * RECOMMENDED:
      * `trice l -p JLINK` or shorter `trice l` for STM32F030R8 (default port is JLINK) starts in background a `JLinkRTTLogger.exe` which connects to J-Link and writes to a logfile which in turn is read by the Trice tool. On exit the `JLinkRTTLogger.exe` is killed automatically.
        * It expects a target sending messages over RTT channel **0** (zero). Chapter 16.3.3 in [UM08001_JLink.pdf](https://github.com/rokath/trice/blob/main/third_party/segger.com/UM08001_JLink.pdf) refers to "Up-Channel 1" but this maybe is a typo and probably a 0 is mend. The `JLinkRTTLogger.exe` main advantage against other free available SEGGER tools is, that all bytes are transferred. Other SEGGER tools assume ASCII characters and use `FF 00` to `FF 0F` as a terminal switch command and filter that out causing Trice data disturbances.
        * It should be possible to start several instances on on different targets using `-SelectEmuBySN <SN>` inside the `-args` Trice CLI switch.
        * `JLinkRTTLogger` binaries for Linux & Darwin (MacOS) can be found at [https://www.segger.com/downloads/jlink/](https://www.segger.com/downloads/jlink/).
      * `trice l -p STLINK` starts in background a `trice/third_party/goST/stRttLogger.exe` which connects to ST-Link and writes to a logfile which in turn is read by the Trice tool. On exit the `stRttLogger.exe` is killed automatically. It expects a target sending messages over RTT channel 0 (other channels supported too but may not work).\
      It is possible to start several instances on different channels as well as on different targets. The source code is in [https://github.com/bbnote/gostlink](https://github.com/bbnote/gostlink) and should work also at least under Linux.
    * If you have the choice, prefer J-Link. It allows parallel debugging and Trice output.
    * The full `-args` string is normally required and depends on the used device. Example: `trice l -args="-Device STM32F070RB -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"`. The `-RTTSearchRanges` part is mostly optional.
    * Enter `trice h -log` and read info for `-args` switch:

```bash
        -args string
        Use to pass port specific parameters. The "default" value depends on the used port:
        port "COMn": default="", use "TARM" for a different driver. (For baud rate settings see -baud.)
        port "J-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000",
                The -RTTSearchRanges "..." need to be written without extra "" and with _ instead of space.
                For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
        port "ST-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000",
                The -RTTSearchRanges "..." need to be written without extra "" and with _ instead of space.
                For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
        port "BUFFER": default="0 0 0 0", Option for args is any byte sequence.
         (default "default")
 ```

<p align="right">(<a href="#top">back to top</a>)</p>

###  31.3. <a id='j-link-option'></a>J-Link option

* Prerequisite is a SEGGER J-Link debug probe or a development board with an on-board J-Link option.

####  31.3.1. <a id='convert-evaluation-board-onboard-st-link-to-j-link'></a>Convert Evaluation Board onboard ST-Link to J-Link

* Following steps describe the needed action for a ST Microelectronics evaluation board and windows - adapt them to your environment.
* It is always possible to turn back to the ST-Link OB firmware with the SEGGER `STLinkReflash.exe` tool but afterwards the ST-Link Upgrade tool should be used again to get the latest version.

<a id='first-step-(to-do-if-some-issues-occur---otherwise-you-can-skip-it)'></a><h5>First step (to do if some issues occur - otherwise you can skip it)</h5>

[Video](https://www.youtube.com/watch?app=desktop&v=g2Kf6RbdrIs)

See also [https://github.com/stlink-org/stlink](https://github.com/stlink-org/stlink)

* Get & install [STM32 ST-LINK utility](https://www.st.com/en/development-tools/stsw-link004.html)
* Run from default install location `"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINKUtility\ST-LINK Utility\ST-LinkUpgrade.exe"`)
* Enable checkbox `Change Type` and select radio button `STM32 Debug+Mass storage + VCP`. *The `STM32Debug+ VCP` won´t be detected by Segger reflash utility.*
  ![ST-LINK-Upgrade.PNG](./ref/ST-LINK-Upgrade.PNG)

<a id='second-step'></a><h5>Second step</h5>

* Check [Converting ST-LINK On-Board Into a J-Link](https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/)
* Use `STLinkReflash.exe` to convert NUCLEO from ST-Link on-board to J-Link on-board. *`STM32 Debug+ VCP` won´t be detected by Segger reflash utility.*

####  31.3.2. <a id='some-segger-tools-in-short'></a>Some SEGGER tools in short

* Download [J-Link Software and Documentation Pack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack) and install.
  * You may need to add `C:\Program Files\SEGGER\JLink` to the %PATH% variable.
* Tested with [NUCLEO64-STM32F030R8 evaluation board](https://www.st.com/en/evaluation-tools/nucleo-F030r8.html).
* For example: Compile and flash `../examples/F030_inst` project.
  * Check in [../examples/F030_inst/Core/Inc/triceConfig.h](https://github.com/rokath/trice/blob/main/examples/F030_inst/Core/Inc/triceConfig.h) if `#define TRICE_RTT_CHANNEL 0` is set as output option.

<a id='jlink.exe'></a><h5>JLink.exe</h5>

* `JLink.exe` is the SEGGER J-Link commander. It starts the **J-Link driver/server** and one can connect to it
* Info found [here](https://gist.github.com/GaryLee/ecd8018d1ca046c1a40fcd265fa109c0):
  * J-Link Commander can be started with different command line options for test and automation
  * purposes. In the following, the command line options which are available for J-Link
  * Commander are explained. All command line options are case insensitive.
  * Command Explanation
  * -AutoConnect Automatically start the target connect sequence
  * -CommanderScript Passes a CommandFile to J-Link
  * -CommandFile Passes a CommandFile to J-Link
  * -Device Pre-selects the device J-Link Commander shall connect to
  * -ExitOnError Commander exits after error.
  * -If Pre-selects the target interface
  * -IP Selects IP as host interface
  * -JLinkScriptFile Passes a JLinkScriptFile to J-Link
  * -JTAGConf Sets IRPre and DRPre
  * -Log Sets logfile path
  * -RTTTelnetPort Sets the RTT Telnetport
  * -SelectEmuBySN Connects to a J-Link with a specific S/N over USB
  * -SettingsFile Passes a SettingsFile to J-Link
  * -Speed Starts J-Link Commander with a given initial speed
* Documentation: [https://wiki.segger.com/J-Link_Commander](https://wiki.segger.com/J-Link_Commander)
* If you run successful `jlink -device STM32F030R8 -if SWD -speed 4000 -autoconnect 1` the target is stopped.
  * To let in run you need manually execute `go` as command in the open jlink window.
  * To automate that create a text file named for example `jlink.go` containing the `go` command: `echo go > jlink.go` and do a `jlink -device STM32F030R8 -if SWD -speed 4000 -autoconnect 1 -CommandFile jlink.go`
* It is possible to see some output with Firefox (but not with Chrome?) for example: ![./ref/JLink19021.PNG](./ref/JLink19021.PNG).
* After closing the Firefox the Trice tool can connect to it too:
  * Open a commandline and run:
    ```b
    trice log -port TCP4 -args localhost:19021
    ```
    * Trice output is visible
    * With `h`alt and `g`o inside the Jlink window the MCU can get haltes and released
    * It is possible in parallel to debug-step with a debugger (tested with ARM-MDK)
* ![./ref/JLinkServer.PNG](./ref/JLinkServer.PNG)
* **PLUS:**
  * Works reliable.
  * No file interface needed.
  * Trice can connect over TCP localhost:19021 and display logs over RTT channel 0.
  * The open `jlink` CLI can be handy to control the target: `[r]eset, [g]o. [h]alt`
  * No need to restart the Trice tool after changed firmware download.
* **MINUS:**
  * Uses RTT up-channel 0 and therefore RTT up-channel 0 is not usable differently.
  * No down-channel usable.
  * Needs a separate manual start of the `jlink` binary with CLI parameters.
    * I would not recommend to automate that too, because this step is needed only once after PC power on.

<a id='jlinkrttlogger.exe'></a><h5>JLinkRTTLogger.exe</h5>

* `JLinkRTTLogger.exe` is a CLI tool and connects via the SEGGER API to the target. It is usable for writing RTT channel 0 data from target into a file.
* **PLUS:**
  * Works reliable.
  * Is automatable.
  * Create file with raw log data: `JLinkRTTLogger.exe -Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 triceRaw.log`
    * It is possible to evaluate this file offline: `trice l -p FILE -args triceRaw.log`
    * ![./ref/TriceFILE.PNG](./ref/TriceFILE.PNG)
  * No need to restart the Trice tool after changed firmware download.
* **MINUS:**
  * Logs in a file, so the Trice tool needs to read from that file.
  * Maybe cannot write in a file as background process on Darwin (MacOS).
* The Trice tool can watch the output file and display the *Trices*: `trice log -port JLINK -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0"
![./ref/JlinkLoggerTrice.PNG](./ref/JlinkLoggerTrice.PNG)

####  31.3.3. <a id='jlinkrttclient.exe'></a>JLinkRTTClient.exe

* `JLinkRTTClient.exe` can be used for simple text transmitting to the target, it also displays strings from target coming over channel 0. It is not used by the Trice tool.
  * **PLUS:**
    * Target stimulation with proprietary protocol over RTT down-channel 0 possible.
  * **MINUS:**
    * Unfortunately it cannot run separately parallel to stimulate the target with any proprietary protocol because it connects to localhost:19021 and therefore blockades the only one possible connection.

####  31.3.4. <a id='jlinkrttviewer.exe'></a>JLinkRTTViewer.exe

* `JLinkRTTViewer.exe` is a GUI tool and connects via the SEGGER API to the target. It expects ASCII codes and is not used by the Trice tool. The switching between the 16 possible terminals is done via `FF 00` ... `FF 0F`. These byte pairs can occur inside the Trice data.

<!---

* Start `"C:\Program Files (x86)\SEGGER\JLink\JLinkRTTViewer.exe"` and connect to the J-Link. You only need this as a running server to connect to.
  * Unfortunately the JLinkRTTViewer "steals" from time to time some Trice data packages and displays them as data garbage.
  * Better use JLink.exe or the *Segger J-Link SDK* instead.

-->

<p align="right">(<a href="#top">back to top</a>)</p>

###  31.4. <a id='segger-rtt'></a>Segger RTT

* The main advantages are:
  * Speed
  * No `TriceTransfer()` nor any interrupt is needed in the background
  * No UART or other output is needed
* This is, because automatically done by SeggerRTT. This way one can debug code as comfortable as with `printf()` but with all the TRICE advantages. Have a look here: ![SeggerRTTD.gif](./ref/JLINK-DebugSession.gif)
* Avoid Trice buffering inside target and write with TRICE macro directly into the RTT buffer (direct Trice mode = `#define TRICE_MODE 0` inside [triceConfig.h](https://github.com/rokath/trice/blob/main/examples/F030_inst/Core/Inc/triceConfig.h)).
* Write the bytes per Trice directly (little time & some space overhead on target, but no changes on host side)

  ![triceBlockDiagramWithSeggerRTT.svg](./ref/triceBlockDiagramWithSeggerRTTD.svg)

<p align="right">(<a href="#top">back to top</a>)</p>

###  31.5. <a id='segger-j-link-sdk-(~800-eur)-option'></a>Segger J-Link SDK (~800 EUR) Option

* Segger offers a SeggerRTT SDK which allows to use more than just channel 0 and you can develop your own tooling with it.
* The `trice -port JLINK` is ok for usage **as is** right now. However if you wish more comfort check here:
* Question: [How-to-access-multiple-RTT-channels](https://forum.segger.com/index.php/Thread/6688-SOLVED-How-to-access-multiple-RTT-channels-from-Telnet)
  * "Developer pack used to write your own program for the J-Link. Please be sure you agree to the terms of the associated license found on the Licensing Information tab before purchasing this SDK. You will benefit from six months of free email support from the time that this product is ordered."
* The main [Segger J-Link SDK](https://www.segger.com/products/debug-probes/j-link/technology/j-link-sdk/) disadvantage beside closed source and payment is: **One is not allowed to distribute binaries written with the SDK.** That makes it only interesting for company internal automatization.

<p align="right">(<a href="#top">back to top</a>)</p>

###  31.6. <a id='additional-notes-(leftovers)'></a>Additional Notes (leftovers)

* `Downloading RTT target package` from [https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/).
* Read the manual [UM08001_JLink.pdf](https://github.com/rokath/trice/blob/main/third_party/segger.com/UM08001_JLink.pdf).
* Extract `../third_party/segger.com/SEGGER_RTT_V760g.zip` to `../third_party/segger.com/SEGGER_RTT`. Check for an update @ SEGGER.
* Add `SEGGER_RTTI.c` to target project

<p align="right">(<a href="#top">back to top</a>)</p>

###  31.7. <a id='further-development'></a>Further development

* Check OpenOCD!
  * Use OpenOCD and its built-in RTT feature. OpenOCD then starts a server on localhost:17001 where it dumps all RTT messages.
* The GoST project offers a way bypassing JLINK. Used -port STLINK instead.
* Maybe `libusb` together with `libjaylink` offer some options too.
* Checkout [https://github.com/deadsy/jaylink](https://github.com/deadsy/jaylink).
* `"C:\Program Files (x86)\SEGGER\JLink\JMem.exe"` shows a memory dump.

* Go to [https://libusb.info/](https://libusb.info/)
  * -> Downloads -> Latest Windows Binaries
  * extract `libusb-1.0.23` (or later version)

```b
libusb-1.0.23\examples\bin64> .\listdevs.exe
2109:2811 (bus 2, device 8) path: 6
1022:145f (bus 1, device 0)
1022:43d5 (bus 2, device 0)
0a12:0001 (bus 2, device 1) path: 13
1366:0105 (bus 2, device 10) path: 5
```

* Repeat without connected Segger JLink

```b
libusb-1.0.23\examples\bin64> .\listdevs.exe
2109:2811 (bus 2, device 8) path: 6
1022:145f (bus 1, device 0)
1022:43d5 (bus 2, device 0)
0a12:0001 (bus 2, device 1) path: 13
```

* In this case `1366:0105 (bus 2, device 10) path: 5` is missing, so `vid=1366`, `did=0105` as example
* On Windows install WSL2. The real Linux kernel is needed for full USB access.

<p align="right">(<a href="#top">back to top</a>)</p>

###  31.8. <a id='nucleo-f030r8-example'></a>NUCLEO-F030R8 example

Info: [https://www.st.com/en/evaluation-tools/nucleo-F030r8.html](https://www.st.com/en/evaluation-tools/nucleo-F030r8.html)

####  31.8.1. <a id='rtt-with-original-on-board-st-link-firmware'></a>RTT with original on-board ST-LINK firmware

* `#define TRICE_RTT_CHANNEL 0`:
* If you use a NUCLEO-F030R8 with the original ST-Link on board after firmware download enter: `trice l -p ST-LINK -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x2000"`. After pressing the reset button output becomes visible: ![./ref/STRTT.PNG](./ref/STRTT.PNG)
* It works with both ST-Link variants (with or without mass storage device.)

####  31.8.2. <a id='change-to-j-link-onboard-firmware'></a>Change to J-LINK onboard firmware

 ![./ref/STLinkReflash.PNG](./ref/STLinkReflash.PNG)

####  31.8.3. <a id='rtt-with-j-link-firmware-on-board'></a>RTT with J-LINK firmware on-board

![./ref/J-LinkRTT.PNG](./ref/J-LinkRTT.PNG)

* Observations:
  * When pressing the black reset button, you need to restart the Trice tool.
  * When restarting the Trice tool, a target reset occurs.
  * Other channel numbers than `0` seam not to work for some reason.

<p align="right">(<a href="#top">back to top</a>)</p>

###  31.9. <a id='possible-issues'></a>Possible issues

* These boards seem not to work reliable with RTT over J-Link on-board firmware.
  * NUCLEO-G071RB
  * NUCLEO_G031K8
* After flashing back the ST-LINK OB firmware with the SEGGER tool, it is recommended to use the ST tool to update the ST-LINK OB firmware. Otherwise issues could occur.

<p align="right">(<a href="#top">back to top</a>)</p>

###  31.10. <a id='openocd-with-darwin-(macos)'></a>OpenOCD with Darwin (MacOS)

* OpenOCD on MacOS works out of the box after installing it.
* When using VS code with Cortex-Debug you cannot use OpenOCD at the same time.
* The `openocd.cfg` file is taylored to the flashed on-board J-Link adapter.

**Terminal 1:**

```bash
brew install open-ocd
...
cd ./trice/examples/G0B1_inst
openocd -f openocd.cfg
Open On-Chip Debugger 0.12.0
Licensed under GNU GPL v2
For bug reports, read
    http://openocd.org/doc/doxygen/bugs.html
srst_only separate srst_nogate srst_open_drain connect_deassert_srst

Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
Info : J-Link STLink V21 compiled Aug 12 2019 10:29:20
Info : Hardware version: 1.00
Info : VTarget = 3.300 V
Info : clock speed 2000 kHz
Info : SWD DPIDR 0x0bc11477
Info : [stm32g0x.cpu] Cortex-M0+ r0p1 processor detected
Info : [stm32g0x.cpu] target has 4 breakpoints, 2 watchpoints
Info : starting gdb server for stm32g0x.cpu on 3333
Info : Listening on port 3333 for gdb connections
Info : rtt: Searching for control block 'SEGGER RTT'
Info : rtt: Control block found at 0x20001238
Info : Listening on port 9090 for rtt connections
Channels: up=1, down=0
Up-channels:
0: Terminal 1024 0
Down-channels:

Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
```

**Terminal 2:**

```bash
ms@MacBook-Pro G0B1_inst % trice l -p TCP4 -args localhost:9090  -pf none -d16
Nov 14 17:32:33.319451  TCP4:       triceExamples.c    10        0_000  Hello! 👋🙂
Nov 14 17:32:33.319463  TCP4:
Nov 14 17:32:33.319463  TCP4:         ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
Nov 14 17:32:33.319463  TCP4:         🎈🎈🎈🎈  NUCLEO-G0B1RE   🎈🎈🎈🎈
Nov 14 17:32:33.319463  TCP4:         🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃
Nov 14 17:32:33.319463  TCP4:
Nov 14 17:32:33.319463  TCP4:
Nov 14 17:32:33.406455  TCP4:       triceExamples.c    16        0_037 2.71828182845904523536 <- float number as string
Nov 14 17:32:33.505116  TCP4:       triceExamples.c    17        0_087 2.71828182845904509080 (double with more ciphers than precision)
Nov 14 17:32:33.607518  TCP4:       triceExamples.c    18        0_117 2.71828174591064453125 (float  with more ciphers than precision)
Nov 14 17:32:33.707851  TCP4:       triceExamples.c    19        0_146 2.718282 (default rounded float)
Nov 14 17:32:33.807685  TCP4:       triceExamples.c    20        0_175 A Buffer:
Nov 14 17:32:33.908202  TCP4:       triceExamples.c    21        0_204 32 2e 37 31 38 32 38 31 38 32 38 34 35 39 30 34 35 32 33 35 33 36
Nov 14 17:32:34.007148  TCP4:       triceExamples.c    22        0_254 31372e32  31383238  34383238  34303935  35333235
Nov 14 17:32:35.007949  TCP4:       triceExamples.c    23        0_301 ARemoteFunctionName(2e32)(3137)(3238)(3138)(3238)(3438)(3935)(3430)(3235)(3533)(3633)
Nov 14 17:32:35.112304  TCP4:       triceExamples.c    24              100 times a 16 byte long Trice messages, which not all will be written because of the TRICE_PROTECT:
Nov 14 17:32:35.307567  TCP4:       triceExamples.c    26        0_379 i=44444400 aaaaaa00
Nov 14 17:32:35.408257  TCP4:       triceExamples.c    27    0,000_002 i=44444400 aaaaaa00
Nov 14 17:32:35.509022  TCP4:       triceExamples.c    26        0_441 i=44444401 aaaaaa01
Nov 14 17:32:35.609439  TCP4:       triceExamples.c    27    0,000_002 i=44444401 aaaaaa01
Nov 14 17:32:35.710201  TCP4:       triceExamples.c    26        0_504 i=44444402 aaaaaa02
...
```

###  31.11. <a id='segger-j-link-on-darwin-(macos)'></a>SEGGER J-Link on Darwin (MacOS)

TODO: Working example with SEGGER_RTT J-Link and Open OCD

###  31.12. <a id='links'></a>Links

<!--* [https://www.codeinsideout.com/blog/stm32/j-link-rtt/](https://www.codeinsideout.com/blog/stm32/j-link-rtt/) (A good explanation of SEGGER J-Link Realtime Transfer - Fast Debug protocol: - only suitable for ASCII transfer) -->
* [USB over WSL2?](https://twitter.com/beriberikix/status/1487127732190212102?s=20&t=NQVa27qvOqPi2uGz6pJNRA) (Maybe intersting for OpenOCD)
* https://kickstartembedded.com/2024/03/26/openocd-one-software-to-rule-debug-them-all/?amp=1
* https://mcuoneclipse.com/2021/10/03/visual-studio-code-for-c-c-with-arm-cortex-m-part-9-rtt/

<p align="right">(<a href="#top">back to top</a>)</p>

##  32. <a id='writing-the-trice-logs-into-an-sd-card-(or-a-user-specific-output)'></a>Writing the Trice logs into an SD-card (or a user specific output)

* Enable `TRICE_DEFERRED_AUXILIARY8` in your project specific _triceConfig.h_ file. 
* Enabling `TRICE_DEFERRED_AUXILIARY8` is possible parallel to any direct and/or deferred output.
* The `TRICE_DEFERRED_OUT_FRAMING` value is used also for the deferred auxiliary writes.
* Consider the value for `TRICE_DEFERRED_TRANSFER_MODE`. The `TRICE_SINGLE_PACK_MODE` would trigger a file write function on each single Trice message. 
* Provide a self-made function like this:

    ```C
    /// mySDcardWrite performs SD-card writing by appending to file myTriceLogs.bin.
    mySDcardWrite(const uint8_t* buf, size_t bufLen){
        ...
    }

    /// Assign this function pointer accordingly.
    UserNonBlockingDeferredWrite8AuxiliaryFn = mySDcardWrite; 
    ```

* If the SD-card write is more effective using 32-bits chunks, consider `TRICE_DEFERRED_AUXILIARY32`, what is recommended also if you use the encryption option.
  * `TRICE_DEFERRED_AUXILIARY32` is diabled / not implemented yet. It will be added on demand.
* There maybe use cases for `TRICE_DIRECT_AUXILIARY8` or `TRICE_DIRECT_AUXILIARY32`, but consider the max write time.
* Placing the files *til.json* and *li.json* anto the SD-card as well might be meaninjful.
* To decode *myTriceLogs.bin* later

    ```bash
    trice log -port FILEBUFFER -args myTriceLogs.bin -hs off
    ```

Related issues/discussions:
[\#253](https://github.com/rokath/trice/discussions/253)
[\#405](https://github.com/rokath/trice/discussions/405)
[\#425](https://github.com/rokath/trice/issues/425)
[\#447](https://github.com/rokath/trice/discussions/447)
[\#537](https://github.com/rokath/trice/discussions/537)

<p align="right">(<a href="#top">back to top</a>)</p>

##  33. <a id='trice-target-code-implementation'></a>Trice Target Code Implementation

###  33.1. <a id='trice-macro-structure'></a>TRICE Macro structure

####  33.1.1. <a id='trice_enter'></a>TRICE_ENTER

* Optionally disable interrupts.
* Prepare `TriceBufferWritePosition` and keep its initial value.

####  33.1.2. <a id='trice_put'></a>TRICE_PUT

* Use and increment `TriceBufferWritePosition`.

####  33.1.3. <a id='trice_leave'></a>TRICE_LEAVE

* Use `TriceBufferWritePosition` and its initial value for data transfer
* Optionally restore interrupt state.

###  33.2. <a id='trice_stack_buffer'></a>TRICE_STACK_BUFFER

* `TRICE_ENTER`: Allocate stack
* `TRICE_LEAVE`: Call TriceDirectOut()

###  33.3. <a id='trice_static_buffer'></a>TRICE_STATIC_BUFFER

* This is like `TRICE_STACK_BUFFER` but avoids stack allocation, what is better for many stacks.
* `TRICE_ENTER`: Set TriceBufferWritePosition to buffer start.
* `TRICE_LEAVE`: Call TriceDirectOut().

###  33.4. <a id='trice_double_buffer'></a>TRICE_DOUBLE_BUFFER

* `TRICE_ENTER`: Keep TriceBufferWritePosition.
* `TRICE_LEAVE`: Optionally call TriceDirectOut().

###  33.5. <a id='trice_ring_buffer'></a>TRICE_RING_BUFFER

* `TRICE_ENTER`: Keep or wrap TriceBufferWritePosition and add offset.
* `TRICE_LEAVE`: Optionally call TriceDirectOut().

The `TRICE_RING_BUFFER` allocates incremental ring buffer space and each trice location is read by a deferred task.

###  33.6. <a id='deferred-out'></a>Deferred Out

####  33.6.1. <a id='double-buffer'></a>Double Buffer

* TriceTransfer
  * TriceOut
  * TriceNonBlockingWrite( triceID, enc, encLen );

####  33.6.2. <a id='ring-buffer'></a>Ring Buffer

* TriceTransfer
  * lastWordCount = TriceSingleDeferredOut(addr);
    * int triceID = TriceIDAndBuffer( pData, &wordCount, &pStart, &Length );
    * TriceNonBlockingWrite( triceID, pEnc, encLen );

###  33.7. <a id='direct-transfer'></a>Direct Transfer

* TRICE_LEAVE
  * TriceDirectWrite(triceSingleBufferStartWritePosition, wordCount);
    * optional RTT32 with optional XTEAwithCOBS
    * optional RTT8  with optional XTEAwithCOBS
    * optional
      * triceIDAndLen
      * triceDirectEncode
      * triceNonBlockingDirectWrite

###  33.8. <a id='possible-target-code-improvements'></a>Possible Target Code Improvements

There have been 3 similar implementations for trice encode

```C
static size_t triceDirectEncode(   uint8_t* enc, const uint8_t * buf, size_t len );
       size_t TriceDeferredEncode( uint8_t* enc, const uint8_t * buf, size_t len );

unsigned TriceEncryptAndCobsFraming32( uint32_t * const triceStart, unsigned wordCount ){
```

Now:

```C
size_t TriceEncode( unsigned encrypt, unsigned framing, uint8_t* dst, const uint8_t * buf, size_t len ){
unsigned TriceEncryptAndCobsFraming32( uint32_t * const triceStart, unsigned wordCount ){
```

Currently there are 3 similar implementations for trice buffer reads

```C
static size_t triceIDAndLen(    uint32_t* pBuf,               uint8_t** ppStart, int*      triceID );
static int    TriceNext(        uint8_t** buf,                size_t* pSize,     uint8_t** pStart,    size_t* pLen );
static int    TriceIDAndBuffer( uint32_t const * const pData, int* pWordCount,   uint8_t** ppStart,   size_t* pLength );
```

* The TriceID is only needed for routing and can go in a global variable just for speed.
* The source buffer should be `uint32_t const * const`.
* The destination should be given with `uint32_t * const` and the return value is the trice netto size. For efficiency the result should be ready encoded.

```C
//! \param pTriceID is filled with ID for routing
//! \param pCount is used for double or ring buffer to advance inside the buffer
//! \param dest provides space for the encoded trice
//! \param src is the location of the trice message we want encode
//! \retval is the netto size of the encoded trice data
size_t TriceEncode(int* pTriceID, unsigned int pCount, uint32_t * const dest, uint32_t const * const src );
```

* This function interface is used for all cases.
* First we use the existing code for implementation and then we clean the code.

<p align="right">(<a href="#top">back to top</a>)</p>

