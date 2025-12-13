// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id List is responsible for id List managing
package id

// List management

import (
	"fmt"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// ToFileTilH generates lang:C header file.
func ToFileTilH(fSys afero.Fs, fn string) (err error) {
	fh, err := fSys.Create(fn)
	msg.FatalOnErr(err)
	defer func() {
		err = fh.Close()
		msg.FatalOnErr(err)
	}()

	h := []byte(`//! \file ` + fn + `
//! ///////////////////////////////////////////////////////////////////////////

//! With Trice generated code - do not edit!

#include <stdint.h>

typedef struct{
	uint16_t id;
	uint8_t  bitWidth;
	int16_t  paramCount;
	char*    formatString;
} triceFormatStringList_t;

extern const triceFormatStringList_t triceFormatStringList[];
extern const unsigned triceFormatStringListElements;
`)
	_, err = fh.Write(h)
	msg.FatalOnErr(err)
	return
}

// ToFileTilC generates lang:C helpers for a third party tool.
func (ilu TriceIDLookUp) ToFileTilC(fSys afero.Fs, fn string) (err error) {
	fh, err := fSys.Create(fn)
	msg.FatalOnErr(err)

	defer func() {
		err = fh.Close()
		msg.FatalOnErr(err)
	}()

	text, err := ilu.toListTilC(fn)
	msg.FatalOnErr(err)
	_, err = fh.Write(text)
	msg.FatalOnErr(err)
	return
}

// toListTilC converts lim into C-source byte slice in human-readable form.
func (ilu TriceIDLookUp) toListTilC(filename string) ([]byte, error) {
	var (
		fileNameBody = strings.TrimSuffix(filename, filepath.Ext(filename))
		text         = []byte(`//! \file ` + filename + `
//! ///////////////////////////////////////////////////////////////////////////

//! Trice generated code - do not edit!

#include "` + fileNameBody + `.h"

//! triceFormatStringList contains all trice format strings together with id and parameter information.
//!
//! The bitWidth value is not transmitted in the binary data stream and needed for its decoding.
//! The paramCount is de-facto not needed. It is derivable from the received data, see docs/TriceUserManual.md#binary-encoding.
//! It is recommended to check if both values are matching. A negative paramCount indicates, that its value is unknown at compile time.
const triceFormatStringList_t triceFormatStringList[] = {
	/* Trice type (  extended  ) */  //  id, bitWidth, paramCount, format-string
`)
		tail = []byte(`};

//! triceFormatStringListElements holds the compile time computed count of list elements.
const unsigned triceFormatStringListElements = sizeof(triceFormatStringList) / sizeof(triceFormatStringList_t);
`)
	)
	defaultBitWidth, err := strconv.Atoi(DefaultTriceBitWidth)
	msg.FatalOnErr(err)

	for id, t := range ilu {
		extType, bitWidth, paramCount := computeValues(t, defaultBitWidth)
		text = append(text, []byte(fmt.Sprintf(`	/* %10s ( %10s ) */ { %5d, %3d, %2d, "%s" },`+"\n", t.Type, extType, id, bitWidth, paramCount, t.Strg))...)
	}

	text = append(text, tail...)
	return text, nil
}
