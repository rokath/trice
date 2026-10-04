# Manual Reference Assets

This directory contains the images, diagrams, generated command-line help, and
small reference files used by the Trice manuals and README. It is the asset
base directory for the PDF renderer, so manual links such as `./ref/name.png`
remain valid in both GitHub and generated PDFs.

## Maintained asset families

| Family | Role |
| --- | --- |
| `trice_abc_core_workflow.drawio` and `trice_abc_demo_bus.drawio` | Editable sources for the Trice ABC diagrams. Their PNG and SVG files are rendered exports for the README and manual. |
| `trice_abc_*2.svg` | SVG exports currently embedded in the Reference Manual. Keep them together with their Drawio source and other exports until the diagram workflow is deliberately changed. |
| `trice_logo_*`, `trice_log_icon_*`, and `TriceGirl*` | Logos, icons, and mascot variants at the sizes required by different documentation surfaces. |
| `trice-help-all.txt` | Generated CLI help referenced by the manual. Regenerate it through the repository's documentation refresh workflow; do not edit it by hand. |
| `Screenshot_2026-03-26_*`, `2024-12-05_*`, and timing images such as `MEASURE_executionClocks.PNG` | Dated demonstrations and measurements. They document the stated setup; they are not general current performance guarantees. |
| Transport, terminal, debugger, and board screenshots | Supporting illustrations for the corresponding Reference Manual sections. Some use names from external tools or vendors. |
| `Backup.7z` | Explicitly retained historical backup. It is not an active documentation input and must remain untouched unless a separate task names it. |

Names alone and a missing direct Markdown reference are insufficient evidence
that an asset is redundant. An export can serve PDF rendering, a repository
preview, or a presentation size while its editable source is retained for later
updates. Do not remove assets through an automated unused-file search. When a
family is deliberately changed, record its source, required exports, and active
manual embedding in the same change.
