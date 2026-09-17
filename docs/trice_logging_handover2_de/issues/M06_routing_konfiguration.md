# M06: Routing-Aktivierung und unvollständige Bereiche vereinheitlichen

**Status:** geplante C-Konfigurationskorrektur. **Alt:** L09.

UARTA aktiviert den Bereich derzeit bei mindestens einer von null verschiedenen Grenze; UARTB verlangt beide. Andere Deferred-Ausgänge prüfen die Existenz der Defines. Gleiche Absichten können dadurch unterschiedlich wirken.

**Entwurfsregel:** Nicht gesetzte Grenzen gelten als null. `0/0` deaktiviert die Bereichsauswahl. Genau eine von null verschiedene Grenze sowie umgekehrte oder unzulässige Grenzen erzeugen einen verständlichen Konfigurationsfehler. Ein gültiger vollständiger Bereich aktiviert das Routing.

## Abnahme und Tests

- [ ] Für jeden betroffenen Ausgang: fehlend, `0/0`, nur Minimum, nur Maximum, gültiger Bereich und umgekehrter Bereich.
- [ ] Fehlermeldungen nennen die zu korrigierenden Defines; ein zufälliger anderer Compilerfehler zählt nicht als Erfolg.
- [ ] Erlaubte Konfigurationen routen nach M05 inklusive Grenzen.
- [ ] Custom-/Direct-Routing wird anhand seines eigenen Vertrags bewertet; keine pauschale Änderung fremder Callback-Pfade.

**Quellen:** [Aktivierungsbedingungen](../../../src/trice.c), [Defaultwerte](../../../src/triceDefaultConfig.h).
