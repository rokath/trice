# Reject simultaneous pick and ban options during startup validation

**Local reference:** L08  
**Type:** Bug / error handling  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed late fatal check.

## Summary

The unsupported combination of `-pick` and `-ban` must be rejected before processing any input, including when the input is empty or a selector is invalid.

## Background and evidence

The mutual-exclusion check currently runs in `banOrPickFilter`, after decoding has reached that path. A command with no events may never reach the check. A mistyped selector can also disappear before the conflict is detected. The deep filter path uses fatal process termination rather than a startup configuration error.

## Reproduction or review scenario

Invoke both switches with a valid pair, with an unknown token, and with an empty input. Assert that no receiver or output sink is opened and no process termination occurs inside a per-event unit test.

## Requested change

Track whether each option was supplied and validate mutual exclusion during CLI configuration finalization. Return a normal command error at that boundary. Keep the intended prohibition rather than inventing combined semantics in a bug fix.

## Acceptance criteria and regression tests

- [ ] Both valid options always fail before acquisition, even with zero input events.
- [ ] Unknown selectors do not conceal the conflict; diagnostics remain actionable.
- [ ] Unit tests do not need to trigger a deep `os.Exit` to verify the policy.
- [ ] Either option alone still works; repeated occurrences of the same option remain supported.
- [ ] Both public logging entry points behave identically.

## Compatibility and scope boundaries

No target or wire-format change. Supporting pick plus ban together would be a separate interface decision.

## Source references

- [internal/emitter/emitter.go](https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go) - `banOrPickFilter`
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - `runLog` and receiver setup

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L07](L07_reject_unknown_selectors.md), [L17](L17_early_host_filtering.md). Local references are handover identifiers, not existing GitHub issue numbers.
