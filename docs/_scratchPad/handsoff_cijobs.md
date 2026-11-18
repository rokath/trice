# Handoff für Codex: Ereignisgesteuerte GitHub-Actions-CI für Trice

**Repository:** `rokath/trice` — <https://github.com/rokath/trice>  
**Stand der Bestandsaufnahme:** 18. November 2026  
**Status:** Implementierungsauftrag; **keine Änderungen sind durch diese Übergabe bereits vorgenommen worden**.

## 1. Auftrag und Ziel

Die CI soll auf Änderungen reagieren statt auch bei unverändertem Projekt regelmäßig stundenlange Testmatrizen auszuführen.

**Leitprinzip:**

> Geänderter Code bzw. geänderte Test-/Build-Konfiguration → geeignete Tests.  
> Unverändertes Repository → keine unnötigen vollständigen Regressionstests.  
> Neue externe Tool-Versionen → gesonderte, kleine Kompatibilitätsprüfung.  
> Volle Testmatrix bleibt jederzeit manuell startbar.

Konkret:

1. **Keine geplanten täglichen oder wöchentlichen Full-Regressionstests** mehr.
2. **Pull Requests nach `main`** weiterhin vor dem Merge überprüfen. Existierende Required Checks und ihr bisheriger inhaltlicher Prüfumfang dürfen nicht unbemerkt geschwächt werden.
3. **Push nach `main`** (etwa nach einem Merge) startet die vollständigen Native-Tests einschließlich macOS Intel sowie die vollständigen Library-Tests.
4. **`workflow_dispatch`** zum manuellen Start bleibt erhalten.
5. **Zukunftskompatibilität externer Tools** getrennt von der eigentlichen CI prüfen, vorerst **monatlich** und manuell startbar; kein Full-Matrix-Ersatz und kein verpflichtender PR-Check.
6. Logs, Artefakte und stabile Check-Namen erhalten. Regressionsfehler und CI-Infrastrukturprobleme möglichst eindeutig unterscheiden.

**Wichtig:** Primär geht es um **Ausführungshäufigkeit**, nicht um den Abbau von Testabdeckung. Änderungen am PR-Testumfang sind separat zu begründen und sollten nicht stillschweigend erfolgen.

## 2. Verifizierte Ausgangslage

Folgende Dateien wurden in `main` geprüft. Vor Implementierung den tatsächlichen aktuellen Stand erneut lesen, da die Dateien zwischenzeitlich geändert worden sein können.

| Datei | Aktueller Stand (18.11.2026) | Ziel |
| --- | --- | --- |
| `.github/workflows/native-tests.yml` | `pull_request`, manuell, täglich `30 1 * * *`; PR: Quick auf Ubuntu/Windows/macOS 26; non-PR: Full plus macOS Intel | Schedule entfernen; `push` auf `main` ergänzen; PR Quick, Main Full, manuell Quick/Full korrekt berücksichtigen |
| `.github/workflows/native-tests-reusable.yml` | Reusable Runner; Full Intel/Windows bis zu 360 Minuten; Tool-Setup und Testartefakte | Grundsätzlich beibehalten; lediglich nötige Diagnoseverbesserungen im separaten Arbeitsschritt |
| `.github/workflows/trice_lib_ci.yml` | PR + täglicher Quick-Schedule + manuell; Guard prüft Commits der letzten 24 h; **PR benutzt trotz Workflow-Namen derzeit `configs_mode: full`** | Täglichen Schedule und überflüssigen Guard entfernen; PR/Manuell unverändert funktionsfähig halten |
| `.github/workflows/trice_lib_ci_full.yml` | Täglicher Full-Schedule + manuell; Guard auf Commits der letzten 24 h | Schedule durch `push` auf `main` ersetzen; Guard entfernen |
| `.github/workflows/tool-compatibility.yml` | `workflow_dispatch` + wöchentlicher Schedule `15 6 * * 1`; nutzt absichtlich `stable` und `latest` | Nur dieser Tool-Kompatibilitätstest monatlich, z. B. erster Montag im Monat |
| `.github/workflows/coverage.yml` | PR + Push auf `main` + monatlicher Schedule; Job `coverage` | Monatlichen Schedule entfernen, weil PR/Main bereits getestet werden; manuell behalten; irreführenden Workflow-Namen/Kommentar ggf. aktualisieren |
| `.github/dependabot.yml` | Wöchentliche GitHub-Actions- und npm-Updates | Nicht im Hauptauftrag ändern; eigener Folgeschritt zur Update-Stabilität |

Links:

- <https://github.com/rokath/trice/blob/main/.github/workflows/native-tests.yml>
- <https://github.com/rokath/trice/blob/main/.github/workflows/trice_lib_ci.yml>
- <https://github.com/rokath/trice/blob/main/.github/workflows/trice_lib_ci_full.yml>
- <https://github.com/rokath/trice/blob/main/.github/workflows/tool-compatibility.yml>
- <https://github.com/rokath/trice/blob/main/.github/workflows/coverage.yml>

### 2.1 Eine wichtige Inkonsistenz im bisherigen Gespräch

Der Name `TRICE Library CI (Nightly Quick)` ist irreführend: Sein PR-Job benutzt aktuell explizit `configs_mode: full`, um die vollständige Konfigurationsmatrix **vor dem Merge** zu prüfen. **Diese Absicherung nicht versehentlich auf `quick` reduzieren.** Wenn die PR-Matrix später bewusst verkleinert werden soll, ist das eine eigene Abwägung mit dokumentierter Konsequenz für Branch Protection und Merge-Sicherheit.

### 2.2 Warum ein 24-Stunden-Guard nicht ausreicht

Ein geplanter Workflow startet selbst dann einen GitHub-Actions-Lauf und ggf. einen kurzen Guard-Job, wenn `main` unverändert blieb. Er entspricht daher nicht dem Ziel, vollständig ohne unnötige planmäßige CI-Läufe auszukommen. Außerdem kann eine täglich laufende Prüfung nach einem verpassten Zeitfenster (oder durch Scheduler-Verzögerungen) ungewollt ausfallen. Der direkte Trigger `push` auf `main` ist einfacher und zuverlässiger.

## 3. Zielmatrix der Ereignisse

| Ereignis | Native-Tests | Library Quick-Workflow | Library Full-Workflow | Go Coverage | Future tool compatibility |
| --- | --- | --- | --- | --- | --- |
| PR nach `main` | Quick: Ubuntu, Windows, macOS ARM; **kein Intel** | **bestehender PR-Prüfumfang** (aktuell `full`) | kein Lauf | Lauf (wie bisher) | kein Lauf |
| Push nach `main` | Full: Ubuntu, Windows, macOS ARM, macOS Intel | kein zusätzlicher Lauf erforderlich | Full | Lauf (wie bisher) | kein Lauf |
| Manuell: Native Quick | Quick: Ubuntu, Windows, macOS ARM; **kein Intel-Full-Job** | – | – | – | – |
| Manuell: Native Full | Full auf allen vier Plattformen | – | – | – | – |
| Manuell: Library Quick/Full | – | im jeweiligen Workflow | im jeweiligen Workflow | optional | optional |
| Geplante Routine bei unverändertem `main` | **kein Lauf** | **kein Lauf** | **kein Lauf** | **kein Lauf** | **nur monatlicher, kleiner Tool-Test** |

**Hinweis:** GitHub löst PR- und Push-Workflows unabhängig aus. Bei Merge kann daher zusätzlich zur bereits absolvierten PR-Prüfung eine Full-Prüfung des resultierenden `main`-Commits folgen. Das ist beabsichtigt: Die vollständige integrierte Hauptbranch-Version wird geprüft.

## 4. Implementierungsdetails

### 4.1 `.github/workflows/native-tests.yml`

Den täglichen `schedule`-Trigger entfernen und `push` auf `main` ergänzen. Repräsentative Zielstruktur:

```yaml
name: Native Trice tests

on:
  pull_request:
    branches: [main]
  push:
    branches: [main]
  workflow_dispatch:
    inputs:
      selection:
        description: Native test scope
        type: choice
        options: [full, quick]
        default: full
```

Die vorhandene Matrix `ubuntu-24.04`, `windows-2025`, `macos-26` erhalten. Auswahl bleibt sinngemäß:

```yaml
selection: ${{ github.event_name == 'pull_request' && 'quick' || inputs.selection || 'full' }}
```

**Besonderheit manuelle Quick-Läufe:** Der jetzige Intel-Job hat `if: github.event_name != 'pull_request'` und startet deshalb auch bei `workflow_dispatch(selection=quick)` fälschlich **immer Full auf Intel**. Korrigieren, z. B.:

```yaml
if: ${{ github.event_name == 'push' || (github.event_name == 'workflow_dispatch' && (inputs.selection == 'full' || inputs.selection == '')) }}
```

Die Guard-Bedingung an die tatsächlich verwendeten Input-Defaults anpassen und **alle drei Fälle** (`PR`, `push`, `manual quick/full`) prüfen.

`race`-Semantik beibehalten: Quick-PR ohne Race, Main-Full bzw. manuelles Full mit Race; bei manuellem Quick idealerweise kein unnötiger Race-Lauf. Die derzeitige Bedingung `race: ${{ github.event_name != 'pull_request' }}` sollte deshalb ebenfalls geprüft und gegebenenfalls expliziter gemacht werden.

**Concurrency:** Neue Pushes auf `main` sollen keine lange Warteschlange mit obsoleten Full-Läufen erzeugen. Eine sichere Policy entwickeln, die alte Push-Läufe bei neueren Pushes möglichst abbrechen darf, **manuelle Läufe aber nicht unbeabsichtigt mit `main`-Pushes in dieselbe Gruppe mischt**. PRs weiterhin pro PR-Nummer entprellen. Beispielprinzip: getrennte Concurrency-Gruppen nach Ereignistyp und PR-Nummer/Ref; nicht blind jede `workflow_dispatch`-Ausführung abbrechen.

### 4.2 `.github/workflows/trice_lib_ci.yml`

- `schedule` vollständig entfernen.
- `pull_request` nach `main` sowie `workflow_dispatch` behalten.
- Den ausschließlich für `schedule` existierenden `guard_activity`-Job entfernen.
- `needs: guard_activity` am `heavy_ci`-Job entfernen.
- Das alte `if` mit Bezug auf den Guard entfernen bzw. sinnvoll vereinfachen.
- Vorhandene Konfigurations- und Toolchain-Parameter beibehalten, **insbesondere PR `configs_mode: full`**, solange nicht ausdrücklich eine Änderung der Pre-Merge-Testabdeckung beschlossen wird.
- **Bestehende Job-/Required-Check-Namen** (`Heavy CI (quick)` etc.) nach Möglichkeit stabil lassen; Änderung des Workflow-Anzeigenamens zur Entfernung von `Nightly` nur nach Prüfung der Regeln und Dokumentation.

Die manuelle Ausführung muss auch ohne den Guard weiterhin laufen.

### 4.3 `.github/workflows/trice_lib_ci_full.yml`

- `schedule` durch `push: branches: [main]` ersetzen.
- `workflow_dispatch` behalten.
- `guard_activity` und die davon abhängigen `needs`/`if`-Bedingungen vollständig entfernen.
- Den Full-Umfang (`configs_mode: full`, alle vorgesehenen Toolchains und Artefakte) unverändert lassen.
- Wenn mehrere Pushes schnell aufeinander folgen, über eine geeignete `concurrency`-Policy nachdenken; alte Full-Läufe sollen optional durch neuere ersetzt werden, manuelle Runs dürfen nicht unbeabsichtigt abgebrochen werden.
- Full-Lauf darf keine PR-Pflichtprüfung blockieren, die nur bei Push nach `main` existiert.

### 4.4 `.github/workflows/coverage.yml`

- Monatlichen `schedule` entfernen.
- `pull_request`, `push` auf `main` und `workflow_dispatch` beibehalten.
- Überschrift/Kommentare anpassen, weil „PR + Monthly“ dann falsch ist. Job-ID `coverage` und ggf. exakte Required-Check-Bezeichnungen nicht versehentlich ändern.
- Vorhandene Coveralls-Integration und Permissions unverändert halten.

### 4.5 `.github/workflows/tool-compatibility.yml`

- Als **separaten, bewusst einzigen periodischen Kompatibilitäts-Canary** behalten.
- Frequenz von wöchentlich auf monatlich ändern. Empfehlung: **am 1. jedes Monats** um 06:15 UTC:

```yaml
on:
  workflow_dispatch:
  schedule:
    - cron: '15 6 1 * *'   # Am 1. jedes Monats, 06:15 UTC
```

Bei einem alternativen Wunschtermin wie „erster Montag“ GitHubs Cron-Semantik beachten: Einschränkungen von Tag-des-Monats und Wochentag können per **ODER** verknüpft werden. Einen solchen Termin daher nicht unvalidiert durch eine kombinierte Cron-Expression ausdrücken.

- `stable`/`latest` **nur hier** sind sinnvoll, denn der Workflow soll gerade wechselnde externe Versionen prüfen.
- Laufzeitbegrenzung (30 Minuten), Artefakte und manuelle Startbarkeit behalten.
- Kompatibilitätsausfälle als **Wartungshinweise**, nicht als ungeprüfte Regression des für Trice fixierten Build-Vertrags behandeln.
- Falls der Benutzer wirklich **gar keine** periodischen Tests möchte, ist eine zweite akzeptable Variante: `schedule` auch hier entfernen und den Kompatibilitätstest rein manuell ausführen. Die bevorzugte Variante ist ein monatlicher Canary.

### 4.6 Andere Workflows inventarisieren

Alle `.github/workflows/*.yml` bzw. `.yaml` auf `schedule` prüfen und nach Zweck klassifizieren:

1. **Regression/Build/Tests:** Wenn sie dasselbe Verhalten testen wie PR-/Push-CI, möglichst auf Änderungen statt Uhrzeit umstellen.
2. **Tool-/OS-Kompatibilität:** Getrennte, seltene Canary-Checks; kein Volltest ohne Änderung.
3. **Echte Wartungsaufgaben:** z. B. Statistiken, Backups, Labelpflege, Release-Checks, Sicherheitsprüfungen. Deren Schedules nicht allein wegen dieses Auftrags löschen.

Ergebnis im PR kurz dokumentieren: Welche Schedules bleiben bewusst und warum? **Keine pauschale Abschaltung aller GitHub-Actions-Zeitpläne.**

## 5. Required Checks, Branch Protection und Pfadfilter

**Besonders wichtig, um keine blockierten PRs zu erzeugen:**

- Vor Umbenennung/Entfernung von Jobs prüfen, welche Check-Namen als `required` in Branch-Protection- oder Repository-Rulesets hinterlegt sind.
- Den Full-Library-Workflow (Push auf `main`) **nicht** als Required Check für PR-Merges voraussetzen: Vor dem Merge existiert sein Push-Ereignis nicht.
- Bei `pull_request`-Workflows nicht leichtfertig top-level `paths`-Filter hinzufügen: GitHub kann erforderliche Checks bei übersprungenem Workflow auf `Pending` lassen.
- Falls später Docs-only-PRs geschont werden sollen, besser einen immer anwesenden, stabilen Pflicht-Job mit interner Änderungsprüfung implementieren oder die Branch-Regeln bewusst anpassen.
- Die Umstellung soll im Zweifel **Fail Closed** bleiben: ungeprüfte Änderungen dürfen nicht durch einen falsch übersprungenen Required Check gemergt werden.

**Out of scope:** Ein umfassendes Umschreiben der Tests oder der Branch-Protection-Policy ist nicht nötig, um die CI-Häufigkeit zu korrigieren.

## 6. Nachweis/Tests vor dem Merge

1. YAML syntaktisch und hinsichtlich GitHub-Actions-Ausdrücken prüfen (`actionlint`, ggf. weitere vorhandene Repository-Tests).
2. `git diff --check` und alle im Repo vorgesehenen Workflow-/Script-Lints ausführen.
3. Die Trigger-/Matrix-Tabelle aus Abschnitt 3 gegen die tatsächlichen `if`- und `with`-Bedingungen abgleichen. Besonders wichtig sind `workflow_dispatch(quick)` und `macos-intel`.
4. Prüfen, dass es keine toten `needs: guard_activity`-Referenzen oder `if`-Bedingungen mehr gibt, die Pull Requests oder manuelle Läufe skippen.
5. PR mit Workflow-Änderungen anlegen; PR-Checks vollständig laufen lassen und Statusnamen kontrollieren.
6. Nach Merge auf `main` prüfen, dass Native Full und Library Full **einmalig aufgrund des Push-Ereignisses** starten.
7. Ohne neue Commits darf am nächsten Tag keine Native-/Library-Full-/Coverage-Nightly-Regressionsprüfung starten.
8. Einen manuellen **Native Quick**-Lauf prüfen: kein macOS-Intel-Full-Job. Manueller **Native Full**-Lauf: Intel enthalten.
9. Monatlicher Tool-Canary: Cron nachvollziehbar, manuelle Ausführung möglich, keine unnötigen Full-Matrix-Jobs.
10. Teste, dass zuletzt archivierte Logs/Artefakte bei regulär scheiternden Schritten verfügbar bleiben. Einen Runner-Abbruch ohne Artefakt nicht als automatisch behobenen Produktfehler interpretieren.

## 7. Konkrete Abnahmekriterien

- [ ] Kein `schedule` mehr in Native-Tests, Library Quick, Library Full und Go Coverage.
- [ ] Native PRs: unverändert Quick auf Linux/Windows/macOS ARM; Intel wird nicht gestartet.
- [ ] Native Push auf `main`: Full auf Linux/Windows/macOS ARM/macOS Intel.
- [ ] Native manueller Quick-Lauf: kein heimlicher Intel-Full-Lauf.
- [ ] Native manueller Full-Lauf: alle vier Plattformen.
- [ ] Library Quick: PR und manuell funktionieren; Required Checks unverändert; bestehende PR-Testabdeckung nicht versehentlich verringert.
- [ ] Library Full: Push auf `main` und manuell funktionieren; keine zeitgesteuerten Läufe.
- [ ] Coverage: PR/Push/Manuell bleiben erhalten; kein monatlicher redundanter Regressionstest.
- [ ] Future tool compatibility: bewusst monatlich (`1 * *`) + manuell, ohne Full-Matrix.
- [ ] Alle entfernten Schedule-Guards samt Abhängigkeiten sind sauber entfernt.
- [ ] Branch Protection/Rulesets und benötigte Check-Namen sind berücksichtigt.
- [ ] Concurrency vermeidet möglichst veraltete Läufe, ohne manuelle Läufe irrtümlich abzubrechen.
- [ ] Sonstige geplante Workflows sind inventarisiert; Wartungsjobs nicht versehentlich abgeschaltet.
- [ ] Keine Änderung am eigentlichen Trice-Funktionscode erforderlich.
- [ ] `actionlint` und geeignete CI-Tests bestehen.
- [ ] PR-Beschreibung enthält die Vorher/Nachher-Ereignismatrix und ausdrücklich verbleibende periodische Jobs.

## 8. Getrennte Folgeaufgaben – nicht mit diesem Trigger-PR vermischen

### 8.1 Dependabot PR #799: doppelte Puppeteer-Versionsquelle

<https://github.com/rokath/trice/pull/799>

Bei der Analyse wurde festgestellt:

- `scripts/testcheck/package.json` und `package-lock.json` werden durch Dependabot auf Puppeteer 25.17.0 aktualisiert.
- `scripts/test-tools.env` enthält separat `TRICE_TOOL_PUPPETEER_VERSION=25.16.0`.
- Tests und Skripte (`scripts/testcheck/tools_test.go`, `scripts/tool_plan_test.go`, `scripts/_116_test_tool_plan.sh`, `scripts/_118_test_npm_tools.cjs`, weitere Stellen) vergleichen mit der alten separaten Version bzw. benutzen Hardcodings.
- **Nachhaltige Lösung:** genau eine normative npm-Versionsquelle (Manifest/Lockfile), trotzdem strenge Prüfung der tatsächlich installierten Versionen. Keine Tests abschalten, nur um Dependabot grün zu bekommen.

**Eigenen PR** für die Architekturkorrektur vorsehen; keine bloße Aktualisierung hartcodierter Versionen als langfristige Lösung.

### 8.2 Intel-macOS-Job im Lauf #44271369190

<https://github.com/rokath/trice/actions/runs/44271369190>

Beobachtung vom 18.11.2026:

- Ubuntu 24.04, Windows 2025 und macOS 26: erfolgreich.
- `macos-15-intel`: fehlgeschlagen, nach nahezu exakt drei Stunden; Tool-Setup und Intel-LLVM-Vorbereitung erfolgreich.
- `Run the native shared test plan` wurde nicht regulär abgeschlossen; `Upload native evidence even after failure` lief nicht; kein Intel-Artefakt vorhanden.
- Das eigentliche Log war zum Analysezeitpunkt über die GitHub-Schnittstelle nicht verfügbar (`BlobNotFound`).
- Der Reusable Workflow sieht für Full Intel **360 Minuten** vor; aus den Daten allein folgt **nicht**, warum trotzdem bei etwa 180 Minuten abgebrochen wurde. Mögliche Runner-/Infrastrukturunterbrechung, äußere Begrenzung oder Test-Hänger **untersuchen**, nicht ungeprüft behaupten.
- Am 17.11.2026 war ein Intel-Full-Lauf erfolgreich; am 16.11.2026 scheiterten zwei Host-Integrationstests im Intel-Lauf. Die Ursachen können unterschiedlich sein.

**Eigene Folgeaufgabe:** lange Full-Suite in sinnvoll isolierbare Teiljobs zerlegen, Fortschritt und Artefakte zwischenspeichern, bei Ausfällen den letzten ausgeführten Test erkennen können. Häufigkeitsreduktion ersetzt keine Root-Cause-Analyse.

### 8.3 Gezieltes Pinning

Normale reproduzierbare CI möglichst mit den im Projekt deklarierten festen Versionen betreiben. `stable` und `latest` nur in expliziten Zukunftskompatibilitätstests verwenden. Nicht beiläufig in diesem Trigger-PR alle Toolchain- oder Action-Versionen aktualisieren.

## 9. Gewünschtes Codex-Ergebnis

1. Einen kleinen, überprüfbaren Pull Request, der primär die **Workflow-Trigger, Guard-Abhängigkeiten und relevante Conditions/Concurrency** ändert.
2. Eine klare Dokumentation von Ereignis → tatsächlich ausgeführter CI-Stufe.
3. Tests/Validierungen mit Ergebnissen und eventuellen Einschränkungen.
4. Eine getrennte Liste offener Probleme (`#799` npm-Versionsquelle; Intel-Runner-Diagnostik), statt sie durch Scheduling zu verstecken.
5. **Kein** bloßes Abstellen von Fehlerbenachrichtigungen und **keine** generelle Abschaltung der Testabdeckung.

**Akzeptanz in einem Satz:** Solange weder Trice noch seine Test-/Build-Konfiguration geändert wird, darf die umfangreiche Regressionstest-Matrix nicht von selbst starten; nach einem relevanten PR bzw. Main-Push wird sie in der definierten Tiefe ausgeführt, und der separate seltene Tool-Kompatibilitäts-Canary bleibt kontrolliert verfügbar.
