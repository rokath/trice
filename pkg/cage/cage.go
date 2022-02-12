// Package cage copies all output to stdout, stderr and log output in a logfile.
//
// Usage:
// cage.Name = cage.DefaultLogfileName
// cage.Enable(w)
// defer cage.Disable(w)
// do stuff...
package cage

import (
	"fmt"
	"io"
	"log"
	"os"
	"sync"
	"time"

	"github.com/rokath/trice/pkg/msg"
)

// some references:
// https://stackoverflow.com/questions/54276178/golang-add-custom-os-file-to-os-stdout
// https://play.golang.org/p/CqUOP8aKL0
// https://gist.github.com/jmoiron/e9f72720cef51862b967
// https://medium.com/@hau12a1/golang-capturing-log-println-and-fmt-println-output-770209c791b4

var (
	// Verbose gives more information on output if set. The value is injected from main packages.
	Verbose bool

	// DefaultLogfileName is the pattern for default logfile name. The timestamp is replaced with the actual time.
	DefaultLogfileName = "2006-01-02_1504-05_cage.log"

	// Name is the filename of the logfile. "off" inhibits logfile writing.
	Name = "off"

	// pContainer hold the restore values
	pContainer *Container
)

// Enable starts take notes mode, means parallel writing into a file.
// Name has to be assigned to a value other than "off" or "none" for taking effect.
func Enable(w io.Writer) {
	pContainer = Start(w, Name)
}

// Disable ends take notes mode, means parallel writing into a file.
// It takes no effect when pContainer is nil, what is the case when Enable was started with "off" or "none" as 'Name'.
func Disable(w io.Writer) {
	Stop(w, pContainer)
}

// Container keeps re-direction information for restoring
type Container struct {
	// old
	oldLog     io.Writer
	origStdout *os.File
	origStderr *os.File
	//origOut    io.Writer

	// new
	writerStdout *os.File
	writerStderr *os.File
	lfHandle     *os.File // logfile handle
	lfName       string
	wg           sync.WaitGroup
}

// Start does append all output parallel into a logfile with name fn
func Start(w io.Writer, fn string) *Container {

	// start logging only if fn not "none" or "off"
	if "none" == fn || "off" == fn {
		if Verbose {
			fmt.Fprintln(w, "No logfile writing...")
		}
		return nil
	}
	if "auto" == fn {
		fn = DefaultLogfileName
	}
	// open logfile
	if DefaultLogfileName == fn {
		fn = time.Now().Format(fn) // Replace timestamp in default log filename.
	} // Otherwise, use cli defined log filename.
	lfH, err := os.OpenFile(fn, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	msg.FatalOnErr(err)
	if Verbose {
		fmt.Fprintf(w, "Writing to logfile %s...\n", fn)
	}

	// open pipes
	rStdout, wStdout, err := os.Pipe()
	msg.FatalOnErr(err)
	rStderr, wStderr, err := os.Pipe()
	msg.FatalOnErr(err)

	// create container for recovering
	c := &Container{
		oldLog: log.Writer(),

		origStdout: os.Stdout,
		origStderr: os.Stderr,

		//origOut: w,

		writerStdout: wStdout,
		writerStderr: wStderr,

		lfHandle: lfH,
		lfName:   fn,
	}

	// re-direct
	log.SetOutput(io.MultiWriter(c.oldLog, c.lfHandle)) // writing to log will go also to logfile now
	os.Stdout = c.writerStdout                          // all to os.Stdout goes now to c.writerStdout and comes out of rStdout now
	os.Stderr = c.writerStderr                          // all to os.Stderr goes now to c.writerStderr and comes out of rStderr now

	// create duplication
	teeOut := io.MultiWriter(c.origStdout, c.lfHandle)
	teeErr := io.MultiWriter(c.origStderr, c.lfHandle)

	// copy from pipe to tee
	c.wg.Add(2)
	go func(w io.Writer, r io.Reader) {
		defer c.wg.Done()
		for {
			_, err = io.Copy(w, r)
			if nil != err {
				return // pipe closed
			}
		}
	}(teeOut, rStdout)
	go func(w io.Writer, r io.Reader) {
		defer c.wg.Done()
		for {
			_, err = io.Copy(w, r)
			if nil != err {
				return // pipe closed
			}
		}
	}(teeErr, rStderr)

	return c
}

// Stop does return to normal state.
func Stop(w io.Writer, c *Container) {

	// only if logging was enabled
	if nil == c {
		if Verbose {
			fmt.Fprintln(w, "No logfile writing...done")
		}
		return
	}

	// close pipes
	msg.OnErr(c.writerStdout.Close())
	msg.OnErr(c.writerStderr.Close())

	// c.wg.Wait() // 2 go routines to be finished
	time.Sleep(100 * time.Millisecond)

	// restore
	os.Stdout = c.origStdout
	os.Stderr = c.origStderr
	log.SetOutput(c.oldLog)

	// logfile
	msg.OnErr(c.lfHandle.Close())
	if Verbose {
		log.Printf("Writing to logfile %s...done\n", c.lfName)
	}
}
