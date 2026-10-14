# Validate the draft C, shell, JSON, and macro-stringization examples before publication

**Local reference:** L23  
**Type:** Documentation correctness  
**Suggested priority:** P2 for publication; does not block current logging  
**Target branch:** `wip`  
**Review date:** 2026-09-10  
**Evidence/status:** Draft defects were identified in the review; examples were not executed against a complete checkout in this handover.

## Summary

Prevent examples in the future enrichment chapter from appearing executable or format-safe when their syntax or assumptions are invalid.

## Background and evidence

The reviewed discussion identifies malformed quoting in JSON-like templates and one generated C call; accidental shell backticks and unsafe unquoted expansions; and a `#pragma` example that expects `XSTR(TRICE_ETC)` to become empty after `#undef`. An undefined token is instead stringized as its name. A valid final JSON illustration does not validate the template that allegedly produced it.

A printf-style template is not a JSON serializer: static or dynamic text can contain quotes, backslashes, and control characters. The draft's speculative TCOBS size reduction must not be stated as a guaranteed ratio.

## Reproduction or review scenario

Extract every purportedly executable example from the current chapter and classify it as real, illustrative, or dependent on unimplemented options. Compile/preprocess actual C examples, parse shell examples, and validate produced JSON. Use an undefined macro, embedded quotes, a backslash, newline, and representative numeric edge values.

## Requested change

Repair syntax and mark non-executable proposals clearly. Specify any multi-stage template substitution before giving output claims. Replace unsafe JSON construction with a serializer in future host-output design. Move the specialized compiler-diagnostic/pragma technique to an optional experiment or clearly label its portability limits.

## Acceptance criteria and regression tests

- [ ] Every code block is either validated with its dependencies or explicitly illustrative.
- [ ] The undefined-macro example shows the correct token-stringization result.
- [ ] Shell examples avoid accidental command substitution and word splitting.
- [ ] JSON examples distinguish templates from serialized data and validate escaping.
- [ ] Integer types, formatting precision, units, NaN/infinity, and large-integer policy are not silently conflated.
- [ ] Compression-size and zero-cost claims are conditional or backed by a reproducible measurement.
- [ ] The draft does not suggest that dynamic-string hashing preserves the original string content.

## Compatibility and scope boundaries

Do not implement stf/stv merely to make documentation examples runnable. This issue groups the executable-example quality problem; the reversibility architecture is separate.

## Source references

- [docs/TriceUserManual.md](https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md) - enrichment examples, user-defined values, CLI configuration and byte-count discussion
- [GCC stringization rules](https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html)
- [JSON syntax and interoperability rules](https://www.rfc-editor.org/rfc/rfc8259.html)

References identify the web-retrieved `wip` source reviewed on the date above; they are not immutable commit permalinks. Reconfirm the finding in the implementation checkout and record its commit before changing code. No upstream Go suite, target build, or hardware test was executed for this handover.

## Related local issues

[L21](L21_clarify_logging_terminology.md), [L22](L22_reversible_enrichment_instrumentation.md), [L26](L26_structured_logging_feasibility.md). Local references are handover identifiers, not existing GitHub issue numbers.
