# M06: Routing-Aktivierung und unvollständige Bereiche vereinheitlichen

**Status:** umgesetzt. **Alt:** L09.

UARTA aktiviert den Bereich derzeit bei mindestens einer von null verschiedenen Grenze; UARTB verlangt beide. Andere Deferred-Ausgänge prüfen die Existenz der Defines. Gleiche Absichten können dadurch unterschiedlich wirken.

**Entwurfsregel:** Nicht gesetzte Grenzen gelten als null. `0/0` deaktiviert die Bereichsauswahl. Genau eine von null verschiedene Grenze sowie umgekehrte oder unzulässige Grenzen erzeugen einen verständlichen Konfigurationsfehler. Ein gültiger vollständiger Bereich aktiviert das Routing.

## Abnahme und Tests

- [x] Für jeden betroffenen Ausgang werden fehlend, `0/0`, nur Minimum, nur Maximum, gültig, umgekehrt, negativ und oberhalb von 16383 kompiliert.
- [x] Fehlermeldungen nennen beide zu korrigierenden Defines und die verletzte Regel; die Tests prüfen genau diese Diagnose.
- [x] Gültige Bereiche routen nach M05 inklusive Grenzen; `0/0` lässt alle geprüften IDs durch.
- [x] Custom-/Direct-Routing und seine Callback-Pfade bleiben unverändert.

**Umsetzung:** [einheitliche Aktivierung und Diagnosen](../../../src/trice.c), [Defaultgrenzen](../../../src/triceDefaultConfig.h), [Compiler- und Laufzeitmatrix](../../../pkg/tst/trice_routing_test.go).
