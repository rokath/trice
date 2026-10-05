# Mit Codex auf einen anderen Rechner wechseln

Für Mac, Linux und Windows mit Git Bash sind die Handgriffe gleich. Alle Befehle unten im Trice-Verzeichnis ausführen. Git, Codex und Python ab 3.11 müssen installiert sein; den passenden Python-Aufruf wählen die Skripte selbst.

## Auf dem bisherigen Rechner

1. In Codex schreiben: **„Rechnerwechsel vorbereiten, committen und pushen.“** Die [Regeln in AGENTS.md](../AGENTS.md#switching-computers-with-codex) legen fest, wie Codex Arbeit, Testergebnisse und den nächsten Schritt sichert. Die Abschlussmeldung abwarten.
2. Codex beenden, auch weitere lokale Codex-Sessions und Codex in App oder IDE. Dann im Terminal:

   ```sh
   ./scripts/codex_handover_export.sh
   ```

3. Die angezeigte ZIP aus `docs/scratchPad` übertragen, beispielsweise per E-Mail. Falls mehrere Sessions infrage kommen, fragt das Skript nach einer Auswahl.

## Auf dem Zielrechner

1. Im passenden Arbeitsbranch den gepushten Stand holen, etwa mit `git pull --ff-only`. Die erhaltene ZIP unverändert unter `docs/scratchPad` ablegen und auch hier andere Codex-Sessions beenden.
2. Im Terminal:

   ```sh
   ./scripts/codex_handover_start.sh
   ```

3. Das Skript prüft und importiert die Session und startet Codex. Dort genügt: **„Rechnerwechsel fortsetzen.“**

Auch beim nächsten Wechsel bleibt dieser Ablauf gleich. Auf demselben Rechner kann das Startskript die bekannte Session ohne neue ZIP fortsetzen. Beide Skripte erklären jeden Schritt und melden konkret, falls noch etwas fehlt; Codex-Version und Zustand müssen nicht von Hand zusammengesucht werden.

---

## Was automatisch geschieht

| Prüfung oder Arbeit | Verhalten |
| --- | --- |
| Python auswählen | macOS/Linux: `python3`, ersatzweise `python`; Git Bash: zuerst `py -3`, dann `python` oder `python3`. Zu alte Interpreter werden übersprungen. Unter Git Bash wird natives Windows-Python verlangt. |
| Git prüfen | Änderungen einschließlich unversionierter Dateien verhindern die Übergabe. Repository, Branch und vollständige Commit-ID werden angezeigt. Beim Import muss der Commit genau zur ZIP passen. |
| Codex prüfen | Laufende lokale Codex-Prozesse einschließlich Hintergrunddienst verhindern den Zugriff. CLI-Version und verwendetes Codex-Profil werden angezeigt. Quelle und Ziel müssen dieselbe CLI-Version verwenden. |
| Anmeldung prüfen | Vor Import oder lokalem Start prüft `codex login status` die Anmeldung im Zielprofil. Kontodetails werden nicht ausgegeben. Es wird keine Modellanfrage ausgeführt. |
| Session auswählen | Die bekannte Projekt-Session wird wiederverwendet; bei mehreren Möglichkeiten wird gefragt. Neue ZIPs werden erkannt, bereits übernommene nicht ständig neu importiert. |
| Inhalt sichern und übernehmen | Prüfsummen, Session-Zuordnung und bisheriger Verlauf werden geprüft. Betroffene Dateien werden gesichert, Eingabe-History und Titel übernommen. Andere Sessions bleiben erhalten. |
| Fortsetzen | Start mit der konkreten Session-ID im Ziel-Checkout. Kein alter Prompt wird erneut gesendet; auch absichtlich wiederholte Eingaben wie „weiter“ bleiben erhalten. |

Die [Export-](../scripts/codex_handover_export.sh) und [Startskripte](../scripts/codex_handover_start.sh) erledigen den Datentransfer lokal. Sie installieren keine Programme und führen weder Commit noch Push, Pull oder Reset aus. Das Sichern und Pushen übernimmt Codex vorher auf den ausdrücklichen Auftrag aus der Kurzanleitung. Vor der ersten Nutzung müssen auch die Skripte selbst auf beiden Rechnern vorhanden und committed sein.

Ein sauberer Checkout allein beweist keinen Push. Nach einem beauftragten Push soll Codex deshalb den tatsächlichen Serverstand prüfen. Die Transferskripte arbeiten ohne Netzwerkzugriff; der genaue Commit-Abgleich beim Import verhindert, dass eine Unterhaltung versehentlich mit einem anderen Dateistand fortgesetzt wird.

## Wenn ein Skript anhält

| Meldung | Nächster Schritt |
| --- | --- |
| Git ist nicht clean | Änderungen prüfen und gezielt sichern/committen lassen. Nicht pauschal verwerfen. |
| Git-Commit passt nicht zur ZIP | Den angegebenen Branch und Commit auf dem Ziel über Git bereitstellen; dann dasselbe Startskript aufrufen. |
| Codex läuft noch lokal | Die genannten Sessions, Apps oder IDE-Anbindungen schließen. Nur wenn alle Arbeiten beendet sind und noch der Dienst läuft: `codex app-server daemon stop`. Danach erneut starten. |
| Python fehlt oder ist zu alt | Python ab 3.11 installieren und das Terminal neu öffnen. Derselbe Shell-Befehl bleibt gültig. |
| Codex-Versionen unterscheiden sich | Die in der Meldung genannte Quellversion auch auf dem Ziel installieren. Kein automatisches Update während des Transfers. |
| Codex-Anmeldung nicht bestätigt | Auf dem Ziel `codex login` ausführen, danach das Startskript wiederholen. Bei einem technischen Statusfehler hilft `codex login status`. |
| Session bereits abgegeben | Die vorhandene Export-ZIP übertragen; auf diesem Rechner erst mit der neuesten Rückgabe-ZIP weiterarbeiten. |
| ZIP ist älter oder Verlauf unabhängig verändert | Die neueste ZIP verwenden. Bei zwei tatsächlich auseinanderentwickelten Verläufen beide aufbewahren und klären lassen. |
| Unvollständige oder nicht unterstützte Session | Die Erläuterung unten beachten; das Skript kopiert keinen nachweislich unvollständigen Verlauf. |

Ein Fehler wird mit einem Fehler-Exitcode gemeldet. Fehlende Voraussetzungen werden vor dem Import geprüft. Laufende Sessions werden nicht automatisch beendet: Die Prozessliste verrät nicht zuverlässig, ob noch Arbeit ungesichert ist.

## Wiederholt zwischen mehreren Rechnern wechseln

Mac → Windows → Debian → Mac ist derselbe Ablauf mit derselben Session-ID. Nach jedem Export gilt die Session lokal als abgegeben. Das Startskript lässt dort erst nach Übernahme einer passenden Rückgabe-ZIP die Fortsetzung zu. Der eigene Export und bereits vor der Abgabe importierte ZIPs heben diese Sperre nicht auf.

Ein eingehender Verlauf muss den bereits bekannten Inhalt unverändert als Anfang enthalten. Dateidatum und Rechneruhr entscheiden nicht darüber, welcher Stand neuer ist. Derselbe unveränderte Import erzeugt keine doppelten History-Einträge. Fehlende Einträge für die Pfeil-hoch-History werden soweit möglich aus den gespeicherten Benutzernachrichten ergänzt.

Auf allen Rechnern das Startskript verwenden. Ein direktes `codex resume` kennt die zusätzliche Abgabe-Sperre nicht. Ohne gemeinsamen Online-Dienst können die Skripte außerdem keine neuere ZIP erkennen, die nur auf einem anderen Rechner liegt. Deshalb immer die zuletzt exportierte ZIP übertragen und die Session jeweils nur auf einem Rechner weiterführen.

## Auswahl und lokale Profile

Die normalen Aufrufe benötigen keine Optionen. Für eine ausdrückliche Auswahl gibt es:

```sh
./scripts/codex_handover_export.sh --session SESSION-UUID
```

```sh
./scripts/codex_handover_start.sh --archive "docs/scratchPad/DATEINAME.zip"
```

```sh
./scripts/codex_handover_start.sh --local
```

Die Platzhalter durch die tatsächlichen Werte ersetzen. Relative ZIP-Pfade beziehen sich auf das Aufrufverzeichnis. `--local` überspringt die ZIP-Auswahl, erhält aber die Abgabe-Sperre. `--help` zeigt die Optionen ohne Profiländerung an.

Ohne `CODEX_HOME` wird das `.codex`-Verzeichnis im Benutzerprofil verwendet. Ein gesetztes `CODEX_HOME` wird auch für Anmeldeprüfung und Resume benutzt. Unter Git Bash gehören Python und Codex zur nativen Windows-Umgebung; WSL hat ein eigenes Linux-Profil. Historische absolute Pfade im Gespräch bleiben unverändert. Der Start erfolgt mit `codex --no-daemon resume SESSION-UUID --cd CHECKOUT`, damit Codex im tatsächlichen Ziel-Checkout arbeitet.

## Inhalt, Sicherungen und Grenzen

Die ZIP enthält den vollständigen JSONL-Verlauf der ausgewählten Session, ihre Eingabe-History, ihren Titel und ein Manifest mit Prüfsummen. Anmeldung, `auth.json`, lokale Konfiguration, Skills, Caches und SQLite-Datenbanken werden nicht kopiert. Einstellungen und Werkzeuge werden auf jedem Rechner separat eingerichtet. Eine vorhandene Anmeldung beweist weder Netzwerkzugang noch verfügbare Credits.

Die ZIP bleibt wie bisher unter `docs/scratchPad`; eine enge `.gitignore`-Regel schließt `codex-handover-*.zip` dort aus. Die Programme selbst liegen unter `scripts`. Die ZIP kann Gesprächsinhalte und Quellcode enthalten und ist nicht verschlüsselt. Beim Versand das Größenlimit des Transportwegs beachten.

Unter `.codex/trice-handover` liegen der lokale Übergabestatus und Sicherungen unter `backups`. Ein Betriebssystem-Lock verhindert gleichzeitig laufende Übergabeskripte. Ein Journal erlaubt beim nächsten Aufruf, eine unterbrochene Übergabe zurückzunehmen, sofern zwischenzeitlich niemand die betroffenen Dateien anderweitig verändert hat. Bei einem solchen Konflikt bleiben Journal und Sicherungen zur Klärung erhalten; sie nicht einfach löschen. Die Prüfungen ersetzen keine globale Sperre gegen direkt gestartete Codex-Prozesse.

Unterstützt werden Sessions mit vollständigem JSONL-Verlauf. Meldet der Codex-Index stattdessen eine datenbankbasierte History, bricht der Export ab. Erkannte lokale beziehungsweise nicht eingebettete Bildanhänge werden ebenfalls abgewiesen. Beliebige im Gespräch erwähnte Dateien werden nicht eingesammelt. Die Grenze beträgt 512 MiB pro gelesener Datei und insgesamt für den entpackten ZIP-Inhalt.

## Arbeitsstand und Übergabe

Der Codex-Verlauf und Git sichern unterschiedliche Dinge: Eine Unterhaltung stellt uncommittierte Quelldateien nicht wieder her. Bei „Rechnerwechsel vorbereiten“ sorgt Codex gemäß `AGENTS.md` für einen nachvollziehbaren Haltepunkt. Bei unfertiger Arbeit hält es Ziel, verbindliche Entscheidungen, erledigte und offene Teile, konkrete Testbefehle samt Ergebnissen und den nächsten Schritt in `docs/scratchPad/Codex_Handover.md` fest. Das ist eine Aufgabenübergabe, keine Voraussetzung für den technischen ZIP-Import.

„Committen und pushen“ im Auftrag erlaubt ausdrücklich diese Git-Schritte. Ohne diese Wörter bleibt es bei Vorbereitung und Statusmeldung. Ein Sicherungscommit darf unfertige Arbeit enthalten; sie gilt dadurch nicht als abgeschlossen. Neue relevante Dateien müssen mitgesichert werden. Lokale Stashes, ignorierte Eingabedaten, Testprotokolle und laufende Prozesse reisen nicht mit einem normalen Push mit.

Bei Trice können Tests vorübergehend Quellen und IDs verändern. Ihre Wiederherstellung muss vor dem Wechsel abgeschlossen sein. Abgebrochene Tests sind nicht bestanden. Auf dem Ziel prüft Codex nach „Rechnerwechsel fortsetzen“ Übergabe, Checkout und benötigte Werkzeuge und arbeitet am dokumentierten nächsten Schritt weiter. Alte Testergebnisse belegen nur den damaligen Stand auf dem damaligen Rechner.

Wenn ein Push nicht möglich ist, können ausdrücklich erstellte lokale Commits separat als Git-Bundle übertragen werden, etwa mit `git bundle create ../trice-handover.bundle wip`. Auch ein Bundle enthält keine uncommittierten oder ignorierten Dateien. Ein Rollout allein erlaubt allenfalls eine nachträglich zu prüfende Rekonstruktion. Bei Git-Worktrees kann eine einfache Ordnerkopie wegen der `.git`-Verweisdatei ebenfalls unvollständig sein.

## Die Skripte testen

Die [Verhaltenstests](../scripts/test_codex_handover.py) verwenden temporäre Repositories und künstliche Profile. Sie prüfen drei aufeinanderfolgende Rechner, Versionskonflikte, aktive Prozesse, Anmeldung, Wiederherstellung und die Shell-Aufrufe einschließlich Git-Bash-Auswahl:

```sh
./scripts/test_codex_handover.sh -v
```

Optional prüft der installierte Codex-Leser den importierten Verlauf in einem isolierten Profil ohne Zugangsdaten. Dabei wird `thread/read` über den App-Server aufgerufen; es gibt keine Modellanfrage:

```sh
TRICE_CODEX_HANDOVER_INTEGRATION=1 ./scripts/test_codex_handover.sh -v -k installed_codex
```

Die simulierten Windows-/Linux-Fälle ersetzen keinen vollständigen praktischen Umzug auf diesen Systemen. Der reguläre TestAll-Lauf startet diese unabhängigen Entwicklerwerkzeug-Tests nicht.
