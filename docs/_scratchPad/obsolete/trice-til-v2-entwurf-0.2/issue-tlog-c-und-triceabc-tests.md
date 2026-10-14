# Issue: `examples/tlog-c` und externe TriceABC-Verifikation

## Ziel

Ein kleines C-Hostprogramm `triceLogC` liest binäre Trice-Logdateien und erzeugt einfache ASCII-Ausgabe. Es dient als Demo, Binary→ASCII-Konverter und unabhängige Gegenprobe zum Go-Decoder.

## Projekt

```text
examples/tlog-c/
    README.md
    main.c
    build.*
    test/
```

Vorhandene RX-Komponenten, insbesondere `src/triceRx.c`, werden wiederverwendet. Es soll kein zweiter, paralleler TREX-Parser entstehen.

## Bewusster Funktionsumfang

Enthalten:

- Datei als Eingabe,
- COBS und TCOBS,
- single und multi,
- gemischte Records mit `ts0`, `ts16`, `ts32` und CountedBuffer/X0,
- normale feste Trice-Typen,
- ID-Auflösung über ein generiertes `til.c`,
- deterministische ASCII-Ausgabe.

Nicht enthalten:

- XTEA,
- serielle, RTT-, TCP- oder UDP-Quellen,
- Farben, Filter, Prefix/Suffix, Location Information,
- vollständige V2-JSON-, Profil- oder Historienlogik,
- vollständige Kompatibilität mit allen Komfortfunktionen von `trice log`.

## Testmatrix

Mindestens vier Binärlogs:

| Framing | Paketmodus |
|---|---|
| COBS | single |
| COBS | multi |
| TCOBS | single |
| TCOBS | multi |

Jedes Log enthält bunt gemischt:

- `ts0`, `ts16`, `ts32`,
- mehrere Bitbreiten und Parameterzahlen,
- `typeX0`/CountedBuffer.

Für jedes Log werden verglichen:

```text
erwartete Ausgabe
Go trice log
C triceLogC
```

Ausgaben werden nur hinsichtlich unvermeidbarer Hostdetails wie Zeilenende normalisiert.

## TriceABC-Anbindung

`examples/TriceAbc` erhält einen optionalen Test-/Capture-Pfad, der den tatsächlich erzeugten Binärstrom in eine Datei schreibt. Diese Datei kann anschließend durch

- den internen TriceABC-C-Empfangspfad,
- Go `trice log`,
- `triceLogC`

decodiert werden. So fällt auf, wenn TriceABC intern ein formal falsches Format erzeugt, das nur vom eigenen Demo-Pfad akzeptiert wird.

BcSim bleibt unverändert protokollneutral.

## Akzeptanz

- `triceLogC` baut ohne Go-Laufzeit und ohne JSON-Bibliothek.
- Alle vier Kernlogs werden korrekt decodiert.
- Go- und C-Ausgabe stimmen mit der Referenzausgabe überein.
- Mindestens ein TriceABC-Lauf wird extern durch Go und C gegengeprüft.
- Fehlende oder beschädigte Frames führen zu einem klaren Fehler, nicht zu stiller Falschausgabe.
