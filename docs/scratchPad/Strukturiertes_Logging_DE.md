**Redaktioneller Kommentar — nicht Teil des UM.**

M19 ist implementiert. Dieses Dokument bleibt die Entwurfsreferenz; das [deutsche UM-Kapitel](../TriceUserManual.md#strukturiertes-logging) beschreibt die vorhandene Funktion, konkrete Optionen und Migration. Der bestätigte Scope umfasst skalare Felder sowie Strings über `triceS`/`triceN`; benannte Pufferfelder werden mit einem klaren Fehler abgewiesen. Klassische Pufferlogs bleiben als `message` erhalten. Das Drahtformat ändert sich nicht: Das Target überträgt weiterhin ID und Werte; Feldnamen und Typsemantik bleiben Host-/Wörterbuchinformation.
Dieser Entwurf beschreibt die geplante Structured-Logging-Erweiterung M19 für Trice; sie ist noch nicht implementiert. Die für den aktuellen Scope nötigen Designentscheidungen sind abgeschlossen. Das Drahtformat ändert sich nicht: Das Target überträgt weiterhin ID und Werte; Feldnamen und Typsemantik bleiben Host-/Wörterbuchinformation.

---

# Strukturiertes Logging

## 1. Ziel und gewählter Ansatz

Trice verwendet benannte Message-Templates. Eine zusätzliche Target-API ist nicht erforderlich; `trice()` bleibt die normale Aufrufform.

```c
trice("info:Motor {motor_id}: {temperature_c} C", motor_id, aFloat(temperature_c));
```

Der Host kann daraus neben der normalen Textmeldung beispielsweise erzeugen:

```json
{
  "tag": "info",
  "level": "INFO",
  "message": "Motor 3: 87.500000 C",
  "fields": {
    "motor_id": 3,
    "temperature_c": 87.5
  }
}
```

Das Target überträgt weiterhin nur ID und Werte. Structured Logging erhöht daher nicht allein durch Feldnamen die Target-Codegröße oder Übertragungsbandbreite.

Ein separates zentrales Event-Schema ist für den allgemeinen Trice-Fall nicht vorgesehen. Explizite Name/Wert-Paare können später als zusätzlicher syntaktischer Zucker ergänzt werden, ohne das Host-Datenmodell zu ändern.

## 2. Template-Syntax

Ein strukturierter Platzhalter hat die Form

```text
{ [name] [ ":" Darstellung ] }
```

Beispiele:

```c
{}                         // Name aus Argument, Defaultdarstellung
{motor_id}                 // expliziter Name, Defaultdarstellung
{:%.1f}                    // Name aus Argument, explizite Darstellung
{: = %.1f C}               // Name aus Argument, freier Darstellungstext
{temperature_c:%.1f C}     // expliziter Name und Darstellung
{motor_id:: %d, }          // Name motor_id, Darstellung ": %d, "
```

Klassische `printf`-Platzhalter und strukturierte Platzhalter dürfen gemischt werden. Beide konsumieren Argumente von links nach rechts; nur `{...}` erzeugt zusätzlich ein strukturiertes Feld.

```c
trice("%d,{}", a, x);      // %d -> a; {} -> Feld x
```

### 2.1 Defaultdarstellung

Ohne explizite Darstellung gilt:

```text
aFloat(...) / aDouble(...) -> %f
sonst                      -> %d
```

Eine explizite Darstellung beeinflusst nur die Textmeldung, nicht den strukturierten Wert.

```c
trice("Temperature{: = %.1f C}", aFloat(temperature_c));
```

### 2.2 Literale geschweifte Klammern

Literale Klammern werden ausschließlich durch Verdopplung geschrieben:

```text
{{ -> {
}} -> }
```

Beispiel:

```c
trice("set={{1,2,3}}, value={value}", value);
```

Textausgabe:

```text
set={1,2,3}, value=17
```

Backslash-Escaping bleibt davon vollständig getrennt. `\n`, `\t`, `\\`, `\"` usw. sind normale C-String-Escapes. `\{` und `\}` erhalten für Structured Logging keine neue Sonderbedeutung.

## 3. Feldnamen und Kanonisierung

### 3.1 Explizite Feldnamen

```c
trice("info:Motor {motor_id}: {temperature_c} C", motor, aFloat(temp));
```

Der Feldname ist stabil und unabhängig vom C-Ausdruck.

### 3.2 Automatische Namensableitung

Bei `{}` wird ein einfacher C-Bezeichner als Feldname übernommen. `aFloat(x)` und `aDouble(x)` sind für die Namensableitung transparente Hüllen.

```c
trice("info:Motor {}: {} C", motor_id, aFloat(temperature_c));
```

Reine Memberketten werden ebenfalls unterstützt. `.` und `->` werden zu `.` kanonisiert:

```text
motor.temperature_c             -> motor.temperature_c
motor->temperature_c            -> motor.temperature_c
controller->motor.temperature_c -> controller.motor.temperature_c
```

`{prefix.}` ergänzt einen expliziten Präfix um den aus dem Argument abgeleiteten Namen:

```c
trice("{motor.}", temperature_c); // -> motor.temperature_c
```

Bei allgemeinen Ausdrücken, Arrayzugriffen, Dereferenzierung oder Funktionsaufrufen ist ein expliziter Feldname erforderlich.

```c
trice("{temperature_c:%.1f C}", aFloat(getTemperature()));
trice("{motor.temperature_c}", &motor->temperature_c);
```

### 3.3 Eindeutigkeit

Feldnamen müssen innerhalb eines einzelnen strukturierten Records eindeutig sein. Ein doppelter Feldname ist ein Fehler und wird von `bind` beziehungsweise `insert` abgewiesen.

Hierarchische Namen wie `motor` und `motor.temperature` sind erlaubt, da es sich um zwei verschiedene kanonische Namen handelt.

## 4. Strukturierte Typen

Trice führt keine C-Typinferenz für beliebige Ausdrücke ein. Der strukturierte Typ ergibt sich aus der Trice-Bitbreite, dem Formatspezifizierer und den vorhandenen Wrappern wie `aFloat`/`aDouble`.

Die globale Zuordnung ist Teil der Trice-Sprachregel und soll zentral im Trice-Code verankert werden; sie wird nicht pro ID in `til.json` gespeichert.

| Formatspezifizierer | Strukturierter Typ |
|---|---|
| `%d`, `%i` | `signed integer` |
| `%u`, `%o`, `%O`, `%x`, `%X`, `%b` | `unsigned integer` |
| `%f`, `%F`, `%e`, `%E`, `%g`, `%G` | `floating point` |
| `%c`, `%q` | `character` |
| `%t` | `boolean` |
| `%s` | `string` |
| `%p` | `address` |
| `%%` | kein Feld, vollständig ignoriert |

Feldbreite, Precision, Groß-/Kleinschreibung und Präfixdarstellung ändern nur die Textdarstellung. Optionen wie `-unsigned=false` dürfen den strukturierten Typ nicht nachträglich umdeuten.

`%p` bedeutet nur, dass Trice eine Adresse kennt. Der Pointee-Typ (`uint8_t*`, `uint32_t*`, Struct-Pointer usw.) wird nicht ermittelt. Der strukturierte Typ heißt deshalb `address`, nicht `pointer` oder `buffer`.

## 5. `til.json`, Kanonisierung und Schemaidentität

Das bestehende JSON-Schema von `til.json` bleibt unverändert. Insbesondere erhält `TriceFmt` keine zusätzlichen Felder für Structured Logging.

`Strg` enthält den vollständig kanonisierten strukturierten Template-String, zum Beispiel:

```json
{
  "4711": {
    "Type": "TRICE32_2",
    "Strg": "info:Motor {motor_id}: {temperature_c:%.1f C}"
  }
}
```

Aus `Strg` kann der Host deterministisch ableiten:

- den klassischen printf-kompatiblen Textformatstring,
- die strukturierten Feldnamen,
- die Argumentpositionen,
- die strukturierten Typen nach der globalen Formatspezifizierer-Regel.

Source-Kurzformen wie `{}` werden vor Speicherung auf den kanonischen Feldnamen aufgelöst. `motor.temperature_c` und `motor->temperature_c` ergeben daher denselben gespeicherten Feldnamen.

`Type + Strg` bleibt die Schemaidentität. Eine Feldumbenennung ändert `Strg` und erfordert dadurch eine neue ID; eine rein syntaktische Änderung mit identischem kanonischem Ergebnis nicht.

Lokale generierte Texttabellen oder Decoder-Artefakte dürfen aus dem kanonischen `Strg` den erforderlichen printf-kompatiblen Formatstring ableiten. Dafür ist keine zweite dauerhafte Schemaquelle erforderlich.

Für M20 Context Enrichment kann `bind` den finalen kanonischen `Strg` gegenüber dem unveränderten User-Source erweitern. Die dazu zusätzlich auf dem Target benötigten Runtime-Ausdrücke werden nicht aus `til.json` gewonnen, sondern müssen callsite-spezifisch über den Bind-Sidecar in die Compilerexpansion injiziert werden. Der technische Nachweis dafür ist eine separate M20-Implementierungsvoraussetzung; siehe [Kontextanreicherung_DE.md](Kontextanreicherung_DE.md).

## 6. Ausgabe mit `trice log`

Ein neuer Schalter wählt die äußere Darstellung:

```text
-logFormat text   // Default, klassische Ausgabe
-logFormat json   // ein JSON-Objekt pro Logrecord
-logFormat kv     // ein key=value-Record pro Logrecord
```

Alle drei Formate verwenden denselben dekodierten Record.

### 6.1 `text`

`text` verhält sich wie die bisherige Trice-Ausgabe. Der strukturierte Template-String wird entsprechend seiner Darstellung formatiert; es werden keine zusätzlichen `fields`, `field.*` oder anderen Structured-Logging-Zusätze an die Textzeile angehängt.

```c
trice("info:Motor {motor_id}: {temperature_c:%.1f C}", motor, aFloat(temp));
```

kann als Text beispielsweise ergeben:

```text
info:Motor 3: 87.5 C
```

Bestehende Textoptionen und Textdekorationen bleiben Sache des bisherigen Textpfads.

### 6.2 `json`

User-Felder stehen ausschließlich unter `fields`, sodass Namen wie `tag`, `level`, `message`, `id` oder `ts` ohne Kollision erlaubt bleiben.

```json
{
  "tag": "info",
  "level": "INFO",
  "message": "Motor 3: 87.5 C",
  "fields": {
    "motor_id": 3,
    "temperature_c": 87.5
  }
}
```

Regeln:

- Ganzzahlen, einschließlich 64-Bit-Werten, werden als JSON-Zahlen ausgegeben. Ein Consumer ist selbst für ausreichende Integer-Präzision verantwortlich.
- Endliche Floating-Point-Werte werden als JSON-Zahlen ausgegeben.
- `NaN`, `+Inf` und `-Inf` sind keine JSON-Zahlen; das betreffende strukturierte Feld wird im JSON vollständig weggelassen. Die formatierte `message`-/Textdarstellung bleibt davon unberührt.
- Sind danach keine strukturierten Felder vorhanden, wird `fields` vollständig weggelassen.
- `boolean` wird als JSON-Boolean ausgegeben.
- `string` und `character` werden als JSON-String ausgegeben.
- `address` wird als String in der üblichen Adressdarstellung, beispielsweise `"0x20001234"`, ausgegeben.
- Die Reihenfolge der User-Felder entspricht der Quell-/Argumentreihenfolge. JSON-Objekte sind semantisch ungeordnet, Trice erzeugt die Ausgabe dennoch deterministisch in dieser Reihenfolge.

Beispiel ohne strukturierte Felder:

```json
{
  "tag": "info",
  "level": "INFO",
  "message": "Motor started"
}
```

### 6.3 `kv`

Host-Metadaten verwenden ihre normalen Namen; User-Felder erhalten immer den Präfix `field.`:

```text
tag=info level=INFO message="Motor 3: 87.5 C" field.motor_id=3 field.temperature_c=87.5
```

Damit sind auch User-Felder wie `{tag}` oder `{message}` kollisionsfrei:

```text
tag=info level=INFO message="..." field.tag=17 field.message="user text"
```

Quoting-Regeln:

- Zahlen, Booleans und Adressen: unquoted.
- Strings und Characters: immer in `"..."`.
- `message`: immer in `"..."`.
- Innerhalb quoted values gelten mindestens `\"`, `\\`, `\n`, `\r`, `\t`.
- User-Felder erscheinen in Quell-/Argumentreihenfolge.

Beispiele:

```text
field.count=17
field.temperature=23.5
field.enabled=true
field.addr=0x20001234
field.name="left motor"
field.character="A"
field.text="Motor \"A\" stopped"
field.path="C:\\tmp\\log"
field.multiline="Hello\nWorld"
```

## 7. Metadaten, Tags und Level

Maschinenlesbare Ausgabe übernimmt vorhandene Metadaten nur, wenn die jeweilige bestehende Trice-Ausgabeoption aktiv ist und ein Wert vorhanden ist. Dazu gehören beispielsweise `id`, `file`, `line`, Target-Stempel `ts` und Host-Stempel `hs`.

- `ts` ist ein roher unsigned Target-Wert. Trice unterstellt keine physikalische Einheit.
- `hs` ist der formatierte Host-Zeitstring.
- Reine Textdekoration wie Padding, Spaltentrenner oder ANSI-Sequenzen gehört nicht in `json` oder `kv`.

`tag` wird für jeden Anwendungsrecord ausgegeben. Ist keine bekannte Klassifikation vorhanden, lautet der Wert `untagged`.

Der Wert von `tag` bewahrt die tatsächlich verwendete Tag-/Label-Schreibweise. `level` ist dagegen ein kanonischer Log-Level-Wert.

Beispiel:

```text
warn:... -> tag="warn", level="WARNING"
Warning:... -> tag="Warning", level="WARNING"
```

Die Level-Erkennung ist case-neutral und basiert logisch auf einer Alias->Canonical-Level-Tabelle. Die Defaulttabelle enthält die gängigen Log-Level-Namen, insbesondere die in Trice gebräuchlichen Gruppen wie `EMERGENCY`, `ALERT`, `CRITICAL`, `FATAL`, `ERROR`, `WARNING`/`WARN`, `NOTICE`, `ATTENTION`, `INFO`, `DEBUG`, `TRACE` und `VERBOSE`. Die Tabelle soll später per CLI modifizierbar sein. Sie ist unabhängig von numerischen `-ulabel`-Gewichten und Farben.

`-ulabel` bleibt orthogonal zu Structured Logging und CE.

## 8. Stabile Ausgabe-Reihenfolge

Für reproduzierbare Logs und Tests wird eine stabile Ausgabe garantiert:

- User-Felder: Quell-/Argumentreihenfolge.
- `kv`: dieselbe Reihenfolge.
- `json`: dieselbe Reihenfolge, obwohl JSON-Objekte semantisch ungeordnet sind.
- optionale Metadaten werden in einer festen Implementierungsreihenfolge ausgegeben; fehlende Metadaten werden ausgelassen.

## 9. Feldnamen-Registry `trice-fields.txt`

Zur manuellen Qualitätskontrolle wird im Build-Verzeichnis bei jedem relevanten Compiler-/Instrumentierungslauf eine neue Datei `trice-fields.txt` erzeugt. Sie ist kein persistenter inkrementeller Katalog.

Sie enthält ausschließlich User- und CE-Feldnamen, keine festen Hostfelder wie `tag`, `level`, `message`, `id`, `ts`, `hs`, `file` oder `line`.

Die Zahl ist die Anzahl der Logstellen des aktuellen Laufs, in denen der kanonische Feldname vorkommt. Da doppelte Feldnamen pro Record verboten sind, ist die Zählung eindeutig.

Ausgabeformat:

```text
%8d %s\n
```

Beispiel:

```text
       1 temperture_c
       1 motor.temperture
       2 voltage_raw
      17 motor.temperature_c
      43 motor.id
```

Sortierung:

1. Count aufsteigend.
2. Bei gleichem Count Feldname alphabetisch.

So stehen seltene und damit besonders reviewwürdige Feldnamen oben. Die Feldnamenlänge wird nicht künstlich begrenzt.

Für `bind` ist das Build-Verzeichnis bereits Teil des Workflows. `insert` benötigt für M19 eine analoge eindeutige Build-Verzeichnis-Regel, damit `trice-fields.txt` ebenfalls nur den aktuellen Lauf repräsentiert.

## 10. Migration vorhandener Klammertexte

Mit Einführung der neuen Syntax bedeutet ein einzelnes `{...}` künftig Structured Logging. Alte literal gemeinte Klammern müssen einmalig zu `{{...}}` migriert werden.

```text
alt: set={1,2,3}
neu: set={{1,2,3}}
```

Es gibt keinen dauerhaften Legacy-Modus und keinen zweiten Klammerparser. Eine einmalige Konvertierung bestehender Quellen und vorhandener `til.json`-Bestände ist akzeptierter Bestandteil der Migration.

Ungültige oder nicht balancierte Structured-Syntax ist ein klarer Fehler bei `bind`/`insert`.

## 11. Implementierungsgrenzen

M19 umfasst die Template-Unterstützung für `bind` sowie `insert/clean`. Die Transformationen müssen idempotent und die vorhandenen ID-/Schreibsicherungen weiterhin wirksam sein.

Ein zusätzlicher allgemeiner `-ignore`-/`-suppress`-Schalter ist für M19 nicht vorgesehen.

## 12. Referenzen

- Microsoft Logging: https://learn.microsoft.com/dotnet/core/extensions/logging
- Microsoft source-generated Logging: https://learn.microsoft.com/dotnet/core/extensions/high-performance-logging
- Go `slog`: https://pkg.go.dev/log/slog
- Rust `tracing`: https://docs.rs/tracing/latest/tracing/
- LTTng Tracepoints: https://lttng.org/docs/v2.14/
