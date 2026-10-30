// SPDX-License-Identifier: MIT

package scripts_test

import (
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"runtime"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

// g0b1BuildFixture exercises the actual entry points without a compiler, ID
// installation or source edits. Make records real build requests separately from
// clean requests, so a successful no-op cannot masquerade as a completed build.
func g0b1BuildFixture(t *testing.T) string {
	t.Helper()
	root := scriptFixture(t, "examples/G0B1_inst/build.sh", "examples/G0B1_inst/build_with_clang.sh")
	writeFixture(t, root, "examples/prepareTriceBind.sh", `prepare_trice_bind_build() {
  echo prepared >> "$1/preparation.calls"
  export TRICE_ID_WORKFLOW_OWNER=1 TRICE_ID_WORKFLOW=bind
}
`)
	writeFixture(t, root, "scripts/_150_setup_build_environment.sh", "MAKE_JOBS=-j1\n")
	writeFixture(t, root, "bin/make", `#!/bin/sh
if [ "${1:-}" = clean ]; then echo clean >> ../../clean.calls; exit 0; fi
printf '%s\n' "$@" > ../../build.args
echo built >> ../../build.calls
`)
	return root
}

// TestG0B1ConfigurationCLI preserves existing define arguments and checks both
// compilers' cold builds, repeated builds and transitions back to configuration 0.
func TestG0B1ConfigurationCLI(t *testing.T) {
	for _, compiler := range []string{"gcc", "clang"} {
		t.Run(compiler, func(t *testing.T) {
			root := g0b1BuildFixture(t)
			entry := "build.sh"
			if compiler == "clang" {
				entry = "build_with_clang.sh"
			}
			for _, tc := range []struct{ arguments, flags string }{
				{"", ""}, {"98", "-DCONFIGURATION=98 "}, {"98", "-DCONFIGURATION=98 "},
				{"0", "-DCONFIGURATION=0 "}, {"CONFIGURATION=98", "-DCONFIGURATION=98 "},
				{"98 TRICE_OFF=1", "-DCONFIGURATION=98 -DTRICE_OFF=1 "},
			} {
				out, err := runFixture(t, root, "bash examples/G0B1_inst/"+entry+" "+tc.arguments,
					map[string]string{"TRICE_ID_WORKFLOW_OWNER": "0"})
				assert.NoError(t, err, out)
				args, err := os.ReadFile(filepath.Join(root, "build.args"))
				assert.NoError(t, err)
				assert.Contains(t, string(args), "TRICE_FLAGS="+tc.flags+"\n")
				assert.True(t, strings.HasSuffix(string(args), "\n"+compiler+"\n"), string(args))
			}
			calls, err := os.ReadFile(filepath.Join(root, "build.calls"))
			assert.NoError(t, err)
			expected := 6
			if compiler == "gcc" {
				expected++ // TRICE_OFF retains its additional no-ID verification build.
			}
			assert.Equal(t, expected, strings.Count(string(calls), "\n"))
		})
	}
}

// TestG0B1ConfigurationRejectsUnknownProfiles verifies that errors/help have no
// ID preparation or compiler side effects and point users to the central manual.
func TestG0B1ConfigurationRejectsUnknownProfiles(t *testing.T) {
	for _, entry := range []string{"build.sh", "build_with_clang.sh"} {
		for _, argument := range []string{"1", "99", "CONFIGURATION=97", "--help"} {
			t.Run(entry+"/"+argument, func(t *testing.T) {
				root := g0b1BuildFixture(t)
				out, err := runFixture(t, root, "bash examples/G0B1_inst/"+entry+" "+argument, nil)
				assert.Equal(t, argument != "--help", err != nil, out)
				assert.Contains(t, out, "docs/rm/examples.md#g0b1-clock-benchmark")
				for _, marker := range []string{"preparation.calls", "build.calls"} {
					_, err := os.Stat(filepath.Join(root, marker))
					assert.True(t, os.IsNotExist(err), marker)
				}
			})
		}
	}
}

// g0b1HostCompile runs small native behavioral tests with register doubles. It
// never links ARM firmware or shares mutable executable outputs across cases.
func g0b1HostCompile(t *testing.T, root string, sources []string, defines ...string) (string, error) {
	t.Helper()
	compiler, err := exec.LookPath("gcc")
	if err != nil {
		t.Skip("a native GCC-compatible C compiler is needed for the register-model tests")
	}
	executable := filepath.Join(root, "register-test")
	if runtime.GOOS == "windows" {
		executable += ".exe"
	}
	args := []string{"-std=c11", "-Wall", "-Wextra", "-Werror", "-Iinclude"}
	args = append(args, defines...)
	args = append(args, sources...)
	args = append(args, "-o", executable)
	command := exec.Command(compiler, args...)
	command.Dir = root
	output, err := command.CombinedOutput()
	if err != nil {
		return string(output), err
	}
	output, err = exec.Command(executable).CombinedOutput()
	return string(output), err
}

// TestG0B1ConfigurationKeepsNormalDefaults checks the actual project header,
// including the original stamp units and the fast profile's intentionally empty
// critical sections. Unsupported direct-Make selections must also fail clearly.
func TestG0B1ConfigurationKeepsNormalDefaults(t *testing.T) {
	for _, configuration := range []string{"default", "0", "98", "1"} {
		t.Run(configuration, func(t *testing.T) {
			root := scriptFixture(t, "examples/G0B1_inst/Core/Inc/triceConfig.h")
			writeFixture(t, root, "examples/G0B1_inst/Core/Inc/main.h", `#include <stdint.h>
typedef struct { uint32_t CNT; } Timer;
extern Timer timer2, timer17;
#define TIM2 (&timer2)
#define TIM17 (&timer17)
#define TRICE_RING_BUFFER 1
#define TRICE_DOUBLE_BUFFER 2
#define TRICE_MULTI_PACK_MODE 1
#define TRICE_FRAMING_COBS 1
uint32_t HAL_GetTick(void);
`)
			writeFixture(t, root, "include/cmsis_gcc.h", `#define __get_PRIMASK() 0u
#define __disable_irq() (++locks)
#define __set_PRIMASK(value) ((void)(value), ++unlocks)
`)
			writeFixture(t, root, "check.c", `#include <assert.h>
#include "examples/G0B1_inst/Core/Inc/triceConfig.h"
Timer timer2 = {0x12345678u}, timer17 = {123u};
uint32_t HAL_GetTick(void) { return 456u; }
int main(void) {
  unsigned locks=0, unlocks=0;
  TRICE_ENTER_CRITICAL_SECTION
  TRICE_LEAVE_CRITICAL_SECTION
#if CONFIGURATION == 0
  assert(TRICE_BUFFER == TRICE_RING_BUFFER);
  assert(TRICE_DEFERRED_BUFFER_SIZE == 2000);
  assert(TRICE_DIRECT_OUTPUT == 1 && TRICE_DEFERRED_XTEA_ENCRYPT == 1);
  assert(TriceStamp16 == 123u && TriceStamp32 == 456u);
  assert(locks == 1 && unlocks == 1);
#else
  assert(TRICE_BUFFER == TRICE_DOUBLE_BUFFER);
  assert(TRICE_DEFERRED_BUFFER_SIZE == 4096);
  assert(TRICE_DIRECT_OUTPUT == 0 && TRICE_DEFERRED_OUTPUT == 1);
  assert(TRICE_PROTECT == 0 && TRICE_DIAGNOSTICS == 0 && TRICE_CYCLE_COUNTER == 0);
  assert(TRICE_DEFERRED_XTEA_ENCRYPT == 0);
  assert(TriceStamp16 == 0x5678u && TriceStamp32 == 0x12345678u);
  assert(locks == 0 && unlocks == 0);
#endif
  return 0;
}
`)
			defines := []string{"-DTRICE_OFF=0"}
			if configuration != "default" {
				defines = append(defines, "-DCONFIGURATION="+configuration)
			}
			out, err := g0b1HostCompile(t, root, []string{"check.c"}, defines...)
			if configuration == "1" {
				assert.Error(t, err, out)
				assert.Contains(t, out, "supports CONFIGURATION=0")
			} else {
				assert.NoError(t, err, out)
			}
		})
	}
}

// TestG0B1FastRegisterModel compiles the actual benchmark against a deterministic
// counter/IRQ model. Values include wraparound and an empty block longer than a
// measured block. Every report must be published after every measured record.
func TestG0B1FastRegisterModel(t *testing.T) {
	for _, mask := range []string{"0", "1"} {
		t.Run("initial_IRQ_mask="+mask, func(t *testing.T) {
			root := scriptFixture(t, "examples/G0B1_inst/Core/Src/triceFast.c")
			data, err := os.ReadFile(filepath.Join(root, "examples/G0B1_inst/Core/Src/triceFast.c"))
			if !assert.NoError(t, err) {
				return
			}
			// The generated owner include is irrelevant to the fake Trice writer;
			// discover its stable filename rather than hard-coding a random key.
			for _, match := range regexp.MustCompile(`#include "(trice_sidecar_[^"]+)"`).FindAllStringSubmatch(string(data), -1) {
				writeFixture(t, root, "include/"+match[1], "/* register-model sidecar */\n")
			}
			writeFixture(t, root, "include/trice.h", `#include <stdint.h>
#define TRICE_DEFERRED_BUFFER_SIZE 4096
#define TRICE_DATA_OFFSET 64
typedef struct {
  uint32_t CR1, DIER, SMCR, PSC, ARR, EGR, SR;
  volatile uint32_t ticks[33];
} Timer;
extern Timer timer;
extern unsigned clockIndex;
extern uint32_t SystemCoreClock;
#define TIM2 (&timer)
/* Each register read obtains the next deterministic counter value. */
#define CNT ticks[clockIndex++]
#define TIM_CR1_CEN 1u
#define TIM_EGR_UG 1u
#define LL_APB1_GRP1_PERIPH_TIM2 1u
void LL_APB1_GRP1_EnableClock(uint32_t clock);
uint32_t __get_PRIMASK(void);
void __disable_irq(void);
void __set_PRIMASK(uint32_t mask);
void __DSB(void);
void fakeTrice(const char *format, ...);
#define trice(...) fakeTrice(__VA_ARGS__)
`)
			writeFixture(t, root, "check.c", `#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include "trice.h"
Timer timer;
unsigned clockIndex;
uint32_t SystemCoreClock = 64000000u;
static uint32_t irqMask = INITIAL_MASK;
static unsigned clockEnabled, disabled, restored, records, reports, descriptions;
/* Expected public results, deliberately independent of the production formula. */
static const unsigned overheads[8] = {4,8,100,4,4,4,4,4};
static const unsigned raws[8] = {100,104,4,96,116,132,148,164};
static const unsigned nets[8] = {96,96,0,92,112,128,144,160};
static const unsigned averages[8] = {6,6,0,5,7,8,9,10};
static const unsigned fractions[8] = {0,0,0,75,0,0,0,0};
void G0B1FastInit(void);
void G0B1FastMeasure(void);
void LL_APB1_GRP1_EnableClock(uint32_t clock) { assert(clock == 1u); ++clockEnabled; }
uint32_t __get_PRIMASK(void) { return irqMask; }
void __disable_irq(void) { irqMask=1u; ++disabled; }
void __set_PRIMASK(uint32_t mask) { assert(mask == INITIAL_MASK); irqMask=mask; ++restored; }
void __DSB(void) { }
void fakeTrice(const char *format, ...) {
  if (strstr(format, "att:G0B1 fast record")) {
    assert(irqMask == 1u && reports == 0u && descriptions == 0u);
    ++records;
    return;
  }
  assert(records == 128u && restored == 8u && clockIndex == 33u);
  assert(irqMask == INITIAL_MASK);
  if (strstr(format, "sample=%u raw=")) {
    va_list args;
    va_start(args, format);
    assert(va_arg(args, unsigned) == reports);
    assert(va_arg(args, unsigned) == raws[reports]);
    assert(va_arg(args, unsigned) == overheads[reports]);
    assert(va_arg(args, unsigned) == nets[reports]);
    assert(va_arg(args, unsigned) == averages[reports]);
    assert(va_arg(args, unsigned) == fractions[reports]);
    va_end(args);
    ++reports;
  } else ++descriptions;
}
int main(void) {
  G0B1FastInit();
  assert(clockEnabled == 1u && clockIndex == 1u && timer.ticks[0] == 0u);
  assert(timer.CR1 == TIM_CR1_CEN && timer.PSC == 0u && timer.ARR == UINT32_MAX);
  assert(timer.DIER == 0u && timer.SMCR == 0u && timer.EGR == TIM_EGR_UG && timer.SR == 0u);
  for (unsigned sample=0; sample<8; ++sample) {
    unsigned index=1+sample*4;
    timer.ticks[index]=1000u*sample;
    timer.ticks[index+1]=timer.ticks[index]+overheads[sample];
    timer.ticks[index+2]=sample == 3u ? UINT32_MAX-15u : 100000u+1000u*sample;
    timer.ticks[index+3]=timer.ticks[index+2]+raws[sample];
  }
  G0B1FastMeasure();
  assert(records == 128u && reports == 8u && descriptions == 2u);
  assert(disabled == 8u && restored == 8u && irqMask == INITIAL_MASK);
  return 0;
}
`)
			out, err := g0b1HostCompile(t, root, []string{"examples/G0B1_inst/Core/Src/triceFast.c", "check.c"},
				"-DCONFIGURATION=98", "-DTRICE_OFF=0", "-DINITIAL_MASK="+mask)
			assert.NoError(t, err, out)
		})
	}
}
