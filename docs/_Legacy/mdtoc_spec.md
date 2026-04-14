
Neuer Ansatz zur Bewertung;

mdtoc generate [--numbering on|off] [--min-level 2] [--max-level 5] [--link-anchor on|off] [--file <path>] [--stdout]

Baut alles. Defaults:
 numbering:on
 min:1 
 max:4 
 anchor:on
 file: .docs/TriceUserManual.md 
 
 style erst mal weglassen, aber als ein globales pattern vorhalten, dass es leicht angepasst/nachgerüstet werden kann. Erkläre mir aber die Optionen genauer nur zur Info.
  
 generate soll die verwendeten bzw. Default Parameter in einem Block am ToC Ende ablegen/updaten:
 <!-- mdtoc-config
	--numbering=on
    --min-level=2
	--max-level 5
	--link-anchor=on 
	--file=file_name
	state: generated
mdtoc-config -->

mdtoc strip (ohne Optionen) entfernt alles außer config und setzt state auf stripped
wenn config nicht gefunden wird, keine Aktion und Fehler

mdtoc check (ohne Optionen) prüft anhand des Config Blockes

Wenn check keinen Config-Bock findet: FAIL

Conclusion 1:

--anchors on|off (Default on)
--file=... nicht in die config
Default min-level=2
Default max-level=4
Wenn --file fehlt automatisch stdin annehmen?
Ich habe bei strip deine Variante A vs B vs Empfehlung nicht genau verstanden. Wir sind uns einig: Keine Filemanipulation wen kein config, Fehlercode bei Rückkkehr, 

Die Fehlermeldung ist doch perfekt. was meinst Du mit "generischer fehler"?

Dein check-Verständnis hatte ich genauso gemeint

--anchor oder --anchors? Wahrscheinlich ist Plural besser.

Ich hatte state:... statt state=... vorgeschlagen, damit klar ist, dass es keine Option ist, sondern ein Zustand.

Einen --verbose Switch halte ich insbesondere für Debugging und Fehlersuche für hilfreich.

Frage: on|off vs true|false - was ist besser?
Frage: zwei Unterstriche oder einer bei den Optionen. Go hat als default wohl nur einen, aber viele Tools haben 2

Wenn Du jetzt antwortest wiederholde nicht, was geklärt ist, sondern gehe nur auf Fragen und Optionen ein.

CONCLUSION 2

bei stdin ohne --stdout: Fehler, weil kein Rückschreibziel existiert <- Warum kann bei stdin nicht in stdout geschrieen werden? Fälle:
--file name: ändere in Datei name
--file name --stdout: schreibe nach stdout
(keine Angabe) oder nur --stdout: lies stdin und schreibe nach stdout (unix-like)
Was wir nicht haben: --in-file und --out-file, aber das sit overkill

Was ist mit zusätzlich
<!-- mdtoc-state 
generated 
mdtoc-state-->

Ohne --verbose grundsätzlich keine Ausgabe, außer bei Fehlern, die dann über den Fehlerkanal, oder?

Go ist vermutlich tolerant bzgl. Leerzeichen oder = bei CLI Werten, aber Leerzeichen ist perfekt.

Kurzsyntax Optionen: 
--numbering -n
--min-level 
--max-level 
--anchors   -a
--file -f
--stdout -o

CONCXLUSION 3:

mdtoc generate [OPTIONEN]
mdtoc strip [--verbose|-v]
mdtoc check [--verbose|v]

| Option                   | Kurzform | Default | Beschreibung           |
| ------------------------ | -------- | ------- | ---------------------- | 
| `--numbering <on|off>    | -n       | on      | Kapitelnummern aktivieren |
| `--min-level <N>`        | –        | 2       | Minimale Heading-Ebene |       
| `--max-level <N>`        | –        | 4       | Maximale Heading-Ebene |      
| `--anchors <on|off>      | -a       | on      | Anchor-Tags erzeugen      |

| Fall                              | Verhalten                |
| --------------------------------- | ------------------------ |
| `--stdout` gesetzt                | Ausgabe auf stdout       |
| `--file` gesetzt, kein `--stdout` | Datei wird überschrieben |
| `--file` nicht gesetzt            | Eingabe von stdin, Ausgabe auf stdout       |




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
- Idempotent: Mehrfaches Ausführen von `generate` verändert das Dokument nicht weiter.

## 2. CLI

mdtoc generate [OPTIONEN]  
mdtoc strip [--verbose|-v] [--raw]  
mdtoc check [--verbose|-v]

## 3. Optionen (generate)

--numbering <on|off> (-n) Default: on  
--min-level <N> Default: 2  
--max-level <N> Default: 4  
--anchors <on|off> (-a) Default: on  
--file <path> (-f)  
--stdout  
--verbose (-v)

## 4. I/O Verhalten

--file gesetzt → Datei lesen  
kein --file → stdin lesen  

--stdout → stdout schreiben  
--file ohne --stdout → Datei überschreiben  
kein --file → stdin → stdout  

Dokumentinhalt → stdout  
Fehler/Logs → stderr  

## 5. Config-Block

<!-- mdtoc-config
numbering=on
min-level=2
max-level=4
anchors=on
state: generated
mdtoc-config -->

## 6. Kommandos

### generate
- entfernt generierte Artefakte
- erzeugt ToC, Nummern, Anchors
- schreibt Config
- setzt state: generated
- Fehler bei ungültiger Config

### strip
- benötigt gültigen Config-Block
- entfernt ToC, Nummern, Anchors
- behält Config
- setzt state: stripped
- Fehler wenn Config fehlt/kaputt

### strip --raw
- ignoriert Config
- entfernt generierte Artefakte + Config vollständig

### check
- benötigt gültigen Config-Block
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

- keine Config → Fehler
- ungültige Config → Fehler
- keine implizite Reparatur

## 9. Idempotenz

mdtoc generate mehrfach → keine Änderung