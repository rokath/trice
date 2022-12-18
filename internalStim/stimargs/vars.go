// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net

package stimargs

import (
	"flag"
	"io"

	"github.com/rokath/trice/internalStim/stim"
	"github.com/rokath/trice/internalStim/stimcom"
)

var (
	// Version is the program version number and is injected from main package.
	Version string

	// Commit is the program checksum and injected from main package.
	Commit string

	// Date is the compilation time and injected from main package.
	Date string

	// verbose gives more information on output if set. This variable is injected into the appropriate packages.
	// Accessing for example args.Verbose from the packages makes them dependent on args and causes circular dependencies.
	verbose bool

	// serialPort is the OS specific comport name.
	serialPort string

	// baudRate is the selected serial port speed.
	baudRate int

	// parity is the selected serial port bit parity.
	parity string

	// fsScHelp is flag set for sub command 'help'.
	fsScHelp *flag.FlagSet

	// fsScVersion is flag set for sub command 'version'.
	fsScVersion *flag.FlagSet

	// fsScScan is flag set for sub command 'scan'.
	fsScScan *flag.FlagSet

	// fsScWrite is flag set for sub command 'write'.
	fsScWrite *flag.FlagSet

	allHelp     bool // flag for partial help
	helpHelp    bool // flag for partial help
	versionHelp bool // flag for partial help
	scanHelp    bool // flag for partial help
	writeHelp   bool // flag for partial help

	// hexBytes is a space separated sequence of bytes in hexadecimal notation like "a1 b2"
	hexBytes string
)

// distributeArgs is distributing values used in several packages.
// It must not be called before the appropriate arg parsing.
func distributeArgs(w io.Writer) {
	stimcom.Verbose = verbose
	stimcom.SerialPort = serialPort
	stimcom.BaudRate = baudRate
	stimcom.Parity = parity
	stim.Verbose = verbose
}
