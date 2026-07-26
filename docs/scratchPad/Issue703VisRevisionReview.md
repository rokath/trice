# Feature request: configurable -vis output transformation for dynamic data visualization

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
- [19. Concrete implementation plan](#concrete-implementation-plan)
  - [19.1. CLI collection and lifetime](#cli-collection-and-lifetime)
  - [19.2. Decoder-local typed record](#decoder-local-typed-record)
  - [19.3. Translator integration and filtering order](#translator-integration-and-filtering-order)
  - [19.4. Rule and expression model](#rule-and-expression-model)
  - [19.5. Record-oriented sinks](#record-oriented-sinks)
  - [19.6. Rule eligibility and disable behavior](#rule-eligibility-and-disable-behavior)
  - [19.7. No changes outside the MVP](#no-changes-outside-the-mvp)
  - [19.8. Expected change footprint](#expected-change-footprint)
- [20. Testing requirements](#testing-requirements)
  - [20.1. CLI and rule-parser tests](#cli-and-rule-parser-tests)
  - [20.2. LUT validation tests](#lut-validation-tests)
  - [20.3. TREX record-capture tests](#trex-record-capture-tests)
  - [20.4. Single-line eligibility tests](#single-line-eligibility-tests)
  - [20.5. Expression and formatting tests](#expression-and-formatting-tests)
  - [20.6. Target-Stamp tests](#target-stamp-tests)
  - [20.7. Sink tests](#sink-tests)
  - [20.8. Filter and normal-output tests](#filter-and-normal-output-tests)
  - [20.9. Integration tests](#integration-tests)
- [21. Documentation requirement](#documentation-requirement)
- [22. Non-goals for the first implementation](#non-goals-for-the-first-implementation)

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

Hint: Arithmetic expressions are evaluated as `float64`. For an integer output verb such as `%d`, the `printf` encoder checks that the result is finite and within the `int64` range, truncates it toward zero, and passes it to Go's formatter as `int64`. Thus `ts*0.123` has a defined meaning without adding Go- or C-style cast syntax to the expression language. Any intentional precision loss caused by truncation remains the user's responsibility.

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
- `tlog` must validate the syntax of all `-vis` CLI strings before decoding starts and classify all tag-matching entries currently present in `til.json` as compatible or incompatible candidates for each rule.
- Existing `-pick` and `-ban` processing runs before `-vis`. A message removed by `-pick` or `-ban` is invisible to `-vis` as well as to normal output.
- The normal `tlog` output remains unchanged unless at least one matching, successful `-vis` rule explicitly uses `log=drop`.
- A `-vis` rule is never disabled silently. Every rule disable action emits a prominent non-fatal warning identifying the rule, the reason, and affected IDs when known; normal logging continues.
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

Existing `-pick` or `-ban` filtering is applied before this matching step. If multiple enabled `-vis` rules match the remaining decoded message, every eligible transformation is attempted. A rule is eligible only if its Target-Stamp requirement is met, the message is one supported fixed-width numeric TREX message forming one complete log line, every referenced `vN` exists, and every referenced value can undergo the documented conversion for its output verb. A rule may use only a subset of the values in the message; additional unreferenced values do not make the message ineligible.

## 6. <a id="lut-compatibility-and-runtime-eligibility"></a>LUT compatibility and runtime eligibility

At startup, each syntactically valid `-vis` rule scans every historical `til.json` entry whose original format string starts with the selected `<tag>:` prefix. A tag match makes the entry a candidate for that rule; it does not require every historical entry with that tag to have the same data shape.

Each candidate is classified independently. It is compatible when:

- the entry describes a supported fixed-width numeric TREX message;
- every `v0...v11` referenced by the rule exists in that message;
- every referenced value type can undergo the documented format-driven conversion for its corresponding output verb; and
- the rule can be evaluated without extracting names from the human-readable format text.

A rule may reference only a subset of the values in a candidate message. Additional unreferenced values are allowed and do not participate in compatibility checks. A `%u` conversion in the target Trice format is also allowed and produces an unsigned typed value; only `%u` in the `-vis` output format is rejected for the MVP.

The rule remains enabled when at least one compatible candidate exists. Incompatible candidates are excluded from that rule and do not disable it. They are ignored silently in normal mode; verbose mode reports each excluded ID once when it is classified, together with the reason. If the tag has no candidate IDs, or if none of its candidates is compatible, the rule is disabled with a prominent non-fatal warning. A disabled rule is not silently or automatically re-enabled during the same process.

When `til.json` is updated while tlog is running, a new or changed matching ID is classified before a corresponding runtime record is used. An incompatible candidate is ignored by that rule, silently in normal mode and with one classification diagnostic in verbose mode. It does not disable a rule while another compatible candidate remains. If a changed entry reclassifies the rule's last compatible candidate as incompatible, the rule is disabled with a prominent non-fatal warning. Unused historical entries added during the run need not be classified until the next tlog start. Normal logging continues.

The Target-Stamp width is not reliably available in `til.json`; it is encoded in each runtime TREX record. Therefore target-stamp eligibility is handled at runtime:

- a rule referencing `ts16` processes only 16-bit-stamped records;
- a rule referencing `ts32` processes only 32-bit-stamped records;
- records with no stamp or the other explicit width are ignored by that rule;
- a rule referencing generic `ts` records the width of its first otherwise eligible stamped record and is disabled with a prominent warning if a later otherwise eligible record has the other width.

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
- A rule referencing generic `ts` accepts the width of its first otherwise eligible stamped record and remembers that width.
- If the same generic-`ts` rule later receives the other stamp width, the rule is disabled with a prominent non-fatal warning containing the rule, both widths, and affected IDs. Normal logging continues.
- A record without a Target-Stamp is ignored by a rule that references `ts`, `ts16`, or `ts32`.
- The first implementation performs no scaling, normalization, unit conversion, wrap extension, reset detection, or reconstruction of mixed 16-/32-bit stamp sequences.
- The optimization where a 16-bit stamp represents the low bits of the most recently transmitted 32-bit stamp is explicitly unsupported by `-vis`.
- A future general tlog Target-Stamp scaling feature is independent of `-vis` and must not silently change the meaning of these raw `-vis` fields.

The Target-Stamp selector spelling (`trice`/`Trice`/`TRice` or `TRICE(id(...))`/`TRICE(Id(...))`/`TRICE(ID(...))`) is not reliably represented in `til.json`. Consequently, an accidental unstamped/16-bit/32-bit spelling change cannot be diagnosed at startup from the LUT. It is handled only when a corresponding runtime TREX record arrives, according to the explicit-width or generic-`ts` rules above.

Users apply any required scaling explicitly, for example `ts32*0.001`. A Target-Stamp may also deliberately represent a non-time x or y coordinate.

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

The output format verb determines the final encoder argument conversion:

- Direct integral fields retain their signed or unsigned type for `%d`, `%b`, `%o`, `%x`, and `%X`.
- A direct floating-point field or arithmetic result used with an integer verb must be finite and within the `int64` range. It is then truncated toward zero and passed to the formatter as `int64`.
- Integral fields used with `%f`, `%e`, `%E`, `%g`, or `%G` are converted to `float64`; any loss of integer precision has the normal Go conversion semantics and remains the user's responsibility.
- `%v` preserves the evaluator result type.
- `%t` accepts only a direct Boolean field.
- A Boolean field may not participate in arithmetic or be used with a numeric verb.

An expression/verb incompatibility known from the rule text alone is an invalid `-vis` rule and prevents tlog from starting. A candidate-specific type incompatibility excludes only that candidate. Division by zero or an unsafe conversion discovered from an actual runtime value disables the rule with a prominent non-fatal warning. The failed rule does not activate `log=drop`; absent another successful matching `log=drop` rule, the affected record remains in normal logging. No Go `%!verb(...)` diagnostic is written to the sink. Truncation within the valid `int64` range and possible precision loss during integral-to-`float64` conversion are deliberately allowed; whether the resulting loss of precision is semantically appropriate is the user's responsibility.

For exact integer or hexadecimal output, pass an unmodified integral field directly:

```bash
-vis='msg:printf("%d,%08x\n",ts32,v0)@out.txt'
```

For scaled floating-point output, use a floating-point verb:

```bash
-vis='msg:printf("%0.3f,%0.6f\n",ts32/1000,v0*0.001)@out.csv'
```

Format-driven scaled integer output is also valid:

```bash
-vis='msg:printf("%d\n",ts32*0.123)@out.txt'
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

Different Trice format strings may use the same tag. The tag selects candidates; each rule independently retains only the candidates that provide all fields referenced by that rule and support the required format-driven conversions. Complete equality of the human-readable Trice format strings is not required.

A rule may intentionally use only a subset of a message's values. Additional unreferenced values do not affect compatibility. For example, both messages below are compatible with a rule using `v0`, while only the second is compatible with a rule using `v0`, `v1`, and `v2`:

```c
TRice("msg:a=%f\n", aFloat(a));
TRice("msg:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
```

For a rule requiring `v0`, `v1`, and `v2`, the first candidate below is excluded and the second remains compatible:

```c
Trice("msg:hi\n");
TRice("msg:ax=%f,ay=%f,az=%f\n", aFloat(ax), aFloat(ay), aFloat(az));
```

The incompatible zero-value candidate does not disable the useful rule. If no candidate for the tag provides all three referenced values, the rule is disabled with a prominent non-fatal warning.

## 13. <a id="printf-formatter"></a>`printf(...)` formatter

The first implementation should support:

```text
printf("<go-fmt>",<expr-list>)
```

Examples:

```bash
-vis='msg:printf("%d,%f,%f,%f\n",ts,v0,v1,v2)@out.csv'
```

The formatting should follow Go `fmt` semantics as far as practical. Go's formatter is powerful and should not be artificially reduced without a reason.

At minimum, the implementation should support:

```text
%d, %b, %o, %x, %X for integer-like output
%f, %e, %E, %g, %G for floating-point output
%v for generic value output
%t for a direct Boolean value
width and precision, for example %3.2f, %6.1f, %08x
%% for a literal percent sign
\n in the format string for newline output
```

Do not promise C-style `%u` in the first implementation. Go `fmt` does not use `%u` as the normal unsigned decimal verb. Use `%d` for decimal integer output and `%x`/`%X` for hexadecimal output.

The formatter must never emit Go `%!verb(...)` diagnostics into a visualization stream. Before calling Go's formatter, it performs only the documented format-driven conversions: integral-to-`float64` conversion with normal Go precision semantics, checked and truncating floating-to-`int64` conversion for integer verbs, direct integral formatting, `%v` type preservation, and direct Boolean `%t`.

An expression/verb incompatibility known from the rule text alone makes the `-vis` rule invalid and prevents tlog from starting. At startup, every known tag-matching `til.json` candidate is checked for compatible field availability and candidate-dependent type categories. An incompatible candidate is excluded rather than disabling a rule that still has another compatible candidate. A division by zero or unsafe conversion first discovered from an actual runtime value disables the rule with a prominent non-fatal warning. That failed rule does not activate `log=drop`; absent another successful matching `log=drop` rule, the current record remains in normal logging.

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
- A sink or evaluation failure disables the affected rule with a prominent non-fatal warning; it does not itself activate `log=drop`, and normal logging continues.
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

## 18. <a id="startup-validation-requirements"></a>Startup validation requirements

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
- rejection of dynamic width/precision (`*`), explicit argument indexes, `%u`, unsupported verbs, and unsupported sink schemes;
- rejection of expression/verb combinations that are incompatible from the rule text alone, such as an arithmetic expression or numeric literal used with `%t`; candidate-dependent `vN` compatibility is handled by the subsequent LUT classification.

After syntax validation, tlog scans every historical `til.json` entry selected by the tag and classifies it for that rule. A candidate is compatible when it is a supported fixed-width numeric TREX format, provides every referenced `vN`, and provides types for which the required format-driven conversions are defined. Unreferenced additional values are allowed.

The rule remains enabled when at least one compatible candidate exists. Incompatible candidates are excluded silently in normal mode; verbose mode reports each excluded candidate once when it is classified, together with the reason. No tag-matching ID, or no compatible candidate among the matching IDs, disables the rule with a prominent non-fatal warning. One incompatible candidate never disables a rule while another compatible candidate remains.

An incompatible LUT candidate does not stop tlog and does not by itself disable a rule. It is ignored silently in normal mode; verbose mode identifies the rule, ID, and exclusion reason. If a rule has no compatible candidate, tlog emits a prominent non-fatal warning and disables only that rule. The warning includes the rule number and text, the reason, the candidate IDs when known, the explicit action `rule disabled`, and the statement that normal logging continues. A rule is never disabled silently.

Examples:

```text
VERBOSE: -vis rule 2 ignores candidate ID 123: v5 is unavailable; record provides 3 values
```

```text
WARNING: -vis rule 2 disabled: tag "msg" references v5, but no candidate ID provides it; candidate IDs: 123, 456; normal logging continues
```

```text
WARNING: -vis rule 1 disabled: tag "msg" has no matching ID in til.json; normal logging continues
```

```text
invalid -vis rule 3: unsupported sink scheme "tcp://"; first implementation accepts file paths, file:, and udp://
```

```text
invalid -vis rule 4: unsupported printf verb "%u"; use "%d" for decimal integer output
```

`til.json` does not provide a reliable Target-Stamp width. Stamp-dependent eligibility and generic-`ts` width conflicts are therefore checked against runtime TREX records rather than guessed at startup.

If a matching ID is first observed after a runtime LUT update, or its type/format differs from the version classified at startup, that actual entry is classified before use. An incompatible candidate is ignored, normally silently and with one classification diagnostic in verbose mode. If a changed entry reclassifies the rule's last compatible candidate as incompatible, the rule is disabled with the same prominent warning. Automatic re-enabling is not required; the user can correct `til.json` and restart tlog.

## 19. <a id="concrete-implementation-plan"></a>Concrete implementation plan

The implementation is intentionally a small structured side path around the existing text decoder. It does not replace the current `decoder.Decoder` interface, the existing `Read` behavior, or the normal line composer.

### 19.1. <a id="cli-collection-and-lifetime"></a>CLI collection and lifetime

- Register repeatable `-vis` values on the existing `log` flag set in `internal/args` using a dedicated `flag.Value` that stores the raw strings in order.
- Add an `internal/vis` package containing rule parsing, expression evaluation, encoding, sink management, diagnostics, and tests. It must not depend on `internal/translator` or `internal/emitter`.
- After `til.json` is loaded and before the input receiver loop starts, construct one `vis.Router` from all raw rule strings, the LUT, and its existing read/write mutex. Every router LUT scan uses that mutex.
- After LUT validation, open every unique file or UDP sink referenced by an enabled rule once during router construction. Rules with the same canonical destination share the same sink; disabled rules create no files or network connections.
- Pass the router explicitly into `translator.Translate`; do not introduce a global visualization router.
- Keep the router and its sinks alive for the complete tlog command, including input-port reconnects, and close them when the log command ends.
- If no `-vis` option is present, use a nil/no-op router and preserve the current decode path and output without additional per-record work.

### 19.2. <a id="decoder-local-typed-record"></a>Decoder-local typed record

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

### 19.3. <a id="translator-integration-and-filtering-order"></a>Translator integration and filtering order

After each successful `dec.Read`:

1. Apply the existing `emitter.BanOrPickFilter` to the existing formatted text exactly where it runs today.
2. If the filter returns zero, skip `-vis` and normal output.
3. If the decoder implements `VisRecordProvider`, obtain the record that corresponds to this exact `Read` result.
4. Give an eligible record and the current line-open state to `vis.Router.Process` before adding location, Target-Stamp, ID, or text to the normal line composer.
5. `Router.Process` attempts every enabled matching rule and returns whether at least one successful rule requested `log=drop`.
6. If drop is false, execute the existing metadata and `TriceLineComposer` calls unchanged. If drop is true, skip those calls for this record.

The selector matches the tag prefix against `VisRecord.Format`, not against a reconstructed value stream. The existing pick/ban filter deliberately uses the current formatted-text behavior and has absolute precedence.

### 19.4. <a id="rule-and-expression-model"></a>Rule and expression model

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
  compatible candidate ID/type/format fingerprints
  optional verbose exclusion reason per incompatible candidate
```

Parse the expression list with `go/parser.ParseExpr`, then reject every AST node outside the documented numeric subset. Resolve identifiers to field references during rule creation. Direct fields preserve their exact `VisValue` type. Any unary or binary arithmetic expression is evaluated as `float64`.

Scan the Go format string once during rule creation. Support numeric verbs `%d`, `%b`, `%o`, `%x`, `%X`, `%f`, `%e`, `%E`, `%g`, `%G`, generic `%v`, Boolean `%t`, literal width and precision, and `%%`. Reject `%u`, dynamic `*` width/precision, explicit argument indexes, strings, and unsupported verbs. For each consuming verb, store the required conversion operation so compatibility and unsafe-value checks occur before `fmt.Sprintf` is called.

The converter preserves direct signed and unsigned integral values for integer verbs, converts integral values to `float64` for floating-point verbs, and converts direct floating-point values or arithmetic results to `int64` for integer verbs after finite and range checks and truncation toward zero. `%v` preserves the evaluator result, and `%t` accepts only a direct Boolean field.

Reject during rule construction any expression/verb combination whose incompatibility is already independent of a candidate record. Defer only `vN` type compatibility to candidate classification.

The `printf` encoder evaluates every expression, performs its stored conversion, renders the complete record into one temporary byte buffer, checks that no Go formatting diagnostic was produced, and calls the sink exactly once for that record. It never adds a newline that is absent from the user format.

### 19.5. <a id="record-oriented-sinks"></a>Record-oriented sinks

Use a sink contract equivalent to:

```text
WriteRecord(completeRecord []byte) error
Close() error
```

- File sink: open with create, write-only, and append behavior; write each complete record to the shared file handle; flush through normal file semantics; close on tlog shutdown.
- UDP sink: resolve and connect the destination during router construction; send each complete record with one `Write`, producing one datagram; close on tlog shutdown.
- Both sinks are synchronous. Do not add goroutines, queues, retries, acknowledgements, reconnect state, or delivery statistics in the first implementation.

If sink creation fails, tlog does not start. If a runtime sink write fails, mark the shared sink failed, disable every rule using it with an individual prominent non-fatal warning, and continue normal logging. The failed sink does not itself cause the current record to be dropped; another successful matching `log=drop` rule may still do so.

### 19.6. <a id="rule-eligibility-and-disable-behavior"></a>Rule eligibility and disable behavior

- A tag-matching candidate is compatible with a rule when every referenced `vN` exists and every referenced value type supports the required format-driven conversion. Additional unreferenced message values are allowed.
- An incompatible candidate is ignored by that rule, silently in normal mode and with one classification diagnostic in verbose mode. It does not disable a rule while another compatible candidate remains.
- A rule using `ts16` ignores unstamped and 32-bit-stamped records. A rule using `ts32` ignores unstamped and 16-bit-stamped records.
- A rule using generic `ts` ignores unstamped records, remembers the width of the first otherwise eligible stamped record, and is disabled with a prominent non-fatal warning if it later observes an otherwise eligible record with the other width.
- A rule using no stamp field accepts stamped and unstamped records equally.
- Explicit width mismatches and unsupported multi-Trice/multi-line records are ignored without changing rule state. They are reported for every occurrence only when the existing verbose mode is enabled.
- Division by zero, unsafe runtime format conversion, generic-`ts` width conflict, reclassification of the last compatible candidate as incompatible after a LUT update, and sink errors disable the affected rule.
- A new or changed incompatible LUT candidate is otherwise ignored by that rule; it does not disable a rule that still has another compatible candidate.
- Every rule disable action emits one prominent non-fatal warning at the state transition. It is never conditional on verbose mode and is never silent.
- A disabled rule stays disabled until tlog is restarted.
- `log=drop` is effective only for a rule whose record was encoded and accepted by its sink. For UDP, successful acceptance means a successful local `Write`, not confirmed network delivery.

### 19.7. <a id="no-changes-outside-the-mvp"></a>No changes outside the MVP

- Do not modify CHAR or DUMP decoder behavior.
- Do not parse `name=%...` value names.
- Do not aggregate multiple Trice messages into one visualization record.
- Do not implement Target-Stamp scaling, normalization, wrap handling, or 16-/32-bit reconstruction.
- Do not add TCP, WebSocket, named-pipe, process-spawning, JSON-specific, binary, or graphics-tool-specific code.
- Do not introduce a third-party dependency for the first implementation.

### 19.8. <a id="expected-change-footprint"></a>Expected change footprint

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

## 20. <a id="testing-requirements"></a>Testing requirements

Implementation must include focused unit and integration tests without requiring hardware, an external graphics tool, or network access beyond a local UDP listener.

### 20.1. <a id="cli-and-rule-parser-tests"></a>CLI and rule-parser tests

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
- consuming format-verb count must equal expression count while `%%` consumes none;
- statically incompatible expression/verb pairs are rejected, while a `vN` whose compatibility depends on its LUT candidate is deferred to candidate classification.

### 20.2. <a id="lut-validation-tests"></a>LUT validation tests

- one compatible matching ID enables a rule;
- a compatible ID keeps the rule enabled when other historical IDs sharing the tag are incompatible;
- a rule may reference a subset of a candidate's values, and additional unreferenced values remain compatible;
- no matching ID disables the rule with one prominent warning;
- matching IDs but no compatible candidate disable the rule with one prominent warning listing the candidate IDs and reasons;
- an unavailable `vN` excludes only that candidate; it disables the rule only when no compatible candidate remains;
- supported signed, unsigned, and floating-point values are accepted when the output verb has a documented format-driven conversion;
- incompatible Boolean use and unsupported special TREX formats exclude the affected candidates;
- `%u` in the target Trice format yields an unsigned typed candidate and does not conflict with a supported `-vis` integer verb;
- obsolete matching entries are classified like current entries but cannot disable a rule that has another compatible candidate;
- a new incompatible candidate first observed after a LUT update is ignored silently in normal mode and reported once when classified in verbose mode;
- reclassification of the final compatible candidate as incompatible disables the rule with one prominent warning;
- disabling is never silent and the warning states that normal logging continues;
- a disabled rule is not automatically re-enabled.

### 20.3. <a id="trex-record-capture-tests"></a>TREX record-capture tests

- no record is returned before the first successful numeric TREX decode;
- the record is invalidated at the beginning of every `Read`;
- decoder errors, zero-length reads, X0, and unsupported special records cannot expose a stale record;
- ID, original effective format, Stamp value, Stamp bits, value count, exact kind, and bit width are preserved;
- signed, unsigned, float32, float64, Boolean, and pointer-like numeric values are captured before text formatting;
- `v0...v11` map to the first twelve decoded positions;
- existing formatted TREX text remains byte-for-byte unchanged when `-vis` is absent;
- CHAR and DUMP do not implement or expose a visualization record.

### 20.4. <a id="single-line-eligibility-tests"></a>Single-line eligibility tests

- one message with exactly one trailing newline is eligible;
- a message without a newline is ignored;
- a message with an internal newline is ignored;
- one message containing multiple complete lines is ignored;
- a message that continues an already open normal-output line is ignored;
- two or more Trice messages forming one normal line are not transformed;
- ignored cases are silent normally and produce diagnostics for every occurrence in verbose mode.

### 20.5. <a id="expression-and-formatting-tests"></a>Expression and formatting tests

- direct signed, unsigned, float, Boolean, ID, and stamp fields preserve their evaluator types;
- `ts32/10`, `v0*0.5`, `v1+100`, `20.0*v2-1000`, parentheses, and unary minus evaluate as `float64`;
- decimal, floating-point, and hexadecimal literals are accepted;
- direct signed and unsigned fields retain their types for `%d`, `%b`, `%o`, `%x`, and `%X`;
- direct floating-point fields and arithmetic results work with integer verbs after finite and `int64` range checks and truncation toward zero;
- truncation behavior is verified for positive and negative fractional values;
- integral fields are converted to `float64` for `%f`, `%e`, `%E`, `%g`, and `%G`, including the documented Go precision behavior for large integers;
- `%v` preserves the evaluator result type, and `%t` accepts only a direct Boolean field;
- Boolean arithmetic and Boolean use with numeric verbs are rejected;
- division by zero and `NaN`, infinity, or out-of-range values in a floating-to-integer conversion disable the rule with one prominent warning;
- width, precision, `%v`, `%t`, and literal `%%` work as documented;
- no output can contain a Go `%!verb(...)` formatting diagnostic;
- output is reordered according to the expression list;
- a numeric JSON one-liner can be produced entirely by the `printf` format string;
- the encoder adds no implicit newline.

### 20.6. <a id="target-stamp-tests"></a>Target-Stamp tests

- `ts16` transforms 16-bit-stamped records and ignores 0-/32-bit records;
- `ts32` transforms 32-bit-stamped records and ignores 0-/16-bit records;
- generic `ts` accepts repeated records of one width;
- generic `ts` remembers its first observed width;
- a later otherwise eligible record with the second width disables the generic-`ts` rule with a prominent warning containing the IDs and both widths;
- unstamped records are ignored by any stamp-dependent rule;
- a rule without a stamp field can transform stamped and unstamped records;
- no scaling, normalization, wrap extension, or mixed-width reconstruction occurs;
- explicit-width and unstamped ignores are silent normally and reported in verbose mode.

### 20.7. <a id="sink-tests"></a>Sink tests

- a missing file is created and an existing file is appended;
- multiple rules using the same normalized file path share one handle without truncation or overlapping offsets;
- a complete encoded record is passed to the file sink in one call;
- one complete encoded record is received as one datagram by a local UDP listener;
- UDP has no receiver acknowledgement requirement;
- invalid file and UDP destinations fail router creation before decoding begins;
- an injected file or UDP runtime write failure marks the shared sink failed, disables every attached rule with prominent warnings, and does not itself activate `log=drop`;
- all opened sinks close on normal tlog shutdown and input errors.

### 20.8. <a id="filter-and-normal-output-tests"></a>Filter and normal-output tests

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

### 20.9. <a id="integration-tests"></a>Integration tests

- fake encoded TREX input exercises decoder capture, pick/ban, routing, encoding, sink output, and normal-output retention together;
- include one `Trice(...)` 16-bit-stamp sample, one `TRice(...)` 32-bit-stamp sample, and one `trice(...)` unstamped sample;
- include one LabPlot-/Serial-Studio-compatible CSV file and UDP example;
- include one numeric JSON-over-UDP example;
- include multiple rules for one tag and independent rules for different tags;
- verify that tlog and the standalone `tlog` executable use the same implementation path;
- verify that a rule using only `v0` also transforms a compatible record containing `v0` and `v1`, without exposing or requiring `v1`;
- verify deliberate overlap when two rules with the same tag both accept one record, and demonstrate distinct tags where message shapes must be separated.

## 21. <a id="documentation-requirement"></a>Documentation requirement

When implemented, add a dedicated chapter to `docs/TriceUserManual.md` using the terms tlog, Target-Stamp, and stamp consistently.

The chapter must document:

- the deliberately limited purpose and TREX-only scope of `-vis`;
- the tag selector, repeatable rules, `printf` encoder, sink, and option syntax;
- `v0...v11` positional fields and the explicit absence of value-name extraction;
- the raw `ts`, `ts16`, and `ts32` semantics;
- that a Target-Stamp is not necessarily time;
- the exclusion of scaling, wrap handling, and mixed 16-/32-bit reconstruction inside tlog;
- direct-field type preservation, `float64` arithmetic results, and the documented format-driven numeric conversions, including checked truncation toward zero for integer output verbs and normal Go precision behavior for integral-to-`float64` conversion;
- supported Go format verbs and rejected `%u`, dynamic width, and argument indexes;
- exact output bytes and the absence of an automatically added newline;
- bare paths, `file:<path>`, append behavior, and why `file://out.csv` is rejected;
- one-record-per-UDP-datagram behavior and lack of delivery acknowledgement;
- synchronous file/UDP writes and runtime sink-error behavior;
- candidate-based validation of historical `til.json` entries, subset use of message values, silent exclusion of incompatible candidates in normal mode, and verbose exclusion reasons;
- prominent diagnostics and the rule-disable lifecycle;
- single-message/single-line eligibility and verbose diagnostics for ignored records;
- precedence of existing `-pick`/`-ban` filtering;
- `log=keep|drop` behavior, including multiple matching rules and failures;
- unchanged lower-case tag stripping;
- CSV file, CSV UDP, numeric JSON UDP, reordered values, `ts16`, and `ts32` examples;
- deferred support for named values, TCP, WebSocket, named pipes, special TREX types, CHAR, DUMP, and binary encoders.

References to LabPlot, Serial Studio, PlotJuggler, uPlot, and Grafana remain examples of consumers or future adapters, not promises of tool-specific integration.

## 22. <a id="non-goals-for-the-first-implementation"></a>Non-goals for the first implementation

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
- No user-written cast syntax and no unchecked numeric conversion; integer output verbs use the documented finite, range-checked, truncating conversion to `int64`.
- No automatic re-enabling of a rule disabled during a tlog run.

The first step is a robust and well-tested TREX-only side path that preserves the existing formatted log behavior, captures one decoder-local typed record before text formatting, applies existing pick/ban filtering before visualization, and routes eligible single-line records through a `printf` encoder to an append-only file or one-record-per-datagram UDP sink. It uses positional fields only, permits a rule to use a subset of a message's values, classifies historical LUT entries per rule without letting incompatible legacy candidates poison compatible ones, never disables a rule silently, and leaves all broader transports and tool-specific behavior for later extensions.
