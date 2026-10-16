// SPDX-License-Identifier: MIT

package do

import (
	"bytes"
	"io"
	"net"
	"path/filepath"
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
	"github.com/stretchr/testify/assert"
)

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

// snapshotDoGlobals captures do package globals so a test can restore them afterwards.
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

// restoreDoGlobals restores do package globals from a saved snapshot.
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

// setupValidIDRange prepares a valid ID range for tests that depend on global ID settings.
func setupValidIDRange() {
	id.Min = 1
	id.Max = 10
}

// TestDistributeArgsSetsGlobalFlagsAndStampID verifies the expected behavior.
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

// TestDistributeArgsFallsBackForNilWriterAndNilFs verifies the expected behavior.
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

// TestEvaluateNamedColorPalettes verifies that selectable palettes pass CLI
// validation without warnings or silent replacement by another palette.
func TestEvaluateNamedColorPalettes(t *testing.T) {
	s := snapshotDoGlobals()
	t.Cleanup(func() { restoreDoGlobals(s) })
	for _, palette := range []string{"dark", "light", "contrast", "default", "color", "off", "none"} {
		t.Run(palette, func(t *testing.T) {
			var out bytes.Buffer
			emitter.ColorPalette = palette
			evaluateColorPalette(&out)
			assert.Equal(t, palette, emitter.ColorPalette)
			assert.Empty(t, out.String())
		})
	}
}

// TestEvaluateColorPaletteUnknownFallsBackToDefault verifies the expected behavior.
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

// TestTriceOutputOffSkipsLogfileCreation verifies the expected behavior.
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

// TestTriceOutputAutoCreatesTimestampedLogfile verifies the expected behavior.
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

// TestTCPWriterWithoutAddressReturnsDiscardWriter verifies the expected behavior.
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

// pickLoopbackAddr keeps the selected socket open until tcpWriter accepts its
// client. Injecting that listener prevents a reserve/close/rebind race.
func pickLoopbackAddr(t *testing.T) string {
	t.Helper()
	l, err := net.ListenTCP("tcp4", &net.TCPAddr{IP: net.IPv4(127, 0, 0, 1)})
	if err != nil {
		t.Fatalf("failed to reserve loopback port: %v", err)
	}
	assert.NoError(t, l.SetDeadline(time.Now().Add(5*time.Second)))
	savedListen := TCPListen
	TCPListen = func(network, address string) (net.Listener, error) { return l, nil }
	t.Cleanup(func() {
		_ = l.Close()
		TCPListen = savedListen
	})
	return l.Addr().String()
}

// connectWithRetry connects to the TCP endpoint with retries until it becomes reachable.
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

// startTCPWriterTest joins the output setup even if an assertion stops the
// caller before it receives the writer. Closing the client releases a pending
// handshake; any unclaimed server connection is then closed before globals reset.
func startTCPWriterTest(t *testing.T) (net.Conn, <-chan io.Writer) {
	t.Helper()
	TCPOutAddr = pickLoopbackAddr(t)
	ready := make(chan io.Writer, 1)
	done := make(chan struct{})
	var client net.Conn
	t.Cleanup(func() {
		if client != nil {
			_ = client.Close()
		}
		select {
		case <-done:
		case <-time.After(6 * time.Second):
			t.Error("TCP writer setup did not stop")
		}
		select {
		case writer := <-ready:
			if closer, ok := writer.(io.Closer); ok {
				assert.NoError(t, closer.Close())
			}
		default:
		}
	})
	go func() {
		ready <- tcpWriter()
		close(done)
	}()
	client = connectWithRetry(t, TCPOutAddr)
	assert.NoError(t, client.SetDeadline(time.Now().Add(2*time.Second)))
	return client, ready
}

// TestTCPWriterWithAddressReturnsConnAndForwardsWrites verifies the expected behavior.
func TestTCPWriterWithAddressReturnsConnAndForwardsWrites(t *testing.T) {
	s := snapshotDoGlobals()
	t.Cleanup(func() { restoreDoGlobals(s) })

	Verbose = false
	client, ready := startTCPWriterTest(t)
	defer client.Close()

	if _, err := client.Write([]byte("hello")); err != nil {
		t.Fatalf("client write failed: %v", err)
	}

	var serverWriter io.Writer
	select {
	case serverWriter = <-ready:
	case <-time.After(5 * time.Second):
		t.Fatal("TCP writer did not finish its handshake")
	}
	if serverWriter == nil {
		t.Fatalf("expected server writer")
	}
	defer serverWriter.(io.Closer).Close()
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

// TestTCPWriterVerboseSendsGreeting verifies the expected behavior.
func TestTCPWriterVerboseSendsGreeting(t *testing.T) {
	s := snapshotDoGlobals()
	t.Cleanup(func() { restoreDoGlobals(s) })

	Verbose = true
	client, ready := startTCPWriterTest(t)
	defer client.Close()

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

	select {
	case writer := <-ready:
		if writer == nil {
			t.Fatal("expected server writer")
		}
		assert.NoError(t, writer.(io.Closer).Close())
	case <-time.After(5 * time.Second):
		t.Fatal("TCP writer did not finish its greeting")
	}
}
