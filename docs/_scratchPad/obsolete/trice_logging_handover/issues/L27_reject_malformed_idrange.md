# Reject malformed IDRange arguments instead of silently skipping them

**Local reference:** L27  
**Type:** Bug / validation - additional finding during handover review  
**Suggested priority:** P1 when allocation policy drives routing  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed missing-colon branch in `EvaluateIDRangeStrings`.

## Summary

A malformed `-IDRange` argument must not be silently omitted while the command continues with an unintended allocation policy.

## Background and evidence

`EvaluateIDRangeStrings` splits each rule at `:`. If the separator is absent, the function currently continues to the next item rather than returning an error. Other malformed forms reach a syntax error. This makes one malformed syntax class unexpectedly permissive and can undermine a target route relying on the intended range.

## Reproduction or review scenario

Pass a range value without a colon, for example `-IDRange err10,99`, alone and alongside another valid rule. Repeat through insert and bind. Confirm that validation fails before source or sidecar modification.

## Requested change

Return an actionable syntax error for every malformed rule, including missing separators and empty components. Reuse consistent tag validation and preserve existing overlap/duplicate-range checks. Review valid numeric bounds for the supported ID representation without assuming that generic Go integer parsing proves they are legal Trice IDs.

## Acceptance criteria and regression tests

- [ ] Missing `:`, missing comma, empty tag/bounds, non-numeric bounds, and reversed ranges fail consistently.
- [ ] One valid rule does not conceal another malformed rule.
- [ ] Unknown tags and duplicate alias-equivalent rules produce useful diagnostics.
- [ ] Validation fails before any artifact is modified.
- [ ] Insert and bind share the same rule contract.
- [ ] Legal adjacent non-overlapping ranges and inclusive endpoints remain accepted.

## Compatibility and scope boundaries

This is a newly rechecked finding, not a retroactive claim that it was already in the earlier issue list. It does not request automatic renumbering.

## Source references

- [internal/id/switchIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go) - `EvaluateIDRangeStrings`, missing-colon `continue`
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - range validation before insert/bind

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L01](L01_unique_tag_aliases.md), [L03](L03_validate_active_id_policy.md), [L04](L04_safe_id_policy_migration.md). Local references are handover identifiers, not existing GitHub issue numbers.
