# Make tag aliases unambiguous and validate the complete tag registry

**Local reference:** L01  
**Type:** Bug / regression coverage  
**Suggested priority:** P1 - correctness  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed ambiguity; runtime reproduction still required.

## Summary

The same tag spelling currently identifies different groups. Alias resolution, level filtering, coloring, ID-range assignment, and statistics must agree on one interpretation.

## Background and evidence

`Tags` in `lineTransformerANSI.go` contains cross-group duplicates: `w` (Warning/Write), `rx` (Read/Receive), `tx` (Write/Transmit), and `s`/`S` (Signal/seconds). `FindTagName` and `tagVariants` return the first matching group. The level/counting loop continues through later matches, while color selection returns on the first match. Therefore one spelling can select one category but receive another effective threshold position and more than one counter increment.

This is not a request to collapse legitimate upper/lower-case variants. Tag spelling currently also affects whether the prefix is removed from text output.

## Reproduction or review scenario

Use the existing emitter test harness with `w:example`, `-pick wrn`, and `-logLevel wrn`. Check canonicalization, selection, effective level, color group, and counts separately. Enumerate every exact spelling in all groups to reproduce the duplicate registry entries without a device.

## Requested change

Choose and document an explicit owner for each conflicting spelling. Introduce one canonical registry/lookup policy and validation for built-in and user-added tags. Reject a spelling assigned to two groups instead of relying on table order. Handle a redundant repeated registration in the same group explicitly. Review source compatibility before removing an established short alias.

## Acceptance criteria and regression tests

- [ ] A table-wide test rejects cross-group duplicate spellings and automatically covers future entries.
- [ ] All known aliases resolve consistently across selectors, ID-range assignment, display, and statistics.
- [ ] Tests cover all five listed conflicting spellings, not only `w`.
- [ ] User labels cannot shadow a built-in alias or create another ambiguous alias.
- [ ] Lower/upper-case display behavior is retained or explicitly migrated; one event is not counted in two tag groups.
- [ ] Release notes identify changed aliases and recommend unambiguous alternatives.

## Compatibility and scope boundaries

No target encoding, ID-width, or TCOBS change. Alias reassignment is an observable compatibility change; do not silently reinterpret deployed tags.

## Source references

- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `Tags`, `FindTagName`, `tagVariants`, `colorize`, `AddUserLabels`
- [internal/id/switchIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go) - `newID` and tag normalization

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L05](L05_ulabel_registration_order.md), [L07](L07_reject_unknown_selectors.md), [L11](L11_loglevel_semantics.md), [L16](L16_statistics_semantics.md). Local references are handover identifiers, not existing GitHub issue numbers.
