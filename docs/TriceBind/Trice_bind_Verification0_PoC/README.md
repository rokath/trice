# PoC_bind_6

## Zweck

Der PoC bindet eine bereits bekannte Trice-ID über einen generierten Header an eine ID-lose Logstelle. Die Userquelle `app/module.c` wird direkt kompiliert.

```c
#include "trice.h"
#include "module.c.triceIDs.h"

trice("msg:Hello trice bind world\n");
```

`til.json` enthält ID und Formatstring. `li.json` enthält Pfad und Sourcezeile. Das compilierbare Go-Kommando erzeugt daraus:

```sh
./build/trice bind \
  -til til.json \
  -li li.json \
  -src app/module.c \
  -out build/module.c.triceIDs.h
```

Der Sidecar enthält nur die Zeilenbindung:

```c
#define TRICE_ID_L9 1001u
```

## Bindung im Trice-Makro

Der PoC enthält unter `trice/src` einen kleinen Ausschnitt der Target-Library-Struktur von [rokath/trice](https://github.com/rokath/trice/tree/main/src).

`triceOn.h` bildet die aktuelle Sourcezeile auf den Sidecar-Eintrag ab:

```c
#define TRICE_BIND_ID_AT_LINE(line) TRICE_BIND_JOIN(TRICE_ID_L, line)
#define TRICE_BIND_ID_HERE() TRICE_BIND_ID_AT_LINE(__LINE__)
#define trice(format) trice32fn_0((uint16_t)iD(TRICE_BIND_ID_HERE()))
```

In `module.c`, Zeile 9, expandiert der Aufruf damit zu einer normalen ID-Konstante:

```c
trice32fn_0((uint16_t)1001u);
```

Der Formatstring wird im Targetcode nicht verwendet. Seine Zuordnung bleibt in `til.json`.

## Projektstruktur

```text
app/module.c                 direkt kompilierte Userquelle
cmd/trice/main.go            Go-PoC für `trice bind`
build/module.c.triceIDs.h    generierter Sidecar
til.json                     ID und Formatstring
li.json                      Pfad und Sourcezeile
trice/src/trice.h            öffentliche Target-Schnittstelle
trice/src/triceOn.h          Bind-Erweiterung des Makros
trice/src/trice.c            minimaler File-Backend
app/triceConfig.h            Projektkonfiguration
demo.sh                      vollständiger Demoablauf
```

Die Target-Library ist auf `trice(...)` ohne Timestamp und ohne Nutzwerte reduziert.

## Voraussetzungen

- macOS mit Xcode Command Line Tools (`clang`)
- Go 1.22 oder neuer
- `tlog` für die Decodierung

## Demo

```sh
./demo.sh
```

Der Ablauf:

1. Go-Kommando `trice` bauen.
2. `trice bind` ausführen.
3. `app/module.c` direkt mit `clang` kompilieren.
4. Targetdemo starten und `build/log.bin` schreiben.
5. Binärdaten anzeigen.
6. `tlog` zur Decodierung verwenden.

Decoderaufruf:

```sh
tlog -p FILEBUFFER -args ./build/log.bin -pf none -prefix off -hs off -i ./til.json -li ./li.json
```

## Trice-Datensatz

Der File-Backend schreibt den aktuellen ungeframten no-stamp/no-value-Datensatz. Für ID `1001` entstehen:

```text
e9 43 c0 00
```

- `0x43e9`: no-stamp-Kennung und ID `1001`
- `0xc0`: statischer Cycle-Wert
- `0x00`: Nutzdatenlänge null

## Umfang

Der PoC verarbeitet genau einen direkten `trice(...)`-Aufruf ohne Werte. Der Binder liest die Zuordnung aus `til.json` und `li.json`, prüft die angegebene Sourcezeile und erzeugt den Sidecar deterministisch.
