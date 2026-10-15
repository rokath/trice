# Dokumentationsübergabe

Stand: 12. Oktober 2026, Europe/Berlin. Fortsetzen mit `handson doc`.

Diese eigenständige Übergabe ersetzt `handsoff_rm_performance.md` und `handsoff_rm-speedup2.md` und enthält die anschließende OSS-Recherche. Die beiden Vorgänger werden zum Fortsetzen nicht benötigt. Der Nutzer möchte sie nach der Zusammenführung selbst löschen.

## Auftrag und verbindliche Entscheidungen

Ziel: RM-Verweise sollen insbesondere auf schwächeren Handys schnell zur richtigen, lesbaren, stabil positionierten und bedienbaren Zielstelle führen. Gute Lesbarkeit und einfache langfristige Pflege sind ebenso wichtig wie Geschwindigkeit. Manuelle Handytests gelten als nicht zuverlässig reproduzierbar.

- **GitHub bleibt das Repository. Das Lesen darf auf einer eigenen Website erfolgen**, wenn das die Lösung vereinfacht. GitHub Pages ist dafür ein möglicher Veröffentlichungsort. Die ältere Vorgabe, GitHub-Dateiansicht und Pages gleich stark zu priorisieren, ist damit überholt.
- Ausschließlich Open Source; vorhandene Werkzeuge bevorzugen. Keine vorschnelle Eigenentwicklung eines Parsers oder Dokumentationssystems.
- Inhalte einmal pflegen. Parallel gepflegte Kapitelkopien irritieren; skriptgesteuert aus Dokumentationsquellen erzeugte Ausgaben sind ausdrücklich akzeptabel.
- UM und TriceGuide müssen nicht Markdown bleiben. Eine Formatkonvertierung darf durch einen KI-Agenten erfolgen; ihre einmaligen Kosten sind kein Grund, bessere Formate auszuschließen.
- Inhaltsverzeichnisse und abgeleitete Linkübersichten sollen automatisch gepflegt werden. Kleine Seiten sind ein möglicher Lösungsweg, keine eigenständige Nutzeranforderung.
- Quellen und Lesefassung müssen klar erkennbar sein. Dokumentation auf schnelle Erfassbarkeit und gute Verständlichkeit ausrichten.
- **Freigegeben ist derzeit nur Analyse und diese Zusammenführung.** Kein Startsignal für Prototyp, Formatmigration, neue Abhängigkeiten, Änderungen an UM/RM/Guide, Build-Skripten oder Veröffentlichung.
- Alle Lesezugriffe sind dauerhaft genehmigt; technisch erzwungene Plattformfreigaben bleiben möglich. Geltende [Agent-Regeln](../../AGENTS.md) lesen. Archive unter `obsolete` bleiben unberührt.

## Checkout und bereits erledigte Arbeit

- Branch: `wip`.
- HEAD: `ab80b5a82591c99d299a8130a200c4ec4b3f292e` — `Merge branch 'main' into wip`.
- Vor dieser Zusammenführung war der Arbeitsbaum sauber. Gegenüber dem lokal vorhandenen `origin/wip` ist `wip` zehn Commits voraus; kein Fetch oder Push ausgeführt. Diese Tracking-Angabe beweist keinen aktuellen Remote-Stand.
- Neue lokale Änderung dieses Auftrags: ausschließlich `docs/scratchPad/handsoff_doc.md`. Die Vorgänger bleiben zunächst erhalten. Kein Staging, Commit, Push oder Deployment.
- Die Medienoptimierung ist bereits in Git: `276e3b05`, `docs: clarify payload limits and add reference previews`. Fünf statische Vorschauen verlinken auf erhaltene Originalanimationen. Der Commit ist in `wip` und im lokal vorhandenen `origin/main` enthalten. Nicht erneut als unerledigte Arbeit beginnen.
- Die Freigabe aller Lesezugriffe und die Regel zur verständlichen Dokumentation sind bereits in `AGENTS.md` enthalten; keine weitere Regeländerung nötig.

## Befunde und Grenzen der Diagnose

Am aktuellen Checkout am 12. Oktober erneut lesend geprüft:

| Dokument / Prüfung | Ergebnis |
| --- | --- |
| RM | 843.505 Bytes, 13.295 Zeilen, 53 Hauptkapitel |
| UM | 26.197 Bytes, 299 Zeilen |
| TriceGuide | 24.352 Bytes, 166 Zeilen |
| Explizite RM-IDs | 559; keine doppelten IDs |
| Direkte UM-Verweise auf RM-Fragmente | 28; alle Quelltext-Ziel-IDs vorhanden |

Vorhandene IDs beweisen weder einen erfolgreichen Ankersprung noch die Bedienbarkeit im Browser. Explizite HTML-IDs und automatisch aus nummerierten Überschriften erzeugte Slugs sind verschieden; bei einer Konvertierung nicht gleichsetzen. Ein Fragmentverweis auf das große RM lädt weiterhin das gesamte Dokument.

Die erste Untersuchung einer damaligen öffentlichen Pages-Fassung ergab rund 1,35 MB HTML, 21.131 Starttags und 70 Bild-Elemente; die zugeordneten Bilddateien umfassten rund 16,4 MB. Keine Bild-Elemente hatten `loading="lazy"` oder gleichzeitig Breite und Höhe. Das sind **historische Ressourcenbefunde vor der Medienoptimierung**, keine aktuellen Netzwerk- oder Laufzeitmessungen. Große Dokumentstruktur und nachträgliche Layout-Verschiebungen sind plausible Ursachen, aber noch nicht abschließend nachgewiesen. GitHubs eigenes Rendering wurde nicht profiliert.

Aktueller Build: [Pages-Workflow](../../.github/workflows/pages.yml) mit Jekyll, Cayman und kramdown; Veröffentlichung bei Push auf `main` oder manuellem Workflow-Start. Der Workflow schreibt Links der beiden Manuals für Pages um. [Manual-Pflege](../../scripts/_310_refresh_trice_user_manual.sh) verwendet `mdtoc` für ToC, Nummerierung und Anker; PDF-Erzeugung ist separat. Noch keine neue Werkzeugkette installiert oder konfiguriert.

**Ausstehend:** reproduzierbare Browsermessungen, geprüfte mobile Lesefassung, vollständige Konvertierungs- und Linkprüfung sowie Verifikation der aktuell veröffentlichten Fassung. Keine Verbesserung in Sekunden behaupten. Ein früher Pandoc-Chunked-HTML-Versuch scheiterte an einer nicht lokal vorhandenen Bildressource; er war kein erfolgreicher Kompatibilitätstest.

## Ergebnis der OSS-Recherche

Gemeinsames bewährtes Prinzip: überschaubare Themenquellen, daraus erzeugte Website und gegebenenfalls weitere Ausgaben. Nicht das Dateiformat allein entscheidet über schnelles Nachschlagen.

| Projekt / Modell | Befund | Bewertung für Trice |
| --- | --- | --- |
| [Rust-Buch](https://github.com/rust-lang/book): Markdown + mdBook | Einzelne Kapitel bilden ein zusammenhängendes Buch. | Bevorzugter Kandidat: passende Handbuchfunktionen bei überschaubarer Werkzeugkette. |
| [Zephyr](https://docs.zephyrproject.org/latest/contribute/documentation/generation.html): reStructuredText + Sphinx | HTML/PDF aus Quellen, ergänzt durch generierte API- und Konfigurationsreferenzen. | Starke Alternative, wenn solche Referenzen und mehrere Ausgabeformate wichtiger werden. [MyST](https://myst-parser.readthedocs.io/en/latest/syntax/cross-referencing.html) ermöglicht Markdown als Sphinx-Quelle. |
| [Spring Boot](https://github.com/spring-projects/spring-boot/wiki/Antora): AsciiDoc + Antora | Thematische Module und mehrere Versionen auf einer Website. | Leistungsfähig für eine größere versionierte Dokumentationslandschaft; derzeit zusätzlicher Struktur- und Pflegeaufwand. |

**Empfehlung, noch keine Nutzerentscheidung:** eine statisch erzeugte Handbuchwebsite, bevorzugt mdBook, mit modularen Quellen und GitHub Pages als möglichem Leseort. UM, Guide und RM bleiben erkennbare Einstiege; das RM wird nach zusammenhängenden Themen gegliedert. UM und Guide sind bereits klein und müssen dafür nicht ebenfalls in viele Dateien zerlegt werden. Markdown wird wegen einfacher laufender Pflege vorgeschlagen, ist aber keine bindende Formatvorgabe.

[mdBook](https://rust-lang.github.io/mdBook/format/configuration/renderers.html) bietet Suche mit Abschnittstreffern, Kapitel- und Abschnittsnavigation, eine Gesamtdruckansicht und Weiterleitungen einschließlich alter Fragmente. Ein hochwertiger separater PDF-Export braucht einen zusätzlichen Exportweg. Relative Markdown-Dateilinks werden für HTML umgesetzt; siehe [Linkverhalten](https://rust-lang.github.io/mdBook/format/markdown.html).

mdBook benötigt eine [SUMMARY.md](https://rust-lang.github.io/mdBook/format/summary.html) für Reihenfolge und Hierarchie. Sie entsteht nicht automatisch durch Dateisuche. Vorgeschlagen ist eine einzige Strukturdefinition für Reihenfolge/Gruppierung, aus der Übersichten erzeugt werden; Titel und Anker stammen aus den Quellen. Das genaue Verfahren ist offen, nicht bereits implementiert. KI unterstützt die einmalige Konvertierung und redaktionelle Arbeit; der regelmäßige Build soll deterministisch ohne KI laufen.

**Zurückgestellte Wege, jeweils nur die wesentliche Begründung:**

- Bestehendes Jekyll mit [Just the Docs](https://just-the-docs.com/docs/navigation/): ernsthafte Alternative mit geringer Umstellung; Navigation und Suche vorhanden. mdBook erscheint für zusammenhängende Handbücher geschlossener. Kein Geschwindigkeitsvorteil eines Generators ist bereits gemessen.
- Monolithisches RM als Quelle, daraus Kapitel-HTML oder Kapitel-Markdown erzeugen: legitimer Übergang ohne redaktionelle Duplikation, aber die große Bearbeitungsdatei und Regeln zur Zerlegung bleiben. Zusätzliche `docs/split`-Ansichten sind keine priorisierte Dauerlösung.
- Material for MkDocs: bei [uv](https://github.com/astral-sh/uv/blob/main/mkdocs.yml) eingesetzt; für eine neue Festlegung wegen [Wartungsmodus und Übergang zu Zensical](https://squidfunk.github.io/mkdocs-material/blog/2025/11/05/zensical/) zurückgestellt. Zensical wurde nicht am Trice-Inhalt erprobt.
- Weitere Medienoptimierung, Lazy Loading oder `content-visibility` allein: können helfen, lösen aber nicht verlässlich das gezielte Nachschlagen im großen RM. PDF als primärer Leseort ist für mobile Abschnittsaufrufe weniger geeignet.
- Bookdown und kleine Markdown-Splitter: zusätzlicher Werkzeugaufwand bzw. nicht nachgewiesene vollständige Behandlung von Trice-Ankern, Querverweisen und Bildern; kein erkennbarer Vorteil für den bevorzugten Weg.

## Möglicher zukünftiger docs-Ordner

**Entwurf zur Entscheidung, kein angelegter Ordnerbaum und kein freigegebener Umbau.** Dateinamen und Themenauswahl sind Beispiele. Legende: `[S]` einmal gepflegte Quelle, `[K]` Konfiguration, `[G]` automatisch erzeugt, `[A]` bestehendes Archiv.

```text
docs/
├── README.md                         [S] Einstieg zur Lesefassung und Pflege
├── book.toml                         [K] mdBook-Konfiguration
├── structure.yml                     [K] Reihenfolge und Gruppierung; Vorschlag
├── src/                              [S] einzige bearbeitete Dokumentation
│   ├── SUMMARY.md                    [G] Kapitelübersicht aus der Struktur
│   ├── index.md                      [S] Startseite
│   ├── user-manual.md                [S] UM: kurzer Lernpfad
│   ├── trice-guide.md                [S] Guide: Orientierung und Verweise
│   └── reference/                    [S] RM: technische Themen
│       ├── index.md                  [G] Themenübersicht
│       ├── bind.md                   [S]
│       ├── rtt.md                    [S]
│       ├── uart.md                   [S]
│       ├── binary-encoding.md        [S]
│       └── ...                       [S] weitere zusammenhängende Themen
├── ref/                              [S/G] bestehende Medien und CLI-Hilfe
├── scratchPad/                       interne Arbeit; keine Lesernavigation
│   ├── handsoff_doc.md               diese konsolidierte Übergabe
│   ├── ...                           andere vorhandene Pläne bleiben
│   └── obsolete/                     [A] Geschichte; nicht umorganisieren
└── _build/                           [G] lokal/CI, keine redaktionelle Pflege
    ├── html/                         Website für GitHub Pages
    └── exports/                      optionale Gesamtfassung/PDF
```

Die Texte werden ausschließlich unter `src/` gepflegt; generierte Übersichten sind als solche gekennzeichnet. `ref/` zeigt den bestehenden Medien-/Hilfebestand: Medien und automatisch erzeugte CLI-Hilfe sind getrennt zu behandeln. Wie dessen Ressourcen in den mdBook-Build gelangen, muss im Prototyp geprüft werden; der Baum ist noch keine ausführbare Konfiguration. Das frühere einzelne RM würde nach einer genehmigten Umstellung durch Themenquellen und bei Bedarf eine erzeugte Gesamtfassung ersetzt. Keine parallel bearbeitete Vollfassung neben den Themen.

```text
[S] Texte + Medien + [K] Struktur
                │
                ▼
       deterministischer Build
                │
                ├──► [G] Themenwebsite ──► Lesen auf GitHub Pages
                └──► [G] Gesamtfassung / optionales PDF

GitHub-Repository: Quellen, Änderungen und Reviews
Website: lesbare Navigation, Suche und direkte Abschnittsaufrufe
```

## Prüfung vor einer vollständigen Umstellung

Nach einem ausdrücklichen Startsignal zuerst Binary Encoding, Bind und RTT mit unverändertem Inhalt als begrenzten Prototyp verwenden. Die am 11. Oktober erhobenen Quellgrößen lagen bei rund 16, 59 und 39 kB. Struktur, Lesbarkeit und Geschwindigkeit an diesen tatsächlichen Inhalten prüfen, bevor die ganze Dokumentation konvertiert wird.

- Playwright/Chromium ist als reproduzierbarer Prüfstand vorgeschlagen, noch nicht eingerichtet. Feste Dokumentstände, Browserversion, Rechner, Bildschirmprofil sowie CPU-/Netzdrosselung verwenden. Ein mobiles Viewportprofil allein simuliert keine Handy-Rechenleistung.
- Frühe, mittlere und späte Zielstellen; Direktaufruf, Klick vom UM und Zurücknavigation. Leeren und gefüllten Cache getrennt prüfen, beispielsweise 20 sequenzielle Läufe; Median, langsamere Durchläufe und Fehlerzahl berichten.
- Zeit bis Zielüberschrift und erster Absatz lesbar, stabil im sichtbaren Bereich und bedienbar sind messen. Der Test darf nicht selbst zum Ziel scrollen oder einen fehlgeschlagenen Sprung reparieren. HTTP 200, vorhandene ID und bloße DOM-Sichtbarkeit reichen nicht.
- Mobile Schrift/Zeilenlänge, Navigation, Code und Tabellen visuell kontrollieren. Bildabmessungen und spätere Positionsverschiebungen prüfen. Grundtext soll als normales HTML lesbar sein.
- Kontrollierte lokale Ausgabe und tatsächlich veröffentlichte Website getrennt prüfen. GitHub-Dateiansicht ist ein zusätzlicher Nutzungsweg, aber keine gleichrangige Voraussetzung mehr. Lighthouse-Gesamtwerte ersetzen keine Abschnittsprüfung.
- ToCs, Bildpfade, Querverweise und sämtliche übernommenen expliziten Anker prüfen. Wichtige alte Website-Fragmente lassen sich auf neue Ziele weiterleiten. Für GitHub-Datei-URLs gibt es keine eigene Redirect-Logik; eine kleine Verweisübersicht wäre ein möglicher Umgang mit alten Links, noch keine freigegebene Migration.

Primärquellen zum Prüfstand: [Playwright-Emulation](https://playwright.dev/docs/emulation), [Viewport-Prüfung](https://playwright.dev/docs/api/class-locatorassertions#locator-assertions-to-be-in-viewport), [CPU-/Netzdrosselung](https://developer.chrome.com/docs/devtools/settings/throttling).

## Nächster Schritt und Transfer

Auf dem anderen PC mit `handson doc` diese Übergabe laden, Agent-Regeln, Branch, HEAD und Arbeitsbaum gegenprüfen und dann auf die nächste Nutzeranweisung warten. Die Empfehlung und der Ordnerbaum sind Entscheidungshilfen, keine Umsetzungsfreigabe. Offen sind insbesondere Generatorentscheidung, endgültige Themenstruktur, automatische Übersichtserzeugung, Umgang mit alten Links und messbare Abnahmekriterien.

Aktuelle Quellen: [RM](../TriceReferenceManual.md), [UM](../TriceUserManual.md), [Guide](../TriceGuide.md), [Medien/CLI-Hilfe](../ref/), [Pages-Workflow](../../.github/workflows/pages.yml), [Jekyll-Konfiguration](../../_config.yml), [Manual-Pflege](../../scripts/_310_refresh_trice_user_manual.sh), [PDF-Erzeugung](../../scripts/_320_generate_trice_user_manual_pdf.sh).

Prüfung dieser Übergabe: `rtk proxy markdownlint --config .markdownlint.yaml --stdin < docs/scratchPad/handsoff_doc.md` — PASS. Lesende Dateiprüfung — PASS: gültiges UTF-8, abschließender Zeilenumbruch, keine nachgestellten Leerzeichen, alle elf relativen Links auf vorhandene Ziele. `rtk git diff --check` — PASS. Keine Testsuite und kein Browserbenchmark ausgeführt.

Für den Rechnerwechsel diese neue Übergabe sowie die anschließend gewünschten Löschungen der beiden Vorgänger manuell stagen und committen; den Commit normalerweise pushen und auf dem anderen PC übernehmen. Die zehn bereits lokalen Commits müssen für denselben Checkout ebenfalls übertragen werden. Es sind keine zusätzlichen nur lokal vorhandenen Messdaten oder Prototypdateien erforderlich.
