# Implementierungsplan für M19 und M20

Stand: 24. September 2026. **Nur Planung; Umsetzung erst auf ausdrückliches Kommando.** Maßgeblich sind [Strukturiertes Logging](Strukturiertes_Logging_DE.md) und [Context Enrichment](Kontextanreicherung_DE.md). Die alten M19/M20-Issues sind durch diesen Plan abgelöst.

## 1. Einschätzung der Implementierungsreife

Die für den aktuellen Scope nötigen Produkt- und Designentscheidungen sind abgeschlossen. Eine weitere Auswahl zwischen Logging-Modellen, Exportvarianten oder CE-Semantiken ist vor Implementierungsbeginn nicht erforderlich.

Offen bleiben technische Implementierungsdetails, konkrete interne Datenstrukturen, Generatorpfade und Tests. Diese sollen innerhalb des festgelegten Vertrags ohne neue Freigaberunde entschieden werden, solange sie kein beobachtbares Verhalten ändern.

M01–M16 und die Farberweiterung für `-ulabel` bilden die vorhandene Grundlage. M17 bleibt zurückgestellt; M18 wird Abnahmearbeit innerhalb M19/M20.

Vor Codeänderungen ist weiterhin ein nachvollziehbarer Test-Ausgangsstand erforderlich: In [insertIDs_test.go](../../internal/id/insertIDs_test.go) erwarten `TestInsertExistingID_A/B` nach bisherigem Dokumentationsstand noch, dass ID 77 bei Bereich 100..999 unverändert bleibt. Das widerspricht der mit M15 eingeführten Neuzuweisung. Diese bekannte Testaltlast ist separat abzugleichen.

## 2. Festgelegter M19-Vertrag

| Bereich | Vertrag |
|---|---|
| Aufruf | Normales `trice()`; keine neue primäre Target-API. |
| Platzhalter | `{name}`, `{}`, `{prefix.}` und Darstellung nach dem ersten Doppelpunkt; `%...` und `{...}` konsumieren Argumente von links nach rechts. |
| Literale Klammern | `{{` und `}}`; keine neue `\{`-/`\}`-Semantik. |
| Namensableitung | Bezeichner und reine Memberketten; `.`/`->` werden zu Punktnamen, `aFloat`/`aDouble` sind transparente Hüllen. Andere Ausdrücke brauchen explizite Namen. |
| Eindeutigkeit | Kanonische Feldnamen müssen pro Record eindeutig sein; Duplikate sind Fehler. |
| Typmodell | Keine allgemeine C-Typinferenz. Typ aus Trice-Bitbreite, Formatspezifizierer und Wrapper. |
| Typen | `%d/%i` signed integer; `%u/%o/%O/%x/%X/%b` unsigned integer; Float-Spezifizierer floating point; `%c/%q` character; `%t` boolean; `%s` string; `%p` address; `%%` ignoriert. |
| TIL | Bestehendes `til.json`-Schema bleibt unverändert. `Strg` speichert den vollständig kanonisierten strukturierten Template-String. |
| Schemaidentität | `Type + Strg`; Feldumbenennung erzeugt neues Schema/neue ID. |
| Text | `-logFormat text` bleibt klassische formatierte Ausgabe ohne zusätzliche Structured-Felder. |
| JSON | User-Felder unter optionalem `fields`; 64-Bit-Integer als JSON-Zahlen; nicht-endliche Floats werden als Felder weggelassen; leeres `fields` wird weggelassen. |
| KV | User-Felder als `field.<name>`; String/Character/Message quoted, Zahlen/Boolean/Address unquoted; definierte Escapes für Quote, Backslash, LF, CR, Tab. |
| Metadaten | Bestehende Optionen steuern `id`, `file`, `line`, `ts`, `hs`; `ts` raw unsigned, `hs` formatierter Host-Zeitstring. |
| Tag | Immer vorhanden, bei fehlender Klassifikation `untagged`; tatsächliche verwendete Schreibweise wird erhalten. |
| Level | Kanonischer Levelwert aus case-neutraler Alias->Canonical-Tabelle; Default mit gängigen Log-Level-Namen; später per CLI modifizierbar. |
| Reihenfolge | User-Felder deterministisch in Quell-/Argumentreihenfolge in JSON und KV. |
| Registry | `trice-fields.txt` wird pro aktuellem Build-/Instrumentierungslauf im Build-Verzeichnis neu erzeugt; nur User-/CE-Felder. |
| Registry-Format | `%8d %s\n`, Count aufsteigend, bei Gleichstand Feldname alphabetisch; keine Namenslängenbegrenzung. |
| Migration | Alte literal gemeinte `{`/`}` in Source und bestehenden TIL-Beständen werden einmalig zu `{{`/`}}` migriert; kein Legacy-Modus. |
| Scope | M19 Template-Unterstützung für `bind` sowie `insert/clean`. |

Die globale Formatspezifizierer->Typ-Zuordnung wird zentral im Trice-Code verankert und nicht redundant pro TIL-Eintrag gespeichert.

## 3. Festgelegter M20-Vertrag

| Bereich | Vertrag |
|---|---|
| Scope | Aktuell ausschließlich `trice bind -ce`. |
| Syntax | `-ce 'selector:"format-extension"[, <C-expression>]...'` |
| C-Ausdrücke | Bewusst einfach; jeder CE-Ausdruck muss kommafrei sein. |
| Unkonfigurierter Präfix | Ohne passende `-ce`-Regel bleibt der gesamte Präfix normaler Logtext. |
| Case | Selektoren werden case-neutral verglichen; nur vollständig kleingeschriebene aktive Selektoren werden aus dem sichtbaren Text entfernt. |
| Aliase | Eingebaute Trice-Tags verwenden ihre feste Alias-Gruppe; freie Selektoren nur case-neutral. |
| `-ulabel` | Orthogonal; derselbe Name darf gleichzeitig Label/Tag und CE-Selektor sein. |
| Mehrere Selektoren | Verschiedene Selektoren in Source-Reihenfolge. |
| Mehrere Regeln | Mehrere `-ce`-Regeln für denselben Selektor in CLI-Reihenfolge. |
| Doppelte Selektoren | Derselbe Selektor mehrfach an einer Logstelle wirkt einmal; Warnung. |
| Transformation | CE erfolgt vor M19-Kanonisierung, Schema- und ID-Bestimmung. |
| TIL | Der finale CE-erweiterte kanonische Template-String wird im bestehenden `Strg` gespeichert. |
| Feldkonflikte | CE und Source bilden ein gemeinsames Feldschema; doppelte kanonische Feldnamen sind Fehler. |
| Auswertung | Jeder injizierte Runtime-Ausdruck wird pro Trice-Aufruf genau einmal ausgewertet. |
| Fehler | Argumentzahl-, Arity-, Bitbreiten-/Wrapper- und syntaktische CE-Fehler müssen vor inkonsistentem Schreiben scheitern. |
| Idempotenz | Gleicher Source + gleiche CE-Konfiguration -> dasselbe kanonische Ergebnis und dieselbe ID-Zuordnung. |
| Insert/Clean später | Möglich, aber nicht aktueller Scope; nur unter streng reversiblem/idempotentem Vertrag mit identischen CE-Optionen für `insert` und `clean`. |

## 4. Vorgesehene Implementierungsreihenfolge

| Schritt | Konkretes Ergebnis | Abnahme |
|---|---|---|
| Ausgangsstand | Bekannte M15-Testaltlast geklärt; grüne relevante Baseline oder exakt abgegrenzte bekannte Fehler | Keine unbelegten Erfolgsangaben aus alten Checklisten übernehmen |
| M19 Formatparser | Gemeinsamer Parser für `%...`, `{...}`, `{{`/`}}`, Darstellung und Namensableitung | Erfolgs- und Fehlerfälle für gemischte Platzhalter, unbalancierte Klammern, Namensableitung und Argumentzahl |
| M19 Kanonisierung/TIL | Source-Kurzformen in kanonisches `Strg`; bestehendes TIL-Schema unverändert | Feldumbenennung -> neue ID; syntaktisch äquivalente Memberkette -> gleiche Schemaidentität |
| M19 Bind/Insert/Clean | Template-Unterstützung in allen drei M19-Pfaden | Wiederholte Läufe idempotent; `clean(insert(S)) = S` für M19; historische Definitionen bleiben dekodierbar |
| M19 Decoder-Record | Typisierte Werte vor Textformatierung; feste Metadaten | Typgrenzen, `%p` als address, Character/String/Boolean, 8/16/32/64 Bit, Float/Double |
| M19 JSON | Vertrag exakt umgesetzt | 64-Bit-Grenzwerte, NaN/Inf-Auslassung, optionale `fields`, Tag/Level, ts/hs, deterministische Reihenfolge |
| M19 KV | Präfix- und Quotingvertrag exakt umgesetzt | Spaces, Quotes, Backslash, LF/CR/Tab, User-Felder namens `tag`/`message`, stabile Reihenfolge |
| M19 Registry | `trice-fields.txt` im Build-Verzeichnis neu erzeugen | Count-Sortierung, Tie-Break alphabetisch, CE-/User-Felder, Teil-/Buildlauf-Scope eindeutig |
| Migration | Einmaliger Pfad für bestehende literal gemeinte Klammern in Source/TIL | Kein Legacy-Parser; migrierte Bestände ergeben gleiche alte Textausgabe |
| M20 CE-Parser | `-ce`-Syntax mit kommafreien Ausdrücken und deterministischer Regelauswahl | Case/Aliase, unbekannte Präfixe, Mehrfachregeln, doppelte Selektoren |
| M20 Bind-Erzeugung | Zusätzliche Runtime-Argumente im lokalen Scope ohne Sourceänderung | Kompilierte C-Integration, einmalige Auswertung, `TRICE_OFF`, Arity/Bitbreite, Wrapper |
| M20 Lebenszyklus | CE-Konfiguration reproduzierbar in den finalen Artefakten | CE an/aus/geändert, Rückkehr zur alten Konfiguration, verschobene/duplizierte Sites, wiederholter identischer Lauf |
| Gemeinsamer Abschluss | Beispiele gegen reale CLI validiert; spätere englische UM-Übernahme | Passende Go- und C-Tests; Coverage-Prüfung vor Push; keine Implementierung nur zur Bedienung eines Doku-Beispiels |

## 5. Technische Anforderungen ohne neue Produktentscheidung

Die Implementierung darf folgende Punkte intern lösen, solange der oben festgelegte Vertrag eingehalten wird:

- konkrete Go-Strukturen und Helper,
- Parser-Dateiaufteilung,
- Generator-/Sidecar-Details,
- atomisches Schreiben und bestehende ID-/TIL-Locking-Mechanismen,
- konkrete Fehlertexte,
- interne Reihenfolge optionaler Metafelder,
- Rebase-/Wrapper-Implementierung,
- genaue CLI-Namensgebung für die spätere Level-Alias-Konfiguration.

Falls sich bei der Implementierung zeigt, dass einer dieser Punkte beobachtbares Verhalten ändern würde, ist das eine neue Designfrage; andernfalls nicht.

## 6. Idempotenz und Konsistenz

M19 und M20 dürfen keine kumulierenden Transformationen erzeugen.

Für `bind` gilt konzeptionell:

```text
bind(S, config) = bind(S, config)
```

Für M19 `insert/clean` muss die bestehende reversible Arbeitsweise erhalten bleiben.

Für eine mögliche spätere M20-Erweiterung auf `insert/clean` gelten zusätzlich:

```text
insert(insert(S, CE), CE) = insert(S, CE)
clean(clean(S, CE), CE)   = clean(S, CE)
clean(insert(S, CE), CE)  = S
```

unter identischer CE-Konfiguration und unverändertem Source zwischen den Operationen.

## 7. M17 und M18

**M17 bleibt zurückgestellt.** Bei späterem Auftrag zuerst Replay-Messungen für 0, 50, 90 und 100 Prozent verworfene Ereignisse mit CPU, Allokationen und Durchsatz. Danach über eine Optimierung entscheiden. Framing/Integritätsprüfung, Stempel-/Cycle-Zustand, Rohaufzeichnung, Statistik, Diagnosen und akzeptierte Records müssen identisch bleiben.

**M18 wird Abnahmearbeit innerhalb M19/M20.** Beispiele erst gegen die dann vorhandene CLI und passende Wörterbücher ausführen; JSON/KV einschließlich Grenzwerten vergleichen, C-Kontexte kompilieren und Shell-Quoting prüfen. Kein eigenes großes Testframework und keine Implementierung einer Option allein zum Ausführen eines Dokumentbeispiels.
