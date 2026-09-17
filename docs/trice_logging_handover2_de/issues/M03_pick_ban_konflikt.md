# M03: Gleichzeitiges pick und ban schon beim Start ablehnen

**Status:** geplante Fehlerkorrektur. **Alt:** L08.

Die gegenseitige Sperre liegt derzeit im Filterpfad einer eingelesenen Meldung. Ein widersprüchlicher Aufruf soll vor dem Öffnen eines Ports oder einer Datei scheitern, auch wenn keine Meldung eintrifft.

## Abnahme und Tests

- [ ] `-pick err -ban dbg` endet mit Fehlerstatus und nennt beide unvereinbaren Optionen.
- [ ] Umgekehrte Reihenfolge, leere Eingabe und malformed Selektoren umgehen die Sperre nicht.
- [ ] Ein Eingabe-/Ausgabe-Stub bestätigt, dass bei diesem Fehler kein Kanal geöffnet wird.
- [ ] Pick allein und Ban allein bleiben zulässig; ebenso die jeweils geplante Kombination mit `-logLevel`.

**Quellen:** [Filterpfad](../../../internal/emitter/emitter.go), [Startablauf](../../../internal/args/handler.go).
