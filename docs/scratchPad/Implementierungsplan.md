# Release-Vorbereitung und weiterer Arbeitsplan

[Hintergrund, Beschlüsse und Vorbemerkungen](#hintergrund-und-vorbemerkungen)

## Aufgabenübersicht

| ID | Status | Aufgabe | Gewicht | Aufwand | Voraussetzung |
| --- | --- | --- | ---: | --- | --- |
| [R18](#bisheriges-user-manual-als-reference-manual-weiterführen) | Offen | Bisheriges UM in TriceReferenceManual.md umbenennen und Pfade nachziehen | 4 | M | [R10](#anwenderdokumentation-von-entwicklungsständen-befreien), [R11](#sl--und-ce-kapitel-vollständig-ins-englische-übertragen), [R17](#bestandszuordnung-und-befunde-der-repo-prüfung) |
| [R19](#ein-kurzes-user-manual-zum-ausprobieren-erstellen) | Offen | Kurzes, einladendes User Manual erstellen | 4 | M | [R18](#bisheriges-user-manual-als-reference-manual-weiterführen); Installationsentscheidung aus [R05](#kompatibilitätsvertrag-und-release-ziel-festgelegt)/[R14](#checkout-binary-installationswege-für-v2-absichern) |
| [R23](#beispielanleitungen-zentralisieren-und-readmes-auf-links-reduzieren) | Offen | Beispielanleitungen ins UM/RM übernehmen; Beispiel-READMEs auf Links reduzieren | 4 | M | [R18](#bisheriges-user-manual-als-reference-manual-weiterführen), [R19](#ein-kurzes-user-manual-zum-ausprobieren-erstellen); Inhalte vor Kürzung zuordnen |
| [R12](#readme-repo-orientierung-beispiele-und-zusagen-verbessern) | Offen | Root-README und Repo-Orientierung einladend überarbeiten; Zusagen präzisieren | 4 | M | [R05](#kompatibilitätsvertrag-und-release-ziel-festgelegt), [R17](#bestandszuordnung-und-befunde-der-repo-prüfung), [R18](#bisheriges-user-manual-als-reference-manual-weiterführen), [R19](#ein-kurzes-user-manual-zum-ausprobieren-erstellen), [R23](#beispielanleitungen-zentralisieren-und-readmes-auf-links-reduzieren) |
| [R20](#link-forwarding-dateien-entfernen-und-docs-konsolidieren) | Offen | Link-Forwarding-Dateien entfernen und aktive docs konsolidieren | 4 | S–M | [R12](#readme-repo-orientierung-beispiele-und-zusagen-verbessern), [R17](#bestandszuordnung-und-befunde-der-repo-prüfung), [R18](#bisheriges-user-manual-als-reference-manual-weiterführen), [R19](#ein-kurzes-user-manual-zum-ausprobieren-erstellen) |
| [R21c](#root-ausgaben-und-generierte-beispieldaten-unterscheiden) | Erledigt | Root-Ausgaben und generierte Beispieldaten unterscheiden | 3 | S–M | [R17](#bestandszuordnung-und-befunde-der-repo-prüfung) |
| [R21d](#unfertige-tools-ruhende-tests-und-entwicklernotizen-einordnen) | Erledigt | Unfertige Tools, ruhende Tests und Entwicklernotizen einordnen | 3 | S–M | [R10](#anwenderdokumentation-von-entwicklungsständen-befreien) |
| [R21e](#dokumentationsbilder-und-vergleichsberichte-konsolidieren) | Erledigt | Dokumentationsbilder und Vergleichsberichte konsolidieren | 3 | M | [R10](#anwenderdokumentation-von-entwicklungsständen-befreien), [R12](#readme-repo-orientierung-beispiele-und-zusagen-verbessern), [R18](#bisheriges-user-manual-als-reference-manual-weiterführen)–[R20](#link-forwarding-dateien-entfernen-und-docs-konsolidieren) |
| [R21f](#fremdsoftware-ablage-erklären-und-alt-konfiguration-abgleichen) | Offen | Fremdsoftware-Ablage und alte Linkchecker-Konfiguration abgleichen | 3 | S–M | [R10](#anwenderdokumentation-von-entwicklungsständen-befreien), [R18](#bisheriges-user-manual-als-reference-manual-weiterführen), [R20](#link-forwarding-dateien-entfernen-und-docs-konsolidieren) |
| [R24](#entwicklerwerkzeuge-vom-scratchpad-entkoppeln) | Offen | Codex-Umzugswerkzeuge, Tests und Anleitung gemeinsam aus aktiven Scratchpad-Abhängigkeiten lösen | 3 | M | Eigenständiger Entwicklerablauf; ZIP-/Resume-Verhalten erhalten |
| [R22](#github-pages-mit-eindeutigem-einstieg-und-veröffentlichungsumfang) | Offen | Pages-Einstieg und Veröffentlichungsumfang eindeutig machen | 4 | M | [R18](#bisheriges-user-manual-als-reference-manual-weiterführen), [R19](#ein-kurzes-user-manual-zum-ausprobieren-erstellen), [R12](#readme-repo-orientierung-beispiele-und-zusagen-verbessern), [R20](#link-forwarding-dateien-entfernen-und-docs-konsolidieren); Bestandsbefund aus [R17](#bestandszuordnung-und-befunde-der-repo-prüfung) |
| [R14](#checkout-binary-installationswege-für-v2-absichern) | Offen | Checkout-/Binary-Installationswege für v2 absichern | 5 | S–M | [R05](#kompatibilitätsvertrag-und-release-ziel-festgelegt) abgeschlossen; kein `/v2` beschlossen |
| [R15](#release-notes-und-ausgelieferte-dateien-prüfen) | Offen | Release Notes und Prüfung der ausgelieferten Artefakte | 5 | M | [R05](#kompatibilitätsvertrag-und-release-ziel-festgelegt), [R07](#ce-sl-integration-und-feature-beispiele-verbindlich-ausgewählt), [R11](#sl--und-ce-kapitel-vollständig-ins-englische-übertragen), [R12](#readme-repo-orientierung-beispiele-und-zusagen-verbessern), [R14](#checkout-binary-installationswege-für-v2-absichern), [R18](#bisheriges-user-manual-als-reference-manual-weiterführen)–[R20](#link-forwarding-dateien-entfernen-und-docs-konsolidieren), [R22](#github-pages-mit-eindeutigem-einstieg-und-veröffentlichungsumfang), [R23](#beispielanleitungen-zentralisieren-und-readmes-auf-links-reduzieren) |
| [R16](#release-abnahme-auf-einem-feststehenden-stand) | Offen | Abschließende Release-Abnahme | 5 | M; lange Laufzeit | [R01](#kein-automatisch-erzeugtes-untagged-präfix-ausgeben)–[R05](#kompatibilitätsvertrag-und-release-ziel-festgelegt), [R07](#ce-sl-integration-und-feature-beispiele-verbindlich-ausgewählt), [R11](#sl--und-ce-kapitel-vollständig-ins-englische-übertragen), [R13](#test-ausgangszustand-einschließlich-standalone-beispielen-erhalten), [R15](#release-notes-und-ausgelieferte-dateien-prüfen); alle aufgenommenen Korrekturen einschließlich Repo-Aufräumen |

Die weiter unten aufgeführten P- und F-Aufgaben sind kein Grund, ein ansonsten abgenommenes Release um neue Features zu vergrößern.

[Beschleunigung](#weitere-beschleunigung-ohne-geringere-abdeckung) · [Optionale Erweiterungen](#sinnvolle-erweiterungen-zur-späteren-diskussion) · [Erledigte Aufgaben](#erledigte-korrekturen) · [Repo-Bestandsprüfung](#bestandszuordnung-und-befunde-der-repo-prüfung)

## Konkrete Aufgaben vor dem Release

### Anwenderdokumentation von Entwicklungsständen befreien

**R10 · Gewicht 4 · Aufwand M · Erledigt**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Ausgangsbefund: Im aktiven UM standen MVP-Bezeichnungen sowohl im `-vis`-Kapitel als auch ausführlich im Bind-Kapitel. Das inzwischen [archivierte deutsche Bind-Manual](obsolete/TriceBind/Trice_bind_90_MVP_User_Manual.md) und die dortige README präsentierten parallel eine weitere normative Anwendersicht. Die Aufgabenreste im CE-Anhang sind mit R11 bereinigt. Der früher beanstandete Bitbreiten-Link ist inzwischen vom Benutzer auf `#trice-parameter-bit-widths` korrigiert und kein offener Auftrag mehr.

Aufgabe: Das bisherige UM als künftiges Reference Manual zur eindeutigen fachlichen Referenz machen. Das neue kurze User Manual führt später durch die Nutzung und verweist für vollständige Verträge dorthin. Aktuelle Grenzen konkret benennen; „MVP“ nicht blind durch „vollständig unterstützt“ ersetzen. Reine Architekturhistorie liegt außerhalb der Anwenderreferenz; nützliche Gründe für aktuelle Grenzen bleiben verständlich erklärt und dürfen aktueller Wrapper-Unterstützung nicht widersprechen. Implementierungsaufträge/Entwurfsberichte nach Prüfung aus dem aktiven Einstieg nehmen; wertvolle Begründungen erhalten. Die tatsächliche Dateibereinigung folgt R20/R21.

Die CE-PoC-Ergebnisse bleiben wie beauftragt im kapitelinternen Anhang, einschließlich reproduzierbarer Testreferenzen und ihrer Aussagegrenzen. A9/A10 sind dort im vorgezogenen R11 bereits durch verständliche Bezeichnungen für den direkten Nachweis und die produktive Unterstützung ersetzt. Testnamen und Experimentpfade werden nicht nur wegen eines historischen Namens umbenannt.

R17 konkretisiert den Abgleich: Die neun Dateien unter `docs/TriceBind` enthalten neben dem parallelen User Manual auch Spezifikationen, Testanforderungen, Implementierungsprompts, einen Bericht und Architekturbegründungen. Zuerst die noch gültigen, nur dort vorhandenen Aussagen der Vollreferenz zuordnen. Danach die eigenständigen Entwicklungsnachweise als historische Dokumentation einordnen und aktive normative Verweise umstellen. Abgeschlossene Experimente dürfen nach Archivierung nicht mehr von Schritt 500 aufgerufen werden; relevante Produktverträge werden durch aktive Integrationstests abgesichert. Die beiden aus dem README verlinkten KI-Vergleichsberichte in `docs` sind datierte Fremdeinschätzungen, keine zweite Produktspezifikation; ihre Rolle im Einstieg gehört zu R12, ihre Ablage zu R21e.

**Abnahme:** Aktive Anwendertexte enthalten keine unerklärten Arbeitsauftragsnummern oder überholten MVP-Status. Vorhandene Archive bleiben unangetastet. Kommentarblöcke und historische Changelogs werden nicht als neue Produktanforderungen behandelt.

**Ergebnis:** Das aktive Bind-Kapitel ist nun die eindeutige Anwenderreferenz. Die Visualisierung beschreibt ihre aktuelle Syntax und positionalen Felder ohne MVP-Status. Der separate deutsche Bind-Text ist ausdrücklich ein historischer Entwicklungsstand; sein ursprünglicher Haupttext bleibt erhalten. Die Bind-README führt zuerst zur aktuellen Referenz und erklärt Spezifikationen, Prompts, Bericht und Strategien als Entwicklungsnachweise. Ihre alten Startkommandos und vorgeschlagenen Erweiterungen sind keine heutigen Aufträge.

| Abgeglichener Bestand | Übernahme beziehungsweise heutige Rolle |
| --- | --- |
| Deutsches Bind-Manual (90) | Anwenderablauf, Includes, Dateiklassifikation, ID-/Stamp-Formen, Rebase, Diagnosen und Anhänge sind bereits im englischen Bind-Kapitel enthalten. Alle 49 Code-/Textblöcke sind zugeordnet: 42 stehen wörtlich im UM, sieben haben englisch übersetzte CLI-Platzhalter oder Erläuterungen mit gleicher Bedeutung. Kein Beispiel ging verloren. |
| Generator-Spezifikation (10) | Aktuell gültige, noch nicht ausdrückliche Details ergänzt: leere Besitzerdatei behält Key/Include, Key-Format und Persistenz, guard-freie Besitzer-Sidecars, Ausschluss von `-genDir` aus dem Scan und Vorrang der expliziten Stamp-Nullform. Die spätere Wrapper-/Rebase-Unterstützung ersetzt die damaligen pauschalen Ausschlüsse. |
| Testdesign, Prompts und Bericht (20/30/40/50/70) | Historische Aufgaben-/Testnachweise liegen nun unter `obsolete/TriceBind`. Der neue englische [Bind-/Insert-Testanhang](../TriceUserManual.md#appendix-bind-and-insert-test-evidence) bündelt die weiterhin gültige Architektur, Zustandswiederherstellung und aktuellen Einstiege. Keine alten Versions-, CLI-, Compiler- oder PASS-Aussagen als heutige Release-Abnahme übernommen. |
| Strategiepapier (60) | Der englische [Architekturvergleich](../TriceUserManual.md#appendix-why-bind-uses-local-counter-rebasing) erklärt Auswahl, Nutzen, Kosten und Grenzen aller drei Ansätze. Das vollständige historische Alternativenpapier liegt im Archiv. Keine neuen Buildschritte; produktives CE für Wrapper/Rebase bleibt zurückgestellt. |
| ELF-Architekturvergleich im UM | Der vollständige englische Entscheidungstext ist inzwischen separat archiviert. Im UM bleiben die aktuellen Bind-Abläufe, Compileranforderungen und CE-Grenzen; historische MVP-Annahmen sind keine Anwenderreferenz. |

**Gezielte Prüfung:** Bestehende Tests und Renderer belegen die ergänzten Bind-Details. Dokumentationsprüfungen kontrollieren Markdown, UM-Format, relative Links und Fragmentziele; Vergleich mit HEAD sichert alle UM-Codeblöcke, den vollständigen übernommenen ELF-Entscheidungstext und den ursprünglichen Haupttext des deutschen Bind-Manuals. Produktcode, Testnamen, Experimente und bestehende Archive sind unverändert. Die tatsächliche Umbenennung und Ablagebereinigung bleiben R18/R20/R21.

**Ergänzende Archivierung auf Benutzerauftrag:** Die neun Bind-Dokumente sind nach Inhaltsabgleich nach [obsolete/TriceBind](obsolete/TriceBind/README.md) verschoben; ausschließlich ihre durch die Verschiebung betroffenen relativen Links sind angepasst. Aktive Anwenderverweise führen zum Handbuch; das Archiv dient ausschließlich der internen Historie. Anwenderinformationen des deutschen Manuals und die ELF-Begründung waren bereits vollständig englisch übernommen; ergänzt sind der Architekturvergleich, die Testübersicht und der Hinweis, dass `TRICE_CLEAN=1` fehlende Include-Dateien nicht ersetzt. Implementierungsprompts und damalige PASS-Berichte bleiben historische Nachweise, keine neuen Anforderungen.

`docs/trice_logging_handover2_de` enthielt nur noch leere Verzeichnisse; seine Dokumente liegen bereits vollständig unter [obsolete/trice_logging_handover2_de](obsolete/trice_logging_handover2_de). Die aktuellen Verträge stehen englisch in [Tags/Filtern](../TriceUserManual.md#trice-tags-color-and-weights), [Structured Logging](../TriceUserManual.md#structured-logging) und [Context Enrichment](../TriceUserManual.md#trice-context-enrichment), ergänzt durch die ID-Policy- und Routingabschnitte. Überholte Entwurfsoptionen, Gewichte, Ausgabeformen und Statusangaben werden nicht als aktuelle Produktaussagen wieder eingeführt. Der leere Restordner ist entfernt; das vorhandene Handover-Archiv bleibt unverändert. R20 bleibt für die übrigen aktiven Weiterleitungsdateien offen; die beiden hier benannten Entwicklungsordner sind kein offener Verschiebeauftrag mehr.

**Aktueller Stand:** Die neun Bind-Unterlagen und der frühere Handover sind archiviert. Ihre vorhandenen Inhalte werden nicht nachträglich repariert. Die aktive Anwenderreferenz benötigt keine Archivverweise; der englische historische ELF-Vergleich ist dorthin ausgelagert. Der frühere Linkfehler zu den alten Encodings ist beseitigt. Die Anleitung zum Rechnerwechsel liegt derzeit hier im Scratchpad und wird mit ihren Werkzeugen in R24 gemeinsam eingeordnet.


### Bisheriges User Manual als Reference Manual weiterführen

**R18 · Gewicht 4 · Aufwand M · Nach R10/R11; vollständige Inhalte erhalten**

[Zur Aufgabenübersicht](#aufgabenübersicht)

`docs/TriceUserManual.md` in `docs/TriceReferenceManual.md` umbenennen und Titel, Selbstverweise sowie aktive eingehende Verweise entsprechend anpassen. Die Umbenennung selbst ist keine Kürzung: Gültige Anwenderverträge, Beispiele, Einschränkungen und die zum Verständnis erforderlichen Hintergründe/Anhänge bleiben vollständig. Die gesonderte Bereinigung reiner Entwicklungshistorie nach R20 ist kein Inhaltsverlust bei der Umbenennung. Der bisherige Pfad wird unter R19 für das neue kurze UM verwendet; alte Kapitelverweise müssen gezielt zum Reference Manual führen, statt unbemerkt im neuen UM zu landen.

Die Pfadänderung durchgängig berücksichtigen: Dokumentationspflege und mdtoc, Markdown-/Linkprüfungen, PDF-Erzeugung mit Dateiname und Kopfzeile, GitHub Pages, Release-Paketierung und Artefaktprüfungen sowie aktive Anleitungstexte und Agentenregeln. Konkrete vorhandene Verbraucher sind unter anderem `scripts/_310_refresh_trice_user_manual.sh`, `scripts/_320_generate_trice_user_manual_pdf.sh`, `scripts/_610_test_goreleaser_snapshot.sh`, `.goreleaser.yaml` und die Pages-/Installations-/Release-Workflows. Nur tatsächlich nötige Pfadanpassungen vornehmen, keine allgemeine Skriptumbenennung.

**Abnahme:** Vollständiger Inhaltsvergleich vor/nach der Umbenennung; Reference Manual als Markdown/PDF erzeugbar, Links und veröffentlichte Ziele stimmen. Suchhinweise wie `bind-limits` bleiben auffindbar. Archive bleiben unverändert; dadurch veraltete Archivverweise als verbleibende Folge dokumentieren, ohne neue Weiterleitungsdateien anzulegen. R19 ergänzt danach das neue UM samt derselben erforderlichen Dokumentationsprüfungen.

### Ein kurzes User Manual zum Ausprobieren erstellen

**R19 · Gewicht 4 · Aufwand M · Geführter Einstieg statt zweiter Vollreferenz**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Ein neues englisches `docs/TriceUserManual.md` erstellen, das deutlich kürzer als die bisherige Vollreferenz ist. Einstieg mit wenigen Sätzen zu Nutzen und Funktionsweise, anschließend Voraussetzungen, Installation gemäß R14 und ein ausführbares PC-Beispiel mit erwarteter Ausgabe. Danach die Schritte zur eigenen Target-Anbindung zeigen, mit einem klaren Standardweg und passenden Verweisen für alternative ID-Workflows und Transportwege.

Bestätigte Zielstruktur: **Das kurze UM bleibt ausdrücklich erhalten.** Es führt Neuankömmlinge durch einen nachvollziehbaren Weg und verweist gezielt ins RM, zu ausführbaren Beispielen und bei Bedarf in Code und Tests. Die Root-README ist die kurze Projektvorstellung. Beispiel-READMEs enthalten künftig ausschließlich Links auf passende UM-/RM-Kapitel; sie sind keine zusätzlichen Anleitungsorte. Vom ersten Log bis zum nächsten sinnvollen Versuch soll kein Zusammensuchen verteilter Readme-Texte nötig sein.

Tags/Filter, Stempel, Structured Logging und Context Enrichment anhand kleiner Änderungen und sofort sichtbarer Ausgaben vorstellen. Vorhandene PC-/G0B1-Feature-Touren und ihre `show_*.sh`-Skripte verwenden; keine neuen Features oder Beispielprojekte allein für das neue Handbuch. Kurz erklären, welche Dateien dauerhaft zum Projekt gehören und welche generiert werden. Eine knappe Fehlerhilfe soll den nächsten sinnvollen Prüfschritt nennen. Eine kompakte Beispielauswahl nennt Nutzen und den nächsten Versuch und verlinkt das ausführbare Projekt sowie dessen ausführliche Anleitung im RM. Code-/Testverweise sind weiterführende Erklärungen und Nachweise, keine Voraussetzung für das erste Ausprobieren.

Vollständige Optionslisten, Compiler-Matrizen, Protokolldetails und PoC-Begründungen bleiben im Reference Manual. Erforderliche Einschränkungen stehen dort im Einstieg, wo sie die konkrete Wahl beeinflussen, ohne den Beginn mit allen Sonderfällen zu überladen. Begriffe beim ersten Auftreten erklären; keine Vorkenntnis von internen Auftragsnummern oder Architekturentwürfen verlangen.

**Abnahme:** Vom sauberen Checkout bis zum ersten PC-Log ist der beschriebene Weg reproduzierbar; Voraussetzungen und erwartete Ausgabe sind sichtbar. Der Umfang ist deutlich reduziert, die zentralen Nutzeraufgaben sind auffindbar und Detailverweise funktionieren. Das UM führt zusammenhängend durch den Einstieg; zusätzliche fachliche Erklärung wird im RM gefunden, ohne Beispiel-READMEs durchsuchen zu müssen. Keine fachlichen Abweichungen zur Referenz. Beide Handbücher sind als Markdown/PDF prüfbar und für die Veröffentlichung unter ihren eindeutigen Namen vorbereitet.

### Beispielanleitungen zentralisieren und READMEs auf Links reduzieren

**R23 · Gewicht 4 · Aufwand M · Neue, ausdrücklich bestätigte Dokumentationsaufgabe**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die eigenen Beispiel-READMEs unter `demo` und `examples` einschließlich der Verzeichniswegweiser und eigener Unterprojekte vollständig auf Inhalte prüfen. Ihre fachlichen Erklärungen, Voraussetzungen, Aufrufe, erwarteten Ausgaben, Experimentierhinweise und Fehlerhilfen nach Zweck ins kurze UM oder vollständige RM übernehmen. Der kurze Standardweg gehört ins UM; besondere Compiler-/Hardwareeinrichtung, Projektvarianten, sämtliche Logskripte, generierte Dateien, Visualisierungsaufbau und Detaildiagnosen ins RM. Fremdquellen und Lizenznachweise bleiben erhalten; diese Aufgabe konsolidiert die Trice-Anwenderanleitungen, nicht Vendor-Code oder vorhandene Archive.

Für jedes Beispiel vor der Kürzung festhalten, welche UM-/RM-Abschnitte seine bisherige Anleitung ersetzen. Dabei insbesondere `demo` Direct/Deferred, PC-/G0B1-Feature-Touren, Target-seitige Formatierung, ABC mit `NodeLib`/Auswahl-Header/Generatoren, CSV-/Trice-Datenproduzenten und den LabPlot-Nachbau berücksichtigen. Die Erklärung zur Anpassung von `PC_features/check_output.sh` nach Source-/Logskriptänderungen ebenfalls zentral erhalten. Inhalte, die bereits im Handbuch stehen, zusammenführen; keine zusätzliche Vollanleitung pro Beispiel im UM erzeugen.

Erst nach vollständiger Inhaltsübernahme die eigenen `README.md`/`ReadMe.md` der Beispiele auf **ausschließlich relative Links ins UM oder RM** reduzieren. Aussagekräftige Linktexte nennen das Beispiel und gegebenenfalls Einstieg, Aufbau oder Ausgabeoptionen. Dort keine eigenständigen Absätze, Kommando-/Codeblöcke, Voraussetzungen, Screenshots oder Fehleranleitungen mehr pflegen. Die ausführbaren Quellen und Skripte bleiben am Ort; UM/RM verlinken direkt auf sie und passende Verhaltenstests. Codekommentare behalten ihren normalen Erklärungszweck und werden nicht als Ersatz für zentrale Anwenderdokumentation ausgebaut.

**Abnahme:** Alle bisherigen Anwenderinformationen sind einer zentralen Zielstelle zugeordnet und übernommen oder als bereits vorhanden nachgewiesen. Beispiel-READMEs bestehen ausschließlich aus gültigen UM-/RM-Links; das gilt auch für die Nachbauanleitung, die bislang allein unter `LabPlotUser` liegt. Der Pfad README → UM/RM → ausführbares Beispiel funktioniert mit korrekter Groß-/Kleinschreibung. Neue Anwender finden die vollständige Anleitung über das UM. Markdown, Fragmente, Bilder und beide PDFs prüfen; dokumentierte Aufrufe anhand vorhandener Beispiele und gezielter Ausgabeprüfungen validieren. Keine Veränderung von Produktverhalten, Tests oder Buildabläufen allein für die Dokumentationskonsolidierung.

### README, Repo-Orientierung, Beispiele und Zusagen verbessern

**R12 · Gewicht 4 · Aufwand M · Mit R17–R19 abgestimmt**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die Root-[README](../../README.md) nennt Bind noch experimentell und unveröffentlicht; das muss zum gewählten Release-Status passen. Sie verweist bei Structured Logging noch auf einen auskommentierten Future-Draft. SL und CE gehören in die Feature-Übersicht mit kurzen Beispielen und Links auf den geführten UM-Einstieg; von dort führen zentrale Anleitungen zu den Feature-Touren und ins RM.

Das README einladend und übersichtlich überarbeiten: ein kurzer Nutzenabschnitt, ein verständliches Code-/Ausgabebeispiel, ein klarer Einstieg über das neue User Manual und gezielte Links zur Vollreferenz. Lange Detaildiskussionen in das fachlich zuständige Handbuch überführen; weder Vollreferenz noch neues UM im README wiederholen. Installation aus Checkout beziehungsweise Binaries entsprechend der bestätigten Entscheidung erklären.

Eine kompakte Repo-Karte nach Nutzeraufgaben aufnehmen: Wo anfangen, Beispiele ausprobieren, Target-Code einbinden, Hosttool bauen, Tests ausführen und Details nachschlagen? Die wichtigsten Verzeichnisse erhalten je eine kurze Zweckbeschreibung und einen sinnvollen Einstiegspunkt. Basis ist die vollständige Bestandsprüfung aus R17; die öffentliche Karte zählt nicht jede Einzeldatei auf. `docs/README.md` erklärt die Rollen der beiden Handbücher und der übrigen relevanten Dokumente. Beispiel-READMEs sind die unter R23 festgelegten reinen UM-/RM-Verweise; keine zusätzlichen dezentralen Anleitungen oder neue README pro Ordner allein aus formalen Gründen anlegen.

Weitere konkrete Ergänzungen ausschließlich im passenden Handbuch erläutern und von dort auf die bestehenden Beispiele verlinken:

- Ein kleines Verzeichnisbeispiel für `-genDir`: relativ zum Aufrufverzeichnis, Sidecars im Include-Pfad, persistente TIL/LI gegenüber generierten Dateien, `-logC` und ABC-Ausgaben.
- Erklären, warum beim G0B1-Beispiel auch gemeinsame Quellen Sidecars erzeugen können, obwohl ihre Demo-Funktion nicht aufgerufen wird: Scan-/Buildumfang und tatsächliche Laufzeitaufrufe sind verschiedene Dinge.
- Zum Aufräumen keine pauschale Löschung von `generated` empfehlen: Bei ABC kann dort eine vom Benutzer bearbeitete Auswahl-Headerdatei liegen. Alte Sidecars können außerdem historische ID-Evidenz liefern; Details siehe F01.
- Compiler-Zusagen getrennt für gewöhnliches Bind, direkte CE und experimentelles Rebase aufführen. C++20 mit strengen Warnungen scheitert laut vorhandenem PoC schon am Enum-Rebase; MSVC/IAR/armclang sind dort nicht nachgewiesen. Cross-Compile ist keine MCU-Laufzeitabnahme.
- Das Feature „keine dynamische Speicherverwaltung“ auf den Trice-Target-Loggingpfad beziehen. Stack-Puffer sind keine statischen Objekte; Hosttool, RTOS und benutzereigene CE-Funktionen sind nicht von derselben Zusage umfasst.
- ABC-Einstieg mit einer kurzen Karte von `NodeLib`, Auswahl-Header, generierter C-Tabelle und Buildausgabe erklären. Die heutige Dokumentation weiterverwenden; keine allgemeine Skript-Neuorganisation erforderlich.

**Abnahme:** Ein neuer Anwender erkennt Nutzen und ersten Schritt, findet seine Aufgabe in der Repo-Karte, versteht die Ablage und kann CE/SL ohne Kenntnis von A-/M-Aufträgen ausprobieren. Root-README und kurzes UM laden ein, statt mit der vollständigen Options- und Sonderfallliste zu beginnen. Die Navigation führt über UM/RM zu den Beispielen; Beispiel-READMEs enthalten nur die unter R23 festgelegten Links. Grenzen sind erreichbar und korrekt; Release-Zusagen decken sich mit nachgewiesenen Plattformen und Funktionen.

### Link-Forwarding-Dateien entfernen und docs konsolidieren

**R20 · Gewicht 4 · Aufwand S–M · Nach Festlegung und Befüllung der Zieldokumente**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Reine Weiterleitungsdateien aus dem aktiven `docs`-Bestand entfernen. Konkrete Beispiele sind `docs/TriceUserGuide.md`, `docs/TriceColor.md` und `docs/TriceIDManagement.md`; die vollständige Auswahl liefert R17. Nicht allein nach Dateinamen löschen: Eigenständige Informationen gegebenenfalls zuvor ins passende Handbuch oder andere begründete Zieldokument übernehmen. Der neue Dokumentationswegweiser `docs/README.md` bleibt wegen seines eigenen Orientierungszwecks erhalten.

Vor dem Entfernen alle aktiven eingehenden Links auf das fachlich passende Kapitel im kurzen UM oder im Reference Manual umstellen. Pfade, Anker, Bilder und Downloads im Repo, auf GitHub Pages und in PDFs prüfen. Keine neuen Markdown-Weiterleitungsstubs als Ersatz erzeugen. Nicht kontrollierbare externe Altlinks und unveränderte Archivverweise als verbleibende Folgen benennen; bestehende Archive dafür nicht bearbeiten. Doppelte aktive Dokumente zusammenführen, sobald ihre einzigartigen Inhalte und etwaige historischen Nachweise zugeordnet sind.

**Weitere redaktionelle Prüfung:** Den umfangreichen Handbuchbestand auf weitere interne Entwicklungsberichte, Vergleichspapiere, datierte PoC-Verläufe und auskommentierte Entwürfe prüfen. Aktuelle Syntax, Beispiele, Grenzen und nachvollziehbare Gründe erhalten; reine Entwicklungsabläufe aus dem Anwendertext nehmen und gegebenenfalls im Scratchpad ablegen. Den ausdrücklich gewünschten CE-Anhang nicht pauschal löschen: Seine Beispiele und Testreferenzen dienen dem Verständnis der verbleibenden Grenzen. Einzelne historische Details gegebenenfalls kürzen, ohne die aktiven CE-Tests zu entfernen. Die dafür notwendige Inhaltszuordnung gehört mit R18/R19 zur größeren Handbuchredaktion.

Die unter R23 ausdrücklich gewünschten Link-READMEs in `demo`/`examples` sind davon ausgenommen: Sie bleiben als lokale Verweise auf UM/RM erhalten. Entfernt werden die zusätzlichen einzelnen Themen-Weiterleitungen im aktiven `docs`-Bestand, nicht die beschlossene Navigation der Beispielprojekte.

**Abnahme:** Keine reinen Link-Forwarding-Dateien mehr im aktiven `docs`-Bestand, keine aktiven Verweise auf entfernte Dateien und kein Verlust eigenständiger Informationen. Jeder verbleibende aktive Dokumentationsbestand hat eine nachvollziehbare Aufgabe; lokale Link-/Ankerprüfungen und die betroffenen Veröffentlichungswege bestehen.

### Übriges Repo anhand belegter Zwecke aufräumen

**R21 · Gewicht 3 · Aufwand M–L · Kleine zusammenhängende Gruppen nach R17/R20**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die geprüften Aufräumgruppen aus R17 umsetzen. Überflüssige Dateien entfernen, unnötige Doppelbestände zusammenführen und nachweislich unpassend abgelegte Dateien nur dann verschieben, wenn dies die Orientierung verbessert. Root-Dateien, Beispiele, Experimente, Skripte, Konfiguration, Testdaten und Fremdquellen gehören zur Prüfung. Benötigte Lizenzen, reproduzierbare PoCs, Regressionstest-Fixtures und bewusst archivierte Historie besitzen eine Daseinsberechtigung, auch wenn Anwender sie nicht täglich öffnen.

Mit jeder Gruppe ihre aktiven Pfadabhängigkeiten, Build-/Test-/Release-Verwendung, Ignore-Regeln und die Repo-Karte nachziehen. Keine funktionalen Umbauten unter dem Etikett Aufräumen. Benutzerbearbeitete Dateien unter `generated` nicht pauschal löschen; lokale Artefakte und versionierte Produktdateien unterscheiden. Vorhandene `obsolete`- und andere ausdrücklich archivierte Bestände bleiben ohne gesonderten Auftrag unverändert. Bei ungeklärtem Zweck zunächst die konkrete Frage klären, statt versuchsweise zu löschen.

**Abnahme:** Jede verbleibende versionierte Datei und jedes verbleibende Repo-Verzeichnis hat einen dokumentierten Zweck in der Bestandszuordnung; entfernte oder verschobene Gruppen sind nachvollziehbar begründet. Die öffentliche Übersicht entspricht dem Ergebnis. Betroffene Beispiele, Builds, Tests und Paketierung bestehen; Testumfang, Lizenznachweise und reproduzierbare Entwicklungsnachweise sind erhalten. Die vollständige Abnahme des ausgewählten Aufräumumfangs folgt R16.

#### Kleine Zustandsreste und Workflow-Begleitdateien

**R21a · Gewicht 3 · Aufwand S · Erledigt**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die zwölf leeren Dateien `.vscode/.cortex-debug.peripherals.state.json` und `.vscode/.cortex-debug.registers.state.json` in den sechs G0B1-/L432-Beispielen sind entfernt. Zwölf genaue Root-bezogene Ignore-Regeln verhindern ihre erneute Aufnahme. Gemeinsame Start-, Task-, Compiler- und Boardkonfigurationen bleiben erhalten.

Der Benutzer hat GitHub als einzigen möglichen externen Verbraucher bestätigt. GitHub beschreibt Organisationsvorlagen unter `workflow-templates` in einem Organisations-Repo namens `.github`, nicht als `workflows/properties` in diesem Projekt. Die vier ungenutzten Vorlagen-Metadaten und `icons/go.svg` sind entfernt; die 17 Workflow-YAML bleiben bytegleich. Siehe [GitHub: Workflow templates](https://docs.github.com/en/actions/how-tos/reuse-automations/create-workflow-templates).

Die Workflow-README erklärt jetzt die tatsächlichen Prüfgruppen und lokalen Einstiege mit relativen Links. Die aktiven UM-Verweise auf die entfernten Ordner sind bereinigt; Trigger und Berechtigungen werden zutreffend den YAML-Dateien zugeordnet. Der auskommentierte historische Übersichtsblock bleibt für die spätere Dokumentationsbereinigung erhalten.

**Abnahme:** Genau 17 benannte Dateien entfernt. Ignore-Prüfung für alle zwölf Zustandsdateien und Gegenprobe für die erhaltenen `launch.json`/`tasks.json`; bytegleiche Workflow-YAML und aktive Debugkonfigurationen; Markdownlint, lokale Linkziele und UM-Format geprüft. Keine Änderung der Actions-Versionen, Trigger oder Testauswahl. Die gezählten R17-Tabellen bleiben der historische Bestand ihres ausdrücklich genannten Commits; nach R21a verbleiben 2.222 Dateien, davon 25 unter `.github` und 1.104 unter `examples`.

#### IDE-Einstiege portabel und tatsächlich benutzbar machen

**R21b · Gewicht 3 · Aufwand M · Erledigt**

[Zur Aufgabenübersicht](#aufgabenübersicht)

In den vier G0B1-Varianten steht ein festes Windows-Compilerverzeichnis in `.vscode/c_cpp_properties.json`. Die Root-`launch.json` verweist unter anderem auf fehlende Dateien unter `internal/decoder/testdata`, `internal/receiver/rttfile/testdata` und `_test/testdata/til.json`; mindestens ein Pfad ist zusätzlich rechnergebunden. Konfigurationen nach Zweck prüfen: gültige Debug-Einstiege auf existierende Daten und portable Toolwahl umstellen, nachweislich entfallene Aufrufvarianten entfernen. Keine beliebigen Ersatzdaten einsetzen, nur damit ein Pfad existiert.

Bei `.idea` die beabsichtigte CLion-Unterstützung für `_test/clion-review` erhalten. `trice.iml` und `trice.rokath.iml` sind inhaltsgleich, `modules.xml` verwendet nur `trice.iml`. README, Ignore-Kommentare und tatsächliche Wörterbuchablage widersprechen einander (`trice.dict`, `trice.dic`, `dictionaries/project.xml`). Gemeinsame Projekteinstellungen von lokalem Zustand unterscheiden und diese kleinen Inkonsistenzen bereinigen; nicht pauschal `.idea` löschen. Eine weitergehende Änderung persönlicher Inspektionspräferenzen braucht eine eigene Entscheidung.

**Abnahme:** Keine fest eingebauten Benutzer-/Installationspfade in den bearbeiteten Vorlagen; referenzierte Repo-Dateien existieren; jeweiliger Debug-Aufruf passt zur aktuellen CLI. CMake-Einstieg für CLion bleibt konfigurierbar. Nicht verfügbare IDE-/Hardware-Abnahmen explizit benennen, statt Portabilität allein aus JSON-Syntax abzuleiten.

**Ergebnis:** Die vier G0B1-C/C++-Vorlagen suchen `arm-none-eabi-gcc` über `PATH`. Ihre fest kopierten GCC-12-Builtindefines sind entfernt; die Abfrage erhält `-mcpu=cortex-m0plus`, damit der Compiler die Defines des tatsächlichen Zielprozessors liefert. Die Projektdefines bleiben erhalten. Die Root-Startvorlagen verwenden aktuelle CLI-Optionen, Repo-relative Eingaben und ein ausdrückliches Arbeitsverzeichnis. UART-Port und TCP-Endpunkt werden beim Start abgefragt. Die G0B1-/L432-Einstiege verwenden die dazugehörigen Root-Demotabellen und die Einstellungen ihrer Beispielprojekte; nicht beliebige Ersatzdaten.

Entfernt sind Varianten mit nicht mehr vorhandenen Captures, `try`-Quellen oder Zufallsdaten-Optionen sowie unzuordenbare alte Serial-/RTT-Varianten und der veraltete `check`-Aufruf. Der Testeinstieg verwendet einen vorhandenen Bind-Test in `internal/id`. Der C-Generator verwendet `-logC` und die aktuelle G0B1-Main-Datei nach deren Bind-Vorbereitung; insert/clean bleiben ausdrücklich Befehle zur Quelltextpflege ohne Abhängigkeit von einem persönlichen Cache. Die vorhandene DUMP-Aufzeichnung und ihre passenden Testtabellen bleiben erhalten. Ihre Lücke bei Cycle 194 wird weiterhin diagnostiziert und ist jetzt erklärt, statt von einem sachfremden alten TCOBS-Kommentar überlagert zu werden.

Die doppelte, unreferenzierte Moduldatei `trice.rokath.iml` ist entfernt. JetBrains-Ignore-Regeln trennen gültige Muster von Kommentaren: persönlicher Zustand wird tatsächlich ignoriert, gemeinsame Einstellungen bleiben versionierbar. Die README beschreibt das vorhandene XML-Wörterbuch und den erhaltenen CLion-Review-Host; der veraltete Archiv-Ausschlusspfad ist korrigiert. Inspektionsprofile, Code-Stile, lokale Workspace-Dateien, Beispiel-Builds und Archive bleiben unverändert. Die Nutzung und Voraussetzungen stehen im [IDE-Abschnitt des Handbuchs](../TriceUserManual.md#vs-code).

**Gezielte Prüfung:** Selbstbeschreibende Tests in [ide_templates_test.go](../../internal/args/ide_templates_test.go) kontrollieren die aktuelle CLI-Registrierung, eindeutige Startnamen, vorhandene Eingabepfade, Prompt-Auflösung und das existierende Debug-Testziel. Sie dekodieren den echten DUMP einschließlich seiner einen Cycle-Diagnose und erzeugen aus dem tatsächlichen G0B1-Quelltext nach Bind eine C-Tabelle, ausschließlich im Speicherdateisystem. Git-Ignore-Verhalten wird in einem temporären Repository geprüft; Modulverweise und C/C++-Includes werden aufgelöst. Eine verfügbare ARM-GCC-Installation wird zusätzlich nach den Cortex-M0+-Defines abgefragt.

CMake konfiguriert den unveränderten CLion-Host mit dem lokalen Hostcompiler; alle 24 C-Eingaben seiner Compile-Datenbank existieren und die Review-Konfiguration steht zuerst im Include-Pfad. Markdownlint und UM-Format bestehen. Die lokale Linkprüfung bestätigt die neuen Ziele, meldet aber weiterhin den bereits unter R10 festgehaltenen fehlenden UM-Verweis auf `docs/_Legacy/TriceObsoleteEncodings.md`; dessen allgemeine Bereinigung bleibt R20. VS-Code-/CLion-Oberflächen, Indexierung und reale UART-/RTT-Verbindungen wurden nicht ausgeführt; Windows-/Linux-IDE-Abnahmen sind damit nicht behauptet.

#### Root-Ausgaben und generierte Beispieldaten unterscheiden

**R21c · Gewicht 3 · Aufwand S–M · Erledigt**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die leere Root-Datei `trice.bin` und die 226.455 Byte große Root-`til.c` hatten keinen konkreten Build- oder Laufzeitverbraucher. Sie sind entfernt. Genau diese beiden möglichen Root-Ausgaben werden ignoriert; es gibt ausdrücklich keine pauschale Ignore-Regel für `*.c` oder `*.bin`.

`examples/TriceAbc/NodeLib/til.c` ist ebenfalls entfernt und gezielt ignoriert. `examples/TriceAbc/build.sh` löscht sie vor jedem Lauf, erzeugt sie mit `generate -logC` aus den vorbereiteten Quellen und kompiliert sie anschließend für `N6_rx` und `N7_bi`. Die Anleitung erklärt diesen Ablauf nun für einen sauberen Checkout. Der benutzerbearbeitbare ABC-Auswahl-Header `NodeLib/nodeAbc.h` bleibt versioniert. Die fünf Descriptor-Header unter `docs/scratchPad/obsolete/experiments/TriceBind/20_Target_Library_Integration/triceIDs` bleiben historische PoC-Eingaben.

`demoTIL.json`, `demoLI.json` und die projektbezogenen TIL/LI bleiben versioniert: Skripte, Testmatrizen, Beispiele, die TriceABC-Generierung und die IDE-Startprofile verwenden sie als gemeinsame persistente ID-/Standortdaten. Sie sind keine generierten Ausgaben und keine Löschkandidaten.

**Abnahme:** Die drei entfernten Dateien sind entweder ohne Verbraucher oder eindeutig durch den betreffenden Build erzeugt. `bash -n` bestätigt den unveränderten TriceABC-Buildablauf; die C-Generator-Verträge bleiben durch die bestehenden `internal/id`-Tests abgedeckt. Nach `build.sh` ist `NodeLib/til.c` vorhanden, aber wegen der spezifischen Ignore-Regel keine versionierte Arbeitsbaumänderung.

#### Unfertige Tools, ruhende Tests und Entwicklernotizen einordnen

**R21d · Gewicht 3 · Aufwand S–M · Erledigt**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die unfertigen Werkzeuge `cmd/_cui` und `cmd/_stim` sind auf ausdrücklichen Benutzerauftrag mit allen fünf Dateien nach `docs/scratchPad/obsolete/cmd/` verschoben. Inhalte einschließlich Lizenzhinweisen sind unverändert; aktive Builds, Releases und Tests verwendeten diese Verzeichnisse nicht. Die Einträge für diese unfertigen Werkzeuge sind aus der aktiven UM-Dateiübersicht entfernt; Anwender brauchen keinen Verweis auf das Archiv. Dieser Teil ist erledigt.

Die ruhenden Tests prüften ausschließlich den 2025 entfernten Befehl `trice update`/`trice u` und dessen alte Stamp-/Makroformen. Parser-, ID-, Insert-, Makroalias- und Stamp-Verhalten liegen heute in aktiven Tests unter `internal/id` und `internal/args`. Die beiden nicht ausführbaren Dateien sind deshalb als Entwicklungsnachweis nach `docs/scratchPad/obsolete/cmd/trice/` verschoben, nicht als ungeprüfte Tests reaktiviert.

`GoInfos.txt` wurde bereits zusammen mit anderen alten Entwicklernotizen nach `docs/scratchPad/` verschoben; der aktive `docs/`-Bestand enthält diese Datei nicht mehr. `CONTRIBUTING.md` nennt jetzt die tatsächlichen TestAll-/Coverage-Wege ohne die veralteten Zusatzframeworks. `cmd/clang-filter/ReadMe.md` beschreibt den aktuellen Aufruf über `scripts/_280_format_c_code.sh` und dessen gemeinsame CI-Anbindung. Der Filter selbst bleibt aktiv.

Die von Hand nutzbaren Git-Helfer unter `scripts` und die bewusst beauftragten Codex-Umzugsskripte bleiben Entwicklerwerkzeuge. Im Entwickler-Einstieg knapp auffindbar machen, nicht in den ersten Anwender-Logweg aufnehmen. `docs/scratchPad/scratchPad.md` enthält derzeit nur auskommentierte Notizen und einen Trenner: keine offene Spezifikation daraus ableiten. Eine spätere Archivierung dieses aktiven Notizzettels ist eine eigene Ablageänderung, kein Anlass, vorhandene Archive anzufassen.

**Abnahme:** Kein Verlust einzigartiger Testfälle und keine unbemerkte Erweiterung der ausgelieferten CLI. Die aktiven Insert-/Parser-Tests decken die relevanten alten Fälle ab. Entwicklerhinweise nennen die tatsächlich vorhandenen Werkzeuge und Tests. Ruhende Entwicklungsnachweise sind als solche erkennbar. Die gemeinsame Neuablage der Handoverskripte, ihrer Tests und Anleitung bleibt als R24 erfasst; keine isolierte Verschiebung mit defekten Imports.

#### Dokumentationsbilder und Vergleichsberichte konsolidieren

**R21e · Gewicht 3 · Aufwand M · Erledigt**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die 94 vorhandenen Referenzdateien sind in [`docs/ref/README.md`](../ref/README.md) nach ihrer Rolle eingeordnet: direkte Handbuchbilder, Drawio-Quellen mit benötigten PNG-/SVG-Exporten, Logos und Größenvarianten, generierte CLI-Hilfe, datierte Messbelege sowie der historische Backup-Container. Die PDF-Erzeugung verwendet `docs/ref` als Basisverzeichnis. Deshalb bleiben alle Medien erhalten; weder ähnliche Namen noch fehlende direkte Markdown-Treffer belegen Redundanz. Das ausdrücklich historische `Backup.7z` ist unverändert.

Die beiden zuvor aus dem Root-README erreichbaren KI-Vergleichsberichte sind nach [`obsolete/reports`](obsolete/reports/README.md) verschoben. Ihr Archivindex nennt Datum und Herkunft und grenzt sie von Produktdokumentation, Release-Zusagen und Leistungsversprechen ab. Das README führt neue Anwender nicht mehr zu diesen historischen Einschätzungen. Datierten Messbildern wird ebenfalls keine neue allgemeine Leistungszusage zugeschrieben.

**Abnahme:** Die aktive Dokumentation referenziert weiter ihre benötigten Medien, und der PDF-Renderer behält `docs/ref` als Asset-Basis. Die CLI-Hilfe bleibt über die Dokumentationsaktualisierung regenerierbar. Kein Asset wurde allein aufgrund einer Textsuche entfernt; die Vergleichsberichte sind ausdrücklich als historische Unterlagen eingeordnet.

#### Fremdsoftware-Ablage erklären und Alt-Konfiguration abgleichen

**R21f · Gewicht 3 · Aufwand S–M · Nach R10/R18/R20; kein Vendor-Update**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die 23 Dateien unter `third_party` liefern optionale Werkzeuge, Quellarchive, Anleitungen und RTT-/Debugger-Unterlagen. Zu den elf ZIP-Dateien die beabsichtigte Rolle festhalten: Offline-Hilfe, ursprüngliche Quelle oder bewusst erhaltene Version. Besonders die mehrfachen COBS-/ST-Link-Versionen benötigen diese Einordnung. Herkunft und Lizenzhinweise erhalten; kein Zip-Inhalt wird allein wegen seines Alters gelöscht oder ausgeführt. Wenn heutiger Bezug und Aufbewahrungszweck unklar bleiben, diese Gruppe ausdrücklich zurückstellen.

`.markdownlinkcheck.json` wird noch im UM für einen als veraltet bezeichneten lokalen Aufruf erwähnt; die aktive Linkprüfung verwendet Lychee mit `lychee.toml`. Nach Abgleich des tatsächlichen lokalen Unterstützungsumfangs entweder den alten Weg gezielt dokumentieren oder Konfiguration und veraltete Anleitung gemeinsam entfernen. Die zentrale Vendor-Ausnahme `.clang-format-ignore` ist dagegen aktiv und bleibt.

**Abnahme:** Kein neuer Pflichtdownload für zuvor direkt baubare Beispiele; Lizenz-/Herkunftsnachweise bleiben erhalten. Die ausgewählte Linkprüfung und ihre dokumentierte Konfiguration stimmen überein. `Drivers`/`Middlewares` werden weder umformatiert noch zwischen Beispielen zusammengelegt. Ungeklärte manuelle Nutzung ist vor einer Entfernung zu entscheiden.

### Entwicklerwerkzeuge vom Scratchpad entkoppeln

**R24 · Gewicht 3 · Aufwand M · Offen; unabhängig von der Handbuch-Umbenennung**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Befund: `scripts/test_codex_handover.py` importiert die ausdrücklich beauftragten, weiterhin benutzbaren Codex-Umzugswerkzeuge aus `docs/scratchPad`. Dieser Test wird nicht von den nummerierten TestAll-Schritten aufgerufen. Die Werkzeuge sind keine obsolete Trice-Experimente, benötigen aber eine dauerhafte Entwicklerablage.

Die zusammengehörigen Export-/Startskripte mit ihren Imports nach `scripts` überführen und den Python-Test gemeinsam anpassen. Die tatsächlich vorhandene Anleitung `docs/scratchPad/Codex_Rechnerwechsel_DE.md` samt Aufrufbeispielen und Ausgabe-/Importpfaden prüfen; den Entwicklerablauf über `CONTRIBUTING.md` auffindbar machen. Persönliche ZIP-Dateien dürfen nicht versehentlich eingecheckt werden. Keine neue CLI, keinen automatischen Import und keine ungefragte Bearbeitung persönlicher Codex-Daten ergänzen.

**Abnahme:** Kein aktiver Test importiert Dateien aus Scratchpad oder Archiv. Export, Import/Start, Clean-Git-Prüfung, aktive-Session-Prüfung und Schutz gegen veraltete Übergaben behalten ihr Verhalten. Die vorhandenen Python-Verhaltenstests bestehen mit der neuen Ablage; die Anleitung stimmt auf Linux, macOS und Git Bash unter Windows mit den tatsächlichen Aufrufen überein.

### GitHub Pages mit eindeutigem Einstieg und Veröffentlichungsumfang

**R22 · Gewicht 4 · Aufwand M · Neuer Folgeauftrag aus R17; nach R18/R19/R12/R20**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Es gibt zwei Implementierungen des Website-Einstiegs: Die versionierte `index.md` bindet die README per Liquid ein; `pages.yml` überschreibt sie beim Build mit einer zweiten, per `sed` erzeugten Variante und behauptet im Kommentar, die Datei sei nicht versioniert. Einen einzigen nachvollziehbaren Erzeugungsweg wählen und lokale Vorschau sowie CI darauf ausrichten. Beide Handbücher und ihre gegenseitigen Links müssen auf Pages richtig dargestellt werden; Quellcode-Links dürfen weiterhin zur passenden Repo-Ansicht führen.

Der Workflow baut vom Repo-Root; `_config.yml` enthält bislang nur Theme und Markdown-Engine. Ein ausdrücklicher fachlicher Veröffentlichungsumfang ist nicht erkennbar. Das ist noch kein Nachweis, welche Dateien Jekyll tatsächlich ausliefert. Den erzeugten Site-Baum prüfen und dann gezielt auf Einstieg, Handbücher, benötigte Bilder und bewusst verlinkte Inhalte begrenzen. Scratchpad, Entwicklerzustand und Fremdsoftware-Archive sollen nicht versehentlich Teil des Anwender-Einstiegs werden. Bestehende Archive dafür nicht verändern; die Auswahl gehört in den Veröffentlichungsweg.

**Abnahme:** Site-Build ohne Deployment prüfen; erzeugter Einstieg, beide Handbücher, Bilder und Querverweise funktionieren. Liste der ausgelieferten Dateigruppen passt zur beabsichtigten Website. Keine widersprüchliche zweite `index.md`-Erzeugung, kein unbeabsichtigter Verlust bewusst angebotener Downloads. Deployment und Publish bleiben gesonderte Aufträge.

### Checkout-/Binary-Installationswege für v2 absichern

**R14 · Gewicht 5 · Aufwand S–M · Distributionsentscheidung getroffen; verbleibender Abgleich vor Release**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Entscheidung des Anwenders: **`/v2` vorerst weglassen.** `go.mod` bleibt bei `module github.com/rokath/trice`, Imports bleiben unverändert. Die Produktversion v2.0.0 wird über fertige Binaries und lokale Builds aus einem Repo-Checkout angeboten. Das [README](../../README.md#project-information) nennt dafür jetzt `./scripts/buildTriceTool.sh` statt `go install github.com/rokath/trice/cmd/trice@latest`, mit Aufrufort und Hinweis auf die ausgegebenen Installationspfade.

Lokales Bauen und Installieren aus dem Checkout benötigt allein wegen der Produktversion keinen Major-Modulpfad. Das vom Skript intern verwendete lokale `go install ./cmd/trice ./cmd/tlog` bleibt zulässig. Versionierte Go-Auflösung über `@v2.0.0` wäre dagegen an einen passenden Major-Modulpfad gebunden und gehört vorerst nicht zum zugesagten Installationsumfang. Siehe [Go: Major version suffixes](https://go.dev/ref/mod#major-version-suffixes).

Verbleibende Aufgabe: Aktive Installationsanleitungen und Release-Prüfungen auf den beschlossenen Umfang abstimmen. Weitere entfernte Go-Installationsanweisungen dürfen v2 nicht versprechen. Lokalen Build beider Tools und ausgelieferte Binaries prüfen; eine spätere Modulpfadumstellung benötigt eine neue Entscheidung.

**Abnahme:** Der lokale Build mit `./scripts/buildTriceTool.sh` und die angebotenen Binary-Installationswege funktionieren in einer sauberen Umgebung. Dokumentation und Prüfungen passen dazu; keine `/v2`-Umstellung und keine v2-Modulinstallationszusage. Kein Tag oder Publish im Rahmen dieses Auftrags.

### Release Notes und ausgelieferte Dateien prüfen

**R15 · Gewicht 5 · Aufwand M**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Der aktuelle [Changelog](../../CHANGELOG.md) endet bei v1.3.0. Einen verständlichen neuen Release-Abschnitt erstellen: Bind, SL, CE, Tags/Gewichte, Visualisierung, Generatoren, Ablage, Plattform-/Compilergrenzen und konkret nötige Anpassungen bestehender Projekte. Ein vollständiger Git-Log ersetzt diese Anwendersicht nicht.

Die [Installationsprüfungen](../../.github/workflows/install-checks.yml) und [Release-Prüfungen](../../.github/workflows/release-audit.yml) testen bereits Archive, Pakete, PDF und klassisches Decoding. Eine kleine neue Feature-Abnahme soll mit den **ausgepackten** Tools und Target-Quellen erfolgen: Bind bzw. Insert mit SL/CE, tatsächlich erzeugter Record, Text/JSON/KV und ausschließlich Anwendungsrecords auf dem maschinenlesbaren Kanal. Passende plattformunabhängige Fixture verwenden.

Die Toolchain-Angaben angleichen: `go.mod` verlangt Go 1.25.0; der separat manuell gestartete Workflow `go.yml` nennt noch 1.24.0. Das muss keinen unmittelbaren Buildfehler verursachen, weil Go einen Toolchain-Wechsel auslösen kann, macht die geprüfte Umgebung aber unnötig unklar.

Die unter R18/R19 getrennten Handbücher unter eindeutigen Namen ausliefern: `TriceUserManual` für den Einstieg und `TriceReferenceManual` für Details. Downloadlinks und Artefaktprüfungen müssen beide Dokumente dem richtigen Zweck zuordnen; die bisherige PDF-Prüfung nur umzubenennen reicht nicht.

Zusätzlicher Befund aus R17: Das Target-Archiv wird über `src/*.c`, `src/*.h` und `src/*.md` zusammengestellt. Beim ausgepackten Archiv ausdrücklich prüfen, ob die benötigten Lizenzinformationen vorhanden sind und die mitgelieferte `src/ReadMe.md` auch ohne Repo-Elternverzeichnis verständlich ist. Ihre relativen Verweise auf Root-Dokumente dürfen nicht ungeprüft als funktionierende Archivnavigation gelten. Fremdquellenhinweise erhalten; keine Vendor-Aktualisierung in diesen Auftrag aufnehmen.

**Abnahme:** Verständliche englische Release Notes und Anpassungshinweise; finales englisches User Manual und Reference Manual als Markdown/PDF; passende Target-Quellen; nachvollziehbare Artefaktprüfungen auf den zugesagten Betriebssystemen. Kein Publish oder Tag ohne ausdrücklichen Auftrag.

### Release-Abnahme auf einem feststehenden Stand

**R16 · Gewicht 5 · Aufwand M plus Full-Testlauf**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Nach den ausgewählten Änderungen zuerst ihre gezielten Prüfungen, anschließend einmal den vollständigen Lauf auf einem feststehenden Stand ausführen. Die Auswertung des abgeschlossenen Laufs vom 29./30. September steht bei R01, R06 und R13; seine flachen Logdateien wurden beim späteren Quick-Lauf ersetzt. Künftige Fehlerprotokolle und Zeitbasen vor einem weiteren `testAll`-Lauf gesondert sichern, weil der Runner alte Logs entfernt. Keine konkurrierenden zustandsverändernden Läufe starten.

**Abnahme:**

- Full-Matrix mit Bind und Insert/Clean ohne ungeklärte Fehler; alle Pflichtwerkzeuge vorhanden. PASS einer Suite darf relevante intern übersprungene Tests nicht verdecken.
- CE-/SL-End-to-End-Abnahme gemäß R07 und installierte Release-Artefakte gemäß R15.
- Keine zurückgebliebenen unbeabsichtigten Source-/Metadatenänderungen.
- Go-Coverage mit der Paket- und `-coverpkg`-Auswahl des [Coverage-Workflows](../../.github/workflows/coverage.yml) gegen die Zielbranch-Baseline vergleichen; Coveralls-Zeilenabdeckung und Go-Statement-Coverage unterscheiden. C-Abnahme separat.
- Finale Dokumentation: kurzes UM und Reference Manual mit mdtoc, Markdown, relativen Links mit korrekter Groß-/Kleinschreibung und PDF prüfen; README, Dokumentationswegweiser und GitHub Pages einbeziehen. Keine aktiven Links auf entfernte Weiterleitungsdateien; Repo-Karte und Bestandszuordnung stimmen mit den tatsächlich vorgenommenen Aufräumarbeiten überein. Danach keine ungeprüften inhaltlichen Änderungen mehr.
- Ein kurzer echter G0B1-Boardlauf für zwei Task-Kontexte, Stempel und lesbare/strukturierte Ausgabe, sofern diese Hardware-Zusage im Release gemacht wird. Fehlender Hardwarezugang wird als fehlender Nachweis benannt.
- Versionsentscheidung, bekannte Grenzen und eventuelle bewusst akzeptierte Abweichungen sind dokumentiert.

## Weitere Beschleunigung ohne geringere Abdeckung

R08/R09, P04, P01 und P03 sind umgesetzt. Alle Erwartungen, Konfigurationen und beide ID-Workflows bleiben erhalten. Die Einzelzeilenausführung bleibt für Diagnose und ungeframte beziehungsweise besondere Konfigurationen verfügbar. `quick` anstelle von `full`, weniger Konfigurationen oder verkürzte Testdaten wären keine gleichwertige Beschleunigung. P02 bleibt die getrennte Folgearbeit.

| ID | Aufgabe und Status | Gewicht | Aufwand | Abhängigkeiten |
| --- | --- | ---: | --- | --- |
| [P02](#cache-nutzen-ohne-veralteten-c-code-zu-testen) | Go-/CGO-Buildcache gezielt und nachweisbar invalidieren | 3 | M–L | [R06](#testbeschleunigung-mit-vollständigen-pc-matrizen-geprüft), [R13](#test-ausgangszustand-einschließlich-standalone-beispielen-erhalten) |
| [P03](#l432-builds-isolieren-und-begrenzen) | Erledigt: L432-Matrix von 27:06 auf 4:55 verkürzt, alle 101 Konfigurationen bestanden | 4 | L | [R06](#testbeschleunigung-mit-vollständigen-pc-matrizen-geprüft), [R13](#test-ausgangszustand-einschließlich-standalone-beispielen-erhalten) |

### Cache nutzen, ohne veralteten C-Code zu testen

**P02 · Offen**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Der PC-Worker löscht vor jedem Workflow `go clean -cache -testcache`. Das ist wegen außerhalb der Go-Paketverzeichnisse eingebundener C-Quellen begründet. Die [Go-Dokumentation zum Buildcache](https://pkg.go.dev/cmd/go#hdr-Build_and_test_caching) weist auf Grenzen bei C-Abhängigkeiten hin; schlichtes Entfernen der Bereinigung wäre nicht ausreichend.

Prüfen, ob vollständig erfasste Inhalts-/Konfigurationssignaturen oder vorbereitete lokale Build-Eingaben die korrekte Invalidierung gezielter machen. Die bloße Trennung in zwei Cacheordner garantiert keine Aktualität. `-count=1` kann weiterhin die tatsächliche Testausführung erzwingen und unabhängig davon den Buildcache nutzen.

**Nachweis:** Änderungen an `triceCheck.c`, gemeinsam eingebundenen Headern, Sidecars, Workflow, Compileroptionen und Konfiguration erzwingen den passenden Neubau; kalte und warme Läufe liefern dieselben Ergebnisse. Keine Wiederverwendung bereits erfolgreich gemeldeter Testergebnisse als Ersatz für geforderte Ausführung.

### L432-Builds isolieren und begrenzen

**P03 · Umsetzung und gezielte Abnahme abgeschlossen**

[Zur Aufgabenübersicht](#aufgabenübersicht)

[all_configs_build.sh](../../examples/L432_inst/all_configs_build.sh) baute bisher 0 bis 100 nacheinander mit `make clean` und vollständigem `build.sh` je Konfiguration. Der abgeschlossene Full-Lauf am 3. Oktober dauerte **48 Minuten 45 Sekunden**, davon die L432-Matrix **27 Minuten 6,416 Sekunden**. Alle Compiler-Matrizen bestanden. Die zwei Fehler in Go/Go-Coverage hatten dieselbe Umgebungsursache: Ein unpräfixiertes ARM-`nm` stand vor dem macOS-`nm` im PATH. Nach der lokalen PATH-Korrektur bestanden beide betroffenen Tests gezielt; der Full-Lauf vom 4. Oktober zeigte jedoch erneut die falsche Werkzeugauswahl. Die dauerhafte Korrektur der Tests steht unten. Die alten Testprotokolle bleiben unter `temp/before-l432-parallel-*` erhalten.

**Umsetzung:** Einmalige Bind-Vorbereitung und Umgebungsprüfung, danach parallele Konfigurationen mit jeweils `make -j1`. Das Standardbudget folgt der verfügbaren CPU-Anzahl beziehungsweise unter Windows dem begrenzten Budget der gemeinsamen Buildumgebung mit bevorzugter Erkennung physischer Kerne. Bei gescheiterter Erkennung gelten vier Jobs. `TRICE_L432_TEST_JOBS` erlaubt eine ausdrückliche Begrenzung. Jede Konfiguration erhält bei jedem Aufruf ein frisches, eigenes Buildverzeichnis unter `temp/log/l432.*`; alle bisherigen Quellen, Defines und Linkziele bleiben erhalten. Bestehende `out.gcc`-Artefakte bleiben unberührt. Erfolgreiche temporäre Objekte werden entfernt, vollständige Compilerprotokolle und fehlgeschlagene Teil-Builds bleiben zur Diagnose erhalten. Es gibt bewusst keinen neuen Objektcache und keine zusätzliche Abhängigkeit; auch Folgeaufrufe übersetzen vollständig neu.

**Fehlerverhalten:** Die Matrix respektiert die Stop-Policy des Test-Runners, zeigt Konfiguration, Fehlerauszug, Protokoll und Wiederholungsbefehl. Bereits gestartete Jobs werden abgeschlossen; bei Abbruch werden auch Compiler-Kindprozesse beendet, bevor die verwaltete Wiederherstellung beginnt. Abgeschlossene Konfigurationen erscheinen sofort im Schrittprotokoll.

**Zusätzlicher Engpass:** Parallelität allein reicht hier nicht: Die Assembler-Listings (`.lst`) verursachten unter paralleler Last erhebliche zusätzliche Laufzeit. Die Matrix schaltet deshalb ausschließlich diese Textausgabe mit `GCC_LISTINGS=0` ab. Normale Einzelbuilds behalten Listings als Standard; alle Optionen für Zielcode, Konfigurationsdefines, Warnungen und Linkziele bleiben erhalten. Konfiguration 0 benötigte im direkten Vergleich **13 Sekunden mit Listings und 6 Sekunden ohne**; ELF, HEX und BIN waren jeweils **bytegleich**.

**Verhaltenstests:** Die beschreibenden Tests in [l432_matrix_test.go](../../scripts/l432_matrix_test.go) prüfen alle 101 Konfigurationen, unveränderte Konfigurationsdefines und Buildziele, serielle und parallele Ausführung, automatische Budgeterkennung und ausdrückliche Limits, einmalige Vorbereitung, Warnungen, Fehlerfortsetzung, Abbruch nach dem gestarteten Batch, fehlende Binärdateien, ungültige Einstellungen, neue Buildpfade nach Headeränderung und das Beenden von Kindprozessen. Der echte Makefile-Auszug wird zusätzlich mit und ohne Listings ausgewertet: Andere Compileroptionen bleiben erhalten, Einzelbuilds erzeugen standardmäßig weiterhin Listings. `go test ./scripts -count=1`, ShellCheck, Shellformat, UM-Format und Markdownlint bestehen.

**Vollständige ARM-Abnahme am 3. Oktober:** Alle **101/101 Konfigurationen bestanden** mit ARM GCC 15.3.1, automatisch erkanntem Budget von zwölf Jobs und frischen Buildverzeichnissen. Laufzeit der Matrix **295 Sekunden (4:55)** gegenüber **1626,416 Sekunden (27:06)** im vorherigen Full-Lauf auf demselben Mac: rund **82 % weniger Zeit**, Faktor **5,5**. Das ist ein Vergleich dieser lokalen Läufe, keine plattformübergreifende Laufzeitzusage. Code-, Daten- und BSS-Größen stimmen für sämtliche Konfigurationen mit dem vorherigen Lauf überein. Erfolgreiche temporäre Buildverzeichnisse wurden entfernt; vollständige Protokolle liegen unter `temp/log/l432.hqSVXw`, die Firmware-Gegenprobe unter `temp/log/l432-listing-probe.o2ty4t` und `temp/log/l432_listing_probe.log`. Die verwaltete Wiederherstellung bestätigte den exakten Ausgangszustand nach Erfolg und nach den zuvor kontrolliert abgebrochenen Probeläufen. Folgeaufrufe nutzen ebenfalls frische Objekte; es gibt keinen warmen Objektcache, dessen Ergebnis die Testausführung ersetzen könnte. Reale Windows-/Linux-Abnahmen wurden für diese Änderung nicht ausgeführt; die abschließende plattformübergreifende Release-Abnahme bleibt R16.

**Full-Nachprüfung am 4. Oktober:** Der erste Benutzerlauf `./scripts/testAll.sh full` benötigte **1580 Sekunden (26:20)**, rund **46 % weniger** als der zuvor dokumentierte Full-Lauf. **23 Schritte bestanden**, nur Go und Go-Coverage scheiterten erneut an den beiden `nm`-Prüfungen. Die L432-Matrix bestand mit **101/101 Konfigurationen in 270 Sekunden (4:30)**; auch die übrigen Compiler-Matrizen, der Release-Snapshot und beide PC-Workflows bestanden. Die Fehlerprotokolle wurden ausgewertet; der anschließende Benutzerlauf hat die flachen Schrittprotokolle ersetzt.

**Dauerhafte Korrektur der Symbolprüfung:** Die Tests in [local_log_integration_test.go](../../internal/id/local_log_integration_test.go) lesen ELF-, Mach-O- und COFF-Objekte jetzt mit der Go-Standardbibliothek. Compiler und Symbolleser werden nicht mehr unabhängig voneinander aus dem PATH ausgewählt. Gegenproben prüfen vorhandene Funktions-, globale, lokale und undefinierte Symbole, gültige leere Objekte, fehlende beziehungsweise beschädigte Dateien und ein absichtlich unbrauchbares `nm` an erster Stelle im PATH. Auch mit dem tatsächlich problematischen ARM-`nm` vorne im PATH bestehen beide ursprünglichen Compile-out-Tests. Die Objektformat-Gegenproben liefen auf macOS mit Clang-Cross-Compilation; sie ersetzen keine echte Windows-/Linux-Abnahme.

**Abnahme der Korrektur:** `go test ./... -count=1` und der vollständige Go-Coverage-Lauf mit `-count=1 -covermode=atomic -coverpkg=./...` bestehen. Das separate Profil `temp/log/coverage-nm-fix.out` vermied ein Überschreiben der damaligen Full-Protokolle während dieser gezielten Abnahme. Der danach vom Benutzer gestartete Full-Lauf vom 4. Oktober, 10:03 bis 10:29 Uhr, bestand mit **25/25 Schritten in 1583 Sekunden (26:23)** vollständig. Die L432-Matrix bestand erneut mit **101/101 Konfigurationen in 272 Sekunden (4:32)**; der Ausgangszustand der versionierten Dateien wurde bestätigt.

**Laufzeiten sichtbar machen:** Der Runner zeigt nun vor jedem Skriptnamen die einzeln gemessene Laufzeit als `(%3dm%3ds)`, etwa `(  4m 30s)`. Die Angabe gilt für alle Ergebniszustände und bleibt in `temp/log/testAll_summary.log` erhalten. Im interaktiven Terminal aktualisiert sie sich während des Schritts. Damit lassen sich insbesondere die PC-Blöcke unter Windows vergleichen, ohne jeden einzelnen Testfall zu instrumentieren. Die bisherigen Messwerte auf macOS ersetzen weiterhin keine neue Windows-Zeitmessung.

## Sinnvolle Erweiterungen zur späteren Diskussion

Diese Punkte sind Vorschläge, keine offenen Versprechen für das nächste Release. Bestehende Tests, PoCs und frühere Entscheidungen bleiben maßgeblich. Vor einer Issue-Erstellung Nutzen, Scope und vorhandene Issues abgleichen; externe Issue-Texte später englisch formulieren.

| ID | Idee | Gewicht | Aufwand | Empfehlung |
| --- | --- | ---: | --- | --- |
| [F01](#generierte-dateien-verständlich-zuordnen) | Verständliche Bestandsprüfung generierter Artefakte | 3 | M | Zuerst rein lesende Diagnose diskutieren |
| [F02](#parser-und-recordpfade-systematisch-auf-unerwartete-eingaben-prüfen) | Fuzz- und Race-Prüfungen für neue Parser und Recordpfade | 3 | M | Robustheit vor weiteren Ausgabeformaten |
| [F03](#frühe-hostfilterung-messen) | Frühe Hostfilterung / Template-Caching | 2 | M–L | Alte A11/M17-Aufgabe; zuerst messen |
| [F04](#benannte-felder-besser-weiterverwenden) | Benannte SL-Felder für Visualisierung oder Schemaübersicht | 2 | M | Mit einem konkreten Anwenderfall beginnen |
| [F05](#kleine-wrapper-erweiterung-getrennt-bewerten) | CE für eindeutig zuordenbare Ein-Logstellen-Wrapper | 2 | M | Kleiner separater Ausbau auf Basis des PoC |
| [F06](#allgemeines-ce-rebase-bleibt-eine-architekturentscheidung) | Allgemeines CE für Bind-Wrapper und Counter-Rebase | 1 | L | Weiter zurückstellen |

### Generierte Dateien verständlich zuordnen

**F01:** Ein lesender Bericht könnte erklären, welche Source welchen Sidecar besitzt, welche Dateien für den gewählten Build gebraucht werden und welche nicht mehr im gewählten Scan vorkommen. Das beantwortet die Frage nach unerwarteten `triceCheck`-Artefakten besser als pauschales Löschen.

[Bind](../../internal/id/bindIDs.go) entfernt bereits überzählige Rebase-Helfer eines aktuell bekannten Owners. Eine globale Bereinigung aller verwaisten Owner-Dateien ist ein anderer Vertrag. Nicht gescannte Sources können weiterhin legitime Besitzer sein; historische Sidecars werden zudem als ID-Evidenz genutzt. Benutzereigene ABC-Auswahlheader im selben Verzeichnis müssen geschützt sein.

Ein optionales späteres Löschen braucht daher eindeutig nachgewiesenen Besitz und vollständigen Projektumfang. **Kein automatisches Pruning als unbemerkte Nebenwirkung eines partiellen Bind-Laufs.** Abnahme zunächst: nützlicher Bericht ohne Dateiänderungen; Löschverhalten erst nach eigener Entscheidung.

### Parser und Recordpfade systematisch auf unerwartete Eingaben prüfen

**F02:** Im untersuchten Go-Code sind viele tabellarische Verhaltenstests, aber keine `Fuzz...`-Einstiegspunkte vorhanden. Ergänzende Fuzztests für Template-/CE-Parsing und Decoder-Eingaben könnten Escapes, Klammern, verkürzte Records und ungewöhnliche Kombinationen abdecken. Ein gezielter Race-Lauf ist besonders bei Abschluss-/Watcher-Arbeiten sinnvoll.

Nicht jede Testfunktion parallelisieren. Relevante Invarianten sind unter anderem: kein Panic bei abgewiesener Eingabe, keine Teilveröffentlichung, stabiler kanonischer Template-Roundtrip, kein Diagnosetext im JSON-Kanal und keine Datenrennen bei echten nebenläufigen Pfaden. Gefundene Fälle als lesbare Regressionstests erhalten. Ein begrenzter Fuzz-Lauf verspricht keine vollständige Sicherheit.

### Frühe Hostfilterung messen

**F03 übernimmt die offene A11, zuvor M17.** Replay mit 0, 50, 90 und 100 Prozent verworfenen Ereignissen vergleichen; CPU, Allokationen und Durchsatz für Text, JSON und KV erfassen. Im [TREX-Decoder](../../internal/trexDecoder/trexDecoder.go) werden Templates momentan pro Ereignis geparst und validiert; ein Cache ist ein weiterer Kandidat, aber noch keine beschlossene Umsetzung.

Framing/Integrität, Rohaufzeichnung, Statistik, Diagnosen, Stempelreferenzen und akzeptierte Records müssen gleich bleiben. Typisierte SL-/CE-Records und aktualisierte Wörterbücher gehören in den Vergleich. R09 betrifft wiederholte endliche Testläufe; diese Aufgabe betrifft Durchsatz im dauerhaften Betrieb und darf damit nicht verwechselt werden.

### Benannte Felder besser weiterverwenden

**F04:** Die heutige Visualisierung adressiert numerische Werte über `v0...v11`. Ein Zugriff über bereits vorhandene Feldnamen könnte Regeln lesbarer und gegen geänderte Parameterreihenfolgen robuster machen. Alternativ könnte eine reine Schemaübersicht Namen, Typen und zugehörige IDs über das heutige Häufigkeitsregister hinaus zeigen.

Zuerst einen dieser Anwenderfälle auswählen. Kein neuer Parallelvertrag für dasselbe Schema, keine automatische C-Typinferenz und keine zusätzliche Target-Nutzlast. Gleiche Feldnamen mit unterschiedlichen Typen, fehlende Werte, User- gegenüber Hostfeldern und historische TIL-Einträge benötigen klare Regeln. Keine neue CLI festlegen, bevor der konkrete Nutzen entschieden ist.

### Kleine Wrapper-Erweiterung getrennt bewerten

**F05:** Der [erweiterte CE-PoC](../../internal/id/context_enrichment_rebase_poc_test.go) zeigt einen eindeutig über eine Zeile zuordenbaren Wrapper mit einer Logstelle ohne Compiler-Vorlauf und ohne `__COUNTER__`. Das könnte die bestehende einfache CLI erhalten und ist unabhängig vom allgemeinen Rebase-Ausbau bewertbar.

Vor produktiver Freigabe sind Alias-/Selektorverhalten, lokale Sichtbarkeit, Bitbreiten/Stempel, einmalige Auswertung, wiederholte Expansionen, `generate -logC` und klare Ablehnung nicht unterstützter Formen nachzuweisen. Bis dahin bleibt die heutige dokumentierte Bind-Grenze bestehen; Insert/Clean ist die dauerhafte Alternative.

### Allgemeines CE-Rebase bleibt eine Architekturentscheidung

**F06 übernimmt die bisher zurückgestellte CE-Folgeaufgabe.** Der PoC wählt pro Expansion einen CE-Adapter im Präprozessor. Für komplexe Fälle liefert ein zusätzlicher Vorlauf mit dem konkreten Compiler absolute Counter-Werte. Das löst in den geprüften Fällen die falschen Scope-Anforderungen, kostet aber einen Buildschritt pro Übersetzungseinheit und Konfiguration.

Erhaltene Nachweise: getrennte lokale Scopes, wiederholte Wrapper-Aufrufe, einmalige Auswertung, echte Host-Records und Ablehnung veralteter Zuordnungen. Die dokumentierte Matrix umfasst Host-Clang sowie ARM-GCC für Cortex-M0/M4 in mehreren C-/C++-Modi; ARM-Nachweise sind Compile-Nachweise. Strenges C++20 ist wegen der vorhandenen Enum-Rebase-Warnung nicht bestanden. Weitere Compiler, PCH/Module, zusätzliche Counter-Verwendung, Build-Abhängigkeiten und getrennte Artefakte bleiben Integrationsfragen.

**Empfehlung:** Für das nächste Release nicht aufnehmen. Ein einfaches, merkbares Userinterface hat Vorrang. Der bereits verfügbare Insert/Clean-Weg und ausdrücklich geschriebene Werte decken die praktische Alternative ab. Kein Compiler-Vorlauf und keine neue Kontextverwaltung werden implizit eingeführt.

---

## Erledigte Korrekturen

| ID | Status | Ergebnis / Nachweis |
| --- | --- | --- |
| [R10](#anwenderdokumentation-von-entwicklungsständen-befreien) | Erledigt | Aktuelle Referenz von historischen Bind-Entwicklungsständen getrennt; gültige Einzelinformationen übernommen. |
| [R21a](#kleine-zustandsreste-und-workflow-begleitdateien) | Erledigt | Leere Debugger-Zustände und ungenutzte Vorlagenbegleiter entfernt; CI-Orientierung aktualisiert. |
| [R21b](#ide-einstiege-portabel-und-tatsächlich-benutzbar-machen) | Erledigt | IDE-Vorlagen auf aktuelle CLI und portable Compilerwahl umgestellt; JetBrains-Metadaten und Ignore-Verhalten geprüft. |
| [R01](#kein-automatisch-erzeugtes-untagged-präfix-ausgeben) | Erledigt | Kein automatisch erzeugtes untagged-Präfix ausgeben |
| [R02](#kopierbare-dokumentationsbeispiele-berichtigt) | Erledigt | Kopierbare Dokumentationsbeispiele berichtigt |
| [R03](#fehlerstatus-bei-fehlgeschlagenem-clean-erhalten) | Erledigt | Fehlerstatus bei fehlgeschlagenem Clean erhalten |
| [R04](#automatisches-nachladen-wiederhergestellt) | Erledigt | Automatisches Nachladen wiederhergestellt |
| [R05](#kompatibilitätsvertrag-und-release-ziel-festgelegt) | Erledigt | Kompatibilitätsvertrag und Release-Ziel festgelegt |
| [R06](#testbeschleunigung-mit-vollständigen-pc-matrizen-geprüft) | Erledigt | Testbeschleunigung mit vollständigen PC-Matrizen geprüft |
| [R07](#ce-sl-integration-und-feature-beispiele-verbindlich-ausgewählt) | Erledigt | CE-/SL-Integration und Feature-Beispiele verbindlich ausgewählt |
| [R08](#ressourcen-und-signalbehandlung-pro-loglauf-abgeschlossen) | Erledigt | Ressourcen und Signalbehandlung pro Loglauf abgeschlossen |
| [R09](#endliche-eingaben-ohne-pauschale-wartezeit-abgeschlossen) | Erledigt | Endliche Eingaben ohne pauschale Wartezeit abgeschlossen |
| [R11](#sl--und-ce-kapitel-vollständig-ins-englische-übertragen) | Erledigt | SL- und CE-Kapitel vollständig ins Englische übertragen |
| [R13](#test-ausgangszustand-einschließlich-standalone-beispielen-erhalten) | Erledigt | Test-Ausgangszustand einschließlich Standalone-Beispielen erhalten |
| [R17](#bestandszuordnung-und-befunde-der-repo-prüfung) | Erledigt | Bestandszuordnung und Befunde der Repo-Prüfung |
| [P01](#pc-konfigurationen-begrenzt-parallel-geprüft) | Erledigt | PC-Konfigurationen begrenzt parallel geprüft |
| [P03](#l432-builds-isolieren-und-begrenzen) | Erledigt | L432-Builds isolieren und begrenzen |
| [P04](#bulk-als-regulärer-pc-testweg-mit-verwertbarer-fehlerdiagnose) | Erledigt | Bulk als regulärer PC-Testweg mit verwertbarer Fehlerdiagnose |

### Test-Ausgangszustand einschließlich Standalone-Beispielen erhalten

**R13 · Gewicht 5 · Aufwand M · Umsetzung abgeschlossen; abschließende Full-Matrix bei R16**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die vier lokalen Tabellen in `examples/PC_log` und `examples/G0B1_log` waren nach zusätzlichen SL-/CE-Quellen einmalig aktualisiert worden. Vorhandene TIL-Einträge behielten ihre IDs; in den LI-Tabellen verschoben sich Zeilennummern. Diese inhaltliche Aktualisierung ist bereits im aktuellen Commit-Stand enthalten. Bei unveränderten Eingaben überspringt Bind identische Schreibvorgänge; der vorhandene `TestBindDoesNotReplaceUnchangedFiles` prüft das ausdrücklich.

Die Standalone-Builds aus Schritt 600 laufen weiterhin im Checkout. Der verwaltete Snapshot umfasst jetzt auch die lokalen Beispielquellen, TIL/LI-Dateien, generierten Verzeichnisse und Buildausgaben einschließlich ursprünglich fehlender Pfade und vorhandener Benutzerauswahl. Schritt 490 bereitet Bind innerhalb desselben Zustandsvertrags vor. Am Ende von `testAll` vergleicht eine zusätzliche Prüfung die wirklichen Bytes aller versionierten Dateien mit dem Ausgangszustand und nennt veränderte Pfade; ein bereits zuvor schmutziger Git-Status gilt nicht als Gleichheitsnachweis. Es gibt keine pauschale Git-Rücksetzung.

**Gezielte Abnahme am 30. September:** Die zwei Standalone-Builds pro `PC_log` und `G0B1_log` ließen alle vier Tabellen bytegleich und behielten beim zweiten Lauf deren Inodes; der äußere Snapshot stellte den Checkout anschließend wieder her. Isolierte Verhaltenstests prüfen schmutzige und ursprünglich fehlende lokale Dateien, generierte Benutzerauswahl und Buildausgaben nach Erfolg, Worker-Fehler und Signal, außerdem die transaktionale Vorbereitung in Schritt 490 bei Erfolg und Fehler. Ein Runner-Test beweist, dass eine erneute Änderung einer schon vorher schmutzigen Datei trotz unverändertem Git-Kurzstatus erkannt und mit Pfad gemeldet wird. `testAll quick` besteht mit 19/19 Schritten einschließlich der realen Schritte 490, 600 und der Byte-Prüfung. Die vollständige Release-Matrix bleibt bei R16.

### Kein automatisch erzeugtes untagged-Präfix ausgeben

**R01 · Gewicht 5 · Aufwand M · Umsetzung abgeschlossen; abschließende Full-Matrix bei R16**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die sichtbare Ausgabe enthält kein synthetisches `untagged:` mehr. Für ungetaggte oder unbekannt getaggte Ereignisse verhindert die Textausgabe zugleich, dass ein Doppelpunkt im Benutzertext nachträglich als Format-Tag interpretiert wird. JSON/KV verwenden weiter `tag=untagged` als Metadatum, ohne die Message zu verändern. Die `Fish`-Erwartung ist korrigiert. Die C-gestützten Vergleichsschleifen brechen bei der ersten Abweichung pro Konfiguration ab; `--no-stop` steuert weiterhin die Fortsetzung im äußeren Test-Worker. Gezielte Regressionstests decken die unabhängigen Fälle ab. Die vollständige Release-Matrix bleibt ausdrücklich Aufgabe R16.

**Gezielte Abnahme am 30. September:** Translator-Paket und die einschlägigen Emitter-Tests bestehen. Die Fehlerprobe erzwingt je einen falschen Bulk-, Einzelzeilen- und Direct-/Deferred-Vergleich und bestätigt jeweils nur eine Diagnose trotz `TRICE_TEST_NO_STOP=1`. Die kurzen PC-Workflows bestehen mit Insert und Bind. Eine komplette Ringpuffer-Einzelzeilenkonfiguration sowie die kombinierte Direct-/Deferred-Konfiguration und alle drei zuvor betroffenen Spezialfälle bestehen unter Insert; die kombinierte Konfiguration und Spezialfälle bestehen auch unter Bind. Der unabhängige `Fish`-Fall ist Teil der Ringpuffer-Einzelzeilenprüfung. Ein ad hoc Bind-Mehrpaketlauf ohne den vorgesehenen Go-Cache-Reset lieferte zunächst einen leeren Direct-Wert; der kontrollierte Bind-Einzellauf nach `go clean -cache -testcache` bestand. Für die abschließende Matrix den regulären PC-Test-Worker mit seinem Cache-Reset verwenden. Markdownlint für das geänderte UM besteht. Die vollständige Full-Matrix bleibt bei R16.

**Verbindliche Ausgaberegel: Trice darf die Zeichenfolge `untagged:` niemals selbst zu einer Logmeldung hinzufügen.** Sie darf in der sichtbaren Meldung nur vorkommen, wenn die Anwendung sie selbst als Text geliefert hat. Das gilt für Text, JSON und KV und ausdrücklich auch bei `-color off`. Bei `trice("hi")` lautet die sichtbare Message deshalb `hi`, ohne `untagged:` vor oder innerhalb des Textes. Die interne Klassifizierung heißt trotzdem `untagged`; JSON/KV dürfen dafür ein separates Tag-Metadatum mit dem Wert `untagged` ausgeben. Filterung, Gewicht, Farbe und Ereignisstatistik verwenden weiterhin diese Klassifizierung. Ein vom Anwender tatsächlich gelieferter Text `untagged:` darf nicht pauschal entfernt werden.

| Format | Ausgabe für `trice("hi")`, ohne weitere Metadaten |
| --- | --- |
| Text | `hi` |
| JSON | `{"tag":"untagged","message":"hi"}` |
| KV | `tag=untagged message="hi"` |

Bei einem unbekannten Präfix wie `trice("mgs:blah")` bleibt die Message `mgs:blah`, während das Tag-Metadatum `untagged` lautet. Dadurch bleibt auch ein möglicher Tippfehler sichtbar. Die bestehenden Darstellungsregeln für ausdrücklich geschriebene bekannte Tags bleiben erhalten; ebenso die Regeln für Leerraum und Zeilenabschluss. Anwendertext darf nicht durch pauschales Entfernen gleichlautender Textstücke verändert werden.

Die damaligen Protokolle `_630_test_pc_targets_insert.log` und `_640_test_pc_targets_bind.log` enthielten pro Workflow 14.655 fehlgeschlagene Einzelzeilen-/Spezialvergleiche. Der Abgleich aller erwarteten und tatsächlichen Strings ergab in beiden Workflows dieselben Abweichungen. Die folgende Tabelle beschreibt ausschließlich den **damals beobachteten Fehler**: `tatsächlich` ist keine gewünschte neue Testerwartung. Insbesondere bleibt `Hello World!` der richtige Erwartungswert; `untagged:Hello World!` war der zu behebende Ist-Wert.

| Ursache | Konkreter Unterschied | Häufigkeit je Workflow |
| --- | --- | ---: |
| Unerwünschtes automatisch ergänztes Präfix bei `-color off` | Erwartet `Hello World!`, tatsächlich `untagged:Hello World!`; entsprechend bei anderen ungetaggten Meldungen | 14.581 |
| Veralteter Feldname im manuellen JSON-Beispiel | Erwartet `... Birn:2, Fisch:2.781000}`, tatsächlich `... Birn:2, Fish:2.781000}` | 74 |

Das unerwünschte Präfix betrifft die Vergleiche mit den `//exp:`-Erwartungen in [triceCheck.c](../../_test/testdata/triceCheck.c), außerdem die Spezialfälle `dblB_de_protect_tcobs_ua`, `ringB_de_protect_tcobs_ua` und `modify_for_debug`. Bei zwei getrennten ungetaggten Aufrufen mit `Hello ` und `World!` muss die zusammengesetzte Textausgabe `Hello World!` bleiben. Das derzeit beobachtete `untagged:Hello untagged:World!` verletzt diesen Vertrag; die Klassifizierung beider Ereignisse bleibt trotzdem jeweils `untagged`. Der unabhängige `Fish`-Unterschied stammt aus `exampleOfManualJSONencoding()`: Der Formatstring verwendet bereits `Fish`, nur die Erwartung an der aufrufenden Teststelle enthält noch `Fisch`.

Je gewöhnlicher Einzelzeilenkonfiguration sind es 197 Präfixabweichungen und eine `Fish`-Abweichung. Die kombinierten Direct-/Deferred-Konfigurationen prüfen beides zweimal. Die drei genannten Spezialtests haben jeweils eine Präfixabweichung. Die hohen Summen entstehen durch Wiederholung über die Konfigurationen, nicht durch ebenso viele verschiedene Ursachen.

[Der Bulk-Vergleich](../../_test/testdata/cgoPackage.go) schneidet nach der erwarteten Textlänge weiter. Bereits das erste zusätzliche `untagged:` verschob in den früheren Protokollen die folgenden Ausschnitte; jeder der acht Bulk-Läufe meldete deshalb 1.743 Textunterschiede und Restdaten. Diese Folgefehler waren kein unabhängiger Nachweis eines Framing- oder Übertragungsdefekts. Nach der Korrektur besteht der gezielt ausgeführte Ringpuffer-Bulk-Lauf unter Insert und Bind; alle Bulk-Konfigurationen bleiben Teil der Full-Matrix bei R16.

Umgesetzt: Die automatisch zugewiesene Klassifizierung verändert den Message-Text in Text, JSON und KV nicht mehr. Die vorhandenen präfixfreien Erwartungen blieben erhalten; die separate `Fisch`-Erwartung lautet jetzt `Fish`. Weder Testausgaben noch Erwartungen wurden pauschal um `untagged:` bereinigt beziehungsweise erweitert. Verhaltenstests und die betreffenden UM-Aussagen folgen dem präzisierten Vertrag.

**Früher Testabbruch:** Vor der Korrektur koppelte `keepCheckingAfterFailure()` das Weiterprüfen innerhalb eines Pakets an `TRICE_TEST_NO_STOP=1`. Dadurch meldete der Bulk-Vergleich nach dem ersten Längenfehler Tausende Folgeunterschiede; die Einzelzeilenprüfung führte noch alle übrigen Logaufrufe aus. Die Bulk-, Einzelzeilen- und kombinierten Direct-/Deferred-Tests brechen jetzt innerhalb **jeder Konfiguration beim ersten fehlgeschlagenen Vergleich ab**. Die Diagnose nennt Index, Source-Zeile sowie erwarteten und tatsächlichen Wert und markiert die Konfiguration als fehlgeschlagen. Der äußere [PC-Test-Worker](../../scripts/_160_pc_target_test_worker.sh) diagnostiziert bei `--no-stop` fehlgeschlagene Bulk-Konfigurationen weiterhin einzeln und prüft danach die **nächste Konfiguration**; ohne `--no-stop` bleibt der bisherige Abbruch des Workflows nach Fehler und Gegenprobe. `-failfast` allein ersetzt den Rücksprung aus einer bereits laufenden Vergleichsschleife nicht.

Ein erster Fehler darf unabhängige Fehler nicht dauerhaft verdecken: Der `Fish`-Fall und die `untagged`-Ausgabe sind auch in gezielten, voneinander unabhängigen Verhaltenstests abgesichert. Im erfolgreichen Durchlauf bleiben sämtliche bisherigen Testfälle und Assertions aktiv; nur die Fehlerdiagnose endet pro Konfiguration früher. Der Bulk-Vergleich zählt nach einem Längenunterschied keine verschobenen Ausschnitte als eigenständige Fehler weiter.

**Abnahme:** Ungetaggte und unbekannt getaggte Meldungen in Text, JSON und KV prüfen, einschließlich `-color off`, zusammengesetzter Textfragmente und unverändertem Leerraum: Ohne wörtliches `untagged:` im Anwendungstext enthält keine ausgegebene Meldung diese Zeichenfolge; ein vom Anwender gelieferter gleichlautender Text bleibt erhalten. JSON/KV enthalten `tag="untagged"` und den unveränderten Message-Text; explizite bekannte Tags folgen weiterhin ihren bisherigen Regeln. Nachweisen, dass Auswahl über `untagged`, Gewichtsschwellen, Farbe und Ereignisstatistik weiterhin auf der Klassifizierung beruhen. Einen absichtlich ausgelösten frühen Fehler in Bulk, Einzelzeile und Direct/Deferred prüfen: jeweils genau ein aussagekräftiger Vergleichsfehler pro Konfiguration, aber unter `--no-stop` läuft die nächste Konfiguration weiter und der Gesamtschritt bleibt FAIL. Den unabhängigen `Fish`-Fall separat prüfen. Eine Bulk-/Einzelzeilenkonfiguration, eine kombinierte Direct-/Deferred-Konfiguration und die drei Spezialfälle gezielt nach Insert und Bind prüfen. Anschließend vollständige Matrix im finalen Lauf. Alle bisherigen Assertions und Konfigurationen bleiben erhalten; eventuelle dann noch sichtbare Abweichungen getrennt untersuchen.

### Kopierbare Dokumentationsbeispiele berichtigt

**R02 · Gewicht 5 · Aufwand S · Umsetzung abgeschlossen**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Im [UM](../TriceUserManual.md) ist die alternative CE-Regel für Position und Geschwindigkeit syntaktisch gültig. Die ABC-Dateinamen und ihr Standardort `generated/` entsprechen der CLI; ein expliziter Zielpfad bleibt möglich. Das nicht vorhandene `-tilCS`-Beispiel wurde durch eine zutreffende C#-Integrationsnotiz ersetzt. Das Testkapitel nennt die heutigen Skripte und `_test`; die Release-Kommandos sind einzeln kopierbar. Der Link im [PC-Feature-Beispiel](../../examples/PC_features/README.md) verwendet die versionierte Schreibweise `ReadMe.md`.

**Gezielte Abnahme:** Isolierte Bind- und ABC-Tests prüfen die CE-Regel sowie generierte Namen und Pfade. Die CLI weist `-tilCS` als unbekannten Schalter ab. UM-Format und Markdownlint bestehen; die lokale Linkprüfung für UM und PC-README findet keine Fehler. Die netzabhängige vollständige Linkprüfung war in dieser Umgebung wegen blockierter Verbindungen zu externen Websites nicht abschließbar. Release-/Git-Kommandos wurden nicht ausgeführt.

### Fehlerstatus bei fehlgeschlagenem Clean erhalten

**R03 · Gewicht 5 · Aufwand S · Umsetzung abgeschlossen**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die vier betroffenen Buildskripte speichern den echten Status von `trice clean`, bevor sie ihn auswerten. Ein allein fehlgeschlagenes Clean führt zum Fehlerstatus und meldet dessen Exitcode; ein bereits fehlgeschlagener Build oder eine Unterbrechung behalten ihren ursprünglichen Status. Das vereinfachte Cleanup-Beispiel im UM reicht Clean-Fehler ebenfalls weiter, ohne im normalen Abschluss erneut zu bereinigen.

**Gezielte Abnahme:** Isolierte Verhaltenstests führen alle vier Skripte mit erfolgreichem Build und Clean, Clean-Fehlercode 23, vorigem Build-Fehlercode 17 sowie SIGINT und SIGTERM aus. Pro Lauf werden Vor- und Nach-Clean genau einmal aufgerufen; Status und Warnung stimmen in allen Fällen. Shell-Formatprüfung, ShellCheck und die vollständige `scripts`-Testsuite bestehen.

### Automatisches Nachladen wiederhergestellt

**R04 · Gewicht 5 · Aufwand M · Umsetzung abgeschlossen; abschließende Full-Matrix bei R16**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Entscheidung: Automatisches Nachladen bleibt ein wichtiges zugesagtes Feature. Der Logger überwacht die Verzeichnisse der geladenen TIL-/LI-Dateien und verarbeitet damit auch atomaren Dateiersatz durch Bind/Insert. Rasche Folgespeicherungen gehen nicht mehr in einer fünfsekündigen Sperre verloren. Vollständig eingelesene Tabellen werden unter gemeinsamem Schreib-/Leseschutz ersetzt; Decoder, Positionsausgabe und Statistik verwenden denselben Schutz. Fehlerhafte, leere oder vorübergehend fehlende Dateien lassen den letzten gültigen Stand unverändert und werden erneut eingelesen. Diagnoseausgaben gehen auf stderr; gleiche wiederholte Lesefehler erzeugen keinen Warnungsstrom. Beim Verlassen des Loglaufs werden Watcher und Timer beendet und ihr Abschluss abgewartet.

Das [UM unter Easy-to-use](../TriceUserManual.md#easy-to-use) erklärt Bedienung und Grenzen: TIL und LI sind keine dateiübergreifende Transaktion, `{}` leert die jeweilige Tabelle absichtlich, eine beim Start fehlende LI-Datei bleibt für diesen Loglauf deaktiviert, und bereits deaktivierte Visualisierungsregeln werden nicht automatisch wieder aktiviert. Allgemeine Signal-/Receiver-Lebenszyklen bleiben Gegenstand von R08.

**Gezielte Abnahme:** [Watcher-Tests](../../internal/id/fileWatcher_test.go) prüfen reale Schreib-/Ersetzungsereignisse, Wiederanlage, ungültiges JSON ohne Teilübernahme, Wiederholung ohne Folgeereignis, stille Fehlerwiederholungen, abgeschaltete Pfade, Backendfehler und Ressourcenfreigabe. [CLI-Integrationstests](../../internal/args/fileWatcher_test.go) betreiben jeweils einen laufenden Logger für Text, JSON und KV mit echter Dateieingabe: geänderte Feldschemata und Positionen, mehrfacher Dateiersatz, Weiterloggen bei defektem JSON, Erholung und fortlaufende Visualisierung. Diese Tests bestehen auch mit Race Detector; die betroffenen Go-Paketsuites bestehen. Die lange Full-Matrix wurde nicht erneut gestartet.

### Kompatibilitätsvertrag und Release-Ziel festgelegt

**R05 · Gewicht 5 · Aufwand S–M · Abgeschlossen; Release-Ziel v2.0.0 bestätigt**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Entscheidung des Anwenders: **v2.0.0 ist das verbindliche Release-Ziel**. Die nachgewiesenen absichtlichen Änderungen an veröffentlichten CLI- und Template-Schnittstellen sind inkompatibel; nach [Semantic Versioning](https://semver.org/spec/v2.0.0.html) ist dafür ein Major-Schritt vorgesehen. Die Zahl der Brüche ist unerheblich. Ein v1.4.0 mit unveränderter Rückwärtskompatibilitätszusage beschreibt diesen Stand nicht zutreffend. Ein Tag oder eine Veröffentlichung sind damit nicht beauftragt.

Nachweis: Der lokale Release-Tag `v1.3.0` zeigt auf `54ce845b069f989bfc762f28f6dd364954e6050f`. Sein unveränderter Quellstand wurde mit `git archive` in ein temporäres Verzeichnis extrahiert und dort mit lokal vorhandenen Abhängigkeiten gebaut. Das aktuelle Tool wurde aus Stand `768844f1` separat in die temporäre Ablage gebaut. Beide CLI-Binaries wurden mit identischen TIL-/TREX-Fixtures ausgeführt. Es wurden weder historische Dateien im Repo geändert noch alte Quellstände in den Worktree zurückgesetzt.

| Vertrag | Beobachtete Änderung / zu dokumentierende Folge |
| --- | --- |
| CLI für C-Generierung | v1.3.0 erzeugt mit `generate -tilC` eine `til.c`; der aktuelle Host weist den Schalter mit Exitcode 2 ab. `-logC` hat einen auf aktuelle Source-Stellen bezogenen Vertrag. |
| CLI für Location-Daten | v1.3.0 akzeptiert `-liPath base`; der aktuelle Host weist es mit Exitcode 2 ab. `-liRoot` und `-liMaxDirs` trennen jetzt Speicherung und Darstellung. **Beide veröffentlichten Vergleichsschemata verwenden `File` und `Line`; `Path` war ein unveröffentlichter Zwischenstand.** |
| User-Tags | v1.3.0 akzeptiert `-ulabel alpha:beta` als zwei Tags. Der aktuelle Host weist `beta` als unbekannte Farbe ab. Je Tag eine Option verwenden; Doppelpunkt für Gewicht/Farbe. |
| Formatstrings | Der alte Host gibt `literal={x}` und `set={1,2}` wörtlich aus. Der aktuelle Host erwartet bei `{x}` einen Wert beziehungsweise weist `{1,2}` als ungültigen Feldnamen ab. `{{x}}` erscheint im alten Host doppelt geklammert und im aktuellen Host als `{x}`. Ein neues `{x}` mit einem 32-Bit-Wert wird nur vom aktuellen Host als strukturiertes Feld dekodiert. |
| Klassische Meldungen | Derselbe 32-Bit-Record mit `msg:count=%d` ergibt in beiden Hosts `count=7`; `hi` bleibt `hi`. [R01](#kein-automatisch-erzeugtes-untagged-präfix-ausgeben) ist umgesetzt: Automatische Klassifizierung als `untagged` fügt kein Präfix in den Meldungstext ein. |
| Tag-Auswahl und Darstellung | Eindeutige Aliase und gewichtete Schwellen gelten pro Anwendungsereignis; Metadaten werden separat behandelt. Unbekanntes `-logLevel` oder `-pick` wird jetzt vor dem Öffnen der Eingabe abgewiesen; v1.3.0 akzeptiert dieselben geprüften Werte. |
| Generierte Ablage | `generate -abc deviceX` erzeugt unter v1.3.0 `deviceX.h/.c` im Aufrufverzeichnis, aktuell unter `generated/`. `-genDir` ist der gemeinsame Verzeichnisschalter. |

Wichtig: `-buildDir`/`-bindDir` und das zeitweilige LI-Feld `Path` waren Zwischenstände der neuen Arbeit, keine zusätzlichen Brüche gegenüber v1.3.0. Release Notes müssen veröffentlichte Änderungen von unveröffentlichten Umbenennungen unterscheiden.

Der unterstützte Vertrag steht jetzt im [UM-Kapitel zur Firmware-/Host-Kompatibilität](../TriceUserManual.md#compatibility-with-firmware-and-host-tool-versions), mit einer Kombinationstabelle und konkreten Vorher-/Nachher-Ausgaben. README und CLI-Hilfe verweisen auf beziehungsweise nennen die Template-Grenze. Historische Firmware, zugehörige TIL/LI, Host-Version und Decodieroptionen zusammen archivieren. Alte Firmware mit literalen Klammern bleibt mit ihrem passenden alten Host reproduzierbar; neue Quellen nutzen doppelte literale Klammern und werden neu instrumentiert und gebaut. Ein kompatibles Recordlayout allein ist keine pauschale Zusage für beliebig gemischte Target-Quellen, Wörterbücher und Hosts. Ein Decoderdiagnose-Record führt zudem nicht zwingend zu einem fehlerhaften Prozess-Exitcode; Abnahme muss Meldungen und Diagnosen prüfen.

**Kein Migrationsprogramm und kein erneutes `-migrationBraces`.** Die früher verworfene Migration bleibt ausgeschlossen. Es geht um eine ehrliche Kompatibilitätsbeschreibung und gezielte Vergleichstests, nicht um still eingeführte Kompatibilitätsmechanismen.

**Gezielte Abnahme:** Der neue Test `TestReleaseCompatibilityWithV130Dictionaries` in [structured_test.go](../../internal/args/structured_test.go) sichert klassische Meldungen, ungetaggten Text, historische Klammerfehler, aktuelle Escape-Schreibweise und benannte Felder mit echten TREX-Bytes ab; die TIL bleibt dabei bytegleich. Vorhandene CLI-, Generator-, Tag- und Template-Tests decken die übrigen aktuellen Verträge ab. Der direkte Zwei-Binary-Vergleich bestätigt die dokumentierten alten Ausgaben und CLI-Unterschiede; die Standardtests benötigen weder Git-Historie noch eine installierte alte Trice-Version.

**Folgearbeiten:** Die Distributionsentscheidung ist getroffen: vorerst kein `/v2`, Installation über Binaries oder Repo-Checkout mit `./scripts/buildTriceTool.sh`. R14 sichert die dazugehörigen Anleitungen und Prüfungen ab. Go-Tests für `cmd`, `internal` und `pkg`, UM-Format, Markdownlint und lokale Linkprüfung bestanden. Die ausführlichen Release Notes bleiben R15, die vollständige Plattform-/Target-Abnahme R16.

### Testbeschleunigung mit vollständigen PC-Matrizen geprüft

**R06 · Gewicht 4 · Aufwand M · Umsetzung und vollständige PC-Gegenproben abgeschlossen**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Der Auftrag umfasst R08/R09 und P01/P04. Die wiederholte 100-ms-Wartezeit bei endlichen Eingaben entfällt, Logaufrufe geben ihre Ressourcen frei, geeignete Konfigurationen decodieren gesammelt und der PC-Worker führt höchstens vier Konfigurationen gleichzeitig aus. Die ursprünglichen Erwartungen und die zusätzlichen Tests jedes Pakets bleiben enthalten. Eine Infrastruktur zur Zeitmessung jedes Einzeltests wurde nicht eingeführt.

**Wiederherstellung nach Rechnerwechsel:** Der neue Checkout und der Server standen auf `c4bf94267d74e6e2dc07d03085a35c1425ee4ddb`; die Implementierungsänderungen waren darin noch nicht enthalten. Aus dem übertragenen Codex-Verlauf wurden 25 erfolgreiche Patch-Vorgänge für 19 Dateien einschließlich drei neuer Testdateien chronologisch rekonstruiert. Drei protokollierte fehlgeschlagene Patch-Versuche wurden ausgelassen. Die anschließende Formatierung und Prüfung erfolgten auf dem neuen Mac. Nicht protokollierte manuelle Änderungen und alte vollständige Logdateien lassen sich damit nicht beweisen oder wiederherstellen. Der [Leitfaden zum Rechnerwechsel](Codex_Rechnerwechsel_DE.md) beschreibt die künftig nötige Sicherung.

Die neue Abnahme erfolgte auf macOS/arm64 mit Go 1.25.3 und Apple Clang 21.0.0. Gemessen wurde die verstrichene Zeit des jeweiligen vollständigen verwalteten Workflows, einschließlich ID-Vorbereitung, Cache-Leerung, Kompilierung und Wiederherstellung. Alle Läufe verwendeten vier Worker. Die Werte sind Einzelmessungen und keine Hardware-unabhängige Laufzeitzusage.

| PC-Workflow | Ausführungsmodus | Ergebnis auf dem neuen Mac | Gesamtdauer |
| --- | --- | --- | --- |
| Bind, vollständige Matrix | `auto` | 63 von 63 Paketen bestanden; ursprünglicher Dateistand exakt wiederhergestellt | 6 min 17,37 s |
| Insert/Clean, vollständige Matrix | `auto` | 63 von 63 Paketen bestanden; ursprünglicher Dateistand exakt wiederhergestellt | 6 min 1,31 s |
| Bind, vollständige Gegenprobe | `line-by-line` | 63 von 63 Paketen bestanden; ursprünglicher Dateistand exakt wiederhergestellt | 7 min 16,92 s |

Die bestandenen Bind- und Insert-Läufe prüfen im Automatikmodus jeweils 30 Bulk-, 24 Einzelzeilen- und neun Spezialkonfigurationen. Die 54 gewöhnlichen Konfigurationen prüfen zusammen **129.130 erwartete Ausgaben pro Workflow**: 34 mit einem Kanal und 20 mit zwei Kanälen, jeweils 1.745 Erwartungen pro Kanal. Spezialtests und zusätzliche Verhaltenstests kommen hinzu. Die Gegenprobe bestand mit denselben 129.130 erwarteten Ausgaben, allen 54 gewöhnlichen Konfigurationen im Einzelzeilenweg und denselben neun Spezialkonfigurationen. Die Protokolle liegen lokal unter `temp/testtime-recovered-bind/`, `temp/testtime-recovered-insert/` und `temp/testtime-recovered-line/`; sie gehören nicht zum versionierten Stand.

**Historischer Vergleich:** Im Full-Lauf vom 29./30. September benötigte PC/Insert 3 h 52 min 8 s und PC/Bind 3 h 52 min 29 s. Diese Läufe scheiterten an den bei R01 beschriebenen Ausgaben; sie sind keine erfolgreiche Ausgangsabnahme. Der Rechner und die Werkzeugversionen unterscheiden sich zudem vom neuen Lauf. Daher keinen belastbaren Beschleunigungsfaktor aus diesen Einzelwerten ableiten. Der separat protokollierte erfolgreiche neue Bind-Lauf auf dem alten Mac benötigte rund 4 min 17 s. Für den dort zuletzt gestarteten Insert-Lauf fehlt nach dem Rechnerwechsel ein verlässlich übertragener Abschluss.

**Weitere Abnahme auf dem neuen Mac:** Die Go-Suites für `cmd`, `internal`, `pkg` und `scripts` bestehen. Gezielte Replay-/Lifecycle-Tests bestehen zusätzlich mit Race Detector. ShellCheck, Formatprüfung der geänderten Shell-/C-Dateien, UM-Format und Markdownlint bestehen. Ein erster eingeschränkter Go-Lauf scheiterte ausschließlich an den gesperrten lokalen TCP-/UDP-Testports; der anschließende Lauf mit erlaubtem Portzugriff bestand. Die gesamte `testAll full`-Suite einschließlich L432 und eine reale Windows-Matrix wurden hier nicht erneut ausgeführt; das bleibt R16. P02 und P03 bleiben getrennte optionale Folgearbeiten.

**Verbleibender Plattformhinweis für R16:** Im bestandenen Einzelzeilenlauf meldete Bash einmal `child setpgid ...: Operation not permitted`. Sämtliche Pakete, der Workflow-Exit und die exakte Dateiwiederherstellung waren erfolgreich. Der gezielte Test `TestPCWorkerCancellationReachesDescendants` bestand anschließend dreimal, jeweils für normale Jobs und einen Diagnose-Nachlauf einschließlich verzögert beendeter Kindprozesse. Die einmalige Meldung ist damit nicht reproduziert oder ursächlich erklärt; bei der Release-Abnahme auf die Prozessgruppenbildung und auf einen realen Matrixabbruch achten. Die Meldung wurde nicht unterdrückt.

### CE-/SL-Integration und Feature-Beispiele verbindlich ausgewählt

**R07 · Gewicht 5 · Aufwand M · Umsetzung und gezielte lokale Abnahme abgeschlossen**

[Zur Aufgabenübersicht](#aufgabenübersicht)

[Schritt 515](../../scripts/_515_test_logging_features.sh) läuft einmal in `quick` und `full`. Er aktiviert gezielt `TestContextEnrichmentTargetToDecoder` und `TestContextInsertCleanTargetToDecoder`: C/C++-Records, Text/JSON/KV, Feldtypen und Stempel, Bind, Insert/Clean-Rücknahme, abgeschaltetes Logging und einmalige Auswertung. Die bestehenden SL-Unit-Tests und bisherigen Bind-Prüfungen bleiben erhalten und werden nicht zusätzlich im neuen Schritt wiederholt.

Die kleinen Nachweise `TestContextEnrichmentPoC` und `TestContextEnrichmentPoCRebaseScopeBoundary` laufen als separat bezeichnete Gruppe mit. Der große experimentelle `TestContextEnrichmentRebasePoC` bleibt gezielt aufrufbar; sein Aufruf und die begrenzte Aussagekraft der jeweils verfügbaren Compiler stehen im UM. Daraus folgt weiterhin keine produktive CE-Unterstützung für Wrapper/Rebase.

Die unveränderten [PC-Ausgabeprüfungen](../../examples/PC_features/check_output.sh) und [G0B1-Buildprüfungen](../../examples/G0B1_features/check_build.sh) laufen in einer frischen Kopie der benötigten versionierten Dateien. Dabei werden aktuelle Worktree-Bytes einschließlich lokaler Source-Änderungen kopiert, keine früheren Objekte oder Captures. Originalquellen, gemeinsame `exampleData`-Dateien, TIL/LI und benutzereigene generierte Dateien bleiben unberührt. Erfolgreiche Kopien werden entfernt, fehlgeschlagene bleiben neben den Logs erhalten. Nichtleere ELF/HEX/BIN-Dateien sind zusätzliche Pflichtnachweise für G0B1.

Fehlende Werkzeuge führen bei `quick` zu einem ausdrücklich sichtbaren `WARN`, bei `full` zum Fehler. Ausgewählte Go-Tests müssen namentlich `PASS` melden; leere Auswahl oder übersprungene Untertests zählen als Fehler. Ein Fehler beendet den Schritt ohne Folgeprüfungen; Abbruch-Exitcodes bleiben erhalten. Die [Library CI](../../.github/workflows/trice_lib_reusable.yml) installiert zusätzlich das bereits von den Tests benötigte clangd und ruft denselben Schritt mit `full` auf.

**Gezielte Abnahme:** Die vier ausgewählten Go-Tests, die PC-Ausgabeprüfung und der G0B1-Firmwarebuild bestanden lokal auf macOS mit Clang/clangd und ARM GCC. [Verhaltenstests](../../scripts/logging_features_test.go) prüfen Auswahl und Einmaligkeit, fehlende Werkzeuge, leere/übersprungene Go-Auswahl, Fehlerabbruch, Signal-Exitcodes, fehlende/leere Firmware und unveränderte Originaldateien bei Erfolg und Fehler. Ein echter Windows-/Linux-Lauf, die Ausführung auf einem G0B1-Board und der GitHub-CI-Lauf wurden hier nicht durchgeführt; diese Plattformnachweise bleiben R16. Der vorherige Full-Lauf mit 25 Schritten bleibt historische Abnahme; die neue Auswahl umfasst 26 Schritte.

Die zusätzliche Abnahme durch den tatsächlichen Runner, mit ausschließlich Schritt 515 und strenger `full`-Werkzeugprüfung, bestand in **37 Sekunden für den Schritt**, einschließlich der abschließenden Dateizustandsprüfung **39 Sekunden insgesamt**. Das sind lokale Messwerte auf diesem Mac mit vorhandenem Go-Buildcache. Die Protokolle liegen unter `temp/log/r07-validation/`; der vorhandene Standardbericht wurde nicht überschrieben und zeigt inzwischen den späteren grünen Quick-Lauf mit 19 Schritten und 312 Sekunden. `go test ./scripts -count=1`, ShellCheck, Shellformat, Actionlint, UM-Format und Markdownlint bestehen. Für R07 wurde die gesamte Full-Matrix nicht erneut gestartet.

### Ressourcen und Signalbehandlung pro Loglauf abgeschlossen

**R08 · Gewicht 4 · Aufwand M · Umsetzung abgeschlossen**

[Zur Aufgabenübersicht](#aufgabenübersicht)

[Translate](../../internal/translator/translator.go) meldet seine Signalbehandlung beim normalen Abschluss wieder ab und wartet auf das Ende der zuständigen Goroutine. Der nutzlose periodische Ticker entfällt; die bestehende kurze Schonfrist nach SIGINT/SIGTERM bleibt erhalten und lässt sich beim normalen Abschluss abbrechen. Statistik, Diagnosen und Signal-Exitcode bleiben erhalten.

`binaryLogger.Close` und `bytesViewer.Close` reichen das Schließen an ihre besessenen Ressourcen weiter. Eingabe und Binärlogdatei werden genau einmal geschlossen, auch wenn eine Schließoperation fehlschlägt; der geliehene Diagnose-Writer bleibt offen. Der CLI-Loglauf schließt die vollständige Wrapperkette sofort nach `Translate`, bevor ein weiterer Loglauf beginnen könnte. Schließfehler gehen an den Aufrufer zurück.

**Abnahme:** [Lifecycle-Tests des Translators](../../internal/translator/lifecycle_test.go) starten und beenden die Signalbehandlung wiederholt und prüfen den Abschluss während der Schonfrist. Die Signal-Prozessprüfung in [translator_delta_test.go](../../internal/translator/translator_delta_test.go) prüft SIGINT und SIGTERM mit Bereitschaftssignal statt Warteannahme, genau einen Input-Close und erfolgreichen Exit. [CLI-Lifecycle-Tests](../../internal/args/lifecycle_test.go) prüfen echte Eingabe-/Binärdateien unter beiden Wrappers bei EOF, Lesefehler, Schreibfehler und Schließfehler; [Receiver-Tests](../../internal/receiver/receiver_test.go) prüfen Besitz und wiederholtes Close. Die gezielten Lifecycle-Tests bestehen auch mit Race Detector.

### Endliche Eingaben ohne pauschale Wartezeit abgeschlossen

**R09 · Gewicht 4 · Aufwand M · Umsetzung abgeschlossen**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die bisherige 100-ms-Mindestlaufzeit pro endlichem Logaufruf ist entfernt. Der TREX-Decoder gibt EOF erst zurück, wenn nach dem tatsächlichen Eingabeende keine gepufferten vollständigen Records mehr verarbeitet werden können. Ein leerer Record ist Fortschritt und beendet die Ausgabe nicht vorzeitig. Endliche Quellen schließen dann sofort ab; ein begonnenes letztes Textfragment wird weiterhin ausgegeben. Normales EOF erzeugt keine zusätzliche Diagnose im Text-/JSON-/KV-Ausgabekanal.

Ungeframte, fragmentiert gelesene Header und Nutzdaten bleiben bis zur Vervollständigung erhalten. Ein unmöglicher Längenwert eines bekannten festen Recordtyps beziehungsweise eine unbekannte ID geht weiterhin durch die Resynchronisierung. `FILE` bleibt eine Live-Quelle; `TCP4BUFFER` endet erst bei echtem Peer-EOF, nicht bei einem kurzzeitig leeren Read. Die bestehende Pause gegen beschäftigtes Warten bei inaktiven Live-Quellen bleibt erhalten.

Der historische Befund erklärt den großen Gewinn: Je Workflow liefen 34 gewöhnliche Konfigurationen mit etwa 183–185 Sekunden und 20 Direct-/Deferred-Kombinationen mit etwa 365 Sekunden, jeweils mit 1.745 Erwartungen pro Kanal. Allein 100 ms für `2 × (34 + 2 × 20) × 1.745` Logaufrufe ergeben rechnerisch **7 Stunden 10 Minuten 26 Sekunden**. Go/C-Übergänge waren damit nicht als Hauptursache nachgewiesen; auch der neue Bulk-Weg führt weiterhin jede C-Teststelle aus.

**Abnahme:** [Replay-Tests](../../internal/translator/lifecycle_test.go) verwenden echte TREX-Bytes: Daten und EOF im selben Read, mehrere gepufferte Records einschließlich leerer Meldung, Byte-für-Byte-Eingabe, verkürztes Endpaket, 16-/32-Bit-Stempel, doppelte 16-Bit-ID, langen Laufzeitstring, Abschlussfragment und Live-Pausen. Begrenzte Read-Zahlen weisen nach, dass endliche Eingaben nicht weiter gepollt werden. Die bestehenden Decoder-, Translator- und CLI-Suites prüfen zusätzlich Filter, Stempel, strukturierte Ausgabe und Ausgabefehler. Der Zeitvergleich steht bei R06.

### SL- und CE-Kapitel vollständig ins Englische übertragen

**R11 · Gewicht 5 · Aufwand M–L · Umsetzung abgeschlossen; für den Merge vorgezogen**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Die Kapitel „Structured Logging“ und „Trice Context Enrichment“ im [UM](../TriceUserManual.md#structured-logging) sind vollständig englisch, einschließlich aller Tabellen, Beispiele, Einschränkungen, Fehlererklärungen und des kapitelinternen CE-PoC-Anhangs. Auch der aktive CE-Absatz unter „Future Development“ ist übersetzt. Arbeitsauftragsnummern A9/A10 sind innerhalb dieser Kapitel durch fachliche Beschreibungen ersetzt; die experimentelle Wrapper-/Rebase-Unterstützung bleibt ausdrücklich von der produktiven Unterstützung getrennt. Testnamen, Testpfade und Funktionsumfang bleiben unverändert. Die weitere Bereinigung des Bind-/Visualisierungs-Kapitels und paralleler Dokumente bleibt R10.

Die vollständigen deutschen Originale vor der Übersetzung liegen als datierte Kapitelkopien unter [Structured_Logging_DE_2026-10-04.md](obsolete/Structured_Logging_DE_2026-10-04.md) und [Context_Enrichment_DE_2026-10-04.md](obsolete/Context_Enrichment_DE_2026-10-04.md). Auf weiteren Benutzerauftrag sind auch die verbliebenen deutschen Texte unter `bind-limits` und die deutsche Scratch-Pad-Notiz übersetzt; ihre Originale liegen unter [Remaining_Manual_Texts_DE_2026-10-04.md](obsolete/Remaining_Manual_Texts_DE_2026-10-04.md). `bind-limits` verweist nun ausdrücklich auf den vorhandenen Architektur-PoC und unterscheidet dessen Nachweis von der weiterhin zurückgestellten produktiven Integration. Bereits vorhandene Archive wurden nicht geändert. Die Kopien behalten die damaligen Überschriften und Verweise als historische Referenz; sie werden nicht als eigenständige Manuals gepflegt.

**Gezielte Abnahme am 4. Oktober:** Die Archivkopien stimmen bis auf den abschließenden Leerraum bytegenau mit den ursprünglichen vollständigen Kapiteln überein. Alle 47 Code-/Ausgabeblöcke sind unverändert und in gleicher Reihenfolge vorhanden. Der fachliche Absatzvergleich erhält insbesondere `message` und Leerraum, flache Punktnamen, Typen/NaN/64-Bit-Werte, getrennte Stempel-/Delta-Metadaten, CE-Regelreihenfolge, exakten Suffix-Match, lokale Sichtbarkeit, einmalige Auswertung und die unterschiedlichen Bind-/Insert-Grenzen. mdtoc erzeugt ToC, Nummerierung und Anker neu; Markdownlint und mdtoc-Check bestehen. Aktive eingehende Links zu den übersetzten Kapitelankern sind angepasst; `bind-limits` bleibt erhalten.

Die strengere lokale Lychee-Prüfung mit Fragmenten hatte außerhalb des Auftrags den Link `#Trice Parameter Bit Widths` im Target-Code-Überblick beanstandet. Der Benutzer hat ihn anschließend auf `#trice-parameter-bit-widths` korrigiert. Der parallel gestartete Full-Test bestand alle 26 Einzelschritte; seine abschließende Byte-Prüfung meldete die parallel beauftragten Dokumentationsänderungen. Der anschließende Quick-Lauf bestand einschließlich Zustandsprüfung (20 Schritte, 380 Sekunden). Produktcode und Testauswahl wurden für R11 nicht verändert. Das ist weiterhin kein vollständiger Release-Nachweis nach R16.

### Bulk als regulärer PC-Testweg mit verwertbarer Fehlerdiagnose

**P04 · Gewicht 4 · Aufwand M–L · Umsetzung abgeschlossen**

[Zur Aufgabenübersicht](#aufgabenübersicht)

`TRICE_PC_TEST_MODE=auto` wählt im [gemeinsamen Harness](../../_test/testdata/cgoPackage.go) den nachgewiesenen Weg je Konfiguration. Von 63 Paketen verwenden 30 Bulk, 24 weiterhin den Einzelweg und neun ihre speziellen Tests. Framed Direct und Deferred werden getrennt gesammelt und decodiert. Der bisherige Transfer nach jeder C-Teststelle bleibt dort erhalten, wo kleine Puffer ihn benötigen; die ursprünglichen acht Deferred-Bulk-Fälle behalten ihre Mehrstellen-Transfers zur Prüfung des Pufferns. Ungeframte Kanäle bleiben einzeln, damit Padding und Paketgrenzen nicht durch Verkettung verändert werden.

Alle 1.745 Erwartungen pro gewöhnlichem Kanal und sämtliche weiteren Tests in jedem Paket bleiben aktiv. Erfolgreiche Bulk-Konfigurationen werden nicht nochmals vollständig einzeln ausgeführt. `TRICE_PC_TEST_MODE=line-by-line` bleibt als explizite Gegenprobe verfügbar. Ein Overlay verwendet die zentralen Harness-Vorlagen, ohne 61 generierte Kopien umzuschreiben.

Der erste Bulk-Unterschied nennt `triceCheck.c:<Zeile>`, Erwartungsindex, Kanal, Byteposition, Soll/Ist mit sichtbaren Steuerzeichen und begrenzten Kontext. Mehrzeilige und leere Erwartungen behalten ihre Grenzen. Die genannte Zeile ist die erste abweichende Erwartung, nicht zwingend die Ursache einer früheren Datenstrombeschädigung. Original-Binärstrom und kompletter Text bleiben im konfigurationsbezogenen Logverzeichnis erhalten. Eine automatische Einzelgegenprobe nach einem Bulk-Fehler kann dessen Gesamtexit nicht wieder auf PASS setzen.

**Abnahme:** Deskriptive Harness-Tests prüfen richtigen, veränderten, fehlenden und zusätzlichen Text sowie leere und mehrzeilige Erwartungen. Der [C-gestützte Fehlerproben-Test](../../_test/ringB_de_multi_cobs_ua/cgo_test.go) beweist den ersten Abbruch für Einzel-, Bulk- und kombinierten Weg und prüft erhaltene Binär-/Textartefakte. Die vollständigen Matrixvergleiche stehen bei R06.

### PC-Konfigurationen begrenzt parallel geprüft

**P01 · Gewicht 4 · Aufwand M · Umsetzung abgeschlossen**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Der [PC-Worker](../../scripts/_160_pc_target_test_worker.sh) startet standardmäßig höchstens vier Konfigurationen als getrennte Prozesse. `TRICE_PC_TEST_JOBS` erlaubt eine andere positive Grenze, einschließlich `1` für seriellen Betrieb. Globale Go-/C-Zustände werden nicht mit `t.Parallel` geteilt. Jede Konfiguration besitzt ein eigenes `output.log`; pro Lauf entsteht ein neues Verzeichnis unter `temp/log/pc-<workflow>.<Lauf>/`.

ID-Vorbereitung und Wiederherstellung bleiben außerhalb der parallelen Phase. Ohne `--no-stop` wird nach dem ersten erkannten Fehler nur die bereits gestartete Gruppe beendet und keine weitere Gruppe begonnen. Mit `--no-stop` laufen die übrigen Konfigurationen weiter, der Gesamtexit bleibt fehlerhaft. Ein Abbruch erreicht auch Compiler-/Test-Kindprozesse und automatische Diagnose-Nachläufe; der Worker wartet vor der Source-Wiederherstellung auf deren Ende.

Fehlerberichte nennen Workflow, Konfiguration, Logpfad, relevante Diagnose und einen Reproduktionsaufruf. Auch der stille äußere `testAll`-Runner zeigt konkrete Fehlerausschnitte statt nur FAIL. Der Reproduktionsaufruf setzt denselben vorbereiteten ID-Zustand und die passenden Compiler-Include-Pfade voraus; der verwaltete Workflow stellt diese weiterhin bereit. Die groben Fortschrittsgewichte berücksichtigen den verkleinerten Anteil der PC-Matrizen.

**Abnahme:** [Isolierte Worker-Verhaltenstests](../../scripts/pc_target_worker_test.go) prüfen seriellen Erfolg, tatsächliche parallele Überlappung, Jobgrenze, getrennte Logs, Fail-fast, `--no-stop`, Fehlererhalt trotz erfolgreicher Gegenprobe, ungültige Steuerwerte und Abbruch einschließlich verzögert beendeter Kindprozesse im normalen und diagnostischen Lauf. [Runner-Tests](../../scripts/portability_test.go) prüfen die konkreten Fehlerdetails auch bei stiller Ausführung. Die vollständige `scripts`-Suite besteht. Die beschriebenen Signal-Prozessprüfungen laufen unter POSIX; eine reale Windows-Matrix bleibt Teil von R16.

## Bestandszuordnung und Befunde der Repo-Prüfung

**R17 · Gewicht 4 · Lesende Bestandsprüfung abgeschlossen am 4. Oktober 2026**

[Zur Aufgabenübersicht](#aufgabenübersicht)

Grundlage ist `git ls-files -z` auf `cb4dda6495530ad33ed703301ee3df9625ecf3ce`: **2.239 versionierte Dateien**, davon **27 unmittelbar im Root**. Die folgenden disjunkten Hauptgruppen erfassen alle Pfade einschließlich versteckter Dateien. Ein Verzeichnisprefix umfasst seine Unterverzeichnisse; spezifische Ausnahmen stehen darunter. Damit ist auch jeder durch versionierte Dateien belegte Unterordner zugeordnet. Git führt keine eigenständigen leeren Verzeichnisse. Lokale Ausgaben werden separat betrachtet.

Die Zuordnung beruht auf Dateiinventar, Einstiegstexten, aktiven Referenzen sowie Build-/Test-/Release-Verbrauchern und deren Globs. Sie ist keine erneute Einzelprüfung jeder C-Funktion, keine Ausführung sämtlicher Beispiele und keine Untersuchung des Inhalts aller Fremdsoftware-Archive. „Behalten“ bedeutet ein belegbarer Zweck, nicht garantierte Fehlerfreiheit. „Noch klären“ ist eine ausdrückliche offene Frage und keine verkappte Löschfreigabe. Bei R17 wurde nur dieser Plan geändert; keine Dateien wurden bereinigt und keine Produkttests oder Builds gestartet.

### Vollständige Zuordnung der Hauptgruppen

| Pfadgruppe | Dateien | Zweck und Verbraucher | Empfehlung und Ziel |
| --- | ---: | --- | --- |
| Root-Dateien ohne Unterverzeichnisse | 27 | Einstieg, Regeln/Lizenzen, Toolkonfiguration, Modul, gemeinsame ID-Daten; Einzelzuordnung unten. | Überwiegend **behalten**; fragliche Ausgaben getrennt unter [R21c](#root-ausgaben-und-generierte-beispieldaten-unterscheiden). |
| `.code_snippets/` | 11 | Eine README und zehn `.7z`-Archive mit ausdrücklich als Backup/Legacy beschriebenen Codefragmenten. | **Behalten** als bestehende Historie; keine Entpack-/Aufräumaktion in [R21](#übriges-repo-anhand-belegter-zwecke-aufräumen). |
| `.github/` | 30 | 17 ausführbare Workflow-YAML, sechs Repo-/Issue-/PR-Konfigurationen, eine Smoke-Fixture und sechs erklärende/Vorlagen-Dateien. | Aktive Automation/Fixture **behalten**; fünf Vorlagenbegleiter und allgemeine README unter [R21a](#kleine-zustandsreste-und-workflow-begleitdateien); Pages unter [R22](#github-pages-mit-eindeutigem-einstieg-und-veröffentlichungsumfang). |
| `.idea/` | 14 | Gemeinsamer IDE-/CLion-Einstieg, Format-/Inspektionsregeln und Projektmetadaten. | Zweck **behalten**, Doppelmodul/Wörterbuch-/Ignore-Widersprüche unter [R21b](#ide-einstiege-portabel-und-tatsächlich-benutzbar-machen) **zusammenführen**. |
| `.vscode/` | 3 | Root-Editor-, C/C++- und Go-Debugkonfiguration. | **Behalten**, kaputte/persönliche Startpfade unter [R21b](#ide-einstiege-portabel-und-tatsächlich-benutzbar-machen) korrigieren. |
| `_test/` | 427 | PC-/CGO-Matrix, ABC-Tests, kanonische C-/Harness-Eingaben und CLion-Reviewprojekt. | **Behalten**; Aufteilung unten. Keine Konfiguration wegen ähnlicher Dateien streichen. |
| `cmd/` | 19 | Ausgelieferte Tools, Dokumentations-/Formatter-Helfer sowie unfertige Tools und ruhende Tests. | Aktive Programme **behalten**; fünf Experimentdateien inzwischen nach `obsolete/cmd` archiviert. Zwei Unterstrich-Tests unter [R21d](#unfertige-tools-ruhende-tests-und-entwicklernotizen-einordnen) **noch klären**. |
| `demo/` | 9 | Kleiner PC-Einstieg für direkte/verzögerte Ausgabe mit gemeinsamem Skript und TIL/LI. | **Behalten**; Einstieg für [R19](#ein-kurzes-user-manual-zum-ausprobieren-erstellen), kein Duplikat der umfangreicheren Feature-Tour. |
| `docs/` | 215 | Vollreferenz, Bind-Dokumente, Bilder, Planung und historische Unterlagen. | Zuordnung unten; **zusammenführen/verschieben** unter [R10](#anwenderdokumentation-von-entwicklungsständen-befreien)/[R18](#bisheriges-user-manual-als-reference-manual-weiterführen)–[R20](#link-forwarding-dateien-entfernen-und-docs-konsolidieren), Archive **behalten**. |
| `examples/` | 1.116 | Ausführbare PC-/STM32-/ABC-/LabPlot-Beispiele, Vendorquellen, Projektdaten und Builds. | **Behalten**; 766 Dateien liegen in `Drivers`/`Middlewares`. Ausnahmen [R21a](#kleine-zustandsreste-und-workflow-begleitdateien)–c. |
| `experiments/` | 63 | Sechs Bind-Architektur-/Integrationsnachweise mit eigenen Eingaben und erwarteten Resultaten. | Inzwischen vollständig archiviert; keine Abhängigkeit regulärer Produktprüfungen mehr, Detailzuordnung unten. |
| `internal/` | 145 | Hostimplementierung und Tests: args, charDecoder, com, decoder, do, dumpDecoder, emitter, fmtspec, id, keybcmd, link, receiver, translator, trexDecoder, vis. | **Behalten** am Ort; auch `id/remigratecmd` ist ein aktiver Helfer des Legacy-Testworkflows. |
| `pkg/` | 20 | Gemeinsame Go-Pakete und Tests: ant (3), cipher (5), msg (5), tst (7). | **Behalten**; keine Paketneuorganisation aus der Bestandsprüfung ableiten. |
| `scripts/` | 62 | 50 nummerierte Workflow-/Prüf-/Pflegeskripte, vier Einstiegsskripte, drei Git-Helfer und fünf Testdateien. | **Behalten**; vorhandene Funktionen statt neuer paralleler Abläufe dokumentieren. |
| `src/` | 55 | 53 Target-C-/Headerdateien, `ReadMe.md` und eine lokale VS-Code-Konfiguration. | **Behalten**; Quellcode-/README-Auslieferung über GoReleaser, Archivabnahme [R15](#release-notes-und-ausgelieferte-dateien-prüfen). |
| `third_party/` | 23 | Elf ZIP-Dateien, zwei PDFs, sechs Readmes und vier Ignore-Dateien für optionale Werkzeuge/Fremdquellen. | Benötigte Quellen/Hinweise **behalten**; konkrete Aufbewahrungsrolle je Archiv unter [R21f](#fremdsoftware-ablage-erklären-und-alt-konfiguration-abgleichen) **noch klären**. |
| **Summe** | **2.239** | **Jeder versionierte Pfad gehört genau einer Hauptgruppe an.** | **Keine pauschale Löschliste.** |

### Root-Dateien und operative Konfiguration

| Dateien | Anzahl | Heutiger Zweck / Entscheidung |
| --- | ---: | --- |
| `README.md`, `AGENTS.md`, `AUTHORS.md`, `CHANGELOG.md`, `CODE_OF_CONDUCT.md`, `CONTRIBUTING.md`, `LICENSE.md`, `SECURITY.md` | 8 | Projekteinstieg, Mitarbeit, Urheberschaft, Historie und Regeln. **Behalten**; README [R12](#readme-repo-orientierung-beispiele-und-zusagen-verbessern), Release-Abschnitt [R15](#release-notes-und-ausgelieferte-dateien-prüfen), knappe Entwicklerorientierung [R21d](#unfertige-tools-ruhende-tests-und-entwicklernotizen-einordnen). Historische Changelog-Einträge nicht umschreiben. |
| `.clang-format`, `.clang-format-ignore`, `.editorconfig`, `.gitattributes`, `.gitignore`, `.goreleaser.yaml`, `.markdownlint.yaml`, `.markdownlintignore`, `lychee.toml` | 9 | Aktive Format-/Git-/Release-/Lintkonfiguration. **Behalten**; gezielte Pfadänderungen bei [R18](#bisheriges-user-manual-als-reference-manual-weiterführen)–[R22](#github-pages-mit-eindeutigem-einstieg-und-veröffentlichungsumfang). Vendor-/Archiv-Ausnahmen nicht durch Aufräumen aufheben. |
| `.markdownlinkcheck.json` | 1 | Veralteter lokaler Markdown-Linkchecker laut UM; aktive CI nutzt Lychee. **Noch klären**, dann [R21f](#fremdsoftware-ablage-erklären-und-alt-konfiguration-abgleichen). |
| `_config.yml`, `index.md` | 2 | Website-Konfiguration und Einstieg; konkurrierende Erzeugung im Pages-Workflow. Zweck **behalten**, Erzeugung unter [R22](#github-pages-mit-eindeutigem-einstieg-und-veröffentlichungsumfang) **zusammenführen**. |
| `go.mod`, `go.sum` | 2 | Hostmodul und reproduzierbare Abhängigkeiten. **Behalten**, ausdrücklich keine `/v2`-Umstellung. |
| `demoLI.json`, `demoTIL.json` | 2 | Gemeinsame Standort-/ID-Tabellen der Beispiele und ID-Workflows. **Behalten**; lokale Veränderungen weiterhin nicht als Aufräumauftrag behandeln. |
| `trice_bindIDs_in_examples_and_test_folder.sh` | 1 | Gemeinsame Bind-Vorbereitung; wird von Formatter und verwalteten Testworkflows aufgerufen. **Behalten**, kein unbenutztes Root-Skript. |
| `til.c`, `trice.bin` | 2 | Ehemalige generierte Root-Tabelle und leere Binärdatei ohne konkreten Buildverbraucher. Unter [R21c](#root-ausgaben-und-generierte-beispieldaten-unterscheiden) entfernt und gezielt ignoriert. |

Die vier nicht nummerierten Test-/Build-Einstiege sind `scripts/testAll.sh`, `format_repo.sh`, `buildTriceTool.sh` und `release_prep.sh`. Die drei `git*.sh`-Dateien dienen ausdrücklich manueller Entwicklungsarbeit und benötigen keinen CI-Aufrufer, um sinnvoll zu sein. Die fünf Testdateien prüfen PC-Worker, L432-Matrix, Logging-Auswahl, Skriptportabilität und Codex-Handover. Zusammen mit den 50 nummerierten Skripten sind damit alle 62 Dateien zugeordnet.

`cmd/trice` enthält fünf aktive Dateien; die zwei alten `main_update*_test.go` liegen als Entwicklungsnachweis unter `docs/scratchPad/obsolete/cmd/trice/`. `cmd/tlog` enthält zwei aktive Dateien. `cmd/clang-filter` (3) und `cmd/generate-helpall-doc` (2) sind Entwicklerwerkzeuge, keine unfertigen Produkt-CLIs. Die früheren `cmd/_cui` (2) und `cmd/_stim` (3) liegen inzwischen unverändert unter `docs/scratchPad/obsolete/cmd/`. Die Zahlen der Bestandsaufnahme bleiben historische Vergleichswerte.

### Beispiele und Tests sind keine beliebigen Doppelbestände

| Beispielgruppe unter `examples/` | Dateien | Zweck, Verbraucher und Ziel |
| --- | ---: | --- |
| `F030_bare`, `F030_inst` | 61 + 71 | Basisprojekt und instrumentierte Variante; STM32Cube-/Compilerdateien, Vendorquellen und Trice-Anbindung **behalten**. |
| `G0B1_bare`, `G0B1_inst` | 142 + 154 | Basisprojekt und RTOS-/Trice-Integration; Quelle der bewusst geklonten Feature-Tour. **Behalten**. |
| `G0B1_features`, `G0B1_log` | 159 + 154 | SL-/CE-/Task-Beispiele beziehungsweise Target-seitige Logformatierung. **Behalten**, IDE-Zustand unter [R21a](#kleine-zustandsreste-und-workflow-begleitdateien)/b. |
| `L432_bare`, `L432_inst` | 143 + 159 | Basis/Instrumentierung und Quelle der 101 Konfigurationsprüfungen in Schritt 620. **Behalten**. |
| `PC_features`, `PC_log` | 11 + 6 | Schnell ausführbare Feature-Tour mit `show_*.sh`/Ausgabeprüfung und lokale Target-Formatierung. **Behalten**, wichtige Einstiegspunkte [R19](#ein-kurzes-user-manual-zum-ausprobieren-erstellen)/[R12](#readme-repo-orientierung-beispiele-und-zusagen-verbessern). |
| `DemoData_CSV`, `DemoData_Trice` | 4 + 5 | Zwei Datenproduzenten für denselben Visualisierungsweg, direktes CSV gegenüber Binärlogging. **Behalten**. |
| `LabPlotDemo`, `LabPlotUser` | 4 + 1 | Fertiges Visualisierungsbeispiel und bisher eigenständige Nachbauanleitung. Nach R23 liegt die Nachbauanleitung zentral im RM; die lokale README bleibt ausschließlich als Link dorthin. |
| `TriceAbc` | 34 | Broadcast-/Empfangsbeispiel mit NodeLib, Auswahl-/Generatorworkflow und Laufskripten. **Behalten**; generierte Tabelle separat [R21c](#root-ausgaben-und-generierte-beispieldaten-unterscheiden). |
| `exampleData` | 3 | Gemeinsam eingebundene Beispiel-/Diagnosequellen; erklärt Sidecars auch für nicht aufgerufene Demo-Funktionen. **Behalten**. |
| Direkte Dateien: `ReadMe.md`, `buildAllTargets_TRICE_OFF.sh`, `buildAllTargets_TRICE_ON.sh`, `cleanAllTargets.sh`, `prepareTriceBind.sh` | 5 | Gemeinsame Beispiel-Workflows und derzeit nur weiterleitender Einstieg. Skripte **behalten**; Auswahlhilfe zentral ins UM, README unter [R23](#beispielanleitungen-zentralisieren-und-readmes-auf-links-reduzieren) ausschließlich als Links auf UM/RM **behalten**. |
| **Summe** | **1.116** | **766 Vendor-Dateien sind darin enthalten, nicht zusätzlich gezählt.** |

Die STM32-Projekte werden über ihre Buildskripte, Makefiles und IDE-/Cube-Konventionen benutzt. Eine Textsuche nach jeder Vendor-Headerdatei genügt deshalb nicht. Die vom Benutzer ausdrücklich gewünschten eigenständig kopierbaren Projekte werden nicht zur Platzersparnis in eine neue gemeinsame Vendor-Struktur umgebaut. Innerhalb der acht Boardprojekte bleiben Anwendungsquellen, Startup/Linker, Projektgenerator-/Debuggerkonfiguration, Build-/Logscripts und Drittanbieterhinweise beim jeweiligen Beispiel. Die zwölf leeren Debugger-Zustandsdateien bilden die eng begrenzte Ausnahme R21a.

Für `_test` ist die vollständige Zuordnung: `dblB_*` 138 Dateien, `ringB_*` 138, `stackB_*` 38, `staticB_*` 42, `be_*` 12, `alias_*` 9, `aliasassert_*` 9, `userprint_*` 9, `abc_rx_host`/`abc_tx_host` zusammen 12, `modify_for_debug` 8, `clion-review` 5, `testdata` 6 und `ReadMe.md` 1; zusammen **427**. Die Buffer-/Framing-/Endian-/Ausgabevarianten sind Testeingaben, auch wenn Harness-Dateien ähnlich aussehen. `_160_pc_target_test_worker.sh` verwendet bereits gemeinsame Harness-Vorlagen per Go-Overlay. Eine weitere Zusammenlegung wäre Testarchitekturarbeit und gehört nicht zu R21. Das CLion-Projekt ist ein manueller Code-Review-/Build-Einstieg. `_test/ReadMe.md` kann R12 zu einem kurzen Wegweiser ergänzen; sein bisheriger Weiterleitungscharakter macht nicht den Ordner überflüssig.

### Entwicklungsnachweise und Archive erhalten

Die sechs abgeschlossenen Bind-Experimente (63 Dateien) und die unfertigen Werkzeuge `cmd/_cui`/`cmd/_stim` (5 Dateien) liegen unter `obsolete/experiments` beziehungsweise `obsolete/cmd`. Sie sind historische Unterlagen, keine aktiven Testeingaben und keine Anwenderbeispiele.

**Korrektur der bisherigen Archivierung:** Schritt 500 führt die archivierten Präprozessor-, Rebase- und CMake-Versuche nicht mehr aus und kopiert keine Archivdateien. Er prüft ausschließlich die aktuelle Bind-Implementierung über `internal/id/bindIntegration_test.go`: generierte C/C++-Header, die kanonische Makromatrix, Rebase, Counter-Fehlerfälle und ausgegebene IDs. Die vollständigen PC-Insert-/Bind-Matrizen prüfen weiterhin Target-Ausgabe und Dekodierung. Die zusätzlichen historischen Demonstrationen gehören nicht mehr zur regulären Suite; damit wird nicht behauptet, ihr früherer Sonderumfang sei unverändert erhalten.

Die zugehörigen Skript-Verhaltenstests prüfen erfolgreiche Produkttests ohne Experimente/CMake, Compiler-Fallback, Go-Fehler mit Diagnose und frühes Überspringen bei fehlendem Go beziehungsweise C-/C++-Compiler. Die aktiven CE-Integrationstests bleiben erhalten: Sie prüfen aktuelle CE-Funktionen und deren Scope-Grenzen, ohne Archivdateien einzubinden.

**Anwenderdokumentation:** Das UM enthält keine Verweise mehr auf Scratchpad, Plan oder archivierte Werkzeuge/Experimente. Der vollständige englische historische ELF-Vergleich ist separat unter `obsolete/TriceBind/Bind_ELF_Comparison_EN.md` erhalten; aktuelle Bind-Nutzung und Grenzen bleiben im UM. Der überholte Link zu den alten Encodings ist entfernt. Die bisher vorhandenen Archivdateien bleiben bei dieser Korrektur unverändert.

**Dauerhafte Trennung:** Aktive Anwenderdokumentation muss aus sich heraus zur Benutzung genügen. Einzigartige gültige Informationen aus Entwürfen zuerst integrieren; keine Archivlinks als Ersatz für eine Erklärung. Reguläre Produktprüfungen verwenden gepflegte Quellen, Beispiele und Fixtures außerhalb von Scratchpad/Archiv. Schutzregeln in `.gitignore`, Markdownlint, Lychee und der IDE bleiben bestehen: Sie schließen Arbeitsmaterial aus und sind keine inhaltlichen Abhängigkeiten. Die verbliebene Python-Importabhängigkeit der Entwicklerwerkzeuge ist konkret in R24 erfasst.

### Dokumentationsbestand und fachliche Ziele

Die **215 Dokumentationsdateien** zerfallen in 16 direkte Dateien, 9 Bind-Dokumente, 94 Referenz-/Bilddateien, 8 `_Legacy`-Dateien sowie 88 Scratchpad-Dateien. Im Scratchpad sind vier aktiv: dieser Plan, `scratchPad.md` und die zwei `codex_handover_*.py`; die übrigen 84 liegen unter `obsolete`.

| Aktiver Bestand | Zweck heute | Ziel und Empfehlung |
| --- | --- | --- |
| `docs/TriceUserManual.md` | Vollständige Referenz einschließlich übersetzter SL-/CE-Kapitel. | **Verschieben** nach `TriceReferenceManual.md` gemäß [R18](#bisheriges-user-manual-als-reference-manual-weiterführen), Inhalt erhalten; [R19](#ein-kurzes-user-manual-zum-ausprobieren-erstellen) belegt den alten Namen mit dem kurzen Einstieg. |
| `docs/README.md` | Derzeit pauschaler Verweis auf das UM; behauptet unzutreffend, die anderen Dateien seien nur Links. | Als eigenständigen Dokumentationswegweiser **behalten** und unter [R12](#readme-repo-orientierung-beispiele-und-zusagen-verbessern) neu füllen. |
| Zehn Weiterleitungsdateien, unten einzeln aufgeführt | Keine eigene fachliche Erklärung. | Unter [R20](#link-forwarding-dateien-entfernen-und-docs-konsolidieren) **entfernen**, nachdem aktive Verweise und Zielkapitel stimmen. |
| `docs/scratchPad/Codex_Rechnerwechsel_DE.md` | Eigenständige, ausdrücklich beauftragte Anleitung für Entwickler; derzeit zusammen mit den Handover-Werkzeugen im Scratchpad. | Entwicklerablauf **erhalten**, Ablage und Aufrufbeispiele gemeinsam unter [R24](#entwicklerwerkzeuge-vom-scratchpad-entkoppeln) bereinigen; nicht ins englische Anwenderhandbuch mischen. |
| `docs/scratchPad/GoInfos.txt` | Acht Zeilen ältere Testwerkzeug-/Coverage-Notizen. | Bereits außerhalb aktiver Dokumentation; die gültigen Testwege stehen nach [R21d](#unfertige-tools-ruhende-tests-und-entwicklernotizen-einordnen) in `CONTRIBUTING.md`. |
| `docs/ChatGPTo4-mini-high_TriceCompare.html`, `docs/2026-02-16_ChatGPT5.2ProExtThinking_embedded_logging_tracing_comparison_trice_focus.html` | Zwei aus dem Root-README verlinkte Vergleichsberichte. | Datierte Fremdeinschätzungen; Einbindung unter [R12](#readme-repo-orientierung-beispiele-und-zusagen-verbessern), mögliche historische Ablage unter [R21e](#dokumentationsbilder-und-vergleichsberichte-konsolidieren) **noch klären**. |
| `docs/TriceBind/` (damaliger Pfad) | README; Dateien 10/30: Spezifikation und Testanforderungen; 20/40/70: Implementierungsaufträge; 50: Bericht; 60: Strategien; 90: paralleles User Manual. | Nach [R10](#anwenderdokumentation-von-entwicklungsständen-befreien) auf Benutzerauftrag inhaltlich abgeglichen und nach `docs/scratchPad/obsolete/TriceBind/` **verschoben**. Gültige Anwenderinformationen, Architekturvergleich und Testeinstiege stehen englisch im Handbuch. |
| `docs/ref/` ohne `Backup.7z` | 67 PNG, 8 GIF, 8 SVG, 7 JPG, 2 Drawio-Quellen und eine generierte CLI-Hilfe. | Benötigte Quellen/Exporte **behalten**; fragliche Varianten [R21e](#dokumentationsbilder-und-vergleichsberichte-konsolidieren). Bilder nicht als aktuelle Messungen neu ausgeben. |
| Aktive vier Dateien unter `docs/scratchPad/` | Deutsche Planung, auskommentierte Notizen und beauftragte Umzugswerkzeuge. | Plan/Werkzeuge **behalten**; Notizzettel unter [R21d](#unfertige-tools-ruhende-tests-und-entwicklernotizen-einordnen) **noch klären**. Keine daraus abgeleiteten neuen Features. |

#### Vollständige Auswahl der Weiterleitungsdateien

| Zu entfernende Datei unter `docs/` | Fachlicher Zielinhalt nach [R18](#bisheriges-user-manual-als-reference-manual-weiterführen)/[R19](#ein-kurzes-user-manual-zum-ausprobieren-erstellen) |
| --- | --- |
| `TriceColor.md` | Reference Manual: Tags, Farben und Log-Level; der Stub verwendet noch den alten Anker `trice-tags-and-color`. |
| `TriceIDManagement.md` | Reference Manual: ID-Verwaltung. |
| `TriceMessagesEncoding.md` | Reference Manual: aktuelle binäre Kodierung. |
| `TriceObsoleteEncodings.md` | Bestehende historische Texte unter `_Legacy`, sofern ein historischer Verweis benötigt wird; keine Gleichsetzung mit aktueller Kodierung. |
| `TriceOverOneWire.md` | Reference Manual: Betrieb ohne UART. |
| `TriceOverRTT.md` | Reference Manual: RTT. |
| `TriceSpace.md` | Reference Manual: Speicherbedarf und Messbedingungen. |
| `TriceSpeed.md` | Reference Manual: Geschwindigkeit und Messbedingungen. |
| `TriceUserGuide.md` | Neues kurzes User Manual als Einstieg. |
| `TriceVsPrintfSimilaritiesAndDifferences.md` | Reference Manual: Vergleich mit printf. |

Bei der aktiven Textsuche wurden überwiegend Dateinamen in historischen Changelog-Einträgen gefunden, keine notwendige neue Nutzung der Stubs. Der historische Kodierungsverweis des UM führt bereits direkt nach `_Legacy`. Vor Entfernung dennoch die tatsächlichen Linkziele einschließlich HTML und veröffentlichtem Site-Baum prüfen; eine Namenssuche unterscheidet gleichnamige Archivziele nicht sicher. Changelog und Archive bleiben unverändert. Externe Altlinks können nach Entfernung brechen; R20 dokumentiert diese Folge, ohne neue Weiterleitungsstubs zu erfinden.

Der Referenzbestand enthält 23 nicht archivierte Dateien ohne gefundenen Dateinamenverweis in den untersuchten aktiven Texten außerhalb von `docs/ref`. Darunter sind die beiden `.drawio`-Quellen, mehrere Logoauflösungen und ältere Mess-/Boardbilder. Das ist lediglich eine Prüfauswahl für R21e: Quell-/Exportbeziehungen innerhalb der Bildfamilien und andere Renderwege können ihre Aufbewahrung begründen. Insbesondere `trice_abc_*`-Diagramme mit `.png`, `.svg`, `*2.svg` und `.drawio` nicht anhand der Endung oder eines Suffixes als überflüssig erklären.

### Lokale Ausgaben und Aussagegrenzen

Zusätzlich zum versionierten Bestand sind im aktuellen Repo-Root `.git`, `.gocache`, `build`, `dist`, `generated`, `temp` und `coverage.out` vorhanden. `.git` ist lokale Repository-Verwaltung und kein aufzuräumender Produktbestand. Cache, Build-/Releaseausgaben und Coverage sind lokale Ergebnisse; `temp` enthält auch Testlogs und Wiederherstellungsdaten. Die vorhandenen Ignore-Regeln unterscheiden diese Gruppen bereits. Auch entsprechende Unterverzeichnisse in Beispielen werden nicht zur versionierten Dateizahl addiert.

`generated` enthält unter anderem Sidecars und die Feldregistrierung. Die Rolle eines Ordners ist nicht gleichbedeutend mit der Erlaubnis, alle seine Dateien zu löschen: ABC-Auswahl-Header können Benutzereingaben sein, alte Sidecars können ID-Evidenz liefern. Handover-ZIPs enthalten persönliche Sitzungsdaten und gehören gemäß vorhandener Ignore-Regel nicht ins Repo oder in die Website. Dieser Auftrag liest ihre Inhalte nicht und räumt sie nicht auf.

Die Dateigröße des Repos allein ist kein Grund zur Konsolidierung der 766 Vendor-Dateien oder der Testkonfigurationen. Die Bestandsprüfung bestätigt überwiegend begründete Gruppen und isoliert kleinere Unsicherheiten. Bei unklaren manuellen Verwendungen werden R21c/d/f zunächst mit einer Zweckentscheidung abgeschlossen oder ausdrücklich zurückgestellt; ein Release muss dafür keine beliebige Universalbereinigung abwarten.

### Aus der Bestandsprüfung abgeleitete Orientierung

Für R12 reicht im öffentlichen Einstieg diese Karte: **Ausprobieren:** `demo` und `examples/PC_features`; **Target integrieren:** `src` und die passenden `*_inst`-/Feature-Beispiele; **lokal formatieren/ABC/visualisieren:** `PC_log`, `TriceAbc`, `LabPlotDemo`; **Hosttool bauen:** `scripts/buildTriceTool.sh`; **Tests ausführen:** `scripts/testAll.sh`; **nachschlagen:** kurzes UM und Reference Manual; **beitragen:** `CONTRIBUTING.md`, danach bei Bedarf `cmd`, `internal`, `pkg`, `_test` und `scripts`. Experimente, Planung und Archive sind weiterführende Entwicklungsunterlagen, keine Voraussetzungen für das erste Log.

**Abnahme R17:** Alle 2.239 Pfade sind den gezählten Hauptgruppen zugeordnet; Root-Dateien, Beispiele, Testfamilien, Experimente und aktive Dokumentationsgruppen sind zusätzlich aufgeschlüsselt. Referenzprüfung und Build-/Test-/Release-Verwendung begründen die Empfehlungen. Offene manuelle Nutzungen sind sichtbar. Sechs kleine Folgegruppen R21a–f und der Veröffentlichungsauftrag R22 ergänzen R10/R12/R15/R18–R20, ohne deren Arbeit doppelt zu beauftragen. Es wurde keine der empfohlenen Bereinigungen ausgeführt.

**Prüfung des Ergebnisses:** Die Tabellenzählung wurde maschinell mit `git ls-files -z` verglichen: alle Hauptgruppen, alle 27 Root-Dateien und alle 1.116 Beispieldateien sind vollständig und ohne Doppelzuordnung erfasst; die sechs Experimentzahlen stimmen ebenfalls. Markdownlint wurde für diese normalerweise ausgeschlossene Plan-Datei ausdrücklich ausgeführt und besteht. Lokale Dateiziele ihrer Markdown-Links existieren; `git diff --check` ist sauber. Externe Webseiten wurden für diese Repo-Bestandsprüfung nicht erneut geprüft.

## Hintergrund und Vorbemerkungen

Stand: 4. Oktober 2026. Ursprüngliche Bestandsaufnahme auf Basis von Commit `9b4e2abb`, des lokalen Release-Tags `v1.3.0` und des abgeschlossenen Full-Testlaufs vom 29./30. September, ergänzt um die gezielte Abnahme der Testbeschleunigung und die Full-Läufe vom 3. und 4. Oktober. Die Repo-Bestandsprüfung R17 bezieht sich auf `cb4dda6495530ad33ed703301ee3df9625ecf3ce`; ihre Zuordnung und Folgeaufgaben stehen am Ende dieses Plans. Dieser Plan bleibt deutsch. Er erteilt **keinen Implementierungs-, Commit-, Issue- oder Release-Auftrag**.

Ziel ist ein verlässliches Release der bereits vorhandenen Funktionen. Weitere Features sind dafür nicht erforderlich. Vorrang haben nachgewiesene Fehler, vollständige Abnahme und verständliche, zutreffende englische Anwenderdokumentation.

**Beschlossenes Release-Ziel: v2.0.0.** Der Go-Modulpfad bleibt vorerst ohne `/v2`. Angeboten werden fertige Binaries und der lokale Build aus einem Repo-Checkout über `./scripts/buildTriceTool.sh`; versionierte Go-Modulinstallation von v2 wird vorerst nicht angeboten. R14 sichert diese Installationswege ab. Ein Tag oder eine Veröffentlichung sind damit nicht beauftragt.

## Stand und Aussagegrenzen

Untersucht wurden die CLI und ihre Hilfe, ID-Verwaltung und Generatoren, Bind und Insert/Clean einschließlich CE, Template-Parser, Decoder, strukturierte Ausgabe, Tags und Filter, Visualisierung, Transport- und Ausgabeabschluss, Target-Konfiguration und Testaufbau, Beispiele, aktives UM, README sowie Test- und Release-Workflows. Code und vorhandene Verhaltenstests wurden mit den dokumentierten Verträgen verglichen. Das ist eine breite statische Bestandsaufnahme mit konkreten Belegen, keine vollständige Fehlerfreiheitserklärung oder neue Hardware-Abnahme.

Der vom Benutzer gestartete Lauf `./scripts/testAll.sh full --no-stop` wurde nach seinem Abschluss anhand der damaligen vollständigen Protokolle ausgewertet. Bei jener Bestandsaufnahme wurde kein weiterer Testlauf, Build, Formatter oder ID-Workflow gestartet. Die vier damals abweichenden Beispiel-JSON-Dateien wurden semantisch mit HEAD verglichen und zunächst unverändert gelassen. Ein späterer `testAll quick` für R13 hat die flachen Logdateien des Full-Laufs ersetzt; dessen hier festgehaltene Messwerte und Fehleranalyse bleiben historische Befunde.

- Ergebnis laut der damaligen `testAll_summary.log`: **23 Schritte PASS, 2 Schritte FAIL**, Gesamtdauer **8 Stunden 12 Minuten 4 Sekunden**. Nur Schritt 630 (PC/Insert) und Schritt 640 (PC/Bind) scheiterten. Die L432-Matrix mit 101 Konfigurationen bestand und brauchte etwa **20 Minuten 14 Sekunden**.
- Im damaligen Lauf führten beide PC-Workflows jeweils acht Bulk- und 63 Einzelzeilen-/Spezialkonfigurationen aus. Acht Bulk- und 57 Einzelzeilen-/Spezialläufe scheiterten, sechs Spezialläufe bestanden. Die fehlgeschlagenen Tests hießen jeweils `TestTriceLog`; es waren keine fehlgeschlagenen Compileraufrufe. Die spätere Beschleunigung und erfolgreiche PC-Abnahme stehen bei R06/R08/R09/P01/P04.
- Sämtliche protokollierten Einzelzeilen-Abweichungen sind zwischen Insert und Bind identisch und fallen in zwei Gruppen: ein unerwünschtes automatisch ergänztes `untagged:` im ausgegebenen Meldungstext und einmal `Fisch` gegenüber tatsächlich ausgegebenem `Fish`. Nach der präzisierten Benutzerentscheidung bleibt `untagged` eine Klassifizierung und darf die Message nicht verändern; die Präfix-Erwartungen sind deshalb nicht pauschal zu erweitern. Der Bulk-Vergleich verschiebt nach dem ersten Längenunterschied weitere Ausschnitte und erzeugt dadurch umfangreiche Folgefehler. Einzelheiten und Abnahme stehen bei R01.
- Schritt 600 führt eigenständige Builds von `PC_log` und `G0B1_log` aus. Die vier lokalen TIL/LI-Tabellen sind inzwischen auf dem aktuellen Source-Stand. R13 schützt ihren Anfangszustand und die zugehörigen lokalen Artefakte bei Testläufen; zwei gezielte identische Standalone-Builds pro Beispiel haben weder JSON-Bytes noch Datei-Inodes verändert.
- Schritt 550 meldet **91,1 % Go-Statement-Coverage**. Das ist weder ein Vergleich mit der Zielbranch-Baseline noch Coveralls-Zeilenabdeckung oder Target-C-Abdeckung.
- Die Compiler-/Decoder-Integrationstests für CE benötigen `TRICE_BIND_INTEGRATION=1`. Die ursprüngliche Auswahl ließ sie aus; R07 aktiviert sie jetzt gezielt in Schritt 515 für `quick`, `full` und die Library CI. Normale Go-Unit-/Coverage-Läufe bleiben davon getrennt.
- Aktuelle GitHub-Issues und Live-CI-Ergebnisse wurden nicht vollständig abgeglichen. Vor einer späteren Issue-Erstellung sind vorhandene Issues auf Dopplungen zu prüfen. Dieser Auftrag erstellt keine Issues.

Die erledigten A1–A10 und der CE-Folgeauftrag für Insert/Clean sind aus der offenen Liste entfernt. Ihre Details bleiben im [historischen Abschlussbericht](obsolete/Implementierungsplan_bis_A10.md). Die früheren M01–M16 und Handovers bleiben im vorhandenen Archiv. Ein historisch erledigter Implementierungsauftrag ersetzt keine heutige Regressionstest-Abnahme; die frühere A6-Notiz zur `untagged:`-Erwartung wird durch den bei R01 präzisierten Ausgabevertrag neu bewertet.

Vorhanden sind insbesondere:

- Structured Logging mit skalaren 8/16/32/64-Bit-Werten, `triceS`/`triceN`, Text, NDJSON und KV; benannte Pufferfelder bleiben ausdrücklich ausgeschlossen.
- CE für direkte, zeileneindeutige Bind-Stellen und für erkannte Insert/Clean-Aufrufe einschließlich statischer Wrapperdefinitionen. Insert/Clean verwendet den vollständigen Format-/Argumentsuffix, ohne Herkunftskommentare.
- Tag-Aliase, Gewichte, Auswahl, `untagged`, Ereignisstatistik, getrennte Diagnosen, Zeitstempel und Deltas, Visualisierung sowie gemeinsame generierte Ablage über `-genDir`.
- `generate -logC`, zusätzliche `*.oneline.json`-Ansichten und die PC-/G0B1-Feature-Beispiele.
- Erhaltene CE-PoCs einschließlich Wrapper-/Counter-Rebase-Untersuchung. Deren allgemeine produktive Integration ist weiterhin zurückgestellt.

## Ziel für Dokumentation und Orientierung

README und User Manual sollen zum Ausprobieren einladen: Nutzen schnell erkennen, einen überschaubaren Einstieg finden und erst bei Bedarf Details nachschlagen. Das bisherige ausführliche UM bleibt inhaltlich erhalten und wird zu `docs/TriceReferenceManual.md`. Ein neues, deutlich kürzeres `docs/TriceUserManual.md` übernimmt den geführten Einstieg.

**Verbindliche Dokumentationsentscheidung:** Ein überschaubares UM und ein vollständiges RM sind gewünscht. Alle Trice-Anwendererklärungen zu den Beispielen gehören nach Inhalt in diese beiden Handbücher. Beispiel-READMEs enthalten ausschließlich Links auf ihre zentralen Anleitungen. Das UM verweist gezielt ins RM sowie in Beispiele, Code und Tests, damit Neuankömmlinge nicht zwischen verstreuten Anleitungen suchen müssen. Die Root-README bleibt der kurze Projekteinstieg. Diese Entscheidung ersetzt frühere Empfehlungen, eigenständige Anleitungen in Beispiel-READMEs zu pflegen.

| Dokument | Aufgabe im künftigen Aufbau |
| --- | --- |
| `README.md` | Kurze Vorstellung, Nutzen, kleines Beispiel mit Ausgabe, verlässlicher Startpunkt und kompakte Repo-Orientierung. |
| `docs/TriceUserManual.md` | Zusammenhängender, überschaubarer Einstieg vom ersten PC-Log bis zur eigenen Target-Anbindung; kurze Feature-Beispiele und gezielte Verweise ins RM, zu Beispielen, Code und Tests. |
| `docs/TriceReferenceManual.md` | Vollständige Verträge, Optionen, Konfiguration, Grenzen, Hintergrund und CE-PoC-Anhang; fachlich maßgebliches Nachschlagewerk. |
| Eigene `README.md`/`ReadMe.md` unter `demo` und `examples` | Ausschließlich relative Links auf die passende zentrale Anleitung im UM/RM; keine eigenständigen Anleitungsabsätze oder Kommandos. |
| `docs/README.md` | Kurzer Dokumentationswegweiser mit Zielgruppe und Zweck der verbleibenden aktiven Dokumente; keine bloße Weiterleitungsdatei. |

Reine Link-Forwarding-Dateien in `docs` entfallen, nachdem ihre aktiven eingehenden Verweise angepasst sind. Für das gesamte Repo wird der Zweck jedes Verzeichnisses und jeder Datei geprüft. Die öffentliche Übersicht bleibt kompakt; die vollständige Bestandsprüfung wird dadurch nicht ersetzt. R17–R21 ergänzen dafür die bestehenden R10–R12, ohne Übersetzung und Einstieg doppelt zu beauftragen. Jetzt wird ausschließlich geplant.

## Gewichtung und Planungshinweise

**Gewicht:** 5 = vor Release zu klären oder abzustellen; 4 = hoher Nutzen für Zuverlässigkeit, Dokumentation oder Testdauer; 3 = sinnvolle Wartung nach den dringenden Punkten; 2 = optionaler Ausbau; 1 = bewusst zurückgestellt.

**Aufwand:** S = kleine, abgegrenzte Änderung; M = mehrere zusammenhängende Änderungen mit Verhaltenstests; L = Architektur-/Buildänderung oder breiter Plattformnachweis. Das sind Schätzungen, keine Zeitversprechen. Fehlersuche kann eine Aufgabe vergrößern.

Die Reihenfolge bevorzugt kleine Aufgaben, berücksichtigt aber Abhängigkeiten. R01–R09, R11, R13 und P01/P04 sind umgesetzt; ihre Nachweise stehen unter den erledigten Korrekturen. P03 ist ebenfalls abgeschlossen; der Nachweis steht bei der L432-Beschleunigung. R17 ist als lesende Bestandsprüfung abgeschlossen, nicht als Repo-Bereinigung. Die abschließende Release-Abnahme bleibt bei R16. Die vorhandenen IDs bleiben für Verweise erhalten. Unabhängige Dokumentationsarbeit kann während langer Tests erfolgen. Für Gewicht 5 reicht kein stilles Vertagen: Vor Release muss entweder die Korrektur abgenommen oder eine konkrete Einschränkung ausdrücklich entschieden und dokumentiert sein.

## Vorschlag für die nächsten Aufträge

Die bisher beauftragten Schritte der Gesamtaufgabe **Testzeit verkürzen** sind abgeschlossen: R06, R08/R09, P01/P04 und die L432-Beschleunigung P03. Umsetzung und Nachweise stehen unten. P02 zur gezielten Go-/CGO-Cache-Invalidierung bleibt ein möglicher nächster Beschleunigungsschritt; Laufzeiten je Skript sind inzwischen sichtbar. Eine Einzeltest-Zeitmessungsinfrastruktur wurde wie vereinbart nicht aufgebaut. Die produktive CE-/SL-Testauswahl **R07** ist umgesetzt. R17, R21a, R21b und R10 sind abgeschlossen. Als nächster Auftrag folgt R18: das vollständige UM als Reference Manual weiterführen und seine Verbraucher nachziehen. R19 bleibt der ausdrücklich gewünschte kurze Einstieg; R23 zentralisiert anschließend die Beispielanleitungen und reduziert deren READMEs auf Links. P02 kann anhand neuer Plattformmessungen priorisiert werden. v2.0.0 ist weiterhin das bestätigte Release-Ziel.

R11 wurde auf Benutzerwunsch für den zeitnahen Merge von `wip` nach `main` vorgezogen und ist abgeschlossen, einschließlich der eng begrenzten Bereinigung von Aufgabenbezeichnungen innerhalb der beiden Kapitel. Den Merge führt der Benutzer auf GitHub aus. Nach der abgeschlossenen Bestandsprüfung folgt die Dokumentationsarbeit weiterhin **R10 Bereinigung → R18 Reference Manual → R19 kurzes User Manual → R23 zentrale Beispielanleitungen und Link-READMEs → R12 Root-README und Orientierung → R20 zusätzliche Themen-Weiterleitungen entfernen → R22 Pages-Abnahme**. R21a–c sind unabhängig davon abgeschlossen; R21d–f haben oben eigene Voraussetzungen. Die Dateiablage allein rechtfertigt weder neue Features noch einen Umbau der STM32-Beispiele oder der Testmatrix.

R14 sichert den beschlossenen v2-Distributionsweg ab; seine Installationsvorgaben werden bereits beim Schreiben des neuen Einstiegs verwendet. Danach R15/R16 für Release Notes und Abnahme beider Handbücher und des bereinigten Repos. P02 und F-Aufgaben bleiben zur späteren Auswahl offen. Die deutsche Planung und historischen deutschen Texte bleiben außerhalb der englischen Anwenderdokumentation.
