## 33. <a id="trice-context-enrichment"></a>Trice Context Enrichment

Context Enrichment (CE) ergänzt ausgewählte Trice-Meldungen um zusätzliche Werte, etwa Task-Kontext, Position oder Betriebszustand. Eine CLI-Regel gilt für alle Logstellen mit dem passenden Selektorpräfix. So lässt sich zusätzliche Diagnoseinformation für einen Build einschalten, ohne jede Logstelle von Hand zu erweitern.

Die wiederholbare Option lautet bei `bind`, `insert` und `clean` gleich:

```text
-ce 'selector:"format-extension"[, comma-free C-expression]...'
```

Beispielsweise ergänzt `-ce 'ctx7:", clock={}", clock'` die Meldung `trice("msg:ctx7:hi\n");` um den an dieser Stelle gültigen Wert von `clock`. Bei `clock == 42` lautet der Meldungsteil mit `-color none`: `hi, clock=42`. Das freie `ctx7:` bleibt als Auswahlmerkmal im Source Code erhalten. Es verschwindet aus dem endgültigen Log wenn es nur Kleinbuchstaben enthält. Das abschließende `\n` bleibt hinter dem angehängten Wert. (*Hinweis: `{}` kann in diesem Beispiel auch `%d` oder `%08x` sein. Die geschweifte Klammer zeigt lediglich, dass [Structured Logging](#strukturiertes-logging) und Kontext Enrichment orthogonal sind, also unabhängig voneinander und gemischt verwendet werden dürfen.*)

| Befehl               | Wirkung                                                                                                                                             |
|----------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------|
| `trice bind -ce …`   | Ergänzt generierte Sidecars; die Trice-Aufrufe selbst bleiben unverändert. Unterstützt direkte, eindeutig über ihre Quellzeile zuordenbare Stellen. |
| `trice insert -ce …` | Schreibt ID, Erweiterung und Argumente in die erkannten Source-Aufrufe. Wiederholung mit denselben Regeln ergänzt nichts ein zweites Mal.           |
| `trice clean -ce …`  | Nimmt die erzeugte CE-Erweiterung mit denselben Regeln zurück und bereinigt die IDs nach den üblichen Regeln. Wiederholung ist unschädlich.         |

CE benötigt keinen globalen Runtime-Context oder Push/Pop-Aufrufe auf dem Target. Jeder ausgeführte Record überträgt seine eigenen zusätzlichen Werte. CE ist damit unabhängig von [Structured Logging](#strukturiertes-logging): Eine Erweiterung kann klassische printf-Platzhalter verwenden oder zusätzlich benannte Felder erzeugen.

Zwei lauffähige Anwendungen zeigen denselben Grundgedanken: Im [PC-Beispiel](../examples/PC_features/README.md) ergänzt `bind -ce` an einer gemeinsamen Logstelle einen Zykluswert. Im direkt von `G0B1_inst` abgeleiteten [FreeRTOS-Beispiel](../examples/G0B1_features/ReadMe.md) ergänzt dieselbe Logstelle die Kennung des jeweils aufrufenden Tasks. Beide Beispiele verwenden daneben `triceS` für einen Laufzeitstring; CE hängt an String-Trices keine zusätzlichen Runtime-Argumente an.

### 33.1. <a id="einstieg-mit-position-und-geschwindigkeit"></a>Einstieg mit Position und Geschwindigkeit

Ausgangspunkt ist ein regulär eingerichtetes [Bind-Projekt](#trice-bind). `./generated` (Default relativ zum Aufrufverzeichnis) muss im Include-Pfad des Compilers stehen. Diese Werte sind an der Logstelle sichtbar:

```c
struct Position {
    int32_t x;
    int32_t y;
};
struct Position pos = {-444, 77};
float velocity = 33.33f;
```

Die Logstelle enthält zwei frei gewählte Selektorpräfixe:

```c
trice32("info:pos:speed:Moving sample={sample}\n", 3);
```

Der Bind-Aufruf ergänzt Position und Geschwindigkeit:

```sh
trice bind -ce 'pos:", x={}, y={}", pos.x, pos.y' -ce 'speed:", m/s=%f", aFloat(velocity)'
```

Im endgültigen Template steht nun:

```text
info:Moving sample={sample}, x={pos.x}, y={pos.y}, m/s=%f\n
```

Übertragen werden der ursprüngliche Wert `3`, danach `pos.x`, `pos.y` und die Float-Bitdarstellung von `velocity`. Mit `-color none` ergibt der Meldungsteil der Textausgabe:

```text
Moving sample=3, x=-444, y=77, m/s=33.330002
```

Das gleiche Ergebnis ließe sich auch erreichen mit:

```c
trice32("info:Moving sample={sample}\n", 3);
```

und diesem Bind-Aufruf:

```sh
trice bind -ce 'info:", x={}, y={}, m/s=%f", pos.x, pos.y, aFloat(velocity)'
```

Die Nachkommastellen folgen der 32-Bit-Floatdarstellung und `%f`; `%.2f` würde `33.33` anzeigen. JSON und KV enthalten dieselbe Meldung einschließlich ihres abschließenden Newlines als escaped String. Zusätzlich entstehen die numerischen Felder `sample`, `pos.x` und `pos.y`. `%f` allein erzeugt kein benanntes Feld; dafür kann die Regel beispielsweise `speed:", m/s={speed:%.2f}", aFloat(velocity)` verwenden.

Die CE-Beispiele in [triceCheck.c](../_test/testdata/triceCheck.c) stehen unmittelbar nach den Structured-Logging-Beispielen. Ihre `//exp:`-Erwartungen beschreiben den normalen Lauf ohne `-ce`. Die CE-Integrationstests verwenden dieselben Aufrufe und prüfen die tatsächlich übertragenen Werte mit den oben gezeigten Regeln.

### 33.2. <a id="regeln-und-selektoren"></a>Regeln und Selektoren

`bind`, `insert` für die Erweiterung und `clean` für die zu entfernende Erweiterung verwenden dieselbe Syntax. Die daraus an einer Logstelle entstehende Regelgruppe muss in Format und Argumentreihenfolge vollständig passen:

```text
-ce 'selector:"format-extension"[, comma-free C-expression]...'
```

Die Shell muss den gesamten Optionswert als ein Argument übergeben; die Beispiele verwenden dafür einfache Anführungszeichen. Innerhalb der Erweiterung gelten dieselben C-Escapes und Platzhalter wie in einem Trice-Formatstring. Die Erweiterungen stehen vor einem abschließenden `\n` des ursprünglichen Templates, andernfalls an dessen Ende. Führende und folgende Leerzeichen bleiben erhalten.

Selektoren werden in der zusammenhängenden Präfixfolge am Anfang des Formatstrings gesucht, beispielsweise `info:pos:speed:`. Der Vergleich ignoriert Groß-/Kleinschreibung; bekannte eingebaute Tag-Aliase gehören dabei zu derselben Gruppe, etwa `warn` und `WARNING`.

So wählen `-ce 'Wrn:", attempt={attempt}", 7'` und `-ce 'WARNING:", attempt={attempt}", 7'` dieselben Logstellen aus, auch wenn deren Tag-Aliase im Source gemischt geschrieben sind:

```c
trice("WARNING:Connection lost");
trice("wrn:Retrying");
```

Beide Aufrufe erhalten die Erweiterung; `WARNING:` beziehungsweise `wrn:` bleibt in seiner ursprünglichen Schreibweise im Source stehen. Diese Alias-Auflösung gilt bei `bind`, `insert` und `clean` für die Auswahl der Logstelle. Der vollständige Match der Format- und Argumenterweiterung bei `insert` und `clean` wird davon getrennt geprüft.

- Nur konfigurierte Selektoren lösen CE aus. Ohne passende Regel bleibt ein Präfix ohne CE Wirkung.
- Ein freier, vollständig kleingeschriebener Selektor wird bei Anwendung seiner Regel aus dem endgültigen Template entfernt, nicht aus dem Source. `pos:` verschwindet aus der Ausgabe; `PoS:` und `POS:` bleiben sichtbar und lösen dieselbe Regel aus.
- Registrierte Trice-Tags und `-ulabel`-Namen behalten ihr Präfix im endgültigen Template. Für Tag-Metadaten und Darstellung gelten die normalen Regeln: `-color off` erhält beispielsweise `info:`; CE entfernt dieses bekannte Tag nicht.
- Verschiedene Selektoren wirken in ihrer Reihenfolge im Source. Mehrere Regeln für denselben Selektor wirken in CLI-Reihenfolge.
- Kommt derselbe Selektor, auch über einen Alias, mehrfach an einer Logstelle vor, wird seine Regelgruppe nur einmal angewendet. Beim Erweitern gibt das Tool eine Warnung für diese Logstelle aus.

Damit ergänzt `info:pos:speed:` zuerst die Position und danach die Geschwindigkeit, auch wenn die CLI die `speed`-Regel zuerst nennt. Eine gleiche Bezeichnung darf sowohl User-Label als auch CE-Selektor sein; `-ulabel` und `-ce` haben getrennte Aufgaben.

### 33.3. <a id="reversibler-ablauf-mit-insert-und-clean"></a>Reversibler Ablauf mit insert und clean

Ausgangspunkt in `main.c`:

```c
trice("msg:ctx7:hi\n");
```

Einfügen:

```sh
trice insert -src main.c -ce 'ctx7:", clock={}", clock'
```

Der Aufruf sieht danach so aus (die ID `1234` ist nur ein Beispiel):

```c
trice(iD(1234), "msg:ctx7:hi, clock={}\n", clock);
```

Es entstehen keine Herkunftskommentare und keine zusätzlichen CE-Metadatendateien. Für die Erkennung zählen ausschließlich der passende Selektor sowie die vollständige Erweiterung am Ende von Formatstring und Argumentliste. Ob dieser Text von Hand oder durch einen früheren Insert-Aufruf geschrieben wurde, spielt keine Rolle.

In `til.json` steht für diese ID das kanonische Template `msg:hi, clock={clock}\n`. Der Source behält `ctx7:` und die ursprüngliche Schreibweise seiner Felder. Der Compiler erhält den zusätzlichen Wert; der Decoder erhält das passende Schema. Ein zweiter identischer Insert-Aufruf erhält Erweiterung und ID.

Rücknahme:

```sh
trice clean -src main.c -ce 'ctx7:", clock={}", clock'
```

Danach steht wieder `trice("msg:ctx7:hi\n");` im Source. Eine feste Argumentzahl wird entsprechend angepasst: Aus `TRICE16_2(Id(1234), …)` wird nach Entfernung eines CE-Arguments `TRICE16_1(Id(0), …)`. Eine generische feste Null-Argument-Form wird als `trice0` beziehungsweise `TRICE0` geschrieben; die historische Schreibweise `trice_0` lässt sich ohne Herkunftsdaten nicht unterscheiden. Für IDs gelten weiterhin die üblichen Clean-Regeln: IDs kleingeschriebener Makrofamilien werden entfernt, IDs der entsprechenden Großschreibungsvarianten auf null gesetzt.

**Ein vollständiger Match muss positionsgenau sein.** Für die obige Regel müssen `, clock={}` am Ende des Formatstrings und `clock` am Ende der Argumentliste stehen. Ein abschließendes `\n` der Meldung bleibt hinter der Erweiterung erhalten. Enthält die Regel selbst ein abschließendes `\n`, gehört dieses zur Erweiterung und wird mit entfernt. Formattext, Leerzeichen im Format und Feldschreibweise müssen übereinstimmen: `{}` und `{clock}` sind für diesen Vergleich verschieden. Beim Argumentvergleich werden C-Kommentare wie beim normalen Einlesen durch Leerraum ersetzt und äußere Leerzeichen ignoriert; verschiedene Ausdrücke wie `clock`, `readClock()` oder `clock + 0` gelten nicht als gleich.

Auch ein scheinbar passendes Textende ist kein Match, wenn es zu einer vorhandenen Formatkonvertierung gehört. Bei `-ce 'ctx:"d"'` endet der folgende Formatstring zwar mit `d`, dieses Zeichen ist aber Teil von `%d`, dem Platzhalter für `x`:

```c
trice("ctx:value=%d", x);
```

`clean -ce` lässt den Aufruf unverändert: Das Entfernen des `d` würde aus `%d` ein einzelnes `%` machen. `insert -ce` hängt stattdessen ein eigenes `d` an und erzeugt `trice("ctx:value=%dd", x);`.

Ebenso ist bei `-ce 'ctx:"%d", clock'` das sichtbare `%d` am Ende des nächsten Formatstrings kein Wertplatzhalter:

```c
trice("ctx:value=%d %%d", clock);
```

Das erste `%d` gibt `clock` aus; `%%d` gibt wörtlich `%d` aus und benötigt kein weiteres Argument. Obwohl die letzten Zeichen `%d` und das letzte Argument `clock` zur Regel zu passen scheinen, gehören sie hier nicht zusammen. `clean -ce` lässt den Aufruf unverändert. `insert -ce` ergänzt einen eigenen Platzhalter mit eigenem Argument und erzeugt `trice("ctx:value=%d %%d%d", clock, clock);`.

| Zustand an der ausgewählten Logstelle | `insert -ce` | `clean -ce` |
| --- | --- | --- |
| Vollständige Format- und Argumenterweiterung vorhanden | Nichts ergänzen | Eine vollständige Erweiterung entfernen |
| Kein vollständiger Match, einschließlich Teilmatch | Die gesamte Erweiterung anhängen | CE unverändert lassen |

Die gewöhnliche ID-Verarbeitung findet in beiden Fällen statt. Bei mehreren passenden Regeln wird die gesamte Regelgruppe in ihrer Anwendungsreihenfolge verglichen. Einzelne bereits passende Bestandteile werden weder übersprungen noch separat entfernt. Wiederholtes `insert` ergänzt deshalb nichts doppelt. `clean` entfernt pro Aufruf höchstens eine vollständige Gruppe; liegen zwei identische Gruppen hintereinander, kann ein zweiter Clean-Aufruf auch die zweite entfernen.

Ein Teilmatch ist beispielsweise dieser Aufruf zur Regel `ctx7:", clock=%d", clock`:

```c
trice("msg:ctx7:hi, clock=%d\n", other);
```

Der Format-Anhang passt, das letzte Argument `other` jedoch nicht. `clean -ce` lässt den CE-Anteil unverändert. `insert -ce` hängt die vollständige Erweiterung an:

```c
trice(iD(1234), "msg:ctx7:hi, clock=%d, clock=%d\n", other, clock);
```

Ein erneutes `insert` erkennt nun den vollständigen Match am Ende. `clean` mit derselben Regel entfernt genau das letzte `, clock=%d` und das letzte Argument `clock`; der vorher vorhandene Teilmatch mit `other` bleibt stehen. Normale Prüfungen auf gültige Trice-Aufrufe und eindeutige strukturierte Feldnamen gelten weiterhin auch beim Anhängen an einen Teilmatch.

Soll eine vorhandene Erweiterung durch eine andere ersetzt werden, wird zuerst die alte passende Gruppe entfernt:

```sh
trice clean -src main.c -ce 'ctx7:", clock={}", clock'
trice insert -src main.c -ce 'ctx7:", clock={clock}", readClock()'
```

Ein `insert` oder `clean` **ohne** `-ce` führt nur seine normale ID-Aufgabe aus. Es nimmt eine vorhandene CE-Erweiterung nicht zurück. Für die vollständige Rücknahme müssen die bisherigen `-ce`-Optionen angegeben werden. Weitere Optionen, etwa `-src`, `-til`, `-li` und verwendete Trice-Aliase, müssen wie im normalen Workflow zum Projekt passen.

Auch dieser von Hand geschriebene Aufruf enthält einen vollständigen Match:

```c
trice("msg:ctx7:manual, clock={clock}\n", clock);
```

`clean -ce 'ctx7:", clock={clock}", clock'` entfernt das Feld und das letzte Argument. Ein entsprechendes `insert -ce` ergänzt nichts. Der Aufruf kann an eine andere Zeile oder in eine andere Datei verschoben werden; die Erkennung hängt weder von seinem früheren Ort noch von Build-Dateien ab.

`insert -ce` und `clean -ce` beachten `-src`, `-exclude` und `TRICE_INSERT_OFF`/`TRICE_INSERT_ON`. Trice-Aufrufe in gewöhnlichen C-Kommentaren werden nicht durch CE verändert; die bisherige ID-Verarbeitung solcher Beispiele bleibt bestehen. Bereits über Sidecar-Includes gebundene Dateien werden nicht automatisch auf Insert umgestellt. Für diese gilt der [Rückweg zu `trice insert`](#re-migration-to-trice-insert).

Der CE-Pfad prüft alle ausgewählten Dateien vor dem Veröffentlichen und schreibt Source, TIL, LI sowie das Insert-Feldregister gemeinsam mit Rücknahme bei Schreibfehlern. `-dry-run` veröffentlicht nichts. Mit `-ce` wird der experimentelle, nur auf Zeitstempeln beruhende `-cache` umgangen, damit geänderte Regeln nicht mit alten Source-Kopien vermischt werden. `trice-fields.txt` beschreibt weiterhin den letzten erfolgreichen Insert-/Bind-Lauf; Clean erzeugt kein neues Feldregister.

### 33.4. <a id="ausdrücke-felder-und-auswertung"></a>Ausdrücke, Felder und Auswertung

Jeder CE-Ausdruck muss kommafrei sein und an jeder ausgewählten Logstelle gültig und sichtbar sein. Geeignet sind etwa `pos.x`, `motor->speed`, `array[i]`, `x + 1`, `aFloat(velocity)` oder `condition ? a : b`. `getValue(a, b)` und der Kommaoperator sind in der CLI-Liste nicht zulässig; solche Ergebnisse können vorher in einer lokalen Variable berechnet werden. Jeder Optionswert steht vollständig auf einer Zeile; `//`-Kommentare sind in den Ausdrücken nicht zulässig.

`{}` leitet einen Feldnamen aus einem einfachen Ausdruck ab: `pos.x` wird `pos.x`, `motor->speed` wird `motor.speed`. Für komplexere Ausdrücke ist ein Name anzugeben, beispielsweise `ctx:", next={next}", x + 1`. Klassische printf-Platzhalter und benannte Felder dürfen gemischt werden. Literale geschweifte Klammern werden als `{{` und `}}` geschrieben. Doppelte Feldnamen im endgültigen Record sind ein Fehler, auch wenn einer im Source und einer in einer CE-Regel steht.

Ursprüngliche Argumente stehen vor den zusätzlichen CE-Argumenten. Jeder zusätzliche Ausdruck wird pro tatsächlich ausgeführtem Aufruf genau einmal ausgewertet. Ein nicht ausgeführter Aufruf sowie `TRICE_OFF` oder `TRICE_CLEAN` werten ihn nicht aus. CE führt keine zusätzliche Reihenfolgegarantie zwischen verschiedenen C-Ausdrücken ein; abhängige Seiteneffekte gehören in separate Anweisungen vor dem Aufruf.

Skalare Trices übertragen weiterhin höchstens zwölf Werte derselben Bitbreite. CE behält Bitbreite und Stempeltyp bei und passt eine feste Arity an, beispielsweise `TRice32_1` zu `TRice32_3`. Ein Floatwert benötigt bei 32 Bit ausdrücklich `aFloat(...)`, bei 64 Bit `aDouble(...)`; eine automatische Konvertierung findet nicht statt. 8-/16-Bit-Trices können keine Floatwerte übertragen. Der Compiler prüft die tatsächlichen C-Typen und die Sichtbarkeit der Bezeichner.

String-, Puffer- und andere besondere Trice-Familien erhalten keine zusätzlichen Runtime-Argumente durch CE. Eine reine Texterweiterung ohne zusätzliche Werte ist möglich, soweit das endgültige Format für die ursprüngliche Familie gültig bleibt, etwa `label:" online"` an einem `triceS`. Benannte Pufferfelder bleiben wie bei Structured Logging ausgeschlossen.

### 33.5. <a id="globale-und-lokale-werte-verständlich-einsetzen"></a>Globale und lokale Werte verständlich einsetzen

Globale Zustandswerte und überall verfügbare Funktionen sind häufig besonders praktisch:

```sh
trice insert -ce 'ctx7:", clock={clock}", readClock()'
```

Jede ausgewählte Logstelle muss `readClock()` aufrufen dürfen; die passende Deklaration muss dort bekannt sein. Der Wert wird beim tatsächlichen Logaufruf gelesen, nicht beim Aufruf des Trice-Tools. Ohne ausgeführten Logaufruf entsteht auch kein CE-Aufruf der Funktion.

Lokale Variablen sind ebenfalls sinnvoll, wenn alle ausgewählten Stellen denselben Ausdruck verwenden können. Beispielsweise passt die Regel `-ce 'job:", job={job}", jobId'` zu beiden Funktionen:

```c
void startJob(int jobId) {
    trice("info:job:start\n");
}

void finishJob(int jobId) {
    trice("info:job:finish\n");
}
```

Die gemeinsame Regel liest jeweils den lokalen Parameter der ausgeführten Funktion. Es gibt keinen globalen `jobId`-Speicher und keine Vermischung verschiedener Aufrufe.

Diese Variante funktioniert dagegen nicht mit derselben Regel:

```c
void startJob(int jobId) {
    trice("info:job:start\n");
}

void finishJob(int finishedJobId) {
    trice("info:job:finish\n");
}
```

In `finishJob` existiert `jobId` nicht. Der Compiler meldet den fehlenden Namen automatisch; es ist keine zusätzliche CLI-Option erforderlich. Mögliche Lösungen sind ein einheitlicher Parametername, ein lokaler Hilfswert, getrennte Selektoren mit passenden Regeln oder das direkt angegebene Feld `trice("info:finish, job={job}\n", finishedJobId);`.

Auch eine normale Hilfsfunktion kann nicht auf die lokalen Variablen ihres Aufrufers zugreifen. Das gilt ebenso für `static inline`: Inlining schafft keine zusätzliche Sichtbarkeit. Werte müssen als Parameter übergeben werden:

```c
static inline void logJob(int jobId) {
    trice("info:job:progress\n");
}

void worker(void) {
    int currentJob = 17;
    logJob(currentJob);
}
```

Diese Einschränkung bleibt bestehen, weil CE normalen C-/C++-Code erzeugt. Das Trice-Tool kennt weder alle Typen und Deklarationen noch die vom konkreten Compiler ausgewählten Präprozessorzweige. Es prüft Syntax, Schema und unterstützte Logformen selbst; die genaue Sichtbarkeit prüft der ohnehin erforderliche Compiler. Ein eigener vollständiger Compiler-Vorlauf nur für eine frühere Fehlermeldung würde die Bedienung und den Build unnötig verkomplizieren.

### 33.6. <a id="build-ids-und-generierte-dateien"></a>Build, IDs und generierte Dateien

CE wird vor der Schema- und ID-Bestimmung angewendet. Die ID richtet sich nach dem endgültigen Trice-Typ und dem kanonischen Template einschließlich Feldnamen. Ein anderer Ausdruck bei identischem Schema ändert die ID nicht: `ctx:", x={position}", pos.x` kann zu `ctx:", x={position}", pos.y` wechseln. Eine Änderung des Feldnamens, des Formats oder des endgültigen Typs folgt dagegen den bestehenden ID-Vergaberegeln. Historische TIL-Einträge bleiben für ältere Firmware erhalten.

Nach jeder Änderung an Source oder CE-Konfiguration wird `bind` mit der vollständigen gewünschten Regelliste erneut ausgeführt und die Firmware neu gebaut. Ohne `-ce` entstehen beim nächsten Bind-Lauf wieder die normalen Schemas ohne CE. Die Regeln werden nicht aus einem früheren Lauf fortgeschrieben. Wörterbuch und erzeugte Firmware müssen zusammengehören; eine Änderung nur an `til.json` kann keine zusätzlichen Target-Werte erzeugen.

Die normalen Bind-Einrichtungsschritte, etwa das erstmalige Sidecar-Include, bleiben bestehen. CE selbst schreibt weder die Erweiterung noch zusätzliche Argumente in die User-Logstellen. Wiederholungsläufe mit gleicher Konfiguration erhalten Source, IDs und generierte Inhalte. `trice-fields.txt` zählt die endgültigen CE-Felder zusammen mit den direkt angegebenen Feldern für den aktuellen Lauf. `-dry-run` veröffentlicht keine Änderungen. Ungültige Regeln, Feldkonflikte und ausgewählte nicht unterstützte Bind-Stellen werden vor Schreibzugriffen abgewiesen; bei einem Veröffentlichungsfehler greift die bestehende Bind-Rücknahme.

`generate -logC` verwendet die endgültigen CE-Schemas aus TIL. Bei Bind liefern die Sidecars die Zuordnung, bei Insert die expliziten IDs im Source. Die Regeln müssen dafür nicht nochmals angegeben werden:

```sh
trice generate -til til.json -genDir generated -logC triceLog.c
```

Bei Bind führen veraltete oder widersprüchliche CE-Metadaten zu einem Fehler; nach einem geänderten Trice-Aufruf muss zuerst erneut mit den gewünschten Regeln gebunden werden. Bei Insert darf der Source die freien kleingeschriebenen Selektoren zusätzlich zum TIL-Template enthalten. Meldung, Feldschema und Trice-Typ müssen zum Eintrag der expliziten ID passen. Für geänderte Meldungen wird zuerst erneut `insert` ausgeführt; für den Austausch einer CE-Erweiterung gilt der oben gezeigte Ablauf `clean -ce`, dann `insert -ce`. Derselbe Source-Umfang und dieselbe TIL müssen zugänglich sein; Bind benötigt zusätzlich seine Sidecars im passenden Build-Verzeichnis.

### 33.7. <a id="unterstützte-logstellen-und-alternativen"></a>Unterstützte Logstellen und Alternativen

`bind -ce` unterstützt direkte, anhand von Datei und Quellzeile eindeutig zuordenbare Trice-Aufrufe, auch innerhalb normaler und `static inline` Funktionen. Dieser Weg benötigt kein `__COUNTER__`. Ein mehrzeiliger Aufruf ist ebenfalls möglich, wenn auf seinen belegten Zeilen keine andere Bind-Logstelle liegt.

Ausgewählte Wrappermakros und Counter-Rebase-Stellen sind zurückgestellt. Ein typischer Fehler lautet:

```text
main.c:42: error: CE requires a direct, line-addressable bind site. Search UM for "bind-limits".
```

Der Abschnitt [bind-limits](#bind-limits) erklärt die Ursache und mögliche Codeanpassungen ohne Compiler-Spezialwissen. Geeignete Schritte sind getrennte Quellzeilen oder normale Funktionen mit ausdrücklich übergebenen lokalen Werten. Nicht von CE ausgewählte Wrapper-/Rebase-Stellen behalten das bisherige Bind-Verhalten einschließlich ihrer Compileranforderungen.

`insert -ce` schreibt die endgültigen Argumente direkt an jede erkannte Logstelle. Dadurch entfällt die Bind-Auswahl über Quellzeile oder Compilerzähler. Insbesondere können diese Aufrufe mit `-ce 'ctx7:", clock={}", clock'` erweitert werden:

```c
trice("msg:ctx7:first\n"); trice("msg:ctx7:second\n");

#define LOG_STATUS() trice("msg:ctx7:status\n")
```

Bei einem Wrapper ergänzt Insert den Trice-Aufruf in der **Makrodefinition**. Jeder spätere Aufruf von `LOG_STATUS()` verwendet diese Erweiterung. `clock` muss an jeder Expansion sichtbar sein. Derselbe Wrapper wird dadurch nicht pro Aufrufort mit unterschiedlichen Regeln ausgestattet; Selektoren gehören zum erkannten Formatstring in der Definition.

Auch Parameter eines solchen Wrappers können verwendet werden:

```c
#define LOG_JOB(jobId) trice("info:job:progress\n")

void worker(void) {
    LOG_JOB(17);
}
```

Mit `insert -ce 'job:", job={job}", jobId'` wird `jobId` direkt in die Definition eingesetzt und bei der Makroexpansion durch `17` ersetzt. Die normalen Makroregeln gelten unverändert; insbesondere sind Argumente mit voneinander abhängigen Seiteneffekten zu vermeiden.

Das setzt einen vom Trice-Parser erkennbaren Aufruf mit bekanntem Formatstring voraus. Aus einer Definition wie `#define LOG_ANY(format) trice(format)` lässt sich dagegen kein statischer Selektor und kein vollständiges Schema ablesen. CE ist kein allgemeiner C-Präprozessor und verspricht keine Unterstützung beliebiger per Makro zusammengesetzter Formate. Ein expliziter Trice-Aufruf oder eine Funktion mit festem Format und übergebenen Werten bleibt die einfache Alternative.

| Logform | `bind -ce` | `insert/clean -ce` |
| --- | --- | --- |
| Direkter eindeutig zuordenbarer Aufruf, auch in einer Inline-Funktion | Unterstützt | Unterstützt |
| Mehrere direkte Aufrufe auf derselben Zeile | Bei Auswahl durch CE abgewiesen | Erkennbare Aufrufe werden einzeln erweitert |
| Wrapperdefinition mit statischem Trice-Format | Bei Auswahl durch CE weiterhin abgewiesen | Die erkannte Definition wird erweitert und wiederhergestellt |
| Lokaler Name fehlt an der tatsächlichen Expansion | Compilerfehler | Compilerfehler |
| String-/Pufferrecord mit zusätzlichen skalaren CE-Argumenten | Fehler vor Veröffentlichung | Fehler vor Veröffentlichung |

**Warum die Bind-Grenze trotz erfolgreichem PoC bleibt:** Beim bestehenden Counter-Rebase gelangten Ausdrücke verschiedener Logstellen in mehrere C-Verzweigungen. Der Compiler prüft auch den nicht ausgeführten Zweig. Eine nur links gültige lokale Variable kann daher rechts einen künstlichen Fehler auslösen. Der erweiterte PoC wählt den Adapter bereits während der Makroexpansion und vermeidet diesen Fehler für die geprüften Fälle. Dafür benötigt er einen zusätzlichen Vorlauf mit dem konkreten Compiler pro Übersetzungseinheit und Build-Konfiguration sowie passende erzeugte Zuordnungsdateien.

Dieser Build-Aufwand wurde nicht als produktiver Workflow eingeführt. Außerdem ist `__COUNTER__` keine überall verfügbare Compilereigenschaft und kein Laufzeit- oder Cycle-Counter. Selbst global sichtbare CE-Werte führen deshalb nicht zu einer gesonderten Freischaltung komplexer Bind-Stellen. Die einheitliche Grenze lässt sich einfach erklären und bereits bei Bind abweisen. `insert/clean -ce` benötigt diese Zuordnung und diesen Vorlauf nicht. Die genauen Nachweise, Kosten und offenen Punkte stehen im [kapitelinternen PoC-Anhang](#anhang-ce-machbarkeitsnachweise).

### 33.8. <a id="prüfumfang"></a>Prüfumfang

Die [Regel- und Bind-Tests](../internal/id/contextEnrichment_test.go) prüfen Selektoren, Aliase, Reihenfolge, ungültige Regeln, Grenzen, stabile IDs, Konfigurationswechsel, das Feldregister sowie unveränderte Dateien bei Ablehnungen und Schreibfehlern. Die [Insert-/Clean-Tests](../internal/id/contextSource_test.go) ergänzen vollständige und teilweise Matches, die Position von Format- und Argumentsuffix, mehrteilige Regelgruppen, Newlines, Wiederholungen, handgeschriebene Felder, Kommentare, verschobene Aufrufe, Ausschlüsse und Rücknahme nach Schreibfehlern. Die [CLI- und Target-Tests](../internal/args/context_enrichment_test.go) führen beide öffentlichen Wege über `generate -logC` und echte Target-Records bis zur Text-/JSON-/KV-Ausgabe aus.

Der produktive Target-Nachweis umfasst Clang in C11 und C++17, `clangd` mit realer Compile-Konfiguration, 8/16/32/64-Bit-Werte, verschiedene Stempeltypen und Builds ohne `__COUNTER__`. Er prüft getrennte lokale Sichtbarkeitsbereiche, einmalige Auswertung, `TRICE_OFF`, `TRICE_CLEAN` und verständliche Compiler-/Editorfehler bei fehlenden Bezeichnern. Insert prüft zusätzlich zwei Logstellen in getrennten lokalen Blöcken derselben Wrapperzeile und deren vollständige Rücknahme. Die Editorprüfung schließt lediglich clangds Refactoring-Aktion `SwapBinaryOperands` aus: Clangd 21 schlägt dafür innerhalb eines expliziten `Id(...)` überlappende Textänderungen vor. Compilerdiagnosen und Fehler bei fehlenden Bezeichnern bleiben geprüft. Weitere Compiler werden getrennt im PoC-Anhang eingeordnet.

Die gezielte Abnahme lässt sich im Repository-Root wiederholen:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id ./internal/args -run '^(TestBindContext|TestContextEnrichment|TestInsertCleanContext|TestSourceContext|TestContextInsertClean)' -count=1
```

### 33.9. <a id="anhang-ce-machbarkeitsnachweise"></a>Anhang: CE-Machbarkeitsnachweise

Stand: 27. September 2026. Der isolierte A9-Nachweis für direkte Bind-Logstellen ist bestanden. Er liegt in [context_enrichment_poc_test.go](../internal/id/context_enrichment_poc_test.go). Die darauf aufbauende produktive Option `trice bind -ce` ist inzwischen mit A10 implementiert; ihre Bedienung und Abnahme stehen im [User Manual](#trice-context-enrichment). Dieser Anhang enthält außerdem die ursprüngliche Rebase-Gegenprobe und den neuen [PoC für Wrappermakros und Counter-Rebase](#erweiterter-poc-für-wrappermakros-und-counter-rebase). Letzterer ist eine Entscheidungsgrundlage und aktiviert keine zusätzliche produktive CE-Unterstützung.

#### 33.9.1. <a id="geprüfter-mechanismus"></a>Geprüfter Mechanismus

Der bestehende Bind-Deskriptor enthält neben der ID ein anzuwendendes Makro. Der PoC verwendet an ausgewählten Logstellen ein generiertes Adaptermakro, das die ursprünglichen Argumente übernimmt und die Context-Ausdrücke anhängt. Diese Ausdrücke werden erst bei der Expansion des ursprünglichen Trice-Aufrufs ausgewertet und haben dort Zugriff auf lokale Variablen.

Für eine ursprünglich argumentlose Logstelle entspricht der Adapter beispielsweise:

```c
#define TRICE_CE_POC_SITE(ignoredImplementation, tid, format) \
    TRICE_INSERT_trice(tid, format, (x))
```

Bei vorhandenen Argumenten erhält das Adaptermakro passende zusätzliche Parameter. Jeder wird genau einmal in den endgültigen Aufruf übernommen. Generische Trice-Makros bestimmen ihre Arity aus der erweiterten Argumentliste. Bei festen Arity-Makros wählt der Adapter die passende Implementierung, beispielsweise `TRICE_INSERT_trice_1` für eine erweiterte `trice_0`-Logstelle.

Der Test erzeugt den erweiterten Template-String und die zusätzlichen Argumente zunächst ausschließlich in einer privaten In-Memory-Sourceansicht. Auf dieser Ansicht läuft der vorhandene `SubCmdIdBind` mit dem normalen Structured-Logging-Parser und der normalen ID-Vergabe. Damit wird die ID aus dem endgültigen Schema bestimmt. Der Compiler sieht dagegen weiterhin den unveränderten User-Source und das erzeugte Sidecar mit den Adaptermakros. Das ist ein Testadapter für den Architekturbeweis, noch keine produktive CE-Integration.

Die Fixture enthält bereits das reguläre Bind-Sidecar-Include und einen festen File-Key. Der Test prüft deshalb die durch CE geforderte Source-Unveränderlichkeit unabhängig von der erstmaligen Einrichtung eines Bind-Projekts.

#### 33.9.2. <a id="nachgewiesenes-verhalten"></a>Nachgewiesenes Verhalten

| Fall | Erwartetes Ergebnis | Nachweis |
| --- | --- | --- |
| Argumentloses `trice` | Lokales `x` wird als zusätzlicher Wert übertragen | Binärrecord enthält `7` |
| Bereits parametrisiertes `trice` | Originalwert steht vor dem CE-Wert | Binärrecord enthält `1, 1` |
| Einfacher Ausdruck | `x + 1` wird am Aufrufort ausgewertet | Binärrecord enthält `8` |
| Feste Arity | `trice_0` und `trice_1` erhalten die passende endgültige Arity | Binärrecords enthalten `7` bzw. `22, 7` |
| Innerer Block-Scope | Nur dort sichtbares `blockValue` wird verwendet | Binärrecord enthält `11` |
| Unselektierte Logstelle | Keine zusätzlichen Werte | Binärrecord bleibt argumentlos |
| Seiteneffekte | Originalausdruck und CE-Ausdruck werden jeweils genau einmal ausgewertet | Zwei getrennte Laufzeitzähler stehen auf `1` |
| Nicht ausgeführter Aufruf | Kein Record und kein CE-Seiteneffekt | Sieben Records trotz acht instrumentierter Logstellen; CE-Zähler bleibt auf `1` |
| TIL-Konsistenz | Finale Templates, Arity, IDs und Nutzdaten passen zusammen | Explizite Template-Erwartungen und echter Trice-Record-Parser/Resolver |
| Wiederholung | Identische IDs und generierte Inhalte | Bytevergleich von Source, Konfiguration, TIL, LI, Sidecar und Feldregister nach zwei PoC-Bind-Läufen |
| Ungültiger Context | Ein nicht sichtbarer Bezeichner wird diagnostiziert | Compiler und `clangd` weisen `ceMissingLocal` ab |

Der Laufzeittest verwendet die tatsächlichen Target-Makros und die Trice-Bibliothek. Der Auxiliary-Ausgang liefert die erzeugten Binärrecords an `TriceParseRecord`; `TriceResolveLog` prüft sie gegen eine aus der endgültigen TIL erzeugte C-Metadatentabelle. Damit wird auch eine falsche Payload-Länge oder Parameterzahl erkannt. Die C-Tabelle wird im PoC direkt aus der TIL erzeugt; der öffentliche `generate -logC`-Workflow ist dabei nicht geprüft.

#### 33.9.3. <a id="compiler-und-editor-diagnosen"></a>Compiler und Editor-Diagnosen

Der Test kompiliert und startet dieselbe Fixture als C11 und C++17 mit `-Wall -Wextra -Werror`. Die Bibliotheksquellen werden als C übersetzt. Für beide Sprachmodi wird eine `compile_commands.json` mit den tatsächlichen Compilerargumenten erzeugt. `clangd --check` muss diese Datenbank laden und ohne Fehler abschließen. Der Negativtest zeigt zusätzlich, dass fehlende Context-Bezeichner weiterhin sichtbar diagnostiziert werden.

Geprüfte Umgebung: macOS auf ARM64, Apple Clang/Clang++ 21.0.0 und Apple clangd 21.0.0. Beide Sprachmodi und die negativen Diagnoseprüfungen bestanden. Dies belegt den Language-Server-Pfad für clangd-basierte Editoren mit dem generierten Include-Verzeichnis und der realen Compile-Konfiguration. Andere Language-Server, IDE-eigene Parser, GCC und MSVC wurden in diesem A9-Lauf nicht geprüft.

#### 33.9.4. <a id="reproduzieren"></a>Reproduzieren

Im Repository-Root ausführen:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id -run '^TestContextEnrichmentPoC$' -count=1 -v
```

Erforderlich sind ein GCC-/Clang-kompatibler C- und C++-Compiler sowie `clangd` im `PATH`. Der gezielte Test verlangt diese Werkzeuge ausdrücklich. Ohne `TRICE_BIND_INTEGRATION=1` wird er übersprungen. Alle Fixtures und Build-Artefakte entstehen in einem temporären Testverzeichnis und werden anschließend entfernt. Die bestehenden Repository-Workflows werden nicht verändert.

#### 33.9.5. <a id="abgrenzung-zu-a10"></a>Abgrenzung zu A10

Der Nachweis erfüllt die Mindestfälle aus dem [CE-Kapitel](#trice-context-enrichment). Er prüft direkte skalare 32-Bit-Logstellen mit einer Logstelle pro physischer Zeile und dem `iD`-Stempeltyp. Die PoC-Regeln sind feste Testdaten; der A9-Test enthält keinen CLI-Parser, keine vollständige Selektor-/Alias-Policy und keine produktive Fehlervalidierung.

A10 bindet die Transformation vor der produktiven Schema-/ID-Vergabe ein und erzeugt die Sidecar-Erweiterung dauerhaft. Die erste Ausbaustufe bleibt auf direkte, eindeutig über ihre Quellzeile adressierbare Logstellen begrenzt. Die zusätzlichen [Bind-Verhaltenstests](../internal/id/contextEnrichment_test.go) und [CLI-/Target-Tests](../internal/args/context_enrichment_test.go) prüfen die breitere Abnahme getrennt vom A9-PoC: 8/16/32/64 Bit, verschiedene Stempeltypen, `TRICE_OFF`/`TRICE_CLEAN`, Regelkonflikte, Konfigurationswechsel, Mehrzeiler, Inline-Funktionen und echte Compiler-/Editorläufe ohne `__COUNTER__`. Vier Beispiele werden aus `triceCheck.c` übernommen; insgesamt vierzehn Records durchlaufen den öffentlichen `generate -logC`-Resolver und den Go-Decoder für Text, JSON und KV. CE für Wrappermakros und Counter-Rebase bleibt eine eigene Folgeaufgabe.

Damit ist der direkte Mechanismus nicht mehr nur ein PoC. Die weiterhin offenen Varianten bleiben im [Arbeitsplan](./scratchPad/Implementierungsplan.md) abgegrenzt.

#### 33.9.6. <a id="ergänzende-rebase-gegenprobe-vor-a10"></a>Ergänzende Rebase-Gegenprobe vor A10

Am 27. September 2026 wurde die direkte Übertragung des Adapteransatzes auf Counter-Rebase geprüft. Der zusätzliche Test `TestContextEnrichmentPoCRebaseScopeBoundary` zeigt eine Grenze: Zwei von Bind unterstützte Logstellen auf derselben Sourcezeile liegen in getrennten Blöcken und verwenden jeweils eine nur dort sichtbare Variable. Der normale Bind-Build besteht. Werden die beiden CE-Ausdrücke in die jeweiligen Zweige des generierten Rebase-Dispatchers eingefügt, scheitert die Übersetzung an den Variablennamen des jeweils anderen Scopes.

Der Grund ist die C-seitige Ordinalauswahl: Auch ein zur Laufzeit nicht gewählter `if`-Zweig wird vom Compiler auf gültige Bezeichner geprüft. Die betroffenen Ausdrücke sind an ihrer vorgesehenen Logstelle gültig. Der Fehler wäre deshalb eine unzulässige zusätzliche Scope-Anforderung der Instrumentierung. Der Test erwartet und belegt genau diese fehlgeschlagene Erweiterung; er ist keine bestandene CE-Rebase-Abnahme.

Der direkte A9-Nachweis bleibt gültig. Das bloße Anhängen von CE-Argumenten an Rebase-Zweige genügt für eine allgemeine CE-Unterstützung jedoch nicht. Am 27. September wurde deshalb die erste Ausbaustufe auf direkte, eindeutig über ihre Quellzeile adressierbare Bind-Logstellen begrenzt. A10 weist ausgewählte Wrapper-/Rebase-Stellen vor Dateiänderungen ab und verweist mit `Search UM for "bind-limits".` auf die verständliche Erklärung im User Manual. Ohne passende CE-Regel bleiben die bisherigen Bind-Fähigkeiten erhalten. Der zusätzliche Architektur-Nachweis für komplexe CE-Stellen ist eine zurückgestellte Folgeaufgabe; diese Gegenprobe allein belegt keine grundsätzliche Unmöglichkeit einer späteren Lösung.

Die Gegenprobe ist separat reproduzierbar:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id -run '^TestContextEnrichmentPoCRebaseScopeBoundary$' -count=1 -v
```

#### 33.9.7. <a id="erweiterter-poc-für-wrappermakros-und-counter-rebase"></a>Erweiterter PoC für Wrappermakros und Counter-Rebase

**Ergebnis:** Eine Auswahl des CE-Adapters bereits im Präprozessor beseitigt das nachgewiesene Problem fremder lokaler Variablen. Der neue Test [context_enrichment_rebase_poc_test.go](../internal/id/context_enrichment_rebase_poc_test.go) weist einen funktionierenden Ansatz mit einem zusätzlichen Compiler-Vorlauf nach. Er enthält außerdem einen einfacheren Sonderfall: Ein Wrapper mit genau einer Logstelle kann bei eindeutiger Zeilenzuordnung ohne diesen Vorlauf und ohne `__COUNTER__` angereichert werden. Beides bleibt Testcode; `bind -ce` weist die bisher ausgeschlossenen Konstrukte weiterhin ab.

##### Wie der untersuchte Ansatz arbeitet

Bei der bisherigen C-Verzweigung gelangen die Ausdrücke aller möglichen Logstellen zum Compiler. Im neuen PoC wählt dagegen die Makroexpansion genau einen Adapter aus. Nur dessen Ausdrücke erscheinen im endgültigen C-/C++-Code. Dadurch kann etwa ein Wrapper in seinem linken Zweig eine Variable `branchLeft` und in seinem rechten Zweig eine andere Variable `branchRight` verwenden, ohne dass einer dieser Namen im jeweils anderen Block existieren muss.

Die Zuordnung benötigt einen Wert, den der Präprozessor direkt als Teil eines Makronamens verwenden kann. Die bestehende relative Rechnung aus `__COUNTER__` und einer C-Enum-Konstante eignet sich dafür nicht. Der PoC ermittelt deshalb die tatsächlichen absoluten Counter-Werte mit dem jeweils verwendeten Compiler:

1. Der bestehende Bind-Mechanismus richtet die temporären Testquellen regulär ein. Eine private Sourceansicht erhält die CE-Erweiterungen und durchläuft die vorhandene Schema-/ID-Vergabe. Der tatsächlich kompilierte User-Source behält seine ursprünglichen Trice-Aufrufe und Wrapperdefinitionen.
2. Der Compiler verarbeitet diese Quellen mit den tatsächlichen Sprach-, Target- und Präprozessoroptionen vor. Eine nur für den Test eingebundene Datei lässt für jede Rebase-Expansion eine Markierung mit Region und Counter-Wert erscheinen.
3. Der Test ordnet diese Markierungen den numerischen Definition-/Location-Deskriptoren und der Expansionsreihenfolge aus dem erzeugten Bind-Sidecar zu. Er errät keine IDs aus Formatstrings. Daraus entsteht ein Header mit genau einem Makro pro tatsächlich beobachteter Expansion.
4. Beim normalen Übersetzen wählt `__COUNTER__` dieses Makro aus. Es übergibt die bestehenden Argumente und ergänzt nur die zugehörigen CE-Ausdrücke. Die bestehenden Rebase-Endprüfungen bleiben aktiv. Eine zusätzliche Prüfung des Basiswertes weist eine verschobene Zuordnung zurück, auch wenn zufällig noch ein anderer gültiger Eintrag getroffen würde.

Dieser Vorlauf ist pro Übersetzungseinheit und konkreter Build-Konfiguration erforderlich. Eine Übersetzungseinheit ist hier beispielsweise eine `.c`- oder `.cpp`-Datei einschließlich ihrer eingebundenen Header. Derselbe gemeinsam verwendete Wrapper kann deshalb für verschiedene Übersetzungseinheiten unterschiedliche Counter-Zuordnungen benötigen, während seine logischen IDs gleich bleiben.

##### Nachgewiesene Fälle

| Fall | Geprüftes Ergebnis |
| --- | --- |
| Einfacher Wrapper mit einer Logstelle | CE am Aufrufort funktioniert auch mit entferntem `__COUNTER__`; ein normaler Zeilendeskriptor genügt. |
| Wrapper mit zwei Logstellen | Ursprüngliche Werte stehen vor den jeweiligen CE-Werten; generische und feste Arity funktionieren. |
| Wiederholte Wrapper-Aufrufe | Dieselben logischen IDs übertragen unterschiedliche lokale Context-Werte ihrer Aufrufer. |
| Wrapper mit getrennten Zweigen | `branchLeft` und `branchRight` bleiben jeweils auf ihren eigenen Block beschränkt. |
| Zwei direkte Aufrufe auf derselben Zeile | Getrennte Blöcke mit `onlyLeft` und `onlyRight` funktionieren ohne fremde Scope-Anforderungen. |
| Nicht ausgewählte Rebase-Stellen | Die bestehenden Records bleiben unverändert und ohne zusätzliche Werte. |
| Seiteneffekte | Ursprünglicher Ausdruck und CE-Ausdruck werden je ausgeführtem Aufruf genau einmal ausgewertet. Ein nicht ausgeführter Wrapper-Aufruf bewirkt nichts. |
| Tatsächliche Records | Elf ausgegebene Records werden durch die echte Target-Bibliothek erzeugt, gegen die finale C-TIL aufgelöst und auf IDs, Parameterzahl und sämtliche geordneten Werte geprüft. |
| Wiederholung | Erneute private Bind-Generierung erhält Schema, IDs, Regionenzuordnung und Source. Derselbe Compiler-Vorlauf reproduziert denselben Zuordnungsheader. |
| Fremde Counter-Verwendungen vor den Logstellen | Zusätzliche Verwendungen mit den Abständen 0, 1 und 7 ändern die Zuordnung, während IDs und erwartete Records gleich bleiben. |
| Veralteter Zuordnungsheader | Ein normaler Build scheitert. Insbesondere wird auch eine Verschiebung um eins erkannt, die sonst auf einen benachbarten gültigen Adapter treffen könnte. |
| Fehlender Context-Bezeichner | Compiler und die geprüften `clangd`-Varianten melden `cePocMissingLocal` als echten Fehler. |
| Zusätzlicher Counter-Verbrauch im CE-Ausdruck | Wird abgewiesen; der untersuchte Vorlauf setzt einen Counter pro Rebase-Expansion voraus. |
| `TRICE_OFF` und `TRICE_CLEAN` | Keine Records und keine Argument-/CE-Auswertung, auch ohne verfügbares `__COUNTER__`. |
| Aktives Rebase ohne `__COUNTER__` | Klarer Buildfehler einschließlich `Search UM for "bind-limits".`. Der PoC behauptet für diesen Fall keine Lösung. |

Der Laufzeitnachweis verwendet skalare 32-Bit-Records mit `iD`. Er ersetzt nicht die breitere Bitbreiten-/Stempel-Abnahme von A10 und ist noch keine vollständige Abnahme sämtlicher denkbaren Wrapper.

##### Compiler-Matrix und Aussagegrenzen

Am 27. September 2026 wurden folgende installierte Werkzeugvarianten geprüft:

| Werkzeug und Ziel | Sprachmodi | Nachweis |
| --- | --- | --- |
| Apple Clang/Clang++ 21.0.0, macOS ARM64 | C99, C11, C17, C++11, C++17 | Vorverarbeitung, Kompilierung mit `-Wall -Wextra -Werror`, Linken und tatsächliche Programmausführung bestanden. |
| ARM GNU Toolchain 13.3.Rel1, GCC/G++ 13.3.1, Cortex-M0/Thumb | C99, C11, C17, C++11, C++17 | Vorverarbeitung und Erzeugung echter ARM-Objektdateien mit `-Wall -Wextra -Werror` bestanden; keine Ausführung auf MCU oder Emulator. |
| Dieselbe ARM-GCC-Version, Cortex-M4/Thumb | C99, C11, C17, C++11, C++17 | Vorverarbeitung und Erzeugung echter ARM-Objektdateien bestanden; keine Target-Laufzeitaussage. |
| Alle drei Konfigurationen | C++20 | Der bestehende Bind-Code scheitert bereits ohne experimentelles CE an einer mit `-Werror` eskalierten Enum-Warnung. Mit ausschließlich dieser Warnung auf Warnungsstatus zurückgesetzt besteht der CE-PoC; Clang einschließlich Laufzeit, ARM-GCC als Objekt-Build. |
| Apple clangd 21.0.0 | Host-C11 und Host-C++17 | Reale `compile_commands.json` einschließlich des experimentellen Headers wird geladen. Gültige Quellen sind fehlerfrei; der absichtlich fehlende Bezeichner wird diagnostiziert. |

Damit wurden 18 Kombinationen aus Toolchain/Target und Sprachmodus untersucht, jeweils mit drei Counter-Ausgangslagen sowie zusätzlichen Negativ- und Abschaltfällen. Die macOS-Kommandos `gcc`/`g++` sind in dieser Umgebung Clang-Aliase und werden ausdrücklich nicht als GCC-Nachweis gezählt. Der eigenständige GCC-Nachweis stammt vom ARM-Crosscompiler. Native GCC-Varianten werden bei Verfügbarkeit ebenfalls in die Testmatrix aufgenommen. MSVC, IAR, Arm Compiler/armclang und andere Language-Server wurden nicht geprüft; ihre Unterstützung ist daraus nicht ableitbar.

Die C++20-Grenze stammt aus der bestehenden Rebase-Prüfung: Sie subtrahiert Werte verschiedener anonymer Enum-Typen. Der Test weist das zuerst mit gewöhnlichem Bind nach und verwendet danach nur `-Wno-error=deprecated-anon-enum-enum-conversion` bei Clang beziehungsweise `-Wno-error=deprecated-enum-enum-conversion` bei GCC. Das ist ausdrücklich kein erfolgreicher strenger C++20-Build. Der Produktcode wurde für den PoC nicht geändert. Bei der Simulation eines fehlenden `__COUNTER__` wird die Warnung über das Entfernen eines eingebauten Makros ebenfalls nicht als Fehler behandelt.

##### Konsequenzen für eine mögliche Umsetzung

**Die technische Scope-Hürde ist für die geprüften Fälle gelöst; der Preis dieses Ansatzes ist ein zusätzlicher compilerabhängiger Build-Schritt.** Eine produktive Entscheidung muss diesen Aufwand bewusst einschließen. Der PoC liefert noch keinen solchen Workflow und keine neue CLI-Option.

Vor einer Umsetzung wären insbesondere folgende Punkte festzulegen oder nachzuweisen:

- Einbindung des Vorlaufs in die unterstützten Build-Systeme, einschließlich derselben Defines, Include-Pfade, Sprachmodi und Target-Optionen wie beim eigentlichen Übersetzen.
- Getrennte Zuordnungsartefakte pro Übersetzungseinheit und Konfiguration sowie verlässliche Neuerzeugung nach relevanten Änderungen. Die Counter-Prüfungen erkennen Verschiebungen, ersetzen aber keine vollständige Build-Abhängigkeitsprüfung oder einen Schutz gegen beliebig beschädigte Artefakte.
- Verhalten bei Precompiled Headers, Modulen, zusätzlichen Counter-Verwendungen in Argumentmakros und weiteren Compilerfamilien. Der PoC macht dafür keine Zusage.
- Falls strenge C++20-Builds zum Ziel gehören, eine gesonderte Korrektur und Abnahme der bestehenden Enum-Rebase-Prüfung.
- Entscheidung, ob die einfachere Erweiterung für eindeutig zuordenbare Wrapper mit einer Logstelle zunächst unabhängig von allgemeinem CE-Rebase umgesetzt werden soll. Der PoC belegt diesen Fall ohne Counter-Vorlauf; die produktive Freischaltung bleibt ein eigener Auftrag.

Ein zweiter Compilerlauf ist damit eine nachgewiesene Möglichkeit, keine Behauptung, dass es keinen einfacheren Ansatz geben kann. Die erste direkte CE-Ausbaustufe bleibt unverändert. Dieser PoC untersucht keine Source-Transformation. Das inzwischen verfügbare `insert/clean -ce` ist separat implementiert und oben mit seinen eigenen Tests beschrieben.

##### Den erweiterten PoC wiederholen

Im Repository-Root ausführen:

```sh
TRICE_BIND_INTEGRATION=1 go test ./internal/id -run '^TestContextEnrichmentRebasePoC$' -count=1 -v
```

Der Test erkennt installierte Clang-/GCC-C/C++-Toolchains und ARM-GCC selbst, meldet fehlende Werkzeuge und Compiler-Aliase und installiert nichts. Mindestens eine passende C/C++-Toolchain ist erforderlich. Ohne `TRICE_BIND_INTEGRATION=1` wird der Compiler-Test übersprungen. `clangd` wird bei Verfügbarkeit geprüft; ein fehlendes Werkzeug wird ausdrücklich gemeldet. Alle Source-Kopien, experimentellen Header und Build-Artefakte entstehen in temporären Testverzeichnissen. Produktive CLI, Target-Header und Build-Skripte bleiben unverändert.

### 33.10. <a id="ansatz-und-abgrenzung"></a>Ansatz und Abgrenzung

CE ist eine optionale Build-Time-Instrumentierung: Regeln wählen Logstellen aus, deren Records zusätzliche Runtime-Werte enthalten. Es führt keinen allgemeinen, impliziten Context-Zustand ein. Es gibt daher weder Push/Pop-Aufrufe noch Task-lokalen Zustand oder Context-Handles, die bei Taskwechseln oder Interrupts gesondert verwaltet werden müssten.

Andere Logging-Systeme bieten verwandte, aber anders aufgebaute Konzepte, etwa [Go `slog.Logger.With`](https://pkg.go.dev/log/slog), [Microsoft `ILogger.BeginScope`](https://learn.microsoft.com/dotnet/api/microsoft.extensions.logging.ilogger.beginscope), [Serilog `LogContext`](https://github.com/serilog/serilog/wiki/Enrichment) und [Rust `tracing` spans](https://docs.rs/tracing/latest/tracing/span/). Diese Referenzen beschreiben keine Trice-Abhängigkeiten oder Kompatibilitätszusagen.
