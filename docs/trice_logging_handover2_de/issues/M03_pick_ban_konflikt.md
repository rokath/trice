# M03: Gleichzeitiges pick und ban schon beim Start ablehnen

**Status:** umgesetzt. **Alt:** L08.

Die gegenseitige Sperre liegt derzeit im Filterpfad einer eingelesenen Meldung. Ein widersprüchlicher Aufruf soll vor dem Öffnen eines Ports oder einer Datei scheitern, auch wenn keine Meldung eintrifft.

## Abnahme und Tests

- [x] `-pick err -ban dbg` endet mit Fehlerstatus und nennt beide unvereinbaren Optionen.
- [x] Umgekehrte Reihenfolge, leere Eingabe und unbekannte Selektoren umgehen die Sperre nicht.
- [x] Ein Start-Stub bestätigt, dass bei diesem Fehler kein Eingabekanal geöffnet wird.
- [x] Pick allein und Ban allein bleiben zulässig; ebenso die jeweilige Kombination mit `-logLevel`.

**Umsetzung:** [frühe CLI-Prüfung und Startgrenze](../../../internal/args/handler.go), [Start- und Optionsvarianten](../../../internal/args/handler_additional_test.go). Die defensive Prüfung im [Filterpfad](../../../internal/emitter/emitter.go) bleibt bestehen.
