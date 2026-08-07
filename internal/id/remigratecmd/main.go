// SPDX-License-Identifier: MIT

// Command remigratecmd provides the repository test workflow with a safe
// Bind-to-Clean transition without adding a public trice CLI operation.
package main

import (
	"flag"
	"fmt"
	"os"

	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
)

// main parses only the source-ownership options needed for re-migration and
// delegates all scanning, validation, and transactional changes to internal/id.
func main() {
	var (
		sources  id.ArrayFlag
		excludes id.ArrayFlag
	)
	flag.Var(&sources, "src", "Source file or directory to remigrate; repeatable.")
	flag.Var(&excludes, "exclude", "Source file or directory to exclude; repeatable.")
	flag.StringVar(&id.BindDir, "bindDir", "./build/triceIDs", "Directory containing generated Trice bind sidecars.")
	flag.BoolVar(&id.Verbose, "v", false, "Print each planned re-migration.")
	flag.BoolVar(&id.DryRun, "dry-run", false, "Validate and report without changing files.")
	flag.Parse()
	if len(sources) == 0 {
		sources = id.ArrayFlag{"."}
	}
	id.Srcs = sources
	id.ExcludeSrcs = excludes
	id.CompactSrcs()

	fileSystem := &afero.Afero{Fs: afero.NewOsFs()}
	if err := id.SubCmdIdRemigrateBindToClean(os.Stdout, fileSystem); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}
