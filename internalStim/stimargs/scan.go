// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net

package stimargs

import (
	"io"

	"github.com/rokath/trice/internalStim/stimcom"
)

func scScan(w io.Writer) error {
	distributeArgs(w)
	_, err := stimcom.GetSerialPorts(w)
	return err
}
