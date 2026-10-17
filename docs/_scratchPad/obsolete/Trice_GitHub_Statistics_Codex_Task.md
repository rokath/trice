# Codex-Aufgabe: Öffentliche GitHub-Statistik für Trice

**Projekt:** <https://github.com/rokath/trice>  
**Arbeitsbranch:** `wip` (vor Beginn tatsächlichen Branch prüfen)  
**Ziel:** Öffentliches, automatisch aktualisiertes Statistik-Dashboard innerhalb des bestehenden Trice-Repositories und seiner vorhandenen GitHub-Pages-Site.  
**Priorität:** Einfache Integration vorhandener OSS-Komponenten, minimale Wartung und aussagekräftige, methodisch korrekte Diagramme.

## 1. Ausgangslage und Motivation

Trice besitzt Release-Downloads, Git-Clones, Besucher, Stars und Forks auf GitHub. Die bisherigen GitHub-Insights reichen für eine langfristige Auswertung nicht aus: Traffic-Daten für Clones und Views stehen über die GitHub-API nur für die letzten 14 Tage zur Verfügung.

Die sichtbaren Download-Zähler sind keine eindeutigen Nutzerzahlen. Automatisierte CI-Systeme, Skripte und wiederholte Downloads beeinflussen die Werte. Gesucht ist daher ein *transparentes öffentliches Nutzungs- und Interessen-Dashboard*, **keine erfundene Zahl aktiver Nutzer**.

Die Statistik soll über Monate und Jahre zeigen, wie sich das Interesse an Trice verändert, besonders vor und nach Releases (etwa Trice 2.0).

## 2. Verbindliche Anforderungen

1. **Nur das bestehende Repository `rokath/trice` nutzen.** Kein zweites Repository, keine externe Datenbank, kein eigener Server und kein externer Analysedienst.
2. Eine oder möglichst wenige GitHub Actions, bevorzugt **ein Workflow** mit gegebenenfalls mehreren Jobs. Tägliche Ausführung (`schedule`) und manueller Start (`workflow_dispatch`).
3. **Öffentliche, grafische Auswertung über die bestehende GitHub-Pages-Site**, vorzugsweise `/trice/stats/` unter `https://rokath.github.io/trice/`.
4. **Bestehende GitHub-Pages-Dokumentation darf nicht verändert, entfernt oder überschrieben werden.** Insbesondere vorhandene Pages-Deployments und Workflows untersuchen und integrieren, statt ein konkurrierendes Deployment einzurichten.
5. Roh-/Aggregatdaten müssen die 14-Tage-Grenze überleben: langfristig versioniert im **selben Repository**. Ein separater Datenbranch **innerhalb desselben Repositories** ist erlaubt, sofern dies sauber mit GitHub Pages zusammenspielt. Große tägliche Änderungen im Entwicklungsbranch möglichst vermeiden.
6. Vorhandene, bewährte, quelloffene Komponenten bevorzugen; so wenig eigener Code und zusätzliche Abhängigkeiten wie möglich. Lizenzen beachten.
7. Keine personenbezogenen Daten oder Identitäten von Clonern, Besuchern oder Stargazern veröffentlichen. Tokens niemals im Repository, Bericht oder Actions-Log ausgeben.
8. **Stars im Zeitverlauf** sind ausdrücklich ein Kernbestandteil, nicht nur die aktuelle Star-Anzahl.
9. Bereits vorhandene Build-/Release-/CI-Workflows nicht beeinträchtigen. Alle Änderungen zunächst im Arbeitsbranch vorbereiten; nichts ohne ausdrücklichen Auftrag auf `main` mergen oder live veröffentlichen.

## 3. Erst prüfen, dann entscheiden

Vor Implementierung bitte untersuchen und kurz dokumentieren:

- `AGENTS.md` und einschlägige Projektvorgaben.
- Bestehende Workflows unter `.github/workflows/`, insbesondere Pages-Build/Deployment, Trigger, Branch- und Schreibberechtigungen.
- Aktuelle Pages-Konfiguration (`index.md`, `_config.yml`, `docs/`, `gh-pages`-Branch oder Actions-Deployment, soweit vorhanden).
- Welcher Branch Default-Branch ist (`main` wird vermutet); `schedule` läuft nur für Workflow-Versionen im Default-Branch. Test/Entwicklung auf `wip`, Aktivierung nach Merge erklären.
- Ob bereits Statistik- oder Reporting-Komponenten eingebunden sind; Wiederverwendung ist vorzuziehen.
- Prüfung vorhandener OSS-Lösungen bezüglich Wartungsstand, Lizenz, benötigter Rechte und tatsächlicher Funktionsabdeckung.

**Primärer OSS-Kandidat:** <https://github.com/jgehrcke/github-repo-stats> (Apache-2.0).

Diese Action erfasst und visualisiert bereits Views, Clones, Referrer, Stars und Forks; sie speichert Snapshots langfristig in Git. Sie kann Beobachtungs- und Datenrepository identisch verwenden. Bitte **zuerst** prüfen, ob sie in Trice mit wenig Konfiguration einsatzfähig ist. Nicht unnötig nachprogrammieren. Für Release-Downloads ist voraussichtlich eine kleine Ergänzung nötig.

Falls diese Action in der bestehenden Trice-Pages-Pipeline schlecht integrierbar, nicht mehr lauffähig oder für Tokens/Berechtigungen ungeeignet ist, eine mindestens ebenso einfache OSS-Alternative begründet wählen. Keine umfangreiche Neuentwicklung als Standardweg.

## 4. Gewünschte öffentlich sichtbare Diagramme

### 4.1 GitHub Stars – hohe Priorität

- Aktuelle Anzahl der Stars als Kennzahl.
- **Entwicklung der Star-Anzahl über die Zeit**, möglichst seit Projektbeginn, sofern historisch seriös rekonstruierbar.
- Zeitreihe neuer Stars pro Woche oder Monat; optional tägliche Detailansicht.
- Entwicklung der tatsächlich *bestehenden* Stars regelmäßig als Snapshot speichern, sodass spätere Unstars korrekt in der laufenden Bestandszahl sichtbar werden.
- Historische Rückrechnung und ab Start gemessene Bestandswerte unterscheiden und mögliche Lücken oder Abweichungen erläutern.
- Eine Zeitachse, auf der wichtige Releases bei vertretbarem Aufwand markiert werden können.

Für die historische Recherche GitHubs aktuelle REST-API prüfen: `GET /repos/rokath/trice/stargazers/history` sowie gegebenenfalls `GET /repos/rokath/trice/stargazers` mit passendem Accept-Header. Insbesondere die seit 2026 geltenden Zugriffsbeschränkungen beachten. Die historische API bzw. eine Rekonstruktion aus heute vorhandenen Stars ist **nicht automatisch** eine vollständige, historische Netto-Bestandskurve. Keine Genauigkeit suggerieren, die die Daten nicht tragen.

### 4.2 Repository Traffic – hohe Priorität

- Clones pro Tag und Unique Cloners pro Tag.
- Views pro Tag und Unique Visitors pro Tag.
- Linien- oder Säulendiagramme mit gut lesbarer Datumsachse; sinnvoll auch Wochen-/Monatsansicht, **ohne tägliche Unique-Werte als eindeutige Monatsnutzer zu summieren**.
- Die letzten 14 Tage sofort importieren, soweit noch abrufbar; danach täglich weiter archivieren.
- Fehlende Tage, API-Ausfälle oder verspätete Runs als Datenlücke kennzeichnen, niemals mit erfundenen Nullen auffüllen.
- Überlappende 14-Tage-API-Abfragen datumsbasiert **deduplizieren bzw. aktualisieren**, nicht die jeweiligen 14-Tage-Gesamtwerte summieren.

### 4.3 Release-Downloads – hohe Priorität

- Summe aller Downloads der **GitHub-Release-Assets**, inklusive verständlichem Hinweis zur Zählweise.
- Downloads nach Release/Version (z. B. v1.3.0, v2.0.0-rc.1, v2.0.0).
- Downloads nach Asset/Datei; nach Möglichkeit Gruppierung von Plattform und Architektur aus den Dateinamen, mit Fallback `other`.
- Reine Programm-Downloads von Handbüchern, Checksums und anderen Beigaben getrennt zeigen, soweit eine verlässliche Asset-Klassifizierung möglich ist.
- **Zeitentwicklung der kumulierten Download-Zähler** und **Tageszuwachs ab Erfassungsbeginn** darstellen.
- Historisch vor dem ersten Snapshot sind Release-Zähler nur als kumulierte Gesamtwerte verfügbar. **Keine historischen Tagesdownloads aus dem Release-Datum erfinden.**
- Assets robust anhand `asset_id`, `release_id`, Dateiname und Zeitpunkt identifizieren. Austausch, Löschung oder Neuerstellung einer Datei nicht fälschlich als negative Downloads interpretieren; solche Diskontinuitäten kennzeichnen.
- API-Paginierung: alle relevanten Releases/Assets erfassen, nicht nur die erste Ergebnisseite.

Wichtig: `git clone`, `git pull`, `git fetch`, automatisch generierte GitHub-Source-Archive und viele Installationswege außerhalb der Release-Assets sind **nicht** Bestandteil dieser Download-Zähler. Skripte können wiederholt Downloads auslösen.

### 4.4 Forks – mittlere Priorität

- Aktuelle Fork-Anzahl.
- Historische Entwicklung, sofern OSS-Werkzeug/API sie zuverlässig liefern; ansonsten tägliche Bestandssnapshots ab Start.

### 4.5 Referrer und beliebte Inhalte – optional

- Top-Referrer und populäre GitHub-Seiten aus der verfügbaren Traffic-API, bevorzugt als aktuelle 14-Tage-Momentaufnahme.
- Keine vermeintlichen langfristig eindeutigen Nutzerzahlen aus überlappenden 14-Tage-Rankings ableiten.
- Bei zusätzlichem Wartungsaufwand zurückstellen.

## 5. Veröffentlichung und UX

- Eine statische, öffentlich lesbare Statistikseite; **kein Login** und keine clientseitigen API-Tokens.
- Eindeutige Überschriften, Achsen, Einheiten, Legenden und Datenquelle GitHub.
- Auf Desktop und Smartphone brauchbar.
- Im Kopfbereich: aktueller Stand, **Zeitpunkt der letzten erfolgreichen Datenerfassung (UTC)** und ausgewählter Beobachtungszeitraum.
- Zeitreihen mit klarer Unterscheidung zwischen Gesamtsummen, Tageswerten und täglich unterschiedenen Quellen.
- Ein prägnanter Hinweis in Englisch für die öffentliche Leserschaft: "GitHub activity statistics are not a count of unique Trice users. Automated access and repeated downloads are included."
- Trice-Dokumentation und Statistik sollen unter derselben vorhandenen Pages-Site erreichbar bleiben.
- README nur **sparsam** ändern: ein Link `Usage statistics` oder `Project statistics`, nicht mehrere große Diagramme ins README einbauen.
- Möglichst ein bereits von der OSS-Komponente generierter HTML-Bericht; kein neues Frontend-Framework nur für dieses Dashboard.

## 6. Empfohlene technische Architektur (nach Audit anpassen)

```text
rokath/trice                (ein einziges GitHub-Repository)
  .github/workflows/
    <bestehender Pages-Workflow, falls vorhanden>
    statistics.yml          (täglich + workflow_dispatch)
  docs/ ...                 (bestehende Dokumentation: unverändert)
  <Git-Datenbranch oder vergleichbare repo-interne Ablage>
    <archivierte Snapshots und Berichte>
  GitHub Pages
    /trice/                 (bestehende Trice-Dokumentation)
    /trice/stats/           (neues Statistik-Dashboard)
```

Dieses Schema ist **logisch, nicht zwingend die genaue Dateistruktur**. Der Codex soll die vorhandene Pages-Pipeline respektieren. Wenn ein weiterer `deploy-pages`-Workflow das gesamte bestehende Site-Artefakt ersetzen würde, **nicht** verwenden; stattdessen Statistik-Berichte in den bestehenden Build/Deploy einbeziehen oder eine funktional gleichwertige konfliktfreie Lösung umsetzen.

Erstaufbau und tägliche Routine sollen möglichst vollautomatisch laufen. Datensammlung und HTML-Erstellung dürfen in demselben Workflow stattfinden. Ein zweiter Workflow ist nur dann sinnvoll, wenn die bestehende Pages-Veröffentlichung das erfordert.

### Robuste Speicherung

- UTC-Datum als stabile Zuordnung verwenden.
- Idempotent: mehrmaliger manueller Start am selben Tag darf keine doppelten Werte erzeugen.
- Jobs mit `concurrency` gegen gleichzeitige Schreibkonflikte schützen.
- Fehlende API-Werte nicht überschreiben oder künstlich mit null auffüllen.
- Alte Rohdaten nicht bei jeder Ausführung komplett neu schreiben, falls dies das Repository unnötig aufbläht.
- Fehlgeschlagene API-Abfragen als Fehler melden, aber bereits archivierte Daten erhalten.
- Geringer Aufwand für Updates und Reparatur einzelner fehlender Tage.

## 7. Berechtigungen und Sicherheit

Die GitHub-Traffic-API benötigt für ein Fine-grained PAT oder eine passende GitHub-App-Zugriffsberechtigung `Administration: read` auf **nur `rokath/trice`**. Die normale `GITHUB_TOKEN`-Berechtigung reicht dafür unter Umständen nicht aus. Prüfen, welche minimalen Berechtigungen die gewählte Action konkret benötigt, insbesondere wenn sie selbst Daten in einen Branch pusht.

- Falls nötig, ein Fine-grained PAT als GitHub Actions Secret vorsehen; **kein klassisches breites PAT**, sofern vermeidbar.
- Nach Möglichkeit das reguläre `GITHUB_TOKEN` mit `contents: write` ausschließlich für Schreibvorgänge im selben Repository nutzen und das zusätzliche Secret nur zum Lesen geschützter Traffic-Endpunkte einsetzen. Falls die OSS-Action ein einziges Token für beides verlangt, die minimal funktionierenden Rechte explizit dokumentieren.
- Keine Secrets in Ausgaben, HTML, Datenfiles oder Workflow-Artefakten.
- Actions-Versionen auf überprüfte Versionen oder Commit-SHAs pinnen; keine unkontrollierten Downloads aus fremden Skripten.
- Keine veröffentlichten User-Logins/IP-Adressen von Stargazern oder anderen Nutzern, sondern nur aggregierte Zahlen.
- Keine Aktionen mit Write-Rechten auf untrusted `pull_request`-Code.
- Manuellen Schritt zum Anlegen des Secrets in `Settings > Secrets and variables > Actions` exakt benennen. **Nicht nach Tokens im Chat fragen und keine Tokens in Dateien ablegen.**

## 8. Berechnungen und statistische Ehrlichkeit

Bitte vor allem diese Fälle korrekt behandeln und auf der Seite kurz erläutern:

Metrik | Was sie bedeutet | Was sie nicht bedeutet
--- | --- | ---
Release-Downloads | Downloads von GitHub-Release-Assets, auch wiederholt/automatisch | Installationen oder eindeutige Nutzer
Git-Clones | gezählte Clone-Vorgänge | alle Installationen oder aktive Nutzer
Unique Cloners / Visitors | innerhalb des von GitHub gelieferten Zeitfensters unterschiedene Quellen | sicher unterschiedliche Menschen; über Tage addierbare Nutzer
Stars | GitHub-Sterne als Indikator für Interesse | aktive Trice-Nutzer
Forks | GitHub-Forks | produktive Integrationen

Insbesondere **Unique-Werte nicht über Tage kumulieren**. Für Tages- und Wochenansichten sachlich korrekte Bezeichnungen verwenden und unbekannte periodenübergreifende Uniques nicht ausrechnen.

## 9. Tests und Abnahme

Bitte möglichst automatisiert prüfen:

- Datensammlung anhand kleiner Fixtures für Clones, Views, Releases und Stars.
- Wiederholte API-Snapshots und manueller Re-Run erzeugen keine Doppelzählung.
- Ein fehlender Tag wird nicht als `0` dargestellt.
- Download-Snapshot mit altem und neuem Release; neue Assets und ausgetauschte Assets sind korrekt zugeordnet.
- Nichtnegative Zuwächse werden nur für beständige Asset-IDs berechnet; Resets als Bruch markieren.
- API-Paginierung (Releases/Assets/Stars), Rate Limits und 403/404/5xx behandeln.
- Öffentliche Seite lädt mit verständlichen Diagrammen, ohne im Browser ein Secret zu benötigen.
- Pages-Deploy enthält **sowohl** bisherige Trice-Dokumentation **als auch** `/stats/`.
- Vorhandene Workflows bleiben intakt; möglichst keine Auswirkungen auf Trice-Build, Tests oder Releases.
- Formatierung/Linting gemäß Repository-Konventionen.

**Akzeptanzkriterien:** Nach dem Merge auf den Default-Branch und dem einmaligen Einrichten notwendiger Secrets läuft die Statistik täglich ohne Handarbeit. Das Dashboard ist öffentlich. Star-Entwicklung und Clones/Views sind als Zeitreihen sichtbar; Download-Zähler sind nach Version/Asset nachvollziehbar. Die bestehende Dokumentation funktioniert unverändert.

## 10. Gewünschtes Arbeitsergebnis von Codex

1. Kurzbericht zur vorhandenen Trice-Pages-/Actions-Struktur und Bewertung der OSS-Kandidaten; **`github-repo-stats` zuerst prüfen**.
2. Konkrete Integration in `wip`: Workflow(s), minimale Konfiguration bzw. Hilfsskripte und GitHub-Pages-Anbindung.
3. Kleine technische Dokumentation zur Datenerhebung, Grenzen der Aussagekraft, Architektur und zum Wiederanlauf nach einer Unterbrechung.
4. Tests und Nachweis, dass die bestehende Pages-Site nicht überschrieben wird.
5. Genaue Liste der **einmaligen manuellen GitHub-Schritte** (insbesondere Secret, nötige Berechtigungen und ggf. GitHub-Pages-Konfiguration).
6. Links zu benutzten Drittprojekten, Lizenz und festgelegten Versionen.
7. Ein kurzes Änderungsprotokoll: bearbeitete/neu angelegte Dateien, getestete Funktionen und offene Punkte.

**Arbeitsmodus:** Codeänderungen lokal vornehmen und soweit möglich testen. Bei unklaren GitHub-Zugriffsrechten oder Pages-Deployment-Einstellungen zuerst das Problem benennen; keinen riskanten Live-Deploy oder Merge versuchen. Keine unnötige zweite Repository-Struktur aufbauen. Code und Kommentare möglichst englisch; Berichtsseite ebenfalls englisch.

## 11. Nützliche Primärquellen

- OSS-Grundlage: <https://github.com/jgehrcke/github-repo-stats>
- GitHub Traffic-API: <https://docs.github.com/en/rest/metrics/traffic>
- GitHub Stars einschließlich History-Endpoint: <https://docs.github.com/en/rest/activity/starring>
- GitHub Release-API: <https://docs.github.com/en/rest/releases/releases>
- GitHub Release-Assets: <https://docs.github.com/en/rest/releases/assets>
- GitHub Pages mit Actions: <https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages>
- GitHub Actions `schedule`-Verhalten: <https://docs.github.com/en/actions/reference/workflows-and-actions/events-that-trigger-workflows>

**Leitgedanke:** Lieber eine gut verständliche, dauerhaft zuverlässige Statistik mit wenigen aussagekräftigen Kurven als ein großes, wartungsintensives Analytics-System. Die Star-Historie ist ausdrücklich wichtig.
