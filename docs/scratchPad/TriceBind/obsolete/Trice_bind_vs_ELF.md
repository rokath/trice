# Trice Bind als bevorzugter Mechanismus zur ID-Bindung

## 1. Zweck und Ausgangslage

Trice verwendet stabile numerische IDs, um Formatstrings und Location Information vom Targetcode zu entkoppeln. Die dauerhafte Zuordnung wird in `til.json` und `li.json` gespeichert. Beide Dateien gehören zum Projekt und damit in die Versionsverwaltung.

Der bisherige Workflow bringt die ID mit `trice insert` direkt in den Usercode und entfernt sie bei Bedarf wieder mit `trice clean`:

```text
trice insert
Build
trice clean
```

Das ist funktional zuverlässig, verändert aber temporär die Quelldateien. Das Ziel von `trice bind` ist deshalb klar begrenzt:

> Die stabile ID-Zuordnung bleibt unverändert erhalten. Lediglich die Einbringung der ID in den Compilerlauf wird aus dem Usercode in ein generiertes Buildartefakt verlagert.

Der Usercode enthält weiterhin nur die ID-lose Logstelle:

```c
trice("msg:Hello trice bind world\n");
```

Die zugehörige ID steht dauerhaft in `til.json` und `li.json`, während `trice bind` daraus vor dem Build einen temporären, dateispezifischen Header erzeugt:

```text
module.c → build/module.c.triceIDs.h
```

Der Header enthält ausschließlich Compile-Time-Zuordnungen, beispielsweise:

```c
#define TRICE_ID_L9 1001u
```

Die Trice-Makrologik setzt die zur aktuellen Sourcezeile gehörende ID ein. Der Compiler verarbeitet weiterhin unmittelbar die Originalquelle. Es werden keine generierten C- oder C++-Schattenquellen gebaut.

## 2. Architektur von `trice bind`

### 2.1 Dauerhafte Daten

Dauerhaft und versionsverwaltet sind:

- Userquellen,
- `til.json`,
- `li.json`,
- Trice-Konfiguration,
- Buildskripte beziehungsweise Buildsystemdateien.

`til.json` enthält die stabile Beziehung zwischen ID, Trice-Typ und Formatstring. `li.json` enthält die zugehörige Location Information. Diese Dateien bilden die persistente ID-Datenbank des Projekts.

### 2.2 Temporäre Daten

`trice bind` erzeugt pro Quelldatei einen temporären Header, beispielsweise:

```text
build/main.c.triceIDs.h
build/module.c.triceIDs.h
```

Diese Dateien:

- werden vor dem Kompilieren erzeugt,
- enthalten ausschließlich bereits bekannte beziehungsweise neu zugeordnete IDs,
- sind während Build und Debugging verfügbar,
- werden normalerweise nicht eingecheckt,
- sind nach dem Build reproduzierbar und daher entbehrlich.

Die Sidecar-Dateien sind damit normale generierte Buildartefakte, vergleichbar mit automatisch erzeugten Konfigurations- oder Protokollheadern.

### 2.3 Bindung an die Logstelle

Die minimale Bindung verwendet die Quelldatei und die Sourcezeile als buildlokale Adresse einer Logstelle. Der generierte Header enthält beispielsweise:

```c
#define TRICE_ID_L9 1001u
#define TRICE_ID_L15 1002u
```

Die allgemeine Trice-Makrologik bildet `__LINE__` auf den entsprechenden Makronamen ab. Der Compiler sieht dadurch letztlich eine normale Konstante:

```c
trice32fn_0((uint16_t)1001u);
```

Wesentliche Eigenschaften:

- keine Runtime-Suche,
- keine Mappingtabelle im Target,
- kein zusätzlicher Tabellenzugriff,
- keine Änderung des Trice-Drahtformats,
- keine Abhängigkeit von einer bestimmten Objektdatei oder einem bestimmten ELF zur späteren Decodierung.

Mehrere direkte Trice-Aufrufe in derselben physischen Sourcezeile können zunächst als nicht unterstützter Stil behandelt werden. Eine spätere optionale Erweiterung kann verfügbare Mechanismen wie `__COUNTER__` nutzen. Diese Erweiterung ist für das Grundkonzept nicht erforderlich.

### 2.4 Wrappermakros

Für Wrappermakros bleibt die bisherige `trice insert`-Semantik erhalten:

```c
#define LOG_ERROR(x) trice("error=%d\n", x)
```

Die Trice-Logstelle erhält genau eine stabile ID an der Definitionsstelle des Wrappermakros. Alle Aufrufstellen verwenden dieselbe ID. Die Location Information verweist ebenfalls auf die Definitionsstelle.

Aufrufstellenabhängige IDs sind nicht Bestandteil des Grundkonzepts. Sie könnten später optional ergänzt werden, sind aber weder für Kompatibilität noch für die normale Trice-Arbeitsweise erforderlich.

### 2.5 Verbindliche Prüfung der Include-Reihenfolge

Die korrekte Position des dateispezifischen ID-Headers ist Teil des `trice bind`-Vertrags. Sie darf nicht erst durch zufällige Makrokollisionen oder schwer verständliche Compilerfehler auffallen. `trice bind` prüft deshalb die Include-Struktur jeder direkt kompilierten Quelldatei statisch und bricht den Build bei einer Verletzung mit Datei- und Zeilenangabe ab.

Für eine Quelldatei wie `module.c` gelten mindestens folgende Regeln:

1. Der zugehörige Header `module.c.triceIDs.h` wird genau einmal eingebunden.
2. Er wird nach `trice.h` beziehungsweise nach dem Header eingebunden, der die Trice-Makros bereitstellt.
3. Er steht nach allen übrigen Includes der Translation Unit.
4. Nach dem dateispezifischen ID-Header folgen keine weiteren `#include`-Direktiven.
5. Der eingebundene Sidecar muss anhand des normalisierten Quellpfads eindeutig zur Quelldatei gehören. Fehlende, doppelte oder fremde Sidecars sind Fehler.

Die beabsichtigte Struktur ist damit beispielsweise:

```c
#include "project.h"
#include "driver.h"
#include "trice.h"
#include "module.c.triceIDs.h"

void moduleInit(void)
{
    trice("msg:module initialized\n");
}
```

Diese Prüfung gehört zur normalen Bind-Phase und ist keine optionale Zusatzanalyse. Ein zufälliges `TRICE_UNIQUE`-Makro in den generierten Headern wäre dafür kein gleichwertiger Ersatz: Es könnte legitime Mehrfachdefinitionen erzeugen, würde nicht jede falsche Reihenfolge zuverlässig erkennen und lieferte weniger präzise Diagnosen. Deterministische Kennungen können ergänzend zur Erkennung veralteter oder falsch zugeordneter Buildartefakte verwendet werden; die semantische Include-Prüfung bleibt dennoch maßgeblich.

## 3. Build- und Debugverhalten

Der Compiler übersetzt die Originalquellen. Die generierten Header liefern lediglich Konstanten. Dadurch bleiben erhalten:

- normale Sourcepfade in Compilerdiagnosen,
- normale Debuggerzuordnung,
- normale Breakpoints,
- normale Include-Abhängigkeiten,
- inkrementelle Builds auf Ebene der jeweiligen Translation Unit.

Ändert sich nur `module.c.triceIDs.h`, muss lediglich die zugehörige Translation Unit neu gebaut werden. Der Generator sollte eine Datei nur dann ersetzen, wenn sich ihr Inhalt tatsächlich geändert hat. Dadurch werden unnötige Neubauten vermieden.

Ein typischer Ablauf ist:

```text
trice bind
    → Logstellen erfassen und IDs zuordnen
    → dateispezifische ID-Header erzeugen
    → Include-Reihenfolge und Sidecar-Zuordnung validieren
Build
```

`trice clean` ist in diesem Workflow nicht erforderlich, da der Usercode nicht instrumentiert wird. Ein fehlender, falsch positionierter oder nicht zur Quelldatei gehörender Sidecar führt bereits in `trice bind` zu einem eindeutigen Fehler.

## 4. Bewertung eines ELF-basierten Bindings

Ein ELF-basierter Ansatz würde Trice-Metadaten und ID-Platzhalter in besonderen Objektsektionen speichern. Ein Werkzeug könnte diese Informationen nach der Compilation auswerten und IDs beim Linken oder durch Relocations binden.

Technisch ist das möglich. Für normalen verfügbaren C- oder C++-Sourcecode entsteht daraus jedoch kein wesentlicher funktionaler Vorteil gegenüber `trice bind` mit generierten ID-Headern.

### 4.1 Höhere technische Komplexität

Ein ELF-Binder muss mindestens berücksichtigen:

- ELF-Sektionsformate,
- Symboltabellen,
- Relocation-Typen,
- Zielarchitektur und ABI,
- Linkerverhalten,
- Section Garbage Collection,
- Link-Time Optimization,
- Archive und deren Objektmitglieder,
- Unterschiede zwischen GCC-, Clang-, IAR-, Keil- und weiteren Toolchains.

Der Sidecar-Ansatz arbeitet dagegen oberhalb dieser Ebene. Er erzeugt C-Präprozessorkonstanten und überlässt dem vorhandenen Compiler die normale Codeerzeugung.

### 4.2 Kein Vorteil bei der späteren Decodierung

Beim Sidecar-Modell werden stabile Trice-IDs in das Image kompiliert. Historische Logs werden wie bisher mit der kumulativen `til.json` und `li.json` decodiert.

Ein buildspezifisches ELF ist dafür nicht erforderlich.

Ein ELF-Ansatz wäre nur dann gleichwertig, wenn er ebenfalls endgültige stabile IDs in den Targetcode bindet. Bleiben die IDs dagegen vom konkreten Linkerlayout abhängig, entsteht die zusätzliche Pflicht, das zum Image passende ELF oder Dictionary dauerhaft zu archivieren. Das wäre gegenüber dem Sidecar-Modell ein Nachteil.

## 5. Prüfung der scheinbaren ELF-Vorteile

Mehrere Eigenschaften wirken zunächst wie Vorteile des ELF-Ansatzes. Bei genauer Betrachtung erfordern sie jedoch keine Änderung des grundlegenden `trice bind`-Mechanismus. Sie können bei Bedarf durch nachträgliche, optionale Erweiterungen ergänzt werden.

### 5.1 Vorkompilierte `.a`-Libraries

Eine vorbereitete statische Library kann Trice-Metadaten und offene ID-Platzhalter enthalten. Ein späteres `trice bind` könnte:

1. die Objektmitglieder der Library untersuchen,
2. bisher nicht verwendete beziehungsweise neue stabile IDs bestimmen,
3. `til.json` und `li.json` ergänzen,
4. die IDs beim finalen Link binden.

Als Location Information kann beispielsweise verwendet werden:

```text
libfoo.a(foo.o):site-17
```

Wichtig ist: Auch ein ELF-Ansatz benötigt eine vorbereitete Library. Eine beliebige fertige `.a` enthält weder die erforderlichen Trice-Metadaten noch eindeutig bindbare ID-Platzhalter.

Die Library-Unterstützung ist daher eine mögliche Erweiterung von `trice bind`, kein Grund, den normalen Sourcecode-Weg auf ELF umzustellen.

### 5.2 Erfassung aktiver und inaktiver Logstellen

Ein Source-Scan kann bewusst alle Trice-Logstellen erfassen, auch solche in aktuell inaktiven Präprozessorzweigen:

```c
#if FEATURE_X
trice("feature X\n");
#endif
```

Das ist für stabile IDs vorteilhaft. Eine Logstelle behält ihre ID auch dann, wenn eine bestimmte Buildkonfiguration sie vorübergehend deaktiviert.

Falls zusätzlich bekannt sein soll, welche Logstellen in einer konkreten Konfiguration aktiv sind, kann `trice bind` optional den realen Präprozessor mit den tatsächlichen Defines und Include-Pfaden ausführen.

Damit entstehen zwei getrennte Sichten:

```text
Source-Scan       → alle bekannten Logstellen
Präprozessor-Scan → in dieser Konfiguration aktive Logstellen
```

Dafür ist keine Änderung auf ein ELF-basiertes Binding erforderlich.

### 5.3 Tatsächlich im finalen Image enthaltene Logstellen

Ein Präprozessorlauf allein kann nicht sicher bestimmen, welche aktiven Logstellen nach Optimierung und Linken im Image verbleiben. Entfernt werden können beispielsweise:

- unbenutzte Funktionen,
- unreferenzierte Sections,
- nicht ausgewählte Objektmodule aus Archiven,
- durch LTO eliminierter Code.

Falls eine exakte Image-Inventur erforderlich ist, kann sie als optionale Post-Link-Prüfung ergänzt werden:

```text
Source-Scan       → alle bekannten Logstellen
Präprozessor-Scan → aktive Logstellen
ELF-/Map-Analyse  → tatsächlich im Image enthaltene Logstellen
```

Diese Analyse nutzt das ELF oder die Link-Map ausschließlich zur Inventur. Die ID-Vergabe und ID-Bindung bleiben unverändert. Ein solcher Post-Link-Schritt erfordert daher keine Umstellung des Hauptmechanismus.

### 5.4 Komplexe Makroexpansionen

Trice geht von statischen, direkt erkennbaren Formatstrings aus. Präprozessorbasierte Stringverkettung beziehungsweise die Erzeugung verschiedener Formatstrings aus derselben Makrodefinition ist nicht vorgesehen.

Unterschiedliche dynamische Inhalte werden als Parameter übertragen, beispielsweise:

```c
#define DEVICE(x) triceS("initialize %s\n", #x)
```

Die Logstelle bleibt an der Makrodefinition eindeutig. Sie erhält eine stabile ID; der konkrete Name wird als Parameter übertragen.

Damit entfällt auch hier die Notwendigkeit, einzelne Makroexpansionen über ELF als unterschiedliche Logstellen zu behandeln.

### 5.5 Nur eine Include-Zeile

Ein ELF-Ansatz könnte vermeiden, dass eine Quelldatei neben `trice.h` noch ihren generierten ID-Header einbindet.

Dies ist der verbleibende direkte Komfortvorteil:

```c
#include "trice.h"
#include "module.c.triceIDs.h"
```

gegenüber:

```c
#include "trice.h"
```

Die zusätzliche Include-Zeile ist jedoch explizit, leicht verständlich, compilerneutral und buildsystemneutral. Sie rechtfertigt nicht die zusätzliche Komplexität eines ELF-, Symbol- und Relocation-basierten Bindings.

## 6. Erweiterbarkeit ohne Architekturwechsel

Ein wesentlicher Vorteil des Sidecar-Modells ist, dass spätere Anforderungen als additive Erweiterungen umgesetzt werden können:

- optionaler Präprozessorlauf für die aktive Konfiguration,
- optionale Post-Link-Inventur anhand von ELF oder Link-Map,
- optionale Unterstützung vorbereiteter `.a`-Libraries,
- optionaler `__COUNTER__`-Modus für mehrere Logstellen pro Zeile,
- weitere optionale Validierungen für Wrappermakros und projektspezifische Konventionen,
- Buildsystemintegration oder automatische Generatoraufrufe.

Keine dieser Erweiterungen verlangt eine Änderung der grundlegenden ID-Verwaltung oder des bestehenden Trice-Drahtformats.

Insbesondere gilt:

> ELF kann als Eingabeformat für spezielle Erweiterungen oder als Quelle einer Post-Link-Analyse verwendet werden, ohne dass ELF zum zentralen Binding-Mechanismus wird.

Damit bleiben der normale Buildpfad einfach und portabel, während anspruchsvollere Spezialfälle bei Bedarf ergänzt werden können.

## 7. Vergleich

| Kriterium | `trice bind` mit ID-Headern | Zentrales ELF-Binding |
|---|---|---|
| IDs im Usercode sichtbar | nein | nein |
| Originalquellen werden kompiliert | ja | ja |
| Stabile IDs auf dem Draht | ja | möglich, aber zusätzlich zu implementieren |
| `til.json`/`li.json` bleiben maßgeblich | ja | nur bei entsprechender Auslegung |
| Laufzeit-Mapping erforderlich | nein | nein oder optional, abhängig vom Design |
| Zusätzlicher Runtimeaufwand | nein | nicht zwingend |
| Compilerunabhängiger Kern | weitgehend ja | nein |
| Linker-/ABI-Abhängigkeit | nein | ja |
| LTO-/Relocation-Komplexität | nein | ja |
| Inaktive Logstellen erfassbar | ja | nicht automatisch |
| Aktive Konfiguration bestimmbar | optionale Präprozessorerweiterung | teilweise |
| Exakte Image-Inventur | optionale Post-Link-Erweiterung | möglich |
| Vorbereitete `.a`-Libraries | optionale Erweiterung | mögliche Spezialanwendung |
| Buildspezifisches ELF zur Decodierung nötig | nein | nur vermeidbar, wenn stabile IDs endgültig gebunden werden |
| Zusätzliche Include-Zeile | ja | nein |

## 8. Architekturentscheidung

Für normalen verfügbaren C- und C++-Sourcecode ist `trice bind` mit generierten, dateispezifischen ID-Headern der bevorzugte Mechanismus.

Die Entscheidung beruht auf folgenden technischen Eigenschaften:

1. Die dauerhafte ID-Wahrheit bleibt in `til.json` und `li.json`.
2. Der Usercode enthält keine numerischen IDs.
3. Die Originalquellen werden direkt kompiliert.
4. Die IDs gelangen als Compile-Time-Konstanten in den Targetcode.
5. Es entsteht kein zusätzlicher Runtimeaufwand.
6. Das Trice-Drahtformat bleibt unverändert.
7. Historische Logs bleiben ohne buildspezifisches ELF decodierbar.
8. `trice bind` validiert die erforderliche Include-Reihenfolge und die eindeutige Zuordnung jedes Sidecars vor der Compilation.
9. Der Mechanismus ist weitgehend unabhängig von Compiler, Linker, ABI und Objektformat.
10. Scheinbare ELF-Vorteile können bei Bedarf durch additive Präprozessor-, Library- oder Post-Link-Erweiterungen erreicht werden.
11. Der einzige verbleibende allgemeine ELF-Komfortvorteil ist der Wegfall einer zusätzlichen Include-Zeile.

## 9. Schlussfolgerung

`trice bind` ersetzt nicht die bestehende stabile ID-Verwaltung. Es ersetzt ausschließlich das Sourcecode-Patching als Transportmechanismus der ID zum Compiler.

Die resultierende Aufgabentrennung ist klar:

```text
til.json / li.json
    → persistente stabile ID-Zuordnung

trice bind
    → Erzeugung temporärer dateispezifischer ID-Header
    → Prüfung von Include-Reihenfolge und Sidecar-Zuordnung

Compiler
    → Einbau der IDs als Konstanten in das Image

tlog
    → Decodierung anhand der kumulativen til.json und li.json
```

Ein zentrales ELF-Binding würde für den normalen Anwendungsfall erheblich mehr Toolchainkomplexität erzeugen, ohne einen entsprechenden funktionalen Mehrwert zu liefern.

ELF bleibt dennoch nutzbar:

- als Grundlage einer späteren `.a`-Library-Erweiterung,
- als Quelle einer optionalen Post-Link-Inventur,
- als Diagnoseformat für spezielle Toolchains.

Diese Funktionen ergänzen `trice bind`; sie ersetzen oder verändern den bevorzugten Sidecar-Mechanismus nicht.
