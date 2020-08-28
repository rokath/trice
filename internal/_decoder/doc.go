// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder reads from differently encoded byte streams and writes decoded items to appropriate write channels.
//
// To use it you need a to provide a byte reader like COM or FILE and information about the byte stream encoding.
// The 3 channel interface lets the consumer detect the exact sequence of the decoded data.
//
// The decoder is extendable in a way to get additional write channels for some data type.
// Also the decoding can made be able to handle different encodings in one data stream.
// In future a heuristic approach for auto encoding could be implemented as well.
package decoder
