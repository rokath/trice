# Evaluate host selection before expensive message rendering where safe

**Local reference:** L17  
**Type:** Optimization  
**Suggested priority:** P3 - after semantic fixes and measurement  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed late rendering/filter sequence; performance gain is not measured.

## Summary

Avoid rendering messages that will be discarded by host filters, without changing the wire format or compromising decoding state.

## Background and evidence

`dec.Read` currently returns a formatted message (or a diagnostic), and the translator then applies pick/ban. For ID-based messages, tag/level information can in principle be resolved from the dictionary once rather than extracted from each rendered string. The current optional visualization path already exposes some typed numeric data separately, but it is not a complete general event API.

## Reproduction or review scenario

Benchmark replay of a representative capture at 0%, 50%, 90%, and 100% suppression. Include numeric messages, supported dynamic-string forms, dictionary changes, diagnostics, visualization, and multi-pack input. Compare event/output results as well as CPU and allocations.

## Requested change

Cache selection metadata per dictionary entry if appropriate. Skip only work that is provably unnecessary: framing, length validation, integrity checks, protocol state, and required raw capture still run. Preserve the current or explicitly approved policies for visualization, display deltas, diagnostics, fragments, and statistics. Keep non-ID encodings such as CHAR/DUMP on appropriate paths.

## Acceptance criteria and regression tests

- [ ] Measured CPU/allocation benefit and benchmark inputs are reported; no unsubstantiated speed factor is claimed.
- [ ] Accepted output remains equivalent under the approved semantics.
- [ ] Cycle/timestamp/framing state remains correct across hidden events.
- [ ] Display-delta behavior remains consistent with its documented reference to previously displayed timestamps.
- [ ] Dictionary reload/replacement invalidates cached selection metadata correctly.
- [ ] Raw capture, diagnostics, and supported visualization behavior are preserved.
- [ ] The unfiltered path does not suffer an unexplained material regression.

## Compatibility and scope boundaries

No early target suppression and no TCOBS redesign. A large decoder rewrite is not automatically authorized; a narrow event boundary may be sufficient.

## Source references

- [internal/translator/translator.go](https://github.com/rokath/trice/blob/wip/internal/translator/translator.go) - `dec.Read`, pick/ban, visualization, timestamp state
- [internal/decoder/decoder.go](https://github.com/rokath/trice/blob/wip/internal/decoder/decoder.go) - `VisRecord`, `VisValue`, `VisRecordProvider`
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - dictionary and raw capture setup

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L11](L11_loglevel_semantics.md), [L12](L12_event_metadata_filtering.md), [L13](L13_fragment_and_multiline_policy.md), [L14](L14_diagnostic_filter_isolation.md), [L15](L15_raw_capture_replay_guarantee.md), [L16](L16_statistics_semantics.md), [L24](L24_separate_parser_chapter.md). Local references are handover identifiers, not existing GitHub issue numbers.
