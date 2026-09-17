# M14: Ereignisse unabhängig von Farbe und Zusatzspalten zählen

**Status:** geplanter begrenzter Ausbau nach Aufwandsschätzung. **Alt:** L16. **Voraussetzung:** M13.

Die Tag-Zähler werden derzeit bei der Farbverarbeitung verändert. Damit können Darstellungsfragmente und Formatierungswege die Bedeutung beeinflussen.

**Vorschlag:** Die bestehenden ID-/Tag-Statistiken zählen erfolgreich dekodierte Anwendungsmeldungen vor der Auswahl, einmal je Aufruf. Diagnosen sind separat. Ein zusätzlicher Zähler für angenommene Ereignisse ist nur bei kleinem Zusatzaufwand vorgesehen; seine Bezeichnung muss ihn vom Gesamtaufkommen unterscheiden.

## Abnahme und Tests

- [ ] Drei gültige Ereignisse, davon eines angenommen: Gesamtzahl drei, gegebenenfalls Auswahlzahl eins.
- [ ] Farbmodus, Metadaten, erneute Statistikdarstellung und Anzahl sichtbarer Zeilen verändern die Ereigniszahlen nicht.
- [ ] Zwei Aufrufe in einer Zeile zählen zweimal; ein mehrzeiliger Aufruf einmal. `untagged` wird regulär gezählt.
- [ ] Ein Decoderfehler zählt nicht als erfolgreich dekodiertes Anwendungsereignis.
- [ ] Aufwand und Änderungen gegenüber bisherigen Zahlen vor Umsetzung benennen; bei größerem Umbau Zusatzstatistik zurückstellen.

**Quellen:** [Tag-Zähler](../../../internal/emitter/lineTransformerANSI.go), [ID-Statistik](../../../internal/decoder/decoder.go).
