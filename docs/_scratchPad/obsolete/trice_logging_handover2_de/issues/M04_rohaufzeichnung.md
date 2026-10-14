# M04: Ungefilterte binäre Aufzeichnung und Wiedergabe absichern

**Status:** umgesetzt. **Alt:** L15.

Der BinaryLogger umschließt den Empfänger bereits vor der Übersetzung. Auch bei restriktiver Anzeige müssen dieselben empfangenen Bytes aufgezeichnet werden. Das schützt die spätere Auswahl beim Replay.

## Abnahme und Tests

- [x] Derselbe Eingang ergibt bei `-pick`, `-ban`, `-logLevel all` und `-logLevel off` byteidentische Rohdateien.
- [x] Replay mit neuer Auswahl zeigt ein bei der Aufzeichnung ausgeblendetes Ereignis wieder.
- [x] Unvollständig empfangene Daten bleiben in der Rohdatei erhalten; Schreibfehler werden zurückgegeben.
- [x] Die Filtervarianten verwenden getrennte Dateien; Append-Verhalten wird eigens geprüft. Replay zeichnet nicht in seine Eingabedatei auf.

Die Zusage betrifft tatsächlich empfangene Bytes, nicht schon auf dem Target verlorene Daten. Hostzeit beim Replay ist Wiedergabezeit.

**Umsetzung:** [BinaryLogger und Fehlerweitergabe](../../../internal/receiver/receiver.go), [Rohdaten-, Replay- und Filtertests](../../../internal/args/handler_additional_test.go), [Teil-, Fehler- und Append-Tests](../../../internal/receiver/receiver_test.go).
