# M02: Fehlerhafte IDRange-Angaben vollständig als CLI-Fehler abweisen

**Status:** geplante Korrektur mit vorhandener Teilabsicherung. **Alt:** L27.

`EvaluateIDRangeStrings` überspringt eine Angabe ohne Doppelpunkt. `TestEvaluateIDRangeStringsAdditionalBranches` erwartet das momentan ausdrücklich. Überlappungen, unbekannte Tags und mehrere andere Fehler werden dagegen bereits abgewiesen.

Jede ungültige Regel muss vor Änderungen an Quellen, Wörterbüchern oder Bind-Artefakten einen Fehler mit der betroffenen Angabe auslösen. Vorhandene Prüfungen erweitern; kein neues paralleles Validierungssystem.

## Abnahme und Tests

- [ ] Die bisherige Erfolgserwartung für `missing-separator` wird durch eine Fehlererwartung ersetzt.
- [ ] Fehlende/leere Teile, ungültige Zahlen, umgekehrte Grenzen und Werte außerhalb des zulässigen ID-Raums scheitern.
- [ ] Tests für Überlappungen mit allgemeinem Bereich und anderen Tags sowie doppelte Aliasgruppen bleiben erhalten.
- [ ] Ein gemeinsamer Endpunkt zählt als Überlappung; direkt benachbarte getrennte Bereiche und ein einzelner erlaubter ID-Wert funktionieren.
- [ ] Ein gültiger Eintrag verdeckt keinen ungültigen; Insert und Bind brechen ohne Dateiveränderungen ab.

**Quellen:** [Validierung](../../../internal/id/switchIDs.go), [vorhandene Tests](../../../internal/id/coverage_additional_test.go).
