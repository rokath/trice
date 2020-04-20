package cage

import (
	"io"
	"log"
	"os"
	"sync"
	"time"
)

// Container keeps re-direction informantion
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
		log.Println("No logfile writing...")
		return nil
	}

	// open logfile
	lFn := time.Now().Format("2006-01-02_150405_") + fn
	lfH, err := os.OpenFile(lFn, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	if err != nil {
		log.Fatalf("error opening file %s: %v", fn, err)
		fn = "off"
		return nil
	}
	log.Printf("Writing to logfile %s...\n", lFn)

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
		lfName:   lFn,
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

// Stop does return to normal state
func Stop(c *Container) {

	// only if loggig was enabled
	if nil == c {
		log.Println("No logfile writing...done")
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
	log.Printf("Writing to logfile %s...done\n", c.lfName)
}
