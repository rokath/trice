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
      Use in a separate console. On Windows use wt (https://github.com/microsoft/terminal) or a linux shell like git-bash to avoid ANSI color issues.
      Running "trice ds" inside a console opens a display server to be used for displaying the TRICE logs remotely.
      Several instances of 'trice l -ds -port ...' (for different ports) will send output there in parallel.
      example: 'trice ds': Start display server.
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
              "auto": Use as logfile name "2006-01-02_1504-05_trice.log" with actual time.
              "filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
              All trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.
              Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
               (default "off")
      sub-command 'h|help': For command line usage.
      "trice h" will print this help text as a whole.
      example 'trice h': Print short help.
      example 'trice h -all': Print all help.
      example 'trice h -log': Print log help.
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
        -lf string
              Short for logfile (default "off")
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
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
        -version
              Show ver|version specific help.
        -z    Show zeroSourceTreeIds specific help.
        -zeroSourceTreeIds
              Show zeroSourceTreeIds specific help.
      sub-command 'l|log': For displaying trice logs coming from port. With "trice log" the trice tool display mode is activated.
      example: 'trice l -p COM15 -baud 38400': Display trice log messages from serial port COM15
      example: 'trice l': Display flexL data format trice log messages from default source J-LINK over Segger RTT protocol.
      example: 'trice l -port ST-LINK -v -s': Shows verbose version information and also the received raw bytes.
        -args string
              Use to pass port specific parameters. The "default" value depends on the used port:
              port "BUFFER": default="0 0 0 0", Option for args is any space separated decimal number byte sequence.
              port "DUMP": default="", Option for args is any space or comma separated byte sequence.
              port "COMn": default="", Unused option for a different driver. (For baud rate settings see -baud.)
              port "FILE": default="trices.raw", Option for args is any file name. Trice retries on EOF.
              port "FILEBUFFER": default="trices.raw", Option for args is any file name. Trice stops on EOF.
              port "J-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000",
                      The -RTTSearchRanges "..." need to be written without "" and with _ instead of space.
                      For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
              port "ST-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000",
                      The -RTTSearchRanges "..." need to be written without "" and with _ instead of space.
                      For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
              port "TCP4": default="localhost:17001", use any IP:port endpoint like "127.0.0.1:19021"
               (default "default")
        -ban value
              Channel(s) to ignore. This is a multi-flag switch. It can be used several times with a colon separated list of channel descriptors not to display.
              Example: "-ban dbg:wrn -ban diag" results in suppressing all as debug, diag and warning tagged messages. Not usable in conjunction with "-pick".
        -baud int
              Set the serial port baudrate.
              It is the only setup parameter. The other values default to 8N1 (8 data bits, no parity, one stopbit).
               (default 115200)
        -binaryLogfile string
              Append all output to logfile. Options are: 'off|none|filename|auto':
              "off": no binary logfile (same as "none")
              "none": no binary logfile (same as "off")
              "auto": Use as binary logfile name "2006-01-02_1504-05_trice.bin" with actual time.
              "filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
              All trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.
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
        -ds
              Short for '-displayserver'.
        -e string
              Short for -encoding. (default "TREX")
        -encoding string
              The trice transmit data format type, options: '(CHAR|DUMP|TLE|TREX)'. Target device encoding must match.
                                TLE=TriceLegacyEncoding expects 0-delimited COBS byte sequences. Needs '#define TRICE_ENCODING TRICE_LEGACY_ENCODING' inside triceConfig.h. Use not for new projects.
                                TREX=TriceExtendableEncoding, see Trice1.0Specification. Needs '#define TRICE_ENCODING TRICE_TREX_ENCODING' inside triceConfig.h.
                                CHAR prints the received bytes as characters.
                                COBS = TLE (obsolete naming)
                                DUMP prints the received bytes as hex code (see switch -dc too).
               (default "TREX")
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
        -lf string
              Short for logfile (default "off")
        -li string
              Short for '-locationInformation'.
               (default "li.json")
        -liFmt string
              Target location format string at start of each line, if target location existent (configured). Use "off" or "none" to suppress existing target location. If several trices form a log line only the location of first trice ist displayed. (default "info:%20s:%4d ")
        -locationInformation string
              The trice location list file.
              The specified JSON file is needed to display the location information for each ID during runtime and needs no version control.
              It is regenerated on each refresh, update or renew trice run. When trice log finds a location information file, it is used for
              log output with location information. Otherwise no location information is displayed, what usually is wanted in the field.
              This way the newest til.json can be used also with legacy firmware, but the li.json must match the current firmware version.
              "off" or "none" suppress the display of the location information even a li.json file exists. Avoid shared ID's for correct
              location information. See information for the -SharedIDs switch for additionals hints. See -tLocFmt for formatting.
               (default "li.json")
        -logLevel string
              Level based log filtering. "off" suppresses everything. If equal to a channel specifier all with a bigger index inside emitter.ColorChannels is not shown. (default "all")
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
              Example: "-pick err:wrn -pick default" results in suppressing all messages despite of as error, warning and default tagged messages. Not usable in conjunction with "-ban".
        -port string
              receiver device: 'BUFFER|DUMP|FILE|FILEBUFFER|JLINK|STLINK|TCP4|serial name.
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
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
        -showKey
              Show encryption key. Use this switch for creating your own password keys. If applied together with "-password MySecret" it shows the encryption key.
              Simply copy this key than into the line "#define ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret" inside triceConfig.h.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
        -stopbits string
              Serial port stopbit, options: 1.5, 2 (default "1")
        -suffix string
              Append suffix to all lines, options: any string.
        -tcp string
              TCP address for an external log receiver like Putty. Example: 1st: "trice log -p COM1 -tcp localhost:64000", 2nd "putty". In "Terminal" enable "Implicit CR in every LF", In "Session" Connection type:"Other:Telnet", specify "hostname:port" here like "localhost:64000".
        -testTable
              Generate testTable output and ignore -prefix, -suffix, -ts, -color. This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
        -til string
              Short for '-idlist'.
               (default "til.json")
        -triceEndianness string
              Target endianness trice data stream. Option: "bigEndian". (default "littleEndian")
        -ts string
              PC timestamp for logs and logfile name, options: 'off|none|UTCmicro|zero'
              This timestamp switch generates the timestamps on the PC only (reception time), what is good enough for many cases.
              "LOCmicro" means local time with microseconds. "UTCmicro" shows timestamps in universal time. When set to "off" no PC timestamps displayed. (default "LOCmicro")
        -tsf string
              Target timestamp general format string at start of each line, if target timestamps existent (configured). Choose between "µs" or "us" and "ms", use "" to suppress existing target timestamps. Sets tsf0, tsf16, tsf32 if these not passed. If several trices form a log line only the timestamp of first trice ist displayed. (default "ms")
        -tsf0 string
              Target stamp format string at start of each line, if no target stamps existent (configured). Use "" to suppress existing target timestamps. If several trices form a log line only the timestamp of first trice ist displayed. (default "             ")
        -tsf16 string
              16-bit Target stamp format string at start of each line, if 16-bit target stamps existent (configured). Choose between "µs" or "us" and "ms", use "" to suppress or use s.th. like "...%d...". If several trices form a log line only the timestamp of first trice ist displayed. (default "ms")
        -tsf32 string
              32-bit Target stamp format string at start of each line, if 32-bit target stamps existent (configured). Choose between "µs" or "us" and "ms", use "" to suppress or use s.th. like "...%d...". If several trices form a log line only the timestamp of first trice ist displayed. (default "ms")
        -u    Short for '-unsigned'. (default true)
        -unsigned
              Hex, Octal and Bin values are printed as unsigned values. (default true)
        -v    short for verbose
        -verbose
              Gives more informal output if used. Can be helpful during setup.
              For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
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
      example: 'trice refresh': Update ID list from source tree.
        -dry-run
              No changes applied but output shows what would happen.
              "trice refresh -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
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
        -li string
              Short for '-locationInformation'.
               (default "li.json")
        -locationInformation string
              The trice location list file.
              The specified JSON file is needed to display the location information for each ID during runtime and needs no version control.
              It is regenerated on each refresh, update or renew trice run. When trice log finds a location information file, it is used for
              log output with location information. Otherwise no location information is displayed, what usually is wanted in the field.
              This way the newest til.json can be used also with legacy firmware, but the li.json must match the current firmware version.
              "off" or "none" suppress the display of the location information even a li.json file exists. Avoid shared ID's for correct
              location information. See information for the -SharedIDs switch for additionals hints. See -tLocFmt for formatting.
               (default "li.json")
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
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
      sub-command 'renew': It is like refresh, but til.json is cleared first, so all 'old' trices are removed. Use with care.
      example: 'trice renew': Rebuild ID list from source tree, discard old IDs.
        -dry-run
              No changes applied but output shows what would happen.
              "trice renew -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
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
        -li string
              Short for '-locationInformation'.
               (default "li.json")
        -locationInformation string
              The trice location list file.
              The specified JSON file is needed to display the location information for each ID during runtime and needs no version control.
              It is regenerated on each refresh, update or renew trice run. When trice log finds a location information file, it is used for
              log output with location information. Otherwise no location information is displayed, what usually is wanted in the field.
              This way the newest til.json can be used also with legacy firmware, but the li.json must match the current firmware version.
              "off" or "none" suppress the display of the location information even a li.json file exists. Avoid shared ID's for correct
              location information. See information for the -SharedIDs switch for additionals hints. See -tLocFmt for formatting.
               (default "li.json")
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
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
      sub-command 's|scan': Shows available serial ports)
      example: 'trice s': Show COM ports.
      sub-command 'sd|shutdown': Ends display server at IPA:IPP, works also on a remote machine.
      example: 'trice sd': Shut down remote display server.
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
              "auto": Use as logfile name "2006-01-02_1504-05_trice.log" with actual time.
              "filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
              All trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.
              Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
               (default "off")
        -v    short for verbose
        -verbose
              Gives more informal output if used. Can be helpful during setup.
              For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
      sub-command 'u|update': For updating ID list and source files.
      "trice update" will parse source tree(s) for new or changed TRICE macros, modify them appropriate and update/generate the JSON list.
      The "update" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
      example: 'trice update -src ../A -src ../../B': Parse ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json
        -IDMax value
              Upper end of ID range for normal trices. (default 7999)
        -IDMethod string
              Search method for new ID's in range- Options are 'upward', 'downward' & 'random'. (default "random")
        -IDMin value
              Lower end of ID range for normal trices. (default 1000)
        -addParamCount
              Extend TRICE macro names with the parameter count _n to enable compile time checks.
        -defaultStampSize int
              Default stamp size for written TRICE macros without id(0), Id(0 or ID(0). Valid values are 0, 16 or 32. (default 32)
        -dry-run
              No changes applied but output shows what would happen.
              "trice update -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
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
        -li string
              Short for '-locationInformation'.
               (default "li.json")
        -locationInformation string
              The trice location list file.
              The specified JSON file is needed to display the location information for each ID during runtime and needs no version control.
              It is regenerated on each refresh, update or renew trice run. When trice log finds a location information file, it is used for
              log output with location information. Otherwise no location information is displayed, what usually is wanted in the field.
              This way the newest til.json can be used also with legacy firmware, but the li.json must match the current firmware version.
              "off" or "none" suppress the display of the location information even a li.json file exists. Avoid shared ID's for correct
              location information. See information for the -SharedIDs switch for additionals hints. See -tLocFmt for formatting.
               (default "li.json")
        -s value
              Short for src.
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
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
      sub-command 'zeroSourceTreeIds': Set all Id(n) inside source tree dir to Id(0).
      The switch "-src" is mandatory and a multi-flag here. So you can use the "-src" flag several times.
      example: 'trice zeroSourceTreeIds -src ../A': Sets all TRICE IDs to 0 in ../A. Use with care!
        -dry-run
              No changes applied but output shows what would happen.
              "trice zeroSourceTreeIds -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
        -s value
              Short for src.
        -src value
              Source dir or file, It has one parameter. Not usable in the form "-src *.c".
              This is a multi-flag switch. It can be used several times for directories and also for files.
              Example: "trice zeroSourceTreeIds -dry-run -v -src ./test/ -src pkg/src/trice.h" will scan all C|C++ header and
              source code files inside directory ./test and scan also file trice.h inside pkg/src directory.
              Without the "-dry-run" switch it would create|extend a list file til.json in the current directory.
               (default "./")
        -v    short for verbose
        -verbose
              Gives more informal output if used. Can be helpful during setup.
              For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
              This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true.
      `
	id.FnJSON = "til.json"
	execHelper(t, input, expect)
}
