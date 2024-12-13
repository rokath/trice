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
	GenerateTilH   bool
	GenerateTilC   bool
	GenerateTilCS  bool
	GenerateRpcH   bool
	GenerateRpcC   bool
	WriteAllColors bool
)

// SubCmdIdGenerate performs sub-command generate, creating support files/output.
func SubCmdGenerate(w io.Writer, fSys *afero.Afero) (err error) {
	if !GenerateTilH && !GenerateTilC && !GenerateTilCS && !GenerateRpcH && !GenerateRpcC && !WriteAllColors {
		fmt.Fprintln(w, `The "trice generate" command needs at least one parameter. Check "trice help -generate".`)
		return nil
	}

	if WriteAllColors {
		emitter.ShowAllColors()
		if Verbose {
			fmt.Fprintln(w, `Modify ansi.ColorFunc assignments in lineTransformerANSI.go to change Trice colors.`)
		}
	}

	ilu := NewLut(w, fSys, FnJSON) // read til.json
	fn := strings.TrimSuffix(FnJSON, filepath.Ext(FnJSON))
	msg.FatalOnErr(err)

	if GenerateTilH {
		fnH := fn + ".h"
		msg.FatalOnErr(ToFileTilH(fSys, fnH))
		if Verbose {
			fmt.Fprintln(w, "generated", fnH)
		}
	}

	if GenerateRpcH {
		fnRPC := fn + "Rpc.h"
		msg.FatalOnErr(ilu.ToFileTriceRpcH(fSys, fnRPC))
		if Verbose {
			fmt.Fprintln(w, "generated", fnRPC)
		}
	}

	if GenerateTilC {
		fnC := fn + ".c"
		msg.FatalOnErr(ilu.ToFileTilC(fSys, fnC))
		if Verbose {
			fmt.Fprintln(w, "generated", fnC)
		}
	}

	if GenerateTilCS {
		fnCS := fn + ".cs"
		msg.FatalOnErr(ilu.ToFileTilCSharp(fSys, fnCS))
		if Verbose {
			fmt.Fprintln(w, "generated", fnCS)
		}
	}

	if GenerateRpcC {
		fnRPC := fn + "Rpc.c"
		msg.FatalOnErr(ilu.ToFileRpcC(fSys, fnRPC))
		if Verbose {
			fmt.Fprintln(w, "generated", fnRPC)
		}
	}

	return nil
}

// ToFileTilCSharp generates C# helpers for a third party tool.
func (ilu TriceIDLookUp) ToFileTilCSharp(fSys afero.Fs, fn string) (err error) {
	fh, err := fSys.Create(fn)
	msg.FatalOnErr(err)
	defer func() {
		err = fh.Close()
		msg.FatalOnErr(err)
	}()

	cs, e := ilu.toListTilCS(fn)
	_, err = fh.Write(cs)
	msg.FatalOnErr(e)
	return
}

// toListTilCS converts ilu into CS-source byte slice in human-readable form.
func (ilu TriceIDLookUp) toListTilCS(filename string) ([]byte, error) {
	text := []byte(`//! \file ` + filename + ` 

// Trice generated code - do not edit!

// There is still a need to exchange the format specifier from C to C# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// See https://stackoverflow.com/questions/33432341/how-to-use-c-language-format-specifiers-in-c-sharp
// and https://www.codeproject.com/Articles/19274/A-printf-implementation-in-C for possible help.

namespace TriceIDList;

	public class TilItem
	{
		public TilItem(int bitWidth, int paramCount, string strg)
		{
			BitWidth = bitWidth;
			ParamCount = paramCount;
			Strg = strg;
		}

		public int BitWidth { get; init; }
		public int ParamCount { get; init; }
		public string Strg { get; init; }
	}

	//! Til contains all trice format strings together with id and parameter information.
	//!
	//! The bitWidth value is not transmitted in the binary data stream and needed for its decoding.
	//! The paramCount is de-facto not needed. It is derivable from the received data, see docs/TriceUserManual.md/#binary-encoding.
	//! It is recommended to check if both values are matching. A negative paramCount indicates, that its value is unknown at compile time.
	public static class Til
	{
		public static readonly Dictionary<int, TilItem> TilList= new Dictionary<int, TilItem>
		{ /* triceType ( extended ) */ //   id,     TilItem( bitWidth, paramCount, Strg )
`)

	defaultBitWidth, err := strconv.Atoi(DefaultTriceBitWidth)
	msg.FatalOnErr(err)

	for id, t := range ilu {
		extType, bitWidth, paramCount := computeValues(t, defaultBitWidth)
		text = append(text, []byte(fmt.Sprintf(`		/* %10s ( %10s )*/ { %5d, new TilItem( %2d, %2d, "%s" ) },`+"\n", t.Type, extType, id, bitWidth, paramCount, t.Strg))...)
	}

	tail := []byte(`    };
}

`)
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
		extType, _ = ConstructFullTriceInfo(t.Type, paramCount)
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

// ConstructFullTriceInfo returns full TRICE info, if not exist in triceType string
// Possible inputs: trice, TRICE_B, trice32F, TRICE8_3, trice16_12, TRICE0
// see also AddFmtCount
func ConstructFullTriceInfo(origType string, paramCount int) (fullTriceType string, err error) {
	fullTriceType = origType
	for _, name := range []string{"TRICE", "TRice", "Trice", "trice"} {
		if strings.HasPrefix(origType, name+"_") { // when no bit width, insert it
			// ice_B -> ice32_B
			fullTriceType = name + DefaultTriceBitWidth + "_" + origType[6:]
		}
		if origType == name { // when plain trice name
			if paramCount == 0 { // no parameters
				fullTriceType = name + "0" // special case
			} else { // append bit width and count
				fullTriceType = fmt.Sprintf(name+DefaultTriceBitWidth+"_%d", paramCount)
			}
		}
		if origType == name+"8" || origType == name+"16" || origType == name+"32" || origType == name+"64" { // when no count
			fullTriceType = fmt.Sprintf(origType+"_%d", paramCount) // append count
		}
	}
	return fullTriceType, err
}
