# M08: Tag-Gewichte und gewichtete ulabel-Angaben einführen

**Status:** geplante Erweiterung. **Alt:** L05/L06 und Teil L11. **Voraussetzung:** M01.

Jede Tag-Gruppe erhält eine Priorität unabhängig von Position und Farbe. Höhere Zahlen bedeuten höhere Wichtigkeit. Die [Gewichtstabelle](../chapters/Log_Auswahl_DE.md#la-gewichte) und `0..999` sind Vorschläge zur Abnahme.

`-ulabel name[:gewicht]` registriert einen Namen oder ändert das Gewicht einer vorhandenen Gruppe samt Aliasen für diesen Aufruf. Ein bekannter Name ohne Gewicht verändert nichts. Bei mehreren expliziten Zuweisungen gewinnt die letzte. Neue Namen ohne eigenes Gewicht übernehmen im Entwurf das nach allen Optionen wirksame INFO-Gewicht.

Die frühere Doppelpunktliste wird nicht repariert: `-ulabel tagA:tagB` ist ungültig. Alle Registrierungen werden gesammelt, bevor M09 Selektoren auflöst. Insert/Bind müssen den reinen Namen derselben Registrierung für ihre Tag-Regeln erhalten; Gewichte verändern keine Targetdaten.

## Abnahme und Tests

- [ ] Neue Namen, bekannte Namen, Aliase und wiederholte Angaben erzeugen keine doppelten Gruppen.
- [ ] `-ulabel msg:150 -ulabel M:600 -ulabel msg` ergibt 600 für die ganze Message-Gruppe.
- [ ] `-ulabel motor` übernimmt INFO; die Position einer INFO-Überschreibung ändert diese Vorgabe nicht.
- [ ] Leere Namen/Gewichte, Listen, weitere Doppelpunkte, ungültige Zahlen und Bereichsüberschreitungen scheitern vor Nebenwirkungen.
- [ ] Reine Zahlennamen sowie `all`/`off` werden nach dem Entwurf reserviert; bestehende Unicode-Aliase bleiben nutzbar.
- [ ] Umordnen der Tabelle oder Wechseln einer Farbe verändert kein Gewicht. Wiederholte Befehlsaufrufe im selben Prozess übernehmen keine alten User-Konfigurationen.

**Quellen:** [Tags und AddUserLabels](../../../internal/emitter/lineTransformerANSI.go), [ArrayFlag](../../../internal/emitter/emitter.go), [flagUserLabel](../../../internal/args/init.go).
