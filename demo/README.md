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
- ein C-Compiler mit dem Kommando `cc`,
- Bash beziehungsweise eine vergleichbare POSIX-Shell.

Fehlt `trice` oder `cc`, bricht das Script vor dem Binden oder Kompilieren mit
einer verständlichen Meldung ab. Es installiert nichts automatisch.

## Direct-Demo starten

Wichtig: Zuerst in den Ordner der Demo wechseln und das Script dort starten:

```sh
cd demo/direct
./build_and_run.sh
```

## Deferred-Demo starten

Auch diese Demo wird in ihrem eigenen Ordner gestartet:

```sh
cd demo/deferred
./build_and_run.sh
```

Jede Demo besitzt ihr eigenes `build`-Verzeichnis. Darin liegen ausschließlich
erzeugte Dateien:

```text
build/
├── demo_direct oder demo_deferred
├── log.bin
└── triceIDs/
```

Unter Windows erhält der Anwendungsname zusätzlich die Endung `.exe`. Das Script
startet ihn automatisch mit dem richtigen Namen.

Nach dem Programmstart liest dasselbe Script `log.bin` mit `trice log`. `tlog`
wird für diese Demos nicht benötigt.

## Gemeinsame und lokale Dateien

`til.json` und `li.json` liegen gemeinsam in diesem Ordner. Sie enthalten die
IDs und Quellorte aller ausgeführten Demos und gehören zum Demo-Projekt. Die
kleine `SEGGER_RTT_Conf.h` wird nur benötigt, weil der bewusst einfache
Compileraufruf mit `../../src/*.c` auch die optionale RTT-Quelle erfasst.

Jeder Unterordner enthält dagegen nur seine eigene Anwendungskonfiguration:

```text
direct/ oder deferred/
├── main.c
├── triceConfig.h
├── build_and_run.sh
└── build/                 erzeugt und nicht versioniert
```

`build_and_run.sh` prüft nur, ob es aus dem richtigen Demo-Ordner gestartet
wurde, und ruft dann `../build_demo.sh` auf. Dort stehen die gemeinsamen,
ausführlich kommentierten Schritte genau einmal.
