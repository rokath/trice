# Protect unfiltered binary capture and replay with integration tests

**Local reference:** L15  
**Type:** Regression protection / documentation  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Existing placement of the binary logger is source-confirmed; byte-exact regression tests are requested.

## Summary

Keep the ability to hide events now and recover them later from a raw recording, independent of host display filters.

## Background and evidence

`logLoop` wraps the receiver in `NewBinaryLogger` before calling the translator. This is distinct from `-logfile`/`-lf`, which records output text. The architectural advantage depends on not moving binary capture behind selection or reusing a filtered record stream as the supposed raw capture.

## Reproduction or review scenario

Feed identical recorded input into separate runs with all output, restrictive pick, ban, and level settings. Use fresh capture destinations, because explicit binary filenames are append targets. Compare captured bytes and replay with the matching dictionary and format configuration.

## Requested change

Add an integration invariant: display selection does not alter the received bytes recorded by `-blf`/`-binaryLogfile`. Document append behavior, the difference from a text log, and the need to archive matching dictionary/build/encoding information. Preserve visibility of capture write failures.

## Acceptance criteria and regression tests

- [ ] For identical received input, binary captures are byte-identical under all supported host filters.
- [ ] Replay can reveal an event that was hidden during the original display.
- [ ] The test does not append replay output back into its own input file.
- [ ] Text logging is documented as output capture, not a substitute for raw replay.
- [ ] Missing/unmatched dictionaries and truncated recordings produce diagnostics rather than invented values.
- [ ] The guarantee is limited to bytes actually received; target-side suppression, source loss, and filtered-out target routes cannot be recovered.

## Compatibility and scope boundaries

This protects an existing architectural property. It does not request storing secrets or extra metadata in every target event.

## Source references

- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - `NewBinaryLogger` before `translator.Translate`
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - binary and text logfile flags
- [internal/receiver/receiver.go](https://github.com/rokath/trice/blob/wip/internal/receiver/receiver.go) - file-based input

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L14](L14_diagnostic_filter_isolation.md), [L17](L17_early_host_filtering.md), [L18](L18_central_manual_chapter.md). Local references are handover identifiers, not existing GitHub issue numbers.
