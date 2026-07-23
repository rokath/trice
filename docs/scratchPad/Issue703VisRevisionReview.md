# Feature request: configurable -vis output transformation for dynamic data visualization

<!--
Review copy of https://github.com/rokath/trice/issues/703.
The GitHub issue itself is unchanged.

Review convention:
- Text between >DELETE: BEGIN and >DELETE: END exists in the current issue and is proposed for deletion.
- Text between >NEW: BEGIN and >NEW: END is proposed replacement or additional issue text.
- Unmarked text is copied unchanged from the current issue.
-->

<h2>Table of Contents</h2>
<!-- mdtoc -->

- [1. Summary](#summary)
- [2. Motivation](#motivation)
- [3. Design goals](#design-goals)
- [4. Proposed CLI syntax](#proposed-cli-syntax)
- [5. Matching rule](#matching-rule)
- [6. LUT compatibility and runtime eligibility](#lut-compatibility-and-runtime-eligibility)
- [7. Target-Stamp handling](#target-stamp-handling)
- [8. Internal field names](#internal-field-names)
- [9. No value-name extraction in the first implementation](#no-value-name-extraction-in-the-first-implementation)
- [10. Expressions in printf(...)](#expressions-in-printf)
- [11. Reordering values](#reordering-values)
- [12. Compatibility expectation for one tag](#compatibility-expectation-for-one-tag)
- [13. printf(...) formatter](#printf-formatter)
- [14. Sink/address syntax](#sinkaddress-syntax)
  - [14.1. Plain file path](#plain-file-path)
  - [14.2. UDP](#udp)
  - [14.3. Other sinks](#other-sinks)
- [15. log=keep and log=drop](#logkeep-and-logdrop)
- [16. Interaction with normal lower-case tag stripping](#interaction-with-normal-lower-case-tag-stripping)
- [17. Examples](#examples)
  - [17.1. LabPlot CSV file](#labplot-csv-file)
  - [17.2. LabPlot or Serial Studio UDP stream with scaling](#labplot-or-serial-studio-udp-stream-with-scaling)
  - [17.3. JSON line in the printf format](#json-line-in-the-printf-format)
  - [17.4. Two outputs from the same tag](#two-outputs-from-the-same-tag)
  - [17.5. Different tags](#different-tags)
  - [17.6. Reordered output](#reordered-output)
  - [17.7. No Target-Stamp: runtime ignore for a stamp-dependent rule](#no-target-stamp-runtime-ignore-for-a-stamp-dependent-rule)
  - [17.8. Separate explicit 16- and 32-bit Target-Stamp rules](#separate-explicit-16--and-32-bit-target-stamp-rules)
- [18. Startup validation requirements](#startup-validation-requirements)
- [19. Startup validation requirements](#startup-validation-requirements-1)
- [20. Concrete implementation plan](#concrete-implementation-plan)
  - [20.1. CLI collection and lifetime](#cli-collection-and-lifetime)
  - [20.2. Decoder-local typed record](#decoder-local-typed-record)
  - [20.3. Translator integration and filtering order](#translator-integration-and-filtering-order)
  - [20.4. Rule and expression model](#rule-and-expression-model)
  - [20.5. Record-oriented sinks](#record-oriented-sinks)
  - [20.6. Rule eligibility and disable behavior](#rule-eligibility-and-disable-behavior)
  - [20.7. No changes outside the MVP](#no-changes-outside-the-mvp)
  - [20.8. Expected change footprint](#expected-change-footprint)
- [21. Testing requirements](#testing-requirements)
  - [21.1. CLI and rule-parser tests](#cli-and-rule-parser-tests)
  - [21.2. LUT validation tests](#lut-validation-tests)
  - [21.3. TREX record-capture tests](#trex-record-capture-tests)
  - [21.4. Single-line eligibility tests](#single-line-eligibility-tests)
  - [21.5. Expression and formatting tests](#expression-and-formatting-tests)
  - [21.6. Target-Stamp tests](#target-stamp-tests)
  - [21.7. Sink tests](#sink-tests)
  - [21.8. Filter and normal-output tests](#filter-and-normal-output-tests)
  - [21.9. Integration tests](#integration-tests)
- [22. Documentation requirement](#documentation-requirement)
- [23. Non-goals for the first implementation](#non-goals-for-the-first-implementation)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

This issue proposes a repeatable -vis option for tlog (`trice log`) to transform selected tagged Trice messages into visualization-friendly output streams. The goal is not to add graphics capabilities to Trice itself, but to provide a small, generic host-side adapter layer for external tools such as LabPlot, uPlot, Serial Studio, Grafana, or custom viewers.

The target-side Trice format string should remain simple and tool-independent. It only marks a message as visualization-relevant and defines the transmitted values. Target-stamp selection, value ordering, scaling, output formatting, sink address, and optional suppression from normal log output are configured on the tlog command line. A target stamp is treated as an unscaled numeric value; it is not assumed to represent time.

Simple CSV Output Example:

```C
TRice("msg:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
```

```bash
tlog ... -vis='msg:printf("%8d,%0.3f,%0.3f,%0.3f\n", ts*0.123, v0-1000, v1, v2/3.14)@udp://127.0.0.1:7010;log=drop'
```

Hint: The `%d` format specifier demands, that the expression `ts*0.123` needs a cast to int64. It would be possible to write `int64(ts*0.123)` instead but this th Go specific syntax. Also `(int64)(ts*0.123)` would be an option. For the sake of simplicity we allow just `ts*0.123` and take the risk of a silent user spcific mistake.

This keeps visualization-specific decisions out of target firmware while allowing the same Trice messages to be reused with different output formats and visualization tools.

## 1. <a id="summary"></a>Summary

Add a repeatable `-vis` command line option to `tlog` for transforming selected decoded Trice messages into visualization-friendly output streams.

The goal is to support simple demo integrations with tools such as LabPlot, Serial Studio, uPlot, PlotJuggler, Grafana, or plain files without encoding visualization addresses or tool-specific output formatting into the embedded target-side Trice format strings.

The first implementation supports a generic `printf(...)` encoder with simple arithmetic expressions, the fields `id`, `ts`, `ts16`, `ts32`, and `v0...v11`, and plain-file and UDP sinks. It supports fixed-width numeric TREX messages only. Value-name extraction, other decoders, multi-Trice log lines, additional sinks, and specialized encoders are explicitly deferred. The internal selector, encoder, sink, and output-policy concepts remain separate so that these extensions can be added later without changing the target-side Trice messages.

## 2. <a id="motivation"></a>Motivation

Trice already carries structured binary data on the target side. `tlog` can decode the Trice Binary Data (TBD) stream and knows the Trice ID, the original format string, the target-stamp class, and the typed parameter values before they are converted into normal log text. This is enough to create visualization-oriented output streams without parsing the already formatted log text.

For example, a target-side Trice message like this:

```c
TRice("msg:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
```

could be transformed by `tlog` into CSV lines for LabPlot:

```text
123456,0.012,-0.034,9.810
```

or into key-value text for another tool, or later into a binary WebSocket stream for uPlot.

The target-side Trice format string should not contain socket addresses, file names, LabPlot-specific information, Serial Studio configuration, or other visualization sink configuration. These belong to the host-side `tlog` command line or to future host-side configuration files, even if the target-side format string text itself is not transported in the runtime TBD stream.

## 3. <a id="design-goals"></a>Design goals

- One generic and repeatable CLI option: `-vis`.
- Multiple `-vis` options may be used at the same time.
- One decoded Trice message may be transformed to multiple outputs.
- No visualization destination address in the Trice target format string.
- No need for special hard-coded tags such as `plot_` or `vis_labplot`.
- Any suitable tag prefix can be used, for example `draw:`, `imu:`, `pow:`, or `vis_xyz:`.
- The tag selects messages for a `-vis` rule.
- The `-vis` rule defines transformation, destination, and optional normal-log behavior.
- `tlog` must validate the syntax of all `-vis` CLI strings before decoding starts and validate each rule against all tag-matching entries currently present in `til.json`.
- Existing `-pick` and `-ban` processing runs before `-vis`. A message removed by `-pick` or `-ban` is invisible to `-vis` as well as to normal output.
- The normal `tlog` output remains unchanged unless at least one matching, successful `-vis` rule explicitly uses `log=drop`.
- A `-vis` rule is never disabled silently. Every rule disable action emits a prominent non-fatal error identifying the rule, the reason, and affected IDs when known; normal logging continues.
- Only fixed-length numeric Trice messages are supported by the first implementation.
  - Examples: `trice` and `trice8_3` are both fixed-length messages, but `triceB` and `triceX0` are not.
  - Trice messages carry only fixed-width values, but the optional target stamps can have a different width.
- Every visualization record must originate from one self-contained Trice message forming one complete log line. Other messages remain valid normal logs but are ignored by `-vis`.

## 4. <a id="proposed-cli-syntax"></a>Proposed CLI syntax

Extensible conceptual form:

```text
-vis='<selector>:<encoder>(<encoder-arguments>)@<sink>[;<option>=<value>...]'
```

First-implementation form:

```text
-vis='<tag>:printf("<go-fmt>",<expr-list>)@<file-path-or-udp-sink>[;log=keep|drop]'
```

The first implementation has one selector kind (the tag prefix like `"draw:...`), one encoder (`printf`), two sink kinds (plain file and UDP), and one optional output policy (`log=keep|drop`). The parser and internal model must nevertheless keep these four concepts separate.

Examples:

```bash
tlog ... \
  -vis='msg:printf("%d,%f,%f,%f\n",ts,v0,v1,v2)@out.csv'
```

```bash
tlog ... \
  -vis='msg:printf("%8d,%0.3f,%0.3f,%0.3f\n",ts32/10,v0*0.5,v1*100,20.0*v2-1000)@udp://127.0.0.1:7010;log=drop'
```

```bash
tlog ... \
  -vis='imu:printf("%d,%f,%f,%f\n",ts32,v0,v1,v2)@file:logs/imu.csv' \
  -vis='imu:printf("%d,%f,%f,%f\n",ts32,v0,v1,v2)@udp://127.0.0.1:7010'
```

The part before the first colon in the decoded Trice format string is the selectable tag. For example:

```c
TRice("draw:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
```

is selected by:

```text
-vis='draw:printf(...)@...'
```

A special `vis_` prefix is not required. It may be used by convention, but it is not part of the core mechanism.

## 5. <a id="matching-rule"></a>Matching rule

A `-vis` rule matches a decoded Trice log entry if the decoded Trice format string starts with:

```text
<tag>:
```

Example match:

```text
msg:ax=%f,ay=%f,az=%f\n
```

for:

```text
-vis='msg:printf(...)@...'
```

Existing `-pick` or `-ban` filtering is applied before this matching step. If multiple enabled `-vis` rules match the remaining decoded message, every eligible transformation is attempted. A rule is eligible only if its explicit target-stamp requirement is met and the message is one supported fixed-width numeric TREX message forming one complete log line.

## 6. <a id="lut-compatibility-and-runtime-eligibility"></a>LUT compatibility and runtime eligibility

At startup, each syntactically valid `-vis` rule is checked strictly against every historical `til.json` entry whose original format string starts with the selected `<tag>:` prefix.

For every such entry, `tlog` checks that:

- the entry describes a supported numeric Trice message;
- every referenced `v0...v11` exists;
- the referenced value types and direct-field types are compatible with the corresponding Go format verbs;
  - Hint: `%u` format specifiers are allowed in Trice format strings. For the MVP these are not allowed in -vis.
- the complete rule can be evaluated without extracting names from the human-readable format text.

If any matching historical entry is incompatible, the complete rule is disabled. `tlog` emits a prominent non-fatal error containing the rule, the reason, and all affected IDs. The user may remove obsolete entries from `til.json` and restart tlog. A disabled rule is not silently or automatically re-enabled during the same process.

>DISCUSSION: BEGIN

Is it better to not disable the rule instead? Legacy til.json entries may disturb then more than wanted.

>DISCUSSION RESPONSE:

Yes. A tag match should make a `til.json` entry a candidate, not force every historical entry with that tag to be compatible with every rule. At startup, each rule should build its set of compatible IDs and ignore incompatible candidate IDs. The rule remains enabled when at least one compatible ID exists. If no compatible ID exists at all, the rule is disabled with the already agreed prominent diagnostic because it cannot produce output with the current `til.json`.

This avoids legacy entries disabling useful rules. In normal mode, excluded candidate IDs are silent; verbose mode can list them together with the reason. The exact, deliberately simple compatibility signature is proposed in the later discussion about parameter counts. This decision would require corresponding changes in sections 6, 12, 19, 20.6, and the related tests.

>DISCUSSION: END

When `til.json` is updated while tlog is running, a new or changed matching ID is validated when a corresponding runtime record is first observed. An incompatible entry disables the rule with the same prominent diagnostic. Unused historical entries added during the run need not be detected until the next tlog start, where the complete strict scan runs again. Normal logging continues.

The Target-Stamp width is not reliably available in `til.json`; it is encoded in each runtime TREX record. Therefore target-stamp eligibility is handled at runtime:

- a rule referencing `ts16` processes only 16-bit-stamped records;
- a rule referencing `ts32` processes only 32-bit-stamped records;
- records with no stamp or the other explicit width are ignored by that rule;
- a rule referencing generic `ts` records the width of its first matching stamped record and is disabled with a prominent diagnostic if a later matching record has the other width.

Unsupported multi-Trice or multi-line records and explicit target-stamp-width mismatches are ignored silently during normal operation. The existing verbose mode reports every such ignored record. Ignoring one record never disables a rule.

`log=drop` applies only after a record was successfully encoded and accepted by at least one matching `log=drop` rule. An ignored or failed record remains available to normal logging.

## 7. <a id="target-stamp-handling"></a>Target-Stamp handling

The fields `ts`, `ts16`, and `ts32` refer to the unmodified numeric Target-Stamp transported in the TREX record. The stamp is not assumed to represent time. Its unit and meaning are determined entirely by the target application and the user's `-vis` expression.

Rules:

- `TRice*(...)` and `TRICE*(ID(n), ...)` provides a 32-bit Target-Stamp.
- `Trice*(...)` and `TRICE*(Id(n), ...)` provides a 16-bit Target-Stamp.
- `trice*(...)` and `TRICE*(id(n), ...)` provides no Target-Stamp.
- A rule may reference `ts16` or `ts32`, but not both.
- `ts16` makes only 16-bit-stamped records eligible for that rule.
- `ts32` makes only 32-bit-stamped records eligible for that rule.
- A rule referencing generic `ts` accepts its first matching stamped width and remembers that width.
- If the same generic-`ts` rule later receives the other stamp width, the rule is disabled with a prominent non-fatal error containing the rule and affected IDs. Normal logging continues.
- A record without a Target-Stamp is ignored by a rule that references `ts`, `ts16`, or `ts32`.
- The first implementation performs no scaling, normalization, unit conversion, wrap extension, reset detection, or reconstruction of mixed 16-/32-bit stamp sequences.
- The optimization where a 16-bit stamp represents the low bits of the most recently transmitted 32-bit stamp is explicitly unsupported by `-vis`.
- A future general tlog Target-Stamp scaling feature is independent of `-vis` and must not silently change the meaning of these raw `-vis` fields.

Users apply any required scaling explicitly, for example `ts32*0.001`. A Target-Stamp may also deliberately represent a non-time x or y coordinate.

>DISCUSSION: BEGIN

ID, Id and id are not visible in til.json, so some errors are detectable only during logging. What do you propose?

>DISCUSSION RESPONSE:

Do not extend or reinterpret `til.json` for this MVP. Target-Stamp width remains a runtime property:

- `ts16` and `ts32` are runtime eligibility predicates. A record with no stamp or the other width is ignored; verbose mode reports it.
- Generic `ts` remembers the first observed width for this rule. A later matching record with the other width emits the prominent diagnostic and disables the rule.
- An unstamped record is ignored by every stamp-dependent rule.

Consequently, using `Trice` instead of `TRice`, or `TRICE(Id(...))` instead of `TRICE(ID(...))`, cannot be detected at startup from `til.json`. It is detected only when such a record arrives. This limitation should be stated explicitly rather than approximated from the format type. If explicit-width mismatches should always warn rather than warn only in verbose mode, that would be a separate change to the already agreed diagnostic policy; my recommendation remains the less noisy verbose-only diagnostic.

>DISCUSSION: END

## 8. <a id="internal-field-names"></a>Internal field names

For every matching decoded Trice message, `tlog` exposes this deliberately small field set to the `printf` encoder:

```text
id       Trice ID with its unsigned integer type preserved
ts       unmodified 16- or 32-bit Target-Stamp, with one width allowed per rule
ts16     unmodified 16-bit Target-Stamp; other widths make the record ineligible
ts32     unmodified 32-bit Target-Stamp; other widths make the record ineligible
v0       first typed Trice parameter value
v1       second typed Trice parameter value
v2       third typed Trice parameter value
...
v11      twelfth typed Trice parameter value
```

There are no `ts_raw` or `ts_bits` fields in the first implementation because `ts`, `ts16`, and `ts32` already expose the unmodified value and constrain its width.

Only `v0` through `v11` should be supported initially. Do not add `p0...p11` synonyms. The 0-based numbering is preferred because it maps directly to Go/C-style parameter arrays and avoids off-by-one conversion in the implementation.

The `vN` names are positional. They refer to the values in the order encoded by the decoded Trice message.

Example:

```c
TRice("msg:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
```

Available positional fields:

```text
v0 = ax
v1 = ay
v2 = az
```

## 9. <a id="no-value-name-extraction-in-the-first-implementation"></a>No value-name extraction in the first implementation

The first implementation does not derive names such as `ax`, `y1`, or `motor_rpm` from human-readable `name=%...` fragments. Expressions use only `id`, `ts`, `ts16`, `ts32`, and `v0...v11`. This avoids an additional format-string grammar, ambiguity checks, historical-name consistency rules, and associated tests. Named value references may be added later without changing positional fields.

## 10. <a id="expressions-in-printf"></a>Expressions in `printf(...)`

The `printf(...)` argument list should allow simple arithmetic expressions, not only direct field names.

Example:

```bash
-vis='msg:printf("%8d,%0.3f,%0.3f,%0.3f\n",ts32/10,v0*0.5,v1*100,20.0*v2-1000)@udp://127.0.0.1:7010;log=drop'
```

Supported expression elements for the first implementation:

```text
fields:      id, ts, ts16, ts32, v0...v11
constants:   10, -10, 10.0, 0x20
operators:   +  -  *  /
parentheses: ( ... )
unary minus: -x
```

The implementation parses expressions with Go's standard `go/parser.ParseExpr` and accepts only identifiers, supported numeric literals, parentheses, unary minus, and the four listed binary operators. Function calls, selectors, indexing, assignments, and all other Go syntax are rejected.

Type rules are deliberately small:

- A direct field reference without an arithmetic operator preserves its decoded type and exact value.
- A direct integer or floating-point literal retains the corresponding literal category.
- Every expression containing unary minus or a binary arithmetic operator is evaluated as `float64`, including `ts/10` and `v0+0x1000`.
>DISCUSSION: BEGIN
- If an integer format specifier occurs in the -vis printf format string, an automatic implicit cast is performed.

>DISCUSSION RESPONSE:

Agreed, provided the conversion is defined by the output format verb rather than delegated blindly to Go `fmt`:

- Direct integral fields keep their signed or unsigned type for `%d`, `%b`, `%o`, `%x`, and `%X`.
- A direct floating-point field or an arithmetic result used with an integer verb is checked for `NaN`, infinity, and the `int64` range, then truncated toward zero and passed as `int64`.
- Integral fields used with `%f`, `%e`, `%E`, `%g`, or `%G` are converted to `float64`.
- `%v` preserves the evaluator result type, and `%t` accepts only Boolean values.

Thus `printf("%d", ts*0.123)` has a documented meaning without exposing a Go-specific cast syntax. The possible semantic loss caused by truncation is intentionally the user's responsibility. Unsafe conversions still disable the rule with a prominent diagnostic, and `%!verb(...)` must never reach the sink. The formatter, validation, implementation, and test sections will need to use this format-driven conversion rule consistently.
>DISCUSSION: END
- A Boolean field may be passed directly to a compatible Go format verb but may not participate in arithmetic.
- Division by zero, a non-finite result not accepted by the chosen output format, or an incompatible Go format verb disables the rule with a prominent non-fatal error. The affected record remains in normal logging.

This makes scaling predictable without implementing mixed signed/unsigned integer arithmetic. For exact integer or hexadecimal output, pass an unmodified integer field directly:

```bash
-vis='msg:printf("%d,%08x\n",ts32,v0)@out.txt'
```

For scaled output, use a floating-point verb:

```bash
-vis='msg:printf("%0.3f,%0.6f\n",ts32/1000,v0*0.001)@out.csv'
```

## 11. <a id="reordering-values"></a>Reordering values

The expression list allows arbitrary reordering of Target-Stamp and values.

Example target message:

```c
TRice("msg:x=%f,y1=%f,y2=%f\n", aFloat(x), aFloat(y1), aFloat(y2));
```

Example transformation:

```bash
-vis='msg:printf("y2=%6.1f,y1=%3.2f,time=%d\n",v2,v1,ts32)@out.txt'
```

This intentionally allows an output order different from the target-side Trice parameter order.

## 12. <a id="compatibility-expectation-for-one-tag"></a>Compatibility expectation for one tag

Different Trice format strings may use the same tag, but every historical `til.json` entry selected by one rule must be statically compatible with that rule's positional fields and direct-field format verbs.

For example, these are compatible with a rule using `v0`, `v1`, and `v2` as floating-point fields:

```c
TRice("msg:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
TRice("msg:A=%f,B=%f,C=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
```

These are not compatible with one rule that requires `v0`, `v1`, and `v2`:

```c
Trice("msg:hi\n");
TRice("msg:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
```

>DISCUSSION: BEGIN

The first message is simply ignored by a `-vis` rule that requires `ts,v0,v1,v2`.

~~If both entries exist in `til.json`, the rule is disabled with a prominent diagnostic listing the incompatible IDs. This strict check intentionally includes historical entries. The user may remove obsolete entries manually and restart tlog.~~

Gennerally, not-matching messages are silently ignored. Only 16-bit - 32-bit target stamp clashes and an obviously wrong v5 on a 3-value messages are reported as error.

>DISCUSSION RESPONSE:

I agree with silently ignoring non-matching candidate messages and with removing the struck-through whole-rule behavior. To keep matching predictable when several rules use the same tag, I recommend one additional MVP restriction: the required positional arity is `highest referenced vN + 1`, and the decoded message must have exactly that many values. A rule referencing `v0` therefore matches one-value messages, while a rule referencing `v0` and `v1` matches two-value messages. A rule with no `vN` reference has no arity restriction.

Exact equality of the human-readable Trice format strings is not needed. Messages such as `plot:a=%d` and `plot:b=%d` may intentionally share one rule because their tag, arity, and numeric compatibility are the same. Their semantic equivalence remains the user's responsibility.

An obviously wrong `v5` disables the rule at startup only when no candidate ID for that tag has six values. If at least one six-value candidate exists, the rule is useful and three-value candidates are simply excluded, with details available in verbose mode. Otherwise legacy entries would again disable valid rules.

Target-Stamp width is an independent runtime eligibility check as described above. A generic-`ts` width clash remains a prominent runtime error and disables the rule.

>CLARIFICATION: A -vis rule is allowed to use only a subset of the Trice message values.

>DISCUSSION: END

## 13. <a id="printf-formatter"></a>`printf(...)` formatter

The first implementation should support:

```text
printf("<go-fmt>",<expr-list>)
```

Examples:

```bash
-vis='msg:printf("%d,%f,%f,%f\n",ts,v0,v1,v2)@out.csv'
```

>DISCUSSION: BEGIN

```bash
-vis='msg:printf("%d,%0.3f,%0.3f,%08x\n",ts32/10,v0*0.5,v1*100,20.0*v2-1000)@udp://127.0.0.1:7010'
```

As mentioned: implicit type conversion according %name.

>DISCUSSION RESPONSE:

With the format-driven conversion rule above, this example is valid:

- `ts32/10` is evaluated as `float64`, then truncated toward zero and converted to `int64` for `%d`.
- The middle two arithmetic expressions remain `float64` for `%0.3f`.
- `20.0*v2-1000` is evaluated as `float64`, range-checked, truncated toward zero, and converted to `int64` for `%08x`.

The conversion is selected by the format verb, not by a field name. For a negative hexadecimal result, Go's signed integer formatting includes a minus sign; users wanting an unsigned bit pattern should pass an unmodified unsigned field or explicitly choose a future conversion feature. This example and the introductory `%d` example therefore no longer represent an accidental type mismatch, but a documented lossy conversion.

>DISCUSSION: END

The formatting should follow Go `fmt` semantics as far as practical. Go's formatter is powerful and should not be artificially reduced without a reason.

At minimum, the implementation should consider numeric verbs such as:

```text
%d, %b, %o, %x, %X for integer-like output
%f, %e, %E, %g, %G for floating-point output
%v for generic value output if acceptable
width and precision, for example %3.2f, %6.1f, %08x
%% for a literal percent sign
\n in the format string for newline output
```

Do not promise C-style `%u` in the first implementation. Go `fmt` does not use `%u` as the normal unsigned decimal verb. Use `%d` for decimal integer output and `%x`/`%X` for hexadecimal output.

The formatter must never emit Go `%!verb(...)` diagnostics into a visualization stream. It does not perform an implicit float-to-integer conversion for incompatible verbs. Compatibility is checked against all known matching `til.json` entries at startup when possible. A conflict disables the rule with a prominent diagnostic. A conflict first discovered at runtime has the same result, while the current record remains in normal logging.

The encoder renders the complete output record into one temporary buffer before passing it to a sink. The encoder emits exactly the bytes described by the Go format string; it does not add an implicit newline.

## 14. <a id="sinkaddress-syntax"></a>Sink/address syntax

The sink is specified after `@`.

### 14.1. <a id="plain-file-path"></a>Plain file path

```text
@out.csv
@logs/imu.csv
@/tmp/imu.csv
@file:out.csv
@file:logs/imu.csv
```

A bare path and the explicit `file:<path>` form both identify a normal file. Relative paths remain relative to tlog's working directory. The first implementation creates a missing file and appends to an existing file. Every unique normalized file destination is opened once and shared by rules that use the same destination.

`file://out.csv` is not accepted. Under standard URI parsing it identifies `out.csv` as an authority/host rather than as a relative path. Full `file:///absolute/path` URI semantics are outside the first implementation; accepting the non-standard relative spelling now would make a later standards-based parser incompatible.

### 14.2. <a id="udp"></a>UDP

```text
@udp://127.0.0.1:7010
@udp://localhost:7010
```

The UDP destination is resolved and opened when the rule set is initialized. Each completely encoded visualization record is sent with exactly one UDP write and therefore becomes one UDP datagram. A successful write means that the local operating system accepted the datagram; it does not guarantee receipt by the visualization tool.

File and UDP writes are synchronous in the first implementation. There is no background queue, reconnect loop, or delivery acknowledgement.

### 14.3. <a id="other-sinks"></a>Other sinks

TCP client/server, WebSocket client/server, named pipes/FIFOs, Windows named pipes, process pipes, and tool-specific protocols are not part of the first implementation. Their schemes are rejected rather than partially accepted. They can be added later behind the same record-oriented sink contract.

Named pipes are deliberately deferred because Unix FIFOs and Windows named pipes are not one portable Go standard-library abstraction, and opening a FIFO writer can block until a reader connects.

## 15. <a id="logkeep-and-logdrop"></a>`log=keep` and `log=drop`

The optional suffix controls normal tlog output for one successfully transformed decoded TREX record:

```text
log=keep  default; keep the normal decoded record
log=drop  emit the visualization record but omit that decoded record from normal output
```

Examples:

```bash
-vis='msg:printf("%d,%f,%f,%f\n",ts32,v0,v1,v2)@out.csv'
```

is equivalent to:

```bash
-vis='msg:printf("%d,%f,%f,%f\n",ts32,v0,v1,v2)@out.csv;log=keep'
```

To suppress normal output after successful visualization output:

```bash
-vis='msg:printf("%d,%f,%f,%f\n",ts32,v0,v1,v2)@out.csv;log=drop'
```

There is no need for `log=strip`, because tlog already removes known lower-case-only tags before normal output.

Hint: The implementation must be aware of that fact and not rely on the stripped-tag formatstring.

Processing order and requirements:

1. Decode one TREX message and retain its typed visualization record alongside the existing formatted text.
2. Apply the existing `-pick` or `-ban` filter to the formatted text.
3. If the message was filtered out, do not evaluate any `-vis` rule and do not write normal output.
4. Evaluate every enabled and eligible matching `-vis` rule.
5. If at least one successful matching rule uses `log=drop`, omit the decoded TREX record from normal output; otherwise pass the existing formatted text to the unchanged normal line composer.

Additional rules:

- `-pick` and `-ban` retain their existing mutual-exclusion behavior.
- `-ban` cannot replace `log=drop`: a banned record is also invisible to `-vis`.
- If multiple matching rules use different log modes, one successful `log=drop` rule wins for normal-output suppression while all matching rules are still attempted.
- An ignored record, encoding error, or sink-write error does not activate `log=drop`.
- A sink or evaluation failure disables the affected rule with a prominent non-fatal error; normal logging continues and retains the current record.
- The policy applies to one decoded TREX record, not to an already composed line containing multiple Trice messages. Such multi-Trice lines are outside `-vis` eligibility.

## 16. <a id="interaction-with-normal-lower-case-tag-stripping"></a>Interaction with normal lower-case tag stripping

`-vis` tag matching must not change the existing normal-log tag stripping rules.

A tag used only by `-vis` is not automatically registered as a normal-output tag.

Therefore:

- `-vis='imu:printf(...)@...'` may match and transform `imu:...` messages.
- But the normal log output removes `imu:` only if `tlog` would already remove it, for example because it is known internally or was explicitly made known via `-ulabel`.
- Unknown lower-case-only tags used only by `-vis` must not be silently stripped from normal output.

This behavior must be tested.

## 17. <a id="examples"></a>Examples

### 17.1. <a id="labplot-csv-file"></a>LabPlot CSV file

Target:

```c
TRice("msg:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
```

`tlog`:

```bash
tlog ... \
  -vis='msg:printf("%d,%0.3f,%0.3f,%0.3f\n",ts32,v0,v1,v2)@file:imu.csv;log=drop'
```

Output:

```csv
123456,0.012,-0.034,9.810
```

LabPlot can be configured separately with a LiveDataSource reading the CSV file. The LabPlot `.lml` project file remains LabPlot-specific and is not embedded in the Trice target format string.

### 17.2. <a id="labplot-or-serial-studio-udp-stream-with-scaling"></a>LabPlot or Serial Studio UDP stream with scaling

```bash
tlog ... \
  -vis='msg:printf("%0.3f,%0.3f,%0.3f,%0.3f\n",ts32/10,v0*0.5,v1*100,20.0*v2-1000)@udp://127.0.0.1:7010;log=drop'
```

The complete CSV row is emitted as one UDP datagram.

### 17.3. <a id="json-line-in-the-printf-format"></a>JSON line in the `printf` format

```bash
tlog ... \
  -vis='msg:printf("{\"stamp\":%d,\"ax\":%f,\"ay\":%f,\"az\":%f}\n",ts32,v0,v1,v2)@udp://127.0.0.1:7010'
```

This is sufficient for simple numeric JSON consumers. A schema-aware JSON encoder is not part of the first implementation.

### 17.4. <a id="two-outputs-from-the-same-tag"></a>Two outputs from the same tag

```bash
tlog ... \
  -vis='msg:printf("%d,%f,%f,%f\n",ts32,v0,v1,v2)@out.csv' \
  -vis='msg:printf("%d,%f,%f,%f\n",ts32,v0,v1,v2)@udp://127.0.0.1:7010'
```

### 17.5. <a id="different-tags"></a>Different tags

Target:

```c
TRice("imu:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
TRice("pow:u=%f,i=%f\n", aFloat(u), aFloat(i));
TRice("tmp:t=%f\n", aFloat(temp));
```

`tlog`:

```bash
tlog ... \
  -vis='imu:printf("%d,%f,%f,%f\n",ts32,v0,v1,v2)@imu.csv' \
  -vis='pow:printf("%d,%f,%f\n",ts32,v0,v1)@power.csv;log=drop' \
  -vis='tmp:printf("%d,%f\n",ts32,v0)@udp://127.0.0.1:7020'
```

### 17.6. <a id="reordered-output"></a>Reordered output

Target:

```c
TRice("msg:x=%f,y1=%f,y2=%f\n", aFloat(x), aFloat(y1), aFloat(y2));
```

`tlog`:

```bash
tlog ... \
  -vis='msg:printf("%d,%6.1f,%3.2f\n",ts32,v2,v1)@out.csv'
```

### 17.7. <a id="no-target-stamp-runtime-ignore-for-a-stamp-dependent-rule"></a>No Target-Stamp: runtime ignore for a stamp-dependent rule

Target:

```c
trice("msg:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
TRice("msg:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
```

`tlog`:

```bash
tlog ... \
  -vis='msg:printf("%d,%f,%f,%f\n",ts32,v0,v1,v2)@out.csv'
```

The first decoded message is ignored because the rule explicitly requires a 32-bit Target-Stamp. The second decoded message is transformed. Under the existing verbose mode, the ignored first message produces a diagnostic.

### 17.8. <a id="separate-explicit-16--and-32-bit-target-stamp-rules"></a>Separate explicit 16- and 32-bit Target-Stamp rules

```bash
tlog ... \
  -vis='fast:printf("%d,%f\n",ts16,v0)@fast.csv' \
  -vis='slow:printf("%d,%f\n",ts32,v0)@slow.csv'
```

The explicit fields make records of the other width ineligible instead of mixing their numeric ranges.

## 19. <a id="startup-validation-requirements-1"></a>Startup validation requirements

Before input decoding starts, tlog parses every raw `-vis` argument, validates every rule as far as the available `til.json` permits, and then creates the unique sinks required by the remaining enabled rules.

CLI syntax and sink initialization errors prevent tlog from starting. These checks include:

- a non-empty tag selector followed by `:`;
- the known encoder name `printf`;
- quote-aware and parenthesis-aware parsing rather than delimiter-only string splitting;
- exactly one `@` separator outside quoted strings and parentheses;
- the optional `;log=keep|drop` policy and rejection of unknown options;
- a valid plain path, `file:<path>`, or `udp://host:port` sink;
- correct `printf(...)` quoting and balanced parentheses;
- valid whitelisted expression syntax;
- only `id`, `ts`, `ts16`, `ts32`, and `v0...v11` identifiers;
- rejection of a rule that references both `ts16` and `ts32`;
- one consuming Go format verb per expression, with `%%` consuming no expression;
- supported numeric and Boolean Go format verbs with literal width and precision;
- rejection of dynamic width/precision (`*`), explicit argument indexes, `%u`, unsupported verbs, and unsupported sink schemes.

After syntax validation, tlog scans every historical `til.json` entry selected by the tag. The rule remains enabled only when all matching entries are supported fixed-width numeric TREX formats and provide compatible types for every referenced `vN` and every direct-field Go format verb. No matching ID, an unavailable `vN`, or any incompatible matching ID disables the complete rule.

>DISCUSSION: BEGIN

- What exactly is ment with enabled/disabled -vis rule?
- Should an exact equality of Trice format strings (with different IDs) be a demand for a matching -vis for simplification? What happens with

```C
Trice("plot:a=%d", x);
Trice("plot:a=%d", x);
Trice("plot:b=%d", y);
TRice("plot:a=%d, b=%d", x, y);
Trice("plot:x=%d, y=%d", x, y);
TRice("plot:x=%d, y=%d", x, y);
TRICE(ID(0), "plot:a=%d, b=%d", x, y);
TRICE(Id(0), "plot:x=%d, y=%d", x, y);
TRICE(ID(0), "plot:x=%d, y=%d", x, y);
```

and 

```bash
tlog ...\
    -vis=plot:printf("%f, %d\n"), ts, v0)@plot.csv \
    -vis=plot:printf("%f, %d, %d\n"), ts, v0, v1)@plot.csv \
```

For the MVP prefer an easy to understand, smooth to use and stable way. 

- Because "plot" is a user free selectable name, collisions with legacy til.json entries are of minor relevance, I think.
- The first 3 Trice messages are matching the first rule with a 16-bit target stamp and the 2nd 3 would match the 2nd rule with a 32-bit target stamp but there is an error: `Trice("plot:x=%d, y=%d", x, y);` shoulb be `TRice("plot:x=%d, y=%d", x, y);`. That is detectable at start-up. But the error `TRICE(Id(0), "plot:x=%d, y=%d", x, y);` ist detectable only during runtime. There must be a clear rule-set, easy to understand and to handle. No all-inclusive solution. Keep it simple! During the development process all the time mistakes occur and their fixing usually is quickly done.

>DISCUSSION RESPONSE:

`Enabled` means that the rule was parsed successfully, has at least one compatible LUT candidate, has an initialized sink, and is considered for every surviving runtime record. `Disabled` means that the rule remains known for diagnostics but is skipped for the rest of this tlog run. Every transition to disabled emits one prominent diagnostic. Invalid CLI syntax or failure to initialize a sink prevents tlog from starting instead of creating a disabled rule.

I do not recommend exact equality of complete Trice format strings. It would make harmless label changes such as `plot:a=%d` versus `plot:b=%d` separate otherwise identical data shapes and would make historical text changes disruptive. For the MVP, use this matching sequence:

1. The record survives existing pick/ban filtering.
2. The original, unstripped format starts with the selected tag.
3. The record is a supported numeric, single-Trice, single-line TREX record.
4. Its value count exactly equals `highest referenced vN + 1`; no `vN` means no arity restriction.
5. Every referenced value is numerically or Boolean-compatible with the format-driven conversion required by its output verb.
6. The runtime Target-Stamp condition (`ts`, `ts16`, or `ts32`) is satisfied.

All rules satisfying these conditions run; overlap is allowed deliberately. Candidate IDs failing conditions 3 through 5 are excluded from that rule rather than disabling it. The rule is disabled at startup only when no compatible LUT candidate remains.

With the shown one-value and two-value records, arity separates the two rules. Stamp intent should additionally be explicit:

```bash
tlog ... \
    -vis='plot:printf("%f, %d\n",ts16,v0)@plot.csv' \
    -vis='plot:printf("%f, %d, %d\n",ts32,v0,v1)@plot.csv'
```

The one-value rule accepts the first three records. The two-value rule accepts two-value records only when they carry a 32-bit stamp. The accidentally written `Trice("plot:x=%d, y=%d", ...)` and `TRICE(Id(...), "plot:x=%d, y=%d", ...)` cannot be detected at startup because the stamp selector spelling is not present in `til.json`. At runtime they fail the `ts32` condition and are ignored, with a diagnostic in verbose mode. If generic `ts` were used instead, the first observed width would be remembered and a later second width would disable that rule prominently.

The source `%u` hint does not make an ID incompatible by itself: `%u` is allowed in the target Trice format and yields an unsigned typed value. Only `%u` in the `-vis` output format is rejected for the MVP; `%d` can format a direct unsigned value.

This rule set is intentionally not all-inclusive, but it is deterministic, keeps legacy tag collisions harmless, and leaves richer selectors for a later extension.

>DISCUSSION: END

A LUT compatibility conflict does not stop tlog. It emits a prominent non-fatal error and disables only that rule. The diagnostic includes the rule number and text, the reason, all affected IDs, the explicit action `rule disabled`, and the statement that normal logging continues. A rule is never disabled silently.

Examples:

```text
ERROR: -vis rule 2 disabled: tag "msg" references v3, incompatible IDs: 123, 456; normal logging continues
```

```text
ERROR: -vis rule 1 disabled: tag "msg" has no matching ID in til.json; normal logging continues
```

```text
invalid -vis rule 3: unsupported sink scheme "tcp://"; first implementation accepts file paths, file:, and udp://
```

```text
invalid -vis rule 4: unsupported printf verb "%u"; use "%d" for decimal integer output
```

`til.json` does not provide a reliable Target-Stamp width. Stamp-dependent eligibility and generic-`ts` width conflicts are therefore checked against runtime TREX records rather than guessed at startup.

If a matching ID is first observed after a runtime LUT update, or its type/format differs from the version validated at startup, that actual entry is validated before use. An incompatibility disables the rule with the same prominent diagnostic. Automatic re-enabling is not required; the user can correct `til.json` and restart tlog.

## 20. <a id="concrete-implementation-plan"></a>Concrete implementation plan

The implementation is intentionally a small structured side path around the existing text decoder. It does not replace the current `decoder.Decoder` interface, the existing `Read` behavior, or the normal line composer.

### 20.1. <a id="cli-collection-and-lifetime"></a>CLI collection and lifetime

- Register repeatable `-vis` values on the existing `log` flag set in `internal/args` using a dedicated `flag.Value` that stores the raw strings in order.
- Add an `internal/vis` package containing rule parsing, expression evaluation, encoding, sink management, diagnostics, and tests. It must not depend on `internal/translator` or `internal/emitter`.
- After `til.json` is loaded and before the input receiver loop starts, construct one `vis.Router` from all raw rule strings, the LUT, and its existing read/write mutex. Every router LUT scan uses that mutex.
- After LUT validation, open every unique file or UDP sink referenced by an enabled rule once during router construction. Rules with the same canonical destination share the same sink; disabled rules create no files or network connections.
- Pass the router explicitly into `translator.Translate`; do not introduce a global visualization router.
- Keep the router and its sinks alive for the complete tlog command, including input-port reconnects, and close them when the log command ends.
- If no `-vis` option is present, use a nil/no-op router and preserve the current decode path and output without additional per-record work.

### 20.2. <a id="decoder-local-typed-record"></a>Decoder-local typed record

Add a small optional record-provider interface in `internal/decoder`; do not change the existing `Decoder` interface implemented by TREX, CHAR, and DUMP:

```text
VisRecordProvider
  VisRecord() (VisRecord, bool)
```

The concrete TREX decoder implements this provider and stores exactly one last record as decoder-instance state. The record is reset to invalid at the beginning of every `Read` call so that decoder errors, zero-length reads, CHAR/DUMP output, and stale records cannot be mistaken for visualization data.

The record contains:

```text
VisRecord
  ID              Trice ID
  Type            decoded LUT type for this record
  Format          original effective Trice format string
  Stamp           raw uint64 container for the unmodified 16-/32-bit value
  StampBits       0 | 16 | 32
  Values          fixed storage for v0...v11
  ValueCount      number of available stored values
  SingleLine      true only for one complete newline-terminated Trice message

VisValue
  Kind            signed | unsigned | float | bool
  Bits            8 | 16 | 32 | 64
  exact value     int64 | uint64 | float64 | bool according to Kind
```

Use fixed storage for the twelve values rather than allocating a per-record map or value-name table. If a supported TREX message contains more than twelve parameters, only the first twelve are addressable by `-vis`; normal decoding remains unchanged.

Populate this record in the existing fixed-width numeric TREX value-decoding path while the signed, unsigned, floating-point, and Boolean values already exist in typed form and before the existing `fmt.Sprintf` call converts them to normal text. Do not parse the formatted log text and do not move any sink or expression logic into the TREX decoder.

The `SingleLine` fact is based on the effective Trice format: it must form exactly one logical line, have exactly one newline at the end, and contain no internal newline. The translator additionally requires that the existing normal line composer did not already contain a partial line before this decoded record.

### 20.3. <a id="translator-integration-and-filtering-order"></a>Translator integration and filtering order

After each successful `dec.Read`:

1. Apply the existing `emitter.BanOrPickFilter` to the existing formatted text exactly where it runs today.
2. If the filter returns zero, skip `-vis` and normal output.
3. If the decoder implements `VisRecordProvider`, obtain the record that corresponds to this exact `Read` result.
4. Give an eligible record and the current line-open state to `vis.Router.Process` before adding location, Target-Stamp, ID, or text to the normal line composer.
5. `Router.Process` attempts every enabled matching rule and returns whether at least one successful rule requested `log=drop`.
6. If drop is false, execute the existing metadata and `TriceLineComposer` calls unchanged. If drop is true, skip those calls for this record.

The selector matches the tag prefix against `VisRecord.Format`, not against a reconstructed value stream. The existing pick/ban filter deliberately uses the current formatted-text behavior and has absolute precedence.

### 20.4. <a id="rule-and-expression-model"></a>Rule and expression model

Use separate internal objects for:

```text
Rule
  raw rule text and stable command-line index
  TagSelector
  PrintfEncoder
  RecordSink
  LogMode keep | drop
  enabled/disabled state and disable reason
  optional generic-ts width already observed
  validated ID/type/format fingerprints
```

Parse the expression list with `go/parser.ParseExpr`, then reject every AST node outside the documented numeric subset. Resolve identifiers to field references during rule creation. Direct fields preserve their exact `VisValue` type. Any unary or binary arithmetic expression is evaluated as `float64`.

Scan the Go format string once during rule creation. Support numeric verbs `%d`, `%b`, `%o`, `%x`, `%X`, `%f`, `%e`, `%E`, `%g`, `%G`, generic `%v`, Boolean `%t`, literal width and precision, and `%%`. Reject `%u`, dynamic `*` width/precision, explicit argument indexes, strings, and unsupported verbs. Store the expected argument category for each consuming verb so incompatible values are rejected before `fmt.Sprintf` is called.

The `printf` encoder evaluates every expression into a typed encoder argument, renders the complete record into one temporary byte buffer, checks that no Go formatting diagnostic was produced, and calls the sink exactly once for that record. It never adds a newline that is absent from the user format.

### 20.5. <a id="record-oriented-sinks"></a>Record-oriented sinks

Use a sink contract equivalent to:

```text
WriteRecord(completeRecord []byte) error
Close() error
```

- File sink: open with create, write-only, and append behavior; write each complete record to the shared file handle; flush through normal file semantics; close on tlog shutdown.
- UDP sink: resolve and connect the destination during router construction; send each complete record with one `Write`, producing one datagram; close on tlog shutdown.
- Both sinks are synchronous. Do not add goroutines, queues, retries, acknowledgements, reconnect state, or delivery statistics in the first implementation.

If sink creation fails, tlog does not start. If a runtime sink write fails, mark the shared sink failed, disable every rule using it with an individual prominent non-fatal error, and continue normal logging. The failed record is not dropped from normal output.

### 20.6. <a id="rule-eligibility-and-disable-behavior"></a>Rule eligibility and disable behavior

- A rule using `ts16` ignores unstamped and 32-bit-stamped records. A rule using `ts32` ignores unstamped and 16-bit-stamped records.
- A rule using generic `ts` ignores unstamped records, remembers the first observed stamped width, and is disabled if it later observes the other width.
- A rule using no stamp field accepts stamped and unstamped records equally.
- Explicit width mismatches and unsupported multi-Trice/multi-line records are ignored without changing rule state. They are reported for every occurrence only when the existing verbose mode is enabled.
- Expression evaluation, format compatibility, changed/new incompatible LUT entries, and sink errors disable the affected rule.
- Every rule disable action emits one prominent non-fatal error at the state transition. It is never conditional on verbose mode and is never silent.
- A disabled rule stays disabled until tlog is restarted.
- `log=drop` is effective only for a rule whose record was encoded and accepted by its sink. For UDP, successful acceptance means a successful local `Write`, not confirmed network delivery.

### 20.7. <a id="no-changes-outside-the-mvp"></a>No changes outside the MVP

- Do not modify CHAR or DUMP decoder behavior.
- Do not parse `name=%...` value names.
- Do not aggregate multiple Trice messages into one visualization record.
- Do not implement Target-Stamp scaling, normalization, wrap handling, or 16-/32-bit reconstruction.
- Do not add TCP, WebSocket, named-pipe, process-spawning, JSON-specific, binary, or graphics-tool-specific code.
- Do not introduce a third-party dependency for the first implementation.

### 20.8. <a id="expected-change-footprint"></a>Expected change footprint

The implementation is expected to have this focused production-code footprint:

- `internal/args/init.go`: register the repeatable `-vis` flag and its help text.
- `internal/args/handler.go`: construct and close the command-lifetime router after loading `til.json`, and pass it into translation.
- `internal/decoder`: add the small record/value types and optional provider interface without changing the existing `Decoder` contract.
- `internal/trexDecoder/trexDecoder.go`: reset, populate, and expose the decoder-local record in the existing fixed-width numeric path.
- `internal/translator/translator.go`: invoke the router after existing pick/ban filtering and before normal metadata/line composition.
- new focused files below `internal/vis`: rule parser, format scanner, expression evaluator, router, file sink, UDP sink, and their package-local helpers.
- corresponding package tests plus integration tests in the existing decoder/translator test locations.
- `docs/TriceUserManual.md`: add the user-facing chapter only after behavior and tests are implemented.

The standalone `tlog` executable and `trice log` must call this same shared path; no duplicate implementation is added. No target-side file below `src` is changed.

## 21. <a id="testing-requirements"></a>Testing requirements

Implementation must include focused unit and integration tests without requiring hardware, an external graphics tool, or network access beyond a local UDP listener.

### 21.1. <a id="cli-and-rule-parser-tests"></a>CLI and rule-parser tests

- repeated `-vis` arguments preserve command-line order and stable rule numbers;
- valid bare relative and absolute file paths;
- valid `file:<path>` sink;
- valid IPv4/IPv6/hostname UDP destination accepted by Go's resolver;
- valid default `log=keep` and explicit `log=keep|drop`;
- quoted format strings containing `:`, `@`, `;`, commas, parentheses, and escaped quotes are parsed correctly;
- missing tag, colon, encoder, `@`, sink, quote, or closing parenthesis is rejected;
- unknown encoder or option is rejected;
- `file://out.csv`, TCP, TCP-listen, WebSocket, and pipe schemes are rejected;
- `ts16` and `ts32` in one rule are rejected;
- unknown identifiers, `v12`, function calls, indexing, and unsupported AST nodes are rejected;
- `%u`, unsupported verbs, dynamic `*` width/precision, and explicit argument indexes are rejected;
- consuming format-verb count must equal expression count while `%%` consumes none.

### 21.2. <a id="lut-validation-tests"></a>LUT validation tests

- one compatible matching ID enables a rule;
- all compatible historical IDs sharing a tag enable a rule;
- no matching ID disables the rule with one prominent diagnostic;
- one historical ID with too few parameters disables the complete rule and lists that ID;
- incompatible signed/unsigned/float/Boolean direct-field use disables the rule and lists affected IDs;
- an unsupported special TREX format selected by the tag disables the rule;
- obsolete matching entries are treated exactly like current entries;
- a new or changed incompatible entry first observed after LUT update disables the rule;
- disabling is never silent and the diagnostic states that normal logging continues;
- a disabled rule is not automatically re-enabled.

### 21.3. <a id="trex-record-capture-tests"></a>TREX record-capture tests

- no record is returned before the first successful numeric TREX decode;
- the record is invalidated at the beginning of every `Read`;
- decoder errors, zero-length reads, X0, and unsupported special records cannot expose a stale record;
- ID, original effective format, Stamp value, Stamp bits, value count, exact kind, and bit width are preserved;
- signed, unsigned, float32, float64, Boolean, and pointer-like numeric values are captured before text formatting;
- `v0...v11` map to the first twelve decoded positions;
- existing formatted TREX text remains byte-for-byte unchanged when `-vis` is absent;
- CHAR and DUMP do not implement or expose a visualization record.

### 21.4. <a id="single-line-eligibility-tests"></a>Single-line eligibility tests

- one message with exactly one trailing newline is eligible;
- a message without a newline is ignored;
- a message with an internal newline is ignored;
- one message containing multiple complete lines is ignored;
- a message that continues an already open normal-output line is ignored;
- two or more Trice messages forming one normal line are not transformed;
- ignored cases are silent normally and produce diagnostics for every occurrence in verbose mode.

### 21.5. <a id="expression-and-formatting-tests"></a>Expression and formatting tests

- direct signed, unsigned, float, Boolean, ID, and stamp fields preserve their types;
- `ts32/10`, `v0*0.5`, `v1+100`, `20.0*v2-1000`, parentheses, and unary minus evaluate as `float64`;
- decimal, floating-point, and hexadecimal literals are accepted;
- direct exact integer and hexadecimal formatting works;
- arithmetic results require a compatible floating-point or generic verb;
- Boolean arithmetic is rejected;
- division by zero and incompatible runtime formatting disable the rule with one prominent diagnostic;
- width, precision, `%v`, `%t`, and literal `%%` work as documented;
- no output can contain a Go `%!verb(...)` formatting diagnostic;
- output is reordered according to the expression list;
- a numeric JSON one-liner can be produced entirely by the `printf` format string;
- the encoder adds no implicit newline.

### 21.6. <a id="target-stamp-tests"></a>Target-Stamp tests

- `ts16` transforms 16-bit-stamped records and ignores 0-/32-bit records;
- `ts32` transforms 32-bit-stamped records and ignores 0-/16-bit records;
- generic `ts` accepts repeated records of one width;
- generic `ts` remembers its first observed width;
- a later second width disables the generic-`ts` rule with a prominent diagnostic containing the IDs and both widths;
- unstamped records are ignored by any stamp-dependent rule;
- a rule without a stamp field can transform stamped and unstamped records;
- no scaling, normalization, wrap extension, or mixed-width reconstruction occurs;
- explicit-width and unstamped ignores are silent normally and reported in verbose mode.

### 21.7. <a id="sink-tests"></a>Sink tests

- a missing file is created and an existing file is appended;
- multiple rules using the same normalized file path share one handle without truncation or overlapping offsets;
- a complete encoded record is passed to the file sink in one call;
- one complete encoded record is received as one datagram by a local UDP listener;
- UDP has no receiver acknowledgement requirement;
- invalid file and UDP destinations fail router creation before decoding begins;
- an injected file or UDP runtime write failure marks the shared sink failed, disables every attached rule with prominent diagnostics, and retains the current normal log record;
- all opened sinks close on normal tlog shutdown and input errors.

### 21.8. <a id="filter-and-normal-output-tests"></a>Filter and normal-output tests

- existing `-pick` and `-ban` behavior and mutual exclusion remain unchanged;
- a record removed by `-ban` never reaches `-vis`;
- a record excluded by `-pick` never reaches `-vis`;
- default `log=keep` preserves the current normal output exactly;
- successful `log=drop` suppresses only the corresponding decoded TREX record;
- ignored records and failed rules do not activate `log=drop`;
- with multiple matching rules, all are attempted and one successful `log=drop` wins;
- lower-case-only tag removal remains unchanged;
- `-vis` does not automatically register an unknown tag for normal-output stripping;
- no `-vis` option preserves current translator and line-composer behavior.

### 21.9. <a id="integration-tests"></a>Integration tests

- fake encoded TREX input exercises decoder capture, pick/ban, routing, encoding, sink output, and normal-output retention together;
- include one `Trice(...)` 16-bit-stamp sample, one `TRice(...)` 32-bit-stamp sample, and one `trice(...)` unstamped sample;
- include one LabPlot-/Serial-Studio-compatible CSV file and UDP example;
- include one numeric JSON-over-UDP example;
- include multiple rules for one tag and independent rules for different tags;
- verify that tlog and the standalone `tlog` executable use the same implementation path.

## 22. <a id="documentation-requirement"></a>Documentation requirement

When implemented, add a dedicated chapter to `docs/TriceUserManual.md` using the terms tlog, Target-Stamp, and stamp consistently.

The chapter must document:

- the deliberately limited purpose and TREX-only scope of `-vis`;
- the tag selector, repeatable rules, `printf` encoder, sink, and option syntax;
- `v0...v11` positional fields and the explicit absence of value-name extraction;
- the raw `ts`, `ts16`, and `ts32` semantics;
- that a Target-Stamp is not necessarily time;
- the exclusion of scaling, wrap handling, and mixed 16-/32-bit reconstruction inside tlog;
- direct-field type preservation and `float64` arithmetic results;
- supported Go format verbs and rejected `%u`, dynamic width, and argument indexes;
- exact output bytes and the absence of an automatically added newline;
- bare paths, `file:<path>`, append behavior, and why `file://out.csv` is rejected;
- one-record-per-UDP-datagram behavior and lack of delivery acknowledgement;
- synchronous file/UDP writes and runtime sink-error behavior;
- strict validation of every matching historical `til.json` entry;
- prominent diagnostics and the rule-disable lifecycle;
- single-message/single-line eligibility and verbose diagnostics for ignored records;
- precedence of existing `-pick`/`-ban` filtering;
- `log=keep|drop` behavior, including multiple matching rules and failures;
- unchanged lower-case tag stripping;
- CSV file, CSV UDP, numeric JSON UDP, reordered values, `ts16`, and `ts32` examples;
- deferred support for named values, TCP, WebSocket, named pipes, special TREX types, CHAR, DUMP, and binary encoders.

References to LabPlot, Serial Studio, PlotJuggler, uPlot, and Grafana remain examples of consumers or future adapters, not promises of tool-specific integration.

## 23. <a id="non-goals-for-the-first-implementation"></a>Non-goals for the first implementation

- No need to implement a full LabPlot `.lml` generator.
- No need to implement a Serial Studio project generator.
- No need to implement binary uPlot frames immediately.
- No need to implement Grafana integration immediately.
- No need to put destination addresses into target-side Trice format strings.
- No need to support arbitrary programming-language expressions beyond the small arithmetic expression set described above.
- No need to support `p0...p11` synonyms.
- No need to support C-style `%u` unless explicitly added and documented.

Additional first-implementation non-goals:

- No value names extracted from `name=%...` text.
- No CHAR or DUMP support and no special string, buffer, command, or X0 TREX records.
- No visualization of messages that form partial, combined, or multi-line normal log lines.
- No `ts_raw`, `ts_bits`, normalized time field, unit conversion, wrap extension, reset handling, or mixed 16-/32-bit stamp reconstruction.
- No TCP, TCP-listen, WebSocket, named pipe/FIFO, Windows named pipe, child-process, MQTT, or graphics-tool-specific sink.
- No automatic JSON schema handling; simple numeric JSON can be written by the `printf` format.
- No asynchronous queues, reconnect loops, acknowledgements, or delivery guarantees.
- No implicit conversion of an arithmetic `float64` result to an integer Go format verb.
- No automatic re-enabling of a rule disabled during a tlog run.

The first step is a robust and well-tested TREX-only side path that preserves the existing formatted log behavior, captures one decoder-local typed record before text formatting, applies existing pick/ban filtering before visualization, and routes eligible single-line records through a `printf` encoder to an append-only file or one-record-per-datagram UDP sink. It uses positional fields only, performs strict historical LUT validation, never disables a rule silently, and leaves all broader transports and tool-specific behavior for later extensions.
