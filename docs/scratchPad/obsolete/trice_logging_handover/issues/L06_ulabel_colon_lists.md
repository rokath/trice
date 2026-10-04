# Parse colon-separated user-label lists as documented

**Local reference:** L06  
**Type:** Bug  
**Suggested priority:** P2 - CLI correctness  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed mismatch with CLI help.

## Summary

The documented `-ulabel motor:sensor -ulabel control` form must register three distinct labels, not a single literal `motor:sensor` name.

## Background and evidence

The help for `flagUserLabel` promises repeated flags containing colon-separated lists. `ArrayFlag.Set` appends each value unchanged and `AddUserLabels` creates one tag for each stored value without splitting it. A label containing the separator cannot match the normal prefix extraction as intended.

## Reproduction or review scenario

Parse `-ulabel motor:sensor -ulabel control`, register the labels, and inspect canonical lookups for `motor`, `sensor`, `control`, and the accidental literal `motor:sensor`. Also test `-ulabel motor:`, `-ulabel :motor`, and repeated names.

## Requested change

Implement the documented list syntax in label-specific parsing or registration. Validate each resulting name and define empty-element and duplicate handling. Do not modify a generic flag type used by unrelated features without reviewing those users.

## Acceptance criteria and regression tests

- [ ] Three intended labels are registered from the example.
- [ ] Repeated individual flags and one colon-separated list are equivalent.
- [ ] Empty elements and invalid names have documented, deterministic handling; preferably reject malformed lists.
- [ ] Duplicate same-label input is either idempotent or clearly diagnosed, never added as another ambiguous group.
- [ ] Resolved labels can be used with pick, ban, and applicable ID-range options.
- [ ] CLI help and tests use the exact lower-case switch name `-ulabel`.

## Compatibility and scope boundaries

Keep legitimate user spellings and case behavior. Combine with post-parse registration/resolution but retain a separate regression test for list syntax.

## Source references

- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - `flagUserLabel`
- [internal/emitter/emitter.go](https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go) - `ArrayFlag.Set`
- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `AddUserLabels`

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L01](L01_unique_tag_aliases.md), [L05](L05_ulabel_registration_order.md), [L07](L07_reject_unknown_selectors.md). Local references are handover identifiers, not existing GitHub issue numbers.
