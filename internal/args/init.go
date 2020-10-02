// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"flag"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/pkg/cage"
	"github.com/rokath/trice/pkg/cipher"
)

func init() {
	fsScLog = flag.NewFlagSet("log", flag.ExitOnError)                                                                                                                                                                           // subcommand
	fsScLog.StringVar(&encoding, "encoding", "bare", "trice transmit data format type, option: 'esc'.")                                                                                                                          // flag
	fsScLog.StringVar(&encoding, "e", "bare", "short for -encoding")                                                                                                                                                             // short flag
	fsScLog.StringVar(&cipher.Password, "password", "none", "decrypt passphrase")                                                                                                                                                // flag
	fsScLog.StringVar(&cipher.Password, "pw", "none", "short for -password")                                                                                                                                                     // short flag
	fsScLog.BoolVar(&cipher.ShowKey, "key", false, "show encryption key")                                                                                                                                                        // flag
	fsScLog.StringVar(&emitter.TimeStampFormat, "ts", "LOCmicro", "PC timestamp for logs and logfile name, options: 'off|none|UTCmicro|zero'")                                                                                   // flag
	fsScLog.StringVar(&emitter.ColorPalette, "color", "default", "color set, 'off' disables color handling (\"w:x\"->\"w:x\"), 'none' disables channels color (\"w:x\"->\"x\"), options: 'off|none'")                            // flag
	fsScLog.StringVar(&emitter.Prefix, "prefix", "source: ", "line prefix, options: any string or 'off|none' or 'source:' followed by 0-12 spaces, 'source:' will be replaced by source value e.g., 'COM17:'")                   // flag
	fsScLog.StringVar(&emitter.Suffix, "suffix", "", "append suffix to all lines, options: any string")                                                                                                                          // flag
	fsScLog.StringVar(&Port, "port", "STLINK", "receiver device: 'STLINK'|'JLINK'|serial name. The serial name is like 'COM12' for Windows or a Linux name like '/dev/tty/usb12'.")                                              //|filename|SIM|RND|HTTP'")                                                                                            // flag
	fsScLog.StringVar(&Port, "p", "STLINK", "short for -port")                                                                                                                                                                   // short flag
	fsScLog.IntVar(&com.Baud, "baud", 115200, "COM baudrate, valid only for '-port COMn'")                                                                                                                                       // flag flag
	fsScLog.StringVar(&link.Args, "largs", "-Device STM32F070F6 -if SWD -Speed 4000 -RTTChannel 0", "passed parameter string, valid only for '-p STLINK|JLINK', see for STLINK also JLinkRTTLogger in SEGGER UM08001_JLink.pdf") // JLRTT flag
	//fsScLog.StringVar(&link.Args, "largs", `-RTTSearchRanges "0x20000000 0x1800"`, "passed parameter string, valid only for '-p STLINK|JLINK', see for STLINK also JLinkRTTLogger in SEGGER UM08001_JLink.pdf") // JLRTT flag
	fsScLog.BoolVar(&displayRemote, "displayserver", false, "send trice lines to displayserver @ ipa:ipp")
	fsScLog.BoolVar(&displayRemote, "ds", false, "short for '-displayserver'")
	fsScLog.BoolVar(&autostart, "autostart", false, "autostart displayserver @ ipa:ipp (works not good with windows, because of cmd and powershell color issues and missing cli params in wt and gitbash)")
	fsScLog.BoolVar(&autostart, "a", false, "short for '-autostart'")
	fsScLog.BoolVar(&showInputBytes, "showInputBytes", false, "show incoming bytes")
	fsScLog.BoolVar(&showInputBytes, "sib", false, "short for '-showInputBytes'")
	flagLogfile(fsScLog)
	flagVerbosity(fsScLog)
	flagIDList(fsScLog)
	flagIPAddress(fsScLog)
}

func init() {
	fsScUpdate = flag.NewFlagSet("update", flag.ExitOnError)                                // subcommand
	fsScUpdate.BoolVar(&id.DryRun, "dry-run", false, "no changes are applied")              // flag
	fsScUpdate.Var(&id.Srcs, "src", "source dir or file, multi use possible, default './'") // multi flag
	flagVerbosity(fsScUpdate)
	flagIDList(fsScUpdate)
}

func init() {
	fsScZero = flag.NewFlagSet("zeroSourceTreeIds", flag.ContinueOnError)                 // subcommand (during development only)
	pSrcZ = fsScZero.String("src", "", "zero all Id(n) inside source tree dir, required") // flag
	fsScZero.BoolVar(&id.DryRun, "dry-run", false, "no changes are applied")              // flag
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
	fsScSv = flag.NewFlagSet("displayServer", flag.ExitOnError)                                   // subcommand
	fsScSv.StringVar(&emitter.ColorPalette, "color", "default", "color set, options: 'off|none'") // flag
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
	p.StringVar(&cage.Name, "logfile", "off", "append all output to logfile, options: 'none|filename|auto', 'auto' for \"2006-01-02_1504-05_trice.log\" with actual time") // flag
	p.StringVar(&cage.Name, "lg", "off", "short for -logfile")                                                                                                             // short flag
}

func flagVerbosity(p *flag.FlagSet) {
	p.BoolVar(&verbose, "verbose", false, "verbose, more informal output if used") // flag
	p.BoolVar(&verbose, "v", false, "short for verbose")                           // flag
}

func flagIDList(p *flag.FlagSet) {
	p.StringVar(&fnJSON, "idlist", "til.json", "trice ID list path, 'none' possible") // flag
	p.StringVar(&fnJSON, "i", "til.json", "short for '-idlist'")                      // flag
}

func flagIPAddress(p *flag.FlagSet) {
	p.StringVar(&emitter.IPAddr, "ipa", "localhost", "ip address like '127.0.0.1'") // flag
	p.StringVar(&emitter.IPPort, "ipp", "61497", "16 bit port number")              // flag
}
