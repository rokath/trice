# Record the deferred proposal for dynamic target-side log-level control

**Local reference:** L20  
**Type:** Deferred design proposal - not a bug  
**Suggested priority:** Deferred - reopen only for a concrete requirement  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** UM proposal; existing output ID routing is not the same as early dynamic suppression.

## Summary

Add target runtime level control only if measured event rate, bandwidth, storage, or argument-evaluation cost justifies it.

## Background and evidence

The existing compact logging path and host selection are deliberate strengths. Deferred ID routing can restrict a destination but, in the inspected ring-buffer path, occurs after data preparation/encoding. A runtime threshold at acquisition could save additional work, but requires a defined control/configuration path, concurrency behavior, and filtering position.

## Reproduction or review scenario

A reopening request should show overflow, transmission load, bounded offline storage, or another resource limit. Quantify events per second and bytes per event. Include the cost of context providers and expensive argument expressions, not only the macro's buffer write.

## Requested change

Record deferral and conditions for reconsideration. If reopened, define how the level is set, how interrupt/task producers observe it, and exactly which work is skipped. Consider application-local control as well as a remote command path; a new remote protocol is not always necessary. If ID ranges implement thresholds, require a validated ID policy and history-safe migration.

## Acceptance criteria and regression tests

- [ ] The manual distinguishes existing static range routing from a dynamic acquisition-time level controller.
- [ ] The controller is not advertised as already implemented.
- [ ] No new level bytes or protocol changes are presumed necessary.
- [ ] A future proposal measures both filtered and unfiltered costs and defines disabled/default behavior.
- [ ] A future test plan covers concurrency, argument evaluation, buffering/packing, and control changes.
- [ ] Useful pre-failure context is considered when deciding what to discard.

## Compatibility and scope boundaries

No implementation is requested now. Do not add a check to every current log call without a justified requirement and measured impact.

## Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - Run-time Log-level Control draft
- [src/trice.c](https://github.com/rokath/trice/blob/wip/src/trice.c) - current output routing
- [src/triceRingBuffer.c](https://github.com/rokath/trice/blob/wip/src/triceRingBuffer.c) - deferred encode/output sequence

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L03](L03_validate_active_id_policy.md), [L10](L10_single_pack_guard_tests.md), [L18](L18_central_manual_chapter.md), [L19](L19_deferred_compile_time_levels.md). Local references are handover identifiers, not existing GitHub issue numbers.
