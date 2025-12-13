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
	//! The paramCount is de-facto not needed. It is derivable from the received data, see docs/TriceUserManual.md#binary-encoding.
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
	DefaultTriceBitWidth = strconv.Itoa(defaultBitWidth)
	switch t.Type[len(t.Type)-1:] {
	case "B", "F", "N", "S":
		paramCount = -1
		extType = t.Type
	default:
		paramCount = formatSpecifierCount(t.Strg)
		extType, _ = ConstructFullTriceInfo(t.Type, paramCount)
	}
	bitWidth = defaultBitWidth
	for i, w := range []string{"64", "32", "16", "8"} {
		found := strings.Contains(extType, w)
		if found {
			bitWidth = 64 >> i
			break
		}
	}
	return
}

// ConstructFullTriceInfo returns full TRICE info, if not exist in orig‚Type string
// For examples see TestConstructFullTriceInfo
// see also AddFmtCount
func ConstructFullTriceInfo(origType string, paramCount int) (fullTriceType string, err error) {
	if strings.ToUpper(origType[:5]) != "TRICE" {
		err = fmt.Errorf(origType, "starts not with trice")
		return
	}
	if len(origType) == 5 { // most often expected case: trice, Trice, TRice or TRICE
		if paramCount == 0 {
			fullTriceType = fmt.Sprintf(origType+"_%d", paramCount)
		} else {
			fullTriceType = fmt.Sprintf(origType+DefaultTriceBitWidth+"_%d", paramCount)
		}
		return
	}

	before, after, found := strings.Cut(origType, "_")
	if found {
		switch after {
		case "N", "S": // TRICE_S
			if paramCount == 1 {
				fullTriceType = before + after
			} else {
				err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
			}
		case "B": // TRICE_B or TRICE8_B
			if paramCount == 1 {
				if len(before) == 5 { // no bitwidth
					fullTriceType = before + DefaultTriceBitWidth + after
				} else {
					fullTriceType = before + after
				}
			} else {
				err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
			}
		case "F": // TRICE16_F or TRICE_F
			if paramCount == 0 {
				if len(before) == 5 { // no bitwidth
					fullTriceType = before + DefaultTriceBitWidth + after
				} else {
					fullTriceType = before + after
				}
			} else {
				err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
			}
		default:
			cnt, e := strconv.Atoi(after)
			if e != nil {
				err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
			}
			if cnt != paramCount {
				err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
			}
			if cnt == 0 {
				fullTriceType = before[:5] + "_0" // no bitwidth
			} else {
				if len(before) == 5 { // trice_7 cases
					fullTriceType = before + DefaultTriceBitWidth + "_" + after
				} else {
					fullTriceType = before + "_" + after
				}
			}
		}
		return
	}
	switch before[5:] {
	case "0": // TRICE0
		if paramCount != 0 {
			err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
		}
		fullTriceType = before[:len(before)-1] + "_0"
	case "S", "N": // triceS
		if paramCount == 1 {
			fullTriceType = before
		} else {
			err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
		}
	case "B", "8B", "16B", "32B", "64B": // TRICE16B or triceF or Trice8B
		if paramCount == 1 {
			if len(before) == 6 { // no bit width given
				fullTriceType = before[:len(before)-1] + DefaultTriceBitWidth + before[len(before)-1:]
			} else {
				fullTriceType = before
			}
		} else {
			err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
		}
	case "F", "8F", "16F", "32F", "64F": // TRICE16B or triceF or Trice8B
		if paramCount == 0 {
			if len(before) == 6 { // no bit width given
				fullTriceType = before[:len(before)-1] + DefaultTriceBitWidth + before[len(before)-1:]
			} else {
				fullTriceType = before
			}
		} else {
			err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
		}
	case "8", "16", "32", "64":
		if paramCount == 0 {
			fullTriceType = fmt.Sprintf(before[:5]+"_%d", paramCount) // discard bitwidth
		} else {
			fullTriceType = fmt.Sprintf(before+"_%d", paramCount)
		}
	default:
	}
	return
}
