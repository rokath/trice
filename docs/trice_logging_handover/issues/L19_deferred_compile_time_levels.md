# Record the deferred proposal for level-specific compile-time selection

**Local reference:** L19  
**Type:** Deferred design proposal - not a bug  
**Suggested priority:** Deferred - reopen only for a concrete requirement  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** UM proposal, not a missing mandatory feature.

## Summary

Keep fine-grained compile-time selection such as `trice_INFO` on record without implementing it merely to imitate other loggers.

## Background and evidence

The UM proposes transforming a normal Trice call into level-specific macros and compiling selected levels out. Existing `TRICE_OFF` already provides broad file/project control. Per-level compile-time suppression could remove code, argument evaluation, and transmission for selected events, but introduces macro, parser, build-policy, and compatibility surface.

## Reproduction or review scenario

A reopening request should provide a firmware/code-size or deterministic-execution requirement that cannot be met adequately by existing controls. Include representative code and measured baseline cost, rather than only a preference for familiar logger syntax.

## Requested change

Record the decision to defer. If reopened, compare approaches compatible with both bind and insert. Preserve the existing default path and guarantee that disabled logging arguments are not evaluated. Do not require reversible source rewriting merely because it appears in an older draft.

## Acceptance criteria and regression tests

- [ ] The UM explicitly identifies `trice_INFO` and the proposed insert `-loglevel` as unimplemented draft concepts.
- [ ] The rationale for deferral and measurable reopening criteria are recorded.
- [ ] Existing `TRICE_OFF` capabilities and scope are described accurately.
- [ ] A later implementation proposal includes code-size, disabled-argument, compiler, and build-mode tests.
- [ ] Structured logging and context enrichment are not made dependent on this extension.

## Compatibility and scope boundaries

No implementation is requested now. Do not describe the proposal as inherently useless or confuse it with host `-logLevel`.

## Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - Compile-time Log-level Control draft
- [src/triceDefaultConfig.h](https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h) - `TRICE_OFF`
- [src/trice.h](https://github.com/rokath/trice/blob/wip/src/trice.h) - disabled/enabled public macro paths

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L18](L18_central_manual_chapter.md), [L20](L20_deferred_runtime_levels.md), [L21](L21_clarify_logging_terminology.md). Local references are handover identifiers, not existing GitHub issue numbers.
