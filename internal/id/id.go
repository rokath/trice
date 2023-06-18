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
	// Verbose gives more information on output if set. The value is injected from main packages.
	Verbose bool

	// DryRun if set, inhibits real changes
	DryRun bool

	// FnJSON is the filename for the JSON formatted ID list.
	FnJSON = "til.json"

	// LIFnJSON is the filename for the JSON formatted location information list.
	LIFnJSON string

	// Min is the smallest allowed ID for normal trices.
	Min = TriceID(1000)

	// Max is the biggest allowed ID for normal trices.
	Max = TriceID(7999)

	// SearchMethod is the next ID search method.
	SearchMethod = "random"

	// SharedIDs true: TriceFmt's without TriceID get equal TriceID if an equal TriceFmt exists already. Use with care: The location information for only one location is displayed but it can be a wrong one.
	// SharedIDs false: TriceFmt's without TriceID get a different TriceID if an equal TriceFmt exists already (default).
	// SharedIDs bool
)

// TriceID is the trice ID referencing to Fmt.
type TriceID int

// TriceIDs is a slice of trice IDs.
type TriceIDs []TriceID

// String implements part of flag.Value interface. It returns id as string.
func (id *TriceID) String() string {
	return fmt.Sprintf("%d", *id)
}

// Set implements part of flag.Value interface. It initializes id from the partial command line string
func (id *TriceID) Set(value string) error {
	n, err := strconv.Atoi(value)
	msg.FatalOnErr(err)
	*id = TriceID(n)
	return err
}

// TriceFmt is the trice format information assigned to a trice ID.
type TriceFmt struct {
	Type string `json:"Type"` // format type (bit-size and number of fmt string parameters)
	Strg string `json:"Strg"` // format string
}

// TriceIDLookUp is the ID-to-TriceFmt info translation map. Different IDs can refer to equal TriceFmt's.
// It is used during logging.
// Example: 1:A, 5:C, 7:C
// An ID can point to one and only format string.
type TriceIDLookUp map[TriceID]TriceFmt

// triceFmtLookUp is the TriceFmt-to-ID info translation map. Equal TriceFmt cannot have different IDs in this translation map.
//
// It is derived from IDLookUp reversing it.
type triceFmtLookUp map[TriceFmt]TriceIDs

// TriceLI is the trice location information assigned to a trice ID.
type TriceLI struct {
	File string `json:"file"` // source file name of trice id
	Line int    `json:"Line"` // source file line of trice id
}

type TriceIDLookUpLI map[TriceID]TriceLI

type TriceItem struct {
	fmt        TriceFmt
	li         TriceLI
	lineOffset int // line offset, several trices are allowed to be in one single line and must be distinguishable
}

type TriceItemLookUpID map[TriceItem]TriceID

type TriceIDLookupItem map[TriceID]TriceItem
