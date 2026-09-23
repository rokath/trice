**Redaktioneller Kommentar — nicht Teil des UM.** Gewichtungsrichtung, INFO als User-Default, Überschreiben per `-ulabel`, `untagged` und UND-Verknüpfung der Filter sind abgestimmt. Die Teilzeilenregel ist noch ein Vorschlag zur Abnahme. Gewichte liegen als Eigenschaft der Gruppen in `lineTransformerANSI.go`; Tabellenposition und Farbe bestimmen keine Priorität mehr. Tests und Umsetzung: [M08–M14](../README.md#abarbeitungsfolge). Das vollständige Beispiel für den heutigen Level-Filter braucht die Regression aus [M13](../issues/M13_ereignisfilter.md).

---

## <a id="log-auswahl"></a>Log-Auswahl mit Tags und Prioritäten

Mit Tags wählst du bestimmte Meldungsarten aus. Mit `-logLevel` wählst du Meldungen ab einer Mindestpriorität. Beides wirkt auf dem Host; bereits erzeugte Targetdaten werden dadurch nicht kleiner.

### Vorhanden: Tags, Farben und gezielte Auswahl

Ein bekannter Name vor dem ersten Doppelpunkt ist ein Tag:

```c
trice("wrn:Motor temperature is %d C\n", temperature);
```

Der Host erkennt `wrn`, färbt die Meldung entsprechend `./internal/emitter/lineTransformerANSI.go` und kann sie auswählen. Der Tag ist optionaler Teil des Formatstrings und damit in `til.json` (Wörterbuch); für ihn werden bei gewöhnlichen ID-basierten Trice-Meldungen keine zusätzlichen Target-Nutzdaten übertragen.

| Aufgabe | Option für `trice log` |
|---|---|
| Genau nur Warnungen und Fehler sehen | `-pick wrn:err` oder `-pick wrn -pick err`|
| Debug-Meldungen ausblenden | `-ban dbg` |
| Eigene Tags bekannt machen | `-ulabel motor` `-ulabel pressure`|

`-pick` und `-ban` schließen einander aus und sind nützlich, wenn du gezielt einzelne Meldungstypen (Gruppen) untersuchst. Sie wirken immer auf eine gesamte Gruppe.

Aliase wie `err` und `ERROR` gehören zur selben Gruppe. Welche Tag Aliase existieren, steht in der Tags Slice innerhalb von `./internal/emitter/lineTransformerANSI.go`. Mit CLI Switch `-color default` werden bekannte rein kleingeschriebene Tag-Präfixe entfernt und Farben hinzugefügt; Präfixe (Tags) mit mindestens einem Großbuchstaben bleiben sichtbar. `-color none` entfernt ebenfalls komplett kleingeschriebenen Präfixe, erzeugt aber keine Farben. `-color off` lässt Präfixe unverändert, fügt aber keine Farben hinzu. Details: [Tags und Farben](../../TriceUserManual.md#trice-tags-and-color).

Gib eigene Tags einzeln mit wiederholtem `-ulabel` an. Doppelpunktlisten für `-ulabel` sind ungültig. Neu registrierte User-Tags können unabhängig von der Optionsreihenfolge mit `-pick` und `-ban` ausgewählt werden.

### <a id="la-heute-loglevel"></a>Vorhanden: logLevel mit Einschränkungen

`-logLevel wrn` verwendet das Gewicht der Warning-Gruppe als inklusive Schwelle. Ein Tag-Alias und ein numerischer Wert zwischen `0` und `999` sind ebenfalls zulässig. Unbekannte Werte werden vor dem Öffnen der Eingabe abgewiesen. Numerische und tagbezogene Schwellen lassen Text ohne erkannten Tag derzeit passieren; `off` unterdrückt alle Ausgabeteile. Die Filterung arbeitet noch auf Ausgabeteilen und kann deshalb Zeitstempel, Quellposition, ID, Präfix und Suffix getrennt unterdrücken. M13 stellt diese Entscheidung auf das vollständige Ereignis um.

Wer die vorhandene Level-Schwelle benötigt, kann ergänzte Spalten ohne eigene Tag-Präfixe formatieren. Beispiel für eine vorhandene TREX/TCOBSv1-Aufzeichnung ohne Verschlüsselung und passende `til.json`/`li.json` im Arbeitsverzeichnis:

```sh
trice log -p FILEBUFFER -args capture.bin \
  -logLevel wrn \
  -hs UTCmicro -liFmt '[%20s:%3d] ' -showID '[id=%d] ' \
  -ts0 '[no target stamp] ' -ts16 '[t16 = %9d] ' -ts32 '[t32 = %9d] ' \
  -prefix '[' -suffix ']'
```

Das Beispiel zielt auf Meldungen mit jeweils abschließendem Zeilenumbruch; Teilzeilen können sich anders verhalten. Weitere Optionen: [Zeitstempel](../../TriceUserManual.md#trice-timestamps).

### <a id="la-gewichte"></a>Vorhanden: feste Gewichte statt Tabellenposition

Ein höheres Gewicht bedeutet höhere Priorität. Eine Meldung passiert `-logLevel N`, wenn ihr Gewicht **größer oder gleich N** ist. Ein niedrigerer Schwellenwert zeigt mehr Meldungen. `-logLevel wrn` verwendet das Gewicht der Gruppe `wrn`; `all` lässt alle Anwendungsmeldungen zu, `off` keine.

Die Anfangswerte sind Ganzzahlen von `0` bis `999`. Alle Aliase einer Gruppe haben dasselbe Gewicht.

| Gruppe | Gewicht |
|---|---:|
| Fatal | 790 |
| Critical | 780 |
| Emergency | 770 |
| Error | 760 |
| Warning | 750 |
| Attention | 740 |
| Assert, Alarm, Alert | 760 |
| Notice | 600 |
| INFO, Time, Message, Read, Write, Receive, Transmit, Diag, Interrupt, Signal, Test, Default, Config, Microseconds, Milliseconds, Seconds, Delta | 500 |
| `untagged` | 500 |
| Neue User-Tags ohne explizites Gewicht | endgültiges INFO-Gewicht |
| Debug | 200 |
| Trace | 100 |
| Verbose | 50 |

`CYCLE_ERROR` kennzeichnet eine Werkzeugdiagnose und nimmt nicht an der Auswahl von Anwendungsmeldungen teil. Die Tabelle ist eine Trice-Auswahlkonvention; sie behauptet keine allgemeingültige Schweregradordnung. Ein fachlicher Tag wie `receive` bezeichnet zunächst ein Thema. Sein Gewicht ist eine konfigurierbare Priorität.

Für den Normalbetrieb ist `-logLevel info` praktisch: Neue Gruppen mit ausreichend hohem Gewicht erscheinen automatisch. Für eine Untersuchung nur bestimmter Gruppen passt `-pick` besser.

### Vorhanden: eigene Gewichte und validierte Selektoren

```text
-ulabel motor
-ulabel sensor:150
-ulabel msg:600
```

`motor` übernimmt das INFO-Gewicht. `sensor` erhält 150. `msg:600` ändert das Gewicht der Message-Gruppe samt Aliasen für diesen Aufruf. Ein bekanntes `-ulabel msg` ohne Zahl verändert nichts. Bei mehreren expliziten Gewichten für dieselbe Gruppe gilt die letzte Angabe, auch über verschiedene Aliase.

Alle Labels werden registriert, bevor Selektoren und Schwellen aufgelöst werden. Daher sind `-pick motor -ulabel motor` und die umgekehrte Reihenfolge gleichwertig. Ein neuer Tag ohne explizites Gewicht übernimmt das endgültige INFO-Gewicht nach Verarbeitung aller Optionen.

Nach dem Doppelpunkt von `-ulabel` darf nur ein Gewicht stehen: `tagA:tagB` ist ungültig. Bei `-pick` und `-ban` bleibt der Doppelpunkt dagegen das Listentrennzeichen. Unbekannte oder leere Selektoren, ungültige Gewichte und `-pick` zusammen mit `-ban` führen vor dem Öffnen der Eingabe zu einem CLI-Fehler. Rein numerische Tag-Namen sowie `all` und `off` sind reserviert, damit die Level-Eingabe eindeutig bleibt.

### Geplant: Filter gemeinsam auf das Ereignis anwenden

Für einfache Meldungen wirken Tag-Auswahl und Schwelle bereits nacheinander. M13 fasst beide Entscheidungen für das vollständige Ereignis samt Zusatzspalten zusammen. Dann bleiben von den ausgewählten Gruppen nur Meldungen ab Schwellgewicht übrig. Für `-ban` gilt: nicht verboten **und** ausreichend wichtig. Beispiele:

`-pick err:wrn -logLevel err` zeigt nur die ERROR Gruppe, wenn WARNIG geringeres Gewicht als ERROR hat.
`-ban err:wrn -logLevel err` zeigt nur die Gruppen mit Gewicht größer ERROR an, aber auf keinen Fall die WARNING Gruppe.
`-ban all -logLevel info` wird nicht als CLI Fehler behandelt. Es werden keine Messages angezeigt.
`-pick off -logLevel info` wird nicht als CLI Fehler behandelt. Es werden keine Messages angezeigt.
`-pick all -logLevel info` wird nicht als CLI Fehler behandelt. Es werden alle Messages ab Fehlergewicht INFO angezeigt. Gleichbedeutend mit `-ban off -logLevel info` oder einfach `-logLevel info`

### <a id="la-untagged"></a>Vorhanden: untagged

Fehlt ein bekannter Tag, ordnet der Host die Anwendungsmeldung der reservierten Gruppe `untagged` zu. Sie hat das Gewicht 500 und eine neutrale Farbe. Ihr eigenes Gewicht lässt sich unabhängig von INFO ändern. Du kannst sie wie andere Gruppen auswählen:

```text
-pick untagged
-ban untagged
-ulabel untagged:150
```

`untagged` kann nicht als zusätzliche unabhängige User-Gruppe angelegt werden. Ein gleichnamiges `-ulabel` ist wie bei anderen vorhandenen Gruppen zulässig. Ein ausdrückliches `untagged:` im Formatstring wählt die Gruppe direkt und wird nicht verdoppelt.

| Ursprünglicher Text | Interne Darstellung für den Textausgabepfad |
|---|---|
| `Hello` | `untagged:Hello` |
| `untagged:Hello` | `untagged:Hello` |
| `mgs:blah` | `untagged:mgs:blah` |
| `msg:Hello` | `msg:Hello` |

Bei `default`/`none` wird nur das erkannte äußere Präfix entfernt; der Tippfehler `mgs:` bleibt sichtbar. Bei `-color off` bleibt auch das intern ergänzte `untagged:` sichtbar.

Die Zuordnung erfolgt einmal für die Anwendungsmeldung, bevor Zeitstempel, Quellposition, ID, Präfix oder Suffix ergänzt werden. Bei ID-basierten Meldungen ist der Formatstring im Wörterbuch maßgeblich; ein Laufzeitwert mit Text `err:...` ändert die Zuordnung nicht. Originalquelle, gespeicherter Formatstring, IDs und binäre Rohaufzeichnung werden durch diese Hostzuordnung nicht umgeschrieben. Die ID-Vergabe bleibt unverändert. Byteorientierte CHAR-/DUMP-Ausgabe erhält keine Ereigniszuordnung aus einzelnen Decoderblöcken.

### <a id="la-ereignisse"></a>Geplant: Meldung samt Zusatzspalten auswählen

Wenn eine Warnung angezeigt wird, bleiben ihre eingeschalteten Zeitstempel, Quellposition und ID erhalten. Ihr Darstellungstag beeinflusst die Auswahl nicht. Wird die Warnung ausgeblendet, erscheinen auch ihre Zusatzspalten nicht. Das gilt ebenso für Präfix und Suffix sowie für lokale und entfernte Anzeige.

Ein Trice-Aufruf ist ein Ereignis. Eine Zeile kann mehrere Aufrufe enthalten. **Entwurf für Teilzeilen:** Jeder Aufruf wird einzeln ausgewählt; nur angenommene Textteile bilden die Ausgabe. Wird ein Teil unterdrückt, werden die übrigen Teile direkt aneinandergefügt. Die Zusatzspalten gehören zum ersten angenommenen Ereignis der Zeile. Ein unterdrückter Zeilenumbruch beendet sie nicht; ein offener Rest wird spätestens am Eingabeende ausgegeben. `-addNL` beendet jeden angenommenen Aufruf als eigene Zeile. Bei einem mehrzeiligen Aufruf gehören alle Zeilen zur selben Filterentscheidung.

Werkzeugdiagnosen wie „unbekannte ID“, beschädigte COBS-/TCOBS-Frames, kurze Pakete und Cycle-Fehler werden bereits beim Dekodieren von Anwendungstext getrennt. Sie bleiben unabhängig von `-pick`, `-ban` und `-logLevel off` auf der lokalen Werkzeugausgabe sichtbar, erhalten keine Anwendungsmetadaten und gelangen nicht in `-vis`. Sie sind keine `untagged`-Anwendungsmeldungen. Nicht fatale Diagnosen lassen die Verarbeitung weiterlaufen; Eingabe- und Schreibfehler behalten ihre vorhandene Behandlung. Die Binäraufzeichnung liegt vor dieser Trennung und bleibt bytegleich. Eine künftige maschinenlesbare Anwendungsausgabe verwendet den getrennten Line-Composer und belässt freie Diagnosetexte beim Werkzeug-Writer. Die gemeinsame Ereignisauswahl soll auch für die Visualisierung mit `-vis` gelten.

### Vorhanden und geplant: IDs vergeben und auf dem Target routen

`insert` und `bind` bieten allgemeine ID-Grenzen und tagbezogene Bereiche:

```sh
trice bind -IDMin 1000 -IDMax 9999 -IDRange err:10,99
```

Der Bereich `10..99` ist hier für Fehler vorgesehen; der allgemeine Bereich überschneidet ihn nicht. Vergabegrenzen sind einschließlich. Der Befehl allein konfiguriert nur die ID-Vergabe, keinen Target-Ausgang.

ID-Routing wird durch Defines wie `TRICE_UARTA_MIN_ID` im Build eingerichtet. Zur Laufzeit entscheidet die ID, an welchen Ausgang die Meldung geht. Aktives Deferred-Routing benötigt den vorhandenen Single-Pack-Modus. Konfiguration in projektspezifischer `triceConfig.h`, siehe [triceDefaultConfig.h](../../../src/triceDefaultConfig.h); Hintergrund: [ID-Management](../../TriceUserManual.md#trice-id-management) und [ID-Routing](../../TriceUserManual.md#id-routing).

**Vorhandene Einschränkung:** Im Deferred-Routing werden Bereichsgrenzen derzeit ausgeschlossen; die Aktivierung bei nur einer gesetzten Grenze ist je nach Ausgang unterschiedlich. Geplant sind inklusive Grenzen und klare Konfigurationsfehler für unvollständige Bereiche.

**Geplante Neuzuweisung:** Hat eine bearbeitete Logstelle ID `250`, aber nun den Bereich `10..99`, erhalten `insert` beziehungsweise `bind` für sie eine passende ID. Die alte Zuordnung bleibt in `til.json` zur Dekodierung älterer Firmware. Ohne neue Policy-Verletzung bleibt die aktive ID stabil. Quellen außerhalb des bearbeiteten Umfangs werden nicht geändert.

Mit `-v` wird einmal die Anzahl der zur aktuellen Policy unpassenden Wörterbucheinträge samt einem Beispiel gemeldet. Ohne `-v` entfällt diese zusätzliche Meldung. Alte Einträge werden weder gelöscht noch umgedeutet. Nach Neuzuweisung muss die betroffene Firmware neu gebaut werden.

### Vorhanden: Aufzeichnen und später anders auswählen

Die binäre Rohaufzeichnung erfolgt vor den Hostfiltern:

```sh
trice log -p COM3 -binaryLogfile capture.bin -pick err
trice log -p FILEBUFFER -args capture.bin -til til.json -logLevel all
```

Mit passendem Wörterbuch zeigt der zweite Befehl auch ursprünglich ausgeblendete Meldungen. Bestehende Dateien werden beim Aufzeichnen erweitert; wähle für eine neue Sitzung einen neuen Namen und niemals die Wiedergabequelle als Aufzeichnungsziel. Siehe [binäre Logdateien](../../TriceUserManual.md#binary-logfile).

Hostfilter sparen keine Targetarbeit. Zum Herauskompilieren normaler Trice-Aufrufe dient das vorhandene [TRICE_OFF](../../TriceUserManual.md#target-side-compile-time--trice-on-off). Zusätzliche Target-Level-Makros und dynamische Target-Level-Steuerung gehören nicht zu diesem Entwurf.
