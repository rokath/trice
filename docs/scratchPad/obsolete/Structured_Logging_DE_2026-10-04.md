## 32. <a id="strukturiertes-logging"></a>Strukturiertes Logging

Maschinenlesbare Ausgabe setzt das standardmäßige TREX-Drahtformat voraus. CHAR und DUMP haben keine passenden Ereignisgrenzen und werden mit `-logFormat json` oder `-logFormat kv` abgewiesen.

Strukturiertes Logging ergänzt eine lesbare Meldung um benannte, typisierte Werte. Ein normaler Trice-Aufruf genügt:

```c
trice("info:Motor {motor_id}: {temperature_c:%.1f C}", motor_id, aFloat(temperature_c));
```

Bei `motor_id = 3` und `temperature_c = 87.5` entstehen daraus je nach Ausgabeformat:

Für diese drei Ausgaben sind `-li off -showID '' -hs off -ts off` gesetzt. Dadurch erscheinen hier keine optionalen Host- oder Target-Metadaten.

`tlog -logFormat text` (default):

```text
Motor 3: 87.5 C
```

`tlog -logFormat json` (NDJSON):

```json
{"tag":"INFO","level":"INFO","message":"Motor 3: 87.5 C","fields":{"motor_id":3,"temperature_c":87.5}}
```

`tlog -logFormat kv`:

```text
tag=INFO level=INFO message="Motor 3: 87.5 C" field.motor_id=3 field.temperature_c=87.5
```

Weitere Ausgabeformate, etwa CSV, sind bei Bedarf nachrüstbar.

Das Target überträgt weiterhin ID und Werte im bestehenden Drahtformat. Feldnamen werden weder als zusätzliche Runtime-Argumente noch als zusätzliche Nutzdaten übertragen; sie stehen im Wörterbuch auf dem Host.

Unterstützt werden skalare Trices mit 8, 16, 32 oder 64 Bit sowie Strings über `triceS` und `triceN`. Die Target-Makros und ihre Bitbreitenregeln bleiben maßgeblich. Context Enrichment (`bind -ce`) kann die unterstützten strukturierten Felder ergänzen; die Details stehen in [Kapitel 33](#trice-context-enrichment).

Zum Ausprobieren zeigen der [PC Feature Tour](../examples/PC_features/README.md) und der [G0B1 Feature Tour](../examples/G0B1_features/ReadMe.md) jeweils benannte Zahlen- und Stringfelder sowie Text-, NDJSON- und KV-Ausgabe. Der PC-Durchlauf benötigt keine Hardware und liefert sofort eine kurze Binäraufzeichnung.

Benannte Felder sind für Pufferformate wie `triceB` derzeit keine Option. Dort wird ein printf-Platzhalter für jedes Pufferelement wiederholt; ein strukturiertes Feld beschreibt dagegen einen einzelnen benannten Wert. Ob ein benannter Puffer als Zahlenliste, Bytefolge oder Text erscheinen sollte, ist im aktuellen Feldschema nicht festgelegt. `bind` und `insert` weisen deshalb `trice8B("msg:{bytes:%02x}", bytes, 2)` mit einem Fehler ab. Auch `triceF` unterstützt keine benannten Felder.

Ein Pufferlog ohne benanntes Feld bleibt möglich: `trice8B("msg:%02x ", bytes, 2)` ergibt für `0x01` und `0x02` in JSON `{"tag":"MESSAGE","message":"01 02 "}`. Ein `fields`-Objekt entsteht dabei nicht. Für eine feste Anzahl einzelner benannter Werte können stattdessen skalare Trices verwendet werden; eine strukturierte Ausgabe ganzer Puffer erfordert eine eigene Format- und Schemafestlegung.

### 32.1. <a id="platzhalter-und-namen"></a>Platzhalter und Namen

| Schreibweise im Formatstring | Bedeutung                                     |
|------------------------------|-----------------------------------------------|
| `{motor_id}`                 | Expliziter Feldname, Standarddarstellung.     |
| `{}`                         | Name aus dem zugehörigen C-Argument ableiten. |
| `{plant.}`                   | Abgeleiteten Namen mit `plant.` ergänzen.     |
| `{:%.1f}`                    | Abgeleiteter Name und explizite Darstellung.  |
| `{temperature:%.1f C}`       | Expliziter Name und Darstellung.              |
| `{: = %.1f C}`               | Abgeleiteter Name und Darstellung.            |
| `{motor_id:: %d, }`          | Name `motor_id`, Darstellung `: %d, `.        |
| `{{` und `}}`                | Literale öffnende und schließende Klammer.    |

Der erste Doppelpunkt trennt Name und Darstellung. Der Darstellungsteil darf freien Text und weitere Doppelpunkte enthalten, muss aber genau einen unterstützten Formatspezifizierer enthalten. `%%` ist kein zusätzlicher Wert. Dynamische Breiten wie `%*d` sind in einem strukturierten Feld nicht erlaubt.

Ohne Darstellung wird `%d` verwendet. Ist das vollständige Argument in `aFloat(...)` oder `aDouble(...)` eingeschlossen, ist der Default `%f`. Es gibt keine allgemeine C-Typinferenz: Ein unsigned Wert benötigt beispielsweise `{counter:%u}`, eine Adresse `{address:%p}` und ein String `{text:%s}`.

```c
trice("info:{} {}", motor_id, aFloat(temperature_c));
trice("info:{plant.}", motor->temperature);
trice("info:{temperature:%.2f}", aFloat(read_temperature()));
triceS("info:{message:%s}", "motor ready");
triceN("info:{message:%s}", buffer, length);
```

Namen lassen sich aus einzelnen Bezeichnern und reinen Memberketten ableiten. `motor.temperature` und `motor->temperature` ergeben beide `motor.temperature`. Die Wrapper `aFloat` und `aDouble` werden für die Namensableitung entfernt. Ausdrücke wie `values[0]`, `a+b` oder `read_temperature()` brauchen einen expliziten Namen. Namen bestehen aus Bezeichnern mit optionalen Punktsegmenten; Leerzeichen innerhalb eines Bezeichners, leere Segmente und führende Ziffern sind ungültig.

Kanonische Namen müssen innerhalb eines Aufrufs eindeutig sein. `{motor->id}` und `{motor.id}` kollidieren. `motor` und `motor.id` sind dagegen zwei verschiedene, flache Schlüssel; Punktnamen erzeugen keine verschachtelten JSON-Objekte.

Gewöhnliche `%...`-Konvertierungen und strukturierte Platzhalter verbrauchen Argumente gemeinsam von links nach rechts:

```c
trice("info:%u {temperature:%.1f} %x {last}", sequence, aFloat(temperature), flags, last);
```

Nur `temperature` und `last` werden hier als Felder exportiert. `sequence` und `flags` erscheinen ausschließlich in der Meldung. Fehlende oder zusätzliche Argumente sowie doppelte oder fehlerhafte Felder werden beim Instrumentieren geprüft. Bei Schemafehlern werden keine Teiländerungen des Instrumentierungslaufs veröffentlicht.

Für literale Klammern gilt die neue Schreibweise auch im Textmodus:

```c
trice("info:set={{1,2}}, value={value}", value);
```

Das ergibt `set={1,2}, value=7` bei `value = 7`. Ein Backslash vor einer Klammer ersetzt das Verdoppeln nicht.

### 32.2. <a id="feldtypen-und-darstellung"></a>Feldtypen und Darstellung

| Formatspezifizierer | Strukturierter Wert |
|---|---|
| `%d`, `%i` | Vorzeichenbehafteter Integer der Trice-Bitbreite. |
| `%u`, `%o`, `%O`, `%x`, `%X`, `%b` | Vorzeichenloser Integer. Die gewählte Zahlenbasis betrifft nur die Meldung. |
| `%f`, `%F`, `%e`, `%E`, `%g`, `%G` | Gleitkommazahl; 32 Bit mit `aFloat`, 64 Bit mit `aDouble`. |
| `%c`, `%q`, `%U` | Zeichen als String; ungültige Unicode-Codepoints werden durch das Ersatzzeichen ersetzt. |
| `%t` | Boolean: null ist `false`, andere Werte sind `true`. |
| `%s` | String aus `triceS` oder `triceN`. |
| `%p` | Adresse in der Form `0x...`; kein Rückschluss auf den Typ des referenzierten Objekts. |

`%%` ist ein literales Prozentzeichen und erzeugt weder ein Feld noch ein zusätzliches Argument.

Unterstützte C-Längenmodifikatoren wie `%lu` oder `%llX` ändern diese Semantik nicht; die Trice-Familie bestimmt die transportierte Bitbreite. Präzision, Feldbreite und Darstellungstext beeinflussen ausschließlich `message`. Beispielsweise erzeugt `{value:%.1f}` bei einem Wert von `1.25` die Textdarstellung `1.2`, während das Feld `1.25` enthält. Ebenso kürzt `{text:%.3s}` nur die Meldung, nicht den exportierten String.

Auch `-unsigned=false` ändert die Bedeutung strukturierter unsigned Felder nicht. Es steuert weiterhin die entsprechende klassische Textdarstellung.

Benannte Stringfelder verwenden `%s`; alternative klassische Stringdarstellungen wie `%x` bleiben Meldungstext ohne benanntes Feld. Ein strukturiertes Floatfeld einer 64-Bit-Trice benötigt `aDouble(...)`; `aFloat(...)` wird dort abgewiesen, damit die übertragenen Bits nicht als Double fehlinterpretiert werden.

### 32.3. <a id="instrumentierung-und-wörterbuch"></a>Instrumentierung und Wörterbuch

Sowohl `bind` als auch `insert` unterstützen strukturierte Templates. `clean` entfernt wie bisher eingefügte IDs und erhält die ursprüngliche Schreibweise des Templates. Die Kurzformen in den Quelltexten werden nicht durch kanonische Feldnamen ersetzt.

```sh
trice bind -src app -genDir generated -til til.json -li li.json
```

Alternativ für den Insert/Clean-Workflow:

```sh
trice insert -src app -genDir generated -til til.json -li li.json
trice clean -src app -til til.json -li li.json
```

Diese Beispiele setzen wie die vorhandenen Workflows ein initialisiertes TIL voraus. Die übliche Build-Einbindung der Bind-Sidecars bleibt erforderlich.

Die [C-Testbeispiele](../_test/testdata/triceCheck.c) zeigen zusätzlich 8-, 16-, 32- und 64-Bit-Werte, verschiedene Stempel, `triceS`/`triceN`, gemischte Platzhalter und die Wrapper `aFloat()`/`aDouble()`. Die zugehörigen Integrationstests prüfen ihre Textausgabe nach `bind` und `insert` mit denselben CLI-Einstellungen wie die PC-Target-Tests. Namen aus `motor.state` und `motorPtr->rpm` erscheinen im Feldregister kanonisch als `motor.state` und `motorPtr.rpm`.

In `til.json` bleiben die einzigen Schemafelder `Type` und `Strg`. `Strg` enthält alle zur Dekodierung nötigen Informationen, einschließlich abgeleiteter Namen und Float-Defaults. Zum Beispiel wird

```c
trice("info:Motor {}: {} C", motor_id, aFloat(temperature_c));
```

mit einem kanonischen `Strg` wie diesem gespeichert:

```json
{"Type":"trice","Strg":"info:Motor {motor_id}: {temperature_c:%f} C"}
```

Die konkrete Schreibweise von `Type` folgt weiterhin dem jeweiligen Instrumentierungspfad. Die Schemaidentität bleibt `Type + Strg`. Eine Feldumbenennung ergibt eine neue Schemaidentität und damit eine andere ID; historische TIL-Einträge bleiben für alte Firmware erhalten. Äquivalente Namensschreibweisen wie `motor->temperature` und `motor.temperature` behalten dieselbe kanonische Identität. Wiederholte unveränderte Läufe erzeugen keine zusätzlichen Schemas. Generierte lokale C-Formatdaten enthalten den daraus abgeleiteten printf-Formatstring.

### 32.4. <a id="ausgabeformate-und-ereignisgrenzen"></a>Ausgabeformate und Ereignisgrenzen

`trice log` und `tlog` unterstützen `-logFormat text`, `-logFormat json` und `-logFormat kv` sowie `-logFormat key-value` als Alias für `kv`. Die Werte sind unabhängig von Groß- und Kleinschreibung; Standard bleibt `text`. Der textbasierte Remote-Display-Modus und Testtabellenausgabe sind mit maschinenlesbaren Formaten nicht kombinierbar.

```sh
trice log -p FILEBUFFER -args capture.bin -pf TCOBSv1 -til til.json -li off -hs off -ts off -logFormat json
```

Framing und Wörterbuch müssen zur Aufzeichnung passen. Mit `-logFormat kv` wird dieselbe Aufnahme als Schlüssel/Wert-Ausgabe gelesen; mit `text` bleibt die bisherige Textausgabe verfügbar.

`-logFormat json` erzeugt NDJSON (JSON Lines): ein JSON-Objekt und ein abschließendes LF pro akzeptiertem Trice-Ereignis. Es gibt keinen separaten CLI-Wert `ndjson`. KV erzeugt ebenfalls genau eine Zeile pro Ereignis. Mehrere Teilaufrufe werden nicht zusammengefügt, mehrzeilige Meldungen nicht in mehrere Ereignisse aufgeteilt. Auch eine leere Meldung ist ein Ereignis. Newlines innerhalb der Meldung werden escaped.

Textpräfix, Suffix, Farben, Einrückung, Zeitdifferenzspalten und `-addNL` dekorieren keine JSON/KV-Records. Die tatsächlichen Metadaten werden stattdessen durch eigene Felder dargestellt. Diagnosen und Statusmeldungen gehen bei JSON/KV nach stderr; stdout, `-logfile` und TCP-Logausgabe enthalten die Anwendungsrecords. Fehler beim Schreiben werden an den Aufrufer zurückgegeben.

`-pick`, `-ban` und `-logLevel` wählen weiterhin ganze Anwendungsereignisse aus. Statistiken zählen erfolgreich dekodierte Ereignisse vor dieser Auswahl. Auch die Visualisierung liegt nach der Auswahl; ihre bestehenden Einschränkungen, etwa auf unterstützte numerische Einzeilenmeldungen, bleiben bestehen. `log=drop` unterdrückt nach erfolgreicher Visualisierung den gesamten normalen Record. Eine Binäraufzeichnung bleibt ungefiltert und wird durch das gewählte Ausgabeformat nicht verändert.

### 32.5. <a id="json--und-kv-vertrag"></a>JSON- und KV-Vertrag

Jeder Record enthält `tag` und `message`. `tag` ist der kanonische Name eines registrierten Formatstring-Tags; die Alias-Suche für dieses Metadatenfeld ist unabhängig von Groß- und Kleinschreibung. Beispielsweise liefern `inf:Hi` und `Inf:Hi` beide `tag=INFO`. Fehlt ein passender registrierter Tag, lautet der Wert `untagged`.

`message` übernimmt den Meldungsinhalt der Textausgabe ohne ANSI-Farbe und ohne äußere Metadaten, Textpräfixe oder Suffixe. Bei `-color none` oder `default` wird nur ein exakt registrierter, vollständig kleingeschriebener Formatstring-Tag entfernt: `inf:Hi` ergibt `Hi`, `Inf:Hi` bleibt `Inf:Hi`. Auch ein unbekannter Präfix wie `mgs:Hi` bleibt sichtbar. Bei `-color off` bleiben ausdrücklich geschriebene Tag-Präfixe wie im Textmodus stehen. Trice ergänzt niemals selbst `untagged:` zum Meldungstext: `trice("Hi")` liefert `message="Hi"` und `tag="untagged"`; `trice("mgs:Hi")` liefert `message="mgs:Hi"` und `tag="untagged"`. Führende und folgende Leerzeichen sowie leere und nur aus Leerzeichen bestehende Meldungen bleiben erhalten.

Ein Laufzeitstring ändert die Tag-Zuordnung nicht: Bei `triceS("{text:%s}", value)` mit einem Wert, der mit `err:` beginnt, bleibt `tag=untagged`. Die bisherige Textausgabe wandelt Zeichenfolgen wie `\n` und `\t` auch innerhalb von Laufzeitstrings für die Anzeige um; `message` folgt dieser Darstellung. Das benannte Feld `fields.text` enthält weiterhin den übertragenen String, abgesehen von äußerem Leerraum.

Ein optionales `level` wird über eine feste, von Farben und Gewichten unabhängige Alias-Tabelle bestimmt. Der Vergleich ist case-neutral. Beispielsweise führen `err`, `ERR` und `Error` zu `ERROR`; `warn`, `wrn` und `Warning` zu `WARNING`. Unterstützte kanonische Werte sind `FATAL`, `CRITICAL`, `EMERGENCY`, `ERROR`, `WARNING`, `ATTENTION`, `INFO`, `DEBUG`, `TRACE`, `NOTICE`, `ALERT`, `ASSERT`, `ALARM` und `VERBOSE`. Ein reiner Ausgabe-Tag wie `msg` oder ein frei definiertes User-Label erhält kein erfundenes Level. Die Alias-Suche für `tag` ändert das vorhandene Tag-Register und dessen Filterverhalten nicht; auch die Level-Zuordnung bleibt davon unabhängig.

In JSON liegen User-Felder unter `fields`. Hostfelder und User-Felder können sich daher nicht überschreiben: Ein User-Feld `tag` erscheint unter `fields.tag`. User-Felder werden in ihrer Reihenfolge im Template ausgegeben. Ein Record ohne exportierbare User-Felder enthält kein leeres `fields`-Objekt.

Integer bleiben JSON-Zahlen, einschließlich der vollständigen 64-Bit-Grenzwerte. Lesende Programme müssen einen ausreichend genauen Zahlentyp verwenden; eine Umwandlung aller Zahlen in IEEE-754-Double kann große Integer runden. Adressen sind JSON-Strings wie `"0x20001234"`. Nicht-endliche Gleitkommawerte (`NaN`, `+Inf`, `-Inf`) werden als JSON-Felder weggelassen; ihre Textdarstellung bleibt in `message`. Andere Felder desselben Ereignisses bleiben erhalten.

In KV heißen User-Felder `field.<name>` und folgen ebenfalls der Template-Reihenfolge. Zahlen, Boolean und Adressen sind unquoted; String-, Zeichen- und Meldungswerte stehen immer in doppelten Anführungszeichen. Quotes, Backslashes, LF, CR und Tab werden escaped. Nicht-endliche Floats erscheinen in KV als `NaN`, `+Inf` oder `-Inf`.

Äußerer Leerraum wird bei anderen Stringwerten wie `hs`, `file` und benannten Stringfeldern entfernt. Ein nur aus Leerraum bestehendes benanntes Feld wird als leerer String ausgegeben. Für `message` gilt diese Kürzung nicht.

```text
tag=INFO level=INFO message="Motor \"A\"\nready" field.message="Motor \"A\"\nready" field.address=0x20001234
```

### 32.6. <a id="optionale-metadaten"></a>Optionale Metadaten

| Feld | Voraussetzung und Inhalt |
|---|---|
| `id` | ID-basiertes Ereignis und aktiviertes `-showID`; numerische Trice-ID ohne Textpadding. |
| `file`, `line` | Vorhandener LI-Eintrag, aktiviertes `-li` und `-liFmt`; jeweils nur vorhandene Dateiangabe bzw. von null verschiedene Zeilennummer. |
| `ts16`, `ts32` | Vorhandener 16- bzw. 32-Bit-Target-Stempel und aktivierte Ausgabe durch `-ts` oder die passende `-ts16`/`-ts32`-Option. Jeder Wert ist ein String in der eigenen CLI-Darstellung; auch ein vorhandener Wert null wird ausgegeben. |
| `ts16Delta`, `ts32Delta` | Aktivierte `-ts16delta`- bzw. `-ts32delta`-Option und ein vorheriger Stempel derselben Bitbreite. Beim ersten Stempel fehlt das Delta-Feld vollständig. Die Darstellung folgt der jeweiligen Delta-Option. |
| `hs` | Aktiviertes `-hs`; formatierter Host-Zeitstring ohne angehängtes Spaltenpadding. `-hs off` oder `none` lässt ihn weg. |

Die Target-Stempelwerte enthalten keinen vorangestellten Stempel-Tag wie `time:` oder `dt:`, behalten aber konfigurierten Zusatztext und Einheiten. Äußerer Leerraum entfällt. Die vier Arten bleiben getrennt: Beispielsweise kann `ts16` eine Temperatur und `ts32` eine Zeit darstellen. `-ts0` und `-ts0delta` sind reine Textplatzhalter und erzeugen keine Metadatenfelder.

Beispielsweise ergeben `-ts off -ts16 'temp:%d C' -ts16delta 'step:%d C'` für zwei aufeinanderfolgende 16-Bit-Stempel mit den Werten 8 und 11 zuerst `"ts16":"8 C"` und danach `"ts16":"11 C","ts16Delta":"3 C"`. Bei `-logFormat kv` werden diese Werte als `ts16="11 C" ts16Delta="3 C"` ausgegeben.

Die feste Reihenfolge lautet `tag`, optional `level`, `message`, danach vorhandene `id`, `file`, `line`, Target-Stempel und Delta, `hs`, anschließend die User-Felder. Fehlende Metadaten werden weggelassen und nicht durch null oder Ersatzwerte simuliert. Formatierte ID-lose `typeX0`-Ereignisse besitzen keine ID, TIL-Felder oder Target-Zeitstempel; sie erhalten dennoch `tag`, `message` und gegebenenfalls `level` und `hs`.

### 32.7. <a id="feldregister"></a>Feldregister

Ein erfolgreicher `bind`- oder `insert`-Lauf erzeugt `trice-fields.txt` als Register der Feldnamen und ihrer Häufigkeiten im letzten erfolgreichen Lauf. `-genDir` wählt für beide Befehle das Verzeichnis; der Default ist `./generated` relativ zum Aufrufverzeichnis. Bei `bind` liegen dort auch die Sidecar-Header. `-buildDir` und `-bindDir` werden abgewiesen.

```text
       1 motor_id
       1 temperature_c
       4 motor.state
```

Gezählt werden die instrumentierten Stellen mit diesem User-Feld im aktuellen Aufruf. Die Datei wird vollständig neu erzeugt, nicht um historische TIL-Felder ergänzt. Ein Lauf über einen Teil der Quellen beschreibt nur diesen Teil; deshalb sollten projektweite Prüfungen alle relevanten Quellen einschließen. Cache-Treffer werden mitgezählt. Hostmetadaten erscheinen nicht, ein tatsächlich vom Benutzer benanntes Feld `tag` dagegen schon.

Die Sortierung ist zuerst nach Anzahl aufsteigend, bei gleicher Anzahl alphabetisch nach Feldname. Das Zeilenformat ist `%8d %s\n`; Feldnamen haben keine künstliche Längenbegrenzung. Ein erfolgreicher Lauf ohne User-Felder erzeugt eine leere Datei. `-dry-run` veröffentlicht keine neue Datei und erhält ein vorhandenes Register (`trice-fields.txt`).

<p align="right">(<a href="#top">back to top</a>)</p>
