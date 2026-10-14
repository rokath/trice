# M09: Alle Selektoren nach der Tag-Registrierung validieren

**Status:** umgesetzt. **Alt:** L07 und Teil L05/L11. **Voraussetzung:** M08.

Pick-/Ban-Namen werden zunächst unverändert gesammelt. Nach allen `-ulabel`-Registrierungen werden bekannte Gruppen samt Aliasen aufgelöst und unbekannte oder leere Namen abgewiesen. Erst danach werden Ein- und Ausgaben geöffnet.

`-logLevel` akzeptiert `all`, `off`, einen bekannten Tag/Alias oder ein zulässiges numerisches Gewicht. Tag und Alias ergeben dieselbe inklusive Gewichtsschwelle. Unbekannter Meldungstext ist dagegen keine fehlerhafte CLI: Dafür dient später M11.

`all` und `off` sind auch für Pick/Ban reservierte Mengenselektoren: Pick-all beziehungsweise Ban-off lassen alles zu, Pick-off beziehungsweise Ban-all nichts. Die gemeinsame ereignisweite Verknüpfung von Tag-Auswahl, Gewicht und Zusatzspalten bleibt M13.

## Abnahme und Tests

- [x] `-pick motor -ulabel motor` und umgekehrte Reihenfolge wählen dieselbe Gruppe; ebenso Ban.
- [x] Unbekannte Namen, leere Listenteile, doppelte Trennzeichen und ungültige Levelwerte scheitern mit verständlicher Angabe des Fehlers.
- [x] Ein bekannter Tag ohne auftretende Meldungen bleibt ein gültiger Selektor.
- [x] Tags und ihre Aliase ergeben dieselbe Schwelle; numerische Grenzwerte werden geprüft.
- [x] Die Validierung gilt für `trice log` und `tlog` und findet vor Kanalöffnung statt; M03 bleibt wirksam.

Die Benutzerbeschreibung steht in [Trice Tags, Color, and Weights](../../TriceTagsAndColor.md#selecting-tags-and-priority).

**Quellen:** [channelArrayFlag.Set](../../../internal/emitter/emitter.go), [runLog](../../../internal/args/handler.go).
