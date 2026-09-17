# M07: Vorhandene Single-Pack-Sperren für ID-Routing testen

**Status:** Regressionstests, keine neu zu erfindende Sperre. **Alt:** L10. **Voraussetzung:** M06.

Aktives Deferred-ID-Routing besitzt bereits `#error`-Prüfungen gegen Multi-Pack. Ihre Erreichbarkeit muss nach der vereinheitlichten Aktivierung abgesichert werden.

## Abnahme und Tests

- [ ] Für jeden betroffenen Deferred-Ausgang kompiliert gültiges Routing mit Single-Pack.
- [ ] Derselbe aktive Bereich mit Multi-Pack scheitert an der vorgesehenen Routing-Diagnose.
- [ ] Multi-Pack ohne aktivierte ID-Bereichsauswahl bleibt zulässig, soweit die übrige Konfiguration es unterstützt.
- [ ] Die Tests decken die relevanten Deferred-Pufferpfade ab und akzeptieren keine anderweitigen Compilerfehler.

Die Regel betrifft diesen Routing-Pfad, nicht TCOBS allgemein.

**Quelle:** [bestehende Compilezeitprüfungen](../../../src/trice.c).
