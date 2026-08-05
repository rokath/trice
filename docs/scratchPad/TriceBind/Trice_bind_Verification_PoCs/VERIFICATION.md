# Verifikationsstatus

Die beiden PoCs wurden in der Erstellungsumgebung erfolgreich getestet mit:

- GCC 14.2.0 als C11-Compiler
- Clang 17.0.0 als C11-Compiler
- G++ 14.2.0 als C++17-Compiler
- Clang++ 17.0.0 als C++17-Compiler

Verwendete Warnoptionen:

```text
-Wall -Wextra -Werror -pedantic
```

Zusätzlich vergleichen die C-Testskripte die erzeugte Laufzeitausgabe mit `expected.txt` und zeigen die relevanten vollständig präprozessierten Aufrufe an.

## Ergebnis

- PoC 1: lokaler Expansion-Time-Dispatch erfolgreich
- PoC 2: einzeiliger Site-Deskriptor für `AUTO` und `REPLACE` erfolgreich
- C11 und C++17: erfolgreich
- GCC und Clang: erfolgreich
