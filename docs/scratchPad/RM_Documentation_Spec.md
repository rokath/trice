# Modulares Reference Manual mit GitHub-Navigation und Stichwortindex

Stand: 12. Oktober 2026. Diese Spezifikation hält die beschlossenen Ergebnisse der Dokumentationsdiskussion fest. Sie ist ein Plan für eine spätere Umsetzung, kein Startsignal für Migration, Installation, Build-Änderungen oder Veröffentlichung. Die zugehörige Übergabe steht in [handsoff_doc.md](handsoff_doc.md).

## Ziel und Geltungsbereich

Das große [Trice Reference Manual](../TriceReferenceManual.md) wird einmalig mit KI-Unterstützung in ungefähr 50 Hauptkapitel zerlegt. Anschließend werden ausschließlich die einzelnen Kapitel gepflegt. Leser können auf GitHub über einen Haupt-ToC, kapitelinterne ToCs und einen Stichwortindex navigieren. Suche über sämtliche Inhalte bleibt ein eigenes, wichtiges Anliegen.

Diese Umstellung betrifft das RM und die aktiven Verweise darauf. UM und TriceGuide bleiben eigene Einstiege; ihre Inhalte werden deshalb nicht umgeschrieben oder neu organisiert. Die bestehenden Medien unter [docs/ref/](../ref/) bleiben erhalten. Eine zusätzliche Website kann dieselben Quellen verwenden, ist aber für das vereinbarte Markdown-Layout nicht erforderlich.

## Zielstruktur und Verantwortlichkeiten

Die folgenden Pfade sind ein Zielbild. Der Ordner `rm/` und seine Dateien existieren im aktuellen Checkout noch nicht.

```text
docs/
├── rm/
│   ├── README.md
│   ├── INDEX.md
│   ├── HEADER.md
│   ├── FOOTER.md
│   ├── abstract.md
│   ├── binary_encoding.md
│   ├── bind.md
│   ├── rtt.md
│   └── ...
├── ref/
├── TriceUserManual.md
├── TriceGuide.md
└── scratchPad/
    ├── handsoff_doc.md
    ├── handsoff_doc_0.md
    ├── RM_Documentation_Spec.md
    └── obsolete/
        └── TriceReferenceManual.md
```

`HEADER.md` und `FOOTER.md` sind bei Bedarf erlaubt; keine Pflicht, leere Dateien anzulegen. Die gezeigten Kapiteldateinamen sind Beispiele. Ein Ablageort für `RM-search.txt` oder HTML ist noch nicht entschieden.

| Bestandteil | Verantwortung |
| --- | --- |
| Kapiteldateien | Einzige laufend gepflegte Quellen des fachlichen RM-Inhalts |
| `README.md` | Hauptnavigation und einzige manuell gepflegte Hauptkapitelreihenfolge |
| Kapitel-ToCs, Nummerierung und Anker | Aus Kapitelüberschriften mit mdtoc erzeugt |
| `INDEX.md` | Mit KI initial erzeugter und anschließend redaktionell kontrollierter Stichwortindex |
| `HEADER.md` und `FOOTER.md` | Bei Bedarf erhaltene RM-Inhalte außerhalb der Hauptkapitel |
| Archiviertes Original | Historischer Stand, keine zweite aktive Quelle |
| Optionale Such-/HTML-Ausgaben | Aus denselben Quellen erzeugt, nicht parallel redaktionell gepflegt |

## Kapiteldateien und Überschriften

- Jedes bisherige Hauptkapitel erhält eine eigene Markdown-Datei unter `docs/rm/`.
- Dateinamen werden aus dem Hauptkapiteltext abgeleitet, im Stil `heading_text.md`, ohne Kapitelnummer. Die genaue Normalisierung von Sonderzeichen und die Auflösung möglicher Namenskonflikte sind bei der Bestandsaufnahme festzulegen.
- Jedes Kapitel beginnt mit einer H1-Hauptüberschrift ohne Hauptkapitelnummer. Seine bisherigen Unterüberschriften werden entsprechend eine Ebene angehoben.
- Unterkapitel werden innerhalb der Datei lokal nummeriert, jeweils neu beginnend bei 1. Die H1-Hauptüberschrift bleibt von dieser Nummerierung ausgenommen.
- mdtoc erzeugt den lokalen ToC und die erforderlichen stabilen Überschriftenanker. Die dafür passende vorhandene Funktion oder nötige Erweiterung ist noch zu prüfen.
- Autoren schreiben normale Markdown-Überschriften. Nummern und explizite Anker werden nicht zusätzlich manuell gepflegt.
- Ein Querverweis zwischen Hauptkapiteln wird zu einem relativen Dateilink, bei Bedarf mit Fragment auf einen Abschnitt. Spätere Titeländerungen dürfen nicht unbemerkt bestehende Dateilinks oder Fragmente brechen.

Die Zerlegung erhält Texte, Beispiele, Tabellen, Medien und fachliche Aussagen. Sie ist keine redaktionelle Überarbeitung des ganzen RM. Generierte globale ToCs und bisherige globale Kapitelnummern werden durch die neue Navigation beziehungsweise die lokalen Generatorausgaben ersetzt.

## Hauptnavigation in README.md

`docs/rm/README.md` wird von GitHub beim Öffnen des Ordners direkt gerendert. Sie enthält die Links auf alle Hauptkapitel in der gewünschten Lesereihenfolge. Die initiale Reihenfolge wird aus dem bisherigen RM übernommen; anschließend wird sie manuell gepflegt.

```markdown
- [Abstract](abstract.md)
- [Binary Encoding](binary_encoding.md)
- [Bind](bind.md)
- [RTT](rtt.md)
```

Es gibt keine globale Hauptkapitelnummerierung, keine Reihenfolge in Dateinamen und keine zweite manuell gepflegte Sortierdatei. Die Reihenfolge der Linkzeilen ist maßgeblich; GitHubs alphabetische Dateiliste ist davon unabhängig. Neue Hauptkapitel werden an geeigneter Stelle in die README aufgenommen. Eine automatische Erkennung neuer Dateien ist derzeit keine Voraussetzung.

Der Einstieg muss auch den Stichwortindex erreichbar machen und bei Bedarf auf HEADER-/FOOTER-Inhalte verweisen. Ein Rücklink von den Kapiteln zum Einstieg wurde als praktische Hilfe besprochen, ist aber noch keine festgelegte gemeinsame Kapitelschablone.

Für einen späteren mdBook-Build kann eine passende `SUMMARY.md` aus der Kapitelreihenfolge dieser README erzeugt werden. Eine solche generierte Anpassung darf keine zweite manuelle Reihenfolge einführen. Die genaue Abgrenzung der Kapitel-Linkliste gegenüber sonstigen README-Inhalten ist dann zu spezifizieren.

## HEADER.md und FOOTER.md

RM-Inhalte vor dem ersten oder nach dem letzten Hauptkapitel sowie sonstige nicht sinnvoll zuordenbare Bestandteile dürfen in `HEADER.md` und `FOOTER.md` erhalten werden. Welche konkreten Passagen dazugehören, wird bei der Zerlegung anhand des Quelltexts geprüft; nicht allein nach ihrer Position entscheiden.

Dabei bleiben Autorenschaft, Lizenzhinweise, Einstiegsinformationen und Ressourcenverweise erhalten, soweit sie im RM vorhanden sind. Veraltete, vom Monolithen abhängige Navigation wie globale Top-/Bottom-Links muss im Zusammenhang der neuen Dateien bewertet werden. Es darf nichts ungeprüft verschwinden. Ein globaler generierter ToC gehört dagegen nicht als zweite manuell gepflegte Übersicht in diese Dateien.

## Einmalige Linkmigration und Archivierung

Die initiale Umstellung umfasst aktive Links in das bisherige RM sowie die Links innerhalb und zwischen seinen neuen Kapiteln. Grundlage ist eine vollständige Zuordnung von bisherigen RM-Zielstellen zu neuen Dateien und Fragmenten.

- Aktive Verweise aus UM, Guide, Dokumentationseinstiegen und weiteren aktiven Repository-Dokumenten werden auf `rm/heading_text.md` beziehungsweise dessen Abschnitt umgestellt.
- Bisherige RM-interne Fragmentlinks werden innerhalb einer Datei lokal belassen oder bei einem Kapitelwechsel um einen relativen Dateipfad ergänzt.
- Bilder aus `docs/ref/` werden in Kapiteldateien typischerweise über `../ref/...` eingebunden. Andere relative Ressourcenlinks müssen ebenfalls vom neuen Standort aus aufgelöst werden.
- Nicht nur Markdown-Inline-Links betrachten: Referenzlinks, HTML-Links, HTML-Bilder und bereits explizite IDs sind bei der Prüfung zu berücksichtigen.
- Vorhandene explizite IDs und automatisch erzeugte GitHub-Slugs dürfen nicht ungeprüft gleichgesetzt werden. mdtoc-Zielanker müssen nach der Generierung erneut gegen die Links geprüft werden.
- Aktive Leserlinks zeigen auf die Kapitelquellen, nicht auf eine optionale erzeugte Gesamtfassung.

Nach erfolgreicher Umstellung wird das bisherige `docs/TriceReferenceManual.md` nach `docs/scratchPad/obsolete/TriceReferenceManual.md` verschoben und nicht weiter gepflegt. Die Archivkopie wird nicht als aktive Navigation verwendet. Ihre nach dem Verschieben gegebenenfalls nicht mehr auflösbaren relativen Links werden nicht als Nebenarbeit repariert. Bereits bestehende Archive bleiben unangetastet.

Externe oder historische GitHub-Links auf die alte Datei lassen sich durch die Anpassung aktiver Repository-Verweise nicht automatisch reparieren. Ein Weiterleitungs- oder Kompatibilitätskonzept wurde nicht beschlossen. Falls eines benötigt wird, ist das vor Umsetzung separat zu entscheiden.

## Stichwortindex in INDEX.md

`docs/rm/INDEX.md` soll ausgewählte Fachbegriffe und jeweils einen oder mehrere Links auf relevante Kapitelabschnitte enthalten. Er ist eine ergänzende Navigation, keine vollständige Volltextsuche.

Der gewünschte Arbeitsablauf ist:

1. Ein KI-Agent erzeugt initial aus den Kapitelquellen eine Begriffsauswahl mit passenden Fundstellen.
2. Der Nutzer prüft die Auswahl, löscht unerwünschte Begriffszeilen und ergänzt fehlende Begriffe zentral im Index.
3. Ein Tool oder KI-Agent kann für diese beibehaltenen beziehungsweise ergänzten Begriffe Links vervollständigen und aktualisieren.

Verpflichtende Stichwortmarkierungen im Kapiteltext werden abgelehnt: Sie können leicht vergessen werden und würden relevante Begriffe dann aus dem Index ausschließen. Es soll auch keine zusätzliche manuell gepflegte Begriffsliste nötig sein, solange sich die Pflege sinnvoll in `INDEX.md` unterbringen lässt.

Für einen späteren regelmäßigen Aktualisierer wurden folgende Anforderungen vorgeschlagen; das genaue Format und Werkzeug sind noch offen:

- Zentral aufgeführte Begriffe steuern die Aktualisierung. Vom Nutzer gelöschte Begriffe werden nicht ungefragt wieder eingeführt.
- Mehrere sinnvoll verschiedene Fundstellen pro Begriff sind möglich. Wiederholungen desselben Wortes im selben Abschnitt führen nicht zu entsprechend vielen identischen Links.
- Links verwenden vorhandene Kapiteldateien und mdtoc-Zielanker.
- Die Suche nach Fundstellen überspringt den Index selbst und erzeugte ToCs, damit sie sich nicht an eigenen Navigationslinks orientiert.
- Synonyme und mehrteilige Fachbegriffe müssen berücksichtigt werden können; ihre Darstellung und Pflege sind noch nicht festgelegt.
- Ein Tool kann tatsächliche Wortfundstellen sammeln. Welche Abschnitte einen Begriff am besten erklären, bleibt eine redaktionelle Auswahl durch Mensch oder KI.
- Falls Links manuell ausgewählt oder verändert werden dürfen, muss vor Automatisierung geklärt werden, welche Teile der Aktualisierer ersetzen darf.

Eine alphabetische Wortliste mit Häufigkeiten kann bei der initialen Begriffsauswahl helfen. Stopwörter können herausgefiltert, Überschriften und technische Bezeichner stärker gewichtet werden. Hohe Häufigkeit allein ist kein Ausschlusskriterium: Zentrale Fachbegriffe können gerade häufig vorkommen. Nur Überschriften zu indizieren wäre einfacher, übersähe aber Begriffe im Fließtext und in Beispielen. Das sind mögliche Hilfsmittel, keine vorgeschriebene zusätzliche Verarbeitungskette.

Für genau diesen Ablauf wurde noch kein fertiger Generator verifiziert. Eine eigene Erweiterung ist möglich, aber weder beauftragt noch als neue mdtoc-Funktion festgelegt. Die konkrete INDEX-Syntax, die Zuständigkeit von KI gegenüber Tool und das Aktualisierungsverfahren sind vor ihrer Implementierung zu entscheiden.

## Suche und RM-search.txt

Das bequeme Durchsuchen des gesamten RM bleibt wichtig. Lokal kann eine Editorsuche über den Ordner alle Kapitel durchsuchen. Die Browsersuche auf einer einzelnen GitHub-Kapitelseite erfasst hingegen nur dieses Kapitel. Ein Stichwortindex deckt zudem nicht jede beliebige Suchphrase ab.

`RM-search.txt` ist als einfache, automatisch erzeugte Zusammenfassung für die Suche positiv bewertet. Im Gespräch war eine aneinandergereihte Textfassung mit erkennbaren Quelldateinamen gemeint, ohne den Anspruch einer voll funktionsfähigen Markdown-Lesefassung mit Bildern und allen Querverweisen.

**Noch nicht zur Umsetzung spezifiziert:** Ablageort, Erzeugungszeitpunkt, Kapitelreihenfolge, Aufnahme von HEADER/FOOTER, Textaufbereitung einschließlich Code/Tabellen, Behandlung von ToCs und HTML, Zuordnung eines Treffers zur Quelle, Zeichencodierung sowie Versionierung oder Release-Verteilung. Ein Implementierungsauftrag muss diese Fragen zuerst konkretisieren. Es wird dafür jetzt keine Datei und kein Skript angelegt.

Ein vollständig zusammengefügtes Markdown-RM mit reparierten Medienpfaden und umgeschriebenen internen Links ist zurückgestellt. Für den ursprünglichen Zweck, einfach alles durchsuchen zu können, wäre die Behandlung von ToCs, mehrfachen IDs und Querverweisen eine unverhältnismäßig große zusätzliche Baustelle.

## mdBook als zusätzliche Ausgabe

Nach dem Wechsel auf Kapitelquellen ist mdBook ein sinnvoller zusätzlicher HTML-Kandidat. Seine eingebaute Volltextsuche benötigt keine markierten Indexbegriffe. Relative Markdown-Dateilinks werden für die HTML-Ausgabe angepasst. Die Hauptkapitelnummerierung der HTML-Navigation kann abgeschaltet werden. Siehe [HTML-Konfiguration und Suche](https://rust-lang.github.io/mdBook/format/configuration/renderers.html), [SUMMARY-Format](https://rust-lang.github.io/mdBook/format/summary.html) und [Markdown-Linkbehandlung](https://rust-lang.github.io/mdBook/format/markdown.html).

Für einen später beauftragten Build wären die SUMMARY-Erzeugung aus README, die Einbindung von `docs/ref/`, die Erhaltung der mdtoc-Fragmente, die Darstellung vorhandener Kapitel-ToCs und der Veröffentlichungsort zu klären und praktisch zu prüfen. HEADER/FOOTER und der normale Markdown-Stichwortindex müssen passend einbezogen werden. Keine parallelen redaktionellen HTML-Quellen schaffen.

Das optionale Plugin [mdbook-indexing](https://github.com/daviddrysdale/mdbook-indexing) erzeugt einen Fachbegriffsindex anhand von `{{i:term}}`, `{{ii:term}}` und `{{hi:term}}` im Kapiteltext. Es passt deshalb nicht direkt zur gewünschten zentralen Begriffspflege und würde ohne Vorverarbeitung sichtbare Makros in GitHub-Quellen hinterlassen. Für mdBooks Volltextsuche wird es nicht benötigt.

Die Wahl und Einrichtung des zusätzlichen HTML-Builds ist noch kein Implementierungsauftrag. GitHub Pages ist ein möglicher Veröffentlichungsort. Gemessene Geschwindigkeitsverbesserungen dürfen erst nach einem tatsächlichen Vergleich behauptet werden; im Gespräch wurde kein Browserbenchmark umgesetzt.

## Bereits untersuchte Alternativen und Grenzen

- [Pandoc Chunked HTML](https://pandoc.org/MANUAL.html): Ein automatisch aus dem Monolithen erzeugter HTML-Baum ist grundsätzlich eine Option; die frühere Aussage, dies sei kein praktikabler Weg, wurde korrigiert. Mit der nun akzeptierten Pflege einzelner Kapitel entfällt der Hauptgrund, den Monolithen als aktive Quelle zu behalten.
- [Just the Docs](https://just-the-docs.com/), Zensical, Sphinx und Antora wurden als Dokumentationssysteme betrachtet. Sie lösen unterschiedliche Website-, Referenz- und Versionsanforderungen; ihre erneute Einrichtung ist für das beschlossene Markdown-Layout nicht erforderlich. Popularität oder Sterne allein beweisen weder Eignung noch Wartungszukunft.
- [stitchmd](https://github.com/abhinav/stitchmd) wurde genauer anhand Dokumentation und Quelltext geprüft. Es kann Dateien zusammenführen, Überschriften verschieben und Pfade anpassen. `-no-toc` unterdrückt jedoch nicht die bestehenden Kapitel-ToCs. Explizite mdtoc-IDs und Kollisionen beim Zusammenfügen erfordern weitere Behandlung. Es wurde nicht installiert, ausgeführt oder geforkt.
- Der [Obsidian Book Index](https://github.com/usero2/obsidian-plugins-book-index) sammelt Begriffe und Fundstellen innerhalb Obsidian; ein passender statischer Markdown-Export wurde nicht verifiziert. Er ist deshalb kein nachgewiesener Generator für den gewünschten Repository-Index.

Frühere Performancebefunde, OSS-Recherche und der alte Strukturvorschlag sind in [handsoff_doc_0.md](handsoff_doc_0.md) erhalten. Dort enthaltene Vorschläge dürfen die neueren Nutzerentscheidungen nicht überschreiben.

## Bestehende Builds und ausdrücklich vertagte Arbeit

Die aktuelle [Manual-Pflege](../../scripts/_310_refresh_trice_user_manual.sh) und [PDF-Erzeugung](../../scripts/_320_generate_trice_user_manual_pdf.sh) erwarten noch das bisherige RM. Auch [Pages](../../.github/workflows/pages.yml) und [Release-Konfiguration](../../.goreleaser.yaml) müssen vor einer tatsächlichen Ablösung auf ihre Abhängigkeiten geprüft werden.

Diese Abhängigkeiten sind eine offene Integrationsfrage, keine Freigabe, jetzt PDF-, Release- oder CI-Verfahren umzubauen. PDF-Erzeugung aus den Kapiteldateien wird später diskutiert. Ebenso offen sind Installationen, neue Abhängigkeiten, Veröffentlichung, ein eventueller stitchmd-Fork und ein PR. UM-/Guide-Konvertierung oder sonstige Dokumentationsbereinigung sind kein Teil des aktuellen Plans.

## Vorgehen nach einem ausdrücklichen Implementierungsauftrag

1. Kapitel und Nicht-Kapitel-Inhalte inventarisieren. Alte Ziel-IDs, aktive eingehende Verweise, Medienpfade und Build-Abhängigkeiten erfassen.
2. Noch offene Entscheidungen für den konkret beauftragten Umfang klären, insbesondere den Umgang bestehender Builds bei Ablösung des RM. Zusätzliche Such-, Index- oder HTML-Automatisierung nicht stillschweigend mit einführen.
3. Kapitel und gegebenenfalls HEADER/FOOTER inhaltserhaltend zerlegen; README in der bisherigen Reihenfolge initial erzeugen. Die genaue Dateinamen- und Zielzuordnung nachvollziehbar machen.
4. Lokale mdtoc-ToCs, Nummerierung und Anker erzeugen und alle aktiven Querverweise auf die neuen Ziele anpassen.
5. INDEX mit KI-Unterstützung initial erzeugen und redaktionell prüfen. Eine regelmäßige Tool-Aktualisierung erst nach Festlegung ihres Formats implementieren.
6. Inhaltserhaltung und Navigation prüfen. Erst bei erfolgreicher Umstellung das Original an den festgelegten Archivort verschieben und aktive Verweise nochmals prüfen.

Die einmalige Migration betrifft deutlich mehr als fünf Dateien. Nach den geltenden Agent-Regeln zunächst einen repräsentativen Diff mit Zusammenfassung zeigen, bevor die breite Änderung fortgesetzt wird. Eine solche Review-Grenze ist keine Freigabe, ungeklärte Entscheidungen selbst zu treffen.

## Abnahmekriterien für die Markdown-Umstellung

- Die bisherigen Hauptkapitel sind vollständig zugeordnet und als einzelne Quellen vorhanden; übrige Inhalte sind erhalten oder als ausdrücklich zu entscheidende Ausnahme ausgewiesen.
- GitHub zeigt beim Öffnen von `docs/rm/` die README-Navigation. Alle Hauptkapitel sind von dort in der gewünschten Reihenfolge erreichbar.
- Hauptkapitel und Dateinamen enthalten keine globale Nummerierung. Lokale Unterkapitelnummern, ToCs und Anker passen zu ihrer jeweiligen Datei.
- Aktive eingehende RM-Links, kapitelübergreifende Links und Medienpfade sind für die neue Struktur geprüft und funktionieren. Archivlinks werden dabei nicht umgeschrieben.
- Ein initialer INDEX enthält geprüfte Fachbegriffe mit gültigen Links auf die neuen Quellen. Er behauptet keine vollständige Volltextabdeckung.
- Der alte Monolith liegt nur noch am vereinbarten Archivort; es gibt keine zweite aktiv gepflegte RM-Gesamtquelle.
- Die Auswirkungen auf bestehende Pflege-/Build-Abläufe sind vor Ablösung geklärt und nicht verdeckt durch Ersatzdateien oder stillschweigende Zusatzmigrationen umgangen.

Für `RM-search.txt`, HTML, PDF, Performance und automatisierte Indexpflege gelten erst später vereinbarte eigene Abnahmekriterien. Für diese Ausgaben ist hier kein fertiger Erfolg behauptet.
