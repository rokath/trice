# Minimale Trice-Demos

Diese beiden kleinen PC-Programme zeigen denselben Trice-Ausgabekanal in zwei
Betriebsarten:

- `direct`: Jeder Trice-Aufruf schreibt seine Binärdaten sofort nach
  `build/log.bin`.
- `deferred`: Die Trice-Aufrufe schreiben zunächst in einen Ringpuffer.
  `TriceTransfer()` überträgt die Daten anschließend nach `build/log.bin`.

Beide Programme verwenden unmittelbar die Trice-Quellen aus `../src`. Es wird
keine Bibliothek kopiert und kein Buildsystem benötigt.

## Voraussetzungen

Benötigt werden nur:

- `trice` im `PATH`,
- ein C-Compiler mit dem Kommando `cc` oder `gcc`,
- Bash beziehungsweise eine vergleichbare POSIX-Shell.

Die kompakten, auskommentierten Vorabprüfungen am Anfang von `demo.sh` können
bei Bedarf aktiviert werden. Das Script installiert nichts automatisch.

## Demos starten

Das zentrale Script wird aus dem Ordner `demo` gestartet:

```sh
cd demo
./demo.sh
```

Es bindet einmal beide Programme, übersetzt und startet zuerst `deferred` und
danach `direct`. Anschließend zeigt es beide Logdateien nacheinander an.

Jede Demo besitzt ihr eigenes `build`-Verzeichnis. Darin liegen ausschließlich
erzeugte Dateien:

```text
demo/build/triceIDs/       gemeinsame generierte Bind-Header
demo/deferred/build/       demo_deferred und log.bin
demo/direct/build/         demo_direct und log.bin
```

Unter Windows erhält der Anwendungsname zusätzlich die Endung `.exe`. Das Script
startet ihn automatisch mit dem richtigen Namen.

`trice bind` läuft aus dem Demo-Ordner ohne Optionen und verwendet damit direkt
`til.json`, `li.json` sowie `build/triceIDs`. Auch `trice log` läuft dort und
benötigt neben `FILEBUFFER` nur den jeweiligen Pfad zu `log.bin`. `tlog` wird
für diese Demos nicht benötigt.

Beim ersten Lauf ergänzt `trice bind` in jeder `main.c` automatisch die zunächst
ungewohnt aussehende Zeile `#include "trice_main_c_K...h"`. Der Dateiname wird
generiert und muss vom Benutzer weder geschrieben noch gepflegt werden. Der
eingebundene Header liegt anschließend unter `build/triceIDs`.

## Gemeinsame und lokale Dateien

`til.json` und `li.json` liegen gemeinsam in diesem Ordner. Sie enthalten die
IDs und Quellorte aller ausgeführten Demos und gehören zum Demo-Projekt.

Der Compiler-Ausdruck `../src/[a-z]*.c` erfasst alle regulären Trice-Quellen.
Die unbenutzte Vendor-Datei `SEGGER_RTT.c` beginnt mit einem Großbuchstaben und
bleibt deshalb außen vor. Eine Dummy-Konfiguration für SEGGER RTT ist nicht
erforderlich.

Jeder Unterordner enthält dagegen nur seine eigene Anwendungskonfiguration:

```text
direct/ oder deferred/
├── main.c
├── triceConfig.h
└── build/                 erzeugt und nicht versioniert
```

Der gemeinsame Ablauf steht ausschließlich in `demo.sh`.
