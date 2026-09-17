# M11: Unbekannte und ungetaggte Meldungen der Gruppe untagged zuordnen

**Status:** geplante Erweiterung aus der Abstimmung. **Voraussetzung:** M08–M10.

`untagged` wird eine reservierte eingebaute Gruppe mit eigenem Gewicht und eigener Farbe. Vorgabe im Entwurf: 500 als INFO-Anfangswert und neutrale Darstellung. Eine spätere INFO-Überschreibung ändert dieses eigenständige Gewicht nicht. Pick, Ban, Level und Statistik behandeln die Gruppe wie andere Anwendungstags.

Einmal pro Anwendungsmeldung ordnet der Host fehlende oder unbekannte Tags dieser Gruppe zu. Für den Textpfad darf er intern `untagged:` voranstellen: `mgs:blah` wird zu `untagged:mgs:blah`. Der Originaltext bleibt erhalten. Ein ausdrückliches `untagged:` wird nicht nochmals ergänzt; bekannte Präfixe ebenfalls nicht.

## Abnahme und Tests

- [ ] Fehlender Tag, Tippfehler, leerer Präfix und ein normaler Text mit Doppelpunkt erhalten die erwartete Gruppe.
- [ ] Bei `default`/`none` bleibt `mgs:blah` sichtbar; bei `off` auch das synthetische äußere Präfix.
- [ ] `-pick untagged`, `-ban untagged` und `-ulabel untagged:150` wirken wie für eingebaute Gruppen; kein zweiter gleichnamiger Eintrag entsteht.
- [ ] Klassifikation basiert bei ID-Meldungen auf der Wörterbuchvorlage, nicht auf einem zufällig wie `err:...` aussehenden Laufzeitwert.
- [ ] Zeitstempel, Präfix/Suffix und Diagnosen werden nicht als eigene ungetaggte Ereignisse behandelt.
- [ ] Quelltext, gespeicherter Formatstring, IDs und Rohbytes bleiben unverändert. Byteorientierte CHAR-/DUMP-Ausgabe bekommt keine Ereigniszuordnung aus willkürlichen Decoder-Teilblöcken.

Eine Änderung der IDRange-Zuordnung für ursprünglich ungetaggte Quellen ist hier nicht enthalten.

**Quellen:** [heutige Präfixbehandlung](../../../internal/emitter/lineTransformerANSI.go), [Ereignis-/Metadatengrenze](../../../internal/translator/translator.go).
