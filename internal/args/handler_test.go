// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"fmt"
	"io/ioutil"
	"os"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/msg"

	"github.com/rokath/trice/pkg/tst"
	"github.com/stretchr/testify/assert"
)

var (
	m *sync.RWMutex // var verbose is used differently in tests, so there parallel execution must be avoided
	h *sync.RWMutex // help vars are used differently in tests, so there parallel execution must be avoided
)

func init() {
	m = new(sync.RWMutex)
	h = new(sync.RWMutex)
}

func TestMain(m *testing.M) {
	id.FnJSON = getTemporaryFileName("til-*.JSON")
	code := m.Run()
	os.Remove(id.FnJSON) // // os.Exit() does not respect defer statements
	os.Exit(code)
}

func testVersion(t *testing.T, v []string) {
	fi, err := os.Stat(os.Args[0])
	assert.Nil(t, err)
	buildTime := fi.ModTime().String()
	exp := v[0] + "version=devel, built " + buildTime + "\n" + v[1]

	fn := func() {
		msg.OnErr(Handler([]string{"trice", "ver"}))
	}
	act := tst.CaptureStdOut(fn)
	assert.Equal(t, exp, act)

	fn = func() {
		msg.OnErr(Handler([]string{"trice", "version"}))
	}
	act = tst.CaptureStdOut(fn)
	assert.Equal(t, exp, act)
}

func TestVersion(t *testing.T) {
	verbose = false
	v := []string{"", ""}
	testVersion(t, v)
	m.Lock()
	verbose = true
	v[0] = "No logfile writing...\nhttps://github.com/rokath/trice\n"
	v[1] = "No logfile writing...done\n"
	testVersion(t, v)
	verbose = false
	m.Unlock()
}

func Example_handlerNone() {
	fn := func() {
		err := Handler([]string{"trice"})
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
	// no args, try: 'trice help'
}

func Example_wrongSubcommand() {
	fn := func() {
		err := Handler([]string{"trice", "xyz"})
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
	// unknown subcommand 'xyz'. try: 'trice help|h'
}

func Example_renew() {
	fn := func() {
		m.Lock()
		err := Handler([]string{"trice", "renew"})
		m.Unlock()
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
}

func Example_refresh() {
	fn := func() {
		m.Lock()
		err := Handler([]string{"trice", "refresh"})
		m.Unlock()
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
}

func Example_update() {
	fn := func() {
		m.Lock()
		err := Handler([]string{"trice", "update"})
		m.Unlock()
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
}

func Example_log() {
	fn := func() {
		m.Lock()
		err := Handler([]string{"trice", "log", "-p", "COMX"})
		m.Unlock()
		if nil != err {
			fmt.Print(err)
		}
	}
	act := tst.CaptureStdOut(fn)
	fmt.Print(act)
	// Output:
	// can not open COMX
}

func Example_help_a() {
	fn := func() {
		err := Handler([]string{"trice", "help"})
		if nil != err {
			fmt.Print(err)
		}
	}
	h.Lock()
	m.Lock()
	act := tst.CaptureStdOut(fn)
	m.Unlock()
	h.Unlock()
	fmt.Print(act)
	// Output:
	// syntax: 'trice subcommand' [params]
	// example 'trice h -help': Print help for help.
}

func TestHelp(t *testing.T) {
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
	exp := "syntax: 'trice subcommand' [params]\nsubcommand 'h|help': For command line usage.\n\t\"trice h\" will print this help text as a whole.\n  -all\n    \tShow all help.\n  -displayserver\n    \tShow ds|displayserver specific help.\n  -ds\n    \tShow ds|displayserver specific help.\n  -h\tShow h|help specific help.\n  -help\n    \tShow h|help specific help.\n  -l\tShow l|log specific help.\n  -log\n    \tShow l|log specific help.\n  -logfile string\n    \tAppend all output to logfile. Options are: 'off|none|filename|auto':\n    \t\"off\": no logfile (same as \"none\")\n    \t\"none\": no logfile (same as \"off\")\n    \t\"auto\": Use as logfile name \"2006-01-02_1504-05_trice.log\" with actual time.\n    \t\"filename\": Any other string than \"auto\", \"none\" or \"off\" is treated as a filename. If the file exists, logs are appended.\n    \tAll trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.\n    \tChange the filename with \"-logfile myName.txt\" or switch logging off with \"-logfile none\".\n    \t (default \"off\")\n  -r\tShow r|refresh specific help.\n  -refresh\n    \tShow r|refresh specific help.\n  -renew\n    \tShow renew specific help.\n  -s\tShow s|scan specific help.\n  -scan\n    \tShow s|scan specific help.\n  -sd\n    \tShow sd|shutdown specific help.\n  -shutdown\n    \tShow sd|shutdown specific help.\n  -u\tShow u|update specific help.\n  -update\n    \tShow u|update specific help.\n  -v\tshort for verbose\n  -ver\n    \tShow ver|version specific help.\n  -verbose\n    \tGives more informal output if used. Can be helpful during setup.\n    \tFor example \"trice u -dry-run -v\" is the same as \"trice u -dry-run\" but with more descriptive output.\n    \tThis is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.\n  -version\n    \tShow ver|version specific help.\n  -z\tShow zeroSourceTreeIds specific help.\n  -zeroSourceTreeIds\n    \tShow zeroSourceTreeIds specific help.\nexample 'trice h': Print short help.\nexample 'trice h -all': Print all help.\nexample 'trice h -log': Print log help.\n"
	assert.Equal(t, exp, act)
}

func getTemporaryFileName(pattern string) string {
	tempFileHandle, e := ioutil.TempFile(os.TempDir(), pattern) // opens for read and write
	msg.OnErr(e)
	tempFileName := tempFileHandle.Name()
	msg.OnErr(tempFileHandle.Close())
	return tempFileName
}

func TestHelpAll(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "help", "-all"})
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
	exp := `syntax: 'trice subcommand' [params]
subcommand 'ds|displayServer': Starts a display server.
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
        You can specify this swich if you intend to use the remote display option to show the output on a different PC in the network.
         (default "localhost")
  -ipp string
        16 bit IP port number.
        You can specify this swich if you want to change the used port number for the remote display functionality.
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
subcommand 'h|help': For command line usage.
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
subcommand 'l|log': For displaying trice logs coming from port. With "trice log" the trice tool display mode is activated.
  -a    Short for '-autostart'.
  -args string
        Use to pass port specific parameters. The "default" value depends on the used port:
        port "COMn": default="", use "TARM" for a different driver. (For baud rate settings see -baud.)
        port "J-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000",
                The -RTTSearchRanges "..." need to be written without "" and with _ istead of space.
                For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
        port "ST-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000",
                The -RTTSearchRanges "..." need to be written without "" and with _ istead of space.
                For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
        port "BUFFER": default="0 0 0 0", Option for args is any byte sequence.
         (default "default")
  -autostart
        Autostart displayserver @ ipa:ipp.
        Works not perfect with windows, because of cmd and powershell color issues and missing cli params in wt and gitbash.
        Example: "trice l -port COM38 -displayserver -autostart" opens a separate display window automatically on the same PC.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
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
  -displayserver
        Send trice lines to displayserver @ ipa:ipp.
        Example: "trice l -port COM38 -ds -ipa 192.168.178.44" sends trice output to a previously started display server in the same network.
  -ds
        Short for '-displayserver'.
  -e string
        Short for -encoding. (default "flexL")
  -encoding string
        The trice transmit data format type, options: 'esc|ESC|(flex|FLEX)[(l|L)'. Target device encoding must match. (default "flexL")
  -i string
        Short for '-idlist'.
         (default "til.json")
  -idlist string
        The trice ID list file.
        The specified JSON file is needed to display the ID coded trices during runtime and should be under version control.
         (default "til.json")
  -ipa string
        IP address like '127.0.0.1'.
        You can specify this swich if you intend to use the remote display option to show the output on a different PC in the network.
         (default "localhost")
  -ipp string
        16 bit IP port number.
        You can specify this swich if you want to change the used port number for the remote display functionality.
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
  -showInputBytes
        Show incoming bytes, what can be helpful during setup.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -showKey
        Show encryption key. Use this switch for creating your own password keys. If applied together with "-password MySecret" it shows the encryption key.
        Simply copy this key than into the line "#define ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret" inside triceConfig.h.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -suffix string
        Append suffix to all lines, options: any string.
  -testTable
        Generate testTable output and ignore -prefix, -suffix, -ts, -color. This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -ts string
        PC timestamp for logs and logfile name, options: 'off|none|UTCmicro|zero'
        This timestamp switch generates the timestamps on the PC only (reception time), what is good enough for many cases.
        "LOCmicro" means local time with microseconds.
        "UTCmicro" shows timestamps in universal time.
        When set to "off" no PC timestamps displayed.
        If you need target timestamps you need to get the time inside the target and send it as TRICE* parameter.
         (default "LOCmicro")
  -u    Short for '-unsignedHex'.
  -unsignedHex
        Hex and Bin values are printed as unsigned values.
  -v    short for verbose
  -verbose
        Gives more informal output if used. Can be helpful during setup.
        For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
example: 'trice l -p COM15 -baud 38400': Display trice log messages from serial port COM15
example: 'trice l': Display flexL data format trice log messages from default source J-LINK over Segger RTT protocol.
example: 'trice l -port ST-LINK -v -s': Shows verbose version information and also the received raw bytes.
subcommand 'r|refresh': For updating ID list from source files but does not change the source files.
        "trice refresh" will parse source tree(s) for TRICE macros, and refresh/generate the JSON list.
        This command should be run on adding souce files to the project before the first time "trice update" is called.
        If the new source files contain TRICE macros with IDs these are added to til.json if not already used.
        Already used IDs are reported, so you have the chance to remnove them from til.son and then do "trice u" again.
        This way you can make sure to get the new sources unchanged in your list. (to do: -force switch)
        Already used IDs are replaced by new IDs during the next "trice update", so the old IDs in the list will survive.
        If you do not refresh the list after adding source files and perform an "trice update" new generated IDs could be equal to
        IDs used in the added sources with the result that IDs in the added sources could get changed what you may not want.
        Using "trice u -IDMethod random" (default) makes the chance for such conflicts very low.
        The "refresh" subcommand has no mantadory switches. Omitted optional switches are used with their default parameters.
  -dry-run
        No changes applied but output shows what would happen.
        "trice refresh -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -i string
        Short for '-idlist'.
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
  -v    short for verbose
  -verbose
        Gives more informal output if used. Can be helpful during setup.
        For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
example: 'trice refresh': Update ID list from source tree.
subcommand 'renew': It is like refresh, but til.json is cleared first, so all 'old' trices are removed. Use with care.
  -dry-run
        No changes applied but output shows what would happen.
        "trice renew -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -i string
        Short for '-idlist'.
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
  -v    short for verbose
  -verbose
        Gives more informal output if used. Can be helpful during setup.
        For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
example: 'trice renew': Rebuild ID list from source tree, discard old IDs.
subcommand 's|scan': Shows available serial ports)
example: 'trice s': Show COM ports.
subcommand 'sd|shutdown': Ends display server at IPA:IPP, works also on a remote mashine.
  -ipa string
        IP address like '127.0.0.1'.
        You can specify this swich if you intend to use the remote display option to show the output on a different PC in the network.
         (default "localhost")
  -ipp string
        16 bit IP port number.
        You can specify this swich if you want to change the used port number for the remote display functionality.
         (default "61497")
example: 'trice sd': Shut down remote display server.
subcommand 'ver|version': For displaying version information.
        "trice v" will print the version information. In trice is unversioned the build time will be displayed instead.
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
subcommand 'u|update': For updating ID list and source files.
        "trice update" will parse source tree(s) for new or changed TRICE macros, modify them appropriate and update/generate the JSON list.
        The "update" subcommand has no mantadory switches. Omitted optional switches are used with their default parameters.
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
  -v    short for verbose
  -verbose
        Gives more informal output if used. Can be helpful during setup.
        For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
example: 'trice update -src ../A -src ../../B': Parse ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json
subcommand 'zeroSourceTreeIds': Set all Id(n) inside source tree dir to Id(0).
        Avoid using this subcommand normally. The switch "-src" is mantadory and no multi-flag here.
        This subcommand is mainly for testing. For several source directories you need several runs.
  -dry-run
        No changes applied but output shows what would happen.
        "trice zeroSourceTreeIds -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -src string
        Zero all Id(n) inside source tree dir, required.
example: 'trice zeroSourceTreeIds -src ../A': Sets all TRICE IDs to 0 in ../A. Use with care!
`
	tst.EqualLines(t, exp, act)
}
