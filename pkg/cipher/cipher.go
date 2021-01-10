// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package cipher provides an encryption and decryption interface.
//
// It abstracts from the used cipher algorithm
package cipher

import (
	"crypto/sha1"
	"errors"
	"fmt"

	"golang.org/x/crypto/xtea"
)

// local config values
var (
	// Password is the key one needs to derypt trice logs if enncrypted
	Password string

	// ShowKey, if set, allows to see the encryption passphrase
	ShowKey bool

	// cipher is a pointer to the cryptpo struct filled during initialization
	ci *xtea.Cipher

	// enabled set to true if a -password other than "none" was given
	enabled bool
)

// SetUp uses the Password to create a cipher. If Password is "none" encryption/decryption is disabled.
func SetUp() error {
	var err error
	ci, enabled, err = createCipher()
	if nil != err {
		return err
	}
	bsize := ci.BlockSize()
	if 8 != bsize {
		fmt.Println("cipher blocksize", bsize)
		return errors.New("unexpected value")
	}
	return nil
}

// createCipher prepares decryption, with password "none" the encryption flag is set false, otherwise true
func createCipher() (*xtea.Cipher, bool, error) {
	h := sha1.New() // https://gobyexample.com/sha1-hashes
	h.Write([]byte(Password))
	key := h.Sum(nil)
	key = key[:16] // only first 16 bytes needed as key

	c, err := xtea.NewCipher(key)
	if err != nil {
		return nil, false, errors.New("NewCipher returned error")
	}
	var e bool
	if "none" != Password {
		e = true
		if true == ShowKey {
			fmt.Printf("% 20x is XTEA encryption key\n", key)
		}
	} else if true == ShowKey {
		fmt.Printf("no encryption\n")
	}
	return c, e, nil
}

//! tested with little endian embedded device
func swap8Bytes(b []byte) []byte {
	return []byte{b[3], b[2], b[1], b[0], b[7], b[6], b[5], b[4]}
}

// Encrypt8 translates a byte slice in a protected slice of length 8.
//
// Shorter slices are extented with 0x16 until length 8.
// Langer slices are truncated to length 8.
func Encrypt8(b []byte) []byte {
	for len(b) < 8 {
		b = append(b, 0x16) // if len(b) == 4, {0x16,0x16,0x16,0x16} is appended and this is a TRICE_SYNC message
	}
	b = b[:8]
	if enabled {
		//	b = swap8Bytes(b)
		ci.Encrypt(b, b)
		//	b = swap8Bytes(b)
	}
	return b
}

// Decrypt8 translates an Encrypt protected byte slice back in a slice of length 8.
//
// Shorter slices are extented with 0 until length 8.
// Langer slices are truncated to length 8.
func Decrypt8(b []byte) []byte {
	if enabled {
		//b = swap8Bytes(b)
		ci.Decrypt(b, b)
		//b = swap8Bytes(b)
	}
	return b
}
