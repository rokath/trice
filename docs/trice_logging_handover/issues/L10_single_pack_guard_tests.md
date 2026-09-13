# Regression-test the existing single-pack requirement for deferred ID routing

**Local reference:** L10  
**Type:** Test hardening / documentation  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** The relevant compile-time `#error` checks already exist; this issue does not claim they are missing.

## Summary

Protect the existing incompatibility checks and explain their scope: current deferred per-ID routing requires individually handled events, not arbitrary mixed-event multi-pack output.

## Background and evidence

`src/trice.c` already rejects `TRICE_MULTI_PACK_MODE` in enabled ID-range branches for UARTA, UARTB, auxiliary outputs, and the inspected RTT path. The exact reachability of a guard depends on route activation. This is not a general restriction on TCOBS, host filtering, or every possible custom direct-output implementation.

## Reproduction or review scenario

Compile minimal fixtures for each relevant enabled output with a valid active range: single-pack should compile; multi-pack should fail with the routing diagnostic. Compile multi-pack without ID routing as a separate supported baseline where otherwise valid.

## Requested change

Add positive and negative compilation tests around the real configuration checks. Audit custom/direct routing separately and document its own contract rather than applying the deferred rule indiscriminately. Keep failure tests focused so that an unrelated missing backend stub does not masquerade as the intended rejection.

## Acceptance criteria and regression tests

- [ ] Every relevant active deferred route rejects multi-pack for the intended reason.
- [ ] Each corresponding valid single-pack fixture compiles.
- [ ] No-routing multi-pack fixtures retain their supported behavior.
- [ ] Partial-bound cases follow the policy from L09 and cannot bypass validation accidentally.
- [ ] No redundant unconditional `#error` is added that forbids unrelated valid configurations.
- [ ] Documentation states that future pre-pack filtering could remove this limitation without changing TCOBS itself, but is not implemented here.

## Compatibility and scope boundaries

Test and document the existing design. Do not implement multi-pack ID routing or a new target-level controller in this issue.

## Source references

- [src/trice.c](https://github.com/rokath/trice/blob/wip/src/trice.c) - existing `TRICE_MULTI_PACK_MODE` routing errors
- [src/triceDefaultConfig.h](https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h) - routing configuration defaults

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L02](L02_inclusive_routing_bounds.md), [L09](L09_consistent_routing_configuration.md), [L18](L18_central_manual_chapter.md). Local references are handover identifiers, not existing GitHub issue numbers.
