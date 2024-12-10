// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id List is responsible for id List managing
package id

// List management

import (
	"fmt"
	"io"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

var (
	GenerateHFile           bool
	GenerateCFile           bool
	GenerateCSFile          bool
	WriteAllColors          bool
	IDToFunctionPointerList bool
)

// ConstructFullTriceInfo returns full TRICE info, if not exist in triceType string
func ConstructFullTriceInfo(triceType string, paramCount int) string {
	origType := triceType
	for _, name := range []string{"TRICE", "TRice", "Trice", "trice"} {
		if strings.HasPrefix(origType, name+"_") { // when no bit width, insert it
			triceType = name + DefaultTriceBitWidth + "_" + origType[6:]
		}
		if origType == name { // when plain trice name
			if paramCount == 0 { // no parameters
				triceType = name + "0" // special case
			} else { // append bit width and count
				triceType = fmt.Sprintf(name+DefaultTriceBitWidth+"_%d", paramCount)
			}
		}
		if origType == name+"8" || origType == name+"16" || origType == name+"32" || origType == name+"64" { // when no count
			triceType = fmt.Sprintf(origType+"_%d", paramCount) // append count
		}
	}
	return triceType
}

// SubCmdIdGenerate performs sub-command generate, creating support files/output.
func SubCmdGenerate(w io.Writer, fSys *afero.Afero) (err error) {
	if WriteAllColors {
		emitter.ShowAllColors()
		if Verbose {
			fmt.Fprintln(w, `Modify ansi.ColorFunc assignments in lineTransformerANSI.go to change Trice colors.`)
		}
	}

	if !GenerateHFile && !GenerateCFile && !GenerateCSFile && !IDToFunctionPointerList {
		if !WriteAllColors {
			fmt.Fprintln(w, `The "trice generate" command needs at least one parameter. Check "trice help -generate".`)
		}
		return nil
	}

	ilu := NewLut(w, fSys, FnJSON) // read til.json
	fn := strings.TrimSuffix(FnJSON, filepath.Ext(FnJSON))
	msg.FatalOnErr(err)

	if GenerateHFile {
		fnH := fn + ".h"
		msg.FatalOnErr(ToFileCHeader(fSys, fnH))
		if Verbose {
			fmt.Fprintln(w, "generated", fnH)
		}
	}

	if GenerateCFile {
		fnC := fn + ".c"
		msg.FatalOnErr(ilu.ToFileLangC(fSys, fnC))
		if Verbose {
			fmt.Fprintln(w, "generated", fnC)
		}
	}

	if GenerateCSFile {
		fnCS := fn + ".cs"
		msg.FatalOnErr(ilu.ToFileCSharp(fSys, fnCS))
		if Verbose {
			fmt.Fprintln(w, "generated", fnCS)
		}
	}

	if IDToFunctionPointerList {
		fmt.Fprintln(w, `CLI "-fpl" not implemented yet`)
	}

	return nil
}

// ToFileCHeader generates lang:C header file.
func ToFileCHeader(fSys afero.Fs, fn string) (err error) {
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

// ToFileLangC generates lang:C helpers for a third party tool.
func (ilu TriceIDLookUp) ToFileLangC(fSys afero.Fs, fn string) (err error) {
	fh, err := fSys.Create(fn)
	msg.FatalOnErr(err)

	defer func() {
		err = fh.Close()
		msg.FatalOnErr(err)
	}()

	c, err := ilu.toCFmtList(fn)
	msg.FatalOnErr(err)
	_, err = fh.Write(c)
	msg.FatalOnErr(err)
	return
}

// toCFmtList converts lim into C-source byte slice in human-readable form.
func (ilu TriceIDLookUp) toCFmtList(filename string) ([]byte, error) {
	var (
		fileNameBody = strings.TrimSuffix(filename, filepath.Ext(filename))
		text         = []byte(`//! \file ` + filename + `
	//! ///////////////////////////////////////////////////////////////////////////
	
	//! Trice generated code - do not edit!
	
	#include "` + fileNameBody + `.h"
	
	//! triceFormatStringList contains all trice format strings together with id and parameter information.
	//!
	//! The bitWidth value is not transmitted in the binary data stream and needed for its decoding.
	//! The paramCount is de-facto not needed. It is derivable from the received data, see docs/TriceUserManual.md/#binary-encoding.
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

func computeValues(t TriceFmt, defaultBitWidth int) (extType string, bitWidth, paramCount int) {
	switch t.Type[len(t.Type)-1:] {
	case "B", "F", "N", "S":
		paramCount = -1
		extType = t.Type
	default:
		paramCount = formatSpecifierCount(t.Strg)
		extType = ConstructFullTriceInfo(t.Type, paramCount)
	}
	bitWidth = defaultBitWidth
	for i, w := range []string{"8", "16", "32", "64"} {
		found := strings.Contains(extType, w)
		if found {
			bitWidth = 8 << i
			break
		}
	}
	return
}

// ToFileCSharp generates C# helpers for a third party tool.
func (ilu TriceIDLookUp) ToFileCSharp(fSys afero.Fs, fn string) (err error) {
	fh, err := fSys.Create(fn)
	msg.FatalOnErr(err)
	defer func() {
		err = fh.Close()
		msg.FatalOnErr(err)
	}()

	cs, e := ilu.toCSFmtList(fn)
	_, err = fh.Write(cs)
	msg.FatalOnErr(e)
	return
}

// toCSFmtList converts ilu into CS-source byte slice in human-readable form.
func (ilu TriceIDLookUp) toCSFmtList(filename string) ([]byte, error) {
	c := []byte(`//! \file ` + filename + ` 

// Trice generated code - do not edit!

// There is still a need to exchange the format specifier from C to C#.
// See https://stackoverflow.com/questions/33432341/how-to-use-c-language-format-specifiers-in-c-sharp
// and https://www.codeproject.com/Articles/19274/A-printf-implementation-in-C for possible help.

namespace TriceIDList;

    public class TilItem
    {
        public TilItem(string strg, int bitWidth, int size)
        {
            Strg = strg;
            BitWidth = bitWidth;
            Size = size;
        }

        public string Strg { get; init; }
        public int BitWidth { get; init; }
        public int Size { get; init; }
    }

    public static class Til
    {
        public static readonly Dictionary<int, TilItem> TilList= new Dictionary<int, TilItem>
        { // id, TilItem ( Strg, bitWidth, dataLength )
`)
	var paramCount int
	var bitWidth int
	var dataLength int
	var add bool

	for id, k := range ilu {
		switch strings.ToUpper(k.Type) {

		case "TRICE0", "TRICE32", "TRICE32_1", "TRICE32_2", "TRICE32_3", "TRICE32_4", "TRICE32_5", "TRICE32_6", "TRICE32_7", "TRICE32_8", "TRICE32_9", "TRICE32_10", "TRICE32_11", "TRICE32_12":
			bitWidth = 32
			paramCount = formatSpecifierCount(k.Strg)
			dataLength = paramCount * 4 // use for checks
			add = true
		case "TRICE16", "TRICE16_1", "TRICE16_2", "TRICE16_3", "TRICE16_4", "TRICE16_5", "TRICE16_6", "TRICE16_7", "TRICE16_8", "TRICE16_9", "TRICE16_10", "TRICE16_11", "TRICE16_12":
			bitWidth = 16
			paramCount = formatSpecifierCount(k.Strg)
			dataLength = paramCount * 2 // use for checks
			add = true
		case "TRICE8", "TRICE8_1", "TRICE8_2", "TRICE8_3", "TRICE8_4", "TRICE8_5", "TRICE8_6", "TRICE8_7", "TRICE8_8", "TRICE8_9", "TRICE8_10", "TRICE8_11", "TRICE8_12":
			bitWidth = 8
			paramCount = formatSpecifierCount(k.Strg)
			dataLength = paramCount * 1 // use for checks
			add = true
		case "TRICE64", "TRICE64_1", "TRICE64_2", "TRICE64_3", "TRICE64_4", "TRICE64_5", "TRICE64_6", "TRICE64_7", "TRICE64_8", "TRICE64_9", "TRICE64_10", "TRICE64_11", "TRICE64_12":
			bitWidth = 64
			paramCount = formatSpecifierCount(k.Strg)
			dataLength = paramCount * 8 // use for checks
			add = true
		case "TRICE_S":
			bitWidth = 8
			paramCount = 1
			dataLength = -1 // needs to be derived
			add = true
		case "TRICE_N":
			bitWidth = 8
			paramCount = 1
			dataLength = -2 // needs to be derived, add terminating 0
			add = true
		case "TRICE_B", "TRICE8_B":
			bitWidth = 8
			paramCount = 1
			dataLength = -8 // needs to be derived
			add = false     // provide generated format string
		case "TRICE16_B":
			bitWidth = 16
			paramCount = 1
			dataLength = -16 // needs to be derived
			add = false      // provide generated format string
		case "TRICE32_B":
			bitWidth = 32
			paramCount = 1
			dataLength = -32 // needs to be derived
			add = false      // provide generated format string
		case "TRICE64_B":
			bitWidth = 64
			paramCount = 1
			dataLength = -64 // needs to be derived
			add = false      // provide generated format string
		case "TRICE8_F", "TRICE_F": // example: TRICE_F( Id( 2844), "info:FunctionNameW",  b8,  sizeof(b8) /sizeof(int8_t) );
			bitWidth = 8
			paramCount = 1
			dataLength = -9 // needs to be derived
			add = false     // provide generated format string
		case "TRICE16_F":
			bitWidth = 16
			paramCount = 1
			dataLength = -17 // needs to be derived
			add = false      // provide generated format string
		case "TRICE32_F":
			bitWidth = 32
			paramCount = 1
			dataLength = -33 // needs to be derived
			add = false      // provide generated format string
		case "TRICE64_F":
			bitWidth = 64
			paramCount = 1
			dataLength = -65 // needs to be derived
			add = false      // provide generated format string
		default:

		}
		if add {
			c = append(c, []byte(fmt.Sprintf(`        { %5d, new TilItem( "%s", %d, %d ) },`+"\n", id, k.Strg, bitWidth, dataLength))...)
			add = false
		}
	}

	tail := []byte(`    };
}

`)
	c = append(c, tail...)
	return c, nil
}
