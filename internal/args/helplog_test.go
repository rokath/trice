// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"fmt"
	"testing"

	"github.com/rokath/trice/pkg/tst"
)

func TestHelpLog(t *testing.T) {
	fn := func() {
		err := Handler([]string{"trice", "help", "-log"})
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
            Target timestamp format string at start of each line, if target timestamps existent (configured). Use "" to suppress existing target timestamps. If several trices form a log line only the timestamp of first trice ist displayed. (default "tim:%9d")
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
	tst.EqualLines(t, exp, act)
}
