<a id="sl-redaktion"></a>

**Redaktioneller Kommentar — nicht Teil des UM.** 

Dieser Entwurf beschreibt eine mögliche allgemeine Structured-Logging-Erweiterung für Trice; sie ist noch nicht implementiert. Bevorzugt wird **A: ein Message-Template mit benannten oder automatisch aus einfachen Argumenten abgeleiteten Feldnamen**. Das Drahtformat ändert sich nicht: Das Target überträgt weiterhin ID und Werte; Feldnamen bleiben Host-/Wörterbuchinformation.

Eine optionale Feldnamen-Registry kann durch `bind`/`insert` automatisch und schweigend erzeugt und erweitert und anschließend manuell reviewed werden. **C: explizite Name/Wert-Paare** bleibt als möglicher späterer syntaktischer Zucker offen. Ein separat definiertes Event-Schema nach Art von ETW/LTTng (B) ist etabliert, für die allgemeine Trice-Logstelle aber deutlich schwergewichtiger und löst insbesondere das Problem vertauschter gleichartiger Werte nicht.

---

## <a id="strukturiertes-logging"></a>Strukturiertes Logging

### 1. Was ist es?

Strukturiertes Logging bewahrt die Werte einer Meldung zusätzlich zum lesbaren Text als einzeln benannte Felder auf. Aus

```c
strice("info:Motor {motor_id}: {temperature_c} C",
       motor_id, aFloat(temperature_c));
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

### 2. Gängige Methoden

Etablierte Systeme verwenden im Wesentlichen drei Modelle:

- **Benannte Message-Templates (A):** Serilog und Microsoft Logging verwenden Platzhalter wie `{motor_id}`. Name und Wert bleiben als strukturierte Information erhalten; die Zuordnung der Argumente erfolgt üblicherweise positionsabhängig. Rust `tracing` kennt zusätzlich die Kurzform, einen Variablennamen zugleich als Feldname und Wert zu verwenden.
- **Separates Event-Schema (B):** ETW und LTTng definieren Ereignisse mit festen Namen, Typen und Feldern getrennt von der Aufrufstelle. Das ist sinnvoll für einen zentral verwalteten Katalog wiederverwendbarer Ereignistypen. Es erkennt jedoch nicht Vertauschung wie `Event(temperature_c, motor_id)`, wenn beide Parameter denselben Typ haben.
- **Explizite Name/Wert-Paare (C):** beispielsweise Go `slog`: `"motor_id", motor_id`.

B ist daher kein sichererer Ersatz für A, sondern löst ein anderes Problem: die zentrale Definition stabiler Ereignisschemata.

### 3. Sinnvolle Optionen für Trice

Für Trice passt A besonders gut, weil Feldnamen vollständig auf dem Host bleiben können und weder Target-Codegröße noch Übertragungsbandbreite erhöhen müssen.

**Expliziter Feldname:**

```c
strice("info:Motor {motor_id}: {temperature_c} C",
       motor, aFloat(temp));
```

Der Feldname ist stabil und unabhängig vom C-Ausdruck. `bind`/`insert` kann bei einfachen Argumenten zusätzlich prüfen, ob Platzhalter und Argumentname plausibel zusammenpassen.

**Feldname automatisch aus dem Argument:**

```c
strice("info:Motor {}: {} C",
       motor_id, aFloat(temperature_c));
```

Bei `{}` wird der Name eines einfachen C-Bezeichners übernommen. Bei `aFloat(x)` und `aDouble(x)` wird `x` als Name verwendet. Damit sind Feldname und Wert gekoppelt; ein Tippfehler im Bezeichner wird normalerweise bereits vom Compiler erkannt.

Ohne explizite Darstellung gilt als Default:

```text
aFloat(...) / aDouble(...) -> %f
sonst                       -> %d
```

Eine abweichende Darstellung kann nach dem ersten `:` frei angegeben werden:

```c
strice("Temperature{: = %.1f |}", aFloat(temperature_c));
strice("Motor{motor_id:: %d, }", motor_id);
```

Alles zwischen dem ersten `:` und der schließenden `}` ist Darstellungstext einschließlich genau eines `printf`-Formatspezifizierers. Der erste Doppelpunkt trennt nur Feldname und Darstellung; weitere Doppelpunkte sind normaler Text.

Für Ausdrücke, aus denen kein stabiler Feldname eindeutig abgeleitet werden kann, ist ein expliziter Name erforderlich:

```c
strice("{temperature_c:%.1f C}", aFloat(getTemperature()));
```

**Nicht als primäre Lösung vorgesehen:**

- Eine manuell gepflegte Zuordnung `arg0 -> motor_id` außerhalb der Logstelle ist fehleranfällig.
- Ein vollständiges B-Modell mit zentralen Event-Schemata und generierter API ist möglich, aber für allgemeines Trice-Logging unnötig schwergewichtig. Es wäre nur bei Bedarf an einem verbindlichen, vielfach wiederverwendeten Ereigniskatalog sinnvoll.

**C als spätere Option:**

```c
striceX("info:Motor temperature %d: %.1f C", "motor_id", motor_id,
        "temperature_c", aFloat(getTemperature());
striceX("info:Motor temperature %d: %.1f C", "", motor_id,
        "temperature_c", aFloat(getTemperature()); // possible short form
```

C kann später als syntaktischer Zucker ergänzt werden. Hostseitig kann es in dasselbe Datenmodell wie A überführt werden; das Drahtformat muss sich dadurch nicht ändern.

### 4. Entwurf für A mit Kurzform und optionaler Registry

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

Alle Varianten erzeugen dasselbe Host-Schema, beispielsweise:

```text
ID 4711:
    arg0 -> motor_id
    arg1 -> temperature_c
```

Optional führt das Trice Tool eine Feldnamen-Registry. Sie wird beim ersten Lauf **automatisch und ohne Rückfrage erzeugt**, bei späteren Läufen ebenso **schweigend erweitert und aktualisiert** und anschließend wie eine normale Projektdatei manuell reviewed.

Beispiel:

```text
motor_id          12
temperature_c     27
```

Die Registry ist alphabetisch sortiert; die Zahl gibt an, an wie vielen Logstellen der Name im analysierten Quellbestand verwendet wird. Dadurch fallen beim Review ähnliche oder versehentlich neu entstandene Namen wie `temperature_c` und `temperture_c` leicht auf. Die Registry ist eine Qualitätskontrolle, keine Voraussetzung für das Drahtprotokoll und kein Ersatz für Compiler- oder Build-Prüfungen.

**Tags und Level:** Beginnt der Formatstring mit einem bereits bekannten Trice-Tag beziehungsweise einem seiner Aliase, übernimmt der Host dafür den festen Feldnamen `tag`. Als Wert wird immer der **erste Eintrag der zugehörigen Tag-Gruppe als kanonischer Name** verwendet, unabhängig davon, welcher Alias im Formatstring steht. Die kanonischen Namen stehen deshalb jeweils an erster Stelle in der jeweilen Tag Gruppe im Go Code `./internal/emitter/lineTransformerANSI.go` und sind einheitlich großgeschrieben. Beispielsweise kann `msg:`, `message:` oder `MESSAGE:` zu

```json
"tag": "MESSAGE"
```

normalisiert werden. Ist kein bekannter Tag vorhanden, wird kein `tag`-Feld erzeugt; ein künstlicher Wert wie `untagged` ist nicht erforderlich.

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

**Vorgesehener Ablauf:** A als Grundmodell implementieren, `{}` als kurze und sichere Namensableitung für einfache Bezeichner einschließlich `aFloat()`/`aDouble()` zulassen, optionale Darstellungen nach `:` unterstützen und alles in dasselbe Host-Datenmodell überführen. Die Registry kann optional durch `bind`/`insert` gepflegt werden; C kann später ohne Änderung dieses Modells ergänzt werden.

### Referenzen

- Microsoft Logging: https://learn.microsoft.com/dotnet/core/extensions/logging
- Microsoft source-generated Logging: https://learn.microsoft.com/dotnet/core/extensions/high-performance-logging
- Go `slog`: https://pkg.go.dev/log/slog
- Rust `tracing`: https://docs.rs/tracing/latest/tracing/
- LTTng Tracepoints: https://lttng.org/docs/v2.14/
