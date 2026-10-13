# Trice Context Enrichment

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

- [1. Getting Started with Position and Speed](#getting-started-with-position-and-speed)
- [2. Rules and Selectors](#rules-and-selectors)
- [3. Reversible Workflow with insert and clean](#reversible-workflow-with-insert-and-clean)
- [4. Expressions, Fields and Evaluation](#expressions-fields-and-evaluation)
- [5. Using Global and Local Values](#using-global-and-local-values)
- [6. Build, IDs and Generated Files](#build-ids-and-generated-files)
- [7. Supported Log Sites and Alternatives](#supported-log-sites-and-alternatives)
- [8. Test Coverage](#test-coverage)
- [9. Appendix: CE Feasibility Proofs](#appendix-ce-feasibility-proofs)
  - [9.1. Mechanism Under Test](#mechanism-under-test)
  - [9.2. Verified Behavior](#verified-behavior)
  - [9.3. Compiler and Editor Diagnostics](#compiler-and-editor-diagnostics)
  - [9.4. Reproducing the Direct-Site Proof](#reproducing-the-direct-site-proof)
  - [9.5. Relationship to Production Support](#relationship-to-production-support)
  - [9.6. Counterexample for the Original Rebase Approach](#counterexample-for-the-original-rebase-approach)
  - [9.7. Extended PoC for Wrapper Macros and Counter Rebasing](#extended-poc-for-wrapper-macros-and-counter-rebasing)
    - [9.7.1. How the Investigated Approach Works](#how-the-investigated-approach-works)
    - [9.7.2. Verified Cases](#verified-cases)
    - [9.7.3. Compiler Matrix and Evidence Limits](#compiler-matrix-and-evidence-limits)
    - [9.7.4. Implications for a Possible Implementation](#implications-for-a-possible-implementation)
    - [9.7.5. Repeating the Extended PoC](#repeating-the-extended-poc)
- [10. Approach and Boundaries](#approach-and-boundaries)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

Context Enrichment (CE) adds extra values to selected Trice messages, such as task context, position or operating state. A CLI rule applies to all log sites with the matching selector prefix. This enables additional diagnostics for a build without extending every log site by hand.

The repeatable option has the same syntax for `bind`, `insert` and `clean`:

```text
-ce 'selector:"format-extension"[, comma-free C-expression]...'
```

For example, `-ce 'ctx7:", clock={}", clock' adds the value of `clock`, valid at that site, to `trice("msg:ctx7:hi\n");`. With `clock == 42` and `-color none`, the message is `hi, clock=42`. The custom `ctx7:` remains in the source as a selection marker. It disappears from the final log when it is entirely lowercase. The trailing `\n` remains after the appended value. (*Note: `{}` could also be `%d` or `%08x` in this example. The braces simply demonstrate that [Structured Logging](structured_logging.md) and Context Enrichment are orthogonal: they can be used independently or together.*)

| Command | Effect |
|---|---|
| `trice bind -ce …` | Extends generated sidecars; the Trice calls themselves remain unchanged. Supports direct sites uniquely addressable by source line. |
| `trice insert -ce …` | Writes the ID, extension and arguments into recognized source calls. Repeating the same rules does not add the extension again. |
| `trice clean -ce …` | Removes the matching CE extension using the same rules and cleans IDs according to the usual rules. Repetition is harmless. |

CE requires no global runtime context or push/pop calls on the target. Every executed record transmits its own additional values. CE is therefore independent of [Structured Logging](structured_logging.md): an extension may use classical printf placeholders or also create named fields.

Two runnable applications demonstrate the same idea: in the [PC example](examples.md#pc-feature-tour), `bind -ce` adds a cycle value at a shared log site. In the [FreeRTOS example](examples.md#g0b1-feature-tour), derived directly from `G0B1_inst`, the same log site adds the identity of its calling task. Both examples also use `triceS` for a runtime string; CE does not append additional runtime arguments to string Trices.

## 1. <a id="getting-started-with-position-and-speed"></a>Getting Started with Position and Speed

Start with a normally configured [Bind project](bind.md). `./generated` (the default, relative to the invocation directory) must be on the compiler include path. These values are visible at the log site:

```c
struct Position {
    int32_t x;
    int32_t y;
};
struct Position pos = {-444, 77};
float velocity = 33.33f;
```

The log site contains two freely chosen selector prefixes:

```c
trice32("info:pos:speed:Moving sample={sample}\n", 3);
```

The Bind invocation adds position and speed:

```sh
trice bind -ce 'pos:", x={}, y={}", pos.x, pos.y' -ce 'speed:", m/s=%f", aFloat(velocity)'
```

The final template now contains:

```text
info:Moving sample={sample}, x={pos.x}, y={pos.y}, m/s=%f\n
```

The transmitted values are the original `3`, followed by `pos.x`, `pos.y` and the floating-point bit representation of `velocity`. With `-color none`, the message portion of text output is:

```text
Moving sample=3, x=-444, y=77, m/s=33.330002
```

The same result could also be obtained with:

```c
trice32("info:Moving sample={sample}\n", 3);
```

and this Bind invocation:

```sh
trice bind -ce 'info:", x={}, y={}, m/s=%f", pos.x, pos.y, aFloat(velocity)'
```

The decimal digits follow the 32-bit floating-point representation and `%f`; `%.2f` would display `33.33`. JSON and KV contain the same message, including its trailing newline, as an escaped string. They also include the numeric fields `sample`, `pos.x` and `pos.y`. `%f` alone creates no named field; a rule such as `speed:", m/s={speed:%.2f}", aFloat(velocity)` can create one.

The CE examples in [triceCheck.c](../../_test/testdata/triceCheck.c) immediately follow the Structured Logging examples. Their `//exp:` expectations describe the normal run without `-ce`. CE integration tests use the same calls and verify the actually transmitted values with the rules shown above.

## 2. <a id="rules-and-selectors"></a>Rules and Selectors

`bind`, `insert` for adding an extension, and `clean` for removing it use the same syntax. The resulting rule group at a log site must match completely in format and argument order:

```text
-ce 'selector:"format-extension"[, comma-free C-expression]...'
```

The shell must pass the entire option value as one argument; the examples use single quotes for this. Extensions use the same C escapes and placeholders as a Trice format string. They are placed before a trailing `\n` in the original template, or at its end otherwise. Leading and trailing whitespace is preserved.

Selectors are looked up in the contiguous prefix sequence at the beginning of the format string, for example `info:pos:speed:`. Comparison is case-insensitive; known built-in tag aliases belong to the same group, such as `warn` and `WARNING`.

Thus `-ce 'Wrn:", attempt={attempt}", 7'` and `-ce 'WARNING:", attempt={attempt}", 7'` select the same log sites, even when their tag aliases use mixed spellings in the source:

```c
trice("WARNING:Connection lost");
trice("wrn:Retrying");
```

Both calls receive the extension; `WARNING:` or `wrn:` retains its original spelling in the source. This alias resolution applies to log site selection with `bind`, `insert` and `clean`. The complete match of format and argument extensions for `insert` and `clean` is checked separately.

- Only configured selectors trigger CE. Without a matching rule, a prefix has no CE effect.
- A custom, entirely lowercase selector is removed from the final template when its rule is applied, but retained in the source. `pos:` disappears from output; `PoS:` and `POS:` remain visible and trigger the same rule.
- Registered Trice tags and `-ulabel` names retain their prefixes in the final template. Normal rules apply to tag metadata and display: for example, `-color off` preserves `info:`; CE does not remove this known tag.
- Different selectors act in source order. Multiple rules for the same selector act in CLI order.
- If a selector occurs more than once at a site, including through an alias, its rule group is applied only once. During extension, the tool emits a warning for that site.

Thus `info:pos:speed:` adds position first and speed second, even if the CLI lists the `speed` rule first. The same name may serve as both a user label and a CE selector; `-ulabel` and `-ce` have separate purposes.

## 3. <a id="reversible-workflow-with-insert-and-clean"></a>Reversible Workflow with insert and clean

Starting point in `main.c`:

```c
trice("msg:ctx7:hi\n");
```

Insert:

```sh
trice insert -src main.c -ce 'ctx7:", clock={}", clock'
```

The call then looks like this (ID `1234` is only an example):

```c
trice(iD(1234), "msg:ctx7:hi, clock={}\n", clock);
```

No ownership comments or additional CE metadata files are created. Recognition depends solely on the matching selector and the complete extension at the end of the format string and argument list. Whether that text was written by hand or by an earlier Insert invocation does not matter.

For this ID, `til.json` contains the canonical template `msg:hi, clock={clock}\n`. The source retains `ctx7:` and the original field spelling. The compiler receives the additional value; the decoder receives the matching schema. A second identical Insert invocation preserves the extension and ID.

Remove:

```sh
trice clean -src main.c -ce 'ctx7:", clock={}", clock'
```

The source then contains `trice("msg:ctx7:hi\n");` again. Fixed argument counts are adjusted accordingly: removing one CE argument turns `TRICE16_2(Id(1234), …)` into `TRICE16_1(Id(0), …)`. A generic fixed zero-argument form is written as `trice0` or `TRICE0`; without ownership data, the historical spelling `trice_0` cannot be distinguished. Usual Clean rules still apply to IDs: IDs of lowercase macro families are removed, while IDs of the corresponding uppercase variants are set to zero.

**A complete match must occur at exactly the right position.** For the rule above, `, clock={}` must end the format string, and `clock` must end the argument list. A trailing message `\n` remains after the extension. If the rule itself contains a trailing `\n`, that newline belongs to the extension and is removed with it. Format text, whitespace in the format and field spelling must match: `{}` and `{clock}` differ for this comparison. When comparing arguments, C comments are replaced with whitespace as during normal parsing, and outer whitespace is ignored; different expressions such as `clock`, `readClock()` or `clock + 0` are not considered equal.

Even an apparently matching text ending is not a match if it belongs to an existing format conversion. With `-ce 'ctx:"d"'`, the following format string ends in `d`, but that character is part of `%d`, the placeholder for `x`:

```c
trice("ctx:value=%d", x);
```

`clean -ce` leaves the call unchanged: removing `d` would turn `%d` into a lone `%`. Instead, `insert -ce` appends a separate `d`, producing `trice("ctx:value=%dd", x);`.

Similarly, with `-ce 'ctx:"%d", clock'`, the visible `%d` at the end of the next format string is not a value placeholder:

```c
trice("ctx:value=%d %%d", clock);
```

The first `%d` prints `clock`; `%%d` prints the literal text `%d` and needs no additional argument. Although the last characters `%d` and the last argument `clock` seem to match the rule, they do not belong together here. `clean -ce` leaves the call unchanged. `insert -ce` adds its own placeholder and argument, producing `trice("ctx:value=%d %%d%d", clock, clock);`.

| State at the selected log site | `insert -ce` | `clean -ce` |
|---|---|---|
| Complete format and argument extension present | Add nothing | Remove one complete extension |
| No complete match, including a partial match | Append the entire extension | Leave CE unchanged |

Normal ID processing occurs in both cases. When several rules match, the entire rule group is compared in application order. Individual matching parts are neither skipped nor removed separately. Repeated `insert` therefore adds nothing twice. `clean` removes at most one complete group per invocation; if two identical groups occur consecutively, a second Clean invocation can remove the second group too.

For example, this call is a partial match for the rule `ctx7:", clock=%d", clock`:

```c
trice("msg:ctx7:hi, clock=%d\n", other);
```

The format suffix matches, but the last argument `other` does not. `clean -ce` leaves the CE part unchanged. `insert -ce` appends the complete extension:

```c
trice(iD(1234), "msg:ctx7:hi, clock=%d, clock=%d\n", other, clock);
```

A subsequent `insert` now recognizes the complete match at the end. With the same rule, `clean` removes exactly the final `, clock=%d` and final argument `clock`; the previous partial match using `other` remains. Normal checks for valid Trice calls and unique structured field names still apply when appending to a partial match.

To replace an existing extension with another one, first remove the old matching group:

```sh
trice clean -src main.c -ce 'ctx7:", clock={}", clock'
trice insert -src main.c -ce 'ctx7:", clock={clock}", readClock()'
```

An `insert` or `clean` invocation **without** `-ce` performs only its normal ID task. It does not undo an existing CE extension. For full removal, supply the previous `-ce` options. Other options, such as `-src`, `-til`, `-li` and any Trice aliases, must match the project as in the normal workflow.

This handwritten call also contains a complete match:

```c
trice("msg:ctx7:manual, clock={clock}\n", clock);
```

`clean -ce 'ctx7:", clock={clock}", clock'` removes the field and last argument. A corresponding `insert -ce` adds nothing. The call may be moved to another line or file; recognition depends neither on its previous location nor on build files.

`insert -ce` and `clean -ce` respect `-src`, `-exclude` and `TRICE_INSERT_OFF`/`TRICE_INSERT_ON`. CE does not modify Trice calls in ordinary C comments; existing ID processing of those examples remains in place. Files already bound through sidecar includes are not automatically converted to Insert. Use [re-migration to `trice insert`](bind.md#re-migration-to-trice-insert) for these files.

The CE path validates all selected files before publication and writes source, TIL, LI and the Insert field registry together, rolling back on write errors. `-dry-run` publishes nothing. With `-ce`, the experimental timestamp-only `-cache` is bypassed to prevent changed rules from mixing with old source copies. `trice-fields.txt` still describes the last successful Insert/Bind run; Clean creates no new field registry.

## 4. <a id="expressions-fields-and-evaluation"></a>Expressions, Fields and Evaluation

Every CE expression must be comma-free, valid and visible at every selected log site. Suitable examples include `pos.x`, `motor->speed`, `array[i]`, `x + 1`, `aFloat(velocity)` and `condition ? a : b`. `getValue(a, b)` and the comma operator are not allowed in the CLI list; calculate such results in a local variable beforehand. Each option value occupies one complete line; `//` comments are not allowed in expressions.

`{}` derives a field name from a simple expression: `pos.x` becomes `pos.x`, and `motor->speed` becomes `motor.speed`. More complex expressions require a name, for example `ctx:", next={next}", x + 1`. Classical printf placeholders and named fields can be mixed. Literal braces are written as `{{` and `}}`. Duplicate field names in the final record are an error, including when one occurs in the source and another in a CE rule.

Original arguments precede additional CE arguments. Each additional expression is evaluated exactly once per actually executed call. A call that is not executed, or a build with `TRICE_OFF` or `TRICE_CLEAN`, does not evaluate it. CE adds no ordering guarantee between different C expressions; dependent side effects belong in separate statements before the call.

Scalar Trices still transmit at most twelve values of the same bit width. CE preserves bit width and stamp type and adjusts fixed arity, for example from `TRice32_1` to `TRice32_3`. Floating-point values explicitly require `aFloat(...)` at 32 bits and `aDouble(...)` at 64 bits; there is no automatic conversion. 8-/16-bit Trices cannot transmit floating-point values. The compiler checks actual C types and identifier visibility.

String, buffer and other special Trice families receive no additional runtime arguments through CE. A text-only extension without additional values is possible if the final format remains valid for the original family, for example `label:" online"` on a `triceS`. Named buffer fields remain excluded, as in Structured Logging.

## 5. <a id="using-global-and-local-values"></a>Using Global and Local Values

Global state values and functions available everywhere are often especially convenient:

```sh
trice insert -ce 'ctx7:", clock={clock}", readClock()'
```

Every selected log site must be able to call `readClock()`; its declaration must be known there. The value is read when the log call actually executes, not when the Trice tool runs. Without an executed log call, CE does not call the function either.

Local variables are also useful when all selected sites can use the same expression. For example, the rule `-ce 'job:", job={job}", jobId'` fits both functions:

```c
void startJob(int jobId) {
    trice("info:job:start\n");
}

void finishJob(int jobId) {
    trice("info:job:finish\n");
}
```

The shared rule reads the local parameter of whichever function executes. There is no global `jobId` storage or mixing of different calls.

The following variant cannot use that same rule:

```c
void startJob(int jobId) {
    trice("info:job:start\n");
}

void finishJob(int finishedJobId) {
    trice("info:job:finish\n");
}
```

`jobId` does not exist in `finishJob`. The compiler reports the missing name automatically; no additional CLI option is needed. Solutions include a consistent parameter name, a local helper value, separate selectors with appropriate rules, or a directly specified field: `trice("info:finish, job={job}\n", finishedJobId);`.

An ordinary helper function cannot access its caller's local variables either. This also applies to `static inline`: inlining adds no visibility. Values must be passed as parameters:

```c
static inline void logJob(int jobId) {
    trice("info:job:progress\n");
}

void worker(void) {
    int currentJob = 17;
    logJob(currentJob);
}
```

This limitation remains because CE produces ordinary C/C++ code. The Trice tool knows neither all types and declarations nor the preprocessor branches selected by the actual compiler. It checks syntax, schema and supported log forms itself; the compiler, already required for the build, checks exact visibility. A separate full compiler preprocessing pass solely to report errors earlier would unnecessarily complicate usage and builds.

## 6. <a id="build-ids-and-generated-files"></a>Build, IDs and Generated Files

CE is applied before schema and ID determination. The ID follows the final Trice type and canonical template, including field names. A different expression with the same schema does not change the ID: `ctx:", x={position}", pos.x` may change to `ctx:", x={position}", pos.y`. Changes to the field name, format or final type follow the existing ID assignment rules instead. Historical TIL entries remain available for older firmware.

After every source or CE configuration change, rerun `bind` with the complete desired rule list and rebuild the firmware. Without `-ce`, the next Bind run produces normal schemas without CE again. Rules are not carried forward from earlier runs. The dictionary and generated firmware must belong together; changing only `til.json` cannot create additional target values.

Normal Bind setup steps, such as the initial sidecar include, still apply. CE itself writes neither the extension nor additional arguments into user log sites. Repeated runs with the same configuration preserve source, IDs and generated contents. `trice-fields.txt` counts final CE fields together with directly specified fields for the current run. `-dry-run` publishes no changes. Invalid rules, field conflicts and selected unsupported Bind sites are rejected before writes; publication failures use the existing Bind rollback.

`generate -logC` uses the final CE schemas from TIL. Bind sidecars provide the mapping; Insert uses explicit IDs in the source. Rules need not be supplied again:

```sh
trice generate -til til.json -genDir generated -logC triceLog.c
```

With Bind, stale or contradictory CE metadata causes an error; after changing a Trice call, first bind again with the desired rules. With Insert, the source may contain custom lowercase selectors in addition to the TIL template. Message, field schema and Trice type must match the explicit ID's entry. For changed messages, rerun `insert` first; to replace a CE extension, use the workflow above: `clean -ce`, then `insert -ce`. The same source scope and TIL must be accessible; Bind also requires its sidecars in the corresponding build directory.

## 7. <a id="supported-log-sites-and-alternatives"></a>Supported Log Sites and Alternatives

`bind -ce` supports direct Trice calls uniquely addressable by file and source line, including within ordinary and `static inline` functions. This path requires no `__COUNTER__`. Multiline calls are also possible if no other Bind log site occupies their lines.

Selected wrapper macros and counter-rebase sites remain deferred. A typical error is:

```text
main.c:42: error: CE requires a direct, line-addressable bind site. Search UM for "bind-limits".
```

The [bind-limits](bind.md#bind-limits) section explains the cause and possible code adjustments without requiring compiler expertise. Suitable changes include separate source lines or ordinary functions with explicitly passed local values. Wrapper/rebase sites not selected by CE retain existing Bind behavior, including its compiler requirements.

`insert -ce` writes final arguments directly into each recognized log site. This avoids Bind selection through source lines or compiler counters. In particular, these calls can be extended with `-ce 'ctx7:", clock={}", clock'`:

```c
trice("msg:ctx7:first\n"); trice("msg:ctx7:second\n");

#define LOG_STATUS() trice("msg:ctx7:status\n")
```

For a wrapper, Insert extends the Trice call in the **macro definition**. Every later call to `LOG_STATUS()` uses this extension. `clock` must be visible at every expansion. This does not configure different rules per call site for the same wrapper; selectors belong to the recognized format string in its definition.

Parameters of such a wrapper can also be used:

```c
#define LOG_JOB(jobId) trice("info:job:progress\n")

void worker(void) {
    LOG_JOB(17);
}
```

With `insert -ce 'job:", job={job}", jobId'`, `jobId` is inserted directly into the definition and replaced with `17` during macro expansion. Normal macro rules remain unchanged; in particular, avoid arguments with mutually dependent side effects.

This requires a call with a known format string that the Trice parser can recognize. A definition such as `#define LOG_ANY(format) trice(format)` provides neither a static selector nor a complete schema. CE is not a general C preprocessor and does not promise support for arbitrary macro-assembled formats. An explicit Trice call or a function with a fixed format and passed values remains a simple alternative.

| Log form | `bind -ce` | `insert/clean -ce` |
|---|---|---|
| Direct, uniquely addressable call, including in an inline function | Supported | Supported |
| Multiple direct calls on the same line | Rejected when selected by CE | Recognizable calls are extended individually |
| Wrapper definition with a static Trice format | Still rejected when selected by CE | The recognized definition is extended and restored |
| Local name missing at the actual expansion | Compiler error | Compiler error |
| String/buffer record with additional scalar CE arguments | Error before publication | Error before publication |

**Why the Bind limitation remains despite a successful PoC:** Existing counter rebasing placed expressions from different log sites into multiple C branches. The compiler checks even branches that are not executed. A local variable valid only on the left can therefore cause an artificial error on the right. The extended PoC selects the adapter during macro expansion and avoids this error for the tested cases. It requires an additional preprocessing pass with the actual compiler per translation unit and build configuration, plus matching generated mapping files.

This build effort has not been introduced as a production workflow. Also, `__COUNTER__` is not available in every compiler and is neither a runtime counter nor a cycle counter. Even globally visible CE values therefore do not separately enable complex Bind sites. The uniform limitation is easy to explain and reject during Bind. `insert/clean -ce` requires neither this mapping nor this preliminary pass. Exact evidence, costs and open questions appear in the [PoC appendix within this chapter](#appendix-ce-feasibility-proofs).

## 8. <a id="test-coverage"></a>Test Coverage

The [rule and Bind tests](../../internal/id/contextEnrichment_test.go) check selectors, aliases, order, invalid rules, limits, stable IDs, configuration changes, the field registry and unchanged files after rejection or write errors. The [Insert/Clean tests](../../internal/id/contextSource_test.go) additionally cover complete and partial matches, format and argument suffix positions, multi-part rule groups, newlines, repetition, handwritten fields, comments, moved calls, exclusions and rollback after write errors. The [CLI and target tests](../../internal/args/context_enrichment_test.go) exercise both public workflows through `generate -logC` and actual target records to text/JSON/KV output.

Evidence for the production implementation covers Clang in C11 and C++17, `clangd` with a real compile configuration, 8/16/32/64-bit values, different stamp types and builds without `__COUNTER__`. It checks separate local scopes, once-only evaluation, `TRICE_OFF`, `TRICE_CLEAN` and understandable compiler/editor errors for missing identifiers. Insert additionally tests two log sites in separate local blocks on the same wrapper line and their complete removal. The editor check excludes only clangd's `SwapBinaryOperands` refactoring action: Clangd 21 proposes overlapping edits within an explicit `Id(...)` for this action. Compiler diagnostics and missing-identifier errors remain checked. Other compilers are assessed separately in the PoC appendix.

Repeat the focused acceptance tests from the repository root:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id ./internal/args -run '^(TestBindContext|TestContextEnrichment|TestInsertCleanContext|TestSourceContext|TestContextInsertClean)' -count=1
```

## 9. <a id="appendix-ce-feasibility-proofs"></a>Appendix: CE Feasibility Proofs

As of 27 September 2026, the isolated proof for direct Bind log sites passes. It is in [context_enrichment_poc_test.go](../../internal/id/context_enrichment_poc_test.go). The production option `trice bind -ce` has since been implemented on this foundation; usage and acceptance are described in [this chapter](#trice-context-enrichment). This appendix also contains the original rebase counterexample and the [extended PoC for wrapper macros and counter rebasing](#extended-poc-for-wrapper-macros-and-counter-rebasing). The latter informs a future decision and enables no additional production CE support.

### 9.1. <a id="mechanism-under-test"></a>Mechanism Under Test

The existing Bind descriptor contains both the ID and a macro to apply. At selected log sites, the PoC uses a generated adapter macro that accepts the original arguments and appends the context expressions. These expressions are evaluated only during expansion of the original Trice call, where they can access local variables.

For an originally argument-free log site, the adapter is equivalent to:

```c
#define TRICE_CE_POC_SITE(ignoredImplementation, tid, format) \
    TRICE_INSERT_trice(tid, format, (x))
```

For calls with existing arguments, the adapter macro receives appropriate additional parameters. Each appears exactly once in the final call. Generic Trice macros determine arity from the extended argument list. For fixed-arity macros, the adapter selects the appropriate implementation, for example `TRICE_INSERT_trice_1` for an extended `trice_0` site.

The test first creates the extended template string and additional arguments solely in a private in-memory source view. Existing `SubCmdIdBind` runs on that view with the normal Structured Logging parser and ID assignment. The ID is therefore determined from the final schema. The compiler still sees unchanged user source and the generated sidecar with adapter macros. This is a test adapter for proving the architecture, not the production CE integration.

The fixture already contains the regular Bind sidecar include and a fixed file key. The test therefore checks the source preservation required by CE independently of initial Bind project setup.

### 9.2. <a id="verified-behavior"></a>Verified Behavior

| Case | Expected result | Evidence |
|---|---|---|
| Argument-free `trice` | Local `x` is transmitted as an additional value | Binary record contains `7` |
| Already parameterized `trice` | Original value precedes CE value | Binary record contains `1, 1` |
| Simple expression | `x + 1` is evaluated at the call site | Binary record contains `8` |
| Fixed arity | `trice_0` and `trice_1` receive the appropriate final arity | Binary records contain `7` and `22, 7` respectively |
| Inner block scope | Uses `blockValue`, visible only there | Binary record contains `11` |
| Unselected log site | No additional values | Binary record remains argument-free |
| Side effects | Original and CE expressions are each evaluated exactly once | Two separate runtime counters equal `1` |
| Unexecuted call | No record or CE side effect | Seven records despite eight instrumented sites; CE counter remains `1` |
| TIL consistency | Final templates, arity, IDs and payload agree | Explicit template expectations and the actual Trice record parser/resolver |
| Repetition | Identical IDs and generated contents | Byte comparison of source, configuration, TIL, LI, sidecar and field registry after two PoC Bind runs |
| Invalid context | An invisible identifier is diagnosed | Compiler and `clangd` reject `ceMissingLocal` |

The runtime test uses the actual target macros and Trice library. Auxiliary output delivers the generated binary records to `TriceParseRecord`; `TriceResolveLog` checks them against a C metadata table generated from the final TIL. This also detects incorrect payload lengths or parameter counts. The PoC generates that C table directly from TIL; it does not test the public `generate -logC` workflow.

### 9.3. <a id="compiler-and-editor-diagnostics"></a>Compiler and Editor Diagnostics

The test compiles and runs the same fixture as C11 and C++17 with `-Wall -Wextra -Werror`. Library sources are compiled as C. For both language modes, it creates a `compile_commands.json` with the actual compiler arguments. `clangd --check` must load this database and complete without errors. The negative test additionally demonstrates that missing context identifiers remain visibly diagnosed.

Tested environment: macOS on ARM64, Apple Clang/Clang++ 21.0.0 and Apple clangd 21.0.0. Both language modes and negative diagnostic checks passed. This demonstrates the language-server path for clangd-based editors with the generated include directory and real compile configuration. Other language servers, IDE-specific parsers, GCC and MSVC were not tested in this direct-site proof.

### 9.4. <a id="reproducing-the-direct-site-proof"></a>Reproducing the Direct-Site Proof

Run from the repository root:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id -run '^TestContextEnrichmentPoC$' -count=1 -v
```

A GCC-/Clang-compatible C and C++ compiler and `clangd` must be in `PATH`. The focused test explicitly requires these tools. Without `TRICE_BIND_INTEGRATION=1`, it is skipped. All fixtures and build artifacts are created in a temporary test directory and removed afterwards. Existing repository workflows are not modified.

### 9.5. <a id="relationship-to-production-support"></a>Relationship to Production Support

The proof covers the minimum cases from the [CE chapter](#trice-context-enrichment). It checks direct scalar 32-bit log sites with one site per physical line and the `iD` stamp type. PoC rules are fixed test data; the direct-site proof contains no CLI parser, complete selector/alias policy or production error validation.

The production implementation applies the transformation before schema/ID assignment and generates the sidecar extension persistently. Its first stage remains limited to direct sites uniquely addressable by source line. Additional [Bind behavior tests](../../internal/id/contextEnrichment_test.go) and [CLI/target tests](../../internal/args/context_enrichment_test.go) check broader acceptance separately from the direct-site PoC: 8/16/32/64 bits, different stamp types, `TRICE_OFF`/`TRICE_CLEAN`, rule conflicts, configuration changes, multiline calls, inline functions and real compiler/editor runs without `__COUNTER__`. Four examples come from `triceCheck.c`; fourteen records in total pass through the public `generate -logC` resolver and Go decoder for text, JSON and KV. CE for wrapper macros and counter rebasing remains a separate follow-up task.

For supported source constructs and practical alternatives, see [Bind Limits](bind.md#bind-limits). The feasibility tests below do not enable CE for Bind wrappers or counter-rebase sites.

### 9.6. <a id="counterexample-for-the-original-rebase-approach"></a>Counterexample for the Original Rebase Approach

On 27 September 2026, direct transfer of the adapter approach to counter rebasing was tested. The additional `TestContextEnrichmentPoCRebaseScopeBoundary` test shows a limitation: two log sites supported by Bind, on the same source line, occupy separate blocks and each use a variable visible only in that block. The normal Bind build passes. Inserting both CE expressions into their respective branches of the generated rebase dispatcher makes compilation fail on names belonging to the other scope.

The reason is ordinal selection in C: even an `if` branch not selected at runtime is checked by the compiler for valid identifiers. The affected expressions are valid at their intended log sites. The error would therefore be an invalid additional scope requirement imposed by instrumentation. The test expects and demonstrates precisely this failed extension; it is not a successful CE rebase acceptance test.

The direct-site proof remains valid. Simply appending CE arguments to rebase branches is insufficient for general CE support, however. On 27 September, the first implementation stage was therefore limited to direct Bind sites uniquely addressable by source line. Production CE rejects selected wrapper/rebase sites before modifying files and points to the explanation in the Reference Manual with `Search UM for "bind-limits".` Without a matching CE rule, existing Bind capabilities remain available. The additional architecture proof for complex CE sites was deferred; this counterexample alone does not establish that a later solution is impossible.

The counterexample can be reproduced separately:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id -run '^TestContextEnrichmentPoCRebaseScopeBoundary$' -count=1 -v
```

### 9.7. <a id="extended-poc-for-wrapper-macros-and-counter-rebasing"></a>Extended PoC for Wrapper Macros and Counter Rebasing

**Result:** Selecting the CE adapter in the preprocessor eliminates the demonstrated problem with local variables from other scopes. The new [context_enrichment_rebase_poc_test.go](../../internal/id/context_enrichment_rebase_poc_test.go) test demonstrates a working approach with an additional compiler preprocessing pass. It also contains a simpler special case: a wrapper with exactly one log site can be enriched without this pass and without `__COUNTER__` if its line mapping is unambiguous. Both remain test code; `bind -ce` still rejects the previously excluded constructs.

#### 9.7.1. <a id="how-the-investigated-approach-works"></a>How the Investigated Approach Works

With the existing C branching, expressions from all possible log sites reach the compiler. In the new PoC, macro expansion selects exactly one adapter. Only that adapter's expressions appear in the final C/C++ code. A wrapper can therefore use `branchLeft` in its left branch and `branchRight` in its right branch without either name needing to exist in the other block.

The mapping requires a value that the preprocessor can use directly as part of a macro name. The existing relative calculation from `__COUNTER__` and a C enum constant is unsuitable. The PoC therefore determines actual absolute counter values with the compiler being used:

1. Existing Bind sets up the temporary test sources normally. A private source view receives CE extensions and goes through existing schema/ID assignment. The user source actually compiled retains its original Trice calls and wrapper definitions.
2. The compiler preprocesses these sources with the actual language, target and preprocessor options. A file included only for the test emits a marker with the region and counter value for each rebase expansion.
3. The test maps these markers to numeric definition/location descriptors and expansion order from the generated Bind sidecar. It does not guess IDs from format strings. This produces a header with exactly one macro per observed expansion.
4. During normal compilation, `__COUNTER__` selects this macro. It passes existing arguments and adds only the associated CE expressions. Existing rebase end checks remain active. An additional base-value check rejects a shifted mapping even if it would happen to hit another valid entry.

This preliminary pass is required per translation unit and concrete build configuration. Here, a translation unit is, for example, a `.c` or `.cpp` file together with its included headers. The same shared wrapper may therefore need different counter mappings for different translation units while retaining the same logical IDs.

#### 9.7.2. <a id="verified-cases"></a>Verified Cases

| Case | Verified result |
|---|---|
| Simple wrapper with one log site | CE at the call site works even with `__COUNTER__` removed; an ordinary line descriptor suffices. |
| Wrapper with two log sites | Original values precede their CE values; generic and fixed arity work. |
| Repeated wrapper calls | The same logical IDs transmit different local context values from their callers. |
| Wrapper with separate branches | `branchLeft` and `branchRight` remain confined to their own blocks. |
| Two direct calls on the same line | Separate blocks with `onlyLeft` and `onlyRight` work without imposing visibility requirements from other scopes. |
| Unselected rebase sites | Existing records remain unchanged and have no additional values. |
| Side effects | Original and CE expressions are each evaluated exactly once per executed call. An unexecuted wrapper call has no effect. |
| Actual records | Eleven records are produced by the real target library, resolved against the final C TIL, and checked for IDs, parameter counts and all ordered values. |
| Repetition | Repeated private Bind generation preserves schema, IDs, region mapping and source. The same compiler pass reproduces the same mapping header. |
| Other counter uses before log sites | Additional uses at offsets 0, 1 and 7 change the mapping while IDs and expected records remain the same. |
| Stale mapping header | A normal build fails. In particular, a shift by one is detected even when it could otherwise hit an adjacent valid adapter. |
| Missing context identifier | The compiler and tested `clangd` variants report `cePocMissingLocal` as a real error. |
| Additional counter consumption in a CE expression | Rejected; the investigated preliminary pass assumes one counter per rebase expansion. |
| `TRICE_OFF` and `TRICE_CLEAN` | No records and no argument/CE evaluation, even without available `__COUNTER__`. |
| Active rebase without `__COUNTER__` | Clear build error including `Search UM for "bind-limits".` The PoC claims no solution for this case. |

The runtime proof uses scalar 32-bit records with `iD`. It does not replace the broader bit-width/stamp acceptance of production CE and is not complete acceptance of every conceivable wrapper.

#### 9.7.3. <a id="compiler-matrix-and-evidence-limits"></a>Compiler Matrix and Evidence Limits

On 27 September 2026, the following installed tool variants were tested:

| Tool and target | Language modes | Evidence |
|---|---|---|
| Apple Clang/Clang++ 21.0.0, macOS ARM64 | C99, C11, C17, C++11, C++17 | Preprocessing, compilation with `-Wall -Wextra -Werror`, linking and actual program execution passed. |
| ARM GNU Toolchain 13.3.Rel1, GCC/G++ 13.3.1, Cortex-M0/Thumb | C99, C11, C17, C++11, C++17 | Preprocessing and generation of real ARM object files with `-Wall -Wextra -Werror` passed; no execution on an MCU or emulator. |
| Same ARM GCC version, Cortex-M4/Thumb | C99, C11, C17, C++11, C++17 | Preprocessing and generation of real ARM object files passed; no target runtime claim. |
| All three configurations | C++20 | Existing Bind code fails even without experimental CE on an enum warning promoted by `-Werror`. With only that warning downgraded to warning status, the CE PoC passes; Clang includes runtime execution, ARM GCC produces object files. |
| Apple clangd 21.0.0 | Host C11 and host C++17 | Loads a real `compile_commands.json` including the experimental header. Valid sources are error-free; the deliberately missing identifier is diagnosed. |

This covers 18 combinations of toolchain/target and language mode, each with three initial counter states and additional negative and disabled-build cases. In this environment, the macOS `gcc`/`g++` commands are Clang aliases and explicitly do not count as GCC evidence. Independent GCC evidence comes from the ARM cross-compiler. Native GCC variants are also included in the test matrix when available. MSVC, IAR, Arm Compiler/armclang and other language servers were not tested; their support cannot be inferred from these results.

The C++20 limitation comes from the existing rebase check: it subtracts values of different anonymous enum types. The test first demonstrates this with ordinary Bind, then uses only `-Wno-error=deprecated-anon-enum-enum-conversion` for Clang or `-Wno-error=deprecated-enum-enum-conversion` for GCC. This is explicitly not a successful strict C++20 build. Product code was not changed for the PoC. When simulating an unavailable `__COUNTER__`, the warning about undefining a built-in macro is also not treated as an error.

#### 9.7.4. <a id="implications-for-a-possible-implementation"></a>Implications for a Possible Implementation

**The scope obstacle is solved for the tested cases; the cost of this approach is an additional compiler-dependent build step.** A production decision must explicitly include this effort. The PoC provides neither such a workflow nor a new CLI option.

Before implementation, the following points in particular would need decisions or evidence:

- Integrating the preliminary pass into supported build systems, with the same defines, include paths, language modes and target options as the actual compilation.
- Separate mapping artifacts per translation unit and configuration, and reliable regeneration after relevant changes. Counter checks detect shifts but do not replace complete build dependency checking or protection against arbitrarily damaged artifacts.
- Behavior with precompiled headers, modules, additional counter uses in argument macros and other compiler families. The PoC makes no guarantees for these.
- If strict C++20 builds are a goal, a separate correction and acceptance of the existing enum rebase check.
- Deciding whether the simpler extension for uniquely addressable wrappers with one log site should be implemented independently of general CE rebasing first. The PoC proves this case without a counter preprocessing pass; enabling it in production remains a separate task.

A second compiler pass is therefore a demonstrated possibility, not a claim that no simpler approach could exist. The initial direct-site CE stage remains unchanged. This PoC does not investigate source transformation. The now available `insert/clean -ce` is implemented separately and described above with its own tests.

#### 9.7.5. <a id="repeating-the-extended-poc"></a>Repeating the Extended PoC

Run from the repository root:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id -run '^TestContextEnrichmentRebasePoC$' -count=1 -v
```

The test detects installed Clang/GCC C/C++ toolchains and ARM GCC itself, reports missing tools and compiler aliases, and installs nothing. At least one suitable C/C++ toolchain is required. Without `TRICE_BIND_INTEGRATION=1`, the compiler test is skipped. `clangd` is checked when available; a missing tool is explicitly reported. All source copies, experimental headers and build artifacts are created in temporary test directories. The production CLI, target headers and build scripts remain unchanged.

## 10. <a id="approach-and-boundaries"></a>Approach and Boundaries

CE is optional build-time instrumentation: rules select log sites whose records contain additional runtime values. It introduces no general implicit context state. There are therefore no push/pop calls, task-local state or context handles requiring special management during task switches or interrupts.

Other logging systems offer related but differently structured concepts, such as [Go `slog.Logger.With`](https://pkg.go.dev/log/slog), [Microsoft `ILogger.BeginScope`](https://learn.microsoft.com/dotnet/api/microsoft.extensions.logging.ilogger.beginscope), [Serilog `LogContext`](https://github.com/serilog/serilog/wiki/Enrichment) and [Rust `tracing` spans](https://docs.rs/tracing/latest/tracing/span/). These references imply no Trice dependencies or compatibility guarantees.

