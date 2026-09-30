// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"sync"
	"testing"
	"time"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// reloadLogBuffer permits assertions while exec copies a running logger's output.
type reloadLogBuffer struct {
	mu sync.Mutex
	b  bytes.Buffer
}

func (b *reloadLogBuffer) Write(p []byte) (int, error) {
	b.mu.Lock()
	defer b.mu.Unlock()
	return b.b.Write(p)
}

func (b *reloadLogBuffer) String() string {
	b.mu.Lock()
	defer b.mu.Unlock()
	return b.b.String()
}

// TestLiveReloadLoggerProcess is the isolated logging process for the test
// below. Reusing this test binary also preserves race instrumentation when the
// parent is run with -race; no installed trice executable or hardware is needed.
func TestLiveReloadLoggerProcess(t *testing.T) {
	if os.Getenv("TRICE_LIVE_RELOAD_TEST") != "1" {
		return
	}
	for i, argument := range os.Args {
		if argument == "--" {
			FlagsInit()
			if err := Handler(os.Stdout, &afero.Afero{Fs: afero.NewOsFs()}, append([]string{"trice", "log"}, os.Args[i+1:]...)); err != nil {
				fmt.Fprintln(os.Stderr, err)
				os.Exit(2)
			}
			os.Exit(0)
		}
	}
	t.Fatal("missing logger arguments")
}

// TestLiveReloadUpdatesRunningLogger proves the public command is wired to the
// watcher: a single process decodes changed field schemas and locations after
// ordinary saves and repeated rename replacements. Bad JSON keeps logging with
// the previous schema; a later valid save recovers. Visualization stays active.
func TestLiveReloadUpdatesRunningLogger(t *testing.T) {
	executable, err := os.Executable()
	require.NoError(t, err)
	for _, format := range []string{"text", "json", "kv"} {
		t.Run(format, func(t *testing.T) {
			root := t.TempDir()
			tilPath, liPath := filepath.Join(root, "til.json"), filepath.Join(root, "li.json")
			inputPath := filepath.Join(root, "capture.bin")
			require.NoError(t, os.WriteFile(inputPath, nil, 0o600))
			// rename replacement is exactly the relevant filesystem operation;
			// fallback removal matches platforms that cannot replace an open name.
			save := func(path string, data []byte, atomic bool) {
				t.Helper()
				if !atomic {
					require.NoError(t, os.WriteFile(path, data, 0o600))
					return
				}
				temporary := path + ".next"
				require.NoError(t, os.WriteFile(temporary, data, 0o600))
				if err := os.Rename(temporary, path); err != nil {
					require.NoError(t, os.Remove(path))
					require.NoError(t, os.Rename(temporary, path))
				}
			}
			saveTables := func(field string, line int, atomic bool) {
				t.Helper()
				save(tilPath, []byte(fmt.Sprintf(`{"100":{"Type":"TRICE32","Strg":"msg:%s={%s}\n"}}`, field, field)), atomic)
				save(liPath, []byte(fmt.Sprintf(`{"100":{"File":"%s.c","Line":%d}}`, field, line)), atomic)
			}
			saveTables("initial", 10, false)
			command := exec.Command(executable, "-test.run=^TestLiveReloadLoggerProcess$", "--",
				"-p", "FILE", "-args", inputPath, "-pf", "none", "-i", tilPath, "-li", liPath,
				"-hs", "off", "-ts", "off", "-prefix", "", "-suffix", "", "-color", "none", "-liFmt", "%s:%d ",
				"-logFormat", format, "-logfile", "application.log", "-v", "-vis", `msg:printf("%d\n",v0)@values.txt`)
			command.Dir = root
			command.Env = append(os.Environ(), "TRICE_LIVE_RELOAD_TEST=1")
			var application, diagnostics reloadLogBuffer
			command.Stdout, command.Stderr = &application, &diagnostics
			require.NoError(t, command.Start())
			t.Cleanup(func() {
				_ = command.Process.Kill()
				_ = command.Wait()
				assert.NotContains(t, diagnostics.String(), "DATA RACE")
			})
			require.Eventually(t, func() bool { return strings.Contains(diagnostics.String(), "Reloaded "+tilPath) }, 5*time.Second, 10*time.Millisecond, "logger must start its watcher")
			// Feed complete, unstamped 32-bit records while reload can happen.
			// The first few records may legitimately use the preceding tables.
			observe := func(field string, line int, value uint32) {
				t.Helper()
				packet := binary.LittleEndian.AppendUint16(nil, 0x4064)
				packet = binary.LittleEndian.AppendUint16(packet, 0x04c0)
				packet = binary.LittleEndian.AppendUint32(packet, value)
				want := fmt.Sprintf("%s=%d", field, value)
				require.Eventually(t, func() bool {
					f, err := os.OpenFile(inputPath, os.O_APPEND|os.O_WRONLY, 0o600)
					if err != nil {
						return false
					}
					_, err = f.Write(packet)
					_ = f.Close()
					if err != nil {
						return false
					}
					for _, outputLine := range strings.Split(application.String(), "\n") {
						if strings.Contains(outputLine, want) && strings.Contains(outputLine, field+".c") && strings.Contains(outputLine, fmt.Sprint(line)) {
							if format == "json" {
								return strings.Contains(outputLine, fmt.Sprintf(`"fields":{"%s":%d}`, field, value))
							}
							if format == "kv" {
								return strings.Contains(outputLine, fmt.Sprintf("field.%s=%d", field, value))
							}
							return true
						}
					}
					return false
				}, 5*time.Second, 20*time.Millisecond, "missing %s at %s.c:%d; stdout=%s stderr=%s", want, field, line, application.String(), diagnostics.String())
			}
			observe("initial", 10, 41)
			for index, field := range []string{"written", "renamed", "renamed_again"} {
				saveTables(field, 20+index, index > 0)
				observe(field, 20+index, 42+uint32(index))
			}
			save(tilPath, []byte(`{"100":`), false)
			require.Eventually(t, func() bool { return strings.Contains(diagnostics.String(), "keeping last valid table") }, 3*time.Second, 5*time.Millisecond)
			observe("renamed_again", 22, 55)
			saveTables("recovered", 30, true)
			observe("recovered", 30, 66)
			values, err := os.ReadFile(filepath.Join(root, "values.txt"))
			require.NoError(t, err)
			assert.Contains(t, string(values), "66\n", "visualization uses the reloaded numeric record")
			assert.NotContains(t, application.String(), "Reloaded")
			assert.NotContains(t, application.String(), "cannot reload")
			logfile, err := os.ReadFile(filepath.Join(root, "application.log"))
			require.NoError(t, err)
			assert.Contains(t, string(logfile), "recovered=66")
			assert.NotContains(t, string(logfile), "Reloaded")
			assert.NotContains(t, string(logfile), "cannot reload")
			if format == "json" {
				// Inspect complete lines only: the logger continues appending until cleanup.
				lines := strings.Split(application.String(), "\n")
				for _, line := range lines[:len(lines)-1] {
					assert.True(t, json.Valid([]byte(line)), "every stdout line must remain an NDJSON record: %q", line)
				}
			}
		})
	}
}
