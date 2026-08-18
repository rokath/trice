# PoC: lokaler `__COUNTER__`-Rebase für `trice bind`

Dieses ausführbare PoC konkretisiert Variante 1 aus
[`Trice_bind_60_MVP2_Implementation_Strategies.md`](../../../docs/TriceBind/Trice_bind_60_MVP2_Implementation_Strategies.md).
Es verändert weder den aktuellen Generator noch Dateien in [`src`](../../../src/).
Der produktive Dispatch aus [`triceBind.h`](../../../src/triceBind.h) wird jedoch
direkt eingebunden.

## Sicht des Anwenders

Der Anwender definiert ein gewöhnliches Statement-Makro und ruft es ohne ID,
Counterwert oder neue Kommandozeilenoption auf:

```c
#define LOG_ERROR(value)                                      \
    do {                                                      \
        switch (value) {                                      \
        case 0:                                               \
            break;                                            \
        case 7:                                               \
            trice("cannot open file\n");                     \
            break;                                            \
        default:                                              \
            trice("error=%d", 8);                             \
            break;                                            \
        }                                                     \
    } while (0)

LOG_ERROR(0);   // no log record
LOG_ERROR(7);   // stable ID 6101
LOG_ERROR(99);  // stable ID 6102
```

`main.c` enthält außerdem zwei Wrapperaufrufe auf derselben physischen Zeile:

```c
LOG_ERROR(7); LOG_ERROR(99);
```

Damit prüft ein Segment vier Präprozessor-Expansionen: zwei Trice-Stellen pro
Wrapper mal zwei Wrapperaufrufe. Die Definitions-IDs bleiben dabei 6101 und
6102; sie werden nicht pro Wrapperaufruf neu vergeben.

## Was `trice bind` im PoC simuliert

Ein künftiger Generator würde um jede erweiterte Stelle automatisch einen
lokalen Bereich legen. Der Anwender müsste diese Zeilen weder schreiben noch
pflegen:

```c
#define TRICE_BIND_REBASE_SCOPE S_CASE7
#define TRICE_BIND_REBASE_BEGIN
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_BEGIN

LOG_ERROR(7);

#define TRICE_BIND_REBASE_END
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_END
```

Der BEGIN-Helper merkt sich den aktuellen `__COUNTER__`-Stand. Jede folgende
Trice-Expansion verwendet nur ihre Differenz zu diesem lokalen Startwert. Die
absoluten Werte dürfen sich daher durch Header oder fremde Makros vor dem
Bereich ändern. `main.c` verbraucht absichtlich unterschiedlich viele
Counterwerte vor jedem Bereich und beweist dieses Verhalten zur Laufzeit.

Der generierte Helper schaltet den bestehenden file-key-basierten Dispatch nur
innerhalb des Bereichs von `BIND` auf `REBASE`. Die lokalen Ordinale werden mit
generierten konstanten Ausdrücken auf stabile IDs abgebildet. Es entstehen
keine veränderlichen Tabellen, keine dynamische Speicherbelegung und kein
globaler Laufzeitzustand.

## Robuste Fehler statt stiller Fehlzuordnung

Ein lokaler Start allein wäre nicht ausreichend: Ein fremdes `__COUNTER__`
zwischen BEGIN und END könnte alle nachfolgenden IDs verschieben. Dieses PoC
prüft deshalb zwei Invarianten bereits beim Compilieren:

1. Jede einzelne Trice-Expansion muss innerhalb des erzeugten Ordinalbereichs
   liegen.
2. Am END muss exakt die erwartete Anzahl Counterwerte verbraucht worden sein.

Schon ein absichtlich eingeschobener fremder Counter führt dadurch zu einer
`static_assert`- beziehungsweise `_Static_assert`-Diagnose. Es wird niemals
still eine verschobene reguläre ID akzeptiert. Fehlt `__COUNTER__`, erzeugt der
Helper eine gezielte `#error`-Meldung mit den Auswegen `trice insert/clean` oder
einer Anpassung des Makros.

## Ausführen

```sh
cd experiments/TriceBind/60_MVP2_Local_Counter_Rebase
./run.sh
```

Mit `CC` und `CXX` kann jeweils ein Compiler gewählt werden:

```sh
CC=clang CXX=clang++ ./run.sh
```

Das Skript prüft mit allen gefundenen GCC-/Clang-kompatiblen C- und
C++-Compilern:

- den positiven Lauf bei `-O0` und `-O2`;
- die stabilen IDs bei `LOG_ERROR(0)`, `LOG_ERROR(7)` und einem Defaultwert;
- zwei `LOG_ERROR`-Aufrufe auf derselben Zeile;
- den erwarteten Compilefehler bei zusätzlichem Counterverbrauch;
- den erwarteten Compilefehler bei simuliert fehlendem `__COUNTER__`.

## Aussage und bewusste Grenzen

Das PoC belegt den lokalen Rebase für gewöhnliche Statement-Kontexte, das oben
gezeigte mehrzeilige Wrappermakro, mehrere Wrapperaufrufe auf einer Zeile, C11
und C++17. Es belegt außerdem, dass fremde Counterverbräuche **vor** einem
Bereich unschädlich sind und Verbräuche **innerhalb** des Bereichs erkannt
werden.

Es implementiert noch keinen Generator. Ein produktives `trice bind` müsste
die Bereiche automatisch erkennen, erzeugen, bei erneuten Läufen aktualisieren
und transaktional schreiben. Zunächst weiterhin zurückzuweisen wären
verschachtelte Logging-Wrapper, Token-Pasting, Stringification, dynamische
Formatkonstruktion und Sourcekontexte, in denen vor einer Anweisung keine
Deklaration zulässig ist. Diese Grenzen sollen klar diagnostiziert werden;
`trice insert`/`trice clean` bleibt der universelle Fallback.
