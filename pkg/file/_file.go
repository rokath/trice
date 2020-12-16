// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package file helps with file handling.
// package file
//
// import (
// 	"bufio"
// 	"fmt"
// 	"io"
// 	"io/ioutil"
// 	"log"
// 	"os"
// 	"path/filepath"
// 	"runtime"
// )

//// EqualContent returns true if contece is equal
//func EqualContent(fn0, fn1 string) bool {
//	cmp := equalfile.New(nil, equalfile.Options{}) // compare using single mode
//	ok, err := cmp.CompareFile(fn0, fn1)
//	if nil != err {
//		ok = false
//	}
//	return ok
//}

//// SmallReadLines reads all lines of a file into lines.
//// If file ends with a newline the last line in lines is empty.
//// Do not use with big files.
//func SmallReadLines(filename string) (lines []string, err error) {
//	content, err := ioutil.ReadFile(filename)
//	stringContent := string(content)
//	if runtime.GOOS == "windows" {
//		stringContent = strings.ReplaceAll(stringContent, "\r\n", "\n")
//	}
//	lines = strings.Split(stringContent, "\n")
//	return
//}

//// SortLines reads inFile, sorts the lines and writes to outFile
//// inFile and outFile can be the same
//func SortLines(inFile, outFile string) {
//	lines, err := ReadLines(inFile)
//	if err != nil {
//		fmt.Println(err)
//		os.Exit(1)
//	}
//	sort.Strings(lines)
//	err = WriteLines(outFile, lines)
//	if err != nil {
//		fmt.Println(err)
//		os.Exit(1)
//	}
//}

//// UniqLines reads inFile, removes duplicate lines and writes to outFile.
//// inFile and outFile can be the same
//// it returns count of remaining lines plus 1 (io.EOF ?)
//func UniqLines(inFile, outFile string) int {
//	il, err := ReadLines(inFile)
//	errorFatal(err)
//	ol := uniqueString(il)
//	errorFatal(WriteLines(outFile, ol))
//
//	return len(ol)
//}

//// RemoveWindowsLineEndings accepts a string representing a path and converts \r\n into \n
//// (taken from https://github.com/cgati/dos2unix/blob/master/main.go)
//func RemoveWindowsLineEndings(fileName string) {
//	file, err := os.Open(fileName)
//	if err != nil {
//		panic(err)
//	}
//	defer file.Close()
//
//	wFile, err := os.Create(fileName + ".clean")
//	if err != nil {
//		panic(err)
//	}
//	defer wFile.Close()
//	w := bufio.NewWriter(wFile)
//
//	data := make([]byte, 128)
//	for {
//		data = data[:cap(data)]
//		n, err := file.Read(data)
//		if err != nil {
//			if err == io.EOF {
//				break
//			}
//		}
//		data = data[:n]
//		var indexesToDelete []int
//		for i, b := range data {
//			if b == '\r' {
//				if i+1 < len(data) {
//					if data[i+1] == '\n' {
//						indexesToDelete = append(indexesToDelete, i)
//					}
//				}
//				data[i] = '\n'
//			}
//		}
//		data = removeFromSlice(data, indexesToDelete)
//		w.Write(data)
//	}
//	w.Flush()
//}

//// LineNumber returns the line number of the calling function.
//func LineNumber() (s string) {
//	_, fileName, fileLine, ok := runtime.Caller(1)
//	if ok {
//		s = fmt.Sprintf("%s:%d", fileName, fileLine)
//	} else {
//		s = ""
//	}
//	return
//}
