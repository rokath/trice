
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
- Idempotent: Mehrfaches Ausführen von Kommandos verändert das Dokument nicht weiter.

## 2. CLI Kommandos

| Option                                  | Beschreibung          |
|-----------------------------------------|-----------------------|
| `mdtoc --version`                       | Gibt Versionsinfo aus |
| `mdtoc generate [--verbose] [OPTIONEN]` | generiert ToC         |
| `mdtoc strip    [--verbose] [--raw]`    | entfernt ToC          |
| `mdtoc check    [--verbose]`            | prüft ToC             |

## 3. Optionen (generate)

### Generator-Steuerung

| Option                  | Default | Beschreibung              | Kurzforn |
|-------------------------|---------|---------------------------|----------|
| `--numbering <on\|off>` | on      | Kapitelnummern aktivieren | `-n`     |
| `--min-level <N>`       | 2       | Minimale Heading-Ebene    |          |
| `--max-level <N>`       | 4       | Maximale Heading-Ebene    |          |
| `--anchors <on\|off>`   | on      | Anchor-Tags erzeugen      | `-a`     |

### Input & Output Steuerung

| Option                   | Beschreibung                               | Kurzform |
|--------------------------|--------------------------------------------|----------|
| `--file <name>`          | Datei wird gelesen und überschrieben       | `-f`     |
| `--file <name> --stdout` | Datei wird gelesen und nicht überschrieben | `-f`     |
| `--stdout`               | Eingabe von stdin, Ausgabe auf stdout      |          |
|                          | Eingabe von stdin, Ausgabe auf stdout      |          |
| `--verbose`              | Gibt Log-Info aus, nutzbar für Debugging   | `-v`     |

Ohne `--verbose` grundsätzlich keine Ausgabe, außer bei Fehlern, die dann über den Fehlerkanal stderr kommen.

### Kurzformen

| Option        | Kurzform |
|---------------|----------|
| `--numbering` | `-n`     |
| `--anchors`   | `-a`     |
| `--file`      | `-f`     |
| `--verbose`   | `-v`     |

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

Alle Optionen werden geschrieben, wenn nicht als Parameter angegeben, dann mit dem Default-Wert.

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
  <!-- mdtoc -->
  ```

  - setzt state: generated

### strip

- Fehler wenn Config fehlt/kaputt
- benötigt gültigen Config-Block
- entfernt ToC, Nummern, Anchors
- behält Config
- setzt state: stripped

### strip --raw

- ignoriert Config
- entfernt generierte Artefakte + Config vollständig
  - Wenn nicht sicher entschieden werden kann, ob etwas generiert ist: stehen lassen und Info im Detail ausgeben. 

### check

- benötigt gültigen Config-Block, sonst Fehler
- rekonstruiert Sollzustand
- vergleicht mit Istzustand
- Exit 0 bei Gleichheit, sonst Fehler

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