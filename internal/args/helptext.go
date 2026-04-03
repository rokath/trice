// SPDX-License-Identifier: MIT

package args

import (
	"bytes"

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
	return out.String(), err
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
	id.LIPathKind = "base"
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
