// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

//go:build amd64 || 386 || arm || arm64 || ppc64le || mips64le || mipsle || wasm
// +build amd64 386 arm arm64 ppc64le mips64le mipsle wasm

// Package endian provides an architecture agnostic value describing the native endianness on the local machine.
// See '$GOARCH' in https://golang.org/doc/install/source#environment
package endian

// Native holds the native endianness of the local machine. It is binary.LittleEndian or binary.BigEndian
//  var Native = binary.LittleEndian
