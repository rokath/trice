// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package cipher provides an encryption and decryption interface.
//
// It abstracts from the used cipher algorithm
package cipher

import (
	"crypto/sha1"
	"fmt"
	"io"

	"github.com/rokath/trice/pkg/msg"
	"golang.org/x/crypto/xtea"
)

// local config values
var (
	// Password is the key one needs to decrypt trice logs if encrypted
	Password string

	// ShowKey if set, allows to see the encryption passphrase
	ShowKey bool

	key []byte

	// cipher is a pointer to the crypto struct filled during initialization
	ci *xtea.Cipher

	// enabled set to true if a -password other than "" was given
	enabled bool
)

// SetUp uses the Password to create a cipher. If Password is "" encryption/decryption is disabled.
func SetUp(w io.Writer) error {
	var err error
	ci, enabled, err = createCipher(w)
	msg.FatalOnErr(err)

	bsize := ci.BlockSize()
	msg.FatalOnTrue(8 != bsize)

	return nil
}

// createCipher prepares decryption, with password "none" the encryption flag is set false, otherwise true
func createCipher(w io.Writer) (*xtea.Cipher, bool, error) {
	switch Password {
	case "0000000000000000":
		key = []byte{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // used for checking only
	case "1000000000000000":
		key = []byte{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // used for checking only
	case "0001000000000000":
		key = []byte{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // used for checking only
	default:
		h := sha1.New() // https://gobyexample.com/sha1-hashes
		h.Write([]byte(Password))
		key = h.Sum(nil)
		key = key[:16] // only first 16 bytes needed as key
	}
	c, err := xtea.NewCipher(key)
	msg.FatalOnErr(err)

	var e bool
	if "" != Password {
		e = true
		if ShowKey {
			fmt.Fprintf(w, "% 20x is XTEA encryption key\n", key)
		}
	}
	return c, e, nil
}

// tested with little endian embedded device
func swap8Bytes(src []byte) []byte {
	b := make([]byte, 8)
	copy(b, src)
	return []byte{b[3], b[2], b[1], b[0], b[7], b[6], b[5], b[4]}
}

// Encrypt8 translates a byte slice in a protected slice of length 8.
//
// The input must be exactly 8 bytes; otherwise the function logs an info message.
func Encrypt8(b []byte) (e []byte) {
	msg.InfoOnFalse(8 == len(b), "Buffer len is not 8.")
	if enabled {
		src := swap8Bytes(b) // HtoN
		dst := make([]byte, 8)
		ci.Encrypt(dst, src) // assumes network order
		e = swap8Bytes(dst)  // NtoH (should be done in target before decrypt)
	} else {
		e = b
	}
	return
}

// Decrypt8 translates an encryption protected byte slice back in a slice of length 8.
//
// The input must be exactly 8 bytes; otherwise the function logs an info message.
func Decrypt8(b []byte) (d []byte) {
	msg.InfoOnFalse(8 == len(b), "Buffer len is not 8.")
	if enabled {
		src := swap8Bytes(b) // HtoN (not done in Target after encrypt)
		dst := make([]byte, 8)
		ci.Decrypt(dst, src) // assumes network order
		d = swap8Bytes(dst)  // NtoH
	} else {
		d = b
	}
	return
}

// decrypt8 translates src, an encryption protected byte slice, back in dst, a byte slice of length 8.
//
// src and dst are expected to be exactly 8 bytes long.
func decrypt8(dst, src []byte) {
	swap := src
	if enabled {
		swap = swap8Bytes(src) // HtoN (not done in Target after encrypt)
		ci.Decrypt(dst, swap)  // assumes network order
		swap = swap8Bytes(dst) // NtoH
	}
	_ = copy(dst, swap)
}

// encrypt8 translates byte slice src, in an encryption protected byte slice dst.
//
// src and dst are expected to be exactly 8 bytes long.
func encrypt8(dst, src []byte) {
	swap := src
	if enabled {
		swap = swap8Bytes(src) // HtoN
		ci.Encrypt(dst, swap)  // assumes network order
		swap = swap8Bytes(dst) // NtoH (not done in Target after receive)
	}
	_ = copy(dst, swap)
}

// Decrypt converts src into dst and returns count of converted bytes.
// Only multiple of 8 are convertable, so last 0-7 bytes are not convertable and c is a multiple of 8.
// The smaller byte slice limits the conversion.
func Decrypt(dst, src []byte) (c int) {
	for c = 0; c+8 <= len(dst) && c+8 <= len(src); c += 8 {
		decrypt8(dst[c:c+8], src[c:c+8])
	}
	return
}

//  // Encrypt converts src into dst and returns count of converted bytes.
//  // Only multiple of 8 are convertable, so last 0-7 bytes are not convertable and c is a multiple of 8.
//  // The smaller byte slice limits the conversion.
//  func Encrypt(dst, src []byte) (c int) {
//  	for c = 0; c+8 <= len(dst) && c+8 <= len(src); c += 8 {
//  		encrypt8(dst[c:c+8], src[c:c+8])
//  	}
//  	return
//  }
