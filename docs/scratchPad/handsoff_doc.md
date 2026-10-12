# Dokumentationsübergabe

Stand: 12. Oktober 2026, Europe/Berlin. Auf dem anderen Rechner mit `handson doc` laden, Checkout prüfen und auf die nächste Nutzeranweisung warten.

## Aktueller Auftrag und Plan

Die Diskussion zur RM-Struktur ist abgeschlossen genug, um die Entscheidungen zu sichern. Dieser Auftrag erstellt ausschließlich die Übergabe und die Spezifikation für den Rechnerwechsel; er startet keine Dokumentationsmigration.

Aktueller Plan: **Modulares Reference Manual mit GitHub-Navigation und Stichwortindex**. Die detaillierte Spezifikation steht ausschließlich in [RM_Documentation_Spec.md](RM_Documentation_Spec.md).

Die bisherige Übergabe wurde unverändert nach [handsoff_doc_0.md](handsoff_doc_0.md) umbenannt. Sie bewahrt frühere Recherche und Messbefunde; ihre Vorschläge sind keine aktuelle Umsetzungsfreigabe. Insbesondere sind ihr `docs/src/`-Baum, die separate `structure.yml` und die frühere Prototyp-Reihenfolge durch den aktuellen Plan überholt.

## Verbindliche Entscheidungen und Grenzen

- Kapitel-Markdown unter `docs/rm/` wird die einzige gepflegte RM-Quelle. Die aktuelle Gesamtdatei wird bei einer später beauftragten Migration nach `docs/scratchPad/obsolete/` verschoben.
- Die Kapitelreihenfolge wird manuell in `rm/README.md` gepflegt. Hauptkapitel und Dateinamen bleiben unnummeriert; mdtoc erzeugt lokale Kapitel-ToCs und lokale Unterkapitelnummerierung.
- `rm/INDEX.md` wird mit KI-Unterstützung initial erstellt und redaktionell gepflegt. Keine verpflichtenden Begriffsmakros im Kapiteltext. Zentrale Begriffe sollen gelöscht oder ergänzt und ihre Links später per Tool oder KI aktualisiert werden können.
- Vorhandene Inhalte und aktive Verweise werden bei der einmaligen Migration erhalten und angepasst. Nicht zu Kapiteln passende Teile dürfen in `HEADER.md` und `FOOTER.md` liegen. Die konkrete Aufteilung ist noch nicht durchgeführt.
- `RM-search.txt` ist gewünscht als einfache Suchhilfe, aber noch nicht ausreichend spezifiziert. Ein vollständig verlinktes, zusammengefügtes Markdown-RM ist zurückgestellt.
- mdBook ist ein sinnvoller zusätzlicher HTML-Kandidat aus denselben Quellen, keine Voraussetzung für das Markdown-Layout. Konfiguration, Veröffentlichung und Freigabe zur Umsetzung sind offen. PDF aus Kapiteln wird ausdrücklich erst später diskutiert.
- Keine Änderungen an Quellcode, UM/Guide-Inhalten, Build-/Release-/Pages-Skripten, Abhängigkeiten oder bestehenden Archiven als Nebenarbeit. Geltende [Agent-Regeln](../../AGENTS.md) beachten. Kein Commit oder Push ohne ausdrückliche Anweisung.

## Tatsächlicher Checkout und Transferbedarf

- Branch: `wip`.
- HEAD: `90b240696fbbd612facee7917fc8d9acd7156ced` — `wip`.
- Arbeitsbaum unmittelbar vor unseren Dateiänderungen: sauber. Während der lesenden Bestandsaufnahme wechselte HEAD von `6aeb816238529c2ac8a1067b145d92ed65e5868f` auf den genannten Commit; die zunächst sichtbare Änderung an `scratchPad/scratchPad.md` ist jetzt darin enthalten und wurde von uns nicht bearbeitet.
- Gegenüber dem lokal vorhandenen `origin/wip`: vier Commits voraus. Kein Fetch oder Remote-Abgleich; diese Angabe beweist keinen aktuellen Remote-Stand.
- Noch uncommittet aus diesem Auftrag: neue Fassung von `docs/scratchPad/handsoff_doc.md`, unveränderte Vorgängerfassung als `docs/scratchPad/handsoff_doc_0.md` und neue `docs/scratchPad/RM_Documentation_Spec.md`.
- Im Checkout existiert weiterhin [TriceReferenceManual.md](../TriceReferenceManual.md); `docs/rm/` wurde noch nicht angelegt. Keine Kapitelmigration, Linkumschreibung oder Werkzeuginstallation durchgeführt.
- Frühere Medienoptimierung ist bereits in Git. Nicht erneut als unerledigte Aufgabe beginnen. Details und historische Performancebefunde stehen in der Vorgängerübergabe.

Die drei Übergabedateien manuell stagen und committen und den Commit für den anderen Rechner übertragen, normalerweise durch Push. Auch die vier bereits lokalen Commits müssen für denselben Stand übertragen werden, soweit sie remote noch fehlen. Keine zusätzlichen lokalen Messdaten oder Prototypdateien sind erforderlich. Wir haben kein `git add`, `git commit` oder `git push` ausgeführt.

## Relevante Dateien und Erkenntnisse

- Aktive Quellen: [RM](../TriceReferenceManual.md), [UM](../TriceUserManual.md), [Guide](../TriceGuide.md), [Medien/CLI-Hilfe](../ref/), [Doku-Einstieg](../README.md).
- Bestehende Abläufe: [mdtoc-Pflege](../../scripts/_310_refresh_trice_user_manual.sh), [PDF-Erzeugung](../../scripts/_320_generate_trice_user_manual_pdf.sh), [Pages-Workflow](../../.github/workflows/pages.yml), [Release-Konfiguration](../../.goreleaser.yaml). Abhängigkeiten vom bisherigen RM müssen vor einer späteren Migration erfasst werden; PDF-/Release-Umbau nicht stillschweigend mit erledigen.
- mdBook hat eine eingebaute Volltextsuche ohne Begriffsmakros. Seine `SUMMARY.md` könnte später aus der gepflegten `README.md` entstehen. Ein Stichwortindex ergänzt Volltextsuche, ersetzt sie nicht.
- `mdbook-indexing` benötigt Makros im Quelltext und passt deshalb nicht direkt zum gewünschten zentralen Index. Kein fertiges Werkzeug für exakt den beschlossenen INDEX-Ablauf wurde verifiziert.
- stitchmd wurde anhand Dokumentation und Quelltext untersucht, nicht installiert oder am RM ausgeführt. Kapitel-ToCs, explizite mdtoc-IDs und Link-/Bildpfade erfordern zusätzliche Behandlung; für reine Suche lohnt diese Baustelle derzeit nicht.

## Prüfungen und offene Arbeit

In dieser Sitzung wurden Recherche, lokale Bestandsaufnahme und zwei kleine mdtoc-Diagnosen über stdin durchgeführt; sie änderten keine Dateien. `mdtoc strip` entfernte im Beispiel ToC, Nummerierung und erzeugte Anker. `mdtoc generate --toc off --numbering on --anchor on` erhielt im Beispiel Nummerierung und Anker, entfernte aber nicht den äußeren Details-Container. Das sind keine Migrationstests und keine vollständige RM-Kompatibilitätsprüfung.

Prüfungen der jetzt geschriebenen Übergabedateien, ausgeführt aus `docs/`:

- `markdownlint --config ../.markdownlint.yaml scratchPad/handsoff_doc.md scratchPad/RM_Documentation_Spec.md` — PASS, Exitcode 0, keine Meldungen.
- `git diff --check` — PASS, Exitcode 0, keine Meldungen.
- `python -` mit lesendem Prüfsnippet über stdin — PASS: beide neuen Texte sind gültiges UTF-8 ohne BOM, haben einen abschließenden Zeilenumbruch und keine nachgestellten Leerzeichen; alle 21 relativen Links außerhalb von Codebeispielen existieren. Die umbenannte Vorgängerfassung ist bytegenau identisch mit `git show HEAD:docs/scratchPad/handsoff_doc.md`.
- Keine Testsuite, kein Browserbenchmark und kein mdBook-, PDF- oder Release-Build für die geplante Struktur ausgeführt. Diese Prüfungen bleiben offen; nichts wurde abgebrochen oder als bestanden angenommen.

Offen bleiben insbesondere das genaue INDEX-Pflegeformat, die Spezifikation von `RM-search.txt`, der Umgang bestehender Builds mit dem abgelösten RM und die optionale mdBook-Einrichtung. Die Spezifikation kennzeichnet diese Fragen ausdrücklich; sie sind keine stillschweigenden Implementierungsaufträge.

## Nächster Schritt auf dem anderen Rechner

Nach Übernahme des Git-Stands `handson doc` ausführen. Branch, HEAD, Arbeitsbaum und verlinkte Spezifikation prüfen. Danach auf die nächste Anweisung warten; nicht automatisch migrieren. Vor einem späteren Implementierungsauftrag die noch offenen Entscheidungen klären und den Umfang ausdrücklich festlegen.
