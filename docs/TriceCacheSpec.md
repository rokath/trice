# Trice Cache Specification Draft (Issue [#488](https://github.com/rokath/trice/issues/488))

> Experimental implementation!.

## Preface

The `trice insert` and `trice clean` commands (mainly), are parsing and modifying the source code files. Even this is (thanks to Go) a reasonable fast procedure, it could get time consuming on large projects, especially when using these commands as pre-compile and post-compile step. It is assumed, that usually between 2 compile steps not all project files are changed. The project files majority will stay unchanged despite the ID insertion and removal. This repeated parsing and modifying of unchanged source code is avoidable with the Trice cache technique. Also it could get anoying to recompile files all the time only because they got Trice IDs removed and inserted. With the Trice cache we get also a solution not to re-compile un-edited files as well.

<!--
## Trice Cache Idea

Lets talk about just one source file `$HOME/my/src/foo.c` and imagine we process many in one shot.

- On `trice insert foo.c`, get full path of `foo.c`, then:
  If `.trice/cache/inserted/home/my/src/foo.c` exists and has the same modification time as `/home/my/src/foo.c`, copy `.trice/cache/inserted/home/my/src/foo.c` to `/home/my/src/foo.c`. Otherwise insert IDs into `/home/my/src/foo.c` and afterwards copy it to `.trice/cache/inserted/home/my/src/foo.c`.
- On `trice clean  foo.c`, get full path of `foo.c`, then:
  If `.trice/cache/cleaned/home/my/src/foo.c` exists and has the same modification time as `/home/my/src/foo.c`, copy `.trice/cache/cleaned/home/my/src/foo.c` to `/home/my/src/foo.c`. Otherwise remove IDs from `/home/my/src/foo.c` and copy it to `.trice/cache/cleaned/home/my/src/foo.c`.
- On any repeated or alternate `trice insert` and `trice clean`, we are done.
- When a file in cleaned or inserted ID state was edited somehow, its IDs are inserted/cleaned and the cache is updated accordingly on `trice clean` or `trice insert`.
-->

## Trice Cache Logic

When `id.TriceCacheEnabled` is true (applied `-cache` CLI switch) and `~/.trice/cache` exists, we have
- optionally _cleaned cache file_   `~/.trice/cache/cleaned/path/file`  with mtime of _IDs cleaned_
- optionally  _inserted cache file_ `~/.trice/cache/inserted/path/file` with mtime of _IDs inserted_
- `path/file` with mtime of _IDs cleaned_ **OR** _IDs inserted_ **OR** _last edit_. When mtime of `path/file` is:
  - _IDs cleaned_:
      - On command `trice c`, nothing to do
      - On command `trice i`, copy, if existing, _inserted cache file_ into `path/file`. Otherwise process `trice i` and copy result into _inserted cache file_.
  - _IDs inserted_:
      - On command `trice c`, copy, if existing, _cleaned cache file_  into `path/file`. Otherwise process `trice c` and copy result into _cleaned cache file_. 
      - On command `trice i`, nothing to do
  - _last edit_:
      - On command `trice c`, invalidate cache, process `trice c` and update _cleaned cache file_, file gets a new mtime, the mtime of _IDs cleaned._ On a following command `trice i`, file mtime is _IDs cleaned_, BUT the cache is invalid, so process `trice i` and update cache/inserted.
      - On command `trice i`, invalidate cache, process `trice i` and update _inserted cache file_, file gets a new mtime, the mtime of _IDs inserted_. On a following command `trice c`, file mtime is _IDs inserted_, BUT the cache is invalid, so process `trice c` and update cache/cleaned.


## Remarks

- The `~/.trice/cache` folder should the Trice tool create automatically in the users home folder `$HOME`. For now the existence of this folder is user controlled. The folder must exist.
- The `~/.trice/cache` folder should **not** go under revision control.
- A CLI switch `-cache` does enable/disable the Trice cache. Default is off.
- The user should consider what happens, if other pre-compile ode post-compile steps are modifying files as well, before enabling the Trice cache. 
