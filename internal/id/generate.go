// SPDX-License-Identifier: MIT

// Package id List is responsible for id List managing
package id

// List management

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"path/filepath"
	"sort"
	"strconv"
	"strings"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

var (
	GenerateLogC bool // GenerateLogC selects the current target-side log table generator.
	// GenerateLogCPath optionally overrides the conventional til.c output path.
	GenerateLogCPath string
	// GenerateOneLineJSON selects the readable, derived TIL and LI JSON views.
	GenerateOneLineJSON bool
	// GenerateBindReport selects an inspection-only inventory of existing Bind artifacts.
	GenerateBindReport bool
	GenerateABC        string
	WriteAllColors     bool
)

// OptionalFilenameFlag allows a flag to be used either as a boolean switch or
// with an optional output path, for example -logC, -logC=out/til, or -logC=out/til.c.
type OptionalFilenameFlag struct {
	Enabled *bool
	Path    *string
}

func (f OptionalFilenameFlag) String() string {
	if f.Path == nil {
		return ""
	}
	return *f.Path
}

func (f OptionalFilenameFlag) Set(value string) error {
	switch value {
	case "true":
		if f.Enabled != nil {
			*f.Enabled = true
		}
		if f.Path != nil {
			*f.Path = ""
		}
	case "false":
		if f.Enabled != nil {
			*f.Enabled = false
		}
		if f.Path != nil {
			*f.Path = ""
		}
	default:
		if f.Enabled != nil {
			*f.Enabled = true
		}
		if f.Path != nil {
			*f.Path = value
		}
	}
	return nil
}

func (f OptionalFilenameFlag) IsBoolFlag() bool {
	return true
}

// LogCOutputPath returns the explicit C filename or til.c in the generated directory.
// Explicit paths remain relative to the caller's working directory.
func LogCOutputPath(target string) string {
	if target == "" {
		return filepath.Join(BindDir, "til.c")
	}
	if strings.EqualFold(filepath.Ext(target), ".c") {
		return target
	}
	return target + ".c"
}

// SubCmdIdGenerate performs sub-command generate, creating support files/output.
func SubCmdGenerate(w io.Writer, fSys *afero.Afero) (err error) {
	if GenerateBindReport {
		if GenerateLogC || GenerateOneLineJSON || GenerateABC != "" || WriteAllColors {
			return errors.New("trice generate: -bindReport cannot be combined with -logC, -onelineJSON, -abc or -colors")
		}
		return generateBindReport(w, fSys)
	}
	if !GenerateLogC && !GenerateOneLineJSON && GenerateABC == "" && !WriteAllColors {
		fmt.Fprintln(w, `The "trice generate" command needs at least one parameter. Check "trice help -generate".`)
		return nil
	}
	if GenerateLogC && GenerateABC != "" {
		return errors.New("trice generate: -logC and -abc are alternative generators and cannot be used together")
	}
	if GenerateOneLineJSON && (GenerateLogC || GenerateABC != "") {
		return errors.New("trice generate: -onelineJSON cannot be combined with -logC or -abc")
	}
	if (GenerateLogC || GenerateOneLineJSON || GenerateABC != "") && strings.TrimSpace(BindDir) == "" {
		return errors.New("trice generate: -genDir must not be empty")
	}

	if WriteAllColors {
		emitter.ShowAllColors()
		if Verbose {
			fmt.Fprintln(w, `Modify ansi.ColorFunc assignments in lineTransformerANSI.go to change Trice colors.`)
		}
	}

	ilu := make(TriceIDLookUp)
	if GenerateLogC || GenerateOneLineJSON || GenerateABC != "" {
		content, readErr := fSys.ReadFile(FnJSON)
		if readErr != nil {
			return fmt.Errorf("trice generate: cannot read TIL %s: %w", FnJSON, readErr)
		}
		parseErr := ilu.FromJSON(content)
		if GenerateOneLineJSON && (len(bytes.TrimSpace(content)) == 0 || bytes.Equal(bytes.TrimSpace(content), []byte("null"))) && parseErr == nil {
			parseErr = errors.New("expected a JSON object")
		}
		if parseErr != nil {
			return fmt.Errorf("trice generate: cannot parse TIL %s: %w", FnJSON, parseErr)
		}
		if Verbose {
			fmt.Fprintln(w, "Read ID List file", FnJSON, "with", len(ilu), "items.")
		}
	}

	if GenerateLogC {
		current, selectErr := selectCurrentLogEntries(w, fSys, ilu)
		if selectErr != nil {
			return selectErr
		}
		fnC := LogCOutputPath(GenerateLogCPath)
		generated, renderErr := current.toListTilC(fnC)
		if renderErr != nil {
			return fmt.Errorf("trice generate: cannot render %s: %w", fnC, renderErr)
		}
		unchanged, compareErr := bindFileHasContent(fSys, fnC, generated)
		if compareErr != nil {
			return fmt.Errorf("trice generate: %w", compareErr)
		}
		if unchanged {
			if Verbose {
				fmt.Fprintln(w, "unchanged", fnC)
			}
			return nil
		}
		if dir := filepath.Dir(fnC); dir != "." && dir != "" {
			if mkdirErr := fSys.MkdirAll(dir, 0o755); mkdirErr != nil {
				return fmt.Errorf("trice generate: cannot create output directory %s: %w", dir, mkdirErr)
			}
		}
		if writeErr := atomicWriteFile(fSys, fnC, generated, fileWritePerm(fSys, fnC, 0o644)); writeErr != nil {
			return fmt.Errorf("trice generate: cannot write %s: %w", fnC, writeErr)
		}
		if Verbose {
			fmt.Fprintln(w, "generated", fnC)
		}
	}

	if GenerateABC != "" {
		baseDir := filepath.Dir(FnJSON)
		if filepath.Base(GenerateABC) == GenerateABC {
			baseDir = BindDir
		}
		msg.FatalOnErr(ilu.toFilesAbcAt(w, fSys, GenerateABC, baseDir))
		if Verbose {
			_, _, _, _, headerPath, sourcePath, err := abcTargetPaths(baseDir, GenerateABC)
			msg.FatalOnErr(err)
			fmt.Fprintln(w, "generated", headerPath)
			fmt.Fprintln(w, "generated", sourcePath)
		}
	}
	if GenerateOneLineJSON {
		if err := generateOneLineJSONFiles(w, fSys, ilu); err != nil {
			return err
		}
	}

	return nil
}

// oneLineLocation is an output-only view; it places the line number before the
// filename without changing the persisted TriceLI schema or its normal writer.
type oneLineLocation struct {
	Line int    `json:"Line"`
	File string `json:"File"`
}

// oneLineJSONPath places a derived JSON view in the shared generated directory.
func oneLineJSONPath(source string) string {
	name := filepath.Base(source)
	extension := filepath.Ext(name)
	if strings.EqualFold(extension, ".json") {
		return filepath.Join(BindDir, strings.TrimSuffix(name, extension)+".oneline.json")
	}
	return filepath.Join(BindDir, name+".oneline.json")
}

// renderOneLineJSON keeps the outer lookup object valid JSON while placing one
// standard-library-encoded key and value on each physical line. Keys use the
// same lexical order as encoding/json's map encoder.
func renderOneLineJSON(entries map[TriceID]any, escapeHTML bool) ([]byte, error) {
	keys := make([]string, 0, len(entries))
	ids := make(map[string]TriceID, len(entries))
	for id := range entries {
		key := strconv.Itoa(int(id))
		keys = append(keys, key)
		ids[key] = id
	}
	sort.Strings(keys)
	if len(keys) == 0 {
		return []byte("{}"), nil
	}
	var output bytes.Buffer
	output.WriteString("{\n")
	for index, key := range keys {
		encodedKey, err := json.Marshal(key)
		if err != nil {
			return nil, err
		}
		var value bytes.Buffer
		encoder := json.NewEncoder(&value)
		encoder.SetEscapeHTML(escapeHTML)
		if err := encoder.Encode(entries[ids[key]]); err != nil {
			return nil, fmt.Errorf("cannot encode ID %s: %w", key, err)
		}
		output.WriteByte('\t')
		output.Write(encodedKey)
		output.WriteString(": ")
		output.Write(bytes.TrimSuffix(value.Bytes(), []byte("\n")))
		if index < len(keys)-1 {
			output.WriteByte(',')
		}
		output.WriteByte('\n')
	}
	output.WriteByte('}')
	return output.Bytes(), nil
}

// generateOneLineJSONFiles validates and renders both requested views before
// publishing either. The original dictionaries remain authoritative and intact.
func generateOneLineJSONFiles(w io.Writer, fSys *afero.Afero, til TriceIDLookUp) error {
	includeLI := LIFnJSON != "off" && LIFnJSON != "none"
	tilPath := oneLineJSONPath(FnJSON)
	if filepath.Clean(tilPath) == filepath.Clean(FnJSON) {
		return errors.New("trice generate: one-line output path conflicts with an input or another output")
	}
	if includeLI {
		liPath := oneLineJSONPath(LIFnJSON)
		if filepath.Clean(tilPath) == filepath.Clean(LIFnJSON) || filepath.Clean(liPath) == filepath.Clean(FnJSON) || filepath.Clean(liPath) == filepath.Clean(LIFnJSON) || filepath.Clean(tilPath) == filepath.Clean(liPath) {
			return errors.New("trice generate: one-line output path conflicts with an input or another output")
		}
	}
	tilEntries := make(map[TriceID]any, len(til))
	for id, value := range til {
		tilEntries[id] = value
	}
	tilOutput, err := renderOneLineJSON(tilEntries, false)
	if err != nil {
		return fmt.Errorf("trice generate: cannot render TIL %s: %w", FnJSON, err)
	}
	outputs := []bindWrite{{path: tilPath, data: tilOutput, perm: fileWritePerm(fSys, tilPath, 0o666), kind: "til"}}
	if includeLI {
		content, err := fSys.ReadFile(LIFnJSON)
		if err != nil {
			return fmt.Errorf("trice generate: cannot read LI %s: %w", LIFnJSON, err)
		}
		locations := make(TriceIDLookUpLI)
		if err := json.Unmarshal(content, &locations); err != nil {
			return fmt.Errorf("trice generate: cannot parse LI %s: %w", LIFnJSON, err)
		}
		if bytes.Equal(bytes.TrimSpace(content), []byte("null")) {
			return fmt.Errorf("trice generate: cannot parse LI %s: expected a JSON object", LIFnJSON)
		}
		liEntries := make(map[TriceID]any, len(locations))
		for id, location := range locations {
			liEntries[id] = oneLineLocation{Line: location.Line, File: location.File}
		}
		liPath := oneLineJSONPath(LIFnJSON)
		liOutput, err := renderOneLineJSON(liEntries, true)
		if err != nil {
			return fmt.Errorf("trice generate: cannot render LI %s: %w", LIFnJSON, err)
		}
		outputs = append(outputs, bindWrite{path: liPath, data: liOutput, perm: fileWritePerm(fSys, liPath, 0o666), kind: "li"})
	}
	var changes []bindWrite
	for _, output := range outputs {
		unchanged, err := bindFileHasContent(fSys, output.path, output.data)
		if err != nil {
			return fmt.Errorf("trice generate: cannot inspect %s: %w", output.path, err)
		}
		if unchanged {
			if Verbose {
				fmt.Fprintln(w, "unchanged", output.path)
			}
			continue
		}
		changes = append(changes, output)
	}
	if err := commitBindWrites(fSys, changes); err != nil {
		return fmt.Errorf("trice generate: %w", err)
	}
	if Verbose {
		for _, output := range changes {
			fmt.Fprintln(w, "generated", output.path)
		}
	}
	return nil
}

// ToFileTilCSharp generates C# helpers for a third party tool.
func (ilu TriceIDLookUp) ToFileTilCSharp(fSys afero.Fs, fn string) (err error) {
	fh, err := fSys.Create(fn)
	msg.FatalOnErr(err)
	defer func() {
		err = fh.Close()
		msg.FatalOnErr(err)
	}()

	cs, e := ilu.toListTilCS(fn)
	_, err = fh.Write(cs)
	msg.FatalOnErr(e)
	return
}

// toListTilCS converts ilu into CS-source byte slice in human-readable form.
func (ilu TriceIDLookUp) toListTilCS(filename string) ([]byte, error) {
	text := []byte(`//! \file ` + filename + ` 

// Trice generated code - do not edit!

// There is still a need to exchange the format specifier from C to C# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// See https://stackoverflow.com/questions/33432341/how-to-use-c-language-format-specifiers-in-c-sharp
// and https://www.codeproject.com/Articles/19274/A-printf-implementation-in-C for possible help.

namespace TriceIDList;

	public class TilItem
	{
		public TilItem(int bitWidth, int paramCount, string strg)
		{
			BitWidth = bitWidth;
			ParamCount = paramCount;
			Strg = strg;
		}

		public int BitWidth { get; init; }
		public int ParamCount { get; init; }
		public string Strg { get; init; }
	}

	//! Til contains all trice format strings together with id and parameter information.
	//!
	//! The bitWidth value is not transmitted in the binary data stream and needed for its decoding.
	//! The paramCount is de-facto not needed. It is derivable from the received data, see docs/TriceReferenceManual.md#binary-encoding.
	//! It is recommended to check if both values are matching. A negative paramCount indicates, that its value is unknown at compile time.
	public static class Til
	{
		public static readonly Dictionary<int, TilItem> TilList= new Dictionary<int, TilItem>
		{ /* triceType ( extended ) */ //   id,     TilItem( bitWidth, paramCount, Strg )
`)

	defaultBitWidth, err := strconv.Atoi(DefaultTriceBitWidth)
	msg.FatalOnErr(err)

	for id, t := range ilu {
		extType, bitWidth, paramCount := computeValues(t, defaultBitWidth)
		text = append(text, []byte(fmt.Sprintf(`		/* %10s ( %10s )*/ { %5d, new TilItem( %2d, %2d, "%s" ) },`+"\n", t.Type, extType, id, bitWidth, paramCount, t.Strg))...)
	}

	tail := []byte(`    };
}

`)
	text = append(text, tail...)
	return text, nil
}

func computeValues(t TriceFmt, defaultBitWidth int) (extType string, bitWidth, paramCount int) {
	DefaultTriceBitWidth = strconv.Itoa(defaultBitWidth)
	if info := abcTypeInfo(t.Type); info.isABC {
		return t.Type, info.bitWidth, -1
	}
	switch t.Type[len(t.Type)-1:] {
	case "B", "F", "N", "S", "C":
		paramCount = -1
		extType = t.Type
	default:
		paramCount = formatSpecifierCount(t.Strg)
		extType, _ = ConstructFullTriceInfo(t.Type, paramCount)
	}
	bitWidth = defaultBitWidth
	for i, w := range []string{"64", "32", "16", "8"} {
		found := strings.Contains(extType, w)
		if found {
			bitWidth = 64 >> i
			break
		}
	}
	return
}

// ConstructFullTriceInfo returns full TRICE info, if not exist in orig‚Type string
// For examples see TestConstructFullTriceInfo
// see also AddFmtCount
func ConstructFullTriceInfo(origType string, paramCount int) (fullTriceType string, err error) {
	if strings.ToUpper(origType[:5]) != "TRICE" {
		err = fmt.Errorf(origType, "starts not with trice")
		return
	}
	if len(origType) == 5 { // most often expected case: trice, Trice, TRice or TRICE
		if paramCount == 0 {
			fullTriceType = fmt.Sprintf(origType+"_%d", paramCount)
		} else {
			fullTriceType = fmt.Sprintf(origType+DefaultTriceBitWidth+"_%d", paramCount)
		}
		return
	}

	before, after, found := strings.Cut(origType, "_")
	if found {
		switch after {
		case "N", "S": // TRICE_S
			if paramCount == 1 {
				fullTriceType = before + after
			} else {
				err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
			}
		case "B": // TRICE_B or TRICE8_B
			if paramCount == 1 {
				if len(before) == 5 { // no bitwidth
					fullTriceType = before + DefaultTriceBitWidth + after
				} else {
					fullTriceType = before + after
				}
			} else {
				err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
			}
		case "F": // TRICE16_F or TRICE_F
			if paramCount == 0 {
				if len(before) == 5 { // no bitwidth
					fullTriceType = before + DefaultTriceBitWidth + after
				} else {
					fullTriceType = before + after
				}
			} else {
				err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
			}
		case "C": // TRICE_C has no payload; TRICE8_C and wider variants carry counted ABC payload bytes.
			if paramCount == 0 {
				if len(before) == 5 { // no bitwidth
					fullTriceType = before + after
				} else {
					fullTriceType = before + after
				}
			} else {
				err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
			}
		default:
			cnt, e := strconv.Atoi(after)
			if e != nil {
				err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
			}
			if cnt != paramCount {
				err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
			}
			if cnt == 0 {
				fullTriceType = before[:5] + "_0" // no bitwidth
			} else {
				if len(before) == 5 { // trice_7 cases
					fullTriceType = before + DefaultTriceBitWidth + "_" + after
				} else {
					fullTriceType = before + "_" + after
				}
			}
		}
		return
	}
	switch before[5:] {
	case "0": // TRICE0
		if paramCount != 0 {
			err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
		}
		fullTriceType = before[:len(before)-1] + "_0"
	case "S", "N": // triceS
		if paramCount == 1 {
			fullTriceType = before
		} else {
			err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
		}
	case "B", "8B", "16B", "32B", "64B": // TRICE16B or triceF or Trice8B
		if paramCount == 1 {
			if len(before) == 6 { // no bit width given
				fullTriceType = before[:len(before)-1] + DefaultTriceBitWidth + before[len(before)-1:]
			} else {
				fullTriceType = before
			}
		} else {
			err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
		}
	case "F", "8F", "16F", "32F", "64F": // TRICE16B or triceF or Trice8B
		if paramCount == 0 {
			if len(before) == 6 { // no bit width given
				fullTriceType = before[:len(before)-1] + DefaultTriceBitWidth + before[len(before)-1:]
			} else {
				fullTriceType = before
			}
		} else {
			err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
		}
	case "C": // triceC carries no payload and uses ABC command display.
		if paramCount == 0 {
			fullTriceType = before
		} else {
			err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
		}
	case "8C", "16C", "32C", "64C": // trice8C and wider ABC forms carry counted payload bytes.
		if paramCount == 0 {
			fullTriceType = before
		} else {
			err = fmt.Errorf(origType, "has invalid parameter count", paramCount)
		}
	case "8", "16", "32", "64":
		if paramCount == 0 {
			fullTriceType = fmt.Sprintf(before[:5]+"_%d", paramCount) // discard bitwidth
		} else {
			fullTriceType = fmt.Sprintf(before+"_%d", paramCount)
		}
	default:
	}
	return
}
