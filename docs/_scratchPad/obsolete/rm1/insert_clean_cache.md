# Trice Insert/Clean Cache

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Trice Cache Idea](#trice-cache-idea)
* [2. Trice Cache Logic](#trice-cache-logic)
* [3. Trice Cache Remarks](#trice-cache-remarks)
* [4. Trice Cache Tests](#trice-cache-tests)
* [5. Possible Trice Cache Editor-Issues And How To Get Around](#possible-trice-cache-editor-issues-and-how-to-get-around)
* [6. Activating the Trice Cache](#activating-the-trice-cache)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

The `trice insert` and `trice clean` commands are parsing and modifying the source code files. Even this is a reasonable fast procedure, this could get time consuming on large projects, especially when using these commands as permanent pre-compile and post-compile steps. It is assumed, that usually between 2 compile steps not all project files are changed. The project files majority will stay unchanged despite the ID insertion and removal. This repeated parsing and modifying of unchanged source code is avoidable with the Trice cache technique. Also it could get annoying to recompile files all the time only because they got Trice IDs removed and inserted. With the Trice cache we get also a solution not to re-compile un-edited files as well.

## 1. <a id="trice-cache-idea"></a>Trice Cache Idea

Lets talk about just one source file `$HOME/my/src/foo.c` and imagine we process many in one shot.

* On `trice insert foo.c`, get full path of `foo.c`, then:
  If `.trice/cache/cleaned/home/my/src/foo.c` exists and has the same modification time as `/home/my/src/foo.c`, copy `.trice/cache/inserted/home/my/src/foo.c` (if existing) to `/home/my/src/foo.c`. Otherwise insert IDs into `/home/my/src/foo.c` and afterwards copy it to `.trice/cache/inserted/home/my/src/foo.c`.
* On `trice clean  foo.c`, get full path of `foo.c`, then:
  If `.trice/cache/inserted/home/my/src/foo.c` exists and has the same modification time as `/home/my/src/foo.c`, copy `.trice/cache/cleaned/home/my/src/foo.c` (if existing) to `/home/my/src/foo.c`. Otherwise remove IDs from `/home/my/src/foo.c` and copy it to `.trice/cache/cleaned/home/my/src/foo.c`.
* On any repeated or alternate `trice insert` and `trice clean`, we are done.
* When a file in cleaned or inserted ID state was edited somehow, its IDs are inserted/cleaned and the cache is updated accordingly on `trice clean` or `trice insert` because the file modification time has changed.

## 2. <a id="trice-cache-logic"></a>Trice Cache Logic

When `id.TriceCacheEnabled` is true (applied `-cache` CLI switch) and the folder `~/.trice/cache` exists, we have

* optionally a _cleaned cache file_   `~/.trice/cache/cleaned/fullpath/file`  with mtime of _IDs cleaned_
* optionally an _inserted cache file_ `~/.trice/cache/inserted/fullpath/file` with mtime of _IDs inserted_
* `fullpath/file` with mtime of _IDs cleaned_ **OR** _IDs inserted_ **OR** _last edit_. When mtime of `path/file` is:
  * _IDs cleaned_:
    * On command `trice c`, nothing to do
    * On command `trice i`, copy, if existing, _inserted cache file_ into `fullpath/file`. Otherwise process `trice i` and copy result into _inserted cache file_.
  * _IDs inserted_:
    * On command `trice c`, copy, if existing, _cleaned cache file_  into `fullpath/file`. Otherwise process `trice c` and copy result into _cleaned cache file_.
    * On command `trice i`, nothing to do
  * _last edit_:
    * On command `trice c`, invalidate cache, process `trice c` and update _cleaned cache file_, file gets a new mtime, the mtime of _IDs cleaned_. <sub>On a following command `trice i`, file mtime is _IDs cleaned_, BUT the cache is invalid, so process `trice i` and update cache/inserted.</sub>
    * On command `trice i`, invalidate cache, process `trice i` and update _inserted cache file_, file gets a new mtime, the mtime of _IDs inserted_. <sub>On a following command `trice c`, file mtime is _IDs inserted_, BUT the cache is invalid, so process `trice c` and update cache/cleaned.</sub>

## 3. <a id="trice-cache-remarks"></a>Trice Cache Remarks

* `fullpath/file` means `/home/me/proj3/file` for example. When copied to the cache, the real "fullpath" is there `/home/me/.trice/cache/cleaned/home/me/proj3/file`.

> Should the `.trice/cache` be better located inside the project folder? What, if the user has several projects and several users on the same machine working on projects together? What about libraries containing trice code?

* The `~/.trice/cache` folder should the Trice tool **not** create automatically in the users home folder `$HOME`. The existence of this folder is user controlled. The folder must exist. If several users work on the same project and some use the cache and some not - it is possible this way, even build scripts are shared.
* The `~/.trice/cache` folder should **not** go under revision control.
* A CLI switch `-cache` does enable/disable the Trice cache. Default is off.
* The user should consider what happens, if other pre-compile or post-compile steps are modifying files as well, before enabling the Trice cache.

## 4. <a id="trice-cache-tests"></a>Trice Cache Tests

Nr    | Action   | cCache  | iCache  | ID state   | Edited state | Test function
-------|----------|---------|---------|------------|--------------|-------------------------------------------------------------------------------
0,1   | 0:clean  | 0:inval | 0:inval | 0:cleaned  | X:any        | Test_0_1_0000X_clean_on_invalid_cCache_invalid_iCache_cleaned_file
2,3   | 0:clean  | 0:inval | 0:inval | 1:inserted | X:any        | Test_2_3_00011_clean_on_inalid_cCache_invalid_iCache_inserted_edited_file
4,5   | 0:clean  | 0:inval | 1:valid | 0:cleaned  | X:any        | Test_4_5_0010X_clean_on_invalid_cCache_valid_iCache_cleaned_file
6     | 0:clean  | 0:inval | 1:valid | 1:inserted | 0:not        | Test_6_00110_clean_on_invalid_cCache_valid_iCache_inserted_not_edited_file
7     | 0:clean  | 0:inval | 1:valid | 1:inserted | 1:yes        | Test_7_00111_clean_on_invalid_cCache_valid_iCache_inserted_edited_file
8     | 0:clean  | 1:valid | 0:inval | 0:cleaned  | 0:not        | Test_8_01000_clean_on_valid_cCache_invalid_iCache_cleaned_not_edited_file
9     | 0:clean  | 1:valid | 0:inval | 0:cleaned  | 1:yes        | Test_9_01001_clean_on_valid_cCache_invalid_iCache_cleaned_edited_file
10    | 0:clean  | 1:valid | 0:inval | 1:inserted | 0:not        | Test_10_01011_clean_on_valid_cCache_invalid_iCache_inserted_not_edited_file
11    | 0:clean  | 1:valid | 0:inval | 1:inserted | 1:yes        | Test_11_01011_clean_on_valid_cCache_invalid_iCache_inserted_edited_file
12    | 0:clean  | 1:valid | 1:valid | 0:cleaned  | 0:not        | Test_12_01100_clean_on_valid_iCache_valid_cCache_clean_file_not_edited
13    | 0:clean  | 1:valid | 1:valid | 0:cleaned  | 1:yes        | Test_13_01101_clean_on_valid_iCache_valid_cCache_clean_file_edited
14    | 0:clean  | 1:valid | 1:valid | 1:inserted | 0:not        | Test_14_01110_clean_on_valid_iCache_valid_cCache_inserted_file_not_edited
15    | 0:clean  | 1:valid | 1:valid | 1:inserted | 1:yes        | Test_15_01111_clean_on_valid_iCache_valid_cCache_inserted_file_edited
16,17 | 1:insert | 0:inval | 0:inval | 0:cleaned  | X:any        | Test_16_17_1000X_insert_on_invalid_cCache_invalid_iCache_cleaned_file
18,19 | 1:insert | 0:inval | 0:inval | 1:inserted | X:any        | Test_18_19_1001X_insert_on_invalid_cCache_invalid_iCache_inserted_edited_file
20,21 | 1:insert | 0:inval | 1:valid | 0:cleaned  | X:any        | Test_20_21_1010X_insert_on_invalid_cCache_valid_iCache_cleaned_file
22    | 1:insert | 0:inval | 1:valid | 1:inserted | 0:not        | Test_22_10100_insert_on_invalid_cCache_valid_iCache_inserted_not_edited_file
23    | 1:insert | 0:inval | 1:valid | 1:inserted | 1:yes        | Test_23_10101_insert_on_invalid_cCache_valid_iCache_inserted_edited_file
24    | 1:insert | 1:valid | 0:inval | 0:cleaned  | 0:not        | Test_24_11000_insert_on_valid_cCache_invalid_iCache_cleaned_not_edited_file
25    | 1:insert | 1:valid | 0:inval | 0:cleaned  | 1:yes        | Test_25_11001_insert_on_valid_cCache_invalid_iCache_cleaned_edited_file
26,27 | 1:insert | 1:valid | 0:inval | 1:inserted | X:any        | Test_26_27_1010X_insert_on_invalid_cCache_valid_iCache_cleaned_file
28    | 1:insert | 1:valid | 1:valid | 0:cleaned  | 0:not        | Test_28_11100_insert_on_valid_cCache_valid_iCache_cleaned_not_edited_file
29    | 1:insert | 1:valid | 1:valid | 0:cleaned  | 1:yes        | Test_29_11100_insert_on_valid_cCache_valid_iCache_cleaned_edited_file
30    | 1:insert | 1:valid | 1:valid | 1:inserted | 0:not        | Test_30_11110_insert_on_valid_cCache_valid_iCache_inserted_not_edited_file
31    | 1:insert | 1:valid | 1:valid | 1:inserted | 1:yes        | Test_31_11111_insert_on_valid_cCache_valid_iCache_inserted_edited_file

## 5. <a id="possible-trice-cache-editor-issues-and-how-to-get-around"></a>Possible Trice Cache Editor-Issues And How To Get Around

* When a `trice i -cache && make && trice c -cache` sequence is executed, it could happen that the editor-view is not refreshed for opened and unedited files containing Trice statements.
  * It looks like the Trice IDs were not cleaned.
  * Closing and opening the file again shows, that the Trice IDs are indeed cleaned.
  * If the file is edited then without refreshing the view, that means with the shown Trice IDs, this is no problem, because after saving the edited file, it gets processed anyway, so no data loss is possible.
  * An automatic view refresh (close & open) for the editor could help here. But how to do that in an universal way?
* A workaround is, at least for VS Code, to first run `trice clean` in the build script.
  * See [examples/G1B1_inst/build.sh](../../examples/G0B1_inst/build.sh) for an implementation.

## 6. <a id="activating-the-trice-cache"></a>Activating the Trice Cache

* Create Trice cache folder:

```bash
mkdir -p ~/.trice/cache
```

* Apply `-cache` CLI switch on `trice insert` and `trice clean`. See [../scripts/_230_legacy_insert_ids.sh](../../scripts/_230_legacy_insert_ids.sh) and [../scripts/_240_legacy_clean_ids.sh](../../scripts/_240_legacy_clean_ids.sh), which both call [../scripts/_120_setup_trice_environment.sh](../../scripts/_120_setup_trice_environment.sh) and are used for example in [../examples/G0B1_inst/build.sh](../../examples/G0B1_inst/build.sh).

* Do **NOT** add the Trice cache to the version control.
* It is safe to `rm -rf ~/.trice/cache` and not to use the `-cache` CLI switch anymore.

<p align="right">(<a href="#trice-insertclean-cache">back to top</a>)</p>

