# Arbeitsplan für Structured Logging und Context Enrichment

Stand: 25. September 2026. Dieser Plan ordnet die noch offenen Arbeiten. Die Bedienungsdokumentation für Structured Logging steht in [Kapitel 32 des User Manuals](../TriceUserManual.md#strukturiertes-logging); bis zur dortigen Übernahme fehlender Inhalte bleibt der [deutsche Entwurf](Strukturiertes_Logging_DE.md) erhalten. [Context Enrichment](Kontextanreicherung_DE.md) ist zurückgestellt. Die Reihenfolge unten ist verbindlich: erst Structured Logging abschließen, dann den CE-Machbarkeitsnachweis, danach CE implementieren. Die hier beschriebenen Arbeiten an Code, Tests und UM sind mit diesem reinen ScratchPad-Dokumentationsschritt noch nicht ausgeführt.

## Offene Aufgaben in Arbeitsreihenfolge

### A1 – Test-Ausgangsstand klären und Fehler beheben

- Erfasse die aktuell fehlschlagenden relevanten Tests mit ihren konkreten Ursachen. Trenne überholte Erwartungen nach der geänderten ID- und Tag-Policy von tatsächlichen Produktfehlern; behaupte keinen grünen Gesamtlauf aufgrund einzelner erfolgreicher Tests. Insbesondere sind die Erwartungen zu ID 77 außerhalb 100..999 in `TestInsertExistingID_A/B` und die Bind-Tests für wiederholte Formate zu prüfen.
- Passe Tests an die vereinbarte Policy an und behebe echte Fehler. Ergänze verständliche Verhaltenstests für korrigierte Erfolgs-, Ablehnungs- und Grenzfälle. Führe zuerst die betroffenen Pakete und die einschlägigen C-Integrationstests aus; das lange `testAll.sh` nur gezielt, wenn die restlichen Risiken es erfordern.
- Abnahme: relevante Tests bestehen oder verbleibende, unabhängig bestätigte Fehler sind mit konkretem Testnamen und Ursache dokumentiert. Diese Klärung hat Vorrang vor weiteren Structured-Logging-Erweiterungen.

### A2 – Metadatenvertrag für JSON und Key-Value festlegen und umsetzen

- Erhalte die Kontrolle des Users durch die bestehenden CLI-Optionen: `id`, `file`, `line`, `hs` und Target-Zeitstempel nur ausgeben, wenn sie aktiviert und verfügbar sind. `hs` ist der nach CLI-Vorgabe formatierte Host-Zeitstring. Textpräfix und -suffix bleiben außerhalb der maschinenlesbaren Records. Entferne aus Kapitel 32 die Aussage, die Beispiele setzten pauschal deaktivierte Host-Stempel und sonst keine Metadaten voraus; zeige stattdessen die jeweilige Beispielkonfiguration ausdrücklich.
- Verwende für die vier unabhängigen Zeitstempelarten exakt die JSON-/KV-Schlüssel `ts16`, `ts32`, `ts16Delta` und `ts32Delta`. Jeder vorhandene und aktivierte Wert ist ein String gemäß seiner eigenen CLI-Formatierung: ohne Stempel-Tag, mit konfiguriertem Zusatztext und ohne äußere Leerzeichen. Die Werte dürfen nicht in einem gemeinsamen `ts`- oder `tsDelta`-Feld zusammengefasst werden; zum Beispiel kann `ts32` eine Zeit und `ts16` eine Temperatur bedeuten. `ts0` und `ts0delta` erzeugen kein Metadatenfeld. Sichere Einzelstempel, Differenzen, Formatierung, gleichzeitig konfigurierte Optionen und deaktivierte Fälle mit Tests ab.
- Beim ersten Ereignis ohne vorherigen Vergleichswert fehlt das jeweilige Delta-Feld vollständig. Sobald ein Vergleichswert vorliegt, wird nur das zur jeweiligen Stempelart gehörende Delta-Feld gemäß deren CLI-Formatierung ausgegeben. Teste das erste und das folgende Ereignis für 16- und 32-Bit-Stempel getrennt.

### A3 – Bedeutung von `tag`, `message` und Stringwerten korrigieren

- Definiere `tag` in JSON/KV als kanonische Form des erkannten Formatstring-Tags, unabhängig von dessen Schreibweise oder Alias: `trice("inf:Hi")` und `trice("Inf:Hi")` sollen beide `tag=INFO` und `message=Hi` ergeben. `message` soll dem sichtbaren Text-Meldungsinhalt ohne Farbe entsprechen. Prüfe dabei die Behandlung bisheriger Textpräfixe und unbekannter Tags ausdrücklich. Ein unbekannter Präfix wie `mgs:` bleibt als Text sichtbar und erhält `untagged`.
- Klassische Pufferlogs ohne benannte Felder bleiben als `message` verfügbar; sie erzeugen keine strukturierten User-Werte. Ergänze ein konkretes Beispiel in Kapitel 32 und einen Verhaltenstest.
- Entferne bei **allen Stringwerten** in JSON/KV führende und folgende Leerzeichen, also auch bei `message`, `hs` und benannten Stringfeldern. Decke leere Werte, Unicode und nur aus Leerzeichen bestehende Werte ab. Die Textausgabe muss gesondert gegen ihre bisherige Semantik geprüft werden.
- Erläutere mit einem Beispiel, dass ein von `triceS`/`triceN` gelieferter Laufzeitstring wie `err:\n` oder `err:\t` weder erneut als C-Escape noch als Formatstring-Tag interpretiert wird; andernfalls streiche die derzeitige, ohne Kontext schwer verständliche Aussage aus dem UM. Teste diesen Unterschied zwischen Formatstring und Laufzeitwert.

### A4 – Ausgabeformat-Option und TREX-Geltungsbereich bereinigen

- Mache die Werte von `-logFormat` ohne Rücksicht auf Groß-/Kleinschreibung wählbar. Erlaube zusätzlich `key-value` als verständlichen Alias für `kv`; der bestehende Wert `kv` bleibt gültig. Teste gültige Varianten und ungültige Werte.
- Bezeichne `-logFormat json` im UM präzise als NDJSON: genau ein JSON-Objekt mit abschließendem LF pro akzeptiertem Ereignis. **Vor einer Umbenennung der CLI entscheiden**, ob `json` als Wert genügt oder zusätzlich `ndjson` angeboten werden soll. Die vorhandene CLI-Verwendung darf nicht stillschweigend gebrochen werden.
- Stelle nur einmal am Anfang des Structured-Logging-Kapitels klar, dass maschinenlesbare Ausgabe das standardmäßige TREX-Drahtformat voraussetzt und nicht mit CHAR/DUMP arbeitet. Entferne die wiederholten TREX-Hinweise im Kapitel und den Satz „Structured formats require TREX“ aus der `-logFormat`-Hilfe. Prüfe, dass die CLI ungeeignete Eingaben weiterhin verständlich ablehnt.

### A5 – Gemeinsames Build-Verzeichnis für das Feldregister

- Prüfe die vorhandenen Optionen `-bindDir` und `-buildDir`. Führe für `bind` und `insert` eine gemeinsame, verständliche `-buildDir`-Bedienung für `trice-fields.txt` ein und aktualisiere Kapitel 32.7 sowie die CLI-Hilfe. **Vor der Änderung entscheiden**, ob `-bindDir` als Alias bestehen bleibt und wie abweichende gleichzeitige Angaben behandelt werden.
- Erkläre in Kapitel 32.7 „Register“ als die Datei `trice-fields.txt` mit den Feldnamen und Häufigkeiten des letzten erfolgreichen Laufs. `-dry-run` darf diese Datei weder überschreiben noch neu veröffentlichen. Teste Pfadwahl, wiederholte Läufe und `dry-run`.

### A6 – Verständliche C-Beispiele und Integrationstests ergänzen

- Ergänze in `_test/testdata/triceCheck.c` nach den `assert`-Zeilen einen repräsentativen, kompilierbaren Satz strukturierter Trices. Zeige 8/16/32/64 Bit, Stempelvarianten, `triceS` und `triceN`, gemischte `%d`- und `{}`-Platzhalter, `.` und `->` sowie `aFloat()` und `aDouble()`. Vermeide eine vollständige Kreuzprodukt-Testmatrix.
- Verwende für die erwartete Textausgabe die CLI-Einstellungen der bestehenden Testläufe. Führe die betroffenen Generator-, C- und Decoder-Tests zusammen aus und prüfe die Beispiele außerdem als nachvollziehbare Anleitung für User.

### A7 – Structured Logging dokumentarisch abschließen

- Gleiche Kapitel 32 mit dem aktuellen Verhalten und den Ergebnissen aus A1–A6 ab. Übernimm alle noch gültigen, fehlenden Inhalte des [deutschen Entwurfs](Strukturiertes_Logging_DE.md) in das UM, ohne widersprüchliche ältere Entwurfsangaben zu übernehmen. Prüfe besonders Tag-Kanonisierung, NDJSON, Metadaten, Pufferlogs, Strings und Feldregister.
- Entferne danach die Entwurfskopie `Strukturiertes_Logging_DE.md`. Stelle alle aktiven Links darauf auf das UM um. Der UM-Link auf die bisherige ScratchPad-README ist beim späteren UM-Schritt auf diesen Plan umzusetzen. Am Ende sollen hier nur `Kontextanreicherung_DE.md`, `Implementierungsplan.md` und die unveränderte `scratchPad.md` als aktive Texte verbleiben.
- Abnahme: kein widersprüchlicher Vertrag zwischen UM, Hilfe, Code und Tests; Beispiele gegen die reale CLI geprüft. CE bleibt bis dahin zurückgestellt.

### A8 – Einzeilige Einträge in `til.json` und `li.json` nur untersuchen

- Entscheide anhand der Vorprüfung am Ende dieses Plans, ob eine kleine eigene Zusammensetzung aus per Standardbibliothek serialisierten Einträgen noch unter die gewünschte Bedingung „Standard-Lib-Konverter“ fällt. Wenn ausschließlich eine eingebaute Formatoption erlaubt ist, entfällt die Formatänderung.
- Falls später beauftragt: Neue Dateien sollen einen Eintrag pro Zeile erhalten, vorhandene Legacy-Dateien ihre bisherige Darstellung behalten. Prüfe dafür Stilerkennung, stabile Reihenfolge, `Line` vor `File` und Roundtrips. **Jetzt keine Dateiformatänderung.**

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

Der gemeinsame Template-Parser, die Kanonisierung in `til.json`, `bind`/`insert`/`clean`, typisierte Decoder-Records, `-logFormat text|json|kv` und `trice-fields.txt` sind vorhanden. Skalare Werte sowie Strings über `triceS`/`triceN` gehören zum aktuellen Scope; benannte Pufferfelder werden abgewiesen. Die Bedienung ist in [UM-Kapitel 32](../TriceUserManual.md#strukturiertes-logging) beschrieben. Diese Bestandsaufnahme ist keine Behauptung, dass alle bestehenden Tests bestehen oder die oben genannten Details bereits dem gewünschten Vertrag entsprechen.

Die früher abgeschlossenen Arbeiten an Tag-Aliasen, Gewichten, Auswahl, ID-Policy, Rohaufzeichnung, Statistik und Diagnosen stehen in den jeweiligen UM-Kapiteln. Die alten Handovers und Aufgaben liegen unter [obsolete](obsolete/README.md). Beispielvalidierung ist in A6, A7 und A10 als Abnahme enthalten und benötigt keinen eigenen Implementierungsblock.

Die Vorprüfung zum JSON-Layout ist erfolgt: Die aktuellen Serializer in `internal/id/manage.go` verwenden `json.Encoder.SetIndent` bzw. `json.MarshalIndent`; die Standardbibliothek bietet dafür keine Option „äußerer Eintrag auf eigener Zeile, inneres Objekt kompakt“. `json.Marshal` pro Eintrag plus eigenes Zusammensetzen der äußeren Datei wäre möglich, aber mehr als eine reine Standard-Converter-Einstellung. `json.Unmarshal` liest beide Layouts unabhängig von Leerraum und Feldreihenfolge. Für `Line` vor `File` wäre eine gezielte Ausgabeform nötig, da `TriceLI` derzeit `File` vor `Line` deklariert. Die heutigen `toFile`-Pfade schreiben vorhandene Dateien bei Aktualisierung neu; die gewünschte Erhaltung ihres Layouts benötigte daher zusätzliche Stilerkennung.
