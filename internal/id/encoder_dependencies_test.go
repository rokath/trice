// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"

	cobs "github.com/rokath/cobs/go"
	"github.com/rokath/tcobs/v1"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"golang.org/x/crypto/xtea"
)

// encoderBuildCase describes both active output paths and the exact optional
// sources allowed in a build. Missing encoders, encryption and RTT are not
// replaced by stubs, so an accidental reference fails at compile/link time.
type encoderBuildCase struct {
	name, buffer, direct, deferred, extra string
	cobs, tcobs, encrypted                bool
}

// encoderProject copies the library into an isolated project, deliberately
// omitting unused encoder headers as well as their implementation files.
// Native builds use -O0 and ordinary linking: no LTO, function sections or GC.
func encoderProject(t *testing.T, tc encoderBuildCase) (string, []string) {
	t.Helper()
	project := t.TempDir()
	root := bindRepositoryRoot(t)
	library := filepath.Join(project, "library")
	require.NoError(t, os.MkdirAll(library, 0o755))
	headers, err := filepath.Glob(filepath.Join(root, "src", "*.h"))
	require.NoError(t, err)
	for _, path := range headers {
		name := filepath.Base(path)
		if (!tc.cobs && name == "cobs.h") || (!tc.tcobs && name == "tcobs.h") ||
			(!tc.encrypted && name == "xtea.h") || strings.HasPrefix(name, "SEGGER") {
			continue
		}
		data, err := os.ReadFile(path)
		require.NoError(t, err)
		require.NoError(t, os.WriteFile(filepath.Join(library, name), data, 0o644))
	}
	sources, err := filepath.Glob(filepath.Join(root, "src", "trice*.c"))
	require.NoError(t, err)
	if tc.cobs {
		sources = append(sources, filepath.Join(root, "src", "cobsEncode.c"))
	}
	if tc.tcobs {
		sources = append(sources, filepath.Join(root, "src", "tcobsv1Encode.c"))
	}
	if tc.encrypted {
		sources = append(sources, filepath.Join(root, "src", "xtea.c"))
	}
	var copied []string
	for _, path := range sources {
		data, err := os.ReadFile(path)
		require.NoError(t, err)
		destination := filepath.Join(library, filepath.Base(path))
		require.NoError(t, os.WriteFile(destination, data, 0o644))
		copied = append(copied, destination)
	}
	// Inactive framing settings are left at their defaults; they must not force
	// TCOBS into a direct-only build or another encoder into a deferred build.
	config := "// SPDX-License-Identifier: MIT\n#define TRICE_BUFFER " + tc.buffer + "\n" +
		"#define TRICE_DIAGNOSTICS 0\n#define TRICE_CONFIG_WARNINGS 0\n#define TRICE_CYCLE_COUNTER 0\n"
	if tc.direct != "" {
		config += "#define TRICE_DIRECT_OUTPUT 1\n#define TRICE_DIRECT_AUXILIARY8 1\n#define TRICE_DIRECT_OUT_FRAMING " + tc.direct + "\n"
	}
	if tc.deferred != "" {
		config += "#define TRICE_DEFERRED_OUTPUT 1\n#define TRICE_DEFERRED_AUXILIARY8 1\n#define TRICE_DEFERRED_OUT_FRAMING " + tc.deferred + "\n"
	}
	config += tc.extra
	require.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(config), 0o644))
	return project, copied
}

// encoderFixture emits real direct/deferred records and calls the public
// runtime API with each framing. Unavailable modes must return zero before
// touching either buffer, even if the caller asks for encryption.
const encoderFixture = `// SPDX-License-Identifier: MIT
#include "trice.h"
#include <stdio.h>

// Each output line carries one channel followed by the exact transferred bytes.
static void printBytes(const char* channel, const uint8_t* data, size_t length) {
	printf("%s=", channel);
	for (size_t i = 0; i < length; ++i) {
		printf("%02x", (unsigned)data[i]);
	}
	putchar('\n');
}
static void directWrite(const uint8_t* data, size_t length) {
	printBytes("direct", data, length);
}
static void deferredWrite(const uint8_t* data, size_t length) {
	printBytes("deferred", data, length);
}

// Aligned storage leaves the normal framing headroom between destination and input.
static int checkRuntimeFraming(const char* name, unsigned framing, int supported) {
	uint32_t storage[64];
	uint8_t* dst = (uint8_t*)storage;
	uint8_t* input = dst + 64;
	const uint8_t payload[] = {0, 1, 2, 0, 0, 3, 3, 3};
	memset(storage, 0xa5, sizeof(storage));
	memcpy(input, payload, sizeof(payload));
	size_t count = TriceEncode(0, framing, dst, input, sizeof(payload));
	if (supported) {
		if (count == 0) return 10;
		printBytes(name, dst, count);
		return 0;
	}
	if (count != 0) return 11;
	// A rejected request must not encrypt in place or overwrite destination bytes.
	if (TriceEncode(1, framing, dst, input, sizeof(payload)) != 0) return 12;
	for (size_t i = 0; i < 64; ++i) {
		if (dst[i] != 0xa5) return 13;
	}
	if (memcmp(input, payload, sizeof(payload)) != 0) return 14;
	printf("%s=unavailable\n", name);
	return 0;
}
int main(void) {
#if TRICE_DIRECT_OUTPUT == 1
	UserNonBlockingDirectWrite8AuxiliaryFn = directWrite;
#else
	(void)directWrite;
#endif
#if TRICE_DEFERRED_OUTPUT == 1
	UserNonBlockingDeferredWrite8AuxiliaryFn = deferredWrite;
#else
	(void)deferredWrite;
#endif
	TriceInit();
	TRICE32(id(1000), "Value=%u", 42u);
	TriceTransfer();
	int result = checkRuntimeFraming("cobs", TRICE_FRAMING_COBS, TRICE_COBS_ENCODE_SUPPORT);
	if (result) return result;
	result = checkRuntimeFraming("tcobs", TRICE_FRAMING_TCOBS, TRICE_TCOBS_ENCODE_SUPPORT);
	if (result) return result;
	result = checkRuntimeFraming("none", TRICE_FRAMING_NONE, 1);
	if (result) return result;
	if (checkRuntimeFraming("unknown", 12345u, 0)) return 15;
	return 0;
}
`

// decodeEncoderFrame uses the independent host decoder, never a target decoder
// linked into the fixture (which could hide an unwanted library dependency).
func decodeEncoderFrame(t *testing.T, framing, text string) []byte {
	t.Helper()
	frame, err := hex.DecodeString(text)
	require.NoError(t, err)
	if framing == "TRICE_FRAMING_NONE" {
		return frame
	}
	require.NotEmpty(t, frame)
	require.Zero(t, frame[len(frame)-1], "framed output ends in exactly one delimiter")
	frame = frame[:len(frame)-1]
	assert.NotContains(t, frame, byte(0), "encoded payload must not contain a delimiter")
	decoded := make([]byte, 256)
	var count int
	if framing == "TRICE_FRAMING_COBS" {
		count, err = cobs.Decode(decoded, frame)
	} else {
		count, err = tcobs.Decode(decoded, frame)
		require.NoError(t, err)
		// TCOBS writes backwards from the end of the supplied buffer.
		return decoded[len(decoded)-count:]
	}
	require.NoError(t, err)
	return decoded[:count]
}

// decryptEncoderRecord converts the target's little-endian words to XTEA's
// network-order block interface, then back. The fixture uses an all-zero key.
func decryptEncoderRecord(t *testing.T, data []byte) []byte {
	t.Helper()
	require.Zero(t, len(data)%8)
	algorithm, err := xtea.NewCipher(make([]byte, 16))
	require.NoError(t, err)
	result := append([]byte(nil), data...)
	for i := 0; i < len(result); i += 8 {
		block := make([]byte, 8)
		for j := 0; j < 8; j += 4 {
			binary.BigEndian.PutUint32(block[j:], binary.LittleEndian.Uint32(data[i+j:]))
		}
		algorithm.Decrypt(block, block)
		for j := 0; j < 8; j += 4 {
			binary.LittleEndian.PutUint32(result[i+j:], binary.BigEndian.Uint32(block[j:]))
		}
	}
	return result
}

// TestConfiguredEncodersLinkAndDecodeWithoutLTO covers direct, both deferred
// buffers, mixed outputs and explicit runtime opt-ins. Builds have neither
// unused encoder headers nor encoder/RTT/XTEA objects unless actually needed.
func TestConfiguredEncodersLinkAndDecodeWithoutLTO(t *testing.T) {
	t.Parallel()
	compilers := availableBindCompilers("cc", "gcc", "clang")
	if len(compilers) == 0 {
		t.Skip("no native C compiler available")
	}
	cases := []encoderBuildCase{
		{name: "direct_TCOBS_omits_COBS_and_XTEA", buffer: "TRICE_STACK_BUFFER", direct: "TRICE_FRAMING_TCOBS", tcobs: true},
		{name: "direct_COBS_ignores_inactive_deferred_TCOBS_default", buffer: "TRICE_STATIC_BUFFER", direct: "TRICE_FRAMING_COBS", cobs: true},
		{name: "direct_NONE_omits_both_encoders", buffer: "TRICE_STACK_BUFFER", direct: "TRICE_FRAMING_NONE"},
		{name: "ring_TCOBS_omits_COBS", buffer: "TRICE_RING_BUFFER", deferred: "TRICE_FRAMING_TCOBS", tcobs: true},
		{name: "inactive_direct_COBS_and_auxiliary_do_not_add_dependencies", buffer: "TRICE_RING_BUFFER", deferred: "TRICE_FRAMING_TCOBS", tcobs: true, extra: "#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_COBS\n#define TRICE_DIRECT_AUXILIARY8 1\n"},
		{name: "ring_COBS_omits_TCOBS", buffer: "TRICE_RING_BUFFER", deferred: "TRICE_FRAMING_COBS", cobs: true},
		{name: "double_TCOBS_omits_COBS", buffer: "TRICE_DOUBLE_BUFFER", deferred: "TRICE_FRAMING_TCOBS", tcobs: true},
		{name: "double_COBS_omits_TCOBS", buffer: "TRICE_DOUBLE_BUFFER", deferred: "TRICE_FRAMING_COBS", cobs: true},
		{name: "ring_NONE_omits_both_encoders", buffer: "TRICE_RING_BUFFER", deferred: "TRICE_FRAMING_NONE"},
		{name: "mixed_direct_COBS_deferred_TCOBS", buffer: "TRICE_RING_BUFFER", direct: "TRICE_FRAMING_COBS", deferred: "TRICE_FRAMING_TCOBS", cobs: true, tcobs: true},
		{name: "mixed_direct_TCOBS_deferred_COBS", buffer: "TRICE_DOUBLE_BUFFER", direct: "TRICE_FRAMING_TCOBS", deferred: "TRICE_FRAMING_COBS", cobs: true, tcobs: true},
		{name: "runtime_COBS_opt_in_with_TCOBS_output", buffer: "TRICE_STACK_BUFFER", direct: "TRICE_FRAMING_TCOBS", cobs: true, tcobs: true, extra: "#define TRICE_COBS_ENCODE_SUPPORT 1\n"},
		{name: "runtime_TCOBS_opt_in_with_NONE_output", buffer: "TRICE_STACK_BUFFER", direct: "TRICE_FRAMING_NONE", tcobs: true, extra: "#define TRICE_TCOBS_ENCODE_SUPPORT 1\n"},
		{name: "encrypted_COBS_omits_TCOBS", buffer: "TRICE_STACK_BUFFER", direct: "TRICE_FRAMING_COBS", cobs: true, encrypted: true, extra: "#define TRICE_DIRECT_XTEA_ENCRYPT 1\n#define XTEA_ENCRYPT_KEY {0, 0, 0, 0}\n"},
		{name: "encrypted_TCOBS_omits_COBS", buffer: "TRICE_RING_BUFFER", deferred: "TRICE_FRAMING_TCOBS", tcobs: true, encrypted: true, extra: "#define TRICE_DEFERRED_XTEA_ENCRYPT 1\n#define XTEA_ENCRYPT_KEY {0, 0, 0, 0}\n"},
	}
	for _, compiler := range compilers {
		for _, tc := range cases {
			t.Run(filepath.Base(compiler)+"/"+tc.name, func(t *testing.T) {
				// Each case owns its entire library/configuration, so compilers can
				// run concurrently without instrumenting shared repository sources.
				t.Parallel()
				project, sources := encoderProject(t, tc)
				main := filepath.Join(project, "main.c")
				require.NoError(t, os.WriteFile(main, []byte(encoderFixture), 0o644))
				executable := filepath.Join(project, "encoder.exe")
				args := []string{"-std=c11", "-O0", "-Wall", "-Wextra", "-Werror", "-I", project, "-I", filepath.Join(project, "library"), main}
				args = append(args, sources...)
				args = append(args, "-o", executable)
				output, err := exec.Command(compiler, args...).CombinedOutput()
				require.NoError(t, err, "%s %s\n%s", compiler, strings.Join(args, " "), output)
				output, err = exec.Command(executable).CombinedOutput()
				require.NoError(t, err, "%s", output)
				// Missing or duplicate channels are failures, not silently ignored output.
				lines := make(map[string]string)
				for _, line := range strings.Split(strings.TrimSpace(string(output)), "\n") {
					channel, frame, found := strings.Cut(line, "=")
					require.True(t, found, line)
					_, duplicate := lines[channel]
					assert.False(t, duplicate, "one record per channel: %s", channel)
					lines[channel] = frame
				}
				for _, mode := range []struct {
					name, framing string
					available     bool
				}{
					{"cobs", "TRICE_FRAMING_COBS", tc.cobs}, {"tcobs", "TRICE_FRAMING_TCOBS", tc.tcobs}, {"none", "TRICE_FRAMING_NONE", true},
				} {
					if mode.available {
						assert.Equal(t, []byte{0, 1, 2, 0, 0, 3, 3, 3}, decodeEncoderFrame(t, mode.framing, lines[mode.name]))
					} else {
						assert.Equal(t, "unavailable", lines[mode.name])
					}
				}
				assert.Equal(t, "unavailable", lines["unknown"])
				// The real log record has ID 1000, no stamp, four payload bytes and value 42.
				// 0xc0 is the protocol's marker for a disabled cycle counter.
				wantRecord := []byte{0xe8, 0x43, 0xc0, 4, 42, 0, 0, 0}
				for channel, framing := range map[string]string{"direct": tc.direct, "deferred": tc.deferred} {
					if framing == "" {
						_, present := lines[channel]
						assert.False(t, present, "inactive output cannot emit data")
						continue
					}
					data := decodeEncoderFrame(t, framing, lines[channel])
					if tc.encrypted {
						assert.NotEqual(t, wantRecord, data, "encryption must be applied, not bypassed")
						data = decryptEncoderRecord(t, data)
					}
					assert.Equal(t, wantRecord, data, channel)
				}
			})
		}
	}
}

// TestDeferredRTTDoesNotPullInactiveDirectEncoder checks the shared RTT helper
// boundary. Real RTT headers are present, but the inactive direct COBS header
// is absent; only the deferred TCOBS encoder may appear in the object symbols.
func TestDeferredRTTDoesNotPullInactiveDirectEncoder(t *testing.T) {
	compiler := firstAvailableCompiler("cc", "gcc", "clang")
	if compiler == "" {
		t.Skip("no native C compiler available")
	}
	project, _ := encoderProject(t, encoderBuildCase{
		buffer: "TRICE_RING_BUFFER", deferred: "TRICE_FRAMING_TCOBS", tcobs: true,
		// Match the real RTT defaults; no device transport is executed.
		extra: "#define TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE 1\n#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_COBS\n#define TRICE_BUFFER_SIZE_DOWN BUFFER_SIZE_DOWN\n#define TRICE_SEGGER_RTT_PRINTF_BUFFER_SIZE SEGGER_RTT_PRINTF_BUFFER_SIZE\n",
	})
	library := filepath.Join(project, "library")
	root := bindRepositoryRoot(t)
	for _, name := range []string{"SEGGER_RTT.h", "SEGGER_RTT_ConfDefaults.h", "default_conf/SEGGER_RTT_Conf.h"} {
		data, err := os.ReadFile(filepath.Join(root, "src", name))
		require.NoError(t, err)
		destination := filepath.Join(library, name)
		require.NoError(t, os.MkdirAll(filepath.Dir(destination), 0o755))
		require.NoError(t, os.WriteFile(destination, data, 0o644))
	}
	configPath := filepath.Join(project, "triceConfig.h")
	config, err := os.ReadFile(configPath)
	require.NoError(t, err)
	for _, diagnostics := range []string{"0", "1"} {
		t.Run("protected_RTT_with_diagnostics_"+diagnostics, func(t *testing.T) {
			// Keep protection enabled to compile the RTT overflow path as well.
			require.NoError(t, os.WriteFile(configPath, []byte(strings.Replace(string(config), "#define TRICE_DIAGNOSTICS 0", "#define TRICE_DIAGNOSTICS "+diagnostics, 1)), 0o644))
			object := filepath.Join(project, "trice.o")
			output, err := exec.Command(compiler, "-std=c11", "-O0", "-Wall", "-Wextra", "-Werror", "-I", project, "-I", library, "-I", filepath.Join(library, "default_conf"), "-c", filepath.Join(library, "trice.c"), "-o", object).CombinedOutput()
			require.NoError(t, err, "%s", output)
			symbols, err := localLogObjectSymbols(object)
			require.NoError(t, err)
			// Mach-O prefixes symbols with an underscore; ELF/COFF use the C spelling.
			for i := range symbols {
				symbols[i] = strings.TrimPrefix(symbols[i], "_")
			}
			assert.Contains(t, symbols, "TCOBSEncode")
			assert.NotContains(t, symbols, "COBSEncode")
			assert.NotContains(t, symbols, "directXEncode8")
			assert.NotContains(t, symbols, "TriceDirectWrite8")
			assert.NotContains(t, symbols, "TriceDirectOverflowCount")
			if diagnostics == "1" {
				assert.Contains(t, symbols, "TriceDeferredOverflowCount")
			} else {
				assert.NotContains(t, symbols, "TriceDeferredOverflowCount")
			}
		})
	}
}

// TestEncoderConfigurationRejectsMissingRequiredSupport catches inconsistent
// opt-outs and invalid booleans before they become an undefined encoder symbol.
func TestEncoderConfigurationRejectsMissingRequiredSupport(t *testing.T) {
	compiler := firstAvailableCompiler("cc", "gcc", "clang")
	if compiler == "" {
		t.Skip("no native C compiler available")
	}
	for _, tc := range []struct{ name, extra, message string }{
		{"COBS_output_cannot_disable_COBS", "#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_COBS\n#define TRICE_COBS_ENCODE_SUPPORT 0\n", "active COBS output requires TRICE_COBS_ENCODE_SUPPORT == 1"},
		{"TCOBS_output_cannot_disable_TCOBS", "#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_TCOBS\n#define TRICE_TCOBS_ENCODE_SUPPORT 0\n", "active TCOBS output requires TRICE_TCOBS_ENCODE_SUPPORT == 1"},
		{"COBS_support_must_be_boolean", "#define TRICE_COBS_ENCODE_SUPPORT 2\n", "TRICE_COBS_ENCODE_SUPPORT must be 0 or 1"},
		{"TCOBS_support_must_be_boolean", "#define TRICE_TCOBS_ENCODE_SUPPORT -1\n", "TRICE_TCOBS_ENCODE_SUPPORT must be 0 or 1"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			project := t.TempDir()
			config := "#define TRICE_BUFFER TRICE_STACK_BUFFER\n#define TRICE_DIRECT_OUTPUT 1\n#define TRICE_DIRECT_AUXILIARY8 1\n" + tc.extra
			require.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(config), 0o644))
			source := filepath.Join(project, "main.c")
			require.NoError(t, os.WriteFile(source, []byte("#include \"trice.h\"\n"), 0o644))
			output, err := exec.Command(compiler, "-I", project, "-I", filepath.Join(bindRepositoryRoot(t), "src"), "-c", source, "-o", filepath.Join(project, "main.o")).CombinedOutput()
			require.Error(t, err)
			assert.Contains(t, string(output), tc.message)
		})
	}
}

// TestDisabledBackendNeedsNoEncoderOrUartHardware proves that retaining UART
// settings while disabling/cleaning logs needs no user hardware header or hooks.
func TestDisabledBackendNeedsNoEncoderOrUartHardware(t *testing.T) {
	compiler := firstAvailableCompiler("cc", "gcc", "clang")
	if compiler == "" {
		t.Skip("no native C compiler available")
	}
	for _, switchName := range []string{"TRICE_OFF", "TRICE_CLEAN"} {
		t.Run(switchName, func(t *testing.T) {
			project, sources := encoderProject(t, encoderBuildCase{
				buffer: "TRICE_RING_BUFFER",
				extra:  fmt.Sprintf("#define %s 1\n#define TRICE_TX_X0_COUNTED_BUFFER_SUPPORT 0\n#define TRICE_DEFERRED_UARTA 1\n#define TRICE_DEFERRED_UARTB 1\n", switchName),
			})
			main := filepath.Join(project, "main.c")
			require.NoError(t, os.WriteFile(main, []byte("#include \"trice.h\"\nint main(void) { TRICE32(id(1000), \"value=%u\", 42u); return 0; }\n"), 0o644))
			args := []string{"-std=c11", "-O0", "-Wall", "-Wextra", "-Werror", "-I", project, "-I", filepath.Join(project, "library"), main}
			args = append(args, sources...)
			executable := filepath.Join(project, "disabled.exe")
			args = append(args, "-o", executable)
			output, err := exec.Command(compiler, args...).CombinedOutput()
			require.NoError(t, err, "%s", output)
			output, err = exec.Command(executable).CombinedOutput()
			require.NoError(t, err, "%s", output)
			assert.Empty(t, output)
		})
	}
}

// TestARMEncoderReferencesMatchConfiguration checks cross-compiled objects
// without a target board or host nm. Even at -O0, absent encoders and XTEA/RTT
// must leave no symbol reference for a linker to resolve or eliminate.
func TestARMEncoderReferencesMatchConfiguration(t *testing.T) {
	compiler := firstAvailableCompiler("arm-none-eabi-gcc")
	if compiler == "" {
		t.Skip("ARM GCC not installed; native no-LTO link/decoder tests still run")
	}
	for _, tc := range []encoderBuildCase{
		{name: "TCOBS_only", buffer: "TRICE_STACK_BUFFER", direct: "TRICE_FRAMING_TCOBS", tcobs: true},
		{name: "COBS_only", buffer: "TRICE_STACK_BUFFER", direct: "TRICE_FRAMING_COBS", cobs: true},
		{name: "NONE_only", buffer: "TRICE_STACK_BUFFER", direct: "TRICE_FRAMING_NONE"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			project, _ := encoderProject(t, tc)
			object := filepath.Join(project, "trice.o")
			output, err := exec.Command(compiler, "-std=c11", "-O0", "-Wall", "-Wextra", "-Werror", "-I", project, "-I", filepath.Join(project, "library"), "-c", filepath.Join(project, "library", "trice.c"), "-o", object).CombinedOutput()
			require.NoError(t, err, "%s", output)
			symbols, err := localLogObjectSymbols(object)
			require.NoError(t, err)
			joined := strings.Join(symbols, "\n")
			for symbol, enabled := range map[string]bool{"COBSEncode": tc.cobs, "TCOBSEncode": tc.tcobs} {
				found := bytes.Contains([]byte("\n"+joined+"\n"), []byte("\n"+symbol+"\n"))
				assert.Equal(t, enabled, found, symbol)
			}
			assert.NotContains(t, joined, "XTEA")
			assert.NotContains(t, joined, "SEGGER")
		})
	}
}

// TestCountedX0RetainsEncoderWithOrdinaryLogsDisabled proves that OFF/CLEAN
// remove ordinary log functions while the independently enabled X0 backend
// still links its selected encoder and transfers the caller's counted bytes.
func TestCountedX0RetainsEncoderWithOrdinaryLogsDisabled(t *testing.T) {
	compiler := firstAvailableCompiler("cc", "gcc", "clang")
	if compiler == "" {
		t.Skip("no native C compiler available")
	}
	for _, switchName := range []string{"TRICE_OFF", "TRICE_CLEAN"} {
		t.Run(switchName, func(t *testing.T) {
			project, sources := encoderProject(t, encoderBuildCase{
				buffer: "TRICE_STACK_BUFFER", direct: "TRICE_FRAMING_TCOBS", tcobs: true,
				extra: fmt.Sprintf("#define %s 1\n#define TRICE_TX_X0_COUNTED_BUFFER_SUPPORT 1\n", switchName),
			})
			// Keep the disabled ordinary log call; only the explicit X0 payload
			// is allowed to reach the output callback.
			source := strings.Replace(encoderFixture, "\tTriceTransfer();", "\tconst uint8_t payload[] = {0x41, 0, 0xff};\n\ttriceX0(payload, sizeof(payload));\n\tTriceTransfer();", 1)
			main := filepath.Join(project, "main.c")
			require.NoError(t, os.WriteFile(main, []byte(source), 0o644))
			executable := filepath.Join(project, "x0.exe")
			args := []string{"-std=c11", "-O0", "-Wall", "-Wextra", "-Werror", "-I", project, "-I", filepath.Join(project, "library"), main}
			args = append(args, sources...)
			args = append(args, "-o", executable)
			output, err := exec.Command(compiler, args...).CombinedOutput()
			require.NoError(t, err, "%s", output)
			output, err = exec.Command(executable).CombinedOutput()
			require.NoError(t, err, "%s", output)
			var frames []string
			for _, line := range strings.Split(string(output), "\n") {
				if strings.HasPrefix(line, "direct=") {
					frames = append(frames, strings.TrimPrefix(line, "direct="))
				}
			}
			require.Len(t, frames, 1, "the disabled ordinary log must not create another frame")
			data := decodeEncoderFrame(t, "TRICE_FRAMING_TCOBS", frames[0])
			require.GreaterOrEqual(t, len(data), 5)
			assert.Equal(t, []byte{3, 0, 0x41, 0, 0xff}, data[:5], "counted X0 header and payload survive OFF/CLEAN")
			for _, padding := range data[5:] {
				assert.Zero(t, padding, "direct word alignment may append only zero padding")
			}
		})
	}
}
