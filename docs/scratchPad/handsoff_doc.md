# Dokumentationsübergabe

Stand: 13. Oktober 2026, Europe/Berlin. Auf dem anderen Rechner mit `handson doc` laden, Checkout prüfen und auf die nächste Nutzeranweisung warten.

## Ziel und aktueller Stand

Das RM soll auf GitHub gut und schnell lesbar sein und leicht gepflegt werden können. **Zerlegung, aktive Linkmigration und Anpassung des bestehenden Pages-Linkumschreibers sind umgesetzt und committet.** Weitere Dokumentationsarbeiten sind noch nicht beauftragt.

Der Plan steht in [RM_Documentation_Spec.md](RM_Documentation_Spec.md). Seine Bestandsangaben sind teilweise überholt: `docs/rm/` existiert; Linkmigration und Pages-Anpassung sind erledigt. Für den tatsächlichen Stand und spätere Nutzerentscheidungen ist diese Übergabe maßgeblich. Frühere Recherche liegt in [obsolete/handsoff_doc_0.md](obsolete/handsoff_doc_0.md); deren Vorschläge sind keine aktuelle Umsetzungsfreigabe.

## Git-Stand und Transfer

- Branch: `wip`.
- HEAD vor einem späteren Übergabecommit: `e332c81b7d510a76bfc4a34fc78d6dfd8c750636` — `Keep split manual navigation on GitHub Pages`.
- Arbeitsbaum vor dieser Aktualisierung: sauber. Danach ist ausschließlich diese Übergabe uncommittet.
- Der lokale Tracking-Ref `origin/wip` zeigt ebenfalls auf diesen HEAD. Kein Live-Abgleich mit dem Remote; dies beweist keinen aktuellen Serverstand. Der Agent hat nicht gepusht.
- Keine erforderlichen lokalen Messdaten, Build-Artefakte oder Hilfsskripte müssen übertragen werden. Temporäre Zerlegungs-/Linkmigrationshilfen sind nicht versioniert und für die abgeschlossene Migration keine Voraussetzung.

Die aktualisierte Übergabe manuell stagen und committen und den Commit übertragen, normalerweise durch Push. Dieser Übergabeauftrag führt kein `git add`, `git commit` oder `git push` aus.

## Bereits in Git enthalten

- `837b8cf77c9a06706a4e89b7ba8b223a17fb367e` — `Split the reference manual into browsable chapters`: 52 Kapitel sowie README, HEADER und FOOTER unter `docs/rm/`; interne Links und Medienpfade angepasst. Bei der Zerlegung wurden 16 HTML-Kommentarblöcke und 491 Codeblöcke bytegenau erhalten.
- `186df880d0a071ab2ff4dbe9d111b01b6dd79f6d` — `Point documentation links to the split reference manual`: aktive Leserlinks aus UM, Guide, README, Beispielen, aktiven Notizen sowie Go-/C- und Build-Kommentaren zeigen auf Kapitel und passende Anker. Agent-Regeln verweisen auf den modularen RM-Einstieg.
- `e332c81b7d510a76bfc4a34fc78d6dfd8c750636` — `Keep split manual navigation on GitHub Pages`: Pages verarbeitet Doku-Einstieg, UM, Guide und RM-Kapitel ohne Monolithen. Dokumentationslinks werden relative HTML-Links; Quelltextlinks führen zur GitHub-Dateiansicht. Bilder, Anker, Query-Strings, Codebeispiele und Kommentare bleiben erhalten. Fehlende Einstiegsdateien werden vor dem Umschreiben gemeldet; Verhaltenstests sind ergänzt.

Die vom Nutzer testweise umbenannte Gesamtdatei ist als [docs/_TriceReferenceManual.md](../_TriceReferenceManual.md) unverändert in Git erhalten. SHA-256: `417c8384ae23bc06ec3780e1c8babaf41019413fa72d57807aed7330fa93ee5c`. Der bisherige Pfad `docs/TriceReferenceManual.md` existiert nicht mehr. Eine spätere Archivierung wurde noch nicht ausgeführt.

## Verbindliche Entscheidungen und Grenzen

- Kapitel-Markdown ist die einzige laufend gepflegte RM-Quelle. README enthält die manuell gepflegte Hauptkapitelreihenfolge; keine zweite Sortierdatei oder Kapitelduplikate einführen.
- Hauptüberschriften und Dateinamen bleiben unnummeriert. Autoren schreiben normale Markdown-Überschriften; mdtoc erzeugt lokale Unterkapitelnummern, Anker und ToCs. Der Contents-Details-Container verwendet `numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto`.
- Auskommentierte Teststellen und vorhandene Inhalte erhalten. Die Zerlegung war keine redaktionelle Neufassung.
- **Noch kein INDEX.md, keine RM-search.txt.** Der geplante Stichwortindex wird zentral redaktionell gepflegt, ohne verpflichtende Begriffsmakros in Kapiteln. Format, Werkzeug und Aktualisierung sind offen.
- mdBook bleibt ein optionaler HTML-/Volltextsuch-Kandidat aus denselben Quellen. Keine Installation, zusätzliche Website oder Veröffentlichung ist beauftragt. Eine zusammengefügte Markdown-Lesefassung ist zurückgestellt.
- PDF-Erzeugung aus Kapiteln und Release-/Installations-/Audit-Umstellung sind ausdrücklich vertagt. Die Pages-Linkumschreibung wurde dagegen separat freigegeben und erledigt.
- UM und Guide dürfen für diese Migration auf neue RM-Ziele verlinken; weitere Inhaltsänderungen oder Konvertierungen sind nicht freigegeben. Der Nutzer bearbeitet das UM selbst.
- Archive bleiben unverändert. Keine Kompatibilitätsdatei am alten RM-Pfad oder zusätzliche Migration ohne Auftrag. [Agent-Regeln](../../AGENTS.md) beachten; Lesen ist dauerhaft erlaubt.

## Relevante Dateien und offene Integrationsfragen

- Quellen: [RM-Einstieg](../rm/README.md), [UM](../TriceUserManual.md), [Guide](../TriceGuide.md), [Doku-Einstieg](../README.md), [Medien/CLI-Hilfe](../ref/).
- Pages: [pages.yml](../../.github/workflows/pages.yml); Tests in [portability_test.go](../../scripts/portability_test.go).
- [mdtoc-Pflege](../../scripts/_310_refresh_trice_user_manual.sh) und [PDF-Erzeugung](../../scripts/_320_generate_trice_user_manual_pdf.sh) enthalten weiterhin `docs/TriceReferenceManual.md` als Eingabe. Sie können mit der jetzigen Umbenennung nicht unverändert laufen. Auch `format_repo.sh` und der Manual-Formatcheck verwenden die bisherige Pflege. Nicht durch eine Ersatz-Gesamtdatei umgehen.
- [GoReleaser](../../.goreleaser.yaml), [Snapshot-Prüfung](../../scripts/_610_test_goreleaser_snapshot.sh) sowie Release-/Installations-/Audit-Workflows erwarten weiterhin das bisherige RM-PDF. Diese Artefaktverträge wurden nicht geändert.
- Verbliebene alte Dateinamen in historischen Beispielen, Quellenangaben, Plantexten und Testfixtures sind keine offenen aktiven RM-Hyperlinks.
- Die Wirkung der kleineren Kapitel auf mobile Ladezeit wurde noch nicht reproduzierbar gemessen.

Lychee 0.24.2 war bereits installiert; keine zusätzliche Abhängigkeit wurde eingeführt. Es prüft auch URLs in anderen Textdateien. Relative Markdown-Links in Go/C erkennt der normale Textscan jedoch nicht: Quelltext über stdin als Markdown mit passendem Basisverzeichnis prüfen (`--default-extension md --base-url <Verzeichnis-URI> -`). Dies bestand für die geänderten Links in `src/triceRx.h` und `internal/id/insertIDs.go`; der RM-Pfad im erzeugten C#-Kommentar wurde auf Existenz geprüft. Diese Zusatzprüfung ist noch nicht in den Repository-Linkcheck integriert. Siehe [Dateiverarbeitung](https://lychee.cli.rs/guides/preprocessing/) und [CLI-Optionen](https://lychee.cli.rs/guides/cli/).

## Durchgeführte Prüfungen

Diese Ergebnisse gelten für die implementierten Änderungen vor den letzten beiden Commits, nicht als Tests auf dem anderen Rechner:

- `lychee --offline --include-fragments --no-progress --format json .` — PASS, Exitcode 0: 2.939 erfolgreiche Verweise, 0 Fehler; zuvor 150 Fehler. 491 Verweise ausgeschlossen. Externe URLs sowie ausgeschlossene Scratchpad-/Archivbereiche wurden damit nicht vollständig geprüft.
- `rtk proxy markdownlint .` — PASS. `rtk proxy actionlint .github/workflows/pages.yml` — PASS. `rtk git diff --check` — PASS.
- `mdtoc check --file <Dokument>` für UM, Guide und alle Kapitel mit mdtoc-Container — PASS für 54 Dokumente.
- `go test ./scripts -run 'TestManualPages|TestManualPDFNavigationOutlivesTheRenderer' -count=1` — PASS.
- `go test ./internal/id -run '^Test(Generate|ToListTil|AbcGenerate)' -count=1` — PASS. Beide Go-Aufrufe erfolgten über `rtk env` mit GOCACHE/GOMODCACHE in temporären Verzeichnissen.
- Isolierte Kopie der echten Doku, ausgeführt mit den beiden tatsächlichen Workflow-Shellblöcken: 58 Dokumente ohne Monolith vorbereitet; alle 682 Code-/Kommentarblöcke bytegenau erhalten; keine aktiven relativen Markdown-Dokumentziele verblieben in den vorbereiteten Seiten.
- Kodierung und Zeilenenden der 57 geänderten Textdateien erhalten, einschließlich des vorhandenen BOM in CHANGELOG; umbenannter Monolith bytegenau identisch.

Kein vollständiger `testAll`-Lauf wegen der noch nicht umgestellten Manual-Pflege. Kein vollständiger Jekyll-Build, Pages-Deployment, neuer PDF-/Release-Build oder Performancebenchmark. Kein laufender Test muss für den Rechnerwechsel beendet werden.

Prüfung dieser aktualisierten Übergabe: `rtk proxy markdownlint --config ../.markdownlint.yaml scratchPad/handsoff_doc.md` aus `docs/` und `rtk git diff --check` — beide PASS, Exitcode 0. Ein lesendes Python-Prüfsnippet bestätigte UTF-8 ohne BOM, LF-Zeilenenden, keine nachgestellten Leerzeichen und 15 existierende relative Linkziele. Nur diese Datei wurde verändert; Index und HEAD blieben unverändert.

## Nächster Schritt auf dem anderen Rechner

Nach Übernahme des Git-Stands `handson doc` ausführen. Branch, HEAD, Arbeitsbaum, RM-Einstieg, unveränderte Gesamtdatei und Übergabe prüfen. Werkzeuge des neuen Rechners gesondert prüfen; bisherige Ergebnisse nicht als dortige Tests ausgeben. Danach auf die nächste Nutzeranweisung warten.

Der nächste sinnvolle Implementierungsumfang wäre die Kapitelpflege mit mdtoc, weil sie aktuell Formatierung und Gesamtprüfung blockiert. Das ist ein Vorschlag, keine Freigabe. Such-/Indexformat, reproduzierbare Performanceprüfung, spätere Archivierung und PDF-/Release-Verträge jeweils konkret entscheiden; keine dieser Arbeiten automatisch starten.
