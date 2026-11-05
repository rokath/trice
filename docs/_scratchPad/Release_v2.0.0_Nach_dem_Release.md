# Trice v2.0.0: Dokumentationsabschluss und Folgearbeiten

## Zuerst erledigen

1. Die endgültige Freigabe aus [Vor dem Release](Release_v2.0.0_Vor_dem_Release.md) bleibt Voraussetzung für die Veröffentlichung.
2. **N06 und der Markdown-Teil von N02 fließen bereits in beta-4 ein:** kurze Einstiege, konkrete Ein-/Ausgaben und die heutige Host-Architektur. Die folgende Tabelle zeigt den Umfang.
3. **N01, N02 und N05 sind umgesetzt.** Als Nächstes N03 bearbeiten: Tests an tatsächlichen Fehlergrenzen ergänzen.
4. **N04 entfällt nach Nutzerentscheidung.** Die fünf eigenen `third_party`-READMEs bleiben von der automatischen Linkprüfung ausgenommen.
5. **N07 ist im RM gesichert:** Zukunftsideen bleiben auffindbar, ohne neue Funktionen zuzusagen.

Stand: 2026-11-05. Ausgangspunkt war das Review vom 2026-11-03 auf `wip`, HEAD `08b31e2ca033aa428e4d06e7bd41231ef2075b0d`. Im Dokumentationsschritt für beta-4 blieben Code, Tests und Prüfskripte unverändert. Anschließend wurden N01 mit Code- und Teständerungen, N02 mit Kommentarkorrekturen und N05 mit gezielten Skriptumbenennungen umgesetzt. N04 entfällt nach Nutzerentscheidung; die Abschnitte unten halten Ergebnisse und Prüfungen fest.

Punkt                          | Dokumentationsstand für beta-4                                                                                                                                                                                | Was offen bleibt
------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------
N06: Einstieg und Orientierung | README, Guide, Quickstarts und Beispielübersicht führen direkt zur passenden Aufgabe. Bind zeigt einen konkreten Aufruf statt eines Platzhalters; ältere IDE-/Insert-Anleitungen sind als solche eingeordnet. | Historische Hardware-Rezepte sind dadurch nicht neu validiert.
N06: Werte und Ausgabe         | User Manual und Referenz zeigen zwei CSV-Spalten samt Dateiinhalt, Umrechnung und Grenzen. Strukturierte Ausgabe und CE beginnen mit Eingabe, Befehl und Ergebnis.                                            | Die technischen Detailregeln bleiben in den Fachkapiteln und vorhandenen Anhängen.
N02: Host-Architektur          | [Aktueller Datenfluss](../rm/target_test_internals.md#host-logging-pipeline) mit Besitz, Lebensdauer, Locks, Diagnosen und Fehlerweitergabe; die historische Parserbeschreibung verweist darauf.              | Erledigt; siehe N02.
N04: Link-Prüfumfang           | Relative Links der eigenen Wrapper-READMEs wurden mit den bearbeiteten Dokumenten lokal geprüft.                                                                                                              | Zusätzliche automatische Prüfung entfällt nach Nutzerentscheidung.
N01: ungenutzter Code          | Go-Altzweige und auskommentierter Receiver-Code entfernt; Tests prüfen die aktiven Wege.                                                                                                                      | Öffentliche C-Deklaration geprüft und bewusst beibehalten; siehe N01.
N03: Verhaltenstests           | Einzelne Regressionstests wurden mit N01 und den EPUB-/Release-Korrekturen ergänzt.                                                                                                                           | Weiterer gezielter Testausbau.
N05: Skriptnamen               | Sieben interne Helfer folgen jetzt dem Muster `_NNN_name.ext`; Aufrufer, Tests und aktive Dokumentation verwenden die neuen Namen.                                                                            | Erledigt; siehe N05.

Die Beispiele wurden mit den vorhandenen Quellen und Tests abgeglichen. Dabei wurde auch die Dateibehandlung von `-vis` klargestellt: Jeder Lauf ersetzt eine vorhandene Ausgabedatei. Die bestehende LabPlot-Animation wird wiederverwendet; eine kleine Tabelle erklärt ihre Spaltenzuordnung. Das ist kein neuer Lauf der Programme oder der Hardware-Beispiele; die finale Testausführung bleibt beim Benutzer.

## Ungenutzten internen Code entfernen

**N01 · umgesetzt am 2026-11-05 · öffentliche C-Deklaration bewusst beibehalten**

Die Go-Kandidaten wurden gegen alle aktiven Quellreferenzen geprüft. Entfernt sind die ungenutzten Helfer und ihre ausschließlich alten Tests. Der heutige CLI-Pfad `add` verwendet `SubCmdIdAdd` und `zeroTriceIDs`; seine Implementierung blieb unverändert.

Bereich                                                 | Ergebnis
------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
[Printf-Ausdrücke](../../internal/printf/expression.go) | `CompileExpression` entfernt; der aktive Parser verwendet weiterhin `compileInContext`.
Alter `add`-/Quellparser                                | `SubCmdAddToList` und seine ungenutzte Aufrufkette einschließlich `triceParse` entfernt. `list.go` und die ausschließlich alten Parsertests in `update_additional_test.go` entfallen.
[Parameterparser](../../internal/id/helper.go)          | Den Zählwrapper entfernt; Tests prüfen direkt die getrennten Argumente und Fehler von `splitTriceParametersUntilClosingBracket`.
[Decoder](../../internal/decoder/decoder.go)            | Die ungenutzten Kopien von `LocationInformation` und `CorrectWrappedTimestamp` entfernt; die verwendeten Translator-Funktionen bleiben erhalten.
[Zeitstempel](../../internal/translator/translator.go)  | `targetStampWidth` und `formatTargetDeltaValue` entfernt. Die Formatprüfungen laufen über den aktiven Formatter einschließlich Epoch-Grenzfällen.
[Receiver](../../internal/receiver/receiver.go)         | Den vollständig auskommentierten alten Receive-Loop entfernt. Weitere Kommentare anschließend mit N02 aktualisiert.
[Öffentlicher C-Header](../../src/triceRx.h)            | `fn_TriceDispatchAbc` geprüft und bewusst beibehalten: intern ohne Nutzer, aber eine öffentliche Deklaration. Externe Nutzung ist durch die lokale Suche nicht ausgeschlossen.

`list.go` wurde erst nach Prüfung aller Aufrufer vollständig entfernt: Auch Adapter und Traversierung gehörten ausschließlich zum alten Zweig. Gemeinsam genutzte Matcher, Quellfilter und Formatprüfungen in `Update.go`, `vars.go` und `helper.go` bleiben erhalten. Neue Tests am aktuellen `add`-Einstieg prüfen die ID-Schreibweisen, rekursive Quellenauswahl, unveränderte Quelldateien und die Erhaltung bestehender TIL-/LI-Einträge. Der Markertest prüft jetzt ebenfalls diesen Einstieg samt Zeilennummern.

**Prüfergebnis:** gezielte Go-Tests in `internal/id`, `internal/printf`, `internal/decoder`, `internal/translator`, `internal/receiver` und `internal/args` erfolgreich (`-count=1 -timeout=2m`). Die Auswahl umfasste die neuen Add-/Parameterfälle sowie bestehende Add-, Marker-, Stamp-, Orts-, Translator-Loop- und Receiver-Wrapper-Tests. Die Nachsuche fand keine verbliebenen Go-Referenzen auf die entfernten Symbole. Keine Gesamtsuite, Coverage, C-Compiler-Matrix oder Hardwaretests ausgeführt.

Registrierte Decoder, Plattformcode, Callbacks und öffentliche `pkg`-Hilfen wurden nicht pauschal bereinigt. [internal/id/_nondeterministic_insert_test.go](../../internal/id/_nondeterministic_insert_test.go) bleibt als ausdrücklich deaktivierter historischer Test unverändert; die frühere Anforderung wird nicht wieder eingeführt.

## Die Host-Architektur an einer Stelle aktuell erklären

**N02 · umgesetzt am 2026-11-05 · Dokumentation und Quellkommentare abgeglichen**

Der [Host-Datenfluss](../rm/target_test_internals.md#host-logging-pipeline) erklärt den aktiven Weg von Transport und Decoder über Records und Filter bis zur Ausgabe. Besitz, Lebensdauer, Dictionary-Locks, Diagnosen und Fehlerweitergabe sowie der Anhang [Adding a host decoder](../rm/target_test_internals.md#adding-a-host-decoder) sind dort verknüpft. Die historische Parserbeschreibung in [Project Resources](../rm/project_resources.md#improving-the-trice-tool-internal-parser-not-planned-right-now) verweist darauf und bleibt ausdrücklich ein nicht geplanter Umbauvorschlag.

Die verbleibenden Quellkommentare sind aktualisiert:

- [Receiver](../../internal/receiver/receiver.go): heutiger `io.ReadWriteCloser`-Pfad statt alter Kanäle und Funktionsnamen; Eigentümer von Quellen, Capture-Dateien und Diagnose-Writern; konkrete Hex-/Dezimalbeispiele und korrigierte Transportangaben.
- [Translator](../../internal/translator/translator.go): tatsächliche Fehler- und EOF-Rückgabe, Diagnoseziele, Signalbehandlung sowie die Verarbeitung geliehener Puffer und geschützter LI-Daten. Die bisherige Behauptung, der Loop kehre nicht zurück, ist entfernt.
- [Decoder-Schnittstellen](../../internal/decoder/decoder.go) und [TREX](../../internal/trexDecoder/trexDecoder.go): gemeinsam verwendete Locks, Pufferbesitz, `SetInput` ohne Zustandsreset, Records unabhängig von Newlines sowie unterschiedliche Behandlung unvollständiger und beschädigter Eingaben. Der TIL-Eintrag wird unter Lock kopiert; seine unveränderlichen Strings bleiben nach Unlock nutzbar.

**Prüfung:** Aussagen mit den aktiven Implementierungen abgeglichen. Der Quelltextvergleich ohne Kommentare bestätigt unveränderten ausführbaren Code in allen vier Go-Dateien. UTF-8 und Zeilenenden bleiben erhalten; Formatierung und Diff sind geprüft. Keine Verhaltenstests, Gesamtsuite oder Compiler-Matrix ausgeführt.

## Verhaltenstests an Grenzen statt nur mehr Testmenge

**N03 · mittlere Priorität · Ausbau nach den konkreten Regressionen**

Die vorhandene Testbasis ist umfangreich: Go-Tests, zahlreiche getrennt kompilierte CGO-Konfigurationen, CLI-/Skriptfixtures, native Plattformtests und Release-Smokes. Pauschal „mehr Coverage“ wäre keine hilfreiche Aufgabe. Die vor dem Release gefundenen Fehler zeigen konkrete Muster:

- **Öffentliche Erzeugung bis zum Ende testen.** Ein Factory-Test darf fehlende private Felder nicht selbst reparieren. Erzeugung, normale Nutzung, Fehler und Close gehören in denselben beobachtbaren Ablauf.
- **Asynchrone Tests müssen ihre Arbeit beenden.** EOF, Abbruch, geschlossene Writer und der Abschluss gestarteter Goroutinen gehören zur Erwartung. Die vorhandene Race-Auswahl in CI ersetzt keine gezielten Lebenszyklustests für `keybcmd`.
- **String- und Puffergrenzen variieren.** Formatkonversion am Stringende, unmittelbar vor einem Literal und vor Newline; abgeschnittene Records; gerade ausreichende und zu kleine Ausgabepuffer. Das `%t`-Problem ist ein konkretes Beispiel.
- **Produktive Ausgabewege prüfen.** Helper-Tests für alte Stamp-/Location-Funktionen helfen wenig, wenn die CLI einen anderen Pfad verwendet. Ergänzungen sollten Text/JSON/KV, Diagnostiktrennung und Fehlerverhalten über den benutzten Einstieg prüfen.

Als spätere Ergänzung eignen sich begrenzte Go-Fuzz-Tests für Format-/Expression-Parser und fragmentierte Decoder-Eingaben. Im aktiven Go-Bestand wurde kein `func Fuzz...`-Einstieg gefunden. [Running the Tests](../rm/target_tests.md) enthält bereits weiterführende Testideen; diese nicht als paralleles zweites Programm neu erfinden.

**Geeignete Invarianten:** kein Panic bei beliebigen Eingabebytes, kein Lesen außerhalb des verfügbaren Records, nachvollziehbare Fehlerrückgabe, keine Teilausgabe als vermeintlich gültiger strukturierter Record und keine beschädigten Quelldateien nach abgebrochenem ID-Lauf. Korrekte Ziel- und Formatbeschränkungen dürfen weiterhin ablehnen. Ein Fuzzer ersetzt keine spezifischen Erfolgserwartungen.

**Erledigt, wenn:** wenige nachvollziehbare Grenztests reale öffentliche Pfade schützen und reproduzierbare Fehler minimierte Regressionen bekommen. Keine neue Fuzz-Infrastruktur oder langen Kampagnen für v2.0.0 voraussetzen.

## Selbst gepflegte Dokumentation vollständig in Prüfungen erfassen

**N04 · entfällt nach Nutzerentscheidung vom 2026-11-05**

Die bekannten veralteten Links in den fünf eigenen Wrapper-READMEs unter `third_party` sind korrigiert. Auf eine zusätzliche automatische Prüfung dieser Dateien wird verzichtet. Der Ausschluss von `third_party` in [lychee.toml](../../lychee.toml) bleibt bestehen; künftige kaputte Links in diesen READMEs werden damit nicht automatisch erkannt.

## Skriptumbenennungen klein und getrennt halten

**N05 · umgesetzt am 2026-11-05 · interne Helfer einheitlich benannt**

Interne Shell-, Node.js- und Python-Helfer folgen dem Muster `_NNN_name.ext`. Der Unterstrich kennzeichnet den internen Helfer; die Nummer dient der Sortierung und bestimmt keine Ausführungsreihenfolge. Öffentlich dokumentierte Einstiege behalten ihre Namen, auch wenn CI sie ebenfalls aufruft.

Umbenannt sind:

Bisher                                  | Jetzt
--------------------------------------- | -------------------------------------------
`examples/buildCLI.sh`                  | `examples/_010_build_cli.sh`
`examples/prepareTriceBind.sh`          | `examples/_020_prepare_trice_bind.sh`
`scripts/_test_npm_tools.cjs`           | `scripts/_118_test_npm_tools.cjs`
`scripts/_prepare_reference_manual.cjs` | `scripts/_315_prepare_reference_manual.cjs`
`scripts/_prepare_pages.py`             | `scripts/_340_prepare_pages.py`
`scripts/_github_downloads.py`          | `scripts/_350_github_downloads.py`
`scripts/_verify_release_features.cjs`  | `scripts/_615_verify_release_features.cjs`

Die Renderer-Konfiguration `scripts/_manual_pdf_config.cjs` behält ihren Namen. Weitere kosmetische Umbenennungen von `scripts/testAll.sh`, `scripts/buildTriceTool.sh`, dem Bind-Skript im Root und dem PDF-Einstieg entfallen. Beispiel-Einstiege wie `build.sh` und `show_json.sh` bleiben ebenfalls erhalten.

Beispiel-Builds, Tool-Vorbereitung, Snapshot-Prüfung, GitHub-Workflows, Testfixtures und aktive Dokumentation verwenden die neuen Namen. Die bestehenden PDF-/EPUB-Hooks bleiben die öffentlichen Aufrufwege. Es gibt keine zusätzlichen Kompatibilitätswrapper; die Skriptlogik bleibt unverändert.

**Prüfung:** gezielte Go-Tests in `scripts` und `scripts/testcheck` erfolgreich (`-count=1 -timeout=6m`), darunter Beispiel-CLI und Bind-Vorbereitung, npm-/Browser-Auswahl, echte Handbuch-Vorbereitung, PDF-/EPUB-Wrapper, Pages, Downloadberichte und Release-Prüfung. Die betroffene Shell-Auswahl besteht auch mit macOS-Bash 3.2. Syntax, ShellCheck, shfmt, gofmt, Actionlint und lokale Dokumentlinks sind geprüft; der Inhaltsvergleich zeigt ausschließlich die vereinbarten Namens-/Pfadersetzungen. Ausgeführt auf macOS arm64 in einer isolierten Kopie. Keine Gesamtsuite, realen Compiler-/PDF-/EPUB-Builds oder nativen Linux-/Windows-Läufe; die finale Gesamtabnahme bleibt beim Benutzer.

## Dokumentation mit konkreten Anwendungsbeispielen verständlich machen

**N06 · mittlere Priorität · systematische Überarbeitung der aktiven Dokumentation**

**Auftrag vom 2026-11-04:** die gesamte selbst gepflegte aktive Dokumentation auf Verständlichkeit für menschliche Leser prüfen. Technische Korrektheit und Klarheit sind gleich wichtig. Release Notes, Einstieg und Migration zuerst bearbeiten: tatsächliche Änderungen, nötige Anpassungen und sichtbare Ergebnisse erklären. Danach folgen die übrigen Referenzkapitel und Beispielanleitungen. Die bereits festgelegten Grundsätze stehen in der Referenz zur Release-Dokumentation unten. Archivierte und fremde Texte bleiben ausgenommen.

**Maßstab:** Leser sollen erkennen, wann sie eine Funktion brauchen, was sie tun müssen und welches Ergebnis sie erhalten. Abstrakte Fachbegriffe und Regeln vermeiden oder unmittelbar durch ein kleines Anwendungsbeispiel erklären. Beispiel: Für `MyWrn("err:temperature=%u\n", 85)` schreibt `-vis='wrn:printf("%d\n",v0)@warnings.csv'` die Zeile `85` in `warnings.csv`; `v0` ist der erste Messwert. Das zeigt die Wirkung deutlicher als „registered selectors select event tags“.

**Vorgehen:** pro erklärungsbedürftiger Regel einen realistischen Aufruf oder eine Eingabe und das erwartete Ergebnis zeigen. Nur wenn eine Grenze sonst unklar bleibt, einen Gegenfall ergänzen. Begriffe beim ersten Gebrauch kurz erklären. Vage Absätze durch Beispiele ersetzen, unnötige Sätze streichen und längere Details verlinken; nicht zu jeder bisherigen Erklärung zusätzliche Beispiele anhängen. Auch die To-do-Papiere selbst nach diesem Maßstab prüfen.

User Manual, Quickstarts und Referenz erfüllen verschiedene Aufgaben. Kurze Wiederholungen von Voraussetzung, Beispiel und erwartetem Ergebnis sind dort sinnvoll. Die neuen Vergleichstabellen zu Bind, CE und Compilergrenzen sollten erhalten bleiben.

**Dabei Wiederholungen und historischen Kontext prüfen:**

- Die verbindlichen Upgrade-Regeln in [ID Management](../rm/id_management.md) belassen. README, Release Notes und Einführungen mit kurzen Hinweisen dorthin verlinken, statt jeweils vollständige Migrationslisten zu führen.
- Längere Entscheidungsbegründungen in den vorhandenen thematischen Anhängen von [Bind](../rm/bind.md) und [CE](../rm/context_enrichment.md) halten. Bei künftigen Ergänzungen zuerst prüfen, ob bereits eine Tabelle dieselbe Grenze beschreibt.
- Die historischen Integrationsrezepte in [Platform Integration Recipes](../rm/integration_notes.md) weiterhin klar als solche kennzeichnen. Die dort bereits genannte fehlende heutige Bind-Validierung ist keine neue erfolgreiche Abnahme. Aktualisierung nur zusammen mit einer tatsächlich nachvollzogenen Beispielintegration.
- Die historische Parserbeschreibung aus [Project Resources](../rm/project_resources.md) ist durch N02 mit der aktuellen Architektur verknüpft. Die ausdrückliche Kennzeichnung als nicht geplanter Umbau erhalten.

Zusätzliche Referenzlinks lohnen sich vor allem bei Compiler-Sonderfällen, Formatsemantik und Ablaufverträgen. Primärdokumentation plus ein konkreter Repository-Test ist hilfreicher als viele allgemeine Bloglinks. Version oder Einschränkung daneben nennen, wenn das Verhalten davon abhängt.

**Erledigt, wenn:** Leser die nötige Handlung und deren Ergebnis aus dem Text oder einem unmittelbar folgenden Beispiel verstehen, ohne interne Begriffe selbst erschließen zu müssen. Jede Ergänzung muss dem Verständnis oder der korrekten Anwendung helfen; bloße Wiederholungen entfallen. Die technische Bedeutung bleibt erhalten. Leser finden eine maßgebliche Erklärung pro Thema und erkennen, ob ein Beispiel aktuell geprüft, historisch oder ein Zukunftsvorschlag ist. Bestehende Zeilenumbrüche, Abstände und generierte Navigation nicht nebenbei großflächig normalisieren.

## Zukunftsideen aus dem Archiv

**N07 · inhaltliche Sicherung im RM umgesetzt · keine Implementierungszusage**

Die wichtigen Inhalte stehen jetzt im RM-Anhang [Future Design Ideas](../rm/future_development.md). Er enthält konkrete Entwurfsregeln, offene Entscheidungen, Grenzen und die Git-Fundstellen der Originale; das gelöschte Archiv wird dafür nicht benötigt.

- [Dictionary-Schema](../rm/future_development.md#a-self-describing-dictionary): Profile, Historie, feste Protokolltabellen und Unterschiede zwischen den beiden Entwürfen. „TIL V2“ ist unabhängig von Trice v2.0.0; keine automatische Datenkonversion.
- [Dashboard-Adapter](../rm/future_development.md#typed-streams-for-dashboard-tools): typisierte Kanäle, Ausgabeformate und Erhalt manueller Projekteinstellungen. Serial Studio bleibt zurückgestellt; aktuell gelten `-vis` und LabPlot.
- [C-Hostdecoder](../rm/future_development.md#a-small-c-host-decoder): möglicher kleiner Datei-Decoder und Vergleichstests mit Go; ein vollständiger C-Ersatz des Host-Tools bleibt eine eigene Produktentscheidung.

Nicht wieder eröffnen: bereits abgelehnte oder abgeschlossene Anforderungen aus alten Plänen, unvalidierte Leistungsversprechen, alte Profiltransfer-Automatisierung und historische Testfehler, die im aktuellen Stand behoben sind. `_Legacy`, archivierter CUI-/STIM-Code und `.code_snippets` sind keine Aufforderung zur Wiederbelebung. Vor einer späteren Bereinigung solcher Bestände deren tatsächliche Rolle und externe Nutzung gesondert prüfen.

## Referenz: abgeschlossene Vor-Release-Aufgaben

Die folgenden Inhalte sichern die früheren Punkte V04, V05 und V07 aus `Release_v2.0.0_Vor_dem_Release.md`. Sie waren am 2026-11-04 bereits umgesetzt und sind keine neuen offenen Aufgaben. Die damaligen Regeln und Ergebnisse sind hier zusammengefasst, damit die Folgearbeiten ohne das alte Dokument verständlich bleiben.

Der vollständige damalige Text steht in Commit `b11f468a41cdb0aacdf0edd8920ebd9302278ed5`:

```sh
git show b11f468a41cdb0aacdf0edd8920ebd9302278ed5:docs/_scratchPad/Release_v2.0.0_Vor_dem_Release.md
```

### Release-Dokumentation und Veröffentlichungspolitik

**Früher V04: Stable-Release und Versionsbeschreibung fertigstellen.**

- GoReleaser verwendet `release.prerelease: auto`: Beta-/RC-Tags ergeben ein Pre-Release, ein Tag wie `v2.0.0` ein stabiles Release. Homebrew verwendet `skip_upload: auto`; nur stabile Releases aktualisieren den normalen Tap.
- Der ausgewählte Tag braucht einen eigenen, nicht leeren Abschnitt im [Changelog](../../CHANGELOG.md). Die Schreibweise muss exakt passen: `v2.0.0-beta3` und `v2.0.0-beta-3` sind verschiedene Tags. Bestehende Tags bleiben unverändert.
- Der Stable-Überblick bündelt die tatsächlich enthaltenen Neuerungen, Korrekturen und Upgrade-Grenzen. Historische Beta-Notes beschreiben weiterhin ihren damaligen Umfang. Die Installation bleibt bei Release-Archiven oder lokalen Tag-Checkouts; eine `/v2`-Modulmigration gehört nicht dazu.
- Changelog, README, Einstieg und [Migrationshinweise](../rm/id_management.md#migrating-1x-scripts-and-projects) erklären die nötige Handlung und das erwartete Ergebnis. Upgrade-Beispiele zeigen bisherige und neue Verwendung. Fachbegriffe werden am Einsatzort kurz erklärt. Release Notes bleiben kurz; vollständige Anleitungen stehen in `docs/rm/`.

Die damaligen gezielten Prüfungen bestätigten die versionsgenaue Auswahl der Release Notes und die GoReleaser-Konfiguration. Sie ersetzen keine Abnahme eines späteren Release-Kandidaten. Der aktuelle Ablauf steht in [Building and Releasing the Tools](../rm/tool_build_release.md).

### Handbuchvollständigkeit und Stamp-Ausdrücke

**Früher V05: Das Kapitel zu Stamp-Ausdrücken fehlt im Referenz-PDF.**

Das Kapitel [Stamp Expressions](../rm/stamp_expressions.md) wurde in [das Referenzinventar](../rm/README.md) direkt nach den Zeitstempel-Grundlagen aufgenommen. Seine Navigation läuft über den bestehenden `mdtoc`-Prozess. Ein Inventartest erfasst alle Fachkapitel, auch solche ohne `mdtoc`; nur README, Index, Header und Footer sind ausgenommen.

Der ursprüngliche Fehler entstand, weil der PDF-Assembler Kapitel aus der README-Liste auswählt. Ein fehlendes Kapitel konnte deshalb als Weblink erscheinen, ohne im PDF enthalten zu sein. Ein erfolgreicher PDF-Build allein beweist keine Vollständigkeit. Die Regressionstests verwenden das echte Stamp-Kapitel und prüfen, dass die Beispiele für `printf`, `unwrap` und `delta` sowie interne Kapitelziele im zusammengesetzten Handbuch erhalten bleiben. Der damalige PDF-Stand wurde zusätzlich auszugsweise sichtbar geprüft; das war keine vollständige Sichtprüfung des Buchs.

### Gesicherte Decoder- und RX-Anleitungen

**Früher V07: Erhaltenswerte Inhalte vor einer Archivlöschung sichern.**

- [Adding a host decoder](../rm/target_test_internals.md#adding-a-host-decoder) erhält den Erweiterungspfad über `decoder.Decoder`, die Konstruktor-Signatur, `NewDecoderData`, Initialisierung, Registrierung, den Translator-Import und passende Tests.
- [Receiving and resolving records](../rm/target_implementation.md#receiving-and-resolving-records) erklärt Parse → Resolve → Consume, die logische Record-Länge und unvollständige Daten. Die Payload wird nicht kopiert; ihre Lebensdauer hängt am Eingabepuffer. Der Parser braucht zunächst keine TIL-Metadaten, und RX kann unabhängig von TX eingebunden werden.
- Aktive Dokumentation braucht keine Archivdateien mehr; ein Regressionstest schützt diese Grenze. Bereits vorhandene Bind-, CE-, Tag-, Alias- und ABC-Erklärungen bleiben maßgeblich. Alte offene Listen und erledigte Fehlerstände werden nicht erneut zu Anforderungen.
- Die gesonderten Zukunftsideen stehen in [Future Design Ideas](../rm/future_development.md). Historische Messungen, Experimente und Testprotokolle bleiben über Git verfügbar und gelten nicht als aktuelle Abnahme.

## Bewusst nicht als Releasehindernis gewertet

- Die fehlende `/v2`-Go-Modul-Endung: die bestehende Installationspolitik erklärt Release-Archive und lokale Tag-Checkouts. Eine Modulmigration wäre ein eigener Auftrag.
- Die Existenz historischer oder vendorter Dateien allein: Alter und fehlende direkte Aufrufer beweisen keine entbehrliche öffentliche Schnittstelle oder Lizenzdatei.
- Ältere Messbilder mit ausdrücklich historischem Kontext: sie sind kein aktueller Benchmark, aber auch kein Grund, ohne Auftrag neue Hardwaremessungen zu starten.
- Das Fehlen einer neuen Funktion aus einem Archivplan: maßgeblich sind der vereinbarte Umfang und das implementierte Verhalten.
- Die Zahl der Tests oder Kommentarzeilen: entscheidend sind verständliche Verträge und beobachtbares Verhalten, nicht bloß mehr Text oder Coverage-Prozentpunkte.
