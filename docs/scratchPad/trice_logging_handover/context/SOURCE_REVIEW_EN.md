# Source review and evidence limits

## Review scope

This handover was prepared on 10 September 2026. Public raw `wip` sources were retrieved and inspected through the web tool. The earlier discussion's implementation claims were not accepted solely from memory; the principal code paths were checked again.

An attempted checkout in the execution container failed because the container could not resolve GitHub. A direct container download also failed. Therefore no complete local checkout, immutable commit snapshot, upstream Go suite, firmware build, or hardware measurement is claimed. A commit listing obtained separately was not a reliable basis for pinning the retrieved moving-branch files; no unrelated hash was assigned to them.

The evidence is **dated source inspection of the retrieved branch files**. A later implementation must record `git rev-parse HEAD` from the actual checkout and reproduce the relevant cases. Links can change because `wip` can move. The package preserves observations and symbol names even if line numbers shift. It does not contain a complete repository snapshot.

## Evidence classes

| Class | Meaning |
|---|---|
| Source-confirmed | The reported condition follows directly from inspected code or a documented mismatch. It is not automatically an executed end-to-end reproduction. |
| Reliability/test requirement | A relevant boundary or shared code path exists; a protective behavior and regression test are requested. Not every potential failure has been demonstrated. |
| Proposed enhancement | A useful capability or workflow is being requested; the current implementation is not called defective simply for lacking it. |
| Deferred proposal | The idea is recorded for later reconsideration and is explicitly not an immediate implementation instruction. |
| Artifact validation | File structure, references, UTF-8, heading levels, and archive contents can be checked locally. Such checks are not Trice tests. |

## Rechecked source facts

| Source and symbol | Observation | Issues |
|---|---|---|
| [emitter.go](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/internal/emitter/emitter.go), `channelArrayFlag.Set` | Splits selector lists, expands known aliases, does not return an unknown-name error. | L05, L07 |
| Same file, `banOrPickFilter` | Nil selections mean no filtering; the mutual-exclusion fatal check is in the per-message path; pick and ban treat untagged input differently. | L07, L08 |
| Same file, `ArrayFlag.Set` | Appends user-label flag values unchanged. | L06 |
| [lineTransformerANSI.go](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/internal/emitter/lineTransformerANSI.go), `Tags` | Cross-group duplicates include w, rx, tx, s, S; categories/units and severity-like tags share one table. | L01, L11 |
| Same file, `FindTagName`, `tagVariants`, `colorize` | First-match lookups coexist with later-match/counting behavior. | L01, L16 |
| Same file, `AddUserLabels` | Registers one name for each stored value; registration has a process-wide once guard. | L05, L06 |
| Same file, `WriteLine` | Filters display fragments, with special handling of a position near the end of the line and comments about partial lines/addNL. | L12, L13 |
| [handler.go](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/internal/args/handler.go), `runLog` | Parses before registering user labels. | L05 |
| Same file, `logLoop` | Wraps the receiver with a binary logger before translation. | L15 |
| Same file, public command paths | `tlog` and `trice log` use the logging handler; insert and bind both evaluate ID-range options. | L03, L05, L08, L27 |
| [init.go](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/internal/args/init.go) | Actual flag spellings, custom-label list promise, arbitrary string logLevel registration, ID options, append semantics for binary filenames. | L06, L07, L11, L15, L18 |
| [switchIDs.go](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/internal/id/switchIDs.go), `PreProcessing` | Allocates from an inclusive minimum through maximum and excludes occupied IDs. | L02, L03 |
| Same file, `EvaluateIDRangeStrings` | Rejects several syntax/overlap conditions but skips a missing-colon rule. | L27 |
| [insertIDs.go](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/internal/id/insertIDs.go), `insertTriceIDsVisit` | Reuses IDs, including the explicitly commented possible out-of-IDSpace case; has a visit hook used by bind. | L03, L04 |
| [trice.c](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/src/trice.c), `TriceNonBlockingDeferredWrite8` | Strict endpoint comparisons; UARTA uses OR and UARTB AND in activation tests; active range branches already reject multi-pack. | L02, L09, L10 |
| [triceDefaultConfig.h](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/src/triceDefaultConfig.h) | Bounds described as smallest/largest; zero defaults; broad TRICE_OFF scope and routing options. | L02, L09, L18-L20 |
| [triceRingBuffer.c](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/src/triceRingBuffer.c) | Deferred single-event output prepares/encodes before calling the common output dispatch; multi-event output uses a different path. | L17, L20 |
| [translator.go](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/internal/translator/translator.go) | The buffer is already a rendered message or error at pick/ban; metadata follows; visualization consumes the permitted event path separately. | L12-L17 |
| [decoder.go](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/internal/decoder/decoder.go) | Optional typed numeric VisRecord/VisValue path; not a general structured schema or an always-enabled capture API. | L17, L24, L26 |
| [receiver.go](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/internal/receiver/receiver.go) | FILE/FILEBUFFER read from the supplied args; used for chapter replay examples. | L15, L18 |
| [id.go](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/internal/id/id.go) | Dictionary format entry retains Type and Strg. | L26 |
| [TriceUserManual.md](https://raw.githubusercontent.com/rokath/trice/refs/heads/wip/docs/TriceUserManual.md) | Current section map, unimplemented target-level drafts, enrichment terminology/reversal assumptions, parser topic, server claims. | L18-L26 |

## Findings deliberately not overstated

The source proves that diagnostics can share an event-filter path; it does not demonstrate loss of every diagnostic. The raw-capture placement is positive existing behavior, not a newly discovered missing feature. Single-pack routing checks already exist. A general source-history audit is not implemented merely because allocation options exist. A new severity map is not yet approved, and custom labels cannot be assigned arbitrary severities in prose and then treated as existing behavior.

No universal cycle/byte/energy saving is asserted. No target typeX0, legacy RPC, special user hook, or every possible custom backend has been exhaustively audited. The chapter's TRICE_OFF claims concern normal Trice macro use and the documented file/project scope, not a guarantee that every independently configured backend activity vanishes.

## Verification performed on the handover

See `ARTIFACT_VALIDATION.md` for the actual local artifact checks. Proposed acceptance-test checkboxes in issues are intentionally unchecked. They must not be interpreted as passing test results.
