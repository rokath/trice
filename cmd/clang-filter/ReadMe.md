# clang-filter

`clang-filter` reads paths from standard input and removes paths matched by
[.clang-format-ignore](../../.clang-format-ignore). It keeps vendor sources,
generated files, and other deliberately excluded C/C++ files out of formatting.

The normal entry point is:

```sh
./scripts/_280_format_c_code.sh format
```

Use `check` instead of `format` to verify formatting without modifying files:

```sh
./scripts/_280_format_c_code.sh check
```

The script runs this helper with `go run ./cmd/clang-filter`. GitHub Actions
uses the same script, so local checks and CI apply the same ignore rules.
