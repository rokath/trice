# Entscheidungsgrundlage: Implementierungsstrategien für zentrale `trice bind`-MVP2-Fähigkeiten

**Status:** Technisches Entscheidungspapier, nicht normativ  
**Stand:** 8. August 2026  
**Gegenstand:** Mehrere Trice-Stellen pro physischer Sourcezeile und Trice-Stellen in Präprozessormakros  
**Nicht behandelt:** `trice bind -check`, Post-Link-Inventur und allgemeine Analyse-/Berichtsoptionen

## 1. Kurzantwort

Ja. Die beiden zentralen sourcebezogenen MVP2-Fähigkeiten erscheinen unter
klar begrenzten Regeln implementierbar:

1. mehrere direkte Trice-Aufrufe auf derselben physischen Sourcezeile;
2. Trice-Aufrufe in gewöhnlichen Makrodefinitionen, einschließlich eines
   Makros mit mehreren Trice-Stellen wie einem `switch`-Logger.

Das vorhandene PoC unter
[`../../examples/PoC_bind_mvp2`](../../examples/PoC_bind_mvp2/README.md)
belegt die grundsätzliche Target-Machbarkeit. Es zeigt außerdem zwei wichtige
Grenzen:

- Eine reine Zuordnung über Datei und Zeilennummer kann mehrere Stellen auf
  derselben Zeile nicht unterscheiden.
- `__COUNTER__` kann die Stellen unterscheiden, aber seine Zahlenfolge hängt
  vom vollständigen Präprozessorlauf ab. Eine fremde `__COUNTER__`-Expansion
  verschiebt alle nachfolgenden Werte.

Die Entscheidung lautet deshalb nicht nur »`__COUNTER__` verwenden oder
nicht«, sondern vor allem:

> Wie wird eine lokale Trice-Stelle unabhängig von irrelevanten
> Präprozessoraktivitäten reproduzierbar derselben stabilen ID zugeordnet?

Dieses Papier beschreibt drei Varianten:

1. **Zeilenlokaler Counter-Rebase:** Der Counter wird unmittelbar vor einer
   erweiterten Stelle lokal neu referenziert.
2. **Exakter Target-Präprozessorlauf:** `trice bind` beobachtet die Werte mit
   derselben Toolchain und Konfiguration wie der spätere Build.
3. **Generierter Compiler-Input mit expliziten Ordinalen:** Eine erzeugte
   Shadow-Source enthält eindeutige Stellennummern und benötigt keinen Counter.

Für das gewünschte einfache Bedienmodell ist Variante 1 die interessanteste
Default-Kandidatin. Sie benötigt jedoch einen weiteren produktionsnahen PoC.
Variante 2 eignet sich als optionale Absicherung in vollständig beschriebenen
Buildumgebungen. Variante 3 ist die deterministischste Counter-unabhängige
Fallback-Technik, verlangt aber eine Integration in das Buildsystem.

## 2. Geltungsbereich und Abgrenzung

Die bestehende
[`Trice_bind_Generator_Spezifikation_MVP.md`](Trice_bind_Generator_Spezifikation_MVP.md)
führt die folgenden Punkte getrennt als MVP2-Ideen:

- begrenzte Wrappermakros;
- mehrere Trice-Stellen auf einer Zeile;
- aktive Präprozessorkonfiguration;
- Post-Link-Inventur;
- vorbereitete statische Libraries;
- zusätzliche Hygiene- und Konsistenzprüfungen.

Die drei hier untersuchten Varianten lösen primär die ersten beiden Punkte.
Sie lösen nicht automatisch alle übrigen MVP2-Themen. Insbesondere sind
vorbereitete statische Libraries und Post-Link-Inventuren eigenständige
Architekturaufgaben.

Mit »Makrodefinition« ist in diesem Papier beispielsweise gemeint:

```c
#define LOG_ERROR(value)           \
    do {                           \
        switch (value) {           \
        case 7:                    \
            trice("error=%d", 7); \
            break;                 \
        case 8:                    \
            trice("error=%d", 8); \
            break;                 \
        default:                   \
            break;                 \
        }                          \
    } while (0)
```

Nicht gemeint ist zunächst die uneingeschränkte Unterstützung beliebiger
Präprozessorkonstruktionen. Token-Pasting, Stringification, dynamisch erzeugte
Formatstrings, rekursive oder verschachtelte Wrapper und indirekt umdefinierte
Loggingmakros bleiben gesondert zu bewerten.

## 3. Begriffe ohne Compiler-Vorkenntnisse

### 3.1 Präprozessor

Vor der eigentlichen C- oder C++-Compilation läuft der Präprozessor. Er führt
unter anderem `#include`, `#define`, `#if` und Makroexpansionen aus. Der Compiler
sieht anschließend nicht mehr exakt den geschriebenen Usercode, sondern das
Ergebnis dieser Vorverarbeitung.

### 3.2 Physische Zeile und `__LINE__`

`__LINE__` wird vom Präprozessor durch die aktuelle Zeilennummer ersetzt. Zwei
Trice-Aufrufe auf derselben Zeile erhalten dieselbe Nummer:

```c
trice("first"); trice("second");
```

Eine Sidecar-Zuordnung nur über Datei und Zeile sieht daher zweimal denselben
Schlüssel. Sie kann nicht entscheiden, welche ID zum ersten und welche zum
zweiten Aufruf gehört.

### 3.3 `__COUNTER__`

`__COUNTER__` ist ein vom Präprozessor bereitgestellter Zähler. Bei jeder
Expansion liefert er die nächste Ganzzahl:

```c
enum { a = __COUNTER__ }; // commonly 0
enum { b = __COUNTER__ }; // commonly 1
```

Damit können zwei Makroexpansionen auf derselben Zeile unterschieden werden.
Der Zähler gehört jedoch der gesamten Translation Unit. Dazu zählen die
Hauptdatei und alle eingebundenen Header. Auch fremde Makros können ihn
verbrauchen.

### 3.4 Translation Unit

Eine Translation Unit ist vereinfacht die C- oder C++-Datei einschließlich
aller durch `#include` eingefügten Header nach der Präprozessorverarbeitung.
Jeder Compile-Aufruf verarbeitet eine eigene Translation Unit und besitzt
seinen eigenen Counterverlauf.

### 3.5 Sidecar

Ein Bind-Sidecar ist eine von `trice bind` erzeugte Hilfsdatei. Sie ordnet einer
Sourceposition eine stabile Trice-ID zu. Der User schreibt weiterhin keine
numerische ID in den Trice-Aufruf.

### 3.6 Stabile Definitions-ID und Aufrufstellendeskriptor

Bei einem Wrappermakro müssen zwei Identitäten unterschieden werden:

- Die **Definitionsstelle** ist die Trice-Stelle im `#define`. Sie besitzt die
  stabile fachliche ID und soll in `til.json` und `li.json` deterministisch
  repräsentiert werden.
- Die **Aufrufstelle** ist jede Expansion des Wrappermakros. Für sie benötigt
  der Target-Präprozessor einen Deskriptor, damit er die stabile Definitions-ID
  in die konkrete Expansion einsetzen kann.

Ein Wrapper mit zwei inneren Trices besitzt zwei Definitions-IDs. Jeder
Wrapperaufruf erzeugt zwei Aufrufstellendeskriptoren, die diese beiden IDs
wiederverwenden.

## 4. Welche Konstruktionen benötigen tatsächlich einen Counter?

Nicht jede Trice-Stelle in einem Makro benötigt zwingend `__COUNTER__`.

| Sourcekonstruktion | Zusätzlicher Auftretensschlüssel nötig? |
|---|---:|
| Normale Trice-Stelle auf eigener Zeile | Nein |
| Mehrere direkte Trices auf einer Zeile | Ja |
| Wrapper mit genau einem Trice, maximal ein Wrapperaufruf pro Zeile | Nicht zwingend |
| Wrapper mit mehreren inneren Trices | Ja |
| Mehrere Wrapperaufrufe auf derselben Zeile | Ja |
| Verschachtelte Wrapper | Nicht Gegenstand der ersten Ausbaustufe |

Für den User sollte diese Unterscheidung unsichtbar bleiben. `trice bind`
klassifiziert die Stellen intern und verwendet nur dort einen erweiterten
Mechanismus, wo Datei und Zeile nicht eindeutig sind.

## 5. Verfügbarkeit von `__COUNTER__`

### 5.1 Aktueller Stand der großen Compilerfamilien

Die Annahme einer breiten Verfügbarkeit ist für die dominierenden
Compilerfamilien plausibel:

- Die [GCC-Präprozessordokumentation](https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html)
  dokumentiert `__COUNTER__` als GNU-Erweiterung mit fortlaufenden Werten ab
  null. Sie warnt ausdrücklich vor Wechselwirkungen mit vorkompilierten
  Headern.
- Die [Clang-Dokumentation](https://clang.llvm.org/docs/LanguageExtensions.html)
  dokumentiert ebenfalls den bei null beginnenden, bei jeder Expansion
  inkrementierten Zähler. Sie führt `__COUNTER__` inzwischen als C2y-Feature
  und in älteren C-Modi sowie C++ als Erweiterung.
- Die [MSVC-Dokumentation](https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170)
  bezeichnet `__COUNTER__` als immer definiert und beschreibt, dass auch
  Expansionen in eingebundenen Headern den Zustand verändern.
- Das WG14-Papier
  [N3457](https://open-std.org/jtc1/sc22/wg14/www/docs/n3457.htm)
  beschreibt die Standardisierung der bereits verbreiteten Erweiterung für
  eine kommende C-Sprachversion.

Viele Embedded-Toolchains basieren auf GCC oder Clang und dürften deren
Verhalten übernehmen. Das ist jedoch eine begründete Erwartung, keine Garantie
für jede konkrete Hersteller- und Versionskombination. Proprietäre Compiler,
alte Long-Term-Support-Versionen und besondere Strict-Mode-Optionen müssen mit
dem tatsächlich verwendeten Tool geprüft werden.

### 5.2 Warum keine feste Compilerliste verwendet werden sollte

Eine Liste wie »GCC ab Version X« ist weniger zuverlässig als ein direkter
Featuretest. Projekte verwenden Cross-Compiler, Hersteller-Forks und
unterschiedliche Sprachmodi. Außerdem kann ein Compiler eine Erweiterung zwar
kennen, sie unter den wirklichen Warnungsoptionen des Projekts aber ablehnen.

Die technische Prüfung ist klein:

```c
#if !defined(__COUNTER__)
#error "Target preprocessor does not provide __COUNTER__."
#endif

enum { triceCounterProbe0 = __COUNTER__ };
enum { triceCounterProbe1 = __COUNTER__ };
```

Ein vollständiger Probe-Compile sollte zusätzlich sicherstellen, dass der
zweite Wert genau um eins größer ist und die im Projekt üblichen
Warnings-as-Errors-Optionen akzeptiert werden.

### 5.3 Automatische Prüfung ohne zusätzliche Useroption

Das gewünschte Bedienmodell kann erhalten bleiben:

```sh
trice bind
Build
```

Der Ablauf sollte sein:

1. `trice bind` scannt zuerst die Quellen.
2. Gibt es keine Counter-pflichtige Stelle, erfolgt keinerlei Compilerprobe.
3. Ist die Target-Toolchain eindeutig aus einer vorhandenen Buildbeschreibung
   bekannt, kann `trice bind` den Featuretest sofort ausführen.
4. Ist die Target-Toolchain nicht zuverlässig bekannt, darf nicht ersatzweise
   irgendein Host-`cc` als Beweis verwendet werden.
5. Der erzeugte Sidecar enthält dann ausschließlich für betroffene Dateien
   eine Target-seitige Schutzprüfung.

Beispiel für einen Sidecar-Guard:

```c
#if !defined(__COUNTER__)
#error "trice bind: advanced source sites require __COUNTER__; use trice insert/clean or rewrite the source"
#endif
```

Damit wird eine nicht unterstützte Toolchain spätestens beim Target-Build klar
abgewiesen. Normale Bind-Projekte ohne die erweiterten Konstruktionen bleiben
vollständig unbeeinflusst.

### 5.4 Verfügbarkeit und Reproduzierbarkeit sind verschiedene Prüfungen

Die obige Abfrage beantwortet nur:

> Kennt der Präprozessor `__COUNTER__`?

Sie beantwortet nicht:

> Erzeugt er beim Generatorlauf und beim späteren Build dieselbe Zahlenfolge an
> denselben Trice-Stellen?

Eine produktive Implementierung muss beide Fragen lösen. Andernfalls kann ein
Build erfolgreich sein und trotzdem eine falsche ID verwenden. Das wäre
gefährlicher als ein sichtbarer Compilerfehler.

## 6. Gemeinsame fachliche Anforderungen an alle Varianten

Unabhängig von der gewählten Technik gelten folgende Anforderungen:

1. Die stabile ID wird weiterhin mit der bestehenden Insert-/Bind-ID-Logik
   vergeben. Der Counter selbst ist niemals die Trice-ID.
2. Mehrere Stellen derselben Zeile werden nach ihrer textuellen Reihenfolge
   unterschieden.
3. Bei Wrappern gehört die stabile ID zur Trice-Definitionsstelle, nicht zur
   zufälligen Counterzahl und nicht zu einer einzelnen Wrapperexpansion.
4. Alle Aufrufstellendeskriptoren eines Wrappers verweisen reproduzierbar auf
   dieselbe Definitions-ID.
5. `til.json`, `li.json` und das Drahtformat bleiben unverändert.
6. Ein fehlender oder widersprüchlicher Deskriptor muss einen Compiler- oder
   Bind-Fehler auslösen und darf nicht still auf eine andere ID fallen.
7. Bei einem fatalen Bind-Fehler werden weder teilweise Sidecars noch teilweise
   aktualisierte Listen hinterlassen.
8. `trice insert` und `trice clean` bleiben der universelle Fallback.
9. Der User muss keine Counterwerte sehen, pflegen oder versionieren.
10. Die erweiterten Regeln werden zunächst auf klar analysierbare Makros
    begrenzt.

## 7. Variante 1: Zeilenlokaler Counter-Rebase

### 7.1 Idee für Compiler-Laien

Der globale Counter ist mit einem Kilometerzähler vergleichbar. Sein absoluter
Stand ist schwer vorherzusagen, weil auch fremder Code Kilometer hinzufügen
kann. Unmittelbar vor der interessanten Sourcezeile wird deshalb ein lokaler
Startwert notiert. Die Trice-Stellen verwenden nur noch die Differenz zu diesem
Startwert.

Wenn der Startwert 87 ist, liefern die drei unmittelbar folgenden Trice-
Expansionen beispielsweise 88, 89 und 90. Lokal entsprechen sie damit den
Ordinalen 0, 1 und 2, unabhängig davon, wie der Counter vor dieser Stelle auf
87 gekommen ist.

### 7.2 Möglicher Source- und Helperablauf

`trice bind` könnte um eine erweiterte Zeile zwei generierte
Reaktivierungs-Includes platzieren:

```c
#include "trice_counter_begin_KABC_L100.h"
trice("first"); trice("second"); trice("third");
#include "trice_counter_end_KABC_L100.h"
```

Der Begin-Helper könnte konzeptionell enthalten:

```c
#if !defined(__COUNTER__)
#error "trice bind: this source line requires __COUNTER__"
#endif

enum { TRICE_BIND_COUNTER_BASE_KABC_L100 = __COUNTER__ };
static const unsigned triceBindIDsKABCL100[] = { 4101u, 4102u, 4103u };

#define TRICE_BIND_ADVANCED_ID_KABC_L100() \
    triceBindIDsKABCL100[__COUNTER__ - TRICE_BIND_COUNTER_BASE_KABC_L100 - 1]
```

Das ist ein Designentwurf, noch keine fertige Target-API. Insbesondere muss die
Einbindung in `TRICE_BIND_AUTO`, die Trice-Makrofamilien und den anschließenden
Reset des Bind-Routings sauber verifiziert werden.

### 7.3 Implementierung in `trice bind`

Der Generator müsste:

1. alle direkten Trice-Stellen weiterhin mit dem gemeinsamen Parser erfassen;
2. pro physischer Zeile die Spaltenpositionen und daraus lokale Ordinale
   bestimmen;
3. begrenzte Wrapperdefinitionen analysieren und deren innere Trice-Stellen als
   stabile Definitionen registrieren;
4. Wrapperaufrufe erkennen und pro Aufruf eine geordnete ID-Tabelle erzeugen;
5. unmittelbar vor der erweiterten Zeile einen lokalen Counter-Start etablieren;
6. nach der Zeile wieder auf den normalen `(FileKey, Line)`-Dispatch
   zurückschalten;
7. die hinzugefügten Includes bei Re-Migration eindeutig erkennen und entfernen;
8. Source-, Sidecar- und Listenänderungen weiterhin transaktional schreiben.

Der Counter wählt dabei nur einen Tabelleneintrag aus. Die ID-Tabelle stammt
aus der bestehenden stabilen ID-Vergabe.

### 7.4 Anwendungssicht

Für den Anwender bleibt der Ablauf idealerweise unverändert:

```sh
trice bind
cmake --build build
```

Die Originalquelle bleibt ID-frei. Sie erhält jedoch gegebenenfalls zusätzliche
generierte Include-Zeilen um erweiterte Stellen. Diese Includes dürfen nicht
manuell verschoben werden.

Fehlt `__COUNTER__`, entsteht entweder schon beim Bind-Lauf oder spätestens beim
Target-Build eine konkrete Meldung mit folgenden Alternativen:

- `trice insert`/`trice clean` verwenden;
- direkte Trice-Aufrufe auf getrennte Zeilen verteilen;
- einen komplexen Wrapper in eine `static inline`-Funktion umwandeln;
- einen Wrapper auf genau eine innere Trice-Stelle reduzieren.

### 7.5 Vorteile

- Sehr einfacher Userworkflow ohne neue Pflichtoption.
- Der normale Build kompiliert weiterhin die Originalquelle.
- Keine vollständige Target-Präprozessoranalyse durch das Go-Tool nötig.
- Fremde Counterverbräuche vor dem lokalen Rebase sind bedeutungslos.
- Sourcebasierte stabile IDs und bestehende Listenformate bleiben erhalten.
- Der Mechanismus kann nur für tatsächlich erweiterte Stellen aktiviert werden.
- Ein fehlender Counter lässt sich lokal und verständlich diagnostizieren.

### 7.6 Nachteile und Risiken

- Zusätzliche Begin-/End-Includes verändern den sichtbaren Sourcekontext.
- Ein fremder Counterverbrauch zwischen lokalem Start und Trice-Expansion kann
  die lokale Zuordnung weiterhin verschieben.
- Bei Wrappern muss deshalb ausgeschlossen oder verstanden werden, dass andere
  Makros in der Replacement-Liste ebenfalls `__COUNTER__` verwenden.
- Ein Include unmittelbar vor einer Anweisung ist nicht in jedem C-Kontext
  problemlos. Beispiele sind `case`-Labels, Initializer und ungewöhnliche
  Makro-/Statementkombinationen.
- Die ID-Tabelle führt eine C-Ausdrucksebene in die bisher tokenbasierte
  Deskriptorwahl ein. Der Compiler soll sie wegoptimieren, dies muss aber für
  Embedded-Builds bezüglich Flash, RAM und Laufzeit nachgewiesen werden.
- `TRICE_BIND_REPLACE` für explizite Null-Platzhalter ist schwieriger als der
  reine ID-freie `TRICE_BIND_AUTO`-Fall, weil der Verarbeitungsmodus bisher
  bereits im Präprozessor ausgewählt wird.
- Debugzeilen und `li.json` müssen die durch zusätzliche Includes verschobenen
  physischen Zeilen korrekt behandeln.

### 7.7 Sinnvolle erste Begrenzung

Eine risikoarme erste Ausbaustufe sollte nur zulassen:

- ID-freie Trice-Aufrufe;
- mehrere direkte Trices in einer normalen Statementzeile;
- begrenzte Wrapper mit statischen Formatstrings;
- keine verschachtelten Wrapper;
- kein Token-Pasting und keine Stringification;
- keine weitere `__COUNTER__`-Expansion im erweiterten Segment;
- keine Null-Platzhalter im erweiterten Segment, bis der Replace-Pfad separat
  verifiziert ist.

### 7.8 Bewertung

Diese Variante bietet das beste Verhältnis aus einfacher Anwendung und
begrenzter Buildkopplung. Sie ist die bevorzugte Default-Kandidatin, aber der
zeilenlokale Rebase ist durch das vorhandene PoC noch nicht bewiesen. Vor einer
Integration ist ein zusätzlicher PoC mit realen Includes, `case`-Kontexten,
Headern, C, C++ und Optimierungsprüfung erforderlich.

## 8. Variante 2: Exakter Lauf des Target-Präprozessors

### 8.1 Idee für Compiler-Laien

Diese Variante ist mit einer Generalprobe vergleichbar. `trice bind` lässt
zuerst genau den Präprozessor arbeiten, den anschließend auch der Build nutzt.
Es beobachtet dabei die tatsächlich entstehenden Trice-Aufrufstellen und
Counterwerte. Danach schreibt es Sidecars für genau diese beobachtete
Konfiguration.

Die Generalprobe ist nur dann aussagekräftig, wenn Bühne, Darsteller und
Reihenfolge beim eigentlichen Auftritt identisch bleiben. Übersetzt bedeutet
das: Compiler, Optionen, Defines, Includepfade, Forced Includes, PCH und
Headerinhalte müssen übereinstimmen.

### 8.2 Ermittlung des Target-Compile-Kommandos

Mögliche Quellen sind:

- `compile_commands.json`;
- eine bestehende CMake-File-API-Antwort;
- explizite, bereits projektweit verwendete `CC`, `CXX` und Flags;
- ein Buildsystemadapter für Make, Ninja oder eine IDE;
- ein Compiler-Launcher, der den realen Aufruf an `trice bind` meldet.

Das zufällige Auffinden eines Host-`cc` darf bei einem Cross-Compile-Projekt
nicht als Targetnachweis gelten. Ein Host-Clang sagt nichts darüber aus, ob der
spätere Embedded-Compiler dieselben Makros und Includes sieht.

### 8.3 Implementierungsablauf

Ein möglicher Ablauf ist:

1. Die normale Sourceanalyse identifiziert direkte und begrenzte
   makrodefinierte Trice-Stellen.
2. `trice bind` ermittelt das exakte Compile-Kommando jeder betroffenen
   Translation Unit.
3. Ein instrumentierter Präprozessorlauf ersetzt die Trice-Makros temporär
   durch auswertbare Markertokens.
4. Die Marker enthalten mindestens File Key, Aufrufzeile, beobachteten Counter
   und eine generatorseitig bekannte Definitionsidentität.
5. `trice bind` ordnet die Marker den stabilen IDs zu und schreibt Sidecars mit
   den beobachteten Counterwerten.
6. Sidecar und Buildbeschreibung erhalten einen Fingerprint der verwendeten
   Konfiguration.
7. Der normale Compile-Aufruf wiederholt denselben Präprozessorverlauf und
   verwendet die erzeugten Deskriptoren.

### 8.4 Wrappermakros

Der Präprozessor zeigt zuverlässig, welche Wrapperexpansionen in der aktiven
Konfiguration entstehen. Er liefert aber nicht automatisch die gewünschte
fachliche Identität der ursprünglichen Trice-Definition im `#define`.

Deshalb bleibt zusätzlich eine begrenzte Sourceanalyse notwendig:

- Makrodefinition erfassen;
- innere Trice-Stellen nach Definitionsreihenfolge nummerieren;
- jeder inneren Stelle eine stabile ID geben;
- Präprozessormarker einer Definition und einer konkreten Expansion zuordnen.

Eine vollständig allgemeine Rekonstruktion beliebiger Makroherkunft aus
normalem `-E`-Text ist nicht portabel. Compiler-spezifische APIs wie detaillierte
Clang-Preprocessing-Records wären leistungsfähiger, würden `trice bind` aber an
eine Compilerfamilie koppeln.

### 8.5 Mehrere Buildkonfigurationen

Ein Projekt kann dieselbe Datei mit unterschiedlichen Defines mehrfach
kompilieren. Dann können aktive Zweige und Counterfolgen verschieden sein:

```c
#if FEATURE_A
trice("A");
#endif
```

Die Implementierung benötigt hierfür entweder:

- einen Sidecar pro eindeutiger Compile-Konfiguration;
- einen gemeinsamen Sidecar mit konfliktfrei vereinigten Deskriptoren;
- oder die verbindliche Festlegung einer kanonischen Bind-Konfiguration.

Konfigurationsspezifische Sidecars erhöhen die Komplexität von Outputpfaden,
Buildabhängigkeiten und Listenkonsistenz erheblich.

### 8.6 Anwendungssicht

In einem sauber vorbereiteten CMake-/Ninja-Projekt kann der Userworkflow
weiterhin einfach aussehen:

```sh
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
trice bind
cmake --build build
```

Ohne brauchbare Buildbeschreibung müsste `trice bind` erklären, welche
Information fehlt. Eine stillschweigende Host-Compilerannahme wäre nicht
zulässig.

Fehlt `__COUNTER__`, erkennt der Probe-Compile dies vor regulären Ausgaben und
`trice bind` kann transaktional abbrechen.

### 8.7 Vorteile

- Verwendet die reale Target-Toolchain statt Annahmen über Compilerfamilien.
- Berücksichtigt fremde Counterexpansionen in Includes und Makros.
- Erkennt aktive und inaktive Präprozessorzweige der konkreten Konfiguration.
- Liefert bei fehlendem `__COUNTER__` eine frühe Bind-Diagnose.
- Kann komplexere reale Makroexpansionen beobachten als ein reiner Sourceparser.
- Passt gut in vollständig beschriebene, reproduzierbare CI-Builds.

### 8.8 Nachteile und Risiken

- Starke Kopplung von `trice bind` an Compiler, Flags und Buildsystem.
- Cross-Compiler müssen beim Bind-Lauf installiert und ausführbar sein.
- `compile_commands.json` existiert häufig erst nach einer Konfiguration und
  kann veraltet sein.
- Jede relevante Buildvariante kann einen eigenen Präprozessorlauf benötigen.
- Änderungen in eingebundenen Headern zwischen Bind und Build können die
  Counterfolge verschieben.
- Ein einfacher Fingerprint des Kommandos erkennt keine nachträglich geänderten
  Headerinhalte. Vollständige Dependency-Hashes wären aufwendig.
- Eine verschobene Counterfolge muss sicher zu einem Fehler führen. Sie darf
  nicht zufällig einen formal vorhandenen, aber fachlich falschen Deskriptor
  treffen.
- Normale Präprozessorausgabe enthält nicht portabel alle Informationen über
  die Herkunft verschachtelter Makroexpansionen.
- Der Ansatz nähert sich der in der Spezifikation separat genannten aktiven
  Präprozessorkonfigurationsanalyse und erweitert damit den fachlichen Umfang.

### 8.9 Bewertung

Variante 2 ist besonders attraktiv als optionale Verifikationsstufe in CI oder
in Projekten mit zuverlässiger Compilation Database. Als einziges normatives
Standardverfahren würde sie den heutigen einfachen, sourcebasierten Bind-
Workflow zu stark an individuelle Buildsysteme koppeln.

## 9. Variante 3: Generierter Compiler-Input mit expliziten Ordinalen

### 9.1 Idee für Compiler-Laien

`trice bind` erstellt eine nummerierte Arbeitskopie der Source. Die vom User
geschriebene Datei bleibt unverändert, aber der Compiler kompiliert die
Arbeitskopie. Darin wird jede Trice-Stelle explizit nummeriert:

Userquelle:

```c
trice("first"); trice("second");
```

Generierter Compiler-Input, konzeptionell:

```c
TRICE_BIND_AT(0, "first"); TRICE_BIND_AT(1, "second");
```

Da die Nummern bereits im Tokenstrom stehen, sind weder `__LINE__` noch
`__COUNTER__` zur Unterscheidung erforderlich.

### 9.2 Verzeichnis- und Buildmodell

Ein möglicher Output ist:

```text
build/triceBindSource/
  app/main.c
  module/logging.h
  triceBindSources.json
```

Das Manifest ordnet Originaldatei, generierte Datei, Hash und Sidecar einander
zu. Das Buildsystem kompiliert die generierten `.c`-/`.cpp`-Dateien und nimmt
den Shadow-Tree in die Includepfade auf.

### 9.3 Transformation direkter Stellen

Der Generator verwendet denselben Sourceparser und dieselbe ID-Logik wie
`trice insert`, schreibt aber nicht in die Userquelle. Stattdessen erzeugt er
eine Kopie mit internen Bind-Markern. Die Marker können einen File Key und ein
lokales Ordinal tragen:

```c
TRICE_BIND_AT(K73A915E9C4021B8, 0, trice, "first");
TRICE_BIND_AT(K73A915E9C4021B8, 1, trice, "second");
```

Der Target-Header setzt daraus die stabile ID ein. Es gibt keinen globalen
veränderlichen Präprozessorzustand.

### 9.4 Transformation von Makrodefinitionen

Auch innere Trice-Stellen einer begrenzten Makrodefinition erhalten explizite
Definitionsordinale:

```c
#define LOG_ERROR(value)                                      \
    do {                                                      \
        switch (value) {                                      \
        case 0:                                               \
            break;                                            \
        case 7:                                               \
            trice("cannot open file\n");                      \
            break;                                            \
        default:                                              \
            trice("error=%d", 8);                             \
            break;                                            \
        }                                                     \
    } while (0)
```

Die IDs gehören direkt zu den Definitionen 0 und 1. Jede Makroexpansion trägt
diese expliziten Tokens weiter. Ein invocation-spezifischer Counter ist nicht
nötig.

### 9.5 Diagnose- und Debugabbildung

Damit Compilerfehler weiterhin auf Userdateien zeigen, kann der Shadow-Source
`#line`-Direktiven verwenden:

```c
#line 73 "app/main.c"
TRICE_BIND_AT(0, "first"); TRICE_BIND_AT(1, "second");
```

Dies verbessert Diagnose und Debugmapping, löst aber nicht automatisch alle
Unterschiede:

- `__BASE_FILE__` kann weiterhin den Shadow-Pfad nennen.
- Relative Quote-Includes werden zunächst relativ zur generierten Datei
  gesucht.
- Coverage- und Debugwerkzeuge können generierte Pfade anzeigen.
- Compilerdepfiles und IDE-Indizes müssen den Shadow-Tree kennen.

Der Generator müsste daher die originale Verzeichnisstruktur spiegeln und die
Includeauflösung sorgfältig erhalten.

### 9.6 Anwendungssicht

Ohne Buildintegration ist der Ablauf nicht vollständig transparent. Ein
CMake-Projekt könnte ihn jedoch kapseln:

```sh
trice bind
cmake --build build
```

Intern verwendet CMake dann die im Bind-Manifest genannten Shadow-Sourcen.
Andere Buildsysteme benötigen eigene Adapter oder einen Compiler-Launcher.

Ein Projekt ohne `__COUNTER__` könnte diese Variante automatisch oder nach
einer klaren Projektkonfiguration nutzen. Für das vom User gewünschte erste
MVP2 ist aber ebenso zulässig, stattdessen verständlich auf
`trice insert`/`trice clean` zu verweisen.

### 9.7 Vorteile

- Vollständig deterministische Stellenidentität ohne globalen Counterzustand.
- Funktioniert prinzipiell auch auf Compilern ohne `__COUNTER__`.
- Fremde Makros und Header können keine Ordinale verschieben.
- Mehrere direkte Stellen und mehrere innere Wrapperstellen sind eindeutig.
- Explizite Null-Platzhalter und unterschiedliche Verarbeitungsmodi lassen sich
  leichter bereits in der generierten Source normalisieren.
- Keine zweite Target-Präprozessor-Generalprobe nötig.
- Fehlerhafte oder fehlende Ordinale können generatorseitig vollständig
  validiert werden.

### 9.8 Nachteile und Risiken

- Der Compiler verarbeitet nicht mehr die originale Source, sondern ein
  Buildartefakt.
- Jedes unterstützte Buildsystem benötigt Integration.
- Includeauflösung, Debugpfade, Coverage und IDE-Navigation werden komplexer.
- Shadow-Sourcen müssen atomar erzeugt, bereinigt und gegen veraltete Inputs
  abgesichert werden.
- C und C++ besitzen viele syntaktische Sonderfälle; eine Transformation muss
  Token, Kommentare, Zeilenfortsetzungen und Encoding exakt erhalten.
- Die Lösung nähert sich stärker einer Source-to-Source-Instrumentierung und
  damit konzeptionell `trice insert`, obwohl die Userquelle unverändert bleibt.
- Buildzeiten und erzeugtes Datenvolumen steigen.
- Der Anspruch »bestehendes Buildsystem unverändert« ist nicht haltbar.

### 9.9 Bewertung

Variante 3 ist technisch am deterministischsten und der überzeugendste
Counter-unabhängige Fallback. Sie widerspricht aber am stärksten dem Ziel, eine
beliebige vorhandene Anwendung allein durch `trice bind` und einen Includepfad
umzustellen. Sie sollte deshalb nicht die erste Default-Implementierung sein.

## 10. Vergleichsmatrix

| Kriterium | 1: lokaler Rebase | 2: Target-Präprozessor | 3: Shadow-Source |
|---|---|---|---|
| Normaler Useraufruf bleibt `trice bind` | Sehr gut | Gut bei vorhandener Buildbeschreibung | Nur mit Buildadapter |
| Originalsource wird kompiliert | Ja | Ja | Nein |
| Benötigt `__COUNTER__` | Ja | Ja | Nein |
| Früher Fehler bei fehlendem Counter | Wenn Target bekannt, sonst beim Build | Ja | Nicht erforderlich |
| Robust gegen Counterverbrauch vor der Stelle | Ja | Ja, sofern Konfiguration identisch | Vollständig |
| Robust gegen Counterverbrauch innerhalb der Stelle | Nur bei strikten Regeln | Ja, sofern Build identisch | Vollständig |
| Benötigt Target-Compiler beim Bind-Lauf | Nein | Ja | Nein |
| Benötigt Buildsystemintegration | Gering | Mittel bis hoch | Hoch |
| Mehrkonfigurationsaufwand | Gering bis mittel | Hoch | Mittel |
| Unterstützung komplexerer Wrapper | Begrenzt | Mittel, Herkunft bleibt schwierig | Am besten kontrollierbar |
| Embedded-Codegrößenrisiko | ID-Tabelle nachweisen | Gering | Gering |
| Implementierungsaufwand | Hoch | Sehr hoch | Hoch bis sehr hoch |
| Portabilität | Gut bei Counter-Support | Abhängig von Toolchainintegration | Sprachseitig gut, buildseitig schwächer |
| Hauptrisiko | Lokaler Counter wird unerwartet verbraucht | Bind und Build laufen auseinander | Shadow- und Originalbuild unterscheiden sich |

## 11. Empfohlene Produktentscheidung

### 11.1 Empfehlung

Für die nächste `trice bind`-Ausbaustufe wird folgende Kombination empfohlen:

1. **Variante 1 als vorgesehener Default**, sobald ein zusätzlicher
   produktionsnaher Rebase-PoC erfolgreich ist.
2. **Automatische `__COUNTER__`-Prüfung nur bei Counter-pflichtigen Stellen.**
3. **Frühe Compilerprobe**, wenn die tatsächliche Target-Toolchain bereits
   eindeutig bekannt ist.
4. **Target-seitiger Sidecar-Guard als universeller Rückfall**, wenn sie beim
   Bind-Lauf nicht bekannt ist.
5. **Variante 2 optional als Verifikation**, wenn eine zuverlässige
   `compile_commands.json` oder ein Buildadapter vorhanden ist.
6. **Variante 3 zunächst dokumentierter Fallback**, nicht Default. Sie kann
   später für Toolchains ohne Counter oder besonders komplexe Wrapper
   implementiert werden.

Der User muss damit weiterhin keine neue Option lernen. Er verwendet:

```sh
trice bind
```

Nur bei einer tatsächlich Counter-pflichtigen Konstruktion und einer nicht
geeigneten Target-Toolchain erhält er eine Fehlermeldung.

### 11.2 Vorgeschlagener erster Funktionsumfang

Die erste Umsetzung sollte unterstützen:

- mehrere ID-freie direkte Trice-Aufrufe auf einer normalen Sourcezeile;
- begrenzte Wrappermakros mit statischen Formatstrings;
- ein oder mehrere direkte Trices innerhalb eines solchen Wrappers;
- stabile ID pro innerer Makrodefinitionsstelle;
- Wiederverwendung dieser ID an allen Wrapperaufrufstellen;
- C und C++;
- Source- und Headerdateien;
- automatische Capability-Diagnose;
- unveränderte `til.json`-, `li.json`- und Drahtformate.

Zunächst zurückgewiesen werden sollten:

- Token-Pasting im Wrapper;
- Stringification zur Formatstringerzeugung;
- dynamisch zusammengesetzte Formatstrings;
- rekursive oder verschachtelte Wrapper;
- indirekte Umdefinitionen des Trice-Makros;
- unbekannte zusätzliche `__COUNTER__`-Verbräuche im lokalen Segment;
- explizite Null-Platzhalter in Counter-selektierten Stellen, bis der
  Replace-Pfad separat bewiesen ist;
- mehrere Buildkonfigurationen mit widersprüchlicher Wrapperexpansion, solange
  keine klare Sidecar-Semantik spezifiziert ist.

### 11.3 Kein stiller Fallback

Wenn eine erweiterte Stelle nicht sicher gebunden werden kann, darf
`trice bind` sie nicht still wie `trice insert` instrumentieren und nicht eine
beliebige vorhandene ID verwenden. Die Verarbeitung muss mit geordneten
Diagnosen abbrechen.

Beispiel:

```text
module.c:73: error: trice bind found 3 Trice sites on one physical line,
but the target preprocessor does not provide __COUNTER__.
Use trice insert/clean or place one Trice call per line.
```

Für einen Wrapper:

```text
logging.h:41: error: macro LOG_ERROR contains 2 Trice sites and requires
__COUNTER__ for trice bind.
Use trice insert/clean, reduce the macro to one Trice site, or replace it
with a static inline function.
```

### 11.4 Warum `static inline` ein guter Ausweg bleibt

Ein komplexes Loggingmakro lässt sich häufig als `static inline`-Funktion
formulieren. Diese besitzt normale Trice-Stellen in einer physischen Datei und
wird bereits vom heutigen Bind-Modell unterstützt. Der Compiler kann sie in der
Regel genauso inline optimieren wie ein Makro, während Typprüfung, Debugging und
Parserbehandlung einfacher werden.

## 12. Notwendige Generatoränderungen

Die Implementierung sollte keine zweite unabhängige Trice-Parser- oder
ID-Logik einführen. Stattdessen wird die bestehende Bind-Analyse erweitert.

### 12.1 Datenmodell

Eine Bind-Stelle benötigt zusätzlich mindestens:

- physische Zeile und Spalte;
- lokales Ordinal auf der Zeile;
- direkte Stelle oder Makrodefinitionsstelle;
- Makroname und Definitionsordinal, falls vorhanden;
- stabile Definitionsidentität;
- Liste der erkannten Aufrufstellen;
- Kennzeichen `requiresCounter`;
- gewählte Strategie und gegebenenfalls Konfigurationsfingerprint.

### 12.2 Makroanalyse

Eine begrenzte erste Analyse kann folgende Regeln verwenden:

1. Funktionsähnliches `#define` mit vollständiger Replacement-Liste erfassen.
2. Direkte User-Level-Trice-Aufrufe darin mit dem gemeinsamen Parser erkennen.
3. Statische Formatstrings und Argumentanzahl wie bei `insert` validieren.
4. Jede innere Trice-Stelle anhand Definitionsdatei, Definitionsposition und
   Definitionsordinal stabil identifizieren.
5. Aufrufe des Wrappernamens außerhalb der Definition erfassen.
6. Verschachtelte oder nicht transparent analysierbare Wrapper gesammelt
   ablehnen.

Die Makroanalyse muss Zeilenfortsetzungen und Kommentare korrekt behandeln.
Eine reine zeilenweise Suche nach `#define` reicht nicht.

### 12.3 Sidecar-Rendering

Der Sidecar muss klar zwischen fachlicher ID und technischem
Aufrufstellendeskriptor unterscheiden. Beispielhaft:

```c
#define TRICE_BIND_DEFINITION_KABC_D0 iD(5101u)
#define TRICE_BIND_DEFINITION_KABC_D1 iD(5102u)

#define TRICE_BIND_INVOCATION_KABC_L300_O0 TRICE_BIND_DEFINITION_KABC_D0
#define TRICE_BIND_INVOCATION_KABC_L300_O1 TRICE_BIND_DEFINITION_KABC_D1
```

Die endgültige Syntax hängt von der gewählten Variante ab. Die Trennung im
Datenmodell verhindert jedoch, dass eine zufällige Aufrufposition zur
fachlichen Identität wird.

### 12.4 Listeninformation

Für Wrapper wird empfohlen:

- `til.json`: Typ und Formatstring der inneren Definitionsstelle;
- `li.json`: Definitionsdatei und Definitionszeile, nicht die erste zufällige
  Aufrufstelle;
- Sidecar: technische Aufrufstellendeskriptoren, die dieselbe ID
  wiederverwenden.

Bei mehreren inneren Trices besitzt jede Definitionsstelle einen eigenen
Eintrag.

### 12.5 Transaktion und Diagnosen

Alle Dateien werden zuerst analysiert. Erst wenn keine fatale Diagnose
vorliegt, werden Sidecars, Includes, `til.json` und `li.json` gemeinsam
committed. Fehldiagnosen sollten Datei, Zeile, Makroname und Grund enthalten.

## 13. Erforderliche Verifikation vor der Produktintegration

### 13.1 Counter-Capability

- Compiler mit `__COUNTER__`;
- simulierter Compiler ohne `__COUNTER__`;
- C und C++;
- Strict-/Pedantic-Modi und Warnings-as-Errors;
- vorkompilierte Header;
- Forced Includes;
- Cross-Compiler.

### 13.2 Mehrere direkte Stellen

- zwei und drei Trices auf einer Zeile;
- verschiedene Trice-Makrofamilien;
- identische und unterschiedliche Formatstrings;
- andere Makroaufrufe zwischen den Trice-Stellen;
- Header und Source;
- statische und Inline-Funktionen;
- bedingte Compilation.

### 13.3 Wrapper

- genau ein inneres Trice;
- mehrere innere Trices in `switch`-/`if`-Zweigen;
- mehrere Wrapperaufrufe;
- wiederverwendete Definitions-IDs;
- Wrapper in Headern mit mehreren Translation Units;
- absichtlich nicht unterstützte verschachtelte Wrapper;
- Zeilenfortsetzungen mit LF und CRLF;
- Kommentare und auskommentierte Fake-Aufrufe.

### 13.4 Stabilität

- zweiter Bind-Lauf erzeugt byteidentische Ausgaben;
- Einfügen einer fremden `__COUNTER__`-Expansion vor der lokalen Stelle;
- Einfügen einer fremden Expansion innerhalb des lokalen Segments;
- Quellzeilen werden verschoben;
- Build ohne vorherigen neuen Bind-Lauf muss erkennbar scheitern;
- parallele Analyse bleibt deterministisch;
- fataler Fehler hinterlässt keine Teilresultate.

### 13.5 Embedded-Eigenschaften

- kein zusätzlicher RAM-Verbrauch;
- kein unerwarteter Flashverbrauch durch ID-Tabellen;
- keine zusätzlichen Laufzeitloads nach Optimierung;
- unverändertes Drahtformat;
- identisches Loggingverhalten zu Insert;
- GCC-/Clang-/MSVC- und relevante Embedded-Compiler-Matrizen.

## 14. Offene Entscheidungsfragen

Vor der Implementierung müssen insbesondere folgende Punkte entschieden oder
durch PoCs beantwortet werden:

1. Kann ein lokaler Counter-Rebase in allen unterstützten Sourcekontexten
   syntaktisch sicher platziert werden?
2. Erzeugt eine lokale ID-Tabelle nach Optimierung garantiert keinen messbaren
   Code- oder RAM-Overhead?
3. Werden Counter-pflichtige Null-Platzhalter zunächst abgelehnt oder sofort
   unterstützt?
4. Wie wird ein zusätzlicher Counterverbrauch innerhalb eines Wrappermakros
   sicher erkannt?
5. Welche Makroverschachtelung gilt noch als »begrenzt und transparent«?
6. Genügt eine Build-time-Diagnose bei unbekannter Toolchain oder soll jedes
   Projekt eine Target-Capability-Datei bereitstellen?
7. Wie wird bei mehreren Compile-Konfigurationen derselben Datei verfahren?
8. Soll Variante 2 nur Test-/CI-Verifikation oder ein öffentlich unterstützter
   Generatorpfad sein?
9. Welche Buildsysteme würden für einen späteren Shadow-Source-Fallback
   offiziell unterstützt?

## 15. Schlussfolgerung

Die wesentlichen sourcebezogenen MVP2-Fähigkeiten sind technisch realistisch,
wenn der Umfang bewusst begrenzt bleibt. Die breite Verfügbarkeit von
`__COUNTER__` in GCC, Clang und MSVC spricht dafür, Counter-Unterstützung direkt
in die nächste Ausbaustufe aufzunehmen. Die konkrete Toolchain muss dennoch
automatisch geprüft werden; eine angenommene Compilerliste reicht nicht.

Für ein möglichst glattes Nutzererlebnis ist der zeilenlokale Counter-Rebase
die bevorzugte Richtung. Er hält den Aufruf bei `trice bind`, kompiliert die
Originalsource und aktiviert die zusätzliche Komplexität nur für betroffene
Stellen. Sein verbleibendes Risiko ist die zuverlässige lokale Countersequenz.
Dieses Risiko muss vor der Produktintegration durch einen gezielten Rebase-PoC
und Embedded-Codegrößenprüfung geschlossen werden.

Der exakte Target-Präprozessorlauf ist eine wertvolle optionale Absicherung,
aber als alleinige Lösung zu buildabhängig. Die Shadow-Source mit expliziten
Ordinalen ist der robuste Counter-unabhängige Rückfall, verlangt jedoch die
größte Änderung an Build und Debugworkflow.

Die empfohlene Reihenfolge lautet daher:

1. zeilenlokalen Rebase als PoC verifizieren;
2. begrenzte direkte Mehrfachstellen und Wrapper implementieren;
3. automatische Target-Capability-Diagnose integrieren;
4. exakten Präprozessorlauf optional zur Verifikation ergänzen;
5. Shadow-Source nur bei realem Bedarf als separaten Fallback entwickeln.

