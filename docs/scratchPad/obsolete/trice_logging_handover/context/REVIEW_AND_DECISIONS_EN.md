# Trice logging review: context, decisions, and open questions

**Date:** 10 September 2026.  
**Continuation branch requested by the maintainer:** `wip`. The maintainer reported that the branches had been merged and aligned. This handover does not independently certify equality of their commit hashes.  
**Purpose:** Preserve the technical substance and decision rationale so work can continue without the original conversation. This is not a verbatim transcript or a finished extension specification.

## 1. Executive decision

Retain Trice's efficient target-side ID-plus-values design. Correct the existing CLI/tag/routing problems before adding another target-level API. Preserve the ability to record received binary data and choose what to display later.

Additional per-level compile-time selection and a dynamic target-side level controller are deferred, not rejected as inherently useless. Reopen them for a measured resource limit, deterministic-execution requirement, or concrete build policy.

Keep `-logLevel` as a useful host-interface concept unless a separate compatibility decision retires it. Its present coupling to a display table and fragments needs clarification/correction. Do not confuse that option with the unimplemented draft `trice insert -loglevel`.

An optional structured-event extension could add significant value for automated firmware tests, measurement processing, and field diagnosis without changing target payloads for equivalent values. First demonstrate a real workflow; do not commit to a new server-logging ecosystem or a detailed API specification yet.

These are review recommendations and recorded planning decisions, not claims that patches have been merged. The issues distinguish observed defects from proposals and protective tests.

## 2. How the terminology was clarified

The original draft equated automatic addition of compile-time/runtime information with structured logging. That equivalence was the main conceptual error, not evidence that the enrichment idea itself was useless.

**Log level** classifies an event's seriousness or importance. **Tag/category** classifies its subject or purpose and need not imply severity. **Context enrichment** adds relevant properties without requiring them to be repeated at each call site. **Structured logging** preserves independently accessible fields. **Rendering** decides how humans see them. **Encoding/framing** transports them. **Indexing** supports efficient searching over stored data. These are separate dimensions.

For example, this is already structured in its outer fields:

```json
{"level":"warning","task_id":7,"message":"Motor 3 has temperature 87 C"}
```

However, the motor number and temperature remain embedded in text. For temperature queries, this record is more useful:

```json
{"level":"warning","task_id":7,"motor_id":3,"temperature_c":87,"message":"Motor 3 has temperature 87 C"}
```

A logger need not extract every possible fact into its own field. The useful question is which values later consumers must query without understanding message wording. A free-text body is explicitly compatible with the OpenTelemetry log data model. JSON is optional; binary records plus a known schema can also be structured. [OpenTelemetry](https://opentelemetry.io/docs/specs/otel/logs/data-model/)

"Automatic Context Enrichment" is the preferred name for the context-focused draft. It is more precise than "Enhancement" for the intended operation. Serilog uses enrichment for adding event properties. [Serilog enrichment](https://github.com/serilog/serilog/wiki/Enrichment)

## 3. The proposed minimal structured interface

The initial illustration repeated values in both a formatted message and explicit key/value arguments. That duplication is not an inherent requirement and was discarded as an unattractive interface.

The preferred discussion example is an unimplemented C-like proposal:

```c
strice("Motor {motor_id}: temperature {temperature_c} C",
       motor, temperature);
```

Each runtime value is supplied once. The template names the field and supplies default display wording. The build/host side could retain names, types, widths, argument positions, and rendering metadata while the target keeps transmitting an ID and numeric values. Named message templates are an established pattern rather than a Trice-specific invention. [Message Templates](https://messagetemplates.org/)

A project may centralize event definitions in typed wrapper functions:

```c
void log_motor_temperature(int motor, int temperature)
{
    strice("Motor {motor_id}: temperature {temperature_c} C",
           motor, temperature);
}
```

This is illustrative, not compilable against an approved current `strice` API. The wrapper prevents repeated spelling of keys at many call sites, but does not detect a typo in its own definition. Similar generated typed-event methods exist in .NET. [.NET logging source generation](https://learn.microsoft.com/en-us/dotnet/core/extensions/logging/source-generation)

Wrappers have additional consequences: a non-inlined call may add runtime overhead; the logged source position is normally the wrapper's location rather than the caller's; and disabling logging inside the function need not suppress evaluation of the function-call arguments. Requirements for actual call-site attribution, exactly-once evaluation, and complete removal when disabled must be explicit. A shared ID alone cannot distinguish ten callers of the same wrapper.

## 4. What til.json can and cannot validate

A project dictionary can serve as a reviewed vocabulary and schema reference. Validate new field names before accepting them into that reference. A name such as `temperture_c` can produce a warning/error with `temperature_c` as a suggestion; automatic correction is not advised.

Merely collecting all names and later checking against that enlarged set is not typo detection: it can legitimize a typo on first use. A strict mode needs an explicit approval process for legitimate new names. A centrally consistent misspelling still cannot be recognized as semantically wrong without an external convention or human review.

The future contract also needs types, widths, units, and argument positions. A `%d` or `{temperature_c}` token alone is not a complete domain schema. Retain raw numeric values independently of display precision or units attached to a string. The same field name must not silently change meaning between sites or firmware versions.

The reviewed `TriceFmt` definition has `Type` and `Strg`; this is a starting point, not proof that a complete structured-field schema is implemented. [Current ID data type](https://github.com/rokath/trice/blob/wip/internal/id/id.go)

## 5. One event ID can legitimately represent multiple keys

A single ID can select a template with several named fields:

```text
ID 4711: arg0 = motor_id, arg1 = temperature_c, arg2 = ctx.task_id
ID 5822: arg0 = temperature_c, arg1 = fan_rpm
```

On loading the dictionary, a host can build a lookup for `temperature_c`: ID 4711/argument 1 and ID 5822/argument 0. It can then evaluate a temperature query without generating or reparsing either message. No per-field identifier has to be transmitted on the target for that operation.

Distinguish four identities: the local decoding/template ID; the field key; an optional stable domain-event name; and one concrete event occurrence. Repeated transmission of one ID produces many event instances. The ID is not itself a globally unique occurrence identifier.

A host may intern keys to numeric IDs, build indexes, or transform records for a consumer. This is optional and does not imply a new target protocol. Field IDs do not themselves replace storage indexing. Source/dictionary identity can disambiguate independently assigned target IDs, while exhaustion within a local dictionary remains a separate capacity issue.

Restricting each target event to one key was considered and is not recommended. Splitting related values creates extra headers and a correlation problem under concurrent producers. Context enrichment naturally creates multi-field events anyway. A downstream transformation may split data only while preserving source and event-instance association. Windows event templates are another established example of one event definition containing multiple named data items. [Windows event definitions](https://learn.microsoft.com/en-us/windows/win32/wes/defining-events)

## 6. Enrichment and structured events fit together

Use one enrichment model for ordinary and prospective structured events where practical. Static fields can stay in the dictionary; source/session identity can be attached by the receiver; additional dynamic context must actually be acquired and associated with the event.

For identical values, widths, and existing encoding, retaining field names on the host adds no target payload bytes. That statement does not imply zero cost for wrapper calls, additional values, wider encodings, timestamp capture, task lookup, or a sensor read. Pigweed provides an embedded analogue for carrying static metadata through tokenized strings. [Pigweed tokenized logging](https://pigweed.dev/pw_log_tokenized/)

The conceptual operation is:

```text
original typed fields + configured context fields = one enriched event
```

Do not first collapse the original fields into a rendered `message` and then pretend the data remain accessible. Reserve or otherwise define a context namespace (for example `ctx.*`) to prevent collisions. An automatic ambient-temperature value must not overwrite a motor-temperature field. The namespace example is not a settled syntax.

Collection configuration belongs to the build/target contract. Display configuration belongs to the host. A host may reorder, hide, or repeat existing values, but cannot reconstruct a task ID that was never collected. A changed collection layout must retain a correct association with the firmware and dictionary.

The old stf/stv heuristic reversal and cache-invalidation assumptions are not a sound basis for a new implementation. Prefer generated artifacts or retained provenance compatible with bind. Support deterministic recovery from interrupted builds/configuration changes. See L22 rather than treating enrichment as permission to rewrite user sources unreliably.

## 7. Findings about existing level functionality

The implemented feature set is already substantial: broad `TRICE_OFF`, host pick/ban, host logLevel, ID allocation policies, and configured target ID routing. This is not a logger without level functionality.

The review identified concrete inconsistencies: cross-group aliases; exclusive routing endpoints despite inclusive allocation; inconsistent partial-bound activation; user-label list and initialization-order bugs; silently ignored selectors; late pick/ban conflict detection; and, during handover verification, a missing-colon `-IDRange` rule that is skipped instead of rejected.

Several other items are deliberately not labeled as proven missing features: single-pack `#error` checks already exist; raw capture is already before translation; source ID reuse outside a changed policy is an intentional stability behavior but needs auditing support. Desired tests must preserve these strengths, not replace them unnecessarily.

Host logLevel currently uses table positions and display fragments. Prefer a defined severity mapping and a clear treatment of categories without severity, rather than deleting the switch or imposing new target work. Preserve compatibility consciously. Do not assume every alias/category is a severity or that level names such as Fatal must trigger process termination.

Earlier host selection can save rendering effort, but requires agreed event/line boundaries, diagnostics, statistics, visualization, and timestamp-delta semantics. Decode/framing state and raw capture cannot simply be skipped along with presentation. Performance is not yet measured.

Detailed evidence is recorded in the individual issues and source review. The supplied chapters distinguish current limitations from future corrections.

## 8. ID history, routing, and packing constraints

An allocation range is not a migration. Existing active IDs can remain outside a newly specified range. Audit the current source/bind result; do not mistake every retained dictionary/location entry for an active call site.

A fix must be explicit: preview changes, allocate unused IDs, update source or sidecars coherently, preserve old mappings, protect shared state against competing writes, and rebuild. Do not edit only `til.json` or delete history to make a range checker quiet.

Current active deferred per-ID routing requires the relevant single-pack mode and already contains compile-time errors for incompatible multi-pack settings. Normalize activation logic and test guard reachability. This is not a universal TCOBS restriction, and it is not a blanket statement about custom direct routes. A future pre-pack decision could change the implementation boundary without changing the encoding itself.

Filtering on the host does not save target bandwidth. Routing a destination does not necessarily save initial acquisition or encoding work. Raw recordings can recover hidden host output, but not data lost or suppressed before receipt.

## 9. Reference systems and lessons retained

| System | Relevant observation | Implication for Trice |
|---|---|---|
| [Logrus](https://github.com/sirupsen/logrus) | Multiple fields per event, reusable entry context, formatters, and severity methods; the project is in maintenance mode. | Multi-field events are normal. Study the concepts, not a requirement to copy every API or fatal/panic side effect. |
| [Go slog](https://pkg.go.dev/log/slog) | Records, attributes, levels, reusable context, and handler backends. | Separate event data from output and consider integration rather than a new host ecosystem. |
| [Message Templates / Serilog](https://messagetemplates.org/) | Named placeholders can retain values separately from rendering. | The proposed compact `strice` call shape is conceptually established. |
| [.NET source generation](https://learn.microsoft.com/en-us/dotnet/core/extensions/logging/source-generation) | Typed generated logging methods check template/parameter relationships. | Central event definitions can reduce spelling and type mistakes without duplicating values. |
| [Zap](https://github.com/uber-go/zap) and [Zerolog](https://github.com/rs/zerolog) | Structured fields and efficient output paths; Zerolog also offers CBOR. | Efficiency and structured logging are compatible. Compare Trice to efficient alternatives, not only printf text. |
| [Pigweed](https://pigweed.dev/pw_log_tokenized/) | Tokenized logging and static metadata carried through the dictionary/template. | No need to transmit static key names per event. |
| [Zephyr](https://docs.zephyrproject.org/latest/services/logging/index.html) | Separate compile-time/runtime filtering and optional dictionary logging. | These are independent features; their existence elsewhere does not prove an immediate Trice requirement. |
| [Windows event definitions](https://learn.microsoft.com/en-us/windows/win32/wes/defining-events) | An event definition can refer to a template with several data items. | One event ID for several fields is not intrinsically unsuitable for server analysis. |
| [OpenTelemetry](https://opentelemetry.io/docs/specs/otel/logs/data-model/) | Separates body, severity, source, and attributes. | A useful host export need not invent universal numeric key IDs. |
| [CLP at Uber](https://www.uber.com/au/en/blog/reducing-logging-cost-by-two-orders-of-magnitude-using-clp/) | A deployed approach separates repetitive log structure and values for compact storage/analysis. | There is demand for efficient logging; this does not establish Trice's superiority or prove further compression useless. |

These sources validate useful patterns, not adoption of the proposed Trice extension. Absence of feedback on an abstract draft is not enough evidence either for or against demand. Test actual workflows.

## 10. Server scope and practical value

Prioritize a server or gateway consuming device Trice streams and exporting usable fields to existing tools. A native server producer replacing an established logging library is a separate, broader project involving arbitrary strings, changing context sets, concurrency, errors, and integration.

The strongest near-term candidates are firmware-test assertions independent of English wording; measurement filtering/charting without reparsing text; and field diagnosis using stable field meaning across builds. Existing visualization features must be considered so a new API proves additional value rather than duplicating an adequate workflow.

Do not build a global key-ID registry, a new analysis platform, or a server-logger replacement merely to claim structured logging support. Do not widen the target ID format solely because fields need names. Benchmark CPU, bytes, storage, and end-to-end query costs at equivalent information content before making server performance claims.

## 11. Open extension questions retained for later specification

A later specification should resolve placeholder grammar and literal braces; argument order/count; duplicate names; types/widths and formatting hints; mixed data/string support and payload limits; canonical keys/units; missing values; safe JSON output; integer precision, NaN/infinity, and raw-versus-rendered values; dictionary and firmware identity; versioned event meaning; source/session metadata; context collisions and override rules; provider cost/ISR safety; event atomicity; wrapper provenance; disabled argument evaluation; and consumer/error-channel behavior.

Initially limiting support to known numeric messages is legitimate. Hashing a dynamic string can be useful for identification but does not retain its content. Static source locations should usually use suitable dictionary/location metadata rather than automatically increasing every event's runtime payload. A monotonic uptime is not automatically wall-clock time.

No final placeholder syntax, dictionary schema, new switch names, or automatic-migration command is approved by this handover. The examples explain the intended direction. Full specification work remains deferred until understanding and usefulness are established.

## 12. Manual organization and continuation

The web-retrieved UM map was 45.1 levels, 45.2 enrichment under the old title, 45.3 parser, 45.4 servers. Add the new central main chapter without hard-coding its eventual number. Rename the enrichment section with anchor compatibility, and move parser work to an independent chapter. Keep source-of-truth detail sections linked rather than maintaining contradictory descriptions.

The package contains 27 issue drafts, two language versions of the chapter, an issue index, coverage mapping, a source review, and a test plan. L01-L17 cover present behavior and safeguards; L18 is central documentation; L19-L20 record deferred target proposals; L21-L26 preserve the broader design context; L27 is an additional confirmed validation finding. These references are local labels, not GitHub issue numbers.

For continuation: record the actual `wip` commit, reproduce relevant static findings, fix and test the selected correctness issues, then update the known-issue notes. Do not infer that any fix was applied from the existence of these documents.
