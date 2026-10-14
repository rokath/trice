# Provide an explicit, non-destructive migration for active ID-range violations

**Local reference:** L04  
**Type:** Optional enhancement  
**Suggested priority:** P2 - after validation; implement only if migration is needed  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Proposed follow-up; not a claim that stable ID reuse is itself a bug.

## Summary

Offer a safe repair path for intentionally moving current log sites into new ID ranges without invalidating older firmware logs.

## Background and evidence

An allocation policy checker can identify active out-of-range sites, but changing numbers in `til.json` alone would desynchronize firmware, source or sidecars, and the dictionary. Historical mappings must remain available for recordings and older firmware. Different projects may share a dictionary while using independent build directories.

## Reproduction or review scenario

Use a project with one active violation, one compliant site, a historical dictionary-only entry, and a second project sharing the dictionary. Simulate a failed write and an exhausted destination range during a trial migration.

## Requested change

Provide an explicit preview of old/new assignments and affected artifacts. Migrate only selected active sites; allocate unused IDs under the chosen policy; update source or bind sidecars as applicable; preserve old mappings. Protect shared state against concurrent writers. Make failure recovery and rebuild requirements clear. Keep the operation opt-in and separate from ordinary validation.

## Acceptance criteria and regression tests

- [ ] A preview is available without file changes and lists every affected assignment/artifact.
- [ ] No existing historical mapping is repurposed for a different event.
- [ ] Source/sidecar assignments and generated metadata stay coherent after migration.
- [ ] Unrelated projects and historical-only entries are not silently renumbered.
- [ ] Exhaustion, conflicts, write failures, and concurrent updates fail safely without a falsely successful mixed state.
- [ ] Rerunning a completed migration is idempotent; old and new firmware fixtures remain decodable with their appropriate metadata.

## Compatibility and scope boundaries

This is not permission to rewrite an entire dictionary or require migration for host-only filtering. Choose the actual CLI after design review.

## Source references

- [internal/id/insertIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/insertIDs.go) - assignment reuse and source writes
- [internal/id/switchIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go) - allocation state and post-processing
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - insert/bind entry points

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L03](L03_validate_active_id_policy.md), [L27](L27_reject_malformed_idrange.md). Local references are handover identifiers, not existing GitHub issue numbers.
