# Validate active ID assignments against changed ID-range policies

**Local reference:** L03  
**Type:** Enhancement / correctness safeguard  
**Suggested priority:** P1 when target routing depends on ranges; otherwise P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed ID reuse behavior; validation is proposed, not existing functionality.

## Summary

Changing `-IDRange`, `-IDMin`, or `-IDMax` must not give a false impression that all existing log sites now follow the new allocation policy.

## Background and evidence

The insertion path preserves matching IDs and explicitly permits reusing an ID that may no longer belong to the current ID space. This protects stable decoding but means a new error-only range does not automatically contain every active error event. The shared insertion/allocator path also supplies assignments for bind.

`til.json` is historical as well as current. Scanning it alone cannot establish which entries are active. `li.json` may retain old locations too; active-use classification must consider the source set/current bind result.

## Reproduction or review scenario

Create an error log site with ID 250 and a matching dictionary entry. Introduce an error range 10..99 and a disjoint common range. Run insert and bind in separate fixtures. Include another old out-of-policy error entry that is not used in the current source set.

## Requested change

Report active policy violations with ID, tag/canonical group, location, expected range, and actual assignment. Distinguish active, historical-only, and unknown-activity entries. Warn without mutation by default; consider an explicit strict CI mode. Flag incomplete source coverage instead of claiming a complete audit. A repair, if offered, belongs to the separate migration workflow.

## Acceptance criteria and regression tests

- [ ] Existing active out-of-range assignments produce actionable diagnostics in both insert and bind workflows.
- [ ] Historical-only entries are not reported as mandatory renumbering errors.
- [ ] Aliases and user labels use the validated registry; default and tag-specific ranges are checked.
- [ ] Validation does not mutate source, sidecars, `til.json`, or `li.json`.
- [ ] Strict checking, if added, returns a failing status before a misleading successful build result.
- [ ] Incomplete source coverage and range exhaustion are distinguished from policy compliance.

## Compatibility and scope boundaries

Preserve dictionary history and stable IDs unless explicitly migrating. New CLI names are intentionally not specified in this issue.

## Source references

- [internal/id/insertIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/insertIDs.go) - `insertTriceIDsVisit`, existing-ID reuse and bind visit hook
- [internal/id/switchIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go) - `PreProcessing`, `EvaluateIDRangeStrings`
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - insert and bind configuration flow

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L01](L01_unique_tag_aliases.md), [L02](L02_inclusive_routing_bounds.md), [L04](L04_safe_id_policy_migration.md), [L27](L27_reject_malformed_idrange.md). Local references are handover identifiers, not existing GitHub issue numbers.
