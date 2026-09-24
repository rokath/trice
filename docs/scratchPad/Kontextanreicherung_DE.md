# Context Enrichment

**Planungsstand:** Aktueller deutscher Entwurf für M20, noch nicht implementiert. Die für den aktuellen Scope nötigen Designentscheidungen sind abgeschlossen. M20 unterstützt zunächst ausschließlich `trice bind -ce`. Eine spätere Erweiterung auf `insert/clean` bleibt unter einem strikten reversiblen Vertrag möglich; siehe Anhang.

## 1. Einordnung

Trice führt für M20 keinen allgemeinen Runtime-Context mit Push/Pop, Task-local State oder Context-Handles ein. Stattdessen kann `bind` ausgewählte Trice-Aufrufe beim Build gezielt um zusätzliche Runtime-Werte instrumentieren.

Damit bleibt jeder Record vollständig und es entsteht kein impliziter Context-Zustand.

## 2. Grundsyntax

Eine Trice-Meldung kann statische Selektorpräfixe enthalten:

```c
trice("MSG:ctx7: hi\n");
```

Eine Bind-Option definiert für einen Selektor zusätzliche Darstellung und Runtime-Ausdrücke:

```text
trice bind -ce 'ctx7:", pos={x},{y}", x, y'
```

Die allgemeine Syntax lautet:

```text
-ce 'selector:"format-extension"[, <C-expression>]...'
```

Die Erweiterung wird vor einem abschließenden `\n` eingefügt, andernfalls am Ende angehängt.

Logisch kann daraus werden:

```c
trice("MSG: hi, pos={x},{y}\n", x, y);
```

ohne dass der User-Sourcecode durch `bind` geändert wird.

## 3. C-Ausdrücke in `-ce`

Der CE-Parser bleibt bewusst einfach. Jeder nach dem Formatstring angegebene C-Ausdruck muss selbst **kommafrei** sein. Dadurch kann die CLI-Liste eindeutig an Kommata getrennt werden, ohne einen vollständigen C-Parser einzuführen.

Erlaubte Beispiele:

```text
x
motor.speed
motor->speed
array[i]
&buffer
*x
x + 1
aFloat(temp)
aDouble(value)
condition ? a : b
```

Nicht direkt erlaubt sind beispielsweise Ausdrücke mit eigenem Komma:

```text
getValue(a, b)
(a++, a)
```

Bei Bedarf wird das Ergebnis vorher in eine lokale Variable gelegt und diese Variable an `-ce` übergeben.

Die Ausdrücke müssen an jeder ausgewählten Logstelle sichtbar und kompilierbar sein.

## 4. Selektoren

### 4.1 Nur konfigurierte Selektoren haben CE-Bedeutung

Ein Präfix wird nur dann als CE-Selektor behandelt, wenn für ihn im aktuellen Lauf eine passende `-ce`-Regel existiert.

Ohne passende Regel bleibt der Text vollständig normaler Logtext:

```c
trice("msg:ctx7:hi");
trice("ctx7:ho");
```

ohne `-ce 'ctx7:...'` ergibt weiterhin sichtbar:

```text
msg:ctx7:hi
ctx7:ho
```

Damit verschwinden keine Logteile nur deshalb, weil eine Zeichenfolge zufällig wie ein möglicher Context-Selektor aussieht.

### 4.2 Case und Aliase

Selektoren werden case-neutral verglichen. `ctx7`, `Ctx7` und `CTX7` treffen dieselbe Regel.

Nur die vollständig kleingeschriebene Schreibweise wird bei aktiver Regel aus dem sichtbaren Meldungstext entfernt:

```c
trice("MSG:ctx7: hi\n");  // ctx7 wird entfernt
trice("MSG:Ctx7: hi\n");  // Ctx7 bleibt sichtbar, löst CE aber ebenfalls aus
```

Bekannte Trice-Tags können ebenfalls als CE-Selektoren dienen. Bei eingebauten Tag-Aliasen wird die vorhandene Alias-Gruppe verwendet. Freie CE-Selektoren besitzen keine zusätzliche Alias-Liste und werden nur case-neutral verglichen.

`-ulabel` ist orthogonal zu `-ce`. Derselbe Name darf gleichzeitig User-Label/Tag und CE-Selektor sein. Die Tag-/Label-Metadaten werden aus dem ursprünglichen Präfix bestimmt und gehen durch die anschließende CE-Transformation nicht verloren.

## 5. Mehrere Selektoren und mehrere Regeln

Verschiedene Selektoren werden in ihrer Reihenfolge im Source ausgewertet.

```c
trice("msg:ctx7:ctx8: hi");
```

mit Regeln für `ctx7` und `ctx8` wendet zuerst die `ctx7`-, dann die `ctx8`-Erweiterung an.

Mehrere `-ce`-Regeln für denselben Selektor sind zulässig und werden in CLI-Reihenfolge angewendet.

```text
-ce 'ctx7:", x={x}", x'
-ce 'ctx7:", y={y}", y'
```

Ein Selektor, der an derselben Logstelle mehrfach vorkommt, wird nur einmal wirksam; `bind` warnt über das Duplikat.

## 6. Zusammenspiel mit Structured Logging

Der `-ce`-Formatstring verwendet exakt denselben Formatparser wie M19. Klassische `%...`-Platzhalter und strukturierte `{...}`-Platzhalter dürfen gemischt werden.

```text
-ce 'ctx7:", pos=%d,{position_y}", x, y'
```

Beide Platzhalterarten konsumieren CE-Ausdrücke in ihrer Reihenfolge; nur `{...}` erzeugt zusätzlich ein strukturiertes Feld. Literale geschweifte Klammern werden wie in M19 als `{{` und `}}` geschrieben.

Auch `{}` ist möglich, wenn der Feldname eindeutig aus dem einfachen CE-Ausdruck ableitbar ist:

```text
-ce 'ctx7:", x={}", x'
```

Bei einem Ausdruck ohne eindeutige Namensableitung ist ein expliziter Feldname erforderlich.

Nach der CE-Erweiterung durchläuft der resultierende Record die normale M19-Kanonisierung. CE-Felder und direkt im Source geschriebene Felder bilden dasselbe flache Feldschema.

Doppelte kanonische Feldnamen innerhalb des finalen Records sind wie in M19 nicht erlaubt und führen zu einem Fehler.

`trice-fields.txt` zählt CE-Felder genauso wie direkt geschriebene Structured-Logging-Felder.

## 7. Bind-Mechanismus: Runtime-Ausdrücke am ursprünglichen Callsite

`til.json` allein kann CE nicht implementieren. Der CE-erweiterte Template-String ist Host-/Wörterbuchinformation; die zusätzlich konfigurierten C-Ausdrücke müssen auf dem Target tatsächlich ausgewertet und übertragen werden.

Beispiel:

```c
void f(int x) {
    trice("msg:hi");
}
```

mit:

```text
-ce 'msg:", x={}", x'
```

Der User-Source bleibt unverändert. `bind` muss jedoch über sein generiertes Sidecar-/Makro-Artefakt bewirken, dass die effektive Compilerexpansion an genau dieser Logstelle zusätzlich den lokalen Ausdruck `x` verwendet und die passende Trice-Arity auswählt. Logisch entspricht dies einem Aufruf mit einem zusätzlichen Runtime-Wert.

Wichtig ist die Trennung:

- `til.json` enthält den finalen kanonischen CE-erweiterten Template-String;
- der Target-Build benötigt die dazu passenden zusätzlichen Runtime-Werte;
- der CE-erweiterte Text selbst muss nicht als Runtime-String auf das Target gelangen;
- die injizierten C-Ausdrücke müssen im lexikalischen Scope der ursprünglichen `trice(...)`-Logstelle ausgewertet werden;
- jeder injizierte Ausdruck darf pro tatsächlich ausgeführtem Trice-Aufruf genau einmal ausgewertet werden.

Damit muss `bind` bei CE nicht nur IDs binden, sondern zusätzlich eine callsite-spezifische Argumenterweiterung erzeugen. Bestehende User-Argumente bleiben in ihrer Reihenfolge erhalten; CE-Argumente werden entsprechend der angewendeten CE-Regeln ergänzt.

### 7.1 Verbindlicher Machbarkeitsnachweis vor M20

Vor der eigentlichen M20-Implementierung muss ein isolierter PoC nachweisen, dass diese Callsite-Injektion mit dem Bind-Sidecar technisch sauber funktioniert. Maßgeblich ist [Issue_M20_Bind_CE_Callsite_Injection_PoC.md](Issue_M20_Bind_CE_Callsite_Injection_PoC.md).

Der PoC muss mindestens zeigen:

- zusätzlicher lokaler Wert bei einem bisher argumentlosen Trice;
- Ergänzung eines CE-Werts hinter bereits vorhandenen Trice-Argumenten;
- einfacher injizierter Ausdruck wie `x + 1`;
- exakt einmalige Auswertung, auch bei einem Testausdruck mit beobachtbarem Seiteneffekt;
- korrekte finale Trice-Arity und Übereinstimmung zwischen übertragenen Werten und `til.json`;
- unveränderten User-Source und idempotente wiederholte Bind-Läufe.

Zusätzlich darf der Mechanismus keine CE-bedingten False-Positive-Warnungen in üblichen compilerbewussten C/C++-Editoren bzw. Language-Servern erzeugen. Es ist zulässig, dass diese dafür das generierte Build-Verzeichnis und die reale Compile-Konfiguration kennen müssen. Nicht akzeptabel wäre ein Design, das zwar vom Build-Compiler angenommen wird, im normalen Editor aber systematisch falsche Argumentzahl-, Syntax- oder Identifierfehler anzeigt.

Die erfolgreiche Abarbeitung dieses PoC ist eine Implementierungsvoraussetzung für M20. Schlägt der Nachweis fehl, ist die Bind/CE-Architektur vor weiterer M20-Arbeit neu zu bewerten.

## 8. Schema, `til.json` und IDs

CE wird vor Schema- und ID-Bestimmung angewendet.

Der User-Source bleibt bei `bind` unverändert, aber `til.json` enthält für den jeweiligen Build den finalen kanonischen, CE-erweiterten Template-String im bestehenden `Strg`-Feld.

Beispiel Source:

```c
trice("MSG:ctx7:Speed={speed}\n", speed);
```

mit:

```text
-ce 'ctx7:", Pos={position}", position'
```

führt logisch zu einem kanonischen Record wie:

```c
trice("MSG:Speed={speed}, Pos={position}\n", speed, position);
```

und entsprechend zu einem CE-erweiterten `Strg` in `til.json`.

Ändert die aktive CE-Konfiguration den finalen Template-String, die Argumente oder die strukturierten Feldnamen, ergibt sich ein anderes Schema und damit eine andere ID. Ohne passende `-ce`-Regel entstehen weder zusätzliche Target-Werte noch eine CE-Transformation.

Gleicher Source plus gleiche CE-Konfiguration muss bei wiederholtem `bind` dasselbe kanonische Ergebnis und dieselbe ID-Zuordnung ergeben.

## 9. Fehlervertrag

Folgende Fälle sind Fehler und müssen vor einem inkonsistenten Schreibzustand scheitern:

- Anzahl Format-Platzhalter und CE-Ausdrücke passt nicht zusammen.
- Durch CE entstehen doppelte strukturierte Feldnamen im finalen Record.
- Die resultierende Trice-Arity oder Makrofamilie ist nicht unterstützt.
- Eine nicht unterstützte Bitbreiten-/Wrapper-Kombination würde entstehen.
- Die CE-Regel selbst ist syntaktisch ungültig.

Ein C-Ausdruck, der an der ausgewählten Logstelle semantisch ungültig oder nicht sichtbar ist, führt spätestens beim Compiler zu einem klaren Buildfehler.

Jeder CE-Ausdruck darf pro tatsächlichem Trice-Aufruf genau einmal ausgewertet werden. Ausdrücke mit Seiteneffekten dürfen durch die Bind-Instrumentierung nicht dupliziert werden.

## 10. Aktueller Scope: nur `bind -ce`

M20 implementiert CE zunächst ausschließlich für `bind`.

Der Grund ist nicht eine grundsätzliche Unmöglichkeit von `insert/clean`, sondern der derzeit nicht gerechtfertigte Zusatzaufwand für eine vollständig reversible Source-Transformation mit zusätzlichen Runtime-Ausdrücken.

M19 Structured Logging selbst bleibt davon getrennt und soll weiterhin `bind` sowie `insert/clean` unterstützen.

## 11. Anhang: mögliche spätere CE-Unterstützung für `insert/clean`

CE kann später auch für `insert/clean` ergänzt werden, wenn ein strikter Transformationsvertrag eingehalten wird.

Mindestens gelten dann:

- `insert` und `clean` werden mit identischen `-ce`-Optionen in identischer Reihenfolge ausgeführt.
- Die CE-Transformation ist vollständig deterministisch.
- Die durch CE erzeugten Formatstring-Anteile und Runtime-Argumente sind eindeutig rekonstruierbar.
- `insert` ist idempotent:

```text
insert(insert(S, CE), CE) = insert(S, CE)
```

- `clean` ist idempotent:

```text
clean(clean(S, CE), CE) = clean(S, CE)
```

- Die zentrale Rücknahmebedingung lautet:

```text
clean(insert(S, CE), CE) = S
```

Diese Garantie gilt nur, wenn Source, CE-Konfiguration und Transformationsregeln zwischen `insert` und `clean` nicht manuell verändert wurden.

Diese spätere Erweiterung ist ausdrücklich nicht Teil des aktuellen M20-Implementierungsumfangs.

## 12. Charakter des Ansatzes

`-ce` ist keine allgemeine Context-Vererbung, sondern eine optionale statisch ausgewählte Build-Time-Instrumentierung mit Runtime-Werten. Dadurch entstehen weder globaler Context-State noch besondere Probleme durch Taskwechsel oder Interrupts. Gleichzeitig kann zusätzliche Diagnoseinformation für bestimmte Builds aktiviert werden, ohne die User-Logstellen einzeln zu ändern.

## 13. Referenzen

- Go `slog.Logger.With`: https://pkg.go.dev/log/slog
- Microsoft `ILogger.BeginScope`: https://learn.microsoft.com/dotnet/api/microsoft.extensions.logging.ilogger.beginscope
- Serilog `LogContext`: https://github.com/serilog/serilog/wiki/Enrichment
- Rust `tracing` spans: https://docs.rs/tracing/latest/tracing/span/
