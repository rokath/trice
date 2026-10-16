# Release-Übergabe

Stand: 15. Oktober 2026, nach dem erfolgreichen Mac-Full-Lauf. Auf dem anderen Rechner mit `handson release` laden, den Checkout vergleichen und auf die nächste Nutzeranweisung warten.

## Ziel und Git-Stand

Vorbereitung von `v2.0.0-beta-1`. Alle CI-Skripte sind laut aktueller Nutzermeldung PASS; der vollständige native Mac-Lauf ist zusätzlich anhand des lokalen Summary-Logs bestätigt. Der nächste Untersuchungsbedarf sind die deutlich längeren Windows-Testzeiten. Noch kein Auftrag für Tag oder Veröffentlichung.

- Branch: `wip`.
- HEAD: `aae662c6f9f3bb9918924cc1a1a78337844832bc` — `Finish restoring test files after repeated Ctrl-C`.
- Vor dieser Aktualisierung war der Arbeitsbaum sauber. Die lokale Tracking-Referenz `origin/wip` steht auf demselben Commit; der Remote wurde für diese Übergabe nicht erneut live abgefragt.
- Nur diese Übergabe wird jetzt geändert. Sie ist uncommittet; kein Staging, Commit oder Push für diesen Auftrag.
- Der [Implementierungsplan](Implementierungsplan.md) enthält ausführliche Nachweise, aber ältere offene Teststände und redaktionelle Befunde. Für die aktuelle Abnahme und die folgenden Nutzerentscheidungen gilt diese Übergabe; erledigte Punkte nicht erneut implementieren.

## In Git enthaltene Ergebnisse

- Skriptpflege und Scratchpad-Ausschlüsse sind umgesetzt: `docs/_scratchPad/` bleibt ein Schmierzettelverzeichnis und wird aus Tests, Formatprüfungen und Linkchecks ausgeschlossen. Kein zusätzliches `go.mod` dort anlegen. Die drei alten Profiltransfer-Helfer sind entfernt; Archive bleiben unangetastet.
- Bind-Einstiege für ABC, CLion und Standalone-/Clang-Builds, CGO-Vorlagen und die zugehörige RM-Test-/Buildanleitung sind umgesetzt. Die großen Insert- und Bind-Matrizen bleiben erhalten. Die Tabelle in `docs/rm/maintenance.md` ist ebenfalls repariert.
- `8c50efee` — `Run the full library matrix for pull requests to main`: PRs nach `main` führen die volle Bibliotheks-Konfigurationsmatrix aus; weitere Pushes auf einen offenen PR starten sie erneut. Ein reiner `wip`-Push ohne PR löst diese Bibliotheksworkflows nicht aus. Nur geplante Läufe verwenden den 24-Stunden-Aktivitätsfilter; manuelle Starts laufen auch bei unverändertem `main`. Bestehende Check-Namen bleiben erhalten. Diese Matrix ist nicht mit dem gesamten lokalen `testAll.sh full` gleichzusetzen.
- `cbee465b` — `Honor standing read permission without optional approval prompts`: Lesezugriffe sind dauerhaft autorisiert. Blockierte optionale Abfragen dürfen weder neue Freigabefragen noch einen Arbeitsstopp verursachen; technische Plattformgrenzen bleiben davon getrennt.
- `aae662c6` — `Finish restoring test files after repeated Ctrl-C`: Wiederherstellung ignoriert weitere INT-/TERM-Signale einschließlich ihrer Hilfsprozesse. Aufrufskript und Runner warten auf das Aufräumen; eine Meldung erklärt die Wartezeit. Ursprünglicher Abbruchstatus bleibt erhalten, weitere Tests starten nicht.
- Bereits zuvor: Checkout-Installer/CI, Release-Notes-Auswahl, taggebundene Veröffentlichung/Audits, Archiv-Lizenzen, PDF-Navigation und `scripts/_verify_release_features.cjs` sind implementiert. Frühere Windows-Abnahmen stehen im Plan; sie ersetzen nicht die Prüfung später veröffentlichter Beta-Assets.

## Aktuelle Testnachweise

- **CI: alle CI-Skripte PASS**, vom Nutzer am 15. Oktober gemeldet. Keine Run-IDs übermittelt und für diese Übergabe keine zusätzliche Online-Abfrage vorgenommen.
- **Mac: `./scripts/format_repo.sh` und anschließend `./scripts/testAll.sh full` PASS.** Alle 26 Schritte bestanden; der abschließende Vergleich bestätigt unveränderte versionierte Arbeitsdateien. Nutzerprotokoll und lokales `temp/log/testAll_summary.log` stimmen überein.
- Start 15. Oktober 2026, 10:49:00 CEST; Ende 11:19:49 CEST; gemeldete Dauer **1850 s = 30 min 50 s**.
- Der letzte Commit entstand um 10:49:41 CEST, also während dieses Laufs. Alle Inhalte waren bereits vorher im Arbeitsbaum; während des Laufs wurde nur committet. Der bestandene Dateivergleich belegt unveränderte Arbeitsdateien, aber der Lauf hatte nicht durchgehend dieselbe Git-HEAD.
- Der neue Abbruchtest reproduzierte zunächst den Fehler. Nach der Korrektur bestanden sechs Fälle mit wiederholten Prozessgruppensignalen: INT/TERM jeweils für einzelnen Testschritt, Runner und öffentlichen `testAll.sh`-Aufruf, einschließlich Fortschrittsanzeige. Exakte Wiederherstellung von Quellen, TIL/LI und Sidecars sowie ausbleibende Folgetests werden geprüft.
- Gezielte Prüfungen bestanden: `go test ./scripts -run '^TestManagedWorkflowRepeatedInterrupts$' -count=1 -timeout=45s -v` sowie `go test ./scripts -run '^Test(Managed|Runner|RuntimePreparation|PCWorkerCancellation|L432MatrixCancellation)' -count=1 -timeout=90s`. ShellCheck mit Repository-Warnschwelle, shfmt, gofmt und `git diff --check` ebenfalls PASS. Der spätere Full-Lauf enthält auch die regulären Go-/Coverage-Schritte.
- Schritt 610 führte `goreleaser release --snapshot --clean` einschließlich PDF-Hook aus: Snapshot `1.3.1-next`, zehn Host-Archive, Target-Archiv und Linux-Pakete. Beide PDFs wurden neu erzeugt; UM 869178 Bytes, RM 20754704 Bytes. Ausgepackte native Darwin-arm64-Tools bestanden die Feature-Prüfung mit frischem Bind/CE, strukturierten Feldern, Stempeln, Tags, Text/JSON/KV und vollständigen Lizenzen. Das ist keine Veröffentlichung und keine neue visuelle PDF-Abnahme.
- Schritt 550 meldet **90,5 % Go-Statement-Coverage**. Ein aktueller Coveralls-Zeilenvergleich zur Zielbranch-Baseline wurde hier nicht separat durchgeführt.

## Mac-Zeitbasis für den Windows-Vergleich

Dies ist ein normaler Entwicklerlauf mit vorhandenen Werkzeug-/Buildcaches, kein kontrollierter Kaltstart-Benchmark. Die Prozentwerte unten sind relative Arbeitsgewichte, keine Zeitprognose. Windows-Zeiten zuerst pro Schritt vergleichen, bevor die Gesamtzeit bewertet wird.

Bei der Übergabe ermittelte Umgebung: macOS 26.6.2 (25G83), Darwin/arm64, Go 1.25.3, Bash 5.3.20, Arm GNU Toolchain 15.3.Rel1 / GCC 15.3.1, Apple Clang 21.0.0. GoReleaser 2.18.3 ist im Snapshot-Log belegt. CPU-Modell und RAM sind nicht erfasst.

Aus den tatsächlichen Worker-Logs:

Schritt | Umfang | Parallelität | Dauer
--- | --- | --- | ---
620 L432 | 101 Konfigurationen | 12 Jobs, je `make-jobs=1` | 283 s
630 PC Insert | 63 Konfigurationen, `mode=auto` | 4 Jobs | 325 s
640 PC Bind | 63 Konfigurationen, `mode=auto` | 4 Jobs | 333 s

Go-Paketzeiten aus den beiden Logs; Pakete laufen parallel, diese Werte daher nicht summieren:

Paket | Schritt 540: Go-Tests | Schritt 550: Coverage
--- | ---: | ---:
`scripts` | 183,919 s | 170,586 s
`pkg/tst` | 56,426 s | 50,055 s
`internal/id` | 35,475 s | 34,645 s

Die folgenden Summary-Zeilen sind absichtlich in Git mitgenommen: Die lokalen Rohlogs werden nicht auf den anderen Rechner übertragen und beim nächsten `testAll` teilweise ersetzt.

```text
Preparing testAll: capturing tracked worktree state...
Starting testAll at Thu Oct 15 10:49:00 CEST 2026
Selection: full
ID workflows: bind plus legacy insert/clean
Failure policy: continue after failures (--no-stop, default)
Progress scale: expected relative test work (hardware-independent; no ETA)
[ 1/26 | ~  0.4%] (  0m  0s) _400_test_shell_format.sh: PASS
[ 2/26 | ~  0.7%] (  0m  4s) _410_test_shellcheck.sh: PASS
[ 3/26 | ~  1.0%] (  0m  0s) _420_test_dsstore.sh: PASS
[ 4/26 | ~  1.4%] (  0m  1s) _430_test_clang_format.sh: PASS
[ 5/26 | ~  1.7%] (  0m  1s) _440_test_manual_format.sh: PASS
[ 6/26 | ~  2.0%] (  0m  1s) _450_test_markdownlint.sh: PASS
[ 7/26 | ~  2.4%] (  0m  1s) _460_test_goreleaser_config.sh: PASS
[ 8/26 | ~  2.7%] (  0m  0s) _470_test_actionlint.sh: PASS
[ 9/26 | ~  3.7%] (  0m  2s) _480_test_build_trice_tool.sh: PASS
[10/26 | ~  4.0%] (  0m  1s) _490_test_runtime_prepare.sh: PASS
[11/26 | ~  4.7%] (  0m  5s) _500_test_bind.sh: PASS
[12/26 | ~  5.4%] (  0m  1s) _510_test_bind_workflows.sh: PASS
[13/26 | ~  6.4%] (  1m 24s) _515_test_logging_features.sh: PASS
[14/26 | ~  7.0%] (  0m 11s) _520_test_target_code_lint.sh: PASS
[15/26 | ~  7.7%] (  0m 16s) _530_test_links.sh: PASS
[16/26 | ~  8.7%] (  3m 11s) _540_test_go.sh: PASS
[17/26 | ~  9.7%] (  3m  0s) _550_test_go_coverage.sh: PASS
[18/26 | ~ 10.4%] (  0m  6s) _560_test_clang_insert.sh: PASS
[19/26 | ~ 11.0%] (  0m  5s) _570_test_clang_bind.sh: PASS
[20/26 | ~ 12.4%] (  0m 34s) _580_test_gcc_off.sh: PASS
[21/26 | ~ 13.7%] (  2m 12s) _590_test_gcc_insert.sh: PASS
[22/26 | ~ 15.0%] (  3m 24s) _600_test_gcc_bind.sh: PASS
[23/26 | ~ 20.0%] (  0m 26s) _610_test_goreleaser_snapshot.sh: PASS
[24/26 | ~ 76.7%] (  4m 43s) _620_test_l432_configs.sh: PASS
[25/26 | ~ 88.4%] (  5m 25s) _630_test_pc_targets_insert.sh: PASS
[26/26 | ~100.0%] (  5m 33s) _640_test_pc_targets_bind.sh: PASS
Verifying tracked worktree state...
Result: PASS
Duration: 1850s
Finished at Thu Oct 15 11:19:49 CEST 2026
```

## Verbindliche Entscheidungen und Grenzen

- Inhaltliche Dokumentationsarbeiten und einzelne Hardwareprüfungen möchte der Nutzer **nach Beta-1** angehen. Ältere Plananforderungen dürfen diese ausdrückliche Entscheidung nicht als neue Vorabaufgabe wieder einführen.
- `docs/rm/*.md` sind die gepflegten Kapitelquellen; `docs/rm/README.md` bestimmt die Reihenfolge. mdtoc erzeugt lokale ToCs, das Gesamt-PDF vollständige Kapitelnummern. Das UM bleibt getrennt und nutzergepflegt; Änderungen daran nur auf ausdrücklichen Auftrag.
- Kein `/v2`-Modulumbau. Installation über Release-Archive oder Tag-Checkout; Modulpfad/Imports bleiben unverändert. Beta-Tag wäre `v2.0.0-beta-1`; stabile Homebrew-Veröffentlichung bleibt getrennt.
- Keine zusätzlichen Abhängigkeiten, neuen Such-/Dokumentationssysteme, Archivänderungen oder automatische Umsetzung weiterer Planpunkte.
- Große Gesamtläufe übernimmt der Nutzer manuell. Keine konkurrierenden zustandsverändernden Tests, Formatläufe oder ID-Neuvergabe starten. Tests und Builds benötigen keine erneute Freigabe; bei Windows Git Bash und den bestehenden Go-Cache verwenden.
- Commit, Push, Tag, Prerelease und Publish jeweils nur auf ausdrücklichen Auftrag. Frühere Commit-Aufträge sind erledigt und gelten nicht für diese neue Übergabe.

## Offenes und nächste Schritte auf Windows

1. Nach `handson release` Branch, HEAD und Arbeitsbaum vergleichen; danach auf den nächsten Auftrag warten. Das frühere Scratchpad-`go.mod`-Problem und die Bind-Einstiegslücken sind erledigt.
2. Für die Laufzeitanalyse zunächst vorhandene Windows-Summary-/Worker-Logs mit der Mac-Zeitbasis oben vergleichen: gleicher Source-Stand, Full-Auswahl, Konfigurationszahl und tatsächliche Joblimits. Vor einem neuen Gesamtlauf die bisherigen Windows-Logs sichern.
3. Nur die auffällig langsamen Schritte gezielt untersuchen. Bei 540/550 Paketzeiten und nötigenfalls einzelne Go-Tests messen, etwa mit `go test -json -count=1 ./scripts`; bei 620/630/640 tatsächliche Parallelität, Compiler-/CGO-Buildzeiten, Cache-Nutzung und Dateizugriffe vergleichen. Die Ursache der Windows-Langsamkeit ist bislang nicht belegt; weder Caches pauschal löschen noch Matrizen zur Zeitersparnis kürzen.
4. Für die Release-Abnahme bleiben insbesondere veröffentlichte Beta-Assets, endgültige Download-/Taglinks und gegebenenfalls gesonderte Paketinstallationsnachweise offen. CI und der Mac-Full-Lauf sind nicht mehr als fehlender PASS aufzuführen. Neue inhaltliche Arbeiten oder Boardtests erst nach der festgehaltenen Nutzerentscheidung.

## Lokale Dateien und Transfer

`temp/log/` enthält das vollständige Mac-Summary, die einzelnen Schrittlogs und Worker-Logs; `dist/` und die beiden PDFs unter `docs/` enthalten die neu erzeugten lokalen Artefakte. Diese ignorierten Dateien sind nicht Bestandteil des Git-Transfers und für das Laden der Übergabe nicht erforderlich. Die relevante Zeitbasis ist deshalb oben vollständig eingebettet.

Nur diese Übergabe ist jetzt uncommittet. Für den Rechnerwechsel manuell adden, committen und den Commit übertragen, normalerweise per Push. Keine weiteren lokalen Änderungen waren vor der Aktualisierung vorhanden.
