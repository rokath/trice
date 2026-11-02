# Alias Tags und Context Enrichment für v2.0.0

Die nachfolgende Bedienung und alle Suchwort- und Ausschlusslisten einschließlich `trial` sind akzeptiert. Die CLI- und Tag-Grundlagen aus #768 sind auf ausdrücklichen Auftrag umgesetzt; die Alias- und CE-Integration aus #761 und #769 benötigt jeweils einen eigenen Auftrag. Der seltene CE-Fallback-Sonderfall wird mit Warnung und Workarounds akzeptiert, fester CE-Text für `-salias` wird zurückgestellt, und eigene Tags werden wie Built-ins ohne Beachtung der Groß-/Kleinschreibung behandelt. Die begrenzte Integrationsprüfung zu Issue #767 ist abgeschlossen; ihre Ergebnisse und die zugeordneten Folgearbeiten stehen unten.

Stand: 2. November 2026. Die Issues #761, #767, #768 und #769 bilden die englische Entwicklungsdokumentation im Meilenstein „Version 2.0.0“. Dieses deutsche Papier verbindet die Reviewgrundlage mit den vorgesehenen RM-Erklärungen. Es unterscheidet den umgesetzten Stand von #768 vom verbleibenden Zielverhalten.

## Entwicklungsaufgaben und Reihenfolge

| Reihenfolge | Issue | Ergebnis |
|---|---|---|
| Abgeschlossen | #767 — Review alias integration and CE fallback warnings before implementation | Warnungsfälle und Aufrufer geprüft; kanonische Tag-Ausgabe und Vis-Auswahl entschieden; Sollfälle den Folgeissues zugeordnet. |
| Umgesetzt | #768 — Unify CLI option and tag name case rules and replace -ulabel with -utag | Optionsnamen-Regeln, kommandospezifisches `-utag` und gemeinsame schreibungsunabhängige Tag-Identität einschließlich kanonischer Großschreibung implementiert. |
| Danach | #761 — Preserve alias identity and resolve tags consistently across build and logging | Aliasregistrierung, TIL-Identität, gemeinsame Tag-Auflösung und Host-Verarbeitung umsetzen. |
| Abschließend | #769 — Integrate alias tags with bounded context enrichment | Skalare Alias-CE und festen Text für natives `triceS` integrieren, Verarbeitungspfade prüfen und RM, Beispiele und CLI-Hilfe vervollständigen. |

Diese Staffelung trennt überschaubare Änderungen. Veröffentlicht wird eine gemeinsame Alias-Semantik für v2.0.0. Zwischenstände mit unterschiedlichen öffentlichen Tag-Regeln sind kein eigenes Releaseziel. Die Vorprüfung benötigt weder einen Prototyp noch eine neue Speicher-Machbarkeitsstudie. Neu entdeckte wesentliche Verhaltensfragen werden vor der betreffenden Implementierung ausdrücklich geklärt.

Die Vorprüfung und die Umsetzung von #768 wurden jeweils ausdrücklich beauftragt. #761 und #769 sind noch nicht implementiert.

## Implementierungsstand von Issue #768

Die gemeinsame CLI-Verarbeitung normalisiert nur echte Optionsnamen. Einbuchstabige Namen, Unterkommandos und verbrauchte Optionswerte bleiben unverändert. Mehrbuchstabige Namen einschließlich `-SA`, explizit gesetzte Stempel-/Filteroptionen und die optionalen `generate -logC`-Pfadformen sind berücksichtigt. Verschiedene Einstellungen mit kollidierenden Optionsnamen werden abgewiesen.

`-utag` ersetzt `-ulabel` einschließlich einer Fehlermeldung für alte Schreibweisen. Bind, Insert und Clean akzeptieren ausschließlich Namen und weisen Gewichte/Farben vor Änderungen ab. Die Tag-Registry führt bereits veröffentlichte und noch ungewichtete Case-Varianten zusammen, verwendet SENSOR/X als kanonische Custom-Metadaten und erhält die Präfixschreibweise für die Darstellung. CLI-Hilfe, aktive RM-Beispiele und aufrufende Beispiel-/Releaseprüfungen sind angepasst.

Die Go-Tests für `internal/args`, `internal/emitter` und `internal/translator` bestehen. Die betroffenen ID-Bereichs-/CE-Tests und die Skript-Fixtures für LabPlot und Release-Featureausgaben bestehen ebenfalls. ShellCheck, Shellformat, JavaScript-Syntax und Dokumentlinks sind geprüft. Ein Gesamtlauf, reale Firmware-Builds und native Linux-/Windows-Abnahmen wurden nicht ausgeführt. Alias-Registrierung mit `Name:Tag`, gespeicherte Aliasidentität, die geänderte Vis-Auswahl und CE-Fallback-Warnungen verbleiben bei #761/#769.

## Aliasarten und Registrierung

Ein Projekt kann seine eigenen Logging-Makros beibehalten:

```c
MyWrn("err:code=%u", code);
LegacyPrint("value=%d", value);
```

| Registrierung | Bedeutung |
|---|---|
| `-alias Name`, kurz `-a Name` | Skalarer Alias mit dem Grundtyp `trice`. |
| `-salias Name`, kurz `-sa Name` | String-Alias mit dem Grundtyp `triceS`. |
| `-alias Name:Tag`, `-salias Name:Tag` | Explizite Tag-Zuordnung; Kurzformen akzeptieren dieselbe Syntax. |

Das Projektmakro muss tatsächlich zur registrierten Übertragungsart passen. Der Aliasname bestimmt weder Bitbreite noch Argumentzahl oder Grundtyp. Die normale Abstimmung der Default-Bitbreite zwischen Projekt und Host bleibt erforderlich.

Aliasnamen sind exakte C-Identifier nach `[A-Za-z_][A-Za-z0-9_]*`. Ihre Schreibweise bleibt gespeichert. Namen, die unabhängig von Groß-/Kleinschreibung mit `trice` beginnen, sind für native Typen reserviert. `TriceWarning` ist deshalb kein zulässiger Alias; `MyTriceWarning` ist zulässig.

| Mehrere Angaben für denselben exakten Namen | Verhalten |
|---|---|
| Gleiche Aliasart und gleiche kanonische Zuordnung | Zulässig, einmal wirksam. |
| `Name` und `Name:Tag`, unabhängig von der Reihenfolge | Zusammenführen; die explizite Zuordnung gilt. |
| `Name:err` und `Name:ERROR` | Gleichwertig und zulässig. |
| Verschiedene explizite Tags | Fehler vor Änderungen beziehungsweise vor dem Öffnen des Log-Eingangs. |
| Derselbe Name unter `-alias` und `-salias` | Immer Fehler; die bisherige Entscheidung anhand von `s`/`S` am Namensende entfällt. |

Ein gültiger ungenutzter Alias bleibt ohne normale Warnung und erzeugt allein durch die Registrierung keinen TIL-Eintrag. `-verbose` zeigt ihn an. Ungültige oder widersprüchliche Definitionen sind auch ungenutzt Fehler.

`Name:Tag` registriert einen gültigen neuen Custom-Tag im jeweiligen Kommando automatisch. Beispielsweise benötigt `-alias SensorAlarm:sensor` kein zusätzliches `-utag sensor`. Built-in- und Custom-Tags werden ohne Beachtung der Groß-/Kleinschreibung aufgelöst: `sensor`, `Sensor` und `SENSOR` bezeichnen dieselbe Gruppe. Das gilt für Registrierung, Overrides, Format-Fallback, CE und Hostauswahl. Die C-Identifier der Aliase bleiben dagegen exakt. Bestehende Default-Gewichte sowie Host-Overrides für Gewicht und Farbe gelten weiter; je Eigenschaft gewinnt die letzte Zuweisung.

Die Registrierung wird in den passenden Quellkommandos einschließlich Bind, Insert, Clean und Generate mit Quellscan verwendet. Für explizite Host-Overrides akzeptieren auch `log` und `tlog` die Aliasoptionen.

Eigene Tag-Gruppen erhalten großgeschriebene kanonische Namen in Metadaten: `sensor`, `Sensor` und `SENSOR` werden zu SENSOR, `x` und `X` zu X. Bestehende kanonische Built-in-Namen, insbesondere `untagged`, bleiben erhalten. Die Schreibweise von Alias-Identifiern und geschriebenen Meldungspräfixen wird dadurch nicht verändert.

## Tag Zuordnung

Für registrierte Alias-Ereignisse wird genau ein Tag bestimmt:

| Vorrang | Quelle |
|---|---|
| Zuerst | Explizites `Name:Tag` im aktuellen Kommando. |
| Danach | Längster nicht ausgeschlossener Suchworttreffer im Aliasnamen. |
| Danach | Gültiger Tag vor dem ersten Doppelpunkt des ursprünglichen statischen Formatliterals. |
| Zuletzt | `untagged`. |

Ein explizites `Name:untagged` beendet die Suche. Es wird anschließend weder eine Namensheuristik noch der Format-Fallback angewendet.

Der Fallback durchsucht keine Präfixkette: `printi("err:failed")` wird ERROR; das ursprüngliche `printi("temperature:err:failed")` wird ohne Registrierung von `temperature` als Tag `untagged`. Ein freier CE-Selektor kann trotzdem `temperature` heißen.

Bei String-Aliasen wird nur ein statisch erkennbares Formatliteral als erstes semantisches Benutzerargument verwendet. Bei `MyAssert(condition, "err:failed")` wird nicht nach einem späteren Format gesucht. Empfangene Laufzeitstrings sind keine Tag-Quelle. Für Custom-Tags im Format-Fallback müssen Build und Host die benötigten Namen kennen; sie werden keine neuen Heuristik-Suchwörter.

Der Build verwendet das ursprüngliche statische Format, der Host das im gespeicherten Eintrag verfügbare statische Format. Ändert CE den Format-Fallback, dürfen die Tags in diesem dokumentierten Sonderfall voneinander abweichen. Bind/Insert warnen sichtbar und verarbeiten ansonsten gültige Eingaben weiter. Clean mit passenden CE-Einstellungen prüft die entsprechende Transformation ebenfalls. Die konkreten Fälle und Workarounds stehen unten bei der begrenzten Integrationsprüfung; zusätzliche Speicherung ist dafür nicht vorgesehen.

### Positive Suchwörter

Für den Vergleich wird der Aliasname kleingeschrieben. Gesucht werden zusammenhängende Teilwörter, ohne Pflicht zu Wortgrenzen oder CamelCase-Zerlegung.

| Gruppe und Vorrang bei Gleichstand | Suchwörter |
|---|---|
| FATAL | `fatal` |
| CRITICAL | `critical`, `crit` |
| ERROR | `error`, `err` |
| WARNING | `warning`, `warn`, `wrn` |
| INFO | `informal`, `info`, `inf` |
| DEBUG | `debug`, `dbg`, `deb`, `db` |
| TRACE | `trace`, `tr` |
| VERBOSE | `verbose` |

Alle Vorkommen werden einzeln betrachtet. Nach den Ausschlüssen gewinnt das längste verbliebene Suchwort. Bei gleicher Länge entscheidet die Tabellenreihenfolge. `Log_Error_Warning` wird WARNING; `LogErrWrn` wird ERROR. Farben, Gewichte, allgemeine Registry-Erweiterungen und Optionsreihenfolge ändern dieses Ergebnis nicht.

### Akzeptierte Ausschlussliste

Die Ausschlussbegriffe werden ebenfalls ohne Beachtung der Großschreibung als zusammenhängende Teilwörter gesucht. Ein positives Vorkommen entfällt genau dann, wenn sein gesamter Trefferbereich innerhalb eines gefundenen Ausschlussbegriffs liegt.

Die Gruppierung der Tabelle dient nur der Übersicht. Ein Ausschluss unterdrückt sämtliche vollständig darin liegenden Suchworttreffer: `infrastructure` unterdrückt sowohl sein `inf` als auch sein `tr`. Eigenständige Treffer außerhalb bleiben wirksam. Teilweise überlappende längere Treffer werden nicht entfernt: `critical` bleibt trotz des kürzeren Ausschlussworts `critic` ein CRITICAL-Treffer. Es gibt keine automatische Ableitung zusätzlicher Wortformen; Unterstrichvarianten sind ausdrücklich aufgeführt.

| Gruppe zur Orientierung | Ausschlussbegriffe |
|---|---|
| FATAL | `nonfatal`, `non_fatal` |
| CRITICAL | `criterion`, `criteria`, `critic`, `critique`, `noncritical`, `non_critical`, `criticalsection`, `critical_section`, `criticalregion`, `critical_region`, `criticalpath`, `critical_path` |
| ERROR | `interrupt`, `override`, `overridden`, `overriding`, `ferrite`, `ferromagnet`, `ferric`, `ferrous`, `ferrule`, `terrain`, `territory`, `terrestrial`, `berry`, `cherry` |
| WARNING | `nowarn`, `no_warn`, `nowarning`, `no_warning` |
| INFO | `infrared`, `infrastructure`, `infer`, `inflect`, `infix`, `infimum`, `infinite`, `infinity`, `inflate`, `inflation`, `inflating`, `inflow`, `influx`, `influence`, `infill`, `infect` |
| DEBUG | `readback`, `readbinary`, `readbit`, `readbyte`, `readblock`, `readbuffer`, `feedback`, `seedbed`, `broadband`, `deadband`, `fieldbus`, `fieldbuffer`, `modbus`, `redblack`, `codebase`, `codeblock`, `codebook`, `modebit`, `debit`, `debt`, `debounce`, `deblock`, `debian`, `loadbalance`, `loadbalancing` |
| TRACE | `abstract`, `attribute`, `constrain`, `contract`, `contrast`, `control`, `electric`, `electrode`, `electron`, `entries`, `entry`, `entropy`, `extract`, `instrument`, `intrinsic`, `matrix`, `matrices`, `metric`, `retrieve`, `retrieval`, `stream`, `strength`, `stress`, `stride`, `string`, `strip`, `strobe`, `strong`, `struct`, `track`, `trap`, `tray`, `treat`, `tree`, `trend`, `tremor`, `transact`, `transceiver`, `transfer`, `transform`, `transient`, `transistor`, `transit`, `translate`, `translation`, `translator`, `transmission`, `transmit`, `transparent`, `transparency`, `transpose`, `transposition`, `transport`, `transducer`, `trial`, `triangle`, `triangular`, `trigger`, `trim`, `trip`, `tristate`, `tri_state`, `true`, `truncate`, `truncation`, `try`, `ultrasonic`, `ultrasound`, `ultraviolet` |
| VERBOSE | `nonverbose`, `non_verbose` |

`trial` ist eingeschlossen. `Bandwidth` benötigt keinen Ausschluss, weil es weder `db` noch `deb` enthält; `Broadband` enthält dagegen `db`.

| Name ohne Format-Fallback | Ergebnis |
|---|---|
| `NonFatalError` | ERROR |
| `LogCriticalSection` | `untagged` |
| `CriticalSectionError` | ERROR |
| `InterruptError` | ERROR |
| `InfrastructureInfo` | INFO |
| `ReadbackDebug` | DEBUG |
| `TransferTrace`, `TrialTrace` | TRACE |
| `TrialMode`, `Retrial` | `untagged` |
| `LogNonVerbose` | `untagged` |

Die veröffentlichten Suchwörter, Ausschlüsse, Prioritäten und Fallback-Regeln werden Teil des TIL-Vertrags. Ihre spätere Änderung darf alte Aufzeichnungen nicht stillschweigend anders klassifizieren. Dafür müsste zuerst eine ausdrücklich geprüfte Unterscheidung alter und neuer Bedeutung festgelegt werden. Die jetzige Arbeit führt dafür keine Versions- oder Historienfelder ein.

## Seltene Overrides und historische Grenzen

Beispiel mit den relevanten Optionen; Quell- und Transportparameter kommen wie üblich hinzu:

```sh
trice bind -src app -til til.json -li li.json -alias MyWrn:err
tlog -til til.json -li li.json -alias MyWrn:err
```

Die freie Abweichung `MyWrn:err` wird nicht in der TIL gespeichert. Der Logger muss sie erneut erhalten. Ohne sie ergibt der Name wieder WARNING. Ein vergessenes Argument kann der Host nicht zuverlässig erkennen.

Beim Build beeinflusst der Override Tag, passende ID-Bereiche und CE-Auswahl. Beim Logging beeinflusst er nur die Host-Klassifikation. Er ergänzt keine fehlenden CE-Werte und ändert keine bereits gebaute Target-Route.

Der Override gilt für alle geladenen IDs mit diesem Aliasnamen. Unterschiedliche historische Overrides desselben Namens lassen sich mit einer Hostoption auch bei unterschiedlichen IDs nicht auseinanderhalten. Benötigte Logging-Konfiguration muss deshalb extern mit der Aufzeichnung aufbewahrt werden. Eine allgemeine historische Tag-Garantie oder ein garantierter ID-Wechsel bei geändertem Override wird nicht versprochen.

## Darstellung und Host Funktionen

Der bestimmte Ereignis-Tag gilt für Filter, Farbe, Statistik, Level, JSON/KV und Visualisierung. Die Darstellung wertet nur einen bekannten statischen Quellpräfix nach den folgenden Regeln aus:

- Entspricht dessen kanonischer Tag dem Ereignis-Tag, gelten die üblichen Darstellungsregeln.
- Bei aktiver Palette und `-color none` verschwindet ein vollständig kleingeschriebener passender Präfix.
- Gemischte oder große Schreibweisen bleiben sichtbar.
- Ein widersprechender Präfix bleibt Meldungstext.
- `-color off` erhält sämtliche geschriebenen Präfixe.
- Es wird kein künstlicher Präfix hinzugefügt.

| Quellaufruf | Ereignis-Tag | Meldung bei aktiver Palette oder `none` | Meldung bei `off` |
|---|---|---|---|
| `printi("err:failed")` | ERROR durch Fallback | `failed` | `err:failed` |
| `MyWrn("wrn:failed")` | WARNING durch Namen | `failed` | `wrn:failed` |
| `MyWrn("WRN:failed")` | WARNING durch Namen | `WRN:failed` | `WRN:failed` |
| `MyWrn("err:failed")` | WARNING durch Namen | `err:failed` | `err:failed` |

JSON/KV verwenden dieselbe Präfixregel für das Meldungsfeld und enthalten keine ANSI-Farbe. Tag-artiger Text aus Parametern oder empfangenen Laufzeitstrings darf weder neu klassifiziert noch als vermeintlicher Quellpräfix entfernt werden. Das gilt auch bei mehrzeiligen Meldungen und mehreren Ereignissen innerhalb einer Textzeile.

Im ausdrücklich akzeptierten CE-Fallback-Sonderfall verwendet der Host das gespeicherte statische Format für Klassifikation und Darstellung. Die Groß-/Kleinschreibung eigener Tags folgt denselben Darstellungsregeln wie bei Built-ins; ihre schreibungsunabhängige Zuordnung schreibt den Meldungstext nicht um.

Die Statistik zählt weiterhin erfolgreich dekodierte Ereignisse vor der Filterung. Tool- und Transportdiagnosen werden keine Alias-Ereignisse. LI zeigt auf die Alias-Aufrufstelle; Tag-Auflösung funktioniert auch mit `-li off`.

Bei `-vis` vergleichen registrierte Tag-Selektoren den aufgelösten Ereignis-Tag. Freie, unregistrierte Selektoren behalten ihre bisherige exakte Auswahl am statischen Formatpräfix. Die normalen Ereignisfilter werden weiterhin vor der Vis-Auswahl angewendet. Details und Sollbeispiele stehen im Ergebnis der Vorprüfung.

`-verbose` erklärt Grundtyp, Tag und Zuordnungsgrund einschließlich Ausschlüssen, Gleichstand, Fallback und Override. Ein widersprechender bekannter Quellpräfix wird ausschließlich dort erläutert. Die neue Warnung bei verändertem CE-Fallback erscheint davon unabhängig auch ohne `-verbose`. Es gibt keinen neuen Warnschalter. Echte Eingabe- oder Dateifehler bleiben normale Fehler.

## Context Enrichment

| CE-Erweiterung | Skalare native Trices und `-alias` | Natives `triceS` | `-salias` |
|---|---|---|---|
| Fester Text ohne zusätzliche Werteargumente | Erlaubt innerhalb der Format- und Erkennungsgrenzen | Erlaubt innerhalb der Format- und Erkennungsgrenzen | Für v2.0.0 zurückgestellt; ausgewählte Erweiterung ist ein Fehler |
| Zusätzliche skalare Laufzeitwerte | Nach den bisherigen skalaren Regeln erlaubt | Ausgeschlossen | Ausgeschlossen |
| Zusätzliche Laufzeitstrings, auch `%s` in benannten Feldern | Ausgeschlossen | Ausgeschlossen | Ausgeschlossen |

Es gibt keine Ausnahme für Meldungen ohne ursprüngliche Werteargumente. Bereits vorhandene Stringargumente in unterstützten Quellaufrufen bleiben nutzbar. Die Grenze betrifft die von CE neu hinzugefügten Werte.

Skalare CE behält die vorhandenen Grenzen: höchstens zwölf Werte gleicher Bitbreite, unveränderte Stempelart und Float-Regeln mit `aFloat` beziehungsweise `aDouble`. Zusätzliche Ausdrücke dürfen nicht mehrfach ausgewertet werden.

Bei Aliasen wird zuerst die CE-Gruppe des effektiven Ereignis-Tags ausgewählt, danach die zusammenhängende Quellpräfixfolge in Quellreihenfolge. Jede kanonische Gruppe wirkt insgesamt einmal; Regeln derselben Gruppe behalten ihre CLI-Reihenfolge.

Für diese Auswahl gilt der aus dem ursprünglichen Quellformat bestimmte Build-Tag. Auch bei der akzeptierten Fallback-Warnung wird CE nicht erneut mit dem erwarteten Host-Tag ausgewählt. Built-in- und Custom-Tags ignorieren die Groß-/Kleinschreibung: Ein virtueller Tag `sensor` und Quellpräfixe `Sensor` oder `SENSOR` wählen dieselbe CE-Gruppe insgesamt einmal. Freie CE-Selektoren bleiben ebenfalls schreibungsunabhängig.

Ein Quellpräfix kann CE auswählen, ohne den Ereignis-Tag zu ändern. Bei `MyWrn("err:ctx:value=%u", value)` wird zuerst WARNING-CE ausgewählt. ERROR-CE kann über `err` und weitere CE über `ctx` hinzukommen. Passende freie kleingeschriebene Selektoren verschwinden nach den bestehenden CE-Regeln; bekannte Tag-Präfixe bleiben für die vereinbarte Darstellung erhalten.

Eine ausgewählte unzulässige Regel wird mit Fehler abgewiesen. Erreicht beispielsweise eine WARNING-CE-Regel mit festem Text oder zusätzlichen Werten auch einen WARNING-String-Alias, wird dieser nicht stillschweigend übersprungen. Das ist ein Fehler und nicht die tolerierte Fallback-Warnung. Ungültige String-CE-Regeln werden auch dann abgewiesen, wenn sie keine Aufrufstelle auswählen.

### Fester Text und die Grenzen bei String Trices

Skalares `trice` und natives `triceS` besitzen ein reguläres Hostformat. Bei `triceS("msg:%s\n", text)` kann `-ce 'msg:" [Kontext]"'` das effektive Format zu `msg:%s [Kontext]\n` erweitern. Das Target überträgt weiterhin nur seinen ursprünglichen Laufzeitstring; der Host ergänzt den festen Text.

Ein natives `triceS` überträgt eine Zeichenkette. Zusätzliche Zahlen oder weitere Strings würden einen anderen Übertragungs- oder Formatierungsweg verlangen. Auch ein zunächst wertloser skalarer Aufruf müsste für String-CE seine Übertragungsart wechseln.

Ein printf-artiger String-Alias könnte zusätzliche Argumente vor der Übertragung auf dem Target formatieren. Dafür wären jedoch ein eigener Vertrag für die Projektmakros, zusätzliche Analyse und unterschiedliche Regeln für skalare und vorformatierte Werte erforderlich. Diese Werte wären anschließend Text und keine einzeln typisierten Structured-Logging-Felder. Außerdem gibt es String-Aliase, die nur an `triceS` weiterleiten oder eine andere Argumentliste besitzen.

Der printf-CE-Sonderpfad ist deshalb für v2.0.0 insgesamt zurückgestellt, auch für zusätzliche Integer- und Float-Werte. Bei `-salias` wird jetzt auch fester CE-Text zurückgestellt: Die gespeicherte C-Argumenthülle ist kein reguläres Hostformat, und ein eigener Speicher- oder Formatierungsweg wäre zusätzlicher Aufwand. Ursprüngliche Stringargumente und die bisherige Projektformatierung bleiben ohne CE-Erweiterung nutzbar. Eine spätere Erweiterung wird bei konkretem Bedarf neu bewertet; #767 muss dafür keinen neuen Speicherweg mehr nachweisen.

## CLI Optionsnamen und User Tags

Optionsnamen mit genau einem Zeichen bleiben case-sensitiv. Längere Optionsnamen werden case-tolerant, auch mehrbuchstabige Kurzformen. Unterkommandos behalten ihre bisherige exakte Schreibweise.

| Namen | Verhalten |
|---|---|
| `-v` und `-V`, `-a` und `-A` | Verschiedene Namen; nicht registrierte Formen bleiben Fehler. |
| `-sa`, `-sA`, `-SA` | Gleichwertig. |
| `-utag`, `-uTag`, `-UTAG` | Gleichwertig. |
| `-IDRange`, `-idrange` | Gleichwertig. |
| `-logFormat`, `-logformat` | Gleichwertig. |

Optionswerte werden nicht pauschal kleingeschrieben. C-Identifier, Dateipfade, Formate und Ausdrücke behalten ihre Bytes. Tag-Werte werden über die gemeinsame schreibungsunabhängige Suche aufgelöst; die ursprüngliche Präfixschreibweise bleibt für die Darstellung erhalten. Beide Formen `-name value` und `-name=value` gelten. Die bestehende Behandlung wiederholter Zuweisungen bleibt erhalten.

Auch eigene Tags aus einem Zeichen folgen der Tag-Regel: `x` und `X` bezeichnen eine Gruppe, während einbuchstabige CLI-Optionsnamen weiterhin case-sensitiv sind. Früher getrennte Custom-Tags wie `new` und `NEW` fallen zusammen. Das ist eine bewusst dokumentierte Verhaltensänderung in v2.0.0; historische Wörterbücher werden dafür nicht umgeschrieben. Die feste Suchwortliste für Aliasnamen wird nicht um Custom-Tags erweitert.

`-ulabel` entfällt vollständig. Alle seine Groß-/Kleinschreibungsvarianten scheitern mit einem hilfreichen Verweis auf `-utag`.

| Kommando | Zulässige `-utag`-Werte |
|---|---|
| `bind`, `insert`, `clean` | `name` |
| `log`, `tlog` | `name`, `name:weight`, `name:color` nach der bisherigen Wertesyntax |

Gewicht und Farbe sind Host-Einstellungen. Build-Kommandos weisen solche Angaben vor Änderungen ab. Clean benötigt die passende Namensregistrierung für CE-Rücknahme, nicht für das bloße Entfernen von IDs.

Die CLI-Umsetzung muss zusätzlich `generate -logC` mit optionalem Pfad, Werte mit führendem Minus, `--`, Bool-Optionen und die Erkennung ausdrücklich gesetzter Optionen korrekt behandeln. Eine Umwandlung der gesamten Argumentliste in Kleinbuchstaben ist ungeeignet.

## TIL Darstellung und ID Identität

Die TIL bleibt eine ID-Map mit `Type` und `Strg`. Neue Alias-Einträge speichern den exakten Aliasnamen in `Type`. Für die Verarbeitung werden gespeicherter Name und aufgelöster Grundtyp getrennt gehalten.

Eine vollständige vorhandene Hülle `SAlias_Strg('...')SAlias_Strg` in `Strg` kennzeichnet den String-Grundtyp. Ohne diese Hülle ist ein zulässiger Alias skalar. Namensbestandteile wie `32`, `Assert` oder ein abschließendes `S` bestimmen dies nicht.

Bei skalaren Aliasformaten sind `SAlias_Strg('` am Anfang und `')SAlias_Strg` am Ende reserviert. Bereits einer dieser Randmarker führt vor Änderungen zum Fehler. Markerähnlicher Text mitten in der Meldung bleibt zulässig. Beschädigte Teilhüllen sind zu diagnostizieren; beim Verlust beider Marker ist eine Beschädigung nicht stets erkennbar.

Der Vergleichsschlüssel umfasst den exakten Aliasnamen und das effektive `Strg` einschließlich einer vorhandenen Hülle. Unveränderte Instrumentierung hält IDs stabil. Eine Umbenennung, eine reine Schreibweisenänderung, ein Grundtypwechsel oder eine Formatänderung kann eine andere historische oder eine neue ID benötigen. Der bisherige Eintrag bleibt erhalten. Cachefelder, aufgelöste Typen, Farben und Gewichte dürfen den Vergleichsschlüssel nicht verändern.

Alte normalisierte native Einträge bleiben nach ihren bisherigen Nutzdatenregeln lesbar. Die neue schreibungsunabhängige Tag-Regel gilt in dieser Version auch für native Einträge. Deren frühere Aliasnamen sind nicht rekonstruierbar. Neue Instrumentierung darf solche historischen Einträge nicht unter gleicher ID auf Aliasnamen umschreiben. Gemischte TIL sind zulässig. Alte Tools müssen neue Alias-Typen nicht korrekt auswerten können; eine automatische Migration ist nicht vorgesehen.

Zusätzliche Tag-/Meta-/Schemafelder, eine Tag-Begleitdatei, synthetische sichtbare Präfixe und neue Wire-Felder sind nicht beauftragt. Für den akzeptierten CE-Fallback-Sonderfall wird auch keine neue `Strg`-Kodierung eingeführt und keine getrennte ID garantiert.

## Ergebnis der Integrationsprüfung zu Issue #767

Die Vorprüfung ist am Quellstand `11c47309` abgeschlossen. Sie hat Quellpfade und Sollfälle geprüft, keine Feature-Implementierung oder Verhaltensprüfung ausgeführt. Die offenen Implementierungsarbeiten sind #768, #761 und #769 zugeordnet. Zusätzliche Speicherung und ein CE-Textweg für `-salias` sind dafür nicht erforderlich.

### CE-Fallback-Warnung und Verarbeitungsreihenfolge

Bind bestimmt den Build-Tag vor der CE-Auswahl aus dem ursprünglichen statischen Format. Danach werden der virtuelle Tag und die Quellgruppen jeweils einmal ausgewählt, Erweiterung und Nutzdaten validiert und das effektive Wörterbuchformat gebildet. Erst dann wird dessen erwarteter Host-Tag mit dem Build-Tag verglichen. Der Vergleich löst bei Abweichung eine Warnung aus; er wählt weder CE erneut aus noch ändert er den ursprünglichen Tag für die ID-Bereichsauswahl.

Mit `-alias printi -ce 'ctx:" [checked]"'` gelten diese Sollwerte:

| Ursprüngliches Quellformat | Build-Tag | Effektives `Strg` | Host-Tag | Warnung |
|---|---|---|---|---|
| `ctx:err:failed` | `untagged` | `err:failed [checked]` | ERROR | Ja |
| `err:failed [checked]` | ERROR | `err:failed [checked]` | ERROR | Nein |
| `untagged:ctx:err:failed` | `untagged` | `untagged:err:failed [checked]` | `untagged` | Nein |
| `ctx:failed` | `untagged` | `failed [checked]` | `untagged` | Nein |

`printi:untagged` als Override bei Build und Host hält beide Tags auf `untagged`. Bei `MyWrn("ctx:err:failed")` hält die Namensheuristik beide auf WARNING. Beide Fälle erzeugen keine neue Warnung. Bei einem registrierten `ctx` bleibt der Präfix erhalten und beide Tags sind CTX; auch dann gibt es keine Fallback-Warnung.

Die ersten beiden Tabellenzeilen dürfen denselben Aliasnamen und dasselbe effektive Format besitzen. Die verlorene Unterscheidung wird nicht gespeichert und nicht durch eine Kollisionssuche oder zusätzliche ID-Schlüssel ersetzt. Workarounds bleiben `printi("untagged:ctx:err:failed")` oder ein bei Build und Logging wiederholter expliziter Override.

Insert/Clean behalten CE-Selektoren im physischen Quellliteral. Bei wiederholtem Insert oder Clean mit passenden CE-Einstellungen muss deshalb zuerst die vorhandene vollständige Erweiterung über `sourceContextMatch` erkannt und das Originalformat rekonstruiert werden. Das physische Literal allein ist nicht das gespeicherte effektive Format. Für die Warnung werden rekonstruiertes Original und effektive Wörterbuchform verglichen; die Prüfung gehört vor Cache-Abkürzungen und vor Veröffentlichung der Änderungen. Der heutige erste Clean-Durchlauf kehrt noch vor dieser Prüfung zurück; der spätere Restore-Durchlauf darf die Warnung nicht doppelt ausgeben.

Ein mögliches Warnungsformat ist: `app.c:12: warning: alias printi: CE changes format fallback from untagged to ERROR; keep a first tag prefix or repeat -alias printi:untagged when logging`. Pro betroffener Aufrufstelle erscheint höchstens eine solche normale Warnung, auch ohne `-verbose`; ansonsten gültige Verarbeitung läuft weiter. Einfaches ID-Clean erzeugt keine neue CE-Warnung. Ungültige Regeln und ausgewählte feste oder werttragende CE-Erweiterungen für `-salias` bleiben Fehler vor Änderungen.

### Kanonische Tag-Ausgabe und Vis-Auswahl

Die Reviewentscheidung lautet: Eigene Tags erhalten in Metadaten einen großgeschriebenen kanonischen Namen. Bestehende Built-in-Namen bleiben erhalten, insbesondere `untagged`.

| Eingabe oder Registrierung | Sollverhalten |
|---|---|
| `-utag sensor`, `-utag Sensor`, `-utag SENSOR` | Eine Gruppe SENSOR; JSON/KV und Statistik verwenden SENSOR. |
| Dieselben ungewichteten Registrierungen in anderer Reihenfolge | Dieselbe kanonische Identität und Ausgabe. |
| Beim Logging `-utag sensor:300 -utag SENSOR:650` | Eine Gruppe SENSOR mit Gewicht 650; Farbe bleibt davon unabhängig. |
| `-alias Name:sensor -alias Name:SENSOR` | Gleiche explizite Zuordnung, kein Konflikt. |
| `-utag x`, danach Auswahl mit `X` | Eine Gruppe X; die Regel für einbuchstabige CLI-Optionen ändert sich nicht. |
| Virtueller Tag SENSOR und Quellpräfixe `Sensor:SENSOR:sensor:` | Eine CE-Gruppe, insgesamt einmal angewendet; CLI-Reihenfolge innerhalb der Gruppe bleibt erhalten. |

Die gemeinsame Registry muss sowohl bereits veröffentlichte als auch noch ausstehende ungewichtete Registrierungen zusammenführen. Eine Änderung nur an `tagIndex` reicht nicht: `AddUserLabels` verwendet heute zusätzlich schreibungsabhängige Pending-Map-Schlüssel und erzeugt Custom-Gruppen ohne das Built-in-Case-Flag. Reservierte Selektornamen wie `all` und `off` dürfen nicht über andere Schreibweisen zu neuen Tags werden. Meldungspräfixe behalten ihre Quellschreibweise; nur passende kleingeschriebene Präfixe verschwinden bei aktiver Palette oder `none`.

Die zweite Reviewentscheidung betrifft `-vis`: Registrierte Tag-Selektoren vergleichen den aufgelösten Ereignis-Tag, freie unregistrierte Selektoren behalten ihre bisherige exakte Auswahl am statischen Formatpräfix. Bei `MyWrn("err:x=%u\n", x)` wählt deshalb `-vis wrn:…` das WARNING-Ereignis, `-vis err:…` dagegen nicht. Ein unregistriertes `imu` kann weiterhin über `imu:` im Wörterbuchformat auswählen. Nach Registrierung als Tag greifen für `imu/IMU` die gemeinsamen Tag-Regeln. Normale Ereignisfilter laufen weiterhin vor Vis; Nutzdaten-, Zeilen- und Sink-Grenzen bleiben bestehen.

### Betroffene Aufrufer und Zuständigkeiten

| Pfad und heutige Annahme | Erforderliche Einbindung | Issue |
|---|---|---|
| [CLI und Registry](../../internal/args/handler.go), [Tag-Verarbeitung](../../internal/emitter/lineTransformerANSI.go): exakte Flags, getrennte Custom-Schreibweisen und Pending-Map | Kanonische Optionsnamen; Registry und implizite Alias-Tags vor ID-Bereichen, Filtern und Vis aufbauen; SENSOR/X als kanonische Custom-Ausgabe; letzte Zuweisung je Eigenschaft. | #768, Aliasregistrierung #761 |
| [Aliasparser und Quell-Edits](../../internal/id/helper.go), [Bind-Scan](../../internal/id/bindParse.go), [Matcher](../../internal/id/match.go) und [Update](../../internal/id/Update.go) | Native Namensregeln getrennt von exakten Alias-Identifiern halten; Case-Folding des nativen Regex-Zweigs darf nicht auf Aliasalternativen wirken. Native Indizes wie `Type[2]` und `Type[:5]` nur auf aufgelöste Nutzdatentypen anwenden. | #761 |
| [TriceFmt](../../internal/id/id.go), [Dictionary und Rückwärtsindex](../../internal/id/manage.go), [Bind-Schlüssel](../../internal/id/bindMetadata.go), [geordnete IDs](../../internal/id/orderedIDs.go) | Rohidentität aus exaktem Aliasnamen und effektivem `Strg` halten; aufgelöste Nutzdatenart und Build-Tag separat im Speicher führen, nicht als zusätzliche Vergleichsfelder. Alte native Einträge erhalten. | #761 |
| [Insert](../../internal/id/insertIDs.go), [Clean](../../internal/id/cleanIDs.go), [Zero-ID-Helfer](../../internal/id/zeroIDs.go), [Insert-Cache](../../internal/id/cachedInsert.go) und [Clean-Cache](../../internal/id/cachedClean.go) | Aliasname beim Schreiben erhalten; vorhandene Cache-/ID-Pläne dürfen Validierung, die aktuelle Zuordnung oder Warnungen nicht umgehen. Keine Cache-Migration oder Datenbereinigung. | #761, CE-Warnung #769 |
| [ID-Bereiche](../../internal/id/switchIDs.go) und [CE-Bind](../../internal/id/contextEnrichment.go) | Quell-Tag vor CE für aktuelle ID-Auswahl halten; Warnungsvergleich mit effektivem Format; String-Alias-Erweiterung vor Adaptererzeugung abweisen. | #761 und #769 |
| [CE Insert/Clean](../../internal/id/contextSource.go) | Vollständige Suffixerkennung, Originalrekonstruktion und effektive Wörterbuchform berücksichtigen; Warnung vor Änderungen einmalig, auch bei Wiederholung und Cache. | #769 |
| [Initiales Laden](../../internal/args/handler.go), [AddFmtCount](../../internal/id/manage.go) und [Nachladen](../../internal/id/fileWatcher.go) | `AddFmtCount` verändert heute `Type`. Rohdaten und aufgelöste Sicht getrennt halten und nach erfolgreicher Prüfung gemeinsam unter dem bestehenden Lock veröffentlichen. Ungültige Alias-/Hüllenupdates erhalten die letzte gültige Sicht. Kommando-Registry und Statistik nicht beim TIL-Reload neu initialisieren. | #761 |
| [TREX-Decoder](../../internal/trexDecoder/trexDecoder.go), [Ereignisrecord](../../internal/decoder/record.go), [Textausgabe](../../internal/emitter/lineTransformerANSI.go), [JSON/KV](../../internal/translator/structured.go), [Statistik](../../internal/decoder/decoder.go) | Payload-Familie vor nativer Typauswertung auflösen; Ereignis-Tag und statische Präfixschreibweise bis zur Ausgabe führen. Ein ursprüngliches `%s` darf empfangenes `wrn:payload` nicht zum entfernbaren Präfix machen. Statistik zählt erfolgreiche Ereignisse vor Filtern. | #761 |
| [Vis-Router](../../internal/vis/router.go) und [numerischer Record](../../internal/decoder/decoder.go) | Startup und Laufzeit verwenden dieselbe Tag-Auswahl; numerische Form über aufgelösten Nutzdatentyp bestimmen. `fixedNumericShape` lehnt heute Aliasnamen ab; Fingerprints behalten die Rohidentität. Freie Präfixauswahl erhalten. | #761; gemeinsame Tag-Suche #768 |
| [Generatoren](../../internal/id/generate.go), [C-Tabelle](../../internal/id/generateTil.go), [Quellauswahl](../../internal/id/generateLog.go), [ABC-Auswahl](../../internal/id/generateAbc.go) und [Bind-Bericht](../../internal/id/generateBindReport.go) | Namen/Hüllen in Exporten erhalten, Parameterzahl und Familie aus der aufgelösten Sicht bestimmen; kein `S`-/`32`-/`Assert`-Raten im Aliasnamen. Bestehende native Sondertypen bleiben unverändert unterstützt; keine zusätzlichen Alias-Nutzdatenfamilien oder lokalen Tag-/Filterfunktionen einführen. | #761; CE-Abnahme #769 |

### Zugeordnete Regressionen und Abschluss

| Issue | Später zu implementierende Sollfälle |
|---|---|
| #768 | SENSOR/X in allen Registrierungsreihenfolgen einschließlich Pending-Gruppen; letzte Gewichts-/Farbzuweisung; reservierte Namen in anderer Schreibweise; unveränderte Präfixdarstellung; einbuchstabige Optionsnamen bleiben exakt. |
| #761 | Kurze Aliase wie `A`, reine Case-Unterschiede und Namen mit `S`, `32` oder `Assert`; alte/gemischte TIL, stabile Rohschlüssel, Renames und erhaltene historische Einträge; gültiger und ungültiger Reload; Text/JSON/KV einschließlich Laufzeitpräfixen und LI off; Vis mit Ereignis-Tag sowie freiem Präfix. |
| #769 | Warnungstabelle, Overrides und Namensheuristik ohne Fehlwarnung, wiederholtes Insert und passendes Clean mit/ohne Cache; eine Warnung pro Stelle vor Veröffentlichung und keine zweite CE-Auswahl; native `triceS`-Text-CE positiv, jede ausgewählte `-salias`-Erweiterung negativ; hardwarefreie C-/Host-Integration. |

Alle gefundenen Integrationspunkte und die beiden während der Prüfung geklärten Verhaltensentscheidungen sind damit zugeordnet. Die nächsten Arbeitsschritte bleiben #768 → #761 → #769; deren Feature-Implementierung benötigt einen eigenen Auftrag.

## Geplante Abnahme und Dokumentation

Jede spätere Implementierungsstufe erhält verständliche Verhaltenstests mit unabhängigen Sollwerten. Wesentlich sind:

| Bereich | Nachweis |
|---|---|
| Heuristik | Alle Suchwörter und Ausschlüsse, einzelne Vorkommen, längster Treffer, Gleichstand, Teilüberlappung und unveränderte Schreibweise. |
| Registrierung | Reihenfolgen, Wiederholungen, Konflikte, ungenutzte Definitionen, schreibungsunabhängige Custom-Tags und reservierte Formen. |
| TIL und Historie | Stabile Wiederholung, Renames, gemischte Alt-/Neueinträge, keine Umdeutung alter IDs und dokumentierte Override-Grenze. |
| CE | Virtueller Tag plus Quellgruppen einmalig, fester Text bei nativem `triceS`, abgewiesene String-Alias-Erweiterungen, skalare Grenzen, ausgeschlossene Laufzeitstrings und keine Teiländerungen bei Fehlern. Die akzeptierte Fallback-Abweichung warnt und setzt gültige Verarbeitung fort. |
| Host | Text/JSON/KV, Präfixherkunft, alle Farbmodi, Filter, Statistik, Visualisierung und konsistentes Nachladen. |
| Integration | Hardwarefreier C-Sender und Hostdecoder mit skalarem Alias samt CE, nativem `triceS` samt festem CE-Text sowie printf-String-Alias und direktem String-Wrapper ohne CE; dazu LI und Generate. |

Fokussierte Tests begleiten die Implementierung nach dem Startsignal. Gesamtläufe, Coverage und breite Compiler-/Plattformabnahme werden erst auf ausdrücklichen Auftrag ausgeführt. Native Linux-, macOS- und Windows-Ergebnisse sind von simulierten Plattformtests zu unterscheiden. Diese Vorprüfung hat keine Builds oder Verhaltenstests ausgeführt.

Später werden die vorhandenen RM-Kapitel zu Aliasen, Tags, CLI, IDs, CE, Structured Logging und Generate erweitert. Die obigen Tabellen und Beispiele bilden dafür die deutsche Reviewgrundlage. Der Changelog erhält nur kurze Hinweise und relative RM-Links.

Die LI-Archivierung bleibt extern: finale build-spezifische LI aufbewahren, bei Bedarf über Inhalts-Hash identifizieren, passenden Datensatz extern auswählen und mit `-li` verwenden. ZIPs werden extern entpackt. Trice erhält dafür keine Hash-/Archivdienste oder Historienfelder.

## Archivierte Vorarbeiten

Die früheren Entwürfe sind unveränderte historische Unterlagen und werden durch den akzeptierten Stand dieses Papiers und der Issues abgelöst:

- [Frühere Analyse](obsolete/531-761_Analyse.md)
- [Früherer Diskussionsstand](obsolete/handsoff_discussion_531_alias_tag_mapping_DE.md)

Archivinterne relative Links werden beim unveränderten Verschieben nicht repariert. Eine Entfernung oder Umbenennung von `TRICE_LEGACY_RPC_SUPPORT`, eine Datenbereinigung oder eine automatische LI-/TIL-Migration gehört weiterhin nicht zum Umfang.

