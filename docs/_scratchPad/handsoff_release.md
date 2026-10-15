# Release-Übergabe

Stand: 14. Oktober 2026. Auf dem anderen Rechner mit `handson release` laden, den tatsächlichen Checkout vergleichen und auf die nächste Nutzeranweisung warten.

## Ziel und Git-Stand

Vorbereitung von `v2.0.0-beta-1`. R15 ist lokal auf Windows umgesetzt, geprüft und vollständig committet; veröffentlichte Assets und die abschließende Plattformabnahme bleiben offen. Maßgeblich ist der [Implementierungsplan](Implementierungsplan.md), mit offenen Aufgaben vorne und erledigten Nachweisen hinten.

- Branch: `wip`.
- HEAD: `afb82f17c1dcde7c3da067b26d131ff2034343ea` — `Record R15 validation and remaining release checks`.
- Vier R15-Commits seit `e3c217d6`: Go-Workflow, PDF-Konfiguration mit Regressionstest, Target-README und Plan-Nachweis. Lokal `ahead 4` gegenüber `origin/wip`; der Remote-Stand wurde nicht live abgefragt. Kein Push durch den Agenten.
- Bereits vor dieser Übergabe uncommittet: `docs/scratchPad/scratchPad.md`, `examples/F030_inst/Core/Inc/triceConfig.h` und `examples/G0B1_inst/Core/Inc/triceConfig.h`. Diese fremden Änderungen wurden nicht in R15 aufgenommen. Die Headeränderungen betreffen kommentierte Aufrufbeispiele mit `-typeX0`.
- Diese Übergabe ist neu und uncommittet. Für ihren Auftrag werden weder Staging noch Commit oder Push ausgeführt.

## In Git enthaltene Ergebnisse

- R26: Windows-Testfehler behoben; 99 gezielte Tests und vollständige Schritte 540/550 bestanden. R32: gepinntes stitchmd korrekt erkannt, PDF-/Snapshot-Build und Links geprüft. R14: Windows-GOPATH-/Installationsschutz korrigiert und Checkout-CI eingerichtet. Detailnachweise stehen im Plan; native Linux-/macOS-Gegenproben bleiben offen.
- R15: [go.yml](../../.github/workflows/go.yml) liest die Version aus `go.mod` statt fest Go 1.24.0. Die entsprechende alte Befundzeile unter R27 im Plan ist dadurch überholt.
- [Target-README](../../src/ReadMe.md) erklärt den Inhalt des eigenständigen Target-Archivs und die getrennten Host-Tools und Manuals; keine Archivnavigation zu fehlenden Root-Dokumenten.
- [PDF-Konfiguration](../../scripts/_manual_pdf_config.cjs) bindet UM-Querverweise wie im RM an den Quellcommit und setzt die PDF-Metadatentitel aus der ersten Hauptüberschrift. Ohne Git-Metadaten bleibt `main` der Rückfall. [Regressionstest](../../scripts/portability_test.go) prüft beide Fälle, Navigation und Titel.
- Release-Notes-Auswahl, taggebundene Veröffentlichung/Audits, vollständige Archiv-Lizenzen und [_verify_release_features.cjs](../../scripts/_verify_release_features.cjs) waren bereits implementiert und wurden tatsächlich ausgeführt, nicht neu entwickelt.

## Geprüfter Stand und Ergebnisse

R15 lief in einem isolierten Checkout bei `41291b9f0070b8b5d221811c4bf77b65235b8be6` plus den anschließend committeten R15-Korrekturen. Das ist keine Full-Abnahme des jetzigen HEAD. Lokal: Windows mit Git Bash, Go 1.26.1, Node 24.14.0 und stitchmd-Modul v0.9.0. Vorhandene Renderer-Caches aus R32 wurden wiederverwendet.

- `rtk proxy goreleaser release --snapshot --clean --skip=publish` — PASS einschließlich Hooks, beider PDFs, Cross-Builds, zehn Host-Archiven, Target-Archiv und Linux-Paketbau. Snapshot-Version `1.3.1-next` folgt dem vorhandenen Tag `v1.3.0`; kein v2-Tag oder Publish.
- `rtk go test ./scripts -run 'Test(ManualPDFNavigationOutlivesTheRenderer|ReleasePDFChecksRequireEachBook|ReferencePDFPreparation|ReleaseAuditSelectsRequestedTag|SelectedReleaseNotes|ReleaseTagCheckout|ReleaseWorkflowBindings|ReleaseFeatureOutputValidation)$' -count=1 -v` — 28 Tests PASS.
- `rtk go test ./scripts -run 'TestManualPDFNavigationOutlivesTheRenderer$' -count=1 -v` und `rtk proxy bash scripts/_320_generate_trice_user_manual_pdf.sh` — nach der Metadatentitel-Korrektur erneut PASS.
- Ausgepackte Windows-Archive für amd64 und 386 mit `scripts/_verify_release_features.cjs` — beide PASS. Eingaben im Prüfcheckout: `../r15-tools/trice_tool_windows_amd64` bzw. `../r15-tools/trice_tool_windows_386` und `dist/trice_target_sources_1.3.1-next.zip`; Windows setzte `RELEASE_FEATURE_BASH="$(cygpath -w "$BASH")"`. Frisches Bind mit CE, C-Kompilierung und erzeugtes `capture.bin`; beide Tools liefern jeweils zehn korrekte Records als Text, JSON und KV ohne Diagnosezeilen im Maschinenkanal.
- Zehn Tool-Archive mit beiden Binaries und vollständiger MIT-Lizenz; alle 58 Target-Dateien einschließlich Fallbacks und MIT-/SEGGER-Texten sowie alle 17 Archiv-/Paketprüfsummen — PASS.
- Echter Release-Notes-Schritt für Beta-1: korrekter englischer Abschnitt und 15 taggebundene, lokal vorhandene Dokumentationsziele — PASS. Echte CI-Artefaktvorprüfung und beide Workflow-PDF-Größenprüfungen — PASS.
- PDFs: UM 9 Seiten, RM 458 Seiten; korrekte Titel, 69 bzw. 432 commitgebundene Links, keine localhost- oder `blob/main`-Navigationsziele. Einstiegsseiten und Binary-Encoding-Diagramme visuell geprüft.
- Offline-Lychee: 1.979 Vorkommen, 289 Ausschlüsse, 0 Fehler. `markdownlint src/ReadMe.md docs/scratchPad/Implementierungsplan.md`, `actionlint .github/workflows/go.yml`, Go-Formatierung, JS-Syntax und gezieltes `git diff --check` — PASS.
- Kein neuer Full-Testlauf, keine native Linux-/macOS-Ausführung, Paketinstallation oder Hardwareabnahme in R15. Nach den Commits wurden keine Builds wiederholt.

## Verbindliche Entscheidungen

- `docs/rm/*.md` sind die gepflegten Kapitelquellen; `docs/rm/README.md` bestimmt die Reihenfolge. mdtoc erzeugt lokale ToCs; das Gesamt-PDF erhält vollständige Kapitelnummern. Das kurze UM bleibt getrennt. UM-Markdown nur auf ausdrücklichen Auftrag ändern.
- Kein `/v2`-Modulumbau. Installation über passende Release-Archive oder lokalen Tag-Checkout; Modulpfad und Imports bleiben unverändert. Beta-Tag wäre `v2.0.0-beta-1`; stabile Homebrew-Veröffentlichung bleibt davon getrennt.
- Keine neuen Abhängigkeiten, Suchindizes, mdBook-Ausgabe oder Änderungen an Archiven/`obsolete` ohne eigenen Auftrag. Keine automatische Umsetzung weiterer Planpunkte.
- Zwischenergebnisse unter `temp/`; Generatoren löschen ihre eigenen vorherigen Zwischenstände. Keine konkurrierenden zustandsverändernden Tests. Auf Windows Git Bash verwenden; Go-Builds/-Tests benötigen gemäß AGENTS.md Zugriff auf den externen Go-Cache.
- Commit, Push, Tag und Publish jeweils nur auf ausdrücklichen Auftrag. Der letzte Commit-Auftrag ist erledigt und gilt nicht für diese neue Übergabe.

## Offenes und Fortsetzung

- In der uncommitteten `scratchPad.md` steht eine CI-Meldung: `TestScratchpadModuleBoundary` erwartet `docs/scratchPad/go.mod`, das aktuell fehlt. Test vorhanden in `scripts/portability_test.go`. In R15 weder ausgeführt noch korrigiert. Der dort notierte Wunsch, scratchPad aus Tests auszuschließen, ist noch kein Implementierungsauftrag; zunächst mit dem Nutzer klären.
- R15: veröffentlichte Beta-Assets, endgültige Downloadziele und taggebundene Links nach ausdrücklich freigegebenem Tag/Publish abnehmen. R16: einen finalen Stand festlegen, native Plattform-/Paketprüfungen, Full-Matrix, Coverage und gegebenenfalls Hardwareabnahme ausführen. Windows-Cross-Builds ersetzen diese Nachweise nicht.
- Weitere Arbeit nur nach Auswahl: R27 Skripte, R28 Bind-Lücken, R29 leichte RM-Kürzung/Ordnung, R22 Pages, R30 alte RM-Spezifikation/Suchoptionen. Details und Prioritäten im Plan.
- Nach Rechnerwechsel zuerst Branch, HEAD, Arbeitsbaum und Plan prüfen. Fehlende Werkzeuge über die vorhandenen Build-/CI-Vorgaben bereitstellen; stitchmd v0.9.0 ist gepinnt, seine Anzeige `dev` allein ist kein Versionsfehler. Danach auf den nächsten Auftrag warten.

Lokale Nachweise und PDFs liegen in `temp/r15-logs`, `temp/r15-tools` und `temp/r15-workspace/docs`; Tool-/Renderer-Caches in `temp/r32-tools`. Sie sind ignoriert, werden nicht durch Git übertragen und sind für die Fortsetzung nicht erforderlich. Auf dem anderen Rechner bei Bedarf frisch erzeugen. Es läuft kein Test oder Generator aus dieser Arbeit mehr.

Für den Transfer diese Übergabe sowie gewünschte weitere lokale Änderungen manuell committen und die lokalen Commits normalerweise pushen. Die drei oben genannten fremden Änderungen müssen bewusst mit übertragen oder anderweitig gesichert werden.
