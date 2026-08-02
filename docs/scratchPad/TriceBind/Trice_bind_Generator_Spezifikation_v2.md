# Spezifikation: `trice bind`

**Version:** 2  
**Status:** Spezifikationsentwurf für das MVP  
**Repository-Bezug:** Branch `wip`, insbesondere `examples/PoC_bind_v2`, `internal/id` und `_test/testdata/triceCheck.c`  
**Zielgruppe:** Implementierung, Review, Tests und spätere Übernahme in die Trice-Dokumentation

## 1. Zweck und Grundidee

`trice bind` ist ein neues Subkommando und eine Alternative zum bisherigen Ablauf:

```text
trice insert
Build
trice clean
```

Es verwendet dieselbe stabile ID-Verwaltung wie `trice insert`, schreibt die zugeordneten IDs jedoch nicht in die Trice-Aufrufe der Userquellen. Stattdessen erzeugt es pro Source- oder Headerdatei einen temporären Sidecar-Header mit Präprozessordefinitionen.

Der Usercode bleibt beispielsweise:

```c
trice("msg:Hello trice bind world\n");
```

Die dauerhafte Zuordnung bleibt in:

- `til.json`,
- `li.json`,
- der markierten Sidecar-Include-Zeile der jeweiligen Userdatei.

Der Compiler übersetzt die Userquellen unmittelbar. Der Sidecar liefert die zugehörige ID als Compile-Time-Konstante.

### 1.1 Grundmechanik in Kurzform

Ein Sidecar enthält beispielsweise:

```c
#define TRICE_FILE_KEY K73A915E9C4021B8
#define TRICE_ID_K73A915E9C4021B8_L9 12345u // trice("msg:Hello trice bind world\n")
```

Die Target-Library bildet den aktiven File Key und `__LINE__` auf diesen Makronamen ab:

```c
#define TRICE_BIND_ID_I(key, line) TRICE_ID_##key##_L##line
#define TRICE_BIND_ID(key, line)   TRICE_BIND_ID_I(key, line)
#define TRICE_BIND_ID_HERE()       TRICE_BIND_ID(TRICE_FILE_KEY, __LINE__)
```

An Sourcezeile 9 expandiert der Präprozessor schrittweise:

```text
TRICE_BIND_ID_HERE()
→ TRICE_BIND_ID(K73A915E9C4021B8, 9)
→ TRICE_ID_K73A915E9C4021B8_L9
→ 12345u
```

Die Makroschicht fügt die Konstante anschließend mit der zur Trice-Schreibweise passenden ID- beziehungsweise Stamp-Form in den bestehenden Targetpfad ein. Der Compiler sieht damit denselben fachlichen ID-Wert wie nach `trice insert`.

Ein vollständiges Beispiel einschließlich ID-freier, `id(0)`-/`Id(0)`-/`ID(0)`- und expliziter Aufrufe steht in [Anhang C](#anhang-c-präprozessorauflösung-und-site-deskriptoren).

## 2. Normative Begriffe

In diesem Dokument bedeuten:

- **MUSS:** verbindliche Anforderung des MVP,
- **SOLL:** erwartetes Verhalten, von dem nur mit dokumentiertem Grund abgewichen werden darf,
- **KANN:** optionale Eigenschaft.

## 3. Kompatibilität und Target-Konfiguration

### 3.1 Keine beabsichtigten Breaking Changes

Das MVP ist als additive Erweiterung spezifiziert.

Insbesondere gilt:

- `trice insert` bleibt unverändert verfügbar.
- `trice clean` bleibt unverändert verfügbar.
- Bestehende CLI-Aufrufe behalten ihre Bedeutung.
- Bestehende Projekte mit `TRICE_CLEAN 0` oder `TRICE_CLEAN 1` bauen unverändert.
- Das Schema von `til.json` und `li.json` wird nicht geändert.
- Das Trice-Drahtformat wird nicht geändert.
- Explizite IDs behalten ihre Bedeutung.
- Ohne aktivierten Bind-Modus gelten die bisherigen Target-Makroverträge unverändert.

Falls sich während der Implementierung eine Abweichung davon als unvermeidbar erweist, MUSS sie vor dem Merge ausdrücklich als Breaking Change dokumentiert, begründet und mit einer Migrationsanleitung versehen werden. Nach aktuellem Stand ist dies nicht erforderlich.

### 3.2 Einheitlicher `TRICE_MODE`

Als langfristige Konfiguration wird ein einzelner Modusschalter vorgesehen:

```c
#define TRICE_MODE TRICE_MODE_BIND
```

Vorgeschlagene Modi:

```c
#define TRICE_MODE_INSERTED 0
#define TRICE_MODE_CLEAN    1
#define TRICE_MODE_BIND     2
```

Bedeutung:

- `TRICE_MODE_INSERTED`: bisheriger aktiver Zustand mit expliziten IDs im Source,
- `TRICE_MODE_CLEAN`: bisheriger bereinigter beziehungsweise abgeschalteter Zustand,
- `TRICE_MODE_BIND`: aktiver Zustand mit IDs aus Sidecars.

`TRICE_CLEAN` bleibt für bestehende Projekte kompatibel. Ist `TRICE_MODE` nicht definiert, wird der Modus intern aus `TRICE_CLEAN` abgeleitet:

```c
#ifndef TRICE_MODE
    #if defined(TRICE_CLEAN) && (TRICE_CLEAN == 1)
        #define TRICE_MODE TRICE_MODE_CLEAN
    #else
        #define TRICE_MODE TRICE_MODE_INSERTED
    #endif
#endif
```

Ist `TRICE_MODE` explizit definiert, hat es Vorrang. Die Target-Implementierung KANN für bestehende interne Prüfungen weiterhin einen daraus abgeleiteten booleschen `TRICE_CLEAN`-Wert bereitstellen.

Der derzeitige PoC-Schalter `TRICE_BIND` ist nicht als zusätzliche dauerhafte öffentliche Konfiguration vorgesehen. Seine Funktion geht in `TRICE_MODE_BIND` auf.

`TRICE_OFF == 1` hat weiterhin Vorrang und schaltet Trice unabhängig vom gewählten Modus ab.

### 3.3 Bewusste einmalige Sourceänderung

`trice bind` KANN einmalig eine markierte Sidecar-Include-Zeile in eine Userdatei einfügen:

```c
#include "trice_module_c_K73A915E9C4021B8.h" // trice-bind
```

Diese Zeile wird versioniert. Sie speichert die stabile Dateiidentität und wählt den passenden Sidecar.

Normale spätere Bind-Läufe verändern die Userquelle nicht, solange die Include-Zeile vorhanden und konsistent ist.

Mit `-dry-run` darf keine Userdatei verändert werden.

## 4. Umfang des MVP

### 4.1 Unterstützte User-Level-Makros

Das MVP SOLL dieselben eingebauten User-Level-Trice-Makros erkennen und binden, die der bestehende `trice insert`-Parser unterstützt und für die ein Target-Makro vorhanden ist.

Dazu gehören insbesondere:

- die generischen Familien `trice`, `Trice`, `TRice` und `TRICE`,
- die Bitbreitenfamilien mit `8`, `16`, `32` und bei aktivierter 64-Bit-Unterstützung `64`,
- variadische und explizit aritätscodierte Formen wie `Trice16(...)`, `TRice16_2(...)` und `TRICE8_3(...)`,
- die Formen `trice0`, `trice_0` und ihre Case-Varianten,
- die eingebauten Spezialfamilien für Strings, gezählte Daten, Buffer, Remote-Function-Call beziehungsweise ABC und Assertions, soweit sie heute vom Insert-Parser und der Target-Library unterstützt werden,
- registrierte `-alias`- und `-salias`-Namen gemäß Abschnitt 4.4.

Die konkrete Vollständigkeitsreferenz ist nicht eine handgeschriebene Namensliste, sondern:

1. das vom bestehenden Insert-Parser erzeugte Makromuster,
2. die User-Level-Makros der Target-Library,
3. `_test/testdata/triceCheck.c`.

Neue eingebaute User-Level-Makros sollen künftig nur dann als vollständig integriert gelten, wenn auch ihr Bind-Verhalten festgelegt und getestet ist.

### 4.2 Drei ID-Zustände

Für jede erkannte Trice-Stelle unterscheidet `trice bind` drei Fälle.

#### A. ID-freier Aufruf

```c
trice("msg:hello\n");
TRice16_2("msg:x=%d y=%d\n", x, y);
TRICE8_3("msg:%d %d %d\n", a, b, c);
```

`trice bind` bestimmt eine stabile ID und der Sidecar stellt den vollständigen zur Stelle gehörenden ID-Ausdruck bereit.

Für die normalen nicht ausschließlich großgeschriebenen Trice-Familien wird `iD(...)` verwendet. Die Stamp-Bitbreite ergibt sich weiterhin aus der vorhandenen Makroschreibweise.

Für ein ID-freies ausschließlich großgeschriebenes Makro bestimmt `-defaultStampSize` die Form:

- `0` → `id(...)`,
- `16` → `Id(...)`,
- `32` → `ID(...)`.

#### B. Platzhalter-ID 0

```c
TRICE8_3(id(0), "msg:%d %d %d\n", a, b, c);
TRICE8_3(Id(0), "msg:%d %d %d\n", a, b, c);
TRICE8_3(ID(0), "msg:%d %d %d\n", a, b, c);
```

Die Schreibweise `id`, `Id` beziehungsweise `ID` MUSS erhalten bleiben, weil sie unterschiedliche Targetstamp-Aktionen auslöst. Die Null wird durch die zugeordnete stabile ID ersetzt.

Ein expliziter Platzhalter hat Vorrang vor `-defaultStampSize`.

#### C. Explizite ID größer null

```c
trice(iD(123), "msg:fixed\n");
TRICE8_3(Id(456), "msg:%d %d %d\n", a, b, c);
```

Der Aufruf bleibt unverändert. `trice bind` MUSS ihn jedoch mit derselben Semantik wie `trice insert` gegen `til.json`, `li.json`, den Formatstring und bestehende ID-Belegungen validieren.

Würde `trice insert` die explizite ID wegen einer Kollision oder Inkonsistenz ändern, darf `trice bind` diese Korrektur nicht nur in einer virtuellen Kopie ausführen. Es MUSS die Inkonsistenz melden und fehlschlagen, weil die Userquelle unverändert bleibt.

ID-freie, Platzhalter- und explizite Aufrufe dürfen in derselben Datei gemischt vorkommen.

### 4.3 Stamp-Semantik

Die bestehende Insert-Semantik ist maßgeblich. `trice bind` führt keine neue Case-Codierung ein.

Insbesondere:

- Canonical lower- und mixed-case Formen verwenden `iD(...)`.
- Ausschließlich großgeschriebene `TRICE...`-Formen verwenden vorhandene `id(...)`, `Id(...)` oder `ID(...)` beziehungsweise den durch `-defaultStampSize` gewählten Default.
- Ein vorhandener Wrapper mit ID 0 wird exakt beibehalten.
- Ein vorhandener Wrapper mit einer ID größer null wird nicht verändert.
- Nichtkanonische, aber vom bestehenden Parser akzeptierte Schreibweisen folgen derselben Entscheidung wie `trice insert`.

Eine neue Schreibweise wie `trRICE` oder `TrICE` zur zusätzlichen Stamp-Codierung ist nicht Bestandteil des MVP.

### 4.4 Aliase

`trice bind` MUSS `-alias` und `-salias` durch dieselbe Parser- und Zuordnungslogik wie `trice insert` berücksichtigen.

Ein transparenter Alias ist unmittelbar bindbar, beispielsweise:

```c
#define printi trice
```

mit:

```sh
trice bind -alias printi
```

Bei funktionsartigen oder anderweitig angepassten Aliasmakros muss die Targetdefinition bind-kompatibel sein. Der Generator kann die beliebige Semantik einer User-Makrodefinition nicht allgemein rekonstruieren.

Für das MVP gilt daher:

- Der Host-Parser erkennt registrierte Aliase und ordnet ihnen IDs wie bisher zu.
- Objektartige Aliase auf eine kanonische Trice-Familie werden vollständig unterstützt.
- Funktionsartige Aliase werden unterstützt, wenn sie im Bind-Modus ID-frei an eine bind-fähige kanonische Trice-Familie weiterleiten oder ausdrücklich `TRICE_BIND_TID_HERE()` verwenden.
- Eine Aliasdefinition, die selbst einen gewöhnlichen Trice-Aufruf in ihrer Replacement-Liste enthält und keine Bind-Anpassung besitzt, fällt unter die Einschränkung „Trice in Makrodefinition“ aus [Anhang A.2](#a2-trice-aufruf-in-einer-präprozessormakrodefinition).

Die Dokumentation soll mindestens je ein funktionierendes Beispiel für `-alias` und `-salias` enthalten.

### 4.5 Parser-Ausschlussmarker

`trice bind` MUSS folgende Marker exakt wie `trice insert` berücksichtigen:

```c
// TRICE_INSERT_OFF
...
// TRICE_INSERT_ON
```

Der bestehende Maskierungsmechanismus soll wiederverwendet werden. Ausgeschlossene Bereiche:

- erhalten keine ID-Zuordnung,
- erzeugen keine Sidecar-Stelle,
- werden bei bind-spezifischen Syntaxprüfungen ignoriert,
- behalten Byteoffsets und Zeilennummern der restlichen Datei bei.

### 4.6 Unterstützte Sourcekontexte

Das MVP unterstützt direkte User-Level-Trice-Aufrufe in:

- C- und C++-Sourcedateien,
- Headerdateien,
- normalen Funktionen,
- `inline`- und `static inline`-Funktionen.

Ein direkter Aufruf in einem Header wird der physischen Headerdatei und deren File Key zugeordnet. Wird derselbe Header in mehreren Translation Units verarbeitet, verwenden alle Instanzen dieser Source-Logstelle dieselbe stabile Trice-ID.

Nicht die spätere Codegenerierung oder Inlining-Entscheidung des Compilers bestimmt die Identität, sondern die textuelle Definitionsstelle des direkten Trice-Aufrufs.

### 4.7 Nicht unterstützte Konstruktionen

Das MVP unterstützt zunächst nicht:

- mehrere bindbare Trice-Stellen in derselben physischen Sourcezeile,
- Trice-Aufrufe innerhalb gewöhnlicher Präprozessormakrodefinitionen,
- durch Präprozessorlogik zusammengesetzte oder veränderte Formatstrings,
- nicht transparente Custom-Aliase ohne Bind-Adapter.

Diese Fälle sind mit Beispielen, Anwendungsfällen, Begründung und möglichen späteren Lösungen in [Anhang A](#anhang-a-nicht-unterstützte-konstruktionen-im-mvp) beschrieben.

`trice bind` darf für diese Konstruktionen im MVP keinen stillen `trice insert`-Fallback ausführen und keine numerischen IDs in User-Trice-Aufrufe schreiben.

## 5. File Key, Sidecar und persistente Dateizuordnung

### 5.1 Dateilokaler Include ist erforderlich

Eine Datei mit erkannten Trice-Stellen enthält einen zugehörigen markierten Include:

```c
#include "trice_module_c_K73A915E9C4021B8.h" // trice-bind
```

Der Include erfüllt zwei Aufgaben:

1. Er speichert den stabilen File Key in der versionierten Userdatei.
2. Er setzt vor den Trice-Stellen dieser physischen Datei den passenden `TRICE_FILE_KEY`.

Eine zentrale Einbindung aller Sidecars in `triceConfig.h` ersetzt diese lokale Auswahl nicht. Sie könnte zwar alle ID-Makros sichtbar machen, aber der Standardpräprozessor kann aus dem Stringliteral `__FILE__` keinen dynamischen C-Makronamen erzeugen. Ohne dateilokale Auswahl wäre daher nicht bestimmt, welcher File Key mit `__LINE__` zu kombinieren ist.

### 5.2 File Key

Der File Key ist ein einmalig erzeugter zufälliger 64-Bit-Wert, beispielsweise:

```text
K73A915E9C4021B8
```

Eigenschaften:

- Präfix `K` zur klaren Kennzeichnung als Key,
- 16 großgeschriebene Hexadezimalziffern,
- Erzeugung mit Go-Standardbibliothek `crypto/rand`,
- Speicherung in der markierten Include-Zeile,
- Wiederverwendung bei späteren Läufen,
- keine Neuberechnung bei Dateiinhalt-, Pfad- oder Namensänderungen,
- keine Übertragung zum Target,
- kein Speicherverbrauch im Target,
- keine Änderung des JSON-Schemas.

Der Generator MUSS projektweit prüfen, dass ein Key nicht mehreren unterschiedlichen Dateien zugeordnet ist. Bei einer extrem unwahrscheinlichen Neukollision wird ein neuer Wert erzeugt. Wird eine Userdatei einschließlich Include-Zeile kopiert, meldet `trice bind` den duplizierten Key.

Beispielcode für die Erzeugung:

```go
func newFileKey() (string, error) {
    var b [8]byte
    if _, err := rand.Read(b[:]); err != nil {
        return "", err
    }
    return "K" + strings.ToUpper(hex.EncodeToString(b[:])), nil
}
```

### 5.3 Sidecar-Dateiname und Ausgabeordner

Vorgeschlagenes Namensschema:

```text
trice_<normalisierter Basisname>_<FileKey>.h
```

Beispiel:

```text
module.c
→ trice_module_c_K73A915E9C4021B8.h
```

Der Basisname einschließlich Source-Extension verbessert die Lesbarkeit. Zeichen außerhalb `[A-Za-z0-9_]` werden durch `_` ersetzt. Die Eindeutigkeit liefert der File Key.

Alle Sidecars liegen standardmäßig in einem flachen Ordner:

```text
./build/triceIDs
```

Der Compiler benötigt dafür genau einen zusätzlichen Include-Pfad. Der Ordner MUSS automatisch vom Source-Scan ausgeschlossen werden.

### 5.4 Sidecar-Inhalt

Ein Sidecar enthält mindestens:

```c
// Generated by trice bind. Do not edit.

#undef TRICE_FILE_KEY
#define TRICE_FILE_KEY K73A915E9C4021B8

#define TRICE_ID_K73A915E9C4021B8_L9 12345u // trice("msg:Hello trice bind world\n")
```

Für die vollständige Unterstützung der drei ID-Zustände und aller Makrofamilien darf der Sidecar zusätzlich interne Site-Deskriptoren enthalten, beispielsweise:

```c
#define TRICE_BIND_KIND_K73A915E9C4021B8_L9 TRICE_BIND_SITE_AUTO
#define TRICE_BIND_TID_K73A915E9C4021B8_L9  iD(TRICE_ID_K73A915E9C4021B8_L9)
```

Die exakten internen Namen können bei der Implementierung angepasst werden. Normativ ist:

- Die numerische ID ist als einzelne lesbare Definition vorhanden.
- Der Kommentar nennt den normalisierten Trice-Aufruf beziehungsweise mindestens den Formatstring.
- Alle für die Target-Makros erforderlichen Angaben zu Auto-Binding, Null-Platzhalter oder expliziter ID sind eindeutig verfügbar.
- Die Reihenfolge ist nach Sourcezeile deterministisch.
- Kommentare werden auf eine Zeile normalisiert und dürfen keine unbeabsichtigte Präprozessor-Zeilenfortsetzung erzeugen.
- Eine Datei wird nur ersetzt, wenn sich ihr Inhalt tatsächlich ändert.

### 5.5 Kein klassischer Include-Guard

Der Sidecar erhält keinen klassischen Include-Guard. Eine erneute Einbindung desselben Sidecars kann den zugehörigen `TRICE_FILE_KEY` bewusst wiederherstellen.

`trice bind` SOLL eine mehrfache Einbindung desselben Sidecars als Warnung melden. Sie ist allein kein Fehler, sofern alle Vorkommen denselben File Key und denselben Sidecar betreffen.

### 5.6 Datei ohne aktuelle Trice-Stelle

Existiert eine markierte Include-Zeile, obwohl die Datei aktuell keine erkannte Trice-Stelle mehr enthält, bleibt die Include-Zeile erhalten. Der Sidecar KANN nur den File Key setzen.

Dadurch bleibt die Dateizuordnung stabil. Eine strengere Hygieneprüfung gehört zu einem späteren `-check`-Modus.

## 6. Verwaltung der Sidecar-Includes

### 6.1 Erwartete Include-Zeile

Für jede Datei mit mindestens einer bindbaren Trice-Stelle existiert genau eine zu dieser Datei gehörende markierte Include-Zeile:

```c
#include "trice_module_c_K73A915E9C4021B8.h" // trice-bind
```

Der Dateiname und der darin gespeicherte File Key müssen zum aktuellen Projektfile gehören. Der Include wird normalerweise nach den für die Datei wirksamen normalen Includes und vor der ersten eigenen Trice-Stelle platziert.

Der dateilokale Include ist erforderlich, weil in einer Translation Unit Sidecars mehrerer physischer Dateien verarbeitet werden können. Die eindeutigen Makronamen verhindern Kollisionen; der jeweils zuletzt aktivierte `TRICE_FILE_KEY` bestimmt, zu welcher physischen Datei `__LINE__` gehört.

### 6.2 Bereits vorhandener Include

Ist der erwartete Include vorhanden, MUSS `trice bind` ihn grundsätzlich an seiner Position belassen. Es prüft mindestens:

- Der Include kommt in dieser Datei nicht widersprüchlich mit verschiedenen Keys vor.
- Der im Namen enthaltene Key gehört zur Datei.
- Der Include steht vor der ersten bindbaren Trice-Stelle dieser Datei.
- Die Position ist nicht offensichtlich auf einen einzelnen unpassenden Präprozessorzweig beschränkt.
- Nach dem Include wird der aktive File Key nicht offensichtlich durch einen anderen Sidecar überschrieben, bevor die eigenen Trice-Stellen verarbeitet werden.

Eine mehrfache identische Einbindung desselben Sidecars ist eine Warnung, aber allein kein Fehler. Sie kann den File Key nach zwischenzeitlich verarbeiteten Headern bewusst wiederherstellen.

### 6.3 Automatisches Einfügen

Fehlt der Include und ist `-insertIncludes=true`, fügt `trice bind` ihn einmalig in die Userdatei ein. Der Default ist:

```text
-insertIncludes=true
```

Die Heuristik wählt eine Position:

1. innerhalb eines erkannten Include-Guards bei Headerdateien,
2. nach dem letzten wahrscheinlich zur initialen Include-Gruppe gehörenden Include,
3. vor der ersten bindbaren Trice-Stelle der physischen Datei.

Die eingefügte Zeile trägt immer den Kommentar:

```c
// trice-bind
```

Die Heuristik ist kein vollständiger C/C++-Präprozessor. Der User darf die Zeile verschieben. Bei späteren Läufen wird eine vorhandene konsistente Zeile nicht automatisch zurückverschoben.

### 6.4 Kriterien für eine unsichere Position

Die Position gilt mindestens in folgenden Fällen als unsicher:

- Es existieren mehrere voneinander getrennte Include-Blöcke mit Deklarationen, Definitionen oder sonstigem Code dazwischen.
- Ein `#include` steht nach der ersten eigenen Trice-Stelle.
- Zwischen mehreren eigenen Trice-Stellen stehen weitere Includes.
- Relevante Includes liegen in `#if`-, `#ifdef`-, `#ifndef`-, `#elif`- oder `#else`-Zweigen.
- Der Sidecar würde nur innerhalb eines Präprozessorzweigs aktiv, während bindbare Trice-Stellen auch in anderen Zweigen stehen.
- Die erste Trice-Stelle und die wahrscheinlich letzte Include-Position liegen in logisch verschiedenen Präprozessorzweigen.
- Ein Include wird über ein Makro angegeben, beispielsweise `#include HEADER_NAME`.
- Ein Header besitzt keinen sicher erkennbaren Include-Guard beziehungsweise eine für die Heuristik ungewöhnliche Guard-Struktur.
- Nach der vorgeschlagenen Position kann ein weiterer Header mit eigenem Sidecar den File Key umschalten.
- Die Datei mischt Includes und ausführbaren beziehungsweise deklarativen Code in einer Weise, die keine einzelne plausible Umschaltposition erkennen lässt.

Bei unsicherer Position darf `trice bind` die wahrscheinlichste Position verwenden, MUSS jedoch eine Warnung mit Datei, Zeile und konkretem Grund ausgeben. Ist eine Position offensichtlich falsch oder würde sie zu einer nicht auflösbaren ID führen, ist dies ein Fehler.

### 6.5 Zeilennummer nach Include-Einfügung

Eine neu eingefügte Include-Zeile kann die Zeilennummern nachfolgender Trice-Stellen verschieben. Deshalb MUSS die Sidecar-Erzeugung auf dem finalen, für den Build vorgesehenen Sourcezustand basieren.

Der Ablauf ist:

1. Include-Änderung im Speicher planen.
2. Bei aktiviertem Schreiben die Userdatei aktualisieren.
3. Die daraus resultierenden finalen Sourcezeilen bestimmen.
4. Sidecar-Namen aus diesen finalen Zeilennummern erzeugen.

Die ID-Zuordnung selbst bleibt stabil. Nur der buildlokale Makroname ändert sich bei einer Zeilenverschiebung, beispielsweise von:

```text
TRICE_ID_K73A915E9C4021B8_L9
```

zu:

```text
TRICE_ID_K73A915E9C4021B8_L10
```

Wurde eine Datei nach dem letzten Bind-Lauf so verändert, dass eine Trice-Stelle auf eine andere Zeile rutscht, MUSS `trice bind` erneut ausgeführt werden. Ein veralteter Sidecar soll vorzugsweise zu einem nicht definierten Makronamen und damit zu einem Compilerfehler führen, nicht zu einer stillschweigend falschen ID.

### 6.6 Verhalten bei `-insertIncludes=false`

Mit:

```text
-insertIncludes=false
```

werden Userdateien nicht um Include-Zeilen ergänzt. Fehlende Includes werden diagnostiziert.

Im Verbose-Modus MUSS mindestens die exakt einzufügende Zeile ausgegeben werden:

```text
src/foo/module.c: #include "trice_module_c_K73A915E9C4021B8.h" // trice-bind
```

Empfohlene ausführlichere Form:

```text
bind: missing include: src/foo/module.c:17: #include "trice_module_c_K73A915E9C4021B8.h" // trice-bind
```

Der File Key wird dabei bereits erzeugt und in der Ausgabe stabil verwendet. Ohne gespeicherte Include-Zeile muss der Lauf sicherstellen, dass derselbe Key innerhalb dieses Laufs für Sidecar, Diagnosen und Debugartefakte verwendet wird.

## 7. Kommandozeilenschnittstelle

### 7.1 Subkommando

Das neue Subkommando lautet:

```sh
trice bind [Optionen]
```

Es verwendet die bestehende Cobra-/Flag-Infrastruktur des Projekts und erscheint in `trice help` sowie `trice help -all`.

### 7.2 Wiederverwendete Insert-Optionen

Soweit fachlich anwendbar, MUSS `bind` dieselben Optionen und Defaults wie `insert` verwenden. Dazu gehören insbesondere:

- Source- und Suchpfade,
- Datei- und Verzeichnisausschlüsse,
- `-til` und `-li`,
- ID-Bereich und Vergabemethode,
- `-defaultStampSize`,
- `-alias` und `-salias`,
- Label-, Prüfsummen- und Konsistenzoptionen, soweit sie bei `insert` auf die ID-Datenbanken wirken,
- `-dry-run`,
- Verbosity- beziehungsweise Logoptionen.

Optionen, die ausschließlich die textuelle Darstellung einer instrumentierten Userquelle steuern, sollen nicht unkritisch übernommen werden. Beispiele sind reine Whitespace- oder Cache-Optionen. Sie werden nur registriert, wenn sie im Bind-Ablauf eine klar definierte Wirkung haben.

### 7.3 Bind-spezifische Optionen

Für das MVP sind vorgesehen:

```text
-bindDir string
    Ausgabeordner der Sidecar-Header.
    Default: ./build/triceIDs

-insertIncludes bool
    Fehlende markierte Sidecar-Includes heuristisch in Userdateien einfügen.
    Default: true
```

Der Name `-bindDir` kann an vorhandene Namenskonventionen des Projekts angepasst werden; die Semantik ist verbindlich.

Ein späteres:

```text
-check
```

ist in Abschnitt 9.4 beschrieben, aber nicht Voraussetzung der ersten funktionsfähigen Implementierung.

### 7.4 Dry Run

Mit `-dry-run` werden:

- Source- und Listenänderungen berechnet,
- File Keys und Sidecar-Inhalte bestimmt,
- Diagnosen ausgegeben,
- aber keine Userdatei, JSON-Datei, Sidecar-Datei oder Debugkopie geschrieben.

Die Ausgabe MUSS erkennen lassen, welche Dateien geändert oder erzeugt würden.

### 7.5 Exitstatus

- Keine Fehler: Exitstatus 0.
- Nur Warnungen: Exitstatus 0.
- Mindestens ein Fehler: Exitstatus ungleich 0.

Alle während der Analyse erkennbaren Diagnosen werden gesammelt und gemeinsam ausgegeben; ein Fehler in einer Datei soll die Analyse der übrigen Dateien nicht vorzeitig beenden.

## 8. Verarbeitungsmodell

### 8.1 Phasen

Der MVP-Ablauf besteht aus folgenden Phasen:

1. Optionen validieren und Pfade normalisieren.
2. `til.json` und `li.json` mit den bestehenden Pre-Processing-Funktionen laden.
3. Kandidatendateien mit derselben Walk- und Ausschlusslogik wie `trice insert` bestimmen.
4. Dateien parallel analysieren und `TRICE_INSERT_OFF`-/`ON`-Bereiche maskieren.
5. Vorhandene Sidecar-Includes und File Keys erfassen.
6. Fehlende Includes und neue Keys planen.
7. Den finalen Sourcezustand einschließlich geplanter Include-Zeilen im Speicher herstellen.
8. Die bestehende Insert-ID-Zuordnung auf diesem In-Memory-Inhalt ausführen, ohne den instrumentierten Inhalt in die Userdatei zurückzuschreiben.
9. Aus Original-, finalem und virtuell instrumentiertem Inhalt strukturierte Bind-Stellen erzeugen.
10. Alle Diagnosen und globalen Konflikte sammeln.
11. Bei fehlerfreier Analyse JSON-Dateien, notwendige Include-Änderungen, Sidecars und Debugkopien konsistent schreiben.

### 8.2 Wiederverwendung der Insert-Logik

Für die erste Implementierung SOLL die bestehende `trice insert`-Funktionalität möglichst weitgehend wiederverwendet werden. Dies umfasst insbesondere:

- Makroerkennung,
- Aliasauflösung,
- Maskierung ausgeschlossener Bereiche,
- Erkennung vorhandener ID-Wrapper,
- Wiederverwendung bestehender IDs,
- Vergabe neuer IDs,
- ID-Bereich und Vergabemethode,
- Aktualisierung von `til.json` und `li.json`,
- Validierung expliziter IDs,
- `-defaultStampSize`.

Der wesentliche Unterschied lautet:

> Der durch die Insert-Logik erzeugte instrumentierte Sourceinhalt wird nicht als Userquelle gespeichert.

Stattdessen dient er:

- als Referenz für die gebundene ID und Stamp-Form,
- zur Erzeugung der Sidecar-Deskriptoren,
- als diagnostische Vergleichskopie gemäß Abschnitt 8.5.

Diese Lösung darf intern zusätzlichen Parse- und Speicheraufwand verursachen. Korrektheit und schnelle Implementierbarkeit haben im MVP Vorrang. Eine spätere direkte Site-Erfassung ohne virtuellen Insert-Pass ist eine Optimierung und ändert das spezifizierte Verhalten nicht.

### 8.3 Vergleich von Original und virtueller Insert-Fassung

Der Binder MUSS für jede erkannte Stelle feststellen:

- ursprüngliche Schreibweise,
- finale Sourcezeile,
- von `insert` zugeordnete numerische ID,
- verwendete Form `iD`, `id`, `Id` oder `ID`,
- Trice-Typ und Formatstring,
- ob die Stelle ID-frei, ein Null-Platzhalter oder eine explizite ID ist.

Für explizite IDs gilt:

- Führt der virtuelle Insert-Pass nur zu derselben ID und einer konsistenten Listenpflege, ist die Stelle gültig.
- Würde die Source-ID geändert, entfernt oder wegen einer Kollision ersetzt, ist dies ein Fehler. `bind` darf diese Änderung nicht verbergen.

### 8.4 Parallelität und Determinismus

Die Dateiverarbeitung MUSS dieselbe Parallelitätsstrategie wie `trice insert` verwenden. Insbesondere darf die Analyse nicht als rein sequentielle Schleife über große Projekte implementiert werden.

Zulässig ist ein Modell mit:

- parallelem Einlesen und Analysieren pro Datei,
- gemeinsamem, durch die vorhandenen Synchronisationsmechanismen geschütztem ID-Allocator,
- paralleler Sidecar-Aufbereitung,
- einer abschließenden deterministischen Commit-Phase.

Die resultierenden IDs, Sidecar-Inhalte und JSON-Dateien müssen bei identischer Eingabe unabhängig von der Goroutine-Reihenfolge reproduzierbar sein. Bestehende Insert-Synchronisation und Mutex-Verwendung sollen wiederverwendet werden.

### 8.5 Diagnostische virtuelle Insert-Fassungen

Für die Inbetriebnahme und Fehlersuche schreibt Version 1 die im Speicher instrumentierten Fassungen immer unter:

```text
./TriceIDs/<projekt-relativer-Pfad>
```

Beispiel:

```text
src/driver/module.c
→ TriceIDs/src/driver/module.c
```

Diese Dateien:

- entsprechen soweit möglich dem Ergebnis eines separaten `trice insert`-Laufs,
- werden nicht kompiliert,
- werden nicht als Userquellen behandelt,
- werden vom Source-Scan ausgeschlossen,
- dienen dem Vergleich und der Parserdiagnose,
- sind generierte, normalerweise nicht versionierte Artefakte.

Der Name `TriceIDs` ist bewusst vom Sidecar-Ordner `build/triceIDs` verschieden. Eine spätere Option zum Abschalten dieser Debugausgabe ist möglich; für Version 1 ist die Ausgabe verbindlich.

### 8.6 Konsistentes Schreiben

Reguläre Ausgaben werden erst geschrieben, nachdem alle Dateien analysiert und globale Konflikte geprüft wurden.

Bei mindestens einem fatalen Fehler gilt:

- Keine teilweise aktualisierte `til.json` oder `li.json`.
- Keine Sidecar-Menge, die als vollständig gültig erscheinen könnte.
- Keine teilweise eingetragenen neuen Includes, soweit die Änderung noch nicht committed wurde.

Dateien werden über temporäre Dateien und atomaren Ersatz geschrieben, soweit das Betriebssystem dies ermöglicht. Ein bestehendes Artefakt wird nur ersetzt, wenn sich sein Inhalt tatsächlich ändert.

## 9. Validierung und Diagnosen

### 9.1 Verbindliche MVP-Prüfungen

Version 1 MUSS alle Prüfungen enthalten, deren Auslassen falschen Targetcode oder eine falsche ID-Zuordnung ermöglichen könnte. Dazu gehören mindestens:

- Duplizierter File Key in unterschiedlichen Dateien.
- Widersprüchlicher Sidecar-Name oder Key.
- Fehlender Sidecar bei `-insertIncludes=false`.
- Nicht auflösbare oder mehrfach belegte Bind-Stelle.
- Mehrere bindbare Trice-Aufrufe auf derselben physischen Zeile.
- Trice-Aufruf in einer nicht unterstützten Makrodefinition.
- Nicht transparenter Alias ohne Bind-Adapter.
- Widerspruch zwischen expliziter ID, Formatstring, `til.json` oder `li.json`.
- Virtueller Insert-Pass würde eine explizite ID in der Userquelle ändern.
- Nicht unterstützter oder ungültiger `-defaultStampSize`.
- Sidecar steht nach der ersten zu bindenden Trice-Stelle.
- Der aktive File Key kann für eine Stelle nicht eindeutig bestimmt werden.
- Erzeugter Sidecar würde einen im Build nicht definierten Site-Namen liefern.

### 9.2 Warnungen

Warnungen sind insbesondere angemessen bei:

- heuristisch unsicherer Include-Position,
- mehrfacher identischer Einbindung desselben Sidecars,
- vorhandener Include-Zeile für eine Datei ohne aktuelle Trice-Stelle,
- ungewöhnlicher, aber eindeutig auswertbarer Include-Struktur,
- diagnostischen Artefakten, die nicht entfernt werden konnten.

Warnungen dürfen nicht zur stillen Akzeptanz einer nachweislich falschen Bindung führen.

### 9.3 Ausgabeformat

Diagnosen SOLLEN mindestens enthalten:

```text
<severity>: <path>:<line>:<column>: <message>
```

Beispiel:

```text
warning: src/module.c:18:1: sidecar placement is uncertain because a conditional include follows
error: include/log.h:27:5: Trice call inside macro definition is not supported by bind MVP
```

Die Gesamtausgabe wird nach normalisiertem Pfad, Zeile, Spalte und Schweregrad sortiert. Zusammenfassend werden Anzahl der analysierten Dateien, erzeugten Sidecars, neuen IDs, Warnungen und Fehler ausgegeben.

### 9.4 Späterer `-check`-Modus

Ein umfassender Hygiene-Modus ist nicht Voraussetzung der ersten Version. Ein späteres:

```sh
trice bind -check
```

kann zusätzlich prüfen:

- Include eines leeren Sidecars,
- verwaiste Sidecars,
- Sidecars ohne zugehörige Projektdatei,
- unbenutzte File Keys,
- alte Definitionen im Sidecar,
- Abweichungen zwischen Source, Sidecar, `til.json` und `li.json`,
- fehlende Aktualisierung nach einer reinen Zeilenverschiebung,
- nicht mehr benötigte markierte Include-Zeilen.

Diese Prüfungen verändern den MVP-Bindemechanismus nicht.

## 10. Implementierungsumriss

### 10.1 Vorgeschlagene Dateien und Verantwortlichkeiten

Die konkrete Dateiaufteilung darf an die bestehende Projektstruktur angepasst werden. Vorgesehen sind sinngemäß:

```text
cmd/ oder internal/...       Registrierung des Subkommandos und der Flags
internal/id/bindIDs.go       Orchestrierung des Bind-Ablaufs
internal/id/bindParse.go     Site- und Include-Analyse
internal/id/bindRender.go    Sidecar- und Debugartefakte
internal/id/bindTypes.go     interne Datenstrukturen und Diagnosen
src/trice*.h                 TRICE_MODE und bind-fähige User-Level-Makros
```

Gemeinsame Insert-/Bind-Funktionalität soll nicht dupliziert werden. Geeignete vorhandene Funktionen werden extrahiert oder parametrisiert, sofern dies ohne Verhaltensänderung von `insert` möglich ist.

### 10.2 Interne Datenstrukturen

Eine Bind-Stelle benötigt mindestens:

```go
type bindIDState uint8

const (
    bindIDFree bindIDState = iota
    bindIDZeroPlaceholder
    bindIDExplicit
)

type bindStampForm uint8

const (
    bindStampID bindStampForm = iota // iD
    bindStamp0                        // id
    bindStamp16                       // Id
    bindStamp32                       // ID
)

type bindSite struct {
    Path         string
    FileKey      string
    Line         int
    Column       int
    MacroName    string
    AliasName    string
    Type         string
    Format       string
    ID           int
    IDState      bindIDState
    StampForm    bindStampForm
    SourceText   string
}
```

Eine Dateiplan-Struktur enthält zusätzlich:

- Originalinhalt,
- finalen Inhalt mit geplantem Include,
- virtuell instrumentierten Inhalt,
- vorhandenen beziehungsweise neuen File Key,
- Include-Position und deren Sicherheitsbewertung,
- Sidecar-Pfad und -Inhalt,
- Diagnosen.

Die genauen Typnamen sind nicht normativ.

### 10.3 Anpassung der bestehenden Insert-Funktionen

Die vorhandene Insert-Logik soll so parametrisiert werden, dass sie:

- wie bisher Userdateien instrumentieren kann,
- für `bind` denselben instrumentierten Inhalt nur zurückliefert,
- optional strukturierte Informationen je ersetzter Stelle an einen Collector meldet.

Für die erste Implementierung ist es zulässig, die Site-Information aus dem Vergleich von Original und instrumentierter Fassung zu gewinnen. Ein Collector ist vorzuziehen, sobald er ohne Risiko für `insert` ergänzt werden kann.

Die bestehende Funktion, welche den instrumentierten Inhalt tatsächlich in die Userdatei schreibt und `TRICE_CLEAN` ändert, darf vom Bind-Ablauf nicht aufgerufen werden.

### 10.4 Target-Makroschicht

Die Target-Library benötigt einen bind-fähigen Pfad für alle in Abschnitt 4.1 genannten User-Level-Familien.

Die Umsetzung SOLL zentral und mechanisch erfolgen:

1. Generische Site-Helfer bilden `TRICE_FILE_KEY + __LINE__` auf ID, TID und Bind-Art ab.
2. Jede öffentliche Makrofamilie dispatcht abhängig von `TRICE_MODE` auf:
   - bisherigen expliziten Pfad,
   - bisherigen Clean-/Off-Pfad,
   - neuen Bind-Pfad.
3. Der Bind-Pfad unterscheidet ID-frei, Null-Platzhalter und explizite ID.
4. Interne Daten-, Pack-, Buffer- und Transportfunktionen bleiben unverändert.

Vereinfachtes, nicht normatives Prinzip:

```c
#if TRICE_MODE == TRICE_MODE_BIND
    #define TRICE_BIND_ID_HERE()  /* Sidecar-ID über key + line */
    #define TRICE_BIND_TID_HERE() /* iD/id/Id/ID aus Site-Deskriptor */
#endif
```

Die Makrofamilien dürfen nicht einzeln mit voneinander abweichender Semantik handgeschrieben werden, wenn sich gemeinsame Generator- oder Meta-Makrologik verwenden lässt.

### 10.5 Behandlung der drei ID-Zustände im Target

#### ID-freier Aufruf

Der Bind-Pfad fügt `TRICE_BIND_TID_HERE()` als fehlendes erstes Argument ein.

#### Null-Platzhalter

Ein bestehendes `id(0)`, `Id(0)` oder `ID(0)` wird als Platzhalter erkannt. Der numerische Nullwert wird durch die Sidecar-ID ersetzt; die Wrapperform bleibt erhalten.

Die konkrete Präprozessortechnik kann beispielsweise einen Bind-spezifischen Wrapperdeskriptor oder eine Erkennung des Literal-Tokens `0` verwenden. Sie MUSS mindestens die exakt vom Insert-Parser unterstützten Nullformen abdecken.

#### Explizite ID

Ein vorhandenes `iD(n)`, `id(n)`, `Id(n)` oder `ID(n)` mit `n > 0` wird vom Targetpfad unverändert verwendet. Der Sidecar-Eintrag darf diese ID nicht überschreiben.

Die Host-Validierung stellt vor dem Build sicher, dass die explizite ID konsistent ist.

### 10.6 Makrofamilien und erwarteter Aufwand

Die Repositoryanalyse ergibt folgende Implementierungsgruppen:

| Gruppe | Beispiele | Erwarteter Aufwand | Hauptpunkt |
|---|---|---:|---|
| Generische mixed-/lower-case Familien | `trice`, `Trice`, `TRice` | gering bis mittel | ID-freie Überladung und zentraler Site-TID |
| Reine Uppercase-Familien | `TRICE`, `TRICE8_3`, `TRICE_0` | mittel | Default-Stamp und Erhalt `id`/`Id`/`ID` |
| Breiten-/Arity-Familien | `TRice16_2`, `Trice32`, `trice64_4` | gering nach zentraler Basis | mechanische Weiterleitung auf gemeinsame Implementierung |
| String-/Buffer-Familien | `triceS`, `triceN`, `triceB` | mittel | abweichende Argumentverträge, gleiche Site-ID-Idee |
| Float-/Spezialfamilien | `triceF`, `triceC` und vorhandene Spezialformen | mittel | vorhandene Targetpfade prüfen und anbinden |
| Assertions | `TRICE_ASSERT...` und verwandte Formen | mittel | User-Level-Aufruf und intern erzeugte Trice-Stelle unterscheiden |
| Transparente Aliase | `printi` als Alias auf `trice` | gering | vorhandene Aliasliste und kanonischen Targetpfad verwenden |
| Angepasste funktionsartige Aliase | projektspezifisch | potenziell hoch | benötigen klaren Bind-Adapter oder bleiben außerhalb des MVP |

Die Parserseite ist bereits breit angelegt. Der wesentliche Implementierungsaufwand liegt in einer konsistenten Target-Makroschicht und ihrer Testabdeckung.

### 10.7 Keine automatische Source-Fallback-Instrumentierung

Version 1 führt bei einer nicht bindbaren Stelle keinen impliziten Teil-`insert` aus. Ein solcher Fallback würde erneut Sourceänderungen und einen Cleanup-Ablauf einführen.

Nicht unterstützte Stellen werden gesammelt gemeldet. Das bestehende `trice insert` bleibt als separater Workflow verfügbar.

## 11. Tests und Nachweise

### 11.1 Unit-Tests

Mindestens erforderlich sind Tests für:

- 64-Bit-File-Key-Erzeugung und Kollisionsbehandlung,
- Lesen und Wiederverwenden eines vorhandenen Keys,
- Erkennung kopierter beziehungsweise doppelter Keys,
- Sidecar-Namensbildung,
- deterministisches Rendering und Kommentare,
- Include-Erkennung und -Einfügung,
- alle Kriterien unsicherer Include-Positionen,
- Zeilenneuberechnung nach Include-Einfügung,
- `TRICE_INSERT_OFF`-/`ON`-Maskierung,
- ID-freie Aufrufe,
- `id(0)`, `Id(0)` und `ID(0)`,
- `-defaultStampSize` mit 0, 16 und 32,
- explizite IDs und Konfliktfälle,
- gemischte ID-Zustände in einer Datei,
- Alias- und Simple-Alias-Erkennung,
- Diagnoseaggregation und Sortierung,
- atomisches Schreiben und „nur bei Inhaltsänderung ersetzen“.

### 11.2 Target-Präprozessortests

Für jede unterstützte Makrofamilie MUSS mindestens geprüft werden:

- ID-freie Form bindet die erwartete ID.
- Null-Platzhalter behält seine Stamp-Form.
- Explizite ID bleibt erhalten.
- Bind- und Insert-Variante erreichen denselben internen TID-Wert.
- `TRICE_MODE_INSERTED`, `TRICE_MODE_CLEAN`, `TRICE_MODE_BIND` und `TRICE_OFF` dispatchen korrekt.

Wo möglich, wird die Präprozessorausgabe gegen Golden-Dateien geprüft.

### 11.3 Makro-Abdeckungsmatrix

`_test/testdata/triceCheck.c` ist die maßgebliche breite Makroreferenz. Sie MUSS bind-fähig gemacht werden, ohne die fachliche Testabdeckung zu reduzieren.

Dazu gehören insbesondere:

- generische Case-Varianten,
- Breiten 8/16/32/64,
- feste Arity-Varianten,
- `TRICE0` und `TRICE_0`,
- String-, Buffer-, Count-, Float-, RPC-/ABC- und Assertion-Familien,
- explizite und fehlende IDs,
- `id`, `Id`, `ID` und `iD`,
- Aliase.

Mehrere Trice-Aufrufe auf derselben physischen Zeile werden im Testfile auf getrennte Zeilen verteilt. Absichtlich getestete nicht unterstützte Makrodefinitionen gehören in einen separaten Negativtest oder werden mit den vorhandenen Ausschlussmarkern vom Bind-Lauf ausgenommen.

### 11.4 Integrationstests

Mindestens folgende Abläufe werden automatisiert:

1. Frisches Projekt ohne Sidecars und ohne File Keys.
2. `trice bind` erzeugt Keys, Includes, Sidecars, JSON-Updates und Debugkopien.
3. Projekt baut im `TRICE_MODE_BIND`.
4. Erzeugter Datenstrom wird mit normaler `til.json` und `li.json` decodiert.
5. Zweiter Bind-Lauf ohne Sourceänderung verändert keine Dateizeit unnötig.
6. Einfügen einer Nicht-Trice-Zeile verschiebt eine Logstelle; nach erneutem Bind bleibt die numerische ID gleich.
7. Umbenennen beziehungsweise Verschieben einer Datei bei erhaltenem Include-Key behält die Dateiidentität und IDs.
8. Kopieren einer Datei mit demselben Key erzeugt eine klare Diagnose.
9. `-insertIncludes=false` gibt die exakten Include-Zeilen aus.
10. `-dry-run` schreibt keine Dateien.

### 11.5 Parallelitäts- und Race-Tests

Der Bind-Lauf wird mit vielen Dateien und unter dem Go-Race-Detector ausgeführt. Zu prüfen sind:

- keine Data Races in ID-Listen, Location-Listen, Key-Registry und Diagnosen,
- identische Ergebnisse bei unterschiedlicher Scheduling-Reihenfolge,
- keine doppelten neuen IDs,
- keine teilweise geschriebenen Artefakte.

### 11.6 Regressionstests

- Bestehende `insert`-, `clean`-, `update`- und Logtests bleiben unverändert grün.
- `./scripts/testAll.sh full` MUSS erfolgreich laufen.
- Wo ein Bind-Test dieselbe Stelle wie `insert` verarbeitet, müssen ID, Typ, Formatstring und Location übereinstimmen.
- Der PoC in `examples/PoC_bind_v2` wird auf den echten Generator umgestellt beziehungsweise durch einen äquivalenten Generatorintegrationstest ergänzt.

## 12. Abnahmekriterien des MVP

Das MVP gilt als implementiert, wenn:

1. `trice bind` als dokumentiertes Subkommando verfügbar ist.
2. Die bestehende ID-Zuordnung von `insert` wiederverwendet wird, ohne ID-Werte in User-Trice-Aufrufe zu schreiben.
3. `til.json` und `li.json` dieselben fachlichen Ergebnisse wie bei `insert` erhalten.
4. File Keys und markierte Includes stabil erzeugt und geprüft werden.
5. Sidecars unter `./build/triceIDs` deterministisch erzeugt werden.
6. Die Userquellen direkt im `TRICE_MODE_BIND` gebaut werden.
7. ID-freie, Null-Platzhalter- und explizite Aufrufe gemischt funktionieren.
8. `-defaultStampSize` und die bestehende Case-/Stamp-Semantik erhalten bleiben.
9. Alle unterstützten User-Level-Makrofamilien und transparente Aliase durch Tests abgedeckt sind.
10. `_test/testdata/triceCheck.c` bind-fähig ist und `./scripts/testAll.sh full` erfolgreich läuft.
11. `TRICE_INSERT_OFF` und `TRICE_INSERT_ON` identisch zu `insert` wirken.
12. Die Dateiverarbeitung dieselbe Parallelität wie `insert` nutzt.
13. Alle Diagnosen gesammelt, sortiert und mit korrektem Exitstatus ausgegeben werden.
14. Die virtuellen instrumentierten Fassungen unter `./TriceIDs` erzeugt werden.
15. Bestehende Projekte ohne `TRICE_MODE` und mit bisherigem `TRICE_CLEAN` unverändert bauen.
16. Kein neues buildspezifisches Decoderartefakt und keine Änderung des Drahtformats erforderlich ist.

## 13. Zukünftige Erweiterungen und Optimierungen

Dieser Abschnitt ist nicht Bestandteil des MVP-Verhaltens. Die Erweiterungen dürfen das bestehende Bind-Format und die ID-Datenbanken nicht unnötig brechen.

### 13.1 Direkte Site-Erfassung ohne virtuellen Insert-Pass

Der Parser kann später die zugeordnete ID, Stamp-Form und Sourceposition direkt an einen Bind-Collector melden. Dadurch entfallen Vergleich und erneutes Parsen der virtuellen instrumentierten Fassung.

**Gewinn:** geringerer Speicher- und Laufzeitaufwand bei unverändertem Verhalten.

### 13.2 Analyse der aktiven Präprozessorkonfiguration

Ein optionaler Lauf des realen Präprozessors kann bestimmen, welche der textuell erfassten Logstellen in einer konkreten Buildkonfiguration aktiv sind.

**Erforderlich:** tatsächliche Compilerdefines, Include-Pfade und gegebenenfalls `compile_commands.json` oder eine gleichwertige Buildbeschreibung.

**Gewinn:** zusätzlicher Bericht über die aktive Teilmenge; die stabile ID-Vergabe bleibt sourcebasiert.

### 13.3 Post-Link-Image-Inventur

Eine optionale ELF- oder Link-Map-Analyse kann bestimmen, welche aktiven Logstellen nach Optimierung, LTO, Archivselektion und Section-Garbage-Collection im finalen Image enthalten sind.

**Gewinn:** exakte Buildinventur. Das Binding selbst bleibt unverändert.

### 13.4 Vorbereitete statische Libraries

Vorbereitete `.a`-Libraries können Metadaten und bindbare ID-Platzhalter enthalten. `trice bind` kann diese beim Produkt-Build in den stabilen ID-Raum einordnen.

**Erforderlich:** definiertes Library-Metadatenformat, bindbare Platzhalter und ein Link-Bindeartefakt.

**Gewinn:** stabile produktweite IDs für Libraries, deren Source beim Endprodukt nicht erneut gebunden wird.

### 13.5 Mehrere Trice-Aufrufe pro Zeile

Ein zusätzlicher Site-Index oder ein optionaler, nachweislich geeigneter `__COUNTER__`-Pfad kann mehrere Logstellen derselben physischen Zeile unterscheiden.

**Gewinn:** Unterstützung eines seltenen Codestils.

### 13.6 Trice-Aufrufe in Makrodefinitionen

Eine spätere Erweiterung kann eine Definitionsstellen-ID für Wrappermakros bereitstellen. Denkbare Verfahren sind:

- ein expliziter bind-fähiger Wrapperadapter,
- selektives und ausdrücklich aktiviertes `insert` für solche Definitionen,
- eine präprozessorbasierte Erfassung mit festgelegter Definitionsstellen-Semantik.

**Gewinn:** Migration bestehender Makro-Wrapper wie `LOG_ERROR`.

Ein solcher Modus darf nicht stillschweigend Sourcecode verändern.

### 13.7 Umfassender Check-Modus

Der in Abschnitt 9.4 beschriebene Hygiene- und Konsistenzlauf kann unabhängig vom normalen Bind-Lauf ausgebaut werden.

### 13.8 Abschaltbare Debugkopien

Nach Stabilisierung der Implementierung kann die Ausgabe unter `./TriceIDs` über eine Option abschaltbar werden. Der Default kann dann anhand praktischer Erfahrungen neu bewertet werden.

## Anhang A: Nicht unterstützte Konstruktionen im MVP

### A.1 Mehrere Trice-Aufrufe in einer physischen Zeile

Beispiel:

```c
trice("msg:first\n"); trice("msg:second\n");
```

**Anwendungsfall:** stark komprimierter Sourcecode oder generierter Code.

**Problem:** Beide Stellen sehen denselben `TRICE_FILE_KEY` und denselben Wert von `__LINE__`. Ein Sidecar-Makroname kann deshalb nicht zwischen ihnen unterscheiden.

**Warum nicht im MVP:** Der übliche und lesbarere Stil verwendet getrennte Zeilen. Die Einschränkung ist leicht diagnostizierbar und vermeidet eine zusätzliche Toolchainannahme.

**Spätere Lösung:** zusätzlicher Auftretensindex oder optional `__COUNTER__`, wenn Scan- und Compilerlauf zuverlässig dieselbe Expansion liefern.

### A.2 Trice-Aufruf in einer Präprozessormakrodefinition

Beispiel:

```c
#define LOG_ERROR(x) trice("msg:error=%d\n", x)
```

**Anwendungsfall:** kurze projektspezifische Logging-Wrapper.

**Problem:** Der enthaltene Trice-Aufruf wird erst bei Expansion von `LOG_ERROR` verarbeitet. `__LINE__` und der aktive File Key können dann die Aufrufstelle beschreiben. Die bisherige `insert`-Semantik vergibt dagegen genau eine ID an der Definitionsstelle.

**Warum nicht im MVP:** Die gewünschte Definitionsstellen-Semantik lässt sich mit dem einfachen direkten `File Key + __LINE__`-Mechanismus nicht allgemein herstellen. Ein automatischer Teil-`insert` würde die zentrale Eigenschaft unveränderter User-Trice-Aufrufe aufgeben.

**Spätere Lösung:** bind-fähiger Wrapperadapter, expliziter Kompatibilitätsmodus oder Präprozessoranalyse mit eindeutig festgelegter Semantik.

**MVP-Verhalten:** Fehlerdiagnose. Für diese Quellen bleibt `trice insert` verfügbar. Eine geeignete normale oder `static inline`-Funktion ist bind-fähig.

### A.3 Zusammengesetzter Formatstring

Beispiel:

```c
#define PREFIX "msg:"
trice(PREFIX "value=%d\n", value);
```

oder eine noch stärker makrogenerierte Variante.

**Anwendungsfall:** Wiederverwendung von Textpräfixen oder generierte Meldungen.

**Problem:** Der bestehende Trice-Parser setzt statische, direkt erkennbare Formatstrings voraus. Makroverkettung öffnet zusätzliche Fragen zu Präprozessorzustand, Identität und Listenstabilität.

**Warum nicht im MVP:** Diese Form ist auch im bestehenden Trice-Modell nicht als allgemeine Erweiterung vorgesehen. Unterschiedliche Inhalte sollen als Parameter übertragen werden, beispielsweise mit einer Stringvariante.

**Spätere Lösung:** keine allgemeine Zusage. Nur klar begrenzte Parsererweiterungen, falls das Trice-Formatstringmodell später ausdrücklich geändert wird.

### A.4 Nicht transparenter Custom-Alias

Beispiel:

```c
#define MY_TRACE(x) trice("msg:value=%d\n", transform(x))
```

mit Registrierung über `-alias`.

**Anwendungsfall:** projektspezifische Argumenttransformation oder ein Wrapper mit festem Formatstring.

**Problem:** Der Alias ist nicht nur ein weiterer Name für eine kanonische Trice-Signatur, sondern enthält eigene Semantik und einen Trice-Aufruf in einer Makrodefinition.

**Warum nicht im MVP:** Der Host-Parser kann einen Aliasnamen registrieren, aber nicht jede beliebige Target-Makrosemantik automatisch bind-fähig umschreiben.

**Spätere Lösung:** expliziter Bind-Adapter für den Alias oder Umstellung auf eine normale beziehungsweise `static inline`-Funktion.

## Anhang B: Makro- und Alias-Abdeckungsmatrix

Vor dem Merge wird aus Target-Headern, Insert-Parser und `_test/testdata/triceCheck.c` eine testbare Matrix erzeugt. Mindestens folgende Dimensionen werden kombiniert:

| Dimension | Werte beziehungsweise Beispiele |
|---|---|
| Case-Familie | `trice`, `Trice`, `TRice`, `TRICE` |
| Datenbreite | generisch, 8, 16, 32, optional 64 |
| Arity | variadisch, `_0`, `0`, `_1` bis unterstütztes Maximum |
| ID-Zustand | frei, `iD(0)`, `id(0)`, `Id(0)`, `ID(0)`, explizit > 0 |
| Spezialtyp | Standard, S, N, B, F, C, RPC/ABC, Assertions |
| Name | eingebaut, `-alias`, `-salias` |
| Modus | Inserted, Clean, Bind, Off |

Für jede vom Projekt als User-Level-Makro angebotene Kombination muss entweder:

- ein positiver Bind-Test existieren, oder
- eine ausdrücklich dokumentierte Nichtunterstützung mit Negativtest vorliegen.

Ein Makro darf nicht allein deshalb aus der Bind-Unterstützung fallen, weil sein Name ausschließlich großgeschrieben oder explizit aritätscodiert ist.

## Anhang C: Präprozessorauflösung und Site-Deskriptoren

### C.1 Einfacher ID-freier Aufruf

Userdatei `module.c`:

```c
#include "trice.h"
#include "trice_module_c_K73A915E9C4021B8.h" // trice-bind

void f(void)
{
    trice("msg:Hello trice bind world\n"); // Zeile 9
}
```

Sidecar:

```c
#undef TRICE_FILE_KEY
#define TRICE_FILE_KEY K73A915E9C4021B8

#define TRICE_ID_K73A915E9C4021B8_L9 12345u // trice("msg:Hello trice bind world\n")
#define TRICE_BIND_TID_K73A915E9C4021B8_L9 iD(TRICE_ID_K73A915E9C4021B8_L9)
```

Generische Helfer, vereinfacht:

```c
#define TRICE_CAT_ID_I(key, line)  TRICE_ID_##key##_L##line
#define TRICE_CAT_ID(key, line)    TRICE_CAT_ID_I(key, line)
#define TRICE_BIND_ID_HERE()       TRICE_CAT_ID(TRICE_FILE_KEY, __LINE__)

#define TRICE_CAT_TID_I(key, line) TRICE_BIND_TID_##key##_L##line
#define TRICE_CAT_TID(key, line)   TRICE_CAT_TID_I(key, line)
#define TRICE_BIND_TID_HERE()      TRICE_CAT_TID(TRICE_FILE_KEY, __LINE__)
```

Expansion an Zeile 9:

```text
TRICE_BIND_TID_HERE()
→ TRICE_CAT_TID(K73A915E9C4021B8, 9)
→ TRICE_BIND_TID_K73A915E9C4021B8_L9
→ iD(TRICE_ID_K73A915E9C4021B8_L9)
→ iD(12345u)
```

Das bind-fähige `trice`-Makro reicht `iD(12345u)` an denselben internen Trice-Pfad weiter, den die durch `trice insert` erzeugte Source verwenden würde. Der C-Compiler sieht nach der Präprozessierung eine gewöhnliche Konstante; es gibt keinen Runtime-Lookup.

### C.2 Uppercase-Makro ohne ID

Usercode:

```c
TRICE8_3("msg:%d %d %d\n", a, b, c);
```

Bei:

```text
-defaultStampSize 16
```

liefert der Sidecar sinngemäß:

```c
#define TRICE_BIND_TID_K73A915E9C4021B8_L20 Id(12346u)
```

Bei 0 beziehungsweise 32 wird `id(12346u)` beziehungsweise `ID(12346u)` erzeugt.

### C.3 Uppercase-Makro mit Null-Platzhalter

Usercode:

```c
TRICE8_3(ID(0), "msg:%d %d %d\n", a, b, c);
```

Der Sidecar kennt:

```c
#define TRICE_ID_K73A915E9C4021B8_L24 12347u
#define TRICE_BIND_TID_K73A915E9C4021B8_L24 ID(12347u)
```

Der Bind-Pfad ersetzt semantisch nur die Null durch `12347u`; die 32-Bit-Stamp-Form `ID` bleibt erhalten.

### C.4 Explizite ID

Usercode:

```c
trice(iD(777), "msg:fixed\n");
```

Der Host validiert ID, Typ, Formatstring und Location. Der Target-Makropfad verwendet weiterhin `iD(777)`. Ein vorhandener Sidecar-Eintrag an dieser Zeile darf den Wert nicht ersetzen.

### C.5 Warum ein Hash der Trice-Zeile die erneute Bindung nicht ersetzt

Ein Generator könnte zwar einen Hash des Sourcetextes in einen Sidecar-Makronamen schreiben. Der unveränderte C-Präprozessoraufruf kennt diesen Hash jedoch nicht. Er kann standardkonform nur bereits verfügbare Tokens wie `TRICE_FILE_KEY` und `__LINE__` zusammensetzen.

Verschiebt sich eine Logstelle, erzeugt `__LINE__` einen anderen Makronamen. Deshalb muss der Sidecar neu erzeugt werden. Die numerische ID bleibt dabei durch `til.json` und `li.json` stabil.

## Referenzen zum Repository

Die Implementierung soll sich insbesondere an folgenden bestehenden Bestandteilen des Branches `wip` orientieren:

- [`internal/id/insertIDs.go`](https://github.com/rokath/trice/blob/wip/internal/id/insertIDs.go): ID-Zuordnung, Listenpflege und Parallelverarbeitung von `insert`,
- [`internal/id`](https://github.com/rokath/trice/tree/wip/internal/id): Parser, Aliasbehandlung und gemeinsame ID-Verwaltung,
- [`src/triceOn.h`](https://github.com/rokath/trice/blob/wip/src/triceOn.h), [`src/trice.h`](https://github.com/rokath/trice/blob/wip/src/trice.h) und [`src/triceOff.h`](https://github.com/rokath/trice/blob/wip/src/triceOff.h): User-Level- und Target-Makros,
- [`_test/testdata/triceCheck.c`](https://github.com/rokath/trice/blob/wip/_test/testdata/triceCheck.c): breite Makroabdeckung,
- [`scripts/testAll.sh`](https://github.com/rokath/trice/blob/wip/scripts/testAll.sh): vollständiger Regressionstest mit `full`,
- [`examples/PoC_bind_v2`](https://github.com/rokath/trice/tree/wip/examples/PoC_bind_v2): Nachweis des grundsätzlichen Target-Bindemechanismus.
