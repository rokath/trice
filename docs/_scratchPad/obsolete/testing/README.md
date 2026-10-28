# Retired test documentation

Use [Testing Trice — Running the Tests](../../../rm/target_tests.md) for local commands, OS prerequisites, GitHub Actions and release checks. Use [Testing Trice — Internals](../../../rm/target_test_internals.md) for the CGO architecture and configuration catalogue.

The following originals are retained as history. Their instructions, relative links and recorded results may describe an older checkout; they are not current test guidance.

| Archived source | Original location and replacement |
| --- | --- |
| [Target-test chapter](target_tests.md) | `docs/rm/target_tests.md`; replaced by the two chapters above. Includes the historical 2024 output. |
| [Workflow overview](github_workflows_README.md) | `.github/workflows/README.md`; current test workflow details are in [GitHub Actions](../../../rm/target_tests.md#github-actions), with repository automation in [Maintenance](../../../rm/maintenance.md#github-workflows). |
| [Native-test design](Native_Test_Gesamtloesung_DE.md) | `docs/_scratchPad/Native_Test_Gesamtloesung_DE.md`; replaced by the implemented behavior documented in the two chapters. |
| [Extracted instructions](test_instructions_excerpts.md) | Replaced sections from maintenance, tool build/release, contribution guidance, ID migration, development setup and CE testing; includes old transcripts, the snapshot procedure and test-default claims. |
| [CGO package READMEs](cgo) | Moved from `_test/<package>/ReadMe.md` or `README.md`, retaining each package name. These include 61 identical maintenance notes and the ABC transmitter description. Current guidance is in [adding tests](../../../rm/target_test_internals.md#add-or-change-a-test) and the [configuration catalogue](../../../rm/target_test_internals.md#cgo-configuration-catalogue); `_test/ReadMe.md` remains the central source-tree entry point. |
