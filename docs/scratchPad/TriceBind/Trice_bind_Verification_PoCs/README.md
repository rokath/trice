# Trice Bind – Verifikations-PoCs

Dieses Paket enthält zwei voneinander unabhängige Präprozessor-PoCs für die beiden vor der Bind-Spezifikation V4 zu verifizierenden Kernmechanismen.

## Inhalt

### `poc1_local_dispatch`

Verifiziert den lokalen Insert-/Bind-Dispatch über `TRICE_BIND_FILE_KEY`.

Nachgewiesen wird:

- `trice.h` kann vor dem Sidecar eingelesen werden.
- Ohne definierten `TRICE_BIND_FILE_KEY` wird der bestehende Inserted-Pfad gewählt.
- Ein später inkludierter Sidecar schaltet die nachfolgenden Aufrufe bei der Makroexpansion auf den Bind-Pfad.
- `#undef TRICE_BIND_FILE_KEY` schaltet in einem seltenen Hybridfall wieder auf den Inserted-Pfad zurück.
- Ein beim Einlesen von `trice.h` ausgewertetes `#ifdef TRICE_BIND_FILE_KEY` ist nicht erforderlich.

Der PoC verwendet einen tokenbasierten Expansion-Time-Dispatch. Die Laufzeitausgabe und die Präprozessorausgabe werden geprüft.

### `poc2_site_descriptor`

Verifiziert einen einzigen einzeiligen Site-Deskriptor pro Trice-Stelle.

Nachgewiesen wird:

- ID-freier Uppercase-Aufruf mit Default-Stamp:
  `TRICE("Hello")`
- Null-Platzhalter mit allen drei Legacy-Stampformen:
  `id(0)`, `Id(0)` und `ID(0)`
- ID-freier Lowercase-Aufruf mit `iD(...)`
- Aufrufe ohne und mit Datenargumenten
- Ersetzen eines vorhandenen Null-Platzhalters ohne zweite `HAS_TID`-Definition

Ein Sidecar-Eintrag hat die Form:

```c
#define TRICE_BIND_SITE_K73A915E9C4021B8_L101 TRICE_BIND_REPLACE, id(12346u) // TRICE(id(0), "world");
```

Der Deskriptor enthält damit in genau einer Zeile:

1. `TRICE_BIND_AUTO` oder `TRICE_BIND_REPLACE`,
2. den vollständigen TID-Ausdruck.

## Ausführen

Alle verfügbaren GCC-/Clang-C- und C++-Frontends prüfen:

```sh
./run_all.sh
```

Einen einzelnen PoC mit dem standardmäßigen C-Compiler prüfen:

```sh
cd poc1_local_dispatch
./run.sh
```

oder:

```sh
cd poc2_site_descriptor
./run.sh
```

Ein bestimmter C-Compiler kann über `CC` gewählt werden:

```sh
CC=clang ./run.sh
```

## Abgrenzung

Die PoCs verwenden eine minimale simulierte Trice-Library. Sie verifizieren ausschließlich die C/C++-Präprozessormechanik. Sie sind noch keine Integration in die vollständige Trice-Makromatrix und kein Ersatz für die späteren Repositorytests.
