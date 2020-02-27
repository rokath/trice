//
// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package receiver

type HTTPReceiver struct {
	receiver

	ip_address []int
	port       int
}

func (self *HTTPReceiver) SetUp() bool {
	return true
}

// starts receiving of serial data
func (self *HTTPReceiver) Start() {
	// start webserver
}

// Stops receiving of serial data
func (self *HTTPReceiver) Stop() {
	// stop webserver
}

func (self *HTTPReceiver) CleanUp() {
	// clean up after closing webserver
}
