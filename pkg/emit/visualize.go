// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emit

import (
	"strings"
	"time"

	"github.com/fatih/color"
)

var noColor = color.New()

// visualize displays s and sets color and linebreaks
// The timestamp is printed only and only after \n and with the next s
// todo: The timestamp should be created on client side for more accurate timings
func visualize(s string) error {
	var err error
	s = trimBackslashes(s)
	c, s := colorChannel(s)

	// When a carriage return is executed, the whole next line gets the current background color.
	// Therefore detect this case and set the color to a default value before the carriage return.
	if strings.HasSuffix(s, "\n") {
		s := strings.TrimSuffix(s, "\n")
		if true == tsFlag {
			//tsFlag = false
			o := color.NoColor
			color.NoColor = true // disabling colored output here reduces probabillity for additional spaces in terminal under stress
			switch TimeStampFormat {
			case "LOCmicro":
				_, err = noColor.Print(time.Now().Format(time.StampMicro), ": ")
			case "UTCmicro":
				_, err = noColor.Print(time.Now().UTC().Format(time.StampMicro), ": ")
			case "off": // do nothing
			}
			color.NoColor = o
		}
		noColor.Print(Prefix)
		printIt(s, c)
		o := color.NoColor
		color.NoColor = true // disabling colored output here reduces probabillity for additional spaces in terminal under stress
		_, _ = noColor.Println()
		color.NoColor = o
		//tsFlag = true
	} else {
		_, err = printIt(s, c) ////////////// NEEDS to apend string here as slice!!!
	}

	return err
}

func printIt(s string, c *color.Color) (int, error) {
	if nil != c {
		return c.Print(s)
	}
	return noColor.Print(s)
}
