# PoC_bind_v2

Dieses Beispiel prüft, ob ID-freie `trice(...)`-Aufrufe über dateilokale
Sidecar-Header ohne Compilerprobleme gegen die echte Target-Library in
`../../../src` übersetzt werden können.

## Geprüfte Fälle

- `main.c` mit gewöhnlichen Logaufrufen und Aufrufen der Device-Funktionen,
- `device/dev.c` und `device/dev.h`,
- ein zweites gleichnamiges Paar unter `components/deep/device/`,
- `static inline`-Funktionen in beiden Headern,
- identische Formatstrings an unterschiedlichen Logstellen,
- unterschiedliche Formatstrings in allen beteiligten Dateien,
- Wiederherstellung des Source-Dateischlüssels nach Header-Includes.

Identische Formate erhalten im PoC absichtlich getrennte IDs. Damit ist auch
sichtbar, dass die Bindung die Logstelle und nicht nur den String identifiziert.

## Build und Ausführung

Vom Verzeichnis dieses Beispiels aus:

```sh
cmake -S . -B build
cmake --build build
./build/PoC_bind_v2
```

Das Programm legt `log.bin` bei jedem Start mit `fopen(..., "wb")` neu an.
Bei Multi-Configuration-Generatoren liegt das Programm gegebenenfalls unter
`build/Release/PoC_bind_v2` beziehungsweise `build/Debug/PoC_bind_v2`.

Die TCOBS-gerahmten Daten lassen sich mit den lokalen Listen anzeigen:

```sh
tlog -p FILEBUFFER -args log.bin -pf TCOBS -prefix off -hs off \
  -i til.json -li li.json
```

Alternativ kann das Repository-Kommando verwendet werden:

```sh
go run ../../../cmd/trice log -p FILEBUFFER -args log.bin -pf TCOBS \
  -prefix off -hs off -i til.json -li li.json
```

## Simulierter Bind-Ablauf

Eine echte `trice bind`-Implementierung existiert noch nicht. Für diesen PoC
wurde das Ergebnis folgendermaßen simuliert:

1. Die ID-freien Quellen und die markierten Sidecar-Includes wurden angelegt.
2. `trice insert` wurde nur auf dieses Verzeichnis und die lokalen JSON-Dateien
   angewendet:

   ```sh
   go run ../../../cmd/trice insert \
     -src . -i til.json -li li.json -liRoot . \
     -IDMin 1000 -IDMax 1999 -IDMethod upward
   ```

3. Die eingefügten IDs wurden in `triceIDs/*.h` als
   `TRICE_ID_<DATEISCHLUESSEL>_L<ZEILE>` übernommen.
4. Danach wurden die numerischen IDs wieder aus den Userquellen entfernt:

   ```sh
   go run ../../../cmd/trice clean \
     -src . -i til.json -li li.json -liRoot .
   ```

Die eingecheckten Sidecars sind deshalb absichtlich simulierte Buildartefakte.
Nach Änderungen an den Zeilennummern müssen die Sidecars auf dieselbe Weise
erneuert werden.

## Library-Erweiterung

`TRICE_BIND` ist ausschließlich in dieser `triceConfig.h` aktiv. In diesem
Modus setzt das lowercase Makro `trice(...)` intern eine ID aus
`TRICE_FILE_KEY` und `__LINE__` vor die bestehende Argumentliste. Ohne
`TRICE_BIND` bleibt die bestehende explizite Schreibweise unverändert.

Der Bind-Modus behandelt den von `trice clean` auf `1` gesetzten Marker
`TRICE_CLEAN` innerhalb der Library als aktiven Codezustand. Die ID-freien
Aufrufe sind nach dem simulierten Clean-Lauf daher ausführbar und nicht wie
gewöhnlicher bereinigter Sourcecode abgeschaltet.

Jeder Sidecar wählt zunächst seinen Dateischlüssel:

```c
#undef TRICE_FILE_KEY
#define TRICE_FILE_KEY F5050505050505050

#define TRICE_ID_F5050505050505050_L42 1000u
```

Der Sidecar eines Headers ist während seiner `static inline`-Definitionen
aktiv. Nach allen normalen Includes wählt der Sidecar der `.c`-Datei ihren
eigenen Schlüssel erneut aus.
