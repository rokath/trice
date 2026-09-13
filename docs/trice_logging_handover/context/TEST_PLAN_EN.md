# Consolidated test and implementation plan

## Status

This is a plan, not an execution report. No listed upstream test has been run as part of this handover. Use the actual `wip` checkout, record its commit, and first reproduce each selected source finding. Reuse existing project fixtures rather than introducing a parallel testing framework.

## A. Tag and CLI tests

Validate the full exact-spelling registry, not a hard-coded blacklist of current duplicate aliases. Test built-in canonical names, case variants, all known collisions, user labels, and repeat registration. Restore global state between independent test runs; do not depend on process-wide initialization from earlier cases.

Test both option orders for user-label selection; repeated `-ulabel`; colon lists; duplicate/empty items; unknown pick/ban/logLevel names; a known but absent category; and simultaneous pick plus ban with both empty and nonempty input. Assert no port/file is opened on invalid configuration. Exercise both `tlog` and `trice log` entry points.

For IDRange, test missing colon/comma, empty parts, invalid numbers, reversed bounds, unknown names, alias-equivalent duplicate rules, touching/overlapping ranges, and legal adjacent disjoint ranges. A valid rule must not hide an invalid one.

## B. Target routing and compile matrix

For each relevant UART, auxiliary, and RTT backend, test absent/default routing, complete routing, partial bounds, reversed intervals, one-ID intervals, and legal extreme IDs. Include IDs `min-1`, `min`, an interior value, `max`, and `max+1` whenever legal in the test representation. Capture actual output callbacks.

Build the valid single-pack configurations and reject the incompatible active deferred multi-pack configurations. Confirm the expected error text; do not accept unrelated compile failures as success. Include valid multi-pack without ID routing. Audit custom/direct paths under their own contract, not by assuming every route is identical.

Where practical, cover both deferred buffer implementations and the relevant framing choices. Make sure inclusive comparisons do not accidentally activate disabled/default routes.

## C. ID policy and migration

Use current sources and generated assignments to classify active versus historical-only entries. Include a partially scanned project and stale location metadata. Test insert and bind independently with an existing assignment outside a changed policy. Pure validation must not write anything.

For any later explicit fix: preserve old mappings, preview every affected artifact, allocate only unused IDs, test exhaustion and write failure, use concurrency protection for shared state, and verify old/new recordings against the appropriate dictionaries. Do not claim a multi-file operation is atomic without a real transaction/recovery design.

## D. Host acceptance and rendering

Capture legacy behavior before changing severity semantics. Cover every built-in group, aliases, custom/unknown categories, untagged data, all/off, representative thresholds, and explicit tag selections. Choose and approve the no-severity policy before golden outputs are changed.

Compare color off/none/default; all metadata columns; prefix/suffix; local and remote rendering where supported; partial lines; multiple events per line; multiple lines per event; hidden fragments; EOF flush; addNL; and ID/location attribution. Define visualization interaction explicitly instead of silently changing it as an optimization side effect.

## E. Integrity, diagnostics, and recording

Use valid events interleaved with unknown IDs, malformed and truncated input, and supported integrity/cycle errors. Restrictive application filters must not erase the information needed to diagnose failure unless an explicit diagnostic policy says so. Any future JSON or similar stream must remain syntactically valid and not contain injected console banners.

For byte-exact binary capture tests, use fresh distinct output files. Compare the same received input under different host selectors and replay it with matching metadata. Never append into the replay input. Test recording write errors and incomplete input. Capture is limited to bytes actually received.

Count decoded, selected, displayed, and diagnostic events according to documented semantics. Color and metadata fragments must not multiply application-event counts. Preserve the reference used for displayed timestamp deltas across hidden events.

## F. Performance gates

Benchmark representative inputs at several suppression ratios, including no suppression. Record CPU time, allocations, throughput, output equivalence, and configuration. The early-filter implementation may avoid rendering, but still has to consume and validate the stream and maintain protocol state.

No target-level controller is to be added as part of the host optimization. For either deferred target proposal, first provide event-rate, bandwidth/storage, code-size, or argument-cost evidence. Compare the same retained information and configuration; do not infer a general speed or energy improvement from a small microbenchmark.

## G. Suggested sequencing

1. Establish a reproducible baseline and fix independent CLI/registry/routing correctness defects.
2. Add active ID-policy diagnostics; decide separately whether an explicit migration tool is needed.
3. Preserve packing guards, raw capture, diagnostics, and accounting boundaries with tests.
4. Approve and implement any host severity/metadata/fragment compatibility change.
5. Measure and implement earlier host filtering only if worthwhile.
6. Update the central chapter and known-issue notes alongside verified changes.
7. Keep target-level proposals deferred; evaluate structured-field usefulness in a small real workflow before writing a detailed extension specification.
