# Spezifikation: `trice bind`

**Version:** 4  
**Status:** Spezifikationsentwurf für das MVP  
**Repository-Bezug:** Branch `wip`, insbesondere `internal/id`, `src`, `_test/testdata/triceCheck.c`, `experiments/TriceBind/20_Target_Library_Integration` sowie `experiments/TriceBind/30_Preprocessor_Verification`<br>
**Zielgruppe:** Implementierung, Review, Tests und spätere Übernahme in die Trice-Dokumentation

## 1. Ziel

`trice bind` ist ein neues Subkommando zur stabilen ID-Vergabe ohne numerische IDs in bind-verwalteten User-Trice-Aufrufen.

Der Usercode bleibt beispielsweise:

```c
trice("msg:Hello trice bind world\n");
```

`trice bind` verwendet dieselbe ID-Datenbasis und dieselbe fachliche Zuordnungslogik wie `trice insert`, schreibt die ermittelte ID jedoch in einen temporären, dateilokalen Sidecar-Header. Die Originalquelle wird anschließend direkt kompiliert.

Dauerhaft und versionsverwaltet sind:

- die Userquellen,
- `til.json`,
- `li.json`,
- die einmalig eingefügte Sidecar-Include-Zeile mit stabilem File Key.

Die Sidecars sind reproduzierbare Buildartefakte und werden normalerweise nicht versioniert.

Der normale Workflow lautet:

```text
trice bind
Build
```

`trice bind` MUSS nach jeder Änderung einer gescannten Source- oder Headerdatei vor dem Build ausgeführt werden. Ein veralteter Sidecar kann trotz erfolgreicher Compilation eine falsche ID-Zuordnung enthalten; ein Compilerfehler ist deshalb kein ausreichender Aktualitätsnachweis.

## 2. Normative Begriffe

- **MUSS:** verbindliche Anforderung des MVP.
- **SOLL:** erwartetes Verhalten; Abweichungen benötigen einen dokumentierten Grund.
- **KANN:** optionale Eigenschaft.

## 3. Kompatibilität

Das MVP ist als additive Erweiterung spezifiziert.

- `trice insert` und `trice clean` bleiben verfügbar.
- Dieselbe Trice-Library unter `./src` unterstützt Insert-, Bind- und Clean-Projekte.
- Bestehende CLI-Aufrufe behalten ihre Bedeutung.
- Das Schema von `til.json` und `li.json` bleibt unverändert.
- Das Trice-Drahtformat bleibt unverändert.
- Es wird weder eine separate Bind-Library noch ein öffentliches `TRICE_MODE` eingeführt.
- Ist `TRICE_CLEAN` in `triceConfig.h` vorhanden, hält `trice bind` den Wert auf `0`; `trice bind` fügt die Definition nicht neu ein.

Abweichungen davon gelten als Breaking Change und MÜSSEN vor einem Merge ausdrücklich dokumentiert und mit einer Migrationsanleitung versehen werden. Nach aktuellem Stand ist kein Breaking Change erforderlich.

Die vollständige Wirkung von `TRICE_CLEAN`, einschließlich Migration, Re-Migration und `trice clean` nach einem Bind-Lauf, ist in [Anhang B](#anhang-b-trice_clean-migration-und-re-migration) beschrieben.

## 4. Dateiklassifikation

`trice bind` klassifiziert jede physische Source- oder Headerdatei anhand ihrer vom ID-Parser verwalteten Trice-Stellen.

### 4.1 Insert-owned

Alle verwalteten Trice-Stellen besitzen explizite IDs größer null:

```c
trice(iD(123), "msg:legacy\n");
```

Für eine Insert-owned Datei gilt:

- `trice bind` validiert IDs, Typen, Formatstrings sowie `til.json` und `li.json` mit der bestehenden Insert-Semantik.
- Die Datei wird nicht verändert.
- Es wird kein eigener Sidecar erzeugt.
- Im normalen Lauf ist keine Warnung erforderlich.

### 4.2 Bind-owned

Die Datei enthält ausschließlich ID-freie Aufrufe und/oder Null-Platzhalter:

```c
trice("msg:bound\n");
TRICE(ID(0), "msg:bound with explicit stamp\n");
```

Für eine Bind-owned Datei erzeugt und verwaltet `trice bind` File Key, Sidecar-Include und Sidecar.

### 4.3 Gemischt

Eine Datei enthält sowohl explizite IDs größer null als auch bindbare ID-freie beziehungsweise Null-Platzhalter-Aufrufe:

```c
trice(iD(123), "msg:legacy\n");
trice("msg:new\n");
```

Dieser Zustand ist im MVP ein Fehler. Der User entscheidet sich dateiweise vollständig für `insert` oder `bind`; zur Migration einer solchen Datei ist zuvor `trice clean` auszuführen.

Bereiche zwischen `TRICE_INSERT_OFF` und `TRICE_INSERT_ON` werden wie bei `trice insert` nicht verwaltet und beeinflussen die Klassifikation nicht.

### 4.4 Koexistenz im Projekt

Insert-owned und Bind-owned Dateien dürfen im selben Projekt vorkommen.

Bind-owned Header können in Insert-owned Dateien eingebunden werden. Bevor die Insert-owned Datei eigene Trice-Aufrufe mit expliziten IDs verwendet, MUSS sie nach ihren Includes den Bind-Kontext entfernen:

```c
#include "bound_header.h"

#undef TRICE_BIND_FILE_KEY

trice(iD(123), "msg:insert-owned source\n");
```

Die alternative Idee, expliziten IDs im Bind-Dispatch automatisch Vorrang zu geben, wird im MVP nicht verfolgt; siehe [Anhang G.3](#g3-automatischer-vorrang-expliziter-ids).

## 5. Unterstützte Trice-Stellen

### 5.1 Gemeinsame Parser- und ID-Logik

`trice bind` MUSS denselben Parser und dieselbe Makroerkennung wie `trice insert` verwenden. Eine unabhängig nachimplementierte, nur ähnliche Erkennung ist nicht ausreichend.

Wiederzuverwenden sind insbesondere:

- User-Level-Makroerkennung,
- Alias- und Simple-Alias-Behandlung,
- ID-Wrapper-Erkennung,
- Typ- und Formatstring-Ermittlung,
- bestehende Syntax- und Konsistenzprüfungen,
- ID-Wiederverwendung und ID-Vergabe,
- ID-Bereich und Vergabemethode,
- Aktualisierung von `til.json` und `li.json`,
- `-defaultStampSize`,
- `TRICE_INSERT_OFF` und `TRICE_INSERT_ON`.

Das MVP MUSS alle User-Level-Trice-Makros unterstützen, die von `trice insert` und der Target-Library als öffentliche User-Schnittstelle unterstützt werden. Dazu gehören auch Spezialformen wie Assertions und `triceAssertOrReturnValue`, sofern sie Teil dieser Schnittstelle sind.

Registrierte `-alias`- und `-salias`-Namen werden mit derselben Semantik wie bei `trice insert` behandelt. Neue Unterstützung für beliebige Wrappermakros mit eigener Semantik ist nicht Bestandteil des MVP.

### 5.2 ID-Zustände in Bind-owned Dateien

#### ID-freier Aufruf

```c
trice("msg:hello\n");
TRICE8_3("msg:%d %d %d\n", a, b, c);
```

Der Sidecar liefert den vollständigen TID-Ausdruck. Für Makrofamilien mit mindestens einem Kleinbuchstaben wird `iD(...)` verwendet. Für ausschließlich großgeschriebene User-Level-Makros bestimmt `-defaultStampSize` die Form:

- `0` → `id(...)`,
- `16` → `Id(...)`,
- `32` → `ID(...)`.

#### Null-Platzhalter

```c
TRICE8_3(id(0), "msg:%d %d %d\n", a, b, c);
TRICE8_3(Id(0), "msg:%d %d %d\n", a, b, c);
TRICE8_3(ID(0), "msg:%d %d %d\n", a, b, c);
```

Die Form `id`, `Id` beziehungsweise `ID` MUSS erhalten bleiben. Nur die Null wird semantisch durch die stabile ID ersetzt. Ein expliziter Null-Platzhalter hat Vorrang vor `-defaultStampSize`.

### 5.3 Unterstützte Sourcekontexte

Das MVP unterstützt direkte User-Level-Trice-Aufrufe in:

- C- und C++-Sourcedateien,
- Headerdateien,
- normalen Funktionen,
- `inline`- und `static inline`-Funktionen.

Die Identität ist die textuelle Stelle in der physischen Datei, nicht eine spätere Compilerinstanz oder Inlining-Entscheidung.

### 5.4 Nicht unterstützte Konstruktionen

Das MVP unterstützt nicht:

- mehrere bindbare Trice-Stellen in derselben physischen Sourcezeile,
- Trice-Aufrufe innerhalb gewöhnlicher Präprozessormakrodefinitionen,
- vom bestehenden Insert-Parser nicht unterstützte zusammengesetzte Formatstrings,
- nicht transparente Custom-Wrapper außerhalb der vorhandenen Alias-Semantik.

`trice bind` meldet diese Fälle gesammelt als Fehler und führt keinen stillen Teil-`insert`-Fallback aus. Beispiele und Begründungen stehen in [Anhang E](#anhang-e-nicht-unterstützte-konstruktionen-im-mvp). Mögliche spätere Erweiterungen sind ausschließlich in [Anhang F](#anhang-f-mvp2-optionen-und-unabhängige-spätere-erweiterungen) beschrieben.

## 6. File Key und Sidecar

### 6.1 File Key

Jede Bind-owned Datei erhält einen einmalig erzeugten zufälligen 64-Bit-Key:

```text
K73A915E9C4021B8
```

Eigenschaften:

- Präfix `K`,
- 16 großgeschriebene Hexadezimalziffern,
- Erzeugung mit Go `crypto/rand`,
- Speicherung im Sidecar-Include-Namen der Userdatei,
- Wiederverwendung bei späteren Läufen,
- keine Neuberechnung bei Dateiinhalt-, Pfad- oder Namensänderungen,
- keine Übertragung zum Target,
- keine Änderung des JSON-Schemas.

`trice bind` MUSS prüfen, dass ein Key nicht unterschiedlichen Besitzerdateien zugeordnet ist. Eine Kopie einer Datei einschließlich Sidecar-Include erzeugt deshalb einen Fehler, bis eine der Dateien einen neuen Key erhält.

Dass ein Header und sein Sidecar in mehreren Translation Units verarbeitet werden, ist dagegen vorgesehen und kein Key-Konflikt.

### 6.2 Sidecar-Name und Ausgabeordner

Namensschema:

```text
trice_<normalisierter-Basisname>_<FileKey>.h
```

Beispiel:

```text
module.c
→ trice_module_c_K73A915E9C4021B8.h
```

Der Basisname einschließlich Source-Extension dient der Lesbarkeit. Zeichen außerhalb `[A-Za-z0-9_]` werden durch `_` ersetzt. Die Eindeutigkeit liefert der File Key.

Alle Sidecars liegen standardmäßig flach unter dem vom User gewählten `-bindDir` mit Default:

```text
./build/triceIDs
```

Der Ordner wird bei Bedarf erzeugt und MUSS unabhängig von den User-Ausschlüssen automatisch vom Source-Scan ausgeschlossen werden. Der Build benötigt diesen Ordner als Include-Pfad.

### 6.3 Sidecar-Inhalt

Das MVP verwendet einen einzeiligen Site-Deskriptor pro bindbarer Trice-Stelle:

```c
/// \file trice_module_c_K73A915E9C4021B8.h
/// \brief Generated by trice bind. Do not edit.

// TRICE_INSERT_OFF - Trice parser exclusion marker

#undef TRICE_BIND_FILE_KEY
#define TRICE_BIND_FILE_KEY K73A915E9C4021B8
#define TRICE_BIND_ROUTE_K73A915E9C4021B8 BIND

// -defaultStampSize 16
#define TRICE_BIND_SITE_K73A915E9C4021B8_L9    TRICE_BIND_AUTO,    Id(12345u) // TRICE("Hello");
#define TRICE_BIND_SITE_K73A915E9C4021B8_L10   TRICE_BIND_REPLACE, id(12346u) // TRICE(id(0), "world");
#define TRICE_BIND_SITE_K73A915E9C4021B8_L11   TRICE_BIND_AUTO,    iD(12347u) // trice("!\n");
```

Zur besseren Lesbarkeit SOLLEN die einzelnen `TRICE_BIND_...`, `iD(...)` und `// trice(...);` ausgerichtet sein. `TRICE_INSERT_OFF` ist im Sidecar formal nicht erforderlich, aber als zusätzliche Sicherung vorgesehen, falls der User den Sidecar kopiert hat.

`TRICE_BIND_AUTO` bedeutet: Der TID-Ausdruck wird als fehlendes erstes Argument eingefügt.

`TRICE_BIND_REPLACE` bedeutet: Ein vorhandener Null-Platzhalter wird verworfen und durch den im Sidecar gespeicherten TID-Ausdruck ersetzt.

Normativ gilt:

- Jede Site-Definition steht vollständig in einer physischen Zeile.
- Der Sidecar enthält keinen klassischen Include-Guard.
- Die Definitionen sind nach Sourcezeile sortiert.
- Der vollständige TID-Ausdruck ist direkt lesbar.
- Der Kommentar enthält den normalisierten Trice-Aufruf oder mindestens den Formatstring.
- Bei mehrzeiligem Source wird nur die erste logische Zeile gezeigt und mit `...` abgeschlossen.
- Der Kommentar darf keine unbeabsichtigte Präprozessor-Zeilenfortsetzung erzeugen.
- Der Sidecar wird nur ersetzt, wenn sich sein Inhalt tatsächlich ändert.

Die verifizierte Präprozessormechanik ist in [Anhang A](#anhang-a-verifizierte-target-makromechanik) dokumentiert.

### 6.4 Datei ohne aktuelle Trice-Stelle

Besitzt eine Datei bereits einen Sidecar-Include, aktuell aber keine verwaltete Trice-Stelle, bleiben Include, File Key und ein leerer Sidecar erhalten. Dadurch bleiben Dateiidentität und Include-Position stabil.

## 7. Sidecar-Includes

### 7.1 Form und Erkennung

Eine Bind-owned Datei enthält mindestens einmal:

```c
#include "trice_module_c_K73A915E9C4021B8.h" // trice-bind: keep as last include before this file's Trice calls
```

Der Kommentar ist für Menschen und nicht Teil der technischen Identität. Wird er entfernt, MUSS `trice bind` den Include weiterhin anhand der Präprozessordirektive und des Sidecar-Namens erkennen.

Zulässig sind übliche Whitespace-Varianten wie:

```c
# include "trice_module_c_K73A915E9C4021B8.h"
```

### 7.2 Warum der Include dateilokal sein muss

Der Sidecar stellt nicht nur Definitionen bereit, sondern wählt vor den folgenden Trice-Stellen den File Key und den Bind-Dispatch der physischen Datei. Eine zentrale Einbindung aller Sidecars kann diese lokale Auswahl nicht ersetzen.

Die Begründung und die genaue Expansion stehen in [Anhang A.1](#a1-warum-ein-einfaches-ifdef-nicht-genügt).

### 7.3 Automatisches Einfügen

Fehlt der Include, fügt `trice bind` ihn standardmäßig automatisch ein.

Die MVP-Heuristik ist bewusst konservativ:

1. Der letzte textuell erkannte `#include` vor der ersten bindbaren Trice-Stelle wird bestimmt.
2. Gibt es vor diesem Include keine bindbare Trice-Stelle, wird der Sidecar unmittelbar danach eingefügt.
3. Gibt es keine vorherige Include-Direktive, wird der Sidecar vor der ersten bindbaren Trice-Stelle eingefügt.
4. Liegt bereits eine bindbare Trice-Stelle vor einem späteren Include, wird nicht automatisch eingefügt; der Lauf meldet einen Fehler mit der erforderlichen Include-Zeile.

Die Heuristik wertet keine vollständige C/C++-Präprozessorkonfiguration aus. Kann sie keine konservativ sichere Position bestimmen, MUSS der User den Include manuell platzieren.

### 7.4 Mehrfache Einbindung

Derselbe Sidecar darf in seiner Besitzerdatei mehrfach eingebunden werden, um nach einem späteren Header-Include den eigenen File Key erneut zu aktivieren:

```c
#include "trice_module_c_K73A915E9C4021B8.h"
trice("msg:first\n");

#include "late_header.h"
#include "trice_module_c_K73A915E9C4021B8.h"
trice("msg:second\n");
```

Mehrfache identische Einbindungen sind im normalen Bind-Lauf keine Warnung. Widersprüchliche Keys oder ein fremder Besitzer-Sidecar sind Fehler.

## 8. Kommandozeile

### 8.1 Subkommando

```sh
trice bind [Optionen]
```

Das Subkommando verwendet die bestehende CLI- und Flag-Infrastruktur und erscheint in der regulären Hilfe.

### 8.2 Gemeinsame Optionen mit `trice insert`

`trice bind` übernimmt grundsätzlich alle für Source-Suche, Parserverhalten, ID-Vergabe und Listenverwaltung relevanten Optionen von `trice insert` mit identischen Defaults und identischer Semantik.

In der `bind`-Hilfe dürfen diese Optionen vollständig angezeigt werden. Die Implementierung SOLL gemeinsame Registrierung und Validierung wiederverwenden, statt parallele Definitionen zu pflegen.

Nicht übernommen werden nur Optionen, die ausschließlich die textuelle Darstellung dauerhaft instrumentierter Userquellen steuern und im Bind-Ablauf keine definierte Wirkung besitzen.

### 8.3 Bind-spezifische Option

```text
-bindDir string
    Ausgabeordner der Sidecar-Header.
    Default: ./build/triceIDs
```

Der endgültige Optionsname darf an bestehende Projektkonventionen angepasst werden; die Semantik ist verbindlich.

### 8.4 Dry Run

Das bereits vorhandene `-dry-run` dient als Vorschau. Es berechnet File Keys, Include-Änderungen, IDs, Listenänderungen und Sidecars, schreibt jedoch keine User-, JSON- oder Sidecar-Datei.

Eine separate MVP-Option zum Unterdrücken der automatischen Include-Einfügung ist nicht vorgesehen.

### 8.5 Verbose-Ausgabe

Bei entsprechender Verbosity SOLL `trice bind` mindestens anzeigen:

- klassifizierte Datei (`insert-owned`, `bind-owned`, `mixed`),
- eingefügten oder erkannten Sidecar-Include,
- erzeugten oder aktualisierten Sidecar,
- Anzahl wiederverwendeter und neuer IDs,
- übersprungene Insert-owned Dateien.

## 9. Konsistenzanforderungen

- Parser-, Alias-, ID- und Listenlogik werden mit `trice insert` geteilt.
- Die Dateiverarbeitung verwendet dieselbe Parallelitätsstrategie wie `trice insert`.
- Ergebnisse sind bei identischer Eingabe deterministisch. Ausnahme: Neue IDs die mit random-Policy vergeben werden.
- Alle analysierbaren Fehler werden vor dem Schreiben regulärer Ausgaben gesammelt.
- Unveränderte Dateien werden nicht ersetzt.
- `bindDir` wird automatisch vom Source-Scan ausgeschlossen.

Der nicht normative Implementierungsumriss steht in [Anhang C](#anhang-c-implementierungsumriss).

## 10. Validierung und Diagnosen

Das MVP MUSS mindestens folgende Fehler erkennen:

- derselbe File Key gehört zu unterschiedlichen Besitzerdateien,
- eine Datei ist `mixed`,
- ein Sidecar-Name oder Key widerspricht der Besitzerdatei,
- eine bindbare Site kann nicht eindeutig aufgelöst werden,
- mehrere bindbare Trice-Stellen liegen auf derselben physischen Zeile,
- ein Trice-Aufruf liegt in einer nicht unterstützten Makrodefinition,
- Alias- oder Formatstringkonstruktion wird vom gemeinsamen Parser nicht unterstützt,
- ID, Typ, Formatstring, `til.json` oder `li.json` widersprechen sich,
- `-defaultStampSize` ist ungültig,
- ein Sidecar-Include kann nicht konservativ sicher automatisch platziert werden,
- der für eine bindbare Stelle aktive File Key ist nicht eindeutig,
- ein erzeugter Site-Name ist für den Build nicht definiert.

Die von `trice insert` bereits ausgeführten syntaktischen und semantischen Prüfungen werden unverändert übernommen, einschließlich vorhandener Sonderbehandlungen für spezielle Makrofamilien.

Alle erkennbaren Diagnosen werden gesammelt. Exitstatus:

- keine Fehler: `0`,
- nur Hinweise beziehungsweise Warnungen: `0`,
- mindestens ein Fehler: ungleich `0`.

Ein umfassender Hygiene- und Berichtslauf ist nicht Teil des MVP; siehe [Anhang F.1](#f1-mvp2-option-check-modus).

## 11. Zusammenspiel mit `insert` und `clean`

### 11.1 `trice insert`

`trice insert` MUSS Dateien mit einem erkannten eigenen Sidecar-Include im MVP überspringen. Dadurch werden bind-verwaltete Dateien nicht versehentlich wieder instrumentiert.

Insert-owned Dateien ohne eigenen Sidecar werden unverändert wie bisher verarbeitet.

### 11.2 `trice clean`

`trice clean` entfernt weiterhin IDs aus Insert-owned Dateien. Bind-owned Dateien enthalten bereits keine IDs größer null; ihre Sidecar-Includes und Sidecars bleiben bestehen.

Das genaue Verhalten mit und ohne `TRICE_CLEAN` ist in [Anhang B](#anhang-b-trice_clean-migration-und-re-migration) normativ erläutert.

### 11.3 Migration zu `bind`

Für ein bisher vollständig mit `insert` verwaltetes Projekt:

```text
1. trice clean
2. trice bind
3. Build
```

`trice bind` erzeugt die File Keys, fügt Sidecar-Includes ein, erzeugt Sidecars und hält ein vorhandenes `TRICE_CLEAN` auf `0`.

### 11.4 Re-Migration zu `insert`

Der Rückweg ist bewusst manuell:

1. Eigene Sidecar-Includes aus den betreffenden Dateien entfernen.
2. Gegebenenfalls manuelle `#undef TRICE_BIND_FILE_KEY`-Zeilen bereinigen.
3. `trice insert` ausführen.
4. Nicht mehr benötigte Sidecars löschen.

Ein automatisches Re-Migrationskommando ist nicht Bestandteil des MVP.


---

## Anhang A: Verifizierte Target-Makromechanik

Die beiden vor V4 erforderlichen Präprozessormechanismen wurden in zwei unabhängigen PoCs unter folgendem Repositorypfad verifiziert:

```text
experiments/TriceBind/30_Preprocessor_Verification/
├── 10_Local_Dispatch/
└── 20_Site_Descriptor/
```

Die PoCs sind Referenznachweise für die Mechanik, noch keine vollständige Integration in die Trice-Makromatrix.

### A.1 Warum ein einfaches `#ifdef` nicht genügt

Folgendes funktioniert nicht:

```c
/* in trice.h */
#ifdef TRICE_BIND_FILE_KEY
#define trice(...) TRICE_BIND_IMPL(__VA_ARGS__)
#else
#define trice(...) TRICE_INSERT_IMPL(__VA_ARGS__)
#endif
```

Userdatei:

```c
#include "trice.h"
#include "trice_module_c_K73A915E9C4021B8.h"

trice("msg:hello\n");
```

Beim Einlesen von `trice.h` ist `TRICE_BIND_FILE_KEY` noch nicht definiert. Der Präprozessor legt deshalb bereits den Insert-Pfad fest. Der spätere Sidecar-Include wertet das frühere `#ifdef` nicht erneut aus.

Präprozessordirektiven können auch nicht verzögert in eine Makro-Replacement-Liste eingebettet werden.

### A.2 Expansion-Time-Dispatch

Stattdessen wird die Route bei jeder Makroexpansion aus dem aktuell sichtbaren Key gebildet.

Vereinfachte Referenzmechanik aus `10_Local_Dispatch`:

```c
#define TRICE_BIND_ROUTE_TRICE_BIND_FILE_KEY INSERT
#define TRICE_BIND_ROUTE_I(key) TRICE_BIND_ROUTE_##key
#define TRICE_BIND_ROUTE(key) TRICE_BIND_ROUTE_I(key)

#define TRICE_DISPATCH_I(route, ...) TRICE_ROUTE_##route(__VA_ARGS__)
#define TRICE_DISPATCH(route, ...) TRICE_DISPATCH_I(route, __VA_ARGS__)

#define trice(...) TRICE_DISPATCH(TRICE_BIND_ROUTE(TRICE_BIND_FILE_KEY), __VA_ARGS__)
```

Sidecar:

```c
#undef TRICE_BIND_FILE_KEY
#define TRICE_BIND_FILE_KEY K73A915E9C4021B8
#define TRICE_BIND_ROUTE_K73A915E9C4021B8 BIND
```

#### Ohne aktiven Sidecar

`TRICE_BIND_FILE_KEY` ist nicht definiert und bleibt bei der Expansion als Token stehen:

```text
TRICE_BIND_ROUTE(TRICE_BIND_FILE_KEY)
→ TRICE_BIND_ROUTE_TRICE_BIND_FILE_KEY
→ INSERT
```

Der bestehende Inserted-Pfad wird gewählt:

```c
trice(iD(1001u), "msg:inserted\n");
```

#### Mit aktivem Sidecar

```text
TRICE_BIND_ROUTE(TRICE_BIND_FILE_KEY)
→ TRICE_BIND_ROUTE(K73A915E9C4021B8)
→ TRICE_BIND_ROUTE_K73A915E9C4021B8
→ BIND
```

Der Bind-Pfad wird gewählt:

```c
trice("msg:bound\n");
```

#### Rückkehr zum Inserted-Pfad

```c
#undef TRICE_BIND_FILE_KEY
```

Danach bleibt der Key wieder als ungelöstes Fallback-Token stehen und die Route wird `INSERT`.

Damit dient `TRICE_BIND_FILE_KEY` gleichzeitig als:

- Dateiidentität,
- lokaler Bind-Indikator,
- Umschaltpunkt zurück zum Inserted-Pfad.

Ein zusätzlicher öffentlicher Modus oder ein `TRICE_SOURCE_KIND` ist nicht erforderlich.

### A.3 Einzeiliger Site-Deskriptor

Die zweite Verifikation zeigt, dass pro Site genau eine Definition genügt:

```c
#define TRICE_BIND_SITE_K73A915E9C4021B8_L101 TRICE_BIND_REPLACE, id(12346u) // TRICE(id(0), "world");
```

Der Deskriptor enthält:

1. `TRICE_BIND_AUTO` oder `TRICE_BIND_REPLACE`,
2. den vollständigen TID-Ausdruck.

Vereinfachte Referenzmechanik aus `20_Site_Descriptor`:

```c
#define TRICE_BIND_SITE_I(key, line) TRICE_BIND_SITE_##key##_L##line
#define TRICE_BIND_SITE(key, line) TRICE_BIND_SITE_I(key, line)
#define TRICE_BIND_SITE_HERE() TRICE_BIND_SITE(TRICE_BIND_FILE_KEY, __LINE__)

#define TRICE_BIND_APPLY(...) TRICE_BIND_APPLY_I(__VA_ARGS__)
#define TRICE_BIND_APPLY_I(mode, tid, ...) mode(tid, __VA_ARGS__)

#define TRICE_BIND_AUTO(tid, ...) TRICE_INTERNAL(tid, __VA_ARGS__)
#define TRICE_BIND_REPLACE(tid, ignoredTid, ...) TRICE_INTERNAL(tid, __VA_ARGS__)

#define TRICE(...) TRICE_BIND_APPLY(TRICE_BIND_SITE_HERE(), __VA_ARGS__)
```

In der vollständigen Library wird dieser Site-Mechanismus in den unter A.2 beschriebenen lokalen Route-Dispatch eingebettet. Der bestehende Inserted-Pfad bleibt unverändert.

### A.4 Vollständiges Beispiel

Userdatei:

```c
#include "trice.h"
#include "trice_module_c_K73A915E9C4021B8.h" // trice-bind: keep as last include before this file's Trice calls

void f(void)
{
    TRICE("Hello");
    TRICE(id(0), "world");
    trice("!\n");
}
```

Angenommene Sourcezeilen 6, 7 und 8 sowie `-defaultStampSize 16`:

```c
/// \file trice_module_c_K73A915E9C4021B8.h
/// \brief Generated by trice bind. Do not edit.

#undef TRICE_BIND_FILE_KEY
#define TRICE_BIND_FILE_KEY K73A915E9C4021B8
#define TRICE_BIND_ROUTE_K73A915E9C4021B8 BIND

// -defaultStampSize 16
#define TRICE_BIND_SITE_K73A915E9C4021B8_L6 TRICE_BIND_AUTO, Id(12345u) // TRICE("Hello");
#define TRICE_BIND_SITE_K73A915E9C4021B8_L7 TRICE_BIND_REPLACE, id(12346u) // TRICE(id(0), "world");
#define TRICE_BIND_SITE_K73A915E9C4021B8_L8 TRICE_BIND_AUTO, iD(12347u) // trice("!\n");
```

Expansion der zweiten Stelle:

```text
TRICE_BIND_SITE_HERE()
→ TRICE_BIND_SITE_K73A915E9C4021B8_L7
→ TRICE_BIND_REPLACE, id(12346u)
```

Zusammen mit den Userargumenten:

```text
TRICE_BIND_APPLY(
    TRICE_BIND_REPLACE, id(12346u),
    id(0), "world"
)
```

wird daraus:

```text
TRICE_BIND_REPLACE(id(12346u), id(0), "world")
→ TRICE_INTERNAL(id(12346u), "world")
```

Die Legacy-Stamp-Form `id` bleibt erhalten. Der Null-Platzhalter wird nicht im Usercode verändert.

### A.5 Verifikationsstatus

Die PoCs wurden mit GCC und Clang als C11- sowie C++17-Frontend und mit strengen Warnoptionen ausgeführt. Sie verifizieren:

- lokales Umschalten nach einem Sidecar-Include,
- Fallback auf den Inserted-Pfad ohne Key,
- Rückkehr per `#undef`,
- `TRICE_BIND_AUTO`,
- `TRICE_BIND_REPLACE`,
- `id`, `Id`, `ID` und `iD`,
- Aufrufe mit und ohne Datenargumente.

Die Integration in alle echten Trice-Makrofamilien bleibt eine Implementierungs- und Testaufgabe.

## Anhang B: `TRICE_CLEAN`, Migration und Re-Migration

### B.1 Rolle von `TRICE_CLEAN`

`TRICE_CLEAN` bleibt ein optionaler Legacy-Status beziehungsweise Compile-Time-Schalter. Bei Wert `1` verwendet `trice.h` den bestehenden Clean-/Off-Pfad. Bei Wert `0` oder fehlender Definition wird die lokale Insert-/Bind-Auswahl aus Anhang A verwendet.

`TRICE_CLEAN` ersetzt keine fehlende Sidecar-Datei. Ein physisch nicht vorhandener, aber inkludierter Sidecar kann weiterhin eine Editor- oder Compilerdiagnose erzeugen.

### B.2 Wirkung der Werkzeuge

| Werkzeug | User-Trice-Aufrufe | Sidecar-Includes | Sidecars | vorhandenes `TRICE_CLEAN` |
|---|---|---|---|---|
| `trice insert` | setzt IDs in Insert-owned Dateien; überspringt Bind-owned Dateien | unverändert | unverändert | setzt auf `0` |
| `trice bind` | schreibt keine IDs in Bind-owned Aufrufe | erzeugt/erhält | erzeugt/aktualisiert | setzt auf `0` |
| `trice clean` | entfernt IDs aus Insert-owned Dateien | unverändert | unverändert | setzt auf `1` |

Fehlt `TRICE_CLEAN`, fügt `trice bind` keine Definition hinzu.

### B.3 `trice clean` nach `trice bind`

#### Mit `TRICE_CLEAN` in `triceConfig.h`

Ausgangszustand:

```c
#define TRICE_CLEAN 0
```

Nach:

```sh
trice clean
```

steht:

```c
#define TRICE_CLEAN 1
```

Die ID-freien Bind-Aufrufe, Sidecar-Includes und Sidecars bleiben unverändert. Der Clean-/Off-Pfad verhindert jedoch normale Trice-Codegenerierung.

Ein anschließendes:

```sh
trice bind
```

setzt die vorhandene Definition wieder auf `0` und aktualisiert die Sidecars.

#### Ohne `TRICE_CLEAN`

`trice clean` findet in Bind-owned Dateien keine IDs größer null. Es entfernt weder Sidecar-Includes noch Sidecars und ändert keinen Modus. Der Bind-Zustand bleibt daher praktisch unverändert.

Folgerung:

> In einem stabilen Bind-Projekt wird `trice clean` nicht verwendet.

### B.4 Migration von `insert` zu `bind`

1. Projektzustand sichern beziehungsweise committen.
2. `trice clean` ausführen, damit die zu migrierenden Dateien keine IDs größer null mehr enthalten.
3. `trice bind` ausführen.
4. Eingefügte Sidecar-Includes, `til.json` und `li.json` reviewen und versionieren.
5. `build/triceIDs` als generierten Ordner behandeln.
6. `trice bind` als verpflichtenden Pre-Build-Schritt integrieren.

Ist `TRICE_CLEAN` vorhanden, setzt `clean` den Wert zunächst auf `1`; `bind` setzt ihn anschließend auf `0`.

### B.5 Re-Migration von `bind` zu `insert`

Der Rückweg erfolgt manuell und dateiweise:

1. Eigene Sidecar-Includes aus den betroffenen Dateien entfernen.
2. Manuelle `#undef TRICE_BIND_FILE_KEY`-Zeilen entfernen, soweit sie nicht mehr benötigt werden.
3. Nicht mehr benötigte Sidecars löschen.
4. `trice insert` ausführen.
5. Das Ergebnis und die Listen prüfen.

Durch die manuelle Entfernung des Sidecar-Includes wird die Datei wieder vom Inserted-Pfad übernommen.

### B.6 Hybridprojekte

Ein Projekt darf Insert-owned und Bind-owned Dateien enthalten. Der Workflow ist dateibezogen:

- `trice bind` validiert Insert-owned Dateien und bindet Bind-owned Dateien.
- `trice insert` verarbeitet Insert-owned Dateien und überspringt Dateien mit eigenem Sidecar-Include.
- `trice clean` kann Insert-owned Dateien ID-frei machen; sie werden dadurch bei einem folgenden Bind-Lauf zu Bind-Kandidaten.

Ein globales `trice clean` in einem Hybridprojekt sollte deshalb nur bei beabsichtigter Migration ausgeführt werden.

## Anhang C: Implementierungsumriss

### C.1 CLI-Integration

`bind` wird wie die bestehenden Subkommandos in die vorhandene CLI- und Flag-Infrastruktur eingebunden. Gemeinsame Insert-/Bind-Optionen werden aus denselben Definitionen registriert und validiert.

Nur tatsächlich bind-spezifische Werte erhalten neue Felder, insbesondere der Sidecar-Ausgabeordner.

### C.2 Vorgeschlagene Verantwortlichkeiten

Die konkrete Dateiaufteilung darf der Repositorystruktur folgen. Sinngemäß werden benötigt:

```text
internal/id/bindIDs.go       Orchestrierung
internal/id/bindParse.go     Klassifikation, Site- und Include-Analyse
internal/id/bindRender.go    Sidecar-Erzeugung
internal/id/bindTypes.go     Pläne, Sites und Diagnosen
src/trice*.h                 lokaler Dispatch und Site-Anwendung
```

Gemeinsame Insert-/Bind-Funktionalität wird extrahiert oder parametrisiert, nicht dupliziert.

### C.3 Verarbeitungsphasen

1. Optionen und Pfade validieren.
2. `til.json` und `li.json` mit der bestehenden Logik laden.
3. Kandidatendateien mit derselben Walk- und Ausschlusslogik wie `trice insert` bestimmen; `bindDir` zusätzlich ausschließen.
4. Dateien parallel einlesen und `TRICE_INSERT_OFF`-/`ON`-Bereiche maskieren.
5. Vorhandene Sidecar-Includes und File Keys erfassen.
6. Dateien als Insert-owned, Bind-owned oder mixed klassifizieren.
7. Fehlende Keys und Include-Änderungen planen.
8. Den finalen In-Memory-Sourcezustand einschließlich geplanter Includes herstellen.
9. Für Bind-owned Dateien die bestehende Insert-ID-Zuordnung in-memory ausführen.
10. Sidecar-Deskriptoren, Diagnosen und globale Konflikte bestimmen.
11. Nach fehlerfreier Analyse JSON-Dateien, Source-Includes und Sidecars konsistent schreiben.

### C.4 In-Memory-Insert-Pass

Für Bind-owned Dateien wird der finale In-Memory-Sourcezustand an die bestehende Insert-Logik übergeben. Diese liefert beziehungsweise bestimmt:

- numerische ID,
- `iD`-, `id`-, `Id`- oder `ID`-Form,
- Typ,
- Formatstring,
- Location,
- bisherige Listenprüfungen.

Aus der Änderung pro Site wird gerendert:

- `TRICE_BIND_AUTO`, wenn `insert` ein fehlendes TID-Argument ergänzt hätte,
- `TRICE_BIND_REPLACE`, wenn `insert` einen Null-Platzhalter ersetzt hätte.

Der virtuell instrumentierte Sourceinhalt wird nicht in die Userdatei geschrieben.

### C.5 Dateiplan

Eine interne Dateiplan-Struktur enthält mindestens:

- Pfad und Klassifikation,
- Originalinhalt,
- finalen Inhalt mit geplanter Include-Änderung,
- vorhandenen oder neuen File Key,
- Sidecar-Namen und Sidecar-Inhalt,
- Bind-Sites,
- Diagnosen.

Ein Bind-Site enthält mindestens:

- File Key,
- finale Zeile und Spalte,
- User-Level-Makroname beziehungsweise Alias,
- Typ und Formatstring,
- numerische ID,
- Stamp-Form,
- Modus `AUTO` oder `REPLACE`,
- normalisierten Sourcetext für den Kommentar.

### C.6 Transaktion und Parallelität

Analyse und Sidecar-Aufbereitung laufen parallel entsprechend dem vorhandenen Insert-Modell. Globale ID- und Key-Konflikte werden mit den bestehenden Synchronisationsmechanismen beziehungsweise klar abgegrenzten Registries geschützt.

Nach der Analyse folgt eine deterministische Commit-Phase. Ein echtes virtuelles Dateisystem ist nicht vorgeschrieben; Tests können reale temporäre Verzeichnisse verwenden.

## Anhang D: Implementierung und Tests

### D.1 Essenzielle Target-Nachweise

Die beiden vorhandenen PoCs sind vor und während der Integration als Regressionstests beizubehalten:

- `10_Local_Dispatch`: lokaler Expansion-Time-Dispatch,
- `20_Site_Descriptor`: einzeiliger Site-Deskriptor.

Die echte Trice-Library MUSS dieselben Eigenschaften für repräsentative Makrofamilien nachweisen:

1. generisches Lower-/Mixed-Case-Makro,
2. ausschließlich großgeschriebenes Makro einschließlich `-defaultStampSize`,
3. Spezialfamilie wie String oder Buffer,
4. Assertion beziehungsweise `triceAssertOrReturnValue`.

### D.2 Unit-Tests

Mindestens:

- File-Key-Erzeugung, Wiederverwendung und Konflikte,
- Sidecar-Namensbildung,
- Include-Erkennung unabhängig vom Kommentar,
- konservative automatische Include-Einfügung,
- mehrfache Sidecar-Reaktivierung,
- Dateiklassifikation,
- ID-freie und Null-Platzhalter-Sites,
- Stamp-Semantik,
- Aliasbehandlung,
- `TRICE_INSERT_OFF`/`ON`,
- deterministisches Rendering ohne Zeilenumbruch,
- Kommentar-Normalisierung,
- gesammelte und sortierte Diagnosen,
- atomisches beziehungsweise inhaltsabhängiges Schreiben.

### D.3 Makroabdeckung

`_test/testdata/triceCheck.c` ist in einem temporären Testbaum zu verwenden. `trice bind` fügt dort die benötigten Sidecar-Includes selbst ein. Die kanonische Testquelle soll keine Bind-spezifischen `#if`-Blöcke erhalten.

Für jedes vom Projekt angebotene User-Level-Makro muss ein positiver Bind-Test oder ein dokumentierter Negativtest existieren.

### D.4 Testskripte

Während der Inbetriebnahme ist der kurze Pflichtlauf:

```sh
./_testAll_10_PcTargetTests.sh
```

Zusätzlich ist ein Bind-Testskript analog zu den bestehenden Insert-Tests vorzusehen. Gemeinsame Shell-Hilfen und temporäre Kopien vermeiden Codeduplizierung.

Vor Merge läuft mindestens:

```sh
./scripts/testAll.sh quick
```

Der vollständige Lauf bleibt Bestandteil der abschließenden Repository- beziehungsweise Release-Regression, ist aber nicht der innere Entwicklungszyklus:

```sh
./scripts/testAll.sh full
```

### D.5 Generator-PoC

`experiments/TriceBind/20_Target_Library_Integration` sowie die beiden Verifikations-PoCs bleiben unverändert erhalten. Für den echten Generator wird unter `experiments/TriceBind/40_MVP_Generator` ein zusätzlicher unabhängiger PoC angelegt.


### D.6 Abnahmekriterien

Das MVP gilt als implementiert, wenn:

1. `trice bind` als dokumentiertes Subkommando verfügbar ist.
2. Die fachliche Parser-, ID- und Listenlogik von `trice insert` wiederverwendet wird.
3. Bind-owned User-Trice-Aufrufe keine numerischen IDs enthalten.
4. Insert-owned und Bind-owned Dateien projektweit koexistieren können; gemischte Dateien werden abgelehnt.
5. File Keys, Includes und Sidecars stabil und deterministisch erzeugt werden.
6. Die Target-Library den lokalen Insert-/Bind-Dispatch sowie `TRICE_BIND_AUTO` und `TRICE_BIND_REPLACE` für die vollständige unterstützte Makromatrix bereitstellt.
7. ID-freie Uppercase-Makros, `id(0)`, `Id(0)`, `ID(0)` und Lower-/Mixed-Case-Makros die bestehende Stamp-Semantik bewahren.
8. Direkte Trice-Aufrufe in Source-, Header- und `static inline`-Code funktionieren.
9. `TRICE_INSERT_OFF` und `TRICE_INSERT_ON` identisch zu `insert` wirken.
10. `til.json`, `li.json` und der Datenstrom kompatibel bleiben.
11. `experiments/TriceBind/20_Target_Library_Integration` unverändert erhalten bleibt und unter `experiments/TriceBind/40_MVP_Generator` ein zusätzlicher unabhängiger Generator-PoC hinzukommt.
12. Die beiden Verifikations-PoCs aus `experiments/TriceBind/30_Preprocessor_Verification` weiterhin erfolgreich laufen.
13. `_test/testdata/triceCheck.c` über einen temporären Bind-Testbaum bind-fähig geprüft wird.
14. Die bestehenden Insert-, Clean-, Update- und Logtests grün bleiben.
15. Die in [Anhang D](#anhang-d-implementierung-und-tests) festgelegten Entwicklungs- und Regressionstests erfolgreich sind.

## Anhang E: Nicht unterstützte Konstruktionen im MVP

### E.1 Mehrere direkte Trice-Stellen pro Zeile

```c
trice("msg:first\n"); trice("msg:second\n");
```

Beide Stellen besitzen denselben File Key und denselben `__LINE__`-Wert. Im MVP sind getrennte Sourcezeilen zu verwenden.

### E.2 Trice-Aufruf in einer Makrodefinition

```c
#define LOG_ERROR(n) trice("msg:error=%d\n", n)
```

Der Trice-Aufruf wird erst bei der Expansion von `LOG_ERROR` verarbeitet. File Key und `__LINE__` beschreiben dann die Aufrufstelle. Eine kompatible Definitionsstellen-Semantik ist im einfachen MVP nicht allgemein verfügbar.

Eine `static inline`-Funktion ist im MVP unterstützt:

```c
static inline void logError(int n)
{
    trice("msg:error=%d\n", n);
}
```

### E.3 Zusammengesetzter Formatstring

```c
#define PREFIX "msg:"
trice(PREFIX "value=%d\n", value);
```

Diese Form folgt den bereits bestehenden Grenzen des Insert-Parsers und des Trice-Formatstringmodells. Variable Texte sollen über geeignete Parameter- beziehungsweise Stringformen übertragen werden.

### E.4 Nicht transparenter Custom-Wrapper

```c
#define MY_TRACE(x) trice("msg:value=%d\n", transform(x))
```

Dies ist kein bloßer Aliasname, sondern eine Makrodefinition mit eigener Semantik und fällt unter E.2.

## Anhang F: MVP2-Optionen und unabhängige spätere Erweiterungen

Alle Inhalte dieses Anhangs liegen ausdrücklich außerhalb des MVP. Sie dürfen erst nach separater Spezifikation und Verifikation implementiert werden.

### F.1 MVP2-Option: Check-Modus

Ein späteres:

```sh
trice bind -check
```

kann zusätzliche Hygiene- und Berichtsfunktionen enthalten:

- leere Sidecars,
- verwaiste Sidecars,
- Sidecars ohne Besitzerdatei,
- ungewöhnliche mehrfache Einbindungen,
- Dateien ohne aktuelle Trice-Stelle,
- möglicherweise nicht mehr benötigtes `trice.h`,
- alte Site-Definitionen,
- Abweichungen zwischen Source, Sidecar, `til.json` und `li.json`,
- optionaler Hinweis auf eine nicht mehr benötigte `TRICE_CLEAN`-Definition.

Der normale MVP-Lauf bleibt auf bindungsrelevante Fehler konzentriert.

### F.2 MVP2-Option: Begrenzte Wrappermakros

Ein späteres MVP2 kann einfache Wrapper unterstützen:

```c
// module.h
#define LOG_ERROR(n) trice("msg:error=%d\n", n)
```

Aufrufstellen:

```c
LOG_ERROR(0); // Zeile 7
LOG_ERROR(1); // Zeile 8
```

Möglicher Sidecar:

```c
#define TRICE_BIND_SITE_K73A915E9C4021B8_L7 TRICE_BIND_AUTO, iD(12345u) // LOG_ERROR(0) -> trice("msg:error=%d\n", n)
#define TRICE_BIND_SITE_K73A915E9C4021B8_L8 TRICE_BIND_AUTO, iD(12345u) // LOG_ERROR(1) -> trice("msg:error=%d\n", n)
```

Beide Aufrufstellen verwenden dieselbe stabile ID der Makrodefinition. Die Location Information soll deterministisch auf die Definitionsstelle verweisen.

Eine portable sourcebasierte Unterstützung ist für folgende begrenzte Klasse denkbar:

- genau ein direkter Trice-Aufruf in der Replacement-Liste,
- statischer Formatstring,
- kein Token-Pasting,
- keine Formatstringerzeugung per Stringification,
- keine verschachtelten Wrapper,
- höchstens ein Wrapperaufruf pro physischer Sourcezeile.

Ein GCC-/Clang-Präprozessorlauf kann optional Aufrufstellen liefern, ist aber wegen abweichender Targetcompiler, vordefinierter Makros und inaktiver Zweige nicht die bevorzugte normative Grundlage.

### F.3 MVP2-Option: Mehrere Trice-Stellen pro Zeile

Ein gesonderter `__COUNTER__`-PoC kann prüfen, ob mehrere direkte Trice-Stellen derselben Sourcezeile zuverlässig unterscheidbar sind. Generator- und Compilerexpansion müssen dabei nachweislich denselben Counterverlauf besitzen.

Diese Option ist unabhängig von F.2. `__COUNTER__` stellt nicht automatisch die bisherige Definitionsstellen-Semantik eines Wrappermakros her.

### F.4 MVP2-Option: Maximale Sidecar-Zeilenlänge

Das MVP bricht Site-Definitionen nicht um. Falls praktische Compiler- oder Werkzeuggrenzen auftreten, kann später eine Option wie:

```text
-maxLineLength
```

spezifiziert werden.

### F.5 MVP2-Option: Aktive Präprozessorkonfiguration

Ein optionaler Lauf des tatsächlichen Targetpräprozessors kann berichten, welche sourcebasiert erfassten Logstellen in einer konkreten Buildkonfiguration aktiv sind. Das Binding und die stabile ID-Vergabe bleiben sourcebasiert.

### F.6 MVP2-Option: Post-Link-Inventur

Eine optionale ELF- oder Link-Map-Auswertung kann berichten, welche aktiven Logstellen nach Optimierung, LTO, Archivselektion und Section-Garbage-Collection im finalen Image verbleiben. Sie verändert das Binding nicht.

### F.7 MVP2-Option: Vorbereitete statische Libraries

Vorbereitete `.a`-Libraries können Metadaten und bindbare ID-Platzhalter enthalten. Ein späterer Bind-Schritt kann sie in den ID-Raum des Endprodukts einordnen. Beliebige unvorbereitete Libraries sind damit nicht nachträglich bindbar.

### F.8 MVP2-Option: Tiefere `insert`-Prüfung von Bind-Dateien

Das MVP lässt `trice insert` Dateien mit eigenem Sidecar-Include aus. Eine spätere Erweiterung kann zusätzlich Sidecar, `til.json` und `li.json` auf Konsistenz prüfen.

### F.9 Unabhängige spätere Option: Referenzliste der User-Level-Makros

Unabhängig von `trice bind` kann `trice generate` eine maschinenlesbare und dokumentierbare Referenz der User-Level-Makros erzeugen, beispielsweise unter `docs/ref`. Parser, Tests und Dokumentation sollten dafür möglichst eine gemeinsame Registry verwenden.

Mögliche Ausgaben:

```text
docs/ref/TriceUserMacros.md
docs/ref/TriceUserMacros.json
```

Diese Option ist keine Voraussetzung des Bind-MVP.

### F.10 Unabhängige spätere Option: Allgemeinere Parser-Marker

Die vorhandenen Marker `TRICE_INSERT_OFF` und `TRICE_INSERT_ON` bleiben im MVP unverändert. Später können zusätzliche neutrale Synonyme wie `TRICE_ID_OFF` und `TRICE_ID_ON` geprüft werden.

## Anhang G: Nicht verfolgte Alternativen

### G.1 Globales `TRICE_MODE`

Ein zusätzlicher globaler Modus ist nicht erforderlich. `TRICE_CLEAN` wählt weiterhin den Off-Pfad; der lokale File Key wählt bei der Makroexpansion Insert oder Bind.

### G.2 Einfaches `#ifdef TRICE_BIND_FILE_KEY`

Ein beim Einlesen von `trice.h` ausgewertetes `#ifdef` sieht den später inkludierten Sidecar nicht. Der verifizierte Expansion-Time-Dispatch aus Anhang A wird verwendet.

### G.3 Automatischer Vorrang expliziter IDs

Ein Bind-Dispatch, der ein vorhandenes explizites ID-Argument automatisch erkennt und bevorzugt, würde die Target-Makrokomplexität erhöhen. Im MVP erfolgt die Trennung dateiweise; im seltenen Hybridfall wird `TRICE_BIND_FILE_KEY` explizit undefiniert.

### G.4 Separate `HAS_TID`-Definitionen

Zwei Sidecar-Definitionen pro Site sind nicht erforderlich. Der einzeilige Deskriptor enthält Verarbeitungsmodus und vollständigen TID-Ausdruck.

### G.5 Generierte Schattenquellen

Das MVP kompiliert die Userquellen direkt. Ein virtueller Insert-Inhalt darf intern zur Ermittlung der Site-Daten entstehen, wird aber nicht als Buildquelle verwendet.

### G.6 Allgemeines ELF-Patching

Eine allgemeine ELF-Patch-Lösung wird für normale Userquellen nicht verfolgt. Spezielle ELF-basierte Analysen oder vorbereitete Libraries können additive Erweiterungen sein; siehe die in [`Trice_bind_90_MVP_User_Manual.md`](./Trice_bind_90_MVP_User_Manual.md#anhang-d-unverändert-übernommene-architekturentscheidung-gegen-elf-patching) übernommene Architekturentscheidung.

## Anhang H: Repositoryreferenzen

- [`internal/id`](../../internal/id): gemeinsame Parser-, Alias- und ID-Verwaltung
- [`internal/id/insertIDs.go`](../../internal/id/insertIDs.go): bestehende Insert-Zuordnung und Parallelverarbeitung
- [`src/trice.h`](../../src/trice.h), [`src/triceOn.h`](../../src/triceOn.h) und [`src/triceOff.h`](../../src/triceOff.h): Target-Makroschicht
- [`_test/testdata/triceCheck.c`](../../_test/testdata/triceCheck.c): breite Makroabdeckung
- [`scripts/testAll.sh`](../../scripts/testAll.sh): Repositoryregression
- [`experiments/TriceBind/20_Target_Library_Integration`](../../experiments/TriceBind/20_Target_Library_Integration): bisheriger Target-Bind-PoC
- [`experiments/TriceBind/30_Preprocessor_Verification`](../../experiments/TriceBind/30_Preprocessor_Verification): verifizierter lokaler Dispatch und einzeiliger Site-Deskriptor
