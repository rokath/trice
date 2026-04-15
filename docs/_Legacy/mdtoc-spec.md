
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

| Option                                  | Beschreibung                                   |
|-----------------------------------------|------------------------------------------------|
| `mdtoc --version`                       | Gibt Versionsinfo aus                          |
| `mdtoc generate [--verbose] [OPTIONEN]` | generiert/updated ToC, numbers, anchors        |
| `mdtoc strip    [--verbose] [--raw]`    | entfernt ToC, numbers, anchors und ggf. Config |
| `mdtoc check    [--verbose]`            | prüft Config und ggf. ToC, numbers, anchors    |

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
| `--file <name>` | Datei wird gelesen und überschrieben     |
| (kein `--file`) | Eingabe von stdin, Ausgabe auf stdout    |
| `--verbose`     | Gibt Log-Info aus, nutzbar für Debugging |

Ohne `--verbose` grundsätzlich keine Ausgabe, außer bei Fehlern, die dann über den Fehlerkanal stderr kommen.

### Kurzformen

| Option        | Kurzform |
|---------------|----------|
| `--numbering` | `-n`     |
| `--anchors`   | `-a`     |
| `--file`      | `-f`     |
| `--verbose`   | `-v`     |

## 4. Processing/Parsing

* Ignoriere alles zwischen Block-Quotes
  * Beispiel: ```diff ... ```
* Ignoriere HTML-Kommentare 
  * Ausnahme: <!-- mdtoc ... /mdtoc -->
  * Ausnahme: <!-- mdtoc-config ... /mdtoc-config -->

## 5. Config-Block

Der Config-Block wird am ToC-Ende eingefügt bzw. geupdatet. Beispiel:

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
- generierte <a id=...>-Anker entfernen
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

Beliebige handgeschriebene <a id="..."> im Dokument nicht blind alle entfernen.

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

- Success: keine Ausgabe
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
````

→

```md
### 4.1. <a id="open-source"></a>Open source
````

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
````

→ keine Änderung beim zweiten Lauf

```bash
mdtoc strip
mdtoc strip
````

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
