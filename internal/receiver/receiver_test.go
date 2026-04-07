// SPDX-License-Identifier: MIT

package receiver

import (
	"bytes"
	"fmt"
	"io"
	"net"
	"os"
	"runtime"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// requireWindowsTCPTestsEnabled skips the test unless the Windows TCP test environment is enabled.
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

// TestBUFFERReceiver tests the NewReadWriteCloser BUFFER functionality.
func TestBUFFERReceiver(t *testing.T) {
	rc, err := NewReadWriteCloser(nil, nil, false, "BUFFER", "7 123 44")
	assert.Nil(t, err)
	b := make([]byte, 4)
	n, err := rc.Read(b)
	assert.Nil(t, err)
	assert.True(t, n == 3)
	assert.True(t, b[0] == 7)
	assert.True(t, b[1] == 123)
	assert.True(t, b[2] == 44)
	n, err = rc.Read(b)
	assert.True(t, n == 0)
	assert.True(t, err == io.EOF)
	assert.Nil(t, rc.Close())
}

// TestDUMPReceiver tests the NewReadWriteCloser DUMP functionality.
func TestDUMPReceiver(t *testing.T) {
	rc, err := NewReadWriteCloser(nil, nil, false, "DUMP", "7B 1A ee,88, 5a")
	assert.Nil(t, err)
	b := make([]byte, 100)
	n, err := rc.Read(b)
	assert.Nil(t, err)
	assert.True(t, n == 5)
	assert.True(t, b[0] == 0x7b)
	assert.True(t, b[4] == 0x5a)
	n, err = rc.Read(b)
	assert.True(t, n == 0)
	assert.True(t, io.EOF == err)
	assert.Nil(t, rc.Close())
}

// TestFILEReceiver tests the NewReadWriteCloser FILE functionality.
func TestFILEReceiver(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewOsFs()}
	fn := "./trices.raw"
	f, err := fSys.Create(fn)
	assert.Nil(t, err)
	d := []byte{115, 111, 109, 101, 10}
	n, err := f.Write(d)
	assert.Nil(t, err)
	assert.True(t, n == len(d))
	rc, err := NewReadWriteCloser(nil, fSys, false, "FILE", fn)
	assert.Nil(t, err)
	b := make([]byte, 100)
	n, err = rc.Read(b)
	assert.Nil(t, err)
	assert.True(t, n == len(d))
	assert.True(t, b[0] == 115)
	assert.True(t, b[4] == 10)
	rc.Close()
}

// TestSpaceStringsBuilder verifies the expected behavior.
func TestSpaceStringsBuilder(t *testing.T) {
	got := spaceStringsBuilder(" a b\tc\n")
	if got != "abc" {
		t.Fatalf("unexpected result: %q", got)
	}
}

// TestRemoveWhitespaces verifies the expected behavior.
func TestRemoveWhitespaces(t *testing.T) {
	if removeWhitespaces("x y z") != "xyz" {
		t.Fatalf("removeWhitespaces failed")
	}
}

// TestScanHexDump verifies the expected behavior.
func TestScanHexDump(t *testing.T) {
	data, err := scanHexDump("09 a1 fe")
	if err != nil {
		t.Fatalf("scanHexDump: %v", err)
	}
	if !bytes.Equal(data, []byte{0x09, 0xa1, 0xfe}) {
		t.Fatalf("unexpected data: %#v", data)
	}
}

// TestScanBytes verifies the expected behavior.
func TestScanBytes(t *testing.T) {
	got := scanBytes("10 11,12\n13")
	expected := []byte{10, 11, 12, 13}
	if !bytes.Equal(got, expected) {
		t.Fatalf("unexpected bytes: %#v", got)
	}
}

// TestFileReadWriterDiscard verifies FILE readers read existing content and discard writes.
func TestFileReadWriterDiscard(t *testing.T) {
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, fs.WriteFile("trace.bin", []byte("abc"), 0o644))

	r := newFileReader(fs, "trace.bin")
	buf := make([]byte, 3)
	n, err := r.Read(buf)
	require.NoError(t, err)
	assert.Equal(t, "abc", string(buf[:n]))

	n, err = r.Write([]byte("ignored"))
	require.NoError(t, err)
	assert.Equal(t, len("ignored"), n)

	data, err := fs.ReadFile("trace.bin")
	require.NoError(t, err)
	assert.Equal(t, "abc", string(data))
}

// TestNewReadWriteCloserUsesDefaultArguments verifies default aliases produce readable closers.
func TestNewReadWriteCloserUsesDefaultArguments(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, fSys.WriteFile(DefaultFileArgs, []byte("xy"), 0o644))

	rc, err := NewReadWriteCloser(nil, fSys, false, "FILEBUFFER", "default")
	require.NoError(t, err)
	defer rc.Close()

	buf := make([]byte, 4)
	n, err := rc.Read(buf)
	require.NoError(t, err)
	assert.Equal(t, "xy", string(buf[:n]))
}

type stubReadWriteCloser struct {
	readData []byte
	readErr  error
	writes   [][]byte
}

func (s *stubReadWriteCloser) Read(buf []byte) (int, error) {
	n := copy(buf, s.readData)
	return n, s.readErr
}

func (s *stubReadWriteCloser) Write(buf []byte) (int, error) {
	cp := append([]byte(nil), buf...)
	s.writes = append(s.writes, cp)
	return len(buf), nil
}

func (s *stubReadWriteCloser) Close() error { return nil }

// TestNewBinaryLoggerAutoFileNameAndRead verifies auto log naming and binary mirroring.
func TestNewBinaryLoggerAutoFileNameAndRead(t *testing.T) {
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	source := &stubReadWriteCloser{readData: []byte{0x01, 0x02, 0x03}}
	previous := BinaryLogfileName
	BinaryLogfileName = "auto"
	t.Cleanup(func() { BinaryLogfileName = previous })

	logger := NewBinaryLogger(io.Discard, fs, source)
	bl, ok := logger.(*binaryLogger)
	require.True(t, ok)

	buf := make([]byte, 8)
	n, err := logger.Read(buf)
	require.NoError(t, err)
	assert.Equal(t, []byte{0x01, 0x02, 0x03}, buf[:n])

	files, err := afero.ReadDir(fs, ".")
	require.NoError(t, err)
	require.Len(t, files, 1)
	assert.Contains(t, files[0].Name(), "_trice.bin")

	logged, err := afero.ReadFile(fs, files[0].Name())
	require.NoError(t, err)
	assert.Equal(t, []byte{0x01, 0x02, 0x03}, logged)
	assert.NotNil(t, bl.w)
}

// TestNewBinaryLoggerDisabledReturnsSource verifies disabled logfile settings are pass-through.
func TestNewBinaryLoggerDisabledReturnsSource(t *testing.T) {
	source := &stubReadWriteCloser{}
	previous := BinaryLogfileName
	BinaryLogfileName = "off"
	t.Cleanup(func() { BinaryLogfileName = previous })

	got := NewBinaryLogger(io.Discard, &afero.Afero{Fs: afero.NewMemMapFs()}, source)
	assert.Same(t, source, got)
}

// TestNewBytesViewerReadFormatsHex verifies the debug wrapper prints incoming bytes.
func TestNewBytesViewerReadFormatsHex(t *testing.T) {
	source := &stubReadWriteCloser{readData: []byte{0x0a, 0xbc}}
	var out bytes.Buffer

	viewer := NewBytesViewer(&out, source)
	buf := make([]byte, 8)
	n, err := viewer.Read(buf)
	require.NoError(t, err)
	assert.Equal(t, []byte{0x0a, 0xbc}, buf[:n])
	assert.Equal(t, "Input(0a bc)\n", out.String())
}

// TestUDP4ConnectionReceivesPackets verifies the expected behavior.
func TestUDP4ConnectionReceivesPackets(t *testing.T) {
	conn := newUDPConnection("127.0.0.1:0")
	defer conn.Close()

	peer, err := net.DialUDP("udp4", nil, conn.conn.LocalAddr().(*net.UDPAddr))
	require.NoError(t, err)
	defer peer.Close()

	_, err = peer.Write([]byte{0x11, 0x22, 0x33})
	require.NoError(t, err)

	buf := make([]byte, 8)
	n, err := conn.Read(buf)
	require.NoError(t, err)
	assert.Equal(t, []byte{0x11, 0x22, 0x33}, buf[:n])
}

// TestUDP4WritePanics verifies the expected behavior.
func TestUDP4WritePanics(t *testing.T) {
	conn := &udp4{}
	assert.PanicsWithValue(t, "udp4.Write not implemented", func() {
		_, _ = conn.Write([]byte{0x01})
	})
}

// TestBinaryLoggerWriteAndClose verifies the expected behavior.
func TestBinaryLoggerWriteAndClose(t *testing.T) {
	logger := &binaryLogger{}

	n, err := logger.Write([]byte("ignored"))
	require.NoError(t, err)
	assert.Zero(t, n)
	assert.NoError(t, logger.Close())
}

// TestBytesViewerWriteAndClose verifies the expected behavior.
func TestBytesViewerWriteAndClose(t *testing.T) {
	viewer := &bytesViewer{}

	n, err := viewer.Write([]byte("ignored"))
	require.NoError(t, err)
	assert.Zero(t, n)
	assert.NoError(t, viewer.Close())
}

// TestTCP4Receiver tests the NewReadWriteCloser TCP4 functionality.
func TestTCP4Receiver(t *testing.T) {
	requireWindowsTCPTestsEnabled(t)

	ch := make(chan net.Addr)

	go func() {
		const (
			CONN_TYPE = "tcp4"
			CONN_HOST = "127.0.0.1"
			CONN_PORT = "" // let port be chosen
		)
		// Listen for incoming connections.
		l, err := net.Listen(CONN_TYPE, CONN_HOST+":"+CONN_PORT)
		assert.Nil(t, err)
		ch <- l.Addr()
		// Close the listener when the application closes.
		defer l.Close()
		for {
			// Listen for an incoming connection.
			conn, err := l.Accept()
			assert.Nil(t, err)
			// Handle connections in a new goroutine.
			go handleRequest(conn)
		}
	}()

	addr := <-ch
	s := fmt.Sprint(addr)
	rc, err := NewReadWriteCloser(nil, nil, false, "TCP4", s)
	assert.Nil(t, err)
	b := make([]byte, 100)
	n, err := rc.Read(b)
	assert.Nil(t, err)
	assert.Equal(t, []byte{0x01, 0x7f, 0xFF}, b[:n])
}

// handleRequest handles incoming requests.
func handleRequest(conn net.Conn) {
	// Send a response back to person contacting us.
	conn.Write([]byte{0x01, 0x7f, 0xFF})
	// Close the connection when you're done with it.
	conn.Close()
}
