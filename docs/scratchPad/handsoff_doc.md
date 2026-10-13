# Dokumentationsübergabe

Stand: 13. Oktober 2026, Europe/Berlin. Auf dem anderen Rechner mit `handson doc` laden, den Checkout prüfen und anschließend auf die nächste Nutzeranweisung warten.

## Ziel und Git-Stand

Die Kapitelpflege ist umgesetzt. Der neue PDF-Build erzeugt das Gesamt-RM aus den Kapitelquellen und läuft lokal; die Unterkapitelnummerierung im Gesamt-PDF ist noch offen. Der Nutzer verlangt jetzt ausschließlich diese Übergabe und Commits aller lokalen Änderungen. Keine weitere Implementierung, PDF-Neugenerierung oder langen Tests vor dem Rechnerwechsel.

- Branch: `wip`.
- Ausgangs-HEAD vor den aktuellen Commits: `7de67c5b5e6abebf6bd56fad9ce15e80f9ef1545` — `wip RM doc`.
- HEAD der gesicherten Änderungen vor dem abschließenden Übergabecommit: `f6a83a5f8dc38b79032a4cc87dad41ae257bea8b` — `Update the G0B1 Windows logger port example`. Der Checkout auf dem anderen Rechner soll zusätzlich den nachfolgenden Übergabecommit enthalten.
- Alle bisherigen Arbeitsbaumänderungen sind in sieben Themen-Commits gesichert: PDF-Pipeline `72eeee94`, Kapitel-Trennlinien `c423f920`, Prerelease-Einstellung `faa0322f`, UM samt Bildern `83ec3ddf`, F030-Bindingdaten `c29050f2`, Beispiel-Diagnostik samt `demoTIL.json` `c50d5f56`, G0B1-Loggerbeispiel `f6a83a5f`. Vor dem Übergabecommit ist ausschließlich diese Datei noch uncommittet.
- Kein Push beauftragt oder ausgeführt. Für den anderen Rechner müssen die neuen Commits noch übertragen werden, normalerweise durch Push. Lokale Tracking-Refs beweisen keinen aktuellen Remote-Stand.

## Verbindliche Entscheidungen

- [docs/rm/](../rm/README.md) enthält die gepflegten RM-Quellen. README bestimmt die manuelle Kapitelreihenfolge; HEADER und FOOTER ergänzen die 52 Hauptkapitel.
- Hauptüberschriften und Dateinamen der Quellen bleiben unnummeriert. mdtoc erzeugt die kapitelinternen ToCs, Unterkapitelnummern und Überschriftenanker. [Die Kapitelpflege](../../scripts/_310_refresh_trice_user_manual.sh) ist bereits im Ausgangs-HEAD enthalten.
- Im Gesamt-PDF sollen die Hauptkapitel automatisch nummeriert werden. Das ist umgesetzt. Die vollständige Hierarchie der Unterkapitel, etwa `19.1` und `19.1.1`, ist noch nicht umgesetzt.
- Vor `<!-- mdtoc -->` und nach `<!-- /mdtoc -->` stehen jetzt in allen 52 Kapiteln jeweils `---` und eine Leerzeile. Der Nutzer hat dies auch im UM eingefügt. Die Marker selbst bleiben unverändert.
- Zwischenstände gehören nach `temp/manual-pdf/`. Vor jeder Generierung wird ausschließlich dieses Unterverzeichnis geleert; sonstige Dateien in `temp/` bleiben erhalten.
- Im PDF gehen Back-to-Top-Links zum Dokumentanfang. Jedes neue Hauptkapitel beginnt auf einer neuen Seite. Die Kapitelquellen behalten ihre lokale Navigation.
- Die Release-Artefakte heißen weiterhin `docs/TriceReferenceManual.pdf` und `docs/TriceUserManual.pdf`.
- Keine Dateiumbenennungen, Archivierung, INDEX.md, RM-search.txt oder mdBook-Installation in diesem Auftrag. Archive und [der bisherige Monolith](../_TriceReferenceManual.md) bleiben unverändert. Frühere Pläne sind keine zusätzliche Umsetzungsfreigabe.

## Antwort zur Unterkapitelnummerierung und mdtoc

Ich habe die Unterkapitelnummern unverändert übernommen, weil du dazu ausdrücklich „Ich frage nur“ geschrieben hattest. Für das Gesamt-PDF ist eine vollständige Nummerierung wie `19.1` sinnvoller. Die Beschränkung auf Hauptkapitelnummern und der eigene Nummerierungsschritt waren meine Entscheidung; den einfacheren Weg über `mdtoc` hätte ich früher prüfen sollen.

Der aktuelle Zwischenstand enthält bereits viel HTML, weshalb `mdtoc` dort die Unterüberschriften nicht vollständig erfassen kann. Das ist eine Folge meines Aufbaus, kein guter Grund, `mdtoc` grundsätzlich auszuschließen. Die sachgerechte nächste Prüfung ist deshalb: strukturelle Überschriften beim Zusammenfügen als Markdown erhalten, lokale generierte ToCs/Nummern/Anker in temporären Kopien entfernen und erst auf dem Gesamt-Markdown `mdtoc` für die globale Hierarchie, ToC und Anker laufen lassen. Dabei müssen die bestehenden Querlinks, Bilder, Codeblöcke und Kommentare erhalten bleiben. Einfach `mdtoc` auf den heutigen HTML-lastigen Zwischenstand anzuwenden reicht nicht.

Diese Erklärung ist gespeichert; die Implementierung bleibt vor dem Rechnerwechsel unverändert. Die konkrete Umstellung ist der nächste Vorschlag, keine automatisch fortzusetzende Arbeit.

## Implementierter PDF-Build

- Einstieg: [scripts/_320_generate_trice_user_manual_pdf.sh](../../scripts/_320_generate_trice_user_manual_pdf.sh). Verwendet stitchmd **0.9.0**, md-to-pdf **5.2.4** und Puppeteer **24.31.0**. Beide PDFs werden mit `bash scripts/_320_generate_trice_user_manual_pdf.sh` erzeugt.
- [scripts/_prepare_reference_manual.cjs](../../scripts/_prepare_reference_manual.cjs) liest die README-Reihenfolge, entfernt komplette lokale Contents-Container und bereitet temporäre Kapitel vor. Unterüberschriften und Kapitelinhalt werden derzeit früh zu HTML gerendert; die Hauptüberschrift bleibt für stitchmd als Markdown erhalten. Kapitelcontainer verhindern, dass fehlerhaft abgeschlossene HTML-Listen nachfolgende Kapitel einrücken.
- `stitchmd -no-toc` erzeugt `temp/manual-pdf/TriceReferenceManual.md`. Der Helper erzeugt eine globale verlinkte ToC, nummeriert die 52 Hauptkapitel und prüft eindeutige Anker und interne Ziele. `--verify` legt auch das zusammengefügte HTML ab.
- Querlinks zwischen enthaltenen Kapiteln werden PDF-interne Links. Bilder werden lokal eingebunden und vorab auf Existenz geprüft. Andere Repository-Links werden GitHub-Links auf den Build-Commit. Namensräume vermeiden kollidierende Kapitelanker.
- [scripts/_manual_pdf_config.cjs](../../scripts/_manual_pdf_config.cjs) öffnet Contents-Details für den Druck und korrigiert Back-to-Top-Ziele. CSS und PDF-Optionen werden als einzeilige CLI-Argumente übergeben, weil der Windows-npm-Aufruf mehrzeilige Argumente zuvor unvollständig weitergab. PDF-Lesezeichen sind aktiviert.
- [Release-Workflow](../../.github/workflows/goreleaser.yml) und [Installationsprüfung](../../.github/workflows/install-checks.yml) installieren dieselbe stitchmd-Version. Verhaltenstests stehen in [scripts/portability_test.go](../../scripts/portability_test.go).
- In [der GoReleaser-Konfiguration](../../.goreleaser.yaml) steht unter `release:` jetzt `prerelease: true`. Das markiert während der Beta-Phase alle veröffentlichten Releases als Prerelease; vor einer stabilen Veröffentlichung muss diese Einstellung neu entschieden werden. Vorgeschlagener Tag: `v2.0.0-beta-1`. Kein Tag und kein Release wurden erstellt.

## Prüfungen und Grenzen

Bereits vor diesem Übergabeauftrag bestanden; keine Behauptung über Tests auf dem anderen Rechner:

- `rtk go test ./scripts -run '^Test(ManualPDFGeneration|ManualPDFNavigation|ReferencePDF)' -count=1` — PASS, 10 gezielte Tests. Prüft unter anderem README-Reihenfolge, temporäre Bereinigung, Quellenintegrität, Code/Kommentare, Bilder, Anker, Navigation und fehlerhafte Eingaben.
- `bash scripts/_310_refresh_trice_user_manual.sh check` — PASS nach den Trennlinien in den 52 Kapiteln. Die zuletzt gespeicherten parallelen UM-Inhaltsänderungen erfolgten danach.
- `goreleaser check` — PASS für `prerelease: true`.
- Shellsyntax, Node-Syntax beider CJS-Dateien, Go-Formatierung, Shellformatierung, ShellCheck, actionlint für die beiden Workflows und `git diff --check` — PASS.
- Tatsächliche lokale PDF-Erzeugung — PASS. Letzter Build am 13. Oktober um 16:12:54 Europe/Berlin: RM 455 Seiten, 629 Lesezeichen, 942 interne PDF-Linkannotationen; UM 9 Seiten, 15 Lesezeichen, 14 interne PDF-Linkannotationen.
- PDF-Prüfung — alle 52 Hauptkapitel nummeriert und jeweils auf einer neuen Seite; alle 69 Back-to-Top-Links im RM führen auf die erste Seite; keine horizontal abgeschnittenen Texte oder lokalen/temp/file-Linkziele. Repräsentative Seiten visuell geprüft.
- Zwischenstand — 891 interne Links und 1.258 eindeutige Anker geprüft; Text aller 495 Codeblöcke bis ins zusammengefügte HTML erhalten. Alle 52 neuen Kapitel-ToC-Trennlinien werden beim Entfernen des jeweiligen lokalen ToC mit entfernt.
- Keine vollständigen C-/Integrationstests, kein `testAll`, kein GitHub-CI-/Release-Lauf. Die parallelen Nutzeränderungen an Beispielen und JSON-Daten wurden nur für den Commit gesichtet, nicht neu funktional getestet.
- Übergabeprüfung — UTF-8 ohne BOM, LF, keine nachgestellten Leerzeichen und alle 13 relativen Linkziele geprüft; `git diff --check` und die Prüfungen der gestagten Diffs bestanden. Vor dem abschließenden Übergabecommit keine anderen uncommitteten Dateien.

## Lokale Artefakte und parallele Nutzeränderungen

PDFs und `temp/manual-pdf/` sind regulär ignorierte Build-Artefakte und werden nicht erzwungen in Git aufgenommen. Temporäre PDF-Prüfwerkzeuge und Bilder unter `temp/` sind keine neue Projektabhängigkeit und werden nicht benötigt, um die PDFs zu bauen. Kein laufender Test oder Generator muss für den Rechnerwechsel beendet werden.

Das zuletzt erzeugte UM-PDF enthält noch nicht alle danach gespeicherten UM-Änderungen, insbesondere die Trennlinien und `FirstDiffView.png`. Die aktuelle Markdown-Quelle und beide neuen Bilder sind committet; auf dem anderen Rechner erneut generieren.

Parallel bearbeitete und ausdrücklich mit zu sichernde Dateien: [UM](../TriceUserManual.md), [demoTIL-Bild](../ref/demoTIL.json.png), [Vergleichsbild](../ref/FirstDiffView.png), `demoTIL.json`, `examples/F030_inst_uart/Core/Src/main.c`, dessen `til.json`, `examples/G0B1_inst/Core/Inc/triceConfig.h` und `examples/exampleData/triceExamples.c`. Inhalte und vorhandene Zeilenenden bleiben unverändert. Der geänderte F030-Sidecar-Include verweist auf eine generierte, nicht versionierte Datei; auf dem anderen Rechner bei Bedarf mit der bestehenden Bind-Pflege regenerieren, kein spontaner Reparaturauftrag.

## Nächste Schritte auf dem anderen Rechner

Die neuen Commits übertragen und `handson doc` ausführen. Branch, HEAD, Arbeitsbaum und relevante Quellen gegen diese Übergabe prüfen; Node/npm, stitchmd 0.9.0, mdtoc und GoReleaser dort gesondert prüfen. Danach auf die nächste Nutzeranweisung warten.

Als nächster Implementierungsschritt bietet sich die vollständige PDF-Unterkapitelnummerierung mit `mdtoc` auf dem zusammengefügten Markdown an. Anschließend beide PDFs einschließlich der neuesten UM-Änderungen neu erzeugen und erneut Querlinks, Codeblöcke, Nummerierung und Seitenanfänge prüfen. Ein Prerelease folgt erst auf gesonderten Auftrag.
