// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"fmt"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

func TestHelpRefresh(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "help", "-r"})
		if nil != err {
			fmt.Print(err)
		}
	}
	h.Lock()
	m.Lock()
	act := tst.CaptureStdOut(fn)
	allHelp = false
	m.Unlock()
	h.Unlock()
	fmt.Print(act)
	exp := `syntax: 'trice sub-command' [params]
      sub-command 'r|refresh': For updating ID list from source files but does not change the source files.
              "trice refresh" will parse source tree(s) for TRICE macros, and refresh/generate the JSON list.
              This command should be run on adding souce files to the project before the first time "trice update" is called.
              If the new source files contain TRICE macros with IDs these are added to til.json if not already used.
              Already used IDs are reported, so you have the chance to remnove them from til.son and then do "trice u" again.
              This way you can make sure to get the new sources unchanged in your list. (to do: -force switch)
              Already used IDs are replaced by new IDs during the next "trice update", so the old IDs in the list will survive.
              If you do not refresh the list after adding source files and perform an "trice update" new generated IDs could be equal to
              IDs used in the added sources with the result that IDs in the added sources could get changed what you may not want.
              Using "trice u -IDMethod random" (default) makes the chance for such conflicts very low.
              The "refresh" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
        -dry-run
              No changes applied but output shows what would happen.
              "trice refresh -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
        -i string
              Short for '-idlist'.
               (default "til.json")
        -idList string
              Alternate for '-idlist'.
               (default "til.json")
        -idlist string
              The trice ID list file.
              The specified JSON file is needed to display the ID coded trices during runtime and should be under version control.
               (default "til.json")
        -s value
              Short for src.
        -src value
              Source dir or file, It has one parameter. Not usable in the form "-src *.c".
              This is a multi-flag switch. It can be used several times for directories and also for files.
              Example: "trice refresh -dry-run -v -src ./test/ -src pkg/src/trice.h" will scan all C|C++ header and
              source code files inside directory ./test and scan also file trice.h inside pkg/src directory.
              Without the "-dry-run" switch it would create|extend a list file til.json in the current directory.
               (default "./")
        -til string
              Short for '-idlist'.
               (default "til.json")
        -v    short for verbose
        -verbose
              Gives more informal output if used. Can be helpful during setup.
              For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
      example: 'trice refresh': Update ID list from source tree.      
      `
	tst.EqualLines(t, exp, act)
}
