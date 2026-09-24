# Aktueller Dokumentations- und Planungsstand

Stand: 24. September 2026. M19 ist implementiert und im [deutschen UM-Kapitel „Strukturiertes Logging“](../TriceUserManual.md#strukturiertes-logging) dokumentiert. Dieser Bereich enthält die zugehörige Entwurfsreferenz und die weiterhin geplante Erweiterung M20.
Dieser Bereich enthält die aktuellen Entwürfe und den Implementierungsplan für M19 und M20. Das [User Manual](../TriceUserManual.md) beschreibt vorhandene Funktionen auf Englisch; die folgenden deutschen Dokumente beschreiben noch nicht implementierte Erweiterungen.

## Hier weiterarbeiten

- [Strukturiertes Logging – M19](Strukturiertes_Logging_DE.md): Entwurfsreferenz zur implementierten Funktion; die Bedienungsdokumentation steht im UM.
- [Strukturiertes Logging – M19](Strukturiertes_Logging_DE.md): geschlossener Funktions- und Ausgabeentwurf.
- [Context Enrichment – M20](Kontextanreicherung_DE.md): geschlossener Entwurf für `bind -ce`.
- [Implementierungsplan](Implementierungsplan.md): festgelegte Verträge, Arbeitsschritte und Abnahme.
- [M20 Bind/CE Callsite-Injection PoC](Issue_M20_Bind_CE_Callsite_Injection_PoC.md): technischer Machbarkeitsnachweis und verbindliches Gate vor der M20-Implementierung.

M19 umfasst skalare Felder und Strings über `triceS`/`triceN`. Benannte Pufferfelder werden ausdrücklich abgewiesen; klassische Pufferlogs bleiben Meldungen. M20 ist weiterhin nicht implementiert und benötigt einen eigenen Auftrag. M19 nimmt keine CE-Regeln oder zusätzlichen Target-Argumente vorweg.
Die für den aktuellen Scope von M19 und M20 nötigen Produkt- und Designentscheidungen sind abgeschlossen. Offen sind Implementierungsdetails, technische Nachweise und Tests. Für M20 ist vor der eigentlichen Implementierung zwingend der separate Bind/CE-Callsite-Injection-PoC abzuarbeiten. Er muss insbesondere die Injektion zusätzlicher Runtime-Ausdrücke im lokalen Callsite-Scope sowie eine diagnosefreie Nutzung mit üblichen C/C++-Editoren/Language-Servern nachweisen. Implementierung erst auf ausdrückliches Kommando; dieser Dokumentationsauftrag erteilt es nicht.

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

**M18 – Beispiele validieren:** Die M19-Beispiele sind über die reale CLI-Verarbeitung einschließlich Replay, JSON/KV, Logdatei und Rohaufzeichnung geprüft; Target-Beispiele kompilieren für `bind` und `insert`. Die entsprechende Abnahme für M20 bleibt Teil seines späteren Auftrags.
**M18 – Beispiele validieren:** keine neue Funktion und derzeit kein eigener Implementierungsauftrag. Beispielprüfungen gehören in die Abnahme von M19/M20 und vor die spätere Übernahme ihrer englischen Bedienungsdokumentation ins UM.

## Weitere zurückgestellte Notizen

Die allgemeine Notizsammlung ist [archiviert](obsolete/scratchPad.md). Daraus bleiben zwei mögliche spätere Aufgaben: Lesbarkeit/Kommentierung der TriceABC-Beispielskripte prüfen und die MVP-Bezeichnungen im [Bind-Handbuch](../TriceBind/Trice_bind_90_MVP_User_Manual.md) redaktionell ablösen. Die Ablage erzeugter TriceABC-Dateien ist inzwischen in der [Beispiel-README](../../examples/TriceAbc/README.md#generator-workflow) beschrieben.

Diese Notizen sind kein zusätzlicher Auftrag zur Code- oder Handbuchüberarbeitung.

## Archivierte Unterlagen

[obsolete](obsolete/README.md) enthält frühere Handovers, erledigte M01–M16-Unterlagen und alte Reviews. Die ursprünglichen Kapitelkopien dort sind eingefrorene Referenzen. Für vorhandene Funktionen ist das UM maßgeblich; für M20 sind es der aktuelle Entwurf und Implementierungsplan.
[obsolete](obsolete/README.md) enthält frühere Handovers, erledigte M01–M16-Unterlagen und alte Reviews. Die ursprünglichen Kapitelkopien dort sind eingefrorene Referenzen. Maßgeblich sind ausschließlich die beiden Entwürfe direkt in diesem Verzeichnis und der aktuelle Implementierungsplan.
