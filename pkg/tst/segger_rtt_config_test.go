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
				flags                       []string
				defaultFirst                bool
			}{
				{name: "missing_project_header_uses_fallback", want: "up=1024 down=16 channels=1/1 mode=0 written=5 read=5 locks=0 data=hello"},
				{name: "project_header_replaces_sizes_mode_and_locks", configDirectory: "project", want: "up=64 down=8 channels=2/1 mode=1 written=5 read=5 locks=1 data=hello"},
				{name: "generated_header_also_precedes_fallback", configDirectory: "generated", want: "up=64 down=8 channels=2/1 mode=1 written=5 read=5 locks=1 data=hello"},
				{name: "command_line_can_override_fallback_buffer_size", flags: []string{"-DBUFFER_SIZE_UP=128"}, want: "up=128 down=16 channels=1/1 mode=0 written=5 read=5 locks=0 data=hello"},
				{name: "ordinary_windows_build_needs_no_embos_functions", flags: []string{"-DWIN32"}, want: "up=1024 down=16 channels=1/1 mode=0 written=5 read=5 locks=0 data=hello"},
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
						if !assert.NoError(t, os.WriteFile(path, []byte(rttProjectConfiguration), 0o644)) {
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
