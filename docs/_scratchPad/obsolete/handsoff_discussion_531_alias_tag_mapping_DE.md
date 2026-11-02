
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
  - [6.2. Bedeutung von `Name:Tag`](#bedeutung-von-nametag)
  - [6.3. Keine Semantik aus Aliasnamen ableiten](#keine-semantik-aus-aliasnamen-ableiten)
  - [6.4. Formatstrings bleiben Eigentum des Quellcodes und werden durch Alias-Tag-Zuordnung nicht umgeschrieben](#formatstrings-bleiben-eigentum-des-quellcodes-und-werden-durch-alias-tag-zuordnung-nicht-umgeschrieben)
- [7. Alias-Tag und CE-Selektor sind getrennte Dimensionen](#alias-tag-und-ce-selektor-sind-getrennte-dimensionen)
  - [7.1. Vorhandene CE-Selektorregeln wiederverwenden](#vorhandene-ce-selektorregeln-wiederverwenden)
  - [7.2. Offene Entscheidung: Soll der Alias-Tag selbst CE-Regeln auswählen?](#offene-entscheidung-soll-der-alias-tag-selbst-ce-regeln-auswählen)
- [8. Was passiert bei einem tag-artigen Präfix im Alias-Format?](#was-passiert-bei-einem-tag-artigen-präfix-im-alias-format)
  - [8.1. Zu analysierendes Warnverhalten](#zu-analysierendes-warnverhalten)
- [9. Zentrale offene Architekturfrage: Wo lebt der zugeordnete Tag?](#zentrale-offene-architekturfrage-wo-lebt-der-zugeordnete-tag)
  - [9.1. Warum Persistenz nötig ist](#warum-persistenz-nötig-ist)
  - [9.2. Variante A: optionales Tag-Metadatum in `til.json`](#variante-a-optionales-tag-metadatum-in-tiljson)
    - [9.2.1. Vorteile](#vorteile)
    - [9.2.2. Nachteile / notwendige Arbeit](#nachteile--notwendige-arbeit)
    - [9.2.3. Kritische ID-Historienregel](#kritische-id-historienregel)
  - [9.3. Variante B: synthetischen Tag nur dem effektiven Dictionary-Format voranstellen](#variante-b-synthetischen-tag-nur-dem-effektiven-dictionary-format-voranstellen)
    - [9.3.1. Vorteile](#vorteile-1)
    - [9.3.2. Nachteile / Risiken](#nachteile--risiken)
  - [9.4. Variante C: dieselbe Alias-/Tag-CLI auch beim Logging verlangen](#variante-c-dieselbe-alias-tag-cli-auch-beim-logging-verlangen)
    - [9.4.1. Vorteil](#vorteil)
    - [9.4.2. Nachteile](#nachteile)
  - [9.5. Variante D: `-ulabel` / `untagged` ohne explizite neue Metadaten wiederverwenden](#variante-d--ulabel--untagged-ohne-explizite-neue-metadaten-wiederverwenden)
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
  - [12.1. Rückwärtskompatibilität für `untagged`](#rückwärtskompatibilität-für-untagged)
- [13. Location Information: kein neuer File-ID-Mechanismus](#location-information-kein-neuer-file-id-mechanismus)
  - [13.1. Separates `files.json`](#separates-filesjson)
  - [13.2. Bind-Sidecar-File-Keys übertragen](#bind-sidecar-file-keys-übertragen)
  - [13.3. `__FILE__` in mehrere Integerwerte packen](#__file__-in-mehrere-integerwerte-packen)
- [14. Caller-Location: Alias vs. Helper-Funktion vs. Wrapper](#caller-location-alias-vs-helper-funktion-vs-wrapper)
  - [14.1. Direkter Trice-Aufruf oder registrierter Alias](#direkter-trice-aufruf-oder-registrierter-alias)
  - [14.2. Normale Hilfsfunktion](#normale-hilfsfunktion)
  - [14.3. Allgemeines Logging-Wrapper-Makro mit inneren Trice-Aufrufen](#allgemeines-logging-wrapper-makro-mit-inneren-trice-aufrufen)
- [15. Laufzeitkontext aus #531](#laufzeitkontext-aus-531)
  - [15.1. Task-Kontext](#task-kontext)
  - [15.2. Timestamp](#timestamp)
  - [15.3. MODULE / PROCESS](#module--process)
- [16. Runtime-Strings: ausdrückliches Nicht-Ziel](#runtime-strings-ausdrückliches-nicht-ziel)
- [17. Mögliche Verhaltensmatrix für `-alias Name:Tag`](#mögliche-verhaltensmatrix-für--alias-nametag)
- [18. Kompatibilitäts- und Migrationsanforderungen](#kompatibilitäts--und-migrationsanforderungen)
  - [18.1. Bestehende CLI muss unverändert funktionieren](#bestehende-cli-muss-unverändert-funktionieren)
  - [18.2. Alte TIL -> neues Tool](#alte-til---neues-tool)
  - [18.3. Neue TIL -> altes Tool](#neue-til---altes-tool)
  - [18.4. Historische kumulative TIL](#historische-kumulative-til)
  - [18.5. ID-Änderungen](#id-änderungen)
- [19. Wechselwirkung mit `-ulabel`](#wechselwirkung-mit--ulabel)
- [20. Wechselwirkung mit tag-spezifischen ID-Ranges / Target Routing](#wechselwirkung-mit-tag-spezifischen-id-ranges--target-routing)
- [21. Präsentation und `-color`](#präsentation-und--color)
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
  - [24.12. `-ulabel`](#-ulabel)
  - [24.13. ID Range / Routing](#id-range--routing)
  - [24.14. Rückwärtskompatibilität](#rückwärtskompatibilität)
  - [24.15. `-salias`](#-salias)
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
  - [30.1. Separate stabile Source-File-IDs / `files.json`](#separate-stabile-source-file-ids--filesjson)
  - [30.2. Bind-Sidecar-File-Keys übertragen](#bind-sidecar-file-keys-übertragen-1)
  - [30.3. Dateinamen-Zeichen in `uint64_t`-CE-Werte packen](#dateinamen-zeichen-in-uint64_t-ce-werte-packen)
  - [30.4. Severity automatisch aus Aliasnamen ableiten](#severity-automatisch-aus-aliasnamen-ableiten)
  - [30.5. Allgemeine `log_error()`-Hilfsfunktion als Hauptlösung](#allgemeine-log_error-hilfsfunktion-als-hauptlösung)
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
- [35. Fragen und Notizen](#fragen-und-notizen)
- [36. Antworten auf Fragen und Notizen](#antworten-auf-fragen-und-notizen)
- [37. Umsetzungsspezifikation zur Freigabe](#umsetzungsspezifikation-zur-freigabe)
  - [37.1. Ergebnis aus Benutzersicht](#ergebnis-aus-benutzersicht)
  - [37.2. CLI und Tag-Begriffe](#cli-und-tag-begriffe)
  - [37.3. CE, Meldungstext und Hinweise](#ce-meldungstext-und-hinweise)
  - [37.4. TIL-Schema und historische IDs](#til-schema-und-historische-ids)
  - [37.5. Host-Auswertung und Kompatibilität](#host-auswertung-und-kompatibilität)
  - [37.6. Dokumentation und Umfang](#dokumentation-und-umfang)
  - [37.7. Abnahmekriterien](#abnahmekriterien)
- [38. Rückfragen](#rückfragen)
- [39. Antworten auf die Rückfragen](#antworten-auf-die-rückfragen)
  - [39.1. Alter von `-ulabel` und vollständige Umbenennung](#alter-von--ulabel-und-vollständige-umbenennung)
  - [39.2. Neue User-Tags direkt durch `-alias`](#neue-user-tags-direkt-durch--alias)
  - [39.3. Zusätzliche CE-Selektoren an einem Alias](#zusätzliche-ce-selektoren-an-einem-alias)
  - [39.4. `-alias Name` gegenüber `-alias Name:untagged`](#-alias-name-gegenüber--alias-nameuntagged)
  - [39.5. Gewicht und Farbe bei Build-Kommandos](#gewicht-und-farbe-bei-build-kommandos)
  - [39.6. Englischer Feldname `SchemaVersion`](#englischer-feldname-schemaversion)
  - [39.7. Automatische Tag-Erkennung im Aliasnamen](#automatische-tag-erkennung-im-aliasnamen)
  - [39.8. Aliasname oder Tag in `Type` statt `Meta`](#aliasname-oder-tag-in-type-statt-meta)
  - [39.9. Wann die TIL-Erweiterung tatsächlich entsteht](#wann-die-til-erweiterung-tatsächlich-entsteht)
- [40. Feedback](#feedback)
- [41. Bewertung des Feedbacks](#bewertung-des-feedbacks)
  - [41.1. Groß- und Kleinschreibung bei CLI-Optionen](#groß--und-kleinschreibung-bei-cli-optionen)
  - [41.2. Empfehlung für die weitere CLI-Gestaltung](#empfehlung-für-die-weitere-cli-gestaltung)
  - [41.3. Warum auch automatische Alias-Tags übertragen werden müssen](#warum-auch-automatische-alias-tags-übertragen-werden-müssen)
  - [41.4. Konkrete Teilwort-Treffer aus der vorhandenen Tag-Tabelle](#konkrete-teilwort-treffer-aus-der-vorhandenen-tag-tabelle)
  - [41.5. Eine einfache Namenskonvention ist vertretbar](#eine-einfache-namenskonvention-ist-vertretbar)
  - [41.6. Separate `aliasTags.json` oder Vertagung](#separate-aliastagsjson-oder-vertagung)
- [42. Names-Heuristik](#names-heuristik)
- [43. Bewertung der Namens-Heuristik](#bewertung-der-namens-heuristik)
  - [43.1. Der Aliasname genügt als gespeicherte Grundlage](#der-aliasname-genügt-als-gespeicherte-grundlage)
  - [43.2. Die Unterscheidung zwischen skalarem Alias und String-Alias](#die-unterscheidung-zwischen-skalarem-alias-und-string-alias)
  - [43.3. Was „unbekannter Type“ künftig bedeutet](#was-unbekannter-type-künftig-bedeutet)
  - [43.4. Die wichtigste zusätzliche Schwachstelle ist die Versionsstabilität](#die-wichtigste-zusätzliche-schwachstelle-ist-die-versionsstabilität)
  - [43.5. Der Aliasname wird bei einfacher Umsetzung Teil der ID-Identität](#der-aliasname-wird-bei-einfacher-umsetzung-teil-der-id-identität)
  - [43.6. Deine Regel für `MyWrn("err:bla")` ist richtig](#deine-regel-für-mywrnerrbla-ist-richtig)
  - [43.7. Auswirkungen auf die vorhandene Verarbeitung](#auswirkungen-auf-die-vorhandene-verarbeitung)
- [44. Implementationsspezifikation für Alias-Namensheuristik und CLI](#implementationsspezifikation-für-alias-namensheuristik-und-cli)
  - [44.1. Umfang und Ergebnis aus Anwendersicht](#umfang-und-ergebnis-aus-anwendersicht)
  - [44.2. Begriffe und getrennte Zuständigkeiten](#begriffe-und-getrennte-zuständigkeiten)
  - [44.3. CLI-Namen und Groß-/Kleinschreibung](#cli-namen-und-groß-kleinschreibung)
  - [44.4. Zulässige Werte von `-utag`](#zulässige-werte-von--utag)
  - [44.5. Registrierung und Prüfung der Aliasnamen](#registrierung-und-prüfung-der-aliasnamen)
  - [44.6. Vollständig festgelegte Namensheuristik](#vollständig-festgelegte-namensheuristik)
  - [44.7. TIL-Darstellung und interne Darstellung](#til-darstellung-und-interne-darstellung)
  - [44.8. `trice` und `triceS`: zuerst beim Instrumentieren unterscheiden](#trice-und-trices-zuerst-beim-instrumentieren-unterscheiden)
  - [44.9. `trice` und `triceS`: später aus der TIL unterscheiden](#trice-und-trices-später-aus-der-til-unterscheiden)
  - [44.10. Grenzen der Typunterscheidung](#grenzen-der-typunterscheidung)
  - [44.11. Verarbeitung in Bind, Insert, Clean und Generate](#verarbeitung-in-bind-insert-clean-und-generate)
  - [44.12. ID-Identität, Historie und alte Dateien](#id-identität-historie-und-alte-dateien)
  - [44.13. CE-Auswahl und ihre Grenze bei String-Aliasen](#ce-auswahl-und-ihre-grenze-bei-string-aliasen)
  - [44.14. Host-Klassifikation und unveränderter Meldungstext](#host-klassifikation-und-unveränderter-meldungstext)
  - [44.15. Diagnostik](#diagnostik)
  - [44.16. Verbindliche Verhaltenstests](#verbindliche-verhaltenstests)
  - [44.17. Verbindliche Dokumentation](#verbindliche-dokumentation)
  - [44.18. Noch zu prüfende Detailentscheidungen](#noch-zu-prüfende-detailentscheidungen)

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

| Source Call                            | Ereignis-Tag | Semantik des Formattexts                                                             | Mögliche Build-Diagnostik             |
|----------------------------------------|--------------|--------------------------------------------------------------------------------------|---------------------------------------|
| `MyAlarm("Too hot %d", v)`             | Warning      | unverändert                                                                          | keine                                 |
| `MyAlarm("temperature:Too hot %d", v)` | Warning      | `temperature:` steht CE zur Verfügung; normale CE-Entfernungsregeln gelten bei Match | keine                                 |
| `MyAlarm("err:Too hot %d", v)`         | Warning      | `err:` überschreibt Alias-Tag nicht; bleibt Text/Selektorinput                       | unterdrückbare Warnung empfohlen      |
| `MyAlarm("wrn:Too hot %d", v)`         | Warning      | Quelltext wird durch Alias-Zuordnung nicht umgeschrieben                             | optional Redundanzhinweis oder keiner |
| `MyAlarm("INFO:Too hot %d", v)`        | Warning      | Quellpräfix überschreibt Alias-Tag nicht                                             | unterdrückbare Warnung empfohlen      |

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

| Information                                                    | Zuständigkeit                                    | Zum späteren Logger übertragen?                                                 |
|----------------------------------------------------------------|--------------------------------------------------|---------------------------------------------------------------------------------|
| Aliasname und seine Erkennung im Quelltext                     | Bind; entsprechend bestehende Insert-/Clean-Wege | Nein, der aufgelöste Tag reicht.                                                |
| Ereignis gehört zur Warning-Gruppe oder zur Kategorie `sensor` | Instrumentierung legt die Identität fest         | Ja, pro ID im Dictionary.                                                       |
| Tag-spezifischer ID-Bereich                                    | Bind-/ID-Policy                                  | Für Host-Filter nicht nötig. Der Tag wird nicht aus dem Bereich rekonstruiert.  |
| Gewicht, Farbe, Auswahl durch `-loglevel`, `-pick`, `-ban`     | Host-Auswertung                                  | Kein Rücktransport von Bind; bei Bedarf gemeinsames Logging-Skript archivieren. |
| Datei und Zeile                                                | Instrumentierung, abhängig vom Quellstand        | Optional über passende LI; Tag-Filter müssen auch mit `-li off` funktionieren.  |

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

| Variante                                                          | Benutzung und Historie                                                                                                                                             | Bewertung                                                                |
|-------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------|
| Gleiche Alias-CLI oder gemeinsames Skript                         | Gut für reproduzierbare Build-Eingaben, aber ohne ID-Zuordnung unzureichend zum Dekodieren. Spätere Skriptänderungen dürfen alte Captures nicht umdeuten.          | Ergänzung für Konfiguration, kein Metadatentransport.                    |
| Optionale Metadaten je TIL-Eintrag                                | Der Benutzer übergibt weiterhin dieselbe TIL. Historische ID und Tag bleiben zusammen.                                                                             | Bevorzugt.                                                               |
| Zusätzliche Tag-JSON, indiziert nach ID                           | Bestehende TIL bleibt unverändert; neue und alte Werkzeuge können getrennte Dateien verwenden. Zusätzlicher Pfad, Archivierung und Konsistenzprüfung werden nötig. | Sinnvoll, falls unveränderte TIL zwingend ist; sonst mehr Fehlerquellen. |
| Tag in LI                                                         | Erspart eine weitere Datei, koppelt aber Filter an optionale und buildabhängige Quellpositionen.                                                                   | Nicht empfehlen.                                                         |
| Synthetisches Tag-Präfix in `Strg`                                | Alte Logger verstehen bekannte Tags bereits. Unter `-color off` wird das Präfix sichtbar; Formattext und Klassifikation bleiben gekoppelt.                         | Möglicher Legacy-Export, nicht bevorzugtes Hauptformat.                  |
| Neues Gesamtformat mit Version, TIL, LI und gemeinsamen Tabellen  | Gut für globale Metadaten und Kompression. Alte Leser erwarten aber eine ID-Map und scheitern.                                                                     | Erst bei einem eigenständigen Bedarf an einem neuen Archivformat.        |
| ID-Bereiche, erneutes Quellscannen oder zusätzliche Target-Felder | Benötigt historische Policy/Quellen oder erhöht den Target-Aufwand. Bereiche können sich ändern.                                                                   | Keine einfachere Lösung.                                                 |

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

| Kombination                                              | Erwartbares Verhalten und Grenze                                                                                                                                                                     |
|----------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Neues Tool, alte TIL ohne Metadaten                      | Bisherige Präfix-/`untagged`-Semantik bleibt erhalten. Diese Erweiterung hebt sonstige historische Template-Grenzen nicht auf.                                                                       |
| Neues Tool, erweiterte TIL                               | Explizite Tags steuern Filter, Statistik und Ausgabe; Host-Policy ergänzt Gewichte und Farben.                                                                                                       |
| Bisheriger Logger, erweiterte TIL                        | `Type` und `Strg` bleiben lesbar. Alias-Tag wird ignoriert: das Ereignis gilt als `untagged` oder sogar als die andere Gruppe aus einem echten Textpräfix. Korrekte Filterung ist nicht zugesichert. |
| Bisheriger Bind-/Insert-/Generate-Writer, erweiterte TIL | Bei erneutem Serialisieren gehen unbekannte Felder verloren. Auch ID-Wiederverwendung kann verschiedene Tag-Semantiken zusammenfassen. Nicht auf der maßgeblichen erweiterten TIL arbeiten lassen.   |
| Bisheriger Logger, ausdrücklich erzeugte Legacy-Kopie    | Bekannte Tag-Präfixe können die Klassifikation vermitteln. Sichtbarer Text, Custom-Labels und Präfixkonflikte brauchen dokumentierte Grenzen.                                                        |

**Die entscheidende Grenze:** Unverändertes `Strg`, unveränderter alter Logger und volle neue Alias-Semantik lassen sich nicht gleichzeitig garantieren. Ein zusätzliches JSON-Feld kann einem alten Programm kein neues Filterverhalten beibringen. Auch ein Versionsfeld innerhalb von `Meta` kann alte Writer nicht am Ignorieren hindern.

Falls alte Logger weiter zuverlässig filtern müssen, ist ein **bewusster Legacy-Export** die verständlichste Ausweichmöglichkeit: Für eine separate Kopie ein passendes synthetisches Präfix vor das effektive Format setzen. Die maßgebliche TIL und die IDs bleiben dabei unverändert. Built-in-Tags sind am einfachsten; Custom-Tags benötigen im alten Host weiter passende `-ulabel`-Einstellungen. Unter `-color off` erscheint das zusätzliche Präfix. Reine Alias-Metadaten dürfen durch diesen Export keine neue Payload-Semantik erhalten.

Diese Kopie ist ein verlustbehaftetes Ausgabeformat für alte Logger, keine neue Arbeits-TIL. Sie gehört in ein separates Export-/Releaseverzeichnis außerhalb der Bind-Metadatensuche, damit zwei verschiedene `Strg`-Darstellungen derselben ID nicht als konkurrierende Build-Dictionaries eingelesen werden. Ein Exportbefehl wird hier weder implementiert noch endgültig benannt.

Wenn dauerhaft **alte und neue schreibende Werkzeuge** dieselben Arbeitsdateien bearbeiten müssen, ist das eine stärkere Anforderung als alte Dateien lesen zu können. Dann wären getrennte Arbeitsstände oder eine zusätzliche Metadatendatei erneut abzuwägen; auch eine solche Datei müsste gegen veraltete ID-Zuordnungen abgesichert werden.

### 34.8. <a id="li-sammelt-bereits-ids-aber-keine-vollständige-positionshistorie"></a>LI sammelt bereits IDs, aber keine vollständige Positionshistorie

Im aktuellen Code werden neu ermittelte Positionen in die vorhandene LI eingetragen. Nicht erneut gefundene IDs bleiben dabei grundsätzlich erhalten; für dieselbe ID wird die bisherige Position ersetzt. Das gilt für [Bind](../../internal/id/bindIDs.go) und die [gemeinsame Nachverarbeitung](../../internal/id/switchIDs.go). Die [Bind-Dokumentation](../rm/bind.md#hierarchical-metadata-reuse) beschreibt ausdrücklich eine Position je ID.

Deshalb müssen zwei Wünsche unterschieden werden:

| Gewünschte Akkumulation                                          | Heutiger Stand / Konsequenz                                                   |
|------------------------------------------------------------------|-------------------------------------------------------------------------------|
| Alte IDs mit ihrer zuletzt bekannten Position aufbewahren        | Bereits grundsätzlich vorhanden. Kompaktere Speicherung ist davon unabhängig. |
| Jede frühere Position derselben ID je Firmwarestand wiederfinden | Noch nicht abgebildet. Benötigt eine Zuordnung zum Build oder zur Aufnahme.   |

Beispiel: ID `1234` gehört in Firmware A zu `motor.c:42`, in Firmware B nach eingefügten Quellzeilen zu `motor.c:55`. Eine Liste beider Positionen sagt dem Logger noch nicht, welche richtig ist. Eine Zeitangabe oder „zuletzt gesehen“ löst das bei parallelen Branches und alten Geräten nicht. Auch identische Aufrufe können durch die vorhandenen ID-Regeln neu zugeordnet werden.

**Keine neue Trice-ID allein für jede Zeilenverschiebung vergeben.** Das würde Format-/Ereignisidentität mit Quellposition koppeln und den begrenzten ID-Raum schneller verbrauchen. Für exakte historische Locations stattdessen den passenden Build auswählen. Das kann ohne neue Target-Payload durch die Wahl des archivierten Metadatensatzes oder durch einen Aufnahmebegleiter geschehen. Gemischte Firmwarestände in einer Aufnahme benötigen entsprechend getrennte Sitzungen oder eine zusätzliche eindeutige Zuordnung.

### 34.9. <a id="platzsparende-li-kleiner-schritt-und-spätere-historie"></a>Platzsparende LI: kleiner Schritt und spätere Historie

**Kleiner Schritt ohne Schemaänderung:** ein kompakter Eintrag pro Zeile, weiterhin mit `File` und `Line`. Dafür existiert bereits `generate -onelineJSON`; es erzeugt abgeleitete Dateien und stellt die maßgebliche LI nicht automatisch auf ein anderes Schreibformat um. Siehe [Generator](../../internal/id/generate.go). Das ist eine Formatierungsfrage, noch keine neue Historienfunktion.

Ein reiner Größenvergleich im Speicher für [`_test/testdata/testLI.json`](../../_test/testdata/testLI.json), mit 2.329 IDs und 16 verschiedenen Pfaden:

| Darstellung                                                                 |   Bytes | Aussage                                                                      |
|-----------------------------------------------------------------------------|--------:|------------------------------------------------------------------------------|
| Vorliegende Datei                                                           | 168.499 | Ausgangspunkt im geprüften Checkout.                                         |
| Ein kompakter Eintrag pro Zeile, nach dem Layout des vorhandenen Generators | 145.209 | Etwa 13,8 % kleiner; gleiches JSON-Schema.                                   |
| Vollständig kompaktes JSON                                                  | 138.221 | Weniger angenehm für Diffs.                                                  |
| Experimentelle gemeinsame Pfadtabelle plus `ID -> [Pfadindex, Zeile]`       |  39.474 | Etwa 76,6 % kleiner, aber inkompatibles Schema und noch ohne Build-Historie. |

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

## 35. <a id="fragen-und-notizen"></a>Fragen und Notizen

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

## 36. <a id="antworten-auf-fragen-und-notizen"></a>Antworten auf Fragen und Notizen

**LI-Archivierung:** Dein kurzer RM-Abschnitt reicht. Das vorgeschlagene Trice-eigene Historienarchiv würde ich damit zurückstellen. Wichtig ist nur: den Hash nach dem letzten Bind-/Insert-Lauf aus genau der archivierten LI bilden und beim Einbetten keine erneute LI-Änderung verursachen. Eine ZIP-Datei muss vor Verwendung mit `-li` entpackt werden.

**Wozu `-ulabel` außerhalb von Log?** Die Registrierung eines Namens hat heute folgende Aufgaben; Gewicht und Farbe werden für ID-Vergabe und CE nicht benötigt:

| Kommando          | Verwendung des registrierten Namens                                                                                                                                            |
|-------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `bind` / `insert` | Tag-spezifische ID-Bereiche **und CE-Präfixbehandlung**. Ein registrierter Tag bleibt im effektiven Format erhalten; ein passender freier lowercase CE-Selektor wird entfernt. |
| `clean`           | Bei `-ce` dieselbe Interpretation wie Insert, damit der TIL-Abgleich stimmt. Ohne CE ist `-ulabel` zum bloßen Entfernen der IDs nicht nötig.                                   |
| `log`             | Tag-Erkennung, Gewicht, Filter, Level und Farbe.                                                                                                                               |

Das ist in [CE-Auswahl](../../internal/id/contextEnrichment.go) und [Clean](../../internal/id/cleanIDs.go) nachvollziehbar. Die gemeinsame Hilfe sollte deshalb je Kommando präzisiert werden.

**Name und Begriffe:** Ich empfehle `-utag` als einheitlichen Hauptnamen und `-ulabel` als weiterhin gültigen Alias. Das wäre vor 2.0 sinnvoll; unterschiedliche Namen je Kommando würden die Sache nicht vereinfachen. Aktuell hat eine Meldung **einen Ereignis-Tag**. „User Label“ bezeichnet einen registrierten Tag-Namen, keine eigene Kategorie. Mehrere CE-Präfixe sind Selektoren, keine mehreren Ereignis-Tags.

**Implizite Registrierung durch Alias:** Ja: `-alias SensorAlarm:sensor` sollte `sensor` selbst bekannt machen, sodass dafür bei Bind kein zusätzliches `-ulabel` nötig ist. Dabei entsteht eine Tag-Zuordnung, **kein eingefügter Text `sensor:`**. Normale Aufrufe mit Custom-Präfixen ohne Alias brauchen für tag-spezifische ID-Bereiche weiterhin eine Registrierung.

**TIL-Erweiterung:** Als angenommen festgehalten. Für diese Funktion wird `Meta.Tag` nur bei Aliasen mit expliziter Tag-Zuordnung gebraucht. `-alias Name` und bisherige normale Aufrufe bleiben ohne diesen Zusatz. Das gilt entsprechend auch für Insert; Bind allein ist nicht die Grenze.

**`Created` / `Used`:** Zurückgestellt. Solche Angaben dürfen später die ID-Identität nicht verändern. „Beim Bind zuletzt gesehen“ bedeutet außerdem nicht „auf Geräten nicht mehr benutzt“; Alter allein wäre kein sicherer Löschgrund.

**Metadatenversion:** Ich würde das Feld zur Klarheit `SchemaVersion` nennen und mit `1` beginnen. `2` wäre möglich, aber nur eine andere Startnummer. Eine Tool-Mindestversion ist eine getrennte Angabe, etwa `MinWriterVersion`, falls später nötig. `2.0.0` dafür erst verwenden, wenn diese Toolversion das Schema tatsächlich unterstützt. Eine Schemaänderung soll keinen Tool-Versionswechsel vortäuschen und umgekehrt.

**Alte Tools:** Als Entscheidung festgehalten: Lesen und Ignorieren genügt; Schreiben neuer Dateien ist nicht gefordert. Damit entfällt ein verpflichtender Legacy-Export. Die bereits beschriebenen [Filtergrenzen alter Logger](#kompatibilität-lesen-bedeutung-und-schreiben-unterscheiden) bleiben bestehen.

## 37. <a id="umsetzungsspezifikation-zur-freigabe"></a>Umsetzungsspezifikation zur Freigabe

**Status:** vollständiger Entwurf des geplanten Ergebnisses, noch keine Freigabe zur Implementierung. Dieser Abschnitt führt die Entscheidungen zusammen und ersetzt abweichende Vorschläge und offene Alternativen der vorherigen Abschnitte. Bestätigt sind insbesondere CE-Auswahl über den Alias-Tag, getaggte String-Aliase und Konflikthinweise ausschließlich bei `-verbose`.

### 37.1. <a id="ergebnis-aus-benutzersicht"></a>Ergebnis aus Benutzersicht

Ein Projekt kann einen Alias einem Ereignis-Tag zuordnen, ohne den Formatstring entsprechend zu ändern:

```c
MyAlarm("Temperature too high: %d degrees", temperature);
```

```sh
trice bind -alias MyAlarm:wrn -src app -til til.json -li li.json
tlog -til til.json -li li.json -loglevel wrn -logFormat json
```

Die üblichen Eingabe-/Portoptionen kommen hinzu. Bei Standardgewichten und `temperature == 85` erhält der Host `tag=WARNING`, `level=WARNING` und `message="Temperature too high: 85 degrees"`; die Location bezeichnet die Alias-Aufrufstelle. Der Logger benötigt die Alias-CLI nicht. Es entstehen keine zusätzlichen Target-Payload-Bytes.

### 37.2. <a id="cli-und-tag-begriffe"></a>CLI und Tag-Begriffe

| Option                                 | Festgelegtes Verhalten                                                                                                                         |
|----------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------|
| `-alias Name` / `-salias Name`         | Bisheriges Verhalten ohne explizite Tag-Zuordnung.                                                                                             |
| `-alias Name:Tag` / `-salias Name:Tag` | Explizite Tag-Zuordnung für skalare beziehungsweise String-Aliase. Entsprechende Kurzoptionen `-a` / `-sa` akzeptieren dieselbe Syntax.        |
| `-utag`                                | Neuer Hauptname überall dort, wo bisher `-ulabel` angeboten wird; bestehende Werteformen `name`, `name:weight`, `name:color` bleiben erhalten. |
| `-ulabel`                              | Weiterhin gültiger Alias für `-utag`; beide Schreibweisen lassen sich gemeinsam wiederholen.                                                   |

- Eine Meldung besitzt einen Ereignis-Tag. Mehrere CE-Selektoren sind keine zusätzlichen Ereignis-Tags. Aliasnamen erhalten keine Bedeutung durch Namensheuristiken.
- Built-in-Schreibweisen werden kanonisiert (`wrn`, `warn`, `WARNING` ergeben `WARNING`). Custom-Namen bleiben schreibungsabhängig. Ein gültiger neuer Name in `Name:Tag` wird implizit registriert; ein zusätzliches `-utag` ist dafür nicht nötig. Explizites `untagged` ist zulässig.
- Leere Namen/Tags, ungültige Syntax und widersprüchliche Mehrfachdefinitionen je Alias-Art werden vor Änderungen abgewiesen. Identische, kanonisch gleichwertige Definitionen sind zulässig; `Name` und `Name:Tag` für dieselbe Alias-Art gelten als widersprüchlich. Die bestehende Unterscheidung zwischen skalarem und String-Alias bei gleichem Namen bleibt erhalten.
- Registrierung und Auflösung erfolgen vor CE und ID-Vergabe. Die Reihenfolge von Alias-, Tag- und ID-Bereichsoptionen darf ihre Zuordnung nicht beeinflussen; die bestehende Reihenfolge expliziter Gewicht-/Farb-Overrides bleibt erhalten.

Die CLI-Hilfe wird nach Aufgabe getrennt:

| Kommando          | Erklärung zu `-utag`                                                                                       |
|-------------------|------------------------------------------------------------------------------------------------------------|
| `log` / `tlog`    | Namen registrieren, Gewichte/Farben setzen; Auswirkung auf Filter und Level erklären.                      |
| `bind` / `insert` | Namen für ID-Bereiche und CE-Präfixbehandlung registrieren. Gewicht/Farbe steuern dabei weder IDs noch CE. |
| `clean`           | Bei CE dieselbe Namensregistrierung wie Insert verwenden; für reines ID-Entfernen nicht nötig.             |

Historisch akzeptierte Gewicht-/Farbangaben bei ID-Kommandos bleiben gültig, erzeugen aber keine neuen Build-Metadaten. Die neue Alias-Syntax gilt auch für die entsprechenden Alias-Optionen von `generate`.

### 37.3. <a id="ce-meldungstext-und-hinweise"></a>CE, Meldungstext und Hinweise

1. Zuerst werden passende CE-Regeln des expliziten Alias-Tags ausgewählt, danach die Regeln der Quellpräfixe in deren bisheriger Reihenfolge.
2. Jede kanonische Selektorgruppe wird höchstens einmal angewendet. Innerhalb einer Gruppe bleibt die CLI-Reihenfolge der Regeln erhalten. Wird dieselbe Gruppe über Alias und Quellpräfix erreicht, entstehen keine doppelten Kontextwerte.
3. Bestehende CE-Regeln zur Präfixbehandlung und Argumentprüfung bleiben gültig. Ein Alias-Tag wird nicht als Textpräfix eingefügt. Die Erweiterung beseitigt keine bisherigen Wrapper-, Record- oder Argumentgrenzen.
4. `-salias Name:Tag` unterstützt die Klassifikation. Zusätzliche skalare CE-Werte an Laufzeitstrings bleiben ausgeschlossen; unzulässige Kombinationen werden nach den bestehenden CE-Regeln abgewiesen.
5. Ein abweichendes erkanntes Tag-Präfix im Quellformat überschreibt den Alias-Tag nicht. Nur mit `-verbose` erscheint ein Hinweis mit Aufrufstelle und beiden Tags. Gleichwertige Tags und freie CE-Selektoren erzeugen keinen Konflikthinweis. Es gibt keinen neuen Warnschalter.

Ohne passende CE-Transformation gilt für `-alias MyAlarm:wrn`:

| Format im Quellaufruf | Ereignis-Tag | Meldungstext nach Formatierung |
|-----------------------|--------------|--------------------------------|
| `Too hot %d`          | `WARNING`    | `Too hot 85`                   |
| `err:Too hot %d`      | `WARNING`    | `err:Too hot 85`               |
| `wrn:Too hot %d`      | `WARNING`    | `wrn:Too hot 85`               |

Diese echten Textpräfixe bleiben bei expliziter Tag-Zuordnung auch unter `-color default`, `none` und `off` erhalten. Farbe und Klassifikation richten sich nach dem zugeordneten Tag. Bei alten Einträgen ohne explizite Zuordnung gilt weiterhin die bisherige Präfixdarstellung. Bei String-Aliasen wird die Klassifikation ebenfalls nicht aus dem Inhalt des Laufzeitstrings abgeleitet.

### 37.4. <a id="til-schema-und-historische-ids"></a>TIL-Schema und historische IDs

Die TIL bleibt eine Map numerischer IDs. `Type` und `Strg` behalten ihre bisherige Bedeutung. Für explizit getaggte Aliase kommt folgendes optionales Objekt hinzu:

```json
{
  "1234": {
    "Type": "trice",
    "Strg": "Temperature too high: %d degrees",
    "Meta": {
      "SchemaVersion": 1,
      "Tag": {"Kind": "builtin", "Name": "WARNING"}
    }
  }
}
```

Für einen Custom-Tag lautet `Tag` beispielsweise `{"Kind":"user","Name":"sensor"}`. Die Unterscheidung verhindert eine spätere Umdeutung gespeicherter Custom-Namen zu neu eingeführten Built-ins.

- Neue Metadaten werden für diese Funktion nur bei Aliasen mit expliziter Tag-Zuordnung erzeugt. Normale Aufrufe und ungetaggte Aliase erhalten keinen solchen Zusatz; vorhandene historische Einträge werden nicht pauschal umgeschrieben.
- Ohne `Meta.Tag` gilt die bisherige Präfixklassifikation, sonst `untagged`. Ein vorhandener Tag ist maßgeblich; explizites `untagged` unterdrückt ebenfalls die Präfixklassifikation.
- Wenn `Meta` vorhanden ist, muss seine Schemaversion unterstützt werden. Bekannte Felder werden auf Struktur und zulässige Werte geprüft. Ungültige oder nicht unterstützte Metadaten führen beim initialen Laden zu einer verständlichen Fehlermeldung vor Quellenänderungen beziehungsweise vor Öffnen des Log-Eingangs.
- Unbekannte optionale Felder einer unterstützten Version bleiben beim Schreiben erhalten, auch in JSON-Exporten. Neue verpflichtende Semantik benötigt eine neue Schemaversion. Es gibt keine Versions-/Tag-Sonderobjekte auf Dateiebene und keine künstlichen ID-Einträge.
- Farbe, Gewicht, abgeleiteter Level, Aliasname und Tool-Mindestversion werden nicht gespeichert. `Created`, `Used` und automatische Bereinigung bleiben zurückgestellt.
- Die semantische Vergleichsidentität besteht aus effektivem Typ, effektivem Format und Tag-Zuordnung einschließlich des Unterschieds zwischen expliziter Zuordnung und Legacy-Präfixverhalten. Unbekannte optionale Metadaten und deren JSON-Reihenfolge sind kein zusätzlicher ID-Schlüssel.
- Ein Tag-Wechsel verwendet eine passende andere historische ID oder eine neue ID; bestehende ID-Einträge werden nicht umgedeutet. Kanonisch gleiche Tag-Schreibweisen, Farben und Gewichte verursachen keinen ID-Wechsel. Tag-spezifische ID-Bereiche berücksichtigen den zugeordneten Tag.

Diese Regeln gelten für Bind/Insert/Clean, Wiederverwendung aus Sekundär-Dictionaries und die vorhandene Ordnung identischer Aufrufe. Clean mit denselben Alias-/CE-Einstellungen erhält die historische TIL-Semantik und entfernt die vorgesehenen ID-/CE-Quellergänzungen.

### 37.5. <a id="host-auswertung-und-kompatibilität"></a>Host-Auswertung und Kompatibilität

- Text, JSON/KV, Filter, Tag-Statistik und Visualisierung verwenden dieselbe aufgelöste Ereignisidentität. Die Tag-Zuordnung funktioniert auch mit `-li off`.
- Built-in-Tags verwenden die bestehende Host-Registry. Ein expliziter Custom-Tag aus TIL ist ohne weitere CLI-Registrierung nutzbar und erhält ohne Override das finale INFO-Gewicht. `-utag sensor:650` überschreibt dieses Gewicht am Host. Gewicht und daraus abgeleiteter Level sind weiterhin Host-Policy.
- Implizit aus TIL bekannte Custom-Tags ändern die Präfixinterpretation alter Einträge nicht. Eine ausdrückliche Registrierung durch `-utag` behält dagegen ihre bisherige Wirkung auf Präfixe.
- TIL-Tags stehen vor der Filtervalidierung und dem Öffnen des Eingangs bereit. Beim Nachladen werden Dictionary und davon abhängige Tag-Auflösung konsistent aktualisiert. Ein ungültiges Update wird diagnostiziert; die letzte gültige Sicht bleibt erhalten.
- Neue Tools akzeptieren alte Dateien mit ihrem bestehenden Verhalten. Diese Erweiterung beseitigt keine anderweitigen historischen Formatgrenzen.
- Alte Tools dürfen neue Felder beim Lesen ignorieren. Korrekte Alias-Filterung durch alte Logger und das Schreiben neuer Dateien mit alten Tools sind nicht zugesichert. Ein Legacy-Export ist kein Bestandteil der Umsetzung.
- Generierte JSON-Dictionary-Sichten erhalten die Metadaten. Generierte C-Logtabellen verwenden weiterhin ihr bestehendes Typ-/Formatmodell; die neue Host-Tag-Zuordnung erweitert weder die Target-Library noch deren lokale Filter-/Farbfunktionen.

### 37.6. <a id="dokumentation-und-umfang"></a>Dokumentation und Umfang

UM/RM und CLI-Hilfe dokumentieren `-utag` mit dem kompatiblen Namen `-ulabel`, die neuen Aliasformen, CE-Reihenfolge, ID-/Tag-Historie und die Grenzen alter Logger. Ein kleines hardwarefreies Beispiel zeigt skalaren Alias, String-Alias, Custom-Tag und Alias-CE. Der Changelog enthält einen kurzen Hinweis mit Verweis auf die ausführliche Dokumentation.

Im RM-Kapitel zur Location Information wird der vorgeschlagene kurze Abschnitt zur externen Archivierung ergänzt: finale LI per Inhalts-Hash identifizieren, unverändert archivieren, passenden Datensatz extern auswählen und mit `-li` verwenden. ZIP-Archive werden extern entpackt. Trice erhält weder Hash-Erzeugung noch Archivverwaltung oder eine neue LI-Struktur. Zusätzliche Target-Protokollfelder, Laufzeitstring-CE, TIL-Zeitstempel und automatische Datenbereinigung gehören nicht zu dieser Umsetzung.

### 37.7. <a id="abnahmekriterien"></a>Abnahmekriterien

Die Umsetzung ist freigabefähig, wenn beschreibende Verhaltenstests folgende Ergebnisse belegen:

| Prüfung                         | Erwartetes Ergebnis                                                                                                                                                                              |
|---------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Bestehende CLI und Dictionaries | Ungetaggte Aliase, normale Aufrufe, alte TIL/LI und `-ulabel` behalten ihr Verhalten.                                                                                                            |
| Aliasformen und Registrierung   | `-alias` und `-salias` mit Built-in-, Custom- und explizitem `untagged`-Tag funktionieren; ungültige/konfligierende Definitionen scheitern ohne Teiländerungen.                                  |
| CE                              | Alias-Gruppe zuerst, Quellgruppen danach, gemeinsame Gruppe einmal; Insert/Clean-Roundtrip und unveränderte String-/Wrapper-Grenzen.                                                             |
| Text und Klassifikation         | Die obige Präfixmatrix stimmt für Text und JSON/KV in allen drei Farbmodi; Konflikthinweise erscheinen ausschließlich mit `-verbose`.                                                            |
| Historie und ID-Policy          | Gleichlautende Meldungen mit verschiedenen Tags bleiben unterscheidbar; Tagwechsel bewahrt alte Captures; kanonisch gleiche Schreibweisen behalten IDs; passende ID-Bereiche werden eingehalten. |
| Host-Funktionen                 | Filter, Level, Statistik und Visualisierung stimmen überein; Custom-Default/Override, `-li off`, echte Aufrufposition und TIL-Nachladen funktionieren.                                           |
| Dateivertrag                    | Alte und neue Einträge können gemischt werden; unbekannte optionale Felder überstehen Schreiben/JSON-Export; ungültige Schemata werden verständlich behandelt.                                   |
| Alte Leser                      | Eine gezielt geprüfte bisherige Toolversion liest Typ/Format einer erweiterten TIL weiter; die dokumentierte fehlende Alias-Klassifikation wird ausdrücklich nachgewiesen.                       |
| Dokumentation und Beispiel      | CLI-Hilfe passt zum Kommando; das hardwarefreie Beispiel zeigt die beschriebenen Ergebnisse; ToC und Links stimmen.                                                                              |

Vor Abschluss folgen die üblichen projektbezogenen Tests und die für geänderten Go-Code vorgeschriebene Coverage-Prüfung. Diese Spezifikation dokumentiert Soll-Ergebnisse; Implementierung und deren Tests stehen noch aus.

## 38. <a id="rückfragen"></a>Rückfragen

- Seit wie lange existiert -uLabel? Ist es vielleicht sauberer es im Zuge des v2.0.0 Releases ganz durch -uTag zu ersetzen?
- -alias MyAlarm:neu geht auch für in diesem Moment neu angelegte User-Tags?
- Ist es sinnvoll Aliasen zusätzliche CE Tags mitgeben zu können, etwa -alias MyAlarm:neu:ctx7:ctx2? Oder ist das over-Design? Wie aufwändig ware das für die Implementation einerseits und für die Dokumentation andererseits?
- -alias Name und -alias Name:untagged --- Wie damit umgehen? Ich bin für die einfachste Lösung.
- "Historisch akzeptierte Gewicht-/Farbangaben bei ID-Kommandos bleiben gültig, erzeugen aber keine neuen Build-Metadaten." --- Was meinst Du damit genau? Diese Angaben z.B. in Skripten bei build würde ich als Fehler zuückweisen.
- Ist "SchemaVersion" sauberes Englisch?
- Soll für vereinfachstes Handling der Aliasname auf standard-Tags geparst werden und diese werden implizit zugewiesen? Etwa -alias Log_Error würde dann ausreichen und nicht verlangen -alias Log_Error:err. Allerdings könnte ein anderer Tag zugewiesen werden  -alias Log_Error:warning.
- Ist es sinnvoll den Aliasnamen in der TIL abzulegen, vielleicht sogar als "Alias": "Log_Error:warning"? Oder vielleicht gar als erweiterte "Type": "TRice:Log_Error:warning" ? Dann würde das "Meta" Feld verzichtbar. Dass dann alte Trice Versionen neue TILs nicht verstehen wäre akzeptabel, da wir an der Schwelle zu 2.0 stehen. Wichtig: Es soll einfach bleiben und gut verständlich.
- Im Augenblick ist es anscheinend so, dass nur, wenn -alias Log_Error:warning eine TIL-Erweiterung nötig ist, oder?

## 39. <a id="antworten-auf-die-rückfragen"></a>Antworten auf die Rückfragen

**Analysebasis:** aktueller Checkout `wip`, Commit `6815a9e3`, lokale Git-Historie und die unten verlinkten Implementierungsstellen. `-utag` und die Alias-Tag-Syntax sind noch nicht implementiert. Die Beispiele beschreiben das vorgeschlagene Ergebnis. Die folgenden Empfehlungen ergänzen die Diskussion; sie sind keine Implementierungsfreigabe. Dein Wunsch, Gewicht und Farbe bei Build-Kommandos zurückzuweisen, korrigiert den entsprechenden Kompatibilitätsvorschlag in Abschnitt 37.2.

### 39.1. <a id="alter-von--ulabel-und-vollständige-umbenennung"></a>Alter von `-ulabel` und vollständige Umbenennung

`-ulabel` wurde am **1. Juli 2025** mit Commit `d735a528282e803f53d42b17cc5c78d461175fee` (`experimental: user labels`) eingeführt. Veröffentlicht ist die Option mindestens seit **v1.1.0 vom 19. Dezember 2025**; der [Changelog zu v1.1.0](../../CHANGELOG.md#v1.1.0-changes) führt sie ausdrücklich als neuen CLI-Schalter auf. Es handelt sich damit bereits um eine veröffentlichte Schnittstelle. Die tatsächliche Schreibweise ist `-ulabel`, nicht `-uLabel`; siehe [CLI-Registrierung](../../internal/args/init.go).

**Für den von dir gewünschten klaren Schnitt empfehle ich, in v2 nur `-utag` anzubieten und `-ulabel` zu entfernen.** Ein Hauptversionswechsel ist der passende Zeitpunkt für eine solche inkompatible Schnittstellenänderung, entsprechend dem Prinzip von [Semantic Versioning](https://semver.org/#spec-item-8). Betroffene Skripte müssen den Optionsnamen ändern; bei Build-Kommandos müssen zusätzlich Gewicht und Farbe entfallen. Das sollte kurz im Changelog mit Verweis auf die ausführliche CLI-Dokumentation stehen.

Der bisher vorgeschlagene Kompatibilitätsalias war eine Abwägung zugunsten bestehender Skripte, keine technische Notwendigkeit. Er wäre im Code billig, ließe aber beide Begriffe dauerhaft bestehen. Die vollständige Umbenennung vereinfacht die öffentliche Sprache: Ein Ereignis hat einen Tag, benutzerdefinierte Tags werden mit `-utag` registriert. Ich würde die Kleinschreibung beibehalten und keine zusätzlichen Schreibvarianten einführen.

### 39.2. <a id="neue-user-tags-direkt-durch--alias"></a>Neue User-Tags direkt durch `-alias`

**Ja, `-alias MyAlarm:neu` soll einen bisher unbekannten gültigen Namen `neu` unmittelbar als User-Tag registrieren und dem Alias zuordnen.** Ein vorheriges `-utag neu` ist dafür unnötig. Die Registrierung muss vor CE-Auswahl und Auflösung der ID-Bereiche erfolgen, unabhängig von der Reihenfolge der CLI-Optionen.

`neu` bleibt ein eigener, schreibungsabhängiger Tag. Eine bereits bekannte Built-in-Schreibweise wie `warning` wird dagegen als `WARNING` aufgelöst. Im vorgeschlagenen TIL-Schema steht für `neu` entsprechend `{"Kind":"user","Name":"neu"}`. Der spätere Logger kennt diesen expliziten Tag aus der TIL; ein Host-Override wie `-utag neu:650` ist nur für eine gewünschte Gewichtung erforderlich. Ohne Override gilt das finale INFO-Gewicht.

Das bedeutet zugleich: Ein syntaktisch gültiger Tippfehler wie `warnng` würde einen User-Tag erzeugen. Automatische Registrierung kann nicht zugleich jeden unbekannten Namen als Fehler behandeln. Die tatsächliche Zuordnung sollte deshalb bei `-verbose` nachvollziehbar sein.

### 39.3. <a id="zusätzliche-ce-selektoren-an-einem-alias"></a>Zusätzliche CE-Selektoren an einem Alias

**Das ist sinnvoll, wenn mehrere Aliase denselben Ereignis-Tag, aber unterschiedliche wiederverwendbare Kontextgruppen bekommen sollen. Für den bisherigen Anwendungsfall würde ich es zunächst zurückstellen.** Es ist keine grundsätzlich übertriebene Idee, aber eine zusätzliche Funktion über die bereits vorgesehene CE-Auswahl durch den Alias-Tag hinaus.

Für `MyAlarm:neu` lassen sich bereits im bisherigen Entwurf mehrere Erweiterungen an dieselbe Gruppe hängen:

```sh
-alias MyAlarm:neu \
-ce 'neu:", ctx7={ctx7:%u}", ctx7Value' \
-ce 'neu:", ctx2={ctx2:%u}", ctx2Value'
```

Beide Werte müssen an den ausgewählten Aufrufstellen verfügbar sein. Die Regeln wirken in CLI-Reihenfolge. Für einzelne Aufrufstellen können weiterhin Quellpräfixe wie `MyAlarm("ctx7:ctx2:Ready");` mit passenden `-ce`-Regeln verwendet werden.

Die zusätzliche Form `-alias MyAlarm:neu:ctx7:ctx2` hätte vor allem diesen Nutzen: Die Definitionen von `ctx7` und `ctx2` könnten von mehreren Aliasgruppen gemeinsam verwendet werden, ohne sie unter jedem Ereignis-Tag erneut zu konfigurieren oder Präfixe in die Quelle zu schreiben.

Falls diese Erweiterung später gewünscht wird, wäre die verständliche Regel: **erster Wert nach dem Alias = Ereignis-Tag; weitere Werte = reine CE-Selektoren.** Die Reihenfolge wäre `neu`, `ctx7`, `ctx2`, danach die Quellselektoren. Jede kanonische Gruppe würde insgesamt einmal angewendet. Die zusätzlichen Selektoren dürften nicht automatisch als Ereignis-Tags registriert werden, weil eine solche Registrierung die vorhandene Präfixbehandlung beeinflusst.

Qualitative Aufwandsschätzung **zusätzlich zur ohnehin geplanten Alias-Tag-Funktion**:

| Bereich                                  | Aufwand und Grund                                                                                                                                                               |
|------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Syntax und Speicherung der Selektorliste | Klein: geordnete Liste einlesen und ungültige/leere Bestandteile abweisen.                                                                                                      |
| CE-Integration und Verhaltenstests       | Mittel: gemeinsame Reihenfolge und Duplikaterkennung über Alias und Quelle, wiederholtes Insert, Clean-Rückweg, Argumentgrenzen und String-Aliase prüfen.                       |
| Dokumentation                            | Klein bis mittel: ein zusätzlicher Abschnitt mit Beispielen; die Unterscheidung zwischen einem Ereignis-Tag und mehreren CE-Selektoren muss in CLI-Hilfe und RM eindeutig sein. |

Die bestehende [CE-Auswahl](../../internal/id/contextEnrichment.go) gruppiert und entdoppelt Regeln bereits. Der Aufwand liegt daher vor allem in der Integration und Absicherung der verschiedenen Arbeitsabläufe, weniger im Zerlegen der CLI-Zeichenkette. Zusätzliche TIL-Felder für die CE-Selektorliste wären nicht nötig: Das wirksame Format und der Typ werden bereits nach der CE-Transformation gespeichert.

### 39.4. <a id="-alias-name-gegenüber--alias-nameuntagged"></a>`-alias Name` gegenüber `-alias Name:untagged`

**Ich empfehle, beide Formen zu erlauben und ihre Bedeutung klar zu unterscheiden.** Dafür ist kein besonderer dritter Modus nötig: Es gibt nur „kein Tag zugeordnet“ und „Tag ausdrücklich zugeordnet“. `untagged` ist im zweiten Fall ein gewöhnlicher Wert der vorhandenen Tag-Registry.

| Konfiguration          | Format `Ready` | Format `err:Ready`                     | Zusätzliche Tag-Metadaten |
|------------------------|----------------|----------------------------------------|---------------------------|
| `-alias Name`          | `untagged`     | `ERROR`, bisherige Präfixbehandlung    | Nein                      |
| `-alias Name:untagged` | `untagged`     | `untagged`, `err:` bleibt Meldungstext | Ja                        |

Die Tabelle gilt ohne passende CE-Transformation. Die beiden Formen gleichzusetzen würde entweder das bestehende Verhalten von `-alias Name` verändern oder die ausdrückliche Zuordnung wirkungslos machen. Die Unterscheidung ergibt sich bereits aus dem optionalen Tag-Feld und braucht keinen zusätzlichen CLI-Schalter.

Für denselben Alias beide Formen gleichzeitig anzugeben, sollte wie jeder andere widersprüchliche Alias-Eintrag vor Änderungen als Fehler enden. Identische Wiederholungen bleiben zulässig. So hängt das Ergebnis nicht von der Reihenfolge ab.

### 39.5. <a id="gewicht-und-farbe-bei-build-kommandos"></a>Gewicht und Farbe bei Build-Kommandos

Der beanstandete Satz sollte bedeuten: Ein bisher akzeptiertes `bind -ulabel sensor:650` registriert weiterhin `sensor`, aber die `650` wird weder in die TIL übernommen noch zur ID-Vergabe oder CE-Auswahl verwendet. Der heutige gemeinsame [Parser](../../internal/emitter/lineTransformerANSI.go) akzeptiert Gewicht und Farbe auch dort, weil [Bind, Insert, Clean und Log](../../internal/args/handler.go) dieselbe Registrierung aufrufen.

**Dein Vorschlag ist klarer: Bei `bind`, `insert` und `clean` nur Namen zulassen und Gewicht-/Farbangaben als Fehler zurückweisen.** Diese Vorgabe ersetzt für die weitere Planung den Satz aus Abschnitt 37.2.

| Kommando                  | `-utag sensor`                | `-utag sensor:650` / `-utag sensor:red:blue` |
|---------------------------|-------------------------------|----------------------------------------------|
| `bind`, `insert`, `clean` | Zulässige Namensregistrierung | Fehler vor Dateiänderungen                   |
| `log`, `tlog`             | Zulässige Namensregistrierung | Zulässige Gewichtung beziehungsweise Farbe   |

Die Fehlermeldung sollte den betroffenen Wert nennen und erklären, dass Gewicht und Farbe beim Logging gesetzt werden. Die Hilfe beschreibt jeweils die tatsächlich zulässige Syntax. Ein Alias mit explizitem Custom-Tag benötigt bei Bind weiterhin überhaupt kein zusätzliches `-utag`.

Das ist eine bewusste Änderung gegenüber bislang akzeptierten Build-Aufrufen. Sie gilt unabhängig davon, ob der alte Optionsname letztlich entfernt oder als Alias behalten wird. Einen neuen Build-Schalter für Gewichte oder Farben brauchen wir dafür nicht.

### 39.6. <a id="englischer-feldname-schemaversion"></a>Englischer Feldname `SchemaVersion`

**Ja, „schema version“ ist korrektes Englisch und bedeutet „Version des Datenschemas“.** `SchemaVersion` ist die dazu passende Schreibweise eines Feldnamens und passt zu `Type`, `Strg` und `Meta`. `SchemeVersion` wäre hier der falsche Begriff.

Der Feldname bezeichnet in diesem Entwurf das Format des `Meta`-Objekts. Eine kurze englische Erklärung genügt: „Version of the metadata schema.“ Dafür bleibt eine ganze Zahl wie `1` ausreichend; eine Toolversion oder Mindestversion wird damit nicht behauptet.

### 39.7. <a id="automatische-tag-erkennung-im-aliasnamen"></a>Automatische Tag-Erkennung im Aliasnamen

**Ich empfehle weiterhin eine explizite Zuordnung wie `-alias Log_Error:err`.** Sie ist kurz und erlaubt jeden Projektnamen, ohne dass dessen Schreibweise zusätzliche Regeln erfüllen muss.

Eine automatische Erkennung wäre implementierbar, bräuchte aber einen eigenen Vertrag: Genügt ein Teilwort, muss es ein durch `_` getrenntes Wort sein oder das letzte Wort? Was bedeutet `Log_Error_Count`, was `Log_Error_Warning`? Sollen auch Kategorie-Tags wie `READ` oder `TIME` erkannt werden? Neue Built-ins könnten später bisher neutrale Aliasnamen anders interpretieren. Ein Alias-Rename könnte dadurch sogar Klassifikation, CE-Auswahl und ID-Bereich verändern.

Ein explizites `:warning` müsste bei einer solchen Lösung immer Vorrang haben. Das löst aber nur den Override-Fall, nicht die übrigen Erkennungsregeln. Für die eingesparten vier Zeichen `:err` würde ich diese zusätzliche Semantik nicht einführen. Die gewünschte Zuordnung steht einmal in der Build-Konfiguration, nicht an jeder Log-Stelle.

### 39.8. <a id="aliasname-oder-tag-in-type-statt-meta"></a>Aliasname oder Tag in `Type` statt `Meta`

**Den Aliasnamen würde ich für diese Funktion nicht in der TIL speichern und `Type` nicht mit Alias-/Tag-Text erweitern.** Das gilt auch unter deiner nun gelockerten Bedingung, dass alte Tools eine neue TIL gegebenenfalls nicht mehr verstehen müssen.

Der Logger benötigt `ID -> effektiver Typ, effektives Format, Ereignis-Tag`. Er benötigt nicht den Namen des Quellmakros. Heute wird dieser Name in [`TriceFmt.Alias`](../../internal/id/id.go) nur vorübergehend gehalten und mit `json:"-"` bewusst nicht gespeichert.

| Darstellung                         | Bewertung                                                                                                                                                                                     |
|-------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `"Alias": "Log_Error"`              | Reicht nicht: Die historische Zuordnung zu `WARNING` fehlt weiterhin.                                                                                                                         |
| `"Alias": "Log_Error:warning"`      | Enthält den Tag, vermischt aber Quellname und Ereignisidentität. Erfordert Zerlegung, Kanonisierung und Regeln für Alias-Renames; die Unterscheidung Built-in/User fehlt.                     |
| `"Type": "TRice:Log_Error:warning"` | Vermischt zusätzlich den Dekodiertyp mit diesen Angaben. Bestehende Typauswertung in Decoder, Validierung und Generator benötigt einen vorgeschalteten Parser oder entsprechende Anpassungen. |
| Separates Tag-Metadatum             | Enthält unmittelbar die Information, die der Logger braucht, und hält Typauswertung und Quellnamen davon unabhängig.                                                                          |

`Type` ist bereits funktional belegt: Beispielsweise rekonstruieren [`ConstructFullTriceInfo` und der Generator](../../internal/id/generate.go) daraus Argumentanzahl und Bitbreite; der [Decoder](../../internal/trexDecoder/trexDecoder.go) wählt darüber die passende Verarbeitung. Die gepackte Schreibweise spart JSON-Felder, vereinfacht diese Verarbeitung aber nicht. Auch mit einem zentralen Parser blieben Tag-Validierung, historische ID-Vergleiche und einheitliche Host-Klassifikation notwendig.

Ein Alias-Rename bei unverändertem Typ, Format und Tag sollte für sich keinen ID-Wechsel erzwingen. Ebenso sollten `Log_Error:wrn` und `OtherName:WARNING` dieselbe semantische Vergleichsidentität haben; die bestehenden Regeln für mehrere Aufrufstellen bleiben davon getrennt. Ein gespeicherter Aliasname müsste aus dieser Identität ausdrücklich herausgehalten werden. Ohne konkreten Bedarf an Herkunftsinformation ist sein Weglassen einfacher.

**Meine Empfehlung bleibt deshalb der bereits akzeptierte optionale `Meta`-Bereich:**

```json
{
  "Type": "trice",
  "Strg": "Ready",
  "Meta": {
    "SchemaVersion": 1,
    "Tag": {"Kind": "builtin", "Name": "WARNING"}
  }
}
```

Ein flaches `"Tag": "WARNING"` wäre die kleinste reine Tag-Lösung, falls du den Versions- und Built-in/User-Vertrag bewusst vereinfachen möchtest. Das wäre eine neue Schemaentscheidung. `Alias` oder ein erweiterter `Type` ersetzen diesen Vertrag hingegen nicht; sie verpacken dieselben Fragen nur in einer Zeichenkette. Die Metadatenversion bleibt auch bei akzeptierter Inkompatibilität mit alten Tools nützlich, um spätere, noch unbekannte Metadatensemantik zu erkennen.

### 39.9. <a id="wann-die-til-erweiterung-tatsächlich-entsteht"></a>Wann die TIL-Erweiterung tatsächlich entsteht

**Ja: Im derzeitigen Entwurf entstehen neue Tag-Metadaten nur für tatsächlich verarbeitete Aufrufstellen mit ausdrücklich zugeordnetem Alias-Tag.** Das betrifft nicht nur `Log_Error:warning` und nicht nur Bind:

| Verwendung                                                           | Neue Tag-Metadaten für die betroffenen Einträge?                            |
|----------------------------------------------------------------------|-----------------------------------------------------------------------------|
| Gewöhnliche Trice-Aufrufe, auch mit bekanntem Formatpräfix           | Nein                                                                        |
| `-alias Name` oder `-salias Name` ohne Tag                           | Nein                                                                        |
| `-alias Name:warning` oder `-alias Name:neu`                         | Ja                                                                          |
| `-alias Name:untagged`                                               | Ja, weil das Präfixverhalten ausdrücklich übersteuert wird                  |
| `-salias Name:warning`                                               | Ja, unabhängig von der Grenze für zusätzliche skalare CE-Werte              |
| Nur CE ohne ausdrücklichen Alias-Tag                                 | Nein; CE verändert wie bisher den effektiven Typ beziehungsweise das Format |
| Alias-Option angegeben, aber keine passende Aufrufstelle verarbeitet | Allein dadurch entstehen keine neuen TIL-Einträge                           |

Dies gilt entsprechend für Insert. Generate muss bereits vorhandene Metadaten in JSON-Sichten erhalten. Auch wenn der zugeordnete Tag zufällig zu einem Quellpräfix passt, bleibt die explizite Zuordnung gespeichert: Sie legt fest, dass dieses Präfix Meldungstext und nicht die Quelle der Klassifikation ist.

Eine kumulative TIL kann solche Einträge weiterhin enthalten, nachdem ein späterer Build ohne getaggte Aliase erfolgt. Historische Einträge werden dadurch nicht gelöscht oder ihrer Metadaten beraubt. Wer die neue Alias-Tag-Funktion nicht verwendet und keine entsprechenden historischen Einträge hat, sieht für diese Erweiterung weiterhin nur das bisherige `Type`-/`Strg`-Schema.


## 40. <a id="feedback"></a>Feedback

- Wie ist es generell bei der Groß-Klein Schreibung bei CLI Switches 1) im Allgemeinen 2) beim Trice Tool 3) Sollte eine generelle Case-Toleranz eingeführt werden für trice 4) Einige CLI Switches anpassen? 5) alles im Wesentliches so lassen?
- Die TIL Formatänderung ist also ausschließlich erforderlich für -alias name:tag bzw. -salias:tag, richtig? Ist es dann nicht klüger, entweder ":tag" als Erweiterung vorerst nicht anzubieten oder für solche Fälle eine dritte aliasTags.json vorzusehen? Außerdem sehe ich bei den begrenzen LogLeveln, wie Err, warning, Info, Debug, ... keine Unlösbarkeit, diese anhand der Aliasnamen zuzuordnen, wenn es bei nicht Zuzuordnenden Namen ein Detault-Tag gibt. bind könnte Partial-Matches nutzen, diese Zuordnung bei -verbose auch anzeigen. Das halte ich aktuell für am anwenderfreundlichsten. die ":tag" Erweiterungsoption würde dann erst bei nachgewiedsenenm Bedarf angegangen werden und damit kann die Entscheidung, wie die bind Tag Info übermittelt wird verztagt werden. Die Fest eingebaute Tag-Tabelle sollte gute Match-Möglichkeiten liefern, Bittze prüfe das und zeige, wo missverständliche Zuordnungen möglich sind.

## 41. <a id="bewertung-des-feedbacks"></a>Bewertung des Feedbacks

**Eine dokumentierte Namenskonvention kann die Alias-Konfiguration vereinfachen. Die Übermittlung des daraus ermittelten Tags an den Logger muss trotzdem gelöst werden.** Meine vorige Antwort war in diesem Punkt zu eng: Nicht die Schreibweise `:tag` erzeugt den Informationsbedarf, sondern jede Tag-Zuordnung, die sich nicht aus dem gespeicherten Format ergibt. Bei der CLI empfehle ich, bestehende Namen im Wesentlichen zu erhalten und neue Namen einheitlich zu wählen.

Die Bewertung beruht weiterhin auf `6815a9e3`. Für die folgenden Match-Beispiele habe ich die tatsächlich eingetragenen Namen aus der Tag-Tabelle ausgelesen und im Speicher verglichen. Das ist eine Analyse des vorgeschlagenen Verhaltens, kein Test einer bereits implementierten Alias-Erkennung.

### 41.1. <a id="groß--und-kleinschreibung-bei-cli-optionen"></a>Groß- und Kleinschreibung bei CLI-Optionen

**Im Allgemeinen gibt es keine für alle Programme geltende Regel.** Viele klassische CLI-Werkzeuge unterscheiden die Schreibweise. Beispielsweise sind bei curl `-o` und `-O` unterschiedliche Optionen: einmal mit vorgegebenem Ausgabedateinamen, einmal mit einem aus der URL übernommenen Namen. [curl-Handbuch](https://curl.se/docs/manpage.html)

PowerShell behandelt dagegen die Parameternamen seiner eigenen Befehle ohne Unterscheidung der Groß-/Kleinschreibung. Diese Regel überträgt sich nicht automatisch auf die Argumentauswertung eines externen Programms wie Trice. Entscheidend ist dessen Parser, nicht die Shell, aus der es gestartet wird. [PowerShell-Dokumentation](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_case-sensitivity?view=powershell-7.5)

**Trice unterscheidet Optionsnamen grundsätzlich exakt.** Die [CLI-Registrierung](../../internal/args/init.go) verwendet Go-FlagSets; der [Handler](../../internal/args/handler.go) führt keine allgemeine Normalisierung der Namen durch. Zusätzliche Schreibweisen gelten nur, wenn sie eigens registriert sind.

| Gegenstand                                     | Heutiges Verhalten                                                                                                |
|------------------------------------------------|-------------------------------------------------------------------------------------------------------------------|
| `-ulabel` / `-uLabel`                          | Nur `-ulabel` ist registriert.                                                                                    |
| `-logLevel` / `-loglevel`                      | Beide sind ausdrücklich registriert und setzen dieselbe Variable. `-LogLevel` ist keine weitere gültige Variante. |
| `-logFormat` / `-logformat`                    | Nur `-logFormat` ist registriert.                                                                                 |
| `-IDMin` / `-idmin`, `-genDir` / `-gendir`     | Jeweils nur die erste Schreibweise ist registriert.                                                               |
| Unterkommandos wie `bind`                      | Ebenfalls exakte Erkennung; `BIND` ist kein Alias.                                                                |
| Built-in-Tag als Optionswert, etwa `-pick WrN` | Schreibweise wird bereits tolerant erkannt.                                                                       |
| User-Tag als Optionswert                       | Schreibweise bleibt relevant; `sensor` und `Sensor` können verschiedene Tags sein.                                |
| Wert von `-logFormat`, etwa `JSON`             | Wird bereits unabhängig von der Schreibweise ausgewertet.                                                         |

Optionsnamen und Optionswerte sind somit getrennte Fragen. Eine tolerante Erkennung von `-alias` dürfte beispielsweise weder den C-Makronamen `Log_Error` noch Dateipfade, Formatstrings oder CE-Ausdrücke in Kleinbuchstaben umwandeln.

### 41.2. <a id="empfehlung-für-die-weitere-cli-gestaltung"></a>Empfehlung für die weitere CLI-Gestaltung

**Ich würde zunächst Variante 5 wählen: im Wesentlichen so lassen, mit gezielten Ergänzungen aus Variante 4.** Für neue Optionsnamen eine kleingeschriebene Hauptform verwenden, etwa `-utag`; bei längeren neuen Namen sind durch Bindestriche getrennte Wörter gut lesbar. Die vorhandene Mischung aus `-IDMin`, `-logFormat` und `-dry-run` rechtfertigt für sich keine allgemeine Umbenennung.

Ein zusätzlicher Alias wie `-logformat` wäre eine kleine, kompatible Erleichterung, falls gerade diese Schreibweise häufig Schwierigkeiten macht. Eine flächendeckende Sammlung neuer Schreibvarianten würde dagegen Hilfe und Dokumentation verlängern. Die begriffliche Änderung `-ulabel` zu `-utag` ist davon unabhängig: Sie vereinheitlicht die Begriffe „Tag“ und „Label“, nicht bloß die Großschreibung.

**Allgemeine Case-Toleranz wäre technisch möglich und bei Trice nicht offensichtlich durch bestehende Namenskollisionen verhindert.** In den geprüften Registrierungen ist `logLevel`/`loglevel` die vorhandene Gruppe unterschiedlich geschriebener, beim Kleinschreiben gleicher Namen; beide haben bereits dieselbe Bedeutung. Es wäre daher übertrieben, diese Option als grundsätzlich problematisch abzulehnen.

Sie wäre aber eine eigene CLI-Änderung mit Prüfbedarf: Namensauflösung je Unterkommando, gleiche Behandlung von `-name value` und `-name=value`, unveränderte Werte, Hilfe und Erkennung ausdrücklich gesetzter Optionen. Auch die Sonderbehandlung von `generate -logC path` müsste mitziehen. Bloß sämtliche Argumente kleinzuschreiben wäre falsch. Falls diese Erleichterung gewünscht wird, würde ich sie auf lange Optionsnamen begrenzen, kurze Optionen weiterhin exakt behandeln und Unterkommandos ausdrücklich separat entscheiden.

### 41.3. <a id="warum-auch-automatische-alias-tags-übertragen-werden-müssen"></a>Warum auch automatische Alias-Tags übertragen werden müssen

Im bisherigen Entwurf wäre `-alias Name:Tag` beziehungsweise **`-salias Name:Tag`** der Auslöser für neue Tag-Metadaten. Daraus folgt aber nicht, dass das Weglassen von `:Tag` die Metadaten überflüssig macht.

Angenommen, Bind erkennt aus den registrierten Namen automatisch:

```c
Log_Error("Ready");
Log_Info("Ready");
```

Der heutige [Alias-Resolver](../../internal/id/helper.go) macht aus beiden skalaren Aliasnamen den Typ `trice`. Der [Bind-Vergleichseintrag](../../internal/id/bindMetadata.go) entfernt den vorübergehenden Aliasnamen; außerdem wird [`TriceFmt.Alias`](../../internal/id/id.go) nicht nach JSON geschrieben. Bei zwei vergebenen IDs könnte die TIL deshalb schematisch so aussehen:

```json
{
  "1234": {"Type": "trice", "Strg": "Ready"},
  "1235": {"Type": "trice", "Strg": "Ready"}
}
```

Der Logger sieht unterschiedliche IDs, aber keine Information darüber, welche ERROR und welche INFO bedeutet. Er erhält weder den ursprünglichen Aliasnamen noch die Bind-Kommandozeile. Eine perfekte Namensheuristik beim Bind ändert daran nichts. Auch eine Ausgabe unter `-verbose` ist noch keine vom Logger verwendbare Zuordnung.

**Die Entscheidung über den Transport lässt sich deshalb nur zusammen mit der eigentlichen Host-Klassifikation vertagen.** Ein zunächst rein diagnostischer Bind-Hinweis auf vermutete Tags wäre möglich, erfüllte aber noch nicht das Ziel, danach mit `-pick`, `-loglevel`, Farben und JSON dieselben Tags zu verwenden.

Eine TIL-**Schemaänderung** ist dabei nicht zwingend: Eine zusätzliche Datei oder ein geeignetes Formatpräfix können die Information ebenfalls tragen. Irgendeine dauerhafte Zuordnung bleibt erforderlich. Fest vereinbarte ID-Bereiche wären auch eine solche Zuordnung und müssten dem Logger samt historischer Bedeutung bekannt sein; sie entstehen nicht automatisch aus den Zahlen.

### 41.4. <a id="konkrete-teilwort-treffer-aus-der-vorhandenen-tag-tabelle"></a>Konkrete Teilwort-Treffer aus der vorhandenen Tag-Tabelle

**Dein Einwand ist berechtigt: Für übliche Namen ist die Zuordnung einfach und keineswegs unlösbar.** `Log_Error`, `LOG_ERR`, `LogWarning`, `Log_Info` und `Log_Debug` liefern mit der vorhandenen Tabelle die naheliegenden Gruppen.

Die Tabelle enthält jedoch mehr als die acht Level-Gruppen `FATAL`, `CRITICAL`, `ERROR`, `WARNING`, `INFO`, `DEBUG`, `TRACE` und `VERBOSE`. Hinzu kommen Kategorien wie `ALARM`, `READ`, `WRITE` und `INTERRUPT` sowie kurze Alternativnamen wie `db`, `em`, `wr` und `int`. Siehe [`defaultTags` und `levelTags`](../../internal/emitter/lineTransformerANSI.go). Die heutige Tag-Erkennung vergleicht **ganze Namen**; ihre Eindeutigkeit lässt sich nicht ohne Weiteres auf Teilwortsuche übertragen.

Für die Prüfung bedeutet „Partial-Match“ konkret: Ein Tabellenname kommt irgendwo im Aliasnamen vor, ohne Beachtung der Groß-/Kleinschreibung. Mehrere Schreibweisen derselben Gruppe zählen als ein Treffer. Die folgenden Aliasnamen sind gezielte Analysebeispiele:

| Aliasname              | Treffer nur in den acht Level-Gruppen | Zusätzliche Treffer aus der übrigen Anwendungstag-Tabelle | Bewertung                                                                                                      |
|------------------------|---------------------------------------|-----------------------------------------------------------|----------------------------------------------------------------------------------------------------------------|
| `Log_Error`, `LOG_ERR` | ERROR                                 | Keine                                                     | Üblicher, eindeutiger Fall; `err` und `error` gehören zur selben Gruppe.                                       |
| `LogWarning`           | WARNING                               | Keine                                                     | Üblicher, eindeutiger Fall.                                                                                    |
| `Log_Error_Warning`    | ERROR und WARNING                     | Keine                                                     | Ein Default für fehlende Treffer löst diese Mehrdeutigkeit nicht.                                              |
| `Log_Infrared`         | INFO durch `inf`                      | Keine                                                     | Ein Wort für Infrarot enthält zufällig einen Level-Kurznamen.                                                  |
| `Log_Readback`         | DEBUG durch `db`                      | READ durch `read`                                         | Auch die Beschränkung auf Level-Gruppen verhindert den zufälligen DEBUG-Treffer nicht.                         |
| `Log_ErrorCount`       | ERROR                                 | Keine                                                     | Kann absichtlich Error bedeuten oder nur einen Fehlerzähler ausgeben; der Name allein klärt die Absicht nicht. |
| `LOG_WRN`              | WARNING                               | WRITE durch `wr`                                          | Die gesamte Tabelle erzeugt hier eine zusätzliche, unerwünschte Alternative.                                   |
| `Log_Temperature`      | Keine                                 | EMERGENCY durch `em`                                      | Besonders deutliche Fehlzuordnung bei Nutzung aller kurzen Tabellenaliase.                                     |
| `PrintError`           | ERROR                                 | INTERRUPT durch `int`                                     | Ein sinnvoller Error-Name wird durch die vollständige Tabelle mehrdeutig.                                      |
| `MyAlarm`              | Keine                                 | ALARM                                                     | ALARM hat standardmäßig Level ERROR; das frühere Beispiel wollte hingegen WARNING.                             |

Die Beschränkung auf die acht Level-Gruppen beseitigt also viele Probleme, aber nicht alle. „Längster Treffer gewinnt“ oder „höchstes Gewicht gewinnt“ wäre zwar deterministisch, würde jedoch die beabsichtigte Bedeutung bei `Log_Error_Warning` oder `Log_ErrorCount` weiterhin nicht erkennen.

**Ein konkreter Bedarf für eine explizite Zuordnung steckt bereits im bisherigen Beispiel `MyAlarm:wrn`, sofern diese Zuordnung beibehalten werden soll.** Die vollständige Tabelle ergibt ALARM mit Level ERROR; die reine Level-Tabelle findet nichts. Ohne Override müsste der Anwender den Alias passend zur Konvention benennen oder sich mit dem Fallback begnügen. Wenn das Beispiel nur illustrativ war und alle echten Projekte passende Namen wählen können, kann `:tag` dagegen durchaus warten.

### 41.5. <a id="eine-einfache-namenskonvention-ist-vertretbar"></a>Eine einfache Namenskonvention ist vertretbar

Ich würde meine frühere pauschale Ablehnung deshalb präzisieren: **Eine bewusst begrenzte Namenskonvention kann anwenderfreundlich sein; beliebige Teilwortsuche über die gesamte Tag-Tabelle würde ich nicht empfehlen.**

Eine kleine Variante könnte die vorhandenen Schreibweisen der acht Level-Gruppen als **vollständige, durch `_` getrennte Namensbestandteile** erkennen. Damit funktionieren etwa `Log_Error`, `LOG_ERR` und `Log_Error_S`, während `Log_Infrared` und `Log_Readback` nicht zufällig klassifiziert werden. `LogError` würde bei dieser bewusst einfachen Regel noch nicht erkannt; CamelCase könnte später ausdrücklich hinzukommen. Die Regel müsste nicht den eigentlichen C-Identifier verändern.

Für fehlende oder mehrere Treffer sind zwei verschiedene Fälle zu behandeln:

| Fall                                 | Meine Empfehlung                                                                                                                                                                      |
|--------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Genau eine kanonische Gruppe erkannt | Diese Gruppe verwenden; unter `-verbose` Alias, Treffer und aufgelösten Tag anzeigen. Die Priorität gegenüber einem abweichenden Quellpräfix muss ausdrücklich festgelegt sein.       |
| Keine Gruppe erkannt                 | Bisherige Präfixklassifikation beibehalten; ohne erkanntes Präfix bleibt das Ereignis `untagged`. Unter `-verbose` den Fallback erklären.                                             |
| Mehrere verschiedene Gruppen erkannt | Verständlicher Konfigurationsfehler vor Änderungen, statt die Tabellenreihenfolge entscheiden zu lassen. Ohne expliziten Override muss der Aliasname dann eindeutiger gewählt werden. |

Ein erzwungener Default wie INFO wäre ebenfalls definierbar, könnte aber bestehendes `LegacyLog("err:...")` zu INFO machen. `untagged`, INFO und DEFAULT sind zudem unterschiedliche Tag-Gruppen, auch wenn ihre Standardgewichte derzeit gleich sind. Der Fallback sollte deshalb eine genaue Regel bekommen und nicht nur „Default-Tag“ heißen.

`-verbose` hilft bei der Kontrolle, verhindert aber keine Fehlzuordnung in normalen Builds. Außerdem wäre automatische Klassifikation eine Verhaltensänderung des bisherigen `-alias Name`: Ein zuvor ungetaggtes `Log_Error("Ready")` würde künftig ERROR. Das müsste für v2 bewusst beschlossen und dokumentiert werden. Wenn sich die Erkennungsregel später ändert, dürfen alte IDs nicht nachträglich einen anderen Tag erhalten.

### 41.6. <a id="separate-aliastagsjson-oder-vertagung"></a>Separate `aliasTags.json` oder Vertagung

**Eine dritte Datei ist technisch möglich. Einfacher wird der Gesamtworkflow dadurch nur, wenn das unveränderte TIL-Schema eine wichtige eigenständige Anforderung ist.** Die Datei müsste die empfangene ID auf den aufgelösten Tag abbilden, beispielsweise:

```json
{
  "1234": "ERROR",
  "1235": "INFO"
}
```

Eine bloße Tabelle `Log_Error -> ERROR` genügt nicht, weil dem Logger weiterhin die Verbindung von der ID zum Aliasnamen fehlen würde.

Auch bei einer separaten Datei muss die ID-Vergabe den Tag berücksichtigen. Bei einem Wechsel von ERROR zu WARNING darf nicht einfach der bisherige Eintrag für dieselbe ID überschrieben werden. Hinzu kommen gemeinsames Speichern, Archivieren und konsistentes Nachladen von TIL und Tag-Datei. Fehlende oder veraltete Tag-Daten dürfen nicht unbemerkt falsche Filterergebnisse erzeugen. Bei vollständig unveränderter TIL kann der Logger allein an ihr nicht erkennen, ob eine zusätzliche Tag-Datei fehlt; die Erwartung an diese Datei müsste daher anderweitig feststehen.

| Weg                                                        | TIL-Schema erweitern?                                | Konsequenz                                                                                                         |
|------------------------------------------------------------|------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------|
| Alias-Klassifikation vollständig vertagen                  | Nein                                                 | Bestehende Aliase bleiben nutzbar; Tags kommen weiterhin aus dem Format.                                           |
| Automatische Erkennung und separate `aliasTags.json`       | Nein                                                 | Vollständige Host-Funktion möglich, aber zusätzlicher Dateivertrag und dieselben Anforderungen an historische IDs. |
| Automatische Erkennung und optionales Tag-Metadatum in TIL | Ja                                                   | Name und Syntax können einfach bleiben; die ID und ihre Bedeutung stehen zusammen in einer Datei.                  |
| Ermittelten Tag als Präfix in `Strg` speichern             | Nein, aber der gespeicherte Formatinhalt ändert sich | Für skalare Aliase denkbar; bewusst andere Präfixdarstellung und Anpassungen der ID-/CE-Verarbeitung nötig.        |

Beim letzten Weg könnte die Quelle `Log_Error("Ready")` unverändert bleiben und die TIL `err:Ready` enthalten. Das wäre der kleinste Ansatz mit dem bisherigen Dateischema, **wenn ein sichtbares `err:` bei `-color off` akzeptiert wird**. Er weicht damit vom bisher gewünschten Meldungstext-Vertrag ab. Für String-Aliase ist er außerdem keine unmittelbare allgemeine Lösung: Der [Decoder](../../internal/trexDecoder/trexDecoder.go) erkennt deren besondere `SAlias_Strg(...)`-Darstellung an Anfang und Ende und ersetzt sie durch `%s`. Einfach einen Tag davorzusetzen würde diese Erkennung verändern.

**Meine Empfehlung für die weitere Entscheidung:** Die Namenskonvention unabhängig vom Speicherformat bewerten. Wenn automatische Tags jetzt bis zur Host-Ausgabe funktionieren sollen, den aufgelösten Tag bereits jetzt persistent speichern; dafür bleibt die optionale TIL-Erweiterung mein bevorzugter Weg. Falls auch diese Speicherentscheidung warten soll, die Alias-Klassifikation insgesamt vertagen. Nur `:tag` wegzulassen verschiebt die notwendige Entscheidung nicht.

## 42. <a id="names-heuristik"></a>Names-Heuristik

Die Beschränkung auf die acht Level-Gruppen ist ok. Erweitern mit deterministischen Zusatzregeln passt. Fehlzuordnungsfälle wie bei LogErrorCount sind sehr speziell und damit akzeptabel. Diese potentiellen Fehlzuordnungsfälle als hingenommen allgemein im RM dokumentieren und zusammen mit der späteren (nächste Trice Version) Erweiterungsoption -alias name:tag und dem dann notwenigen Erfordernis tag von bind an log zu übergeben und dafür noch eine Lösung zu finden. 

-alias Zuordnungen sind immer auf trice und -salias Zuordnungen immer auf triceS. Dann könnten Aliasnamen im TIL "Type" stehen. Wenn tlog dort einen unbekannten Type findet kann es nur trice oder triceS sein. Und diese Unterscheidung ist anhand des Formatstrings einfach zu machen. Das wäre dann zwar auch eine TIL-Änderung und alte Versionen könnte Probleme haben diese auszuwerten, aber aus meiner Sicht ein kleinerer Eingriff.
Zusammen mit einer heuristischen Tag-Zuordnung wäre damit das "Meta" Feld vorest unnötig. Wenn dieser Weg gegangen wird, muss noch klar sein, dass Formatstrings entsprechend -color und dem zugewiesenem Tag eingefärbt werden, auch wenn sie selbst keinen Tag oder einen (dann nicht validen) Tag haben, etwa MyWrn("err:bla"). Die heuristische Tag-Zuordnung ist hier WARNING, aber im Formatsting steht "err:" Die Einfärbung ist dann entsprechend WARNING und "err:" darf nicht entfernt werden, da es in einem Alias steht. Ich denke das ist wasserdicht.

Wenn dann also der Aliasname unter "Type" abgelegt wird, wie hier vorgeschlagen, kann log mit der gleichen Funktion den Tag rekonstruieren.
Wo ist hier eine Schwachstelle außer der genannten? Es wird damit insgesamt einfacher für den Nutzer und die Logik auf den Tag zu schließen aus dem Alias-Namen ist begrenzter Aufwand. Bitte bewerten.

## 43. <a id="bewertung-der-namens-heuristik"></a>Bewertung der Namens-Heuristik

**Ja: Für den jetzt eingegrenzten Funktionsumfang halte ich diesen Ansatz für sinnvoll. Der Aliasname in `Type` schließt die bisherige Informationslücke; zusätzliche Tag-Metadaten in `Meta` sind damit vorerst entbehrlich.** Die akzeptierten Fehlzuordnungen durch Teilworttreffer sind eine dokumentierbare Grenze der Namenskonvention. Sie sind kein Grund, den Ansatz erneut abzulehnen.

Meine vorige Empfehlung ging davon aus, dass die TIL weiterhin nur den normalisierten Typ `trice` beziehungsweise `triceS` enthält. Dein neuer Vorschlag ändert genau diese Voraussetzung. Damit kann Log den Tag tatsächlich selbst bestimmen, ohne die Bind-Kommandozeile oder eine zusätzliche Datei zu benötigen. Noch nicht vollständig festgelegt sind die Typunterscheidung, die Stabilität der Heuristik über Versionen und einige Folgen für ID-Vergabe und Ausgabe.

Die folgenden Aussagen zum heutigen Code beziehen sich auf `6815a9e3`. Die vorgeschlagene Funktion ist damit noch nicht implementiert oder getestet.

### 43.1. <a id="der-aliasname-genügt-als-gespeicherte-grundlage"></a>Der Aliasname genügt als gespeicherte Grundlage

Mit dem vorgeschlagenen Vertrag wären diese Einträge ausreichend:

```json
{
  "1234": {"Type": "Log_Error", "Strg": "Ready"},
  "1235": {"Type": "Log_Info", "Strg": "Ready"}
}
```

Log kann aus `Log_Error` ERROR und aus `Log_Info` INFO ableiten. Anders als im Beispiel meines vorigen Kapitels bleibt die Verbindung zwischen ID und Alias erhalten. Bind und Log können dafür dieselbe Funktion verwenden. Das gilt entsprechend für Insert.

Dein Vorschlag überträgt die Information schon in der ersten Ausbaustufe von Bind an Log: **über den Aliasnamen, aus dem der Tag eindeutig nach der vereinbarten Regel folgt.** Erst eine spätere freie Zuordnung wie `-alias MyAlarm:warning` benötigt zusätzliche Information, weil `warning` dann nicht zwingend aus `MyAlarm` ableitbar ist. Deren Speicherform kann tatsächlich bis zu dieser Erweiterung warten.

Auch der Target-Datenstrom muss dafür nicht erweitert werden. Die ID verweist weiterhin auf einen TIL-Eintrag; dessen zusätzliche Bedeutung wird auf dem Host ausgewertet.

### 43.2. <a id="die-unterscheidung-zwischen-skalarem-alias-und-string-alias"></a>Die Unterscheidung zwischen skalarem Alias und String-Alias

**Die Unterscheidung ist möglich, sollte aber ausdrücklich an der vorhandenen String-Alias-Kennzeichnung hängen.** Sie sollte nicht durch Zählen oder Suchen von `%s` neu geraten werden.

Heute speichern [Bind](../../internal/id/bindParse.go) und [Insert](../../internal/id/insertIDs.go) die Quellargumente eines String-Alias in der Sonderform `SAlias_Strg('...')SAlias_Strg`. Der [Decoder](../../internal/trexDecoder/trexDecoder.go) erkennt diese Hülle und verwendet für die empfangene Zeichenkette `%s`. Die vorhandenen [Alias-Tests](../../internal/id/insertIDs_test.go) enthalten beispielsweise `SAlias_NO_MESSAGE(42)` und String-Aliase mit einem `%d` im Quelltext. Ein gewöhnlicher Formatstring ist deren gespeicherter `Strg`-Inhalt somit nicht immer.

Für einen bereits als Alias eingeordneten `Type` ergibt sich ein kleiner Vertrag:

| TIL-Eintrag                                                  | Abgeleiteter Grundtyp                                                          |
|--------------------------------------------------------------|--------------------------------------------------------------------------------|
| `Strg` trägt vollständig die vorhandene String-Alias-Hülle   | `triceS`                                                                       |
| `Strg` trägt diese Hülle nicht                               | `trice`                                                                        |
| Aliasname endet auf `S`, enthält `32` oder das Wort `Assert` | Daraus folgt kein Grundtyp und keine Bitbreite. Entscheidend bleibt die Hülle. |

Eine reale Grenze bleibt: Ein skalarer Alias könnte als normalen Meldungstext exakt `SAlias_Strg('abc')SAlias_Strg` verwenden. Die Hülle wird schon heute besonders behandelt; in deinem Modell wäre sie zusätzlich die einzige gespeicherte Unterscheidung der beiden Aliasarten. **Diese Form muss deshalb reserviert sein.** Für die kleine Lösung würde ich einen solchen skalaren Meldungstext bei Bind/Insert mit einer verständlichen Fehlermeldung ablehnen. Das benötigt kein neues JSON-Feld.

Die Unterscheidung ist damit belastbar, sofern alle Schreibpfade diese Hülle erhalten. Auch eine Bearbeitung des Formats durch CE darf sie nicht versehentlich entfernen oder durch angehängten Text unkenntlich machen.

### 43.3. <a id="was-unbekannter-type-künftig-bedeutet"></a>Was „unbekannter Type“ künftig bedeutet

„Unbekannt“ ist zunächst nur eine Aussage über den Leser. Es beweist nicht, dass der Eintrag ein gültiger Alias ist. Auch ein beschädigter Typname oder ein erst in einer späteren Version eingeführter nativer Typ kann unbekannt sein.

Dein Ansatz kann daraus aber bewusst eine neue Formatregel machen: **Ein gültiger Alias-Identifier außerhalb des reservierten Trice-Typnamensraums bezeichnet einen Alias; sein Grundtyp folgt aus der String-Alias-Hülle.** Native Typnamen müssen Vorrang haben und für Alias-Registrierungen ausgeschlossen sein. Für künftige native Typen sollte außerdem ein eindeutiger Namensraum reserviert bleiben, beispielsweise Namen, die unabhängig von der Großschreibung mit `trice` beginnen.

Damit würde ein Leser einen ihm unbekannten nativen Typ weiterhin als nicht unterstützt melden, statt ihn als skalaren Alias zu dekodieren. Ein Tippfehler in einem ansonsten gültigen Alias-Identifier wäre dagegen nicht zuverlässig von einem absichtlich gewählten Namen unterscheidbar. Das ist eine Grenze dieses kompakten Formats, aber kein zwingender Grund für `Meta`.

Die von dir akzeptierte Einschränkung für alte Tools bleibt bestehen. JSON bleibt syntaktisch gleich, die Bedeutung von `Type` ändert sich jedoch. Alte Leser können daraus weder den ursprünglichen Typ noch den Tag nach dem neuen Vertrag ableiten.

### 43.4. <a id="die-wichtigste-zusätzliche-schwachstelle-ist-die-versionsstabilität"></a>Die wichtigste zusätzliche Schwachstelle ist die Versionsstabilität

**Dieselbe Funktion in Bind und Log genügt innerhalb einer Version. Für alte Aufzeichnungen muss auch eine spätere Version dieselbe Zuordnung liefern.**

Wenn eine spätere Version neue Suchwörter ergänzt, die Trefferpriorität verändert oder einen anderen Default wählt, kann derselbe unveränderte TIL-Eintrag plötzlich einen anderen Tag erhalten. Dann ändern sich beispielsweise `-pick` und `-loglevel` für eine historische ID, obwohl weder die Firmware noch die TIL geändert wurde.

Für die kleine Lösung lässt sich das ohne Zusatzfeld lösen: Die erste veröffentlichte Heuristik wird Teil des TIL-Vertrags und bleibt für diese Einträge stabil. Das umfasst:

- die acht zugelassenen Level-Gruppen und ihre verwendeten Suchwörter;
- Groß-/Kleinschreibung und Art der Teilwortsuche;
- die Priorität bei mehreren Treffern;
- den Tag bei keinem Treffer.

Neue Namen in der allgemeinen Tag-Tabelle dürfen dadurch nicht automatisch neue Heuristik-Treffer erzeugen. Ebenso dürfen konfigurierbare Gewichte und Farben die Namenszuordnung nicht verändern.

Eine einfache deterministische Regel wäre beispielsweise: längster gefundener Suchname gewinnt; bei gleich langen Treffern verschiedener Gruppen entscheidet eine fest dokumentierte Reihenfolge. Das ist ein Vorschlag zur noch offenen Detailregel, keine bereits getroffene Entscheidung. Die Laufzeitgewichte aus `-ulabel` wären dafür ungeeignet, weil sie die gleiche TIL je nach Log-Aufruf unterschiedlich klassifizieren könnten.

Wenn die Heuristik später absichtlich geändert werden soll, braucht es dann einen Weg, die bisherige Bedeutung alter Einträge zu erhalten, etwa eine explizit gespeicherte Zuordnung oder eine erkennbare Version. **Diese Erweiterung muss heute nicht vorweggenommen werden; die bestehende Regel darf dann nur nicht stillschweigend ersetzt werden.**

### 43.5. <a id="der-aliasname-wird-bei-einfacher-umsetzung-teil-der-id-identität"></a>Der Aliasname wird bei einfacher Umsetzung Teil der ID-Identität

Die heutige [Rückwärtssuche für IDs](../../internal/id/manage.go) vergleicht `TriceFmt` einschließlich `Type` und `Strg`. Bleibt dieses Prinzip bestehen, unterscheiden sich künftig auch diese beiden Einträge:

```json
{
  "1234": {"Type": "Log_Error", "Strg": "Ready"},
  "1235": {"Type": "Report_Error", "Strg": "Ready"}
}
```

Beide ergeben ERROR und denselben Grundtyp. Trotzdem verändert eine Alias-Umbenennung den Vergleichsschlüssel und kann eine neue ID erforderlich machen. Auch eine bloße Änderung der Schreibweise des Aliasnamens kann das bewirken.

**Für die einfache Variante halte ich diese Folge für vertretbar.** Sie weicht von meiner früheren Empfehlung ab, reine Alias-Umbenennungen bei unverändertem Typ, Format und Tag von der ID-Identität auszunehmen. Diese zusätzliche Optimierung würde ich hier nicht zur Voraussetzung machen. Ein historischer Eintrag darf allerdings nicht unter derselben ID auf einen Alias mit anderer Tag-Bedeutung umgeschrieben werden.

Bereits vorhandene Einträge mit normalisiertem `Type: "trice"` oder `Type: "triceS"` enthalten ihren früheren Aliasnamen nicht. Log kann ihn nachträglich nicht rekonstruieren. Solche Einträge behalten ihre bisherige Interpretation; die neue Heuristik greift für Einträge, in denen der Alias tatsächlich gespeichert ist. Daraus folgt keine Notwendigkeit, hier zusätzlich eine Migration zu entwerfen.

### 43.6. <a id="deine-regel-für-mywrnerrbla-ist-richtig"></a>Deine Regel für `MyWrn("err:bla")` ist richtig

**WARNING bestimmt Klassifikation und Darstellung; `err:` gehört vollständig zum Meldungstext.** Dass `err` in anderen Zusammenhängen ein gültiger Tag ist, spielt für diese Aliasstelle keine Rolle.

| Ausgabe beziehungsweise Funktion                  | Erwartetes Ergebnis                               |
|---------------------------------------------------|---------------------------------------------------|
| Text mit aktiver Farbpalette                      | Sichtbarer Text `err:bla`, gestaltet nach WARNING |
| `-color none`                                     | Sichtbarer Text `err:bla`, ohne Farbsequenzen     |
| `-color off`                                      | Sichtbarer Text `err:bla`, ohne Farbsequenzen     |
| JSON beziehungsweise andere strukturierte Ausgabe | Tag WARNING und Meldung `err:bla`                 |
| Filter und Tag-Statistik                          | Zuordnung zu WARNING, nicht zu ERROR              |

Das muss genauso für `MyWrn("bla")` und `MyWrn("wrn:bla")` gelten: kein künstlich sichtbares `wrn:` hinzufügen und auch ein zufällig passendes `wrn:` nicht entfernen. Bei String-Aliasen und mehrzeiligen Meldungen muss diese Regel den gesamten ausgegebenen Text erfassen.

**Allein `record.Tag = "WARNING"` zu setzen, würde im heutigen Code noch nicht reichen.** Die [Textdarstellung](../../internal/emitter/lineTransformerANSI.go) wertet Textpräfixe selbst aus; auch die [strukturierte Ausgabe](../../internal/translator/structured.go) enthält eigene Regeln zum Entfernen eines Präfixes. Diese Stellen müssen erfahren, dass die Klassifikation aus dem Alias stammt und ein Präfix im Meldungstext deshalb unangetastet bleibt. Diese Herkunft ist aus `Type` ableitbar und kann intern weitergereicht werden; dafür ist kein zusätzliches TIL-Feld erforderlich.

Noch festzulegen ist der Fall ohne Namenstreffer. Wenn „ein Präfix in einem Alias ist Meldungstext“ für alle Aliase gelten soll, darf `LegacyLog("err:bla")` nicht doch wieder ERROR aus dem Text ableiten. **Ich empfehle dafür `untagged`, bei unverändertem Text `err:bla`.** Ein fester Default wie INFO wäre ebenfalls möglich, hätte aber eine andere Filterbedeutung. Meine vorige Empfehlung, bei fehlendem Namenstreffer auf die Präfixklassifikation zurückzufallen, würde ich für diesen einheitlichen Alias-Vertrag verwerfen.

### 43.7. <a id="auswirkungen-auf-die-vorhandene-verarbeitung"></a>Auswirkungen auf die vorhandene Verarbeitung

Der Umfang der eigentlichen Namensheuristik ist begrenzt. Dein Vorschlag vermeidet zusätzliche JSON-Felder und eine weitere Datei. Im vorhandenen Code ist `Type` allerdings an mehreren Stellen ein auswertbarer Trice-Typ; dort darf künftig nicht ungeprüft der Aliasname hineingereicht werden.

| Bereich                             | Erforderliche Konsequenz des Vorschlags                                                                                                                                                                                                                                                  |
|-------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Bind und Insert                     | Originalen Aliasnamen für TIL und ID-Vergleich erhalten; für Prüfung und Codeerzeugung den Grundtyp verwenden.                                                                                                                                                                           |
| CE                                  | Aliasname zusätzlich zum effektiv bearbeiteten Format erhalten. [`enrichBindSite`](../../internal/id/contextEnrichment.go) übernimmt heute den normalisierten Typ zurück in `site.macro`.                                                                                                |
| TIL-Lesen und erneutes Schreiben    | Für die Verarbeitung aufgelösten Typ und gespeicherten Alias auseinanderhalten, damit ein Schreibvorgang den Alias nicht wieder verliert.                                                                                                                                                |
| Typprüfung, Decoder und Generatoren | Alias vor den bestehenden Typoperationen auflösen. Beispielsweise erwartet [`ConstructFullTriceInfo`](../../internal/id/generate.go) einen Namen mit `trice` am Anfang und greift auf dessen erste fünf Zeichen zu. Ein kurzer Alias wie `Wrn` ist dafür kein gültiger direkter Eingang. |
| Ausgabe, Filter und Statistik       | Den einmal abgeleiteten Tag verwenden und den Alias-Meldungstext vor erneuter Präfixauswertung schützen.                                                                                                                                                                                 |

Das ist überschaubare, aber nicht auf eine Suchfunktion beschränkte Arbeit. Die Trennung von Tag und sichtbarem Text wäre bei einer `Meta`-Lösung ebenfalls erforderlich; sie ist kein besonderer Nachteil des vorgeschlagenen Speicherorts.

**Ich würde den Ansatz für die erste Ausbaustufe weiterverfolgen:** Aliasname in `Type`, vorhandene reservierte String-Alias-Hülle, eine festgelegte und versionsstabile Heuristik sowie unveränderter Alias-Meldungstext. Für diesen Umfang sehe ich keine grundsätzliche Informationslücke mehr. Vor der Implementierung sollten vor allem Default und Mehrfachtrefferregel sowie die Folgen von Alias-Umbenennungen ausdrücklich festgehalten werden. Die freie Zuordnung mit `:tag` und ihre zusätzliche Speicherung können wie vorgeschlagen einer späteren Version vorbehalten bleiben.

## 44. <a id="implementationsspezifikation-für-alias-namensheuristik-und-cli"></a>Implementationsspezifikation für Alias-Namensheuristik und CLI

**Ziel:** Registrierte Aliasnamen bestimmen automatisch den Ereignis-Tag. Die TIL speichert den Aliasnamen in `Type`; Log rekonstruiert daraus den Tag und aus `Strg` den benötigten Grundtyp `trice` oder `triceS`. Zusätzliche TIL-Felder und eine separate Tag-Datei werden für diese Ausbaustufe nicht benötigt.

Dieses Kapitel beschreibt die spätere Implementierung einschließlich Tests und Dokumentation. Es ersetzt für diese Funktion widersprechende Vorgaben früherer Kapitel, insbesondere die Spezifikation aus Kapitel 37. Die bisher noch offenen Detailregeln werden hier konkret vorgeschlagen und am Kapitelende zur Prüfung zusammengefasst. Das Schreiben dieser Spezifikation führt noch keine Implementierung aus.

### 44.1. <a id="umfang-und-ergebnis-aus-anwendersicht"></a>Umfang und Ergebnis aus Anwendersicht

Ein Projekt registriert beispielsweise:

```sh
trice bind -src app -til til.json -li li.json -alias MyWrn
```

Die erkannte Quellstelle lautet:

```c
MyWrn("err:code=%u", code);
```

Bei `code == 7` entsteht ein Ereignis mit Tag WARNING und Meldungstext `err:code=7`. Bei Standardgewichten ist sein Level ebenfalls WARNING. Der Logger benötigt weder `-alias MyWrn` noch die ursprüngliche Bind-Kommandozeile. Er kann die Meldung beispielsweise mit `-pick wrn` auswählen. Text, JSON/KV, Filter und Statistik verwenden denselben Ereignis-Tag.

Diese Ausbaustufe umfasst:

- automatische Tag-Zuordnung für `-alias` und `-salias`;
- Speicherung und erneute Auflösung der Aliasnamen über `Type`;
- konsistente ID-Vergabe, tag-spezifische ID-Bereiche und CE-Auswahl;
- korrekte Typverarbeitung in den betroffenen Lesern und Generatoren;
- die unten festgelegten CLI-Anpassungen, Verhaltenstests und Dokumentation.

Die freie Zuordnung `Name:Tag`, zusätzliche Alias-CE-Selektoren wie `Name:Tag:ctx7`, implizite Custom-Tags aus solchen Angaben, `Meta`, `SchemaVersion`, `Created`, `Used` und automatische Datenbereinigung bleiben zurückgestellt. Dasselbe gilt für eine neue Kombination aus Laufzeitstring und zusätzlichen skalaren Payload-Werten. Die Alias-Klassifikation selbst fügt keine Target-Payload-Bytes hinzu. Ausdrücklich konfigurierte skalare CE-Werte benötigen weiterhin ihre normalen Payload-Bytes.

### 44.2. <a id="begriffe-und-getrennte-zuständigkeiten"></a>Begriffe und getrennte Zuständigkeiten

| Begriff            | Bedeutung                                                                                                                                        |
|--------------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| Aliasname          | Exakter Name des registrierten C/C++-Makros, beispielsweise `MyWrn`.                                                                             |
| Grundtyp           | Für die Verarbeitung aufgelöstes `trice` oder `triceS`. Er legt die Art der Nutzdaten fest.                                                      |
| Ereignis-Tag       | Aus dem Aliasnamen abgeleitete Gruppe, beispielsweise WARNING. Pro Ereignis gibt es einen Tag.                                                   |
| Level              | Aus dem Tag und der bestehenden Host-Gewichtung abgeleitete Prioritätsstufe.                                                                     |
| CE-Selektor        | Wählt konfigurierte Kontextregeln aus. Mehrere Selektoren sind keine mehreren Ereignis-Tags.                                                     |
| String-Alias-Hülle | Die bereits verwendete Kennzeichnung `SAlias_Strg('...')SAlias_Strg` in `Strg`. Sie kennzeichnet gespeicherte Quellargumente eines String-Alias. |

**Die Tag-Heuristik bestimmt niemals den Grundtyp.** Beispielsweise ergibt `LogError` den Tag ERROR, aber weder eine bestimmte Bitbreite noch einen skalaren oder einen String-Datensatz. Umgekehrt enthält die String-Alias-Hülle keine Tag-Zuordnung.

### 44.3. <a id="cli-namen-und-groß-kleinschreibung"></a>CLI-Namen und Groß-/Kleinschreibung

Es wird keine allgemeine Case-Toleranz eingeführt. Optionsnamen und Unterkommandos bleiben grundsätzlich exakt; vorhandene ausdrücklich registrierte Schreibvarianten bleiben gültig.

| Gegenstand                                                             | Festlegung dieser Spezifikation                                                                                           |
|------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------|
| Neue Optionsnamen                                                      | Kleingeschriebene Hauptform; bei künftig neu eingeführten längeren Namen gegebenenfalls Wörter mit Bindestrichen trennen. |
| `-utag`                                                                | Ersetzt `-ulabel` bei den Kommandos, die diese Option heute anbieten.                                                     |
| `-ulabel`                                                              | Entfällt in dieser v2-Ausbaustufe. Ein Aufruf damit scheitert mit einem Hinweis auf `-utag`.                              |
| `-uTag`, `-uLabel`, `-Utag`                                            | Keine zusätzlichen Schreibvarianten.                                                                                      |
| `-loglevel` und `-logLevel`                                            | Beide bleiben gültig. Neue Beispiele bevorzugen `-loglevel`.                                                              |
| `-logformat`                                                           | Neue, ausdrücklich registrierte Schreibvariante von `-logFormat`; neue Beispiele bevorzugen die kleingeschriebene Form.   |
| `-logFormat`                                                           | Bleibt gültig und verwendet denselben Wert wie `-logformat`.                                                              |
| Andere bestehende Namen, etwa `-IDRange`, `-IDMin`, `-genDir`, `-logC` | Unverändert; keine flächendeckende Ergänzung weiterer Schreibvarianten.                                                   |
| `-a`, `-sa`                                                            | Bleiben Kurzformen von `-alias`, `-salias` mit identischer Wertauswertung.                                                |

Für beide Logformat-Schreibweisen funktionieren `-name value` und `-name=value`. Bei mehrfacher Zuweisung über beide Namen gilt wie bei `-loglevel`/`-logLevel` die letzte Zuweisung. Prüfungen auf ausdrücklich gesetzte Optionen müssen beide Schreibweisen berücksichtigen.

**Optionswerte werden nicht pauschal kleingeschrieben.** Aliasnamen, Dateipfade, Formatstrings, C-Ausdrücke und schreibungsabhängige Custom-Tags behalten ihre Schreibweise. Die bestehenden Regeln für einzelne Werte gelten weiter: Built-in-Tag-Namen und Logformat-Werte werden tolerant erkannt, Custom-Tag-Namen bleiben exakt. Die Heuristik darf den Aliasnamen intern zum Vergleichen kleinschreiben, muss ihn aber unverändert speichern.

Die gezielte Ergänzung `-logformat` benötigt keine allgemeine Vorverarbeitung der Kommandozeile. Insbesondere bleibt die Sonderbehandlung des optionalen Pfads bei `generate -logC` unberührt.

### 44.4. <a id="zulässige-werte-von--utag"></a>Zulässige Werte von `-utag`

Die Umbenennung wird mit der bereits gewünschten Trennung zwischen Instrumentierung und Darstellung verbunden:

| Kommando         | Zulässige Werte                                                   | Aufgabe                                                                                                              |
|------------------|-------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------|
| `bind`, `insert` | `name`                                                            | Namen für die bestehende Präfix-/CE-Behandlung und tag-spezifische ID-Bereiche registrieren.                         |
| `clean`          | `name`                                                            | Bei CE dieselbe Namensregistrierung wie Insert ermöglichen. Zum bloßen Entfernen von IDs ist sie nicht erforderlich. |
| `log`, `tlog`    | `name`, `name:weight`, `name:color` in der bisherigen Wertesyntax | Namen registrieren und Host-Gewicht beziehungsweise Farbe einstellen.                                                |

Bei `bind`, `insert` und `clean` werden Gewicht-/Farbangaben vor Dateiänderungen zurückgewiesen. Die Fehlermeldung nennt den Wert und erklärt, dass Gewicht und Farbe beim Logging angegeben werden. Die jeweilige CLI-Hilfe darf nur die für das Kommando gültige Syntax zeigen.

Die bestehende Bedeutung wiederholter Gewicht-/Farbzuweisungen beim Logging bleibt erhalten. Namensregistrierung erfolgt weiterhin vor der Validierung von Filtern, CE-Auswahl und ID-Bereichen, soweit das jeweilige Kommando diese verwendet.

`-utag sensor` macht `sensor` nicht zu einem Suchwort der Alias-Heuristik. In dieser Ausbaustufe kann ein Alias nur eine der acht Level-Gruppen oder `untagged` erhalten. Custom-Tags über normale Formatpräfixe bleiben davon unabhängig nutzbar. Der spätere Alias-Override `:tag` wird jetzt weder akzeptiert noch stillschweigend ignoriert.

### 44.5. <a id="registrierung-und-prüfung-der-aliasnamen"></a>Registrierung und Prüfung der Aliasnamen

Ein Aliasname muss dem Muster `[A-Za-z_][A-Za-z0-9_]*` entsprechen. Er wird bei der Erkennung von Quellstellen exakt verglichen. Leere Namen, Leerzeichen, Doppelpunkte und andere Zeichen außerhalb dieses Musters sind Fehler.

Der gesamte mit `trice` beginnende Namensraum ist unabhängig von der Großschreibung für native Trice-Typen reserviert. Beispielsweise sind `trice`, `TRICE8_2` und `TriceWarning` keine neu registrierbaren Aliasnamen. Dadurch kann ein späterer Leser einen unbekannten nativen Typ von einem grundsätzlich zulässigen Aliasnamen unterscheiden. Dies ist eine bewusst eingeschränkte Namenswahl; vorhandene Projekte mit einem solchen Alias benötigen eine andere Projektbenennung, keine automatische Umbenennung durch Trice.

Identische Wiederholungen derselben Aliasoption sind zulässig und dürfen keine mehrfachen Effekte erzeugen. Für einen Namen, der unter beiden Aliasarten angegeben wird, bleibt die bestehende Sonderregel aus [`resolveTriceAlias`](../../internal/id/helper.go) erhalten: Endet er auf `s` oder `S`, gilt er als String-Alias, sonst als skalarer Alias. Die Reihenfolge der Optionen ändert das Ergebnis nicht. Für neue Konfigurationen wird empfohlen, jeden Namen nur unter der tatsächlich benötigten Aliasart anzugeben.

Diese Sonderregel gilt ausschließlich bei doppelter Registrierung. Ein allein mit `-alias StatusS` registrierter Name bleibt skalar; ein allein mit `-salias LogErr` registrierter Name bleibt ein String-Alias.

Alle Aliasdefinitionen werden vor Quelländerungen validiert, auch wenn kein entsprechender Aufruf gefunden wird. Für `Name:Tag` erklärt die Fehlermeldung ausdrücklich, dass freie Alias-Tag-Zuordnungen in dieser Ausbaustufe noch nicht unterstützt werden.

### 44.6. <a id="vollständig-festgelegte-namensheuristik"></a>Vollständig festgelegte Namensheuristik

Die gemeinsame Funktion wird von Instrumentierung und Host-Auswertung verwendet. Sie arbeitet allein auf dem Aliasnamen und der folgenden festen Tabelle. Sie hängt weder vom Meldungstext noch von der aktuellen Tag-Registry, Farbe, Gewichtung oder Reihenfolge der CLI-Optionen ab.

| Vorrang bei gleich langen Treffern | Ergebnis | Suchwörter nach Kleinschreibung |
|------------------------------------|----------|---------------------------------|
| 1                                  | FATAL    | `fatal`                         |
| 2                                  | CRITICAL | `critical`, `crit`              |
| 3                                  | ERROR    | `error`, `err`                  |
| 4                                  | WARNING  | `warning`, `warn`, `wrn`        |
| 5                                  | INFO     | `informal`, `info`, `inf`       |
| 6                                  | DEBUG    | `debug`, `dbg`, `deb`, `db`     |
| 7                                  | TRACE    | `trace`, `tr`                   |
| 8                                  | VERBOSE  | `verbose`                       |

Die Auswertung erfolgt in dieser Reihenfolge:

1. Für den Vergleich eine kleingeschriebene Kopie des Aliasnamens verwenden.
2. Alle Suchwörter ermitteln, die irgendwo zusammenhängend im Namen vorkommen. Unterstriche oder Wortgrenzen sind nicht erforderlich; CamelCase braucht keine eigene Zerlegung.
3. Die Länge des längsten gefundenen Suchworts bestimmen.
4. Wenn mehrere Gruppen einen Treffer dieser maximalen Länge besitzen, die zuerst genannte Gruppe der Tabelle wählen.
5. Wenn kein Suchwort vorkommt, `untagged` zurückgeben.

Mehrere Treffer derselben Gruppe ändern deren Ergebnis nicht. Es werden weder eine Warnung noch ein Fehler allein wegen mehrerer Treffer erzeugt. Die längste Schreibweise hat Vorrang vor einer kürzeren Schreibweise einer höheren Level-Gruppe; erst bei gleicher Länge entscheidet die feste Reihenfolge.

| Aliasname              | Ergebnis   | Grund                                          |
|------------------------|------------|------------------------------------------------|
| `MyWrn`                | WARNING    | `wrn`                                          |
| `LOG_ERROR`            | ERROR      | `error`, unabhängig von Großschreibung         |
| `LogErrorCount`        | ERROR      | Akzeptierter Teilworttreffer                   |
| `Log_Error_Warning`    | WARNING    | `warning` ist länger als `error`.              |
| `LogErrWrn`            | ERROR      | Gleich lange Treffer; ERROR steht vor WARNING. |
| `LogDebugTrace`        | DEBUG      | Gleich lange Treffer; DEBUG steht vor TRACE.   |
| `Log_Infrared`         | INFO       | Akzeptierter zufälliger Treffer durch `inf`    |
| `Log_Readback`         | DEBUG      | Akzeptierter zufälliger Treffer durch `db`     |
| `MyAlarm`, `LegacyLog` | `untagged` | Kein Suchwort dieser Tabelle                   |

Die Tabelle entspricht den gegenwärtigen Schreibweisen der acht Level-Gruppen aus der [Tag-Registry](../../internal/emitter/lineTransformerANSI.go), wird für die Heuristik aber als fester Vertrag behandelt. Neue allgemeine Tag-Aliase dürfen die Heuristik nicht automatisch erweitern. Die genannten Zufallstreffer müssen ausdrücklich im RM erklärt werden.

**Dieser Vertrag bleibt für damit geschriebene TIL-Einträge versionsstabil.** Eine spätere Verbesserung darf historische IDs nicht anders klassifizieren. Soll die Heuristik später geändert werden, ist dafür zunächst eine erkennbare Unterscheidung alter und neuer Bedeutung oder eine gespeicherte explizite Zuordnung zu spezifizieren. Eine gemeinsame Funktion allein löst diese Frage nicht, wenn Bind und Log aus unterschiedlichen Versionen stammen.

### 44.7. <a id="til-darstellung-und-interne-darstellung"></a>TIL-Darstellung und interne Darstellung

Die TIL bleibt eine Map von IDs auf Einträge mit `Type` und `Strg`.

| Eintrag              | Persistiertes `Type` | Persistiertes `Strg`                                                |
|----------------------|----------------------|---------------------------------------------------------------------|
| Nativer Trice-Aufruf | Bisheriger Trice-Typ | Bisheriges effektives Format                                        |
| Skalarer Alias       | Originaler Aliasname | Effektives Format nach den zulässigen CE-Transformationen           |
| String-Alias         | Originaler Aliasname | Vorhandene String-Alias-Hülle mit den gespeicherten Quellargumenten |

Tag, Grundtyp, Gewicht, Farbe und Level werden für Alias-Einträge nicht zusätzlich gespeichert. Es gibt keine künstlichen ID-Einträge, keine Versionszeile und keine neue Begleitdatei.

Intern müssen mindestens folgende Informationen auseinandergehalten werden:

- die unveränderten Daten für erneutes Schreiben und ID-Vergleich;
- ob der Eintrag ein Alias ist und welcher Grundtyp benötigt wird;
- der daraus abgeleitete Ereignis-Tag;
- das tatsächlich für die Dekodierung benötigte Format.

Konkrete Struct- und Funktionsnamen bleiben Implementierungsdetails. Entscheidend ist, dass temporäre Typauflösung keinen Aliasnamen in der TIL überschreibt. Ebenso darf aus `MyWrn` beim Ermitteln der Argumentzahl weder in der Quelle noch in der TIL ein erfundener Alias `MyWrn_1` werden. Vorhandene Optionen zur Erweiterung nativer Makronamen um die Argumentzahl dürfen nicht ungeprüft auf Aliasnamen angewendet werden.

### 44.8. <a id="trice-und-trices-zuerst-beim-instrumentieren-unterscheiden"></a>`trice` und `triceS`: zuerst beim Instrumentieren unterscheiden

**Beim Lesen des C/C++-Quellcodes bestimmt die Aliasoption den Grundtyp. Hier muss das Tool nichts aus dem Formatstring erraten.**

| Registrierung                      | Grundtyp                                              | Erwartete Target-Nutzdaten                 |
|------------------------------------|-------------------------------------------------------|--------------------------------------------|
| Nur `-alias Name`                  | `trice`                                               | Die skalaren Werte der Meldung             |
| Nur `-salias Name`                 | `triceS`                                              | Eine zur Laufzeit übertragene Zeichenkette |
| Beide Optionen für denselben Namen | Bestehende Sonderregel anhand des letzten Buchstabens | Entsprechend dem so gewählten Grundtyp     |

Die Registrierung erzeugt keine C-Funktion und keine Laufzeitformatierung. Das Projekt muss das Makro so definieren, dass es den angegebenen Grundtyp tatsächlich verwendet. Ein Alias auf einen beliebigen anderen Trice-Typ wird durch diese Funktion nicht automatisch unterstützt.

Ein einfaches skalares Beispiel ist:

```c
#define MyWrn trice

MyWrn("err:code=%u", code);
```

Das Tool kennt `MyWrn` durch `-alias MyWrn`. Der Wert von `code` wird übertragen; der Host formatiert ihn später anhand von `err:code=%u`.

Ein einfaches String-Beispiel ist:

```c
#define MyWrnText triceS

MyWrnText("%s", text);
```

Das Tool kennt `MyWrnText` durch `-salias MyWrnText`. In diesem Beispiel enthält `text` bereits die gewünschte Meldung, etwa `err:code=7`. Der Target überträgt diese Zeichenkette. Der Host gibt sie über `%s` aus.

String-Aliase dürfen im Rahmen der bestehenden Erkennung auch projektspezifische Wrapper mit anderer Argumentliste bezeichnen. Die vorhandenen [Tests](../../internal/id/insertIDs_test.go) enthalten beispielsweise `SAlias_NO_MESSAGE(42)`. Ein solcher Wrapper muss selbst für eine gültige String-Übertragung sorgen. Das Tool kann deshalb nicht voraussetzen, dass in seinen Quellargumenten überhaupt ein `%s` steht.

### 44.9. <a id="trice-und-trices-später-aus-der-til-unterscheiden"></a>`trice` und `triceS`: später aus der TIL unterscheiden

Beim Logging fehlen die Aliasoptionen. Deshalb muss die beim Instrumentieren bekannte Unterscheidung in der TIL erhalten bleiben. Dafür dient die bereits existierende String-Alias-Hülle.

Für die beiden vorigen Beispiele ergibt sich schematisch:

```json
{
  "1234": {
    "Type": "MyWrn",
    "Strg": "err:code=%u"
  },
  "1235": {
    "Type": "MyWrnText",
    "Strg": "SAlias_Strg('\"%s\", text')SAlias_Strg"
  }
}
```

Nach dem JSON-Lesen ist der zweite `Strg`-Wert wörtlich:

```text
SAlias_Strg('"%s", text')SAlias_Strg
```

Er besteht aus drei Teilen:

| Teil                                           | Inhalt im Beispiel |
|------------------------------------------------|--------------------|
| Fester Anfang                                  | `SAlias_Strg('`    |
| Gespeicherte Quellargumente ohne eingefügte ID | `"%s", text`       |
| Festes Ende                                    | `')SAlias_Strg`    |

**Die Hülle ist eine Kennzeichnung, kein auszugebender Text.** Der Logger wertet die darin stehende C-Argumentliste nicht aus. Insbesondere sucht er keine Host-Variable `text` und formatiert nicht mit dieser Argumentliste. Er erkennt an der Hülle den Grundtyp `triceS`, verwendet für die Dekodierung `%s` und gibt damit die vom Target empfangene Zeichenkette aus.

Die ursprünglichen Quellargumente bleiben für die ID-Zuordnung gespeichert. Sie werden nicht in der TIL durch `%s` ersetzt, denn dann gingen Unterschiede zwischen Quellstellen verloren.

Der gemeinsame Leser arbeitet in dieser Reihenfolge:

1. Ist `Type` ein unterstützter nativer Trice-Typ, gilt die bisherige native Verarbeitung einschließlich der bisherigen String-Alias-Sonderform alter Einträge.
2. Beginnt ein nicht unterstützter `Type` unabhängig von Großschreibung mit `trice`, wird er als unbekannter oder ungültiger nativer Typ gemeldet. Er wird nicht als Alias behandelt.
3. Jeder andere `Type` muss ein gültiger Alias-Identifier nach der oben festgelegten Syntax sein.
4. Für einen solchen Alias wird die Hülle im JSON-dekodierten `Strg` geprüft, vor der Auswertung von Formatplatzhaltern und ohne Leerzeichen abzuschneiden:

| Zustand von `Strg` bei einem Alias                                                       | Ergebnis                                                                  |
|------------------------------------------------------------------------------------------|---------------------------------------------------------------------------|
| Exakter fester Anfang und exaktes festes Ende vorhanden, ohne Überlappung der Randmarker | Grundtyp `triceS`; Dekodierformat `%s`                                    |
| Weder fester Anfang noch festes Ende vorhanden                                           | Grundtyp `trice`; gespeichertes Format verwenden                          |
| Nur ein Randmarker vorhanden oder beide überlappen sich wegen zu kurzer Zeichenkette     | Fehler wegen unvollständiger oder unzulässiger String-Alias-Kennzeichnung |

Die vollständige Hülle muss mindestens so lang wie ihre beiden Randmarker zusammen sein. Diese Prüfung erfolgt vor jedem Zugriff auf ihren inneren Teil. Danach wird unabhängig davon der Ereignis-Tag aus dem Aliasnamen ermittelt.

Somit kann `StatusS` trotz des letzten `S` ein skalarer Alias sein, und `LogErr` ohne letztes `S` ein String-Alias. Der Logger entscheidet anhand der gespeicherten Hülle. Er wiederholt nicht die CLI-Sonderregel für doppelte Registrierung.

### 44.10. <a id="grenzen-der-typunterscheidung"></a>Grenzen der Typunterscheidung

| Grenze                                                                                 | Konsequenz und erforderliches Verhalten                                                                                                                                                                                                                                |
|----------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Ein skalarer Meldungstext könnte die Hülle nachahmen.                                  | Für neue skalare Alias-Einträge sind der feste Anfang am Stringanfang und das feste Ende am Stringende reserviert. Bind/Insert müssen solche Texte vor Änderungen ablehnen, auch wenn nur einer der beiden Randmarker vorkommt.                                        |
| Eine beschädigte String-Alias-TIL könnte beide Randmarker vollständig verlieren.       | Ohne zusätzliche Information ist das nicht sicher von einem skalaren Alias zu unterscheiden. Ein solcher Eintrag wird zunächst als skalar eingeordnet; bestehende Format-/Payload-Prüfungen können Fehler erkennen, garantieren dies aber nicht für jede Beschädigung. |
| Ein beliebiger unbekannter Identifier könnte aus einem Tippfehler stammen.             | Der Leser kann seine Herkunft nicht beweisen. Ein syntaktisch gültiger Name außerhalb des reservierten Namensraums gilt laut Dateivertrag als Alias.                                                                                                                   |
| Ein Name enthält `8`, `16`, `32`, `64`, `Assert` oder endet auf `S`.                   | Diese Bestandteile bestimmen beim Lesen eines Alias weder Bitbreite noch Grundtyp. Vorhandene native Typoperationen dürfen sie nicht versehentlich auswerten.                                                                                                          |
| Ein Projektmakro sendet etwas anderes als den registrierten Grundtyp.                  | Die Aliasoption repariert das nicht. Grundtyp und tatsächliches Makroverhalten müssen zusammenpassen; die normale Abstimmung der Trice-Bitbreite zwischen Target und Tool bleibt erforderlich.                                                                         |
| Ein beliebiger Wrapper verdeckt mehrere Aufrufstellen oder erzeugt mehrere Datensätze. | Die bestehenden Parser-, Bind- und Location-Grenzen bleiben bestehen. Die Erweiterung ist kein allgemeiner C-Präprozessor.                                                                                                                                             |

Das Verfahren ist damit für korrekt erzeugte TIL-Einträge eindeutig, aber keine selbstprüfende Kodierung gegen jede manuelle Beschädigung. Die Garantie beruht auf der reservierten Hülle und dem Vertrag, dass registrierte Makros wirklich `trice` beziehungsweise `triceS` verwenden.

Das Erhalten der Hülle ist zwingend. Weder CE noch JSON-Export, temporäre Normalisierung oder ein späterer Schreibvorgang dürfen Text außerhalb der Hülle anhängen oder einen ihrer Randmarker verlieren.

### 44.11. <a id="verarbeitung-in-bind-insert-clean-und-generate"></a>Verarbeitung in Bind, Insert, Clean und Generate

Für Bind und Insert muss die Verarbeitung folgende Reihenfolge einhalten:

1. CLI und Aliasnamen validieren; benötigte Tag-Namen registrieren.
2. Quellstellen erkennen und Alias-Grundtyp nach der Registrierung bestimmen.
3. Originalen Aliasnamen behalten und den Ereignis-Tag auflösen.
4. Zulässige CE-Regeln auswählen und das effektive Format bestimmen.
5. Format, Argumente, String-Alias-Kennzeichnung und geltende Aufrufgrenzen prüfen.
6. Die endgültige Vergleichsidentität bilden und IDs innerhalb der passenden Bereiche auswählen.
7. Alle geplanten Änderungen prüfen und über die bestehenden Schreib-/Rollback-Verfahren veröffentlichen.

Ein Fehler darf keine teilweise aktualisierten Quellen, TIL-, LI- oder Bind-Dateien zurücklassen. Ein ungenutzter gültiger Alias erzeugt für sich keinen TIL-Eintrag. Clean und Generate verwenden dieselbe Typ- und Identitätsauflösung innerhalb ihrer bisherigen Aufgaben; daraus entsteht keine zusätzliche ID-Vergabe durch diese Kommandos.

| Bereich                                                   | Implementierungsanforderung                                                                                                                                                                          |
|-----------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Aliasauflösung und `bindSiteFormat`                       | Originalname und aufgelösten Grundtyp getrennt erhalten; den Alias nicht vor dem ID-Vergleich verwerfen.                                                                                             |
| Typprüfung, Parameterzählung und `ConstructFullTriceInfo` | Nur den aufgelösten Typ prüfen. Kurze Namen wie `Wrn` dürfen keinen Zugriff außerhalb ihrer Zeichenkettenlänge auslösen.                                                                             |
| CE und Bind-Sidecars                                      | Die Aliasidentität auch nach Änderung des effektiven Formats und beim Wiederlesen bestehender generierter Metadaten erhalten.                                                                        |
| ID-Bereiche                                               | Für Aliasstellen den abgeleiteten Tag verwenden. `MyWrn("err:...")` gehört in einen konfigurierten WARNING-Bereich, nicht in den ERROR-Bereich.                                                      |
| Clean                                                     | Mit den entsprechenden Alias-/CE-Einstellungen die vorgesehenen Quellergänzungen entfernen; historische TIL-Einträge erhalten.                                                                       |
| Generate mit Quellscan                                    | Dieselbe Aliasregistrierung und dieselbe Vergleichsidentität verwenden wie Bind/Insert; nicht nur den normalisierten Typ mit dem gespeicherten Alias vergleichen.                                    |
| JSON-Dictionary-Exporte                                   | Aliasname und `Strg` einschließlich Hülle semantisch unverändert erhalten.                                                                                                                           |
| Generierte typabhängige Tabellen                          | Grundtyp und Format korrekt auflösen, bevor Bitbreite oder Parameterzahl abgeleitet werden. Die neuen Host-Tags erweitern nicht automatisch die lokalen Filter-/Farbfunktionen generierter C-Logger. |

Betroffene Stellen sind unter anderem [Aliasauflösung](../../internal/id/helper.go), [Bind-Metadaten](../../internal/id/bindMetadata.go), [ID-Verwaltung](../../internal/id/manage.go), [ID-Bereiche](../../internal/id/switchIDs.go), [CE](../../internal/id/contextEnrichment.go) und [Generatoren](../../internal/id/generate.go). Die Tabelle beschreibt erforderliche Ergebnisse, keine Aufforderung zu einem allgemeinen Refactoring dieser Bereiche.

### 44.12. <a id="id-identität-historie-und-alte-dateien"></a>ID-Identität, Historie und alte Dateien

Für neue Alias-Einträge besteht der Vergleichsschlüssel aus dem exakt gespeicherten Aliasnamen und dem effektiven `Strg` einschließlich einer gegebenenfalls vorhandenen String-Alias-Hülle. Interne Cachefelder, Farben, Gewichte und Anzeigeoptionen sind kein zusätzlicher Schlüsselbestandteil. Der Tag muss nicht zusätzlich in den Schlüssel, weil er nach dem eingefrorenen Vertrag aus dem Namen folgt.

Es gelten folgende Regeln:

- Wiederholtes Bind/Insert ohne fachliche Änderung hält IDs und erzeugte Inhalte stabil.
- Eine Umbenennung oder Änderung der Groß-/Kleinschreibung des Alias verändert den Schlüssel. Es wird eine passende historische oder eine neue ID verwendet, auch wenn der Tag gleich bleibt.
- Ein Wechsel zwischen skalarem und String-Alias verändert die gespeicherte Darstellung und darf die bisherige ID nicht umdeuten.
- Änderungen des wirksamen Formats oder der CE-Erweiterung werden nach den bestehenden Regeln für eine andere Meldungsidentität behandelt.
- Eine bestehende ID wird nicht mit einer anderen Alias-/Formatbedeutung überschrieben. Die bisherigen Regeln für mehrere identische Aufrufstellen, Teilscans und reservierte IDs bleiben bestehen.
- Ein Alias und ein nativer Aufruf mit gleichem sichtbarem Text bleiben unterscheidbar, weil ihre Präfix- und Tag-Bedeutung verschieden sein kann.

Alte Einträge mit `Type: "trice"` oder `Type: "triceS"` besitzen keinen rekonstruierbaren Aliasnamen. Das neue Tool liest sie nach ihren bisherigen Regeln. Bei erneuter Instrumentierung einer nun nach dem neuen Vertrag gespeicherten Aliasstelle darf es deren bisherige native TIL-Zeile nicht einfach auf den Aliasnamen umschreiben. Die neue Identität erhält eine dazu passende andere ID; der alte Eintrag bleibt für historische Daten erhalten.

Es wird keine automatische Migration, Rekonstruktion alter Aliasnamen oder Bereinigung historischer Einträge implementiert. Eine gemischte TIL mit alten nativen und neuen Alias-Einträgen ist zulässig.

Alte Toolversionen müssen neue Alias-TILs nicht korrekt lesen können. Die frühere Anforderung, dass alte Leser neue Metadaten lediglich ignorieren können, gilt für diese neue `Type`-Bedeutung nicht mehr. Die Dokumentation muss diesen Versionsbruch ausdrücklich benennen.

### 44.13. <a id="ce-auswahl-und-ihre-grenze-bei-string-aliasen"></a>CE-Auswahl und ihre Grenze bei String-Aliasen

Für skalare Aliase bleibt die bereits festgehaltene Auswahl über den Alias-Tag Bestandteil der Funktion:

1. Zuerst die CE-Regelgruppe des abgeleiteten Ereignis-Tags auswählen, einschließlich `untagged`, wenn dafür Regeln vorhanden sind.
2. Danach die Gruppen aus der zusammenhängenden Präfixfolge im Quellformat in deren Reihenfolge auswählen.
3. Jede kanonische Gruppe insgesamt nur einmal anwenden. Innerhalb einer Gruppe bleibt die CLI-Reihenfolge der Regeln erhalten.
4. Die vorhandenen Prüfungen auf zulässige Argumente, sichtbare C-Ausdrücke, Parametergrenzen und direkte Bind-Aufrufstellen beibehalten.

Ein Formatpräfix ist bei einem Alias keine Quelle des Ereignis-Tags. Es kann trotzdem ein ausdrücklich konfigurierter CE-Selektor sein. Die bestehende Unterscheidung bleibt dabei wichtig: Passende freie, vollständig kleingeschriebene Selektoren werden aus dem effektiven Format entfernt; bekannte Tag-Präfixe bleiben stehen. Ohne passende CE-Regel wird deshalb kein Präfix als vermeintlicher Kontext entfernt.

Beispiel mit `-alias MyWrn`:

```c
MyWrn("err:ctx:code={code:%u}", code);
```

```sh
-ce 'wrn:", task={task:%u}", taskId' \
-ce 'ctx:", run={run:%u}", runId'
```

Die WARNING-Gruppe liefert zuerst `task`, danach liefert `ctx` den Wert `run`. `ctx:` wird als passender freier Selektor entfernt, `err:` bleibt Text. Bei den Werten 7, 2 und 3 lautet die Meldung `err:code=7, task=2, run=3`; ihr Tag bleibt WARNING. Eine zusätzliche passende ERROR-Regel könnte über das Quellpräfix ebenfalls Kontext hinzufügen, ohne den Ereignis-Tag zu ändern.

**Für String-Aliase wird in dieser Ausbaustufe nur die Tag-Zuordnung unterstützt. Eine über ihren Tag ausgewählte CE-Regel wird vor Änderungen abgewiesen, auch wenn sie nur Text anhängen würde.** Ihre gespeicherte Hülle enthält eine Quellargumentliste; sie enthält kein allgemeines Host-Ausgabeformat, an das die bestehende CE-Verarbeitung gefahrlos Text oder Werte anhängen könnte. Laufzeitinhalte und die Argumentliste innerhalb der Hülle werden nicht nach CE-Selektoren durchsucht.

Dies grenzt die neue automatische CE-Auswahl für String-Aliase ausdrücklich ein. Die bestehende Möglichkeit einer zulässigen reinen Texterweiterung an einem nativen `triceS` bleibt davon getrennt. Eine entsprechende Erweiterung für `-salias` würde einen eigenen Format- und Darstellungsvertrag benötigen.

Enthält ein Projekt skalare und String-Aliase mit WARNING, würde `-ce 'wrn:...'` daher auch die nicht unterstützten String-Aliasstellen auswählen und den Lauf ablehnen. Soll nur den skalaren Stellen Kontext hinzugefügt werden, kann dort ein eigener Quellselektor wie `ctx:` mit einer passenden CE-Regel verwendet werden. Nicht unterstützte Stellen werden nicht stillschweigend übersprungen.

### 44.14. <a id="host-klassifikation-und-unveränderter-meldungstext"></a>Host-Klassifikation und unveränderter Meldungstext

Der Leser muss für jedes neue Alias-Ereignis Tag und Herkunft der Klassifikation bis zur Darstellung weitergeben. Die Herkunft kann aus dem gespeicherten Alias abgeleitet werden; sie benötigt kein zusätzliches TIL-Feld.

Für Alias-Ereignisse gilt nach einer ausdrücklich konfigurierten CE-Transformation:

| Meldung                                     | Ereignis-Tag | Sichtbarer Text |
|---------------------------------------------|--------------|-----------------|
| `MyWrn("bla")`                              | WARNING      | `bla`           |
| `MyWrn("err:bla")`                          | WARNING      | `err:bla`       |
| `MyWrn("wrn:bla")`                          | WARNING      | `wrn:bla`       |
| `LegacyLog("err:bla")` ohne Namenstreffer   | `untagged`   | `err:bla`       |
| String-Alias `MyWrnText` empfängt `err:bla` | WARNING      | `err:bla`       |

Bei einer aktiven Farbpalette wird die Meldung nach ihrem zugeordneten Tag gestaltet. Bei `-color none` und `-color off` enthält die Ausgabe keine Farbsequenzen; alle genannten Textpräfixe bleiben trotzdem erhalten. JSON/KV enthalten ebenfalls die vollständige Meldung und den gleichen Tag, ohne ANSI-Farbe.

Es wird kein sichtbares Tag-Präfix künstlich eingefügt. Auch gleichlautende Präfixe, Texte nach einem Zeilenumbruch und aus Parametern oder Laufzeitstrings stammender Text werden nicht erneut als Tag ausgewertet. Kommen mehrere Ereignisse in einer Textzeile zusammen, bleibt die Klassifikation pro Ereignis erhalten.

Ein bloßes Setzen von `ApplicationRecord.Tag` reicht nicht: Auch [Textdarstellung](../../internal/emitter/lineTransformerANSI.go), [strukturierte Ausgabe](../../internal/translator/structured.go) und gegebenenfalls die Darstellung von Formaten in Statistiken müssen Aliastext vor erneuter Präfixauswertung schützen.

`-pick`, `-ban`, `-loglevel`, Tag-Statistik und die vorhandene Visualisierungsverarbeitung müssen dieselbe Ereigniszuordnung verwenden. Die bestehende Statistik zählt erfolgreich dekodierte Ereignisse vor der Filterung; Tooldiagnosen werden dabei nicht als Alias-Ereignisse mitgezählt. Gewicht und Farbe sind Host-Einstellungen: Sie dürfen den abgeleiteten Tag oder die TIL nicht ändern, können aber Filterergebnis und abgeleiteten Level beeinflussen.

Die Funktion arbeitet mit `-li off`. Location Information bestimmt weiterhin die Quellposition und ist keine Voraussetzung für Tag oder Grundtyp. Beim TIL-Nachladen werden die gespeicherten Einträge und ihre aufgelöste Verarbeitung gemeinsam aktualisiert. Ein ungültiges Update wird diagnostiziert; die letzte gültige Sicht bleibt nutzbar. Beim initialen Laden muss ein erkennbar ungültiger Eintrag vor dem Öffnen des Log-Eingangs abgewiesen werden.

### 44.15. <a id="diagnostik"></a>Diagnostik

Mit `-verbose` müssen Bind/Insert je registriertem Alias den gewählten Grundtyp, den aufgelösten Tag und den Grund der Zuordnung zeigen. Dazu gehören längster Treffer, Entscheidung bei Gleichstand oder Fallback auf `untagged`. Die Ausgabe muss für dieselbe Konfiguration reproduzierbar sein.

Ein bekanntes abweichendes Tag-Präfix im Quellformat erzeugt ausschließlich mit `-verbose` einen Hinweis mit Aufrufstelle, Alias-Tag und Präfix-Tag. Ein gleichwertiges Präfix oder ein freier CE-Selektor erzeugt keinen solchen Konflikthinweis. Inhalt von Laufzeitstrings wird dafür nicht untersucht. Die bisherigen sonstigen CE-Diagnosen bleiben erhalten.

Ungültige CLI-Werte, reservierte Namen, unvollständige Hüllen und nicht unterstützte CE-Kombinationen sind dagegen echte Fehler und werden auch ohne `-verbose` gemeldet. Sie nennen Option beziehungsweise Datei, ID oder Aufrufstelle und erklären die verletzte Regel. Es gibt keinen zusätzlichen Warnschalter.

### 44.16. <a id="verbindliche-verhaltenstests"></a>Verbindliche Verhaltenstests

Die Implementierung muss Tests mit unabhängigen Soll-Ergebnissen erhalten. Ein Test, der nur dieselbe Hilfsfunktion erneut zur Erzeugung seines Erwartungswerts aufruft, genügt nicht.

| Testbereich                  | Zu prüfende Ergebnisse                                                                                                                                                                                                                                                             |
|------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| CLI-Schreibweisen            | `-logformat` und `-logFormat` liefern für beide Argumentformen denselben Wert; letzte Zuweisung gewinnt. `-loglevel`/`-logLevel` bleiben gültig. Nicht registrierte Varianten bleiben Fehler.                                                                                      |
| `-utag`                      | Neue Schreibweise in allen vorgesehenen Kommandos; alter Name mit hilfreicher Fehlermeldung; Namen bei Build-Kommandos zulässig, Gewicht/Farbe dort vor Änderungen abgewiesen; Host-Overrides funktionieren weiter.                                                                |
| Unveränderte Werte           | Aliasnamen, Custom-Tags mit unterschiedlicher Großschreibung, Pfade und C-Ausdrücke werden nicht durch die CLI-Anpassung verändert.                                                                                                                                                |
| Aliasregistrierung           | Beide Kurzformen, identische Wiederholungen, fehlende/leere Werte, ungültige Zeichen, reservierte Namen und abgewiesenes `Name:Tag`; bestehende Doppelregistrierungsregel in beiden Optionsreihenfolgen.                                                                           |
| Heuristik                    | Alle Suchwörter, gemischte Schreibweisen, sämtliche Beispiele der Ergebnistabelle, Mehrfachtreffer derselben Gruppe, längster Treffer, Gleichstand und `untagged`.                                                                                                                 |
| Fester Heuristikvertrag      | Zusätzliche Custom-Tags, geänderte Gewichte/Farben und Änderungen der allgemeinen Registry beeinflussen die festgelegten Namenszuordnungen nicht.                                                                                                                                  |
| Grundtyp                     | `-alias StatusS` bleibt skalar; `-salias LogErr` bleibt String. Namen wie `Wrn` und `Motor32AssertError` werden nicht als native Typen zerlegt.                                                                                                                                    |
| Hülle                        | Vollständige Hülle, nur Anfang, nur Ende, überlappende Randmarker, leere normale Meldung, Anführungszeichen, Backslashes und markerähnlicher Text mitten in der Meldung. Reservierte Randmarker in skalaren Aliasformaten scheitern vor Änderungen.                                |
| String-Argumentlisten        | Bestehende Sonderfälle ohne `%s` und mit anderem Formatplatzhalter bleiben erfassbar; der Host verwendet die empfangene Zeichenkette, nicht die gespeicherte C-Argumentliste.                                                                                                      |
| Bind/Insert                  | Originalname wird in `Type` geschrieben; Wiederholung hält IDs und Inhalte stabil; ungenutzte Aliasoption erzeugt keinen TIL-Eintrag.                                                                                                                                              |
| Historische IDs              | Alias-Rename, reine Schreibweisenänderung, Typwechsel und CE-Änderung deuten keine alte ID um; gleiche sichtbare Texte mit verschiedenen Aliasnamen bleiben getrennt.                                                                                                              |
| Alte und gemischte TIL       | Native Einträge behalten ihr Verhalten; neue Alias-Einträge werden ohne Alias-CLI gelesen; erneute Instrumentierung überschreibt keine alte normalisierte Aliaszeile.                                                                                                              |
| ID-Bereiche                  | Alias-Tag schlägt abweichendes Formatpräfix; Bereichsgrenzen, erschöpfte Bereiche und die bestehenden Regeln für Teilscans bleiben korrekt.                                                                                                                                        |
| Skalare CE                   | Aliasgruppe zuerst, Quellgruppen danach, jede Gruppe einmal, mehrere Regeln in CLI-Reihenfolge; freie Selektoren werden gezielt entfernt, Tag-Präfixe bleiben Text.                                                                                                                |
| String-Alias und CE          | Kein passender Selektor: normale Tag-Funktion. Passende CE-Gruppe: verständlicher Fehler, auch bei reiner Texterweiterung; keine Teiländerungen. Native `triceS`-Fälle bleiben separat abgedeckt.                                                                                  |
| Clean und Generatoren        | Insert/Clean mit denselben Einstellungen funktioniert; Bind-Metadaten erhalten Aliasidentität; JSON-Export erhält Hülle und Namen; typabhängige Ausgaben verwenden den Grundtyp.                                                                                                   |
| Text/JSON/KV                 | Die Meldungstabelle stimmt für aktive Paletten, `none` und `off`; kein verlorenes oder künstliches Präfix. Auch mehrzeilige Meldungen, mehrere Ereignisse pro Zeile und tag-artiger Parametertext sind abgedeckt.                                                                  |
| Filter und Statistik         | `-pick wrn` zeigt `MyWrn("err:bla")`, `-pick err` zeigt es nicht. Bei Standardgewichten zeigt `-loglevel wrn` das Ereignis, `-loglevel err` nicht. `LegacyLog("err:bla")` bleibt `untagged`. Ausgefilterte erfolgreiche Ereignisse zählen weiter gemäß bestehender Statistikregel. |
| Location und Nachladen       | Tag-Auswertung mit `-li off`; richtige Alias-Aufrufposition mit LI; gültiger Reload übernimmt eine konsistente Sicht, ungültiger Reload erhält die letzte gültige Sicht.                                                                                                           |
| Fehler und Wiederherstellung | Ungültige Definitionen, Formate und Schreibfehler hinterlassen keine teilweisen Quellen-/Dictionary-/Sidecar-Änderungen. Kurze oder fehlerhafte `Type`-Werte führen nicht zu Panics.                                                                                               |

Zusätzlich ist ein hardwarefreier Integrationstest erforderlich: Ein vorhandenes C-Testgerüst erzeugt tatsächlich einen skalaren Alias-Datensatz und einen String-Alias-Datensatz. Der Host dekodiert beide über die neue TIL in Text und JSON/KV. Damit wird geprüft, dass die angenommene Typauflösung auch zu den tatsächlich übertragenen Bytes passt. Das Testgerüst verwendet die vorhandenen Toolchains und führt keine neue Laufzeitbibliothek oder Abhängigkeit ein.

Nach den fokussierten Tests ist `./scripts/testAll.sh` ohne Argumente auszuführen. `./scripts/testAll.sh full` ist eine abschließende Prüfung bei dafür relevanten C-/Compileränderungen, kein Pflichtlauf für jeden Zwischenschritt.

Für geänderten Go-Code ist außerdem die Paket- und Coverage-Auswahl aus der [Coverage-Workflowdatei](../../.github/workflows/coverage.yml) einzuhalten; derzeit:

```sh
mkdir -p ./temp/log
go test -count=1 ./... -covermode=atomic -coverprofile=./temp/log/coverage.out -coverpkg=./...
```

Die Coveralls-artige abgedeckte Zeilenzahl ist mit dem Zielbranch zu vergleichen. Ein Rückgang muss mit genauer Ursache ausgewiesen werden; die gerundete Go-Statement-Prozentzahl genügt nicht. Go-Coverage ersetzt den C-Integrationstest nicht. Die spätere Abschlussmeldung muss ausgeführte, bestandene, fehlgeschlagene und ausgelassene Prüfungen eindeutig unterscheiden.

### 44.17. <a id="verbindliche-dokumentation"></a>Verbindliche Dokumentation

Die spätere Implementierung muss CLI-Hilfe und aktive Referenzdokumentation zusammen aktualisieren. Vorhandene Kapitel sollen erweitert werden; ein weiterer paralleler Gesamtentwurf ist nicht erforderlich.

| Dokumentationsbereich                                                               | Erforderlicher Inhalt                                                                                                                                                                      |
|-------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [Integration vorhandener Logging-Makros](../rm/legacy_user_code.md)                 | Zwei verständliche Beispiele für `-alias` und `-salias`; Projektverantwortung für den tatsächlichen Grundtyp; TIL-Beispiel mit Erklärung der Hülle; Doppelregistrierung und Namensgrenzen. |
| [Tags, Farben, Filter und Statistik](../rm/tags_color_and_weights.md)               | Feste Suchworttabelle, längster Treffer, Gleichstand, Fallback, akzeptierte Fehlzuordnungen und unveränderter Alias-Meldungstext.                                                          |
| [IDs und Dictionaries](../rm/id_management.md)                                      | Neue Bedeutung von `Type`, Unterschied gespeicherter Name/Grundtyp, ID-Wechsel bei Renames, gemischte TIL, Grenzen alter Tools und Versionsstabilität.                                     |
| [CLI und Logging](../rm/cli.md)                                                     | `-utag`, Wegfall von `-ulabel`, zusätzliche Schreibweise `-logformat`, weiterhin exakte Optionsnamen sowie kommandospezifische Werteformen.                                                |
| [Bind](../rm/bind.md) und [CE](../rm/context_enrichment.md)                         | Reihenfolge der Auswahl, ID-Bereiche, unveränderte Aufrufgrenzen und ausdrücklicher Ausschluss von CE-Änderungen an String-Aliasen.                                                        |
| [Structured Logging](../rm/structured_logging.md) und [Generate](../rm/generate.md) | Gleicher Tag in JSON/KV, vollständiger Meldungstext, korrekte Verwendung des Grundtyps und Grenze zur lokalen C-Logdarstellung.                                                            |
| [Changelog](../../CHANGELOG.md)                                                     | Kurzer Hinweis auf automatische Alias-Tags, inkompatible `Type`-Bedeutung, `-utag` und relevante Grenzen; Details über relative RM-Links.                                                  |

Ein kleines ausführbares Beispiel muss ohne Hardware die skalare Aliasvariante, einen String-Alias und skalare CE zeigen. Es verwendet die neue Syntax und dokumentiert die erwarteten Tags und Meldungen. Freie `Name:Tag`-Zuordnungen dürfen nicht als bereits verfügbare Funktion erscheinen; sie werden nur als mögliche Erweiterung einer späteren Version genannt.

Der bereits vorgeschlagene kurze RM-Abschnitt zur LI-Archivierung bleibt Teil der Dokumentationsarbeit: finale LI über einen Inhalts-Hash identifizieren, unverändert extern archivieren, passenden Datensatz extern auswählen und über `-li` verwenden. ZIP-Dateien werden extern entpackt. Dafür werden weder Hash-Erzeugung noch Archivverwaltung oder LI-Historienfelder in Trice implementiert.

Aktive Beispiele, Inhaltsverzeichnisse und Links sind auf Konsistenz zu prüfen. Archivierte Dokumente bleiben unverändert. Neue Codekommentare und öffentliche CLI-Hilfetexte werden auf Englisch geschrieben; diese Diskussionsspezifikation bleibt deutsch.

### 44.18. <a id="noch-zu-prüfende-detailentscheidungen"></a>Noch zu prüfende Detailentscheidungen

Die folgende Tabelle trennt neu konkretisierte Vorschläge von der bereits beschriebenen Grundidee. Die vorherigen Abschnitte verwenden jeweils die hier genannte Variante, sodass daraus nach der inhaltlichen Prüfung eine eindeutige Implementierungsaufgabe entsteht.

| Punkt                 | In dieser Spezifikation verwendeter Vorschlag                                                          | Bedeutung für die Prüfung                                                                                         |
|-----------------------|--------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------|
| Kein Namenstreffer    | `untagged`, kein Rückgriff auf das Meldungspräfix                                                      | Auch `LegacyLog("err:...")` wird als Alias nicht ERROR.                                                           |
| Mehrere Namenstreffer | Längster Treffer, danach feste Tabellenreihenfolge                                                     | Beispielsweise wird `Log_Error_Warning` WARNING.                                                                  |
| Alias-Renames         | Exakter Name ist Teil der ID-Identität                                                                 | Auch ein Rename ohne Tag-Wechsel kann eine andere ID erfordern.                                                   |
| Reservierte Formen    | `trice*` für native Namen; Randmarker der String-Alias-Hülle für String-Aliase                         | Manche bisher möglichen Projektnamen beziehungsweise skalaren Texte werden abgewiesen.                            |
| String-Alias und CE   | Passende CE-Regeln ablehnen, einschließlich reiner Texterweiterungen                                   | Vermeidet eine nicht definierte Änderung der Hülle; gemischte Gruppen benötigen gezielte skalare Quellselektoren. |
| CLI-Vereinfachung     | `-utag` ohne alten Kompatibilitätsnamen; zusätzlich `-logformat`; sonst keine allgemeine Case-Toleranz | Klar benannter v2-Bruch und eine gezielte kompatible Ergänzung.                                                   |

Die bestehende Doppelregistrierungsregel wird bewusst erhalten, obwohl eine künftig strengere Ablehnung derselben Namen unter beiden Aliasarten einfacher zu erklären wäre. Eine solche zusätzliche Änderung ist in dieser Spezifikation nicht enthalten.

Die früheren Meta-Entwürfe, Alias-Overrides und automatischen LI-/TIL-Historienfunktionen sind keine offenen Pflichtpunkte dieser Umsetzung. Prüfpflichtig bleiben dagegen die genaue Heuristik, die hier erklärte Typunterscheidung, die String-Alias-CE-Grenze sowie die Konsistenz von Instrumentierung, Historie und Host-Ausgabe.

## Notizen

- Frage: 44.8.: "Bestehende Sonderregel anhand des letzten Buchstabens" - Ich sehe keinen Anwendungfall für -alias und -salias auf den gleichen Namen, Ist das nicht ein CLI Fehler?
- Ergänzung: Ungenutzte CLI Aliase schweigend ignorieren, aber bei -verbose anzeigen.
- Frage: 44.13.: "Für String-Aliase wird in dieser Ausbaustufe nur die Tag-Zuordnung unterstützt. Eine über ihren Tag ausgewählte CE-Regel wird vor Änderungen abgewiesen, auch wenn sie nur Text anhängen würde.” Ich verstehe diese Einschränkung nicht. Stein Aliase benutzen intern printf in einen String und sollten doch daher beliebig erweiterbar sein, wenn dabei auch der eigentliche Vorteil ID statt String verloren gent. Aber als Ausnahme-Spezialfall doch interessant. Ich möchte zunächst die Problematik genau verstehen.

## KLärungbedarf

- "Beide Optionen für denselben Namen" in 44.8. - Wo ist der Sinn? Sollte das nicht ein Fehler sein?
- "String-Aliase dürfen im Rahmen der bestehenden Erkennung auch projektspezifische Wrapper mit anderer Argumentliste bezeichnen." - Das würde ich gerne genauer verstehen. Heißt das, dass zB %d und %s in -salias Funktionen mehrfach gemischt vorkommen dürfen? Vermutlich printed ein print zunächst in einen Laufzeitstring, der dann übertragen wird. Das widerspricht zwar der ID Idee, bietet aber für Spezialfälle Flexibilität.