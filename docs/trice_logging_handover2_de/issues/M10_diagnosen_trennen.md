# M10: Decoder- und Werkzeugfehler von Anwendungsfiltern trennen

**Status:** zu reproduzierendes Risiko und geplante Absicherung. **Alt:** L14.

Der Übersetzer erhält im selben Textpfad eine Trice-Meldung oder einen Fehler und wendet darauf Pick/Ban an. Konkreter Prüffall: Eine Aufnahme enthält eine im Wörterbuch fehlende ID; `-pick err` darf die Diagnose dazu nicht ausblenden, nur weil ihr Text kein `err:`-Anwendungsereignis ist.

Diagnosen werden vor der Anwendungsauswahl erkennbar getrennt. Sie bleiben bei `-pick`, `-ban` und `-logLevel off` beobachtbar und werden nicht dem späteren `untagged` zugeordnet. Eine allgemeine Behauptung, aktuell verschwinde jede Diagnose, ist damit nicht verbunden.

## Abnahme und Tests

- [ ] Unbekannte ID, beschädigter/abgeschnittener Frame und unterstützte Cycle-/Integritätsfehler bleiben unter restriktiven Filtern sichtbar.
- [ ] Fehler liefern keinen erfundenen normalen Datensatz und behalten gegebenenfalls ihren Fehlerstatus.
- [ ] Künftige maschinenlesbare Ausgabe enthält keine hineingemischten Banner oder freien Warntexte.
- [ ] Ziel der Diagnoseausgabe und Fehlerbehandlung bleiben dokumentiert; die Rohaufzeichnung bleibt bytegleich.

**Quelle:** [decodeAndComposeLoop](../../../internal/translator/translator.go).
