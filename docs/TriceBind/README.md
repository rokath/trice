# Trice Bind

Dieser Ordner enthält die Spezifikationen, Implementierungsaufträge,
Testdokumente und Anwenderdokumentation zu `trice bind`. Die Nummern bilden die
Entwicklungsreihenfolge ab; freie Nummernbereiche lassen Platz für spätere
Ergänzungen.

## Dokumente

| Nr. | Dokument | Zweck |
| ---: | --- | --- |
| 10 | [MVP-Spezifikation](./Trice_bind_10_MVP_Spezifikation.md) | Verbindliche technische Grundlage des MVP |
| 20 | [MVP-Implementierungs-Prompt](./Trice_bind_20_MVP_Implementation_Prompt.md) | Auftrag zur Implementierung des MVP |
| 30 | [MVP-Testspezifikation](./Trice_bind_30_MVP_Test_Spezifikation.md) | Anforderungen an Testaufbau und Regression |
| 40 | [MVP-Test-Prompt](./Trice_bind_40_MVP_Test_Prompt.md) | Auftrag zur Umsetzung des Testsystems |
| 50 | [MVP-Testbericht](./Trice_bind_50_MVP_Test_Report.md) | Bericht zur ausgeführten Testimplementierung |
| 60 | [MVP2-Implementierungsstrategien](./Trice_bind_60_MVP2_Implementation_Strategies.md) | Technische Entscheidungsgrundlage für die MVP2-Präprozessormechanik |
| 70 | [MVP2-Implementierungs-Prompt](./Trice_bind_70_MVP2_Implementation_Prompt.md) | Auftrag für den lokalen `__COUNTER__`-Rebase |
| 90 | [MVP User Manual](./Trice_bind_90_MVP_User_Manual.md) | Anwendersicht und technische Hintergründe |

## Experimente

Die ausführbaren Referenz- und Entwicklungs-PoCs liegen zentral unter
[`experiments/TriceBind`](../../experiments/TriceBind). Ihre Nummerierung folgt
der tatsächlichen Entstehungs- und Evolutionsreihenfolge.

| Nr. | Experiment | Nachweis |
| ---: | --- | --- |
| 10 | [Minimal Line Binding](../../experiments/TriceBind/10_Minimal_Line_Binding/README.md) | Minimale Bindung einer bekannten ID über Sourcezeile und Sidecar |
| 20 | [Target Library Integration](../../experiments/TriceBind/20_Target_Library_Integration/README.md) | Kompilierung ID-freier Aufrufe gegen die echte Target-Library |
| 30 | [Preprocessor Verification](../../experiments/TriceBind/30_Preprocessor_Verification/README.md) | Lokaler Dispatch und einzeiliger Site-Deskriptor |
| 40 | [MVP Generator](../../experiments/TriceBind/40_MVP_Generator/README.md) | End-to-End-Nachweis mit dem echten `trice bind`-Generator |
| 50 | [MVP2 Counter and Macro Definitions](../../experiments/TriceBind/50_MVP2_Counter_and_Macro_Definitions/README.md) | Mehrere Trice-Stellen pro Zeile und Trices in Makrodefinitionen |
| 60 | [MVP2 Local Counter Rebase](../../experiments/TriceBind/60_MVP2_Local_Counter_Rebase/README.md) | Lokaler `__COUNTER__`-Rebase mit Compile-Time-Prüfungen |
