
# Vorschlag zur Straffung des Reference Manual

Stand: 16. Oktober 2026, Branch `wip`, Ausgangscommit `9583c90386a4abd11120153077a31be2d232bcdb`.

**Nur Analyse und Vorschlag.** Für diesen Auftrag entsteht ausschließlich dieses Dokument. RM, UM, Guide, Kapitelreihenfolge, Links, Skripte und Code werden nicht verändert. Auf dem anderen PC laufen weitere Arbeiten; technische Aussagen vor einer späteren Umsetzung erneut mit dem dann aktuellen Stand vergleichen.

<!-- mdtoc -->
<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

## Empfehlung

Nach dem Abgleich mit Root-README, Guide, UM und den Notes: Die **52 bisherigen Inhaltsverzeichnis-Kapitel auf 34 reduzieren**, mit dieser Lesereihenfolge. Die zusätzliche Fachdatei GitHub Statistics wird im Projektpflege-Anhang berücksichtigt. Die ersten Schritte gehören ins UM. Gegenüber dem vorherigen Vorschlag entstehen drei zusätzliche Fachkapitel für VIS, Target-Routing und Generate; die sauber getrennten Aufgaben sind hilfreicher als das Festhalten an 31 Kapiteln.

```text
Reference Manual
├── Normale Nutzung: 14 Kapitel
│   └── Konzepte + Verweis aufs UM → Integrationsrezepte → Bind → Referenz
├── Advanced Features: 9 Kapitel
│   └── Filter → VIS → SL → CE → Routing → ABC → Aliases → Generate → typeX0
└── Anhang: 11 Kapitel
    └── Protokoll, Implementierung, Spezialintegration, Entwicklung und Legacy
```

Das sind 34 tatsächlich verbleibende Fachkapitel, nicht bloß 34 sichtbare Links bei unverändertem Dateibestand. `HEADER.md`, `FOOTER.md` und `README.md` sind dabei nicht mitgezählt. Die Gruppen sind eine Leserführung; dafür werden keine zusätzlichen Unterverzeichnisse benötigt. Das neue aktive RM entsteht unter `docs/rm/`; der bisherige Ordner wird erst nach gesondertem GO unverändert nach `docs/_scratchPad/rm1/` verschoben.

**Bind wird der normale Weg.** `insert`/`clean` und ihr `-cache` erhalten die Kennzeichnung **Deprecated** und einen gemeinsamen hinteren Bereich. Sie verschwinden nicht aus der Dokumentation. typeX0 bleibt als eigenes Kapitel weitgehend inhaltlich erhalten und steht hinten bei Advanced. Nach der aktualisierten Vorgabe sind gezielte Anpassungen erlaubt; die frühere Forderung nach einer bytegleichen neuen typeX0-Datei entfällt. Das gesamte alte RM in `rm1` bleibt dagegen unverändert.

## Aufgabenverteilung zwischen README, Guide, UM und RM

Der Abgleich berücksichtigt [Root-README](../../README.md), [Guide](../TriceGuide.md) und [UM](../TriceUserManual.md). Diese drei Dateien sollen auch bei der späteren Umsetzung **inhaltlich und strukturell weitgehend unverändert bleiben**. Die Notes erlauben jedoch aufgabenbezogene Anpassungen, wenn sie die neue Aufteilung verständlicher machen oder Wiederholungen vermeiden. Das ist keine Beschränkung auf Linkänderungen, aber auch kein Anlass für eine allgemeine Neufassung. Der vollständige erste Lernweg bleibt ausdrücklich im UM. Jetzt wird weiterhin nur dieses Planungsdokument bearbeitet.

| Dokument | Aufgabe | Konsequenz für das neue RM |
| --- | --- | --- |
| Root-README | Kurzer Projekteinstieg: Nutzen, Demo, belegte Aussagen und Link zum Guide. | Keine zweite Werbeeinleitung oder vollständige Feature-Liste. Die technischen Belege hinter den Fußnoten bleiben gezielt erreichbar. |
| Guide | Wegweiser nach Nutzeraufgabe, Dokumentübersicht und kurzer Überblick über das Repository. | Kein zweiter dokumentübergreifender Wegweiser. RM-README ordnet nur die RM-Fachkapitel und verweist für Orientierung auf den Guide. |
| UM | **Die ersten Schritte:** Voraussetzungen, erster PC-Log, kleine Experimente, erste Firmware-Einbindung, Dateien aufbewahren und erste Fehlerhilfe. | Kein paralleler Anfängerkurs. RM-Einstieg verweist direkt auf die passenden UM-Schritte. Das UM bleibt für den normalen Lernweg ausreichend verständlich, ohne dauernd zwischen Dateien springen zu müssen. |
| RM | Vollständige technische Referenz: Regeln, Optionen, Grenzen, vertiefende Integrationsrezepte, Spezialfälle und Nachweise. | Referenzdetails einmal vollständig erklären. Für eine geführte erste Anwendung zurück ins UM verweisen; kurze Beispiele zur Erklärung einer Regel bleiben sinnvoll. |

**Kreuzverweise ersetzen doppelte Anleitungen, nicht die unmittelbar nötige Erklärung.** Das UM darf beispielsweise einen Bind-Aufruf mit Include-Reihenfolge zeigen; das RM erklärt dazu die vollständigen Build-Regeln und Sonderfälle. Das RM soll nicht dieselbe gesamte Firmware-Tour erneut erzählen. Umgekehrt dürfen sicherheitsrelevante Bedingungen wie notwendiger Interruptschutz neben einem RM-Rezept stehen, auch wenn das UM bereits davor warnt.

Vorgeschlagener knapper Einstieg im RM:

> **New to Trice?** Follow the [User Manual](../TriceUserManual.md) for your first PC log and firmware integration. This reference explains the complete options, rules and limits. Use the [Guide](../TriceGuide.md) to find a task.

### Konkrete Überschneidungen und ihre Behandlung

| Vorhandene Inhalte | Behandlung im neuen RM |
| --- | --- |
| UM „Get lib and tools“ / „See your first log“ und RM Getting Started „Get/Install/Try/Use It“ | Die geführten ersten Schritte bleiben im UM. RM verlinkt dorthin; zusätzliche Betriebssystemdetails nach Environment Setup, Toolbau nach Building and Releasing. Kein zweiter Installations- oder Demo-Durchlauf. |
| UM Firmware-Integration und RM AUX-/RTT-/UART-Quickstarts | UM behält das einfache Grundgerüst. RM heißt hier **Firmware Integration Recipes** und ergänzt die vollständigen transportspezifischen Bedingungen, Konfiguration und Fehlerfälle. Gemeinsame Installation und Bind-Grundlagen verlinken; kurze zusammenhängende Konfigurationsbeispiele erhalten. |
| UM PC-Feature-Experimente und RM Examples, SL, CE, Tags und Stamps | Die geführten Änderungen an Beispielwerten bleiben im UM. Examples beschreibt Projektauswahl, Voraussetzungen und zusätzliche Abläufe; Fachkapitel definieren Syntax und Verhalten. Keine Kopie der gesamten PC-Tour. |
| UM „Know which files to keep“, Guide „Generated and local files“ und RM Bind/IDs | Bestehende kurze Nutzungsregeln außen erhalten. Vollständige Eigentums-, Wiederverwendungs-, Archivierungs- und Versionsregeln in IDs and Dictionaries; Bind ergänzt nur seine besonderen Artefakte und verlinkt die gemeinsamen Regeln. |
| UM erste Fehlerhilfe und RM Troubleshooting | RM ergänzt gezielte Diagnose und Sonderfälle. Keine zweite identische Symptomtabelle; für erste Prüfungen aufs UM, für Bind-/CE-Details auf deren Fachkapitel verweisen. |
| Guide Projektstruktur und RM Maintenance | Guide bleibt die kurze Orientierung. Maintenance dokumentiert nur die zusätzlichen Pflege- und Workflowdetails; die allgemeine Ordnerübersicht verlinken. |
| README Nutzenversprechen, UM Vier-Byte-Erklärung und RM Overview/Features | Kurze technische Konzepte im RM, ausführliche Nachweise bei Binary Format, Performance und Target Configuration. Keine erneute Aufzählung aller Verkaufsargumente. |

### Getting Started auflösen, Fachinformationen erhalten

Das bisherige [Getting Started](../rm/getting_started.md) enthält weit mehr als einen Einstieg. Beim Auflösen werden seine eigenständigen Referenzinformationen gezielt übernommen:

- Makros und Parameterbreiten → Writing Trice Messages; Stempel → Stamps.
- Critical Sections, Checks und Konfiguration → Target Configuration.
- Kommunikationswege → Transports; Codeübersicht → Target Implementation.
- Build-Einbindung → Bind; bestehende Makros/Aliases → Integrating Existing Logging Macros.
- Compiler-, Header- und Aufrufstellenlimits → jeweilige Bind-/CE-/Legacy-Referenz, nicht als alte pauschale Einschränkungen übernehmen.
- Fork/Clone → Repository-Anhang; Toolbau → Building and Releasing; zusätzliche Installationsdetails → Environment Setup.
- Insert/Clean-spezifische Abläufe → Deprecated-Anhang. Die normale erste Anwendung wird durch den UM-Verweis abgedeckt.

### Verweise aus den drei Einstiegsdokumenten

„Weitgehend unverändert“ lässt sich nicht als Garantie aller alten Linkziele verstehen: Zusammenlegungen entfernen Dateien und verschieben Abschnitte. Bei der späteren Umsetzung sind notwendige Linkkorrekturen sowie kurze inhaltliche Anpassungen für klare Zuständigkeiten, Bind-Fokus und weniger Wiederholungen erlaubt. Für jede weitergehende Änderung muss der Nutzen für diese Aufgabe erkennbar sein; die drei Dokumente nicht pauschal neu verfassen. **Jetzt werden auch diese Links nicht verändert.**

Beispiele für die spätere Zuordnung: README-Fußnoten auf Features/Logging Output führen künftig zum tatsächlichen Nachweis; Guide-Verweise auf Getting Started führen je nach Frage ins UM oder zum Fachkapitel. Die allgemeine Guide-Empfehlung „Avoid unnecessary recompilation → Trice Cache“ muss eng als Insert/Clean-Cache bezeichnet werden; Insert/Clean-Einstiege erhalten den Deprecated-Hinweis. Generate bleibt nach den Notes ein eigenes Advanced-Kapitel; seine fachlichen Anwendungen verweisen auf dessen Optionen statt sie mehrfach zu dokumentieren.

Keine Links aus README, Guide oder UM auf `rm1` umlenken. Bestehende sinnvolle Fachdateinamen und Abschnittsanker im neuen RM möglichst erhalten, soweit sie zum Inhalt passen; keine leeren Weiterleitungsdateien nur zur Bewahrung alter URLs. Gegenseitige Links müssen direkt zum gesuchten Inhalt führen: kein UM→RM→UM-Kreis ohne zusätzliche Information.

## Was die Bestandsaufnahme zeigt

Alle 52 Kapitel wurden nach Umfang, Untergliederung und Thema inventarisiert. Vertieft gelesen wurden die Einstiegstexte, kurzen Kapitel, ID-/Bind-Regeln, CE-Arbeitsabläufe und die verstreuten Insert-/Cache-Empfehlungen. Das ist eine Struktur- und Inhaltsprüfung, keine vollständige Neuabnahme aller historischen Messungen und Codebeispiele.

| Befund | Folgerung |
| --- | --- |
| [Target Configuration](../rm/target_configuration.md): 22 Zeilen; [Logging Output](../rm/logging_output.md): 16; [Endianness](../rm/endianness.md): 10; [Test Issues](../rm/test_issues.md): 6. | Eigenständige Hauptkapitel sind hier unnötige Zwischenstationen. Inhalte in das passende Fachkapitel übernehmen. |
| [Overview](../rm/overview.md), [How It Works](../rm/how_it_works.md) und [Features](../rm/features.md) erklären Nutzen und Grundprinzip mehrfach. | Eine gemeinsame Einführung; ausführliche Bedienregeln jeweils an ihrem fachlichen Ort. |
| [IDs](../rm/id_numbers.md), [ID Management](../rm/id_management.md), [TIL](../rm/id_reference_list_til_json.md) und [Decoding](../rm/decoding.md) erklären zusammen ID-Zuordnung, Dictionaries und deren Aufbewahrung. | Ein gemeinsames Nachschlagekapitel verhindert widersprüchliche Nebenregeln. |
| Getting Started bietet bereits Bind, erklärt danach aber erneut Insert/Clean und Cache. Die Firmware-Quickstarts führen noch durch `trice insert`. | Ein neuer Nutzer sollte einen durchgehenden Bind-Weg bekommen und nicht mehrere gleichrangige Abläufe zusammensetzen müssen. |
| Bind hat 1068, RTT 670, Examples 704 und Development Environment Setup 1052 Zeilen, einschließlich ToCs und Code. | Diese Kapitel nicht mit weiteren großen Kapiteln zusammenkleben. Die frühere Aufteilung des Monolithen soll ihren Nutzen behalten. |

Zeilenzahlen sind nur Größenhinweise: Bilder, Code und generierte ToCs beeinflussen sie. Eine starre Zeilenquote wäre kein sinnvoller Grund, zusammengehörige Inhalte zu zerlegen oder Beispiele zu streichen.

Beim erneuten Dateiabgleich zusätzlich gefunden: [GitHub Statistics](../rm/github_statistics.md) steht nicht im heutigen RM-Inhaltsverzeichnis. Die Zahl 52 bezeichnet dessen Fachkapitel; tatsächlich liegt noch diese weitere Fachdatei im Ordner. **GitHub-Statistik ist keine Trice-Tool-Funktion.** Die beiden Notes dazu werden gemeinsam umgesetzt: kein eigenes Kapitel der Trice-Fachreferenz, sondern ausschließlich ein Unterabschnitt des ausdrücklich gewünschten Projektpflege-Anhangs zu GitHub-Workflows und Maintenance-Skripten. Dort bleibt auch der noch nicht bestätigte Aktivierungsstand sichtbar. Die alte Datei wird wie der übrige Bestand unverändert archiviert.

## Vorgeschlagene Kapitel und Reihenfolge

Die englischen Titel sind Vorschläge für das spätere RM. Die Links zeigen auf die **heutigen Quellen**, nicht auf schon umgesetzte Ziele. Nach dem GO ist `rm1` die unveränderte Vergleichsbasis; das aktive RM wird neu aufgebaut. Passende bisherige Fachdateinamen möglichst übernehmen, um unnötige Änderungen eingehender Links zu vermeiden.

### Normale Nutzung

| Position | Vorgeschlagenes Kapitel | Heutige Quellen und Zweck |
| ---: | --- | --- |
| 1 | Trice Overview and Concepts | [Overview](../rm/overview.md), [How It Works](../rm/how_it_works.md), [Features](../rm/features.md). Knappe technische Konzepte: ID-Prinzip und Direct/Deferred; erste Schritte im UM verlinken, Bedienungsdetails an die Fachkapitel verweisen. |
| 2 | Firmware Integration Recipes | [Quickstarts](../rm/quickstarts.md). Die konkreten AUX-, RTT- und UART-Rezepte behalten und vollständig auf Bind ausrichten. Ergänzende Referenz zur UM-Einbindung, keine erneute Anfängertour. |
| 3 | Writing Trice Messages | [Compared with printf](../rm/printf_comparison.md). Der eigentliche Inhalt ist eine Makro-/Formatreferenz: Parameterbreiten, Floats, Strings, Buffer und unterstützte Formate. Der Vergleich mit printf bleibt Teil davon. |
| 4 | Trice Bind | [Bind](../rm/bind.md). Maßgeblicher Ort für Sidecars, Build-Abhängigkeiten, Include-Reihenfolge, unterstützte Aufrufstellen und Fehlerbehebung. Nicht mit der allgemeinen ID-Referenz verschmelzen. |
| 5 | Trice IDs and Dictionaries | [ID Numbers](../rm/id_numbers.md), [ID Management](../rm/id_management.md), [TIL](../rm/id_reference_list_til_json.md), [Decoding](../rm/decoding.md). IDs, Bereiche, TIL/LI, Eigentum, Stabilität, Versionsverträglichkeit und Replay zusammenführen. Target-Routing nur verlinken; das bekommt eine eigene Advanced-Referenz. |
| 6 | Target Configuration | [Target Configuration](../rm/target_configuration.md) und die targetseitigen Teile von [ON/OFF](../rm/switching_trice_on_and_off.md). Konfiguration, Critical Sections, Puffer und Abschalten zur Compile-Zeit. **Log-Level Control gehört vollständig zur Host-Auswahl, nicht hierher.** |
| 7 | Logging and Command-Line Tools | [CLI](../rm/cli.md), [Logging Output](../rm/logging_output.md). Empfang, Dateien, Replay und Displayserver; Befehlswegweiser mit Links auf die Fachreferenz. VIS und Generate stehen bei Advanced, keine zweite Optionsreferenz hier. |
| 8 | Stamps and Time Measurements | [Stamps](../rm/stamps.md). Target-/Host-Stempel und Delta-Spalten als zusammenhängende Funktion behalten. |
| 9 | Transports and Custom Writers | [Without UART](../rm/without_uart.md), [SD Cards and Custom Writers](../rm/custom_output.md). Ausgabewege auswählen, eigene Writer, Dateiaufzeichnung und spätere Dekodierung. RTT-Kurzhinweis mit Link; keine zweite RTT-Anleitung. |
| 10 | Trice over RTT | [RTT](../rm/rtt.md). Eigenständig behalten: J-Link-/ST-Link-Wege, Fallback-Konfiguration und Fehlerfälle sind umfangreich genug. |
| 11 | Example Projects | [Examples](../rm/examples.md). PC-/Feature-Touren und Boardprojekte als Katalog. F030-Folder-Diff bleibt der übersichtliche Instrumentierungsnachweis. Nicht zusätzlich mit Quickstarts oder Integrationsquelltexten überladen. |
| 12 | Troubleshooting and Limits | [Troubleshooting](../rm/troubleshooting.md), [Limitations](../rm/limitations.md), [Usage Tips](../rm/usage_tips.md). Fehlerbild → Prüfung → Abhilfe. Allgemeine Hinweise verteilen; genaue Bind-/CE-Grenzen an ihren maßgeblichen Stellen belassen und hier verlinken. |
| 13 | Performance and Buffer Selection | [Performance](../rm/performance.md). Auswahl zwischen Direct/Deferred und Puffertypen vor die unveränderten Messnachweise stellen. |
| 14 | Memory Usage and Code Size | [Memory Usage](../rm/memory_usage.md), [Code Size](../rm/code_size.md). RAM, Flash, Linker-/Compileroptionen und Messbedingungen gehören zusammen. Historische Messungen dahinter klar als solche kennzeichnen. |

### Advanced Features

Diese Kapitel bleiben aktuelle Funktionen, keine historischen Anhänge. Die Einführung nennt sie kurz und verlinkt sie; für den ersten Log muss man sie nicht lesen.

| Position | Kapitel | Behandlung |
| ---: | --- | --- |
| 15 | Tags, Colors, Filters and Statistics | [Tags, Color, and Weights](../rm/tags_color_and_weights.md), [Log-Level Control](../rm/log_levels.md) und Host-Filter aus [ON/OFF](../rm/switching_trice_on_and_off.md). `-logLevel`, `-pick`, `-ban`, Gewichte, Farben und Ereignisstatistik wirken auf dem Host. Target-Routing getrennt verlinken. |
| 16 | Visualization | VIS-Abschnitte aus [CLI](../rm/cli.md): Ausgabeformat, Weiterleitung, LabPlot-Aufbau, ausführbare Beispiele und Fehlerfälle. CLI und Guide verlinken hierher; keine doppelte LabPlot-Anleitung. |
| 17 | Structured Logging | [SL](../rm/structured_logging.md) eigenständig behalten: Feldsyntax, Typen, JSON/KV und Registry bilden einen eigenen Vertrag. |
| 18 | Context Enrichment | [CE](../rm/context_enrichment.md) eigenständig behalten und Bind zuerst erklären. Den Insert/Clean-Sonderweg später anordnen. Den vorhandenen PoC-Anhang samt Grenzen und Nachweisen erhalten. |
| 19 | Target Routing | Routing-Abschnitt aus [ID Management](../rm/id_management.md) und zugehörige Ausgabekonfiguration. ID-Bereiche und Zielkanäle zusammen erklären; allgemeine ID-Vergabe verlinken. Target-Routing ist keine Host-Filterung und `-logLevel` schaltet keine Target-Aufrufe ab. |
| 20 | Asynchronous Broadcast Commands | [ABC](../rm/abc_asynchronous_broadcast_commands.md) eigenständig behalten: Empfängerauswahl, Handler, Antworten und Simulator gehören zusammen. |
| 21 | Integrating Existing Logging Macros | [Legacy User Code](../rm/legacy_user_code.md). Der vorgeschlagene Titel vermeidet die Verwechslung mit deprecated ID-Verwaltung. Aliases, Wrapper und gemischte Ausgaben bleiben aktuelle Integrationsmöglichkeiten. |
| 22 | Generating Supporting Files | [Generate](../rm/generate.md) als gemeinsame Options- und Artefaktreferenz erhalten. Fachkapitel verlinken die passenden Abschnitte; Beispiele dort nur soweit für den jeweiligen Ablauf nötig. |
| 23 | typeX0 User Packets | [typeX0](../rm/typex0_user_packets.md) weitgehend erhalten und nach hinten stellen. Gezielte Korrekturen, Querverweise und verständlichere Formulierungen sind erlaubt; Protokollregeln, Beispiele und Besonderheiten nicht bei einer pauschalen Kürzung verlieren. |

### Anhang

Ein Anhang ist weiterhin gepflegte RM-Dokumentation. Nur der ausdrücklich bezeichnete Insert/Clean-Bereich bekommt den Status Deprecated.

| Position | Anhang | Heutige Quellen und Zweck |
| ---: | --- | --- |
| 24 | Binary Format, Framing and Encryption | [Binary Encoding](../rm/binary_encoding.md), [Framing](../rm/framing.md), [Endianness](../rm/endianness.md), [XTEA](../rm/optional_xtea_encryption.md). Record → Byte-Reihenfolge → Framing → optionale Verschlüsselung. Die kurzen Einbindungsanweisungen bleiben über Target Configuration auffindbar. |
| 25 | Target Implementation | [Target Implementation](../rm/target_implementation.md). Makro-, Puffer- und Transferinterna behalten; nicht noch die komplette Protokollspezifikation dazupacken. |
| 26 | Platform Integration Recipes | [Integration Notes](../rm/integration_notes.md). Vor allem das vollständige LibOpenCM3-Rezept mit Quelltexten ist ein Spezialfall, kein allgemeiner Einstieg. Nicht den gesamten Bereich pauschal als deprecated bezeichnen. |
| 27 | Development Environment Setup | [Environment Setup](../rm/development_environment_setup.md). Betriebssystem-, Compiler- und IDE-Details. UM und Integrationsrezepte verlinken direkt auf den benötigten Abschnitt; Installation nicht an mehreren Stellen vollständig erklären. |
| 28 | Testing Trice | [Target Tests](../rm/target_tests.md), [Test Issues](../rm/test_issues.md). Testtechniken im Detail: CGO-Datenweg, Erwartungen, Konfigurationsmatrizen, Overlay/Cache, Skriptsteuerung, Wiederherstellung und gezielte Fehlerreproduktion. Geplante Untergliederung siehe unten. |
| 29 | Building and Releasing the Tools | [Tool Build/Release](../rm/tool_build_release.md). Für Toolentwickler und Maintainer; Release- und Installationsdetails erhalten. |
| 30 | Working with the Repository | [Repository Workflow](../rm/repository_workflow.md), zusätzlich Fork-/Clone-Details aus Getting Started. Optionale OpenCommit-Anleitung klar als optional kennzeichnen; sie ist keine Trice-Voraussetzung. |
| 31 | Repository and Documentation Maintenance | [Maintenance](../rm/maintenance.md), zusätzlich [GitHub Statistics](../rm/github_statistics.md). GitHub-Workflows, zugehörige Maintenance-Skripte und Dokumentationspflege als Projektpflege-Anhang; Statistik nur als Unterabschnitt. Für Aufgaben, Auslöser, Ein-/Ausgaben und Seiteneffekte der Skripte kurze Tabellen. Allgemeine Projektstruktur im Guide, Build/Release und Testmechanik in deren Kapiteln verlinken. |
| 32 | Safe Build Scripts and Cancellation | [Ctrl-C](../rm/ctrl_c_robust_use_of_trice_insert_and_trice_clean.md). Allgemeine Zustandsverantwortung, Fehlerstatus und Abbruchbehandlung behalten. Insert/Clean-Beispiele als Legacy kennzeichnen; Muster fachlich an der aktuellen Wiederherstellung prüfen. |
| 33 | Deprecated: Insert/Clean and Its Cache | [Insert Algorithm](../rm/the_trice_insert_algorithm.md), [Insert/Clean Cache](../rm/insert_clean_cache.md), ergänzende bisher verstreute Legacy-Abläufe. Nutzung bestehender Projekte zuerst, Cache danach, Algorithmus-/Testdetails zuletzt. `trice add` als Import-/Wiederherstellungshilfe erklären, ohne aus seiner Platzierung eine neue Deprecation des Befehls abzuleiten. |
| 34 | Project Background and Resources | [History](../rm/history.md), [Future Development](../rm/future_development.md), [Resources](../rm/project_resources.md). Geschichte, nicht zugesagte Ideen, alternative Projekte und Community in getrennten Unterabschnitten. Kontakt-/Downloadlinks zusätzlich im RM-Einstieg direkt anbieten. |

## Zusammenführen heißt auch richtig zuordnen

### Generate zentral erklären und nach Zweck verlinken

Die Notes ändern den bisherigen Vorschlag: Generate bleibt als eigenes Advanced-Kapitel bestehen. Das schafft einen gemeinsamen Ort für Befehlsoptionen und erzeugte Dateien. Die Fachkapitel liefern den Anwendungskontext und verlinken direkt auf den passenden Generate-Abschnitt:

| Funktion in Generate | Fachlicher Einstieg mit Verweis dorthin |
| --- | --- |
| `generate -bindReport` | Bind: Sidecar-Diagnose. |
| `generate -onelineJSON` | IDs and Dictionaries: lesbare Arbeitskopien, Originale bleiben maßgeblich. |
| `generate -logC` | IDs and Dictionaries: Metadatenexport für eigene/lokale Decoder; Verweis aus Target Implementation. |
| `generate -colors` | Tags, Colors, Filters and Statistics. |
| `generate -abc` | ABC: Empfängerauswahl und generierte Tabelle. |
| Nicht vorhandener C#-Generator | Kurzer Hinweis bei den Exportmöglichkeiten; keine unterstützte Funktion daraus machen. |

Logging and Command-Line Tools enthält dazu nur einen kompakten Befehlswegweiser. Die genaue Bedeutung einer Generate-Option wird einmal im Generate-Kapitel gepflegt; die allgemeinen Regeln für Dictionaries, ABC oder Tags bleiben in deren Fachkapiteln. **Generate nicht mit Release-Werkzeugen zusammenfassen:** Ein Nutzer braucht Sidecar-Diagnosen auch ohne Interesse an GoReleaser.

### Host-Auswahl und Target-Verhalten klar trennen

| Mechanismus | Wo er wirkt | Ort der vollständigen Erklärung |
| --- | --- | --- |
| `-logLevel`, `-pick`, `-ban`, Taggewichte | Auf dem Host: empfangene Meldungen auswählen. Keine Verringerung der Target-Aufrufe oder ihres Übertragungsaufwands daraus ableiten. | Advanced: Tags, Colors, Filters and Statistics. |
| Target-Routing | Auf dem Target: konfigurierte ID-Bereiche und Ausgabekanäle verwenden. Die ID-Vergabe wird vom Host-Werkzeug vorbereitet. | Advanced: Target Routing. |
| Compile-Zeit-Abschaltung | Beim Firmware-Build: Log-Aufrufe abschalten. | Target Configuration. |
| VIS | Auf dem Host: Messwerte an eine Visualisierung weitergeben. | Advanced: Visualization. |

Das UM darf weiterhin einfache Filterexperimente anbieten. „Advanced“ bezeichnet den Ort der vollständigen Referenz, keine Voraussetzung für erste Experimente. Das bisherige Log-Level-Control-Kapitel geht vollständig in die Host-Referenz ein; seine veraltete v1.0-Erklärung wird nicht als Target-Konfiguration umgedeutet.

### Testtechniken mit nachvollziehbarem Datenweg erläutern

Der Testing-Anhang soll über eine Liste von Startbefehlen hinausgehen. Geplante Unterabschnitte, am aktuellen Code zu prüfen:

| Unterabschnitt | Inhalt und maßgebliche Quellen |
| --- | --- |
| Welche Prüfung weist was nach? | Go-Unit-/Integrationstests, native CGO-Targettests, Compiler-/Board-Konfigurationsbuilds und echte Hardwaretests unterscheiden. Ein Cross-Build ist kein Hardwaretest; Go-Coverage misst keine C-Abdeckung. |
| Ein CGO-Test vom Aufruf zum Vergleich | C-Bibliothek und Testkonfiguration werden in ein natives Testprogramm eingebunden. Go ruft C-Testfälle auf, C erzeugt Trice-Bytes, der echte Hostdecoder verarbeitet sie, Erwartungen werden verglichen. [`cgoPackage.go`](../../_test/testdata/cgoPackage.go), [`cgoTrice.c`](../../_test/testdata/cgoTrice.c) und [`triceCheck.c`](../../_test/testdata/triceCheck.c) als konkrete Quellen. CGO kurz als Go-Schnittstelle zu C erklären. |
| Fälle und Erwartungen hinzufügen | Konfiguration, passender Decoderaufruf, `//exp:`-Erwartungen und gemeinsame Harness-Vorlagen. Generierte Kopien nicht einzeln bearbeiten; Testgrenzen anhand des aktuellen Harness erklären. Die alte pauschale Aussage „nur ein Trice pro Testzeile“ nicht übernehmen: der heutige Harness behandelt auch mehrere und leert Deferred-Ausgabe mit mehreren Transferaufrufen. |
| Bulk-Ausführung und Einzeldiagnose | Der [PC-Worker](../../scripts/_160_pc_target_test_worker.sh) wählt den Testmodus und startet bei Bulk-Fehlern eine Einzeldiagnose. Eine bestandene Diagnose hebt den ursprünglichen Fehler nicht auf. Framing-/Pufferzustand zwischen Meldungen als möglichen Unterschied erläutern. |
| Build-Cache und Testausführung | [CGO-Overlay](../../scripts/pc_cache_overlay.go): Hashes der C-/Header-/Sidecar-/Konfigurationseingaben machen Änderungen für Go sichtbar, ohne Originalquellen umzuschreiben. Build-Cache spart Kompilierung; `-count=1` führt Tests trotzdem aus. Warm-/Kaltlauf auseinanderhalten. Dieser Cache ist nicht der deprecated Insert/Clean-Cache. |
| Skriptsteuerung und Zustandsbesitz | `testAll.sh`, [Runner](../../scripts/_110_test_runner.sh), Workflow-Wrapper und Worker: Auswahl quick/full, Parallelität, Logs, Exitstatus und Stop/Continue-Verhalten. `_test` wird bei `go test ./...` vom Root nicht automatisch durchlaufen; direkte Einzeltests und verwaltete Testläufe haben unterschiedliche Vorbereitung. |
| Schutz des Arbeitsstands | [Snapshot/Wiederherstellung](../../scripts/_140_trice_test_state.sh) für Quellen, TIL/LI und Sidecars; Erfolg, Fehler und wiederholtes Ctrl-C. Erst Worker beenden, dann wiederherstellen. Keine parallele Formatierung oder ID-Verarbeitung. Allgemeines Signal-/Skriptmuster im Cancellation-Anhang verlinken. |
| Fehler reproduzieren und Laufzeiten vergleichen | Fehlerlog, konkrete Konfiguration, ID-Workflow, Compiler, Jobzahl, Cachezustand und Testmodus dokumentieren. Zeit für Vorbereitung, Kompilierung und Ausführung unterscheiden; keine pauschale Windows-Ursache aus einer Gesamtdauer ableiten. |

Die GitHub-Maintenance-Referenz erklärt, **wann und durch welchen Workflow** diese Tests laufen; der Testing-Anhang erklärt, **wie sie prüfen**. Damit werden Skriptdetails nicht in zwei Kapiteln vollständig wiederholt. Diese Planung veranlasst keine Test- oder Skriptänderungen und keinen langen Testlauf.

### `trice add` als begrenztes Hilfswerkzeug erhalten

Der aktuelle [Befehl](../../internal/id/addIDs.go) liest bereits im Quelltext stehende numerische IDs und ergänzt die ID-/Location-Tabellen. Er schreibt die gelesenen Quelldateien nicht zurück; das prüft unter anderem [TestAddWithLIExtension](../../internal/id/addIDs_test.go). Er vergibt keine IDs für ID-freie Aufrufe und erzeugt keine Bind-Sidecars.

Damit bleibt er sinnvoll für den Import bereits instrumentierter Quellen und die Wiedergewinnung noch im Quelltext vorhandener Zuordnungen. **Für den normalen Bind-Ablauf wird `add` nicht benötigt.** Aus ID-freien Bind-Quellen allein kann es verlorene historische Zuordnungen nicht rekonstruieren; ebenso wenig aus dem Quelltext längst entfernter Meldungen. Es ist daher kein Ersatz für das Aufbewahren von TIL/LI.

Empfehlung: Befehl erhalten, als Spezialwerkzeug beim Legacy-Workflow dokumentieren und aus IDs and Dictionaries kurz verlinken. Kein eigenes Hauptkapitel und keine automatische Einstufung von `add` als deprecated. Über eine spätere Entfernung wäre gesondert zu entscheiden; dieser Dokumentationsauftrag ändert keine CLI-Funktion.

**Konkrete Aufgabe für das neue RM:** Den Unterschied zwischen `add` und `insert` mit einer kurzen Tabelle und einem Beispiel erklären. Auch `insert` übernimmt vorhandene IDs in TIL/LI; das ist kein Alleinstellungsmerkmal von `add`. Dessen besonderer Nutzen ist: Tabellen ergänzen, ohne Quellen zu verändern oder Aufrufe neu zu instrumentieren. `add` ist keine notwendige Vorstufe zu `insert`.

| Verhalten | `add` | `insert` |
| --- | --- | --- |
| Vorhandene IDs in TIL/LI übernehmen | Ja. | Ja. |
| Aufrufe ohne ID | Überspringt sie. | Vergibt IDs. |
| ID bereits für eine andere Meldung belegt | Meldet den Konflikt; Quelle bleibt unverändert. | Vergibt eine andere passende ID und ändert die Quelle. |
| Quelldateien verändern | Nie. | Bei Bedarf. |

Beispiel für die spätere Erklärung:

```c
trice(iD(123), "Existing");
trice("New");
```

Bei konfliktfreier ID `123` erfasst `add` nur die erste Zuordnung; `insert` erfasst sie ebenfalls und ergänzt eine ID beim zweiten Aufruf. Bei vollständig und konfliktfrei instrumentierten Quellen können beide denselben Zweck erfüllen. `add` ist besonders dann sinnvoll, wenn vorhandene Quellen exakt erhalten bleiben müssen, etwa beim Import instrumentierter Fremdquellen oder beim Erfassen der noch vorhandenen Zuordnungen eines bestehenden Firmwarestands. Keine vollständige Wiederherstellung verlorener Historie versprechen.

### Usage Tips verteilen

Ein Sammelkapitel mit Installationshinweis, Pufferschutz, Buffer-Makros, Logdateien und Race-Tests zwingt Leser zum Suchen. Diese Inhalte bekommen feste Ziele:

- Konfiguration, Overflow-Schutz und Stackbedarf → Target Configuration beziehungsweise Memory Usage.
- Buffer-/String-Makros → Writing Trice Messages.
- Logdateien, mehrere Targets und Drittprogramme → Logging and Command-Line Tools.
- Erste Verbindung und Cycle-Fehler → Troubleshooting; die Bitdarstellung bleibt beim Binary Format.
- Entwickler-/Race-Tests → Testing Trice.

Auch die anderen Zusammenlegungen sind keine unveränderte Aneinanderreihung alter Kapitel: gemeinsame Erklärung einmal, zusätzliche Beispiele und besondere Bedingungen vollständig erhalten.

## Bind als durchgehender Standard

Der im UM gezeigte normale Ablauf ist auch die Grundlage der RM-Referenz:

```text
Edit source → trice bind → build/flash → tlog
```

Das UM enthält bereits Sidecar-Verzeichnis und Compiler-Include-Pfade. Im RM stehen die vollständigen Build-Regeln im Bind-Kapitel; die transportspezifischen Rezepte verlinken sie und zeigen nur ihre jeweiligen Ergänzungen. Bind hält numerische IDs aus den Log-Aufrufen heraus, kann aber verwaltete Includezeilen in Quellen ergänzen oder aktualisieren. Nicht behaupten, Bind ändere grundsätzlich keine Quelldateien.

| Heutige Stelle | Vorgeschlagene Änderung bei einer späteren Umsetzung |
| --- | --- |
| How It Works beschreibt das Grundprinzip über hineingepatchte IDs und endet mit `insert -cache`/`clean -cache`. | Grundprinzip über Bind, Dictionary und generierte Compiler-Header erklären. Das frühere Quelltext-Patching ist ein Legacy-Verweis. |
| Features nennt Insert/Clean samt Cache weiterhin als normalen Build-Ablauf. | Bind als normalen Ablauf zeigen. Cache- und `TRICE_CLEAN`-Bedienung in den Deprecated-Bereich; aktuelle Vorteile wie Dictionary-Reload an ihrem Fachort erhalten. |
| Getting Started mischt Bind mit ausführlichen Legacy-Alternativen, Checkout-Hooks und Cache-Empfehlungen. | Kapitel auflösen: erste Schritte im UM verlinken, Fachdetails zuordnen, Legacy-Abläufe nach hinten. Der kurze Umstiegshinweis steht im Bind-Kapitel. |
| Alle drei Firmware-Quickstarts haben Insert-Schritte. | Tatsächliche Bind-Rezepte mit Sidecar-Include-Pfad und Build-Abhängigkeit schreiben. Nicht lediglich das Wort `insert` durch `bind` ersetzen. |
| CLI, IDs/TIL und printf-Vergleich setzen teilweise sichtbare Inline-IDs voraus. | Begriffe über Bind erklären. Explizite IDs nur in bewusst bezeichneten Legacy-, Format- oder Diagnosebeispielen. |
| CE erläutert Insert/Clean bereits vor vielen Regeln zum normalen Einsatz. | Bind-Einstieg und gemeinsame CE-Regeln zuerst. Die reversible Insert/Clean-Erweiterung danach als gekennzeichneten Sonderweg erhalten. |

Im Bind-Kapitel bleibt der **kurze Umstieg von Insert zu Bind** früh auffindbar. Rückmigration, lange `TRICE_CLEAN`-Zustandstabellen und Insert-Buildfolgen können weiter hinten stehen. Die Grenzen von Bind und die Compiler-/`__COUNTER__`-Tabellen bleiben sichtbar und maßgeblich; der bisherige technische Fallback darf nicht verschwiegen werden.

## Deprecated klar und eng definieren

Vorgeschlagener kurzer englischer Hinweis:

> **Deprecated workflow. Use `trice bind` for new projects. `insert`/`clean` and their `-cache` option remain available for existing projects and cases not supported by Bind.**

Im hinteren Legacy-Bereich einmal ausführlich erklären, vorne nur kurz dorthin verweisen. In diesem Vorschlag ist „Deprecated“ eine Dokumentationsentscheidung: keine entfernten Befehle, neuen Laufzeitwarnungen, Entfernungstermine oder geänderten Compilerfähigkeiten behaupten.

Die Kennzeichnung betrifft **`trice insert`, `trice clean` und deren Quelltext-Cache**. Sie betrifft nicht:

- `make clean`, normale Build-Bereinigung oder die exakte Wiederherstellung nach Tests;
- Go-/Compiler-Caches oder generierte Sidecars;
- IDs und `til.json` selbst;
- aktuelle Aliases, bestehende Logging-Makros, SL, CE oder typeX0.

Mit diesem Status auch Formulierungen wie „permanently available insert/clean workflow“ im heutigen Bind-Kapitel abstimmen: verfügbarer Fallback, aber keine zusätzliche unbegrenzte Support- oder Entfernungsaussage erfinden.

## Inhaltliche Stellen, die nicht blind übernommen werden sollten

| Stelle | Befund und vorsichtiger Umgang |
| --- | --- |
| [Log-Level Control](../rm/log_levels.md) und [Features](../rm/features.md) | Die v1.0-Überschriften beziehungsweise die Erklärung über eine Position in `ColorChannels` passen nicht zur aktuellen Gewichtsreferenz in [Tags](../rm/tags_color_and_weights.md). Host-Filter dorthin konsolidieren; Target-Abschaltung und ID-Routing getrennt erklären. |
| [Features](../rm/features.md) gegenüber [printf-Referenz](../rm/printf_comparison.md) und [Binary Encoding](../rm/binary_encoding.md) | Features nennt 32737 Stringbytes; die anderen Kapitel unterscheiden Formatgrenze 32767 und aktuelle Makrogrenze bis 32752. Beim Umschreiben die bereits hergeleiteten Grenzen verlinken, nicht die abweichende Zahl weiterkopieren. |
| Bind, Decoding und TIL | „Für die nächste ID-Verarbeitung behalten“, „passend zur Firmware archivieren“ und „versionieren“ sind verschiedene Anforderungen. Eine gemeinsame TIL-/LI-/Sidecar-Tabelle soll sie unterscheiden; LI ist im Repository lokal generiert, kann in anderen Projekten bewusst versioniert werden. |
| [Getting Started](../rm/getting_started.md), letzte Limitations-Abschnitte | Aussagen wie mehrere Trices in einer Zeile oder keine Makros in Headern sind ohne Trennung nach Bind, Insert und CE missverständlich. Auf die aktuellen Unterstützungstabellen verweisen; alte pauschale Grenzen nicht in die neue Einleitung übernehmen. |
| [Test Issues](../rm/test_issues.md) | Die pauschale Aussage über unbekannt fehlschlagende `ERROR_`-Tests vor einer Übernahme am aktuellen Teststand prüfen. Kein neuer bekannter Fehler allein aus altem Text. |
| [Ctrl-C-Muster](../rm/ctrl_c_robust_use_of_trice_insert_and_trice_clean.md) | Das Beispiel setzt mit `trap - INT TERM EXIT` Signale zurück. Die aktuelle Testwiederherstellung schützt sich gegen weitere INT/TERM. Den alten Beispielcode nicht ungeprüft als aktuelle Empfehlung übernehmen; keine Codeänderung aus diesem Strukturvorschlag ableiten. |
| [Integration Notes](../rm/integration_notes.md) | Das LibOpenCM3-Rezept erzeugt Insert-Zwischenquellen. Als historisches beziehungsweise noch nicht auf Bind angepasstes Rezept kennzeichnen, solange keine gesonderte Anpassung und Prüfung erfolgt ist. Nicht durch Wortersetzung eine neue Bind-Abnahme behaupten. |

## Was ich nicht zusammenlegen würde

| Möglichkeit | Bewertung |
| --- | --- |
| Getting Started + Quickstarts + Examples | Verworfen: bereits heute zusammen 1609 Quellzeilen; Einstieg und gezielte Rezepte wären wieder schlechter direkt erreichbar. |
| Bind + ID-Referenz + Insert-Algorithmus | Verworfen: Bedienung, gemeinsame Datenregeln und deprecated Implementierungsdetails würden erneut vermischt. |
| SL + CE + typeX0 | Verworfen: drei unterschiedliche Verträge; typeX0 bleibt ein eigenständiges, weitgehend erhaltenes Kapitel, auch wenn gezielte Anpassungen nun erlaubt sind. |
| RTT + alle anderen Ausgabewege | Verworfen: der kleine allgemeine Transportwegweiser würde im großen RTT-Kapitel untergehen. |
| Performance + Memory + Code Size als ein Gesamtkapitel | Verworfen: knapp 800 Zeilen mit Messprotokollen. Memory und Code Size zusammenführen, Ausführungsgeschwindigkeit eigenständig lassen. |
| Repository Workflow + Maintenance + Build/Release + Ctrl-C | Verworfen: mehr als 1200 Zeilen und mehrere unterschiedliche Leserfragen. Die Anhangsgruppe ordnet sie, ohne sie in eine Datei zu pressen. |
| Nur 14 vordere Kapitel zählen und die übrigen unverändert verstecken | Kein Ersatz für die Zusammenlegung: auch Advanced und Anhänge zählen zum Bestand. Das Zielbild hat insgesamt 34 Fachkapitel. |

## Vorsichtiger späterer Umsetzungsweg

1. **Gesondertes GO abwarten.** Jetzt ausschließlich diesen Vorschlag bearbeiten. Die Zuordnung deckt alle heutigen 52 Fachkapitel ab; kein Fachthema wird allein zur Verringerung der Anzahl gelöscht. Vor Umsetzung den aktuellen Stand mit den Arbeiten auf dem anderen PC abgleichen.
2. **Bestand erfassen und unverändert archivieren.** Vor Änderungen Dateiliste, Inhalte und eingehende Links aufnehmen. Dann den gesamten bisherigen Ordner `docs/rm/` nach `docs/_scratchPad/rm1/` verschieben, einschließlich README, HEADER, FOOTER und aller sonstigen enthaltenen Dateien. Die Schreibweise folgt dem vorhandenen `_scratchPad`-Verzeichnis. Ein bereits vorhandenes `rm1` nicht überschreiben. Dateiinhalt und relative Struktur müssen exakt erhalten bleiben: keine Formatierung, ToC-Neuerzeugung, neuen Archivhinweise oder Linkanpassungen darin.
3. **Neues RM unter `docs/rm/` aufbauen.** Zunächst die gewählte Leserführung, dann eine fachlich zusammengehörige Gruppe nach der anderen. Inhalte aus `rm1` übernehmen und im neuen RM bearbeiten; das Archiv bleibt eingefroren. typeX0 weitgehend übernehmen, gezielte Anpassungen am Inhalt reviewen und in der neuen README/PDF-Reihenfolge nach hinten stellen.
4. **Konkrete Übernahmeliste führen.** Für jeden Abschnitt Quelle, Ziel, besondere Beispiele und eingehende Links festhalten. Doppelte Erklärungen konsolidieren; bereits im UM geführte erste Schritte verlinken. Eigenständige Kommentare, Bilder, Codebeispiele, Grenztabellen, Messwerte und CE-PoC erhalten. Widersprüchliche Alttexte fachlich prüfen, nicht nur kopieren.
5. **Aktive Links und Einstiege abstimmen.** Interne Links im neuen RM anpassen. Root-README, Guide und UM bei Bedarf gezielt mit anpassen; ihren Aufbau und den ersten Lernweg im UM erhalten. Die Notes erlauben sinnvolle inhaltliche Änderungen für diese Aufgabe, nicht nur neue Linkziele. Ebenso betroffene Verweise in Quellen, Hilfe und Tests erfassen und vor einer Bearbeitung den Umfang benennen. Alles bleibt bis zum GO Planung. Keine aktive Dokumentation nach `rm1` verlinken und keine alten Kapitel als leere Weiterleitungsdateien stehen lassen.
6. **Bestehenden Pflegeweg erhalten.** README-Reihenfolge, lokale mdtoc-ToCs und temporärer stitchmd-Monolith bleiben maßgeblich für das neue RM. `rm1` ist ausschließlich historische Vergleichsbasis, keine zweite gepflegte Quelle und kein PDF-/Pages-Eingang. Kein INDEX und kein neues Dokuwerkzeug. Die aktuelle PDF-Erzeugung nummeriert Kapitel weiter fortlaufend; keine automatische A/B/C-Anhangsnummerierung versprechen.
7. **Gezielt prüfen.** Unverändertes Archiv gegen den zuvor erfassten Bestand vergleichen; typeX0-Änderungen im neuen RM auf inhaltliche Erhaltung prüfen. Alle aktiven Links einschließlich README, Guide und UM testen. Verschiebungsbedingt ungültige relative Links im Archiv bewusst unangetastet lassen und als Archivgrenze benennen; keine Linkreparatur oder Formatierung darüber laufen lassen. Danach RM-PDF mit Nummerierung, Querverweisen, Tabellen und Seitenanfängen kontrollieren. Kein globales `format_repo.sh` und kein paralleler Full-Test für eine redaktionelle Änderung.

```text
README.md                         Projekteinstieg, weitgehend unverändert
docs/
├── TriceGuide.md                  Wegweiser, weitgehend unverändert
├── TriceUserManual.md             Erste Schritte, weitgehend unverändert
├── rm/                           Neues aktives RM nach gesondertem GO
│   ├── README.md                  Leserführung und Kapitelverzeichnis
│   └── …                         34 Fachkapitel + HEADER/FOOTER
└── _scratchPad/
    ├── RM_Strukturvorschlag.md    Dieses Planungsdokument
    └── rm1/                      Bisheriges docs/rm, exakt unverändert
```

Eine behutsamere Variante wären **35 Kapitel**, wenn Memory Usage und Code Size zunächst getrennt bleiben. Die Kapitelzahl ist kein Selbstzweck: VIS, Target-Routing und Generate bleiben nach den Notes bewusst getrennt. Weitere Reduktionen sollen aus echten Überschneidungen entstehen, nicht aus einer Mischung verschiedener Leserfragen.


---

## Notes

Ausgewertet und oben eingearbeitet. Die folgenden ursprünglichen Notizen bleiben als Entscheidungsgrundlage erhalten. GitHub-Statistik wird nur im ausdrücklich genannten Projektpflege-Anhang behandelt; typeX0 und aufgabenbezogene Änderungen an README/Guide/UM sind nun erlaubt. Das GO zur Umsetzung steht weiterhin aus.

- Wenn es für die Aufgabe sinnvoll ist, dürfen UM, Guide und Root README.md verändert werden.
- gh statistics gehört nicht ins rm - es hat mit dem Trice Tool nichts zu tun.
- Advanced: VIS, LogLevel / ban pick Target Roiuting Generate typeX0
- LogLevel Control ist keine Targetkonfiguration! Das passiert auf dem Host
- trice add - wird das noch gebraucht?
- GH Maintenance Skripts auch in einem Anhangskapitel erläutern. Hier kann auch statistice dann mit rein.
- Trice Tests Techniken im Detail erläutern, Speziell CGO Tests und Testskript Besonderheiten
- typeX0 soll inhaltlich weitgehend erhalten bleiben, darf aber angepasst werden.

## Nachtrag (Nur zur Info für das RM Update - kein Umsetzungsauftrag für andere Repo-Bereiche)

RPC ist komplett obsolete. Nicht aus den Sourcen entfernen, aber aus den examples. Soweit noch in der Doku, in den Anhang als obsolet verbannen.
Dort wo insert in den examples noch auftaucht, soll es später komplett verschwinden
