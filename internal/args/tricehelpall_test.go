// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"testing"

	"github.com/rokath/trice/internal/id"
)

func TestHelpAll(t *testing.T) {
	input := []string{"trice", "help", "-all"}
	expect := `syntax: 'trice sub-command' [params]
sub-command 'ds|displayServer': Starts a display server. 
#	Use in a separate console. On Windows use wt (https://github.com/microsoft/terminal) or a linux shell like git-bash to avoid ANSI color issues. 
#	Running "trice ds" inside a console opens a display server to be used for displaying the TRICE logs remotely.
#	Several instances of 'trice l -ds -port ...' (for different ports) will send output there in parallel.
#	Example: 'trice ds': Start display server.
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
  -lf string
    	Short for logfile (default "off")
  -logfile string
    	Append all output to logfile. Options are: 'off|none|filename|auto':
    	"off": no logfile (same as "none")
    	"none": no logfile (same as "off")
    	"my/path/auto": Use as logfile name "my/path/2006-01-02_1504-05_trice.log" with actual time. "my/path/" must exist.
    	"filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
    	All trice output of the appropriate subcommands is appended per default into the logfile additionally to the normal output.
    	Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
    	 (default "off")
sub-command 'h|help': For command line usage.
#	"trice h" will print this help text as a whole.
#	Example 'trice h': Print short help.
#	Example 'trice h -all': Print all help.
#	Example 'trice h -log': Print log help.
  -a	Show a|add specific help.
  -add
    	Show a|add specific help.
  -all
    	Show all help.
  -c	Show c|clean specific help.
  -clean
    	Show c|clean specific help.
  -displayserver
    	Show ds|displayserver specific help.
  -ds
    	Show ds|displayserver specific help.
  -generate
    	Show generate specific help.
  -h	Show h|help specific help.
  -help
    	Show h|help specific help.
  -i	Show i|insert specific help.
  -insert
    	Show i|insert specific help.
  -l	Show l|log specific help.
  -lf string
    	Short for logfile (default "off")
  -log
    	Show l|log specific help.
  -logfile string
    	Append all output to logfile. Options are: 'off|none|filename|auto':
    	"off": no logfile (same as "none")
    	"none": no logfile (same as "off")
    	"my/path/auto": Use as logfile name "my/path/2006-01-02_1504-05_trice.log" with actual time. "my/path/" must exist.
    	"filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
    	All trice output of the appropriate subcommands is appended per default into the logfile additionally to the normal output.
    	Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
    	 (default "off")
  -s	Show s|scan specific help.
  -scan
    	Show s|scan specific help.
  -sd
    	Show sd|shutdown specific help.
  -shutdown
    	Show sd|shutdown specific help.
  -v	short for verbose
  -ver
    	Show ver|version specific help.
  -verbose
    	Gives more informal output if used. Can be helpful during setup.
    	For example "trice log -p JLINK -v" is the same as "trice log -p JLINK" but with more descriptive output what could be helpful during setup.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -version
    	Show ver|version specific help.
sub-command 'l|log': For displaying trice logs coming from port. With "trice log" the trice tool display mode is activated.
#	Example: 'trice l -p COM15 -baud 38400': Display trice log messages from serial port COM15
#	Example: 'trice l': Display flexL data format trice log messages from default source J-LINK over Segger RTT protocol.
#	Example: 'trice l -port ST-LINK -v -s': Shows verbose version information and also the received raw bytes.
  -addNL
    	Add a newline char at trice messages end to use for example "hi" instead of "hi\n" in source code.
  -args string
    	Use to pass port specific parameters. The "default" value depends on the used port:
    	port "COMn": default="-baud 115200", Unused option for a different driver. (For baud rate settings see -baud.)
    	port "J-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000", 
    		The -RTTSearchRanges "..." need to be written without "" and with _ instead of space.
    		For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
    	port "ST-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000", 
    		The -RTTSearchRanges "..." need to be written without "" and with _ instead of space.
    		For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
    	port "FILE": default="trices.raw", Option for args is any file name for binary log data like written []byte{115, 111, 109, 101, 10}. Trice retries on EOF.
    	port "FILEBUFFER": default="trices.raw", Option for args is any file name for binary log data like written []byte{115, 111, 109, 101, 10}. Trice stops on EOF.
    	port "TCP4": default="localhost:17001", use any IPv4 IP:port endpoint like "127.0.0.1:19021". This port is usable for reading, when the Trice logs go into a TCP server.
    	port "TCP4BUFFER": default="localhost:17001". This port is used for "-port TCP4" testing, to shutdown the Trice tool automatically.
    	port "UDP4": default="0.0.0.0:17005", use any IPv4 IP:port endpoint to listen on, like 192.168.1.1:17005".
    	port "DEC" or "BUFFER": default="0 0 0 0", Option for args is any space separated decimal number byte sequence. Example -p BUFFER -args "7 123 44".
    	port "HEX" or "DUMP": default="", Option for args is any space or comma separated byte sequence in hex. Example: -p DUMP -args "7B 1A ee,88, 5a".
    	 (default "default")
  -ban value
    	Channel(s) to ignore. This is a multi-flag switch. It can be used several times with a colon separated list of channel descriptors not to display.
    	Example: "-ban dbg:wrn -ban diag" results in suppressing all as debug, diag and warning tagged messages. Not usable in conjunction with "-pick". See also "-logLevel".
  -baud int
    	Set the serial port baudrate.
    	It is the only setup parameter. The other values default to 8N1 (8 data bits, no parity, one stopbit).
    	 (default 115200)
  -binaryLogfile string
    	Append all output to logfile. Options are: 'off|none|filename|auto':
    	"off": no binary logfile (same as "none")
    	"none": no binary logfile (same as "off")
    	"my/path/auto": Use as binary logfile name "my/path/2006-01-02_1504-05_trice.bin" with actual time. "my/path/" must exist.
    	"filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
    	All trice output of the appropriate subcommands is appended per default into the logfile additionally to the normal output.
    	Change the filename with "-binaryLogfile myName.bin" or switch logging off with "-binaryLogfile none".
    	 (default "off")
  -blf string
    	Short for binaryLogfile (default "off")
  -color string
    	The format strings can start with a lower or upper case channel information.
    	See https://github.com/rokath/trice/blob/master/pkg/src/triceCheck.c for examples. Color options: 
    	"off": Disable ANSI color. The lower case channel information is kept: "w:x"-> "w:x" 
    	"none": Disable ANSI color. The lower case channel information is removed: "w:x"-> "x"
    	"default|color": Use ANSI color codes for known upper and lower case channel info are inserted and lower case channel information is removed.
    	 (default "default")
  -d16
    	Short for '-Doubled16BitID'.
  -databits int
    	Set the serial port databits, options: 7, 9 (default 8)
  -dc int
    	Dumped bytes per line when "-encoding DUMP" (default 32)
  -debug
    	Show additional debug information
  -defaultTRICEBitwidth string
    	The expected value bit width for TRICE macros. Options: 8, 16, 32, 64. Must be in sync with the 'TRICE_DEFAULT_PARAMETER_BIT_WIDTH' setting inside triceConfig.h (default "32")
  -displayserver
    	Send trice lines to displayserver @ ipa:ipp.
    	Example: "trice l -port COM38 -ds -ipa 192.168.178.44" sends trice output to a previously started display server in the same network.
  -doubled16BitID
    	Tells, that 16-bit IDs are doubled. That switch is needed when un-routed direct output is used like (TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1), but also with double buffer in (TRICE_DEFERRED_TRANSFER_MODE==TRICE_MULTI_PACK_MODE) and XTEA encryption. Read the user guide for more details.
  -ds
    	Short for '-displayserver'.
  -e string
    	Short for -encoding. (default "TREX")
  -encoding string
    	The trice transmit data format type, options: '(CHAR|DUMP|TREX)'. Target device encoding must match. 
    			  TREX=TriceExtendableEncoding, see Trice1.0Specification. Needs '#define TRICE_ENCODING TRICE_TREX_ENCODING' inside triceConfig.h.
    			  CHAR prints the received bytes as characters.
    			  DUMP prints the received bytes as hex code (see switch -dc too).
    	 (default "TREX")
  -exec string
    	Use to pass an additional command line for port TCP4 (like gdbserver start).
  -hs string
    	PC timestamp for logs and logfile name, options: 'off|none|UTCmicro|zero'
    	This timestamp switch generates the timestamps on the PC only (reception time), what is good enough for many cases.
    	"LOCmicro" means local time with microseconds. "UTCmicro" shows timestamps in universal time. When set to "off" no PC timestamps displayed. (default "LOCmicro")
  -i string
    	Short for '-idlist'.
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
  -lf string
    	Short for logfile (default "off")
  -li string
    	Short for '-locationInformation'.
    	 (default "li.json")
  -liFmt string
    	Target location format string at start of each line, if target location existent (configured). Use "off" or "none" to suppress existing target location. If several trices form a log line only the location of first trice ist displayed. (default "info:%21s %5d ")
  -liPath string
    	How to store location information paths inside li.json: base, relative, path/relative or full. When relative is given, the location information is stored relative to the current location. If a path is given additionally, like ../../relative the path is added to the location information.
    	 (default "base")
  -locationInformation string
    	The trice location information file.
    	The specified JSON file is needed to display the location information for each ID during runtime. 
    	It is regenerated on each add, clean, or insert trice run. When trice log finds a location information file, it is used for 
    	log output with location information. Otherwise no location information is displayed, what usually is wanted in the field.
    	This way the newest til.json can be used also with legacy firmware, but the li.json must match the current firmware version.
    	With "off" or "none" suppress the display or generation of the location information. See -tLocFmt for formatting.
    	 (default "li.json")
  -logLevel string
    	Level based log filtering. "off" suppresses everything. If equal to a channel specifier all with a bigger index inside emitter.ColorChannels the log not shown. 
    	A typical use case is "-logLevel wrn". Attention this switch influences also location information (-liFmt), target stamps (-ts0, -ts16, -ts32), prefix and suffix information. Set these channel information appropriate. 
    	Logs without channel specifier are not suppressed. Using an invalid value like "x" suppresses all logs with a channel specifier. See also CLI switches -pick and -ban. (default "all")
  -logfile string
    	Append all output to logfile. Options are: 'off|none|filename|auto':
    	"off": no logfile (same as "none")
    	"none": no logfile (same as "off")
    	"my/path/auto": Use as logfile name "my/path/2006-01-02_1504-05_trice.log" with actual time. "my/path/" must exist.
    	"filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
    	All trice output of the appropriate subcommands is appended per default into the logfile additionally to the normal output.
    	Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
    	 (default "off")
  -newlineIndent int
    	Force newline offset for trice format strings with line breaks before end. -1=auto sense (default -1)
  -noCycle
    	Disables reporting of cycle errors.
  -p string
    	short for -port (default "J-LINK")
  -packageFraming string
    	Use "none" or "COBS" as alternative. "COBS" needs "#define TRICE_FRAMING TRICE_FRAMING_COBS" inside "triceConfig.h". (default "TCOBSv1")
  -parity string
    	Serial port bit parity value, options: odd, even (default "none")
  -password string
    	The decrypt passphrase. If you change this value you need to compile the target with the appropriate key (see -showKeys).
    	Encryption is recommended if you deliver firmware to customers and want protect the trice log output. This does work right now only with flex and flexL format.
  -pf string
    	Short for '-packageFraming'. (default "TCOBSv1")
  -pick value
    	Channel(s) to display. This is a multi-flag switch. It can be used several times with a colon separated list of channel descriptors only to display.
    	Example: "-pick err:wrn -pick default" results in suppressing all messages despite of as error, warning and default tagged messages. Not usable in conjunction with "-ban". See also "-logLevel".
  -port string
    	Case insensitive receiver device name: 'serial name|JLINK|STLINK|FILE|FILEBUFFER|TCP4|TCP4BUFFER|DEC|BUFFER|HEX|DUMP. 
    	The serial name is like 'COM12' for Windows or a Linux name like '/dev/tty/usb12'. 
    	Using a virtual serial COM port on the PC over a FTDI USB adapter is a most likely variant.
    	 (default "J-LINK")
  -prefix string
    	Line prefix, options: any string or 'off|none' or 'source:' followed by 0-12 spaces, 'source:' will be replaced by source value e.g., 'COM17:'. (default "source: ")
  -pw string
    	Short for -password.
  -s	Short for '-showInputBytes'.
  -showID string
    	Format string for displaying first trice ID at start of each line. Example: "debug:%7d ". Default is "". If several trices form a log line only the first trice ID ist displayed.
  -showInputBytes
    	Show incoming bytes, what can be helpful during setup.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -showKey
    	Show encryption key. Use this switch for creating your own password keys. If applied together with "-password MySecret" it shows the encryption key.
    	Simply copy this key than into the line "#define ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret" inside triceConfig.h.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -singleFraming
    	singleFraming demands, that each received package contains not more than a singe Trice message. This is the case for all direct modes or if you "#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE" in your triceConfig.h file. Applying this switch makes sense with COBS and TCOBS framing to improve Trice tool error recognition in received binary data. Not usable together with encryption because of the padding bytes.
  -stat
    	Print complete statistics on exit.
  -stopbits string
    	Serial port stopbit, options: 1.5, 2 (default "1")
  -suffix string
    	Append suffix to all lines, options: any string.
  -tagStat
    	Print Trices occurrences count on exit.
  -tcp string
    	TCP address for an external log receiver like Putty. Example: 1st: "trice log -p COM1 -tcp localhost:64000", 2nd "putty". In "Terminal" enable "Implicit CR in every LF", In "Session" Connection type:"Other:Telnet", specify "hostname:port" here like "localhost:64000".
  -testTable
    	Generate testTable output and ignore -prefix, -suffix, -ts, -color. This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -til string
    	Short for '-idlist'.
    	 (default "til.json")
  -triceEndianness string
    	Target endianness trice data stream. Option: "bigEndian". (default "littleEndian")
  -triceStat
    	Print Trices occurrences count on exit.
  -ts string
    	Target timestamp general format string at start of each line, if target timestamps existent (configured). Choose between "µs" (or "us") and "ms", use "" or 'off' or 'none' to suppress existing target timestamps. Sets ts0, ts16, ts32 if these not passed. If several trices form a log line only the timestamp of first trice ist displayed. (default "µs")
  -ts0 string
    	Target stamp format string at start of each line, if no target stamps existent (configured). Use "" to suppress existing target timestamps. If several trices form a log line only the timestamp of first trice ist displayed. (default "time:            ")
  -ts16 string
    	16-bit Target stamp format string at start of each line, if 16-bit target stamps existent (configured). Choose between "µs" (or "us") and "ms", use "" to suppress or use s.th. like "...%d...". If several trices form a log line only the timestamp of first trice ist displayed. (default "ms")
  -ts32 string
    	32-bit Target stamp format string at start of each line, if 32-bit target stamps existent (configured). Choose between "µs" (or "us"), "ms", "epoch060102150405" like, use "" to suppress or use s.th. like "...%d...". If several trices form a log line only the timestamp of first trice ist displayed. (default "ms")
  -u	Short for '-unsigned'. (default true)
  -unsigned
    	Hex, Octal and Bin values are printed as unsigned values. For signed output use -unsigned=false (default true)
  -v	short for verbose
  -verbose
    	Gives more informal output if used. Can be helpful during setup.
    	For example "trice log -p JLINK -v" is the same as "trice log -p JLINK" but with more descriptive output what could be helpful during setup.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
sub-command 'a|add': Use for adding library source files containing already trice IDs to your project.
#	It extends the ID list from these source files but does not change the source files, even if they contain Trices without IDs.
#	Already used IDs are reported, so you have the chance to remove them from til.json and your project and then do "trice add" again.
#	This way you can make sure to get the new sources unchanged in your list, even they contain trice IDs already.
#	Afterwards the newly added library source files are treated as regular project files. If you do not edit them, they stay unchanged.
#	If in one of your project files a trice from the library occurs again, like "trice( iD(123), "Hi!");" in a lib file and "trice( "Hi!");"
#	in a project file, it will get a different ID in your project file because of the used location information.
#	The "add" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
#	Example: 'trice add': Update ID list from source tree.
  -a value
    	Short for trice() aliases.
  -alias value
    	Additional macro names to treat like trice().
    	This flag can be specified multiple times to add more aliases. Each provided name will be
    	recognized as equivalent to a trice() call. (default none)
    	 (default none)
  -dry-run
    	No changes applied but output shows what would happen.
    	"trice add -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -e value
    	Short for exclude.
  -exclude value
    	Exclude dir or file, It has one parameter. Not usable in the form "-exclude *.c".
    	This is a multi-flag switch. It can be used several times for directories and also for files.
    	Example: "trice add -v -src ./_test/ -exclude _test/src/trice.h" will scan all C|C++ header and
    	source code files inside directory ./_test EXCEPT file trice.h inside _test/src directory.
    	 (default none)
  -i string
    	Short for '-idlist'.
    	 (default "til.json")
  -idlist string
    	The trice ID list file.
    	The specified JSON file is needed to display the ID coded trices during runtime and should be under version control.
    	 (default "til.json")
  -li string
    	Short for '-locationInformation'.
    	 (default "li.json")
  -liPath string
    	How to store location information paths inside li.json: base, relative, path/relative or full. When relative is given, the location information is stored relative to the current location. If a path is given additionally, like ../../relative the path is added to the location information.
    	 (default "base")
  -locationInformation string
    	The trice location information file.
    	The specified JSON file is needed to display the location information for each ID during runtime. 
    	It is regenerated on each add, clean, or insert trice run. When trice log finds a location information file, it is used for 
    	log output with location information. Otherwise no location information is displayed, what usually is wanted in the field.
    	This way the newest til.json can be used also with legacy firmware, but the li.json must match the current firmware version.
    	With "off" or "none" suppress the display or generation of the location information. See -tLocFmt for formatting.
    	 (default "li.json")
  -s value
    	Short for src.
  -sa value
    	Short for triceS() aliases.
  -salias value
    	Additional macro names to treat like triceS().
    	This flag can be specified multiple times to add more aliases. Each provided name will be
    	recognized as equivalent to a triceS() call.
    	 (default none)
  -skip
    	short for skipAdditionalChecks
  -skipAdditionalChecks
    	No parameter count checks, when this flag is true.
    	This reduces the processing time by a few percent but does not detect wrong parameter counts, anyway the compiler would complain.
    	Add this flag for script speed-up, when not editing the sources. Alternatively consider the -cache switch.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -src value
    	Source dir or file, It has one parameter. Not usable in the form "-src *.c".
    	This is a multi-flag switch. It can be used several times for directories and also for files. 
    	Example: "trice add -dry-run -v -src ./_test/ -src pkg/src/trice.h" will scan all C|C++ header and 
    	source code files inside directory ./_test and scan also file trice.h inside pkg/src directory. 
    	Without the "-dry-run" switch it would create|extend a list file til.json in the current directory.
    	 (default "./")
  -til string
    	Short for '-idlist'.
    	 (default "til.json")
  -v	short for verbose
  -verbose
    	Gives more informal output if used. Can be helpful during setup.
    	For example "trice log -p JLINK -v" is the same as "trice log -p JLINK" but with more descriptive output what could be helpful during setup.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
sub-command 'g|gen|generate': Use for creating colors or support files.
  -colors
    	Write all possible colors.
  -i string
    	Short for '-idlist'.
    	 (default "til.json")
  -idlist string
    	The trice ID list file.
    	The specified JSON file is needed to display the ID coded trices during runtime and should be under version control.
    	 (default "til.json")
  -rpcC
    	Create a til_rpc.c file.
  -rpcH
    	Create a til_rpc.h file.
  -til string
    	Short for '-idlist'.
    	 (default "til.json")
  -tilC
    	Create a til.c file.
  -tilCS
    	Create a til.cs file.
  -tilH
    	Create a til.h file.
  -v	short for verbose
  -verbose
    	Gives more informal output if used. Can be helpful during setup.
    	For example "trice log -p JLINK -v" is the same as "trice log -p JLINK" but with more descriptive output what could be helpful during setup.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
sub-command 's|scan': Shows available serial ports)
#	Example: 'trice s': Show COM ports.
sub-command 'sd|shutdown': Ends display server at IPA:IPP, works also on a remote machine.
#	Example: 'trice sd': Shut down remote display server.
  -ipa string
    	IP address like '127.0.0.1'.
    	You can specify this switch if you intend to use the remote display option to show the output on a different PC in the network.
    	 (default "localhost")
  -ipp string
    	16 bit IP port number.
    	You can specify this switch if you want to change the used port number for the remote display functionality.
    	 (default "61497")
sub-command 'ver|version': For displaying version information.
	"trice v" will print the version information. If trice is not versioned the build time will be displayed instead.
  -lf string
    	Short for logfile (default "off")
  -logfile string
    	Append all output to logfile. Options are: 'off|none|filename|auto':
    	"off": no logfile (same as "none")
    	"none": no logfile (same as "off")
    	"my/path/auto": Use as logfile name "my/path/2006-01-02_1504-05_trice.log" with actual time. "my/path/" must exist.
    	"filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
    	All trice output of the appropriate subcommands is appended per default into the logfile additionally to the normal output.
    	Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
    	 (default "off")
  -v	short for verbose
  -verbose
    	Gives more informal output if used. Can be helpful during setup.
    	For example "trice log -p JLINK -v" is the same as "trice log -p JLINK" but with more descriptive output what could be helpful during setup.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
sub-command 'i|insert': For updating til.json and inserting IDs into source files.
#	"trice insert" will parse source tree(s) for new or changed TRICE macros, modify them appropriate and extend the JSON list.
#	This command relies on existing til.json and li.json files. The li.json file is used as reference and generated new during insert.
#	Without li.json the insert command assigns new IDs to all found trice macros, because it cannot assign files to them, and extends 
#	the til.json file accordingly. That is for safety, because the insert command acts in multiple files in Go routines parallel. 
#	If you lost the li.json file, you can run "trice add" first, to re-generate a new li.json and then execute "trice insert".
#	With an empty til.json file, the insert command re-creates a new til.json (and a fresh li.json) from the source code.
#	If the insert command finds an ID for a trice inside the sources, used already for an other trice inside til.json, it reports that
#	and assigns a new ID into the source file, adding it to til.json as well.
#	The "insert" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
#	The switch "-src" is optional (default is "./") and a multi-flag here. So you can use the "-src" flag several times.
#	Example: 'trice i -src ../A -src ../../B': Parse ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json
  -IDMax value
    	Upper end of ID range for normal trices. (default 7999)
  -IDMethod string
    	Search method for new ID's in range- Options are 'upward', 'downward' & 'random'. (default "random")
  -IDMin value
    	Lower end of ID range for normal trices. (default 1000)
  -IDRange value
    	This allows tag specific routing in the target code. 
    	This switch has one parameter string and is a multi-flag switch. It can be used for each Trice tag. Example:
    	Assign error tag Trice IDs in the range 10-99 and msg tag IDs in the range 100-199:
    	"trice insert -IDRange err:10,99 -IDRange msg:100,999" (overlapping ID ranges are forbidden)
    	All other trice tags get IDs from the -IDMin, -IDMax range. The used -IDMethod is the same for all tags. 
    	For example you can have all trice messages in direct mode over RTT but err & msg tagged trices additionally
    	in deferred mode over a serial port and, if you need, store all error tagged Trice logs additionally in the Flash memory.
    	You need to configure the target code in your triceConfig.h accordingly (search trice code for MIN_ID): 
    	Use "#define TRICE_UARTA_MIN_ID 10" and "#define TRICE_UARTA_MAX_ID 999" for example. (default "")
  -a value
    	Short for trice() aliases.
  -addParamCount
    	Extend TRICE macro names with the parameter count _n to enable compile time checks.
  -alias value
    	Additional macro names to treat like trice().
    	This flag can be specified multiple times to add more aliases. Each provided name will be
    	recognized as equivalent to a trice() call. (default none)
    	 (default none)
  -cache
    	Use "~/.trice/cache/" for fast ID insert (EXPERIMENTAL!). The folder must exist.
  -defaultStampSize int
    	Default stamp size for written TRICE macros without id(0), Id(0 or ID(0). Valid values are 0, 16 or 32.
  -dry-run
    	No changes applied but output shows what would happen.
    	"trice insert -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -e value
    	Short for exclude.
  -exclude value
    	Exclude dir or file, It has one parameter. Not usable in the form "-exclude *.c".
    	This is a multi-flag switch. It can be used several times for directories and also for files.
    	Example: "trice insert -v -src ./_test/ -exclude _test/src/trice.h" will scan all C|C++ header and
    	source code files inside directory ./_test EXCEPT file trice.h inside _test/src directory.
    	 (default none)
  -i string
    	Short for '-idlist'.
    	 (default "til.json")
  -idlist string
    	The trice ID list file.
    	The specified JSON file is needed to display the ID coded trices during runtime and should be under version control.
    	 (default "til.json")
  -li string
    	Short for '-locationInformation'.
    	 (default "li.json")
  -liPath string
    	How to store location information paths inside li.json: base, relative, path/relative or full. When relative is given, the location information is stored relative to the current location. If a path is given additionally, like ../../relative the path is added to the location information.
    	 (default "base")
  -locationInformation string
    	The trice location information file.
    	The specified JSON file is needed to display the location information for each ID during runtime. 
    	It is regenerated on each add, clean, or insert trice run. When trice log finds a location information file, it is used for 
    	log output with location information. Otherwise no location information is displayed, what usually is wanted in the field.
    	This way the newest til.json can be used also with legacy firmware, but the li.json must match the current firmware version.
    	With "off" or "none" suppress the display or generation of the location information. See -tLocFmt for formatting.
    	 (default "li.json")
  -s value
    	Short for src.
  -sa value
    	Short for triceS() aliases.
  -salias value
    	Additional macro names to treat like triceS().
    	This flag can be specified multiple times to add more aliases. Each provided name will be
    	recognized as equivalent to a triceS() call.
    	 (default none)
  -skip
    	short for skipAdditionalChecks
  -skipAdditionalChecks
    	No parameter count checks, when this flag is true.
    	This reduces the processing time by a few percent but does not detect wrong parameter counts, anyway the compiler would complain.
    	Add this flag for script speed-up, when not editing the sources. Alternatively consider the -cache switch.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -spaceInsideParenthesis trice(<space>iD(<space>123<space>), "...);
    	Add space inside Trice braces: trice(<space>iD(<space>123<space>), "...);. Use this if your default code auto-formatting is with space inside braces.
  -src value
    	Source dir or file, It has one parameter. Not usable in the form "-src *.c".
    	This is a multi-flag switch. It can be used several times for directories and also for files. 
    	Example: "trice insert -dry-run -v -src ./_test/ -src pkg/src/trice.h" will scan all C|C++ header and 
    	source code files inside directory ./_test and scan also file trice.h inside pkg/src directory. 
    	Without the "-dry-run" switch it would create|extend a list file til.json in the current directory.
    	 (default "./")
  -til string
    	Short for '-idlist'.
    	 (default "til.json")
  -v	short for verbose
  -verbose
    	Gives more informal output if used. Can be helpful during setup.
    	For example "trice log -p JLINK -v" is the same as "trice log -p JLINK" but with more descriptive output what could be helpful during setup.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -w	Short for (white)spaceInsideParenthesis or "wide".
sub-command 'c|clean': Set all [id|Id|ID](n) inside source tree dir to [id|Id|ID](0). Remove all iD(n).
#	All in source code found IDs are added to til.json if not already there. Inside til.json differently used IDs are 
#	reported and just cleaned inside the source files. The existing li.json is updated/extended. 
#	The switch "-src" is optional (default is "./") and a multi-flag here. So you can use the "-src" flag several times.
#	Example: 'trice clean -src ../A -src B/x.c': Sets all TRICE IDs to 0, or removes them, in folder ../A. and file B/x.c
#	EXPERIMENTAL! The command itself works reliable, but a sophisticated editor will detect inconsistencies with removed IDs.
#	EXPERIMENTAL! With "#define TRICE_CLEAN 1" inside "triceConfig.h" these displayed "errors" are suppressed.
#	EXPERIMENTAL! All files including trice.h are re-compiled then on the next compiler run, what could be time-consuming.
#	In difference to "trice zero", Trice function calls get iD(n) removed. Example: "TRice( iD(88), "hi);" -> "TRice("hi);"
  -a value
    	Short for trice() aliases.
  -alias value
    	Additional macro names to treat like trice().
    	This flag can be specified multiple times to add more aliases. Each provided name will be
    	recognized as equivalent to a trice() call. (default none)
    	 (default none)
  -cache
    	Use "~/.trice/cache/" for fast ID clean (EXPERIMENTAL!). The folder must exist.
  -dry-run
    	No changes applied but output shows what would happen.
    	"trice clean -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -e value
    	Short for exclude.
  -exclude value
    	Exclude dir or file, It has one parameter. Not usable in the form "-exclude *.c".
    	This is a multi-flag switch. It can be used several times for directories and also for files.
    	Example: "trice clean -v -src ./_test/ -exclude _test/src/trice.h" will scan all C|C++ header and
    	source code files inside directory ./_test EXCEPT file trice.h inside _test/src directory.
    	 (default none)
  -i string
    	Short for '-idlist'.
    	 (default "til.json")
  -idlist string
    	The trice ID list file.
    	The specified JSON file is needed to display the ID coded trices during runtime and should be under version control.
    	 (default "til.json")
  -li string
    	Short for '-locationInformation'.
    	 (default "li.json")
  -liPath string
    	How to store location information paths inside li.json: base, relative, path/relative or full. When relative is given, the location information is stored relative to the current location. If a path is given additionally, like ../../relative the path is added to the location information.
    	 (default "base")
  -locationInformation string
    	The trice location information file.
    	The specified JSON file is needed to display the location information for each ID during runtime. 
    	It is regenerated on each add, clean, or insert trice run. When trice log finds a location information file, it is used for 
    	log output with location information. Otherwise no location information is displayed, what usually is wanted in the field.
    	This way the newest til.json can be used also with legacy firmware, but the li.json must match the current firmware version.
    	With "off" or "none" suppress the display or generation of the location information. See -tLocFmt for formatting.
    	 (default "li.json")
  -s value
    	Short for src.
  -sa value
    	Short for triceS() aliases.
  -salias value
    	Additional macro names to treat like triceS().
    	This flag can be specified multiple times to add more aliases. Each provided name will be
    	recognized as equivalent to a triceS() call.
    	 (default none)
  -skip
    	short for skipAdditionalChecks
  -skipAdditionalChecks
    	No parameter count checks, when this flag is true.
    	This reduces the processing time by a few percent but does not detect wrong parameter counts, anyway the compiler would complain.
    	Add this flag for script speed-up, when not editing the sources. Alternatively consider the -cache switch.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -spaceInsideParenthesis trice(<space>"...)
    	Add space after Trice opening brace: trice(<space>"...). Use this if your default code auto-formatting is with space after opening brace.
  -src value
    	Source dir or file, It has one parameter. Not usable in the form "-src *.c".
    	This is a multi-flag switch. It can be used several times for directories and also for files. 
    	Example: "trice clean -dry-run -v -src ./_test/ -src pkg/src/trice.h" will scan all C|C++ header and 
    	source code files inside directory ./_test and scan also file trice.h inside pkg/src directory. 
    	Without the "-dry-run" switch it would create|extend a list file til.json in the current directory.
    	 (default "./")
  -til string
    	Short for '-idlist'.
    	 (default "til.json")
  -v	short for verbose
  -verbose
    	Gives more informal output if used. Can be helpful during setup.
    	For example "trice log -p JLINK -v" is the same as "trice log -p JLINK" but with more descriptive output what could be helpful during setup.
    	This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
  -w	Short for (white)spaceInsideParenthesis or "wide".
`
	id.FnJSON = "til.json"
	execHelper(t, input, expect)
}
