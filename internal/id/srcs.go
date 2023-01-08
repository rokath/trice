// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// arrayFlag is a slice type for multi flag
type arrayFlag []string

var (
	// Srcs gets multiple files or directories.
	Srcs arrayFlag
)

// String method is the needed for interface satisfaction.
func (i *arrayFlag) String() string {
	return ""
}

// Set is a needed method for multi flags.
func (i *arrayFlag) Set(value string) error {
	*i = append(*i, value)
	return nil
}

//  // FullFilePath returns absolute file path if fn is not "off" or "none".
//  func FullFilePath(fn string) string {
//  	if fn == "none" || fn == "off" {
//  		return fn
//  	}
//  	s, err := filepath.Abs(fn)
//  	msg.InfoOnErr(err, fmt.Sprintf("failed to parse %s\n", fn))
//  	return s
//  }
//
//  // FullFilePath2 returns absolute file path if fn is not "off" or "none".
//  func FullFilePath2(fSys *afero.Afero, fn string) string {
//  	xType := reflect.TypeOf(fSys)
//  	xValue := reflect.ValueOf(fSys)
//  	fmt.Println(xType, xValue) // Os: *afero.Afero &{0x85d228} // MemMap: *afero.Afero &{0xc00007bb60}
//  	if fn == "none" || fn == "off" {
//  		return fn
//  	}
//  	var e error
//  	var s string
//  	//if xValue < &{0xc000000000} {
//  	s, e = filepath.Abs(fn)
//  	msg.InfoOnErr(e, fmt.Sprintf("failed to parse %s\n", fn))
//  	//}
//  	return s
//  }
