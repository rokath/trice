<!-- Integration: vollstaendiges UM-Hauptkapitel (H2) mit H3/H4-Unterabschnitten.
Keine feste Kapitelnummer vergeben. Nach dem Einfuegen Nummerierung und
Inhaltsverzeichnis mit dem Projektverfahren aktualisieren. Deutsch und Englisch
sind alternative Sprachfassungen, keine aufeinanderfolgenden Kapitel. -->

## <a id="log-levels-tags-and-filtering"></a>Log-Level, Tags und Filterung

> **Status:** Kapitelentwurf auf Grundlage der am 10. September 2026 abgerufenen `wip`-Quellen. Vorhandenes Verhalten, bekannte Einschränkungen und vorgeschlagene Änderungen sind getrennt beschrieben. Die Quellen wurden geprüft; die vollständigen Testsuiten und Target-Builds wurden für dieses Kapitel nicht ausgeführt. Die genannten Korrekturen werden nicht als bereits umgesetzt vorausgesetzt.

### <a id="ll-purpose"></a>Zweck und Grundprinzip

Im normalen Trice-Logging wird eine Meldung durch eine ID identifiziert. Statt eines vollständig formatierten Meldungstextes werden ihre Laufzeitwerte übertragen. Der Host erzeugt die Ausgabe mithilfe des passenden Wörterbuchs. Dadurch ist es praktikabel, Diagnoseinformationen zunächst zu behalten und die sichtbare Teilmenge später auszuwählen. Dasselbe Prinzip kann eine optionale strukturierte Ausgabe tragen, ohne jedem Target-Ereignis zusätzliche Level- oder Feldnamenbytes hinzuzufügen. [S1] [S2]

**Den effizienten Target-Kern beibehalten und zuerst die vorhandenen Auswahlmöglichkeiten nutzen. Feinere Target-Steuerungen erst ergänzen, wenn ein konkreter Ressourcen- oder Ausführungsbedarf sie rechtfertigt.** Das ist die Empfehlung dieser Bewertung, nicht die Behauptung, Target-Filter seien generell nutzlos.

Ein kurzer einzelner Logaufruf garantiert keine vernachlässigbare Gesamtbelastung. Zu betrachten sind Ereignisrate, Argumentberechnung, Pufferung, Framing, Übertragung und Aufzeichnung:

```text
Bytes pro Sekunde = Ereignisse pro Sekunde * mittlere Bytes pro Ereignis
```

TCOBS-Framing beziehungsweise -Kodierung definiert keinen Log-Level. Kompakte Kodierung garantiert weder eine feste Kompressionsrate noch, dass zusätzliche Speicherkompression keinen Nutzen mehr hätte. Für die hier beschriebenen Korrekturen ist keine TCOBS-Änderung erforderlich.

### <a id="ll-terminology"></a>Begriffe

| Begriff | Bedeutung in diesem Kapitel |
|---|---|
| Ereignis | Ein konkreter Trice-Logaufruf. Eine angezeigte Zeile kann mehrere Ereignisse enthalten; ein Ereignis kann mehrere Zeilenumbrüche enthalten. |
| Tag oder Kanal | Ein Präfix wie `err:`, `wrn:` oder `read:`. Es kann einen Schweregrad, eine Kategorie oder eine Darstellungskonvention bezeichnen. |
| Alias | Eine andere akzeptierte Schreibweise derselben Tag-Gruppe. Eine identische Schreibweise darf nicht verschiedenen Gruppen zugeordnet sein. |
| Log-Level / Schweregrad | Wichtigkeit oder Schwere eines Ereignisses, beispielsweise Debug, Information, Warnung oder Fehler. |
| Tag-Auswahl | Auswahl einer ausdrücklichen Kategorienmenge mit `-pick` beziehungsweise Ausschluss mit `-ban`. |
| Level-Schwelle | Auswahl nach einer festgelegten Schweregradordnung. Sie ist nicht dasselbe wie eine ausdrückliche Tag-Liste. |
| Kontextanreicherung | Ergänzen von Eigenschaften wie Task-ID, Firmwarekennung, Quellposition oder Zeitstempel, ohne sie an jeder Logstelle zu wiederholen. |
| Strukturiertes Logging | Informationen bleiben als einzeln zugängliche Datenfelder erhalten und stehen nicht ausschließlich innerhalb eines formulierten Satzes. |
| ID-Routing | Auswahl eines Ausgabekanals anhand der numerischen Ereignis-ID. |

Eine Kategorie wie Lesen, Schreiben oder Security ist nicht von sich aus ein Schweregrad. `OFF` ist eine Einstellung, kein Schweregrad eines ausgegebenen Ereignisses. Numerische Levelwerte sind Konventionen und benötigen zwischen Systemen ausdrückliche Zuordnungen; Trice muss keine vermeintlich universelle Nummerierung übernehmen.

Ein strukturierter Datensatz darf eine freie Textmeldung als einen seiner Werte enthalten. Umgekehrt macht das automatische Einfügen von Kontext in einen Textstring diese Angaben noch nicht automatisch einzeln abfragbar. JSON ist eine mögliche Ausgabeform, nicht die Definition strukturierten Loggings. Diese Unterscheidungen passen zu den getrennten Konzepten für Meldungsinhalt, Attribute, Quelle und Schweregrad im OpenTelemetry-Logmodell. [S10]

### <a id="ll-selection-points"></a>Wo ausgewählt wird und welche Kosten entfallen

| Mechanismus | Auswahlzeitpunkt | Mögliche Einsparung | Was bleibt |
|---|---|---|---|
| `TRICE_OFF` | Kompilierung im konfigurierten Datei-/Projektumfang | Code normaler abgeschalteter Trice-Makros und deren Argumentauswertung | Nicht erzeugte Ereignisse sind später nicht wiederherstellbar |
| Konfiguriertes Target-ID-Routing | Weiterleitung an eine bestimmte Ausgabe | Übertragung/Speicherung an dieser Ausgabe | Erfassung und im geprüften Deferred-Pfad auch Kodierung sind bereits erfolgt |
| `-pick` / `-ban` | Host, derzeit nach dem Formatieren der Meldung | Nachfolgende normale Anzeige/Ausgabe ausgeschlossener Ereignisse | Targetaufwand, Datenübertragung und vorherige Hostdekodierung/-formatierung |
| `-logLevel` | Host-Darstellung im geprüften Stand | Anzeige nach den aktuellen Tag-Reihenfolgeregeln | Targetaufwand und Erfassung; keine Target-Steuerung |
| Geplante Compilezeit-Auswahl nach Level | Als Draft-Erweiterung nicht implementiert | Würde ausgewählte Level aus einem Build entfernen | Wiederaufnahme erfordert einen anderen Build |
| Geplante dynamische Target-Level-Steuerung | Als Draft-Erweiterung nicht implementiert | Könnte Erfassungs- und/oder Übertragungsarbeit vermeiden | Zusätzliche Regeln, Steuerung und Targetentscheidungen |

Die Aussage zum Deferred-Zeitpunkt gilt für die geprüfte Implementierung, nicht pauschal für jede anwenderdefinierte Direct-Ausgabe. Routing ist nicht automatisch eine Unterdrückung vor der Erfassung. [S2] [S3] [S4] [S12] [S5] [S14]

### <a id="ll-target-off"></a>Compilezeit-Steuerung mit TRICE_OFF

Für die normale Trice-Makronutzung einer Quelldatei wird `TRICE_OFF` vor dem ersten Einbinden von `trice.h` in dieser Übersetzungseinheit definiert:

```c
#define TRICE_OFF 1
#include "trice.h"
```

Eine projektweite Compilerdefinition wie `-DTRICE_OFF=1` wirkt entsprechend breiter. Buildkonfiguration und Include-Reihenfolge müssen zusammenpassen. Die Trice-Quellwerkzeuge können die Logstellen und Wörterbuchinformationen weiterhin berücksichtigen; das Herauskompilieren ist keine Aufforderung, historische Einträge aus `til.json` zu löschen. [S1] [S5]

Logargumente sollten keine für das Programm notwendigen Seiteneffekte enthalten. Bei Wrappern ist zusätzlich zu beachten: Das Abschalten eines Trice-Makros innerhalb einer normalen Funktion verhindert nicht zwingend die Auswertung ihrer Argumente beim Aufrufer. Beispielsweise kann `log_motor(read_sensor())` weiterhin `read_sensor()` aufrufen. Ein späterer levelabhängiger Wrapperentwurf muss dies ausdrücklich berücksichtigen.

Diese breite Compilezeit-Steuerung existiert bereits. Der zusätzliche Mechanismus mit `trice_INFO`/`trice_DEBUG` ist eine eigene, zurückgestellte Erweiterung.

### <a id="ll-tags-aliases"></a>Tags, Aliase und eigene Labels

Ein bekannter Tag wird normalerweise am Meldungsanfang vor dem ersten Doppelpunkt erkannt. Aliasgruppen ordnen Schreibweisen wie `err` und `ERROR` derselben Kategorie zu. Die Groß-/Kleinschreibung kann trotzdem die Darstellung beeinflussen: Bekannte kleingeschriebene Präfixe werden in den entsprechenden Farbmodi normalerweise entfernt, großgeschriebene können sichtbar bleiben. Die Farboptionen `off` und `none` sind deshalb nicht gleichbedeutend. [S3] [S6]

**Bekannter Fehler im geprüften Stand:** Die identischen Schreibweisen `w`, `rx`, `tx`, `s` und `S` kommen in unterschiedlichen Gruppen vor. Einige Codepfade nehmen den ersten Treffer, andere berücksichtigen weitere Treffer. Bis zur Korrektur sollten eindeutige Schreibweisen wie `wrn`, `write`, `read`, `receive`, `transmit`, `signal` und `Seconds` verwendet werden. Die Korrektur benötigt eine Prüfung des gesamten Registers einschließlich künftiger Ergänzungen und eigener Labels; es handelt sich nicht nur um eine kosmetische Tabellenbereinigung. [S3]

Der tatsächliche Schalter heißt **`-ulabel`**, nicht `-uLabel`. Er registriert eigene Labels, weist ihnen aber nicht automatisch einen fachlich sinnvollen Schweregrad zu. Dokumentiert ist diese Syntax:

```text
-ulabel motor:sensor -ulabel control
```

**Bekannte Einschränkungen im geprüften Stand:** Die Implementierung zerlegt die Doppelpunktliste nicht wie dokumentiert. Außerdem werden Pick-/Ban-Aliase aufgelöst, bevor die neu angegebenen Labels registriert sind. Ein eigener Selektor im selben Kommando funktioniert deshalb nicht schon dadurch zuverlässig, dass `-ulabel` zuerst steht. Diese Verwendung setzt die entsprechenden Korrekturen oder eine ausdrückliche Prüfung des konkreten Verhaltens voraus. [S3] [S4] [S12] [S6]

Die angestrebte Reihenfolge lautet: Optionen sammeln; alle Labels registrieren und validieren; Selektoren auflösen; ungültige oder widersprüchliche Angaben ablehnen; erst danach Ein- und Ausgaben öffnen. Das ist eine vorgeschlagene Korrektur, keine Beschreibung des aktuellen Ablaufs.

### <a id="ll-pick-ban"></a>Ausdrückliche Tag-Auswahl mit pick und ban

`-pick` behält die genannten Tag-Gruppen. `-ban` unterdrückt die genannten Gruppen. Listen verwenden Doppelpunkte; die Schalter können wiederholt werden. Bekannte Aliase werden auf ihre Gruppe erweitert. [S4] [S12]

```text
-pick wrn:err
-ban dbg
```

`-pick wrn:err` wählt genau diese Gruppen. Es bedeutet **nicht** eine Level-Schwelle wie „Warnung und alles Schwerwiegendere“; weitere schwere Gruppen werden nicht automatisch eingeschlossen.

Im geprüften Filter wird ein ungetaggtes Ereignis bei einer nichtleeren Pick-Auswahl unterdrückt und passiert eine Ban-Auswahl. Ein nicht passendes Präfix wird ebenso durch Pick ausgeschlossen und durch Ban durchgelassen. Beide Optionen sparen weder Targetarbeit noch Übertragungsbytes. [S4] [S12]

`-pick` und `-ban` dürfen nicht gemeinsam verwendet werden. Der entsprechende Fehler wird aktuell erst im Ereignisfilter erkannt; er sollte stattdessen bereits bei der Startvalidierung gemeldet werden.

**Bekannter Validierungsfehler:** Unbekannte Namen werden stillschweigend verworfen. Eine vollständig unbekannte Pick-Auswahl kann dadurch zu „kein Filter“ werden. Richtig wäre ein Kommandozeilenfehler mit dem ungültigen Namen, nachdem eigene Labels registriert sind. Ein bekannter Selektor ohne eintreffende passende Ereignisse ist hingegen gültig; fehlende Daten sind kein Schreibfehler. [S4] [S12]

### <a id="ll-host-level"></a>Hostseitiges logLevel

Die vorhandene Hostoption wird mit großem `L` geschrieben:

```text
-logLevel all
-logLevel wrn
-logLevel off
```

`all` ist die Voreinstellung. `off` unterdrückt die normale Ausgabe im betreffenden Darstellungspfad; es schaltet weder die Targeterfassung noch eine angeforderte Binäraufzeichnung ab. Für Werkzeugdiagnosen und andere Verbraucher braucht es ausdrückliche Regeln statt der Annahme, der Schalter lege alles still. [S3] [S6]

**Aktuelle Einschränkung:** Der Schwellwert wird aus der Position in der Darstellungstabelle `Tags` abgeleitet. Diese enthält neben herkömmlichen Levelnamen auch Kategorien, Einheitentags und angehängte Benutzerlabels. Die Levelprüfung beeinflusst zudem Darstellungsfragmente wie Zeitstempel, Quellposition, Präfix und Suffix. Sie ist damit noch kein unabhängiges, durchgängiges Ereignis-Schweregradmodell. [S3] [S6]

Bevorzugt wird, die nützliche Schwellwertfunktion zu behalten und Schweregrade von der Darstellung zu trennen. Für Kategorien ohne Schweregrad, Benutzerlabels, ungetaggte Ereignisse und die Kombination mit einer ausdrücklichen Tag-Auswahl sind Regeln festzulegen. Ein neuer Farbeintrag darf den Schweregrad nicht verändern. Dafür braucht es weder ein zusätzliches Target-Level-Feld noch nach Schweregrad geordnete IDs.

`-logLevel` sollte nicht stillschweigend entfernt werden. Auch seine Entfernung bricht Kompatibilität. Falls seine Pflege bewusst verworfen wird, braucht es eine angekündigte Abkündigung mit Migrationsweg. Unbekannte Werte sollten Startfehler auslösen statt zufälliger Tabellenindexeffekte.

### <a id="ll-raw-recording"></a>Binäraufzeichnung und spätere Neuauswahl

`-binaryLogfile` und der Alias `-blf` zeichnen den empfangenen Datenstrom vor Übersetzung und Anzeigefilterung auf. `-logfile`/`-lf` erfassen Textausgabe und sind kein gleichwertiger Ersatz. Eine explizit benannte Binärdatei wird erweitert, wenn sie bereits existiert. [S6] [S7]

Ein Beispiel für die Wiedergabe, wenn `session.bin` und die passende `til.json` bereits im Arbeitsverzeichnis liegen:

```bash
tlog -p FILEBUFFER -args session.bin -idlist til.json -pick wrn:err -blf off
```

Dieselben empfangenen Daten ohne diese Tag-Auswahl wiedergeben:

```bash
tlog -p FILEBUFFER -args session.bin -idlist til.json -logLevel all -blf off
```

`tlog` verwendet den Logging-Kommandopfad; `trice log` akzeptiert die entsprechenden Logging-Optionen. Abweichende Kodierung, Framing- und Targeteinstellungen sowie eine gewünschte Standortdatei müssen passend angegeben werden. Eine Wiedergabe kann kein Ereignis rekonstruieren, das nie empfangen wurde. [S7] [S8]

Zur Aufzeichnung gehören das relevante Wörterbuch und die Firmware-/Buildidentität. Bei wichtigen Quellpositionen sind auch die passenden Standortmetadaten aufzubewahren. Die Wiedergabe darf nicht in ihre eigene Eingabedatei zurückschreiben. Eine absicherbare Eigenschaft lautet: Gleicher empfangener Datenstrom ergibt unabhängig von Host-Anzeigefiltern dieselbe Binäraufzeichnung.

### <a id="ll-id-allocation"></a>ID-Vergaberegeln und Wörterbuchhistorie

Insert und Bind bieten `-IDMin`, `-IDMax`, `-IDMethod` (`random`, `upward` oder `downward`) sowie wiederholbare tagbezogene `-IDRange`-Regeln. Allgemeiner und spezielle Bereiche dürfen sich nicht überschneiden. Diese Optionen steuern die ID-Vergabe; sie konfigurieren nicht von selbst ein Ausgabegerät. [S6] [S9] [S13]

Dieses Beispiel veranschaulicht eine Regel für ein Anwendungsquellverzeichnis `app`. Es schreibt die üblichen Bind-Artefakte und muss an das tatsächliche Projekt angepasst werden:

```bash
trice bind -src ./app -IDMin 1000 -IDMax 9999 -IDRange err:10,99
```

Der Fehlerbereich reicht einschließlich von 10 bis 99. Andere betreffende Tags erhalten IDs aus dem allgemeinen Bereich. Die Bereiche müssen im tatsächlich unterstützten ID-Raum liegen; die Konfiguration sollte zum Build archiviert werden.

**Vorhandene IDs werden nicht automatisch migriert.** Die Wiederverwendungslogik kann eine ID außerhalb einer neu konfigurierten Vorgabe beibehalten. Das dient der Stabilität, macht eine Vergaberegel aber noch nicht zum Nachweis, dass alle aktiven Fehlermeldungen bereits in diesem Bereich liegen. `til.json` enthält außerdem historische Einträge und darf nicht pauschal umnummeriert werden. [S9] [S13]

Vorgeschlagen ist eine Prüfung aktiver Zuordnungen gegen die aktuelle Regel, mit aussagekräftigen Warnungen und gegebenenfalls strengem CI-Modus. Rein historische Einträge sind getrennt zu behandeln; eine unvollständige Quellprüfung kann keine vollständige Konformität beweisen. Ein optionaler Fix sollte Änderungen vorab anzeigen, Quellen oder Sidecars konsistent aktualisieren, alte Zuordnungen erhalten und einen Neubau verlangen. Solche Prüf-/Migrationsfunktionen sind Vorschläge, keine hier dokumentierten vorhandenen Kommandos.

**Zusätzlicher Validierungsfehler:** `EvaluateIDRangeStrings` überspringt im geprüften Stand Regeln ohne Doppelpunkt stillschweigend. Fehlerhafte Angaben sollten vor Änderungen an Quellen oder generierten Artefakten abgelehnt werden. [S9] [S13]

### <a id="ll-target-routing"></a>Target-ID-Routing und Packing-Modi

Eine konfigurierte Ausgabe kann mittels ID-Intervall nur ausgewählte Meldungen weiterleiten. Andere Ausgaben können dasselbe Ereignis trotzdem erhalten. Beispielsweise kann ein Ziel den breiten Datenstrom und ein anderes nur den Fehlerbereich bekommen. Das ist Ausgaberouting, nicht zwangsläufig eine Unterdrückung der anfänglichen Erfassung. [S2] [S5] [S6]

Der beabsichtigte Intervallvertrag ist einschließlich beider Grenzen. **Bekannter Fehler:** Die geprüften Deferred-Vergleiche verwenden beidseitig striktes `<`, obwohl Vergabe und Beschreibung der kleinsten/größten ID einschließlich sind. Bis zur Korrektur darf die Weiterleitung der Grenzereignisse nicht vorausgesetzt werden. Eine zwischenzeitliche Erweiterung der Grenzen als Workaround ist nach einer Korrektur erneut zu prüfen. [S2] [S5] [S9] [S13]

Ein zweites Konfigurationsproblem betrifft die Aktivierung: UARTA und UARTB prüfen von null verschiedene Grenzen unterschiedlich (`||` beziehungsweise `&&`). Auxiliary-/RTT-Pfade verwenden Definitionstests. Fehlende, voreingestellte und nullwertige Grenzen brauchen einen eindeutigen gemeinsamen Vertrag; der bloße Austausch eines Operators reicht nicht. [S2]

Für die derzeitigen aktiven Deferred-Routingzweige nach Ereignis-ID gilt:

```c
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
```

**Die dazugehörigen Compilezeit-`#error`-Prüfungen existieren bereits.** Sie sollten mit positiven und negativen Buildtests für die betreffenden Ausgaben und Teilkonfigurationen abgesichert werden. Ein pauschaler Fehler für jede Multi-Pack-Nutzung wäre falsch. Gültiges Multi-Pack ohne ID-Routing ist ein anderer Fall; Direct-/Custom-Routen brauchen ihren eigenen Vertrag. [S2]

Die Einschränkung ist keine grundsätzliche TCOBS-Eigenschaft. Eine spätere Implementierung könnte vor dem Zusammenpacken über einzelne Ereignisse entscheiden; das wäre eine separate, durch Nutzen zu begründende Änderung.

### <a id="ll-integrity-and-performance"></a>Diagnosen, Ereignisgrenzen, Statistiken und Hostleistung

Ein zugelassenes Ereignis und seine ausgewählten Metadaten sollten derselben Filterentscheidung folgen. Der Styling-Tag eines Zeitstempels darf nicht entscheiden, ob der Zeitstempel einer zugelassenen Warnung verschwindet. Zugleich darf die bestehende Zusammensetzung von Teilzeilen nicht versehentlich verändert werden: Mehrere Aufrufe können eine Zeile bilden und ein Aufruf kann mehrere Zeilen umfassen. Tests müssen EOF-Abschluss, `-addNL`, ausgeblendete Fragmente und die Herkunft der Metadaten abdecken. [S3] [S4] [S12]

Der Translator führt formatierte Ereignisse und bestimmte Fehlermeldungen durch denselben Tag-Filter. Für unbekannte IDs und Dekodier-/Integritätsdiagnosen ist deshalb eine bewusste eigene Regel erforderlich. Sie dürfen nicht nur deshalb verschwinden, weil die Anwendungsauswahl ihren Darstellungstag nicht enthält. Eine künftige maschinenlesbare Ausgabe muss Diagnosen trennen oder unter einem definierten Schema kodieren. [S4] [S12]

Bei Statistiken ist festzulegen, was gezählt wird: Empfangene Daten, dekodierte Ereignisse, ausgewählte Ereignisse, angezeigte Ereignisse und Diagnosen sind nicht identisch. Einfärben und Aufteilen in Darstellungsfragmente dürfen keine zusätzlichen Anwendungsereignisse erzeugen.

Eine frühere Hostfilterung ist eine sinnvolle Optimierungsoption, da Pick/Ban aktuell auf bereits formatierten Meldungen arbeiten. Eine Entscheidung anhand von Decoder und Wörterbuch könnte Formatierungsarbeit sparen. Framing, Cycle-/Integritätszustand, notwendige Prüfungen, Rohaufzeichnung sowie die festgelegte Semantik von Zeitdifferenzen, Statistiken und Visualisierung müssen dabei erhalten bleiben. Vor zusätzlicher Komplexität sind CPU, Allokationen und der ungefilterte Ausgangspfad zu messen. [S4] [S12] [S11]

### <a id="ll-deferred-extensions"></a>Zurückgestellte Target-Level-Erweiterungen

#### <a id="ll-deferred-compile"></a>Compilezeit-Auswahl nach Level

Der Future-Development-Draft schlägt Namen wie `trice_INFO`, `trice_DEBUG` und eine dazugehörige Buildtransformation vor. Damit könnten ausgewählte Ereignisse samt Argumentauswertung aus einem Produktionsbuild verschwinden. Gleichzeitig entstehen weitere Makrofamilien, Werkzeugregeln und Buildvorgaben. [S1]

**Entscheidung dieser Bewertung: Bis zu einem unmittelbaren, dokumentierten Bedarf zurückstellen.** Ein Anlass wäre eine gemessene Codegrößen-/Laufzeitgrenze oder eine notwendige Buildvorgabe, die `TRICE_OFF` und die vorhandenen Mittel nicht ausreichend erfüllen. Ein späterer Entwurf muss zu den vorgesehenen Bind-/Insert-Arbeitsabläufen passen und die Argumentauswertung bei abgeschaltetem Logging verhindern.

Das im Draft vorgeschlagene `trice insert -loglevel` ist **nicht** der vorhandene Hostschalter `-logLevel`. Großschreibung und Kommandokontext sind relevant. Der Vorschlag darf nicht als benutzbares Kommando erscheinen.

#### <a id="ll-deferred-runtime"></a>Dynamische Level-Steuerung auf dem Target

Ein dynamischer Schwellwert könnte bei hohen Ereignisraten, begrenzter Übertragungsbandbreite, kleinem Offline-Speicher oder teuren Logargumenten nützlich werden. Dafür wären Aktualisierung, Nebenläufigkeit und genauer Entscheidungszeitpunkt festzulegen. Eine lokale Anwendungssteuerung kann ausreichen; Fernsteuerung kann einen vorhandenen oder neuen Kommandopfad benötigen. [S1]

**Entscheidung dieser Bewertung: Ebenfalls bis zu einem konkreten Bedarf zurückstellen.** Der geringe Aufwand je Ereignis ist ein gutes Argument für das bestehende Design, beweist aber keine geringen Gesamtkosten für jeden Anwendungsfall. Das vorhandene, einkompilierte Routing ist kein dynamischer früher Filter.

Keine der beiden Target-Level-Erweiterungen ist Voraussetzung für Kontextanreicherung oder strukturierte Felder auf dem Host. Zusätzliche Levelbytes pro Ereignis werden nicht vorausgesetzt.

### <a id="ll-related-work"></a>Beziehung zu Enrichment, strukturiertem Logging und Servern

Die geprüfte UM-Zuordnung lautet: 45.1 Level-Control-Draft; 45.2 der überwiegend auf Enrichment gerichtete Draft; 45.3 interne Parserentwicklung; 45.4 Servernutzung. Bei einer Verschiebung sollten stabile Anker verwendet werden. Das Enrichment-Kapitel sollte umbenannt und die Parseraufgabe in ein eigenes Kapitel beziehungsweise Arbeitspaket ausgelagert werden. [S1]

Eine mögliche `strice`-Erweiterung kann eine Ereignis-ID für mehrere benannte Felder behalten. Der Host kann ein Feld über verschiedene IDs und Argumentpositionen auflösen, optional eine numerische Host-Feld-ID vergeben und daraus Text oder strukturierte Ausgabe erzeugen. Eine Beschränkung auf einen Key pro Targetnachricht ist nicht notwendig und kann die sinnvolle Zusammenfassung von Kontext erschweren. Dies bleibt ein Vorschlag, keine implementierte öffentliche Schnittstelle.

Bei Servern ist zwischen dem Auswerten von Geräte-Trice-Strömen und dem Ersetzen eines nativen Serverloggers zu unterscheiden. Ersteres passt zum effizienten Target-Design; Letzteres benötigt eigene Bedarfsnachweise und Messungen. Tokenisierung beweist weder geringeren Gesamtenergiebedarf noch den Wegfall jeder zusätzlichen Kompression. Zuerst ist ein praktischer Arbeitsablauf zu validieren, danach eine detaillierte Erweiterungsspezifikation zu erstellen.

### <a id="ll-summary"></a>Empfohlene Reihenfolge

Target-Kern und Datenformat beibehalten. Zuerst mehrdeutige Aliase, CLI-Validierung, Benutzerlabels, Routinggrenzen und Routingaktivierung korrigieren. Vorhandene Packing-Prüfungen und Rohaufzeichnung durch Tests absichern. Vor früherer Filterung die Hostsemantik für Schweregrade, Metadaten, Zeilenzusammensetzung, Diagnosen und Statistiken klären. Diese Übersicht mit den technischen Detailkapiteln verbinden. Feinere Target-Auswahl erst bei einem tatsächlichen Bedarf erneut aufgreifen.

### <a id="ll-references"></a>Implementierungs- und Begriffsreferenzen

Die Quellenlinks zeigen auf den geprüften Branch, nicht auf einen unveränderlichen Commit. Vor Implementierungsarbeiten erneut prüfen und den tatsächlichen Commit festhalten.

[S1]: https://github.com/rokath/trice/blob/wip/docs/TriceUserManual.md
[S2]: https://github.com/rokath/trice/blob/wip/src/trice.c
[S3]: https://github.com/rokath/trice/blob/wip/internal/emitter/lineTransformerANSI.go
[S4]: https://github.com/rokath/trice/blob/wip/internal/translator/translator.go
[S5]: https://github.com/rokath/trice/blob/wip/src/triceDefaultConfig.h
[S6]: https://github.com/rokath/trice/blob/wip/internal/args/init.go
[S7]: https://github.com/rokath/trice/blob/wip/internal/args/handler.go
[S8]: https://github.com/rokath/trice/blob/wip/internal/receiver/receiver.go
[S9]: https://github.com/rokath/trice/blob/wip/internal/id/switchIDs.go
[S10]: https://opentelemetry.io/docs/specs/otel/logs/data-model/
[S11]: https://github.com/rokath/trice/blob/wip/internal/decoder/decoder.go
[S12]: https://github.com/rokath/trice/blob/wip/internal/emitter/emitter.go
[S13]: https://github.com/rokath/trice/blob/wip/internal/id/insertIDs.go
[S14]: https://github.com/rokath/trice/blob/wip/src/triceRingBuffer.c
