// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id List is responsible for id List managing
package id

// List management

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"math/rand"
	"os"

	"github.com/rokath/trice/pkg/msg"
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// DryRun if set, inhibits real changes
	DryRun bool

	// FnJSON is the filename for the JSON formatted ID list.
	FnJSON string

	// LowerBound is the smallest allowed ID
	LowerBound = 1

	// UpperBound is the biggest allowed ID
	UpperBound = 65535

	// SearchMethod is the next ID search method.
	SearchMethod = "random"

	// ReUse to do: Behaviour not implemented yet.
	// ReUse assigns the same ID if trice format is already known.
	ReUse = "no"
)

// Item is the basic element
type Item struct {
	FmtType string `json:"fmtType"` // format type (bitsize and number of fmt string parameters)
	FmtStrg string `json:"fmtStrg"` // format string
	Created int32  `json:"created"` // utc unix time of creation
	Removed int32  `json:"removed"` // utc unix time of disappearing in processed src directory
}

// LookUp is the ID-to-format info translation map.
// The key is identical with ID inside Item.
type LookUp map[int]Item

// Lut is the global trice ID look-up table. It is permanetly updated using fileWatcher.
var Lut LookUp

// LutToJSON converts lut into JSON byte slice in human readable form.
func LutToJSON(lut LookUp) ([]byte, error) {
	return json.MarshalIndent(lut, "", "\t")
}

// JSONToLut converts JSON byte slice to lut.
func JSONToLut(b []byte) (lut LookUp, err error) {
	lut = make(LookUp, 1000)
	if 0 < len(b) {
		err = json.Unmarshal(b, &lut)
	}
	return
}

// JSONToLut converts JSON byte slice to lut.
func (lut LookUp) JSONToLut(b []byte) (err error) {
	if 0 < len(b) {
		err = json.Unmarshal(b, &lut)
	}
	return
}

// WriteLutToFileJSON writes lut into file fnJSON.
func WriteLutToFileJSON(fn string, lut LookUp) (err error) {
	var b []byte
	b, err = LutToJSON(lut)
	msg.FatalOnErr("", err)
	var f *os.File
	f, err = os.Create(fn)
	msg.FatalOnErr(fn, err)
	defer func() {
		err = f.Close()
	}()
	_, err = f.Write(b)
	return
}

// ReadLutFromFileJSON reads file fnJSON into lut.
func ReadLutFromFileJSON(fn string) (LookUp, error) {
	b, err := ioutil.ReadFile(fn)
	msg.FatalOnErr("May be need to create an empty file first? (Safety feature)", err)
	return JSONToLut(b)
}

// NewLut returns a lut map which stays up-to-date in case the til.json file changes.
//
// Just in case the id list file FnJSON gets updated, the file watcher updates lut.
// This way trice needs not to be restarted during development process.
func NewLut() LookUp {
	lut, err := ReadLutFromFileJSON(FnJSON)
	msg.FatalOnErr("", err)
	go lut.FileWatcher()
	if true == Verbose {
		fmt.Println("Read ID List file", FnJSON, "with", len(lut), "items.")
	}
	return lut
}

// ZeroTimestampCreated sets all timestamps 'created' to 0.
func (lut LookUp) ZeroTimestampCreated() {
	for k, v := range lut {
		v.Created = 0
		lut[k] = v
	}
}

// newID() gets a random ID not used so far.
// If all IDs used, longest removed ID is reused (TODO)
func (lut LookUp) newID() (id int) {
	switch SearchMethod {
	case "random":
		return lut.newRandomID()
	case "upward":
		return lut.newUpwardID()
	case "downward":
		return lut.newDownwardID()
	}
	msg.Info(fmt.Sprint("ERROR:", SearchMethod, "is unknown ID search method."))
	return 0
}

func (lut LookUp) newRandomID() (id int) {
	interval := UpperBound - LowerBound
	freeIDs := interval - len(lut)
	msg.FatalOnFalse("no new ID possible"+fmt.Sprint(LowerBound, UpperBound, len(lut)), freeIDs > 0)
	msg.InfoOnFalse("WARNING: More than 75% IDs used!", freeIDs < (3*interval)>>2)
	id = LowerBound + rand.Intn(interval)
	if 0 == len(lut) {
		return
	}
	for {
	nextTry:
		for k := range lut {
			if id == k { // id used
				fmt.Println("ID", id, "used, next try...")
				id = LowerBound + rand.Intn(interval)
				goto nextTry
			}
		}
		return
	}
}

func (lut LookUp) newUpwardID() (id int) {
	interval := UpperBound - LowerBound
	freeIDs := interval - len(lut)
	msg.FatalOnFalse("no new ID possible"+fmt.Sprint(LowerBound, UpperBound, len(lut)), freeIDs > 0)
	id = LowerBound
	if 0 == len(lut) {
		return
	}
	for {
	nextTry:
		for k := range lut {
			if id == k { // id used
				id++
				goto nextTry
			}
		}
		return
	}
}

func (lut LookUp) newDownwardID() (id int) {
	interval := UpperBound - LowerBound
	freeIDs := interval - len(lut)
	msg.FatalOnFalse("no new ID possible"+fmt.Sprint(LowerBound, UpperBound, len(lut)), freeIDs > 0)
	id = UpperBound
	if 0 == len(lut) {
		return
	}
	for {
	nextTry:
		for k := range lut {
			if id == k { // id used
				id--
				goto nextTry
			}
		}
		return
	}
}

/*
// appendIfMissing is appending item to p.List.
// It returns true if item was missing or changed, otherwise false.
func (p *List) appendIfMissing(item Item, verbose bool) (int, bool) {
	fmt.Print("appendIfMissing")
	return 0, false
}

// // appendIfMissing is appending item to p.List.
// // It returns true if item was missing or changed, otherwise false.
// func (p *List) appendIfMissing(item Item, verbose bool) (int, bool) {
// 	for _, e := range p.ItemList {
// 		if e.ID == item.ID { // if id exists
// 			if (e.FmtType == item.FmtType) && (e.FmtStrg == item.FmtStrg) { // identical
// 				if 0 == e.Removed { // is active
// 					return item.ID, false // known i, nothing todo
// 				}
// 				// id is inactive
// 				e.Removed = 0 // i exists again, clear removal, so it is active again
// 				return item.ID, true
// 			}
// 			// arriving here means a data difference for the identical id between the actual file
// 			// and the ID List. So a new ID is generated and goes with the actual file information
// 			// into the ID List. Also the ID inside the file must be replaced with the new ID.
// 			// The legacy ID inside the ID List can not invalidated here because it is possibly
// 			// used  on a different place in unchanged form. The ID invalidation could be done
// 			// globally later in a separate action.
// 			if verbose {
// 				fmt.Println("Same ID", e.ID, "but not identical:", e.FmtType, "?", item.FmtType, "or", e.FmtStrg, "?", item.FmtStrg, "so get a new ID.")
// 				fmt.Println(e)
// 				fmt.Println(item)
// 			}
// 			item.ID = p.newID()
// 			item.Created = int32(time.Now().Unix())
// 			fmt.Println(item)
// 			p.ItemList = append(p.ItemList, item)
// 			// Need to change file! Therefore the (new) ID is delivered back.
// 			return item.ID, true
// 		}
// 		// Do not care about same format for different IDs, what could be done here.
// 		// Having different IDs for identical TRICE's is more an advantage for debugging.
// 		// If for some reason a huge amount of identical TRICE's should get identical
// 		// IDs this could be done here.
// 	}
// 	p.ItemList = append(p.ItemList, item)
// 	return item.ID, true
// }

// ExtendIDList returns id because it could get changed when id is in List with different typ or fmts.
// It is an exported function for simplifying tests in other packets.
func (p *List) ExtendIDList(id int, typ, fmt string, verbose bool) (int, bool) {
	i := Item{
		//ID:      id,
		FmtType: typ,
		FmtStrg: fmt,
		Created: int32(time.Now().Unix()),
		Removed: 0,
	}
	return p.appendIfMissing(i, verbose)
}

// Index returns the index of id inside p.List. If id is not found it returns -1.
func (p *List) Index(id int) int {
	fmt.Println("Index")
	return 0
}

// // Index returns the index of id inside p.List. If id is not found it returns -1.
// func (p *List) Index(id int) int {
// 	for i := range p.ItemList {
// 		iD := p.ItemList[i].ID
// 		if id == iD {
// 			return i
// 		}
// 	}
// 	return -1
// }

// Item returns the trice item on index from the List
func (p *List) Item(index int) Item {
	fmt.Println("Item")
	var i Item
	return i
}

// // Item returns the trice item on index from the List
// func (p *List) Item(index int) Item {
// 	return p.ItemList[index]
// }

// ScZero does replace all ID's in source tree with 0
func ScZero(SrcZ string, cmd *flag.FlagSet) error {
	if SrcZ == "" {
		cmd.PrintDefaults()
		return errors.New("no source tree root specified")
	}
	ZeroSourceTreeIds(SrcZ, !DryRun)
	return nil
}

// ScUpdate is subcommand update
func ScUpdate(fnJSON string) error {
	p := NewList(fnJSON)
	p.ReadListFile()
	if 0 == len(Srcs) {
		Srcs = append(Srcs, "./") // default value
	}
	for i := range Srcs {
		s := Srcs[i]
		srcU := ConditionalFilePath(s)
		if _, err := os.Stat(srcU); err == nil { // path exists
			err = p.update(srcU)
			if nil != err {
				return err
			}
		} else if os.IsNotExist(err) { // path does *not* exist
			fmt.Println(s, " -> ", srcU, "does not exist!")
		} else {
			fmt.Println(s, "Schrodinger: file may or may not exist. See err for details.")
			// Therefore, do *NOT* use !os.IsNotExist(err) to test for file existence
			// https://stackoverflow.com/questions/12518876/how-to-check-if-a-file-exists-in-go
		}
	}
	return nil
}

// update does parse source tree, update IDs and is List
func (p *List) update(dir string) error {
	fmt.Println("update")
	return nil
}
*/
// // update does parse source tree, update IDs and is List
// func (p *List) update(dir string) error {
// 	err := p.Update(dir, !DryRun, Verbose)
// 	if nil != err {
// 		return fmt.Errorf("failed update on %s with %s: %v", dir, p.FnJSON, err)
// 	}
// 	if Verbose {
// 		fmt.Println(len(p.ItemList), "ID's in List", p.FnJSON)
// 	}
// 	return nil
// }

// // newIDLegacyMethod() gets a random ID not used so far.
// // If all IDs used, longest removed ID is reused (TODO)
// func (p *List) newIDLegacyMethod() (id int) {
// start:
// 	for { // this is good enough if id count is less than 2/3 of total count, otherwise it will take too long
// 		id = 20 + rand.Intn(65535) // 2^16=65536, id 0 used for params, ids 1-19 reserved, 515 ids forbidden, so 65000 ids possible // BUG!!!!!!! Must be 65535-20 but many tests need to be adapted!!!!!!!!!!!!!!!!!!!!!!!
// 		ih := uint8(id >> 8)       // todo: endianness
// 		il := uint8(id)
// 		if 0xef == ih || 0x89 == il || 0x89ab == id || 0xabcd == id || 0xcdef == id { // 515 ids forbidden, see bare.go
// 			continue // next try
// 		}
// 		if 0 == len(p.ItemList) {
// 			return
// 		}
// 		for _, item := range p.ItemList { // todo: binary search
// 			if id == item.ID {
// 				goto start // id used
// 			}
// 			return
// 		}
// 	}
// }
