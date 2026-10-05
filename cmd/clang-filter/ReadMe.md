# clang-filter

`clang-filter` reads paths from standard input and removes paths matched by
[.clang-format-ignore](../../.clang-format-ignore) using gitignore-style rules.
It remains available as a standalone helper; the formatting workflow now uses
clang-format's native ignore handling and does not require Go.

The normal entry point is:

```sh
./scripts/_280_format_c_code.sh format
```

Use `check` instead of `format` to verify formatting without modifying files:

```sh
./scripts/_280_format_c_code.sh check
```

The script pins clang-format 23.1.2 and processes tracked C/C++ files in one
`--files` batch. All example `Core` trees are included; vendor and scratch-pad
paths remain excluded. `format` automatically installs a missing formatter in
`temp/tools`; `check` only reuses an available exact version. To install or
validate the tool without editing sources, run:

```sh
./scripts/_280_format_c_code.sh setup
```

GitHub Actions uses `setup` followed by `check` with the same script and rules.
