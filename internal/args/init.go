// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"flag"
	"fmt"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/pkg/cage"
)

var (
	colorInfo = `The format strings can start with a lower or upper case channel infomation.
See https://github.com/rokath/trice/blob/master/srcTrice.C/triceCheck.c for examples. Color options: 
"off": Disable ANSI color. The lower case channel information is kept: "w:x"-> "w:x" 
"none": Disable ANSI color. The lower case channel information is removed: "w:x"-> "x"
"default": Use ANSI color codes for known upper and lower case channel info are inserted and lower case channel information is removed.
`

	boolInfo = "This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true."
)

func init() {
	fsScLog = flag.NewFlagSet("log", flag.ExitOnError)                                                                                                                           // subcommand
	fsScLog.StringVar(&decoder.Encoding, "encoding", "packl", "The trice transmit data format type, options: 'pack[l]|esc|bare[l]|wrap[l]'. Target device encoding must match.") // flag
	fsScLog.StringVar(&decoder.Encoding, "e", "packl", "Short for -encoding.")                                                                                                   // short flag
	//fsScLog.StringVar(&cipher.Password, "password", "none", "The decrypt passphrase.")                                                                                                                                                // flag
	//fsScLog.StringVar(&cipher.Password, "pw", "none", "Short for -password.")                                                                                                                                                     // short flag
	//	fsScLog.BoolVar(&cipher.ShowKey, "key", false, `Show encryption key.
	//- +boolInfo
	//- Use this switch for creating your own password keys. If applied together with "-password MyPwd" it shows the encryption key.
	//Simply copy this key than into the line
	//"#define ENCRYPT XTEA_KEY( a9, 4a, 8f, e5, cc, b1, 9b, a6, 1c, 4c, 08, 73, d3, 91, e9, 87 ); //!< -key test" inside triceConfig.h.`)
	// flag
	//- This is a `string` switch. It has one parameter. Its default value is `none`. If you change this value you need to compile the target with the appropriate key.
	//- This option is recommended if you deliver firmware to customers and want protect the trice log output. This does not work together with the `#define TRICE_STRINGS FULL_RUNTIME` option right now.

	fsScLog.StringVar(&emitter.TimestampFormat, "ts", "LOCmicro",
		`PC timestamp for logs and logfile name, options: 'off|none|UTCmicro|zero'
This timestamp switch generates the timestamps on the PC only (reception time), what is good enough for many cases. 
"LOCmicro" means local time with microseconds.
"UTCmicro" shows timestamps in universal time.
When set to "off" no PC timestamps displayed.
If you need target timestamps you need to get the time inside the target and send it as TRICE* parameter.
`) // flag

	fsScLog.StringVar(&emitter.ColorPalette, "color", "default", colorInfo)                                                                                                                                     // flag
	fsScLog.StringVar(&emitter.Prefix, "prefix", "source: ", "Line prefix, options: any string or 'off|none' or 'source:' followed by 0-12 spaces, 'source:' will be replaced by source value e.g., 'COM17:'.") // flag
	fsScLog.StringVar(&emitter.Suffix, "suffix", "", "Append suffix to all lines, options: any string.")                                                                                                        // flag

	info := fmt.Sprint(`receiver device: 'STLINK'|'JLINK'|serial name. 
The serial name is like 'COM12' for Windows or a Linux name like '/dev/tty/usb12'. 
Using a virtual serial COM port on the PC over a FTDI USB adapter is a most likely variant.
`)

	fsScLog.StringVar(&receiver.Port, "port", "JLINK", info)           // flag
	fsScLog.StringVar(&receiver.Port, "p", "JLINK", "short for -port") // short flag
	fsScLog.IntVar(&com.Baud, "baud", 115200, `Set the serial port baudrate.
It is the only setup parameter. The other values default to 8N1 (8 data bits, no parity, one stopbit).
`) // flag flag

	linkArgsInfo := `
	The -RTTSearchRanges "..." need to be written without "" and with _ istead of space.
	For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.`

	argsInfo := fmt.Sprint(`Use to pass port specific parameters. The "default" value depends on the used port:
port "COMn": default="`, defaultCOMArgs, `", use "TARM" for a different driver. (For baud rate settings see -baud.)
port "JLINK": default="`, defaultLinkArgs, `", `, linkArgsInfo, `
port "STLINK": default="`, defaultLinkArgs, `", `, linkArgsInfo, `
port "BUFFER": default="`, defaultBUFFERArgs, `", Option for args is any byte sequence.
`)

	fsScLog.StringVar(&receiver.PortArguments, "args", "default", argsInfo)
	fsScLog.BoolVar(&emitter.DisplayRemote, "displayserver", false, `Send trice lines to displayserver @ ipa:ipp.
Example: "trice l -port COM38 -ds -ipa 192.168.178.44" sends trice output to a previously started display server in the same network.`)
	fsScLog.BoolVar(&emitter.DisplayRemote, "ds", false, "Short for '-displayserver'.")

	fsScLog.BoolVar(&emitter.Autostart, "autostart", false, `Autostart displayserver @ ipa:ipp.
Works not perfect with windows, because of cmd and powershell color issues and missing cli params in wt and gitbash.
Example: "trice l -port COM38 -displayserver -autostart" opens a separate display window automatically on the same PC.
`+boolInfo)

	fsScLog.BoolVar(&emitter.Autostart, "a", false, "Short for '-autostart'.")
	fsScLog.BoolVar(&receiver.ShowInputBytes, "showInputBytes", false, `Show incoming bytes, what can be helpful during setup.
`+boolInfo)
	fsScLog.BoolVar(&receiver.ShowInputBytes, "s", false, "Short for '-showInputBytes'.")
	flagLogfile(fsScLog)
	flagVerbosity(fsScLog)
	flagIDList(fsScLog)
	flagIPAddress(fsScLog)
}

func init() {
	fsScUpdate = flag.NewFlagSet("update", flag.ExitOnError) // subcommand
	fsScUpdate.BoolVar(&id.DryRun, "dry-run", false, `No changes applied but output shows what would happen.
"trice u -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
`+boolInfo) // flag

	fsScUpdate.Var(&id.Srcs, "src", `Source dir or file, It has one parameter. Not usable in the form "-src *.c".
This is a multi-flag switch. It can be used several times and for directories and also for files. 
Example: "trice u  -dry-run -v -src ./test/ -src srcTrice.C/trice.h" will scan all C|C++ header and 
source code files inside directory ./test and scan also file trice.h inside srcTrice.C directory. 
Without the "-dry-run" switch it would create|extend a list file til.json in the current directory.
 (default "./")`) // multi flag
	fsScUpdate.Var(&id.Srcs, "s", "Short for src.") // multi flag
	flagVerbosity(fsScUpdate)
	flagIDList(fsScUpdate)
}

func init() {
	fsScZero = flag.NewFlagSet("zeroSourceTreeIds", flag.ContinueOnError)
	pSrcZ = fsScZero.String("src", "", "Zero all Id(n) inside source tree dir, required.") // flag
	fsScZero.BoolVar(&id.DryRun, "dry-run", false, `No changes applied but output shows what would happen.
`+boolInfo) // flag
}

func init() {
	fsScHelp = flag.NewFlagSet("help", flag.ContinueOnError) // subcommand
	flagLogfile(fsScHelp)
	flagVerbosity(fsScHelp)
}

func init() {
	fsScVersion = flag.NewFlagSet("version", flag.ContinueOnError) // subcommand
	flagLogfile(fsScVersion)
	flagVerbosity(fsScVersion)
}

func init() {
	fsScSv = flag.NewFlagSet("displayServer", flag.ExitOnError)            // subcommand
	fsScSv.StringVar(&emitter.ColorPalette, "color", "default", colorInfo) // flag
	flagLogfile(fsScSv)
	flagIPAddress(fsScSv)
}

func init() {
	fsScScan = flag.NewFlagSet("scan", flag.ContinueOnError) // subcommand
}

func init() {
	fsScSdSv = flag.NewFlagSet("shutdownServer", flag.ExitOnError) // subcommand
	flagIPAddress(fsScSdSv)
}

func flagLogfile(p *flag.FlagSet) {
	p.StringVar(&cage.Name, "logfile", "off", `Append all output to logfile. Options are: 'off|none|filename|auto':
"off": no logfile (same as "none")
"none": no logfile (same as "off")
"auto": Use as logfile name "2006-01-02_1504-05_trice.log" with actual time.
"filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
All trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.
Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
`) // flag
	p.StringVar(&cage.Name, "l", "off", `Short for -logfile.
`) // short flag
}

func flagVerbosity(p *flag.FlagSet) {
	p.BoolVar(&verbose, "verbose", false, `Gives more informal output if used. Can be helpful during setup.
For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
`+boolInfo) // flag
	p.BoolVar(&verbose, "v", false, "short for verbose") // flag
}

func flagIDList(p *flag.FlagSet) {
	p.StringVar(&id.FnJSON, "idlist", "til.json", `The trice ID list file.
The specified JSON file is the key to display the ID coded trices during runtime and should be under version control.
`) // flag
	p.StringVar(&id.FnJSON, "i", "til.json", `Short for '-idlist'.
`) // flag
}

func flagIPAddress(p *flag.FlagSet) {
	p.StringVar(&emitter.IPAddr, "ipa", "localhost", `IP address like '127.0.0.1'.
You can specify this swich if you intend to use the remote display option to show the output on a different PC in the network.
`) // flag

	p.StringVar(&emitter.IPPort, "ipp", "61497", `16 bit IP port number.
You can specify this swich if you want to change the used port number for the remote display functionality.
`) // flag
}
