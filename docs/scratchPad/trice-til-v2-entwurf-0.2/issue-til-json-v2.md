# Issue: `til.json` Schema V2 und persistente Decoderparameter

## Ziel

Neue TIL-Dateien erhalten Schema V2. V2 speichert alle veränderlichen, projektspezifischen TREX-Decoderwerte mit Profilen und parameterweiser Historie. Bestehende V1-Dateien bleiben unverändert V1.

## Umfang

- V2-Reader und -Writer für `SchemaVersion`, `Profiles`, `StreamFormat`, `Trices`
- keine automatische V1→V2-Migration
- neue Datei: Tool-Defaults plus explizite CLI-Werte
- vorhandene V2: V2-Werte statt impliziter CLI-Defaults; explizit gesetzte Flags müssen vom Flag-Default unterscheidbar sein
- `-profile`, Defaultprofil und case-insensitive Namensauflösung
- `-stream-date` mit UTC-Präfixen wie `2026-06`
- `Current`/`HistoryUntil` je Parameter und Profil
- persistente Parameter gemäß Spezifikation
- TCOBS/TCOBSv1-Kanonisierung
- Expansion von `-ts`, `us`, `µs`, `ms`
- optionale Encryption-Felder `Password` und `Key`; leere oder entfernte Felder werden nicht automatisch rekonstruiert

## CLI-Änderungen

Ein expliziter, validierter CLI-Wert wird mit dem wirksamen V2-Wert verglichen. Bei Abweichung erfolgt je Parameter eine eigene Rückfrage. Erst wenn alle Abweichungen bestätigt sind, werden sie mit einem gemeinsamen UTC-Zeitpunkt gespeichert. Eine Ablehnung bricht ohne Änderung ab. Nichtinteraktive Abweichungen sind Fehler.

Ein temporärer Override ist nicht vorgesehen.

## Profilpflege

Add/Delete/Rename erfolgt zunächst nur per Editor. `Profiles` ist maßgeblich. Verwaiste Parameterwerte werden ignoriert und beim nächsten Schreiben weggelassen.

## Nicht enthalten

- V1-Migration
- Port- und Kommunikationsparameter
- Prefix/Suffix, Filter, Farben, PC-Zeitstempel
- `li.json`
- feste TREX-, Typ- oder Algorithmusdefinitionen

## Akzeptanz

- V1 Roundtrip bleibt V1.
- Neue Datei ist V2.
- V2 Roundtrip erhält Profile, Historie und optionale Secret-Felder.
- Fehlendes CLI verwendet V2, nicht Tool-Defaults.
- Abweichendes CLI fragt einzeln nach und schreibt nur nach vollständiger Bestätigung.
- `-stream-date=2026-06` wählt den Stand am 1. Juni 2026, 00:00:00 UTC.
- Profilnamen unterscheiden sich nicht nur durch Groß-/Kleinschreibung.
- Gelöschte Profile hinterlassen nach dem nächsten Schreiben keine Parameterreste.
