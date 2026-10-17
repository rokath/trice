# RM2: Umsetzung und Übernahme

Stand: 16. Oktober 2026. Branch `rm2`, Ausgangscommit `beb8acee`. Umsetzung von [RM_Strukturvorschlag.md](RM_Strukturvorschlag.md). Die Ergänzung „Nachtrag“ im Vorschlag stammt vom Benutzer und bleibt erhalten.

## Ergebnis

- 34 Fachkapitel: 14 normale Nutzung, 9 Advanced, 11 Anhänge. Dazu README, HEADER und FOOTER.
- Bind ist der normale Integrationsweg. Insert/Clean und deren Quelltext-Cache sind als deprecated im Anhang dokumentiert; Add ist davon ausdrücklich ausgenommen.
- README und UM behalten Inhalt/Aufbau; dort nur betroffene Verweise angepasst. Im Guide zusätzlich die beiden Legacy-Einstiege eindeutig gekennzeichnet.
- Aktive Verweise in Dokumenten und Quell-/Buildkommentaren aktualisiert. Keine Laufzeit-, Compiler- oder Testablaufänderung.
- Das alte RM liegt vollständig unter `rm1/`: 56 Dateien bytegleich mit `HEAD:docs/rm/`. Seine relativen Links wurden absichtlich nicht repariert. Es bleibt von aktiven Checks und Generierung ausgeschlossen.

## Kapitelübernahme

Quellen in dieser Tabelle bezeichnen den eingefrorenen Bestand unter `rm1/`; Ziele liegen unter `../rm/`, außer dem ausdrücklich genannten UM. Abschnitte mit unverändertem Thema behalten soweit möglich ihren Namen und Anker. Die Navigation folgt der neuen RM-README.

| Alte Datei | Aktive Ziele | Behandlung |
| --- | --- | --- |
| `FOOTER.md` | `FOOTER.md` |  |
| `HEADER.md` | `HEADER.md` |  |
| `README.md` | `README.md` |  |
| `abc_asynchronous_broadcast_commands.md` | `abc_asynchronous_broadcast_commands.md` |  |
| `binary_encoding.md` | `binary_encoding.md` |  |
| `bind.md` | `bind.md`, `insert_clean_cache.md` |  |
| `cli.md` | `visualization.md`, `cli.md`, `insert_clean_cache.md`, `tags_color_and_weights.md` |  |
| `code_size.md` | `memory_usage.md` |  |
| `context_enrichment.md` | `context_enrichment.md` |  |
| `ctrl_c_robust_use_of_trice_insert_and_trice_clean.md` | `ctrl_c_robust_use_of_trice_insert_and_trice_clean.md` |  |
| `custom_output.md` | `custom_output.md` |  |
| `decoding.md` | `id_management.md` |  |
| `development_environment_setup.md` | `development_environment_setup.md` |  |
| `endianness.md` | `binary_encoding.md` |  |
| `examples.md` | `examples.md` |  |
| `features.md` | `overview.md`, `memory_usage.md`, `cli.md`, `target_configuration.md`, `insert_clean_cache.md`, `binary_encoding.md`, `project_resources.md`, `performance.md`, `tags_color_and_weights.md`, `stamps.md`, `custom_output.md`, `id_management.md`, `printf_comparison.md`, `generate.md`, `legacy_user_code.md` | Gemeinsame Einführung gekürzt; Fachregeln nach Zuständigkeit verteilt. |
| `framing.md` | `binary_encoding.md` |  |
| `future_development.md` | `project_resources.md` |  |
| `generate.md` | `generate.md` |  |
| `getting_started.md` | `../TriceUserManual.md`, `target_configuration.md`, `insert_clean_cache.md`, `printf_comparison.md`, `stamps.md`, `target_implementation.md`, `repository_workflow.md`, `custom_output.md`, `troubleshooting.md`, `tool_build_release.md`, `examples.md` | Erste Schritte ins UM verlinkt; Makros, Stempel, Schutz, Entwicklung und Legacy an Fachstellen. |
| `github_statistics.md` | `maintenance.md` |  |
| `history.md` | `project_resources.md` |  |
| `how_it_works.md` | `overview.md` |  |
| `id_management.md` | `target_routing.md`, `insert_clean_cache.md`, `id_management.md` |  |
| `id_numbers.md` | `id_management.md` |  |
| `id_reference_list_til_json.md` | `id_management.md` |  |
| `insert_clean_cache.md` | `insert_clean_cache.md` |  |
| `integration_notes.md` | `integration_notes.md` |  |
| `legacy_user_code.md` | `legacy_user_code.md` |  |
| `limitations.md` | `troubleshooting.md`, `insert_clean_cache.md`, `printf_comparison.md` |  |
| `log_levels.md` | `tags_color_and_weights.md` |  |
| `logging_output.md` | `cli.md` |  |
| `maintenance.md` | `maintenance.md` | Aktuelle Workflow-/Skripttabellen ersetzen historische Behauptungen. |
| `memory_usage.md` | `memory_usage.md` |  |
| `optional_xtea_encryption.md` | `binary_encoding.md` |  |
| `overview.md` | `overview.md` |  |
| `performance.md` | `performance.md` |  |
| `printf_comparison.md` | `printf_comparison.md` |  |
| `project_resources.md` | `project_resources.md` |  |
| `quickstarts.md` | `quickstarts.md` |  |
| `repository_workflow.md` | `repository_workflow.md` |  |
| `rtt.md` | `rtt.md` |  |
| `stamps.md` | `stamps.md` |  |
| `structured_logging.md` | `structured_logging.md` |  |
| `switching_trice_on_and_off.md` | `target_configuration.md`, `tags_color_and_weights.md` |  |
| `tags_color_and_weights.md` | `tags_color_and_weights.md` |  |
| `target_configuration.md` | `target_configuration.md` |  |
| `target_implementation.md` | `target_implementation.md` |  |
| `target_tests.md` | `target_tests.md` |  |
| `test_issues.md` | `target_tests.md` | Alte pauschale ERROR_-Fehlerbehauptung nicht als aktuellen Fehler übernehmen. |
| `the_trice_insert_algorithm.md` | `insert_clean_cache.md` |  |
| `tool_build_release.md` | `tool_build_release.md` |  |
| `troubleshooting.md` | `troubleshooting.md` |  |
| `typex0_user_packets.md` | `typex0_user_packets.md` | Fachinhalt unverändert; nur Navigation/ToC. |
| `usage_tips.md` | `troubleshooting.md`, `memory_usage.md`, `cli.md`, `target_configuration.md`, `../TriceUserManual.md`, `printf_comparison.md`, `project_resources.md`, `target_tests.md` |  |
| `without_uart.md` | `custom_output.md` |  |

## Wichtige Abschnittszuordnungen und Erhaltung

| Alter Abschnitt | Neues Ziel / Entscheidung |
| --- | --- |
| CLI: VIS und LabPlot einschließlich CSV-/Trice-Demo, Projektanlage und Fehlerhilfe | `visualization.md`, mit allen vier Demo-/Aufbaubereichen. |
| ID Management: ID Routing und Ausgabe nach Tags | `target_routing.md`; Bind-Beispiele, inklusive Bereichsgrenzen und Single-Pack-Bedingung. |
| Bind: TRICE_CLEAN, Rückmigration und Zustandstabelle | `insert_clean_cache.md`; die kurze Migration nach Bind und Compiler-/Wrapper-Grenzen bleiben in Bind. |
| TIL, LI, Decoding, ID Numbers | `id_management.md`: Eigentum, stabile Zuordnung, Aufbewahrung, Export, Versionsverträglichkeit und 1.x-Migration. |
| Getting Started: Makros, Bitbreiten, Puffer, Float-/String-Umstellung | `printf_comparison.md`; Beispiele korrigiert, ausführliche Formatgrenzen erhalten. |
| Getting Started: Critical Sections, Checks; Features/Usage Tips: Überlauf | `target_configuration.md`; Overflow-Zähler ausdrücklich von Cycle-Fehlern getrennt. |
| Getting Started: Timestamp-Konfiguration | `stamps.md`. |
| Getting Started: Quellmodulübersicht | `target_implementation.md`. |
| Getting Started: Fork/Clone | `repository_workflow.md`. |
| Usage Tips: Logdateien, Drittwerkzeuge, mehrere Targets | `cli.md`. |
| Features: Dictionary-Reload | `cli.md#automatic-dictionary-reload`. |
| Features: Transferbytes, Framing, Endianness, XTEA | `binary_encoding.md`; Transferbeispiele erhalten, XTEA-Einstellung konkretisiert. |
| LogLevel und ON/OFF | Host-Auswahl in `tags_color_and_weights.md`; Compile-Time-Abschaltung in `target_configuration.md`. |
| Code Size und Memory | `memory_usage.md`; historische Messungen und Bildbelege erhalten. |
| Performance | Puffervergleich vor den erhaltenen Timing-/Disassembly-Belegen; Messbedingungen klar abgegrenzt. |
| CE: Insert/Clean-Erweiterung | Nach den gemeinsamen Regeln; Bind bleibt Einstieg. PoC und Grenzen erhalten. |
| Ctrl-C | Aktuelles atomisches Schreiben und Snapshot-Wiederherstellung dokumentiert; unsicheres altes Trap-Muster ersetzt. |
| Tests | CGO→echte C-Bytes→Decoder→Erwartung, Bulk/Einzelmodus, Overlay-/Build-Cache, Ergebnis-Cache, Grenzen und Zeitvergleich erklärt. Historischer 2024-Log klar bezeichnet. |
| GitHub Statistics | Nur `maintenance.md#github-statistics`, einschließlich Einrichtung, Grenzen und noch unbestätigter Aktivierung. |
| RPC | Ältere RPC-Unterstützung im Implementierungsanhang als obsolet gekennzeichnet. Beispielquellen bleiben gemäß Nachtrag unverändert; ihre Entfernung ist spätere Arbeit. ABC und allgemeine RPC-Protokollentwürfe darüber sind davon zu unterscheiden. |

Alle eigenständigen HTML-Kommentarblöcke des alten RM sind im neuen Bestand vorhanden (mdtoc-Verwaltungsblöcke separat erzeugt). Die frühere Download-Schaltflächenabbildung entfällt mit der doppelten Installationsführung; erste Schritte bleiben im UM. typeX0-Fachtext, CE-PoC, historische Messprotokolle und Protokoll-Grenztabellen bleiben erhalten. Historische Shell-/Go-Pseudocodepassagen im deprecated Insert-Anhang sind keine neue Codeabnahme.

## Prüfstand

- Archivvergleich aller 56 Dateien mit dem Git-Ausgangsstand: **PASS**, keine Byteabweichung.
- `./scripts/_440_test_manual_format.sh`: **PASS**.
- `./scripts/_450_test_markdownlint.sh`: **PASS**.
- `lychee --config lychee.toml --offline --include-fragments ...`: **PASS**, 1772 Links, keine Fehler.
- `./scripts/_530_test_links.sh`: **PASS**, 1770 Links, keine Fehler; 94 konfigurierte Ausschlüsse.
- `go test ./scripts -run 'Test(Manual|ReferencePDF)' -count=1` mit freigegebenem temporärem `GOCACHE`: **PASS**, 21.262 s. Erster Versuch mit dem normalen macOS-Cache scheiterte an Sandbox-Zugriff; Wiederholung mit bestehendem freigegebenem Cache erfolgreich.
- `./scripts/_470_test_actionlint.sh`: **PASS**; im Workflow nur der Diagnoseverweis geändert.
- `git diff --check`: **PASS**.
- Beide PDFs mit dem bestehenden Skript erzeugt: UM 9 Seiten, RM 433 Seiten; **801 interne Links und 1192 Anker** geprüft. Alle 34 Kapitelziele im PDF vorhanden. Inhaltsverzeichnis, Integrationscode, IDs, Testtabelle und Add/Insert-Vergleich stichprobenartig als PNG kontrolliert: lesbar, keine sichtbaren Überlappungen oder abgeschnittenen Inhalte.
- Suche nach entfernten RM-Dateinamen in aktiven versionierten Dateien einschließlich Code/Skripten: **keine verbleibenden Verweise**.
- Finaler PDF-Neulauf zunächst durch Sandbox-DNS (`ENOTFOUND registry.npmjs.org`) blockiert; Wiederholung über den bereits freigegebenen Buildpfad: **PASS**, beide PDFs neu erzeugt.
- Kein Full-Test, Hardwaretest, Release, Commit oder Push durchgeführt.

## Abschluss

Die Umsetzung liegt uncommitted vor. PDFs unter `docs/` und Prüfartefakte unter `temp/` sind lokale, neu erzeugbare Dateien. Kein weiterer Implementierungsschritt ist für diesen Strukturauftrag offen. Vor einem späteren Commit die neue Kapitelgliederung und die gezielten Textkorrekturen reviewen; die Benutzerergänzung im Vorschlagsdokument gehört separat zur Planung.

