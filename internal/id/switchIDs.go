// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"log"
	"math/rand"

	"github.com/rokath/trice/pkg/ant"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// idData holds the Id specific data.
type idData struct {
	idToTrice      TriceIDLookUp   // idToTrice is a trice ID lookup map and is generated from existing til.json file at the begin of SubCmdIdInsert. This map is only extended during SubCmdIdInsert and goes back into til.json afterwards.
	triceToId      triceFmtLookUp  // triceToId is a trice fmt lookup map (reversed idToFmt for faster operation). Each fmt can have several trice IDs (slice). This map is only reduced during SubCmdIdInsert and goes _not_ back into til.json afterwards.
	idToLocRef     TriceIDLookUpLI // idToLocRef is the trice ID location information as reference generated from li.json (if exists) at the begin of SubCmdIdInsert and is not modified at all. At the end of SubCmdIdInsert a new li.json is generated from itemToId.
	idToLocNew     TriceIDLookUpLI // idToLocNew is the trice ID location information generated during insertTriceIDs. At the end of SubCmdIdInsert a new li.json is generated from idToLocNew.
	idInitialCount int             // idInitialCount is the initial used ID count.
	IDSpace        []TriceID       // IDSpace contains unused IDs.
}

var (
	IDData idData
)

// newID returns a new, so far unused trice ID for usage.
// The global variable SearchMethod controls the way a new ID is selected.
func (p *idData) newID() (id TriceID) {
	if SearchMethod == "random" {
		if len(p.IDSpace) <= 0 {
			log.Fatal("Remaining IDSpace = is empty, check til.json. (You could re-create it or change -IDMin, -IDMax)")
		}
		index := rand.Intn(len(p.IDSpace))
		id = p.IDSpace[index]                          // use random
		p.IDSpace[index] = p.IDSpace[len(p.IDSpace)-1] // overwrite with last
		p.IDSpace = p.IDSpace[:len(p.IDSpace)-1]       // remove last
	} else if SearchMethod == "upward" {
		id = p.IDSpace[0]         // use first
		p.IDSpace = p.IDSpace[1:] // remove first
	} else {
		id = p.IDSpace[len(p.IDSpace)-1]         // use last
		p.IDSpace = p.IDSpace[:len(p.IDSpace)-1] // remove last
	}
	return
}

// PreProcessing reads til.json and li.json and converts the data for processing.
// Also the ID space for new trice IDs is created.
func (p *idData) PreProcessing(w io.Writer, fSys *afero.Afero) {

	// get state
	p.idToTrice = NewLut(w, fSys, FnJSON)
	p.triceToId = p.idToTrice.reverseS()
	p.idInitialCount = len(p.idToTrice)
	p.idToLocRef = NewLutLI(w, fSys, LIFnJSON) // for reference lookup
	p.idToLocNew = make(TriceIDLookUpLI, 4000) // for new li.json

	// create IDSpace
	p.IDSpace = make([]TriceID, 0, Max-Min+1)
	for id := Min; id <= Max; id++ {
		_, usedFmt := p.idToTrice[id]
		_, usedLoc := p.idToLocRef[id]
		if !usedFmt && !usedLoc {
			p.IDSpace = append(p.IDSpace, id)
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
		fmt.Fprintln(w, Max-Min+1, "IDs total space,", len(p.IDSpace), "IDs usable")
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
		msg.FatalInfoOnErr(p.idToLocNew.toFile(fSys, LIFnJSON), "could not write LIFnJSON")
	}
	if Verbose {
		fmt.Fprintln(w, len(p.idToLocNew), "ID's in source code and now in", LIFnJSON, "file.")
	}
}

// cmdSwitchTriceIDs performs action (triceIDCleaning or triceIDInsertion) between preProcessing and postProcessing.
// This is done implicit by calling a.Walk for all source tree files, each in a separate Go routine.
func cmdSwitchTriceIDs(w io.Writer, fSys *afero.Afero, action ant.Processing) error {
	// initialize
	a := new(ant.Admin)
	a.Action = action
	if len(Srcs) == 0 {
		a.Trees = append(Srcs, "./") // default value
	} else {
		a.Trees = Srcs
	}
	a.MatchingFileName = isSourceFile

	// process
	IDData.PreProcessing(w, fSys)
	err := a.Walk(w, fSys)
	IDData.postProcessing(w, fSys)
	//
	return err
}
