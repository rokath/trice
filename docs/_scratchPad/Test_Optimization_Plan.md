# Test Optimierungsplan

Stand: 2026-11-10. Status: Planung; alle Abarbeitungsstufen sind offen.

Die Tests sollen auf Windows, Linux und macOS schneller und weiterhin verlässlich laufen. Der vollständige Intel-Lauf soll innerhalb des bestehenden CI-Joblimits abschließen. Testabdeckung, verständliche Fehlerdiagnosen und einfache Bedienung bleiben erhalten.

<h2>ToC</h2>
<!-- mdtoc -->

- [1. Hinweis](#hinweis)
- [2. Empfehlung](#empfehlung)
- [3. Abarbeitungsstufen](#abarbeitungsstufen)
- [4. Nächster Auftrag und Freigabe](#nächster-auftrag-und-freigabe)
- [5. Anhang A Messdaten und Vergleich](#anhang-a-messdaten-und-vergleich)
  - [5.1. Bisherige Ausgangsbefunde](#bisherige-ausgangsbefunde)
  - [5.2. Wichtige Details des Windows Laufs](#wichtige-details-des-windows-laufs)
  - [5.3. Geplanter Vergleich auf demselben Dual Boot PC](#geplanter-vergleich-auf-demselben-dual-boot-pc)
- [6. Anhang B Stufendetails](#anhang-b-stufendetails)
  - [6.1. Messdaten und Testabdeckung erfassen](#messdaten-und-testabdeckung-erfassen)
  - [6.2. Zuschnitt und Reihenfolge entscheiden](#zuschnitt-und-reihenfolge-entscheiden)
  - [6.3. Toolcache und Fortschritt absichern](#toolcache-und-fortschritt-absichern)
  - [6.4. Mehrfachausführungen bewerten und reduzieren](#mehrfachausführungen-bewerten-und-reduzieren)
  - [6.5. PC Ausführungskosten verringern](#pc-ausführungskosten-verringern)
  - [6.6. Vorhandene Worker Parallelisierung abstimmen](#vorhandene-worker-parallelisierung-abstimmen)
  - [6.7. Teilweise Aufteilung in CI Jobs erproben](#teilweise-aufteilung-in-ci-jobs-erproben)
  - [6.8. Ergebnisse abnehmen und dokumentieren](#ergebnisse-abnehmen-und-dokumentieren)
  - [6.9. Compilerabdeckung gesondert entscheiden](#compilerabdeckung-gesondert-entscheiden)
- [7. Anhang C Jobaufteilung und technische Grenzen](#anhang-c-jobaufteilung-und-technische-grenzen)
  - [7.1. Bedienung und Pflege begrenzen](#bedienung-und-pflege-begrenzen)
  - [7.2. Verbindliche Grenzen](#verbindliche-grenzen)
- [8. Anhang D Entscheidungen und Fortschritt](#anhang-d-entscheidungen-und-fortschritt)
- [9. Anhang E Vollständige ursprüngliche Übergabe](#anhang-e-vollständige-ursprüngliche-übergabe)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

## 1. <a id="hinweis"></a>Hinweis

Um die Test-Logs geräteübergreifend und auch in ihrer Veränderung besser vergleichen zu können, sollen zukünftig keine Zufallsnamen mehr verwendet werden. Die Logs sollen im temp/log Folder einfach unter testAll bzw. l432 angelegt werden aber am besten pro Testlauf nur einen Folder mit Subfolderen und intern auch keine Zufallsnamen haben. Wenn ein Testlauf beendet wird, auch abgebrochen, dann soll ein zip mit Datum und Uhrzeit mit Sekunde und OS im Namen erstellt werden für die Archivierung. Wenn ein neuer Test angestoßen wird, werden einfach alle Files im `./temp/log/` Folder gelöscht, so dass nur die zip-Files mit der Vergangenheit bleiben. In dem zip sollen auch die zugehörigen Konsolenausgaben, nutzbar als Überblick, enthalten sein.

## 2. <a id="empfehlung"></a>Empfehlung

Doppelarbeit gezielt reduzieren und für die vollen CI-Läufe eine teilweise Aufteilung auf **vier feste Gruppen** prüfen: allgemeine Prüfungen, Firmware/L432, PC-GCC und PC-Clang. Beim vorhandenen Windows-Lauf entfallen darauf ungefähr **93, 52, 34 und 31 Minuten**. Bei nur drei Gruppen bliebe ein Block von rund **145 Minuten**. Das spricht vorläufig für vier Gruppen; zusätzliche Vorbereitung, Runner-Wartezeit und Gesamtaufwand müssen vor der Entscheidung einbezogen werden.

Der neue Debian-Lauf auf derselben Hardware dauert **76 min 33 s**, gegenüber **210 min 25 s** unter Windows. Seine größten Blöcke sind die PC-Matrizen; die Go-/Integrationstests sind deutlich kürzer. Der Gruppenzuschnitt bleibt deshalb vorläufig und muss je Plattform anhand der Messwerte bewertet werden. Die detaillierten MacBook-M4-Pro-Daten stehen noch aus.

Ein gemeinsamer Einstieg und eine zentrale Stufenliste bleiben erhalten. Keine eigene Pipeline pro Konfiguration. Lokale Standardbedienung und PR-Auswahl `quick` bleiben zunächst unverändert. Die Jobaufteilung verkürzt vor allem die Wartezeit; überflüssige Testausführung, Build- und Prozessstarts müssen daneben gesondert untersucht werden.

Dieser Plan ist die **alleinige Arbeitsgrundlage für diese Optimierungsaufgabe**. Er enthält die vollständige bisherige Übergabe als historischen Text in Anhang E. Die Regeln in [AGENTS.md](../../AGENTS.md) gelten weiterhin.

## 3. <a id="abarbeitungsstufen"></a>Abarbeitungsstufen

Stufe | Aufgabe                                  | Ergebnis
----- | ---------------------------------------- | ----------------------------------------------
P1    | Messdaten und Testabdeckung erfassen     | Vergleichbare Basis und Testlandkarte
P2    | Maßnahmen und Jobzuschnitt entscheiden   | Prioritäten und begrenzter Pilot
P3    | Toolcache und Fortschritt absichern      | Verlässliche Vorbereitung und Statusanzeige
P4    | Mehrfachausführungen reduzieren          | Weniger Wiederholung bei belegter Abdeckung
P5    | PC Ausführungskosten verringern          | Weniger Starts oder Builds; gleiche Diagnose
P6    | Bestehende Worker abstimmen              | Gemessene Budgets für L432 und PC
P7    | Teilweise Aufteilung in CI Jobs erproben | Isolierte Gruppen mit gemeinsamem Gesamtstatus
P8    | Ergebnisse abnehmen                      | Messvergleich und native Plattformnachweise
O1    | Compilerabdeckung gesondert bewerten     | Optional; keine automatische GCC-Reduktion

P1 und P2 bilden die erste Untersuchung und Entscheidung. P3 bis P6 sind getrennt prüfbare Maßnahmen. P7 kann danach vorgezogen werden, wenn dies den Intel-Nachweis schneller ermöglicht; es muss nicht auf alle anderen Optimierungen warten. Die Stufendetails und Abnahmekriterien stehen in Anhang B, der Jobvergleich in Anhang C.

## 4. <a id="nächster-auftrag-und-freigabe"></a>Nächster Auftrag und Freigabe

Empfehlung für den nächsten Auftrag: **P1 bearbeiten und P2 als konkrete Entscheidungsvorlage ausarbeiten.** Dabei zuerst die vorhandenen Logs nutzen. Zusätzliche notwendige Messläufe und die danach umzusetzende Stufe getrennt benennen. Der inzwischen erfasste Debian-Lauf auf demselben Dual-Boot-PC wird in P1 als besonders hilfreicher Vergleich berücksichtigt. Die detaillierten MacBook-M4-Pro-Daten folgen später.

Aktuell ist ausschließlich dieses Plan-Dokument beauftragt. Es wurden keine neuen Tests oder Benchmarks gestartet und keine Testskripte oder CI-Workflows geändert. „P4 konkretisieren“ bedeutet Planarbeit; „P4 umsetzen“ beauftragt die betreffende Stufe. Teststreichungen, Änderungen der Compilerabdeckung, vollständige Abschlussläufe, Git-Ausführung und Release-Aktionen benötigen weiterhin ihre jeweilige ausdrückliche Beauftragung.

## 5. <a id="anhang-a-messdaten-und-vergleich"></a>Anhang A Messdaten und Vergleich

### 5.1. <a id="bisherige-ausgangsbefunde"></a>Bisherige Ausgangsbefunde

Bei der bisherigen Bestandsaufnahme lag der Checkout auf `wip`, HEAD `5f1d0bcde105bb61bae0f78a15fd23f6d20745fa`. Gegenüber dem in der Übergabe genannten Stand `3719635e` waren nur Changelog und Übergabe hinzugekommen beziehungsweise geändert; Testcode und CI entsprachen weiterhin diesem Ausgangsstand. Beim Eintragen der Debian-Daten liegt der Checkout auf `wip`, HEAD `f713ceab876bdbaeaffe862745743632a6d5a63e`; der Commit des Debian-Testlaufs ist im übermittelten Konsolenauszug nicht enthalten.

Lauf                 | Dauer und Vorbereitung               | Einordnung
-------------------- | ------------------------------------ | -----------------------------------------------------------------------
Windows lokal, full  | 12.625 s Testzeit; 68 s Vorbereitung | PASS; alle 25 Stufen; Go 1.27.3
Debian lokal, full   | 4.593 s Testzeit; 101 s Vorbereitung | Alle 25 Stufen PASS; abschließender Zustandsvergleich FAIL, siehe unten
Mac M4 Pro lokal     | ca. 4.000–5.000 s laut Nutzer        | Detaillierte Daten folgen; Aufruf, Commit, Cache und Zeitbasis offen
macOS Intel CI, full | 180 min Joblimit erreicht            | Unvollständig; letzte PC-Clang-Stufe abgebrochen

Der lokale Windows-Lauf vom 2026-11-09 ist im vorhandenen Protokoll `temp/log/testAll.M77lk0/testAll_summary.log` bestätigt. Aufruf: `./scripts/testAll.sh --no-format`; protokollierter Commit: `3719635e40372b5fa5ca18ed225fb685351781bb`. Die Rohprotokolle liegen nur lokal; die wesentlichen Zahlen sind deshalb hier festgehalten.

Die drei Windows-Stufen 515, 540 und 550 benötigen zusammen **5.445 s beziehungsweise 90 min 45 s**, rund **43,1 %** der Testzeit: 11 min 49 s Features, 38 min 56 s normale Go-Tests und 40 min Coverage. Daraus folgt ein Untersuchungsbedarf, aber keine bereits nachgewiesene Einsparung in dieser Höhe.

Auf Windows liefen L432 bereits mit **8 äußeren Workern** und `make -j1`, die PC-Matrizen mit **4 Workern** und `TRICE_PC_TEST_MODE=auto`. Eine fehlende Parallelisierung oder ein vollständig fehlender Bulk-Modus ist daher keine zutreffende Ausgangsannahme.

Die Übergabe dokumentiert für CI vollständige PASS-Läufe auf Linux, Windows und ARM-macOS mit ungefähr 88, 115 und 118 Minuten einschließlich Vorbereitung. Intel erreichte im Lauf `42682343023`, Job `129379957725`, das 180-Minuten-Limit. Die kalte Intel-LLVM-Vorbereitung bestand nach 26 Sekunden; das ersetzt keinen vollständigen Intel-PASS. Der nachfolgende Race-Check und das Speichern des neuen Projekt-Toolcaches blieben dort aus.

Lokale Testzeit, CI-Jobdauer und die ungefähre M4-Pro-Angabe werden nicht unmittelbar als Geschwindigkeitsverhältnis gewertet. Der lokale Windows-/Debian-Vergleich auf derselben Hardware wird unten gesondert ausgewiesen. P1 ordnet Commit, Umfang, Worker, Toolversionen und Cachezustand weiter zu.

### 5.2. <a id="wichtige-details-des-windows-laufs"></a>Wichtige Details des Windows Laufs

- Tatsächlich gewählte Compiler: **WinLibs GCC 16.2.0**, **Clang 23.1.3**, Target `x86_64-w64-windows-gnu`; ARM-GCC **15.2.1**. CMake **4.4.4** wurde wiederverwendet.
- Die anfänglichen `INCOMPLETE`-Meldungen zu TDM-GCC 10.3.0 und systemweitem LLVM 21.1.7 betreffen verworfene Kandidaten. Die anschließenden Laufzeitproben mit den ausgewählten Compilern und der vollständige Lauf bestanden.
- L432: **101 Konfigurationen, 8 Worker, inneres Make mit einem Job**, Dauer **15 min 23 s**.
- PC: je **63 Konfigurationen** pro Compiler-/ID-Kombination, **4 Worker**, inneres Go-Build-Budget `-p 1`, Modus `auto`. GCC Insert/Clean **16 min 44 s**, GCC Bind **17 min 05 s**, Clang zusammen **31 min 08 s**.
- Go verwendete `.gocache`. Der Inhalt vor dem Start ist nicht dokumentiert. Die 68 Sekunden Vorbereitung und wiederverwendete Tools beweisen keinen vollständig warmen Go-/Compiler-Cache.
- Für die Wiederherstellung wurden Zustands-Snapshots angelegt; der abschließende Vergleich des getrackten Arbeitsbaums bestand.

Die JSON-Protokolle zeigen für `scripts` **2.313,622 s** in Stufe 540 und **2.364,941 s** in Stufe 550. Es ist dort jeweils das langsamste Paket und liegt nahe an der gesamten Stufenzeit. Das macht die Skript- und Integrationsprüfungen zu einem ersten Untersuchungsschwerpunkt neben den eigentlichen PC-Matrizen.

Ausgewählte bereits vorhandene PASS-Ereignisse:

Test im Paket scripts                     | 515 in s         | 540 in s | 550 in s
----------------------------------------- | ---------------- | -------- | --------
TestHostDemoEntrypointIntegration         | 244,46           | 240,61   | 250,39
TestBindEntrypointIntegration             | 184,92           | 207,82   | 207,10
TestRuntimePreparationChecksHostCompilers | nicht ausgewählt | 233,98   | 233,70
TestPCCacheOverlayRealCGOBuilds           | nicht ausgewählt | 88,81    | 89,19

Gleiche Testnamen belegen hier wiederholte Ausführung, aber nicht automatisch identische Bedingungen oder verzichtbare Nachweise. Einzeltest- und Paketzeiten können sich durch Parallelität oder Verschachtelung überlappen und dürfen nicht ungeprüft addiert werden. Die Rohdaten stammen aus den vorhandenen JSON-Dateien der Stufen 515, 540 und 550 im oben genannten Laufverzeichnis.

### 5.3. <a id="geplanter-vergleich-auf-demselben-dual-boot-pc"></a>Geplanter Vergleich auf demselben Dual Boot PC

Der Nutzer hat am **2026-11-10 unter Debian auf derselben Hardware wie beim Windows-Lauf** gemessen. Quelle ist der übermittelte Konsolenauszug; Aufruf `./scripts/testAll.sh`, angezeigter Umfang `full`, Host `Linux x86_64`. Formatierung wurde mit `N` abgelehnt. Start **12:02:16 GMT**, Ende **13:18:49 GMT**, reine Testzeit **4.593 s = 76 min 33 s**. Vorbereitung: **101 s**, einschließlich interaktiver Auswahl und außerhalb der Testzeit; zusammen **4.694 s = 78 min 14 s**.

**Ergebnis:** alle **25 Teststufen PASS**. Der Runner meldete anschließend `Result: FAIL` und Exitstatus 1, weil sich `docs/_scratchPad/Test_Optimization_Plan.md` während des Laufs geändert hatte. Laut Nutzer wurde parallel im Scratchpad gearbeitet. Dieser abschließende Zustandsvergleich ist deshalb hier kein Hinweis auf einen Testfehler; die Wiederherstellung wird durch diesen Lauf aber auch nicht unabhängig nachgewiesen. Toolprotokoll: `temp/log/testAll.pFPVw7/tools.log`; Zustandsevidenz: `temp/log/testAll.pFPVw7/testAll-state.LIEq0R`. Diese Pfade sind lokale Nachweise und müssen nicht auf anderen Rechnern vorhanden sein.

Die auswertbaren Protokolle sind im [Debian-Logarchiv vom 2026-11-10](test_logs/2026-11-10_debian_full.zip) gesichert: **412 Dateien, 2.839.677 Bytes ZIP**, rund 29,9 MB unkomprimiert. Enthalten sind Zusammenfassung, `results.json`, `steps.tsv`, Tool- und Stufenlogs einschließlich Go-JSON- und ergänzender Build-Protokolle, `coverage.out`, alle **252 PC-Konfigurationslogs** und **101 L432-Konfigurationslogs**. Die ursprünglichen relativen Pfade und Dateiinhalte bleiben erhalten. Generierte Quellen, Cache-Inhalte, Binärdateien und Zustandssnapshots sind ausgeschlossen; die oben genannte Zustandsevidenz bleibt nur lokal. ZIP-Integrität und Übereinstimmung aller archivierten Dateien mit den Originalen wurden erfolgreich geprüft.

Alle angezeigten Stufenzeiten:

Stufe | Skript                         | Debian      | Ergebnis
----- | ------------------------------ | ----------- | --------
400   | _400_test_shell_format.sh      | 0 min 00 s  | PASS
410   | _410_test_shellcheck.sh        | 0 min 10 s  | PASS
420   | _420_test_dsstore.sh           | 0 min 00 s  | PASS
430   | _430_test_clang_format.sh      | 0 min 02 s  | PASS
440   | _440_test_manual_format.sh     | 0 min 00 s  | PASS
450   | _450_test_markdownlint.sh      | 0 min 02 s  | PASS
460   | _460_test_goreleaser_config.sh | 0 min 00 s  | PASS
470   | _470_test_actionlint.sh        | 0 min 01 s  | PASS
480   | _480_test_build_trice_tool.sh  | 0 min 05 s  | PASS
490   | _490_test_runtime_prepare.sh   | 0 min 02 s  | PASS
500   | _500_test_bind.sh              | 0 min 06 s  | PASS
510   | _510_test_bind_workflows.sh    | 0 min 01 s  | PASS
515   | _515_test_logging_features.sh  | 2 min 18 s  | PASS
520   | _520_test_target_code_lint.sh  | 0 min 12 s  | PASS
540   | _540_test_go.sh                | 2 min 44 s  | PASS
550   | _550_test_go_coverage.sh       | 2 min 50 s  | PASS
560   | _560_test_clang_insert.sh      | 0 min 06 s  | PASS
570   | _570_test_clang_bind.sh        | 0 min 06 s  | PASS
580   | _580_test_gcc_off.sh           | 0 min 18 s  | PASS
590   | _590_test_gcc_insert.sh        | 1 min 26 s  | PASS
600   | _600_test_gcc_bind.sh          | 2 min 33 s  | PASS
620   | _620_test_l432_configs.sh      | 7 min 12 s  | PASS
630   | _630_test_pc_targets_insert.sh | 16 min 09 s | PASS
640   | _640_test_pc_targets_bind.sh   | 16 min 22 s | PASS
645   | _645_test_pc_targets_clang.sh  | 23 min 47 s | PASS

Die angezeigten Stufen summieren sich auf **4.592 s**; zur gemeldeten Gesamtdauer bleibt **1 s** Ablauf-/Rundungsdifferenz. Angezeigte 0 Sekunden bedeuten keine nachgewiesene Auslassung.

Toolstand laut Vorbereitung:

Tool             | Version im Debian-Lauf | Herkunft laut Ausgabe
---------------- | ---------------------- | --------------------------------------------
Go               | 1.27.3                 | System
Node.js          | 26.12.1                | System
cmake            | 4.4.4                  | System
gcc              | 15.2.0                 | System
clang            | 23.1.4                 | System
arm              | 15.2.1                 | System
goreleaser       | 2.19.0                 | temp/tools; Systemversion 2.20.0
actionlint       | 1.7.12                 | System
shellcheck       | 0.11.0                 | System
jq               | 1.8.2                  | System
shfmt            | 3.14.1                 | System
mdtoc            | 1.0.3                  | System
stitchmd         | dev / 0.9.0            | System; Anzeige: stitchmd dev / 0.9.0
pandoc           | 3.8.3                  | temp/tools
cppcheck         | 2.22.0                 | System
clang-format     | 23.1.3                 | System
Python           | 3.13.5                 | System; Anforderung >=3.9
markdownlint-cli | 0.50.0                 | temp/tools
md-to-pdf        | 5.2.5                  | temp/tools
puppeteer        | 25.16.0                | Vorab als System angezeigt; npm-Installation

Auswahl `u`: keine System-Upgrades verfügbar. Installation in `temp/tools`: goreleaser **2 s**, npm-Tools (`markdownlint-cli`, `md-to-pdf`, `puppeteer`) zusammen **3 s**, pandoc **3 s**. Die Vorbereitung nennt dafür ungefähr **588 MB** Download-/Installationsbedarf und **2,6 GB** freien Platz. Der vorhandene Zustand von Go- und Compiler-Build-Caches ist nicht dokumentiert; die kurzen Installationszeiten belegen keinen kalten Download und keinen warmen Build-Cache.

Vergleich der größten Blöcke; Faktor = Windows-Zeit geteilt durch Debian-Zeit:

Block                    | Windows      | Debian      | Faktor
------------------------ | ------------ | ----------- | ------
Gesamte Testzeit         | 210 min 25 s | 76 min 33 s | 2,75
515: Features            | 11 min 49 s  | 2 min 18 s  | 5,14
540: Go                  | 38 min 56 s  | 2 min 44 s  | 14,24
550: Coverage            | 40 min 00 s  | 2 min 50 s  | 14,12
515 + 540 + 550          | 90 min 45 s  | 7 min 52 s  | 11,54
620: L432                | 15 min 23 s  | 7 min 12 s  | 2,14
630: PC-GCC Insert/Clean | 16 min 44 s  | 16 min 09 s | 1,04
640: PC-GCC Bind         | 17 min 05 s  | 16 min 22 s | 1,04
645: PC-Clang            | 31 min 08 s  | 23 min 47 s | 1,31

**Erste Einschätzung:** Debian benötigt auf derselben Hardware **63,6 % weniger Testzeit**, entsprechend Faktor **2,75** und einer Differenz von **133 min 52 s**. Davon entfallen **82 min 53 s** auf 515/540/550. Diese Stufen beanspruchen unter Debian nur **10,3 %** statt **43,1 %** der Testzeit. Die PC-Matrizen benötigen dagegen zusammen **56 min 18 s = 73,5 %** der Debian-Testzeit; insbesondere PC-GCC ist auf beiden Systemen fast gleich schnell. Die Unterschiede verteilen sich also stark ungleich. Das stützt eine getrennte Untersuchung von Go-/Integrationsprüfungen, Firmware und PC-Matrizen; eine konkrete Ursache oder verzichtbare Prüfung ist damit noch nicht belegt.

Noch offen für die Vergleichbarkeit: exakter Commit und Test-/CI-Code-Stand des Debian-Laufs, Debian-Version, Checkout-Dateisystem und Cache-Lage, Shell-Version, wirksame L432-/PC-Worker und innere Build-Budgets, Build-Cache-Zustand, Energieprofil und Hintergrundlast. GCC unterscheidet sich bereits sichtbar (**Windows 16.2.0, Debian 15.2.0**), ebenso Clang (**23.1.3 / 23.1.4**). Die Gesamtdifferenz wird daher nicht als reine Betriebssystemwirkung gewertet. Der Debian-Lauf bleibt ein eigener Cache-Fall; neue Messläufe sind nicht beauftragt. Die detaillierten **MacBook-M4-Pro-Daten folgen später**.

## 6. <a id="anhang-b-stufendetails"></a>Anhang B Stufendetails

### 6.1. <a id="messdaten-und-testabdeckung-erfassen"></a>Messdaten und Testabdeckung erfassen

**P1**

Vorhandene JSON-, Stufen- und Konfigurationsprotokolle auswerten. Testnamen, Pakete, Build-Tags, Umgebungsvariablen, Compiler, ID-Workflow, Coverage-Instrumentierung und Fehlernachweise den aufrufenden Stufen zuordnen. Zusätzlich zur Suite die eigenständigen Workflows für [Coverage](../../.github/workflows/coverage.yml), [Script portability](../../.github/workflows/script-portability.yml) und [Bibliothekstests](../../.github/workflows/trice_lib_reusable.yml) berücksichtigen.

Für die größten Blöcke Vorbereitung, Kompilierung, Logger-/Prozessstarts und eigentliche Prüfzeit trennen. Die tatsächlichen Intel-Worker und deren Auslastung ermitteln; die Windows-Budgets nicht einfach auf andere Runner übertragen. Den M4-Pro-Lauf anhand seines Protokolls zuordnen.

**Ergebnis:** eine kompakte Testlandkarte und Laufzeittabelle für vergleichbare Umfänge. Fehlende Messungen benennen und gegebenenfalls einen gesondert beauftragbaren Messlauf vorschlagen. Keine neuen Benchmarks während der reinen Bestandsaufnahme.

### 6.2. <a id="zuschnitt-und-reihenfolge-entscheiden"></a>Zuschnitt und Reihenfolge entscheiden

**P2**

Die Maßnahmen nach erwarteter Wirkung, Aufwand, Abdeckungsrisiko und Pflegeaufwand ordnen. Für jede vorgeschlagene Änderung angeben, welche Arbeit tatsächlich entfällt und welcher Nachweis erhalten bleibt. Laufzeiteinsparung und kürzere Wartezeit durch mehrere Runner getrennt bewerten.

Den nachfolgenden Drei-/Vier-Gruppen-Vergleich anhand der Testlandkarte bestätigen oder korrigieren. Zusätzliche Vorbereitung, Cache-Transfers, verfügbare Runner, Warteschlangen und gesamte Runner-Minuten einrechnen. Konkrete Umsetzung und Erfolgskriterien der nächsten Stufe vor dem Start festhalten.

**Ergebnis:** eine entschiedene Reihenfolge und ein begrenzter Pilotumfang. Eine GCC-Reduktion bleibt eine gesonderte Entscheidung unter O1.

### 6.3. <a id="toolcache-und-fortschritt-absichern"></a>Toolcache und Fortschritt absichern

**P3**

Prüfen, wie vollständig verifizierte Tools bereits nach erfolgreicher Vorbereitung gespeichert werden können, auch wenn spätere Tests abbrechen. Cache-Schlüssel, Inhalt, Integritätsprüfung und Zuständigkeit für das Speichern ausdrücklich bestimmen; keine unvollständigen Installationen als gültigen Cache veröffentlichen.

Die Fortschrittsanzeige so konkretisieren, dass eine noch laufende letzte Stufe nicht wie abgeschlossene Arbeit aussieht. Erwartete Gewichte, tatsächlich abgeschlossene Stufen und Endergebnis klar unterscheiden; keine ungemessene Restzeit versprechen.

**Abnahme:** ein späterer Testfehler erhält verwertbare Vorbereitung und Diagnose. Ein Warmcache-Nachweis muss einen tatsächlichen Restore zeigen. PASS, FAIL und Abbruch bleiben eindeutig.

### 6.4. <a id="mehrfachausführungen-bewerten-und-reduzieren"></a>Mehrfachausführungen bewerten und reduzieren

**P4**

Schwerpunkt sind [515](../../scripts/_515_test_logging_features.sh), [540](../../scripts/_540_test_go.sh) und [550](../../scripts/_550_test_go_coverage.sh). 515 besitzt gezielt ausgewählte CE/SL- und Demo-Prüfungen sowie eigene Feature-Beispiele. 540 und 550 führen normale Pakete aus, 550 mit globaler Coverage-Instrumentierung. Diese Unterschiede zunächst erhalten und die tatsächlich identischen teuren Ausführungen aus P1 benennen.

Erst dann entscheiden, ob identische Integrationsergebnisse wiederverwendet oder Prüfungen anders zugeordnet werden. Die normale und die instrumentierte Go-Ausführung nicht ohne begründete Entscheidung gleichsetzen. Auch wiederholte Arbeit zwischen eigenständigem Coverage-Workflow, Portabilitäts-Workflow und nativer Suite bewerten.

**Abnahme:** Vorher-/Nachher-Zuordnung aller betroffenen Pflichtprüfungen; dieselben Fehler werden weiterhin erkannt. Benannte PASS-Nachweise, fehlende Voraussetzungen, leere Testauswahl und SKIP werden weiter geprüft. Der vereinbarte Coverage-Umfang bleibt vollständig.

### 6.5. <a id="pc-ausführungskosten-verringern"></a>PC Ausführungskosten verringern

**P5**

In [triceCheck.c](../../_test/testdata/triceCheck.c) und dem [CGO-Harness](../../_test/testdata/cgoPackage_test.go) ermitteln, wo Einzelzeilen besonders viele Logger- oder Prozessstarts verursachen. Bestehendes `auto` und vorhandene Bulk-Pfade berücksichtigen; insbesondere verbleibende `combinedMode`- und ungeframte Pfade untersuchen.

Nur kompatible Erwartungen weiter bündeln. Content-basierte [Cache-Overlays](../../scripts/pc_cache_overlay.go), unterschiedliche C-Konfigurationen und Source-Zuordnung erhalten. Kompilierungszeit und Ausführungszeit getrennt vergleichen.

**Abnahme:** absichtlich ausgelöste Fehler bleiben auf die ursprüngliche Zeile zurückführbar. Bulk-FAIL behält FAIL, auch wenn die zeilenweise Diagnose besteht. Unterstützte und nicht bündelbare Kanäle sowie Direct-, Deferred- und Combined-Verhalten bleiben abgedeckt.

### 6.6. <a id="vorhandene-worker-parallelisierung-abstimmen"></a>Vorhandene Worker Parallelisierung abstimmen

**P6**

Für [L432](../../examples/L432_inst/all_configs_build.sh) und den [PC-Worker](../../scripts/_160_pc_target_test_worker.sh) anhand repräsentativer, gesondert beauftragter Messungen sinnvolle Budgets vergleichen. CPU, RAM, Datenträger, Gesamtzeit und Abbruchverhalten erfassen. Äußere Worker und innere Build-Parallelität gemeinsam betrachten; nicht mehrere Parallelisierungsebenen unkontrolliert multiplizieren.

Unterschiedliche Konfigurationen benötigen getrennte Ausgaben. Compiler- und ID-Workflows bleiben im selben Checkout nacheinander, solange Quellen gemeinsam verändert und restauriert werden. Ein größeres Worker-Budget ist nur bei gemessener Verbesserung ein Fortschritt.

**Abnahme:** nachvollziehbare Budgets für Windows, Linux, ARM-macOS und Intel-macOS; sichere Begrenzung und Überschreibbarkeit. Keine Ressourcenfehler, verwaisten Prozesse, beschädigten Ausgaben oder verlorene Wiederherstellung.

### 6.7. <a id="teilweise-aufteilung-in-ci-jobs-erproben"></a>Teilweise Aufteilung in CI Jobs erproben

**P7**

Nach entschiedenem Zuschnitt zunächst den vollen Intel-Lauf als begrenzten Pilot betrachten. Den lokalen Standardaufruf und die PR-Auswahl `quick` zunächst erhalten. Erst nach erfolgreichem Pilot über die Ausweitung derselben Gruppen auf die übrigen nativen CI-Ziele entscheiden.

Gruppenauswahl und Stufenliste zentral halten. Jeder Job erhält einen eigenen Checkout und eigene veränderliche Ausgaben. Nur verifizierte wiederverwendbare Tool-/Build-Caches teilen. Gruppenweise Logs und ein vollständiger Gesamtstatus gehören zum Pilot.

**Abnahme:** jede bisher erforderliche Prüfung hat einen Besitzer; keine Gruppe fehlt und kein Test wird allein durch die Aufteilung unbeabsichtigt vervielfacht. Fehlende oder abgebrochene Gruppen erlauben keinen Gesamt-PASS. Abbruch, Wiederherstellung, Artefakte und bestehende erforderliche Check-Namen beziehungsweise ihre notwendige Zuordnung sind geklärt.

### 6.8. <a id="ergebnisse-abnehmen-und-dokumentieren"></a>Ergebnisse abnehmen und dokumentieren

**P8**

Den tatsächlich umgesetzten Stand mit P1 vergleichen: Testzeit, Vorbereitung, Wartezeit, längster Job, gesamte Runner-Minuten, Cache-Hits und Fehlerdiagnose. Bei veränderter Gruppierung sind mehrere vollständige Gruppenergebnisse gemeinsam der Nachweis für den bisherigen vollständigen Umfang.

Die beauftragten finalen Läufe umfassen Windows, Linux, ARM-macOS und Intel-macOS sowie relevante `quick`-/`full`- und kalte/warme Szenarien. Die nötigen kalten/warmen Nachweise gezielt vereinbaren; nicht jede teure Kombination wahllos wiederholen. Für Intel fehlen weiterhin vollständiger Nachweis und Race-Ergebnis; die erfolgreiche LLVM-Vorbereitung allein schließt diese Lücke nicht.

**Abnahme:** dokumentierte PASS-/FAIL-/INCOMPLETE-Ergebnisse des konkreten Stands, keine unbeabsichtigte Abdeckungsänderung und weiterhin verständliche Reproduktion. Release- oder Issue-Aktionen bleiben gesondert zu beauftragen.

### 6.9. <a id="compilerabdeckung-gesondert-entscheiden"></a>Compilerabdeckung gesondert entscheiden

**O1, optional**

Den Vorschlag „volle Breite mit Clang, weniger GCC-Varianten“ erst anhand unterschiedlicher Fehlerklassen und gemessener Laufzeiten bewerten. Auf dem lokalen Windows-Lauf liegen PC-GCC mit 33 min 49 s und PC-Clang mit 31 min 08 s nahe beieinander; das begründet allein keine Reduktion.

**Ergebnis:** konkrete erhaltene und entfallende Kombinationen mit Begründung und notwendigem nativen Nachweis. Ohne ausdrückliche Entscheidung bleibt der GCC-Umfang unverändert.

## 7. <a id="anhang-c-jobaufteilung-und-technische-grenzen"></a>Anhang C Jobaufteilung und technische Grenzen

Der einfachste zusätzliche Schnitt wären drei Gruppen: allgemeine Prüfungen einschließlich Firmware/L432, PC-GCC und PC-Clang. Beim gemessenen Windows-Lauf enthält die erste Gruppe jedoch bereits **145 min 16 s** an Stufenzeit. Die beiden anderen Gruppen benötigen rund 34 und 31 Minuten. Drei Jobs beseitigen diese Unwucht nicht.

Als vorläufige Empfehlung deshalb vier feste fachliche Gruppen vergleichen:

Gruppe   | Zugeordnete Stufen im vollen Lauf                 | Windows lokal | Debian lokal | Intel laut Übergabe
-------- | ------------------------------------------------- | ------------- | ------------ | ---------------------------
checks   | 400–550: Konsistenz, Bind, Features, Go, Coverage | 92 min 55 s   | 8 min 33 s   | mind. 47 min 19 s
firmware | 560–620: Clang/GCC Firmware und L432              | 52 min 21 s   | 11 min 41 s  | mind. 50 min 33 s
pc-gcc   | 630 und 640: Insert/Clean und Bind mit GCC        | 33 min 49 s   | 32 min 31 s  | 57 min 35 s
pc-clang | 645: Insert/Clean und Bind mit Clang              | 31 min 08 s   | 23 min 47 s  | nach 9 min 43 s abgebrochen

Die Windows-Zeiten sind Summen der seriell gemessenen Stufen; 12 Sekunden Ablauf-/Rundungsdifferenz zur Gesamtdauer sind nicht verteilt. Die Intel-Untergrenzen enthalten nur die in der Übergabe ausgewiesenen großen Blöcke. Insbesondere die vollständige Intel-Clang-Dauer ist unbekannt.

Die Debian-Gruppen summieren sich auf 76 min 32 s, mit 1 Sekunde Differenz zur Gesamtdauer. Dort wären allgemeine Prüfungen und Firmware zusammen nur 20 min 14 s lang; im Drei- wie im Viererzuschnitt bliebe PC-GCC mit 32 min 31 s der längste Block. Der Windows-Vorteil von vier Gruppen lässt sich daher nicht unmittelbar auf Debian übertragen. Dies ist eine Auswertung serieller Stufenzeiten, kein gemessener paralleler Lauf und keine Änderung der bisherigen Empfehlung oder Freigabe.

Bei gleich leistungsfähigen, sofort verfügbaren getrennten Runnern wäre der längste bekannte Windows-Block im Viererzuschnitt etwa 93 statt 145 Minuten im Dreierzuschnitt. Das ist ein Verteilungsbeispiel, keine zugesicherte CI-Laufzeit: zusätzliche Vorbereitung, Transfers, Warteschlangen und veränderte Auslastung kommen hinzu. Nach P4 bis P6 muss die Verteilung erneut geprüft werden.

### 7.1. <a id="bedienung-und-pflege-begrenzen"></a>Bedienung und Pflege begrenzen

- Ein öffentlicher Einstieg bleibt `scripts/testAll.sh`; ohne Gruppenauswahl läuft weiterhin der ganze gewählte Umfang. Eine mögliche zusätzliche Gruppenauswahl wird in P2 spezifiziert, hier noch kein neuer CLI-Vertrag festgelegt.
- Die Gruppen leiten sich aus derselben zentralen Stufenliste ab. Keine mehrfach gepflegten Skriptlisten in YAML und Shell, keine eigene Pipeline pro Konfiguration.
- `quick`/`full` und die Gruppen sind getrennte Entscheidungen. Der erste Pilot erweitert nicht automatisch PR-Matrix oder Testumfang.
- GCC- und Clang-PC-Gruppen führen ihre ID-Workflows intern weiterhin geordnet aus. Der Firmware-Job besitzt die 101 L432-Konfigurationen; jede volle PC-Compiler-/ID-Kombination behält ihre 63 Konfigurationen.
- Coverage und die bisherige Race-Nachprüfung erhalten je einen ausdrücklichen Besitzer. Eine Aufteilung erzeugt nicht vier vollständige Go-Coverage- oder Race-Läufe.
- Vorbereitung und Cache-Nutzung möglichst wiederverwenden. Einen zusätzlichen zentralen Vorbereitungsjob nur einführen, wenn er messbar hilft und die Abhängigkeiten vereinfacht; zunächst kein kompliziertes Artefakt-Verteilnetz aufbauen.
- Artefaktnamen unterscheiden Runner, Architektur, Umfang und Gruppe. Der Gesamtstatus erklärt, welche Gruppen vollständig sind; einzelne erfolgreiche Uploads gelten nicht als Test-PASS.
- Der Pilot muss auf den bisherigen seriellen Ablauf zurückstellbar bleiben, ohne Tests oder Diagnosen zu entfernen.

### 7.2. <a id="verbindliche-grenzen"></a>Verbindliche Grenzen

- Zunächst den vorhandenen Testumfang erhalten: `quick` und `full`, GCC und Clang, Insert/Clean und Bind, relevante C/C++-, Kanal- und Ausgabevarianten, Go-Coverage und Race-Prüfung.
- Keine Tests stillschweigend streichen, Pflichtprüfungen in SKIP umwandeln oder Limits erhöhen, um einen grünen Lauf zu erhalten.
- Ein FAIL oder INCOMPLETE bleibt fehlgeschlagen beziehungsweise unvollständig, auch wenn ein späterer Diagnoselauf besteht.
- Jede Diagnose muss Compiler, ID-Workflow, Konfiguration und betroffene Quellzeile oder Erwartung erkennen lassen. Originalausgabe und reproduzierbarer Aufruf bleiben verfügbar.
- Veränderliche Quellen, generierte Dateien, Build-Ausgaben und Zustandswiederherstellung dürfen nicht gleichzeitig mehreren Besitzern gehören. Vorhandene fremde Testläufe und Sperren bleiben geschützt.
- Tatsächlichen Testhost und simuliertes Ziel unterscheiden. Native Windows-, Linux- und macOS-Ergebnisse werden getrennt von Simulationen ausgewiesen; Intel und ARM-macOS sind eigene Ausführungsziele.
- Die neueste stabile Go-Version bleibt Vorgabe. Die pro Lauf gewählte Version wird protokolliert und innerhalb des Laufs beibehalten. Versionswechsel zwischen Messungen werden als Vergleichsgrenze ausgewiesen.
- Kein Refactoring von Produktcode oder Umbau von Toolchains als Nebenarbeit. #786, Release v2.0.1, Tags und Issue-Abschluss sind keine automatischen Folgen dieses Plans.

## 8. <a id="anhang-d-entscheidungen-und-fortschritt"></a>Anhang D Entscheidungen und Fortschritt

Punkt                   | Festlegung oder Vorschlag                                     | Status
----------------------- | ------------------------------------------------------------- | -----------------------------------
Arbeitsgrundlage        | Dieser Plan einschließlich vollständiger Übergabe in Anhang E | Festgelegt
Bisherige Übergabedatei | Inhaltlich vollständig übernommen; kann separat entfallen     | Original noch unverändert vorhanden
CI Zuschnitt            | Vier feste Gruppen; drei Gruppen als Vergleich                | Nach P1/P2 offen
PR Umfang               | quick zunächst unverändert lassen                             | Kein Ausbau beauftragt
Compilerabdeckung       | GCC und Clang zunächst vollständig erhalten                   | Reduktion nur über O1
Start der Umsetzung     | Nur ausdrücklich beauftragte Stufen bearbeiten                | Noch keine Stufe freigegeben

Nach jeder beauftragten Stufe hier kurz nachtragen: Status und Ergebnis, Entscheidungen, geprüften Commit beziehungsweise Diff, tatsächliche native Prüfungen je Plattform, getrennte Simulationen und noch offene Nachweise. Messungen verschiedener Stufen nicht ungeprüft als addierbare Einsparungen behandeln.

Die bisherige Datei `Codex_Handover.md` kann nach Prüfung der vollständigen Übernahme gelöscht werden; dieser Plan benötigt ihren separaten Inhalt nicht mehr. Sie wurde in dieser Planungsrunde nicht verändert oder gelöscht. `AGENTS.md` verwendet den Dateinamen weiterhin als allgemeines Ziel für künftige Rechnerwechsel; dann genügt eine neue kurze Übergabe, die für diese Aufgabe auf den aktuellen Plan verweist.

## 9. <a id="anhang-e-vollständige-ursprüngliche-übergabe"></a>Anhang E Vollständige ursprüngliche Übergabe

Der folgende Text ist die vollständige, unveränderte Übergabe vom 2026-11-10, lediglich als Zitat formatiert. Angaben zu damaligem HEAD, uncommitteten Dateien, Arbeitsstopps und nächsten Schritten beschreiben den damaligen Rechnerwechsel. Der aktuelle Arbeitsstand und die Freigaben richten sich nach dem Hauptteil und dem Fortschritt dieses Plans.

> # Übergabe: CI-Laufzeiten und Vorbereitung von v2.0.1
>
> Stand: 2026-11-10. Jetzt nur den Rechnerwechsel vorbereiten; die folgenden Optimierungen weder weiter untersuchen noch umsetzen. Sie sind offene Fragen, keine beschlossenen Änderungen.
>
> ## Zuerst nach dem Rechnerwechsel
>
> 1. Branch, HEAD und Arbeitsbaum mit diesem Stand vergleichen; die uncommitteten Änderungen müssen mit übertragen werden.
> 2. Nach Aufforderung die tatsächlichen Zeitfresser und Testwiederholungen anhand der vorhandenen CI-Protokolle untersuchen. Vor Änderungen einen konkreten Vorschlag mit erwarteter Zeitersparnis, verbleibender Testabdeckung und verständlicher Fehlerdiagnose vorlegen.
> 3. Erst danach über Umsetzung und den noch fehlenden vollständigen Intel-Nachweis für #786 entscheiden. Keine Tests streichen oder Zeitlimits erhöhen, nur um einen grünen Lauf zu erhalten.
>
> ## Git und lokale Änderungen
>
> - Branch: `wip`; HEAD: `3719635e40372b5fa5ca18ed225fb685351781bb` — `Strengthen cross-platform implementation and validation rules`.
> - Lokaler Trackingstand: keine Commits vor `origin/wip`; dessen tatsächlicher Remote-Stand wurde für diese Übergabe nicht erneut abgefragt.
> - Bereits uncommittet: [CHANGELOG.md](../../CHANGELOG.md), vorbereitete v2.0.1-Notizen zu `-logLevel` mit `-pick`/`-ban`, Windows-Eingabeabbruch und Buslock-Retry sowie LLVM-, Pandoc- und Go-Vorbereitung. Diese Änderung erhalten.
> - Neu und ebenfalls uncommittet: diese Übergabe. Für den Rechnerwechsel beide Dateien manuell hinzufügen, committen und übertragen, üblicherweise per Push. Hier wurde nichts gestagt, committet oder gepusht.
> - Vor der Übergabe keine weiteren sichtbaren Änderungen oder ungetrackten Dateien; kein `temp/testAll.lock` vorhanden. Keine Testquellen temporär umgeschrieben. Keine lokalen Dateien außerhalb von Git für die Fortsetzung erforderlich; heruntergeladene CI-Protokolle können erneut bezogen werden.
>
> ## Gesicherter Befund zum CI-Abbruch
>
> Untersucht: `rokath/trice`, Actions-Run `42682343023`, Commit `dd15c207c0b17a1acac1afebf153af6b1c8aac63` auf `main`. Sein Git-Tree entspricht dem lokalen HEAD. Intel-Job: `129379957725`; Artefakt: `13715653254`, `native-macos-15-intel-full`.
>
> - Der Projekt-Toolcache fehlte. Trotzdem war `Prepare prebuilt LLVM on Intel macOS` nach **26 Sekunden erfolgreich**. Das ursprüngliche LLVM-Kompilierungsproblem von #786 ist damit im echten kalten Intel-Lauf behoben.
> - `bash scripts/testAll.sh full`: auf Intel **unvollständig**, weil der gesamte Job nach den konfigurierten 180 Minuten abgebrochen wurde. Stufen 1–24: PASS. Stufe 25, `_645_test_pc_targets_clang.sh`: nach 9 min 43 s abgebrochen. Vier Konfigurationen liefen gerade; kein konkreter Testfehler war bis dahin protokolliert. Ein Hänger ist damit nicht nachgewiesen.
> - Derselbe vollständige Lauf bestand nativ auf Linux, Windows und ARM-macOS. Gesamte Jobdauer einschließlich Vorbereitung: ungefähr 88, 115 und 118 Minuten. Intel-Race-Nachprüfung wegen Abbruch übersprungen. Kein vollständiger oder warmer Intel-PASS vorhanden.
> - Die Fortschrittsanzeige zeigt bereits beim Start der letzten Stufe 100 %. Sie beschreibt keine tatsächlich abgeschlossene Arbeit. Die letzte Stufe enthält zwei vollständige PC-Matrizen.
> - `Post Restore verified project tools` wurde auf Intel übersprungen; der neue Projekt-Toolcache wurde nicht gespeichert. Ein bloßer Wiederholungslauf ist daher kein verlässlicher Warmcache-Test. Der Artefakt-Upload war erfolgreich und beweist keinen Test-PASS.
>
> Große Zeitblöcke auf Intel:
>
> Bereich                                       | Dauer
> --------------------------------------------- | ----------------------
> Featuretests, normale Go-Tests und Coverage   | 47 min 19 s
> GCC-Firmwaretests                             | 21 min 25 s
> 101 L432-Konfigurationen                      | 29 min 08 s
> 63 PC-Konfigurationen, GCC mit `insert`       | 26 min 48 s
> 63 PC-Konfigurationen, GCC mit `bind`         | 30 min 47 s
> PC-Clang mit `insert` und anschließend `bind` | 9 min 43 s bis Abbruch
>
> Die großen Teststufen laufen im Job nacheinander. Innerhalb einzelner Matrizen existiert bereits Parallelisierung; deren tatsächliche Auslastung ist noch nicht untersucht.
>
> ## Offene Untersuchungen und Überlegungen des Nutzers
>
> - **Unnötige doppelte Tests:** `_515_test_logging_features.sh`, `_540_test_go.sh` und `_550_test_go_coverage.sh` führen teilweise dieselben aufwendigen Demo- und Compiler-Integrationstests aus. Notwendige Unterschiede, etwa Coverage-Instrumentierung oder zusätzliche Varianten, von vermeidbaren Wiederholungen unterscheiden. Verbesserungen vorschlagen, ohne dabei unbemerkt Testabdeckung zu verlieren.
> - **Einzelzeilen gegen Bulk:** Der Nutzer vermutet einen wesentlichen Zeitanteil bei den vielen Einzelzeilentests in „checkTrice.c“. Der tatsächliche Dateiname ist [triceCheck.c](../../_test/testdata/triceCheck.c). Kompilierung, Logger-Neustarts pro Erwartung und eigentliche Testausführung getrennt betrachten. Bulk ist bereits vorhanden: Der PC-Worker verwendet `TRICE_PC_TEST_MODE=auto`; unterstützte gerahmte Kanäle werden gebündelt, andere Pfade bleiben zeilenweise. Besonders die verbleibenden `combinedMode`- und ungeframten Pfade prüfen, statt einen vollständig fehlenden Bulk-Modus anzunehmen.
> - **Fehler müssen auffindbar bleiben:** Bei weiterer Bündelung muss ein FAIL die betroffene Konfiguration, Compiler-/ID-Variante und die ursprüngliche Quellzeile bzw. Erwartung leicht erkennen lassen. Bestehende Bulk-Diagnose mit Quellzeilenzuordnung und Originalausgabe sowie den zeilenweisen Diagnoselauf bei Bulk-FAIL erhalten oder gezielt verbessern. Ein erfolgreicher Diagnoselauf darf einen zuvor fehlgeschlagenen Bulk-Test nicht nachträglich als PASS werten.
> - **L432 parallel:** Prüfen, ob sich die ungefähr 100 Konfigurationen besser parallel ausführen lassen. Bereits bekannt: `all_configs_build.sh` besitzt eine äußere Parallelisierung mit `TRICE_L432_TEST_JOBS`, während der einzelne Build `make -j1` nutzt. Tatsächliche Jobzahl, Engpässe und mögliche gemeinsame Ausgaben im Intel-Lauf sind noch zu prüfen.
> - **Clang und GCC parallel:** Getrennte CI-Jobs oder anderweitig isolierte Arbeitsverzeichnisse erwägen. Die ID-Workflows ändern und restaurieren Quellen; sie dürfen nicht gleichzeitig denselben Checkout oder veränderliche Build-Ausgaben benutzen.
> - **GCC-Testumfang reduzieren:** Nutzerüberlegung: volle Bandbreite mit Clang, weniger Varianten mit GCC, weil Clang aus seiner Sicht schneller und neuer ist. Das ist noch nicht beschlossen. Anhand gemessener Laufzeiten und tatsächlich unterschiedlicher Compilerabdeckung bewerten; keine GCC-Tests ohne Entscheidung streichen.
> - **Weitere bisherige Vorschläge:** Den vollständigen Lauf auf allgemeine Tests/Firmware, PC-GCC und PC-Clang verteilen; verifizierte Tools schon nach erfolgreicher Vorbereitung speichern; Fortschrittsanzeige korrigieren. Ebenfalls noch nicht umgesetzt.
>
> ## Relevante Dateien und Grenzen
>
> - CI: [native-tests.yml](../../.github/workflows/native-tests.yml), [native-tests-reusable.yml](../../.github/workflows/native-tests-reusable.yml). PRs verwenden `quick`; Intel läuft zusätzlich bei Nicht-PR-Ereignissen mit `full`.
> - Ablauf und Wiederholungen: `scripts/_110_test_runner.sh`, `_515_test_logging_features.sh`, `_540_test_go.sh`, `_550_test_go_coverage.sh`.
> - PC-Matrizen: `scripts/_160_pc_target_test_worker.sh`, `_630_test_pc_targets_insert.sh`, `_640_test_pc_targets_bind.sh`, `_645_test_pc_targets_clang.sh`; `scripts/pc_cache_overlay.go`; `_test/testdata/cgoPackage.go` und `cgoPackage_test.go`.
> - L432: `scripts/_620_test_l432_configs.sh`, `examples/L432_inst/all_configs_build.sh`.
> - #785 hatte erfolgreiche Windows-Nachweise; #786 hat jetzt den kalten LLVM-Nachweis, aber weiterhin keinen vollständigen Intel-PASS. Issue-Status vor einer späteren Abschlussaktion prüfen. Kein Issue in dieser Sitzung geschlossen, kein Release oder Tag angelegt.
> - Während dieser Untersuchung nur vorhandene CI-Protokolle und Quelltexte gelesen; keine neuen lokalen Tests, Coverage- oder Compilerläufe gestartet. Die Übergabe ist keine Freigabe für solche Abschlussläufe oder für CI-Änderungen. Die finale Validierung übernimmt der Nutzer, sofern er sie nicht ausdrücklich beauftragt.
> - `AGENTS.md` beachten: Änderungen gezielt halten; alle drei Betriebssysteme berücksichtigen; vorhandene laufende Tests und ihre Zustandswiederherstellung schützen. Ein schlichtes `commit` verlangt nur kopierbare Befehle, keine Ausführung.

