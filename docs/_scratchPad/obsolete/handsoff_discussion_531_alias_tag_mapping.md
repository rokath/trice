# HANDSOFF: GitHub Discussion #531 — Alias-Assigned Tags, Context Enrichment, and Structured Logging

**Repository:** `rokath/trice`  
**Primary discussion:** https://github.com/rokath/trice/discussions/531  
**Baseline for this analysis:** `v2.0.0-beta-1` (published 2026-10-17)  
**Status:** Design/analysis handoff for Codex — do not treat undecided design points as requirements yet.  
**Primary implementation direction:** Extend the existing alias CLI so an alias can optionally be assigned a Trice tag, e.g. `-alias MyAlarm:wrn`, without inferring semantics from the alias name and without requiring the source format string to carry the event tag.

---

## 0. Executive summary

Discussion #531 asked how an existing structured embedded logging style could be expressed efficiently with Trice. The original user wanted short calls such as:

```c
log_error("My Error %2X", value);
```

while obtaining, without repeatedly transmitting constant text:

- log severity / level,
- source file and line,
- module/process context,
- FreeRTOS task context,
- timestamp,
- reliable machine-readable formatting,
- correct behavior under preemption,
- and a compact binary representation.

When #531 was opened in May 2025, Trice did not yet have the current v2 structured-logging and Context Enrichment workflows. `v2.0.0-beta-1` now solves a substantial part of the original request:

- `bind` keeps IDs out of user source while assigning IDs to source sites.
- `li.json` already maps each Trice ID to file and line. No extra file-ID transport is needed for ordinary source-location reporting.
- Structured Logging exports one event as text, JSON/NDJSON, or key/value, including tag, derived level, message, optional source location, timestamps, IDs, and named values.
- Tags, weights, `-pick`, `-ban`, `-logLevel`, `-ulabel`, and `untagged` provide a real severity/category model.
- Context Enrichment (`-ce`) can add selected scalar runtime context, e.g. a FreeRTOS task handle, to the same event without editing every source call.
- Existing alias support (`-alias` / `-salias`) allows project-specific macro names to be treated as Trice calls while preserving the call site for ID/location handling.

The remaining gap relevant to #531 is not primarily file/line transport. The important usability gap is this:

> A project-specific alias such as `MyAlarm(...)` should be able to carry an explicitly configured Trice tag (for example Warning) even if the format string itself does not start with `wrn:`.

Proposed CLI direction:

```sh
trice bind -alias MyAlarm:wrn ...
```

Source:

```c
MyAlarm("Temperature too high: %d degrees", value);
```

The alias name itself has **no semantic meaning**. `MyAlarm`, `LogX`, `Banana`, etc. are equivalent names until the CLI explicitly assigns a tag. There must be no heuristic such as looking for `error`, `warn`, `alarm`, etc. in the alias name.

A source prefix such as:

```c
MyAlarm("temperature:Temperature too high: %d degrees", value);
```

may still be used as a Context Enrichment selector:

```sh
-ce 'temperature:", limit={limit:%d}", temperatureLimit'
```

The alias-assigned tag and CE selectors are intentionally separate concepts.

The unresolved architectural question is how the assigned tag becomes durable metadata for later logging while the source format string remains semantically unchanged with respect to tag classification. In particular, **do not assume yet that `til.json` must gain a `Tag` field**. Codex should analyze at least these approaches:

1. store explicit tag metadata in TIL (`Tag` or equivalent),
2. prepend a synthetic/virtual tag only to the effective dictionary schema while leaving source untouched,
3. reuse or extend existing `-ulabel` / `untagged` / tag-classification machinery in another way,
4. or another simpler design that preserves historical decoding and current Trice semantics.

The design must remain compatible with Bind/Insert/Clean, cumulative TIL history, ID stability rules, CE, tag-specific ID ranges, Structured Logging, filtering, statistics, and both old and new dictionaries as far as reasonably possible.

Runtime strings through CE are **explicitly out of scope** for this work. The project can state that this new alias-tag feature applies to the normal scalar Trice families; it does not need to solve arbitrary combinations of runtime strings and scalar CE values.

---

## 1. Why this work exists: Discussion #531

### 1.1 Original user scenario

Discussion #531 is titled **“How to use trice to cover these example use cases?”** and was opened by `jnz86` on 2025-05-20:

https://github.com/rokath/trice/discussions/531

The user had an existing system that sent log messages over RTT to a telnet-oriented host program. Their text contained machine-readable marker fields such as `<...>` before the human-readable message. Their logger abstracted repetitive context and allowed calls conceptually like:

```c
log_error("My Error %2X", value);
```

The logger automatically supplied context such as:

- module,
- process,
- task context under FreeRTOS,
- timestamp,
- log severity,
- and source information.

The discussion raised several concrete questions.

### 1.2 Essential requirements extracted from #531

#### A. File and line only where useful

The user asked whether file name and line number could be included only for error-level logs.

The key v2 insight is that file and line normally do **not** need to be transmitted as target runtime values. A normal Trice ID already identifies a source site, and the matching `li.json` contains the file/line mapping. Therefore the host can display, suppress, or structurally export location metadata as desired.

#### B. Abstraction without losing the caller location

The user was concerned that wrapping a Trice call in a helper function or general wrapper would make location information point to the helper definition rather than the application call site.

That concern is valid for a shared Trice site in an ordinary helper function and for some wrapper-macro patterns. However, a **registered Trice alias at the invocation site** is a better fit: each alias invocation can be treated as the actual Trice source site and receive its own ID/location mapping.

This distinction is important:

```c
// Shared function: one inner Trice site.
void logError(int code) {
    trice32("err:code=%d", code);
}

// Alias-style source site: each MyAlarm(...) invocation is an instrumented site.
MyAlarm("Temperature too high: %d", value);
```

The first form naturally identifies the function body. The second can identify the application call site if it is handled through Trice alias instrumentation.

#### C. Constant metadata should not become runtime string traffic

The user explicitly wanted to avoid transmitting static strings such as MODULE and PROCESS for every event.

Trice's general architectural principle should remain:

> Compile-time information belongs in dictionaries/metadata; only runtime-varying values should consume target payload bytes.

For source location, `li.json` already implements this principle. Tags are also currently represented as static format/dictionary information rather than runtime payload.

Module/process mapping is not fully solved merely by alias-to-tag mapping. This handoff does **not** propose a universal metadata system for arbitrary module/process constants. However, v2 already provides building blocks:

- source path from LI can be mapped by a consumer to a module,
- static literal context can be added to effective formats with CE rules where selectors are available,
- dynamic task context can be added as scalar CE data,
- target timestamps already exist,
- JSON/KV output avoids parsing ad-hoc textual markers.

Do not expand this task into a general arbitrary compile-time metadata framework unless analysis shows that it is necessary for the alias-tag feature.

#### D. Consistent machine-readable formatting

The user did not merely want pretty human-readable output. Their markers were used by software.

`v2.0.0-beta-1` now provides Structured Logging:

```sh
trice log ... -logFormat json
```

which emits one JSON object per accepted event (NDJSON) and exposes host metadata and user fields separately. This is a much better answer to the original marker-parsing requirement than concatenating multiple text Trices.

#### E. Avoid multiple-call fragmentation

The original user considered multiple Trice calls for context + message and correctly worried about preemption and fragmentation under FreeRTOS.

For normal scalar context, v2 CE removes most of the reason to split a logical log event. The preferred pattern is one event containing its ordinary message values plus extra runtime context fields.

Deferred mode itself does not magically make several calls one semantic event; it is orthogonal to compile-time instrumentation and event composition.

#### F. Argument-width and runtime-string concerns

The original discussion also asked about mixed runtime strings plus numeric arguments. Current Trice still has family-specific restrictions. This handoff deliberately narrows scope:

> **Do not solve arbitrary runtime-string + scalar CE composition as part of this alias-tag task.**

Document that limitation instead. The alias-tag feature should focus on the scalar/ordinary Trice use case that covers typical `printf`-like integer/float logging and the #531 severity/context problem.

---

## 2. Historical context: the old #531 answer versus v2 beta 1

The accepted answer in June 2025 explored techniques that were reasonable at the time:

- separate Trice calls for module/process/context/message,
- critical sections to avoid interleaving,
- possible future context insertion,
- host-side compile-time context ideas.

Later comments in the discussion pointed to the then-new structured logging documentation.

Since then the project has evolved materially. `v2.0.0-beta-1` was published on 2026-10-17 and its release notes explicitly include:

- Bind,
- Structured Logging,
- Context Enrichment,
- tag/severity filtering,
- JSON/KV output,
- source locations as structured metadata,
- improved examples including FreeRTOS task context.

Therefore do **not** design a solution as if the 2025 implementation state were still current. The purpose of this work is to close the remaining ergonomic gap using the v2 architecture, not to revive the multi-call workarounds from the original answer.

Release reference:

https://github.com/rokath/trice/releases/tag/v2.0.0-beta-1

---

## 3. Current v2.0.0-beta-1 capability matrix

### 3.1 Source location: already solved for normal source sites

`li.json` maps each Trice ID to a canonical source path and line number, e.g. conceptually:

```json
{
  "1234": {
    "File": "src/motor.c",
    "Line": 42
  }
}
```

Relevant documentation:

- `docs/rm/cli.md` — Location Information
- `docs/rm/id_management.md` — Trice location information file `li.json`

Important properties:

- File/line is host metadata; it need not consume target payload.
- LI must match the exact firmware build when exact locations matter.
- `trice log` / `tlog` can display it.
- Structured JSON/KV can expose `file` and `line` as separate fields.
- The Trice ID itself is sufficient to perform the lookup.

**Conclusion:** do not add a second source-file ID mechanism merely to answer #531.

### 3.2 Structured Logging: already solves the host parsing problem

Relevant documentation:

- `docs/rm/structured_logging.md`

Current structured records provide:

- `tag`,
- derived `level`,
- `message`,
- optionally `id`,
- optionally `file`, `line`,
- target timestamps and deltas,
- host timestamp,
- named user fields.

This is already a strong replacement for custom `<markers>` followed by text.

The user's existing display application can either:

1. consume JSON/KV directly, or
2. use an adapter that converts Trice structured output into the legacy marker format.

A special `tlog` feature that emits file/line only for errors is therefore **not necessary to make #531 usable**. The consumer can simply ignore those fields for other levels. A host-only location-selection option could still be added later if desired, but it is not the primary missing feature.

### 3.3 Tags, weights, levels, filtering: already present

Relevant documentation:

- `docs/rm/tags_color_and_weights.md`

Current v2 semantics include:

- built-in tag groups and aliases,
- weights 0..999,
- derived levels such as WARNING and ERROR,
- `-pick`, `-ban`, `-logLevel`,
- custom labels via `-ulabel`,
- an `untagged` group for events whose stored format has no recognized tag,
- tag statistics,
- tag-specific ID range handling.

Important architectural detail:

> Today, tags normally come from a prefix at the beginning of the stored format string.

This is exactly the coupling the proposed alias-tag feature may need to relax.

### 3.4 Context Enrichment: already supplies runtime scalar context

Relevant documentation:

- `docs/rm/context_enrichment.md`
- `docs/rm/examples.md`

Example concept:

```sh
-ce 'ctx:", task={task:%p}", osThreadGetId()'
```

A selected event gains the runtime task value in the same binary event.

The `G0B1_features` example demonstrates a shared logging site called from two FreeRTOS tasks; the event carries different task handles.

This directly addresses a major #531 concern: task context does not need a separate Trice call and therefore does not need textual line concatenation or a critical section merely to preserve context/message association.

Current limits must remain documented:

- scalar CE values must fit the Trice family's value model,
- max scalar argument count remains limited,
- `bind -ce` currently supports direct line-addressable sites and deliberately rejects some complex wrapper/rebase sites,
- runtime string/buffer families do not gain arbitrary scalar CE arguments.

### 3.5 Existing alias support: the right starting point

Relevant documentation:

- `docs/rm/legacy_user_code.md`
- existing `-alias` / `-salias` help

Current behavior:

```sh
-alias DEBUG_PRINT
-salias DEBUG_PRINT_S
```

registers project-specific macro names as equivalents of `trice()` or `triceS()`.

Current implementation areas include:

- `internal/id/vars.go` — `TriceAliases`, `TriceSAliases`
- `internal/id/manage.go` (or current file containing `resolveTriceAlias` / `ProcessAliases`)
- `internal/id/bindAdvanced.go`
- alias integration tests in `internal/id/*_test.go`
- CLI setup in `internal/args/init.go`

`TriceFmt` currently contains:

```go
type TriceFmt struct {
    Type  string `json:"Type"`
    Strg  string `json:"Strg"`
    Alias string `json:"-"`
}
```

The alias name is currently transient/internal and not written to `til.json`.

This is the correct subsystem to extend. Do not introduce a separate unrelated mechanism for “logging functions with levels” if alias metadata can solve it cleanly.

---

## 4. What v2 beta 1 still does NOT solve cleanly

### 4.1 A custom logging alias cannot currently define its tag independently of the format string

Desired source:

```c
MyAlarm("Temperature too high: %d degrees", value);
```

Desired build configuration:

```sh
trice bind -alias MyAlarm:wrn ...
```

Desired semantics:

- `MyAlarm` is treated as a normal Trice alias at the invocation site.
- The event's true tag is Warning.
- The default derived level is therefore WARNING.
- The source format string does not need to contain `wrn:`.
- The tag costs no target runtime bytes.
- The source location remains the alias invocation site through normal ID/LI behavior.
- Host filtering/statistics/colors/structured level must all see the assigned tag.

This is the primary proposed enhancement.

### 4.2 A normal helper function is not an equivalent replacement when caller location matters

This is valid code:

```c
static inline void log_error(int value) {
    trice32("err:Error %d", value);
}
```

but LI naturally identifies the Trice site in the helper body, not every caller.

That is acceptable for some applications but does not fully satisfy the #531 caller-location requirement.

A registered alias at the application invocation site is therefore the preferred abstraction for this specific use case.

### 4.3 Module/process automation is still application-specific

Alias-to-tag mapping does not automatically solve all static metadata mentioned in #531.

Do not claim otherwise.

Possible current approaches include:

- derive module from LI path on the host,
- add static format text with CE when selectors/configuration make that practical,
- add dynamic task/process IDs as scalar CE values,
- let the consumer map IDs/locations to project-specific metadata.

A future general compile-time metadata registry may be interesting, but it is not required for this task.

---

## 5. Preferred CLI design direction

### 5.1 Extend the existing option, do not add name heuristics

Preferred syntax:

```sh
-alias MyAlarm:wrn
```

Backward-compatible existing syntax remains:

```sh
-alias MyAlarm
```

Potential symmetric string-alias syntax:

```sh
-salias MyTextAlarm:err
```

Whether tagged `-salias` is implemented in the same change should be decided after inspecting complexity. Semantically it is natural, even though scalar CE for runtime strings remains out of scope.

### 5.2 Meaning of `Name:Tag`

`Name` is a C macro/function-like token registered as a Trice alias.

`Tag` is a Trice application tag group, not a hard-coded “level”. For example:

```sh
-alias MyAlarm:wrn
```

means:

- alias = `MyAlarm`,
- application tag group = Warning,
- default weight = whatever Warning currently has (normally 600),
- exported level follows the ordinary weight-to-level mapping.

Therefore if Warning's weight is deliberately changed with existing tag configuration, the derived level follows existing Trice rules.

**Do not introduce a second severity model inside alias handling.**

### 5.3 Do not infer tag semantics from alias spelling

These must be legal and equivalent if configured identically:

```sh
-alias MyAlarm:wrn
-alias ABC:wrn
-alias Banana:wrn
```

No code should inspect substrings such as:

- `Error`,
- `Warn`,
- `Alarm`,
- `Fatal`,
- `Debug`.

The mapping is explicit CLI configuration only.

### 5.4 Format strings remain source-owned and are not rewritten merely because of alias tag assignment

Given:

```sh
-alias MyAlarm:wrn
```

this source must remain source-level text as written:

```c
MyAlarm("Temperature too high: %d degrees", value);
```

Do not rewrite it into:

```c
MyAlarm("wrn:Temperature too high: %d degrees", value);
```

and do not require the user to write `wrn:`.

For Bind, the source should of course remain unchanged as usual.

For Insert/Clean, normal ID insertion/removal and CE source transformations may still occur according to their established contract, but the alias-tag mapping itself should not cause a literal tag to be inserted into the user's format string.

The unresolved question is whether a synthetic tag prefix may exist **internally in the effective schema/TIL**. That is analyzed separately below.

---

## 6. Alias tag and CE selector are separate dimensions

This is a core requirement from the design discussion.

Example source:

```c
MyAlarm("temperature:Temperature too high: %d degrees", value);
```

CLI:

```sh
trice bind \
  -alias MyAlarm:wrn \
  -ce 'temperature:", limit={limit:%d}", temperatureLimit'
```

Intended interpretation:

- `MyAlarm:wrn` assigns the event to the Warning tag group.
- `temperature:` is a CE selector contained in the source format string.
- `temperatureLimit` is runtime context appended by CE.
- The Warning classification is not derived from `temperature:`.
- The source does not need `wrn:`.

A plausible final human message could be:

```text
Temperature too high: 85 degrees, limit=80
```

while structured output contains the Warning tag/level and the named context field.

### 6.1 Existing CE selector rules should be reused, not duplicated

Current CE already scans a contiguous prefix sequence and supports:

- built-in tags as selectors,
- user labels,
- free lowercase selectors,
- multiple selectors in source order.

The alias-tag feature should integrate with this rather than creating a second enrichment language.

### 6.2 Open decision: should the assigned alias tag itself select CE rules?

This is desirable and should be strongly considered.

Example:

```sh
trice bind \
  -alias MyAlarm:wrn \
  -ce 'wrn:", task={task:%p}", osThreadGetId()'
```

Source:

```c
MyAlarm("Temperature too high: %d", value);
```

Expected intuitive behavior would be that the event receives the Warning-wide CE rule even though `wrn:` is not written in the format string.

This means CE selection may need to consider:

1. the alias-assigned event tag, and
2. explicit source prefix selectors such as `temperature:`.

If the same CE selector group is reached through both routes, it must be applied only once.

This behavior is not yet a final requirement; Codex should analyze whether it can be achieved cleanly with the existing `selectContextRules` design and document the result.

---

## 7. What if the alias format begins with something that looks like another tag?

This is intentionally **not** a hard semantic conflict.

Example:

```sh
-alias MyAlarm:wrn
```

Source:

```c
MyAlarm("err:Temperature too high: %d", value);
```

Proposed semantics:

- Event classification remains the explicitly assigned alias tag: Warning.
- `err:` in the source format is not allowed to silently override the alias tag.
- The source text is not automatically rewritten or removed by the alias-tag mapping.
- Existing CE prefix handling may still see `err:` as a selector if a corresponding CE rule is explicitly configured; this interaction must be tested/documented.
- The build may emit a **suppressible diagnostic hint/warning** because a tag-looking source prefix can mislead a reader.

The diagnostic should be informational about ambiguity, not semantic.

Conceptual diagnostic:

```text
temperature.c:137: warning: alias MyAlarm is assigned tag WARNING,
but the format begins with tag-like prefix "err:"; the alias tag controls
event classification and the prefix remains source text / selector input.
```

Do not freeze this exact wording or warning-switch name yet.

### 7.1 Warning behavior to analyze

Possible policy:

- same tag group as alias assignment: no warning, or a low-value redundancy warning,
- different built-in severity tag: warning,
- different built-in category tag: warning or no warning depending on false-positive risk,
- registered `-ulabel` prefix: likely warn only if clearly being used as a tag rather than intentionally as CE selection,
- free CE selector (`temperature:`) not registered as a tag: no warning.

The diagnostic must be suppressible. Before inventing a new `-Wno-*` mechanism, inspect whether Trice already has a suitable warning-control pattern. If not, propose the smallest consistent mechanism.

The suppression setting must affect diagnostics only. It must not change:

- IDs,
- TIL,
- LI,
- CE selection,
- emitted binary records,
- tag classification.

---

## 8. Central unresolved architecture question: where does the assigned tag live?

This must be analyzed before implementation.

### 8.1 Why persistence is required

At runtime the target transmits an ID plus runtime values. Later the logger needs to know the event tag in order to perform:

- tag classification,
- derived level,
- colors,
- `-pick`,
- `-ban`,
- `-logLevel`,
- tag statistics,
- structured JSON/KV `tag` and `level` fields.

If the source format string does not itself contain the tag, the information must survive instrumentation somewhere.

Requiring the user to repeat exactly the same alias mapping during every later decode would be fragile, especially for saved field captures. Historical recordings must remain decodable from build metadata.

### 8.2 Option A: add optional tag metadata to `til.json`

Conceptual schema:

```json
{
  "1234": {
    "Type": "trice",
    "Strg": "Temperature too high: %d degrees",
    "Tag": "WARNING"
  }
}
```

The exact representation is undecided; `Tag` could store a canonical group name, an internal key, or another stable spelling.

#### Advantages

- Clean semantic separation between message template and event classification.
- Source format remains exactly the application message / CE selector text.
- No synthetic tag leaks into text output under `-color off`.
- JSON `message` does not need to strip a prefix that was never written by the user.
- Works naturally with the user's idea that format-string content and alias tag are independent.
- Historical ID metadata can be self-contained.
- A new logger can cleanly prefer explicit metadata and fall back to current prefix classification for old entries.
- User-defined aliases can theoretically map to tags regardless of display casing without manufacturing a textual prefix.

#### Disadvantages / required work

- Changes the TIL schema.
- `TriceFmt` or equivalent persistent type changes.
- All code assuming schema identity is only `Type + Strg` must be reviewed.
- Decoder/logger must use explicit tag metadata when present.
- Structured output, text colors, filters, statistics, etc. must all use the same resolved tag.
- Generated/readable dictionary copies and tests need updates.
- Old host tools will ignore an unknown JSON field when unmarshalling into the old struct, but they would then classify such a no-prefix event as `untagged`. Therefore host/dictionary version compatibility needs explicit documentation.
- On-target/local decoding paths must be reviewed to determine whether they need explicit tag metadata or can safely ignore it.

#### Critical ID-history rule

If the alias mapping changes from Warning to Error, the meaning of an existing historical ID must not silently change.

Therefore an explicit tag must participate in **schema identity / ID ownership** if stored separately.

For example:

```text
MyAlarm + "Temperature..." + WARNING
```

must not be considered the same historical schema as:

```text
MyAlarm + "Temperature..." + ERROR
```

unless there is some other immutable mechanism that preserves old semantics.

A likely implementation property is that adding `Tag string` to comparable `TriceFmt` would naturally make map-key equality include the tag, but do not assume that alone is sufficient; review all ID allocation/reuse paths.

### 8.3 Option B: prepend a synthetic tag only to the effective dictionary format

Source remains:

```c
MyAlarm("Temperature too high: %d degrees", value);
```

but instrumentation internally treats the effective stored schema as if it were:

```text
wrn:Temperature too high: %d degrees
```

without modifying user source.

#### Advantages

- Reuses the existing tag parser and most host logging behavior.
- No TIL schema extension required.
- Existing schema identity automatically changes when the assigned tag changes because `Strg` changes.
- Existing filtering, statistics, colors, levels, and CE tag selectors may work with relatively little new host code.
- Historical TIL behavior remains close to current conventions.

#### Disadvantages / risks

This is deceptively simple and must be tested carefully.

1. The stored `Strg` is no longer literally the user's format string.
2. `-color off` currently keeps written tag prefixes. A synthetic prefix could become visible even though the user never wrote it.
3. Lowercase-prefix stripping becomes part of semantics. User-defined tag casing may cause synthetic text leakage.
4. If the original source begins with another prefix:

   ```c
   MyAlarm("err:Temperature ...")
   ```

   the effective schema could become:

   ```text
   wrn:err:Temperature ...
   ```

   This may be workable, but text and structured message behavior must be verified.
5. A source selector such as `temperature:` becomes the second prefix after the synthetic tag; CE selection order and prefix removal need explicit tests.
6. A custom tag assigned through `-ulabel` may be case-sensitive. Manufacturing a canonical hidden textual spelling could be non-trivial.
7. The user explicitly wants source format strings not to be semantically interpreted as the event tag for tagged aliases. A synthetic prefix is still an implementation coupling between tag and `Strg`, even if it does not modify source.

This option may still be the lowest-risk implementation, but only if presentation can distinguish synthetic classification metadata from user text or if the behavior is otherwise proven acceptable.

### 8.4 Option C: require the same alias/tag CLI mapping during logging

Example:

```sh
trice log -alias MyAlarm:wrn ...
```

#### Advantage

- No TIL change.

#### Disadvantages

- The binary event contains only the ID, not the alias name.
- The alias name is not currently persisted in `til.json` (`Alias` is `json:"-"`).
- Saved captures would require external build CLI knowledge.
- Historical decoding becomes fragile.
- A logging command does not necessarily scan the original source tree.
- This conflicts with Trice's self-contained dictionary philosophy.

**This should normally be rejected.** The instrumentation command may accept the alias mapping, but the resulting build metadata should carry whatever the logger later needs.

### 8.5 Option D: reuse `-ulabel` / `untagged` without adding explicit metadata

This must be investigated because the current tag system already contains useful concepts:

- user-defined tags (`-ulabel`),
- built-in `untagged`,
- runtime classification from stored format prefixes,
- custom weights/colors,
- tag-specific ID ranges.

However, `-ulabel` currently registers **names found in format prefixes**. It does not map a Trice ID or an alias invocation to a tag when no such prefix exists.

Therefore `-ulabel` alone does not obviously solve alias-to-tag classification. It may still provide reusable registration/canonicalization infrastructure for validating the right-hand side of `-alias Name:Tag`.

Likewise, `untagged` is valuable as the default/fallback group, but it does not itself carry an external alias-assigned classification.

Codex should inspect the emitter/tag registry and determine which pieces can be reused rather than inventing parallel tag tables.

### 8.6 Option E: infer tag from ID ranges

Do not use tag-specific ID ranges as the primary persisted tag representation.

Although tag-specific ranges exist for target routing/assignment, deriving semantic tag classification from an ID numeric range would couple unrelated concerns and break easily when ranges change or overlap in intent.

Treat ID ranges as consumers of the resolved tag, not as the canonical tag database.

---

## 9. Recommended internal model to investigate

Do not mechanically keep `TriceAliases []string` if richer metadata is required.

A possible internal representation is conceptually:

```go
type AliasSpec struct {
    Name string
    Kind AliasKind // scalar trice or string triceS
    Tag  string    // empty means current behavior
}
```

This is illustrative, not a mandated type name.

Requirements:

- Preserve the current `-alias Name` and `-salias Name` behavior exactly.
- Parse `-alias Name:Tag` deterministically.
- Alias C identifiers cannot contain `:`, so first-colon splitting is practical.
- Validate `Name` as current alias handling requires.
- Resolve built-in tag aliases through the existing emitter/tag registry.
- Consider user-defined `-ulabel` tags.
- Preserve exact-case semantics for user-defined labels.
- Reject genuinely ambiguous duplicate configuration.

### 9.1 Duplicate/conflicting CLI definitions

Suggested behavior:

```sh
-alias MyAlarm:wrn -alias MyAlarm:wrn
```

may be accepted idempotently.

But:

```sh
-alias MyAlarm:wrn -alias MyAlarm:err
```

should be a configuration error.

Likewise, analyze:

```sh
-alias MyAlarm
-alias MyAlarm:wrn
```

Prefer rejecting ambiguous mixed registration rather than silently making option order semantic, unless there is a strong compatibility reason for a clear last-wins rule.

Existing behavior where one token is registered in both `-alias` and `-salias` must also be reviewed; the current code contains special disambiguation logic. Tag metadata must not accidentally break it.

### 9.2 Canonicalization of the assigned tag

For built-in tag aliases:

```text
wrn
warn
WARNING
```

refer to the same group.

The internal representation should ideally store a stable canonical identity so semantically equivalent CLI spelling does not create unnecessary new IDs.

But if the implementation uses a synthetic prefix, it must also choose a deterministic display-safe spelling. This is one reason explicit tag metadata may be cleaner.

For custom `-ulabel` values, current rules are exact/case-sensitive. Preserve that distinction.

---

## 10. Ordering relative to CE and ID assignment

This is critical.

A target design should conceptually resolve:

```text
source alias invocation
    -> identify alias and its optional assigned tag
    -> parse source CE selector prefixes
    -> determine CE rules selected by source selectors and, if adopted, alias tag
    -> construct final schema
    -> validate value arity/types
    -> assign/reuse Trice ID using the full final semantic schema
    -> emit Bind sidecar or Insert source edit
    -> write TIL/LI/build metadata
```

The alias-assigned tag must be known early enough for:

- CE selection by tag, if supported,
- tag-specific ID range selection,
- schema identity,
- diagnostics about tag-like source prefixes.

But it must not cause unwanted literal source rewriting.

### 10.1 Bind

For Bind:

- source call remains unchanged,
- generated sidecar carries the normal ID adapter and any CE adapter,
- LI should refer to the alias invocation site,
- tag assignment must survive into the build dictionary.

Test direct aliases thoroughly because `bind -ce` has stricter rules for wrappers. A **registered direct alias** should not accidentally become classified as an unsupported wrapper just because it has tag metadata.

### 10.2 Insert

For Insert:

- preserve existing alias recognition,
- insert IDs as current alias logic requires,
- CE may alter source according to current Insert semantics,
- alias tag assignment itself should not insert a literal tag into the user's source format unless the final design explicitly chooses and documents that (current preference: do not).

### 10.3 Clean

Clean must be able to reverse Insert/CE transformations using the same alias configuration.

If alias tag metadata changes how CE rules are selected, repeated Insert/Clean with the same options must remain stable and reversible.

### 10.4 Generate / generated lookup data

Review:

- `generate -logC`,
- readable JSON dictionary copies,
- any generated C tables,
- local/on-target decode tables,
- bind metadata used by `generate`.

If the tag becomes a new persistent schema field, all generated representations must either preserve it where needed or intentionally document why it is host-only.

---

## 11. Logging-side behavior that must remain coherent

Whatever storage model is chosen, all host features must observe the **same resolved event tag**.

Do not patch only JSON output.

Review at least:

- text color/prefix handling,
- `-pick`,
- `-ban`,
- `-logLevel`,
- tag statistics,
- structured JSON `tag`,
- structured JSON `level`,
- KV `tag`/`level`,
- visualization event filtering,
- target ID range assignment during instrumentation,
- any remote display mode that consumes classified application events.

Current structured logging code derives `level` from `tag`. Preserve that single source of truth.

### 11.1 Untagged backward compatibility

Old TIL entries with no explicit alias-assigned tag must continue to use today's behavior:

- recognized format-prefix tag -> that group,
- no recognized prefix -> `untagged`.

A new explicit metadata field, if adopted, should therefore be optional and have a clean fallback path.

---

## 12. Location information: no new file-ID mechanism

Several approaches were considered during analysis and should **not** become the default design:

### 12.1 Separate `files.json`

Idea: create another dictionary assigning stable IDs to source files, then transmit file ID + line.

Rejected for #531 because the normal Trice ID already maps through `li.json` to file and line. It duplicates information and adds target payload.

### 12.2 Reusing Bind sidecar file keys as transmitted file IDs

Technically possible, but unnecessary for ordinary source-location output for the same reason.

Bind sidecar file keys remain useful internally for Bind ownership, but should not be transmitted merely to reconstruct a location the host already knows from the event ID.

### 12.3 Packing `__FILE__` into multiple integer arguments

Also technically possible, especially with CE, but much worse:

- wastes payload,
- consumes argument slots,
- needs packing/unpacking conventions,
- duplicates static metadata already available in LI.

Keep this only as a niche option for genuinely dynamic/caller location that cannot be represented by the instrumented ID, not as a solution to #531.

---

## 13. Caller location: alias versus helper function versus wrapper

This distinction must be documented because it is central to #531.

### 13.1 Direct Trice or registered alias invocation

Preferred when caller location matters:

```c
MyAlarm("Temperature too high: %d", value);
```

The alias invocation should be the instrumented source site, with its own Trice ID and LI entry.

### 13.2 Ordinary helper function

Example:

```c
void log_error(int value) {
    trice32("err:Error %d", value);
}
```

All callers execute the same inner Trice site. LI points to that inner function-body site.

This is valid and simple when the helper location is sufficient, but it does not provide the application caller's file/line automatically.

### 13.3 General logging wrapper macro containing inner Trice calls

Existing Bind documentation explains that wrapper-definition sites may own the IDs, and `bind -ce` deliberately rejects some complex wrapper/rebase situations.

Do not conflate a **registered alias invocation** with a **wrapper macro containing a Trice call**. The proposed alias-tag feature is intended to enhance the former, not to remove every current Bind wrapper limitation.

---

## 14. Runtime context from #531

### 14.1 Task context

Already a good CE use case.

Conceptual example:

```sh
-ce 'wrn:", task={task:%p}", osThreadGetId()'
```

if alias-assigned tags are allowed to select CE, or alternatively use an independent source selector:

```c
MyAlarm("taskctx:Temperature too high: %d", value);
```

```sh
-ce 'taskctx:", task={task:%p}", osThreadGetId()'
```

### 14.2 Timestamp

Use existing Trice target-stamp mechanisms rather than appending a generic timestamp argument unless application semantics require a separate value.

Structured output already exports target stamp metadata separately.

### 14.3 MODULE / PROCESS

Do not automatically transmit them as runtime strings.

Possible application-level strategies:

- derive module from source path in the host consumer,
- use custom selectors with static CE text,
- use custom alias families if the project chooses,
- transmit only a compact dynamic process/task ID when process context actually changes at runtime.

Do not expand alias-to-tag into arbitrary alias-to-N-metadata unless specifically approved.

---

## 15. Runtime strings: explicit non-goal

For this task, it is acceptable and preferable to say:

> Context Enrichment of arbitrary runtime strings is not supported by this feature.

Do not redesign Trice's scalar/string record model in order to implement alias tags.

The current restrictions around `triceS`, buffers, and scalar CE values remain.

If a project needs a runtime-generated string, it can use the existing string Trice family under its existing constraints. Tagged `-salias` may still be meaningful for classification, but combining that with arbitrary scalar CE is out of scope.

---

## 16. Potential behavior matrix for `-alias Name:Tag`

Assume:

```sh
-alias MyAlarm:wrn
```

The table below describes the preferred conceptual semantics. Exact diagnostics and internal storage are still open.

| Source call | Event tag | Format text semantics | Possible build diagnostic |
|---|---|---|---|
| `MyAlarm("Too hot %d", v)` | Warning | unchanged | none |
| `MyAlarm("temperature:Too hot %d", v)` | Warning | `temperature:` available to CE; normal CE removal rules apply if selected | none |
| `MyAlarm("err:Too hot %d", v)` | Warning | `err:` does not override alias tag; remains text/selector input | suppressible warning recommended |
| `MyAlarm("wrn:Too hot %d", v)` | Warning | source text not rewritten by alias mapping | optional redundancy warning or none |
| `MyAlarm("INFO:Too hot %d", v)` | Warning | source prefix does not override alias tag | suppressible warning recommended |

Important: if the final implementation chooses a synthetic tag prefix in TIL, “format text unchanged” means **user source** unchanged; host-visible message behavior still needs careful work so synthetic metadata does not leak unexpectedly.

---

## 17. Compatibility and migration requirements

### 17.1 Existing CLI must continue to work

These existing commands must retain behavior:

```sh
trice bind -alias LOG
trice insert -alias LOG
trice clean -alias LOG
```

Tagged alias syntax is additive.

### 17.2 Old TIL -> new tool

Must work exactly as today.

If a new optional `Tag` field is introduced, its absence must fall back to format-prefix / `untagged` classification.

### 17.3 New TIL -> old tool

If TIL gains a new JSON field, old Go JSON decoding will likely ignore it structurally, but old logging behavior will not understand the assigned tag if no format prefix exists.

Document this as a host-compatibility boundary. Do not call it fully backward compatible merely because JSON parsing succeeds.

### 17.4 Historical cumulative TIL

Trice intentionally keeps old ID/template mappings so old captures remain decodable.

Changing alias tag mapping must not mutate the semantics of an old ID in place.

This is a hard requirement regardless of the chosen storage strategy.

### 17.5 ID changes

A tag change changes event semantics (filtering, severity, statistics, possibly routing). Therefore it is reasonable and probably necessary that the event receive a new schema identity / ID where historical correctness requires it.

Test this explicitly.

---

## 18. Interaction with `-ulabel`

This area needs deliberate analysis.

Existing `-ulabel` can:

- register a custom tag name,
- set/change its weight,
- set/change its color,
- participate in Bind/Insert tag handling and ID ranges.

Potential desirable syntax:

```sh
trice bind \
  -ulabel sensor:650 \
  -alias SensorAlarm:sensor
```

Questions:

1. Must `sensor` be registered with `-ulabel` before it can be used on the right side of `-alias`?
2. Should an unknown alias tag be rejected with a hint to `-ulabel`?
3. Is CLI option order irrelevant, as it is for current label/filter resolution?
4. If TIL stores only the tag name, does a later `trice log` also require `-ulabel sensor:650` to know its weight/color? Current custom-tag philosophy suggests yes unless metadata is expanded further.
5. Should `-alias X:untagged` be allowed explicitly?

Recommended principle:

> Reuse the existing tag registry for validation and weight/color semantics. Do not create a second independent alias severity registry.

But keep alias-to-tag mapping persistent enough that the event's group identity survives later decoding.

---

## 19. Interaction with tag-specific ID ranges / target routing

Assigned alias tags should behave like ordinary source-format tags for instrumentation-time policies.

If existing configuration says a Warning group uses a particular ID range or route, then:

```sh
-alias MyAlarm:wrn
```

should naturally participate in that policy.

Do not use the numeric range itself as the persisted tag representation.

Review ordering carefully: assigned tag must be resolved before ID-range allocation.

---

## 20. Interaction with presentation and `-color`

This section is especially important if the synthetic-prefix TIL approach is chosen.

Current behavior distinguishes:

- `-color default` / palettes: recognized lowercase source tag prefixes can be removed and colorized,
- `-color none`: recognized lowercase prefixes are removed without colors,
- `-color off`: written prefixes remain visible.

For alias-assigned tags, the user may not have written any prefix at all.

Therefore an implementation must answer:

> Should `-color off` cause a synthetic `wrn:` to appear for `MyAlarm("Too hot")`?

Preferred answer: **No**, because that text was not part of the source message.

This preference strongly favors explicit tag metadata or an internal marker that distinguishes synthetic classification from user text. If the virtual-prefix approach is chosen, make sure presentation does not unexpectedly expose implementation metadata.

Likewise, a genuine source prefix that conflicts with the alias tag should remain user text unless existing CE transformation deliberately removes it.

---

## 21. Structured output requirements

For an alias-tagged event, JSON should conceptually look like:

```json
{
  "tag": "WARNING",
  "level": "WARNING",
  "message": "Temperature too high: 85 degrees",
  "file": "src/temperature.c",
  "line": 137,
  "fields": {
    "value": 85
  }
}
```

If CE adds task/limit fields, they should appear in the existing user-field namespace according to current Structured Logging rules.

Do not add alias names to each record unless there is a separate justified requirement. The alias name is source/instrumentation metadata, not necessarily runtime log semantics.

---

## 22. Selective file/line for errors: likely no implementation required for #531

The original user asked for file/line only on errors.

Because v2 structured output already provides file/line as separate host metadata, the simplest answer is:

- let Trice resolve LI normally,
- let the downstream consumer display source location only for chosen tags/levels.

If the project later wants `trice log` / `tlog` itself to omit location fields for other events, that can be a small independent host-output feature. Do not entangle it with alias tagging.

Possible future concept (not a requirement): a location-output selector analogous to `-pick`/`-logLevel`.

---

## 23. Test plan

Codex should create/extend tests before considering the feature complete.

### 23.1 CLI parsing

Test:

```text
-alias LOG
-alias LOG:wrn
-alias LOG:WARNING
-alias LOG:customTag (with matching -ulabel)
```

Invalid:

- empty alias,
- empty tag,
- unknown tag if registration is required,
- malformed multiple-colon form,
- conflicting duplicate alias mappings.

### 23.2 Canonical equivalence

Built-in equivalents:

```text
wrn
warn
WARNING
```

should resolve to the same semantic group and should not cause arbitrary ID churn purely because CLI spelling differs.

### 23.3 Bind direct alias

Example:

```c
MY_ALARM("Temperature %d", value);
```

Verify:

- source remains unchanged,
- alias invocation receives an ID,
- LI points to invocation line,
- TIL/schema persists assigned tag by the chosen mechanism,
- decode yields Warning tag/level.

### 23.4 Insert / Clean

Verify round trip:

```text
original
-> insert with alias tag
-> clean with same options
-> original
```

Tag assignment must not inject residual source text.

### 23.5 Alias tag + CE source selector

```c
MY_ALARM("temperature:Temperature %d", value);
```

with:

```sh
-alias MY_ALARM:wrn
-ce 'temperature:", limit={limit:%d}", limit'
```

Verify:

- tag = Warning,
- CE applies exactly once,
- source selector behaves according to existing removal rules,
- fields decode correctly,
- LI remains the alias call site.

### 23.6 Alias tag + CE by assigned tag

If adopted:

```sh
-alias MY_ALARM:wrn
-ce 'wrn:", task={task:%p}", osThreadGetId()'
```

Verify Warning-wide CE applies even without literal `wrn:` in source.

### 23.7 Conflicting tag-like source prefix

```c
MY_ALARM("err:Temperature %d", value);
```

with `-alias MY_ALARM:wrn`.

Verify:

- event remains Warning,
- diagnostic appears if enabled,
- warning suppression removes only the diagnostic,
- `err:` treatment as text/CE selector follows documented decision,
- no hard error merely because the prefix differs.

### 23.8 Historical ID behavior

Build A:

```sh
-alias MY_ALARM:wrn
```

Build B:

```sh
-alias MY_ALARM:err
```

Verify old capture/dictionary semantics are preserved and not silently rewritten.

### 23.9 Structured output

Verify JSON/KV:

- correct `tag`,
- correct derived `level`,
- correct `message`,
- `file` and `line` from LI,
- CE fields,
- no synthetic prefix leakage into `message` unless explicitly intended.

### 23.10 Text output and colors

Check at least:

```text
-color default
-color none
-color off
```

especially if using a synthetic internal prefix.

### 23.11 Filtering and statistics

Verify alias-tagged events participate correctly in:

- `-pick wrn`,
- `-ban wrn`,
- `-logLevel wrn`,
- tag statistics,
- weight overrides.

### 23.12 `-ulabel`

If custom alias tags are supported, verify exact-case semantics and weights.

### 23.13 ID range / routing

Verify assigned alias tag participates in tag-specific ID range selection exactly like a normal format-string tag.

### 23.14 Backward compatibility

- existing untagged aliases with no `:Tag`,
- ordinary Trice calls,
- old TIL entries,
- aliases shared between current Insert/Bind test corpora.

### 23.15 `-salias`

If included, test tag classification while keeping the current restriction that runtime string records do not gain arbitrary scalar CE values.

---

## 24. Candidate implementation areas to inspect

Do not assume these are the only files; use code search.

Likely relevant areas in `v2.0.0-beta-1` / current main:

- `internal/args/init.go`
  - CLI definitions for `-alias`, `-salias`, `-ulabel`.
- `internal/id/vars.go`
  - current `TriceAliases` / `TriceSAliases` storage.
- file containing `resolveTriceAlias` and `ProcessAliases`
  - current alias type conversion and regex construction.
- `internal/id/bindAdvanced.go`
  - alias/wrapper distinction.
- `internal/id/contextEnrichment.go`
  - CE selector resolution and final schema construction.
- `internal/id/contextSource.go`
  - Insert/Clean CE processing.
- `internal/id/id.go`
  - `TriceFmt`, TIL representation, LI types.
- ID allocation/reverse lookup files
  - schema identity and historical reuse.
- `internal/translator/structured.go`
  - structured tag/message/level rendering.
- `internal/translator/translator.go`
  - text event filtering/presentation and LI.
- `internal/emitter/*`
  - tag registry, weights, aliases, colors, event classification.
- generated dictionary / `generate -logC` paths.
- alias tests:
  - `internal/id/manage_test.go`
  - `internal/id/bindIntegration_test.go`
  - `internal/id/bindIDs_test.go`
  - `internal/id/contextSource_test.go`
  - relevant CLI integration tests.

Repository agent rules require focused diffs and English comments for new code. Consult `AGENTS.md` before editing.

---

## 25. Design questions Codex must answer explicitly before a large implementation

Produce a short design conclusion covering these points:

1. **Persistence:** Is an optional TIL `Tag` field necessary, or can existing format/tag machinery represent the mapping without bad presentation/compatibility effects?
2. **Schema identity:** How will a tag mapping change force safe historical semantics?
3. **Canonical tag identity:** How are `wrn`, `warn`, `WARNING`, and custom `-ulabel` tags normalized?
4. **CE:** Does the alias-assigned tag itself select `-ce` rules?
5. **Source prefix:** What exactly happens if the source begins with `err:` while alias tag is Warning?
6. **Diagnostics:** What warning is emitted and how is it suppressed using a Trice-consistent mechanism?
7. **Text presentation:** How do we ensure synthetic metadata never unexpectedly appears with `-color off`?
8. **Custom tags:** Can `-alias X:sensor` use `-ulabel sensor:650`; what must later logging commands know?
9. **ID ranges:** Does assigned tag participate in `-IDRange` before allocation?
10. **Bind/Insert/Clean:** Can the same semantics be provided in all workflows without source corruption?
11. **Generate/local decode:** Does on-target/generated logging need tag metadata too?
12. **Compatibility:** What happens with new TIL + old host and old TIL + new host?
13. **`-salias`:** include now or defer?
14. **Warnings:** restrict tag-like-prefix warnings to built-in severity tags or broader registered tag groups?

Prefer the smallest design that preserves Trice's existing concepts rather than adding parallel mechanisms.

---

## 26. Suggested staged implementation strategy

### Stage 1 — Design proof / tests

Before modifying persistent schema, create focused tests or a small proof showing:

- existing alias invocation location behavior,
- how current tag extraction sees an effective synthetic prefix,
- `-color off` behavior,
- CE prefix-chain behavior with `wrn:temperature:...`,
- custom `-ulabel` casing.

Use those results to choose TIL metadata vs synthetic-prefix approach.

### Stage 2 — Alias CLI parser and internal representation

Add optional tag association while preserving legacy alias syntax.

No name inference.

### Stage 3 — Bind/Insert/Clean integration

Resolve tag before ID allocation and CE decisions where needed.

Keep source stable with respect to alias tag assignment.

### Stage 4 — Persistence and logging

Implement the chosen persistent representation and make all host classification consumers use one resolved tag.

### Stage 5 — Diagnostics

Add conservative, suppressible warnings for misleading source prefixes.

### Stage 6 — Examples and docs

Add one small hardware-free example, ideally extending `PC_features` or a focused alias example, demonstrating:

```c
MyAlarm("temperature:Temperature too high: %d degrees", value);
```

with:

```sh
-alias MyAlarm:wrn
-ce 'temperature:", limit={limit:%d}", temperatureLimit'
```

and JSON output with:

- Warning tag/level,
- call-site file/line,
- value,
- enriched context.

Do not make the example depend on runtime strings.

---

## 27. Potential user documentation — draft text (English)

The following is deliberately close to publishable documentation. Adjust only after the final design is known.

### 27.1 Draft: Tagged Trice aliases

#### Tagged aliases

Project-specific logging names can be registered as Trice aliases. An alias can optionally be assigned an application tag so the source call does not need to repeat that tag in every format string.

```sh
trice bind -alias MyAlarm:wrn -src app -til til.json -li li.json
```

The source can then use:

```c
MyAlarm("Temperature too high: %d degrees\n", temperature);
```

`MyAlarm` is treated as a normal Trice source site, while its configured `wrn` tag classifies the event as Warning. The alias name itself has no special meaning: Trice does not infer severity from names such as `Alarm`, `Error`, or `Debug`.

The assigned tag is compile-time metadata. It does not add a tag string or another runtime value to the target record. The normal Trice ID still identifies the source location through the matching `li.json`.

Existing aliases without an assigned tag keep their previous behavior:

```sh
trice bind -alias LegacyLog ...
```

### 27.2 Draft: Tags and format-string selectors are independent

An assigned alias tag does not prevent the format string from using Context Enrichment selectors:

```c
MyAlarm("temperature:Temperature too high: %d degrees\n", temperature);
```

```sh
trice bind \
  -alias MyAlarm:wrn \
  -ce 'temperature:", limit={limit:%d}", temperatureLimit' \
  -src app -til til.json -li li.json
```

Here `wrn` classifies the event, while `temperature:` selects additional context. Context Enrichment follows its normal selector and source-rewrite rules; the alias tag itself does not require a literal `wrn:` prefix in the source.

[If implemented:] An alias-assigned tag can also act as a CE selector. This is useful for context that should be added to every Warning event:

```sh
trice bind \
  -alias MyAlarm:wrn \
  -ce 'wrn:", task={task:%p}", osThreadGetId()' ...
```

### 27.3 Draft: Source location

A tagged alias remains an ordinary instrumented source site. Its Trice ID is resolved through `li.json`, so file and line information do not need to be transmitted as runtime strings.

This differs from a helper function containing one shared Trice call. In that case the Trice source site belongs to the helper function body, so its location identifies the helper rather than each caller. Use a registered alias when the application call site itself should own the Trice ID and location.

### 27.4 Draft: Tag-like text inside a tagged alias

[Final wording depends on the chosen warning design.]

When an alias already has an assigned tag, a tag-looking prefix in its source format string does not override the configured alias tag. For example, with:

```sh
-alias MyAlarm:wrn
```

this call is still a Warning event:

```c
MyAlarm("err:Temperature too high: %d", temperature);
```

Trice may issue a build warning because the text can be misleading. The warning can be suppressed when the prefix is intentional. Context Enrichment selector handling remains independent.

### 27.5 Draft: Custom tags

[Only publish if supported.]

A tagged alias may use a registered user label:

```sh
trice bind \
  -ulabel sensor:650 \
  -alias SensorAlarm:sensor ...
```

The alias belongs to the `sensor` group. Its derived structured level follows the configured weight using the normal Trice level boundaries.

### 27.6 Draft: Runtime context

Context that changes at runtime should be added as values rather than repeated text. For example, a project can enrich selected events with a task handle:

```sh
-ce 'ctx:", task={task:%p}", osThreadGetId()'
```

The task value is transmitted in the same Trice event as the normal message arguments. Source file and line remain host-side metadata from the Trice ID and `li.json`.

### 27.7 Draft: Runtime strings are not Context Enrichment values

Tagged aliases do not change the Trice runtime-string model. String and buffer families keep their existing restrictions, and Context Enrichment does not append arbitrary scalar runtime values to those records. Use ordinary scalar Trice calls for the tagged-alias + scalar-context workflow described here.

---

## 28. Potential response to Discussion #531 after implementation — draft

Do not post this yet; update syntax and claims after implementation/tests.

> Trice 2.0 now covers most of this use case differently from the state of the project when this discussion was opened.
>
> A received Trice ID already resolves to file and line through the matching `li.json`, so source names do not need to be transmitted as runtime strings. Structured Logging can export the event as JSON/KV with separate tag, level, message, source location, timestamps, and named values. Context Enrichment can add runtime values such as the current FreeRTOS task to the same event.
>
> For project-specific logging names, tagged aliases allow a call such as:
>
> ```c
> MyAlarm("temperature:Temperature too high: %d degrees", value);
> ```
>
> to be configured with:
>
> ```sh
> -alias MyAlarm:wrn
> ```
>
> so the alias is classified as Warning without repeating `wrn:` in every source format. `temperature:` remains available independently as a Context Enrichment selector. The alias invocation owns the normal Trice ID/location, unlike a shared helper function whose inner Trice call would identify the helper body.
>
> Runtime strings remain subject to the existing Trice string-record restrictions; the intended workflow here is scalar Trice events plus scalar Context Enrichment.
>
> Please try this with `v2.x` and the structured logging examples and let us know whether your host program can consume JSON/KV directly or whether you still need the old marker layout.

---

## 29. Rejected / deprioritized solution ideas from the design discussion

Keep these only as historical context; do not implement them unless a new requirement appears.

### Separate stable source-file IDs / `files.json`

Rejected because Trice ID + `li.json` already provides source location.

### Transmitting Bind sidecar file keys

Rejected for the same reason; adds unnecessary payload.

### Packing filename characters into `uint64_t` CE values

Technically feasible but wasteful and unnecessary for static source location.

### Automatically inferring severity from alias names

Rejected. Explicit CLI mapping is clearer, deterministic, and project-independent.

### General `log_error()` helper function as the main solution

Valid for some code, but not sufficient when LI must identify each application caller.

### Multiple Trice calls for context + message

No longer preferred for the normal scalar case because CE and Structured Logging can keep logical context in one event. Multiple-call composition also reintroduces ordering/preemption concerns.

### Solving runtime-string + arbitrary scalar CE now

Out of scope. Do not let this block the alias-tag feature.

---

## 30. Acceptance criteria for the final feature

The work is successful when all of the following hold:

1. Existing `-alias Name` behavior is unchanged.
2. A project can explicitly associate a tag with an alias, preferably through the existing option, e.g. `-alias MyAlarm:wrn`.
3. No alias-name heuristic exists.
4. Alias source format strings do not need to contain the assigned tag.
5. Alias tag assignment itself does not rewrite the user's format string.
6. The event tag is durable enough for saved-capture decoding without requiring the original alias CLI to be repeated at log time.
7. Changing the assigned tag cannot silently reinterpret historical IDs.
8. The assigned tag participates coherently in filtering, structured output, colors, statistics, and tag-specific ID policies.
9. Direct alias invocation retains correct LI call-site location.
10. CE selectors in the source remain independent from alias tag classification.
11. If adopted, the assigned alias tag can select CE rules without duplicate application.
12. A conflicting tag-looking source prefix does not override the alias tag; it may produce a suppressible diagnostic.
13. `-ulabel` integration is defined and tested, not accidentally duplicated.
14. No new target runtime bytes are required merely for alias tag classification.
15. No new binary Trice wire format is required.
16. Runtime strings through CE remain explicitly out of scope.
17. Documentation includes a complete example relevant to #531.
18. Tests cover Bind and, where supported, Insert/Clean round trips and historical ID behavior.

---

## 31. Sources / baseline files reviewed for this handoff

Primary external context:

- Discussion #531: https://github.com/rokath/trice/discussions/531
- Release `v2.0.0-beta-1`: https://github.com/rokath/trice/releases/tag/v2.0.0-beta-1

Relevant v2.0.0-beta-1 documentation:

- `docs/rm/bind.md`
- `docs/rm/cli.md`
- `docs/rm/context_enrichment.md`
- `docs/rm/examples.md`
- `docs/rm/id_management.md`
- `docs/rm/legacy_user_code.md`
- `docs/rm/structured_logging.md`
- `docs/rm/tags_color_and_weights.md`

Relevant implementation areas inspected during discussion:

- `internal/id/id.go`
- current file containing `resolveTriceAlias` / `ProcessAliases`
- `internal/id/contextEnrichment.go`
- `internal/translator/structured.go`
- `internal/translator/translator.go`
- `internal/args/init.go`
- `internal/id/bindAdvanced.go`
- alias and CE tests under `internal/id` and `internal/args`

Also read repository agent guidance:

- `AGENTS.md`

---

## 32. Final guidance to Codex

Start from the current repository state, but use `v2.0.0-beta-1` as the behavioral baseline described here. First verify whether `main` has already changed any relevant alias/tag/CE internals.

Do **not** jump immediately to adding `Tag` to `til.json`. Build a small design proof comparing explicit metadata versus synthetic effective-format prefixing, especially around:

- `-color off`,
- custom `-ulabel` casing,
- historical IDs,
- CE selector chaining,
- old/new dictionary compatibility.

Then state the chosen design and why it is simpler/safer before making a broad change.

The intended user-facing model should remain simple:

```sh
-alias MyAlarm:wrn
```

```c
MyAlarm("temperature:Temperature too high: %d degrees", value);
```

with Trice providing:

- Warning classification,
- normal call-site ID and `li.json` location,
- optional CE runtime context,
- structured host output,
- no repeated tag text in source,
- no file-name runtime transmission,
- no alias-name heuristics.

Keep the implementation focused. Do not turn this into a general metadata language or runtime-string redesign unless a concrete requirement makes that unavoidable.
