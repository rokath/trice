# Context Enrichment – Machbarkeitsnachweise

Stand: 27. September 2026. Der isolierte A9-Nachweis für direkte Bind-Logstellen ist bestanden. Er liegt in [context_enrichment_poc_test.go](../../internal/id/context_enrichment_poc_test.go). Die darauf aufbauende produktive Option `trice bind -ce` ist inzwischen mit A10 implementiert; ihre Bedienung und Abnahme stehen im [User Manual](../TriceUserManual.md#trice-context-enrichment). Dieser Bericht enthält außerdem die ursprüngliche Rebase-Gegenprobe und den neuen [PoC für Wrappermakros und Counter-Rebase](#erweiterter-poc-für-wrappermakros-und-counter-rebase). Letzterer ist eine Entscheidungsgrundlage und aktiviert keine zusätzliche produktive CE-Unterstützung.

## Geprüfter Mechanismus

Der bestehende Bind-Deskriptor enthält neben der ID ein anzuwendendes Makro. Der PoC verwendet an ausgewählten Logstellen ein generiertes Adaptermakro, das die ursprünglichen Argumente übernimmt und die Context-Ausdrücke anhängt. Diese Ausdrücke werden erst bei der Expansion des ursprünglichen Trice-Aufrufs ausgewertet und haben dort Zugriff auf lokale Variablen.

Für eine ursprünglich argumentlose Logstelle entspricht der Adapter beispielsweise:

```c
#define TRICE_CE_POC_SITE(ignoredImplementation, tid, format) \
    TRICE_INSERT_trice(tid, format, (x))
```

Bei vorhandenen Argumenten erhält das Adaptermakro passende zusätzliche Parameter. Jeder wird genau einmal in den endgültigen Aufruf übernommen. Generische Trice-Makros bestimmen ihre Arity aus der erweiterten Argumentliste. Bei festen Arity-Makros wählt der Adapter die passende Implementierung, beispielsweise `TRICE_INSERT_trice_1` für eine erweiterte `trice_0`-Logstelle.

Der Test erzeugt den erweiterten Template-String und die zusätzlichen Argumente zunächst ausschließlich in einer privaten In-Memory-Sourceansicht. Auf dieser Ansicht läuft der vorhandene `SubCmdIdBind` mit dem normalen Structured-Logging-Parser und der normalen ID-Vergabe. Damit wird die ID aus dem endgültigen Schema bestimmt. Der Compiler sieht dagegen weiterhin den unveränderten User-Source und das erzeugte Sidecar mit den Adaptermakros. Das ist ein Testadapter für den Architekturbeweis, noch keine produktive CE-Integration.

Die Fixture enthält bereits das reguläre Bind-Sidecar-Include und einen festen File-Key. Der Test prüft deshalb die durch CE geforderte Source-Unveränderlichkeit unabhängig von der erstmaligen Einrichtung eines Bind-Projekts.

## Nachgewiesenes Verhalten

| Fall | Erwartetes Ergebnis | Nachweis |
| --- | --- | --- |
| Argumentloses `trice` | Lokales `x` wird als zusätzlicher Wert übertragen | Binärrecord enthält `7` |
| Bereits parametrisiertes `trice` | Originalwert steht vor dem CE-Wert | Binärrecord enthält `1, 1` |
| Einfacher Ausdruck | `x + 1` wird am Aufrufort ausgewertet | Binärrecord enthält `8` |
| Feste Arity | `trice_0` und `trice_1` erhalten die passende endgültige Arity | Binärrecords enthalten `7` bzw. `22, 7` |
| Innerer Block-Scope | Nur dort sichtbares `blockValue` wird verwendet | Binärrecord enthält `11` |
| Unselektierte Logstelle | Keine zusätzlichen Werte | Binärrecord bleibt argumentlos |
| Seiteneffekte | Originalausdruck und CE-Ausdruck werden jeweils genau einmal ausgewertet | Zwei getrennte Laufzeitzähler stehen auf `1` |
| Nicht ausgeführter Aufruf | Kein Record und kein CE-Seiteneffekt | Sieben Records trotz acht instrumentierter Logstellen; CE-Zähler bleibt auf `1` |
| TIL-Konsistenz | Finale Templates, Arity, IDs und Nutzdaten passen zusammen | Explizite Template-Erwartungen und echter Trice-Record-Parser/Resolver |
| Wiederholung | Identische IDs und generierte Inhalte | Bytevergleich von Source, Konfiguration, TIL, LI, Sidecar und Feldregister nach zwei PoC-Bind-Läufen |
| Ungültiger Context | Ein nicht sichtbarer Bezeichner wird diagnostiziert | Compiler und `clangd` weisen `ceMissingLocal` ab |

Der Laufzeittest verwendet die tatsächlichen Target-Makros und die Trice-Bibliothek. Der Auxiliary-Ausgang liefert die erzeugten Binärrecords an `TriceParseRecord`; `TriceResolveLog` prüft sie gegen eine aus der endgültigen TIL erzeugte C-Metadatentabelle. Damit wird auch eine falsche Payload-Länge oder Parameterzahl erkannt. Die C-Tabelle wird im PoC direkt aus der TIL erzeugt; der öffentliche `generate -logC`-Workflow ist dabei nicht geprüft.

## Compiler und Editor-Diagnosen

Der Test kompiliert und startet dieselbe Fixture als C11 und C++17 mit `-Wall -Wextra -Werror`. Die Bibliotheksquellen werden als C übersetzt. Für beide Sprachmodi wird eine `compile_commands.json` mit den tatsächlichen Compilerargumenten erzeugt. `clangd --check` muss diese Datenbank laden und ohne Fehler abschließen. Der Negativtest zeigt zusätzlich, dass fehlende Context-Bezeichner weiterhin sichtbar diagnostiziert werden.

Geprüfte Umgebung: macOS auf ARM64, Apple Clang/Clang++ 21.0.0 und Apple clangd 21.0.0. Beide Sprachmodi und die negativen Diagnoseprüfungen bestanden. Dies belegt den Language-Server-Pfad für clangd-basierte Editoren mit dem generierten Include-Verzeichnis und der realen Compile-Konfiguration. Andere Language-Server, IDE-eigene Parser, GCC und MSVC wurden in diesem A9-Lauf nicht geprüft.

## Reproduzieren

Im Repository-Root ausführen:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id -run '^TestContextEnrichmentPoC$' -count=1 -v
```

Erforderlich sind ein GCC-/Clang-kompatibler C- und C++-Compiler sowie `clangd` im `PATH`. Der gezielte Test verlangt diese Werkzeuge ausdrücklich. Ohne `TRICE_BIND_INTEGRATION=1` wird er übersprungen. Alle Fixtures und Build-Artefakte entstehen in einem temporären Testverzeichnis und werden anschließend entfernt. Die bestehenden Repository-Workflows werden nicht verändert.

## Abgrenzung zu A10

Der Nachweis erfüllt die Mindestfälle aus dem [CE-Vertrag](Kontextanreicherung_DE.md#71-verbindlicher-machbarkeitsnachweis-vor-m20). Er prüft direkte skalare 32-Bit-Logstellen mit einer Logstelle pro physischer Zeile und dem `iD`-Stempeltyp. Die PoC-Regeln sind feste Testdaten; der A9-Test enthält keinen CLI-Parser, keine vollständige Selektor-/Alias-Policy und keine produktive Fehlervalidierung.

A10 bindet die Transformation vor der produktiven Schema-/ID-Vergabe ein und erzeugt die Sidecar-Erweiterung dauerhaft. Die erste Ausbaustufe bleibt auf direkte, eindeutig über ihre Quellzeile adressierbare Logstellen begrenzt. Die zusätzlichen [Bind-Verhaltenstests](../../internal/id/contextEnrichment_test.go) und [CLI-/Target-Tests](../../internal/args/context_enrichment_test.go) prüfen die breitere Abnahme getrennt vom A9-PoC: 8/16/32/64 Bit, verschiedene Stempeltypen, `TRICE_OFF`/`TRICE_CLEAN`, Regelkonflikte, Konfigurationswechsel, Mehrzeiler, Inline-Funktionen und echte Compiler-/Editorläufe ohne `__COUNTER__`. Vier Beispiele werden aus `triceCheck.c` übernommen; insgesamt vierzehn Records durchlaufen den öffentlichen `generate -logC`-Resolver und den Go-Decoder für Text, JSON und KV. CE für Wrappermakros und Counter-Rebase bleibt eine eigene Folgeaufgabe.

Damit ist der direkte Mechanismus nicht mehr nur ein PoC. Die weiterhin offenen Varianten bleiben im [Arbeitsplan](Implementierungsplan.md) abgegrenzt.

## Ergänzende Rebase-Gegenprobe vor A10

Am 27. September 2026 wurde die direkte Übertragung des Adapteransatzes auf Counter-Rebase geprüft. Der zusätzliche Test `TestContextEnrichmentPoCRebaseScopeBoundary` zeigt eine Grenze: Zwei von Bind unterstützte Logstellen auf derselben Sourcezeile liegen in getrennten Blöcken und verwenden jeweils eine nur dort sichtbare Variable. Der normale Bind-Build besteht. Werden die beiden CE-Ausdrücke in die jeweiligen Zweige des generierten Rebase-Dispatchers eingefügt, scheitert die Übersetzung an den Variablennamen des jeweils anderen Scopes.

Der Grund ist die C-seitige Ordinalauswahl: Auch ein zur Laufzeit nicht gewählter `if`-Zweig wird vom Compiler auf gültige Bezeichner geprüft. Die betroffenen Ausdrücke sind an ihrer vorgesehenen Logstelle gültig. Der Fehler wäre deshalb eine unzulässige zusätzliche Scope-Anforderung der Instrumentierung. Der Test erwartet und belegt genau diese fehlgeschlagene Erweiterung; er ist keine bestandene CE-Rebase-Abnahme.

Der direkte A9-Nachweis bleibt gültig. Das bloße Anhängen von CE-Argumenten an Rebase-Zweige genügt für eine allgemeine CE-Unterstützung jedoch nicht. Am 27. September wurde deshalb die erste Ausbaustufe auf direkte, eindeutig über ihre Quellzeile adressierbare Bind-Logstellen begrenzt. A10 weist ausgewählte Wrapper-/Rebase-Stellen vor Dateiänderungen ab und verweist mit `Search UM for "bind-limits".` auf die verständliche Erklärung im User Manual. Ohne passende CE-Regel bleiben die bisherigen Bind-Fähigkeiten erhalten. Der zusätzliche Architektur-Nachweis für komplexe CE-Stellen ist eine zurückgestellte Folgeaufgabe; diese Gegenprobe allein belegt keine grundsätzliche Unmöglichkeit einer späteren Lösung.

Die Gegenprobe ist separat reproduzierbar:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id -run '^TestContextEnrichmentPoCRebaseScopeBoundary$' -count=1 -v
```

## Erweiterter PoC für Wrappermakros und Counter-Rebase

**Ergebnis:** Eine Auswahl des CE-Adapters bereits im Präprozessor beseitigt das nachgewiesene Problem fremder lokaler Variablen. Der neue Test [context_enrichment_rebase_poc_test.go](../../internal/id/context_enrichment_rebase_poc_test.go) weist einen funktionierenden Ansatz mit einem zusätzlichen Compiler-Vorlauf nach. Er enthält außerdem einen einfacheren Sonderfall: Ein Wrapper mit genau einer Logstelle kann bei eindeutiger Zeilenzuordnung ohne diesen Vorlauf und ohne `__COUNTER__` angereichert werden. Beides bleibt Testcode; `bind -ce` weist die bisher ausgeschlossenen Konstrukte weiterhin ab.

### Wie der untersuchte Ansatz arbeitet

Bei der bisherigen C-Verzweigung gelangen die Ausdrücke aller möglichen Logstellen zum Compiler. Im neuen PoC wählt dagegen die Makroexpansion genau einen Adapter aus. Nur dessen Ausdrücke erscheinen im endgültigen C-/C++-Code. Dadurch kann etwa ein Wrapper in seinem linken Zweig eine Variable `branchLeft` und in seinem rechten Zweig eine andere Variable `branchRight` verwenden, ohne dass einer dieser Namen im jeweils anderen Block existieren muss.

Die Zuordnung benötigt einen Wert, den der Präprozessor direkt als Teil eines Makronamens verwenden kann. Die bestehende relative Rechnung aus `__COUNTER__` und einer C-Enum-Konstante eignet sich dafür nicht. Der PoC ermittelt deshalb die tatsächlichen absoluten Counter-Werte mit dem jeweils verwendeten Compiler:

1. Der bestehende Bind-Mechanismus richtet die temporären Testquellen regulär ein. Eine private Sourceansicht erhält die CE-Erweiterungen und durchläuft die vorhandene Schema-/ID-Vergabe. Der tatsächlich kompilierte User-Source behält seine ursprünglichen Trice-Aufrufe und Wrapperdefinitionen.
2. Der Compiler verarbeitet diese Quellen mit den tatsächlichen Sprach-, Target- und Präprozessoroptionen vor. Eine nur für den Test eingebundene Datei lässt für jede Rebase-Expansion eine Markierung mit Region und Counter-Wert erscheinen.
3. Der Test ordnet diese Markierungen den numerischen Definition-/Location-Deskriptoren und der Expansionsreihenfolge aus dem erzeugten Bind-Sidecar zu. Er errät keine IDs aus Formatstrings. Daraus entsteht ein Header mit genau einem Makro pro tatsächlich beobachteter Expansion.
4. Beim normalen Übersetzen wählt `__COUNTER__` dieses Makro aus. Es übergibt die bestehenden Argumente und ergänzt nur die zugehörigen CE-Ausdrücke. Die bestehenden Rebase-Endprüfungen bleiben aktiv. Eine zusätzliche Prüfung des Basiswertes weist eine verschobene Zuordnung zurück, auch wenn zufällig noch ein anderer gültiger Eintrag getroffen würde.

Dieser Vorlauf ist pro Übersetzungseinheit und konkreter Build-Konfiguration erforderlich. Eine Übersetzungseinheit ist hier beispielsweise eine `.c`- oder `.cpp`-Datei einschließlich ihrer eingebundenen Header. Derselbe gemeinsam verwendete Wrapper kann deshalb für verschiedene Übersetzungseinheiten unterschiedliche Counter-Zuordnungen benötigen, während seine logischen IDs gleich bleiben.

### Nachgewiesene Fälle

| Fall | Geprüftes Ergebnis |
| --- | --- |
| Einfacher Wrapper mit einer Logstelle | CE am Aufrufort funktioniert auch mit entferntem `__COUNTER__`; ein normaler Zeilendeskriptor genügt. |
| Wrapper mit zwei Logstellen | Ursprüngliche Werte stehen vor den jeweiligen CE-Werten; generische und feste Arity funktionieren. |
| Wiederholte Wrapper-Aufrufe | Dieselben logischen IDs übertragen unterschiedliche lokale Context-Werte ihrer Aufrufer. |
| Wrapper mit getrennten Zweigen | `branchLeft` und `branchRight` bleiben jeweils auf ihren eigenen Block beschränkt. |
| Zwei direkte Aufrufe auf derselben Zeile | Getrennte Blöcke mit `onlyLeft` und `onlyRight` funktionieren ohne fremde Scope-Anforderungen. |
| Nicht ausgewählte Rebase-Stellen | Die bestehenden Records bleiben unverändert und ohne zusätzliche Werte. |
| Seiteneffekte | Ursprünglicher Ausdruck und CE-Ausdruck werden je ausgeführtem Aufruf genau einmal ausgewertet. Ein nicht ausgeführter Wrapper-Aufruf bewirkt nichts. |
| Tatsächliche Records | Elf ausgegebene Records werden durch die echte Target-Bibliothek erzeugt, gegen die finale C-TIL aufgelöst und auf IDs, Parameterzahl und sämtliche geordneten Werte geprüft. |
| Wiederholung | Erneute private Bind-Generierung erhält Schema, IDs, Regionenzuordnung und Source. Derselbe Compiler-Vorlauf reproduziert denselben Zuordnungsheader. |
| Fremde Counter-Verwendungen vor den Logstellen | Zusätzliche Verwendungen mit den Abständen 0, 1 und 7 ändern die Zuordnung, während IDs und erwartete Records gleich bleiben. |
| Veralteter Zuordnungsheader | Ein normaler Build scheitert. Insbesondere wird auch eine Verschiebung um eins erkannt, die sonst auf einen benachbarten gültigen Adapter treffen könnte. |
| Fehlender Context-Bezeichner | Compiler und die geprüften `clangd`-Varianten melden `cePocMissingLocal` als echten Fehler. |
| Zusätzlicher Counter-Verbrauch im CE-Ausdruck | Wird abgewiesen; der untersuchte Vorlauf setzt einen Counter pro Rebase-Expansion voraus. |
| `TRICE_OFF` und `TRICE_CLEAN` | Keine Records und keine Argument-/CE-Auswertung, auch ohne verfügbares `__COUNTER__`. |
| Aktives Rebase ohne `__COUNTER__` | Klarer Buildfehler einschließlich `Search UM for "bind-limits".`. Der PoC behauptet für diesen Fall keine Lösung. |

Der Laufzeitnachweis verwendet skalare 32-Bit-Records mit `iD`. Er ersetzt nicht die breitere Bitbreiten-/Stempel-Abnahme von A10 und ist noch keine vollständige Abnahme sämtlicher denkbaren Wrapper.

### Compiler-Matrix und Aussagegrenzen

Am 27. September 2026 wurden folgende installierte Werkzeugvarianten geprüft:

| Werkzeug und Ziel | Sprachmodi | Nachweis |
| --- | --- | --- |
| Apple Clang/Clang++ 21.0.0, macOS ARM64 | C99, C11, C17, C++11, C++17 | Vorverarbeitung, Kompilierung mit `-Wall -Wextra -Werror`, Linken und tatsächliche Programmausführung bestanden. |
| ARM GNU Toolchain 13.3.Rel1, GCC/G++ 13.3.1, Cortex-M0/Thumb | C99, C11, C17, C++11, C++17 | Vorverarbeitung und Erzeugung echter ARM-Objektdateien mit `-Wall -Wextra -Werror` bestanden; keine Ausführung auf MCU oder Emulator. |
| Dieselbe ARM-GCC-Version, Cortex-M4/Thumb | C99, C11, C17, C++11, C++17 | Vorverarbeitung und Erzeugung echter ARM-Objektdateien bestanden; keine Target-Laufzeitaussage. |
| Alle drei Konfigurationen | C++20 | Der bestehende Bind-Code scheitert bereits ohne experimentelles CE an einer mit `-Werror` eskalierten Enum-Warnung. Mit ausschließlich dieser Warnung auf Warnungsstatus zurückgesetzt besteht der CE-PoC; Clang einschließlich Laufzeit, ARM-GCC als Objekt-Build. |
| Apple clangd 21.0.0 | Host-C11 und Host-C++17 | Reale `compile_commands.json` einschließlich des experimentellen Headers wird geladen. Gültige Quellen sind fehlerfrei; der absichtlich fehlende Bezeichner wird diagnostiziert. |

Damit wurden 18 Kombinationen aus Toolchain/Target und Sprachmodus untersucht, jeweils mit drei Counter-Ausgangslagen sowie zusätzlichen Negativ- und Abschaltfällen. Die macOS-Kommandos `gcc`/`g++` sind in dieser Umgebung Clang-Aliase und werden ausdrücklich nicht als GCC-Nachweis gezählt. Der eigenständige GCC-Nachweis stammt vom ARM-Crosscompiler. Native GCC-Varianten werden bei Verfügbarkeit ebenfalls in die Testmatrix aufgenommen. MSVC, IAR, Arm Compiler/armclang und andere Language-Server wurden nicht geprüft; ihre Unterstützung ist daraus nicht ableitbar.

Die C++20-Grenze stammt aus der bestehenden Rebase-Prüfung: Sie subtrahiert Werte verschiedener anonymer Enum-Typen. Der Test weist das zuerst mit gewöhnlichem Bind nach und verwendet danach nur `-Wno-error=deprecated-anon-enum-enum-conversion` bei Clang beziehungsweise `-Wno-error=deprecated-enum-enum-conversion` bei GCC. Das ist ausdrücklich kein erfolgreicher strenger C++20-Build. Der Produktcode wurde für den PoC nicht geändert. Bei der Simulation eines fehlenden `__COUNTER__` wird die Warnung über das Entfernen eines eingebauten Makros ebenfalls nicht als Fehler behandelt.

### Konsequenzen für eine mögliche Umsetzung

**Die technische Scope-Hürde ist für die geprüften Fälle gelöst; der Preis dieses Ansatzes ist ein zusätzlicher compilerabhängiger Build-Schritt.** Eine produktive Entscheidung muss diesen Aufwand bewusst einschließen. Der PoC liefert noch keinen solchen Workflow und keine neue CLI-Option.

Vor einer Umsetzung wären insbesondere folgende Punkte festzulegen oder nachzuweisen:

- Einbindung des Vorlaufs in die unterstützten Build-Systeme, einschließlich derselben Defines, Include-Pfade, Sprachmodi und Target-Optionen wie beim eigentlichen Übersetzen.
- Getrennte Zuordnungsartefakte pro Übersetzungseinheit und Konfiguration sowie verlässliche Neuerzeugung nach relevanten Änderungen. Die Counter-Prüfungen erkennen Verschiebungen, ersetzen aber keine vollständige Build-Abhängigkeitsprüfung oder einen Schutz gegen beliebig beschädigte Artefakte.
- Verhalten bei Precompiled Headers, Modulen, zusätzlichen Counter-Verwendungen in Argumentmakros und weiteren Compilerfamilien. Der PoC macht dafür keine Zusage.
- Falls strenge C++20-Builds zum Ziel gehören, eine gesonderte Korrektur und Abnahme der bestehenden Enum-Rebase-Prüfung.
- Entscheidung, ob die einfachere Erweiterung für eindeutig zuordenbare Wrapper mit einer Logstelle zunächst unabhängig von allgemeinem CE-Rebase umgesetzt werden soll. Der PoC belegt diesen Fall ohne Counter-Vorlauf; die produktive Freischaltung bleibt ein eigener Auftrag.

Ein zweiter Compilerlauf ist damit eine nachgewiesene Möglichkeit, keine Behauptung, dass es keinen einfacheren Ansatz geben kann. Die erste direkte CE-Ausbaustufe bleibt unverändert. Automatisches CE für `insert/clean` wurde nicht untersucht und bleibt eine separate reversible Source-Transformation.

### Den erweiterten PoC wiederholen

Im Repository-Root ausführen:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id -run '^TestContextEnrichmentRebasePoC$' -count=1 -v
```

Der Test erkennt installierte Clang-/GCC-C/C++-Toolchains und ARM-GCC selbst, meldet fehlende Werkzeuge und Compiler-Aliase und installiert nichts. Mindestens eine passende C/C++-Toolchain ist erforderlich. Ohne `TRICE_BIND_INTEGRATION=1` wird der Compiler-Test übersprungen. `clangd` wird bei Verfügbarkeit geprüft; ein fehlendes Werkzeug wird ausdrücklich gemeldet. Alle Source-Kopien, experimentellen Header und Build-Artefakte entstehen in temporären Testverzeichnissen. Produktive CLI, Target-Header und Build-Skripte bleiben unverändert.
