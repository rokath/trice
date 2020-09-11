// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// +build ppc64 mips64 mips s390x

// Package endian provides an architecture agnostic value describing the native endianess on the local machine.
// See '$GOARCH' in https://golang.org/doc/install/source#environment
package endian

import "encoding/binary"

// Native holds the native endianess of the local machine. Is is binary.LittleEndian or binary.BigEndian
var Native = binary.BigEndian
