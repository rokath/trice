# Trice v2.0.0: sinnvoll vor dem Release

## Zuerst erledigen

1. **V01–V03 umgesetzt:** Tests und Dokumentation sind ergänzt; dieser Zwischenstand ist in Git enthalten.
2. **V04–V06 umgesetzt:** Stable-/Upgrade-Hinweise und Veröffentlichungspolitik sind vorbereitet, das Stamp-Kapitel ist im PDF enthalten und die unbenutzte Option `-exec` ist vollständig entfernt. Gezielte Prüfungen stehen unten.
3. **V07 umgesetzt:** Decoder-Anleitung und aktuelle RX-Grundsätze sind außerhalb des Archivs gesichert, aktive Archivverweise entfernt und ein Regressionstest ergänzt. Das Archiv selbst bleibt unverändert.
4. **V08 geprüft, endgültige Freigabe offen:** Die früheren LinkCheck-/ARM64-Lücken sind für den veröffentlichten Commit `83d7316f` geschlossen. Die noch uncommitteten Änderungen V04–V07 gehören nicht zu diesen CI-Ergebnissen. Erst den endgültigen Kandidaten prüfen, danach den unveränderlichen Release-Tag setzen und veröffentlichen.

Die größere Aufräumarbeit, mögliche Skriptumbenennungen und weitere Testideen stehen in [Besser nach dem Release](Release_v2.0.0_Nach_dem_Release.md). Die Fehlerbeschreibungen dokumentieren den ursprünglichen Review-Stand; der jeweilige Umsetzungsstand steht direkt beim Punkt.

## Stand und Aussagekraft

Review vom 2026-11-03, Branch `wip`, HEAD `08b31e2ca033aa428e4d06e7bd41231ef2075b0d`. Ausgangspunkt war ein sauberer Arbeitsbaum. Inventar: 2.530 versionierte Dateien, davon 338 unter `docs/_scratchPad/obsolete` mit zusammen rund 5,4 MB.

Untersucht wurden Repository-Struktur, aktive Handbücher und ihre Verweise, Versionsgeschichte ab `v1.0.0`, Host- und Target-Code mit Schwerpunkt auf Fehlergrenzen, zugehörige Tests, Beispiele, Skriptaufrufer, Release-Konfiguration, vorhandene CI-Ergebnisse und die Themen des Archivs. Das ist eine breite statische Prüfung mit gezielten Reproduktionen, kein Nachweis für jede Codezeile oder jede Hardwarekonfiguration. Vendorte Bibliotheken und Binärpakete wurden nicht vollständig auditiert.

Beim ursprünglichen Review wurden kleine eindeutige Text-, Link- und Kommentarkorrekturen direkt vorgenommen. Sie werden hier nicht als offene Aufgaben wiederholt. Laufzeitverhalten, Build-Konfiguration und Testlogik wurden damals nicht geändert. Das Archiv blieb unverändert. Es wurden keine vollständigen Testsuiten, Coverage-Läufe, Firmwarematrizen oder Release-Veröffentlichungen gestartet.

## Netzwerkverbindungen stürzen beim ausführlichen Schließen ab

**V01 · hohe Priorität · reproduzierter Fehler**

**Umgesetzt am 2026-11-04:** TCP4 und UDP4 übernehmen den Diagnosewriter bereits im Konstruktor. Der neue Factory-Test prüft beide Transporte mit und ohne `Verbose`, das geschlossene Socket und bei TCP zusätzlich EOF beim Peer. Die bisherigen Tests reparieren keine Writer-Felder mehr nachträglich. Gezielte V01-Prüfung: **9 Tests erfolgreich** unter Windows; keine Gesamtabnahme.

**Fundstelle:** [internal/receiver/receiver.go](../../internal/receiver/receiver.go), `newTCP4Connection`, `newUDPConnection`, beide `Close`-Methoden und `NewReadWriteCloser`; im Review etwa Zeilen 153–220 und 294–317.

Die beiden Konstruktoren lassen das Feld `w` leer. Bei `receiver.Verbose = true` schreibt `Close()` zuerst mit `fmt.Fprintln(p.w, ...)`. Das verursacht einen Nil-Pointer-Panic, bevor `conn.Close()` erreicht wird. Der öffentliche Factory-Aufruf übernimmt den übergebenen Ausgabewriter nicht in diese Objekte.

**Reproduktion:** je eine lokale TCP4- und UDP4-Verbindung über `NewReadWriteCloser` erzeugen, ausführliche Ausgabe aktivieren und das zurückgegebene Objekt schließen. Beide Fälle lieferten `runtime error: invalid memory address or nil pointer dereference`.

**Warum vorhandene Tests das übersehen:** [receiver_test.go](../../internal/receiver/receiver_test.go) setzt beim UDP-Factory-Test `udp.w` nachträglich. Die separaten Close-Tests bauen Objekte bereits mit Writer auf. Damit wird die fehlerhafte öffentliche Kette nicht geprüft.

**Maßnahme:** den Writer im echten Erzeugungspfad konsistent setzen und den Close-Pfad mit dem unveränderten Factory-Ergebnis testen. Vorhandene Close-Diagnostik erhalten.

**Erledigt, wenn:** TCP und UDP mit und ohne `Verbose` über die öffentliche Factory sicher schließen; der Test prüft auch das tatsächliche Schließen der Verbindung und repariert keine privaten Felder.

## Tastatur-EOF erzeugt Kommandos, Diagnosewrapper verschlucken sie

**V02 · hohe Priorität · EOF und verlorene Writes reproduziert, UDP-Panic statisch belegt**

**Umgesetzt am 2026-11-04:** EOF beendet die Shell; eine letzte Zeile ohne Newline wird einmal gesendet, eine absichtlich leere Zeile bleibt ein Nullbyte-Kommando. Andere Lesefehler, Schreibfehler und kurze Writes beenden die Shell mit Diagnose, ohne Wiederholung. Der Log-Lauf besitzt und beendet die Shell einschließlich blockierter Reads/Writes; das geliehene Standardinput bleibt offen. Eigene Eingabehandles ermöglichen den Abbruch unter Windows und Unix. Beide Diagnosewrapper leiten Writes und deren Rückgaben unverändert weiter. `-ds`/`-displayserver` mit UDP4 wird vor dem Start abgelehnt; direkte UDP-Writes liefern einen Fehler statt eines Panics. Das Verhalten ist in [CLI](../rm/cli.md#stimulate-target-with-a-user-command-over-uart) beschrieben.

**Gezielt geprüft:** 55 Tests für V01/V02 mit Race-Detector erfolgreich unter Windows/amd64, Go 1.26.1. Der neue Unix-Adapter kompiliert für Linux/amd64 und Darwin/amd64. Native Linux-/macOS-Läufe und eine interaktive Terminalabnahme wurden nicht ausgeführt; die Windows-Lebenszyklustests verwenden Pipes.

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

**Umgesetzt am 2026-11-04:** Die Integer-Konversionserkennung schließt den Nullabschluss ausdrücklich aus. Der neue öffentliche Producer-/`TriceLog`-Test prüft `0`, `1` und `255` für `%t` am Formatende, vor Newline und vor Text; `%td` und `%tu` bleiben Integer-Konversionen. Alle Fälle laufen mit Ring- und Doppelpuffer, jeweils mit aktivierten und deaktivierten erweiterten Formaten. Bei deaktiviertem `%t` werden Feature-Fehler und leere Ausgabe geprüft. [Local Deferred Text Log](../rm/target_implementation.md#local-deferred-text-log) enthält das konkrete Beispiel und die Abgrenzung zu C-Längenmodifikatoren.

**Gezielt geprüft:** Die drei ausgewählten Go-Integrationstests mit insgesamt acht kompilierten und ausgeführten C-Testprogrammen bestehen unter Windows/amd64 mit GCC 10.3.0. Dazu gehören die bisherigen Puffer-/Byteorder- und deaktivierten Formatter-Fälle. Keine vollständige Compiler-Matrix oder Gesamtabnahme.

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

**Umgesetzt am 2026-11-04:** `release.prerelease: auto` unterscheidet Beta-/RC-Tags von stabilen Versions-Tags. Das bestehende Homebrew-`skip_upload: auto` bleibt erhalten: nur Stable aktualisiert den normalen Tap. Der neue Abschnitt `v2.0.0 (unreleased)` bündelt die tatsächlichen Neuerungen, Korrekturen und Upgrade-Grenzen; historische Beta-Notes bleiben unverändert. README, User Manual und Quickstarts verlinken die Migration und zeigen nötige Anpassungen. Die vorhandene Migrationstabelle wurde geprüft; das Release-Handbuch erklärt Tag-Schreibweise und Tap-Veröffentlichung mit einer Vergleichstabelle. Keine `/v2`-Modulmigration und keine Veröffentlichung.

**Gezielt geprüft:** 18 Release-Tests erfolgreich; die vier Fälle der echten Notes-Auswahl nach der letzten Changelog-Ergänzung erneut erfolgreich. Stable und `v2.0.0-beta-3` erhalten ihre eigenen Notes; `v2.0.0-beta3` wird ohne eigene Sektion ausdrücklich abgelehnt. `goreleaser check` mit Version 2.19.0 bestätigt die Konfiguration.

**Fundstellen:** [.goreleaser.yaml](../../.goreleaser.yaml), `release.prerelease: true`; [.github/workflows/goreleaser.yml](../../.github/workflows/goreleaser.yml), Schritt `Prepare selected release notes`; [CHANGELOG.md](../../CHANGELOG.md).

Zwei konkrete Voraussetzungen fehlen für `v2.0.0`:

- `prerelease: true` würde auch den stabilen Tag als Prerelease veröffentlichen. GoReleaser unterscheidet ausdrücklich `true`, `false` und tagabhängiges `auto`; siehe [offizielle Release-Konfiguration](https://goreleaser.com/customization/publish/scm/).
- Der Workflow verlangt eine nicht leere Überschrift `## v2.0.0` oder dieselbe Version mit folgendem Leerzeichen. Diese fehlt. Der derzeitige Workflow bricht deshalb für diesen Tag beim Auswählen der Release Notes ab.

Der aktuelle Changelog verteilt den Stand auf `Unreleased` und drei Beta-Abschnitte. Die Aussage im dritten Beta-Abschnitt, dass der Alias-/Tag-/CLI-/CE-Umbau nicht implementiert sei, beschreibt dessen historischen Umfang; sie darf nicht als Beschreibung des stabilen Gesamtstands übernommen werden. Lokal existieren außerdem die unterschiedlichen Tag-Schreibweisen `v2.0.0-beta3` und `v2.0.0-beta-3`. Bestehende Tags nicht nachträglich umbenennen; beim Auswählen von Release Notes exakt auf die Schreibweise achten.

**Maßnahme:** den Stable-Abschnitt mit den tatsächlich enthaltenen Änderungen konsolidieren und den Prerelease-Schalter passend zur vorgesehenen Beta-/Stable-Politik setzen. Das vorhandene `skip_upload: auto` für Homebrew im Zusammenhang mit beiden Tagarten kontrollieren; siehe [GoReleaser Homebrew-Konfiguration](https://goreleaser.com/customization/publish/homebrew_casks/). Die bestehende Entscheidung für Release-Archive bzw. lokale Checkout-Builds ohne `/v2`-Modulmigration bleibt unverändert.

**Verständlichkeit vor dem Release:** Changelog, README, User Manual/Quickstarts und die Migrationshinweise aus Sicht eines neuen Anwenders durchgehen. Abstrakte Aussagen wie „selectors select event tags“ durch eine konkrete Handlung und deren Ergebnis ersetzen oder unmittelbar mit einem kurzen Beispiel erklären. Bei Upgrade-Regeln möglichst bisherige und neue Verwendung samt sichtbarer Auswirkung zeigen. Fachbegriffe kurz am Einsatzort erklären; ein Link darf notwendiges Grundverständnis nicht ersetzen. Beispiele sollen unklare Prosa ersetzen und Wiederholungen vermeiden. Der vollständige Durchgang durch die übrige aktive Dokumentation ist als N06 in [Nach dem Release](Release_v2.0.0_Nach_dem_Release.md) eingeplant.

**Erledigt, wenn:** die Vorbereitung für einen Beta-Tag und `v2.0.0` jeweils die richtigen, nicht leeren Notes auswählt; die Stable-Veröffentlichung kein Prerelease wird und die gewünschte Tap-Veröffentlichung eindeutig ist. Prüfungen an [release_preparation_test.go](../../scripts/release_preparation_test.go) anschließen, ohne im Test reale Releases zu veröffentlichen.

Für die Dokumentation gilt zusätzlich: Leser erkennen ohne Kenntnis interner Begriffe, wann eine Neuerung hilft, was sie eingeben oder ändern müssen und welches Ergebnis sie erwarten können. Changelog-Einträge bleiben kurz; ein nötiges Inline-Beispiel ist erlaubt, vollständige Anleitungen stehen in `docs/rm/`.

## Das Kapitel zu Stamp-Ausdrücken fehlt im Referenz-PDF

**V05 · mittlere Priorität · statisch nachgewiesene Dokumentationslücke**

**Umgesetzt am 2026-11-04:** Das Stamp-Kapitel steht direkt nach den Zeitstempel-Grundlagen in der Referenzliste und besitzt eine vom vorhandenen `mdtoc`-Prozess verwaltete Inhaltsnavigation. Die bisherigen Überschriften bleiben ohne zusätzliche Quellnummern oder Anker. Ein Inventartest kontrolliert alle Fachkapitel, auch solche ohne `mdtoc`; nur README, Index, Header und Footer sind ausgenommen. Der Assembler-Test verwendet das echte Stamp-Kapitel und prüft Beispiele sowie interne Kapitelziele.

**Gezielt geprüft:** 54 Tests und der gemeinsame Handbuchcheck erfolgreich. Beide PDFs am V04–V06-Zwischenstand erzeugt: User Manual 9 Seiten, Reference Manual 485 Seiten. Die `printf`-/`unwrap`-/`delta`-Beispiele sind auf Seite 116 enthalten und wurden als gerenderte PNG-Seite gelesen; zusätzlich wurden die Referenzseiten 97, 98, 117 und 121 sowie User-Manual-Seiten 1 und 2 sichtgeprüft. Der Assembler bestätigt 929 interne Verweise und 1.284 eindeutige Anker. Im tatsächlichen Referenz-PDF zeigen 971 interne Link-Annotationen auf vorhandene Seitenobjekte, einschließlich des Stamp-Kapitels und seiner Unterabschnitte. Keine vollständige Sichtprüfung aller 485 Seiten.

[stamp_expressions.md](../rm/stamp_expressions.md) wird aus README, Changelog und anderen Kapiteln verlinkt, fehlt aber in [docs/rm/README.md](../rm/README.md). Es hat außerdem keinen `mdtoc`-Block.

Der Assembler [scripts/_prepare_reference_manual.cjs](../../scripts/_prepare_reference_manual.cjs) wählt Kapitel aus dieser README-Liste. Verweise auf andere Markdown-Dateien werden zu Weblinks, statt deren Inhalt in das PDF aufzunehmen. Der Refresh-Check in [_310_refresh_trice_manuals.sh](../../scripts/_310_refresh_trice_manuals.sh) verlangt die Behandlung gelisteter beziehungsweise bereits von `mdtoc` verwalteter Kapitel; dieses Kapitel fällt durch beide Auswahlkriterien. Ein erfolgreicher PDF-Build allein beweist deshalb keine Vollständigkeit.

**Maßnahme:** das Kapitel nahe `Stamps and Time Measurements` einordnen, seine Navigation in den vorhandenen Handbuchprozess aufnehmen und einen kleinen Kapitelinventar-Test ergänzen. Gewollte Ausnahmen wie Index, Header und Footer ausdrücklich behandeln.

**Erledigt, wenn:** das Referenz-PDF die Beispiele für `printf`, `unwrap` und `delta` selbst enthält, interne Kapitelverweise funktionieren und der Inventar-Test ein erneut ausgelassenes Fachkapitel erkennt. Die aktuelle Release-PDF-Prüfung kontrolliert Vorhandensein/Größe, nicht diesen Inhalt.

## Die Option `-exec` verspricht eine nicht implementierte Aktion

**V06 · mittlere Priorität · erreichbarer Platzhalter**

**Umgesetzt am 2026-11-04, anschließend nach Benutzerentscheidung vollständig entfernt:** Die repositoryweite Suche fand keinen produktiven Trice-Aufruf mit `-exec`, sondern nur Registrierung, Platzhalter, die zwischenzeitliche Ablehnung, Tests und Dokumentation. Die `find -exec`-Aufrufe im Release-Audit gehören zum Unix-Werkzeug und bleiben erhalten; eine historische Erwähnung unter `obsolete` bleibt unverändert. Flag-Registrierung, globale Variable und Sonderbehandlung sind entfernt. Auch `-exec=""` ist nun eine unbekannte Option. Hilfe und generierte Referenz führen sie nicht mehr auf; das Handbuch verlangt nur den manuellen Serverstart. Es wurde keine Prozessverwaltung ergänzt.

**Gezielt geprüft nach vollständiger Entfernung:** 57 Tests in `internal/args` und `internal/receiver` erfolgreich. Beide Programme behandeln frühere Schreibweisen einschließlich leerer Werte, `--exec` und `-EXEC` als unbekannte Option und öffnen dabei keinen Eingang. Normale Aufrufe ohne den Schalter, explizite Hilfe und die neu generierte Referenz bestehen ihre Prüfungen. Die vier Release-Notes-Fälle wurden nach der Changelog-Anpassung erneut erfolgreich ausgeführt. Beide PDFs wurden neu erzeugt; die bereinigte CLI-Seite 98 und User-Manual-Seite 2 wurden erneut sichtgeprüft, alle 971 internen PDF-Verweise erneut aufgelöst. Die frühere Prüfung der bloßen Ablehnung ist dadurch als aktueller V06-Nachweis ersetzt.

**Ursprünglicher Befund:** [internal/args/init.go](../../internal/args/init.go) bewarb `-exec` als zusätzliche Kommandozeile für TCP4, etwa zum Start eines GDB-Servers. Der TCP- und UDP-Pfad in [receiver.go](../../internal/receiver/receiver.go) druckte jedoch nur `todo: execute ...`. Es wurde kein Prozess gestartet. Auch die generierte CLI-Referenz übernahm das Versprechen.

**Verbindliche Entscheidung:** Nach Prüfung auf echte Verwendung die Option vollständig entfernen. Eine echte Prozessverwaltung bleibt außerhalb dieses Auftrags.

**Erledigt, wenn:** Registrierung, Zustand und Sonderbehandlung fehlen; Hilfe und generierte Referenz enthalten keinen solchen Schalter. Beide Programme behandeln seine frühere Verwendung als unbekannte Option, während normale Aufrufe ohne ihn weiter funktionieren.

## Vor der Löschung von `obsolete` Wissen gezielt sichern

**V07 · mittlere Priorität · an die geplante Archivlöschung gebunden**

**Umgesetzt am 2026-11-04:** [Adding a host decoder](../rm/target_test_internals.md#adding-a-host-decoder) erhält die Konstruktor-Signatur, Initialisierung, Registrierung, den Translator-Import und passende Testverweise. [Receiving and resolving records](../rm/target_implementation.md#receiving-and-resolving-records) erklärt den heutigen Parse-/Resolve-/Consume-Pfad, Pufferlebensdauer, logische Länge und unvollständige Records anhand der aktuellen C-Schnittstellen. Die Zukunftsideen bleiben im Bericht [Nach dem Release](Release_v2.0.0_Nach_dem_Release.md) auffindbar. Keine Archivdatei wurde verändert oder gelöscht.

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

**Drei aktive Verweise bereits angepasst:** [.github/workflows/README.md](../../.github/workflows/README.md), [Development Environment Setup](../rm/development_environment_setup.md) und [Running the Tests](../rm/target_tests.md) benötigen das Archiv nicht mehr. Der neue Test `TestActiveManualsHaveNoArchiveDependencies` kontrolliert die aktiven Handbücher und Einstiegspunkte; Beispiele, Kommentare und das historische Scratchpad sind ausgenommen. Die Archivlöschung ist ein separater Schritt.

Ein Original lässt sich nach der Löschung beispielsweise so lesen:

```sh
git show 08b31e2ca033aa428e4d06e7bd41231ef2075b0d:docs/_scratchPad/obsolete/AddingANewDecoder.md
```

**Erledigt, wenn:** die Decoder-Anleitung und die aktuellen RX-Grundsätze verständlich außerhalb des Archivs stehen, die Zukunftsideen auffindbar bleiben und keine aktive Dokumentation mehr auf gelöschte Archivdateien angewiesen ist. Die Archivlöschung selbst wurde in diesem Review nicht ausgeführt.

## CI-Evidenz dem endgültigen Kandidaten zuordnen

**V08 · hohe Priorität für die endgültige Freigabe · offene Nachweise, nicht pauschal Produktfehler**

**Aktualisiert am 2026-11-04, 10:40 UTC:** Lokaler und veröffentlichter Branch `wip` stehen auf `83d7316ff024b77aab586edbe427a767a3d6da67`. Die Read-only-Abfrage aller diesem SHA zugeordneten Actions-Läufe liefert 15 von insgesamt 15 Einträgen, ohne Event-Filter. Alle 15 Läufe sind abgeschlossen und erfolgreich. Die Joblisten der unten verlinkten vier Kernläufe sind vollständig (14/14, 4/4, 1/1 und 2/2). Die PR-Läufe verwenden zusätzlich die PR-Merge-Revision; diese Evidenz umfasst keine nachträglichen Änderungen im lokalen Arbeitsbaum.

| Nachweis für `83d7316f` | Ergebnis und Grenze |
| --- | --- |
| [Link Check, Run 41803513920](https://github.com/rokath/trice/actions/runs/41803513920) | **PASS:** Markdown-Linkprüfung erfolgreich; die frühere LLVM-Verbindungslücke ist für diesen Stand geschlossen. |
| [install-checks, Run 41803513828](https://github.com/rokath/trice/actions/runs/41803513828) | **PASS:** alle 14 Jobs, einschließlich nativer Archive für [macOS arm64](https://github.com/rokath/trice/actions/runs/41803513828/job/126486895459) und [macOS amd64](https://github.com/rokath/trice/actions/runs/41803513828/job/126486895721), Windows/Linux, DEB/RPM/APK, Checkout-Installationen, Target-Sources und Release-PDFs. Beide macOS-Archive bestehen auch die Smoke- und ausgelieferten Funktionstests. |
| [Native Trice tests, Run 41803513935](https://github.com/rokath/trice/actions/runs/41803513935) | **PASS:** schnelle native Auswahl auf Windows, Linux und macOS arm64. Der native Intel-Full-Job und die zusätzlichen Concurrency-Schritte sind **übersprungen**, daher kein Full-Nachweis. Der erfolgreiche Intel-Archivtest oben ist davon getrennt. |
| [TRICE Library CI, Run 41803513930](https://github.com/rokath/trice/actions/runs/41803513930) | **PASS:** Heavy Build/Compile-Auswahl `quick`; keine vollständige neue Compiler-Matrix. |
| Übrige elf Läufe desselben SHA | **PASS:** Coverage, CodeQL, Script-Portabilität, Installations-/Format-/Lint-Prüfungen und PR-Automatisierung. Erfolgreiche Automatisierungsjobs ersetzen keine Produktprüfung. |

**Noch offen:** Die Änderungen V04–V07 müssen zuerst in einen überprüfbaren Kandidaten-Commit gelangen und übertragen werden. Anschließend die vorgesehenen nativen Full-/Compiler- und Paketnachweise genau diesem Stand zuordnen und fehlende Checks vom Benutzer ausführen lassen. Die oben gelesenen Ergebnisse dürfen nicht als CI-Abnahme des uncommitteten Arbeitsbaums gelten. Es wurden keine Workflows gestartet, Tags gesetzt oder Releases veröffentlicht.

Die folgende Tabelle hält den **historischen Review-Stand vom 2026-11-03** fest. Damals wurden nur PR-Läufe und die erste Ergebnisseite gelesen; die beiden offenen Nachweise sind inzwischen für `83d7316f` geschlossen.

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

## Prüfungen der Umsetzung

Stand 2026-11-04, Windows/amd64, Go 1.26.1 und GCC 10.3.0. Die oben dokumentierten ursprünglichen Review-Prüfungen sind von diesen neuen Ergebnissen zu unterscheiden.

- **PASS:** V01/V02, 55 ausgewählte Tests einschließlich Unterfällen mit Race-Detector. Die Shell-Tests verbinden ihre Goroutinen vor dem Testende wieder mit dem Testablauf. Native Eingabeabbruchtests verwenden Windows-Pipes.
- **PASS:** V03, drei ausgewählte Go-Integrationstests mit acht C-Testprogrammen, einschließlich vier neuer Puffer-/Feature-Kombinationen. RTK zählt 10 erfolgreiche Tests einschließlich Unterfällen.
- **PASS, nur kompiliert:** Tastaturadapter für Linux/amd64 und Darwin/amd64. Die Ausgabe `NUL` in den folgenden Befehlen ist das Windows-Nullgerät des ausführenden Hosts.
- **PASS:** `gofmt -l` ohne Ausgabe für alle betroffenen Go-Dateien, `git diff --check`, UTF-8-/BOM-/Zeilenendenkontrolle. Neue Go-Dateien beginnen mit dem MIT-SPDX-Header.
- **Nicht ausgeführt:** native Linux-/macOS-Tests, interaktive Terminal-/Hardwareabnahme, vollständige Paketsuiten, `testAll.sh`, Coverage und Full-Compiler-Matrix. Die finale Abnahme bleibt beim Benutzer.

Die fokussierten Test- und Kompilierungsbefehle:

```sh
rtk go test -race ./internal/receiver ./internal/keybcmd ./internal/args -run 'Test(NetworkFactoryClose|UDP4WriteRejectsCommands|DiagnosticWrappersForwardCommands|BinaryLoggerWriteAndClose|BytesViewerWriteAndClose|LoopAction.*|KeyboardInput.*|ReadInput.*|RemoteDisplay.*|CLIReplayClosesInputAndBinaryLogWithDebugWrapper)$' -count=1 -timeout=90s
GOOS=linux CGO_ENABLED=0 go test -c -o NUL ./internal/keybcmd
GOOS=darwin CGO_ENABLED=0 go test -c -o NUL ./internal/keybcmd
rtk go test ./internal/id -run '^TestLocalLog(BooleanConversionBoundaries|CompilesAndRunsWithBothDeferredBuffers|DynamicStringNeedsNoPrintfImplementation)$' -count=1 -timeout=120s
```

Frühe Entwicklungsläufe fanden einen geschlossenen Windows-Eingabehandle sowie fehlende Display-/Printf-Voraussetzungen im Testaufbau. Diese sind korrigiert; die oben genannten erfolgreichen Wiederholungen prüfen den damaligen V01–V03-Codestand. V04–V08 waren zu diesem Zwischenstand noch nicht begonnen. Das Archiv blieb unverändert; Git-Schreiboperationen waren nicht Teil dieser ersten Umsetzungsrunde.

## Prüfungen der Release-Vorbereitung

Stand 2026-11-04, Windows/amd64. Go-Tests mit Go 1.26.1; Konfiguration mit GoReleaser 2.19.0. Dokumentation mit Node.js 26.12.1, `mdtoc` 1.0.3, `stitchmd` 0.9.0, `md-to-pdf` 5.2.5 und Puppeteer 25.16.0.

| Prüfung | Ergebnis |
| --- | --- |
| Release-Notes-Auswahl, Veröffentlichungspolitik und Workflow-/Tagbindung | **PASS:** 18 ausgewählte Tests; Notes-Auswahl am finalen Changelog erneut **PASS**, 4 Fälle. Keine realen Releases oder Tap-Schreibzugriffe. |
| Kapitelinventar, unmanaged Kapitel, echter PDF-Assembler, bisherige Handbuch-/PDF-Fixtures | **PASS:** 54 ausgewählte Tests. Der Assembler-Test läuft mit dem vorhandenen Renderer-Cache, ohne npm-Downloads. |
| Vollständig entfernte Option, unbekannte frühere Schreibweisen, Hilfe-/Snapshot-Gleichheit und Netzwerkregressionen | **PASS:** 57 ausgewählte Tests in `internal/args` und `internal/receiver` nach der vollständigen Entfernung. |
| `goreleaser check`, `scripts/_310_refresh_trice_manuals.sh check` | **PASS.** |
| `scripts/_320_generate_trice_user_manual_pdf.sh` mit passendem Headless-Browser | **PASS:** beide PDFs erzeugt; Inhalts-, Ziel- und begrenzte Sichtprüfung wie unter V05. |
| Neue relative Markdown-Verweise | **PASS:** 41 Ziele/Fragmente vor der Statuspflege; Statuslinks verwenden dieselben bereits geprüften Kapitel. |
| Formatierung und Encoding | `gofmt -l` ohne Ausgabe; UTF-8, BOM und Zeilenenden erhalten; `git diff --check` erfolgreich. Der entfernte Hilfeeintrag benötigt keine Umformatierung des übrigen Snapshots. |
| Gesamtsuiten, Coverage, Full-Compiler-Matrix, native Linux-/macOS-Läufe und Veröffentlichung | **Nicht ausgeführt.** Die finale Abnahme und V08 bleiben beim Benutzer. |

Die fokussierten Go-Befehle:

```sh
rtk go test ./scripts -run '^Test(SelectedReleaseNotes|ReleaseCandidateNotes|ReleasePublicationPolicy|ReleaseTagCheckout|ReleaseWorkflowBindings)$' -count=1 -timeout=90s
rtk go test ./scripts -run '^Test(ReferenceManualChapterInventory|ReferenceInventoryFindsUnmanagedChapters|ReferencePDFPreparation|ManualMaintenance.*|ManualPDFGenerationPreservesBookIdentity)$' -count=1 -timeout=120s
go generate ./internal/args
rtk go test ./internal/args ./internal/receiver -run '^Test(RemovedExecOptionIsUnknown|LogHelpOmitsExec|HelpAll|GeneratedHelpAllDocIsUpToDate|CLIExplicitHelpStillAvailable|NetworkFactoryClose|NewReadWriteCloserUDP4Default|TCP4WriteAndDefaultArguments)$' -count=1 -timeout=90s
rtk go test ./scripts -run '^TestReleaseCandidateNotes$' -count=1 -timeout=45s
```

Der erste PDF-Versuch bereitete den bereits gepinnten Browser vor, scheiterte aber unter Windows beim `--version`-Aufruf von `chrome.exe` mit Timeout. Die Headless-Ausgabe derselben Revision `156.0.8078.47` besteht die unveränderte Versionsprüfung und erzeugt beide Bücher erfolgreich. Für diesen lokalen Lauf wurde in Git Bash gesetzt:

```sh
PUPPETEER_EXECUTABLE_PATH="$(cygpath -m "$PWD")/temp/tools/puppeteer/chrome-headless-shell/win64-156.0.8078.47/chrome-headless-shell-win64/chrome-headless-shell.exe" bash ./scripts/_320_generate_trice_user_manual_pdf.sh
```

Build-Skripte und Toolversionen wurden dafür nicht geändert. PDFs und Vorschaubilder bleiben generierte, Git-ignorierte Prüfartefakte. Diese zweite Umsetzungsrunde betraf V04–V06; V07/V08 folgten in der nächsten Runde. Es wurde nichts gestagt, committet, gepusht oder veröffentlicht.

## Prüfungen der Wissenssicherung und CI-Abfrage

Stand 2026-11-04, Windows/amd64, gleiche Werkzeuge wie oben. V07 ist umgesetzt; V08 bleibt bis zur Abnahme des endgültigen Kandidaten offen.

| Prüfung | Ergebnis |
| --- | --- |
| Aktive Handbücher ohne Archivabhängigkeiten; Erkennung früherer, normalisierter und URL-kodierter Links sowie Abgrenzung von Beispielen, Kommentaren und externen URLs | **PASS:** 11 Tests einschließlich Unterfällen. |
| Bestehende Tests für Decoder-Registrierung, Namensnormalisierung, gemeinsame Initialisierung und Character-Decoder-Reads | **PASS:** 9 Tests in zwei Paketen; die dokumentierten Schnittstellen bleiben unverändert. |
| `scripts/_310_refresh_trice_manuals.sh check` | **PASS:** neue Anhänge sind verlinkt, bestehende Navigation bleibt konsistent. |
| Erneuter PDF-Build mit den V07-Anhängen | **PASS:** User Manual 9 Seiten, Reference Manual 488 Seiten. Assembler: 935 interne Verweise, 1.286 eindeutige Anker. Im Referenz-PDF lösen alle 977 internen Link-Annotationen zu vorhandenen Seiten auf. RX-Anhang auf Seiten 358–359 und Decoder-Anhang auf Seiten 429–431 sowie User-Manual-Seite 2 gerendert und sichtgeprüft; keine vollständige Sichtprüfung des Buchs. |
| Read-only-CI-Abfrage für `83d7316f` | **PASS:** vollständige Lauf- und relevante Joblisten gelesen; Einzelheiten und Grenzen unter V08. Keine Workflow-Ausführung ausgelöst. |
| Neue relative Verweise, Formatierung und Encoding | **PASS:** 68 hinzugefügte relative Ziele/Fragmente geprüft; `gofmt -l` ohne Ausgabe, `git diff --check` erfolgreich; UTF-8, BOM und CRLF-Zahl erhalten. |
| Gesamtsuiten, Full-Compiler-Matrix und native finale Paketabnahme | **Nicht ausgeführt:** finale Prüfung durch den Benutzer; veröffentlichte CI-Ergebnisse gelten nur für die dort geprüfte Revision. |

Die zusätzlichen fokussierten Go-Befehle:

```sh
rtk go test ./scripts -run '^Test(ActiveManualsHaveNoArchiveDependencies|ArchiveDependencyDetection)$' -count=1 -timeout=60s
rtk go test ./internal/decoder ./internal/charDecoder -run '^Test(NormalizeEncoding|Register.*|NewDecoderDataWithoutBuffers|NewInitializesDecoderData|ReadPassThrough|SetInputSwitchesReader|ReadWithoutInputReturnsEOF)$' -count=1 -timeout=60s
```

Das Archiv ist unverändert. Die Commit-Befehle wurden nur vorgeschlagen; es wurde nichts gestagt, committet, gepusht oder veröffentlicht.
