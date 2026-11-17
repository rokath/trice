# Trice / tlog: Schnittstellen-Redesign

**Nachdenk- und Designdokument · 17. November 2026 · Revision: verbindliche Nicht-Blockier-Semantik · Status: Architektur festgelegt, Details offen**  
**Projekt:** https://github.com/rokath/trice  
**Geltung:** `tlog` und `trice log`; vorgeschlagene neue Optionen sind **nicht** implementiert. Kein Implementierungsauftrag.

## 1. Verbindliche Architekturentscheidungen

1. **Genau ein `-port` pro `tlog`-Instanz.** Er bezeichnet eine Target-/Datenquellenverbindung und behält die vorhandene Bedeutung. Keine Wiederholbarkeit, keine implizite Mehr-Target-Verwaltung.
2. **`-ctrl` ist mehrfach möglich, bezieht sich aber immer auf dieses eine Target.** Verschiedene lokale oder externe Steuerzugänge nutzen den gleichen Target-Schreibweg. Die jeweiligen Kommandos müssen dort geordnet werden.
3. **`-out` ist mehrfach möglich.** Jedes Vorkommen beschreibt ein Ausgabeziel und dessen Format/Transformation; **keine unabhängigen allgemeinen Log-Filter pro Ausgang**.
4. **Alle nicht-rohen Ausgaben teilen die globale Filterung** (`-logLevel`, `-pick`, `-ban` usw.). Filterung findet einmal statt, bevor dekodierte Ereignisse auf die entsprechenden Ausgaben verteilt werden.
5. **Alle `format=raw`-Ausgaben sind ausnahmslos ungefiltert.** Sie erhalten den empfangenen Binärdatenstrom **vor Dekodierung und Host-Filtern**, einschließlich vorhandenen Framings und Cycle Counters; `raw` ist nicht gleich „unframed“.
6. **Kein `-out` darf den Target-Empfang oder andere Ausgänge blockieren – ausnahmslos.** Das Target wird nie wegen eines Host-Ausgangs angehalten; es kann selbst bereits Trices verwerfen. Jeder Ausgang hat einen begrenzten Puffer. Bei Überlauf wird **dessen gesamte wartende Queue geleert (verworfen)**; danach werden neue Daten angenommen. Es gibt **keinen** `overflow=block`- oder `reliability=strict`-Modus. Verlust und Fehler werden gezählt und diagnostiziert.
7. **Mehrere Targets werden durch mehrere `tlog`-Instanzen behandelt.** Diese können ihre Ausgaben über `trice ds` zusammen anzeigen. Jeder Logger behält seine eigenen Decoder-, Filter- und Steuereinstellungen.
8. **Der Displayserver bleibt grundsätzlich ein Aggregator bereits dekodierter/formattierter Logzeilen.** Es besteht derzeit kein Bedarf, ihn durch einen Mehr-Target-`tlog` zu ersetzen.

**Nicht Bestandteil dieses Designs:** wiederholbares `-port`, ein `-port DS`, per-Ausgang-`level`/`pick`/`ban`, eine universelle Mehr-Target-ID-Zuordnung oder eine neue DLT-Implementierung im `tlog`-Kern. **Ebenso ausgeschlossen:** blockierende `-out`-Modi oder der Anspruch auf vollständige Rohdatenaufzeichnung.

## 2. Datenfluss

```text
                              tlog (genau ein Target)

Target <-----------------> -port / -args
                                |
                       eingehende Raw-Bytes
                                |
                  +-------------+-----------------+
                  |                               |
         raw fan-out (vor Decode)             Decoder + TIL/LI
                  |                               |
             -out raw                           globale Filter
              /    \                              |
          Datei   TCP --> weitere tlog         dekodierter fan-out
                       mit eigenen Filtern        /   |   |   \
                                           stdout JSON CSV DS

Externe CLI / Automatisierung -- -ctrl (mehrfach) --> geordneter Target-TX
                                      (ein Ziel: die -port-Verbindung)
```

`-out`-Ausgänge können das jeweils passende Format unabhängig serialisieren, aber **nicht** unterschiedlich nach Log-Level oder Tags filtern. Eine CSV-/Visualisierungs-Transformation verarbeitet naturgemäß nur Ereignisse, deren Werte und Schema zu ihrer Umwandlung passen; das ist **keine zweite globale Filterentscheidung**. Sie darf die Ausgabe desselben Ereignisses an andere Ausgänge nicht unterdrücken.

Wird eine andere Log-Auswahl benötigt, liest eine weitere `tlog`-Instanz denselben ungefilterten Rohstrom (z. B. über Datei oder zukünftigen TCP-Raw-Ausgang) und filtert für sich.

## 3. CLI-Zielbild (Syntax nur illustrativ)

```bash
# Ein Target, zwei Steuerzugänge, drei Ausgaben:
tlog -port COM7 -baud 115200 \
  -ctrl 'stdin:' \
  -ctrl 'tcp-listen://127.0.0.1:9002' \
  -logLevel wrn \
  -out 'sink=file:trace.bin;format=raw' \
  -out 'sink=stdout;format=text' \
  -out 'sink=ds://127.0.0.1:61497;format=text'

# Andere, unabhängig filternde Instanz liest dieselbe Rohdatei:
tlog -port FILE -args trace.bin -pick dbg

# Weiterer Logger, derselbe Displayserver:
tlog -port COM8 -out 'sink=ds://127.0.0.1:61497;format=text'
```

Die `sink=...;format=...`-Grammatik und `-ctrl`-Syntax sind noch **Vorschläge**, keine lauffähigen Befehle. Die alten Parameter `-port`, `-args` und `-baud` bleiben für den Ein-Target-Fall zunächst erhalten; eine separate Änderung ihrer Grammatik ist nicht nötig. Listener/Client-Richtung sollte bei TCP explizit sein (`tcp-listen://` gegenüber `tcp-connect://`).

**Anwendungsfälle für `-out`:** `raw` nach Datei oder TCP, `text` nach stdout/Datei/TCP/DS, `json`/NDJSON oder `kv` nach stdout/Datei/TCP, sowie numerische Transformationen für LabPlot und nachgelagerte Adapter (DLT, GTKWave/VCD, Perfetto). Formatkonverter für fremde Protokolle möglichst extern halten.

## 4. Queues: Warum und mit welchem Einfluss?

**Problem:** Bei direktem synchronem Schreiben kann ein blockierender TCP-Client, eine langsame Datei oder ein stockendes Terminal den Empfang im Host ausbremsen. Das ist unzulässig: Das Target wird niemals für Host-Ausgaben angehalten; möglicher Datenverlust am Target ist bereits Teil des Systemmodells.

**Verbindliche Regel:** Ein schneller Dispatcher legt Daten in **eine begrenzte Queue pro Ausgang**; ein unabhängiger Worker schreibt diese Daten. Für einen TCP-Listener erhält **jeder Client** eine eigene Queue. **Bei Platzmangel wird die gesamte noch wartende Queue dieses Ausgangs verworfen und anschließend mit neuen Daten weitergemacht.** Es wird niemals auf das Freigeben von Pufferplatz gewartet. Bereits im Betriebssystem oder im laufenden `Write()` befindliche Bytes können durch Queue-Leeren nicht zurückgeholt werden.

| Situation | Ohne Entkopplung | Mit begrenzter eigener Queue (Default) |
|---|---|---|
| Empfänger kurzzeitig langsam | Er kann das ganze `tlog` bremsen | Queue fängt die kurze Verzögerung auf |
| Empfänger dauerhaft langsam | Rückstau bis zum Target möglich | Nur die betroffene Queue wird wiederholt geleert; bei hängendem `Write()` wird der Client getrennt bzw. der Sink deaktiviert |
| Sehr großer Puffer | Weniger kurzfristige Überläufe, mehr RAM und verzögerte Verlustanzeige | Auch ein großer Puffer hilft **nicht** bei dauerhaft zu geringer Leserate |
| Sehr kleiner Puffer | Wenig RAM | Frühere Verluste bei kurzen Lastspitzen |

**Noch offen:** Praktische Queue-Größe und sinnvolle Begrenzung (Bytes/Frames/Events), Schreib-Timeouts und Wiederverbindung nach einem **hängenden** Empfänger. **Nicht offen:** Die Überlaufstrategie ist immer `clear-queue`: sämtliche wartenden Elemente des betroffenen Ausgangs verwerfen. Es gibt keine wählbaren alternativen Überlaufstrategien und insbesondere keine blockierende Ausgabe.

**Raw-Besonderheit:** Rohdaten sind typischerweise bereits COBS-/TCOBS-geframet und enthalten üblicherweise Cycle Counter. Ein `Read()`-Chunk kann dennoch mitten in einem Frame enden. Die Queue darf daher zwar vollständig geleert werden, **die neue Ausgabe muss nach einem Verlust möglichst an einer gültigen Frame-Grenze einsetzen**; dafür ist eine leichte Frame-Grenzenerkennung oder ein geeigneter Reset/Reconnect notwendig. Sonst kann der Empfänger bis zur nächsten vollständigen Begrenzung zunächst einen beschädigten Frame sehen. Cycle Counter helfen, fehlende Records zu erkennen; sie ersetzen keine fehlenden Daten. Ein Flush meint hier **verwerfen**, nicht das aufwendige Ausschreiben des Puffers.

**Verbindliche Konsequenz:** Es gibt keine Garantie verlustfreier Host-Aufzeichnung und **keinen strengeren blockierenden Modus**. Wenn ein Ausgang nicht mithält, verliert ausschließlich dieser Ausgang wartende Daten. Das Target und andere Ausgänge dürfen dadurch nicht ausgebremst werden.

## 5. Empfohlene Fehlerbehandlung

| Fall | Default-Vorschlag | Sichtbarkeit |
|---|---|---|
| Ungültige CLI / nicht öffnbarer zwingender `-port` | Start verhindern | Klarer Fehler, Exit-Status |
| TCP-/DS-Client verbindet sich nicht oder bricht ab | Nur betroffenen Ausgang deaktivieren oder erneut verbinden; andere laufen weiter | Warnung und Status |
| Raw-TCP-Client ist dauerhaft langsam | Bei Queue-Überlauf **nur dessen wartende Queue leeren**; hängt der aktuelle Socket-Write, Timeout/Verbindung schließen | Queue-Reset-, Abbruch- und Verlustzähler |
| UDP-Ausgabe nicht zustellbar | Best effort; bekannte lokale Schreibfehler zählen | Zähler/Warnung; Netzwerkverluste nicht zuverlässig messbar |
| Datei-Queue voll | **Nur diese Queue vollständig leeren**; Datei als lückenhaft markieren, danach weiterschreiben, sofern Sink gesund | Drop-/Reset-Zähler, deutliche Lückenmeldung |
| Datei voll oder Schreibfehler | Betroffenen Dateiausgang deaktivieren; niemals den Target-Empfang anhalten | Fehlerdiagnose, unvollständige Aufnahme markieren |
| Dekodierte Text-/JSON-Ausgabe überlastet | Nur dessen vollständige wartende Queue leeren; neue Events weiter annehmen | Drop-/Reset-Zähler (Events/Bytes) |
| Mehrere `-ctrl` senden zugleich | Target-Schreiboperationen serialisieren; niemals Bytes vermischen | Pro Request Fehler/Status |
| Target-Verbindung fällt aus | Bestehendes Reconnect-Verhalten erhalten bzw. getrennt spezifizieren | Quellfehler; `-out`-Fehler davon unterscheiden |

**Diagnosen dürfen Raw-Daten und maschinenlesbare JSON/KV-Streams nicht beschädigen.** Fehler und Drop-Zähler auf `stderr`/einem separaten Diagnosekanal; periodisch oder bei Zustandswechsel (kein Warn-Sturm pro verworfenem Byte). Ein erfolgreicher Write zum Target bestätigt **nicht** die Ausführung eines ABC-Kommandos.

**Keine Ausnahme für Aufzeichnungen:** Weder `overflow=block` noch `reliability=strict` werden vorgesehen. Bei blockiertem `Write()` muss der betreffende Sink über einen Timeout, das Schließen der Verbindung oder das Deaktivieren des Ausgangs wieder begrenzt bearbeitbar werden. Ein Queue-Reset allein kann einen schon blockierten Systemaufruf nicht lösen. Diagnosen erfolgen gedrosselt, nicht einmal pro verworfenem Event.

## 6. Hat `trice ds` heute einen eigenen Filter?

**Nein, keinen eigenen Trice-Log-Level-/Tag-Filter.** Der aktuelle `trice ds`-Prozess erhält bereits vom jeweiligen Logger ausgewählte und fertig aufbereitete Logzeilen über Go-RPC und zeigt sie an. Die DS-Seite bietet eine Farbpalette. Die RPC-Methode `LogSetFlags` verändert die Flags der Go-`log`-Bibliothek, **nicht** `-logLevel`, `-pick` oder `-ban` für Trice-Events.

Damit bleibt die Filterentscheidung **in jeder `tlog`-Instanz**, nicht im Displayserver. Wer im DS *alle* Events haben will, muss den jeweiligen Logger ungefiltert an den DS senden; für abweichende parallele Sichtweisen sind weitere `tlog`-Instanzen über denselben Raw-Stream vorgesehen.

Ein Displayserver, der mehrere Quellen nur als Zeilen aggregiert, darf weder voraussetzen, dass IDs global eindeutig sind, noch vorgeben, nachträglich zuverlässig typisierte Trice-Parameter filtern zu können.

## 7. Bestand und Migration

| Heute | Perspektive | Status |
|---|---|---|
| `-port` / `-p`, `-args`, UART-/RTT-Parameter | **Behalten**; ein Eingang | Festgelegt |
| `-binaryLogfile` / `-blf` | `-out ...;format=raw` nach Datei | Ersatz planen, bestehende Append-Semantik beachten |
| `-logfile` / `-lf` | `-out ...;format=text` nach Datei | Ersatz planen |
| `-tcp` | `-out` nach TCP mit explizitem Listener | Ersatz planen; altes Verhalten nicht kommentarlos ändern |
| `-ds` / `-displayserver` beim Logger | `-out` nach DS | Ersatz planen |
| `-ipa`, `-ipp` (auch bei `ds`/`shutdown`) | Ein Endpoint, z. B. `-listen ds://...`, `-connect ds://...` | Migration getrennt prüfen |
| `-vis` | Transformations-/Formatierungsoption von `-out` | Ersatz planen; keine zusätzliche allgemeine Filterung |
| `-logFormat` | Format je `-out` | Ersatz planen, globaler Filter bleibt |
| Lokale Target-Tastatursteuerung bei `-ds` | Wiederholbares, targetbezogenes `-ctrl` | Neues Protokoll festlegen |
| `-logLevel`, `-pick`, `-ban` | **Eine globale Auswahl für alle dekodierten Ausgaben** | Festgelegt |
| `-debug`, `-showInputBytes`, Statistiken | Diagnosefunktionen; kein eigener Ausgangsfilter | Bestand berücksichtigen |

Bestehende Schalter zunächst vollständig kompatibel belassen, Alt- und Neumodus gegeneinander testen, erst danach deprecaten und mit dokumentierter Übergangsfrist entfernen. Die genaue Versionspolitik ist offen. Ein alter DS-Go-RPC-Endpunkt lässt sich nicht automatisch durch irgendeinen JSON-Stream ersetzen; entsprechende Kompatibilität braucht einen Adapter oder einen bewussten Protokollwechsel.

## 8. Kleine verbleibende Entscheidungen vor der Implementierung

1. Konkrete `-out`-/`-ctrl`-Grammatik, Fehler bei doppelten Zielen, Default-stdout, Dateimodus (`append`/`truncate`) und geordneter Shutdown. `flush` bei Überlauf heißt **Queue verwerfen**, nicht zum blockierten Sink ausschreiben.
2. Queue-Standardgröße und technische Umsetzung von `clear-queue` **pro Sink bzw. pro TCP-Client**; keine Varianten für blockierendes oder gezielt teilweises Verwerfen.
3. Raw-Frame-Semantik bei Chunkgrenzen, UDP-Datagrammen, Reconnect und sichtbaren Lücken.
4. Einheitliches Diagnoseschema: `dropped_bytes`, `dropped_events`, `dropped_frames`, `queue_resets`, `client_disconnects`, `write_errors`, `queue_high_watermark`; soweit jeweils messbar.
5. `-ctrl`-Requestformat, Serialisierung, lokale Sicherheitsvorgaben (Listener standardmäßig Loopback), Rechte und Antwort-/Timeout-Semantik.
6. DS-Endpoint-Syntax und Kompatibilität des bisherigen RPC-Protokolls.

**Nicht wieder öffnen ohne neuen Anwendungsfall:** mehrere `-port`, pro-Ausgang-Logfilter, Dekodierung im DS, direkte DLT-Abhängigkeiten im `tlog`.

## 9. Geprüfte Quellen (Repository `main`, 17.11.2026)

- [`internal/args/init.go`](https://github.com/rokath/trice/blob/main/internal/args/init.go) – aktuell registrierte CLI-Optionen einschließlich `-debug`, `-vis`, `-ds`, `-ipa`, `-ipp`.
- [`internal/args/handler.go`](https://github.com/rokath/trice/blob/main/internal/args/handler.go) – Logpfad, Filterbedingungen und Input-Setup.
- [`internal/receiver/receiver.go`](https://github.com/rokath/trice/blob/main/internal/receiver/receiver.go) – Raw-Logging vor Dekodierung; File-Reader und TCP/UDP.
- [`internal/emitter/server.go`](https://github.com/rokath/trice/blob/main/internal/emitter/server.go) – DS-RPC-Methoden; keine Tag-/Level-Filter.
- [`internal/emitter/remoteDisplay.go`](https://github.com/rokath/trice/blob/main/internal/emitter/remoteDisplay.go) – Weitergabe formatierter Zeilen an DS.
- [`docs/rm/cli.md`](https://github.com/rokath/trice/blob/main/docs/rm/cli.md) – bestehendes CLI-Verhalten und binäre Logdatei.
- [`docs/_scratchPad/handsoff_tlog_out.md`](https://github.com/rokath/trice/blob/main/docs/_scratchPad/handsoff_tlog_out.md) – ältere Entwurfsgrundlage; **deren per-Ausgang-Filter-Konzept wird durch dieses Dokument ausdrücklich verworfen**.
