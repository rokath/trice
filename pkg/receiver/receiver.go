//
// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package receiver

type TriceReceiver interface {
	SetUp() bool
	CleanUp()
	Start()
	Stop()
	GetReceiveChannel() *chan []byte
}

type receiver struct {
	name           string
	receiving_data bool
	bytes_channel  chan []byte
}

func (self *receiver) GetReceiveChannel() *chan []byte {
	return &self.bytes_channel
}
