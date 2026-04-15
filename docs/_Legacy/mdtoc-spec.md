
# mdtoc – Spezifikation (v1)

## 1. Zweck

mdtoc ist ein deterministisches CLI-Werkzeug zur Verarbeitung von Markdown-Dokumenten.

Funktionen:
- Generierung eines Inhaltsverzeichnisses (ToC)
- Konsistente Kapitelnummerierung
- Erzeugung stabiler Anchor-IDs (unabhängig von Kapitelnummern)
- Entfernen aller generierten Artefakte
- Zustandsprüfung eines Dokuments (CI-tauglich)

Eigenschaft:
- **Idempotent**: Mehrfaches Ausführen von Kommandos verändert das Dokument nicht weiter.

### Grundprinzipien

- Überschriftentext ist die **einzige Quelle der Wahrheit**
- Kapitelnummern sind **abgeleitet**, nicht persistent
- Anchor-IDs werden **nur aus dem unnummerierten Titel** erzeugt
- Generierte Inhalte sind vollständig rekonstruierbar
- Keine Änderung ohne klare Zustandsdefinition (Config-Block)

## 2. CLI Kommandos

| Option                                  | Beschreibung                                     |
|-----------------------------------------|--------------------------------------------------|
| `mdtoc --version`                       | Gibt kurze Versionsinfo aus.                     |
| `mdtoc --version --verbose`             | Gibt ausführliche Versionsinfo aus.              |
| `mdtoc --help`                          | Gibt kurzen Help Text aus aus.                   |
| `mdtoc --help --verbose`                | Gibt langen Help Text aus aus.                   |
| `mdtoc generate  --help`                | Gibt langen Help Text speziell für generate aus. |
| `mdtoc strip     --help`                | Gibt langen Help Text speziell für strip aus.    |
| `mdtoc check     --help`                | Gibt langen Help Text speziell für check aus.    |
| `mdtoc generate [--verbose] [OPTIONEN]` | generiert/updated ToC, numbers, anchors.         |
| `mdtoc strip    [--verbose] [--raw]`    | entfernt ToC, numbers, anchors und ggf. Config.  |
| `mdtoc check    [--verbose]`            | prüft Config und ggf. ToC, numbers, anchors.     |

## 3. Optionen (generate)

### Generator-Steuerung

| Option                  | Default | Beschreibung              |
|-------------------------|---------|---------------------------|
| `--numbering <on\|off>` | on      | Kapitelnummern aktivieren |
| `--min-level <N>`       | 2       | Minimale Heading-Ebene    |
| `--max-level <N>`       | 4       | Maximale Heading-Ebene    |
| `--anchors <on\|off>`   | on      | Anchor-Tags erzeugen      |

### Input & Output Steuerung

| Option          | Beschreibung                             |
|-----------------|------------------------------------------|
| `--file <name>` | Datei wird gelesen und überschrieben.     |
| (kein `--file`) | Eingabe von stdin, Ausgabe auf stdout.    |
| `--version`     | Gibt Version-Info aus. |
| `--help`     | Gibt Hilfe-Info aus.|
| `--verbose`     | Gibt Log-Info aus, nutzbar für Debugging, bzw. liefert ausführlichere Informationen. |

- Ohne `--verbose` bei Kommandos grundsätzlich keine Ausgabe, außer bei Fehlern, die dann über den Fehlerkanal stderr kommen.
- `--help` und `--version` geben auch ohne `--verbose` Infos aus.

### Kurzformen

| Option        | Kurzform |
|---------------|----------|
| `--numbering` | `-n`     |
| `--anchors`   | `-a`     |
| `--file`      | `-f`     |
| `--verbose`   | `-v`     |
| `--help`      | `-h`     |

## 4. Processing/Parsing

* Ignoriere alles zwischen Block-Quotes.
  * Beispiel: 
  
  ```diff
   # ignoriere dies
  ```

* Ignoriere alles zwischen zwei Back-Ticks.
  * Beispiel: `Tue so als ob <!-- das hier -->` nicht da ist.
* Ignoriere HTML-Kommentare 
  * Ausnahme: 
  
  ```md
  <!-- mdtoc-config 
  Hier kommt die Config rein. 
  /mdtoc-config -->
  ```

* Suche nach Zeile `<!-- mdtoc -->`
* Suche nach Zeile `<!-- /mdtoc -->`
* Auswertung:
  * Beides nicht gefunden:
    * Füge Zeile `<!-- mdtoc -->` am Dateianfang ein
    * In den nachfolgenden Zeilen wird dann der ToC generiert.
    * Füge Config-Block danach ein.
    * Füge Zeile `<!-- /mdtoc -->` danach ein.
  * `<!-- mdtoc -->` mehrfach gefunden: Parsing Fehler
  * `<!-- /mdtoc -->` mehrfach gefunden: Parsing Fehler
  * `<!-- mdtoc -->` Zeilennummer >= `<!-- mdtoc -->` mehrfach gefunden: Parsing Fehler
  * Wenn gefunden, suche und parse Config-Block.
    * Der Config-Block darf nur einmal existieren, sonst Fehler.
    * Der Config-Block muss unmittelbar vor der Zeile `<!-- /mdtoc -->` sein.
    * Der Config-Block muss konsistent sein.

* Hinweise:
  * Wenn noch kein ToC generiert wurde, wird der Dateianfang als Lokation genommen.
  * Der User kann durch Verschieben des Toc-Bereiches bestimmen, wo das Inhaltsverzeichnis sein soll.
  * Alles was zwischen Zeile `<!-- mdtoc -->` und dem Anfang des Config-Blocks gefunden wird wird als generierter ToC erwartet.
  * Sollten dort andere Inhalte stehen, werden diese nicht gelöscht, sondern in HTML-Kommentar gekapselt (und damit zukünftig ignoriert).

## 5. Config-Block

Der Config-Block wird am ToC-Ende, also nach der eingefügt bzw. geupdatet. Beispiel:

```html
<!-- mdtoc-config
numbering=on
min-level=2
max-level=4
anchors=on
state: generated
/mdtoc-config -->
```

* Alle Generator-Optionen werden geschrieben, falls nicht als Parameter angegeben, dann mit dem Default-Wert.
* Der State wird geschrieben wenn erfolgreich generated oder stripped (außer bei `--raw`).

## 6. Kommandos

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
  <!-- mdtoc -->
  * [1. Abstract](#abstract)
  ...
  <!-- /mdtoc -->
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
- generierte `<a id=...>`-Anker entfernen
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
### 4.1. <a id="open-source"></a>Open source
````

Dabei gilt intern:

- sichtbarer Titel: Open source
- Nummer: 4.1.
- ID: open-source

Und die ID wird nur aus dem unnummerierten Titel berechnet, nie aus der Kapitelnummer.

Worauf du achten solltest

1) Nur generierte Anker entfernen

Beliebige handgeschriebene `<a id="...">` im Dokument nicht blind alle entfernen.

Besser:

Nur Anker entfernen, die direkt am Beginn einer Überschrift stehen und dem generierten Muster entsprechen oder optional einen Marker verwenden.

Beispiel:

`<a id="open-source"></a>` nur entfernen, wenn es direkt nach der Heading-Nummer und direkt vor dem Heading-Text sitzt

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

- Success: keine Ausgabe (außer bei `--verbose`)
- Fehler: Meldung auf stderr
  - Beispiel: `mdtoc: no mdtoc-config block found; file is not managed by mdtoc`
- `--verbose`: Diagnose-/Ablaufmeldungen auf stderr
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
### 4.1. <a id="open-source"></a>Open source
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
  - `--file` wird nicht gespeichert
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

- keine partielle Verarbeitung (--*-only)
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

### strip

- Fehler wenn Config fehlt/kaputt
- benötigt gültigen Config-Block
- entfernt ToC, Nummern, Anchors
- behält Config
- setzt state: stripped

1) Verhalten/Details

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

### strip --raw

- ignoriert Config
- entfernt generierte Artefakte + Config vollständig
  - Wenn nicht sicher entschieden werden kann, ob etwas generiert ist: stehen lassen und Info im Detail ausgeben. 

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

### check

- benötigt gültigen Config-Block, sonst Fehler
- rekonstruiert Sollzustand
- vergleicht mit Istzustand
- Exit 0 bei Gleichheit, sonst Fehler

1) Details

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
- mit `--verbose`:
  - Diagnoseinformationen

## 7. Anchor-Regeln

- Nummer entfernen
- lowercase
- Leerzeichen → '-'
- Sonderzeichen entfernen
- Kollisionen: suffix -2, -3, ...

## 8. Fehlerverhalten

- keine Config → Fehler (außer bei `mdtoc strip --raw`)
- ungültige Config → Fehler
- keine implizite Reparatur

## 9. Idempotenz

mdtoc generate, strip mehrfach → keine Änderung

---



Überarbeite das Dokument direkt, lösche aber nichts. Restrukturiere. Kommentiere den alten Text (wenn auch komplett) stückweise mit HTML-Syntax aus, damit ich es beim review einfacher habe. gib mir einen Download-Link. Noch eins: Ist das hier nicht vielleicht leichter zu implementieren: hashes      := "# " | "## " | "### " | "#### " | "##### " | "###### ", also die Leerzeichen gleich in die hashes. Dein "<!-- mdtoc -->
[TOC CONTENT]
<!-- mdtoc-config -->
...
<!-- /mdtoc -->" muss sein "<!-- mdtoc -->
[TOC CONTENT]
<!-- mdtoc-config
...
-->
<!-- /mdtoc -->" Eine explizite Strukturdefinition ist verständlicher.

Wichtig: Wenn Anchor, dann zwischen Anchorende und ÜberschriftenText KEIN Space um mit dumeng-toc kompatibel zu bleiben. 

Setze all deine Vorschläge um. 

Wenn Du sehr formal wirst, erläutere es.

Overdesigne nicht. Ziel ist Verständlichkei und Eindeutigkeit. Es geht um ein kleines Helper-Tool.

Wenn noch entscheidungen offen sind äußere diese Fragen in Blockquotes an Ort und stelle im Dokument. Stelle anschließend einen Download-Link zur überarbeiteten Makrdown-Datei bereit.

Nutze Die Blockquotes yntax auch für Anmerkungen von Deiner Seite direkt im Dokument.
Überarbeitungshinweise (etwas zusammengestoppelt und nicht komplett):

* Blockquotes: klar definieren und ins Parsing inkludieren

Ignored regions:
1. Fenced code blocks (``` ... ```)
2. Inline code spans (`...`)
3. HTML comments (<!-- ... -->) except mdtoc-config

Not ignore regions:
4. Blockquotes 

Hinweis: Da Blockquotes mit optionalen Space und `>` anfangen könne sie auf keine Hedaline matchen. Siehe Headingt-Syntax

Heading-Syntax:

heading_line :=
  hashes SP [number SP] [anchor SP] title

hashes      := "# " | "## " | "### " | "#### " | "##### " | "###### "
number      := DIGIT+ ("." DIGIT+)* "."
anchor      := "<a id=\"" anchor_id "\"></a>"
title       := NONEMPTY_TEXT
SP          := exactly one U+0020 space

Hinweise:

nach den # muss genau ein Leerzeichen folgen
zwischen allen Komponenten genau ein Leerzeichen
keine führenden Spaces vor #

Also besser:

A managed heading line MUST match:
^#{1,6} ...



Es wird nur dieses Anchor-Format generiert: `<a id="..."></a>` Dann kann der Parser sagen:

wenn Anchor exakt an dieser Stelle steht, ist er verwaltet
wenn nicht, ist er normaler Dokumentinhalt

Das Tool ist ja kein allgemeiner Markdown-Parser, sondern ein deterministisches Dokument-Transformationswerkzeug. Also darf die Spezifikation sagen:

unterstützt nur ATX headings
nur definierte Heading-Struktur
keine Setext-Headings
keine mehrdeutigen Sonderfälle

X) Titel, die absichtlich mit Zahlen anfangen

Das bleibt nur dann sauber lösbar, wenn du wirklich sagst:

eine Nummer ist nur dann eine Nummer, wenn sie direkt nach den Hashes steht
und exakt dem Muster x.y.z. entspricht
und danach ein Leerzeichen folgt

Dann ist das hier kein generierter Nummernblock:

### 2024 roadmap
### 3D graphics

Das ist gut.

Aber das hier wäre dann ein Nummernblock:

Dieses Format darf nicht frei für normale Titel verwenden: Beispiel: `### 2.1. API`

zwischen #, Nummer, Anchor und Titel: genau ein Leerzeichen
innerhalb des Titels: beliebiger Text, unverändert erhalten

Managed headings use this syntax:

<hashes><space>[<number><space>][<anchor><space>]<title>

Where:
- <hashes> is 1 to 6 '#' characters followed by a space
- <space> is exactly one ASCII space
- <number> matches [0-9]+(\.[0-9]+)*\.
- <anchor> matches <a id="..."></a>
- <title> is non-empty text and is the only semantic source of truth

Only headings matching this positional structure may be rewritten by mdtoc.

„mdtoc verarbeitet nur Überschriften, die exakt diesem Syntaxschema entsprechen.“

