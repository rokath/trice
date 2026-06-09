// SPDX-License-Identifier: MIT

package id

import (
	"fmt"
	"io"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strings"

	"github.com/spf13/afero"
)

// abcTypeMeta keeps the target-code facts encoded in one ABC macro spelling.
type abcTypeMeta struct {
	isABC     bool
	bitWidth  int
	stampBits int
}

// abcCommand is the normalized form used while generating target-specific ABC files.
type abcCommand struct {
	id        TriceID
	typeName  string
	strg      string
	name      string
	bitWidth  int
	stampBits int
}

// abcDeclaration is one handler prototype selected by the target-owned ABC header.
type abcDeclaration struct {
	name string
}

// abcDeclPattern accepts only the public first-version byte-oriented handler signature.
var abcDeclPattern = regexp.MustCompile(`\bvoid\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*const\s+triceAbcRx_t\s*\*\s*(?:[A-Za-z_][A-Za-z0-9_]*)?\s*\)\s*;`)

// abcCommandNamePattern enforces that a command name can become a C function identifier.
var abcCommandNamePattern = regexp.MustCompile(`^[A-Za-z_][A-Za-z0-9_]*$`)

// abcTypeInfo decodes whether a Trice macro type is an ABC form and what it carries.
func abcTypeInfo(typeName string) abcTypeMeta {
	prefixes := map[string]int{
		"trice": 0,
		"Trice": 16,
		"TRice": 32,
		"TRIce": 0,
		"TRICe": 16,
		"TRICE": 32,
	}
	for prefix, stampBits := range prefixes {
		if !strings.HasPrefix(typeName, prefix) {
			continue
		}
		rest := strings.TrimPrefix(typeName, prefix)
		explicitID := false
		if strings.HasSuffix(rest, "_C") {
			explicitID = true
			rest = strings.TrimSuffix(rest, "_C")
		} else if strings.HasSuffix(rest, "C") {
			rest = strings.TrimSuffix(rest, "C")
		} else {
			continue
		}
		if explicitID && (prefix == "trice" || prefix == "Trice" || prefix == "TRice") {
			continue
		}
		if !explicitID && (prefix == "TRIce" || prefix == "TRICe" || prefix == "TRICE") {
			continue
		}
		switch rest {
		case "":
			return abcTypeMeta{isABC: true, bitWidth: 0, stampBits: stampBits}
		case "8":
			return abcTypeMeta{isABC: true, bitWidth: 8, stampBits: stampBits}
		case "16":
			return abcTypeMeta{isABC: true, bitWidth: 16, stampBits: stampBits}
		case "32":
			return abcTypeMeta{isABC: true, bitWidth: 32, stampBits: stampBits}
		case "64":
			return abcTypeMeta{isABC: true, bitWidth: 64, stampBits: stampBits}
		}
	}
	return abcTypeMeta{}
}

// abcCommandName extracts the handler identifier from the format string after the last tag colon.
func abcCommandName(format string) (string, error) {
	name := format
	if index := strings.LastIndex(name, ":"); index >= 0 {
		name = name[index+1:]
	}
	if !abcCommandNamePattern.MatchString(name) {
		return "", fmt.Errorf("invalid ABC command name %q extracted from %q", name, format)
	}
	return name, nil
}

// abcDeclarationText renders the editable target selection prototype for one command.
func abcDeclarationText(cmd abcCommand) string {
	return fmt.Sprintf("void %s(const triceAbcRx_t* rx);", cmd.name)
}

// abcIncludeGuard derives a stable C include guard from the generated header filename.
func abcIncludeGuard(filename string) string {
	var b strings.Builder
	for _, r := range filename {
		switch {
		case 'a' <= r && r <= 'z':
			b.WriteRune(r - 'a' + 'A')
		case 'A' <= r && r <= 'Z':
			b.WriteRune(r)
		case '0' <= r && r <= '9':
			b.WriteRune(r)
		default:
			b.WriteByte('_')
		}
	}
	guard := b.String()
	if guard == "" || ('0' <= guard[0] && guard[0] <= '9') {
		guard = "_" + guard
	}
	if !strings.HasSuffix(guard, "_") {
		guard += "_"
	}
	return guard
}

// abcCommands filters TIL entries to ABC commands and rejects ambiguous handler signatures.
func (ilu TriceIDLookUp) abcCommands() ([]abcCommand, error) {
	ids := make([]int, 0, len(ilu))
	for id := range ilu {
		ids = append(ids, int(id))
	}
	sort.Ints(ids)

	var commands []abcCommand
	byName := map[string]abcCommand{}
	for _, n := range ids {
		id := TriceID(n)
		t := ilu[id]
		info := abcTypeInfo(t.Type)
		if !info.isABC {
			continue
		}
		name, err := abcCommandName(t.Strg)
		if err != nil {
			return nil, fmt.Errorf("ABC ID %d: %w", id, err)
		}
		cmd := abcCommand{id: id, typeName: t.Type, strg: t.Strg, name: name, bitWidth: info.bitWidth, stampBits: info.stampBits}
		if existing, ok := byName[name]; ok && existing.bitWidth != cmd.bitWidth {
			return nil, fmt.Errorf("ABC command %q has conflicting bit widths: ID %d uses %d-bit payload, ID %d uses %d-bit payload", name, existing.id, existing.bitWidth, cmd.id, cmd.bitWidth)
		}
		byName[name] = cmd
		commands = append(commands, cmd)
	}
	return commands, nil
}

// readAbcDeclarations reads the user-editable selection header without executing C preprocessing.
func readAbcDeclarations(fSys *afero.Afero, filename string) (map[string]abcDeclaration, error) {
	in, err := fSys.ReadFile(filename)
	if err != nil {
		return nil, err
	}
	text := stripCComments(string(in))
	declarations := map[string]abcDeclaration{}
	for _, match := range abcDeclPattern.FindAllStringSubmatch(text, -1) {
		declarations[match[1]] = abcDeclaration{name: match[1]}
	}
	return declarations, nil
}

// stripCComments blanks comments so disabled prototypes are not selected accidentally.
func stripCComments(s string) string {
	out := []byte(s)
	inBlock := false
	inLine := false
	inString := false
	inChar := false
	escaped := false
	for i := 0; i < len(s); i++ {
		c := s[i]
		switch {
		case inLine:
			if c == '\n' {
				inLine = false
			} else {
				out[i] = ' '
			}
		case inBlock:
			out[i] = ' '
			if c == '*' && i+1 < len(s) && s[i+1] == '/' {
				out[i+1] = ' '
				inBlock = false
				i++
			}
		case inString:
			if escaped {
				escaped = false
			} else if c == '\\' {
				escaped = true
			} else if c == '"' {
				inString = false
			}
		case inChar:
			if escaped {
				escaped = false
			} else if c == '\\' {
				escaped = true
			} else if c == '\'' {
				inChar = false
			}
		default:
			if c == '/' && i+1 < len(s) {
				switch s[i+1] {
				case '/':
					out[i] = ' '
					out[i+1] = ' '
					inLine = true
					i++
				case '*':
					out[i] = ' '
					out[i+1] = ' '
					inBlock = true
					i++
				}
			} else if c == '"' {
				inString = true
			} else if c == '\'' {
				inChar = true
			}
		}
	}
	return string(out)
}

// ToFilesAbc creates or reads <target>_abc.h and regenerates <target>_abc.c from the TIL.
func (ilu TriceIDLookUp) ToFilesAbc(w io.Writer, fSys *afero.Afero, target string) error {
	if target == "" {
		return fmt.Errorf("missing ABC target name")
	}
	if strings.ContainsAny(target, `/\`) {
		return fmt.Errorf("ABC target %q must be a filename stem, not a path", target)
	}
	commands, err := ilu.abcCommands()
	if err != nil {
		return err
	}
	dir := filepath.Dir(FnJSON)
	if dir == "." {
		dir = ""
	}
	headerName := target + "_abc.h"
	sourceName := target + "_abc.c"
	headerPath := filepath.Join(dir, headerName)
	sourcePath := filepath.Join(dir, sourceName)

	declarations := map[string]abcDeclaration{}
	if fileExists(fSys, headerPath) {
		declarations, err = readAbcDeclarations(fSys, headerPath)
		if err != nil {
			return err
		}
	} else {
		for _, cmd := range commands {
			declarations[cmd.name] = abcDeclaration{name: cmd.name}
		}
		if err := writeAbcHeader(fSys, headerPath, headerName, target, commands); err != nil {
			return err
		}
	}

	selected, err := selectAbcCommands(w, headerPath, commands, declarations)
	if err != nil {
		return err
	}
	return writeAbcSource(fSys, sourcePath, sourceName, headerName, selected)
}

// selectAbcCommands intersects TIL commands with the target header selection.
func selectAbcCommands(w io.Writer, headerPath string, commands []abcCommand, declarations map[string]abcDeclaration) ([]abcCommand, error) {
	byName := map[string]abcCommand{}
	for _, cmd := range commands {
		byName[cmd.name] = cmd
	}
	for _, decl := range declarations {
		if _, ok := byName[decl.name]; !ok {
			fmt.Fprintf(w, "warning: ABC handler %q declared in %s but no matching ABC command exists in %s\n", decl.name, headerPath, FnJSON)
			continue
		}
	}
	var selected []abcCommand
	for _, cmd := range commands {
		if _, ok := declarations[cmd.name]; ok {
			selected = append(selected, cmd)
		}
	}
	return selected, nil
}

// writeAbcHeader creates the first-run target selection header and then leaves it user-owned.
func writeAbcHeader(fSys *afero.Afero, path, filename, target string, commands []abcCommand) error {
	var b strings.Builder
	guard := abcIncludeGuard(filename)
	fmt.Fprintf(&b, "//! \\file %s\n", filename)
	fmt.Fprintf(&b, "//! Trice ABC selection file for target %s.\n", target)
	b.WriteString("//! Generated once; edit this file to select received ABC commands.\n\n")
	fmt.Fprintf(&b, "#ifndef %s\n#define %s\n\n", guard, guard)
	b.WriteString("#include \"triceAbcReceive.h\"\n\n")
	b.WriteString("#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n")
	seen := map[string]bool{}
	for _, cmd := range commands {
		if seen[cmd.name] {
			continue
		}
		b.WriteString(abcDeclarationText(cmd))
		b.WriteByte('\n')
		seen[cmd.name] = true
	}
	b.WriteString("\n#ifdef __cplusplus\n}\n#endif\n\n")
	fmt.Fprintf(&b, "#endif /* %s */\n", guard)
	return fSys.WriteFile(path, []byte(b.String()), 0o644)
}

// writeAbcSource regenerates the read-only wrapper table used by TriceAbcOnReceive.
func writeAbcSource(fSys *afero.Afero, path, filename, headerName string, commands []abcCommand) error {
	var b strings.Builder
	fmt.Fprintf(&b, "//! \\file %s\n", filename)
	b.WriteString("//! Trice generated ABC code - do not edit!\n\n")
	fmt.Fprintf(&b, "#include %q\n", headerName)
	b.WriteString("#include \"triceAbcReceive.h\"\n\n")
	seenWrappers := map[string]bool{}
	for _, cmd := range commands {
		key := fmt.Sprintf("%s/%d", cmd.name, cmd.bitWidth)
		if seenWrappers[key] {
			continue
		}
		fmt.Fprintf(&b, "static void triceAbcCall_%s(const triceAbcRx_t* rx) {\n", cmd.name)
		fmt.Fprintf(&b, "\t%s(rx);\n", cmd.name)
		b.WriteString("}\n\n")
		seenWrappers[key] = true
	}
	b.WriteString("const triceAbc_t triceAbc[] = {\n")
	if len(commands) == 0 {
		b.WriteString("\t{ 0u, 0u, 0 }\n")
	} else {
		b.WriteString("\t/* Trice type */  /* id, bitWidth, function pointer */\n")
		for _, cmd := range commands {
			fmt.Fprintf(&b, "\t/* %-8s */ { %du, %du, triceAbcCall_%s },\n", cmd.typeName, cmd.id, cmd.bitWidth, cmd.name)
		}
	}
	b.WriteString("};\n\n")
	if len(commands) == 0 {
		b.WriteString("const unsigned triceAbcElements = 0u;\n")
	} else {
		b.WriteString("const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc[0]);\n")
	}

	if info, err := fSys.Stat(path); err == nil && info.Mode().Perm()&0o222 == 0 {
		_ = fSys.Chmod(path, info.Mode().Perm()|0o200)
	}
	if err := fSys.WriteFile(path, []byte(b.String()), 0o644); err != nil {
		return err
	}
	_ = fSys.Chmod(path, os.FileMode(0o444))
	return nil
}
