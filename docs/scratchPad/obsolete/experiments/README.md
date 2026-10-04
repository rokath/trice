# Archived experiments

The six projects under `TriceBind` record completed Bind development and
architecture comparisons. Current behavior is specified in the
[Bind chapter](../../../TriceUserManual.md#trice-bind).

| Project | Retained role |
| --- | --- |
| `10_Minimal_Line_Binding` | Minimal standalone prototype with its own reduced library, Go module and license. |
| `20_Target_Library_Integration` | Historical integration with the then-current target library and simulated sidecars. |
| `30_Preprocessor_Verification` | Historical dispatch and site-descriptor proofs. |
| `40_MVP_Generator` | Historical generator, target execution and decoding demonstration. |
| `50_MVP2_Counter_and_Macro_Definitions` | Historical counter and wrapper alternatives; not the adopted generator strategy. |
| `60_MVP2_Local_Counter_Rebase` | Historical local counter-rebase proof. |

The original prototype descriptions and limitations remain historical records.
Projects 20 and 50 already failed to build against the current library before
archiving; moving them does not restore those old interfaces. Their source,
metadata, expected outputs and license notices are retained.

These archived projects are not inputs to the regular test suite. Run checks of
the current Bind implementation from the repository root:

```sh
./scripts/_500_test_bind.sh
```

Step 500 runs the maintained generator and generated-target integration tests
under `internal/id`. Existing local build outputs moved with these folders may
contain old absolute CMake paths; regular tests do not use or rewrite them.
