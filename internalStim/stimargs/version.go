// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net

package stimargs

import (
	"fmt"
	"io"
)

// scVersion is sub-command 'version'. It prints version information.
func scVersion(w io.Writer) error {
	distributeArgs(w)
	if verbose {
		fmt.Fprintln(w, "https://github.com/rokath/trice/cmd/stim")
	}
	if Version != "" {
		fmt.Fprintf(w, "version=%v, commit=%v, built at %v\n", Version, Commit, Date)
	} else {
		fmt.Fprintf(w, "version=devel, built %s\n", Date)
	}
	return nil
}
