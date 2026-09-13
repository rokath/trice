# Remove unchanged-configuration heuristics from the future enrichment transformation contract

**Local reference:** L22  
**Type:** Draft architecture risk  
**Suggested priority:** Deferred design work before implementing enrichment  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** The UM explicitly documents dependence on unchanged stf/stv settings and manual cache invalidation.

## Summary

A future enrichment implementation must not depend on guessing how to remove inserted context from user source after configuration changes.

## Background and evidence

The draft proposes heuristic recognition/removal of inserted context, requires `stf`/`stv` to remain unchanged between insert/clean, and tells users to clean before configuration changes and invalidate a cache. Arbitrary formatting transformations are not generally invertible without retained provenance. An interrupted build can also leave modified source visible.

## Reproduction or review scenario

Review configuration changes between insert and clean; aborted builds; user edits to an instrumented call; stale caches; and multiple projects sharing a source file. A template that omits or changes original content is a useful non-invertibility example.

## Requested change

Before implementing the extension, define a deterministic source-of-truth and provenance model. Prefer compatibility with generated bind artifacts or a provably reversible transform rather than a format-string heuristic. Keep field/type/position metadata coherent with generated argument additions. Separate collection configuration from host rendering configuration.

## Acceptance criteria and regression tests

- [ ] A changed enrichment configuration cannot silently corrupt source or remove original arguments.
- [ ] The recovery contract for interrupted builds and user edits is defined.
- [ ] Caches are keyed/invalidation-safe without requiring an undocumented manual deletion ritual.
- [ ] Shared-source and concurrent build/write cases have an explicit policy.
- [ ] Original template/arguments and structured field information remain recoverable where rewriting is used.
- [ ] No promise states that users always see original source while a pre/post-build rewrite is in progress.

## Compatibility and scope boundaries

This is a pre-implementation requirement, not a claim that an existing enrichment implementation is defective. Do not turn it into an unrelated parser rewrite.

## Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - enrichment draft stf/stv heuristic, clean and cache discussion
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - commented draft stf/stv option registrations

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L21](L21_clarify_logging_terminology.md), [L23](L23_validate_draft_examples.md), [L24](L24_separate_parser_chapter.md), [L26](L26_structured_logging_feasibility.md). Local references are handover identifiers, not existing GitHub issue numbers.
