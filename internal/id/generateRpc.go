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

// ToFileTriceRpcH generates RPC header file.
func (ilu TriceIDLookUp) ToFileTriceRpcH(fSys afero.Fs, fn string) (err error) {
	fh, err := fSys.Create(fn)
	msg.FatalOnErr(err)

	defer func() {
		err = fh.Close()
		msg.FatalOnErr(err)
	}()

	text, err := ilu.toListTriceRpcH(fn)
	msg.FatalOnErr(err)
	_, err = fh.Write(text)
	msg.FatalOnErr(err)
	return
}

// ToFileRpcC generates lang:C helpers for a third party tool.
func (ilu TriceIDLookUp) ToFileRpcC(fSys afero.Fs, fn string) (err error) {
	fh, err := fSys.Create(fn)
	msg.FatalOnErr(err)

	defer func() {
		err = fh.Close()
		msg.FatalOnErr(err)
	}()

	text, err := ilu.toListRpcC(fn)
	msg.FatalOnErr(err)
	_, err = fh.Write(text)
	msg.FatalOnErr(err)
	return
}

// toListTriceRpcH converts lim into C-source byte slice in human-readable form.
func (ilu TriceIDLookUp) toListTriceRpcH(filename string) ([]byte, error) {
	var (
		text = []byte(`//! \file ` + filename + `
//! ///////////////////////////////////////////////////////////////////////////

//! Trice generated code - do not edit!

#include <stdint.h>

typedef void (*triceRpcHandler_t)(void* buffer, int count);

typedef struct{
    int id;
    triceRpcHandler_t fn;
} triceRpc_t;

extern triceRpc_t triceRpc[];
extern int triceRpcCount;

`)
		tail = []byte(`
// End of file
`)
	)
	defaultBitWidth, err := strconv.Atoi(DefaultTriceBitWidth)
	msg.FatalOnErr(err)

	for _, t := range ilu {
		if t.Type[len(t.Type)-1:] == "F" {
			before, after, tagged := strings.Cut(t.Strg, ":")
			var funcName string
			if tagged {
				funcName = after
			} else {
				funcName = before
			}
			_, bitWidth, _ := computeValues(t, defaultBitWidth)
			text = append(text, []byte(fmt.Sprintf(`/* %10s */ void %s( int%d_t* p, int cnt );`+"\n", t.Type, funcName, bitWidth))...)
		}
	}

	text = append(text, tail...)
	return text, nil
}

// toListTilC converts lim into C-source byte slice in human-readable form.
func (ilu TriceIDLookUp) toListRpcC(filename string) ([]byte, error) {
	var (
		fileNameBody = strings.TrimSuffix(filename, filepath.Ext(filename))
		text         = []byte(`//! \file ` + filename + `
//! ///////////////////////////////////////////////////////////////////////////

//! Trice generated code - do not edit!

#include <stdio.h> // needed for __attribute__((weak)) 
#include "` + fileNameBody + `.h"

//! triceRpc contains all rpc IDs together with their function pointer address.
const triceRpc_t triceRpc[] = {
	/* Trice type */  //  id, function pointer
`)
		tail = []byte(`
};

//! triceRpcListElements holds the compile time computed count of list elements.
const unsigned triceRpcElements = sizeof(triceRpc) / sizeof(triceRpc_t);

`)
	)

	next := false
	for id, t := range ilu {
		if t.Type[len(t.Type)-1:] == "F" {
			before, after, tagged := strings.Cut(t.Strg, ":")
			var funcName string
			if tagged {
				funcName = after
			} else {
				funcName = before
			}
			if next {
				text = append(text, []byte(fmt.Sprintln(`,`))...)
			}
			text = append(text, []byte(fmt.Sprintf(`	/* %10s */ { %5d, %s }`, t.Type, id, funcName))...)
			next = true
		}
	}
	text = append(text, tail...)

	defaultBitWidth, err := strconv.Atoi(DefaultTriceBitWidth)
	msg.FatalOnErr(err)

	for _, t := range ilu {
		if t.Type[len(t.Type)-1:] == "F" {
			before, after, tagged := strings.Cut(t.Strg, ":")
			var funcName string
			if tagged {
				funcName = after
			} else {
				funcName = before
			}
			_, bitWidth, _ := computeValues(t, defaultBitWidth)
			text = append(text, []byte(fmt.Sprintf(`void %s( int%d_t* p, int cnt) __attribute__((weak)) {}`+"\n", funcName, bitWidth))...)
		}
	}
	return text, nil
}
