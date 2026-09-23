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
	weight   int
	names    []string
	colorize func(string) string
}

type emitterSnapshot struct {
	verbose       bool
	hostStamp     string
	prefix        string
	suffix        string
	colorPalette  string
	displayRemote bool
	ipAddr        string
	ipPort        string
	logLevel      string
	allStats      bool
	tagStats      bool
	ban           channelArrayFlag
	pick          channelArrayFlag
	userLabel     ArrayFlag
	tags          []tagSnapshot
	logFlags      int
}

// cloneTags copies the current tag state into a stable snapshot for restoration.
func cloneTags(src []tag) []tagSnapshot {
	dst := make([]tagSnapshot, len(src))
	for i := range src {
		dst[i] = tagSnapshot{
			count:    src[i].count,
			weight:   src[i].weight,
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
			weight:   src[i].weight,
			Names:    append([]string(nil), src[i].names...),
			colorize: src[i].colorize,
		}
	}
}

// snapshotEmitterState captures emitter package globals so tests can restore them afterwards.
func snapshotEmitterState() emitterSnapshot {
	return emitterSnapshot{
		verbose:       Verbose,
		hostStamp:     HostStamp,
		prefix:        Prefix,
		suffix:        Suffix,
		colorPalette:  ColorPalette,
		displayRemote: DisplayRemote,
		ipAddr:        IPAddr,
		ipPort:        IPPort,
		logLevel:      LogLevel,
		allStats:      AllStatistics,
		tagStats:      TagStatistics,
		ban:           append(channelArrayFlag(nil), Ban...),
		pick:          append(channelArrayFlag(nil), Pick...),
		userLabel:     append(ArrayFlag(nil), UserLabel...),
		tags:          cloneTags(Tags),
		logFlags:      log.Flags(),
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
	restoreTags(s.tags)
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

// TestChannelArrayFlagSetCollectsRawSelectors verifies that parsing preserves
// names and empty parts until the complete tag registry is available.
func TestChannelArrayFlagSetCollectsRawSelectors(t *testing.T) {
	var f channelArrayFlag
	require.NoError(t, f.Set("msg::motor"))
	assert.Equal(t, channelArrayFlag{"msg", "", "motor"}, f)
}

// TestResolveFilterSelectorsUsesCompleteTagRegistry verifies that built-in and
// user-defined groups resolve identically after all -ulabel values are known.
func TestResolveFilterSelectorsUsesCompleteTagRegistry(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	UserLabel = ArrayFlag{"motor:350"}
	require.NoError(t, AddUserLabels())
	Pick = channelArrayFlag{"motor", "msg", "M"}
	Ban = nil
	LogLevel = "M"
	require.NoError(t, ResolveFilterSelectors())

	assert.Contains(t, []string(Pick), "motor")
	for _, alias := range tagVariants("msg") {
		assert.Contains(t, []string(Pick), alias)
	}
	assert.Equal(t, len(tagVariants("msg"))+1, len(Pick))
	messageThreshold, err := logLevelWeight(LogLevel)
	require.NoError(t, err)
	assert.Equal(t, 500, messageThreshold)
}

// TestResolveFilterSelectorsRejectsInvalidValues verifies understandable
// errors for malformed selectors and levels without partially resolving state.
func TestResolveFilterSelectorsRejectsInvalidValues(t *testing.T) {
	tests := []struct {
		name     string
		ban      channelArrayFlag
		pick     channelArrayFlag
		logLevel string
		contains string
	}{
		{name: "unknown pick", pick: channelArrayFlag{"missing"}, logLevel: "all", contains: "unknown tag"},
		{name: "empty pick", pick: channelArrayFlag{""}, logLevel: "all", contains: "empty tag name"},
		{name: "double separator", ban: channelArrayFlag{"err", "", "wrn"}, logLevel: "all", contains: "empty tag name"},
		{name: "empty level", logLevel: "", contains: "level is empty"},
		{name: "negative level", logLevel: "-1", contains: "range 0..999"},
		{name: "large level", logLevel: "1000", contains: "range 0..999"},
		{name: "unknown level", logLevel: "missing", contains: "unknown tag"},
		{name: "fractional level", logLevel: "1.5", contains: "numeric weight"},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			s := snapshotEmitterState()
			t.Cleanup(func() { restoreEmitterState(s) })
			UserLabel = nil
			require.NoError(t, AddUserLabels())
			Ban = append(channelArrayFlag(nil), tt.ban...)
			Pick = append(channelArrayFlag(nil), tt.pick...)
			LogLevel = tt.logLevel
			beforeBan := append(channelArrayFlag(nil), Ban...)
			beforePick := append(channelArrayFlag(nil), Pick...)

			err := ResolveFilterSelectors()
			require.Error(t, err)
			assert.Contains(t, err.Error(), tt.contains)
			assert.Equal(t, beforeBan, Ban)
			assert.Equal(t, beforePick, Pick)
		})
	}
}

// TestResolveFilterSelectorsAcceptsLevelAliasesAndBoundaries verifies all
// supported threshold forms and the inclusive numeric range.
func TestResolveFilterSelectorsAcceptsLevelAliasesAndBoundaries(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = nil
	require.NoError(t, AddUserLabels())

	for _, level := range []string{"all", "off", "0", "999", "MESSAGE", "msg", "M"} {
		LogLevel = level
		Ban = nil
		Pick = nil
		assert.NoError(t, ResolveFilterSelectors(), level)
	}
	for _, level := range []string{"MESSAGE", "msg", "M"} {
		weight, err := logLevelWeight(level)
		require.NoError(t, err)
		assert.Equal(t, 500, weight)
	}
}

// TestSpecialPickAndBanSelectors verifies that all denotes the complete set
// and off denotes the empty set for either selector type.
func TestSpecialPickAndBanSelectors(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = nil
	require.NoError(t, AddUserLabels())
	line := []byte("msg:hello")
	tests := []struct {
		name string
		ban  channelArrayFlag
		pick channelArrayFlag
		want int
	}{
		{name: "pick all", pick: channelArrayFlag{"all"}, want: len(line)},
		{name: "pick off", pick: channelArrayFlag{"off"}, want: 0},
		{name: "ban all", ban: channelArrayFlag{"all"}, want: 0},
		{name: "ban off", ban: channelArrayFlag{"off"}, want: len(line)},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			Ban = append(channelArrayFlag(nil), tt.ban...)
			Pick = append(channelArrayFlag(nil), tt.pick...)
			LogLevel = "all"
			require.NoError(t, ResolveFilterSelectors())
			assert.Equal(t, tt.want, BanOrPickFilter(line))
		})
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

// tagWeightsByAlias captures registry membership and weights without depending
// on table position or function identity.
func tagWeightsByAlias(tags []tag) map[string]int {
	weights := make(map[string]int)
	for _, group := range tags {
		for _, alias := range group.Names {
			weights[alias] = group.weight
		}
	}
	return weights
}

// TestDefaultTagWeights protects the documented built-in priority policy.
func TestDefaultTagWeights(t *testing.T) {
	expected := map[string]int{
		"FATAL": 790, "CRITICAL": 780, "EMERGENCY": 770,
		"ERROR": 760, "WARNING": 750, "ATTENTION": 740,
		"INFO": 500, "DEBUG": 200, "TRACE": 100,
		"TIME": 500, "MESSAGE": 500, "READ": 500, "WRITE": 500,
		"RECEIVE": 500, "TRANSMIT": 500, "DIAG": 500,
		"INTERRUPT": 500, "SIGNAL": 500, "TEST": 500,
		"DEFAULT": 500, "untagged": 500, "NOTICE": 600, "ALERT": 760,
		"ASSERT": 760, "ALARM": 760, "CYCLE_ERROR": 0,
		"VERBOSE": 50, "CONFIG": 500, "MICROSECOND": 500,
		"MILLISECOND": 500, "SECOND": 500, "DELTATIME": 500,
	}

	for canonical, weight := range expected {
		actual, err := TagWeight(canonical)
		require.NoError(t, err)
		assert.Equal(t, weight, actual, canonical)
	}
}

// TestUntaggedIsAnIndependentBuiltInGroup verifies its fixed default, user
// override, and separation from later INFO weight changes.
func TestUntaggedIsAnIndependentBuiltInGroup(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	UserLabel = ArrayFlag{"INFO:650", "untagged:150"}
	require.NoError(t, AddUserLabels())
	assert.Empty(t, duplicateTagAliases(Tags))
	assert.Equal(t, len(defaultTags), len(Tags))
	weight, err := TagWeight("untagged")
	require.NoError(t, err)
	assert.Equal(t, 150, weight)

	UserLabel = ArrayFlag{"INFO:650"}
	require.NoError(t, AddUserLabels())
	weight, err = TagWeight("untagged")
	require.NoError(t, err)
	assert.Equal(t, 500, weight)
}

// TestNormalizeApplicationTag preserves the complete original text and adds
// exactly one reserved prefix only for absent or unknown template tags.
func TestNormalizeApplicationTag(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })
	UserLabel = ArrayFlag{"motor"}
	require.NoError(t, AddUserLabels())

	tests := []struct {
		name      string
		candidate string
		text      string
		want      string
	}{
		{name: "missing", text: "Hello", want: "untagged:Hello"},
		{name: "empty prefix", text: ":Hello", want: "untagged::Hello"},
		{name: "unknown typo", candidate: "mgs", text: "mgs:blah", want: "untagged:mgs:blah"},
		{name: "normal text colon", candidate: "12", text: "12:34", want: "untagged:12:34"},
		{name: "explicit untagged", candidate: "untagged", text: "untagged:Hello", want: "untagged:Hello"},
		{name: "built in", candidate: "msg", text: "msg:Hello", want: "msg:Hello"},
		{name: "user tag", candidate: "motor", text: "motor:running", want: "motor:running"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			buffer := make([]byte, len(tt.text), len(tt.text)+len("untagged:"))
			copy(buffer, tt.text)
			assert.Equal(t, tt.want, string(NormalizeApplicationTag(buffer, tt.candidate)))
		})
	}

	// A full buffer exercises the safe fallback without changing semantics.
	full := []byte("plain")
	assert.Equal(t, "untagged:plain", string(NormalizeApplicationTag(full, "")))
}

// TestAddUserLabelsAppliesWeightsWithoutDuplicateGroups verifies aliases,
// repeated options, the final INFO default, and isolation between commands.
func TestAddUserLabelsAppliesWeightsWithoutDuplicateGroups(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	UserLabel = ArrayFlag{
		"msg:150", "M:600", "msg",
		"motor", "INFO:550", "afterInfo",
		"sensor:150", "sensor:175", "zero:0", "maximum:999", "µs",
	}
	require.NoError(t, AddUserLabels())
	assert.Empty(t, duplicateTagAliases(Tags))
	assert.Equal(t, len(defaultTags)+5, len(Tags))

	for _, alias := range []string{"MESSAGE", "msg", "M"} {
		weight, err := TagWeight(alias)
		require.NoError(t, err)
		assert.Equal(t, 600, weight)
	}
	motorWeight, err := TagWeight("motor")
	require.NoError(t, err)
	assert.Equal(t, 550, motorWeight)
	afterInfoWeight, err := TagWeight("afterInfo")
	require.NoError(t, err)
	assert.Equal(t, 550, afterInfoWeight)
	sensorWeight, err := TagWeight("sensor")
	require.NoError(t, err)
	assert.Equal(t, 175, sensorWeight)
	zeroWeight, err := TagWeight("zero")
	require.NoError(t, err)
	assert.Equal(t, 0, zeroWeight)
	maximumWeight, err := TagWeight("maximum")
	require.NoError(t, err)
	assert.Equal(t, 999, maximumWeight)
	microsecondsWeight, err := TagWeight("µs")
	require.NoError(t, err)
	assert.Equal(t, 500, microsecondsWeight)

	// Table order and palette changes do not alter the stored group priority.
	for left, right := 0, len(Tags)-1; left < right; left, right = left+1, right-1 {
		Tags[left], Tags[right] = Tags[right], Tags[left]
	}
	Tags[tagIndex(Tags, "msg")].colorize = colorizeFATAL
	reorderedWeight, err := TagWeight("msg")
	require.NoError(t, err)
	assert.Equal(t, 600, reorderedWeight)

	UserLabel = ArrayFlag{"next"}
	require.NoError(t, AddUserLabels())
	_, err = TagWeight("motor")
	require.Error(t, err)
	nextWeight, err := TagWeight("next")
	require.NoError(t, err)
	assert.Equal(t, 500, nextWeight)
}

// TestAddUserLabelsRejectsInvalidSpecificationsAtomically verifies that no
// partial registration survives any malformed -ulabel value.
func TestAddUserLabelsRejectsInvalidSpecificationsAtomically(t *testing.T) {
	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	invalid := []string{"", ":150", "tag:", "tagA:tagB", "tag:1:2", "tag:-1", "tag:1000", "123", "all", "off"}
	for _, specification := range invalid {
		t.Run(specification, func(t *testing.T) {
			UserLabel = nil
			require.NoError(t, AddUserLabels())
			before := tagWeightsByAlias(Tags)

			UserLabel = ArrayFlag{"valid:42", specification}
			require.Error(t, AddUserLabels())
			assert.Equal(t, before, tagWeightsByAlias(Tags))
		})
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

// TestScDisplayServerStartAndShutdown verifies two complete server lifecycles.
// Running twice in one process guards against accidental use of net/rpc's
// package-global server registry, which cannot register DisplayServer twice.
func TestScDisplayServerStartAndShutdown(t *testing.T) {
	requireWindowsTCPTestsEnabled(t)

	s := snapshotEmitterState()
	t.Cleanup(func() { restoreEmitterState(s) })

	for cycle := 1; cycle <= 2; cycle++ {
		addr := pickLoopbackAddr(t)
		host, port, err := net.SplitHostPort(addr)
		if err != nil {
			t.Fatalf("cycle %d: SplitHostPort failed: %v", cycle, err)
		}
		IPAddr = host
		IPPort = port
		ColorPalette = "none"

		var serverOut bytes.Buffer
		errCh := make(chan error, 1)
		go func() {
			errCh <- ScDisplayServer(&serverOut)
		}()

		waitForTCPListener(t, addr)

		if err := ScShutdownRemoteDisplayServer(io.Discard, 0, host, port); err != nil {
			t.Fatalf("cycle %d: shutdown rpc failed: %v", cycle, err)
		}

		select {
		case err := <-errCh:
			if err == nil {
				t.Fatalf("cycle %d: expected listener close error from ScDisplayServer", cycle)
			}
		case <-time.After(3 * time.Second):
			t.Fatalf("cycle %d: ScDisplayServer did not return after shutdown", cycle)
		}

		if !strings.Contains(serverOut.String(), "displayServer @ "+addr) {
			t.Fatalf("cycle %d: missing startup output: %q", cycle, serverOut.String())
		}
		if !strings.Contains(serverOut.String(), "displayServer shutdown") {
			t.Fatalf("cycle %d: missing shutdown output: %q", cycle, serverOut.String())
		}
	}
}
