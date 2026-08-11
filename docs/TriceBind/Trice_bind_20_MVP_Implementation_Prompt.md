# Codex-Prompt: Implementierung von `trice bind` MVP

Arbeite im Repository `trice` auf dem Branch `wip`.

## Verbindliche Grundlage

Lies zuerst vollständig:

```text
docs/TriceBind/Trice_bind_10_MVP_Spezifikation.md
```

Diese Datei ist die einzige fachlich verbindliche Spezifikation. Wiederhole oder interpretiere ihre Anforderungen nicht eigenmächtig neu. Bei Widersprüchen zwischen diesem Prompt und der Spezifikation gilt die Spezifikation.

Lies anschließend die beiden verifizierten PoCs unter:

```text
experiments/TriceBind/30_Preprocessor_Verification/
```

Sie sind die technische Referenz für die dort geprüften Präprozessormechanismen.

## Auftrag

Implementiere ausschließlich das in der Spezifikation definierte **MVP** von:

```text
trice bind
```

Alle ausdrücklich als **MVP2**, spätere Erweiterung, Option oder verworfene Alternative gekennzeichneten Punkte bleiben unimplementiert.

## Randbedingungen

- Keine beabsichtigten Breaking Changes.
- Bestehendes Verhalten von `trice insert`, `trice clean`, Decoder, `til.json`, `li.json`, Drahtformat und vorhandenen Projekten erhalten.
- Keine zweite unabhängige Trice-Parser- oder ID-Logik implementieren.
- Bestehende Insert-Funktionalität möglichst extrahieren, parametrisieren oder direkt wiederverwenden.
- Keine fachliche Änderung an der Spezifikation vornehmen.
- Wenn eine Anforderung nicht kompatibel oder nicht eindeutig umsetzbar ist, dokumentiere die konkrete Konfliktstelle mit Datei, Symbol und Minimalbeispiel. Nimm in diesem Teilbereich keine eigenmächtige API-, Format- oder Semantikänderung vor.
- Normale Implementierungsdetails darfst du selbstständig entscheiden.

## Vorgehen

1. Analysiere Repository, aktuelle Insert-/Clean-Implementierung, Target-Makros, CLI-Infrastruktur, Parallelisierung und Tests.
2. Führe vor Änderungen den kurzen Baseline-Test aus:

   ```sh
   ./_testAll_10_PcTargetTests.sh quick
   ```

3. Erstelle einen kompakten Implementierungsplan mit:
   - betroffenen Dateien,
   - wiederverwendeter beziehungsweise zu extrahierender Insert-Logik,
   - Target-Makrointegration,
   - Generatorablauf,
   - Teststrategie.
4. Implementiere das MVP schrittweise.
5. Ergänze Unit-, Präprozessor-, Integrations-, Parallelitäts- und Regressionstests gemäß Spezifikation.
6. Verwende während der Entwicklung regelmäßig den u.U. einige Minuten dauernden Test (also Geduld!):

   ```sh
   ./_testAll_10_PcTargetTests.sh quick
   ```

7. Führe vor Abschluss mindestens aus:

   ```sh
   ./scripts/testAll.sh quick
   ```

8. Verändere oder entferne die vorhandenen PoCs nicht. Erstelle für den echten Generator bei Bedarf einen zusätzlichen unabhängigen PoC mit eigenem Namen.

## Qualitätsanforderungen

- Gemeinsame Logik für `insert` und `bind` statt Codeduplizierung.
- Deterministisches Verhalten trotz paralleler Verarbeitung.
- Keine teilweise gültigen regulären Ausgaben nach fatalen Fehlern.
- Unveränderte Dateien nicht unnötig neu schreiben.
- Alle erkennbaren Diagnosen sammeln und geordnet ausgeben.
- Neue öffentliche Optionen und Hilfeausgaben in die bestehende CLI-Struktur integrieren.
- Keine unnötigen Refactorings außerhalb des für das MVP benötigten Umfangs.

## Abschlussbericht

Liefere nach der Umsetzung:

1. kurze Architekturzusammenfassung,
2. Liste der geänderten Dateien,
3. Beschreibung der wiederverwendeten oder extrahierten Insert-Logik,
4. Beschreibung der Target-Makrointegration,
5. neue beziehungsweise geänderte CLI-Optionen,
6. neue und geänderte Tests,
7. ausgeführte Testkommandos mit Ergebnis,
8. verbleibende MVP-Lücken,
9. ausdrücklich nicht implementierte MVP2-Punkte,
10. Abweichungen von der Spezifikation oder mögliche Breaking Changes.

## Genehmigungen / Generalvollmacht

Frage nicht um Erlaubnis

- um in Github oder im lokalen Repo etwas zu lesen
- um die Go Quellen zu verändern
- um im ./src Folder etwas zu ändern
- um mehr als 5 Dateien zu ändern
- um Tools auszuführen
- um zu compilieren (Go oder C)
- um Tests auszuführen

## Verbote

- Du darfst nicht selbstständig etwas ins Repo comitten oder pushen.
- Du darfst die Spezifikation nicht ändern. Wenn es sinnvoll erscheint, dafst Du einen begründeten Vorschlag machen und mich fragen.

## Wichtig

Ich möchte, dass Du durchgehend diese Aufgabe bearbeitest ohne zwischendurch Freigaben anzufordern. Wenn Du noch Erlaubnisse brauchst, frage jetzt danach.

## Startkommando

Beginne jetzt mit Repositoryanalyse, Baseline-Test und Implementierungsplan. Fahre danach selbstständig mit der MVP-Implementierung fort.
