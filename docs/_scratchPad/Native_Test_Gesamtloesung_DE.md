# Gesamtlösung für zuverlässige native Trice Tests

## Ziel und verbindlicher Prüfnachweis

Trice soll unter Windows, Linux und macOS mit den tatsächlichen Datei-, Prozess-, Netzwerk- und Compilermechanismen des jeweiligen Systems geprüft werden. Ein erfolgreicher Lauf bedeutet, dass der festgelegte Prüfumfang vollständig ausgeführt wurde. Die lokale Installation eines zusätzlichen oder neueren Werkzeugs darf diesen Umfang nicht stillschweigend verändern.

Die Lösung baut auf dem bestehenden Runner, den Transaktionen zur Quellenwiederherstellung, den Go-Tests, den PC/CGO-Matrizen und den vorhandenen C/C++-Integrationstests auf. Die Steuerung wird vereinheitlicht; bewährte Tests werden weiterverwendet. Ein neuer allgemeiner Testframework-Unterbau ist nicht erforderlich.

Ein lokaler Lauf bestätigt immer nur das aktuelle Betriebssystem und die aktuelle Architektur. Der Nachweis für alle drei Systeme entsteht in der nativen CI am selben Commit. Ein grüner Linux-Lauf ist kein Windows- oder macOS-Nachweis.

## Einfache Bedienung

| Befehl | Bedeutung |
|---|---|
| `./scripts/testAll.sh` | Vollständiger nativer Lauf einschließlich der bestehenden PC-/Firmware-Konfigurationsmatrizen und umfangreicher Insert/Clean-Prüfungen. |
| `./scripts/testAll.sh quick` | Kürzerer nativer Lauf mit repräsentativen C-Konfigurationen und gezielten Plattformfällen. |
| `./scripts/testAll.sh full` | Ausdrücklicher Name für den vollständigen Standardlauf. |
| Bestehende einzelne Testskripte | Einen Fehler mit denselben Werkzeugen und Regeln gezielt reproduzieren. |

`--stop` und `--no-stop` bleiben erhalten. Unabhängige Tests laufen nach einem Testfehler standardmäßig weiter. Eine fehlende gemeinsame Voraussetzung beendet die Vorbereitung, bevor abhängige Tests beginnen. Es gibt keine Installationsfragen mitten in einem langen Testlauf.

Werkzeuge werden vor dem Start der eigentlichen Tests aufgelöst, geprüft und bei Bedarf im Projektcache bereitgestellt. Netzwerkzugriff wird nur benötigt, wenn etwas im Cache fehlt oder eine ausdrücklich ausgewählte Netzwerkprüfung läuft. Ein Downloadfehler ist ein verständlicher Einrichtungsfehler und kein bestandener oder übersprungener Test.

Systemvoraussetzungen wie Git Bash, ein macOS-SDK oder nötige Systembibliotheken werden früh und zusammenhängend gemeldet. Der Runner installiert keine SDKs oder Systempakete mit Administratorrechten. Er nennt für fehlende Voraussetzungen die passenden Installationsschritte.

### Prüfumfänge sichtbar halten

Der normale Lauf prüft Quellcode, lokale Dokumentkonsistenz und Buildkonfiguration. Er führt nach erfolgreicher Vorbereitung keine externen HTTP-Prüfungen und keine vollständige Release-Erzeugung aus.

Die bisherigen Schritte 530 und 610 werden nicht aufgegeben: externe Links und Release-Artefakte erhalten ausdrücklich benannte, gesonderte CI-Jobs und bleiben einzeln ausführbar. Der externe Linklauf behält seine bisherigen Prüfungen bei. Eine spätere Aufteilung lokaler und externer Links muss deren Gesamtabdeckung nachweisbar erhalten. Ein voll bestandener Codetest verspricht weder erreichbare Fremdserver noch fertig geprüfte Releasepakete.

Diese Änderung des bisherigen `testAll.sh`-Umfangs ist Teil des vorgeschlagenen Bedienvertrags. Sie muss in der Implementierung ausdrücklich dokumentiert werden; ein stilles Weglassen bisheriger Prüfungen ist ausgeschlossen.

## Ergebnisse ohne falsches Grün

| Ergebnis | Aussage | Exitstatus |
|---|---|---|
| PASS | Alle für diesen Lauf verpflichtenden Prüfungen ausgeführt und bestanden. | `0` |
| FAIL | Mindestens eine Prüfung hat einen Fehler gefunden. | `1` |
| INCOMPLETE | Vorbereitung fehlgeschlagen oder verpflichtende Prüfung unerwartet übersprungen. | `2` |
| ABORTED | Lauf abgebrochen; Wiederherstellung abgeschlossen oder verbleibende Einschränkung benannt. | `130` bzw. `143` bei den bisherigen Signalpfaden |

Ein weiterer Testfehler ändert INCOMPLETE nicht in ein vollständiges Ergebnis. Fehler bei der Wiederherstellung verhindern ebenfalls PASS. Die Gesamtzusammenfassung nennt alle betroffenen Gruppen, unabhängig vom finalen Exitstatus.

Nicht anwendbare Einzeltests werden nur nach einer überprüfbaren Bedingung zugelassen: beispielsweise ein POSIX-spezifischer Signaltest auf Windows. Eine kleine, explizite Ausnahmeliste benennt Test, Plattform, Grund und entsprechende alternative Abdeckung. Fehlender Compiler, deaktivierte Integration oder unerreichter ausgewählter Test sind keine zulässigen Ausnahmen.

Go-Ergebnisse werden über `go test -json` ausgewertet. Ein ausgewählter verpflichtender Test muss tatsächlich PASS melden; ein leerer `-run`-Treffer zählt nicht. Unbekannte SKIPs werden sichtbar und verhindern vollständige Abnahme. Erwartete Untertests und dynamische Compilerfälle werden gezielt geprüft; bloßes Zählen der obersten Pakete genügt nicht.

Die Ergebnisdatei enthält Prüfumfang, OS, Architektur, Commit, lokalen Änderungsstatus, eingesetzte Werkzeuge, Zeitbedarf, Testgruppen und Fehler. Ein knapper Textbericht erklärt dieselben Ergebnisse für Menschen. Die Konsole zeigt zuerst Fehlerursache und Reproduktionsbefehl, anschließend den Pfad zum vollständigen Log.

## Native Plattformmatrix

Die verpflichtende erste Matrix besteht aus Windows x64, Linux x64 und macOS ARM64. Vor einem Release wird zusätzlich macOS Intel nativ geprüft, solange Intel-Pakete ausgeliefert werden. Weitere ausgelieferte Architekturen erhalten einen eigenen ausgewiesenen Status; ein Cross-Build zählt nicht als dortige Ausführung.

| Prüfschicht | Windows | Linux | macOS |
|---|---|---|---|
| Aktuelle Trice- und tlog-Binaries bauen und ausführen | Natives EXE | Natives ELF | Natives Mach-O |
| Go-Pakettests | Pflicht | Pflicht | Pflicht |
| Bind und C/C++-Integration | Pflicht | Pflicht | Pflicht |
| Insert/Clean und Quellenwiederherstellung | Pflicht | Pflicht | Pflicht |
| CE und Structured Logging bis zum realen Decoder | Pflicht | Pflicht | Pflicht |
| PC/CGO mit tatsächlichen C-Datensätzen | Pflicht | Pflicht | Pflicht |
| Pfade und LF/CRLF | Pflicht | Pflicht | Pflicht |
| Lokales TCP/UDP und TIL-Nachladen | Pflicht | Pflicht | Pflicht |
| Shell-Einstieg | Git Bash plus native CLI über PowerShell | Bash | Bash |
| Native Compilerfamilien | MinGW-w64-GCC und passender Clang | GCC und Clang | Apple Clang und echter GCC |

Ein Compiler muss C und C++ einschließlich Standardheadern und Runtime übersetzen, linken und das Ergebnis ausführen können. Auf macOS wird `gcc` als Alias für Apple Clang nicht als zweite Compilerfamilie gezählt. Auf Windows wird kein MinGW-/MSVC-Gemisch als geprüfte Installation akzeptiert.

Firmware-Cross-Builds werden ebenfalls auf allen drei Hosts geprüft, weil Shell-, Make- und Pfadfehler auch dort auftreten. Sie belegen Übersetzung und Linken für den MCU, keine Hardwareausführung. Flashen, UART-Hardware und RTT benötigen weiterhin eigene Hardwareprüfungen.

Die reguläre Matrix nimmt für jede Compilerfamilie eine feste repräsentative Auswahl. Der vollständige Lauf deckt die bestehende Konfigurationsbreite ab. Eine Auswahl nach zufällig installierten Compilern entfällt in den verpflichtenden Läufen.

## Gezielt auf Plattformfehler prüfen

Die Testprojekte liegen auf dem echten Dateisystem des Runners und werden mit der aktuell gebauten CLI verarbeitet. Stringtests mit künstlichem `GOOS` bleiben als schnelle Ergänzung erhalten.

| Fall | Verlangtes Ergebnis |
|---|---|
| LF- und CRLF-Quelldateien mit denselben Trice-Aufrufen | Gleichwertige ID-/LI-Bedeutung und dekodierte Datensätze; korrekte Zeilennummern. |
| Bind auf vorhandener Quelle | Alle Quellbytes bleiben unverändert. |
| Insert und anschließendes Clean | Ursprüngliche Quellbytes einschließlich Zeilenenden und UTF-8 werden wiederhergestellt. |
| Windows-Pfade mit `/` und `\` | Unterstützte Eingabeschreibweisen finden dieselbe Datei und erzeugen die vereinbarte portable LI-Darstellung. |
| Relative Pfade aus verschiedenen Arbeitsverzeichnissen | Richtiges Projekt und richtige Aufrufposition; keine unbeabsichtigten Dateien im Aufrufverzeichnis. |
| Projekt-, Tool- und Ausgabepfade mit Leerzeichen und Umlauten | Argumente erreichen Trice, Compiler und Linker unverändert. |
| Windows-Laufwerke und UNC | Pfadberechnung anhand bestehender CLI-Regeln; tatsächlicher UNC-Zugriff nur mit einem eigens bereitgestellten Share. |
| Groß-/Kleinschreibung | Korrektes Verhalten auf dem erkannten Dateisystem; keine Annahme, alle macOS-Volumes seien gleich. |
| Geöffnete oder gesperrte Datei | Verständlicher Fehler bzw. korrektes unterstütztes Ersetzen; keine beschädigte TIL und keine unkontrollierte Teiländerung. |
| TIL-Ersetzung bei laufendem Logger | Konsistentes Nachladen; fehlerhafte Aktualisierung wird diagnostiziert. |
| Vorzeitiger Prozessabbruch | Eigene Ressourcen freigegeben und Testquellen restauriert. |
| Git Bash und PowerShell | Die dokumentierte CLI-Argumentübergabe einschließlich CE-Ausdrücken und Pfaden funktioniert jeweils nativ. |

Die erzeugten LF-/CRLF-Fixtures werden bytegenau geschrieben. Git-Normalisierung darf sie nicht vor dem Test vereinheitlichen. Shellskripte bleiben ausführbare LF-Dateien; das CRLF-Testziel sind unter anderem die vom Werkzeug gelesenen und bearbeiteten Projektquellen.

Windows-TCP-Tests werden in der nativen CI ausdrücklich aktiviert. Die vorhandene lokale Opt-in-Regel wegen Firewalldialogen kann für einzelne gewöhnliche Go-Aufrufe bestehen bleiben; der verpflichtende native Testlauf muss deren Abdeckung trotzdem eindeutig einfordern.

## Werkzeuge einmal definieren und gezielt auswählen

Eine zentrale Versionsdatei nennt die unterstützten Werkzeuge mit exakter Version und den benötigten Paketen je OS/Architektur. Bestehende Pins für shfmt, clang-format, mdtoc, stitchmd, Cppcheck und die PDF-Werkzeuge werden übernommen. Neu aufzulösende Pins werden anhand tatsächlich verfügbarer Releases und Probeausführungen festgelegt.

Die Liste ist einfache Datenkonfiguration, keine neue frei programmierbare Installationssprache. Der vorhandene Setup-Helfer übernimmt Laden, Versionsprüfung, Cache und Aktivierung. Downloadarchive erhalten SHA-256-Prüfungen; Go-Module werden mit exakter Modulversion und vorhandener Go-Prüfsummenprüfung installiert. Bei npm gehören auch die aufgelösten Abhängigkeiten in eine festgelegte Installation.

| Werkzeuggruppe | Regel |
|---|---|
| Formatter und Linter | Exakte freigegebene Version; globale neuere Version bleibt unberücksichtigt. |
| Go und Node | Freigegebene Version für die Abnahme; keine stillen Wechsel über `stable`, `latest` oder bewegliche Versionsbereiche. |
| Native Compiler | Pro Plattform benannte, zusammenpassende Compiler-/Runtime-Kombination; überprüfte Familien und Versionen. |
| ARM-Toolchain | Ein vollständiges festgelegtes Paket je unterstütztem Host mit Headern, Bibliotheken und Binutils. |
| macOS-SDK und Systembibliotheken | Unterstützte Umgebung prüfen und protokollieren; keine Zusage byteidentischer Runner-Images. |

Eine vorhandene Installation kann wiederverwendet werden, wenn Identität und Probeausführung genau den Vorgaben entsprechen. Andernfalls wird die Projektinstallation gewählt. Die Auswahl wird vor Beginn eingefroren und an alle Kindprozesse weitergegeben. Ein Kindprozess darf nicht später wieder eine andere globale Version wählen.

Werkzeugcaches werden nach OS, Architektur und Versionsdefinition getrennt. Eine vollständige Installation erhält erst nach erfolgreicher Prüfung eine Fertigmarkierung. Gleichzeitige Installationen benötigen Sperre und atomare Veröffentlichung. Ein abgebrochener Download oder Build darf den nächsten Lauf nicht mit einem halbfertigen Werkzeug starten lassen.

Ein Lauf legt seine ausgewählten Pfade und Versionen offen. Standardcache und Toolpfade bleiben projektbezogen; persönliche Go-/Python-/Node-Installationen werden nicht überschrieben. Eine erstmalige Vorbereitung kann länger dauern; spätere Läufe verwenden geprüfte Caches.

Die GoReleaser-Konfiguration muss zusätzlich den aktuellen Hook-Vertrag unterstützen. Eine festgelegte Version ersetzt die Behebung der gefundenen Deprecation nicht. Ein separater Kompatibilitätslauf erprobt künftige Werkzeugversionen, bevor die verbindliche Versionsdatei geändert wird.

## Bestehenden Code geschützt und Tests reproduzierbar ausführen

Der Runner prüft eine Liste aktueller Eingaben, einschließlich relevanter uncommitteter und neuer Dateien. Für Fixtures und Beispielprojekte werden isolierte Arbeitskopien verwendet. Ein Test darf nicht unbemerkt nur `HEAD` prüfen, während lokal bereits andere Quellen vorliegen.

Die bestehenden transaktionalen Bind-/Insert-Workflows bleiben für die großen Matrizen erhalten, solange ihre Wiederherstellung nachgewiesen ist. Vorherige lokale Änderungen und generierte Daten werden bytegenau erhalten. Es gibt kein pauschales `git reset`, `clean` oder Verwerfen von Nutzeränderungen.

Neue Pfad-/Zeilenendentests arbeiten vollständig in eigenen temporären Projekten. Ein gemeinsamer Runner darf nicht parallel zweimal dieselben instrumentierten Quellen ändern. Parallele Compiler arbeiten ausschließlich auf den dafür vorgesehenen isolierten Verzeichnissen; die vorhandenen begrenzten Parallelitätsregeln werden beibehalten.

Jeder Lauf bekommt ein eigenes Ergebnisverzeichnis unter `temp/log`. Ein zweiter Lauf darf weder die Logs des ersten löschen noch dessen Wiederherstellungsdaten überschreiben. Erfolgreiche temporäre Buildkopien können entfallen; Fehlernachweise und Recovery-Verzeichnisse bleiben erhalten.

Reproduktionsbefehle nennen Prüfumfang, Testnamen und ausgewählte Compiler. Sie dürfen keine maschinenspezifischen absoluten Pfade als dauerhaftes Repository-Kommando voraussetzen.

## CI mit denselben Prüfplänen

Die YAML-Dateien wählen OS, Architektur, Prüfumfang und Artefakte. Sie enthalten keine zweite, unabhängig gepflegte Liste der eigentlichen Tests. Der bestehende Runner definiert die Testgruppen und ruft die bestehenden Einzelschritte auf.

| Zeitpunkt | Verbindlicher Umfang |
|---|---|
| Pull Request | Native reguläre Läufe auf Windows x64, Linux x64 und macOS ARM64; vorhandene verpflichtende ARM-/Full-Prüfungen bleiben bis zur nachgewiesenen Übernahme bestehen. |
| Nächtlich und manuell | Vollständige native Matrizen für dieselben drei Plattformen; Race Detector in getrennten Prozessen. |
| Releasekandidat | Vollständige native Abnahme am Releasecommit plus Prüfung der tatsächlich erzeugten Pakete auf ihren unterstützten Plattformen. |
| Gesonderter Wartungslauf | Externe Links und neue Werkzeugversionen mit jeweils klar benanntem Ergebnis. |

Die Plattformmatrix arbeitet mit `fail-fast: false`, damit ein Windows-Fehler die Linux-/macOS-Ergebnisse nicht verhindert. Feste OS-Labels vermeiden unbeabsichtigte Hauptversionswechsel. Runner-Image, SDK und Compiler werden trotzdem protokolliert, weil auch feste Labels aktualisiert werden.

Die vorhandene Linux-Coverage bleibt zunächst die vergleichbare Coveralls-Basis. Die Paket- und `-coverpkg`-Auswahl muss dem Coverage-Workflow entsprechen; eine neue Teststeuerung darf den Messumfang nicht verkleinern. Ein normaler Go-Lauf und ein Coverage-Lauf sollen langfristig nicht unabsichtlich dieselben umfangreichen Tests doppelt ausführen. Zunächst wird deren Gleichwertigkeit nachgewiesen, bevor ein Lauf zusammengelegt wird.

Race-Tests erhalten einen getrennten, begrenzten Umfang und Prozesslauf. Sie benötigen insbesondere unter Windows eine nachgewiesen passende CGO-/MinGW-Runtime. Ihre Ergebnisse werden nicht in die bisherige Coverage-Basis eingemischt.

Bestehende Required-Check-Namen bleiben bestehen, bis ihre Ablösung ausdrücklich organisiert ist. Neue native Plattformchecks müssen als Pflichtchecks eingerichtet werden; YAML allein aktiviert keinen Branchschutz. Release-Erzeugung und Veröffentlichung bleiben getrennt. Diese Testarbeit autorisiert keinen Push und keine Veröffentlichung.

## Überschaubare Umsetzung

### Ergebnisvertrag und Runner

Zuerst die bisherigen Abläufe mit Verhaltenstests absichern: WARN/SKIP darf keinen vollständigen PASS erzeugen, leere ausgewählte Go-Testmengen müssen auffallen, Abbruch und Wiederherstellung bleiben korrekt. Danach Ergebnisformat, Runverzeichnis und gemeinsame Vorabprüfung einführen. Die historischen Einstiege `quick`, `full`, `--stop` und `--no-stop` bleiben verständlich.

Abnahme: absichtlich fehlendes Werkzeug, übersprungener Pflichtfall, echter Testfehler und Benutzerabbruch ergeben die festgelegten unterschiedlichen Ergebnisse. Nach jedem Fall ist der ursprüngliche Quellzustand nachgewiesen.

### Werkzeugverwaltung und GoReleaser

Bestehende Pins zentralisieren, fehlende Werkzeugdefinitionen ergänzen, automatische lokale Bereitstellung vor dem Lauf ermöglichen und GoReleaser-Hook korrigieren. Setup- und CI-Installationen verwenden dieselben Definitionen. Die vorhandenen Download- und Kompatibilitätstests werden erweitert.

Abnahme: ein absichtlich falsches globales Werkzeug verändert weder Auswahl noch Ergebnis. Kalter Cache, warmer Cache und unterbrochene Installation sind geprüft. Ohne nötigen Downloadzugriff bleibt die Ursache klar erkennbar.

### Native Pflichtläufe und Plattformfälle

Vorhandene echte Integrationen auf den drei OS aktivieren und ihre benötigten Compilerfamilien einfordern. Danach die fehlenden Datei-/Pfad-/Shell-Fälle ergänzen. Gefundene Produktfehler werden als kleine fachlich getrennte Änderungen behandelt; ein Test wird nicht zugunsten eines grünen Ergebnisses abgeschwächt.

Abnahme: die native CI zeigt auf jedem OS ausgeführte Pflichtgruppen und liefert die Logs bei Fehlern. Ein absichtlich eingeführter CRLF-, Pfad- oder Decoderfehler lässt den passenden Test scheitern. Zulässige OS-Ausnahmen werden namentlich sichtbar.

### Vollständige Abnahme und kurze Dokumentation

Die bestehenden vollständigen Compiler-/PC-Matrizen und nativen Paketprüfungen anschließen. Externe Links und Release-Erzeugung als gesonderte sichtbare Jobs erhalten. Aktive Testdokumentation auf Bedienung, Prüfumfang und Fehlersuche begrenzen; Detailregeln direkt neben der zentralen Konfiguration dokumentieren.

Abnahme: alle drei nativen Systeme am selben Commit vollständig geprüft. Bis diese Läufe tatsächlich vorliegen, darf die Lösung nur als lokal geprüft und für die übrigen Plattformen ausstehend beschrieben werden.

Änderungen bleiben in zusammenhängenden kleinen Gruppen reviewbar. Vor einer Änderung von mehr als fünf Dateien wird gemäß Repository-Regel eine repräsentative Diff-Zusammenfassung gezeigt. Veraltete Skripte werden erst nach nachgewiesener Übernahme und ausdrücklicher Scopeentscheidung entfernt; Archive bleiben unangetastet.

## Vorhandene Bausteine und konkrete Lücken

| Baustein | Weiterverwenden | Zu beheben |
|---|---|---|
| [Runner](../../scripts/_110_test_runner.sh) | Ablauf, Fortschritt, Stop-Verhalten und Quellbyteprüfung | Gesamt-PASS trotz WARN; gemeinsame Voraussetzungen; pro Lauf getrennte Logs. |
| [Setup-Helfer](../../scripts/_105_setup_test_tools.sh) | Geprüfte Windows-Pakete, Cppcheck und lokale Caches | Verteilte Pins, interaktive Zwischenfragen, unvollständige Werkzeugabdeckung. |
| [Compilerprüfung](../../scripts/_490_test_runtime_prepare.sh) | C/C++-Übersetzung, Linken, Ausführung und Compilerfamilienprüfung | Einheitlich vor abhängigen Tests ausführen; Auswahl nicht von beliebigen Zusatzinstallationen abhängig machen. |
| [Logging-Integration](../../scripts/_515_test_logging_features.sh) | Reale CE-/SL-Datensätze und namentliche PASS-Prüfung | Gemeinsames Ergebnisverfahren und eindeutig vollständige native Ausführung. |
| [Skript-CI](../../.github/workflows/script-portability.yml) | Drei native Betriebssysteme und schnelle Fixturetests | Echte Integrationen als zusätzliche Pflichtabdeckung. |
| [PC-Matrix](../../scripts/_160_pc_target_test_worker.sh) | Reale C-Datensätze, Decoder, begrenzte Parallelität und vorhandene Konfigurationen | Definierter Compilerumfang pro Plattform und vollständige CI-Anbindung. |
| [Coverage](../../.github/workflows/coverage.yml) | Vergleichbare Linux-Messung und bisheriger Paketumfang | Gleichwertigkeit zur lokalen Auswahl sichern; keine verdeckte Reduktion. |
| [Installationsprüfungen](../../.github/workflows/install-checks.yml) | Native Toolinstallation und Paket-Smoke-Tests | Dieselben Werkzeugdefinitionen und eindeutiger Bezug zum geprüften Releasecommit. |

## Grenzen des Nachweises

Ein einmal bestandener Lauf garantiert keine Fehlerfreiheit für alle künftigen OS-/Werkzeugversionen. Die belastbare Aussage nennt Commit, Plattformen, Werkzeuge und ausgeführte Prüfungen. Periodische native Läufe und kontrollierte Werkzeugupdates halten diesen Nachweis aktuell.

Eine lokale macOS-Sitzung kann keine native Windows-/Linux-Abnahme liefern. Die Implementierung wird lokal soweit möglich geprüft; die übrigen Nachweise müssen von ihren tatsächlichen Runnern kommen. Eine CI-Ausführung, ein Push oder eine Änderung am Branchschutz wird nicht allein durch das Vorhandensein eines Workflows behauptet.
