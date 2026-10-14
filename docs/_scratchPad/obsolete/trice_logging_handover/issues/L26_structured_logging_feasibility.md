# Validate a useful structured-event workflow before specifying a strice extension

**Local reference:** L26  
**Type:** Exploratory feasibility - not an implementation specification  
**Suggested priority:** P3 - evidence first  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Conceptual proposal from the review; strice syntax and schema are not yet approved APIs.

## Summary

Determine whether optional named, typed fields provide measurable practical value while retaining Trice's efficient target encoding.

## Background and evidence

A proposed `strice("Motor {motor_id}: temperature {temperature_c} C", motor, temperature)` can associate one event ID with multiple argument fields. The host can resolve a key across many IDs and argument positions without parsing rendered text. One key per target message is unnecessary and would lose convenient event grouping. Optional numeric key IDs belong on the host if a consumer benefits from them.

The promising workflows are automated firmware assertions, measurement analysis, and field diagnosis. A formatter alone is not sufficient evidence of demand.

## Reproduction or review scenario

Choose one real workflow: for example, assert `temperature_c < limit` across two event IDs and firmware builds while freely changing display wording. Compare the effort and reliability with the current Trice/visualization workflow. Retain a free message as an optional rendering, not the only copy of the values.

## Requested change

Use the smallest reversible prototype or design demonstration needed to assess usefulness. Record field types/widths, names/units, dictionary identity, and event grouping. Show how optional enrichment extends the same event without collisions. Check new names against a reviewed dictionary/schema before accepting them; a dictionary that automatically accepts every typo is not validation. Defer a full syntax/schema/CLI specification until value and constraints are clear.

## Acceptance criteria and regression tests

- [ ] The demonstration supports a field query across multiple event IDs without rendering and reparsing messages.
- [ ] It shows identical target payload values/widths to an equivalent ordinary Trice event; any wrapper/code/runtime overhead is measured separately.
- [ ] Additional dynamic context is not advertised as free.
- [ ] Name collisions, duplicate placeholders, type/argument mismatch, and dictionary/version mismatch have stated handling.
- [ ] A real workflow owner or concrete maintenance benefit is identified; lack of benefit can justify stopping.
- [ ] The output leaves a decision record, not an unapproved full extension specification.

## Compatibility and scope boundaries

Preserve normal Trice behavior. No mandatory JSON on target, one-key restriction, new per-event key IDs, or new target-level selection is required. Wrapper call-site provenance and exactly-once/disabled argument evaluation remain design questions.

## Source references

- [internal/decoder/decoder.go](https://github.com/rokath/trice/blob/wip/internal/decoder/decoder.go) - existing optional typed numeric records
- [internal/id/id.go](https://github.com/rokath/trice/blob/wip/internal/id/id.go) - current dictionary entry structure
- [named message-template fields](https://messagetemplates.org/)
- [tokenized static metadata](https://pigweed.dev/pw_log_tokenized/)

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L17](L17_early_host_filtering.md), [L19](L19_deferred_compile_time_levels.md), [L20](L20_deferred_runtime_levels.md), [L21](L21_clarify_logging_terminology.md), [L22](L22_reversible_enrichment_instrumentation.md), [L24](L24_separate_parser_chapter.md), [L25](L25_server_use_evidence.md). Local references are handover identifiers, not existing GitHub issue numbers.
