// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"bytes"
	"fmt"
	"sync"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

var x *sync.RWMutex

func init() {
	x = new(sync.RWMutex)
}

func execHelper(t *testing.T, input []string, expect string) {
	x.Lock()
	defer x.Unlock()
	var out bytes.Buffer
	FlagsInit() // maybe needed for clearance of previous tests (global vars)
	err := Handler(&out, input)
	if err != nil {
		fmt.Fprint(&out, err)
	}
	act := out.String()
	tst.EqualLines(t, expect, act)
}

func TestHelpRenew(t *testing.T) {
	args := []string{"trice", "help", "-renew"}
	expect := `syntax: 'trice sub-command' [params]
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
	execHelper(t, args, expect)
}

func TestHelpShutdown(t *testing.T) {
	args := []string{"trice", "help", "-sd"}
	expect := `syntax: 'trice sub-command' [params]
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
	execHelper(t, args, expect)
}

func TestHelpUpdate(t *testing.T) {
	args := []string{"trice", "help", "-update"}
	expect := `syntax: 'trice sub-command' [params]
      sub-command 'u|update': For updating ID list and source files.
              "trice update" will parse source tree(s) for new or changed TRICE macros, modify them appropriate and update/generate the JSON list.
              The "update" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
        -IDMax value
              Upper end of ID range for normal trices. (default 65535)
        -IDMethod string
              Search method for new ID's in range- Options are 'upward', 'downward' & 'random'. (default "random")
        -IDMin value
              Lower end of ID range for normal trices. (default 32768)
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
              ID policy:
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
	execHelper(t, args, expect)
}

func TestHelpVersion(t *testing.T) {
	args := []string{"trice", "help", "-version"}
	expect := `syntax: 'trice sub-command' [params]
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
	execHelper(t, args, expect)
}

func TestHelpDisplayServer(t *testing.T) {
	args := []string{"trice", "help", "-ds"}
	expect := `syntax: 'trice sub-command' [params]
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
	execHelper(t, args, expect)
}

func TestHelpScan(t *testing.T) {
	args := []string{"trice", "help", "-scan"}
	expect := `syntax: 'trice sub-command' [params]
      sub-command 's|scan': Shows available serial ports)
      example: 'trice s': Show COM ports.
      `
	execHelper(t, args, expect)
}

func TestHelpZero(t *testing.T) {
	args := []string{"trice", "help", "-z"}
	expect := `syntax: 'trice sub-command' [params]
      sub-command 'zeroSourceTreeIds': Set all Id(n) inside source tree dir to Id(0).
                  Avoid using this sub-command normally. The switch "-src" is mandatory and no multi-flag here.
                  This sub-command is mainly for testing. For several source directories you need several runs.
        -dry-run
                  No changes applied but output shows what would happen.
                  "trice zeroSourceTreeIds -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
                  This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
        -src string
                  Zero all Id(n) inside source tree dir, required.
      example: 'trice zeroSourceTreeIds -src ../A': Sets all TRICE IDs to 0 in ../A. Use with care!
      `
	execHelper(t, args, expect)
}

func TestHelpHelp(t *testing.T) {
	args := []string{"trice", "help", "-help"}
	expect := `syntax: 'trice sub-command' [params]
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
	execHelper(t, args, expect)
}
func TestHelpSdV(t *testing.T) {
	args := []string{"trice", "help", "-sd", "-v"}
	expect := `
      *** https://github.com/rokath/trice ***

      If a non-multi parameter is used more than one times the last value wins.
      No logfile writing...
      syntax: 'trice sub-command' [params]
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
      No logfile writing...done
      `
	execHelper(t, args, expect)
}

func TestRenew(t *testing.T) {
	args := []string{"trice", "help", "-renew"}
	expect := `syntax: 'trice sub-command' [params]
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
	execHelper(t, args, expect)
}
func TestHelpRefresh(t *testing.T) {
	args := []string{"trice", "help", "-refresh"}
	expect := `syntax: 'trice sub-command' [params]
      sub-command 'r|refresh': For updating ID list from source files but does not change the source files.
              "trice refresh" will parse source tree(s) for TRICE macros, and refresh/generate the JSON list.
              This command should be run on adding source files to the project before the first time "trice update" is called.
              If the new source files contain TRICE macros with IDs these are added to til.json if not already used.
              Already used IDs are reported, so you have the chance to remove them from til.son and then do "trice u" again.
              This way you can make sure to get the new sources unchanged in your list.
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
	execHelper(t, args, expect)
}

// needs to work in Linux and Windows!
func _TestCOBSLog(t *testing.T) {
	args := []string{"trice", "log", "-idList", "til.json", "-port", "FILEBUFFER", "-args", "C:/Users/T.Hoehenleitner/AppData/Local/Temp/trice-317444282.bin", "-ts", "off"}
	expect := ""
	execHelper(t, args, expect)
}

/*
func _TestHelpLog(t *testing.T) {
	args := []string{"trice", "help", "-log"}
	expect := `syntax: 'trice sub-command' [params]
      sub-command 'l|log': For displaying trice logs coming from port. With "trice log" the trice tool display mode is activated.
        -args string
              Use to pass port specific parameters. The "default" value depends on the used port:
              port "COMn": default="", use "TARM" for a different driver. (For baud rate settings see -baud.)
              port "J-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000",
                      The -RTTSearchRanges "..." need to be written without "" and with _ instead of space.
                      For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
              port "ST-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000",
                      The -RTTSearchRanges "..." need to be written without "" and with _ instead of space.
                      For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
              port "BUFFER": default="0 0 0 0", Option for args is any byte sequence.
               (default "default")
        -ban value
              Channel(s) to ignore. This is a multi-flag switch. It can be used several times with a colon separated list of channel descriptors not to display.
              Example: "-ban dbg:wrn -ban diag" results in suppressing all as debug, diag and warning tagged messages. Not usable in conjunction with "-pick".
        -baud int
              Set the serial port baudrate.
              It is the only setup parameter. The other values default to 8N1 (8 data bits, no parity, one stopbit).
               (default 115200)
        -color string
              The format strings can start with a lower or upper case channel information.
              See https://github.com/rokath/trice/blob/master/pkg/src/triceCheck.c for examples. Color options:
              "off": Disable ANSI color. The lower case channel information is kept: "w:x"-> "w:x"
              "none": Disable ANSI color. The lower case channel information is removed: "w:x"-> "x"
              "default|color": Use ANSI color codes for known upper and lower case channel info are inserted and lower case channel information is removed.
               (default "default")
        -dc int
              Dumped bytes per line when "-encoding DUMP" (default 32)
        -debug
              Show additional debug information
        -displayserver
              Send trice lines to displayserver @ ipa:ipp.
              Example: "trice l -port COM38 -ds -ipa 192.168.178.44" sends trice output to a previously started display server in the same network.
        -ds
              Short for '-displayserver'.
        -e string
              Short for -encoding. (default "COBS")
        -encoding string
              The trice transmit data format type, options: '(CHAR|COBS|DUMP|ESC|FLEX)'. Target device encoding must match.
                                CHAR prints the received bytes as characters.
                                COBS expects 0 delimited byte sequences.
                                DUMP prints the received bytes as hex code (see switch -dc too).
                                ESC is a legacy format and will be removed in the future.
                                FLEX is a legacy format and will be removed in the future.
               (default "COBS")
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
        -p string
              short for -port (default "J-LINK")
        -password string
              The decrypt passphrase. If you change this value you need to compile the target with the appropriate key (see -showKeys).
              Encryption is recommended if you deliver firmware to customers and want protect the trice log output. This does work right now only with flex and flexL format.
        -pick value
              Channel(s) to display. This is a multi-flag switch. It can be used several times with a colon separated list of channel descriptors only to display.
              Example: "-pick err:wrn -pick default" results in suppressing all messages despite of as error, warning and default tagged messages. Not usable in conjunction with "-ban".
        -port string
              receiver device: 'ST-LINK'|'J-LINK'|serial name.
              The serial name is like 'COM12' for Windows or a Linux name like '/dev/tty/usb12'.
              Using a virtual serial COM port on the PC over a FTDI USB adapter is a most likely variant.
               (default "J-LINK")
        -prefix string
              Line prefix, options: any string or 'off|none' or 'source:' followed by 0-12 spaces, 'source:' will be replaced by source value e.g., 'COM17:'. (default "source: ")
        -pw string
              Short for -password.
        -s    Short for '-showInputBytes'.
        -showID string
              Format string for displaying first trice ID at start of each line. Example: "debug:%7d ". Default is "". If several trices form a log line only the first trice ID ist displayed.
        -showInputBytes
              Show incoming bytes, what can be helpful during setup.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
        -showKey
              Show encryption key. Use this switch for creating your own password keys. If applied together with "-password MySecret" it shows the encryption key.
              Simply copy this key than into the line "#define ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret" inside triceConfig.h.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
        -suffix string
              Append suffix to all lines, options: any string.
        -tLocFmt string
              Target location format string at start of each line, if target location existent (configured). Use "" to suppress existing target location. If several trices form a log line only the location of first trice ist displayed. (default "%12s:%4d ")
        -targetEndianess string
              Target endianness trice data stream. Option: "bigEndian". (default "littleEndian")
        -testTable
              Generate testTable output and ignore -prefix, -suffix, -ts, -color. This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
        -til string
              Short for '-idlist'.
               (default "til.json")
        -ts string
              PC timestamp for logs and logfile name, options: 'off|none|UTCmicro|zero'
              This timestamp switch generates the timestamps on the PC only (reception time), what is good enough for many cases.
              "LOCmicro" means local time with microseconds.
              "UTCmicro" shows timestamps in universal time.
              When set to "off" no PC timestamps displayed.
              If you need target timestamps you need to get the time inside the target and send it as TRICE* parameter.
               (default "LOCmicro")
        -ttsf string
              Target timestamp format string at start of each line, if target timestamps existent (configured). Use "" to suppress existing target timestamps. If several trices form a log line only the timestamp of first trice ist displayed. (default "time:%9d ")
        -u    Short for '-unsigned'. (default true)
        -unsigned
              Hex, Octal and Bin values are printed as unsigned values. (default true)
        -v    short for verbose
        -verbose
              Gives more informal output if used. Can be helpful during setup.
              For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
      example: 'trice l -p COM15 -baud 38400': Display trice log messages from serial port COM15
      example: 'trice l': Display flexL data format trice log messages from default source J-LINK over Segger RTT protocol.
      example: 'trice l -port ST-LINK -v -s': Shows verbose version information and also the received raw bytes.
      `
	execHelper(t, args, expect)
}

*/
