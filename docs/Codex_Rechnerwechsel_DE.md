# Mit einer laufenden Codex-Aufgabe auf einen anderen Rechner wechseln

Vor dem Wechsel müssen **Arbeitsdateien, Git-Stand und Übergabeinformationen** gesichert sein. Am zuverlässigsten sind ein dokumentierter Zwischenstand auf einem gepushten Arbeitsbranch und eine kurze Übergabe im Repository. Eine noch unvollständige Aufgabe darf einen Sicherungscommit bekommen; dadurch gilt sie nicht als fertig.

Ein Codex-Verlauf und ein Git-Repository sichern unterschiedliche Dinge. Codex setzt eine gespeicherte Unterhaltung fort, liest die Dateien aber aus dem aktuellen Arbeitsverzeichnis. Eine kopierte Rollout-Datei stellt uncommittierte Quelldateien nicht automatisch wieder her. Die offizielle Dokumentation beschreibt diese Trennung unter [Projects and chats](https://learn.chatgpt.com/docs/projects#work-in-a-project-directory).

## Die Skripte für den regelmäßigen Wechsel

Für den Wechsel derselben Session zwischen Mac, Windows mit Git Bash und Debian gibt es zwei Skripte:

| Skript | Aufgabe |
| --- | --- |
| [codex_handover_export.py](scratchPad/codex_handover_export.py) | Prüft den Ausgangsrechner, erstellt eine ZIP unter `docs/scratchPad` und markiert die Session lokal als abgegeben. |
| [codex_handover_start.py](scratchPad/codex_handover_start.py) | Prüft den Zielrechner, übernimmt die ZIP und startet die darin enthaltene Session im aktuellen Checkout. Ohne neue ZIP setzt es die bekannte lokale Session fort. |

Beide melden ihre einzelnen Schritte und brechen mit einer Begründung ab, sobald eine Voraussetzung fehlt. Sie benötigen Python ab 3.11, Git und eine lokal installierte und angemeldete Codex CLI. Zusätzliche Python-Pakete sind nicht erforderlich. Auf Ausgangs- und Zielrechner muss dieselbe Codex-CLI-Version installiert sein; bei unterschiedlichen Versionen wird der Import abgewiesen.

**Vor der ersten Benutzung müssen auch diese Skripte und ihre `.gitignore`-Regel committed und auf den anderen Rechner übertragen sein.** Beide Skripte verlangen einen sauberen Checkout, einschließlich bisher unversionierter Dateien. Ein sauberer Checkout beweist jedoch keinen Push. Den Git-Stand wie im folgenden manuellen Ablauf beschrieben sichern und auf dem Ziel denselben Commit bereitstellen. Die Skripte führen selbst weder Commit noch Push, Pull oder Reset aus.

### Auf dem Ausgangsrechner

Die Arbeit sichern und Codex vollständig beenden. Das betrifft auch andere lokale Codex-Sessions, die Codex-/ChatGPT-App, Codex in einer IDE und den Hintergrunddienst. Auch ein scheinbar untätiger Dienst wird vorsichtshalber als möglicher Schreiber behandelt. Das Skript nennt gefundene Prozess-IDs, beendet aber keinen Prozess. Wenn alle Arbeiten beendet sind und nur noch der Codex-Dienst läuft, kann er ausdrücklich beendet werden:

```sh
codex app-server daemon stop
```

Danach im Repository unter macOS oder Debian:

```sh
python3 docs/scratchPad/codex_handover_export.py
```

Unter Windows in Git Bash mit installiertem Python-Launcher:

```sh
py -3 docs/scratchPad/codex_handover_export.py
```

Ist Python dort als `python` statt über `py` verfügbar, entsprechend `python` verwenden. Gemeint ist unter Git Bash die native Windows-Installation; WSL hat ein eigenes Linux-Profil. Ohne abweichendes `CODEX_HOME` verwendet das Skript das `.codex`-Verzeichnis im Benutzerprofil des verwendeten Python. Ein gesetztes `CODEX_HOME` muss zu diesem Interpreter und zur verwendeten Codex-Installation passen. Siehe [Codex unter Windows](https://learn.chatgpt.com/docs/windows/windows-app) und [Umgebungsvariablen](https://learn.chatgpt.com/docs/config-file/environment-variables).

Beim ersten Export werden passende Projekt-Sessions mit Titel, Änderungszeit und UUID zur Auswahl angezeigt, wenn mehrere infrage kommen. Danach merken sich die Skripte die ausgewählte Session. Bei Bedarf lässt sie sich ausdrücklich wählen:

```sh
python3 docs/scratchPad/codex_handover_export.py --session SESSION-UUID
```

`SESSION-UUID` durch die tatsächliche UUID ersetzen. Das Skript zeigt den vollständigen Namen und die Größe der fertigen `codex-handover-….zip`. Genau diese ZIP übertragen, beispielsweise per E-Mail. Das Archiv wird durch eine enge `.gitignore`-Regel ignoriert und verhindert deshalb den nächsten sauberen Git-Status nicht. Die Quelldatei der Unterhaltung bleibt unverändert.

### Auf dem Zielrechner

Denselben Repository-Commit bereitstellen, alle lokalen Codex-Prozesse beenden und die erhaltene ZIP mit unverändertem Namen unter `docs/scratchPad` ablegen. Dann unter macOS oder Debian:

```sh
python3 docs/scratchPad/codex_handover_start.py
```

Unter Windows in Git Bash:

```sh
py -3 docs/scratchPad/codex_handover_start.py
```

Das Skript prüft Projekt, Commit, Codex-Version, Session-Zuordnung und Prüfsummen. Es sichert betroffene lokale Dateien, ergänzt die Eingabe-History und übernimmt ausschließlich die ausgewählte Session. Andere Sessions bleiben erhalten. Anschließend startet es `codex --no-daemon resume SESSION-UUID --cd CHECKOUT`; das Arbeitsverzeichnis stammt vom Zielrechner, historische Pfade im Gespräch werden nicht umgeschrieben. Es sendet keinen alten Prompt erneut. Zum Resume-Aufruf siehe [CLI-Befehle](https://learn.chatgpt.com/docs/developer-commands).

Mehrere neue ZIPs erfordern eine Auswahl. Eine ZIP lässt sich auch ausdrücklich mit `--archive docs/scratchPad/DATEINAME.zip` angeben. Bei einem weiteren Start auf demselben Rechner genügt wieder das Startskript: Bereits übernommene ZIPs werden nicht erneut automatisch importiert. `--local` verlangt ausdrücklich die lokale Fortsetzung und überspringt die ZIP-Auswahl; die Abgabe-Sperre bleibt dabei wirksam.

### Wiederholt zwischen den Rechnern wechseln

Der Ablauf bleibt immer gleich: **Arbeit sichern → Codex beenden → exportieren → ZIP übertragen → auf dem Ziel starten.** Das funktioniert auch als Mac → Windows → Debian → Mac, ohne eine neue Session-ID anzulegen.

- Nach dem Export verweigert das Startskript auf dem Ausgangsrechner die lokale Fortsetzung, bis eine passende Rückgabe-ZIP übernommen wurde. Der eigene Export oder eine bereits vor der Abgabe importierte ZIP hebt diese Sperre nicht auf.
- Die Fortsetzung muss den bereits bekannten Gesprächsinhalt unverändert als Anfang enthalten. Ältere oder unabhängig weitergeführte Fassungen werden abgewiesen; Dateidatum und Rechneruhr entscheiden darüber nicht.
- Ein erneuter Import desselben unveränderten Standes erzeugt keine zusätzlichen History-Einträge. Bewusst mehrfach eingegebenes „weiter“ bleibt dagegen mehrfach erhalten.
- Fehlende Eingaben in `history.jsonl`, etwa nach einer früheren Übertragung nur der Rollout-Datei, werden aus den gespeicherten Benutzer-Nachrichten ergänzt. Das hilft auch bei der Pfeil-hoch-History im Codex-Eingabefeld.

**Auf allen beteiligten Rechnern das Startskript verwenden.** Ein direktes `codex resume` kennt die zusätzliche Abgabe-Sperre nicht. Ohne gemeinsamen Online-Dienst können die Skripte außerdem nicht erkennen, ob eine noch nie gesehene neuere ZIP auf einem anderen Rechner liegt oder jemand die Session dort parallel fortführt. Besonders bei der ersten Übernahme auf einem Rechner deshalb immer die zuletzt exportierte ZIP verwenden. Die Sperren sichern den vorgesehenen Ablauf ab; sie sind keine globale Zugriffskontrolle.

### Sicherungen, Abbrüche und Grenzen

Im jeweiligen Codex-Profil liegt der Übergabestatus unter `trice-handover`. Vor Änderungen entstehen dort Sicherungen unter `backups`. Ein Betriebssystem-Lock verhindert gleichzeitig laufende Übergabeskripte und wird bei einem Prozessabbruch automatisch freigegeben. Zusätzlich prüfen die Skripte wiederholt auf Codex-Prozesse und inzwischen veränderte Dateien.

Bleibt nach einem Abbruch `pending.json` zurück, versucht der nächste Aufruf nach den Git- und Prozessprüfungen, die unvollständige Übergabe zurückzunehmen. Dabei werden nur Dateien zurückgesetzt, deren Inhalt noch zur begonnenen Übergabe passt. Bei zwischenzeitlichen fremden Änderungen bricht die Wiederherstellung ab und behält Journal und Sicherungen zur Klärung. Diese Dateien nicht einfach löschen, um die Prüfung zu umgehen. Kein Skript-Lock kann ein direkt gestartetes Codex am Schreiben hindern; während einer Übergabe deshalb keinen Codex-Prozess starten.

Übertragen werden der vollständige JSONL-Verlauf der ausgewählten Session, ihre Eingabe-History, ihr Sessiontitel und ein Manifest mit Prüfsummen. **Anmeldung, `auth.json`, lokale Konfiguration, Skills, Caches und SQLite-Datenbanken werden nicht kopiert.** Benötigte lokale Einstellungen und Werkzeuge auf jedem Rechner separat einrichten. Die ZIP kann trotzdem sensible Inhalte aus dem Gespräch und Quellcode enthalten. Sie ist nicht verschlüsselt; beim Versand auch das Größenlimit des gewählten Transportwegs beachten.

Die Skripte unterstützen Sessions, deren vollständige Unterhaltung in einer JSONL-Rollout-Datei liegt. Zeigt der lokale Codex-Index stattdessen eine datenbankbasierte History, brechen sie ab, um keinen veralteten oder unvollständigen Verlauf zu übertragen. Ebenso werden erkannte lokale beziehungsweise nicht eingebettete Bildanhänge abgewiesen. Beliebige Dateien, auf die im Gespräch nur verwiesen wird, werden nicht eingesammelt: benötigte Projektdateien gehören in Git oder in eine separate Übergabe. Die Größenbegrenzung beträgt 512 MiB pro gelesener Datei und insgesamt für den entpackten ZIP-Inhalt.

Das Lesen eines so importierten JSONL-Verlaufs ohne kopierte Codex-Datenbank wurde mit der installierten Codex CLI 0.159.3 geprüft. Die automatischen Tests simulieren die drei Rechner mit getrennten Profilen und prüfen auch Windows-Prozesserkennung, Konflikte und Wiederherstellung. Ein tatsächlicher Lauf auf Windows und Debian ist damit noch nicht nachgewiesen.

### Die Skripte gezielt testen

Die [beschreibenden Tests](../scripts/test_codex_handover.py) arbeiten ausschließlich mit temporären Git-Repositories und künstlichen Codex-Profilen:

```sh
python3 -B -m unittest discover -s scripts -p test_codex_handover.py -v
```

Optional lässt sich zusätzlich der echte Codex-Leser in einem isolierten Profil ohne Zugangsdaten prüfen. Dieser Test ruft `thread/read` über den [App-Server](https://learn.chatgpt.com/docs/app-server) auf, startet keine Modellanfrage und verändert kein persönliches Codex-Profil:

```sh
TRICE_CODEX_HANDOVER_INTEGRATION=1 python3 -B -m unittest discover -s scripts -p test_codex_handover.py -k installed_codex -v
```

Unter Windows in Git Bash `python3` in diesen Befehlen durch `py -3` ersetzen.

## Git-Sicherung und manuelle Übergabe im Detail

### Auf dem alten Rechner einen sicheren Haltepunkt herstellen

Diese Nachricht rechtzeitig an Codex senden:

> Ich wechsle jetzt den Rechner. Beginne keine weitere Implementierung und keinen neuen langen Test. Lass laufende Änderungen und notwendige Aufräumarbeiten kontrolliert abschließen. Beende oder unterbrich Tests so, dass ihr Ausgangszustand wiederhergestellt ist. Halte Ziel, Entscheidungen, aktuellen Stand, offene Punkte und den nächsten konkreten Schritt in einer Übergabe unter docs/scratchPad fest. Dokumentiere bestandene, fehlgeschlagene, übersprungene und abgebrochene Tests getrennt. Committe alle zu dieser Aufgabe gehörenden Änderungen einschließlich neuer Dateien in sinnvollen Gruppen und pushe den Arbeitsbranch. Kennzeichne unfertige Teile ehrlich als Zwischenstand. Prüfe danach den Remote-Commit und nenne mir Branch, Commit-ID und alle noch ausschließlich lokalen Dateien. Danach halte an.

Diese Formulierung erteilt ausdrücklich den Commit- und Push-Auftrag. Ein bloßes „weiter“, „umsetzen“ oder „ich wechsle den Rechner“ tut das nach den [Repository-Regeln](../AGENTS.md) nicht.

Bei Trice können Tests vorübergehend Quellen, IDs und generierte Dateien ändern. Deshalb erst die Wiederherstellung abwarten und dann sichern. Ein abgebrochener Test ist nicht bestanden. Einen Rechner nicht herunterfahren, während noch Quellen umgeschrieben oder wiederhergestellt werden.

### Prüfen, was tatsächlich übertragen wird

Im tatsächlich verwendeten Repository beziehungsweise Worktree ausführen, jeweils einen Befehl kopieren:

```sh
git status --short --untracked-files=all
```

```sh
git branch --show-current
```

```sh
git rev-parse HEAD
```

Für den hier häufig verwendeten Branch `wip` den Serverstand direkt prüfen:

```sh
git ls-remote origin refs/heads/wip
```

Bei einem anderen Arbeitsbranch `wip` im letzten Befehl entsprechend ersetzen. Die Commit-ID auf dem Server muss mit der lokalen `HEAD`-ID übereinstimmen. Ein leerer `git status` allein beweist keinen Push; auch die lokale Anzeige `origin/wip` kann veraltet sein.

| Bestand | Was dafür nötig ist |
| --- | --- |
| Bereits versionierte Änderungen und Löschungen | In den passenden Commit aufnehmen und diesen pushen. |
| Neue Dateien, im Status mit `??` | Gezielt aufnehmen; ein gewöhnlicher `git diff` enthält sie nicht. |
| Ignorierte lokale Dateien, etwa benötigte Testprotokolle oder Eingabedaten | Bei Bedarf separat übertragen und in der Übergabe nennen. |
| Lokale Commits oder Stashes | Lokale Commits pushen; ein Stash wird durch gewöhnliches Push/Pull nicht übertragen. |
| Codex-Unterhaltung/Rollout | Zusätzlich sichern, falls derselbe Verlauf weiterverwendet werden soll. |
| Laufende Prozesse, offene Ports und Test-Sessions | Werden nicht durch Git oder einen kopierten Verlauf fortgesetzt; auf dem neuen Rechner bei Bedarf neu starten. |

Nicht pauschal sämtliche lokalen Dateien committen. Insbesondere lokale Einstellungen, Zugangsdaten, Build-Caches und generierte Testreste gezielt behandeln. Für die Aufgabe notwendige neue Tests gehören dagegen ausdrücklich in die Sicherung.

### Auf dem neuen Rechner zuerst den Dateistand prüfen

Den passenden Checkout öffnen. Vor einem Pull zunächst kontrollieren, ob dort eigene Änderungen liegen:

```sh
git status --short --untracked-files=all
```

Ist der Checkout sauber, für das Beispiel `wip` einzeln ausführen:

```sh
git fetch origin
```

```sh
git switch wip
```

```sh
git pull --ff-only
```

```sh
git rev-parse HEAD
```

Die ID mit der Übergabe vergleichen und exemplarisch prüfen, dass die neuen Dateien vorhanden sind. Bei lokalen Änderungen, einem abweichenden Branch oder einem fehlgeschlagenen Fast-forward erst die Ursache klären; nicht mit Reset oder Clean darübergehen.

Danach Codex im neuen Projektverzeichnis öffnen und die Übergabe lesen lassen:

> Wir arbeiten jetzt auf dem neuen Rechner. Lies die Übergabe unter docs/scratchPad und die gültigen Repository-Regeln. Prüfe Branch, Commit-ID und vorhandene Dateien gegen den übergebenen Stand. Übernimm die bisherigen Entscheidungen. Prüfe die benötigten Werkzeuge und führe offene oder abgebrochene Tests hier erneut aus. Setze anschließend beim dokumentierten nächsten Schritt fort. Commit und Push erfolgen weiterhin nur auf meinen ausdrücklichen Auftrag.

Die Codex CLI kann gespeicherte Unterhaltungen mit `codex resume` öffnen. Wenn der übertragene Verlauf hier nicht verfügbar ist, reicht für die fachliche Fortsetzung auch eine neue Unterhaltung mit dem richtigen Checkout und einer vollständigen Übergabe. Die Verfügbarkeit der Unterhaltung ersetzt die Prüfung des Arbeitsverzeichnisses nicht. Siehe [Codex CLI](https://learn.chatgpt.com/docs/codex/cli).

Ein anderer Benutzername oder ein anderer Projektpfad ist kein Grund, alte absolute Pfade in Konfigurationen zu übernehmen. Go, C-Compiler und weitere Testwerkzeuge auf dem neuen Rechner prüfen. Auch Codex-Anmeldung, benötigte Skills, lokale Einstellungen und Zugriffsrechte prüfen; die Übergabe sollte benötigte Besonderheiten nennen, aber keine Zugangsdaten enthalten. Alte Testergebnisse bleiben Nachweise für den damaligen Stand auf dem alten Rechner; sie beweisen keine erfolgreiche Ausführung in der neuen Umgebung.

## Was in die Übergabe gehört

- Ziel, vereinbarter Umfang, getroffene Entscheidungen und ausdrücklich ausgeschlossene Arbeiten.
- Arbeitsbranch und Ausgangscommit; die endgültige gepushte Commit-ID zusätzlich in der abschließenden Chat-Antwort festhalten.
- Fertige, unfertige und noch ungeprüfte Änderungen, einschließlich neuer Dateien.
- Exakte relevante Testbefehle, Werkzeugversionen und Ergebnisse; Abbrüche, Fehler und übersprungene Tests ausdrücklich nennen.
- Benötigte lokale Daten und Protokolle mit relativem Pfad und Übertragungsort; nur auf dem alten Rechner vorhandene Dateien kennzeichnen.
- Verbleibende Schritte, beginnend mit einer konkreten nächsten Aktion.

Die Übergabe selbst kann ihre eigene endgültige Commit-ID nicht zuverlässig enthalten, weil jede Änderung an ihr wieder einen neuen Commit erzeugt. Deshalb den Ausgangscommit in der Datei und die abschließende ID nach Commit und Push im Chat beziehungsweise separat notieren.

## Wenn ein Push gerade nicht möglich ist

Nach dem kontrollierten Anhalten einen lokalen Sicherungscommit auf einem Arbeitsbranch erstellen lassen und diesen Branch als Git-Bundle übertragen. Beispiel aus dem Repository für `wip`:

```sh
git bundle create ../trice-handover.bundle wip
```

Das Bundle und benötigte lokale Daten auf den anderen Rechner kopieren. Dort kann aus dem Bundle in ein neues Verzeichnis geklont werden:

```sh
git clone -b wip trice-handover.bundle trice-handover
```

Das neue Repository verwendet zunächst das Bundle als Remote-Quelle. Vor einem späteren Push die gewünschte Server-URL wieder konfigurieren. Auch ein Bundle enthält nur die aufgenommenen Git-Commits, keine uncommittierten, neuen oder ignorierten Arbeitsdateien.

Falls auch kein Sicherungscommit gewünscht ist, das angehaltene Arbeitsverzeichnis einschließlich versteckter Dateien separat vollständig sichern. Bei Git-Worktrees kann `.git` nur eine Verweisdatei auf das Hauptrepository sein; eine bloße Ordnerkopie ist dann kein eigenständiges Git-Backup. Ein Commit plus Bundle ist hierfür leichter überprüfbar.

## Wenn nur noch ein Rollout vorhanden ist

Ein Rollout kann konkrete Änderungspatches und Testergebnisse enthalten. Daraus lässt sich ein Arbeitsstand unter Umständen rekonstruieren. Das ist eine Wiederherstellung mit anschließender Prüfung, kein garantierter Dateiexport: fehlgeschlagene Patches, spätere Korrekturen, Formatter und nicht protokollierte manuelle Änderungen müssen berücksichtigt werden. Alte Werkzeugaufrufe nicht blind erneut ausführen.

Den alten Checkout oder seine Sicherung erst löschen, wenn der neue Rechner die benötigten Dateien nachweislich besitzt und die Fortsetzung geprüft ist.
