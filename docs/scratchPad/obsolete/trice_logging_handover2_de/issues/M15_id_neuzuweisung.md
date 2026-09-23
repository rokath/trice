# M15: Aktive Logstellen beim nächsten Lauf an die ID-Policy anpassen

**Status:** umgesetzt und getestet. **Alt:** L04 und Teil L03. **Voraussetzung:** M01/M02.

Der vorhandene Vergabepfad kann eine passende historische ID auch außerhalb des neuen Bereichs wiederverwenden. Künftig prüft jeder Wiederverwendungspfad die aktuelle Policy.

Beispiel: Eine bearbeitete Fehler-Logstelle hat ID `250`; ihr neuer Bereich ist `10..99`. Der nächste normale Insert-/Bind-Lauf weist ihr eine passende ID zu. Die Zuordnung von `250` bleibt unverändert in `til.json`. Ein eigenes Migrationskommando, eine zusätzliche Warnpflicht oder eine automatische Wörterbuchbereinigung sind nicht vorgesehen.

## Abnahme und Tests

- [x] Insert und Bind korrigieren aktive Verletzungen tagbezogener und allgemeiner Bereiche; passende aktive IDs bleiben stabil.
- [x] Aus Quellen, Ortsdaten oder Wörterbuch wiedergefundene regelwidrige IDs werden nicht erneut zugewiesen.
- [x] Der nächste unveränderte Lauf behält die korrigierten IDs. Historische Zuordnungen dekodieren alte Aufzeichnungen weiter.
- [x] Nur der bearbeitete Quellumfang wird geändert. Quellen/Bind-Artefakte, Wörterbuch und Ortsdaten passen zusammen; historische Einträge sind kein Beweis für aktive Nutzung.
- [x] Bei erschöpftem Bereich oder Schreibfehler: klarer Fehler, keine historische ID umdeuten, keinen erfolgreichen inkonsistenten Lauf melden.
- [x] Geteilte Wörterbücher und konkurrierende Läufe werden mit den vorhandenen Sicherungen berücksichtigt; Bind verändert keine Anwenderquellen.

Die spätere knappe Wörterbuchwarnung steht separat in M16. Nach einer Neuzuweisung ist ein Neubau der betroffenen Firmware nötig.

**Quellen:** [Wiederverwendung](../../../internal/id/insertIDs.go), [Bereichsverwaltung](../../../internal/id/switchIDs.go).
