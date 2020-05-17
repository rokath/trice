// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

package receiver

// HTTPReceiver defines ...
type HTTPReceiver struct {
	triceReceiver

	IPaddress []int
	port      int
}

// setUp opens...
func (p *HTTPReceiver) setUp() bool {
	return true
}


// CleanUp washes...
func (p *HTTPReceiver) cleanUp() {
	// clean up after closing webserver
}
