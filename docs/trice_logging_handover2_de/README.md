# Trice-Logging: überarbeiteter Arbeitsstand

Stand: 17. September 2026. Grundlage: lokaler Branch `wip`, Commit `bc7c542b0252`, die inzwischen lokal vorliegende Alias-Korrektur in `lineTransformerANSI.go` und die Abstimmung zum [Auftrag](../Command.md). **Dieses Paket dokumentiert; es implementiert keine der geplanten Änderungen.**

## Hier anfangen

1. [Log-Auswahl](chapters/Log_Auswahl_DE.md): vorhandene Bedienung und geplante Regeln für Gewichte, `-ulabel`, `untagged` und Filter.
2. [Kontextanreicherung](chapters/Kontextanreicherung_DE.md): zusätzliche Informationen, Bind/Insert und historische IDs.
3. [Strukturiertes Logging](chapters/Strukturiertes_Logging_DE.md): benannte Werte und zwei Bedienungsvarianten. Danach die offenen Entscheidungen unten besprechen.
4. [Korrekturvorschläge](Korrekturvorschlaege.md): konkrete Fundstellen im bestehenden UM und Help-Text; englischer Ersatztext zur späteren Übernahme.
5. Die M-Issues unten einzeln abarbeiten, sobald die jeweilige Spezifikation freigegeben ist.

Jedes Kapitel beginnt mit einem redaktionellen Vorspann; nur der Text unter der Trennlinie ist der UM-Entwurf. „Vorhanden“ bezeichnet den gelesenen Code, „geplant“ das gewünschte Verhalten. Noch nicht abgestimmte Details sind als Vorschläge bezeichnet.

Die Einzel-Issues enthalten ihre Testanforderungen. Es gibt keine zusätzliche Sammelkopie, Zwischenübersetzung oder wiederholten Testplan. M-Nummern sind lokale Referenzen, keine GitHub-Issues. Das [ursprüngliche Handover](../trice_logging_handover/README_DE.md) einschließlich [Chat-Export](../trice_logging_handover/Trice_Logging_Chat_Transcript_2026-09-14.md) bleibt als historische Grundlage erhalten. Neu abgestimmte Regeln dieses Pakets ersetzen widersprechende ältere Empfehlungen.

## Offene Entscheidungen zur Freigabe

| Frage | Vorschlag / Ort |
|---|---|
| Welche konkreten Zahlen und Grenzen gelten für Gewichte? | Bereich `0..999` und Gruppentabelle im [Log-Kapitel](chapters/Log_Auswahl_DE.md#la-gewichte). Richtung und inklusiver Vergleich sind festgelegt. |
| Wie sollen synthetische Präfixe und Teilzeilen erscheinen? | `untagged:` bei `-color off` sichtbar lassen; nur angenommene Teilaufrufe zusammensetzen. Vorschläge im [Log-Kapitel](chapters/Log_Auswahl_DE.md#la-untagged) und [M12](issues/M12_teilzeilen.md). |
| Wie werden strukturierte Felder angegeben? | Benannte Platzhalter oder separate Feldzuordnung; [Variantenvergleich](chapters/Strukturiertes_Logging_DE.md#sl-redaktion). |
| Wie werden Feldtypen und neue Feldnamen freigegeben? | Kleiner geprüfter Feldkatalog; automatische Aufnahme allein erkennt keine Tippfehler. [M19](issues/M19_strukturierte_felder.md). |
| Wie werden dynamische Kontextwerte bei Bind ergänzt und Logstellen nach Umbauten erkannt? | Erzeugte Artefakte mit Herkunftsnachweis; Mechanismus und Speicherort offen. [M20](issues/M20_kontext_vertrag.md). |
| Wie viel Statistik und Optimierung lohnt sich? | Zunächst bestehende Zähler eindeutig machen; Aufwand vor Ausbau bewerten. [M14](issues/M14_statistik.md), [M17](issues/M17_fruehe_filterung.md). |

Diese Fragen gehören zur Entwurfsabnahme. Die Kapitel stellen die Vorschläge nicht als verfügbare Funktionen dar.

## Abarbeitungsfolge

Kleine unabhängige Aufgaben stehen vorn; Voraussetzungen gehen vor Größe. S/M/L sind relative Umfangseinschätzungen, keine Zeitversprechen. M19/M20 können schon während der Dokumentabstimmung besprochen werden.

| Neu | Aufgabe | Alt | Umfang / Voraussetzung |
|---|---|---|---|
| [M07](issues/M07_single_pack_tests.md) | Vorhandene Single-Pack-Sperren testen | L10 | S/M; nach M06 |
| [M08](issues/M08_tag_gewichte_ulabel.md) | Gewichte und gewichtete User-Tags | L05, L06, Teil L11 | M; nach M01 |
| [M09](issues/M09_selektoren_validieren.md) | Selektoren nach Registrierung prüfen | L07, Teil L05/L11 | S/M; nach M08 |
| [M10](issues/M10_diagnosen_trennen.md) | Werkzeugfehler von Logmeldungen trennen | L14 | M |
| [M11](issues/M11_untagged.md) | Reservierten Auffangtag einführen | neu | M; nach M08–M10 |
| [M12](issues/M12_teilzeilen.md) | Teilzeilen und Ereignisgrenzen festlegen | L13 | M; vor Filterumbau |
| [M13](issues/M13_ereignisfilter.md) | Gemeinsam nach Tag und Gewicht filtern | L12, Teil L11 | M; nach M03, M08–M12 |
| [M14](issues/M14_statistik.md) | Statistik von Darstellung entkoppeln | L16 | M; nach M13, Aufwand prüfen |
| [M15](issues/M15_id_neuzuweisung.md) | Aktive IDs an neue Policy anpassen | L04, Teil L03 | M; nach M01/M02 |
| [M16](issues/M16_id_policy_warnung.md) | Historische Abweichungen knapp melden | Teil L03 | S/M; nach M15 |
| [M17](issues/M17_fruehe_filterung.md) | Nutzen früher Hostfilterung messen | L17 | M/L; nach M10–M14 |
| [M18](issues/M18_beispiele_pruefen.md) | Beispiele vor UM-Übernahme prüfen | L23 | S/M; je freigegebenem Entwurf |
| [M19](issues/M19_strukturierte_felder.md) | Strukturierte Bedienung auswählen | L26 | Entwurfsaufgabe |
| [M20](issues/M20_kontext_vertrag.md) | Reversible Kontextanreicherung spezifizieren | L22 | Entwurfsaufgabe; mit M19 abstimmen |

Abgeschlossen: [M01](issues/M01_tag_aliases.md) ordnet alle eingebauten Alias-Schreibweisen eindeutig zu, prüft das vollständige Register und dokumentiert den Kompatibilitätswechsel. [M02](issues/M02_idrange_validierung.md) weist fehlerhafte Bereichsregeln atomar vor Dateiänderungen ab. [M03](issues/M03_pick_ban_konflikt.md) erkennt den Pick-/Ban-Konflikt vor dem Start des Eingabekanals. [M04](issues/M04_rohaufzeichnung.md) schützt ungefilterte Rohbytes, Replay, Append und Schreibfehler. [M05](issues/M05_routing_grenzen.md) schließt beide Grenzen aller Deferred-ID-Routen ein. [M06](issues/M06_routing_konfiguration.md) vereinheitlicht Aktivierung, Defaultgrenzen und Konfigurationsfehler. Damit verbleiben 14 aktive M-Issues.

L11 legt fest, **welche Wichtigkeit** ein Tag hat; L12 verhindert, dass eine erlaubte Meldung ihren Zeitstempel verliert; L13 betrifft mehrere Aufrufe pro Zeile beziehungsweise mehrere Zeilen pro Aufruf; L17 soll unnötige Formatierungsarbeit sparen. Das sind unterschiedliche Aufgaben.

| Weitere alte Referenz | Umgang im neuen Paket |
|---|---|
| L18 | Durch den Log-Kapitelentwurf bearbeitet. Englische Übersetzung und UM-Einbau erst auf gesonderten Auftrag. |
| L19, L20 | Zusätzliche Compilezeit-Level-Makros und dynamische Target-Level-Steuerung verworfen; keine M-Issues. Vorhandenes `TRICE_OFF` bleibt dokumentiert. |
| L21 | Begriffsklärung und Umbenennung durch die drei Entwürfe und Korrekturvorschläge bearbeitet. |
| L24 | Parserkapitel-Umbau zurückgestellt. Eine begrenzte Datenschnittstelle für M19 ist keine Freigabe zum Parserumbau. |
| L25 | Geräte-Logs am Server auswerten und einen Serverlogger ersetzen sind zwei Aufgaben. Ersteres passt zum Entwurf; für Letzteres fehlt ein konkreter Bedarf. Unbelegte Leistungsversprechen sind in den Korrekturvorschlägen behandelt. |
| L26 | M19 prüft einen konkreten Auswerteablauf; eine neue allgemeine Logging-Plattform ist nicht beauftragt. |

## Konkrete Befunde und Prüfgrenze

- Im Basiscommit sind `rx`, `tx`, `s` und `S` gruppenübergreifend doppelt. Die inzwischen vorliegende lokale Korrektur ordnet sie eindeutig zu und verschiebt `W` von Warning zu Write. M01 hält noch nötige Registertests und Kompatibilitätshinweise fest. Das im alten Handover genannte `w` war bereits im Basiscommit nicht doppelt.
- IDRange-Regeln werden vollständig und atomar geprüft; fehlerhafte Angaben erreichen weder Insert noch Bind.
- Binäre Aufzeichnung vor der Hostfilterung ist einschließlich Replay, Append und Fehlerweitergabe abgesichert. Die Single-Pack-Sperren im Deferred-Routing werden mit M07 geprüft.
- Gewichte, `untagged` und die neue Neuzuweisungsregel sind geplant. Tabellenposition und mögliche Wiederverwendung regelwidriger IDs sind noch im Code.

Gelesen wurden insbesondere [Tag-Tabelle](../../internal/emitter/lineTransformerANSI.go), [Selektoren](../../internal/emitter/emitter.go), [CLI](../../internal/args/init.go), [Startablauf](../../internal/args/handler.go), [Übersetzer](../../internal/translator/translator.go), [ID-Regeln](../../internal/id/switchIDs.go), [ID-Wiederverwendung](../../internal/id/insertIDs.go), [Range-Tests](../../internal/id/coverage_additional_test.go), [Routing](../../src/trice.c) und [Default-Konfiguration](../../src/triceDefaultConfig.h).

Das ist eine lokale Quellenprüfung. Für dieses Dokumentpaket wurden keine Go-Tests, Firmware-Builds oder Hardwareversuche ausgeführt. Testlisten in Issues sind Anforderungen, keine Erfolgsmeldungen.

Geprüft wurden alle 25 Markdown-Dateien auf UTF-8, lokale Verweise einschließlich Sprungmarken und vollständige L→M-Zuordnung. Die zwei JSON-Beispiele sind syntaktisch gültig; die drei Shell-Blöcke bestehen `bash -n`. Das ist eine Dokumentprüfung, kein Ausführungstest der gezeigten Trice-Befehle.
