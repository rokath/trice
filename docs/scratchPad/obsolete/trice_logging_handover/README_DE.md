# Trice: Issues, UM-Kapitel und Entscheidungsarchiv

**Stand:** 10. September 2026. **Weiterarbeit:** `wip`.

Dieses Paket enthält die sachlichen Ergebnisse der Diskussion als eigenständige Dateien. Es wurden keine GitHub-Issues angelegt und keine Repository-Dateien verändert. Die lokalen Kennungen L01 bis L27 sind keine GitHub-Issue-Nummern.

## Einstieg

| Datei | Inhalt |
|---|---|
| [Issue-Verzeichnis](ISSUE_INDEX.md) | Alle 27 englischen Issue-Dateien, einzeln verlinkt und nach Aufgabenart gruppiert |
| [UM-Kapitel Deutsch](chapters/Log_Level_Tags_und_Filterung_DE.md) | Vollständiger deutscher Kapitelentwurf |
| [UM-Kapitel Englisch](chapters/Log_Levels_Tags_and_Filtering_EN.md) | Inhaltlich entsprechende englische Fassung |
| [Entscheidungen und Kontext](context/REVIEW_AND_DECISIONS_EN.md) | Begriffsklärung, Begründungen, verworfene Alternativen, andere Logger und offene Fragen |
| [Abdeckungsmatrix](context/COVERAGE_MAP_EN.md) | Zuordnung der nummerierten Fragen und der früheren Befunde zu Dateien/Issues |
| [Quellenprüfung](context/SOURCE_REVIEW_EN.md) | Konkrete Codebeobachtungen, Quellen und Grenzen der Verifikation |
| [Testplan](context/TEST_PLAN_EN.md) | Zusammenhängende Regressionstests und empfohlene Arbeitsreihenfolge |
| [Alle Issues in einer Datei](ALL_ISSUES_EN.md) | Lesefassung und zusätzliche Kopiermöglichkeit; die Einzeldateien bleiben maßgeblich |
| [Alles in einer Markdown-Datei](COMPLETE_HANDOVER.md) | Archivfassung der Kapitel, Entscheidungen, Prüfhinweise und Issues |
| [Dateiprüfung](context/ARTIFACT_VALIDATION.md) | Ergebnis der lokal ausgeführten Prüfungen des Pakets |

## Was festgehalten wurde

Den effizienten Target-Kern und den bestehenden Datenstrom beibehalten. Zuerst konkrete Fehler und Inkonsistenzen der vorhandenen Funktionen korrigieren. Zusätzliche Compilezeit-Selektion nach Level und dynamische Target-Level-Steuerung sind zurückgestellt, nicht grundsätzlich verworfen. Der Hostschalter `-logLevel` soll nicht beiläufig verschwinden; seine Semantik braucht eine bewusste Kompatibilitätsentscheidung.

Die Issues trennen Fehler, Absicherung vorhandener Eigenschaften, Optimierung, Dokumentation und zurückgestellte Entwürfe. Insbesondere wird nicht behauptet, die bereits vorhandenen Single-Pack-`#error`-Prüfungen müssten erst neu erfunden werden. Rohaufzeichnung vor der Filterung ist ebenfalls bereits eine Stärke, die erhalten bleiben soll.

Die breitere Diskussion zu Kontextanreicherung und `strice` ist im Entscheidungsarchiv und L21-L26 gesichert. Eine ID für mehrere Felder bleibt sinnvoll; optionale Key-IDs können auf dem Host liegen. Eine detaillierte Erweiterungsspezifikation wird damit noch nicht vorweggenommen.

## Issues verwenden

Die Dateien in `issues/` sind unabhängig kopierbar. Die erste Überschrift ist der vorgeschlagene Issue-Titel. Jede Datei enthält Zusammenfassung, Befund, Reproduktions-/Prüfszenario, gewünschte Änderung, offene Akzeptanzkriterien, Kompatibilitätsgrenzen und Quellen. Vorgeschlagene Prioritäten sind keine verbindliche Releaseplanung.

Nach dem Anlegen echter GitHub-Issues die lokalen Querverweise durch deren Nummern oder URLs ersetzen. Bei einer Zusammenlegung von Aufgaben die individuellen Akzeptanzkriterien erhalten. L19/L20 ausdrücklich als zurückgestellt behandeln; sie sind keine Aufforderung, jetzt Target-Overhead einzuführen.

## Kapitel ins UM einbauen

Beide Fassungen beginnen auf Ebene `##`, weil die bestehenden UM-Hauptkapitel diese Ebene verwenden. Unterabschnitte verwenden `###`, zwei Unter-Unterabschnitte `####`. Es gibt absichtlich keine feste Kapitelnummer. Englisch und Deutsch sind alternative Fassungen mit denselben expliziten Ankern; nicht beide unverändert in dieselbe Datei einfügen.

Ein geeigneter Ort ist im funktionalen Teil nahe „Switching Trice ON and OFF“, nicht ausschließlich unter „Future Development“. Die genaue Position bleibt eine redaktionelle Entscheidung. Nach dem Einfügen Inhaltsverzeichnis und Nummerierung mit dem Projektverfahren neu erzeugen. Referenzlabels wie `S1` bei Bedarf präfixieren, falls das UM bereits gleichnamige Markdown-Linkdefinitionen enthält.

Der Kapitelentwurf beschreibt den geprüften Stand und kennzeichnet bekannte Fehler. Diese Hinweise erst entfernen, wenn die jeweilige Korrektur und ihre Tests tatsächlich vorliegen. Vorgeschlagene neue Validierungs-/Migrationsfunktionen werden nicht als vorhandene CLI-Schalter ausgegeben.

Die bei der Prüfung sichtbare Zuordnung war 45.1 Level-Control-Draft, 45.2 Enrichment unter falschem Titel, 45.3 Parseraufgabe und 45.4 Servernutzung. Das zentrale neue Kapitel ergänzt die Detailabschnitte. Die Parseraufgabe soll getrennt werden; die Enrichment-Umbenennung muss bestehende Verweise berücksichtigen.

## Verifikation und Archivierung

Die wichtigsten Codebefunde wurden an über das Web abgerufenen `wip`-Quellen nachgeprüft. Ein vollständiger Checkout und die Go-/Target-Tests waren hier nicht ausführbar; es wird kein unveränderlicher Commit-Snapshot behauptet. Vor der Umsetzung den tatsächlichen Commit festhalten und die jeweiligen Fälle reproduzieren. Details stehen in der Quellenprüfung.

Die Markdown-Dateien sind UTF-8. Relative Links funktionieren nach dem Entpacken bei unveränderter Ordnerstruktur. Für die unabhängige Archivierung das gesamte ZIP lokal sichern; `COMPLETE_HANDOVER.md` bietet zusätzlich eine einzelne durchsuchbare Lesefassung. `SHA256SUMS.txt` dokumentiert die Prüfsummen der Paketdateien.
