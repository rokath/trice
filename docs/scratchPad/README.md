# Aktueller Dokumentations- und Planungsstand

Stand: 24. September 2026. Dieser Bereich enthält die aktuellen Entwürfe und den Implementierungsplan für M19 und M20. Das [User Manual](../TriceUserManual.md) beschreibt vorhandene Funktionen auf Englisch; die folgenden deutschen Dokumente beschreiben noch nicht implementierte Erweiterungen.

## Hier weiterarbeiten

- [Strukturiertes Logging – M19](Strukturiertes_Logging_DE.md): geschlossener Funktions- und Ausgabeentwurf.
- [Context Enrichment – M20](Kontextanreicherung_DE.md): geschlossener Entwurf für `bind -ce`.
- [Implementierungsplan](Implementierungsplan.md): festgelegte Verträge, Arbeitsschritte und Abnahme.

Die für den aktuellen Scope von M19 und M20 nötigen Produkt- und Designentscheidungen sind abgeschlossen. Offen sind Implementierungsdetails, technische Nachweise und Tests. Implementierung erst auf ausdrückliches Kommando; dieser Dokumentationsauftrag erteilt es nicht.

## Erledigte Aufgaben und UM-Abdeckung

M01–M16 sind als Implementierungsaufgaben abgeschlossen. Die nachfolgende Zuordnung ersetzt die früheren laufenden Aufgabenlisten. Sie ist ein Dokumentationsabgleich mit dem aktuellen Code, keine Behauptung eines vollständig grünen Testlaufs.

| Aufgaben | Vorhandene Funktion | Stelle im User Manual |
|---|---|---|
| M01 | Eindeutige Tag-Aliase und Kompatibilitätshinweise | [Tags](../TriceUserManual.md#how-to-use-tags) |
| M02 | Atomare Prüfung vollständiger ID-Bereichsregeln | [ID Routing](../TriceUserManual.md#id-routing) |
| M03, M09 | Pick/Ban-Konflikt und ungültige Selektoren vor Eingabestart abweisen | [Auswahl](../TriceUserManual.md#selecting-tags-and-priority) |
| M04 | Ungefilterte Rohaufzeichnung, Replay, Append und Fehlerweitergabe | [Binary Logfile](../TriceUserManual.md#binary-logfile) |
| M05–M07 | Inklusive Deferred-Routen, einheitliche Grenzprüfung, Single-Pack-Pflicht | [ID Routing](../TriceUserManual.md#id-routing) |
| M08 | Gruppengewichte, INFO-Default, wiederholtes `-ulabel` | [Gewichte](../TriceUserManual.md#tag-weights), [User-Tags](../TriceUserManual.md#user-defined-tags-weights-and-colors) |
| M10 | Diagnosen bleiben von Anwendungsauswahl und Statistik getrennt | [Decoder diagnostics](../TriceUserManual.md#decoder-diagnostics) |
| M11 | Fehlende/unbekannte Formatstring-Tags werden `untagged` | [Untagged](../TriceUserManual.md#untagged-application-events) |
| M12, M13 | Ereignisfilter, Teilzeilen, Metadatenherkunft und Zeitdifferenzen | [Auswahl](../TriceUserManual.md#selecting-tags-and-priority) |
| M14 | Statistik zählt dekodierte Ereignisse vor der Anzeigeauswahl | [Statistik](../TriceUserManual.md#event-statistics) |
| M15, M16 | Aktive IDs an aktuelle Policy anpassen, historische TIL-Einträge behalten und mit `-v` knapp melden | [ID Routing](../TriceUserManual.md#id-routing) |

Die inzwischen ergänzten Farben für `-ulabel` sind ebenfalls im UM dokumentiert. INFO hat standardmäßig Gewicht 500, Message und `untagged` haben Gewicht 400; die Gewichtstabelle entspricht dem Register im Code.

## Zurückgestellte Aufgaben

**M17 – frühe Hostfilterung:** optionale Messung und erst danach eine Optimierungsentscheidung. Keine Voraussetzung für M19 oder M20. Der spätere Vergleich muss typisierte strukturierte Ausgabe und CE berücksichtigen, damit nicht gegen einen überholten Decoderpfad optimiert wird.

**M18 – Beispiele validieren:** keine neue Funktion und derzeit kein eigener Implementierungsauftrag. Beispielprüfungen gehören in die Abnahme von M19/M20 und vor die spätere Übernahme ihrer englischen Bedienungsdokumentation ins UM.

## Weitere zurückgestellte Notizen

Die allgemeine Notizsammlung ist [archiviert](obsolete/scratchPad.md). Daraus bleiben zwei mögliche spätere Aufgaben: Lesbarkeit/Kommentierung der TriceABC-Beispielskripte prüfen und die MVP-Bezeichnungen im [Bind-Handbuch](../TriceBind/Trice_bind_90_MVP_User_Manual.md) redaktionell ablösen. Die Ablage erzeugter TriceABC-Dateien ist inzwischen in der [Beispiel-README](../../examples/TriceAbc/README.md#generator-workflow) beschrieben.

Diese Notizen sind kein zusätzlicher Auftrag zur Code- oder Handbuchüberarbeitung.

## Archivierte Unterlagen

[obsolete](obsolete/README.md) enthält frühere Handovers, erledigte M01–M16-Unterlagen und alte Reviews. Die ursprünglichen Kapitelkopien dort sind eingefrorene Referenzen. Maßgeblich sind ausschließlich die beiden Entwürfe direkt in diesem Verzeichnis und der aktuelle Implementierungsplan.
