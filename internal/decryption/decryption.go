// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package decrypt
// It provides optional decryption and activates receiver package,
// which in turn calls emit.Trice on every received trice.

package decryption

import (
	"crypto/sha1"
	"errors"
	"fmt"

	"github.com/rokath/trice/internal/receiver"
	"golang.org/x/crypto/xtea"
)

// local config values
var (
	// Password is the key one needs to derypt trice logs if enncrypted
	Password string

	// ShowPassword, if set, allows to see the encryption passphrase
	ShowPassword bool
)

func SetUp() error {
	var err error
	receiver.Cipher, receiver.Crypto, err = createCipher()
	if nil != err {
		return err
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
		if true == ShowPassword {
			fmt.Printf("% 20x is XTEA encryption key\n", key)
		}
	} else if true == ShowPassword {
		fmt.Printf("no encryption\n")
	}
	return c, e, nil
}
