// SPDX-License-Identifier: MIT

// Package tst_test contains black-box tests for pkg/tst helpers.
package tst_test

import (
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"

	"github.com/rokath/trice/pkg/tst"
)

// TestTempFileName verifies the expected behavior.
func TestTempFileName(t *testing.T) {
	s := tst.TempFileName("AB_*.CD")
	b := filepath.Base(s)
	assert.True(t, strings.HasPrefix(b, "AB_"))
	assert.True(t, strings.HasSuffix(b, ".CD"))
	assert.True(t, len(s) > 10)
}

// TestEqual verifies the expected behavior.
func TestEqual(t *testing.T) {
	assert.Equal(t, 33, 33)
}

// TestEqualLines verifies the expected behavior.
func TestEqualLines(t *testing.T) {
	exp := "Hello\r\nWorld\r\n"
	act := "Hello\nWorld\n"
	tst.EqualLines(t, exp, act)
}

// TestEqualLinesIgnoresWhitespaceNoise verifies the expected behavior.
func TestEqualLinesIgnoresWhitespaceNoise(t *testing.T) {
	exp := " Hello\tworld \n"
	act := "Hello  world\n"
	tst.EqualLines(t, exp, act)
}

// TestEqualTextFiles verifies the expected behavior.
func TestEqualTextFiles(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewOsFs()} // os.DirFS("")
	fd0, e0 := afero.TempFile(fSys, "", "*.txt")
	assert.Nil(t, e0)
	defer func() {
		assert.Nil(t, fd0.Close())
		assert.Nil(t, fSys.Remove(fd0.Name()))
	}()

	fd1, e1 := afero.TempFile(fSys, "", "*.txt")
	assert.Nil(t, e1)
	defer func() {
		assert.Nil(t, fd1.Close())
		assert.Nil(t, fSys.Remove(fd1.Name()))
	}()

	_, e2 := fd0.WriteString("Hello\r\nWorld\r\n")
	assert.Nil(t, e2)
	_, e3 := fd1.WriteString("Hello\nWorld\n")
	assert.Nil(t, e3)
	tst.EqualTextFiles(t, fSys, fd0.Name(), fd1.Name())
}

// TestEqualFiles verifies the expected behavior.
func TestEqualFiles(t *testing.T) {
	tst.EqualLines(t, os.Args[0], os.Args[0])
}

// TestNormalizeMapString verifies the expected behavior.
func TestNormalizeMapString(t *testing.T) {
	in := "map[12:{ t11 s11 } 13:{ t13 s13 }]"
	exp := "map[12:{t11 s11} 13:{t13 s13}]"
	assert.Equal(t, exp, tst.NormalizeMapString(in))
}

// TestCaptureStdOut verifies the expected behavior.
func TestCaptureStdOut(t *testing.T) {
	out := tst.CaptureStdOut(func() {
		_, _ = os.Stdout.WriteString("captured")
	})
	assert.Equal(t, "captured", out)
}

// TestAssertEqualFiles verifies the expected behavior.
func TestAssertEqualFiles(t *testing.T) {
	f0 := tst.TempFileName("equal0-*.txt")
	f1 := tst.TempFileName("equal1-*.txt")
	assert.Nil(t, os.WriteFile(f0, []byte("same"), 0o644))
	assert.Nil(t, os.WriteFile(f1, []byte("same"), 0o644))
	defer func() {
		_ = os.Remove(f0)
		_ = os.Remove(f1)
	}()
	tst.AssertEqualFiles(t, f0, f1)
}
