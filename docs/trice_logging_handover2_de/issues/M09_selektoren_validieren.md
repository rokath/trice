# M09: Alle Selektoren nach der Tag-Registrierung validieren

**Status:** geplante CLI-Korrektur und Erweiterung. **Alt:** L07 und Teil L05/L11. **Voraussetzung:** M08.

Unbekannte Pick-/Ban-Namen werden derzeit ohne Fehler übergangen; User-Tags werden zu spät registriert. Gewünschter Ablauf: Optionen sammeln, Register vervollständigen, dann Selektoren und Level auflösen, erst danach Ein-/Ausgaben öffnen.

`-logLevel` akzeptiert `all`, `off`, einen bekannten Tag/Alias oder ein zulässiges numerisches Gewicht. Unbekannter Meldungstext ist dagegen keine fehlerhafte CLI: Dafür dient später M11.

## Abnahme und Tests

- [ ] `-pick motor -ulabel motor` und umgekehrte Reihenfolge wählen dieselbe Gruppe; ebenso Ban.
- [ ] Unbekannte Namen, leere Listenteile, doppelte Trennzeichen und ungültige Levelwerte scheitern mit verständlicher Angabe des Fehlers.
- [ ] Ein bekannter Tag ohne auftretende Meldungen bleibt ein gültiger Selektor.
- [ ] Tags und ihre Aliase ergeben dieselbe Schwelle; numerische Grenzwerte werden geprüft.
- [ ] Die Validierung gilt für `trice log` und `tlog` und findet vor Kanalöffnung statt; M03 bleibt wirksam.

**Quellen:** [channelArrayFlag.Set](../../../internal/emitter/emitter.go), [runLog](../../../internal/args/handler.go).
