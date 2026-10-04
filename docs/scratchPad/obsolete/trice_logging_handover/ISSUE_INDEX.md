# Issue index

All issue files are in English and can be copied independently. Use the first heading as the GitHub title and the remainder as the body. L01-L27 are local handover references, not already-created GitHub issues. Priorities and labels are suggestions.

## Current behavior, safety, and host optimization

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

## Central documentation and deferred target proposals

| Ref | Standalone issue | Classification |
|---|---|---|
| L18 | [Add a central manual chapter for log levels, tags, filtering, and ID routing](issues/L18_central_manual_chapter.md) | Documentation |
| L19 | [Record the deferred proposal for level-specific compile-time selection](issues/L19_deferred_compile_time_levels.md) | Deferred design proposal - not a bug |
| L20 | [Record the deferred proposal for dynamic target-side log-level control](issues/L20_deferred_runtime_levels.md) | Deferred design proposal - not a bug |

## Broader design context retained from the discussion

| Ref | Standalone issue | Classification |
|---|---|---|
| L21 | [Rename the enrichment draft and separate levels, enrichment, and structured logging](issues/L21_clarify_logging_terminology.md) | Documentation / conceptual correction |
| L22 | [Remove unchanged-configuration heuristics from the future enrichment transformation contract](issues/L22_reversible_enrichment_instrumentation.md) | Draft architecture risk |
| L23 | [Validate the draft C, shell, JSON, and macro-stringization examples before publication](issues/L23_validate_draft_examples.md) | Documentation correctness |
| L24 | [Move internal parser development into a separate chapter and track it independently](issues/L24_separate_parser_chapter.md) | Documentation organization / scope control |
| L25 | [Separate device-log ingestion from native server logging and qualify performance claims](issues/L25_server_use_evidence.md) | Documentation / feasibility evidence |
| L26 | [Validate a useful structured-event workflow before specifying a strice extension](issues/L26_structured_logging_feasibility.md) | Exploratory feasibility - not an implementation specification |

## Additional source finding during handover

| Ref | Standalone issue | Classification |
|---|---|---|
| L27 | [Reject malformed IDRange arguments instead of silently skipping them](issues/L27_reject_malformed_idrange.md) | Bug / validation - additional finding during handover review |

## Interpretation

L19 and L20 deliberately preserve a deferred state. L22 and L26 are design/feasibility tasks, not an approved implementation specification. L10 and L15 protect behavior that already exists. L14 records a diagnostic-isolation risk supported by a shared path, not an executed proof that every error is suppressed. L27 was newly noticed while rechecking the source for this package.
