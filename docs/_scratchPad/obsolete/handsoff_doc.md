# Dokumentationsübergabe

Stand: 14. Oktober 2026. Auf dem anderen Rechner mit `handson doc` laden, den Checkout prüfen und auf die nächste Nutzeranweisung warten.

## Ziel und Git-Stand

Die aktuelle Dokumentationsarbeit ist abgeschlossen; der Rechnerwechsel steht an. Neue Anleitung gehört ins RM, der Changelog enthält nur wesentliche Änderungen mit Links.

- Branch: `wip`.
- Basis-HEAD: `9d2c2a6e709119cdc6740376f30e103397bd71dc` — `wip`. Ein späterer Commit dieser Übergabe folgt diesem Stand.
- Noch uncommittet: [AGENTS.md](../../AGENTS.md), [CHANGELOG.md](../../CHANGELOG.md), [Context Enrichment](../rm/context_enrichment.md), [1.x-Migration](../rm/id_reference_list_til_json.md) und diese Übergabe.
- Kein Commit, Push, Tag oder Release wurde für diesen Auftrag ausgeführt. Alle fünf Dateien müssen vor dem Rechnerwechsel noch gesichert und übertragen werden.

## Bereits in Git enthalten

- [docs/rm/README.md](../rm/README.md) bestimmt die Reihenfolge der 52 gepflegten Hauptkapitel. Die Dateinamen wurden vereinfacht; aktive Links zeigen auf die Kapitel.
- Der PDF-Build erzeugt beide Manuals aus den Quellen, einschließlich vollständiger Haupt- und Unterkapitelnummerierung. Die frühere offene Nummerierungsaufgabe ist erledigt (`f29a3f97`). Einstieg: [PDF-Skript](../../scripts/_320_generate_trice_user_manual_pdf.sh); Zusammenbau: [RM-Helper](../../scripts/_prepare_reference_manual.cjs). PDF-Navigation und neue Seiten je Hauptkapitel sind implementiert.
- Die Release-Vorbereitung enthält taggebundene Veröffentlichung und Audits, Changelog-Auswahl als Release Notes, vollständige Archiv-Lizenzen und Prüfungen der ausgelieferten Bind-/Text-/JSON-/KV-Funktionen. Siehe [Release-Anleitung](../rm/tool_build_release.md). Veröffentlichungsstatus wurde in diesem Auftrag nicht extern geprüft.

## Aktuelle lokale Änderungen

- Im RM stehen zwei Vergleichstabellen für Compiler mit bzw. ohne `__COUNTER__`, mit kurzem GCC-/Clang-Hinweis und Beispiel für ein einfaches Log-Makro. [Einstieg zur Übersicht](../rm/context_enrichment.md#supported-log-sites-and-alternatives). Die alte überlappende Vergleichstabelle wurde ersetzt; sonstige CE-Fehlerfälle bleiben dokumentiert.
- Bind ohne CE unterstützt mehrere Trice-Statements pro Zeile mit `__COUNTER__`. Ohne dieses Compiler-Makro unterstützt Bind einzelne direkte Aufrufe und einfache, eindeutig aufrufbare Makros. Bind mit CE lehnt ausgewählte Aufrufe auf gemeinsamen Zeilen und in Log-Makros auch mit `__COUNTER__` ab. Insert/Clean braucht für diese Fälle keinen Counter, mit und ohne CE.
- Die ausführliche CLI-Migration und Build-/Replay-Hinweise liegen jetzt im [RM-Migrationsabschnitt](../rm/id_reference_list_til_json.md#migrating-1x-scripts-and-projects). ABC und `tlog` waren bereits in v1.3.0 vorhanden.
- Der v2-Beta-Changelog ist auf Neuerungen, Korrekturen und kurze Migrationshinweise mit RM-Links gekürzt. Historische Einträge blieben bytegleich.
- AGENTS.md verlangt kurze, menschlich verständliche Doku, konkrete Beispiele, übersichtliche Tabellen und bei Bedarf getrennte Tabellen je Umgebung. Neue Doku und Migrationstabellen gehören in `docs/rm/`, nicht in den Changelog.
- Beide geänderten Kapitel wurden mit `mdtoc` regeneriert; lokale Nummerierung, ToCs und Anker sind aktuell. Keine Quellcode- oder Workflowänderungen in diesem Auftrag.

## Verbindliche Grenzen

- UM nicht ohne ausdrücklichen Auftrag ändern; der Nutzer bearbeitet es selbst.
- Archive und der alte RM-Monolith bleiben unverändert. Keine neuen INDEX.md, RM-search.txt, mdBook-Installation oder zusätzliche Abhängigkeiten.
- [RM_Documentation_Spec.md](RM_Documentation_Spec.md) enthält teilweise ältere Zielbilder; daraus keine vertagten Aufgaben automatisch wieder aufnehmen.
- Git-Commits, Push, Tags und Prerelease-Veröffentlichung brauchen einen ausdrücklichen Auftrag. `handson doc` allein startet keine Implementierung.

## Prüfungen

Go-Prüfungen verwendeten einen beschreibbaren temporären GOCACHE. Die Befehle unten geben die Testauswahl ohne dessen rechnerabhängigen Pfad wieder.

- `rtk proxy mdtoc check --file docs/rm/context_enrichment.md` und entsprechend `docs/rm/id_reference_list_til_json.md` — PASS.
- `rtk proxy markdownlint AGENTS.md CHANGELOG.md docs/rm/context_enrichment.md docs/rm/id_reference_list_til_json.md` — PASS. Die Übergabe wurde wegen des scratchPad-Ausschlusses separat geprüft: `rtk proxy cat docs/scratchPad/handsoff_doc.md | rtk proxy markdownlint --stdin` — PASS.
- `rtk proxy lychee --offline --include-fragments --no-progress CHANGELOG.md docs/rm/context_enrichment.md docs/rm/id_reference_list_til_json.md` — PASS, keine lokalen Link-/Fragmentfehler. Externe URLs werden offline nicht geprüft. Die Lychee-Konfiguration schließt scratchPad aus; die relativen Übergabelinks und ihre Anker wurden separat mit Python geprüft — PASS.
- `rtk go test ./scripts -run '^TestSelectedReleaseNotes$' -count=1` — PASS.
- Bereits in dieser Sitzung: `rtk env TRICE_BIND_INTEGRATION=1 go test ./internal/id ./internal/args -run '^(TestBindMVP2RebaseCompilesCAndCPP|TestBindMVP2CounterGuardsAndGeneratedInvariants|TestBindContextRejectsUnsupportedSitesWithoutPartialWrites|TestContextInsertCleanTargetToDecoder)$' -count=1 -v` — alle vier Tests PASS; reale C/C++-Kompilierung, einschließlich Insert mit CE ohne `__COUNTER__`.
- `rtk git diff --check` und Vergleich der historischen Changelog-Abschnitte — PASS.
- Kein neues `testAll`, PDF-Build, GitHub-CI-Lauf oder Hardwaretest nach diesen Dokumentationsänderungen.

## Offenes und nächster Schritt

Die PDFs wurden nach den aktuellen RM-Änderungen nicht neu erzeugt und können veraltet sein. PDFs und temporäre Build-Artefakte sind ignoriert; sie werden nicht für den Transfer benötigt. Es läuft kein Test oder Generator mehr.

Alle fünf Änderungen committen und normalerweise pushen. Auf dem anderen Rechner Branch, HEAD und Arbeitsbaum gegen diese Übergabe prüfen; danach `handson doc`. Für spätere PDF-Arbeit Node/npm, mdtoc, stitchmd und die im PDF-Skript vorgegebenen Versionen prüfen. Bei Fortsetzung der PDF-/Release-Arbeit beide PDFs aus den aktuellen Quellen neu erzeugen und Navigation, Tabellen und Seitenanfänge prüfen; keinen Prerelease automatisch veröffentlichen.
