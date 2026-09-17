# M01: Lokale Alias-Korrektur durch Registertests absichern

**Status:** lokale Tabellenkorrektur vorhanden; Absicherung und Kompatibilitätshinweis offen. **Alt:** L01.

Im Basiscommit `bc7c542b0252` sind `rx` (Read/Receive), `tx` (Write/Transmit) sowie `s` und `S` (Signal/Seconds) doppelt. Erste und spätere Treffer können unterschiedliche Zuordnungen erzeugen. `w` war bereits dort nicht doppelt.

**Inzwischen lokal korrigiert:** `rx` gehört zu Receive, `tx` zu Transmit, `s`/`S` zu Seconds. Signal verwendet `sig` als kanonischen Namen. Zusätzlich gehört `W` jetzt zu Write statt Warning. Diese Quelländerung lag bei Wiederaufnahme bereits vor und wurde im Dokumentationsauftrag nicht vorgenommen. Tests und ein Hinweis auf betroffene Filter/IDRange-Regeln fehlen als Absicherung noch.

## Abnahme und Tests

- [ ] Jede Schreibweise im vollständigen Register gehört genau einer Gruppe; künftig ergänzte Tags werden mitgeprüft.
- [ ] Lookup, Filter, Farbe, Gewicht und Statistik verwenden dieselbe Gruppe.
- [ ] Tests prüfen die genannten Kollisionen und eine künstlich neu eingeführte Doppelbelegung.
- [ ] Die Bedeutung von `W` als Write und der Wechsel von `s`/`S` werden ausdrücklich geprüft und im Kompatibilitätshinweis genannt.
- [ ] Die späteren `-ulabel`-Überschreibungen verändern vorhandene Gruppen, statt doppelte Gruppen anzulegen.

**Quelle:** [Tag-Tabelle und Lookup](../../../internal/emitter/lineTransformerANSI.go).
