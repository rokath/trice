# Rename the enrichment draft and separate levels, enrichment, and structured logging

**Local reference:** L21  
**Type:** Documentation / conceptual correction  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed terminology mismatch in the UM draft.

## Summary

The draft currently called "Trice Structured Logging" primarily describes automatic context enrichment. Correct the definition without discarding its useful embedded-logging ideas.

## Background and evidence

In the reviewed UM, 45.1 is the level-control draft and 45.2 is the context-oriented draft. The latter defines structured logging by automatically adding compile-time/runtime data. These are different dimensions: severity classifies an event, enrichment adds properties, and structured logging retains independently accessible fields. JSON is an output format, not the definition. A free-text message may coexist with structured context.

## Reproduction or review scenario

Compare the opening definitions with the examples: adding task ID, source position, and uptime to a printf-style message is enrichment; keeping those as independently accessible typed fields also yields structured data. Neither behavior requires the other in all cases.

## Requested change

Prefer the title "Automatic Context Enrichment" for that draft. Keep one small example showing enriched text versus an enriched structured event. Preserve links or redirect old anchors while updating references from 45.1. Separate implementation status from possibilities. Keep compile-time metadata in the dictionary and preserve the cost distinction for added dynamic values.

## Acceptance criteria and regression tests

- [ ] Definitions no longer equate automatic context addition with structured logging.
- [ ] Static context, source/session context, and per-event dynamic context are distinguished.
- [ ] A message may be free text without making the entire record unstructured.
- [ ] Existing examples are not presented as an implemented CLI or JSON guarantee.
- [ ] The terminology is consistent with the central chapter and both languages.
- [ ] Renaming does not lose prior design decisions, open questions, or old anchor compatibility.

## Compatibility and scope boundaries

Documentation correction now; a detailed `strice` extension specification remains a later task.

## Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - 45.1/45.2 headings and opening definition
- [Serilog enrichment](https://github.com/serilog/serilog/wiki/Enrichment)
- [OpenTelemetry log data model](https://opentelemetry.io/docs/specs/otel/logs/data-model/)

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L18](L18_central_manual_chapter.md), [L22](L22_reversible_enrichment_instrumentation.md), [L23](L23_validate_draft_examples.md), [L26](L26_structured_logging_feasibility.md). Local references are handover identifiers, not existing GitHub issue numbers.
