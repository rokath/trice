# Aktueller Dokumentations- und Planungsstand

Stand: 23. September 2026. Dieser Bereich enthält die aktuellen Entwürfe und den nächsten Arbeitsplan. Das [User Manual](../TriceUserManual.md) beschreibt vorhandene Funktionen auf Englisch; die folgenden deutschen Entwürfe beschreiben noch nicht implementierte Erweiterungen.

## Hier weiterarbeiten

- [Strukturiertes Logging – M19](Strukturiertes_Logging_DE.md): gewählte Template-Syntax, Feldnamen und Ausgabeformen.
- [Context Enrichment – M20](Kontextanreicherung_DE.md): selektive Erweiterung von Bind-Aufrufen durch `-ce`.
- [Implementierungsplan](Implementierungsplan.md): festgelegte Regeln, verbleibende Entscheidungen, Arbeitsschritte und Prüfungen.

M19 und M20 sind in ihrer Richtung festgelegt. Sie sind keine offenen Grundsatzvergleiche mehr, aber noch keine vollständig geschlossenen Implementierungsverträge. Der Plan benennt die konkreten Lücken. Implementierung erst auf ausdrückliches Kommando; dieser Dokumentationsauftrag erteilt es nicht.

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

**M17 – frühe Hostfilterung:** optionale Messung und erst danach eine Optimierungsentscheidung. Keine Voraussetzung für M19 oder M20. Der spätere Vergleich muss typisierte strukturierte Ausgabe und CE berücksichtigen, damit nicht gegen einen bald überholten Decoderpfad optimiert wird.

**M18 – Beispiele validieren:** keine neue Funktion und derzeit kein eigener Implementierungsauftrag. Beispielprüfungen gehören in die Abnahme von M19/M20 und vor die spätere Übernahme ihrer englischen Bedienungsdokumentation ins UM. Im heutigen UM ergänzte Bedienungsregeln sind gegen Code und bestehende Regressionen geprüft; Hardwareabläufe wurden hierfür nicht ausgeführt.

## Weitere zurückgestellte Notizen

Die allgemeine Notizsammlung ist [archiviert](obsolete/scratchPad.md). Daraus bleiben zwei mögliche spätere Aufgaben: Lesbarkeit/Kommentierung der TriceABC-Beispielskripte prüfen und die MVP-Bezeichnungen im [Bind-Handbuch](../TriceBind/Trice_bind_90_MVP_User_Manual.md) redaktionell ablösen. Die Ablage erzeugter TriceABC-Dateien ist inzwischen in der [Beispiel-README](../../examples/TriceAbc/README.md#generator-workflow) beschrieben. Diese Notizen sind kein zusätzlicher Auftrag zur Code- oder Handbuchüberarbeitung.

## Archivierte Unterlagen

[obsolete](obsolete/README.md) enthält beide früheren Handovers, erledigte M01–M16-Unterlagen und alte Reviews. Die ursprünglichen Kapitelkopien dort sind eingefrorene Referenzen. Maßgeblich sind ausschließlich die beiden Entwürfe direkt in diesem Verzeichnis und der aktuelle Implementierungsplan. Historische Aussagen wie „noch umzusetzen“ oder frühere `-stf`-/`ctx`-Vorschläge sind keine neuen Aufträge.
