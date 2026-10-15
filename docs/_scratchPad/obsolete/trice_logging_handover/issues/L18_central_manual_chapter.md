# Add a central manual chapter for log levels, tags, filtering, and ID routing

**Local reference:** L18  
**Type:** Documentation  
**Suggested priority:** P2 - after behavior is checked  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Proposed chapter; English and German drafts are supplied in the handover.

## Summary

Provide one reliable entry point that explains existing selection mechanisms, their costs, limitations, and the two deferred target-level proposals.

## Background and evidence

Relevant information is distributed over CLI help, target configuration, ID management, and UM future-development sections. The reviewed section map is 45.1 level-control draft, 45.2 the misnamed enrichment draft, 45.3 parser work, and 45.4 server use. Section numbers can change and should not be hard-coded into the new chapter title.

## Reproduction or review scenario

Have a reader answer from the chapter alone: how to show only chosen tags; how to keep a complete raw capture; what changes target bandwidth; whether changing ID allocation moves existing IDs; and whether `trice_INFO` or a dynamic target level controller is implemented.

## Requested change

Integrate the supplied chapter at the UM main-chapter level (`##`), with `###` subsections. Regenerate numbering/TOC using the project workflow. Explain terminology, all relevant flags, `TRICE_OFF`, direct/deferred routing boundaries, inclusive ID policy, known defects, and deferred compile-time/runtime selection. Link technical detail rather than duplicating conflicting reference manuals. Mark proposals as proposals until implemented.

## Acceptance criteria and regression tests

- [ ] Both language drafts carry the same technical content and current-versus-proposed distinctions.
- [ ] Implemented host `-logLevel` is not confused with the unimplemented draft `trice insert -loglevel` switch.
- [ ] Costs saved at compile time, on a target route, and on the host are distinguished.
- [ ] Custom-label limitations, ambiguous aliases, ID-history caveats, and existing compile guards are included.
- [ ] The two target extensions are described as deferred for lack of a demonstrated need, not universally useless.
- [ ] All internal links, heading levels, numbering, and command examples are validated before merging.
- [ ] Temporary known-issue notes are updated only after the corresponding changes and tests land.

## Compatibility and scope boundaries

This task does not authorize changing runtime behavior to match prose. The package chapters describe the reviewed behavior and clearly label the desired corrections.

## Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - current levels/tags documentation and future-development sections
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - actual CLI spellings and help
- [src/triceDefaultConfig.h](https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h) - target configuration

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L11](L11_loglevel_semantics.md), [L19](L19_deferred_compile_time_levels.md), [L20](L20_deferred_runtime_levels.md), [L21](L21_clarify_logging_terminology.md), [L24](L24_separate_parser_chapter.md), [L25](L25_server_use_evidence.md). Local references are handover identifiers, not existing GitHub issue numbers.
