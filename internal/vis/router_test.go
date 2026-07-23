// SPDX-License-Identifier: MIT

package vis

import (
	"bytes"
	"fmt"
	"io"
	"net"
	"strings"
	"sync"
	"testing"
	"time"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// testFileSystem returns an isolated filesystem for deterministic file-sink tests.
func testFileSystem() *afero.Afero {
	return &afero.Afero{Fs: afero.NewMemMapFs()}
}

// testRecord constructs one otherwise eligible single-line typed TREX record.
func testRecord(
	triceID id.TriceID,
	typeName string,
	format string,
	stamp uint64,
	stampBits int,
	values ...decoder.VisValue,
) decoder.VisRecord {
	record := decoder.VisRecord{
		ID:         triceID,
		Type:       typeName,
		Format:     format,
		Stamp:      stamp,
		StampBits:  stampBits,
		ValueCount: len(values),
		SingleLine: true,
	}
	copy(record.Values[:], values)
	return record
}

// signedValue creates one source-width signed decoder value for tests.
func signedValue(bits int, value int64) decoder.VisValue {
	return decoder.VisValue{Kind: decoder.VisValueSigned, Bits: bits, Signed: value}
}

// unsignedValue creates one source-width unsigned decoder value for tests.
func unsignedValue(bits int, value uint64) decoder.VisValue {
	return decoder.VisValue{Kind: decoder.VisValueUnsigned, Bits: bits, Unsigned: value}
}

// floatValue creates one source-width floating decoder value for tests.
func floatValue(bits int, value float64) decoder.VisValue {
	return decoder.VisValue{Kind: decoder.VisValueFloat, Bits: bits, Float: value}
}

// boolValue creates one source-width Boolean decoder value for tests.
func boolValue(bits int, value bool) decoder.VisValue {
	return decoder.VisValue{Kind: decoder.VisValueBool, Bits: bits, Bool: value}
}

// readTestFile reads a complete in-memory sink and fails the test on filesystem errors.
func readTestFile(t *testing.T, fileSystem *afero.Afero, path string) string {
	t.Helper()
	data, err := fileSystem.ReadFile(path)
	require.NoError(t, err)
	return string(data)
}

// TestRouterEncodesTypedExpressionsAndDropsOnlyAfterSuccess verifies the central record path.
func TestRouterEncodesTypedExpressionsAndDropsOnlyAfterSuccess(t *testing.T) {
	fileSystem := testFileSystem()
	lut := id.TriceIDLookUp{
		17: {
			Type: "TRICE32_4",
			Strg: "msg:a=%f,count=%u,ready=%t,extra=%d\n",
		},
	}
	var diagnostics bytes.Buffer
	router, err := NewRouter(
		&diagnostics,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{`msg:printf("%d,%0.1f,%d,%t\n",ts32/10,v0*2,v1,v2)@out.csv;log=drop`},
		true,
	)
	require.NoError(t, err)
	t.Cleanup(func() { require.NoError(t, router.Close()) })

	record := testRecord(
		17,
		"TRICE32_4",
		"msg:a=%f,count=%u,ready=%t,extra=%d\n",
		125,
		32,
		floatValue(32, 1.5),
		unsignedValue(32, 7),
		boolValue(32, true),
		signedValue(32, -9),
	)
	assert.True(t, router.Process(record, false))
	assert.Equal(t, "12,3.0,7,true\n", readTestFile(t, fileSystem, "out.csv"))
	assert.Empty(t, diagnostics.String())
}

// TestRouterClassifiesHistoricalCandidatesIndependently verifies that one old incompatible ID does not disable a useful rule.
func TestRouterClassifiesHistoricalCandidatesIndependently(t *testing.T) {
	fileSystem := testFileSystem()
	lut := id.TriceIDLookUp{
		1: {Type: "TRICE32_3", Strg: "msg:x=%d,y=%d,z=%d\n"},
		2: {Type: "TRICE32_1", Strg: "msg:legacy=%d\n"},
	}
	var diagnostics bytes.Buffer
	router, err := NewRouter(
		&diagnostics,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{`msg:printf("%d\n",v2)@out.txt`},
		true,
	)
	require.NoError(t, err)
	t.Cleanup(func() { require.NoError(t, router.Close()) })

	assert.True(t, router.rules[0].enabled)
	assert.Equal(t, 1, router.rules[0].compatibleCount)
	assert.Contains(t, diagnostics.String(), "excluded til.json ID 2")

	legacy := testRecord(2, "TRICE32_1", "msg:legacy=%d\n", 0, 0, signedValue(32, 8))
	assert.False(t, router.Process(legacy, false))
	assert.Empty(t, readTestFile(t, fileSystem, "out.txt"))

	current := testRecord(
		1,
		"TRICE32_3",
		"msg:x=%d,y=%d,z=%d\n",
		0,
		0,
		signedValue(32, 1),
		signedValue(32, 2),
		signedValue(32, 3),
	)
	assert.False(t, router.Process(current, false))
	assert.Equal(t, "3\n", readTestFile(t, fileSystem, "out.txt"))
}

// TestRouterAcceptsTheAddressablePrefixOfLargerCandidates verifies that unreferenced later values stay compatible.
func TestRouterAcceptsTheAddressablePrefixOfLargerCandidates(t *testing.T) {
	const valueCount = decoder.VisValueCapacity + 1
	fileSystem := testFileSystem()
	lut := id.TriceIDLookUp{
		1: {
			Type: fmt.Sprintf("TRICE8_%d", valueCount),
			Strg: "msg:" + strings.TrimSpace(strings.Repeat("%d ", valueCount)) + `\n`,
		},
	}
	router, err := NewRouter(
		io.Discard,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{`msg:printf("%d\n",v11)@out.txt`},
		false,
	)
	require.NoError(t, err)
	t.Cleanup(func() { require.NoError(t, router.Close()) })

	values := make([]decoder.VisValue, decoder.VisValueCapacity)
	for i := range values {
		values[i] = signedValue(8, int64(i+1))
	}
	record := testRecord(1, fmt.Sprintf("TRICE8_%d", valueCount), lut[1].Strg, 0, 0, values...)
	assert.False(t, router.Process(record, false))
	assert.Equal(t, "12\n", readTestFile(t, fileSystem, "out.txt"))
}

// TestRouterDisablesRulesWithoutCompatibleCandidates verifies prominent non-fatal startup warnings.
func TestRouterDisablesRulesWithoutCompatibleCandidates(t *testing.T) {
	tests := []struct {
		name     string
		lut      id.TriceIDLookUp
		want     string
		wantAlso string
	}{
		{
			name: "no matching tag",
			lut:  id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: "other:v=%d\n"}},
			want: `no til.json entry matches tag "msg"`,
		},
		{
			name: "matching but incompatible",
			lut: id.TriceIDLookUp{
				4: {Type: "TRICE32_1", Strg: "msg:v=%d\n"},
				9: {Type: "TRICE_S", Strg: "msg:text=%s\n"},
			},
			want:     "candidates: 4 (",
			wantAlso: "9 (",
		},
	}

	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			var diagnostics bytes.Buffer
			router, err := NewRouter(
				&diagnostics,
				testFileSystem(),
				test.lut,
				new(sync.RWMutex),
				[]string{`msg:printf("%d\n",v2)@unused.txt`},
				false,
			)
			require.NoError(t, err)
			t.Cleanup(func() { require.NoError(t, router.Close()) })
			assert.False(t, router.rules[0].enabled)
			assert.Contains(t, diagnostics.String(), "WARNING: -vis rule 1 disabled")
			assert.Contains(t, diagnostics.String(), test.want)
			if test.wantAlso != "" {
				assert.Contains(t, diagnostics.String(), test.wantAlso)
			}
			assert.Contains(t, diagnostics.String(), "normal logging continues")
		})
	}
}

// TestRouterTargetStampEligibility verifies explicit-width ignores and generic-width conflict disable behavior.
func TestRouterTargetStampEligibility(t *testing.T) {
	t.Run("explicit width", func(t *testing.T) {
		fileSystem := testFileSystem()
		lut := id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: "msg:v=%d\n"}}
		var diagnostics bytes.Buffer
		router, err := NewRouter(
			&diagnostics,
			fileSystem,
			lut,
			new(sync.RWMutex),
			[]string{`msg:printf("%d\n",ts16)@stamp.txt;log=drop`},
			true,
		)
		require.NoError(t, err)
		t.Cleanup(func() { require.NoError(t, router.Close()) })

		wrongWidth := testRecord(1, "TRICE32_1", "msg:v=%d\n", 23, 32, signedValue(32, 1))
		assert.False(t, router.Process(wrongWidth, false))
		unstamped := testRecord(1, "TRICE32_1", "msg:v=%d\n", 0, 0, signedValue(32, 1))
		assert.False(t, router.Process(unstamped, false))
		assert.Contains(t, diagnostics.String(), "requires ts16")
		assert.Contains(t, diagnostics.String(), "ignored unstamped ID 1")

		correct := testRecord(1, "TRICE32_1", "msg:v=%d\n", 24, 16, signedValue(32, 1))
		assert.True(t, router.Process(correct, false))
		assert.Equal(t, "24\n", readTestFile(t, fileSystem, "stamp.txt"))
	})

	t.Run("generic width conflict", func(t *testing.T) {
		fileSystem := testFileSystem()
		lut := id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: "msg:v=%d\n"}}
		var diagnostics bytes.Buffer
		router, err := NewRouter(
			&diagnostics,
			fileSystem,
			lut,
			new(sync.RWMutex),
			[]string{`msg:printf("%d\n",ts)@generic.txt;log=drop`},
			false,
		)
		require.NoError(t, err)
		t.Cleanup(func() { require.NoError(t, router.Close()) })

		first := testRecord(1, "TRICE32_1", "msg:v=%d\n", 10, 16, signedValue(32, 1))
		assert.True(t, router.Process(first, false))
		conflict := testRecord(1, "TRICE32_1", "msg:v=%d\n", 11, 32, signedValue(32, 1))
		assert.False(t, router.Process(conflict, false))
		assert.False(t, router.rules[0].enabled)
		assert.Contains(t, diagnostics.String(), "generic ts changed from 16 bits at ID 1 to 32 bits at ID 1")

		// A disabled rule never silently re-enables even if the original width returns.
		assert.False(t, router.Process(first, false))
		assert.Equal(t, "10\n", readTestFile(t, fileSystem, "generic.txt"))
	})
}

// TestRouterIgnoresMultiTriceAndMultilineRecords verifies the deliberately small line contract.
func TestRouterIgnoresMultiTriceAndMultilineRecords(t *testing.T) {
	fileSystem := testFileSystem()
	lut := id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: "msg:v=%d\n"}}
	var diagnostics bytes.Buffer
	router, err := NewRouter(
		&diagnostics,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{`msg:printf("%d\n",v0)@lines.txt;log=drop`},
		true,
	)
	require.NoError(t, err)
	t.Cleanup(func() { require.NoError(t, router.Close()) })

	record := testRecord(1, "TRICE32_1", "msg:v=%d\n", 0, 0, signedValue(32, 1))
	record.SingleLine = false
	assert.False(t, router.Process(record, false))
	record.SingleLine = true
	assert.False(t, router.Process(record, true))
	assert.Equal(t, 2, strings.Count(diagnostics.String(), "does not form one self-contained"))
	_, err = fileSystem.Stat("lines.txt")
	require.NoError(t, err)
	assert.Empty(t, readTestFile(t, fileSystem, "lines.txt"))
}

// TestRouterRuntimeExpressionFailureDisablesRuleWithoutDropping verifies failure isolation.
func TestRouterRuntimeExpressionFailureDisablesRuleWithoutDropping(t *testing.T) {
	fileSystem := testFileSystem()
	lut := id.TriceIDLookUp{1: {Type: "TRICE32_2", Strg: "msg:a=%d,b=%d\n"}}
	var diagnostics bytes.Buffer
	router, err := NewRouter(
		&diagnostics,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{`msg:printf("%f\n",v0/v1)@division.txt;log=drop`},
		false,
	)
	require.NoError(t, err)
	t.Cleanup(func() { require.NoError(t, router.Close()) })

	zero := testRecord(
		1,
		"TRICE32_2",
		"msg:a=%d,b=%d\n",
		0,
		0,
		signedValue(32, 5),
		signedValue(32, 0),
	)
	assert.False(t, router.Process(zero, false))
	assert.False(t, router.rules[0].enabled)
	assert.Contains(t, diagnostics.String(), "division by zero")
	assert.Contains(t, diagnostics.String(), "WARNING")
	assert.Empty(t, readTestFile(t, fileSystem, "division.txt"))

	nonzero := zero
	nonzero.Values[1] = signedValue(32, 2)
	assert.False(t, router.Process(nonzero, false))
	assert.Empty(t, readTestFile(t, fileSystem, "division.txt"))
}

// TestRouterRejectsUnsafeFloatToIntegerConversion verifies the explicit int64 safety boundary.
func TestRouterRejectsUnsafeFloatToIntegerConversion(t *testing.T) {
	fileSystem := testFileSystem()
	lut := id.TriceIDLookUp{1: {Type: "TRICE64_1", Strg: "msg:v=%f\n"}}
	var diagnostics bytes.Buffer
	router, err := NewRouter(
		&diagnostics,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{`msg:printf("%d\n",v0)@integer.txt;log=drop`},
		false,
	)
	require.NoError(t, err)
	t.Cleanup(func() { require.NoError(t, router.Close()) })

	record := testRecord(1, "TRICE64_1", "msg:v=%f\n", 0, 0, floatValue(64, 9223372036854775808.0))
	assert.False(t, router.Process(record, false))
	assert.Contains(t, diagnostics.String(), "cannot be converted safely to int64")
	assert.Empty(t, readTestFile(t, fileSystem, "integer.txt"))
}

// TestRouterAttemptsAllOverlappingRules verifies fan-out and successful drop precedence.
func TestRouterAttemptsAllOverlappingRules(t *testing.T) {
	fileSystem := testFileSystem()
	lut := id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: "msg:v=%u\n"}}
	router, err := NewRouter(
		nil,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{
			`msg:printf("%d\n",v0)@first.txt;log=keep`,
			`msg:printf("0x%08x\n",v0)@second.txt;log=drop`,
		},
		false,
	)
	require.NoError(t, err)
	t.Cleanup(func() { require.NoError(t, router.Close()) })

	record := testRecord(1, "TRICE32_1", "msg:v=%u\n", 0, 0, unsignedValue(32, 15))
	assert.True(t, router.Process(record, false))
	assert.Equal(t, "15\n", readTestFile(t, fileSystem, "first.txt"))
	assert.Equal(t, "0x0000000f\n", readTestFile(t, fileSystem, "second.txt"))
}

// TestRouterFileSinkAppendsExistingContent verifies create-or-append behavior without truncation.
func TestRouterFileSinkAppendsExistingContent(t *testing.T) {
	fileSystem := testFileSystem()
	require.NoError(t, fileSystem.WriteFile("append.txt", []byte("existing\n"), 0o644))
	lut := id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: "msg:v=%d\n"}}
	router, err := NewRouter(
		nil,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{`msg:printf("%d\n",v0)@append.txt`},
		false,
	)
	require.NoError(t, err)

	record := testRecord(1, "TRICE32_1", "msg:v=%d\n", 0, 0, signedValue(32, 7))
	assert.False(t, router.Process(record, false))
	require.NoError(t, router.Close())
	assert.Equal(t, "existing\n7\n", readTestFile(t, fileSystem, "append.txt"))
}

// TestRouterSharesNormalizedFileSinksAndDisablesAllAttachedRulesOnFailure verifies sink ownership.
func TestRouterSharesNormalizedFileSinksAndDisablesAllAttachedRulesOnFailure(t *testing.T) {
	fileSystem := testFileSystem()
	lut := id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: "msg:v=%d\n"}}
	var diagnostics bytes.Buffer
	router, err := NewRouter(
		&diagnostics,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{
			`msg:printf("%d\n",v0)@shared.txt`,
			`msg:printf("%x\n",v0)@./shared.txt;log=drop`,
		},
		false,
	)
	require.NoError(t, err)
	require.Len(t, router.sinks, 1)

	for _, sink := range router.sinks {
		require.NoError(t, sink.writer.Close())
	}
	record := testRecord(1, "TRICE32_1", "msg:v=%d\n", 0, 0, signedValue(32, 7))
	assert.False(t, router.Process(record, false))
	assert.False(t, router.rules[0].enabled)
	assert.False(t, router.rules[1].enabled)
	assert.Equal(t, 2, strings.Count(diagnostics.String(), "WARNING: -vis rule"))
	_ = router.Close()
}

// TestRouterReclassifiesChangedRuntimeCandidates verifies last-compatible disable semantics.
func TestRouterReclassifiesChangedRuntimeCandidates(t *testing.T) {
	fileSystem := testFileSystem()
	lut := id.TriceIDLookUp{
		1: {Type: "TRICE32_1", Strg: "msg:a=%d\n"},
		2: {Type: "TRICE32_1", Strg: "msg:b=%d\n"},
	}
	var diagnostics bytes.Buffer
	router, err := NewRouter(
		&diagnostics,
		fileSystem,
		lut,
		new(sync.RWMutex),
		[]string{`msg:printf("%d\n",v0)@changed.txt`},
		true,
	)
	require.NoError(t, err)
	t.Cleanup(func() { _ = router.Close() })

	changedOne := testRecord(1, "TRICE32_1", "msg:a=%t\n", 0, 0, boolValue(32, true))
	assert.False(t, router.Process(changedOne, false))
	assert.True(t, router.rules[0].enabled)
	assert.Equal(t, 1, router.rules[0].compatibleCount)

	stillGood := testRecord(2, "TRICE32_1", "msg:b=%d\n", 0, 0, signedValue(32, 8))
	assert.False(t, router.Process(stillGood, false))
	assert.Equal(t, "8\n", readTestFile(t, fileSystem, "changed.txt"))

	changedTwo := testRecord(2, "TRICE32_1", "msg:b=%t\n", 0, 0, boolValue(32, false))
	assert.False(t, router.Process(changedTwo, false))
	assert.False(t, router.rules[0].enabled)
	assert.Contains(t, diagnostics.String(), "runtime LUT change left no compatible candidate")

	newCompatible := testRecord(3, "TRICE32_1", "msg:new=%d\n", 0, 0, signedValue(32, 9))
	assert.False(t, router.Process(newCompatible, false))
	assert.Equal(t, "8\n", readTestFile(t, fileSystem, "changed.txt"))
}

// TestRouterRejectsUnsupportedOrUnusableSinksAtStartup verifies the deliberately limited transport set.
func TestRouterRejectsUnsupportedOrUnusableSinksAtStartup(t *testing.T) {
	lut := id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: "msg:v=%d\n"}}
	directoryFileSystem := testFileSystem()
	require.NoError(t, directoryFileSystem.Mkdir("not-a-file", 0o755))
	tests := []struct {
		name       string
		sink       string
		fileSystem *afero.Afero
		want       string
	}{
		{name: "relative file URI", sink: "file://out.csv", fileSystem: testFileSystem(), want: "plain path or file:<path>"},
		{name: "TCP", sink: "tcp://127.0.0.1:7", fileSystem: testFileSystem(), want: `scheme "tcp" is not supported`},
		{name: "empty explicit file", sink: "file:", fileSystem: testFileSystem(), want: "path must not be empty"},
		{name: "invalid UDP address", sink: "udp://127.0.0.1", fileSystem: testFileSystem(), want: "udp://host:port"},
		{name: "non-regular file", sink: "not-a-file", fileSystem: directoryFileSystem, want: "not a regular file"},
		{
			name:       "unwritable file",
			sink:       "out.csv",
			fileSystem: &afero.Afero{Fs: afero.NewReadOnlyFs(afero.NewMemMapFs())},
			want:       "cannot open sink",
		},
	}
	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			_, err := NewRouter(
				nil,
				test.fileSystem,
				lut,
				new(sync.RWMutex),
				[]string{fmt.Sprintf(`msg:printf("%%d",v0)@%s`, test.sink)},
				false,
			)
			require.Error(t, err)
			assert.Contains(t, err.Error(), test.want)
		})
	}
}

// TestRouterWritesOneUDPDatagramPerRecord verifies the MVP's record-oriented UDP contract.
func TestRouterWritesOneUDPDatagramPerRecord(t *testing.T) {
	listener, err := net.ListenUDP("udp4", &net.UDPAddr{IP: net.ParseIP("127.0.0.1")})
	if err != nil && strings.Contains(err.Error(), "operation not permitted") {
		t.Skipf("local UDP sockets are unavailable in this sandbox: %v", err)
	}
	require.NoError(t, err)
	t.Cleanup(func() { require.NoError(t, listener.Close()) })

	lut := id.TriceIDLookUp{1: {Type: "TRICE32_1", Strg: "msg:v=%d\n"}}
	specification := fmt.Sprintf(
		`msg:printf("value=%%d\n",v0)@udp://%s`,
		listener.LocalAddr().String(),
	)
	router, err := NewRouter(
		nil,
		testFileSystem(),
		lut,
		new(sync.RWMutex),
		[]string{specification},
		false,
	)
	require.NoError(t, err)
	t.Cleanup(func() { require.NoError(t, router.Close()) })

	record := testRecord(1, "TRICE32_1", "msg:v=%d\n", 0, 0, signedValue(32, 42))
	assert.False(t, router.Process(record, false))

	require.NoError(t, listener.SetReadDeadline(time.Now().Add(2*time.Second)))
	buffer := make([]byte, 64)
	count, _, err := listener.ReadFromUDP(buffer)
	require.NoError(t, err)
	assert.Equal(t, "value=42\n", string(buffer[:count]))
}
