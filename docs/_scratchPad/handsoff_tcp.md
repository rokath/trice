# Codex handoff: Make Trice TCP tests reliable on Windows without firewall pop-ups

## Assignment

Investigate and fix the TCP-related test lifecycle defects in `rokath/trice`, using the **currently checked-out `wip` branch** as the source of truth. The original symptom is a Windows Defender Firewall approval dialog during `testAll.sh full`. Further isolation uncovered independently reproducible test failures and a hanging test when `go test -count=2` is used.

**Fix the repository, not the operating system.** Preserve actual TCP integration coverage and preserve **all production remote-network functionality**, especially the ability to connect to a Trice display server from a different computer.

Work toward a small, maintainable patch. Inspect the live branch before editing; code on `main` may have changed.

## Non-negotiable constraints

1. **Do not change Windows Firewall settings**, install firewall rules, request administrator privileges, use `netsh`/PowerShell policies, or ask users to dismiss/approve dialogs as the solution.
2. **Do not skip or disable tests on Windows** (`t.Skip`, build tags, or opt-in environment variables are not acceptable as the fix).
3. **Do not replace the existing TCP integration tests with `net.Pipe()` or mocks.** Real TCP listen/dial and transfer behavior must remain tested. Additional unit tests with fakes are fine but cannot replace TCP coverage.
4. **Do not restrict production listeners to localhost or otherwise degrade remote connectivity.** The Trice display server must remain reachable from remote hosts when configured accordingly. A loopback-only bind is appropriate **inside local tests only**.
5. Avoid unrelated refactoring or changes to production network defaults. If a narrowly scoped production change is truly needed for deterministic shutdown/testability, explain why and verify behavioral compatibility.
6. Keep tests usable on **Windows, Linux, and macOS**; no arbitrary fixed TCP ports, orphan listeners, runaway goroutines, unbounded waits, or reliance on a particular test order.

## Observed behavior (Windows 11, Git Bash, Go 1.26.1)

User ran the following commands from `~/repos/trice` on the `wip` branch:

Command / test selection | Observed outcome
--- | ---
`go test ./cmd/trice -run '^TestDisplayServerStartAndShutdown0$' -count=1 -v` | PASS, no dialog
`go test ./cmd/trice -run '^TestHEXToTCP$' -count=1 -v` | PASS, no dialog
`go test ./cmd/trice -run '^TestTCP4Reception$' -count=1 -v` | PASS, no dialog
`go test ./cmd/trice -count=1 -v` | PASS, firewall dialog **after** test completion
`go test ./cmd/trice -count=1 -v -skip '^TestHEXToTCP$'` | PASS, no dialog
`go test ./cmd/trice -count=1 -v -run '^(TestDisplayServerStartAndShutdown0\|TestTCP4Reception\|TestHEXToTCP)$'` | PASS, firewall dialog
`go test ./cmd/trice -count=1 -v -run '^(TestTCP4Reception\|TestHEXToTCP)$'` | PASS, firewall dialog
`go test ./cmd/trice -count=1 -v -run '^(TestDisplayServerStartAndShutdown0\|TestHEXToTCP)$'` | PASS, no dialog

Additional decisive reproductions:

### Repro A: repeated `TestTCP4Reception`

```text
go test ./cmd/trice -count=2 -v -run '^TestTCP4Reception$'
=== RUN   TestTCP4Reception
... PASS (0.02s)
=== RUN   TestTCP4Reception
... tcp4_test.go:66: Expected nil, but got: &net.OpError{Op:"listen", ...}
panic: runtime error: invalid memory address or nil pointer dereference
... tcp4_test.go:71
FAIL
```

### Repro B: repeated `TestHEXToTCP`

```text
go test ./cmd/trice -count=2 -v -run '^TestHEXToTCP$'
=== RUN   TestHEXToTCP
Listening on localhost:8081...
... PASS (0.50s)
=== RUN   TestHEXToTCP
Listening on localhost:8081...
[no further progress; command hangs]
```

The firewall dialog does **not** appear in those last two runs, according to the user. It is not yet proven that the popup and both lifecycle defects have the same underlying cause. Firewall notification timing/state can make occurrence non-deterministic. Do not claim causality without verification.

## Suspect code to inspect

The following is visible in the public `main` version of `cmd/trice/tcp4_test.go`; **re-check the local `wip` version first**:

- `TestTCP4Reception` uses `net.Listen("tcp", ":54321")`: a fixed port and a wildcard bind, not a loopback-only bind.
- It starts an `Accept()` loop in a goroutine, with no evident listener close/join on normal test exit.
- On `Accept()` errors it prints the error and continues, potentially looping forever after shutdown. A failed `net.Listen` is only checked with `assert.Nil`, so execution continues with an invalid listener and can panic.
- `TestHEXToTCP` configures `-tcp localhost:8081`, starts `args.Handler(...)` in a goroutine, and sleeps for 500 ms before dialing `":8081"`. There is no evident server-goroutine shutdown/join and no explicit close of the client connection.
- Its goroutine calls a test assertion. Avoid invoking `require`/`Fatal` or otherwise causing unsafe test-lifecycle behavior from background goroutines; report asynchronous errors via channels and inspect them in the main test goroutine.
- `TestDisplayServerStartAndShutdown0` in `main_test.go` already uses a loopback address and an explicit shutdown path; it passed independently. Preserve that coverage and avoid needlessly changing it.

Reference only (not necessarily identical to `wip`):

- <https://github.com/rokath/trice/blob/main/cmd/trice/tcp4_test.go>
- <https://github.com/rokath/trice/blob/main/cmd/trice/main_test.go>

Also inspect the production components exercised by `-tcp` and `TCP4BUFFER` to understand how a listener exits and where readiness/shutdown can be observed. The **test harness** is the preferred place to fix lifecycle problems.

## Required implementation approach

### 1. Fix `TestTCP4Reception`

- Use a **real TCP/IPv4 listener** bound to `127.0.0.1:0`; derive the assigned port from `ln.Addr()` and pass it to the Trice client.
- Fail immediately and cleanly if `net.Listen` fails (`require.NoError` / `t.Fatal` before starting dependent goroutines). Prevent nil-pointer panics.
- Close the listener on every exit path (e.g. `t.Cleanup` or a correctly structured `defer`).
- Make the server goroutine terminate and **wait for termination** before the test returns. A successful `Accept` should handle the intended connection; a closed listener should terminate rather than loop forever. Treat `net.ErrClosed` during expected shutdown as normal.
- Collect/report unexpected asynchronous errors without unbounded blocking or data races. Keep the existing real-data COBS/encrypted TCP4BUFFER reception and decoded-output assertions.

### 2. Fix `TestHEXToTCP`

- Keep the actual `-tcp` transport test and its data assertion. Configure the **test** to use an explicitly local IPv4 address (`127.0.0.1`), not an unspecified or wildcard address; do **not** change the production `-tcp` behavior.
- Avoid hard-coding port `8081`. Prefer an OS-assigned port and a sound way of passing the selected address to the server. Do not introduce a new bind race through fragile reserve/close/rebind unless justified.
- Replace the unconditional `time.Sleep(500 * time.Millisecond)` with explicit readiness synchronization or a **bounded** readiness probe, with early failure if the server exits and a clear timeout error.
- Explicitly close the client connection, terminate the server/handler, and wait until all related goroutines have exited before returning from the test. Inspect the existing `args.Handler`/`-tcp` lifecycle and choose the least invasive correct shutdown mechanism. If the production command is intentionally long-running, it is acceptable to isolate it in a managed subprocess or introduce a minimal testable lifecycle seam, but document the design and ensure no behavioral regression.
- Avoid goroutine leaks, test assertions made after the test returns, accidental global flag leakage, and unbounded read/write waits. Use deadlines where appropriate.

### 3. Inspect for similar test-only defects

Search **test files** for `net.Listen`, wildcard binds (`":port"`, `"0.0.0.0:port"`), fixed ports, goroutines that are not joined, missing `Close()`, and sleeps used as readiness signaling. Fix closely related instances if needed for `testAll.sh full`, but keep the patch scoped and explain any additional changes.

Do not globally replace wildcard binds or remote-facing production listeners with loopback: that would violate the project's network requirements.

### 4. Protect coverage and regressions

The two named tests should still exercise real TCP sockets, actual `args.Handler` data paths and their original output/data expectations. Add assertions or small regression tests if helpful to make repeated invocation and cleanup verifiable. Do not weaken assertions just to make tests pass.

## Validation

Run as much as your environment supports; **report what was executed, on which OS, and what could not be verified**.

```bash
# Repeatability of each previously broken test
go test ./cmd/trice -count=2 -v -run '^TestTCP4Reception$' -timeout 90s
go test ./cmd/trice -count=2 -v -run '^TestHEXToTCP$' -timeout 90s

# Stronger repeatability check
go test ./cmd/trice -count=10 -run '^(TestTCP4Reception|TestHEXToTCP)$' -timeout 120s

# Previously dialog-triggering pair
go test ./cmd/trice -count=1 -v -run '^(TestTCP4Reception|TestHEXToTCP)$' -timeout 90s

# Whole package
go test ./cmd/trice -count=1 -v -timeout 120s

# Where toolchain/CGO supports it
go test -race ./cmd/trice -count=2 -timeout 180s

# Full repository test orchestration; inspect script first
./testAll.sh full
```

For the Windows popup, have the user **manually** run the previously triggering pair and full `testAll.sh full` on Windows and wait briefly after program exit. Automated tests alone cannot establish absence of a GUI firewall notification. If the dialogue persists, identify the remaining test/listener rather than hiding it or disabling the test. Do not report the GUI issue as fixed until the Windows reproduction is confirmed.

## Acceptance criteria

- `TestTCP4Reception` passes repeatedly under `-count=2` and `-count=10`, with no port conflict, nil-pointer panic, or leaked listener/goroutine.
- `TestHEXToTCP` passes repeatedly under `-count=2` and `-count=10`, without hanging or leaking its server/client resources.
- Existing real TCP integration coverage and output expectations remain intact; no tests are skipped on Windows.
- Any loopback-only restrictions apply **only to local test fixtures**, never to the production display server or other network features.
- The package and, where available, `testAll.sh full` complete successfully on Windows and do not regress Linux/macOS.
- The Windows Firewall popup is rechecked after the changes. If not resolved, provide a concrete remaining diagnosis and follow-up; do not suppress the popup through OS configuration or false claims.

## Deliverables

1. A focused code diff implementing the fixes.
2. A brief root-cause explanation for each repeated-test failure and, if verified, for the Windows firewall popup.
3. Exact test commands and outcomes by OS.
4. Explicit confirmation that production remote access (including the Trice display server) remains unchanged.
5. Any outstanding uncertainties, especially if Windows GUI behavior could not be tested from the execution environment.

**Priority:** correctness and full TCP test coverage first, deterministic teardown second, elimination of the Windows dialog through repository changes third. No Windows-side workaround.
