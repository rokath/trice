# Abgeschlossene Arbeiten an Structured Logging und Context Enrichment

Stand des übernommenen Arbeitsplans: 27. September 2026. Dieser historische Bericht bewahrt die Abschlussinformationen zu A1–A10 und dem CE-Folgeauftrag. Er ersetzt keine aktuelle Release-Abnahme; offene Befunde und weitere Entscheidungen stehen im [aktiven Implementierungsplan](../Implementierungsplan.md).

## Erledigter Stand als Reviewhilfe

### A1 – Test-Ausgangsstand und Policy-Anpassungen

Die Erwartungen an ID-Bereich, Tag-Policy und wiederholte Bind-Formate wurden korrigiert. `TestInsertExistingID_A/B` verwenden nun ausdrücklich einen Bereich, der ihre ID 77 enthält. Eine ungültige auskommentierte Trice-Zeile in `examples/G0B1_inst/Core/Inc/triceConfig.h` wurde entfernt. Die betroffenen Go-Pakete und der Clang-Insert-Integrationstest bestanden bei der A1-Prüfung. Das ist keine Aussage über einen vollständigen `testAll.sh`-Lauf; dessen Ergebnis ist gesondert zu bewerten.

### A2 – Metadatenvertrag für JSON und Key-Value

JSON und KV übernehmen aktivierte, vorhandene ID-, Orts- und Host-Zeitinformationen. Target-Stempel und Differenzen verwenden getrennte, formatierte Stringfelder `ts16`, `ts32`, `ts16Delta` und `ts32Delta`. Der erste Stempel jeder Bitbreite hat kein Delta-Feld; `ts0` und `ts0delta` erzeugen keine Metadaten. Kapitel 32 beschreibt die CLI-Konfiguration und Ausgabe.

### A3 – Tag, Meldungsinhalt und Stringwerte

`tag` wird über registrierte Aliase case-neutral kanonisiert. `message` folgt der Textdarstellung ohne äußere Dekoration und behält ihre führenden und folgenden Leerzeichen. Nur exakt registrierte, vollständig kleingeschriebene Präfixe werden bei aktiver Farbbehandlung entfernt; `-color off` erhält die Textpräfixe einschließlich des synthetischen `untagged:` für unbekannte Tags. Andere Stringwerte verlieren äußeren Leerraum. Klassische Pufferlogs bleiben feldfreie Meldungen. Laufzeitstrings ändern den Formatstring-Tag nicht; die bestehende Textumwandlung von Escape-Folgen gilt für `message`, während benannte Stringfelder den übertragenen Wert mit gekürztem äußerem Leerraum behalten. Das Verhalten und Beispiele stehen in Kapitel 32.

Der gemeinsame Template-Parser, die Kanonisierung in `til.json`, `bind`/`insert`/`clean`, typisierte Decoder-Records, `-logFormat text|json|kv` und `trice-fields.txt` sind vorhanden. Skalare Werte sowie Strings über `triceS`/`triceN` gehören zum aktuellen Scope; benannte Pufferfelder werden abgewiesen. Die Bedienung ist in [UM-Kapitel 32](../../TriceUserManual.md#strukturiertes-logging) beschrieben. Diese Bestandsaufnahme ist keine Behauptung, dass alle bestehenden Tests bestehen oder die oben genannten Details bereits dem gewünschten Vertrag entsprechen.

Die früher abgeschlossenen Arbeiten an Tag-Aliasen, Gewichten, Auswahl, ID-Policy, Rohaufzeichnung, Statistik und Diagnosen stehen in den jeweiligen UM-Kapiteln. Die alten Handovers und Aufgaben liegen unter [obsolete](README.md). Beispielvalidierung ist in A6, A7 und A10 als Abnahme enthalten und benötigt keinen eigenen Implementierungsblock.

### A4 – Ausgabeformat-Option und TREX-Geltungsbereich

`-logFormat` akzeptiert die Werte `text`, `json` und `kv` unabhängig von Groß- und Kleinschreibung sowie `key-value` als Alias für `kv`. Der Wert `json` erzeugt NDJSON mit genau einem JSON-Objekt und abschließendem LF pro akzeptiertem Ereignis; einen separaten CLI-Wert `ndjson` gibt es nicht. Die CLI-Hilfe benennt NDJSON kurz, Kapitel 32 beschreibt TREX als Voraussetzung für maschinenlesbare Ereignisse und die CLI weist CHAR/DUMP weiterhin verständlich ab. Gültige Varianten, ungültige Werte und reale Ausgabe werden getestet.

### A5 – Gemeinsames Build-Verzeichnis für das Feldregister

`bind`, `insert` und `generate` verwenden nun `-genDir` mit dem Default `./generated` relativ zum Aufrufverzeichnis. Bei `bind` liegen dort Sidecar-Header und `trice-fields.txt`, bei `insert` das Feldregister. `generate -logC` liest die Bind-Sidecars dort und schreibt ohne expliziten Dateipfad `til.c` dorthin; `-onelineJSON` und ein einfacher `-abc`-Zielname erzeugen ihre Dateien ebenfalls dort. Ein expliziter Ausgabepfad bleibt maßgeblich. Der frühere Schalter `-buildDir` sowie `-bindDir` werden abgewiesen; die Repository-Workflows und das CMake-Beispiel verwenden `-genDir`. Kapitel 32.7 beschreibt das Register als Datei mit Feldnamen und Häufigkeiten des letzten erfolgreichen Laufs. Die Tests decken benutzerdefinierte Pfade, Wiederholung, alte Schalter, `-dry-run` und die öffentlichen Bind-Workflows ab.

### A6 – Verständliche C-Beispiele und Integrationstests

Nach den `assert`-Zeilen in `_test/testdata/triceCheck.c` stehen neun kompilierbare Beispiele für 8/16/32/64-Bit-Werte, Stempelvarianten, `triceS`/`triceN`, gemischte klassische und strukturierte Platzhalter, per `.` und `->` abgeleitete Feldnamen sowie `aFloat()` und `aDouble()`. Ein gezielter PC-Integrationstest prüft ihre vollständige Textausgabe mit den bestehenden CLI-Einstellungen durch C-Zielcode und Decoder, jeweils nach `bind` und `insert`. Generator- und strukturierte Decoder-Tests bestanden ebenfalls. Die vollständigen PC-Testläufe bleiben unabhängig davon an einer älteren Erwartung hängen: Bei `-color=off` erscheint für ungetaggte Meldungen heute `untagged:`, während die erste Bestandszeile noch ohne dieses Präfix erwartet wird. Diese Bestandsabweichung gehört nicht zu den neun A6-Beispielen.

### A7 – Structured Logging dokumentarisch abgeschlossen

Kapitel 32 wurde mit dem aktuellen CLI-Verhalten, den strukturierten Generator- und Decoder-Tests sowie A1–A6 abgeglichen. Die weiterhin gültigen Hinweise zu Platzhaltern, Typen und C-Beispielen stehen nun im UM; ältere Entwurfsannahmen zu Tag-Schreibweise, Target-Zeitstempeln und Build-Verzeichnis wurden nicht übernommen. Die frühere Entwurfskopie ist entfernt und der UM-Link auf die nicht mehr vorhandene ScratchPad-README zeigt auf diesen Plan. Context Enrichment wurde getrennt in A10 umgesetzt.

### A8 – Zusätzliche JSON-Ansichten über `generate`

`trice generate -onelineJSON -til til.json -li li.json` erzeugt auf Abruf `til.oneline.json` und `li.oneline.json`. Beide bleiben vollständige JSON-Objekte, nicht NDJSON: Jede ID und ihr kompakter Eintrag stehen zusammen auf einer Zeile. In der LI-Ansicht steht `Line` vor `File`, beispielsweise `"13000": {"Line":163,"File":"src/main.c"}`. Der Name `oneline` bezieht sich auf den Eintrag, nicht auf die ganze Datei. Mit `-li off` wird nur die TIL-Ansicht erzeugt. Fehlende oder ungültige angeforderte Eingabedateien werden vor Ausgabeschreibzugriffen abgewiesen.

Die Originaldateien bleiben maßgeblich und werden bei diesem Export nicht verändert. Der Export wird nach Änderungen an den Originalen erneut ausgeführt; `bind`, `insert`, `clean`, `add` und die Remigration erzeugen keine automatischen Kopien. Die bestehende JSON-Datenstruktur bleibt erhalten. `encoding/json` serialisiert Schlüssel und Werte; nur äußere Klammern, Kommata und Zeilenumbrüche werden für die Ansicht zusammengesetzt. Für TIL bleibt `SetEscapeHTML(false)` maßgeblich. Tests decken stabile ID-Reihenfolge, Escapes, Unicode, Roundtrips, unveränderte Wiederholungsläufe und das Zurückrollen bei einem Schreibfehler ab. Eine externe Abhängigkeit ist nicht erforderlich; der zusätzliche Aufwand entsteht nur beim angeforderten Export auf dem Host.

### A9 – Context-Enrichment-Machbarkeitsnachweis bestanden

Der [isolierte PoC im UM-Anhang](../../TriceUserManual.md#anhang-ce-machbarkeitsnachweise) weist zusätzliche lokale Werte, ursprüngliche Argumentreihenfolge, einfache Ausdrücke, einmalige Auswertung, feste und generische Arity sowie TIL-konsistente Binärrecords nach. Die User-Source bleibt bytegleich; zwei PoC-Bind-Läufe erzeugen dieselben IDs und Artefakte. Die Tests bestehen als C11 und C++17 mit den echten Trice-Headern und der Target-Bibliothek. `clangd` lädt dieselbe Compile-Konfiguration und meldet keine CE-bedingten Fehler; ein nicht sichtbarer Context-Ausdruck wird dagegen von Compiler und Language-Server abgewiesen.

Der A9-Nachweis gilt für direkte skalare 32-Bit-Logstellen mit `iD` und einer Logstelle pro Zeile. Andere Language-Server wurden nicht geprüft. Der isolierte Adapter bleibt Testcode; die darauf aufbauende produktive Option `bind -ce` wurde mit A10 getrennt implementiert und umfassender geprüft.

### A10 – Context Enrichment implementiert und abgenommen

`trice bind -ce` setzt den [deutschen CE-Vertrag im UM](../../TriceUserManual.md#trice-context-enrichment) für direkte, eindeutig über ihre Quellzeile adressierbare Logstellen um. Normale und `static inline` Funktionen sowie eindeutig zuordenbare mehrzeilige Aufrufe sind eingeschlossen. Die erste Ausbaustufe benötigt kein `__COUNTER__`. Ursprüngliche Argumente behalten ihre Position; zusätzliche Ausdrücke werden am jeweiligen Aufrufort genau einmal ausgewertet. CE selbst verändert die User-Logstellen nicht. Die normalen erstmaligen Bind-Einrichtungsschritte bleiben bestehen.

Selektoren, Tag-Aliase, CLI-/Source-Reihenfolge und einmalige Anwendung doppelter Selektoren sind umgesetzt. Bekannte Tags behalten ihre normalen Darstellungsregeln. Freie kleingeschriebene Selektoren werden nur bei passender Regel entfernt. Gemeinsame Feldvalidierung, explizite `aFloat`-/`aDouble`-Wrapper, Arity-/Bitbreitenprüfung, IDs aus dem endgültigen Schema und `trice-fields.txt` verwenden den bestehenden Structured-Logging-Vertrag. Ein reiner Ausdruckswechsel bei gleichem Schema behält seine ID. Wiederholungsläufe sind bytegleich; ohne `-ce` entsteht wieder das normale Schema.

`generate -logC` liest die endgültigen CE-Metadaten aus dem Sidecar und benötigt keine erneuten Regeln. Geänderte Source-Aufrufe oder widersprüchliche Metadaten werden erkannt. Ungültige Regeln und ausgewählte Wrapper-/Rebase-Stellen scheitern vor Schreibzugriffen auf Source, Sidecars, TIL, LI und Feldregister. Schreibfehler werden über die bestehende Bind-Transaktion zurückgerollt. Nicht ausgewählte komplexe Bind-Stellen bleiben nutzbar.

Das finale deutsche [UM-Kapitel 33](../../TriceUserManual.md#trice-context-enrichment) beschreibt Bedienung und Grenzen. [bind-limits](../../TriceUserManual.md#bind-limits) erklärt Zeilenzuordnung, lokale Sichtbarkeit und `__COUNTER__` verständlich, zeigt getrennte Zeilen sowie Funktionen mit übergebenen Werten und nennt `insert/clean` als dauerhafte Alternative einschließlich Rückweg aus Bind. Bind-Diagnosen und der Compilerhinweis für fehlendes `__COUNTER__` enthalten `Search UM for "bind-limits".`.

Die vier Beispiele nach den Structured-Logging-Sequenzen in `_test/testdata/triceCheck.c` verwenden die globalen Werte `pos.x = -444`, `pos.y = 77` und `velocity = 33.33f`. Die Integration übernimmt diese Aufrufe und verwendet unter anderem `-ce 'pos:", x={}, y={}", pos.x, pos.y'` sowie `-ce 'speed:", m/s=%f", aFloat(velocity)'`. Sie prüft reale Records gegen die mit dem öffentlichen `generate -logC` erzeugte C-Tabelle und dekodiert sie als Text, JSON und KV.

Bestanden sind die Go-Pakete unter `cmd`, `internal` und `pkg`, der vollständige `internal/id`-Lauf mit `TRICE_BIND_INTEGRATION=1` sowie die neue CE-Target-Integration. Letztere umfasst C11/C++17 mit Clang, `clangd`, alle vier skalaren Bitbreiten, verschiedene Stempeltypen, getrennte lokale Scopes, Mehrzeiler, eine Inline-Funktion, deaktiviertes Logging mit `TRICE_OFF`/`TRICE_CLEAN` und Builds ohne `__COUNTER__`. Negative Tests prüfen echte Fehler bei nicht sichtbaren Bezeichnern. Andere Compiler/Language-Server und ein vollständiger `testAll.sh full`-Lauf sind damit nicht abgenommen.

### CE-Folgeauftrag – Reversibles insert und clean

`insert -ce` hängt die vollständige Regelgruppe nur an, wenn Format- und Argumentsuffix am Ende des ausgewählten Aufrufs nicht vollständig passen. Teilmatches zählen als nicht vorhanden. `clean -ce` entfernt eine vollständig passende Gruppe unabhängig von ihrer Herkunft und ignoriert Teilmatches. Herkunftskommentare und zusätzliche CE-Metadatendateien sind nicht erforderlich; freie kleingeschriebene Selektoren bleiben im Source und fehlen im endgültigen TIL-Template. Ungültige Regeln und Schema-Konflikte scheitern vor Veröffentlichung. Die Transaktion umfasst Source, TIL, LI und Insert-Feldregister; Schreibfehler rollen bereits veröffentlichte Änderungen zurück.

Statische Wrapperdefinitionen und mehrere erkannte Aufrufe pro Zeile benötigen bei Insert keine Bind-Zuordnung und keinen `__COUNTER__`. Lokale Ausdrücke müssen an jeder tatsächlichen Expansion gültig sein; der normale Compiler meldet fehlende Bezeichner. Ein einfaches CLI bleibt Vorrang vor allgemeiner compilerabhängiger Instrumentierung. Die Bind-Grenze bleibt bestehen.

Die [Verhaltenstests](../../../internal/id/contextSource_test.go) prüfen unter anderem vollständige und teilweise Matches, die Endposition von Format und Argumenten, Regelgruppen, Newlines, Wiederholungen, handgeschriebene Felder, verschobene Logstellen, Kommentare, Ausschlüsse, Dry-Run und Schreibfehler. Die [öffentliche Target-Integration](../../../internal/args/context_enrichment_test.go) prüft Insert, `generate -logC`, echte C11-/C++17-Records, Text/JSON/KV, einmalige Auswertung, Abschaltung und anschließendes Clean. Ein Wrapper enthält dabei zwei Logstellen in getrennten lokalen Blöcken derselben Sourcezeile. Das deutsche UM-Kapitel erklärt Syntax und Nutzen zuerst, anschließend die Workflows und Grenzen mit Beispielen. Die bisherigen PoC-Tests sind unverändert erhalten; ihre vollständige Dokumentation ist jetzt [kapitelinterner Anhang](../../TriceUserManual.md#anhang-ce-machbarkeitsnachweise).
