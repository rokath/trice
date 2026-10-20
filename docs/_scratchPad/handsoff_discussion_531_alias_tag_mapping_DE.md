
# HANDSOFF: GitHub Discussion #531 — Alias-zugeordnete Tags, Context Enrichment und Structured Logging

**Repository:** `rokath/trice`  
**Ausgangspunkt:** https://github.com/rokath/trice/discussions/531  
**Verhaltensbasis dieser Analyse:** `v2.0.0-beta-1` (veröffentlicht 2026-10-17)  
**Status:** Design-/Analyse-Handoff für Codex — offene Designpunkte sind noch **keine** festgelegten Anforderungen.  
**Bevorzugte Entwicklungsrichtung:** Die bestehende Alias-CLI so erweitern, dass einem Alias optional ein Trice-Tag zugeordnet werden kann, zum Beispiel `-alias MyAlarm:wrn`. Die Semantik darf weder aus dem Aliasnamen geraten noch voraussetzen, dass der Quell-Formatstring den Ereignis-Tag selbst enthält.

Erstellt. Das Hands-Off ist bewusst ausführlich gehalten und enthält:
- Discussion #531 als Ausgangspunkt und die tatsächlichen Anforderungen. GitHub
- Abgleich mit v2.0.0-beta-1 und dessen neuen Funktionen wie Bind, Structured Logging und CE. GitHub
- klare Trennung zwischen bereits gelösten Punkten und offenen Lücken,
- die verworfenen File-ID/Sidecar-ID/__FILE__-Packing-Ansätze nur als kurze Historie,
- -alias MyAlarm:wrn als bevorzugten CLI-Weg,
- unabhängige CE-Selektoren wie temperature:,
- Analyse der offenen Frage TIL-Tag-Feld vs. synthetischer Dictionary-Tag vs. Nutzung bestehender -ulabel/untagged-Mechanismen,
- ID-/Historien-/Kompatibilitätsfragen,
- Bind/Insert/Clean/Generate/Logging-Auswirkungen,
- Warnungssemantik bei z. B. err: innerhalb eines Warning-Alias,
- ausführlichen Testplan,
- staged implementation strategy,
- weitgehend fertige englische Manual-Dokumentation,
- einen später möglichen Antwortentwurf für Discussion #531.
Die Datei umfasst rund 1.880 Zeilen.

---

<h2>ToC</h2>

<!-- mdtoc -->

- [1. Kurzfassung](#kurzfassung)
- [2. Anlass: Discussion #531](#anlass-discussion-531)
  - [2.1. Ursprüngliches Nutzungsszenario](#ursprüngliches-nutzungsszenario)
  - [2.2. Wesentliche Anforderungen aus #531](#wesentliche-anforderungen-aus-531)
    - [2.2.1. Datei und Zeile nur dort anzeigen, wo sie sinnvoll sind](#datei-und-zeile-nur-dort-anzeigen-wo-sie-sinnvoll-sind)
    - [2.2.2. Logging-Abstraktion, ohne die eigentliche Aufrufposition zu verlieren](#logging-abstraktion-ohne-die-eigentliche-aufrufposition-zu-verlieren)
    - [2.2.3. Konstante Metadaten sollen nicht zu Laufzeit-Stringverkehr werden](#konstante-metadaten-sollen-nicht-zu-laufzeit-stringverkehr-werden)
    - [2.2.4. Konsistente maschinenlesbare Ausgabe](#konsistente-maschinenlesbare-ausgabe)
    - [2.2.5. Keine Fragmentierung eines logischen Ereignisses über mehrere Trice-Aufrufe](#keine-fragmentierung-eines-logischen-ereignisses-über-mehrere-trice-aufrufe)
    - [2.2.6. Argumentbreite und Runtime-Strings](#argumentbreite-und-runtime-strings)
- [3. Historischer Kontext: alte Antwort zu #531 vs. v2 Beta 1](#historischer-kontext-alte-antwort-zu-531-vs-v2-beta-1)
- [4. Fähigkeiten von v2.0.0-beta-1](#fähigkeiten-von-v200-beta-1)
  - [4.1. Quellposition: für normale Quellstellen bereits gelöst](#quellposition-für-normale-quellstellen-bereits-gelöst)
  - [4.2. Structured Logging löst das Host-Parsing bereits weitgehend](#structured-logging-löst-das-host-parsing-bereits-weitgehend)
  - [4.3. Tags, Gewichte, Levels und Filter sind bereits vorhanden](#tags-gewichte-levels-und-filter-sind-bereits-vorhanden)
  - [4.4. Context Enrichment liefert bereits skalaren Laufzeitkontext](#context-enrichment-liefert-bereits-skalaren-laufzeitkontext)
  - [4.5. Bestehende Alias-Unterstützung ist der richtige Ausgangspunkt](#bestehende-alias-unterstützung-ist-der-richtige-ausgangspunkt)
- [5. Was v2 Beta 1 noch nicht sauber löst](#was-v2-beta-1-noch-nicht-sauber-löst)
  - [5.1. Ein benutzerdefinierter Alias kann seinen Tag nicht unabhängig vom Formatstring definieren](#ein-benutzerdefinierter-alias-kann-seinen-tag-nicht-unabhängig-vom-formatstring-definieren)
  - [5.2. Eine normale Hilfsfunktion ist kein gleichwertiger Ersatz, wenn die Caller-Location wichtig ist](#eine-normale-hilfsfunktion-ist-kein-gleichwertiger-ersatz-wenn-die-caller-location-wichtig-ist)
  - [5.3. MODULE-/PROCESS-Automatisierung bleibt anwendungsspezifisch](#module-process-automatisierung-bleibt-anwendungsspezifisch)
- [6. Bevorzugte CLI-Richtung](#bevorzugte-cli-richtung)
  - [6.1. Bestehende Option erweitern, keine Namensheuristik](#bestehende-option-erweitern-keine-namensheuristik)
  - [6.2. Bedeutung von Name:Tag](#bedeutung-von-nametag)
  - [6.3. Keine Semantik aus Aliasnamen ableiten](#keine-semantik-aus-aliasnamen-ableiten)
  - [6.4. Formatstrings bleiben Eigentum des Quellcodes und werden durch Alias-Tag-Zuordnung nicht umgeschrieben](#formatstrings-bleiben-eigentum-des-quellcodes-und-werden-durch-alias-tag-zuordnung-nicht-umgeschrieben)
- [7. Alias-Tag und CE-Selektor sind getrennte Dimensionen](#alias-tag-und-ce-selektor-sind-getrennte-dimensionen)
  - [7.1. Vorhandene CE-Selektorregeln wiederverwenden](#vorhandene-ce-selektorregeln-wiederverwenden)
  - [7.2. Offene Entscheidung: Soll der Alias-Tag selbst CE-Regeln auswählen?](#offene-entscheidung-soll-der-alias-tag-selbst-ce-regeln-auswählen)
- [8. Was passiert bei einem tag-artigen Präfix im Alias-Format?](#was-passiert-bei-einem-tag-artigen-präfix-im-alias-format)
  - [8.1. Zu analysierendes Warnverhalten](#zu-analysierendes-warnverhalten)
- [9. Zentrale offene Architekturfrage: Wo lebt der zugeordnete Tag?](#zentrale-offene-architekturfrage-wo-lebt-der-zugeordnete-tag)
  - [9.1. Warum Persistenz nötig ist](#warum-persistenz-nötig-ist)
  - [9.2. Variante A: optionales Tag-Metadatum in til.json](#variante-a-optionales-tag-metadatum-in-tiljson)
    - [9.2.1. Vorteile](#vorteile)
    - [9.2.2. Nachteile / notwendige Arbeit](#nachteile--notwendige-arbeit)
    - [9.2.3. Kritische ID-Historienregel](#kritische-id-historienregel)
  - [9.3. Variante B: synthetischen Tag nur dem effektiven Dictionary-Format voranstellen](#variante-b-synthetischen-tag-nur-dem-effektiven-dictionary-format-voranstellen)
    - [9.3.1. Vorteile](#vorteile-1)
    - [9.3.2. Nachteile / Risiken](#nachteile--risiken)
  - [9.4. Variante C: dieselbe Alias-/Tag-CLI auch beim Logging verlangen](#variante-c-dieselbe-alias-tag-cli-auch-beim-logging-verlangen)
    - [9.4.1. Vorteil](#vorteil)
    - [9.4.2. Nachteile](#nachteile)
  - [9.5. Variante D: -ulabel / untagged ohne explizite neue Metadaten wiederverwenden](#variante-d--ulabel--untagged-ohne-explizite-neue-metadaten-wiederverwenden)
  - [9.6. Variante E: Tag aus ID-Bereichen ableiten](#variante-e-tag-aus-id-bereichen-ableiten)
- [10. Empfohlenes internes Modell zur Prüfung](#empfohlenes-internes-modell-zur-prüfung)
  - [10.1. Doppelte bzw. widersprüchliche CLI-Definitionen](#doppelte-bzw-widersprüchliche-cli-definitionen)
  - [10.2. Kanonisierung des zugeordneten Tags](#kanonisierung-des-zugeordneten-tags)
- [11. Reihenfolge relativ zu CE und ID-Vergabe](#reihenfolge-relativ-zu-ce-und-id-vergabe)
  - [11.1. Bind](#bind)
  - [11.2. Insert](#insert)
  - [11.3. Clean](#clean)
  - [11.4. Generate / generierte Lookup-Daten](#generate--generierte-lookup-daten)
- [12. Logging-Seite: einheitliches Verhalten erzwingen](#logging-seite-einheitliches-verhalten-erzwingen)
  - [12.1. Rückwärtskompatibilität für untagged](#rückwärtskompatibilität-für-untagged)
- [13. Location Information: kein neuer File-ID-Mechanismus](#location-information-kein-neuer-file-id-mechanismus)
  - [13.1. Separates files.json](#separates-filesjson)
  - [13.2. Bind-Sidecar-File-Keys übertragen](#bind-sidecar-file-keys-übertragen)
  - [13.3. __FILE__ in mehrere Integerwerte packen](#__file__-in-mehrere-integerwerte-packen)
- [14. Caller-Location: Alias vs. Helper-Funktion vs. Wrapper](#caller-location-alias-vs-helper-funktion-vs-wrapper)
  - [14.1. Direkter Trice-Aufruf oder registrierter Alias](#direkter-trice-aufruf-oder-registrierter-alias)
  - [14.2. Normale Hilfsfunktion](#normale-hilfsfunktion)
  - [14.3. Allgemeines Logging-Wrapper-Makro mit inneren Trice-Aufrufen](#allgemeines-logging-wrapper-makro-mit-inneren-trice-aufrufen)
- [15. Laufzeitkontext aus #531](#laufzeitkontext-aus-531)
  - [15.1. Task-Kontext](#task-kontext)
  - [15.2. Timestamp](#timestamp)
  - [15.3. MODULE / PROCESS](#module--process)
- [16. Runtime-Strings: ausdrückliches Nicht-Ziel](#runtime-strings-ausdrückliches-nicht-ziel)
- [17. Mögliche Verhaltensmatrix für -alias Name:Tag](#mögliche-verhaltensmatrix-für--alias-nametag)
- [18. Kompatibilitäts- und Migrationsanforderungen](#kompatibilitäts--und-migrationsanforderungen)
  - [18.1. Bestehende CLI muss unverändert funktionieren](#bestehende-cli-muss-unverändert-funktionieren)
  - [18.2. Alte TIL -> neues Tool](#alte-til---neues-tool)
  - [18.3. Neue TIL -> altes Tool](#neue-til---altes-tool)
  - [18.4. Historische kumulative TIL](#historische-kumulative-til)
  - [18.5. ID-Änderungen](#id-änderungen)
- [19. Wechselwirkung mit -ulabel](#wechselwirkung-mit--ulabel)
- [20. Wechselwirkung mit tag-spezifischen ID-Ranges / Target Routing](#wechselwirkung-mit-tag-spezifischen-id-ranges--target-routing)
- [21. Präsentation und -color](#präsentation-und--color)
- [22. Anforderungen an Structured Output](#anforderungen-an-structured-output)
- [23. Selektive Datei-/Zeilenanzeige bei Errors: für #531 wahrscheinlich keine Implementierung nötig](#selektive-datei-zeilenanzeige-bei-errors-für-531-wahrscheinlich-keine-implementierung-nötig)
- [24. Testplan](#testplan)
  - [24.1. CLI-Parsing](#cli-parsing)
  - [24.2. Kanonische Äquivalenz](#kanonische-äquivalenz)
  - [24.3. Bind mit direktem Alias](#bind-mit-direktem-alias)
  - [24.4. Insert / Clean](#insert--clean)
  - [24.5. Alias-Tag + CE-Quellselektor](#alias-tag--ce-quellselektor)
  - [24.6. Alias-Tag + CE über zugeordneten Tag](#alias-tag--ce-über-zugeordneten-tag)
  - [24.7. Kollidierendes tag-artiges Quellpräfix](#kollidierendes-tag-artiges-quellpräfix)
  - [24.8. Historisches ID-Verhalten](#historisches-id-verhalten)
  - [24.9. Structured Output](#structured-output)
  - [24.10. Textausgabe und Farben](#textausgabe-und-farben)
  - [24.11. Filtering und Statistik](#filtering-und-statistik)
  - [24.12. -ulabel](#-ulabel)
  - [24.13. ID Range / Routing](#id-range--routing)
  - [24.14. Rückwärtskompatibilität](#rückwärtskompatibilität)
  - [24.15. -salias](#-salias)
- [25. Voraussichtlich relevante Implementierungsbereiche](#voraussichtlich-relevante-implementierungsbereiche)
- [26. Designfragen, die Codex vor größerer Implementierung explizit beantworten muss](#designfragen-die-codex-vor-größerer-implementierung-explizit-beantworten-muss)
- [27. Empfohlene stufenweise Implementierungsstrategie](#empfohlene-stufenweise-implementierungsstrategie)
  - [27.1. Stufe 1 — Design-Proof / Tests](#stufe-1--design-proof--tests)
  - [27.2. Stufe 2 — Alias-CLI-Parser und interne Repräsentation](#stufe-2--alias-cli-parser-und-interne-repräsentation)
  - [27.3. Stufe 3 — Bind/Insert/Clean-Integration](#stufe-3--bindinsertclean-integration)
  - [27.4. Stufe 4 — Persistenz und Logging](#stufe-4--persistenz-und-logging)
  - [27.5. Stufe 5 — Diagnostik](#stufe-5--diagnostik)
  - [27.6. Stufe 6 — Beispiele und Doku](#stufe-6--beispiele-und-doku)
- [28. Potenzielle User-/Reference-Manual-Dokumentation — deutscher Entwurf](#potenzielle-user-reference-manual-dokumentation--deutscher-entwurf)
  - [28.1. Tagged Trice Aliases](#tagged-trice-aliases)
    - [28.1.1. Getaggte Aliase](#getaggte-aliase)
  - [28.2. Tags und Formatstring-Selektoren sind unabhängig](#tags-und-formatstring-selektoren-sind-unabhängig)
  - [28.3. Quellposition](#quellposition)
  - [28.4. Tag-artiger Text innerhalb eines getaggten Alias](#tag-artiger-text-innerhalb-eines-getaggten-alias)
  - [28.5. Custom Tags](#custom-tags)
  - [28.6. Laufzeitkontext](#laufzeitkontext)
  - [28.7. Runtime-Strings sind keine Context-Enrichment-Werte](#runtime-strings-sind-keine-context-enrichment-werte)
- [29. Mögliche spätere Antwort auf Discussion #531 — Entwurf](#mögliche-spätere-antwort-auf-discussion-531--entwurf)
- [30. Verworfene bzw. zurückgestellte Lösungsansätze](#verworfene-bzw-zurückgestellte-lösungsansätze)
  - [30.1. Separate stabile Source-File-IDs / files.json](#separate-stabile-source-file-ids--filesjson)
  - [30.2. Bind-Sidecar-File-Keys übertragen](#bind-sidecar-file-keys-übertragen-1)
  - [30.3. Dateinamen-Zeichen in uint64_t-CE-Werte packen](#dateinamen-zeichen-in-uint64_t-ce-werte-packen)
  - [30.4. Severity automatisch aus Aliasnamen ableiten](#severity-automatisch-aus-aliasnamen-ableiten)
  - [30.5. Allgemeine log_error()-Hilfsfunktion als Hauptlösung](#allgemeine-log_error-hilfsfunktion-als-hauptlösung)
  - [30.6. Mehrere Trice-Aufrufe für Kontext + Meldung](#mehrere-trice-aufrufe-für-kontext--meldung)
  - [30.7. Runtime-String + beliebige skalare CE jetzt lösen](#runtime-string--beliebige-skalare-ce-jetzt-lösen)
- [31. Akzeptanzkriterien der finalen Funktion](#akzeptanzkriterien-der-finalen-funktion)
- [32. Quellen / geprüfte Basis](#quellen--geprüfte-basis)
- [33. Abschließende Arbeitsanweisung an Codex](#abschließende-arbeitsanweisung-an-codex)
- [34. Designnachtrag: Tag-Transport und kompakte Location-Historie](#designnachtrag-tag-transport-und-kompakte-location-historie)
  - [34.1. Empfehlung und gewünschte Zuständigkeiten](#empfehlung-und-gewünschte-zuständigkeiten)
  - [34.2. Warum die Tag-Information den Build überleben muss](#warum-die-tag-information-den-build-überleben-muss)
  - [34.3. Vergleich der Transportwege](#vergleich-der-transportwege)
  - [34.4. Vorschlag für eine erweiterbare TIL](#vorschlag-für-eine-erweiterbare-til)
  - [34.5. Custom-Tags, Gewichte und ein verständlicher Ablauf](#custom-tags-gewichte-und-ein-verständlicher-ablauf)
  - [34.6. Historische Identität und gemeinsame Auswertung](#historische-identität-und-gemeinsame-auswertung)
  - [34.7. Kompatibilität: Lesen, Bedeutung und Schreiben unterscheiden](#kompatibilität-lesen-bedeutung-und-schreiben-unterscheiden)
  - [34.8. LI sammelt bereits IDs, aber keine vollständige Positionshistorie](#li-sammelt-bereits-ids-aber-keine-vollständige-positionshistorie)
  - [34.9. Platzsparende LI: kleiner Schritt und spätere Historie](#platzsparende-li-kleiner-schritt-und-spätere-historie)
  - [34.10. Konkreter Entscheidungsstand für die Weiterarbeit](#konkreter-entscheidungsstand-für-die-weiterarbeit)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

## 1. <a id="kurzfassung"></a>Kurzfassung

Discussion [#531](https://github.com/rokath/trice/discussions/531) fragt, wie ein bestehender strukturierter Embedded-Logging-Stil effizient mit Trice abgebildet werden kann. Der ursprüngliche Nutzer möchte kurze Aufrufe wie:

```c
log_error("My Error %2X", value);
```

und dabei ohne wiederholte Übertragung konstanter Texte unter anderem erhalten:

- Log-Schweregrad / Level,
- Quelldatei und Zeile,
- Modul-/Prozesskontext,
- FreeRTOS-Task-Kontext,
- Zeitstempel,
- zuverlässige maschinenlesbare Ausgabe,
- korrektes Verhalten bei Präemption,
- kompakte Binärdarstellung.

Als #531 im Mai 2025 eröffnet wurde, gab es den heutigen v2-Workflow mit Structured Logging und Context Enrichment noch nicht. `v2.0.0-beta-1` löst inzwischen einen großen Teil der damaligen Anforderungen:

- `bind` hält IDs aus dem Benutzerquelltext heraus und weist Quellstellen IDs über generierte Sidecars zu.
- `li.json` bildet eine Trice-ID bereits auf Datei und Zeile ab. Für normale Quellpositionsausgabe ist **keine zusätzliche Datei-ID im Target-Datenstrom nötig**.
- Structured Logging exportiert Ereignisse als Text, JSON/NDJSON oder Key/Value inklusive Tag, abgeleitetem Level, Meldung, optionaler Quellposition, Zeitstempeln, ID und benannten Nutzfeldern.
- Tags, Gewichte, `-pick`, `-ban`, `-logLevel`, `-ulabel` und `untagged` bilden bereits ein konsistentes Severity-/Kategorie-Modell.
- Context Enrichment (`-ce`) kann selektiv skalaren Laufzeitkontext, z. B. einen FreeRTOS-Task-Handle, **im selben Ereignis** ergänzen, ohne jede Log-Stelle manuell zu erweitern.
- Die bestehende Alias-Unterstützung (`-alias` / `-salias`) erlaubt projektspezifische Makronamen als Trice-Aufrufe zu behandeln und dabei die eigentliche Aufrufstelle für ID/LI zu erhalten.

Die für #531 noch relevante Lücke ist daher **nicht primär Datei-/Zeilenübertragung**. Die wichtige Ergonomielücke ist:

> Ein projektspezifischer Alias wie `MyAlarm(...)` sollte per CLI explizit einem Trice-Tag, z. B. Warning, zugeordnet werden können, auch wenn der Formatstring selbst nicht mit `wrn:` beginnt.

Bevorzugte CLI-Richtung:

```sh
trice bind -alias MyAlarm:wrn ...
```

Quellcode:

```c
MyAlarm("Temperature too high: %d degrees", value);
```

Der Aliasname selbst hat **keine eingebaute Bedeutung**. `MyAlarm`, `LogX`, `Banana` usw. sind gleichwertige Namen, bis die CLI explizit einen Tag zuordnet. Es darf keine Heuristik geben, die in Aliasnamen nach `error`, `warn`, `alarm`, `fatal`, `debug` usw. sucht.

Ein Quellpräfix wie:

```c
MyAlarm("temperature:Temperature too high: %d degrees", value);
```

kann unabhängig davon als CE-Selektor dienen:

```sh
-ce 'temperature:", limit={limit:%d}", temperatureLimit'
```

**Alias-Tag und CE-Selektor sind zwei getrennte Dimensionen.**

Die zentrale offene Architekturfrage lautet, wie der zugeordnete Alias-Tag für spätere Log-Auswertung dauerhaft erhalten bleibt, wenn der Quell-Formatstring bzgl. Tag-Klassifikation unverändert bleiben soll. Insbesondere ist **noch nicht entschieden**, ob `til.json` ein zusätzliches `Tag`-Feld bekommen muss.

Codex soll mindestens diese Varianten analysieren:

1. explizite Tag-Metadaten in TIL (`Tag` oder äquivalent),
2. ein synthetisches/virtuelles Tag-Präfix nur im effektiven Dictionary-Schema, während der Quelltext unverändert bleibt,
3. Wiederverwendung bzw. Erweiterung vorhandener Mechanismen rund um `-ulabel`, `untagged` und die Tag-Klassifikation,
4. eine andere, einfachere Lösung, sofern sie historische Dekodierung und bestehende Trice-Semantik sauber erhält.

Das Design muss mit Bind/Insert/Clean, kumulativer TIL-Historie, ID-Stabilitätsregeln, CE, tag-spezifischen ID-Bereichen, Structured Logging, Filtern, Statistik sowie alten und neuen Dictionaries soweit sinnvoll kompatibel bleiben.

**Runtime-Strings über CE sind ausdrücklich nicht Teil dieser Aufgabe.** Für diese Alias-Tag-Erweiterung darf festgelegt werden, dass der Workflow auf normalen skalaren Trice-Familien basiert. Arbiträre Kombinationen aus Laufzeitstrings und zusätzlichen skalaren CE-Werten müssen hier nicht gelöst werden.

---

## 2. <a id="anlass-discussion-531"></a>Anlass: Discussion #531

### 2.1. <a id="ursprüngliches-nutzungsszenario"></a>Ursprüngliches Nutzungsszenario

Discussion #531 trägt den Titel **„How to use trice to cover these example use cases?“** und wurde von `jnz86` am 2025-05-20 eröffnet:

https://github.com/rokath/trice/discussions/531

Der Nutzer hatte bereits ein System, das Log-Meldungen über RTT an ein Telnet-orientiertes Hostprogramm sendete. Die Texte enthielten maschinenlesbare Markerfelder, zum Beispiel `<...>`, vor dem eigentlichen Nachrichtentext. Der Logger abstrahierte sich wiederholenden Kontext und erlaubte sinngemäß Aufrufe wie:

```c
log_error("My Error %2X", value);
```

Der Logger ergänzte automatisch unter anderem:

- Modul,
- Prozess,
- Task-Kontext unter FreeRTOS,
- Zeitstempel,
- Schweregrad,
- Quellinformation.

Aus der Diskussion ergeben sich mehrere konkrete Anforderungen.

### 2.2. <a id="wesentliche-anforderungen-aus-531"></a>Wesentliche Anforderungen aus #531

#### 2.2.1. <a id="datei-und-zeile-nur-dort-anzeigen-wo-sie-sinnvoll-sind"></a>Datei und Zeile nur dort anzeigen, wo sie sinnvoll sind

Der Nutzer fragte, ob Dateiname und Zeilennummer nur bei Error-Meldungen einbezogen werden können.

Die wichtige v2-Erkenntnis lautet: Datei und Zeile müssen normalerweise **nicht** als Target-Laufzeitwerte übertragen werden. Eine normale Trice-ID identifiziert bereits eine Quellstelle; das dazu passende `li.json` enthält Datei und Zeile. Der Host kann diese Information je nach Tag/Level anzeigen, ignorieren oder strukturiert weitergeben.

#### 2.2.2. <a id="logging-abstraktion-ohne-die-eigentliche-aufrufposition-zu-verlieren"></a>Logging-Abstraktion, ohne die eigentliche Aufrufposition zu verlieren

Der Nutzer befürchtete zu Recht, dass ein Wrapper bzw. eine Hilfsfunktion dazu führt, dass die Location auf die Definition des Helpers statt auf die eigentliche Anwendungstelle zeigt.

Das ist bei einer gemeinsamen Trice-Stelle in einer normalen Hilfsfunktion und bei bestimmten Wrapper-Makro-Mustern tatsächlich der Fall. Ein **registrierter Alias an der Aufrufstelle** ist deshalb die passendere Lösung: Jede Alias-Invocation kann als eigene Trice-Quellstelle mit eigener ID und eigener LI behandelt werden.

Wichtige Unterscheidung:

```c
// Gemeinsame Funktion: eine innere Trice-Stelle.
void logError(int code) {
    trice32("err:code=%d", code);
}

// Alias-Stil: jede MyAlarm(...)-Invocation ist eine instrumentierte Quellstelle.
MyAlarm("Temperature too high: %d", value);
```

Die erste Variante identifiziert natürlicherweise den Funktionskörper. Die zweite kann die Anwendungs-Aufrufstelle identifizieren, wenn sie über Trices Alias-Instrumentierung verarbeitet wird.

#### 2.2.3. <a id="konstante-metadaten-sollen-nicht-zu-laufzeit-stringverkehr-werden"></a>Konstante Metadaten sollen nicht zu Laufzeit-Stringverkehr werden

Der Nutzer möchte statische Informationen wie MODULE und PROCESS nicht bei jedem Ereignis als String übertragen.

Trice sollte diesem Grundprinzip folgen:

> Compile-Time-Information gehört in Dictionaries/Metadaten; nur tatsächlich variable Laufzeitwerte sollen Target-Payload kosten.

Für die Quellposition setzt `li.json` dieses Prinzip bereits um. Auch Tags sind aktuell statische Format-/Dictionary-Information und kein Laufzeitargument.

Alias-zu-Tag-Mapping löst allerdings nicht automatisch beliebige MODULE-/PROCESS-Metadaten. Dieses Handoff schlägt **kein allgemeines Compile-Time-Metadaten-Framework** vor. Vorhandene Bausteine sind aber:

- Quellpfad aus LI kann im Host auf ein Modul abgebildet werden,
- statischer Text kann bei geeignetem Selektor per CE in das effektive Format aufgenommen werden,
- dynamischer Task-/Prozesskontext kann als kompakter skalarer CE-Wert übertragen werden,
- Target-Timestamps existieren bereits,
- JSON/KV vermeidet das Parsen proprietärer Markerketten.

Diese Aufgabe nicht zu einem allgemeinen Metadaten-System ausweiten, sofern die Alias-Tag-Funktion das nicht zwingend benötigt.

#### 2.2.4. <a id="konsistente-maschinenlesbare-ausgabe"></a>Konsistente maschinenlesbare Ausgabe

Der Nutzer wollte nicht nur schöne Terminalausgabe. Seine Marker wurden von Software verarbeitet.

`v2.0.0-beta-1` bietet Structured Logging:

```sh
trice log ... -logFormat json
```

Das liefert ein JSON-Objekt pro akzeptiertem Ereignis (NDJSON) und trennt Host-Metadaten von Benutzerfeldern. Das beantwortet den ursprünglichen Marker-Parsing-Bedarf wesentlich sauberer als mehrere zusammengesetzte Text-Trices.

#### 2.2.5. <a id="keine-fragmentierung-eines-logischen-ereignisses-über-mehrere-trice-aufrufe"></a>Keine Fragmentierung eines logischen Ereignisses über mehrere Trice-Aufrufe

Der Nutzer erwog mehrere Trice-Aufrufe für Kontext + Meldung und hatte berechtigte Sorgen bezüglich Präemption und Vermischung unter FreeRTOS.

Für normalen skalaren Kontext beseitigt CE den Hauptgrund für dieses Aufsplitten. Bevorzugt wird **ein Ereignis**, das normale Meldungswerte und zusätzlichen Laufzeitkontext gemeinsam trägt.

Deferred Mode macht mehrere getrennte Trice-Aufrufe nicht automatisch zu einem semantisch atomaren Ereignis; das ist eine andere Ebene.

#### 2.2.6. <a id="argumentbreite-und-runtime-strings"></a>Argumentbreite und Runtime-Strings

In #531 wurde auch nach Kombinationen aus Laufzeitstrings und Zahlen gefragt. Trice besitzt weiterhin familienabhängige Einschränkungen. Für diese Aufgabe gilt bewusst:

> **Arbiträre Runtime-String- plus skalare CE-Kombinationen werden nicht gelöst.**

Diese Begrenzung dokumentieren statt das Alias-Tag-Thema mit einer grundlegenden Neugestaltung des Record-Modells zu blockieren.

---

## 3. <a id="historischer-kontext-alte-antwort-zu-531-vs-v2-beta-1"></a>Historischer Kontext: alte Antwort zu #531 vs. v2 Beta 1

Die angenommene Antwort von Juni 2025 untersuchte damals plausible Techniken:

- getrennte Trice-Aufrufe für Modul/Prozess/Kontext/Meldung,
- Critical Sections gegen Interleaving,
- zukünftige Context-Insertion-Ideen,
- Host-seitige Compile-Time-Kontextideen.

Spätere Kommentare verwiesen bereits auf neu entstehende Structured-Logging-Dokumentation.

Seitdem hat sich das Projekt deutlich weiterentwickelt. `v2.0.0-beta-1` wurde am 2026-10-17 veröffentlicht und nennt ausdrücklich:

- Bind,
- Structured Logging,
- Context Enrichment,
- tag-/severity-basiertes Filtern,
- JSON/KV-Ausgabe,
- Quellposition als strukturierte Metadaten,
- Beispiele inklusive FreeRTOS-Task-Kontext.

Daher **nicht** so designen, als wäre der Projektstand von 2025 noch aktuell. Ziel ist, die verbleibende Ergonomielücke auf Basis der v2-Architektur zu schließen, nicht die damaligen Multi-Call-Workarounds wiederzubeleben.

Release-Referenz:

https://github.com/rokath/trice/releases/tag/v2.0.0-beta-1

---

## 4. <a id="fähigkeiten-von-v200-beta-1"></a>Fähigkeiten von v2.0.0-beta-1

### 4.1. <a id="quellposition-für-normale-quellstellen-bereits-gelöst"></a>Quellposition: für normale Quellstellen bereits gelöst

`li.json` ordnet jeder Trice-ID einen kanonischen Quellpfad und eine Zeile zu, konzeptionell etwa:

```json
{
  "1234": {
    "File": "src/motor.c",
    "Line": 42
  }
}
```

Relevante Dokumentation:

- `docs/rm/cli.md` — Location Information
- `docs/rm/id_management.md` — Trice location information file `li.json`

Wichtige Eigenschaften:

- Datei/Zeile ist Host-Metadatum und kostet keine zusätzlichen Target-Payload-Bytes.
- LI muss zum exakten Firmware-Build passen, wenn genaue Quellposition wichtig ist.
- `trice log` / `tlog` können Location Information anzeigen.
- JSON/KV kann `file` und `line` als getrennte Felder ausgeben.
- Die Trice-ID genügt für den Lookup.

**Folgerung:** Für #531 keinen zweiten Quell-Datei-ID-Mechanismus einführen.

### 4.2. <a id="structured-logging-löst-das-host-parsing-bereits-weitgehend"></a>Structured Logging löst das Host-Parsing bereits weitgehend

Relevante Dokumentation:

- `docs/rm/structured_logging.md`

Strukturierte Records enthalten heute:

- `tag`,
- abgeleiteten `level`,
- `message`,
- optional `id`,
- optional `file`, `line`,
- Target-Timestamps und Deltas,
- Host-Timestamp,
- benannte Benutzerfelder.

Das ist bereits ein starker Ersatz für benutzerdefinierte `<marker>`-Ketten vor Text.

Die bestehende Anzeigeanwendung des Nutzers kann entweder:

1. JSON/KV direkt konsumieren oder
2. einen Adapter verwenden, der Trice Structured Output in das alte Markerformat umsetzt.

Ein spezielles `tlog`-Feature „Datei/Zeile nur bei Errors“ ist daher **nicht notwendig**, um #531 nutzbar zu machen. Der Consumer kann diese Felder bei anderen Levels ignorieren. Ein Host-only-Location-Filter kann später unabhängig ergänzt werden, ist aber nicht die primäre Lücke.

### 4.3. <a id="tags-gewichte-levels-und-filter-sind-bereits-vorhanden"></a>Tags, Gewichte, Levels und Filter sind bereits vorhanden

Relevante Dokumentation:

- `docs/rm/tags_color_and_weights.md`

Aktuelle v2-Semantik:

- eingebaute Tag-Gruppen und Aliase,
- Gewichte `0..999`,
- daraus abgeleitete Levels wie WARNING und ERROR,
- `-pick`, `-ban`, `-logLevel`,
- Custom Labels über `-ulabel`,
- `untagged` für Ereignisse ohne erkannten Tag im gespeicherten Format,
- Tag-Statistik,
- tag-spezifische ID-Bereiche.

Wichtiger Architekturpunkt:

> Heute stammt der Tag normalerweise aus einem Präfix am Anfang des gespeicherten Formatstrings.

Genau diese Kopplung muss die Alias-Tag-Funktion möglicherweise lockern.

### 4.4. <a id="context-enrichment-liefert-bereits-skalaren-laufzeitkontext"></a>Context Enrichment liefert bereits skalaren Laufzeitkontext

Relevante Dokumentation:

- `docs/rm/context_enrichment.md`
- `docs/rm/examples.md`

Beispiel:

```sh
-ce 'ctx:", task={task:%p}", osThreadGetId()'
```

Ein ausgewähltes Ereignis erhält den Task-Wert im selben Binärereignis.

Das Beispiel `G0B1_features` zeigt eine gemeinsame Log-Stelle, die aus zwei FreeRTOS-Tasks ausgeführt wird; die Records tragen unterschiedliche Task-Handles.

Das beantwortet eine wesentliche #531-Frage: Task-Kontext braucht keine separate Trice-Meldung und damit keine Textverkettung oder Critical Section, nur um Kontext und Meldung zusammenzuhalten.

Aktuelle Grenzen bleiben zu dokumentieren:

- skalare CE-Werte müssen in das Wertemodell der jeweiligen Trice-Familie passen,
- die maximale skalare Argumentanzahl bleibt begrenzt,
- `bind -ce` unterstützt derzeit direkt und zeilenadressierbare Stellen und lehnt bestimmte komplexe Wrapper-/Rebase-Fälle bewusst ab,
- Runtime-String-/Buffer-Familien erhalten nicht beliebig zusätzliche skalare CE-Argumente.

### 4.5. <a id="bestehende-alias-unterstützung-ist-der-richtige-ausgangspunkt"></a>Bestehende Alias-Unterstützung ist der richtige Ausgangspunkt

Relevante Dokumentation:

- `docs/rm/legacy_user_code.md`
- vorhandene Hilfe zu `-alias` / `-salias`

Aktuelles Verhalten:

```sh
-alias DEBUG_PRINT
-salias DEBUG_PRINT_S
```

registriert projektspezifische Makronamen als Äquivalente zu `trice()` bzw. `triceS()`.

Aktuelle Implementierungsbereiche umfassen unter anderem:

- `internal/id/vars.go` — `TriceAliases`, `TriceSAliases`
- Datei mit `resolveTriceAlias` / `ProcessAliases`
- `internal/id/bindAdvanced.go`
- Alias-Integrationstests in `internal/id/*_test.go`
- CLI-Setup in `internal/args/init.go`

`TriceFmt` enthält aktuell:

```go
type TriceFmt struct {
    Type  string `json:"Type"`
    Strg  string `json:"Strg"`
    Alias string `json:"-"`
}
```

Der Aliasname ist transient/intern und wird nicht nach `til.json` geschrieben.

**Dieses Subsystem soll erweitert werden.** Kein separates, paralleles System „Logging-Funktion mit Level“ erfinden, wenn Alias-Metadaten das sauber lösen können.

---

## 5. <a id="was-v2-beta-1-noch-nicht-sauber-löst"></a>Was v2 Beta 1 noch nicht sauber löst

### 5.1. <a id="ein-benutzerdefinierter-alias-kann-seinen-tag-nicht-unabhängig-vom-formatstring-definieren"></a>Ein benutzerdefinierter Alias kann seinen Tag nicht unabhängig vom Formatstring definieren

Gewünschter Quellcode:

```c
MyAlarm("Temperature too high: %d degrees", value);
```

Gewünschte Build-Konfiguration:

```sh
trice bind -alias MyAlarm:wrn ...
```

Gewünschte Semantik:

- `MyAlarm` wird an seiner Invocation-Stelle wie ein normaler Trice-Alias instrumentiert.
- Der wahre Ereignis-Tag ist Warning.
- Der Standard-Level ist damit WARNING.
- Der Quell-Formatstring muss kein `wrn:` enthalten.
- Der Tag erzeugt keine Target-Laufzeitbytes.
- Die Quellposition bleibt über normale ID/LI-Mechanik die Alias-Aufrufstelle.
- Host-Filter, Statistik, Farben und strukturierter Level müssen denselben zugeordneten Tag sehen.

Das ist die primär vorgeschlagene Erweiterung.

### 5.2. <a id="eine-normale-hilfsfunktion-ist-kein-gleichwertiger-ersatz-wenn-die-caller-location-wichtig-ist"></a>Eine normale Hilfsfunktion ist kein gleichwertiger Ersatz, wenn die Caller-Location wichtig ist

Folgender Code ist gültig:

```c
static inline void log_error(int value) {
    trice32("err:Error %d", value);
}
```

LI identifiziert dann aber natürlicherweise die Trice-Stelle im Helper-Body, nicht jeden Aufrufer.

Das kann für manche Anwendungen völlig ausreichend sein, erfüllt aber die #531-Anforderung „Caller-Location“ nicht vollständig.

Ein registrierter Alias an der Anwendungstelle ist deshalb für diesen konkreten Fall die bessere Abstraktion.

### 5.3. <a id="module-process-automatisierung-bleibt-anwendungsspezifisch"></a>MODULE-/PROCESS-Automatisierung bleibt anwendungsspezifisch

Alias-zu-Tag löst nicht automatisch alle statischen Metadaten aus #531.

Nicht anders behaupten.

Mögliche vorhandene Ansätze:

- Modul aus LI-Pfad auf dem Host ableiten,
- statischen Text per CE ergänzen, wenn Selektoren/Konfiguration das sinnvoll erlauben,
- dynamische Task-/Prozess-IDs als skalare CE-Werte übertragen,
- Consumer mappt IDs/Locations auf projektspezifische Metadaten.

Ein späteres allgemeines Compile-Time-Metadaten-Registry kann interessant sein, ist aber keine Voraussetzung dieser Aufgabe.

---

## 6. <a id="bevorzugte-cli-richtung"></a>Bevorzugte CLI-Richtung

### 6.1. <a id="bestehende-option-erweitern-keine-namensheuristik"></a>Bestehende Option erweitern, keine Namensheuristik

Bevorzugte Syntax:

```sh
-alias MyAlarm:wrn
```

Die bestehende Syntax bleibt rückwärtskompatibel:

```sh
-alias MyAlarm
```

Mögliche symmetrische String-Alias-Syntax:

```sh
-salias MyTextAlarm:err
```

Ob tagged `-salias` im selben Schritt implementiert wird, nach Komplexitätsanalyse entscheiden. Semantisch wäre es konsequent, auch wenn scalar CE für Runtime-Strings außerhalb des Scopes bleibt.

### 6.2. <a id="bedeutung-von-nametag"></a>Bedeutung von `Name:Tag`

`Name` ist ein C-Makro-/funktionsartiger Token, der als Trice-Alias registriert wird.

`Tag` ist eine Trice-Anwendungstag-Gruppe, **kein zweites fest verdrahtetes Level-System**. Beispiel:

```sh
-alias MyAlarm:wrn
```

bedeutet:

- Alias = `MyAlarm`,
- Anwendungstag-Gruppe = Warning,
- Standardgewicht = das aktuelle Warning-Gewicht, normalerweise 600,
- exportierter Level folgt der normalen Gewicht-zu-Level-Abbildung.

Wenn das Warning-Gewicht absichtlich durch bestehende Trice-Konfiguration verändert wird, folgt der abgeleitete Level den bestehenden Regeln.

**Kein zweites Severity-Modell in Alias-Handling einführen.**

### 6.3. <a id="keine-semantik-aus-aliasnamen-ableiten"></a>Keine Semantik aus Aliasnamen ableiten

Diese Konfigurationen müssen identisch wirken, wenn derselbe Tag zugeordnet wird:

```sh
-alias MyAlarm:wrn
-alias ABC:wrn
-alias Banana:wrn
```

Kein Code darf Teilstrings wie `Error`, `Warn`, `Alarm`, `Fatal`, `Debug` interpretieren.

### 6.4. <a id="formatstrings-bleiben-eigentum-des-quellcodes-und-werden-durch-alias-tag-zuordnung-nicht-umgeschrieben"></a>Formatstrings bleiben Eigentum des Quellcodes und werden durch Alias-Tag-Zuordnung nicht umgeschrieben

Bei:

```sh
-alias MyAlarm:wrn
```

bleibt dieser Quellcode so wie geschrieben:

```c
MyAlarm("Temperature too high: %d degrees", value);
```

Nicht umschreiben in:

```c
MyAlarm("wrn:Temperature too high: %d degrees", value);
```

und nicht verlangen, dass der Benutzer `wrn:` selbst schreibt.

Bei Bind bleibt der Quelltext ohnehin unverändert.

Bei Insert/Clean dürfen normale ID-Insertion/-Entfernung und CE-Quelltransformationen gemäß bestehendem Vertrag stattfinden; **die Alias-Tag-Zuordnung allein darf keinen literalen Tag in den Benutzerformatstring einfügen**.

Offen bleibt, ob ein synthetisches Tag-Präfix **intern** im effektiven Schema/TIL existieren darf. Das wird weiter unten getrennt analysiert.

---

## 7. <a id="alias-tag-und-ce-selektor-sind-getrennte-dimensionen"></a>Alias-Tag und CE-Selektor sind getrennte Dimensionen

Das ist eine Kernanforderung dieses Designs.

Beispielquelle:

```c
MyAlarm("temperature:Temperature too high: %d degrees", value);
```

CLI:

```sh
trice bind \
  -alias MyAlarm:wrn \
  -ce 'temperature:", limit={limit:%d}", temperatureLimit'
```

Beabsichtigte Interpretation:

- `MyAlarm:wrn` ordnet das Ereignis der Warning-Gruppe zu.
- `temperature:` ist ein CE-Selektor im Quell-Formatstring.
- `temperatureLimit` ist Laufzeitkontext, der durch CE ergänzt wird.
- Warning-Klassifikation wird **nicht** aus `temperature:` abgeleitet.
- Die Quelle braucht kein `wrn:`.

Eine plausible finale sichtbare Meldung:

```text
Temperature too high: 85 degrees, limit=80
```

während Structured Output Warning-Tag/-Level und benanntes Kontextfeld trägt.

### 7.1. <a id="vorhandene-ce-selektorregeln-wiederverwenden"></a>Vorhandene CE-Selektorregeln wiederverwenden

CE scannt bereits eine zusammenhängende Präfixsequenz und unterstützt:

- Built-in-Tags als Selektoren,
- User Labels,
- freie lowercase Selektoren,
- mehrere Selektoren in Quellreihenfolge.

Die Alias-Tag-Funktion muss sich hier integrieren, nicht eine zweite Enrichment-Sprache erzeugen.

### 7.2. <a id="offene-entscheidung-soll-der-alias-tag-selbst-ce-regeln-auswählen"></a>Offene Entscheidung: Soll der Alias-Tag selbst CE-Regeln auswählen?

Das wäre wünschenswert und sollte ernsthaft geprüft werden.

Beispiel:

```sh
trice bind \
  -alias MyAlarm:wrn \
  -ce 'wrn:", task={task:%p}", osThreadGetId()'
```

Quelle:

```c
MyAlarm("Temperature too high: %d", value);
```

Intuitiv sollte jede als Warning konfigurierte Alias-Stelle diese Warning-weite CE-Regel erhalten können, obwohl kein literales `wrn:` im Quellformat steht.

CE-Selektion könnte also zwei Quellen berücksichtigen:

1. den zugeordneten Alias-Tag,
2. explizite Quellpräfixe wie `temperature:`.

Wird dieselbe CE-Regelgruppe über beide Wege erreicht, darf sie nur einmal angewendet werden.

Das ist noch keine endgültige Festlegung. Codex soll prüfen, ob es sich sauber in das bestehende `selectContextRules`-Design integrieren lässt.

---

## 8. <a id="was-passiert-bei-einem-tag-artigen-präfix-im-alias-format"></a>Was passiert bei einem tag-artigen Präfix im Alias-Format?

Dies ist bewusst **kein harter semantischer Konflikt**.

Beispiel:

```sh
-alias MyAlarm:wrn
```

Quelle:

```c
MyAlarm("err:Temperature too high: %d", value);
```

Vorgeschlagene Semantik:

- Ereignisklassifikation bleibt der explizit zugeordnete Alias-Tag: Warning.
- `err:` im Quellformat darf den Alias-Tag nicht still überschreiben.
- Der Quelltext wird aufgrund der Alias-Zuordnung weder automatisch entfernt noch umgeschrieben.
- Vorhandene CE-Präfixbehandlung darf `err:` weiterhin als Selektor sehen, wenn ausdrücklich eine entsprechende CE-Regel konfiguriert ist; diese Wechselwirkung muss getestet/dokumentiert werden.
- Der Build **kann einen unterdrückbaren Hinweis/Warnung** ausgeben, weil der Quelltext für Leser missverständlich sein kann.

Der Hinweis informiert über Ambiguität, er bestimmt nicht die Semantik.

Konzeptionell:

```text
temperature.c:137: warning: alias MyAlarm is assigned tag WARNING,
but the format begins with tag-like prefix "err:"; the alias tag controls
event classification and the prefix remains source text / selector input.
```

Die konkrete Formulierung und ein möglicher Warning-Schalter sind noch nicht festzulegen.

### 8.1. <a id="zu-analysierendes-warnverhalten"></a>Zu analysierendes Warnverhalten

Mögliche Policy:

- gleiche Tag-Gruppe wie Alias: kein Hinweis oder nur Redundanzhinweis,
- andere Built-in-Severity-Gruppe: Warnung,
- andere Built-in-Kategorie: Warnung oder keine, abhängig vom False-Positive-Risiko,
- registriertes `-ulabel`-Präfix: eher nur warnen, wenn es klar als Tag und nicht absichtlich als CE-Selektor benutzt wird,
- freier CE-Selektor wie `temperature:`: keine Warnung.

Der Hinweis muss unterdrückbar sein. Bevor ein neues `-Wno-*`-System erfunden wird, prüfen, ob Trice bereits ein passendes Warning-Control-Muster hat. Falls nicht, kleinstmögliche konsistente Lösung vorschlagen.

Die Unterdrückung darf **nur Diagnostik** beeinflussen, niemals:

- IDs,
- TIL,
- LI,
- CE-Selektion,
- erzeugte Binärrecords,
- Tag-Klassifikation.

---

## 9. <a id="zentrale-offene-architekturfrage-wo-lebt-der-zugeordnete-tag"></a>Zentrale offene Architekturfrage: Wo lebt der zugeordnete Tag?

Diese Entscheidung muss vor einer breiten Implementierung analysiert werden.

### 9.1. <a id="warum-persistenz-nötig-ist"></a>Warum Persistenz nötig ist

Zur Laufzeit überträgt das Target eine ID plus Laufzeitwerte. Später muss der Logger den Ereignis-Tag kennen für:

- Tag-Klassifikation,
- abgeleiteten Level,
- Farben,
- `-pick`,
- `-ban`,
- `-logLevel`,
- Tag-Statistik,
- Structured JSON/KV `tag` und `level`.

Wenn der Quell-Formatstring den Tag nicht selbst enthält, muss die Information irgendwo die Instrumentierung überleben.

Den Benutzer zu zwingen, dieselbe Alias-Zuordnung bei jedem späteren Decode erneut anzugeben, wäre fragil, besonders für gespeicherte Feldaufzeichnungen. Alte Captures müssen aus Build-Metadaten dekodierbar bleiben.

### 9.2. <a id="variante-a-optionales-tag-metadatum-in-tiljson"></a>Variante A: optionales Tag-Metadatum in `til.json`

Konzeptionelles Schema:

```json
{
  "1234": {
    "Type": "trice",
    "Strg": "Temperature too high: %d degrees",
    "Tag": "WARNING"
  }
}
```

Die exakte Repräsentation ist offen; `Tag` könnte kanonischen Gruppennamen, internen Schlüssel oder andere stabile Schreibweise speichern.

#### 9.2.1. <a id="vorteile"></a>Vorteile

- Saubere semantische Trennung zwischen Meldungstemplate und Ereignisklassifikation.
- Quellformat bleibt vollständig Anwendungstext/CE-Selektortext.
- Kein synthetischer Tag kann unter `-color off` sichtbar werden, obwohl der Benutzer ihn nie geschrieben hat.
- JSON `message` muss kein künstliches Präfix entfernen.
- Passt genau zur gewünschten Trennung zwischen Formatstringinhalt und Alias-Tag.
- Historische ID-Metadaten können selbstständig sein.
- Neuer Logger kann explizite Metadaten bevorzugen und bei alten Einträgen auf Präfixklassifikation zurückfallen.
- Userdefinierte Alias-Tags könnten ohne künstliche Textpräfixe sauber klassifiziert werden.

#### 9.2.2. <a id="nachteile--notwendige-arbeit"></a>Nachteile / notwendige Arbeit

- Ändert das TIL-Schema.
- `TriceFmt` oder äquivalenter persistenter Typ ändert sich.
- Alle Stellen, die Schemaidentität bislang als `Type + Strg` betrachten, müssen geprüft werden.
- Decoder/Logger muss explizites Tag-Metadatum verwenden, wenn vorhanden.
- Structured Output, Textfarben, Filter, Statistik usw. müssen denselben aufgelösten Tag nutzen.
- Generierte/lesbare Dictionary-Kopien und Tests müssen angepasst werden.
- Alte Hosttools werden ein unbekanntes JSON-Feld strukturell vermutlich ignorieren, würden ein Ereignis ohne Präfixtag aber weiter als `untagged` klassifizieren. Deshalb klare Host-/Dictionary-Kompatibilitätsgrenze dokumentieren.
- On-target/local-decoding-Pfade prüfen: brauchen sie explizite Tag-Metadaten oder dürfen sie sie bewusst ignorieren?

#### 9.2.3. <a id="kritische-id-historienregel"></a>Kritische ID-Historienregel

Ändert sich die Alias-Zuordnung von Warning auf Error, darf ein bestehender historischer ID-Eintrag nicht still seine Bedeutung ändern.

Daher muss ein expliziter Tag an der **Schemaidentität / ID-Ownership** teilnehmen, wenn er separat gespeichert wird.

Beispiel:

```text
MyAlarm + "Temperature..." + WARNING
```

ist semantisch nicht dasselbe historische Schema wie:

```text
MyAlarm + "Temperature..." + ERROR
```

Ein zusätzliches vergleichbares Feld in `TriceFmt` könnte Map-Key-Gleichheit automatisch beeinflussen, aber das allein nicht ungeprüft als vollständig ausreichend annehmen; alle ID-Allokations-/Reuse-Pfade kontrollieren.

### 9.3. <a id="variante-b-synthetischen-tag-nur-dem-effektiven-dictionary-format-voranstellen"></a>Variante B: synthetischen Tag nur dem effektiven Dictionary-Format voranstellen

Quelle bleibt:

```c
MyAlarm("Temperature too high: %d degrees", value);
```

Intern behandelt die Instrumentierung das effektive gespeicherte Schema aber wie:

```text
wrn:Temperature too high: %d degrees
```

ohne Benutzerquelltext zu ändern.

#### 9.3.1. <a id="vorteile-1"></a>Vorteile

- Wiederverwendung des vorhandenen Tag-Parsers und großer Teile des Host-Loggings.
- Keine TIL-Schemaerweiterung.
- Schemaidentität ändert sich automatisch mit dem zugeordneten Tag, weil `Strg` sich ändert.
- Bestehende Filter, Statistik, Farben, Levels und ggf. CE-Tagselektion könnten mit relativ wenig Host-Code funktionieren.
- Historische TIL-Semantik bleibt näher am heutigen Modell.

#### 9.3.2. <a id="nachteile--risiken"></a>Nachteile / Risiken

Diese Variante wirkt simpel, ist aber nur nach Tests akzeptabel:

1. Das gespeicherte `Strg` entspricht nicht mehr wörtlich dem Benutzerformatstring.
2. `-color off` zeigt heute geschriebene Tagpräfixe an. Ein synthetisches Präfix könnte sichtbar werden, obwohl der Benutzer es nie schrieb.
3. Lowercase-Präfixentfernung wird Teil der Semantik. Bei userdefinierten Tags/Casing kann künstlicher Text „leaken“.
4. Beginnt die Quelle mit einem anderen Präfix:

   ```c
   MyAlarm("err:Temperature ...")
   ```

   könnte das effektive Schema zu:

   ```text
   wrn:err:Temperature ...
   ```

   werden. Das kann funktionieren, muss aber für Text- und Structured-Message-Verhalten explizit geprüft werden.
5. Ein Quellselektor `temperature:` steht dann hinter dem synthetischen Tag; CE-Reihenfolge und Präfixentfernung brauchen gezielte Tests.
6. Custom Tags aus `-ulabel` sind case-sensitive. Eine kanonische versteckte Textschreibweise zu erzeugen kann unnötig kompliziert werden.
7. Der Nutzer möchte gerade, dass Quellformatinhalt nicht semantisch als Ereignis-Tag interpretiert werden muss. Ein synthetisches Präfix koppelt Klassifikation intern weiterhin an `Strg`.

Diese Variante kann trotzdem die risikoärmste Implementierung sein, **wenn** die Präsentation synthetische Klassifikation zuverlässig von Benutzertext trennt oder Tests zeigen, dass keine unerwünschten Effekte entstehen.

### 9.4. <a id="variante-c-dieselbe-alias-tag-cli-auch-beim-logging-verlangen"></a>Variante C: dieselbe Alias-/Tag-CLI auch beim Logging verlangen

Beispiel:

```sh
trice log -alias MyAlarm:wrn ...
```

#### 9.4.1. <a id="vorteil"></a>Vorteil

- Keine TIL-Änderung.

#### 9.4.2. <a id="nachteile"></a>Nachteile

- Binärereignis enthält nur ID, nicht Aliasname.
- Aliasname ist aktuell nicht persistent in TIL (`Alias` ist `json:"-"`).
- Gespeicherte Captures würden externes Wissen über Build-CLI benötigen.
- Historische Dekodierung wird fragil.
- Ein Logging-Kommando scannt nicht notwendigerweise den ursprünglichen Source-Tree.
- Widerspricht dem Dictionary-Gedanken von Trice.

**Diese Variante normalerweise verwerfen.** Instrumentierung darf die Zuordnung per CLI erhalten; die später nötige Information soll aber aus Build-Metadaten stammen.

### 9.5. <a id="variante-d--ulabel--untagged-ohne-explizite-neue-metadaten-wiederverwenden"></a>Variante D: `-ulabel` / `untagged` ohne explizite neue Metadaten wiederverwenden

Unbedingt analysieren, weil das aktuelle Tag-System bereits wichtige Bausteine besitzt:

- userdefinierte Tags (`-ulabel`),
- Built-in `untagged`,
- Klassifikation aus gespeicherten Formatpräfixen,
- Gewichte/Farben,
- tag-spezifische ID-Bereiche.

Aber: `-ulabel` registriert derzeit **Namen, die in Formatpräfixen vorkommen**. Es ordnet nicht automatisch eine Trice-ID oder Alias-Invocation einer Tag-Gruppe zu, wenn im Format kein solches Präfix steht.

Daher löst `-ulabel` allein Alias-zu-Tag nicht offensichtlich. Seine Registrierung/Kanonisierung sollte aber möglichst für die rechte Seite von `-alias Name:Tag` wiederverwendet werden.

Auch `untagged` bleibt als Default-/Fallback-Gruppe wertvoll, trägt aber keine externe Alias-Zuordnung.

Codex soll den Emitter-/Tag-Registry-Code prüfen und wiederverwenden statt parallele Tag-Tabellen zu schaffen.

### 9.6. <a id="variante-e-tag-aus-id-bereichen-ableiten"></a>Variante E: Tag aus ID-Bereichen ableiten

Tag-spezifische ID-Ranges **nicht** als primäre persistente Tag-Darstellung verwenden.

ID-Ranges existieren für Routing/Allokation; die semantische Klassifikation daraus abzuleiten koppelt unabhängige Konzepte und wird fragil bei späterer Range-Änderung.

ID-Ranges sollen **Consumer des aufgelösten Tags** sein, nicht dessen Datenbank.


---

## 10. <a id="empfohlenes-internes-modell-zur-prüfung"></a>Empfohlenes internes Modell zur Prüfung

Wenn reichere Metadaten nötig sind, `TriceAliases []string` nicht zwangsläufig unverändert lassen.

Konzeptionell könnte es intern etwa so aussehen:

```go
type AliasSpec struct {
    Name string
    Kind AliasKind // scalar trice oder string triceS
    Tag  string    // leer bedeutet heutiges Verhalten
}
```

Das ist nur ein Modell, kein vorgegebener Typname.

Anforderungen:

- Bestehendes `-alias Name` und `-salias Name` exakt erhalten.
- `-alias Name:Tag` deterministisch parsen.
- C-Identifier enthalten kein `:`, daher ist Split am ersten Doppelpunkt praktikabel.
- `Name` gemäß bestehender Alias-Regeln validieren.
- Built-in-Tag-Aliase über bestehende Emitter-/Tag-Registry auflösen.
- `-ulabel`-Tags berücksichtigen.
- Exakte Groß-/Kleinschreibung userdefinierter Labels erhalten.
- Wirklich mehrdeutige Duplicate-Konfiguration ablehnen.

### 10.1. <a id="doppelte-bzw-widersprüchliche-cli-definitionen"></a>Doppelte bzw. widersprüchliche CLI-Definitionen

Folgendes kann idempotent akzeptiert werden:

```sh
-alias MyAlarm:wrn -alias MyAlarm:wrn
```

Aber:

```sh
-alias MyAlarm:wrn -alias MyAlarm:err
```

sollte ein Konfigurationsfehler sein.

Ebenso analysieren:

```sh
-alias MyAlarm
-alias MyAlarm:wrn
```

Bevorzugt Mehrdeutigkeit ablehnen statt still Option-Reihenfolge semantisch werden zu lassen, sofern keine starke Kompatibilitätsbegründung für „last wins“ existiert.

Auch bestehendes Verhalten, wenn derselbe Token zugleich in `-alias` und `-salias` registriert wird, prüfen. Der aktuelle Code besitzt dafür spezielle Disambiguierung; neue Tag-Metadaten dürfen diese Logik nicht versehentlich brechen.

### 10.2. <a id="kanonisierung-des-zugeordneten-tags"></a>Kanonisierung des zugeordneten Tags

Bei Built-in-Aliasen:

```text
wrn
warn
WARNING
```

handelt es sich semantisch um dieselbe Gruppe.

Intern möglichst stabile kanonische Identität speichern, damit unterschiedliche CLI-Schreibweise nicht unnötig ID-Churn erzeugt.

Falls die Implementierung einen synthetischen Präfix verwendet, muss zusätzlich eine deterministische, präsentationssichere Schreibweise gewählt werden. Das ist ein Argument für explizite Tag-Metadaten.

Bei `-ulabel`-Tags die heute exakte/case-sensitive Semantik bewahren.

---

## 11. <a id="reihenfolge-relativ-zu-ce-und-id-vergabe"></a>Reihenfolge relativ zu CE und ID-Vergabe

Das ist kritisch.

Konzeptionell sollte ein Zielworkflow so aussehen:

```text
Alias-Invocation in Source
    -> Alias und optional zugeordneten Tag bestimmen
    -> Quell-CE-Präfixselektoren analysieren
    -> CE-Regeln aus Source-Selektoren und ggf. Alias-Tag auswählen
    -> finales semantisches Schema konstruieren
    -> Argumentanzahl/-typen validieren
    -> Trice-ID anhand des vollständigen finalen Schemas vergeben/wiederverwenden
    -> Bind-Sidecar oder Insert-Edit erzeugen
    -> TIL/LI/Build-Metadaten schreiben
```

Der Alias-Tag muss früh genug bekannt sein für:

- CE-Auswahl über Tag, falls unterstützt,
- tag-spezifische ID-Range-Auswahl,
- Schemaidentität,
- Diagnostik über tag-artige Quellpräfixe.

Er darf aber nicht unerwünschte literale Quelltextänderungen verursachen.

### 11.1. <a id="bind"></a>Bind

Für Bind:

- Quellaufruf bleibt unverändert,
- generierter Sidecar trägt normalen ID-Adapter und ggf. CE-Adapter,
- LI soll auf die Alias-Invocation zeigen,
- Tag-Zuordnung muss in Build-Dictionary/Metadaten erhalten bleiben.

Direkte Aliase gründlich testen, weil `bind -ce` strengere Wrapper-Grenzen hat. Ein **registrierter direkter Alias** darf durch Tag-Metadaten nicht versehentlich als nicht unterstützter Wrapper klassifiziert werden.

### 11.2. <a id="insert"></a>Insert

Für Insert:

- vorhandene Alias-Erkennung erhalten,
- IDs wie bisher in Alias-Aufrufe einsetzen,
- CE darf Source gemäß heutiger Insert-Semantik verändern,
- Alias-Tag-Zuordnung selbst soll keinen literalen Tag in den Benutzerformatstring schreiben, sofern das finale Design nicht ausdrücklich anders entscheidet und dokumentiert.

### 11.3. <a id="clean"></a>Clean

Clean muss Insert-/CE-Transformationen mit derselben Alias-Konfiguration rückgängig machen können.

Falls Alias-Tag beeinflusst, welche CE-Regeln greifen, müssen wiederholtes Insert/Clean mit identischen Optionen stabil und reversibel bleiben.

### 11.4. <a id="generate--generierte-lookup-daten"></a>Generate / generierte Lookup-Daten

Prüfen:

- `generate -logC`,
- lesbare JSON-Dictionary-Kopien,
- generierte C-Tabellen,
- Local-/On-target-Decoding-Tabellen,
- Bind-Metadaten, die `generate` konsumiert.

Wenn Tag ein neues persistentes Schemafeld wird, müssen alle generierten Repräsentationen es dort erhalten, wo es nötig ist, oder explizit dokumentieren, warum es Host-only ist.

---

## 12. <a id="logging-seite-einheitliches-verhalten-erzwingen"></a>Logging-Seite: einheitliches Verhalten erzwingen

Unabhängig vom Speicherdesign müssen **alle** Host-Funktionen denselben aufgelösten Ereignis-Tag verwenden.

Nicht nur JSON patchen.

Mindestens prüfen:

- Textfarbe/Präfixbehandlung,
- `-pick`,
- `-ban`,
- `-logLevel`,
- Tag-Statistik,
- Structured JSON `tag`,
- Structured JSON `level`,
- KV `tag`/`level`,
- Visualisierungsfilter,
- tag-spezifische ID-Range-Auswahl während Instrumentierung,
- Remote-Display-Pfade, die klassifizierte Application Events nutzen.

Structured Logging leitet `level` bereits aus `tag` ab. Diese Single-Source-of-Truth erhalten.

### 12.1. <a id="rückwärtskompatibilität-für-untagged"></a>Rückwärtskompatibilität für `untagged`

Alte TIL-Einträge ohne expliziten Alias-Tag müssen exakt wie heute funktionieren:

- erkanntes Formatpräfix -> entsprechende Gruppe,
- kein erkanntes Präfix -> `untagged`.

Falls ein neues explizites Metadatenfeld eingeführt wird, muss es optional sein und sauber auf dieses Verhalten zurückfallen.

---

## 13. <a id="location-information-kein-neuer-file-id-mechanismus"></a>Location Information: kein neuer File-ID-Mechanismus

Mehrere Ansätze wurden erwogen und sollen **nicht** Default-Design werden.

### 13.1. <a id="separates-filesjson"></a>Separates `files.json`

Idee: weitere Registry mit stabilen Datei-IDs und Übertragung von Datei-ID + Zeile.

Für #531 verworfen, weil normale Trice-ID bereits über `li.json` auf Datei/Zeile abbildet. Doppelt Information und erhöht Target-Payload.

### 13.2. <a id="bind-sidecar-file-keys-übertragen"></a>Bind-Sidecar-File-Keys übertragen

Technisch möglich, aber aus demselben Grund unnötig für normale Quellpositionsausgabe.

Sidecar-Keys bleiben intern für Bind-Ownership nützlich, sollen aber nicht übertragen werden, nur um eine vom Event-ID bereits bekannte Position wieder aufzubauen.

### 13.3. <a id="__file__-in-mehrere-integerwerte-packen"></a>`__FILE__` in mehrere Integerwerte packen

Technisch möglich, insbesondere mit CE, aber für statische Source-Location klar schlechter:

- verschwendet Payload,
- belegt Argumentslots,
- braucht Pack-/Unpack-Konvention,
- dupliziert LI-Metadaten.

Nur als Nischenlösung denkbar, wenn **dynamische Caller-Location** wirklich nicht durch instrumentierte ID dargestellt werden kann; nicht für #531.

---

## 14. <a id="caller-location-alias-vs-helper-funktion-vs-wrapper"></a>Caller-Location: Alias vs. Helper-Funktion vs. Wrapper

Diese Unterscheidung ist zentral für #531 und muss dokumentiert werden.

### 14.1. <a id="direkter-trice-aufruf-oder-registrierter-alias"></a>Direkter Trice-Aufruf oder registrierter Alias

Bevorzugt, wenn Caller-Location wichtig ist:

```c
MyAlarm("Temperature too high: %d", value);
```

Die Alias-Invocation soll die instrumentierte Quellstelle sein, mit eigener Trice-ID und LI-Eintrag.

### 14.2. <a id="normale-hilfsfunktion"></a>Normale Hilfsfunktion

```c
void log_error(int value) {
    trice32("err:Error %d", value);
}
```

Alle Aufrufer verwenden dieselbe innere Trice-Stelle. LI zeigt auf den Funktionskörper.

Das ist valide und einfach, wenn diese Location genügt, liefert aber nicht automatisch Datei/Zeile des Anwendungscallers.

### 14.3. <a id="allgemeines-logging-wrapper-makro-mit-inneren-trice-aufrufen"></a>Allgemeines Logging-Wrapper-Makro mit inneren Trice-Aufrufen

Die bestehende Bind-Doku beschreibt, dass Wrapper-Definitionsstellen die IDs besitzen können und `bind -ce` bestimmte komplexe Wrapper-/Rebase-Fälle bewusst zurückweist.

Nicht einen **registrierten Alias an der Invocation-Stelle** mit einem **Wrapper-Makro, das intern einen Trice-Aufruf enthält**, verwechseln. Diese Alias-Tag-Erweiterung soll ersteres verbessern, nicht sämtliche Wrapper-Grenzen von Bind beseitigen.

---

## 15. <a id="laufzeitkontext-aus-531"></a>Laufzeitkontext aus #531

### 15.1. <a id="task-kontext"></a>Task-Kontext

Bereits ein guter CE-Fall.

Wenn Alias-Tag CE auswählen darf:

```sh
-ce 'wrn:", task={task:%p}", osThreadGetId()'
```

oder unabhängig davon über eigenen Quellselektor:

```c
MyAlarm("taskctx:Temperature too high: %d", value);
```

```sh
-ce 'taskctx:", task={task:%p}", osThreadGetId()'
```

### 15.2. <a id="timestamp"></a>Timestamp

Vorhandene Trice-Target-Stamp-Mechanismen verwenden statt pauschal ein weiteres Zeitargument anzuhängen, sofern die Anwendung keinen separaten semantischen Wert braucht.

Structured Output exportiert Target-Stamps bereits getrennt.

### 15.3. <a id="module--process"></a>MODULE / PROCESS

Nicht automatisch als Runtime-Strings übertragen.

Mögliche Strategien:

- Modul aus Quellpfad im Host-Consumer ableiten,
- Custom Selektoren mit statischem CE-Text,
- projektspezifische Alias-Familien,
- nur kompakte dynamische Prozess-/Task-ID übertragen, wenn Kontext wirklich laufzeitvariabel ist.

Alias-zu-Tag nicht zu Alias-zu-beliebigen-N-Metadaten ausweiten, sofern nicht separat entschieden.

---

## 16. <a id="runtime-strings-ausdrückliches-nicht-ziel"></a>Runtime-Strings: ausdrückliches Nicht-Ziel

Für diese Aufgabe ist ausdrücklich akzeptabel:

> Context Enrichment beliebiger Runtime-Strings wird durch diese Funktion nicht unterstützt.

Nicht Trices Scalar-/String-Record-Modell neu entwerfen, nur um Alias-Tags zu ermöglichen.

Bestehende Einschränkungen von `triceS`, Buffern und skalarer CE bleiben.

Falls ein Projekt einen zur Laufzeit erzeugten String braucht, kann es die existierende String-Familie unter deren Regeln verwenden. Tagged `-salias` kann trotzdem für Klassifikation sinnvoll sein; beliebige zusätzliche skalare CE-Werte daran anzuhängen bleibt out of scope.

---

## 17. <a id="mögliche-verhaltensmatrix-für--alias-nametag"></a>Mögliche Verhaltensmatrix für `-alias Name:Tag`

Angenommen:

```sh
-alias MyAlarm:wrn
```

| Source Call | Ereignis-Tag | Semantik des Formattexts | Mögliche Build-Diagnostik |
|---|---|---|---|
| `MyAlarm("Too hot %d", v)` | Warning | unverändert | keine |
| `MyAlarm("temperature:Too hot %d", v)` | Warning | `temperature:` steht CE zur Verfügung; normale CE-Entfernungsregeln gelten bei Match | keine |
| `MyAlarm("err:Too hot %d", v)` | Warning | `err:` überschreibt Alias-Tag nicht; bleibt Text/Selektorinput | unterdrückbare Warnung empfohlen |
| `MyAlarm("wrn:Too hot %d", v)` | Warning | Quelltext wird durch Alias-Zuordnung nicht umgeschrieben | optional Redundanzhinweis oder keiner |
| `MyAlarm("INFO:Too hot %d", v)` | Warning | Quellpräfix überschreibt Alias-Tag nicht | unterdrückbare Warnung empfohlen |

Wichtig: Falls das finale Design einen synthetischen Tag in TIL verwendet, bedeutet „Formattext unverändert“ zunächst **Benutzerquelltext unverändert**. Host-visible `message` muss trotzdem so behandelt werden, dass künstliche Metadaten nicht unerwartet auftauchen.

---

## 18. <a id="kompatibilitäts--und-migrationsanforderungen"></a>Kompatibilitäts- und Migrationsanforderungen

### 18.1. <a id="bestehende-cli-muss-unverändert-funktionieren"></a>Bestehende CLI muss unverändert funktionieren

Diese Befehle müssen ihr Verhalten behalten:

```sh
trice bind -alias LOG
trice insert -alias LOG
trice clean -alias LOG
```

Tagged-Alias-Syntax ist additiv.

### 18.2. <a id="alte-til---neues-tool"></a>Alte TIL -> neues Tool

Muss exakt wie heute funktionieren.

Falls optionales `Tag`-Feld kommt, führt dessen Fehlen zur bisherigen Formatpräfix-/`untagged`-Klassifikation.

### 18.3. <a id="neue-til---altes-tool"></a>Neue TIL -> altes Tool

Falls TIL ein neues JSON-Feld erhält, wird altes Go-JSON-Decoding dieses wahrscheinlich strukturell ignorieren. Aber alte Logger verstehen die Alias-Tag-Semantik nicht, wenn kein Formatpräfix vorhanden ist.

Das als Host-Kompatibilitätsgrenze dokumentieren. Nicht „voll rückwärtskompatibel“ nennen, nur weil JSON geparst werden kann.

### 18.4. <a id="historische-kumulative-til"></a>Historische kumulative TIL

Trice bewahrt alte ID-/Template-Mappings, damit alte Captures dekodierbar bleiben.

Eine geänderte Alias-Tag-Zuordnung darf die Semantik einer alten ID nicht in-place verändern.

Das ist eine harte Anforderung, unabhängig von der gewählten Speichervariante.

### 18.5. <a id="id-änderungen"></a>ID-Änderungen

Ein Tag-Wechsel verändert Ereignissemantik: Filter, Severity, Statistik, ggf. Routing. Deshalb ist es plausibel und wahrscheinlich notwendig, dass eine solche Änderung eine neue Schemaidentität/ID erzeugt, sofern historische Korrektheit das erfordert.

Explizit testen.

---

## 19. <a id="wechselwirkung-mit--ulabel"></a>Wechselwirkung mit `-ulabel`

Diesen Bereich bewusst analysieren.

`-ulabel` kann bereits:

- einen Custom-Tag-Namen registrieren,
- Gewicht setzen/ändern,
- Farbe setzen/ändern,
- in Bind/Insert-Taghandling und ID-Ranges teilnehmen.

Wünschenswerte Syntax:

```sh
trice bind \
  -ulabel sensor:650 \
  -alias SensorAlarm:sensor
```

Fragen:

1. Muss `sensor` durch `-ulabel` registriert sein, bevor es rechts in `-alias` benutzt werden darf?
2. Soll unbekannter Alias-Tag mit Hinweis auf `-ulabel` abgewiesen werden?
3. Soll CLI-Reihenfolge wie heute bei Label-/Filterauflösung egal sein?
4. Wenn TIL nur Tagname speichert: Muss späteres `trice log` ebenfalls `-ulabel sensor:650` erhalten, um Gewicht/Farbe zu kennen? Nach heutiger Custom-Tag-Philosophie wahrscheinlich ja, solange Metadaten nicht weiter ausgebaut werden.
5. Soll `-alias X:untagged` explizit erlaubt sein?

Empfohlenes Prinzip:

> Bestehende Tag-Registry für Validierung, Gewicht und Farbe wiederverwenden. Keine zweite unabhängige Alias-Severity-Registry bauen.

Gleichzeitig muss die Alias-zu-Tag-Gruppenidentität persistent genug sein, um spätere Dekodierung zu überleben.

---

## 20. <a id="wechselwirkung-mit-tag-spezifischen-id-ranges--target-routing"></a>Wechselwirkung mit tag-spezifischen ID-Ranges / Target Routing

Zugeordnete Alias-Tags sollen bei Instrumentierungsregeln wie normale Formatstring-Tags behandelt werden.

Wenn Warning einer ID-Range/einem Routing zugeordnet ist, soll:

```sh
-alias MyAlarm:wrn
```

natürlich daran teilnehmen.

Die numerische Range darf nicht selbst zur persistenten Tag-Repräsentation werden.

Reihenfolge prüfen: Tag muss vor ID-Range-Allokation aufgelöst sein.

---

## 21. <a id="präsentation-und--color"></a>Präsentation und `-color`

Besonders wichtig, falls synthetisches TIL-Präfix gewählt wird.

Heute gilt sinngemäß:

- `-color default` / Paletten: erkannte lowercase Quell-Tagpräfixe können entfernt und eingefärbt werden,
- `-color none`: lowercase Präfixe werden ohne Farbe entfernt,
- `-color off`: geschriebene Präfixe bleiben sichtbar.

Bei Alias-Tags hat der Benutzer ggf. **überhaupt kein Präfix geschrieben**.

Deshalb muss beantwortet werden:

> Soll `-color off` für `MyAlarm("Too hot")` plötzlich ein synthetisches `wrn:` anzeigen?

Bevorzugte Antwort: **Nein.** Dieser Text war nicht Teil der Quellmeldung.

Das spricht stark für explizite Tag-Metadaten oder wenigstens einen internen Marker, der synthetische Klassifikation von Benutzertext unterscheidet.

Ebenso soll ein echtes Quellpräfix, das mit Alias-Tag kollidiert, Benutzertext bleiben, sofern nicht eine explizite bestehende CE-Regel es transformiert.

---

## 22. <a id="anforderungen-an-structured-output"></a>Anforderungen an Structured Output

Ein Alias-getaggtes Ereignis sollte konzeptionell so aussehen:

```json
{
  "tag": "WARNING",
  "level": "WARNING",
  "message": "Temperature too high: 85 degrees",
  "file": "src/temperature.c",
  "line": 137,
  "fields": {
    "value": 85
  }
}
```

Ergänzt CE Task-/Limit-Felder, erscheinen sie nach den bestehenden Structured-Logging-Regeln im Benutzerfeld-Namespace.

Den Aliasnamen nicht automatisch in jeden Record aufnehmen, sofern dafür kein separates Bedürfnis besteht. Aliasname ist Source-/Instrumentierungsmetadatum, nicht zwangsläufig Laufzeit-Logsemantik.

---

## 23. <a id="selektive-datei-zeilenanzeige-bei-errors-für-531-wahrscheinlich-keine-implementierung-nötig"></a>Selektive Datei-/Zeilenanzeige bei Errors: für #531 wahrscheinlich keine Implementierung nötig

Der ursprüngliche Nutzer fragte nach Datei/Zeile nur bei Errors.

Da v2 Structured Output `file`/`line` bereits als getrennte Host-Metadaten liefert, ist die einfachste Antwort:

- LI normal auflösen,
- Downstream-Consumer zeigt Quellposition nur für gewünschte Tags/Levels.

Falls später `trice log` / `tlog` selbst Location für andere Events unterdrücken soll, kann das als kleine unabhängige Host-Ausgabeoption ergänzt werden. Nicht mit Alias-Tag-Design verschränken.

Mögliche Zukunftsidee, **keine aktuelle Anforderung**: Location-Ausgabeselektor analog zu `-pick` / `-logLevel`.


---

## 24. <a id="testplan"></a>Testplan

Codex soll Tests ergänzen bzw. erweitern, bevor die Funktion als vollständig gilt.

### 24.1. <a id="cli-parsing"></a>CLI-Parsing

Testen:

```text
-alias LOG
-alias LOG:wrn
-alias LOG:WARNING
-alias LOG:customTag   (mit passendem -ulabel)
```

Ungültig:

- leerer Alias,
- leerer Tag,
- unbekannter Tag, falls Registrierung vorgeschrieben wird,
- malformed Mehrfach-Doppelpunkt-Form,
- widersprüchliche doppelte Alias-Zuordnungen.

### 24.2. <a id="kanonische-äquivalenz"></a>Kanonische Äquivalenz

Built-in-Entsprechungen:

```text
wrn
warn
WARNING
```

müssen dieselbe semantische Gruppe ergeben und dürfen nicht allein aufgrund unterschiedlicher CLI-Schreibweise unnötigen ID-Wechsel erzeugen.

### 24.3. <a id="bind-mit-direktem-alias"></a>Bind mit direktem Alias

Beispiel:

```c
MY_ALARM("Temperature %d", value);
```

Prüfen:

- Source bleibt unverändert,
- Alias-Invocation erhält ID,
- LI zeigt auf Invocation-Zeile,
- TIL/Schema persistiert zugeordneten Tag durch die gewählte Methode,
- Decode ergibt Warning-Tag/-Level.

### 24.4. <a id="insert--clean"></a>Insert / Clean

Round Trip prüfen:

```text
Original
-> insert mit Alias-Tag
-> clean mit denselben Optionen
-> Original
```

Tag-Zuordnung darf keinen Resttext im Source hinterlassen.

### 24.5. <a id="alias-tag--ce-quellselektor"></a>Alias-Tag + CE-Quellselektor

```c
MY_ALARM("temperature:Temperature %d", value);
```

mit:

```sh
-alias MY_ALARM:wrn
-ce 'temperature:", limit={limit:%d}", limit'
```

Prüfen:

- Tag = Warning,
- CE genau einmal,
- Quellselektor gemäß bestehenden Entfernungsregeln,
- Felder korrekt dekodiert,
- LI bleibt Alias-Call-Site.

### 24.6. <a id="alias-tag--ce-über-zugeordneten-tag"></a>Alias-Tag + CE über zugeordneten Tag

Falls dieses Verhalten angenommen wird:

```sh
-alias MY_ALARM:wrn
-ce 'wrn:", task={task:%p}", osThreadGetId()'
```

Prüfen, dass Warning-weites CE auch ohne literales `wrn:` im Source greift.

### 24.7. <a id="kollidierendes-tag-artiges-quellpräfix"></a>Kollidierendes tag-artiges Quellpräfix

```c
MY_ALARM("err:Temperature %d", value);
```

mit `-alias MY_ALARM:wrn`.

Prüfen:

- Ereignis bleibt Warning,
- Diagnostik erscheint, falls aktiviert,
- Warnungsunterdrückung entfernt nur die Diagnostik,
- Behandlung von `err:` als Text/CE-Selektor entspricht Doku,
- kein Hard Error nur wegen abweichendem Präfix.

### 24.8. <a id="historisches-id-verhalten"></a>Historisches ID-Verhalten

Build A:

```sh
-alias MY_ALARM:wrn
```

Build B:

```sh
-alias MY_ALARM:err
```

Prüfen, dass alte Capture-/Dictionary-Semantik erhalten bleibt und nicht still umgedeutet wird.

### 24.9. <a id="structured-output"></a>Structured Output

JSON/KV prüfen:

- korrektes `tag`,
- korrekt abgeleiteter `level`,
- korrektes `message`,
- `file` / `line` aus LI,
- CE-Felder,
- kein synthetisches Präfix im `message`, sofern nicht ausdrücklich beabsichtigt.

### 24.10. <a id="textausgabe-und-farben"></a>Textausgabe und Farben

Mindestens prüfen:

```text
-color default
-color none
-color off
```

besonders bei synthetischer interner Präfixlösung.

### 24.11. <a id="filtering-und-statistik"></a>Filtering und Statistik

Prüfen, dass Alias-getaggte Ereignisse korrekt teilnehmen an:

- `-pick wrn`,
- `-ban wrn`,
- `-logLevel wrn`,
- Tag-Statistik,
- Gewicht-Overrides.

### 24.12. <a id="-ulabel"></a>`-ulabel`

Falls Custom Alias Tags unterstützt werden, exakte Case-Semantik und Gewichte testen.

### 24.13. <a id="id-range--routing"></a>ID Range / Routing

Prüfen, dass zugeordneter Alias-Tag genau wie ein normaler Formatstring-Tag an tag-spezifischer ID-Range-Auswahl teilnimmt.

### 24.14. <a id="rückwärtskompatibilität"></a>Rückwärtskompatibilität

Testen:

- vorhandene ungetaggte Aliase ohne `:Tag`,
- normale Trice-Aufrufe,
- alte TIL-Einträge,
- bestehende Alias-Testkorpora für Insert/Bind.

### 24.15. <a id="-salias"></a>`-salias`

Falls im Scope: Tag-Klassifikation testen, während die bestehende Einschränkung bestehen bleibt, dass Runtime-String-Records keine beliebigen zusätzlichen skalaren CE-Werte erhalten.

---

## 25. <a id="voraussichtlich-relevante-implementierungsbereiche"></a>Voraussichtlich relevante Implementierungsbereiche

Nicht davon ausgehen, dass dies vollständig ist; Code-Suche verwenden.

Wahrscheinlich relevant in `v2.0.0-beta-1` bzw. aktuellem `main`:

- `internal/args/init.go`
  - CLI-Definitionen für `-alias`, `-salias`, `-ulabel`.
- `internal/id/vars.go`
  - aktueller Speicher für `TriceAliases` / `TriceSAliases`.
- Datei mit `resolveTriceAlias` und `ProcessAliases`
  - Alias-Typauflösung und Regex-Konstruktion.
- `internal/id/bindAdvanced.go`
  - Abgrenzung Alias vs. Wrapper.
- `internal/id/contextEnrichment.go`
  - CE-Selektorauflösung und finale Schemaerzeugung.
- `internal/id/contextSource.go`
  - Insert/Clean CE-Verarbeitung.
- `internal/id/id.go`
  - `TriceFmt`, TIL-Repräsentation, LI-Typen.
- ID-Allokations-/Reverse-Lookup-Dateien
  - Schemaidentität und historische Wiederverwendung.
- `internal/translator/structured.go`
  - Structured Tag/Message/Level Rendering.
- `internal/translator/translator.go`
  - Textfilter/-präsentation und LI.
- `internal/emitter/*`
  - Tag-Registry, Gewichte, Aliase, Farben, Event-Klassifikation.
- generierte Dictionaries / `generate -logC`.
- Alias-Tests:
  - `internal/id/manage_test.go`
  - `internal/id/bindIntegration_test.go`
  - `internal/id/bindIDs_test.go`
  - `internal/id/contextSource_test.go`
  - passende CLI-Integrationstests.

Vor Änderungen `AGENTS.md` lesen. Neue Kommentare im Code entsprechend Repository-Konventionen auf Englisch halten und Diffs fokussiert lassen.

---

## 26. <a id="designfragen-die-codex-vor-größerer-implementierung-explizit-beantworten-muss"></a>Designfragen, die Codex vor größerer Implementierung explizit beantworten muss

Kurze Designentscheidung zu allen Punkten erstellen:

1. **Persistenz:** Braucht TIL ein optionales `Tag`-Feld, oder kann bestehende Format-/Tag-Mechanik die Zuordnung ohne schlechte Präsentations-/Kompatibilitätseffekte darstellen?
2. **Schemaidentität:** Wie erzwingt ein Tag-Wechsel historisch sichere Semantik?
3. **Kanonische Tag-Identität:** Wie werden `wrn`, `warn`, `WARNING` und Custom-`-ulabel` normalisiert?
4. **CE:** Darf der zugeordnete Alias-Tag selbst `-ce`-Regeln auswählen?
5. **Quellpräfix:** Was genau passiert bei `err:` im Source, wenn Alias-Tag Warning ist?
6. **Diagnostik:** Welche Warnung wird ausgegeben und wie konsistent unterdrückt?
7. **Textpräsentation:** Wie wird sichergestellt, dass synthetische Metadaten bei `-color off` nicht unerwartet sichtbar werden?
8. **Custom Tags:** Kann `-alias X:sensor` zusammen mit `-ulabel sensor:650` verwendet werden; was muss späteres Logging wissen?
9. **ID-Ranges:** Nimmt Alias-Tag vor Allokation an `-IDRange` teil?
10. **Bind/Insert/Clean:** Lassen sich identische Semantiken ohne Quellkorruption gewährleisten?
11. **Generate/local decode:** Braucht generiertes/on-target Logging Tag-Metadaten ebenfalls?
12. **Kompatibilität:** Verhalten bei neuer TIL + altem Host sowie alter TIL + neuem Host?
13. **`-salias`:** jetzt mitnehmen oder verschieben?
14. **Warnungen:** tag-artige-Präfixwarnung nur für Built-in-Severity-Tags oder für breitere registrierte Tag-Gruppen?

Bevorzugt kleinste Lösung, die bestehende Trice-Konzepte erhält statt parallele Mechanismen einzuführen.

---

## 27. <a id="empfohlene-stufenweise-implementierungsstrategie"></a>Empfohlene stufenweise Implementierungsstrategie

### 27.1. <a id="stufe-1--design-proof--tests"></a>Stufe 1 — Design-Proof / Tests

Bevor persistentes Schema verändert wird, fokussierte Tests oder kleinen Proof erstellen für:

- bestehendes Alias-Invocation-Location-Verhalten,
- aktuelle Tag-Erkennung mit synthetischem effektiven Präfix,
- `-color off`,
- CE-Präfixkette bei `wrn:temperature:...`,
- Custom-`-ulabel`-Casing.

Diese Ergebnisse zur Entscheidung TIL-Metadatum vs. synthetischer Präfix nutzen.

### 27.2. <a id="stufe-2--alias-cli-parser-und-interne-repräsentation"></a>Stufe 2 — Alias-CLI-Parser und interne Repräsentation

Optionale Tag-Zuordnung hinzufügen und Legacy-Alias-Syntax vollständig erhalten.

Keine Namensheuristik.

### 27.3. <a id="stufe-3--bindinsertclean-integration"></a>Stufe 3 — Bind/Insert/Clean-Integration

Tag vor ID-Allokation und, soweit nötig, vor CE-Entscheidungen auflösen.

Source hinsichtlich Alias-Tag-Zuordnung stabil halten.

### 27.4. <a id="stufe-4--persistenz-und-logging"></a>Stufe 4 — Persistenz und Logging

Gewählte persistente Repräsentation implementieren und alle Host-Klassifikationsconsumer auf **einen** resolved Tag ausrichten.

### 27.5. <a id="stufe-5--diagnostik"></a>Stufe 5 — Diagnostik

Konservative, unterdrückbare Hinweise für irreführende Quellpräfixe ergänzen.

### 27.6. <a id="stufe-6--beispiele-und-doku"></a>Stufe 6 — Beispiele und Doku

Ein kleines hardwarefreies Beispiel ergänzen, idealerweise `PC_features` erweitern oder fokussiertes Alias-Beispiel anlegen:

```c
MyAlarm("temperature:Temperature too high: %d degrees", value);
```

mit:

```sh
-alias MyAlarm:wrn
-ce 'temperature:", limit={limit:%d}", temperatureLimit'
```

und JSON-Ausgabe mit:

- Warning-Tag/-Level,
- Datei/Zeile der Invocation,
- eigentlichem Wert,
- CE-Kontext.

Beispiel nicht von Runtime-Strings abhängig machen.

---

## 28. <a id="potenzielle-user-reference-manual-dokumentation--deutscher-entwurf"></a>Potenzielle User-/Reference-Manual-Dokumentation — deutscher Entwurf

Dieser Abschnitt ist bewusst weitgehend publizierbar formuliert. Vor Übernahme ins Repository entsprechend finalem Design anpassen und für die englische Projektdokumentation übersetzen.

### 28.1. <a id="tagged-trice-aliases"></a>Tagged Trice Aliases

#### 28.1.1. <a id="getaggte-aliase"></a>Getaggte Aliase

Projektspezifische Logging-Namen können als Trice-Aliase registriert werden. Einem Alias kann optional ein Anwendungstag zugeordnet werden, sodass dieser Tag nicht in jedem Formatstring wiederholt werden muss.

```sh
trice bind -alias MyAlarm:wrn -src app -til til.json -li li.json
```

Im Source genügt dann:

```c
MyAlarm("Temperature too high: %d degrees\n", temperature);
```

`MyAlarm` wird als normale Trice-Quellstelle behandelt, während der konfigurierte `wrn`-Tag das Ereignis der Warning-Gruppe zuordnet. Der Aliasname selbst besitzt keine besondere Bedeutung: Trice leitet den Severity-Level nicht aus Namen wie `Alarm`, `Error` oder `Debug` ab.

Der zugeordnete Tag ist Compile-Time-Metadatum. Er fügt dem Target-Record keinen zusätzlichen Laufzeitstring und keinen weiteren Laufzeitwert hinzu. Die normale Trice-ID identifiziert weiterhin die Quellposition über das passende `li.json`.

Bestehende Aliase ohne zugeordneten Tag behalten ihr bisheriges Verhalten:

```sh
trice bind -alias LegacyLog ...
```

### 28.2. <a id="tags-und-formatstring-selektoren-sind-unabhängig"></a>Tags und Formatstring-Selektoren sind unabhängig

Ein Alias-Tag verhindert nicht, dass ein Formatstring eigene Context-Enrichment-Selektoren verwendet:

```c
MyAlarm("temperature:Temperature too high: %d degrees\n", temperature);
```

```sh
trice bind \
  -alias MyAlarm:wrn \
  -ce 'temperature:", limit={limit:%d}", temperatureLimit' \
  -src app -til til.json -li li.json
```

Hier klassifiziert `wrn` das Ereignis, während `temperature:` zusätzlichen Kontext auswählt. Context Enrichment folgt seinen normalen Selektor- und Quelltransformationsregeln; der Alias-Tag erfordert kein literales `wrn:` im Source.

**Falls implementiert:** Ein zugeordneter Alias-Tag kann zusätzlich selbst als CE-Selektor dienen. Das ist nützlich für Kontext, der jeder Warning-Meldung hinzugefügt werden soll:

```sh
trice bind \
  -alias MyAlarm:wrn \
  -ce 'wrn:", task={task:%p}", osThreadGetId()' ...
```

### 28.3. <a id="quellposition"></a>Quellposition

Ein getaggter Alias bleibt eine normale instrumentierte Quellstelle. Seine Trice-ID wird über `li.json` aufgelöst; Datei und Zeile müssen deshalb nicht als Runtime-Strings übertragen werden.

Das unterscheidet sich von einer Hilfsfunktion mit einem gemeinsamen inneren Trice-Aufruf. Dort gehört die Trice-Quellstelle zum Funktionskörper und die Location identifiziert den Helper statt jeden Caller. Einen registrierten Alias verwenden, wenn die eigentliche Anwendungstelle ihre eigene Trice-ID/Location besitzen soll.

### 28.4. <a id="tag-artiger-text-innerhalb-eines-getaggten-alias"></a>Tag-artiger Text innerhalb eines getaggten Alias

Die finale Formulierung hängt vom Warning-Design ab.

Hat ein Alias bereits einen zugeordneten Tag, überschreibt ein tag-artiger Präfix im Sourceformat diese Zuordnung nicht. Beispiel:

```sh
-alias MyAlarm:wrn
```

Dieser Aufruf bleibt ein Warning-Ereignis:

```c
MyAlarm("err:Temperature too high: %d", temperature);
```

Trice kann einen Build-Hinweis ausgeben, weil der Text missverständlich wirkt. Der Hinweis kann unterdrückt werden, wenn das Präfix absichtlich verwendet wird. CE-Selektorbehandlung bleibt davon unabhängig.

### 28.5. <a id="custom-tags"></a>Custom Tags

Nur veröffentlichen, falls unterstützt:

```sh
trice bind \
  -ulabel sensor:650 \
  -alias SensorAlarm:sensor ...
```

Der Alias gehört zur `sensor`-Gruppe. Sein strukturierter Level wird aus dem konfigurierten Gewicht über die normalen Trice-Levelgrenzen abgeleitet.

### 28.6. <a id="laufzeitkontext"></a>Laufzeitkontext

Kontext, der sich zur Laufzeit ändert, sollte als Wert statt als wiederholter Text ergänzt werden. Zum Beispiel kann ein Projekt ausgewählte Ereignisse mit einem Task-Handle enrichern:

```sh
-ce 'ctx:", task={task:%p}", osThreadGetId()'
```

Der Task-Wert wird im selben Trice-Ereignis wie die normalen Meldungsargumente übertragen. Datei und Zeile bleiben Host-Metadaten aus Trice-ID und `li.json`.

### 28.7. <a id="runtime-strings-sind-keine-context-enrichment-werte"></a>Runtime-Strings sind keine Context-Enrichment-Werte

Tagged Aliases ändern Trices Runtime-String-Modell nicht. String- und Buffer-Familien behalten ihre bestehenden Einschränkungen; Context Enrichment hängt an solche Records nicht beliebig skalare Laufzeitwerte an. Für den hier beschriebenen Tagged-Alias-plus-Scalar-Context-Workflow normale skalare Trice-Aufrufe verwenden.

---

## 29. <a id="mögliche-spätere-antwort-auf-discussion-531--entwurf"></a>Mögliche spätere Antwort auf Discussion #531 — Entwurf

Noch nicht posten. Syntax und Aussagen nach Implementierung/Tests aktualisieren.

> Trice 2.0 deckt inzwischen den größten Teil dieses Use Cases anders ab als zum Zeitpunkt der Eröffnung dieser Discussion.
>
> Eine empfangene Trice-ID lässt sich über das passende `li.json` bereits auf Datei und Zeile abbilden. Daher müssen Quelldateinamen nicht als Runtime-Strings übertragen werden. Structured Logging kann jedes Ereignis als JSON/KV mit getrennten Feldern für Tag, Level, Meldung, Quellposition, Timestamps und benannte Werte exportieren. Context Enrichment kann Laufzeitwerte wie den aktuellen FreeRTOS-Task in dasselbe Ereignis aufnehmen.
>
> Für projektspezifische Logging-Namen ermöglichen getaggte Aliase einen Aufruf wie:
>
> ```c
> MyAlarm("temperature:Temperature too high: %d degrees", value);
> ```
>
> mit einer Konfiguration wie:
>
> ```sh
> -alias MyAlarm:wrn
> ```
>
> Dadurch wird der Alias als Warning klassifiziert, ohne `wrn:` in jedem Source-Formatstring wiederholen zu müssen. `temperature:` bleibt unabhängig davon als Context-Enrichment-Selektor nutzbar. Die Alias-Invocation besitzt die normale Trice-ID/Location — im Unterschied zu einer gemeinsamen Hilfsfunktion, deren innerer Trice-Aufruf den Helper-Body identifizieren würde.
>
> Runtime-Strings unterliegen weiterhin den bestehenden Trice-Einschränkungen. Der hier vorgesehene Workflow ist ein skalares Trice-Ereignis plus skalare Context-Enrichment-Werte.
>
> Bitte testen Sie dies mit einer v2-Version und den Structured-Logging-Beispielen. Besonders interessant wäre, ob Ihr Hostprogramm JSON/KV direkt konsumieren kann oder weiterhin das bisherige Markerlayout benötigt.

---

## 30. <a id="verworfene-bzw-zurückgestellte-lösungsansätze"></a>Verworfene bzw. zurückgestellte Lösungsansätze

Nur als Historie behalten; nicht implementieren, sofern keine neue konkrete Anforderung entsteht.

### 30.1. <a id="separate-stabile-source-file-ids--filesjson"></a>Separate stabile Source-File-IDs / `files.json`

Verworfen, weil Trice-ID + `li.json` Quellposition bereits liefert.

### 30.2. <a id="bind-sidecar-file-keys-übertragen-1"></a>Bind-Sidecar-File-Keys übertragen

Aus demselben Grund verworfen; unnötiger Target-Payload.

### 30.3. <a id="dateinamen-zeichen-in-uint64_t-ce-werte-packen"></a>Dateinamen-Zeichen in `uint64_t`-CE-Werte packen

Technisch machbar, aber verschwenderisch und für statische Source-Location unnötig.

### 30.4. <a id="severity-automatisch-aus-aliasnamen-ableiten"></a>Severity automatisch aus Aliasnamen ableiten

Verworfen. Explizites CLI-Mapping ist klarer, deterministisch und projektunabhängig.

### 30.5. <a id="allgemeine-log_error-hilfsfunktion-als-hauptlösung"></a>Allgemeine `log_error()`-Hilfsfunktion als Hauptlösung

Für manche Anwendungen valide, aber ungeeignet, wenn LI jede Anwendungscall-Site identifizieren soll.

### 30.6. <a id="mehrere-trice-aufrufe-für-kontext--meldung"></a>Mehrere Trice-Aufrufe für Kontext + Meldung

Für normalen skalaren Fall nicht mehr bevorzugt, da CE und Structured Logging logischen Kontext in einem Event halten können. Multi-Call-Komposition bringt zusätzlich Ordering-/Präemptionsfragen zurück.

### 30.7. <a id="runtime-string--beliebige-skalare-ce-jetzt-lösen"></a>Runtime-String + beliebige skalare CE jetzt lösen

Out of scope. Darf Alias-Tag nicht blockieren.

---

## 31. <a id="akzeptanzkriterien-der-finalen-funktion"></a>Akzeptanzkriterien der finalen Funktion

Die Arbeit ist erfolgreich, wenn alle folgenden Punkte erfüllt sind:

1. Bestehendes `-alias Name` verhält sich unverändert.
2. Ein Projekt kann einen Tag explizit einem Alias zuordnen, bevorzugt über die bestehende Option, z. B. `-alias MyAlarm:wrn`.
3. Keine Aliasnamens-Heuristik existiert.
4. Source-Formatstrings müssen den zugeordneten Tag nicht enthalten.
5. Alias-Tag-Zuordnung selbst schreibt den Benutzerformatstring nicht um.
6. Ereignis-Tag ist persistent genug, um gespeicherte Captures ohne Wiederholung der ursprünglichen Alias-CLI korrekt zu dekodieren.
7. Änderung des zugeordneten Tags kann historische IDs nicht still umdeuten.
8. Zugeordneter Tag wirkt konsistent auf Filter, Structured Output, Farben, Statistik und tag-spezifische ID-Policies.
9. Direkte Alias-Invocation behält korrekte LI-Call-Site.
10. CE-Selektoren im Source bleiben unabhängig von Alias-Tag-Klassifikation.
11. Falls angenommen, kann Alias-Tag CE-Regeln auswählen, ohne doppelte Anwendung.
12. Konfligierender tag-artiger Quellpräfix überschreibt Alias-Tag nicht; optional unterdrückbare Diagnostik.
13. `-ulabel`-Integration ist definiert und getestet statt parallel dupliziert.
14. Für reine Alias-Tag-Klassifikation entstehen keine neuen Target-Laufzeitbytes.
15. Kein neues Trice-Wire-Format erforderlich.
16. Runtime-Strings über CE bleiben ausdrücklich out of scope.
17. Dokumentation enthält vollständiges #531-nahes Beispiel.
18. Tests decken Bind und, soweit unterstützt, Insert/Clean-Roundtrip sowie historische ID-Semantik ab.

---

## 32. <a id="quellen--geprüfte-basis"></a>Quellen / geprüfte Basis

Externer Kontext:

- Discussion #531: https://github.com/rokath/trice/discussions/531
- Release `v2.0.0-beta-1`: https://github.com/rokath/trice/releases/tag/v2.0.0-beta-1

Relevante v2.0.0-beta-1-Dokumentation:

- `docs/rm/bind.md`
- `docs/rm/cli.md`
- `docs/rm/context_enrichment.md`
- `docs/rm/examples.md`
- `docs/rm/id_management.md`
- `docs/rm/legacy_user_code.md`
- `docs/rm/structured_logging.md`
- `docs/rm/tags_color_and_weights.md`

Während der Designanalyse relevante Implementierungsbereiche:

- `internal/id/id.go`
- Datei mit `resolveTriceAlias` / `ProcessAliases`
- `internal/id/contextEnrichment.go`
- `internal/translator/structured.go`
- `internal/translator/translator.go`
- `internal/args/init.go`
- `internal/id/bindAdvanced.go`
- Alias-/CE-Tests unter `internal/id` und `internal/args`

Außerdem Repository-Agent-Regeln berücksichtigen:

- `AGENTS.md`

---

## 33. <a id="abschließende-arbeitsanweisung-an-codex"></a>Abschließende Arbeitsanweisung an Codex

Vom aktuellen Repositoryzustand ausgehen, aber `v2.0.0-beta-1` als in diesem Handoff beschriebene Verhaltensbasis verwenden. Zuerst prüfen, ob `main` bereits Änderungen an Alias-/Tag-/CE-Interna enthält.

**Nicht sofort `Tag` zu `til.json` hinzufügen.** Zunächst kleinen Design-Proof bauen und explizite Metadaten gegen synthetische effektive Formatpräfixe vergleichen, besonders bezüglich:

- `-color off`,
- Custom-`-ulabel`-Casing,
- historischen IDs,
- CE-Selektorketten,
- Kompatibilität alter/neuer Dictionaries.

Danach die gewählte Architektur kurz festhalten und begründen, warum sie einfacher/sicherer ist, bevor breit geändert wird.

Das beabsichtigte User-Modell soll einfach bleiben:

```sh
-alias MyAlarm:wrn
```

```c
MyAlarm("temperature:Temperature too high: %d degrees", value);
```

Trice soll daraus liefern:

- Warning-Klassifikation,
- normale Call-Site-ID und `li.json`-Location,
- optionalen skalaren CE-Laufzeitkontext,
- strukturierten Host-Output,
- keine wiederholte Tag-Zeichenkette im Source,
- keine Dateinamenübertragung zur Laufzeit,
- keine Aliasnamens-Heuristik.

Implementierung fokussiert halten. **Nicht** daraus eine allgemeine Metadatensprache oder Runtime-String-Neugestaltung machen, solange keine konkrete Anforderung das erzwingt.

---

## 34. <a id="designnachtrag-tag-transport-und-kompakte-location-historie"></a>Designnachtrag: Tag-Transport und kompakte Location-Historie

**Arbeitsstand:** Analyse auf `wip`, Commit `bdda0a9d` (`alias tag mapping ideas added DE`). Dieser Nachtrag enthält Empfehlungen zur nächsten Designentscheidung, keine freigegebene Implementierung. Der Auftrag beschränkt sich auf diesen Anhang und die ToC-Aktualisierung. Die älteren Abschnitte bleiben als Diskussionsstand erhalten; insbesondere ihre Vorschläge zu `-ulabel` bei Bind sind zusammen mit der folgenden Präzisierung zu lesen.

### 34.1. <a id="empfehlung-und-gewünschte-zuständigkeiten"></a>Empfehlung und gewünschte Zuständigkeiten

**Empfehlung:** Bind speichert den zugeordneten Ereignis-Tag je ID in der TIL. `tlog` beziehungsweise `trice log` liest diese Identität automatisch. Farben, Gewichte und Filter bleiben Host-Konfiguration. Für Alias-Tags ist weder eine zweite Zuordnungsdatei noch eine Änderung des Target-Datenstroms erforderlich. Eine echte LI-Historie sollte unabhängig davon entschieden werden.

| Information | Zuständigkeit | Zum späteren Logger übertragen? |
|---|---|---|
| Aliasname und seine Erkennung im Quelltext | Bind; entsprechend bestehende Insert-/Clean-Wege | Nein, der aufgelöste Tag reicht. |
| Ereignis gehört zur Warning-Gruppe oder zur Kategorie `sensor` | Instrumentierung legt die Identität fest | Ja, pro ID im Dictionary. |
| Tag-spezifischer ID-Bereich | Bind-/ID-Policy | Für Host-Filter nicht nötig. Der Tag wird nicht aus dem Bereich rekonstruiert. |
| Gewicht, Farbe, Auswahl durch `-loglevel`, `-pick`, `-ban` | Host-Auswertung | Kein Rücktransport von Bind; bei Bedarf gemeinsames Logging-Skript archivieren. |
| Datei und Zeile | Instrumentierung, abhängig vom Quellstand | Optional über passende LI; Tag-Filter müssen auch mit `-li off` funktionieren. |

Die gewünschte Trennung ist sinnvoll. **Im aktuellen Code ist `-ulabel` allerdings noch nicht ausschließlich Präsentation:** Es registriert auch Namen, wird bei Bind/Insert/Clean angeboten und beeinflusst über Gewichte den strukturierten Level sowie Filter. `-ulabel` ist die derzeitige Schreibweise; `-logLevel` und `-loglevel` sind beide registriert. Siehe [CLI-Registrierung](../../internal/args/init.go), [Kommandoinitialisierung](../../internal/args/handler.go) und [Tag-Registry](../../internal/emitter/lineTransformerANSI.go).

Für das Zielmodell ist deshalb genauer: **Ereignisidentität gehört zum Build; ihre Bewertung und Darstellung gehören zum Host.** Neue explizite Alias-Tags sollten keine Farbangabe oder Gewichtsregistrierung bei Bind benötigen. Bestehende Bind-Aufrufe mit `-ulabel` bleiben aus Kompatibilitätsgründen gültig; ihr Entfernen ist kein Bestandteil dieses Vorschlags.

### 34.2. <a id="warum-die-tag-information-den-build-überleben-muss"></a>Warum die Tag-Information den Build überleben muss

Angenommen, zwei Aliase verwenden denselben effektiven Typ und denselben Meldungstext, einer mit `wrn`, einer mit `err`. Im Binärrecord steht die ID, aber kein Aliasname. Die heutige TIL speichert `Type` und `Strg`; `Alias` trägt ausdrücklich `json:"-"`. Ohne weitere Zuordnung kann der Logger diese beiden Klassifikationen nicht aus dem Meldungstext ableiten. Siehe [persistente Typen](../../internal/id/id.go).

Auch identische CLI-Einstellungen auf beiden Seiten lösen das allein nicht: `MyAlarm -> wrn` beantwortet noch nicht `empfangene ID -> MyAlarm`. Ein Skript müsste zusätzlich diese Zuordnung erzeugen und bewahren. Damit hätte es faktisch bereits ein weiteres Dictionary gebaut.

```text
Bind: Alias + Quellaufruf -> ID + effektives Format + Ereignis-Tag -> TIL
Target:                     ID + Laufzeitwerte                 -> Datenstrom
Host: Datenstrom + TIL -> Ereignis-Tag -> Host-Gewicht -> Filter und Ausgabe
```

Keine zusätzliche Information ist nötig, wenn der Tag bereits eindeutig im gespeicherten Format steht und diese bisherige Präfixsemantik gewünscht ist. Für einen unabhängig vom Format zugeordneten Alias-Tag ist die persistente Zuordnung dagegen notwendig. Sie kostet keine zusätzlichen Target-Bytes und keine Tag-Berechnung im Interrupt.

### 34.3. <a id="vergleich-der-transportwege"></a>Vergleich der Transportwege

| Variante | Benutzung und Historie | Bewertung |
|---|---|---|
| Gleiche Alias-CLI oder gemeinsames Skript | Gut für reproduzierbare Build-Eingaben, aber ohne ID-Zuordnung unzureichend zum Dekodieren. Spätere Skriptänderungen dürfen alte Captures nicht umdeuten. | Ergänzung für Konfiguration, kein Metadatentransport. |
| Optionale Metadaten je TIL-Eintrag | Der Benutzer übergibt weiterhin dieselbe TIL. Historische ID und Tag bleiben zusammen. | Bevorzugt. |
| Zusätzliche Tag-JSON, indiziert nach ID | Bestehende TIL bleibt unverändert; neue und alte Werkzeuge können getrennte Dateien verwenden. Zusätzlicher Pfad, Archivierung und Konsistenzprüfung werden nötig. | Sinnvoll, falls unveränderte TIL zwingend ist; sonst mehr Fehlerquellen. |
| Tag in LI | Erspart eine weitere Datei, koppelt aber Filter an optionale und buildabhängige Quellpositionen. | Nicht empfehlen. |
| Synthetisches Tag-Präfix in `Strg` | Alte Logger verstehen bekannte Tags bereits. Unter `-color off` wird das Präfix sichtbar; Formattext und Klassifikation bleiben gekoppelt. | Möglicher Legacy-Export, nicht bevorzugtes Hauptformat. |
| Neues Gesamtformat mit Version, TIL, LI und gemeinsamen Tabellen | Gut für globale Metadaten und Kompression. Alte Leser erwarten aber eine ID-Map und scheitern. | Erst bei einem eigenständigen Bedarf an einem neuen Archivformat. |
| ID-Bereiche, erneutes Quellscannen oder zusätzliche Target-Felder | Benötigt historische Policy/Quellen oder erhöht den Target-Aufwand. Bereiche können sich ändern. | Keine einfachere Lösung. |

Falls eine separate Tag-Datei gewählt würde, müsste sie automatisch und gemeinsam mit der TIL entstehen. Sie sollte einen überprüfbaren Bezug auf deren ID-/Schema-Zuordnungen haben; ein bloß gleicher Dateiname genügt nicht. Fehlende oder widersprüchliche Tag-Daten dürften bei angeforderter Alias-Filterung nicht still als `untagged` gelten. Diese Zusatzarbeit entfällt weitgehend, wenn die Informationen im selben TIL-Eintrag liegen.

### 34.4. <a id="vorschlag-für-eine-erweiterbare-til"></a>Vorschlag für eine erweiterbare TIL

Die äußere Struktur bleibt `ID -> Eintrag`. `Type` und `Strg` behalten Namen, Datentyp und bisherige Bedeutung. Hinzu kommt nur bei Bedarf ein optionales, versioniertes `Meta`-Objekt. Folgende Schreibweise ist ein **Schema-Vorschlag**, noch keine implementierte Schnittstelle:

```json
{
  "1234": {
    "Type": "trice",
    "Strg": "Temperature too high: %d degrees",
    "Meta": {
      "Version": 1,
      "Tag": {"Kind": "builtin", "Name": "WARNING"}
    }
  },
  "1235": {
    "Type": "trice",
    "Strg": "Voltage: %d mV",
    "Meta": {
      "Version": 1,
      "Tag": {"Kind": "user", "Name": "sensor"}
    }
  },
  "1236": {
    "Type": "trice",
    "Strg": "inf:Existing format-prefix behavior"
  }
}
```

Ein flaches optionales `Tag`-Stringfeld wäre die kleinste Änderung. Das kleine `Meta`-Objekt ist meine Empfehlung für die ausdrücklich gewünschte Zukunftsoffenheit: Es bietet einen abgegrenzten Erweiterungsbereich und eine Versionsgrenze für neue Leser. `Kind` unterscheidet eingebaute von benutzerdefinierten Namen, damit ein später neu eingeführter Built-in-Name keine historische Custom-Kategorie übernimmt. Dies bleibt ein Dateidetail; die CLI bleibt `-alias MyAlarm:wrn`.

Der Preis sind zusätzliche Host-Dateibytes und mehr Reader-/Writer-Arbeit. Das rechtfertigt weder eine globale Tag-Tabelle noch weitere vorsorgliche Metadatenfelder. Der Target-Record bleibt gleich groß.

Vorgeschlagener Vertrag:

1. **Ohne `Meta.Tag`:** bisherige Klassifikation aus dem gespeicherten Präfix, sonst `untagged`. Keine automatische Umschreibung alter Einträge.
2. **Mit `Meta.Tag`:** diese Identität ist maßgeblich, auch bei einem abweichenden Präfix in `Strg`. Das Format wird allein deshalb weder ergänzt noch gekürzt. Explizites `untagged` unterscheidet sich von fehlenden Metadaten.
3. **Kanonisierung:** Built-in-Aliase wie `wrn`, `warn` und `WARNING` werden vor der ID-Vergabe zu derselben Identität. Custom-Namen behalten ihre exakte Schreibweise. Aliasnamen selbst brauchen nicht in die TIL.
4. **Keine Host-Policy im Eintrag:** keine Farbe, kein CLI-Filter, kein aus dem aktuellen Gewicht abgeleiteter `Level`. Ein Tagwechsel ist eine Build-Änderung; ein anderes Host-Gewicht ist es nicht.
5. **Version:** `Meta.Version` bezeichnet das Metadatenschema, nicht die Toolversion. Unbekannte optionale Informationen innerhalb einer unterstützten Version dürfen ignoriert, müssen bei erneutem Schreiben aber erhalten werden. Neue Pflichtsemantik erfordert eine neue Version. Nicht unterstützte Versionen dürfen neue Tools nicht still falsch klassifizieren oder umschreiben.
6. **Keine Sonderobjekte auf Dateiebene:** weder `_meta`, `Version`, `Tags` noch ein künstlicher ID-0-Eintrag neben echten IDs. Die alten Reader erwarten numerische IDs; Bind erkennt Sekundärdateien zusätzlich anhand positiver ID-Schlüssel und ihrer Eintragsfelder. Siehe [JSON-Leser](../../internal/id/manage.go) und `classifyBindMetadataJSON` in [Bind-Metadaten](../../internal/id/bindMetadata.go).
7. **Unbekannte Felder bewahren:** Künftige Writer müssen Erweiterungen verlustfrei durchreichen, soweit sie deren Version sicher bearbeiten können. Erst normalisieren und dann nur bekannte Struct-Felder zurückschreiben genügt nicht. Bei unklarer Pflichtsemantik ist ein verständlicher Abbruch besser als Datenverlust.

Diese Regeln machen additive Erweiterungen möglich. Sie garantieren ausdrücklich nicht, dass ein altes Werkzeug beliebige neue Semantik versteht.

### 34.5. <a id="custom-tags-gewichte-und-ein-verständlicher-ablauf"></a>Custom-Tags, Gewichte und ein verständlicher Ablauf

Für Built-in-Tags genügt später die TIL: `WARNING` hat am Host sein bestehendes Standardgewicht. Ein Benutzer kann dieses dort mit `-ulabel` ändern. Für Custom-Tags braucht es eine explizite Defaultregel, weil der Name `sensor` allein keinen Schweregrad festlegt.

**Empfehlung für Custom-Tags:** Ein ausdrücklich in TIL gespeicherter Custom-Tag ist ohne erneute Registrierung benutzbar. Ohne Host-Override erhält er das finale INFO-Gewicht, entsprechend dem heutigen Verhalten eines gewichtslos registrierten User Labels. `-ulabel sensor:650` ändert am Host seine Bewertung, nicht die in TIL gespeicherte Identität. Das sollte auch für `-pick sensor`, Statistik und JSON/KV gelten.

Konzeptioneller Zielablauf; die `Name:Tag`-Alias-Erweiterung ist noch nicht implementiert:

```sh
trice bind -alias MyAlarm:wrn -src app -til til.json -li li.json
tlog -til til.json -loglevel wrn
```

Für eine eigene Kategorie:

```sh
trice bind -alias SensorAlarm:sensor -src app -til til.json -li li.json
tlog -til til.json -ulabel sensor:650 -loglevel 600
```

Die üblichen Eingabe-/Portoptionen kommen beim Logging hinzu. Der zweite Logger braucht keine Alias-Einstellung. Ohne `-ulabel sensor:650` gilt der dokumentierte INFO-Default; unter einer Warning-Schwelle kann das Ereignis dann ausgeblendet sein. Eine projektspezifische historische Gewichtung ist nur mit archivierter Host-Konfiguration reproduzierbar. Falls stattdessen jeder Firmwarestand einen unveränderlichen Custom-Schweregrad garantieren soll, wäre dessen Speicherung eine **zusätzliche semantische Designentscheidung**.

Zwei Fallstricke für die spätere Umsetzung:

- TIL-Tags müssen für Filter verfügbar sein, bevor ein Eingabekanal geöffnet wird. Heute validiert `runLog` die Filter vor dem eigentlichen Log-Loop. Auch beim automatischen Nachladen der TIL müssen neue Tags, Gewichte und Filter konsistent aktualisiert werden; siehe [Initialisierung](../../internal/args/handler.go) und [Datei-Watcher](../../internal/id/fileWatcher.go).
- Das Bekanntwerden eines expliziten Custom-Tags darf alte Einträge ohne Metadaten nicht nachträglich anders interpretieren. Ein bislang gewöhnlicher Text `sensor:ready` darf nicht allein durch einen neuen Alias-Eintrag zum Tag werden. Explizite Tag-Auflösung und Legacy-Präfixerkennung brauchen diese Unterscheidung, auch wenn sie dieselbe Registry verwenden.

### 34.6. <a id="historische-identität-und-gemeinsame-auswertung"></a>Historische Identität und gemeinsame Auswertung

Ein Wechsel von Warning zu Error darf eine bereits gespeicherte ID nicht umdeuten. Bind muss dafür eine passende andere historische ID verwenden oder eine neue vergeben. Ein bloßer Schreibweisenwechsel `wrn -> WARNING` darf dagegen keinen Wechsel erzwingen. Dasselbe gilt bei zwei gleichlautenden Meldungen mit verschiedenen expliziten Tags.

Die Vergleichsidentität muss deshalb effektiven Typ, effektives Format und die Tag-Semantik enthalten. Dabei ist auch der Modus **Legacy-Präfix versus explizite Zuordnung** relevant: Derselbe sichtbare Tag kann unterschiedliche Behandlung des Meldungstexts bedeuten. Farbe, Host-Gewicht, Aliasname, Dateizeile und JSON-Feldreihenfolge gehören nicht in diesen Schlüssel.

Heute wird `TriceFmt` selbst als Go-Map-Schlüssel verwendet. Ein verschachteltes persistentes Metadatenobjekt darf nicht einfach samt Maps, Zeigern oder unbekannten Feldern zum neuen Gleichheitsmaß werden. Ein kleiner, ausdrücklich definierter semantischer Vergleichsschlüssel ist nötig. Prüfen muss man neben Allocation auch Bind-Reuse, Sekundär-TILs und die Umsortierung identischer Aufrufe; siehe [Reverse-Lookup](../../internal/id/manage.go), [Bind-Metadaten](../../internal/id/bindMetadata.go) und [ID-Reihenfolge](../../internal/id/orderedIDs.go).

Der Decoder sollte danach eine eindeutige Tag-Identität und getrennt die Information liefern, ob überhaupt ein entfernbares Textpräfix vorliegt. Nur `record.Tag` anders zu setzen reicht nicht: [Structured Output](../../internal/translator/structured.go) und [Textausgabe](../../internal/translator/translator.go) entfernen beziehungsweise interpretieren bislang Präfixe. Filter, Statistik, Farben und JSON/KV müssen dieselbe Identität verwenden, ohne echten Meldungstext versehentlich zu entfernen.

### 34.7. <a id="kompatibilität-lesen-bedeutung-und-schreiben-unterscheiden"></a>Kompatibilität: Lesen, Bedeutung und Schreiben unterscheiden

Die folgende Einschätzung beruht auf den gelesenen aktuellen Reader-/Writer-Pfaden, **nicht auf ausgeführten Tests verschiedener Release-Binaries**. Bei strukturell unveränderten `Type`-/`Strg`-Einträgen ignoriert der verwendete Go-JSON-Decoder unbekannte Struct-Felder; siehe [Go-Dokumentation zu Unmarshal](https://pkg.go.dev/encoding/json#Unmarshal). Ein altes Tool erhält dadurch aber keine Kenntnis ihrer Bedeutung.

| Kombination | Erwartbares Verhalten und Grenze |
|---|---|
| Neues Tool, alte TIL ohne Metadaten | Bisherige Präfix-/`untagged`-Semantik bleibt erhalten. Diese Erweiterung hebt sonstige historische Template-Grenzen nicht auf. |
| Neues Tool, erweiterte TIL | Explizite Tags steuern Filter, Statistik und Ausgabe; Host-Policy ergänzt Gewichte und Farben. |
| Bisheriger Logger, erweiterte TIL | `Type` und `Strg` bleiben lesbar. Alias-Tag wird ignoriert: das Ereignis gilt als `untagged` oder sogar als die andere Gruppe aus einem echten Textpräfix. Korrekte Filterung ist nicht zugesichert. |
| Bisheriger Bind-/Insert-/Generate-Writer, erweiterte TIL | Bei erneutem Serialisieren gehen unbekannte Felder verloren. Auch ID-Wiederverwendung kann verschiedene Tag-Semantiken zusammenfassen. Nicht auf der maßgeblichen erweiterten TIL arbeiten lassen. |
| Bisheriger Logger, ausdrücklich erzeugte Legacy-Kopie | Bekannte Tag-Präfixe können die Klassifikation vermitteln. Sichtbarer Text, Custom-Labels und Präfixkonflikte brauchen dokumentierte Grenzen. |

**Die entscheidende Grenze:** Unverändertes `Strg`, unveränderter alter Logger und volle neue Alias-Semantik lassen sich nicht gleichzeitig garantieren. Ein zusätzliches JSON-Feld kann einem alten Programm kein neues Filterverhalten beibringen. Auch ein Versionsfeld innerhalb von `Meta` kann alte Writer nicht am Ignorieren hindern.

Falls alte Logger weiter zuverlässig filtern müssen, ist ein **bewusster Legacy-Export** die verständlichste Ausweichmöglichkeit: Für eine separate Kopie ein passendes synthetisches Präfix vor das effektive Format setzen. Die maßgebliche TIL und die IDs bleiben dabei unverändert. Built-in-Tags sind am einfachsten; Custom-Tags benötigen im alten Host weiter passende `-ulabel`-Einstellungen. Unter `-color off` erscheint das zusätzliche Präfix. Reine Alias-Metadaten dürfen durch diesen Export keine neue Payload-Semantik erhalten.

Diese Kopie ist ein verlustbehaftetes Ausgabeformat für alte Logger, keine neue Arbeits-TIL. Sie gehört in ein separates Export-/Releaseverzeichnis außerhalb der Bind-Metadatensuche, damit zwei verschiedene `Strg`-Darstellungen derselben ID nicht als konkurrierende Build-Dictionaries eingelesen werden. Ein Exportbefehl wird hier weder implementiert noch endgültig benannt.

Wenn dauerhaft **alte und neue schreibende Werkzeuge** dieselben Arbeitsdateien bearbeiten müssen, ist das eine stärkere Anforderung als alte Dateien lesen zu können. Dann wären getrennte Arbeitsstände oder eine zusätzliche Metadatendatei erneut abzuwägen; auch eine solche Datei müsste gegen veraltete ID-Zuordnungen abgesichert werden.

### 34.8. <a id="li-sammelt-bereits-ids-aber-keine-vollständige-positionshistorie"></a>LI sammelt bereits IDs, aber keine vollständige Positionshistorie

Im aktuellen Code werden neu ermittelte Positionen in die vorhandene LI eingetragen. Nicht erneut gefundene IDs bleiben dabei grundsätzlich erhalten; für dieselbe ID wird die bisherige Position ersetzt. Das gilt für [Bind](../../internal/id/bindIDs.go) und die [gemeinsame Nachverarbeitung](../../internal/id/switchIDs.go). Die [Bind-Dokumentation](../rm/bind.md#hierarchical-metadata-reuse) beschreibt ausdrücklich eine Position je ID.

Deshalb müssen zwei Wünsche unterschieden werden:

| Gewünschte Akkumulation | Heutiger Stand / Konsequenz |
|---|---|
| Alte IDs mit ihrer zuletzt bekannten Position aufbewahren | Bereits grundsätzlich vorhanden. Kompaktere Speicherung ist davon unabhängig. |
| Jede frühere Position derselben ID je Firmwarestand wiederfinden | Noch nicht abgebildet. Benötigt eine Zuordnung zum Build oder zur Aufnahme. |

Beispiel: ID `1234` gehört in Firmware A zu `motor.c:42`, in Firmware B nach eingefügten Quellzeilen zu `motor.c:55`. Eine Liste beider Positionen sagt dem Logger noch nicht, welche richtig ist. Eine Zeitangabe oder „zuletzt gesehen“ löst das bei parallelen Branches und alten Geräten nicht. Auch identische Aufrufe können durch die vorhandenen ID-Regeln neu zugeordnet werden.

**Keine neue Trice-ID allein für jede Zeilenverschiebung vergeben.** Das würde Format-/Ereignisidentität mit Quellposition koppeln und den begrenzten ID-Raum schneller verbrauchen. Für exakte historische Locations stattdessen den passenden Build auswählen. Das kann ohne neue Target-Payload durch die Wahl des archivierten Metadatensatzes oder durch einen Aufnahmebegleiter geschehen. Gemischte Firmwarestände in einer Aufnahme benötigen entsprechend getrennte Sitzungen oder eine zusätzliche eindeutige Zuordnung.

### 34.9. <a id="platzsparende-li-kleiner-schritt-und-spätere-historie"></a>Platzsparende LI: kleiner Schritt und spätere Historie

**Kleiner Schritt ohne Schemaänderung:** ein kompakter Eintrag pro Zeile, weiterhin mit `File` und `Line`. Dafür existiert bereits `generate -onelineJSON`; es erzeugt abgeleitete Dateien und stellt die maßgebliche LI nicht automatisch auf ein anderes Schreibformat um. Siehe [Generator](../../internal/id/generate.go). Das ist eine Formatierungsfrage, noch keine neue Historienfunktion.

Ein reiner Größenvergleich im Speicher für [`_test/testdata/testLI.json`](../../_test/testdata/testLI.json), mit 2.329 IDs und 16 verschiedenen Pfaden:

| Darstellung | Bytes | Aussage |
|---|---:|---|
| Vorliegende Datei | 168.499 | Ausgangspunkt im geprüften Checkout. |
| Ein kompakter Eintrag pro Zeile, nach dem Layout des vorhandenen Generators | 145.209 | Etwa 13,8 % kleiner; gleiches JSON-Schema. |
| Vollständig kompaktes JSON | 138.221 | Weniger angenehm für Diffs. |
| Experimentelle gemeinsame Pfadtabelle plus `ID -> [Pfadindex, Zeile]` | 39.474 | Etwa 76,6 % kleiner, aber inkompatibles Schema und noch ohne Build-Historie. |

Diese Zahlen sind eine Layoutrechnung an einer Fixture, kein implementierter Kompressor und keine allgemeine Einspargarantie. Sie zeigen: Das größere Potenzial liegt in den wiederholten Pfaden. Git-/Archivkompression kann diese Wiederholung ebenfalls reduzieren, ohne das Arbeitsformat zu ändern.

**Falls eine echte Historie gewünscht wird:** vorzugsweise ein optionales, versioniertes Host-Archiv, vorläufig `li-history.json` genannt. Die gewöhnliche LI bleibt die einfache Sicht eines ausgewählten Builds für vorhandene Tools. Ein mögliches logisches Schema:

```json
{
  "Format": "trice-li-history",
  "Version": 1,
  "Files": ["src/motor.c"],
  "Locations": [[0, 42], [0, 55]],
  "Snapshots": {
    "firmware-a": {"1234": 0},
    "firmware-b": {"1234": 1}
  }
}
```

Hier enthalten `Locations` Paare aus Pfadindex und Zeile; die Werte in `Snapshots` verweisen auf diese Positionen. Es handelt sich ausschließlich um Host-Dateiindizes, nicht um neue File-IDs im Target-Protokoll. Dateinamen und Schema sind noch nicht festgelegt.

Für eine spätere Spezifikation sind folgende Regeln wichtiger als ein möglichst kurzes JSON-Beispiel:

- Jeder Pfad und jedes Positionspaar wird nur einmal gespeichert. Gleiche vollständige Zuordnungen können von mehreren Builds gemeinsam genutzt werden. Stabile Indizes vermeiden eine Neunummerierung des gesamten Archivs bei jeder Ergänzung.
- Ein Snapshot enthält die tatsächliche aktive Zuordnung des Builds. Die kumulative Arbeits-LI kann auch alte, inzwischen inaktive IDs enthalten und ist dafür allein kein verlässliches Aktivitätsverzeichnis. Partielle Bind-Scans dürfen nicht als vollständiger neuer Build ausgegeben werden.
- Ein unveränderlicher Build-Bezug muss Firmware und relevante Metadaten zusammenführen. Ein Git-Commit allein reicht bei lokalen Änderungen oder verschiedenen Build-Optionen nicht. Da Bind vor dem Linken läuft, wird ein fertiger Firmware-Hash erst im abschließenden Build-/Archivschritt zugeordnet.
- Ohne passenden Build darf der Logger bei mehreren möglichen Positionen nicht einfach die neueste als historisch korrekt ausgeben. Eindeutige Auswahl verlangen oder die Location mit verständlichem Hinweis weglassen.
- Unabhängige ID-Dictionaries beziehungsweise kollidierende Branch-Historien müssen getrennte Namensräume behalten. Dieselbe Zahl ist nicht projektübergreifend dieselbe Trice-ID.
- Historische Snapshots bleiben unverändert. Pfadwurzeln und Normalisierung müssen festgelegt sein; ein Rechnerwechsel darf keine neuen Hostpfade in dieselbe Historie hineinmischen.
- Ein gewählter Snapshot lässt sich wieder in eine gewöhnliche `li.json` mit `File` und `Line` exportieren. Alte Tools können diese Sicht weiterhin nutzen; sie können das kompakte Archiv nicht direkt lesen.
- Delta-Ketten über viele Builds, automatische Bereinigung und ein kombiniertes TIL-/LI-Archiv zunächst zurückstellen. Vollständige logische Snapshots mit geteilten Tabellen sind verständlicher; weitere Verdichtung erst anhand echter Archivgrößen entscheiden.

Ein solcher Host-Archivpfad eröffnet später Platz für Build-bezogene Metadaten, ohne die einfache TIL-Erweiterung oder Alias-Tags davon abhängig zu machen. Falls hingegen ausdrücklich **eine einzige kompakte neue `li.json`** statt einer zusätzlichen Archivdatei gewünscht wird, ist das ein bewusster Formatwechsel: neue Tools brauchen beide Reader, alte Tools eine exportierte Legacy-Sicht. Beides gleichzeitig in einer alten ID-Map unterzubringen würde Daten duplizieren oder alte Leser täuschen.

### 34.10. <a id="konkreter-entscheidungsstand-für-die-weiterarbeit"></a>Konkreter Entscheidungsstand für die Weiterarbeit

Die derzeitige Empfehlung lautet:

1. Alias-Tags explizit und historisch stabil pro ID in einer optional erweiterten TIL erhalten; keine Wiederholung der Bind-Alias-CLI beim Logging verlangen.
2. Die vorhandene äußere TIL-Struktur erhalten; den kleinen versionierten `Meta`-Bereich als bevorzugten Schemaentwurf prüfen. Bestehende Einträge ohne Metadaten behalten ihre Semantik.
3. Gewicht und Farbe bleiben Host-Policy. Explizite Custom-Tags erhalten ohne Override den dokumentierten INFO-Default. Für identische historische Ausgabe die Host-Konfiguration mit archivieren.
4. Neue Tools müssen alte Dateien weiter akzeptieren. Alte Logger können neue Zusatzfelder ignorieren, aber die neue Semantik nur über einen gesonderten Legacy-Export erhalten. Alte Writer dürfen erweiterte Arbeitsdateien nicht still zerstören; deren tatsächlicher weiterer Einsatz muss vor Implementierung geklärt werden.
5. LI-Kompaktheit und vollständige LI-Historie getrennt entscheiden. Für Alias-Tags ist beides keine Voraussetzung. Bei echter Historie ein optionales Host-Archiv mit Build-Auswahl und exportierbarer klassischer LI bevorzugen.

**Vor Implementierung am anderen Rechner noch mit dem Nutzer entscheiden:** Reicht alte Tools weiterhin zum Lesen einzusetzen, oder müssen sie die neuen Arbeitsdateien auch schreiben können? Ist der vorgeschlagene `Meta`-Bereich gegenüber dem flachen `Tag`-Feld erwünscht? Bedeutet „akkumulierende LI“ nur kompaktere letzte Positionen je ID oder exakte Positionen über mehrere Builds? Diese Analyse legt dafür eine Empfehlung vor und ersetzt die Zustimmung zu diesen wesentlichen Entscheidungen nicht.

Danach zuerst kleine Kompatibilitäts- und Verhaltenstests planen: alte/neue Reader und Writer, Alias-Tagwechsel, gleichlautende Formate verschiedener Tags, kollidierende Textpräfixe, Custom-Default und Host-Override, TIL-Nachladen, unbekannte Metadaten, Sekundär-Dictionaries sowie unveränderte Legacy-Einträge. Für eine separat beauftragte LI-Historie zusätzlich zwei Builds mit gleicher ID an verschiedenen Zeilen und die Rückgabe einer alten LI testen. Aktuell wurden nur Code und Dokumentation gelesen sowie die genannten Größen im Speicher berechnet; kein Laufzeitcode, kein Dictionary und kein Buildskript wurde geändert.

## Fragen und Notizen

- Vorschlag für das Reference Manual
Ich würde im Kapitel Location Information lediglich einen kurzen Abschnitt ergänzen:
Archiving location information
Location information is build-specific. Users who need to resolve source locations from older firmware should preserve the corresponding li.json.
A simple approach is to calculate a content hash (for example SHA-256) of the final li.json during the build and make this hash available from the device. This can be done through startup logging, periodic output, a firmware information command, a device label, or any other suitable mechanism.
Historical LI files can be archived under their content hashes, including in ZIP archives. Given a device's hash, any external utility can locate the corresponding li.json.
Trice itself requires no special support for this workflow. The selected file can be passed through the existing -li option.

- -uLabel help bei trice log|bind|insert|clean mit identischem CLI Hilfetext ist irritierend. 1) Wozu braucht trice clean -ulabel? 2) Bei bind/insert wird -uLabel doch nur benötigt, wenn die ID-Vergabe gesteuert werden soll, oder? Gewicht und Farbe sind doch bei bind/insert sinnlos. 3) Andere CLI Schalter sind in ihrer Verwendung und damit Doku identisch wenn in unterschiedlichen Kommandos verwendet (vermute ich mal). Bei -uLabel ist das anders. Derzeit wird -uLabel zB für bind und log gleichermaßen im CLI dokumentiert, was m. E. falsch ist. 4) Sollte evtl. ein anderer Name verwendet werden, zB -uTag? Welcher dann besser wo? Noch bessser: -alias SensorAlarm:sensor -> "sensor:" wird implizit als Tag erzeugt und -uLabel gar nicht für bind benötigt. 5) Wäre eiune generelle umbenennung von -uLabel in uTag besser? Jetzt noch vor dem 2.0.0 Release? Generell verstehe ich jetzt Tag als ein von mehreren vergebbaren pro Trice Message und Label als einen einzigen Label pro TRice Message. Ist das eine korreckte Sichtweise?
- Die til.json Erweiterung wird eigentlich nur gebraucht, wenn zb bind -alias verwendet, evtl. sogar nur wenn -alias mit Tag verwendet wird. Dann wäre diese Erweiterung für viele User zunächst gar nicht sichtbar. Den Vorschlag zur til Erweiterung akzeptiere ich.
- Die "Meta": Option ließe auch zu, etwa "Created", und/oder "Used" Zeiten mitzuführen um til.json ggf später mit einem Tool von Altlasten zu befreien, etwa "Alles was 1 Jahr nicht benutzt wurde entfernen". Würde ich aber jetzt noch nicht einbauen.
- Version in Meta hat nichts der aktuellen Toolversion zu tun, das ist klar. Aber vielleicht sollte mit 2 oder sogar 2.0.0 angefangen werden um Missverständnisse zu vermeiden. Dann würde hier die Mindestversion stehen um das jeweilige Meta Feld schreibend zu supporten.
- Alte Tools sollen nur lesen und ignorieren können. Die neuen Versionen sind immer frei verfügbar, nur ggf. noch nicht installiert, etwa auf einem Server.
