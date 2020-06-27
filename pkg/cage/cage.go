// Package cage copies all output to stdout, stderr and log output in a logfile.
//
// Usage:
// cage.Name = cage.DefaultLogfileName
// cage.Enable()
// defer cage.Disable()
// do stuff...
// for capturing output in a string consider "import capturer "github.com/kami-zh/go-capturer"
//
// TODO: debug code:
//	Problem:
//	cage.Enable() re-direkted os.Stderr und dupliziert so Ausgaben an os.Sterr in trice.log
//	hCmd.Output() usw. geben os.Stderr zurück. Das ist auch wirklich die re-directed Adresse.
//	Der Paketest von cage funktioniert, auch TestScHelp() geht fehlerfrei durch.
//	Wenn aber über die Kommandozeile "trice h" erfolgt, landen Ausgaben an os.Stderr NICHT in trice.log obwohl os.Stderr re-directed ist.
//		log.SetFlags(0)
//		fmt.Println("0: os.Stderr     addr:", os.Stderr) // ok not in trice.log
//		cage.Enable()
//		defer cage.Disable()
//		fmt.Println("1: os.Stderr     addr:", os.Stderr)     // ok in trice.log (os.Stdout)
//		fmt.Println("2: hCmd.Output() addr:", hCmd.Output()) // ok in trice.log (os.Stdout)
//
//		fmt.Fprintln(os.Stderr, "3:      OS.STDERR")       // NOT in trice.log
//		fmt.Fprintln(hCmd.Output(), "4:    hCmd.Output()") // NOT in trice.log
//		log.Println("5: TryIt")                            // ok in trice.log (log out)
//		fmt.Println("6: TryIt2")                           // ok in trice.log (os.Stdout)
//		fmt.Fprintln(os.Stdout, "7:      OS.STDOUT")       // ok in trice.log (os.Stdout)
package cage

import (
	"fmt"
	"io"
	"log"
	"os"
	"sync"
	"time"

	"github.com/rokath/trice/internal/global"
)

// some references:
// https://stackoverflow.com/questions/54276178/golang-add-custom-os-file-to-os-stdout
// https://play.golang.org/p/CqUOP8aKL0
// https://gist.github.com/jmoiron/e9f72720cef51862b967
// https://medium.com/@hau12a1/golang-capturing-log-println-and-fmt-println-output-770209c791b4

var (
	// DefaultLogfileName is the pattern for default logfile name. The timestamp is replaced with the actual time.
	DefaultLogfileName = "2006-01-02_1504-05_cage.log"

	// Name is the filename of the logfile. "off" inhibits logfile writing.
	Name = "off"

	// pContainer hold the restore values
	pContainer *Container
)

// Enable starts take notes mode, means parallel writing into a file.
// Name has to be assigned to a value other than "off" or "none" for taking effect.
func Enable() {
	pContainer = Start(Name)
}

// Disable ends take notes mode, means parallel writing into a file.
// It takes no effect when pContainer is nil, what is the case when Enable was started with "off" or "none" as 'Name'.
func Disable() {
	Stop(pContainer)
}

// Container keeps re-direction informantion for restoring
type Container struct {
	// old
	oldLog     io.Writer
	origStdout *os.File
	origStderr *os.File

	// new
	writerStdout *os.File
	writerStderr *os.File
	lfHandle     *os.File // logfile handle
	lfName       string
	wg           sync.WaitGroup
}

// Start does append all output parallel into a logfile with name fn
func Start(fn string) *Container {

	// start logging only if fn not "none" or "off"
	if "none" == fn || "off" == fn {
		if global.Verbose {
			fmt.Println("No logfile writing...")
		}
		return nil
	}
	if "auto" == fn {
		fn = DefaultLogfileName
	}
	// open logfile
	if DefaultLogfileName == fn {
		fn = time.Now().Format(fn) // replace timestamp in default logfilename
	} // otherwise use cli defined logfilename
	lfH, err := os.OpenFile(fn, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	if err != nil {
		log.Fatalf("error opening file %s: %v", fn, err)
		fn = "off"
		return nil
	}
	if global.Verbose {
		log.Printf("Writing to logfile %s...\n", fn)
	}

	// open pipes
	rStdout, wStdout, _ := os.Pipe()
	rStderr, wStderr, _ := os.Pipe()

	// create container for recovering
	c := &Container{
		oldLog: log.Writer(),

		origStdout: os.Stdout,
		origStderr: os.Stderr,

		writerStdout: wStdout,
		writerStderr: wStderr,

		lfHandle: lfH,
		lfName:   fn,
	}

	// re-direct
	log.SetOutput(io.MultiWriter(c.oldLog, c.lfHandle)) // writing to log will go also to logfile now
	os.Stdout = c.writerStdout                          // all to os.Stdout goees now to c.writerStdout and comee out of rStdout now
	os.Stderr = c.writerStderr                          // all to os.Stderr goees now to c.writerStderr and comes out of rStderr now

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
func Stop(c *Container) {

	// only if loggig was enabled
	if nil == c {
		if global.Verbose {
			fmt.Println("No logfile writing...done")
		}
		return
	}

	// close pipes
	_ = c.writerStdout.Close()
	_ = c.writerStderr.Close()

	// c.wg.Wait() // 2 go routines to be finished
	time.Sleep(100 * time.Millisecond)

	// restore
	os.Stdout = c.origStdout
	os.Stderr = c.origStderr
	log.SetOutput(c.oldLog)

	// logfile
	c.lfHandle.Close()
	if global.Verbose {
		log.Printf("Writing to logfile %s...done\n", c.lfName)
	}
}
