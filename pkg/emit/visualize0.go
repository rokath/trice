// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// +build lf0
// `go build -tags lf0` does include this file

package emit

import (
	"fmt"
	"strings"
	"time"

	"github.com/fatih/color"
	"github.com/rokath/trice/pkg/lgf"
)

// visualize displays s and sets color and linebreaks
// The timestamp is printed only and only after \n and with the next s
// todo: The timestamp should be created on client side for more accurate timings
func visualize(s string) error {
	var err error
	s = trimBackslashes(s)
	c, s := colorChannel(s)
	b := color.New(color.FgWhite).Add(color.BgBlack) // assuming this as the default terminal background color
	if true == tsFlag {
		tsFlag = false
		switch TimeStampFormat {
		case "LOCmicro":
			//_, err = b.Print(time.Now().Format(time.StampMicro), ": ")
			_, err = b.Fprint(lgf.Tee, time.Now().Format(time.StampMicro), ": ") // needed for -tags lf0
		case "UTCmicro":
			//_, err = b.Print(time.Now().UTC().Format(time.StampMicro), ": ")
			_, err = b.Fprint(lgf.Tee, time.Now().UTC().Format(time.StampMicro), ": ") // needed for -tags lf0
		case "off": // do nothing
		}
	}
	// When a carriage return is executed, the whole next line gets the current background color.
	// Therefore detect this case and set the color to a default value before the carriage return.
	if strings.HasSuffix(s, "\n") {
		s := strings.TrimSuffix(s, "\n")
		printIt(s, c)
		//_, _ = b.Println()
		_, _ = b.Fprintln(lgf.Tee) // needed for -tags lf0
		//_, _ = fmt.Fprintln(lgf.Tee)
		tsFlag = true
	} else {
		_, err = printIt(s, c)
	}
	return err
}

func printIt(s string, c *color.Color) (int, error) {
	if nil != c {
		//log.Fprint(Tee,s)
		//return 0, nil
		//return c.Print(s) // ok for display server, but not possible with logfile0.go
		return c.Fprint(lgf.Tee, s) // needed for -tags lf0 but not ok for diaplay server
		//return fmt.Fprint(lgf.Tee, s) /////////////////#########################!!!!!!!!!!!!!!
	}
	//return fmt.Print(s)
	return fmt.Fprint(lgf.Tee, s) // // needed for -tags lf0
}
