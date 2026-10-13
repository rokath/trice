# Structured Logging

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Placeholders and Names](#placeholders-and-names)
* [2. Field Types and Display](#field-types-and-display)
* [3. Instrumentation and Dictionary](#instrumentation-and-dictionary)
* [4. Output Formats and Event Boundaries](#output-formats-and-event-boundaries)
* [5. JSON and KV Contract](#json-and-kv-contract)
* [6. Optional Metadata](#optional-metadata)
* [7. Field Registry](#field-registry)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

Machine-readable output requires the standard TREX wire format. CHAR and DUMP do not provide suitable event boundaries and are rejected with `-logFormat json` or `-logFormat kv`.

Structured Logging adds named, typed values to a readable message. A regular Trice call is enough:

```c
trice("info:Motor {motor_id}: {temperature_c:%.1f C}", motor_id, aFloat(temperature_c));
```

With `motor_id = 3` and `temperature_c = 87.5`, the output depends on the selected format:

These three outputs use `-li off -showID '' -hs off -ts off`, so no optional host or target metadata appears.

`tlog -logFormat text` (default):

```text
Motor 3: 87.5 C
```

`tlog -logFormat json` (NDJSON):

```json
{"tag":"INFO","level":"INFO","message":"Motor 3: 87.5 C","fields":{"motor_id":3,"temperature_c":87.5}}
```

`tlog -logFormat kv`:

```text
tag=INFO level=INFO message="Motor 3: 87.5 C" field.motor_id=3 field.temperature_c=87.5
```

Further output formats, such as CSV, could be added when needed.

The target continues to transmit the ID and values using the existing wire format. Field names are not transmitted as additional runtime arguments or payload; they reside in the dictionary on the host.

Scalar Trices with 8, 16, 32 or 64 bits and strings through `triceS` and `triceN` are supported. The target macros and their bit-width rules remain authoritative. Context Enrichment (`bind -ce`) can add supported structured fields; see [Context Enrichment](trice_context_enrichment.md).

Try the [PC Feature Tour](example_projects_without_and_with_trice_instrumentation.md#pc-feature-tour) or the [G0B1 Feature Tour](example_projects_without_and_with_trice_instrumentation.md#g0b1-feature-tour): both demonstrate named numeric and string fields and text, NDJSON and KV output. The PC tour requires no hardware and immediately produces a short binary capture.

Named fields are currently unavailable for buffer formats such as `triceB`. These repeat a printf placeholder for each buffer element, whereas a structured field describes one named value. The current field schema does not define whether a named buffer should appear as a numeric list, a byte sequence or text. Therefore, `bind` and `insert` reject `trice8B("msg:{bytes:%02x}", bytes, 2)` with an error. `triceF` does not support named fields either.

Buffer logging without a named field remains available: for `0x01` and `0x02`, `trice8B("msg:%02x ", bytes, 2)` produces JSON `{"tag":"MESSAGE","level":"INFO","message":"01 02 "}` without a `fields` object. For a fixed number of individually named values, use scalar Trices instead. Structured output of entire buffers requires a separate format and schema definition.

## 1. <a id="placeholders-and-names"></a>Placeholders and Names

Start with a familiar printf-style call:

```c
trice("info:Supply %d mV, %d mA", voltage_mV, current_mA);
```

Replace the value conversions with structured placeholders to keep the same text while exporting named values as well:

```c
trice("info:Supply {} mV, {} mA", voltage_mV, current_mA);
```

With `voltage_mV = 3300` and `current_mA = 120`, either call displays `Supply 3300 mV, 120 mA` with `-color none` and no outer metadata. The structured form derives field names from the C arguments. With `-logFormat json`, the records differ as follows:

```json
{"tag":"INFO","level":"INFO","message":"Supply 3300 mV, 120 mA"}
{"tag":"INFO","level":"INFO","message":"Supply 3300 mV, 120 mA","fields":{"voltage_mV":3300,"current_mA":120}}
```

These are alternatives for the same log site, not two calls required to produce one event. Bind or Insert must run after changing the template. For these integer values, `{}` uses the same `%d` display as the original call.

| Format string syntax | Meaning |
|---|---|
| `{motor_id}` | Explicit field name, default display. |
| `{}` | Derive the name from the corresponding C argument. |
| `{plant.}` | Prefix the derived name with `plant.`. |
| `{:%.1f}` | Derived name and explicit display. |
| `{temperature:%.1f C}` | Explicit name and display. |
| `{: = %.1f C}` | Derived name and display. |
| `{motor_id:: %d, }` | Name `motor_id`, display `: %d, `. |
| `{{` and `}}` | Literal opening and closing braces. |

The first colon separates the name from its display. The display may contain free text and further colons, but must contain exactly one supported format specifier. `%%` is not an additional value. Dynamic widths such as `%*d` are not allowed in a structured field.

Without an explicit display, `%d` is used. If the entire argument is wrapped in `aFloat(...)` or `aDouble(...)`, the default is `%f`. There is no general C type inference: an unsigned value needs, for example, `{counter:%u}`, an address `{address:%p}`, and a string `{text:%s}`.

```c
trice("info:{} {}", motor_id, aFloat(temperature_c));
trice("info:{plant.}", motor->temperature);
trice("info:{temperature:%.2f}", aFloat(read_temperature()));
triceS("info:{message:%s}", "motor ready");
triceN("info:{message:%s}", buffer, length);
```

Names can be derived from individual identifiers and simple member chains. Both `motor.temperature` and `motor->temperature` yield `motor.temperature`. The `aFloat` and `aDouble` wrappers are removed when deriving names. Expressions such as `values[0]`, `a+b` or `read_temperature()` need an explicit name. Names consist of identifiers with optional dot-separated segments; whitespace within an identifier, empty segments and leading digits are invalid.

Canonical names must be unique within a call. `{motor->id}` and `{motor.id}` collide. However, `motor` and `motor.id` are two distinct, flat keys; dotted names do not create nested JSON objects.

Ordinary `%...` conversions and structured placeholders consume arguments together, from left to right:

```c
trice("info:%u {temperature:%.1f} %x {last}", sequence, aFloat(temperature), flags, last);
```

Only `temperature` and `last` are exported as fields here. `sequence` and `flags` appear only in the message. Instrumentation checks missing or extra arguments and duplicate or malformed fields. Schema errors prevent publication of partial changes from the instrumentation run.

The new syntax for literal braces also applies in text mode:

```c
trice("info:set={{1,2}}, value={value}", value);
```

This produces `set={1,2}, value=7` when `value = 7`. A backslash before a brace does not replace doubling it.

## 2. <a id="field-types-and-display"></a>Field Types and Display

| Format specifier | Structured value |
|---|---|
| `%d`, `%i` | Signed integer of the Trice bit width. |
| `%u`, `%o`, `%O`, `%x`, `%X`, `%b` | Unsigned integer. The selected numeric base affects only the message. |
| `%f`, `%F`, `%e`, `%E`, `%g`, `%G` | Floating-point number; 32 bits with `aFloat`, 64 bits with `aDouble`. |
| `%c`, `%q`, `%U` | Character as a string; invalid Unicode code points become the replacement character. |
| `%t` | Boolean: zero is `false`, other values are `true`. |
| `%s` | String from `triceS` or `triceN`. |
| `%p` | Address in the form `0x...`; does not imply the type of the referenced object. |

`%%` is a literal percent sign and creates neither a field nor an additional argument.

Supported C length modifiers such as `%lu` or `%llX` do not change these semantics; the Trice family determines the transmitted bit width. Precision, field width and display text affect only `message`. For example, `{value:%.1f}` displays `1.2` for a value of `1.25`, while the field contains `1.25`. Similarly, `{text:%.3s}` truncates only the message, not the exported string.

`-unsigned=false` does not change the meaning of structured unsigned fields either. It continues to control the corresponding classical text display.

Named string fields use `%s`; alternative classical string displays such as `%x` remain message text without a named field. A structured floating-point field in a 64-bit Trice requires `aDouble(...)`; `aFloat(...)` is rejected there to prevent interpreting the transmitted bits as a double.

## 3. <a id="instrumentation-and-dictionary"></a>Instrumentation and Dictionary

Both `bind` and `insert` support structured templates. As before, `clean` removes inserted IDs and preserves the original template spelling. Source shorthand is not replaced with canonical field names.

```sh
trice bind -src app -genDir generated -til til.json -li li.json
```

Alternatively, for the Insert/Clean workflow:

```sh
trice insert -src app -genDir generated -til til.json -li li.json
trice clean -src app -til til.json -li li.json
```

Like the existing workflows, these examples require an initialized TIL. The usual build integration of Bind sidecars is still required.

The [C test examples](../../_test/testdata/triceCheck.c) also demonstrate 8-, 16-, 32- and 64-bit values, different stamps, `triceS`/`triceN`, mixed placeholders and the `aFloat()`/`aDouble()` wrappers. Their integration tests verify text output after `bind` and `insert`, using the same CLI settings as the PC target tests. Names from `motor.state` and `motorPtr->rpm` appear canonically in the field registry as `motor.state` and `motorPtr.rpm`.

In `til.json`, the only schema fields remain `Type` and `Strg`. `Strg` contains all information needed for decoding, including derived names and floating-point defaults. For example,

```c
trice("info:Motor {}: {} C", motor_id, aFloat(temperature_c));
```

is stored with a canonical `Strg` such as:

```json
{"Type":"trice","Strg":"info:Motor {motor_id}: {temperature_c:%f} C"}
```

The actual spelling of `Type` still follows the respective instrumentation path. Schema identity remains `Type + Strg`. Renaming a field creates a new schema identity and therefore a different ID; historical TIL entries remain available for older firmware. Equivalent name spellings such as `motor->temperature` and `motor.temperature` retain the same canonical identity. Repeating unchanged runs creates no additional schemas. Generated local C format data contains the derived printf format string.

## 4. <a id="output-formats-and-event-boundaries"></a>Output Formats and Event Boundaries

`trice log` and `tlog` support `-logFormat text`, `-logFormat json` and `-logFormat kv`, with `-logFormat key-value` as an alias for `kv`. Values are case-insensitive; the default remains `text`. The text-based remote display mode and test table output cannot be combined with machine-readable formats.

```sh
trice log -p FILEBUFFER -args capture.bin -pf TCOBSv1 -til til.json -li off -hs off -ts off -logFormat json
```

Framing and dictionary must match the capture. `-logFormat kv` reads the same capture as key/value output; `text` retains the existing text output.

`-logFormat json` produces NDJSON (JSON Lines): one JSON object followed by LF per accepted Trice event. There is no separate CLI value `ndjson`. KV also produces exactly one line per event. Partial calls are not combined, and multiline messages are not split into multiple events. An empty message is also an event. Newlines within the message are escaped.

Text prefixes, suffixes, colors, indentation, time-difference columns and `-addNL` do not decorate JSON/KV records. Actual metadata appears in separate fields instead. In JSON/KV mode, diagnostics and status messages go to stderr; stdout, `-logfile` and TCP log output contain application records. Write errors are returned to the caller.

`-pick`, `-ban` and `-logLevel` continue to select whole application events. Statistics count successfully decoded events before this selection. Visualization also follows selection; its existing restrictions, such as supported numeric single-line messages, still apply. After successful visualization, `log=drop` suppresses the entire normal record. Binary captures remain unfiltered and unaffected by the selected output format.

## 5. <a id="json-and-kv-contract"></a>JSON and KV Contract

Every application record contains `tag`, `level` and `message`. `tag` is the canonical name of a registered format string tag. Built-in aliases ignore case in both metadata and filtering: `inf:Hi` and `Inf:Hi` both yield `tag=INFO`; `rX:Hi` yields `tag=RECEIVE`. User-defined tags match exactly, so `new` and `NEW` can be separate groups. If no registered tag matches, the value is `untagged`.

`message` takes the message content of text output without ANSI colors, outer metadata, text prefixes or suffixes. With `-color none` or `default`, only a recognized, entirely lowercase format string tag is removed: `inf:Hi` becomes `Hi`, whereas `Inf:Hi` remains `Inf:Hi`. Likewise, `rx:Hi` becomes `Hi`, while `rX:Hi` retains its prefix even though it belongs to RECEIVE. An unknown prefix such as `mgs:Hi` also remains visible. With `-color off`, explicitly written tag prefixes remain, as in text mode. Trice never adds `untagged:` to message text: `trice("Hi")` yields `message="Hi"` and `tag="untagged"`; `trice("mgs:Hi")` yields `message="mgs:Hi"` and `tag="untagged"`. Leading and trailing whitespace, empty messages and messages consisting only of whitespace are preserved.

A runtime string does not change tag classification: with `triceS("{text:%s}", value)` and a value starting with `err:`, `tag` remains `untagged`. Existing text output converts sequences such as `\n` and `\t` for display, including within runtime strings; `message` follows that display. The named field `fields.text` retains the transmitted string, except for outer whitespace.

`level` is derived from the tag group's effective numeric weight, including `-ulabel` overrides. Select the named level with the highest lower boundary not exceeding that weight. The eight boundaries come from the built-in defaults and stay fixed for the command, even when the weights of level-named tags are overridden. Category tags, custom tags and `untagged` use the same rule. Colors and tag names do not determine the level.

| Effective weight | Exported level |
| --- | --- |
| 900–999 | FATAL |
| 800–899 | CRITICAL |
| 700–799 | ERROR |
| 600–699 | WARNING |
| 500–599 | INFO |
| 300–499 | DEBUG |
| 100–299 | TRACE |
| 0–99 | VERBOSE |

VERBOSE starts at 0, so every supported weight belongs to exactly one interval. There is no default-level or INFO fallback. A new custom tag without an explicit weight still inherits the effective INFO **weight**; its level is then derived from that number. For example, `-ulabel INFO:750 -ulabel sensor` gives both groups weight 750 and level ERROR, while the INFO boundary remains 500 for other groups. Equal overridden tag weights are harmless; the eight fixed level boundaries are distinct.

For example, an application category can pass a severity threshold and receive a level while retaining its tag:

```c
trice("sensor:Supply {} mV", voltage_mV);
```

```sh
trice log -ulabel sensor:650 -logLevel wrn -logFormat json
```

Using the matching dictionary and input, with `voltage_mV = 3300`, the event is:

```json
{"tag":"sensor","level":"WARNING","message":"Supply 3300 mV","fields":{"voltage_mV":3300}}
```

The `sensor` weight of 650 passes the default Warning threshold of 600 and falls in the WARNING interval. With `-logFormat kv`, the same event is:

```text
tag=sensor level=WARNING message="Supply 3300 mV" field.voltage_mV=3300
```

Changing `sensor` to weight 550 gives level INFO; changing it to 750 gives level ERROR. Its tag, message and fields remain unchanged. The rule also applies to level-named tags: `-ulabel INFO:750` produces `tag=INFO level=ERROR`. Built-in category RECEIVE normally produces `tag=RECEIVE level=DEBUG`; an untagged message normally produces `tag=untagged level=INFO` without injecting `untagged:` into its message. Register `sensor` consistently for the relevant instrumentation and logging commands; see [user-defined tags](trice_tags_color_and_weights.md#user-defined-tags-weights-and-colors).

In JSON, user fields reside under `fields`. Host and user fields therefore cannot overwrite each other: a user field named `tag` appears under `fields.tag`. User fields are emitted in template order. A record without exportable user fields has no empty `fields` object.

Integers remain JSON numbers, including the full 64-bit boundary values. Readers must use a sufficiently precise numeric type; converting every number to an IEEE-754 double may round large integers. Addresses are JSON strings such as `"0x20001234"`. Non-finite floating-point values (`NaN`, `+Inf`, `-Inf`) are omitted as JSON fields; their text display remains in `message`. Other fields of the same event are preserved.

In KV, user fields are named `field.<name>` and also follow template order. Numbers, Booleans and addresses are unquoted; strings, characters and messages are always double-quoted. Quotes, backslashes, LF, CR and tabs are escaped. Non-finite floats appear in KV as `NaN`, `+Inf` or `-Inf`.

Outer whitespace is removed from other string values such as `hs`, `file` and named string fields. A named field containing only whitespace is emitted as an empty string. This trimming does not apply to `message`.

```text
tag=INFO level=INFO message="Motor \"A\"\nready" field.message="Motor \"A\"\nready" field.address=0x20001234
```

## 6. <a id="optional-metadata"></a>Optional Metadata

| Field | Prerequisite and content |
|---|---|
| `id` | ID-based event and enabled `-showID`; numeric Trice ID without text padding. |
| `file`, `line` | Existing LI entry, enabled `-li` and `-liFmt`; only an available filename or a nonzero line number is emitted. |
| `ts16`, `ts32` | Existing 16- or 32-bit target stamp and output enabled through `-ts` or the corresponding `-ts16`/`-ts32` option. Each value is a string in its own CLI display format; an existing zero value is also emitted. |
| `ts16Delta`, `ts32Delta` | Enabled `-ts16delta` or `-ts32delta` option and a previous stamp of the same bit width. The first stamp has no delta field at all. Display follows the respective delta option. |
| `hs` | Enabled `-hs`; formatted host time string without trailing column padding. `-hs off` or `none` omits it. |

Target stamp values have no leading stamp tag such as `time:` or `dt:`, but retain configured additional text and units. Outer whitespace is removed. The four kinds remain separate: for example, `ts16` may represent a temperature and `ts32` a time. `-ts0` and `-ts0delta` are text placeholders only and create no metadata fields.

For example, `-ts off -ts16 'temp:%d C' -ts16delta 'step:%d C'` with two successive 16-bit stamps of 8 and 11 first yields `"ts16":"8 C"` and then `"ts16":"11 C","ts16Delta":"3 C"`. With `-logFormat kv`, these values appear as `ts16="11 C" ts16Delta="3 C"`.

The fixed order is `tag`, `level`, `message`, then any available `id`, `file`, `line`, target stamp and delta, `hs`, and finally user fields. Missing optional metadata is omitted rather than simulated with null or substitute values. Formatted ID-less `typeX0` events have no ID, TIL fields or target timestamps; they still receive `tag`, `level`, `message` and, where applicable, `hs`.

## 7. <a id="field-registry"></a>Field Registry

A successful `bind` or `insert` run produces `trice-fields.txt`, listing field names and their frequencies in the last successful run. `-genDir` selects the directory for both commands; the default is `./generated`, relative to the invocation directory. For `bind`, the sidecar headers are stored there too. `-buildDir` and `-bindDir` are rejected.

```text
       1 motor_id
       1 temperature_c
       4 motor.state
```

The count covers instrumented sites with that user field in the current invocation. The file is regenerated completely, without adding historical TIL fields. A run over part of the sources describes only that part; project-wide checks should therefore include all relevant sources. Cache hits are counted. Host metadata is excluded, but a field actually named `tag` by the user is included.

Sorting is by ascending count, then alphabetically by field name for equal counts. The line format is `%8d %s\n`; field names have no artificial length limit. A successful run without user fields creates an empty file. `-dry-run` publishes no new file and preserves an existing registry (`trice-fields.txt`).

<p align="right">(<a href="#structured-logging">back to top</a>)</p>

