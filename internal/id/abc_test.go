// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestAbcInsertCleanAndTil(t *testing.T) {
	defer Setup(t)()

	Min = 1200
	Max = 1300
	SearchMethod = "upward"
	Srcs = ArrayFlag{SFName}

	source := `void AbcParserFixture(void) {
	uint16_t stamp16 = 0xeb61u;
	uint32_t stamp32 = 0xc0de3020u;
	int8_t b8[4] = { 1, 2, 3, 4 };
	int16_t b16[2] = { 100, 200 };
	int32_t b32[2] = { 1000, 2000 };

	triceC("cmd:motor_stop");
	TriceC("cmd:motor_stop16", stamp16);
	TRiceC("cmd:motor_stop32", stamp32);

	trice8C("cmd:set_leds", b8, 4);
	Trice16C("cmd:set_pwm16", stamp16, b16, 2);
	TRice32C("cmd:set_time32", stamp32, b32, 2);

	TRIce32_C(iD(0), "cmd:explicit_no_stamp", b32, 2);
	TRICe32_C(iD(0), "cmd:explicit_stamp16", stamp16, b32, 2);
	TRICE32_C(iD(0), "cmd:explicit_stamp32", stamp32, b32, 2);

	TRice8F("rpc:legacy_not_abc", b8, 4);
}
`
	require.NoError(t, FSys.WriteFile(SFName, []byte(source), 0o666))

	var w bytes.Buffer
	require.NoError(t, SubCmdIdInsert(&w, FSys))

	inserted, err := FSys.ReadFile(SFName)
	require.NoError(t, err)
	insertedText := string(inserted)
	for _, want := range []string{
		`triceC(iD(1200), "cmd:motor_stop")`,
		`TriceC(iD(1201), "cmd:motor_stop16", stamp16)`,
		`TRiceC(iD(1202), "cmd:motor_stop32", stamp32)`,
		`trice8C(iD(1203), "cmd:set_leds", b8, 4)`,
		`Trice16C(iD(1204), "cmd:set_pwm16", stamp16, b16, 2)`,
		`TRice32C(iD(1205), "cmd:set_time32", stamp32, b32, 2)`,
		`TRIce32_C(iD(1206), "cmd:explicit_no_stamp", b32, 2)`,
		`TRICe32_C(iD(1207), "cmd:explicit_stamp16", stamp16, b32, 2)`,
		`TRICE32_C(iD(1208), "cmd:explicit_stamp32", stamp32, b32, 2)`,
	} {
		assert.Contains(t, insertedText, want)
	}
	assert.NotContains(t, insertedText, `TriceC(Id(`)
	assert.NotContains(t, insertedText, `TRiceC(ID(`)

	til := NewLut(&w, FSys, FnJSON)
	assert.Equal(t, TriceFmt{Type: "triceC", Strg: "cmd:motor_stop"}, til[1200])
	assert.Equal(t, TriceFmt{Type: "TriceC", Strg: "cmd:motor_stop16"}, til[1201])
	assert.Equal(t, TriceFmt{Type: "TRiceC", Strg: "cmd:motor_stop32"}, til[1202])
	assert.Equal(t, TriceFmt{Type: "trice8C", Strg: "cmd:set_leds"}, til[1203])
	assert.Equal(t, TriceFmt{Type: "Trice16C", Strg: "cmd:set_pwm16"}, til[1204])
	assert.Equal(t, TriceFmt{Type: "TRice32C", Strg: "cmd:set_time32"}, til[1205])
	assert.Equal(t, TriceFmt{Type: "TRIce32_C", Strg: "cmd:explicit_no_stamp"}, til[1206])
	assert.Equal(t, TriceFmt{Type: "TRICe32_C", Strg: "cmd:explicit_stamp16"}, til[1207])
	assert.Equal(t, TriceFmt{Type: "TRICE32_C", Strg: "cmd:explicit_stamp32"}, til[1208])
	assert.Equal(t, TriceFmt{Type: "TRice8F", Strg: "rpc:legacy_not_abc"}, til[1209])

	require.NoError(t, SubCmdIdClean(&w, FSys))
	cleaned, err := FSys.ReadFile(SFName)
	require.NoError(t, err)
	cleanedText := string(cleaned)
	assert.Contains(t, cleanedText, `triceC("cmd:motor_stop")`)
	assert.Contains(t, cleanedText, `TriceC("cmd:motor_stop16", stamp16)`)
	assert.Contains(t, cleanedText, `TRiceC("cmd:motor_stop32", stamp32)`)
	assert.NotContains(t, cleanedText, `triceC(iD(`)
}

func TestAbcGenerateWorkflow(t *testing.T) {
	defer Setup(t)()

	til := TriceIDLookUp{
		1001: {Type: "TRiceC", Strg: "cmd:get_power_state"},
		1002: {Type: "trice16C", Strg: "cmd:set_pwm"},
		1003: {Type: "TRice32C", Strg: "cmd:set_time"},
		1004: {Type: "trice32F", Strg: "rpc:legacy_not_abc"},
	}
	require.NoError(t, til.toFile(FSys.Fs, FnJSON))

	var w bytes.Buffer
	require.NoError(t, til.ToFilesAbc(&w, FSys, "abc/deviceA"))

	header, err := FSys.ReadFile(Proj + "abc/deviceA.h")
	require.NoError(t, err)
	headerText := string(header)
	assert.Contains(t, headerText, "#ifndef DEVICEA_H_")
	assert.Contains(t, headerText, `#include "triceRx.h"`)
	assert.Contains(t, headerText, "void get_power_state(const triceRx_t* rx);")
	assert.Contains(t, headerText, "void set_pwm(const triceRx_t* rx);")
	assert.Contains(t, headerText, "void set_time(const triceRx_t* rx);")
	assert.NotContains(t, headerText, "legacy_not_abc")
	assert.NotContains(t, headerText, "__attribute__((weak))")

	editedHeader := `#ifndef DEVICEA_H_
#define DEVICEA_H_

#include "triceRx.h"

void get_power_state(const triceRx_t* rx);
// void set_pwm(const triceRx_t* rx);
void set_time(const triceRx_t* rx);
void unknown_local_handler(const triceRx_t* rx);

#endif /* DEVICEA_H_ */
`
	require.NoError(t, FSys.WriteFile(Proj+"abc/deviceA.h", []byte(editedHeader), 0o644))
	w.Reset()
	require.NoError(t, til.ToFilesAbc(&w, FSys, "abc/deviceA"))

	assert.Contains(t, w.String(), "unknown_local_handler")
	source, err := FSys.ReadFile(Proj + "abc/deviceA.c")
	require.NoError(t, err)
	sourceText := string(source)
	assert.Contains(t, sourceText, `#include "deviceA.h"`)
	assert.Contains(t, sourceText, `#include "triceRx.h"`)
	assert.NotContains(t, sourceText, "static void triceAbcCall_")
	assert.Contains(t, sourceText, "{  1001u,   0u, get_power_state }")
	assert.Contains(t, sourceText, "{  1003u,  32u, set_time }")
	assert.NotContains(t, sourceText, "set_pwm")
	assert.NotContains(t, sourceText, "legacy_not_abc")

	rereadHeader, err := FSys.ReadFile(Proj + "abc/deviceA.h")
	require.NoError(t, err)
	assert.Equal(t, editedHeader, string(rereadHeader))
}

func TestAbcGenerateConflicts(t *testing.T) {
	defer Setup(t)()

	tests := []struct {
		name string
		til  TriceIDLookUp
		want string
	}{
		{
			name: "same command different width",
			til: TriceIDLookUp{
				1001: {Type: "trice16C", Strg: "cmd:set_value"},
				1002: {Type: "trice32C", Strg: "cmd:set_value"},
			},
			want: "conflicting bit widths",
		},
		{
			name: "invalid command name",
			til: TriceIDLookUp{
				1001: {Type: "triceC", Strg: "cmd:1invalid"},
			},
			want: "invalid ABC command name",
		},
	}
	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			var w bytes.Buffer
			err := tc.til.ToFilesAbc(&w, FSys, "deviceB")
			require.Error(t, err)
			assert.True(t, strings.Contains(err.Error(), tc.want), err.Error())
		})
	}
}
