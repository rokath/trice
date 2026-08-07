# Trice Bind

## Überblick

`trice bind` ordnet Trice-Logstellen stabile IDs zu, ohne numerische IDs in die Trice-Aufrufe des Users einzutragen. Das Werkzeug scannt die Projektquellen, aktualisiert `til.json` und `li.json` und erzeugt für jede Source- oder Headerdatei mit unterstützten Trice-Anweisungen einen temporären ID-Header.

Der Compiler übersetzt weiterhin unmittelbar die Originalquellen. Es werden keine generierten Schattenquellen verwendet, und ein nachfolgendes `trice clean` ist nicht erforderlich.

Ein typischer Trice-Aufruf bleibt ID-frei:

```c
trice("msg:module initialized\n");
```

## Dauerhafte und generierte Dateien

Folgende Dateien sind dauerhaft und sollten in der Versionsverwaltung liegen:

- die Userquellen,
- `til.json`,
- `li.json`,
- jede von `trice bind` in eine Source- oder Headerdatei eingefügte Include-Zeile.

Die generierten Sidecar-Header sind Buildartefakte. Sie können jederzeit gelöscht und neu erzeugt werden und werden normalerweise nicht versioniert.

Alle Sidecars können in einem gemeinsamen Verzeichnis liegen, beispielsweise:

```text
build/triceIDs/
```

Dieses Verzeichnis muss dem Compiler als Include-Pfad bekannt sein.

## Dateischlüssel und Sidecar-Namen

Jede Source- oder Headerdatei mit unterstützten Trice-Anweisungen erhält einen stabilen zufälligen 64-Bit-Dateischlüssel. Er wird als mit `F` beginnendes Präprozessortoken aus 16 Hexadezimalziffern dargestellt und steht im Namen des Sidecar-Includes, beispielsweise:

```c
#include "trice_module_c_F73A915E9C4021B8.h" // trice-bind
```

Der Basisname macht den generierten Header leichter erkennbar. Der Dateischlüssel stellt die tatsächliche Identität her und verhindert Kollisionen zwischen Dateien mit gleichem Basisnamen. Er ist keine Trice-ID, wird nicht zum Target übertragen und belegt dort keinen Speicher.

Zwei unterschiedliche Dateien namens `module.c` können beispielsweise folgende Sidecars verwenden:

```text
trice_module_c_F73A915E9C4021B8.h
trice_module_c_F88217D4AC101E62.h
```

`trice bind` prüft, dass ein Dateischlüssel nicht mehreren unterschiedlichen Projektdateien zugeordnet ist. Wird eine Quelldatei zusammen mit ihrem Sidecar-Include kopiert, entsteht daher eine eindeutige Diagnose, bis eine der Kopien einen neuen Schlüssel erhält.

## Inhalt eines Sidecars

Ein generierter Sidecar wählt den aktuellen Dateischlüssel und ordnet Sourcezeilen stabile Trice-IDs zu:

```c
#undef TRICE_FILE_KEY
#define TRICE_FILE_KEY F73A915E9C4021B8

#define TRICE_ID_F73A915E9C4021B8_L9 12345u // trice("msg:module initialized\n")
#define TRICE_ID_F73A915E9C4021B8_L17 12346u // trice("msg:value=%d\n", value)
```

Die Kommentare dienen nur der Diagnose. Sie erleichtern die Prüfung der generierten Zuordnung bei Buildproblemen.

Die Trice-Makros kombinieren `TRICE_FILE_KEY` mit dem standardisierten Makro `__LINE__`. Die resultierende ID ist eine ganzzahlige Compile-Time-Konstante. Im Target entsteht kein zusätzlicher Tabellenzugriff zur Laufzeit.

## Position des Sidecar-Includes

Der Sidecar einer Datei muss aktiv sein, während die direkten Trice-Anweisungen dieser Datei verarbeitet werden.

Für eine `.c`-Datei ist folgende Struktur vorgesehen:

```c
#include "trice.h"
#include "module.h"
#include "driver.h"

#include "trice_module_c_F73A915E9C4021B8.h" // trice-bind

void moduleInit(void)
{
    trice("msg:module initialized\n");
}
```

Der Sidecar steht normalerweise nach den für die Datei wirksamen normalen Includes und vor ihrer ersten direkten Trice-Anweisung. Das ist notwendig, weil eingebundene Header eigene Dateischlüssel auswählen können.

Fehlt das Sidecar-Include, fügt `trice bind` es einmalig an einer wahrscheinlich geeigneten Stelle ein und markiert es mit `// trice-bind`. Bedingte oder ungewöhnlich angeordnete Includes können die korrekte Position uneindeutig machen. In diesem Fall muss der User das markierte Include an die richtige Stelle verschieben.

Bei späteren Läufen lässt `trice bind` ein vorhandenes Include an seiner Position und prüft mindestens:

- Der erwartete Sidecar wird genau einmal eingebunden.
- Sein Dateischlüssel gehört zur aktuellen Datei.
- Er steht vor der ersten unterstützten Trice-Anweisung dieser Datei.
- Kein offensichtlich später wirksames Include macht den ausgewählten Dateischlüssel ungültig.

Kann die Struktur nicht sicher akzeptiert werden, meldet `trice bind` Datei und Sourceposition, statt eine unsichere Bindung stillschweigend zu erzeugen.

## Headerdateien

Ein Header mit direkten Trice-Anweisungen erhält einen eigenen Sidecar und Dateischlüssel.

```c
#ifndef MODULE_H
#define MODULE_H

#include "trice.h"
#include "dependency.h"

#include "trice_module_h_F1111111111111111.h" // trice-bind

static inline void moduleCheck(int value)
{
    trice("msg:value=%d\n", value);
}

#endif
```

Der Sidecar steht innerhalb des Include-Guards, nach den für den Header wirksamen normalen Includes und vor seiner ersten direkten Trice-Anweisung.

Eine `.c`-Datei, die diesen Header einbindet, muss anschließend mit ihrem eigenen Sidecar wieder ihren eigenen Dateischlüssel auswählen. Die Definitionen beider Sidecars können gleichzeitig vorhanden sein, weil ihre Makronamen unterschiedliche Dateischlüssel enthalten.

Direkte Trice-Anweisungen in normalen Funktionen und in `static inline`-Funktionen werden sowohl in Source- als auch in Headerdateien unterstützt.

## Bind-Ablauf

Der normale Buildablauf ist:

```text
trice bind
Build
```

Während `trice bind`:

1. werden die konfigurierten Source- und Headerdateien ohne Entfernen inaktiver Präprozessorzweige gescannt,
2. werden bestehende stabile IDs aus `til.json` und `li.json` wiederverwendet,
3. erhalten neue unterstützte Trice-Anweisungen neue stabile IDs,
4. werden fehlende Dateischlüssel und markierte Sidecar-Includes erzeugt,
5. werden die Sidecar-Header im konfigurierten Buildverzeichnis geschrieben,
6. werden Sidecar-Zuordnung, Eindeutigkeit der Dateischlüssel, Include-Position und unterstützte Syntax geprüft,
7. wird eine generierte Datei nur ersetzt, wenn sich ihr Inhalt geändert hat.

Der anschließende Build übersetzt die Originalquellen. Das Buildsystem muss die Sidecars wie gewöhnliche Header-Abhängigkeiten erfassen. Bei üblicher automatischer Header-Abhängigkeitserfassung werden dadurch nur die betroffenen Translation Units neu übersetzt.

## Unterstützte Trice-Anweisungen

Das MVP unterstützt direkte Trice-Aufrufe in:

- C- und C++-Sourcedateien,
- Headerdateien,
- normalen Funktionen,
- `static inline`-Funktionen.

Jeder unterstützte Trice-Aufruf muss einen statischen, im Source erkennbaren Formatstring besitzen.

Beispiel:

```c
void reportValue(int value)
{
    trice("msg:value=%d\n", value);
}
```

Auch Logstellen in inaktiven bedingten Sourceabschnitten erhalten stabile IDs, weil der Scanner die ursprünglichen Dateien untersucht:

```c
#if FEATURE_X
trice("msg:feature X active\n");
#endif
```

Dadurch bleibt die ID erhalten, wenn sich `FEATURE_X` zwischen Builds ändert.

## Einschränkungen des MVP

### Nur eine Trice-Anweisung pro physischer Sourcezeile

Zwei Trice-Aufrufe dürfen nicht in derselben physischen Sourcezeile stehen, weil das MVP eine Logstelle durch Dateischlüssel und `__LINE__` identifiziert.

Nicht unterstützt:

```c
trice("msg:first\n"); trice("msg:second\n");
```

Stattdessen sind getrennte Zeilen zu verwenden:

```c
trice("msg:first\n");
trice("msg:second\n");
```

### Keine Trice-Aufrufe in Makrodefinitionen

Ein Trice-Aufruf innerhalb einer Präprozessormakrodefinition wird vom `trice bind` MVP nicht unterstützt:

```c
#define LOG_ERROR(x) trice("error=%d\n", x)
```

Die für die Bindung verwendeten Makros werden erst bei der Expansion von `LOG_ERROR` ausgewertet. Dateischlüssel und Zeilennummer würden dann die Makroaufrufstelle beschreiben und die Trice-Anweisung in der Makrodefinition nicht zuverlässig identifizieren.

`trice bind` meldet solche Definitionen als nicht unterstützt. Projekte, die dieses Codemuster benötigen, müssen für diese Quellen weiterhin `trice insert` verwenden oder den Wrapper, soweit geeignet, durch eine normale beziehungsweise `static inline`-Funktion ersetzen.

### Statische Formatstrings

Der Trice-Formatstring muss statisch erkennbar sein. Durch Präprozessorverkettung zusammengesetzte Formatstrings gehören nicht zum unterstützten Trice-Modell. Variable Texte sind mit der dafür vorgesehenen Trice-Stringvariante als Parameter zu übertragen.

## Diagnosen

`trice bind` beendet die Verarbeitung mit einer präzisen Diagnose, wenn eine Bedingung zu einer falschen Bindung führen könnte. Dazu gehören insbesondere:

- ein fehlender, doppelter oder fremder Sidecar-Include,
- derselbe Dateischlüssel in unterschiedlichen Projektdateien,
- ein nicht unterstützter Trice-Aufruf in einer Makrodefinition,
- mehrere Trice-Aufrufe in einer physischen Sourcezeile,
- ein Sidecar nach der ersten direkten Trice-Anweisung,
- ein späteres Include, das den aktuellen Dateischlüssel ersetzen kann,
- widersprüchliche Einträge in `til.json` oder `li.json`.

Die Kommentare in den generierten Sidecars zeigen den zugehörigen Trice-Sourcetext und erleichtern die Prüfung der Zuordnung:

```c
#define TRICE_ID_F73A915E9C4021B8_L17 12346u // trice("msg:value=%d\n", value)
```

## Ergebnis

Mit `trice bind` enthält der Usercode lesbare ID-freie Trice-Aufrufe und pro Datei mit unterstützten Trice-Anweisungen genau ein stabiles markiertes Sidecar-Include. `til.json` und `li.json` behalten die dauerhafte ID-Information, während temporäre Sidecar-Header dem Build der Originalquellen die benötigten Compile-Time-Konstanten bereitstellen.
