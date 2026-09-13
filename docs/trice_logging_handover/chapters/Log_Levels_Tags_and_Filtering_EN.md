<!-- Integration: this is a complete main UM chapter (H2), with H3/H4 subsections.
Do not assign a fixed chapter number here. Insert into the UM, then regenerate
numbering and the table of contents. This English file and the German file are
alternative language versions, not consecutive chapters. -->

## <a id="log-levels-tags-and-filtering"></a>Log Levels, Tags and Filtering

> **Status:** Review draft based on the `wip` sources retrieved on 10 September 2026. Existing behavior, known limitations, and proposed changes are distinguished below. The referenced source was inspected, but the upstream test suites and target builds were not executed for this chapter. The fixes mentioned here are not assumed to have landed.

### <a id="ll-purpose"></a>Purpose and design principle

Trice's normal logging path identifies a message by an ID and transmits its runtime values rather than a fully rendered message. The host uses the matching dictionary to reconstruct the output. This makes it practical to retain diagnostic detail and select the visible subset later. The same principle can support optional structured output without adding a level or field name to every target event. [S1] [S2]

**Keep the efficient target design. Use existing selection mechanisms first. Add finer target-side controls only when a concrete resource or execution requirement justifies them.** This is the recommendation of this review, not a claim that target filtering is never useful.

A short individual logging call does not guarantee negligible aggregate cost. Evaluate event frequency, argument computation, buffering, framing, transmission, and recording together:

```text
bytes per second = events per second * average transmitted bytes per event
```

TCOBS framing/encoding does not define a log level. Nor does compact encoding guarantee a fixed compression ratio or eliminate the possible benefit of subsequent storage compression. No TCOBS change is needed for the corrections discussed in this chapter.

### <a id="ll-terminology"></a>Terminology

| Term | Meaning in this chapter |
|---|---|
| Event | One occurrence of a Trice logging call. A displayed line may contain several events, and one event may contain several newlines. |
| Tag or channel | A classification prefix such as `err:`, `wrn:`, or `read:`. It can identify a severity, a category, or a presentation convention. |
| Alias | Another accepted spelling for the same tag group. One exact spelling must not identify different groups. |
| Log level / severity | The importance or seriousness of an event, for example debug, information, warning, or error. |
| Tag selection | Choosing an explicit set of categories using `-pick` or excluding a set using `-ban`. |
| Level threshold | Selecting events according to a defined severity order. This is conceptually different from an explicit tag list. |
| Context enrichment | Adding properties such as a task ID, firmware identity, source position, or timestamp without repeating them at every log site. |
| Structured logging | Retaining information as separately accessible data fields instead of only as text embedded in a sentence. |
| ID routing | Selecting an output destination according to an event's numeric ID. |

A category such as read, write, or security is not intrinsically a severity. An `OFF` setting is not the severity of an emitted event. Numeric severity values are conventions that require explicit mappings between systems; there is no universal numbering that Trice must adopt.

A structured record may contain a free-text message as one value. Conversely, automatically adding context to a text string does not by itself expose that context as independently queryable fields. JSON is one possible output representation, not the definition of structured logging. These distinctions are consistent with the separate body, attributes, source, and severity concepts of the OpenTelemetry log model. [S10]

### <a id="ll-selection-points"></a>Where selection happens and what it saves

| Mechanism | Selection point | What can be saved | What remains |
|---|---|---|---|
| `TRICE_OFF` | Compilation, at the configured file/project scope | Normal disabled Trice macro code and its argument evaluation | No later recovery of events that were not generated |
| Configured target ID routing | Output dispatch for a selected destination | Transfer/storage at that destination | Earlier acquisition and, in the inspected deferred path, encoding have already happened |
| `-pick` / `-ban` | Host, currently after message rendering | Subsequent normal presentation/output of excluded events | Target work, link traffic, and previous host decoding/rendering |
| `-logLevel` | Host presentation path in the reviewed implementation | Presentation selected by its current tag-order rules | Target work and acquisition; it is not a target controller |
| Proposed per-level compile-time selection | Not implemented as the draft extension | Would eliminate selected levels from a build | Requires a rebuild to restore them |
| Proposed dynamic target level control | Not implemented as the draft extension | Would potentially avoid acquisition and/or transmission work | Adds a policy, control mechanism, and target-side decisions |

The deferred timing statement applies to the inspected implementation, not to every user-defined direct-output callback. Routing is not automatically acquisition-time suppression. [S2] [S3] [S4] [S12] [S5] [S14]

### <a id="ll-target-off"></a>Compile-time control with TRICE_OFF

For normal Trice macro use in a source file, define `TRICE_OFF` before the first inclusion of `trice.h` in that translation unit:

```c
#define TRICE_OFF 1
#include "trice.h"
```

A project-wide compiler definition such as `-DTRICE_OFF=1` provides broader control. The build configuration and include order must agree. The Trice source tools can still see the log statements and dictionary information; compile-time elimination is not a request to delete historical entries from `til.json`. [S1] [S5]

Logging arguments should not be used for required program side effects. There is also an important wrapper distinction: disabling a Trice macro inside an ordinary function does not necessarily prevent the caller from evaluating that function's arguments. For example, `log_motor(read_sensor())` may still call `read_sensor()`. A future level-specific wrapper design must address that explicitly.

This broad compile-time control already exists. The additional per-level `trice_INFO`/`trice_DEBUG` mechanism described later is a separate, deferred proposal.

### <a id="ll-tags-aliases"></a>Tags, aliases, and user labels

A known tag is normally recognized at the beginning of a message before the first colon. Alias groups let spellings such as `err` and `ERROR` refer to the same category. Case can still affect presentation: known lower-case prefixes are normally removed by the appropriate color modes, while upper-case prefixes may remain visible. The `off` and `none` color options are therefore not identical. [S3] [S6]

**Known defect in the reviewed version:** exact spellings occur in different groups: `w`, `rx`, `tx`, `s`, and `S`. Some code paths return the first match; others continue to later matches. Until resolved, use unambiguous spellings such as `wrn`, `write`, `read`, `receive`, `transmit`, `signal`, and `Seconds`. The correction requires registry-wide tests, including future additions and user-label collisions; it is not simply a cosmetic table cleanup. [S3]

The actual CLI spelling is **`-ulabel`**, not `-uLabel`. It declares additional labels; it does not inherently assign a meaningful severity to them. The documented syntax is:

```text
-ulabel motor:sensor -ulabel control
```

**Known limitations in the reviewed version:** the implementation does not split the colon-separated user-label list as documented, and pick/ban aliases are resolved before the newly declared labels are registered. Thus a custom-label selector in the same command is not reliable merely because `-ulabel` appears first. Use this syntax only with an implementation in which those issues are fixed, or verify the particular behavior before relying on it. [S3] [S4] [S12] [S6]

The intended processing order is: collect options; register and validate all labels; resolve selectors; reject invalid/conflicting options; then open inputs and outputs. That order is a requested correction, not a description of the current code.

### <a id="ll-pick-ban"></a>Explicit tag selection with pick and ban

`-pick` retains the named tag groups. `-ban` removes the named tag groups. Lists use colons and the switches can be repeated. Known aliases expand to their group. [S4] [S12]

```text
-pick wrn:err
-ban dbg
```

`-pick wrn:err` means those groups explicitly. It does **not** mean a severity threshold such as "warning and everything more severe"; other severe groups are not implicitly included.

In the reviewed filter implementation, an untagged event is suppressed by a nonempty pick selection and passes a ban selection. A nonmatching prefix likewise fails pick and passes ban. Neither switch removes target-side costs or transport bytes. [S4] [S12]

Do not combine `-pick` with `-ban`: this is unsupported. The current error is detected too late, in the per-message filter; it should instead be a startup validation error.

**Known validation defect:** unknown names are silently discarded. An entirely unknown pick selection can consequently become no filter at all. The required behavior is a command error naming the invalid token, after valid custom labels have been registered. A known selector for which no events arrive is valid; absence of matching data is not a spelling error. [S4] [S12]

### <a id="ll-host-level"></a>Host-side logLevel

The existing host option is spelled with an upper-case `L`:

```text
-logLevel all
-logLevel wrn
-logLevel off
```

`all` is the default. `off` suppresses normal output in the relevant presentation path; it does not disable target acquisition or stop a requested binary recording. Tool diagnostics and other consumers need an explicit policy rather than an assumption that this switch shuts down everything. [S3] [S6]

**Current limitation:** the implementation derives a threshold from the position of entries in the `Tags` display table. That table includes categories, unit tags, and appended user labels as well as conventional severity names. Level filtering also touches presentation fragments such as timestamps, locations, prefixes, and suffixes. It is not yet an independent, consistently applied event-severity model. [S3] [S6]

The preferred correction is to retain a useful threshold interface but define severity separately from presentation. The behavior of categories without severity, user labels, untagged events, and the interaction with explicit tag selection must be approved and documented. Adding a color entry must not change severity. No target level field or severity-ordered ID layout is required for a host-only correction.

Do not silently remove `-logLevel`. Removing it also breaks compatibility. If maintaining it is ultimately rejected, use an explicit deprecation and migration process. Unknown values should become startup errors, not accidental table-index behavior.

### <a id="ll-raw-recording"></a>Binary recording and later re-selection

`-binaryLogfile` and its alias `-blf` record the received stream before translation and display filtering. `-logfile`/`-lf` record text output and are not equivalent. An explicit binary filename is appended to if it already exists. [S6] [S7]

A replay example, assuming `session.bin` and its matching `til.json` already exist in the working directory, is:

```bash
tlog -p FILEBUFFER -args session.bin -idlist til.json -pick wrn:err -blf off
```

To replay the same received data without that tag selection:

```bash
tlog -p FILEBUFFER -args session.bin -idlist til.json -logLevel all -blf off
```

`tlog` uses the logging command path; `trice log` accepts the corresponding logging options. Use the matching encoding, framing, target settings, and optional location file when they differ from defaults. A replay does not reconstruct an event that was never received. [S7] [S8]

Archive the relevant dictionary and firmware/build identity with a recording. Where source positions matter, retain the appropriate location metadata too. Do not append replay output into its own input. A testable invariant is that identical received input produces identical binary captures regardless of host display selection.

### <a id="ll-id-allocation"></a>ID allocation policy and dictionary history

The existing insert and bind workflows expose `-IDMin`, `-IDMax`, `-IDMethod` (`random`, `upward`, or `downward`), and repeated tag-specific `-IDRange` rules. The common and specialized ranges must not overlap. These options allocate IDs; they do not configure an output device by themselves. [S6] [S9] [S13]

For example, the following is a policy illustration for an application source directory named `app`; it writes the normal bind artifacts and must be adapted to the actual project:

```bash
trice bind -src ./app -IDMin 1000 -IDMax 9999 -IDRange err:10,99
```

The error range is 10 through 99 inclusive. All other eligible tags use the common range. Select a range within the actual supported ID space and archive the configuration with the build.

**Existing IDs are not automatically migrated.** The reuse logic may retain an ID outside a newly configured range. That is valuable for stability, but it makes an allocation rule different from a verified statement that all active errors currently occupy that range. `til.json` also contains historical entries; it must not be renumbered indiscriminately. [S9] [S13]

The proposed safeguard is an audit of active assignments against the current policy, with useful warnings and optionally strict CI checking. Historical-only entries need separate treatment, and a partial source scan cannot prove complete compliance. An optional repair should preview changes, update source or sidecars coherently, preserve old mappings, and require a rebuild. These audit/migration functions are proposals, not documented existing commands.

**Additional validation defect:** the reviewed `EvaluateIDRangeStrings` silently skips a rule with no colon. Malformed rules should fail before modifying source or generated artifacts. [S9] [S13]

### <a id="ll-target-routing"></a>Target ID routing and packing modes

A configured output can use an ID interval to forward only selected messages. Other outputs may still receive the same event. For example, a system may send a broad stream to one destination and only an error range to another. This is output routing, not necessarily suppression of the event's initial acquisition. [S2] [S5] [S6]

The intended interval contract is inclusive. **Known defect:** the inspected deferred comparisons use strict `<` at both ends, despite inclusive allocation and the documented smallest/largest limits. Do not assume boundary events are routed correctly until this is fixed and tested. A workaround that widens limits must be reviewed again after an inclusive fix. [S2] [S5] [S9] [S13]

There is a second configuration defect: UARTA and UARTB use different tests for whether a range is enabled (`||` versus `&&` for nonzero bounds). Auxiliary/RTT configuration uses defined-macro tests. Missing/default/zero bounds require an explicit, consistent policy; changing one operator without defining that policy is insufficient. [S2]

For the current active deferred per-ID routing branches, use:

```c
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
```

**The corresponding compile-time `#error` checks already exist.** Protect them with positive and negative build tests for each relevant output and partial-bound case. Do not add an unconditional error that rejects all multi-pack use. A valid multi-pack configuration without ID routing is a different case, and direct/custom routes require their own contract. [S2]

The limitation is not intrinsic to TCOBS. A future implementation could decide on individual events before packing, but that would be a separate change requiring a demonstrated benefit.

### <a id="ll-integrity-and-performance"></a>Diagnostics, event boundaries, statistics, and host performance

An allowed event and its chosen metadata should share one acceptance decision. A timestamp's styling tag must not decide whether the timestamp of an accepted warning disappears. At the same time, existing partial-line composition must not be changed accidentally: several calls may form one line, and one call may span lines. Tests need to cover EOF flushing, `-addNL`, hidden fragments, and metadata attribution. [S3] [S4] [S12]

The translator currently sends both formatted events and some error messages through the same tag filter. Unknown-ID and decode/integrity diagnostics therefore require a deliberate separate policy. They must not vanish merely because the application selection excludes their display tag. A future machine-readable output must keep diagnostics separate or encode them under a defined schema. [S4] [S12]

Statistics need explicit counting semantics: received data, decoded events, selected events, displayed events, and diagnostic occurrences are not the same thing. Coloring or splitting a message into display fragments must not invent extra application events.

Earlier host filtering is a useful optimization candidate because the present pick/ban stage follows message rendering. A decoder/dictionary-based decision could avoid unnecessary formatting. It must still maintain framing, cycle/integrity state, required decoding checks, raw capture, and approved timestamp-delta/statistics/visualization behavior. Measure CPU, allocations, and the unfiltered baseline before adding complexity. [S4] [S12] [S11]

### <a id="ll-deferred-extensions"></a>Deferred target-level extensions

#### <a id="ll-deferred-compile"></a>Per-level compile-time selection

The future-development draft proposes names such as `trice_INFO`, `trice_DEBUG`, and a corresponding build-time transformation. Such selection could remove selected event code and argument evaluation from a production build. It also adds macro families, tooling rules, and another build policy. [S1]

**Decision of this review: defer implementation until an immediate, documented requirement exists.** Reopen it for a measured code-size/execution constraint or a required build policy that broad `TRICE_OFF` and existing controls do not address adequately. A later design must support the intended bind/insert workflows and eliminate argument evaluation when disabled.

The draft's proposed `trice insert -loglevel` is **not** the implemented host `-logLevel` switch. Capitalization and command context matter. Do not present the proposal as an available command.

#### <a id="ll-deferred-runtime"></a>Dynamic target-side level control

A dynamic threshold could become useful for excessive event rates, limited transmission bandwidth, bounded offline storage, or costly event arguments. It would need a defined update mechanism, concurrency policy, and a precise decision point. A local application control can be sufficient; remote operation may require an existing or new command path. [S1]

**Decision of this review: also defer this extension until a concrete requirement justifies it.** Low per-event overhead is a strong reason to prefer the current design, but not a proof that all workloads are cheap. Existing compile-configured routing is not the same as a dynamic early filter.

Neither target-level proposal is a prerequisite for context enrichment or structured host-side fields. No additional per-event level bytes are presumed necessary.

### <a id="ll-related-work"></a>Relationship to enrichment, structured logging, and server use

The reviewed UM section map is: 45.1 level-control draft; 45.2 the draft primarily about enrichment; 45.3 internal-parser development; and 45.4 server use. Prefer stable anchors when these sections move. The enrichment chapter should be renamed and the parser task should become a separate chapter/work item. [S1]

A possible `strice` extension can retain one event ID for multiple named fields. The host can resolve a field across multiple IDs and argument positions, optionally translate it to a host-side numeric field ID, and produce text or structured output. Restricting the target to one key per message is not required and can break convenient context grouping. This remains a proposal, not an implemented public interface.

For server use, distinguish a server consuming device Trice streams from replacing a native server logger. The former fits the efficient target design naturally; the latter needs separate evidence and benchmarks. Do not assume that tokenization proves lower total energy use or eliminates further compression. First validate a practical workflow, then prepare a detailed extension specification.

### <a id="ll-summary"></a>Recommended order of work

Preserve the target core and current wire format. Correct alias ambiguity, CLI validation, custom-label handling, routing endpoints, and routing activation first. Protect existing packing guards and raw capture with tests. Clarify host severity, metadata, line composition, and diagnostic/statistics behavior before moving filtering earlier. Integrate this overview with the technical reference sections. Reconsider finer target selection only when a real requirement appears.

### <a id="ll-references"></a>Implementation and terminology references

Source links are to the reviewed branch, not an immutable commit. Recheck them and record the actual commit when implementing changes.

[S1]: https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md
[S2]: https://github.com/rokath/trice/blob/wip/src/trice.c
[S3]: https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go
[S4]: https://github.com/rokath/trice/blob/wip/internal/translator/translator.go
[S5]: https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h
[S6]: https://github.com/rokath/trice/blob/wip/internal/args/init.go
[S7]: https://github.com/rokath/trice/blob/wip/internal/args/handler.go
[S8]: https://github.com/rokath/trice/blob/wip/internal/receiver/receiver.go
[S9]: https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go
[S10]: https://opentelemetry.io/docs/specs/otel/logs/data-model/
[S11]: https://github.com/rokath/trice/blob/wip/internal/decoder/decoder.go
[S12]: https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go
[S13]: https://github.com/rokath/trice/blob/wip/internal/id/insertIDs.go
[S14]: https://github.com/rokath/trice/blob/wip/src/triceRingBuffer.c
