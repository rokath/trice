# Testing the Target `trice` C-code

The main aim of these tests is to automatic compile and run the target code in many different compiler switch variants avoiding manual testing this way.

## How to run the tests

- In trice folder execute `go clean -cache && go test ./...`. Cleaning the cache is recommended, because the CGO tests somehow keep pre-compiled files and when editing, this can led to confusing results.

## Tests Details

All folders despite `testdata` are test folders and the name `tf` is used as a place holder for them in this document.

The `tf` are serving for target code testing in different configuration variants on the host machine. The file [./testdata/triceCheck.c.txt](./testdata/triceCheck.c.txt) is the master file for editing. After changing it, [./updateTestData.sh](./updateTestData.sh) needs to run. It copies [./testdata/triceCheck.c.txt](./testdata/triceCheck.c.txt) into [./testdata/triceCheck.c](./testdata/triceCheck.c) and runs `trice insert` on it, updating [./testdata/til.json](./testdata/til.json). It then distributes some file copies.

To be able to run `go test ./...` successfully without running the [./updateTestData.sh](./updateTestData.sh) script in advance, the script action results are checked in as well. That implies that [./testdata/triceCheck.c](./testdata/triceCheck.c) goes with IDs into the repo.

> An alternative would be to copy [./testdata/triceCheck.c.txt](./testdata/triceCheck.c.txt) as `triceCheck.c` to all test folders (without IDs) and to use TestMain() in each test folder to insert and remove the IDs around the test, having individual `til.json` and `li.json` files as well. That would avoid checked-in IDs but increase the data and slow down the tests without any need.

[./testdata/cgoPackage.go](./testdata/cgoPackage.go) is the common master for the `generated_cgoPackage.go` files and contains the common test code. 

The folders `tf` are Go packages just for tests. They all have the same package name `cgot` and are not included into the trice tool. The different `cgot` packages are independent and could have any names. They do not see each other and are used for target code testing independently.

The `tf/triceConfig.h` files differ and correspondent to the `tf/cgo_test.go` files in the same folder. On test execution, the `./testdata/*.c` files are compiled into the trice test executable together with the trice sources `../src` using the `cgo_.../triceConfig.h` file. 

The individual tests collect the expected results (`//exp: result`) together with the line numbers into a slice to execute the test loop on it. The `triceLogTest` function gets the `triceLog` function as parameter.

`triceLogTest` iterates over the results slice and calls for each line the C-function `triceCheck`. Then the line specific binary data buffer is passed to the `triceLog` parameter function which "logs" the passed buffer into an actual result string which in turn is compared with the expected result.

The whole process is relatively slow because of the often passed Go - C barrier, but allows automated tests in many different configuration variants.

The `testdata\cgoPackage.go` file contains a variable `testLines = 20`, which limits the amount of performed trices for each test case. Changing this value will heavily influence the test duration.

## How to add new test cases

- Choose a test folder similar to the intended test and copy it under a new descriptive name like `newTest`.
- Extend file `testdata/updateTestData.sh` accordingly.
- Edit files `newTest/triceConfig.h` and `newTest/cgo_test.go` in a matching way.
- Run command `go test test/newTest`

