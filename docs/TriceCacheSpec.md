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

## Remarks

- The `.trice` folder should the Trice tool create automatically in the users home folder `$HOME`.
- The `.trice` folder should go under revison control.
- A CLI switch should enable(default)/disable the Trice cache.