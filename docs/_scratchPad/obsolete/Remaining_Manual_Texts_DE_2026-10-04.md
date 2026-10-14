# Remaining German Manual Texts

Original text preserved from `docs/TriceUserManual.md` on 4 October 2026, before translation. The original section heading and links below belong to that historical manual version.

+#### 24.19.1. <a id="bind-limits"></a>bind-limits

Wenn `bind` eine Schreibweise im Quellcode ablehnt, kann es die darin enthaltenen Logstellen nicht sicher zuordnen oder unterstützen. Der kurze Hinweis `Search UM for "bind-limits".` verweist auf diesen Abschnitt. Datei, Zeile und konkrete Ursache bleiben Teil der Fehlermeldung. Auch der Compilerfehler für einen benötigten, aber nicht verfügbaren `__COUNTER__` enthält diesen Verweis.

Für einen direkten Trice-Aufruf genügen normalerweise Datei und Quellzeile zur Zuordnung. Mehrere Aufrufe auf derselben Zeile oder ein Wrappermakro, hinter dem mehrere Aufrufe stehen, benötigen teilweise zusätzliche Unterstützung. Bind verwendet dafür den Compilerzähler `__COUNTER__`. Dieser zählt beim Übersetzen des Programms; er ist kein Laufzeit- oder Cycle-Counter. Nicht jeder Compiler stellt ihn bereit. Direkte, eindeutig zuordenbare Logstellen kommen ohne ihn aus.

Bei [Context Enrichment](#trice-context-enrichment) (`bind -ce`) müssen zusätzliche Werte genau an der ausgewählten Logstelle verfügbar sein. Eine Variable, die nur innerhalb einer Funktion oder eines Blocks existiert, darf nicht zusätzlich an einer fremden Logstelle verlangt werden. Die bisherige technische Umsetzung komplexer Bind-Stellen würde solche fremden Ausdrücke mitprüfen lassen. Deshalb unterstützt CE zunächst nur direkte, eindeutig über ihre Quellzeile zuordenbare Logstellen. Auch ein über mehrere Zeilen verteilter direkter Aufruf ist möglich, solange keine seiner Zeilen zugleich eine andere Bind-Logstelle enthält. Eine ausgewählte Wrapper-/Rebase-Stelle wird vor Dateiänderungen abgewiesen. CE für solche Stellen benötigt einen eigenen Architektur-Nachweis; vorhandenes `__COUNTER__` allein genügt dafür nicht. Ohne passende CE-Regel gelten weiterhin die bisherigen Bind-Fähigkeiten.

Mögliche Anpassungen sind:

- **Direkte Aufrufe auf getrennte Zeilen stellen.** Aus `trice("msg:first"); trice("msg:second");` wird:

  ```c
  trice("msg:first");
  trice("msg:second");
  ```

- **Geeignete Wrapper durch eine normale oder `static inline` Funktion ersetzen.** Jeder Trice-Aufruf steht darin auf einer eigenen Zeile. Benötigte lokale Werte werden als Parameter übergeben; eine Funktion sieht lokale Variablen ihres Aufrufers nicht automatisch. Beispiel:

  ```c
  static inline void logPosition(int x, int y)
  {
      trice("msg:x=%d, y=%d", x, y);
  }
  ```

  Der Aufrufer übergibt seine Werte mit `logPosition(pos.x, pos.y);`. Das ist nur für Wrapper geeignet, die keine besonderen Makrofunktionen benötigen. Die [Hinweise zur Umstellung auf Funktionen](#preferred-form-normal-or-static-inline-function) erklären die Unterschiede.

- **Den dauerhaft verfügbaren Workflow `insert/clean` verwenden.** `trice insert` schreibt die IDs direkt in die Logstellen; `trice clean` entfernt sie wieder. Damit entfällt die Bind-Zuordnung über Zeile oder Compilerzähler. Die Formatstrings und Aufrufe müssen weiterhin vom Trice-Parser erkannt werden können. Bei bereits gebundenen Projekten ist zuerst der [Rückweg zu `trice insert`](#re-migration-to-trice-insert) zu beachten; einzelne generierte Bind-Dateien oder Include-Zeilen dürfen nicht isoliert entfernt werden.

Automatisches `insert/clean -ce` ist ebenfalls verfügbar: Insert erweitert erkannte Source-Aufrufe einschließlich statischer Wrapperdefinitionen, Clean nimmt die erzeugten Anteile mit denselben Regeln zurück. Dieser Weg benötigt weder Bind-Zeilenzuordnung noch `__COUNTER__`. Zusätzliche Werte können weiterhin ausdrücklich im Formatstring und in den Argumenten stehen, beispielsweise `trice("msg:Wert=%d, x={x}", value, x);`. Beispiele und Rücknahmebedingungen stehen im [CE-Kapitel](#reversible-workflow-with-insert-and-clean).

## Scratch Pad Note

*) Noch nicht machen - nur mit mir diskutieren.
