# Architekturentscheidung: `trice bind` statt einer ELF-Patch-Lösung

## 1. Zweck dieses Dokuments

Dieses Dokument begründet die Entscheidung, stabile Trice-IDs mit `trice bind` und generierten Sidecar-Headern in den Targetcode einzubringen. Als Alternative wurde eine ELF-basierte Patch- beziehungsweise Link-Lösung untersucht.

Die Entscheidung betrifft ausschließlich den Mechanismus, mit dem eine bereits bestimmte Trice-ID den Compiler beziehungsweise den finalen Targetcode erreicht. Die bestehende dauerhafte ID-Verwaltung mit `til.json` und `li.json` bleibt unverändert.

## 2. Anforderungen

Der Bindungsmechanismus soll:

- stabile IDs aus `til.json` und `li.json` verwenden,
- keine numerischen IDs in Trice-Aufrufen des Users erfordern,
- die Originalquellen direkt kompilieren,
- keinen zusätzlichen Runtime-Lookup im Target benötigen,
- das bestehende Trice-Drahtformat unverändert lassen,
- historische Logs ohne das zum Build gehörende ELF decodierbar halten,
- mit unterschiedlichen C- und C++-Toolchains verwendbar sein,
- inkrementelle Builds nicht unnötig vergrößern,
- verständliche und reproduzierbare Buildartefakte erzeugen.

## 3. MVP von `trice bind`

### 3.1 Grundprinzip

Der User schreibt eine ID-lose Trice-Logstelle:

```c
trice("msg:module initialized\n");
```

`trice bind` scannt die nicht präprozessierten Quelldateien, ordnet jeder unterstützten Logstelle eine stabile ID aus `til.json` und `li.json` zu und erzeugt pro Datei einen Sidecar-Header.

Für eine Quelldatei `module.c` kann die dauerhaft im Usercode gespeicherte Include-Zeile beispielsweise lauten:

```c
#include "trice_module_c_F73A915E9C4021B8.h" // trice-bind
```

`F73A915E9C4021B8` ist ein einmalig erzeugter zufälliger 64-Bit-Dateischlüssel, dargestellt als mit `F` beginnendes Präprozessortoken. Er identifiziert die Datei unabhängig von ihrem Pfad und unterscheidet auch gleichnamige Dateien sicher voneinander. Der Dateischlüssel ist keine Trice-ID, wird nicht zum Target übertragen und belegt dort keinen Speicher.

Der generierte Header enthält beispielsweise:

```c
#undef TRICE_FILE_KEY
#define TRICE_FILE_KEY F73A915E9C4021B8

#define TRICE_ID_F73A915E9C4021B8_L9 12345u // trice("msg:module initialized\n")
```

Die Trice-Makros kombinieren den aktuellen `TRICE_FILE_KEY` mit dem standardisierten Präprozessormakro `__LINE__`. Der Compiler sieht dadurch letztlich eine gewöhnliche ganzzahlige Konstante.

### 3.2 Warum ein dateilokales Include erforderlich ist

Alle in einer Translation Unit verarbeiteten Sidecars teilen denselben Makronamensraum. Der 64-Bit-Dateischlüssel verhindert Kollisionen zwischen ihren ID-Definitionen. Zusätzlich muss für jeden direkten Trice-Aufruf bekannt sein, zu welcher Datei dessen Zeilennummer gehört.

Eine zentrale Einbindung aller Sidecars, beispielsweise in `triceConfig.h`, stellt zwar alle ID-Makros bereit, wählt aber nicht den zu einer konkreten Logstelle gehörenden Dateischlüssel aus. Der C-Präprozessor kann aus dem Stringliteral `__FILE__` keinen passenden Makronamen ableiten.

Deshalb setzt der Sidecar jeder Datei unmittelbar vor deren eigenen Trice-Logstellen den aktuellen `TRICE_FILE_KEY`. Das dateilokale Include ist nicht nur ein Ablageort für Definitionen, sondern Teil der eindeutigen Auswahl von `Datei + Zeile`.

### 3.3 Sidecar-Verzeichnis und Dateinamen

Alle Sidecars können in einem einzigen Buildverzeichnis liegen, beispielsweise:

```text
build/triceIDs/
```

Der Build benötigt dafür nur einen zusätzlichen Include-Pfad. Der Basisname im Sidecar-Namen verbessert die Lesbarkeit; die Eindeutigkeit liefert der Dateischlüssel:

```text
trice_module_c_F73A915E9C4021B8.h
trice_module_c_F88217D4AC101E62.h
trice_module_h_F1111111111111111.h
```

Eine Abbildung der vollständigen Source-Verzeichnisstruktur unterhalb von `build/` ist nicht erforderlich.

### 3.4 Persistenz und Prüfung des Dateischlüssels

Der Dateischlüssel wird in der versionsverwalteten Include-Zeile der Userdatei gespeichert. Dadurch bleibt er erhalten, wenn das Buildverzeichnis gelöscht oder die Datei verschoben wird.

64 Bit sind hierfür ausreichend. Zusätzlich prüft `trice bind`, dass innerhalb eines Projekts kein Schlüssel mehreren unterschiedlichen Dateien zugeordnet ist. Dadurch werden sowohl eine extrem unwahrscheinliche Zufallskollision als auch ein durch Kopieren einer Quelldatei duplizierter Schlüssel eindeutig erkannt.

### 3.5 Einmaliges Einfügen des Includes

Fehlt der Sidecar-Include, fügt `trice bind` ihn einmalig anhand einer Heuristik an einer wahrscheinlich geeigneten Stelle ein. Bevorzugt wird eine Position:

- nach den für die Datei wirksamen normalen Includes,
- vor der ersten direkten Trice-Logstelle der Datei,
- innerhalb eines vorhandenen Include-Guards bei Headerdateien.

Die Position lässt sich ohne vollständige Auswertung aller Präprozessorbedingungen nicht in jedem C- oder C++-Programm sicher bestimmen. Bedingte Includes können eine eindeutige automatische Platzierung verhindern. Daher gilt:

- Ein bereits vorhandener korrekter Include wird nicht verschoben.
- Eine automatisch eingefügte Zeile ist klar mit `// trice-bind` markiert.
- `trice bind` prüft die strukturelle Plausibilität.
- Bei unsicherer oder widersprüchlicher Struktur erhält der User eine konkrete Diagnose und verschiebt die markierte Zeile bei Bedarf.

Die einmalige Include-Ergänzung ist keine wiederkehrende Instrumentierung. Im normalen Build werden Userquellen nicht verändert.

### 3.6 Headerdateien und `static inline`

Headerdateien mit direkten Trice-Aufrufen erhalten einen eigenen Dateischlüssel und Sidecar. Das gilt auch für Trice-Aufrufe innerhalb von `static inline`-Funktionen.

Beispiel:

```c
#ifndef MODULE_H
#define MODULE_H

#include "dependency.h"
#include "trice_module_h_F1111111111111111.h" // trice-bind

static inline void moduleCheck(int value)
{
    trice("msg:value=%d\n", value);
}

#endif
```

Nach der Verarbeitung dieses Headers setzt der Sidecar der einbindenden `.c`-Datei deren eigenen `TRICE_FILE_KEY`. Dadurch bleiben Logstellen aus Header und Quelldatei kollisionsfrei.

### 3.7 MVP-Einschränkungen

Das MVP unterstützt direkte Trice-Aufrufe in `.c`-, `.cc`-, `.cpp`- und Headerdateien sowie in normalen und `static inline`-Funktionen.

Nicht unterstützt werden zunächst:

- mehrere Trice-Aufrufe in derselben physischen Sourcezeile,
- Trice-Aufrufe innerhalb einer Präprozessormakrodefinition, beispielsweise:

```c
#define LOG_ERROR(x) trice("error=%d\n", x)
```

Bei einem Trice-Aufruf in einer Makrodefinition werden `__LINE__` und der aktuelle Dateischlüssel erst bei der späteren Makroexpansion ausgewertet. Damit beschreiben sie die Aufrufstelle und nicht zuverlässig die Definitionsstelle. Eine einfache Bindung über `Datei + Zeile` ist dafür nicht ausreichend.

`trice bind` meldet solche Konstruktionen im MVP als Fehler. Bestehende Projekte, die darauf angewiesen sind, verwenden weiterhin `trice insert`.

## 4. Untersuchte ELF-Patch-Lösung

Bei einer ELF-basierten Lösung würden Trice-Makros während der Compilation zusätzliche Metadaten und bindbare ID-Platzhalter in Objektdateien erzeugen. Ein späteres Werkzeug müsste diese Informationen auswerten und die endgültigen IDs durch Relocations, zusätzliche Linkobjekte oder direktes Patchen des Objekt- beziehungsweise Imagecodes einsetzen.

Eine solche Lösung benötigt mindestens:

- ein definiertes Metadatenformat in Objektsektionen,
- pro Logstelle einen eindeutig identifizierbaren Platzhalter,
- Unterstützung der jeweiligen ELF- und Relocation-Varianten,
- Kenntnis von Zielarchitektur und ABI,
- abgestimmtes Verhalten mit Linker, Section Garbage Collection und LTO,
- Behandlung von Objektarchiven und nur teilweise übernommenen Archivmitgliedern,
- separate Lösungen für Toolchains, die ELF nicht oder anders verwenden.

Auch eine ELF-Lösung benötigt vorbereitete Trice-Makros beziehungsweise vorbereitete Libraries. Aus einer beliebigen bereits kompilierten `.a`-Datei lassen sich weder die vollständigen Trice-Metadaten noch sicher patchbare ID-Stellen rekonstruieren.

## 5. Vergleich der beiden Ansätze

| Kriterium | `trice bind` MVP | ELF-Patch-Lösung |
|---|---|---|
| Numerische IDs im Trice-Aufruf | nein | nein |
| Originalquellen werden kompiliert | ja | ja |
| Stabile IDs im Targetcode | direkt als C-Konstanten | durch zusätzlichen Patch-/Linkschritt |
| Runtime-Lookup im Target | nein | nicht erforderlich, aber designspezifisch |
| Änderung des Drahtformats | nein | nein, falls entsprechend implementiert |
| Buildspezifisches ELF zur Logdecodierung | nein | nur vermeidbar, wenn endgültige stabile IDs sauber gebunden werden |
| Abhängigkeit von ELF und Relocations | nein | ja |
| Abhängigkeit von Zielarchitektur und ABI | nein | ja |
| Abhängigkeit von Linker und LTO-Verhalten | nein | ja |
| Verwendete Sprachmechanismen | standardisierte C/C++-Präprozessorregeln, `__LINE__`, `##` | Compiler-, Objektformat- und Linkermechanismen |
| Verständlichkeit der Zwischenartefakte | einfache generierte Header | Objektsektionen, Symbole und Relocations |
| Inkrementeller Build | betroffene Translation Unit über ihren Sidecar | abhängig von Patch- und Linkablauf |
| Zusätzlicher Include im Usercode | ja | nein |

Für das normale Sourceprojekt besteht der verbleibende allgemeine Vorteil der ELF-Lösung damit im Wesentlichen darin, dass kein dateilokaler Sidecar-Include erforderlich wäre. Dieser Komfortgewinn steht einer wesentlich größeren Toolchain- und Implementierungskomplexität gegenüber.

## 6. Prüfung der scheinbaren ELF-Vorteile

### 6.1 Vorkompilierte statische Libraries

Eine vorbereitete `.a`-Library kann später unterstützt werden, ohne den normalen Sidecar-Mechanismus zu ersetzen. Die Library müsste beim eigenen Build pro Trice-Stelle Metadaten und einen bindbaren ID-Platzhalter erzeugen. `trice bind` könnte diese Informationen beim Produkt-Build lesen, freie oder neue IDs bestimmen, `til.json` und `li.json` ergänzen und ein zusätzliches Bindeartefakt für den finalen Link erzeugen.

Der Gewinn wäre die nachträgliche Einordnung vorbereiteter Libraries in den stabilen ID-Raum des Endprodukts.

Diese Funktion kann ELF intern verwenden, ist aber eine additive Library-Erweiterung. Sie erfordert keine ELF-basierte Behandlung normaler Userquellen und ist deshalb kein eigenständiger Vorteil einer allgemeinen ELF-Patch-Architektur.

### 6.2 Erfassung inaktiver Logstellen

Das MVP scannt die nicht präprozessierten Quellen. Dadurch werden auch Logstellen in aktuell inaktiven `#if`-Zweigen erfasst. Das ist für stabile IDs erwünscht: Eine Logstelle verliert ihre Zuordnung nicht, nur weil eine bestimmte Buildkonfiguration sie vorübergehend deaktiviert.

Eine ELF-Datei enthält dagegen nur Code, der mindestens bis zur Objekt- oder Linkphase gelangt ist. Sie ist daher nicht die geeignete Quelle für die vollständige, konfigurationsunabhängige ID-Inventur.

### 6.3 Aktive Logstellen einer Buildkonfiguration

Falls zusätzlich die in einer konkreten Konfiguration aktiven Logstellen bestimmt werden sollen, kann später der reale Präprozessor mit den Defines und Include-Pfaden dieses Builds ausgeführt werden.

Dafür werden benötigt:

- die tatsächlichen Präprozessoroptionen jeder Translation Unit,
- Include-Pfade und Defines,
- gegebenenfalls eine `compile_commands.json` oder eine vergleichbare Buildbeschreibung,
- eine Zuordnung der präprozessierten Stellen zu den stabilen IDs.

Der Gewinn ist ein Bericht über die aktive Teilmenge. Die ID-Vergabe und das Sidecar-Binding ändern sich dadurch nicht.

### 6.4 Tatsächlich im finalen Image enthaltene Logstellen

Eine aktive Logstelle kann durch Optimierung, LTO, Section Garbage Collection oder Nichtauswahl eines Archivmitglieds aus dem finalen Image verschwinden. Falls eine exakte Image-Inventur benötigt wird, kann später das fertige ELF oder eine Link-Map ausgewertet werden.

Dafür werden benötigt:

- ein identifizierbarer Bezug zwischen finalem Code und Trice-ID,
- eine toolchainspezifische ELF- oder Map-Auswertung,
- Berücksichtigung von LTO und Linkeroptimierungen.

Der Gewinn ist ein exakter Bericht über die im konkreten Image verbleibende Teilmenge. Auch diese Analyse verändert weder die ID-Zuordnung noch das Sidecar-Binding.

Die drei Mengen sind daher klar getrennt:

```text
alle textuell vorhandenen Logstellen
        ⊇ in einer Konfiguration aktive Logstellen
        ⊇ im finalen Image enthaltene Logstellen
```

Nur die erste Menge ist für die stabile ID-Vergabe des MVP erforderlich.

### 6.5 Makroexpansion und Stringerzeugung

Trice setzt statische, direkt erkennbare Formatstrings voraus. Die Erzeugung verschiedener Formatstrings durch Präprozessorverkettung ist nicht vorgesehen. Variable Inhalte werden als Parameter übertragen, beispielsweise mit einer Stringvariante von Trice.

Damit entfällt die Notwendigkeit, einzelne expandierte Formatstringvarianten erst im Objektcode zu unterscheiden. Auch daraus entsteht kein relevanter ELF-Vorteil.

## 7. Zukünftige additive Erweiterungen

Die folgenden Funktionen sind ausdrücklich nicht Teil des MVP. Sie können später ergänzt werden, ohne das Sidecar-Grundmodell zu verändern.

### 7.1 Analyse der aktiven Konfiguration

**Erforderlich:** Aufruf des realen Präprozessors mit den Buildoptionen jeder Translation Unit.

**Gewinn:** Bericht, welche der bereits gebundenen Logstellen in einer konkreten Konfiguration aktiv sind.

### 7.2 Post-Link-Image-Inventur

**Erforderlich:** ELF- oder Link-Map-Auswertung und toolchainspezifische Zuordnung zu Trice-IDs.

**Gewinn:** Exakte Liste der im finalen Image enthaltenen Logstellen.

### 7.3 Vorbereitete `.a`-Libraries

**Erforderlich:** Libraryseitige Metadaten und bindbare ID-Platzhalter sowie ein zusätzliches Bindeartefakt für den finalen Link.

**Gewinn:** Nachträgliche Vergabe stabiler IDs aus dem ID-Raum des Endprodukts, ohne die Library aus ihren Quellen neu zu binden.

### 7.4 Mehrere Trice-Aufrufe pro Sourcezeile

**Erforderlich:** Ein zusätzlicher stabiler Auftretensindex oder ein geeignetes, ausreichend portables Zählerverfahren.

**Gewinn:** Unterstützung eines derzeit unnötigen, aber syntaktisch möglichen Codestils.

### 7.5 Trice-Aufrufe in Makrodefinitionen

**Erforderlich:** Eine festgelegte Semantik für Definitions- oder Aufrufstellen-IDs und ein zusätzlicher Mechanismus, beispielsweise selektives `trice insert`, explizite Wrapperkennungen oder eine Präprozessoranalyse.

**Gewinn:** Migration bestehender Wrappermakros zu einem gemischten Workflow.

Diese Erweiterungen ergänzen das MVP. Keine davon erfordert, normale Userquellen auf eine allgemeine ELF-Patch-Lösung umzustellen.

## 8. Entscheidung

Für normale C- und C++-Quellen wird `trice bind` mit dateilokalen Sidecar-Headern verfolgt. Eine allgemeine ELF-Patch-Lösung wird nicht weiterverfolgt.

Die Entscheidung beruht auf folgenden technischen Punkten:

1. Das MVP bindet stabile IDs mit standardisierten C/C++-Präprozessormitteln als Compile-Time-Konstanten.
2. Es benötigt weder ELF-Kenntnis noch Architektur-, ABI-, Relocation- oder Linkerlogik.
3. `til.json` und `li.json` bleiben die dauerhafte und buildsübergreifende ID-Wahrheit.
4. Historische Logs bleiben ohne das zugehörige ELF decodierbar.
5. Gleichnamige Dateien und Headerlogstellen werden durch stabile 64-Bit-Dateischlüssel eindeutig behandelt.
6. Die Originalquellen werden direkt gebaut; nur ein einmalig eingefügter Sidecar-Include bleibt im Usercode.
7. Die scheinbaren funktionalen Vorteile der ELF-Lösung lassen sich bei Bedarf als additive Analysen oder Spezialerweiterungen realisieren.
8. Diese Erweiterungen verändern den MVP-Bindungsmechanismus nicht.
9. Als allgemeiner ELF-Vorteil verbleibt im Wesentlichen der Wegfall eines dateilokalen Includes. Dieser Vorteil rechtfertigt die zusätzliche Toolchainkomplexität nicht.

Die Architektur bleibt damit einfach: Source-Scan und stabile Datenbanken bestimmen die IDs, generierte Header stellen sie dem standardkonformen Präprozessor bereit, und der vorhandene Compiler erzeugt den Targetcode.
