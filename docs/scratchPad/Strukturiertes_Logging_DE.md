**Redaktioneller Kommentar — nicht Teil des UM.** 

Dieser aktuelle Entwurf beschreibt die geplante Structured-Logging-Erweiterung für Trice; sie ist noch nicht implementiert. Gewählt ist **A: ein Message-Template mit benannten oder automatisch aus einfachen Argumenten abgeleiteten Feldnamen**. Das Drahtformat ändert sich nicht: Das Target überträgt weiterhin ID und Werte; Feldnamen bleiben Host-/Wörterbuchinformation.

Die Bedienungsrichtung ist festgelegt. Die verbleibenden Verträge für Export, Wörterbuch, Typen und Kompatibilität stehen im [Implementierungsplan](Implementierungsplan.md). Insbesondere ist die JSON-Darstellung mehrfach gleicher Feldnamen noch nicht festgelegt. Die Beispiele mit eindeutigen Feldnamen nehmen diese Entscheidung nicht vorweg. Umsetzung erst auf ausdrückliches Kommando.

Eine optionale Feldnamen-Registry kann durch `bind`/`insert` automatisch und schweigend erzeugt und erweitert und anschließend manuell reviewed werden. **C: explizite Name/Wert-Paare** bleibt als möglicher späterer syntaktischer Zucker offen. Ein separat definiertes Event-Schema nach Art von ETW/LTTng (B) ist etabliert, für die allgemeine Trice-Logstelle aber deutlich schwergewichtiger und löst insbesondere das Problem vertauschter gleichartiger Werte nicht.

---

## Strukturiertes Logging

### Was ist es?

Strukturiertes Logging bewahrt die Werte einer Meldung zusätzlich zum lesbaren Text als einzeln benannte Felder auf. Aus

```c
trice("info:Motor {motor_id}: {temperature_c} C", motor_id, aFloat(temperature_c));
```

kann der Host beispielsweise erzeugen:

```json
{
  "tag": "INFO",
  "level": "INFO",
  "fields": {"motor_id": 3, "temperature_c": 87.5},
  "message": "Motor 3: 87.500000 C"
}
```

Ein Auswerteprogramm kann damit direkt `temperature_c > 80` prüfen, ohne Text zu parsen. JSON ist nur ein mögliches Ausgabeformat; auf dem Draht genügt weiterhin die kompakte Trice-ID mit ihren Werten.

### Gängige Methoden

Etablierte Systeme verwenden im Wesentlichen drei Modelle:

- **Benannte Message-Templates (A):** Serilog und Microsoft Logging verwenden Platzhalter wie `{motor_id}`. Name und Wert bleiben als strukturierte Information erhalten; die Zuordnung der Argumente erfolgt üblicherweise positionsabhängig. Rust `tracing` kennt zusätzlich die Kurzform, einen Variablennamen zugleich als Feldname und Wert zu verwenden.
- **Separates Event-Schema (B):** ETW und LTTng definieren Ereignisse mit festen Namen, Typen und Feldern getrennt von der Aufrufstelle. Das ist sinnvoll für einen zentral verwalteten Katalog wiederverwendbarer Ereignistypen. Es erkennt jedoch auch nicht Vertauschung wie `Event(temperature_c, motor_id)`, wenn beide Parameter denselben Typ haben.
- **Explizite Name/Wert-Paare (C):** beispielsweise Go `slog`: `"motor_id", motor_id`.

B ist daher kein sichererer Ersatz für A, sondern löst ein anderes Problem: die zentrale Definition stabiler Ereignisschemata.

### Gewählter Ansatz für Trice

Für Trice passt A besonders gut, weil Feldnamen vollständig auf dem Host bleiben können und weder Target-Codegröße noch Übertragungsbandbreite erhöhen müssen.

Eine zusätzliche Target-API ist dafür nicht erforderlich: `trice()` bleibt unverändert die normale Aufrufform. `bind`/`insert` interpretiert die erweiterte Formatstring-Syntax und speichert den daraus abgeleiteten Host-Formatstring sowie die strukturierten Feldnamen in `til.json`; das Target überträgt weiterhin nur ID und Werte.

**Expliziter Feldname:**

```c
trice("info:Motor {motor_id}: {temperature_c} C", motor, aFloat(temp));
```

Der Feldname ist stabil und unabhängig vom C-Ausdruck. 

<!-- Ob Platzhalter und Argumentname plausibel zusammenpassen, könnte in einem zusätzlichen separatem `check` Kommando geprüft werden. Das wäre aber ein optionaler nachgelagerter Erntwicklungsschritt. -->

**Feldname automatisch aus dem Argument:**

```c
trice("info:Motor {}: {} C", motor_id, aFloat(temperature_c));
```

Bei `{}` wird der Name eines einfachen C-Bezeichners übernommen. Bei `aFloat(x)` und `aDouble(x)` wird `x` als Name verwendet. Damit sind Feldname und Wert gekoppelt; ein Tippfehler im Bezeichner wird normalerweise bereits vom Compiler erkannt.

Ohne explizite Darstellung gilt als Default:

```text
aFloat(...) / aDouble(...) -> %f
sonst                      -> %d
```

Eine abweichende Darstellung kann nach dem ersten `:` frei angegeben werden, um direkte Logs gefälliger lesbar zu machen, ohne den (zusätzlichen) strukturierten Output zu beeinflussen:

```c
trice("Temperature{: = %.1f |}", aFloat(temperature_c));
trice("Motor{motor_id:: %d, }", motor_id);
```

Alles zwischen dem ersten `:` und der schließenden `}` ist Darstellungstext einschließlich genau eines `printf`-Formatspezifizierers. Der erste Doppelpunkt trennt nur Feldname und Darstellung; weitere Doppelpunkte sind normaler Text.

Für Ausdrücke, aus denen kein stabiler Feldname eindeutig abgeleitet werden kann, ist ein expliziter Name erforderlich:

```c
trice("{temperature_c:%.1f C}", aFloat(getTemperature()));
```

**Nicht als primäre Lösung vorgesehen:**

- Eine manuell gepflegte Zuordnung `arg0 -> motor_id` außerhalb der Logstelle ist fehleranfällig.
- Ein vollständiges B-Modell mit zentralen Event-Schemata und generierter API ist möglich, aber für allgemeines Trice-Logging unnötig schwergewichtig. Es wäre nur bei Bedarf an einem verbindlichen, vielfach wiederverwendeten Ereigniskatalog sinnvoll und könnte später als unabhängige Option gestaltet werden.

**C als spätere Option:**

```c
triceX("info:Motor %d: %.1f C", "motor_id", motor_id, "temperature_c", aFloat(Temp()));
triceX("info:Motor %d: %.1f C",         "", motor_id, "temperature_c", aFloat(Temp())); // short form
```

C kann später als syntaktischer Zucker ergänzt werden. Hostseitig kann es in dasselbe Datenmodell wie A überführt werden; das Drahtformat muss sich dadurch nicht ändern.

### Entwurf für A mit Kurzform und optionaler Registry

Die Platzhalter folgen grundsätzlich der Form

```text
{ [name] [ ":" Darstellung ] }
```

`bind`/`insert` extrahiert für jede strukturierte Logstelle ID, Feldnamen, Argumentpositionen und die für Trice erforderliche Konvertierung. Beispiele:

```c
{}                         // Name aus Argument, Defaultdarstellung
{motor_id}                 // expliziter Name, Defaultdarstellung
{:%.1f}                    // Name aus Argument, explizite Darstellung
{: = %.1f C}               // Name aus Argument, freier Darstellungstext
{temperature_c:%.1f C}     // expliziter Name und Darstellung
{motor_id:: %d, }          // Name motor_id, Darstellung ": %d, "
```

Klassische `printf`-Platzhalter und strukturierte Platzhalter dürfen im selben Formatstring gemischt werden. Beide konsumieren Argumente in ihrer Reihenfolge; nur `{...}` erzeugt zusätzlich ein strukturiertes Feld:

```c
trice("%d,{}", a, x);      // %d -> a; {} -> Feld x
```

Literale geschweifte Klammern werden als `{{` und `}}` geschrieben:

```c
trice("set={{1,2,3}}, value={}", value);
```

`bind`/`insert` verarbeitet den Formatstring von links nach rechts. `%...` erzeugt klassische Textformatierung, `{...}` Textformatierung plus Feldmetadaten, `{{`/`}}` erzeugt nur literale Klammern. Bestehende Legacy-`til.json` benötigen dafür keinen zusätzlichen Kompatibilitätspfad im neuen Parser; erforderliche Altbestände können separat migriert werden.

**Hierarchische Feldnamen:** Punkte trennen Namenssegmente. Einfache C-Memberketten können automatisch abgeleitet werden; `.` und `->` werden dabei beide zu `.` kanonisiert. `aFloat()` und `aDouble()` sind für die Namensableitung transparente Hüllen.

```c
"{motor.temperature_c}", x            // -> motor.temperature_c
"{}", motor.temperature_c             // -> motor.temperature_c
"{}", motor->temperature_c            // -> motor.temperature_c
"{}", controller->motor.temperature_c // -> controller.motor.temperature_c
"{motor.}", temperature_c             // -> motor.temperature_c
```

`{prefix.}` ergänzt somit einen expliziten Präfix um den aus dem Argument abgeleiteten Namen. Automatische Ableitung ist nur für Bezeichner und reine `.`/`->`-Memberketten vorgesehen. Bei `&`, `*`, Arrayzugriffen, allgemeinen Funktionsaufrufen oder Ausdrücken ist ein expliziter Feldname erforderlich, beispielsweise:

```c
"{motor.temperature_c}", &motor->temperature_c
```

Der **aufgelöste kanonische Feldname** wird in `til.json` gespeichert; `{}` selbst ist nur Source-Kurzform. Der Feldname ist Bestandteil des Trice-Schemas: Ändert sich beispielsweise `motor.temperature_c` zu `motor.temperature_f`, ist eine neue Trice-ID erforderlich. Ein Wechsel im C-Code von `motor.temperature_c` zu `motor->temperature_c` erfordert dagegen keine neue ID, da beide denselben kanonischen Feldnamen ergeben.

**Ausgabe mit `trice log`:** `bind`/`insert` speichert für `{...}` den effektiven Host-Formatstring in `til.json`. `trice log` setzt die empfangenen Werte entsprechend dieser Darstellung ein und erzeugt damit weiterhin die normale Textausgabe. Ein neuer CLI-Schalter `-logFormat` wählt die äußere Darstellung:

```text
-logFormat text   // Default, bisherige Ausgabe
-logFormat json   // ein JSON-Objekt pro Logrecord
-logFormat kv     // ein key=value-Record pro Zeile
```

Alle drei Formate verwenden denselben dekodierten Logrecord. Die Darstellung innerhalb `{...}` beeinflusst nur `message`; strukturierte Feldwerte bleiben typisiert und unformatiert. Beispielsweise kann `%.1f C` im Text `87.5 C` erzeugen, während das strukturierte Feld weiterhin den numerischen Wert `87.5` enthält. Hierarchische Feldnamen bleiben dabei als kanonische Punktnamen erhalten.

Bei `json` und `kv` erscheinen Metafelder nur, wenn die entsprechende bestehende Trice-Ausgabeoption aktiv ist und der Wert vorhanden ist: beispielsweise `id`, Target-Zeitstempel `ts`, Host-Zeitstempel `hs` sowie `file` und `line`. `tag` und gegebenenfalls `level` werden wie unten beschrieben ergänzt. Die bestehende Wertformatierung für `ts` und `hs` kann übernommen werden; reine Textdekoration wie Padding, Spaltentrenner oder ANSI-Sequenzen entfällt. Ein zusätzlicher allgemeiner `-ignore`-/`-suppress`-Schalter ist zunächst nicht vorgesehen.

Beispiel für JSON:

```json
{"tag":"INFO","level":"INFO","message":"Motor 3: 87.5 C","fields":{"motor_id":3,"temperature_c":87.5}}
```

Beispiel für `kv`:

```text
tag=INFO level=INFO message="Motor 3: 87.5 C" motor_id=3 temperature_c=87.5
```

Alle Varianten erzeugen dasselbe Host-Schema, welches implizit in `til.json` vorliegt, beispielsweise:

```text
ID 4711:
    arg0 -> motor_id
    arg1 -> temperature_c
```

Optional führt das Trice Tool eine Feldnamen-Registry. Sie wird beim ersten Lauf **automatisch und ohne Rückfrage erzeugt**, bei späteren Läufen ebenso **schweigend erweitert und aktualisiert** und anschließend wie eine normale Projektdatei manuell reviewed.

Beispiel:

```text
motor_id          12
s.t.u              3
temperature_c     27
```

Die Registry ist alphabetisch sortiert; die Zahl gibt an, an wie vielen Logstellen der Name im analysierten Quellbestand verwendet wird. Dadurch fallen beim Review ähnliche oder versehentlich neu entstandene Namen wie `temperature_c` und `temperture_c` leicht auf. Die Registry ist eine Qualitätskontrolle, keine Voraussetzung für das Drahtprotokoll und kein Ersatz für Compiler- oder Build-Prüfungen.

**Tags und Level:** Beginnt der Formatstring mit einem bereits bekannten Trice-Tag beziehungsweise einem seiner Aliase, übernimmt der Host dafür den festen Feldnamen `tag`. Als Wert wird immer der **erste Eintrag der zugehörigen Tag-Gruppe als kanonischer Name** verwendet, unabhängig davon, welcher Alias im Formatstring steht. Die kanonischen Namen stehen jeweils an erster Stelle in [lineTransformerANSI.go](../../internal/emitter/lineTransformerANSI.go). Die eingebauten Namen sind großgeschrieben, mit Ausnahme des reservierten `untagged`; freie User-Labels behalten ihre Schreibweise. Beispielsweise kann `msg:`, `message:` oder `MESSAGE:` zu

```json
"tag": "MESSAGE"
```

normalisiert werden. Für Anwendungsmeldungen ohne bekannten Formatstring-Tag liefert die vorhandene Hostklassifikation die Gruppe `untagged`. Ob eine künftige strukturierte Ausgabe dafür ein Feld `"tag": "untagged"` schreibt, ist mit M19 festzulegen; sie darf die Klassifikation nicht erneut aus Laufzeittext ableiten.

Tags, die zugleich Log-Level darstellen, werden zusätzlich über eine kleine Liste kanonischer Namen gekennzeichnet, gedanklich etwa:

```text
levelTags = {"DEBUG", "INFO", "WARNING", "ERROR", ...}
```

Nach der normalen Alias-Auflösung genügt damit ein Vergleich des kanonischen Tag-Namens mit `levelTags`. Ist er dort enthalten, erzeugt der Host zusätzlich den festen Feldnamen `level` mit demselben kanonischen Wert:

```json
"tag": "INFO",
"level": "INFO"
```

Bei entsprechendem Bedarf kann die levelTags Liste per CLI Switch modifiziert werden.

`tag` und `level` sind damit reservierte, feste Host-Feldnamen; projektspezifische Umbenennungen sind nicht vorgesehen. Die Feldnamen-Registry führt auch diese Host-Felder, also können `tag` und `level` automatisch eingetragen und gezählt werden. Die Aliasnamen selbst gehören nicht in die Registry. Die gedankliche Implementierung bleibt dadurch klein: kanonischer Name an Position 0 jeder Tag-Gruppe, Alias-Auflösung wie bisher, danach optionaler Membership-Test gegen `levelTags`.

**User-Labels:** `-ulabel` ist bereits vorhanden und bleibt davon orthogonal. Trifft sein Selektor einen fest eingebauten Trice-Tag oder einen seiner Aliase, wirkt die Zuordnung auf die gesamte bekannte Tag-Gruppe; beispielsweise gilt `-ulabel msg:300` für alle MESSAGE-Aliase. Freie User-Labels bilden dagegen keine Alias-Gruppen, sodass `new` und `NEW` verschieden bleiben. Zusätzlich zum numerischen Gewicht akzeptiert `-ulabel` bereits Farben aus genau dem von `trice generate -colors` erzeugten Farbvokabular, beispielsweise `-ulabel msg:red:blue`. Gewicht und Farbe sind unabhängige Eigenschaften und können für dieselbe Gruppe getrennt angegeben werden. Dies ändert weder Structured-Logging-Felder noch Drahtformat oder CE-Semantik.

**Vorgesehener Ablauf:** A direkt als Erweiterung von `trice()` implementieren, `{}` als kurze und sichere Namensableitung für einfache Bezeichner einschließlich `aFloat()`/`aDouble()` zulassen, `%...` und `{...}` gemeinsam parsen, `{{`/`}}` als Klammer-Escape unterstützen, optionale Darstellungen nach `:` erlauben und alles in dasselbe Host-Datenmodell überführen. Die Registry kann optional durch `bind`/`insert` gepflegt werden; C kann später ohne Änderung dieses Modells ergänzt werden.

### Referenzen

- Microsoft Logging: https://learn.microsoft.com/dotnet/core/extensions/logging
- Microsoft source-generated Logging: https://learn.microsoft.com/dotnet/core/extensions/high-performance-logging
- Go `slog`: https://pkg.go.dev/log/slog
- Rust `tracing`: https://docs.rs/tracing/latest/tracing/
- LTTng Tracepoints: https://lttng.org/docs/v2.14/
