# Work in progress - do not use

## Files and Folders

`TCOBS.h` and `TCOBS.c` - work in progress, untested code

- The package tcobs is not needed for the `trice` tool.
- File tcobs_test.go contains test functions to execute the C code during `go test ./...`
- File `tcobs.go` does the cgo connection. cgo is not supported inside test files.
