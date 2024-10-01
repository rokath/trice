# Trice Cache Specification Draft (Issue [#488](https://github.com/rokath/trice/issues/488))

> Not implemented yet! Just ideas.

## Preface

The `trice insert` and `trice clean` commands (mainly), are parsing and modifying the source code files. Even this is (thanks to Go) a reasonable fast procedure, it could get time consuming on large projects, especially when using these commands as pre-compile and post-compile step. It is assumed, that usually between 2 compile steps not all project files are changed. The project files majority will stay unchanged despite the ID insertion and removal. This repeated parsing and modifying of unchanged source code is avoidable with the Trice cache technique. Also it could get anoying to recompile files all the time only because they got Trice IDs removed and inserted. With the Trice cache we get also a solution not to re-compile un-edited files as well.

## Trice Cache Idea

Lets talk about just one source file `$HOME/my/src/foo.c` and imagine we process many in one shot.

- On `trice insert foo.c`, get full path of `foo.c`, then:
  If `.trice/cache/inserted/home/my/src/foo.c` exists and has the same modification time as `/home/my/src/foo.c`, copy `.trice/cache/inserted/home/my/src/foo.c` to `/home/my/src/foo.c`. Otherwise insert IDs into `/home/my/src/foo.c` and afterwards copy it to `.trice/cache/inserted/home/my/src/foo.c`.
- On `trice clean  foo.c`, get full path of `foo.c`, then:
  If `.trice/cache/cleaned/home/my/src/foo.c` exists and has the same modification time as `/home/my/src/foo.c`, copy `.trice/cache/cleaned/home/my/src/foo.c` to `/home/my/src/foo.c`. Otherwise remove IDs from `/home/my/src/foo.c` and copy it to `.trice/cache/cleaned/home/my/src/foo.c`.
- On any repeated or alternate `trice insert` and `trice clean`, we are done.
- When a file in cleaned or inserted ID state was edited somehow, its IDs are inserted/cleaned and the cache is updated accordingly on `trice clean` or `trice insert`.

## Trice Cache Logic

When `~/.trice/cache` exists, we have these 3 files for example:
- `~/.trice/cache/cleaned/E/repos/trice_endianness/examples/G0B1_inst/Core/Src/main.c` with mtime of IDs cleaned
- `~/.trice/cache/inserted/E/repos/trice_endianness/examples/G0B1_inst/Core/Src/main.c` with mtime of IDs inserted
- `/E/repos/trice_endianness/examples/G0B1_inst/Core/Src/main.c` with mtime of _IDs cleaned_ **OR** _IDs inserted_ **OR** last edit
  - _IDs cleaned_:
      - On command `trice c`, do nothing
      - On command `trice i`, copy _inserted cache file_ into file 
  - _IDs inserted_:
      - On command `trice c`, copy _cleaned cache file_ into file
      - On command `trice i`, do nothing 
  - _last edit_:
      - On command `trice c`, invalidate cache, process `trice c` and update _cleaned cache file_, file gets a new mtime, mtime of IDs cleaned. On a following command `trice i`, file mtime is IDs cleaned, BUT the chache is invalid, so process `trice i`.
      - On command `trice i`, invalidate cache, process `trice i` and update _inserted cache file_, file gets a new mtime, mtime of IDs inserted. On a following command `trice c`, file mtime is IDs inserted, BUT the chache is invalid, so process `trice c`.

- File: The src file to process.
- iFile: The src file with fresh inserted IDs.
- iCache: The iFile copied into the cache.
- cFile: The cleaned src file (without IDs).
- iCache: The cFile copied into the cache.
- Comparing files is done only by comparing the modification times.

```b
trice i File: File == iCache ? done          (trice i was executed before)
trice i File: File == cCache ? iCache -> F   (trice c was executed before)
trice i File: else File -> iFile -> iCache   (file was edited)
trice c File: File == cCache ? done          (trice c was executed before)
trice c File: File == iCache ? cCache -> F   (trice i was executed before)
trice c File: else File -> cFile -> cCache   (file was edited)
```



## Remarks

- The `.trice/cache` folder should the Trice tool create automatically in the users home folder `$HOME`. For now the existence of this folder is user controlled.
- The `.trice` folder should (not?) go under revison control.
- A CLI switch should enable(default)/disable the Trice cache.