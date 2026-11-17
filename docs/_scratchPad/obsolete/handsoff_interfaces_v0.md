# handsoff_interfaces_v0 — Trice / tlog Schnittstellen und Migration

**Stand:** 17. November 2026 · **Status:** konsolidiertes Nachdenk- und Designdokument **v0**, kein Implementierungsauftrag.  
**Repository:** <https://github.com/rokath/trice> · **ausgewerteter Dokumentationsbranch:** `wip`, HEAD `f7b819207d7e435159890227b7a5fd360d3ec727` (17.11.2026).  
**Ist-Angaben:** geprüft gegen `wip`-CLI-Hilfe und Quellcode; **Zukunft-Angaben sind Syntaxvorschläge**, keine existierenden Optionen.  
**Bezug:** die beigefügte `handsoff_tlog_schnittstellendesign(1).md` und sämtliche fachlich relevanten Handsoffs unter `wip/docs/_scratchPad/`. Die übrigen dortigen Unterlagen werden unten ausdrücklich eingeordnet.

## 0. Leseanleitung

Die folgende Fallliste ist **anwendungsfallorientiert**: Jede Minitabelle zeigt unmittelbar unter **Jetzt** eine oder mehrere denkbare **Zukunft**-Zeilen. Gleiche Syntax in beiden Zeilen bedeutet **unverändert lassen**. Leeres **Jetzt** bedeutet: **der konkrete neue Workflow ist noch nicht implementiert**. Ein Beispiel mit mehreren Befehlen durch Semikolon/Kommentare ist **auf getrennte Prozesse/Terminals aufzuteilen** und keine fertig ausführbare Einzeile. Alle künftigen Adressen, Namen von Bridge-Programmen und neue Flags sind Arbeitssyntax, nicht freigegebenes CLI.

Die Beispiele beanspruchen **nicht**, jede mögliche Kombination aller Schalter abzubilden; aufgeführt sind die relevanten *Anwendungsfälle* und die aktuellen `tlog`-Optionen einschließlich weniger sichtbarer Diagnose-, Alias-, Zeit-, DS- und Framing-Fälle. `tlog` entspricht dem Logmodus `trice log`; `trice ds`, `trice shutdown` und `trice generate` sind **andere** Trice-Subcommands und werden nur eingebunden, wenn sie für das Schnittstellendesign notwendig sind.

## 1. Verbindlicher Architekturvertrag

1. **Ein Prozess, genau eine Datenquelle / ein `-port`.** Keine wiederholten `-port`-Flags, kein `-port DS`, keine Mehr-Target-Dekodierung in `tlog`. `-p`, `-port`, `-args`, UART-/RTT-Parameter behalten im neuen Modell zunächst ihren Sinn.
2. **`-out` darf beliebig oft vorkommen.** Eine Output-Spezifikation bestimmt Sink, Serialisierung und ggf. numerische/signalbezogene Transformation. **Keine eigenen allgemeinen Log-Level-, `-pick`- oder `-ban`-Filter pro `-out`.**
3. **Alle nicht-raw-Ausgaben teilen genau eine Host-Filterentscheidung.** Die bestehenden globalen `-logLevel`, `-pick`, `-ban` und Tagdefinitionen gelten für alle dekodierten Ausgaben derselben Instanz. Formatgebundene Auswahl (nur numerische Structured Fields für VCD/CSV) ist eine Transformation, nicht eine abweichende allgemeine Log-Policy.
4. **Alle `format=raw`-Ausgaben erhalten die vor der Dekodierung abgegriffenen Bytes, ohne Hostfilter.** Enthaltenes COBS/TCOBS-Framing und typischerweise vorhandene Cycle Counter bleiben exakt erhalten. Bereits auf dem Target verworfene Ereignisse kann auch raw nicht rekonstruieren.
5. **Kein `-out` darf Empfang oder andere Ausgänge aufhalten.** Pro Sink, bei mehreren TCP-Clients pro Client, eine **begrenzte Queue** plus unabhängiger Writer. **Ist die Queue voll, wird der gesamte noch wartende Inhalt verworfen** (`clear-queue`); neue Daten werden angenommen. Keine blockierende Alternative und keine vollständige Raw-Aufzeichnungsgarantie.
6. **Mehrfaches `-ctrl` bezieht sich ausnahmslos auf dasselbe eine Target** der `-port`-Verbindung. Auch bei parallelen Clients gibt es nur einen serialisierten Target-TX-Writer. `-ctrl` ist keine Steueroberfläche für den DS oder für einen anderen Logger.
7. **Mehrere Targets sind mehrere `tlog`-Prozesse** mit eigener TIL, Framing, Dekodierung, Filterung und CLI. Der vorhandene `trice ds` sammelt deren **fertig dekodierte/formatierte Zeilen**; er dekodiert keine Trice-Binärdaten und hat **keine eigenen Trice-Level-/Tagfilter**.
8. **Bestehende CLI ist zuerst weiterhin gültig.** Neuer Code parallel zu altem, kein sofortiges Deprecation und keine stillen Verhaltensänderungen; getrennte, überprüfbare Migrationsphasen.

### 1.1 Datenwege

```text
                    genau ein Target
                          ^  |      z.B. UART / RTT / TCP
                          |  v
                 -port / -args / Reader
                    |          ^
             raw input       TX, ein Writer <--- -ctrl stdin:
                    |          ^             -ctrl tcp-listen://... (mehrfach)
       +------------+---------+
       |
       +---- RAW TAP (vor Decoder und Hostfilter)
       |        +--> bounded Queue -> -out raw:file
       |        +--> bounded Queue -> -out raw:TCP -> weitere tlog
       |
       +---- Decoder / TIL / LI / typisierte Records
                       |
                  EINE globale Logauswahl
                       |
             +---------+---------+-------------+
             |         |         |             |
          Queue      Queue     Queue         Queue   (alle begrenzt)
          text       JSON      vis/VCD         DS
             |         |         |             |
          stdout     Datei    LabPlot/GTK     trice ds

  Andere Targets: eigene tlog-Prozesse -> gemeinsame DS-Anzeige.
  Alternative Filter: weitere tlog-Prozesse lesen denselben raw-Stream.
```

**Nicht verwechseln:** Eine `-out`-Queue ist ein Host-Transportpuffer, kein Target-Ringpuffer. Eine Datei als Raw-Sink kann Lücken haben; die Binärdatei stellt keine verlustfreie Aufzeichnung sicher. `-ctrl`-Befehle können dem Target absichtlich Daten schreiben, dürfen aber nicht mehrere Writer gleichzeitig auf dieselbe Verbindung loslassen.

### 1.2 Vorläufige Grammatik, bewusst unverbindlich

```text
-out 'sink=<URI>;format=<raw|text|json|kv|vis|vcd>;...'
-ctrl 'stdin:'
-ctrl 'tcp-listen://127.0.0.1:9002'
trice ds -listen ds://127.0.0.1:61497
trice shutdown -connect ds://127.0.0.1:61497
```

Die tatsächliche CLI könnte auch `-out vcd:datei.vcd` erlauben; die Minibeispiele verwenden beide **historisch diskutierten** Schreibweisen. Beide können erst nach Festlegung von Parsing, Quoting, gleichzeitigen Zielen, Dateisemantik und `stdout` umgesetzt werden. Client/Server-Rolle explizit angeben (`tcp-listen://` vs `tcp-connect://`); Netzwerk-Listener standardmäßig **Loopback**. Kein zusätzlicher Spezialschalter je Viewer.

## 2. Alle relevanten Anwendungsfälle: Jetzt direkt über Zukunft

### A. Ein Target und dessen Eingangsweg

#### 01. UART unter Windows

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -baud 115200` |
| **Zukunft** | `tlog -p COM7 -baud 115200` |

Der eine -port bleibt bestehen; keine Mehrfach-Quelle in einer Instanz.

#### 02. UART unter Linux

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p /dev/ttyUSB0 -baud 115200` |
| **Zukunft** | `tlog -p /dev/ttyUSB0 -baud 115200` |

Serielle Gerätenamen bleiben gültig; Schnittstellenentwurf ist betriebssystemneutral.

#### 03. Serielle Leitung mit anderem Baudwert

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -baud 230400` |
| **Zukunft** | `tlog -p COM7 -baud 230400` |

Bewährte Port-Parameter nicht ohne Not umbenennen.

#### 04. Serielle Rahmeneinstellungen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -baud 115200 -databits 8 -parity even -stopbits 1` |
| **Zukunft** | `tlog -p COM7 -baud 115200 -databits 8 -parity even -stopbits 1` |

Line-Settings sind Parameter der Target-Verbindung, nicht von -out.

#### 05. SEGGER J-Link RTT

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p J-LINK` |
| **Zukunft** | `tlog -p J-LINK` |

Zusätzliche J-Link-Parameter werden weiterhin über -args übergeben.

#### 06. J-Link mit spezifischer MCU

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p J-LINK -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0"` |
| **Zukunft** | `tlog -p J-LINK -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0"` |

Komplexe -args-Strings funktionieren weiterhin; keine Pflicht, sie in -port umzuziehen.

#### 07. ST-Link RTT

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p ST-LINK` |
| **Zukunft** | `tlog -p ST-LINK` |

Bestehender Reader bleibt unverändert.

#### 08. OpenOCD als Quelle

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p OPENOCD` |
| **Zukunft** | `tlog -p OPENOCD` |

Im Receiver-Code als OPENOCD/OPEN-OCD/OOCD/OCD vorhanden; der konkrete OpenOCD-Adapter hat eigene Voraussetzungen.

#### 09. Binäre Logs per TCP empfangen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p TCP4 -args 127.0.0.1:17001` |
| **Zukunft** | `tlog -p TCP4 -args 127.0.0.1:17001` |

Dieser TCP4-Port ist ein Dial-Client; nicht mit einem neuen -out TCP-Listener verwechseln.

#### 10. Binäre Logs per UDP empfangen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p UDP4 -args 0.0.0.0:17005` |
| **Zukunft** | `tlog -p UDP4 -args 0.0.0.0:17005` |

Aktuell UDP-Listener für empfangene Trice-Bytes; keine Rückadresse für Target-Steuerkommandos.

#### 11. Fortlaufendes Nachlesen einer Binärdatei

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p FILE -args trace.bin` |
| **Zukunft** | `tlog -p FILE -args trace.bin` |

FILE liest anhängende Bytes weiter, auch wenn der Writer seine Datei noch nicht geschlossen hat.

#### 12. Endliche Binärdatei wiedergeben

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p FILEBUFFER -args trace.bin` |
| **Zukunft** | `tlog -p FILEBUFFER -args trace.bin` |

FILEBUFFER endet nach EOF; geeignet für deterministische Tests.

#### 13. Hexadezimale Testbytes

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p HEX -args "48 65 6c 6c 6f 0a" -encoding CHAR` |
| **Zukunft** | `tlog -p HEX -args "48 65 6c 6c 6f 0a" -encoding CHAR` |

DUMP ist ein Alias für diesen Eingabetyp; der Inhalt sind bereits Bytes, keine Trice-IDs.

#### 14. Dezimale Testbytes

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p DEC -args "72 101 108 108 111 10" -encoding CHAR` |
| **Zukunft** | `tlog -p DEC -args "72 101 108 108 111 10" -encoding CHAR` |

BUFFER ist ein alternativer Name der dezimalen Bytequelle.

#### 15. Endlicher TCP-Testmodus

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p TCP4BUFFER -args 127.0.0.1:17001` |
| **Zukunft** | `tlog -p TCP4BUFFER -args 127.0.0.1:17001` |

Bestehender, derzeit primär testinterner Modus; muss nicht als offizieller öffentlicher Workflow beworben werden.

### B. Decoder, Protokoll, Tabellen

#### 16. TREX dekodieren

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -encoding TREX` |
| **Zukunft** | `tlog -p COM7 -encoding TREX` |

TREX ist voreingestellt; -out arbeitet erst nach den jeweiligen Decoder-Schritten.

#### 17. CHAR-Stream betrachten

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -encoding CHAR` |
| **Zukunft** | `tlog -p COM7 -encoding CHAR` |

Bei CHAR keine erfundene Trice-Event-Struktur für JSON/VCD annehmen.

#### 18. Hexdump statt Trice-Dekodierung

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -encoding DUMP -dc 16` |
| **Zukunft** | `tlog -p COM7 -encoding DUMP -dc 16` |

DUMP ist eine Decoder-/Monitoransicht; Rohdaten-Ausgang greift unabhängig davon vor der Dekodierung ab.

#### 19. TCOBS-Frames

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -pf TCOBSv1` |
| **Zukunft** | `tlog -p COM7 -pf TCOBSv1` |

Framing muss zum Target passen; bestehende Abkürzung -pf bleibt.

#### 20. COBS-Frames

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -pf COBS` |
| **Zukunft** | `tlog -p COM7 -pf COBS` |

Rohdaten bleiben mit ihren vorhandenen COBS-Frame-Delimitern erhalten.

#### 21. Ohne Paketframing

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -pf none` |
| **Zukunft** | `tlog -p COM7 -pf none` |

Dieser Sonderfall ist bei Raw-Queue-Verlust schwer resynchronisierbar; nicht unbemerkt auf COBS/TCOBS umstellen.

#### 22. Prüfung auf einzelnes Record pro Frame

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -singleFraming` |
| **Zukunft** | `tlog -p COM7 -singleFraming` |

Validierungsoption bleibt; nur verwenden, wenn das Target-Frame-Modell dazu passt.

#### 23. Doppelte 16-Bit-IDs

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -d16` |
| **Zukunft** | `tlog -p COM7 -d16` |

Dekoder-Konfiguration bleibt pro Target.

#### 24. Cycle-Counter-Fehler unterdrücken

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -noCycleCheck` |
| **Zukunft** | `tlog -p COM7 -noCycleCheck` |

Default bleibt die Fehlererkennung; das Flag deaktiviert den entsprechenden Bericht.

#### 25. Big-Endian-Target

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -triceEndianness bigEndian` |
| **Zukunft** | `tlog -p COM7 -triceEndianness bigEndian` |

Byteordnung betrifft Eingangsdekodierung; nicht einzelne Ausgänge.

#### 26. Default-Trice-Parameterbreite

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -defaultTRICEBitwidth 16` |
| **Zukunft** | `tlog -p COM7 -defaultTRICEBitwidth 16` |

Mit der Target-Konfiguration synchron halten.

#### 27. Verschlüsselte Trices dekodieren

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -password MySecret` |
| **Zukunft** | `tlog -p COM7 -password MySecret` |

Passwort bzw. Schlüssel gehören zum Decoder, nicht zu -out. Reelle Geheimnisse nicht in Shell-History ablegen.

#### 28. Schlüssel anzeigen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -password MySecret -showKey` |
| **Zukunft** | `tlog -p COM7 -password MySecret -showKey` |

Bestehende Diagnose-/Entwicklerfunktion; neue -out-Option darf keine Schlüssel im Eventstrom verbreiten.

#### 29. Trice-ID-Tabelle

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -til til.json` |
| **Zukunft** | `tlog -p COM7 -til til.json` |

-i und -idlist sind vorhandene Alternativnamen.

#### 30. Source-Location-Tabelle

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -li li.json` |
| **Zukunft** | `tlog -p COM7 -li li.json` |

Standortinformationen bleiben vom allgemeinen Ausgaberouting getrennt.

#### 31. Location-Daten deaktivieren

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -li off` |
| **Zukunft** | `tlog -p COM7 -li off` |

Möglich ohne Aenderung der TIL-Dekodierung.

#### 32. Automatisches Neuladen der TIL/LI

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -til til.json -li li.json` |
| **Zukunft** | `tlog -p COM7 -til til.json -li li.json` |

Datei-Aenderungen werden im laufenden Logger bereits verfolgt; VCD ist Sonderfall wegen unveränderlichem Header-Schema.

#### 33. Macro-Alias definieren

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -alias MyTrace:info` |
| **Zukunft** | `tlog -p COM7 -alias MyTrace:info` |

Bestehende Alias-Optionen auch für künftiges typed Event-Modell berücksichtigen.

#### 34. String-Macro-Alias definieren

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -salias MyText:info` |
| **Zukunft** | `tlog -p COM7 -salias MyText:info` |

Abweichende Typ-Semantik für triceS() respektieren.

#### 35. Eigene Taggruppe registrieren

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -utag motor:450` |
| **Zukunft** | `tlog -p COM7 -utag motor:450` |

Wird global vor Filtern und Visualisierung aufgelöst; keine eigene Tagregel pro -out.

#### 36. typeX0-Benutzerpakete

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -typeX0 ignore` |
| **Zukunft** | `tlog -p COM7 -typeX0 ignore` |

Weitere vorhandene Werte wie counted:<format> bleiben; bei Rohdaten werden Pakete nicht vorher entfernt.

### C. Eine gemeinsame Logauswahl und Formatierung

#### 37. Alle Logereignisse anzeigen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -logLevel all` |
| **Zukunft** | `tlog -p COM7 -logLevel all -out "sink=stdout;format=text"` |

Globale Auswahl gilt für alle nicht-raw Ausgänge. Bei Standard-stdout bleibt der alte Aufruf möglich.

#### 38. Alle dekodierten Logereignisse ausblenden

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -logLevel off -binaryLogfile trace.bin` |
| **Zukunft** | `tlog -p COM7 -logLevel off -out "sink=file:trace.bin;format=raw"` |

Raw-Daten werden trotzdem aufgezeichnet; Filter wirken nur auf dekodierte Ausgaben.

#### 39. Prioritätsschwelle setzen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -logLevel wrn` |
| **Zukunft** | `tlog -p COM7 -logLevel wrn -out "sink=stdout;format=text"` |

Priorität global. Ein gleichzeitig aktiver JSON-Ausgang erhält ebenfalls nur diese Events.

#### 40. Tag-Auswahl durch -pick

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -pick err -pick wrn` |
| **Zukunft** | `tlog -p COM7 -pick err -pick wrn -out "sink=stdout;format=text"` |

Mehrfaches -pick ist bereits erlaubt; nicht zusammen mit -ban verwenden.

#### 41. Tags durch -ban ausblenden

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -ban dbg` |
| **Zukunft** | `tlog -p COM7 -ban dbg -out "sink=stdout;format=text"` |

Ein gemeinsamer Ausschluss für alle dekodierten -out.

#### 42. Schwelle mit zusätzlichem Pick

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -logLevel wrn -pick dbg` |
| **Zukunft** | `tlog -p COM7 -logLevel wrn -pick dbg -out "sink=stdout;format=text"` |

Bestehende Semantik nicht durch die neue CLI ändern.

#### 43. Text im Terminal

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7` |
| **Zukunft** | `tlog -p COM7 -out "sink=stdout;format=text"` |

Kein -out bedeutet in der Migrationsphase exakt das bisherige Terminalverhalten.

#### 44. Maschinenlesbares NDJSON

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -logFormat json` |
| **Zukunft** | `tlog -p COM7 -out "sink=stdout;format=json"` |

Eine JSON-Zeile pro Event; Diagnosen separat halten. Nur für passende TREX-Dekodierung.

#### 45. Key-Value-Ausgabe

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -logFormat kv` |
| **Zukunft** | `tlog -p COM7 -out "sink=stdout;format=kv"` |

Nicht den formatierten Terminaltext erneut parsen.

#### 46. Hostzeitstempel anpassen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -hs UTCmicro` |
| **Zukunft** | `tlog -p COM7 -hs UTCmicro -out "sink=stdout;format=text"` |

Hostzeit ist Empfangszeit, nicht Targetzeit. -hs off/zero etc. bleiben vorhanden.

#### 47. Target-Timestamp allgemein

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -ts ms` |
| **Zukunft** | `tlog -p COM7 -ts ms -out "sink=stdout;format=text"` |

Zeitauflösung wird pro Target bestimmt, nicht pro Viewer.

#### 48. 16-Bit-Zeitstempel

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -ts16 ms` |
| **Zukunft** | `tlog -p COM7 -ts16 ms -out "sink=stdout;format=text"` |

Wraparound und Skalierung weiterhin aus der Decoder-/Timestamp-Konfiguration.

#### 49. 32-Bit-Zeitstempel

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -ts32 us` |
| **Zukunft** | `tlog -p COM7 -ts32 us -out "sink=stdout;format=text"` |

Vorhandene optionale ts32-Formate unbeschädigt lassen.

#### 50. 16-Bit-Delta

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -ts16delta us` |
| **Zukunft** | `tlog -p COM7 -ts16delta us -out "sink=stdout;format=text"` |

Deltaberechnung bleibt eine Darstellungsoption.

#### 51. 32-Bit-Delta

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -ts32delta us` |
| **Zukunft** | `tlog -p COM7 -ts32delta us -out "sink=stdout;format=text"` |

Für VCD die unabhängige Tickfrequenz-/Counterfrage beachten.

#### 52. Zeitspalte ohne Targetstamp

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -ts0 "" -ts0delta ""` |
| **Zukunft** | `tlog -p COM7 -ts0 "" -ts0delta "" -out "sink=stdout;format=text"` |

Keine erfundene Targetzeit bei ungestempelten Records.

#### 53. Timestamp-Arithmetik

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -ts32 "printf("t:%d",ts/48)"` |
| **Zukunft** | `tlog -p COM7 -ts32 "printf("t:%d",ts/48)" -out "sink=stdout;format=text"` |

Stamp-Expressions stehen für Text bereit; die VCD-Zeitlogik muss konsistente Counterbasis haben.

#### 54. Präfix

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -prefix "source: "` |
| **Zukunft** | `tlog -p COM7 -prefix "source: " -out "sink=stdout;format=text"` |

Bei DS-Aggregation zur Unterscheidung der Targets nützlich.

#### 55. Suffix

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -suffix ";"` |
| **Zukunft** | `tlog -p COM7 -suffix ";" -out "sink=stdout;format=text"` |

Textdarstellung, nicht Rohdaten manipulieren.

#### 56. Farben

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -color light` |
| **Zukunft** | `tlog -p COM7 -color light -out "sink=stdout;format=text"` |

ANSI-Sequenzen dürfen nicht in JSON/VCD/Raw geraten.

#### 57. Quellort anzeigen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -li li.json -liFmt "file:%s:%d "` |
| **Zukunft** | `tlog -p COM7 -li li.json -liFmt "file:%s:%d " -out "sink=stdout;format=text"` |

Location-Formatformat an die bestehende printf-Syntax anpassen; Beispiel ist schematisch.

#### 58. Pfadtiefe für Location begrenzen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -li li.json -liMaxDirs 2` |
| **Zukunft** | `tlog -p COM7 -li li.json -liMaxDirs 2 -out "sink=stdout;format=text"` |

Beeinflusst nur die Darstellung/Statistik, nicht die TIL.

#### 59. Trice-ID anzeigen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -showID "id:%d "` |
| **Zukunft** | `tlog -p COM7 -showID "id:%d " -out "sink=stdout;format=text"` |

Zeilenausgabe kann mehrere Records zusammensetzen; ID bezieht sich auf den ersten.

#### 60. Newline automatisch ergänzen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -addNL` |
| **Zukunft** | `tlog -p COM7 -addNL -out "sink=stdout;format=text"` |

Legacy-Zeilenkomposition weiter testen.

#### 61. Mehrzeilige Ausgabe einrücken

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -newlineIndent 8` |
| **Zukunft** | `tlog -p COM7 -newlineIndent 8 -out "sink=stdout;format=text"` |

Textformatter-Eigenschaft, keine Filtereigenschaft.

#### 62. Unsigned-Semantik

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -unsigned=false` |
| **Zukunft** | `tlog -p COM7 -unsigned=false -out "sink=stdout;format=text"` |

Vorzeichenformate nicht unbemerkt ändern.

#### 63. Trice-Statistiken

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -triceStat` |
| **Zukunft** | `tlog -p COM7 -triceStat` |

Zählt erfolgreich dekodierte ID-basierte Events auch vor Host-Filter.

#### 64. Tag-Statistiken

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -tagStat` |
| **Zukunft** | `tlog -p COM7 -tagStat` |

Separate Statistik über effektiv erkannte Tags.

#### 65. Kombinierte Statistik

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -stat` |
| **Zukunft** | `tlog -p COM7 -stat` |

Bleibt Diagnosefunktion. Späterer Drop-Report muss zusätzlich pro -out anzeigen.

#### 66. Decoder-Debug

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -debug` |
| **Zukunft** | `tlog -p COM7 -debug` |

Zeigt interne TREX-/Deframing-/Decryption-Schritte. Nicht vergessen; Diagnose nicht in JSON/Pipes mischen.

#### 67. Eingangsbytes anzeigen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -showInputBytes` |
| **Zukunft** | `tlog -p COM7 -showInputBytes` |

Byte-Chunks zeigen; keine zweite Raw-Ausgabe mit garantierten Framegrenzen.

#### 68. Verbose-Betrieb

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -v` |
| **Zukunft** | `tlog -p COM7 -v` |

Zusätzliche Betriebsdiagnose; anders als der Decoder-Schalter -debug.

#### 69. Test-Tabellenformat

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -testTable` |
| **Zukunft** | `tlog -p COM7 -testTable` |

Bestehender Sondermodus; keine Annahme, dass JSON/DS gleichzeitig kompatibel sind.

### D. Bestehende Ausgaben, Aufzeichnung, Visualisierung

#### 70. Text in eine Datei schreiben

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -logfile trace.log` |
| **Zukunft** | `tlog -p COM7 -out "sink=file:trace.log;format=text;mode=append"` |

Legacy-Dateilog appendet. Neuer Sink darf vorhandene Dateisemantik nicht versehentlich ändern.

#### 71. Textdatei mit Zeitnamen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -logfile auto` |
| **Zukunft** | `tlog -p COM7 -out "sink=file:auto;format=text;mode=append"` |

Auto-Namen sind eine separate Dateinamens-Policy und bedürfen Tests.

#### 72. Rohdaten an eine Binärdatei anhängen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -binaryLogfile trace.bin` |
| **Zukunft (Variante 1)** | `tlog -p COM7 -out "sink=file:trace.bin;format=raw;mode=append"` |
| **Zukunft (Variante 2)** | `tlog -p COM7 -out raw:trace.bin` |

Raw-Ausgabe vor Decoder und Filter; auch mit -logLevel off. Nonblocking bedeutet: Datei kann bei Überlast Lücken haben.

#### 73. Rohdaten nach automatisch benannter Datei

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -blf auto` |
| **Zukunft** | `tlog -p COM7 -out "sink=file:auto;format=raw;mode=append"` |

Die bestehende -blf-Alias-Schreibweise bleibt während der Parallelmigration.

#### 74. Textausgabe über einen TCP-Listener

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -tcp 127.0.0.1:64000` |
| **Zukunft (Variante 1)** | `tlog -p COM7 -out "sink=tcp-listen://127.0.0.1:64000;format=text"` |
| **Zukunft (Variante 2)** | `tlog -p COM7 -out 'tcp-listen://127.0.0.1:64000;format=text'` |

Heute einfacher Textstream mit nur einem angenommenen Client; neuer Listener soll mehrere Clients über getrennte Queues erlauben.

#### 75. Numerische CSV-Datei via -vis

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -utag motor -vis='motor:printf("%d,%d\n",v0,v1)@motor.csv'` |
| **Zukunft** | `tlog -p COM7 -utag motor -out 'sink=file:motor.csv;format=vis;tag=motor;expr=printf("%d,%d\n",v0,v1)'` |

Schematische spätere Syntax. Transformations-spezifische Tag-Auswahl ist nicht ein unabhängiger allgemeiner -out-Filter.

#### 76. Live-CSV via UDP an LabPlot

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -utag motor -vis='motor:printf("%d\n",v0)@udp://127.0.0.1:9000'` |
| **Zukunft** | `tlog -p COM7 -utag motor -out 'sink=udp://127.0.0.1:9000;format=vis;tag=motor;expr=printf("%d\n",v0)'` |

LabPlot erhält pro Messpunkt ein Datagramm; UDP ist best effort.

#### 77. Mehrere numerische -vis-Ausgaben

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -vis='motor:printf("%d\n",v0)@motor.csv' -vis='motor:printf("%d\n",v0)@udp://127.0.0.1:9000'` |
| **Zukunft** | `tlog -p COM7 -out 'sink=file:motor.csv;format=vis;tag=motor;expr=printf("%d\n",v0)' -out 'sink=udp://127.0.0.1:9000;format=vis;tag=motor;expr=printf("%d\n",v0)'` |

Wiederholbarkeit existiert heute für -vis und soll später allgemein für -out gelten.

#### 78. Werte umrechnen und CSV-Header schreiben

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -utag motor -vis='motor:printf("%d,%.3f\n",v0,v1/1000)@motor.csv;header="rpm,A\n"'` |
| **Zukunft** | `tlog -p COM7 -utag motor -out 'sink=file:motor.csv;format=vis;tag=motor;expr=printf("%d,%.3f\n",v0,v1/1000);header="rpm,A\n"'` |

Werte v0..v11, ts, ts16/32 und Formelparser heute vorhanden; Grammatik/Quoting später testen.

#### 79. Legacy -vis mit log=drop

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -vis='motor:printf("%d\n",v0)@motor.csv;log=drop'` |
| **Zukunft** | `tlog -p COM7 -out 'sink=file:motor.csv;format=vis;tag=motor;expr=printf("%d\n",v0)'` |

Bewusst NICHT semantisch identisch: Neuer -out darf andere Ausgänge nicht unterdrücken. Alte log=drop-Semantik bis zur Migration erhalten.

#### 80. Ungefilterte Aufzeichnung trotz Terminalfilter

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -binaryLogfile all.bin -logLevel wrn` |
| **Zukunft** | `tlog -p COM7 -out "sink=file:all.bin;format=raw" -out "sink=stdout;format=text" -logLevel wrn` |

Das ist der zentrale Beleg für den raw-Tap vor der globalen Host-Auswahl.

#### 81. Filter und JSON gleichzeitig (gleiche Auswahl)

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -logLevel wrn -logFormat json` |
| **Zukunft** | `tlog -p COM7 -logLevel wrn -out "sink=stdout;format=text" -out "sink=file:events.jsonl;format=json"` |

Beide dekodierten Ausgänge zeigen dieselbe globale Auswahl. Wenn unterschiedliche gewünscht, zweiten tlog auf raw starten.

### E. Displayserver, Aggregation und Steuerung heute

#### 82. Lokalen DS starten

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `trice ds` |
| **Zukunft** | `trice ds -listen ds://127.0.0.1:61497` |

DS hat kein Target--port; ein separater Server für formatierte Logzeilen. -listen ist Zukunfts-Syntax.

#### 83. Lokalen DS mit festem Endpoint

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `trice ds -ipa 127.0.0.1 -ipp 61497` |
| **Zukunft (Variante 1)** | `trice ds -listen ds://127.0.0.1:61497` |
| **Zukunft (Variante 2)** | `trice ds -listen tcp://127.0.0.1:61497 -protocol ds` |

Ersetzt getrennte IP/Port-Flags durch eine Endpoint-Angabe.

#### 84. Eine tlog-Instanz an DS senden

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -ds` |
| **Zukunft** | `tlog -p COM7 -out "sink=ds://127.0.0.1:61497;format=text"` |

Heute -ds ist ein Remote-Display-Client, kein eigener DS-Filter.

#### 85. Remote-DS adressieren

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -ds -ipa 192.168.1.20 -ipp 61497` |
| **Zukunft** | `tlog -p COM7 -out "sink=ds://192.168.1.20:61497;format=text"` |

DS-Endpunkt muss erreichbar sein; Sicherheitsfreigabe für fremde Netze gesondert prüfen.

#### 86. Zwei Targets zusammen in einem DS anzeigen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `trice ds  # eigener Prozess; tlog -p COM3 -ds  # Terminal A; tlog -p COM4 -ds  # Terminal B` |
| **Zukunft** | `trice ds -listen ds://127.0.0.1:61497  # eigener Prozess; tlog -p COM3 -out "sink=ds://127.0.0.1:61497"; tlog -p COM4 -out "sink=ds://127.0.0.1:61497"` |

Genau ein Port pro tlog; DS summiert fertig dekodierte/formatierte Zeilen, nicht mehrere Target-Decoder in einem Prozess.

#### 87. DS mit eigener Farbpallette

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `trice ds -color dark` |
| **Zukunft** | `trice ds -color dark -listen ds://127.0.0.1:61497` |

Der DS besitzt eine Palette, aber keinen Tag-/Prioritätsfilter für Trice-Events.

#### 88. DS-Ausgabe zusätzlich protokollieren

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `trice ds -logfile aggregate.log` |
| **Zukunft** | `trice ds -listen ds://127.0.0.1:61497 -logfile aggregate.log` |

DS-eigene Textaufzeichnung ist von Target-raw-Aufzeichnungen zu trennen.

#### 89. DS gezielt herunterfahren

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `trice shutdown -ipa 127.0.0.1 -ipp 61497` |
| **Zukunft** | `trice shutdown -connect ds://127.0.0.1:61497` |

Prozesssteuerung des DS ist kein -ctrl des Targets.

#### 90. Tastaturkommando zum Target

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `trice ds  # starten; tlog -p COM7 -ds  # Kommando in dessen stdin tippen` |
| **Zukunft (Variante 1)** | `tlog -p COM7 -ctrl stdin: -out "sink=stdout;format=text"` |
| **Zukunft (Variante 2)** | `tlog -p COM7 -ctrl 'source=stdin;protocol=legacy-keyboard'` |

Heutiger Keyboard-Pfad hängt am -ds-Modus des Loggers; neue -ctrl-Eingabe unabhängig von der Loganzeige vorsehen.

#### 91. DS mit vorgefiltertem Logging

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -ds -logLevel wrn  # DS zeigt nur die vom tlog übertragenen Zeilen` |
| **Zukunft** | `tlog -p COM7 -logLevel wrn -out "sink=ds://127.0.0.1:61497;format=text"` |

DS selbst besitzt keine eigene -pick/-ban/-logLevel-Logik; die Filterung geschieht im Logger.

#### 92. Paralleler, heute realer Datei-Fan-out

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p COM7 -binaryLogfile shared.bin  # Writer; tlog -p FILE -args shared.bin -pick err  # Leser B; tlog -p FILE -args shared.bin -pick wrn  # Leser C` |
| **Zukunft** | `tlog -p COM7 -out "sink=file:shared.bin;format=raw"; tlog -p FILE -args shared.bin -pick err; tlog -p FILE -args shared.bin -pick wrn` |

Gleichzeitiges Lesen derselben fortgeschriebenen Datei mit unabhängigen Filtern ist bereits möglich; jeder Leser benötigt passende TIL/Framing/Keys.

#### 93. Offline-Fan-out einer abgeschlossenen Aufnahme

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | `tlog -p FILEBUFFER -args shared.bin -pick err  # A; tlog -p FILEBUFFER -args shared.bin -logLevel wrn  # B; tlog -p FILEBUFFER -args shared.bin -logFormat json  # C` |
| **Zukunft** | `tlog -p FILEBUFFER -args shared.bin -pick err -out "sink=stdout;format=text"; tlog -p FILEBUFFER -args shared.bin -logLevel wrn -out "sink=stdout;format=text"; tlog -p FILEBUFFER -args shared.bin -out "sink=stdout;format=json"` |

Die drei Instanzen starten getrennt; jede hat ihren eigenen Decoder und die eigene globale Filterkonfiguration.

### F. Neue Anwendungsfaelle ohne heutige Spezialoption

#### 94. Raw-TCP-Multiclient-Fan-out

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft (Variante 1)** | `tlog -p COM7 -out "sink=tcp-listen://127.0.0.1:9100;format=raw"; tlog -p TCP4 -args 127.0.0.1:9100 -pick err; tlog -p TCP4 -args 127.0.0.1:9100 -pick wrn` |
| **Zukunft (Variante 2)** | `tlog -p COM7 -out 'raw:tcp-listen://127.0.0.1:9100'` |

Neue Funktion. TCP-Listener mit mehreren Clients, jede Client-Queue getrennt, Daten stets vor Dekodierung.

#### 95. Raw-Datei und Raw-TCP gleichzeitig

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -out "sink=file:trace.bin;format=raw" -out "sink=tcp-listen://127.0.0.1:9100;format=raw"` |

Neue -out-Routing-Variante. Auch wenn -logLevel off gesetzt ist, bleiben beide raw.

#### 96. Gleichzeitige Text-, JSON- und Raw-Ausgabe

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -logLevel info -out "sink=stdout;format=text" -out "sink=file:events.jsonl;format=json" -out "sink=file:trace.bin;format=raw"` |

Text und JSON erhalten dieselbe globale Auswahl, Raw keine Hostfilter.

#### 97. Zwei unabhängige TCP-Steuer-Clients am selben Target

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -ctrl tcp-listen://127.0.0.1:9002` |

Ein einziger Listener kann viele Clients bedienen; alle Requests werden auf genau denselben Target-Schreibweg serialisiert.

#### 98. Zwei verschiedene Steuereingänge für ein Target

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -ctrl stdin: -ctrl tcp-listen://127.0.0.1:9002` |

Mehrfaches -ctrl ist erlaubt; niemals mehrere Target--ports oder konkurrierende Target-Writer.

#### 99. Symbolisches ABC-Injection-Kommando

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -til til.json -ctrl tcp-listen://127.0.0.1:9002` |

Künftiges Requestschema etwa {"op":"inject","request_id":"r1","triceC":"demo:reset"}; IDs aus TIL eindeutig auflösen.

#### 100. DLT Viewer über externe Bridge

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -logLevel all -out "sink=tcp-listen://127.0.0.1:9101;format=json"; trice-dlt-bridge --source tcp://127.0.0.1:9101 --listen tcp://127.0.0.1:3490` |

Bridge/Flags sind schematisch. Kein DLT in tlog oder Firmware; DLT-Control-Filter gelten nur in der Bridge.

#### 101. GTKWave: VCD-Aufnahme aus Binärdatei

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft (Variante 1)** | `trice log -p FILEBUFFER -args capture.bin -til til.json -vcdTickHz 1000000 -out vcd:session.vcd` |
| **Zukunft (Variante 2)** | `trice log -p FILEBUFFER -args capture.bin -til til.json -vcdTickHz 1000000 -out 'sink=file:session.vcd;format=vcd'` |

VCD aus typisierten Structured Fields, eindeutiger Zeitbasis; keine signals.yaml-Pflicht.

#### 102. GTKWave: dauerhafte View erzeugen

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `trice generate -gtkw=generated/session.gtkw -til=til.json` |

Generator liegt ausserhalb von tlog; kein VCD beim Generate und kein automatisches Überschreiben manueller Views.

#### 103. GTKWave: Live-VCD als Datei

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `trice log -p COM7 -til til.json -vcdTickHz 1000000 -out vcd:session.vcd` |

Laufende VCD-Updates; Viewer-Liveverhalten versionsabhängig, insbesondere GTKWave 3 vs 4.

#### 104. GTKWave: VCD in eine Pipe

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `trice log -p FILEBUFFER -args capture.bin -til til.json -vcdTickHz 1000000 -out vcd:- > session.vcd` |

stdout im VCD-Modus exklusiv; Diagnosen nach stderr. Verlorene Events sind als Lücken zu behandeln.

#### 105. Perfetto/Chrome Trace

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -out "sink=file:trace.ndjson;format=json"; trice-perfetto-bridge trace.ndjson > trace.json` |

Konverter externer Adapter; Event-, Counter- und Duration-Semantik erst aus typisierten Metadaten festlegen.

#### 106. Eclipse Trace Compass / CTF

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -out "sink=file:trace.ndjson;format=json"; trice-ctf-bridge trace.ndjson` |

CTF/Trace-Compass-Adapter kann später folgen, keine neue Spezialoption in tlog.

#### 107. Serial Studio Live-Dashboard

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -utag motor -out 'sink=udp://127.0.0.1:9000;format=vis;tag=motor;expr=printf("%d\n",v0)'` |

Nur der Transport und ein vereinbartes numerisches Format sind erforderlich; konkrete Viewer-Konfiguration separat testen.

#### 108. Node-RED als interaktive Target-Oberfläche

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -ctrl tcp-listen://127.0.0.1:9002 -out "sink=tcp-listen://127.0.0.1:9101;format=json"` |

Node-RED kann Status aus JSON lesen und Kommandos senden, ohne selbst UART oder Trice-Framing zu implementieren.

#### 109. Grafana/Loki-Langzeitarchiv

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -out "sink=file:trace.ndjson;format=json"  # Agent/Bridge importiert` |

Externer Agent schreibt Loki-Protokoll; globale Hostfilter bleiben verbindlich.

#### 110. Rerun / numerische Telemetrie

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -out "sink=tcp-listen://127.0.0.1:9101;format=json"  # Rerun-Adapter liest mit` |

Rerun-Typisierung/Entitätsschema im Adapter statt in tlog festlegen.

#### 111. Optionale Raw-UDP-Weitergabe

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -out "sink=udp://127.0.0.1:9100;format=raw"` |

Nur Zukunftskandidat; Datagramme sollten vollständige Frames transportieren, IP-MTU und Verlustsemantik beachten.

#### 112. Raw auf Standardausgabe / Pipe

| Stand | CLI / Prozessfolge |
|:---|:---|
| **Jetzt** | — *noch kein entsprechender allgemeiner Workflow implementiert* |
| **Zukunft** | `tlog -p COM7 -out "sink=stdout;format=raw"` |

Nur mit exklusivem stdout sinnvoll; niemals Textbanner, Debug oder Diagnosen in die Binary-Pipe mischen.

---

## 3. Queue- und Fehlervertrag (aus dem aktualisierten Schnittstellendesign)

### 3.1 Was passiert bei langsamen Empfängern?

| Fall | Gewünschtes Verhalten | Technische Folge |
|---|---|---|
| Kurzzeitige Lastspitze | Eigene Queue puffert; alle anderen Ausgänge laufen weiter | Etwas RAM, geringe zusätzliche Latenz |
| Queue erreicht Limit | **Die komplette noch wartende Queue dieses Sinks/Clients wird verworfen** | Nur dort eine Lücke; `queue_resets`, Drop-Zähler erhöhen |
| Derselbe TCP-Client bleibt langsam | Weitere komplette Queue-Resets; ggf. Verbindung schliessen | Socket-`Write()` braucht Deadline/Close; Queue-Clear kann einen laufenden Write nicht unterbrechen |
| Datei schreibt langsam | Worker schreibt unabhängig; seine Queue kann verworfen werden | Aufnahme muss als unvollständig gelten, auch wenn Dateischreiben später wieder geht |
| Datei voll / I/O-Fehler | Nur diesen Sink deaktivieren und melden | Keine Blockade des Readers oder anderer Sinks |
| UDP-Datagramme gehen unterwegs verloren | Best effort, lokal erkennbare Schreibfehler protokollieren | Empfangsseitige Paketverluste sind i.d.R. nicht verlässlich zählbar |
| `stdout` wird langsam oder Pipe bricht | Worker separieren; bei nicht auflösbarem Write-Problem Sink isolieren | Kein weiterer Mischverkehr in `stdout`; `EPIPE` diagnostizieren |

**Es gibt keinerlei `overflow=block`, `reliability=strict`, `lossless` oder ähnliche Option.** Die MCU darf wegen eines Host-Outputs nie absichtlich angehalten werden. Target-Überlaeufe können unabhängig davon auftreten; dies wird nicht durch einen Host-Sink repariert.

**Puffer-Größe ist noch nicht festgelegt**, am besten Bytebudget pro Sink (für Raw sinnvoll: ganze Frames als Queue-Elemente; für dekodiert: ganze Events) und ein deterministischer Grenzwert in Tests. Ist der Puffer voll, wird nur die *wartende* Queue geleert; im Systemcall bereits begonnene Writes können nicht zurückgenommen werden. `clear` ist hier **discard**, nicht `Flush()` zum Empfänger.

**Raw-Framing:** Trice-Daten sind typischerweise COBS/TCOBS-geframet; Frame-Grenze ist nicht identisch mit einem `Read()`-Chunk. Nach Queue-Clear sollte bei framed Raw ab einer vollständigen nächsten Framegrenze weitergesendet werden. Falls bereits ein halber Frame beim Client war, kann erst der nächste gültige Delimiter die Dekodierung resynchronisieren. Cycle Counter (wenn aktiv) erkennen fehlende Trices; sie ersetzen keine verlorenen Bytes. Bei **ungeframten** Quellen gibt es u.U. keine sichere automatische Re-Synchronisation; diesen Sonderfall getrennt diagnostizieren.

**Diagnosezaehler je Sink/Client, soweit messbar:** `dropped_bytes`, `dropped_frames`, `dropped_events`, `queue_resets`, `write_errors`, `disconnects`, `queue_high_watermark`. Warnungen bei Zustandswechseln bzw. aggregiert ausgeben, nicht ein Log pro verworfenem Byte. Diagnosen auf `stderr` oder gesondertem Kanal, niemals in `raw`/NDJSON/VCD mischen. Zusätzliche Input-Dekoder-Cycle-Fehler sind **nicht identisch** mit Output-Queue-Verlusten.

### 3.2 Fehlerreihenfolge und Shutdown

1. **Vor Verbindungsaufbau** Syntax und Kombinationen prüfen; keine versehentliche Anlage von Ziel-Dateien bei invalidem CLI.
2. **Fehlender/ungültiger Target-Port**: kontrollierter Setup-Fehler, existierende Reconnect-Strategien nicht grundlos ändern.
3. **Ein Sink faellt aus**: Fehler lokal melden, Output isolieren/deaktivieren oder gemaess einem später festgelegten, begrenzten Reconnect-Plan wiederherstellen. Kein Ausfall der anderen Sinks.
4. **TCP-Listener/Client**: bounded Queue pro Client. Bei `Write()`-Hänger Deadline und Close/Disconnect, keine unbegrenzten goroutines oder speichernde Retrys. Keine automatische Replay-Garantie nach Reconnect.
5. **Dateien**: für gewoehnliche Text-/Raw-Dateien Legacy-Append beibehalten, wenn aus Legacy-Flag übersetzt. Neu angelegte VCD-Dateien dagegen mit Header *neu anlegen*, nicht an einen aelteren Header anhängen.
6. **Stop**: Workers geordnet beenden, soweit kurzfristig möglich; keinen unbegrenzten `Wait()` auf blockierten Output. Shutdown-Verluste ggf. zaehlen. Keine gezaehlten Erfolge vortaeuschen, wenn Bytes nur in Queue standen.
7. **`-ctrl`-Fehler**: Request-spezifische Antworten; `accepted` (validiert), `written` (Target-Write erfolgreich) und `executed` (Target durch explizite Antwort bestaetigt) unterscheiden. Kein blindes Auto-Retry, welches Kommandos doppelt ausfuehrt.

**Absichtliche Grenze:** Nichtblockierende `tlog`-Sinks können nicht garantieren, dass ein Betriebssystem niemals CPU oder Kernelzeit beansprucht; garantiert werden soll, dass *das Warten auf einen Sink* nicht Teil des Target-Lesepfades ist. Tests prüfen diesen Architekturvertrag mit blockiertem Writer.

## 4. Bestehender Displayserver und heutiger Binary-Fan-out

### 4.1 DS, heute implementiert

```sh
# Terminal 0: Start des Displayservers (kein -port!)
trice ds -ipa 127.0.0.1 -ipp 61497

# Terminal 1: Target A und eigene globale Hostfilter
tlog -p COM3 -ds -ipa 127.0.0.1 -ipp 61497 -logLevel wrn

# Terminal 2: Target B, andere Filter
tlog -p COM4 -ds -ipa 127.0.0.1 -ipp 61497 -pick dbg

# Terminal 3, bei Bedarf: geordneter Shutdown
trice shutdown -ipa 127.0.0.1 -ipp 61497
```

`trice ds` ist ein **Go-net/rpc-Server mit Listener**; mehrere Logger senden fertig aufbereitete Zeilen/Fragmente dorthin, `trice ds` zeigt sie farbig an und kann selbst eine Textdatei fuehren. **Kein DS-eigener `-logLevel`, `-pick`, `-ban`-Filter**. `DisplayServer.LogSetFlags` setzt nur Go-`log`-Flags, nicht Trice-Log-Auswahl. `trice ds` hat **keinen `-port`**. Die Tastaturbefehle beim Logger mit `-ds` gehen über dessen eigenes `stdin` an die beschreibbare Target-Verbindung; DS ist keine allgemeine Target-Injection-API.

### 4.2 Binary-Fan-out ist schon heute möglich (ohne neue -out)

```sh
# Writer auf dem Target-PC:
tlog -p COM7 -binaryLogfile trace.bin

# Mehrere Leser, in je eigenem Terminal / Prozess:
tlog -p FILE -args trace.bin -pick err
tlog -p FILE -args trace.bin -logLevel wrn
tlog -p FILE -args trace.bin -logFormat json
```

`-binaryLogfile` schreibt Empfangsbytes *vor* Dekodierung und Hostfilter. `-port FILE` liest eine weiterhin wachsende Datei; die drei Leser haben eigene Filter. `-port FILEBUFFER` liest dagegen bis EOF. Das Linux/Windows/macOS-Dateisystem kann eine fortgeschriebene Datei mehrfach öffnen; in realen Testumgebungen File-Sharing-Rechte, Startreihenfolge, Schreib-/Lesepufferung und mögliche Teil-Frames testen. Alle Leser brauchen zum Writer passende `til.json`, `-pf`, `-encoding`, Passwort/Endian etc. **Hostzeitstempel der urspruenglichen Aufnahme werden nicht automatisch als Empfangszeit rekonstruiert**.

Die Zukunft kann denselben Fan-out live per **Raw-TCP** ohne Dateisystemzwischenstufe bereitstellen. Weiterhin dekodiert jede nachgeschaltete Instanz eigenständig; das ist gewollt und erspart per-Ausgang-Filter im Produzenten.

### 4.3 DS-Zukunft

```sh
# Aggregator (weiterhin eigenes Subcommand)
trice ds -listen ds://127.0.0.1:61497

# Zwei Logger mit jeweils exakt einem Target
tlog -p COM3 -out 'sink=ds://127.0.0.1:61497;format=text'
tlog -p COM4 -out 'sink=ds://127.0.0.1:61497;format=text'
```

Der DS bleibt ein simpler Zeilen-Aggregator. Es wird **kein `-port DS`** benötigt, und `tlog` wird **kein Multi-Port-Aggregator**. Optional kann ein späterer DS auch Metadaten zusätzlich anzeigen; zur Ablösung des heutigen DS-RPC-Protokolls ist das **keine Voraussetzung**.

## 5. Steuerkanäle (`-ctrl`) — ein Target, mehrere Eingänge

Neues Zielbild:

```sh
tlog -p COM7   -ctrl stdin:   -ctrl tcp-listen://127.0.0.1:9002   -out 'sink=stdout;format=text'
```

**Beide** Clientarten steuern nur COM7. Der Target-TX-Writer serialisiert die Befehle und darf keine Bytes vermischen. Für eine spätere ABC-Injection sind Tokenisierung/NDJSON oder length-prefix, `request_id`, Protokollversion, Maximalgroesse, ID-Aufloesung über TIL, Parametertyp-/Wertebereichpruefung, COBS/TCOBS, ggf. Verschluesselung und ein klarer Statuspfad noetig. Mehrdeutige TIL-IDs **nicht zufaellig waehlen**. Das Request-Schema bleibt offen. Beispiel nur zur Illustration:

```json
{"op":"inject","request_id":"r-12","triceC":"demo:reset"}
{"op":"inject","request_id":"r-13","trice32C":"demo:setRange","values":[958314643,966367642]}
```

ABC-Receivers können heute Target-seitig mit `trice generate -abc ...` erzeugt werden; eine **generische Host-TCP-Steuer-API ist heute noch nicht vorhanden**. Normale Trices loesen nicht automatisch Target-Aktionen aus. Schreibberechtigung ist sicherheitsrelevant: Listener standardmaessig nur `127.0.0.1`; für Remote-Clients TLS/Auth/Berechtigungen getrennt planen. `-ctrl` ist keine Fernadministration eines gesamten DS oder der Hostlog-Filter.

## 6. DLT und weitere OSS-Anbindungen

### 6.1 DLT-Bridge bleibt extern

```text
Target -> tlog -> (gefiltertes) JSON/NDJSON -> trice-dlt-bridge -> DLT/TCP -> COVESA DLT Viewer
                                                          ^ DLT Control Requests
```

**Wichtige Revision gegenüber einem frueheren Handoff:** Es gibt **keine pro-`-out`-Ausnahme**, die ungefilterte JSON-Events trotz global `-logLevel wrn` liefert. Wenn die Bridge alle dekodierten Events bekommen soll, laeuft dieser `tlog` mit `-logLevel all`. Für eine gleichzeitig lokal andere Filteransicht wird **ein weiterer `tlog` auf derselben Raw-Quelle** verwendet. Eine reine Bridge kann DLT-Viewer-Filter anschliessend für ihre eigene Ausgabe anwenden; DLT `SET_LOG_LEVEL` bedeutet **nicht** automatisch, dass die MCU weniger Trices erzeugt.

**MVP: DLT v1 verbose**, keine DLT-Abhaengigkeit in `tlog`, kein DLT auf dem Target, keine Injection über DLT. Adapter erzeugt DLT-Nachrichten mit ECU/APID/CTID, Nachrichtenzaehler und plausiblen Metadaten. Viewer-Handshake und `GET_LOG_INFO`, `SET_LOG_LEVEL`, `SET_TRACE_STATUS` **korrekt** im Adapter implementieren bzw. als nicht unterstuetzt behandeln. `GET_SOFTWARE_VERSION` nur aus einer tatsaechlich empfangenen **frischen** zyklischen Trice-Firmwareversion beantworten; nach Reset/alter Information invalidieren. `GET_LOG_INFO` am echten COVESA DLT Viewer testen. `DLT_TRACE_STATUS` ist **nicht** Trice-Level `TRACE`.

| Effektiver Trice-Tagweight | DLT-v1-Level (Vorschlag) |
|---|---|
| FATAL 900 | FATAL 1 |
| CRITICAL 800 | FATAL 1; eigene Stufe geht verloren |
| ERROR 700 | ERROR 2 |
| WARNING 600 | WARN 3 |
| INFO 500 | INFO 4 |
| DEBUG 300 | DEBUG 5 |
| TRACE 100 | VERBOSE 6 |
| VERBOSE 0 | VERBOSE 6 |

Die Tags können eigenregistrierte Gewichte erhalten; Zuordnung nach **effektivem Gewicht**, nicht blind nach dem Namen. DLT v1 bietet nur vier Zeichen für APID/CTID: entweder Tags kollisionsfrei auf Context-IDs abbilden oder Module/Struktur als Context verwenden und Tags als Nutzdaten/Metadatum mitfuehren. DLT v2 mit laengeren IDs/Tags ist ein späterer Kompatibilitaetstest, **keine** vorausgesetzte Funktion. Trice-ID, Target-/Host-Zeitstempel, SL/CE-Felder und Ereignisquelle explizit abbilden; Diagnosefehler nicht als normale Applikationslogs maskieren.

### 6.2 Weitere Adapter (Auswahl, keine Implementierungszusagen)

| OSS-Programm | Zweck | Host-Schnittstelle / externer Adapter |
|---|---|---|
| LabPlot | numerische Live-Messwerte | Bestehendes `-vis` CSV/UDP; später `-out vis` |
| GTKWave | digitale/numerische Zeitverlaeufe | `-out vcd`, plus einmaliges `trice generate -gtkw` |
| COVESA DLT Viewer | Automotive-Logging | eigenständige DLT-v1-Bridge nach NDJSON |
| Perfetto | Timing, Counter, ISR/Task-Traces | externer Konverter in Trace-JSON/Perfetto-Schema |
| Eclipse Trace Compass | Event-/RTOS-Spuren | externer CTF-Adapter, später wenn Datenmodell traegt |
| Serial Studio | Live-Dashboard numerischer Werte | CSV/UDP/TCP-Ausgabe mit passendem Schema |
| Node-RED | Steuer-/Monitoring-App | NDJSON über `-out`, Befehle via `-ctrl` |
| Grafana + Loki | Langzeitanalyse | externer Agent/Adapter liest NDJSON |
| Rerun | multivariate, zeitabhaengige Telemetrie | typisierender externer Adapter |

**Kein** spezieller `-out dlt`, `-out perfetto`, `-out node-red`-Flag in `tlog` erforderlich. Rohes `raw` ist für andere `tlog`-Dekoder, nicht automatisch für heterogene Viewer decodierbar. Neue Formate nur auf Basis realer Daten- und Zeitsemantik freigeben.

## 7. VCD/GTKWave: technischer Anschlussentwurf aus `handsoff_vcd-gtkw_v3.md`

### 7.1 Rolle der Komponenten

`trice generate -gtkw=generated/session.gtkw -til til.json` ist ein **einmaliger View-Generator**, der aus TIL nur die `.gtkw` erzeugt; **keine** VCD, **keine** Veraenderung von Firmware, Sources oder TIL. `trice log ... -out vcd:<Datei|->` ist ein separater Writer für **endliche** (`FILEBUFFER`) wie **fortlaufende** (COM/RTT/TCP) Eingaben. Beide benutzen denselben TIL-Snapshot/kanonischen Namensgenerator. `log` darf eine vom Nutzer bearbeitete `.gtkw` niemals bei jedem Lauf neu erstellen oder überschreiben. **Keine** Pflicht-`signals.yaml`, kein `-vcdConfig`, keine neuen Target-Makros/Wire-Aenderungen. Das Viewer-4-Format `.gwprj` ist kein MVP-Ziel.

### 7.2 Feldidentitaet und Typvereinigung

**Kanonische Structured-Logging-Feldnamen sind die Signalidentitaet, nie ID, Tag, C-Typ oder Aufrufstelle.** `TRice8("info:{x:%u}", u8)` plus `TRice32("wrn:{x:%d}", i32)` plus `TRice32("dbg:{x:%.3f}", aFloat(xf))` erzeugen **ein** `x`-Signal. Jede Trice-ID kann 0 bis mehrere Felder enthalten; mehrere IDs dürfen dasselbe Feld aktualisieren. `motor.rpm` und `rpm` bleiben verschieden; Punkt-Hierarchien müssen in VCD und `.gtkw` **identisch** gemappt werden.

Vereinigung **vor** `$enddefinitions`, aus *allen* geeigneten TIL-Quellschemas: bool -> 1-bit wire; nur Integer mit unterschiedlichen Breiten/Sign -> ausreichend breiter Bitvektor (Zweierkomplement bei negativen Werten); irgendein float zusammen mit int/bool -> `real 64` für dasselbe Signal. Nicht bei gemischten numerischen Typen abbrechen. Bei Integerwerten über `2^53` in `real` praezise Warnung für möglichen Verlust; reine 64-Bit-Integer als Bitvektor behalten. Strings/Pointer/Buffers zunaechst nicht in VCD abbilden. `NaN`/`Inf` nicht in Null umdeuten. Identische Updates dürfen entfallen; mehrere Feldupdates eines Records tragen denselben Timestamp. Unbekannte Initialzustaende nicht als 0 erfinden.

Das V3-Beispiel enthaelt **14** Felder: `axis.position_steps`, `ctrl.cycle`, `ctrl.demand`, `ctrl.fault_flags`, `ctrl.mode`, `imu.ax_g`, `imu.ay_g`, `imu.az_g`, `irq.TIM2_active`, `motor.rpm`, `power.current_mA`, `power.supply_mV`, `sensor.temperature_c`, `x`. `x` ist trotz drei ID-/Typquellen **ein** Signal. `irq.TIM2_active` besteht aus expliziten 0/1-Events; Freitext `IRQ enter/exit` wird nicht automatisch gemappt. Der Boot-Record ohne Felder erzeugt kein Signal, darf aber die Zeitnull definieren.

### 7.3 Zeitbasis und Recordauswahl

Target-Timestamps sind keine UTC-Zeit und `TRice`-Stampbreite kein Tickfrequenz-Nachweis. Eine explizite Quelle wie `-vcdTickHz 1000000` ist im MVP plausibel; später kann der vorhandene Timestamp-Expression-Mechanismus mit genutzt werden. VCD-Zeitnull: **erster erfolgreich dekodierter gestempelter Record**, auch ohne Structured Field. Counter-/Wrap-Zustand *vor globaler Logauswahl* beobachten, damit wegselektierte Records nicht Wrap-Erkennung oder Zeitursprung verschlechtern. 32-Bit-Upcounter und eindeutig erkennbare `2^32`-Wraps zuerst; ungeloeste Resets, nichtmonotone Stamps und fehlende Timestamps **diagnostizieren, nicht erfinden**. `ts16`/`ts32`-Mischung nur bei definierter gemeinsamer Basis; Downcounter/Expressions später. Integer-/rationale Rundung für VCD-Ticks bevorzugen, ggf. Quantisierung offenlegen.

**Abgleich mit der globalen Filterregel:** `-pick motor` selektiert im gesamten Logger die Records. Der VCD-Sink greift daraus geeignete benannte Felder ab; er darf weder eine eigene allgemeine Hostfilterstufe einfuehren noch andere -out-Pfade unterdrücken. Das erzeugte GTKWave-View kann alle möglichen TIL-Felder umfassen, auch wenn eine einzelne Capture einige davon nie aktualisiert. Historische/stale Eintraege in TIL sind möglich; Auswahl nach tatsaechlicher Capture waere ein optionaler späterer Generator-Modus, keine neue Pflichtkonfigurationsdatei.

### 7.4 File, Pipe, Live und Formatvertragsgrenzen

```sh
# Vorschlag; noch nicht implementiert
trice generate -gtkw=generated/session.gtkw -til=til.json
trice log -p FILEBUFFER -args capture.bin -til til.json   -vcdTickHz 1000000 -out vcd:generated/session.vcd
trice log -p FILEBUFFER -args capture.bin -til til.json   -vcdTickHz 1000000 -out vcd:- > session.vcd
# Legacy-Live-Pipe nur nach echtem GTKWave-3.3-Smoke-Test:
trice log -p COM7 -til til.json -vcdTickHz 1000000 -out vcd:-   | shmidcat | gtkwave -v -I generated/session.gtkw
```

VCD-Header vollständig **vor** Samples schreiben; bei Live-Streams nachliefern und flushen, nicht bis EOF warten. Eine VCD-Datei **neu anlegen**, nie ungeprüft an alte VCD anhängen. `stdout` für VCD exklusiv; Diagnosen nach `stderr`. Schema ist nach Header fest und kann bei Live-TIL-Aenderung nicht beliebig erweitert werden; bei inkompatiblen neuen Feldern diagnostizieren/überspringen und neue Aufnahme anfordern. Gleichnamige Signale stable sortieren, `@`-Ansichtflags/radix mit GTKWave 3.3 real prüfen. Fremdtool-Tests dürfen optional sein; VCD selbst mit Parser testen. GTKWave 4 hat anders gelagerte Live-/`.gwprj`-Entwicklung; **keine** unbelegte Live-Unterstuetzung behaupten.

**Sonderfall Queue-Clear bei VCD:** Ein VCD-Stream kann nach verlorenen Updates syntaktisch lesbar bleiben, aber zeitliche Zustandsübergaenge verpassen; für Datei und Pipe **Lücken explizit diagnostizieren**. Eine *vollständig* ununterbrochene VCD-Aufzeichnung ist wegen des festgelegten Nichtblockier-Defaults nicht versprochen. Für unverzichtbare Nachvollziehbarkeit zusätzlich eine Rohdatenerfassung am Target/Transport erwägen; auch die ist nicht automatisch verlustfrei.

### 7.5 Hardwarefreies Demo- und Testinventar

`examples/VCD/` mit `main.c`, `triceConfig.h`, `til.json`, `README.md`, `run.sh`, `expected/`, `generated/session.gtkw`, beim Replay erzeugter `session.vcd`. Reale PC-Trice-Binaerrecords mit Bind/Insert, **keine** frei erfundenen produktiven IDs. Teste Records mit **0/1/2/3/4** benannten Feldern, verschiedene Updatefrequenzen, einen IRQ-Zustand 0/1, `x` aus uint8/int32/float32, mehrere Captures bei unveränderter manuell bearbeiteter `.gtkw`, endliche und Live-Ausgabe, File und Pipe, Timestamp-Wrap und nichtmonotone Daten, stdout-Reinheit, stale TIL-Schemas, Speicherverbrauch O(Signale) statt O(Recordzahl). Validieren, dass es in `generate` keinen Konflikt mit vorhandenen `-abc`, `-logC`, `-onelineJSON`, `-bindReport`, `-colors` gibt. Alle Regressionstests für `log`, `-vis`, Structured Logging, `bind/insert/clean` erhalten.

**Bewusst nicht jetzt:** `.gwprj`, FST, eigener GTKWave-Viewer, frei textbasiertes Enter/Exit-Mapping, automatische GUI-Prozesssteuerung, neue Firmware-Makros, Multi-Target, zusätzliche `signals.yaml`-Pflicht.

## 8. Bifurkations- / ABC-Demo als späterer Integrationstest

Die alten CLI-/ScratchPad-Handoffs beschreiben eine **Bifurkationsdemo**: STM32G0B1 (16 MHz), logistische Abbildung `x[n+1] = r*x[n]*(1-x[n])`, `r` zwischen 3 und 4, 101 Parameterwerte bei `0.01` Schritt, eigener Zustand je `r` (101 x 4 Byte = 404 Byte), 100 ms pro `r`, nominal 1000 Messpunkte/s und ein Lauf von ca. 10,1 s. Ein möglicher Startwert ist `x0 = 0.314159265`; Ein- und Ausschwingphasen und Numerik (32-Bit-Festkomma mit 64-Bit-Zwischenprodukten) prüfen. Die Datenrate hängt von Recordbreite und Framing ab; vorgeschlagene UART-Baudraten sind 230400/460800, kein garantierter Durchsatz. Die bestehende LabPlot-Demo mit 50 Samples/s und 500 Zeilen ist **nicht** dieser Lastfall.

Die GUI-Idee ist eine kleine Go-App mit eingebettetem HTML/JS: Center-Slider, logarithmischer Zoom, Reset und ggf. Pause/Rate. Symbolische ABC-Requests wie `demo:setRange` und `demo:reset` gehen über `-ctrl` und **denselben Target-TX**, nie über eine zweite UART-Oeffnung. LabPlot muss eine anwachsende Punktwolke statt bloss der letzten 500 Zeilen zeigen; etwa 3,6 Mio. Punkte/h bei 1000/s verlangen Grenz-/Downsampling-Tests. Zoomreihen `3..4 -> 3.50..3.60 -> 3.560..3.580 -> 3.5690..3.5710`; für tiefen Zoom 16-bit absolute r-Kodierung unzureichend, 32-Bit oder relative Kodierung untersuchen. Targetzustand, Zoomparameter und Viewer-Achsen koordinieren. Generischer Host-Trice-Encoder und Requestvalidierung sind **spätere** Module; der historische Switch `-tcpListen` wird **nicht** neu eingefuehrt.

Aus alten ScratchPad-Randnotizen **nicht** als sofortige Schnittstellenaufgaben werten: `examples/TriceABC`-Skripte/README verständlicher machen, `trice-fields` und generierte Dateien auffindbar machen, `-sl`/`-ce`-Beispiele erweitern, laengenreiche RM-Demos kuerzen, ältere 'MVP'-Dokumentnamen prüfen, Feature-Teaser pflegen. Historische Testzeiten Win ca. 11770 s / Mac ca. 4400 s **nicht ohne erneute Messung als aktuell verwenden**. Der `third_party`-README-Linkcheck-Auftrag **N04 ist seit 05.11.2026 entfallen**; keinen neuen Linkcheck dafür einplanen.

## 9. Migration: neue Wege parallel zur alten CLI

**Grundsatz:** Alle heutigen Switches bleiben anfangs weiter implementiert und dokumentiert. Zuerst neue Ausgabepfade **parallel** einführen, mit denselben Input- und Filtereinstellungen beweisen, dass Legacy-Beispiele weiter funktionieren. Erst bei dokumentierter Ersetzung und ausreichender Nutzungs-/Versionsfrist eine Warnung und später eine Entfernung. `deprecated` ist ein **Prozess**, keine sofortige Aenderung des Defaultverhaltens.

| Bestehender Schalter/Funktion | Vorgesehener neuer Weg | Behandlung |
|---|---|---|
| `-port`, `-p`, `-args`, `-baud`, RTT-/Serial-Parameter | gleiche Bedeutung | **Behalten**, nicht deprecaten aus diesem Projekt |
| `-logLevel`, `-pick`, `-ban`, `-utag` | globale Hostfilter | **Behalten**, nicht in einzelnen -out duplizieren |
| `-binaryLogfile`, `-blf` | `-out ...format=raw` Datei | Vorerst parallel; Append-/Autoname-Kompatibilitaet testen |
| `-logfile`, `-lf` | `-out ...format=text` Datei | Parallel; Format, Append, Banner und Mehrzeilen testen |
| `-tcp` | `-out ...tcp-listen;format=text` | Parallel; alte PuTTY-Telnet-Semantik und Blockierungsverhalten nicht still ersetzen |
| `-ds`, `-displayserver` bei `log` | `-out ...ds://...` | Parallel; DS-Go-RPC-Methode und Farblayout respektieren |
| `-ipa`, `-ipp` bei `log`, `ds`, `shutdown` | `-out ds://...`, `-listen`, `-connect` | Parallel; verschiedene Subcommands getrennt testen |
| `-vis` | `-out ...format=vis` mit Transformation | Parallel; bestehendes `log=drop` bleibt im Altpfad funktionsfaehig, neues -out darf andere Sinks nicht unterdrücken |
| `-logFormat` | `format=` je -out, eine globale Filterung | Parallel; keine unerwartete doppelte stdout-Ausgabe |
| Keyboard im `-ds`-Logger | `-ctrl stdin:` | Parallel; DS-Pflicht danach nur bei bewusst gewaehltem Altmodus |
| `-debug`, `-showInputBytes`, `-v`, `-stat` etc. | Diagnose neben Outputs | Weiterhin gültig, Diagnosen sauber vom Datenstrom trennen |
| DS `-color` / DS `-logfile` | ggf. weiterhin DS-eigene Optionen | **Behalten**; keine unnötige Neuimplementierung |

**Mischbetrieb/Mehrdeutigkeit im Migrationsfenster:** Je nach Funktion kann eine Altoption neben `-out` absichtlich einen **zusätzlichen** Sink liefern. Defaults und Doppelziele müssen aber explizit spezifiziert werden. Vorschlag: `tlog` ohne neue Flags byteweise wie zuvor; wenn `-out` gesetzt ist, explizite Sinkliste statt einer impliziten zweiten stdout-Ausgabe, es sei denn ein Legacy-Flag wird ausdrücklich übersetzt. Zwei Konfigurationen, die **dieselbe Datei gleichzeitig öffnen**, vorab ablehnen oder klar als ein gemeinsamer Sink zusammenfassen (nicht unkontrolliert zwei Filehandles). Legacy-Funktionen dürfen in v0 nicht still ohne Kompatibilitaetsadapter umgestellt werden. Unpassende Kombinationen (`-logFormat json` mit Legacy-DS usw.) bleiben bis zur separaten Freigabe unverändert oder werden explizit diagnostiziert.

**Warn- und Entfernungsfolge:** (1) Ersatz implementiert und Tests grün; (2) dokumentierte, funktional aequivalente Beispielkommandos; (3) optionale Hinweise auf neue Syntax ohne warn-spam; (4) formales deprecated ab angekuendigter Version; (5) Entfernung nur in einer späteren, bewusst geplanter Breaking-Version. Keine Termine oder Versionen jetzt erraten. Release Notes, Helptext, Shell Completion, Demos, CI und Windows/macOS/Linux vor jeder Stufe prüfen.

## 10. Ausgewertete `wip/docs/_scratchPad`-Dokumente und Löschkandidaten

**Pruefstand:** `wip` HEAD `f7b819207d7e435159890227b7a5fd360d3ec727` vom 17.11.2026; die folgende Einordnung gilt für genau diesen Stand. **Ich habe im Repository nichts verändert oder gelöscht.** Nach Aufnahme dieser Datei in `wip` und Sichtpruefung können veraltete Entwurfs-Handoffs gelöscht werden. Das Entfernen des Dateinamens aus Git ist nicht gleich dem Loeschen seiner Historie.

| Datei in `docs/_scratchPad` | Fachliche Abdeckung hier | Empfehlung zur Löschung |
|---|---|---|
| `_handsoff_tlog_CLI_Architektur.md` | TCP/API/ABC, DS, Demo, Altoptionen, historische verworfene Varianten | **Ja**, nach Einpflegen/Review von v0; durch spätere Diskussion überholt |
| `handsoff_tlog_out.md` | `-out`, DLT-Bridge, per-Sink-Probleme, Migration, Demo, Altflags | **Ja**, nach Review; die alte Forderung nach individuellen -out-Filtern wird hier ausdrücklich verworfen |
| `handsoff_tlog_schnittstellendesign.md` | letzte verbindliche Grundentscheidungen, Queues, Fehler, DS | **Ja**, nach Review; alle Kernregeln übernommen |
| `scratchPad.md` | Bifurkations-, ABC- und LabPlot-Demo, historische Nebenaufgaben | **Bedingt**: Schnittstellen- und Demo-Kern ist hier zusammengefasst; nur löschen, wenn Detailpseudocode/GUI-Skizzen nicht als eigenes vollständiges Demo-Lastenheft gebraucht werden |
| `handsoff_vcd-gtkw_v3.md` | Signalname/Typvereinigung, TIL, Zeitbasis, .gtkw/VCD/Pipe, Test-/MVP-Grenzen | **Bedingt**: technisch integrierter Entwurf; vor Loeschung gegen v3-Definition-of-Done zeilenweise gegenprüfen, da v3 ein detaillierter Implementierungsauftrag ist |
| `handsoff_benchmark.md` | Benchmark-Thema hier nur als möglicher Folge-Lasttest | **Nein**, eigenes Mess- und Vergleichsvorhaben |
| `handsoff_publications.md` | Publikationsstrategie nicht Teil des Interface-Redesigns | **Nein**, eigenes Thema |
| `handsoff_MCU-Simulation.md` | LabWired-Eignung als Demo-Testkontext, nicht in CLI implementiert | **Nein**, eigenständiges Simulator-Handoff |
| `Test_Optimization_Plan.md` | Test-Infrastruktur als Querschnittsthema, nicht ersetzt | **Nein**, eigener umfassender CI-/Testplan |
| `Control-API.svg` | schematische Grafik | **Nein**, Medienquelle nicht durch Markdown ersetzt |
| `test_logs/` | archivierte Laeufe | **Nein**, keine Text-Handoffs, Daten nicht löschen |

**Praktischer Loeschvorschlag:** Erst v0 in einem kleinen Doku-PR aufnehmen; danach die **drei mit „Ja“ markierten** Handsoffs auf `wip` entfallen lassen. `scratchPad.md` und `handsoff_vcd-gtkw_v3.md` erst nach einem ausdrücklichen Review der jeweiligen Detailanhaenge entfernen, nicht rein aufgrund eines ähnlichen Titels. Die uebrigen Dateien bleiben.

## 11. Quellen und Geltungsbereich

### Primärquellen auf `wip` (17.11.2026)

- [`_handsoff_tlog_CLI_Architektur.md`](https://github.com/rokath/trice/blob/wip/docs/_scratchPad/_handsoff_tlog_CLI_Architektur.md): historische CLI- und ABC-Entscheidungen, TCP-Kommandoeingang, Demo-Kontext.
- [`handsoff_tlog_out.md`](https://github.com/rokath/trice/blob/wip/docs/_scratchPad/handsoff_tlog_out.md): wiederholbares -out, DLT-Bridge, fruehere Filtern-Varianten (durch neuere Entscheidung überholt).
- [`handsoff_tlog_schnittstellendesign.md`](https://github.com/rokath/trice/blob/wip/docs/_scratchPad/handsoff_tlog_schnittstellendesign.md): verbindlicher neuester Stand zu einem Port, mehrfach -out/-ctrl, raw, bounded queues, DS.
- [`handsoff_vcd-gtkw_v3.md`](https://github.com/rokath/trice/blob/wip/docs/_scratchPad/handsoff_vcd-gtkw_v3.md): VCD/GTKWave-Signalidentitaet, Zeitbasis, Demo, Tests.
- [`scratchPad.md`](https://github.com/rokath/trice/blob/wip/docs/_scratchPad/scratchPad.md): historische Bifurkationsdemo und offene Nebenideen; überholtes `-tcpListen`.
- [`handsoff_MCU-Simulation.md`](https://github.com/rokath/trice/blob/wip/docs/_scratchPad/handsoff_MCU-Simulation.md), [`handsoff_benchmark.md`](https://github.com/rokath/trice/blob/wip/docs/_scratchPad/handsoff_benchmark.md), [`handsoff_publications.md`](https://github.com/rokath/trice/blob/wip/docs/_scratchPad/handsoff_publications.md), [`Test_Optimization_Plan.md`](https://github.com/rokath/trice/blob/wip/docs/_scratchPad/Test_Optimization_Plan.md): gesichtet, thematisch getrennt.

### Ist-Verifikation aus CLI und Code

- [`docs/ref/trice-help-all.txt`](https://github.com/rokath/trice/blob/wip/docs/ref/trice-help-all.txt), [`internal/args/init.go`](https://github.com/rokath/trice/blob/wip/internal/args/init.go) — CLI-Optionen, Aliase, Defaults.
- [`internal/args/handler.go`](https://github.com/rokath/trice/blob/wip/internal/args/handler.go), [`internal/receiver/receiver.go`](https://github.com/rokath/trice/blob/wip/internal/receiver/receiver.go), [`internal/translator/translator.go`](https://github.com/rokath/trice/blob/wip/internal/translator/translator.go) — Input, Raw-Capture, Decoding, Filterfolge.
- [`internal/vis/router.go`](https://github.com/rokath/trice/blob/wip/internal/vis/router.go) — bisheriger -vis-Sink und log=drop.
- [`internal/emitter/server.go`](https://github.com/rokath/trice/blob/wip/internal/emitter/server.go), [`internal/emitter/remoteDisplay.go`](https://github.com/rokath/trice/blob/wip/internal/emitter/remoteDisplay.go) — DS-Server und Logger-RPC-Client.
- [`docs/rm/cli.md`](https://github.com/rokath/trice/blob/wip/docs/rm/cli.md), [`docs/rm/visualization.md`](https://github.com/rokath/trice/blob/wip/docs/rm/visualization.md), [`docs/rm/structured_logging.md`](https://github.com/rokath/trice/blob/wip/docs/rm/structured_logging.md) — dokumentierte Verwendung.

## 12. Arbeitsplanentwurf — kleine, testbare Arbeitspakete

Jedes Paket ist **ein eigenständiger, kleiner PR** mit klarer Definition of Done, reproduzierbarem lokalem Test, plattformneutralen Testfixtures und ohne den Zwang, gleichzeitig DLT/VCD/Control fertigzustellen. Die Schritte bauen aufeinander auf, können nach bestandener Stufe beendet oder umgeordnet werden. Kein vorzeitiger Grossumbau.

| Schritt | Kleinster sinnvoller Change | Nachweis / Abnahmetest |
|---:|---|---|
| **0** | CLI-/Verhaltens-Snapshots, Fixtures für alle wichtigen Eingabeformate und DS; Dokument v0 reviewen | Golden Tests für `-port`, `-args`, `-logFormat`, `-vis`, `-binaryLogfile`, `-debug`, DS; bestehende CI bleibt grün |
| **1** | `-out`-Parser und Datenmodell, noch **ohne aktive Sinks** | gültige/ungültige Beispiele, `-help`, Quoting, wiederholte Optionen, Default-stdout, kein Ziel-Oeffnen bei Fehler |
| **2** | Rohbytes-Tap vor Decoder/Filtern; bestehendes Legacy-Logging unverändert | Hex-Identitaet des kompletten Inputs; `-logLevel off` ändert Raw nicht; Cycle/COBS/TCOBS-Vergleich |
| **3** | Generischer Sink-Dispatcher mit **einer begrenzten Queue pro Sink** und `clear-queue` | Fake-Writer blockiert; andere Outputs/Target-Reader laufen weiter; Queue voll -> vollständiger Drop und Zähler |
| **4** | Neuer `-out ...format=raw` zu Datei | Vergleiche gegen `-binaryLogfile` im nicht-überlasteten Fall, `auto`/append, `FILEBUFFER`-Replay, Teil-Frame-/Reset-Test |
| **5** | Neuer Text-Sink `stdout`, File; globale Filter nur einmal | Goldene Textzeilen identisch; mehrere -out; kein ungewollter zweiter stdout; Legacy `-logfile` unverändert |
| **6** | JSON/NDJSON und KV mit separatem Diagnosekanal | JSON-parser je Zeile; keine ANSI/Status-Banner; gleiche gefilterte Ereignismenge wie Text; Raw weiter ungefiltert |
| **7** | TCP-Listener-Sink für Text und Raw, mehrere Clients, pro-Client-Queue | drei Clients, einer stoppt Lesen, nur dessen Queue wird geleert/Client getrennt; andere laufen, Reconnect getestet |
| **8** | Numerischer `vis`-Writer als `-out`-Transformation | CSV/UDP gegen -vis vergleichen; alter `log=drop` funktioniert weiter, neuer Sink unterdrueckt andere nicht |
| **9** | `-out ds://` mit bisherigem DS-RPC-Protokoll | zwei tlogs/ein DS, getrennte globale Filter, gleiche Zeilen/Farben wie Legacy; kein DS-eigener Logfilter |
| **10** | `trice ds -listen`, `trice shutdown -connect` neben `-ipa/-ipp` | Default/IPv4/Hostname, Shutdown, Legacy-Aufruf unverändert, Fehladressierung diagnostiziert |
| **11** | `-ctrl stdin:` ohne notwendiges `-ds` | bisheriges Tastaturkommando an writable UART/TCP4, genau ein Target, `quit`, EOF, fehlerhafte Writes |
| **12** | `-ctrl tcp-listen://` und serialisierter Target-Writer (ohne ABC-Schema) | zwei Clients schreiben konkurrierend, Bytes niemals vermischt, bounded Requests, Abbruch, Loopback-Default |
| **13** | Optionale symbolische ABC-Requestcodierung mit TIL-Lookup | gültige/ungültige IDs, Ambiguitaet, Wertebereich, Framing, Offline/ACK, keine Doppel-Retries; Target-Simulator |
| **14a** | TIL-Signalindex und deterministische Typvereinigung für VCD | `x` aus 3 IDs/Typen ist 1 `real`; Integer 64 Bit; 14 Demo-Signale; kein signals.yaml |
| **14b** | `trice generate -gtkw`, ohne VCD/Target-Log-Pfad | deterministische View, kein ungefragtes Überschreiben, stabile Namen bei TIL-ID-Aenderungen |
| **14c** | VCD-Writer über vorhandenes -out zu Datei/Pipe | Parserpruefung, Timestamp-Wrap, Live-Header, stdout-Reinheit, zwei Captures/gleiche .gtkw, GTKWave 3.3 smoke wenn verfuegbar |
| **15** | Externe, minimale DLT-v1-Bridge (separates Repository/Programm) | DLT Viewer verbindet sich, `GET_LOG_INFO`/Levels korrekt, Tags/Version, keine MCU-Level-Aenderung, keine Injection |
| **16** | Optional Perfetto/Node-RED/Trace-Compass-Adapter, nach realem Bedarf | Echte Beispiel-Datei/Viewer, reproduzierbarer Roundtrip, keine proprietaere tlog-Sink-Kopplung |
| **17** | Dokumentation, Migrationshilfen, Deprecation-Entscheidung **separat** | Legacy-Kommandos unverändert bis zur angekuendigten Versionsgrenze, Beispiele, Hilfe, Release Notes, Win/Linux/macOS-Smoketests |

### 12.1 Kleinste Testmatrix pro Ausgabe

- **Rohdaten:** vor Hostfilter, mit `-logLevel off`, Byteidentitaet ohne Overflow, bei Overflow `clear-queue`, bei COBS/TCOBS erneute gültige Framegrenze, Cycle-Verlustbericht soweit möglich.
- **Dekodiert:** eine gemeinsame globale Auswahl für Text/JSON/KV/Vis/VCD/DS. Bei `-pick err` nie über den zweiten -out Debug-Events zurueckbekommen. Legacy -vis-`log=drop` bleibt eigenständig vorhanden.
- **Nichtblockierung:** eine absichtlich haengende Datei/Socket/`stdout`-Pipe, zwei gesunde Outputs, ein schneller Input. Messwert: gesunde Inputs/Outputs arbeiten trotz Hang weiter; keine unbounded Queue oder wachsender goroutine-Bestand.
- **CLI-Kompatibilitaet:** `trice help -all`, case-insensitive Mehrzeichenflags, gleiches Legacy-Bind-Verhalten, default stdout, ungeplante Doppeldateien, alte DS-RPC-Clients, `-ipa/-ipp` und `-args`.
- **Sicherheit:** Listener nur lokal, keine unerwarteten offenen Ports; Schreibberechtigungen sind auf das eine Target beschraenkt.
- **Ressourcen:** definierte Speichergrenzen pro Sink/Client auch bei 1000+ Ereignissen/s, möglichst ohne echte Hardware im CI; reale UART/RTT-Smokes optional separat dokumentieren.

### 12.2 Offene Entscheidungen für ein kleines v1-Designreview

1. Exakte URI/Optionsgrammatik und Shell-Escaping; `-out vcd:file` vs `-out sink=file:...;format=vcd`.
2. Default-`stdout`, `-out` mit `-logFormat`/`-logfile` gleichzeitig, Dopplungen, Dateimodi/Headers.
3. Konkrete Queue-Größen, Speichermesswerte, Timeout-/Close-Verhalten **ohne** blockierende Modi; `raw` bei unframed Input.
4. TCP-Multiclient-Broadcast, Reconnect, Loopback-Bind, Fehler-/Drop-Metriken und geordneter Stop.
5. Minimaler `-ctrl`-Requestvertrag und Statuscode-Liste; Sicherheitsvorgaben; kein DS-Control mit Target-Control verwechseln.
6. VCD-Zeitbasis/Stamp-Mischung und `.gtkw`-Parser-Tests; DLT-APID/CTID-Mapping ausserhalb des tlog-Kerns.

**Nicht erneut als Grundsatz diskutieren:** mehrmals `-port`, per-Ausgang-Logfilter, DS als Multitarget-Decoder, blockierender -out, Raw nach Hostfiltern oder DLT-Implementation im Target.

---

**Ende von v0.** Der Arbeitsplanentwurf ist bewusst der Schluss des Dokuments.
