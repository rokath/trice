// SPDX-License-Identifier: MIT

package receiver

import (
	"bytes"
	"errors"
	"fmt"
	"io"
	"net"
	"testing"
	"time"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// listenTCP4Test creates a real local listener without a fixed port. Every
// accept and transfer has a deadline, including assertion-failure cleanup.
func listenTCP4Test(t *testing.T) *net.TCPListener {
	t.Helper()
	listener, err := net.ListenTCP("tcp4", &net.TCPAddr{IP: net.IPv4(127, 0, 0, 1)})
	require.NoError(t, err)
	t.Cleanup(func() { _ = listener.Close() })
	require.NoError(t, listener.SetDeadline(time.Now().Add(5*time.Second)))
	return listener
}

// serveTCP4Test handles exactly the connection under test and joins its server
// before the next test can restore globals or reuse other fixture state.
func serveTCP4Test(t *testing.T, listener *net.TCPListener, handle func(net.Conn) error) {
	t.Helper()
	done := make(chan error, 1)
	go func() {
		connection, err := listener.Accept()
		if err == nil {
			err = connection.SetDeadline(time.Now().Add(5 * time.Second))
			if err == nil {
				err = handle(connection)
			}
			// Publish completion only after the server socket is released.
			if closeErr := connection.Close(); err == nil {
				err = closeErr
			}
		}
		done <- err
	}()
	t.Cleanup(func() {
		_ = listener.Close()
		select {
		case err := <-done:
			if !errors.Is(err, net.ErrClosed) {
				assert.NoError(t, err)
			}
		case <-time.After(6 * time.Second):
			t.Error("TCP receiver fixture did not stop")
		}
	})
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

	r := newFileReader(nil, fs, "trace.bin")
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

// TestNewReadWriteCloserVerboseFileDefaults verifies the default FILE logging path.
func TestNewReadWriteCloserVerboseFileDefaults(t *testing.T) {
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, fs.WriteFile(DefaultFileArgs, []byte("xy"), 0o644))

	savedVerbose := Verbose
	Verbose = true
	t.Cleanup(func() { Verbose = savedVerbose })

	var out bytes.Buffer
	rc, err := NewReadWriteCloser(&out, fs, false, "FILE", "default")
	require.NoError(t, err)
	require.IsType(t, &file{}, rc)
	require.NoError(t, rc.Close())

	assert.Contains(t, out.String(), "Assigning default arguments for port FILE")
	assert.Contains(t, out.String(), "PortArguments= trices.raw")
	assert.Contains(t, out.String(), "Closing file trices.raw")
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

// countedResource records ownership, including close failures. Separate
// instances represent the input, owned binary file, and borrowed debug sink.
type countedResource struct {
	stubReadWriteCloser
	closes   int
	closeErr error
}

func (r *countedResource) Close() error { r.closes++; return r.closeErr }

// TestWrapperChainClosesOwnedResourcesOnce ensures cleanup reaches the real
// source and logfile through both wrappers, including failure and repeat paths.
func TestWrapperChainClosesOwnedResourcesOnce(t *testing.T) {
	inputErr := errors.New("input close failed")
	fileErr := errors.New("binary file close failed")
	for _, fail := range []bool{false, true} {
		t.Run(fmt.Sprintf("close_errors=%t", fail), func(t *testing.T) {
			input, file, diagnostics := &countedResource{}, &countedResource{}, &countedResource{}
			if fail {
				input.closeErr, file.closeErr = inputErr, fileErr
			}
			viewer := NewBytesViewer(diagnostics, input)
			logger := &binaryLogger{r: viewer, w: file}
			for range 2 {
				err := logger.Close()
				if fail {
					assert.ErrorIs(t, err, inputErr)
					assert.ErrorIs(t, err, fileErr)
				} else {
					assert.NoError(t, err)
				}
			}
			_ = viewer.Close()
			assert.Equal(t, 1, input.closes)
			assert.Equal(t, 1, file.closes)
			assert.Zero(t, diagnostics.closes, "stderr/stdout is borrowed")
		})
	}
}

// failingWriter returns the configured error for every binary log write.
type failingWriter struct {
	err error
}

// Write implements io.Writer for binary logger error propagation tests.
func (w failingWriter) Write([]byte) (int, error) { return 0, w.err }

// TestNewBinaryLoggerAutoFileNameAndRead verifies auto log naming and binary mirroring.
func TestNewBinaryLoggerAutoFileNameAndRead(t *testing.T) {
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	source := &stubReadWriteCloser{readData: []byte{0x01, 0x02, 0x03}}
	previous := BinaryLogfileName
	savedVerbose := Verbose
	BinaryLogfileName = "auto"
	Verbose = true
	t.Cleanup(func() {
		BinaryLogfileName = previous
		Verbose = savedVerbose
	})

	var out bytes.Buffer
	logger := NewBinaryLogger(&out, fs, source)
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
	assert.Contains(t, out.String(), "Writing trice input to binary logfile")
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

// TestNewBytesViewerReadSkipsPureEOF verifies that no debug output is emitted for an immediate EOF.
func TestNewBytesViewerReadSkipsPureEOF(t *testing.T) {
	source := &stubReadWriteCloser{readErr: io.EOF}
	var out bytes.Buffer

	viewer := NewBytesViewer(&out, source)
	buf := make([]byte, 8)
	n, err := viewer.Read(buf)
	assert.Equal(t, io.EOF, err)
	assert.Zero(t, n)
	assert.Empty(t, out.String())
}

// TestBinaryLoggerReadSkipsPureEOF verifies that EOF-only reads do not append to the logfile.
func TestBinaryLoggerReadSkipsPureEOF(t *testing.T) {
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	source := &stubReadWriteCloser{readErr: io.EOF}
	savedLogfileName := BinaryLogfileName
	BinaryLogfileName = "trace.bin"
	t.Cleanup(func() { BinaryLogfileName = savedLogfileName })

	logger := NewBinaryLogger(io.Discard, fs, source)
	buf := make([]byte, 8)
	n, err := logger.Read(buf)
	assert.Equal(t, io.EOF, err)
	assert.Zero(t, n)

	data, readErr := fs.ReadFile("trace.bin")
	require.NoError(t, readErr)
	assert.Empty(t, data)
}

// TestBinaryLoggerPreservesPartialInputAndReportsWriteErrors verifies that
// received bytes are retained even when the source reports truncation, while
// storage failures are returned to the caller.
func TestBinaryLoggerPreservesPartialInputAndReportsWriteErrors(t *testing.T) {
	t.Run("partial input", func(t *testing.T) {
		fs := &afero.Afero{Fs: afero.NewMemMapFs()}
		source := &stubReadWriteCloser{readData: []byte{0xde, 0xad}, readErr: io.ErrUnexpectedEOF}
		savedLogfileName := BinaryLogfileName
		BinaryLogfileName = "partial.bin"
		t.Cleanup(func() { BinaryLogfileName = savedLogfileName })

		logger := NewBinaryLogger(io.Discard, fs, source)
		buf := make([]byte, 8)
		n, err := logger.Read(buf)
		assert.Equal(t, 2, n)
		assert.ErrorIs(t, err, io.ErrUnexpectedEOF)

		logged, readErr := fs.ReadFile("partial.bin")
		require.NoError(t, readErr)
		assert.Equal(t, []byte{0xde, 0xad}, logged)
	})

	t.Run("write failure", func(t *testing.T) {
		writeErr := errors.New("storage unavailable")
		logger := &binaryLogger{
			r: &stubReadWriteCloser{readData: []byte{0x01}},
			w: failingWriter{err: writeErr},
		}

		n, err := logger.Read(make([]byte, 8))
		assert.Equal(t, 1, n)
		assert.ErrorIs(t, err, writeErr)
		assert.ErrorContains(t, err, "write binary logfile")
	})
}

// TestBinaryLoggerAppendsToExistingFile protects the documented append mode.
func TestBinaryLoggerAppendsToExistingFile(t *testing.T) {
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, fs.WriteFile("append.bin", []byte{0x01, 0x02}, 0o600))
	savedLogfileName := BinaryLogfileName
	BinaryLogfileName = "append.bin"
	t.Cleanup(func() { BinaryLogfileName = savedLogfileName })

	logger := NewBinaryLogger(io.Discard, fs, &stubReadWriteCloser{readData: []byte{0x03, 0x04}})
	_, err := logger.Read(make([]byte, 8))
	require.NoError(t, err)

	logged, err := fs.ReadFile("append.bin")
	require.NoError(t, err)
	assert.Equal(t, []byte{0x01, 0x02, 0x03, 0x04}, logged)
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

// TestNewReadWriteCloserHexAndDecAliases verifies the readable alias ports.
func TestNewReadWriteCloserHexAndDecAliases(t *testing.T) {
	hexRC, err := NewReadWriteCloser(nil, nil, false, "HEX", "0a 0b")
	require.NoError(t, err)
	defer hexRC.Close()

	decRC, err := NewReadWriteCloser(nil, nil, false, "DEC", "10 11")
	require.NoError(t, err)
	defer decRC.Close()

	buf := make([]byte, 4)
	n, err := hexRC.Read(buf)
	require.NoError(t, err)
	assert.Equal(t, []byte{0x0a, 0x0b}, buf[:n])

	n, err = decRC.Read(buf)
	require.NoError(t, err)
	assert.Equal(t, []byte{10, 11}, buf[:n])
}

// TestNewReadWriteCloserDefaultDumpAndBuffer verifies the default argument
// paths for the in-memory ports and keeps their verbose logging covered.
func TestNewReadWriteCloserDefaultDumpAndBuffer(t *testing.T) {
	savedVerbose := Verbose
	Verbose = true
	t.Cleanup(func() { Verbose = savedVerbose })

	var dumpOut bytes.Buffer
	dumpRC, err := NewReadWriteCloser(&dumpOut, nil, false, "DUMP", "default")
	require.NoError(t, err)
	defer dumpRC.Close()
	assert.Contains(t, dumpOut.String(), "Assigning default arguments for port DUMP")
	assert.Contains(t, dumpOut.String(), "PortArguments=")

	buf := make([]byte, 4)
	n, err := dumpRC.Read(buf)
	assert.Equal(t, io.EOF, err)
	assert.Zero(t, n)

	var bufferOut bytes.Buffer
	bufferRC, err := NewReadWriteCloser(&bufferOut, nil, false, "BUFFER", "default")
	require.NoError(t, err)
	defer bufferRC.Close()
	assert.Contains(t, bufferOut.String(), "Assigning default arguments for port BUFFER")
	assert.Contains(t, bufferOut.String(), "PortArguments= 0 0 0 0")

	n, err = bufferRC.Read(buf)
	require.NoError(t, err)
	assert.Equal(t, []byte{0, 0, 0, 0}, buf[:n])
}

// TestNewReadWriteCloserUDP4Default verifies the default UDP configuration path.
func TestNewReadWriteCloserUDP4Default(t *testing.T) {
	savedVerbose := Verbose
	savedDefaultArgs := DefaultUDP4Args
	Verbose = true
	DefaultUDP4Args = "127.0.0.1:0"
	t.Cleanup(func() {
		Verbose = savedVerbose
		DefaultUDP4Args = savedDefaultArgs
	})

	var out bytes.Buffer
	rc, err := NewReadWriteCloser(&out, nil, false, "UDP4", "default")
	require.NoError(t, err)
	udp, ok := rc.(*udp4)
	require.True(t, ok)
	udp.w = &out
	defer rc.Close()

	assert.Contains(t, out.String(), "Assigning default arguments for port UDP4")
	assert.Contains(t, out.String(), "PortArguments= "+DefaultUDP4Args)

	peer, err := net.DialUDP("udp4", nil, udp.conn.LocalAddr().(*net.UDPAddr))
	require.NoError(t, err)
	defer peer.Close()

	_, err = peer.Write([]byte{0x21, 0x22})
	require.NoError(t, err)

	buf := make([]byte, 4)
	n, err := rc.Read(buf)
	require.NoError(t, err)
	assert.Equal(t, []byte{0x21, 0x22}, buf[:n])
}

// TestUDP4WritePanics verifies the expected behavior.
func TestUDP4WritePanics(t *testing.T) {
	conn := &udp4{}
	assert.PanicsWithValue(t, "udp4.Write not implemented", func() {
		_, _ = conn.Write([]byte{0x01})
	})
}

// TestTCP4WriteAndDefaultArguments verifies both the tcp4 write wrapper and
// the default-address branch in NewReadWriteCloser.
func TestTCP4WriteAndDefaultArguments(t *testing.T) {
	listener := listenTCP4Test(t)
	// Exercise the default-argument branch with a test-local ephemeral endpoint;
	// the production default is restored when this test ends.
	savedDefault := DefaultTCP4Args
	DefaultTCP4Args = listener.Addr().String()
	t.Cleanup(func() { DefaultTCP4Args = savedDefault })

	received := make(chan []byte, 1)
	serveTCP4Test(t, listener, func(conn net.Conn) error {
		buf := make([]byte, 8)
		n, err := conn.Read(buf)
		received <- append([]byte(nil), buf[:n]...)
		return err
	})

	savedVerbose := Verbose
	Verbose = true
	t.Cleanup(func() { Verbose = savedVerbose })

	var out bytes.Buffer
	rc, err := NewReadWriteCloser(&out, nil, false, "TCP4", "default")
	require.NoError(t, err)
	rc.(*tcp4).w = &out
	defer rc.Close()
	require.NoError(t, rc.(*tcp4).conn.SetDeadline(time.Now().Add(5*time.Second)))

	assert.Contains(t, out.String(), "Assigning default arguments for port TCP4")
	assert.Contains(t, out.String(), "PortArguments= "+DefaultTCP4Args)

	n, err := rc.Write([]byte{0x31, 0x32, 0x33})
	require.NoError(t, err)
	assert.Equal(t, 3, n)
	select {
	case data := <-received:
		assert.Equal(t, []byte{0x31, 0x32, 0x33}, data)
	case <-time.After(5 * time.Second):
		t.Error("TCP receiver fixture did not receive the written bytes")
	}
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

// TestBufferCloseResetsContent verifies the expected behavior.
func TestBufferCloseResetsContent(t *testing.T) {
	buf := &buffer{}
	_, err := buf.Write([]byte("abc"))
	require.NoError(t, err)

	assert.NoError(t, buf.Close())
	assert.Zero(t, buf.Len())
}

// TestFileCloseVerbose verifies the verbose close message for file readers.
func TestFileCloseVerbose(t *testing.T) {
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, fs.WriteFile("trace.bin", []byte("abc"), 0o644))
	fh, err := fs.Open("trace.bin")
	require.NoError(t, err)

	savedVerbose := Verbose
	Verbose = true
	t.Cleanup(func() { Verbose = savedVerbose })

	var out bytes.Buffer
	r := &file{w: &out, fn: "trace.bin", fh: fh}
	assert.NoError(t, r.Close())
	assert.Contains(t, out.String(), "Closing file trace.bin")
}

// TestUDP4CloseVerbose verifies the verbose close message for UDP readers.
func TestUDP4CloseVerbose(t *testing.T) {
	conn, err := net.ListenUDP("udp4", &net.UDPAddr{IP: net.ParseIP("127.0.0.1"), Port: 0})
	require.NoError(t, err)

	savedVerbose := Verbose
	Verbose = true
	t.Cleanup(func() { Verbose = savedVerbose })

	var out bytes.Buffer
	r := &udp4{w: &out, conn: conn}
	assert.NoError(t, r.Close())
	assert.Contains(t, out.String(), "Closing udp4 device.")
}

// TestTCP4Receiver tests the NewReadWriteCloser TCP4 functionality.
func TestTCP4Receiver(t *testing.T) {
	listener := listenTCP4Test(t)
	serveTCP4Test(t, listener, func(connection net.Conn) error {
		_, err := connection.Write([]byte{0x01, 0x7f, 0xFF})
		return err
	})
	rc, err := NewReadWriteCloser(nil, nil, false, "TCP4", listener.Addr().String())
	require.NoError(t, err)
	defer rc.Close()
	require.NoError(t, rc.(*tcp4).conn.SetDeadline(time.Now().Add(5*time.Second)))
	b := make([]byte, 3)
	_, err = io.ReadFull(rc, b)
	require.NoError(t, err)
	assert.Equal(t, []byte{0x01, 0x7f, 0xFF}, b)
}

// TestTCP4CloseVerbose verifies the verbose close message for TCP readers.
func TestTCP4CloseVerbose(t *testing.T) {
	listener := listenTCP4Test(t)
	serveTCP4Test(t, listener, func(connection net.Conn) error { return nil })

	client := newTCP4Connection(listener.Addr().String())

	savedVerbose := Verbose
	Verbose = true
	t.Cleanup(func() { Verbose = savedVerbose })

	var out bytes.Buffer
	client.w = &out
	assert.NoError(t, client.Close())
	assert.Contains(t, out.String(), "Closing tcp4 device.")
}
