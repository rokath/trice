// SPDX-License-Identifier: MIT

package emitter

import (
	"bytes"
	"io"
	"log"
	"net"
	"net/rpc"
	"os"
	"runtime"
	"strings"
	"testing"
	"time"
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

type rpcWriteLineStub struct {
	lines chan []string
}

func (s *rpcWriteLineStub) WriteLine(line []string, reply *int64) error {
	cp := append([]string(nil), line...)
	s.lines <- cp
	*reply = int64(len(line))
	return nil
}

type tagSnapshot struct {
	count    int
	names    []string
	colorize func(string) string
}

type emitterSnapshot struct {
	verbose         bool
	hostStamp       string
	prefix          string
	suffix          string
	colorPalette    string
	displayRemote   bool
	ipAddr          string
	ipPort          string
	logLevel        string
	allStats        bool
	tagStats        bool
	ban             channelArrayFlag
	pick            channelArrayFlag
	userLabel       ArrayFlag
	userLabelsAdded bool
	tags            []tagSnapshot
	exit            bool
	logFlags        int
}

// cloneTags copies the current tag state into a stable snapshot for restoration.
func cloneTags(src []tag) []tagSnapshot {
	dst := make([]tagSnapshot, len(src))
	for i := range src {
		dst[i] = tagSnapshot{
			count:    src[i].count,
			names:    append([]string(nil), src[i].Names...),
			colorize: src[i].colorize,
		}
	}
	return dst
}

// restoreTags restores tag state from a saved snapshot.
func restoreTags(src []tagSnapshot) {
	Tags = make([]tag, len(src))
	for i := range src {
		Tags[i] = tag{
			count:    src[i].count,
			Names:    append([]string(nil), src[i].names...),
			colorize: src[i].colorize,
		}
	}
}

// snapshotEmitterState captures emitter package globals so tests can restore them afterwards.
func snapshotEmitterState() emitterSnapshot {
	return emitterSnapshot{
		verbose:         Verbose,
		hostStamp:       HostStamp,
		prefix:          Prefix,
		suffix:          Suffix,
		colorPalette:    ColorPalette,
		displayRemote:   DisplayRemote,
		ipAddr:          IPAddr,
		ipPort:          IPPort,
		logLevel:        LogLevel,
		allStats:        AllStatistics,
		tagStats:        TagStatistics,
		ban:             append(channelArrayFlag(nil), Ban...),
		pick:            append(channelArrayFlag(nil), Pick...),
		userLabel:       append(ArrayFlag(nil), UserLabel...),
		userLabelsAdded: userLabelsAdded,
		tags:            cloneTags(Tags),
		exit:            exit,
		logFlags:        log.Flags(),
	}
}

// restoreEmitterState restores emitter package globals from a saved snapshot.
func restoreEmitterState(s emitterSnapshot) {
	Verbose = s.verbose
	HostStamp = s.hostStamp
	Prefix = s.prefix
	Suffix = s.suffix
	ColorPalette = s.colorPalette
	DisplayRemote = s.displayRemote
	IPAddr = s.ipAddr
	IPPort = s.ipPort
	LogLevel = s.logLevel
	AllStatistics = s.allStats
	TagStatistics = s.tagStats
	Ban = append(channelArrayFlag(nil), s.ban...)
	Pick = append(channelArrayFlag(nil), s.pick...)
	UserLabel = append(ArrayFlag(nil), s.userLabel...)
	userLabelsAdded = s.userLabelsAdded
	restoreTags(s.tags)
	exit = s.exit
	log.SetFlags(s.logFlags)
}

// pickLoopbackAddr returns an available loopback address for TCP-based tests.
func pickLoopbackAddr(t *testing.T) string {
	t.Helper()
	l, err := net.Listen("tcp", "127.0.0.1:0")
	if err != nil {
		t.Fatalf("failed to reserve loopback port: %v", err)
	}
	addr := l.Addr().String()
	_ = l.Close()
	return addr
}

// waitForTCPListener waits until the TCP listener is ready to accept connections.
func waitForTCPListener(t *testing.T, addr string) {
	t.Helper()
	deadline := time.Now().Add(2 * time.Second)
	for {
		c, err := net.DialTimeout("tcp", addr, 200*time.Millisecond)
		if err == nil {
			_ = c.Close()
			return
		}
		if time.Now().After(deadline) {
			t.Fatalf("listener %s not reachable: %v", addr, err)
		}
		time.Sleep(25 * time.Millisecond)
	}
}

// TestAppendIfMissing verifies the expected behavior.
func TestAppendIfMissing(t *testing.T) {
	got := appendIfMissing([]string{"a", "b"}, "c")
	if len(got) != 3 || got[2] != "c" {
		t.Fatalf("unexpected append result: %#v", got)
	}

	got = appendIfMissing([]string{"a", "b"}, "a")
	if len(got) != 2 {
		t.Fatalf("duplicate must not be appended: %#v", got)
	}
}

// TestChannelArrayFlagSetAddsUniqueVariants verifies the expected behavior.
func TestChannelArrayFlagSetAddsUniqueVariants(t *testing.T) {
	var f channelArrayFlag
	if err := f.Set("msg:msg:inf"); err != nil {
		t.Fatalf("set failed: %v", err)
	}
	seen := map[string]bool{}
	for _, s := range f {
		if seen[s] {
			t.Fatalf("duplicate element found: %q", s)
		}
		seen[s] = true
	}
	if !seen["msg"] || !seen["inf"] {
		t.Fatalf("expected canonical aliases in set: %#v", []string(f))
	}
}

// TestArrayFlagSetAndString verifies the expected behavior.
func TestArrayFlagSetAndString(t *testing.T) {
	var f ArrayFlag
	if err := f.Set("first"); err != nil {
		t.Fatalf("set failed: %v", err)
	}
	if err := f.Set("second"); err != nil {
		t.Fatalf("set failed: %v", err)
	}
	if len(f) != 2 || f[0] != "first" || f[1] != "second" {
		t.Fatalf("unexpected ArrayFlag content: %#v", []string(f))
	}
	if got := f.String(); got != "" {
		t.Fatalf("unexpected ArrayFlag.String value: %q", got)
	}
}

// TestChannelArrayFlagString verifies the expected behavior.
func TestChannelArrayFlagString(t *testing.T) {
	f := channelArrayFlag{"a", "b"}
	if got := f.String(); got != "[a b]" {
		t.Fatalf("unexpected channelArrayFlag string: %q", got)
	}
}

// TestBanOrPickFilterWrapper verifies the expected behavior.
func TestBanOrPickFilterWrapper(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	Ban = channelArrayFlag{"msg"}
	Pick = nil
	line := []byte("msg:hello")
	if got := BanOrPickFilter(line); got != 0 {
		t.Fatalf("expected banned line to be filtered, got %d", got)
	}
}

// TestBanOrPickFilterUsesFunctionArgs verifies the expected behavior.
func TestBanOrPickFilterUsesFunctionArgs(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	Ban = channelArrayFlag{"ban"}
	Pick = nil

	line := []byte("msg:hello")
	if got := banOrPickFilter(nil, channelArrayFlag{"msg"}, line); got != len(line) {
		t.Fatalf("expected pick parameter to allow line, got %d", got)
	}
}

// TestAddUserLabelsIsIdempotent verifies the expected behavior.
func TestAddUserLabelsIsIdempotent(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	userLabelsAdded = false
	UserLabel = ArrayFlag{"myTag"}
	base := len(Tags)
	AddUserLabels()
	AddUserLabels()
	if len(Tags) != base+1 {
		t.Fatalf("expected exactly one added user tag, got delta %d", len(Tags)-base)
	}
	name, err := FindTagName("myTag")
	if err != nil {
		t.Fatalf("FindTagName failed: %v", err)
	}
	if name != "myTag" {
		t.Fatalf("unexpected canonical user tag name: %q", name)
	}
}

// TestColorizeGlobalFunctionPaths verifies the expected behavior.
func TestColorizeGlobalFunctionPaths(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	LogLevel = "off"
	if got := Colorize("msg:hello"); got != "" {
		t.Fatalf("expected suppressed output for log level off, got %q", got)
	}

	LogLevel = "all"
	ColorPalette = "none"
	if got := Colorize("msg:hello"); got != "hello" {
		t.Fatalf("expected prefix stripped in none palette, got %q", got)
	}

	ColorPalette = "off"
	if got := Colorize("msg:hello"); got != "msg:hello" {
		t.Fatalf("expected unchanged output in off palette, got %q", got)
	}
}

// TestPrintTagStatisticsGuardAndContent verifies the expected behavior.
func TestPrintTagStatisticsGuardAndContent(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	AllStatistics = false
	TagStatistics = false
	var out bytes.Buffer
	PrintTagStatistics(&out)
	if out.Len() != 0 {
		t.Fatalf("expected no output with statistics disabled, got %q", out.String())
	}

	TagStatistics = true
	_ = Colorize("msg:hello")
	PrintTagStatistics(&out)
	if !strings.Contains(out.String(), "Tag Statistics:") {
		t.Fatalf("expected statistics header, got %q", out.String())
	}
}

// TestTagEvents verifies the expected behavior.
func TestTagEvents(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	LogLevel = "all"
	ColorPalette = "none"
	_ = Colorize("msg:hello")
	if TagEvents("msg") <= 0 {
		t.Fatalf("expected msg tag count > 0")
	}
	if TagEvents("__unknown__") != -1 {
		t.Fatalf("expected unknown tag count -1")
	}
}

// TestLineComposerWriteAndTimestampFormats verifies the expected behavior.
func TestLineComposerWriteAndTimestampFormats(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	lw := newCheckDisplay()
	p := newLineComposer(lw)
	p.timestampFormat = "LOCmicro"
	if ts := p.timestamp(); !strings.HasSuffix(ts, "  ") || len(ts) == 0 {
		t.Fatalf("unexpected LOCmicro timestamp: %q", ts)
	}
	p.timestampFormat = "UTCmicro"
	if ts := p.timestamp(); !strings.HasPrefix(ts, "UTC ") {
		t.Fatalf("unexpected UTCmicro timestamp: %q", ts)
	}
	p.timestampFormat = "custom"
	if ts := p.timestamp(); ts != "custom " {
		t.Fatalf("unexpected custom timestamp: %q", ts)
	}

	p.timestampFormat = "off"
	p.prefix = "["
	p.suffix = "]"
	if n, err := p.Write([]byte("x\n")); err != nil || n != 2 {
		t.Fatalf("Write returned (%d, %v)", n, err)
	}
	if len(lw.lines) != 1 || lw.lines[0] != "[x]" {
		t.Fatalf("unexpected line output: %#v", lw.lines)
	}
}

// TestDisplayServerRPCMethodsDirect verifies the expected behavior.
func TestDisplayServerRPCMethodsDirect(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	var out bytes.Buffer
	srv := &DisplayServer{Display: *newColorDisplay(&out, "none")}

	var reply int64
	if err := srv.ColorPalette([]string{"none"}, &reply); err != nil {
		t.Fatalf("ColorPalette failed: %v", err)
	}
	if reply != 0 || ColorPalette != "none" {
		t.Fatalf("unexpected color palette reply/state: %d %q", reply, ColorPalette)
	}

	const flags = int64(0)
	if err := srv.LogSetFlags([]int64{flags}, &reply); err != nil {
		t.Fatalf("LogSetFlags failed: %v", err)
	}
	if reply != flags {
		t.Fatalf("unexpected LogSetFlags reply: %d", reply)
	}

	if err := srv.WriteLine([]string{"msg:hello"}, &reply); err != nil {
		t.Fatalf("WriteLine failed: %v", err)
	}
	if reply != 1 {
		t.Fatalf("unexpected WriteLine reply: %d", reply)
	}
	if !strings.Contains(out.String(), "hello") {
		t.Fatalf("expected output to contain payload, got %q", out.String())
	}
}

// TestRemoteDisplayConnectAlreadyConnected verifies the expected behavior.
func TestRemoteDisplayConnectAlreadyConnected(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	Verbose = true
	var out bytes.Buffer
	p := &remoteDisplay{
		w:      &out,
		Err:    nil,
		IPAddr: "127.0.0.1",
		IPPort: "0",
		PtrRPC: &rpc.Client{},
	}
	p.connect()
	if !strings.Contains(out.String(), "already connected") {
		t.Fatalf("expected already connected message, got %q", out.String())
	}
}

// TestRemoteDisplayWriteLineOverRPC verifies the expected behavior.
func TestRemoteDisplayWriteLineOverRPC(t *testing.T) {
	requireWindowsTCPTestsEnabled(t)

	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	l, err := net.Listen("tcp", "127.0.0.1:0")
	if err != nil {
		t.Fatalf("listen failed: %v", err)
	}
	defer l.Close()
	addr := l.Addr().String()
	host, port, err := net.SplitHostPort(addr)
	if err != nil {
		t.Fatalf("SplitHostPort failed: %v", err)
	}
	stub := &rpcWriteLineStub{lines: make(chan []string, 1)}
	srv := rpc.NewServer()
	if err := srv.RegisterName("DisplayServer", stub); err != nil {
		t.Fatalf("RegisterName failed: %v", err)
	}
	go func() {
		for {
			c, acceptErr := l.Accept()
			if acceptErr != nil {
				return
			}
			go srv.ServeConn(c)
		}
	}()

	IPAddr = host
	IPPort = port
	waitForTCPListener(t, addr)

	client := newRemoteDisplay(io.Discard, []string{"trice"})
	client.WriteLine([]string{"msg:hello rpc"})
	if client.Err != nil {
		t.Fatalf("remote WriteLine failed: %v", client.Err)
	}

	select {
	case got := <-stub.lines:
		if len(got) != 1 || got[0] != "msg:hello rpc" {
			t.Fatalf("unexpected rpc payload: %#v", got)
		}
	case <-time.After(2 * time.Second):
		t.Fatalf("did not receive rpc WriteLine payload")
	}
}

// TestShowAllColorsSmoke verifies the expected behavior.
func TestShowAllColorsSmoke(t *testing.T) {
	origStdout := os.Stdout
	r, w, err := os.Pipe()
	if err != nil {
		t.Fatalf("os.Pipe failed: %v", err)
	}
	os.Stdout = w
	defer func() { os.Stdout = origStdout }()

	var out bytes.Buffer
	done := make(chan error, 1)
	go func() {
		_, copyErr := io.Copy(&out, r)
		done <- copyErr
	}()

	ShowAllColors()

	_ = w.Close()
	if err := <-done; err != nil {
		t.Fatalf("copy failed: %v", err)
	}
	if out.Len() == 0 {
		t.Fatalf("expected some color matrix output")
	}
}

// TestScDisplayServerStartAndShutdown verifies the expected behavior.
func TestScDisplayServerStartAndShutdown(t *testing.T) {
	requireWindowsTCPTestsEnabled(t)

	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	addr := pickLoopbackAddr(t)
	host, port, err := net.SplitHostPort(addr)
	if err != nil {
		t.Fatalf("SplitHostPort failed: %v", err)
	}
	IPAddr = host
	IPPort = port
	ColorPalette = "none"
	exit = false

	var serverOut bytes.Buffer
	errCh := make(chan error, 1)
	go func() {
		errCh <- ScDisplayServer(&serverOut)
	}()

	waitForTCPListener(t, addr)

	if err := ScShutdownRemoteDisplayServer(io.Discard, 0, host, port); err != nil {
		t.Fatalf("shutdown rpc failed: %v", err)
	}

	select {
	case err := <-errCh:
		if err == nil {
			t.Fatalf("expected listener close error from ScDisplayServer")
		}
	case <-time.After(3 * time.Second):
		t.Fatalf("ScDisplayServer did not return after shutdown")
	}

	if !strings.Contains(serverOut.String(), "displayServer @ "+addr) {
		t.Fatalf("missing startup output: %q", serverOut.String())
	}
	if !strings.Contains(serverOut.String(), "displayServer shutdown") {
		t.Fatalf("missing shutdown output: %q", serverOut.String())
	}
}
