// Package tcobs implements ./docs/TCOBSSpecification.md.
// For encoding create an Encoder and use the Write method as shown in TestWrite.
// For decoding create a Decoder and use the Read method as shown in TestSingleRead and TestMultiRead.
// It is possible to use only the functions tcobs.Decode or tcobs.CEncode.
// Package tcobs is also a helper for testing the target C-code.
// Each C function gets a Go wrapper which ist tested in appropriate test functions.
// For some reason inside the tcobs_test.go an 'import "C"' is not possible.
package tcobs
