# Trice v2.0.0: besser nach dem Release

## Zuerst erledigen

1. Zunächst die reproduzierten Fehler und Release-Voraussetzungen aus [Vor dem Release](Release_v2.0.0_Vor_dem_Release.md) abschließen.
2. Danach **N06 und N02** bearbeiten: die übrige Dokumentation systematisch verständlicher machen und die veraltete Beschreibung der Host-Architektur ersetzen.
3. **N01, N03 und N04** gezielt bearbeiten: ungenutzte Helfer prüfen, Tests an tatsächlichen Fehlergrenzen ergänzen und die selbst gepflegte Dokumentation vollständig in Prüfungen erfassen.
4. **N05** nur als getrennte Wartungsaufgabe angehen. Eine breite Skriptumbenennung kurz vor v2.0.0 bringt wenig Anwendernutzen und berührt viele Aufrufer.
5. **N07** bewahrt Zukunftsideen, ohne neue Funktionen zuzusagen.

Stand: Review vom 2026-11-03 auf `wip`, HEAD `08b31e2ca033aa428e4d06e7bd41231ef2075b0d`. Die folgenden Änderungen wurden nicht implementiert. Bereits direkt korrigierte Kleinigkeiten stehen ausschließlich im Git-Diff.

## Kandidaten für ungenutzten Code gezielt untersuchen

**N01 · mittlere Priorität · statische Kandidaten, keine pauschale Löschfreigabe**

Die Suche nach Symbolreferenzen in den aktiven Go-/C-Quellen zeigt mehrere Helfer ohne Produktionsaufrufer. Die Tabelle unterscheidet vollständig unreferenzierte Symbole von Funktionen, die nur noch ihre eigenen Tests beschäftigen. Das ist keine vollständige Callgraph- oder ABI-Analyse.

| Fundstelle | Beobachtung | Sinnvolle nächste Prüfung |
| --- | --- | --- |
| [internal/printf/expression.go](../../internal/printf/expression.go), `CompileExpression`, etwa Zeile 261 | Nur die Definition gefunden. Die aktuellen Aufrufer verwenden den kontextabhängigen Kompilierpfad. | Prüfen, ob der Wrapper noch eine geplante interne Schnittstelle ist; ansonsten separat entfernen. |
| [internal/id/list.go](../../internal/id/list.go), `SubCmdAddToList`, etwa Zeile 23 | Nur noch direkter Aufruf aus `manage_fs_test.go`; der CLI-Pfad verwendet eine andere Implementierung. | Verhaltenstest möglichst an den aktiven öffentlichen `add`-Pfad anschließen. Nicht das gesamte File löschen: darunterliegende Helfer können weiter benötigt werden. |
| [internal/id/Update.go](../../internal/id/Update.go), `triceParse`, etwa Zeile 154 | Nur `update_additional_test.go` ruft den Helfer auf. | Gegen den tatsächlich verwendeten Quellparser abgrenzen, bevor Code und Tests entfallen. |
| [internal/id/helper.go](../../internal/id/helper.go), `countColonsUntilClosingBracket`, etwa Zeile 650 | Referenzen nur in Parser-/Coverage-Tests. | Feststellen, ob ein alter Parserrest oder absichtlich isolierter Baustein; Tests nicht als alleinigen Nutzungsnachweis werten. |
| [internal/decoder/decoder.go](../../internal/decoder/decoder.go), `LocationInformation` und `CorrectWrappedTimestamp`, etwa Zeilen 493/524 | Aufrufe nur in den Tests dieses Pakets. Die Ausgabe wird inzwischen im Translator behandelt. | Aktuelle Zuständigkeit und Unterschiede zur Translator-Implementierung dokumentieren; danach gegebenenfalls eine alte Doppelimplementierung entfernen. |
| [internal/translator/translator.go](../../internal/translator/translator.go), `targetStampWidth` und `formatTargetDeltaValue`, etwa Zeilen 219/347 | Aufrufe nur in `translator_delta_test.go`. | Prüfen, ob die Tests den wirklichen Stamp-Ausgabepfad absichern oder nur verbliebene Altfunktionen. |
| [src/triceRx.h](../../src/triceRx.h), `extern triceFn_t fn_TriceDispatchAbc` | Im aktiven Quellbestand weder Definition noch Verwendung gefunden. | Gegen aktuelle ABC-Dispatch-Schnittstellen und mögliche externe Nutzer prüfen. Eine öffentliche Deklaration nicht allein aufgrund der lokalen Suche still entfernen. |
| [internal/receiver/receiver.go](../../internal/receiver/receiver.go), großer auskommentierter Block ab etwa Zeile 516 | Explizit als möglicherweise obsolet markierter früherer Receive-Loop. Kein ausführbarer Code. | Nach Ersatz der Architekturbeschreibung separat entfernen; Git bewahrt die Historie. |

**Nicht automatisch als tot einstufen:** registrierte Decoder und `init`-Funktionen, plattformabhängige Dateien, Funktionszeiger/Callbacks, öffentliche `pkg`-Hilfen oder absichtliche Testzugänge wie Wrapper um aktive Implementierungen. Makrovarianten in der C-Bibliothek gehören zur Konfigurationsmatrix; ein fehlender Aufruf in einer einzelnen Beispielkonfiguration ist kein Löschbeleg.

[internal/id/_nondeterministic_insert_test.go](../../internal/id/_nondeterministic_insert_test.go) ist ausdrücklich deaktivierter historischer Testcode mit Verweis auf die abgeschlossene Entscheidung zu Issue 616. Das ist kein Auftrag, die alte Anforderung vor dem Release wieder einzuführen. Aktuelle deterministische ID-Regeln haben eigene Tests.

**Erledigt, wenn:** jeder entfernte Helfer nachweislich keine aktive Verantwortung mehr trägt und die relevanten Verhaltenstests weiterhin den produktiven Pfad prüfen. Keine Umbenennungs- oder Kompatibilitätsmigration beiläufig mit dieser Bereinigung verbinden.

## Die Host-Architektur an einer Stelle aktuell erklären

**N02 · mittlere Priorität · inhaltliche Kommentar- und Dokumentationspflege**

Der Paketkommentar in [receiver.go](../../internal/receiver/receiver.go) beginnt mit „possibly obsolete“ und beschreibt unter anderem `trice.DoReceive`, `emit.Trice` und frühere Kanäle. Diese Beschreibung passt nicht zum aktuellen `io.ReadWriteCloser`-/Translator-Pfad. Der Kommentar zu `Translate` beschreibt einen Bool-Rückgabewert, während die Funktion einen Fehler zurückgibt. Weitere alte Namen stehen in den Receiver-Kommentaren.

Auch [Project Background and Resources](../rm/project_resources.md) enthält eine alte Parserdarstellung, bereits korrekt als v1.0 und als nicht geplanter Umbau gekennzeichnet. Der aktuelle TREX-Decoder stellt zusätzlich über `RecordProvider` strukturierte Informationen bereit; das ursprüngliche `Read`-Interface bleibt bestehen. Ein kurzer Verweis auf die heutige Architektur würde zeigen, welche damaligen Ziele bereits anders gelöst sind, ohne den alten Umbau wieder einzuplanen.

**Maßnahme:** eine kurze Darstellung des aktiven Wegs erstellen: Transport → Deframing/Decoder → Record mit Metadaten → Filter/Formatierung/Visualisierung → Ausgabe. Dabei Besitz und Lebensdauer von Readern/Writern, Dictionary-Locks, Diagnoseausgabe und Fehlerweitergabe erklären. Aus dem historischen Abschnitt auf die aktuelle Beschreibung verweisen und den aus V07 geretteten Decoder-Anhang einbeziehen.

Kommentare sollen vor allem Invarianten erklären: Warum wird ein Buffer nicht kopiert? Wer schließt einen Writer? Welche Fehler sind ein unvollständiger Record, welche verwerfen ihn? Welche Daten dürfen nach Unlock noch benutzt werden? Flächendeckende Kommentare nach dem Muster „verifies expected behavior“ erhöhen die Verständlichkeit kaum.

**Erledigt, wenn:** eine neue Entwicklerin den aktuellen Daten- und Fehlerfluss ohne Abgleich mit einer historischen Implementierung nachvollziehen kann. Keine Codeumstrukturierung allein zur Angleichung an die Dokumentation durchführen.

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

**N04 · niedrige bis mittlere Priorität · Werkzeugauswahl verbessern**

[lychee.toml](../../lychee.toml) schließt `third_party` vollständig aus. Dort liegen jedoch auch fünf kurze, vom Projekt gepflegte Wrapper-READMEs. Ihre veralteten Handbuchlinks wurden während dieses Reviews korrigiert, waren vom normalen Linkcheck aber nicht erfasst.

**Maßnahme:** eigene Wrapper-Dokumentation von fremden Texten und Binärpaketen unterscheiden und gezielt mitprüfen. Vendor-Dokumentation weiterhin nicht automatisch umschreiben. Ebenso beim Handbuchinventar klar zwischen Fachkapiteln, Index/Header/Footer und archiviertem Material unterscheiden; V05 ist die konkrete Vorabkorrektur dafür.

**Erledigt, wenn:** ein absichtlich beschädigter relativer Link in einer eigenen `third_party`-README erkannt würde und originale Vendor-Dateien unverändert bleiben. Externe Verbindungsfehler getrennt von fehlenden lokalen Dateien oder Fragmenten ausweisen.

## Skriptumbenennungen klein und getrennt halten

**N05 · niedrige Priorität · keine breite Umbenennung vor v2.0.0 empfohlen**

Die aktuelle Nummerierung trennt bereits interne Schritte und ihre Reihenfolge. Sie wurde erst kürzlich vereinheitlicht; die Migrationsdokumentation nennt die neuen Namen. Eine weitere komplette Umbenennung oder Neunummerierung erhöht jetzt den Aufwand für Benutzer und Aufrufer.

Einzelne Namen könnten später verständlicher werden:

| Heutiger Name | Möglicher Name | Konkreter Grund |
| --- | --- | --- |
| `scripts/_320_generate_trice_user_manual_pdf.sh` | `scripts/_320_generate_trice_manual_pdfs.sh` | Erzeugt inzwischen User- und Reference-Manual-PDF. Der bisherige Singular und die Beschränkung auf das User Manual sind zu eng. |
| `trice_bindIDs_in_examples_and_test_folder.sh` im Repository-Root | `scripts/bind_examples_and_tests.sh` | Name und Ablage könnten das öffentlich aufrufbare Bind-Hilfsskript kürzer und einheitlicher beschreiben. |
| `scripts/buildTriceTool.sh` | `scripts/build_trice_tools.sh` | Baut `trice` und `tlog`; Plural und die verbreitete Unterstrichschreibweise wären präziser. Der vorhandene Name ist aber gut dokumentiert und kein Releaseproblem. |

`scripts/testAll.sh` würde ich als etablierten Einstieg behalten. Interne nummerierte Tests und Beispiel-Wrapper nicht allein aus Stilgründen umbenennen. Ein führender Unterstrich sollte interne Helfer erkennen lassen; öffentlich dokumentierte Einstiege brauchen vor allem stabile, klare Namen.

**Vor einer tatsächlichen Änderung alle Aufrufer erfassen:** direkte Entwickleraufrufe, `testAll.sh`, `format_repo.sh`, Beispielvorbereitung, `.goreleaser.yaml`, GitHub-Workflows, Skripttests und aktive Dokumentation. Die bestehenden Verhaltenstests des jeweiligen Aufrufwegs mitziehen. Keine stillen Kompatibilitätswrapper oder zusätzliche Migration einführen; deren Bedarf wäre gesondert zu entscheiden. Archivierte Texte nicht umschreiben.

**Erledigt, wenn:** jede gewählte Umbenennung einen konkreten Verständlichkeitsgewinn hat, alle aktiven Aufrufer passen und der Änderungssatz ausschließlich diese Aufgabe enthält.

## Dokumentation mit konkreten Anwendungsbeispielen verständlich machen

**N06 · mittlere Priorität · systematische Überarbeitung der aktiven Dokumentation**

**Auftrag vom 2026-11-04:** die gesamte selbst gepflegte aktive Dokumentation auf Verständlichkeit für menschliche Leser prüfen. Technische Korrektheit und Klarheit sind gleich wichtig. Release Notes, Einstieg und Migration zuerst gemäß V04 bearbeiten; danach die übrigen Referenzkapitel und Beispielanleitungen. Archivierte und fremde Texte bleiben ausgenommen.

**Maßstab:** Leser sollen erkennen, wann sie eine Funktion brauchen, was sie tun müssen und welches Ergebnis sie erhalten. Abstrakte Fachbegriffe und Regeln vermeiden oder unmittelbar durch ein kleines Anwendungsbeispiel erklären. Beispiel: Für `MyWrn("err:temperature=%u\n", 85)` schreibt `-vis='wrn:printf("%d\n",v0)@warnings.csv'` die Zeile `85` in `warnings.csv`; `v0` ist der erste Messwert. Das zeigt die Wirkung deutlicher als „registered selectors select event tags“.

**Vorgehen:** pro erklärungsbedürftiger Regel einen realistischen Aufruf oder eine Eingabe und das erwartete Ergebnis zeigen. Nur wenn eine Grenze sonst unklar bleibt, einen Gegenfall ergänzen. Begriffe beim ersten Gebrauch kurz erklären. Vage Absätze durch Beispiele ersetzen, unnötige Sätze streichen und längere Details verlinken; nicht zu jeder bisherigen Erklärung zusätzliche Beispiele anhängen. Auch die To-do-Papiere selbst nach diesem Maßstab prüfen.

User Manual, Quickstarts und Referenz erfüllen verschiedene Aufgaben. Kurze Wiederholungen von Voraussetzung, Beispiel und erwartetem Ergebnis sind dort sinnvoll. Die neuen Vergleichstabellen zu Bind, CE und Compilergrenzen sollten erhalten bleiben.

**Dabei Wiederholungen und historischen Kontext prüfen:**

- Die verbindlichen Upgrade-Regeln in [ID Management](../rm/id_management.md) belassen. README, Release Notes und Einführungen mit kurzen Hinweisen dorthin verlinken, statt jeweils vollständige Migrationslisten zu führen.
- Längere Entscheidungsbegründungen in den vorhandenen thematischen Anhängen von [Bind](../rm/bind.md) und [CE](../rm/context_enrichment.md) halten. Bei künftigen Ergänzungen zuerst prüfen, ob bereits eine Tabelle dieselbe Grenze beschreibt.
- Die historischen Integrationsrezepte in [Platform Integration Recipes](../rm/integration_notes.md) weiterhin klar als solche kennzeichnen. Die dort bereits genannte fehlende heutige Bind-Validierung ist keine neue erfolgreiche Abnahme. Aktualisierung nur zusammen mit einer tatsächlich nachvollzogenen Beispielintegration.
- Die bereits als historisch gekennzeichnete Parserbeschreibung aus [Project Resources](../rm/project_resources.md) im Zuge von N02 mit der aktuellen Architektur verknüpfen. Die ausdrückliche Kennzeichnung als nicht geplanter Umbau erhalten.

Zusätzliche Referenzlinks lohnen sich vor allem bei Compiler-Sonderfällen, Formatsemantik und Ablaufverträgen. Primärdokumentation plus ein konkreter Repository-Test ist hilfreicher als viele allgemeine Bloglinks. Version oder Einschränkung daneben nennen, wenn das Verhalten davon abhängt.

**Erledigt, wenn:** Leser die nötige Handlung und deren Ergebnis aus dem Text oder einem unmittelbar folgenden Beispiel verstehen, ohne interne Begriffe selbst erschließen zu müssen. Jede Ergänzung muss dem Verständnis oder der korrekten Anwendung helfen; bloße Wiederholungen entfallen. Die technische Bedeutung bleibt erhalten. Leser finden eine maßgebliche Erklärung pro Thema und erkennen, ob ein Beispiel aktuell geprüft, historisch oder ein Zukunftsvorschlag ist. Bestehende Zeilenumbrüche, Abstände und generierte Navigation nicht nebenbei großflächig normalisieren.

## Zukunftsideen aus dem Archiv

**N07 · niedrige Priorität · inhaltliche Sicherung, keine Implementierungszusage**

Die folgenden Ideen sind hier bewusst knapp gesichert. Das reicht für die spätere Wiederaufnahme; vollständige Entwürfe, damalige Implementierungspläne und Gesprächsprotokolle müssen nicht in die aktive Referenz kopiert werden. Originalpfade beziehen sich auf den Review-Commit und bleiben darüber aus Git lesbar.

| Quelle unter `docs/_scratchPad/obsolete` | Erhaltenswerter Kern | Abgrenzung und späteres Ziel |
| --- | --- | --- |
| `trice-til-v2-entwurf-0.2/til-json-v2-spezifikation.md` und ältere TIL-Vorschläge | Eine künftige versionierte Dictionary-Struktur könnte Decoder-Metadaten und Format-/Profilinformationen expliziter und selbstbeschreibend ablegen. Entscheidend sind klare Versionskennung, reproduzierbare Interpretation und prüfbare Grenzen zwischen Firmware, Dictionary und Host. | „TIL v2“ im Entwurfsnamen ist kein Nachweis für das Format von Trice v2.0.0. Zuerst den neueren Entwurf mit der heutigen TIL-Struktur vergleichen; keine automatische Datenkonversion. Ein knapper Zukunftsabschnitt in [ID Management](../rm/id_management.md) wäre ein geeignetes Ziel. |
| `trice_serial_studio_tss_entwurf.md` | Wiederverwendbar sind die Fragen nach Kanalnamen, Einheiten, Transformationen, Zeitbasis und einem stabilen Austauschformat für externe Dashboard-Werkzeuge. | Der Entwurf ist ausdrücklich zugunsten anderer Tools zurückgestellt. Das bleibt so; keine geplante Serial-Studio-Integration daraus ableiten. Aktuell dokumentiertes `-vis` und LabPlot bleiben maßgeblich. Allgemeine Überlegungen könnten später in einen Ideenanhang von [Visualization](../rm/visualization.md); vorgeschlagene CLI-Syntaxen, Projektformate und MessagePack-Unterstützung sind nicht als verfügbar darzustellen. |
| `TriceRx_Tlog_C_ArchitectureDesign.md`, Zukunftsteil | Der gemeinsame Record-Parser und getrennte Resolver können später weitere C-seitige Empfänger oder einen eigenständigen C-Decoder unterstützen. Das kann Speicherbedarf und Nutzung ohne Go-Host adressieren. | Lokales Target-Logging ist bereits implementiert; ein vollständiger C-Ersatz des Go-Tools folgt daraus nicht. Die heute gültigen RX-Regeln gehören schon vor der Archivlöschung in V07; der zusätzliche Host bleibt eine eigene Produktentscheidung. |

Nicht wieder eröffnen: bereits abgelehnte oder abgeschlossene Anforderungen aus alten Plänen, unvalidierte Leistungsversprechen, alte Profiltransfer-Automatisierung und historische Testfehler, die im aktuellen Stand behoben sind. `_Legacy`, archivierter CUI-/STIM-Code und `.code_snippets` sind keine Aufforderung zur Wiederbelebung. Vor einer späteren Bereinigung solcher Bestände deren tatsächliche Rolle und externe Nutzung gesondert prüfen.

## Bewusst nicht als Releasehindernis gewertet

- Die fehlende `/v2`-Go-Modul-Endung: die bestehende Installationspolitik erklärt Release-Archive und lokale Tag-Checkouts. Eine Modulmigration wäre ein eigener Auftrag.
- Die Existenz historischer oder vendorter Dateien allein: Alter und fehlende direkte Aufrufer beweisen keine entbehrliche öffentliche Schnittstelle oder Lizenzdatei.
- Ältere Messbilder mit ausdrücklich historischem Kontext: sie sind kein aktueller Benchmark, aber auch kein Grund, ohne Auftrag neue Hardwaremessungen zu starten.
- Das Fehlen einer neuen Funktion aus einem Archivplan: maßgeblich sind der vereinbarte Umfang und das implementierte Verhalten.
- Die Zahl der Tests oder Kommentarzeilen: entscheidend sind verständliche Verträge und beobachtbares Verhalten, nicht bloß mehr Text oder Coverage-Prozentpunkte.
