// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id List is responsible for id List managing
package id

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// DryRun if set, inhibits real changes
	DryRun bool

	// FnJSON is the filename for the JSON formatted ID list.
	FnJSON string

	// Min is the smallest allowed ID.
	Min = 1

	// Max is the biggest allowed ID.
	Max = 65535

	// SearchMethod is the next ID search method.
	SearchMethod = "random"
)

// ID is the trice ID referencing to Fmt.
type ID int

// Fmt is the trice format information assigned to a trice ID.
type Fmt struct {
	Type string `json:"fmtType"` // format type (bitsize and number of fmt string parameters)
	Strg string `json:"fmtStrg"` // format string
}

// I2F is the ID-to-Fmt info translation map. Different IDs can refer to same format. (Case source tree merge)
// It is used during logging.
// Example: 1:A, 5:C, 7:C
// An ID can point to one and only format string.
type I2F map[ID]Fmt

// F2I is the Fmt-to-ID info translation map. Same Format cannot have different IDs.
//
// It is derived from I2F reversing it and used during update.
// Example: A:1, !C:5, C:7 (C.7 will overwrite C:5)
// If in source code same format strings have different IDs they are not touched.
// If an additional same format string occures without ID it gets one of the already for this format string used IDs.
type F2I map[Fmt]ID

// Lut is the look-up table. It contains a map and its reverse map.
type Lut struct {
	i2f I2F
	f2i F2I
}
