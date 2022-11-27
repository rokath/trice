// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net

package args

import (
	"io"

	"github.com/rokath/trice/internalStim/com"
)

func scScan(w io.Writer) error {
	distributeArgs(w)
	_, err := com.GetSerialPorts(w)
	return err
}
