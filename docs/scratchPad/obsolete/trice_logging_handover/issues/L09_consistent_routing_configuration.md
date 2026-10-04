# Make ID-routing activation and partial-bound handling consistent across outputs

**Local reference:** L09  
**Type:** Bug / configuration validation  
**Suggested priority:** P1 - potentially unfiltered or missing output  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed UARTA/UARTB difference; other partial-bound cases require coverage.

## Summary

Equivalent routing configurations must not enable a range on one output while disabling it on another.

## Background and evidence

UARTA activates its range branch when either configured bound is nonzero (`||`). UARTB activates only when both are nonzero (`&&`). Auxiliary and RTT routes use a different defined-macro check. With a partially configured UART range, output can therefore be restricted or unrestricted depending on the backend. The multi-pack rejection is inside these activation branches, so activation also affects validation coverage.

## Reproduction or review scenario

Build a configuration matrix for each supported backend: no bounds/defaults; only lower; only upper; both; reversed interval; one-ID interval. Exercise both single-pack and multi-pack settings. Record whether the route activates and which IDs reach it.

## Requested change

Define one clear configuration policy, including the meaning of zero/default/undefined bounds. Reject ambiguous partial configurations or implement a documented open-ended convention consistently. Preserve the intended no-routing default. Validate active bounds against the configured ID format and reject reversed intervals. Do not blindly replace every `&&` with `||` without defining the contract.

## Acceptance criteria and regression tests

- [ ] All backends agree on equivalent complete, absent, and partial configurations.
- [ ] Invalid/ambiguous configurations fail at compile time with a useful message.
- [ ] Zero/default sentinel behavior is explicitly documented and tested.
- [ ] Range activation cannot accidentally bypass a multi-pack incompatibility check.
- [ ] Inclusive endpoint tests remain valid after normalization.
- [ ] Disabled outputs are not forced to satisfy irrelevant routing configuration unless deliberately documented.

## Compatibility and scope boundaries

Configuration changes can affect output contents. Treat existing partial-bound use as a migration case, not an unnoticed cleanup.

## Source references

- [src/trice.c](https://github.com/rokath/trice/blob/wip/src/trice.c) - `TriceNonBlockingDeferredWrite8` preprocessor guards
- [src/triceDefaultConfig.h](https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h) - UART bound defaults

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L02](L02_inclusive_routing_bounds.md), [L10](L10_single_pack_guard_tests.md). Local references are handover identifiers, not existing GitHub issue numbers.
