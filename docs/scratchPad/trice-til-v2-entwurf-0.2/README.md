# Paket: `til.json` Schema V2 – Entwurf 0.2

Stand: 13. Juli 2026

Dieses Paket fasst den aktuellen Diskussionsstand zusammen. Es trennt bewusst:

- veränderliche, projektspezifische Decoderwerte in `til.json` V2,
- festes Trice-Protokollwissen unter `specs/`,
- Implementierungs- und Integrationstests unter `_test/` und `examples/`.

## Dateien

- [`til-json-v2-spezifikation.md`](til-json-v2-spezifikation.md) – kompakter Spezifikationsentwurf
- [`til-json-v2-beispiel.json`](til-json-v2-beispiel.json) – vollständiges V2-Beispiel
- [`issue-til-json-v2.md`](issue-til-json-v2.md) – Implementierungs-Issue für V2 und CLI-Verhalten
- [`issue-feste-trice-spezifikationen.md`](issue-feste-trice-spezifikationen.md) – Issue für `specs/`, Typentabelle und Referenzvektoren
- [`issue-tlog-c-und-triceabc-tests.md`](issue-tlog-c-und-triceabc-tests.md) – Issue für `examples/tlog-c` und TriceABC-Verifikation

## Vorgeschlagene Zielstruktur im Repository

```text
specs/
    TREX.json
    triceTypes.json
    CountedBuffer.json
    testdata/

examples/
    TriceAbc/
    tlog-c/

_test/
    testdata/
```

Die JSON-Dateien unter `specs/` beschreiben feste Strukturen und Referenzwerte. Sie bilden keine Algorithmen wie COBS, TCOBS oder XTEA in JSON nach.

## Berücksichtigte Repository-Grundlagen

- `docs/ref/trice-help-all.txt`, Abschnitt `-log`
- `docs/TriceRx_Tlog_C_ArchitectureDesign.md`
- `_test/testdata/triceCheck.c`
- `src/triceRx.c`
- `examples/TriceAbc/ReadMe.md`
