# Issue: Feste Trice-Spezifikationen und gemeinsame Referenzvektoren

## Ziel

Festes Trice-Wissen wird aus der projektbezogenen `til.json` herausgehalten und unter `specs/` einmalig, lesbar und testbar beschrieben.

## Zielstruktur

```text
specs/
    TREX.json
    triceTypes.json
    CountedBuffer.json
    testdata/
```

## Aufgaben

### `triceTypes.json`

Eine vollständige, eingecheckte Tabelle aller gültigen TIL-Typnamen erstellen. Jeder Eintrag enthält mindestens:

- Typart,
- feste Bitbreite oder Verweis auf `DefaultTRICEBitWidth`,
- Parameteranzahl oder spezielle Payload-Semantik.

Die Tabelle darf lang sein; sie soll ohne Pattern-Interpreter lesbar sein. Sie kann durch einen Build-Time-Generator expandiert werden, wird aber vollständig eingecheckt. Die initiale Fassung wird aus `internal/`, `src/` und `_test/testdata/triceCheck.c` abgeglichen. Abweichungen werden nicht automatisch zugunsten einer Seite aufgelöst, sondern anhand bytegenauer Referenzfälle und bewusster Entscheidung geklärt.

### `TREX.json`

Nur feste deklarative Record- und Bitfeldfakten aufnehmen. Keine Nachbildung von COBS, TCOBS, XTEA oder des vollständigen Decoderablaufs in JSON.

### `CountedBuffer.json`

Das feste Selector-0-Format festhalten:

- 16-Bit-Feld,
- untere 14 Bit sind die Anzahl der folgenden Nutzbytes,
- Maske `0x3FFF`,
- obere zwei Selector-Bits sind `0`.

### Referenzvektoren

Kleine, unabhängig prüfbare Vektoren unter `specs/testdata` anlegen für:

- TREX-Records,
- COBS,
- TCOBS,
- XTEA, soweit genutzt,
- CountedBuffer.

Go- und C-Tests verwenden dieselben Dateien. Große Regressionstests bleiben unter `_test/testdata`.

## Normative Rolle

Während der Ersterstellung sind Go- und C-Code die vorhandenen Implementierungsreferenzen. Nach Prüfung und Freigabe werden die eingecheckten Dateien unter `specs/` normativ; Go und C müssen mit ihnen übereinstimmen.

## Akzeptanz

- Jeder in TIL-Dateien erzeugbare `Type` ist in `triceTypes.json` enthalten.
- Bitbreite und Parameteranzahl sind ohne Namensheuristik bestimmbar.
- Go und C bestehen dieselben Referenzvektoren.
- TCOBS und TCOBSv1 sind als Synonyme geprüft; kanonischer Name ist `TCOBSv1`.
- CountedBuffer-Grenzfälle für 0, 1, `0x3FFE` und `0x3FFF` Bytes sind abgedeckt, soweit praktisch ausführbar.
