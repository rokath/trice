# Keep an accepted event and its display metadata under one filter decision

**Local reference:** L12  
**Type:** Bug / output consistency  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed fragment-wise level filtering; intended event-level behavior must be tested.

## Summary

An accepted warning should not lose its timestamp, source location, or ID merely because those display fragments use different presentation tags.

## Background and evidence

Pick/ban filtering is already applied before location/timestamp/ID strings are inserted. Level filtering happens later inside the ANSI line transformer for individual strings, including presentation fragments. CLI help explicitly warns that `-logLevel` also influences these metadata formats, prefix, and suffix. This mixes event selection with decoration.

## Reproduction or review scenario

Render an accepted warning with location, target timestamp, host timestamp, displayed ID, prefix, and suffix enabled. Give the metadata ordinary display tags and compare `-logLevel all` with a warning threshold. Repeat for `color=off`, `none`, and default.

## Requested change

Base acceptance on the event's classification, not on the tags chosen to style its metadata. Once accepted, format that event and its configured context as a coherent unit. Keep explicit metadata-hiding options effective. Specify prefix/suffix behavior in conjunction with the separate fragment/line policy.

## Acceptance criteria and regression tests

- [ ] Allowed events retain all explicitly enabled metadata regardless of its styling tag.
- [ ] Suppressed events do not leave orphan timestamps, source locations, IDs, prefixes, or blank decorated lines.
- [ ] Changing color mode does not change event selection.
- [ ] Pick/ban and logLevel can use the same semantic decision without duplicating decoration-specific rules.
- [ ] Tests cover local and remote display paths where supported.
- [ ] No unreviewed change to multi-event line grouping is bundled into this fix.

## Compatibility and scope boundaries

Some users may depend on old decoration filtering. Document the change and retain explicit options to hide each metadata column.

## Source references

- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `colorize`, `WriteLine`
- [internal/translator/translator.go](https://github.com/rokath/trice/blob/wip/internal/translator/translator.go) - filtering before metadata emission
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - logLevel warning about metadata

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L11](L11_loglevel_semantics.md), [L13](L13_fragment_and_multiline_policy.md), [L14](L14_diagnostic_filter_isolation.md). Local references are handover identifiers, not existing GitHub issue numbers.
