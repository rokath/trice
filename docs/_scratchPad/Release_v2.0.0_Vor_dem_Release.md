# Trice v2.0.0: sinnvoll vor dem Release

## Zuerst erledigen

1. **V01–V03 beheben:** lokal reproduzierte Fehler in Netzwerk-Close, Tastatureingabe/Rückkanal und lokalem C-Logging; jeweils den fehlenden Verhaltenstest ergänzen.
2. **V04–V06 abschließen:** Stable-Veröffentlichung vorbereiten, das fehlende PDF-Kapitel aufnehmen und die zugesagte Bedeutung von `-exec` klären.
3. **V07 vor der Archivlöschung bearbeiten:** die wenigen eigenständigen Inhalte sichern und aktive Verweise anpassen.
4. **V08 am endgültigen Release-Kandidaten prüfen:** offene CI-Nachweise schließen. Erst danach den unveränderlichen Release-Tag setzen und veröffentlichen.

Die größere Aufräumarbeit, mögliche Skriptumbenennungen und weitere Testideen stehen in [Besser nach dem Release](Release_v2.0.0_Nach_dem_Release.md). Die folgenden Punkte sind Arbeitsvorschläge, keine bereits ausgeführten Implementierungen.

## Stand und Aussagekraft

Review vom 2026-11-03, Branch `wip`, HEAD `08b31e2ca033aa428e4d06e7bd41231ef2075b0d`. Ausgangspunkt war ein sauberer Arbeitsbaum. Inventar: 2.530 versionierte Dateien, davon 338 unter `docs/_scratchPad/obsolete` mit zusammen rund 5,4 MB.

Untersucht wurden Repository-Struktur, aktive Handbücher und ihre Verweise, Versionsgeschichte ab `v1.0.0`, Host- und Target-Code mit Schwerpunkt auf Fehlergrenzen, zugehörige Tests, Beispiele, Skriptaufrufer, Release-Konfiguration, vorhandene CI-Ergebnisse und die Themen des Archivs. Das ist eine breite statische Prüfung mit gezielten Reproduktionen, kein Nachweis für jede Codezeile oder jede Hardwarekonfiguration. Vendorte Bibliotheken und Binärpakete wurden nicht vollständig auditiert.

Kleine eindeutige Text-, Link- und Kommentarkorrekturen wurden direkt vorgenommen und sind im Git-Diff sichtbar. Sie werden hier nicht als offene Aufgaben wiederholt. Laufzeitverhalten, Build-Konfiguration und Testlogik wurden nicht geändert. Das Archiv blieb unverändert. Es wurden keine vollständigen Testsuiten, Coverage-Läufe, Firmwarematrizen oder Release-Veröffentlichungen gestartet.

## Netzwerkverbindungen stürzen beim ausführlichen Schließen ab

**V01 · hohe Priorität · reproduzierter Fehler**

**Fundstelle:** [internal/receiver/receiver.go](../../internal/receiver/receiver.go), `newTCP4Connection`, `newUDPConnection`, beide `Close`-Methoden und `NewReadWriteCloser`; im Review etwa Zeilen 153–220 und 294–317.

Die beiden Konstruktoren lassen das Feld `w` leer. Bei `receiver.Verbose = true` schreibt `Close()` zuerst mit `fmt.Fprintln(p.w, ...)`. Das verursacht einen Nil-Pointer-Panic, bevor `conn.Close()` erreicht wird. Der öffentliche Factory-Aufruf übernimmt den übergebenen Ausgabewriter nicht in diese Objekte.

**Reproduktion:** je eine lokale TCP4- und UDP4-Verbindung über `NewReadWriteCloser` erzeugen, ausführliche Ausgabe aktivieren und das zurückgegebene Objekt schließen. Beide Fälle lieferten `runtime error: invalid memory address or nil pointer dereference`.

**Warum vorhandene Tests das übersehen:** [receiver_test.go](../../internal/receiver/receiver_test.go) setzt beim UDP-Factory-Test `udp.w` nachträglich. Die separaten Close-Tests bauen Objekte bereits mit Writer auf. Damit wird die fehlerhafte öffentliche Kette nicht geprüft.

**Maßnahme:** den Writer im echten Erzeugungspfad konsistent setzen und den Close-Pfad mit dem unveränderten Factory-Ergebnis testen. Vorhandene Close-Diagnostik erhalten.

**Erledigt, wenn:** TCP und UDP mit und ohne `Verbose` über die öffentliche Factory sicher schließen; der Test prüft auch das tatsächliche Schließen der Verbindung und repariert keine privaten Felder.

## Tastatur-EOF erzeugt Kommandos, Diagnosewrapper verschlucken sie

**V02 · hohe Priorität · EOF und verlorene Writes reproduziert, UDP-Panic statisch belegt**

**Fundstellen:** [internal/keybcmd/keybcmd.go](../../internal/keybcmd/keybcmd.go), `loopAction` und `keyboardInput`; [translator.go](../../internal/translator/translator.go), `Translate`; [receiver.go](../../internal/receiver/receiver.go), `udp4.Write`, `bytesViewer.Write` und `binaryLogger.Write`; [handler.go](../../internal/args/handler.go), Aufbau der Wrapperkette.

`loopAction` ignoriert den Fehler von `ReadString`. Bei EOF wird aus dem leeren String ein an das Ziel geschriebenes Nullbyte. Die umgebende Goroutine ruft die Funktion endlos erneut auf. Geschlossene oder umgeleitete Standardeingabe kann so fortlaufend leere Kommandos, Konsolenausgaben und CPU-Last erzeugen. Auch Schreibfehler werden ignoriert; eine Beendigung mit dem Log-Lauf ist nicht vorgesehen.

Die Shell wird bei `-ds` gestartet. `udp4.Write` enthält einen expliziten Panic; ein weitergeleitetes Kommando bei dieser Transportkombination erreicht daher einen nicht unterstützten Schreibpfad. [TestUDP4WritePanics](../../internal/receiver/receiver_test.go) bestätigt derzeit den Panic, nicht ein brauchbares Benutzerverhalten.

**Reproduktion:** ein einzelner `loopAction`-Aufruf mit einem bereits leeren `strings.Reader` schrieb `[0]`. Für die Diagnose wurde bewusst keine Endlosschleife gestartet. Die Wiederholung folgt unmittelbar aus der vorhandenen Schleife.

Ein weiterer Fehler betrifft auch schreibfähige TCP-/UART-Ziele: `-showInputBytes` und `-binaryLogfile` legen Diagnosewrapper um die Verbindung. Beide Wrapper geben bei `Write` nur `(0, nil)` zurück und leiten nichts weiter. Ein über ihre öffentlichen Konstruktoren eingerichteter Test schrieb `[104 105 0]` (`hi` plus Nullabschluss); bei beiden blieb das Ziel leer, Rückgabe `n=0, err=nil`. Die Optionen können somit den Rückkanal still abschalten. Die bestehenden `TestBinaryLoggerWriteAndClose` und `TestBytesViewerWriteAndClose` erwarten aktuell gerade dieses Verwerfen.

**Testschwäche:** die asynchronen Tests in [keybcmd_test.go](../../internal/keybcmd/keybcmd_test.go) warten auf eine Ausgabe, beenden und verbinden die gestartete Goroutine aber nicht wieder mit dem Testablauf. Nach EOF kann sie weiterlaufen und unter anderem auf die global umgeschaltete Standardausgabe zugreifen.

**Maßnahme:** EOF und andere Lesefehler auswerten, Schreibfehler und unvollständige Writes behandeln und einen beendbaren Shell-Lebenszyklus festlegen. Diagnosewrapper müssen Writes an schreibfähige Quellen weiterreichen. Nicht schreibfähige Transportkombinationen verständlich ablehnen; eine neue UDP-Antwortadressierung ist dafür nicht erforderlich und wäre ein eigener Funktionsauftrag.

**Erledigt, wenn:** Tests EOF ohne Daten, eine letzte Zeile ohne Newline, absichtlich leere Eingaben, Writer-Fehler und Beendigung abdecken; nach Testende bleibt keine Shell-Goroutine aktiv. Beide Diagnosewrapper einzeln und kombiniert auf unveränderte Weiterleitung sowie Fehler-/Short-Write-Weitergabe prüfen. Ein gezielter Race-Test sollte diese Lebenszyklen einschließen. Die Kombination aus Remote-Display und UDP muss ohne Panic definiert behandelt werden.

## Abschließendes `%t` scheitert im lokalen C-Logger

**V03 · hohe Priorität · reproduzierter Fehler**

**Fundstelle:** [src/triceLog.c](../../src/triceLog.c), `triceLogIsIntegerConversion` und `triceLogParseConversion`, etwa Zeilen 124–202. Die unterstützte Bool-Ausgabe ist in [Local Deferred Text Log](../rm/target_implementation.md#local-deferred-text-log) dokumentiert.

Bei `TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS = 1` funktioniert `"bool=%t\n"`, aber `"bool=%t"` liefert `-104` und keinen Text. Ursache: `strchr("diuoxXbOq", '\0')` findet den Stringabschluss. Deshalb hält der Parser das abschließende `t` für einen Längenmodifikator und läuft auf das Ende des Formats.

**Reproduktion:** Ringpuffer, ein `trice8`-Record mit Wert `1`, zwei Metadateneinträge für die Formate mit und ohne Newline, Ausgabe über `TriceLog`:

```text
terminal bool: result=-104 expected=9 text=[]
newline bool: result=10 expected=10 text=[bool=true
]
```

**Testlücke:** [local_log_integration_test.go](../../internal/id/local_log_integration_test.go) enthält unter anderem `bool=%t\n` und `%t` mitten im Format, aber nicht diese Endposition.

**Maßnahme:** die Erkennung eines echten Integer-Konversionszeichens gegen den Nullabschluss absichern.

**Erledigt, wenn:** Bool-Werte am Formatende sowie vor Newline und weiterem Text funktionieren; `%td` bleibt als C-Längenmodifikator korrekt; deaktivierte erweiterte Formate liefern weiterhin den vorgesehenen Feature-Fehler. Die Regression über den öffentlichen Log-Pfad für Ring- und Doppelpuffer absichern.

## Stable-Release und Versionsbeschreibung fertigstellen

**V04 · hohe Priorität · verbleibende Release-Vorbereitung**

**Fundstellen:** [.goreleaser.yaml](../../.goreleaser.yaml), `release.prerelease: true`; [.github/workflows/goreleaser.yml](../../.github/workflows/goreleaser.yml), Schritt `Prepare selected release notes`; [CHANGELOG.md](../../CHANGELOG.md).

Zwei konkrete Voraussetzungen fehlen für `v2.0.0`:

- `prerelease: true` würde auch den stabilen Tag als Prerelease veröffentlichen. GoReleaser unterscheidet ausdrücklich `true`, `false` und tagabhängiges `auto`; siehe [offizielle Release-Konfiguration](https://goreleaser.com/customization/publish/scm/).
- Der Workflow verlangt eine nicht leere Überschrift `## v2.0.0` oder dieselbe Version mit folgendem Leerzeichen. Diese fehlt. Der derzeitige Workflow bricht deshalb für diesen Tag beim Auswählen der Release Notes ab.

Der aktuelle Changelog verteilt den Stand auf `Unreleased` und drei Beta-Abschnitte. Die Aussage im dritten Beta-Abschnitt, dass der Alias-/Tag-/CLI-/CE-Umbau nicht implementiert sei, beschreibt dessen historischen Umfang; sie darf nicht als Beschreibung des stabilen Gesamtstands übernommen werden. Lokal existieren außerdem die unterschiedlichen Tag-Schreibweisen `v2.0.0-beta3` und `v2.0.0-beta-3`. Bestehende Tags nicht nachträglich umbenennen; beim Auswählen von Release Notes exakt auf die Schreibweise achten.

**Maßnahme:** den Stable-Abschnitt mit den tatsächlich enthaltenen Änderungen konsolidieren und den Prerelease-Schalter passend zur vorgesehenen Beta-/Stable-Politik setzen. Das vorhandene `skip_upload: auto` für Homebrew im Zusammenhang mit beiden Tagarten kontrollieren; siehe [GoReleaser Homebrew-Konfiguration](https://goreleaser.com/customization/publish/homebrew_casks/). Die bestehende Entscheidung für Release-Archive bzw. lokale Checkout-Builds ohne `/v2`-Modulmigration bleibt unverändert.

**Erledigt, wenn:** die Vorbereitung für einen Beta-Tag und `v2.0.0` jeweils die richtigen, nicht leeren Notes auswählt; die Stable-Veröffentlichung kein Prerelease wird und die gewünschte Tap-Veröffentlichung eindeutig ist. Prüfungen an [release_preparation_test.go](../../scripts/release_preparation_test.go) anschließen, ohne im Test reale Releases zu veröffentlichen.

## Das Kapitel zu Stamp-Ausdrücken fehlt im Referenz-PDF

**V05 · mittlere Priorität · statisch nachgewiesene Dokumentationslücke**

[stamp_expressions.md](../rm/stamp_expressions.md) wird aus README, Changelog und anderen Kapiteln verlinkt, fehlt aber in [docs/rm/README.md](../rm/README.md). Es hat außerdem keinen `mdtoc`-Block.

Der Assembler [scripts/_prepare_reference_manual.cjs](../../scripts/_prepare_reference_manual.cjs) wählt Kapitel aus dieser README-Liste. Verweise auf andere Markdown-Dateien werden zu Weblinks, statt deren Inhalt in das PDF aufzunehmen. Der Refresh-Check in [_310_refresh_trice_manuals.sh](../../scripts/_310_refresh_trice_manuals.sh) verlangt die Behandlung gelisteter beziehungsweise bereits von `mdtoc` verwalteter Kapitel; dieses Kapitel fällt durch beide Auswahlkriterien. Ein erfolgreicher PDF-Build allein beweist deshalb keine Vollständigkeit.

**Maßnahme:** das Kapitel nahe `Stamps and Time Measurements` einordnen, seine Navigation in den vorhandenen Handbuchprozess aufnehmen und einen kleinen Kapitelinventar-Test ergänzen. Gewollte Ausnahmen wie Index, Header und Footer ausdrücklich behandeln.

**Erledigt, wenn:** das Referenz-PDF die Beispiele für `printf`, `unwrap` und `delta` selbst enthält, interne Kapitelverweise funktionieren und der Inventar-Test ein erneut ausgelassenes Fachkapitel erkennt. Die aktuelle Release-PDF-Prüfung kontrolliert Vorhandensein/Größe, nicht diesen Inhalt.

## Die Option `-exec` verspricht eine nicht implementierte Aktion

**V06 · mittlere Priorität · erreichbarer Platzhalter**

[internal/args/init.go](../../internal/args/init.go) bewirbt `-exec` als zusätzliche Kommandozeile für TCP4, etwa zum Start eines GDB-Servers. Der TCP- und UDP-Pfad in [receiver.go](../../internal/receiver/receiver.go) druckt jedoch nur `todo: execute ...`. Es wird kein Prozess gestartet. Auch die generierte CLI-Referenz übernimmt das Versprechen.

**Empfehlung vor dem Release:** nicht leere Verwendung verständlich als nicht unterstützt ablehnen und die Hilfe entsprechend formulieren. Eine echte Prozessverwaltung wäre eine eigenständige Funktion mit Lebenszyklus-, Fehler- und Plattformfragen und sollte nicht beiläufig ergänzt werden.

**Erledigt, wenn:** ein Benutzer keinen erfolgreichen Kommando-Start aus der Option ableiten kann, obwohl keiner stattfindet; Hilfe, generierte Referenz und CLI-Verhalten stimmen überein. Einen Test für diesen öffentlichen Pfad ergänzen. Das ist mehr als ein Tippfehler, daher wurde die Entscheidung nicht vorweggenommen.

## Vor der Löschung von `obsolete` Wissen gezielt sichern

**V07 · mittlere Priorität · an die geplante Archivlöschung gebunden**

Der tatsächliche Pfad ist `docs/_scratchPad/obsolete`. Die 338 versionierten Dateien enthalten Handbuchstände, abgeschlossene Pläne, Experimente, Codekopien und wenige eigenständige Erläuterungen. Nicht den gesamten Ordner in neue Anhänge verschieben. Für die folgenden Archivpfade gilt der oben genannte Review-Commit; nach der Löschung bleiben sie über Git erreichbar.

| Archivinhalt | Erhaltenswert | Vorgeschlagenes Ziel und Umfang |
| --- | --- | --- |
| `AddingANewDecoder.md` | Ja: aktueller Erweiterungspfad über `decoder.Decoder`, `NewDecoderData`, `Register` und den Import im Translator. Die beschriebenen Schnittstellen existieren weiterhin. | Ein kurzer Anhang „Adding a host decoder“ in [Test Internals](../rm/target_test_internals.md), mit Verweisen auf Registrierung und passende Konstruktor-/Read-Tests. Die eigentliche Anleitung mit Konstruktor-Signatur erhalten. |
| `TriceRx_Tlog_C_ArchitectureDesign.md` | Ja, ausgewählte Architekturregeln: Parse → Resolve → Consume; Payload wird nicht kopiert; Lebensdauer an den Eingabepuffer gebunden; Parser kennt zunächst keine TIL-Metadaten; RX kann unabhängig von TX eingebunden werden. | Kurzer RX-Anhang in [Target Implementation](../rm/target_implementation.md), abgeglichen mit `src/triceRx.h` und `.c`. Parserlänge und Behandlung unvollständiger Daten erklären. Zukunftspläne für einen separaten C-Host-Decoder, alte Konfigurationsnamen und erledigte Implementierungsschritte weglassen. |
| `TriceABC_ImplementationSpec.md` | Die relevante Protokollabgrenzung ist bereits aktiv dokumentiert: Adressierung, Antworten, Zuverlässigkeit und Retries gehören zur Anwendung. | [ABC-Kapitel](../rm/abc_asynchronous_broadcast_commands.md) behalten; keine zweite Spezifikation kopieren. |
| `TriceBind/`, insbesondere `Bind_ELF_Comparison_EN.md`, und `experiments/` | Begründungen für Sidecars, Compilergrenzen und Alternativen sind wertvoll, aber weitgehend bereits in den Bind-Anhängen enthalten. Alte Versuchsbauten sind keine aktuellen Beispiele. | [Bind](../rm/bind.md) als maßgebliche Darstellung behalten. Historische Messungen nur mit Compiler, Revision und Versuchsgrenzen verwenden; kein pauschales Übernehmen von Experimenten. |
| CE-/Rebase-Pläne, `trice_logging_handover*` und Alias-Review-Notizen | Entscheidungen sind überwiegend in Code, Tests und aktiven CE-/Tag-/Alias-Kapiteln angekommen. Frühe Verbote oder alte Fehlerstände können inzwischen überholt sein. | Gegen [Context Enrichment](../rm/context_enrichment.md), [Legacy Macros](../rm/legacy_user_code.md) und die vorhandenen Verhaltenstests abgleichen; keine alten offenen Listen reaktivieren. |
| `trice-til-v2-entwurf-0.2/`, ältere TIL-Entwürfe und Serial-Studio-Ideen | Als mögliche spätere Entwicklung, ausdrücklich nicht als implementierter Bestandteil von Trice v2.0.0. | Die knappe inhaltliche Sicherung steht unter „Zukunftsideen aus dem Archiv“ in [Nach dem Release](Release_v2.0.0_Nach_dem_Release.md). Kein Schemawechsel oder neues Ausgabeformat vor dem Release nötig. |
| `rm1/`, `_Legacy/`, `docs/`, `cmd/`, `testing/`, `reports/`, alte Übergaben | Vor allem historische Handbücher, Transkripte, Code und abgeschlossene Arbeitsstände. Die alten Handbuchdateien sind nicht alle bytegleich mit den neuen; Inhalte wurden auch zusammengeführt. | Aktive Handbücher und Tests als Referenz verwenden. Originale in Git belassen. Historische Testausgaben und KI-Vergleichsberichte nicht als aktuelle Abnahme oder belegte Leistungsdaten ausgeben. |

**Beim späteren Löschcommit außerdem drei aktive Verweise anpassen:** [.github/workflows/README.md](../../.github/workflows/README.md), [Development Environment Setup](../rm/development_environment_setup.md) und [Running the Tests](../rm/target_tests.md) verweisen noch ausdrücklich in das Archiv. Nicht die archivierten Dateien selbst reparieren. Der Ausschluss des Scratchpads in Link-Prüfungen ersetzt diese Kontrolle nicht.

Ein Original lässt sich nach der Löschung beispielsweise so lesen:

```sh
git show 08b31e2ca033aa428e4d06e7bd41231ef2075b0d:docs/_scratchPad/obsolete/AddingANewDecoder.md
```

**Erledigt, wenn:** die Decoder-Anleitung und die aktuellen RX-Grundsätze verständlich außerhalb des Archivs stehen, die Zukunftsideen auffindbar bleiben und keine aktive Dokumentation mehr auf gelöschte Archivdateien angewiesen ist. Die Archivlöschung selbst wurde in diesem Review nicht ausgeführt.

## Die letzte CI-Evidenz ist noch nicht vollständig grün

**V08 · hohe Priorität für die endgültige Freigabe · offene Nachweise, nicht pauschal Produktfehler**

Read-only-Abfrage der PR-Workflows für den Review-HEAD am 2026-11-03. Die verwendete API liefert PR-getriggerte Läufe und nur die erste Ergebnisseite; daraus folgt keine Aussage über sämtliche Nightly-, manuellen oder früheren Läufe.

| Beobachtung | Bedeutung und nächste Handlung |
| --- | --- |
| `LinkCheck`, Run `41666316206`, Job `126024390482`: fehlgeschlagen; 2.036 Links, davon 1.935 erfolgreich, 99 ausgeschlossen und 2 Fehler. | Beide Fehler sind Verbindungsfehler zu `clang.llvm.org/docs/LanguageExtensions.html` und `releases.llvm.org/download.html`, aus CE- und Setup-Dokumentation. Beide Seiten waren beim anschließenden Browserabruf erreichbar. Am Kandidaten erneut prüfen; kein Beleg für dauerhaft falsche URLs und kein Anlass zum pauschalen Ausschließen von LLVM. |
| `install-checks`, Run `41666316384`: fehlgeschlagen. `macOS Archive (arm64)`, Job `126025085135`, ist `cancelled`; der Sammeljob `126033985836` meldet `NATIVE_RESULT: abandoned`. | Ein erfolgreicher nativer ARM64-Archivtest fehlt in diesem Lauf. Ursache des Abbruchs aus diesen Daten nicht belegt. Den fehlenden Nachweis nachholen; nicht als nachgewiesenen Paketfehler oder als Erfolg verbuchen. |
| Derselbe Installationslauf: Snapshot-Build, Checkout-Installationen auf drei OS, Linux-/Windows-/macOS-amd64-Archive, DEB/RPM/APK, Target-Sources und PDF-Check erfolgreich. | Gute vorhandene Evidenz, aber kein Ersatz für den fehlenden ARM64-Schritt oder die inhaltliche PDF-Lücke V05. |
| Native Tests, `libraryNightlyQuick`, Script-Portabilität, Coverage, CodeQL und Format-/Lint-Workflows erfolgreich. | Vorhandene Checks sind umfangreich. Der native PR-Pfad verwendet die schnelle Auswahl; daraus lässt sich keine komplette neue Full-/Compiler-Matrix-Abnahme ableiten. |

**Erledigt, wenn:** der endgültige Kandidat die vorgesehenen Checks abgeschlossen hat und die nativen OS-/Architektur-Nachweise eindeutig zu dieser Revision gehören. Alte lokale Full-Ergebnisse im Scratchpad sind historische Evidenz. Die finale Testausführung bleibt gemäß Arbeitsregeln beim Benutzer; in diesem Review wurden keine Workflows neu gestartet.

## Überblick von 1.0 zu 2.0

Für die Release-Kommunikation die kumulative Entwicklung seit `v1.0.0` von den eigentlichen v2-Neuerungen trennen. Grundlage sind die lokalen Tags, [CHANGELOG.md](../../CHANGELOG.md) und die verlinkten aktuellen Handbuchkapitel. ID-basierte Binärausgabe, Insert/Clean, Pufferbetriebsarten, COBS/TCOBS, XTEA und RTT sind keine erst mit v2 eingeführten Grundlagen.

| Zeitraum | Wesentliche Änderung für Anwender | Referenz |
| --- | --- | --- |
| 1.1.x | Makro-Aliase und Benutzer-Tags, UDP-Empfang, zusätzliche Stamp-/Versionsinformationen sowie erweiterte Host-Plattformpakete. | [Legacy Macros](../rm/legacy_user_code.md), [CLI](../rm/cli.md) |
| 1.2.x | Ausbau der Zeitstempel-Differenzen und Portabilität, gemeinsame Decoder-Registrierung, Korrekturen zur Formatstring-Entfernung und printf-Längenmodifikatoren. Eigenständiges `tlog` seit 1.2.3; Ausbau der Installationspakete. | [Stamps](../rm/stamps.md), [Printf Comparison](../rm/printf_comparison.md), [Tool Build/Release](../rm/tool_build_release.md) |
| 1.2.4 bis 1.3.0 | Gezählt übertragene typeX0-Nutzdaten; ABC-Empfang und Broadcast-Kommandos samt Mehrknoten-Beispielen. ABC war bereits Teil von 1.3.0. | [typeX0](../rm/typex0_user_packets.md), [ABC](../rm/abc_asynchronous_broadcast_commands.md) |
| 2.0 | Bind liefert IDs über generierte Sidecar-Header; die Log-Aufrufe brauchen keine eingetragenen IDs. Compiler- und Wrappergrenzen sind ausdrücklich dokumentiert. Insert/Clean bleibt verfügbar. | [Bind](../rm/bind.md) |
| 2.0 | Strukturierte Felder und Text-/JSON-/KV-Ausgabe; Context Enrichment fügt ausgewählten Aufrufen Kontextwerte hinzu. Direkte Bind-CE-Aufrufe und Wrapper-/Rebase-Fälle haben unterschiedliche Grenzen. | [Structured Logging](../rm/structured_logging.md), [CE](../rm/context_enrichment.md) |
| 2.0 | Gewichtete Tags und Filter, konfigurierbare Farben, konsistente Alias-Identität und Tag-Auswertung, geänderte CLI-/Custom-Tag-Schreibweisen. | [Tags](../rm/tags_color_and_weights.md), [Alias Identity](../rm/legacy_user_code.md#alias-identity-and-tags-in-v2) |
| 2.0 | Hostseitige Stamp-Ausdrücke mit `printf`, `unwrap` und `delta`; numerische Visualisierung über Datei oder UDP. | [Stamp Expressions](../rm/stamp_expressions.md), [Visualization](../rm/visualization.md) |
| 2.0 | Lokales verzögertes Text-Logging auf dem Target; erweiterte Generatoren für ausgewählte C-Metadaten, lesbare Dictionary-Kopien und Bind-Berichte. | [Target Implementation](../rm/target_implementation.md), [Generate](../rm/generate.md) |
| 2.0 | Deterministische Zuordnung identischer Aufrufe, verbesserte Reload-/Replay- und Filterpfade, geführtes User Manual, aufgeteiltes Reference Manual und zwei Release-PDFs. | [IDs](../rm/id_management.md), [User Manual](../TriceUserManual.md), [Reference Manual](../rm/README.md) |

**Die wichtigsten Upgrade-Hinweise prominent verlinken:** [Migration von 1.x](../rm/id_management.md#migrating-1x-scripts-and-projects) und [Firmware-/Dictionary-Kompatibilität](../rm/id_management.md#compatibility-with-firmware-and-host-tool-versions).

- Einzelne `{...}` haben nun Feldbedeutung; literale Klammern benötigen `{{` und `}}`. Ein unverändertes binäres Record-Layout macht alte Formattexte nicht automatisch kompatibel. Historische Firmware mit passenden alten Werkzeugen und Dictionaries auswerten.
- `-tilC` wurde durch `-logC` mit Quellenauswahl ersetzt; generierte Standardpfade und LI-Pfade haben sich geändert. `-liPath` wurde auf `-liRoot` beziehungsweise `-liMaxDirs` aufgeteilt.
- `-pick`/`-ban` werden wiederholt statt mit Doppelpunkten kombiniert; alte Kurz-Tags und `-ulabel`-Verwendung prüfen. `-utag`, Gewichte, kanonische Custom-Tags und Alias-Overrides nachlesen; Overrides werden nicht im TIL gespeichert.
- Identische Aufrufe und geänderte ID-Bereiche können neue IDs erhalten. Firmware neu bauen und die passende LI-Datei aufbewahren.
- Source-Builds benötigen Go 1.25 oder neuer. Für v2 Release-Archive oder einen lokalen Tag-Checkout verwenden; keine nicht unterstützte Remote-`go install ...@v2.0.0`-Anleitung veröffentlichen.

Ein kurzer englischer Einstieg für die späteren Stable-Release-Notes:

> Trice 2.0 adds Bind-based instrumentation, structured log fields, Context Enrichment, configurable tag priorities, host-side stamp expressions, visualization output, and local deferred text logging. Since 1.0, the project has also gained the standalone tlog command, UDP input, counted user packets, and ABC broadcast commands. Review the migration guide before rebuilding a 1.x project: template braces, CLI options, tags, generated paths, and dictionary compatibility have changed.

Details und Beispiele gehören in `docs/rm/`; der Changelog sollte diese Zusammenfassung und kurze Migrationshinweise mit Links enthalten.

## Ausgeführte Prüfungen

| Prüfung | Ergebnis |
| --- | --- |
| Gezielt ergänzte temporäre Go-Overlay-Tests, nur `^TestReview`, Pakete `./internal/receiver` und `./internal/keybcmd`, `-count=1 -timeout=45s`; Go 1.25.3, Darwin/arm64. | **Fehlgeschlagen und damit Defekte bestätigt:** TCP4-/UDP4-Close-Panic sowie Nullbyte bei EOF. Keine vorhandenen Repository-Testdateien dafür geändert. |
| Zusätzlicher Overlay-Test `TestReviewDiagnosticWrappersForwardCommands`, nur `./internal/receiver`, `-count=1 -timeout=45s`, gleiche Go-Umgebung. | **Fehlgeschlagen und damit Defekt bestätigt:** sowohl `NewBytesViewer` als auch `NewBinaryLogger` verwerfen das Kommando mit `n=0, err=nil`. |
| Temporäre C-Reproduktion mit `cc -std=c11 -Wall -Wextra -Werror`, Projektkonfiguration vor `src` und `src/default_conf`; Apple Clang 21.0.0, arm64. | **Kompilierung erfolgreich**, Ausführung **fehlgeschlagen**: terminales `%t` ergibt `-104`; Vergleichsfall mit Newline erfolgreich. |
| Statische Prüfung von Release-Notes-Auswahl, Prerelease-Einstellung und PDF-Kapitelauswahl. | V04 und V05 direkt aus Konfiguration und Auswahlcode bestätigt; keine Veröffentlichung und kein kompletter PDF-Neubau ausgeführt. |
| Read-only-Abfrage vorhandener GitHub-Läufe und der beiden Fehlerprotokolle. | Ergebnisse und Grenzen unter V08 dokumentiert. |
| Lokale Prüfung relativer Markdown-Verweise in den geänderten Dateien und beiden Berichten; UTF-8-/BOM-/CRLF-Kontrolle und Vergleich der Inhalte ohne Kommentare. | Keine fehlenden Ziele oder Fragmente gefunden. Encoding und vorhandene CRLF-Zahl erhalten; C-Header und Workflow enthalten ausschließlich Kommentaränderungen. `git diff --check` erfolgreich. |
| Gesamtsuite, Full-Compiler-Matrix, neue Coverage-/Race-Läufe, Hardwaretests und vollständige Paketabnahme. | **Nicht ausgeführt.** Ausgewählte vorhandene CI-Ergebnisse wurden gelesen, nicht als eigene neue Abnahme ausgegeben. |

Die temporären Reproduktionen sind Hilfsmittel dieses Reviews und keine eingecheckten Regressionstests. Bei der Umsetzung die beschriebenen Fälle in die bestehenden Tests aufnehmen. Nach funktionalen Änderungen gelten frühere Checks nicht automatisch für den neuen Stand.
