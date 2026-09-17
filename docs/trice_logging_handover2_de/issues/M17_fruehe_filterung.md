# M17: Frühe Hostfilterung messen und nur bei Nutzen umsetzen

**Status:** optionale Optimierung nach Semantikkorrekturen. **Alt:** L17. **Voraussetzung:** M10–M14.

Der Decoder liefert heute bereits formatierten Text, bevor Pick/Ban entscheidet. Wörterbuchgebundene Auswahl könnte unnötige Formatierung vermeiden. Wie viel das spart, ist nicht gemessen.

## Abnahme und Tests

- [ ] Repräsentatives Replay bei 0, 50, 90 und 100 Prozent verworfenen Ereignissen messen: CPU, Allokationen und Durchsatz.
- [ ] Angenommene Ausgabe, Metadaten, Teilzeilen, Zeitdifferenzen, Visualisierung und Statistik bleiben semantisch gleich.
- [ ] Framing, Längen-/Integritätsprüfung, Cycle-/Zeitstempelzustand und Rohaufzeichnung laufen auch für verworfene Ereignisse weiter.
- [ ] Wörterbuch-/Labeländerungen machen zwischengespeicherte Entscheidungen ungültig; dynamische Strings und nicht ID-basierte Codierungen haben einen passenden eigenen Pfad.
- [ ] Ohne ausreichenden Nutzen bleibt es bei Messergebnis und Entscheidung; keine große Umstrukturierung allein für vermutete Einsparungen.

Die Aufgabe verändert weder Targetprotokoll noch Target-Level-Steuerung.

**Quellen:** [dec.Read vor Filterung](../../../internal/translator/translator.go), [Decoder-Schnittstellen](../../../internal/decoder/decoder.go).
