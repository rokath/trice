// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package id list is responsible for id list managing
package id

// list management

import (
	"encoding/json"
	"errors"
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"math/rand"
	"os"
	"path/filepath"
	"runtime"
	"time"
)

var (
	// Verbose gives mor information on output if set. The value is injected from main packages.
	Verbose bool

	// DryRun if set, inhibits real changes
	DryRun bool
)

// Item is the basic element
type Item struct {
	ID      int    `json:"id"`      // identifier
	FmtType string `json:"fmtType"` // format type (bitsize and number of fmt string parameters)
	FmtStrg string `json:"fmtStrg"` // format string
	Created int32  `json:"created"` // utc unix time of creation
	Removed int32  `json:"removed"` // utc unix time of disappearing in processed src directory
}

// List is the trice ID list
type List struct {

	// fnJSON is the filename of the id list
	fnJSON string

	// array is the undelying list array for List
	array [65536]Item

	// list is a slice type containing the ID list.
	list []Item

	savedErr error
}

// NewList creates an ID list instance
func NewList(fnJSON string) *List {
	p := &List{}
	p.fnJSON = fnJSON
	p.list = p.array[:0]
	p.ReadListFile()
	return p
}

// ReadListFile reads idlist file in internal struct and starts a file watcher.
//
// Just in case the idlist file gets updated, the file watcher updates the internals struct.
// This way trice needs not to be restarted during development process.
func (p *List) ReadListFile() {
	if "none" != p.fnJSON {
		b, err := ioutil.ReadFile(p.fnJSON)
		errorFatal(err)
		errorFatal(json.Unmarshal(b, p))
		// TODO: sort for binary search
		go p.FileWatcher()
	}
	if true == Verbose {
		fmt.Println("Read ID list file", p.fnJSON, "with", len(p.list), "items.")
	}
}

// newID() gets a random ID not used so far.
// If all IDs used, longest removed ID is reused (TODO)
func (p *List) newID() int {
	var id int

	for { // this is good enough if id count is less than 2/3 of total count, otherwise it will take too long
		id = 1 + rand.Intn(65535) // 2^16 - 1, 0 is reserved
		new := true
		for _, item := range p.list { // todo: binary search
			ih := uint8(id >> 8) // todo: endianess
			il := uint8(id)
			if id == item.ID || 0xef == ih || 0x89 == il || 0x89ab == id || 0xabcd == id || 0xcdef == id { // see bare.go
				new = false // id found or forbidden, so not usable
				break
			}
		}
		if false == new {
			continue // next try
		}
	}
	return id
}

// appendIfMissing is appending item to p.list.
// It returns true if item was missing or changed, otherwise false.
func (p *List) appendIfMissing(item Item, verbose bool) (int, bool) {
	for _, e := range p.list {
		if e.ID == item.ID { // if id exists
			if (e.FmtType == item.FmtType) && (e.FmtStrg == item.FmtStrg) { // identical
				if 0 == e.Removed { // is active
					return item.ID, false // known i, nothing todo
				}
				// id is inactive
				e.Removed = 0 // i exists again, clear removal, so it is active again
				return item.ID, true
			}
			// arriving here means a data difference for the identical id between the actual file
			// and the ID list. So a new ID is generated and goes with the actual file information
			// into the ID list. Also the ID inside the file must be replaced with the new ID.
			// The legacy ID inside the ID list can not invalidated here because it is possibly
			// used  on a different place in unchanged form. The ID invalidation could be done
			// globally later in a separate action.
			if verbose {
				fmt.Println("e.ID format changed, so get a new ID")
				fmt.Println(e)
				fmt.Println(item)
			}
			item.ID = p.newID()
			item.Created = int32(time.Now().Unix())
			fmt.Println(item)
			p.list = append(p.list, item)
			// Need to change file! Therefore the (new) ID is delivered back.
			return item.ID, true
		}
		// Do not care about same format for different IDs, what could be done here.
		// Having different IDs for identical TRICEs is more an advantage for debugging.
		// If for some reason a huge amount of identical TRICEs should get identical
		// IDs this could be done here.
	}
	p.list = append(p.list, item)
	return item.ID, true
}

// ExtendIDList returns id beause it could get changed when id is in list with different typ or fmts.
// It is an exported function for simplyfing tests in other packets.
func (p *List) extendIDList(id int, typ, fmts string, verbose bool) (int, bool) {
	i := Item{
		ID:      id,
		FmtType: typ,
		FmtStrg: fmts,
		Created: int32(time.Now().Unix()),
		Removed: 0,
	}
	return p.appendIfMissing(i, verbose)
}

func (p *List) writeListFile() error {
	b, err := json.MarshalIndent(p, "", "\t")
	if err != nil {
		return err
	}
	return ioutil.WriteFile(p.fnJSON, b, 0644)
}

// Index returns the index of id inside p.list. If id is not found it returns -1.
func (p *List) Index(id int) int {
	for i := range p.list {
		iD := p.list[i].ID
		if id == iD {
			return i
		}
	}
	p.savedErr = errors.New("unknown ID")
	return -1
}

// ScZero does replace all ID's in sourc tree with 0
func ScZero(SrcZ string, cmd *flag.FlagSet) error {
	if SrcZ == "" {
		cmd.PrintDefaults()
		return errors.New("no source tree root specified")
	}
	zeroSourceTreeIds(SrcZ, !DryRun)
	return nil
}

// ScUpdate is subcommand update
func ScUpdate(fnJSON string) error {
	p := NewList(fnJSON)
	if 0 == len(Srcs) {
		Srcs = append(Srcs, "./") // default value
	}
	for i := range Srcs {
		s := Srcs[i]
		srcU := ConditinalFilePath(s)
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

// update does parse source tree, update IDs and is list
func (p *List) update(dir string) error {
	err := p.Update(dir, !DryRun, Verbose)
	if nil != err {
		return fmt.Errorf("failed update on %s with %s: %v", dir, p.fnJSON, err)
	}
	fmt.Println(len(p.list), "ID's in list", p.fnJSON)
	return nil
}

// ListNotFoundMsg give an output message about not found ID list.ListNotFoundMsg.
//
// Base is used to avoid test issues in different operating systems.
func ListNotFoundMsg(pathname string) {
	if false == Verbose {
		pathname = filepath.Base(pathname) // no path info (used for testing)
	}
	fmt.Println("ID list " + pathname + " not found")
}

// errorFatal ends in osExit(1) if err not nil.
func errorFatal(err error) {
	if nil == err {
		return
	}
	if Verbose {
		_, file, line, _ := runtime.Caller(1)
		log.Fatal(err, " "+filepath.Base(file)+" ", line)
	}
	log.Fatal(err)
}
