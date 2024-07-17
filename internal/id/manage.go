// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id List is responsible for id List managing
package id

// List management

import (
	"encoding/json"
	"fmt"
	"io"
	"log"
	"math/rand"
	"path/filepath"
	"strings"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// NewLut returns a look-up map generated from JSON map file named fn.
func NewLut(w io.Writer, fSys *afero.Afero, fn string) TriceIDLookUp {
	lu := make(TriceIDLookUp)
	if fn == "emptyFile" { // reserved name for tests only
		return lu
	}
	msg.FatalOnErr(lu.fromFile(fSys, fn))
	if Verbose {
		fmt.Fprintln(w, "Read ID List file", fn, "with", len(lu), "items.")
	}
	return lu
}

// NewLutLI returns a look-up map generated from JSON map file named fn.
func NewLutLI(w io.Writer, fSys *afero.Afero, fn string) TriceIDLookUpLI {
	li := make(TriceIDLookUpLI)
	if fn == "emptyFile" { // reserved name for tests only
		return li
	}
	msg.FatalOnErr(li.fromFile(fSys, fn))
	if Verbose {
		fmt.Fprintln(w, "Read ID location information file", fn, "with", len(li), "items.")
	}
	return li
}

// newID() gets a new ID not used so far.
// The delivered id is usable as key for lu, but not added. So calling fn twice without adding to ilu could give the same value back.
// It is important that ilu was refreshed before with all sources to avoid finding as a new ID an ID which is already used in the source tree.
func (ilu TriceIDLookUp) newID(w io.Writer, min, max TriceID, searchMethod string) TriceID {
	if Verbose {
		fmt.Fprintln(w, "IDMin=", min, "IDMax=", max, "IDMethod=", searchMethod)
	}
	switch searchMethod {
	case "random":
		return ilu.newRandomID(w, min, max)
	case "upward":
		return ilu.newUpwardID(min, max)
	case "downward":
		return ilu.newDownwardID(min, max)
	}
	msg.Info(fmt.Sprint("ERROR:", searchMethod, "is unknown ID search method."))
	return 0
}

// newRandomID provides a random free ID inside interval [min,max].
// The delivered id is usable as key for lu, but not added. So calling fn twice without adding to ilu could give the same value back.
func (ilu TriceIDLookUp) newRandomID(w io.Writer, min, max TriceID) (id TriceID) {
	interval := int(max - min + 1)
	freeIDs := interval - len(ilu)
	msg.FatalInfoOnFalse(freeIDs > 0, "no new ID possible, "+fmt.Sprint(min, max, len(ilu)))
	wrnLimit := interval >> 3 // 12.5%
	msg.InfoOnTrue(freeIDs < wrnLimit, "WARNING: Less than 12.5% IDs free!")
	if interval <= 0 {
		log.Fatal(w, "No ID space left:", min, max)
	}
	id = min + TriceID(rand.Intn(interval))
	if len(ilu) == 0 {
		return
	}
	for {
	nextTry:
		for k := range ilu {
			if id == k { // id used
				fmt.Fprintln(w, "ID", id, "used, next try...")
				id = min + TriceID(rand.Intn(interval))
				goto nextTry
			}
		}
		return
	}
}

// newUpwardID provides the smallest free ID inside interval [min,max].
// The delivered id is usable as key for lut, but not added. So calling fn twice without adding to ilu gives the same value back.
func (ilu TriceIDLookUp) newUpwardID(min, max TriceID) (id TriceID) {
	interval := int(max - min + 1)
	freeIDs := interval - len(ilu)
	msg.FatalInfoOnFalse(freeIDs > 0, "no new ID possible: "+fmt.Sprint("min=", min, ", max=", max, ", used=", len(ilu)))
	id = min
	if len(ilu) == 0 {
		return
	}
	for {
	nextTry:
		for k := range ilu {
			if id == k { // id used
				id++
				goto nextTry
			}
		}
		return
	}
}

// newDownwardID provides the biggest free ID inside interval [min,max].
// The delivered id is usable as key for lut, but not added. So calling fn twice without adding to ilu gives the same value back.
func (ilu TriceIDLookUp) newDownwardID(min, max TriceID) (id TriceID) {
	interval := int(max - min + 1)
	freeIDs := interval - len(ilu)
	msg.FatalInfoOnFalse(freeIDs > 0, "no new ID possible: "+fmt.Sprint("min=", min, ", max=", max, ", used=", len(ilu)))
	id = max
	if len(ilu) == 0 {
		return
	}
	for {
	nextTry:
		for k := range ilu {
			if id == k { // id used
				id--
				goto nextTry
			}
		}
		return
	}
}

// FromJSON converts JSON byte slice to ilu.
func (ilu TriceIDLookUp) FromJSON(b []byte) (err error) {
	if 0 < len(b) {
		err = json.Unmarshal(b, &ilu)
	}
	return
}

// FromJSON converts JSON byte slice to li.
func (li TriceIDLookUpLI) FromJSON(b []byte) (err error) {
	if 0 < len(b) {
		err = json.Unmarshal(b, &li)
	}
	return
}

// fromFile reads file fn into lut. Existing keys are overwritten, lut is extended with new keys.
func (ilu TriceIDLookUp) fromFile(fSys *afero.Afero, fn string) error {
	b, e := fSys.ReadFile(fn)
	s := fmt.Sprintf("fn=%s, maybe need to create an empty file first? (Safety feature)", fn)
	msg.FatalInfoOnErr(e, s)

	/*
		fh, e := fSys.Open(fn)
		s := fmt.Sprintf("fn=%s, maybe need to create an empty file first? (Safety feature)", fn)
		msg.FatalInfoOnErr(e, s)
		b := make([]byte, 100000)
		n, _ := fh.Read(b)
		msg.FatalInfoOnTrue(n == 100000, "Internal file buffer too small")
		b = b[:n]
	*/

	return ilu.FromJSON(b)
}

var Logging bool // Logging is true, when sub command log is active.

// fromFile reads fSys file fn into lut.
func (li TriceIDLookUpLI) fromFile(fSys *afero.Afero, fn string) error {
	//b, err := os.ReadFile(fn)
	//fSys := os.DirFS("")
	b, err := fSys.ReadFile(fn)
	if err == nil { // file found
		return li.FromJSON(b)
	}
	// no li.json
	if Logging {
		if Verbose {
			fmt.Println("File ", fn, "not found, not showing location information")
		}
		return nil // silently ignore non existing file
	}
	s := fmt.Sprintf("%s not found, maybe need to create an empty file first? (Safety feature)", fn)
	msg.FatalInfoOnErr(err, s)
	return err // not reached
}

// AddFmtCount adds inside ilu to all trice type names without format specifier count the appropriate count.
// example change:
// `map[10000:{Trice8_2 hi %03u, %5x} 10001:{TRICE16 hi %03u, %5x}]
// `map[10000:{Trice8_2 hi %03u, %5x} 10001:{TRICE16_2 hi %03u, %5x}]
func (ilu TriceIDLookUp) AddFmtCount(w io.Writer) {
	for i, x := range ilu {
		if strings.ContainsAny(x.Type, "0_") {
			continue
		}
		n := formatSpecifierCount(x.Strg)
		x.Type = addFormatSpecifierCount(w, x.Type, n)
		ilu[i] = x
	}
}

// toJSON converts lut into JSON byte slice in human-readable form.
func (lu TriceIDLookUp) toJSON() ([]byte, error) {
	return json.MarshalIndent(lu, "", "\t")
}

// toFile writes lut into file fn as indented JSON and in verbose mode helpers for third party.
func (ilu TriceIDLookUp) toFile(fSys afero.Fs, fn string) (err error) {
	var fJSON afero.File
	fJSON, err = fSys.Create(fn)
	msg.FatalOnErr(err)
	defer func() {
		err = fJSON.Close()
		msg.FatalOnErr(err)
	}()
	var b []byte
	b, err = ilu.toJSON()
	msg.FatalOnErr(err)
	_, err = fJSON.Write(b)
	msg.FatalOnErr(err)
	/*
		var fC, fH, fCS afero.File
		if Verbose { // generate helpers for third party
			fnC := fn + ".c"
			fC, err = fSys.Create(fnC)
			msg.FatalOnErr(err)
			fnH := fn + ".h"
			fH, err = fSys.Create(fnH)
			msg.FatalOnErr(err)
			fnCS := fn + ".cs"
			fCS, err = fSys.Create(fnCS)
			msg.FatalOnErr(err)
			defer func() {
				err = fC.Close()
				msg.FatalOnErr(err)
				err = fH.Close()
				msg.FatalOnErr(err)
				err = fCS.Close()
				msg.FatalOnErr(err)
			}()

			cs, e := ilu.toCSFmtList(fnC)
			_, err = fCS.Write(cs)
			msg.FatalOnErr(e)

			c, e := ilu.toCFmtList(fnC)
			_, err = fC.Write(c)
			msg.FatalOnErr(e)

			h := []byte(`//! \file ` + fnH + `
		//! ///////////////////////////////////////////////////////////////////////////

		//! generated code - do not edit!

		#include <stdint.h>

		typedef struct{
		char* formatString;
		uint16_t id;
		int16_t dataLength;
		uint8_t bitWidth;
		} triceFormatStringList_t;

		extern const triceFormatStringList_t triceFormatStringList[];
		extern const unsigned triceFormatStringListElements;

		`)
			_, e = fH.Write(h)
			msg.FatalOnErr(e)
		}
	*/
	return
}

// reverseS returns a reversed map. If different triceID's assigned to several equal TriceFmt all of the TriceID gets it into flu.
func (ilu TriceIDLookUp) reverseS() (flu triceFmtLookUp) {
	flu = make(triceFmtLookUp)
	for id, tF := range ilu {
		addID(tF, id, flu)
	}
	return
}

// addID adds tF and id to flu. If tF already exists inside flu, its id slice is extended with id.
func addID(tF TriceFmt, id TriceID, flu triceFmtLookUp) {
	// tF.Type = strings.ToUpper(tF.Type) // no distinction for lower and upper case Type
	idSlice := flu[tF] // If the key doesn't exist, the first value will be the default zero value.
	idSlice = append(idSlice, id)
	flu[tF] = idSlice
}

// toFile writes lut into file fn as indented JSON.
func (lim TriceIDLookUpLI) toFile(fSys afero.Fs, fn string) (err error) {
	f0, err := fSys.Create(fn)
	msg.FatalOnErr(err)
	defer func() {
		err = f0.Close()
		msg.FatalOnErr(err)
	}()

	b, err := lim.toJSON()
	msg.FatalOnErr(err)

	_, err = f0.Write(b)
	msg.FatalOnErr(err)

	return
}

// toJSON converts lim into JSON byte slice in human-readable form.
func (lim TriceIDLookUpLI) toJSON() ([]byte, error) {
	return json.MarshalIndent(lim, "", "\t")
}

// toCFmtList converts lim into C-source byte slice in human-readable form.
func (ilu TriceIDLookUp) toCFmtList(fileName string) ([]byte, error) {
	fileNameBody := fileNameWithoutSuffix(filepath.Base(fileName))
	c := []byte(`//! \file ` + fileNameBody + `.c
//! ///////////////////////////////////////////////////////////////////////////

//! generated code - do not edit!

#include "` + fileNameBody + `.h"

//! triceFormatStringList contains all trice format strings together with id and parameter information.
const triceFormatStringList_t triceFormatStringList[] = {
	// format-string,                                                                     id, dataLength, bitWidth,
`)
	var s string
	var paramCount int
	var bitWidth int
	var dataLength int
	var add bool

	for id, k := range ilu {
		s = k.Strg
		switch k.Type {

		case "TRICE0", "TRICE", "TRICE32", "TRICE32_1", "TRICE32_2", "TRICE32_3", "TRICE32_4", "TRICE32_5", "TRICE32_6", "TRICE32_7", "TRICE32_8", "TRICE32_9", "TRICE32_10", "TRICE32_11", "TRICE32_12":
			bitWidth = 32
			paramCount = formatSpecifierCount(s)
			dataLength = paramCount * 4 // use for checks
			add = true
		case "TRICE16", "TRICE16_1", "TRICE16_2", "TRICE16_3", "TRICE16_4", "TRICE16_5", "TRICE16_6", "TRICE16_7", "TRICE16_8", "TRICE16_9", "TRICE16_10", "TRICE16_11", "TRICE16_12":
			bitWidth = 16
			paramCount = formatSpecifierCount(s)
			dataLength = paramCount * 2 // use for checks
			add = true
		case "TRICE8", "TRICE8_1", "TRICE8_2", "TRICE8_3", "TRICE8_4", "TRICE8_5", "TRICE8_6", "TRICE8_7", "TRICE8_8", "TRICE8_9", "TRICE8_10", "TRICE8_11", "TRICE8_12":
			bitWidth = 8
			paramCount = formatSpecifierCount(s)
			dataLength = paramCount * 1 // use for checks
			add = true
		case "TRICE64", "TRICE64_1", "TRICE64_2", "TRICE64_3", "TRICE64_4", "TRICE64_5", "TRICE64_6", "TRICE64_7", "TRICE64_8", "TRICE64_9", "TRICE64_10", "TRICE64_11", "TRICE64_12":
			bitWidth = 64
			paramCount = formatSpecifierCount(s)
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
			add = false
		}
		if add {
			c = append(c, []byte(fmt.Sprintf(`    { "%s",%s%5d, %3d, %2d },`+"\n", s, distance(s), id, dataLength, bitWidth))...)
		}
	}

	tail := []byte(`};

//! triceFormatStringListElements holds the compile time computed count of list elements.
const unsigned triceFormatStringListElements = sizeof(triceFormatStringList) / sizeof(triceFormatStringList_t);
`)
	c = append(c, tail...)
	return c, nil
}

// toCSFmtList converts ilu into CS-source byte slice in human-readable form.
func (ilu TriceIDLookUp) toCSFmtList(fileName string) ([]byte, error) {
	c := []byte(`// generated code - do not edit!

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
	var s string
	var paramCount int
	var bitWidth int
	var dataLength int
	var add bool

	for id, k := range ilu {
		s = k.Strg
		switch k.Type {

		case "TRICE0", "TRICE", "TRICE32", "TRICE32_1", "TRICE32_2", "TRICE32_3", "TRICE32_4", "TRICE32_5", "TRICE32_6", "TRICE32_7", "TRICE32_8", "TRICE32_9", "TRICE32_10", "TRICE32_11", "TRICE32_12":
			bitWidth = 32
			paramCount = formatSpecifierCount(s)
			dataLength = paramCount * 4 // use for checks
			add = true
		case "TRICE16", "TRICE16_1", "TRICE16_2", "TRICE16_3", "TRICE16_4", "TRICE16_5", "TRICE16_6", "TRICE16_7", "TRICE16_8", "TRICE16_9", "TRICE16_10", "TRICE16_11", "TRICE16_12":
			bitWidth = 16
			paramCount = formatSpecifierCount(s)
			dataLength = paramCount * 2 // use for checks
			add = true
		case "TRICE8", "TRICE8_1", "TRICE8_2", "TRICE8_3", "TRICE8_4", "TRICE8_5", "TRICE8_6", "TRICE8_7", "TRICE8_8", "TRICE8_9", "TRICE8_10", "TRICE8_11", "TRICE8_12":
			bitWidth = 8
			paramCount = formatSpecifierCount(s)
			dataLength = paramCount * 1 // use for checks
			add = true
		case "TRICE64", "TRICE64_1", "TRICE64_2", "TRICE64_3", "TRICE64_4", "TRICE64_5", "TRICE64_6", "TRICE64_7", "TRICE64_8", "TRICE64_9", "TRICE64_10", "TRICE64_11", "TRICE64_12":
			bitWidth = 64
			paramCount = formatSpecifierCount(s)
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
			add = false
		}
		if add {
			c = append(c, []byte(fmt.Sprintf(`        { %5d, new TilItem( "%s", %d, %d ) },`+"\n", id, s, bitWidth, dataLength))...)
		}
	}

	tail := []byte(`    };
}

`)
	c = append(c, tail...)
	return c, nil
}

func fileNameWithoutSuffix(fileName string) string {
	return fileName[:len(fileName)-len(filepath.Ext(fileName))]
}

// distance returns 80 - len(s) spaces as string
func distance(s string) string {
	switch 80 - len(s) {
	default:
		return ""
	case 0:
		return ""
	case 1:
		return " "
	case 2:
		return "  "
	case 3:
		return "   "
	case 4:
		return "    "
	case 5:
		return "     "
	case 6:
		return "      "
	case 7:
		return "       "
	case 8:
		return "        "
	case 9:
		return "         "
	case 10:
		return "          "
	case 11:
		return "           "
	case 12:
		return "            "
	case 13:
		return "             "
	case 14:
		return "              "
	case 15:
		return "               "
	case 16:
		return "                "
	case 17:
		return "                 "
	case 18:
		return "                  "
	case 19:
		return "                   "
	case 20:
		return "                    "
	case 21:
		return "                     "
	case 22:
		return "                      "
	case 23:
		return "                       "
	case 24:
		return "                        "
	case 25:
		return "                         "
	case 26:
		return "                          "
	case 27:
		return "                           "
	case 28:
		return "                            "
	case 29:
		return "                             "
	case 30:
		return "                              "
	case 31:
		return "                               "
	case 32:
		return "                                "
	case 33:
		return "                                 "
	case 34:
		return "                                  "
	case 35:
		return "                                   "
	case 36:
		return "                                    "
	case 37:
		return "                                     "
	case 38:
		return "                                      "
	case 39:
		return "                                       "
	case 40:
		return "                                        "
	case 41:
		return "                                         "
	case 42:
		return "                                          "
	case 43:
		return "                                           "
	case 44:
		return "                                            "
	case 45:
		return "                                             "
	case 46:
		return "                                              "
	case 47:
		return "                                               "
	case 48:
		return "                                                "
	case 49:
		return "                                                 "
	case 50:
		return "                                                  "
	case 51:
		return "                                                   "
	case 52:
		return "                                                    "
	case 53:
		return "                                                     "
	case 54:
		return "                                                      "
	case 55:
		return "                                                       "
	case 56:
		return "                                                        "
	case 57:
		return "                                                         "
	case 58:
		return "                                                          "
	case 59:
		return "                                                           "
	case 60:
		return "                                                            "
	case 61:
		return "                                                             "
	case 62:
		return "                                                              "
	case 63:
		return "                                                               "
	case 64:
		return "                                                                "
	case 65:
		return "                                                                 "
	case 66:
		return "                                                                  "
	case 67:
		return "                                                                   "
	case 68:
		return "                                                                    "
	case 69:
		return "                                                                     "
	case 70:
		return "                                                                      "
	case 71:
		return "                                                                       "
	case 72:
		return "                                                                        "
	case 73:
		return "                                                                         "
	case 74:
		return "                                                                          "
	case 75:
		return "                                                                           "
	case 76:
		return "                                                                            "
	case 77:
		return "                                                                             "
	case 78:
		return "                                                                              "
	case 79:
		return "                                                                               "
	}
}
