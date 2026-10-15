# Coverage map

This maps the maintainer's last numbered checklist, including repeated numbers, and the earlier relevant design discussion to the standalone files.

| Discussion item | Preserved content | Issue(s) |
|---|---|---|
| 1. Preserve the design | Decision record sections 1 and 7; chapter design principle | Decision record / chapters |
| 2. Defer trice_INFO compile-time selection | Existing TRICE_OFF distinguished from the proposed extra mechanism | [L19](../issues/L19_deferred_compile_time_levels.md), [L18](../issues/L18_central_manual_chapter.md) |
| 3. Defer dynamic target level control | Resource-driven reopening; routing is not the same as early suppression | [L20](../issues/L20_deferred_runtime_levels.md), [L18](../issues/L18_central_manual_chapter.md) |
| 4. Duplicate aliases and table-wide test | All identified spellings, case behavior, and user-label collisions | [L01](../issues/L01_unique_tag_aliases.md) |
| 5a. Inclusive ID bounds | Allocation/output consistency and single-ID ranges | [L02](../issues/L02_inclusive_routing_bounds.md) |
| 5b. Check changed ID policy | Active versus historical assignments; partial scans; optional strict checking | [L03](../issues/L03_validate_active_id_policy.md) |
| 5b. Offer a safe fix | Explicit preview/migration, metadata coherence, historical mappings | [L04](../issues/L04_safe_id_policy_migration.md) |
| 6. User-label implementation | Registration order and documented colon-list parsing | [L05](../issues/L05_ulabel_registration_order.md), [L06](../issues/L06_ulabel_colon_lists.md) |
| 7. Reject unknown selectors | Includes empty/malformed lists and invalid logLevel values | [L07](../issues/L07_reject_unknown_selectors.md) |
| 7. Reject pick plus ban early | Empty input and invalid-token corner cases | [L08](../issues/L08_reject_conflicting_filters_early.md) |
| 7. Move host filtering earlier | Measured optimization after semantics are defined | [L17](../issues/L17_early_host_filtering.md) |
| 8. Single-pack requirement | Existing errors, not a missing check; positive/negative test coverage | [L10](../issues/L10_single_pack_guard_tests.md) |
| 8. Routing activation details | UARTA/UARTB difference; partial/zero/default bounds | [L09](../issues/L09_consistent_routing_configuration.md) |
| 8. Severity and logLevel retention | Category/severity separation; custom labels; deliberate compatibility decision | [L11](../issues/L11_loglevel_semantics.md) |
| 9. Central bilingual UM chapter | H2/H3/H4; existing options and unimplemented drafts; integration instructions | [L18](../issues/L18_central_manual_chapter.md) |
| 10. Metadata consistency | An accepted event and its metadata use a coherent decision | [L12](../issues/L12_event_metadata_filtering.md) |
| 10. Partial lines and multi-line events | Compatibility, EOF, addNL, metadata origin | [L13](../issues/L13_fragment_and_multiline_policy.md) |
| 10. Diagnostics | Separate tool/decode policy and future machine-readable integrity | [L14](../issues/L14_diagnostic_filter_isolation.md) |
| 10. Raw recording and replay | Preserve pre-filter capture; append and dictionary precautions | [L15](../issues/L15_raw_capture_replay_guarantee.md) |
| 10. Statistics | Count semantics independent of presentation and filter optimization | [L16](../issues/L16_statistics_semantics.md) |
| Earlier discussion: terminology | Enrichment is not structured logging; exact UM section map | [L21](../issues/L21_clarify_logging_terminology.md) |
| Earlier discussion: unsafe draft reversal | Provenance, bind compatibility, interrupted builds, caches | [L22](../issues/L22_reversible_enrichment_instrumentation.md) |
| Earlier discussion: example correctness | C/shell/JSON/stringization, serialization, compression qualifications | [L23](../issues/L23_validate_draft_examples.md) |
| Earlier discussion: separate parser task | Independent chapter and narrow typed-event interface boundary | [L24](../issues/L24_separate_parser_chapter.md) |
| Earlier discussion: server usefulness | Device ingestion versus native server logging, evidence and benchmarks | [L25](../issues/L25_server_use_evidence.md) |
| Earlier discussion: strice usefulness and key IDs | Single ID with many fields; no duplication; host transform; typing and wrappers | [L26](../issues/L26_structured_logging_feasibility.md) |
| Newly rechecked detail | Malformed IDRange without colon is silently skipped | [L27](../issues/L27_reject_malformed_idrange.md) |

## No lost distinctions

Existing behavior is not presented as a new feature request merely because it needs tests. Deferred design records are not immediate implementation tickets. Recommendations are not merged changes. The chapter does not claim that proposed CLI validation, a new severity mapping, or ID migration tools already exist.

The English decision record preserves the broader motivation, alternative interfaces, key-ID discussion, reference systems, known draft problems, and open questions for a later specification. It is the continuation context, not a verbatim conversation transcript.
