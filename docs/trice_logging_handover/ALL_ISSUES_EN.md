# Trice logging issue drafts

**Date:** 10 September 2026. **Target branch:** `wip`.

This is an anthology of 27 standalone issue drafts. Each original file is linked at the start of its entry. Local L-numbers are not GitHub issue numbers. Deferred proposals are not immediate implementation requests. Acceptance checkboxes are not executed tests.

---

**Standalone file:** [L01](issues/L01_unique_tag_aliases.md)

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

**Standalone file:** [L02](issues/L02_inclusive_routing_bounds.md)

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

**Standalone file:** [L03](issues/L03_validate_active_id_policy.md)

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

**Standalone file:** [L04](issues/L04_safe_id_policy_migration.md)

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

**Standalone file:** [L05](issues/L05_ulabel_registration_order.md)

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

**Standalone file:** [L06](issues/L06_ulabel_colon_lists.md)

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

**Standalone file:** [L07](issues/L07_reject_unknown_selectors.md)

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

**Standalone file:** [L08](issues/L08_reject_conflicting_filters_early.md)

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

**Standalone file:** [L09](issues/L09_consistent_routing_configuration.md)

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

**Standalone file:** [L10](issues/L10_single_pack_guard_tests.md)

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

**Standalone file:** [L11](issues/L11_loglevel_semantics.md)

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

**Standalone file:** [L12](issues/L12_event_metadata_filtering.md)

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

**Standalone file:** [L13](issues/L13_fragment_and_multiline_policy.md)

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

**Standalone file:** [L14](issues/L14_diagnostic_filter_isolation.md)

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

**Standalone file:** [L15](issues/L15_raw_capture_replay_guarantee.md)

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

**Standalone file:** [L16](issues/L16_statistics_semantics.md)

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

**Standalone file:** [L17](issues/L17_early_host_filtering.md)

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

**Standalone file:** [L18](issues/L18_central_manual_chapter.md)

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

**Standalone file:** [L19](issues/L19_deferred_compile_time_levels.md)

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

**Standalone file:** [L20](issues/L20_deferred_runtime_levels.md)

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

**Standalone file:** [L21](issues/L21_clarify_logging_terminology.md)

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

**Standalone file:** [L22](issues/L22_reversible_enrichment_instrumentation.md)

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

**Standalone file:** [L23](issues/L23_validate_draft_examples.md)

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

**Standalone file:** [L24](issues/L24_separate_parser_chapter.md)

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

**Standalone file:** [L25](issues/L25_server_use_evidence.md)

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

**Standalone file:** [L26](issues/L26_structured_logging_feasibility.md)

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

**Standalone file:** [L27](issues/L27_reject_malformed_idrange.md)

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

