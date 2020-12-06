// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"flag"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/cage"
)

func init() {
	fsScLog = flag.NewFlagSet("log", flag.ExitOnError)                                                                                       // subcommand
	fsScLog.StringVar(&encoding, "encoding", "bare", "The trice transmit data format type, option: 'esc'. Target device coding must match.") // flag
	fsScLog.StringVar(&encoding, "e", "bare", "Short for -encoding.")                                                                        // short flag
	//fsScLog.StringVar(&cipher.Password, "password", "none", "The decrypt passphrase.")                                                                                                                                                // flag
	//fsScLog.StringVar(&cipher.Password, "pw", "none", "Short for -password.")                                                                                                                                                     // short flag
	//fsScLog.BoolVar(&cipher.ShowKey, "key", false, "Show encryption key.")                                                                                                                                                        // flag
	fsScLog.StringVar(&emitter.TimestampFormat, "ts", "LOCmicro", "PC timestamp for logs and logfile name, options: 'off|none|UTCmicro|zero'")                                                                  // flag
	fsScLog.StringVar(&emitter.ColorPalette, "color", "default", "Color set, 'off' disables color handling (\"w:x\"->\"w:x\"), 'none' disables channels color (\"w:x\"->\"x\"), options: 'off|none'.")          // flag
	fsScLog.StringVar(&emitter.Prefix, "prefix", "source: ", "Line prefix, options: any string or 'off|none' or 'source:' followed by 0-12 spaces, 'source:' will be replaced by source value e.g., 'COM17:'.") // flag
	fsScLog.StringVar(&emitter.Suffix, "suffix", "", "Append suffix to all lines, options: any string.")                                                                                                        // flag
	fsScLog.StringVar(&port, "port", "JLINK", "receiver device: 'STLINK'|'JLINK'|serial name. The serial name is like 'COM12' for Windows or a Linux name like '/dev/tty/usb12'.")                              //|filename|SIM|RND|HTTP'")                                                                                            // flag
	fsScLog.StringVar(&port, "p", "JLINK", "short for -port")                                                                                                                                                   // short flag
	fsScLog.IntVar(&com.Baud, "baud", 115200, "COM baudrate, valid only for '-port COMn'.")                                                                                                                     // flag flag
	fsScLog.StringVar(&portArguments, "args", "default", "To port specific passed parameter string.")
	fsScLog.BoolVar(&displayRemote, "displayserver", false, "Send trice lines to displayserver @ ipa:ipp.")
	fsScLog.BoolVar(&displayRemote, "ds", false, "Short for '-displayserver'.")
	fsScLog.BoolVar(&autostart, "autostart", false, "Autostart displayserver @ ipa:ipp (works not good with windows, because of cmd and powershell color issues and missing cli params in wt and gitbash).")
	fsScLog.BoolVar(&autostart, "a", false, "Short for '-autostart'.")
	fsScLog.BoolVar(&showInputBytes, "showInputBytes", false, "Show incoming bytes, what can be helpful during setup.")
	fsScLog.BoolVar(&showInputBytes, "s", false, "Short for '-showInputBytes'.")
	flagLogfile(fsScLog)
	flagVerbosity(fsScLog)
	flagIDList(fsScLog)
	flagIPAddress(fsScLog)
}

func init() {
	fsScUpdate = flag.NewFlagSet("update", flag.ExitOnError)                                                   // subcommand
	fsScUpdate.BoolVar(&id.DryRun, "dry-run", false, "No changes applied but output shows what would happen.") // flag
	fsScUpdate.Var(&id.Srcs, "src", "Source dir or file, multi use possible, default is './'.")                // multi flag
	fsScUpdate.Var(&id.Srcs, "s", "Short for src.")                                                            // multi flag
	flagVerbosity(fsScUpdate)
	flagIDList(fsScUpdate)
}

func init() {
	fsScZero = flag.NewFlagSet("zeroSourceTreeIds", flag.ContinueOnError)                                    // subcommand (during development only)
	pSrcZ = fsScZero.String("src", "", "Zero all Id(n) inside source tree dir, required.")                   // flag
	fsScZero.BoolVar(&id.DryRun, "dry-run", false, "No changes applied but output shows what would happen.") // flag
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
	fsScSv = flag.NewFlagSet("displayServer", flag.ExitOnError)                                    // subcommand
	fsScSv.StringVar(&emitter.ColorPalette, "color", "default", "Color set, options: 'off|none'.") // flag
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
	p.StringVar(&cage.Name, "logfile", "off", "Append all output to logfile, options: 'none|filename|auto', 'auto' for \"2006-01-02_1504-05_trice.log\" with actual time.") // flag
	p.StringVar(&cage.Name, "l", "off", "Short for -logfile.")                                                                                                              // short flag
}

func flagVerbosity(p *flag.FlagSet) {
	p.BoolVar(&verbose, "verbose", false, "Verbose, more informal output if used. Can be helpful during setup.") // flag
	p.BoolVar(&verbose, "v", false, "short for verbose")                                                         // flag
}

func flagIDList(p *flag.FlagSet) {
	p.StringVar(&fnJSON, "idlist", "til.json", "The trice ID list path.") // flag
	p.StringVar(&fnJSON, "i", "til.json", "Short for '-idlist'.")         // flag
}

func flagIPAddress(p *flag.FlagSet) {
	p.StringVar(&emitter.IPAddr, "ipa", "localhost", "IP address like '127.0.0.1'.") // flag
	p.StringVar(&emitter.IPPort, "ipp", "61497", "16 bit IP port number.")           // flag
}
