# Handoff: Universelle tlog-Ausgaben mit wiederholbarem `-out`

> **Projekt:** [rokath/trice](https://github.com/rokath/trice) (`tlog` / `trice log`).  
> **Stand:** 13. November 2026.  
> **Status:** Konsolidierter Architektur- und Design-Handoff, **keine Implementierungsfreigabe**.  
> **Vorgesehener Repository-Pfad:** `docs/_scratchPad/handsoff_tlog_out.md`.  
> **Ablösung:** Dieses Dokument ist als **vollständiger inhaltlicher Nachfolger** von `docs/_scratchPad/handsoff_tlog_CLI_Architektur.md` gedacht. Die früheren Entscheidungen, offenen Fragen, technischen Befunde und Demo-Randbedingungen sind hier integriert. Das alte Dokument muss nach Übernahme nicht mehr als Arbeitsgrundlage gelesen werden. **Nicht eigenmächtig löschen oder umbenennen.**

## 0. Arbeitsauftrag und Grenzen

Zuerst ein konsistentes **Design** für die neue Ausgabearchitektur erstellen, offene Entscheidungen sichtbar machen und die Auswirkungen auf Bestandsfunktionen und Tests klären. **Nicht** vorzeitig `tlog`, MCU-Firmware, DLT-Adapter, neue APIs, Bifurkationsdemo oder UI implementieren. Dies ist ein Architektur-Handoff, kein Auftrag zur sofortigen Migration des Repositories.

Die bisherigen Erkenntnisse stammen aus zwei Arbeitssträngen:

1. Die ältere Diskussion zur universellen CLI-/Kommunikationsarchitektur, `-tcp`-Ablösung, `-api`/`-control` und ABC-Injection; Quellenstand am 10.11.2026: Branch `wip`, Commit [`f713ceab`](https://github.com/rokath/trice/commit/f713ceab876bdbaeaffe862745743632a6d5a63e).
2. Die Diskussion vom 13.11.2026 über eine **separate Trice-DLT-Bridge** und vor allem einen **mehrfach verwendbaren `-out`-Schalter**. `tlog` soll keinerlei DLT-Abhängigkeit bekommen. Aktuelle Dokumentation und exemplarische Dateien unter `main` wurden erneut abgeglichen. `main` kann sich nach diesem Stand ändern.

Begriffe im Folgenden:
- **Festgehaltene Zielrichtung:** Im Gespräch gewünscht, aber noch keine technische Freigabe.
- **Empfehlung:** Konkreter Designvorschlag, zu prüfen.
- **Offen:** Bewusst nicht entschieden; bei späterer Umsetzung erst festlegen.
- **Ist:** Am angegebenen Quellstand beobachtetes Verhalten, nicht notwendigerweise das Ziel.

---

# Teil I - Zielbild

## 1. Kernentscheidungen aus der Diskussion

**Festgehaltene Zielrichtung**

1. `-port` / `-p` bleibt die Auswahl der **einen Trice-Datenquelle/Target-Verbindung**. Es darf nicht zur Beschreibung beliebiger externer Ausgabekanäle umgedeutet werden.
2. `tlog` dekodiert empfangene Trice-Records **einmal** und kann die resultierenden Ereignisse auf **mehrere unabhängige Ausgaben** verteilen.
3. Ein **universeller, wiederholbarer `-out`-Schalter** konfiguriert diese Ausgaben. Kein weiterer Spezialschalter für jedes neue Ausgabegerät oder Protokoll.
4. **Jeder Ausgang** besitzt seine eigene Auswahl, Aufbereitung und Zielverbindung. Eine lokal nur auf WARN/ERROR begrenzte Anzeige darf **nicht** die an einen externen Viewer exportierten Trices begrenzen.
5. Eine **DLT-Bridge ist eine eigenständige Applikation**. Sie konsumiert Trice-Ereignisse, erzeugt DLT-Nachrichten und beantwortet DLT-Control-Requests. Weder DLT-Header noch DLT-Services gehören in `tlog` oder Target-Firmware.
6. **Erste DLT-Ausbaustufe ohne Injection**. Der Adapter kann DLT-Log-Level als eigene Viewer-seitige Filter interpretieren; das ändert weder den Target-Datenstrom noch zwingend die `tlog`-Konsolenausgabe.
7. Der vorhandene Schalter `-tcp` ist ein **Ablösekandidat**, nicht die Grundlage für eine neue API. `-ds`, `-vis` und `-logfile` werden nicht voreilig entfernt.
8. Ein späterer **Eingabe-/Steuerkanal** für externe Apps (`-api`/`-control`) ist ein **separates Thema**. Output-Push und Kommando-/Antwort-Semantik dürfen nicht versehentlich vermischt werden.

**Keine Entscheidung** über die endgültige `-out`-Grammatik, das genaue Lebenszyklus- und Fehlerverhalten, Dateisemantik, API-Namen oder Deprecation-Termine.

## 2. Zielarchitektur

```text
                                 HOST / tlog
 Target -- Trice binary --> source (-port/-p, -args)
                                     |
                            framing / decoding
                            TIL / LI / SL / CE
                                     |
                     successfully decoded Trice event
                           (one event per call)
                                     |
                         +-----------+-----------+
                         |           |           |
                     output A    output B    output C       ... repeat
                     selector    selector    selector          -out
                     formatter   formatter   formatter
                     sink        sink        sink
                         |           |           |
                       stdout    TCP/NDJSON    file/CSV
                                     |
                             independent DLT bridge
                                     | DLT/TCP
                                 DLT Viewer

 Independent future path (NOT part of -out):
 External apps -- optional -api/-control --> request validation
        --> symbolic Trice/ABC encoding --> target TX via -port
```

**Entscheidende Schichtengrenze:** Der Fan-out geschieht **vor** lokalen `-logLevel`/`-pick`/`-ban`-Filtern, vor der textuellen Zeilenzusammensetzung und vor `-vis`-`log=drop`-Effekten. Ein Ausgang filtert seine Kopie eines Ereignisses, nie das zentrale Ereignis.

Die Bibliothek sollte ein unveränderliches, intern nutzbares Event-Modell besitzen. Die existierende JSON-Ausgabe ist ein möglicher Serializer, **nicht** zwangsläufig das interne Datenmodell.

## 3. `-out`: wiederholbar, in sich geschlossen, generisch

**Empfohlene Grundidee:**

```text
-out '<sink-spec>;format=<format>;selector-options;transform-options'
-out '<sink-spec>;format=<format>;selector-options;transform-options'
...
```

Noch **keine verbindliche Syntax**. Folgende Beispiele illustrieren ausschließlich die Semantik (die Konfiguration ist in aktuellen Releases **nicht** implementiert):

```sh
tlog -p COM7 \
  -out 'sink=stdout;format=text;level=warning' \
  -out 'sink=tcp-listen://127.0.0.1:9100;format=json;level=all' \
  -out 'sink=file:all-events.ndjson;format=json;level=all'
```

```sh
# Mögliche spätere Abbildung eines vorhandenen Spezialfalls:
tlog -p COM7 \
  -out 'sink=udp://127.0.0.1:9000;format=vis;select=motor;expr=printf("%d,%d\n",v0,v1)' \
  -out 'sink=stdout;format=text;level=warning'
```

Der Ausdruck im zweiten Beispiel ist **schematisch**. Die vorhandene `-vis`-Syntax soll nicht durch ein ungeprüftes neues Escaping- oder Parserformat gebrochen werden. `stdout`, Datei, TCP-Listener, UDP-Ziel, `trice ds` und spätere Ziele sollen als Sink-Adapter modelliert werden, nicht als verschiedene Logikpfade.

**Empfohlene Begriffsordnung:**

| Element | Aufgabe | Beispiele |
|---|---|---|
| `-port` + `-args` | **Woher** kommen Trice-Bytes? | `COM7`, `J-LINK`, `TCP4`, `UDP4`, `FILEBUFFER` |
| wiederholbares `-out` | **Wohin** und **wie** gehen dekodierte Events? | `stdout`, `file:`, `tcp-listen://`, `udp://`, `ds://` |
| `-api` oder `-control` (später) | **Welche externen Anforderungen** darf `tlog` entgegennehmen? | `inject`, `status`, `config`, ggf. `subscribe` |
| externer DLT-Adapter | **Wie** wird das generische Event-Protokoll in DLT übersetzt? | DLT v1/2, DLT-Control, DLT-Viewer |

Ein Sink wird durch seine **Rolle** beschrieben, nicht bloß durch sein Protokoll. `tcp-listen://` (Server, `tlog` nimmt Clients an) ist etwas anderes als `tcp-connect://` (Client, `tlog` verbindet sich). Diese Richtung darf nicht implizit bleiben.

### 3.1 Unabhängige Filter pro Ausgang

Jeder `-out` benötigt konzeptionell:

- Prioritätsschwelle (`level`/`weight`) einschließlich `all` und `off`;
- optional `pick` **oder** `ban` mit der heutigen Trice-Semantik (inkl. wiederholter Tags/Gewichte), möglichst wiederverwendete Selektorlogik;
- `-utag`-Definitionen als globale **Klassifikationseingabe**; die eigentliche Auswahl bleibt pro Ausgang;
- optional spätere Selektoren für ID, Quelle/Target, SL/CE-Felder oder reguläre Ausdrücke - **nicht** stillschweigend als vorhandene Funktion deklarieren.

Die Trice-Gewichte reichen von `0..999`, **höhere Werte bedeuten höhere Priorität**. Die bestehenden Tag-Kategorien und die per Gewicht abgeleiteten Level müssen korrekt bleiben. `-logLevel`, `-pick` und `-ban` beeinflussen im heutigen Code globale Anzeigewege; dies ist **Ist-Zustand, keine Vorgabe** für die neue Architektur.

**Beispiel:** Konsole `level=warning`, DLT-Export `level=all`, CSV-Plot nur `motor`. Der Konsole unbekannte DEBUG-Trices müssen trotzdem bei DLT ankommen. Bei nachträglich geänderten DLT-Viewer-Filtern können selbstverständlich nur Ereignisse ab dann neu angezeigt werden, die zuvor bis zur Bridge gelangten bzw. dort gepuffert wurden.

### 3.2 Event-Grenzen und Informationen

**Ein Event ist eine dekodierte Trice-Nachricht**, nicht zwingend eine fertige Terminalzeile. Aus mehreren Trice-Calls zusammengesetzte Zeilen sind ein **Formatierungsproblem des jeweiligen Ausganges**. Mehrzeilige Calls bleiben ein Ereignis. Damit können Textausgabe und NDJSON ohne gegenseitigen Einfluss arbeiten.

Für einen generischen Event-Ausgang sind relevant:

| Feld | Herkunft / Regel |
|---|---|
| `tag`, effektives Gewicht, abgeleiteter `level` | Tag-Klassifikation, `-utag`, keine ANSI-Farbinformation als Grundlage |
| `message` | Formatierter Text ohne ANSI/Terminal-Präfixe |
| `id` | Wenn im Trice-Record vorhanden, sonst **weglassen** |
| `file`, `line` | Nur bei verfügbaren/passenden LI-Daten |
| Host-Eingangszeit / Reihenfolge | Sauber definierte Zeitbasis und Reihenfolge; keine erfundenen Target-Zeitstempel |
| Target-Stamps, Deltas | Rohwert, Bitbreite, bekannte Einheit bzw. explizite Unbekanntheit; kein blindes Umdeuten als UTC |
| SL/CE-Felder | Typisiert, nicht aus Bildschirmtext rückgeparst |
| `source` / Target-Verbindung | Bei Aggregation und mehreren Quellen für die Unterscheidung erforderlich |
| Ereignistyp / Herkunft | Anwendung vs. Decoderdiagnostik vs. später TX-Status |

Das aktuell vorhandene `-logFormat json` liefert NDJSON mit `tag`, `level`, `message` und zusätzlichen Metadaten. Es ist für einen ersten Adapter ausreichend, sofern seine Optionen die benötigten Felder einschalten. **Nicht alle oben genannten Felder sind bereits Bestandteil des heutigen JSON-Vertrags**; insbesondere einen vollständigen Rohstempel oder eine fest zugesicherte `source`-Eigenschaft nicht unterstellen. Schema-Erweiterungen brauchen eine explizite Kompatibilitätspolitik.

`CHAR`/`DUMP` verfügen nicht über saubere TREX-Eventgrenzen und unterstützen daher derzeit keine strukturierte JSON/KV-Ausgabe. `typeX0`-Meldungen können ohne Trice-ID vorkommen. Fehler beim Decodieren sind **Diagnosen**, keine normalen `untagged`-Applikationsereignisse. Einen gesonderten Diagnoseausgang bzw. `stderr` erwägen; diese Nachrichten nicht versehentlich als gewöhnliche DLT-Applikationslogs ausgeben.

### 3.3 Formate und Transformationen

**Format** bestimmt die Serialisierung (`text`, `json`/NDJSON, `kv`, zukünftig eventuell `csv`), **Transformation** die fachliche Ableitung (z. B. `-vis`-Ausdrücke aus numerischen Rohwerten), **Sink** die Übertragung. Nicht alles in einen monolithischen Formatter zusammenziehen.

Für JSON: ein vollständiges Objekt pro Trice-Event, UTF-8, mit `LF` getrennt, auch bei mehrzeiligem `message`. Keine ANSI-Farbsequenzen; Diagnosen separat. Für Text: die bisherigen Prefix-/Suffix-, Color-, Timestamp- und Zeilenfunktionen wahren. Für CSV/Vis: Datenspalten, Einheiten, Header, Rechenoperationen und Ziele explizit.

Die bisherige `-vis`-Syntax und ihr Umfang (`v0..v11`, `id`, `ts16`/`ts32`/`ts`, Formelausdrücke, Datei/UDP, wiederholbare Regeln) sind ein nützlicher vorhandener Transformationsmotor. **Sie ist heute nach `-pick`/`-ban` geschaltet**, arbeitet synchron und `log=drop` kann die normale Ausgabe unterdrücken. Ein neues `-out`-Design sollte beides **entkoppeln**, ohne die alten Aufrufe unbemerkt umzudeuten.

### 3.4 Sink-Verhalten und Fehlersemantik

Vor einer Implementierung festlegen:

1. **Standard-stdout:** Was geschieht ohne `-out`, mit erstem `-out`, mit `-out sink=stdout`? Empfehlung: ohne neues Flag das alte Verhalten exakt erhalten; bei expliziten `-out`-Angaben muss Default-stdout klar definiert sein und darf nicht doppelt erscheinen.
2. **Globale Altfilter:** Bestehende `-logLevel`/`-pick`/`-ban` gelten zunächst weiter für die Legacy-Ausgabe. In neuen `-out`-Ausgängen sollen explizite lokale Selektoren Vorrang haben. Kombinationen und widersprüchliche Angaben müssen dokumentiert und getestet werden.
3. **Langsame Empfänger:** Ein blockierter TCP-Client darf Decoder, Terminal und andere Ausgänge nicht unkontrolliert blockieren. Empfehlung: entkoppelte, **begrenzte** Sink-Queues, definierte Drop- oder Trennstrategie und sichtbare Verlustzähler. Keine unbegrenzten Queues.
4. **Zustellung:** Per-Sink Reihenfolge erhalten, aber **keine** unbegründete End-to-End-Garantie. TCP-Write bedeutet nicht, dass der Viewer die Nachricht dargestellt hat. UDP bleibt best effort.
5. **Mehrere TCP-Clients:** Ein Listener kann mehrere Viewer versorgen; Verbindung nach Disconnect weiter akzeptieren. Pro Client Filter/Queue oder gemeinsame Sink-Konfiguration? **Offen**.
6. **Dateien:** `append` vs. `truncate`, Namenskonflikte bei mehreren `-out`, gemeinsame File-Handles und Header-Ausgabe einmal pro Datei explizit festlegen. Alte `-logfile`- und `-vis`-Semantik nicht brechen.
7. **Start/Stop:** Fehlkonfiguration vor dem Öffnen des Target-Ports diagnostizieren; wer einen Sink-Fehler stoppt, wer nur den Sink deaktiviert, und wie ein sauberer Shutdown/Flush erfolgt, muss spezifiziert werden.
8. **Netzwerksicherheit:** Listener standardmäßig nur Loopback. Andere Interfaces nur nach ausdrücklicher Konfiguration. Eingehende Steuerschnittstellen brauchen separate Sicherheitsregeln.
9. **Leistung:** Unter Last messen: einmaliges Decodieren, viele Ausgänge, Teilsynchronisierung, Memory-/CPU-Overhead, korrekte Behandlung von Backpressure und ungebremsten Targets.

---

# Teil II - Ist-Zustand und Migration

## 4. Vorhandene Kommunikationswege

Die Bestandsaufnahme vom 10.11.2026 auf `wip/f713ceab` und der erneute Abgleich mit den nachstehend verlinkten `main`-Dateien zeigen:

| Ist-Option | Tatsächliche Aufgabe | Bedeutung im Zielbild |
|---|---|---|
| `-p` / `-port`, `-args` | Eine Target-Quelle: UART, RTT, Datei, `TCP4`, `UDP4`, ... | **Bleibt** unverändert als Input |
| `-port TCP4` | `tlog` ist **TCP-Client** eines fremden Servers mit binären Trice-Daten, prinzipiell schreibfähig | Nicht mit TCP-Ausgabe verwechseln |
| `-port UDP4` | `tlog` ist **UDP-Empfänger** für binäre Trice-Daten, ohne verfügbaren Target-Reply-Peer | Kein generischer Kommandoeingang |
| `-tcp ADDR` | **TCP-Listener** für einen externen Text/JSON-Empfänger; heute ein Client, blockierender Verbindungsaufbau mit initialem Read | Über `-out` ersetzen, aber erst nach Test/Migration |
| `-ds` / `-displayserver` | `tlog` ist **RPC-Client** eines separat gestarteten `trice ds`; mehrere Logger können an eine Anzeige senden | Kann später ein `-out`-Sink werden; Bestand vorerst erhalten |
| `-vis` | Wiederholbare numerische Auswahl/Transformation zu Datei oder **UDP-Ziel**, `log=keep/drop` | Transformations- und Sink-Modul wiederverwenden, nicht vorschnell entfernen |
| `-logfile` | Ausgabedatei, mit bestehender Append-/Autoname-Logik | Auf `-out`-Datei abbilden, Bestandssemantik sichern |
| `-logFormat text/json/kv` | Format der vorhandenen regulären Ausgaben; NDJSON ist heute filterabhängig | Für neue Outputs **pro Ausgang** statt nur global |
| `-logLevel`, `-pick`, `-ban` | Bisherige Hostfilter; `-vis` läuft nach Auswahl | In lokale Selektoren pro `-out` überführen |

**Korrektur zur Kommunikation:** `trice ds` ist ein Go-`net/rpc`-Displayserver; die dokumentierte Tastatureingabe bei `-ds` stammt aus dem lokalen `stdin` des Loggers und schreibt auf die über `-port` geöffnete Target-Verbindung. Das ist **keine** generische Netzwerk-Injection-API von `trice ds`.

**Eingang und Ausgang niemals aufgrund des Wortes TCP vertauschen:**

```text
-port TCP4          tlog  --connect--> Target-TCP-Server  (Trice binär)
-port UDP4          tlog  <--datagrams-- Trice-UDP-Sender (RX only)
-tcp HOST:PORT      tlog  <--connect--   Text-TCP-Client (Legacy, 1 Client)
-ds                 tlog  --RPC------->  trice ds
-vis @udp://...     tlog  --datagrams->  Visualisierungs-Client
-new -out ...       tlog  ----------->  pro Ausgang definierter Sink
-future -api ...    tlog  <---------->  externe steuernde Anwendung
```

### 4.1 Code- und Dokumentationsbelege

- [`internal/args/init.go`](https://github.com/rokath/trice/blob/main/internal/args/init.go): aktuelle Flag-Registrierung, wiederholbares `-vis`, globales `-logFormat`, `-tcp` und `-ds`.
- [`internal/do/do.go`](https://github.com/rokath/trice/blob/main/internal/do/do.go): aktueller TCP-Writer mit `Accept()` und initialem `Read()` sowie `io.MultiWriter`.
- [`internal/receiver/receiver.go`](https://github.com/rokath/trice/blob/main/internal/receiver/receiver.go): `TCP4` als Client, `UDP4` als Empfänger ohne Reply-Adresse.
- [`internal/vis/router.go`](https://github.com/rokath/trice/blob/main/internal/vis/router.go): `-vis`-Routing, Datei-/UDP-Sinks, gemeinsame Sinks und `Process()`.
- [`internal/emitter/server.go`](https://github.com/rokath/trice/blob/main/internal/emitter/server.go), [`internal/emitter/remoteDisplay.go`](https://github.com/rokath/trice/blob/main/internal/emitter/remoteDisplay.go): RPC-Displayserver und Client.
- [`docs/rm/cli.md`](https://github.com/rokath/trice/blob/main/docs/rm/cli.md), [`docs/rm/visualization.md`](https://github.com/rokath/trice/blob/main/docs/rm/visualization.md), [`docs/rm/structured_logging.md`](https://github.com/rokath/trice/blob/main/docs/rm/structured_logging.md), [`docs/rm/tags_color_and_weights.md`](https://github.com/rokath/trice/blob/main/docs/rm/tags_color_and_weights.md): dokumentierte Nutzersemantik.

Die Quellen unter `main` sind **beweglich**; beim tatsächlichen Implementieren Commit-Hashes fixieren und den Zustand erneut prüfen. Die ältere Codeanalyse ist unter [`f713ceab`](https://github.com/rokath/trice/commit/f713ceab876bdbaeaffe862745743632a6d5a63e) reproduzierbar.

## 5. Migrationsstrategie, ohne Bestandsbruch

**Vorgeschlagene Reihenfolge, keine Freigabe:**

1. Eine verbindliche Kurzspezifikation von `-out` erstellen: Syntax/Quoting, Filter-Semantik, vorhandene Legacy-Optionen, Start-/Fehlerverhalten, Speicherlimits, mehrere Clients, Kompatibilität.
2. Intern **dekodierte Events vor globalen Anzeige-Filtern** identifizieren; Event-/Selektor-/Formatter-/Sink-Schichten definieren. Bestehende Tests als Referenz sichern.
3. Zuerst **neue Ausgaben** (z. B. `stdout:text`, `file:json`, `tcp-listen:json`) ergänzen. Legacy-Modus unangetastet lassen; Ausgabe bei gleichem Setup byteweise/semantisch vergleichen.
4. Danach `-vis` und `-ds` als Adapter an die gemeinsame Architektur anbinden, falls das ohne Verlust ihrer speziellen Funktionalität möglich ist. `-vis`-Filterreihenfolge und `log=drop` ausdrücklich behandeln.
5. **Erst dann** das obsolet werdende `-tcp` mit gleichwertigem Plain-TCP/PuTTY-Ersatz als deprecated kennzeichnen, Tests, Hilfe, Beispiele und Release Notes aktualisieren; Entfernung erst nach geregelter Frist.
6. **Später, getrennt:** generische `-api`/`-control`-Schnittstelle und ABC-Kommandos. Kein Eingabekanal als Nebeneffekt von `-out`.
7. **Unabhängig davon:** DLT-Bridge als externe Anwendung entwickeln; kein DLT-Code in `tlog`.

Offen ist, wie `-out` und globale Altoptionen kombiniert werden. **Kandidaten**: Legacy-Schalter konfigurieren nur eine implizite Standardausgabe oder sie sind bei explizitem `-out` verboten/mit Warnung versehen. Das ist zuerst anhand Nutzerfreundlichkeit und Rückwärtskompatibilität zu entscheiden.

Zu beachten: Der bisherige `-tcp`-Empfänger ist ein **Plain-Text-/Plain-JSON-Client ohne Subscribe-Handshake**. Ein neues `-api`-Protokoll mit `{"op":"subscribe"}` wäre **kein** gleichwertiger Ersatz für PuTTY; `-out sink=tcp-listen://...;format=text` könnte hingegen ein klarer Ersatz sein, sofern Verbindungssemantik und Verhalten kompatibel sind. Die Nutzungsfrequenz des Altflags ist **nicht bekannt**.

## 6. Test- und Abnahmekatalog

Der Handoff ist erst zur Implementierung geeignet, wenn die nachstehenden Tests konzeptionell spezifiziert sind.

| ID | Prüfung | Erwartung |
|---|---|---|
| O01 | Kein `-out`; alter `tlog`-Aufruf | Bisherige Ausgabe und Fehlerbehandlung unverändert |
| O02 | Zwei `-out`, lokale Konsole nur `warning`, externer JSON-Stream `all` | DEBUG-Ereignisse im JSON, nicht in der Konsole |
| O03 | Wiederholte/wechselnde `-pick`, `-ban`, numerische Gewichte, `-utag` | Je Sink korrekte und unabhängige Auswahl; bisherige OR/AND-Regeln gewahrt |
| O04 | Mehrzeiliges Event / mehrere Trices in einer Textzeile | NDJSON: ein Datensatz pro Trice; Text: bekannte Zeilendarstellung |
| O05 | `typeX0`, unbekannte ID, invalides Framing, `CHAR`/`DUMP` | Kein gefälschtes Anwendungsereignis, sauberer Diagnosepfad |
| O06 | TIL-/LI-Neuladen während Loglauf | Nachfolgende Events verwenden konsistente neue Daten; kein unkontrollierter Crash |
| O07 | `-vis`, `log=drop`, CSV/Header, UDP | Keine unerwartete Beeinflussung anderer Ausgänge; Legacy-Verhalten bleibt erhalten |
| O08 | Zwei TCP-Viewer, langsamer Viewer, Disconnect und Reconnect | Andere Sinks bleiben nutzbar; definierte Queue-/Verlust-Semantik |
| O09 | Gleichzeitiges stdout, Datei, DS und TCP | Ausgabe- und Ereignisreihenfolge pro Sink konsistent; kein Double-Print |
| O10 | Ziel-Datei bereits vorhanden, mehrfach dieselbe Datei | Explizite Append-/Truncate-, Sharing- und Header-Semantik |
| O11 | `-port TCP4`, `-port UDP4`, `-tcp`, `-ds` parallel zu neuen Ausgaben | Verbindungsrichtung bleibt klar; keine Regression des Target-Inputs |
| O12 | Bei TCP Listener externes Binding / lokale Firewall | Sicherer Loopback-Default, ausdrückliche Freigabe für externe Interfaces |
| O13 | Fehlschlag eines Sinks bzw. Abbruch des Targets | Definierter Shutdown, Flush, Diagnostik und Exit-Code |
| O14 | Hohe Ereignisrate und große SL/CE-Werte | Speicher begrenzt, Backpressure dokumentiert, Vergleichsmessung |
| O15 | `tlog`-Output an externe Bridge | Getrennte Prozesse, Trice-Protokoll nicht geändert, DLT im Target unnötig |

**Abnahme der Dokumentation:** CLI-Hilfe, Logging-Handbuch, `-vis`-Beispiele, Migrationstabelle, Testanleitungen, Changelog und gegebenenfalls eigenes Schema-Dokument sind konsistent. Bei `-out` ohne explizite Angaben für Netzwerk oder Fehlerstrategie werden keine fragwürdigen Defaults verschwiegen.

---

# Teil III - DLT-Bridge als Anwendungsfall, nicht als tlog-Funktion

## 7. Eigenständige DLT-Anwendung

```text
Target --Trice--> tlog --alle dekodierten Events/NDJSON-->
  trice-dlt-bridge --DLT/TCP--> COVESA DLT Viewer
                         <----DLT Control Requests----
```

Die Bridge kann auf demselben PC laufen, über `stdin`/Pipe oder einen generischen `-out`-TCP-Stream lesen und einen DLT-v1-TCP-Server bereitstellen. So bleibt `tlog` frei von DLT-Bibliotheken, -Headern, -Service-IDs, -Control-Handlern und Viewer-spezifischen Entscheidungen. Ein späterer anderer Viewer könnte dasselbe generische Output-Protokoll konsumieren.

**MVP ohne Injection:**

- DLT-v1-Verbose-Log-Nachrichten aus `tag`, `level`, `message` und gegebenenfalls weiteren Trice-Metadaten erzeugen;
- DLT-konforme ECU-, Application- und Context-IDs sowie Paket-Zähler/Zeiten generieren;
- Anmeldung bzw. `GET_LOG_INFO` aus Adapterkonfiguration und gegebenenfalls bekannten Tags aufbauen;
- `SET_LOG_LEVEL` und `SET_TRACE_STATUS` **nur auf Bridge-eigene Anzeige-/Weiterleitungsfilter** anwenden, nicht so tun, als hätte die MCU die Logerzeugung geändert;
- `GET_SOFTWARE_VERSION` aus einer vereinbarten **zyklischen normalen Trice-Firmware-Version-Meldung** beantworten, sofern bereits frisch empfangen; davor keinen Wert erfinden; nach erkanntem Reset/Stale-State invalidieren;
- andere Steuerbefehle entweder korrekt lokal implementieren oder protokollgerecht als nicht unterstützt melden.

`GET_LOG_INFO` und Viewer-Handshake müssen mit einem **echten DLT Viewer** getestet werden. Ein bloßer TCP-Stream mit beliebigen Textzeilen ist keine DLT-Kompatibilität.

### 7.1 DLT-Mapping: keine exakte 1:1-Abbildung

| Trice | DLT v1 | Regel/Problem |
|---|---|---|
| FATAL (900) | FATAL (1) | direkt |
| CRITICAL (800) | FATAL (1) | eigene Stufe geht in DLT verloren |
| ERROR (700) | ERROR (2) | direkt |
| WARNING (600) | WARN (3) | direkt |
| INFO (500) | INFO (4) | direkt |
| DEBUG (300) | DEBUG (5) | direkt |
| TRACE (100) | VERBOSE (6) | eigene Stufe geht verloren |
| VERBOSE (0) | VERBOSE (6) | direkt |

Die genannte Zuordnung ist eine **Empfehlung**. Bei `-utag`-Overrides ist das **effektive Gewicht** maßgeblich, nicht der Name. Trice hat acht benannte Prioritätsbereiche und beliebige Tag-Kategorien; DLT v1 hat sechs Stufen und `APID`/`CTID` von jeweils vier Zeichen. Der volle Trice-Tag und ggf. das Gewicht sollten **zusätzlich erhalten bleiben**, wenn die Bridge eine spätere verlustarme Auswertung erlauben soll.

Zwei noch offene Mapping-Modelle:

- **Tag als DLT-Context** (`RECEIVE` -> z. B. `RX__`): DLT-Viewer-Context-Baum kann Trice-Kategorien filtern; erfordert stabile, kollisionsfreie Alias-Tabelle, verliert aber modulbezogene Kontextsemantik.
- **Softwaremodul als DLT-Context:** sauberere DLT-Semantik; Trice-Tag bleibt Payload-/Zusatzmetadatum, DLT-v1-Filter nach Tag weniger komfortabel.

**DLT v2** besitzt Tags, längere IDs und Source-Location-Felder, ist laut COVESA aber noch funktional eingeschränkt. **Nicht** stillschweigend DLT-v2-Live-Interop voraussetzen. Für den Einstieg v1 bevorzugen und Viewer-Kompatibilität testen.

DLT kennt außerdem **Log vs. Application Trace vs. Network Trace**. `DLT_TRACE_STATUS` ist **nicht** das Trice-Level `TRACE`. Trice-ID, Host-/Target-Timestamp und SL/CE-Werte benötigen bewusst definierte Typ- bzw. Payload-Abbildung.

**Quellen:** [COVESA DLT](https://github.com/COVESA/dlt-daemon), [DLT v2](https://github.com/COVESA/dlt-daemon/blob/master/doc/dlt_protocol_v2.md), [Viewer](https://github.com/COVESA/dlt-viewer), [DLT-Level](https://github.com/COVESA/dlt-daemon/blob/master/include/dlt/dlt_types.h), [Trice-Tag-/Gewichtesemantik](https://github.com/rokath/trice/blob/main/docs/rm/tags_color_and_weights.md).

### 7.2 Was die Bridge explizit *nicht* voraussetzt

- Kein DLT-Protokoll auf STM32 oder anderen MCUs.
- Keine DLT-Abhängigkeit in `tlog`.
- Keine Veränderung des bestehenden binären Trice-Wire-Formats.
- Keine Fernsteuerung der MCU allein durch `SET_LOG_LEVEL` des DLT Viewers.
- Keine DLT-Injection in der ersten Stufe.
- Kein generisches DLT-`-out` innerhalb von `tlog`.

Wenn `tlog` an die Bridge alle dekodierten Events liefert, kann die Bridge die DLT-Filter unabhängig von der Konsolenanzeige verwalten. Sie kann aber **keine** Meldungen wiederherstellen, die Target-Firmware oder der Transport bereits unterdrückt/verloren hat.

---

# Teil IV - Separate spätere Steuer-/Injection-API

## 8. Früherer API-Entwurf bleibt als eigenständiges Thema erhalten

Der frühere `tlog`-Handoff diskutierte eine generische Schnittstelle für Anwendungen. Diese ist **nicht** identisch mit `-out`: Ein passiver Log-Empfänger benötigt keine Target-Schreibrechte. Eingehende Requests werden getrennt autorisiert, beantwortet und gegebenenfalls zum Target durchgereicht.

Historisch diskutierte Namen und Varianten (alle **offen**):

| Kandidat | Beispiel | Ziel / Konflikt |
|---|---|---|
| `-api` | `-api tcp://127.0.0.1:9002` | Übergreifende Kommandos und Status |
| `-control` | `-control tcp://127.0.0.1:9002` | Ein Endpunkt mit `op` im JSON |
| `-control inject:...` | `-control inject:tcp://127.0.0.1:9002` | Operation als URI-Präfix |
| `-inject` | `-inject tcp://127.0.0.1:9002` | Nur Injection, später evtl. weitere Schalter |
| `-tcpListen` | historischer Demo-Vorschlag | **Nicht bevorzugen**; Namenskollision mit `-tcp` |

**Offene Schnittstellenfrage:** Braucht eine künftige `-api` noch ein `subscribe`, nachdem `-out` bereits Push-Subscriptions zu definierten Empfängern bedient? Beide Konzepte haben unterschiedliche Verbindungshoheit: `-out` konfiguriert der `tlog`-Betreiber, `subscribe` fordert der externe Client an. **Keine doppelte Implementierung ohne klaren Nutzfall.**

### 8.1 Historische JSON-Skizzen (kein aktueller Vertrag)

```json
{"trice":"msg:hi"}
{"trice16":"dbg:value=%u","values":[1234]}
{"triceC":"demo:reset"}
{"trice32C":"demo:setRange","values":[958314643,966367642]}
{"TRice32C":"demo:setRange","stamp":1234,"values":[958314643,966367642]}
{"op":"inject","request_id":"r-12","triceC":"demo:reset"}
{"op":"subscribe","format":"text"}
{"op":"status","request_id":"r-13"}
```

`trice32C` (ohne Stamp), `Trice32C` (16-Bit-Stamp) und `TRice32C` (32-Bit-Stamp) hatten unterschiedliche Semantik. Nicht alle Bitbreiten/Makrovarianten oder `op`-Namen sind festgelegt.

### 8.2 Anforderungen an eine künftige API

- Nach außen symbolische Kommandos, **keine Pflicht**, numerische Trice-IDs zu kennen.
- `til.json` und aktuelle Firmware-Version für exakten ID-Lookup; bei **mehrdeutiger** Zuordnung Fehler statt zufälliger Auswahl.
- Transportneutrale Codierung von Trice/ABC (8/16/32/64 Bit, Stamps je nach definiertem Scope, COBS/TCOBS, optionale Verschlüsselung). **Nicht** an TCP koppeln.
- Nur **ein geordneter Writer** zur Target-TX-Verbindung; Zugriff durch mehrere Clients serialisieren; endliche Queues.
- `accepted`, `written`, `executed` **streng unterscheiden**. `executed` benötigt explizite Target-Antwort/Korrelation. ABC allein definiert kein RPC.
- Reproduzierbare Fehler: unbekannter Befehl, unbekannte/mehrdeutige ID, falscher Typ/Wertebereich, Framing, Offline-Target, Write-Fehler, Timeout, Disconnect.
- Nachrichtenframing für TCP (NDJSON oder Längenpräfix), Maximalgrößen, Versionierung und Client-Lebenszyklus definieren; UDP nur bei bewusst akzeptierter Verlustsemantik.
- TX-Meldungen von RX-Trices unterscheiden; zur Konsole spiegeln, falls gewünscht, **nicht** versehentlich als RX-Events in `-vis` oder DLT ausgeben.
- API-Listener standardmäßig Loopback und **nicht** ungeprüft über das Internet erreichbar machen. Bei Netzwerkzugriff Authentisierung/Transportverschlüsselung/Autorisierung klären.

**Für die erste DLT-Bridge ist dies ausdrücklich außerhalb des Scopes.** Spätere DLT-Injection ließe sich in der Bridge in die generische `-api`-Injection übersetzen, ohne DLT in `tlog` zu integrieren.

---

# Teil V - Bewahrter Demo- und Scratchpad-Kontext

## 9. Bifurkationsdemo: späterer Integrations- und Lasttest

Die alte Idee war eine interaktive **logistische Abbildung** auf STM32G0B1 mit LabPlot-Anzeige und einer kleinen Go-/HTML-Steuerapp. **Nicht Teil dieses Auftrags**, aber als Zukunftstest sinnvoll: `-out` für numerische Daten plus spätere `-api`/ABC für Steuerung in einem `tlog`-Prozess.

Formel:

```text
x[n+1] = r * x[n] * (1 - x[n]),  0 < x < 1,  3 <= r <= 4
Feigenbaum-Punkt r ~= 3.569945672
```

Ursprüngliche Parameter (Designannahmen, **nicht gemessen**):

| Parameter | Gedachter Wert |
|---|---|
| Hardware / Takt | STM32G0B1, 16 MHz |
| Parameterbereich / -raster | `r = 3.0..4.0`, Schritt `0.01`, also 101 Werte |
| Zeit je `r` | 100 ms, Samples jede 1 ms |
| Vollständiger Durchlauf | 10,1 s mit rund 1000 Punkten/s |
| Startwert | z. B. `x0 = 0.314159265` (0 und 0,5 vermeiden) |
| Persistente `x[i]` | 101 mal 32 Bit = 404 Byte, Zustand pro `r` über Durchläufe erhalten |
| Numerik | 32-Bit-Festkomma mit 64-Bit-Zwischenprodukten bevorzugt, alternative Darstellung prüfen |
| UART | Eine bidirektionale UART, `tlog` öffnet sie; Startpunkt 230400 oder 460800 Baud |
| Datenmenge | 16-Bit-Nutzwert ca. 6 Byte, 2x16 Bit ca. 8 Byte, 2x32 Bit ca. 12 Byte pro nominalem Record, jeweils **ohne** Framing |

Die letzte Datenrate entspräche bei 1000 Records/s und 8N1 nominal ca. 60/80/120 kbit/s. Reserven für Framing und Peaks prüfen. Tiefes Zoomen über `[3,4]` ist mit nur 16 absoluten Bits unzureichend; Alternativen: 32-Bit-Festkomma, relative 16-Bit-Kodierung im aktiven Zoomfenster oder Parameterindex plus Host-Transformation.

Ablaufidee: Für jedes `r[i]` **eigenen Zustand** behalten, pro Durchlauf 100 Iterationen weiterführen, Werte senden, Einschwingwerte zunächst nicht wegwerfen. Die Anzeige soll eine wachsende XY-Punktwolke zeigen, nicht nur die letzten 500 Zeilen. Bei 1000 Punkten/s entstehen 10.000 Punkte/10 s, 60.000/min und 3,6 Mio./h. LabPlot-Grenzen, Downsampling und 2D-Histogramm bleiben offen. Die **bestehende** LabPlot-Demo ist ein **anderer** Lastfall (50 Samples/s, 500 Zeilen).

Die spätere Steuerapp soll plattformübergreifend in Go mit eingebettetem HTML/JavaScript arbeiten (kein Qt-Zwang); Bedienung: Center, logarithmischer Zoom, Reset, ggf. Pause/Rate. Symbolische ABC-Befehle `demo:setRange` und `demo:reset`, niemals direkte UART-Zweitöffnung. Beim Zoomen neue `r`-Werte, `x[i]` und View-Achsen koordiniert zurücksetzen; Beispiel `3.0..4.0` -> `3.50..3.60` -> `3.560..3.580` -> `3.5690..3.5710`. Konkrete Auflösung, Achsensynchronisierung und ACK-Semantik noch offen.

Historische Aufteilung war (A) MCU-Generator und LabPlot, (B) Host-Encoder/API, (C) ABC und externe Steuerapp. **Reihenfolge bleibt:** erst universelle CLI-/Output-Architektur entscheiden, dann künftig API, dann Demo; kein paralleler Implementierungsauftrag.

### 9.1 Relevante vorhandene Abbildungen (nur Links)

- [PuTTY-Konfiguration 1](https://github.com/rokath/trice/blob/f713ceab876bdbaeaffe862745743632a6d5a63e/docs/ref/PuttyConfig1.PNG), [PuTTY-Konfiguration 2](https://github.com/rokath/trice/blob/f713ceab876bdbaeaffe862745743632a6d5a63e/docs/ref/PuttyConfig2.PNG), [PuTTY-Ansicht](https://github.com/rokath/trice/blob/f713ceab876bdbaeaffe862745743632a6d5a63e/docs/ref/Putty.PNG).
- [UART-Kommandoanimation](https://github.com/rokath/trice/blob/f713ceab876bdbaeaffe862745743632a6d5a63e/docs/ref/UARTCommandAnimation.gif).
- [LabPlot-Vorschau](https://github.com/rokath/trice/blob/f713ceab876bdbaeaffe862745743632a6d5a63e/docs/ref/LabPlotDemo-preview.png), [LabPlot-Animation](https://github.com/rokath/trice/blob/f713ceab876bdbaeaffe862745743632a6d5a63e/docs/ref/LabPlotDemo.gif).
- [Historischer Scratchpad-Ausgangsstand](https://github.com/rokath/trice/blob/f713ceab876bdbaeaffe862745743632a6d5a63e/docs/_scratchPad/scratchPad.md); **kein Lesezwang**, da der inhaltliche Kontext hier integriert ist.

## 10. Historische Randnotizen, keine priorisierten Aufgaben

Diese aus dem alten Scratchpad gesicherten Themen sind **nicht** Teil des aktuellen `-out`-Handoffs, sollen aber beim Ersetzen der alten Datei nicht verschwinden:

- `examples/TriceABC`: README und Shell-Skripte zugänglicher machen; generierte `*.h`/`*.c`-Dateien und Zuständigkeiten erklären.
- Historische Bezeichnung `MVP` in `docs/TriceBind/Trice_bind_90_MVP_User_Manual.md` ggf. ersetzen; Existenz des Pfades bei Bearbeitung neu verifizieren.
- Zusätzliche `-sl`- und `-ce`-Beispiele; Länge des RM-Demotexts prüfen.
- Featureliste für spätere Demos: `speed`, `space`, `tags`, `selection`, `routing`, `level`, `id`, `history`, `rtt`, `abc`, `vis`, `sl`, `ce`, `COBS/TCOBS`, `encryption`, `hs`, `ts`, `tsdelta`, `endianness`, `X0`, `li`, `small on wire`, `multi-targets`, `serial`, `udp in`, `tcp io`, `protection`, `bind`, `insert-cache`, `aliases`, `assert`, `self-contained`.
- Effizientere Hardware-/Bulk-Tests; frühere, **nicht neu verifizierte** Zeitnotizen: Windows ca. 11.770 s, Mac ca. 4.400 s, Debian vermutlich ähnlich Mac.
- **N04 ausdrücklich erledigt/entfallen (05.11.2026):** Wrapper-README-Links unter `third_party` wurden korrigiert; keine neue automatische Linkprüfung hinzufügen; `third_party` bleibt in `lychee.toml` ausgeschlossen. **Nicht** als offenen Auftrag interpretieren.

---

# Teil VI - Entscheidungen für die nächste Designrunde

## 11. Offene Punkte - in dieser Reihenfolge klären

| ID | Frage | Status |
|---|---|---|
| A1 | Ist `-out` der einzige neue *Ausgabe*-Schalter? Welche Sink-Grammatik und welche Quoting-/Escaping-Regeln? | Zielrichtung klar, Syntax offen |
| A2 | Standardausgabe mit/ohne wiederholtes `-out`; Zusammenspiel mit globalem `-logLevel`, `-pick`, `-ban`, `-logFormat` | Offen |
| A3 | Internes Event-Modell und Position des Fan-outs vor bisheriger globaler Selektion | Zielrichtung klar, Code-Design offen |
| A4 | Event-Schema/Version: NDJSON-Vertrag, Zeitbasen, Quellidentität, Metadaten, Diagnosen | Offen |
| A5 | Welche `-vis`-Semantik bleibt; wie bildet `-out` selektive numerische Transformationsregeln ab? | Offen |
| A6 | TCP-Sink: Client vs. Listener, Multiclient, Reconnect, pro-Client-Queue, Sicherheitsdefault | Offen |
| A7 | File-/UDP-/DS-Sink: Append-/Truncate, Backpressure, Header, Lebenszyklus | Offen |
| A8 | Migration von `-tcp`, `-ds`, `-vis`, `-logfile` ohne Nutzerbruch; Deprecation-Reihenfolge | Offen, keine Frist |
| A9 | DLT-Bridge: v1-Verbose-MVP, APID/CTID-Mapping, Control-Abdeckung, Firmware-Version aus zyklischem Trice | Externe Folgeaufgabe, Mapping offen |
| A10 | Externe Steuerschnittstelle: `-api`/`-control`, `inject`/`subscribe`, Request-Protokoll, Sicherheit | Spätere separate Designrunde |
| A11 | Testabdeckung, Ressourcenlimits, Dokumentation/CLI-Hilfe, Backward Compatibility | Vor Implementierung definieren |

### 11.1 Gewünschtes Ergebnis der nächsten Arbeitsphase

Ein kurzes, freigegebenes **Design-Decision-Record (DDR)** soll mindestens enthalten:

1. Grammatik mit 3-5 **gültigen** und 3-5 **ungültigen** `-out`-Beispielen; standardmäßiges `stdout` und Altflag-Priorität.
2. Zustandsdiagramm für Quelle, Decodierung, Event-Fan-out, per-Sink-Filterung, Formatierung, Transport, Fehler/Shutdown.
3. Protokollvertrag für NDJSON (Minimum und optionale Metadaten), Fehler-/Diagnosekanal und Versionspolitik.
4. Ein Test mit `tlog`-Konsolenfilter WARN und gleichzeitiger **unselektierter** NDJSON-Ausgabe an eine externe Testanwendung, ohne eine einzige DLT-Abhängigkeit.
5. Einen gesonderten Scope für DLT-Bridge und einen noch späteren für Injection; nachvollziehbare Nichtziele.
6. Migrationsplan und Tests für Legacy-Optionen. **Erst danach** darf Implementierungsarbeit konkret beauftragt werden.

### 11.2 Unbedingt zu vermeiden

- Kein DLT-Protokoll oder -Viewer-Code im `tlog`-Prozess oder Target.
- Keine neue `-tcp`-Spezialfunktion nur für einen DLT-Adapter.
- Keine lokale Vorfilterung des Event-Busses durch Terminal-`-logLevel`.
- Keine Ausgabedaten durch Parsten bereits formatierten Terminaltexts erzeugen, sofern strukturierte Events vorliegen.
- Keine unbegrenzte Netzwerkwarteschlange und keine heimliche Remote-Bind-Adresse `0.0.0.0`.
- Kein stilles Entfernen von `-ds`-Interaktion, `-vis`-`log=drop` oder PuTTY-Kompatibilität.
- Keine Annahme, dass ein DLT-Control-ACK eine Ausführung auf der MCU beweist.
- Keine Bifurkationsdemo/Encoder/UI implementieren, bevor Output- und spätere API-Grenzen festgelegt sind.
- Keine automatische Priorisierung der historischen Randnotizen aus Abschnitt 10.

---

**Handoff-Ende.** Dieses Dokument ist inhaltlich eigenständig und ersetzt als neue Arbeitsgrundlage den vorherigen `handsoff_tlog_CLI_Architektur.md`. Es dokumentiert eine gewünschte Richtung und konkrete Designaufgaben; es ist **keine** Freigabe zur Implementierung, zum Löschen alter Dateien oder zum Ändern von GitHub-Inhalten.
