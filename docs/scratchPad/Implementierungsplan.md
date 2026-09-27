# Arbeitsplan für Structured Logging und Context Enrichment

Stand: 27. September 2026. Dieser Plan ordnet die noch offenen Arbeiten. Structured Logging ist in [Kapitel 32 des User Manuals](../TriceUserManual.md#strukturiertes-logging) dokumentiert. Der isolierte [CE-Machbarkeitsnachweis A9](Context_Enrichment_PoC.md) für direkte Logstellen ist bestanden. Wegen der ergänzend nachgewiesenen Rebase-Grenze ist A10 zunächst auf direkte, eindeutig über ihre Quellzeile adressierbare Logstellen begrenzt. Die produktive [Context-Enrichment-Implementierung](Kontextanreicherung_DE.md) steht weiterhin aus.

## Offene Aufgaben in Arbeitsreihenfolge

### A10 – Context Enrichment implementieren und abnehmen

**Entscheidung am 27. September:** Die erste Ausbaustufe verwendet den nachgewiesenen direkten Bind-Pfad und benötigt kein `__COUNTER__`. CE für Wrappermakros und Counter-Rebase wird als eigene Folgeaufgabe zurückgestellt. Die [Rebase-Gegenprobe](Context_Enrichment_PoC.md#ergänzende-rebase-gegenprobe-vor-a10) bleibt der Nachweis, warum das einfache Anhängen lokaler Ausdrücke dort nicht genügt. Die Entscheidungen zu expliziten Float-Wrappern, unveränderter ID bei gleichem Schema und normaler Darstellung bekannter Tags bleiben verbindlich. Diese Festlegung erweitert die Planung; sie kennzeichnet A10 nicht als implementiert.

- Überführe den testinternen A9-Adapter in `trice bind -ce` nach dem [deutschen CE-Vertrag](Kontextanreicherung_DE.md). Unterstützt werden zunächst direkte Trice-Aufrufe, die über Datei und Quellzeile eindeutig zugeordnet werden können, einschließlich solcher in normalen und `static inline` Funktionen. CE ergänzt den finalen strukturierten Template-String vor Schema- und ID-Bestimmung; injizierte Runtime-Ausdrücke werden pro tatsächlich ausgeführtem Aufruf genau einmal ausgewertet. CE selbst verändert die User-Logstellen nicht; Source, Sidecars und TIL müssen bei unveränderter Konfiguration reproduzierbar bleiben. Die normale erstmalige Bind-Einrichtung bleibt davon getrennt.
- Trenne Regelverarbeitung, finale Schema-/ID-Bestimmung und die technische Einfügung der Argumente. Eine spätere Unterstützung komplexer Logstellen soll diese Regeln, Datenformate und Verhaltenstests weiterverwenden. Änderungen an der Makroerzeugung können später nötig werden; eine vorsorgliche Migration oder zusätzliche CLI-Variante gehört nicht zu A10. Source-/TIL-Abgleiche wie bei `generate -logC` müssen bereits in der ersten Ausbaustufe die endgültigen CE-Metadaten konsistent verwenden.
- Trifft eine CE-Regel eine Wrapper-/Rebase-Stelle außerhalb dieses Umfangs, muss `bind` vor Schreibzugriffen auf Source, Sidecars, TIL, LI und Feldregister abbrechen. Kein stilles Auslassen der Regel und kein automatischer Wechsel zu `insert`. Ohne passende CE-Regel bleiben die bisherigen Bind-Fähigkeiten und ihre bestehenden Compileranforderungen erhalten.
- Bei der Ablehnung eines nicht unterstützten Bind-Konstrukts bleiben Datei, Zeile und konkrete Fehlerursache erhalten; hinzu kommt der kurze Hinweis `Search UM for "bind-limits".`. Dies gilt für bestehende Bind-Grenzen und die neuen CE-Grenzen, ebenso für den generierten Compilerhinweis bei fehlendem `__COUNTER__`. Keine langen Lösungstexte in der Fehlermeldung; die Erklärung steht im UM unter [bind-limits](../TriceUserManual.md#bind-limits).
- Dokumentiere dort verständlich für Nicht-Compiler-Experten: Warum eine eindeutige Logstelle nötig ist, warum lokale Werte nur an ihrem jeweiligen Ort verfügbar sind und warum `__COUNTER__`-Unterstützung allein die CE-Grenze nicht löst. Zeige mehrere Trices auf getrennten Zeilen sowie normale/`static inline` Funktionen als mögliche Anpassungen; benötigte lokale Werte müssen als Parameter übergeben werden. Der Workflow `insert/clean` bleibt dauerhaft eine Alternative zur ID-Zuweisung. Automatisches `-ce` für `insert/clean` gehört nicht zu A10; zusätzliche Werte können dort ausdrücklich im Formatstring und in den Argumenten stehen. Verweise für bereits gebundene Projekte auf den bestehenden Rückweg. Der UM-Text muss geplantes CE-Verhalten bis zur Implementierung als geplant kennzeichnen.
- Prüfe Selektoren und Aliase, Regelreihenfolge, doppelte Selektoren, Feldkonflikte, Argumentzahl, Bitbreite/Stempeltyp und Float-Wrapper, Konfigurationswechsel, `TRICE_OFF`, C-/C++-Kompilierung, Editor-Diagnosen und Decoder-Ausgabe. Ausführliche, beschreibende Tests müssen insbesondere direkte lokale CE-Ausdrücke ohne verfügbares `__COUNTER__`, einmalige Auswertung, frühe Ablehnung ausgewählter Wrapper-/Rebase-Stellen mit unveränderten Dateien und UM-Hinweis sowie weiterhin funktionierende unselektierte Bind-Stellen belegen.
- Ergänze nach den Structured-Logging-Sequenzen in `_test/testdata/triceCheck.c` Beispiele mit globalem `pos.x = -444`, `pos.y = 77` und `float velocity = 33.33f`. Verwende in den Tests unter anderem `-ce 'pos:", x={}, y={}", pos.x, pos.y'` und `-ce 'speed:", m/s=%f", aFloat(velocity)'`; prüfe die tatsächlichen übertragenen Werte und die passende Ausgabe.

**Zurückgestellte Folgeaufgabe:** CE für Wrappermakros und Counter-Rebase benötigt einen eigenen Architektur-Nachweis und Implementierungsauftrag. Er muss zeigen, dass jede Logstelle ausschließlich ihre dort gültigen zusätzlichen Ausdrücke erhält, und die unterstützten Compiler mit und ohne `__COUNTER__` ausdrücklich abgrenzen. Die direkte Ausbaustufe bleibt unabhängig davon nutzbar. Eine spätere automatische CE-Erweiterung für `insert/clean` bleibt ein separater Auftrag mit reversiblem Transformationsvertrag.

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

### A9 – Context-Enrichment-Machbarkeitsnachweis bestanden

Der [isolierte PoC](Context_Enrichment_PoC.md) weist zusätzliche lokale Werte, ursprüngliche Argumentreihenfolge, einfache Ausdrücke, einmalige Auswertung, feste und generische Arity sowie TIL-konsistente Binärrecords nach. Die User-Source bleibt bytegleich; zwei PoC-Bind-Läufe erzeugen dieselben IDs und Artefakte. Die Tests bestehen als C11 und C++17 mit den echten Trice-Headern und der Target-Bibliothek. `clangd` lädt dieselbe Compile-Konfiguration und meldet keine CE-bedingten Fehler; ein nicht sichtbarer Context-Ausdruck wird dagegen von Compiler und Language-Server abgewiesen.

Der Nachweis gilt für direkte skalare 32-Bit-Logstellen mit `iD` und einer Logstelle pro Zeile. Andere Language-Server wurden nicht geprüft. Der Adapter ist ausschließlich Testcode; die produktive Option `bind -ce` ist weiterhin nicht implementiert. A10 benötigt einen eigenen Auftrag.
