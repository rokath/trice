<!--

*) In ./examples/TriceABC funktionieren die *.sh Skripte sehr gut, allerdings wirken sie etwas abschreckend insbesondere für User, die keine ash-Experten sind. Lassen sich diese Skripte im Sinne der einfachen Lesarkeit verbessern ohne die Funktionalität zu verschlechteren? Zumindest mit ausführlichen Kommentaren. Auch ist es schwer, die generierten *.h und *.c Dateien zu finden. Wo sind die eigentlich? Das muss unbeding verbessert werden, damit User sich schnell zurechtfinden. Zumindest die README.md Files sollten darüber Aufschluss geben.

*) Die Bezeichnung MVP sollte verschwinden aus dem Inhalt der Datei docs/TriceBind/Trice_bind_90_MVP_User_Manual.md und geeignet ersetzt verden.

TODO:

- -sl Beispiele
- -ce Beispiele
- Trice Features Liste
  - speed, space, tags, selection, routing, level, id, history, rtt, abc, vis, sl, ce, COBS/TCOBS, encryption, hs, ts, tsdelta, endianness, X0, li, small on wire, multi-targets, serial, udp in, tcp io, protection, bind, insert-cache, aliases, assert, self-contained
- RM Demo Text zu lang?
- HW Tests
- Testzeiten: Win ~11770s Mac ~4400s Debian vermutlich wie Mac,
- Mehr Bultests zur Testzeitverkürzung.

## Selbst gepflegte Dokumentation vollständig in Prüfungen erfassen

**N04 · entfällt nach Nutzerentscheidung vom 2026-11-05**

Die bekannten veralteten Links in den fünf eigenen Wrapper-READMEs unter `third_party` sind korrigiert. Auf eine zusätzliche automatische Prüfung dieser Dateien wird verzichtet. Der Ausschluss von `third_party` in [lychee.toml](../../lychee.toml) bleibt bestehen; künftige kaputte Links in diesen READMEs werden damit nicht automatisch erkannt.

Offen: 

-->

# Handoff: Interaktive Trice-Bifurkationsdemo (Aufgabe aufteilen!  )

**Projekt:** rokath/trice  
**Zielplattform:** STM32G0B1, 16 MHz  
**PC-Software:** `tlog` / `trice log`, LabPlot  
**Betriebssysteme:** Windows, Linux, macOS  
**Status:** Architekturkonzept, noch keine Implementierung

---

## 1. Grundidee

Eine attraktive, interaktive Visualisierungsdemo soll mehrere wesentliche Fähigkeiten von Trice demonstrieren:

- Sehr effizientes Embedded Logging mit geringer Target-Belastung.
- Binäre Datenübertragung über UART ohne RTT.
- Live-Visualisierung mit LabPlot.
- Interaktive Steuerung des Targets mittels Trice ABC.
- Bidirektionale Kommunikation über eine einzige UART.
- Plattformübergreifende Nutzung, gegebenenfalls über mehrere PCs.

Als mathematische Grundlage dient das **Bifurkationsdiagramm der logistischen Abbildung**.

Gleichzeitig soll eine kleine, allgemein nutzbare Erweiterung von `tlog` entstehen, die beliebigen Anwendungen das Senden von Trice-Nachrichten über TCP ermöglicht.

Die Erweiterung soll ausdrücklich nicht demospezifisch sein.

---

## 2. Mathematische Grundlage

Die logistische Abbildung lautet:

\[
x_{n+1}=r x_n(1-x_n)
\]

mit

\[
0<x_n<1,\qquad 3\le r\le4
\]

Je nach Parameter \(r\) ergeben sich periodische oder chaotische Zustände.

Bei zunehmendem \(r\) treten Periodenverdopplungen auf, die schließlich in chaotisches Verhalten übergehen.

### 2.1 Bifurkationsdiagramm

Horizontal wird \(r\), vertikal \(x\) dargestellt.

Schematische Darstellung:

```text
x
1.0 |                              . .  . .
    |                         .  . .   . .
    |                    . . .   . . .  .
    |             ------<    . .   . . . .
    |        ----<        . . .  . .  .
    |-------<             . .   . . . .
    |        ----<        . . .  . .  .
    |             ------<    . .  . . .
0.0 +------------------------------------> r
    3.0          3.5       3.57       4.0

       Periodenverdopplung → Chaos
```

Die charakteristischen Strukturen besitzen in bestimmten Parameterbereichen eine näherungsweise Selbstähnlichkeit.

Der Feigenbaum-Punkt liegt bei

\[
r_\infty\approx3.569945672
\]

Eine interaktive Vergrößerung ermöglicht das Erkunden immer kleinerer Strukturen.

---

## 3. Datenerzeugung auf dem Mikrocontroller

### 3.1 Initiale Parameter

| Parameter | Vorgesehener Wert |
|---|---|
| MCU | STM32G0B1 |
| CPU-Takt | 16 MHz |
| Parameterbereich | 3,0 bis 4,0 |
| Parameterschritt | 0,01 |
| Anzahl Parameterwerte | 101 |
| Verweildauer pro Parameter | 100 ms |
| Sampleintervall | 1 ms |
| Samples pro Parameter und Durchlauf | 100 |
| Gesamtdauer eines Durchlaufs | 10,1 s |

### 3.2 Persistente Zustände

**Jeder Parameterwert erhält einen eigenen Zustand \(x_i\).**

Diese Zustände bleiben über mehrere Durchläufe erhalten.

Dadurch wird nicht bei jedem Parameterwechsel neu angefangen, sondern die jeweilige Iterationsfolge fortgesetzt.

Beispiel:

```text
              Durchlauf 1       Durchlauf 2       Durchlauf 3

r = 3.00     x[1..100]         x[101..200]       x[201..300]
r = 3.01     x[1..100]         x[101..200]       x[201..300]
r = 3.02     x[1..100]         x[101..200]       x[201..300]
   ...          ...                ...                ...
r = 4.00     x[1..100]         x[101..200]       x[201..300]
```

Die Zahlen bezeichnen jeweils die Iterationen des betreffenden Parameterwerts.

Alle berechneten Werte werden übertragen. Insbesondere werden die anfänglichen Einschwingwerte nicht verworfen.

### 3.3 Initialisierung

Wichtig:

\[
x_0=0
\]

ist ungeeignet, weil daraus ausschließlich Nullen folgen.

Auch \(x_0=0.5\) ist für \(r=4\) problematisch.

Ein geeigneter gemeinsamer Startwert ist beispielsweise:

\[
x_0=0.314159265
\]

### 3.4 Pseudocode

```c
uint32_t x[101];

initialize_states(x);

while (1) {
    for (unsigned i = 0; i < 101; i++) {
        r = calculate_r(i);

        for (unsigned j = 0; j < 100; j++) {
            x[i] = logistic(r, x[i]);

            send_trice_sample(r, x[i]);

            delay_ms(1);
        }
    }
}
```

Die Zustände benötigen bei 32-Bit-Darstellung nur 404 Bytes RAM.

Die Berechnung soll vorzugsweise mit 32-Bit-Festkomma und geeigneten 64-Bit-Zwischenprodukten erfolgen.

Die 16-Bit-Auflösung der übertragenen Visualisierungswerte muss nicht der internen Rechenauflösung entsprechen.

---

## 4. Visualisierung durch fortlaufende Verdichtung

Die Punkte sollen sich über mehrere Durchläufe akkumulieren.

```text
Nach 1 Durchlauf       Nach 10 Durchläufen       Nach 100 Durchläufen

     .    .                 .   ..                    ......
   .   .  .              . .  ... ..               .. .:.:...
 .    .  . .             ..  .:.. .:.              .:::***::..
   .    .                . .. .  ...               ..:*****::.
```

Diese Darstellung ist rein schematisch.

Bei 1000 Samples/s entstehen:

| Laufzeit | Anzahl Punkte |
|---|---:|
| 10 Sekunden | ca. 10.000 |
| 1 Minute | 60.000 |
| 10 Minuten | 600.000 |
| 1 Stunde | 3.600.000 |

Die zunächst dünn besetzte Punktwolke verdichtet sich zunehmend.

### Anforderungen an LabPlot

- XY-Darstellung statt einer gewöhnlichen Zeitreihenkurve.
- Punkte möglichst ohne Verbindungslinien.
- Neue Punkte sollen vorhandene ergänzen.
- Keine automatische Begrenzung auf die letzten wenigen hundert Werte.
- Möglichst konstante Achsengrenzen während eines Durchlaufs.

Zu prüfen ist, wie viele Punkte LabPlot bei kontinuierlicher Live-Aktualisierung effizient darstellen kann.

Eine zweidimensionale Häufigkeitsdarstellung wäre eine spätere Option, ist aber keine Voraussetzung für die erste Demo.

**LabPlot bleibt als Visualisierungsprogramm gesetzt.**

---

## 5. UART-Datenübertragung

Die UART soll bidirektional verwendet werden.

RTT wird ausdrücklich nicht eingesetzt.

### 5.1 Datenvolumen

Ein Trice mit einem 16-Bit-Nutzwert benötigt ohne Timestamp zunächst 6 Bytes, zuzüglich gegebenenfalls Framing-Overhead.

Bei 1000 Samples/s ergibt das:

\[
1000\cdot6\cdot10=60\,000\ \mathrm{Bit/s}
\]

Für ein XY-Diagramm werden grundsätzlich zwei Werte benötigt.

Bei zwei 16-Bit-Werten beträgt die nominelle Trice-Größe 8 Bytes.

\[
1000\cdot8\cdot10=80\,000\ \mathrm{Bit/s}
\]

Hier ist das Framing noch nicht berücksichtigt.

### 5.2 Empfohlene Baudrate

**230400 Baud** erscheint als sinnvoller Ausgangspunkt.

115200 Baud könnten bei kompakten 16-Bit-Datensätzen funktionieren, lassen jedoch weniger Reserve.

Für eine Übertragung beider Werte als 32-Bit-Daten wären bereits 120 kbit/s ohne Framing erforderlich.

230400 oder gegebenenfalls 460800 Baud wären dafür geeigneter.

### 5.3 Auflösung beim Zoom

Für tiefe Vergrößerungen reicht eine absolute 16-Bit-Codierung von \(r\) über den gesamten Bereich [3,4] nicht aus.

Mögliche Lösungen:

1. \(r\) als 32-Bit-Festkommawert übertragen.
2. \(r\) als relativen 16-Bit-Wert innerhalb des jeweils aktuellen Zoomfensters übertragen.
3. Einen 16-Bit-Parameterindex übertragen und die Transformation auf dem PC durchführen.

Die zweite Variante wäre besonders kompakt. Allerdings müssen dann LabPlot beziehungsweise die PC-seitige Datenaufbereitung den aktuellen Parameterbereich kennen.

Für die erste Implementierung ist die Abwägung zwischen Einfachheit und Bandbreite noch offen.

---

# 6. Interaktive Steuerung

Die Demo soll später eine interaktive Veränderung des Parameterbereichs ermöglichen.

Vorgesehen sind insbesondere:

- Verschieben des Parameterfensters.
- Vergrößern und Verkleinern.
- Zurücksetzen auf [3,4].
- Optional Verändern der Berechnungs- und Ausgaberate.

Beim Wechsel des Parameterbereichs müssen die neuen \(r\)-Werte bestimmt und die zugehörigen Zustände passend initialisiert werden.

Außerdem muss die Visualisierung verhindern, dass Punkte aus verschiedenen Zoomfenstern ununterscheidbar vermischt werden.

---

# 7. Gesamtarchitektur

```text
                 PC A oder PC B
              ┌───────────────────┐
              │     Demo-App      │
              │                   │
              │   Center-Slider   │
              │   Zoom-Slider     │
              │   Reset-Button    │
              └─────────┬─────────┘
                        │
                        │ TCP / JSON
                        │
                        ▼
                 PC A (Host)
       ┌─────────────────────────────┐
       │                             │
       │            tlog             │
       │                             │
       │  ┌───────────────────────┐  │
       │  │     -tcpListen        │  │
       │  │                       │  │
       │  │  JSON → Trice Encoder │  │
       │  │                       │  │
       │  │      til.json         │  │
       │  └───────────┬───────────┘  │
       │              │              │
       │          UART TX            │
       │              │              │
       │          UART RX            │
       │              │              │
       │    ┌─────────▼─────────┐    │
       │    │  Trice Decoder    │    │
       │    │        │          │    │
       │    │       -vis        │    │
       │    └─────────┬─────────┘    │
       │              │              │
       └──────────────┼──────────────┘
                      │
                      ▼
              ┌──────────────┐
              │   LabPlot    │
              │              │
              │ XY-Diagramm  │
              └──────────────┘

       ┌─────────────────────────────┐
       │          STM32G0B1          │
       │                             │
       │    UART RX → ABC Handler    │
       │                             │
       │    Logistic Map Generator   │
       │                             │
       │    Trice → UART TX          │
       └─────────────────────────────┘
```

Die UART wird ausschließlich von `tlog` geöffnet.

Dadurch entfallen:

- Serielle Port-Splitter.
- Zusätzliche USB-UART-Verbindungen.
- RTT.
- Betriebssystemspezifische Zugriffe auf die UART durch andere Programme.

Der TCP-Eingang stellt den Rückkanal bereit.

---

# 8. Neue universelle tlog-Funktion: `-tcpListen`

## 8.1 Grundentscheidung

Ursprünglich war ein Schalter `-abcListen` vorgesehen.

Dieser Ansatz wurde zugunsten einer allgemeineren Lösung erweitert.

**Vorgeschlagener Schalter:**

```bash
tlog ... -tcpListen 127.0.0.1:9002
```

Der Schalter ist neu zu implementieren.

Er öffnet einen TCP-Listener, über den andere Anwendungen Trice-Nachrichten zur Übertragung an das Target einreichen können.

Die Funktion soll sowohl normale Trices als auch ABC-Kommandos unterstützen.

Damit ist sie nicht auf Steuerkommandos beschränkt.

## 8.2 JSON-Nachrichtenformat

Die JSON-Schlüssel entsprechen unmittelbar den vorhandenen Trice-Makronamen.

Beispiele:

**Einfaches Trice:**

```json
{"trice":"msg:hi"}
```

**Trice mit einem 16-Bit-Argument:**

```json
{"trice16":"dbg:value=%u","values":[1234]}
```

**ABC ohne Nutzdaten:**

```json
{"triceC":"demo:reset"}
```

**ABC mit zwei 32-Bit-Argumenten:**

```json
{"trice32C":"demo:setRange","values":[958314643,966367642]}
```

**ABC mit 32-Bit-Stamp:**

```json
{"TRice32C":"demo:setRange","stamp":1234,"values":[958314643,966367642]}
```

Dabei entsprechen beispielsweise:

- `trice32C` einem ABC-Kommando ohne Stamp.
- `Trice32C` einem ABC-Kommando mit 16-Bit-Stamp.
- `TRice32C` einem ABC-Kommando mit 32-Bit-Stamp.

Das JSON-Protokoll sollte grundsätzlich sämtliche unterstützten Trice-Makrovarianten abbilden können.

Für eine erste Implementierung genügt eine Teilmenge.

## 8.3 Nachrichtenabgrenzung

Vorgeschlagen wird **NDJSON** (Newline Delimited JSON).

Jede Zeile enthält genau ein JSON-Objekt.

Beispiel:

```text
{"triceC":"demo:reset"}
{"trice32C":"demo:setRange","values":[958314643,966367642]}
{"trice":"msg:hi"}
```

Das ermöglicht eine einfache Implementierung mit den Standardbibliotheken von Go.

Der TCP-Empfänger sollte die maximale Zeilenlänge begrenzen.

---

# 9. Verarbeitung in tlog

```text
              TCP-Client
                  │
                  ▼
           ┌──────────────┐
           │ JSON lesen   │
           └──────┬───────┘
                  │
                  ▼
           ┌──────────────┐
           │ Typ/Format   │
           │  ermitteln   │
           └──────┬───────┘
                  │
                  ▼
           ┌──────────────┐
           │  til.json    │
           │ ID auflösen  │
           └──────┬───────┘
                  │
                  ▼
           ┌──────────────┐
           │ Parameter    │
           │ validieren   │
           └──────┬───────┘
                  │
                  ▼
           ┌──────────────┐
           │ Trice-Record │
           │  codieren    │
           └──────┬───────┘
                  │
                  ▼
           ┌──────────────┐
           │ COBS/TCOBS   │
           │   Framing    │
           └──────┬───────┘
                  │
                  ▼
           ┌──────────────┐
           │ UART TX      │
           └──────┬───────┘
                  │
                  ▼
           ┌──────────────┐
           │ TX-Logeintrag│
           └──────────────┘
```

Die genaue Framing-Variante muss zur Empfangskonfiguration des Targets passen.

`tlog` übernimmt:

1. TCP-Verbindung annehmen.
2. JSON-Nachricht lesen.
3. Trice-Typ und Formatstring bestimmen.
4. Passenden Eintrag in `til.json` suchen.
5. Trice-ID bestimmen.
6. Nutzdaten validieren und codieren.
7. Trice-Nachricht erzeugen.
8. Framing durchführen.
9. Nachricht über die bereits geöffnete UART übertragen.
10. Versand in der regulären Logausgabe protokollieren.

Der eigentliche UART-Schreibzugriff muss mit anderen möglichen Schreiboperationen synchronisiert werden.

Die Funktionen sollten unabhängig vom TCP-Transport implementiert werden, sodass derselbe Encoder später auch für andere Eingangswege genutzt werden kann.

---

# 10. ID-Verwaltung und Entkopplung

## 10.1 Keine festen IDs reservieren

Eine wichtige Architekturentscheidung:

**Die Demo-App kennt keine numerischen Trice-IDs.**

Sie verwendet ausschließlich:

- Trice-Makrotyp.
- Formatstring beziehungsweise ABC-Kommandonamen.
- Parameterwerte.
- Gegebenenfalls einen Stamp.

`tlog` löst die numerische ID anhand der gemeinsamen `til.json` auf.

Somit können sich IDs ändern, ohne dass die Demo-App angepasst werden muss.

Voraussetzung ist, dass Firmware und `tlog` dieselbe ID-Zuordnung verwenden.

## 10.2 Bereitstellung der ABC-IDs

Die benötigten Kommandoeinträge müssen in `til.json` vorhanden sein.

Dafür kann eine kleine C-Definitionsdatei verwendet werden, die lediglich von `trice bind` verarbeitet wird.

Beispiel:

```c
void demoAbcDefinitions(void) {
    uint32_t range[2] = {0, 0};

    trice32C("demo:setRange", range, 2);
    triceC("demo:reset");
}
```

Diese Funktion dient nur der Bereitstellung der Trice-Aufrufstellen für die ID-Vergabe.

Sie muss auf keinem PC oder Mikrocontroller tatsächlich ausgeführt werden.

Anschließend erzeugt `trice generate -abc` wie bisher die passende Empfängertabelle.

## 10.3 Mehrdeutige IDs

Trice ABC erlaubt, dass unterschiedliche Sende-Aufrufstellen dieselben Kommandonamen verwenden, aber unterschiedliche IDs erhalten.

Daher kann die Kombination

```text
Makrotyp + Formatstring
```

mehrdeutig sein.

Vorgeschlagene Regel:

**Bei mehreren passenden IDs wird zunächst ein Fehler zurückgegeben.**

Keine willkürliche Auswahl.

Später könnte bei Bedarf eine explizite ID oder ein symbolischer Senderbezeichner als zusätzlicher JSON-Parameter unterstützt werden.

Eine globale Reservierung von ID-Bereichen ist dafür nicht notwendig.

---

# 11. Spiegelung ausgehender Trices im Log

Alle über `-tcpListen` erfolgreich an die UART übergebenen Nachrichten sollen zusätzlich in der normalen Logausgabe erscheinen.

Sie erhalten einen eigenen Prefix.

**Vorschlag: `TX:`**

Beispiel:

```text
09:28:14.123 TX: demo:setRange(958314643,966367642)
09:28:14.124 INF: New range accepted
09:28:14.125 vis:r=958314643,x=32768
```

Die erste Nachricht stammt aus dem TCP-Eingang.

Die nachfolgenden Nachrichten werden vom Target empfangen.

Fehler sollten separat erkennbar sein:

```text
09:28:14.123 TX-ERR: Unknown Trice command
```

Wichtige Anforderungen:

- Die gespiegelten Nachrichten erscheinen im normalen Log.
- Der Prefix unterscheidet Host-TX von Target-RX.
- Die Nachrichten werden nicht erneut in den Empfangsdecoder eingespeist.
- Sie erzeugen nicht unbeabsichtigt zusätzliche `-vis`-Datensätze.
- Eine erfolgreich geschriebene UART-Nachricht gilt nicht automatisch als vom Target ausgeführt.

Eine explizite Ausführungsbestätigung kann bei Bedarf durch eine Antwort des Targets erfolgen.

---

# 12. Kleine externe Demo-App

## 12.1 Vorgeschlagene Implementierung

**Go mit eingebettetem HTML und JavaScript.**

Qt ist dafür nicht erforderlich.

Die Go-Anwendung stellt einen kleinen HTTP-Server bereit und öffnet beziehungsweise bietet eine Webseite an.

Das funktioniert auf Windows, Linux und macOS.

Die App könnte auch auf einem anderen PC laufen und ihre JSON-Kommandos über das Netzwerk an `tlog` schicken.

## 12.2 Bedienoberfläche

Prinzipskizze:

```text
┌──────────────────────────────────────────┐
│       Trice Bifurcation Controller       │
├──────────────────────────────────────────┤
│                                          │
│ Parameter Center                         │
│ 3.0  ├──────────────●──────────────┤ 4.0 │
│                                          │
│ Zoom                                     │
│ 1x   ├────────●────────────────────┤ 1000x
│                                          │
│ Current Range                            │
│                                          │
│       3.550000 ... 3.600000              │
│                                          │
│       [ Reset ]     [ Pause ]            │
│                                          │
│ TCP: 192.168.1.20:9002                   │
│ Status: Connected                        │
└──────────────────────────────────────────┘
```

Die genaue Gestaltung ist noch offen.

### Vorgesehene Funktionen

**Center-Slider**

Bestimmt die Mitte des darzustellenden Parameterbereichs.

**Zoom-Slider**

Bestimmt dessen Breite. Eine logarithmische Skalierung wäre sinnvoll.

**Reset**

Stellt den ursprünglichen Bereich [3,4] wieder her.

**Optional: Pause**

Unterbricht die Berechnung beziehungsweise Ausgabe.

Bei einer Slideränderung werden zwei neue Parametergrenzen berechnet und über TCP an `tlog` geschickt.

Die Demo-App muss weder numerische Trice-IDs noch UART-Protokolldetails kennen.

---

# 13. ABC auf dem Target

Der STM32 verwendet die bestehende Trice-ABC-Empfangslogik.

Für die erste Demo genügen zwei Kommandos:

```text
demo:setRange
demo:reset
```

Die erzeugte ABC-Tabelle ordnet die IDs den entsprechenden C-Handlern zu.

Schematisch:

```text
UART RX
   │
   ▼
Framing Decoder
   │
   ▼
Trice RX Parser
   │
   ▼
ABC ID Lookup
   │
   ├── demo:setRange ──► Neue Parametergrenzen
   │
   └── demo:reset ─────► Originalbereich
```

Die Handler sollten nur die neuen Steuerwerte übernehmen.

Die eigentliche Reinitialisierung der Berechnung erfolgt anschließend im normalen Programmablauf.

Normale empfangene Trices ohne ABC-Suffix lösen nicht automatisch Funktionen auf dem Target aus. Dafür wäre eine gesonderte Empfangsverarbeitung erforderlich.

---

# 14. Interaktiver Zoom

Der besonders interessante Teil der Demo besteht im Erkunden immer feinerer Strukturen.

Beispiel:

```text
Gesamtansicht

r = 3.0 ─────────────────────────── 4.0
                   │
                   │ Zoom
                   ▼

r = 3.50 ──────────────── 3.60
                    │
                    │ Zoom
                    ▼

r = 3.560 ─────── 3.580
                    │
                    │ Zoom
                    ▼

r = 3.5690 ───── 3.5710
                    │
                    ▼

            Weitere Details
```

Bei jeder Vergrößerung:

1. Neue Parametergrenzen werden bestimmt.
2. Die Demo-App sendet `demo:setRange`.
3. Das Target erzeugt 101 neue Parameterwerte im ausgewählten Intervall.
4. Die Zustände werden neu initialisiert.
5. Die neuen Daten werden kontinuierlich übertragen.
6. LabPlot zeigt die zunehmend dichter werdende Struktur.

Wichtig: Die bloße Vergrößerung vorhandener Punkte reicht nicht aus.

**Die MCU muss für den neuen Parameterbereich tatsächlich neue Werte berechnen.**

Bei tieferen Zoomstufen begrenzt außerdem die interne numerische Genauigkeit die sinnvolle Vergrößerung.

---

# 15. Bestehende Funktionen, die genutzt werden können

Im Trice-Repository sind bereits wichtige Komponenten vorhanden:

| Komponente | Status |
|---|---|
| UART-Empfang durch `tlog` | Vorhanden |
| UART-Schreibzugriff durch `tlog` | Vorhanden |
| Interaktive Kommandoeingabe über `-ds` | Vorhanden |
| Trice-Decoder | Vorhanden |
| `-vis`-Ausgabe | Vorhanden |
| ABC-Empfang auf dem Target | Vorhanden |
| ABC-Handlergenerierung | Vorhanden |
| `til.json` | Vorhanden |
| Generischer Host-Trice-Encoder | Für diese Funktion zu entwickeln |
| TCP-Listener für JSON-Kommandos | Neu |
| Logspiegelung für Host-TX | Neu |
| Bifurkationsgenerator | Neu |
| Kleine Demo-App | Neu |

Die existierende `-ds`-Funktion kann bereits Textkommandos über eine geöffnete UART senden.

Sie ist allerdings keine allgemeine binäre Trice-ABC-Schnittstelle.

Für die neue Funktion soll kein serieller Splitter verwendet werden.

---

# 16. Sicherheits- und Robustheitsaspekte

Der TCP-Listener sollte standardmäßig nur auf `127.0.0.1` lauschen.

Für eine externe Demo-App muss eine andere Bind-Adresse ausdrücklich konfiguriert werden.

Zu berücksichtigen sind:

- Begrenzung der JSON-Nachrichtenlänge.
- Prüfung der Parameteranzahl und Wertebereiche.
- Behandlung unbekannter Trice-Typen und IDs.
- Erkennung mehrdeutiger TIL-Einträge.
- Synchronisierter UART-Schreibzugriff.
- Behandlung unterbrochener TCP-Verbindungen.
- Keine automatische Wiederholung möglicherweise bereits versendeter Kommandos.
- Kein ungeschützter ABC-Zugriff aus nicht vertrauenswürdigen Netzwerken.

Die erste Version muss keine vollständige RPC-Infrastruktur implementieren.

Insbesondere sind Adressierung, Wiederholungen, Acknowledgements und Authentifizierung keine Voraussetzungen für die lokale Demo.

---

# 17. Offene technische Entscheidungen

### A. Datenformat für die XY-Visualisierung

Zwei Möglichkeiten stehen im Vordergrund:

- Zwei 16-Bit-Werte pro Sample, mit relativer Codierung der horizontalen Koordinate.
- Zwei 32-Bit-Werte pro Sample, mit höherer Präzision und größerem UART-Datenvolumen.

Zu prüfen ist, welche Variante sich mit `tlog -vis` und LabPlot am einfachsten realisieren lässt.

### B. Zoom und LabPlot

Die externe App kann den Parameterbereich auf dem Target verändern.

Noch offen ist, wie LabPlot anschließend seine Darstellung anpasst.

Mögliche einfache Lösung:

- LabPlot verwendet normierte Koordinaten.
- Die Demo-App zeigt die tatsächlichen Parametergrenzen an.
- Beim Zoom wird die bisherige Punktwolke zurückgesetzt.

Eine automatische Steuerung von LabPlot selbst ist zunächst nicht erforderlich.

### C. Umfang des universellen Trice-Encoders

Für eine erste Implementierung:

- Ungestempelte Trices.
- 16- und 32-Bit-Parameter.
- Ungestempelte ABC-Kommandos.
- COBS oder TCOBS entsprechend der Target-Konfiguration.

Später gegebenenfalls:

- 8- und 64-Bit-Werte.
- 16- und 32-Bit-Stamps.
- Weitere Trice-Typen.
- Andere Eingangswege als TCP.

### D. TIL-Mehrdeutigkeiten

Zunächst sollen mehrdeutige Zuordnungen abgewiesen werden.

Eine allgemeinere Lösung für mehrere Sender kann später ergänzt werden.

### E. Performance von LabPlot

Es sollte experimentell geprüft werden, wie sich die kontinuierliche Akkumulation von Hunderttausenden Punkten auf die Darstellungsgeschwindigkeit auswirkt.

---

# 18. Vorgeschlagene Umsetzung

Die Arbeiten lassen sich in drei weitgehend unabhängige Teile zerlegen.

## Teil A: Bifurkationsgenerator

STM32-Firmware:

- Logistische Abbildung implementieren.
- 101 persistente Zustände verwenden.
- Ausgabe über Trice.
- Zunächst festen Bereich [3,4] verwenden.
- Funktion mit LabPlot testen.

Damit entsteht bereits eine eigenständig nutzbare Visualisierungsdemo.

## Teil B: Universelle TCP-Eingangsschnittstelle

`tlog` erweitern:

- `-tcpListen` implementieren.
- NDJSON lesen.
- Trice-Typ und Formatstring auswerten.
- Numerische ID aus `til.json` auflösen.
- Trice-Datensatz erzeugen.
- Framing durchführen.
- Über bestehende UART-Verbindung schreiben.
- Host-TX im Log spiegeln.

Diese Erweiterung soll unabhängig von der Bifurkationsdemo getestet werden.

## Teil C: Interaktive Steuerung

- ABC-Handler auf dem STM32 ergänzen.
- Demo-App mit Go und HTML implementieren.
- Center- und Zoom-Slider bereitstellen.
- JSON-Kommandos an `tlog` senden.
- Zoom-Bereiche und Visualisierung koordinieren.

---

# 19. Wesentliche Architekturentscheidungen

1. **LabPlot bleibt das Visualisierungsprogramm.**
2. **`tlog` bleibt die zentrale PC-seitige Trice-Komponente.**
3. **Keine Verwendung von RTT.**
4. **Eine einzige bidirektionale UART-Verbindung.**
5. **Nur `tlog` öffnet die UART.**
6. **Kein serieller Splitter.**
7. **Generischer TCP-Eingang statt demospezifischer Mausunterstützung.**
8. **JSON-Schlüssel entsprechen den Trice-Makronamen.**
9. **Normale Trices und ABC-Kommandos werden unterstützt.**
10. **Trice-IDs werden anhand der `til.json` aufgelöst.**
11. **Keine festen ABC-ID-Reservierungen notwendig.**
12. **Hostseitige TX-Nachrichten werden mit eigenem Prefix protokolliert.**
13. **Die Demo-App bleibt unabhängig vom binären Trice-Protokoll.**
14. **Go mit eingebettetem HTML ist der bevorzugte GUI-Ansatz.**
15. **Die Erweiterung von `tlog` soll auch außerhalb der Demo verwendbar sein.**

---

# 20. Zusammenfassung

Das Vorhaben besteht aus zwei sich ergänzenden Projektergebnissen.

### Ergebnis 1: Attraktive Embedded-Visualisierungsdemo

Ein STM32G0B1 berechnet bei nur 16 MHz die logistische Abbildung für viele Parameterwerte.

Die entstehenden Daten werden effizient über Trice und UART übertragen.

LabPlot zeigt ein sich fortlaufend verdichtendes Bifurkationsdiagramm.

Durch interaktiven Zoom lassen sich die charakteristischen Strukturen chaotischer Systeme erkunden.

### Ergebnis 2: Allgemeine Trice-TCP-Eingangsschnittstelle

Eine neue Option

```bash
tlog ... -tcpListen 127.0.0.1:9002
```

ermöglicht externen Programmen, Trice-Nachrichten als JSON einzureichen.

Beispielsweise:

```json
{"trice":"msg:hi"}
{"trice16":"dbg:value=%u","values":[1234]}
{"triceC":"demo:reset"}
{"trice32C":"demo:setRange","values":[958314643,966367642]}
```

`tlog` übernimmt ID-Auflösung, Codierung, Framing und UART-Ausgabe.

Die gesendeten Nachrichten erscheinen mit einem eigenen Prefix auch im Log.

Dadurch entsteht eine plattformübergreifende und weitgehend entkoppelte Möglichkeit, beliebige Trice-Nachrichten und ABC-Kommandos von externen Anwendungen an ein Target zu senden.

**Zentrale Zielsetzung: Eine überzeugende Trice-Demo mit möglichst wenig demospezifischem Zusatzcode und einer kleinen, dauerhaft nützlichen Erweiterung des bestehenden Trice-Werkzeugkastens.**

