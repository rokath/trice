// Copyright 2020 basti@blackoutcloud.de
//                Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver

/*
type TriceReceiver interface {
	SetUp() bool
	CleanUp()
	Start()
	Stop()
	GetTriceChannel() *chan []byte
	GetBufferChannel() *chan []byte
}
*/

type receiver struct {
	name          string
	receivingData bool
	triceChannel  chan []byte
	bufferChannel chan []byte
}

func (p *receiver) GetTriceChannel() *chan []byte {
	return &p.triceChannel
}

func (p *receiver) GetBufferChannel() *chan []byte {
	return &p.bufferChannel
}
