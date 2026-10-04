# Separate device-log ingestion from native server logging and qualify performance claims

**Local reference:** L25  
**Type:** Documentation / feasibility evidence  
**Suggested priority:** P3 - before expanding server scope  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Future-development claims are hypotheses, not measured conclusions in this handover.

## Summary

Clarify which server use case Trice should solve and avoid developing a competing logging ecosystem without evidence of a useful advantage.

## Background and evidence

A server that decodes device Trice streams can benefit from named fields while leaving the target format intact. Replacing a server application's existing logger is a much broader proposition, involving dynamic fields/strings, concurrency, error data, ecosystem adapters, and operational behavior. Compact tokenization alone does not establish superior end-to-end throughput, energy consumption, or a lack of benefit from further compression.

## Reproduction or review scenario

Compare a device-log gateway scenario with a native-server producer scenario. For performance claims, compare equivalent retained information, enabled/disabled paths, compression settings, storage, and end-to-end decoding/query cost against relevant alternatives.

## Requested change

Rewrite "Using Trice on Servers" to distinguish the cases. Prioritize reuse of standard host outputs/consumers and require measured adoption/use-case evidence for a native server backend. Treat host key-ID interning as an optional implementation detail, not a new mandatory protocol. Do not require globally unique target event IDs when source/dictionary identity provides disambiguation.

## Acceptance criteria and regression tests

- [ ] Device ingestion and native server logging are separate use cases with separate requirements.
- [ ] Unsupported speed, energy, size, and no-further-compression claims are removed or labeled hypotheses.
- [ ] Comparisons preserve equivalent information and state benchmark conditions.
- [ ] The design retains multi-field events and their source/event-instance association.
- [ ] No new analysis platform, global key registry, or target format is mandated without a concrete requirement.
- [ ] At least one practical consumer/workflow is identified before proposing a server-specific implementation.

## Compatibility and scope boundaries

This is a scope/evidence task, not a demand to replace Logrus, slog, Serilog, or other server loggers.

## Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - Using Trice on Servers
- [Go records, attributes, and handlers](https://pkg.go.dev/log/slog)
- [Logrus structured fields and formatters](https://github.com/sirupsen/logrus)

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L18](L18_central_manual_chapter.md), [L21](L21_clarify_logging_terminology.md), [L26](L26_structured_logging_feasibility.md). Local references are handover identifiers, not existing GitHub issue numbers.
