# M16: Bei Verbose nur Anzahl und ein Beispiel unpassender Wörterbuch-IDs melden

**Status:** abgestimmte Diagnoseergänzung. **Alt:** Teil L03. **Voraussetzung:** M15.

Alte Zuordnungen bleiben zur Kompatibilität in `til.json` und können der aktuellen Policy widersprechen. Diese Einträge werden nur bei `-v` gemeldet: eine Zusammenfassung pro Lauf, die Anzahl betroffener IDs und genau ein Beispiel. Vorschlag für reproduzierbare Ausgabe: Beispiel mit der kleinsten betroffenen ID.

Die Prüfung verwendet aktuelle Tag-Regeln und allgemeine Grenzen. Sie verändert keine Wörterbucheinträge und behauptet nicht, die historischen IDs seien noch aktiv. Vorhandene andere Verbose-Diagnosen werden durch diese Aufgabe nicht entfernt.

## Abnahme und Tests

- [ ] Ohne `-v` keine zusätzliche Policy-Warnung; bei null Abweichungen auch mit `-v` keine Warnung.
- [ ] Ein und sehr viele betroffene Einträge ergeben jeweils eine Zusammenfassung mit exakter Anzahl und einem Beispiel einschließlich erwartetem Bereich.
- [ ] Iterationsreihenfolge ändert das Beispiel nicht; jede ID wird nur einmal gezählt.
- [ ] Tag-spezifischer beziehungsweise allgemeiner Bereich und Grenzen werden korrekt angewendet.
- [ ] Die nach M15 historisch erhaltene `250` wird mitgezählt; die neue passende aktive ID nicht. Die Warnung allein ändert den Erfolgstatus nicht.

**Quellen:** [Wörterbuch und Policy](../../../internal/id/switchIDs.go), [ID-Datentypen](../../../internal/id/id.go).
