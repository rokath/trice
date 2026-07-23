
# Trice → Serial Studio: TSS-Entwurf

Stand: 2026-07-10

<h2>Inhaltsverzeichnis</h2>

<!-- mdtoc -->

- [1. WICHTIG](#wichtig)
- [2. Ziel](#ziel)
- [3. C-Syntax](#c-syntax)
- [4. Adressierung](#adressierung)
- [5. Reicht diese Syntax für Serial Studio?](#reicht-diese-syntax-für-serial-studio)
- [6. tlog und TSS zur Laufzeit](#tlog-und-tss-zur-laufzeit)
- [7. Primärformat: MessagePack Map](#primärformat-messagepack-map)
  - [7.1. MessagePack Map, Beispiel uint](#messagepack-map-beispiel-uint)
  - [7.2. MessagePack Map, Beispiel float](#messagepack-map-beispiel-float)
  - [7.3. MessagePack Map, Wertcodierung](#messagepack-map-wertcodierung)
- [8. Serial-Studio-Konfiguration für MessagePack Map](#serial-studio-konfiguration-für-messagepack-map)
- [9. Alternative: Binary TLV](#alternative-binary-tlv)
- [10. Alternative: Raw Bytes](#alternative-raw-bytes)
- [11. Alternative: ASCII Key-Value](#alternative-ascii-key-value)
- [12. Alternative: Protobuf](#alternative-protobuf)
- [13. trice generate](#trice-generate)
- [14. my.ssproj bei neuen Projekten](#myssproj-bei-neuen-projekten)
- [15. my.ssproj bei bestehenden Projekten](#myssproj-bei-bestehenden-projekten)
- [16. Sidecar my.ssproj.trice.json](#sidecar-myssprojtricejson)
- [17. Backup-Regel](#backup-regel)
- [18. Merge-Regeln](#merge-regeln)
- [19. .ssproj-Strukturstabilität](#ssproj-strukturstabilität)
- [20. KI-Agent für Legacy-Migration](#ki-agent-für-legacy-migration)
- [21. KI-Agent-Prompt Entwurf](#ki-agent-prompt-entwurf)
- [22. Mini-Demo, nur skizziert](#mini-demo-nur-skizziert)
- [23. Beispielprojekte zum Testen des KI-Agenten](#beispielprojekte-zum-testen-des-ki-agenten)
- [24. Vorläufige Festlegung](#vorläufige-festlegung)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

## 1. <a id="wichtig"></a>WICHTIG

>Das ist lediglich ein erster Entwurfsversuch. Er wird aktuell nicht weiter verfolgt, zugunsten anderer Tools.

## 2. <a id="ziel"></a>Ziel

Ausgewählte Trice-Messages sollen zusätzlich zur normalen `tlog`-Anzeige als Datenstrom für Serial Studio ausgegeben werden.

Der Embedded-Code bleibt Trice-Code. Serial-Studio-spezifische Details liegen auf Host-Seite:

```txt
Target
  -> Trice-Binärstrom
  -> tlog / TSS
      -> normale Logausgabe
      -> binärer Serial-Studio-Datenstrom
  -> Serial Studio
```

TSS bedeutet hier: **T**rice to **S**erial **S**tudio.

Primärer Zielpfad ist Trice binär - Serial-Studio binär (B-B):

```txt
Trice binary -> TSS -> Serial-Studio binary
```

ASCII bleibt nur als optionale Migrations-/Debug-Ausgabe B-A:

```txt
Trice binary -> TSS -> Serial-Studio ASCII
```

## 3. <a id="c-syntax"></a>C-Syntax

Verbindliche Grundform:

```c
trice("ss_[ext]:<destination>[:<name>]:<normaler Trice-Text>\n", ...);
```

`\n` am Formatstring Ende ist optional und hat nur Bedeutung für die Zeilenkomposition, die Darstellung in tlog. Siehe auch tlog CLI Switch `-addNL`.

Beispiele:

```c
trice("ss_:left:temp:Die Temperatur ist %f °C\n", aFloat(t));
trice("ss_:left:rpm:RPM=%d\n", rpm);
trice("ss_p:left:pressure:PRESSURE=%u\n", pressure);

trice("ss_:right:vel:Speed x=%u, y=%u, z=%u\n", x, y, z);
trice("ss_:right:dir:Richtung dx=%d, dy=%d, dz=%d\n", dx, dy, dz);

trice("ss_loc:right:pose:x=%u, y=%u, z=%u, dx=%d, dy=%d, dz=%d\n",
      x, y, z, dx, dy, dz);
```

Bedeutung:

```txt
ss_[ext]      normaler Trice-Tag, z.B. für Farbe, Filter, -ban
<destination> Serial-Studio-Namespace / Gruppe / logisches Ziel
<name>        optionaler Datensatzname
Text          normaler Trice-Formattext, wird von tlog vollständig angezeigt
```

Wichtig: `ss_`, `ss_a`, `ss_b`, `ss_loc` sind **keine** Serial-Studio-Destinations. Sie bleiben Trice-Tags. Das ist besser als `ss_right:...`, weil sonst jede neue Serial-Studio-Destination zugleich ein neuer Trice-Tag wäre und `tlog`/CLI-Tagsteuerung dafür angepasst werden müsste.

Eine Trice-Message liefert genau einen TSS-Datensatz. Dieser Datensatz darf mehrere Werte enthalten.

## 4. <a id="adressierung"></a>Adressierung

Die C-Syntax adressiert keine konkreten Serial-Studio-Widgets. Sie erzeugt nur stabile Keys.

Beispiel:

```c
trice("ss_:right:vel:x=%u, y=%u, z=%u\n", x, y, z);
```

Keys:

```txt
right.vel.x
right.vel.y
right.vel.z
```

Bei Einzelwerten wird bevorzugt `<destination>.<name>` verwendet:

```c
trice("ss_:left:temp:Die Temperatur ist %f °C\n", aFloat(t));
```

Key:

```txt
left.temp
```

```c
trice("ss_:left:temp:Die Temperatur ist celsius = %f °C\n", aFloat(t));
```

Key:

```txt
left.temp
```

Ansonsten wird der Name aus dem white-space separierten String vor dem % Zeichen gewonnen:

```c
trice("ss_:left:Die Temperatur ist celsius = %f °C\n", aFloat(t));
```

Key:

```txt
left.celsius
```

Bei mehreren Argumenten werden Namen aus `xxx=%...` abgeleitet:

```c
trice("ss_:right:dir:dx=%d, dy=%d, dz=%d\n", dx, dy, dz);
```

Keys:

```txt
right.dir.dx
right.dir.dy
right.dir.dz
```

Fallback:

```txt
<destination>.<name>.arg0
<destination>.<name>.arg1
...
```

Wenn `<name>` fehlt, muss intern ein kollisionsfreier Default verwendet werden, z.B. `default_<triceID>`.

## 5. <a id="reicht-diese-syntax-für-serial-studio"></a>Reicht diese Syntax für Serial Studio?

Ja, für den geplanten TSS-Pfad.

Serial Studio bekommt letztlich Parser-Ausgabekanäle. Datasets lesen diese Kanäle über ihren `index`. Gruppen, Widgets, Farben, Alarmbänder und Layout bleiben in `.ssproj`. Die Trice-Syntax muss daher nur Werte eindeutig benennen; die Zuordnung auf konkrete Datasets passiert in `.ssproj`<!-- und optional in `my.ssproj.trice.json`-->.

Für Single-Source-Projekte reicht:

```txt
ss_[ext]:<group>[:<dataset>]:...
```

Für Multi-Source-Projekte sollte die C-Syntax **nicht** erweitert werden. Multi-Source ist Serial-Studio-Projektkonfiguration.<!-- Mapping auf `sourceId` gehört in `my.ssproj.trice.json`, nicht in den C-Formatstring.-->

## 6. <a id="tlog-und-tss-zur-laufzeit"></a>tlog und TSS zur Laufzeit

`tlog` zeigt immer die komplette formatierte Zeile an:

```txt
ss_:right:vel:Speed x=10, y=20, z=30
```

TSS parst zur Laufzeit **nicht** diesen Text. TSS arbeitet mit:

```txt
Trice-ID
Argumentposition
Argumentwert
Mapping internem TSS-Mapping (bei Programmstart generiert aus aus til.json) 
```

Ablauf:

```txt
Trice-Binärrecord
  -> ID dekodieren
  -> normale tlog-Zeile formatieren und ausgeben falls nicht mit CLI Switch -ban unterdrückt
  -> falls ID zu ss_[ext]-Message gehört:
       arg0..argN auf Keys abbilden
       binären TSS-Frame erzeugen
       an Serial Studio senden
```

`tlog -ban ss_` kann die normale Textanzeige unterdrücken. Die TSS-Ausgabe bleibt davon unabhängig, sofern TSS explizit aktiviert ist.

## 7. <a id="primärformat-messagepack-map"></a>Primärformat: MessagePack Map

Empfohlenes erstes B-B-Format:

```txt
TSS -> MessagePack Map -> Serial Studio Built-In Parser "MessagePack Data"
```

Serial Studio hat einen Built-In-Parser für MessagePack. Im Map-Modus routet er **String-Keys** anhand einer konfigurierten Key-Liste auf Ausgabekanäle. Fehlende Keys werden gelatcht, also behalten ihren letzten Wert. Das passt sehr gut zu sparse Updates.

Referenz:

- Serial Studio Frame Parser Reference: https://serial-studio.com/help/javascript-api
- Serial Studio MessagePack Built-In Template: https://raw.githubusercontent.com/Serial-Studio/Serial-Studio/master/app/rcc/scripts/native/messagepack.md
- MessagePack-Spezifikation: https://github.com/msgpack/msgpack/blob/master/spec.md

Wichtig:

```txt
MessagePack Map nutzt Keys als UTF-8/ASCII-Strings.
Es werden keine numerischen Keys/Indizes im Datenstrom gesendet.
```

Der Index entsteht erst in Serial Studio durch die Key-Liste:

```txt
Keys parameter: left.temp,right.vel.x,right.vel.y,right.vel.z

MessagePack key "left.temp"    -> Kanal 0 -> Dataset index 1
MessagePack key "right.vel.x"  -> Kanal 1 -> Dataset index 2
MessagePack key "right.vel.y"  -> Kanal 2 -> Dataset index 3
MessagePack key "right.vel.z"  -> Kanal 3 -> Dataset index 4
```

Serial-Studio-Dataset-`index` ist 1-basiert. Der Built-In-Parser-Kanal ist konzeptionell 0-basiert; in der `.ssproj` liest Dataset `index = Kanal + 1`.

### 7.1. <a id="messagepack-map-beispiel-uint"></a>MessagePack Map, Beispiel uint

C-Code:

```c
trice("ss_:right:vel:x=%u, y=%u, z=%u\n", x, y, z);
```

Werte:

```txt
x = 10
y = 20
z = 30
```

Logische Map:

```txt
{
  "right.vel.x": 10,
  "right.vel.y": 20,
  "right.vel.z": 30
}
```

Kompakte MessagePack-Codierung auf Byte-Ebene: 

>ACHTUNG: Evtl. nicht korrekt: Ist der Key 0-terminiert oder count-limitiert? Sind die Values 8, 16, 32 oder 64 Bit?


```txt
83                                      fixmap(3)
  AB 72 69 67 68 74 2E 76 65 6C 2E 78  key "right.vel.x"  (fixstr len=11)
  0A                                    positive fixint 10
  AB 72 69 67 68 74 2E 76 65 6C 2E 79  key "right.vel.y"
  14                                    positive fixint 20
  AB 72 69 67 68 74 2E 76 65 6C 2E 7A  key "right.vel.z"
  1E                                    positive fixint 30
```

Als eine Hex-Zeile:

```txt
83 AB 72 69 67 68 74 2E 76 65 6C 2E 78 0A AB 72 69 67 68 74 2E 76 65 6C 2E 79 14 AB 72 69 67 68 74 2E 76 65 6C 2E 7A 1E
```

Das ist ein vollständiger TSS-Frame.

### 7.2. <a id="messagepack-map-beispiel-float"></a>MessagePack Map, Beispiel float

C-Code:

```c
trice("ss_:left:temp:temp=%f\n", aFloat(t));
```

Wert:

```txt
t = 23.5
```

Logische Map:

```txt
{
  "left.temp": 23.5
}
```

MessagePack-Codierung:

```txt
81                                fixmap(1)
  A9 6C 65 66 74 2E 74 65 6D 70   key "left.temp" (fixstr len=9)
  CA 41 BC 00 00                  float32 23.5, big-endian IEEE 754
```

Als Hex-Zeile:

```txt
81 A9 6C 65 66 74 2E 74 65 6D 70 CA 41 BC 00 00
```

### 7.3. <a id="messagepack-map-wertcodierung"></a>MessagePack Map, Wertcodierung

Empfehlung für TSS:

```txt
aFloat()     -> MessagePack float32, Byte 0xCA + IEEE754 big-endian
signed int   -> kleinste passende MessagePack-Integer-Codierung
unsigned int -> kleinste passende MessagePack-Integer-Codierung
```

MessagePack ist selbstbeschreibend. Serial Studio muss also nicht wissen, ob ein Integer als `fixint`, `uint8`, `uint16` oder `uint32` codiert wurde.

Option für Debug/Reproduzierbarkeit:

```txt
--ss-msgpack-fixed-width
```

Dann kann TSS width-preserving senden, z.B. `%u` aus 32-Bit-Trice immer als `uint32`:

```txt
CE 00 00 00 0A   uint32 10
```

Das ist größer, aber byte-stabiler.

## 8. <a id="serial-studio-konfiguration-für-messagepack-map"></a>Serial-Studio-Konfiguration für MessagePack Map

Für TSS-MSGPACK-MAP muss `trice generate` die `.ssproj` so konfigurieren:

```txt
Input:       UDP empfohlen
Decoder:     Binary / Direct / raw bytes
Parser:      Built-In, MessagePack Data
Layout:      Map
Keys:        kommagetrennte Key-Liste in Kanalreihenfolge
```

Beispiel-Key-Liste:

```txt
left.temp,left.rpm,left.speed,right.vel.x,right.vel.y,right.vel.z,right.dir.dx,right.dir.dy,right.dir.dz
```

Bei UDP gilt als einfache Framing-Regel:

```txt
ein UDP-Datagramm = ein MessagePack-Map-Frame
```

Für TCP/Serial ist Framing schwieriger. MessagePack ist zwar selbstbeschreibend, aber Serial Studio muss trotzdem komplette Frames an den Parser übergeben. Für den ersten TSS-Schritt deshalb UDP bevorzugen.

Wichtiger Lizenz-/Build-Hinweis: Die Serial-Studio-Doku listet **Binary Direct** als Pro-Feature. Für Free/GPL-Builds muss praktisch geprüft werden, ob B-B über Binary Direct verfügbar ist. Falls nicht, bleiben als Fallback B-A ASCII oder ein Hex/Base64-Umweg mit Parseranpassung.

Referenz:

- Project Editor, Decoder/Frame Detection: https://serial-studio.com/help/project-editor
- Pro vs Free, Binary Direct: https://serial-studio.com/help/pro-vs-free

## 9. <a id="alternative-binary-tlv"></a>Alternative: Binary TLV

Serial Studio hat einen Built-In-Parser für Binary TLV.

Wire-Format:

```txt
[tag: 1 byte][length: 1 byte][value: N bytes, big-endian] ...
```

Referenz:

- https://raw.githubusercontent.com/Serial-Studio/Serial-Studio/master/app/rcc/scripts/native/binary_tlv.md

Beispiel:

```txt
Tag 4 -> right.vel.x
Tag 5 -> right.vel.y
Tag 6 -> right.vel.z
```

Werte:

```txt
x = 10
y = 20
z = 30
```

TLV-Frame mit 32-Bit-Integern:

```txt
04 04 00 00 00 0A   tag=4, len=4, value=10
05 04 00 00 00 14   tag=5, len=4, value=20
06 04 00 00 00 1E   tag=6, len=4, value=30
```

Als Hex-Zeile:

```txt
04 04 00 00 00 0A 05 04 00 00 00 14 06 04 00 00 00 1E
```

Serial-Studio-Parameter:

```txt
Tag routing table: 4:0,5:1,6:2
```

Dann gilt:

```txt
tag 4 -> Parser-Kanal 0 -> Dataset index 1
tag 5 -> Parser-Kanal 1 -> Dataset index 2
tag 6 -> Parser-Kanal 2 -> Dataset index 3
```

Vorteile:

```txt
klein
sparse Updates
kein String-Key im Datenstrom
kein eigener Parser
```

Nachteile:

```txt
nur 1-Byte-Tags
Werte werden big-endian als Integer akkumuliert
Float32 ist nicht direkt als Float beschrieben
```

Für `aFloat()` ist TLV deshalb nur mit Fixed-Point sauber:

```txt
temp = 23.5 °C
scale = 100
temp_fixed = 2350

01 04 00 00 09 2E   tag=1, len=4, value=2350
```

Dataset-Transform in Serial Studio:

```js
function transform(value) {
  return value / 100.0;
}
```

TLV ist daher eher eine spätere High-Performance-Option für Integer/fixed-point. Für den Start ist MessagePack Map besser.

## 10. <a id="alternative-raw-bytes"></a>Alternative: Raw Bytes

Serial Studio hat einen Raw-Bytes-Parser. Er gruppiert Bytes in gleich breite numerische Kanäle.

Referenz:

- https://raw.githubusercontent.com/Serial-Studio/Serial-Studio/master/app/rcc/scripts/native/raw_bytes.md

Beispiel mit 2 Byte pro Wert, big-endian:

```txt
00 0A 00 14 00 1E
```

Ergebnis:

```txt
Kanal 0 = 10
Kanal 1 = 20
Kanal 2 = 30
```

Nachteile für TSS:

```txt
keine Keys
keine Tags
keine sparse Updates
alle Werte müssen in fester Reihenfolge kommen
Mischtypen und Float sind unpraktisch
```

Raw Bytes ist für TSS nicht der Primärpfad.

## 11. <a id="alternative-ascii-key-value"></a>Alternative: ASCII Key-Value

Nur für Migration/Debug:

```txt
right.vel.x=10,right.vel.y=20,right.vel.z=30
left.temp=23.5
```

Das ist B-A:

```txt
Trice binary -> TSS -> ASCII key=value
```

Vorteil:

```txt
einfach sichtbar
hilfreich bei vorhandenen ASCII-Serial-Studio-Projekten
kein Binary-Direct-Thema
```

Nachteil:

```txt
nicht Zielpfad
mehr Bandbreite
Stringformatierung auf Host-Seite
```

## 12. <a id="alternative-protobuf"></a>Alternative: Protobuf

Serial Studio kann aus `.proto`-Dateien Projekte erzeugen. Der Import erzeugt `.ssproj` plus Lua-Parser.

Referenz:

- Auto-Generating Projects: https://serial-studio.com/help/auto-generating-projects
- Protobuf Example: https://github.com/Serial-Studio/Serial-Studio/tree/master/examples/Protobuf%20Example

Für TSS ist Protobuf interessant, aber nicht erste Wahl:

```txt
+ sauber typisiert
+ etablierter Binärstandard
- .proto müsste aus til.json generiert werden
- Import-/Merge-Workflow komplexer
- bestehende my.ssproj minimal zu patchen wird schwieriger
```

## 13. <a id="trice-generate"></a>trice generate

`trice generate` liest `til.json` und erkennt Formatstrings, deren Tag mit `ss_` beginnt.

Bekannt aus `til.json`:

```txt
Trice-ID -> Formatstring
Trice-Makro / Parameterbreite
```

`trice generate` erzeugt oder aktualisiert:

```txt
my.ssproj              echte Serial-Studio-Projektdatei
my.ssproj.trice.json   Sidecar für Trice/TSS-Metadaten
```

Eine separate Mappingdatei für TSS ist nicht zwingend nötig. TSS kann `til.json` direkt lesen und beim Start intern ein Mapping bauen. Für Debug ist optional möglich:

```txt
--verbose -> trice.ssmap.json ausgeben
```

## 14. <a id="myssproj-bei-neuen-projekten"></a>my.ssproj bei neuen Projekten

Wenn `my.ssproj` fehlt:

```sh
trice generate -til til.json -ssproj my.ssproj -ss-format msgpack-map
```

Dann erzeugt das Tool:

```txt
my.ssproj
my.ssproj.trice.json
```

Schematische `.ssproj`-Inhalte:

```txt
Source:
  Network UDP, z.B. local port 7878
  Decoder: Binary Direct / raw bytes
  Frame detection: No delimiter / Datagram = frame
  Built-In parser: MessagePack Data
  Payload layout: Map
  Keys: left.temp,left.rpm,right.vel.x,...

Groups/Datasets:
  Left
    Temperature   index 1
    RPM           index 2
  Right Velocity
    Vel X         index 3
    Vel Y         index 4
    Vel Z         index 5
```

Die genauen JSON-Feldnamen sind Serial-Studio-versionabhängig. `trice generate` darf daher nicht mit einem fest verdrahteten Einzelformat rechnen.

## 15. <a id="myssproj-bei-bestehenden-projekten"></a>my.ssproj bei bestehenden Projekten

Ziel: vorhandenes Projekt möglichst unverändert weiterverwenden.

Beispiel: existierendes Projekt hat bereits zwei Kurven:

```txt
Group: Curves
  Dataset: X    index 1, Widget/Plot/Farbe bereits eingestellt
  Dataset: Y    index 2, Widget/Plot/Farbe bereits eingestellt
```

KI-Agent schlägt vor:

```c
trice("ss_:curves:xy:x=%f, y=%f\n", aFloat(x), aFloat(y));
```

Nach Einbau im Code und erneutem `trice generate` entstehen Keys:

```txt
curves.xy.x
curves.xy.y
```

`trice generate` patcht dann minimal:

```txt
Dataset X bleibt index 1, Widget/Farbe/Layout bleiben erhalten
Dataset Y bleibt index 2, Widget/Farbe/Layout bleiben erhalten
MessagePack-Key-Liste wird so sortiert:
  Kanal 0 -> curves.xy.x -> Dataset index 1
  Kanal 1 -> curves.xy.y -> Dataset index 2
Parser/Input wird auf TSS MessagePack Map umgestellt
```

Das Projekt sieht für den Anwender fast gleich aus. Nur die Datenquelle/Parser-Konfiguration wechselt von Legacy-Input auf TSS.

## 16. <a id="sidecar-myssprojtricejson"></a>Sidecar my.ssproj.trice.json

FRAGE: Was macht diese Sidecar Datei effektiv? Also ok, die wird geschrieben. Aber wer benutzt sie wie?

Eigene Trice-Metadaten sollten nicht blind in `.ssproj` geschrieben werden. Serial Studio dokumentiert `.ssproj` als JSON-Projektformat, aber nicht als garantiert fremdfeld-stabiles Erweiterungsformat.

Daher:

```txt
my.ssproj             gehört Serial Studio / User
my.ssproj.trice.json  gehört Trice/TSS
```

Beispiel:

```json
{
  "version": 1,
  "project": "my.ssproj",
  "tssFormat": "msgpack-map",
  "backupOfLastPatch": "my.20260709-143512.ssproj.bak",
  "keys": [
    {
      "key": "curves.xy.x",
      "ss": {
        "sourceId": 0,
        "groupTitle": "Curves",
        "datasetTitle": "X",
        "uniqueId": 10000,
        "index": 1
      },
      "trice": {
        "id": 4711,
        "arg": 0,
        "formatPrefix": "ss_:curves:xy:"
      }
    },
    {
      "key": "curves.xy.y",
      "ss": {
        "sourceId": 0,
        "groupTitle": "Curves",
        "datasetTitle": "Y",
        "uniqueId": 10001,
        "index": 2
      },
      "trice": {
        "id": 4711,
        "arg": 1,
        "formatPrefix": "ss_:curves:xy:"
      }
    }
  ],
  "legacy": {
    "previousDecoder": "plain-text-or-project-specific",
    "previousParserSummary": "kept for rollback/documentation"
  }
}
```

`uniqueId` ist in Serial Studio die stabile Dataset-Identität; `index` ist nur die Parser-Kanalposition. Referenz:

- Dataset Identity Model: https://serial-studio.com/help/identity-model

## 17. <a id="backup-regel"></a>Backup-Regel

Direktes Patchen von `my.ssproj` ist erlaubt, aber immer mit Backup.

Empfohlen:

```txt
my.20260709-143512.ssproj.bak
```

Der Zeitstempel ist die aktuelle Zeit des Patch-Vorgangs. Das ist besser als das alte Dateiänderungsdatum, weil der Backup-Name dann den tatsächlichen Eingriff dokumentiert. Das alte `mtime` kann zusätzlich in `my.ssproj.trice.json` protokolliert werden.

## 18. <a id="merge-regeln"></a>Merge-Regeln

Beim Patchen einer vorhandenen `.ssproj`:

```txt
vorher Backup schreiben
.ssproj lesen
Sidecar lesen oder erzeugen
vorhandene Datasets über uniqueId / Sidecar / Key / Titelpfad wiederfinden
Parser/Input auf TSS setzen oder aktualisieren
Keys in MessagePack-Key-Liste eintragen
neue Datasets nur ergänzen
manuelle Einstellungen nicht überschreiben
alte Datasets nicht automatisch löschen
Sidecar aktualisieren
.ssproj schreiben
```

Hoheit:

```txt
Trice/TSS besitzt:
  technische Herkunft: Trice-ID + argIndex
  generierte Keys
  Parser-/Input-Anbindung für TSS
  Sidecar-Mapping

User/Serial Studio besitzt:
  Titel
  Units
  Widget-Typ
  Farben
  Alarm-Bands
  Min/Max
  Layout
  Workspace
  manuelle Transforms
```

Löschregel:

```txt
Wenn ein Wert nicht mehr in til.json vorkommt:
  Dataset nicht automatisch löschen
  in Sidecar als orphan markieren
  nur mit explizitem Cleanup entfernen
```

Beispiel:

```sh
trice generate -ssproj my.ssproj -ss-clean
```

## 19. <a id="ssproj-strukturstabilität"></a>.ssproj-Strukturstabilität

Serial Studio `.ssproj` ist JSON, aber die Struktur ist versionsabhängig.

Beobachtung aus offiziellen Beispielen:

```txt
Ältere Projekte:
  top-level decoder, frameParser, frameStart, frameEnd, groups

Neuere Projekte:
  schemaVersion, sources, uniqueId, nextUniqueId, widgetSettings, sourceId, groupId, datasetId
```

Konsequenz:

```txt
trice generate muss version-/feature-sensitiv patchen.
Nicht einfach komplette .ssproj neu erzeugen.
Nicht auf eine einzige JSON-Struktur verlassen.
Wenn möglich: Serial-Studio-API oder konservative JSON-Patch-Logik nutzen.
```

Serial Studio bietet eine API für Projekt-JSON, Dataset-Updates und Alarm-Bands. Das ist langfristig robuster als blindes JSON-Rewriting.

Referenzen:

- Project Editor: https://serial-studio.com/help/project-editor
- API Reference: https://serial-studio.com/help/api-reference
- Dataset Identity Model: https://serial-studio.com/help/identity-model
- MPU6050 Legacy-Beispiel: https://raw.githubusercontent.com/Serial-Studio/Serial-Studio/master/examples/MPU6050/MPU6050.ssproj
- Protobuf neueres Beispiel: https://raw.githubusercontent.com/Serial-Studio/Serial-Studio/master/examples/Protobuf%20Example/Protobuf%20Example.ssproj

## 20. <a id="ki-agent-für-legacy-migration"></a>KI-Agent für Legacy-Migration

Die Analyse vorhandener `.ssproj` und das Vorschlagen passender Trice-Messages ist besser als KI-Agent-Workflow statt als `trice generate`-Kernfunktion.

Grund:

```txt
KI-Agent:
  gut für semantisches Matching
  gut für Vorschläge
  darf unsicher sein
  läuft selten und unter Userkontrolle

trice generate:
  muss deterministisch sein
  muss testbar sein
  muss CI-tauglich sein
  sollte keine semantischen Ratespiele machen
```

Der Agent soll keine Dateien ändern. Er gibt nur Vorschläge aus.

## 21. <a id="ki-agent-prompt-entwurf"></a>KI-Agent-Prompt Entwurf

```txt
Du bist ein Migrationsagent für Serial-Studio-Projekte nach Trice/TSS.

Input:
- eine vorhandene Serial-Studio-.ssproj-Datei als JSON
- optional README/Sample-Frames/Parser-Code
- optional vorhandene til.json

Ziel:
Schlage Trice-Messages der Form

  trice("ss_[ext]:<destination>[:<name>]:<Formattext>\n", ...);

vor, so dass das bestehende Serial-Studio-Projekt mit minimalen Änderungen
auf TSS MessagePack Map umgestellt werden kann.

Regeln:
- Keine Dateien verändern.
- Erhalte bestehende Gruppen, Dataset-Titel, uniqueId, index, Widgets, Farben,
  Alarm-Bands, Ranges, Layout und Transforms.
- Nutze bestehende Dataset index-Werte als Zielkanal-Reihenfolge.
- Leite destination/name aus Gruppen- und Dataset-Titeln ab.
- Erzeuge einfache C-taugliche Namen: lowercase, ASCII, keine Leerzeichen,
  kein Doppelpunkt.
- Wenn ein bestehender Parser klare Feldnamen enthält, verwende diese.
- Wenn die Bedeutung unklar ist, markiere den Vorschlag als unsicher.
- Ausgabe ausschließlich als Markdown mit:
  - Projektzusammenfassung
  - vorgeschlagenen Trice-Messages
  - erwarteter TSS-Key-Liste in Kanalreihenfolge
  - offenen Unsicherheiten
  - vorgeschlagenem my.ssproj.trice.json-Fragment

Wichtig:
- MessagePack Map nutzt String-Keys im Datenstrom.
- Die Key-Liste in Serial Studio muss dieselbe Reihenfolge haben wie die
  bestehenden Dataset indices.
```

## 22. <a id="mini-demo-nur-skizziert"></a>Mini-Demo, nur skizziert

Ziel: kleines PC-Programm in C/C++, das zwei Kurven erzeugt.

C-Code-Idee:

```c
float x = sinf(phi);
float y = cosf(phi);

trice("ss_:demo:xy:x=%f, y=%f\n", aFloat(x), aFloat(y));
```

Ablauf später:

```txt
demo_pc
  -> sendet normalen Trice-Binärstrom per UDP4 an tlog

tlog + TSS
  -> zeigt normale Trice-Zeilen
  -> erzeugt MessagePack Map Frames
  -> sendet UDP an Serial Studio

Serial Studio
  -> my.ssproj mit MessagePack Map Parser
  -> zwei Plot-Datasets x und y
```

Mögliche Kommandos später:

```sh
trice generate -til til.json -ssproj demo.ssproj -ss-format msgpack-map

tlog -udp4 :12345 -ss udp4://127.0.0.1:7878 -ssproj demo.ssproj
```

Bis TSS existiert, kann das Demo nur den Trice-Teil und die normale `tlog`-Anzeige zeigen. Die TSS-Stufe wird im Dokument nur vorbereitet, nicht implementiert.

## 23. <a id="beispielprojekte-zum-testen-des-ki-agenten"></a>Beispielprojekte zum Testen des KI-Agenten

Offizielle Serial-Studio-Beispiele:

- Beispiele-Übersicht: https://github.com/Serial-Studio/Serial-Studio/tree/master/examples
- MPU6050, klassisches Projekt/CSV-artig: https://github.com/Serial-Studio/Serial-Studio/tree/master/examples/MPU6050
- Protobuf Example, neues Projekt mit generiertem Parser: https://github.com/Serial-Studio/Serial-Studio/tree/master/examples/Protobuf%20Example
- HexadecimalADC, Binär/Hex/CRC-nahes Beispiel: https://github.com/Serial-Studio/Serial-Studio/tree/master/examples/HexadecimalADC
- IMU Simulator, Batched Frames: https://github.com/Serial-Studio/Serial-Studio/tree/master/examples/IMU%20Simulator
- Camera Telemetry, gemischtere Telemetrie: https://github.com/Serial-Studio/Serial-Studio/tree/master/examples/Camera%20Telemetry
- Dual Drone Telemetry, Multi-Source/komplexeres Projekt: https://github.com/Serial-Studio/Serial-Studio/tree/master/examples/Dual%20Drone%20Telemetry

Diese Projekte eignen sich als Testmenge für den Agenten:

```txt
MPU6050           Legacy/CSV, einfache Dataset-Indizes
Protobuf Example  moderne .ssproj-Struktur, uniqueId, sources
HexadecimalADC    Binär-/Hex-orientiert
IMU Simulator     batched/multi-frame Semantik
Dual Drone        Multi-Source/Pro-Komplexität
```

## 24. <a id="vorläufige-festlegung"></a>Vorläufige Festlegung

Primärpfad:

```txt
C:    trice("ss_[ext]:<destination>[:<name>]:...", ...)
Host: TSS liest Trice-ID + argIndex
Wire: MessagePack Map mit String-Keys
SS:   Built-In MessagePack Data, Layout Map, Binary Direct
```

Beispiel-Wire:

```txt
{ "right.vel.x": 10, "right.vel.y": 20, "right.vel.z": 30 }
```

als MessagePack:

```txt
83 AB 72 69 67 68 74 2E 76 65 6C 2E 78 0A AB 72 69 67 68 74 2E 76 65 6C 2E 79 14 AB 72 69 67 68 74 2E 76 65 6C 2E 7A 1E
```

Sekundärpfade:

```txt
Binary TLV       kompakter, aber Float nur über Fixed-Point/Transform sinnvoll
ASCII key=value  Migration/Debug
Protobuf         interessant, aber nicht Minimalpfad
Raw Bytes        nur Spezialfälle
```

Wichtigste offene Prüfungen:

```txt
Binary Direct Verfügbarkeit im Zielbuild testen
exakte .ssproj-Feldnamen für MessagePack Map in aktueller SS-Version prüfen
kleinen C/C++-Trice-UDP-Demo bauen
KI-Agent gegen offizielle Beispielprojekte laufen lassen
```
