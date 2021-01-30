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

// NewLut returns a look-up map generated from JSON map file named fn.
func NewLut(fn string) TriceIDLookUp {
	lu := new(TriceIDLookUp)
	msg.FatalOnErr(lu.fromFile(fn))
	if true == Verbose {
		fmt.Println("Read ID List file", fn, "with", len(*lu), "items.")
	}
	return *lu
}

// newID() gets a random ID not used so far.
// The delivered id is usable as key for lu, but not added. So calling fn twice without adding to lu could give the same value back.
func (lu TriceIDLookUp) newID() (id TriceID) {
	switch SearchMethod {
	case "random":
		return lu.newRandomID(Min, Max)
	case "upward":
		return lu.newUpwardID(Min, Max)
	case "downward":
		return lu.newDownwardID(Min, Max)
	}
	msg.Info(fmt.Sprint("ERROR:", SearchMethod, "is unknown ID search method."))
	return 0
}

// newRandomID provides a random free ID inside interval [min,max].
// The delivered id is usable as key for lu, but not added. So calling fn twice without adding to lu could give the same value back.
func (lu TriceIDLookUp) newRandomID(min, max TriceID) (id TriceID) {
	interval := int(max - min)
	freeIDs := interval + 1 - len(lu)
	msg.FatalOnFalse(freeIDs > 0, "no new ID possible, "+fmt.Sprint(min, max, len(lu)))
	wrnLimit := interval >> 2 // 25%
	msg.InfoOnTrue(freeIDs < wrnLimit, "WARNING: Less than 25% IDs free!")
	id = min + TriceID(rand.Intn(interval+1))
	if 0 == len(lu) {
		return
	}
	for {
	nextTry:
		for k := range lu {
			if id == k { // id used
				fmt.Println("ID", id, "used, next try...")
				id = min + TriceID(rand.Intn(interval))
				goto nextTry
			}
		}
		return
	}
}

// newUpwardID provides the smallest free ID inside interval [min,max].
// The delivered id is usable as key for lut, but not added. So calling fn twice without adding to lu gives the same value back.
func (lu TriceIDLookUp) newUpwardID(min, max TriceID) (id TriceID) {
	interval := int(max - min)
	freeIDs := interval + 1 - len(lu)
	msg.FatalOnFalse(freeIDs > 0, "no new ID possible: "+fmt.Sprint("min=", min, ", max=", max, ", used=", len(lu)))
	id = min
	if 0 == len(lu) {
		return
	}
	for {
	nextTry:
		for k := range lu {
			if id == k { // id used
				id++
				goto nextTry
			}
		}
		return
	}
}

// newDownwardID provides the biggest free ID inside interval [min,max].
// The delivered id is usable as key for lut, but not added. So calling fn twice without adding to lu gives the same value back.
func (lu TriceIDLookUp) newDownwardID(min, max TriceID) (id TriceID) {
	interval := int(max - min)
	freeIDs := interval + 1 - len(lu)
	msg.FatalOnFalse(freeIDs > 0, "no new ID possible: "+fmt.Sprint("min=", min, ", max=", max, ", used=", len(lu)))
	id = max
	if 0 == len(lu) {
		return
	}
	for {
	nextTry:
		for k := range lu {
			if id == k { // id used
				id--
				goto nextTry
			}
		}
		return
	}
}

// FromJSON converts JSON byte slice to lut.
func (lu TriceIDLookUp) FromJSON(b []byte) (err error) {
	if 0 < len(b) {
		err = json.Unmarshal(b, &lu)
	}
	return
}

// fromFile reads file fn into lut. Existing keys are overwritten, lut is extended with new keys.
func (lu TriceIDLookUp) fromFile(fn string) error {
	b, err := ioutil.ReadFile(fn)
	msg.FatalInfoOnErr(err, "May be need to create an empty file first? (Safety feature)")
	return lu.FromJSON(b)
}

// toJSON converts lut into JSON byte slice in human readable form.
func (lu TriceIDLookUp) toJSON() ([]byte, error) {
	return json.MarshalIndent(lu, "", "\t")
}

// toFile writes lut into file fn as indented JSON.
func (lu TriceIDLookUp) toFile(fn string) (err error) {
	var b []byte
	b, err = lu.toJSON()
	msg.FatalOnErr(err)
	var f *os.File
	f, err = os.Create(fn)
	msg.FatalOnErr(err)
	defer func() {
		err = f.Close()
	}()
	_, err = f.Write(b)
	return
}

// reverse returns a reversed map. If differnt triceID's asigned to several equal TriceFmt only one of the TriceID gets it into tflu.
func (lu TriceIDLookUp) reverse() (tflu TriceFmtLookUp) {
	if nil == tflu {
		tflu = make(TriceFmtLookUp)
	}
	for id, fm := range lu {
		tflu[fm] = id
	}
	return
}

// reverse returns a reversed map.  If differnt triceFmt's asigned to several equal TriceFmt, this is an unexpectd and unhandled error and only one of the TriceFmt's gets it into lu.
func (tflu TriceFmtLookUp) reverse() (lu TriceIDLookUp) {
	if nil == tflu {
		tflu = make(TriceFmtLookUp)
	}
	for fm, id := range tflu {
		lu[id] = fm
	}
	return
}
