# Trice Bind – MVP-Handbuchkopie für `PoC_bind_v2`

Diese Kopie konkretisiert das allgemeine MVP-Konzept für den compilierbaren
PoC in diesem Verzeichnis. Da `trice bind` noch nicht implementiert ist, sind
die Sidecars hier aus einem lokalen `trice insert`/`trice clean`-Durchlauf
abgeleitet.

## Überblick

`trice bind` soll Trice-Logstellen stabile IDs zuordnen, ohne numerische IDs in
die Trice-Aufrufe des Users einzutragen. Das Werkzeug scannt die
Projektquellen, aktualisiert `til.json` und `li.json` und erzeugt für jede
Source- oder Headerdatei mit unterstützten Trice-Anweisungen einen temporären
ID-Header.

Der Compiler übersetzt weiterhin unmittelbar die Originalquellen. Es werden
keine Schattenquellen verwendet, und ein nachfolgendes `trice clean` ist im
späteren echten Bind-Ablauf nicht erforderlich.

```c
trice("msg:module initialized\n");
```

## Dauerhafte und generierte Dateien

Dauerhaft sind:

- die Userquellen,
- `til.json` und `li.json`,
- jede mit `// trice-bind` markierte Include-Zeile.

Die generierten Sidecars sind normalerweise löschbare Buildartefakte. Im PoC
liegen sie sichtbar unter `triceIDs/`, damit der simulierte Zustand direkt
compilierbar und prüfbar ist.

## Dateischlüssel und Sidecar-Namen

Jede Datei mit Trice-Anweisungen erhält einen stabilen 64-Bit-Dateischlüssel.
Er wird als Präprozessortoken mit führendem `F` dargestellt:

```c
#include "trice_main_c_F5050505050505050.h" // trice-bind
```

Der lesbare Basisname ist nur eine Hilfe. Der Schlüssel liefert die
Dateiidentität und verhindert insbesondere Kollisionen zwischen den beiden
Paaren namens `dev.c` und `dev.h`. Er wird nicht zum Target übertragen.

## Inhalt eines Sidecars

Ein Sidecar wählt den aktuellen Dateischlüssel und ordnet Sourcezeilen IDs zu:

```c
#undef TRICE_FILE_KEY
#define TRICE_FILE_KEY F5050505050505050

#define TRICE_ID_F5050505050505050_L42 1000u
```

Die Trice-Makros kombinieren `TRICE_FILE_KEY` mit `__LINE__`. Der Compiler
sieht nach der Präprozessorauflösung eine gewöhnliche Integerkonstante. Ein
Runtime-Lookup oder zusätzliches Target-Memory ist nicht erforderlich.

## Position des Sidecar-Includes

Für eine `.c`-Datei steht der Sidecar nach allen normalen Includes und vor der
ersten direkten Trice-Anweisung:

```c
#include "dev.h"

#include "trice_device_dev_c_F2020202020202020.h" // trice-bind

void primaryDevRun(int value)
{
    trice("msg:source shared value=%d\n", value);
}
```

Diese Reihenfolge ist erforderlich, weil ein zuvor eingebundener Header seinen
eigenen Schlüssel ausgewählt haben kann.

## Header und `static inline`

Ein Header mit direkten Trice-Aufrufen besitzt einen eigenen Sidecar innerhalb
seines Include-Guards:

```c
#include "trice.h"
#include "trice_device_dev_h_F1010101010101010.h" // trice-bind

static inline void primaryDevInline(int value)
{
    trice("msg:inline shared value=%d\n", value);
}
```

Die Sidecar-Makros der Header bleiben im Präprozessornamensraum verfügbar.
Unterschiedliche Dateischlüssel verhindern Namenskollisionen. Der nachfolgende
Source-Sidecar setzt lediglich `TRICE_FILE_KEY` auf die einbindende `.c`-Datei
zurück.

## Identische Logzeilen

Identische Formatstrings können in verschiedenen Dateien oder Zeilen
auftreten. Der PoC vergibt dafür absichtlich verschiedene IDs:

```c
trice("msg:inline shared value=%d\n", value);
```

Diese Anweisung steht in beiden `dev.h`. Dasselbe gilt für
`msg:source shared value=%d` in beiden `dev.c`. Entscheidend ist die Bindung an
Datei und Zeile, nicht die textuelle Gleichheit des Formats.

## Vorgesehener echter Bind-Ablauf

Der spätere normale Ablauf ist:

```text
trice bind
Build
```

`trice bind` soll dabei:

1. konfigurierte C-, C++- und Headerdateien scannen,
2. bestehende IDs aus `til.json` und `li.json` wiederverwenden,
3. neuen Logstellen stabile IDs geben,
4. fehlende Dateischlüssel und markierte Includes erzeugen,
5. Sidecars in das konfigurierte Buildverzeichnis schreiben,
6. Schlüssel, Include-Positionen und Syntax prüfen,
7. unveränderte generierte Dateien nicht neu schreiben.

## Simulierter Ablauf dieses PoC

Bis das Kommando existiert, wurde nur für dieses Verzeichnis ausgeführt:

```sh
go run ../../../cmd/trice insert \
  -src . -i til.json -li li.json -liRoot . \
  -IDMin 1000 -IDMax 1999 -IDMethod upward
```

Die eingefügten IDs wurden in die Sidecars übertragen. Anschließend stellte
folgender Lauf die ID-freien Userquellen wieder her:

```sh
go run ../../../cmd/trice clean \
  -src . -i til.json -li li.json -liRoot .
```

Dieser Hilfsablauf ist nicht das Zielverhalten von `trice bind`, erzeugt aber
dieselben compilerrelevanten Makrobedingungen.

`trice clean` setzt den Konfigurationsmarker `TRICE_CLEAN` auf `1`. Für
`TRICE_BIND == 1` behandelt die Target-Library diesen Marker intern trotzdem
als aktiven Codezustand, weil die IDs aus Sidecars kommen. Ohne Bind-Modus
behält `TRICE_CLEAN` seine bisherige abschaltende Wirkung.

## MVP-Einschränkungen

- Pro physischer Sourcezeile ist nur eine Trice-Anweisung erlaubt.
- Trice-Aufrufe in Präprozessormakrodefinitionen werden nicht unterstützt.
- Formatstrings müssen statisch im Source erkennbar sein.
- Ein Sidecar muss vor der ersten direkten Logstelle seiner Datei aktiv sein.
- Ein späteres normales Include darf den für die Source benötigten Schlüssel
  nicht unbemerkt ersetzen.

Der Binder muss bei fehlenden, doppelten oder fremden Sidecars, duplizierten
Dateischlüsseln und widersprüchlichen JSON-Zuordnungen mit einer konkreten
Datei- und Positionsdiagnose abbrechen.

## Build und Logausgabe

Das CMake-Projekt bindet `../../../src` direkt ein. Die lokale `triceConfig.h`
wählt Stackbuffer, direkte Ausgabe und TCOBS-Framing. `main.c` implementiert
den synchron aufgerufenen Auxiliary-Writer und öffnet `log.bin` mit Modus
`wb`, sodass jeder Programmlauf eine neue Datei erzeugt.

```sh
cmake -S . -B build
cmake --build build
./build/PoC_bind_v2
tlog -p FILEBUFFER -args log.bin -pf TCOBS -prefix off -hs off \
  -i til.json -li li.json
```

## Ergebnis

Der Usercode bleibt ID-frei. Alle fünf Dateien mit Logstellen besitzen einen
eigenen Schlüssel und Sidecar. Der Compiler löst jede Kombination aus Datei
und Zeile zu einer konstanten Trice-ID auf; die bestehende Binärcodierung und
der Decoder bleiben unverändert.
