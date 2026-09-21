# M07: Vorhandene Single-Pack-Sperren für ID-Routing testen

**Status:** umgesetzt; die vorhandene Sperre bleibt unverändert. **Alt:** L10. **Voraussetzung:** M06.

Aktives Deferred-ID-Routing besitzt bereits `#error`-Prüfungen gegen Multi-Pack. Ihre Erreichbarkeit muss nach der vereinheitlichten Aktivierung abgesichert werden.

## Abnahme und Tests

- [x] Für jeden betroffenen Deferred-Ausgang kompiliert gültiges Routing mit Single-Pack.
- [x] Derselbe aktive Bereich mit Multi-Pack scheitert an der vorgesehenen Routing-Diagnose.
- [x] Multi-Pack ohne aktivierte ID-Bereichsauswahl bleibt bei fehlenden Grenzen und `0/0` zulässig.
- [x] Die Tests decken Ring- und Double-Buffer ab und prüfen bei der negativen Variante die vorgesehene Diagnose.

Die Regel betrifft diesen Routing-Pfad, nicht TCOBS allgemein.

**Umsetzung:** [bestehende Compilezeitprüfungen](../../../src/trice.c), [Single-/Multi-Pack-Compilermatrix](../../../pkg/tst/trice_routing_test.go).
