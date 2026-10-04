# M11: Unbekannte und ungetaggte Meldungen der Gruppe untagged zuordnen

**Status:** umgesetzt. **Voraussetzung:** M08–M10.

`untagged` ist eine reservierte eingebaute Gruppe mit eigenem Gewicht 500 und neutraler Darstellung. Eine INFO-Überschreibung ändert dieses eigenständige Gewicht nicht. Pick, Ban, Level und Statistik behandeln die Gruppe wie andere Anwendungstags.

Einmal pro Anwendungsmeldung ordnet der Host fehlende oder unbekannte Tags dieser Gruppe zu. Für den Textpfad stellt er intern `untagged:` voran: `mgs:blah` wird zu `untagged:mgs:blah`. Der Originaltext bleibt erhalten. Ein ausdrückliches `untagged:` wird nicht nochmals ergänzt; bekannte Präfixe ebenfalls nicht. Bei ID-basierten Meldungen stammt die Klassifikation aus dem gespeicherten Formatstring. Der Decoder reicht diesen Kandidaten zusammen mit der Ereignisgrenze an den Übersetzer weiter.

## Abnahme und Tests

- [x] Fehlender Tag, Tippfehler, leerer Präfix und ein normaler Text mit Doppelpunkt erhalten die erwartete Gruppe.
- [x] Bei `default`/`none` bleibt `mgs:blah` sichtbar; bei `off` auch das synthetische äußere Präfix.
- [x] `-pick untagged`, `-ban untagged` und `-ulabel untagged:150` wirken wie für eingebaute Gruppen; kein zweiter gleichnamiger Eintrag entsteht.
- [x] Klassifikation basiert bei ID-Meldungen auf der Wörterbuchvorlage, nicht auf einem zufällig wie `err:...` aussehenden Laufzeitwert.
- [x] Zeitstempel, Präfix/Suffix und Diagnosen werden nicht als eigene ungetaggte Ereignisse behandelt.
- [x] Quelltext, gespeicherter Formatstring, IDs und Rohbytes bleiben unverändert. Byteorientierte CHAR-/DUMP-Ausgabe bekommt keine Ereigniszuordnung aus willkürlichen Decoder-Teilblöcken.

Eine Änderung der IDRange-Zuordnung für ursprünglich ungetaggte Quellen ist hier nicht enthalten.

Die englische Benutzerdokumentation steht in [Trice Tags, Color, and Weights](../../TriceTagsAndColor.md#untagged-application-events).

**Quellen:** [Präfixbehandlung](../../../internal/emitter/lineTransformerANSI.go), [Ereignis-/Metadatengrenze](../../../internal/translator/translator.go).
