# M14: Ereignisse unabhängig von Farbe und Zusatzspalten zählen

**Status:** umgesetzt und getestet. **Alt:** L16. **Voraussetzung:** M13.

Vor M14 wurden Tag-Zähler bei der Farbverarbeitung verändert; selbst eine erneute Ausgabe der ID-Statistik konnte sie erhöhen. Der ID-Zähler stieg bereits beim Lesen einer bekannten ID, auch wenn die anschließende Dekodierung scheiterte. Die neuen Gesamtzahlen zählen stattdessen erfolgreich dekodierte Anwendungsmeldungen vor der Hostauswahl, einmal je Aufruf.

`-tagStat` zählt nach Tag-Gruppe; `-triceStat` zählt nur ID-basierte Aufrufe; `-stat` zeigt beides. Formatierte `typeX0`-Anwendungsausgabe zählt als Tag-Ereignis ohne ID, ignorierte oder fehlerhafte `typeX0`-Pakete zählen nicht. CHAR/DUMP-Byteblöcke haben keine Ereignisgrenze und werden hier nicht gezählt. Ein zusätzlicher Zähler für angenommene Ereignisse würde Datenmodell und Bericht erweitern; er wurde wegen des begrenzten M14-Umfangs zurückgestellt. Bisherige, durch Darstellung mehrfach erhöhte Tag-Zahlen und zu früh erhöhte ID-Zahlen können deshalb kleiner werden. Farben, Metadaten, sichtbare Zeilen und wiederholte Berichte beeinflussen die Zahlen nicht mehr.

## Abnahme und Tests

- [x] Drei gültige Ereignisse, davon eines angenommen: Gesamtzahl drei. Kein zusätzlicher Auswahlzähler.
- [x] Farbmodus, Metadaten, erneute Statistikdarstellung und Anzahl sichtbarer Zeilen verändern die Ereigniszahlen nicht.
- [x] Zwei Aufrufe in einer Zeile zählen zweimal; ein mehrzeiliger Aufruf einmal. `untagged` wird regulär gezählt.
- [x] Ein Decoderfehler zählt nicht als erfolgreich dekodiertes Anwendungsereignis.
- [x] Aufwand und Änderungen gegenüber bisherigen Zahlen benannt; die Zusatzstatistik ist zurückgestellt.

**Quellen:** [Tag-Zähler](../../../internal/emitter/lineTransformerANSI.go), [ID-Statistik](../../../internal/decoder/decoder.go), [Dekodierung](../../../internal/trexDecoder/trexDecoder.go).
