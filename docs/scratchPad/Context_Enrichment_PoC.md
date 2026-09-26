# Context Enrichment – Machbarkeitsnachweis A9

Stand: 26. September 2026. Der isolierte Nachweis für direkte Bind-Logstellen ist bestanden. Er liegt in [context_enrichment_poc_test.go](../../internal/id/context_enrichment_poc_test.go). Die produktive Option `trice bind -ce` bleibt Aufgabe A10.

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

Der Nachweis erfüllt die Mindestfälle aus dem [CE-Entwurf](Kontextanreicherung_DE.md#71-verbindlicher-machbarkeitsnachweis-vor-m20). Er prüft direkte skalare 32-Bit-Logstellen mit einer Logstelle pro physischer Zeile und dem `iD`-Stempeltyp. Die PoC-Regeln sind feste Testdaten; ein CLI-Parser, vollständige Selektor-/Alias-Policy und produktive Fehlervalidierung sind noch nicht vorhanden.

A10 muss die Transformation vor der produktiven Schema-/ID-Vergabe einbinden und die Sidecar-Erweiterung dauerhaft erzeugen. Weitere Bind-Pfade wie Wrappermakros und Counter-Rebase, andere Bitbreiten/Stempeltypen, `TRICE_OFF`, Regelkonflikte und Konfigurationswechsel benötigen ihre eigenen Tests. Auch Verbraucher, die Source und TIL abgleichen, etwa `generate -logC`, müssen mit CE-Metadaten konsistent umgehen. Aus dem erfolgreichen direkten PoC folgt keine bereits bestandene Abnahme dieser weiteren Pfade.

Für den geprüften Mechanismus besteht kein technischer Blocker. A10 bleibt ein eigener Implementierungsauftrag.
