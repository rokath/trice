# Register all user labels before resolving pick and ban selectors

**Local reference:** L05  
**Type:** Bug  
**Suggested priority:** P1 - unreliable CLI selection  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed initialization order.

## Summary

`-ulabel motor -pick motor` must work regardless of the order of these CLI options.

## Background and evidence

`channelArrayFlag.Set` resolves aliases during `flag.Parse`. `runLog` invokes `AddUserLabels` only after parsing. Thus a label introduced in the same command is not present when the selector is resolved; putting `-ulabel` first does not solve it because registration is still deferred.

`AddUserLabels` also has a process-wide once guard. Changes must account for repeated command/test invocations rather than leaving stale global registry state.

## Reproduction or review scenario

Exercise both `tlog -ulabel motor -pick motor` and `tlog -pick motor -ulabel motor` through the argument parser with a controlled input fixture. Repeat with `-ban`, multiple labels, and `trice log` as the entry point. Use a fresh registry for each case.

## Requested change

Collect raw option values first, register and validate all user labels second, and resolve/validate selectors third, before opening an input stream or creating output files. Define registry lifecycle explicitly for tests and in-process command reuse.

## Acceptance criteria and regression tests

- [ ] Both argument orders resolve the same user label correctly for pick and ban.
- [ ] `tlog` and `trice log` share the corrected behavior.
- [ ] Registry initialization does not leak labels or selectors between independent invocations.
- [ ] Unknown labels still cause a useful error after registration is complete.
- [ ] Built-in alias collision checks run for user labels as well.
- [ ] Insert/bind ID-range handling continues to recognize valid user tags.

## Compatibility and scope boundaries

No target change. Avoid a fix that merely relies on option ordering or weakens unknown-selector validation.

## Source references

- [internal/emitter/emitter.go](https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go) - `channelArrayFlag.Set`
- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `AddUserLabels`, `userLabelsAdded`
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - `runLog`, insert/bind flow

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L01](L01_unique_tag_aliases.md), [L06](L06_ulabel_colon_lists.md), [L07](L07_reject_unknown_selectors.md). Local references are handover identifiers, not existing GitHub issue numbers.
