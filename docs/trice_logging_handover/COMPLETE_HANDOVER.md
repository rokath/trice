# Trice logging: complete handover

**Date:** 10 September 2026. **Continuation branch:** `wip`.

This searchable archive contains the independent issue drafts, both manual chapters, the design rationale, coverage map, source-review limits, and test plan. It is not a transcript, an implemented patch, or a final structured-logging specification. Code findings are source-inspected; no upstream Go/target test execution is claimed.

For direct editing or issue creation, prefer the standalone files. Original source links and file paths are retained so the material does not depend on access to the original chat. Repeated IDs/Markdown reference labels in the two language chapters are namespaced in this anthology.

---

**Part 1: Package guide (German)**  
**Standalone source:** [README_DE.md](README_DE.md)

## Trice: Issues, UM-Kapitel und Entscheidungsarchiv

**Stand:** 10. September 2026. **Weiterarbeit:** `wip`.

Dieses Paket enthält die sachlichen Ergebnisse der Diskussion als eigenständige Dateien. Es wurden keine GitHub-Issues angelegt und keine Repository-Dateien verändert. Die lokalen Kennungen L01 bis L27 sind keine GitHub-Issue-Nummern.

### Einstieg

| Datei | Inhalt |
|---|---|
| [Issue-Verzeichnis](ISSUE_INDEX.md) | Alle 27 englischen Issue-Dateien, einzeln verlinkt und nach Aufgabenart gruppiert |
| [UM-Kapitel Deutsch](chapters/Log_Level_Tags_und_Filterung_DE.md) | Vollständiger deutscher Kapitelentwurf |
| [UM-Kapitel Englisch](chapters/Log_Levels_Tags_and_Filtering_EN.md) | Inhaltlich entsprechende englische Fassung |
| [Entscheidungen und Kontext](context/REVIEW_AND_DECISIONS_EN.md) | Begriffsklärung, Begründungen, verworfene Alternativen, andere Logger und offene Fragen |
| [Abdeckungsmatrix](context/COVERAGE_MAP_EN.md) | Zuordnung der nummerierten Fragen und der früheren Befunde zu Dateien/Issues |
| [Quellenprüfung](context/SOURCE_REVIEW_EN.md) | Konkrete Codebeobachtungen, Quellen und Grenzen der Verifikation |
| [Testplan](context/TEST_PLAN_EN.md) | Zusammenhängende Regressionstests und empfohlene Arbeitsreihenfolge |
| [Alle Issues in einer Datei](ALL_ISSUES_EN.md) | Lesefassung und zusätzliche Kopiermöglichkeit; die Einzeldateien bleiben maßgeblich |
| [Alles in einer Markdown-Datei](COMPLETE_HANDOVER.md) | Archivfassung der Kapitel, Entscheidungen, Prüfhinweise und Issues |
| [Dateiprüfung](context/ARTIFACT_VALIDATION.md) | Ergebnis der lokal ausgeführten Prüfungen des Pakets |

### Was festgehalten wurde

Den effizienten Target-Kern und den bestehenden Datenstrom beibehalten. Zuerst konkrete Fehler und Inkonsistenzen der vorhandenen Funktionen korrigieren. Zusätzliche Compilezeit-Selektion nach Level und dynamische Target-Level-Steuerung sind zurückgestellt, nicht grundsätzlich verworfen. Der Hostschalter `-logLevel` soll nicht beiläufig verschwinden; seine Semantik braucht eine bewusste Kompatibilitätsentscheidung.

Die Issues trennen Fehler, Absicherung vorhandener Eigenschaften, Optimierung, Dokumentation und zurückgestellte Entwürfe. Insbesondere wird nicht behauptet, die bereits vorhandenen Single-Pack-`#error`-Prüfungen müssten erst neu erfunden werden. Rohaufzeichnung vor der Filterung ist ebenfalls bereits eine Stärke, die erhalten bleiben soll.

Die breitere Diskussion zu Kontextanreicherung und `strice` ist im Entscheidungsarchiv und L21-L26 gesichert. Eine ID für mehrere Felder bleibt sinnvoll; optionale Key-IDs können auf dem Host liegen. Eine detaillierte Erweiterungsspezifikation wird damit noch nicht vorweggenommen.

### Issues verwenden

Die Dateien in `issues/` sind unabhängig kopierbar. Die erste Überschrift ist der vorgeschlagene Issue-Titel. Jede Datei enthält Zusammenfassung, Befund, Reproduktions-/Prüfszenario, gewünschte Änderung, offene Akzeptanzkriterien, Kompatibilitätsgrenzen und Quellen. Vorgeschlagene Prioritäten sind keine verbindliche Releaseplanung.

Nach dem Anlegen echter GitHub-Issues die lokalen Querverweise durch deren Nummern oder URLs ersetzen. Bei einer Zusammenlegung von Aufgaben die individuellen Akzeptanzkriterien erhalten. L19/L20 ausdrücklich als zurückgestellt behandeln; sie sind keine Aufforderung, jetzt Target-Overhead einzuführen.

### Kapitel ins UM einbauen

Beide Fassungen beginnen auf Ebene `##`, weil die bestehenden UM-Hauptkapitel diese Ebene verwenden. Unterabschnitte verwenden `###`, zwei Unter-Unterabschnitte `####`. Es gibt absichtlich keine feste Kapitelnummer. Englisch und Deutsch sind alternative Fassungen mit denselben expliziten Ankern; nicht beide unverändert in dieselbe Datei einfügen.

Ein geeigneter Ort ist im funktionalen Teil nahe „Switching Trice ON and OFF“, nicht ausschließlich unter „Future Development“. Die genaue Position bleibt eine redaktionelle Entscheidung. Nach dem Einfügen Inhaltsverzeichnis und Nummerierung mit dem Projektverfahren neu erzeugen. Referenzlabels wie `S1` bei Bedarf präfixieren, falls das UM bereits gleichnamige Markdown-Linkdefinitionen enthält.

Der Kapitelentwurf beschreibt den geprüften Stand und kennzeichnet bekannte Fehler. Diese Hinweise erst entfernen, wenn die jeweilige Korrektur und ihre Tests tatsächlich vorliegen. Vorgeschlagene neue Validierungs-/Migrationsfunktionen werden nicht als vorhandene CLI-Schalter ausgegeben.

Die bei der Prüfung sichtbare Zuordnung war 45.1 Level-Control-Draft, 45.2 Enrichment unter falschem Titel, 45.3 Parseraufgabe und 45.4 Servernutzung. Das zentrale neue Kapitel ergänzt die Detailabschnitte. Die Parseraufgabe soll getrennt werden; die Enrichment-Umbenennung muss bestehende Verweise berücksichtigen.

### Verifikation und Archivierung

Die wichtigsten Codebefunde wurden an über das Web abgerufenen `wip`-Quellen nachgeprüft. Ein vollständiger Checkout und die Go-/Target-Tests waren hier nicht ausführbar; es wird kein unveränderlicher Commit-Snapshot behauptet. Vor der Umsetzung den tatsächlichen Commit festhalten und die jeweiligen Fälle reproduzieren. Details stehen in der Quellenprüfung.

Die Markdown-Dateien sind UTF-8. Relative Links funktionieren nach dem Entpacken bei unveränderter Ordnerstruktur. Für die unabhängige Archivierung das gesamte ZIP lokal sichern; `COMPLETE_HANDOVER.md` bietet zusätzlich eine einzelne durchsuchbare Lesefassung. `SHA256SUMS.txt` dokumentiert die Prüfsummen der Paketdateien.

---

**Part 2: Issue index**  
**Standalone source:** [ISSUE_INDEX.md](ISSUE_INDEX.md)

## Issue index

All issue files are in English and can be copied independently. Use the first heading as the GitHub title and the remainder as the body. L01-L27 are local handover references, not already-created GitHub issues. Priorities and labels are suggestions.

### Current behavior, safety, and host optimization

| Ref | Standalone issue | Classification |
|---|---|---|
| L01 | [Make tag aliases unambiguous and validate the complete tag registry](issues/L01_unique_tag_aliases.md) | Bug / regression coverage |
| L02 | [Use inclusive ID-routing bounds consistently with ID allocation](issues/L02_inclusive_routing_bounds.md) | Bug |
| L03 | [Validate active ID assignments against changed ID-range policies](issues/L03_validate_active_id_policy.md) | Enhancement / correctness safeguard |
| L04 | [Provide an explicit, non-destructive migration for active ID-range violations](issues/L04_safe_id_policy_migration.md) | Optional enhancement |
| L05 | [Register all user labels before resolving pick and ban selectors](issues/L05_ulabel_registration_order.md) | Bug |
| L06 | [Parse colon-separated user-label lists as documented](issues/L06_ulabel_colon_lists.md) | Bug |
| L07 | [Reject unknown or malformed filter selectors before starting logging](issues/L07_reject_unknown_selectors.md) | Bug / validation |
| L08 | [Reject simultaneous pick and ban options during startup validation](issues/L08_reject_conflicting_filters_early.md) | Bug / error handling |
| L09 | [Make ID-routing activation and partial-bound handling consistent across outputs](issues/L09_consistent_routing_configuration.md) | Bug / configuration validation |
| L10 | [Regression-test the existing single-pack requirement for deferred ID routing](issues/L10_single_pack_guard_tests.md) | Test hardening / documentation |
| L11 | [Decouple logLevel semantics from tag-table position and color formatting](issues/L11_loglevel_semantics.md) | Behavioral correction / compatibility design |
| L12 | [Keep an accepted event and its display metadata under one filter decision](issues/L12_event_metadata_filtering.md) | Bug / output consistency |
| L13 | [Define and test filtering for partial lines and multi-line events](issues/L13_fragment_and_multiline_policy.md) | Compatibility clarification / regression coverage |
| L14 | [Prevent event filters from silently hiding decoder and tool diagnostics](issues/L14_diagnostic_filter_isolation.md) | Reliability hardening |
| L15 | [Protect unfiltered binary capture and replay with integration tests](issues/L15_raw_capture_replay_guarantee.md) | Regression protection / documentation |
| L16 | [Define counting semantics independently of coloring and filtering](issues/L16_statistics_semantics.md) | Correctness clarification / regression coverage |
| L17 | [Evaluate host selection before expensive message rendering where safe](issues/L17_early_host_filtering.md) | Optimization |

### Central documentation and deferred target proposals

| Ref | Standalone issue | Classification |
|---|---|---|
| L18 | [Add a central manual chapter for log levels, tags, filtering, and ID routing](issues/L18_central_manual_chapter.md) | Documentation |
| L19 | [Record the deferred proposal for level-specific compile-time selection](issues/L19_deferred_compile_time_levels.md) | Deferred design proposal - not a bug |
| L20 | [Record the deferred proposal for dynamic target-side log-level control](issues/L20_deferred_runtime_levels.md) | Deferred design proposal - not a bug |

### Broader design context retained from the discussion

| Ref | Standalone issue | Classification |
|---|---|---|
| L21 | [Rename the enrichment draft and separate levels, enrichment, and structured logging](issues/L21_clarify_logging_terminology.md) | Documentation / conceptual correction |
| L22 | [Remove unchanged-configuration heuristics from the future enrichment transformation contract](issues/L22_reversible_enrichment_instrumentation.md) | Draft architecture risk |
| L23 | [Validate the draft C, shell, JSON, and macro-stringization examples before publication](issues/L23_validate_draft_examples.md) | Documentation correctness |
| L24 | [Move internal parser development into a separate chapter and track it independently](issues/L24_separate_parser_chapter.md) | Documentation organization / scope control |
| L25 | [Separate device-log ingestion from native server logging and qualify performance claims](issues/L25_server_use_evidence.md) | Documentation / feasibility evidence |
| L26 | [Validate a useful structured-event workflow before specifying a strice extension](issues/L26_structured_logging_feasibility.md) | Exploratory feasibility - not an implementation specification |

### Additional source finding during handover

| Ref | Standalone issue | Classification |
|---|---|---|
| L27 | [Reject malformed IDRange arguments instead of silently skipping them](issues/L27_reject_malformed_idrange.md) | Bug / validation - additional finding during handover review |

### Interpretation

L19 and L20 deliberately preserve a deferred state. L22 and L26 are design/feasibility tasks, not an approved implementation specification. L10 and L15 protect behavior that already exists. L14 records a diagnostic-isolation risk supported by a shared path, not an executed proof that every error is suppressed. L27 was newly noticed while rechecking the source for this package.

---

**Part 3: Decisions and broader context**  
**Standalone source:** [context/REVIEW_AND_DECISIONS_EN.md](context/REVIEW_AND_DECISIONS_EN.md)

## Trice logging review: context, decisions, and open questions

**Date:** 10 September 2026.  
**Continuation branch requested by the maintainer:** `wip`. The maintainer reported that the branches had been merged and aligned. This handover does not independently certify equality of their commit hashes.  
**Purpose:** Preserve the technical substance and decision rationale so work can continue without the original conversation. This is not a verbatim transcript or a finished extension specification.

### 1. Executive decision

Retain Trice's efficient target-side ID-plus-values design. Correct the existing CLI/tag/routing problems before adding another target-level API. Preserve the ability to record received binary data and choose what to display later.

Additional per-level compile-time selection and a dynamic target-side level controller are deferred, not rejected as inherently useless. Reopen them for a measured resource limit, deterministic-execution requirement, or concrete build policy.

Keep `-logLevel` as a useful host-interface concept unless a separate compatibility decision retires it. Its present coupling to a display table and fragments needs clarification/correction. Do not confuse that option with the unimplemented draft `trice insert -loglevel`.

An optional structured-event extension could add significant value for automated firmware tests, measurement processing, and field diagnosis without changing target payloads for equivalent values. First demonstrate a real workflow; do not commit to a new server-logging ecosystem or a detailed API specification yet.

These are review recommendations and recorded planning decisions, not claims that patches have been merged. The issues distinguish observed defects from proposals and protective tests.

### 2. How the terminology was clarified

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

### 3. The proposed minimal structured interface

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

### 4. What til.json can and cannot validate

A project dictionary can serve as a reviewed vocabulary and schema reference. Validate new field names before accepting them into that reference. A name such as `temperture_c` can produce a warning/error with `temperature_c` as a suggestion; automatic correction is not advised.

Merely collecting all names and later checking against that enlarged set is not typo detection: it can legitimize a typo on first use. A strict mode needs an explicit approval process for legitimate new names. A centrally consistent misspelling still cannot be recognized as semantically wrong without an external convention or human review.

The future contract also needs types, widths, units, and argument positions. A `%d` or `{temperature_c}` token alone is not a complete domain schema. Retain raw numeric values independently of display precision or units attached to a string. The same field name must not silently change meaning between sites or firmware versions.

The reviewed `TriceFmt` definition has `Type` and `Strg`; this is a starting point, not proof that a complete structured-field schema is implemented. [Current ID data type](https://github.com/rokath/trice/blob/wip/internal/id/id.go)

### 5. One event ID can legitimately represent multiple keys

A single ID can select a template with several named fields:

```text
ID 4711: arg0 = motor_id, arg1 = temperature_c, arg2 = ctx.task_id
ID 5822: arg0 = temperature_c, arg1 = fan_rpm
```

On loading the dictionary, a host can build a lookup for `temperature_c`: ID 4711/argument 1 and ID 5822/argument 0. It can then evaluate a temperature query without generating or reparsing either message. No per-field identifier has to be transmitted on the target for that operation.

Distinguish four identities: the local decoding/template ID; the field key; an optional stable domain-event name; and one concrete event occurrence. Repeated transmission of one ID produces many event instances. The ID is not itself a globally unique occurrence identifier.

A host may intern keys to numeric IDs, build indexes, or transform records for a consumer. This is optional and does not imply a new target protocol. Field IDs do not themselves replace storage indexing. Source/dictionary identity can disambiguate independently assigned target IDs, while exhaustion within a local dictionary remains a separate capacity issue.

Restricting each target event to one key was considered and is not recommended. Splitting related values creates extra headers and a correlation problem under concurrent producers. Context enrichment naturally creates multi-field events anyway. A downstream transformation may split data only while preserving source and event-instance association. Windows event templates are another established example of one event definition containing multiple named data items. [Windows event definitions](https://learn.microsoft.com/en-us/windows/win32/wes/defining-events)

### 6. Enrichment and structured events fit together

Use one enrichment model for ordinary and prospective structured events where practical. Static fields can stay in the dictionary; source/session identity can be attached by the receiver; additional dynamic context must actually be acquired and associated with the event.

For identical values, widths, and existing encoding, retaining field names on the host adds no target payload bytes. That statement does not imply zero cost for wrapper calls, additional values, wider encodings, timestamp capture, task lookup, or a sensor read. Pigweed provides an embedded analogue for carrying static metadata through tokenized strings. [Pigweed tokenized logging](https://pigweed.dev/pw_log_tokenized/)

The conceptual operation is:

```text
original typed fields + configured context fields = one enriched event
```

Do not first collapse the original fields into a rendered `message` and then pretend the data remain accessible. Reserve or otherwise define a context namespace (for example `ctx.*`) to prevent collisions. An automatic ambient-temperature value must not overwrite a motor-temperature field. The namespace example is not a settled syntax.

Collection configuration belongs to the build/target contract. Display configuration belongs to the host. A host may reorder, hide, or repeat existing values, but cannot reconstruct a task ID that was never collected. A changed collection layout must retain a correct association with the firmware and dictionary.

The old stf/stv heuristic reversal and cache-invalidation assumptions are not a sound basis for a new implementation. Prefer generated artifacts or retained provenance compatible with bind. Support deterministic recovery from interrupted builds/configuration changes. See L22 rather than treating enrichment as permission to rewrite user sources unreliably.

### 7. Findings about existing level functionality

The implemented feature set is already substantial: broad `TRICE_OFF`, host pick/ban, host logLevel, ID allocation policies, and configured target ID routing. This is not a logger without level functionality.

The review identified concrete inconsistencies: cross-group aliases; exclusive routing endpoints despite inclusive allocation; inconsistent partial-bound activation; user-label list and initialization-order bugs; silently ignored selectors; late pick/ban conflict detection; and, during handover verification, a missing-colon `-IDRange` rule that is skipped instead of rejected.

Several other items are deliberately not labeled as proven missing features: single-pack `#error` checks already exist; raw capture is already before translation; source ID reuse outside a changed policy is an intentional stability behavior but needs auditing support. Desired tests must preserve these strengths, not replace them unnecessarily.

Host logLevel currently uses table positions and display fragments. Prefer a defined severity mapping and a clear treatment of categories without severity, rather than deleting the switch or imposing new target work. Preserve compatibility consciously. Do not assume every alias/category is a severity or that level names such as Fatal must trigger process termination.

Earlier host selection can save rendering effort, but requires agreed event/line boundaries, diagnostics, statistics, visualization, and timestamp-delta semantics. Decode/framing state and raw capture cannot simply be skipped along with presentation. Performance is not yet measured.

Detailed evidence is recorded in the individual issues and source review. The supplied chapters distinguish current limitations from future corrections.

### 8. ID history, routing, and packing constraints

An allocation range is not a migration. Existing active IDs can remain outside a newly specified range. Audit the current source/bind result; do not mistake every retained dictionary/location entry for an active call site.

A fix must be explicit: preview changes, allocate unused IDs, update source or sidecars coherently, preserve old mappings, protect shared state against competing writes, and rebuild. Do not edit only `til.json` or delete history to make a range checker quiet.

Current active deferred per-ID routing requires the relevant single-pack mode and already contains compile-time errors for incompatible multi-pack settings. Normalize activation logic and test guard reachability. This is not a universal TCOBS restriction, and it is not a blanket statement about custom direct routes. A future pre-pack decision could change the implementation boundary without changing the encoding itself.

Filtering on the host does not save target bandwidth. Routing a destination does not necessarily save initial acquisition or encoding work. Raw recordings can recover hidden host output, but not data lost or suppressed before receipt.

### 9. Reference systems and lessons retained

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

### 10. Server scope and practical value

Prioritize a server or gateway consuming device Trice streams and exporting usable fields to existing tools. A native server producer replacing an established logging library is a separate, broader project involving arbitrary strings, changing context sets, concurrency, errors, and integration.

The strongest near-term candidates are firmware-test assertions independent of English wording; measurement filtering/charting without reparsing text; and field diagnosis using stable field meaning across builds. Existing visualization features must be considered so a new API proves additional value rather than duplicating an adequate workflow.

Do not build a global key-ID registry, a new analysis platform, or a server-logger replacement merely to claim structured logging support. Do not widen the target ID format solely because fields need names. Benchmark CPU, bytes, storage, and end-to-end query costs at equivalent information content before making server performance claims.

### 11. Open extension questions retained for later specification

A later specification should resolve placeholder grammar and literal braces; argument order/count; duplicate names; types/widths and formatting hints; mixed data/string support and payload limits; canonical keys/units; missing values; safe JSON output; integer precision, NaN/infinity, and raw-versus-rendered values; dictionary and firmware identity; versioned event meaning; source/session metadata; context collisions and override rules; provider cost/ISR safety; event atomicity; wrapper provenance; disabled argument evaluation; and consumer/error-channel behavior.

Initially limiting support to known numeric messages is legitimate. Hashing a dynamic string can be useful for identification but does not retain its content. Static source locations should usually use suitable dictionary/location metadata rather than automatically increasing every event's runtime payload. A monotonic uptime is not automatically wall-clock time.

No final placeholder syntax, dictionary schema, new switch names, or automatic-migration command is approved by this handover. The examples explain the intended direction. Full specification work remains deferred until understanding and usefulness are established.

### 12. Manual organization and continuation

The web-retrieved UM map was 45.1 levels, 45.2 enrichment under the old title, 45.3 parser, 45.4 servers. Add the new central main chapter without hard-coding its eventual number. Rename the enrichment section with anchor compatibility, and move parser work to an independent chapter. Keep source-of-truth detail sections linked rather than maintaining contradictory descriptions.

The package contains 27 issue drafts, two language versions of the chapter, an issue index, coverage mapping, a source review, and a test plan. L01-L17 cover present behavior and safeguards; L18 is central documentation; L19-L20 record deferred target proposals; L21-L26 preserve the broader design context; L27 is an additional confirmed validation finding. These references are local labels, not GitHub issue numbers.

For continuation: record the actual `wip` commit, reproduce relevant static findings, fix and test the selected correctness issues, then update the known-issue notes. Do not infer that any fix was applied from the existence of these documents.

---

**Part 4: Source evidence and limits**  
**Standalone source:** [context/SOURCE_REVIEW_EN.md](context/SOURCE_REVIEW_EN.md)

## Source review and evidence limits

### Review scope

This handover was prepared on 10 September 2026. Public raw `wip` sources were retrieved and inspected through the web tool. The earlier discussion's implementation claims were not accepted solely from memory; the principal code paths were checked again.

An attempted checkout in the execution container failed because the container could not resolve GitHub. A direct container download also failed. Therefore no complete local checkout, immutable commit snapshot, upstream Go suite, firmware build, or hardware measurement is claimed. A commit listing obtained separately was not a reliable basis for pinning the retrieved moving-branch files; no unrelated hash was assigned to them.

The evidence is **dated source inspection of the retrieved branch files**. A later implementation must record `git rev-parse HEAD` from the actual checkout and reproduce the relevant cases. Links can change because `wip` can move. The package preserves observations and symbol names even if line numbers shift. It does not contain a complete repository snapshot.

### Evidence classes

| Class | Meaning |
|---|---|
| Source-confirmed | The reported condition follows directly from inspected code or a documented mismatch. It is not automatically an executed end-to-end reproduction. |
| Reliability/test requirement | A relevant boundary or shared code path exists; a protective behavior and regression test are requested. Not every potential failure has been demonstrated. |
| Proposed enhancement | A useful capability or workflow is being requested; the current implementation is not called defective simply for lacking it. |
| Deferred proposal | The idea is recorded for later reconsideration and is explicitly not an immediate implementation instruction. |
| Artifact validation | File structure, references, UTF-8, heading levels, and archive contents can be checked locally. Such checks are not Trice tests. |

### Rechecked source facts

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

### Findings deliberately not overstated

The source proves that diagnostics can share an event-filter path; it does not demonstrate loss of every diagnostic. The raw-capture placement is positive existing behavior, not a newly discovered missing feature. Single-pack routing checks already exist. A general source-history audit is not implemented merely because allocation options exist. A new severity map is not yet approved, and custom labels cannot be assigned arbitrary severities in prose and then treated as existing behavior.

No universal cycle/byte/energy saving is asserted. No target typeX0, legacy RPC, special user hook, or every possible custom backend has been exhaustively audited. The chapter's TRICE_OFF claims concern normal Trice macro use and the documented file/project scope, not a guarantee that every independently configured backend activity vanishes.

### Verification performed on the handover

See `ARTIFACT_VALIDATION.md` for the actual local artifact checks. Proposed acceptance-test checkboxes in issues are intentionally unchecked. They must not be interpreted as passing test results.

---

**Part 5: Coverage map**  
**Standalone source:** [context/COVERAGE_MAP_EN.md](context/COVERAGE_MAP_EN.md)

## Coverage map

This maps the maintainer's last numbered checklist, including repeated numbers, and the earlier relevant design discussion to the standalone files.

| Discussion item | Preserved content | Issue(s) |
|---|---|---|
| 1. Preserve the design | Decision record sections 1 and 7; chapter design principle | Decision record / chapters |
| 2. Defer trice_INFO compile-time selection | Existing TRICE_OFF distinguished from the proposed extra mechanism | [L19](issues/L19_deferred_compile_time_levels.md), [L18](issues/L18_central_manual_chapter.md) |
| 3. Defer dynamic target level control | Resource-driven reopening; routing is not the same as early suppression | [L20](issues/L20_deferred_runtime_levels.md), [L18](issues/L18_central_manual_chapter.md) |
| 4. Duplicate aliases and table-wide test | All identified spellings, case behavior, and user-label collisions | [L01](issues/L01_unique_tag_aliases.md) |
| 5a. Inclusive ID bounds | Allocation/output consistency and single-ID ranges | [L02](issues/L02_inclusive_routing_bounds.md) |
| 5b. Check changed ID policy | Active versus historical assignments; partial scans; optional strict checking | [L03](issues/L03_validate_active_id_policy.md) |
| 5b. Offer a safe fix | Explicit preview/migration, metadata coherence, historical mappings | [L04](issues/L04_safe_id_policy_migration.md) |
| 6. User-label implementation | Registration order and documented colon-list parsing | [L05](issues/L05_ulabel_registration_order.md), [L06](issues/L06_ulabel_colon_lists.md) |
| 7. Reject unknown selectors | Includes empty/malformed lists and invalid logLevel values | [L07](issues/L07_reject_unknown_selectors.md) |
| 7. Reject pick plus ban early | Empty input and invalid-token corner cases | [L08](issues/L08_reject_conflicting_filters_early.md) |
| 7. Move host filtering earlier | Measured optimization after semantics are defined | [L17](issues/L17_early_host_filtering.md) |
| 8. Single-pack requirement | Existing errors, not a missing check; positive/negative test coverage | [L10](issues/L10_single_pack_guard_tests.md) |
| 8. Routing activation details | UARTA/UARTB difference; partial/zero/default bounds | [L09](issues/L09_consistent_routing_configuration.md) |
| 8. Severity and logLevel retention | Category/severity separation; custom labels; deliberate compatibility decision | [L11](issues/L11_loglevel_semantics.md) |
| 9. Central bilingual UM chapter | H2/H3/H4; existing options and unimplemented drafts; integration instructions | [L18](issues/L18_central_manual_chapter.md) |
| 10. Metadata consistency | An accepted event and its metadata use a coherent decision | [L12](issues/L12_event_metadata_filtering.md) |
| 10. Partial lines and multi-line events | Compatibility, EOF, addNL, metadata origin | [L13](issues/L13_fragment_and_multiline_policy.md) |
| 10. Diagnostics | Separate tool/decode policy and future machine-readable integrity | [L14](issues/L14_diagnostic_filter_isolation.md) |
| 10. Raw recording and replay | Preserve pre-filter capture; append and dictionary precautions | [L15](issues/L15_raw_capture_replay_guarantee.md) |
| 10. Statistics | Count semantics independent of presentation and filter optimization | [L16](issues/L16_statistics_semantics.md) |
| Earlier discussion: terminology | Enrichment is not structured logging; exact UM section map | [L21](issues/L21_clarify_logging_terminology.md) |
| Earlier discussion: unsafe draft reversal | Provenance, bind compatibility, interrupted builds, caches | [L22](issues/L22_reversible_enrichment_instrumentation.md) |
| Earlier discussion: example correctness | C/shell/JSON/stringization, serialization, compression qualifications | [L23](issues/L23_validate_draft_examples.md) |
| Earlier discussion: separate parser task | Independent chapter and narrow typed-event interface boundary | [L24](issues/L24_separate_parser_chapter.md) |
| Earlier discussion: server usefulness | Device ingestion versus native server logging, evidence and benchmarks | [L25](issues/L25_server_use_evidence.md) |
| Earlier discussion: strice usefulness and key IDs | Single ID with many fields; no duplication; host transform; typing and wrappers | [L26](issues/L26_structured_logging_feasibility.md) |
| Newly rechecked detail | Malformed IDRange without colon is silently skipped | [L27](issues/L27_reject_malformed_idrange.md) |

### No lost distinctions

Existing behavior is not presented as a new feature request merely because it needs tests. Deferred design records are not immediate implementation tickets. Recommendations are not merged changes. The chapter does not claim that proposed CLI validation, a new severity mapping, or ID migration tools already exist.

The English decision record preserves the broader motivation, alternative interfaces, key-ID discussion, reference systems, known draft problems, and open questions for a later specification. It is the continuation context, not a verbatim conversation transcript.

---

**Part 6: Test plan**  
**Standalone source:** [context/TEST_PLAN_EN.md](context/TEST_PLAN_EN.md)

## Consolidated test and implementation plan

### Status

This is a plan, not an execution report. No listed upstream test has been run as part of this handover. Use the actual `wip` checkout, record its commit, and first reproduce each selected source finding. Reuse existing project fixtures rather than introducing a parallel testing framework.

### A. Tag and CLI tests

Validate the full exact-spelling registry, not a hard-coded blacklist of current duplicate aliases. Test built-in canonical names, case variants, all known collisions, user labels, and repeat registration. Restore global state between independent test runs; do not depend on process-wide initialization from earlier cases.

Test both option orders for user-label selection; repeated `-ulabel`; colon lists; duplicate/empty items; unknown pick/ban/logLevel names; a known but absent category; and simultaneous pick plus ban with both empty and nonempty input. Assert no port/file is opened on invalid configuration. Exercise both `tlog` and `trice log` entry points.

For IDRange, test missing colon/comma, empty parts, invalid numbers, reversed bounds, unknown names, alias-equivalent duplicate rules, touching/overlapping ranges, and legal adjacent disjoint ranges. A valid rule must not hide an invalid one.

### B. Target routing and compile matrix

For each relevant UART, auxiliary, and RTT backend, test absent/default routing, complete routing, partial bounds, reversed intervals, one-ID intervals, and legal extreme IDs. Include IDs `min-1`, `min`, an interior value, `max`, and `max+1` whenever legal in the test representation. Capture actual output callbacks.

Build the valid single-pack configurations and reject the incompatible active deferred multi-pack configurations. Confirm the expected error text; do not accept unrelated compile failures as success. Include valid multi-pack without ID routing. Audit custom/direct paths under their own contract, not by assuming every route is identical.

Where practical, cover both deferred buffer implementations and the relevant framing choices. Make sure inclusive comparisons do not accidentally activate disabled/default routes.

### C. ID policy and migration

Use current sources and generated assignments to classify active versus historical-only entries. Include a partially scanned project and stale location metadata. Test insert and bind independently with an existing assignment outside a changed policy. Pure validation must not write anything.

For any later explicit fix: preserve old mappings, preview every affected artifact, allocate only unused IDs, test exhaustion and write failure, use concurrency protection for shared state, and verify old/new recordings against the appropriate dictionaries. Do not claim a multi-file operation is atomic without a real transaction/recovery design.

### D. Host acceptance and rendering

Capture legacy behavior before changing severity semantics. Cover every built-in group, aliases, custom/unknown categories, untagged data, all/off, representative thresholds, and explicit tag selections. Choose and approve the no-severity policy before golden outputs are changed.

Compare color off/none/default; all metadata columns; prefix/suffix; local and remote rendering where supported; partial lines; multiple events per line; multiple lines per event; hidden fragments; EOF flush; addNL; and ID/location attribution. Define visualization interaction explicitly instead of silently changing it as an optimization side effect.

### E. Integrity, diagnostics, and recording

Use valid events interleaved with unknown IDs, malformed and truncated input, and supported integrity/cycle errors. Restrictive application filters must not erase the information needed to diagnose failure unless an explicit diagnostic policy says so. Any future JSON or similar stream must remain syntactically valid and not contain injected console banners.

For byte-exact binary capture tests, use fresh distinct output files. Compare the same received input under different host selectors and replay it with matching metadata. Never append into the replay input. Test recording write errors and incomplete input. Capture is limited to bytes actually received.

Count decoded, selected, displayed, and diagnostic events according to documented semantics. Color and metadata fragments must not multiply application-event counts. Preserve the reference used for displayed timestamp deltas across hidden events.

### F. Performance gates

Benchmark representative inputs at several suppression ratios, including no suppression. Record CPU time, allocations, throughput, output equivalence, and configuration. The early-filter implementation may avoid rendering, but still has to consume and validate the stream and maintain protocol state.

No target-level controller is to be added as part of the host optimization. For either deferred target proposal, first provide event-rate, bandwidth/storage, code-size, or argument-cost evidence. Compare the same retained information and configuration; do not infer a general speed or energy improvement from a small microbenchmark.

### G. Suggested sequencing

1. Establish a reproducible baseline and fix independent CLI/registry/routing correctness defects.
2. Add active ID-policy diagnostics; decide separately whether an explicit migration tool is needed.
3. Preserve packing guards, raw capture, diagnostics, and accounting boundaries with tests.
4. Approve and implement any host severity/metadata/fragment compatibility change.
5. Measure and implement earlier host filtering only if worthwhile.
6. Update the central chapter and known-issue notes alongside verified changes.
7. Keep target-level proposals deferred; evaluate structured-field usefulness in a small real workflow before writing a detailed extension specification.

---

**Part 7: Manual chapter (German)**  
**Standalone source:** [chapters/Log_Level_Tags_und_Filterung_DE.md](chapters/Log_Level_Tags_und_Filterung_DE.md)

<!-- Integration: vollstaendiges UM-Hauptkapitel (H2) mit H3/H4-Unterabschnitten.
Keine feste Kapitelnummer vergeben. Nach dem Einfuegen Nummerierung und
Inhaltsverzeichnis mit dem Projektverfahren aktualisieren. Deutsch und Englisch
sind alternative Sprachfassungen, keine aufeinanderfolgenden Kapitel. -->

### <a id="part07-log-levels-tags-and-filtering"></a>Log-Level, Tags und Filterung

> **Status:** Kapitelentwurf auf Grundlage der am 10. September 2026 abgerufenen `wip`-Quellen. Vorhandenes Verhalten, bekannte Einschränkungen und vorgeschlagene Änderungen sind getrennt beschrieben. Die Quellen wurden geprüft; die vollständigen Testsuiten und Target-Builds wurden für dieses Kapitel nicht ausgeführt. Die genannten Korrekturen werden nicht als bereits umgesetzt vorausgesetzt.

#### <a id="part07-ll-purpose"></a>Zweck und Grundprinzip

Im normalen Trice-Logging wird eine Meldung durch eine ID identifiziert. Statt eines vollständig formatierten Meldungstextes werden ihre Laufzeitwerte übertragen. Der Host erzeugt die Ausgabe mithilfe des passenden Wörterbuchs. Dadurch ist es praktikabel, Diagnoseinformationen zunächst zu behalten und die sichtbare Teilmenge später auszuwählen. Dasselbe Prinzip kann eine optionale strukturierte Ausgabe tragen, ohne jedem Target-Ereignis zusätzliche Level- oder Feldnamenbytes hinzuzufügen. [part07-S1] [part07-S2]

**Den effizienten Target-Kern beibehalten und zuerst die vorhandenen Auswahlmöglichkeiten nutzen. Feinere Target-Steuerungen erst ergänzen, wenn ein konkreter Ressourcen- oder Ausführungsbedarf sie rechtfertigt.** Das ist die Empfehlung dieser Bewertung, nicht die Behauptung, Target-Filter seien generell nutzlos.

Ein kurzer einzelner Logaufruf garantiert keine vernachlässigbare Gesamtbelastung. Zu betrachten sind Ereignisrate, Argumentberechnung, Pufferung, Framing, Übertragung und Aufzeichnung:

```text
Bytes pro Sekunde = Ereignisse pro Sekunde * mittlere Bytes pro Ereignis
```

TCOBS-Framing beziehungsweise -Kodierung definiert keinen Log-Level. Kompakte Kodierung garantiert weder eine feste Kompressionsrate noch, dass zusätzliche Speicherkompression keinen Nutzen mehr hätte. Für die hier beschriebenen Korrekturen ist keine TCOBS-Änderung erforderlich.

#### <a id="part07-ll-terminology"></a>Begriffe

| Begriff | Bedeutung in diesem Kapitel |
|---|---|
| Ereignis | Ein konkreter Trice-Logaufruf. Eine angezeigte Zeile kann mehrere Ereignisse enthalten; ein Ereignis kann mehrere Zeilenumbrüche enthalten. |
| Tag oder Kanal | Ein Präfix wie `err:`, `wrn:` oder `read:`. Es kann einen Schweregrad, eine Kategorie oder eine Darstellungskonvention bezeichnen. |
| Alias | Eine andere akzeptierte Schreibweise derselben Tag-Gruppe. Eine identische Schreibweise darf nicht verschiedenen Gruppen zugeordnet sein. |
| Log-Level / Schweregrad | Wichtigkeit oder Schwere eines Ereignisses, beispielsweise Debug, Information, Warnung oder Fehler. |
| Tag-Auswahl | Auswahl einer ausdrücklichen Kategorienmenge mit `-pick` beziehungsweise Ausschluss mit `-ban`. |
| Level-Schwelle | Auswahl nach einer festgelegten Schweregradordnung. Sie ist nicht dasselbe wie eine ausdrückliche Tag-Liste. |
| Kontextanreicherung | Ergänzen von Eigenschaften wie Task-ID, Firmwarekennung, Quellposition oder Zeitstempel, ohne sie an jeder Logstelle zu wiederholen. |
| Strukturiertes Logging | Informationen bleiben als einzeln zugängliche Datenfelder erhalten und stehen nicht ausschließlich innerhalb eines formulierten Satzes. |
| ID-Routing | Auswahl eines Ausgabekanals anhand der numerischen Ereignis-ID. |

Eine Kategorie wie Lesen, Schreiben oder Security ist nicht von sich aus ein Schweregrad. `OFF` ist eine Einstellung, kein Schweregrad eines ausgegebenen Ereignisses. Numerische Levelwerte sind Konventionen und benötigen zwischen Systemen ausdrückliche Zuordnungen; Trice muss keine vermeintlich universelle Nummerierung übernehmen.

Ein strukturierter Datensatz darf eine freie Textmeldung als einen seiner Werte enthalten. Umgekehrt macht das automatische Einfügen von Kontext in einen Textstring diese Angaben noch nicht automatisch einzeln abfragbar. JSON ist eine mögliche Ausgabeform, nicht die Definition strukturierten Loggings. Diese Unterscheidungen passen zu den getrennten Konzepten für Meldungsinhalt, Attribute, Quelle und Schweregrad im OpenTelemetry-Logmodell. [part07-S10]

#### <a id="part07-ll-selection-points"></a>Wo ausgewählt wird und welche Kosten entfallen

| Mechanismus | Auswahlzeitpunkt | Mögliche Einsparung | Was bleibt |
|---|---|---|---|
| `TRICE_OFF` | Kompilierung im konfigurierten Datei-/Projektumfang | Code normaler abgeschalteter Trice-Makros und deren Argumentauswertung | Nicht erzeugte Ereignisse sind später nicht wiederherstellbar |
| Konfiguriertes Target-ID-Routing | Weiterleitung an eine bestimmte Ausgabe | Übertragung/Speicherung an dieser Ausgabe | Erfassung und im geprüften Deferred-Pfad auch Kodierung sind bereits erfolgt |
| `-pick` / `-ban` | Host, derzeit nach dem Formatieren der Meldung | Nachfolgende normale Anzeige/Ausgabe ausgeschlossener Ereignisse | Targetaufwand, Datenübertragung und vorherige Hostdekodierung/-formatierung |
| `-logLevel` | Host-Darstellung im geprüften Stand | Anzeige nach den aktuellen Tag-Reihenfolgeregeln | Targetaufwand und Erfassung; keine Target-Steuerung |
| Geplante Compilezeit-Auswahl nach Level | Als Draft-Erweiterung nicht implementiert | Würde ausgewählte Level aus einem Build entfernen | Wiederaufnahme erfordert einen anderen Build |
| Geplante dynamische Target-Level-Steuerung | Als Draft-Erweiterung nicht implementiert | Könnte Erfassungs- und/oder Übertragungsarbeit vermeiden | Zusätzliche Regeln, Steuerung und Targetentscheidungen |

Die Aussage zum Deferred-Zeitpunkt gilt für die geprüfte Implementierung, nicht pauschal für jede anwenderdefinierte Direct-Ausgabe. Routing ist nicht automatisch eine Unterdrückung vor der Erfassung. [part07-S2] [part07-S3] [part07-S4] [part07-S12] [part07-S5] [part07-S14]

#### <a id="part07-ll-target-off"></a>Compilezeit-Steuerung mit TRICE_OFF

Für die normale Trice-Makronutzung einer Quelldatei wird `TRICE_OFF` vor dem ersten Einbinden von `trice.h` in dieser Übersetzungseinheit definiert:

```c
#define TRICE_OFF 1
#include "trice.h"
```

Eine projektweite Compilerdefinition wie `-DTRICE_OFF=1` wirkt entsprechend breiter. Buildkonfiguration und Include-Reihenfolge müssen zusammenpassen. Die Trice-Quellwerkzeuge können die Logstellen und Wörterbuchinformationen weiterhin berücksichtigen; das Herauskompilieren ist keine Aufforderung, historische Einträge aus `til.json` zu löschen. [part07-S1] [part07-S5]

Logargumente sollten keine für das Programm notwendigen Seiteneffekte enthalten. Bei Wrappern ist zusätzlich zu beachten: Das Abschalten eines Trice-Makros innerhalb einer normalen Funktion verhindert nicht zwingend die Auswertung ihrer Argumente beim Aufrufer. Beispielsweise kann `log_motor(read_sensor())` weiterhin `read_sensor()` aufrufen. Ein späterer levelabhängiger Wrapperentwurf muss dies ausdrücklich berücksichtigen.

Diese breite Compilezeit-Steuerung existiert bereits. Der zusätzliche Mechanismus mit `trice_INFO`/`trice_DEBUG` ist eine eigene, zurückgestellte Erweiterung.

#### <a id="part07-ll-tags-aliases"></a>Tags, Aliase und eigene Labels

Ein bekannter Tag wird normalerweise am Meldungsanfang vor dem ersten Doppelpunkt erkannt. Aliasgruppen ordnen Schreibweisen wie `err` und `ERROR` derselben Kategorie zu. Die Groß-/Kleinschreibung kann trotzdem die Darstellung beeinflussen: Bekannte kleingeschriebene Präfixe werden in den entsprechenden Farbmodi normalerweise entfernt, großgeschriebene können sichtbar bleiben. Die Farboptionen `off` und `none` sind deshalb nicht gleichbedeutend. [part07-S3] [part07-S6]

**Bekannter Fehler im geprüften Stand:** Die identischen Schreibweisen `w`, `rx`, `tx`, `s` und `S` kommen in unterschiedlichen Gruppen vor. Einige Codepfade nehmen den ersten Treffer, andere berücksichtigen weitere Treffer. Bis zur Korrektur sollten eindeutige Schreibweisen wie `wrn`, `write`, `read`, `receive`, `transmit`, `signal` und `Seconds` verwendet werden. Die Korrektur benötigt eine Prüfung des gesamten Registers einschließlich künftiger Ergänzungen und eigener Labels; es handelt sich nicht nur um eine kosmetische Tabellenbereinigung. [part07-S3]

Der tatsächliche Schalter heißt **`-ulabel`**, nicht `-uLabel`. Er registriert eigene Labels, weist ihnen aber nicht automatisch einen fachlich sinnvollen Schweregrad zu. Dokumentiert ist diese Syntax:

```text
-ulabel motor:sensor -ulabel control
```

**Bekannte Einschränkungen im geprüften Stand:** Die Implementierung zerlegt die Doppelpunktliste nicht wie dokumentiert. Außerdem werden Pick-/Ban-Aliase aufgelöst, bevor die neu angegebenen Labels registriert sind. Ein eigener Selektor im selben Kommando funktioniert deshalb nicht schon dadurch zuverlässig, dass `-ulabel` zuerst steht. Diese Verwendung setzt die entsprechenden Korrekturen oder eine ausdrückliche Prüfung des konkreten Verhaltens voraus. [part07-S3] [part07-S4] [part07-S12] [part07-S6]

Die angestrebte Reihenfolge lautet: Optionen sammeln; alle Labels registrieren und validieren; Selektoren auflösen; ungültige oder widersprüchliche Angaben ablehnen; erst danach Ein- und Ausgaben öffnen. Das ist eine vorgeschlagene Korrektur, keine Beschreibung des aktuellen Ablaufs.

#### <a id="part07-ll-pick-ban"></a>Ausdrückliche Tag-Auswahl mit pick und ban

`-pick` behält die genannten Tag-Gruppen. `-ban` unterdrückt die genannten Gruppen. Listen verwenden Doppelpunkte; die Schalter können wiederholt werden. Bekannte Aliase werden auf ihre Gruppe erweitert. [part07-S4] [part07-S12]

```text
-pick wrn:err
-ban dbg
```

`-pick wrn:err` wählt genau diese Gruppen. Es bedeutet **nicht** eine Level-Schwelle wie „Warnung und alles Schwerwiegendere“; weitere schwere Gruppen werden nicht automatisch eingeschlossen.

Im geprüften Filter wird ein ungetaggtes Ereignis bei einer nichtleeren Pick-Auswahl unterdrückt und passiert eine Ban-Auswahl. Ein nicht passendes Präfix wird ebenso durch Pick ausgeschlossen und durch Ban durchgelassen. Beide Optionen sparen weder Targetarbeit noch Übertragungsbytes. [part07-S4] [part07-S12]

`-pick` und `-ban` dürfen nicht gemeinsam verwendet werden. Der entsprechende Fehler wird aktuell erst im Ereignisfilter erkannt; er sollte stattdessen bereits bei der Startvalidierung gemeldet werden.

**Bekannter Validierungsfehler:** Unbekannte Namen werden stillschweigend verworfen. Eine vollständig unbekannte Pick-Auswahl kann dadurch zu „kein Filter“ werden. Richtig wäre ein Kommandozeilenfehler mit dem ungültigen Namen, nachdem eigene Labels registriert sind. Ein bekannter Selektor ohne eintreffende passende Ereignisse ist hingegen gültig; fehlende Daten sind kein Schreibfehler. [part07-S4] [part07-S12]

#### <a id="part07-ll-host-level"></a>Hostseitiges logLevel

Die vorhandene Hostoption wird mit großem `L` geschrieben:

```text
-logLevel all
-logLevel wrn
-logLevel off
```

`all` ist die Voreinstellung. `off` unterdrückt die normale Ausgabe im betreffenden Darstellungspfad; es schaltet weder die Targeterfassung noch eine angeforderte Binäraufzeichnung ab. Für Werkzeugdiagnosen und andere Verbraucher braucht es ausdrückliche Regeln statt der Annahme, der Schalter lege alles still. [part07-S3] [part07-S6]

**Aktuelle Einschränkung:** Der Schwellwert wird aus der Position in der Darstellungstabelle `Tags` abgeleitet. Diese enthält neben herkömmlichen Levelnamen auch Kategorien, Einheitentags und angehängte Benutzerlabels. Die Levelprüfung beeinflusst zudem Darstellungsfragmente wie Zeitstempel, Quellposition, Präfix und Suffix. Sie ist damit noch kein unabhängiges, durchgängiges Ereignis-Schweregradmodell. [part07-S3] [part07-S6]

Bevorzugt wird, die nützliche Schwellwertfunktion zu behalten und Schweregrade von der Darstellung zu trennen. Für Kategorien ohne Schweregrad, Benutzerlabels, ungetaggte Ereignisse und die Kombination mit einer ausdrücklichen Tag-Auswahl sind Regeln festzulegen. Ein neuer Farbeintrag darf den Schweregrad nicht verändern. Dafür braucht es weder ein zusätzliches Target-Level-Feld noch nach Schweregrad geordnete IDs.

`-logLevel` sollte nicht stillschweigend entfernt werden. Auch seine Entfernung bricht Kompatibilität. Falls seine Pflege bewusst verworfen wird, braucht es eine angekündigte Abkündigung mit Migrationsweg. Unbekannte Werte sollten Startfehler auslösen statt zufälliger Tabellenindexeffekte.

#### <a id="part07-ll-raw-recording"></a>Binäraufzeichnung und spätere Neuauswahl

`-binaryLogfile` und der Alias `-blf` zeichnen den empfangenen Datenstrom vor Übersetzung und Anzeigefilterung auf. `-logfile`/`-lf` erfassen Textausgabe und sind kein gleichwertiger Ersatz. Eine explizit benannte Binärdatei wird erweitert, wenn sie bereits existiert. [part07-S6] [part07-S7]

Ein Beispiel für die Wiedergabe, wenn `session.bin` und die passende `til.json` bereits im Arbeitsverzeichnis liegen:

```bash
tlog -p FILEBUFFER -args session.bin -idlist til.json -pick wrn:err -blf off
```

Dieselben empfangenen Daten ohne diese Tag-Auswahl wiedergeben:

```bash
tlog -p FILEBUFFER -args session.bin -idlist til.json -logLevel all -blf off
```

`tlog` verwendet den Logging-Kommandopfad; `trice log` akzeptiert die entsprechenden Logging-Optionen. Abweichende Kodierung, Framing- und Targeteinstellungen sowie eine gewünschte Standortdatei müssen passend angegeben werden. Eine Wiedergabe kann kein Ereignis rekonstruieren, das nie empfangen wurde. [part07-S7] [part07-S8]

Zur Aufzeichnung gehören das relevante Wörterbuch und die Firmware-/Buildidentität. Bei wichtigen Quellpositionen sind auch die passenden Standortmetadaten aufzubewahren. Die Wiedergabe darf nicht in ihre eigene Eingabedatei zurückschreiben. Eine absicherbare Eigenschaft lautet: Gleicher empfangener Datenstrom ergibt unabhängig von Host-Anzeigefiltern dieselbe Binäraufzeichnung.

#### <a id="part07-ll-id-allocation"></a>ID-Vergaberegeln und Wörterbuchhistorie

Insert und Bind bieten `-IDMin`, `-IDMax`, `-IDMethod` (`random`, `upward` oder `downward`) sowie wiederholbare tagbezogene `-IDRange`-Regeln. Allgemeiner und spezielle Bereiche dürfen sich nicht überschneiden. Diese Optionen steuern die ID-Vergabe; sie konfigurieren nicht von selbst ein Ausgabegerät. [part07-S6] [part07-S9] [part07-S13]

Dieses Beispiel veranschaulicht eine Regel für ein Anwendungsquellverzeichnis `app`. Es schreibt die üblichen Bind-Artefakte und muss an das tatsächliche Projekt angepasst werden:

```bash
trice bind -src ./app -IDMin 1000 -IDMax 9999 -IDRange err:10,99
```

Der Fehlerbereich reicht einschließlich von 10 bis 99. Andere betreffende Tags erhalten IDs aus dem allgemeinen Bereich. Die Bereiche müssen im tatsächlich unterstützten ID-Raum liegen; die Konfiguration sollte zum Build archiviert werden.

**Vorhandene IDs werden nicht automatisch migriert.** Die Wiederverwendungslogik kann eine ID außerhalb einer neu konfigurierten Vorgabe beibehalten. Das dient der Stabilität, macht eine Vergaberegel aber noch nicht zum Nachweis, dass alle aktiven Fehlermeldungen bereits in diesem Bereich liegen. `til.json` enthält außerdem historische Einträge und darf nicht pauschal umnummeriert werden. [part07-S9] [part07-S13]

Vorgeschlagen ist eine Prüfung aktiver Zuordnungen gegen die aktuelle Regel, mit aussagekräftigen Warnungen und gegebenenfalls strengem CI-Modus. Rein historische Einträge sind getrennt zu behandeln; eine unvollständige Quellprüfung kann keine vollständige Konformität beweisen. Ein optionaler Fix sollte Änderungen vorab anzeigen, Quellen oder Sidecars konsistent aktualisieren, alte Zuordnungen erhalten und einen Neubau verlangen. Solche Prüf-/Migrationsfunktionen sind Vorschläge, keine hier dokumentierten vorhandenen Kommandos.

**Zusätzlicher Validierungsfehler:** `EvaluateIDRangeStrings` überspringt im geprüften Stand Regeln ohne Doppelpunkt stillschweigend. Fehlerhafte Angaben sollten vor Änderungen an Quellen oder generierten Artefakten abgelehnt werden. [part07-S9] [part07-S13]

#### <a id="part07-ll-target-routing"></a>Target-ID-Routing und Packing-Modi

Eine konfigurierte Ausgabe kann mittels ID-Intervall nur ausgewählte Meldungen weiterleiten. Andere Ausgaben können dasselbe Ereignis trotzdem erhalten. Beispielsweise kann ein Ziel den breiten Datenstrom und ein anderes nur den Fehlerbereich bekommen. Das ist Ausgaberouting, nicht zwangsläufig eine Unterdrückung der anfänglichen Erfassung. [part07-S2] [part07-S5] [part07-S6]

Der beabsichtigte Intervallvertrag ist einschließlich beider Grenzen. **Bekannter Fehler:** Die geprüften Deferred-Vergleiche verwenden beidseitig striktes `<`, obwohl Vergabe und Beschreibung der kleinsten/größten ID einschließlich sind. Bis zur Korrektur darf die Weiterleitung der Grenzereignisse nicht vorausgesetzt werden. Eine zwischenzeitliche Erweiterung der Grenzen als Workaround ist nach einer Korrektur erneut zu prüfen. [part07-S2] [part07-S5] [part07-S9] [part07-S13]

Ein zweites Konfigurationsproblem betrifft die Aktivierung: UARTA und UARTB prüfen von null verschiedene Grenzen unterschiedlich (`||` beziehungsweise `&&`). Auxiliary-/RTT-Pfade verwenden Definitionstests. Fehlende, voreingestellte und nullwertige Grenzen brauchen einen eindeutigen gemeinsamen Vertrag; der bloße Austausch eines Operators reicht nicht. [part07-S2]

Für die derzeitigen aktiven Deferred-Routingzweige nach Ereignis-ID gilt:

```c
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
```

**Die dazugehörigen Compilezeit-`#error`-Prüfungen existieren bereits.** Sie sollten mit positiven und negativen Buildtests für die betreffenden Ausgaben und Teilkonfigurationen abgesichert werden. Ein pauschaler Fehler für jede Multi-Pack-Nutzung wäre falsch. Gültiges Multi-Pack ohne ID-Routing ist ein anderer Fall; Direct-/Custom-Routen brauchen ihren eigenen Vertrag. [part07-S2]

Die Einschränkung ist keine grundsätzliche TCOBS-Eigenschaft. Eine spätere Implementierung könnte vor dem Zusammenpacken über einzelne Ereignisse entscheiden; das wäre eine separate, durch Nutzen zu begründende Änderung.

#### <a id="part07-ll-integrity-and-performance"></a>Diagnosen, Ereignisgrenzen, Statistiken und Hostleistung

Ein zugelassenes Ereignis und seine ausgewählten Metadaten sollten derselben Filterentscheidung folgen. Der Styling-Tag eines Zeitstempels darf nicht entscheiden, ob der Zeitstempel einer zugelassenen Warnung verschwindet. Zugleich darf die bestehende Zusammensetzung von Teilzeilen nicht versehentlich verändert werden: Mehrere Aufrufe können eine Zeile bilden und ein Aufruf kann mehrere Zeilen umfassen. Tests müssen EOF-Abschluss, `-addNL`, ausgeblendete Fragmente und die Herkunft der Metadaten abdecken. [part07-S3] [part07-S4] [part07-S12]

Der Translator führt formatierte Ereignisse und bestimmte Fehlermeldungen durch denselben Tag-Filter. Für unbekannte IDs und Dekodier-/Integritätsdiagnosen ist deshalb eine bewusste eigene Regel erforderlich. Sie dürfen nicht nur deshalb verschwinden, weil die Anwendungsauswahl ihren Darstellungstag nicht enthält. Eine künftige maschinenlesbare Ausgabe muss Diagnosen trennen oder unter einem definierten Schema kodieren. [part07-S4] [part07-S12]

Bei Statistiken ist festzulegen, was gezählt wird: Empfangene Daten, dekodierte Ereignisse, ausgewählte Ereignisse, angezeigte Ereignisse und Diagnosen sind nicht identisch. Einfärben und Aufteilen in Darstellungsfragmente dürfen keine zusätzlichen Anwendungsereignisse erzeugen.

Eine frühere Hostfilterung ist eine sinnvolle Optimierungsoption, da Pick/Ban aktuell auf bereits formatierten Meldungen arbeiten. Eine Entscheidung anhand von Decoder und Wörterbuch könnte Formatierungsarbeit sparen. Framing, Cycle-/Integritätszustand, notwendige Prüfungen, Rohaufzeichnung sowie die festgelegte Semantik von Zeitdifferenzen, Statistiken und Visualisierung müssen dabei erhalten bleiben. Vor zusätzlicher Komplexität sind CPU, Allokationen und der ungefilterte Ausgangspfad zu messen. [part07-S4] [part07-S12] [part07-S11]

#### <a id="part07-ll-deferred-extensions"></a>Zurückgestellte Target-Level-Erweiterungen

##### <a id="part07-ll-deferred-compile"></a>Compilezeit-Auswahl nach Level

Der Future-Development-Draft schlägt Namen wie `trice_INFO`, `trice_DEBUG` und eine dazugehörige Buildtransformation vor. Damit könnten ausgewählte Ereignisse samt Argumentauswertung aus einem Produktionsbuild verschwinden. Gleichzeitig entstehen weitere Makrofamilien, Werkzeugregeln und Buildvorgaben. [part07-S1]

**Entscheidung dieser Bewertung: Bis zu einem unmittelbaren, dokumentierten Bedarf zurückstellen.** Ein Anlass wäre eine gemessene Codegrößen-/Laufzeitgrenze oder eine notwendige Buildvorgabe, die `TRICE_OFF` und die vorhandenen Mittel nicht ausreichend erfüllen. Ein späterer Entwurf muss zu den vorgesehenen Bind-/Insert-Arbeitsabläufen passen und die Argumentauswertung bei abgeschaltetem Logging verhindern.

Das im Draft vorgeschlagene `trice insert -loglevel` ist **nicht** der vorhandene Hostschalter `-logLevel`. Großschreibung und Kommandokontext sind relevant. Der Vorschlag darf nicht als benutzbares Kommando erscheinen.

##### <a id="part07-ll-deferred-runtime"></a>Dynamische Level-Steuerung auf dem Target

Ein dynamischer Schwellwert könnte bei hohen Ereignisraten, begrenzter Übertragungsbandbreite, kleinem Offline-Speicher oder teuren Logargumenten nützlich werden. Dafür wären Aktualisierung, Nebenläufigkeit und genauer Entscheidungszeitpunkt festzulegen. Eine lokale Anwendungssteuerung kann ausreichen; Fernsteuerung kann einen vorhandenen oder neuen Kommandopfad benötigen. [part07-S1]

**Entscheidung dieser Bewertung: Ebenfalls bis zu einem konkreten Bedarf zurückstellen.** Der geringe Aufwand je Ereignis ist ein gutes Argument für das bestehende Design, beweist aber keine geringen Gesamtkosten für jeden Anwendungsfall. Das vorhandene, einkompilierte Routing ist kein dynamischer früher Filter.

Keine der beiden Target-Level-Erweiterungen ist Voraussetzung für Kontextanreicherung oder strukturierte Felder auf dem Host. Zusätzliche Levelbytes pro Ereignis werden nicht vorausgesetzt.

#### <a id="part07-ll-related-work"></a>Beziehung zu Enrichment, strukturiertem Logging und Servern

Die geprüfte UM-Zuordnung lautet: 45.1 Level-Control-Draft; 45.2 der überwiegend auf Enrichment gerichtete Draft; 45.3 interne Parserentwicklung; 45.4 Servernutzung. Bei einer Verschiebung sollten stabile Anker verwendet werden. Das Enrichment-Kapitel sollte umbenannt und die Parseraufgabe in ein eigenes Kapitel beziehungsweise Arbeitspaket ausgelagert werden. [part07-S1]

Eine mögliche `strice`-Erweiterung kann eine Ereignis-ID für mehrere benannte Felder behalten. Der Host kann ein Feld über verschiedene IDs und Argumentpositionen auflösen, optional eine numerische Host-Feld-ID vergeben und daraus Text oder strukturierte Ausgabe erzeugen. Eine Beschränkung auf einen Key pro Targetnachricht ist nicht notwendig und kann die sinnvolle Zusammenfassung von Kontext erschweren. Dies bleibt ein Vorschlag, keine implementierte öffentliche Schnittstelle.

Bei Servern ist zwischen dem Auswerten von Geräte-Trice-Strömen und dem Ersetzen eines nativen Serverloggers zu unterscheiden. Ersteres passt zum effizienten Target-Design; Letzteres benötigt eigene Bedarfsnachweise und Messungen. Tokenisierung beweist weder geringeren Gesamtenergiebedarf noch den Wegfall jeder zusätzlichen Kompression. Zuerst ist ein praktischer Arbeitsablauf zu validieren, danach eine detaillierte Erweiterungsspezifikation zu erstellen.

#### <a id="part07-ll-summary"></a>Empfohlene Reihenfolge

Target-Kern und Datenformat beibehalten. Zuerst mehrdeutige Aliase, CLI-Validierung, Benutzerlabels, Routinggrenzen und Routingaktivierung korrigieren. Vorhandene Packing-Prüfungen und Rohaufzeichnung durch Tests absichern. Vor früherer Filterung die Hostsemantik für Schweregrade, Metadaten, Zeilenzusammensetzung, Diagnosen und Statistiken klären. Diese Übersicht mit den technischen Detailkapiteln verbinden. Feinere Target-Auswahl erst bei einem tatsächlichen Bedarf erneut aufgreifen.

#### <a id="part07-ll-references"></a>Implementierungs- und Begriffsreferenzen

Die Quellenlinks zeigen auf den geprüften Branch, nicht auf einen unveränderlichen Commit. Vor Implementierungsarbeiten erneut prüfen und den tatsächlichen Commit festhalten.

[part07-S1]: https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md
[part07-S2]: https://github.com/rokath/trice/blob/wip/src/trice.c
[part07-S3]: https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go
[part07-S4]: https://github.com/rokath/trice/blob/wip/internal/translator/translator.go
[part07-S5]: https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h
[part07-S6]: https://github.com/rokath/trice/blob/wip/internal/args/init.go
[part07-S7]: https://github.com/rokath/trice/blob/wip/internal/args/handler.go
[part07-S8]: https://github.com/rokath/trice/blob/wip/internal/receiver/receiver.go
[part07-S9]: https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go
[part07-S10]: https://opentelemetry.io/docs/specs/otel/logs/data-model/
[part07-S11]: https://github.com/rokath/trice/blob/wip/internal/decoder/decoder.go
[part07-S12]: https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go
[part07-S13]: https://github.com/rokath/trice/blob/wip/internal/id/insertIDs.go
[part07-S14]: https://github.com/rokath/trice/blob/wip/src/triceRingBuffer.c

---

**Part 8: Manual chapter (English)**  
**Standalone source:** [chapters/Log_Levels_Tags_and_Filtering_EN.md](chapters/Log_Levels_Tags_and_Filtering_EN.md)

<!-- Integration: this is a complete main UM chapter (H2), with H3/H4 subsections.
Do not assign a fixed chapter number here. Insert into the UM, then regenerate
numbering and the table of contents. This English file and the German file are
alternative language versions, not consecutive chapters. -->

### <a id="part08-log-levels-tags-and-filtering"></a>Log Levels, Tags and Filtering

> **Status:** Review draft based on the `wip` sources retrieved on 10 September 2026. Existing behavior, known limitations, and proposed changes are distinguished below. The referenced source was inspected, but the upstream test suites and target builds were not executed for this chapter. The fixes mentioned here are not assumed to have landed.

#### <a id="part08-ll-purpose"></a>Purpose and design principle

Trice's normal logging path identifies a message by an ID and transmits its runtime values rather than a fully rendered message. The host uses the matching dictionary to reconstruct the output. This makes it practical to retain diagnostic detail and select the visible subset later. The same principle can support optional structured output without adding a level or field name to every target event. [part08-S1] [part08-S2]

**Keep the efficient target design. Use existing selection mechanisms first. Add finer target-side controls only when a concrete resource or execution requirement justifies them.** This is the recommendation of this review, not a claim that target filtering is never useful.

A short individual logging call does not guarantee negligible aggregate cost. Evaluate event frequency, argument computation, buffering, framing, transmission, and recording together:

```text
bytes per second = events per second * average transmitted bytes per event
```

TCOBS framing/encoding does not define a log level. Nor does compact encoding guarantee a fixed compression ratio or eliminate the possible benefit of subsequent storage compression. No TCOBS change is needed for the corrections discussed in this chapter.

#### <a id="part08-ll-terminology"></a>Terminology

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

A structured record may contain a free-text message as one value. Conversely, automatically adding context to a text string does not by itself expose that context as independently queryable fields. JSON is one possible output representation, not the definition of structured logging. These distinctions are consistent with the separate body, attributes, source, and severity concepts of the OpenTelemetry log model. [part08-S10]

#### <a id="part08-ll-selection-points"></a>Where selection happens and what it saves

| Mechanism | Selection point | What can be saved | What remains |
|---|---|---|---|
| `TRICE_OFF` | Compilation, at the configured file/project scope | Normal disabled Trice macro code and its argument evaluation | No later recovery of events that were not generated |
| Configured target ID routing | Output dispatch for a selected destination | Transfer/storage at that destination | Earlier acquisition and, in the inspected deferred path, encoding have already happened |
| `-pick` / `-ban` | Host, currently after message rendering | Subsequent normal presentation/output of excluded events | Target work, link traffic, and previous host decoding/rendering |
| `-logLevel` | Host presentation path in the reviewed implementation | Presentation selected by its current tag-order rules | Target work and acquisition; it is not a target controller |
| Proposed per-level compile-time selection | Not implemented as the draft extension | Would eliminate selected levels from a build | Requires a rebuild to restore them |
| Proposed dynamic target level control | Not implemented as the draft extension | Would potentially avoid acquisition and/or transmission work | Adds a policy, control mechanism, and target-side decisions |

The deferred timing statement applies to the inspected implementation, not to every user-defined direct-output callback. Routing is not automatically acquisition-time suppression. [part08-S2] [part08-S3] [part08-S4] [part08-S12] [part08-S5] [part08-S14]

#### <a id="part08-ll-target-off"></a>Compile-time control with TRICE_OFF

For normal Trice macro use in a source file, define `TRICE_OFF` before the first inclusion of `trice.h` in that translation unit:

```c
#define TRICE_OFF 1
#include "trice.h"
```

A project-wide compiler definition such as `-DTRICE_OFF=1` provides broader control. The build configuration and include order must agree. The Trice source tools can still see the log statements and dictionary information; compile-time elimination is not a request to delete historical entries from `til.json`. [part08-S1] [part08-S5]

Logging arguments should not be used for required program side effects. There is also an important wrapper distinction: disabling a Trice macro inside an ordinary function does not necessarily prevent the caller from evaluating that function's arguments. For example, `log_motor(read_sensor())` may still call `read_sensor()`. A future level-specific wrapper design must address that explicitly.

This broad compile-time control already exists. The additional per-level `trice_INFO`/`trice_DEBUG` mechanism described later is a separate, deferred proposal.

#### <a id="part08-ll-tags-aliases"></a>Tags, aliases, and user labels

A known tag is normally recognized at the beginning of a message before the first colon. Alias groups let spellings such as `err` and `ERROR` refer to the same category. Case can still affect presentation: known lower-case prefixes are normally removed by the appropriate color modes, while upper-case prefixes may remain visible. The `off` and `none` color options are therefore not identical. [part08-S3] [part08-S6]

**Known defect in the reviewed version:** exact spellings occur in different groups: `w`, `rx`, `tx`, `s`, and `S`. Some code paths return the first match; others continue to later matches. Until resolved, use unambiguous spellings such as `wrn`, `write`, `read`, `receive`, `transmit`, `signal`, and `Seconds`. The correction requires registry-wide tests, including future additions and user-label collisions; it is not simply a cosmetic table cleanup. [part08-S3]

The actual CLI spelling is **`-ulabel`**, not `-uLabel`. It declares additional labels; it does not inherently assign a meaningful severity to them. The documented syntax is:

```text
-ulabel motor:sensor -ulabel control
```

**Known limitations in the reviewed version:** the implementation does not split the colon-separated user-label list as documented, and pick/ban aliases are resolved before the newly declared labels are registered. Thus a custom-label selector in the same command is not reliable merely because `-ulabel` appears first. Use this syntax only with an implementation in which those issues are fixed, or verify the particular behavior before relying on it. [part08-S3] [part08-S4] [part08-S12] [part08-S6]

The intended processing order is: collect options; register and validate all labels; resolve selectors; reject invalid/conflicting options; then open inputs and outputs. That order is a requested correction, not a description of the current code.

#### <a id="part08-ll-pick-ban"></a>Explicit tag selection with pick and ban

`-pick` retains the named tag groups. `-ban` removes the named tag groups. Lists use colons and the switches can be repeated. Known aliases expand to their group. [part08-S4] [part08-S12]

```text
-pick wrn:err
-ban dbg
```

`-pick wrn:err` means those groups explicitly. It does **not** mean a severity threshold such as "warning and everything more severe"; other severe groups are not implicitly included.

In the reviewed filter implementation, an untagged event is suppressed by a nonempty pick selection and passes a ban selection. A nonmatching prefix likewise fails pick and passes ban. Neither switch removes target-side costs or transport bytes. [part08-S4] [part08-S12]

Do not combine `-pick` with `-ban`: this is unsupported. The current error is detected too late, in the per-message filter; it should instead be a startup validation error.

**Known validation defect:** unknown names are silently discarded. An entirely unknown pick selection can consequently become no filter at all. The required behavior is a command error naming the invalid token, after valid custom labels have been registered. A known selector for which no events arrive is valid; absence of matching data is not a spelling error. [part08-S4] [part08-S12]

#### <a id="part08-ll-host-level"></a>Host-side logLevel

The existing host option is spelled with an upper-case `L`:

```text
-logLevel all
-logLevel wrn
-logLevel off
```

`all` is the default. `off` suppresses normal output in the relevant presentation path; it does not disable target acquisition or stop a requested binary recording. Tool diagnostics and other consumers need an explicit policy rather than an assumption that this switch shuts down everything. [part08-S3] [part08-S6]

**Current limitation:** the implementation derives a threshold from the position of entries in the `Tags` display table. That table includes categories, unit tags, and appended user labels as well as conventional severity names. Level filtering also touches presentation fragments such as timestamps, locations, prefixes, and suffixes. It is not yet an independent, consistently applied event-severity model. [part08-S3] [part08-S6]

The preferred correction is to retain a useful threshold interface but define severity separately from presentation. The behavior of categories without severity, user labels, untagged events, and the interaction with explicit tag selection must be approved and documented. Adding a color entry must not change severity. No target level field or severity-ordered ID layout is required for a host-only correction.

Do not silently remove `-logLevel`. Removing it also breaks compatibility. If maintaining it is ultimately rejected, use an explicit deprecation and migration process. Unknown values should become startup errors, not accidental table-index behavior.

#### <a id="part08-ll-raw-recording"></a>Binary recording and later re-selection

`-binaryLogfile` and its alias `-blf` record the received stream before translation and display filtering. `-logfile`/`-lf` record text output and are not equivalent. An explicit binary filename is appended to if it already exists. [part08-S6] [part08-S7]

A replay example, assuming `session.bin` and its matching `til.json` already exist in the working directory, is:

```bash
tlog -p FILEBUFFER -args session.bin -idlist til.json -pick wrn:err -blf off
```

To replay the same received data without that tag selection:

```bash
tlog -p FILEBUFFER -args session.bin -idlist til.json -logLevel all -blf off
```

`tlog` uses the logging command path; `trice log` accepts the corresponding logging options. Use the matching encoding, framing, target settings, and optional location file when they differ from defaults. A replay does not reconstruct an event that was never received. [part08-S7] [part08-S8]

Archive the relevant dictionary and firmware/build identity with a recording. Where source positions matter, retain the appropriate location metadata too. Do not append replay output into its own input. A testable invariant is that identical received input produces identical binary captures regardless of host display selection.

#### <a id="part08-ll-id-allocation"></a>ID allocation policy and dictionary history

The existing insert and bind workflows expose `-IDMin`, `-IDMax`, `-IDMethod` (`random`, `upward`, or `downward`), and repeated tag-specific `-IDRange` rules. The common and specialized ranges must not overlap. These options allocate IDs; they do not configure an output device by themselves. [part08-S6] [part08-S9] [part08-S13]

For example, the following is a policy illustration for an application source directory named `app`; it writes the normal bind artifacts and must be adapted to the actual project:

```bash
trice bind -src ./app -IDMin 1000 -IDMax 9999 -IDRange err:10,99
```

The error range is 10 through 99 inclusive. All other eligible tags use the common range. Select a range within the actual supported ID space and archive the configuration with the build.

**Existing IDs are not automatically migrated.** The reuse logic may retain an ID outside a newly configured range. That is valuable for stability, but it makes an allocation rule different from a verified statement that all active errors currently occupy that range. `til.json` also contains historical entries; it must not be renumbered indiscriminately. [part08-S9] [part08-S13]

The proposed safeguard is an audit of active assignments against the current policy, with useful warnings and optionally strict CI checking. Historical-only entries need separate treatment, and a partial source scan cannot prove complete compliance. An optional repair should preview changes, update source or sidecars coherently, preserve old mappings, and require a rebuild. These audit/migration functions are proposals, not documented existing commands.

**Additional validation defect:** the reviewed `EvaluateIDRangeStrings` silently skips a rule with no colon. Malformed rules should fail before modifying source or generated artifacts. [part08-S9] [part08-S13]

#### <a id="part08-ll-target-routing"></a>Target ID routing and packing modes

A configured output can use an ID interval to forward only selected messages. Other outputs may still receive the same event. For example, a system may send a broad stream to one destination and only an error range to another. This is output routing, not necessarily suppression of the event's initial acquisition. [part08-S2] [part08-S5] [part08-S6]

The intended interval contract is inclusive. **Known defect:** the inspected deferred comparisons use strict `<` at both ends, despite inclusive allocation and the documented smallest/largest limits. Do not assume boundary events are routed correctly until this is fixed and tested. A workaround that widens limits must be reviewed again after an inclusive fix. [part08-S2] [part08-S5] [part08-S9] [part08-S13]

There is a second configuration defect: UARTA and UARTB use different tests for whether a range is enabled (`||` versus `&&` for nonzero bounds). Auxiliary/RTT configuration uses defined-macro tests. Missing/default/zero bounds require an explicit, consistent policy; changing one operator without defining that policy is insufficient. [part08-S2]

For the current active deferred per-ID routing branches, use:

```c
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
```

**The corresponding compile-time `#error` checks already exist.** Protect them with positive and negative build tests for each relevant output and partial-bound case. Do not add an unconditional error that rejects all multi-pack use. A valid multi-pack configuration without ID routing is a different case, and direct/custom routes require their own contract. [part08-S2]

The limitation is not intrinsic to TCOBS. A future implementation could decide on individual events before packing, but that would be a separate change requiring a demonstrated benefit.

#### <a id="part08-ll-integrity-and-performance"></a>Diagnostics, event boundaries, statistics, and host performance

An allowed event and its chosen metadata should share one acceptance decision. A timestamp's styling tag must not decide whether the timestamp of an accepted warning disappears. At the same time, existing partial-line composition must not be changed accidentally: several calls may form one line, and one call may span lines. Tests need to cover EOF flushing, `-addNL`, hidden fragments, and metadata attribution. [part08-S3] [part08-S4] [part08-S12]

The translator currently sends both formatted events and some error messages through the same tag filter. Unknown-ID and decode/integrity diagnostics therefore require a deliberate separate policy. They must not vanish merely because the application selection excludes their display tag. A future machine-readable output must keep diagnostics separate or encode them under a defined schema. [part08-S4] [part08-S12]

Statistics need explicit counting semantics: received data, decoded events, selected events, displayed events, and diagnostic occurrences are not the same thing. Coloring or splitting a message into display fragments must not invent extra application events.

Earlier host filtering is a useful optimization candidate because the present pick/ban stage follows message rendering. A decoder/dictionary-based decision could avoid unnecessary formatting. It must still maintain framing, cycle/integrity state, required decoding checks, raw capture, and approved timestamp-delta/statistics/visualization behavior. Measure CPU, allocations, and the unfiltered baseline before adding complexity. [part08-S4] [part08-S12] [part08-S11]

#### <a id="part08-ll-deferred-extensions"></a>Deferred target-level extensions

##### <a id="part08-ll-deferred-compile"></a>Per-level compile-time selection

The future-development draft proposes names such as `trice_INFO`, `trice_DEBUG`, and a corresponding build-time transformation. Such selection could remove selected event code and argument evaluation from a production build. It also adds macro families, tooling rules, and another build policy. [part08-S1]

**Decision of this review: defer implementation until an immediate, documented requirement exists.** Reopen it for a measured code-size/execution constraint or a required build policy that broad `TRICE_OFF` and existing controls do not address adequately. A later design must support the intended bind/insert workflows and eliminate argument evaluation when disabled.

The draft's proposed `trice insert -loglevel` is **not** the implemented host `-logLevel` switch. Capitalization and command context matter. Do not present the proposal as an available command.

##### <a id="part08-ll-deferred-runtime"></a>Dynamic target-side level control

A dynamic threshold could become useful for excessive event rates, limited transmission bandwidth, bounded offline storage, or costly event arguments. It would need a defined update mechanism, concurrency policy, and a precise decision point. A local application control can be sufficient; remote operation may require an existing or new command path. [part08-S1]

**Decision of this review: also defer this extension until a concrete requirement justifies it.** Low per-event overhead is a strong reason to prefer the current design, but not a proof that all workloads are cheap. Existing compile-configured routing is not the same as a dynamic early filter.

Neither target-level proposal is a prerequisite for context enrichment or structured host-side fields. No additional per-event level bytes are presumed necessary.

#### <a id="part08-ll-related-work"></a>Relationship to enrichment, structured logging, and server use

The reviewed UM section map is: 45.1 level-control draft; 45.2 the draft primarily about enrichment; 45.3 internal-parser development; and 45.4 server use. Prefer stable anchors when these sections move. The enrichment chapter should be renamed and the parser task should become a separate chapter/work item. [part08-S1]

A possible `strice` extension can retain one event ID for multiple named fields. The host can resolve a field across multiple IDs and argument positions, optionally translate it to a host-side numeric field ID, and produce text or structured output. Restricting the target to one key per message is not required and can break convenient context grouping. This remains a proposal, not an implemented public interface.

For server use, distinguish a server consuming device Trice streams from replacing a native server logger. The former fits the efficient target design naturally; the latter needs separate evidence and benchmarks. Do not assume that tokenization proves lower total energy use or eliminates further compression. First validate a practical workflow, then prepare a detailed extension specification.

#### <a id="part08-ll-summary"></a>Recommended order of work

Preserve the target core and current wire format. Correct alias ambiguity, CLI validation, custom-label handling, routing endpoints, and routing activation first. Protect existing packing guards and raw capture with tests. Clarify host severity, metadata, line composition, and diagnostic/statistics behavior before moving filtering earlier. Integrate this overview with the technical reference sections. Reconsider finer target selection only when a real requirement appears.

#### <a id="part08-ll-references"></a>Implementation and terminology references

Source links are to the reviewed branch, not an immutable commit. Recheck them and record the actual commit when implementing changes.

[part08-S1]: https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md
[part08-S2]: https://github.com/rokath/trice/blob/wip/src/trice.c
[part08-S3]: https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go
[part08-S4]: https://github.com/rokath/trice/blob/wip/internal/translator/translator.go
[part08-S5]: https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h
[part08-S6]: https://github.com/rokath/trice/blob/wip/internal/args/init.go
[part08-S7]: https://github.com/rokath/trice/blob/wip/internal/args/handler.go
[part08-S8]: https://github.com/rokath/trice/blob/wip/internal/receiver/receiver.go
[part08-S9]: https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go
[part08-S10]: https://opentelemetry.io/docs/specs/otel/logs/data-model/
[part08-S11]: https://github.com/rokath/trice/blob/wip/internal/decoder/decoder.go
[part08-S12]: https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go
[part08-S13]: https://github.com/rokath/trice/blob/wip/internal/id/insertIDs.go
[part08-S14]: https://github.com/rokath/trice/blob/wip/src/triceRingBuffer.c

---

**Part 9: L01 - Make tag aliases unambiguous and validate the complete tag registry**  
**Standalone source:** [issues/L01_unique_tag_aliases.md](issues/L01_unique_tag_aliases.md)

## Make tag aliases unambiguous and validate the complete tag registry

**Local reference:** L01  
**Type:** Bug / regression coverage  
**Suggested priority:** P1 - correctness  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed ambiguity; runtime reproduction still required.

### Summary

The same tag spelling currently identifies different groups. Alias resolution, level filtering, coloring, ID-range assignment, and statistics must agree on one interpretation.

### Background and evidence

`Tags` in `lineTransformerANSI.go` contains cross-group duplicates: `w` (Warning/Write), `rx` (Read/Receive), `tx` (Write/Transmit), and `s`/`S` (Signal/seconds). `FindTagName` and `tagVariants` return the first matching group. The level/counting loop continues through later matches, while color selection returns on the first match. Therefore one spelling can select one category but receive another effective threshold position and more than one counter increment.

This is not a request to collapse legitimate upper/lower-case variants. Tag spelling currently also affects whether the prefix is removed from text output.

### Reproduction or review scenario

Use the existing emitter test harness with `w:example`, `-pick wrn`, and `-logLevel wrn`. Check canonicalization, selection, effective level, color group, and counts separately. Enumerate every exact spelling in all groups to reproduce the duplicate registry entries without a device.

### Requested change

Choose and document an explicit owner for each conflicting spelling. Introduce one canonical registry/lookup policy and validation for built-in and user-added tags. Reject a spelling assigned to two groups instead of relying on table order. Handle a redundant repeated registration in the same group explicitly. Review source compatibility before removing an established short alias.

### Acceptance criteria and regression tests

- [ ] A table-wide test rejects cross-group duplicate spellings and automatically covers future entries.
- [ ] All known aliases resolve consistently across selectors, ID-range assignment, display, and statistics.
- [ ] Tests cover all five listed conflicting spellings, not only `w`.
- [ ] User labels cannot shadow a built-in alias or create another ambiguous alias.
- [ ] Lower/upper-case display behavior is retained or explicitly migrated; one event is not counted in two tag groups.
- [ ] Release notes identify changed aliases and recommend unambiguous alternatives.

### Compatibility and scope boundaries

No target encoding, ID-width, or TCOBS change. Alias reassignment is an observable compatibility change; do not silently reinterpret deployed tags.

### Source references

- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `Tags`, `FindTagName`, `tagVariants`, `colorize`, `AddUserLabels`
- [internal/id/switchIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go) - `newID` and tag normalization

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L05](issues/L05_ulabel_registration_order.md), [L07](issues/L07_reject_unknown_selectors.md), [L11](issues/L11_loglevel_semantics.md), [L16](issues/L16_statistics_semantics.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 10: L02 - Use inclusive ID-routing bounds consistently with ID allocation**  
**Standalone source:** [issues/L02_inclusive_routing_bounds.md](issues/L02_inclusive_routing_bounds.md)

## Use inclusive ID-routing bounds consistently with ID allocation

**Local reference:** L02  
**Type:** Bug  
**Suggested priority:** P1 - possible missing routed events  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed mismatch between allocation, comments, and routing comparisons.

### Summary

IDs allocated at the configured minimum or maximum must not disappear from an output whose documented range includes those endpoints.

### Background and evidence

`PreProcessing` enumerates candidate IDs using `id <= max`, starting at `min`. The configuration describes UART limits as the smallest/largest routed ID. In `TriceNonBlockingDeferredWrite8`, UARTA, UARTB, auxiliary outputs, and the guarded RTT path instead compare `min < triceID && triceID < max`.

For configured limits 10 and 99, the current comparisons forward only 11 through 98. A single-ID interval forwards nothing.

### Reproduction or review scenario

In a routing test, set a non-disabled range to 10..99 and send IDs 9, 10, 11, 98, 99, and 100. Capture the backend calls rather than depending on serial hardware. Repeat with a one-ID interval such as 42..42.

### Requested change

Use one documented inclusive interval convention for allocation and routing. Audit all ID-limited output paths, not only UARTA. Keep the separate meaning of disabled/default routing configuration intact; changing `<` to `<=` must not accidentally activate a disabled route.

### Acceptance criteria and regression tests

- [ ] The 10..99 route forwards 10 and 99, but not 9 or 100.
- [ ] An enabled 42..42 route forwards exactly ID 42.
- [ ] All enabled affected UART, auxiliary, and RTT paths use the same interval convention.
- [ ] Tests include legal global ID endpoints and no-range/default configuration.
- [ ] Allocation and routing tests share boundary fixtures, including upward/downward allocation endpoints.
- [ ] No additional bytes or changes to frame content are introduced.

### Compatibility and scope boundaries

Existing users may have widened limits as a workaround. Document the semantic correction and migration. Do not alter historical dictionary IDs as part of this fix.

### Source references

- [src/trice.c](https://github.com/rokath/trice/blob/wip/src/trice.c) - `TriceNonBlockingDeferredWrite8` and ID comparisons
- [src/triceDefaultConfig.h](https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h) - `TRICE_UARTA_MIN_ID`, `TRICE_UARTA_MAX_ID`, UARTB counterparts
- [internal/id/switchIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go) - `PreProcessing`

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L03](issues/L03_validate_active_id_policy.md), [L09](issues/L09_consistent_routing_configuration.md), [L10](issues/L10_single_pack_guard_tests.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 11: L03 - Validate active ID assignments against changed ID-range policies**  
**Standalone source:** [issues/L03_validate_active_id_policy.md](issues/L03_validate_active_id_policy.md)

## Validate active ID assignments against changed ID-range policies

**Local reference:** L03  
**Type:** Enhancement / correctness safeguard  
**Suggested priority:** P1 when target routing depends on ranges; otherwise P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed ID reuse behavior; validation is proposed, not existing functionality.

### Summary

Changing `-IDRange`, `-IDMin`, or `-IDMax` must not give a false impression that all existing log sites now follow the new allocation policy.

### Background and evidence

The insertion path preserves matching IDs and explicitly permits reusing an ID that may no longer belong to the current ID space. This protects stable decoding but means a new error-only range does not automatically contain every active error event. The shared insertion/allocator path also supplies assignments for bind.

`til.json` is historical as well as current. Scanning it alone cannot establish which entries are active. `li.json` may retain old locations too; active-use classification must consider the source set/current bind result.

### Reproduction or review scenario

Create an error log site with ID 250 and a matching dictionary entry. Introduce an error range 10..99 and a disjoint common range. Run insert and bind in separate fixtures. Include another old out-of-policy error entry that is not used in the current source set.

### Requested change

Report active policy violations with ID, tag/canonical group, location, expected range, and actual assignment. Distinguish active, historical-only, and unknown-activity entries. Warn without mutation by default; consider an explicit strict CI mode. Flag incomplete source coverage instead of claiming a complete audit. A repair, if offered, belongs to the separate migration workflow.

### Acceptance criteria and regression tests

- [ ] Existing active out-of-range assignments produce actionable diagnostics in both insert and bind workflows.
- [ ] Historical-only entries are not reported as mandatory renumbering errors.
- [ ] Aliases and user labels use the validated registry; default and tag-specific ranges are checked.
- [ ] Validation does not mutate source, sidecars, `til.json`, or `li.json`.
- [ ] Strict checking, if added, returns a failing status before a misleading successful build result.
- [ ] Incomplete source coverage and range exhaustion are distinguished from policy compliance.

### Compatibility and scope boundaries

Preserve dictionary history and stable IDs unless explicitly migrating. New CLI names are intentionally not specified in this issue.

### Source references

- [internal/id/insertIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/insertIDs.go) - `insertTriceIDsVisit`, existing-ID reuse and bind visit hook
- [internal/id/switchIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go) - `PreProcessing`, `EvaluateIDRangeStrings`
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - insert and bind configuration flow

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L01](issues/L01_unique_tag_aliases.md), [L02](issues/L02_inclusive_routing_bounds.md), [L04](issues/L04_safe_id_policy_migration.md), [L27](issues/L27_reject_malformed_idrange.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 12: L04 - Provide an explicit, non-destructive migration for active ID-range violations**  
**Standalone source:** [issues/L04_safe_id_policy_migration.md](issues/L04_safe_id_policy_migration.md)

## Provide an explicit, non-destructive migration for active ID-range violations

**Local reference:** L04  
**Type:** Optional enhancement  
**Suggested priority:** P2 - after validation; implement only if migration is needed  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Proposed follow-up; not a claim that stable ID reuse is itself a bug.

### Summary

Offer a safe repair path for intentionally moving current log sites into new ID ranges without invalidating older firmware logs.

### Background and evidence

An allocation policy checker can identify active out-of-range sites, but changing numbers in `til.json` alone would desynchronize firmware, source or sidecars, and the dictionary. Historical mappings must remain available for recordings and older firmware. Different projects may share a dictionary while using independent build directories.

### Reproduction or review scenario

Use a project with one active violation, one compliant site, a historical dictionary-only entry, and a second project sharing the dictionary. Simulate a failed write and an exhausted destination range during a trial migration.

### Requested change

Provide an explicit preview of old/new assignments and affected artifacts. Migrate only selected active sites; allocate unused IDs under the chosen policy; update source or bind sidecars as applicable; preserve old mappings. Protect shared state against concurrent writers. Make failure recovery and rebuild requirements clear. Keep the operation opt-in and separate from ordinary validation.

### Acceptance criteria and regression tests

- [ ] A preview is available without file changes and lists every affected assignment/artifact.
- [ ] No existing historical mapping is repurposed for a different event.
- [ ] Source/sidecar assignments and generated metadata stay coherent after migration.
- [ ] Unrelated projects and historical-only entries are not silently renumbered.
- [ ] Exhaustion, conflicts, write failures, and concurrent updates fail safely without a falsely successful mixed state.
- [ ] Rerunning a completed migration is idempotent; old and new firmware fixtures remain decodable with their appropriate metadata.

### Compatibility and scope boundaries

This is not permission to rewrite an entire dictionary or require migration for host-only filtering. Choose the actual CLI after design review.

### Source references

- [internal/id/insertIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/insertIDs.go) - assignment reuse and source writes
- [internal/id/switchIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go) - allocation state and post-processing
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - insert/bind entry points

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L03](issues/L03_validate_active_id_policy.md), [L27](issues/L27_reject_malformed_idrange.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 13: L05 - Register all user labels before resolving pick and ban selectors**  
**Standalone source:** [issues/L05_ulabel_registration_order.md](issues/L05_ulabel_registration_order.md)

## Register all user labels before resolving pick and ban selectors

**Local reference:** L05  
**Type:** Bug  
**Suggested priority:** P1 - unreliable CLI selection  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed initialization order.

### Summary

`-ulabel motor -pick motor` must work regardless of the order of these CLI options.

### Background and evidence

`channelArrayFlag.Set` resolves aliases during `flag.Parse`. `runLog` invokes `AddUserLabels` only after parsing. Thus a label introduced in the same command is not present when the selector is resolved; putting `-ulabel` first does not solve it because registration is still deferred.

`AddUserLabels` also has a process-wide once guard. Changes must account for repeated command/test invocations rather than leaving stale global registry state.

### Reproduction or review scenario

Exercise both `tlog -ulabel motor -pick motor` and `tlog -pick motor -ulabel motor` through the argument parser with a controlled input fixture. Repeat with `-ban`, multiple labels, and `trice log` as the entry point. Use a fresh registry for each case.

### Requested change

Collect raw option values first, register and validate all user labels second, and resolve/validate selectors third, before opening an input stream or creating output files. Define registry lifecycle explicitly for tests and in-process command reuse.

### Acceptance criteria and regression tests

- [ ] Both argument orders resolve the same user label correctly for pick and ban.
- [ ] `tlog` and `trice log` share the corrected behavior.
- [ ] Registry initialization does not leak labels or selectors between independent invocations.
- [ ] Unknown labels still cause a useful error after registration is complete.
- [ ] Built-in alias collision checks run for user labels as well.
- [ ] Insert/bind ID-range handling continues to recognize valid user tags.

### Compatibility and scope boundaries

No target change. Avoid a fix that merely relies on option ordering or weakens unknown-selector validation.

### Source references

- [internal/emitter/emitter.go](https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go) - `channelArrayFlag.Set`
- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `AddUserLabels`, `userLabelsAdded`
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - `runLog`, insert/bind flow

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L01](issues/L01_unique_tag_aliases.md), [L06](issues/L06_ulabel_colon_lists.md), [L07](issues/L07_reject_unknown_selectors.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 14: L06 - Parse colon-separated user-label lists as documented**  
**Standalone source:** [issues/L06_ulabel_colon_lists.md](issues/L06_ulabel_colon_lists.md)

## Parse colon-separated user-label lists as documented

**Local reference:** L06  
**Type:** Bug  
**Suggested priority:** P2 - CLI correctness  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed mismatch with CLI help.

### Summary

The documented `-ulabel motor:sensor -ulabel control` form must register three distinct labels, not a single literal `motor:sensor` name.

### Background and evidence

The help for `flagUserLabel` promises repeated flags containing colon-separated lists. `ArrayFlag.Set` appends each value unchanged and `AddUserLabels` creates one tag for each stored value without splitting it. A label containing the separator cannot match the normal prefix extraction as intended.

### Reproduction or review scenario

Parse `-ulabel motor:sensor -ulabel control`, register the labels, and inspect canonical lookups for `motor`, `sensor`, `control`, and the accidental literal `motor:sensor`. Also test `-ulabel motor:`, `-ulabel :motor`, and repeated names.

### Requested change

Implement the documented list syntax in label-specific parsing or registration. Validate each resulting name and define empty-element and duplicate handling. Do not modify a generic flag type used by unrelated features without reviewing those users.

### Acceptance criteria and regression tests

- [ ] Three intended labels are registered from the example.
- [ ] Repeated individual flags and one colon-separated list are equivalent.
- [ ] Empty elements and invalid names have documented, deterministic handling; preferably reject malformed lists.
- [ ] Duplicate same-label input is either idempotent or clearly diagnosed, never added as another ambiguous group.
- [ ] Resolved labels can be used with pick, ban, and applicable ID-range options.
- [ ] CLI help and tests use the exact lower-case switch name `-ulabel`.

### Compatibility and scope boundaries

Keep legitimate user spellings and case behavior. Combine with post-parse registration/resolution but retain a separate regression test for list syntax.

### Source references

- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - `flagUserLabel`
- [internal/emitter/emitter.go](https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go) - `ArrayFlag.Set`
- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `AddUserLabels`

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L01](issues/L01_unique_tag_aliases.md), [L05](issues/L05_ulabel_registration_order.md), [L07](issues/L07_reject_unknown_selectors.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 15: L07 - Reject unknown or malformed filter selectors before starting logging**  
**Standalone source:** [issues/L07_reject_unknown_selectors.md](issues/L07_reject_unknown_selectors.md)

## Reject unknown or malformed filter selectors before starting logging

**Local reference:** L07  
**Type:** Bug / validation  
**Suggested priority:** P1 - prevent silent selection changes  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed silent dropping for pick/ban; logLevel also lacks value validation.

### Summary

A mistyped selector must fail clearly instead of silently changing the requested selection.

### Background and evidence

`channelArrayFlag.Set` expands only recognized aliases and returns no error for an unknown name. If every `-pick` item is unknown, the internal selection can remain nil and be treated as no filter at all. An unknown item mixed into a valid list is silently lost. `-logLevel` is accepted as an arbitrary string and later falls through table-index logic; the help even describes an invalid-value side effect rather than an error.

### Reproduction or review scenario

Test `-pick erorr`, `-ban erorr`, `-pick err:erorr`, empty elements such as `-pick err:`, and `-logLevel erorr`. Include a valid user label declared elsewhere in the same command. Run parser tests without a connected target.

### Requested change

Validate all supplied selector tokens after the complete registry is built. For logLevel, preserve explicitly supported control values such as `all` and `off`; resolve other supported values under the chosen compatibility policy. Return nonzero with the offending token and useful valid alternatives. Similar-name suggestions may help, but must not auto-correct.

### Acceptance criteria and regression tests

- [ ] Unknown pick/ban names fail rather than becoming no filter or a partial filter.
- [ ] Unknown logLevel values fail without starting acquisition.
- [ ] Empty tokens and malformed lists are consistently rejected or explicitly documented.
- [ ] Valid custom labels are accepted independent of argument order.
- [ ] A known selector with zero matching events is not considered invalid.
- [ ] Failure occurs before opening ports, creating/appending log files, or entering the read loop.

### Compatibility and scope boundaries

Rejecting typos intentionally changes erroneous commands. Do not silently remove support for legacy valid logLevel categories in the same patch; coordinate with the severity migration.

### Source references

- [internal/emitter/emitter.go](https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go) - `channelArrayFlag.Set`, `banOrPickFilter`
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - `logLevel` registration and help
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - `runLog`

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L05](issues/L05_ulabel_registration_order.md), [L06](issues/L06_ulabel_colon_lists.md), [L08](issues/L08_reject_conflicting_filters_early.md), [L11](issues/L11_loglevel_semantics.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 16: L08 - Reject simultaneous pick and ban options during startup validation**  
**Standalone source:** [issues/L08_reject_conflicting_filters_early.md](issues/L08_reject_conflicting_filters_early.md)

## Reject simultaneous pick and ban options during startup validation

**Local reference:** L08  
**Type:** Bug / error handling  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed late fatal check.

### Summary

The unsupported combination of `-pick` and `-ban` must be rejected before processing any input, including when the input is empty or a selector is invalid.

### Background and evidence

The mutual-exclusion check currently runs in `banOrPickFilter`, after decoding has reached that path. A command with no events may never reach the check. A mistyped selector can also disappear before the conflict is detected. The deep filter path uses fatal process termination rather than a startup configuration error.

### Reproduction or review scenario

Invoke both switches with a valid pair, with an unknown token, and with an empty input. Assert that no receiver or output sink is opened and no process termination occurs inside a per-event unit test.

### Requested change

Track whether each option was supplied and validate mutual exclusion during CLI configuration finalization. Return a normal command error at that boundary. Keep the intended prohibition rather than inventing combined semantics in a bug fix.

### Acceptance criteria and regression tests

- [ ] Both valid options always fail before acquisition, even with zero input events.
- [ ] Unknown selectors do not conceal the conflict; diagnostics remain actionable.
- [ ] Unit tests do not need to trigger a deep `os.Exit` to verify the policy.
- [ ] Either option alone still works; repeated occurrences of the same option remain supported.
- [ ] Both public logging entry points behave identically.

### Compatibility and scope boundaries

No target or wire-format change. Supporting pick plus ban together would be a separate interface decision.

### Source references

- [internal/emitter/emitter.go](https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go) - `banOrPickFilter`
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - `runLog` and receiver setup

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L07](issues/L07_reject_unknown_selectors.md), [L17](issues/L17_early_host_filtering.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 17: L09 - Make ID-routing activation and partial-bound handling consistent across outputs**  
**Standalone source:** [issues/L09_consistent_routing_configuration.md](issues/L09_consistent_routing_configuration.md)

## Make ID-routing activation and partial-bound handling consistent across outputs

**Local reference:** L09  
**Type:** Bug / configuration validation  
**Suggested priority:** P1 - potentially unfiltered or missing output  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed UARTA/UARTB difference; other partial-bound cases require coverage.

### Summary

Equivalent routing configurations must not enable a range on one output while disabling it on another.

### Background and evidence

UARTA activates its range branch when either configured bound is nonzero (`||`). UARTB activates only when both are nonzero (`&&`). Auxiliary and RTT routes use a different defined-macro check. With a partially configured UART range, output can therefore be restricted or unrestricted depending on the backend. The multi-pack rejection is inside these activation branches, so activation also affects validation coverage.

### Reproduction or review scenario

Build a configuration matrix for each supported backend: no bounds/defaults; only lower; only upper; both; reversed interval; one-ID interval. Exercise both single-pack and multi-pack settings. Record whether the route activates and which IDs reach it.

### Requested change

Define one clear configuration policy, including the meaning of zero/default/undefined bounds. Reject ambiguous partial configurations or implement a documented open-ended convention consistently. Preserve the intended no-routing default. Validate active bounds against the configured ID format and reject reversed intervals. Do not blindly replace every `&&` with `||` without defining the contract.

### Acceptance criteria and regression tests

- [ ] All backends agree on equivalent complete, absent, and partial configurations.
- [ ] Invalid/ambiguous configurations fail at compile time with a useful message.
- [ ] Zero/default sentinel behavior is explicitly documented and tested.
- [ ] Range activation cannot accidentally bypass a multi-pack incompatibility check.
- [ ] Inclusive endpoint tests remain valid after normalization.
- [ ] Disabled outputs are not forced to satisfy irrelevant routing configuration unless deliberately documented.

### Compatibility and scope boundaries

Configuration changes can affect output contents. Treat existing partial-bound use as a migration case, not an unnoticed cleanup.

### Source references

- [src/trice.c](https://github.com/rokath/trice/blob/wip/src/trice.c) - `TriceNonBlockingDeferredWrite8` preprocessor guards
- [src/triceDefaultConfig.h](https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h) - UART bound defaults

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L02](issues/L02_inclusive_routing_bounds.md), [L10](issues/L10_single_pack_guard_tests.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 18: L10 - Regression-test the existing single-pack requirement for deferred ID routing**  
**Standalone source:** [issues/L10_single_pack_guard_tests.md](issues/L10_single_pack_guard_tests.md)

## Regression-test the existing single-pack requirement for deferred ID routing

**Local reference:** L10  
**Type:** Test hardening / documentation  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** The relevant compile-time `#error` checks already exist; this issue does not claim they are missing.

### Summary

Protect the existing incompatibility checks and explain their scope: current deferred per-ID routing requires individually handled events, not arbitrary mixed-event multi-pack output.

### Background and evidence

`src/trice.c` already rejects `TRICE_MULTI_PACK_MODE` in enabled ID-range branches for UARTA, UARTB, auxiliary outputs, and the inspected RTT path. The exact reachability of a guard depends on route activation. This is not a general restriction on TCOBS, host filtering, or every possible custom direct-output implementation.

### Reproduction or review scenario

Compile minimal fixtures for each relevant enabled output with a valid active range: single-pack should compile; multi-pack should fail with the routing diagnostic. Compile multi-pack without ID routing as a separate supported baseline where otherwise valid.

### Requested change

Add positive and negative compilation tests around the real configuration checks. Audit custom/direct routing separately and document its own contract rather than applying the deferred rule indiscriminately. Keep failure tests focused so that an unrelated missing backend stub does not masquerade as the intended rejection.

### Acceptance criteria and regression tests

- [ ] Every relevant active deferred route rejects multi-pack for the intended reason.
- [ ] Each corresponding valid single-pack fixture compiles.
- [ ] No-routing multi-pack fixtures retain their supported behavior.
- [ ] Partial-bound cases follow the policy from L09 and cannot bypass validation accidentally.
- [ ] No redundant unconditional `#error` is added that forbids unrelated valid configurations.
- [ ] Documentation states that future pre-pack filtering could remove this limitation without changing TCOBS itself, but is not implemented here.

### Compatibility and scope boundaries

Test and document the existing design. Do not implement multi-pack ID routing or a new target-level controller in this issue.

### Source references

- [src/trice.c](https://github.com/rokath/trice/blob/wip/src/trice.c) - existing `TRICE_MULTI_PACK_MODE` routing errors
- [src/triceDefaultConfig.h](https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h) - routing configuration defaults

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L02](issues/L02_inclusive_routing_bounds.md), [L09](issues/L09_consistent_routing_configuration.md), [L18](issues/L18_central_manual_chapter.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 19: L11 - Decouple logLevel semantics from tag-table position and color formatting**  
**Standalone source:** [issues/L11_loglevel_semantics.md](issues/L11_loglevel_semantics.md)

## Decouple logLevel semantics from tag-table position and color formatting

**Local reference:** L11  
**Type:** Behavioral correction / compatibility design  
**Suggested priority:** P2 - define policy before implementation  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed coupling; the replacement severity contract is a proposal.

### Summary

Keep the useful host threshold interface, but stop treating an incidental display-table order as the definition of event severity.

### Background and evidence

`lineTransformerANSI.colorize` derives both event level and threshold from indices in `Tags`. The same table also contains categories, measurement units, and custom labels appended at runtime. Thus reordering display entries or adding labels can affect filtering. There is no shared explicit severity metadata. Unknown tagged text defaults differently from untagged text.

The decision from the review is to prefer retaining `-logLevel`, not silently deleting it. Lack of usage evidence is not evidence that nobody depends on it.

### Reproduction or review scenario

Record legacy results for all built-in tags, aliases, untagged text, unknown prefixes, and user labels under `all`, `off`, and representative thresholds. Then reorder only display metadata in a test: event severity must not change in the corrected model.

### Requested change

Specify severity separately from category and presentation. Define which tags have severity, what happens to tags without severity, and how `-ulabel` participates. A safe candidate is to preserve unclassified events unless explicitly selected out, but approve the policy before coding. Define the interaction of an explicit tag filter and a severity threshold. Keep numeric mappings internal or translate them explicitly at export boundaries; do not infer one universal severity numbering.

### Acceptance criteria and regression tests

- [ ] An explicit mapping or equivalent stable contract replaces table-position semantics.
- [ ] `all`, `off`, no-severity categories, user labels, and untagged events have documented behavior.
- [ ] Adding/reordering a color entry cannot change severity.
- [ ] Event tags do not themselves call exit, panic, or a target reset; classification and control flow remain separate.
- [ ] Legacy-versus-new behavior is covered by a comparison matrix and migration/release notes.
- [ ] Any deprecation/removal requires a separate explicit compatibility decision, not an incidental refactor.

### Compatibility and scope boundaries

Host-only semantic work. Do not add level bytes, force severity-ordered IDs, or introduce level-specific target macros. Preserve valid legacy commands or provide a deliberate transition.

### Source references

- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `Tags`, `colorize`, `Colorize`, `WriteLine`
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - `-logLevel` and `-ulabel` help

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L01](issues/L01_unique_tag_aliases.md), [L07](issues/L07_reject_unknown_selectors.md), [L12](issues/L12_event_metadata_filtering.md), [L13](issues/L13_fragment_and_multiline_policy.md), [L16](issues/L16_statistics_semantics.md), [L18](issues/L18_central_manual_chapter.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 20: L12 - Keep an accepted event and its display metadata under one filter decision**  
**Standalone source:** [issues/L12_event_metadata_filtering.md](issues/L12_event_metadata_filtering.md)

## Keep an accepted event and its display metadata under one filter decision

**Local reference:** L12  
**Type:** Bug / output consistency  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed fragment-wise level filtering; intended event-level behavior must be tested.

### Summary

An accepted warning should not lose its timestamp, source location, or ID merely because those display fragments use different presentation tags.

### Background and evidence

Pick/ban filtering is already applied before location/timestamp/ID strings are inserted. Level filtering happens later inside the ANSI line transformer for individual strings, including presentation fragments. CLI help explicitly warns that `-logLevel` also influences these metadata formats, prefix, and suffix. This mixes event selection with decoration.

### Reproduction or review scenario

Render an accepted warning with location, target timestamp, host timestamp, displayed ID, prefix, and suffix enabled. Give the metadata ordinary display tags and compare `-logLevel all` with a warning threshold. Repeat for `color=off`, `none`, and default.

### Requested change

Base acceptance on the event's classification, not on the tags chosen to style its metadata. Once accepted, format that event and its configured context as a coherent unit. Keep explicit metadata-hiding options effective. Specify prefix/suffix behavior in conjunction with the separate fragment/line policy.

### Acceptance criteria and regression tests

- [ ] Allowed events retain all explicitly enabled metadata regardless of its styling tag.
- [ ] Suppressed events do not leave orphan timestamps, source locations, IDs, prefixes, or blank decorated lines.
- [ ] Changing color mode does not change event selection.
- [ ] Pick/ban and logLevel can use the same semantic decision without duplicating decoration-specific rules.
- [ ] Tests cover local and remote display paths where supported.
- [ ] No unreviewed change to multi-event line grouping is bundled into this fix.

### Compatibility and scope boundaries

Some users may depend on old decoration filtering. Document the change and retain explicit options to hide each metadata column.

### Source references

- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `colorize`, `WriteLine`
- [internal/translator/translator.go](https://github.com/rokath/trice/blob/wip/internal/translator/translator.go) - filtering before metadata emission
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - logLevel warning about metadata

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L11](issues/L11_loglevel_semantics.md), [L13](issues/L13_fragment_and_multiline_policy.md), [L14](issues/L14_diagnostic_filter_isolation.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 21: L13 - Define and test filtering for partial lines and multi-line events**  
**Standalone source:** [issues/L13_fragment_and_multiline_policy.md](issues/L13_fragment_and_multiline_policy.md)

## Define and test filtering for partial lines and multi-line events

**Local reference:** L13  
**Type:** Compatibility clarification / regression coverage  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Existing source documents line-dependent behavior and unfinished `-addNL` handling; no complete runtime matrix was executed.

### Summary

Filtering must remain predictable when multiple Trice calls form one displayed line or one event contains multiple newlines.

### Background and evidence

`WriteLine` reasons about fragments and treats the last relevant fragment specially. Its comment discusses a `msg:Hello` fragment followed by an `att:World\n` fragment, and notes further tuning for `-addNL`. An event and a displayed line are therefore not interchangeable in the existing console path.

### Reproduction or review scenario

Create fixtures for: two differently tagged partial events that form one line; a single multi-line event; an event without final newline at EOF; a suppressed fragment between two accepted fragments; and a newline-only fragment. Run them with and without `-addNL`, metadata, and thresholds.

### Requested change

Document the legacy behavior before choosing a corrected event/line policy. Preserve accepted partial-line workflows where practical. If event-based filtering changes composition, specify the transition and metadata attribution explicitly. A future structured output should represent one Trice call as one event even when text contains newlines; do not retroactively treat every physical line as an event.

### Acceptance criteria and regression tests

- [ ] A golden-test matrix covers all listed fragment and multi-line cases.
- [ ] No orphan metadata, accidental concatenation across unrelated events, or unbounded buffered fragment is introduced.
- [ ] EOF flushing and `-addNL` are deterministic.
- [ ] The origin used for the displayed timestamp/location/ID of a composed line is documented.
- [ ] Any change from legacy selection is deliberate and included in release notes.
- [ ] Structured-event boundaries remain independent of console newline layout in future integrations.

### Compatibility and scope boundaries

Do not claim that a console line equals one event in all existing Trice usage. This is a semantic prerequisite for moving filtering earlier.

### Source references

- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `WriteLine` comments and last-fragment test
- [internal/translator/translator.go](https://github.com/rokath/trice/blob/wip/internal/translator/translator.go) - line state and EOF flush

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L11](issues/L11_loglevel_semantics.md), [L12](issues/L12_event_metadata_filtering.md), [L17](issues/L17_early_host_filtering.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 22: L14 - Prevent event filters from silently hiding decoder and tool diagnostics**  
**Standalone source:** [issues/L14_diagnostic_filter_isolation.md](issues/L14_diagnostic_filter_isolation.md)

## Prevent event filters from silently hiding decoder and tool diagnostics

**Local reference:** L14  
**Type:** Reliability hardening  
**Suggested priority:** P1 - diagnosis must remain possible  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed shared input path; individual lost-diagnostic cases still require reproduction.

### Summary

Application event selection must not accidentally conceal unknown-ID, malformed-frame, or transport-integrity diagnostics.

### Background and evidence

The translator explicitly states that the decoded buffer can contain either one formatted Trice message or one error message, then passes that buffer through `BanOrPickFilter`. Filtering diagnostics as though they were ordinary application events can hide the reason output is missing. This is a risk established by the shared path, not a claim that every diagnostic is currently suppressed.

### Reproduction or review scenario

Combine valid events with unknown IDs, malformed/truncated frames, and supported integrity/cycle diagnostics. Apply restrictive pick/ban/level settings. Check both console and recording/output channels and count which diagnostics remain visible.

### Requested change

Distinguish decoder/tool diagnostics from application events before applying application selection. Establish a dedicated diagnostic policy or channel; explicitly requested quiet behavior must be separate from accidental suppression. Keep banners, progress output, ANSI formatting, and diagnostics from contaminating any future machine-readable data stream.

### Acceptance criteria and regression tests

- [ ] Diagnostic fixtures remain observable under restrictive application filters unless an explicit diagnostic policy suppresses them.
- [ ] A malformed frame cannot be misrepresented as a normal filtered event or a zero-valued decoded event.
- [ ] Fatal/startup errors retain nonzero status regardless of display selection.
- [ ] No mixed plain-text warning is inserted into an output that promises valid machine-readable records.
- [ ] The separation does not change the bytes of the raw received-data capture.
- [ ] Existing diagnostic destinations and any migration are documented.

### Compatibility and scope boundaries

No new wire protocol. Structured-output requirements are safeguards for future output, not a claim that a general JSON exporter already exists.

### Source references

- [internal/translator/translator.go](https://github.com/rokath/trice/blob/wip/internal/translator/translator.go) - buffer comment and `BanOrPickFilter` call
- [internal/emitter/emitter.go](https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go) - tag-based selection
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - raw capture wrapper

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L07](issues/L07_reject_unknown_selectors.md), [L12](issues/L12_event_metadata_filtering.md), [L15](issues/L15_raw_capture_replay_guarantee.md), [L17](issues/L17_early_host_filtering.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 23: L15 - Protect unfiltered binary capture and replay with integration tests**  
**Standalone source:** [issues/L15_raw_capture_replay_guarantee.md](issues/L15_raw_capture_replay_guarantee.md)

## Protect unfiltered binary capture and replay with integration tests

**Local reference:** L15  
**Type:** Regression protection / documentation  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Existing placement of the binary logger is source-confirmed; byte-exact regression tests are requested.

### Summary

Keep the ability to hide events now and recover them later from a raw recording, independent of host display filters.

### Background and evidence

`logLoop` wraps the receiver in `NewBinaryLogger` before calling the translator. This is distinct from `-logfile`/`-lf`, which records output text. The architectural advantage depends on not moving binary capture behind selection or reusing a filtered record stream as the supposed raw capture.

### Reproduction or review scenario

Feed identical recorded input into separate runs with all output, restrictive pick, ban, and level settings. Use fresh capture destinations, because explicit binary filenames are append targets. Compare captured bytes and replay with the matching dictionary and format configuration.

### Requested change

Add an integration invariant: display selection does not alter the received bytes recorded by `-blf`/`-binaryLogfile`. Document append behavior, the difference from a text log, and the need to archive matching dictionary/build/encoding information. Preserve visibility of capture write failures.

### Acceptance criteria and regression tests

- [ ] For identical received input, binary captures are byte-identical under all supported host filters.
- [ ] Replay can reveal an event that was hidden during the original display.
- [ ] The test does not append replay output back into its own input file.
- [ ] Text logging is documented as output capture, not a substitute for raw replay.
- [ ] Missing/unmatched dictionaries and truncated recordings produce diagnostics rather than invented values.
- [ ] The guarantee is limited to bytes actually received; target-side suppression, source loss, and filtered-out target routes cannot be recovered.

### Compatibility and scope boundaries

This protects an existing architectural property. It does not request storing secrets or extra metadata in every target event.

### Source references

- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - `NewBinaryLogger` before `translator.Translate`
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - binary and text logfile flags
- [internal/receiver/receiver.go](https://github.com/rokath/trice/blob/wip/internal/receiver/receiver.go) - file-based input

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L14](issues/L14_diagnostic_filter_isolation.md), [L17](issues/L17_early_host_filtering.md), [L18](issues/L18_central_manual_chapter.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 24: L16 - Define counting semantics independently of coloring and filtering**  
**Standalone source:** [issues/L16_statistics_semantics.md](issues/L16_statistics_semantics.md)

## Define counting semantics independently of coloring and filtering

**Local reference:** L16  
**Type:** Correctness clarification / regression coverage  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed counting in presentation/tag loops; intended accounting categories need agreement.

### Summary

Statistics should say what they count and must not change merely because metadata, color handling, or filter placement changes.

### Background and evidence

Tag counts are updated in the display transformer, whose input includes fragments and presentation tags. Duplicate aliases can increment multiple groups. Moving selection before formatting can also change counts unless received, decoded, selected, displayed, and diagnosed events are distinguished deliberately.

### Reproduction or review scenario

Use a known sequence of events with mixed tags, hidden events, metadata fragments, multi-line content, and a decoder diagnostic. Compare statistics across color modes, metadata layouts, and filter settings.

### Requested change

Document the meaning of existing counters and select stable update points. Separate received/decoded counts from displayed-event counts only where useful; do not relabel one as another. Avoid counting display fragments as independent application events. Keep transport-loss, application suppression, and decode-error counts conceptually separate.

### Acceptance criteria and regression tests

- [ ] One decoded application event contributes at most once to its canonical tag counter.
- [ ] Changing color or metadata formatting does not change application-event counts.
- [ ] Hidden-event treatment is explicit and covered by tests.
- [ ] Multi-line and partial-line examples follow the documented event/line model.
- [ ] New earlier filtering does not silently change existing statistics.
- [ ] Any renamed or redefined counters include migration notes.

### Compatibility and scope boundaries

No need for new target counters or bytes. Do not claim a missing-event count can identify undetectable losses.

### Source references

- [internal/emitter/lineTransformerANSI.go](https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go) - `Tags.count`, `TagEvents`, `PrintTagStatistics`, `colorize`
- [internal/translator/translator.go](https://github.com/rokath/trice/blob/wip/internal/translator/translator.go) - filter and output sequencing

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L01](issues/L01_unique_tag_aliases.md), [L11](issues/L11_loglevel_semantics.md), [L13](issues/L13_fragment_and_multiline_policy.md), [L17](issues/L17_early_host_filtering.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 25: L17 - Evaluate host selection before expensive message rendering where safe**  
**Standalone source:** [issues/L17_early_host_filtering.md](issues/L17_early_host_filtering.md)

## Evaluate host selection before expensive message rendering where safe

**Local reference:** L17  
**Type:** Optimization  
**Suggested priority:** P3 - after semantic fixes and measurement  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed late rendering/filter sequence; performance gain is not measured.

### Summary

Avoid rendering messages that will be discarded by host filters, without changing the wire format or compromising decoding state.

### Background and evidence

`dec.Read` currently returns a formatted message (or a diagnostic), and the translator then applies pick/ban. For ID-based messages, tag/level information can in principle be resolved from the dictionary once rather than extracted from each rendered string. The current optional visualization path already exposes some typed numeric data separately, but it is not a complete general event API.

### Reproduction or review scenario

Benchmark replay of a representative capture at 0%, 50%, 90%, and 100% suppression. Include numeric messages, supported dynamic-string forms, dictionary changes, diagnostics, visualization, and multi-pack input. Compare event/output results as well as CPU and allocations.

### Requested change

Cache selection metadata per dictionary entry if appropriate. Skip only work that is provably unnecessary: framing, length validation, integrity checks, protocol state, and required raw capture still run. Preserve the current or explicitly approved policies for visualization, display deltas, diagnostics, fragments, and statistics. Keep non-ID encodings such as CHAR/DUMP on appropriate paths.

### Acceptance criteria and regression tests

- [ ] Measured CPU/allocation benefit and benchmark inputs are reported; no unsubstantiated speed factor is claimed.
- [ ] Accepted output remains equivalent under the approved semantics.
- [ ] Cycle/timestamp/framing state remains correct across hidden events.
- [ ] Display-delta behavior remains consistent with its documented reference to previously displayed timestamps.
- [ ] Dictionary reload/replacement invalidates cached selection metadata correctly.
- [ ] Raw capture, diagnostics, and supported visualization behavior are preserved.
- [ ] The unfiltered path does not suffer an unexplained material regression.

### Compatibility and scope boundaries

No early target suppression and no TCOBS redesign. A large decoder rewrite is not automatically authorized; a narrow event boundary may be sufficient.

### Source references

- [internal/translator/translator.go](https://github.com/rokath/trice/blob/wip/internal/translator/translator.go) - `dec.Read`, pick/ban, visualization, timestamp state
- [internal/decoder/decoder.go](https://github.com/rokath/trice/blob/wip/internal/decoder/decoder.go) - `VisRecord`, `VisValue`, `VisRecordProvider`
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - dictionary and raw capture setup

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L11](issues/L11_loglevel_semantics.md), [L12](issues/L12_event_metadata_filtering.md), [L13](issues/L13_fragment_and_multiline_policy.md), [L14](issues/L14_diagnostic_filter_isolation.md), [L15](issues/L15_raw_capture_replay_guarantee.md), [L16](issues/L16_statistics_semantics.md), [L24](issues/L24_separate_parser_chapter.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 26: L18 - Add a central manual chapter for log levels, tags, filtering, and ID routing**  
**Standalone source:** [issues/L18_central_manual_chapter.md](issues/L18_central_manual_chapter.md)

## Add a central manual chapter for log levels, tags, filtering, and ID routing

**Local reference:** L18  
**Type:** Documentation  
**Suggested priority:** P2 - after behavior is checked  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Proposed chapter; English and German drafts are supplied in the handover.

### Summary

Provide one reliable entry point that explains existing selection mechanisms, their costs, limitations, and the two deferred target-level proposals.

### Background and evidence

Relevant information is distributed over CLI help, target configuration, ID management, and UM future-development sections. The reviewed section map is 45.1 level-control draft, 45.2 the misnamed enrichment draft, 45.3 parser work, and 45.4 server use. Section numbers can change and should not be hard-coded into the new chapter title.

### Reproduction or review scenario

Have a reader answer from the chapter alone: how to show only chosen tags; how to keep a complete raw capture; what changes target bandwidth; whether changing ID allocation moves existing IDs; and whether `trice_INFO` or a dynamic target level controller is implemented.

### Requested change

Integrate the supplied chapter at the UM main-chapter level (`##`), with `###` subsections. Regenerate numbering/TOC using the project workflow. Explain terminology, all relevant flags, `TRICE_OFF`, direct/deferred routing boundaries, inclusive ID policy, known defects, and deferred compile-time/runtime selection. Link technical detail rather than duplicating conflicting reference manuals. Mark proposals as proposals until implemented.

### Acceptance criteria and regression tests

- [ ] Both language drafts carry the same technical content and current-versus-proposed distinctions.
- [ ] Implemented host `-logLevel` is not confused with the unimplemented draft `trice insert -loglevel` switch.
- [ ] Costs saved at compile time, on a target route, and on the host are distinguished.
- [ ] Custom-label limitations, ambiguous aliases, ID-history caveats, and existing compile guards are included.
- [ ] The two target extensions are described as deferred for lack of a demonstrated need, not universally useless.
- [ ] All internal links, heading levels, numbering, and command examples are validated before merging.
- [ ] Temporary known-issue notes are updated only after the corresponding changes and tests land.

### Compatibility and scope boundaries

This task does not authorize changing runtime behavior to match prose. The package chapters describe the reviewed behavior and clearly label the desired corrections.

### Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - current levels/tags documentation and future-development sections
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - actual CLI spellings and help
- [src/triceDefaultConfig.h](https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h) - target configuration

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L11](issues/L11_loglevel_semantics.md), [L19](issues/L19_deferred_compile_time_levels.md), [L20](issues/L20_deferred_runtime_levels.md), [L21](issues/L21_clarify_logging_terminology.md), [L24](issues/L24_separate_parser_chapter.md), [L25](issues/L25_server_use_evidence.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 27: L19 - Record the deferred proposal for level-specific compile-time selection**  
**Standalone source:** [issues/L19_deferred_compile_time_levels.md](issues/L19_deferred_compile_time_levels.md)

## Record the deferred proposal for level-specific compile-time selection

**Local reference:** L19  
**Type:** Deferred design proposal - not a bug  
**Suggested priority:** Deferred - reopen only for a concrete requirement  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** UM proposal, not a missing mandatory feature.

### Summary

Keep fine-grained compile-time selection such as `trice_INFO` on record without implementing it merely to imitate other loggers.

### Background and evidence

The UM proposes transforming a normal Trice call into level-specific macros and compiling selected levels out. Existing `TRICE_OFF` already provides broad file/project control. Per-level compile-time suppression could remove code, argument evaluation, and transmission for selected events, but introduces macro, parser, build-policy, and compatibility surface.

### Reproduction or review scenario

A reopening request should provide a firmware/code-size or deterministic-execution requirement that cannot be met adequately by existing controls. Include representative code and measured baseline cost, rather than only a preference for familiar logger syntax.

### Requested change

Record the decision to defer. If reopened, compare approaches compatible with both bind and insert. Preserve the existing default path and guarantee that disabled logging arguments are not evaluated. Do not require reversible source rewriting merely because it appears in an older draft.

### Acceptance criteria and regression tests

- [ ] The UM explicitly identifies `trice_INFO` and the proposed insert `-loglevel` as unimplemented draft concepts.
- [ ] The rationale for deferral and measurable reopening criteria are recorded.
- [ ] Existing `TRICE_OFF` capabilities and scope are described accurately.
- [ ] A later implementation proposal includes code-size, disabled-argument, compiler, and build-mode tests.
- [ ] Structured logging and context enrichment are not made dependent on this extension.

### Compatibility and scope boundaries

No implementation is requested now. Do not describe the proposal as inherently useless or confuse it with host `-logLevel`.

### Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - Compile-time Log-level Control draft
- [src/triceDefaultConfig.h](https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h) - `TRICE_OFF`
- [src/trice.h](https://github.com/rokath/trice/blob/wip/src/trice.h) - disabled/enabled public macro paths

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L18](issues/L18_central_manual_chapter.md), [L20](issues/L20_deferred_runtime_levels.md), [L21](issues/L21_clarify_logging_terminology.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 28: L20 - Record the deferred proposal for dynamic target-side log-level control**  
**Standalone source:** [issues/L20_deferred_runtime_levels.md](issues/L20_deferred_runtime_levels.md)

## Record the deferred proposal for dynamic target-side log-level control

**Local reference:** L20  
**Type:** Deferred design proposal - not a bug  
**Suggested priority:** Deferred - reopen only for a concrete requirement  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** UM proposal; existing output ID routing is not the same as early dynamic suppression.

### Summary

Add target runtime level control only if measured event rate, bandwidth, storage, or argument-evaluation cost justifies it.

### Background and evidence

The existing compact logging path and host selection are deliberate strengths. Deferred ID routing can restrict a destination but, in the inspected ring-buffer path, occurs after data preparation/encoding. A runtime threshold at acquisition could save additional work, but requires a defined control/configuration path, concurrency behavior, and filtering position.

### Reproduction or review scenario

A reopening request should show overflow, transmission load, bounded offline storage, or another resource limit. Quantify events per second and bytes per event. Include the cost of context providers and expensive argument expressions, not only the macro's buffer write.

### Requested change

Record deferral and conditions for reconsideration. If reopened, define how the level is set, how interrupt/task producers observe it, and exactly which work is skipped. Consider application-local control as well as a remote command path; a new remote protocol is not always necessary. If ID ranges implement thresholds, require a validated ID policy and history-safe migration.

### Acceptance criteria and regression tests

- [ ] The manual distinguishes existing static range routing from a dynamic acquisition-time level controller.
- [ ] The controller is not advertised as already implemented.
- [ ] No new level bytes or protocol changes are presumed necessary.
- [ ] A future proposal measures both filtered and unfiltered costs and defines disabled/default behavior.
- [ ] A future test plan covers concurrency, argument evaluation, buffering/packing, and control changes.
- [ ] Useful pre-failure context is considered when deciding what to discard.

### Compatibility and scope boundaries

No implementation is requested now. Do not add a check to every current log call without a justified requirement and measured impact.

### Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - Run-time Log-level Control draft
- [src/trice.c](https://github.com/rokath/trice/blob/wip/src/trice.c) - current output routing
- [src/triceRingBuffer.c](https://github.com/rokath/trice/blob/wip/src/triceRingBuffer.c) - deferred encode/output sequence

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L03](issues/L03_validate_active_id_policy.md), [L10](issues/L10_single_pack_guard_tests.md), [L18](issues/L18_central_manual_chapter.md), [L19](issues/L19_deferred_compile_time_levels.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 29: L21 - Rename the enrichment draft and separate levels, enrichment, and structured logging**  
**Standalone source:** [issues/L21_clarify_logging_terminology.md](issues/L21_clarify_logging_terminology.md)

## Rename the enrichment draft and separate levels, enrichment, and structured logging

**Local reference:** L21  
**Type:** Documentation / conceptual correction  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed terminology mismatch in the UM draft.

### Summary

The draft currently called "Trice Structured Logging" primarily describes automatic context enrichment. Correct the definition without discarding its useful embedded-logging ideas.

### Background and evidence

In the reviewed UM, 45.1 is the level-control draft and 45.2 is the context-oriented draft. The latter defines structured logging by automatically adding compile-time/runtime data. These are different dimensions: severity classifies an event, enrichment adds properties, and structured logging retains independently accessible fields. JSON is an output format, not the definition. A free-text message may coexist with structured context.

### Reproduction or review scenario

Compare the opening definitions with the examples: adding task ID, source position, and uptime to a printf-style message is enrichment; keeping those as independently accessible typed fields also yields structured data. Neither behavior requires the other in all cases.

### Requested change

Prefer the title "Automatic Context Enrichment" for that draft. Keep one small example showing enriched text versus an enriched structured event. Preserve links or redirect old anchors while updating references from 45.1. Separate implementation status from possibilities. Keep compile-time metadata in the dictionary and preserve the cost distinction for added dynamic values.

### Acceptance criteria and regression tests

- [ ] Definitions no longer equate automatic context addition with structured logging.
- [ ] Static context, source/session context, and per-event dynamic context are distinguished.
- [ ] A message may be free text without making the entire record unstructured.
- [ ] Existing examples are not presented as an implemented CLI or JSON guarantee.
- [ ] The terminology is consistent with the central chapter and both languages.
- [ ] Renaming does not lose prior design decisions, open questions, or old anchor compatibility.

### Compatibility and scope boundaries

Documentation correction now; a detailed `strice` extension specification remains a later task.

### Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - 45.1/45.2 headings and opening definition
- [Serilog enrichment](https://github.com/serilog/serilog/wiki/Enrichment)
- [OpenTelemetry log data model](https://opentelemetry.io/docs/specs/otel/logs/data-model/)

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L18](issues/L18_central_manual_chapter.md), [L22](issues/L22_reversible_enrichment_instrumentation.md), [L23](issues/L23_validate_draft_examples.md), [L26](issues/L26_structured_logging_feasibility.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 30: L22 - Remove unchanged-configuration heuristics from the future enrichment transformation contract**  
**Standalone source:** [issues/L22_reversible_enrichment_instrumentation.md](issues/L22_reversible_enrichment_instrumentation.md)

## Remove unchanged-configuration heuristics from the future enrichment transformation contract

**Local reference:** L22  
**Type:** Draft architecture risk  
**Suggested priority:** Deferred design work before implementing enrichment  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** The UM explicitly documents dependence on unchanged stf/stv settings and manual cache invalidation.

### Summary

A future enrichment implementation must not depend on guessing how to remove inserted context from user source after configuration changes.

### Background and evidence

The draft proposes heuristic recognition/removal of inserted context, requires `stf`/`stv` to remain unchanged between insert/clean, and tells users to clean before configuration changes and invalidate a cache. Arbitrary formatting transformations are not generally invertible without retained provenance. An interrupted build can also leave modified source visible.

### Reproduction or review scenario

Review configuration changes between insert and clean; aborted builds; user edits to an instrumented call; stale caches; and multiple projects sharing a source file. A template that omits or changes original content is a useful non-invertibility example.

### Requested change

Before implementing the extension, define a deterministic source-of-truth and provenance model. Prefer compatibility with generated bind artifacts or a provably reversible transform rather than a format-string heuristic. Keep field/type/position metadata coherent with generated argument additions. Separate collection configuration from host rendering configuration.

### Acceptance criteria and regression tests

- [ ] A changed enrichment configuration cannot silently corrupt source or remove original arguments.
- [ ] The recovery contract for interrupted builds and user edits is defined.
- [ ] Caches are keyed/invalidation-safe without requiring an undocumented manual deletion ritual.
- [ ] Shared-source and concurrent build/write cases have an explicit policy.
- [ ] Original template/arguments and structured field information remain recoverable where rewriting is used.
- [ ] No promise states that users always see original source while a pre/post-build rewrite is in progress.

### Compatibility and scope boundaries

This is a pre-implementation requirement, not a claim that an existing enrichment implementation is defective. Do not turn it into an unrelated parser rewrite.

### Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - enrichment draft stf/stv heuristic, clean and cache discussion
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - commented draft stf/stv option registrations

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L21](issues/L21_clarify_logging_terminology.md), [L23](issues/L23_validate_draft_examples.md), [L24](issues/L24_separate_parser_chapter.md), [L26](issues/L26_structured_logging_feasibility.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 31: L23 - Validate the draft C, shell, JSON, and macro-stringization examples before publication**  
**Standalone source:** [issues/L23_validate_draft_examples.md](issues/L23_validate_draft_examples.md)

## Validate the draft C, shell, JSON, and macro-stringization examples before publication

**Local reference:** L23  
**Type:** Documentation correctness  
**Suggested priority:** P2 for publication; does not block current logging  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Draft defects were identified in the review; examples were not executed against a complete checkout in this handover.

### Summary

Prevent examples in the future enrichment chapter from appearing executable or format-safe when their syntax or assumptions are invalid.

### Background and evidence

The reviewed discussion identifies malformed quoting in JSON-like templates and one generated C call; accidental shell backticks and unsafe unquoted expansions; and a `#pragma` example that expects `XSTR(TRICE_ETC)` to become empty after `#undef`. An undefined token is instead stringized as its name. A valid final JSON illustration does not validate the template that allegedly produced it.

A printf-style template is not a JSON serializer: static or dynamic text can contain quotes, backslashes, and control characters. The draft's speculative TCOBS size reduction must not be stated as a guaranteed ratio.

### Reproduction or review scenario

Extract every purportedly executable example from the current chapter and classify it as real, illustrative, or dependent on unimplemented options. Compile/preprocess actual C examples, parse shell examples, and validate produced JSON. Use an undefined macro, embedded quotes, a backslash, newline, and representative numeric edge values.

### Requested change

Repair syntax and mark non-executable proposals clearly. Specify any multi-stage template substitution before giving output claims. Replace unsafe JSON construction with a serializer in future host-output design. Move the specialized compiler-diagnostic/pragma technique to an optional experiment or clearly label its portability limits.

### Acceptance criteria and regression tests

- [ ] Every code block is either validated with its dependencies or explicitly illustrative.
- [ ] The undefined-macro example shows the correct token-stringization result.
- [ ] Shell examples avoid accidental command substitution and word splitting.
- [ ] JSON examples distinguish templates from serialized data and validate escaping.
- [ ] Integer types, formatting precision, units, NaN/infinity, and large-integer policy are not silently conflated.
- [ ] Compression-size and zero-cost claims are conditional or backed by a reproducible measurement.
- [ ] The draft does not suggest that dynamic-string hashing preserves the original string content.

### Compatibility and scope boundaries

Do not implement stf/stv merely to make documentation examples runnable. This issue groups the executable-example quality problem; the reversibility architecture is separate.

### Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - enrichment examples, user-defined values, CLI configuration and byte-count discussion
- [GCC stringization rules](https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html)
- [JSON syntax and interoperability rules](https://www.rfc-editor.org/rfc/rfc8259.html)

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L21](issues/L21_clarify_logging_terminology.md), [L22](issues/L22_reversible_enrichment_instrumentation.md), [L26](issues/L26_structured_logging_feasibility.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 32: L24 - Move internal parser development into a separate chapter and track it independently**  
**Standalone source:** [issues/L24_separate_parser_chapter.md](issues/L24_separate_parser_chapter.md)

## Move internal parser development into a separate chapter and track it independently

**Local reference:** L24  
**Type:** Documentation organization / scope control  
**Suggested priority:** P2  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** User-directed separation; current section 45.3 is a distinct internal-parser topic.

### Summary

Keep parser modernization separate from terminology, log-level corrections, enrichment, and the possible structured-event extension.

### Background and evidence

The reviewed UM places "Improving the Trice Tool Internal Parser" at 45.3 between enrichment and server use. Future structured consumers need access to event data before rendering, but that interface requirement does not prove a complete parser rewrite is necessary. The current optional `VisRecord`/`VisValue` path is an existing starting point for supported numeric events, not a complete general solution.

### Reproduction or review scenario

Move the section without changing its substantive design content. Check all links, TOC entries, numbering, and references from source/design documents. List the narrow interface dependencies separately.

### Requested change

Create an independent main chapter/work item. State that ID decoding, typed event data, selection, and presentation should have a clear boundary. Assess whether the existing typed-data hook suffices for a small feasibility exercise before expanding the parser project.

### Acceptance criteria and regression tests

- [ ] The parser topic is a separate chapter at the agreed heading level.
- [ ] All previous content and useful anchors are retained or redirected.
- [ ] No parser implementation is implicitly authorized by the documentation move.
- [ ] Structured/event-filter work documents its interface needs without a blanket dependency on a rewrite.
- [ ] The central chapter refers to the parser task only for relevant technical detail.

### Compatibility and scope boundaries

The separate chapter may change numbered locations; references should use stable anchors rather than fixed section numbers.

### Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - Improving the Trice Tool Internal Parser
- [internal/decoder/decoder.go](https://github.com/rokath/trice/blob/wip/internal/decoder/decoder.go) - `VisRecord`, `VisValue`, `VisRecordProvider`

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L17](issues/L17_early_host_filtering.md), [L18](issues/L18_central_manual_chapter.md), [L26](issues/L26_structured_logging_feasibility.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 33: L25 - Separate device-log ingestion from native server logging and qualify performance claims**  
**Standalone source:** [issues/L25_server_use_evidence.md](issues/L25_server_use_evidence.md)

## Separate device-log ingestion from native server logging and qualify performance claims

**Local reference:** L25  
**Type:** Documentation / feasibility evidence  
**Suggested priority:** P3 - before expanding server scope  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Future-development claims are hypotheses, not measured conclusions in this handover.

### Summary

Clarify which server use case Trice should solve and avoid developing a competing logging ecosystem without evidence of a useful advantage.

### Background and evidence

A server that decodes device Trice streams can benefit from named fields while leaving the target format intact. Replacing a server application's existing logger is a much broader proposition, involving dynamic fields/strings, concurrency, error data, ecosystem adapters, and operational behavior. Compact tokenization alone does not establish superior end-to-end throughput, energy consumption, or a lack of benefit from further compression.

### Reproduction or review scenario

Compare a device-log gateway scenario with a native-server producer scenario. For performance claims, compare equivalent retained information, enabled/disabled paths, compression settings, storage, and end-to-end decoding/query cost against relevant alternatives.

### Requested change

Rewrite "Using Trice on Servers" to distinguish the cases. Prioritize reuse of standard host outputs/consumers and require measured adoption/use-case evidence for a native server backend. Treat host key-ID interning as an optional implementation detail, not a new mandatory protocol. Do not require globally unique target event IDs when source/dictionary identity provides disambiguation.

### Acceptance criteria and regression tests

- [ ] Device ingestion and native server logging are separate use cases with separate requirements.
- [ ] Unsupported speed, energy, size, and no-further-compression claims are removed or labeled hypotheses.
- [ ] Comparisons preserve equivalent information and state benchmark conditions.
- [ ] The design retains multi-field events and their source/event-instance association.
- [ ] No new analysis platform, global key registry, or target format is mandated without a concrete requirement.
- [ ] At least one practical consumer/workflow is identified before proposing a server-specific implementation.

### Compatibility and scope boundaries

This is a scope/evidence task, not a demand to replace Logrus, slog, Serilog, or other server loggers.

### Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - Using Trice on Servers
- [Go records, attributes, and handlers](https://pkg.go.dev/log/slog)
- [Logrus structured fields and formatters](https://github.com/sirupsen/logrus)

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L18](issues/L18_central_manual_chapter.md), [L21](issues/L21_clarify_logging_terminology.md), [L26](issues/L26_structured_logging_feasibility.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 34: L26 - Validate a useful structured-event workflow before specifying a strice extension**  
**Standalone source:** [issues/L26_structured_logging_feasibility.md](issues/L26_structured_logging_feasibility.md)

## Validate a useful structured-event workflow before specifying a strice extension

**Local reference:** L26  
**Type:** Exploratory feasibility - not an implementation specification  
**Suggested priority:** P3 - evidence first  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Conceptual proposal from the review; strice syntax and schema are not yet approved APIs.

### Summary

Determine whether optional named, typed fields provide measurable practical value while retaining Trice's efficient target encoding.

### Background and evidence

A proposed `strice("Motor {motor_id}: temperature {temperature_c} C", motor, temperature)` can associate one event ID with multiple argument fields. The host can resolve a key across many IDs and argument positions without parsing rendered text. One key per target message is unnecessary and would lose convenient event grouping. Optional numeric key IDs belong on the host if a consumer benefits from them.

The promising workflows are automated firmware assertions, measurement analysis, and field diagnosis. A formatter alone is not sufficient evidence of demand.

### Reproduction or review scenario

Choose one real workflow: for example, assert `temperature_c < limit` across two event IDs and firmware builds while freely changing display wording. Compare the effort and reliability with the current Trice/visualization workflow. Retain a free message as an optional rendering, not the only copy of the values.

### Requested change

Use the smallest reversible prototype or design demonstration needed to assess usefulness. Record field types/widths, names/units, dictionary identity, and event grouping. Show how optional enrichment extends the same event without collisions. Check new names against a reviewed dictionary/schema before accepting them; a dictionary that automatically accepts every typo is not validation. Defer a full syntax/schema/CLI specification until value and constraints are clear.

### Acceptance criteria and regression tests

- [ ] The demonstration supports a field query across multiple event IDs without rendering and reparsing messages.
- [ ] It shows identical target payload values/widths to an equivalent ordinary Trice event; any wrapper/code/runtime overhead is measured separately.
- [ ] Additional dynamic context is not advertised as free.
- [ ] Name collisions, duplicate placeholders, type/argument mismatch, and dictionary/version mismatch have stated handling.
- [ ] A real workflow owner or concrete maintenance benefit is identified; lack of benefit can justify stopping.
- [ ] The output leaves a decision record, not an unapproved full extension specification.

### Compatibility and scope boundaries

Preserve normal Trice behavior. No mandatory JSON on target, one-key restriction, new per-event key IDs, or new target-level selection is required. Wrapper call-site provenance and exactly-once/disabled argument evaluation remain design questions.

### Source references

- [internal/decoder/decoder.go](https://github.com/rokath/trice/blob/wip/internal/decoder/decoder.go) - existing optional typed numeric records
- [internal/id/id.go](https://github.com/rokath/trice/blob/wip/internal/id/id.go) - current dictionary entry structure
- [named message-template fields](https://messagetemplates.org/)
- [tokenized static metadata](https://pigweed.dev/pw_log_tokenized/)

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L17](issues/L17_early_host_filtering.md), [L19](issues/L19_deferred_compile_time_levels.md), [L20](issues/L20_deferred_runtime_levels.md), [L21](issues/L21_clarify_logging_terminology.md), [L22](issues/L22_reversible_enrichment_instrumentation.md), [L24](issues/L24_separate_parser_chapter.md), [L25](issues/L25_server_use_evidence.md). Local references are handover identifiers, not existing GitHub issue numbers.

---

**Part 35: L27 - Reject malformed IDRange arguments instead of silently skipping them**  
**Standalone source:** [issues/L27_reject_malformed_idrange.md](issues/L27_reject_malformed_idrange.md)

## Reject malformed IDRange arguments instead of silently skipping them

**Local reference:** L27  
**Type:** Bug / validation - additional finding during handover review  
**Suggested priority:** P1 when allocation policy drives routing  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed missing-colon branch in `EvaluateIDRangeStrings`.

### Summary

A malformed `-IDRange` argument must not be silently omitted while the command continues with an unintended allocation policy.

### Background and evidence

`EvaluateIDRangeStrings` splits each rule at `:`. If the separator is absent, the function currently continues to the next item rather than returning an error. Other malformed forms reach a syntax error. This makes one malformed syntax class unexpectedly permissive and can undermine a target route relying on the intended range.

### Reproduction or review scenario

Pass a range value without a colon, for example `-IDRange err10,99`, alone and alongside another valid rule. Repeat through insert and bind. Confirm that validation fails before source or sidecar modification.

### Requested change

Return an actionable syntax error for every malformed rule, including missing separators and empty components. Reuse consistent tag validation and preserve existing overlap/duplicate-range checks. Review valid numeric bounds for the supported ID representation without assuming that generic Go integer parsing proves they are legal Trice IDs.

### Acceptance criteria and regression tests

- [ ] Missing `:`, missing comma, empty tag/bounds, non-numeric bounds, and reversed ranges fail consistently.
- [ ] One valid rule does not conceal another malformed rule.
- [ ] Unknown tags and duplicate alias-equivalent rules produce useful diagnostics.
- [ ] Validation fails before any artifact is modified.
- [ ] Insert and bind share the same rule contract.
- [ ] Legal adjacent non-overlapping ranges and inclusive endpoints remain accepted.

### Compatibility and scope boundaries

This is a newly rechecked finding, not a retroactive claim that it was already in the earlier issue list. It does not request automatic renumbering.

### Source references

- [internal/id/switchIDs.go](https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go) - `EvaluateIDRangeStrings`, missing-colon `continue`
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - range validation before insert/bind

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

### Related local issues

[L01](issues/L01_unique_tag_aliases.md), [L03](issues/L03_validate_active_id_policy.md), [L04](issues/L04_safe_id_policy_migration.md). Local references are handover identifiers, not existing GitHub issue numbers.


---

**Artifact validation report**

## Artifact validation

**Executed:** 10 September 2026, in the document-generation container.

This reports checks of the handover files, not tests of Trice.

### Results

- 27 standalone English issue files found, with unique L01-L27 identifiers.
- Each issue has a single H1 title and the required evidence, scope, reproduction/review, acceptance, compatibility, and source sections.
- Acceptance-test checkboxes remain unchecked.
- Both standalone chapter files start at H2 and use only H2/H3/H4.
- The English and German chapter files have identical explicit section-anchor sequences.
- 37 Markdown files parsed successfully with CommonMark/table support before adding this report.
- 1010 parsed links inspected; relative file targets resolved, including the report emitted in this step.
- No duplicate explicit anchors, duplicate parsed reference definitions, unbalanced triple-backtick fences, replacement characters, or unresolved Unicode escapes found.
- No conversation-only web/file citation IDs remain in the delivered Markdown. Sources use ordinary persistent URLs and named code symbols.
- Combined documents rewrite relative links and namespace reference labels/explicit anchors as needed.

### Not covered

No full Trice checkout, upstream Go suite, target compilation, hardware execution, performance benchmark, or end-to-end command reproduction was completed. External links were consulted during source review, but local artifact validation is not a permanent external link-availability guarantee. The moving branch was not pinned to a certified commit snapshot.

The ZIP archive is separately checked for CRC/read errors after creation; a SHA-256 manifest is included for its file contents.
