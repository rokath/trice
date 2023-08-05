# Testing the Target `trice` C-code

The main aim of these tests is to automatic compile and run the target code in many different compiler switch variants avoiding manual testing this way.

## Tests Details

The `cgo_...` folders are serving for target code testing in different configuration variants on the host machine. The file [./testdata/triceCheck.c.txt](./testdata/triceCheck.c.txt) is the master file for editing. After changing it, [./updateTestData.sh](./updateTestData.sh) needs to run. It copies [./testdata/triceCheck.c.txt](./testdata/triceCheck.c.txt) into [./testdata/triceCheck.c](./testdata/triceCheck.c) and runs `trice insert` on it, updating [./testdata/til.json](./testdata/til.json). It then distributes some file copies.

To be able to run `go test ./...` successfully without running the [./updateTestData.sh](./updateTestData.sh) script in advance, the script action results are checked in as well. That implies that [./testdata/triceCheck.c](./testdata/triceCheck.c) goes with IDs into the repo.

An alternative would be to copy [./testdata/triceCheck.c.txt](./testdata/triceCheck.c.txt) as `./cgo_.../triceCheck.c` in all test folders (without IDs) and to use TestMain() in each test folder to insert and remove the IDs around the test, having individual `./cgo_.../til.json` and `./cgo_.../li.json` files as well. That would avoid checked-in IDs but increase the data and slow down the tests without any need.

[./testdata/cgoPackage.go](./testdata/cgoPackage.go) is the common master for the `cgo_.../generated_cgoPackage.go` files and contains the common test code. 

The folders `cgo_...` are Go packages just for tests. They all have the same package name `cgot` and are not included into the trice tool. The different `cgot` packages are independent and could have any names. They do not see each other and are used for target code testing independently.

The `cgo_.../triceConfig.h` files differ and correspondent to the `cgo_.../cgo_test.go` files. On test execution, the `./testdata/*.c` files are compiled into the trice test executable together with the trice sources `../src` using the `cgo_.../triceConfig.h` file. 

The individual tests collects the expected results (`//exp: result`) together with the line numbers into a slice to execute the test loop on it. The `triceLogTest` function gets the `triceLog` function as parameter.

`triceLogTest` iterates over the results slice and calls for each line the C-function `triceCheck`. Then the line specific binary data buffer is passed to the `triceLog` parameter function which "logs" the passed buffer into an actual result string which in turn is compared with the expected result.

The whole process is relatively slow because of the often passed Go - C barrier, but allows automated tests in many different configuration variants.

