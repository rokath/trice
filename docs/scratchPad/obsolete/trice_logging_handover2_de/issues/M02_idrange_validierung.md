# M02: Fehlerhafte IDRange-Angaben vollständig als CLI-Fehler abweisen

**Status:** umgesetzt. **Alt:** L27.

Zuvor übersprang `EvaluateIDRangeStrings` eine Angabe ohne Doppelpunkt; ein Test erwartete dieses Verhalten ausdrücklich. Die Validierung weist nun auch diese und die übrigen fehlerhaften Formen ab, ohne zuvor geprüfte Regeln teilweise zu übernehmen.

Jede ungültige Regel muss vor Änderungen an Quellen, Wörterbüchern oder Bind-Artefakten einen Fehler mit der betroffenen Angabe auslösen. Vorhandene Prüfungen erweitern; kein neues paralleles Validierungssystem.

## Abnahme und Tests

- [x] Die bisherige Erfolgserwartung für `missing-separator` wurde durch eine Fehlererwartung ersetzt.
- [x] Fehlende/leere Teile, ungültige Zahlen, umgekehrte Grenzen und Werte außerhalb von `1..16383` scheitern.
- [x] Tests für Überlappungen mit allgemeinem Bereich und anderen Tags sowie doppelte Aliasgruppen bleiben erhalten.
- [x] Ein gemeinsamer Endpunkt zählt als Überlappung; direkt benachbarte getrennte Bereiche und ein einzelner erlaubter ID-Wert funktionieren.
- [x] Die Regeln werden atomar geprüft: Ein gültiger Eintrag verdeckt keinen ungültigen; Insert und Bind brechen ohne Dateiveränderungen ab.

**Umsetzung:** [Validierung](../../../internal/id/switchIDs.go), [Parser- und Grenztests](../../../internal/id/coverage_additional_test.go), [Insert-/Bind-Dateischutz](../../../internal/id/id_whitebox_test.go).
