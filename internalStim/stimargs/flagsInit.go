// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net

package stimargs

import (
	"flag"
)

const (
	boolInfo = "This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true. You can also set it explicit: =false or =true"
)

func init() {
	FlagsInit()
}

func FlagsInit() {
	helpInit()
	versionInit()
	scanInit()
	writeInit()
}

func helpInit() {
	fsScHelp = flag.NewFlagSet("help", flag.ContinueOnError) // sub-command
	fsScHelp.BoolVar(&allHelp, "all", false, "Show all help.")

	fsScHelp.BoolVar(&helpHelp, "help", false, "Show h|help specific help.")
	fsScHelp.BoolVar(&helpHelp, "h", false, "Show h|help specific help.")

	fsScHelp.BoolVar(&versionHelp, "version", false, "Show ver|version specific help.")
	fsScHelp.BoolVar(&versionHelp, "ver", false, "Show ver|version specific help.")

	fsScHelp.BoolVar(&scanHelp, "scan", false, "Show s|scan specific help.")
	fsScHelp.BoolVar(&scanHelp, "s", false, "Show s|scan specific help.")

	fsScHelp.BoolVar(&writeHelp, "write", false, "Show w|write specific help.")
	fsScHelp.BoolVar(&writeHelp, "w", false, "Show w|write specific help.")

	flagVerbosity(fsScHelp)
}

func versionInit() {
	fsScVersion = flag.NewFlagSet("version", flag.ContinueOnError) // sub-command
	flagVerbosity(fsScVersion)
}

func scanInit() {
	fsScScan = flag.NewFlagSet("scan", flag.ContinueOnError) // sub-command
	flagVerbosity(fsScScan)
}

func writeInit() {
	fsScWrite = flag.NewFlagSet("write", flag.ContinueOnError) // sub-command
	flagVerbosity(fsScWrite)
	flagPort(fsScWrite)
	flagBaudRate(fsScWrite)
	flagParity(fsScWrite)
	fsScWrite.StringVar(&hexBytes, "hex", "00", `hex is a space separated sequence of bytes in hexadecimal notation like "a1 b2" (case insensitive). Hint: As long no 0-delimiter is sent, the nodes simply receive without reporting.`)
}

func flagVerbosity(p *flag.FlagSet) {
	p.BoolVar(&verbose, "verbose", false, `Gives more informal output if used. Can be helpful during setup.
`+boolInfo) // flag
	p.BoolVar(&verbose, "v", false, "short for verbose") // flag
}

func flagPort(p *flag.FlagSet) {
	p.StringVar(&serialPort, "port", "COM1", `port is the OS specific comport name`)
	p.StringVar(&serialPort, "p", "COM1", `p is short for port`)
}

func flagBaudRate(p *flag.FlagSet) {
	p.IntVar(&baudRate, "baud", 115200, `baud is the selected serial port speed.`)
}

func flagParity(p *flag.FlagSet) {
	p.StringVar(&parity, "parity", "N", `parity is the selected serial port parity. Valid values are "n","none", "o", "odd", "e", "even" (case insensitive)`)
}
