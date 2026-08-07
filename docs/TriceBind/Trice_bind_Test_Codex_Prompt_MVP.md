# Codex-Prompt: Umsetzung der `trice bind` MVP-Tests

Arbeite im Repository `trice` auf dem Branch `wip`.

## Verbindliche Grundlage

Lies zuerst vollständig:

```text
docs/scratchPad/TriceBind/Trice_bind_Test_Spezifikation_MVP.md
```

Diese Datei ist die einzige fachlich verbindliche Spezifikation für Testaufbau, Testzustände, Workflow-Skripte und Testorchestrierung. Wiederhole oder interpretiere ihre Anforderungen nicht eigenmächtig neu. Bei Widersprüchen zwischen diesem Prompt und der Testspezifikation gilt die Testspezifikation.

Lies außerdem die dort referenzierten Dokumente und vorhandenen Skripte, soweit sie zum Verständnis oder zur Umsetzung benötigt werden, insbesondere:

```text
docs/scratchPad/TriceBind/Trice_bind_Generator_Spezifikation.md
scripts/
examples/
_test/
```

Die Generator-Spezifikation definiert das Verhalten von `trice bind`; sie darf durch diese Aufgabe nicht fachlich geändert werden.

## Auftrag

Implementiere vollständig das in der Testspezifikation definierte **MVP-Testsystem** für `trice bind` neben dem bestehenden `trice insert`-/`trice clean`-Workflow.

Alle ausdrücklich als spätere Erweiterung, Option oder nicht zum MVP gehörig gekennzeichneten Punkte bleiben unimplementiert.

Der Schwerpunkt dieser Aufgabe liegt auf:

- Testorchestrierung,
- Workflow- und Re-Migrationsskripten,
- gemeinsamer Nutzung vorhandener Testlogik,
- PC-Target-Tests,
- Builds und Tests der relevanten Projekte unter `./examples`,
- CLI-Hilfetests,
- reproduzierbarer Zustandsherstellung und Wiederherstellung.

Die Testspezifikation enthält die vollständigen fachlichen Anforderungen; dupliziere sie nicht in neuen Dokumenten oder Kommentaren.

## Randbedingungen

- Keine beabsichtigten Breaking Changes.
- Bestehendes Verhalten und bestehende Tests von `trice insert`, `trice clean`, `trice bind`, Decoder, `til.json`, `li.json`, Drahtformat und vorhandenen Projekten erhalten.
- Bestehende Testlogik möglichst extrahieren, parametrisieren oder direkt wiederverwenden; keine unnötige Verdopplung von PC-Target- oder Example-Build-Kommandos.
- Keine dauerhaft auseinanderlaufenden Insert- und Bind-Kopien kanonischer C-/C++-Testquellen anlegen.
- Keine Bind-spezifischen `#if`-Blöcke in kanonische User-Testquellen einführen, sofern die Testspezifikation dies nicht ausdrücklich verlangt.
- Keine fachliche Änderung an der Test- oder Generator-Spezifikation vornehmen.
- Wenn eine Anforderung nicht kompatibel oder nicht eindeutig umsetzbar ist, dokumentiere die konkrete Konfliktstelle mit Datei, Symbol, Skriptablauf und Minimalbeispiel. Nimm in diesem Teilbereich keine eigenmächtige API-, Format- oder Semantikänderung vor.
- Kleine Fehler in der bestehenden `bind`-Implementierung, die eine ausdrücklich spezifizierte Prüfung blockieren, darfst du beheben. Halte solche Änderungen eng begrenzt und dokumentiere sie im Abschlussbericht.
- Normale Implementierungsdetails darfst du selbstständig entscheiden.

## Vorgehen

1. Analysiere vollständig:
   - die Testspezifikation,
   - die aktuelle `testAll`-Orchestrierung,
   - PC-Target-Tests,
   - Example-Build-Skripte,
   - bestehende Insert-/Clean-Hilfsskripte,
   - aktuelle Bind-Hilfsskripte und CLI-Hilfe,
   - Zustandssicherung, Cleanup und Trap-Behandlung.

2. Führe vor Änderungen geeignete Baseline-Tests aus. Mindestens:

```sh
./scripts/_testAll_10_PcTargetTests.sh
```

Verwende die im Repository tatsächlich vorhandene Aufrufsyntax. Dokumentiere bestehende Fehler getrennt von neu verursachten Fehlern.

3. Erstelle vor der ersten größeren Änderung einen kompakten Implementierungsplan mit:
   - betroffenen Dateien,
   - gemeinsamer Worker-/Wrapper-Struktur,
   - Zustandsübergängen und Cleanup,
   - Einbindung in `testAll.sh`,
   - PC-Target- und Example-Testmatrix,
   - vorgesehenen Testkommandos.

4. Implementiere die Testspezifikation schrittweise.

5. Führe während der Entwicklung regelmäßig die betroffenen Teiltests aus. Bevorzuge gezielte kurze Läufe; wiederhole nicht unnötig die gesamte Testsuite.

6. Führe vor Abschluss mindestens die in der Testspezifikation geforderten MVP-Tests sowie aus:

```sh
./scripts/testAll.sh quick
```

Falls ein Test wegen einer dokumentierten lokalen Toolchain- oder Plattformvoraussetzung nicht ausführbar ist, weise dies präzise nach. Ersetze einen nicht ausgeführten Test nicht stillschweigend durch eine schwächere Prüfung.

7. Verändere keine Spezifikationsdokumente. Ergänze nur Implementierung, Skripte, Tests und bei Bedarf knappe nutzungsbezogene Hinweise an den dafür vorgesehenen Stellen.

## Qualitätsanforderungen

- Gemeinsame Test-Worker statt kopierter Build- und Testabläufe.
- Insert- und Bind-Varianten müssen denselben fachlichen Testumfang verwenden, soweit die Testspezifikation dies fordert.
- Die relevanten Projekte unter `./examples` müssen in beiden ID-Workflows geprüft werden.
- Testzustände und Übergänge müssen reproduzierbar und idempotent sein.
- Abbruch, Fehler und Signale dürfen den Arbeitsbaum nicht in einem unklaren Zwischenzustand hinterlassen.
- Logs und Zusammenfassungen müssen den jeweiligen ID-Workflow eindeutig erkennen lassen.
- Keine unnötigen Refactorings außerhalb des für die Testspezifikation benötigten Umfangs.
- Shell-Skripte müssen zu den bestehenden Repositorykonventionen passen und Fehler zuverlässig weiterreichen.
- Veränderte Dateien dürfen nicht unnötig neu geschrieben werden.

## Abschlussbericht

Liefere nach der Umsetzung:

1. kurze Zusammenfassung der Testarchitektur,
2. Liste der geänderten Dateien,
3. Beschreibung der gemeinsam genutzten Worker- und Wrapper-Logik,
4. Beschreibung der implementierten Zustandsübergänge und Cleanup-Mechanismen,
5. Beschreibung der PC-Target- und Example-Testabdeckung für Insert/Clean und Bind,
6. neue oder geänderte öffentliche Hilfsskripte,
7. kleine notwendige Korrekturen an der bestehenden Bind-Implementierung,
8. ausgeführte Testkommandos mit Ergebnis,
9. nicht ausgeführte Tests mit genauer Begründung,
10. verbleibende MVP-Lücken,
11. ausdrücklich nicht implementierte spätere Optionen,
12. Abweichungen von der Testspezifikation oder mögliche Breaking Changes.

## Genehmigungen / Generalvollmacht

Frage nicht um Erlaubnis:

- um in GitHub oder im lokalen Repository etwas zu lesen,
- um Go-, C-, C++-, Shell- oder Testquellen zu verändern,
- um Dateien unter `./scripts`, `./examples`, `./_test`, `./internal` oder `./src` zu ändern,
- um mehr als fünf Dateien zu ändern,
- um Tools auszuführen,
- um Go-, C- oder C++-Code zu kompilieren,
- um Tests auszuführen,
- um temporäre Dateien oder Testarbeitsbäume anzulegen,
- um kleine, von der Testspezifikation verlangte CLI- oder Workflowfehler zu beheben.

## Verbote

- Du darfst nicht selbstständig committen oder pushen.
- Du darfst die Test- oder Generator-Spezifikation nicht ändern. Wenn eine Änderung sinnvoll erscheint, darfst du einen begründeten Vorschlag machen und mich fragen.
- Du darfst Testanforderungen nicht durch schwächere Ersatzprüfungen umgehen.
- Du darfst bestehende Insert-/Clean-Tests nicht entfernen oder reduzieren, um Bind-Tests grün zu bekommen.

## Wichtig

Bearbeite diese Aufgabe durchgehend, ohne zwischendurch Freigaben anzufordern. Wenn du für die Durchführung noch eine unverzichtbare Erlaubnis benötigst, frage jetzt danach.

## Startkommando

Beginne jetzt mit Repositoryanalyse, Baseline-Tests und Implementierungsplan. Fahre danach selbstständig mit der vollständigen Umsetzung der MVP-Testspezifikation fort.
