# Codex-Prompt: Umsetzung von `trice bind` MVP2 mit lokalem `__COUNTER__`-Rebase

Arbeite im Repository `trice` ausgehend vom aktuellen Branch `wip`.

## Branch und Arbeitsbaum

1. Prüfe zuerst den aktuellen Branch und den Arbeitsbaum mit Git.
2. Verwirf, überschreibe oder stash keine vorhandenen Useränderungen eigenmächtig.
3. Wechsle auf den aktuellen lokalen Branch `wip`, sofern dies ohne Verlust vorhandener Änderungen möglich ist.
4. Lege davon den neuen Branch an und checke ihn aus:

```text
feature/trice-bind-mvp2-rebase
```

5. Falls der Branch bereits existiert oder der Arbeitsbaum einen sicheren Branchwechsel verhindert, dokumentiere den konkreten Zustand. Verändere keine fremden Arbeiten.
6. Committe und pushe nicht selbstständig.

## Verbindliche Grundlagen

Lies vor Änderungen vollständig:

```text
docs/TriceBind/Trice_bind_10_MVP_Spezifikation.md
docs/TriceBind/Trice_bind_60_MVP2_Implementation_Strategies.md
docs/TriceBind/Trice_bind_90_MVP_User_Manual.md
experiments/TriceBind/60_MVP2_Local_Counter_Rebase/README.md
```

Analysiere außerdem den vollständigen ausführbaren PoC unter:

```text
experiments/TriceBind/60_MVP2_Local_Counter_Rebase/
```

und die aktuelle produktive Bind-Implementierung, insbesondere:

```text
internal/id/
src/triceBind.h
src/trice.h
src/triceOn.h
src/triceOff.h
scripts/
_test/
examples/
```

Die bestehende MVP-Spezifikation bleibt für das bisherige `trice bind`-Verhalten verbindlich. Das Strategiepapier ist eine technische Entscheidungsgrundlage. Für diese Aufgabe ist **Variante 1: zeilenlokaler Counter-Rebase** ausgewählt. Das erfolgreiche Experiment `60_MVP2_Local_Counter_Rebase` ist die technische Referenz für den lokalen Rebase, die Compile-Time-Prüfungen und die Fehlerfälle.

Ändere die Generator-Spezifikation und das Strategiepapier nicht. Erweitere das User Manual wie weiter unten verlangt.

## Auftrag

Erweitere die bestehende produktive `trice bind`-Implementierung um die hier definierte MVP2-Ausbaustufe.

Die Erweiterung muss unterstützen:

1. mehrere direkte ID-freie Trice-Aufrufe auf derselben physischen Sourcezeile;
2. gewöhnliche Statement-Makros mit statischen Formatstrings und einem oder mehreren direkten Trice-Aufrufen in ihrer Replacement-Liste;
3. mehrere Aufrufe eines solchen Wrappermakros auf derselben physischen Sourcezeile;
4. Source- und Headerdateien;
5. C und C++;
6. stabile Trice-ID und Location Information an jeder inneren Trice-Definitionsstelle eines Wrappermakros;
7. Wiederverwendung dieser stabilen Definitions-ID an allen Wrapperaufrufstellen;
8. lokal begrenzte, sichere Nutzung von `__COUNTER__` nur dort, wo Datei und Zeile allein eine konkrete Expansion nicht eindeutig unterscheiden können.

Der normale Userworkflow bleibt:

```text
trice bind
Build
```

Der User darf weder Counterwerte noch lokale Ordinale pflegen oder als Trice-IDs verwenden.

## Keine projektweite `__COUNTER__`-Anforderung

Die Counter-Abhängigkeit darf nicht projektweit aktiviert werden.

### Normale Dateien und Bereiche

Enthält eine physische Source- oder Headerdatei ausschließlich Stellen, die mit dem vorhandenen `File Key + __LINE__`-Mechanismus eindeutig gebunden werden können, dann gilt:

- keine `__COUNTER__`-Verwendung;
- kein Counter-Feature-Guard;
- kein Rebase-Helper;
- keine neue Warnung;
- unverändertes bisheriges Bind-Verhalten.

Dazu gehören insbesondere:

- direkte Trice-Aufrufe auf jeweils eigener physischer Zeile;
- normale Funktionen;
- `inline`- und `static inline`-Funktionen;
- ein einfacher Wrapper mit genau einer inneren Trice-Stelle, sofern jede Wrapperaufrufstelle durch Datei und Zeile eindeutig ist und daher ohne Counter auf die Definitions-ID abgebildet werden kann.

### Counter-pflichtige Bereiche

Counter-Rebase und Capability-Guard werden ausschließlich für erkannte Bereiche erzeugt, in denen Datei und Zeile nicht ausreichen. Dazu gehören mindestens:

- mehrere direkte Trice-Aufrufe auf derselben physischen Zeile;
- ein Wrappermakro mit mehreren inneren Trice-Stellen;
- mehrere Wrapperaufrufe auf derselben physischen Zeile;
- Kombinationen daraus.

Die Entscheidung wird pro physischer Datei und möglichst pro kleinster sicher umschließbarer Aufrufregion getroffen.

## Headerdateien und Translation Units

Behandle Headerfälle ausdrücklich und teste sie.

### Wrapperdefinition in einem Header, Aufruf in einer Source-Datei

Beispiel:

```c
// logging.h
#define LOG_ERROR(value)      \
    do {                      \
        trice("first");      \
        trice("second=%d", value); \
    } while (0)
```

```c
// module.c
#include "logging.h"

void f(void)
{
    LOG_ERROR(7);
}
```

Anforderungen:

- Die beiden inneren Trice-Stellen besitzen stabile IDs und Location Information an ihren Definitionsstellen in `logging.h`.
- Der lokale Rebase gehört zur Aufrufregion in `module.c`.
- Die Aufrufstellendeskriptoren in `module.c` verweisen in Definitionsreihenfolge auf die beiden stabilen IDs aus `logging.h`.
- Weitere Aufrufe in anderen Source-Dateien verwenden dieselben Definitions-IDs.

### Wrapperaufruf oder mehrere direkte Trices in einem Header

Liegt eine counter-pflichtige Aufrufregion in einem Header, wird der Rebase in beziehungsweise für diesen Header erzeugt.

Dann gilt:

- jede Translation Unit, die diesen Header verarbeitet, benötigt `__COUNTER__`;
- der lokale Rebase muss unabhängig davon funktionieren, wie viele Counterwerte vor dem Header oder in früheren Includes verbraucht wurden;
- die generierten Compile-Time-Prüfungen müssen in jeder Translation Unit wirksam sein;
- ein gewöhnlicher Header ohne counter-pflichtige Stellen darf keine Counter-Abhängigkeit erhalten.

### Lokale statt globale Wirkung

Ein counter-pflichtiger Header macht nur die Translation Units counter-abhängig, die diesen Header tatsächlich verarbeiten. Er macht nicht das gesamte Projekt und keine unbeteiligten Dateien counter-abhängig.

## Fachliche Identität bei Wrappermakros

Unterscheide strikt:

- **Definitionsstelle:** die Trice-Stelle innerhalb der Makrodefinition;
- **Aufrufstelle:** eine konkrete Expansion des Wrappermakros.

Normatives Verhalten:

1. Jede innere Trice-Stelle einer Wrapperdefinition erhält genau eine stabile ID.
2. Ihre Location Information zeigt deterministisch auf die Definitionsstelle.
3. Mehrere Aufrufstellen erzeugen keine neuen fachlichen IDs.
4. Aufrufstellendeskriptoren wählen lediglich die zur Definitionsreihenfolge passende stabile ID aus.
5. Ein Wrapper mit zwei inneren Trices besitzt zwei stabile IDs, unabhängig von der Anzahl seiner Aufrufe.
6. Runtime-Zweige innerhalb des Wrappers ändern nicht die Anzahl der Präprozessor-Expansionen und nicht die ID-Zuordnung.

`til.json`, `li.json` und das Trice-Drahtformat bleiben unverändert.

## Lokaler Counter-Rebase

Implementiere den in `experiments/TriceBind/60_MVP2_Local_Counter_Rebase` verifizierten Ansatz produktionsnah in der vorhandenen Bind-Architektur.

Für jede counter-pflichtige Aufrufregion gilt:

1. unmittelbar vor der Region wird ein lokaler Counter-Basiswert erfasst;
2. jede Trice-Expansion verwendet nur die Differenz zwischen aktuellem Counter und lokalem Basiswert;
3. die lokalen Ordinale werden in textueller beziehungsweise expansionsbezogener Reihenfolge auf stabile IDs abgebildet;
4. Counterverbrauch vor der Region ist irrelevant;
5. nach der Region wird wieder auf den normalen file-key- und zeilenbasierten Bind-Dispatch zurückgeschaltet;
6. es entsteht kein veränderlicher globaler Laufzeitzustand;
7. es entsteht keine dynamische Speicherbelegung;
8. der Counter selbst ist niemals die Trice-ID.

Halte dich möglichst an den verifizierten Begin-/End- und Routing-Mechanismus des PoCs. Weiche nur ab, wenn die Integration in die bestehende produktive Makroschicht dies nachweislich erfordert. Dokumentiere jede relevante Abweichung und belege sie durch gleichwertige oder stärkere Tests.

## Schutz vor stiller Fehlzuordnung

Ein falscher Counterverlauf darf niemals erfolgreich kompilieren und dabei eine formal vorhandene, aber fachlich falsche ID auswählen.

Erzeuge deshalb für jede Rebase-Region Compile-Time-Prüfungen mindestens für folgende Invarianten:

1. Jede einzelne Trice-Expansion liegt innerhalb des erwarteten lokalen Ordinalbereichs.
2. Am Ende der Region wurde exakt die erwartete Anzahl Counterwerte verbraucht.
3. Ein zusätzlicher fremder `__COUNTER__`-Verbrauch innerhalb der Region führt sicher zu einem Compile-Time-Fehler.
4. Ein fehlender oder widersprüchlicher Aufrufstellendeskriptor führt zu einem Bind- oder Compilerfehler.
5. Nach dem Ende der Region ist der normale Bind-Dispatch wieder aktiv.

Nutze eine Compile-Time-Assert-Abstraktion, die zur bereits unterstützten C- und C++-Sprachbaseline des Projekts passt. Erhöhe nicht stillschweigend die minimale C- oder C++-Version. Falls die aktuelle Baseline eine andere Technik als `_Static_assert` beziehungsweise `static_assert` verlangt, implementiere eine kompatible Abstraktion oder dokumentiere den konkreten Konflikt, bevor du die Sprachbaseline änderst.

## Compiler- und Buildsystemunabhängigkeit

`trice bind` kennt normalerweise den Targetcompiler nicht. Daraus folgt:

- keine neue Pflichtoption für Compilername oder Compilerfamilie;
- keine GCC-, Clang-, MSVC- oder Herstellererkennung als Grundlage der ID-Zuordnung;
- kein zufällig gefundener Hostcompiler als angeblicher Nachweis der Targetfähigkeit;
- kein verpflichtender externer Präprozessorlauf;
- keine `compile_commands.json`-Pflicht;
- keine Shadow-Source als Buildinput.

Verwende ausschließlich den lokal generierten Target-Guard für counter-pflichtige Regionen:

```c
#if !defined(__COUNTER__)
#error "trice bind: this source construct requires __COUNTER__; use trice insert/clean, split the Trice calls, or replace the logging macro with a function"
#endif
```

Die konkrete Meldung darf an Repositorykonventionen angepasst werden, muss aber mindestens enthalten:

- dass nur diese erweiterte Sourcekonstruktion `__COUNTER__` benötigt;
- dass normale Bind-Stellen davon nicht betroffen sind;
- die Alternativen `trice insert/clean`, Trennung der Aufrufe auf einzelne Zeilen oder Umstellung eines Wrappers auf eine normale beziehungsweise `static inline`-Funktion.

Wenn keine counter-pflichtige Stelle existiert, darf diese Prüfung nirgends im generierten Compilerinput erscheinen.

## Begrenzter Funktionsumfang dieser Ausbaustufe

Unterstütze zunächst ausschließlich klar analysierbare Konstruktionen:

- ID-freie Trice-Aufrufe;
- statische, direkt erkennbare Formatstrings;
- gewöhnliche Statement-Makros;
- ein oder mehrere direkte Trice-Aufrufe pro unterstütztem Wrapper;
- mehrere Aufrufe unterstützter Wrapper;
- Source- und Headerdateien;
- normale Statement-Kontexte, für die der Rebase sicher eingefügt werden kann.

Zunächst nicht unterstützt und mit präziser Diagnose zurückzuweisen sind:

- `id(0)`, `Id(0)` oder `ID(0)` innerhalb einer counter-selektierten Rebase-Region, solange der Replace-Pfad nicht separat produktionsnah verifiziert wurde;
- explizite IDs größer null innerhalb counter-selektierter Bind-Stellen;
- verschachtelte oder rekursive Logging-Wrapper;
- Token-Pasting im Wrapper;
- Stringification zur Erzeugung oder Veränderung des Trice-Formatstrings;
- dynamisch zusammengesetzte Formatstrings;
- indirekte Umdefinitionen von Trice-Makros;
- unbekannter oder nicht sicher begrenzbarer zusätzlicher `__COUNTER__`-Verbrauch innerhalb der Rebase-Region;
- Sourcekontexte, in denen der benötigte Begin-/End-Mechanismus syntaktisch nicht sicher eingefügt werden kann;
- widersprüchliche Expansion derselben Stelle über mehrere Buildkonfigurationen, wenn keine eindeutige gemeinsame Semantik herstellbar ist.

Kein stiller Fallback auf Source-Instrumentierung ist zulässig. `trice insert` und `trice clean` bleiben der explizite universelle Fallback.

## Generatorverhalten

Erweitere die bestehende Bind-Analyse; implementiere keine zweite unabhängige Trice-Parser- oder ID-Logik.

Die Generatorerweiterung muss mindestens:

1. physische Zeile und Spalte jeder relevanten Trice-Stelle erfassen;
2. mehrere direkte Stellen derselben Zeile in Source-Reihenfolge ordnen;
3. unterstützte Wrapperdefinitionen und ihre inneren Trice-Stellen in Definitionsreihenfolge erfassen;
4. jeder inneren Definitionsstelle eine stabile bestehende oder neue Trice-ID zuordnen;
5. Wrapperaufrufe erkennen und Aufrufstellendeskriptoren erzeugen;
6. intern klassifizieren, ob eine Stelle den normalen Zeilenpfad oder den Counter-Rebase benötigt;
7. Begin-/End- beziehungsweise gleichwertige Rebase-Artefakte nur für counter-pflichtige Regionen erzeugen;
8. normale Sidecars und normale Bind-Stellen unverändert weiterverwenden;
9. wiederholte Bind-Läufe idempotent behandeln;
10. vorhandene generierte Rebase-Marker erkennen, aktualisieren und nicht duplizieren;
11. Re-Migration beziehungsweise vorhandene Cleanup-Mechanismen so erweitern, dass generierte Rebase-Includes oder Marker eindeutig entfernbar sind;
12. bei fatalen Fehlern weder teilweise Sidecars noch teilweise aktualisierte `til.json`-/`li.json`-Dateien hinterlassen;
13. vorhandene Parallelität und deterministische ID-Vergabe erhalten.

Generierte Sourceergänzungen müssen eindeutig als `trice bind`-Artefakte markiert sein und dürfen nicht manuell gepflegte Userkonstruktionen fälschlich entfernen.

## Target-Makroschicht

Erweitere die bestehende zentrale Bind-Makroschicht. Vermeide pro Trice-Familie kopierte Sonderimplementierungen.

Anforderungen:

- Der bestehende normale `BIND`-Pfad bleibt unverändert für gewöhnliche Stellen.
- Ein lokaler `REBASE`-Pfad wird nur innerhalb einer generierten Rebase-Region ausgewählt.
- Alle unterstützten User-Level-Trice-Familien verwenden dieselbe zentrale Ordinal- und ID-Auswahl, soweit ihre Signatur dies erlaubt.
- Nach Ende der Region ist wieder der normale `BIND`-Pfad aktiv.
- Insert-, Clean-, Off- und normaler Bind-Betrieb dürfen nicht regressieren.
- Interne Transport-, Pack-, Buffer-, Decoder- und Drahtformatverträge bleiben unverändert.

## Sourcepositionen und Location Information

Zusätzliche generierte Includes oder Marker können physische Zeilen verschieben. Stelle sicher:

- stabile IDs bleiben an den fachlichen Definitionsstellen erhalten;
- `li.json` zeigt bei Wrappern auf die innere Trice-Definitionsstelle;
- direkte mehrere Trice-Stellen derselben Zeile erhalten getrennte IDs, aber dieselbe physische Zeile und unterschiedliche Source- beziehungsweise Definitionsordinale;
- wiederholte Bind-Läufe verändern IDs nicht;
- ein Sourceedit mit anschließendem `trice bind` aktualisiert alle generierten Aufrufstellendeskriptoren korrekt;
- die Kommentare und Diagnosen nennen verständlich Definitions- und Aufrufstelle, wenn beide relevant sind.

## Tests

Die neu hinzukommenden fachlichen Tests dürfen sich auf `trice bind` beschränken. Du musst keine parallele neue Insert-/Clean-Testmatrix für jede MVP2-Konstruktion erstellen.

Bestehende Regressionstests für Insert, Clean und den bisherigen Bind-MVP müssen dennoch grün bleiben.

### Vorhandener PoC

Führe vor Änderungen aus:

```sh
cd experiments/TriceBind/60_MVP2_Local_Counter_Rebase
./run.sh
```

Verändere oder entferne den PoC nicht. Er bleibt unabhängiger technischer Nachweis.

### Erforderliche neue positive Bind-Tests

Implementiere mindestens:

1. zwei direkte Trice-Aufrufe auf derselben Sourcezeile;
2. drei oder mehr direkte Trice-Aufrufe auf derselben Sourcezeile;
3. Wrapper mit genau einer inneren Trice-Stelle und eindeutigem Aufruf pro Zeile ohne unnötige Counter-Abhängigkeit;
4. Wrapper mit mehreren inneren Trice-Stellen;
5. mehrere Wrapperaufrufe auf unterschiedlichen Zeilen;
6. mehrere Wrapperaufrufe auf derselben Zeile;
7. Wrapperdefinition in einem Header und Aufruf in zwei unterschiedlichen `.c`-Dateien;
8. Wrapperaufruf in einem Header, der von mindestens zwei Translation Units eingebunden wird;
9. mehrere direkte Trice-Stellen auf einer Zeile in einem Header;
10. fremden `__COUNTER__`-Verbrauch vor einer Rebase-Region ohne Einfluss auf die IDs;
11. stabile Definitions-IDs und identische `til.json`-/`li.json`-Zuordnung über mehrere Aufrufstellen;
12. wiederholten `trice bind`-Lauf ohne unnötige Dateiänderungen;
13. erneuten Bind-Lauf nach Sourceverschiebungen;
14. unverändertes normales Binding in Dateien und Headern ohne counter-pflichtige Stellen.

### Erforderliche negative Bind- und Compiler-Tests

Implementiere mindestens:

1. simuliert fehlendes `__COUNTER__` in einer betroffenen Datei führt zu der vorgesehenen verständlichen Fehlermeldung;
2. simuliert fehlendes `__COUNTER__` beeinträchtigt eine ausschließlich normale Bind-Datei nicht;
3. zusätzlicher fremder Counterverbrauch innerhalb einer Rebase-Region führt sicher zu einem Compile-Time-Fehler;
4. zu wenig oder zu viele beobachtete Expansionen führen zu einem Compile-Time-Fehler;
5. Null-Platzhalter in einer counter-pflichtigen Region werden mit präziser Bind-Diagnose abgelehnt;
6. verschachtelte beziehungsweise rekursive Wrapper werden abgelehnt;
7. Token-Pasting, Stringification zur Formatstringerzeugung und dynamische Formatstrings werden abgelehnt;
8. syntaktisch ungeeignete Sourcekontexte werden abgelehnt, ohne teilweise Source- oder Listenänderungen zu hinterlassen;
9. fehlende oder widersprüchliche Aufrufstellendeskriptoren fallen nicht still auf eine andere ID zurück.

### Compiler- und Optimierungstests

Soweit lokal vorhanden, teste mindestens:

- C mit GCC und Clang;
- C++ mit G++ und Clang++;
- `-O0` und `-O2`;
- die im Repository tatsächlich unterstützte minimale C-/C++-Baseline;
- Warnungen als Fehler gemäß vorhandenen Projektkonventionen.

Prüfe zusätzlich, dass die Rebase-Implementierung:

- keinen veränderlichen RAM-Zustand einführt;
- keine dynamische Allokation benötigt;
- bei Optimierung keine unnötige Laufzeit-ID-Tabelle oder zusätzlichen dynamischen Lookup hinterlässt;
- dieselben Trice-IDs im erzeugten Datenstrom liefert wie erwartet.

### Repositorytests

Führe während der Entwicklung regelmäßig gezielte Bind-Tests aus.

Verwende für den kurzen Integrationszyklus mindestens:

```sh
./scripts/_170_pc_target_tests_all_workflows.sh
```

beziehungsweise die im Repository tatsächlich vorhandene äquivalente Aufrufsyntax ohne den langen `full`-Modus.

Vor Abschluss mindestens:

```sh
./scripts/testAll.sh quick
```

`./scripts/testAll.sh full` ist für diese Implementierungsrunde kein notwendiger innerer Entwicklungszyklus. Führe es nur aus, wenn Laufzeit und Umgebung dies sinnvoll erlauben; andernfalls dokumentiere ausdrücklich, dass es nicht ausgeführt wurde.

## Dokumentation

Erweitere:

```text
docs/TriceBind/Trice_bind_90_MVP_User_Manual.md
```

Dokumentiere dort alle für Anwender relevanten Nuancen, ohne das Strategiepapier vollständig zu duplizieren.

Mindestens zu dokumentieren sind:

- der unveränderte Userworkflow `trice bind` plus normaler Build;
- welche Konstruktionen automatisch den lokalen Counter-Rebase benötigen;
- dass normale Dateien und normale Header keinerlei Counter-Abhängigkeit erhalten;
- die datei- und bereichslokale Wirkung;
- Headerdefinition versus Aufrufstelle;
- die Wirkung eines counter-pflichtigen Headers auf alle Translation Units, die ihn einbinden;
- stabile ID und Location an der inneren Makrodefinitionsstelle;
- dass der User keine Counterwerte sieht oder pflegt;
- die generierten Begin-/End- beziehungsweise Rebase-Artefakte und dass sie nicht manuell verschoben werden dürfen;
- die zielgerichtete Fehlermeldung bei fehlendem `__COUNTER__`;
- die empfohlenen Auswege `trice insert/clean`, Aufteilung auf einzelne Zeilen oder `static inline`;
- die unterstützten Wrappergrenzen;
- die weiterhin nicht unterstützten Konstruktionen;
- dass die Erweiterung keine Änderung an `til.json`, `li.json` oder dem Drahtformat verursacht;
- dass nach relevanten Sourceänderungen erneut `trice bind` ausgeführt werden muss.

Verweise für technische Hintergründe auf:

```text
docs/TriceBind/Trice_bind_60_MVP2_Implementation_Strategies.md
experiments/TriceBind/60_MVP2_Local_Counter_Rebase/README.md
```

## Nicht Gegenstand dieser Aufgabe

Implementiere in dieser Runde nicht:

- einen allgemeinen externen Target-Präprozessorlauf;
- eine Pflichtintegration mit `compile_commands.json`;
- Shadow-Sources;
- ELF-/Link-Map-Inventur;
- vorbereitete `.a`-Library-Bindung;
- allgemeine Unterstützung beliebiger Präprozessormakros;
- automatische Compilerfamilienerkennung;
- neue öffentliche Counter- oder Ordinaloptionen;
- eine Änderung des JSON-Schemas oder Drahtformats;
- einen stillen partiellen `trice insert`-Fallback.

## Randbedingungen und Qualitätsanforderungen

- Keine beabsichtigten Breaking Changes.
- Bestehendes Verhalten von `trice insert`, `trice clean`, normalem `trice bind`, Decoder, `til.json`, `li.json`, Drahtformat und vorhandenen Projekten erhalten.
- Keine zweite unabhängige Trice-Parser- oder ID-Logik.
- Gemeinsame Generator- und Targetlogik statt Codeduplizierung.
- Deterministische Ergebnisse trotz paralleler Verarbeitung.
- Transaktionales Schreiben bei fatalen Fehlern.
- Unveränderte Dateien nicht unnötig neu schreiben.
- Alle erkennbaren Diagnosen sammeln und geordnet ausgeben.
- Keine unnötigen Refactorings außerhalb des für diese Ausbaustufe notwendigen Umfangs.
- Keine allgemeine neue Compilerkonfiguration für normale Bind-Projekte.
- Fehlende Targetfähigkeit muss sichtbar scheitern, niemals still falsch binden.

Wenn eine Anforderung mit der bestehenden Architektur nachweislich nicht kompatibel oder nicht eindeutig sicher umsetzbar ist:

1. dokumentiere Datei, Symbol, Sourcekonstruktion und Minimalbeispiel;
2. nimm keine eigenmächtige API-, Format- oder Semantikänderung vor;
3. implementiere die übrigen unabhängigen Teile weiter;
4. beschreibe eine möglichst kleine kompatible Lösung im Abschlussbericht.

## Vorgehen

1. Analysiere Repository, aktuelle Bind-Implementierung, Parser, Target-Makros, Sidecarerzeugung, Re-Migration und Tests.
2. Führe Baseline-Tests einschließlich des vorhandenen Rebase-PoCs aus.
3. Erstelle vor der ersten größeren Änderung einen kompakten Implementierungsplan mit:
   - betroffenen Dateien,
   - Datenmodell für Definitions- und Aufrufstellen,
   - Klassifikation normal versus counter-pflichtig,
   - Generatorartefakten,
   - Target-Dispatch,
   - Headerbehandlung,
   - Fehlerfällen,
   - Teststrategie,
   - Dokumentationsänderung.
4. Implementiere schrittweise und teste gezielt nach jedem Teil.
5. Erweitere das User Manual.
6. Führe die abschließenden Tests aus.
7. Liefere den Abschlussbericht.

## Abschlussbericht

Liefere nach der Umsetzung:

1. kurze Architekturzusammenfassung;
2. Name und Ausgangspunkt des angelegten Branches;
3. Liste der geänderten Dateien;
4. Beschreibung der Generatorerweiterung;
5. Beschreibung der Target-Makrointegration;
6. Erklärung der lokalen Counter-Abgrenzung für Source- und Headerdateien;
7. Beschreibung der Definitions-ID- und Aufrufstellensemantik;
8. neue positive und negative Bind-Tests;
9. ausgeführte Testkommandos mit Ergebnis;
10. nicht ausgeführte Tests mit genauer Begründung;
11. Änderungen am User Manual;
12. verbleibende Einschränkungen;
13. ausdrücklich nicht implementierte Themen;
14. Abweichungen vom ausgewählten Rebase-PoC oder vom Auftrag;
15. mögliche Breaking Changes oder sonstige Risiken.

## Genehmigungen / Generalvollmacht

Frage nicht um Erlaubnis:

- um in GitHub oder im lokalen Repository etwas zu lesen;
- um den neuen Branch aus `wip` anzulegen und auszuchecken;
- um Go-, C-, C++-, Shell-, Test- oder Markdown-Dateien zu verändern;
- um Dateien unter `internal`, `src`, `scripts`, `_test`, `examples`, `experiments` oder `docs/TriceBind` zu ändern;
- um mehr als fünf Dateien zu ändern;
- um Tools auszuführen;
- um Go-, C- oder C++-Code zu kompilieren;
- um Tests auszuführen;
- um temporäre Dateien oder Testarbeitsbäume anzulegen;
- um eng begrenzte Fehler in der bestehenden Bind-Implementierung zu beheben, wenn sie die hier verlangte Ausbaustufe blockieren.

## Verbote

- Du darfst nicht selbstständig committen oder pushen.
- Du darfst keine uncommitteten Useränderungen verwerfen, überschreiben oder ohne Erlaubnis stashen.
- Du darfst die Generator-Spezifikation oder das Strategiepapier nicht fachlich ändern.
- Du darfst die neue Funktion nicht durch einen Shadow-Source-, externen Präprozessor- oder ELF-Ansatz ersetzen.
- Du darfst bestehende Tests nicht entfernen oder abschwächen, um neue Tests grün zu bekommen.
- Du darfst fehlende `__COUNTER__`-Unterstützung nicht durch stillen Fallback oder falsche ID-Zuordnung kaschieren.
- Du darfst keine projektweite Counter-Anforderung erzeugen, wenn nur einzelne Dateien oder Bereiche counter-pflichtig sind.

## Wichtig

Bearbeite diese Aufgabe durchgehend, ohne zwischendurch Freigaben anzufordern. Wenn du für die Durchführung noch eine unverzichtbare Erlaubnis benötigst, frage jetzt danach.

## Startkommando

Beginne jetzt mit Git-Status, Branchanlage aus `wip`, vollständiger Repositoryanalyse, Baseline-Tests und Implementierungsplan. Fahre danach selbstständig mit der Umsetzung, den Bind-spezifischen Tests und der Aktualisierung des User Manuals fort.
