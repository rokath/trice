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

// NewLut returns a lut map generated from file named fn.
func NewLut(fn string) Lut {
	lut := new(Lut)
	lut.i2f = make(I2F)
	lut.f2i = make(F2I)
	msg.FatalOnErr("", lut.fromFile(fn))
	if true == Verbose {
		fmt.Println("Read ID List file", fn, "with", len(lut), "items.")
	}
	return
}

/* NewItemLut returns a reversed lut map from file.
func NewItemLut(fn string) (ilut ItemLookUp) {
	lut := make(LookUp)
	msg.FatalOnErr("", lut.fromFile(fn))
	if true == Verbose {
		fmt.Println("Read ID List file", fn, "with", len(lut), "items.")
	}
	return
}*/

// reverse exchanges the ID:Item pairs in lut into Item:ID pairs and returns that as a new map.
// If the same Item is behind different IDs only one of them survives.
func (lut Lut) sync() {
	for id, fmt := range lut.i2f {
		lut.f2i[fmt] = id
	}
}

// update enriches lut with values from ilut.
//
// ilut normally was generated with lut.reverse. Durring source tree parsing new item:ID pairs
func (lut LookUp) update(ilut ItemLookUp) {
	for item, id := range ilut {
		lut[id] = item
	}
}

// newID() gets a random ID not used so far.
// If all IDs used, longest removed ID is reused (TODO)
func (lut LookUp) newID() (id int) {
	switch SearchMethod {
	case "random":
		return lut.newRandomID(Min, Max)
	case "upward":
		return lut.newUpwardID(Min, Max)
	case "downward":
		return lut.newDownwardID(Min, Max)
	}
	msg.Info(fmt.Sprint("ERROR:", SearchMethod, "is unknown ID search method."))
	return 0
}

// newRandomID provides a random free ID inside interval [min,max].
// The delivered id is usable as key for lut, but not added. So calling fn twice without adding to lut could give the same value back.
func (lut LookUp) newRandomID(min, max int) (id int) {
	interval := max - min
	freeIDs := interval + 1 - len(lut)
	msg.FatalOnFalse("no new ID possible, "+fmt.Sprint(min, max, len(lut)), freeIDs > 0)
	wrnLimit := interval >> 2 // 25%
	msg.InfoOnTrue("WARNING: Less than 25% IDs free!", freeIDs < wrnLimit)
	id = min + rand.Intn(interval+1)
	if 0 == len(lut) {
		return
	}
	for {
	nextTry:
		for k := range lut {
			if id == k { // id used
				fmt.Println("ID", id, "used, next try...")
				id = min + rand.Intn(interval)
				goto nextTry
			}
		}
		return
	}
}

// newUpwardID provides the smallest free ID inside interval [min,max].
// The delivered id is usable as key for lut, but not added. So calling fn twice without adding to lut gives the same value back.
func (lut LookUp) newUpwardID(min, max int) (id int) {
	interval := max - min
	freeIDs := interval + 1 - len(lut)
	msg.FatalOnFalse("no new ID possible: "+fmt.Sprint("min=", min, ", max=", max, ", len(lut)=", len(lut)), freeIDs > 0)
	id = min
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

// newDownwardID provides the biggest free ID inside interval [min,max].
// The delivered id is usable as key for lut, but not added. So calling fn twice without adding to lut gives the same value back.
func (lut LookUp) newDownwardID(min, max int) (id int) {
	interval := max - min
	freeIDs := interval + 1 - len(lut)
	msg.FatalOnFalse("no new ID possible: "+fmt.Sprint("min=", min, ", max=", max, ", len(lut)=", len(lut)), freeIDs > 0)
	id = max
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

// FromJSON converts JSON byte slice to lut.
func (lut Lut) FromJSON(b []byte) (err error) {
	if 0 < len(b) {
		err = json.Unmarshal(b, &lut)
	}
	return
}

// fromFile reads file fn into lut. Existing keys are overwritten, lut is extended with new keys.
func (lut Lut) fromFile(fn string) error {
	b, err := ioutil.ReadFile(fn)
	msg.FatalOnErr("May be need to create an empty file first? (Safety feature)", err)
	return lut.i2f.FromJSON(b)
}

// toJSON converts lut into JSON byte slice in human readable form.
func (lut LookUp) toJSON() ([]byte, error) {
	return json.MarshalIndent(lut, "", "\t")
}

// toFile writes lut into file fn as indented JSON.
func (lut LookUp) toFile(fn string) (err error) {
	var b []byte
	b, err = lut.toJSON()
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
