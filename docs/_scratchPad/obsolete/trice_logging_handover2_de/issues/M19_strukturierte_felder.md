# M19: Eine nützliche strukturierte Bedienung auswählen und spezifizieren

**Status:** archivierter früherer Auftrag, durch [aktuellen Entwurf](../../../Strukturiertes_Logging_DE.md) und [Implementierungsplan](../../../Implementierungsplan.md) abgelöst. **Alt:** L26. Die nachfolgenden Variantenvergleiche und `ctx`-Vorschläge sind historisch; Variante A ist gewählt und CE verwendet das gemeinsame flache Feldschema. Noch keine Implementierungsfreigabe.

Zielbeispiel: `temperature_c` über mindestens zwei Meldungstypen auswerten, obwohl sich deren lesbare Texte ändern. Der [Kapitelentwurf](../chapters/Strukturiertes_Logging_DE.md) vergleicht benannte `strice`-Platzhalter mit einer separaten Feldzuordnung für vorhandene Aufrufe.

## Entscheidung und Abnahmekriterien

- [ ] Für beide Varianten Aufruf, Typzuordnung, Wörterbuchinformation und erwartete Ausgabe gegenüberstellen; danach eine Variante begründet auswählen.
- [ ] Werte werden einmal angegeben und bleiben typisiert erhalten. Einheit, Bitbreite, Argumentposition und Ereignisgrenze sind eindeutig.
- [ ] Geprüfte Feldnamen von neu vorgeschlagenen Namen unterscheiden; Tippfehler, Dopplungen, Typkonflikte und Wörterbuch-/Firmware-Verwechslungen melden.
- [ ] Freien Meldungstext erlauben; JSON-Escaping, große Ganzzahlen und nicht endliche Werte für einen gewählten Export festlegen.
- [ ] Mehrere Felder pro ID und mehrere Quellen mit eigenen Wörterbüchern unterstützen; kein Pflicht-Target-Key-ID-System.
- [ ] Kombination mit `ctx` aus M20 ohne Namenskollision oder Verlust ursprünglicher Felder zeigen.
- [ ] Bei Wrappern tatsächliche Aufrufposition, Code-/Laufzeitaufwand und Argumentauswertung bei abgeschaltetem Logging prüfen.

Ein nachvollziehbares Datenbeispiel genügt für die erste Entscheidung. Falls dafür ein Prototyp erforderlich wird, seinen Umfang gesondert festlegen. Ein kompletter Parserumbau, ein Serverlogger-Ersatz oder eine Analyseplattform sind nicht beauftragt.

**Quellen:** [typisierte Decoderwerte](../../../internal/decoder/decoder.go), [Wörterbuchformat](../../../internal/id/id.go).
