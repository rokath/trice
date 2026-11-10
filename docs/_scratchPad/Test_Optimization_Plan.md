# Test Optimierungsplan

Stand: 2026-11-10. Status: Die drei Logarchive sind verglichen; S1 mit 360 Minuten ist umgesetzt und fokussiert geprüft. S2 und die Optimierungen warten auf eigene Startkommandos.

Zuerst vollständige, fehlerfreie Testläufe auf Windows, Linux und macOS sichern. Dafür zunächst einen kleinen CI-Fix umsetzen und abnehmen. Erst danach Laufzeiten und Logablage verbessern. Testumfang, verständliche Fehlerdiagnosen und einfache Bedienung bleiben erhalten.

<h2>ToC</h2>
<!-- mdtoc -->

- [1. Hinweis](#hinweis)
- [2. Empfehlung](#empfehlung)
- [3. Abarbeitungsstufen](#abarbeitungsstufen)
- [4. Nächster Auftrag und Freigabe](#nächster-auftrag-und-freigabe)
- [5. Anhang A Messdaten und Vergleich](#anhang-a-messdaten-und-vergleich)
  - [5.1. Bisherige Ausgangsbefunde](#bisherige-ausgangsbefunde)
  - [5.2. Wichtige Details des Windows Laufs](#wichtige-details-des-windows-laufs)
  - [5.3. Vergleich der drei lokalen Läufe](#vergleich-der-drei-lokalen-läufe)
  - [5.4. CI-Status und Issue #786](#ci-status-und-issue-786)
- [6. Anhang B Stufendetails](#anhang-b-stufendetails)
  - [6.1. Stabilisierung vor Optimierung](#stabilisierung-vor-optimierung)
  - [6.2. Logablage und Archivierung vereinfachen](#logablage-und-archivierung-vereinfachen)
  - [6.3. Messdaten und Testabdeckung erfassen](#messdaten-und-testabdeckung-erfassen)
  - [6.4. Zuschnitt und Reihenfolge entscheiden](#zuschnitt-und-reihenfolge-entscheiden)
  - [6.5. Toolcache und Fortschritt absichern](#toolcache-und-fortschritt-absichern)
  - [6.6. Mehrfachausführungen bewerten und reduzieren](#mehrfachausführungen-bewerten-und-reduzieren)
  - [6.7. PC Ausführungskosten verringern](#pc-ausführungskosten-verringern)
  - [6.8. Vorhandene Worker Parallelisierung abstimmen](#vorhandene-worker-parallelisierung-abstimmen)
  - [6.9. Teilweise Aufteilung in CI Jobs erproben](#teilweise-aufteilung-in-ci-jobs-erproben)
  - [6.10. Ergebnisse abnehmen und dokumentieren](#ergebnisse-abnehmen-und-dokumentieren)
  - [6.11. Compilerabdeckung gesondert entscheiden](#compilerabdeckung-gesondert-entscheiden)
- [7. Anhang C Jobaufteilung und technische Grenzen](#anhang-c-jobaufteilung-und-technische-grenzen)
  - [7.1. Bedienung und Pflege begrenzen](#bedienung-und-pflege-begrenzen)
  - [7.2. Verbindliche Grenzen](#verbindliche-grenzen)
- [8. Anhang D Entscheidungen und Fortschritt](#anhang-d-entscheidungen-und-fortschritt)
- [9. Anhang E Vollständige ursprüngliche Übergabe](#anhang-e-vollständige-ursprüngliche-übergabe)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

## 1. <a id="hinweis"></a>Hinweis

Für geräteübergreifende Vergleiche soll künftig jeder Testlauf einen festen Ordner `temp/log/testAll/` mit festen Unterordnern, etwa `l432/`, verwenden. Auch intern sollen die Lognamen ohne Zufallsteile auskommen. Nach jedem Lauf, einschließlich FAIL oder Abbruch, soll ein ZIP mit Datum, Uhrzeit einschließlich Sekunden und Betriebssystem im Namen entstehen, etwa `2026-11-10_18-33-12_macos_full.zip`. Die zugehörige Konsolenausgabe gehört als Überblick hinein.

Vor dem nächsten Lauf sollen die bisherigen entpackten Logs entfernt werden; vorhandene ZIPs bleiben erhalten. Aufräumen erst nach abgeschlossener Zustandswiederherstellung und erfolgreicher Archivierung, ohne aktive Läufe anzutasten. Diese Änderung ist als **L1 nach der Stabilisierung** vorgemerkt; der erste CI-Fix ändert keine Logpfade und löscht keine Dateien.

## 2. <a id="empfehlung"></a>Empfehlung

**Zunächst S1 und S2: den bestehenden vollständigen Ablauf mit ausreichendem, begrenztem CI-Zeitbudget abschließen lassen.** Der lokale Windows-Lauf benötigt bereits 210 min 25 s reine Testzeit. Im Nachtlauf vom 2026-11-10 erreichen inzwischen Windows und Intel-macOS das 180-Minuten-Limit, jeweils erst in der letzten Clang-Matrix; die ersten 24 Stufen bestehen. Ein Testumbau ist daraus nicht als notwendiger erster Fix abzuleiten.

**S1, vom Nutzer freigegeben und umgesetzt:** Nur `full` auf Windows und Intel-macOS erhält **360 statt 180 Minuten** Jobbudget. `quick` sowie Linux und ARM-macOS behalten ihr bisheriges Budget. Grundlage sind die gemessenen mehr als 210 Minuten unter Windows und die Intel-Läufe mit bis zu rund 170 Minuten vor Beginn der letzten Matrix. Die größere Reserve berücksichtigt langsamere Runner sowie die noch nicht vollständig gemessene Intel-Matrix, Vorbereitung und Race-Prüfung. S2 muss zeigen, ob der vollständige Ablauf tatsächlich endet. Bei einem Hänger oder Testfehler dessen Ursache beheben.

GitHub begrenzt Jobs auf gehosteten Runnern auf **sechs Stunden, also 360 Minuten**. Ein eingetragenes Limit von 600 Minuten würde diese Grenze nicht erhöhen. Die Grenze gilt für den gesamten Job einschließlich Vorbereitung und Nachprüfungen. Quellen: [Actions limits](https://docs.github.com/en/actions/reference/limits) und [Job timeout](https://docs.github.com/en/actions/reference/workflows-and-actions/workflow-syntax#jobsjob_idtimeout-minutes). Ein größeres Limit verlängert erfolgreiche Läufe nicht, lässt festhängende Jobs aber länger Ressourcen belegen. Die kürzeren internen Test-Timeouts bleiben bestehen.

Dieser Übergangsfix spart keine Rechenzeit. Er vermeidet zunächst einen größeren Umbau von Jobs, Harness und Compilerabdeckung. Testauswahl, Pflichtprüfungen, bestehende interne Zeitlimits, Fehlerstatus, Wiederherstellung und Check-Namen bleiben erhalten. Cache-Umbau, Fortschrittsanzeige, Logablage, Bulk-Ausbau und weniger GCC-Varianten gehören nicht zu S1.

**Nach erfolgreichen vollständigen Läufen** L1 und die Optimierungen P3–P7 einzeln angehen. Die drei lokalen Archive zeigen unterschiedliche Schwerpunkte: Windows 210 min 25 s, ARM-Mac 90 min 29 s, Debian 76 min 33 s. Unter Windows und macOS dominieren die Skript-/Go-Prüfungen, unter Debian die PC-Matrizen. Der Vier-Gruppen-Vorschlag in Anhang C bleibt eine spätere Option und ist kein erster Implementierungsauftrag.

Ein gemeinsamer Einstieg und eine zentrale Stufenliste bleiben erhalten. Lokale Standardbedienung und PR-Auswahl `quick` ändern sich durch den Stabilisierungsfix nicht.

Dieser Plan ist die **alleinige Arbeitsgrundlage für diese Optimierungsaufgabe**. Er enthält die vollständige bisherige Übergabe als historischen Text in Anhang E. Die Regeln in [AGENTS.md](../../AGENTS.md) gelten weiterhin.

## 3. <a id="abarbeitungsstufen"></a>Abarbeitungsstufen

Stufe | Aufgabe                                  | Ergebnis
----- | ---------------------------------------- | ----------------------------------------------
S1    | Begrenzten CI-Zeitbudget-Fix umsetzen    | Bestehender vollständiger Ablauf erhält Zeit
S2    | Vollständige native Läufe abnehmen       | PASS einschließlich Wiederherstellung und Race
L1    | Logablage und Archivierung vereinfachen  | Feste Logpfade, ZIP und Konsolenüberblick
P1    | Messdaten und Testabdeckung erfassen     | Vergleichbare Basis und Testlandkarte
P2    | Maßnahmen und Jobzuschnitt entscheiden   | Prioritäten und begrenzter Pilot
P3    | Toolcache und Fortschritt absichern      | Verlässliche Vorbereitung und Statusanzeige
P4    | Mehrfachausführungen reduzieren          | Weniger Wiederholung bei belegter Abdeckung
P5    | PC Ausführungskosten verringern          | Weniger Starts oder Builds; gleiche Diagnose
P6    | Bestehende Worker abstimmen              | Gemessene Budgets für L432 und PC
P7    | Teilweise Aufteilung in CI Jobs erproben | Isolierte Gruppen mit gemeinsamem Gesamtstatus
P8    | Ergebnisse abnehmen                      | Messvergleich und native Plattformnachweise
O1    | Compilerabdeckung gesondert bewerten     | Optional; keine automatische GCC-Reduktion

**Reihenfolge:** zuerst S1, danach S2. Bis S2 erfolgreich ist, keine Optimierungsstufe beginnen; weitere echte Testfehler zunächst einzeln beheben. Der Logvergleich aus P1 und der Reihenfolgevorschlag aus P2 sind jetzt dokumentiert. Die vollständige Testlandkarte und die Auswahl späterer Optimierungen bleiben offen. Danach L1 und jeweils eine ausgewählte Optimierung bearbeiten; P7 benötigt einen eigenen Auftrag. Details stehen in Anhang B.

## 4. <a id="nächster-auftrag-und-freigabe"></a>Nächster Auftrag und Freigabe

**S1 ist umgesetzt und fokussiert geprüft.** Der nächste Auftrag ist S2: vollständige native CI-Läufe starten oder die vom Nutzer gestarteten Läufe auswerten. Dafür auf ein eigenes Startkommando warten. Keine Jobaufteilung oder andere Optimierung vorziehen.

S2 benötigt vollständige native Nachweise des geänderten Stands, insbesondere Intel mit kaltem und warmem Toolcache. Erst bei erfüllten Abnahmekriterien den nächsten Optimierungsschritt auswählen.

Planprüfung und vergleichende Loganalyse sind erledigt. Für S1 wurden der Workflow und sein gezielter Regressionstest geändert; vollständige Testläufe, Benchmarks und CI-Dispatch wurden nicht gestartet. Jede weitere Umsetzungsstufe wartet auf ihr eigenes Startkommando. „P4 konkretisieren“ bleibt Planarbeit; „P4 umsetzen“ beauftragt diese Stufe. Teststreichungen, Compilerabdeckung, vollständige Abschlussläufe, Git-Ausführung und Release-/Issue-Aktionen benötigen ihre ausdrückliche Beauftragung.

## 5. <a id="anhang-a-messdaten-und-vergleich"></a>Anhang A Messdaten und Vergleich

### 5.1. <a id="bisherige-ausgangsbefunde"></a>Bisherige Ausgangsbefunde

Ausgangsstand vor S1: `wip`, HEAD `2d58d4ea92b56884013ba31259bb6a78592c0445`. Bis dahin sind seit `3719635e` im getrackten Vergleich nur Changelog und Scratchpad-Dokumente/-Archive hinzugekommen beziehungsweise geändert; Testcode und CI waren unverändert. S1 liegt als noch uncommitteter Workflow-/Test-Diff darüber, siehe Anhang D.

Verbindliche Infobasis sind diese drei ZIPs mit jeweils **412 Dateien**, darunter Zusammenfassung, `results.json`, `steps.tsv`, Go-JSON-/Build-Protokolle, `coverage.out`, **252 PC- und 101 L432-Konfigurationslogs**. Relative Pfade und Originalinhalte sind erhalten. Generierte Quellen, Cache-Inhalte, Binärdateien und Zustandssnapshots sind ausgeschlossen.

Lauf und Archiv                                              | Testzeit     | Vorbereitung | Ergebnis
------------------------------------------------------------ | ------------ | ------------ | --------------------------------------
[Windows, 2026-11-09](test_logs/2026-11-09_windows_full.zip) | 210 min 25 s | 1 min 08 s   | 25 Stufen und Zustandsvergleich PASS
[ARM-Mac, 2026-11-09](test_logs/2026-11-09_macos_full.zip)   | 90 min 29 s  | 16 min 51 s  | 25 Stufen und Zustandsvergleich PASS
[Debian, 2026-11-10](test_logs/2026-11-10_debian_full.zip)   | 76 min 33 s  | 1 min 41 s   | 25 Stufen PASS; Zustandsvergleich FAIL

Commit-Zuordnung: macOS `3719635e`; Windows `tools.log` nennt `3719635e`, `results.json` nennt `dd15c207`. Beide Commits haben denselben Git-Tree. Debian nennt in beiden Dateien `f713ceab`; `dirty: true`, mit nachgewiesener Änderung dieser Plandatei während des Laufs. Gegenüber `3719635e` unterscheiden sich auch dort nur Changelog und Plan. Die drei Archive sind somit für den getrackten Testcode vergleichbar; Betriebssystem, Compiler, Worker und Cachebedingungen unterscheiden sich weiterhin.

Windows-Aufruf laut bisherigem Nachweis: `./scripts/testAll.sh --no-format`; Debian `./scripts/testAll.sh` mit abgelehnter Formatierung. Der genaue Mac-Aufruf ist im Archiv nicht festgehalten; Umfang `full`, Commit und Zeiten sind durch `results.json` und Zusammenfassung belegt.

Die drei Windows-Stufen 515, 540 und 550 benötigen zusammen **5.445 s beziehungsweise 90 min 45 s**, rund **43,1 %** der Testzeit: 11 min 49 s Features, 38 min 56 s normale Go-Tests und 40 min Coverage. Daraus folgt ein Untersuchungsbedarf, aber keine bereits nachgewiesene Einsparung in dieser Höhe.

Auf Windows liefen L432 bereits mit **8 äußeren Workern** und `make -j1`, die PC-Matrizen mit **4 Workern** und `TRICE_PC_TEST_MODE=auto`. Eine fehlende Parallelisierung oder ein vollständig fehlender Bulk-Modus ist daher keine zutreffende Ausgangsannahme.

Die Übergabe dokumentiert für CI vollständige PASS-Läufe auf Linux, Windows und ARM-macOS mit ungefähr 88, 115 und 118 Minuten einschließlich Vorbereitung. Intel erreichte im Lauf `42682343023`, Job `129379957725`, das 180-Minuten-Limit. Die kalte Intel-LLVM-Vorbereitung bestand nach 26 Sekunden; das ersetzt keinen vollständigen Intel-PASS. Der nachfolgende Race-Check und das Speichern des neuen Projekt-Toolcaches blieben dort aus.

Lokale Testzeit und CI-Jobdauer werden getrennt verglichen. Nur Windows und Debian liefen laut Nutzer auf derselben Dual-Boot-Hardware; der ARM-Mac ist ein anderer Rechner. Die vollständigen Mac-Zahlen ersetzen jetzt die bisherige Schätzung.

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

### 5.3. <a id="vergleich-der-drei-lokalen-läufe"></a>Vergleich der drei lokalen Läufe

Der Nutzer hat am **2026-11-10 unter Debian auf derselben Hardware wie beim Windows-Lauf** gemessen. Quelle sind jetzt das verlinkte Archiv und der bereits übermittelte Konsolenauszug; Aufruf `./scripts/testAll.sh`, angezeigter Umfang `full`, Host `Linux x86_64`. Formatierung wurde mit `N` abgelehnt. Start **12:02:16 GMT**, Ende **13:18:49 GMT**, reine Testzeit **4.593 s = 76 min 33 s**. Vorbereitung: **101 s**, einschließlich interaktiver Auswahl und außerhalb der Testzeit; zusammen **4.694 s = 78 min 14 s**.

**Ergebnis:** alle **25 Teststufen PASS**. Der Runner meldete anschließend `Result: FAIL` und Exitstatus 1, weil sich `docs/_scratchPad/Test_Optimization_Plan.md` während des Laufs geändert hatte. Laut Nutzer wurde parallel im Scratchpad gearbeitet. Dieser abschließende Zustandsvergleich ist deshalb hier kein Hinweis auf einen Testfehler; die Wiederherstellung wird durch diesen Lauf aber auch nicht unabhängig nachgewiesen. Toolprotokoll: `temp/log/testAll.pFPVw7/tools.log`; Zustandsevidenz: `temp/log/testAll.pFPVw7/testAll-state.LIEq0R`. Diese Pfade sind lokale Nachweise und müssen nicht auf anderen Rechnern vorhanden sein.

Die oben genannte Debian-Zustandsevidenz bleibt nur lokal; die drei verlinkten Archive enthalten die auswertbaren Diagnoseprotokolle. ZIP-Integrität und enthaltene Dateilisten wurden bei dieser Auswertung für alle drei Archive geprüft.

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

Vergleich der größten Blöcke; reine Testzeiten aus `steps.tsv`, ohne Vorbereitung:

Block                    | Windows      | Debian      | ARM-Mac
------------------------ | ------------ | ----------- | -----------
Gesamte Testzeit         | 210 min 25 s | 76 min 33 s | 90 min 29 s
515: Features            | 11 min 49 s  | 2 min 18 s  | 4 min 14 s
540: Go                  | 38 min 56 s  | 2 min 44 s  | 28 min 25 s
550: Coverage            | 40 min 00 s  | 2 min 50 s  | 17 min 14 s
515 + 540 + 550          | 90 min 45 s  | 7 min 52 s  | 49 min 53 s
620: L432                | 15 min 23 s  | 7 min 12 s  | 4 min 46 s
630: PC-GCC Insert/Clean | 16 min 44 s  | 16 min 09 s | 8 min 50 s
640: PC-GCC Bind         | 17 min 05 s  | 16 min 22 s | 9 min 04 s
645: PC-Clang            | 31 min 08 s  | 23 min 47 s | 10 min 35 s

**Erste Einschätzung:** Debian benötigt auf derselben Hardware **63,6 % weniger Testzeit**, entsprechend Faktor **2,75** und einer Differenz von **133 min 52 s**. Davon entfallen **82 min 53 s** auf 515/540/550. Diese Stufen beanspruchen unter Debian nur **10,3 %** statt **43,1 %** der Testzeit. Die PC-Matrizen benötigen dagegen zusammen **56 min 18 s = 73,5 %** der Debian-Testzeit; insbesondere PC-GCC ist auf beiden Systemen fast gleich schnell. Die Unterschiede verteilen sich also stark ungleich. Das stützt eine getrennte Untersuchung von Go-/Integrationsprüfungen, Firmware und PC-Matrizen; eine konkrete Ursache oder verzichtbare Prüfung ist damit noch nicht belegt.

Auf dem ARM-Mac entfallen **55,1 %** der Testzeit auf 515/540/550 und **31,5 %** auf die PC-Matrizen. Das Paket `scripts` dauert dort in 540 **1.697,165 s** und in 550 **1.020,179 s**; unter Debian nur **160,834 s / 163,255 s**, unter Windows **2.313,622 s / 2.364,941 s**. Besonders auffällig sind auf dem Mac auch Vorbereitungstests: `TestRuntimePreparationChecksPython` benötigt **164,92 s / 60,29 s**, `TestRuntimePreparationChecksHostCompilers` **158,80 s / 158,09 s**. Die Ursache dieser Plattform- und Erst-/Folgelauf-Unterschiede ist noch nicht nachgewiesen.

**Mehrfachausführung ist belegt:** In allen drei Archiven bestehen dieselben **12 übergeordneten Tests** sowohl in 515 als auch in 540 und 550. Beispiele sind `TestHostDemoEntrypointIntegration` und `TestBindEntrypointIntegration`. Allein diese beiden benötigen unter Windows in 515 zusammen **429,38 s**. Das ist ein konkreter späterer Ansatzpunkt, aber keine bewiesene Einsparung: 515 prüft benannte Pflichtnachweise, 550 instrumentiert für Coverage, und 515 enthält zusätzlich eigene Feature-Beispiele. Nicht die gesamten 90 min 45 s sind deshalb als vermeidbare Doppelarbeit anzusetzen.

**Einzelzeilen sind teuer, Bulk existiert bereits:** In jeder Compiler-/ID-Kombination verwenden **30 `TestTriceLog`-Konfigurationen** einen Bulk-Pfad; unter anderem ungeframte Direct-/Combined-Pfade bleiben zeilenweise. Für dieselbe Konfiguration `dblB_di_nopf_rtt32__de_xtea_cobs_ua` im GCC-Insert-Lauf dauert `TestTriceLog` auf dem Mac **22,58 s**, unter Windows **40,62 s**, unter Debian **79,48 s**. Solche Fälle sind für P5 relevant. Die Paketzeiten enthalten jedoch zusätzlich Build-/Link- und Startkosten. Vier äußere Worker führen Tests parallel aus; Einzelzeiten dürfen nicht als vermeintlich einsparbare Stufenzeit addiert werden. Größere Bulk-Nutzung muss weiterhin die ursprüngliche Fehlerzeile liefern.

Wirksame Budgets sind jetzt belegt: L432 **8 Worker unter Windows, 16 unter Debian, 12 auf dem Mac**, jeweils `make -j1`; PC überall **4 Worker**, `go test -p 1`, Modus `auto`. Mehr Parallelität ist damit eine spätere Abstimmung, keine bisher fehlende Funktion.

Alle drei Läufe verwenden Go **1.27.3**. GCC unterscheidet sich (**Windows/Mac 16.2.0, Debian 15.2.0**), ebenso Clang (**Windows 23.1.3, Debian/Mac 23.1.4**). Der Mac-Lauf brauchte **1.011 s** Vorbereitung mit mehreren Toolinstallationen; daraus folgt kein vollständig kalter Go-/Compiler-Cache. Offen bleiben unter anderem Checkout-Dateisystem, Shell-Versionen, anfänglicher Build-Cache, Energieprofil und Hintergrundlast. Die Gesamtdifferenz ist deshalb keine reine Betriebssystemmessung. Neue Messläufe sind nicht beauftragt.

### 5.4. <a id="ci-status-und-issue-786"></a>CI-Status und Issue #786

Am 2026-11-10 erneut geprüft: **#786 `Avoid macOS Intel CI timeouts during cold LLVM preparation` ist OPEN.** Die Abnahme verlangt dokumentierte kalte und warme native Intel-Läufe sowie den Abschluss des vollständigen Testplans. Der erfolgreiche kalte LLVM-Download allein reicht nicht zum Schließen.

Der neuere Nachtlauf `42784941279` verwendet weiterhin `dd15c207` und bestätigt das Zeitbudgetproblem:

Ziel                            | Ergebnis            | Relevanter Befund
------------------------------- | ------------------- | ---------------------------------------------------------------
Intel-macOS, Job `129723704620` | Abbruch bei 180 min | 24 Stufen PASS; letzte Clang-Stufe nach 29 min 52 s abgebrochen
Windows, Job `129723705038`     | Abbruch bei 180 min | 24 Stufen PASS; letzte Clang-Stufe nach 33 min 27 s abgebrochen
ARM-macOS, Job `129723705027`   | PASS                | Vollständiger Plan und Race-Prüfung; rund 105 min Jobdauer
Linux, Job `129723705121`       | PASS                | Vollständiger Plan und Race-Prüfung; rund 75 min Jobdauer

Intel hatte erneut keinen Projekt-Toolcache; die vorkompilierte LLVM-Vorbereitung bestand diesmal nach **23 Sekunden**. Windows stellte seinen Projekt-Toolcache wieder her. Beide abgebrochenen Jobs übersprangen die Race-Prüfung und den abschließenden Cache-Speicherschritt. In den Konsolenprotokollen ist vor dem globalen Abbruch kein fehlgeschlagener Test gemeldet; daraus folgt kein vollständiger PASS und auch kein Beweis gegen einen möglichen Hänger in der letzten Matrix.

Der erfolgreiche PR-Lauf `42800233452` verwendet `quick` ohne Intel und ersetzt diese fehlenden Nachweise nicht. **Empfehlung: #786 noch offen lassen**, S1 und S2 zuerst abschließen; Issue-Abschluss separat beauftragen.

## 6. <a id="anhang-b-stufendetails"></a>Anhang B Stufendetails

### 6.1. <a id="stabilisierung-vor-optimierung"></a>Stabilisierung vor Optimierung

**S1: kleiner CI-Fix, Budget 360 Minuten**

Nur das Jobbudget des vorhandenen wiederverwendbaren nativen Workflows für `full` auf Windows und Intel-macOS ändern; die anderen Ziele und `quick` bleiben beim bisherigen Budget. Keine neuen Jobgruppen, Worker, Logpfade, Toolversionen oder Testfilter einführen. Vorhandene Workflow-/Release-Regressionstests gezielt ergänzen beziehungsweise anpassen, sodass Ziel-/Umfangsauswahl, Pflichtprüfungen und Check-Namen erhalten bleiben. Der normale lokale Aufruf ändert sich nicht.

**Abnahme von S1:** fokussierte Prüfungen bestätigen die Budgetauswahl und den unveränderten vollständigen Ablauf. Der Diff bleibt auf diese Korrektur begrenzt. Das ist noch kein nativer vollständiger Testnachweis; keine Suite oder CI-Dispatch ohne gesonderten Auftrag starten.

**S2: vollständige Läufe abnehmen**

Die durch den Nutzer gestarteten oder ausdrücklich beauftragten nativen Läufe des geänderten Stands auswerten. Windows muss den vollständigen Plan, Wiederherstellung und Race-Prüfung abschließen. Auf Intel sind ein vollständiger kalter Lauf und anschließend ein tatsächlicher Cache-Restore mit vollständigem warmem Lauf nachzuweisen. Linux und ARM-macOS für denselben Stand nativ bestätigen; bestehende Nachweise einer früheren Revision getrennt halten.

**Abnahme von S2:** keine fehlgeschlagenen, fehlenden oder durch Abbruch unvollständigen Pflichtprüfungen; Gesamt-PASS einschließlich Wiederherstellung und Race. Zeiten, Revision, Cache-Miss/-Hit und Betriebssystem dokumentieren. Während des Laufs keine getrackten Dateien bearbeiten. Ein echter Testfehler führt zu einem eigenen kleinen Korrekturschritt und dessen Nachweis, bevor L1/P3–P7 beginnen. Erst bei erfüllter #786-Abnahme dessen Schließung vorschlagen.

### 6.2. <a id="logablage-und-archivierung-vereinfachen"></a>Logablage und Archivierung vereinfachen

**L1, erst nach S2**

Den Hinweis am Dokumentanfang umsetzen: ein fester Laufordner mit festen Unterordnern und Dateinamen, Abschluss-ZIP mit Datum, Uhrzeit bis zur Sekunde und OS sowie zugehörige Konsolenausgabe als Überblick. Die vorhandenen drei ZIPs sind unveränderte Ausgangsnachweise und werden nicht umgepackt oder umbenannt.

**Abnahme:** PASS, FAIL und Abbruch liefern auswertbare Archive. Vor einem neuen Lauf nur entpackte Daten eines fertig archivierten Laufs entfernen; ZIPs, aktive Läufe und notwendige Wiederherstellung bleiben geschützt. Verweise in Zusammenfassung, JSON, Detailausgaben und Archiv passen zusammen. Linux, macOS einschließlich Bash 3.2 und Windows berücksichtigen. Keine Testlogik oder Testabdeckung als Nebenarbeit ändern.

### 6.3. <a id="messdaten-und-testabdeckung-erfassen"></a>Messdaten und Testabdeckung erfassen

**P1**

Der Vergleich der drei archivierten Läufe ist in Anhang A erledigt. Die nachfolgende vollständige Testlandkarte ist Vorbereitung für spätere Optimierungen und keine Voraussetzung für S1.

Vorhandene JSON-, Stufen- und Konfigurationsprotokolle auswerten. Testnamen, Pakete, Build-Tags, Umgebungsvariablen, Compiler, ID-Workflow, Coverage-Instrumentierung und Fehlernachweise den aufrufenden Stufen zuordnen. Zusätzlich zur Suite die eigenständigen Workflows für [Coverage](../../.github/workflows/coverage.yml), [Script portability](../../.github/workflows/script-portability.yml) und [Bibliothekstests](../../.github/workflows/trice_lib_reusable.yml) berücksichtigen.

Für die größten Blöcke Vorbereitung, Kompilierung, Logger-/Prozessstarts und eigentliche Prüfzeit trennen. Die tatsächlichen Intel-Worker und deren Auslastung ermitteln; die belegten lokalen Budgets nicht einfach auf CI-Runner übertragen. Die Mac-Ausgangsdaten stehen jetzt in Anhang A.

**Ergebnis:** eine kompakte Testlandkarte und Laufzeittabelle für vergleichbare Umfänge. Fehlende Messungen benennen und gegebenenfalls einen gesondert beauftragbaren Messlauf vorschlagen. Keine neuen Benchmarks während der reinen Bestandsaufnahme.

### 6.4. <a id="zuschnitt-und-reihenfolge-entscheiden"></a>Zuschnitt und Reihenfolge entscheiden

**P2**

Die erste Reihenfolge ist jetzt S1/S2 vor jeder Optimierung. Danach die Maßnahmen nach erwarteter Wirkung, Aufwand, Abdeckungsrisiko und Pflegeaufwand ordnen. Für jede vorgeschlagene Änderung angeben, welche Arbeit tatsächlich entfällt und welcher Nachweis erhalten bleibt. Laufzeiteinsparung und kürzere Wartezeit durch mehrere Runner getrennt bewerten.

Den nachfolgenden Drei-/Vier-Gruppen-Vergleich anhand der Testlandkarte bestätigen oder korrigieren. Zusätzliche Vorbereitung, Cache-Transfers, verfügbare Runner, Warteschlangen und gesamte Runner-Minuten einrechnen. Konkrete Umsetzung und Erfolgskriterien der nächsten Stufe vor dem Start festhalten.

**Ergebnis:** eine entschiedene Reihenfolge und ein begrenzter Pilotumfang. Eine GCC-Reduktion bleibt eine gesonderte Entscheidung unter O1.

### 6.5. <a id="toolcache-und-fortschritt-absichern"></a>Toolcache und Fortschritt absichern

**P3**

Prüfen, wie vollständig verifizierte Tools bereits nach erfolgreicher Vorbereitung gespeichert werden können, auch wenn spätere Tests abbrechen. Cache-Schlüssel, Inhalt, Integritätsprüfung und Zuständigkeit für das Speichern ausdrücklich bestimmen; keine unvollständigen Installationen als gültigen Cache veröffentlichen.

Die Fortschrittsanzeige so konkretisieren, dass eine noch laufende letzte Stufe nicht wie abgeschlossene Arbeit aussieht. Erwartete Gewichte, tatsächlich abgeschlossene Stufen und Endergebnis klar unterscheiden; keine ungemessene Restzeit versprechen.

**Abnahme:** ein späterer Testfehler erhält verwertbare Vorbereitung und Diagnose. Ein Warmcache-Nachweis muss einen tatsächlichen Restore zeigen. PASS, FAIL und Abbruch bleiben eindeutig.

### 6.6. <a id="mehrfachausführungen-bewerten-und-reduzieren"></a>Mehrfachausführungen bewerten und reduzieren

**P4**

Schwerpunkt sind [515](../../scripts/_515_test_logging_features.sh), [540](../../scripts/_540_test_go.sh) und [550](../../scripts/_550_test_go_coverage.sh). 515 besitzt gezielt ausgewählte CE/SL- und Demo-Prüfungen sowie eigene Feature-Beispiele. 540 und 550 führen normale Pakete aus, 550 mit globaler Coverage-Instrumentierung. Diese Unterschiede zunächst erhalten und die tatsächlich identischen teuren Ausführungen aus P1 benennen.

Erst dann entscheiden, ob identische Integrationsergebnisse wiederverwendet oder Prüfungen anders zugeordnet werden. Die normale und die instrumentierte Go-Ausführung nicht ohne begründete Entscheidung gleichsetzen. Auch wiederholte Arbeit zwischen eigenständigem Coverage-Workflow, Portabilitäts-Workflow und nativer Suite bewerten.

**Abnahme:** Vorher-/Nachher-Zuordnung aller betroffenen Pflichtprüfungen; dieselben Fehler werden weiterhin erkannt. Benannte PASS-Nachweise, fehlende Voraussetzungen, leere Testauswahl und SKIP werden weiter geprüft. Der vereinbarte Coverage-Umfang bleibt vollständig.

### 6.7. <a id="pc-ausführungskosten-verringern"></a>PC Ausführungskosten verringern

**P5**

In [triceCheck.c](../../_test/testdata/triceCheck.c) und dem [CGO-Harness](../../_test/testdata/cgoPackage_test.go) ermitteln, wo Einzelzeilen besonders viele Logger- oder Prozessstarts verursachen. Bestehendes `auto` und vorhandene Bulk-Pfade berücksichtigen; insbesondere verbleibende `combinedMode`- und ungeframte Pfade untersuchen.

Nur kompatible Erwartungen weiter bündeln. Content-basierte [Cache-Overlays](../../scripts/pc_cache_overlay.go), unterschiedliche C-Konfigurationen und Source-Zuordnung erhalten. Kompilierungszeit und Ausführungszeit getrennt vergleichen.

**Abnahme:** absichtlich ausgelöste Fehler bleiben auf die ursprüngliche Zeile zurückführbar. Bulk-FAIL behält FAIL, auch wenn die zeilenweise Diagnose besteht. Unterstützte und nicht bündelbare Kanäle sowie Direct-, Deferred- und Combined-Verhalten bleiben abgedeckt.

### 6.8. <a id="vorhandene-worker-parallelisierung-abstimmen"></a>Vorhandene Worker Parallelisierung abstimmen

**P6**

Für [L432](../../examples/L432_inst/all_configs_build.sh) und den [PC-Worker](../../scripts/_160_pc_target_test_worker.sh) anhand repräsentativer, gesondert beauftragter Messungen sinnvolle Budgets vergleichen. CPU, RAM, Datenträger, Gesamtzeit und Abbruchverhalten erfassen. Äußere Worker und innere Build-Parallelität gemeinsam betrachten; nicht mehrere Parallelisierungsebenen unkontrolliert multiplizieren.

Unterschiedliche Konfigurationen benötigen getrennte Ausgaben. Compiler- und ID-Workflows bleiben im selben Checkout nacheinander, solange Quellen gemeinsam verändert und restauriert werden. Ein größeres Worker-Budget ist nur bei gemessener Verbesserung ein Fortschritt.

**Abnahme:** nachvollziehbare Budgets für Windows, Linux, ARM-macOS und Intel-macOS; sichere Begrenzung und Überschreibbarkeit. Keine Ressourcenfehler, verwaisten Prozesse, beschädigten Ausgaben oder verlorene Wiederherstellung.

### 6.9. <a id="teilweise-aufteilung-in-ci-jobs-erproben"></a>Teilweise Aufteilung in CI Jobs erproben

**P7**

Erst nach S2 und einem eigenen Startkommando den entschiedenen Zuschnitt als begrenzten Pilot betrachten. Den lokalen Standardaufruf und die PR-Auswahl `quick` zunächst erhalten. Erst nach erfolgreichem Pilot über die Ausweitung derselben Gruppen auf die übrigen nativen CI-Ziele entscheiden. Die Aufteilung ist keine Voraussetzung für den ersten vollständigen Intel-Nachweis.

Gruppenauswahl und Stufenliste zentral halten. Jeder Job erhält einen eigenen Checkout und eigene veränderliche Ausgaben. Nur verifizierte wiederverwendbare Tool-/Build-Caches teilen. Gruppenweise Logs und ein vollständiger Gesamtstatus gehören zum Pilot.

**Abnahme:** jede bisher erforderliche Prüfung hat einen Besitzer; keine Gruppe fehlt und kein Test wird allein durch die Aufteilung unbeabsichtigt vervielfacht. Fehlende oder abgebrochene Gruppen erlauben keinen Gesamt-PASS. Abbruch, Wiederherstellung, Artefakte und bestehende erforderliche Check-Namen beziehungsweise ihre notwendige Zuordnung sind geklärt.

### 6.10. <a id="ergebnisse-abnehmen-und-dokumentieren"></a>Ergebnisse abnehmen und dokumentieren

**P8**

Den tatsächlich umgesetzten Stand mit P1 vergleichen: Testzeit, Vorbereitung, Wartezeit, längster Job, gesamte Runner-Minuten, Cache-Hits und Fehlerdiagnose. Bei veränderter Gruppierung sind mehrere vollständige Gruppenergebnisse gemeinsam der Nachweis für den bisherigen vollständigen Umfang.

Die beauftragten finalen Läufe umfassen Windows, Linux, ARM-macOS und Intel-macOS sowie relevante `quick`-/`full`- und kalte/warme Szenarien. Die nötigen kalten/warmen Nachweise gezielt vereinbaren; nicht jede teure Kombination wahllos wiederholen. Für Intel fehlen weiterhin vollständiger Nachweis und Race-Ergebnis; die erfolgreiche LLVM-Vorbereitung allein schließt diese Lücke nicht.

**Abnahme:** dokumentierte PASS-/FAIL-/INCOMPLETE-Ergebnisse des konkreten Stands, keine unbeabsichtigte Abdeckungsänderung und weiterhin verständliche Reproduktion. Release- oder Issue-Aktionen bleiben gesondert zu beauftragen.

### 6.11. <a id="compilerabdeckung-gesondert-entscheiden"></a>Compilerabdeckung gesondert entscheiden

**O1, optional**

Den Vorschlag „volle Breite mit Clang, weniger GCC-Varianten“ erst anhand unterschiedlicher Fehlerklassen und gemessener Laufzeiten bewerten. Auf dem lokalen Windows-Lauf liegen PC-GCC mit 33 min 49 s und PC-Clang mit 31 min 08 s nahe beieinander; das begründet allein keine Reduktion.

**Ergebnis:** konkrete erhaltene und entfallende Kombinationen mit Begründung und notwendigem nativen Nachweis. Ohne ausdrückliche Entscheidung bleibt der GCC-Umfang unverändert.

## 7. <a id="anhang-c-jobaufteilung-und-technische-grenzen"></a>Anhang C Jobaufteilung und technische Grenzen

**Spätere Option unter P7; nicht Teil von S1/S2.** Eine Jobaufteilung ist für die erste Stabilisierung nicht beschlossen.

Der einfachste zusätzliche Schnitt wären drei Gruppen: allgemeine Prüfungen einschließlich Firmware/L432, PC-GCC und PC-Clang. Beim gemessenen Windows-Lauf enthält die erste Gruppe bereits **145 min 16 s** an Stufenzeit. Die beiden anderen Gruppen benötigen rund 34 und 31 Minuten. Für eine spätere bessere Verteilung lassen sich deshalb vier feste fachliche Gruppen vergleichen:

Gruppe   | Zugeordnete Stufen im vollen Lauf                 | Windows lokal | Debian lokal | ARM-Mac lokal | Intel laut Übergabe
-------- | ------------------------------------------------- | ------------- | ------------ | ------------- | ---------------------------
checks   | 400–550: Konsistenz, Bind, Features, Go, Coverage | 92 min 55 s   | 8 min 33 s   | 50 min 33 s   | mind. 47 min 19 s
firmware | 560–620: Clang/GCC Firmware und L432              | 52 min 21 s   | 11 min 41 s  | 11 min 26 s   | mind. 50 min 33 s
pc-gcc   | 630 und 640: Insert/Clean und Bind mit GCC        | 33 min 49 s   | 32 min 31 s  | 17 min 54 s   | 57 min 35 s
pc-clang | 645: Insert/Clean und Bind mit Clang              | 31 min 08 s   | 23 min 47 s  | 10 min 35 s   | nach 9 min 43 s abgebrochen

Die Windows-Zeiten sind Summen der seriell gemessenen Stufen; 12 Sekunden Ablauf-/Rundungsdifferenz zur Gesamtdauer sind nicht verteilt. Die Intel-Untergrenzen enthalten nur die in der Übergabe ausgewiesenen großen Blöcke. Insbesondere die vollständige Intel-Clang-Dauer ist unbekannt.

Die Debian-Gruppen summieren sich auf 76 min 32 s, mit 1 Sekunde Differenz zur Gesamtdauer. Dort wären allgemeine Prüfungen und Firmware zusammen nur 20 min 14 s lang; im Drei- wie im Viererzuschnitt bliebe PC-GCC mit 32 min 31 s der längste Block. Auf dem Mac sind die allgemeinen Prüfungen mit 50 min 33 s am längsten. Der Windows-Vorteil von vier Gruppen lässt sich deshalb nicht unmittelbar auf andere Plattformen übertragen. Dies ist eine Auswertung serieller Stufenzeiten, kein gemessener paralleler Lauf und keine Freigabe zur Umsetzung.

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
- Keine Tests stillschweigend streichen oder Pflichtprüfungen in SKIP umwandeln. S1 verwendet auf Basis der dokumentierten Laufzeiten das vom Nutzer freigegebene 360-Minuten-Budget, dessen tatsächlicher Erfolg erst in S2 geprüft wird. Die GitHub-Grenze bleibt bestehen; interne Test-Timeouts werden nicht erhöht.
- Ein FAIL oder INCOMPLETE bleibt fehlgeschlagen beziehungsweise unvollständig, auch wenn ein späterer Diagnoselauf besteht.
- Jede Diagnose muss Compiler, ID-Workflow, Konfiguration und betroffene Quellzeile oder Erwartung erkennen lassen. Originalausgabe und reproduzierbarer Aufruf bleiben verfügbar.
- Veränderliche Quellen, generierte Dateien, Build-Ausgaben und Zustandswiederherstellung dürfen nicht gleichzeitig mehreren Besitzern gehören. Vorhandene fremde Testläufe und Sperren bleiben geschützt.
- Tatsächlichen Testhost und simuliertes Ziel unterscheiden. Native Windows-, Linux- und macOS-Ergebnisse werden getrennt von Simulationen ausgewiesen; Intel und ARM-macOS sind eigene Ausführungsziele.
- Die neueste stabile Go-Version bleibt Vorgabe. Die pro Lauf gewählte Version wird protokolliert und innerhalb des Laufs beibehalten. Versionswechsel zwischen Messungen werden als Vergleichsgrenze ausgewiesen.
- Kein Refactoring von Produktcode oder Umbau von Toolchains als Nebenarbeit. #786, Release v2.0.1, Tags und Issue-Abschluss sind keine automatischen Folgen dieses Plans.

## 8. <a id="anhang-d-entscheidungen-und-fortschritt"></a>Anhang D Entscheidungen und Fortschritt

Punkt                   | Festlegung oder Vorschlag                                         | Status
----------------------- | ----------------------------------------------------------------- | -------------------------------------
Arbeitsgrundlage        | Dieser Plan einschließlich vollständiger Übergabe in Anhang E     | Festgelegt
Reihenfolge             | Kleiner Stabilisierungsfix und vollständige Läufe vor Optimierung | Vom Nutzer vorgegeben
S1 CI-Zeitbudget        | full auf Windows/Intel 360 statt 180 Minuten                      | Umgesetzt; fokussierte Prüfungen PASS
S2 Native Abnahme       | Vollständiger Plan, Wiederherstellung, Race; Intel kalt/warm      | Nach S1 gesondert beauftragen
L1 Logablage            | Feste Pfade, Abschluss-ZIP mit Zeit/OS und Konsolenüberblick      | Vorgemerkt; erst nach S2
P1 Logvergleich         | Drei Archive und aktueller CI-Status ausgewertet                  | Erledigt; Testlandkarte noch offen
CI Zuschnitt            | Vier feste Gruppen; drei Gruppen als Vergleich                    | Zurückgestellt unter P7
PR Umfang               | quick zunächst unverändert lassen                                 | Kein Ausbau beauftragt
Compilerabdeckung       | GCC und Clang zunächst vollständig erhalten                       | Reduktion nur über O1
Start der Umsetzung     | Nur ausdrücklich beauftragte Stufen bearbeiten                    | S1 freigegeben; weitere Stufen warten
#786                    | Kalter LLVM-Nachweis vorhanden; vollständige Intel-Läufe fehlen   | Offen lassen; keine Abschlussaktion

Nach jeder beauftragten Stufe hier kurz nachtragen: Status und Ergebnis, Entscheidungen, geprüften Commit beziehungsweise Diff, tatsächliche native Prüfungen je Plattform, getrennte Simulationen und noch offene Nachweise. Messungen verschiedener Stufen nicht ungeprüft als addierbare Einsparungen behandeln.

**S1-Ergebnis, 2026-11-10:**

- Produktionsänderung: nur `timeout-minutes` plus erklärender Kommentar in [native-tests-reusable.yml](../../.github/workflows/native-tests-reusable.yml). Aufrufer, Testauswahl, interne Timeouts, Vorbereitung, Race-Prüfung und Upload bleiben unverändert. Ergänzt: `TestNativeWorkflowBudgets` in [release_preparation_test.go](../../scripts/release_preparation_test.go).
- Regression vor dem Workflow-Fix: `Windows_full` und `Intel_macOS_full` liefern 180 statt der geforderten 360 Minuten; genau diese beiden Fälle FAIL. Nach dem Fix: **alle acht Runner-/Umfangskombinationen PASS**. Die Auswertung nutzt die vorhandene Node.js-Voraussetzung, ohne neue Abhängigkeit. Das ist eine Logikprüfung auf dem lokalen Host, kein nativer Windows-/Intel-Lauf.
- Fokussierter Aufruf mit Go 1.27.3: `go test ./scripts -run '^(TestNativeWorkflowBudgets|TestIntelLLVMLockAndWorkflowBindings|TestNativeWorkflowPreparesEPUBConverter|TestNativeWorkflowPreparesToolchainSpace)$' -count=1 -v` — **PASS**. `actionlint .github/workflows/native-tests.yml .github/workflows/native-tests-reusable.yml` — **PASS**. Kein vollständiger Paket-/Suite-Lauf, keine Coverage und kein CI-Dispatch gestartet.
- Host: ARM-macOS; Go 1.27.3, Node.js 26.13.0, Bash 5.3.20, actionlint 1.7.12. Native fokussierte Prüfungen auf macOS PASS; native Windows- und Linux-Prüfungen dieses Diffs nicht ausgeführt. Vollständige CI-Nachweise auf Windows, Linux und macOS einschließlich Intel kalt/warm sind S2; #786 bleibt offen.

Die ursprüngliche Übergabe bleibt unten vollständig als historisches Zitat erhalten. `AGENTS.md` verwendet `Codex_Handover.md` weiterhin als allgemeines Ziel für künftige Rechnerwechsel; dann genügt eine neue kurze Übergabe, die für diese Aufgabe auf den aktuellen Plan verweist. Die drei Logarchive und die historische Übergabe wurden bei S1 nicht verändert.

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

