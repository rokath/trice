# Artifact validation

**Executed:** 10 September 2026, in the document-generation container.

This reports checks of the handover files, not tests of Trice.

## Results

- 27 standalone English issue files found, with unique L01-L27 identifiers.
- Each issue has a single H1 title and the required evidence, scope, reproduction/review, acceptance, compatibility, and source sections.
- Acceptance-test checkboxes remain unchecked.
- Both standalone chapter files start at H2 and use only H2/H3/H4.
- The English and German chapter files have identical explicit section-anchor sequences.
- 37 Markdown files parsed successfully with CommonMark/table support before adding this report.
- 1010 parsed links inspected; relative file targets resolved, including the report emitted in this step.
- No duplicate explicit anchors, duplicate parsed reference definitions, unbalanced triple-backtick fences, replacement characters, or unresolved Unicode escapes found.
- No conversation-only web/file citation IDs remain in the delivered Markdown. Sources use ordinary persistent URLs and named code symbols.
- Combined documents rewrite relative links and namespace reference labels/explicit anchors as needed.

## Not covered

No full Trice checkout, upstream Go suite, target compilation, hardware execution, performance benchmark, or end-to-end command reproduction was completed. External links were consulted during source review, but local artifact validation is not a permanent external link-availability guarantee. The moving branch was not pinned to a certified commit snapshot.

The ZIP archive is separately checked for CRC/read errors after creation; a SHA-256 manifest is included for its file contents.
