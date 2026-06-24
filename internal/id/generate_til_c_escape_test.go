// SPDX-License-Identifier: MIT

package id

import (
	"fmt"
	"strconv"
	"strings"
	"testing"
)

func TestTilCFormatLiteralInterpretsCStringEscapesOnce(t *testing.T) {
	tests := []struct {
		name string
		in   string
		want string
	}{
		{name: "newline", in: `tick=%u\n`, want: "tick=%u\n"},
		{name: "carriage return", in: `\rTriceCheck %4d`, want: "\rTriceCheck %4d"},
		{name: "embedded quotes", in: `\"att:%s.\n\"`, want: "\"att:%s.\n\""},
		{name: "literal backslashes", in: `path=C:\\temp\\demo\n`, want: "path=C:\\temp\\demo\n"},
		{name: "unknown escape remains literal", in: `raw=\q`, want: `raw=\q`},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got, want := tilCFormatLiteral(tt.in), strconv.Quote(tt.want); got != want {
				t.Fatalf("tilCFormatLiteral(%q) = %q, want %q", tt.in, got, want)
			}
		})
	}
}

func TestToListTilCUsesCStringSemanticsForGeneratedFormatLiterals(t *testing.T) {
	defer Setup(t)()

	til := TriceIDLookUp{
		1001: {Type: "trice", Strg: `tick=%u\n`},
		1002: {Type: "trice", Strg: `\rTriceCheck %4d`},
		1003: {Type: "trice", Strg: `SAlias_Strg('\"att:%s.\n\"')SAlias_Strg`},

		// The original C spelling for literal backslashes is "\\",
		// otherwise "\t" would correctly become a tab.
		1004: {Type: "trice_0", Strg: `path=C:\\temp\\demo\n`},
	}

	text, err := til.toListTilC("til.c")
	if err != nil {
		t.Fatal(err)
	}
	generated := string(text)

	assertGeneratedTilCFormat(t, generated, 1001, 1, "tick=%u\n")
	assertGeneratedTilCFormat(t, generated, 1002, 1, "\rTriceCheck %4d")
	assertGeneratedTilCFormat(t, generated, 1003, 1, "SAlias_Strg('\"att:%s.\n\"')SAlias_Strg")
	assertGeneratedTilCFormat(t, generated, 1004, 0, "path=C:\\temp\\demo\n")

	if strings.Contains(generated, `tick=%u\\n`) {
		t.Fatalf("generated til.c still contains a double-escaped newline: %s", generated)
	}
}

func assertGeneratedTilCFormat(t *testing.T, generated string, id int, paramCount int, runtimeFormat string) {
	t.Helper()

	want := strconv.Quote(runtimeFormat)
	needle := fmt.Sprintf("%5du,  32u, %du, %s", id, paramCount, want)
	if !strings.Contains(generated, needle) {
		t.Fatalf("generated til.c does not contain %q\n--- generated ---\n%s", needle, generated)
	}
}
