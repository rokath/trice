# Mit einer laufenden Codex-Aufgabe auf einen anderen Rechner wechseln

Vor dem Wechsel müssen **Arbeitsdateien, Git-Stand und Übergabeinformationen** gesichert sein. Am zuverlässigsten sind ein dokumentierter Zwischenstand auf einem gepushten Arbeitsbranch und eine kurze Übergabe im Repository. Eine noch unvollständige Aufgabe darf einen Sicherungscommit bekommen; dadurch gilt sie nicht als fertig.

Ein Codex-Verlauf und ein Git-Repository sichern unterschiedliche Dinge. Codex setzt eine gespeicherte Unterhaltung fort, liest die Dateien aber aus dem aktuellen Arbeitsverzeichnis. Eine kopierte Rollout-Datei stellt uncommittierte Quelldateien nicht automatisch wieder her. Die offizielle Dokumentation beschreibt diese Trennung unter [Projects and chats](https://learn.chatgpt.com/docs/projects#work-in-a-project-directory).

## Der empfohlene Ablauf

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
