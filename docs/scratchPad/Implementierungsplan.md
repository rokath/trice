# Arbeitsplan für Structured Logging und Context Enrichment

Stand: 27. September 2026. A1 bis A10 sind erledigt. Structured Logging ist in [Kapitel 32 des User Manuals](../TriceUserManual.md#strukturiertes-logging), Context Enrichment in [Kapitel 33](../TriceUserManual.md#trice-context-enrichment) dokumentiert. `bind -ce` unterstützt direkte, eindeutig über ihre Quellzeile adressierbare Logstellen ohne zusätzliche `__COUNTER__`-Abhängigkeit. Für Wrappermakros und Counter-Rebase liegt jetzt ein [erweiterter PoC](Context_Enrichment_PoC.md#erweiterter-poc-für-wrappermakros-und-counter-rebase) vor. Eine produktive Erweiterung bleibt bis zur Entscheidung über dessen Ergebnis zurückgestellt.

## Offene Aufgaben in Arbeitsreihenfolge

### A11 – Frühe Hostfilterung gesondert prüfen

- Diese zurückgestellte Optimierung (bisher M17: frühe Hostfilterung) ist keine Voraussetzung für Structured Logging oder CE. Miss zuerst Replay mit 0, 50, 90 und 100 Prozent verworfenen Ereignissen; erfasse CPU, Allokationen und Durchsatz. Entscheide erst anhand dieser Daten über Codeänderungen.
- Framing und Integrität, Stempelzustand, Rohaufzeichnung, Statistik, Diagnosen und akzeptierte Records müssen sich durch eine Optimierung nicht ändern. Beziehe typisierte strukturierte Ausgabe und CE-Records in den Vergleich ein.

### Zurückgestellte CE-Folgeaufgaben

Der [erweiterte PoC](Context_Enrichment_PoC.md#erweiterter-poc-für-wrappermakros-und-counter-rebase) wählt je Expansion genau einen CE-Adapter im Präprozessor. Ein zusätzlicher Vorlauf mit dem konkreten Compiler liefert dafür die tatsächlichen Counter-Werte. Getrennte lokale Scopes, wiederholte Wrapper-Aufrufe, einmalige Auswertung, reale Records und die Ablehnung veralteter Zuordnungen sind nachgewiesen. Geprüft wurden Clang auf dem Host sowie ARM-GCC für Cortex-M0 und Cortex-M4 in C99/C11/C17 und C++11/C++17/C++20. Strenges C++20 scheitert schon am bisherigen Enum-Rebase-Code; die CE-Prüfung besteht nach einer gezielten Warnungsabstufung. ARM-Builds sind Compile-Nachweise, keine Target-Laufzeitabnahme.

**Nächster Schritt ist eine Entscheidung, keine automatische Implementierung:** Soll ein compilerabhängiger Vorlauf pro Übersetzungseinheit und Build-Konfiguration Teil des produktiven Workflows werden? Die [offenen Integrationspunkte](Context_Enrichment_PoC.md#konsequenzen-für-eine-mögliche-umsetzung) betreffen Build-Abhängigkeiten, getrennte Artefakte, weitere Compiler sowie PCH/Module. Ohne `__COUNTER__` unterstützt dieser Rebase-Ansatz kein aktives komplexes Logging. Ein eindeutig zuordenbarer Wrapper mit nur einer Logstelle funktioniert im PoC dagegen ohne Counter und ohne Vorlauf; diese kleinere Erweiterung kann getrennt bewertet werden.

Der PoC ändert keine produktive CE-Freigabe. Ein Implementierungsauftrag bleibt erforderlich; die direkte Ausbaustufe bleibt unabhängig davon nutzbar. Regelverarbeitung, finale Schema-/ID-Bestimmung und Argumenteinfügung sind getrennt, damit eine spätere Makroerzeugung auf dem bestehenden Vertrag und seinen Verhaltenstests aufbauen kann.

Eine spätere automatische CE-Erweiterung für `insert/clean` bleibt ein separater Auftrag mit reversiblem Transformationsvertrag. Der bestehende ID-Workflow ist dauerhaft verfügbar; zusätzliche Werte können dort ausdrücklich im Formatstring und in den Argumenten stehen. Weder eine vorsorgliche Migration noch eine zusätzliche CLI-Variante wurden in A10 eingeführt.

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

Kapitel 32 wurde mit dem aktuellen CLI-Verhalten, den strukturierten Generator- und Decoder-Tests sowie A1–A6 abgeglichen. Die weiterhin gültigen Hinweise zu Platzhaltern, Typen und C-Beispielen stehen nun im UM; ältere Entwurfsannahmen zu Tag-Schreibweise, Target-Zeitstempeln und Build-Verzeichnis wurden nicht übernommen. Die frühere Entwurfskopie ist entfernt und der UM-Link auf die nicht mehr vorhandene ScratchPad-README zeigt auf diesen Plan. Context Enrichment wurde getrennt in A10 umgesetzt.

### A8 – Zusätzliche JSON-Ansichten über `generate`

`trice generate -onelineJSON -til til.json -li li.json` erzeugt auf Abruf `til.oneline.json` und `li.oneline.json`. Beide bleiben vollständige JSON-Objekte, nicht NDJSON: Jede ID und ihr kompakter Eintrag stehen zusammen auf einer Zeile. In der LI-Ansicht steht `Line` vor `File`, beispielsweise `"13000": {"Line":163,"File":"src/main.c"}`. Der Name `oneline` bezieht sich auf den Eintrag, nicht auf die ganze Datei. Mit `-li off` wird nur die TIL-Ansicht erzeugt. Fehlende oder ungültige angeforderte Eingabedateien werden vor Ausgabeschreibzugriffen abgewiesen.

Die Originaldateien bleiben maßgeblich und werden bei diesem Export nicht verändert. Der Export wird nach Änderungen an den Originalen erneut ausgeführt; `bind`, `insert`, `clean`, `add` und die Remigration erzeugen keine automatischen Kopien. Die bestehende JSON-Datenstruktur bleibt erhalten. `encoding/json` serialisiert Schlüssel und Werte; nur äußere Klammern, Kommata und Zeilenumbrüche werden für die Ansicht zusammengesetzt. Für TIL bleibt `SetEscapeHTML(false)` maßgeblich. Tests decken stabile ID-Reihenfolge, Escapes, Unicode, Roundtrips, unveränderte Wiederholungsläufe und das Zurückrollen bei einem Schreibfehler ab. Eine externe Abhängigkeit ist nicht erforderlich; der zusätzliche Aufwand entsteht nur beim angeforderten Export auf dem Host.

### A9 – Context-Enrichment-Machbarkeitsnachweis bestanden

Der [isolierte PoC](Context_Enrichment_PoC.md) weist zusätzliche lokale Werte, ursprüngliche Argumentreihenfolge, einfache Ausdrücke, einmalige Auswertung, feste und generische Arity sowie TIL-konsistente Binärrecords nach. Die User-Source bleibt bytegleich; zwei PoC-Bind-Läufe erzeugen dieselben IDs und Artefakte. Die Tests bestehen als C11 und C++17 mit den echten Trice-Headern und der Target-Bibliothek. `clangd` lädt dieselbe Compile-Konfiguration und meldet keine CE-bedingten Fehler; ein nicht sichtbarer Context-Ausdruck wird dagegen von Compiler und Language-Server abgewiesen.

Der A9-Nachweis gilt für direkte skalare 32-Bit-Logstellen mit `iD` und einer Logstelle pro Zeile. Andere Language-Server wurden nicht geprüft. Der isolierte Adapter bleibt Testcode; die darauf aufbauende produktive Option `bind -ce` wurde mit A10 getrennt implementiert und umfassender geprüft.

### A10 – Context Enrichment implementiert und abgenommen

`trice bind -ce` setzt den [deutschen CE-Vertrag](Kontextanreicherung_DE.md) für direkte, eindeutig über ihre Quellzeile adressierbare Logstellen um. Normale und `static inline` Funktionen sowie eindeutig zuordenbare mehrzeilige Aufrufe sind eingeschlossen. Die erste Ausbaustufe benötigt kein `__COUNTER__`. Ursprüngliche Argumente behalten ihre Position; zusätzliche Ausdrücke werden am jeweiligen Aufrufort genau einmal ausgewertet. CE selbst verändert die User-Logstellen nicht. Die normalen erstmaligen Bind-Einrichtungsschritte bleiben bestehen.

Selektoren, Tag-Aliase, CLI-/Source-Reihenfolge und einmalige Anwendung doppelter Selektoren sind umgesetzt. Bekannte Tags behalten ihre normalen Darstellungsregeln. Freie kleingeschriebene Selektoren werden nur bei passender Regel entfernt. Gemeinsame Feldvalidierung, explizite `aFloat`-/`aDouble`-Wrapper, Arity-/Bitbreitenprüfung, IDs aus dem endgültigen Schema und `trice-fields.txt` verwenden den bestehenden Structured-Logging-Vertrag. Ein reiner Ausdruckswechsel bei gleichem Schema behält seine ID. Wiederholungsläufe sind bytegleich; ohne `-ce` entsteht wieder das normale Schema.

`generate -logC` liest die endgültigen CE-Metadaten aus dem Sidecar und benötigt keine erneuten Regeln. Geänderte Source-Aufrufe oder widersprüchliche Metadaten werden erkannt. Ungültige Regeln und ausgewählte Wrapper-/Rebase-Stellen scheitern vor Schreibzugriffen auf Source, Sidecars, TIL, LI und Feldregister. Schreibfehler werden über die bestehende Bind-Transaktion zurückgerollt. Nicht ausgewählte komplexe Bind-Stellen bleiben nutzbar.

Das finale deutsche [UM-Kapitel 33](../TriceUserManual.md#trice-context-enrichment) beschreibt Bedienung und Grenzen. [bind-limits](../TriceUserManual.md#bind-limits) erklärt Zeilenzuordnung, lokale Sichtbarkeit und `__COUNTER__` verständlich, zeigt getrennte Zeilen sowie Funktionen mit übergebenen Werten und nennt `insert/clean` als dauerhafte Alternative einschließlich Rückweg aus Bind. Bind-Diagnosen und der Compilerhinweis für fehlendes `__COUNTER__` enthalten `Search UM for "bind-limits".`.

Die vier Beispiele nach den Structured-Logging-Sequenzen in `_test/testdata/triceCheck.c` verwenden die globalen Werte `pos.x = -444`, `pos.y = 77` und `velocity = 33.33f`. Die Integration übernimmt diese Aufrufe und verwendet unter anderem `-ce 'pos:", x={}, y={}", pos.x, pos.y'` sowie `-ce 'speed:", m/s=%f", aFloat(velocity)'`. Sie prüft reale Records gegen die mit dem öffentlichen `generate -logC` erzeugte C-Tabelle und dekodiert sie als Text, JSON und KV.

Bestanden sind die Go-Pakete unter `cmd`, `internal` und `pkg`, der vollständige `internal/id`-Lauf mit `TRICE_BIND_INTEGRATION=1` sowie die neue CE-Target-Integration. Letztere umfasst C11/C++17 mit Clang, `clangd`, alle vier skalaren Bitbreiten, verschiedene Stempeltypen, getrennte lokale Scopes, Mehrzeiler, eine Inline-Funktion, deaktiviertes Logging mit `TRICE_OFF`/`TRICE_CLEAN` und Builds ohne `__COUNTER__`. Negative Tests prüfen echte Fehler bei nicht sichtbaren Bezeichnern. Andere Compiler/Language-Server und ein vollständiger `testAll.sh full`-Lauf sind damit nicht abgenommen.
