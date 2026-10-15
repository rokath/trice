# RM-Speedup2: Ergänzung zur Performance-Übergabe

Stand: 11. Oktober 2026, Europe/Berlin. Zuerst [handsoff_rm_performance.md](./handsoff_rm_performance.md) lesen. Diese Ergänzung aktualisiert den Arbeitsstand und den vorgeschlagenen nächsten Schritt; die bestehende Übergabe bleibt erhalten.

## Ziel und Freigabe

RM-Verweise sollen auf schwächeren Handys schnell zur lesbaren, stabilen und bedienbaren Zielstelle führen. Der Nutzer berichtet weiterhin unzumutbare Wartezeiten oder versagende Links und hält manuelle Handytests für nicht zuverlässig reproduzierbar.

Aktueller Auftrag: einen vernünftigen Weg finden, noch nichts implementieren. Ein automatisierter Vergleich und Kapitelansichten wurden vorgeschlagen, aber noch nicht zur Umsetzung freigegeben. Das letzte „ja“ bestätigt ausschließlich diese ergänzende Übergabedatei. Auf `handson rm-speedup2` den Stand prüfen und auf die nächste Nutzeranweisung warten.

GitHub-Dateiansicht und Pages bleiben gleichwertig zu berücksichtigen. Automatische Inhaltsverzeichnisse, ausschließlich Open Source und möglichst geringe Änderungen am UM gelten weiter. Keine Formatmigration oder neue Eigenentwicklung ohne nachgewiesenen Bedarf.

## Git und lokale Änderungen

- Branch: `wip`, beim Schreiben mit `origin/wip` synchron.
- HEAD: `c77f364b5ba3a8e214e568d54f7077b69cd373fb` (`minor`).
- Uncommitted: `AGENTS.md` wurde auf ausdrücklichen Nutzerauftrag geändert: alle Lesezugriffe dauerhaft freigegeben, widersprüchliche Netzwerkregeln entfernt. Technisch erzwungene Plattformfreigaben bleiben möglich; keine zusätzliche Gesprächsfreigabe verlangen.
- Untracked: die bestehende `handsoff_rm_performance.md` und diese neue Ergänzung.
- Weitere lokale Änderungen: `examples/G0B1_log/li.json`, `examples/PC_log/li.json`. Herkunft nicht untersucht, außerhalb der RM-Aufgabe, unberührt lassen.
- In dieser Analyse keine RM-/UM-Änderungen, kein Commit, Push oder Deployment.

## Bereits in Git und geprüfte Befunde

Die Medienoptimierung aus der ersten Übergabe ist bereits umgesetzt: Commit `276e3b05` (`docs: clarify payload limits and add reference previews`) enthält fünf statische Vorschauen mit Links auf erhaltene Originalanimationen. Er ist in `wip` und im lokal vorhandenen `origin/main` enthalten. Diese Arbeit nicht erneut beginnen.

Am aktuellen Checkout durch lesende Python-Auswertung bestätigt:

- RM: 843.505 Bytes, 13.295 Zeilen, 53 Hauptkapitel.
- 559 explizite IDs, keine doppelte ID gefunden.
- Alle 30 direkten UM-Verweise ins RM haben vorhandene Quelltext-Zielanker. Das beweist weder den Ankersprung noch die Bedienbarkeit im Browser.
- Kapitelquelltext: Binary Encoding 15.743 Bytes, Trice Bind 59.139 Bytes, Trice over RTT 39.016 Bytes.

`.github/workflows/pages.yml` veröffentlicht bei Push auf `main`, verwendet Jekyll/Cayman und schreibt Manual-Verweise für Pages um. `mdtoc --help` wurde gelesen: ToC-/Nummerierungs-/Ankerverwaltung vorhanden, kein Split-Befehl aufgeführt.

Kein Browserprofil, keine Laufzeitmessung und kein automatisierter Geschwindigkeitstest ausgeführt. Webabrufe lieferten eine offenbar ältere Darstellung; ein direkter `curl`-Abruf scheiterte zunächst mit DNS-Auflösung im Sandboxkontext, der anschließende Freigabeversuch wurde durch Nutzerunterbrechung abgebrochen. Aktuell tatsächlich veröffentlichte Fassung deshalb nicht verifiziert. Keine Sekundenverbesserung behaupten.

## Vorgeschlagener automatisierter Vergleich

Playwright mit Chromium als Browser-Prüfstand verwenden; Installation und lokale Verfügbarkeit wurden noch nicht geprüft. Mobile Bildschirm-/Touchprofile simulieren keine Handy-Rechenleistung: CPU und Netzwerk gesondert drosseln.

- Feste Dokumentstände, Browserversion, Rechner, Bildschirmgröße und Drosselung; Messungen nacheinander.
- Repräsentative frühe, mittlere und späte Zielstellen; Direktaufruf, Klick vom UM und Zurücknavigation.
- Beispielsweise 20 Läufe je Variante; leerer und gefüllter Browsercache getrennt. Median, langsamere Durchläufe und Fehlerzahl berichten.
- Zielüberschrift und ersten Absatz im sichtbaren Bildschirmbereich prüfen. Zeit bis zur stabilen Zielposition, spätere Verschiebungen und Reaktionszeit auf eine Bedienaktion erfassen. Beobachtungsfenster und Erfolgsschwellen vorab definieren.
- Der Test darf nicht selbst zum Ziel scrollen oder durch automatische Aktionen den Ankersprung reparieren. HTTP 200, vorhandene ID oder bloße DOM-Sichtbarkeit reichen nicht.
- Kontrolliert ausgelieferte Dokumentstände für Ursachenvergleich und tatsächliche GitHub-/Pages-Aufrufe für Veröffentlichung getrennt messen. GitHubs Server/CDN bleiben variable Einflüsse; lokale Pages-Ergebnisse beweisen keine GitHub-Beschleunigung.
- Lighthouse nur ergänzend: seine Gesamtwertung beantwortet nicht, ob der angeklickte Abschnitt erreicht wurde.

Geprüfte Primärquellen: [Playwright-Emulation](https://playwright.dev/docs/emulation), [Viewport-Prüfung](https://playwright.dev/docs/api/class-locatorassertions#locator-assertions-to-be-in-viewport), [CPU-/Netzdrosselung](https://developer.chrome.com/docs/devtools/settings/throttling), [Lighthouse-Messgrößen](https://developer.chrome.com/docs/lighthouse/performance/performance-scoring).

## Mögliche Lösung und nächste Schritte

Eine automatisch erzeugte Kapitelansicht unter beispielsweise `docs/split/` bleibt ein Kandidat: RM zunächst einzige bearbeitete Quelle, stabile Kapiteldateinamen, automatisch gepflegte Anker, Querverweise, Bildpfade und ToCs. Neue Verweise könnten direkt das benötigte Kapitel öffnen. Gesamtfassung für alte Links erhalten; deren Wartezeit würde dadurch nicht verschwinden. Ein passend getesteter Generator ist noch nicht ausgewählt.

Nach einer Umsetzungserlaubnis zuerst am bestehenden RM die automatisierte Ausgangsmessung erstellen und zwischen Lade-, Darstellungs- und Ankersprungproblemen unterscheiden. Danach wenige repräsentative Kapitel gegen denselben Prüfstand vergleichen. Erst anhand der Ergebnisse Struktur und Generator entscheiden. Vorher keine Kapitel erzeugen, UM-Verweise umstellen oder neue Abhängigkeiten einführen.

Relevante Dateien: [RM](../TriceReferenceManual.md), [UM](../TriceUserManual.md), [Pages-Workflow](../../.github/workflows/pages.yml), [Konfiguration](../../_config.yml), [Manual-Pflege](../../scripts/_310_refresh_trice_user_manual.sh), [Agent-Regeln](../../AGENTS.md).

Prüfung der bisherigen Agent-Regeländerung: `rtk git diff --check` bestanden. Für diesen Analyseauftrag keine Testsuite nötig; automatisierte Browserprüfungen sind vollständig ausstehend.
