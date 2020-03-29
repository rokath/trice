// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver

type HTTPReceiver struct {
	receiver

	ip_address []int
	port       int
}

// SetUp opens...
func (self *HTTPReceiver) SetUp() bool {
	return true
}

// Start starts receiving of serial data
func (self *HTTPReceiver) Start() {
	// start webserver
}

// Stop stops receiving of serial data
func (self *HTTPReceiver) Stop() {
	// stop webserver
}

// CleanUp washes...
func (self *HTTPReceiver) CleanUp() {
	// clean up after closing webserver
}
