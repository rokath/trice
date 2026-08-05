# Spezifikation: `trice bind` MVP-Tests

**Version:** 1  
**Status:** Spezifikationsentwurf für das MVP-Testsystem  
**Repository-Bezug:** Branch `wip`, insbesondere `scripts`, `_test`, `examples`, `internal/id` und `docs/scratchPad/TriceBind`  
**Zielgruppe:** Implementierung, Review, CI und Release-Absicherung

## 1. Ziel

Diese Spezifikation legt fest, wie der neue `trice bind`-Workflow neben dem bestehenden `trice insert`/`trice clean`-Workflow getestet wird.

Die Tests MÜSSEN nachweisen, dass:

- die Host-ID-Verwaltung für `bind` und `insert` fachlich konsistent ist,
- dieselben PC-Target-Tests in beiden ID-Workflows erfolgreich laufen,
- die relevanten Projekte unter `./examples` sowohl mit eingesetzten IDs als auch mit Sidecar-Binding gebaut werden können,
- bestehende `insert`- und `clean`-Abläufe nicht regressieren,
- Migration und Re-Migration reproduzierbar funktionieren,
- Testabbrüche den Arbeitsbaum nicht in einem unklaren Zwischenzustand hinterlassen.

Die Generatorfunktion ist in `Trice_bind_Generator_Spezifikation.md` definiert. Dieses Dokument spezifiziert ausschließlich Testaufbau, Testzustände und Orchestrierung.

## 2. Normative Begriffe

- **MUSS:** verbindliche Anforderung des MVP-Testsystems.
- **SOLL:** erwartetes Verhalten; Abweichungen benötigen einen dokumentierten Grund.
- **KANN:** optionale Eigenschaft.

## 3. Grundprinzip

Der fachliche Testinhalt darf nicht für jeden ID-Workflow separat nachimplementiert werden.

Für einen gegebenen Testfall gilt:

```text
gemeinsame Testquellen und Erwartungen
                 |
        +--------+--------+
        |                 |
  Insert-Vorbereitung  Bind-Vorbereitung
        |                 |
        +--------+--------+
                 |
      derselbe Build/Testlauf
```

Die Unterschiede zwischen den Varianten liegen ausschließlich in:

- der Herstellung des Sourcezustands,
- den zusätzlichen Include-Pfaden für Sidecars,
- der abschließenden Zustandsbereinigung beziehungsweise Wiederherstellung.

Buildkommandos, Compileroptionen, Testdaten und Erfolgskriterien MÜSSEN soweit technisch möglich gemeinsam verwendet werden.

Bind-spezifische `#if`-Blöcke in kanonischen User-Testquellen SOLLEN vermieden werden. Der Generator soll seine Includes wie im realen Projekt selbst einfügen.

## 4. Testzustände

### 4.1 Clean

Die verwalteten Trice-Aufrufe enthalten keine IDs größer null. Eigene Bind-Sidecar-Includes und Sidecars sind entfernt, sofern der Test ausdrücklich einen reinen Clean-Zustand verlangt.

Dieser Zustand ist die gemeinsame Ausgangsbasis für eine vollständige Migration.

### 4.2 Inserted

Die verwalteten Trice-Aufrufe enthalten die durch `trice insert` eingesetzten IDs. Bind-Sidecar-Includes sind für diese Dateien nicht aktiv.

### 4.3 Bound

Die Bind-owned Dateien enthalten ID-freie Trice-Aufrufe beziehungsweise Null-Platzhalter und ihre dateilokalen Sidecar-Includes. Die Sidecars liegen standardmäßig unter:

```text
./build/triceIDs
```

`til.json` und `li.json` entsprechen demselben fachlichen ID-Bestand wie im Inserted-Zustand.

### 4.4 Ungültige Zwischenzustände

Folgende Zustände dürfen nicht stillschweigend als erfolgreiche Testvorbereitung gelten:

- eine Datei ist nach der Generator-Spezifikation `mixed`,
- ein benötigter Sidecar fehlt,
- eine Insert-owned Datei bleibt unbeabsichtigt in einem aktiven Bind-Kontext,
- eine Bind-owned Datei wird zusätzlich durch `trice insert` instrumentiert,
- nur ein Teil der gemeinsam getesteten Quellen wurde umgestellt.

## 5. Öffentliche Workflow-Skripte

Die vorhandenen Skripte bleiben erhalten:

```text
trice_insertIDs_in_examples_and_test_folder.sh
trice_cleanIDs_in_examples_and_test_folder.sh
```

Zusätzlich MÜSSEN bereitgestellt werden:

```text
trice_bindIDs_in_examples_and_test_folder.sh
trice_remigrateBindToClean_in_examples_and_test_folder.sh
```

Die endgültige Benennung darf an die bestehenden Repositorykonventionen angepasst werden. Die Semantik ist verbindlich.

### 5.1 Gemeinsame Konfiguration

Alle vier Skripte MÜSSEN dieselbe Projekt- und Listen-Konfiguration verwenden, insbesondere die Werte aus:

```text
scripts/_setup_trice_environment.sh
```

Dazu gehören:

- `TRICE_DEFAULTS`,
- `TRICE_ALIASES`,
- `TRICE_PRJ_FILES`,
- `TRICE_TIL_JSON`,
- `TRICE_LI_JSON`,
- `TRICE_LI_ROOT`,
- identische ID-Grenzen.

Die Listen der bearbeiteten `_test`- und `examples`-Quellen dürfen nicht unabhängig in mehreren Skripten gepflegt werden.

### 5.2 Bind-Skript

`trice_bindIDs_in_examples_and_test_folder.sh` MUSS:

1. `trice bind` mit denselben Source-, Alias-, Listen- und ID-Optionen wie das Insert-Skript aufrufen,
2. `./build/triceIDs` bei Bedarf durch `trice bind` erzeugen lassen,
3. fehlende Sidecar-Includes erzeugen lassen,
4. bei wiederholtem Aufruf idempotent sein,
5. bei Fehlern mit einem Fehlerstatus enden.

### 5.3 Re-Migrationsskript

`trice_remigrateBindToClean_in_examples_and_test_folder.sh` MUSS einen Bind-Zustand in einen insert-bereiten Clean-Zustand überführen.

Es MUSS:

- eindeutig zuordenbare eigene Sidecar-Includes entfernen,
- generierte Sidecars des verwalteten Testumfangs entfernen,
- die ID-freien beziehungsweise Null-Platzhalter-Trice-Aufrufe unverändert lassen,
- keine beliebigen Include-Zeilen anhand unspezifischer Textsuche löschen,
- bei mehrdeutigen oder nicht sicher zuordenbaren Stellen fehlschlagen und die Datei nennen,
- bei wiederholtem Aufruf idempotent sein.

Es führt nicht zwingend selbst `trice insert` aus. Der anschließende Insert-Schritt bleibt als eigener, sichtbarer Workflow-Schritt erhalten:

```text
remigrate Bind → Clean
trice insert
```

Ein optionaler Wrapper darf beide Schritte atomar kombinieren, muss jedoch intern dieselben Funktionen verwenden.

## 6. CLI-Tests

Mindestens folgende Aufrufe MÜSSEN mit Exitstatus `0` enden:

```sh
trice bind -h
trice bind --help
trice b -h
trice b --help
```

Die Hilfe darf nicht mit einer Fehlerzeile wie folgender enden:

```text
flag: help requested
```

und keine durch `log.Fatal` erzeugte Zeitstempel-Fehlerausgabe enthalten.

Die bestehenden Kurzformen, insbesondere:

```sh
trice i -h
```

dürfen nicht regressieren.

Die Hilfe für `bind` MUSS die für `bind` gültigen gemeinsamen Insert-Optionen sowie die bind-spezifischen Optionen anzeigen.

## 7. Testorchestrierung ohne Codeduplizierung

### 7.1 Gemeinsamer Test-Worker

PC-Target- und Example-Build-Tests SOLLEN jeweils aus zwei Ebenen bestehen:

1. **Workflow-Wrapper**
   - stellt Inserted oder Bound her,
   - setzt notwendige Umgebungsvariablen und Include-Pfade,
   - ruft den gemeinsamen Worker auf,
   - stellt den vorherigen Zustand wieder her.

2. **Gemeinsamer Worker**
   - führt die eigentlichen Builds oder Tests aus,
   - enthält keine ID-Workflow-spezifische Sourceinstrumentierung,
   - erhält den Workflow über einen klaren Parameter oder eine Umgebungsvariable.

Beispielhafte interne Schnittstelle:

```text
TRICE_ID_WORKFLOW=insert
TRICE_ID_WORKFLOW=bind
```

Die genaue Bezeichnung ist nicht normativ.

### 7.2 Keine doppelten kanonischen Testquellen

Es SOLL nur eine kanonische Fassung von `triceCheck.c` und den jeweiligen Example-Quellen geben.

Für Tests dürfen temporäre Kopien verwendet werden. Dauerhafte, manuell auseinanderlaufende Insert- und Bind-Kopien derselben C-Datei sind zu vermeiden.

### 7.3 Getrennte Logs

Insert- und Bind-Varianten MÜSSEN getrennte Logs und getrennte Ergebniszeilen in `testAll_summary.log` erhalten. Ein Fehler muss eindeutig einem Workflow zuordenbar sein.

## 8. PC-Target-Tests

Der derzeitige Schritt:

```text
scripts/_testAll_10_PcTargetTests.sh
```

verwendet einen Ablauf nach dem Muster:

```text
insert → PC-Target-Tests → clean
```

Der gleiche Testumfang MUSS zusätzlich im Bound-Zustand ausgeführt werden.

### 8.1 Gleicher Testumfang

Für beide Varianten MÜSSEN identisch sein:

- die ausgewählten Go-Pakete,
- `quick`- beziehungsweise `full`-Auswahl,
- `demoTIL.json` und `demoLI.json`,
- Cache-Bereinigung,
- Umgebungsbehandlung wie `C_INCLUDE_PATH`,
- erwartete Testausgaben,
- Logauswertung und Fehlerkriterien.

Im Quick-Modus betrifft dies mindestens denselben aktuellen Paketsatz wie die Insert-Variante. Im Full-Modus betrifft es denselben vollständigen `_test`-Umfang.

### 8.2 Mögliche Skriptstruktur

Eine zulässige Struktur ist beispielsweise:

```text
scripts/_testAll_10_PcTargetTests_common.sh
scripts/_testAll_10a_PcTargetTests_insert.sh
scripts/_testAll_10b_PcTargetTests_bind.sh
```

Alternativ darf `_testAll_10_PcTargetTests.sh` beide Varianten orchestrieren, sofern:

- die gemeinsame Testlogik nur einmal implementiert ist,
- getrennte Logs beziehungsweise klar getrennte Logabschnitte entstehen,
- die Varianten einzeln aufrufbar bleiben.

### 8.3 Bind-Vorbereitung

Die Bind-Variante MUSS vor dem Test:

- die relevanten `_test`- und Example-Datenquellen in einen konsistenten Bound-Zustand versetzen,
- `./build/triceIDs` in den C/CGO-Include-Pfad aufnehmen,
- danach denselben Go-Testworker wie die Insert-Variante aufrufen.

## 9. Example-Builds

Alle relevanten Projekte unter:

```text
./examples
```

MÜSSEN sowohl im Inserted- als auch im Bound-Zustand gebaut werden können.

Dies gilt nicht nur für den separaten Bind-PoC, sondern für den bestehenden Example-Buildumfang.

### 9.1 Bestehender Umfang

Die bestehende Testorchestrierung umfasst insbesondere:

- `examples/buildAllTargets_TRICE_ON.sh`,
- `examples/buildAllTargets_TRICE_OFF.sh`,
- die G0B1-X0-Matrix,
- die Clang-Übersetzung für `examples/G0B1_inst`,
- die in `scripts/_setup_trice_environment.sh` genannten instrumentierten Example-Quellen.

Dieser Umfang darf durch die Bind-Erweiterung nicht reduziert werden.

### 9.2 TRICE_ON-Matrix

Jeder Example-Build, der im bestehenden `TRICE_ON`-Ablauf mit eingesetzten IDs gebaut wird, MUSS zusätzlich mit Sidecar-Binding gebaut werden.

Für beide Varianten müssen identisch sein:

- Targetkonfiguration,
- Compiler und Compileroptionen,
- Linkeroptionen,
- Trice-Transport- und Puffereinstellungen,
- Warnungs- und Fehlerkriterien,
- erwartete Buildartefakte.

Der einzige fachliche Unterschied ist der ID-Workflow.

### 9.3 TRICE_OFF

`TRICE_OFF` ist nicht vom ID-Bindemechanismus abhängig und muss nicht künstlich doppelt ausgeführt werden. Der bestehende `TRICE_OFF`-Build bleibt jedoch Teil von `testAll.sh` und darf nicht regressieren.

### 9.4 Clang- und GCC-Builds

Soweit ein Example heute sowohl mit GCC als auch in einem gesonderten Clang-Schritt gebaut wird, SOLL der `TRICE_ON`-Build in beiden ID-Workflows geprüft werden.

Ist ein benötigter externer Compiler nicht installiert, gilt weiterhin die bestehende SKIP-/WARN-Semantik.

### 9.5 Include-Pfad

Im Bound-Zustand MUSS jeder betroffene Example-Build den konfigurierten `bindDir`, standardmäßig:

```text
./build/triceIDs
```

als Include-Pfad erhalten.

Diese Ergänzung SOLL zentral über die bestehende Buildumgebung oder eine gemeinsame Workflowvariable erfolgen. Sie darf nicht unabhängig und unterschiedlich in jedem Example-Projekt gepflegt werden.

### 9.6 Example-Skriptintegration

Die individuellen Example-Buildskripte dürfen ihre bisherige Insert-/Clean-Logik nicht parallel zu einer äußeren Bind-Orchestrierung ausführen.

Es MUSS genau einen Eigentümer für den ID-Workflow geben.

Eine mögliche Struktur ist:

```text
äußerer Testschritt
    → setzt TRICE_ID_WORKFLOW
    → gemeinsamer Workflow-Helper bereitet Sources vor
    → bestehender Example-Build
    → gemeinsamer Workflow-Helper räumt auf
```

Die einzelnen Example-Buildkommandos bleiben dabei möglichst unverändert.

## 10. Bind-spezifische Generator- und Targettests

Der vorhandene Bind-Testschritt unter:

```text
scripts/_testAll_09b_BindTests.sh
```

bleibt bestehen und SOLL mindestens prüfen:

- Generator-Unit- und Integrationstests,
- C- und C++-Targetkompilation,
- die beiden verifizierten Präprozessor-PoCs,
- den unabhängigen Generator-PoC,
- Decodierung des erzeugten Datenstroms,
- kanonische `triceCheck.c`-Abdeckung.

Diese Tests ersetzen nicht die doppelte Ausführung der allgemeinen PC-Target- und Example-Buildtests.

## 11. Migration und Re-Migration

### 11.1 Migrationstest

Ein vollständiger Migrationstest MUSS mindestens abdecken:

```text
Inserted
→ trice clean
→ trice bind
→ Bound-Build und Tests
```

Zu prüfen sind:

- numerische IDs bleiben fachlich stabil,
- `til.json` und `li.json` bleiben konsistent,
- Sidecar-Includes und Sidecars werden erzeugt,
- die Originalquellen enthalten danach keine IDs größer null,
- die Targets erzeugen decodierbare Trice-Daten.

### 11.2 Re-Migrationstest

Ein vollständiger Rückweg MUSS mindestens abdecken:

```text
Bound
→ Sidecar-Includes und Sidecars entfernen
→ trice insert
→ Inserted-Build und Tests
```

Zu prüfen sind:

- keine verwaisten aktiven Bind-Kontexte,
- IDs werden wieder korrekt in die User-Trice-Aufrufe eingesetzt,
- `til.json` und `li.json` bleiben konsistent,
- der Build entspricht dem bestehenden Insert-Workflow.

### 11.3 Sichtbare manuelle Abnahmesequenz

Für eine manuelle Gesamtprüfung ist folgende Sequenz geeignet:

```sh
./trice_bindIDs_in_examples_and_test_folder.sh
./scripts/testAll.sh quick

./trice_remigrateBindToClean_in_examples_and_test_folder.sh
./trice_insertIDs_in_examples_and_test_folder.sh
./scripts/testAll.sh quick

./trice_cleanIDs_in_examples_and_test_folder.sh
./trice_bindIDs_in_examples_and_test_folder.sh
```

Nach vollständiger Integration testet bereits ein einzelner `testAll.sh`-Lauf beide Workflows intern. Die Sequenz bleibt als sichtbarer Wartungs- und Abnahmeworkflow nützlich.

## 12. Zustandswiederherstellung und Abbruchverhalten

Automatisierte Tests MÜSSEN ihren Ausgangszustand bei folgenden Ausgängen wiederherstellen:

- Erfolg,
- Build- oder Testfehler,
- `SIGINT`,
- `SIGTERM`.

Die Wiederherstellung muss einen zentralen Eigentümer besitzen. Verschachtelte Wrapper dürfen nicht unabhängig dieselben Dateien bereinigen.

Zulässige Strategien sind:

- Sicherung und exakte Wiederherstellung aller betroffenen Dateien und Listen,
- Ausführung in einem temporären Projektbaum,
- ein anderer nachweislich transaktionaler Mechanismus.

Ein pauschales abschließendes `trice clean` reicht nicht, wenn der Ausgangszustand Bound oder Inserted war.

Die Tests SOLLEN prüfen, dass sich der Status der versionierten Dateien vor und nach `testAll.sh` nicht ändert. Ignorierte Buildartefakte dürfen erzeugt und kontrolliert bereinigt werden.

## 13. `testAll.sh`-Integration

### 13.1 Quick

`./scripts/testAll.sh quick` MUSS mindestens ausführen:

- bestehende Host- und Generator-Tests,
- Bind-Unit-/PoC-Tests,
- PC-Target-Quicktests im Inserted-Zustand,
- dieselben PC-Target-Quicktests im Bound-Zustand,
- den vorhandenen Quick-Example-Buildumfang im Inserted-Zustand,
- denselben `TRICE_ON`-Example-Buildumfang im Bound-Zustand,
- bestehenden `TRICE_OFF`-Umfang.

### 13.2 Full

`./scripts/testAll.sh full` MUSS zusätzlich den bisherigen Full-Umfang ausführen. Die Bind-Variante erhält denselben Full-PC-Target- und Example-Umfang, soweit der Build nicht workflowunabhängig ist.

### 13.3 Entwicklungszyklus

Während der Inbetriebnahme darf als kurzer Zyklus weiterhin verwendet werden:

```sh
./scripts/_testAll_10_PcTargetTests.sh
```

beziehungsweise dessen neue workflowbezogenen Teilschritte.

Vor Merge MUSS mindestens:

```sh
./scripts/testAll.sh quick
```

erfolgreich laufen. Der bestehende Full-Lauf bleibt die abschließende Regression, ist aber nicht für jede lokale Iteration erforderlich.

## 14. Logs und Ergebnisdarstellung

Jeder Workflowtest MUSS im Log erkennen lassen:

- vorbereiteter ID-Zustand,
- bearbeitete Quellen,
- verwendetes `til.json` und `li.json`,
- verwendeter `bindDir`,
- Build- beziehungsweise Testkommando,
- Wiederherstellungsaktion,
- Endstatus.

Die Zusammenfassung soll beispielsweise getrennte Einträge enthalten:

```text
PcTargetTests insert: PASS
PcTargetTests bind: PASS
GccExampleBuilds insert: PASS
GccExampleBuilds bind: PASS
ClangExampleBuild insert: PASS
ClangExampleBuild bind: PASS
```

## 15. Fehlerkriterien

Ein Test MUSS fehlschlagen, wenn mindestens einer der folgenden Fälle eintritt:

- ein CLI-Hilfeaufruf liefert einen Fehlerstatus oder `flag: help requested`,
- Insert- und Bind-Varianten verwenden nicht denselben fachlichen Testumfang,
- ein `TRICE_ON`-Example baut nur im Inserted-, aber nicht im Bound-Zustand,
- ein Bound-Build findet einen Sidecar nicht,
- ein Target erzeugt nicht decodierbare Daten,
- ID-, Typ-, Formatstring- oder Location-Ergebnisse unterscheiden sich unerwartet,
- der Arbeitsbaum bleibt nach dem Test in einem anderen versionierten Zustand,
- ein Cleanup- oder Restore-Schritt schlägt fehl,
- der Test überspringt eine Variante ohne eine explizite Werkzeug- oder Plattformbegründung.

## 16. Abnahmekriterien

Das MVP-Testsystem gilt als vollständig, wenn:

1. `trice b -h` und `trice bind -h` sauber funktionieren.
2. Das Bind-Hilfsskript vorhanden und idempotent ist.
3. Die Re-Migration in einen insert-bereiten Clean-Zustand reproduzierbar ist.
4. Die allgemeinen PC-Target-Tests mit identischem Umfang unter Insert und Bind laufen.
5. Alle bestehenden relevanten `TRICE_ON`-Example-Builds unter Insert und Bind laufen.
6. Die bestehende `TRICE_OFF`-Abdeckung erhalten bleibt.
7. GCC- und vorhandene Clang-Example-Prüfungen den Bind-Zustand berücksichtigen.
8. Gemeinsame Test- und Buildlogik nicht für beide Workflows dupliziert ist.
9. Migration und Re-Migration automatisiert getestet werden.
10. Abbruch und Fehler den Ausgangszustand wiederherstellen.
11. `./scripts/testAll.sh quick` beide Workflows vollständig abdeckt.
12. Die bestehenden Insert-, Clean-, Decoder- und Drahtformat-Tests unverändert grün bleiben.
