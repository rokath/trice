# Release-Vorbereitung und weiterer Arbeitsplan

Stand: 4. Oktober 2026. Bestandsaufnahme auf Basis von Commit `9b4e2abb`, des lokalen Release-Tags `v1.3.0` und des abgeschlossenen Full-Testlaufs vom 29./30. September, ergänzt um die gezielte Abnahme der Testbeschleunigung und die Full-Läufe vom 3. und 4. Oktober. Dieser Plan bleibt deutsch. Er erteilt **keinen Implementierungs-, Commit-, Issue- oder Release-Auftrag**.

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
- Die Compiler-/Decoder-Integrationstests für CE benötigen `TRICE_BIND_INTEGRATION=1`. Die normale Testauswahl aktiviert diese Tests nicht vollständig; Einzelheiten stehen bei R07.
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

| Dokument | Aufgabe im künftigen Aufbau |
| --- | --- |
| `README.md` | Kurze Vorstellung, Nutzen, kleines Beispiel mit Ausgabe, verlässlicher Startpunkt und kompakte Repo-Orientierung. |
| `docs/TriceUserManual.md` | Schrittweise Anleitung vom ersten PC-Log bis zur eigenen Target-Anbindung; kurze Feature-Beispiele und gezielte Verweise auf Details. |
| `docs/TriceReferenceManual.md` | Vollständige Verträge, Optionen, Konfiguration, Grenzen, Hintergrund und CE-PoC-Anhang; fachlich maßgebliches Nachschlagewerk. |
| `docs/README.md` | Kurzer Dokumentationswegweiser mit Zielgruppe und Zweck der verbleibenden aktiven Dokumente; keine bloße Weiterleitungsdatei. |

Reine Link-Forwarding-Dateien in `docs` entfallen, nachdem ihre aktiven eingehenden Verweise angepasst sind. Für das gesamte Repo wird der Zweck jedes Verzeichnisses und jeder Datei geprüft. Die öffentliche Übersicht bleibt kompakt; die vollständige Bestandsprüfung wird dadurch nicht ersetzt. R17–R21 ergänzen dafür die bestehenden R10–R12, ohne Übersetzung und Einstieg doppelt zu beauftragen. Jetzt wird ausschließlich geplant.

## Gewichtung und Arbeitsreihenfolge

**Gewicht:** 5 = vor Release zu klären oder abzustellen; 4 = hoher Nutzen für Zuverlässigkeit, Dokumentation oder Testdauer; 3 = sinnvolle Wartung nach den dringenden Punkten; 2 = optionaler Ausbau; 1 = bewusst zurückgestellt.

**Aufwand:** S = kleine, abgegrenzte Änderung; M = mehrere zusammenhängende Änderungen mit Verhaltenstests; L = Architektur-/Buildänderung oder breiter Plattformnachweis. Das sind Schätzungen, keine Zeitversprechen. Fehlersuche kann eine Aufgabe vergrößern.

Die Reihenfolge bevorzugt kleine Aufgaben, berücksichtigt aber Abhängigkeiten. R01–R06, R08/R09, R13 und P01/P04 sind umgesetzt; ihre Nachweise stehen unter den erledigten Korrekturen. P03 ist ebenfalls abgeschlossen; der Nachweis steht bei der L432-Beschleunigung. Die abschließende Release-Abnahme bleibt bei R16. Die vorhandenen IDs bleiben für Verweise erhalten. Unabhängige Dokumentationsarbeit kann während langer Tests erfolgen. Für Gewicht 5 reicht kein stilles Vertagen: Vor Release muss entweder die Korrektur abgenommen oder eine konkrete Einschränkung ausdrücklich entschieden und dokumentiert sein.

| Reihenfolge / ID | Aufgabe | Gewicht | Aufwand | Voraussetzung |
| --- | --- | ---: | --- | --- |
| R07 | Vorhandene CE-/SL- und Beispielprüfungen verbindlich ausführen | 5 | M | R01, R13; erforderliche Compiler |
| R17 | Repo-Bestand und Dokumentationsziele je Datei prüfen | 4 | S–M | Lesende Bestandsprüfung; vor Löschungen/Verschiebungen |
| R10 | MVP-/Aufgabenreste und doppelte Anwenderdokumentation bereinigen | 4 | M | R02, R04, R17 |
| R11 | SL- und CE-Kapitel vollständig ins Englische übertragen | 5 | M–L | R02, R10 |
| R18 | Bisheriges UM in TriceReferenceManual.md umbenennen und Pfade nachziehen | 4 | M | R10, R11, R17 |
| R19 | Kurzes, einladendes User Manual erstellen | 4 | M | R18; Installationsentscheidung aus R05/R14 |
| R12 | README und Repo-Orientierung einladend überarbeiten; Zusagen präzisieren | 4 | M | R05, R17, R18, R19 |
| R20 | Link-Forwarding-Dateien entfernen und aktive docs konsolidieren | 4 | S–M | R12, R17, R18, R19 |
| R21 | Repo anhand der Bestandsprüfung in kleinen Schritten aufräumen | 3 | M–L | R17, R20; bekannte Datei-/Buildabhängigkeiten |
| R14 | Checkout-/Binary-Installationswege für v2 absichern | 5 | S–M | R05 abgeschlossen; kein `/v2` beschlossen |
| R15 | Release Notes und Prüfung der ausgelieferten Artefakte | 5 | M | R05, R07, R11, R12, R14, R18–R20 |
| R16 | Abschließende Release-Abnahme | 5 | M; lange Laufzeit | R01–R05, R07, R11, R13, R15; alle aufgenommenen Korrekturen einschließlich Repo-Aufräumen |

Die weiter unten aufgeführten P- und F-Aufgaben sind kein Grund, ein ansonsten abgenommenes Release um neue Features zu vergrößern.

## Vorschlag für die nächsten Aufträge

Die bisher beauftragten Schritte der Gesamtaufgabe **Testzeit verkürzen** sind abgeschlossen: R06, R08/R09, P01/P04 und jetzt die L432-Beschleunigung P03. Umsetzung und Nachweise stehen unten. P02 zur gezielten Go-/CGO-Cache-Invalidierung bleibt ein möglicher nächster Beschleunigungsschritt. Eine Einzeltest-Zeitmessungsinfrastruktur wurde wie vereinbart nicht aufgebaut. Die fehlende produktive CE-/SL-Testauswahl **R07** bleibt ein eigener nächster Auftrag. v2.0.0 ist weiterhin das bestätigte Release-Ziel.

Die Dokumentationsarbeit kann parallel zu langen Tests beginnen: **R17 Bestandsprüfung → R10 Bereinigung → R11 Übersetzung → R18 Reference Manual → R19 kurzes User Manual → R12 README und Orientierung → R20 Weiterleitungsdateien entfernen**. Die Bestandsprüfung kommt zuerst, damit beim Verkürzen und Entfernen keine eigenständigen Informationen verloren gehen. R21 räumt anschließend das übrige Repo in belegbaren Einzelgruppen auf; kleine unabhängige Gruppen können nach R17 vorgezogen werden, sofern sie keine offenen Dokumentationspfade betreffen.

R14 sichert den beschlossenen v2-Distributionsweg ab; seine Installationsvorgaben werden bereits beim Schreiben des neuen Einstiegs verwendet. Danach R15/R16 für Release Notes und Abnahme beider Handbücher und des bereinigten Repos. P02 und F-Aufgaben bleiben zur späteren Auswahl offen. Die deutsche Planung und historischen deutschen Texte bleiben außerhalb der englischen Anwenderdokumentation.

## Konkrete Aufgaben vor dem Release

### Vorhandene neue Integrationstests in die Standardabnahme aufnehmen

**R07 · Gewicht 5 · Aufwand M · Lücke in der Testauswahl bestätigt**

[Schritt 500](../../scripts/_500_test_bind.sh) setzt `TRICE_BIND_INTEGRATION=1`, wählt aber nur fünf ältere Bind-Tests in `internal/id` aus. Die normalen Go-/Coverage-Läufe setzen diese Variable nicht. Damit fehlen in der regulären Auswahl insbesondere die vorhandenen [CE-Target-/Decoder-Tests](../../internal/args/context_enrichment_test.go) `TestContextEnrichmentTargetToDecoder` und `TestContextInsertCleanTargetToDecoder`. Auch die CE-PoCs werden dadurch nicht vollständig ausgeführt.

Die neuen [PC-Ausgabeprüfungen](../../examples/PC_features/check_output.sh) und [G0B1-Buildprüfungen](../../examples/G0B1_features/check_build.sh) werden von den untersuchten Test-/CI-Einstiegspunkten ebenfalls nicht aufgerufen. Normale SL-Unit-Tests und Teile der C-Matrix sind bereits vorhanden; diese müssen nicht neu erfunden werden.

Aufgabe: Bestehende Prüfungen einer klaren, dokumentierten Auswahl zuordnen. Erforderliche Compiler/clangd erkennen; im Release-Lauf darf ein fehlendes Pflichtwerkzeug nicht als bestandene Abnahme erscheinen. Den großen experimentellen Rebase-PoC getrennt von produktiver CE-Unterstützung ausweisen. Neue Beispielprüfungen müssen isoliert laufen oder ihren Ausgangszustand exakt wiederherstellen.

**Abnahme:** Protokolle nennen die tatsächlich ausgeführten produktiven CE-/SL-End-to-End-Tests und Beispielprüfungen. C/C++-Records, Text/JSON/KV, Insert/Clean-Rücknahme, Bind, Abschaltung und einmalige Argumentauswertung sind enthalten. Fehlende Plattformnachweise werden offen benannt.

### Daseinsberechtigung und Zielort des Repo-Bestands prüfen

**R17 · Gewicht 4 · Aufwand S–M · Lesende Bestandsprüfung vor der Bereinigung**

Alle versionierten Dateien einschließlich versteckter Konfiguration und alle Repo-Verzeichnisse erfassen. Für jede Datei muss ein begründeter Zweck erkennbar sein: Produktcode, Build/Test, Beispiel, Anwenderdokumentation, Entwicklungsnachweis, benötigte Fremdquelle/Lizenz oder bewusst erhaltene Historie. Gleichartige Bestände dürfen nachvollziehbar als Gruppe beschrieben werden; kein Pfad darf ungeprüft außerhalb der Zuordnung bleiben. Lokale Buildausgaben und generierte Dateien getrennt betrachten, ohne Benutzerdateien oder die vereinbartermaßen ignorierten `demo*.json`-Änderungen einzusammeln.

Für jeden fraglichen Bestand festhalten: heutiger Zweck und Nutzer, aktive Referenzen beziehungsweise Build-/Test-/Release-Verwendung, vorgesehener Zielort und Empfehlung „behalten“, „zusammenführen“, „verschieben“, „entfernen“ oder „noch klären“. Fehlende Textreferenzen allein beweisen keine Nutzlosigkeit; Skripte, Tool-Konventionen, Globs und manuell benutzte Beispiele mitprüfen. Für Dokumente insbesondere reine Weiterleitung von eigenständiger Erklärung unterscheiden und ein eindeutiges fachliches Zieldokument zuordnen.

**Abnahme:** Vollständige, überprüfbare Zuordnung des Bestands und kleine umsetzbare Aufräumgruppen mit Abhängigkeiten. Unklare Zwecke sind ausdrücklich offen, nicht stillschweigend Löschkandidaten. Vorhandene Archive bleiben als Historie unverändert; die Prüfung ersetzt keinen Auftrag, sie zu bearbeiten. Daraus eine knappe Orientierung nach Nutzeraufgaben für R12 ableiten, keine riesige Dateiliste im README.

### Anwenderdokumentation von Entwicklungsständen befreien

**R10 · Gewicht 4 · Aufwand M · Befunde bestätigt**

Im aktiven UM stehen MVP-Bezeichnungen sowohl im `-vis)-Kapitel als auch ausführlich im Bind-Kapitel. Der CE-Anhang enthält A9/A10 im Fließtext und in Überschriften. Das [separate deutsche Bind-Manual](../TriceBind/Trice_bind_90_MVP_User_Manual.md) und die dortige README präsentieren parallel eine weitere normative Anwendersicht.

Aufgabe: Das bisherige UM als künftiges Reference Manual zur eindeutigen fachlichen Referenz machen. Das neue kurze User Manual führt später durch die Nutzung und verweist für vollständige Verträge dorthin. Aktuelle Grenzen konkret benennen; „MVP“ nicht blind durch „vollständig unterstützt“ ersetzen. Historische Architekturvergleiche müssen als solche erkennbar bleiben und dürfen aktueller Wrapper-Unterstützung nicht widersprechen. Implementierungsaufträge/Entwurfsberichte nach Prüfung aus dem aktiven Einstieg nehmen; wertvolle Begründungen erhalten. Die tatsächliche Dateibereinigung folgt R20/R21.

Die CE-PoC-Ergebnisse bleiben wie beauftragt im kapitelinternen Anhang, einschließlich reproduzierbarer Testreferenzen und ihrer Aussagegrenzen. A9/A10 werden dort durch verständliche Bezeichnungen wie „Nachweis für direkte Logstellen“ und „Produktive Unterstützung“ ersetzt. Testnamen und Experimentpfade werden nicht nur wegen eines historischen Namens umbenannt.

**Abnahme:** Aktive Anwendertexte enthalten keine unerklärten Arbeitsauftragsnummern oder überholten MVP-Status. Vorhandene Archive bleiben unangetastet. Kommentarblöcke und historische Changelogs werden nicht als neue Produktanforderungen behandelt.

### Die beiden neuen UM-Kapitel ins Englische übertragen

**R11 · Gewicht 5 · Aufwand M–L · Beauftragung der späteren Umsetzung erforderlich**

Die derzeitigen Kapitel „Strukturiertes Logging“ und „Trice Context Enrichment“ vollständig übersetzen, einschließlich Tabellen, Einschränkungen, Beispiele, Fehlererklärungen und CE-PoC-Anhang. Auch den verbleibenden deutschen CE-Absatz unter „Future Development“ angleichen. Diese vollständigen Kapitel gehören nach R18 ins Reference Manual; das neue kurze UM erhält unter R19 ausgewählte Einstiegsbeispiele mit Verweisen dorthin.

Vorher die deutschen Originale als datierte, vollständige Kapitelkopien unter `docs/scratchPad/obsolete/` sichern. Sie werden anschließend historische Referenzen, keine parallel gepflegten Manuals. Bereits dort liegende alte Drafts nicht überschreiben. Originale vor der englischen Bearbeitung sichern; spätere fachliche Korrekturen müssen im Diff zur Archivfassung nachvollziehbar sein.

Bei der Übersetzung besonders erhalten: Bedeutung von `message` und Leerraum, flache Punktnamen, Typen/NaN/64-Bit-Zahlen, getrennte `ts16`/`ts32`-Metadaten, CE-Regelreihenfolge, exakter Suffix-Match, lokale Sichtbarkeit, einmalige Auswertung, Ausschluss benannter Pufferfelder sowie die unterschiedlichen Bind-/Insert-Grenzen.

**Abnahme:** Vollständiger fachlicher Vergleich mit den deutschen Originalen und dem getesteten Verhalten. Source-/CLI-/JSON-Beispiele bleiben ausführbar und inhaltlich gleich, soweit nicht R02 einen Fehler korrigiert. Überschriften zunächst nur mit `#`; ToC, Nummern und Anker erzeugt später mdtoc. Alle aktiven internen Verweise auf übersetzte Überschriften anpassen. Der in Fehlermeldungen genannte Suchbegriff `bind-limits` bleibt erhalten.

### Bisheriges User Manual als Reference Manual weiterführen

**R18 · Gewicht 4 · Aufwand M · Nach R10/R11; vollständige Inhalte erhalten**

`docs/TriceUserManual.md` in `docs/TriceReferenceManual.md` umbenennen und Titel, Selbstverweise sowie aktive eingehende Verweise entsprechend anpassen. Die Umbenennung selbst ist keine Kürzung: Verträge, Beispiele, Einschränkungen, Hintergrund und Anhänge bleiben vollständig. Der bisherige Pfad wird unter R19 für das neue kurze UM verwendet; alte Kapitelverweise müssen gezielt zum Reference Manual führen, statt unbemerkt im neuen UM zu landen.

Die Pfadänderung durchgängig berücksichtigen: Dokumentationspflege und mdtoc, Markdown-/Linkprüfungen, PDF-Erzeugung mit Dateiname und Kopfzeile, GitHub Pages, Release-Paketierung und Artefaktprüfungen sowie aktive Anleitungstexte und Agentenregeln. Konkrete vorhandene Verbraucher sind unter anderem `scripts/_310_refresh_trice_user_manual.sh`, `scripts/_320_generate_trice_user_manual_pdf.sh`, `scripts/_610_test_goreleaser_snapshot.sh`, `.goreleaser.yaml` und die Pages-/Installations-/Release-Workflows. Nur tatsächlich nötige Pfadanpassungen vornehmen, keine allgemeine Skriptumbenennung.

**Abnahme:** Vollständiger Inhaltsvergleich vor/nach der Umbenennung; Reference Manual als Markdown/PDF erzeugbar, Links und veröffentlichte Ziele stimmen. Suchhinweise wie `bind-limits` bleiben auffindbar. Archive bleiben unverändert; dadurch veraltete Archivverweise als verbleibende Folge dokumentieren, ohne neue Weiterleitungsdateien anzulegen. R19 ergänzt danach das neue UM samt derselben erforderlichen Dokumentationsprüfungen.

### Ein kurzes User Manual zum Ausprobieren erstellen

**R19 · Gewicht 4 · Aufwand M · Geführter Einstieg statt zweiter Vollreferenz**

Ein neues englisches `docs/TriceUserManual.md` erstellen, das deutlich kürzer als die bisherige Vollreferenz ist. Einstieg mit wenigen Sätzen zu Nutzen und Funktionsweise, anschließend Voraussetzungen, Installation gemäß R14 und ein ausführbares PC-Beispiel mit erwarteter Ausgabe. Danach die Schritte zur eigenen Target-Anbindung zeigen, mit einem klaren Standardweg und passenden Verweisen für alternative ID-Workflows und Transportwege.

Tags/Filter, Stempel, Structured Logging und Context Enrichment anhand kleiner Änderungen und sofort sichtbarer Ausgaben vorstellen. Vorhandene PC-/G0B1-Feature-Touren und ihre `show_*.sh`-Skripte verwenden; keine neuen Features oder Beispielprojekte allein für das neue Handbuch. Kurz erklären, welche Dateien dauerhaft zum Projekt gehören und welche generiert werden. Eine knappe Fehlerhilfe soll den nächsten sinnvollen Prüfschritt nennen.

Vollständige Optionslisten, Compiler-Matrizen, Protokolldetails und PoC-Begründungen bleiben im Reference Manual. Erforderliche Einschränkungen stehen dort im Einstieg, wo sie die konkrete Wahl beeinflussen, ohne den Beginn mit allen Sonderfällen zu überladen. Begriffe beim ersten Auftreten erklären; keine Vorkenntnis von internen Auftragsnummern oder Architekturentwürfen verlangen.

**Abnahme:** Vom sauberen Checkout bis zum ersten PC-Log ist der beschriebene Weg reproduzierbar; Voraussetzungen und erwartete Ausgabe sind sichtbar. Der Umfang ist deutlich reduziert, die zentralen Nutzeraufgaben sind auffindbar und Detailverweise funktionieren. Keine fachlichen Abweichungen zur Referenz. Beide Handbücher sind als Markdown/PDF prüfbar und für die Veröffentlichung unter ihren eindeutigen Namen vorbereitet.

### README, Repo-Orientierung, Beispiele und Zusagen verbessern

**R12 · Gewicht 4 · Aufwand M · Mit R17–R19 abgestimmt**

Die Root-[README](../../README.md) nennt Bind noch experimentell und unveröffentlicht; das muss zum gewählten Release-Status passen. Sie verweist bei Structured Logging noch auf einen auskommentierten Future-Draft. SL und CE gehören mit kurzen funktionierenden Beispielen und Links zu den beiden Feature-Touren in den Einstieg und die Feature-Übersicht.

Das README einladend und übersichtlich überarbeiten: ein kurzer Nutzenabschnitt, ein verständliches Code-/Ausgabebeispiel, ein klarer Einstieg über das neue User Manual und gezielte Links zur Vollreferenz. Lange Detaildiskussionen in das fachlich zuständige Handbuch überführen; weder Vollreferenz noch neues UM im README wiederholen. Installation aus Checkout beziehungsweise Binaries entsprechend der bestätigten Entscheidung erklären.

Eine kompakte Repo-Karte nach Nutzeraufgaben aufnehmen: Wo anfangen, Beispiele ausprobieren, Target-Code einbinden, Hosttool bauen, Tests ausführen und Details nachschlagen? Die wichtigsten Verzeichnisse erhalten je eine kurze Zweckbeschreibung und einen sinnvollen Einstiegspunkt. Basis ist die vollständige Bestandsprüfung aus R17; die öffentliche Karte zählt nicht jede Einzeldatei auf. `docs/README.md` erklärt die Rollen der beiden Handbücher und der übrigen relevanten Dokumente. Keine zusätzliche README pro Ordner allein aus formalen Gründen anlegen.

Weitere konkrete Ergänzungen im passenden Handbuch oder Beispiel erläutern und aus dem Einstieg gezielt verlinken:

- Ein kleines Verzeichnisbeispiel für `-genDir`: relativ zum Aufrufverzeichnis, Sidecars im Include-Pfad, persistente TIL/LI gegenüber generierten Dateien, `-logC` und ABC-Ausgaben.
- Erklären, warum beim G0B1-Beispiel auch gemeinsame Quellen Sidecars erzeugen können, obwohl ihre Demo-Funktion nicht aufgerufen wird: Scan-/Buildumfang und tatsächliche Laufzeitaufrufe sind verschiedene Dinge.
- Zum Aufräumen keine pauschale Löschung von `generated` empfehlen: Bei ABC kann dort eine vom Benutzer bearbeitete Auswahl-Headerdatei liegen. Alte Sidecars können außerdem historische ID-Evidenz liefern; Details siehe F01.
- Compiler-Zusagen getrennt für gewöhnliches Bind, direkte CE und experimentelles Rebase aufführen. C++20 mit strengen Warnungen scheitert laut vorhandenem PoC schon am Enum-Rebase; MSVC/IAR/armclang sind dort nicht nachgewiesen. Cross-Compile ist keine MCU-Laufzeitabnahme.
- Das Feature „keine dynamische Speicherverwaltung“ auf den Trice-Target-Loggingpfad beziehen. Stack-Puffer sind keine statischen Objekte; Hosttool, RTOS und benutzereigene CE-Funktionen sind nicht von derselben Zusage umfasst.
- ABC-Einstieg mit einer kurzen Karte von `NodeLib`, Auswahl-Header, generierter C-Tabelle und Buildausgabe erklären. Die heutige Dokumentation weiterverwenden; keine allgemeine Skript-Neuorganisation erforderlich.

**Abnahme:** Ein neuer Anwender erkennt Nutzen und ersten Schritt, findet seine Aufgabe in der Repo-Karte, versteht die Ablage und kann CE/SL ohne Kenntnis von A-/M-Aufträgen ausprobieren. README und kurzes UM laden ein, statt mit der vollständigen Options- und Sonderfallliste zu beginnen. Grenzen sind erreichbar und korrekt; Release-Zusagen decken sich mit nachgewiesenen Plattformen und Funktionen.

### Link-Forwarding-Dateien entfernen und docs konsolidieren

**R20 · Gewicht 4 · Aufwand S–M · Nach Festlegung und Befüllung der Zieldokumente**

Reine Weiterleitungsdateien aus dem aktiven `docs`-Bestand entfernen. Konkrete Beispiele sind `docs/TriceUserGuide.md`, `docs/TriceColor.md` und `docs/TriceIDManagement.md`; die vollständige Auswahl liefert R17. Nicht allein nach Dateinamen löschen: Eigenständige Informationen gegebenenfalls zuvor ins passende Handbuch oder andere begründete Zieldokument übernehmen. Der neue Dokumentationswegweiser `docs/README.md` bleibt wegen seines eigenen Orientierungszwecks erhalten.

Vor dem Entfernen alle aktiven eingehenden Links auf das fachlich passende Kapitel im kurzen UM oder im Reference Manual umstellen. Pfade, Anker, Bilder und Downloads im Repo, auf GitHub Pages und in PDFs prüfen. Keine neuen Markdown-Weiterleitungsstubs als Ersatz erzeugen. Nicht kontrollierbare externe Altlinks und unveränderte Archivverweise als verbleibende Folgen benennen; bestehende Archive dafür nicht bearbeiten. Doppelte aktive Dokumente zusammenführen, sobald ihre einzigartigen Inhalte und etwaige historischen Nachweise zugeordnet sind.

**Abnahme:** Keine reinen Link-Forwarding-Dateien mehr im aktiven `docs`-Bestand, keine aktiven Verweise auf entfernte Dateien und kein Verlust eigenständiger Informationen. Jeder verbleibende aktive Dokumentationsbestand hat eine nachvollziehbare Aufgabe; lokale Link-/Ankerprüfungen und die betroffenen Veröffentlichungswege bestehen.

### Übriges Repo anhand belegter Zwecke aufräumen

**R21 · Gewicht 3 · Aufwand M–L · Kleine zusammenhängende Gruppen nach R17/R20**

Die geprüften Aufräumgruppen aus R17 umsetzen. Überflüssige Dateien entfernen, unnötige Doppelbestände zusammenführen und nachweislich unpassend abgelegte Dateien nur dann verschieben, wenn dies die Orientierung verbessert. Root-Dateien, Beispiele, Experimente, Skripte, Konfiguration, Testdaten und Fremdquellen gehören zur Prüfung. Benötigte Lizenzen, reproduzierbare PoCs, Regressionstest-Fixtures und bewusst archivierte Historie besitzen eine Daseinsberechtigung, auch wenn Anwender sie nicht täglich öffnen.

Mit jeder Gruppe ihre aktiven Pfadabhängigkeiten, Build-/Test-/Release-Verwendung, Ignore-Regeln und die Repo-Karte nachziehen. Keine funktionalen Umbauten unter dem Etikett Aufräumen. Benutzerbearbeitete Dateien unter `generated` nicht pauschal löschen; lokale Artefakte und versionierte Produktdateien unterscheiden. Vorhandene `obsolete`- und andere ausdrücklich archivierte Bestände bleiben ohne gesonderten Auftrag unverändert. Bei ungeklärtem Zweck zunächst die konkrete Frage klären, statt versuchsweise zu löschen.

**Abnahme:** Jede verbleibende versionierte Datei und jedes verbleibende Repo-Verzeichnis hat einen dokumentierten Zweck in der Bestandszuordnung; entfernte oder verschobene Gruppen sind nachvollziehbar begründet. Die öffentliche Übersicht entspricht dem Ergebnis. Betroffene Beispiele, Builds, Tests und Paketierung bestehen; Testumfang, Lizenznachweise und reproduzierbare Entwicklungsnachweise sind erhalten. Die vollständige Abnahme des ausgewählten Aufräumumfangs folgt R16.

### Checkout-/Binary-Installationswege für v2 absichern

**R14 · Gewicht 5 · Aufwand S–M · Distributionsentscheidung getroffen; verbleibender Abgleich vor Release**

Entscheidung des Anwenders: **`/v2` vorerst weglassen.** `go.mod` bleibt bei `module github.com/rokath/trice`, Imports bleiben unverändert. Die Produktversion v2.0.0 wird über fertige Binaries und lokale Builds aus einem Repo-Checkout angeboten. Das [README](../../README.md#project-information) nennt dafür jetzt `./scripts/buildTriceTool.sh` statt `go install github.com/rokath/trice/cmd/trice@latest`, mit Aufrufort und Hinweis auf die ausgegebenen Installationspfade.

Lokales Bauen und Installieren aus dem Checkout benötigt allein wegen der Produktversion keinen Major-Modulpfad. Das vom Skript intern verwendete lokale `go install ./cmd/trice ./cmd/tlog` bleibt zulässig. Versionierte Go-Auflösung über `@v2.0.0` wäre dagegen an einen passenden Major-Modulpfad gebunden und gehört vorerst nicht zum zugesagten Installationsumfang. Siehe [Go: Major version suffixes](https://go.dev/ref/mod#major-version-suffixes).

Verbleibende Aufgabe: Aktive Installationsanleitungen und Release-Prüfungen auf den beschlossenen Umfang abstimmen. Weitere entfernte Go-Installationsanweisungen dürfen v2 nicht versprechen. Lokalen Build beider Tools und ausgelieferte Binaries prüfen; eine spätere Modulpfadumstellung benötigt eine neue Entscheidung.

**Abnahme:** Der lokale Build mit `./scripts/buildTriceTool.sh` und die angebotenen Binary-Installationswege funktionieren in einer sauberen Umgebung. Dokumentation und Prüfungen passen dazu; keine `/v2`-Umstellung und keine v2-Modulinstallationszusage. Kein Tag oder Publish im Rahmen dieses Auftrags.

### Release Notes und ausgelieferte Dateien prüfen

**R15 · Gewicht 5 · Aufwand M**

Der aktuelle [Changelog](../../CHANGELOG.md) endet bei v1.3.0. Einen verständlichen neuen Release-Abschnitt erstellen: Bind, SL, CE, Tags/Gewichte, Visualisierung, Generatoren, Ablage, Plattform-/Compilergrenzen und konkret nötige Anpassungen bestehender Projekte. Ein vollständiger Git-Log ersetzt diese Anwendersicht nicht.

Die [Installationsprüfungen](../../.github/workflows/install-checks.yml) und [Release-Prüfungen](../../.github/workflows/release-audit.yml) testen bereits Archive, Pakete, PDF und klassisches Decoding. Eine kleine neue Feature-Abnahme soll mit den **ausgepackten** Tools und Target-Quellen erfolgen: Bind bzw. Insert mit SL/CE, tatsächlich erzeugter Record, Text/JSON/KV und ausschließlich Anwendungsrecords auf dem maschinenlesbaren Kanal. Passende plattformunabhängige Fixture verwenden.

Die Toolchain-Angaben angleichen: `go.mod` verlangt Go 1.25.0; der separat manuell gestartete Workflow `go.yml` nennt noch 1.24.0. Das muss keinen unmittelbaren Buildfehler verursachen, weil Go einen Toolchain-Wechsel auslösen kann, macht die geprüfte Umgebung aber unnötig unklar.

Die unter R18/R19 getrennten Handbücher unter eindeutigen Namen ausliefern: `TriceUserManual` für den Einstieg und `TriceReferenceManual` für Details. Downloadlinks und Artefaktprüfungen müssen beide Dokumente dem richtigen Zweck zuordnen; die bisherige PDF-Prüfung nur umzubenennen reicht nicht.

**Abnahme:** Verständliche englische Release Notes und Anpassungshinweise; finales englisches User Manual und Reference Manual als Markdown/PDF; passende Target-Quellen; nachvollziehbare Artefaktprüfungen auf den zugesagten Betriebssystemen. Kein Publish oder Tag ohne ausdrücklichen Auftrag.

### Release-Abnahme auf einem feststehenden Stand

**R16 · Gewicht 5 · Aufwand M plus Full-Testlauf**

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
| P02 | Go-/CGO-Buildcache gezielt und nachweisbar invalidieren | 3 | M–L | R06, R13 |
| P03 | Erledigt: L432-Matrix von 27:06 auf 4:55 verkürzt, alle 101 Konfigurationen bestanden | 4 | L | R06, R13 |

### Cache nutzen, ohne veralteten C-Code zu testen

Der PC-Worker löscht vor jedem Workflow `go clean -cache -testcache`. Das ist wegen außerhalb der Go-Paketverzeichnisse eingebundener C-Quellen begründet. Die [Go-Dokumentation zum Buildcache](https://pkg.go.dev/cmd/go#hdr-Build_and_test_caching) weist auf Grenzen bei C-Abhängigkeiten hin; schlichtes Entfernen der Bereinigung wäre nicht ausreichend.

Prüfen, ob vollständig erfasste Inhalts-/Konfigurationssignaturen oder vorbereitete lokale Build-Eingaben die korrekte Invalidierung gezielter machen. Die bloße Trennung in zwei Cacheordner garantiert keine Aktualität. `-count=1` kann weiterhin die tatsächliche Testausführung erzwingen und unabhängig davon den Buildcache nutzen.

**Nachweis:** Änderungen an `triceCheck.c`, gemeinsam eingebundenen Headern, Sidecars, Workflow, Compileroptionen und Konfiguration erzwingen den passenden Neubau; kalte und warme Läufe liefern dieselben Ergebnisse. Keine Wiederverwendung bereits erfolgreich gemeldeter Testergebnisse als Ersatz für geforderte Ausführung.

### L432-Builds isolieren und begrenzen

**P03 · Umsetzung und gezielte Abnahme abgeschlossen**

[all_configs_build.sh](../../examples/L432_inst/all_configs_build.sh) baute bisher 0 bis 100 nacheinander mit `make clean` und vollständigem `build.sh` je Konfiguration. Der abgeschlossene Full-Lauf am 3. Oktober dauerte **48 Minuten 45 Sekunden**, davon die L432-Matrix **27 Minuten 6,416 Sekunden**. Alle Compiler-Matrizen bestanden. Die zwei Fehler in Go/Go-Coverage hatten dieselbe Umgebungsursache: Ein unpräfixiertes ARM-`nm` stand vor dem macOS-`nm` im PATH. Nach der lokalen PATH-Korrektur bestanden beide betroffenen Tests gezielt; der Full-Lauf vom 4. Oktober zeigte jedoch erneut die falsche Werkzeugauswahl. Die dauerhafte Korrektur der Tests steht unten. Die alten Testprotokolle bleiben unter `temp/before-l432-parallel-*` erhalten.

**Umsetzung:** Einmalige Bind-Vorbereitung und Umgebungsprüfung, danach parallele Konfigurationen mit jeweils `make -j1`. Das Standardbudget folgt der verfügbaren CPU-Anzahl beziehungsweise unter Windows dem begrenzten Budget der gemeinsamen Buildumgebung mit bevorzugter Erkennung physischer Kerne. Bei gescheiterter Erkennung gelten vier Jobs. `TRICE_L432_TEST_JOBS` erlaubt eine ausdrückliche Begrenzung. Jede Konfiguration erhält bei jedem Aufruf ein frisches, eigenes Buildverzeichnis unter `temp/log/l432.*`; alle bisherigen Quellen, Defines und Linkziele bleiben erhalten. Bestehende `out.gcc`-Artefakte bleiben unberührt. Erfolgreiche temporäre Objekte werden entfernt, vollständige Compilerprotokolle und fehlgeschlagene Teil-Builds bleiben zur Diagnose erhalten. Es gibt bewusst keinen neuen Objektcache und keine zusätzliche Abhängigkeit; auch Folgeaufrufe übersetzen vollständig neu.

**Fehlerverhalten:** Die Matrix respektiert die Stop-Policy des Test-Runners, zeigt Konfiguration, Fehlerauszug, Protokoll und Wiederholungsbefehl. Bereits gestartete Jobs werden abgeschlossen; bei Abbruch werden auch Compiler-Kindprozesse beendet, bevor die verwaltete Wiederherstellung beginnt. Abgeschlossene Konfigurationen erscheinen sofort im Schrittprotokoll.

**Zusätzlicher Engpass:** Parallelität allein reicht hier nicht: Die Assembler-Listings (`.lst`) verursachten unter paralleler Last erhebliche zusätzliche Laufzeit. Die Matrix schaltet deshalb ausschließlich diese Textausgabe mit `GCC_LISTINGS=0` ab. Normale Einzelbuilds behalten Listings als Standard; alle Optionen für Zielcode, Konfigurationsdefines, Warnungen und Linkziele bleiben erhalten. Konfiguration 0 benötigte im direkten Vergleich **13 Sekunden mit Listings und 6 Sekunden ohne**; ELF, HEX und BIN waren jeweils **bytegleich**.

**Verhaltenstests:** Die beschreibenden Tests in [l432_matrix_test.go](../../scripts/l432_matrix_test.go) prüfen alle 101 Konfigurationen, unveränderte Konfigurationsdefines und Buildziele, serielle und parallele Ausführung, automatische Budgeterkennung und ausdrückliche Limits, einmalige Vorbereitung, Warnungen, Fehlerfortsetzung, Abbruch nach dem gestarteten Batch, fehlende Binärdateien, ungültige Einstellungen, neue Buildpfade nach Headeränderung und das Beenden von Kindprozessen. Der echte Makefile-Auszug wird zusätzlich mit und ohne Listings ausgewertet: Andere Compileroptionen bleiben erhalten, Einzelbuilds erzeugen standardmäßig weiterhin Listings. `go test ./scripts -count=1`, ShellCheck, Shellformat, UM-Format und Markdownlint bestehen.

**Vollständige ARM-Abnahme am 3. Oktober:** Alle **101/101 Konfigurationen bestanden** mit ARM GCC 15.3.1, automatisch erkanntem Budget von zwölf Jobs und frischen Buildverzeichnissen. Laufzeit der Matrix **295 Sekunden (4:55)** gegenüber **1626,416 Sekunden (27:06)** im vorherigen Full-Lauf auf demselben Mac: rund **82 % weniger Zeit**, Faktor **5,5**. Das ist ein Vergleich dieser lokalen Läufe, keine plattformübergreifende Laufzeitzusage. Code-, Daten- und BSS-Größen stimmen für sämtliche Konfigurationen mit dem vorherigen Lauf überein. Erfolgreiche temporäre Buildverzeichnisse wurden entfernt; vollständige Protokolle liegen unter `temp/log/l432.hqSVXw`, die Firmware-Gegenprobe unter `temp/log/l432-listing-probe.o2ty4t` und `temp/log/l432_listing_probe.log`. Die verwaltete Wiederherstellung bestätigte den exakten Ausgangszustand nach Erfolg und nach den zuvor kontrolliert abgebrochenen Probeläufen. Folgeaufrufe nutzen ebenfalls frische Objekte; es gibt keinen warmen Objektcache, dessen Ergebnis die Testausführung ersetzen könnte. Reale Windows-/Linux-Abnahmen wurden für diese Änderung nicht ausgeführt; die abschließende plattformübergreifende Release-Abnahme bleibt R16.

**Full-Nachprüfung am 4. Oktober:** Der Benutzerlauf `./scripts/testAll.sh full` benötigte **1580 Sekunden (26:20)**, rund **46 % weniger** als der zuvor dokumentierte Full-Lauf. **23 Schritte bestanden**, nur Go und Go-Coverage scheiterten erneut an den beiden `nm`-Prüfungen. Die L432-Matrix bestand mit **101/101 Konfigurationen in 270 Sekunden (4:30)**; auch die übrigen Compiler-Matrizen, der Release-Snapshot und beide PC-Workflows bestanden. Die vorhandenen Fehlerprotokolle bleiben als Diagnosebeleg erhalten.

**Dauerhafte Korrektur der Symbolprüfung:** Die Tests in [local_log_integration_test.go](../../internal/id/local_log_integration_test.go) lesen ELF-, Mach-O- und COFF-Objekte jetzt mit der Go-Standardbibliothek. Compiler und Symbolleser werden nicht mehr unabhängig voneinander aus dem PATH ausgewählt. Gegenproben prüfen vorhandene Funktions-, globale, lokale und undefinierte Symbole, gültige leere Objekte, fehlende beziehungsweise beschädigte Dateien und ein absichtlich unbrauchbares `nm` an erster Stelle im PATH. Auch mit dem tatsächlich problematischen ARM-`nm` vorne im PATH bestehen beide ursprünglichen Compile-out-Tests. Die Objektformat-Gegenproben liefen auf macOS mit Clang-Cross-Compilation; sie ersetzen keine echte Windows-/Linux-Abnahme.

**Abnahme der Korrektur:** `go test ./... -count=1` und der vollständige Go-Coverage-Lauf mit `-count=1 -covermode=atomic -coverpkg=./...` bestehen. Das separate Profil `temp/log/coverage-nm-fix.out` bewahrt die bisherigen Full-Protokolle. Nach dieser reinen Testkorrektur wurde kein weiterer `testAll full` gestartet; die erfolgreichen übrigen Schritte des Benutzerlaufs wurden nicht wiederholt.

## Sinnvolle Erweiterungen zur späteren Diskussion

Diese Punkte sind Vorschläge, keine offenen Versprechen für das nächste Release. Bestehende Tests, PoCs und frühere Entscheidungen bleiben maßgeblich. Vor einer Issue-Erstellung Nutzen, Scope und vorhandene Issues abgleichen; externe Issue-Texte später englisch formulieren.

| ID | Idee | Gewicht | Aufwand | Empfehlung |
| --- | --- | ---: | --- | --- |
| F01 | Verständliche Bestandsprüfung generierter Artefakte | 3 | M | Zuerst rein lesende Diagnose diskutieren |
| F02 | Fuzz- und Race-Prüfungen für neue Parser und Recordpfade | 3 | M | Robustheit vor weiteren Ausgabeformaten |
| F03 | Frühe Hostfilterung / Template-Caching | 2 | M–L | Alte A11/M17-Aufgabe; zuerst messen |
| F04 | Benannte SL-Felder für Visualisierung oder Schemaübersicht | 2 | M | Mit einem konkreten Anwenderfall beginnen |
| F05 | CE für eindeutig zuordenbare Ein-Logstellen-Wrapper | 2 | M | Kleiner separater Ausbau auf Basis des PoC |
| F06 | Allgemeines CE für Bind-Wrapper und Counter-Rebase | 1 | L | Weiter zurückstellen |

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

### Test-Ausgangszustand einschließlich Standalone-Beispielen erhalten

**R13 · Gewicht 5 · Aufwand M · Umsetzung abgeschlossen; abschließende Full-Matrix bei R16**

Die vier lokalen Tabellen in `examples/PC_log` und `examples/G0B1_log` waren nach zusätzlichen SL-/CE-Quellen einmalig aktualisiert worden. Vorhandene TIL-Einträge behielten ihre IDs; in den LI-Tabellen verschoben sich Zeilennummern. Diese inhaltliche Aktualisierung ist bereits im aktuellen Commit-Stand enthalten. Bei unveränderten Eingaben überspringt Bind identische Schreibvorgänge; der vorhandene `TestBindDoesNotReplaceUnchangedFiles` prüft das ausdrücklich.

Die Standalone-Builds aus Schritt 600 laufen weiterhin im Checkout. Der verwaltete Snapshot umfasst jetzt auch die lokalen Beispielquellen, TIL/LI-Dateien, generierten Verzeichnisse und Buildausgaben einschließlich ursprünglich fehlender Pfade und vorhandener Benutzerauswahl. Schritt 490 bereitet Bind innerhalb desselben Zustandsvertrags vor. Am Ende von `testAll` vergleicht eine zusätzliche Prüfung die wirklichen Bytes aller versionierten Dateien mit dem Ausgangszustand und nennt veränderte Pfade; ein bereits zuvor schmutziger Git-Status gilt nicht als Gleichheitsnachweis. Es gibt keine pauschale Git-Rücksetzung.

**Gezielte Abnahme am 30. September:** Die zwei Standalone-Builds pro `PC_log` und `G0B1_log` ließen alle vier Tabellen bytegleich und behielten beim zweiten Lauf deren Inodes; der äußere Snapshot stellte den Checkout anschließend wieder her. Isolierte Verhaltenstests prüfen schmutzige und ursprünglich fehlende lokale Dateien, generierte Benutzerauswahl und Buildausgaben nach Erfolg, Worker-Fehler und Signal, außerdem die transaktionale Vorbereitung in Schritt 490 bei Erfolg und Fehler. Ein Runner-Test beweist, dass eine erneute Änderung einer schon vorher schmutzigen Datei trotz unverändertem Git-Kurzstatus erkannt und mit Pfad gemeldet wird. `testAll quick` besteht mit 19/19 Schritten einschließlich der realen Schritte 490, 600 und der Byte-Prüfung. Die vollständige Release-Matrix bleibt bei R16.

### Kein automatisch erzeugtes untagged-Präfix ausgeben

**R01 · Gewicht 5 · Aufwand M · Umsetzung abgeschlossen; abschließende Full-Matrix bei R16**

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

Im [UM](../TriceUserManual.md) ist die alternative CE-Regel für Position und Geschwindigkeit syntaktisch gültig. Die ABC-Dateinamen und ihr Standardort `generated/` entsprechen der CLI; ein expliziter Zielpfad bleibt möglich. Das nicht vorhandene `-tilCS`-Beispiel wurde durch eine zutreffende C#-Integrationsnotiz ersetzt. Das Testkapitel nennt die heutigen Skripte und `_test`; die Release-Kommandos sind einzeln kopierbar. Der Link im [PC-Feature-Beispiel](../../examples/PC_features/README.md) verwendet die versionierte Schreibweise `ReadMe.md`.

**Gezielte Abnahme:** Isolierte Bind- und ABC-Tests prüfen die CE-Regel sowie generierte Namen und Pfade. Die CLI weist `-tilCS` als unbekannten Schalter ab. UM-Format und Markdownlint bestehen; die lokale Linkprüfung für UM und PC-README findet keine Fehler. Die netzabhängige vollständige Linkprüfung war in dieser Umgebung wegen blockierter Verbindungen zu externen Websites nicht abschließbar. Release-/Git-Kommandos wurden nicht ausgeführt.

### Fehlerstatus bei fehlgeschlagenem Clean erhalten

**R03 · Gewicht 5 · Aufwand S · Umsetzung abgeschlossen**

Die vier betroffenen Buildskripte speichern den echten Status von `trice clean`, bevor sie ihn auswerten. Ein allein fehlgeschlagenes Clean führt zum Fehlerstatus und meldet dessen Exitcode; ein bereits fehlgeschlagener Build oder eine Unterbrechung behalten ihren ursprünglichen Status. Das vereinfachte Cleanup-Beispiel im UM reicht Clean-Fehler ebenfalls weiter, ohne im normalen Abschluss erneut zu bereinigen.

**Gezielte Abnahme:** Isolierte Verhaltenstests führen alle vier Skripte mit erfolgreichem Build und Clean, Clean-Fehlercode 23, vorigem Build-Fehlercode 17 sowie SIGINT und SIGTERM aus. Pro Lauf werden Vor- und Nach-Clean genau einmal aufgerufen; Status und Warnung stimmen in allen Fällen. Shell-Formatprüfung, ShellCheck und die vollständige `scripts`-Testsuite bestehen.

### Automatisches Nachladen wiederhergestellt

**R04 · Gewicht 5 · Aufwand M · Umsetzung abgeschlossen; abschließende Full-Matrix bei R16**

Entscheidung: Automatisches Nachladen bleibt ein wichtiges zugesagtes Feature. Der Logger überwacht die Verzeichnisse der geladenen TIL-/LI-Dateien und verarbeitet damit auch atomaren Dateiersatz durch Bind/Insert. Rasche Folgespeicherungen gehen nicht mehr in einer fünfsekündigen Sperre verloren. Vollständig eingelesene Tabellen werden unter gemeinsamem Schreib-/Leseschutz ersetzt; Decoder, Positionsausgabe und Statistik verwenden denselben Schutz. Fehlerhafte, leere oder vorübergehend fehlende Dateien lassen den letzten gültigen Stand unverändert und werden erneut eingelesen. Diagnoseausgaben gehen auf stderr; gleiche wiederholte Lesefehler erzeugen keinen Warnungsstrom. Beim Verlassen des Loglaufs werden Watcher und Timer beendet und ihr Abschluss abgewartet.

Das [UM unter Easy-to-use](../TriceUserManual.md#easy-to-use) erklärt Bedienung und Grenzen: TIL und LI sind keine dateiübergreifende Transaktion, `{}` leert die jeweilige Tabelle absichtlich, eine beim Start fehlende LI-Datei bleibt für diesen Loglauf deaktiviert, und bereits deaktivierte Visualisierungsregeln werden nicht automatisch wieder aktiviert. Allgemeine Signal-/Receiver-Lebenszyklen bleiben Gegenstand von R08.

**Gezielte Abnahme:** [Watcher-Tests](../../internal/id/fileWatcher_test.go) prüfen reale Schreib-/Ersetzungsereignisse, Wiederanlage, ungültiges JSON ohne Teilübernahme, Wiederholung ohne Folgeereignis, stille Fehlerwiederholungen, abgeschaltete Pfade, Backendfehler und Ressourcenfreigabe. [CLI-Integrationstests](../../internal/args/fileWatcher_test.go) betreiben jeweils einen laufenden Logger für Text, JSON und KV mit echter Dateieingabe: geänderte Feldschemata und Positionen, mehrfacher Dateiersatz, Weiterloggen bei defektem JSON, Erholung und fortlaufende Visualisierung. Diese Tests bestehen auch mit Race Detector; die betroffenen Go-Paketsuites bestehen. Die lange Full-Matrix wurde nicht erneut gestartet.

### Kompatibilitätsvertrag und Release-Ziel festgelegt

**R05 · Gewicht 5 · Aufwand S–M · Abgeschlossen; Release-Ziel v2.0.0 bestätigt**

Entscheidung des Anwenders: **v2.0.0 ist das verbindliche Release-Ziel**. Die nachgewiesenen absichtlichen Änderungen an veröffentlichten CLI- und Template-Schnittstellen sind inkompatibel; nach [Semantic Versioning](https://semver.org/spec/v2.0.0.html) ist dafür ein Major-Schritt vorgesehen. Die Zahl der Brüche ist unerheblich. Ein v1.4.0 mit unveränderter Rückwärtskompatibilitätszusage beschreibt diesen Stand nicht zutreffend. Ein Tag oder eine Veröffentlichung sind damit nicht beauftragt.

Nachweis: Der lokale Release-Tag `v1.3.0` zeigt auf `54ce845b069f989bfc762f28f6dd364954e6050f`. Sein unveränderter Quellstand wurde mit `git archive` in ein temporäres Verzeichnis extrahiert und dort mit lokal vorhandenen Abhängigkeiten gebaut. Das aktuelle Tool wurde aus Stand `768844f1` separat in die temporäre Ablage gebaut. Beide CLI-Binaries wurden mit identischen TIL-/TREX-Fixtures ausgeführt. Es wurden weder historische Dateien im Repo geändert noch alte Quellstände in den Worktree zurückgesetzt.

| Vertrag | Beobachtete Änderung / zu dokumentierende Folge |
| --- | --- |
| CLI für C-Generierung | v1.3.0 erzeugt mit `generate -tilC` eine `til.c`; der aktuelle Host weist den Schalter mit Exitcode 2 ab. `-logC` hat einen auf aktuelle Source-Stellen bezogenen Vertrag. |
| CLI für Location-Daten | v1.3.0 akzeptiert `-liPath base`; der aktuelle Host weist es mit Exitcode 2 ab. `-liRoot` und `-liMaxDirs` trennen jetzt Speicherung und Darstellung. **Beide veröffentlichten Vergleichsschemata verwenden `File` und `Line`; `Path` war ein unveröffentlichter Zwischenstand.** |
| User-Tags | v1.3.0 akzeptiert `-ulabel alpha:beta` als zwei Tags. Der aktuelle Host weist `beta` als unbekannte Farbe ab. Je Tag eine Option verwenden; Doppelpunkt für Gewicht/Farbe. |
| Formatstrings | Der alte Host gibt `literal={x}` und `set={1,2}` wörtlich aus. Der aktuelle Host erwartet bei `{x}` einen Wert beziehungsweise weist `{1,2}` als ungültigen Feldnamen ab. `{{x}}` erscheint im alten Host doppelt geklammert und im aktuellen Host als `{x}`. Ein neues `{x}` mit einem 32-Bit-Wert wird nur vom aktuellen Host als strukturiertes Feld dekodiert. |
| Klassische Meldungen | Derselbe 32-Bit-Record mit `msg:count=%d` ergibt in beiden Hosts `count=7`; `hi` bleibt `hi`. R01 ist umgesetzt: Automatische Klassifizierung als `untagged` fügt kein Präfix in den Meldungstext ein. |
| Tag-Auswahl und Darstellung | Eindeutige Aliase und gewichtete Schwellen gelten pro Anwendungsereignis; Metadaten werden separat behandelt. Unbekanntes `-logLevel` oder `-pick` wird jetzt vor dem Öffnen der Eingabe abgewiesen; v1.3.0 akzeptiert dieselben geprüften Werte. |
| Generierte Ablage | `generate -abc deviceX` erzeugt unter v1.3.0 `deviceX.h/.c` im Aufrufverzeichnis, aktuell unter `generated/`. `-genDir` ist der gemeinsame Verzeichnisschalter. |

Wichtig: `-buildDir`/`-bindDir` und das zeitweilige LI-Feld `Path` waren Zwischenstände der neuen Arbeit, keine zusätzlichen Brüche gegenüber v1.3.0. Release Notes müssen veröffentlichte Änderungen von unveröffentlichten Umbenennungen unterscheiden.

Der unterstützte Vertrag steht jetzt im [UM-Kapitel zur Firmware-/Host-Kompatibilität](../TriceUserManual.md#compatibility-with-firmware-and-host-tool-versions), mit einer Kombinationstabelle und konkreten Vorher-/Nachher-Ausgaben. README und CLI-Hilfe verweisen auf beziehungsweise nennen die Template-Grenze. Historische Firmware, zugehörige TIL/LI, Host-Version und Decodieroptionen zusammen archivieren. Alte Firmware mit literalen Klammern bleibt mit ihrem passenden alten Host reproduzierbar; neue Quellen nutzen doppelte literale Klammern und werden neu instrumentiert und gebaut. Ein kompatibles Recordlayout allein ist keine pauschale Zusage für beliebig gemischte Target-Quellen, Wörterbücher und Hosts. Ein Decoderdiagnose-Record führt zudem nicht zwingend zu einem fehlerhaften Prozess-Exitcode; Abnahme muss Meldungen und Diagnosen prüfen.

**Kein Migrationsprogramm und kein erneutes `-migrationBraces`.** Die früher verworfene Migration bleibt ausgeschlossen. Es geht um eine ehrliche Kompatibilitätsbeschreibung und gezielte Vergleichstests, nicht um still eingeführte Kompatibilitätsmechanismen.

**Gezielte Abnahme:** Der neue Test `TestReleaseCompatibilityWithV130Dictionaries` in [structured_test.go](../../internal/args/structured_test.go) sichert klassische Meldungen, ungetaggten Text, historische Klammerfehler, aktuelle Escape-Schreibweise und benannte Felder mit echten TREX-Bytes ab; die TIL bleibt dabei bytegleich. Vorhandene CLI-, Generator-, Tag- und Template-Tests decken die übrigen aktuellen Verträge ab. Der direkte Zwei-Binary-Vergleich bestätigt die dokumentierten alten Ausgaben und CLI-Unterschiede; die Standardtests benötigen weder Git-Historie noch eine installierte alte Trice-Version.

**Folgearbeiten:** Die Distributionsentscheidung ist getroffen: vorerst kein `/v2`, Installation über Binaries oder Repo-Checkout mit `./scripts/buildTriceTool.sh`. R14 sichert die dazugehörigen Anleitungen und Prüfungen ab. Go-Tests für `cmd`, `internal` und `pkg`, UM-Format, Markdownlint und lokale Linkprüfung bestanden. Die ausführlichen Release Notes bleiben R15, die vollständige Plattform-/Target-Abnahme R16.

### Testbeschleunigung mit vollständigen PC-Matrizen geprüft

**R06 · Gewicht 4 · Aufwand M · Umsetzung und vollständige PC-Gegenproben abgeschlossen**

Der Auftrag umfasst R08/R09 und P01/P04. Die wiederholte 100-ms-Wartezeit bei endlichen Eingaben entfällt, Logaufrufe geben ihre Ressourcen frei, geeignete Konfigurationen decodieren gesammelt und der PC-Worker führt höchstens vier Konfigurationen gleichzeitig aus. Die ursprünglichen Erwartungen und die zusätzlichen Tests jedes Pakets bleiben enthalten. Eine Infrastruktur zur Zeitmessung jedes Einzeltests wurde nicht eingeführt.

**Wiederherstellung nach Rechnerwechsel:** Der neue Checkout und der Server standen auf `c4bf94267d74e6e2dc07d03085a35c1425ee4ddb`; die Implementierungsänderungen waren darin noch nicht enthalten. Aus dem übertragenen Codex-Verlauf wurden 25 erfolgreiche Patch-Vorgänge für 19 Dateien einschließlich drei neuer Testdateien chronologisch rekonstruiert. Drei protokollierte fehlgeschlagene Patch-Versuche wurden ausgelassen. Die anschließende Formatierung und Prüfung erfolgten auf dem neuen Mac. Nicht protokollierte manuelle Änderungen und alte vollständige Logdateien lassen sich damit nicht beweisen oder wiederherstellen. Der [Leitfaden zum Rechnerwechsel](../Codex_Rechnerwechsel_DE.md) beschreibt die künftig nötige Sicherung.

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

### Ressourcen und Signalbehandlung pro Loglauf abgeschlossen

**R08 · Gewicht 4 · Aufwand M · Umsetzung abgeschlossen**

[Translate](../../internal/translator/translator.go) meldet seine Signalbehandlung beim normalen Abschluss wieder ab und wartet auf das Ende der zuständigen Goroutine. Der nutzlose periodische Ticker entfällt; die bestehende kurze Schonfrist nach SIGINT/SIGTERM bleibt erhalten und lässt sich beim normalen Abschluss abbrechen. Statistik, Diagnosen und Signal-Exitcode bleiben erhalten.

`binaryLogger.Close` und `bytesViewer.Close` reichen das Schließen an ihre besessenen Ressourcen weiter. Eingabe und Binärlogdatei werden genau einmal geschlossen, auch wenn eine Schließoperation fehlschlägt; der geliehene Diagnose-Writer bleibt offen. Der CLI-Loglauf schließt die vollständige Wrapperkette sofort nach `Translate`, bevor ein weiterer Loglauf beginnen könnte. Schließfehler gehen an den Aufrufer zurück.

**Abnahme:** [Lifecycle-Tests des Translators](../../internal/translator/lifecycle_test.go) starten und beenden die Signalbehandlung wiederholt und prüfen den Abschluss während der Schonfrist. Die Signal-Prozessprüfung in [translator_delta_test.go](../../internal/translator/translator_delta_test.go) prüft SIGINT und SIGTERM mit Bereitschaftssignal statt Warteannahme, genau einen Input-Close und erfolgreichen Exit. [CLI-Lifecycle-Tests](../../internal/args/lifecycle_test.go) prüfen echte Eingabe-/Binärdateien unter beiden Wrappers bei EOF, Lesefehler, Schreibfehler und Schließfehler; [Receiver-Tests](../../internal/receiver/receiver_test.go) prüfen Besitz und wiederholtes Close. Die gezielten Lifecycle-Tests bestehen auch mit Race Detector.

### Endliche Eingaben ohne pauschale Wartezeit abgeschlossen

**R09 · Gewicht 4 · Aufwand M · Umsetzung abgeschlossen**

Die bisherige 100-ms-Mindestlaufzeit pro endlichem Logaufruf ist entfernt. Der TREX-Decoder gibt EOF erst zurück, wenn nach dem tatsächlichen Eingabeende keine gepufferten vollständigen Records mehr verarbeitet werden können. Ein leerer Record ist Fortschritt und beendet die Ausgabe nicht vorzeitig. Endliche Quellen schließen dann sofort ab; ein begonnenes letztes Textfragment wird weiterhin ausgegeben. Normales EOF erzeugt keine zusätzliche Diagnose im Text-/JSON-/KV-Ausgabekanal.

Ungeframte, fragmentiert gelesene Header und Nutzdaten bleiben bis zur Vervollständigung erhalten. Ein unmöglicher Längenwert eines bekannten festen Recordtyps beziehungsweise eine unbekannte ID geht weiterhin durch die Resynchronisierung. `FILE` bleibt eine Live-Quelle; `TCP4BUFFER` endet erst bei echtem Peer-EOF, nicht bei einem kurzzeitig leeren Read. Die bestehende Pause gegen beschäftigtes Warten bei inaktiven Live-Quellen bleibt erhalten.

Der historische Befund erklärt den großen Gewinn: Je Workflow liefen 34 gewöhnliche Konfigurationen mit etwa 183–185 Sekunden und 20 Direct-/Deferred-Kombinationen mit etwa 365 Sekunden, jeweils mit 1.745 Erwartungen pro Kanal. Allein 100 ms für `2 × (34 + 2 × 20) × 1.745` Logaufrufe ergeben rechnerisch **7 Stunden 10 Minuten 26 Sekunden**. Go/C-Übergänge waren damit nicht als Hauptursache nachgewiesen; auch der neue Bulk-Weg führt weiterhin jede C-Teststelle aus.

**Abnahme:** [Replay-Tests](../../internal/translator/lifecycle_test.go) verwenden echte TREX-Bytes: Daten und EOF im selben Read, mehrere gepufferte Records einschließlich leerer Meldung, Byte-für-Byte-Eingabe, verkürztes Endpaket, 16-/32-Bit-Stempel, doppelte 16-Bit-ID, langen Laufzeitstring, Abschlussfragment und Live-Pausen. Begrenzte Read-Zahlen weisen nach, dass endliche Eingaben nicht weiter gepollt werden. Die bestehenden Decoder-, Translator- und CLI-Suites prüfen zusätzlich Filter, Stempel, strukturierte Ausgabe und Ausgabefehler. Der Zeitvergleich steht bei R06.

### Bulk als regulärer PC-Testweg mit verwertbarer Fehlerdiagnose

**P04 · Gewicht 4 · Aufwand M–L · Umsetzung abgeschlossen**

`TRICE_PC_TEST_MODE=auto` wählt im [gemeinsamen Harness](../../_test/testdata/cgoPackage.go) den nachgewiesenen Weg je Konfiguration. Von 63 Paketen verwenden 30 Bulk, 24 weiterhin den Einzelweg und neun ihre speziellen Tests. Framed Direct und Deferred werden getrennt gesammelt und decodiert. Der bisherige Transfer nach jeder C-Teststelle bleibt dort erhalten, wo kleine Puffer ihn benötigen; die ursprünglichen acht Deferred-Bulk-Fälle behalten ihre Mehrstellen-Transfers zur Prüfung des Pufferns. Ungeframte Kanäle bleiben einzeln, damit Padding und Paketgrenzen nicht durch Verkettung verändert werden.

Alle 1.745 Erwartungen pro gewöhnlichem Kanal und sämtliche weiteren Tests in jedem Paket bleiben aktiv. Erfolgreiche Bulk-Konfigurationen werden nicht nochmals vollständig einzeln ausgeführt. `TRICE_PC_TEST_MODE=line-by-line` bleibt als explizite Gegenprobe verfügbar. Ein Overlay verwendet die zentralen Harness-Vorlagen, ohne 61 generierte Kopien umzuschreiben.

Der erste Bulk-Unterschied nennt `triceCheck.c:<Zeile>`, Erwartungsindex, Kanal, Byteposition, Soll/Ist mit sichtbaren Steuerzeichen und begrenzten Kontext. Mehrzeilige und leere Erwartungen behalten ihre Grenzen. Die genannte Zeile ist die erste abweichende Erwartung, nicht zwingend die Ursache einer früheren Datenstrombeschädigung. Original-Binärstrom und kompletter Text bleiben im konfigurationsbezogenen Logverzeichnis erhalten. Eine automatische Einzelgegenprobe nach einem Bulk-Fehler kann dessen Gesamtexit nicht wieder auf PASS setzen.

**Abnahme:** Deskriptive Harness-Tests prüfen richtigen, veränderten, fehlenden und zusätzlichen Text sowie leere und mehrzeilige Erwartungen. Der [C-gestützte Fehlerproben-Test](../../_test/ringB_de_multi_cobs_ua/cgo_test.go) beweist den ersten Abbruch für Einzel-, Bulk- und kombinierten Weg und prüft erhaltene Binär-/Textartefakte. Die vollständigen Matrixvergleiche stehen bei R06.

### PC-Konfigurationen begrenzt parallel geprüft

**P01 · Gewicht 4 · Aufwand M · Umsetzung abgeschlossen**

Der [PC-Worker](../../scripts/_160_pc_target_test_worker.sh) startet standardmäßig höchstens vier Konfigurationen als getrennte Prozesse. `TRICE_PC_TEST_JOBS` erlaubt eine andere positive Grenze, einschließlich `1` für seriellen Betrieb. Globale Go-/C-Zustände werden nicht mit `t.Parallel` geteilt. Jede Konfiguration besitzt ein eigenes `output.log`; pro Lauf entsteht ein neues Verzeichnis unter `temp/log/pc-<workflow>.<Lauf>/`.

ID-Vorbereitung und Wiederherstellung bleiben außerhalb der parallelen Phase. Ohne `--no-stop` wird nach dem ersten erkannten Fehler nur die bereits gestartete Gruppe beendet und keine weitere Gruppe begonnen. Mit `--no-stop` laufen die übrigen Konfigurationen weiter, der Gesamtexit bleibt fehlerhaft. Ein Abbruch erreicht auch Compiler-/Test-Kindprozesse und automatische Diagnose-Nachläufe; der Worker wartet vor der Source-Wiederherstellung auf deren Ende.

Fehlerberichte nennen Workflow, Konfiguration, Logpfad, relevante Diagnose und einen Reproduktionsaufruf. Auch der stille äußere `testAll`-Runner zeigt konkrete Fehlerausschnitte statt nur FAIL. Der Reproduktionsaufruf setzt denselben vorbereiteten ID-Zustand und die passenden Compiler-Include-Pfade voraus; der verwaltete Workflow stellt diese weiterhin bereit. Die groben Fortschrittsgewichte berücksichtigen den verkleinerten Anteil der PC-Matrizen.

**Abnahme:** [Isolierte Worker-Verhaltenstests](../../scripts/pc_target_worker_test.go) prüfen seriellen Erfolg, tatsächliche parallele Überlappung, Jobgrenze, getrennte Logs, Fail-fast, `--no-stop`, Fehlererhalt trotz erfolgreicher Gegenprobe, ungültige Steuerwerte und Abbruch einschließlich verzögert beendeter Kindprozesse im normalen und diagnostischen Lauf. [Runner-Tests](../../scripts/portability_test.go) prüfen die konkreten Fehlerdetails auch bei stiller Ausführung. Die vollständige `scripts`-Suite besteht. Die beschriebenen Signal-Prozessprüfungen laufen unter POSIX; eine reale Windows-Matrix bleibt Teil von R16.
