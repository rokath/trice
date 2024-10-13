// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"log"
	"math/rand"
	"strings"

	"github.com/rokath/trice/pkg/ant"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// idData holds the Id specific data.
type idData struct {
	idToTrice      TriceIDLookUp     // idToTrice is a trice ID lookup map and is generated from existing til.json file at the begin of SubCmdIdInsert. This map is only extended during SubCmdIdInsert and goes back into til.json afterwards.
	triceToId      triceFmtLookUp    // triceToId is a trice fmt lookup map (reversed idToFmt for faster operation). Each fmt can have several trice IDs (slice). This map is only reduced during SubCmdIdInsert and goes _not_ back into til.json afterwards.
	idToLocRef     TriceIDLookUpLI   // idToLocRef is the trice ID location information as reference generated from li.json (if exists) at the begin of SubCmdIdInsert and is not modified at all. At the end of SubCmdIdInsert a new li.json is generated from itemToId.
	idToLocNew     TriceIDLookUpLI   // idToLocNew is the trice ID location information generated during insertTriceIDs. At the end of SubCmdIdInsert a new li.json is generated from idToLocRef + idToLocNew.
	idInitialCount int               // idInitialCount is the initial used ID count.
	IDSpace        []triceTagIDSpace // IDSpace contains the tag specific unused IDs.
	err            error
}

// triceTagIDSpace is a Trice tag specific ID space, specified with the -IDTag CLI switch.
// Example: "trice insert -IDTag err:10,99" specifies for the Trice tag "err" the ID space 10-99.
type triceTagIDSpace struct {
	tag     string    // tag is the Trice message tag like "err" in TRice("err:foo");.
	Min     TriceID   // Min is the smallest allowed trice tag specific ID.
	Max     TriceID   // Max is the biggest allowed trice tag specific ID.
	idSpace []TriceID // idSpace is the to tag assigned ID space.
}

// IDIsPartOfIDSpace returns true if ID is existent inside p.IDSpace.
func (p *idData) IDIsPartOfIDSpace(id TriceID) bool {
	for _, tag := range p.IDSpace {
		for _, i := range tag.idSpace {
			if i == id {
				return true
			}
		}
	}
	return false
}

// removeIDFromIDSpace checks, if p.IDSpace contains id and removes it, when found.
// When p.IDSpace does not contain id, then no action is needed.
// Example: When -IDMin=10, -IDMax=20 and id=99 found in source.
func (p *idData) removeIDFromIDSpace(id TriceID) {
	for _, tag := range p.IDSpace {
		for idx, iD := range tag.idSpace {
			if iD == id {
				if SearchMethod == "random" { // do not care about order inside tag.idSpace, so do it fast
					fmt.Println("tag.idSpace=", tag.idSpace, "idx=", idx, "iD=", iD)
					tag.idSpace[idx] = tag.idSpace[len(tag.idSpace)-1] // overwrite with last
					tag.idSpace = tag.idSpace[:len(tag.idSpace)-1]     // remove last
				} else { // keep order inside tag.idSpace, so do it costly
					tag.idSpace = append(tag.idSpace[:idx], tag.idSpace[idx+1:]...)
				}
			}
		}
	}
}

// triceTag returns the trice tag, if any, or an empty string.
func triceTag(t TriceFmt) string { // t.Strg contains the Trice tag information: string until the first colon matches a string in emitter.colorChannels.
	before, _, ok := strings.Cut(t.Strg, ":")
	if !ok {
		return ""
	}
	return before // todo: normalize?
}

// newID returns a new, so far unused trice ID for usage.
// The global variable SearchMethod controls the way a new ID is selected.
func (p *idData) newID(t TriceFmt) (id TriceID) {
	tag := triceTag(t)
common:
	for i, _ := range p.IDSpace {
		//s := p.IDSpace[i] <-- Why does this not work? todo
		if tag != p.IDSpace[i].tag {
			continue
		}
		if SearchMethod == "random" {
			if len(p.IDSpace[i].idSpace) <= 0 {
				log.Fatal("Remaining IDSpace = is empty, check til.json. (You could re-create it or change -IDMin, -IDMax)")
			}
			index := rand.Intn(len(p.IDSpace[i].idSpace))
			id = p.IDSpace[i].idSpace[index]                                                // use random
			p.IDSpace[i].idSpace[index] = p.IDSpace[i].idSpace[len(p.IDSpace[i].idSpace)-1] // overwrite with last
			p.IDSpace[i].idSpace = p.IDSpace[i].idSpace[:len(p.IDSpace[i].idSpace)-1]       // remove last
		} else if SearchMethod == "upward" {
			id = p.IDSpace[i].idSpace[0]                    // use first
			p.IDSpace[i].idSpace = p.IDSpace[i].idSpace[1:] // remove first
		} else {
			id = p.IDSpace[i].idSpace[len(p.IDSpace[i].idSpace)-1]                    // use last
			p.IDSpace[i].idSpace = p.IDSpace[i].idSpace[:len(p.IDSpace[i].idSpace)-1] // remove last
		}
		return
	}
	if Verbose {
		fmt.Println("newID: Trice tag", tag, " not handled. Using common ID range for it.")
	}
	tag = ""
	goto common
}

// GetIDStateFromJSONFiles reads til and li and fills p (IDData) with this information.
func (p *idData) GetIDStateFromJSONFiles(w io.Writer, fSys *afero.Afero) {
	p.idToTrice = NewLut(w, fSys, FnJSON)
	p.triceToId = p.idToTrice.reverseS()
	p.idInitialCount = len(p.idToTrice)
	p.idToLocRef = NewLutLI(w, fSys, LIFnJSON) // for reference lookup
	p.idToLocNew = make(TriceIDLookUpLI, 4000) // for new li.json
}

// PreProcessing reads til.json and li.json and converts the data for processing.
// Also the ID space for new trice IDs is created.
func (p *idData) PreProcessing(w io.Writer, fSys *afero.Afero) {

	p.GetIDStateFromJSONFiles(w, fSys)

	var common triceTagIDSpace

	common.tag = ""
	common.Min = Min
	common.Max = Max

	p.IDSpace = append(p.IDSpace, common)

	for i, tag := range p.IDSpace {
		// create IDSpace
		//tag.idSpace = make([]TriceID, 0, s.Max-s.Min+1)
		for id := tag.Min; id <= tag.Max; id++ {
			_, usedFmt := p.idToTrice[id]
			_, usedLoc := p.idToLocRef[id]
			if !usedFmt && !usedLoc {
				//tag.idSpace = append(tag.idSpace, id) <- does not work here!
				p.IDSpace[i].idSpace = append(p.IDSpace[i].idSpace, id)
			} else if Verbose {
				if usedFmt && !usedLoc {
					fmt.Fprintln(w, "ID", id, "used, but only inside til.json")
				}
				if !usedFmt && usedLoc {
					fmt.Fprintln(w, "ID", id, "used, but only inside li.json")
				}
				if usedFmt && usedLoc {
					fmt.Fprintln(w, "ID", id, "used inside til.json and li.json")
				}
			}
		}
		if Verbose {
			fmt.Fprintln(w, "trice tag", tag.tag, "has", tag.Max-tag.Min+1, "IDs total space,", len(tag.idSpace), "IDs usable")
		}
	}
}

// postProcessing
func (p *idData) postProcessing(w io.Writer, fSys *afero.Afero) {
	// til.json
	idsAdded := len(p.idToTrice) - p.idInitialCount
	if idsAdded > 0 && !DryRun {
		msg.FatalOnErr(p.idToTrice.toFile(fSys, FnJSON))
	}
	if Verbose {
		fmt.Fprintln(w, idsAdded, "ID's added, now", len(p.idToTrice), "ID's in", FnJSON, "file.")
	}

	// li.json
	if len(p.idToLocNew) > 0 { // Renew li.json only if there are some data.
		// extend li.json
		for k, v := range p.idToLocNew {
			p.idToLocRef[k] = v
		}
		msg.FatalInfoOnErr(p.idToLocRef.toFile(fSys, LIFnJSON), "could not write LIFnJSON")
	}
	if Verbose {
		fmt.Fprintln(w, len(p.idToLocRef), "ID's in source code and now in", LIFnJSON, "file.")
	}
}

// cmdSwitchTriceIDs performs action (triceIDCleaning or triceIDInsertion) between preProcessing and postProcessing.
// This is done implicit by calling a.Walk for all source tree files, each in a separate Go routine.
func (p *idData) cmdSwitchTriceIDs(w io.Writer, fSys *afero.Afero, action ant.Processing) error {
	// initialize
	a := new(ant.Admin)
	a.Action = action
	a.Trees = Srcs
	a.MatchingFileName = isSourceFile

	// process
	p.PreProcessing(w, fSys)
	err := a.Walk(w, fSys)
	p.postProcessing(w, fSys)
	return err
}
