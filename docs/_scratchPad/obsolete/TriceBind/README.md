# Trice Bind

This folder retains the development history of `trice bind`: earlier design
specifications, implementation prompts, test plans, a report and a German
development-stage manual. The numbers identify their historical sequence.

For current usage and limitations, read the [Trice Bind chapter](../../../TriceUserManual.md#trice-bind)
and [Bind Limits](../../../TriceUserManual.md#bind-limits) in the main manual.
[Context Enrichment](../../../TriceUserManual.md#trice-context-enrichment) has its own
restrictions; ordinary Bind wrapper support does not imply CE wrapper support.

These records are not a second current product specification. Requirements and
start commands inside old prompts belong to their original task and do not
authorize new work. “MVP” denotes the initial file-and-line-only stage; “MVP2”
denotes the later local counter-rebase stage. Old restrictions, CLI names and
test results must be read in that context. For example, `-bindDir` and
`build/triceIDs` in the first test report are superseded by `-genDir` and
`./generated`; ordinary wrappers and multiple calls per line are no longer
generally excluded. Current behavior is documented in the main manual.

## Retained development records

| Stage | Document | Historical purpose |
| ---: | --- | --- |
| 10 | [Initial generator specification](./Trice_bind_10_MVP_Spezifikation.md) | Original line-binding requirements and deferred alternatives. |
| 20 | [Initial implementation prompt](./Trice_bind_20_MVP_Implementation_Prompt.md) | Original generator implementation task. |
| 30 | [Initial test specification](./Trice_bind_30_MVP_Test_Spezifikation.md) | Test states, shared workers, restoration and acceptance design. |
| 40 | [Test implementation prompt](./Trice_bind_40_MVP_Test_Prompt.md) | Original test-system task. |
| 50 | [Implementation test report](./Trice_bind_50_MVP_Test_Report.md) | Results and limitations observed at that development stage. |
| 60 | [Counter-binding strategy comparison](./Trice_bind_60_MVP2_Implementation_Strategies.md) | Local rebase versus compiler preprocessing and generated compiler input. |
| 70 | [Local rebase implementation prompt](./Trice_bind_70_MVP2_Implementation_Prompt.md) | Original task for the selected local `__COUNTER__` strategy. |
| 90 | [German development-stage manual](./Trice_bind_90_MVP_User_Manual.md) | Earlier user explanation and architecture rationale, retained for comparison. |

## Reproducible experiments

The executable proofs are retained under
[`experiments/TriceBind`](../../../../experiments/TriceBind). Their historical names
identify the tested design; they are not a current support matrix. The Bind
test step uses the preprocessor verification, generator and local rebase
experiments. The CE wrapper/rebase proof and its limits are documented
separately in the [CE appendix](../../../TriceUserManual.md#extended-poc-for-wrapper-macros-and-counter-rebasing).

| Stage | Experiment | Proof |
| ---: | --- | --- |
| 10 | [Minimal Line Binding](../../../../experiments/TriceBind/10_Minimal_Line_Binding/README.md) | Bind a known ID by source line and sidecar. |
| 20 | [Target Library Integration](../../../../experiments/TriceBind/20_Target_Library_Integration/README.md) | Compile ID-free calls with the real target library. |
| 30 | [Preprocessor Verification](../../../../experiments/TriceBind/30_Preprocessor_Verification/README.md) | Local dispatch and single-line site descriptors. |
| 40 | [Generator Integration](../../../../experiments/TriceBind/40_MVP_Generator/README.md) | End-to-end proof using the actual generator. |
| 50 | [Counter and Macro Definitions](../../../../experiments/TriceBind/50_MVP2_Counter_and_Macro_Definitions/README.md) | Multiple sites per line and calls in macro definitions. |
| 60 | [Local Counter Rebase](../../../../experiments/TriceBind/60_MVP2_Local_Counter_Rebase/README.md) | Local `__COUNTER__` rebasing with compile-time checks. |
