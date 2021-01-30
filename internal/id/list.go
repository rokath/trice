// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id List is responsible for id List managing
package id

// List management

import (
	"errors"
	"flag"
	"fmt"
	"os"

	"github.com/rokath/trice/pkg/msg"
)

// ScZero does replace all ID's in source tree with 0
func ScZero(SrcZ string, cmd *flag.FlagSet) error {
	if SrcZ == "" {
		cmd.PrintDefaults()
		return errors.New("no source tree root specified")
	}
	//	ZeroSourceTreeIds(SrcZ, !DryRun)
	return nil
}

// SubCmdUpdate is subcommand update
func SubCmdUpdate() error {
	lu := NewLut(FnJSON)
	tflu := lu.reverse()
	var modified bool

	if 0 == len(Srcs) {
		Srcs = append(Srcs, "./") // default value
	}

	if SharedIDs {
		update = sharedIDsUpdate
	} else {
		update = separatedIDsUpdate
	}

	for i := range Srcs {
		s := Srcs[i]
		srcU := ConditionalFilePath(s)
		if _, err := os.Stat(srcU); err == nil { // path exists
			modified = update(srcU, lu, tflu)
		} else if os.IsNotExist(err) { // path does *not* exist
			fmt.Println(s, " -> ", srcU, "does not exist!")
		} else {
			fmt.Println(s, "Schrodinger: file may or may not exist. See err for details.")
			// Therefore, do *NOT* use !os.IsNotExist(err) to test for file existence
			// https://stackoverflow.com/questions/12518876/how-to-check-if-a-file-exists-in-go
		}
	}

	if Verbose {
		fmt.Println(len(lu), "ID's in List", FnJSON)
	}
	if modified && !DryRun {
		msg.FatalOnErr(lu.toFile(FnJSON))
	}
	return nil
}

// update returns true if s.th. changed in map.
var update func(string, TriceIDLookUp, TriceFmtLookUp) bool

/*
// update does parse source tree, update IDs and is List
func (tflu TriceFmtLookUp) sharedIDsUpdate(dir string) error {
	err := tflu.Update(dir, !DryRun, Verbose)
	if nil != err {
		return fmt.Errorf("failed update on %s with %s: %v", dir, FnJSON, err)
	}
	if Verbose {
		fmt.Println(len(tflu), "ID's in List", FnJSON)
	}
	return nil
}
// appendIfMissing is appending item to p.List.
// It returns true if item was missing or changed, otherwise false.
func (p LookUp) appendIfMissing0(item Item, verbose bool) (int, bool) {
	fmt.Print("appendIfMissing")
	return 0, false
}

// appendIfMissing is appending item to lut and gives the id back.
// It returns true if item was missing or changed, otherwise false.
func (lut LookUp) appendIfMissing(item Item, verbose bool) (int, bool) {
	for _, e := range p.ItemList {
		if e.ID == item.ID { // if id exists
			if (e.Type == item.Type) && (e.Strg == item.Strg) { // identical
				if 0 == e.Removed { // is active
					return item.ID, false // known i, nothing todo
				}
				// id is inactive
				e.Removed = 0 // i exists again, clear removal, so it is active again
				return item.ID, true
			}
			// arriving here means a data difference for the identical id between the actual file
			// and the ID List. So a new ID is generated and goes with the actual file information
			// into the ID List. Also the ID inside the file must be replaced with the new ID.
			// The legacy ID inside the ID List can not invalidated here because it is possibly
			// used  on a different place in unchanged form. The ID invalidation could be done
			// globally later in a separate action.
			if verbose {
				fmt.Println("Same ID", e.ID, "but not identical:", e.Type, "?", item.Type, "or", e.Strg, "?", item.Strg, "so get a new ID.")
				fmt.Println(e)
				fmt.Println(item)
			}
			item.ID = p.newID()
			item.Created = int32(time.Now().Unix())
			fmt.Println(item)
			p.ItemList = append(p.ItemList, item)
			// Need to change file! Therefore the (new) ID is delivered back.
			return item.ID, true
		}
		// Do not care about same format for different IDs, what could be done here.
		// Having different IDs for identical TRICE's is more an advantage for debugging.
		// If for some reason a huge amount of identical TRICE's should get identical
		// IDs this could be done here.
	}
	p.ItemList = append(p.ItemList, item)
	return item.ID, true
}

// ExtendIDList returns id because it could get changed when id is in List with different typ or fmts.
// It is an exported function for simplifying tests in other packets.
func (p LookUp) ExtendIDList(id int, typ, fmt string, verbose bool) (int, bool) {
	i := Item{
	.Type: typ,
		.Strg: fmt,
	}
	return p.appendIfMissing(i, verbose)
}

// Index returns the index of id inside p.List. If id is not found it returns -1.
func (p LookUp) Index(id int) int {
	fmt.Println("Index")
	return 0
}
*/
