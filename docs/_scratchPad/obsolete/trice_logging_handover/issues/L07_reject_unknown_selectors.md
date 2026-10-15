# Reject unknown or malformed filter selectors before starting logging

**Local reference:** L07  
**Type:** Bug / validation  
**Suggested priority:** P1 - prevent silent selection changes  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Source-confirmed silent dropping for pick/ban; logLevel also lacks value validation.

## Summary

A mistyped selector must fail clearly instead of silently changing the requested selection.

## Background and evidence

`channelArrayFlag.Set` expands only recognized aliases and returns no error for an unknown name. If every `-pick` item is unknown, the internal selection can remain nil and be treated as no filter at all. An unknown item mixed into a valid list is silently lost. `-logLevel` is accepted as an arbitrary string and later falls through table-index logic; the help even describes an invalid-value side effect rather than an error.

## Reproduction or review scenario

Test `-pick erorr`, `-ban erorr`, `-pick err:erorr`, empty elements such as `-pick err:`, and `-logLevel erorr`. Include a valid user label declared elsewhere in the same command. Run parser tests without a connected target.

## Requested change

Validate all supplied selector tokens after the complete registry is built. For logLevel, preserve explicitly supported control values such as `all` and `off`; resolve other supported values under the chosen compatibility policy. Return nonzero with the offending token and useful valid alternatives. Similar-name suggestions may help, but must not auto-correct.

## Acceptance criteria and regression tests

- [ ] Unknown pick/ban names fail rather than becoming no filter or a partial filter.
- [ ] Unknown logLevel values fail without starting acquisition.
- [ ] Empty tokens and malformed lists are consistently rejected or explicitly documented.
- [ ] Valid custom labels are accepted independent of argument order.
- [ ] A known selector with zero matching events is not considered invalid.
- [ ] Failure occurs before opening ports, creating/appending log files, or entering the read loop.

## Compatibility and scope boundaries

Rejecting typos intentionally changes erroneous commands. Do not silently remove support for legacy valid logLevel categories in the same patch; coordinate with the severity migration.

## Source references

- [internal/emitter/emitter.go](https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go) - `channelArrayFlag.Set`, `banOrPickFilter`
- [internal/args/init.go](https://github.com/rokath/trice/blob/wip/internal/args/init.go) - `logLevel` registration and help
- [internal/args/handler.go](https://github.com/rokath/trice/blob/wip/internal/args/handler.go) - `runLog`

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L05](L05_ulabel_registration_order.md), [L06](L06_ulabel_colon_lists.md), [L08](L08_reject_conflicting_filters_early.md), [L11](L11_loglevel_semantics.md). Local references are handover identifiers, not existing GitHub issue numbers.
