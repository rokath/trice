# Spezifikation: `trice bind`

**Status:** Entwurf für das MVP  
**Bezug:** aktueller `wip`-Stand mit `examples/PoC_bind_v2`  
**Zielgruppe:** Implementierung, Review und spätere Übernahme in die Trice-Dokumentation

## 1. Zweck

`trice bind` ist ein neues Subkommando als Alternative zum Workflow

```text
trice insert
Build
trice clean
```

Es verwendet dieselbe stabile ID-Verwaltung wie `trice insert`, schreibt die IDs aber nicht in die Trice-Aufrufe der Userquellen. Stattdessen erzeugt es dateispezifische Sidecar-Header mit Präprozessorkonstanten.

Der Usercode bleibt in der ID-freien Form:

```c
trice("msg:Hello trice bind world\n");
```

Der Compiler übersetzt weiterhin die Originalquellen. Es werden keine generierten C- oder C++-Schattenquellen gebaut.

Die dauerhaften Informationen bleiben in:

- `til.json`,
- `li.json`,
- den markierten Sidecar-Include-Zeilen der Quelldateien.

Die Sidecar-Header sind reproduzierbare Buildartefakte und werden normalerweise nicht versioniert.

## 2. Normative Begriffe

In diesem Dokument bedeuten:

- **MUSS:** verbindliche Anforderung des MVP,
- **SOLL:** erwartetes Verhalten, von dem nur mit dokumentiertem Grund abgewichen werden darf,
- **KANN:** optionale Eigenschaft.

## 3. Kompatibilität

### 3.1 Keine beabsichtigten Breaking Changes

Das MVP erfordert keine Breaking Changes.

Insbesondere gilt:

- `trice insert` bleibt unverändert verfügbar.
- `trice clean` bleibt unverändert verfügbar.
- Bestehende CLI-Aufrufe behalten ihre Bedeutung.
- Das Format von `til.json` und `li.json` wird nicht geändert.
- Das Trice-Drahtformat wird nicht geändert.
- Das Targetverhalten bleibt unverändert, solange `TRICE_BIND` nicht mit dem Wert `1` aktiviert ist.
- Explizite Trice-Aufrufe mit ID behalten außerhalb des Bind-Modus ihre bisherige Bedeutung.

`TRICE_BIND` wird zu einem dokumentierten Trice-Konfigurationsmakro. Da der Präfix `TRICE_` zum Namensraum der Library gehört, ist dies eine additive Erweiterung. Die Einführung SOLL dennoch in den Release Notes erwähnt werden.

### 3.2 Bewusste Sourceänderung

`trice bind` KANN einmalig eine markierte Sidecar-Include-Zeile in eine Quelldatei einfügen. Diese kleine Sourceänderung ist Teil des gewählten Modells und wird versioniert.

Normale spätere `bind`-Läufe verändern die Userquellen nicht.

Mit `-dry-run` MUSS jede geplante Sourceänderung sichtbar sein, ohne geschrieben zu werden.

## 4. Abgrenzung des MVP

### 4.1 Unterstützt

Das MVP unterstützt:

- ID-freie lowercase-Aufrufe `trice(...)`,
- direkte Aufrufe in C- und C++-Quelldateien,
- direkte Aufrufe in Headerdateien,
- direkte Aufrufe in normalen Funktionen,
- direkte Aufrufe in `static inline`-Funktionen,
- statische, vom bestehenden Trice-Parser unterstützte Formatstrings,
- alle bestehenden Mechanismen von `trice insert` zur Wiederverwendung und Vergabe stabiler IDs,
- alle textuell vorhandenen unterstützten Logstellen, auch in aktuell inaktiven Präprozessorzweigen.

### 4.2 Nicht unterstützt

Das MVP unterstützt nicht:

- mehrere bindbare Trice-Aufrufe in derselben physischen Sourcezeile,
- Trice-Aufrufe innerhalb einer Präprozessormakrodefinition, beispielsweise

  ```c
  #define LOG_ERROR(x) trice("err:value=%d\n", x)
  ```

- durch Präprozessorverkettung erzeugte oder veränderte Formatstrings,
- automatisch erzeugte Aufrufstellen-IDs für Wrappermakros,
- eine Präprozessoranalyse der aktiven Buildkonfiguration,
- eine Post-Link-Inventur der im Image verbliebenen Trice-Stellen,
- nachträgliches Binding vorkompilierter Libraries,
- generierte C- oder C++-Schattenquellen.

Nicht unterstützte Konstruktionen MÜSSEN zu einer präzisen Diagnose führen. Sie dürfen nicht stillschweigend falsch gebunden werden.

Projekte mit Trice-Aufrufen in Makrodefinitionen verwenden für diese Quellen weiterhin `trice insert`.

## 5. Usermodell

### 5.1 Projektkonfiguration

Ein Bind-Projekt aktiviert im `triceConfig.h`:

```c
#define TRICE_BIND 1
```

Der Sidecar-Ausgabeordner muss im Include-Suchpfad des Compilers liegen, beispielsweise:

```text
-I./triceIDs
```

Diese Buildkonfiguration ist projektspezifisch und wird von `trice bind` im MVP nicht automatisch geändert.

### 5.2 Markierter Sidecar-Include

Eine Datei mit bindbaren Trice-Aufrufen enthält genau einen zu ihr gehörenden markierten Include, beispielsweise:

```c
#include "trice_module_c_F73A915E9C4021B8.h" // trice-bind
```

Die Include-Zeile gehört zur Userquelle und wird versioniert.

Sie erfüllt zwei Aufgaben:

1. Sie speichert den stabilen File Key der Datei.
2. Sie wählt vor den eigenen Trice-Aufrufen den passenden Sidecar und damit den aktuellen `TRICE_FILE_KEY`.

Eine zentrale Einbindung aller Sidecars in `triceConfig.h` ersetzt den dateilokalen Include nicht. Sie würde zwar alle ID-Makros sichtbar machen, aber nicht festlegen, welcher Dateischlüssel für die aktuelle Sourcezeile gilt.

### 5.3 File Key

Der File Key ist ein einmalig erzeugter zufälliger 64-Bit-Wert:

```text
F73A915E9C4021B8
```

Eigenschaften:

- Präfix `F`, damit der Wert sicher als Bestandteil eines C-Identifier verwendbar ist,
- 16 Hexadezimalziffern in Großschreibung,
- Erzeugung mit `crypto/rand`,
- Wiederverwendung aus einer vorhandenen markierten Include-Zeile,
- keine Neuberechnung bei Pfad-, Namens- oder Inhaltsänderungen,
- keine Übertragung zum Target,
- kein Eintrag im bestehenden JSON-Schema erforderlich.

Der Generator MUSS projektweit prüfen, dass derselbe File Key nicht zu verschiedenen Quelldateien gehört.

Wird eine Datei einschließlich ihrer markierten Include-Zeile kopiert, MUSS `trice bind` den doppelten Key melden. Zur Behebung entfernt der User die markierte Include-Zeile aus einer der Kopien und führt `trice bind` erneut aus.

### 5.4 Sidecar-Dateiname

Das vorgeschlagene Namensschema lautet:

```text
trice_<lesbarer Dateistamm>_<FileKey>.h
```

Beispiel:

```text
module.c
→ trice_module_c_F73A915E9C4021B8.h
```

Der lesbare Dateistamm entsteht aus dem Basisnamen einschließlich Extension. Zeichen außerhalb `[A-Za-z0-9_]` werden durch `_` ersetzt.

Der Dateistamm dient ausschließlich der Lesbarkeit. Die Eindeutigkeit liefert der File Key. Dadurch können alle Sidecars in einem flachen Verzeichnis liegen, auch wenn ein Projekt viele gleichnamige Dateien enthält.

Eine bereits vorhandene markierte Include-Zeile ist für Dateiname und File Key maßgeblich. Das Umbenennen einer Source-Datei erzwingt daher keine automatische Umbenennung ihres Sidecars.

### 5.5 Sidecar-Inhalt

Ein Sidecar enthält mindestens:

```c
// Generated by trice bind. Do not edit.

#undef TRICE_FILE_KEY
#define TRICE_FILE_KEY F73A915E9C4021B8

#define TRICE_ID_F73A915E9C4021B8_L9 12345u // msg:Hello trice bind world
```

Anforderungen:

- keine klassischen Include-Guards,
- deterministische Reihenfolge nach aufsteigender Sourcezeile,
- genau eine Definition pro bindbarer Sourcezeile,
- Makroname `TRICE_ID_<FileKey>_L<Zeile>`,
- ID als Dezimalzahl mit Suffix `u`,
- einzeiliger diagnostischer Kommentar mit dem normalisierten Formatstring,
- atomisches Ersetzen nur bei tatsächlicher Inhaltsänderung.

Ein Include-Guard wäre unzweckmäßig, weil ein erneutes Include desselben Sidecars den passenden `TRICE_FILE_KEY` bewusst wiederherstellen kann.

Eine doppelte Einbindung desselben Sidecars ist nicht automatisch fehlerhaft. `trice bind` SOLL sie melden, darf sie im MVP aber als Warnung behandeln, sofern alle Vorkommen denselben File Key verwenden.

### 5.6 Datei ohne aktuelle Logstellen

Existiert bereits ein markierter Sidecar-Include, obwohl die Datei aktuell keine bindbare Trice-Stelle enthält, SOLL der Include erhalten bleiben. Der Generator KANN einen Sidecar erzeugen, der nur `TRICE_FILE_KEY` setzt.

Dadurch bleibt die Dateiidentität erhalten, wenn später erneut Trice-Aufrufe ergänzt werden.

## 6. Position und Verwaltung des Sidecar-Includes

### 6.1 Verbindliche Position

Der eigene Sidecar MUSS:

- nach allen textuell vorhandenen normalen `#include`-Direktiven der Datei,
- vor dem ersten bindbaren Trice-Aufruf der Datei

stehen.

Bei Headerdateien MUSS er innerhalb eines üblichen Include-Guards liegen.

Der Sidecar-Include SOLL nicht innerhalb eines bedingten Präprozessorzweigs stehen. Ein üblicher Header-Guard ist hiervon ausgenommen.

Diese Regel stellt sicher:

1. Sidecars eingebundener Header können zunächst deren eigenen File Key wählen.
2. Der Sidecar der aktuellen Datei stellt danach deren File Key wieder her.
3. Alle direkten Trice-Aufrufe der aktuellen Datei verwenden `Datei + __LINE__`.

### 6.2 Vorhandener Include

Findet `trice bind` einen markierten Include, MUSS es:

- den File Key daraus übernehmen,
- den Include nicht ohne Not verschieben oder neu formatieren,
- prüfen, dass Sidecar-Name und File Key zusammenpassen,
- prüfen, dass der Include vor der ersten bindbaren Trice-Stelle liegt,
- prüfen, ob danach weitere textuelle Includes folgen,
- doppelte Vorkommen desselben Includes mindestens warnen,
- fremde oder widersprüchliche Sidecar-Includes als Fehler melden.

Ist die Position ungültig, MUSS die Diagnose die Datei, die Include-Zeile und eine geeignete Zielposition nennen. Das MVP verschiebt einen bereits vorhandenen Include nicht automatisch.

### 6.3 Fehlender Include

Fehlt der Include und enthält die Datei bindbare Trice-Stellen, gilt:

1. `trice bind` erzeugt einen neuen File Key.
2. Es bestimmt den letzten textuell vorhandenen normalen Include vor der ersten bindbaren Trice-Stelle.
3. Es fügt den Sidecar-Include unmittelbar danach ein.
4. Bei Headern wird der Include innerhalb eines erkannten üblichen Include-Guards eingefügt.
5. Die Zeile wird mit `// trice-bind` markiert.

Kann keine sichere Position bestimmt werden, MUSS `trice bind` mit einer Diagnose abbrechen oder eine nur heuristisch eingefügte Zeile ausdrücklich als manuell zu prüfen melden. Der Build darf nicht stillschweigend mit einer unbestätigten, möglicherweise falschen Dateiauswahl fortgesetzt werden.

### 6.4 Line-Nummern nach Include-Einfügung

Die Include-Einfügung kann nachfolgende Sourcezeilen verschieben.

Deshalb MUSS die Bind-Planung in dieser Reihenfolge erfolgen:

1. fehlenden Include in einer In-Memory-Kopie einfügen,
2. diese endgültige geplante Sourcefassung erneut beziehungsweise weiter analysieren,
3. IDs und `li.json`-Zeilen anhand dieser Fassung bestimmen,
4. Sidecar-Makros mit diesen endgültigen Zeilen erzeugen,
5. Sourceänderung und Sidecar gemeinsam schreiben.

Es ist unzulässig, IDs anhand der alten Zeilennummern zu planen und anschließend durch die Include-Einfügung zu verschieben.

## 7. CLI-Spezifikation

### 7.1 Subkommando

```text
trice bind [Optionen]
```

Für das MVP ist kein Kurzalias erforderlich.

### 7.2 Beispiel

```sh
trice bind \
  -src . \
  -i til.json \
  -li li.json \
  -liRoot . \
  -IDMin 1000 \
  -IDMax 1999 \
  -IDMethod upward \
  -bindDir ./triceIDs
```

### 7.3 Wiederverwendete Optionen

Wo in dieser Spezifikation keine Abweichung genannt ist, übernimmt `bind` die Bedeutung der entsprechenden `insert`-Option.

Dazu gehören insbesondere:

- Sourceauswahl und Ausschlüsse,
- Aliase und Short-Aliase,
- `-i`, `-til`, `-idlist`,
- `-li`,
- `-liRoot`,
- `-IDMin`,
- `-IDMax`,
- `-IDRange`,
- `-IDMethod`,
- User Labels,
- `-dry-run`,
- Verbosity-Optionen,
- bestehende Prüf- und Skip-Optionen.

Die ID-Wiederverwendung, Kollisionsbehandlung, ID-Range-Policy, Aliasbehandlung und Aktualisierung von `til.json` und `li.json` MÜSSEN semantisch mit `trice insert` übereinstimmen.

### 7.4 Neue Optionen

#### `-bindDir`

```text
-bindDir <Verzeichnis>
```

Vorgeschlagener Default:

```text
./triceIDs
```

Bedeutung:

- Ausgabe aller Sidecar-Header,
- automatische Ausnahme dieses Verzeichnisses vom Source-Scan,
- keine automatische Änderung der Buildsystem-Include-Pfade.

#### `-insertIncludes`

```text
-insertIncludes=true|false
```

Vorgeschlagener Default:

```text
true
```

Bei `true` fügt `trice bind` fehlende markierte Includes gemäß Abschnitt 6 ein.

Bei `false` verändert das Kommando keine Userquelle. Ein fehlender Include führt zu einer Diagnose mit der einzufügenden Zeile.

### 7.5 Für `bind` nicht relevante `insert`-Optionen

Folgende `insert`-Optionen sollen im MVP für `bind` nicht registriert werden, weil sie ausschließlich die zurückgeschriebene instrumentierte Sourceformatierung oder den Insert-Cache betreffen:

- `-cache`,
- `-spaceInsideParenthesis`,
- `-w`,
- `-addParamCount`,
- `-defaultStampSize`.

Die Bind-Makros des MVP unterstützen die ID-freie lowercase-Schreibweise `trice(...)` ohne Target-Timestamp. Eine spätere Erweiterung weiterer Makrovarianten kann die Flag-Auswahl neu bewerten.

### 7.6 `-dry-run`

`-dry-run` MUSS alle geplanten Änderungen berechnen und melden, aber keine der folgenden Dateien ändern:

- Userquellen,
- Sidecar-Header,
- `til.json`,
- `li.json`.

Die Ausgabe SOLL mindestens nennen:

- neu einzufügende Includes,
- neu erzeugte File Keys,
- zu erzeugende oder zu ändernde Sidecars,
- neue beziehungsweise wiederverwendete Trice-IDs,
- Fehler und Warnungen.

## 8. Detaillierter Ablauf

### 8.1 Initialisierung

1. CLI-Optionen auswerten.
2. Sourcepfade wie bei `trice insert` normalisieren.
3. Alias- und ID-Range-Konfiguration wie bei `trice insert` aufbereiten.
4. `til.json` und `li.json` mit derselben Vorverarbeitung wie `trice insert` laden.
5. `bindDir` automatisch aus dem Source-Scan ausschließen.

### 8.2 Source-Vorprüfung

Vor der ID-Vergabe MUSS jede Datei analysiert werden auf:

- bindbare ID-freie lowercase-`trice(...)`-Aufrufe,
- bestehende markierte Sidecar-Includes,
- File Keys,
- doppelte File Keys,
- mehrere bindbare Aufrufe in derselben Sourcezeile,
- Trice-Aufrufe innerhalb von `#define`-Replacement-Listen,
- weitere ID-freie Trice-Makrovarianten, die das MVP nicht binden kann,
- statisch nicht unterstützte Formatstrings,
- mögliche Include-Positionen.

Fehler in dieser Phase MÜSSEN vor dem Schreiben von Sidecars oder JSON-Dateien gemeldet werden.

### 8.3 Geplante Sourcefassung

Für Dateien ohne markierten Include wird der Include zunächst ausschließlich in einer In-Memory-Kopie ergänzt.

Diese geplante Fassung ist anschließend die maßgebliche Source für:

- Zeilennummern,
- `li.json`,
- Sidecar-Makronamen,
- diagnostische Kommentare.

### 8.4 ID-Zuordnung

Die ID-Zuordnung MUSS die bestehende `trice insert`-Semantik verwenden.

Die erste Implementierung DARF dazu intern die vorhandene Insert-Funktionalität auf der geplanten In-Memory-Source ausführen:

```text
geplante Source
→ vorhandene Insert-Zuordnung
→ virtuell instrumentierte Source
```

Die virtuell instrumentierte Source wird nicht zurückgeschrieben.

Insbesondere darf `bind` nicht den Writer-Pfad `processTriceIDInsertion` verwenden, weil dieser:

- die Userdatei atomar zurückschreibt,
- `TRICE_CLEAN` in `triceConfig.h` umschaltet.

Stattdessen ist die eigentliche Zuordnungsfunktion `insertTriceIDs` beziehungsweise ein daraus extrahierter gemeinsamer Kern zu verwenden.

### 8.5 Erfassung der Zuordnungsergebnisse

Die empfohlene erste interne Refaktorierung ist ein optionaler Assignment-Collector:

```go
type assignedTrice struct {
    SourcePath string
    LIFile     string
    Line       int
    ID         TriceID
    Format     TriceFmt
}
```

Der bestehende Zuordnungsloop meldet jeden final bestimmten Datensatz an diesen Collector.

Konzeptionell:

```go
assignTriceIDs(..., collector)
```

- `insert` verwendet den Rückgabepuffer und schreibt ihn wie bisher.
- `bind` verwirft den instrumentierten Rückgabepuffer und verwendet nur die `assignedTrice`-Datensätze.

Dadurch bleibt die ID-Logik gemeinsam, ohne die virtuell erzeugte Source erneut parsen zu müssen.

Als sehr kurzfristiger Prototyp ist auch ein erneutes Parsen der virtuell instrumentierten Source möglich. Der Collector ist jedoch robuster und bleibt eine rein interne, nicht brechende Refaktorierung.

### 8.6 Explizite und andere Trice-Formen

Das MVP bindet nur ID-freie lowercase-`trice(...)`-Aufrufe.

Andere Trice-Formen dürfen nicht stillschweigend in Sidecars umgedeutet werden.

Es gilt:

- Eine andere ID-freie Trice-Form, die normalerweise `insert` benötigt, führt im Bind-MVP zu einer Diagnose.
- Bereits explizit mit einer ID versehene Trice-Formen können im Projekt verbleiben, sofern die vorhandene ID konsistent ist und der Target-Makrovertrag nicht durch `TRICE_BIND` verändert wird.
- Würde die gemeinsame Insert-Logik eine explizite ID korrigieren, muss `bind` abbrechen und auf `trice insert` beziehungsweise eine manuelle Korrektur verweisen.

### 8.7 Sidecar-Planung

Für jede Datei werden aus den Assignment-Datensätzen erzeugt:

- File Key,
- Sidecar-Dateiname,
- `TRICE_FILE_KEY`-Auswahl,
- eine Definition pro bindbarer Zeile,
- ein diagnostischer Formatstringkommentar.

Die Kommentare MÜSSEN auf eine einzelne Zeile normalisiert werden. Ein Kommentar darf nicht mit einem Backslash enden und dadurch eine Präprozessor-Zeilenfortsetzung auslösen.

### 8.8 Globale Validierung

Vor dem Schreiben MUSS mindestens geprüft werden:

- File Keys sind projektweit eindeutig.
- Sidecar-Dateinamen sind eindeutig.
- Eine Datei hat höchstens einen unterschiedlichen markierten Sidecar.
- Jede bindbare Stelle hat genau eine ID.
- Eine Sourcezeile enthält höchstens eine bindbare Stelle.
- Die geplante Include-Position ist gültig.
- `til.json`, `li.json` und die neue Zuordnung sind widerspruchsfrei.
- Es werden keine Dateien innerhalb von `bindDir` als Userquellen verarbeitet.

### 8.9 Schreiben

Alle Inhalte SOLLEN zunächst vollständig im Speicher beziehungsweise in temporären Dateien vorbereitet werden.

Danach:

1. `bindDir` anlegen, falls erforderlich.
2. geänderte Sidecars atomar schreiben.
3. geplante neue Include-Zeilen atomar in Userquellen schreiben.
4. `til.json` und `li.json` mit der bestehenden Post-Processing-Logik schreiben.

Eine Datei darf nur ersetzt werden, wenn sich ihr Inhalt tatsächlich ändert. Dadurch bleiben Zeitstempel und inkrementelle Builds stabil.

Ein Fehler in der Planungsphase darf keinerlei Dateiänderung verursachen. Tritt ein I/O-Fehler während der Commit-Phase auf, MUSS der Befehl fehlschlagen und die betroffenen Pfade nennen. Ein erneuter `trice bind`-Lauf muss den Zustand deterministisch reparieren können.

### 8.10 Veraltete Sidecars

Das MVP muss unbekannte Sidecars in `bindDir` nicht automatisch löschen. Sie sind Buildartefakte und können durch einen normalen Build-Clean entfernt werden.

Ein Sidecar, auf den eine markierte User-Include-Zeile verweist, MUSS jedoch bei jedem erfolgreichen `bind`-Lauf aktuell erzeugt werden.

## 9. Diagnosen und Rückgabestatus

### 9.1 Fehler

Mindestens folgende Situationen sind Fehler:

- doppelter File Key in unterschiedlichen Dateien,
- widersprüchlicher Sidecar-Name oder File Key,
- fehlender Sidecar-Include bei `-insertIncludes=false`,
- keine sichere Include-Position,
- Sidecar nach dem ersten bindbaren Trice-Aufruf,
- weitere textuelle Includes nach dem Sidecar,
- Trice-Aufruf in einer Makrodefinition,
- mehrere bindbare Trice-Aufrufe in derselben Sourcezeile,
- nicht unterstützte ID-freie Trice-Makrovariante,
- nicht statisch unterstützter Formatstring,
- widersprüchliche ID- oder Location-Zuordnung,
- nicht schreibbare Ausgabe- oder JSON-Dateien.

Fehlermeldungen MÜSSEN nach Möglichkeit enthalten:

```text
Datei:Zeile: Ursache
```

### 9.2 Warnungen

Mindestens folgende Situationen können Warnungen sein:

- identischer Sidecar mehrfach eingebunden,
- vorhandener Sidecar-Include ohne aktuelle bindbare Logstelle,
- heuristisch eingefügter Include, der vom User geprüft werden sollte,
- veraltete, nicht referenzierte Sidecars in `bindDir`.

### 9.3 Zusammenfassung

Ein erfolgreicher Lauf SOLL zusammenfassen:

- gescannte Dateien,
- bindbare Logstellen,
- wiederverwendete IDs,
- neu vergebene IDs,
- erzeugte beziehungsweise geänderte Sidecars,
- neu eingefügte Includes,
- Warnungen.

## 10. Geplanter Implementierungsumriss

### 10.1 CLI

Voraussichtliche Änderungen:

- `internal/args/vars.go`
  - neues `FlagSet` für `bind`,
  - Hilfetext.

- `internal/args/init.go`
  - `bindIDsInit()`,
  - gemeinsame Insert-Flags,
  - neue Flags `-bindDir` und `-insertIncludes`,
  - keine für `bind` wirkungslosen Insert-Formatierungsflags.

- `internal/args/handler.go`
  - neuer Fall `"bind"`,
  - dieselbe Vorbereitung von Sources, Aliasen, User Labels und ID-Ranges wie bei `insert`,
  - Aufruf von `id.SubCmdIdBind`.

### 10.2 Neues ID-Modul

Vorgeschlagene Datei:

```text
internal/id/bindIDs.go
```

Hauptbestandteile:

```go
func SubCmdIdBind(w io.Writer, fSys *afero.Afero) error
```

sowie interne Strukturen:

```go
type bindSite struct {
    Line   int
    ID     TriceID
    Format TriceFmt
}

type bindFilePlan struct {
    SourcePath     string
    LIFile         string
    FileKey        string
    SidecarName    string
    SidecarContent []byte
    SourceInput    []byte
    SourceOutput   []byte
    Sites          []bindSite
}
```

Zusätzliche interne Funktionen:

- Erkennen und Parsen markierter Includes,
- Erzeugen eines 64-Bit-File-Keys,
- Berechnen des Sidecar-Namens,
- Prüfen beziehungsweise Planen der Include-Position,
- Erkennen nicht unterstützter Konstruktionen,
- Rendern des Sidecars,
- globale Key- und Namensvalidierung,
- atomisches Schreiben nur bei Inhaltsänderung.

### 10.3 Wiederverwendung der Insert-Logik

Der heutige Ablauf

```text
SubCmdIdInsert
→ cmdSwitchTriceIDs
→ processTriceIDInsertion
→ insertTriceIDs
```

soll für `bind` nicht vollständig kopiert werden.

Wiederverwendet werden:

- Pre-Processing von `til.json`, `li.json` und ID-Ranges,
- Sourceerkennung,
- Trice-Parser und Matching,
- Wiederverwendung bestehender IDs,
- Vergabe neuer IDs,
- Konfliktbehandlung,
- Post-Processing der JSON-Listen.

Nicht verwendet werden:

- Zurückschreiben der virtuell instrumentierten Source,
- Umschalten von `TRICE_CLEAN`,
- Insert-Cache,
- Insert-spezifische Sourceformatierung.

Die vorzugsweise interne Refaktorierung trennt daher:

```text
ID-Zuordnung
```

von:

```text
Darstellung und Schreiben der instrumentierten Source
```

Diese Refaktorierung ändert keine öffentliche API und keine CLI-Semantik.

### 10.4 Reihenfolge und Parallelität

Für die erste Implementierung ist eine einfache, deterministische Verarbeitung wichtiger als maximale Geschwindigkeit.

Sie darf die Dateien seriell verarbeiten oder dieselbe Synchronisation wie `insert` verwenden. Eine spätere Optimierung kann die Planung parallelisieren, sofern:

- die ID-Vergabe reproduzierbar bleibt,
- `IDMethod` unverändert respektiert wird,
- die Ergebnisse mit der Insert-Semantik übereinstimmen.

### 10.5 Target-Library

Der aktuelle PoC zeigt die erforderliche, bedingte Erweiterung in `triceOn.h`:

```c
#define TRICE_BIND_ID_I(fileKey, line) TRICE_ID_##fileKey##_L##line
#define TRICE_BIND_ID(fileKey, line) TRICE_BIND_ID_I(fileKey, line)
#define TRICE_BIND_ID_HERE() TRICE_BIND_ID(TRICE_FILE_KEY, __LINE__)

#define trice(...) \
    TRICE_CONCAT2(trice_, TRICE_COUNT_VALUE_ARGUMENTS(__VA_ARGS__))( \
        iD(TRICE_BIND_ID_HERE()), __VA_ARGS__)
```

Diese Logik ist nur aktiv bei:

```c
#define TRICE_BIND 1
```

Außerhalb dieses Modus bleibt der bisherige `trice(tid, ...)`-Vertrag unverändert.

`trice bind` selbst ändert `triceConfig.h` nicht automatisch und schaltet `TRICE_CLEAN` nicht um. Die bereits im PoC vorhandene Librarybehandlung aktiviert den Backendpfad im Bind-Modus.

## 11. Tests

### 11.1 Unit-Tests

Erforderlich sind mindestens Tests für:

- File-Key-Erzeugung mit gültigem C-Identifier,
- Wiederverwendung eines bestehenden File Keys,
- Erkennung eines kopierten doppelten Keys,
- Sidecar-Namensbildung,
- deterministisches Sidecar-Rendering,
- diagnostische Kommentarbereinigung,
- keine Änderung identischer Sidecars,
- Einfügen des Includes in `.c`,
- Einfügen innerhalb eines Header-Guards,
- Wiederverwendung eines vorhandenen Includes,
- ungültige Include-Reihenfolge,
- doppelte identische Includes als Warnung,
- mehrere Trice-Aufrufe pro Zeile als Fehler,
- Trice in Makrodefinition als Fehler,
- `static inline` in Headern,
- inaktive Präprozessorzweige,
- zwei gleichnamige Dateien in verschiedenen Verzeichnissen,
- identische Formatstrings an unterschiedlichen Stellen,
- `-insertIncludes=false`,
- `-dry-run`,
- Fehler ohne partielle Writes.

### 11.2 Parität zu `insert`

Für identische saubere Quellen, Listen und ID-Optionen MUSS ein Paritätstest prüfen:

1. `trice insert` auf einer Kopie,
2. `trice bind` auf einer zweiten Kopie,
3. Vergleich der zugeordneten IDs in `til.json` und `li.json`,
4. Vergleich der IDs in den instrumentierten Aufrufen mit den Sidecar-Definitionen.

Die Darstellungsform ist verschieden; die fachliche ID-Zuordnung muss gleich sein.

### 11.3 Integrationstest

`examples/PoC_bind_v2` SOLL zum Integrationstest weiterentwickelt werden.

Ein Test aus einem sauberen Zustand MUSS:

1. vorhandene simulierte Sidecars löschen,
2. `trice bind` ausführen,
3. das Beispiel gegen die echte Target-Library bauen,
4. das Programm ausführen,
5. `log.bin` mit den erzeugten `til.json` und `li.json` decodieren,
6. die erwarteten Meldungen und IDs prüfen.

Der Test MUSS weiterhin enthalten:

- mehrere gleichnamige Dateien in verschiedenen Verzeichnissen,
- direkte Trice-Aufrufe in `.c`,
- `static inline`-Aufrufe in Headern,
- identische Formatstrings an unterschiedlichen Logstellen,
- Wiederherstellung des File Keys nach Header-Includes.

### 11.4 Regression

Alle bestehenden Tests für:

- `insert`,
- `clean`,
- ID-Listen,
- Target-Makros ohne `TRICE_BIND`

MÜSSEN unverändert erfolgreich bleiben.

## 12. Abnahmekriterien des MVP

Das MVP gilt als erfüllt, wenn:

1. `trice bind` als neues Subkommando verfügbar ist.
2. Der bestehende `insert`-Allocator für ID-Wiederverwendung und -Vergabe genutzt wird.
3. Die Userquellen keine numerischen IDs in den bindbaren `trice(...)`-Aufrufen enthalten.
4. Ausschließlich die Originalquellen kompiliert werden.
5. Pro Datei ein eindeutiger temporärer Sidecar erzeugt wird.
6. File Keys und Includes automatisch verwaltet und validiert werden.
7. `til.json` und `li.json` mit der `insert`-Semantik aktualisiert werden.
8. Ein zweiter unveränderter `bind`-Lauf keine Dateizeitstempel unnötig ändert.
9. Das PoC-v2-Beispiel nach einem frischen Sidecar-Clean automatisch gebaut und decodiert werden kann.
10. `insert` und `clean` keinerlei Verhaltensänderung zeigen.
11. Keine Änderung des JSON-Schemas und keine Änderung des Trice-Drahtformats erforderlich ist.
12. Nicht unterstützte Konstruktionen zuverlässig und verständlich abgelehnt werden.

## 13. Spätere interne Optimierung

Die erste Implementierung darf zur Risikoreduktion die bestehende Insert-Zuordnung auf einer In-Memory-Kopie ausführen. Das ist nicht laufzeitoptimal, aber einfach zu verifizieren.

Später kann der gemeinsame Parser direkt einen neutralen Bind-Plan erzeugen:

```text
Source
→ []assignedTrice
→ Insert-Renderer oder Sidecar-Renderer
```

Diese Optimierung darf das spezifizierte Verhalten nicht ändern. Sie ist eine interne Implementierungsverbesserung und kein neues Feature.
