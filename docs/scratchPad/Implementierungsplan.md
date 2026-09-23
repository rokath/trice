# Implementierungsplan für M19 und M20

Stand: 23. September 2026. **Nur Planung; Umsetzung erst auf ausdrückliches Kommando.** Maßgeblich sind [Strukturiertes Logging](Strukturiertes_Logging_DE.md) und [Context Enrichment](Kontextanreicherung_DE.md). Die alten M19/M20-Issues sind durch diesen Plan abgelöst.

## Einschätzung der Implementierungsreife

Die gewünschte Bedienung ist hinreichend klar, um den Ablauf und die Arbeitspakete festzulegen. Eine erneute Auswahl zwischen grundverschiedenen Logging-Modellen ist nicht nötig. Für eine durchgängige Implementierung ohne weitere Produktentscheidungen fehlen jedoch noch die unten genannten Verträge. Sie betreffen beobachtbares Verhalten, nicht bloß interne Namen oder Dateiaufteilung.

Interne Datenstrukturen, Hilfsfunktionen und eine minimale Dateiaufteilung kann die Implementierung im beauftragten Umfang selbst festlegen. Die offenen Nutzerentscheidungen betreffen vor allem den verlustfreien Export, die Behandlung vorhandener Klammertexte und den CE-Umfang. Für Routineentscheidungen ist keine neue Freigaberunde vorgesehen.

M01–M16 und die Farberweiterung für `-ulabel` bilden die vorhandene Grundlage. M17 und M18 sind keine vorgeschalteten Entwicklungsprojekte. Vor Codeänderungen ist ein nachvollziehbarer Test-Ausgangsstand erforderlich: In [insertIDs_test.go](../../internal/id/insertIDs_test.go) erwarten `TestInsertExistingID_A/B` weiterhin, dass ID 77 bei Bereich 100..999 unverändert bleibt. Das widerspricht der mit M15 eingeführten Neuzuweisung. Diese bekannte Testaltlast ist separat abzugleichen; der heutige Dokumentationsauftrag ändert keinen Test und behauptet keine bestandene Gesamtsuite.

Der gezielte Lauf `go test ./internal/id -run '^TestInsertExistingID_A$' -count=1` bestätigt diesen Fehler: erwartet wird 77, tatsächlich zugewiesen wird 999. Ein vollständiger Go-/C-Testlauf war nicht Bestandteil dieses Dokumentationsabgleichs.

## Bereits festgelegt

| Bereich | Vertrag |
|---|---|
| Aufruf | Normales `trice()`; keine neue primäre Target-API. Variante A ist gewählt, B nicht Bestandteil, C eine spätere Option. |
| Platzhalter | `{name}`, `{}`, `{prefix.}` sowie Darstellung nach dem ersten Doppelpunkt; `%...` und strukturierte Platzhalter konsumieren Argumente von links nach rechts. `{{`/`}}` sind literale Klammern. |
| Namensableitung | Bezeichner und reine Memberketten; `.`/`->` werden zu Punktnamen, `aFloat`/`aDouble` sind transparente Hüllen. Andere Ausdrücke brauchen einen expliziten Namen. |
| Werte | Darstellung ändert nur `message`. Felder behalten ihre typisierten Werte; Feldnamen werden nicht zusätzlich auf dem Draht übertragen. |
| Ausgabe | Geplant: `-logFormat text` als Default, außerdem `json` und `kv`; ein strukturierter Record pro Trice-Aufruf, unabhängig von Konsolen-Teilzeilen. |
| Metadaten | Bestehende Ausgabeoptionen steuern vorhandene ID-, Orts- und Stempelwerte; keine ANSI-Sequenzen oder Spaltendekoration in maschinenlesbarer Ausgabe. |
| CE | `bind -ce` ergänzt Darstellung und Runtime-Ausdrücke vor Schema-/ID-Bestimmung, vor dem abschließenden Newline. Keine implizite Runtime-Kontextverwaltung. |
| CE-Selektion | Freie CE-Selektoren werden case-neutral verglichen; eingebaute Tag-Aliase nutzen ihre feste Gruppe. Nur vollständig kleingeschriebene Selektoren werden aus dem sichtbaren Text entfernt. |
| Gemeinsames Modell | CE-Felder und direkt geschriebene Felder bilden dasselbe flache Feldschema. Kein automatisch angelegter `ctx`-Namensraum. Doppelte Namen bleiben nach dem CE-Entwurf erlaubt. |
| Abgrenzung | `-ulabel` bleibt unabhängige Tag-Metadatenkonfiguration. Kein allgemeiner Parserumbau, neues Drahtformat oder Serverlogger-Ersatz. |

## Noch zu schließen vor dem jeweiligen Implementierungsschritt

### Feldschema und verlustfreier Export

- **Doppelte Namen:** Der Entwurf erlaubt sie, zeigt für JSON aber ein Objekt `fields`. Für zwei Felder gleichen Namens fehlt die eindeutige verlustfreie Abbildung. Zu entscheiden sind etwa eine geordnete Name/Wert-Liste oder eine Mehrfachwertdarstellung. Stilles Überschreiben wäre mit dem gewünschten Werterhalt unvereinbar. Dasselbe gilt für wiederholte Schlüssel in `kv` und für Kollisionen mit `tag`, `level`, `message`, `id` oder `ts`.
- **Feldtypen:** `%d` als Default kann nicht den C-Typ beliebiger Ausdrücke bestimmen. Zu fixieren sind Typ/Bitbreite und Grenzen aus Trice-Makro, Konvertierung und Wrapper, insbesondere unsigned Werte, Pointer, Strings/Buffer sowie `aFloat`/`aDouble`. Die Darstellung darf den einmal bestimmten strukturierten Wert nicht nachträglich umdeuten.
- **Exportvertrag:** JSON-Zahlen über der exakten JavaScript-Ganzzahlgrenze, NaN/Unendlich, Escaping und Quoting in `kv`, leere Felder, stabile Reihenfolge sowie Stempeltypen und -einheiten benötigen konkrete Beispielausgaben. Ein unterdrückter Text-Newline darf keine strukturierte Recordgrenze beseitigen.
- **Tag-/Level-Metadaten:** Festzulegen sind Ausgabe von `untagged`, die vollständige `levelTags`-Menge und deren Verhältnis zum numerischen Gruppengewicht. Vorschlag: `tag` immer aus der vorhandenen Klassifikation übernehmen und eine CLI-Erweiterung für `levelTags` zunächst zurückstellen. Das ist noch keine akzeptierte Erweiterung der Spezifikation.

### Wörterbuch und Quellkompatibilität

- Das konkrete TIL-Schema muss Originaltemplate, effektiven Textformatstring, Argumentpositionen, Typen und kanonische Feldnamen eindeutig zuordnen. Das bestehende [TriceFmt](../../internal/id/id.go) enthält nur `Type` und `Strg`; seine Verwendung als Vergleichs-/Map-Schlüssel darf nicht versehentlich Schemaänderungen ignorieren.
- Die Schemaidentität muss Feldänderungen auch bei unverändertem effektivem Text erkennen und eine neue ID auslösen. Historische Definitionen bleiben erhalten; neue aktive IDs müssen die M15-Policy einhalten. Wörterbuchauswahl bei mehreren Firmwareständen/Quellen ist Teil des Vertrags, kein impliziter globaler ID-Namensraum.
- Der neue Klammerparser benötigt eine festgelegte Behandlung vorhandener literaler `{`/`}` und eine klare Aktivierung oder Migration. Der Entwurf verzichtet auf einen Legacy-Sonderparser; daraus folgt noch kein sicherer Migrationsablauf für bestehende Quellen und Wörterbücher.
- Bei Bind muss dieselbe unveränderte Source beim nächsten Lauf wieder dasselbe Schema ergeben. Bei Insert/Clean darf die Rücknahme das ursprüngliche Template nicht verlieren. Erzeugte lokale Texttabellen müssen weiterhin printf-kompatible effektive Formate erhalten.
- Für die optionale Registry fehlen Schalter/Dateiname und Update-Scope: insbesondere Teilbaumläufe, Zählung gleicher Namen mehrfach an einer Logstelle sowie Hostfelder ohne Quellplatzhalter. Die vereinbarte automatische, stille Pflege bleibt erhalten; der Umfang muss deterministisch sein.

### CE-Selektoren und Bind-Integration

- Festzulegen sind Erkennung und Behandlung unbekannter beziehungsweise nicht konfigurierter `ctx7:`-Präfixe, wiederholte Selektoren, mehrere `-ce`-Regeln für denselben Selektor und Konflikte zwischen case-neutralen CE-Namen und case-sensitiven freien User-Tags. Die vorhandene Aussage „Selektorreihenfolge“ braucht eine eindeutige Regel für diese Mehrfachtreffer.
- Der aktuelle [Bind-Descriptor](../../internal/id/bindRender.go) und [TRICE_BIND_AUTO/REPLACE](../../src/triceBind.h) ergänzen IDs; sie erzeugen noch keine zusätzlichen Argumente oder geänderte Makro-Arity. Ein konkretes generiertes C-Beispiel muss zeigen, wie neue Ausdrücke am ursprünglichen Aufrufort auf lokale Variablen zugreifen und genau einmal ausgewertet werden. Das ist die zentrale technische Machbarkeitsprüfung für M20.
- Makrofamilie, Gesamtargumentzahl und Bitbreite nach CE, Wrapper/Assertions, Templates/Header sowie Verhalten bei `TRICE_OFF` müssen begrenzt und getestet werden. Überläufe und nicht unterstützte Kombinationen müssen vor dem Schreiben scheitern. Kein Versprechen zusätzlicher Argumente ohne Laufzeit- oder Bandbreitenkosten.
- Der aktuelle CE-Entwurf spezifiziert **Bind**. Das alte Issue forderte zusätzlich Insert/Clean. Vorschlag: M20 zunächst auf Bind begrenzen; Insert/Clean für CE nur nach ausdrücklicher Erweiterung mit eigenem Rücknahmevertrag. Diese Scope-Entscheidung ist vor M20-Code zu bestätigen. Die gewöhnliche M19-Template-Unterstützung für Insert/Clean bleibt davon getrennt.
- Wirksame CE-Konfiguration und Schema müssen in regenerierbaren Artefakten erkennbar sein. Konfigurationswechsel, Rückkehr zu einer alten Konfiguration, verschobene/duplizierte Sites und unterbrochene Schreibvorgänge dürfen alte IDs nicht umdeuten. Geteilte TILs und konkurrierende Läufe benötigen dieselben bestehenden Sicherungen wie die ID-Vergabe.

## Vorgesehene Reihenfolge nach Implementierungsauftrag

| Schritt | Konkretes Ergebnis | Abnahme |
|---|---|---|
| Ausgangsstand | Restentscheidungen dokumentiert; bekannte M15-Testerwartungen geklärt; grüne relevante Baseline oder exakt abgegrenzte bekannte Fehler | Keine unbelegten Erfolgsangaben aus alten Checklisten übernehmen |
| M19 Formatparser | Gemeinsamer Parser für gemischte Platzhalter, Escapes, Darstellung und Namensableitung; reine Analyse ohne Dateischreiben | Lesbare Tabellen mit Erfolg, Ablehnung, unbalancierten Klammern, Kommas in Ausdrücken und Argumentzahlfehlern |
| M19 Schema und IDs | Vereinbartes TIL-Schema, Schemaidentität und Integration in Bind/Insert/Clean, optionaler Registry-Vertrag | Feldumbenennung erzeugt neue ID; wiederholter Lauf stabil; alte Definition weiter dekodierbar; Schreibfehler verändern keine fremden Daten |
| M19 Decoder-Record | Typisierte Werte und Metadaten mit Ereignisgrenze für den vereinbarten Decoderumfang | Werte vor Textformatierung erhalten, keine veralteten Records nach Fehlern; bestehende [VisRecord](../../internal/decoder/decoder.go)-Schnittstelle berücksichtigen |
| M19 Ausgabe | Gemeinsame text/json/kv-Ausgabe hinter denselben Anwendungsfiltern | Exakte Bytes, Recordgrenzen, Präzision, doppelte Namen, Diagnosetrennung, Teilzeilen, Statistik und `-vis`; lokale und Remote-Ausgabe ausdrücklich abdecken oder begrenzen |
| M20 CE-Analyse | Regelparser und deterministische Auswahl; CE vor dem gemeinsamen M19-Parser | Mehrere Treffer, Cases/Aliase, unbekannte Präfixe, gemischte klassische/strukturierte Argumente |
| M20 Bind-Erzeugung | Nachgewiesener Generatorpfad für zusätzliche Runtime-Argumente im lokalen Scope | Kompilierte C-Integration, einmalige Auswertung, `TRICE_OFF`, Arity/Bitbreite, Rebase/Wrapper; unveränderte Log-Aufrufe in Anwenderquellen |
| M20 Lebenszyklus | Konfiguration/Schema in Artefakten, reproduzierbare Regeneration und Fehlerbehandlung | CE an/aus/geändert, Rückkehr zur alten Vorlage, verschobene/duplizierte Sites, unveränderte Wiederholung, volle ID-Bereiche und Schreibfehler |
| Gemeinsamer Abschluss | Geprüfte Beispiele und englische UM-Bedienung nach M18; Entwürfe erhalten eindeutigen Umsetzungsstatus | Passende Go- und C-Tests; vor Push die vorgeschriebene Coverage-Prüfung; separate sinnvolle Commits nur auf Commit-Auftrag |

Die beiden Funktionen bleiben fachlich orthogonal: M19 benennt vorhandene Werte, M20 erfasst zusätzliche Werte. Der gewählte M20-Entwurf verwendet technisch den M19-Parser und dessen Schema. Deshalb zuerst diese gemeinsame Grundlage, anschließend CE. Ein bloßer TIL-Text kann keinen zusätzlichen Runtime-Wert erfassen.

## M17 und M18

**M17 bleibt zurückgestellt.** Bei späterem Auftrag zuerst Replay-Messungen für 0, 50, 90 und 100 Prozent verworfene Ereignisse mit CPU, Allokationen und Durchsatz. Danach über eine Optimierung entscheiden. Framing/Integritätsprüfung, Stempel-/Cycle-Zustand, Rohaufzeichnung, Statistik, Diagnosen und akzeptierte Records müssen identisch bleiben. Wörterbuch- oder Registry-Änderungen dürfen keine veraltete Filterentscheidung behalten. Ohne belegten Nutzen endet M17 beim Messergebnis.

**M18 wird Abnahmearbeit innerhalb M19/M20.** Beispiele erst gegen die dann vorhandene CLI und passende Wörterbücher ausführen; JSON/kv einschließlich Grenzwerten vergleichen, C-Kontexte kompilieren und Shell-Quoting prüfen. Entwurfsoptionen bis dahin deutlich als geplant kennzeichnen. Kein Implementieren einer Option allein zum Ausführen eines Dokumentbeispiels und kein eigenes großes Testframework.
