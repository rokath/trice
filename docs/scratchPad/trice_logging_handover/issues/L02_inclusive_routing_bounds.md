# Use inclusive ID-routing bounds consistently with ID allocation

**Local reference:** L02  
**Type:** Bug  
**Suggested priority:** P1 - possible missing routed events  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed mismatch between allocation, comments, and routing comparisons.

## Summary

IDs allocated at the configured minimum or maximum must not disappear from an output whose documented range includes those endpoints.

## Background and evidence

`PreProcessing` enumerates candidate IDs using `id <= max`, starting at `min`. The configuration describes UART limits as the smallest/largest routed ID. In `TriceNonBlockingDeferredWrite8`, UARTA, UARTB, auxiliary outputs, and the guarded RTT path instead compare `min < triceID && triceID < max`.

For configured limits 10 and 99, the current comparisons forward only 11 through 98. A single-ID interval forwards nothing.

## Reproduction or review scenario

In a routing test, set a non-disabled range to 10..99 and send IDs 9, 10, 11, 98, 99, and 100. Capture the backend calls rather than depending on serial hardware. Repeat with a one-ID interval such as 42..42.

## Requested change

Use one documented inclusive interval convention for allocation and routing. Audit all ID-limited output paths, not only UARTA. Keep the separate meaning of disabled/default routing configuration intact; changing `<` to `<=` must not accidentally activate a disabled route.

## Acceptance criteria and regression tests

- [ ] The 10..99 route forwards 10 and 99, but not 9 or 100.
- [ ] An enabled 42..42 route forwards exactly ID 42.
- [ ] All enabled affected UART, auxiliary, and RTT paths use the same interval convention.
- [ ] Tests include legal global ID endpoints and no-range/default configuration.
- [ ] Allocation and routing tests share boundary fixtures, including upward/downward allocation endpoints.
- [ ] No additional bytes or changes to frame content are introduced.

## Compatibility and scope boundaries

Existing users may have widened limits as a workaround. Document the semantic correction and migration. Do not alter historical dictionary IDs as part of this fix.

## Source references

- [src/trice.c](https://github.com/rokath/trice/blob/wip/src/trice.c) - `TriceNonBlockingDeferredWrite8` and ID comparisons
- [src/triceDefaultConfig.h](https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h) - `TRICE_UARTA_MIN_ID`, `TRICE_UARTA_MAX_ID`, UARTB counterparts
- [internal/id/switchIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go) - `PreProcessing`

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L03](L03_validate_active_id_policy.md), [L09](L09_consistent_routing_configuration.md), [L10](L10_single_pack_guard_tests.md). Local references are handover identifiers, not existing GitHub issue numbers.
