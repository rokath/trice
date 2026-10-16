# Übergabe CI und Statistik

## Ziel und Git-Stand

Stand: 16. Oktober 2026. CI-Korrekturen und Statistik-Einrichtung abschließen;
verbleibende Veröffentlichungs- und Testnachweise klar offen halten.

- Lokaler Branch: `wip`.
- HEAD: `1ad74cc7b04f28c951e6936425b3ef7370625e35` (`wip ci test fixes`).
- Arbeitsbaum vor dieser Übergabe sauber; nur diese neue Übergabe ist uncommitted.
- Laut Nutzer ist PR #748 bereits gemergt und PR #749 inzwischen geschlossen.
  Diese letzten Zustandsänderungen wurden nicht erneut remote geprüft.
  Lokales `main` und Remote-Refs wurden dafür nicht aktualisiert.
- Keine Commits, Pushes, PR-Änderungen oder Workflowstarts durch diese Übergabe.

## Bereits in Git enthalten

- [Coverage-Workflow](../../.github/workflows/coverage.yml): `go test -count=1`
  verhindert gemischte Coverage-Profile aus dem Go-Testcache. Alte Profile hatten
  94 zusätzliche scheinbar ungedeckte Zeilen erzeugt. Linux-Coveralls stieg von
  88.257 % auf 88.843 %; gegenüber dem Main-Vergleich +0.008 Prozentpunkte.
- [Bind-Kapitel](../rm/bind.md): Zwei überflüssige HTML-Anker samt Begleitkommentaren
  entfernt. Seit mdtoc 1.0.3 erzeugten die Überschriften dieselben IDs; die
  Duplikate ließen den PDF-/Snapshot-Build scheitern.
- Elf veraltete Ankerziele in [examples.md](../rm/examples.md) sind bereits in
  `631197d4` korrigiert. mdtoc 1.0.3 aktualisierte nur vorhandene TOCs;
  `_scratchPad` blieb ausgeschlossen. Keine neuen TOCs anlegen.
- Vorherige Bind-/TCP-Korrekturen sind enthalten; Details und zurückgestellte
  Aufgaben stehen in der [Aufgabensammlung](Implementierungsplan.md).

## Statistik und PRs

- [statistics.yml](../../.github/workflows/statistics.yml) sammelt täglich um
  23:43 UTC oder manuell auf dem Default-Branch. Die unveränderte Upstream-Aktion
  schreibt Messwerte und Berichte dauerhaft nach `github-repo-stats`.
- [pages.yml](../../.github/workflows/pages.yml) liest den Bericht direkt aus
  diesem Datenbranch und veröffentlicht ihn unter `/trice/stats/`. Ein
  erfolgreicher Statistiklauf kann den Pages-Build auslösen.
- Der Datenbranch bleibt erhalten und wird nicht nach `main` gemergt.
  Einrichtung und Grenzen: [github_statistics.md](../rm/github_statistics.md).
- PR #749 enthielt nur vier automatisch erzeugte Daten-/Berichtscommits. GitHub
  nennt `rokath` als PR-Ersteller; ob per Klick oder Tool, ist nicht belegt.
  Der PR war für die Statistik-Veröffentlichung unnötig und ist laut Nutzer
  geschlossen. Nicht erneut öffnen oder seine Berichte manuell umformatieren.
- Der letzte Lintlauf von #749 scheiterte an neun Markdown-Diagnosen in
  `rokath/trice/latest-report/report.md`: Überschriftensprünge, fehlende
  Leerzeilen, eingebettetes `script` und Dateischluss. Der anschließende
  Kommentarversuch bekam HTTP 403. Diese Fehler wurden nicht gepatcht und
  betrafen nicht den grünen PR #748. Keine Lintregeln oder Tokenrechte geändert.

## Geprüfte Ergebnisse

Prüfung | Ergebnis
--- | ---
CI für PR #748 auf `1ad74cc7` | Super-Linter PASS (Run `38912949764`); Coverage PASS mit 88.843 % (Run `38912950505`, Coveralls-Build `82410899`); Snapshot und Installationsprüfungen PASS (Run `38912950270`). Vor dem gemeldeten Merge geprüft.
`go test -count=1 ./... -covermode=atomic -coverprofile=./temp/log/ci-review-20261016-coverage-fresh.out -coverpkg=./...` | PASS auf Windows. Covered-line-Auswertung: 12557/14161 = 88.6731 %; kein direkter Vergleich mit Linux wegen anderer Betriebssystempfade.
`bash ./scripts/_320_generate_trice_user_manual_pdf.sh` | PASS; beide PDFs erzeugt. Interne RM-Linkprüfung: 54 Quellen, 913 Links, 1268 eindeutige Anker PASS.
`./scripts/_310_refresh_trice_user_manual.sh check` | PASS. Repository-Markdownlint ebenfalls PASS; installierte CLI direkt über Node aufgerufen.
`actionlint -shellcheck= .github/workflows/coverage.yml` | PASS. Integrierter ShellCheck-Aufruf war lokal blockiert; deshalb dieser begrenzte Check.
`./scripts/testAll.sh full` vom 16. Oktober | ABORTED/130 nach 4743 s. Schritte 400–580 sowie 600/610 PASS; 590 FAIL durch äußeren Eingriff; 620 nach 13m28s abgebrochen; 630/640 nicht ausgeführt. Kein vollständiges Full-PASS.
Isolierte G0B1-Insert-Matrix auf unverändertem `1ad74cc7` | Alle vier `TRICE_OFF`/X0-Kombinationen einschließlich zusätzlicher Off-Builds PASS, Exit 0. Danach alle 2432 versionierten Dateien bytegenau mit dem Git-Archiv identisch.

### Ursache des Fehlers in Schritt 590

Codex hielt während des parallel laufenden Nutzer-Tests dessen vorübergehend
Inserted C-Dateien fälschlich für eigene Testrückstände und stellte sie wieder
auf den eingecheckten Bind-Zustand zurück. Dadurch kamen Bind-Includes während
des Insert-Builds zurück; die gemeldeten fehlenden Sidecar-Header waren die Folge.
Kein zusätzlicher Skriptfehler wurde in der isolierten Wiederholung festgestellt.

Während eines verwalteten Testlaufs keine Quellen bearbeiten oder wiederherstellen.
Diagnosen mit Quelländerungen ausschließlich in einer separaten Kopie aus Git
ausführen. Der abgebrochene Full-Lauf meldete außerdem Änderungen an
`coverage.yml` und `bind.md`, weil diese gewollten, inzwischen committeten
CI-Fixes während des Laufs entstanden. Sie nicht zurücksetzen.

Die isolierte Wiederholung verwendete in einer eigenen Git-Archiv-Kopie:

```bash
source ./scripts/_140_trice_test_state.sh
trice_test_run_managed_workflow insert example "$PWD/examples/G0B1_inst/build.sh" --x0-matrix
```

Lokale Belege: `temp/log/testAll_summary.log`,
`temp/log/ci-review-20261016-coverage-analysis.json` und
`temp/ci-review-20261016-gcc-isolated/temp/log/insert-matrix.log`.
Diese ignorierten Diagnoseartefakte sind auf anderen Rechnern nicht vorhanden;
für die Fortsetzung sind keine lokalen Zusatzdateien nötig.

## Offen und nächster Schritt

1. Zuerst den tatsächlichen Remote-Stand prüfen: #748 gemergt, #749 geschlossen;
   anschließend lokale Basis mit `main` abgleichen. Den erledigten Merge nicht
   erneut als offene Aufgabe behandeln.
2. Statistiklauf auf `main`, nachfolgenden Pages-Lauf und sichtbaren aktuellen
   Bericht unter `/trice/stats/` prüfen. Diese Veröffentlichung wurde hier noch
   nicht nach dem gemeldeten Merge abgenommen. Noch kein Workflowstart beauftragt.
3. Den neuen Teststand bei der nächsten Pflege der Aufgabensammlung übernehmen:
   restliche Zielbuilds aus 590, vollständiger Schritt 620 und 630/640 sind lokal
   nicht durch diesen Lauf abgenommen. Die isolierte Matrix ersetzt nur den
   betroffenen G0B1-Teil. Kein neuer kompletter Full-Lauf beauftragt.
4. Windows-Laufzeitoptimierung P05 und Bind-/Insert-Zeitvergleich P06 bleiben
   ausdrücklich zurückgestellt; Messbasis und Grenzen stehen bereits in der
   Aufgabensammlung. Jetzt keine Optimierung beginnen.
