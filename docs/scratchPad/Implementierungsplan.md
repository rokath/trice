# Release-Vorbereitung und weiterer Arbeitsplan

Stand: 30. September 2026. Bestandsaufnahme auf Basis von Commit `9b4e2abb`, des lokalen Release-Tags `v1.3.0` und des abgeschlossenen Full-Testlaufs vom 29./30. September. Dieser Plan bleibt deutsch. Er erteilt **keinen Implementierungs-, Commit-, Issue- oder Release-Auftrag**.

Ziel ist ein verlässliches Release der bereits vorhandenen Funktionen. Weitere Features sind dafür nicht erforderlich. Vorrang haben nachgewiesene Fehler, vollständige Abnahme und verständliche, zutreffende englische Anwenderdokumentation.

## Stand und Aussagegrenzen

Untersucht wurden die CLI und ihre Hilfe, ID-Verwaltung und Generatoren, Bind und Insert/Clean einschließlich CE, Template-Parser, Decoder, strukturierte Ausgabe, Tags und Filter, Visualisierung, Transport- und Ausgabeabschluss, Target-Konfiguration und Testaufbau, Beispiele, aktives UM, README sowie Test- und Release-Workflows. Code und vorhandene Verhaltenstests wurden mit den dokumentierten Verträgen verglichen. Das ist eine breite statische Bestandsaufnahme mit konkreten Belegen, keine vollständige Fehlerfreiheitserklärung oder neue Hardware-Abnahme.

Der vom Benutzer gestartete Lauf `./scripts/testAll.sh full --no-stop` wurde nach seinem Abschluss anhand der vollständigen Protokolle ausgewertet. Es wurde kein weiterer Testlauf, Build, Formatter oder ID-Workflow gestartet. Die vier verbliebenen Beispiel-JSON-Dateien wurden semantisch mit HEAD verglichen und unverändert gelassen.

- Ergebnis laut `temp/log/testAll_summary.log`: **23 Schritte PASS, 2 Schritte FAIL**, Gesamtdauer **8 Stunden 12 Minuten 4 Sekunden**. Nur Schritt 630 (PC/Insert) und Schritt 640 (PC/Bind) scheitern. Die L432-Matrix mit 101 Konfigurationen besteht und braucht etwa **20 Minuten 14 Sekunden**.
- Beide PC-Workflows führen jeweils acht Bulk- und 63 Einzelzeilen-/Spezialkonfigurationen aus. Acht Bulk- und 57 Einzelzeilen-/Spezialläufe scheitern, sechs Spezialläufe bestehen. Die fehlgeschlagenen Tests heißen jeweils `TestTriceLog`; es sind keine fehlgeschlagenen Compileraufrufe.
- Sämtliche protokollierten Einzelzeilen-Abweichungen sind zwischen Insert und Bind identisch und fallen in zwei Gruppen: ein unerwünschtes automatisch ergänztes `untagged:` im ausgegebenen Meldungstext und einmal `Fisch` gegenüber tatsächlich ausgegebenem `Fish`. Nach der präzisierten Benutzerentscheidung bleibt `untagged` eine Klassifizierung und darf die Message nicht verändern; die Präfix-Erwartungen sind deshalb nicht pauschal zu erweitern. Der Bulk-Vergleich verschiebt nach dem ersten Längenunterschied weitere Ausschnitte und erzeugt dadurch umfangreiche Folgefehler. Einzelheiten und Abnahme stehen bei R01.
- Schritt 600 führt eigenständige Builds von `PC_log` und `G0B1_log` aus. Deren lokale TIL/LI-Dateien wurden aktualisiert, aber vom äußeren Snapshot nicht erfasst. Die vier verbliebenen JSON-Diffs sprechen für einmaligen Nachholbedarf nach Source-Erweiterungen, nicht für neue Inhaltsänderungen bei jedem unveränderten Wiederholungslauf. R13 trennt die bewusste Aktualisierung der Beispieltabellen vom Schutz des Test-Ausgangszustands.
- Schritt 550 meldet **91,1 % Go-Statement-Coverage**. Das ist weder ein Vergleich mit der Zielbranch-Baseline noch Coveralls-Zeilenabdeckung oder Target-C-Abdeckung.
- Die Compiler-/Decoder-Integrationstests für CE benötigen `TRICE_BIND_INTEGRATION=1`. Die normale Testauswahl aktiviert diese Tests nicht vollständig; Einzelheiten stehen bei R07.
- Aktuelle GitHub-Issues und Live-CI-Ergebnisse wurden nicht vollständig abgeglichen. Vor einer späteren Issue-Erstellung sind vorhandene Issues auf Dopplungen zu prüfen. Dieser Auftrag erstellt keine Issues.

Die erledigten A1–A10 und der CE-Folgeauftrag für Insert/Clean sind aus der offenen Liste entfernt. Ihre Details bleiben im [historischen Abschlussbericht](obsolete/Implementierungsplan_bis_A10.md). Die früheren M01–M16 und Handovers bleiben im vorhandenen Archiv. Ein historisch erledigter Implementierungsauftrag ersetzt keine heutige Regressionstest-Abnahme; die frühere A6-Notiz zur `untagged:`-Erwartung wird durch den bei R01 präzisierten Ausgabevertrag neu bewertet.

Vorhanden sind insbesondere:

- Structured Logging mit skalaren 8/16/32/64-Bit-Werten, `triceS`/`triceN`, Text, NDJSON und KV; benannte Pufferfelder bleiben ausdrücklich ausgeschlossen.
- CE für direkte, zeileneindeutige Bind-Stellen und für erkannte Insert/Clean-Aufrufe einschließlich statischer Wrapperdefinitionen. Insert/Clean verwendet den vollständigen Format-/Argumentsuffix, ohne Herkunftskommentare.
- Tag-Aliase, Gewichte, Auswahl, `untagged`, Ereignisstatistik, getrennte Diagnosen, Zeitstempel und Deltas, Visualisierung sowie gemeinsame generierte Ablage über `-genDir`.
- `generate -logC`, zusätzliche `*.oneline.json`-Ansichten und die PC-/G0B1-Feature-Beispiele.
- Erhaltene CE-PoCs einschließlich Wrapper-/Counter-Rebase-Untersuchung. Deren allgemeine produktive Integration ist weiterhin zurückgestellt.

## Gewichtung und Arbeitsreihenfolge

**Gewicht:** 5 = vor Release zu klären oder abzustellen; 4 = hoher Nutzen für Zuverlässigkeit, Dokumentation oder Testdauer; 3 = sinnvolle Wartung nach den dringenden Punkten; 2 = optionaler Ausbau; 1 = bewusst zurückgestellt.

**Aufwand:** S = kleine, abgegrenzte Änderung; M = mehrere zusammenhängende Änderungen mit Verhaltenstests; L = Architektur-/Buildänderung oder breiter Plattformnachweis. Das sind Schätzungen, keine Zeitversprechen. Fehlersuche kann eine Aufgabe vergrößern.

Die Reihenfolge bevorzugt kleine Aufgaben, berücksichtigt aber Abhängigkeiten. Nach der R01-Korrektur rückt R13 wegen der nachgewiesenen bleibenden JSON-Änderungen an die erste Stelle. Die vorhandenen IDs bleiben für Verweise erhalten. Unabhängige Dokumentationsarbeit kann während langer Tests erfolgen. Für Gewicht 5 reicht kein stilles Vertagen: Vor Release muss entweder die Korrektur abgenommen oder eine konkrete Einschränkung ausdrücklich entschieden und dokumentiert sein.

| Reihenfolge / ID | Aufgabe | Gewicht | Aufwand | Voraussetzung |
| --- | --- | ---: | --- | --- |
| R13 | Bleibende Beispiel-JSON-Änderungen nach Tests verhindern | 5 | M | Verursachender Buildpfad und Snapshot-Lücke bekannt |
| R02 | Falsche UM-Kommandos, Dateinamen und Links berichtigen | 5 | S | Keine |
| R03 | Fehlerstatus bei fehlgeschlagenem Clean erhalten | 5 | S | Keine |
| R04 | Aussage zum automatischen TIL-/LI-Nachladen klären | 5 | S; bei Wiederherstellung M | Produktentscheidung |
| R05 | Kompatibilitätsvertrag und Release-Version festlegen | 5 | S–M | Vergleich mit v1.3.0 |
| R06 | Testlaufzeit und tatsächlich ausgeführte Fälle erfassen | 4 | S | Erste Full-Messung liegt vor; dauerhaft erfassen |
| R07 | Vorhandene CE-/SL- und Beispielprüfungen verbindlich ausführen | 5 | M | R01, R13; erforderliche Compiler |
| R08 | Loglauf sauber beenden: Signale, Timer und Ressourcen | 4 | M | Gezielte Reproduktion |
| R09 | Endliche Puffer ohne pauschale Wartezeit abschließen | 4 | M | R01, R06; mit R08 abstimmen |
| R10 | MVP-/Aufgabenreste und doppelte Anwenderdokumentation bereinigen | 4 | M | R02, R04 |
| R11 | SL- und CE-Kapitel vollständig ins Englische übertragen | 5 | M–L | R02, R10 |
| R12 | Einstieg, Beispiele und unterstützte Grenzen vervollständigen | 4 | S–M | R05, R10, R11 |
| R14 | Falls v2 beschlossen: Go-Modul und Installationswege vorbereiten | 5, bedingt | M–L | R05 |
| R15 | Release Notes und Prüfung der ausgelieferten Artefakte | 5 | M | R05, R07, R11, R12; ggf. R14 |
| R16 | Abschließende Release-Abnahme | 5 | M; lange Laufzeit | R01–R05, R07, R11, R13, R15; alle aufgenommenen Korrekturen |

Die weiter unten aufgeführten P- und F-Aufgaben sind kein Grund, ein ansonsten abgenommenes Release um neue Features zu vergrößern.

## Erledigte Korrekturen

### Kein automatisch erzeugtes untagged-Präfix ausgeben

**R01 · Gewicht 5 · Aufwand M · Umsetzung abgeschlossen; abschließende Full-Matrix bei R16**

Die sichtbare Ausgabe enthält kein synthetisches `untagged:` mehr. Für ungetaggte oder unbekannt getaggte Ereignisse verhindert die Textausgabe zugleich, dass ein Doppelpunkt im Benutzertext nachträglich als Format-Tag interpretiert wird. JSON/KV verwenden weiter `tag=untagged` als Metadatum, ohne die Message zu verändern. Die `Fish`-Erwartung ist korrigiert. Die C-gestützten Vergleichsschleifen brechen bei der ersten Abweichung pro Konfiguration ab; `--no-stop` steuert weiterhin die Fortsetzung im äußeren Test-Worker. Gezielte Regressionstests decken die unabhängigen Fälle ab. Die vollständige Release-Matrix bleibt ausdrücklich Aufgabe R16.

**Gezielte Abnahme am 30. September:** Translator-Paket und die einschlägigen Emitter-Tests bestehen. Die Fehlerprobe erzwingt je einen falschen Bulk-, Einzelzeilen- und Direct-/Deferred-Vergleich und bestätigt jeweils nur eine Diagnose trotz `TRICE_TEST_NO_STOP=1`. Die kurzen PC-Workflows bestehen mit Insert und Bind. Eine komplette Ringpuffer-Einzelzeilenkonfiguration sowie die kombinierte Direct-/Deferred-Konfiguration und alle drei zuvor betroffenen Spezialfälle bestehen unter Insert; die kombinierte Konfiguration und Spezialfälle bestehen auch unter Bind. Der unabhängige `Fish`-Fall ist Teil der Ringpuffer-Einzelzeilenprüfung. Ein ad hoc Bind-Mehrpaketlauf ohne den vorgesehenen Go-Cache-Reset lieferte zunächst einen leeren Direct-Wert; der kontrollierte Bind-Einzellauf nach `go clean -cache -testcache` bestand. Für die abschließende Matrix den regulären PC-Test-Worker mit seinem Cache-Reset verwenden. Markdownlint für das geänderte UM besteht. Die vollständige Full-Matrix bleibt bei R16.

**Verbindliche Ausgaberegel: Trice darf die Zeichenfolge `untagged:` niemals selbst zu einer Logmeldung hinzufügen.** Sie darf in der sichtbaren Meldung nur vorkommen, wenn die Anwendung sie selbst als Text geliefert hat. Das gilt für Text, JSON und KV und ausdrücklich auch bei `-color off`. Bei `trice("hi")` lautet die sichtbare Message deshalb `hi`, ohne `untagged:` vor oder innerhalb des Textes. Die interne Klassifizierung heißt trotzdem `untagged`; JSON/KV dürfen dafür ein separates Tag-Metadatum mit dem Wert `untagged` ausgeben. Filterung, Gewicht, Farbe und Ereignisstatistik verwenden weiterhin diese Klassifizierung. Ein vom Anwender tatsächlich gelieferter Text `untagged:` darf nicht pauschal entfernt werden.

| Format | Ausgabe für `trice("hi")`, ohne weitere Metadaten |
| --- | --- |
| Text | `hi` |
| JSON | `{"tag":"untagged","message":"hi"}` |
| KV | `tag=untagged message="hi"` |

Bei einem unbekannten Präfix wie `trice("mgs:blah")` bleibt die Message `mgs:blah`, während das Tag-Metadatum `untagged` lautet. Dadurch bleibt auch ein möglicher Tippfehler sichtbar. Die bestehenden Darstellungsregeln für ausdrücklich geschriebene bekannte Tags bleiben erhalten; ebenso die Regeln für Leerraum und Zeilenabschluss. Anwendertext darf nicht durch pauschales Entfernen gleichlautender Textstücke verändert werden.

Die Protokolle `temp/log/_630_test_pc_targets_insert.log` und `temp/log/_640_test_pc_targets_bind.log` enthalten pro Workflow 14.655 fehlgeschlagene Einzelzeilen-/Spezialvergleiche. Der Abgleich aller erwarteten und tatsächlichen Strings ergibt in beiden Workflows dieselben Abweichungen. Die folgende Tabelle beschreibt ausschließlich den **beobachteten Fehler**: `tatsächlich` ist keine gewünschte neue Testerwartung. Insbesondere bleibt `Hello World!` der richtige Erwartungswert; `untagged:Hello World!` ist der zu behebende Ist-Wert.

| Ursache | Konkreter Unterschied | Häufigkeit je Workflow |
| --- | --- | ---: |
| Unerwünschtes automatisch ergänztes Präfix bei `-color off` | Erwartet `Hello World!`, tatsächlich `untagged:Hello World!`; entsprechend bei anderen ungetaggten Meldungen | 14.581 |
| Veralteter Feldname im manuellen JSON-Beispiel | Erwartet `... Birn:2, Fisch:2.781000}`, tatsächlich `... Birn:2, Fish:2.781000}` | 74 |

Das unerwünschte Präfix betrifft die Vergleiche mit den `//exp:`-Erwartungen in [triceCheck.c](../../_test/testdata/triceCheck.c), außerdem die Spezialfälle `dblB_de_protect_tcobs_ua`, `ringB_de_protect_tcobs_ua` und `modify_for_debug`. Bei zwei getrennten ungetaggten Aufrufen mit `Hello ` und `World!` muss die zusammengesetzte Textausgabe `Hello World!` bleiben. Das derzeit beobachtete `untagged:Hello untagged:World!` verletzt diesen Vertrag; die Klassifizierung beider Ereignisse bleibt trotzdem jeweils `untagged`. Der unabhängige `Fish`-Unterschied stammt aus `exampleOfManualJSONencoding()`: Der Formatstring verwendet bereits `Fish`, nur die Erwartung an der aufrufenden Teststelle enthält noch `Fisch`.

Je gewöhnlicher Einzelzeilenkonfiguration sind es 197 Präfixabweichungen und eine `Fish`-Abweichung. Die kombinierten Direct-/Deferred-Konfigurationen prüfen beides zweimal. Die drei genannten Spezialtests haben jeweils eine Präfixabweichung. Die hohen Summen entstehen durch Wiederholung über die Konfigurationen, nicht durch ebenso viele verschiedene Ursachen.

[Der Bulk-Vergleich](../../_test/testdata/cgoPackage.go) schneidet nach der erwarteten Textlänge weiter. Bereits das erste zusätzliche `untagged:` verschob in den früheren Protokollen die folgenden Ausschnitte; jeder der acht Bulk-Läufe meldete deshalb 1.743 Textunterschiede und Restdaten. Diese Folgefehler waren kein unabhängiger Nachweis eines Framing- oder Übertragungsdefekts. Nach der Korrektur besteht der gezielt ausgeführte Ringpuffer-Bulk-Lauf unter Insert und Bind; alle Bulk-Konfigurationen bleiben Teil der Full-Matrix bei R16.

Umgesetzt: Die automatisch zugewiesene Klassifizierung verändert den Message-Text in Text, JSON und KV nicht mehr. Die vorhandenen präfixfreien Erwartungen blieben erhalten; die separate `Fisch`-Erwartung lautet jetzt `Fish`. Weder Testausgaben noch Erwartungen wurden pauschal um `untagged:` bereinigt beziehungsweise erweitert. Verhaltenstests und die betreffenden UM-Aussagen folgen dem präzisierten Vertrag.

**Früher Testabbruch:** Vor der Korrektur koppelte `keepCheckingAfterFailure()` das Weiterprüfen innerhalb eines Pakets an `TRICE_TEST_NO_STOP=1`. Dadurch meldete der Bulk-Vergleich nach dem ersten Längenfehler Tausende Folgeunterschiede; die Einzelzeilenprüfung führte noch alle übrigen Logaufrufe aus. Die Bulk-, Einzelzeilen- und kombinierten Direct-/Deferred-Tests brechen jetzt innerhalb **jeder Konfiguration beim ersten fehlgeschlagenen Vergleich ab**. Die Diagnose nennt Index, Source-Zeile sowie erwarteten und tatsächlichen Wert und markiert die Konfiguration als fehlgeschlagen. Der äußere [PC-Test-Worker](../../scripts/_160_pc_target_test_worker.sh) diagnostiziert bei `--no-stop` fehlgeschlagene Bulk-Konfigurationen weiterhin einzeln und prüft danach die **nächste Konfiguration**; ohne `--no-stop` bleibt der bisherige Abbruch des Workflows nach Fehler und Gegenprobe. `-failfast` allein ersetzt den Rücksprung aus einer bereits laufenden Vergleichsschleife nicht.

Ein erster Fehler darf unabhängige Fehler nicht dauerhaft verdecken: Der `Fish`-Fall und die `untagged`-Ausgabe sind auch in gezielten, voneinander unabhängigen Verhaltenstests abgesichert. Im erfolgreichen Durchlauf bleiben sämtliche bisherigen Testfälle und Assertions aktiv; nur die Fehlerdiagnose endet pro Konfiguration früher. Der Bulk-Vergleich zählt nach einem Längenunterschied keine verschobenen Ausschnitte als eigenständige Fehler weiter.

**Abnahme:** Ungetaggte und unbekannt getaggte Meldungen in Text, JSON und KV prüfen, einschließlich `-color off`, zusammengesetzter Textfragmente und unverändertem Leerraum: Ohne wörtliches `untagged:` im Anwendungstext enthält keine ausgegebene Meldung diese Zeichenfolge; ein vom Anwender gelieferter gleichlautender Text bleibt erhalten. JSON/KV enthalten `tag="untagged"` und den unveränderten Message-Text; explizite bekannte Tags folgen weiterhin ihren bisherigen Regeln. Nachweisen, dass Auswahl über `untagged`, Gewichtsschwellen, Farbe und Ereignisstatistik weiterhin auf der Klassifizierung beruhen. Einen absichtlich ausgelösten frühen Fehler in Bulk, Einzelzeile und Direct/Deferred prüfen: jeweils genau ein aussagekräftiger Vergleichsfehler pro Konfiguration, aber unter `--no-stop` läuft die nächste Konfiguration weiter und der Gesamtschritt bleibt FAIL. Den unabhängigen `Fish`-Fall separat prüfen. Eine Bulk-/Einzelzeilenkonfiguration, eine kombinierte Direct-/Deferred-Konfiguration und die drei Spezialfälle gezielt nach Insert und Bind prüfen. Anschließend vollständige Matrix im finalen Lauf. Alle bisherigen Assertions und Konfigurationen bleiben erhalten; eventuelle dann noch sichtbare Abweichungen getrennt untersuchen.

## Konkrete Aufgaben vor dem Release

### Kopierbare Dokumentationsbeispiele berichtigen

**R02 · Gewicht 5 · Aufwand S · Befunde bestätigt**

Konkrete Fundstellen im [UM](../TriceUserManual.md):

- CE-Einstieg „Position und Geschwindigkeit“: Der alternative `info:`-Aufruf enthält `...%f",, pos.x, pos.y, m/s=%f"...` und ist syntaktisch ungültig. Gewollt ist etwa `-ce 'info:", x={}, y={}, m/s=%f", pos.x, pos.y, aFloat(velocity)'`.
- „Trice Generate / Generating a Trice ABC Function Pointer List“ nennt `deviceX_abc.h/.c`. [Generator und CLI](../../internal/id/generateAbc.go) verwenden bei `-abc deviceX` tatsächlich `generated/deviceX.h` und `generated/deviceX.c`; explizite Zielpfade behalten ihren Ort.
- „C#-Code“ empfiehlt `-tilCS`, obwohl [die aktuelle CLI](../../internal/args/init.go) keinen solchen Schalter registriert. Veraltetes Beispiel aus der aktiven Anleitung entfernen oder ausdrücklich historisch einordnen; keinen neuen Generator allein zur Rettung des Textes implementieren.
- Das Testkapitel nennt unter anderem die nicht vorhandenen `examples/buildAllTargets.sh` und `renewIDs_in_examples_and_test_folder.sh`, außerdem `_trice` statt `_test`. Durch die heutigen Einstiegspunkte ersetzen.
- Das Releasekapitel fasst mehrere Git-Kommandos in eine einzelne Befehlszeile ohne Trennzeichen zusammen, etwa `git checkout main git pull origin main`. Einzelne kopierbare Schritte daraus machen.
- [PC_features/README.md](../../examples/PC_features/README.md) verlinkt `G0B1_features/README.md`; versioniert ist `ReadMe.md`. Das funktioniert auf einem Dateisystem mit beachteter Groß-/Kleinschreibung nicht.

**Abnahme:** Die betroffenen Beispielwerte werden mit der aktuellen CLI in einer isolierten Fixture geprüft. Dateinamen, relativer Aufrufort und erzeugte Dateien passen zusammen. Links werden mit der tatsächlich versionierten Schreibweise geprüft. Kein pauschales Ausführen aller Shell-Blöcke des UM, insbesondere keiner Release-/Git-Kommandos.

### Fehlgeschlagenes Clean darf keinen Erfolg melden

**R03 · Gewicht 5 · Aufwand S · Befund bestätigt**

In [L432_inst/build.sh](../../examples/L432_inst/build.sh), [F030_inst/build.sh](../../examples/F030_inst/build.sh), [G0B1_inst/build_with_clang.sh](../../examples/G0B1_inst/build_with_clang.sh) und [G0B1_features/build_with_clang.sh](../../examples/G0B1_features/build_with_clang.sh) steht sinngemäß:

~~~sh
if ! cleanup_command; then
  clean_status=$?
  return "$clean_status"
fi
~~~

`$?` ist dort das Ergebnis der Negation und daher 0. Eine isolierte Shellprobe mit Fehlercode 23 bestätigt das. Dadurch kann ein fehlgeschlagenes Clean als Erfolg enden. Auch das vereinfachte Cleanup-Muster im UM muss einen Fehler ausdrücklich weiterreichen; `set -e` allein genügt im Aufruf unter `if ! ...` nicht.

**Abnahme:** Verständliche Shell-Verhaltenstests mit erfolgreichem Build plus fehlgeschlagenem Clean, bereits fehlgeschlagenem Build, SIGINT und SIGTERM. Der ursprüngliche Build-/Signalfehler bleibt erhalten; ein alleiniger Cleanup-Fehler erzeugt einen Fehlerstatus und eine zutreffende Meldung.

### Automatisches Nachladen zutreffend beschreiben

**R04 · Gewicht 5 · Aufwand S für Dokumentation · Befund bestätigt, Produktentscheidung offen**

Das UM verspricht unter „Easy-to-use“, dass ein laufender Logger aktualisierte `til.json` automatisch nachlädt und nicht neu gestartet werden muss. In [handler.go](../../internal/args/handler.go) sind jedoch sowohl `go ilu.FileWatcher(...)` als auch `go li.FileWatcher(...)` auskommentiert. Die Tabellen werden vor der Eingabeschleife geladen.

Zunächst entscheiden: Ist ein Neustart derzeit die unterstützte Bedienung, oder gehört funktionierendes Live-Nachladen zum Release-Vertrag? Die kleine Korrektur ist eine ehrliche Anleitung zum Neustart. Eine Wiederherstellung ist ein eigener, größerer Fehlerbehebungsauftrag; vorhandenen Watcher-Code nicht ungeprüft aktivieren.

Bei Wiederherstellung sind atomarer Dateiersatz durch Bind, mehrere rasche Aktualisierungen, vorübergehend ungültiges JSON, unverändert weiter gültige Tabellen, Synchronisierung von TIL/LI, strukturierte Schemas, Visualisierung und sauberes Beenden zu prüfen. Die vorhandenen Fake-Write-Event-Tests in [fileWatcher_test.go](../../internal/id/fileWatcher_test.go) ersetzen diese Integration nicht.

**Abnahme:** Dokumentation und beobachtetes Verhalten stimmen überein. Wiederholtes reales Speichern/Ersetzen wird getestet, falls Nachladen zugesagt wird.

### Kompatibilitätsvertrag und Versionsnummer entscheiden

**R05 · Gewicht 5 · Aufwand S–M · Entscheidung vor Tag und Veröffentlichung**

Empfehlung: **v2.0.0 ist beim aktuellen Stand sachlich gut begründbar**, nicht nur als Werbesignal. Auch wenige absichtliche inkompatible Änderungen an veröffentlichten Schnittstellen können einen Major-Schritt rechtfertigen. Neue Features allein würden nach [Semantic Versioning](https://semver.org/spec/v2.0.0.html) für eine Minor-Version genügen.

Der lokale Vergleich mit `v1.3.0` zeigt bereits folgende relevante Änderungen:

| Vertrag | Beobachtete Änderung / zu dokumentierende Folge |
| --- | --- |
| CLI für C-Generierung | `-tilC` entfällt; `-logC` hat einen auf aktuelle Source-Stellen bezogenen Vertrag. |
| CLI für Location-Daten | `-liPath` entfällt; `-liRoot` und `-liMaxDirs` trennen Speicherung und Darstellung. Das alte `Path`-Feld entfällt. |
| User-Tags | `-ulabel a:b` registriert nicht mehr zwei Tags. Je Tag eine Option; Doppelpunkt für Gewicht/Farbe. |
| Formatstrings | Literale `{` und `}` müssen als `{{` und `}}` geschrieben werden. Der Decoder interpretiert auch geladene historische TIL-Strings mit dem neuen Template-Parser. |
| Tag-Auswahl und Darstellung | Eindeutige Aliase, gewichtete Schwellen und strengere Validierung können bestehende Aufrufe verändern. `untagged` klassifiziert fehlende/unbekannte Tags; das derzeit automatisch im Meldungstext sichtbare Präfix ist gemäß R01 zu korrigieren und kein beabsichtigter Release-Vertrag. |
| Generierte Ablage | `./generated` relativ zum Aufrufverzeichnis und `-genDir` werden einheitlich verwendet; einfache ABC-Zielnamen landen dort. |

Wichtig: `-buildDir`/`-bindDir` waren Zwischenstände der neuen Arbeit und sind nicht automatisch Brüche gegenüber einem veröffentlichten Release. Release Notes müssen veröffentlichte Änderungen von unveröffentlichten Umbenennungen unterscheiden.

Die pauschale Aussage „die neueste TIL dekodiert alle älteren Firmwares“ ist deshalb auf die unterstützten Template-/Tool-Versionen zu präzisieren. Ein altes wörtliches `"{x}"` wird heute als Feld gelesen; `"{1,2}"` wird abgewiesen. Das unveränderte binäre Drahtformat allein garantiert keine vollständige Host-/Wörterbuchkompatibilität. Alte Firmware, ihre Wörterbücher und passende Decoder-Versionen müssen reproduzierbar zuordenbar bleiben.

**Kein Migrationsprogramm und kein erneutes `-migrationBraces`.** Die früher verworfene Migration bleibt ausgeschlossen. Es geht um eine ehrliche Kompatibilitätsbeschreibung und gezielte Vergleichstests, nicht um still eingeführte Kompatibilitätsmechanismen.

**Abnahme:** Versionsentscheidung und unterstützte Kombinationen aus Target, TIL, Hosttool und CLI sind schriftlich festgehalten. Bei v2 folgt R14. v1.4.0 ist nur plausibel, wenn der vereinbarte öffentliche Vertrag tatsächlich rückwärtskompatibel bleibt; die kleine Zahl der Brüche allein ist kein Argument dafür.

### Tatsächliche Testarbeit und Laufzeit erfassen

**R06 · Gewicht 4 · Aufwand S · Erste Messung vorhanden, dauerhafte Erfassung noch offen**

[Der Runner](../../scripts/_110_test_runner.sh) zeigt grobe Arbeitsgewichte und die Gesamtdauer, aber keine vollständige gemessene Dauerübersicht je Schritt. Der abgeschlossene Lauf vom 29./30. September liefert folgende erste Messgrundlage. Die Abschnitte wurden aus den Startzeitpunkten der Schritte und dem Ende des Gesamtlaufs berechnet; sie enthalten Vorbereitung und Wiederherstellung und sind auf Sekunden aufgelöst.

| Abschnitt | Dauer | Ergebnis |
| --- | --- | --- |
| Schritte 400–610 zusammen | 00:07:13 | PASS |
| Schritt 620, L432 mit 101 Konfigurationen | 00:20:14 | PASS |
| Schritt 630, PC/Insert | 03:52:08 | FAIL |
| Schritt 640, PC/Bind | 03:52:29 | FAIL |
| Gesamtlauf | 08:12:04 | FAIL |

Die beiden PC-Schritte belegen damit rund **94,4 Prozent** der Gesamtdauer. Pro ID-Workflow wurden 63 verschiedene Pakete in 71 Paketaufrufen geprüft: acht Bulk-Läufe und 63 Einzelzeilen-/Spezialläufe. Die acht Einzelzeilen-Gegenproben nach Bulk-Fehlern wurden in der anschließenden regulären Phase nicht nochmals ausgeführt. Das ergibt über beide Workflows 142 Paketaufrufe, davon 130 fehlgeschlagen und zwölf bestanden. Die Fehlerdiagnose auszulassen wäre daher im Full-Lauf kein Ersatz für die Optimierung des eigentlichen Logpfads.

Aufgabe: Zeiten, ausgeführte Konfigurationen, Testmodi, Compiler und Cachezustand künftig direkt und kompakt im Abschlussbericht erfassen. Die grobe Prozentanzeige ist keine Zeitmessung. Diese Werte stammen aus einem Fehlerlauf auf einem konkreten Rechner, nicht aus einer plattformunabhängigen Benchmark.

**Abnahme:** Für denselben Stand liegen nachvollziehbare Zeiten und Falllisten vor: Bind/Insert, Bulk/Einzelzeile, Target-Konfigurationen, Go- und Compilerprüfungen. Fehlerläufe und erfolgreiche Läufe werden getrennt betrachtet. Kein belastbares Beschleunigungsversprechen vor dem Vergleich.

### Vorhandene neue Integrationstests in die Standardabnahme aufnehmen

**R07 · Gewicht 5 · Aufwand M · Lücke in der Testauswahl bestätigt**

[Schritt 500](../../scripts/_500_test_bind.sh) setzt `TRICE_BIND_INTEGRATION=1`, wählt aber nur fünf ältere Bind-Tests in `internal/id` aus. Die normalen Go-/Coverage-Läufe setzen diese Variable nicht. Damit fehlen in der regulären Auswahl insbesondere die vorhandenen [CE-Target-/Decoder-Tests](../../internal/args/context_enrichment_test.go) `TestContextEnrichmentTargetToDecoder` und `TestContextInsertCleanTargetToDecoder`. Auch die CE-PoCs werden dadurch nicht vollständig ausgeführt.

Die neuen [PC-Ausgabeprüfungen](../../examples/PC_features/check_output.sh) und [G0B1-Buildprüfungen](../../examples/G0B1_features/check_build.sh) werden von den untersuchten Test-/CI-Einstiegspunkten ebenfalls nicht aufgerufen. Normale SL-Unit-Tests und Teile der C-Matrix sind bereits vorhanden; diese müssen nicht neu erfunden werden.

Aufgabe: Bestehende Prüfungen einer klaren, dokumentierten Auswahl zuordnen. Erforderliche Compiler/clangd erkennen; im Release-Lauf darf ein fehlendes Pflichtwerkzeug nicht als bestandene Abnahme erscheinen. Den großen experimentellen Rebase-PoC getrennt von produktiver CE-Unterstützung ausweisen. Neue Beispielprüfungen müssen isoliert laufen oder ihren Ausgangszustand exakt wiederherstellen.

**Abnahme:** Protokolle nennen die tatsächlich ausgeführten produktiven CE-/SL-End-to-End-Tests und Beispielprüfungen. C/C++-Records, Text/JSON/KV, Insert/Clean-Rücknahme, Bind, Abschaltung und einmalige Argumentauswertung sind enthalten. Fehlende Plattformnachweise werden offen benannt.

### Ressourcen und Signalbehandlung pro Loglauf abschließen

**R08 · Gewicht 4 · Aufwand M · Konkrete Codepfade, Auswirkung gezielt messen**

[Translate](../../internal/translator/translator.go) startet pro Aufruf einen Signal-Handler mit Ticker. Bei normalem EOF ist kein Abbruchpfad für diese Goroutine, kein `signal.Stop` und kein explizites `ticker.Stop` vorhanden. Die Einzelzeilentests starten den Logger sehr häufig im selben Prozess.

Zusätzlich sind `binaryLogger.Close` und `bytesViewer.Close` in [receiver.go](../../internal/receiver/receiver.go) wirkungslose Methoden. Mit vorgeschalteten Wrappers muss die tatsächliche Schließung von Eingabe und Binärdatei geprüft werden; ein `defer` auf der danach veränderten Reader-Variable garantiert das nicht.

**Abnahme:** Wiederholte endliche Logläufe hinterlassen keine wachsende Zahl von Signalregistrierungen, Goroutinen oder Dateihandles. EOF, Lesefehler, Schreibfehler und SIGINT/SIGTERM schließen die jeweils besessenen Ressourcen genau einmal. Die bisherigen Statistiken, Diagnosen und Exitcodes bleiben fachlich erhalten. Tests verwenden beobachtbare Abschlussbedingungen statt bloßer Sleeps.

### Wartezeit bei endlichen Eingaben entfernen

**R09 · Gewicht 4 · Aufwand M · Laufzeitbefund bestätigt, Speedup einer Korrektur noch nicht gemessen**

In [decodeAndComposeLoopOutput](../../internal/translator/translator.go) endet ein vordefinierter Buffer erst, wenn seit Beginn mehr als 100 ms vergangen sind; zusätzlich existiert eine 100-ms-Pause nach wiederholten leeren Reads. [Die Einzelzeilenprüfung](../../_test/testdata/cgoPackage.go) startet den vollständigen Logpfad für jede Erwartung neu. Der abgeschlossene Testlauf erfasst 1.745 Erwartungen pro gewöhnlichem Durchlauf. Allein 100 ms je Aufruf entsprechen rechnerisch rund 175 Sekunden pro solcher Konfiguration, noch ohne übrige Arbeit.

Gemessen wurden pro ID-Workflow 34 gewöhnliche Konfigurationen mit jeweils etwa 183–185 Sekunden und 20 kombinierte Direct-/Deferred-Konfigurationen mit jeweils etwa 365 Sekunden; hinzu kommen neun kurze Spezialläufe. Die acht zusätzlichen Bulk-Prüfungen brauchten rund eine halbe Sekunde pro Paket. Über beide Workflows entsprechen schon die 100 ms für `2 × (34 + 2 × 20) × 1.745` Logaufrufe rechnerisch **7 Stunden 10 Minuten 26 Sekunden**. Die gemessene PC-Gesamtdauer beträgt 7 Stunden 44 Minuten 37 Sekunden einschließlich Builds und Verwaltung. Damit ist R09 der erste Optimierungsansatz vor Parallelisierung oder Cacheumbau. Das ist noch kein gemessener Speedup einer Korrektur; beide Testarten und ihre unterschiedlichen Prüfziele bleiben erhalten.

Aufgabe: Endliche Buffer-/Dateiquellen anhand ihres tatsächlichen Endes abschließen. Vorher prüfen, wie Decoder gepufferte Records nach einem Eingabe-EOF noch ausgeben. Live-Quellen und `TCP4BUFFER` nicht allein wegen eines kurzzeitig leeren Reads beenden.

**Abnahme:** Identische Records, Diagnosen, Zeitstempel, Auswahl und Abschlussfragmente in beiden ID-Workflows. Fälle für leere Eingabe, letzten Record mit gleichzeitigem EOF, mehrere intern gepufferte Records, fragmentierte Eingabe, verkürztes Paket, Schreibfehler und Live-Pausen. Zeitgewinn mit unveränderter Fallzahl nachweisen; keine künstliche Kürzung von `testLines`.

### Anwenderdokumentation von Entwicklungsständen befreien

**R10 · Gewicht 4 · Aufwand M · Befunde bestätigt**

Im aktiven UM stehen MVP-Bezeichnungen sowohl im `-vis)-Kapitel als auch ausführlich im Bind-Kapitel. Der CE-Anhang enthält A9/A10 im Fließtext und in Überschriften. Das [separate deutsche Bind-Manual](../TriceBind/Trice_bind_90_MVP_User_Manual.md) und die dortige README präsentieren parallel eine weitere normative Anwendersicht.

Aufgabe: Das UM zur eindeutigen Anwenderreferenz machen. Aktuelle Grenzen konkret benennen; „MVP“ nicht blind durch „vollständig unterstützt“ ersetzen. Historische Architekturvergleiche müssen als solche erkennbar bleiben und dürfen aktuelle Wrapper-Unterstützung nicht widersprechen. Implementierungsaufträge/Entwurfsberichte nach Prüfung aus dem aktiven Einstieg nehmen; wertvolle Begründungen erhalten.

Die CE-PoC-Ergebnisse bleiben wie beauftragt im kapitelinternen Anhang, einschließlich reproduzierbarer Testreferenzen und ihrer Aussagegrenzen. A9/A10 werden dort durch verständliche Bezeichnungen wie „Nachweis für direkte Logstellen“ und „Produktive Unterstützung“ ersetzt. Testnamen und Experimentpfade werden nicht nur wegen eines historischen Namens umbenannt.

**Abnahme:** Aktive Anwendertexte enthalten keine unerklärten Arbeitsauftragsnummern oder überholten MVP-Status. Vorhandene Archive bleiben unangetastet. Kommentarblöcke und historische Changelogs werden nicht als neue Produktanforderungen behandelt.

### Die beiden neuen UM-Kapitel ins Englische übertragen

**R11 · Gewicht 5 · Aufwand M–L · Beauftragung der späteren Umsetzung erforderlich**

Die derzeitigen Kapitel „Strukturiertes Logging“ und „Trice Context Enrichment“ vollständig übersetzen, einschließlich Tabellen, Einschränkungen, Beispiele, Fehlererklärungen und CE-PoC-Anhang. Auch den verbleibenden deutschen CE-Absatz unter „Future Development“ angleichen.

Vorher die deutschen Originale als datierte, vollständige Kapitelkopien unter `docs/scratchPad/obsolete/` sichern. Sie werden anschließend historische Referenzen, keine parallel gepflegten Manuals. Bereits dort liegende alte Drafts nicht überschreiben. Originale vor der englischen Bearbeitung sichern; spätere fachliche Korrekturen müssen im Diff zur Archivfassung nachvollziehbar sein.

Bei der Übersetzung besonders erhalten: Bedeutung von `message` und Leerraum, flache Punktnamen, Typen/NaN/64-Bit-Zahlen, getrennte `ts16`/`ts32`-Metadaten, CE-Regelreihenfolge, exakter Suffix-Match, lokale Sichtbarkeit, einmalige Auswertung, Ausschluss benannter Pufferfelder sowie die unterschiedlichen Bind-/Insert-Grenzen.

**Abnahme:** Vollständiger fachlicher Vergleich mit den deutschen Originalen und dem getesteten Verhalten. Source-/CLI-/JSON-Beispiele bleiben ausführbar und inhaltlich gleich, soweit nicht R02 einen Fehler korrigiert. Überschriften zunächst nur mit `#`; ToC, Nummern und Anker erzeugt später mdtoc. Alle aktiven internen Verweise auf übersetzte Überschriften anpassen. Der in Fehlermeldungen genannte Suchbegriff `bind-limits` bleibt erhalten.

### Einstieg, Beispielübersicht und Zusagen vervollständigen

**R12 · Gewicht 4 · Aufwand S–M**

Die Root-[README](../../README.md) nennt Bind noch experimentell und unveröffentlicht; das muss zum gewählten Release-Status passen. Sie verweist bei Structured Logging noch auf einen auskommentierten Future-Draft. SL und CE gehören mit kurzen funktionierenden Beispielen und Links zu den beiden Feature-Touren in den Einstieg und die Feature-Übersicht.

Weitere konkrete Ergänzungen:

- Ein kleines Verzeichnisbeispiel für `-genDir`: relativ zum Aufrufverzeichnis, Sidecars im Include-Pfad, persistente TIL/LI gegenüber generierten Dateien, `-logC` und ABC-Ausgaben.
- Erklären, warum beim G0B1-Beispiel auch gemeinsame Quellen Sidecars erzeugen können, obwohl ihre Demo-Funktion nicht aufgerufen wird: Scan-/Buildumfang und tatsächliche Laufzeitaufrufe sind verschiedene Dinge.
- Zum Aufräumen keine pauschale Löschung von `generated` empfehlen: Bei ABC kann dort eine vom Benutzer bearbeitete Auswahl-Headerdatei liegen. Alte Sidecars können außerdem historische ID-Evidenz liefern; Details siehe F01.
- Compiler-Zusagen getrennt für gewöhnliches Bind, direkte CE und experimentelles Rebase aufführen. C++20 mit strengen Warnungen scheitert laut vorhandenem PoC schon am Enum-Rebase; MSVC/IAR/armclang sind dort nicht nachgewiesen. Cross-Compile ist keine MCU-Laufzeitabnahme.
- Das Feature „keine dynamische Speicherverwaltung“ auf den Trice-Target-Loggingpfad beziehen. Stack-Puffer sind keine statischen Objekte; Hosttool, RTOS und benutzereigene CE-Funktionen sind nicht von derselben Zusage umfasst.
- ABC-Einstieg mit einer kurzen Karte von `NodeLib`, Auswahl-Header, generierter C-Tabelle und Buildausgabe erklären. Die heutige Dokumentation weiterverwenden; keine allgemeine Skript-Neuorganisation erforderlich.

**Abnahme:** Ein neuer Anwender findet einen PC-Einstieg, versteht die Ablage und kann CE/SL sowie ihre Grenzen ohne Kenntnis von A-/M-Aufträgen ausprobieren. Release-Zusagen decken sich mit nachgewiesenen Plattformen und Funktionen.

### Testläufe dürfen keine fremden Arbeitsstände hinterlassen

**R13 · Gewicht 5 · Aufwand M · Dauerhafte Änderungen und fehlender Snapshot-Umfang bestätigt**

Die verbliebenen Änderungen betreffen `til.json` und `li.json` in [PC_log](../../examples/PC_log) und [G0B1_log](../../examples/G0B1_log). Der semantische Vergleich mit HEAD ergibt:

| Beispiel | TIL-Einträge vorher → nachher | Änderung vorhandener TIL-Einträge | Änderung vorhandener LI-Einträge |
| --- | --- | --- | --- |
| PC_log | 2.293 → 2.306 | Keine | 2.283 geänderte Zeilennummern |
| G0B1_log | 2.296 → 2.309 | Keine | 2.283 geänderte Zeilennummern |

Pro Beispiel kommen 13 SL-/CE-Beispiele aus `triceCheck.c` in beiden Tabellen hinzu. Es werden keine vorhandenen TIL-Einträge entfernt oder neu zugewiesen. Bei den bestehenden LI-Einträgen ändern sich ausschließlich Zeilennummern, um 8, 13 oder 31 Zeilen; die Dateipfade bleiben gleich. Die großen Textdiffs sind somit eine inhaltliche Aktualisierung nach Source-Erweiterungen, keine bloße JSON-Umsortierung und kein Nachweis instabiler IDs.

**Bei unveränderten Eingaben ist keine erneute inhaltliche Änderung erforderlich.** Sind Quellen, Einstellungen und der relevante ID-/Artefaktzustand gleich geblieben und die Tabellen bereits aktuell, soll ein weiterer Bind-Lauf sie unverändert lassen. [Bind](../../internal/id/bindIDs.go) vergleicht vor einem vorgesehenen Schreibzugriff die erzeugten Bytes mit dem vorhandenen Inhalt und überspringt identische Dateien. Der vorhandene Test `TestBindDoesNotReplaceUnchangedFiles` in [bindIDs_test.go](../../internal/id/bindIDs_test.go) prüft, dass ein zweiter erfolgreicher Lauf keine Dateiersetzung ausführt. Diese Eigenschaft ist bereits implementiert; sie muss für R13 nicht neu erfunden werden.

Eine weiterhin sichtbare Git-Änderung bedeutet nur, dass die Datei noch vom Commit-Stand abweicht. Sie beweist keine weitere Änderung beim nächsten Testlauf. Für die vier Beispieltabellen ist bislang kein fortlaufender Inhaltswechsel bei identischen Wiederholungen nachgewiesen. Der Befund passt zu einmaligem Nachholbedarf nach den Source-Erweiterungen.

Falls ein Test nach seiner Ausführung die alten, noch nicht aktualisierten Tabellen wiederherstellt, kann der nächste Lauf dieselbe Aktualisierung vorübergehend erneut benötigen. Das ist durch den wiederhergestellten Ausgangszustand bedingt. Nach jedem Lauf müssen dann wieder dessen Anfangsbytes vorliegen. Tests, die Quellen oder ID-Zustände absichtlich vorübergehend verändern, können ebenfalls zeitweise andere Tabellen benötigen; daraus folgt keine allgemeine Pflicht, aktuelle Beispieltabellen bei jedem Build zu ändern.

Der Schreibpfad ist konkret: Im erfolgreichen Schritt 600 ruft [run_standalone_bind_examples](../../scripts/_200_gcc_example_build_worker.sh) unter anderem [PC_log/build_and_run.sh](../../examples/PC_log/build_and_run.sh) und [G0B1_log/build.sh](../../examples/G0B1_log/build.sh) direkt im Checkout auf. Beide führen `trice bind` im Beispielverzeichnis aus und aktualisieren dort die Standarddateien `til.json`/`li.json`. Die Änderungszeiten der LI-Dateien liegen am 29. September bei 23:18:45/46, innerhalb dieses Schritts. Der äußere [Snapshot](../../scripts/_140_trice_test_state.sh) umfasst die gemeinsamen Source-Pfade, die zentral konfigurierten TIL/LI und das zentrale generierte Verzeichnis, aber nicht diese vier lokalen Tabellen und die lokalen generierten Verzeichnisse. Deshalb kann er „exact initial files restored“ melden, obwohl diese Dateien verändert bleiben.

Aufgabe in zwei getrennten Schritten:

- Die versionierten Beispieltabellen nach Prüfung einmal bewusst auf den aktuellen Source-Stand bringen. Damit entfällt dieser konkrete Nachholbedarf bei unveränderten Wiederholungen. Dies ist ein eigener nachvollziehbarer Arbeitsschritt, keine stillschweigende Test-Nebenwirkung und kein automatischer Commit-Auftrag.
- Unabhängig davon den Test-Ausgangszustand schützen: Beispielprüfungen bevorzugt mit ihren erforderlichen Abhängigkeiten in temporären Kopien ausführen. Alternativ den Snapshot-Vertrag um sämtliche tatsächlich beschriebenen lokalen Tabellen und generierten Verzeichnisse erweitern. Dabei auch ursprünglich fehlende Dateien und bereits vorhandene Benutzerauswahl erhalten. Nicht das Scannen von `triceCheck.c` oder Testfälle entfernen, nur um Diffs zu vermeiden.

Ein vollständiger Snapshot vom Beginn des Gesamtlaufs liegt für diese vier Dateien nicht vor; der Vergleich mit HEAD ist kein Beweis für ihren exakten Zustand vor Testbeginn. Schon beim Einstieg in die vorherige Analyse waren sie verändert. Deshalb bleiben die aktuellen Dateien unverändert, statt sie pauschal zurückzusetzen.

Zusätzlich ist am Ende von [testAll](../../scripts/_110_test_runner.sh) die Worktree-Schutzprüfung auskommentiert; Schritt 490 führt die kanonische Bind-Vorbereitung außerhalb eines solchen Snapshots aus. Diese Pfade im selben Zustandsvertrag prüfen. Eine reine `git status --short`-Gleichheit erkennt keine Inhaltsänderung in einer bereits vorher geänderten Datei; die relevante Garantie muss auf den tatsächlichen Anfangsbytes beruhen.

**Abnahme:** Den verursachenden Standalone-Buildpfad aus Schritt 600 gezielt prüfen. Zwei aufeinanderfolgende Builds mit identischen Eingaben und bereits aktuellen Beispieltabellen ergeben identische JSON-Bytes; der zweite Bind-Lauf ersetzt diese Dateien nicht. Getrennt davon die Wiederherstellung mit anfangs veralteten Tabellen sowie vorübergehend veränderten Testeingaben prüfen. Erfolgs-, Fehler- und Signalpfade erhalten einen vorher bereits schmutzigen Arbeitsstand einschließlich Quellen, zentraler und lokaler TIL/LI, generierter Artefakte und Benutzerauswahl bytegenau. Bereits fehlende Dateien bleiben anschließend fehlend. Eine Verletzung meldet die konkret betroffenen Pfade und einen Fehlerstatus. Keine pauschale Git-Rücksetzung. Erst danach weitere zustandsverändernde Workflows parallelisieren und die Beispielabnahme nach R07 erweitern.

### Go-Modulfolgen eines Major-Releases erledigen

**R14 · Gewicht 5 nur bei beschlossener v2 · Aufwand M–L**

`go.mod` lautet derzeit `module github.com/rokath/trice`. Ein reguläres Go-Modul ab Major 2 benötigt einen passenden `/v2`-Modulpfad. Das betrifft interne Imports, veröffentlichte Pakete, `go install`-Anleitungen, mögliche Modulpfad-Annahmen in Tests und Build-/Releaseprüfungen. Ein bloßes `v2.0.0`-Tag erledigt das nicht. Siehe [Go: Developing a major version update](https://go.dev/doc/modules/major-version).

Empfehlung bei v2: den regulären Go-Modulweg bewusst wählen. Vorher klären, ob und wie Nutzer die öffentlichen Go-Pakete importieren. Die fertigen Host-Binaries dürfen weiterhin `trice` und `tlog` heißen; daraus folgt kein neuer Name für die C-Bibliothek.

**Abnahme:** Modulauflösung und Installation beider Tools mit dem beschlossenen Major-Pfad funktionieren in einer sauberen Umgebung; Importe, Tests, Dokumentation und Release-Artefakte passen zusammen. Keine pauschale Umstellung auf eigene Versionssysteme für einzelne Komponenten.

### Release Notes und ausgelieferte Dateien prüfen

**R15 · Gewicht 5 · Aufwand M**

Der aktuelle [Changelog](../../CHANGELOG.md) endet bei v1.3.0. Einen verständlichen neuen Release-Abschnitt erstellen: Bind, SL, CE, Tags/Gewichte, Visualisierung, Generatoren, Ablage, Plattform-/Compilergrenzen und konkret nötige Anpassungen bestehender Projekte. Ein vollständiger Git-Log ersetzt diese Anwendersicht nicht.

Die [Installationsprüfungen](../../.github/workflows/install-checks.yml) und [Release-Prüfungen](../../.github/workflows/release-audit.yml) testen bereits Archive, Pakete, PDF und klassisches Decoding. Eine kleine neue Feature-Abnahme soll mit den **ausgepackten** Tools und Target-Quellen erfolgen: Bind bzw. Insert mit SL/CE, tatsächlich erzeugter Record, Text/JSON/KV und ausschließlich Anwendungsrecords auf dem maschinenlesbaren Kanal. Passende plattformunabhängige Fixture verwenden.

Die Toolchain-Angaben angleichen: `go.mod` verlangt Go 1.25.0; der separat manuell gestartete Workflow `go.yml` nennt noch 1.24.0. Das muss keinen unmittelbaren Buildfehler verursachen, weil Go einen Toolchain-Wechsel auslösen kann, macht die geprüfte Umgebung aber unnötig unklar.

**Abnahme:** Verständliche englische Release Notes und Anpassungshinweise; finale englische UM-Fassung als Markdown/PDF; passende Target-Quellen; nachvollziehbare Artefaktprüfungen auf den zugesagten Betriebssystemen. Kein Publish oder Tag ohne ausdrücklichen Auftrag.

### Release-Abnahme auf einem feststehenden Stand

**R16 · Gewicht 5 · Aufwand M plus Full-Testlauf**

Nach den ausgewählten Änderungen zuerst ihre gezielten Prüfungen, anschließend einmal den vollständigen Lauf auf einem feststehenden Stand ausführen. Die Auswertung des abgeschlossenen Laufs vom 29./30. September steht bei R01, R06 und R13. Dessen Fehlerprotokolle und Zeitbasis vor dem nächsten Gesamtlauf sichern, weil der Runner alte Logs entfernt. Keine konkurrierenden zustandsverändernden Läufe starten.

**Abnahme:**

- Full-Matrix mit Bind und Insert/Clean ohne ungeklärte Fehler; alle Pflichtwerkzeuge vorhanden. PASS einer Suite darf relevante intern übersprungene Tests nicht verdecken.
- CE-/SL-End-to-End-Abnahme gemäß R07 und installierte Release-Artefakte gemäß R15.
- Keine zurückgebliebenen unbeabsichtigten Source-/Metadatenänderungen.
- Go-Coverage mit der Paket- und `-coverpkg`-Auswahl des [Coverage-Workflows](../../.github/workflows/coverage.yml) gegen die Zielbranch-Baseline vergleichen; Coveralls-Zeilenabdeckung und Go-Statement-Coverage unterscheiden. C-Abnahme separat.
- Finales UM: mdtoc, Markdown, relative Links mit korrekter Groß-/Kleinschreibung, PDF; danach keine ungeprüften inhaltlichen Änderungen mehr.
- Ein kurzer echter G0B1-Boardlauf für zwei Task-Kontexte, Stempel und lesbare/strukturierte Ausgabe, sofern diese Hardware-Zusage im Release gemacht wird. Fehlender Hardwarezugang wird als fehlender Nachweis benannt.
- Versionsentscheidung, bekannte Grenzen und eventuelle bewusst akzeptierte Abweichungen sind dokumentiert.

## Weitere Beschleunigung ohne geringere Abdeckung

Die aussichtsreichste Reihenfolge lautet: **R06 messen → R08/R09 Abschlusskosten beseitigen → erst dann P01–P03 bewerten**. Vollständige Bulk- und Einzelzeilenabdeckung sowie beide ID-Workflows bleiben erhalten. `quick` anstelle von `full`, weniger Konfigurationen oder verkürzte Testdaten wären keine gleichwertige Beschleunigung.

| ID | Vorschlag | Gewicht | Aufwand | Abhängigkeiten |
| --- | --- | ---: | --- | --- |
| P01 | PC-Konfigurationen begrenzt parallel in getrennten Prozessen testen | 3 | M | R01, R06, R09, R13 |
| P02 | Go-/CGO-Buildcache gezielt und nachweisbar invalidieren | 3 | M–L | R06, R13 |
| P03 | L432-Konfigurationen mit getrennten Buildverzeichnissen planen | 3 | L | R06, R13 |

### PC-Matrix parallel ausführen

[Paketweises serielles `go test`](../../scripts/_160_pc_target_test_worker.sh) verhindert derzeit Parallelität zwischen Konfigurationen. Unterschiedliche Testprozesse können voneinander unabhängig sein, sobald ihr ID-Zustand feststeht. Globale Go-/C-Zustände innerhalb eines einzelnen Testprozesses dürfen dagegen nicht durch beliebiges `t.Parallel` konkurrieren.

Zuerst die vollständige Bulk-Phase erhalten, danach deren Fehlerdiagnosen und die Einzelzeilenphase wie bisher. Eine begrenzte Zahl paralleler Pakete, getrennte Logs und deterministische Zusammenführung prüfen. Bei `--no-stop` bleibt jeder Fehler sichtbar; Fail-fast und Signalweitergabe brauchen einen definierten Ablauf. Keine gleichzeitigen Insert-/Bind-Umschreibungen derselben Quellen.

**Nachweis:** Gleiche Konfigurationen, Modi, Assertions und Fehlercodes wie seriell; keine Ressourcenspitzen durch unbegrenzte Parallelität. Auch absichtlich eingebaute Fehler und Abbruchfälle vergleichen.

### Cache nutzen, ohne veralteten C-Code zu testen

Der PC-Worker löscht vor jedem Workflow `go clean -cache -testcache`. Das ist wegen außerhalb der Go-Paketverzeichnisse eingebundener C-Quellen begründet. Die [Go-Dokumentation zum Buildcache](https://pkg.go.dev/cmd/go#hdr-Build_and_test_caching) weist auf Grenzen bei C-Abhängigkeiten hin; schlichtes Entfernen der Bereinigung wäre nicht ausreichend.

Prüfen, ob vollständig erfasste Inhalts-/Konfigurationssignaturen oder vorbereitete lokale Build-Eingaben die korrekte Invalidierung gezielter machen. Die bloße Trennung in zwei Cacheordner garantiert keine Aktualität. `-count=1` kann weiterhin die tatsächliche Testausführung erzwingen und unabhängig davon den Buildcache nutzen.

**Nachweis:** Änderungen an `triceCheck.c`, gemeinsam eingebundenen Headern, Sidecars, Workflow, Compileroptionen und Konfiguration erzwingen den passenden Neubau; kalte und warme Läufe liefern dieselben Ergebnisse. Keine Wiederverwendung bereits erfolgreich gemeldeter Testergebnisse als Ersatz für geforderte Ausführung.

### L432-Builds isolieren und begrenzen

[all_configs_build.sh](../../examples/L432_inst/all_configs_build.sh) durchläuft 0 bis 100 mit `make clean` und vollständigem `build.sh` je Konfiguration. Das gemeinsame `out.gcc` verhindert sichere Parallelität. Die Umgebung setzt auf Linux/macOS `MAKE_JOBS=-j`; zusätzlich parallele Konfigurationen könnten daher ungebremst sehr viele Compiler starten.

Getrennte Objekt-/Ausgabeverzeichnisse je vollständiger Konfiguration und ein gemeinsames begrenztes Jobbudget prüfen. Gemeinsame Vorbereitung und Umgebungsprüfung nur dann wiederverwenden, wenn deren Voraussetzungen unverändert sind. Das Weglassen von `clean` allein ist keine korrekte Optimierung: geänderte Defines müssen alle betroffenen Objekte erneuern.

**Nachweis:** Alle 101 Konfigurationen mit gleichen Flags und erfolgreichem Link; korrekte Abhängigkeiten bei Änderungen und Fehlern; Vergleich kalter und warmer Läufe. Keine neue Cache-Abhängigkeit ohne gesonderte Entscheidung.

## Sinnvolle Erweiterungen zur späteren Diskussion

Diese Punkte sind Vorschläge, keine offenen Versprechen für das nächste Release. Bestehende Tests, PoCs und frühere Entscheidungen bleiben maßgeblich. Vor einer Issue-Erstellung Nutzen, Scope und vorhandene Issues abgleichen; externe Issue-Texte später englisch formulieren.

| ID | Idee | Gewicht | Aufwand | Empfehlung |
| --- | --- | ---: | --- | --- |
| F01 | Verständliche Bestandsprüfung generierter Artefakte | 3 | M | Zuerst rein lesende Diagnose diskutieren |
| F02 | Fuzz- und Race-Prüfungen für neue Parser und Recordpfade | 3 | M | Robustheit vor weiteren Ausgabeformaten |
| F03 | Frühe Hostfilterung / Template-Caching | 2 | M–L | Alte A11/M17-Aufgabe; zuerst messen |
| F04 | Benannte SL-Felder für Visualisierung oder Schemaübersicht | 2 | M | Mit einem konkreten Anwenderfall beginnen |
| F05 | CE für eindeutig zuordenbare Ein-Logstellen-Wrapper | 2 | M | Kleiner separater Ausbau auf Basis des PoC |
| F06 | Allgemeines CE für Bind-Wrapper und Counter-Rebase | 1 | L | Weiter zurückstellen |

### Generierte Dateien verständlich zuordnen

**F01:** Ein lesender Bericht könnte erklären, welche Source welchen Sidecar besitzt, welche Dateien für den gewählten Build gebraucht werden und welche nicht mehr im gewählten Scan vorkommen. Das beantwortet die Frage nach unerwarteten `triceCheck`-Artefakten besser als pauschales Löschen.

[Bind](../../internal/id/bindIDs.go) entfernt bereits überzählige Rebase-Helfer eines aktuell bekannten Owners. Eine globale Bereinigung aller verwaisten Owner-Dateien ist ein anderer Vertrag. Nicht gescannte Sources können weiterhin legitime Besitzer sein; historische Sidecars werden zudem als ID-Evidenz genutzt. Benutzereigene ABC-Auswahlheader im selben Verzeichnis müssen geschützt sein.

Ein optionales späteres Löschen braucht daher eindeutig nachgewiesenen Besitz und vollständigen Projektumfang. **Kein automatisches Pruning als unbemerkte Nebenwirkung eines partiellen Bind-Laufs.** Abnahme zunächst: nützlicher Bericht ohne Dateiänderungen; Löschverhalten erst nach eigener Entscheidung.

### Parser und Recordpfade systematisch auf unerwartete Eingaben prüfen

**F02:** Im untersuchten Go-Code sind viele tabellarische Verhaltenstests, aber keine `Fuzz...`-Einstiegspunkte vorhanden. Ergänzende Fuzztests für Template-/CE-Parsing und Decoder-Eingaben könnten Escapes, Klammern, verkürzte Records und ungewöhnliche Kombinationen abdecken. Ein gezielter Race-Lauf ist besonders bei Abschluss-/Watcher-Arbeiten sinnvoll.

Nicht jede Testfunktion parallelisieren. Relevante Invarianten sind unter anderem: kein Panic bei abgewiesener Eingabe, keine Teilveröffentlichung, stabiler kanonischer Template-Roundtrip, kein Diagnosetext im JSON-Kanal und keine Datenrennen bei echten nebenläufigen Pfaden. Gefundene Fälle als lesbare Regressionstests erhalten. Ein begrenzter Fuzz-Lauf verspricht keine vollständige Sicherheit.

### Frühe Hostfilterung messen

**F03 übernimmt die offene A11, zuvor M17.** Replay mit 0, 50, 90 und 100 Prozent verworfenen Ereignissen vergleichen; CPU, Allokationen und Durchsatz für Text, JSON und KV erfassen. Im [TREX-Decoder](../../internal/trexDecoder/trexDecoder.go) werden Templates momentan pro Ereignis geparst und validiert; ein Cache ist ein weiterer Kandidat, aber noch keine beschlossene Umsetzung.

Framing/Integrität, Rohaufzeichnung, Statistik, Diagnosen, Stempelreferenzen und akzeptierte Records müssen gleich bleiben. Typisierte SL-/CE-Records und aktualisierte Wörterbücher gehören in den Vergleich. R09 betrifft wiederholte endliche Testläufe; diese Aufgabe betrifft Durchsatz im dauerhaften Betrieb und darf damit nicht verwechselt werden.

### Benannte Felder besser weiterverwenden

**F04:** Die heutige Visualisierung adressiert numerische Werte über `v0...v11`. Ein Zugriff über bereits vorhandene Feldnamen könnte Regeln lesbarer und gegen geänderte Parameterreihenfolgen robuster machen. Alternativ könnte eine reine Schemaübersicht Namen, Typen und zugehörige IDs über das heutige Häufigkeitsregister hinaus zeigen.

Zuerst einen dieser Anwenderfälle auswählen. Kein neuer Parallelvertrag für dasselbe Schema, keine automatische C-Typinferenz und keine zusätzliche Target-Nutzlast. Gleiche Feldnamen mit unterschiedlichen Typen, fehlende Werte, User- gegenüber Hostfeldern und historische TIL-Einträge benötigen klare Regeln. Keine neue CLI festlegen, bevor der konkrete Nutzen entschieden ist.

### Kleine Wrapper-Erweiterung getrennt bewerten

**F05:** Der [erweiterte CE-PoC](../../internal/id/context_enrichment_rebase_poc_test.go) zeigt einen eindeutig über eine Zeile zuordenbaren Wrapper mit einer Logstelle ohne Compiler-Vorlauf und ohne `__COUNTER__`. Das könnte die bestehende einfache CLI erhalten und ist unabhängig vom allgemeinen Rebase-Ausbau bewertbar.

Vor produktiver Freigabe sind Alias-/Selektorverhalten, lokale Sichtbarkeit, Bitbreiten/Stempel, einmalige Auswertung, wiederholte Expansionen, `generate -logC` und klare Ablehnung nicht unterstützter Formen nachzuweisen. Bis dahin bleibt die heutige dokumentierte Bind-Grenze bestehen; Insert/Clean ist die dauerhafte Alternative.

### Allgemeines CE-Rebase bleibt eine Architekturentscheidung

**F06 übernimmt die bisher zurückgestellte CE-Folgeaufgabe.** Der PoC wählt pro Expansion einen CE-Adapter im Präprozessor. Für komplexe Fälle liefert ein zusätzlicher Vorlauf mit dem konkreten Compiler absolute Counter-Werte. Das löst in den geprüften Fällen die falschen Scope-Anforderungen, kostet aber einen Buildschritt pro Übersetzungseinheit und Konfiguration.

Erhaltene Nachweise: getrennte lokale Scopes, wiederholte Wrapper-Aufrufe, einmalige Auswertung, echte Host-Records und Ablehnung veralteter Zuordnungen. Die dokumentierte Matrix umfasst Host-Clang sowie ARM-GCC für Cortex-M0/M4 in mehreren C-/C++-Modi; ARM-Nachweise sind Compile-Nachweise. Strenges C++20 ist wegen der vorhandenen Enum-Rebase-Warnung nicht bestanden. Weitere Compiler, PCH/Module, zusätzliche Counter-Verwendung, Build-Abhängigkeiten und getrennte Artefakte bleiben Integrationsfragen.

**Empfehlung:** Für das nächste Release nicht aufnehmen. Ein einfaches, merkbares Userinterface hat Vorrang. Der bereits verfügbare Insert/Clean-Weg und ausdrücklich geschriebene Werte decken die praktische Alternative ab. Kein Compiler-Vorlauf und keine neue Kontextverwaltung werden implizit eingeführt.

## Vorschlag für die nächsten Aufträge

R01 ist korrigiert; als Nächstes die nun bestätigte Snapshot-Lücke R13 in einem getrennten Schritt schließen und danach R02–R05 klären beziehungsweise korrigieren. R06 hat jetzt eine konkrete Zeitbasis und soll diese Erfassung verstetigen. R08/R09 sind vor einem weiteren achtstündigen Gesamtlauf der erste Optimierungsansatz. Die fehlende produktive Testabnahme R07 anschließend mit gesichertem Ausgangszustand verbindlich machen.

Anschließend R10–R12 für ein sauberes englisches UM und einen zutreffenden Einstieg, gegebenenfalls R14 für v2, dann R15/R16 für Release Notes und Abnahme. P- und F-Aufgaben werden nur nach eigener Auswahl umgesetzt. Die deutsche Planung und historischen deutschen Texte bleiben außerhalb des englischen Anwender-UM.
