# Trice-IDs ohne Sourcecode-Patching

## Analyse von TreeLog und mögliche Hybridarchitekturen

**Stand:** 23. Juli 2026  
**Status:** Architekturüberlegung, noch kein Implementierungsentwurf  
**Ziel:** Einen tragfähigen Ansatz finden, der TreeLogs unveränderten Sourcecode mit Trices stabilen, policygesteuerten IDs und langfristiger Decodierbarkeit verbindet.

---

## 1. Zusammenfassung

Trice und TreeLog lösen zwei verschiedene Teilprobleme jeweils besonders gut:

- **Trice** verwaltet stabile numerische IDs, eine ID-Policy und eine kumulative `til.json`. Die ID wird mit `trice insert` als Konstante in den Sourcecode geschrieben und dadurch vom Compiler unmittelbar in das Image übernommen.
- **TreeLog** verändert den Sourcecode nicht. Jede Logstelle hinterlegt Metadaten in ELF-Sektionen; der Linker ordnet die Logstellen eindeutig an. Aus dieser buildlokalen Anordnung wird die gesendete ID abgeleitet.

Der zentrale Nachteil von Trice ist der `insert`/`clean`-Workflow beziehungsweise die sichtbare Instrumentierung des Sourcecodes. Der zentrale Nachteil von TreeLog ist, dass seine IDs vom konkreten Linkerlayout abhängen und deshalb zwischen Builds nicht stabil sind. Für die spätere Decodierung muss daher das zum Firmware-Build passende ELF oder ein daraus exportiertes Dictionary verfügbar sein.

Die aussichtsreichste Verbindung beider Welten besteht darin, die bisher gekoppelte Funktion von `trice insert` aufzuteilen:

1. **ID-Allocator:** Erkennt eine Logstelle und ordnet ihr eine stabile, policygerechte ID zu.
2. **ID-Binder:** Bringt diese ID ohne Änderung der Originalquellen in den erzeugten Targetcode.

Dafür erscheinen drei Varianten besonders relevant:

1. **Instrumentierte Build-Kopie der Quellen:** portabel, risikoarm und schnell realisierbar.
2. **Generierter, übersetzungseinheitsspezifischer ID-Header:** Originalquellen bleiben unverändert; die ID ist weiterhin eine Compile-Time-Konstante.
3. **ELF-/Linker-Binder:** Logstellen werden in Objektdateien erkannt; stabile IDs werden vor dem finalen Link direkt als Symbolwerte oder Relocations gebunden. Das ist strategisch am interessantesten und unterstützt grundsätzlich auch vorkompilierte Bibliotheken.

Eine Runtime-Mapping-Tabelle ist ein robuster Fallback, verursacht aber Flashbedarf und einen zusätzlichen Tabellenzugriff pro Logaufruf.

---

## 2. TreeLog: Einordnung und Funktionsprinzip

[TreeLog](https://github.com/hefanbo/treelog) ist ein eigenständiges, auf Trice aufbauendes Projekt für tokenisiertes Logging in C. TreeLog verwendet Trice für wesentliche Teile des Drahtprotokolls, der Pufferung, des Transports und der Host-Decodierung. Die eigene Kernidee ist die linkerbasierte Erzeugung der Log-IDs.

Für jede Logstelle erzeugen die Makros Metadaten, unter anderem:

- Trice- beziehungsweise TreeLog-Typ,
- Timestamp-Variante,
- Formatstring,
- Gruppe und Log-Level,
- Quelldatei und Zeilennummer.

Diese Metadaten werden in speziellen ELF-Sektionen abgelegt. Zusätzlich enthält `.trlog.index` ein Array von Zeigern auf die Metadateneinträge. Die Position eines Zeigers in diesem Array ist innerhalb eines Builds eindeutig.

TreeLog verändert dafür auch das Trice-TID-Layout:

```text
Trice:

15          14 13                              0
+--------------+--------------------------------+
| StampType    |             ID                 |
+--------------+--------------------------------+
     2 Bit                  14 Bit
```

```text
TreeLog:

15                               2 1            0
+--------------------------------+--------------+
|              ID                | StampType    |
+--------------------------------+--------------+
                14 Bit                2 Bit
```

Auf einer 32-Bit-Architektur sind die Zeigerelemente in `.trlog.index` vier Byte groß und ausgerichtet. Ihre Adressen enden daher auf zwei Nullbits:

```text
Adresse des Indexelements = Index << 2
```

TreeLog kann die unteren 16 Adressbits direkt mit dem Timestamp-Typ verknüpfen:

```c
tid = addr16 | stampType;
```

Der Decoder gewinnt daraus:

```c
stampType = tid & 3;
id        = tid >> 2;
```

Die Bitverschiebung ist somit keine allgemeine Verbesserung des Trice-Protokolls. Sie ist eine gezielte Anpassung an die linkerbasierte TreeLog-ID.

Weitere technische Details finden sich im [TreeLog Developer Manual](https://github.com/hefanbo/treelog/blob/master/docs/DeveloperManual.adoc). Der zugehörige angepasste Trice-Fork ist im [Vergleich `main...hefanbo:treelog`](https://github.com/rokath/trice/compare/main...hefanbo%3Atrice%3Atreelog) beziehungsweise im Commit [„adapt for TreeLog integration“](https://github.com/hefanbo/trice/commit/d6b7fb1) sichtbar.

### 2.1 Wichtige Präzisierung zur Build-Abhängigkeit

Die Firmware benötigt zur Laufzeit nicht das ELF oder Dictionary. Der Linker hat die buildlokale ID bereits in den Maschinencode eingebracht.

Das passende Artefakt wird auf der **Hostseite zur Decodierung** benötigt:

- entweder das konkrete Firmware-ELF,
- oder die daraus exportierte `trlog_dict.json`.

TreeLog dokumentiert ausdrücklich, dass das Hinzufügen, Entfernen oder Umsortieren von Logstellen die Positionen in `.trlog.index` und damit die IDs verändern kann. Die `trlog_dict.json` ist deshalb genauso buildspezifisch wie das ELF, aus dem sie erzeugt wurde.

---

## 3. Die zwei voneinander zu trennenden Fragen

Die bisherige Diskussion wird klarer, wenn zwei Aufgaben strikt getrennt betrachtet werden.

### 3.1 Wie wird einer Logstelle eine stabile ID zugeordnet?

Das ist ein Problem der **Identität und Verwaltung**.

Trice löst es heute mit:

- `trice insert`,
- `til.json`,
- `li.json`,
- ID-Bereichen und Vergabepolicy,
- Wiedererkennung bereits bekannter Logstellen.

Beispiel:

```c
trice("temperature=%d\n", temperature);
```

wird logisch der stabilen ID `4711` zugeordnet.

Die Zuordnung muss persistent gespeichert werden:

```text
logische Logstelle -> stabile numerische ID
```

Ohne einen solchen persistenten Zustand sind gleichzeitig

- stabile IDs,
- Kollisionsfreiheit,
- eine kontrollierte ID-Policy und
- langfristige Decodierbarkeit

nicht zuverlässig möglich.

Die persistente Zuordnung muss aber nicht im Sourcecode liegen. Sie kann weiterhin in einer Registry wie `til.json`/`li.json` oder in einem neuen Datenmodell gespeichert werden.

### 3.2 Wie gelangt diese ID in den Targetcode?

Das ist ein Problem der **Bindung beziehungsweise Einbettung**.

Heute erledigt `trice insert` dies, indem es die ID in den Sourcecode schreibt:

```c
trice(iD(4711), "temperature=%d\n", temperature);
```

Der Compiler sieht eine Konstante und übernimmt sie effizient in den Maschinencode.

TreeLog bindet dagegen eine Linkeradresse in den Code ein. Diese Adresse repräsentiert nur die buildlokale Position der Logstelle.

Die gesuchte Hybridlösung soll Folgendes erreichen:

```text
Originalsource ohne numerische ID
          +
externe stabile ID-Zuordnung
          +
automatische Bindung beim Build
          =
Targetcode mit stabiler Trice-ID
```

---

## 4. Anforderungen an eine tragfähige Hybridlösung

Ein überzeugender Ansatz sollte möglichst viele der folgenden Eigenschaften erfüllen:

1. **Keine Änderung der Originalquellen**
   - kein sichtbares `insert`,
   - kein nachträgliches `clean`,
   - keine ID-bedingten Git-Diffs.

2. **Stabile IDs**
   - Verschieben einer unveränderten Logstelle soll ihre ID nicht ändern.
   - Das Hinzufügen anderer Logstellen soll vorhandene IDs nicht beeinflussen.

3. **ID-Policy**
   - definierbare ID-Bereiche,
   - reservierte Bereiche,
   - Vergabe nach Komponenten, Gruppen oder Projekten,
   - kontrollierte Behandlung externer Bibliotheken.

4. **Langfristige Decodierbarkeit**
   - alte Aufzeichnungen sollen mit einer kumulativen ID-Liste decodierbar bleiben,
   - kein zwingendes buildspezifisches ELF für jede Aufzeichnung.

5. **Geringe Targetkosten**
   - ID möglichst als Compile-/Link-Time-Konstante,
   - kein String-Lookup auf dem Target,
   - möglichst kein zusätzlicher Flashbedarf und kein zusätzlicher Tabellenzugriff.

6. **Gute Buildintegration**
   - deterministisch,
   - CI-tauglich,
   - klare Fehler bei Mehrdeutigkeiten,
   - reproduzierbare Artefakte.

7. **Perspektivisch Binärbibliotheken**
   - Logstellen aus `.a`-Bibliotheken sollen ohne vorher ausgehandelte numerische ID-Bereiche eingebunden werden können.

8. **Beibehaltung des Trice-Drahtformats**
   - eine inkompatible Veränderung des TID-Layouts soll vermieden werden.

Keine einzelne Variante erfüllt alle Punkte gleich gut. Daher ist eine gestufte Architektur sinnvoll.

---

## 5. Stabile Identität einer Logstelle

Bevor eine ID gebunden werden kann, muss das Werkzeug entscheiden, ob eine gefundene Logstelle neu oder bereits bekannt ist.

### 5.1 Der Positionsindex ist keine langfristige Identität

Eine deterministische Sortierung wie

```text
Filename + Line + Trice-Typ + Formatstring
```

ist nützlich, um die Logstellen eines konkreten Builds in eine reproduzierbare Reihenfolge zu bringen. Der daraus entstehende Positionsindex ist aber nicht stabil:

```text
src/motor.c:100  "speed=%d"
```

wird nach eingefügten Quellzeilen beispielsweise zu:

```text
src/motor.c:125  "speed=%d"
```

Auch das Einfügen einer früher sortierten Datei kann viele nachfolgende Indizes verändern.

Der Positionsindex eignet sich daher als:

- lokaler Build-Index,
- Tabellenindex,
- technischer Schlüssel zwischen Compiler und Binder,

aber nicht als langfristige Trice-ID.

### 5.2 Empfohlene Wiedererkennung

Ein neuer ID-Allocator könnte mehrere Merkmale kombinieren:

1. normalisierter Projektpfad,
2. Trice-Typ und Parameterstruktur,
3. Formatstring,
4. Quellzeile als Hinweis, nicht als Identität,
5. Hash eines kleinen normalisierten Quellkontexts,
6. Nachbarschaft zu anderen bekannten Trice-Stellen,
7. optional Git-Diff- oder Rename-Informationen.

Eine mögliche Matching-Reihenfolge:

```text
1. Expliziter symbolischer Schlüssel vorhanden?
   -> eindeutig zuordnen.

2. Datei, Typ und Formatstring stimmen exakt?
   -> bekannte ID übernehmen.

3. Datei wurde verschoben oder umbenannt, Typ und Formatstring sind eindeutig?
   -> bekannte ID übernehmen.

4. Mehrere gleiche Meldungen vorhanden?
   -> Zeile, Kontext-Hash und Nachbarschaft auswerten.

5. Zuordnung bleibt mehrdeutig?
   -> nicht raten; Warnung oder Fehler und neue ID nur nach klarer Regel.
```

### 5.3 Änderung des Formatstrings

Eine „stabile ID“ sollte nicht bedeuten, dass jede beliebige Änderung derselben Quellstelle zwingend dieselbe ID behält.

Ändert sich beispielsweise

```c
trice("temperature=%d\n", t);
```

zu

```c
trice("temperature=%d, voltage=%d\n", t, u);
```

haben sich Format und Parameterstruktur geändert. Wird dieselbe ID wiederverwendet, kann eine kumulative `til.json` alte und neue Binärlogs nicht mehr gleichzeitig eindeutig interpretieren.

Daher sollte gelten:

- **reine Verschiebung oder Umbenennung:** ID kann stabil bleiben,
- **Format- oder Typänderung:** normalerweise neue ID,
- **inhaltlich kompatible Textkorrektur:** nur nach expliziter Policy.

### 5.4 Optionaler symbolischer Schlüssel

Für besonders langlebige oder extern relevante Logstellen kann ein optionaler semantischer Schlüssel hilfreich sein:

```c
triceK(
    "motor.overtemperature",
    "motor temperature=%d\n",
    temperature
);
```

Der Schlüssel ersetzt keine Versionsregeln für inkompatible Formatänderungen, verbessert aber die Wiedererkennung bei:

- Verschieben in andere Dateien,
- Refactoring,
- mehrfach identischen Formatstrings,
- gemeinsam genutzten Bibliotheken.

Für normale Debugmeldungen sollte ein solcher Schlüssel nicht zwingend erforderlich sein.

---

## 6. Lösungsvariante A: Instrumentierte Build-Kopie

### 6.1 Prinzip

Die Originalquellen bleiben unverändert. Vor der Compilation erzeugt ein Werkzeug eine instrumentierte Kopie im Buildverzeichnis:

```text
src/
  motor.c

        |
        | trice stage
        v

build/trice-src/
  motor.c
```

Original:

```c
trice("temperature=%d\n", temperature);
```

Build-Kopie:

```c
trice(iD(4711), "temperature=%d\n", temperature);
```

Kompiliert wird ausschließlich die Build-Kopie.

### 6.2 Eigenschaften

**Vorteile**

- compiler- und linkerunabhängig,
- bestehende Trice-Makros und bestehendes Drahtformat,
- IDs bleiben reine Compile-Time-Konstanten,
- keine Target-Mehrkosten,
- ID-Allocator kann weitgehend aus `trice insert` hervorgehen,
- kein `clean`, da die Originaldatei nie verändert wird,
- vergleichsweise einfach und risikoarm.

**Nachteile**

- das Buildsystem muss auf die gespiegelten Quellen umgestellt werden,
- relative Includes und generierte Quellen müssen korrekt behandelt werden,
- Debugpfade und Fehlermeldungen sollen auf Originalquellen zeigen,
- vorkompilierte Bibliotheken können nicht nachträglich instrumentiert werden.

Mit `#line`-Direktiven oder Compileroptionen für Quellpfad-Mapping können Diagnose- und Debugpfade weitgehend auf die Originaldateien zurückgeführt werden.

### 6.3 Bewertung

Diese Variante ist kein fundamentaler technischer Durchbruch, aber ein sehr guter praktischer erster Schritt:

- Sie beseitigt den problematischen Schreibzugriff auf das Arbeitsverzeichnis.
- Sie erhält das bewährte Trice-ID-Modell vollständig.
- Sie bietet eine Referenzimplementierung für den getrennten ID-Allocator.

Sie sollte als **portabler Basismodus** betrachtet werden, selbst wenn später ein eleganterer Binder hinzukommt.

---

## 7. Lösungsvariante B: Generierte ID-Header pro Übersetzungseinheit

### 7.1 Grundidee

Ein Scanner ermittelt vor der Compilation alle Trice-Stellen einer Quelldatei und ordnet ihnen stabile IDs zu. Anschließend erzeugt er einen nur für diese Übersetzungseinheit gültigen Header.

Für `src/motor.c` könnte entstehen:

```c
/* build/trice-ids/src_motor_c.h */

#define TRICE_ID_AT_LINE_42   4711
#define TRICE_ID_AT_LINE_87   8123
#define TRICE_ID_AT_LINE_130  12001
```

Das Trice-Makro verwendet die aktuelle Zeilennummer zur Compilezeit:

```c
#define TRICE_ID_AT_LINE_I(line) TRICE_ID_AT_LINE_##line
#define TRICE_ID_AT_LINE(line)   TRICE_ID_AT_LINE_I(line)

#define trice(...) \
    TRICE_WITH_ID(TRICE_ID_AT_LINE(__LINE__), __VA_ARGS__)
```

Der Compiler erhält für jede Quelldatei den passenden Header, beispielsweise über:

```text
-DTRICE_ID_HEADER="build/trice-ids/src_motor_c.h"
```

und `trice.h` bindet ihn ein:

```c
#include TRICE_ID_HEADER
```

### 7.2 Laufzeitverhalten

Die ID ist nach der Präprozessorauflösung eine normale Konstante:

```c
TRICE_WITH_ID(4711, "temperature=%d\n", temperature);
```

Es gibt:

- keine Runtime-Suche,
- keine Mapping-Tabelle im Target,
- keinen zusätzlichen Tabellenzugriff,
- keine Änderung des Originalsource.

### 7.3 Grenzen

Die einfache Zeilenvariante benötigt Regeln:

- höchstens eine Trice-Logstelle pro physischer Quellzeile,
- `#line`-Direktiven und generierter Code müssen kontrolliert behandelt werden,
- der Header muss nach jeder relevanten Quelländerung neu erzeugt werden,
- jede Übersetzungseinheit benötigt den korrekten, individuellen Header.

`__COUNTER__` könnte statt `__LINE__` lokale Nummern erzeugen. GCC dokumentiert `__COUNTER__` als fortlaufenden Zähler. Seine Verwendung ist jedoch empfindlich, wenn andere Makros derselben Übersetzungseinheit ebenfalls `__COUNTER__` verwenden. Ein Buildwerkzeug müsste dann den tatsächlich präprozessierten Zustand kennen oder einen robusten lokalen Namensraum schaffen.

### 7.4 Buildintegration

Geeignete Integrationsformen wären:

- Compiler-Wrapper,
- CMake-Generator mit per-source compile options,
- Ninja-/Make-Regeln,
- erzeugte Wrapper-Übersetzungseinheiten,
- ein neues Tool, das eine `compile_commands.json` liest und die Compile-Kommandos ergänzt.

### 7.5 Bewertung

Diese Variante ist technisch interessant, weil sie

- ohne Sourcekopie,
- ohne Linkertrick,
- ohne Targettabelle

auskommt.

Sie ist portabler als ein ELF-Binder, verlangt aber eine engere Buildsystemintegration. Die Zeilenadressierung ist weniger elegant als linkerbasierte Metadaten, kann für einen kontrollierten C-Codebestand jedoch tragfähig sein.

Sie eignet sich besonders als **mittelfristiger portabler Binder**, wenn die Einschränkung „eine Trice-Stelle pro Zeile“ akzeptabel ist.

---

## 8. Lösungsvariante C: Buildlokaler Index plus stabile Runtime-Mapping-Tabelle

### 8.1 Prinzip

Jede Logstelle erzeugt wie bei TreeLog einen Anchor beziehungsweise Indexeintrag. Der Linker bringt alle tatsächlich eingebundenen Logstellen in eine eindeutige lokale Reihenfolge:

```text
lokaler Index 0 -> Logstelle A
lokaler Index 1 -> Logstelle B
lokaler Index 2 -> Logstelle C
```

Der ID-Allocator ordnet stabile IDs zu:

```text
A -> 4711
B -> 8123
C -> 12001
```

Ein Buildwerkzeug erzeugt eine Tabelle:

```c
const uint16_t triceIdMap[] = {
    4711,
    8123,
    12001,
};
```

Das Makro berechnet oder erhält seinen lokalen Index und verwendet:

```c
uint16_t id = triceIdMap[localIndex];
```

Danach sendet die Firmware die normale stabile Trice-ID.

### 8.2 Buildablauf

```text
1. Quellen kompilieren
2. Vorläufig linken oder Objektdateien analysieren
3. lokale Logstellenreihenfolge feststellen
4. stabile IDs aus Registry zuordnen
5. triceIdMap.c oder triceIdMap.o erzeugen
6. final linken
```

### 8.3 Kosten

Bei einer 16-Bit-ID:

```text
Flashbedarf = 2 Byte * Anzahl der Logstellen
```

Beispiele:

| Logstellen | Tabellenbedarf |
|---:|---:|
| 500 | ca. 1 KiB |
| 1.000 | ca. 2 KiB |
| 5.000 | ca. 10 KiB |
| 16.384 | 32 KiB |

Zusätzlich entsteht pro Logaufruf mindestens ein Tabellenzugriff. Je nach Indexberechnung kommen Subtraktion und Shift hinzu.

### 8.4 Vorteile

- Originalsource bleibt unverändert,
- stabile IDs auf dem Draht,
- normale kumulative `til.json`,
- linkerbasierte Erfassung tatsächlich verwendeter Logstellen,
- grundsätzlich auch Logstellen aus `.a`-Bibliotheken,
- deutlich einfacher als Relocation- oder Objektdatei-Patching.

### 8.5 Nachteile

- zusätzlicher Flashbedarf,
- zusätzlicher Runtimezugriff,
- mindestens teilweise linker- beziehungsweise Objektformat-spezifisch,
- meist zweistufiger Linkprozess.

### 8.6 Bewertung

Die Tabelle ist ein sehr guter **technischer Fallback**:

- robust,
- gut nachvollziehbar,
- relativ leicht zu testen,
- geeignet für einen Proof of Concept.

Sie ist aber nicht die ideale Endlösung, wenn maximale Targeteffizienz ein Hauptziel bleibt.

---

## 9. Lösungsvariante D: ELF-/Linker-Binder mit direkter ID-Auflösung

### 9.1 Ziel

Das Trice-Makro erzeugt noch keine numerische ID, sondern eine symbolische Referenz auf die lokale Logstelle. Nach der Compilation liest ein Binder die Metadaten aus `.o`- und `.a`-Dateien, ordnet stabile IDs zu und lässt den finalen Linker diese IDs direkt in den Code einsetzen.

Der Sourcecode bleibt:

```c
trice("temperature=%d\n", temperature);
```

Im Objektcode existieren konzeptionell:

- ein Metadateneintrag,
- ein lokales Site-Symbol,
- eine Relocation vom Logaufruf auf den ID-Wert.

Der Binder entscheidet:

```text
Site aus src/motor.c -> stabile ID 4711
```

Der finale Link enthält an der Aufrufstelle direkt `4711`, ohne Runtime-Tabelle.

### 9.2 Möglicher Buildablauf

```text
saubere C-Quellen
       |
       | normale Compilation
       v
.o / .a mit Trice-Metadaten und symbolischen ID-Referenzen
       |
       | trice bind
       | - Metadaten lesen
       | - Sites wiedererkennen
       | - stabile IDs vergeben
       | - Symbolwerte/Relocations erzeugen
       v
finaler Link
       |
       v
Firmware mit normalen stabilen Trice-IDs
```

### 9.3 Technische Umsetzungsformen

#### A. Generiertes Linkerskript mit absoluten Symbolwerten

GNU `ld` kann Symbolen in einem Linkerskript numerische Werte zuweisen. Der Binder könnte aus den zugeordneten IDs ein Fragment erzeugen:

```ld
__trice_id_site_a = 4711;
__trice_id_site_b = 8123;
```

Der Targetcode referenziert die Symboladresse beziehungsweise den Symbolwert.

Problematisch ist die eindeutige Benennung über mehrere Übersetzungseinheiten. Mögliche Lösungen:

- pro Object-Datei eindeutig umbenannte Symbole,
- ein Compiler-Wrapper, der einen Übersetzungseinheits-Hash vorgibt,
- Binder-seitiges Umbenennen lokaler Symbole,
- pro Site eindeutig benannte ELF-Subsektionen.

GNU-`ld` dokumentiert die [Zuweisung von Werten an Symbole](https://sourceware.org/binutils/docs/ld/Assignments.html) und die [Referenz solcher Symbole aus Sourcecode](https://sourceware.org/binutils/docs/ld/Source-Code-Reference.html).

#### B. Erzeugtes ID-Objekt

Der Binder kann eine Assembler- oder Objektdatei erzeugen, welche die benötigten Symbole mit absoluten Werten definiert. Dieses Objekt wird beim finalen Link ergänzt.

#### C. Direkte Anpassung von ELF-Symbolen oder Relocations

Der Binder kann lokale Site-Symbole in den Objektdateien identifizieren und deren Werte beziehungsweise die zugehörigen Relocations auf die stabile ID umstellen.

Das ist besonders leistungsfähig, aber architektur- und objektformatspezifisch:

- ELF-Klassen,
- ARM-, RISC-V- und andere Relocationtypen,
- Archive,
- Section-Garbage-Collection,
- LTO,
- Debugsymbole

müssen korrekt behandelt werden.

#### D. Anchor-Adressen codieren die stabile ID

Ähnlich wie TreeLog könnte jede Logstelle einen Anchor referenzieren. Anders als bei TreeLog würde nicht die zufällige Reihenfolge die ID bestimmen. Der Binder erzeugt ein Linkerskript, das jedem Anchor eine Adresse zuweist, deren relevante Bits der stabilen TID entsprechen.

```text
Anchor A -> Adresse ...4711
Anchor B -> Adresse ...8123
```

Das Makro übernimmt den Adresswert als ID. Die Anchor-Sektionen können nicht allokiert sein oder in einer Dummy-Memory-Region liegen und müssen nicht im BIN/HEX erscheinen.

Diese Variante verbindet TreeLogs Adressmechanik direkt mit einer extern verwalteten stabilen ID. Sie ist konzeptionell elegant, benötigt aber sorgfältige Tests mit den verwendeten Compilern und Linkern.

### 9.4 Vorteile

- unveränderte Originalquellen,
- keine Runtime-Tabelle,
- kein zusätzlicher Tabellenzugriff,
- stabile Trice-IDs auf dem Draht,
- unverändertes Trice-TID-Layout,
- kumulative `til.json`,
- vollständige ID-Policy,
- grundsätzlich Unterstützung vorkompilierter `.a`-Bibliotheken,
- Metadaten können aus BIN/HEX entfernt werden.

### 9.5 Risiken und offene technische Punkte

- zunächst voraussichtlich ELF-spezifisch,
- GNU-GCC/GNU-ld und Clang/LLD können sich unterscheiden,
- LTO verschiebt Codeerzeugung und Optimierung in die Linkphase,
- Compiler können Adressreferenzen unterschiedlich materialisieren,
- nicht allokierte Metadatensektionen dürfen Referenzen aus Code nicht unzulässig machen,
- `--gc-sections` darf benötigte Site-Metadaten nicht vor der Binderanalyse entfernen,
- Archivmitglieder werden normalerweise nur bei Bedarf aus `.a` geladen,
- die Methode muss auf ARM und RISC-V mit realen Relocationtypen verifiziert werden.

### 9.6 Bewertung

Der ELF-/Linker-Binder ist die **strategisch beste Lösung**, sofern der technische Proof of Concept gelingt.

Er erreicht nahezu das Optimum:

```text
TreeLog:
  unveränderte Quellen
  + Binärbibliotheken

Trice:
  stabile IDs
  + ID-Policy
  + langfristige Decodierbarkeit
  + unverändertes Drahtformat
```

Der Preis ist eine deutlich höhere Implementierungs- und Testkomplexität.

---

## 10. Lösungsvariante E: Compiler- oder AST-Plugin

Ein Compiler-Frontend kann Trice-Aufrufe erkennen und die stabile ID während der Compilation in AST oder IR einsetzen.

Mögliche Technologien:

- GCC-Plugin,
- Clang-Plugin,
- [Clang LibTooling](https://clang.llvm.org/docs/LibTooling.html),
- Clang Transformer beziehungsweise AST Matchers.

### Vorteile

- keine Änderung der Originalquellen,
- ID kann direkt als Konstante in den erzeugten Code gelangen,
- semantisch genaue Erkennung von Makros und Aufrufen,
- kein Runtime-Overhead.

### Nachteile

- starke Bindung an Compiler und Version,
- hoher Wartungsaufwand,
- GCC und Clang benötigen unterschiedliche Implementierungen,
- Integration in proprietäre Embedded-Toolchains schwierig,
- vorkompilierte Libraries sind nur erfasst, wenn sie bereits mit dem Plugin gebaut wurden.

Diese Variante ist technisch möglich, aber als allgemeiner Trice-Standard weniger attraktiv als ein ELF-Binder oder der portable Build-Kopien-Modus.

---

## 11. Vorkompilierte Bibliotheken

Eine wesentliche Stärke des TreeLog-Modells ist die Einbindung vorkompilierter `.a`-Bibliotheken ohne vorherige Abstimmung numerischer ID-Bereiche.

Ein Hybridmodell könnte dies erhalten:

1. Die Bibliothek enthält Trice-Code, Metadaten und symbolische Site-Referenzen.
2. Sie enthält noch keine endgültigen projektspezifischen IDs.
3. Beim finalen Produktlink werden nur tatsächlich benötigte Archivobjekte eingebunden.
4. Der Binder sieht Anwendung und Bibliotheken gemeinsam.
5. Die stabile ID-Policy des Endprodukts weist allen Sites IDs zu.
6. Die finalen IDs werden direkt gebunden oder über die Mapping-Tabelle bereitgestellt.

Damit könnten Bibliotheken einen symbolischen Namespace mitbringen, etwa:

```text
vendor.sensor.temperature
vendor.sensor.timeout
```

ohne feste numerische Bereiche zu beanspruchen.

Das spricht stark für einen ELF-/Linker-Binder. Build-Kopie und generierte Header können bereits kompilierte Bibliotheken nicht nachträglich erfassen.

---

## 12. Build-Artefakte und Datenmodell

Ein neues System sollte die Artefakte klar unterscheiden.

### 12.1 Langfristige Registry

Sie enthält die stabile Zuordnung und muss erhalten bleiben:

```text
stable-id-registry.json
```

Mögliche Felder:

```json
{
  "version": 1,
  "entries": {
    "4711": {
      "type": "TRICE32_1",
      "format": "temperature=%d\n",
      "logicalKey": "motor.temperature",
      "lastPath": "src/motor.c",
      "lastLine": 125,
      "contextHash": "..."
    }
  }
}
```

Die existierende `til.json` und `li.json` könnten weiterverwendet oder zu einem erweiterten Registry-Modell ergänzt werden.

### 12.2 Buildlokale Site-Liste

Sie beschreibt die in einem konkreten Build gefundenen Logstellen:

```text
trice-sites.json
```

Beispiel:

```json
{
  "buildSite": 17,
  "object": "build/src/motor.c.o",
  "localSymbol": "__trice_site_4",
  "type": "TRICE32_1",
  "format": "temperature=%d\n",
  "file": "src/motor.c",
  "line": 125,
  "stableID": 4711
}
```

Diese Liste ist ein Build-Hilfsmittel und muss nicht zur späteren Decodierung archiviert werden, wenn die Firmware bereits stabile IDs sendet.

### 12.3 Host-Dictionary

Für das bestehende Trice-Modell bleibt die kumulative `til.json` das zentrale Decodierartefakt.

Zusätzlich ist eine Firmware-Build-ID oder ein Dictionary-Hash sinnvoll, auch wenn stabile IDs verwendet werden. Sie ermöglicht:

- Erkennung falscher Dictionaries,
- Nachvollziehbarkeit von Feldaufzeichnungen,
- Diagnose bei inkompatiblen Änderungen,
- Zuordnung zu Git-Commit und Buildkonfiguration.

---

## 13. Nicht empfohlene Ansätze

Die folgenden Möglichkeiten sind denkbar, lösen das Gesamtproblem aber nicht überzeugend.

### 13.1 Nur hostseitige Umabbildung von TreeLog-IDs

Das Target sendet weiterhin buildabhängige Linker-IDs. Der Host benötigt für jeden Build eine eigene Mappingdatei. Der wesentliche TreeLog-Nachteil bleibt bestehen.

### 13.2 Reiner Hash aus Datei, Zeile und Formatstring

Ein auf 14 Bit verkürzter Hash hat bei größeren Projekten relevante Kollisionswahrscheinlichkeit. Eine Kollisionsauflösung benötigt wiederum eine persistente Registry und eine Bindungsmethode.

### 13.3 Runtime-Suche nach Datei, Zeile oder Formatstring

Strings und Suchstrukturen müssten im Target verbleiben. Flashbedarf und Laufzeitkosten widersprechen den Grundzielen von Trice.

### 13.4 Unveränderte Übernahme des TreeLog-TID-Layouts

Die Bitverschiebung erleichtert TreeLogs Adressmechanik, ist aber für eine stabile Hybrid-ID nicht erforderlich. Sie würde das Trice-Drahtformat ohne entsprechenden Nutzen inkompatibel machen.

### 13.5 Vollständige Abkehr von persistenten ID-Daten

Ohne persistente Zuordnung kann ein Werkzeug keine dauerhaft stabilen, kollisionsfreien und policygesteuerten IDs garantieren. Entfernt werden kann die ID aus dem Sourcecode, nicht die Notwendigkeit einer Registry.

---

## 14. Empfohlene Zielarchitektur

Die Architektur sollte aus zwei klar getrennten Komponenten bestehen.

### 14.1 Trice ID Allocator

Aufgabe:

```text
gefundene Logstelle -> stabile ID
```

Verantwortlichkeiten:

- Scannen von Source- oder Objektmetadaten,
- Wiedererkennung bestehender Logstellen,
- Vergabe neuer IDs,
- Anwendung der ID-Policy,
- Pflege von `til.json`, `li.json` oder einer erweiterten Registry,
- Fehler bei mehrdeutigen Zuordnungen,
- neue ID bei inkompatiblen Formatänderungen.

Der Allocator soll unabhängig davon funktionieren, wie die ID später in den Code gelangt.

### 14.2 Trice ID Binder

Aufgabe:

```text
stabile ID -> Targetcode
```

Mehrere Binder können dasselbe Allocator-Ergebnis verwenden:

1. **Stage-Binder:** erzeugt instrumentierte Build-Kopien.
2. **Header-Binder:** erzeugt per-TU ID-Header.
3. **ELF-Binder:** bindet Symbolwerte oder Relocations.
4. **Map-Binder:** erzeugt eine Runtime-Mapping-Tabelle.

Damit bleibt die Architektur offen und testbar. Ein Projekt kann den Binder entsprechend seiner Toolchain auswählen.

---

## 15. Empfohlener Entwicklungsweg

### Phase 1: Fachliches Datenmodell und Scanner trennen

Zunächst sollte der heutige `trice insert`-Code logisch zerlegt werden:

```text
Scan -> Match -> Allocate -> Persist -> Bind
```

Der bestehende Sourcecode-Binder bleibt funktionsfähig, aber die ID-Zuordnung wird als unabhängige Komponente testbar.

### Phase 2: Build-Kopien-Modus

Ein neuer Befehl könnte beispielsweise heißen:

```text
trice stage
```

Er erzeugt instrumentierte Quellen ausschließlich im Buildverzeichnis.

Ziele:

- kein Schreibzugriff auf Originalquellen,
- kein `clean`,
- identischer Targetcode zum heutigen `insert`,
- Referenz für alle späteren Binder.

### Phase 3: Generierter Header-Binder

Proof of Concept für eine ausgewählte Buildumgebung:

```text
trice headers
```

Prüfen:

- per-source Header-Include,
- `__LINE__`-Zuordnung,
- Diagnose bei zwei Logstellen auf einer Zeile,
- inkrementelle Builds,
- IDE-Diagnosen,
- generierte Quellen.

Dieser Schritt zeigt, ob eine weitgehend portable Lösung ohne Sourcekopie ausreichend praktikabel ist.

### Phase 4: ELF-Binder-Prototyp

Zunächst stark begrenzen:

- GCC,
- ELF32 little-endian,
- ARM Cortex-M,
- kein LTO,
- GNU `ld`,
- normale `.o`-Dateien,
- später `.a`.

Proof-of-Concept-Ziel:

```text
TRICE ohne ID im Source
-> .o mit Site-Metadaten
-> stabile ID aus Registry
-> finaler Maschinencode enthält dieselbe Konstante
   wie heutiges trice insert
```

Zu prüfen sind insbesondere:

- erzeugte Relocationtypen,
- Codegröße und Instruktionsfolge,
- `--gc-sections`,
- Debugging,
- Archive,
- nicht allokierte Metadatensektionen,
- reproduzierbare Builds.

### Phase 5: Runtime-Tabelle als Fallback

Falls direkte Symbolbindung auf einer Toolchain nicht zuverlässig möglich ist, kann dieselbe Metadata- und Allocator-Infrastruktur eine `triceIdMap[]` erzeugen.

### Phase 6: Vorkompilierte Bibliotheken und weitere Toolchains

Erst nach einem stabilen GCC/ARM-Prototyp:

- `.a`-Archive,
- RISC-V,
- Clang/LLD,
- LTO,
- weitere Embedded-Compiler,
- optional Compiler-Plugins.

---

## 16. Vergleich der Hauptvarianten

| Eigenschaft | Build-Kopie | ID-Header | Runtime-Map | ELF-/Linker-Binder |
|---|---:|---:|---:|---:|
| Originalsource unverändert | ja | ja | ja | ja |
| Kein `clean` | ja | ja | ja | ja |
| Stabile IDs | ja | ja | ja | ja |
| ID-Policy | ja | ja | ja | ja |
| Bestehendes Drahtformat | ja | ja | ja | ja |
| Zusätzlicher Target-Flash | nein | nein | ja | nein |
| Zusätzlicher Runtimezugriff | nein | nein | ja | nein |
| Compilerunabhängig | weitgehend | weitgehend | teilweise | nein |
| Linkerunabhängig | ja | ja | nein | nein |
| Vorkompilierte `.a` | nein | nein | grundsätzlich ja | grundsätzlich ja |
| Implementierungsrisiko | niedrig | mittel | mittel | hoch |
| Strategischer Nutzen | mittel | hoch | hoch als Fallback | sehr hoch |

---

## 17. Gesamtbewertung

Die Idee, TreeLogs Linkermechanik mit Trices stabilen IDs zu verbinden, ist tragfähig. Entscheidend ist, TreeLogs Linker-ID nicht als endgültige ID zu übernehmen.

Die Linker- oder Scannerposition sollte lediglich ein **temporärer technischer Site-Identifier** sein:

```text
buildlokale Site
       |
       | ID-Allocator
       v
stabile Trice-ID
       |
       | ID-Binder
       v
Targetcode
```

Die beste langfristige Lösung wäre ein ELF-/Linker-Binder, der stabile IDs direkt in den Code bindet. Sie bietet:

- keine numerischen IDs im Source,
- keine `insert`/`clean`-Schritte,
- keine buildspezifische Hostzuordnung,
- normale Trice-Decodierung,
- stabile ID-Policy,
- Unterstützung vorkompilierter Bibliotheken,
- keine zusätzlichen Targetkosten.

Da dieser Weg technisch anspruchsvoll ist, sollte er nicht als einzige Option begonnen werden. Ein Build-Kopien-Modus liefert schnell einen robusten praktischen Nutzen. Ein generierter Header-Binder ist eine interessante portable Zwischenstufe. Die Runtime-Mapping-Tabelle dient als belastbarer linkerbasierter Fallback.

Ein völlig neues Tool wäre dafür vertretbar, sofern die Architektur zunächst unabhängig von Trice implementiert und erprobt werden soll. Inhaltlich sollte es jedoch dieselben Kernkomponenten besitzen:

```text
Site Discovery
Stable ID Registry
ID Policy
ID Allocator
Target Binder
Dictionary Export
```

Bei erfolgreichem Proof of Concept könnte es später als neuer Trice-Buildmodus integriert werden.

---

## 18. Relevante Links

### Trice

- [Trice Repository](https://github.com/rokath/trice)
- [Trice README – ID-Workflow, `til.json`, `li.json`, `insert` und `clean`](https://github.com/rokath/trice/blob/main/README.md)
- [Trice User Manual](https://github.com/rokath/trice/blob/main/docs/TriceUserManual.md)
- [Issue: Remove IDs from target sources](https://github.com/rokath/trice/issues/421)

### TreeLog

- [TreeLog Repository](https://github.com/hefanbo/treelog)
- [TreeLog Developer Manual](https://github.com/hefanbo/treelog/blob/master/docs/DeveloperManual.adoc)
- [Vergleich Trice `main` mit TreeLog-Fork](https://github.com/rokath/trice/compare/main...hefanbo%3Atrice%3Atreelog)
- [TreeLog-Anpassungscommit im Trice-Fork](https://github.com/hefanbo/trice/commit/d6b7fb1)

### Compiler- und Linkermechanismen

- [GCC: vordefinierte Makros, darunter `__COUNTER__`](https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html)
- [GNU ld: Zuweisung von Werten an Symbole](https://sourceware.org/binutils/docs/ld/Assignments.html)
- [GNU ld: Zugriff auf Linkersymbole aus C](https://sourceware.org/binutils/docs/ld/Source-Code-Reference.html)
- [GNU objcopy](https://sourceware.org/binutils/docs/binutils/objcopy.html)
- [Clang LibTooling](https://clang.llvm.org/docs/LibTooling.html)
