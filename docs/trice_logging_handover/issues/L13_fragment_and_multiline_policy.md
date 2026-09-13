# Define and test filtering for partial lines and multi-line events

**Local reference:** L13  
**Type:** Compatibility clarification / regression coverage  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Existing source documents line-dependent behavior and unfinished `-addNL` handling; no complete runtime matrix was executed.

## Summary

Filtering must remain predictable when multiple Trice calls form one displayed line or one event contains multiple newlines.

## Background and evidence

`WriteLine` reasons about fragments and treats the last relevant fragment specially. Its comment discusses a `msg:Hello` fragment followed by an `att:World\n` fragment, and notes further tuning for `-addNL`. An event and a displayed line are therefore not interchangeable in the existing console path.

## Reproduction or review scenario

Create fixtures for: two differently tagged partial events that form one line; a single multi-line event; an event without final newline at EOF; a suppressed fragment between two accepted fragments; and a newline-only fragment. Run them with and without `-addNL`, metadata, and thresholds.

## Requested change

Document the legacy behavior before choosing a corrected event/line policy. Preserve accepted partial-line workflows where practical. If event-based filtering changes composition, specify the transition and metadata attribution explicitly. A future structured output should represent one Trice call as one event even when text contains newlines; do not retroactively treat every physical line as an event.

## Acceptance criteria and regression tests

- [ ] A golden-test matrix covers all listed fragment and multi-line cases.
- [ ] No orphan metadata, accidental concatenation across unrelated events, or unbounded buffered fragment is introduced.
- [ ] EOF flushing and `-addNL` are deterministic.
- [ ] The origin used for the displayed timestamp/location/ID of a composed line is documented.
- [ ] Any change from legacy selection is deliberate and included in release notes.
- [ ] Structured-event boundaries remain independent of console newline layout in future integrations.

## Compatibility and scope boundaries

Do not claim that a console line equals one event in all existing Trice usage. This is a semantic prerequisite for moving filtering earlier.

## Source references

- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `WriteLine` comments and last-fragment test
- [internal/translator/translator.go](https://github.com/rokath/trice/blob/wip/internal/translator/translator.go) - line state and EOF flush

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L11](L11_loglevel_semantics.md), [L12](L12_event_metadata_filtering.md), [L17](L17_early_host_filtering.md). Local references are handover identifiers, not existing GitHub issue numbers.
