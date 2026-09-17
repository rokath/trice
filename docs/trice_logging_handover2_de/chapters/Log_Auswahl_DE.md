**Redaktioneller Kommentar — nicht Teil des UM.** Gewichtungsrichtung, INFO als User-Default, Überschreiben per `-ulabel` und UND-Verknüpfung der Filter sind abgestimmt. Konkrete Zahlen, `0..999` als Bereich, Farbe und Präfixanzeige für `untagged` sowie die Teilzeilenregel sind Vorschläge zur Abnahme. Gewichte sollen als Eigenschaft der Gruppen in `lineTransformerANSI.go` liegen; Tabellenposition und Farbe bestimmen keine Priorität mehr. Tests und Umsetzung: [M08–M14](../README.md#abarbeitungsfolge). Das vollständige Beispiel für den heutigen Level-Filter braucht die Regression aus [M13](../issues/M13_ereignisfilter.md).

---

## <a id="log-auswahl"></a>Log-Auswahl mit Tags und Prioritäten

Mit Tags wählst du bestimmte Meldungsarten aus. Mit `-logLevel` wählst du Meldungen ab einer Mindestpriorität. Beides wirkt auf dem Host; bereits erzeugte Targetdaten werden dadurch nicht kleiner.

### Vorhanden: Tags, Farben und gezielte Auswahl

Ein bekannter Name vor dem ersten Doppelpunkt ist ein Tag:

```c
trice("wrn:Motor temperature is %d C\n", temperature);
```

Der Host erkennt `wrn`, färbt die Meldung und kann sie auswählen. Der Tag steht im Formatstring des Wörterbuchs; für ihn werden bei gewöhnlichen ID-basierten Trice-Meldungen keine zusätzlichen Target-Nutzdaten übertragen.

| Aufgabe | Option für `trice log` |
|---|---|
| Genau Warnungen und Fehler sehen | `-pick wrn:err` |
| Debug-Meldungen ausblenden | `-ban dbg` |
| Mehrere Gruppen auswählen | `-pick wrn -pick err` |
| Eigenen Tag bekannt machen | `-ulabel motor` |

`-pick` und `-ban` schließen einander aus. `-pick wrn:err` enthält keine anderen Gruppen wie Fatal oder Critical. Das ist nützlich, wenn du gezielt einen Meldungstyp untersuchst.

Aliase wie `err` und `ERROR` gehören zur selben Gruppe. Bei `-color default` werden bekannte kleingeschriebene Tag-Präfixe entfernt und Farben hinzugefügt; großgeschriebene Präfixe bleiben sichtbar. `-color none` entfernt ebenfalls die kleingeschriebenen Präfixe, erzeugt aber keine Farben. `-color off` lässt Präfixe unverändert. Details: [Tags und Farben](../../TriceUserManual.md#trice-tags-and-color).

Im geprüften lokalen Arbeitsstand gehört `rx` zu Receive, `tx` zu Transmit, `s`/`S` zu Seconds und `W` zu Write; für Warnungen verwende beispielsweise `wrn`. Ältere Stände enthalten mehrdeutige Kurzformen. Ausgeschriebene Namen wie `read`, `write`, `receive`, `transmit`, `signal` und `Seconds` erleichtern den Wechsel.

**Derzeitige Einschränkungen:** Gib eigene Tags einzeln mit wiederholtem `-ulabel` an. Doppelpunktlisten für `-ulabel` funktionieren nicht wie beschrieben. Auch die Auswahl neu registrierter User-Tags durch `-pick`/`-ban` ist derzeit fehlerhaft.

### <a id="la-heute-loglevel"></a>Vorhanden: logLevel mit Einschränkungen

`-logLevel wrn` verwendet derzeit die Reihenfolge der Tag-Tabelle: Es lässt `wrn` und davor stehende Gruppen durch. Diese Reihenfolge vermischt Prioritäten mit anderen Tags. Außerdem kann die Filterung Zeitstempel, Quellposition, ID, Präfix und Suffix unterdrücken. Für die alltägliche gezielte Auswahl sind vorerst `-pick` und `-ban` leichter vorhersehbar.

Wer die vorhandene Level-Schwelle benötigt, kann ergänzte Spalten ohne eigene Tag-Präfixe formatieren. Beispiel für eine vorhandene TREX/TCOBSv1-Aufzeichnung ohne Verschlüsselung und passende `til.json`/`li.json` im Arbeitsverzeichnis:

```sh
trice log -p FILEBUFFER -args capture.bin -til til.json -li li.json \
  -encoding TREX -pf TCOBSv1 -logLevel wrn -color none \
  -hs UTCmicro -liFmt '[%s:%d] ' -showID '[id=%d] ' \
  -ts0 '[no target stamp] ' -ts16 '[t16=%d] ' -ts32 '[t32=%d] ' \
  -ts0delta '' -ts16delta '' -ts32delta '' \
  -prefix '[' -suffix ']'
```

Die Metadaten beginnen nicht mit einem bekannten Darstellungstag und werden deshalb im aktuellen Pfad nicht als niedriger priorisierte Gruppe ausgeblendet. Hostzeit bezeichnet beim Abspielen die Wiedergabezeit. Die Targetwerte werden hier ohne Einheitenumrechnung angezeigt.

Das Beispiel zielt auf Meldungen mit jeweils abschließendem Zeilenumbruch; Teilzeilen können sich anders verhalten. Weitere Optionen: [Zeitstempel](../../TriceUserManual.md#trice-timestamps).

### <a id="la-gewichte"></a>Geplant: feste Gewichte statt Tabellenposition

Ein höheres Gewicht bedeutet höhere Priorität. Eine Meldung passiert `-logLevel N`, wenn ihr Gewicht **größer oder gleich N** ist. Ein niedrigerer Schwellenwert zeigt mehr Meldungen. `-logLevel wrn` verwendet das Gewicht der Gruppe `wrn`; `all` lässt alle Anwendungsmeldungen zu, `off` keine.

**Vorschlag für Anfangswerte:** Ganzzahlen von `0` bis `999`. Alle Aliase einer Gruppe haben dasselbe Gewicht.

| Gruppe | Gewicht |
|---|---:|
| Fatal | 990 |
| Critical | 980 |
| Emergency | 970 |
| Error | 960 |
| Warning | 950 |
| Attention | 940 |
| Assert, Alarm, Alert | 960 |
| Notice | 600 |
| Info | 500 |
| Debug | 200 |
| Trace | 100 |
| Verbose | 50 |
| Time, Message, Read, Write, Receive, Transmit, Diag, Interrupt, Signal, Test, Default, Config, Microseconds, Milliseconds, Seconds, Delta | 500 |
| `untagged` | 500 (INFO-Anfangswert) |
| Neue User-Tags ohne explizites Gewicht | endgültiges INFO-Gewicht |

`CYCLE_ERROR` kennzeichnet eine Werkzeugdiagnose und nimmt nicht an der Auswahl von Anwendungsmeldungen teil. Die Tabelle ist eine Trice-Auswahlkonvention; sie behauptet keine allgemeingültige Schweregradordnung. Ein fachlicher Tag wie `receive` bezeichnet zunächst ein Thema. Sein Gewicht ist eine konfigurierbare Priorität.

Für den Normalbetrieb ist `-logLevel wrn` praktisch: Neue Gruppen mit ausreichend hohem Gewicht erscheinen automatisch. Für eine Untersuchung nur bestimmter Gruppen passt `-pick` besser.

### Geplant: eigene Gewichte und kombinierte Filter

```text
-ulabel motor
-ulabel sensor:150
-ulabel msg:600
```

`motor` übernimmt das INFO-Gewicht. `sensor` erhält 150. `msg:600` ändert das Gewicht der Message-Gruppe samt Aliasen für diesen Aufruf. Ein bekanntes `-ulabel msg` ohne Zahl verändert nichts. Bei mehreren expliziten Gewichten für dieselbe Gruppe gilt die letzte Angabe, auch über verschiedene Aliase.

Alle Labels werden registriert, bevor Selektoren und Schwellen aufgelöst werden. Daher sind `-pick motor -ulabel motor` und die umgekehrte Reihenfolge gleichwertig. Vorschlag: Ein neuer Tag ohne explizites Gewicht übernimmt das endgültige INFO-Gewicht nach Verarbeitung aller Optionen.

Nach dem Doppelpunkt von `-ulabel` darf nur ein Gewicht stehen: `tagA:tagB` bleibt ungültig. Bei `-pick` und `-ban` bleibt der Doppelpunkt dagegen das Listentrennzeichen. Unbekannte oder leere Selektoren, ungültige Gewichte und `-pick` zusammen mit `-ban` führen vor dem Öffnen der Eingabe zu einem CLI-Fehler. Vorschlag: rein numerische Tag-Namen sowie `all` und `off` sind reserviert, damit die Level-Eingabe eindeutig bleibt.

Tag-Auswahl und Schwelle wirken gemeinsam:

```text
-pick err:wrn -logLevel err
```

Damit bleiben von den ausgewählten Gruppen nur Meldungen ab Fehlergewicht übrig. Für `-ban` gilt: nicht verboten **und** ausreichend wichtig.

### <a id="la-untagged"></a>Geplant: untagged

Fehlt ein bekannter Tag, ordnet der Host die Anwendungsmeldung der reservierten Gruppe `untagged` zu. Sie erhält im Entwurf den INFO-Anfangswert 500 und eine neutrale Farbe. Ihr eigenes Gewicht lässt sich unabhängig von INFO ändern. Du kannst sie wie andere Gruppen auswählen:

```text
-pick untagged
-ban untagged
-ulabel untagged:150
```

`untagged` kann nicht als zusätzliche unabhängige User-Gruppe angelegt werden. Ein gleichnamiges `-ulabel` ist wie bei anderen vorhandenen Gruppen zulässig.

| Ursprünglicher Text | Interne Darstellung für den Textausgabepfad |
|---|---|
| `Hello` | `untagged:Hello` |
| `mgs:blah` | `untagged:mgs:blah` |
| `msg:Hello` | `msg:Hello` |

Bei `default`/`none` wird nur das erkannte äußere Präfix entfernt; der Tippfehler `mgs:` bleibt sichtbar. Bei `-color off` bleibt nach der allgemeinen Präfixregel auch das intern ergänzte `untagged:` sichtbar. Das ist eine bewusst dokumentierte Änderung der Textausgabe.

Die Zuordnung erfolgt einmal für die Meldung, bevor Zusatzspalten entstehen. Bei ID-basierten Meldungen ist der Formatstring im Wörterbuch maßgeblich; ein Laufzeitwert mit Text `err:...` darf keine neue Priorität vortäuschen. Originalquelle, gespeicherter Formatstring, IDs und binäre Rohaufzeichnung werden durch diese Hostzuordnung nicht umgeschrieben. Diese Hostzuordnung ändert die ID-Vergabe nicht.

### <a id="la-ereignisse"></a>Geplant: Meldung samt Zusatzspalten auswählen

Wenn eine Warnung angezeigt wird, bleiben ihre eingeschalteten Zeitstempel, Quellposition und ID erhalten. Ihr Darstellungstag beeinflusst die Auswahl nicht. Wird die Warnung ausgeblendet, erscheinen auch ihre Zusatzspalten nicht. Das gilt ebenso für Präfix und Suffix sowie für lokale und entfernte Anzeige.

Ein Trice-Aufruf ist ein Ereignis. Eine Zeile kann mehrere Aufrufe enthalten. **Entwurf für Teilzeilen:** Jeder Aufruf wird einzeln ausgewählt; nur angenommene Textteile bilden die Ausgabe. Wird ein Teil unterdrückt, werden die übrigen Teile direkt aneinandergefügt. Die Zusatzspalten gehören zum ersten angenommenen Ereignis der Zeile. Ein unterdrückter Zeilenumbruch beendet sie nicht; ein offener Rest wird spätestens am Eingabeende ausgegeben. `-addNL` beendet jeden angenommenen Aufruf als eigene Zeile. Bei einem mehrzeiligen Aufruf gehören alle Zeilen zur selben Filterentscheidung.

Werkzeugfehler wie „unbekannte ID“ bleiben unabhängig von Anwendungsfiltern sichtbar, auch bei `-logLevel off`. Sie sind keine `untagged`-Anwendungsmeldungen. Die gemeinsame Ereignisauswahl soll auch für die Visualisierung mit `-vis` gelten.

### Vorhanden und geplant: IDs vergeben und auf dem Target routen

`insert` und `bind` bieten allgemeine ID-Grenzen und tagbezogene Bereiche:

```sh
trice bind -src app -IDMin 1000 -IDMax 9999 -IDRange err:10,99
```

Der Bereich `10..99` ist für Fehler vorgesehen; der allgemeine Bereich überschneidet ihn nicht. Vergabegrenzen sind einschließlich. Der Befehl allein konfiguriert keinen Target-Ausgang.

ID-Routing wird durch Defines im Build eingerichtet. Zur Laufzeit entscheidet die ID, an welchen Ausgang die Meldung geht. Aktives Deferred-Routing benötigt den vorhandenen Single-Pack-Modus. Konfiguration: [triceDefaultConfig.h](../../../src/triceDefaultConfig.h); Hintergrund: [ID-Management](../../TriceUserManual.md#trice-id-management) und [ID-Routing](../../TriceUserManual.md#id-routing).

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
