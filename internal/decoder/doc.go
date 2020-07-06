// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decoder reads from differently encoded byte streams and writes decoded items to appropriate writers.
//
// To use it you need a to provide a byte reader like COM or FILE and information about the byte stream encoding.
// Also a new decoder expects writers according the expected items.
// decoder has a StringWriter interface where it writes decoded trices as string slice.
// Because strings can arrive as trices but also as command messages the decoder needs to handle both in one turn
// for correct synchronisation.
// The decoder is extendable in a way to get additional writers for some data type.
// Also the decoding can made be able to handle different encodings in one data stream.
// In future a heuristic approach for auto encoding could be implemented as well.
package decoder
