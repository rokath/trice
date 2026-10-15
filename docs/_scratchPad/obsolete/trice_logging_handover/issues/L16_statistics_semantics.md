# Define counting semantics independently of coloring and filtering

**Local reference:** L16  
**Type:** Correctness clarification / regression coverage  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed counting in presentation/tag loops; intended accounting categories need agreement.

## Summary

Statistics should say what they count and must not change merely because metadata, color handling, or filter placement changes.

## Background and evidence

Tag counts are updated in the display transformer, whose input includes fragments and presentation tags. Duplicate aliases can increment multiple groups. Moving selection before formatting can also change counts unless received, decoded, selected, displayed, and diagnosed events are distinguished deliberately.

## Reproduction or review scenario

Use a known sequence of events with mixed tags, hidden events, metadata fragments, multi-line content, and a decoder diagnostic. Compare statistics across color modes, metadata layouts, and filter settings.

## Requested change

Document the meaning of existing counters and select stable update points. Separate received/decoded counts from displayed-event counts only where useful; do not relabel one as another. Avoid counting display fragments as independent application events. Keep transport-loss, application suppression, and decode-error counts conceptually separate.

## Acceptance criteria and regression tests

- [ ] One decoded application event contributes at most once to its canonical tag counter.
- [ ] Changing color or metadata formatting does not change application-event counts.
- [ ] Hidden-event treatment is explicit and covered by tests.
- [ ] Multi-line and partial-line examples follow the documented event/line model.
- [ ] New earlier filtering does not silently change existing statistics.
- [ ] Any renamed or redefined counters include migration notes.

## Compatibility and scope boundaries

No need for new target counters or bytes. Do not claim a missing-event count can identify undetectable losses.

## Source references

- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `Tags.count`, `TagEvents`, `PrintTagStatistics`, `colorize`
- [internal/translator/translator.go](https://github.com/rokath/trice/blob/wip/internal/translator/translator.go) - filter and output sequencing

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L01](L01_unique_tag_aliases.md), [L11](L11_loglevel_semantics.md), [L13](L13_fragment_and_multiline_policy.md), [L17](L17_early_host_filtering.md). Local references are handover identifiers, not existing GitHub issue numbers.
