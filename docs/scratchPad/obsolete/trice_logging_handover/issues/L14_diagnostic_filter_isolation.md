# Prevent event filters from silently hiding decoder and tool diagnostics

**Local reference:** L14  
**Type:** Reliability hardening  
**Suggested priority:** P1 - diagnosis must remain possible  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed shared input path; individual lost-diagnostic cases still require reproduction.

## Summary

Application event selection must not accidentally conceal unknown-ID, malformed-frame, or transport-integrity diagnostics.

## Background and evidence

The translator explicitly states that the decoded buffer can contain either one formatted Trice message or one error message, then passes that buffer through `BanOrPickFilter`. Filtering diagnostics as though they were ordinary application events can hide the reason output is missing. This is a risk established by the shared path, not a claim that every diagnostic is currently suppressed.

## Reproduction or review scenario

Combine valid events with unknown IDs, malformed/truncated frames, and supported integrity/cycle diagnostics. Apply restrictive pick/ban/level settings. Check both console and recording/output channels and count which diagnostics remain visible.

## Requested change

Distinguish decoder/tool diagnostics from application events before applying application selection. Establish a dedicated diagnostic policy or channel; explicitly requested quiet behavior must be separate from accidental suppression. Keep banners, progress output, ANSI formatting, and diagnostics from contaminating any future machine-readable data stream.

## Acceptance criteria and regression tests

- [ ] Diagnostic fixtures remain observable under restrictive application filters unless an explicit diagnostic policy suppresses them.
- [ ] A malformed frame cannot be misrepresented as a normal filtered event or a zero-valued decoded event.
- [ ] Fatal/startup errors retain nonzero status regardless of display selection.
- [ ] No mixed plain-text warning is inserted into an output that promises valid machine-readable records.
- [ ] The separation does not change the bytes of the raw received-data capture.
- [ ] Existing diagnostic destinations and any migration are documented.

## Compatibility and scope boundaries

No new wire protocol. Structured-output requirements are safeguards for future output, not a claim that a general JSON exporter already exists.

## Source references

- [internal/translator/translator.go](https://github.com/rokath/trice/blob/wip/internal/translator/translator.go) - buffer comment and `BanOrPickFilter` call
- [internal/emitter/emitter.go](https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go) - tag-based selection
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - raw capture wrapper

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L07](L07_reject_unknown_selectors.md), [L12](L12_event_metadata_filtering.md), [L15](L15_raw_capture_replay_guarantee.md), [L17](L17_early_host_filtering.md). Local references are handover identifiers, not existing GitHub issue numbers.
