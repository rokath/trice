# Native testing handover

Updated: 2026-10-27. Continue the analysis of failing checks on PR #763, `wip` into `main`, in `rokath/trice`. The goal remains reliable native Windows, Linux and macOS tests, including fresh preparation.

## Scope and repository state

* The user's latest technical instruction was **analysis only, no fixes**. This handover is authorized; implementation needs a subsequent instruction. No code, workflow or AGENTS changes were made during the CI investigation.
* Current branch: `wip`, HEAD `f34386a9714f2c56ef6d1e8379c1c9da72bfe71c`. The working tree was clean before this handover update; only this file is now uncommitted.
* GitHub PR metadata confirmed the same head, base `12ee0c1f372a46c3d9934c2985464e410c32a4b9`, and synthetic merge commit `76d31f8e69f35d2d56cd181ffb290a1ca23562ee`. CI checked that merge commit. GitHub reported `mergeable: true`: the displayed failures are CI results, not a textual merge conflict. Required branch-protection checks were not inspected.
* The Bash 3.2 report-publication fixture fix (`8cb5d2c8`), regression rules (`13d11b5a`) and RM documentation update (`131ab123`) are already included. Do not repeat their merge or treat them as uncommitted.
* The local macOS full suite has finished, restored tracked sources and removed its lock. No suite was stopped for this handover.

## CI findings

All IDs below belong to `rokath/trice` and the PR head above.

| Check | Evidence and conclusion |
| --- | --- |
| Native Linux | Run `40491254118`, job `122145465477`: quick suite INCOMPLETE at step 500; remaining steps passed. Raw artifact confirms plain `go: downloading ...` lines before valid JSON test events. |
| Native macOS ARM | Same run, job `122145465958`: same step-500 parser error; remaining steps passed. Its raw artifact was not inspected, so the shared download-output cause is strongly indicated, not independently confirmed. |
| Native Windows | Same run, job `122145465686`: preparation stops before test steps with `INCOMPLETE: prepare jq 1.8.2: <nil>`. Exact reason for the identity check failure remains unknown. |
| Library CI | Run `40491254107`, job `122145465722`: step 515 fails with the same JSON parser error on its first Go integration call; the later build orchestrator is skipped. Shared cause likely; no raw evidence file retrieved. |
| Script portability | Run `40491253885`: native Linux, macOS and Windows jobs all PASS. These isolated tests do not establish that real cold preparation works. |
| Coverage | Run `40491253839`, job `122145464016`: tests and upload PASS; Go reports 89.8% statement coverage. Coveralls separately fails with the screenshot's roughly 0.9 percentage-point decrease to 88.119% line coverage, displayed twice. File-level cause and comparison baseline remain unverified. |

The Link Check and install-checks workflows also passed. Intel macOS native acceptance and the Library inactivity guard were skipped as configured for this PR; do not count skipped jobs as passes.

### Confirmed JSON evidence defect

`run_go_tests` in `scripts/_100_test_common.sh` redirects both stdout and stderr from `go test -json` into one evidence file with `>"$evidence" 2>&1`. The strict JSON decoder in `scripts/testcheck/main.go` stops at the first non-JSON line.

The Linux raw file `testAll.b533pT/_500_test_bind.log.go.JxSfvi` begins with downloads of testify, equalfile, go-difflib and go-spew, followed by valid JSON events including passing tests. The auditor instead reports `invalid character 'g' looking for beginning of value`, then missing package/test evidence. Those missing-test messages are consequences of the parser abort, not proof that the tests never ran. The complete event stream was not independently re-audited during analysis.

A warm module cache hides this defect because it emits no download messages. Proposed correction, **not implemented**: preserve diagnostics separately from the JSON stream while retaining exit status, required-test checks and strict rejection of invalid/incomplete evidence. Do not merely discard arbitrary non-JSON lines or weaken acceptance.

### Windows tool identity gap

`scripts/testcheck/tools.go` reports `prepare ...: <nil>` when download/copy has no recorded error but `toolIdentity` returns false. That function discards the actual `--version` output and execution error. Consequently the log does not distinguish a version mismatch from an executable/startup problem.

The earlier messages rejecting globally installed compiler versions lead into bundle preparation; the final observed blocker is jq. Inspect the pinned Windows jq binary and its actual output/start status before changing versions or matching rules. Test native Windows cold and warm paths; do not infer success from mocked tools.

### Coverage uncertainty

Coveralls build: [82630620](https://coveralls.io/builds/82630620); uploaded job: [190350873](https://coveralls.io/jobs/190350873). Its web details could not be fetched during analysis. The Actions coverage log was available. Several testcheck functions have partial coverage, but this alone does not prove the cause of the total decrease. Compare covered lines by file with the actual target baseline; Go's rounded statement percentage is a different measure.

## Evidence retrieval and relevant files

Use the GitHub connector for PR/jobs/logs when available. The Linux ZIP was read with `gh api repos/rokath/trice/actions/artifacts/12729059306/zip` and decoded in memory; it was not saved locally. No signed download URL, credential or local helper is needed for continuation.

Native run `40491254118` retains these artifacts:

| Artifact | ID |
| --- | --- |
| `native-ubuntu-24.04-quick` | `12729059306` |
| `native-macos-26-quick` | `12730041974` |
| `native-windows-2025-quick` | `12729112968` |

Relevant code: `scripts/_100_test_common.sh`, `scripts/_500_test_bind.sh`, `scripts/_515_test_logging_features.sh`, `scripts/testcheck/main.go`, `scripts/testcheck/main_test.go`, `scripts/testcheck/tools.go`, its tool tests, `scripts/portability_test.go` and `scripts/test-tools.env`. The general script fixture replaces `run_go_tests` with a simpler command runner, which helps explain why passing fixture tests did not catch the real JSON capture problem.

Relevant workflows: `.github/workflows/native-tests.yml`, `native-tests-reusable.yml`, `trice_lib_reusable.yml`, `script-portability.yml` and `coverage.yml`. User documentation remains in [Running the Tests](../rm/target_tests.md) and [Test Internals](../rm/target_test_internals.md).

## Local validation

* The user's macOS ARM full run `temp/log/testAll.hfXsK4` ended **PASS**, 2503 seconds, at 2026-10-27 12:55:36 CET. All 25 steps passed and tracked-state verification completed. This was an existing user run, not a new agent-started test. Its warm local environment does not resolve the cold CI defect.
* Step 24, `_640_test_pc_targets_bind.sh`, took 140 seconds in that run, versus 391 seconds in the previous run. This supports a cache/build-cost explanation for the reported slowdown; no controlled benchmark or performance fix was performed.
* Earlier Bash 3.2 fix validation: complete `scripts` package PASS in 378.574 seconds using Go 1.27.3 on native macOS; five focused repetitions also passed. These checks preceded the current CI diagnosis.
* The CI investigation performed only reads. No corrective patch, new runtime test, CI rerun, commit or push was performed.

## Binding decisions and next steps

* Keep native execution on all three OS, Bash 3.2 compatibility, separate worktree caches and strict test-evidence checks. No Linux-container substitute, sleeps/retries, relaxed assertions, or coverage-threshold reduction to manufacture green checks.
* Universal read permission already exists in AGENTS.md and was explicitly reaffirmed. Do not ask for it again or expand AGENTS merely to address sandbox enforcement. Technical platform approval may still be unavoidable; do not duplicate pending requests.
* Plain `commit` means copyable commands only. Do not execute Git writes without explicit execution authorization. Never edit a checkout while its suite owns the state snapshot; preserve archived material.
* On the other PC, load this handover, verify checkout/HEAD and running tests, then wait for the user's next instruction. The previous analysis-only limit remains binding.
* Once implementation is requested: address stdout/stderr capture with cold-cache regression cases; expose and diagnose Windows jq identity failures; investigate Coveralls against its baseline as a separate concern. Validate affected direct commands, suite, CI and release hooks, complete affected Go packages and native OS jobs on the same final revision.
* Commit and transfer this handover before switching computers. Local logs/caches are optional evidence and need not be copied; retain the run/artifact IDs above.
