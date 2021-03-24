// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id List is responsible for id List managing
package id

import (
	"fmt"
	"strconv"

	"github.com/rokath/trice/pkg/msg"
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// DryRun if set, inhibits real changes
	DryRun bool

	// FnJSON is the filename for the JSON formatted ID list.
	FnJSON string

	// MinShort is the smallest allowed ID for short trices.
	MinShort = TriceID(1)

	// MaxShort is the biggest allowed ID for short trices.
	MaxShort = TriceID(32767)

	// Min is the smallest allowed ID for normal trices.
	Min = TriceID(32768)

	// Max is the biggest allowed ID for normal trices.
	Max = TriceID(65535)

	// SearchMethod is the next ID search method.
	SearchMethod = "random"

	// SharedIDs true: TriceFmt's without TriceID get equal TriceID if an equal TriceFmt exists already.
	// SharedIDs false: TriceFmt's without TriceID get a different TriceID if an equal TriceFmt exists already.
	SharedIDs bool
)

// TriceID is the trice ID referencing to Fmt.
type TriceID int

// String implements part of flag.Value interface. It returns id as string.
func (id *TriceID) String() string {
	return fmt.Sprintf("%d", *id)
}

// Set implements part of flag.Value interface. It initializes id from the partial commandline string
func (id *TriceID) Set(value string) error {
	n, err := strconv.Atoi(value)
	msg.FatalOnErr(err)
	*id = TriceID(n)
	return err
}

// TriceFmt is the trice format information assigned to a trice ID.
type TriceFmt struct {
	Type string `json:"Type"` // format type (bitsize and number of fmt string parameters)
	Strg string `json:"Strg"` // format string
}

// TriceIDLookUp is the ID-to-TriceFmt info translation map. Different IDs can refer to equal TriceFmt's.
// It is used during logging.
// Example: 1:A, 5:C, 7:C
// An ID can point to one and only format string.
type TriceIDLookUp map[TriceID]TriceFmt

// TriceFmtLookUp is the TriceFmt-to-ID info translation map. Equal TriceFmt cannot have different IDs in this translation map.
//
// It is derived from IDLookUp reversing it and can be used during SharedUpdate of src tree.
// Example: A:1, !C:5, C:7 (C.7 will overwrite C:5)
// If in source code equal TriceFmt's have different IDs they are not touched.
// If an additional equal TriceFmt occures without ID it gets one of the already for this format string used IDs.
// This is the default. For forcing different IDs on equal Fmt's a SolitaryUpdate function is needed. (to do)
type TriceFmtLookUp map[TriceFmt]TriceID
