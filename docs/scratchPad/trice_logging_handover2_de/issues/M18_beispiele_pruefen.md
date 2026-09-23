# M18: Freigegebene Kapitelbeispiele vor der UM-Übernahme prüfen

**Status:** spätere Dokumentvalidierung. **Alt:** L23.

Der alte Kontextentwurf enthält konkrete C-, Shell-, JSON- und Stringisierungsfehler. L23 meinte deren Korrektur, keine zusätzliche Logging-Funktion. Die [Fundstellen und Ersatzhinweise](../Korrekturvorschlaege.md) sind vorbereitet.

## Abnahme und Tests

- [ ] Jeden Block als vorhandene Bedienung, geplante Syntax oder schematisches Beispiel kennzeichnen.
- [ ] Reale C-Beispiele im passenden Minimalrahmen übersetzen; Shell-Argumentübergabe ohne ungewollte Expansion prüfen.
- [ ] JSON-Ausgaben einschließlich Anführungszeichen, Backslashes, Zeilenumbrüchen und numerischen Grenzfällen validieren.
- [ ] Das Beispiel nach `#undef` zeigt den tatsächlichen Token-Text. Ausgabeversprechen werden nicht aus ungeprüften Vorlagen abgeleitet.
- [ ] Vorhandene CLI-Beispiele mit passenden Fixtures prüfen; geplante Optionen nicht allein für einen Dokumenttest implementieren.

Übersetzung und Einbau ins englische UM erfolgen erst auf ausdrücklichen Auftrag. Die Prüfungen müssen keine neue generelle Testinfrastruktur schaffen.

**Quelle:** [ursprünglicher Kontextentwurf](../../TriceUserManual.md#trice-structured-logging).
