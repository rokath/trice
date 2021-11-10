// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"fmt"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

func TestHelpHelp(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "help", "-help"})
		if nil != err {
			fmt.Print(err)
		}
	}
	h.Lock()
	m.Lock()
	act := tst.CaptureStdOut(fn)
	helpHelp = false
	m.Unlock()
	h.Unlock()
	fmt.Print(act)
	exp := `syntax: 'trice sub-command' [params]
      sub-command 'h|help': For command line usage.
              "trice h" will print this help text as a whole.
        -all
              Show all help.
        -displayserver
              Show ds|displayserver specific help.
        -ds
              Show ds|displayserver specific help.
        -h    Show h|help specific help.
        -help
              Show h|help specific help.
        -l    Show l|log specific help.
        -log
              Show l|log specific help.
        -logfile string
              Append all output to logfile. Options are: 'off|none|filename|auto':
              "off": no logfile (same as "none")
              "none": no logfile (same as "off")
              "auto": Use as logfile name "2006-01-02_1504-05_trice.log" with actual time.
              "filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
              All trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.
              Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
               (default "off")
        -r    Show r|refresh specific help.
        -refresh
              Show r|refresh specific help.
        -renew
              Show renew specific help.
        -s    Show s|scan specific help.
        -scan
              Show s|scan specific help.
        -sd
              Show sd|shutdown specific help.
        -shutdown
              Show sd|shutdown specific help.
        -u    Show u|update specific help.
        -update
              Show u|update specific help.
        -v    short for verbose
        -ver
              Show ver|version specific help.
        -verbose
              Gives more informal output if used. Can be helpful during setup.
              For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
        -version
              Show ver|version specific help.
        -z    Show zeroSourceTreeIds specific help.
        -zeroSourceTreeIds
              Show zeroSourceTreeIds specific help.
      example 'trice h': Print short help.
      example 'trice h -all': Print all help.
      example 'trice h -log': Print log help.
      `
	tst.EqualLines(t, exp, act)
}

func TestHelpDisplayServer(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "help", "-ds"})
		if nil != err {
			fmt.Print(err)
		}
	}
	h.Lock()
	m.Lock()
	act := tst.CaptureStdOut(fn)
	helpHelp = false
	m.Unlock()
	h.Unlock()
	fmt.Print(act)
	exp := `syntax: 'trice sub-command' [params]
      sub-command 'ds|displayServer': Starts a display server.
              Use in a separate console. On Windows use wt (https://github.com/microsoft/terminal) or a linux shell like git-bash to avoid ANSI color issues.
              Running "trice ds" inside a console opens a display server to be used for displaying the TRICE logs remotely.
              Several instances of 'trice l -ds -port ...' (for different ports) will send output there in parallel.
        -color string
              The format strings can start with a lower or upper case channel information.
              See https://github.com/rokath/trice/blob/master/pkg/src/triceCheck.c for examples. Color options:
              "off": Disable ANSI color. The lower case channel information is kept: "w:x"-> "w:x"
              "none": Disable ANSI color. The lower case channel information is removed: "w:x"-> "x"
              "default|color": Use ANSI color codes for known upper and lower case channel info are inserted and lower case channel information is removed.
               (default "default")
        -ipa string
              IP address like '127.0.0.1'.
              You can specify this switch if you intend to use the remote display option to show the output on a different PC in the network.
               (default "localhost")
        -ipp string
              16 bit IP port number.
              You can specify this switch if you want to change the used port number for the remote display functionality.
               (default "61497")
        -logfile string
              Append all output to logfile. Options are: 'off|none|filename|auto':
              "off": no logfile (same as "none")
              "none": no logfile (same as "off")
              "auto": Use as logfile name "2006-01-02_1504-05_trice.log" with actual time.
              "filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
              All trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.
              Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
               (default "off")
      example: 'trice ds': Start display server.      
      `
	tst.EqualLines(t, exp, act)
}

func TestHelpRenew(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "help", "-renew"})
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
      sub-command 'renew': It is like refresh, but til.json is cleared first, so all 'old' trices are removed. Use with care.
        -dry-run
              No changes applied but output shows what would happen.
              "trice renew -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
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
              Example: "trice renew -dry-run -v -src ./test/ -src pkg/src/trice.h" will scan all C|C++ header and
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
      example: 'trice renew': Rebuild ID list from source tree, discard old IDs.           
      `
	tst.EqualLines(t, exp, act)
}

func TestHelpShutdown(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "help", "-sd"})
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
      sub-command 'sd|shutdown': Ends display server at IPA:IPP, works also on a remote machine.
        -ipa string
              IP address like '127.0.0.1'.
              You can specify this switch if you intend to use the remote display option to show the output on a different PC in the network.
               (default "localhost")
        -ipp string
              16 bit IP port number.
              You can specify this switch if you want to change the used port number for the remote display functionality.
               (default "61497")
      example: 'trice sd': Shut down remote display server.          
      `
	tst.EqualLines(t, exp, act)
}

func TestHelpUpdate(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "help", "-u"})
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
      sub-command 'u|update': For updating ID list and source files.
              "trice update" will parse source tree(s) for new or changed TRICE macros, modify them appropriate and update/generate the JSON list.
              The "update" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
        -IDMax value
              Upper end of ID range for normal trices. (default 65535)
        -IDMaxShort value
              Upper end of ID range for short trices. (default 32767)
        -IDMethod string
              Search method for new ID's in range- Options are 'upward', 'downward' & 'random'. (default "random")
        -IDMin value
              Lower end of ID range for normal trices. (default 32768)
        -IDMinShort value
              Lower end of ID range for short trices. (default 1)
        -addParamCount
              Extend TRICE macro names with the parameter count _n to enable compile time checks.
        -dry-run
              No changes applied but output shows what would happen.
              "trice update -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
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
        -sharedIDs
              New ID policy:
              true: TriceFmt's without TriceID get equal TriceID if an equal TriceFmt exists already.
              false: TriceFmt's without TriceID get a different TriceID if an equal TriceFmt exists already.
        -src value
              Source dir or file, It has one parameter. Not usable in the form "-src *.c".
              This is a multi-flag switch. It can be used several times for directories and also for files.
              Example: "trice update -dry-run -v -src ./test/ -src pkg/src/trice.h" will scan all C|C++ header and
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
      example: 'trice update -src ../A -src ../../B': Parse ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json           
      `
	tst.EqualLines(t, exp, act)
}

func TestHelpVersion(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "help", "-ver"})
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
      sub-command 'ver|version': For displaying version information.
              "trice v" will print the version information. If trice is not versioned the build time will be displayed instead.
        -logfile string
              Append all output to logfile. Options are: 'off|none|filename|auto':
              "off": no logfile (same as "none")
              "none": no logfile (same as "off")
              "auto": Use as logfile name "2006-01-02_1504-05_trice.log" with actual time.
              "filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
              All trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.
              Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
               (default "off")
        -v    short for verbose
        -verbose
              Gives more informal output if used. Can be helpful during setup.
              For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.           
      `
	tst.EqualLines(t, exp, act)
}
