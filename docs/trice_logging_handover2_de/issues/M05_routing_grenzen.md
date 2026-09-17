# M05: Beide Grenzen eines Target-Routing-Bereichs einschließen

**Status:** geplante C-Korrektur. **Alt:** L02.

Die ID-Vergabe arbeitet einschließlich Minimum und Maximum. Die Vergleiche im aktiven Deferred-Routing schließen dagegen beide Endpunkte aus. Ein Bereich mit nur einer ID lässt dort nichts durch.

## Abnahme und Tests

- [ ] Gültig aktivierte UARTA-, UARTB-, Auxiliary8/32- und RTT-Bereiche verwenden inklusive Grenzen.
- [ ] Ausgangs-Stubs prüfen `min-1`, `min`, einen inneren Wert, `max` und `max+1`, soweit darstellbar.
- [ ] Ein Ein-ID-Bereich lässt genau diese ID durch.
- [ ] Ohne Routing bleibt der Ausgang unverändert nutzbar; die Aktivierungsregeln werden separat in M06 vereinheitlicht.

**Quellen:** [Deferred-Ausgabe](../../../src/trice.c), [Bedeutung der Grenzen](../../../src/triceDefaultConfig.h), [ID-Vergabe](../../../internal/id/switchIDs.go).
