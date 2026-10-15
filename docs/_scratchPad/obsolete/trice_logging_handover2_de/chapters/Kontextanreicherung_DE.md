## Context Enrichment

### Einordnung

Etablierte Logging-Systeme reichern Logs meist über abgeleitete Logger mit festen Attributen, über logische Scopes oder über Spans an. Beispiele sind Go `slog.Logger.With`, .NET `BeginScope`, Serilog `LogContext` und Rust `tracing`-Spans.

Für Trice wird vorerst kein allgemeiner Runtime-Context mit Push/Pop, Task-local State oder Context-Handles vorgesehen. Stattdessen kann `bind` ausgewählte Trice-Aufrufe beim Build gezielt um zusätzliche Runtime-Werte instrumentieren. Damit bleibt jeder Record vollständig und es entsteht kein impliziter Context-Zustand.

### Selektive Context-Erweiterung mit `-ce`

Eine Trice-Meldung kann statische Context-Selektoren enthalten:

```c
trice("MSG:ctx7: hi\n");
```

Eine Bind-Option kann für einen Selektor zusätzliche Darstellung und C-Ausdrücke festlegen:

```text
trice bind -ce 'ctx7:", pos=%d,%d",x,y'
```

Der effektive Record entspricht dann logisch

```c
trice("MSG: hi, pos=%d,%d\n", x, y);
```

ohne dass der User-Sourcecode bei `bind` geändert wird. Die Erweiterung wird vor einem abschließenden `\n` eingefügt, andernfalls am Ende angehängt. Treffende Erweiterungen werden in Selektorreihenfolge angewendet.

`x`, `y` dürfen gültige C-Ausdrücke sein, beispielsweise auch:

```text
-ce 'ctx7:", x=%d",getX()'
```

Sie müssen an jeder ausgewählten Logstelle sichtbar und kompilierbar sein.

### Selektoren

Selektoren werden **case-neutral** verglichen. `ctx7`, `Ctx7` und `CTX7` wählen somit dieselbe `-ce`-Regel.

Nur die vollständig kleingeschriebene Schreibweise wird aus dem sichtbaren Meldungstext entfernt. Damit gilt beispielsweise:

```c
trice("MSG:ctx7: hi\n");  // ctx7 wird entfernt
trice("MSG:Ctx7: hi\n");  // Ctx7 bleibt sichtbar
```

Normale Trice-Tags können ebenfalls als `-ce`-Selektoren dienen. Trifft ein `-ce`-Selektor einen fest eingebauten Trice-Tag oder einen seiner Aliase, wird die vorhandene Alias-Gruppe verwendet; `msg` und `message` sind damit beispielsweise gleichwertig. Freie CE-Selektoren besitzen keine zusätzliche Alias-Liste und werden nur case-neutral verglichen. Nur der erste Präfix kann zugleich als normaler Trice-Tag interpretiert werden; danach können mehrere Context-Selektoren folgen:

```text
[tag-or-ce:][ce:][ce:]...message
```

Die bestehende Tag-Behandlung bleibt unverändert. `-ulabel` ist dazu orthogonal und kann denselben statischen Selektor unabhängig mit Label-Metadaten versehen. Für bekannte Trice-Tags wirkt `-ulabel` ebenfalls auf die feste Alias-Gruppe; freie User-Labels bleiben dagegen literal, beispielsweise `new` und `NEW` als unterschiedliche Labels.

### Zusammenspiel mit Structured Logging

Der `-ce`-Formatstring verwendet denselben Formatparser wie `trice()`. Klassische `%...`-Platzhalter und strukturierte `{...}`-Platzhalter dürfen gemischt werden:

```text
-ce 'ctx7:", pos=%d,{position.y}",x,y'
```

Beide konsumieren C-Ausdrücke in ihrer Reihenfolge; nur `{...}` erzeugt zusätzlich strukturierte Feldmetadaten. Literale geschweifte Klammern werden wie im normalen Formatstring als `{{` und `}}` geschrieben.

Nach der CE-Erweiterung durchläuft der resultierende Record die normale Structured-Logging-Verarbeitung. Feldnamen, Hierarchie, Registry und ID-Vergabe folgen damit denselben Regeln wie bei direkt im Source geschriebenen Feldern. CE ist damit nur eine vorgelagerte Build-Time-Transformation; es entsteht keine zweite Format- oder Target-API.

Auch die Kurzform `{}` ist möglich, wenn der Feldname eindeutig aus dem C-Ausdruck ableitbar ist:

```text
-ce 'ctx7:", x={}",x'
```

Bei einem allgemeinen Ausdruck wie `getX()` ist dagegen ein expliziter Name erforderlich:

```text
-ce 'ctx7:", x={x}",getX()'
```

Memberzugriffe sind zulässig und werden wie üblich kanonisiert:

```text
motor.x       -> motor.x
motor->x      -> motor.x
```

Ein komplettes C-Struct wird nicht automatisch serialisiert; benötigte Member werden einzeln angegeben.

Damit kollidiert CE nicht mit Structured Logging: injizierte und im Source vorhandene Felder landen im selben flachen Feldschema. Doppelte Feldnamen bleiben zulässig und können von `bind` lediglich gewarnt werden.

### Schema und IDs

Die CE-Erweiterung erfolgt vor der Schema- und ID-Bestimmung. Ändert `-ce` den effektiven Formatstring, die Argumente oder strukturierte Feldnamen, entsteht entsprechend eine andere Trice-ID. Ohne passende `-ce`-Option entstehen keine zusätzlichen Target-Werte.

Doppelte strukturierte Feldnamen sind zulässig. `bind` kann davor warnen, soll sie aber nicht verbieten. Hierarchische Namen wie `motor` und `motor.temperature` sind ebenfalls zulässig, da Trice sie zunächst als flache kanonische Feldnamen behandelt. Eine spätere Umwandlung in verschachtelte JSON-Objekte ist Aufgabe des jeweiligen Output-Renderers.

### Charakter des Ansatzes

`-ce` ist keine allgemeine Context-Vererbung, sondern eine optionale, statisch ausgewählte Instrumentierung mit Runtime-Werten. Dadurch entstehen weder globaler Context-State noch besondere Probleme durch Taskwechsel oder Interrupts. Gleichzeitig kann zusätzliche Diagnoseinformation gezielt für bestimmte Builds aktiviert werden, ohne die betreffenden User-Logstellen einzeln zu ändern.

### Referenzen

- Go `slog.Logger.With`: https://pkg.go.dev/log/slog
- Microsoft `ILogger.BeginScope`: https://learn.microsoft.com/dotnet/api/microsoft.extensions.logging.ilogger.beginscope
- Serilog `LogContext`: https://github.com/serilog/serilog/wiki/Enrichment
- Rust `tracing` spans: https://docs.rs/tracing/latest/tracing/span/
