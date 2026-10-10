// SPDX-License-Identifier: MIT

package tst

import (
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

// rttConfigProbe exercises the real RTT implementation, not just preprocessing.
// Reading back the up-buffer emulates the host and verifies that its configured
// size, mode and project-specific locks actually reach the compiled library.
const rttConfigProbe = `// SPDX-License-Identifier: MIT
#include "SEGGER_RTT.h"
#include <stdio.h>

unsigned test_lock_entries;

// Stand-ins for an explicitly requested embOS Simulation configuration.
void OS_SIM_EnterCriticalSection(void) { ++test_lock_entries; }
void OS_SIM_LeaveCriticalSection(void) {}

int main(void) {
    char received[8] = {0};
    SEGGER_RTT_Init();
    unsigned written = SEGGER_RTT_Write(0, "hello", 5);
    unsigned read = SEGGER_RTT_ReadUpBuffer(0, received, sizeof(received) - 1);
    printf("up=%u down=%u channels=%u/%u mode=%u written=%u read=%u locks=%u data=%s\n",
           _SEGGER_RTT.aUp[0].SizeOfBuffer, _SEGGER_RTT.aDown[0].SizeOfBuffer,
           (unsigned)SEGGER_RTT_MAX_NUM_UP_BUFFERS, (unsigned)SEGGER_RTT_MAX_NUM_DOWN_BUFFERS,
           _SEGGER_RTT.aUp[0].Flags, written, read,
           test_lock_entries >= 2 ? 1u : 0u, received);
    return 0;
}
`

// rttProjectConfiguration deliberately differs from every fallback size. The
// lock counter proves that the project header replaces the whole configuration,
// including its synchronization policy, rather than merely overriding constants.
const rttProjectConfiguration = `// SPDX-License-Identifier: MIT
#ifndef SEGGER_RTT_CONF_H
#define SEGGER_RTT_CONF_H
#define RTT_USE_ASM 0
#define SEGGER_RTT_MAX_NUM_UP_BUFFERS 2
#define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS 1
#define BUFFER_SIZE_UP 64
#define BUFFER_SIZE_DOWN 8
#define SEGGER_RTT_MODE_DEFAULT SEGGER_RTT_MODE_NO_BLOCK_TRIM
extern unsigned test_lock_entries;
#define SEGGER_RTT_LOCK() do { ++test_lock_entries; } while (0)
#define SEGGER_RTT_UNLOCK() do {} while (0)
#endif
`

// TestRTTFallbackAndProjectConfiguration covers a missing project header,
// replacement by project/generated headers, command-line defaults and Windows
// compilation without embOS. It also demonstrates why the default path is last.
func TestRTTFallbackAndProjectConfiguration(t *testing.T) {
	sourceDir, err := filepath.Abs(filepath.Join("..", "..", "src"))
	if !assert.NoError(t, err) {
		return
	}
	for _, compiler := range []string{"cc", "gcc", "clang"} {
		t.Run(compiler, func(t *testing.T) {
			if _, err := exec.LookPath(compiler); err != nil {
				t.Skipf("%s is not installed", compiler)
			}
			for _, tc := range []struct {
				name, configDirectory, want string
				configContent               string
				legacyConfig                bool
				flags                       []string
				defaultFirst                bool
			}{
				{name: "missing_project_header_uses_fallback", want: "up=1024 down=16 channels=1/1 mode=0 written=5 read=5 locks=0 data=hello"},
				{name: "project_header_replaces_sizes_mode_and_locks", configDirectory: "project", want: "up=64 down=8 channels=2/1 mode=1 written=5 read=5 locks=1 data=hello"},
				{name: "generated_header_also_precedes_fallback", configDirectory: "generated", want: "up=64 down=8 channels=2/1 mode=1 written=5 read=5 locks=1 data=hello"},
				{name: "command_line_can_override_fallback_buffer_size", flags: []string{"-DBUFFER_SIZE_UP=128"}, want: "up=128 down=16 channels=1/1 mode=0 written=5 read=5 locks=0 data=hello"},
				{name: "ordinary_windows_build_needs_no_embos_functions", flags: []string{"-DWIN32"}, want: "up=1024 down=16 channels=1/1 mode=0 written=5 read=5 locks=0 data=hello"},
				{name: "empty_project_header_uses_segger_defaults", configDirectory: "project", configContent: "/* Deliberately empty user configuration. */\n", want: "up=1024 down=16 channels=3/3 mode=0 written=5 read=5 locks=0 data=hello"},
				{name: "minimal_project_header_inherits_missing_sizes_and_locks", configDirectory: "project", configContent: "#define SEGGER_RTT_MAX_NUM_UP_BUFFERS 1\n#define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS 1\n#define RTT_USE_ASM 0\n#define BUFFER_SIZE_UP 64\n", want: "up=64 down=16 channels=1/1 mode=0 written=5 read=5 locks=0 data=hello"},
				{name: "existing_board_configuration_keeps_its_values", configDirectory: "project", legacyConfig: true, want: "up=2048 down=0 channels=1/1 mode=0 written=5 read=5 locks=0 data=hello"},
				{name: "explicit_windows_embos_configuration_keeps_its_locks", flags: []string{"-DWIN32", "-DSEGGER_RTT_LOCK_EMBOS"}, want: "up=1024 down=16 channels=1/1 mode=0 written=5 read=5 locks=1 data=hello"},
				{name: "putting_fallback_first_hides_the_project_header", configDirectory: "project", defaultFirst: true, want: "up=1024 down=16 channels=1/1 mode=0 written=5 read=5 locks=0 data=hello"},
			} {
				t.Run(tc.name, func(t *testing.T) {
					root := t.TempDir()
					project := filepath.Join(root, "project")
					generated := filepath.Join(root, "generated")
					for _, directory := range []string{project, generated} {
						if !assert.NoError(t, os.MkdirAll(directory, 0o755)) {
							return
						}
					}
					if tc.configDirectory != "" {
						path := filepath.Join(root, tc.configDirectory, "SEGGER_RTT_Conf.h")
						content := []byte(rttProjectConfiguration)
						if tc.configContent != "" {
							content = []byte(tc.configContent)
						}
						if tc.legacyConfig {
							// Copy an existing full board configuration without rewriting
							// it into the new minimal format. Updates must preserve users.
							content, err = os.ReadFile(filepath.Join(sourceDir, "..", "examples", "G0B1_inst", "Core", "Inc", "SEGGER_RTT_Conf.h"))
							if !assert.NoError(t, err) {
								return
							}
						}
						if !assert.NoError(t, os.WriteFile(path, content, 0o644)) {
							return
						}
					}
					probe := filepath.Join(project, "probe.c")
					if !assert.NoError(t, os.WriteFile(probe, []byte(rttConfigProbe), 0o644)) {
						return
					}
					executable := filepath.Join(root, "probe.exe")
					// The fallback is normally the final include directory. The
					// reversed case is intentional evidence of the ordering rule.
					includes := []string{project, generated, sourceDir, filepath.Join(sourceDir, "default_conf")}
					if tc.defaultFirst {
						includes = []string{filepath.Join(sourceDir, "default_conf"), project, generated, sourceDir}
					}
					args := []string{"-std=c99", "-Wall", "-Wextra", "-Werror"}
					args = append(args, tc.flags...)
					for _, directory := range includes {
						args = append(args, "-I", directory)
					}
					args = append(args, probe, filepath.Join(sourceDir, "SEGGER_RTT.c"), "-o", executable)
					output, err := exec.Command(compiler, args...).CombinedOutput()
					if !assert.NoError(t, err, "%s", output) {
						return
					}
					output, err = exec.Command(executable).CombinedOutput()
					if assert.NoError(t, err, "%s", output) {
						assert.Equal(t, tc.want, strings.TrimSpace(string(output)))
					}
				})
			}
		})
	}
}

// TestRTTFallbackLinksForCortexM proves that the fallback works with the actual
// M0/M4 interrupt-lock instructions and needs no external RTT assembly objects.
// The binaries are linked for the target but cannot run on the host computer.
func TestRTTFallbackLinksForCortexM(t *testing.T) {
	const compiler = "arm-none-eabi-gcc"
	if _, err := exec.LookPath(compiler); err != nil {
		t.Skip("Arm GNU toolchain is not installed")
	}
	sourceDir, err := filepath.Abs(filepath.Join("..", "..", "src"))
	if !assert.NoError(t, err) {
		return
	}
	for _, cpu := range []string{"cortex-m0", "cortex-m4"} {
		t.Run(cpu, func(t *testing.T) {
			project := t.TempDir()
			probe := filepath.Join(project, "probe.c")
			const source = "// SPDX-License-Identifier: MIT\n#include \"SEGGER_RTT.h\"\nint main(void) { SEGGER_RTT_Init(); return (int)SEGGER_RTT_WriteSkipNoLock(0, \"hello\", 5); }\n"
			if !assert.NoError(t, os.WriteFile(probe, []byte(source), 0o644)) {
				return
			}
			executable := filepath.Join(project, "probe.elf")
			args := []string{"-std=c99", "-Wall", "-Wextra", "-Werror", "-mcpu=" + cpu, "-mthumb", "-nostartfiles", "-Wl,-e,main",
				"-I", project, "-I", sourceDir, "-I", filepath.Join(sourceDir, "default_conf"),
				probe, filepath.Join(sourceDir, "SEGGER_RTT.c"), "-o", executable}
			output, err := exec.Command(compiler, args...).CombinedOutput()
			if assert.NoError(t, err, "%s", output) {
				info, err := os.Stat(executable)
				if assert.NoError(t, err) {
					assert.Positive(t, info.Size(), "the target link must produce a nonempty firmware ELF")
				}
			}
		})
	}
}

// rttTriceWriteProbe includes trice.c so it can call the actual internal device
// writers. TRICE_CGO stays disabled: the vendor ring buffer is used rather than
// the usual CGO callback. Draining prefix bytes positions both offsets near the
// end; retaining them instead creates a full-buffer rejection case.
const rttTriceWriteProbe = `// SPDX-License-Identifier: MIT
#include "trice.c"
#include <stdio.h>
#include <string.h>

int main(void) {
    const uint32_t payload[] = {0x12345678u, 0xABCDEF01u, 0x99887766u};
    char prefix[64];
    char received[64] = {0};
    memset(prefix, 'x', sizeof(prefix));
    TriceInit();
    if (TEST_START) {
        // WriteSkipNoLock reports success as 1, not the byte count.
        if (!SEGGER_RTT_WriteSkipNoLock(0, prefix, TEST_START)) {
            fprintf(stderr, "could not position RTT buffer with %u prefix bytes\n", (unsigned)TEST_START);
            return 1;
        }
        if (!TEST_REJECT && SEGGER_RTT_ReadUpBuffer(0, received, TEST_START) != TEST_START) {
            fprintf(stderr, "could not drain RTT prefix bytes\n");
            return 2;
        }
    }
    const unsigned before = _SEGGER_RTT.aUp[0].WrOff;
#if TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE
    TriceDirectWrite32(payload, 3);
#else
    TriceWriteDeviceRtt0((const uint8_t*)payload, sizeof(payload));
#endif
    const unsigned after = _SEGGER_RTT.aUp[0].WrOff;
    const unsigned read = SEGGER_RTT_ReadUpBuffer(0, received, sizeof(received));
    const unsigned wantRead = TEST_REJECT ? TEST_START : sizeof(payload);
    const unsigned wantOffset = TEST_REJECT ? before : (TEST_START + sizeof(payload)) % 64;
    const int matches = read == wantRead &&
        memcmp(received, TEST_REJECT ? (const void*)prefix : (const void*)payload, wantRead) == 0;
    printf("read=%u offset=%u data=%s rejected=%u\n", read, after, matches ? "ok" : "bad",
           TEST_REJECT && before == after ? 1u : 0u);
    return matches && after == wantOffset ? 0 : 3;
}
`

// rttTriceWriterConfiguration matches Trice and RTT sizes explicitly. The small
// buffer makes wrap and rejection easy to exercise. No framing scratch space is
// needed; diagnostics records protected writes rejected for lack of space.
const rttTriceWriterConfiguration = `// SPDX-License-Identifier: MIT
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_SINGLE_MAX_SIZE 16
#define TRICE_DATA_OFFSET 0
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_CGO 0
#define TRICE_CYCLE_COUNTER 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_CONFIG_WARNINGS 0
#define TRICE_BUFFER_SIZE_UP 64
#define TRICE_BUFFER_SIZE_DOWN 16
#define TRICE_SEGGER_RTT_PRINTF_BUFFER_SIZE 64
`

// TestTriceRTTWritersUseVendorBuffer verifies both native byte and optimized
// word writers against SEGGER's real control block: normal writes, exact-end
// wrap, split wrap and protection from writes that would overwrite unread data.
func TestTriceRTTWritersUseVendorBuffer(t *testing.T) {
	sourceDir, err := filepath.Abs(filepath.Join("..", "..", "src"))
	if !assert.NoError(t, err) {
		return
	}
	for _, compiler := range []string{"cc", "gcc", "clang"} {
		t.Run(compiler, func(t *testing.T) {
			if _, err := exec.LookPath(compiler); err != nil {
				t.Skipf("%s is not installed", compiler)
			}
			for _, mode := range []string{"8BIT", "32BIT"} {
				for _, tc := range []struct {
					name, start, reject, want string
				}{
					{"contiguous_write", "0", "0", "read=12 offset=12 data=ok rejected=0"},
					{"write_ending_at_buffer_boundary_wraps_to_zero", "52", "0", "read=12 offset=0 data=ok rejected=0"},
					{"split_write_preserves_bytes_across_wrap", "60", "0", "read=12 offset=8 data=ok rejected=0"},
					{"insufficient_space_keeps_unread_data_and_write_offset", "56", "1", "read=56 offset=56 data=ok rejected=1"},
				} {
					t.Run(mode+"/"+tc.name, func(t *testing.T) {
						project := t.TempDir()
						probe := filepath.Join(project, "probe.c")
						if !assert.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(rttTriceWriterConfiguration), 0o644)) ||
							!assert.NoError(t, os.WriteFile(probe, []byte(rttTriceWriteProbe), 0o644)) {
							return
						}
						executable := filepath.Join(project, "probe.exe")
						args := []string{"-std=c99", "-Wall", "-Wextra", "-Werror", "-DBUFFER_SIZE_UP=64", "-DTEST_START=" + tc.start, "-DTEST_REJECT=" + tc.reject,
							"-DTRICE_DIRECT_SEGGER_RTT_" + mode + "_WRITE=1",
							"-I", project, "-I", sourceDir, "-I", filepath.Join(sourceDir, "default_conf"), probe,
							filepath.Join(sourceDir, "triceStackBuffer.c"), filepath.Join(sourceDir, "SEGGER_RTT.c"), filepath.Join(sourceDir, "cobsEncode.c"), filepath.Join(sourceDir, "tcobsv1Encode.c"), "-o", executable}
						output, err := exec.Command(compiler, args...).CombinedOutput()
						if !assert.NoError(t, err, "%s", output) {
							return
						}
						output, err = exec.Command(executable).CombinedOutput()
						if assert.NoError(t, err, "%s", output) {
							assert.Equal(t, tc.want, strings.TrimSpace(string(output)))
						}
					})
				}
			}
		})
	}
}

// TestCGORTTSizesFollowSelectedVendorConfiguration verifies that legacy CGO
// aliases remain compatible and that they are no longer required. Changing
// SEGGER sizes must update Trice defaults; explicit conflicts must still fail
// when compiling the production source, including with TRICE_CGO enabled.
func TestCGORTTSizesFollowSelectedVendorConfiguration(t *testing.T) {
	root, err := filepath.Abs(filepath.Join("..", ".."))
	if !assert.NoError(t, err) {
		return
	}
	template, err := os.ReadFile(filepath.Join(root, "_test", "testdata", "cgoPackage.go"))
	if !assert.NoError(t, err) {
		return
	}
	// Only the size definitions belong in this isolated probe. Other template
	// options include package-relative paths and unrelated warning settings.
	var sizeFlags []string
	for _, line := range strings.Split(string(template), "\n") {
		if strings.HasPrefix(line, "// #cgo CFLAGS:") {
			for _, flag := range strings.Fields(strings.TrimPrefix(line, "// #cgo CFLAGS:")) {
				if strings.HasPrefix(flag, "-DTRICE_BUFFER_SIZE_DOWN=") || strings.HasPrefix(flag, "-DTRICE_SEGGER_RTT_PRINTF_BUFFER_SIZE=") {
					sizeFlags = append(sizeFlags, flag)
				}
			}
		}
	}
	if !assert.Len(t, sizeFlags, 2, "the legacy shared template contains both explicit RTT aliases") {
		return
	}
	const config = `// SPDX-License-Identifier: MIT
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_CGO 1
#define TRICE_CYCLE_COUNTER 0
#define TRICE_CONFIG_WARNINGS 0
`
	const probe = `// SPDX-License-Identifier: MIT
#include "trice.h"
#include <stdio.h>
int main(void) {
    printf("Trice=%u/%u RTT=%u/%u\n",
           (unsigned)TRICE_BUFFER_SIZE_DOWN, (unsigned)TRICE_SEGGER_RTT_PRINTF_BUFFER_SIZE,
           (unsigned)BUFFER_SIZE_DOWN, (unsigned)SEGGER_RTT_PRINTF_BUFFER_SIZE);
    return 0;
}
`
	for _, compiler := range []string{"cc", "gcc", "clang"} {
		t.Run(compiler, func(t *testing.T) {
			if _, err := exec.LookPath(compiler); err != nil {
				t.Skipf("%s is not installed", compiler)
			}
			for _, tc := range []struct {
				name, want   string
				flags        []string
				omitCoupling bool
				wantMismatch bool
			}{
				{"fallback_sizes", "Trice=16/64 RTT=16/64", nil, false, false},
				{"changed_vendor_sizes_are_inherited", "Trice=7/19 RTT=7/19", []string{"-DBUFFER_SIZE_DOWN=7", "-DSEGGER_RTT_PRINTF_BUFFER_SIZE=19"}, false, false},
				{"explicit_zero_sizes_are_inherited", "Trice=0/0 RTT=0/0", []string{"-DBUFFER_SIZE_DOWN=0", "-DSEGGER_RTT_PRINTF_BUFFER_SIZE=0"}, false, false},
				{"fallback_sizes_need_no_aliases", "Trice=16/64 RTT=16/64", nil, true, false},
				{"changed_vendor_sizes_need_no_aliases", "Trice=7/19 RTT=7/19", []string{"-DBUFFER_SIZE_DOWN=7", "-DSEGGER_RTT_PRINTF_BUFFER_SIZE=19"}, true, false},
				{"zero_vendor_sizes_need_no_aliases", "Trice=0/0 RTT=0/0", []string{"-DBUFFER_SIZE_DOWN=0", "-DSEGGER_RTT_PRINTF_BUFFER_SIZE=0"}, true, false},
				{"explicit_mismatches_still_fail_in_production_source", "", []string{"-DTRICE_BUFFER_SIZE_DOWN=0", "-DTRICE_SEGGER_RTT_PRINTF_BUFFER_SIZE=0"}, true, true},
			} {
				t.Run(tc.name, func(t *testing.T) {
					project := t.TempDir()
					probePath := filepath.Join(project, "probe.c")
					if !assert.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(config), 0o644)) ||
						!assert.NoError(t, os.WriteFile(probePath, []byte(probe), 0o644)) {
						return
					}
					args := []string{"-std=c99", "-Wall", "-Wextra", "-Werror", "-I", project,
						"-I", filepath.Join(root, "src"), "-I", filepath.Join(root, "src", "default_conf")}
					if !tc.omitCoupling {
						args = append(args, sizeFlags...)
					}
					args = append(args, tc.flags...)
					// Compile the production source: this is where inconsistent
					// configurations must still produce a compiler error.
					compileArgs := append(append([]string{}, args...), "-c", filepath.Join(root, "src", "trice.c"), "-o", filepath.Join(project, "trice.o"))
					output, err := exec.Command(compiler, compileArgs...).CombinedOutput()
					if tc.wantMismatch {
						// CGO must not bypass validation of explicit project values.
						assert.Error(t, err, "inconsistent values must remain a compiler error")
						assert.Contains(t, string(output), "TRICE_BUFFER_SIZE_DOWN != BUFFER_SIZE_DOWN")
						assert.Contains(t, string(output), "TRICE_SEGGER_RTT_PRINTF_BUFFER_SIZE != SEGGER_RTT_PRINTF_BUFFER_SIZE")
						return
					}
					if !assert.NoError(t, err, "%s", output) {
						return
					}
					executable := filepath.Join(project, "probe.exe")
					args = append(args, probePath, "-o", executable)
					output, err = exec.Command(compiler, args...).CombinedOutput()
					if !assert.NoError(t, err, "%s", output) {
						return
					}
					output, err = exec.Command(executable).CombinedOutput()
					if assert.NoError(t, err, "%s", output) {
						assert.Equal(t, tc.want, strings.TrimSpace(string(output)))
					}
				})
			}
		})
	}
}
