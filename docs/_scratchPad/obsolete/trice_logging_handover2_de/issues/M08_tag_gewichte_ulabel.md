# M08: Tag-Gewichte und gewichtete ulabel-Angaben einführen

**Status:** umgesetzt. **Alt:** L05/L06 und Teil L11. **Voraussetzung:** M01.

Jede Tag-Gruppe besitzt jetzt eine Priorität unabhängig von Position und Farbe. Höhere Zahlen bedeuten höhere Wichtigkeit. Es gelten die [Gewichtstabelle](../../TriceTagsAndColor.md#tag-weights) und der Bereich `0..999`.

`-ulabel name[:gewicht]` registriert einen Namen oder ändert das Gewicht einer vorhandenen Gruppe samt Aliasen für diesen Aufruf. Ein bekannter Name ohne Gewicht verändert nichts. Bei mehreren expliziten Zuweisungen gewinnt die letzte. Neue Namen ohne eigenes Gewicht übernehmen das nach allen Optionen wirksame INFO-Gewicht.

Die frühere Doppelpunktliste wird nicht repariert: `-ulabel tagA:tagB` ist ungültig. Alle Registrierungen werden gesammelt, bevor M09 Selektoren auflöst. Insert/Bind müssen den reinen Namen derselben Registrierung für ihre Tag-Regeln erhalten; Gewichte verändern keine Targetdaten.

Die eigenständige, für die spätere Aufnahme vorbereitete UM-Kapitelkopie steht unter [Trice Tags, Color, and Weights](../../TriceTagsAndColor.md). Ihre Überschriften enthalten keine von `mdtoc` erzeugten Nummern oder Anker.

## Abnahme und Tests

- [x] Neue Namen, bekannte Namen, Aliase und wiederholte Angaben erzeugen keine doppelten Gruppen.
- [x] `-ulabel msg:150 -ulabel M:600 -ulabel msg` ergibt 600 für die ganze Message-Gruppe.
- [x] `-ulabel motor` übernimmt INFO; die Position einer INFO-Überschreibung ändert diese Vorgabe nicht.
- [x] Leere Namen/Gewichte, Listen, weitere Doppelpunkte, ungültige Zahlen und Bereichsüberschreitungen scheitern vor Nebenwirkungen.
- [x] Reine Zahlennamen sowie `all`/`off` sind reserviert; bestehende Unicode-Aliase bleiben nutzbar.
- [x] Umordnen der Tabelle oder Wechseln einer Farbe verändert kein Gewicht. Wiederholte Befehlsaufrufe im selben Prozess übernehmen keine alten User-Konfigurationen.

**Quellen:** [Tags und AddUserLabels](../../../internal/emitter/lineTransformerANSI.go), [ArrayFlag](../../../internal/emitter/emitter.go), [flagUserLabel](../../../internal/args/init.go).
