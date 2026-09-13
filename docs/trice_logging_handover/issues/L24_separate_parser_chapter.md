# Move internal parser development into a separate chapter and track it independently

**Local reference:** L24  
**Type:** Documentation organization / scope control  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** User-directed separation; current section 45.3 is a distinct internal-parser topic.

## Summary

Keep parser modernization separate from terminology, log-level corrections, enrichment, and the possible structured-event extension.

## Background and evidence

The reviewed UM places "Improving the Trice Tool Internal Parser" at 45.3 between enrichment and server use. Future structured consumers need access to event data before rendering, but that interface requirement does not prove a complete parser rewrite is necessary. The current optional `VisRecord`/`VisValue` path is an existing starting point for supported numeric events, not a complete general solution.

## Reproduction or review scenario

Move the section without changing its substantive design content. Check all links, TOC entries, numbering, and references from source/design documents. List the narrow interface dependencies separately.

## Requested change

Create an independent main chapter/work item. State that ID decoding, typed event data, selection, and presentation should have a clear boundary. Assess whether the existing typed-data hook suffices for a small feasibility exercise before expanding the parser project.

## Acceptance criteria and regression tests

- [ ] The parser topic is a separate chapter at the agreed heading level.
- [ ] All previous content and useful anchors are retained or redirected.
- [ ] No parser implementation is implicitly authorized by the documentation move.
- [ ] Structured/event-filter work documents its interface needs without a blanket dependency on a rewrite.
- [ ] The central chapter refers to the parser task only for relevant technical detail.

## Compatibility and scope boundaries

The separate chapter may change numbered locations; references should use stable anchors rather than fixed section numbers.

## Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - Improving the Trice Tool Internal Parser
- [internal/decoder/decoder.go](https://github.com/rokath/trice/blob/wip/internal/decoder/decoder.go) - `VisRecord`, `VisValue`, `VisRecordProvider`

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L17](L17_early_host_filtering.md), [L18](L18_central_manual_chapter.md), [L26](L26_structured_logging_feasibility.md). Local references are handover identifiers, not existing GitHub issue numbers.
