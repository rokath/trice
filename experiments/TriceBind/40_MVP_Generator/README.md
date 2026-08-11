# PoC_bind_generator

Dieser PoC verwendet den echten `trice bind`-Generator und kompiliert die
Repository-Library direkt aus `../../../src`. Die Trice-Aufrufe bleiben in den
Userquellen ID-frei; nur die einmal erzeugten Sidecar-Includes sowie `til.json`
und `li.json` sind dauerhafte Projektdateien.

Geprüft werden zwei gleichnamige `dev.c`/`dev.h`-Paare in unterschiedlichen
Verzeichnistiefen, `static inline`-Funktionen in beiden Headern, identische und
unterschiedliche Logzeilen sowie normale Funktionsaufrufe aus `main.c`.
Zusätzlich enthalten die Quellen Lower-/Mixed-Case-, Uppercase-, String- und
Buffer-Makros sowie einen expliziten Null-Platzhalter.

## Generieren, bauen und ausführen

Vom PoC-Verzeichnis aus:

```sh
cmake -S . -B build
cmake --build build
./build/PoC_bind_generator
```

Das CMake-Target führt `trice bind` vor jedem Build aus. Beim ersten Lauf fügt
der Generator die stabilen Include-Zeilen in die fünf physischen Userdateien
ein. Die Sidecars selbst entstehen unter `build/triceIDs` und sind ignorierte
Buildartefakte. Das Programm legt `log.bin` bei jedem Start neu an.

Die lokale Binärdatei lässt sich mit den lokalen Listen decodieren:

```sh
tlog -p FILEBUFFER -args log.bin -pf TCOBS -d16 -prefix off -hs off \
  -i til.json -li li.json
```

Alternativ:

```sh
go run ../../../cmd/trice log -p FILEBUFFER -args log.bin -pf TCOBS -d16 \
  -prefix off -hs off -i til.json -li li.json
```

Nach jeder Änderung an einer gescannten `.c`- oder `.h`-Datei muss `trice bind`
vor dem Build erneut laufen. Ein veralteter, aber noch kompilierbarer Sidecar
ist kein gültiger Aktualitätsnachweis.
