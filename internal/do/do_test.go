// SPDX-License-Identifier: MIT

package do

import (
	"bytes"
	"io"
	"net"
	"os"
	"path/filepath"
	"runtime"
	"strings"
	"testing"
	"time"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/internal/link"
	"github.com/rokath/trice/internal/receiver"
	"github.com/rokath/trice/internal/translator"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
)

func requireWindowsTCPTestsEnabled(t *testing.T) {
	t.Helper()
	// These TCP listener tests intentionally open a local server socket.
	// On Windows, Defender Firewall may show interactive prompts for the
	// temporary go test binaries that host these listeners. The binaries live in
	// changing go-build paths, so allowing them once does not reliably suppress
	// later prompts. That makes routine local test runs noisy and disruptive for
	// developers who mostly work on Windows.
	//
	// We therefore skip these tests on Windows by default and require an
	// explicit opt-in when someone actually wants to verify the listener-based
	// behavior there.
	//
	// Enable with:
	//   TRICE_RUN_WINDOWS_TCP_TESTS=1 go test ./...
	if runtime.GOOS == "windows" && os.Getenv("TRICE_RUN_WINDOWS_TCP_TESTS") == "" {
		t.Skip("skipping TCP listener test on Windows; set TRICE_RUN_WINDOWS_TCP_TESTS=1 to enable")
	}
}

type doGlobalsSnapshot struct {
	tcpOutAddr        string
	verbose           bool
	idVerbose         bool
	msgVerbose        bool
	linkVerbose       bool
	decoderVerbose    bool
	emitterVerbose    bool
	receiverVerbose   bool
	translatorVerbose bool
	testTableMode     bool
	emitterTestMode   bool
	idMin             id.TriceID
	idMax             id.TriceID
	defaultStampSize  int
	stampSizeID       string
	colorPalette      string
}

func snapshotDoGlobals() doGlobalsSnapshot {
	return doGlobalsSnapshot{
		tcpOutAddr:        TCPOutAddr,
		verbose:           Verbose,
		idVerbose:         id.Verbose,
		msgVerbose:        msg.Verbose,
		linkVerbose:       link.Verbose,
		decoderVerbose:    decoder.Verbose,
		emitterVerbose:    emitter.Verbose,
		receiverVerbose:   receiver.Verbose,
		translatorVerbose: translator.Verbose,
		testTableMode:     decoder.TestTableMode,
		emitterTestMode:   emitter.TestTableMode,
		idMin:             id.Min,
		idMax:             id.Max,
		defaultStampSize:  id.DefaultStampSize,
		stampSizeID:       id.StampSizeId,
		colorPalette:      emitter.ColorPalette,
	}
}

func restoreDoGlobals(s doGlobalsSnapshot) {
	TCPOutAddr = s.tcpOutAddr
	Verbose = s.verbose
	id.Verbose = s.idVerbose
	msg.Verbose = s.msgVerbose
	link.Verbose = s.linkVerbose
	decoder.Verbose = s.decoderVerbose
	emitter.Verbose = s.emitterVerbose
	receiver.Verbose = s.receiverVerbose
	translator.Verbose = s.translatorVerbose
	decoder.TestTableMode = s.testTableMode
	emitter.TestTableMode = s.emitterTestMode
	id.Min = s.idMin
	id.Max = s.idMax
	id.DefaultStampSize = s.defaultStampSize
	id.StampSizeId = s.stampSizeID
	emitter.ColorPalette = s.colorPalette
}

func setupValidIDRange() {
	id.Min = 1
	id.Max = 10
}

func TestDistributeArgsSetsGlobalFlagsAndStampID(t *testing.T) {
	s := snapshotDoGlobals()
	t.Cleanup(func() { restoreDoGlobals(s) })

	setupValidIDRange()
	decoder.TestTableMode = true
	id.DefaultStampSize = 16
	emitter.ColorPalette = "none"
	TCPOutAddr = ""

	var out bytes.Buffer
	fs := afero.Afero{Fs: afero.NewMemMapFs()}

	got := DistributeArgs(&out, &fs, "off", true)
	if got == nil {
		t.Fatalf("DistributeArgs returned nil writer")
	}
	if !Verbose || !id.Verbose || !msg.Verbose || !link.Verbose || !decoder.Verbose || !emitter.Verbose || !receiver.Verbose || !translator.Verbose {
		t.Fatalf("expected verbose=true in all affected packages")
	}
	if !emitter.TestTableMode {
		t.Fatalf("expected emitter.TestTableMode copied from decoder.TestTableMode")
	}
	if id.StampSizeId != " Id(0)," {
		t.Fatalf("unexpected StampSizeId: %q", id.StampSizeId)
	}
}

func TestDistributeArgsFallsBackForNilWriterAndNilFs(t *testing.T) {
	s := snapshotDoGlobals()
	t.Cleanup(func() { restoreDoGlobals(s) })

	setupValidIDRange()
	id.DefaultStampSize = 0
	emitter.ColorPalette = "default"
	TCPOutAddr = ""

	got := DistributeArgs(nil, nil, "off", false)
	if got == nil {
		t.Fatalf("expected non-nil writer")
	}
}

func TestEvaluateColorPaletteUnknownFallsBackToDefault(t *testing.T) {
	s := snapshotDoGlobals()
	t.Cleanup(func() { restoreDoGlobals(s) })

	var out bytes.Buffer
	emitter.ColorPalette = "mystery"
	evaluateColorPalette(&out)

	if emitter.ColorPalette != "default" {
		t.Fatalf("expected fallback palette default, got %q", emitter.ColorPalette)
	}
	if !strings.Contains(out.String(), "Ignoring unknown -color mystery using default.") {
		t.Fatalf("missing warning output, got %q", out.String())
	}
}

func TestTriceOutputOffSkipsLogfileCreation(t *testing.T) {
	s := snapshotDoGlobals()
	t.Cleanup(func() { restoreDoGlobals(s) })
	TCPOutAddr = ""

	var out bytes.Buffer
	fs := afero.Afero{Fs: afero.NewMemMapFs()}

	w := triceOutput(&out, &fs, "off", false)
	n, err := w.Write([]byte("abc"))
	if err != nil {
		t.Fatalf("write failed: %v", err)
	}
	if n != 3 {
		t.Fatalf("unexpected bytes written: %d", n)
	}
	if out.String() != "abc" {
		t.Fatalf("unexpected output buffer content: %q", out.String())
	}
}

func TestTriceOutputAutoCreatesTimestampedLogfile(t *testing.T) {
	s := snapshotDoGlobals()
	t.Cleanup(func() { restoreDoGlobals(s) })
	TCPOutAddr = ""

	var out bytes.Buffer
	mem := afero.NewMemMapFs()
	fs := afero.Afero{Fs: mem}
	if err := fs.MkdirAll("logs", 0o755); err != nil {
		t.Fatalf("mkdir failed: %v", err)
	}

	w := triceOutput(&out, &fs, filepath.Join("logs", "auto"), false)
	if _, err := io.WriteString(w, "line\n"); err != nil {
		t.Fatalf("write failed: %v", err)
	}

	entries, err := afero.ReadDir(mem, "logs")
	if err != nil {
		t.Fatalf("readdir failed: %v", err)
	}
	if len(entries) != 1 {
		t.Fatalf("expected exactly one logfile, got %d", len(entries))
	}
	if !strings.HasSuffix(entries[0].Name(), "_trice.log") {
		t.Fatalf("unexpected logfile name: %q", entries[0].Name())
	}
	logContent, err := afero.ReadFile(mem, filepath.Join("logs", entries[0].Name()))
	if err != nil {
		t.Fatalf("read logfile failed: %v", err)
	}
	if string(logContent) != "line\n" {
		t.Fatalf("unexpected logfile content: %q", string(logContent))
	}
}

func TestTCPWriterWithoutAddressReturnsDiscardWriter(t *testing.T) {
	s := snapshotDoGlobals()
	t.Cleanup(func() { restoreDoGlobals(s) })

	TCPOutAddr = ""
	w := tcpWriter()
	n, err := w.Write([]byte("x"))
	if err != nil {
		t.Fatalf("unexpected write error: %v", err)
	}
	if n != 1 {
		t.Fatalf("unexpected bytes written: %d", n)
	}
}

func pickLoopbackAddr(t *testing.T) string {
	t.Helper()
	l, err := net.Listen("tcp", "127.0.0.1:0")
	if err != nil {
		t.Fatalf("failed to reserve loopback port: %v", err)
	}
	addr := l.Addr().String()
	if err := l.Close(); err != nil {
		t.Fatalf("failed to close temporary listener: %v", err)
	}
	return addr
}

func connectWithRetry(t *testing.T, addr string) net.Conn {
	t.Helper()
	deadline := time.Now().Add(2 * time.Second)
	for {
		c, err := net.DialTimeout("tcp", addr, 200*time.Millisecond)
		if err == nil {
			return c
		}
		if time.Now().After(deadline) {
			t.Fatalf("dial %s failed: %v", addr, err)
		}
		time.Sleep(20 * time.Millisecond)
	}
}

func TestTCPWriterWithAddressReturnsConnAndForwardsWrites(t *testing.T) {
	requireWindowsTCPTestsEnabled(t)

	s := snapshotDoGlobals()
	t.Cleanup(func() { restoreDoGlobals(s) })

	Verbose = false
	TCPOutAddr = pickLoopbackAddr(t)

	ready := make(chan io.Writer, 1)
	go func() {
		ready <- tcpWriter()
	}()

	client := connectWithRetry(t, TCPOutAddr)
	defer client.Close()
	_ = client.SetDeadline(time.Now().Add(2 * time.Second))

	if _, err := client.Write([]byte("hello")); err != nil {
		t.Fatalf("client write failed: %v", err)
	}

	serverWriter := <-ready
	if serverWriter == nil {
		t.Fatalf("expected server writer")
	}
	if _, err := serverWriter.Write([]byte("world")); err != nil {
		t.Fatalf("server write failed: %v", err)
	}

	buf := make([]byte, 5)
	if _, err := io.ReadFull(client, buf); err != nil {
		t.Fatalf("client read failed: %v", err)
	}
	if string(buf) != "world" {
		t.Fatalf("unexpected forwarded data: %q", string(buf))
	}
}

func TestTCPWriterVerboseSendsGreeting(t *testing.T) {
	requireWindowsTCPTestsEnabled(t)

	s := snapshotDoGlobals()
	t.Cleanup(func() { restoreDoGlobals(s) })

	Verbose = true
	TCPOutAddr = pickLoopbackAddr(t)

	ready := make(chan io.Writer, 1)
	go func() {
		ready <- tcpWriter()
	}()

	client := connectWithRetry(t, TCPOutAddr)
	defer client.Close()
	_ = client.SetDeadline(time.Now().Add(2 * time.Second))

	if _, err := client.Write([]byte("ping")); err != nil {
		t.Fatalf("client write failed: %v", err)
	}

	greeting := make([]byte, len("Trice connected...\r\n"))
	if _, err := io.ReadFull(client, greeting); err != nil {
		t.Fatalf("client read greeting failed: %v", err)
	}
	if string(greeting) != "Trice connected...\r\n" {
		t.Fatalf("unexpected greeting: %q", string(greeting))
	}

	if <-ready == nil {
		t.Fatalf("expected server writer")
	}
}
