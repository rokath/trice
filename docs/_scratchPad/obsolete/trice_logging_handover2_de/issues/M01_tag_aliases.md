# M01: Lokale Alias-Korrektur durch Registertests absichern

**Status:** umgesetzt. **Alt:** L01.

Im Basiscommit `bc7c542b0252` sind `rx` (Read/Receive), `tx` (Write/Transmit) sowie `s` und `S` (Signal/Seconds) doppelt. Erste und spätere Treffer können unterschiedliche Zuordnungen erzeugen. `w` war bereits dort nicht doppelt.

**Inzwischen lokal korrigiert:** `rx` gehört zu Receive, `tx` zu Transmit, `s`/`S` zu Seconds. Signal verwendet `sig` als kanonischen Namen. Zusätzlich gehört `W` jetzt zu Write statt Warning. Diese Quelländerung lag bei Wiederaufnahme bereits vor und wurde im Dokumentationsauftrag nicht vorgenommen. Tests und ein Hinweis auf betroffene Filter/IDRange-Regeln fehlen als Absicherung noch.

## Abnahme und Tests

- [x] Jede Schreibweise im vollständigen Register gehört genau einer Gruppe; künftig ergänzte Tags werden mitgeprüft.
- [x] Ein künstliches Duplikat bestätigt, dass der allgemeine Registertest Doppelbelegungen tatsächlich erkennt.
- [x] Die geänderten Kurzformen werden im Kompatibilitätshinweis genannt; einzelne Aliasnamen erhalten keine redundanten Spezialtests.
- [x] Das Verhalten späterer `-ulabel`-Überschreibungen bleibt bei M08; M01 stellt dafür das eindeutige Basisregister sicher.

**Umsetzung:** [Tag-Tabelle und Lookup](../../../internal/emitter/lineTransformerANSI.go), [Registertests](../../../internal/emitter/lineTransformerANSI_test.go), [Kompatibilitätshinweis](../../TriceUserManual.md#how-to-get).
