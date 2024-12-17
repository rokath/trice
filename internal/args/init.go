// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"flag"
	"fmt"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/do"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/translator"
	"github.com/rokath/trice/internal/trexDecoder"
	"github.com/rokath/trice/pkg/cipher"
)

const (
	defaultPrefix = "source: "
)

var (

	// LogfileName is the filename of the logfile. "off" inhibits logfile writing.
	LogfileName = "off"

	colorInfo = `The format strings can start with a lower or upper case channel information.
See https://github.com/rokath/trice/blob/master/pkg/src/triceCheck.c for examples. Color options: 
"off": Disable ANSI color. The lower case channel information is kept: "w:x"-> "w:x" 
"none": Disable ANSI color. The lower case channel information is removed: "w:x"-> "x"
"default|color": Use ANSI color codes for known upper and lower case channel info are inserted and lower case channel information is removed.
`
	boolInfo = "This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true."
)

func init() {
	FlagsInit()
}

func FlagsInit() {
	helpInit()
	logInit()
	addInit()
	generateInit()
	insertIDsInit()
	cleanIDsInit()
	versionInit()
	dsInit()
	scanInit()
	sdInit()
}

func helpInit() {
	fsScHelp = flag.NewFlagSet("help", flag.ContinueOnError) // sub-command
	fsScHelp.BoolVar(&allHelp, "all", false, "Show all help.")
	fsScHelp.BoolVar(&displayServerHelp, "displayserver", false, "Show ds|displayserver specific help.")
	fsScHelp.BoolVar(&displayServerHelp, "ds", false, "Show ds|displayserver specific help.")
	fsScHelp.BoolVar(&helpHelp, "help", false, "Show h|help specific help.")
	fsScHelp.BoolVar(&helpHelp, "h", false, "Show h|help specific help.")
	fsScHelp.BoolVar(&logHelp, "log", false, "Show l|log specific help.")
	fsScHelp.BoolVar(&logHelp, "l", false, "Show l|log specific help.")
	fsScHelp.BoolVar(&generateHelp, "generate", false, "Show generate specific help.")
	fsScHelp.BoolVar(&addHelp, "add", false, "Show a|add specific help.")
	fsScHelp.BoolVar(&addHelp, "a", false, "Show a|add specific help.")
	fsScHelp.BoolVar(&scanHelp, "scan", false, "Show s|scan specific help.")
	fsScHelp.BoolVar(&scanHelp, "s", false, "Show s|scan specific help.")
	fsScHelp.BoolVar(&shutdownHelp, "shutdown", false, "Show sd|shutdown specific help.")
	fsScHelp.BoolVar(&shutdownHelp, "sd", false, "Show sd|shutdown specific help.")
	fsScHelp.BoolVar(&insertIDsHelp, "insert", false, "Show i|insert specific help.")
	fsScHelp.BoolVar(&insertIDsHelp, "i", false, "Show i|insert specific help.")
	fsScHelp.BoolVar(&versionHelp, "version", false, "Show ver|version specific help.")
	fsScHelp.BoolVar(&versionHelp, "ver", false, "Show ver|version specific help.")
	fsScHelp.BoolVar(&cleanIDsHelp, "clean", false, "Show c|clean specific help.")
	fsScHelp.BoolVar(&cleanIDsHelp, "c", false, "Show c|clean specific help.")
	flagLogfile(fsScHelp)
	flagVerbosity(fsScHelp)
}

func logInit() {
	const defaultEncoding = "TREX"
	fsScLog = flag.NewFlagSet("log", flag.ExitOnError) // sub-command
	fsScLog.StringVar(&translator.Encoding, "encoding", defaultEncoding, `The trice transmit data format type, options: '(CHAR|DUMP|TREX)'. Target device encoding must match. 
		  TREX=TriceExtendableEncoding, see Trice1.0Specification. Needs '#define TRICE_ENCODING TRICE_TREX_ENCODING' inside triceConfig.h.
		  CHAR prints the received bytes as characters.
		  DUMP prints the received bytes as hex code (see switch -dc too).
`) // flag
	fsScLog.StringVar(&translator.Encoding, "e", defaultEncoding, "Short for -encoding.") // short flag
	fsScLog.IntVar(&decoder.DumpLineByteCount, "dc", 32, `Dumped bytes per line when "-encoding DUMP"`)
	fsScLog.IntVar(&decoder.NewlineIndent, "newlineIndent", -1, `Force newline offset for trice format strings with line breaks before end. -1=auto sense`)
	fsScLog.StringVar(&cipher.Password, "password", "", `The decrypt passphrase. If you change this value you need to compile the target with the appropriate key (see -showKeys).
Encryption is recommended if you deliver firmware to customers and want protect the trice log output. This does work right now only with flex and flexL format.`) // flag
	fsScLog.StringVar(&cipher.Password, "pw", "", "Short for -password.") // short flag
	fsScLog.BoolVar(&cipher.ShowKey, "showKey", false, `Show encryption key. Use this switch for creating your own password keys. If applied together with "-password MySecret" it shows the encryption key.
Simply copy this key than into the line "#define ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret" inside triceConfig.h.
`+boolInfo)
	fsScLog.StringVar(&emitter.LogLevel, "logLevel", "all", `Level based log filtering. "off" suppresses everything. If equal to a channel specifier all with a bigger index inside emitter.ColorChannels the log not shown. 
A typical use case is "-logLevel wrn". Attention this switch influences also location information (-liFmt), target stamps (-ts0, -ts16, -ts32), prefix and suffix information. Set these channel information appropriate. 
Logs without channel specifier are not suppressed. Using an invalid value like "x" suppresses all logs with a channel specifier. See also CLI switches -pick and -ban.`)
	fsScLog.StringVar(&id.DefaultTriceBitWidth, "defaultTRICEBitwidth", "32", `The expected value bit width for TRICE macros. Options: 8, 16, 32, 64. Must be in sync with the 'TRICE_DEFAULT_PARAMETER_BIT_WIDTH' setting inside triceConfig.h`)
	fsScLog.StringVar(&emitter.HostStamp, "hs", "LOCmicro",
		`PC timestamp for logs and logfile name, options: 'off|none|UTCmicro|zero'
This timestamp switch generates the timestamps on the PC only (reception time), what is good enough for many cases. 
"LOCmicro" means local time with microseconds. "UTCmicro" shows timestamps in universal time. When set to "off" no PC timestamps displayed.`) // flag
	fsScLog.StringVar(&decoder.ShowID, "showID", "", `Format string for displaying first trice ID at start of each line. Example: "debug:%7d ". Default is "". If several trices form a log line only the first trice ID ist displayed.`)
	fsScLog.StringVar(&decoder.LocationInformationFormatString, "liFmt", "info:%21s %5d ", `Target location format string at start of each line, if target location existent (configured). Use "off" or "none" to suppress existing target location. If several trices form a log line only the location of first trice ist displayed.`)
	fsScLog.StringVar(&decoder.TargetStamp, "ts", "µs", `Target timestamp general format string at start of each line, if target timestamps existent (configured). Choose between "µs" (or "us") and "ms", use "" or 'off' or 'none' to suppress existing target timestamps. Sets ts0, ts16, ts32 if these not passed. If several trices form a log line only the timestamp of first trice ist displayed.`)
	fsScLog.StringVar(&decoder.TargetStamp32, "ts32", "ms", `32-bit Target stamp format string at start of each line, if 32-bit target stamps existent (configured). Choose between "µs" (or "us") and "ms", use "" to suppress or use s.th. like "...%d...". If several trices form a log line only the timestamp of first trice ist displayed.`)
	fsScLog.StringVar(&decoder.TargetStamp16, "ts16", "ms", `16-bit Target stamp format string at start of each line, if 16-bit target stamps existent (configured). Choose between "µs" (or "us") and "ms", use "" to suppress or use s.th. like "...%d...". If several trices form a log line only the timestamp of first trice ist displayed.`)
	fsScLog.StringVar(&decoder.TargetStamp0, "ts0", translator.DefaultTargetStamp0, `Target stamp format string at start of each line, if no target stamps existent (configured). Use "" to suppress existing target timestamps. If several trices form a log line only the timestamp of first trice ist displayed.`)
	fsScLog.BoolVar(&decoder.DebugOut, "debug", false, "Show additional debug information")
	fsScLog.StringVar(&translator.TriceEndianness, "triceEndianness", "littleEndian", `Target endianness trice data stream. Option: "bigEndian".`)
	fsScLog.StringVar(&emitter.ColorPalette, "color", "default", colorInfo)                                                                                                                                        // flag
	fsScLog.StringVar(&emitter.Prefix, "prefix", defaultPrefix, "Line prefix, options: any string or 'off|none' or 'source:' followed by 0-12 spaces, 'source:' will be replaced by source value e.g., 'COM17:'.") // flag
	fsScLog.StringVar(&emitter.Suffix, "suffix", "", "Append suffix to all lines, options: any string.")                                                                                                           // flag

	info := `Case insensitive receiver device name: 'serial name|JLINK|STLINK|FILE|FILEBUFFER|TCP4|TCP4BUFFER|DEC|BUFFER|HEX|DUMP. 
The serial name is like 'COM12' for Windows or a Linux name like '/dev/tty/usb12'. 
Using a virtual serial COM port on the PC over a FTDI USB adapter is a most likely variant.
`
	fsScLog.StringVar(&receiver.Port, "port", "J-LINK", info)           // flag
	fsScLog.StringVar(&receiver.Port, "p", "J-LINK", "short for -port") // short flag
	fsScLog.IntVar(&com.BaudRate, "baud", 115200, `Set the serial port baudrate.
It is the only setup parameter. The other values default to 8N1 (8 data bits, no parity, one stopbit).
`)
	fsScLog.IntVar(&com.DataBits, "databits", 8, `Set the serial port databits, options: 7, 9`)
	fsScLog.StringVar(&com.Parity, "parity", "none", `Serial port bit parity value, options: odd, even`) // flag
	fsScLog.StringVar(&com.StopBits, "stopbits", "1", `Serial port stopbit, options: 1.5, 2`)            // flag
	linkArgsInfo := `
	The -RTTSearchRanges "..." need to be written without "" and with _ instead of space.
	For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.`

	argsInfo := fmt.Sprint(`Use to pass port specific parameters. The "default" value depends on the used port:
port "COMn": default="`, receiver.DefaultCOMArgs, `", Unused option for a different driver. (For baud rate settings see -baud.)
port "J-LINK": default="`, receiver.DefaultLinkArgs, `", `, linkArgsInfo, `
port "ST-LINK": default="`, receiver.DefaultLinkArgs, `", `, linkArgsInfo, `
port "FILE": default="`, receiver.DefaultFileArgs, `", Option for args is any file name for binary log data like written []byte{115, 111, 109, 101, 10}. Trice retries on EOF.
port "FILEBUFFER": default="`, receiver.DefaultFileArgs, `", Option for args is any file name for binary log data like written []byte{115, 111, 109, 101, 10}. Trice stops on EOF.
port "TCP4": default="`, receiver.DefaultTCP4Args, `", use any IP:port endpoint like "127.0.0.1:19021". This port is usable for reading, when the Trice logs go into a TCP server.
port "TCP4BUFFER": default="`, receiver.DefaultTCP4Args, `". This port is used for "-port TCP4" testing, to shutdown the Trice tool automatically.
port "DEC" or "BUFFER": default="`, receiver.DefaultBUFFERArgs, `", Option for args is any space separated decimal number byte sequence. Example -p BUFFER -args "7 123 44".
port "HEX" or "DUMP": default="`, receiver.DefaultDumpArgs, `", Option for args is any space or comma separated byte sequence in hex. Example: -p DUMP -args "7B 1A ee,88, 5a".
`)

	execInfo := `Use to pass an additional command line for port TCP4 (like gdbserver start).`

	fsScLog.StringVar(&receiver.PortArguments, "args", "default", argsInfo)
	fsScLog.StringVar(&do.TCPOutAddr, "tcp", "", `TCP address for an external log receiver like Putty. Example: 1st: "trice log -p COM1 -tcp localhost:64000", 2nd "putty". In "Terminal" enable "Implicit CR in every LF", In "Session" Connection type:"Other:Telnet", specify "hostname:port" here like "localhost:64000".`)
	fsScLog.BoolVar(&emitter.DisplayRemote, "displayserver", false, `Send trice lines to displayserver @ ipa:ipp.
Example: "trice l -port COM38 -ds -ipa 192.168.178.44" sends trice output to a previously started display server in the same network.`)
	fsScLog.BoolVar(&emitter.DisplayRemote, "ds", false, "Short for '-displayserver'.")
	fsScLog.BoolVar(&trexDecoder.Doubled16BitID, "doubled16BitID", false, `Tells, that 16-bit IDs are doubled. That switch is needed when un-routed direct output is used like (TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1), but also with double buffer in (TRICE_DEFERRED_TRANSFER_MODE==TRICE_MULTI_PACK_MODE) and XTEA encryption. Read the user guide for more details.`)
	fsScLog.BoolVar(&trexDecoder.Doubled16BitID, "d16", false, "Short for '-Doubled16BitID'.")
	fsScLog.BoolVar(&trexDecoder.SingleFraming, "singleFraming", false, `singleFraming demands, that each received package contains not more than a singe Trice message. This is the case for all direct modes or if you "#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE" in your triceConfig.h file. Applying this switch makes sense with COBS and TCOBS framing to improve Trice tool error recognition in received binary data. Not usable together with encryption because of the padding bytes.`)

	fsScLog.StringVar(&receiver.ExecCommand, "exec", "", execInfo)

	fsScLog.BoolVar(&decoder.Unsigned, "unsigned", true, "Hex, Octal and Bin values are printed as unsigned values. For signed output use -unsigned=false")
	fsScLog.BoolVar(&decoder.Unsigned, "u", true, "Short for '-unsigned'.")
	fsScLog.BoolVar(&receiver.ShowInputBytes, "showInputBytes", false, `Show incoming bytes, what can be helpful during setup.
`+boolInfo)
	fsScLog.BoolVar(&receiver.ShowInputBytes, "s", false, "Short for '-showInputBytes'.")
	fsScLog.BoolVar(&decoder.TestTableMode, "testTable", false, `Generate testTable output and ignore -prefix, -suffix, -ts, -color. `+boolInfo)
	flagLogfile(fsScLog)
	flagBinaryLogfile(fsScLog)
	flagVerbosity(fsScLog)
	flagIDList(fsScLog)
	flagLIList(fsScLog)
	flagIPAddress(fsScLog)
	fsScLog.Var(&emitter.Ban, "ban", `Channel(s) to ignore. This is a multi-flag switch. It can be used several times with a colon separated list of channel descriptors not to display.
Example: "-ban dbg:wrn -ban diag" results in suppressing all as debug, diag and warning tagged messages. Not usable in conjunction with "-pick". See also "-logLevel".`) // multi flag
	fsScLog.Var(&emitter.Pick, "pick", `Channel(s) to display. This is a multi-flag switch. It can be used several times with a colon separated list of channel descriptors only to display.
Example: "-pick err:wrn -pick default" results in suppressing all messages despite of as error, warning and default tagged messages. Not usable in conjunction with "-ban". See also "-logLevel".`) // multi flag
	fsScLog.StringVar(&decoder.PackageFraming, "packageFraming", "TCOBSv1", `Use "none" or "COBS" as alternative. "COBS" needs "#define TRICE_FRAMING TRICE_FRAMING_COBS" inside "triceConfig.h".`)
	fsScLog.StringVar(&decoder.PackageFraming, "pf", "TCOBSv1", "Short for '-packageFraming'.")
	fsScLog.BoolVar(&trexDecoder.AddNewlineToEachTriceMessage, "addNL", false, `Add a newline char at trice messages end to use for example "hi" instead of "hi\n" in source code.`)
	fsScLog.BoolVar(&emitter.TagStatistics, "tagStat", false, `Print Trices occurrences count on exit.`)
	fsScLog.BoolVar(&decoder.TriceStatistics, "triceStat", false, `Print Trices occurrences count on exit.`)
	fsScLog.BoolVar(&emitter.AllStatistics, "stat", false, `Print complete statistics on exit.`)
}

func addInit() {
	fsScAdd = flag.NewFlagSet("add", flag.ExitOnError) // sub-command
	flagsRefreshAndUpdate(fsScAdd)
}

func generateInit() {
	fsScGenerate = flag.NewFlagSet("generate", flag.ExitOnError) // sub-command
	flagIDList(fsScGenerate)
	flagVerbosity(fsScGenerate)
	fsScGenerate.BoolVar(&id.GenerateTilH, "tilH", false, `Create a til.h file.`)
	fsScGenerate.BoolVar(&id.GenerateTilC, "tilC", false, `Create a til.c file.`)
	fsScGenerate.BoolVar(&id.GenerateTilCS, "tilCS", false, `Create a til.cs file.`)
	fsScGenerate.BoolVar(&id.GenerateRpcH, "rpcH", false, `Create a til_rpc.h file.`)
	fsScGenerate.BoolVar(&id.GenerateRpcC, "rpcC", false, `Create a til_rpc.c file.`)
	fsScGenerate.BoolVar(&id.WriteAllColors, "colors", false, `Write all possible colors.`)
}

func insertIDsInit() {
	fsScInsert = flag.NewFlagSet("insert", flag.ExitOnError) // sub-command
	flagsRefreshAndUpdate(fsScInsert)
	flagTriceIDRange(fsScInsert)
	fsScInsert.Var(&id.Min, "IDMin", "Lower end of ID range for normal trices.")
	fsScInsert.Var(&id.Max, "IDMax", "Upper end of ID range for normal trices.")
	fsScInsert.IntVar(&id.DefaultStampSize, "defaultStampSize", 32, "Default stamp size for written TRICE macros without id(0), Id(0 or ID(0). Valid values are 0, 16 or 32.")
	fsScInsert.StringVar(&id.SearchMethod, "IDMethod", "random", "Search method for new ID's in range- Options are 'upward', 'downward' & 'random'.")
	fsScInsert.BoolVar(&id.ExtendMacrosWithParamCount, "addParamCount", false, "Extend TRICE macro names with the parameter count _n to enable compile time checks.")
	fsScInsert.BoolVar(&id.TriceCacheEnabled, "cache", false, `Use "~/.trice/cache/" for fast ID insert (EXPERIMENTAL!). The folder must exist.`)
	fsScInsert.BoolVar(&id.SpaceInsideParenthesis, "spaceInsideParenthesis", false, "Add space inside Trice braces: `trice(<space>iD(<space>123<space>), \"...);`. Use this if your default code auto-formatting is with space inside braces.")
	fsScInsert.BoolVar(&id.SpaceInsideParenthesis, "w", false, "Short for (white)spaceInsideParenthesis or \"wide\".")
}

func cleanIDsInit() {
	fsScClean = flag.NewFlagSet("clean", flag.ContinueOnError) // sub-command
	flagsRefreshAndUpdate(fsScClean)
	fsScClean.BoolVar(&id.TriceCacheEnabled, "cache", false, `Use "~/.trice/cache/" for fast ID clean (EXPERIMENTAL!). The folder must exist.`)
	fsScClean.BoolVar(&id.SpaceInsideParenthesis, "spaceInsideParenthesis", false, "Add space after Trice opening brace: `trice(<space>\"...)`. Use this if your default code auto-formatting is with space after opening brace.")
	fsScClean.BoolVar(&id.SpaceInsideParenthesis, "w", false, "Short for (white)spaceInsideParenthesis or \"wide\".")
}

func versionInit() {
	fsScVersion = flag.NewFlagSet("version", flag.ContinueOnError) // sub-command
	flagLogfile(fsScVersion)
	flagVerbosity(fsScVersion)
}

func dsInit() {
	fsScSv = flag.NewFlagSet("displayServer", flag.ExitOnError)            // sub-command
	fsScSv.StringVar(&emitter.ColorPalette, "color", "default", colorInfo) // flag
	flagLogfile(fsScSv)
	flagIPAddress(fsScSv)
}

func scanInit() {
	fsScScan = flag.NewFlagSet("scan", flag.ContinueOnError) // sub-command
}

func sdInit() {
	fsScSdSv = flag.NewFlagSet("shutdown", flag.ExitOnError) // sub-command
	flagIPAddress(fsScSdSv)
}

func flagsRefreshAndUpdate(p *flag.FlagSet) {
	flagDryRun(p)
	flagSrcs(p)
	flagVerbosity(p)
	flagIDList(p)
	flagLIList(p)
	flagSkipAdditionalChecks(p)
}

func flagBinaryLogfile(p *flag.FlagSet) {
	p.StringVar(&receiver.BinaryLogfileName, "binaryLogfile", "off", `Append all output to logfile. Options are: 'off|none|filename|auto':
"off": no binary logfile (same as "none")
"none": no binary logfile (same as "off")
"my/path/auto": Use as binary logfile name "my/path/2006-01-02_1504-05_trice.bin" with actual time. "my/path/" must exist.
"filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
All trice output of the appropriate subcommands is appended per default into the logfile additionally to the normal output.
Change the filename with "-binaryLogfile myName.bin" or switch logging off with "-binaryLogfile none".
`)
	p.StringVar(&receiver.BinaryLogfileName, "blf", "off", "Short for binaryLogfile")
}

func flagLogfile(p *flag.FlagSet) {
	p.StringVar(&LogfileName, "logfile", "off", `Append all output to logfile. Options are: 'off|none|filename|auto':
"off": no logfile (same as "none")
"none": no logfile (same as "off")
"my/path/auto": Use as logfile name "my/path/2006-01-02_1504-05_trice.log" with actual time. "my/path/" must exist.
"filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
All trice output of the appropriate subcommands is appended per default into the logfile additionally to the normal output.
Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
`)
	p.StringVar(&LogfileName, "lf", "off", "Short for logfile")
}

func flagSrcs(p *flag.FlagSet) {
	p.Var(&id.Srcs, "src", `Source dir or file, It has one parameter. Not usable in the form "-src *.c".
This is a multi-flag switch. It can be used several times for directories and also for files. 
Example: "trice `+p.Name()+` -dry-run -v -src ./_test/ -src pkg/src/trice.h" will scan all C|C++ header and 
source code files inside directory ./_test and scan also file trice.h inside pkg/src directory. 
Without the "-dry-run" switch it would create|extend a list file til.json in the current directory.
 (default "./")`) // multi flag
	p.Var(&id.Srcs, "s", "Short for src.") // multi flag
}

func flagTriceIDRange(p *flag.FlagSet) {
	p.Var(&id.IDRange, "IDRange", `This allows tag specific routing in the target code. 
This switch has one parameter string and is a multi-flag switch. It can be used for each Trice tag. Example:
Assign error tag Trice IDs in the range 10-99 and msg tag IDs in the range 100-199:
"trice `+p.Name()+` -IDRange err:10,99 -IDRange msg:100,999" (overlapping ID ranges are forbidden)
All other trice tags get IDs from the -IDMin, -IDMax range. The used -IDMethod is the same for all tags. 
For example you can have all trice messages in direct mode over RTT but err & msg tagged trices additionally
in deferred mode over a serial port and, if you need, store all error tagged Trice logs additionally in the Flash memory.
You need to configure the target code in your triceConfig.h accordingly (search trice code for MIN_ID): 
Use "#define TRICE_UARTA_MIN_ID 10" and "#define TRICE_UARTA_MAX_ID 999" for example. (default "")`) // multi flag
}

func flagDryRun(p *flag.FlagSet) {
	p.BoolVar(&id.DryRun, "dry-run", false, `No changes applied but output shows what would happen.
"trice `+p.Name()+` -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
`+boolInfo) // flag
}

func flagVerbosity(p *flag.FlagSet) {
	p.BoolVar(&Verbose, "verbose", false, `Gives more informal output if used. Can be helpful during setup.
For example "trice log -p JLINK -v" is the same as "trice log -p JLINK" but with more descriptive output what could be helpful during setup.
`+boolInfo) // flag
	p.BoolVar(&Verbose, "v", false, "short for verbose") // flag
}

func flagSkipAdditionalChecks(p *flag.FlagSet) {
	p.BoolVar(&id.SkipAdditionalChecks, "skipAdditionalChecks", false, `No parameter count checks, when this flag is true.
This reduces the processing time by a few percent but does not detect wrong parameter counts, anyway the compiler would complain.
Add this flag for script speed-up, when not editing the sources. Alternatively consider the -cache switch.
`+boolInfo) // flag
	p.BoolVar(&id.SkipAdditionalChecks, "skip", false, "short for skipAdditionalChecks") // flag
}

func flagIDList(p *flag.FlagSet) {
	p.StringVar(&id.FnJSON, "idlist", id.FnJSON, `The trice ID list file.
The specified JSON file is needed to display the ID coded trices during runtime and should be under version control.
`) // flag
	p.StringVar(&id.FnJSON, "til", id.FnJSON, `Short for '-idlist'.
`) // flag
	p.StringVar(&id.FnJSON, "i", id.FnJSON, `Short for '-idlist'.
`) // flag
}

func flagLIList(p *flag.FlagSet) {
	p.StringVar(&id.LIFnJSON, "locationInformation", "li.json", `The trice location information file.
The specified JSON file is needed to display the location information for each ID during runtime. 
It is regenerated on each add, clean, or insert trice run. When trice log finds a location information file, it is used for 
log output with location information. Otherwise no location information is displayed, what usually is wanted in the field.
This way the newest til.json can be used also with legacy firmware, but the li.json must match the current firmware version.
With "off" or "none" suppress the display or generation of the location information. See -tLocFmt for formatting.
`) // flag
	p.StringVar(&id.LIFnJSON, "li", "li.json", `Short for '-locationInformation'.
`) // flag
	p.BoolVar(&id.LiPathIsRelative, "liPathIsRelative", false, `Use this flag, if your project has trices inside files with identical names in different folders to distinguish them in the location information.
The default is to use only the files basename.`)
}

func flagIPAddress(p *flag.FlagSet) {
	p.StringVar(&emitter.IPAddr, "ipa", "localhost", `IP address like '127.0.0.1'.
You can specify this switch if you intend to use the remote display option to show the output on a different PC in the network.
`) // flag

	p.StringVar(&emitter.IPPort, "ipp", "61497", `16 bit IP port number.
You can specify this switch if you want to change the used port number for the remote display functionality.
`) // flag
}
