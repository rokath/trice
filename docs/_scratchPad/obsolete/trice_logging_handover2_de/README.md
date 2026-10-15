# Archiv: Logging-Handover und erledigte M-Aufgaben

Dieses Handover ist archiviert. **M01 bis M16 sind als Implementierungsaufgaben abgeschlossen.** Vorhandene Bedienung wird im [User Manual](../../../TriceUserManual.md) gepflegt; die Zuordnung der Aufgaben zu den UM-Abschnitten steht im [aktuellen Index](../../README.md).

## Aktuelle Unterlagen

- [Strukturiertes Logging – M19](../../Strukturiertes_Logging_DE.md)
- [Context Enrichment – M20](../../Kontextanreicherung_DE.md)
- [Implementierungsplan und Restentscheidungen](../../Implementierungsplan.md)

Die Kapitelkopien hier bleiben historische Momentaufnahmen. Die frühere Auswahl zwischen mehreren Logging-Grundmodellen ist entschieden: M19 verwendet normale Trice-Aufrufe mit benannten oder abgeleiteten Template-Feldern. M20 beschreibt selektives Bind-Enrichment durch `-ce`. Die aktuelle Prüfung zeigt noch offene Export- und Integrationsverträge; beide Funktionen sind nicht implementiert und werden erst auf ausdrückliches Kommando begonnen.

## Aufgabenstatus

| Aufgaben | Status |
|---|---|
| M01–M16 | Implementiert; historische Einzel-Issues dokumentieren Absicht und damalige Abnahme |
| [M17](issues/M17_fruehe_filterung.md) | Zurückgestellt; erst messen, dann gegebenenfalls optimieren |
| [M18](issues/M18_beispiele_pruefen.md) | Zurückgestellt als eigenständiger Auftrag; Beispielprüfung gehört zur späteren Abnahme |
| [M19](issues/M19_strukturierte_felder.md), [M20](issues/M20_kontext_vertrag.md) | Alte Issue-Texte durch aktuelle Entwürfe und Implementierungsplan abgelöst |

Frühere Aussagen „noch umzusetzen“, alte Aufgabenanzahlen und frühere Pfade sind keine aktuelle Arbeitsanweisung. Insbesondere sind farbige `-ulabel`-Metadaten inzwischen vorhanden, und es gibt keine acht aktiven Implementierungs-Issues mehr.

Die Abnahmehäkchen im Archiv ersetzen keinen aktuellen Testlauf. Die bekannten M15-Alt-Erwartungen sind im Implementierungsplan als Test-Ausgangsstand vermerkt.

## Historische Grundlage

Das [erste Handover](../trice_logging_handover/README_DE.md) und der [Chat-Export](../trice_logging_handover/Trice_Logging_Chat_Transcript_2026-09-14.md) bleiben erhalten. Ältere relative Codeverweise beziehen sich teilweise auf die ursprüngliche Ablage direkt unter `docs`; für aktuelle Einstiege den zentralen Plan verwenden.
