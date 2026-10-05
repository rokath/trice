# Release-Handover

## Ziel und aktueller Auftrag

Trice v2.0.0 für ein Release vorbereiten. Maßgebliche Aufgaben und Abhängigkeiten stehen im [Implementierungsplan](Implementierungsplan.md). Aktueller Auftrag ist ausschließlich `handsoff release`; keine neue Implementierung gestartet. Nach `handson release` den Stand prüfen und auf den nächsten Benutzerauftrag warten.

## Git-Stand

- Branch: `wip`
- HEAD: `0e563c83dac867541beda4dbe031257dab94e391`
- Arbeitsverzeichnis vor dieser Übergabe: sauber.
- Diese Übergabedatei ist neu und noch nicht committed; keine weiteren lokalen Änderungen vorhanden.
- Vier lokale Commits gegenüber der vorhandenen Tracking-Referenz `origin/wip`; kein Push ausgeführt. Der tatsächliche Remote-Stand wurde nicht live geprüft.

## Abgeschlossener Stand

Die letzten Änderungen sind vollständig committed: native Clang-Format-Batchverarbeitung mit gepinnter Version 23.1.2 und gezielten Stabilitätskorrekturen, Archivierung der Codex-Umzugsskripte, Regeln für `handsoff`/`handson` in [AGENTS.md](../../AGENTS.md) und Aufgabenentwurf R25.

R25 beschreibt die deterministische Zuordnung identischer Trices: normalisierter relativer Dateipfad, numerische Zeile und Quellposition; zulässige IDs aufsteigend zuordnen. Parallel analysieren, anschließend deterministisch zuweisen und Quellen/Sidecars sowie TIL/LI konsistent schreiben. Bei Hinzufügen eines dritten identischen Trice dürfen beide bisherigen IDs wechseln. R25 ist nur ein Entwurf; weder Umsetzung noch Release-Pflicht beauftragt. Vor Umsetzung sind feste IDs und Teilscans zu klären.

## Bindende Entscheidungen

- Release-Ziel v2.0.0; Go-Modulpfad vorerst ohne `/v2`.
- Englische bleibende Anwenderdokumentation und Commit-Nachrichten; Implementierungsplan auf Deutsch.
- Vollreferenz künftig `docs/TriceReferenceManual.md`; neues kurzes, einladendes UM, Root-README als Einstieg. Beispiel-READMEs sollen nur zur zentralen Anwenderdokumentation verlinken.
- Keine eigenmächtige Migration, neue Features oder Erweiterung des vereinbarten Umfangs. Spezifikationsänderung ist kein Implementierungsauftrag.
- Archive unter `obsolete` unverändert lassen. Routinearbeiten ohne zusätzliche Freigabe fortsetzen; nur wesentliche offene Entscheidungen klären. Commit und Push jeweils nur auf ausdrücklichen Auftrag.

## Prüfungen dieses Standes

- `rtk env GOCACHE=/private/tmp/trice-gocache go test ./scripts -run '^TestClangFormat' -count=1`: bestanden, 20.359 s auf macOS.
- `rtk proxy bash -n scripts/_280_format_c_code.sh scripts/_430_test_clang_format.sh`: bestanden.
- `rtk proxy ./scripts/_280_format_c_code.sh check`: bestanden, gepinnter Formatter; 1126 Dateien im Batch.
- `rtk proxy ./scripts/test_codex_handover.sh -v`: fehlgeschlagen mit `ModuleNotFoundError: No module named 'codex_handover_export'`.
- Kein neuer vollständiger `testAll.sh full`-Lauf für diesen HEAD. Frühere Läufe ersetzen keine Abnahme dieses Standes oder eines neuen Rechners.
- Staged-Diff-Prüfungen bestanden außer drei vorhandenen Markdown-Zeilenumbrüchen mit zwei Leerzeichen in der archivierten Clang-Format-Übergabe; Archiv unverändert übernommen.

## Offene Punkte und relevante Dateien

Die vier Umzugsskripte wurden nach `obsolete` verschoben. Aktive Tests und Anleitung verweisen noch auf die früheren Einstiege. Die angefangene Unterstützung paginierter Codex-History liegt unvollständig und ungeprüft im Archiv; nicht als funktionierenden Export behandeln und nicht automatisch fortsetzen. Ein isolierter Vorversuch zeigte, dass bloßes `thread/read` paginierte History nicht vollständig lädt; dies ist kein produktiver Nachweis. Keine persönlichen Codex-Daten exportiert oder importiert. Für die Weiterarbeit ist kein lokales temporäres PoC-Artefakt erforderlich.

Relevante aktive Dateien:

- [Implementierungsplan](Implementierungsplan.md): Release-Aufgaben und R25.
- [Aktuelle Vollreferenz](../TriceUserManual.md): Ausgangspunkt für R18/R19.
- [Clang-Format-Skript](../../scripts/_280_format_c_code.sh), [Prüfeinstieg](../../scripts/_430_test_clang_format.sh), [Verhaltenstests](../../scripts/portability_test.go), [CI](../../.github/workflows/clang-format.yml).
- [Aktiver Handover-Test](../../scripts/test_codex_handover.py) und [Anleitung](../Codex_Rechnerwechsel_DE.md): passen noch nicht zur Archivierung.

## Nächste Schritte

Nach Übernahme Branch, HEAD, Arbeitsverzeichnis und relevante Dateien gegen diese Übergabe prüfen; Abweichungen nennen und auf den Auftrag warten. Als nächster Release-Schritt bietet sich R18 an: vollständige Referenz umbenennen und aktive Verbraucher gezielt nachziehen; anschließend R19, R23, R12 und R20 gemäß Abhängigkeiten. R14/R15/R16 bleiben für Installationswege, Release-Artefakte und finale Abnahme offen. Die Archivierungsfolgen der Umzugswerkzeuge vor erneuter Verwendung gesondert klären; nicht als Nebenarbeit des Releases reparieren.

Vor dem Rechnerwechsel diese Datei manuell hinzufügen, mit englischer Nachricht committen und den Commit übertragen, normalerweise per Push. Auf dem Zielrechner `handson release` verwenden.
