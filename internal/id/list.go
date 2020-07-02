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
	"math/rand"
	"os"
	"path/filepath"
	"time"

	"github.com/rokath/trice/internal/global"
	"github.com/rokath/trice/pkg/lib"
)

// Item is the basic element
type Item struct {
	ID      int    `json:"id"`      // identifier
	FmtType string `json:"fmtType"` // format type (bitsize and number of fmt string parameters)
	FmtStrg string `json:"fmtStrg"` // format string
	Created int32  `json:"created"` // utc unix time of creation
	Removed int32  `json:"removed"` // utc unix time of disappearing in processed src directory
}

// ListT is a slice type containing the ID list
type ListT []Item

var (
	// List is the internal List instancem for 16 bit IDs enough
	List = make(ListT, 0, 65536)

	// FnJSON is the filename of the id list
	FnJSON string

	// DryRun if set, inhibits real changes
	DryRun bool
)

// newID() gets a random ID not used so far.
// If all IDs used, longest removed ID is reused (TODO)
func (p *ListT) newID() (int, error) {
	var i, id int

	for { // this is good enough if id count is less than 2/3 of total count, otherwise it will take too long
		id = 1 + rand.Intn(65535) // 2^16 - 1, 0 is reserved
		new := true
		for i = range *p {
			if id == (*p)[i].ID {
				new = false // id found, so not usable
				break
			}
		}
		if false == new {
			continue // id used already, next try
		}
		n := len(*p)
		if (0 == i && 0 == n) || i+1 == n {
			return id, nil
		}
	}
}

// appendIfMissing is appending i idItem to *p.
// It returns true if item was missing or changed, otherwise false.
func (p *ListT) appendIfMissing(i Item, verbose bool) (int, bool) {
	for _, e := range *p {
		if e.ID == i.ID { // if id exists
			if (e.FmtType == i.FmtType) && (e.FmtStrg == i.FmtStrg) { // identical
				if 0 == e.Removed { // is active
					return i.ID, false // known i, nothing todo
				}
				// id is inactive
				e.Removed = 0 // i exists again, clear removal, so it is active again
				return i.ID, true
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
				fmt.Println(i)
			}
			i.ID, _ = p.newID()
			i.Created = int32(time.Now().Unix())
			fmt.Println(i)
			*p = append(*p, i)
			// Need to change file! Therefore the (new) ID is delivered back.
			return i.ID, true
		}
		// Do not care about same format for different IDs, what could be done here.
		// Having different IDs for identical TRICEs is more an advantage for debugging.
		// If for some reason a huge amount of identical TRICEs should get identical
		// IDs this could be done here.
	}
	*p = append(*p, i)
	return i.ID, true
}

// extendIDList returns id beause it could get changed when id is in list with different typ or fmts.
func (p *ListT) extendIDList(id int, typ, fmts string, verbose bool) (int, bool) {
	i := Item{
		ID:      id,
		FmtType: typ,
		FmtStrg: fmts,
		Created: int32(time.Now().Unix()),
		Removed: 0,
	}
	return p.appendIfMissing(i, verbose)
}

// Read is reading a JSON file fn and returning a slice
func (p *ListT) Read(fn string) error {
	b, err := ioutil.ReadFile(fn)
	if err != nil {
		return fmt.Errorf("failed to read %s: %v", fn, err)
	}
	err = json.Unmarshal(b, p)
	return err
}

func (p *ListT) write(filename string) error {
	b, err := json.MarshalIndent(p, "", "\t")
	if err != nil {
		return err
	}
	return ioutil.WriteFile(filename, b, 0644)
}

// Index returns the index of 'ID' inside 'l'. If 'ID' is not inside 'l' Index() returns 0 and an error.
//
// todo: replace it with functionality in InexEx
func Index(i int, l ListT) (int, error) {
	for x := range l {
		k := l[x].ID
		if i == k {
			return x, nil
		}
	}
	return 0, errors.New("unknown ID")
}

// IndexEx returns the index of id inside List. If is is not inside List Index returns 0 and an error.
func IndexEx(id int) (int, error) {
	for i := range List {
		iD := List[i].ID
		if id == iD {
			return i, nil
		}
	}
	return 0, errors.New("unknown ID")
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
func ScUpdate() error {

	if 0 == len(lib.Srcs) {
		lib.Srcs = append(lib.Srcs, "./") // default value
	}
	for i := range lib.Srcs {
		s := lib.Srcs[i]
		srcU := lib.ConditinalFilePath(s)
		if _, err := os.Stat(srcU); err == nil { // path exists
			err = update(srcU, FnJSON)
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
func update(dir, fn string) error {
	err := List.Update(dir, fn, !DryRun, global.Verbose)
	if nil != err {
		return fmt.Errorf("failed update on %s with %s: %v", dir, fn, err)
	}
	fmt.Println(len(List), "ID's in list", fn)
	return nil
}

// ListNotFoundMsg give an output message about not found ID list.ListNotFoundMsg.
//
// Base is used to avoid test issues in different operating systems.
func ListNotFoundMsg(pathname string) {
	if false == global.Verbose {
		pathname = filepath.Base(pathname) // no path info (used for testing)
	}
	fmt.Println("ID list " + pathname + " not found")
}

// ReadListFile reads idlist file in internal struct and starts a file watcher.
//
// Just in case the idlist file gets updated, the file watcher updates the internals struct.
// This way trice needs not to be restarted during development process.
func ReadListFile() error {
	if "none" != FnJSON {
		// setup ip list
		err := List.Read(FnJSON)
		if nil != err {
			//fmt.Println("ID list " + path.Base(id.FnJSON) + " not found, exit")
			ListNotFoundMsg(FnJSON)
			return errors.New("file not found")
		}
		go List.FileWatcher()
	}
	if true == global.Verbose {
		fmt.Println("id list file", FnJSON, "with", len(List), "items", "on device", global.Source)
	}
	return nil
}
