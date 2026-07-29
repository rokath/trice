// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"strings"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
)

//go:generate go run ../../cmd/generate-helpall-doc

// RenderHelpText returns the help output for the passed help command arguments.
func RenderHelpText(helpArgs ...string) (string, error) {
	resetHelpGenerationDefaults()
	FlagsInit()

	var out bytes.Buffer
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	args := append([]string{"trice", "help"}, helpArgs...)
	err := Handler(&out, fSys, args)
	return normalizeGeneratedHelpWhitespace(out.String()), err
}

// normalizeGeneratedHelpWhitespace keeps new generated help lines free of
// space-before-tab whitespace without reformatting the historical help file.
func normalizeGeneratedHelpWhitespace(s string) string {
	replacements := map[string]string{
		"    \tCreate or use [path/]<target>.h and regenerate [path/]<target>.c for Trice ABC receive handling.":                                                "        Create or use [path/]<target>.h and regenerate [path/]<target>.c for Trice ABC receive handling.",
		"    \tCreate a deprecated legacy til_rpc.c file. Use -abc=<target> for new command-style communication.":                                               "        Create a deprecated legacy til_rpc.c file. Use -abc=<target> for new command-style communication.",
		"    \tCreate a deprecated legacy til_rpc.h file. Use -abc=<target> for new command-style communication.":                                               "        Create a deprecated legacy til_rpc.h file. Use -abc=<target> for new command-style communication.",
		"    \tMaximum number of parent directories displayed before the source filename. The filename is always shown; leading \"..\" components are omitted.": "        Maximum number of parent directories displayed before the source filename. The filename is always shown; leading \"..\" components are omitted.",
		"    \tBase directory for File paths stored in li.json. Each File path is stored relative to this directory. A relative -liRoot value is resolved from the current working directory. Examples: when running in project/build, \"-liRoot ..\" stores ../src/main.c as src/main.c; without -liRoot, the directory containing li.json is used as the base.": "        Base directory for File paths stored in li.json. Each File path is stored relative to this directory. A relative -liRoot value is resolved from the current working directory. Examples: when running in project/build, \"-liRoot ..\" stores ../src/main.c as src/main.c; without -liRoot, the directory containing li.json is used as the base.",
	}
	for old, newText := range replacements {
		s = strings.ReplaceAll(s, old, newText)
	}
	return s
}

func resetHelpGenerationDefaults() {
	allHelp = false
	displayServerHelp = false
	helpHelp = false
	logHelp = false
	addHelp = false
	generateHelp = false
	scanHelp = false
	shutdownHelp = false
	insertIDsHelp = false
	versionHelp = false
	cleanIDsHelp = false
	Verbose = false
	LogfileName = "off"

	id.FnJSON = "til.json"
	id.LIFnJSON = "li.json"
	id.LIRoot = ""
	id.LIMaxDirs = 0
	id.Min = id.TriceID(1000)
	id.Max = id.TriceID(7999)
	id.SearchMethod = "random"
	id.Srcs = nil
	id.ExcludeSrcs = nil
	id.IDRange = nil
	id.TriceAliases = nil
	id.TriceSAliases = nil
	id.DryRun = false
	id.DefaultStampSize = 32
	id.TriceCacheEnabled = false
	id.SpaceInsideParenthesis = false
	id.SkipAdditionalChecks = false

	emitter.Ban = nil
	emitter.Pick = nil
	emitter.UserLabel = nil
}
