// SPDX-License-Identifier: MIT

package id_test

import (
	"testing"
	"time"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
)

// assertFileNotExists asserts, that fn does not exist
func assertFileNotExists(t *testing.T, FSys *afero.Afero, fn string) {
	_, err := FSys.Stat(fn)
	assert.Error(t, err)
}

// assertMTime compares mtime with fName modification times to be equal.
func assertMTime(t *testing.T, FSys *afero.Afero, mtime time.Time, fName string) {
	stat, err := FSys.Stat(fName)
	assert.Nil(t, err)
	assert.True(t, time.Time.Equal(mtime, stat.ModTime()))
}

// assertNotMTime compares mtime with fName modification times to be different.
func assertNotMTime(t *testing.T, FSys *afero.Afero, mtime time.Time, fName string) {
	stat, err := FSys.Stat(fName)
	assert.Nil(t, err)
	assert.False(t, time.Time.Equal(mtime, stat.ModTime()))
}

// assertEqualMTimes comparesfn0 and fn1 modification times to be equal.
func assertEqualMTimes(t *testing.T, FSys *afero.Afero, fn0, fn1 string) {
	fn0Stat, err := FSys.Stat(fn0)
	assert.Nil(t, err)
	fn1Stat, err := FSys.Stat(fn1)
	assert.Nil(t, err)
	assert.True(t, time.Time.Equal(fn0Stat.ModTime(), fn1Stat.ModTime()))
}

// assertEqualMTimes comparesfn0 and fn1 modification times to be equal.
func assertNotEqualMTimes(t *testing.T, FSys *afero.Afero, fn0, fn1 string) {
	fn0Stat, err := FSys.Stat(fn0)
	assert.Nil(t, err)
	fn1Stat, err := FSys.Stat(fn1)
	assert.Nil(t, err)
	assert.False(t, time.Time.Equal(fn0Stat.ModTime(), fn1Stat.ModTime()))
}

// assertFileContent asserts that the file content matches the expected string.
func assertFileContent(t *testing.T, FSys *afero.Afero, fName, exp string) {
	act, e := FSys.ReadFile(fName)
	assert.Nil(t, e)
	assert.Equal(t, exp, string(act))
}

// mTime returns the modification time of the named file.
func mTime(t *testing.T, FSys *afero.Afero, fName string) time.Time {
	stat, err := FSys.Stat(fName)
	assert.Nil(t, err)
	return stat.ModTime()
}

// assertFileCreate creates a file in the test filesystem and verifies the operation succeeds.
func assertFileCreate(t *testing.T, FSys *afero.Afero, fName, src string) {
	assert.Nil(t, FSys.WriteFile(fName, []byte(src), 0777)) // create src file
}
