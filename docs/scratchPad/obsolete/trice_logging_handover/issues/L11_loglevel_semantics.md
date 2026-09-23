# Decouple logLevel semantics from tag-table position and color formatting

**Local reference:** L11  
**Type:** Behavioral correction / compatibility design  
**Suggested priority:** P2 - define policy before implementation  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed coupling; the replacement severity contract is a proposal.

## Summary

Keep the useful host threshold interface, but stop treating an incidental display-table order as the definition of event severity.

## Background and evidence

`lineTransformerANSI.colorize` derives both event level and threshold from indices in `Tags`. The same table also contains categories, measurement units, and custom labels appended at runtime. Thus reordering display entries or adding labels can affect filtering. There is no shared explicit severity metadata. Unknown tagged text defaults differently from untagged text.

The decision from the review is to prefer retaining `-logLevel`, not silently deleting it. Lack of usage evidence is not evidence that nobody depends on it.

## Reproduction or review scenario

Record legacy results for all built-in tags, aliases, untagged text, unknown prefixes, and user labels under `all`, `off`, and representative thresholds. Then reorder only display metadata in a test: event severity must not change in the corrected model.

## Requested change

Specify severity separately from category and presentation. Define which tags have severity, what happens to tags without severity, and how `-ulabel` participates. A safe candidate is to preserve unclassified events unless explicitly selected out, but approve the policy before coding. Define the interaction of an explicit tag filter and a severity threshold. Keep numeric mappings internal or translate them explicitly at export boundaries; do not infer one universal severity numbering.

## Acceptance criteria and regression tests

- [ ] An explicit mapping or equivalent stable contract replaces table-position semantics.
- [ ] `all`, `off`, no-severity categories, user labels, and untagged events have documented behavior.
- [ ] Adding/reordering a color entry cannot change severity.
- [ ] Event tags do not themselves call exit, panic, or a target reset; classification and control flow remain separate.
- [ ] Legacy-versus-new behavior is covered by a comparison matrix and migration/release notes.
- [ ] Any deprecation/removal requires a separate explicit compatibility decision, not an incidental refactor.

## Compatibility and scope boundaries

Host-only semantic work. Do not add level bytes, force severity-ordered IDs, or introduce level-specific target macros. Preserve valid legacy commands or provide a deliberate transition.

## Source references

- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `Tags`, `colorize`, `Colorize`, `WriteLine`
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - `-logLevel` and `-ulabel` help

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L01](L01_unique_tag_aliases.md), [L07](L07_reject_unknown_selectors.md), [L12](L12_event_metadata_filtering.md), [L13](L13_fragment_and_multiline_policy.md), [L16](L16_statistics_semantics.md), [L18](L18_central_manual_chapter.md). Local references are handover identifiers, not existing GitHub issue numbers.
