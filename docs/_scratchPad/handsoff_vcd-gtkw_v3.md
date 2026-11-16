# Handoff v3 — Trice-VCD-Export und wiederverwendbare GTKWave-Ansichten

**Projekt:** https://github.com/rokath/trice  
**Stand:** 15.11.2026  
**Zielgruppe:** Codex / Implementierung im Trice-Repository  
**Status:** Technischer Implementierungsauftrag, **keine Beschreibung bereits vorhandener CLI-Funktionen**.  
**Ersetzt vollständig:** `handsoff_vcd-gtkw.md` (v1) und `handsoff_vcd-gtkw_v2.md` (v2).

## 0. Verbindliche Entscheidungen gegenüber v2

1. **Keine Pflichtdatei `signals.yaml` und keine `-vcdConfig`-Option.** `til.json` enthält die strukturierten Signalnamen und deren Quelltypen. Diese Informationen genügen für die Signaldefinitionen.
2. **Signalidentität ist ausschließlich der kanonische Structured-Logging-Feldname**, nicht die Trice-ID, der Tag, der C-Typ oder der Formatstring. Alle Records, die ein gleichnamiges Feld enthalten, aktualisieren **ein einziges** VCD-Signal — ausdrücklich auch bei unterschiedlichen IDs, Integerbreiten, Vorzeichen und Float-Typen.
3. **Automatische Typvereinheitlichung vor Beginn der VCD-Ausgabe:** Kommt unter einem Feldnamen irgendwo im ausgewählten TIL-Schema ein Float vor, wird das gemeinsame Signal `real`; reine Integer-Signale werden als ausreichend breite Bitvektoren exportiert. Gemischte Quelltypen sind **kein Fehler**. Präzisions- und Anzeigegrenzen dokumentieren, nicht durch pauschale Verbote ersetzen.
4. **`trice generate -gtkw`** erzeugt einmalig eine wiederverwendbare GTKWave-Ansicht aus `til.json`. Es verarbeitet **keine** Capture-Daten, schreibt **keine** VCD-Datei und verändert keine Target-Quellen.
5. **Der geplante universelle `trice log -out vcd:<ziel>`** erzeugt die VCD-Wertänderungen mit demselben Decoder, gleichgültig ob die Eingabe endlich (`FILEBUFFER`) oder fortlaufend (UART/RTT/TCP ...) ist. Ziele: Datei oder `-` für `stdout`/Pipe.
6. **Beide Pfade verwenden denselben Schema-/Namensgenerator.** Ein aus `til.json` abgeleiteter Header und eine daraus erzeugte `.gtkw` müssen dieselben Signale und hierarchischen Namen enthalten.
7. **Tags sind optionale Auswahl- und Gruppierungsmerkmale, keine Signalidentität.** `info:{motor.rpm:%u}` und `wrn:{motor.rpm:%d}` betreffen dasselbe `motor.rpm`.
8. **Der Benutzer gilt als fachkundig.** Keine künstlichen Schemafehler bei plausibler, ausdrücklich gewünschter Typmischung. Warnungen nur dort, wo VCD technisch Grenzen hat, Werte verloren gehen oder die Zeitabbildung unbestimmt ist.
9. **Keine neuen Target-Makros, keine Änderungen am Wire-Format, `til.json`-Schema oder Instrumentierungsprozess.** Kein JSON-/CSV-Zwischenschritt und kein zweiter Binärdecoder.
10. `.gtkw` **nicht beim Logging erzeugen, aktualisieren oder überschreiben**. Ein Benutzer kann diese Datei in GTKWave manuell anpassen und dauerhaft wiederverwenden.

**Scopegrenze:** Dies ist **kein** Auftrag für ein vollständiges neues `-out`-Framework. Besteht im Arbeitsbranch ein universeller `-out`-Mechanismus, dort integrieren. Fehlt er, nur das zur VCD-Anbindung notwendige wiederverwendbare Routing schaffen; keine konkurrierende `-vcdOut`-Sonderlösung.

## 1. Benutzerworkflow ohne `signals.yaml`

### 1.1 GTKWave-Ansicht einmalig generieren

>Hinweise:
>
>Das v4 `.gtprj` Format wird nicht verfolgt derzeit, solange keine bidirektionalen Konverter existieren.
>
>Eine aufgezeichnete VCD wird empfohlen, da `til.json` obsolete Feldnamen enthalten kann. Mit VCD werden nur verwendete Feldnamen in die `.gtkw` übernommen.


```sh
# NEUE vorgeschlagene Option; Dateipfad explizit.
trice generate -gtkw=generated/session.gtkw -til=til.json
```

- Liest `til.json`, ermittelt sämtliche geeigneten benannten Structured-Logging-Felder, vereinheitlicht gleichnamige Felder und schreibt **nur** `generated/session.gtkw`.
- Die Ausgabe ist reproduzierbar; Signale nach kanonischem Feldnamen sortieren, nicht nach instabilen IDs oder Historie.
- Das Standard-Dumpfile in der View ist `session.vcd` (relativ zur `.gtkw`). Eine andere VCD kann beim Viewer-Aufruf explizit angegeben werden.
- Eine bereits bearbeitete `.gtkw` **nicht** ohne ausdrückliche Ersetzungsoption überschreiben. Gleicher Inhalt darf als „unchanged“ erkannt werden. Das konkrete Force-Flag an vorhandene CLI-Konventionen anpassen.
- `generate` braucht **keine** aufgezeichnete Datei und **keine** Ziel-Zeitbasis. GTKWave selbst muss zur Generierung nicht installiert sein.

### 1.2 Endliche Binärdatei nach VCD exportieren

>Hinweis:
>
>Die `-ts*` Switches sollten verwendet werden statt `-vcdTicks`. Wenn `-ts32` und `-ts16` kombiniert werden, muss `-ts16` mit den 16 MSB des vorherigen `-ts32` erweitert werden. Damit ist der zeitlich vorherige gemeint auch wenn dieser keinen Feldnamen enthält. 

```sh
# NEUE vorgeschlagene -out/-vcdTickHz-Syntax; vorhandene -p/-args/-til nutzen.
trice log -p FILEBUFFER -args capture.bin -til til.json \
  -vcdTickHz 1000000 -out vcd:generated/session.vcd

gtkwave generated/session.vcd generated/session.gtkw
```

Hier liefert die MCU einen monoton aufwärts laufenden 32-Bit-Target-Zähler mit **1 MHz**. `-vcdTickHz` ist die wirkliche Tickfrequenz des Targets — **nicht** automatisch aus `til.json` oder dem `TRice`-Makronamen ableitbar.

Andere Capture-Dateien können dieselbe Ansicht verwenden:

```sh
trice log -p FILEBUFFER -args morning.bin -til til.json \
  -vcdTickHz 1000000 -out vcd:captures/morning.vcd
trice log -p FILEBUFFER -args evening.bin -til til.json \
  -vcdTickHz 1000000 -out vcd:captures/evening.vcd

gtkwave captures/morning.vcd generated/session.gtkw
gtkwave captures/evening.vcd generated/session.gtkw
```

GTKWave-Versionen/Pfadauflösung mit einem echten Smoke-Test überprüfen; nicht ausschließlich aus dem Beispiel folgern, dass jede Viewer-Version denselben Aufruf gleich interpretiert.

### 1.3 Fortlaufender VCD-Export

```sh
trice log -p COM7 -til til.json \
  -vcdTickHz 1000000 -out vcd:generated/session.vcd
```

`log` schreibt den vollständigen VCD-Header **vor** den ersten Datensamples und danach laufend neue Wertänderungen. Der Dateimodus legt bei jedem Start einen neuen Stream an; **kein ungeprüftes Append** an eine ältere VCD-Datei.

### 1.4 Pipe statt Datei

```sh
# Endlicher Stream, beispielsweise für einen VCD-Consumer:
trice log -p FILEBUFFER -args capture.bin -til til.json \
  -vcdTickHz 1000000 -out vcd:- > session.vcd

# Direkter Viewer-Aufruf; stdin bedeutet nicht automatisch Echtzeitanzeige:
trice log -p FILEBUFFER -args capture.bin -til til.json \
  -vcdTickHz 1000000 -out vcd:- | gtkwave -v

# Optionale Live-Anzeige unter GTKWave 3.3 mit dem LEGACY-Mechanismus:
trice log -p COM7 -til til.json \
  -vcdTickHz 1000000 -out vcd:- \
  | shmidcat | gtkwave -v -I generated/session.gtkw
```

- Für `-out vcd:-` ist `stdout` **ausschließlich VCD**, niemals mit normalen Trice-Textzeilen, Banner oder Statistiken vermischen. Diagnose nach `stderr` oder in einen getrennten Sink.
- Ausgabe regelmäßig flushen; VCD nicht bis EOF vollständig puffern.
- `gtkwave -v` liest VCD von `stdin`, ist ohne den interaktiven Modus aber keine Garantie für laufende Darstellung.
- `shmidcat`/`-I` ist GTKWave-3.3-spezifisch. In der GTKWave-4-Entwicklung wurde dieser interaktive Modus entfernt; keine GTKWave-4-Live-Unterstützung behaupten.
- Trice soll weder `gtkwave` noch `shmidcat` selbst starten. `-out` liefert Format + Ziel, nicht eine anwendungsspezifische Viewer-Verbindung.

### 1.5 Optionale Tagfilter, **ohne** neue Konfigurationsdatei

Bestehende `trice log -pick`/`-ban`-Semantik möglichst für die VCD-Recordauswahl wiederverwenden:

```sh
# Nur Motor-Records exportieren; 'motor' als Anwendungstag registrieren.
trice log -p FILEBUFFER -args capture.bin -til til.json \
  -utag motor -pick motor -vcdTickHz 1000000 \
  -out vcd:motor.vcd
```

Die hier gezeigte `-utag`/`-pick`-Verwendung ist in `log` grundsätzlich bereits bekannt; Codex muss ihre **Auswirkungen auf VCD-Headerschema und Sample-Auswahl** explizit festlegen und testen. Wenn die Ansichtsdatei ebenfalls nach Tags gefiltert werden soll, kann `generate` eine kompatible Auswahl anbieten (z. B. die dort neu registrierten `-pick`/`-ban`-Flags). **Keine neue YAML-Datei dafür fordern.** Standard: alle unterstützten Felder.

## 2. Konkrete, heterogene Instrumentierung

Die nachstehenden Trice-Aufrufe stehen exemplarisch an **verschiedenen Programmstellen** und können mit völlig unterschiedlichen Frequenzen auftreten. `TRice` nutzt im Beispiel 32-Bit-Target-Timestamps; Trice-Bitbreiten werden wie in der echten API über `TRice8`, `TRice16`, `TRice32`, `TRice64` gewählt. Vor Benutzung der Demo müssen Bind/Insert und Firmware-Build wie gewohnt stattfinden.

```c
// VORAUSSETZUNG: TriceStamp32() liefert einen 1-MHz-Aufwärtszähler.

// 0 Nutzwerte: Marker im normalen Log. Ohne benanntes Feld kein VCD-Signal.
TRice("info:boot complete");

// 1 Wert: Motor-Drehzahl im Regelzyklus.
TRice32("motor:Speed {motor.rpm:%u}", rpm);

// 2 Werte: Strom und Batteriespannung, seltener aufgezeichnet.
TRice32("power:Current {power.current_mA:%u}, supply {power.supply_mV:%u}",
        currentMA, supplyMV);

// 3 Werte: Beschleunigungsvektor, oft als Block protokolliert.
TRice32("imu:Accel {imu.ax_g:%.3f} {imu.ay_g:%.3f} {imu.az_g:%.3f}",
        aFloat(ax), aFloat(ay), aFloat(az));

// 4 Werte: Steuerungsstatus, weitere unabhängige VCD-Signale.
TRice32("ctrl:Mode {ctrl.mode:%u}, faults {ctrl.fault_flags:%08x}, "
        "cycle {ctrl.cycle:%u}, demand {ctrl.demand:%u}",
        mode, faultFlags, cycle, demand);

// Ein Wert, zustandsorientiert; so entsteht ohne Event-Mapping ein IRQ-Intervall.
TRice8("irq:TIM2 {irq.TIM2_active:%u}", 1);
// ... ISR-Arbeit ...
TRice8("irq:TIM2 {irq.TIM2_active:%u}", 0);

// Sporadische Sensortemperatur.
TRice32("sensor:Temperature {sensor.temperature_c:%.2f}", aFloat(tempC));

// Signierte Achsenposition.
TRice32("axis:Position {axis.position_steps:%d}", positionSteps);

// WICHTIG: Verschiedene IDs und Trice-Werttypen schreiben bewusst dasselbe 'x'.
TRice8 ("info:Source A {x:%u}", u8);                 // z. B. 42
TRice32("wrn:Source B {x:%d}", i32);                // z. B. -17
TRice32("dbg:Source C {x:%.3f}", aFloat(valueF));  // z. B. 3.14159
```

**Erwartete Signale aus den benannten Feldern:**

```text
axis.position_steps
ctrl.cycle
ctrl.demand
ctrl.fault_flags
ctrl.mode
imu.ax_g
imu.ay_g
imu.az_g
irq.TIM2_active
motor.rpm
power.current_mA
power.supply_mV
sensor.temperature_c
x
```

Das sind **14** VCD-Signale, obwohl deutlich mehr Trice-Aufrufstellen existieren. `x` kommt aus **drei verschiedenen IDs/Quelltypen**, wird aber genau **einmal** deklariert.

Der nullparametrige Boot-Record taucht **nicht** als eigenständiges Signal auf; sein Target-Zeitstempel darf dennoch den Zeitursprung bestimmen. Wer ein IRQ-Intervall sehen will, schreibt die oben gezeigten **expliziten benannten Zustandswerte**. Eine automatische Interpretation freier Texte wie `TIM2 enter`/`TIM2 exit` gehört **nicht** zum v3-MVP. Optional später Ereignismarker, ohne eine Pflicht-YAML einzuführen.

## 3. Warum `til.json` als einzige Signaldefinition genügt

Ein TIL-Eintrag enthält die `Type`- und `Strg`-Informationen. Beispiel **illustrativ** (IDs sind keine festen Produktiv-IDs):

```json
{
  "1010": {"Type":"TRice8",  "Strg":"info:Source A {x:%u}"},
  "2050": {"Type":"TRice32", "Strg":"wrn:Source B {x:%d}"},
  "3090": {"Type":"TRice32", "Strg":"dbg:Source C {x:%.3f}"}
}
```

Der vorhandene Structured-Template-Parser ermittelt daraus:

| TIL-ID | Feldname | Typ der Nutzwerte | Zielsignal |
|---:|---|---|---|
| 1010 | `x` | uint8 | `x` |
| 2050 | `x` | int32 | `x` |
| 3090 | `x` | float32 | `x` |

Es ist **gewollt**, dass diese Aufrufstellen unterschiedliche IDs haben; für VCD ist die **Feldgleichheit** entscheidend.

### 3.1 Kanonischer Feldname als Schlüssel

- Signaltabelle: `map[canonicalFieldName]SignalDefinition` statt `map[TriceID]SignalDefinition`.
- Jede TIL-ID kann **mehrere** benannte Felder aktualisieren; ein Feld kann von **beliebig vielen** IDs aktualisiert werden.
- Gleiche Feldnamen sind an allen Stellen **per Definition ein Signal**. Kein Fehler „mehrdeutiger Name“, keine ID-Konfiguration, kein Opt-in für Zusammenführung.
- `info:`, `wrn:`, `motor:`, `imu:` usw. können **die Auswahl** von Records beeinflussen, nicht die Identität des Felds.
- `motor.rpm` bleibt von `rpm` und `other.rpm` verschieden. Punktnotation bezeichnet bereits in Structured Logging eine kanonische flache Feldzeichenkette; der VCD/GTKWave-Namensgenerator darf sie optional als Hierarchie darstellen, muss sie aber in **beiden** Artefakten identisch abbilden.
- Wenn `til.json` historische Einträge enthält, dürfen auch diese beitragen. Ob der Anwender nur aktuelle Felder sehen möchte, ist eine **optionale Auswahlfunktion**, kein Grund für eine Pflichtkonfiguration.
- **Typkonflikte nicht als Schemafehler behandeln.** Nur technisch nicht unterstützte Inhalte (etwa gemischte Strings/Numeric) bedürfen einer klar dokumentierten Auslassung oder Warnung.

### 3.2 Gemeinsam verwendbare Definitionen

Vor dem ersten VCD-Byte bzw. dem Generieren der `.gtkw` aus **demselben TIL-Snapshot** ermitteln:

1. Alle erfolgreich parsebaren Structured-Logging-Schemas und ihre kanonischen Feldnamen.
2. Je Feld alle möglichen ID-/Typquellen.
3. Den gemeinsamen VCD-Typ und die Breite nach Abschnitt 4.
4. Einen stabilen, kollisionsfreien VCD-Namen (und gegebenenfalls seine Scope-Hierarchie).
5. Einen stabilen VCD-Identifier pro Signal nach deterministischer Sortierung.

Die Signaldefinitionen sind ein **Schema-Snapshot**. Bei einem live veränderten TIL-File dürfen nach dem `$enddefinitions`-Header keine neuen Signale mitten in einen bestehenden Stream eingeführt werden. Änderungen verlangen eine neue VCD-Aufzeichnung; vorhandene Signale können weiter aktualisiert werden, sofern die Dekodierung zum Snapshot passt.

## 4. Typvereinheitlichung und VCD-Format

VCD deklariert den Signaltyp **vor** den Samples. Deshalb muss die Typvereinigung aus **allen zu einem Namen gehörenden TIL-Quellschemas** erfolgen, nicht erst anhand des ersten eingehenden Werts.

| Quellkombination | Vorgeschlagene VCD-Repräsentation | Semantik |
|---|---|---|
| Nur `bool` | `wire 1` | 0/1 |
| Nur vorzeichenlose und/oder vorzeichenbehaftete Integer, auch gemischte Bitbreiten | `wire N` bzw. geeignetes VCD-Integer/Vector-Format | N = erforderliche gemeinsame Breite, Werte als Bitmuster/Zweierkomplement |
| Mindestens ein `float32` oder `float64`, mit beliebigen Integern | `real 64` | Alle Integer/Float-Updates werden als Dezimalzahl ausgegeben |
| `bool` gemischt mit Integer | Integer-Vektor | 0/1 und Integerwerte im gleichen Signal |
| `bool`/Integer/Float gemischt | `real 64` | Alles numerisch, ohne Typblockade |
| Strings/Pointer/Buffer als alleiniger oder gemischter Feldtyp | zunächst außerhalb des MVP | Gezielte Warnung oder dokumentierte Auslassung; keine willkürliche Text-/Adresskonvertierung |

**Wichtige Grenze bei Ganzzahlen:** `real` entspricht praktisch einer Double-Darstellung; Integer oberhalb `2^53` sind möglicherweise nicht exakt. Das ist **eine bewusste Formatgrenze**, kein Anlass, uint8 + int32 + float zu verbieten. Bei betroffenen 64-Bit-Werten eine verständliche Warnung (einmal je betroffenem Signal, nicht pro Record) ausgeben. Für reine Integer, vor allem 64-Bit-Werte, Bitvektoren zum verlustfreien Transport beibehalten.

**Vorzeichen bei gemischten Integer-Vektoren:** VCD-Vektoren sind zunächst Bitfolgen. GTKWave interpretiert deren Anzeige je nach gewähltem Zahlenformat. Bei Mischung großer unsigned und negativer signed Werte ist eine **einzige** perfekte Dezimal-Interpretation nicht immer möglich. Im MVP Bitwerte verlustfrei erhalten und die Darstellungsentscheidung bzw. Einschränkung dokumentieren; **nicht** wegen gemischter Vorzeichen abbrechen. Optional später eine alternative numerische `real`-Darstellung oder explizite Anzeigeoption.

### 4.1 Konkreter `x`-Dump

Für drei Records mit `x=42` (uint8), `x=-17` (int32) und `x=3.14159` (float) und Target-Zeiten 100/120/150 µs ist **ein** Signal `x` als `real` zu deklarieren:

```vcd
$date
  Trice VCD export
$end
$version Trice $end
$timescale 1us $end
$scope module trice $end
$var real 64 ! x $end
$upscope $end
$enddefinitions $end
#100
r42 !
#120
r-17 !
#150
r3.14159 !
```

`!` ist ein **VCD-Symbol**, keine Trice-ID. VCD-Kommentare und Datumsangaben dürfen die Reproduzierbarkeit nicht durch aktuelle Hostzeit zerstören. Das Beispiel ist **ein reduzierter Ein-Signal-Dump**, nicht das 14-Signal-Gesamtergebnis.

### 4.2 Änderungen und Initialzustände

- Gleicher Name, egal von welcher ID → Wertänderung desselben Signals.
- Mehrere Felder in einem Trice-Record → mehrere Updates mit **demselben** Timestamp.
- Zwischen Updates bleibt der zuletzt beobachtete Wert bestehen; keine Interpolation.
- Vor dem ersten Sample ist ein digitales Signal unbekannt (`x`); ein `real`-Signal darf **nicht** willkürlich auf Null initialisiert werden. VCD-konforme Initialdarstellung mit Parser und GTKWave testen.
- Ein wiederholter identischer Wert kann weggelassen werden. Bei **derselben** Zeit und mehreren Updates desselben Signals gilt deren Reihenfolge; aufeinanderfolgende Records nicht ungeprüft umsortieren.
- Float-Werte so formatieren, dass Präzision erhalten bleibt; `NaN`/`Inf` gesondert und VCD-konform behandeln, keinesfalls stillschweigend als 0 darstellen.
- TIL-Parser und Decoder liefern den ursprünglichen Datentyp; **nicht** aus fertigen Logtexten/JSON Strings zurückinterpretieren.

## 5. Zeitbasis: bewusst ohne YAML, aber nicht erraten

Trice überträgt 0-/16-/32-Bit-Target-Stamps je nach Makro/Setup; die **Ticks pro Sekunde** sind targetabhängig. `til.json` beschreibt keine universelle Echtzeitfrequenz.

Minimaler VCD-Schalterentwurf:

```sh
-vcdTickHz 1000000
```

- Für den MVP Target-Stamps verwenden, zunächst einen klar spezifizierten 32-Bit-Aufwärtszähler. Dokumentieren, dass `TRice` 32-Bit-Stamp-**Breite** und nicht automatisch eine Einheit bedeutet.
- Die VCD-Zeit aus **dem ersten erfolgreich dekodierten, zeitgestempelten Record** der Aufnahme ableiten, auch wenn er kein benanntes Signal besitzt; zum Beispiel `info:boot complete`.
- Stamps **vor** Record-/Tag-Auswahl normalisieren, sonst können fehlende Zwischenereignisse Wrap-Erkennung und Zeitnull verfälschen.
- 32-Bit-Umläufe mit Modulus `2^32` fortlaufend rekonstruieren, soweit aufgrund der Ereignisdichte eindeutig; keine unsichtbare Annahme von mehrfachen Umläufen zwischen zwei Records.
- Bei nichtmonotonen Zeiten, Reset oder Counter-Anomalien nicht stillschweigend fiktive Messzeiten ausgeben. Klare Diagnose/Policy.
- Ungestempelte Records können **keine** verlässliche VCD-Zeit liefern; als ausgewählte Samples im MVP diagnostiziert überspringen. Keine Host-Empfangszeit als stillschweigender Ersatz.
- Mischen von `ts16` und `ts32` nur mit dokumentierter gemeinsamer Zeitbasis; im MVP klare Fehlermeldung für eine unauflösbare Kombination. Erweiterungen mit `unwrap`/Downcounter bzw. existierenden Stamp-Expressions als separaten Schritt planen, **ohne** YAML-Pflicht einzuführen.
- Geeignete `$timescale` wählen (z. B. `1us` für einen 1-MHz-Zähler); Rundung möglichst mit Integer-/rationaler Arithmetik minimieren. Vor allem Monotonie sicherstellen und die eventuelle Quantisierung dokumentieren.

Die Konfiguration von Tickrate und ggf. zukünftiger Counter-Transformation gehört **zur Log/Out-CLI oder zum bereits bestehenden Stamp-Expressions-Mechanismus**, nicht in eine erzwungene zusätzliche Datei.

## 6. `trice generate -gtkw`: dauerhafte Ansicht

Aus dem TIL-Snapshot resultiert beispielsweise:

```text
[*] GTKWave save file generated by Trice
[dumpfile] "session.vcd"
@28
trice.axis.position_steps
trice.ctrl.cycle
trice.ctrl.demand
trice.ctrl.fault_flags
trice.ctrl.mode
trice.imu.ax_g
trice.imu.ay_g
trice.imu.az_g
trice.irq.TIM2_active
trice.motor.rpm
trice.power.current_mA
trice.power.supply_mV
trice.sensor.temperature_c
trice.x
```

**Achtung:** Das ist ein **schematischer Auszug**, keine behauptete vollständige GTKWave-Dateispezifikation. In tatsächlichen `.gtkw`-Dateien `@`-Flags, Hierarchien, `real`-Anzeigen und `radix` anhand eines realen Parsers/GTKWave 3.3 prüfen. Referenz: https://github.com/RTEdbg/FreeRTOS-trace-demo/blob/master/Test/output/RTOS_timing.gtkw

- Namenshierarchie aus Punkten (`imu.ax_g` → `trice.imu.ax_g`) nur mit der **gleichen** logischen Abbildung im VCD-Header. Fälle wie gleichzeitiges `motor` und `motor.rpm`, Sonderzeichen oder Namenskollisionen deterministisch und ohne Datenverlust lösen.
- Kein hartkodiertes VCD-Ziel im `log`-Pfad. Das `.gtkw` verweist auf den relativen Default `session.vcd`; andere Capture-Dateien beim GTKWave-Aufruf übergeben.
- Gleicher TIL-Inhalt und gleiches Ziel → byteidentische `.gtkw`. Änderungen an TIL-IDs ohne Änderung von **Signalnamen/-typen** sollen nach Möglichkeit keine View-Diffs erzeugen.
- Vorhandene manuell bearbeitete Ansicht nicht ungefragt überschreiben. `trice log` ändert die Ansicht **nie**.
- Später möglich: GTKWave 4 verwendet perspektivisch das JSON-basierte `.gwprj`-Projektformat, `.gtkw` ist dort nur noch Import. **Kein `.gwprj`-Generator im v3-MVP.** Quelle: https://github.com/gtkwave/gtkwave/issues/525

## 7. `trice log -out vcd:<ziel>`: derselbe Writer für Datei und Stream

Die Ausgabe ist eine **zustandsbehaftete** Senke auf den vorhandenen **typisierten ApplicationRecords**, nicht auf Textausgabe oder `-vis`-`printf`-Ergebnissen.

1. Vor Aufnahme: passendes `til.json` lesen, Struktur-Schemas auswerten, alle Feldnamen zusammenführen, Typen promoten, vollständige `$var`-Definitionen erstellen.
2. Einmaligen Header ausgeben, **bevor** Datensamples aus dem Binärdecoder eintreffen; bei laufender Datei sofort sichtbar und bei Pipes ohne EOF empfangbar.
3. Für jeden erfolgreich dekodierten Record: Timestamp normalisieren, die benannten Felder und ihre Werte übernehmen und die entsprechenden VCD-Signale fortschreiben. **Die Trice-ID steuert allein den Decoder**, nicht die Zielsignalidentität.
4. Tagfilter optional auf Recordebene anwenden; unselektierte Records können für die Zeitnormalisierung dennoch relevant sein. Die `.gtkw`-Namen bleiben von Tags unabhängig.
5. Für den Stream bereits deklarierte Signale verwenden. Historische/newly appended TIL-Einträge während eines laufenden Streams dürfen die Deklarationen nicht nachträglich ändern. Kommt ein zur Snapshot-Definition inkompatibles Update, angemessen diagnostizieren und Signalupdate auslassen bzw. neue Aufnahme verlangen.
6. Bei endlicher Eingabe bis EOF: flush/close, valide VCD hinterlassen. Bei kontinuierlicher Eingabe: in sinnvollen Intervallen flushen, bis kontrolliert beendet.
7. `stdout`-Ziel sauber exklusiv behandeln; Fehler und Logmeldungen nach `stderr`. Keine zwei Ausgabekanäle ungeplant in einen Pipe-Stream mischen.
8. Ziel-Datei mit definierter Neuanlage-/Überschreibungsregel öffnen, nicht an alte VCD anhängen. Bei Fehlern keinen Erfolg vortäuschen; I/O-Fehler und `EPIPE` kontrolliert propagieren.
9. Große Aufzeichnungen streamingfähig verarbeiten, mit Speicherverbrauch proportional zur Zahl der Signale, **nicht** zur Gesamtlänge des Captures.
10. Die universelle `-out`-Planung berücksichtigen, aber weder eine viewer-spezifische GTKWave-Senke noch eine zweite Dekodierpipeline erschaffen.

## 8. Umgang mit Tags und optionalen Auswahlen

**Tags helfen beim Selektieren, ersetzen aber keine Signal-IDs/-Namen.** Aktuelles Trice-Verhalten beachten: Vorangestellte Tags aus `Strg` wie `wrn:`/`info:` werden hostseitig klassifiziert; benutzerdefinierte Taggruppen werden über `-utag` registriert.

- Standard: **alle geeigneten benannten numerischen Felder** aus `til.json` sind VCD-Signale. Auch wenn nur ein Teil davon in einer konkreten Capture vorkommt, ist das korrekt; nicht auftretende Werte bleiben unbekannt.
- `-pick` und `-ban` selektieren **Records**, nicht Felder. Kommt `x` unter drei Tags vor und wird nur `wrn` ausgewählt, schreibt dieser Export folgerichtig nur die `wrn`-Updates für **dasselbe** `x`.
- Optional `generate`-Auswahl nach Tags einbauen, sobald die `-pick`/`-ban`-Semantik dafür klar geregelt ist. Ist die Ansicht eine Teilmenge der VCD-Signale, darf sie trotzdem funktionieren; eine Inkonsistenz der Signalnamen ist dagegen ein Fehler.
- Nicht automatisch alle gewöhnlichen `info`-/`wrn`-Records als VCD-Signale behandeln. **Nur benannte Structured Fields** werden numerische Signale.
- Ein getrenntes Einmalwerkzeug zur Signalauflistung ist denkbar (z. B. `trice generate -listSignals`), aber **keine notwendige Vorbedingung** für den Export und kein zusätzliches Pflichtformat.
- Bestehendes `generated/trice-fields.txt` zählt aktuelle Source-Sites, aber bildet **nicht** zwingend die gesamte historische Signalmenge des `til.json` ab. Es ist daher **kein** Pflichtinput für diesen Export.

## 9. Hardwarefreie Demo: `examples/VCD/`

```text
examples/VCD/
  README.md
  main.c
  triceConfig.h
  til.json            # regulär von bind/insert erzeugt
  run.sh
  expected/           # reproduzierbare Referenzwerte
  generated/
    session.gtkw      # statisch, von generate
    session.vcd       # erst beim log-Replay erzeugt
```

An `examples/PC_features/` orientieren: native Target-Simulation, echte Trice-Binärdatenerzeugung, keine handerfundenen produktiven IDs. **Kein `signals.yaml`.**

Das Demo muss zeigen:

1. `TRice` ohne Argument für einen Boot-Record, der bei Zeitnull mitwirken kann, aber kein numerisches Signal erzeugt.
2. Trices mit **1, 2, 3, 4** unabhängigen Feldern und unterschiedlichen Updatefrequenzen.
3. Ein Zustandsintervall (`irq.TIM2_active`) über **zwei 1-Wert-Records** (0/1), ohne Textmuster-/Event-Mapping.
4. Dasselbe `x` aus **drei verschiedenen IDs** (`uint8`, `int32`, `float32`) mit dem erwarteten einzigen `real`-Signal.
5. Eine VCD-Datei aus einem endlichen `capture.bin` und eine VCD-Pipe via `stdout`.
6. Eine **zweite** Capture-Datei, die dieselbe `generated/session.gtkw` ohne Neugenerierung öffnet.
7. Dokumentierte Grenzen für GTKWave 3.3-Live-Pipe und GTKWave-4-Änderungen, ohne GTKWave in CI zwingend vorauszusetzen.

Beispiel für `run.sh`:

```sh
# 1. Bind / nativen Demo-Build / Erzeugung capture.bin ausführen.
# 2. Einmalig View erzeugen:
trice generate -gtkw=generated/session.gtkw -til=til.json
# 3. Capture-Daten exportieren:
trice log -p FILEBUFFER -args capture.bin -til til.json \
  -vcdTickHz 1000000 -out vcd:generated/session.vcd
# 4. Optional anzeigen:
gtkwave generated/session.vcd generated/session.gtkw
```

## 10. Abnahmetests

### 10.1 Schema und Typmischung

- Gleicher Feldname in verschiedenen TIL-IDs → **genau eine** `$var`-Deklaration und ein GTKWave-Signal.
- `x` aus `uint8`, `int32`, `float32` → VCD-`real` und die Werte `42`, `-17`, `3.14159` in zeitlicher Reihenfolge. **Kein Fehler wegen unterschiedlicher IDs oder Typen**.
- Derselbe Feldname aus `info`, `wrn` und registrierten benutzerdefinierten Tags → **ein** Signal; Tagwahl steuert bei optionalem Filter die Updates, nicht die Identität.
- Gleichnamige ausschließlich ganzzahlige Felder mit unterschiedlichen Breiten → gemeinsame verlustfreie Bitbreite, inklusive Zweierkomplement. Gemischte Vorzeichen als Anzeigegrenze dokumentieren.
- 64-Bit-Integer über `2^53`, die wegen eines anderen Quelltyps nach `real` konvertiert werden → keine heimliche Exaktheitsgarantie, mindestens gezielte Präzisionsdiagnose.
- 0/1/2/3/4 Werte pro Trice-Aufruf und unregelmäßige Samples; bei einem 3er-/4er-Record identische Timestamps der aktualisierten Felder.
- Keine Signalkonfiguration nötig. TIL mit ausschließlich unbenannten Records liefert einen klar diagnostizierten leeren/nicht nutzbaren VCD-Signalbestand, aber nicht den Vorschlag, man solle eine YAML-Datei schreiben.
- Kollisionen bei Normalisierung von Feldnamen/Hierarchien ohne Datenverlust behandeln; Namensgleichheit der tatsächlichen kanonischen Felder ist hingegen **gewollt**.

### 10.2 GTKWave-Ansicht

- `generate -gtkw` erzeugt ohne Capture-Datei nur `.gtkw`, **keine** VCD.
- `generate` erzeugt alle 14 Beispielsignale in stabiler Reihenfolge; `x` nur einmal, unabhängig von ID-Historie.
- `log` berührt `.gtkw` nicht; bei zwei unterschiedlichen Captures bleibt die manuell bearbeitete View bytegleich.
- Vorhandene `.gtkw` nicht ohne ausdrückliches Opt-in überschreiben. Änderungen an `til.json`/Auswahl erfordern bewusstes Neugenerieren.
- Ein echter GTKWave-3.3-Smoke-Test öffnet beide Dateien, **wenn** GTKWave verfügbar ist; andernfalls Dateisyntax unabhängig prüfen und fehlenden Smoke-Test benennen.

### 10.3 VCD-Datei, Streams und Zeit

- Endliche `FILEBUFFER`-Eingabe erzeugt bei EOF ein gültiges VCD mit allen erwarteten Signalen.
- Fortlaufende Quelle schreibt Header **sofort**, danach Updates regelmäßig und **vor** EOF sichtbar.
- `-out vcd:-` enthält ausschließlich VCD; `stderr` enthält Diagnosen; Pipe-Ergebnis stimmt mit Dateimodus überein.
- 32-Bit-Aufwärtszähler mit `-vcdTickHz 1000000`: richtige relative Zeiten, auch über einen eindeutig erkennbaren Wrap; nicht passende Stamps und Resetfehler werden diagnostiziert.
- Unbenannte Boot-Records dürfen Zeitnull setzen; andere Records ohne ausgewählte Felder dürfen die Counterhistorie erhalten.
- Wiederholte Werte, gleichzeitige Updates, unbekannte Initialzustände, `real`-Format, ungültige Floats, Dateischreibfehler, `EPIPE`, Trice-Framingfehler, inkompatible/fehlende TIL testen.
- Beim Pipe-Modus **nicht** erst zum Schluss alles ausgeben; FIFO/Slow-Consumer-Test.
- Großer Capture: Speicherkosten hauptsächlich nach Signalanzahl, nicht Recordzahl; CPU-Aufwand linear in Events.

### 10.4 Regression und CLI

- Bestehende `trice log`, `trice generate`, `bind`, `insert`, `clean`, Structured Logging, `-vis`, NDJSON/KV und normale Textausgabe unverändert funktional.
- Universeller `-out`-Plan respektiert; keine zusätzliche `-vcdOut`-CLI, keine Pflichtdatei `signals.yaml`, kein `-vcdConfig`.
- Anderen `generate`-Modi (`-logC`, `-abc`, `-onelineJSON`, `-bindReport`, `-colors`) nicht beschädigen; Modusexklusivität sauber definieren.
- C-/Go-Tests, Pipeline-Tests, unabhängiger VCD-Parser und (wenn vorhanden) GTKWave-Smoke-Test durchführen; tatsächlich ausgeführte Prüfungen im Abschlussbericht nennen.

## 11. Implementierungsorientierung und vorhandene Referenzen

Prüfen und möglichst **wiederverwenden**:

- [`internal/decoder/record.go`](https://github.com/rokath/trice/blob/main/internal/decoder/record.go): `ApplicationRecord` mit `ID`, `Fields`, `Stamp`, `StampBits`; `FieldValue.Value` bereits typisiert.
- [`internal/trexDecoder/trexDecoder.go`](https://github.com/rokath/trice/blob/main/internal/trexDecoder/trexDecoder.go): TREX-Binärdecoder und Ursprung der Wire-Typinformationen.
- [`internal/id/structured.go`](https://github.com/rokath/trice/blob/main/internal/id/structured.go), [`internal/fmtspec/`](https://github.com/rokath/trice/tree/main/internal/fmtspec): Kanonisierung und Parse der Structured Fields.
- [`internal/translator/translator.go`](https://github.com/rokath/trice/blob/main/internal/translator/translator.go): bestehender Ereignispfad, EOF-/Live-Verhalten.
- [`internal/vis/router.go`](https://github.com/rokath/trice/blob/main/internal/vis/router.go): bestehendes Sink-Konzept, jedoch nicht dessen Text-`printf` als VCD missbrauchen.
- [`internal/args/init.go`](https://github.com/rokath/trice/blob/main/internal/args/init.go), [`internal/args/handler.go`](https://github.com/rokath/trice/blob/main/internal/args/handler.go), [`internal/id/generate.go`](https://github.com/rokath/trice/blob/main/internal/id/generate.go): bestehende CLI und Generator-Modi.
- [`docs/rm/structured_logging.md`](https://github.com/rokath/trice/blob/main/docs/rm/structured_logging.md), [`docs/rm/tags_color_and_weights.md`](https://github.com/rokath/trice/blob/main/docs/rm/tags_color_and_weights.md), [`docs/rm/stamp_expressions.md`](https://github.com/rokath/trice/blob/main/docs/rm/stamp_expressions.md).
- GTKWave-.gtkw-Beispiel: https://github.com/RTEdbg/FreeRTOS-trace-demo/blob/master/Test/output/RTOS_timing.gtkw
- GTKWave interaktiver VCD-Modus: https://github.com/gtkwave/gtkwave/issues/450 ; Wiederherstellungsversuch: https://github.com/gtkwave/gtkwave/pull/452
- GTKWave 4 `.gwprj`: https://github.com/gtkwave/gtkwave/issues/525

**Erforderliche Dokumentation:** `trice help -generate` für `-gtkw`; `trice help -log` für `-out vcd:...`, `-vcdTickHz` und `stdout`; [`docs/rm/generate.md`](https://github.com/rokath/trice/blob/main/docs/rm/generate.md), [`docs/rm/visualization.md`](https://github.com/rokath/trice/blob/main/docs/rm/visualization.md) und `examples/VCD/README.md` mit gemischten Quelltypen und ausdrücklich **ohne** `signals.yaml`.

## 12. Nicht Teil von v3

- `signals.yaml` oder sonstige Pflicht-Konfigurationsdateien, `-vcdConfig`, `trice generate -vcd` aus Binärlogs.
- Automatische `.gtkw`-Neuerzeugung bei jedem `trice log`.
- Trigger-/Pre-/Post-Trigger-Recorder, Post-mortem-RAM-Flight-Recorder, zusätzliche Target-Makros, Änderungen am TREX-Protokoll.
- Ein eigener GTKWave-Live-Viewer, GTKWave-4-Live-Protokoll, automatische Viewer-Prozessverwaltung.
- `.gwprj`-, FST- oder allgemeine CSV-Output-Implementierung im selben Change.
- Freitext-basiertes automatisches Enter/Exit-Pairing oder implizite semantische Zuordnung verschiedener Feldnamen.
- Generelle Neugestaltung aller Trice-Port-/TCP-/Control-CLI-Optionen.

## 13. Definition of Done

**Ohne `signals.yaml`** kann ein Nutzer einmalig mittels `trice generate -gtkw` aus `til.json` eine GTKWave-Ansicht erstellen und anschließend **beliebig viele** endliche oder laufende Trice-Binärströme über den universellen `trice log -out vcd:<Datei|->` in eine passende VCD-Datei oder Pipe ausgeben. **Alle gleichnamigen Structured-Logging-Felder aktualisieren dasselbe Signal**, auch wenn Trice-ID, Tag, Bitbreite, Vorzeichen oder Integer-/Float-Typ unterschiedlich sind. Der Nutzer erhält echte VCD-Werte, passende GTKWave-Signalnamen und korrekte Target-Zeiten, ohne die Firmware speziell für VCD ändern oder Konfigurationslisten pflegen zu müssen.

**Codex-Auftrag:** Zuerst vorhandene CLI-/`-out`-Planung und Decoder-Hooks prüfen; dann gemeinsamen TIL-Signalindex und Typvereinheitlichung implementieren; danach `generate -gtkw`, zustandsbehafteten VCD-Writer, `log -out` für Datei/`stdout`, Zeitabbildung, heterogene PC-Demo sowie Abnahme-/Regressionstests. Abschlussbericht mit Änderungen, Testkommandos, tatsächlich geprüfter GTKWave-Version und bekannten Einschränkungen liefern.
