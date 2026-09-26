# Arbeitsplan für Structured Logging und Context Enrichment

Stand: 26. September 2026. Dieser Plan ordnet die noch offenen Arbeiten. Structured Logging ist in [Kapitel 32 des User Manuals](../TriceUserManual.md#strukturiertes-logging) dokumentiert. [Context Enrichment](Kontextanreicherung_DE.md) ist zurückgestellt. Der CE-Machbarkeitsnachweis muss vor der CE-Implementierung erfolgen.

## Offene Aufgaben in Arbeitsreihenfolge

### A9 – Context Enrichment mit PoC beginnen

- Erst nach A1–A7: Führe den isolierten Nachweis für die in [Context Enrichment](Kontextanreicherung_DE.md) beschriebene Bind-Injektion aus. Zeige zusätzliche lokale Werte bei argumentlosen und bereits parametrisierten Trices, einfache Ausdrücke, genau einmalige Auswertung, korrekte Arity und TIL-Konsistenz, unveränderten User-Source sowie idempotente Bind-Läufe.
- Prüfe zusätzlich die Diagnosefreiheit in üblichen compilerbewussten C/C++-Editoren und Language-Servern mit realer Compile-Konfiguration. Scheitert der Nachweis, ist die Architektur vor der CE-Implementierung neu zu entscheiden.

### A10 – Context Enrichment implementieren und abnehmen

- Nur nach bestandenem A9: Setze zunächst `trice bind -ce` nach dem [deutschen CE-Vertrag](Kontextanreicherung_DE.md) um. CE ergänzt den finalen strukturierten Template-String vor Schema- und ID-Bestimmung; injizierte Runtime-Ausdrücke werden pro Aufruf genau einmal ausgewertet. Source, Sidecars und TIL müssen bei unveränderter Konfiguration reproduzierbar bleiben.
- Prüfe Selektoren und Aliase, Regelreihenfolge, doppelte Selektoren, Feldkonflikte, Argumentzahl, Bitbreite/Wrapper, Konfigurationswechsel, `TRICE_OFF`, C-Kompilierung und Decoder-Ausgabe. Eine spätere Ausweitung auf `insert/clean` benötigt einen eigenen reversiblen Vertrag und Auftrag.

### A11 – Frühe Hostfilterung gesondert prüfen

- Diese zurückgestellte Optimierung (bisher M17: frühe Hostfilterung) ist keine Voraussetzung für Structured Logging oder CE. Miss zuerst Replay mit 0, 50, 90 und 100 Prozent verworfenen Ereignissen; erfasse CPU, Allokationen und Durchsatz. Entscheide erst anhand dieser Daten über Codeänderungen.
- Framing und Integrität, Stempelzustand, Rohaufzeichnung, Statistik, Diagnosen und akzeptierte Records müssen sich durch eine Optimierung nicht ändern. Beziehe typisierte strukturierte Ausgabe und spätere CE-Records in den Vergleich ein.

## Erledigter Stand als Reviewhilfe

### A1 – Test-Ausgangsstand und Policy-Anpassungen

Die Erwartungen an ID-Bereich, Tag-Policy und wiederholte Bind-Formate wurden korrigiert. `TestInsertExistingID_A/B` verwenden nun ausdrücklich einen Bereich, der ihre ID 77 enthält. Eine ungültige auskommentierte Trice-Zeile in `examples/G0B1_inst/Core/Inc/triceConfig.h` wurde entfernt. Die betroffenen Go-Pakete und der Clang-Insert-Integrationstest bestanden bei der A1-Prüfung. Das ist keine Aussage über einen vollständigen `testAll.sh`-Lauf; dessen Ergebnis ist gesondert zu bewerten.

### A2 – Metadatenvertrag für JSON und Key-Value

JSON und KV übernehmen aktivierte, vorhandene ID-, Orts- und Host-Zeitinformationen. Target-Stempel und Differenzen verwenden getrennte, formatierte Stringfelder `ts16`, `ts32`, `ts16Delta` und `ts32Delta`. Der erste Stempel jeder Bitbreite hat kein Delta-Feld; `ts0` und `ts0delta` erzeugen keine Metadaten. Kapitel 32 beschreibt die CLI-Konfiguration und Ausgabe.

### A3 – Tag, Meldungsinhalt und Stringwerte

`tag` wird über registrierte Aliase case-neutral kanonisiert. `message` folgt der Textdarstellung ohne äußere Dekoration und behält ihre führenden und folgenden Leerzeichen. Nur exakt registrierte, vollständig kleingeschriebene Präfixe werden bei aktiver Farbbehandlung entfernt; `-color off` erhält die Textpräfixe einschließlich des synthetischen `untagged:` für unbekannte Tags. Andere Stringwerte verlieren äußeren Leerraum. Klassische Pufferlogs bleiben feldfreie Meldungen. Laufzeitstrings ändern den Formatstring-Tag nicht; die bestehende Textumwandlung von Escape-Folgen gilt für `message`, während benannte Stringfelder den übertragenen Wert mit gekürztem äußerem Leerraum behalten. Das Verhalten und Beispiele stehen in Kapitel 32.

Der gemeinsame Template-Parser, die Kanonisierung in `til.json`, `bind`/`insert`/`clean`, typisierte Decoder-Records, `-logFormat text|json|kv` und `trice-fields.txt` sind vorhanden. Skalare Werte sowie Strings über `triceS`/`triceN` gehören zum aktuellen Scope; benannte Pufferfelder werden abgewiesen. Die Bedienung ist in [UM-Kapitel 32](../TriceUserManual.md#strukturiertes-logging) beschrieben. Diese Bestandsaufnahme ist keine Behauptung, dass alle bestehenden Tests bestehen oder die oben genannten Details bereits dem gewünschten Vertrag entsprechen.

Die früher abgeschlossenen Arbeiten an Tag-Aliasen, Gewichten, Auswahl, ID-Policy, Rohaufzeichnung, Statistik und Diagnosen stehen in den jeweiligen UM-Kapiteln. Die alten Handovers und Aufgaben liegen unter [obsolete](obsolete/README.md). Beispielvalidierung ist in A6, A7 und A10 als Abnahme enthalten und benötigt keinen eigenen Implementierungsblock.

### A4 – Ausgabeformat-Option und TREX-Geltungsbereich

`-logFormat` akzeptiert die Werte `text`, `json` und `kv` unabhängig von Groß- und Kleinschreibung sowie `key-value` als Alias für `kv`. Der Wert `json` erzeugt NDJSON mit genau einem JSON-Objekt und abschließendem LF pro akzeptiertem Ereignis; einen separaten CLI-Wert `ndjson` gibt es nicht. Die CLI-Hilfe benennt NDJSON kurz, Kapitel 32 beschreibt TREX als Voraussetzung für maschinenlesbare Ereignisse und die CLI weist CHAR/DUMP weiterhin verständlich ab. Gültige Varianten, ungültige Werte und reale Ausgabe werden getestet.

### A5 – Gemeinsames Build-Verzeichnis für das Feldregister

`bind` und `insert` verwenden `-buildDir` mit dem Default `build/triceIDs`. Bei `bind` liegen dort Sidecar-Header und `trice-fields.txt`, bei `insert` das Feldregister. `generate -logC` und der interne Remigrate-Helfer lesen Bind-Sidecars ebenfalls über `-buildDir`. Der alte CLI-Schalter `-bindDir` wird abgewiesen und ist kein Alias; die Repository-Workflows und das CMake-Beispiel verwenden den neuen Schalter. Kapitel 32.7 beschreibt das Register als Datei mit Feldnamen und Häufigkeiten des letzten erfolgreichen Laufs. Die Tests decken benutzerdefinierte Pfade, Wiederholung, alte Schalter, `-dry-run` und die öffentlichen Bind-Workflows ab.

### A6 – Verständliche C-Beispiele und Integrationstests

Nach den `assert`-Zeilen in `_test/testdata/triceCheck.c` stehen neun kompilierbare Beispiele für 8/16/32/64-Bit-Werte, Stempelvarianten, `triceS`/`triceN`, gemischte klassische und strukturierte Platzhalter, per `.` und `->` abgeleitete Feldnamen sowie `aFloat()` und `aDouble()`. Ein gezielter PC-Integrationstest prüft ihre vollständige Textausgabe mit den bestehenden CLI-Einstellungen durch C-Zielcode und Decoder, jeweils nach `bind` und `insert`. Generator- und strukturierte Decoder-Tests bestanden ebenfalls. Die vollständigen PC-Testläufe bleiben unabhängig davon an einer älteren Erwartung hängen: Bei `-color=off` erscheint für ungetaggte Meldungen heute `untagged:`, während die erste Bestandszeile noch ohne dieses Präfix erwartet wird. Diese Bestandsabweichung gehört nicht zu den neun A6-Beispielen.

### A7 – Structured Logging dokumentarisch abgeschlossen

Kapitel 32 wurde mit dem aktuellen CLI-Verhalten, den strukturierten Generator- und Decoder-Tests sowie A1–A6 abgeglichen. Die weiterhin gültigen Hinweise zu Platzhaltern, Typen und C-Beispielen stehen nun im UM; ältere Entwurfsannahmen zu Tag-Schreibweise, Target-Zeitstempeln und Build-Verzeichnis wurden nicht übernommen. Die frühere Entwurfskopie ist entfernt und der UM-Link auf die nicht mehr vorhandene ScratchPad-README zeigt auf diesen Plan. Context Enrichment bleibt ein separater, noch nicht implementierter Auftrag.

### A8 – Zusätzliche JSON-Ansichten über `generate`

`trice generate -onelineJSON -til til.json -li li.json` erzeugt auf Abruf `til.oneline.json` und `li.oneline.json`. Beide bleiben vollständige JSON-Objekte, nicht NDJSON: Jede ID und ihr kompakter Eintrag stehen zusammen auf einer Zeile. In der LI-Ansicht steht `Line` vor `File`, beispielsweise `"13000": {"Line":163,"File":"src/main.c"}`. Der Name `oneline` bezieht sich auf den Eintrag, nicht auf die ganze Datei. Mit `-li off` wird nur die TIL-Ansicht erzeugt. Fehlende oder ungültige angeforderte Eingabedateien werden vor Ausgabeschreibzugriffen abgewiesen.

Die Originaldateien bleiben maßgeblich und werden bei diesem Export nicht verändert. Der Export wird nach Änderungen an den Originalen erneut ausgeführt; `bind`, `insert`, `clean`, `add` und die Remigration erzeugen keine automatischen Kopien. Die bestehende JSON-Datenstruktur bleibt erhalten. `encoding/json` serialisiert Schlüssel und Werte; nur äußere Klammern, Kommata und Zeilenumbrüche werden für die Ansicht zusammengesetzt. Für TIL bleibt `SetEscapeHTML(false)` maßgeblich. Tests decken stabile ID-Reihenfolge, Escapes, Unicode, Roundtrips, unveränderte Wiederholungsläufe und das Zurückrollen bei einem Schreibfehler ab. Eine externe Abhängigkeit ist nicht erforderlich; der zusätzliche Aufwand entsteht nur beim angeforderten Export auf dem Host.
