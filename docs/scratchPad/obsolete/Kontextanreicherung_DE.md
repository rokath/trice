# Context Enrichment

**Stand vom 27. September 2026:** A10 und der anschließend beauftragte reversible `insert/clean -ce`-Workflow sind implementiert und im deutschen [UM-Kapitel Context Enrichment](../TriceUserManual.md#trice-context-enrichment) dokumentiert. Dieses Dokument hält den zugrunde liegenden Vertrag und die zurückgestellten Erweiterungen fest. `bind -ce` unterstützt direkte, eindeutig über ihre Quellzeile adressierbare Logstellen ohne zusätzliche `__COUNTER__`-Abhängigkeit. CE für Bind-Wrappermakros und Counter-Rebase bleibt eine Folgeaufgabe; der vorhandene PoC ist jetzt [Anhang des UM-Kapitels](../TriceUserManual.md#anhang-ce-machbarkeitsnachweise). `insert -ce` erweitert dagegen erkannte Source-Aufrufe einschließlich statischer Wrapperdefinitionen direkt und `clean -ce` nimmt diese Erweiterung mit denselben Regeln zurück.

## 1. Einordnung

Trice führt für M20 keinen allgemeinen Runtime-Context mit Push/Pop, Task-local State oder Context-Handles ein. Stattdessen können `bind` und `insert` ausgewählte Trice-Aufrufe beim Build gezielt um zusätzliche Runtime-Werte instrumentieren.

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

`insert -ce` verwendet dieselbe Syntax, schreibt die Erweiterung aber in den Source. Freie kleingeschriebene Selektoren bleiben dort erhalten und fehlen im endgültigen TIL-Template. `clean -ce` erhält dieselben Optionen in derselben Reihenfolge und entfernt ausschließlich nachweislich erzeugte CE-Anteile.

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

Für Floatwerte gelten die bestehenden Trice-Regeln auch bei CE: 32-Bit-Trices benötigen explizit `aFloat(...)`, 64-Bit-Trices `aDouble(...)`. CE fügt keine automatische Float-Konvertierung hinzu. Beispielsweise verwendet `float velocity = 33.33f` die Regel `-ce 'speed:", m/s=%f", aFloat(velocity)'`.

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
trice("MSG:ctx7: hi\n");  // The free lowercase selector is removed.
trice("MSG:Ctx7: hi\n");  // Mixed case remains visible and still selects CE.
```

Bekannte Trice-Tags behalten dagegen ihre bisherigen Darstellungsregeln, auch wenn sie zugleich CE-Selektoren sind. Ihr Präfix bleibt im endgültigen Template erhalten: Bei `-color off` ist etwa `info:` weiterhin sichtbar, bei aktiver Farbbehandlung wird es wie bisher behandelt. Ihre Tag-Metadaten bleiben erhalten. Die oben beschriebene CE-Entfernung betrifft freie Selektorpräfixe.

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

Ein Selektor, der an derselben Logstelle mehrfach vorkommt, wird nur einmal wirksam; `bind` beziehungsweise `insert` warnt beim Erweitern über das Duplikat.

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

Zusätzliche Runtime-Werte sind auf skalare Trices mit insgesamt höchstens zwölf Werten derselben Bitbreite begrenzt. String-, Puffer- und andere besondere Trice-Familien können nur eine reine Texterweiterung erhalten, wenn das endgültige Format weiterhin gültig ist; CE mischt dort keine zusätzlichen skalaren Werte in den Record.

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

Vor der eigentlichen M20-Implementierung muss ein isolierter PoC nachweisen, dass diese Callsite-Injektion mit dem Bind-Sidecar technisch sauber funktioniert. Maßgeblich ist der [Arbeitsplan](Implementierungsplan.md) mit seinem PoC-Schritt.

Der PoC muss mindestens zeigen:

- zusätzlicher lokaler Wert bei einem bisher argumentlosen Trice;
- Ergänzung eines CE-Werts hinter bereits vorhandenen Trice-Argumenten;
- einfacher injizierter Ausdruck wie `x + 1`;
- exakt einmalige Auswertung, auch bei einem Testausdruck mit beobachtbarem Seiteneffekt;
- korrekte finale Trice-Arity und Übereinstimmung zwischen übertragenen Werten und `til.json`;
- unveränderten User-Source und idempotente wiederholte Bind-Läufe.

Zusätzlich darf der Mechanismus keine CE-bedingten False-Positive-Warnungen in üblichen compilerbewussten C/C++-Editoren bzw. Language-Servern erzeugen. Es ist zulässig, dass diese dafür das generierte Build-Verzeichnis und die reale Compile-Konfiguration kennen müssen. Nicht akzeptabel wäre ein Design, das zwar vom Build-Compiler angenommen wird, im normalen Editor aber systematisch falsche Argumentzahl-, Syntax- oder Identifierfehler anzeigt.

Die erfolgreiche Abarbeitung dieses PoC ist eine Implementierungsvoraussetzung für M20. Schlägt der Nachweis fehl, ist die Bind/CE-Architektur vor weiterer M20-Arbeit neu zu bewerten.

**A9-Ergebnis:** Der [isolierte PoC](Context_Enrichment_PoC.md) erfüllt diese Mindestfälle für direkte skalare Bind-Logstellen. Er verwendet den vorhandenen Bind-Dispatcher, prüft reale Binärrecords gegen die erweiterte TIL und besteht als C11 und C++17 einschließlich `clangd` mit realer Compile-Konfiguration. Die Source bleibt unverändert, Wiederholungsläufe sind bytegleich und fehlende Context-Bezeichner bleiben Compiler-/Language-Server-Fehler. Diese Voraussetzung war vor der produktiven A10-Implementierung erfüllt.

**A10-Ergebnis:** Die öffentliche CLI, finale Schema-/ID-Vergabe, Sidecar-Adapter, Feldregister und `generate -logC` sind integriert. Die produktiven Tests ergänzen 8/16/32/64-Bit-Werte, Stempelvarianten, Konfigurationswechsel und Ablehnungsfälle. Die C11-/C++17- und `clangd`-Prüfungen laufen ohne `__COUNTER__`; echte Target-Records werden als Text, JSON und KV dekodiert. Nicht ausgeführte Aufrufe sowie `TRICE_OFF` und `TRICE_CLEAN` werten weder ursprüngliche noch injizierte Ausdrücke aus. Einzelheiten und reproduzierbare Befehle stehen im UM. CE für Wrappermakros und Counter-Rebase bleibt gesondert zurückgestellt.

### Erste Ausbaustufe und spätere Erweiterung

Für die erste Ausbaustufe muss `bind` jeden ausgewählten direkten Trice-Aufruf anhand seiner Datei und Quellzeile eindeutig erkennen können. Das umfasst auch direkte Aufrufe innerhalb normaler und `static inline` Funktionen. Ein Trice-Aufruf pro Zeile vermeidet dabei die Mehrdeutigkeit mehrerer Aufrufe auf derselben Zeile. Ein mehrzeiliger Aufruf ist zulässig, wenn auf keiner seiner belegten Zeilen eine andere Bind-Logstelle liegt. Dieser Weg benötigt kein `__COUNTER__`; die übrigen Compileranforderungen von Trice bleiben bestehen.

Ein Wrappermakro ist eine Abkürzung, hinter der ein oder mehrere Trice-Aufrufe stehen. Wenn mehrere Aufrufe an derselben Stelle erscheinen, verwendet Bind bisher teilweise einen zusätzlichen Zähler des Compilers, `__COUNTER__`, um sie zu unterscheiden. Dieser Zähler arbeitet beim Übersetzen des Programms und ist kein Laufzeit- oder Cycle-Counter. Er steht nicht bei allen Compilern zur Verfügung.

Bei CE kommt eine weitere Anforderung hinzu: Eine lokale Variable existiert für den Compiler nur innerhalb ihrer Funktion oder ihres Blocks. Die bisherigen erzeugten Rebase-Verzweigungen würden zusätzliche Ausdrücke verschiedener Logstellen gemeinsam zur Prüfung vorlegen. Dadurch kann der Compiler eine Variable an einer Stelle prüfen, an der sie gar nicht verfügbar sein muss. Auch ein später nicht ausgeführter Zweig wird geprüft. Vorhandenes `__COUNTER__` löst dieses Problem deshalb nicht.

A10 lehnt eine durch CE ausgewählte Wrapper-/Rebase-Stelle vor Dateiänderungen klar ab. Eine nicht von CE ausgewählte Stelle behält das bisherige Bind-Verhalten; aus einer CE-Regel für eine direkte Stelle folgt kein pauschales Verbot von Wrappern im restlichen Projekt. Die produktive Abnahme prüft die direkte Variante auch ohne verfügbares `__COUNTER__`.

Die CE-Regeln, finale Schema-/ID-Bestimmung und technische Einfügung der Argumente werden getrennt. So kann die spätere Unterstützung komplexer Stellen auf der ersten Ausbaustufe aufbauen. Ihre Makroerzeugung kann Änderungen benötigen; CE-Syntax, Datenformate und die Tests des sichtbaren Verhaltens sollen weiterverwendet werden. Der direkte Weg bleibt auch danach nutzbar. Die spätere Erweiterung benötigt einen eigenen Architektur-Nachweis und Auftrag.

Das User Manual erklärt unter [bind-limits](../TriceUserManual.md#bind-limits) die konkreten Alternativen: Aufrufe auf getrennte Zeilen stellen, geeignete Wrapper durch Funktionen ersetzen oder den dauerhaft verfügbaren `insert/clean`-Workflow verwenden. Benötigte Werte müssen beim Wechsel zu einer Funktion ausdrücklich als Parameter übergeben werden. Für `insert/clean` ist inzwischen auch die automatische, reversible CE-Erweiterung umgesetzt; explizit geschriebene zusätzliche Logwerte bleiben ebenso möglich.

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

Für die ID gilt das endgültige übertragene Schema aus Trice-Typ und kanonischem Template-String einschließlich der strukturierten Feldnamen. Der Wechsel eines C-Ausdrucks allein, beispielsweise von `pos.x` zu `pos.y` bei unverändertem explizitem Feldnamen und Typ, ändert das Sidecar, aber nicht die ID. Ein geändertes Schema erhält die dazu passende ID nach den bestehenden Vergaberegeln. Ohne passende `-ce`-Regel entstehen weder zusätzliche Target-Werte noch eine CE-Transformation.

Gleicher Source plus gleiche CE-Konfiguration muss bei wiederholtem `bind` dasselbe kanonische Ergebnis und dieselbe ID-Zuordnung ergeben.

Die gewünschte Regelliste wird bei jedem Bind-Lauf vollständig angegeben; ohne `-ce` werden vorherige Regeln nicht fortgeschrieben. Sidecar-Metadaten verknüpfen das finale Schema mit dem ursprünglichen Source-Aufruf und der vergebenen ID. `generate -logC` liest diese Daten ohne erneute `-ce`-Optionen und weist veraltete oder widersprüchliche CE-Fakten ab. Nach einer Source-Änderung ist deshalb zuerst erneut zu binden.

## 9. Fehlervertrag

Folgende Fälle sind Fehler und müssen vor einem inkonsistenten Schreibzustand scheitern:

- Anzahl Format-Platzhalter und CE-Ausdrücke passt nicht zusammen.
- Durch CE entstehen doppelte strukturierte Feldnamen im finalen Record.
- Die resultierende Trice-Arity oder Makrofamilie ist nicht unterstützt.
- Eine nicht unterstützte Bitbreiten-/Wrapper-Kombination würde entstehen.
- Die CE-Regel selbst ist syntaktisch ungültig.
- Eine passende CE-Regel wählt eine Wrapper-/Rebase-Stelle außerhalb der ersten Ausbaustufe aus. Dieser Fall muss vor Schreibzugriffen auf Source, Sidecars, TIL, LI und Feldregister scheitern.

Bei der Ablehnung eines nicht unterstützten Bind-Konstrukts nennt die Fehlermeldung Datei, Zeile und den konkreten Grund, ergänzt um `Search UM for "bind-limits".`. Dieser kurze Verweis gilt auch für bestehende Bind-Grenzen; die ausführliche Erklärung und mögliche Codeanpassungen stehen im UM. Ein CE-Beispiel lautet:

```text
main.c:42: error: CE requires a direct, line-addressable bind site. Search UM for "bind-limits".
```

Auch der generierte Compilerfehler für einen benötigten, aber nicht verfügbaren `__COUNTER__` verweist auf diesen UM-Abschnitt. Eine abgewiesene CE-Regel wird weder stillschweigend ignoriert noch automatisch durch `insert` umgesetzt.

Ein C-Ausdruck, der an der ausgewählten Logstelle semantisch ungültig oder nicht sichtbar ist, führt spätestens beim Compiler zu einem klaren Buildfehler.

Jeder CE-Ausdruck darf pro tatsächlichem Trice-Aufruf genau einmal ausgewertet werden. Ausdrücke mit Seiteneffekten dürfen durch die Bind-Instrumentierung nicht dupliziert werden.

## Aktueller Scope

M20 implementiert CE zunächst ausschließlich für direkte, eindeutig über ihre Quellzeile adressierbare `bind`-Logstellen. CE für Wrappermakros und Counter-Rebase gehört nicht zu A10 und wird als eigene Folgeaufgabe behandelt.

Der nachfolgend beauftragte `insert/clean -ce`-Workflow ist ebenfalls umgesetzt. Er benötigt keinen Compiler-Vorlauf und keinen Counter. Die Einschränkung auf direkte Bind-Logstellen wird dadurch nicht aufgehoben.

M19 Structured Logging bleibt davon getrennt und unterstützt weiterhin `bind` sowie `insert/clean`.

`insert/clean` bleibt dauerhaft eine Alternative. Die CE-Erweiterung erfolgt dort an den vom Parser erkannten Aufrufen; bei Wrappern wird die statische Definition erweitert. Sichtbarkeit und gültige C-Ausdrücke bleiben Anforderungen an jeden tatsächlichen Aufruf. Bei bereits gebundenem Code ist der im UM beschriebene Rückweg aus Bind zu beachten.

## Reversibler Vertrag für insert und clean

`insert/clean -ce` hält den folgenden Transformationsvertrag ein. Die ausführliche Bedienung und Beispiele stehen im [UM](../TriceUserManual.md#reversibler-ablauf-mit-insert-und-clean).

Es gelten:

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

Die Rücknahmebedingung bezieht sich auf Source im normalen bereinigten ID-Zustand. Bereits vorhandene IDs werden nach den bestehenden Clean-Regeln entfernt oder genullt. Die Garantie gilt, wenn die erzeugten Aufrufe und Herkunftskommentare zwischen `insert` und `clean` nicht manuell verändert wurden und dieselben CE-Optionen verwendet werden. Änderungen außerhalb dieser Aufrufe bleiben erhalten.

Automatisch erzeugte `trice-ce`-Kommentare am Aufruf halten den ursprünglichen Aufruf, die relevanten Regeln, die Tag-Einordnung und einen Fingerabdruck der vollständigen geordneten Regelliste fest. Damit hängen Rücknahme und Idempotenz nicht von Zeilennummern oder externen Build-Dateien ab. Ein fehlender Kommentar ist kein Anlass, gleichlautende handgeschriebene Suffixe zu entfernen. Abweichende Regeln, geänderte erweiterte Aufrufe sowie beschädigte oder abgetrennte Kommentare werden vor Veröffentlichung abgewiesen.

Ohne `-ce` führen Insert und Clean nur ihre bisherige ID-Aufgabe aus; vorhandene CE-Anteile bleiben erhalten. Der CE-Pfad umgeht den zeitstempelbasierten Legacy-Cache, prüft alle ausgewählten Quellen vorab und veröffentlicht Source, TIL, LI und das Insert-Feldregister mit Rücknahme bei Schreibfehlern. `generate -logC` liest das endgültige Schema ohne erneute CE-Regeln aus TIL und geprüften Herkunftsangaben. Die [Verhaltenstests](../../internal/id/contextSource_test.go) und [CLI-/Target-Tests](../../internal/args/context_enrichment_test.go) sichern diesen Vertrag ab.

## 12. Charakter des Ansatzes

`-ce` ist keine allgemeine Context-Vererbung, sondern eine optionale statisch ausgewählte Build-Time-Instrumentierung mit Runtime-Werten. Dadurch entstehen weder globaler Context-State noch besondere Probleme durch Taskwechsel oder Interrupts. Gleichzeitig kann zusätzliche Diagnoseinformation für bestimmte Builds aktiviert werden, ohne die User-Logstellen einzeln zu ändern.

## 13. Referenzen

- Go `slog.Logger.With`: https://pkg.go.dev/log/slog
- Microsoft `ILogger.BeginScope`: https://learn.microsoft.com/dotnet/api/microsoft.extensions.logging.ilogger.beginscope
- Serilog `LogContext`: https://github.com/serilog/serilog/wiki/Enrichment
- Rust `tracing` spans: https://docs.rs/tracing/latest/tracing/span/
