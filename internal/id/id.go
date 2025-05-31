// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id List is responsible for id List managing
package id

import (
	"fmt"
	"strconv"

	"github.com/rokath/trice/pkg/msg"
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
	Type  string `json:"Type"`            // format type (bit-size and number of fmt string parameters)
	Strg  string `json:"Strg"`            // format string
	Alias string `json:"Alias,omitempty"` // alias, if any
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
	File string `json:"File"` // source file name of trice id
	Line int    `json:"Line"` // source file line of trice id
}

type TriceIDLookUpLI map[TriceID]TriceLI
