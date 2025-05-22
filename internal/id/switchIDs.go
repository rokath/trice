// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

// source tree management

import (
	"fmt"
	"io"
	"log"
	"math/rand"
	"strconv"
	"strings"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/pkg/ant"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

// idData holds the Id specific data.
type idData struct {
	idToTrice      TriceIDLookUp   // idToTrice is a trice ID lookup map and is generated from existing til.json file at the begin of SubCmdIdInsert. This map is only extended during SubCmdIdInsert and goes back into til.json afterwards.
	triceToId      triceFmtLookUp  // triceToId is a trice fmt lookup map (reversed idToFmt for faster operation). Each fmt can have several trice IDs (slice). This map is only reduced during SubCmdIdInsert and goes _not_ back into til.json afterwards.
	idToLocRef     TriceIDLookUpLI // idToLocRef is the trice ID location information as reference generated from li.json (if exists) at the begin of SubCmdIdInsert and is not modified at all. At the end of SubCmdIdInsert a new li.json is generated from itemToId.
	idToLocNew     TriceIDLookUpLI // idToLocNew is the trice ID location information generated during insertTriceIDs. At the end of SubCmdIdInsert a new li.json is generated from idToLocRef + idToLocNew.
	idInitialCount int             // idInitialCount is the initial used ID count.
	TagList        []TagEntry      // IDSpace contains the tag specific unused IDs.
	err            error
}

// TagEntry is a Trice tag specific ID space, specified with the -IDTag CLI switch.
// Example: "trice insert -IDTag err:10,99" specifies for the Trice tag "err" the ID space 10-99.
type TagEntry struct {
	tagName string    // TagName is the Trice message tag like "err" in TRice("err:foo");.
	min     TriceID   // Min is the smallest allowed trice tag specific ID.
	max     TriceID   // Max is the biggest allowed trice tag specific ID.
	iDSpace []TriceID // idSpace is the to tag assigned ID space.
}

// IDIsPartOfIDSpace returns true if ID is existent inside p.IDSpace.
func (p *idData) IDIsPartOfIDSpace(id TriceID) bool {
	for i := range p.TagList {
		e := &(p.TagList[i]) // get list entry address
		for _, iD := range e.iDSpace {
			if iD == id {
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
	for i := range p.TagList {
		e := &(p.TagList[i]) // get list entry address
		for idx, iD := range e.iDSpace {
			if iD == id {
				if SearchMethod == "random" { // do not care about order inside tag.idSpace, so do it fast
					// fmt.Println("tag.idSpace=", e.iDSpace, "idx=", idx, "iD=", iD)
					e.iDSpace[idx] = e.iDSpace[len(e.iDSpace)-1] // overwrite with last
					e.iDSpace = e.iDSpace[:len(e.iDSpace)-1]     // remove last
				} else { // keep order inside tag.idSpace, so do it costly
					e.iDSpace = append(e.iDSpace[:idx], e.iDSpace[idx+1:]...)
				}
			}
		}
	}
}

// triceTag returns the trice tag, if any, or an empty string.
func triceTag(t TriceFmt) string { // t.Strg contains the Trice tag information: string until the first colon matches a string in emitter.tags.
	before, _, ok := strings.Cut(t.Strg, ":")
	if !ok {
		return ""
	}
	return before // todo: normalize?
}

// newID returns a new, so far unused trice ID for usage.
// The global variable SearchMethod controls the way a new ID is selected.
func (p *idData) newID(t TriceFmt) (id TriceID) {
	name := triceTag(t)
	tag, _ := emitter.FindTagName(name)
common:
	for i := range p.TagList {
		e := &(p.TagList[i]) // get list entry address
		if tag != e.tagName {
			continue
		}
		if len(e.iDSpace) <= 0 {
			log.Fatalf("%+v\n%+v\nRemaining IDSpace = is empty, check til.json. (You could re-create it or change -IDMin, -IDMax)", e, t)
		}
		if SearchMethod == "random" {
			index := rand.Intn(len(e.iDSpace))
			id = e.iDSpace[index]                          // use random
			e.iDSpace[index] = e.iDSpace[len(e.iDSpace)-1] // overwrite with last
			e.iDSpace = e.iDSpace[:len(e.iDSpace)-1]       // remove last
		} else if SearchMethod == "upward" {
			id = e.iDSpace[0]         // use first
			e.iDSpace = e.iDSpace[1:] // remove first
		} else {
			id = e.iDSpace[len(e.iDSpace)-1]                    // use last
			e.iDSpace = e.iDSpace[:len(p.TagList[i].iDSpace)-1] // remove last
		}
		return
	}
	if Verbose {
		fmt.Println("newID: Trice tag", tag, "not handled. Using common ID range for it.")
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

	var common TagEntry

	common.tagName = ""
	common.min = Min
	common.max = Max

	p.TagList = append(p.TagList, common)

	for i := range p.TagList {
		e := &(p.TagList[i]) // get list entry address
		for id := e.min; id <= e.max; id++ {
			_, usedFmt := p.idToTrice[id]  // is id used in til.json?
			_, usedLoc := p.idToLocRef[id] // is id used in li.json?
			if !usedFmt && !usedLoc {
				if Verbose {
					fmt.Fprintln(w, "ID", id, "unused, so add to ID space (ok, just informal)")
				}
				e.iDSpace = append(e.iDSpace, id) // create ID space -> This is what we can use later.
			} else if Verbose {
				if usedFmt && !usedLoc {
					fmt.Fprintln(w, "ID", id, "used, but only inside til.json")
				}
				if !usedFmt && usedLoc {
					fmt.Fprintln(w, "ID", id, "used, but only inside li.json")
				}
				if usedFmt && usedLoc {
					fmt.Fprintln(w, "ID", id, "used (ok, just informal)")
				}
			}
		}
		if Verbose {
			cnt := e.max - e.min + 1
			if len(e.tagName) > 0 {
				fmt.Fprintln(w, "special trice tag", e.tagName, "has", cnt, "IDs total space, with", len(e.iDSpace), "IDs usable")
			} else {
				fmt.Fprintln(w, "common trice tags have", cnt, "IDs total space, with", len(e.iDSpace), "IDs usable")
			}
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
	a.ExcludeTrees = ExcludeSrcs
	a.Verbose = Verbose
	a.MatchingFileName = isSourceFile

	// process
	p.PreProcessing(w, fSys)
	err := a.Walk(w, fSys)
	p.postProcessing(w, fSys)
	return err
}

// EvaluateIDRangeStrings reads the -IDRange strings and fills the IDData.IDSpace accordingly.
// Each tag (like "err:") is allowed to occur only once, so a "e:" will fail after "err" was applied.
// IDRanges are not allowed to overlap.
func EvaluateIDRangeStrings() error {
	var (
		tis    *TagEntry
		mi, ma int
		err    error
	)
	// Interpret supplied IDRange string.
	for _, x := range IDRange {
		tis = new(TagEntry)
		name, mima, found := strings.Cut(x, ":")
		if !found {
			continue
		}
		min, max, ok := strings.Cut(mima, ",")
		if !ok {
			goto returnErr
		}
		mi, err = strconv.Atoi(min)
		if err != nil {
			goto returnErr
		}
		tis.min = TriceID(mi)
		ma, err = strconv.Atoi(max)
		if err != nil {
			goto returnErr
		}
		tis.max = TriceID(ma)
		if mi > ma {
			return fmt.Errorf("the with -IDRange applied ID range is inval Min(%d) > Max(%d)", mi, ma)
		}
		// Find tis.TagName.
		for _, t := range emitter.Tags {
			for _, tn := range t.Names {
				if tn == name {
					tis.tagName = t.Names[0] // take the first tag name as reference.
					goto next
				}
			}
		}
		return fmt.Errorf("the with -IDRange applied name %s is unknown. Please check var Tags inside trice/internal/emitter/lineTransformerANSI.go for options", name)
	next:
		// Check for single name range assignment.
		for i := range IDData.TagList {
			e := &(IDData.TagList[i]) // get list entry address
			if e.tagName == tis.tagName {
				return fmt.Errorf("tagName %s has already an assigned ID range. Please check your command line", tis.tagName)
			}
		}
		// Check for non overlapping ranges.
		if !(Max < tis.min || tis.max < Min) {
			return fmt.Errorf("overlapping ID ranges for %s (Min %d, Max %d) and default (Min %d, Max %d)", tis.tagName, tis.min, tis.max, Min, Max)
		}
		for i := range IDData.TagList {
			e := &(IDData.TagList[i]) // get list entry address
			if e.tagName == tis.tagName {
				return fmt.Errorf("tagName %s has already an assigned ID range. Please check your command line", tis.tagName)
			}
			// e.min --- e.max --- tis.Min --- tis.Max <- non-overlapping: e.max < tis.Min
			// tis.Min --- tis.Max --- e.min --- e.max <- non-overlapping: tis.Max < e.min
			// e.min --- tis.Min --- tis.Max --- e.max <- !!! overlapping
			// tis.Min --- e.min --- e.max --- tis.Max <- !!! overlapping
			// tis.Min --- e.min --- tis.Max --- e.max <- !!! overlapping
			// e.min --- tis.Min --- e.max --- tis.Max <- !!! overlapping
			if !(e.max < tis.min || tis.max < e.min) {
				return fmt.Errorf("overlapping ID ranges for %s (Min %d, Max %d) and %s (Min %d, Max %d)", tis.tagName, tis.min, tis.max, e.tagName, e.min, e.max)
			}
		}
		// // Fill ID space.
		// for iD := tis.min; iD <= tis.max; iD++ {
		// 	tis.iDSpace = append(tis.iDSpace, iD)
		// }
		IDData.TagList = append(IDData.TagList, *tis)
		continue

	returnErr:
		return fmt.Errorf("invalid syntax in %s - expecting \"TagName:number,number\"", x)
	}
	return nil
}
