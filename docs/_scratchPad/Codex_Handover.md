# Übergabe: CI-Laufzeiten und Vorbereitung von v2.0.1

Stand: 2026-11-10. Jetzt nur den Rechnerwechsel vorbereiten; die folgenden Optimierungen weder weiter untersuchen noch umsetzen. Sie sind offene Fragen, keine beschlossenen Änderungen.

## Zuerst nach dem Rechnerwechsel

1. Branch, HEAD und Arbeitsbaum mit diesem Stand vergleichen; die uncommitteten Änderungen müssen mit übertragen werden.
2. Nach Aufforderung die tatsächlichen Zeitfresser und Testwiederholungen anhand der vorhandenen CI-Protokolle untersuchen. Vor Änderungen einen konkreten Vorschlag mit erwarteter Zeitersparnis, verbleibender Testabdeckung und verständlicher Fehlerdiagnose vorlegen.
3. Erst danach über Umsetzung und den noch fehlenden vollständigen Intel-Nachweis für #786 entscheiden. Keine Tests streichen oder Zeitlimits erhöhen, nur um einen grünen Lauf zu erhalten.

## Git und lokale Änderungen

- Branch: `wip`; HEAD: `3719635e40372b5fa5ca18ed225fb685351781bb` — `Strengthen cross-platform implementation and validation rules`.
- Lokaler Trackingstand: keine Commits vor `origin/wip`; dessen tatsächlicher Remote-Stand wurde für diese Übergabe nicht erneut abgefragt.
- Bereits uncommittet: [CHANGELOG.md](../../CHANGELOG.md), vorbereitete v2.0.1-Notizen zu `-logLevel` mit `-pick`/`-ban`, Windows-Eingabeabbruch und Buslock-Retry sowie LLVM-, Pandoc- und Go-Vorbereitung. Diese Änderung erhalten.
- Neu und ebenfalls uncommittet: diese Übergabe. Für den Rechnerwechsel beide Dateien manuell hinzufügen, committen und übertragen, üblicherweise per Push. Hier wurde nichts gestagt, committet oder gepusht.
- Vor der Übergabe keine weiteren sichtbaren Änderungen oder ungetrackten Dateien; kein `temp/testAll.lock` vorhanden. Keine Testquellen temporär umgeschrieben. Keine lokalen Dateien außerhalb von Git für die Fortsetzung erforderlich; heruntergeladene CI-Protokolle können erneut bezogen werden.

## Gesicherter Befund zum CI-Abbruch

Untersucht: `rokath/trice`, Actions-Run `42682343023`, Commit `dd15c207c0b17a1acac1afebf153af6b1c8aac63` auf `main`. Sein Git-Tree entspricht dem lokalen HEAD. Intel-Job: `129379957725`; Artefakt: `13715653254`, `native-macos-15-intel-full`.

- Der Projekt-Toolcache fehlte. Trotzdem war `Prepare prebuilt LLVM on Intel macOS` nach **26 Sekunden erfolgreich**. Das ursprüngliche LLVM-Kompilierungsproblem von #786 ist damit im echten kalten Intel-Lauf behoben.
- `bash scripts/testAll.sh full`: auf Intel **unvollständig**, weil der gesamte Job nach den konfigurierten 180 Minuten abgebrochen wurde. Stufen 1–24: PASS. Stufe 25, `_645_test_pc_targets_clang.sh`: nach 9 min 43 s abgebrochen. Vier Konfigurationen liefen gerade; kein konkreter Testfehler war bis dahin protokolliert. Ein Hänger ist damit nicht nachgewiesen.
- Derselbe vollständige Lauf bestand nativ auf Linux, Windows und ARM-macOS. Gesamte Jobdauer einschließlich Vorbereitung: ungefähr 88, 115 und 118 Minuten. Intel-Race-Nachprüfung wegen Abbruch übersprungen. Kein vollständiger oder warmer Intel-PASS vorhanden.
- Die Fortschrittsanzeige zeigt bereits beim Start der letzten Stufe 100 %. Sie beschreibt keine tatsächlich abgeschlossene Arbeit. Die letzte Stufe enthält zwei vollständige PC-Matrizen.
- `Post Restore verified project tools` wurde auf Intel übersprungen; der neue Projekt-Toolcache wurde nicht gespeichert. Ein bloßer Wiederholungslauf ist daher kein verlässlicher Warmcache-Test. Der Artefakt-Upload war erfolgreich und beweist keinen Test-PASS.

Große Zeitblöcke auf Intel:

Bereich                                       | Dauer
--------------------------------------------- | ----------------------
Featuretests, normale Go-Tests und Coverage   | 47 min 19 s
GCC-Firmwaretests                             | 21 min 25 s
101 L432-Konfigurationen                      | 29 min 08 s
63 PC-Konfigurationen, GCC mit `insert`       | 26 min 48 s
63 PC-Konfigurationen, GCC mit `bind`         | 30 min 47 s
PC-Clang mit `insert` und anschließend `bind` | 9 min 43 s bis Abbruch

Die großen Teststufen laufen im Job nacheinander. Innerhalb einzelner Matrizen existiert bereits Parallelisierung; deren tatsächliche Auslastung ist noch nicht untersucht.

## Offene Untersuchungen und Überlegungen des Nutzers

- **Unnötige doppelte Tests:** `_515_test_logging_features.sh`, `_540_test_go.sh` und `_550_test_go_coverage.sh` führen teilweise dieselben aufwendigen Demo- und Compiler-Integrationstests aus. Notwendige Unterschiede, etwa Coverage-Instrumentierung oder zusätzliche Varianten, von vermeidbaren Wiederholungen unterscheiden. Verbesserungen vorschlagen, ohne dabei unbemerkt Testabdeckung zu verlieren.
- **Einzelzeilen gegen Bulk:** Der Nutzer vermutet einen wesentlichen Zeitanteil bei den vielen Einzelzeilentests in „checkTrice.c“. Der tatsächliche Dateiname ist [triceCheck.c](../../_test/testdata/triceCheck.c). Kompilierung, Logger-Neustarts pro Erwartung und eigentliche Testausführung getrennt betrachten. Bulk ist bereits vorhanden: Der PC-Worker verwendet `TRICE_PC_TEST_MODE=auto`; unterstützte gerahmte Kanäle werden gebündelt, andere Pfade bleiben zeilenweise. Besonders die verbleibenden `combinedMode`- und ungeframten Pfade prüfen, statt einen vollständig fehlenden Bulk-Modus anzunehmen.
- **Fehler müssen auffindbar bleiben:** Bei weiterer Bündelung muss ein FAIL die betroffene Konfiguration, Compiler-/ID-Variante und die ursprüngliche Quellzeile bzw. Erwartung leicht erkennen lassen. Bestehende Bulk-Diagnose mit Quellzeilenzuordnung und Originalausgabe sowie den zeilenweisen Diagnoselauf bei Bulk-FAIL erhalten oder gezielt verbessern. Ein erfolgreicher Diagnoselauf darf einen zuvor fehlgeschlagenen Bulk-Test nicht nachträglich als PASS werten.
- **L432 parallel:** Prüfen, ob sich die ungefähr 100 Konfigurationen besser parallel ausführen lassen. Bereits bekannt: `all_configs_build.sh` besitzt eine äußere Parallelisierung mit `TRICE_L432_TEST_JOBS`, während der einzelne Build `make -j1` nutzt. Tatsächliche Jobzahl, Engpässe und mögliche gemeinsame Ausgaben im Intel-Lauf sind noch zu prüfen.
- **Clang und GCC parallel:** Getrennte CI-Jobs oder anderweitig isolierte Arbeitsverzeichnisse erwägen. Die ID-Workflows ändern und restaurieren Quellen; sie dürfen nicht gleichzeitig denselben Checkout oder veränderliche Build-Ausgaben benutzen.
- **GCC-Testumfang reduzieren:** Nutzerüberlegung: volle Bandbreite mit Clang, weniger Varianten mit GCC, weil Clang aus seiner Sicht schneller und neuer ist. Das ist noch nicht beschlossen. Anhand gemessener Laufzeiten und tatsächlich unterschiedlicher Compilerabdeckung bewerten; keine GCC-Tests ohne Entscheidung streichen.
- **Weitere bisherige Vorschläge:** Den vollständigen Lauf auf allgemeine Tests/Firmware, PC-GCC und PC-Clang verteilen; verifizierte Tools schon nach erfolgreicher Vorbereitung speichern; Fortschrittsanzeige korrigieren. Ebenfalls noch nicht umgesetzt.

## Relevante Dateien und Grenzen

- CI: [native-tests.yml](../../.github/workflows/native-tests.yml), [native-tests-reusable.yml](../../.github/workflows/native-tests-reusable.yml). PRs verwenden `quick`; Intel läuft zusätzlich bei Nicht-PR-Ereignissen mit `full`.
- Ablauf und Wiederholungen: `scripts/_110_test_runner.sh`, `_515_test_logging_features.sh`, `_540_test_go.sh`, `_550_test_go_coverage.sh`.
- PC-Matrizen: `scripts/_160_pc_target_test_worker.sh`, `_630_test_pc_targets_insert.sh`, `_640_test_pc_targets_bind.sh`, `_645_test_pc_targets_clang.sh`; `scripts/pc_cache_overlay.go`; `_test/testdata/cgoPackage.go` und `cgoPackage_test.go`.
- L432: `scripts/_620_test_l432_configs.sh`, `examples/L432_inst/all_configs_build.sh`.
- #785 hatte erfolgreiche Windows-Nachweise; #786 hat jetzt den kalten LLVM-Nachweis, aber weiterhin keinen vollständigen Intel-PASS. Issue-Status vor einer späteren Abschlussaktion prüfen. Kein Issue in dieser Sitzung geschlossen, kein Release oder Tag angelegt.
- Während dieser Untersuchung nur vorhandene CI-Protokolle und Quelltexte gelesen; keine neuen lokalen Tests, Coverage- oder Compilerläufe gestartet. Die Übergabe ist keine Freigabe für solche Abschlussläufe oder für CI-Änderungen. Die finale Validierung übernimmt der Nutzer, sofern er sie nicht ausdrücklich beauftragt.
- `AGENTS.md` beachten: Änderungen gezielt halten; alle drei Betriebssysteme berücksichtigen; vorhandene laufende Tests und ihre Zustandswiederherstellung schützen. Ein schlichtes `commit` verlangt nur kopierbare Befehle, keine Ausführung.
