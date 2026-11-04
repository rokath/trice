# Trice v2.0.0: vor dem Release

Stand: 2026-11-04. **V01–V07 sind umgesetzt und committet.** Das Archiv `docs/_scratchPad/obsolete` wurde mit Commit `3cbd1c7e` entfernt. Offen bleibt die endgültige Freigabe unter V08.

## Endgültigen Kandidaten freigeben

**V08 · offen**

Die [Release-Checkliste](../rm/tool_build_release.md#prepare-a-release) beschreibt die erforderlichen Prüfungen. Für den endgültigen Kandidaten müssen noch folgende Nachweise zugeordnet und festgehalten werden:

| Nachweis | Noch erforderlich |
| --- | --- |
| Kandidat | Überprüften Commit einschließlich aller Release-Änderungen auswählen, übertragen und dessen vollständigen SHA festhalten; sauberer Arbeitsbaum. |
| Native Tests | Erfolgreiche Full-Läufe auf Linux, macOS und Windows für diesen Kandidaten; Intel-/Apple-Silicon-Ergebnisse und akzeptierte Plattformgrenzen unterscheiden. |
| CI und Coverage | Erforderliche Actions erfolgreich; übersprungene Jobs prüfen und Coverage mit der Zielbranch-Basis vergleichen. |
| Installation und Snapshot | Beide Tools, Target-Sourcen, vollständige Lizenzen, Plattformarchive, Linux-Pakete und beide Handbuch-PDFs für diesen Stand prüfen. |
| Manuelle Abnahme und Grenzen | Relevante manuelle Kontrollen aus der Release-Checkliste durchführen; akzeptierte Einschränkungen mit Begründung festhalten. |

Die finale Testausführung übernimmt der Benutzer. Ergebnisse mit Commit, Betriebssystem, Testauswahl und gegebenenfalls Run-URL dokumentieren. Frühere erfolgreiche Läufe gelten nur für ihre ursprüngliche Revision.

Nach erfolgreicher Abnahme den unveränderlichen Tag setzen, veröffentlichen und die tatsächlichen Downloads prüfen; siehe [Release veröffentlichen](../rm/tool_build_release.md#publish-a-release-manually-from-a-selected-tag).

## Bisherige CI-Evidenz

Abfrage vom 2026-11-04, 10:40 UTC für `83d7316ff024b77aab586edbe427a767a3d6da67`: alle 15 zugeordneten Actions-Läufe abgeschlossen und erfolgreich, ohne Event-Filter und mit vollständiger Ergebnisliste. PR-Läufe verwenden zusätzlich die PR-Merge-Revision.

| Lauf | Ergebnis und Grenze |
| --- | --- |
| [Link Check](https://github.com/rokath/trice/actions/runs/41803513920) | **PASS.** Frühere LLVM-Verbindungsfehler in diesem Stand geschlossen. |
| [install-checks](https://github.com/rokath/trice/actions/runs/41803513828) | **PASS:** alle 14 Jobs, einschließlich beider nativer macOS-Archive, Windows/Linux, DEB/RPM/APK, Target-Sourcen und PDFs. |
| [Native Trice tests](https://github.com/rokath/trice/actions/runs/41803513935) | **PASS:** schnelle Auswahl auf Windows, Linux und macOS arm64. Intel-Full-Job und zusätzliche Concurrency-Schritte übersprungen; kein Full-Nachweis. |
| [TRICE Library CI](https://github.com/rokath/trice/actions/runs/41803513930) | **PASS:** Auswahl `quick`; keine vollständige Compiler-Matrix. |

Die damaligen LinkCheck-/ARM64-Lücken sind damit geschlossen. **Diese Läufe decken die späteren Änderungen V04–V07 und die Archivlöschung nicht ab.** Aktuelle Nachweise für den endgültigen Kandidaten sind noch einzutragen; die obige Evidenz ist keine Freigabe dieses Kandidaten.

## Abschluss und Folgearbeiten

V08 ist erledigt, wenn die vorgesehenen Prüfungen für den endgültigen Kandidaten bestanden und mögliche Abweichungen ausdrücklich akzeptiert sind. Hier anschließend den Kandidaten-SHA und die zugehörigen Nachweise festhalten.

Release Notes und Upgrade-Hinweise stehen im [Changelog](../../CHANGELOG.md) und der [Migrationsanleitung](../rm/id_management.md#migrating-1x-scripts-and-projects). Weitere Aufräumarbeiten, Dokumentationsverbesserungen und gesicherte Zukunftsideen stehen in [Nach dem Release](Release_v2.0.0_Nach_dem_Release.md). Erledigte Fehleranalysen und frühere Testprotokolle bleiben über Git verfügbar.
