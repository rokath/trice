# handsoff: Trice RM – Links schneller öffnen

Stand: 11. Oktober 2026, Europe/Berlin  
Projekt: https://github.com/rokath/trice  
Pages: https://rokath.github.io/trice/  
Thema/Tag: rm_performance

## Einstieg für Codex

Lies zuerst die im aktuellen Checkout geltenden AGENTS.md-Dateien und prüfe Branch, Arbeitsbaum und vorhandene Änderungen. Diese Übergabe basiert auf einer Untersuchung des öffentlichen main-Branches und der veröffentlichten Pages-Ausgabe, nicht auf dem lokalen Arbeitsbranch des Nutzers. Kein bestimmter Arbeitsbranch ist mit dieser Übergabe festgelegt.

Setze die Arbeit mit dem Nutzer fort. Ziel ist, Links in das Reference Manual auf schwächeren Handys schneller bis zur lesbaren, stabilen Zielstelle zu öffnen. Der Nutzer begrüßt, zunächst das bestehende RM zu optimieren. Priorität haben Verbesserungen, die auch direkt auf github.com wirken. Nicht erneut bei einer allgemeinen Diskussion über Dokumentationsformate beginnen.

In dieser Chat-Sitzung wurden KEINE Änderungen am Repository vorgenommen, kein Commit/PR erstellt und nichts veröffentlicht. Es wurden öffentliche Dateien gelesen, Ressourcen inventarisiert und Werkzeuge recherchiert. Dieses Dokument ist die einzige beauftragte Übergabedatei.

## Verbindlicher Nutzerkontext

- Viele Leser bleiben direkt auf GitHub; Pages ist kein Ersatz für diesen Nutzungsweg.
- Das jetzige UM und die übrigen Dokumente sollen möglichst unverändert bleiben. Anpassungen einzelner Verweise wären bei Bedarf diskutabel; keine allgemeine Migration.
- Das RM wird künftig inhaltlich überarbeitet und darf sich strukturell ändern.
- Inhaltsverzeichnis/ToC muss automatisch bleiben. Im RM wird bereits mdtoc verwendet.
- Ausschließlich Open Source.
- Weder Markdown noch eine einzige Quelldatei sind grundsätzlich vorgeschrieben.
- „Kleine Seiten“ sind KEIN Ziel und KEINE Anforderung. Entscheidend ist die Zeit bis zum nutzbaren Linkziel.
- Gewünscht ist eine ordentliche, wartbare Lösung; keine vorschnelle Eigenentwicklung und keine bloße Liste ungeprüfter Tools.
- Aktuelle Zustimmung: „Dein Vorschlag das rm zunächst zu optimieren ist aber gut.“
- Der Nutzer wechselt zu Codex, um die Sache praktisch weiter voranzutreiben.

## Aktuell bevorzugter erster Schritt

Die eingebundenen Medien im bestehenden RM optimieren, bevor Generator oder Dokumentstruktur gewechselt werden:

1. Große animierte GIFs durch kleine, aussagekräftige statische Vorschaubilder mit Link auf die Originalanimation ersetzen.
2. Bei Screenshots Auflösung, Textlesbarkeit und Dateigröße prüfen. Nur tatsächlich überdimensionierte Dateien verkleinern; ggf. Vorschau mit Link zum Original.
3. Originalanimationen und hochauflösende Originale zunächst erhalten. Keine Informationen wegoptimieren.
4. Bestehende Kapitel, ToC und explizite Anker möglichst unverändert lassen.
5. Änderungen in GitHub-Dateiansicht UND Pages beurteilen.

Das ist die zuletzt vorgeschlagene konkrete Maßnahme, noch NICHT umgesetzt. Die vorherige Zustimmung galt der Optimierungsrichtung; eine konkrete Auswahl aller zu ersetzenden Bilder wurde noch nicht mit dem Nutzer besprochen. Ein gezielter, reversibler Arbeitsbaum-Diff ist der sinnvolle nächste Schritt.

Markdown-Muster für eine Vorschau, Namen nur beispielhaft:

```markdown
[![Trice-Demo – Animation öffnen](ref/Animation-preview.png)](ref/Animation.gif)
```

Bei einer Animation einen informativen Frame auswählen, nicht automatisch den ersten möglicherweise leeren Frame. Falls ein einzelnes Standbild den Inhalt nicht angemessen vermittelt, mehrere kleine Ausschnitte oder eine andere Darstellung abwägen. Den Zugriff auf die Originalanimation klar kennzeichnen.

## Tatsächlich erhobene Befunde

Untersuchte Quellen:

- https://raw.githubusercontent.com/rokath/trice/main/docs/TriceReferenceManual.md
- https://rokath.github.io/trice/docs/TriceReferenceManual.html
- https://raw.githubusercontent.com/rokath/trice/main/_config.yml
- GitHub API: rekursiver Dateibaum von main zur Zuordnung der Bilddateigrößen.

### Quelltext und Build

- RM: docs/TriceReferenceManual.md
- Gelesene Dateigröße: 841.060 Bytes, etwa 821 KiB.
- GitHub zeigte 13.281 Zeilen.
- _config.yml enthielt:

```yaml
theme: jekyll-theme-cayman
markdown: kramdown
```

- Vollständiger Pages-Workflow und Repository-Anweisungen wurden NICHT untersucht. Codex muss sie im Checkout lesen.
- Pandoc-GFM-Parsing ergab 52 Überschriften auf Ebene 2, 281 auf Ebene 3, 220 auf Ebene 4 und 24 auf Ebene 5.
- Das RM enthält Nummerierung und explizite HTML-Anker, beispielsweise:

```markdown
## 1. <a id="abstract"></a>Abstract
```

- Das bestehende ToC verweist auf diese expliziten IDs, z. B. #abstract.
- Ein automatisch aus der Überschrift erzeugter Slug kann dagegen 1-abstract lauten. Nicht gleichsetzen!
- Vorhandene README-Verweise führen zumindest teilweise auf GitHubs blob-Dateiansicht des RM.

### Ausgeliefertes Pages-HTML

| Messgröße | Erhobener Wert |
| --- | ---: |
| HTML-Größe, unkomprimiert | 1.347.691 Bytes |
| Anzahl HTML-Starttags | 21.131 |
| img-Elemente | 70 |
| Unterschiedliche Bild-URLs | 66 |
| Summe der zugeordneten Bilddateigrößen | 16.406.711 Bytes |
| img mit loading="lazy" | 0 |
| img mit sowohl width als auch height | 0 |

Die Bildsumme stammt aus der Zuordnung der Bild-URLs zum Git-Dateibaum. Sie ist KEIN gemessener Netzwerktransfer: Caches, Proxies, Browser-Ladeentscheidungen und Kompression können die tatsächlich geladenen Bytes verändern.

Im untersuchten Pages-HTML war als script nur strukturierte Metadaten vom Typ application/ld+json vorhanden; kein offensichtlicher großer clientseitiger Markdown-Renderer. Stylesheet kam aus /trice/assets/css/style.css, zusätzlich Verweise auf Google Fonts.

### Größere eingebundene Bilddateien

| Datei | Bytes |
| --- | ---: |
| docs/ref/Animation.gif | 3.677.758 |
| docs/ref/LabPlotDemo.gif | 3.458.773 |
| docs/ref/life.gif | 2.076.449 |
| docs/ref/TriceCheckOutput.gif | 1.289.194 |
| docs/ref/G0B1AliasExample.png | 716.852 |
| docs/ref/manchester2.PNG | 317.461 |
| docs/ref/life0.gif | 279.357 |
| docs/ref/Putty.PNG | 223.540 |
| docs/ref/JLinkConfig0.png | 217.727 |
| docs/ref/2024-12-05_L432_inst_maxSpeed.png | 200.414 |

Die ersten drei GIFs umfassen zusammen 9.212.980 Bytes, etwa 9,2 MB. Diese Liste ist eine Priorisierungshilfe, keine Aufforderung, jedes Bild auszutauschen.

## Grenzen der bisherigen Diagnose

- Es gibt KEINE Laufzeitmessung auf dem betroffenen Handy.
- Es gibt KEIN Browserprofil der GitHub-Dateiansicht.
- Die HTML-Zählungen betreffen Pages, nicht GitHubs eigenes DOM.
- „Viele/große Bilder und lange DOM-Struktur bremsen“ ist eine plausible, durch die Ressourcenanalyse gestützte Hypothese, kein abschließender Ursachenbeweis.
- Fehlende Bildabmessungen können Layout-Verschiebungen verursachen; der Browser kann beim tiefen Ankersprung eine spät stabile Position erreichen.
- GitHub kann Bilder selbst über Proxies verarbeiten oder Lazy Loading einsetzen. Das ist separat zu prüfen. Nicht die Pages-Befunde ungeprüft auf GitHub übertragen.
- Eine Verbesserung der Bildlast garantiert nicht, dass GitHubs serverseitiges Rendering oder die gesamte Wartezeit verschwindet.
- „Datei ist groß, also muss sie geteilt werden“ war eine vorschnelle Schlussfolgerung.

## Vorgehen für die nächste Codex-Sitzung

1. AGENTS.md, Branch und git status prüfen; Nutzeränderungen bewahren. Diese Zahlen am aktuellen Stand gegenprüfen.
2. Bildverwendungen im RM inventarisieren: Dateigröße, Abmessungen, bei GIFs Framezahl/Dauer. Gemeinsame Verwendung derselben Dateien in anderen Dokumenten berücksichtigen.
3. Repräsentative langsame Links wählen: frühes, mittleres und spätes Ziel; jeweils GitHub und Pages getrennt betrachten.
4. Wenn möglich Ausgangszustand mit frischem Cache und erneutem Aufruf erfassen. Hauptkriterium: Zeit bis der Zieltext lesbar, stabil positioniert und die Seite bedienbar ist, nicht nur load oder die vollständig geladene Animation.
5. Mit den größten GIFs einen kleinen, reviewbaren Optimierungs-Diff vorbereiten. Kleine statische Vorschaubilder neu erzeugen, nur die RM-Einbettungen ändern, Originale erhalten.
6. Geeignete vorhandene OSS-Tools nutzen, z. B. ffmpeg/ImageMagick/Pillow je nach Umgebung und Projektkonvention. Für Screenshots verlustfreie Optimierung bevorzugen, solange Text dadurch gut lesbar bleibt.
7. ToC-/Ankererhalt, relative Links, Bildlesbarkeit sowie GitHub-/Pages-Kompatibilität prüfen. Bestehende relevante Repo-Checks ausführen, keine breite fachfremde Testsuite allein wegen der Medienänderung.
8. Vorher/nachher-Dateigrößen berichten und eine visuelle Kontrolle durchführen. Keine Handy-Beschleunigung in Sekunden behaupten, wenn sie nicht gemessen wurde.
9. Falls GitHub weiterhin unbefriedigend ist: Kapitelkopien oder andere Ansätze gezielt anhand des verbleibenden Engpasses untersuchen.
10. Commit/Push/Publikation nach geltenden Repo-Anweisungen und Nutzerauftrag behandeln; aus dieser Übergabe keine bereits erteilte Publikationsfreigabe ableiten.

## Weitere Wege, falls Optimierung nicht ausreicht

### Automatisch erzeugte Kapitel-MDs

Vom Nutzer ausdrücklich vorgeschlagen:

- RM bleibt die bearbeitete Quelle.
- Kapitelkopien entstehen automatisch in docs/split/.
- Verweise aus anderen Dokumenten zeigen künftig auf die passenden Kopien.
- Ein automatisch erzeugtes Kapitelverzeichnis und ggf. lokale ToCs.
- Auch direkt auf GitHub lesbar.

Das ist eine mögliche Lösung, keine bereits beschlossene Architektur. Anforderungen wären: Codeblöcke korrekt parsen, explizite HTML-Anker erhalten, interne Querverweise und relative Bildpfade anpassen, stabile Namen ohne Positionsnummern, veraltete Ausgaben verwalten. Bestehende externe Links auf das vollständige RM bleiben gültig, aber unverändert langsam. GitHubs blob-URLs können nicht durch eigene Pages-Redirects umgeleitet werden.

### Pages-spezifische Optimierung

- Automatisch Bildabmessungen und natives Lazy Loading im Build ergänzen.
- Optional content-visibility: auto auf geeigneten Abschnitten testen.
- Diese Maßnahmen helfen nicht automatisch in GitHubs Dateiansicht.
- Pages darf nicht zum verpflichtenden Lesekanal erklärt werden.
- Jekyll ist bereits der vorhandene Generator. Ein Format-/Generatorwechsel ist nicht nötig, nur um Bilder effizienter auszuliefern.

### Pandoc

- OSS, GPL-2.0-or-later; aktiv gepflegt. Bei Recherche aktuelle Version 3.12.1, veröffentlicht 08.10.2026.
- Chunked HTML: automatische Teilung nach Überschriften, ToC, Navigation, Anpassung interner Links.
- Ausgabe ist HTML, nicht Kapitel-Markdown.
- Kleine Untersuchung mit lokal vorhandenem Pandoc 3.1.3: GFM-Parsing funktionierte, explizite HTML-Anker waren RawInline-Knoten neben generierten Header-IDs.
- Der anschließende Chunked-HTML-Versuch endete wegen nicht lokal heruntergeladener Bildressource ref/TriceCheckOutput.gif. KEIN erfolgreicher vollständiger Konvertierungs-/Kompatibilitätstest.
- Nicht als sofort funktionierenden Einzeiler verkaufen; Roh-HTML-Anker und vorhandenes ToC müssen geprüft werden.

## Recherchierte Werkzeuge und Wartungsstand

Stand der API-/Quellcodeprüfung: 11.10.2026. Sterne sind nur Größenordnung, kein Qualitätsbeweis.

| Projekt | Geprüfter Stand | Bewertung |
| --- | --- | --- |
| jgm/pandoc | Release 3.12.1 vom 08.10.2026; aktuelle Entwicklung | Etabliert für HTML-Erzeugung, erfüllt Kapitel-MD-Wunsch nicht direkt |
| rstudio/bookdown | Letzter abgefragter Commit 18.09.2026; ca. 4.080 Sterne | Automatische HTML-Aufteilung; zusätzliche R-Werkzeugkette, derzeit kein Vorteil für den ersten Schritt |
| ExaDev/markmv | MIT; Release 1.41.7 vom 06.10.2026; ca. 7 Sterne | Aktiv, aber auf Umstrukturierung ausgelegt; kann Original/Verweise ändern, nicht ungeprüft als wiederholbaren Kopiengenerator einsetzen |
| ksylvan/markdown-tree-parser | Letzter Commit 22.08.2025; ca. 33 Sterne | Abschnittextraktion, ToC und Linkprüfung; vollständige Reparatur aller Trice-Querverweise nicht nachgewiesen |
| markusstraub/mdsplit | MIT; letzter Commit 19.10.2024; ca. 65 Sterne | Einfacher Python-Splitter, optionale ToCs/Navigation; vollständige Linkkorrektur nicht dokumentiert |
| alandefreitas/mdsplit | MIT; letzter Commit 11.01.2021; ca. 10 Sterne | Beschreibt passenden Workflow, aber ruht seit fast sechs Jahren; Nutzer beanstandete das zu Recht |

Bei markmv wurden zusätzlich src/core/content-splitter.ts und src/strategies/split-strategies.ts gelesen. Die Implementierung plant Änderungen am Original und an extern verweisenden Dateien. Eine verlässliche Behandlung von Trices HTML-Ankern wurde nicht getestet. Werbeaussagen zur Linkintegrität sind keine Testergebnisse.

Sphinx/reStructuredText/MyST, Antora/AsciiDoc, MkDocs und mdBook wurden zuvor diskutiert. Keine Auswahl getroffen. Nicht ohne konkreten Zusatznutzen eine Migration des UM oder der gesamten Doku anfangen.

## Quellen zur weiteren Prüfung

Projekt und Ist-Zustand:

- https://github.com/rokath/trice
- https://github.com/rokath/trice/blob/main/docs/TriceReferenceManual.md
- https://rokath.github.io/trice/docs/TriceReferenceManual.html
- https://github.com/rokath/trice/blob/main/_config.yml

Browserverhalten und Pages:

- https://web.dev/articles/browser-level-image-lazy-loading
- https://web.dev/articles/optimize-cls
- https://web.dev/articles/content-visibility
- https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages

Werkzeuge:

- https://pandoc.org/MANUAL.html#chunked-html
- https://github.com/jgm/pandoc/releases
- https://github.com/ExaDev/markmv
- https://github.com/ExaDev/markmv/blob/main/CHANGELOG.md
- https://github.com/ksylvan/markdown-tree-parser
- https://github.com/markusstraub/mdsplit
- https://github.com/alandefreitas/mdsplit
- https://pkgs.rstudio.com/bookdown/reference/html_chapters.html
- https://github.com/rstudio/bookdown

## Was vermieden werden soll

- Weitere wechselnde pauschale Empfehlungen ohne Prüfung am RM.
- Pages als alleinigen Nutzungsweg behandeln.
- Kleine Seiten, Markdown-Erhalt oder Formatwechsel als vermeintliche Nutzervorgabe ausgeben.
- Einen ungepflegten kleinen Splitter als etablierte Standardlösung darstellen.
- ToC oder Linkzuordnungen künftig manuell pflegen lassen.
- GitHub-Darstellung mit dem Pages-HTML verwechseln.
- Aus Ressourceninventar eine bereits bewiesene Ladezeitverbesserung machen.
- Ohne Not eigene Parser oder ein neues Dokumentationssystem bauen.

Die nächste sinnvolle Arbeit ist ein überschaubarer Optimierungsversuch an den großen eingebetteten Medien des RM, mit erhaltenen Originalen und überprüfbarem Ergebnis auf beiden Lesekanälen.

