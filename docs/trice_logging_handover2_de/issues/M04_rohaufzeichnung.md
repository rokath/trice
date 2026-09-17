# M04: Ungefilterte binäre Aufzeichnung und Wiedergabe absichern

**Status:** Regressionstests für vorhandenes Verhalten. **Alt:** L15.

Der BinaryLogger umschließt den Empfänger bereits vor der Übersetzung. Auch bei restriktiver Anzeige müssen dieselben empfangenen Bytes aufgezeichnet werden. Das schützt die spätere Auswahl beim Replay.

## Abnahme und Tests

- [ ] Derselbe Eingang ergibt bei `-pick`, `-ban`, `-logLevel all` und `-logLevel off` byteidentische Rohdateien.
- [ ] Replay mit passendem Wörterbuch zeigt zuvor ausgeblendete Ereignisse wieder.
- [ ] Beschädigte beziehungsweise unvollständige empfangene Daten bleiben in der Rohdatei erhalten; Schreibfehler werden gemeldet.
- [ ] Tests verwenden frische getrennte Dateien; vorhandenes Append-Verhalten wird eigens geprüft. Replay liest niemals aus seinem eigenen Aufzeichnungsziel.

Die Zusage betrifft tatsächlich empfangene Bytes, nicht schon auf dem Target verlorene Daten. Hostzeit beim Replay ist Wiedergabezeit.

**Quelle:** [logLoop und BinaryLogger-Einbindung](../../../internal/args/handler.go).
