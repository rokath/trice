# mdtoc – Spezifikation (v1, überarbeitete Review-Fassung)

> _Anmerkung:_ Diese Fassung restrukturiert das Original für Review und spätere Go-Code-Generierung.  
> Der bisherige Text bleibt abschnittsweise als HTML-Kommentar erhalten.  
> Ziel ist Eindeutigkeit und Verständlichkeit für ein kleines Helper-Tool, nicht ein vollständiger Markdown-AST.


## 1. Zweck und Grundprinzipien

`mdtoc` ist ein deterministisches CLI-Werkzeug zur Verarbeitung einzelner Markdown-Dokumente.

Funktionen:
- Generierung eines Inhaltsverzeichnisses (ToC)
- konsistente Kapitelnummerierung
- Erzeugung stabiler Anchor-IDs, unabhängig von Kapitelnummern
- Entfernen aller von `mdtoc` generierten Artefakte
- Zustandsprüfung eines Dokuments für CI

Grundprinzipien:
- Der sichtbare Überschriftentext ist die einzige semantische Quelle der Wahrheit.
- Kapitelnummern sind abgeleitet und nicht persistent.
- Anchor-IDs werden nur aus dem unnummerierten Titel berechnet.
- Generierte Inhalte sind vollständig rekonstruierbar.
- `mdtoc` ändert ein Dokument nur auf Basis einer klar definierten verwalteten Struktur.
- Das Werkzeug ist idempotent.

_Anmerkung:_ "formal" bedeutet in diesem Dokument nur "klar genug für Parser, Tests und spätere Code-Generierung". Gemeint ist keine große Architektur, sondern ein kleiner, robuster Vertragsrahmen.

## 2. Geltungsbereich und Nicht-Ziele

`mdtoc` verarbeitet absichtlich nur einen kleinen, eindeutigen Markdown-Subset.

Unterstützt in v1:
- einzelne Markdown-Datei
- ATX-Überschriften mit `#` bis `#########`
- definierte ToC-Marker
- definierter Config-Block
- definierte Inline-Ankerform in Überschriften

Nicht unterstützt in v1:
- Setext-Überschriften
- GUI-Automation
- PDF-Erzeugung
- Mehrdateiverarbeitung
- vollständiger Markdown-AST
- partielle Verarbeitung wie `--toc-only` oder `--anchors-only`

_Anmerkung:_ Die Beschränkung auf einen kleinen Markdown-Subset ist Absicht. Damit bleiben Parser, Testfälle und Fehlersuche einfach.

## 3. Explizite Dokumentstruktur

Ein von `mdtoc` verwaltetes Dokument verwendet genau diese Container-Struktur:

```md
<!-- mdtoc -->
[TOC CONTENT]
<!-- mdtoc-config
numbering=on
min-level=2
max-level=4
anchors=on
state=generated
-->
<!-- /mdtoc -->
```

Regeln:
- Der äußere Container besteht aus Start-Marker, ToC-Bereich, Config-Block und End-Marker.
- Der Config-Block muss unmittelbar vor `<!-- /mdtoc -->` stehen.
- `<!-- mdtoc -->` darf höchstens einmal vorkommen.
- `<!-- /mdtoc -->` darf höchstens einmal vorkommen.
- Der Config-Block darf höchstens einmal vorkommen.
- Kommt keiner der äußeren Marker vor, fügt `generate` den kompletten Container am Dateianfang ein.
- Kommt nur einer der äußeren Marker vor oder liegt der Start-Marker nach dem End-Marker, ist das ein Parsing-Fehler.
- Alles zwischen `<!-- mdtoc -->` und dem Anfang des Config-Blocks ist der verwaltete ToC-Bereich.
- Fremder Inhalt im ToC-Bereich wird bei `generate` nicht gelöscht, sondern als HTML-Kommentar erhalten.

Hinweis: Der User kann durch Verschieben des Toc-Bereiches bestimmen, wo das Inhaltsverzeichnis sein soll.

_Anmerkung:_ Die explizite Container-Struktur ist absichtlich einfacher lesbar als eine implizite Marker-Logik. So ist sofort sichtbar, welchen Bereich `mdtoc` verwaltet.

## 4. Parsing-Regeln

### 4.1 Grundsatz

Der Parser arbeitet zeilenbasiert.  
Er erkennt nur die in dieser Spezifikation genannten Strukturen und ignoriert bewusst andere Markdown-Sonderfälle.

### 4.2 Ignored Regions

Diese Bereiche werden beim Erkennen von Markern und Überschriften ignoriert:

1. Fenced code blocks mit Backticks:
   - Beginn: eine Zeile, die mit drei Backticks beginnt
   - Ende: die nächste Zeile, die mit drei Backticks beginnt
2. Fenced code blocks mit Tilde:
   - Beginn: eine Zeile, die mit drei Tilden (`~~~`) beginnt
   - Ende: die nächste Zeile, die mit drei Tilden (`~~~`) beginnt
3. Inline code spans:
   - Bereich zwischen zwei Backticks in derselben Zeile
4. HTML-Kommentare:
   - `<!-- ... -->`
   - Ausnahme: `<!-- mdtoc -->`, `<!-- /mdtoc -->` und `<!-- mdtoc-config ... -->`

Nicht ignoriert:
5. Blockquotes

Blockquotes sind normale Eingabezeilen.  
Sie werden nicht als Sonderbereich behandelt.

Praktische Folge:
- Eine Blockquote-Zeile beginnt mit optionalen Spaces und dann `>`.
- Eine von `mdtoc` erkannte Überschrift muss mit einem `hashes`-Präfix direkt an Spalte 1 beginnen.
- Dadurch können Blockquotes nicht auf die Heading-Syntax matchen und brauchen keine Sonderbehandlung.

### 4.3 Parsing-Reihenfolge

Die Verarbeitung läuft in dieser Reihenfolge:

1. Äußeren `mdtoc`-Container und Config-Block erkennen.
2. Ignored Regions berücksichtigen.
3. Überschriften erkennen.
4. Verwaltete Artefakte semantisch normalisieren.
5. Sollzustand ableiten.
6. Ausgabe rendern.

## 5. Heading-Syntax

### 5.1 Kandidaten für Überschriften

Nur Zeilen, die direkt am Zeilenanfang mit einem der folgenden Präfixe beginnen, sind für `mdtoc` überhaupt Überschriften:

```text
hashes := "# " | "## " | "### " | "#### " | "##### " | "###### "| "####### "| "######## "| "######### "
```

Damit gilt zugleich:
- nach den `#` muss genau ein Leerzeichen folgen
- vor den `#` dürfen keine Leerzeichen stehen

_Anmerkung:_ Das Leerzeichen ist hier bewusst Teil von `hashes`.  Das vereinfacht den Parser: Nach dem Präfix kommt entweder direkt die Nummer, direkt der Anchor oder direkt der Titel.

### 5.2 Struktur einer verwalteten Überschrift

Verwaltete Überschriften verwenden genau dieses Schema:

```text
heading_line := hashes [number SP] [anchor] title
number       := DIGIT+ ("." DIGIT+)* "."
anchor       := "<a id=\"anchor_id\"></a>"
title        := NONEMPTY_TEXT
SP           := exactly one U+0020 space
```

Zusätzliche Regeln:
- `number` ist optional.
- Wenn `number` vorkommt, steht sie direkt nach `hashes` und wird von genau einem Leerzeichen gefolgt.
- `anchor` ist optional.
- Wenn `anchor` vorkommt, steht er direkt nach `hashes` oder direkt nach `number SP`.
- Zwischen `</a>` und dem ersten Zeichen des Titels steht **kein** Leerzeichen.
- Diese fehlende Leerstelle ist absichtlich so festgelegt, um mit `dumeng-toc` kompatibel zu bleiben.
- Innerhalb des Titels bleiben Leerzeichen und Zeichen unverändert erhalten.
- Nur Überschriften, die exakt dieser Positionslogik entsprechen, dürfen von `mdtoc` umgeschrieben werden.

Beispiele gültiger verwalteter Überschriften:

```md
# Titel
## 1. Einführung
## <a id="einfuehrung"></a>Einführung
### 2.1. <a id="api-overview"></a>API Overview
```

Beispiele, die `mdtoc` nicht als verwaltete Struktur behandelt:

```md
 # Titel
##  1. Einführung
### 1.2 Einführung
### <a id="x"></a> Einführung
```

### 5.3 Bedeutung der Syntax

- `### 2024 roadmap` ist **keine** Nummer, weil das erste Token nicht auf `.` endet.
- `### 3D graphics` ist **keine** Nummer, weil das erste Token kein reines `x.y.z.`-Muster ist.
- `### 2.1. API` ist eine verwaltete Nummernsyntax.

_Anmerkung:_ Das Muster `### 2.1. API` ist damit bewusst für `mdtoc` reserviert. Wer eine freie Überschrift exakt in diesem Format schreibt, verwendet dieselbe Syntax wie das Tool.

### 5.4 Unterstützter Markdown-Subset

`mdtoc` ist kein allgemeiner Markdown-Parser.

Für Überschriften gilt in v1:
- nur ATX-Überschriften
- nur die oben definierte Heading-Syntax
- keine Setext-Überschriften
- keine impliziten oder mehrdeutigen Sonderfälle

Der praktische Vorfilter lautet damit mindestens:

```text
^#{1,9} 
```

Und die eigentliche Umschreibelogik greift nur auf Zeilen, die auch die restliche Positionslogik erfüllen.

## 6. Kleines formales Modell

Dieser Abschnitt beschreibt die minimale interne Sicht, die für saubere Implementierung und Tests hilfreich ist.

### 6.1 Verwaltete Überschrift

Intern reicht für eine verwaltete Überschrift dieses Modell:

```text
ManagedHeading
- line_index
- level
- title
- number        // abgeleitet oder leer
- anchor_id     // abgeleitet oder leer
```

Semantisch wichtig sind nur:
- `level`
- `title`

Abgeleitet werden daraus:
- `number`
- `anchor_id`

### 6.2 Dokumentzustand

Ein Dokument befindet sich für `mdtoc` praktisch in einem dieser Zustände:

- `unmanaged`  
  Kein gültiger `mdtoc`-Container mit gültigem Config-Block vorhanden.

- `generated`  
  Das Dokument entspricht dem aus Inhalt + Config abgeleiteten Sollzustand.

- `stripped`  
  Die verwalteten Artefakte sind entfernt, der Container mit Config bleibt erhalten.

### 6.3 Verarbeitungspipeline

Die Verarbeitung folgt immer demselben einfachen Muster:

```text
parse -> normalize -> derive -> render
```

Das bedeutet:
- **parse**: Container, Config und Überschriften erkennen
- **normalize**: verwaltete Nummern und verwaltete Anchors semantisch entfernen
- **derive**: Nummern, Anchor-IDs und ToC neu berechnen
- **render**: Dokument deterministisch zurückschreiben

_Anmerkung:_ Das soll keine große AST-Architektur erzwingen. Es legt nur fest, welche Informationen semantisch zählen und welche nur Render-Artefakte sind.

### 6.4 Gültigkeitsbereich von `min-level` und `max-level`

Diese Fassung geht von folgender, leicht verständlicher Regel aus:

- `min-level` und `max-level` filtern dieselbe Menge an Überschriften für
  - ToC-Erzeugung
  - Nummerierung
  - Anchor-Erzeugung

Praktische Folge:
- Bei `generate` werden zunächst alle verwalteten Nummern und verwalteten Anchors aus allen verwalteten Überschriften entfernt.
- Danach werden Nummern und Anchors nur für Überschriften innerhalb des aktiven Level-Bereichs neu gesetzt.
- Überschriften außerhalb des Bereichs bleiben inhaltlich erhalten, werden aber nicht neu verwaltet.

## 7. Config-Block

Der Config-Block hat genau diese Form:

```html
<!-- mdtoc-config
numbering=on
min-level=2
max-level=4
anchors=on
state=generated
-->
```

Regeln:
- Der Config-Block ist zeilenbasiert.
- Jedes Feld steht in einer eigenen Zeile.
- Alle Felder verwenden `key=value`.
- Die Feldreihenfolge ist fest:
  1. `numbering`
  2. `min-level`
  3. `max-level`
  4. `anchors`
  5. `state`
- Zulässige Werte:
  - `numbering=on|off`
  - `anchors=on|off`
  - `state=generated|stripped`
- `min-level` und `max-level` sind positive ganze Zahlen.
- `min-level` darf nicht größer als `max-level` sein.
- `max-level` darf nicht größer als 9 sein.
- `generate` schreibt alle Generator-Optionen in den Config-Block; nicht angegebene Optionen mit Default-Wert.
- `--file`, `--help`, `--version`, `--verbose` und `--raw` werden nicht persistiert.
- `strip` behält den Config-Block und setzt nur `state=stripped`.
- `strip --raw` entfernt den Config-Block vollständig.

_Anmerkung:_ `state` ist hier bewusst ebenfalls auf `key=value` vereinheitlicht. Das macht den Parser trivialer und vermeidet einen unnötigen Sonderfall.

## 8. CLI-Schnittstelle

### 8.1 Kommandos

```text
mdtoc --version
mdtoc --version --verbose
mdtoc --help
mdtoc --help --verbose

mdtoc generate [--verbose] [OPTIONEN]
mdtoc generate --help

mdtoc strip [--verbose] [--raw]
mdtoc strip --help

mdtoc check [--verbose]
mdtoc check --help
```

### 8.2 Optionen für `generate`

| Option                  | Default | Bedeutung                                   |
|-------------------------|---------|---------------------------------------------|
| `--numbering <on\|off>` | `on`    | Kapitelnummern aktivieren oder deaktivieren |
| `--min-level <N>`       | `2`     | minimale verwaltete Heading-Ebene (>=1)     |
| `--max-level <N>`       | `4`     | maximale verwaltete Heading-Ebene (<=9)     |
| `--anchors <on\|off>`   | `on`    | Inline-Anker erzeugen oder deaktivieren     |
| `--file <name>`         | –       | Datei lesen und überschreiben               |
| `--verbose`             | `off`   | Diagnose- und Ablaufmeldungen auf `stderr`  |
| `--help`                | –       | Hilfe anzeigen                              |

Kurzformen:
- `--numbering` -> `-n`
- `--anchors` -> `-a`
- `--file` -> `-f`
- `--verbose` -> `-v`
- `--help` -> `-h`

### 8.3 I/O- und Logging-Verhalten

- Mit `--file` wird die Datei gelesen und überschrieben.
- Ohne `--file` kommt die Eingabe von `stdin` und die Dokumentausgabe geht auf `stdout`.
- Erfolgreiche Kommandos erzeugen keine Ausgabe, außer bei `--help`, `--version` oder `--verbose`.
- Fehler und Diagnosemeldungen gehen ausschließlich auf `stderr`.

<!--
ALT AUS ORIGINAL: Abschnitt 2. CLI Kommandos

## 2. CLI Kommandos

| Option                                  | Beschreibung                                     |
|&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;-|&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;|
| `mdtoc &#45;&#45;version`                       | Gibt kurze Versionsinfo aus.                     |
| `mdtoc &#45;&#45;version &#45;&#45;verbose`             | Gibt ausführliche Versionsinfo aus.              |
| `mdtoc &#45;&#45;help`                          | Gibt kurzen Help Text aus aus.                   |
| `mdtoc &#45;&#45;help &#45;&#45;verbose`                | Gibt langen Help Text aus aus.                   |
| `mdtoc generate  &#45;&#45;help`                | Gibt langen Help Text speziell für generate aus. |
| `mdtoc strip     &#45;&#45;help`                | Gibt langen Help Text speziell für strip aus.    |
| `mdtoc check     &#45;&#45;help`                | Gibt langen Help Text speziell für check aus.    |
| `mdtoc generate [&#45;&#45;verbose] [OPTIONEN]` | generiert/updated ToC, numbers, anchors.         |
| `mdtoc strip    [&#45;&#45;verbose] [&#45;&#45;raw]`    | entfernt ToC, numbers, anchors und ggf. Config.  |
| `mdtoc check    [&#45;&#45;verbose]`            | prüft Config und ggf. ToC, numbers, anchors.     |
-->

<!--
ALT AUS ORIGINAL: Abschnitt 3. Optionen (generate)

## 3. Optionen (generate)

### Generator-Steuerung

| Option                  | Default | Beschreibung              |
|&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;-|&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;-|&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;-|
| `&#45;&#45;numbering &lt;on\|off&gt;` | on      | Kapitelnummern aktivieren |
| `&#45;&#45;min-level &lt;N&gt;`       | 2       | Minimale Heading-Ebene    |
| `&#45;&#45;max-level &lt;N&gt;`       | 4       | Maximale Heading-Ebene    |
| `&#45;&#45;anchors &lt;on\|off&gt;`   | on      | Anchor-Tags erzeugen      |

### Input &amp; Output Steuerung

| Option          | Beschreibung                             |
|&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;-|&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;|
| `&#45;&#45;file &lt;name&gt;` | Datei wird gelesen und überschrieben.     |
| (kein `&#45;&#45;file`) | Eingabe von stdin, Ausgabe auf stdout.    |
| `&#45;&#45;version`     | Gibt Version-Info aus. |
| `&#45;&#45;help`     | Gibt Hilfe-Info aus.|
| `&#45;&#45;verbose`     | Gibt Log-Info aus, nutzbar für Debugging, bzw. liefert ausführlichere Informationen. |

- Ohne `&#45;&#45;verbose` bei Kommandos grundsätzlich keine Ausgabe, außer bei Fehlern, die dann über den Fehlerkanal stderr kommen.
- `&#45;&#45;help` und `&#45;&#45;version` geben auch ohne `&#45;&#45;verbose` Infos aus.

### Kurzformen

| Option        | Kurzform |
|&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;-|&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;&#45;|
| `&#45;&#45;numbering` | `-n`     |
| `&#45;&#45;anchors`   | `-a`     |
| `&#45;&#45;file`      | `-f`     |
| `&#45;&#45;verbose`   | `-v`     |
| `&#45;&#45;help`      | `-h`     |
-->

## 9. Kommandos

### 9.1 `generate`

Verhalten:
1. Dokument parsen.
2. Wenn kein verwalteter Container vorhanden ist, kompletten Container am Dateianfang anlegen.
3. Wenn Marker-Struktur oder Config ungültig ist: Fehler und keine Änderung.
4. Vorhandene verwaltete Artefakte semantisch entfernen:
   - ToC-Inhalt
   - verwaltete Kapitelnummern
   - verwaltete Inline-Anker
5. Relevante Überschriften bestimmen.
6. Nummern neu berechnen, falls `numbering=on`.
7. Anchor-IDs neu berechnen, falls `anchors=on`.
8. ToC neu rendern.
9. Überschriften neu rendern.
10. Config neu rendern und `state=generated` setzen.
11. Dokument zurückschreiben.

Zusätzliche Regeln:
- Nummerierung und Anchor-ID sind strikt entkoppelt.
- Anchor-IDs werden nur aus dem unnummerierten Titel berechnet.
- Duplicate IDs werden deterministisch aufgelöst.
- Fremder Inhalt im ToC-Bereich wird nicht gelöscht, sondern als HTML-Kommentar erhalten.
- Bei Erfolg ist das Ergebnis idempotent.

Beispiel für eine gerenderte Überschrift:

```md
### 4.1. <a id="open-source"></a>Open  source
```

<!--
ALT AUS ORIGINAL: Aus Abschnitt 6. Kommandos / generate

### generate

- Wenn ungültige Config, Fehler und keine Aktion
- Wenn keine Config, Config anlegen
- Wenn gültige Config, Update Config und  Aktion

- Aktion
  - entfernt generierte Artefakte
  - erzeugt Nummern wenn enabled
  - erzeugt Anchors wenn enabled
  - erzeugt ToC in dieser Form:
  
  ```md
  &lt;!&#45;&#45; mdtoc &#45;&#45;&gt;
  * [1. Abstract](#abstract)
  ...
  &lt;!&#45;&#45; /mdtoc &#45;&#45;&gt;
  ```

  - setzt state: generated

- Hinweise:
  - Config auswerten/updaten
  - vorhandene Altlasten bereinigen
  - Nummerierung konsistent berechnen
  - stabile Anchor-IDs erzeugen
  - TOC aus Überschriften ableiten

#### Detaillierte generate Hinweise

- Dokument parsen
- Bestehenden generierten Zustand entfernen
- TOC-Block ersetzen
- generierte Heading-Nummern entfernen
- generierte `&lt;a id=...&gt;`-Anker entfernen
- Strukturmodell aufbauen
- alle relevanten Überschriften extrahieren
- Ebenen erkennen (#, ##, ###, ...)
- Nummern berechnen
- stabile IDs berechnen
- Neuen TOC rendern
- Überschriften neu rendern
- Nummern wieder einsetzen
- Anker wieder einsetzen
- Dokument zurückschreiben

Das ist wichtig, weil du dann intern mit einer kanonischen Repräsentation arbeitest.
Also nicht mehrfach „Text manipulieren“, sondern:

parse → normalize → model → render

Nummerierung und ID strikt entkoppeln.

Beispiel:

```md
### 4.1. &lt;a id=&quot;open-source&quot;&gt;&lt;/a&gt;Open source
````

Dabei gilt intern:

- sichtbarer Titel: Open source
- Nummer: 4.1.
- ID: open-source

Und die ID wird nur aus dem unnummerierten Titel berechnet, nie aus der Kapitelnummer.

Worauf du achten solltest

1) Nur generierte Anker entfernen

Beliebige handgeschriebene `&lt;a id=&quot;...&quot;&gt;` im Dokument nicht blind alle entfernen.

Besser:

Nur Anker entfernen, die direkt am Beginn einer Überschrift stehen und dem generierten Muster entsprechen oder optional einen Marker verwenden.

Beispiel:

`&lt;a id=&quot;open-source&quot;&gt;&lt;/a&gt;` nur entfernen, wenn es direkt nach der Heading-Nummer und direkt vor dem Heading-Text sitzt

2) Nummern sauber strippen

Du musst unterscheiden zwischen:

echter generierter Kapitelnummer: 4.1.
legitimen Titeln, die mit Zahlen anfangen könnten

Beispiel problematisch:

```md
### 3D graphics
### 2024 roadmap
````

Deshalb sollte das Strippen nicht zu aggressiv sein.
Wahrscheinlich nur Muster wie:

```go
^\d+\.\s+
^\d+(?:\.\d+)*\.\s+
````

also mit abschließendem Punkt.

3) Duplicate IDs

Wenn zwei Kapitel denselben Titel haben, brauchst du ein deterministisches Suffixing:

overview
overview-2
overview-3

Sonst brechen Links trotz guter Logik.

4) Codeblöcke und Sonderbereiche ignorieren

ATX-Headings in fenced code blocks dürfen nicht als echte Überschriften erkannt werden.

Also:

fenced code blocks ignorieren
eventuell HTML comments/marker sauber behandeln

Ein guter zeilenbasierter Parser wenn reichen, wenn sauber einschränkt auf:

ATX-Headings (#, ##, ...)
definierte TOC-Marker
definierte Anchor-Form
keine Setext-Headings nötig, falls ihr die nicht nutzt

5) Logging

- Success: keine Ausgabe (außer bei `&#45;&#45;verbose`)
- Fehler: Meldung auf stderr
  - Beispiel: `mdtoc: no mdtoc-config block found; file is not managed by mdtoc`
- `&#45;&#45;verbose`: Diagnose-/Ablaufmeldungen auf stderr
  - Wichtig ist dabei die strikte Trennung:
    - Dokumentausgabe nur ins File oder auf stdout
    - Status/Diagnostik nur auf stderr

6) Anchors 

- Entferne Kapitelnummer
- lowercase
- Leerzeichen → -
- entferne Sonderzeichen
- kollisionsfrei machen:
  - overview
  - overview-2
  - overview-3
- Anchor-Erzeugung Beispiel:

```md
### 4.1. Open source
```

→

```md
### 4.1. &lt;a id=&quot;open-source&quot;&gt;&lt;/a&gt;Open source
```

7) TOC-Verhalten

- basiert auf min-level und max-level
- Hierarchische Struktur entsprechend Heading-Level
- Nummerierung entspricht Kapitelnummern
- Links verwenden generierte Anchor-IDs

8) Config-Block

- Regeln:
  - Wird von generate erzeugt oder aktualisiert
  - Wird von strip beibehalten
  - Wird von check ausgewertet
  - `&#45;&#45;file` wird nicht gespeichert
  - Zeilenbasiertes Key-Value-Format
- Felder:
  - Feld	Bedeutung
  - numbering	on / off
  - min-level	Zahl
  - max-level	Zahl
  - anchors	on / off
  - state	generated / stripped

9) Streams

- Dokumentinhalt → file oder stdout
- Fehler und Logs → stderr

10) Verhalten

- Ablauf
  - Entfernt alle generierten Artefakte:
  - bestehenden ToC
  - Kapitelnummern
  - generierte Anchor-Tags
  - Parst alle Überschriften
  - Berechnet:
  - Kapitelnummern
  - Anchor-IDs
  - ToC-Struktur
- Rendert:
  - ToC
  - Überschriften
  - Config-Block
- Setzt:
  - state: generated
  - Eigenschaften
  - deterministisch
- idempotent
- Fehlerfall
  - Ungültiger vorhandener Config-Block → Fehler, keine Änderung

11) Idempotenz

```bash
mdtoc generate
mdtoc generate
```

→ keine Änderung beim zweiten Lauf

```bash
mdtoc strip
mdtoc strip
```

→ keine Änderung beim zweiten Lauf

12) TOC-Verhalten

- basiert auf min-level und max-level
- Hierarchische Struktur entsprechend Heading-Level
- Nummerierung entspricht Kapitelstruktur
- Links referenzieren Anchor-IDs

13) Fehlerverhalten

- Fehlerbeispiele:
  - fehlender Config-Block (strip)
  - fehlender Config-Block (check)
  - ungültiger Config-Block	
  - Parserfehler
  - ungültige Optionen
- Alle Fehler:
  - Ausgabe auf stderr
  - Exit-Code ≠ 0

14) Nicht-Ziele (v1)

- keine partielle Verarbeitung (&#45;&#45;*-only)
- keine GUI-Automation
- keine PDF-Erzeugung
- keine Mehrdateiverarbeitung
- kein vollständiger Markdown-AST

15) Erweiterbarkeit

- Mögliche Erweiterungen:
  - Anchor-Styles
  - alternative TOC-Formate
  - Versionierung im Config-Block
  - weitere Ausgabeformate
-->

### 9.2 `strip`

Verhalten:
- benötigt einen gültigen Config-Block
- entfernt verwalteten ToC-Inhalt
- entfernt verwaltete Kapitelnummern
- entfernt verwaltete Inline-Anker
- behält äußeren Container
- behält Config-Block
- setzt `state=stripped`

Nach `strip` ist damit diese Struktur weiterhin gültig:

```md
<!-- mdtoc -->
<!-- mdtoc-config
numbering=on
min-level=2
max-level=4
anchors=on
state=stripped
-->
<!-- /mdtoc -->
```

Fehlerfall:
- kein gültiger Config-Block -> Fehler
- keine implizite Reparatur

<!--
ALT AUS ORIGINAL: Aus Abschnitt 6. Kommandos / strip

### strip

- Ablauf:
  - Prüft auf gültigen Config-Block
    - → wenn nicht vorhanden oder ungültig: Fehler
- Entfernt:
  - ToC
  - Kapitelnummern
  - generierte Anchor-Tags
- Belässt:
  - reinen Markdown-Inhalt
  - Config-Block
- Setzt:
  - state: stripped
- Fehlerfall
  - mdtoc: no mdtoc-config block found; file is not managed by mdtoc
  - Exit-Code ≠ 0
-->

### 9.3 `strip --raw`

Verhalten:
- ignoriert den Config-Block
- entfernt den kompletten verwalteten Container, falls vorhanden:
  - `<!-- mdtoc -->`
  - ToC-Inhalt
  - `mdtoc-config`
  - `<!-- /mdtoc -->`
- entfernt zusätzlich verwaltete Kapitelnummern
- entfernt zusätzlich verwaltete Inline-Anker

Konservative Regel:
- Wenn nicht sicher entschieden werden kann, ob eine Nummer oder ein Inline-Anker verwaltet ist, bleibt der Inhalt stehen.
- Dieser Fall soll in jedem Fall als Diagnose ausgegeben werden.

Einsatzfälle:
- beschädigte Config
- Migration
- vollständige Entfernung der `mdtoc`-Verwaltung
- Tests

<!--
ALT AUS ORIGINAL: Aus Abschnitt 6. Kommandos / strip --raw

### strip -raw

1) Details

- Zweck:
  - Erzwingt eine vollständige Bereinigung unabhängig vom Config-Block.
- Verhalten:
  - Ignoriert Config-Parsing
- Entfernt:
  - ToC (nur innerhalb bekannter Marker)
  - generierte Kapitelnummern
  - generierte Anchor-Tags
  - kompletten Config-Block
  - Kein neuer Config-Block wird erzeugt
- Einsatzfälle:
  - beschädigte Config
  - Migration
  - Entfernung von mdtoc-Verwaltung
  - Testing
-->

### 9.4 `check`

Verhalten:
- benötigt einen gültigen Config-Block
- rekonstruiert den Sollzustand aus aktuellem Dokumentinhalt und Config
- vergleicht Sollzustand und Istzustand byte-genau
- liefert `0`, wenn beide identisch sind
- liefert bei Abweichung einen Fehler-Exit-Code

Keine Seiteneffekte:
- `check` verändert das Dokument nie

_Anmerkung:_ "byte-genau" klingt formaler als es praktisch ist. Gemeint ist: `check` berechnet denselben Text, den `generate` oder `strip` schreiben würden, und vergleicht genau diesen.

<!--
ALT AUS ORIGINAL: Aus Abschnitt 6. Kommandos / check

### check

- Ablauf:
  - Prüft auf Config-Block
  - → wenn nicht vorhanden: FAIL
  - Prüft Config-Syntax
  - → bei Fehler: FAIL
- Liest:
  - Konfiguration
  - erwarteten Zustand (state)
  - Rekonstruiert erwarteten Zustand
- Vergleicht:
  - erwarteter Zustand vs. tatsächlicher Inhalt
  - Ergebnis
  - Zustand	Exit-Code
  - identisch	0
  - Abweichung	≠ 0
  - Eigenschaften
  - keine Ausgabe bei Erfolg
  - Fehlerausgabe auf stderr
- mit `&#45;&#45;verbose`:
  - Diagnoseinformationen
-->

## 10. ToC-Regeln

Der ToC basiert auf allen verwalteten Überschriften innerhalb von `min-level` bis `max-level` inklusive.

Render-Regeln:
- Eine Überschrift erzeugt genau einen ToC-Eintrag.
- Die Hierarchie folgt dem Heading-Level.
- Pro zusätzlicher Ebene relativ zu `min-level` wird um zwei Spaces eingerückt.
- Jeder Eintrag ist ein Markdown-Listenpunkt mit Link.

Beispiel:

```md
* [1. Einleitung](#einleitung)
  * [1.1. API](#api)
```

Anzeige im Linktext:
- bei `numbering=on`: `nummer + titel`
- bei `numbering=off`: nur `titel`

Linkziel:
- grundsätzlich die von `mdtoc` berechnete Anchor-ID

> Offene Frage: Was genau bedeutet `anchors=off` für die ToC-Links?  
> Wenn `anchors=off` keine Inline-Anker schreibt, hängt die Zielauflösung sonst vom Markdown-Renderer ab.  
> Für wirklich stabile Links wäre entweder `anchors=on` verpflichtend oder das Linkverhalten müsste explizit anders definiert werden.

## 11. Anchor-Generierung (normativ)

Anchor-IDs werden deterministisch aus dem **unnummerierten Überschriften-Text (title)** erzeugt.

### 11.1 Ziel

Kompatibilität mit:
- dumeng-toc
- vscode-markdown-toc

---

### 11.2 Algorithmus

Für jede Überschrift gilt:

anchor_id := normalize(title)

---

### 11.3 Normalisierung (normativ)

Die Funktion `normalize` MUST wie folgt implementiert werden:

1. Eingabe ist der unveränderte Überschriften-Text (`title`)
2. Konvertiere den gesamten Text zu lowercase
3. Ersetze jedes ASCII-Leerzeichen (U+0020) durch `-`
4. Führe **keine weitere Transformation durch**

---

### 11.4 Explizit NICHT erlaubt

Die Implementierung MUST NOT:

- Nicht-ASCII-Zeichen entfernen (z. B. ä, ö, ü, ß, é, 中文, 🚀)
- Zeichen transliterieren (z. B. ä → ae)
- Sonderzeichen entfernen (z. B. &, (, ), +, *, !)
- Mehrfache `-` zusammenfassen
- führende oder folgende `-` entfernen

---

### 11.5 Kollisionen

Kollisionen werden deterministisch aufgelöst:

- erster Treffer: 그대로
- zweiter Treffer: `-2`
- dritter Treffer: `-3`

Beispiel:

- overview
- overview-2
- overview-3

---

### 11.6 Beispiel

```md
### 4.1. Open source
```

wird zu:

```md
### 4.1. <a id="open-source"></a>Open source
```

---

### 11.7 Bekannte Einschränkungen

Die erzeugten Anchor-IDs sind möglicherweise:

- nicht URL-sicher
- nicht in allen Markdown-Renderern funktionsfähig

Dieses Verhalten ist **by design**.

---

### 11.8 Zukünftige Erweiterung (optional)

Unterstützung alternativer Anchor-Strategien über CLI-Optionen.

Beispiel:

mdtoc generate --anchor-style=slug

Ein „Slug“ ist eine URL-sichere, normalisierte Zeichenkette:

- nur ASCII (a-z, 0-9)
- Leerzeichen → -
- Sonderzeichen entfernt
- optionale Normalisierung von Umlauten
- Reduktion mehrfacher '-'

Für Version 1 gilt:

Default anchor-style = dumeng

## 12. Fehlerverhalten, Logging und Exit-Codes

Fehlerfälle:
- fehlender oder unvollständiger `mdtoc`-Container
- fehlender Config-Block bei `strip` oder `check`
- ungültiger Config-Block
- Parsing-Fehler
- ungültige Optionen

Grundregeln:
- Fehler werden auf `stderr` ausgegeben.
- Bei Fehlern gibt es keine implizite Reparatur, außer dem explizit erlaubten Anlegen eines neuen Containers durch `generate`, wenn noch gar keine `mdtoc`-Verwaltung existiert.
- Erfolgreiche Kommandos schreiben keine Statusmeldungen auf `stdout`.

Empfohlene Exit-Codes:
- `0` -> Erfolg
- `1` -> Parsing-, Config- oder CLI-Fehler
- `2` -> `check` hat eine Abweichung gefunden

<!--
ALT AUS ORIGINAL: Abschnitt 8. Fehlerverhalten

## 8. Fehlerverhalten

- keine Config → Fehler (außer bei `mdtoc strip &#45;&#45;raw`)
- ungültige Config → Fehler
- keine implizite Reparatur
-->

## 13. Idempotenz

Idempotenz ist Teil des Vertrags.

Beispiele:

```bash
mdtoc generate
mdtoc generate
```

=> keine weitere Änderung beim zweiten Lauf

```bash
mdtoc strip
mdtoc strip
```

=> keine weitere Änderung beim zweiten Lauf

```bash
mdtoc strip --raw
mdtoc strip --raw
```

=> keine weitere Änderung beim zweiten Lauf

## 14. Erweiterbarkeit

Mögliche spätere Erweiterungen:
- alternative Anchor-Styles
- alternative ToC-Formate
- Versionierung im Config-Block
- weitere Ausgabeformate

_Anmerkung:_ Diese Punkte sind ausdrücklich Erweiterungen. Sie sollen v1 nicht unnötig komplex machen.
