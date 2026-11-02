# Analyse zu Alias-Tags und String-Alias-CE

Stand: 2026-10-23, Branch `wip`, HEAD `cd26c8e2`. Ausschließlich Analyse und Vorschläge zur Abstimmung; keine Implementierungsfreigabe.

Grundlagen sind der aktive Auftrag in [scratchPad.md](scratchPad.md), der [Diskussionsstand zu #531](handsoff_discussion_531_alias_tag_mapping_DE.md), insbesondere Kapitel 44 und die abschließenden Notizen, sowie der live gelesene Inhalt von Issue #761. Das Issue ist offen, zuletzt am 2026-10-22 aktualisiert, ohne Kommentare. Die neueren Vorgaben im Auftrag gehen widersprechenden älteren Entwürfen vor.

## Ergebnis und Empfehlung

Die gewünschte Bedienung ist machbar: explizite CLI-Zuordnung vor Namensheuristik, danach Rückgriff auf einen gültigen Formatpräfix, zuletzt `untagged`. Der Aliasname in `Type` ermöglicht die automatische Wiedererkennung beim Logging. Für seltene explizite Abweichungen muss dieselbe Zuordnung beim Logging erneut angegeben werden.

Issue #761 und diese Aufgabe sollten gemeinsam spezifiziert, aber in getrennten Umsetzungsschritten bearbeitet werden: zuerst Aliasidentität und Tag-Zuordnung durch alle Verarbeitungsschritte, danach die begrenzte CE-Erweiterung für printf-artige String-Aliase. Zwei unabhängig implementierte Tag-Konzepte würden miteinander kollidieren.

Die wesentliche Einschränkung des gewünschten CLI-Overrides ist stärker als eine mögliche Fehlbedienung: Ohne Speicherung des Overrides kann die TIL allein dessen historische Bedeutung nicht sichern. Das ist mit der strengen Historienforderung aus #761 nicht gleichzeitig erfüllbar. Der Auftrag entscheidet sich für die CLI-Übergabe; die folgende Analyse macht deren Grenze ausdrücklich sichtbar und führt keine zusätzliche Metadatendatei ein.

## User Interface zur Abstimmung

### Tag-Zuordnung in vier Schritten

Für registrierte Aliase gilt der folgende vorgeschlagene Vertrag. Native Trice-Aufrufe behalten ihre bisherige Behandlung.

| Vorrang | Quelle | Beispiel |
|---|---|---|
| Zuerst | Expliziter Override `Name:Tag` im aktuellen Kommando | `-alias MyWrn:err` ergibt ERROR. |
| Danach | Nicht invalidierter Treffer im Aliasnamen | `MyWrn` ergibt WARNING. |
| Danach | Gültiger Tag am Anfang des statischen Formatstrings | `printi("err:code=%u", code)` ergibt ERROR. |
| Zuletzt | Keine Zuordnung | `printi("code=%u", code)` ergibt `untagged`. |

Ein explizites `Name:untagged` beendet die Suche ebenfalls. Es darf nicht anschließend doch das Formatpräfix aktivieren. Built-in-Schreibweisen wie `err` und `ERROR` sind gleichwertig; Aliasnamen selbst bleiben exakt und werden unverändert gespeichert.

Der Format-Fallback sollte zunächst die bestehende Präfixregel verwenden: Text vor dem ersten Doppelpunkt, sofern als Tag bekannt. Kein Suchen nach beliebigen Tags mitten im Satz. `temperature:err:...` liefert damit nicht automatisch ERROR; `temperature` kann unabhängig davon ein CE-Selektor sein. Eine Suche durch ganze Präfixketten wäre eine zusätzliche, gesondert abzustimmende Regel.

Custom-Tags können als gültige Formatpräfixe verwendet werden, wenn sie dem jeweiligen Kommando bekannt sind. Sie werden nicht automatisch Suchwörter der Namensheuristik. Für reproduzierbare Ergebnisse müssen Build und Host dieselben benötigten Custom-Tags kennen. Die acht eingebauten Heuristikgruppen vermeiden diese zusätzliche Registrierung im Normalfall.

### Namensheuristik mit gezielten Ausschlüssen

Die positiven Suchwörter aus Kapitel 44 können beibehalten werden:

| Gruppe | Suchwörter, ohne Beachtung der Großschreibung |
|---|---|
| FATAL | `fatal` |
| CRITICAL | `critical`, `crit` |
| ERROR | `error`, `err` |
| WARNING | `warning`, `warn`, `wrn` |
| INFO | `informal`, `info`, `inf` |
| DEBUG | `debug`, `dbg`, `deb`, `db` |
| TRACE | `trace`, `tr` |
| VERBOSE | `verbose` |

Nach den Ausschlüssen gewinnt das längste verbleibende Suchwort. Bei gleicher Länge gilt die Reihenfolge dieser Tabelle. Beispiel: `Log_Error_Warning` ergibt WARNING; `LogErrWrn` ergibt ERROR. Gewichtung und Farben ändern diese Auswahl nicht.

**Ausschlüsse gelten nur für den konkreten eingebetteten Treffer, niemals für den ganzen Alias oder die ganze Tag-Gruppe.** In `InfraInfo` wird `inf` in `infra` verworfen, das separate `info` bleibt INFO. In `TransferTrace` wird das erste `tr` verworfen, `trace` bleibt TRACE. Alle Vorkommen müssen einzeln betrachtet werden.

Folgende kleine Starttabelle ist ein Vorschlag, keine bereits implementierte oder abschließend freigegebene Liste:

| Suchwort | Invalidierender Kontext | Plausible Namen und Bedeutung |
|---|---|---|
| `inf` | `infra` | `Infrared`, `Infrastructure`: Infrarot bzw. Infrastruktur |
| `inf` | `infer`, `infl`, `infect` | `Inference`, `Inflate`, `Infection`: Schlussfolgerung, Aufblasen, Infektion |
| `db` | `adba`, `adbi`, `edba`, `edbe`, `ndba` | `Readback`, `ReadBinary`, `Feedback`, `Seedbed`, `Bandwidth` |
| `deb` | `debit`, `debt`, `deboun` | `Debit`, `Debt`, `Debounce`: Buchung, Schuld, Entprellen |
| `tr` | `tran`, `tri`, `tro`, `tru`, `try` | `Transfer`, `Trigger`, `Control`, `True`, `Retry` |
| `tr` | `track`, `trap`, `tray`, `treat`, `tree`, `trend`, `trem` | `Track`, `Trap`, `Tray`, `Treat`, `Tree`, `Trend`, `Tremor` |
| `err` | `berry`, `cherry`, `ferr`, `terr` | `Berry`, `Cherry`, `Ferrite`, `Terrain` |

Die Kontextprüfung bedeutet jeweils: Das gefundene Suchwort liegt innerhalb eines Vorkommens des angegebenen Kontextes. `edba` sperrt beispielsweise genau das darin enthaltene `db`. Das vorgeschlagene `tran` sperrt den darin enthaltenen `tr`-Treffer, nicht ein späteres `trace`.

Die kurzen Muster sind bewusst nicht sprachlich vollständig. Beispielsweise könnte `AdbInterface` das legitime Kürzel ADB enthalten und durch `adbi` unterdrückt werden; ein bewusst als TRACE verwendetes `TryLog` würde ebenfalls unterdrückt. `err` in `Interrupt` bleibt mit dieser Liste ein problematischer Treffer. Ein pauschales `interr`-Verbot würde wiederum `PrintError` beschädigen. Deshalb empfehle ich keine immer größere englische Wortliste und keine automatische Erweiterung nach Veröffentlichung. `LogErrorCount` bleibt wie bereits diskutiert ein akzeptierter ERROR-Treffer.

`informal` bleibt hier wegen des bisherigen Entwurfs ein positives INFO-Wort, obwohl es im allgemeinen Englisch „informell“ bedeutet. Auch das ist eine Namenskonvention, keine sichere semantische Erkennung.

| Alias | Format | Ergebnis mit vorgeschlagener Liste |
|---|---|---|
| `LogInfrared` | `ready` | `untagged` |
| `LogInfrared` | `err:failed` | ERROR durch Format-Fallback |
| `Readback` | `ready` | `untagged` |
| `ReadbackDebug` | `ready` | DEBUG durch `debug` |
| `Transfer` | `ready` | `untagged` |
| `TransferTrace` | `ready` | TRACE durch `trace` |
| `Debounce` | `wrn:unstable` | WARNING durch Format-Fallback |
| `InfraInfo` | `err:failed` | INFO; Namenstreffer hat Vorrang |
| `printi` | `err:failed` | ERROR durch Format-Fallback |
| `MyWrn` | `err:failed` | WARNING |
| `MyWrn` mit `MyWrn:err` | `wrn:failed` | ERROR durch Override |

Vor Veröffentlichung sollten positive und negative Tabelle zusammen festgeschrieben werden. Ihre spätere Änderung verändert sonst die Auswertung alter Aufzeichnungen, obwohl deren TIL unverändert bleibt. Eine genaue Erkennungsquote lässt sich aus Beispielen nicht ableiten; dafür wären reale Aliasnamen aus Projekten nötig.

### Meldungstext und Format-Fallback

Für eine einfache Bedienung empfehle ich: **Bei neuen Alias-Einträgen bleibt der Meldungstext einschließlich Tag-Präfix erhalten.** Klassifikation fügt nichts hinzu und entfernt nichts. Das gilt auch dann, wenn erst der Format-Fallback den Tag liefert. Nur ausdrücklich konfigurierte CE-Transformationen dürfen ihren vorgesehenen Text ändern.

| Aufruf | Tag | Text ohne CE |
|---|---|---|
| `MyWrn("err:failed")` | WARNING | `err:failed` |
| `printi("err:failed")` | ERROR | `err:failed` |
| `printi("ready")` | `untagged` | `ready` |

Diese Textregel ist noch abzustimmen: Der Auftrag legt den Tag-Fallback fest, aber nicht das Entfernen des dabei erkannten Präfixes. Sie setzt die bisher gewünschte unveränderte Aliasnachricht fort. Eine vollständige Übernahme bisheriger Präfixdarstellung wäre eine andere mögliche Bedienung und darf nicht stillschweigend unterstellt werden.

Text, JSON/KV, Filter, Statistik und Visualisierung müssen denselben Ereignis-Tag verwenden. Ein `err:` in einem eingesetzten Parameter oder im empfangenen Laufzeitstring darf die bereits bestimmte Zuordnung nicht nachträglich verändern.

### Seltene Overrides beim Build und Logging

Gewünschte neue Syntax, nicht als heute verfügbar zu verstehen:

```sh
trice bind -src app -til til.json -li li.json -alias MyWrn:err
tlog -til til.json -li li.json -alias MyWrn:err
```

Die Aufrufe zeigen nur die relevanten Optionen; Transportparameter kommen wie gewohnt hinzu. `trice log` sollte dieselbe Zuordnung wie `tlog` akzeptieren. Entsprechend gilt `-salias Name:Tag` für String-Aliase; die Kurzformen `-a` und `-sa` sollten dieselbe Syntax erhalten.

Beim Build bestimmt der Override den Ereignis-Tag, gegebenenfalls den ID-Bereich und die tag-basierte CE-Auswahl. Beim Logging bestimmt er ausschließlich die Host-Klassifikation und deren Folgen. Er kann weder bereits gewählte Target-Routen ändern noch fehlende CE-Werte nachträglich hinzufügen.

Ohne `tlog -alias MyWrn:err` wird `MyWrn` wieder WARNING. Der Host kann das vergessene Argument nicht zuverlässig erkennen: Die TIL enthält den Aliasnamen, aber nicht die beim Build gewählte Abweichung. Auch ein ID-Bereich ist kein allgemeiner Ersatz für diese Information.

Ein Override gilt für alle passenden Aliasnamen in der geladenen TIL. Enthält sie alte und neue IDs desselben Alias mit unterschiedlichen beabsichtigten Overrides, kann eine einzelne Host-Option diese nicht unterscheiden. Selbst unterschiedliche IDs lösen das Problem nicht. Für solche Aufzeichnungen muss die passende Logging-Konfiguration extern zusammen mit den Aufzeichnungen aufbewahrt werden; gemeinsam gemischte Bedeutungen desselben Alias bleiben damit nicht eindeutig auswertbar.

Für Custom-Tags empfehle ich, `Name:Tag` als Registrierung dieses Namens im jeweiligen Kommando zu behandeln. Gewicht und Farbe bleiben getrennte Host-Optionen. Das erspart eine doppelte Namensregistrierung, muss aber noch bestätigt werden. Die Umbenennung `-ulabel` nach `-utag` und die zusätzliche Schreibweise `-logformat` aus Kapitel 44 sind separate CLI-Punkte; dieser Analyseauftrag setzt sie nicht um.

### Wiederholungen und Doppelregistrierung

„Bestehende Doppelregistrierungsregel“ meint genau diesen Fall:

```sh
-alias Example -salias Example
```

Die aktuelle Funktion `resolveTriceAlias` entscheidet dann anhand des letzten Buchstabens: `s` oder `S` bedeutet String-Alias, sonst skalar. Die Optionsreihenfolge spielt dabei keine Rolle. Ein ausschließlich unter `-alias` registriertes `StatusS` bleibt dagegen skalar; ein ausschließlich unter `-salias` registriertes `LogErr` bleibt String-Alias. Diese Regel betrifft den Payload-Grundtyp, nicht WARNING/ERROR oder mehrfache Tag-Treffer. Sie wurde im Entwurf aus Bestandsschutz übernommen; aus dem Code ergibt sich kein notwendiger Anwendungsfall für diese Mehrdeutigkeit. Quelle: [helper.go](../../internal/id/helper.go).

Ich empfehle für die neue Bedienung:

| Eingaben für denselben exakten Namen | Verhalten |
|---|---|
| Gleiche Aliasart und gleiche kanonische Zuordnung wiederholt | Akzeptieren, einmal wirksam |
| Einmal ohne Tag, einmal mit explizitem Tag | Explizite Zuordnung ergänzen, unabhängig von der Reihenfolge |
| Verschiedene explizite Tags | Fehler vor Dateiänderungen bzw. Öffnen des Log-Eingangs |
| Derselbe Name unter `-alias` und `-salias` | Fehler, auch bei gleichem Tag |

Der letzte Punkt wäre eine bewusste Änderung des bisherigen Verhaltens und ist deshalb als Vorschlag ausgewiesen. Unbenutzte gültige Aliasdefinitionen bleiben laut abschließender Notiz wirkungslos und ohne normale Warnung; `-verbose` zeigt sie an. Ungültige oder widersprüchliche Definitionen bleiben auch unbenutzt Fehler.

### String-Aliase und Context Enrichment

Der Einwand gegen das pauschale CE-Verbot ist berechtigt: Ein printf-artiger String-Alias kann zusätzliche Formatargumente verarbeiten, bevor er die fertige Zeichenkette über `triceS` überträgt. Es braucht dafür kein neues Wire-Format und keinen gemischten Datensatz aus String und Skalaren.

Beispiel für das gewünschte künftige Verhalten:

```c
LegacyPrint("temperature:value=%d", temperature);
```

```sh
-salias LegacyPrint:wrn -ce 'temperature:", task=%s", taskName()'
```

Vor der projektseitigen Formatierung entspricht dies konzeptionell:

```c
LegacyPrint("value=%d, task=%s", temperature, taskName());
```

Der Target sendet danach eine fertige Zeichenkette. Die zusätzlichen Werte sind in JSON/KV Text und keine einzeln typisierten Felder. Floats folgen den Regeln des verwendeten printf-Formatierers, nicht den Bit-Transportregeln von `aFloat`/`aDouble`. Buffergröße, unterstützte Konvertierungen und Abschneiden bleiben Verantwortung des Projekt-Wrappers.

Nicht jeder `-salias` führt jedoch printf aus. Ein direkter Alias auf `triceS` hat keine solche Formatierungsstufe. Auch `MyAssert(condition, ...)`, dynamische Formate oder vorangestellte Kanalargumente sind nicht mit derselben einfachen Transformation abgedeckt. Die Registrierung muss deshalb für diese Erweiterung einen printf-artigen Vertrag voraussetzen; allein die syntaktische Form beweist die tatsächliche Makroimplementierung nicht.

| Verwendung | CE mit zusätzlichen Werten |
|---|---|
| Skalarer `-alias` | Bestehende skalare Regeln; kein Laufzeit-`%s`, bestehende Breiten- und Argumentgrenzen |
| Printf-artiger `-salias` mit erstem semantischen Argument als Formatliteral | Machbar, einschließlich `%s`, mit begrenzter unterstützter Formatgrammatik |
| Komplexer String-Wrapper | Ohne CE weiter nutzbar; ausgewählte nicht unterstützte CE klar ablehnen |
| Direktes `triceS` oder bloße Weiterleitung dorthin | Keine zusätzlichen Laufzeitargumente; reine Texterweiterung nach bisherigen Regeln |

Virtuelle CE-Auswahl sollte zuerst den effektiven Alias-Tag verwenden, danach die Quellpräfixe, jede kanonische Gruppe einmal und Regeln derselben Gruppe in CLI-Reihenfolge. Dabei hat das Formatpräfix zwei mögliche Rollen: Fallback für den Tag und unabhängiger CE-Selektor. Eine ERROR-CE-Regel kann daher auch bei `MyWrn("err:...")` ausgewählt werden, ohne den Tag WARNING zu ändern.

Für String-Aliase ohne Namenstreffer ist außerdem festzulegen: Der Tag-Fallback liest nur ein statisch erkennbares erstes Formatliteral der einfachen Form. Bei `MyAssert(condition, "err:...")` wird nicht irgendwo in den Argumenten nach einem Tag gesucht. Ohne Namenstreffer oder Override bleibt dieser komplexe Fall `untagged`. Empfangener Text ist keine Ersatzquelle für Build-Zuordnungen.

### Diagnostik und offene UI-Entscheidungen

Mit `-verbose` sollten Grundtyp, gewählter Tag und Ursache sichtbar sein: Override, positiver Namenstreffer, invalidierter Treffer, Format-Fallback oder `untagged`. Ein abweichendes bekanntes Quellpräfix kann ebenfalls dort erklärt werden. Ein neuer Warnschalter ist dafür nicht nötig. Das entspricht dem bisherigen Entwurf, weicht aber von der standardmäßig sichtbaren, unterdrückbaren Warnung in #761 ab.

Vor einer Implementierung sind vor allem diese Punkte abzustimmen:

- Die konkrete Invalidierungstabelle einschließlich akzeptierter Fehlzuordnungen.
- Erhalten des sichtbaren Präfixes auch beim Format-Fallback und Beschränkung auf den ersten Formatpräfix.
- Doppelregistrierung als Fehler sowie Zusammenführen von Registrierung ohne Tag und explizitem Override.
- Custom-Tag-Registrierung durch `Name:Tag` und der beschriebene statische String-Alias-Fallback.
- Der ausdrückliche Verzicht auf die strenge Override-Historiengarantie von #761 zugunsten erneuter CLI-Angabe.
- Printf-artige String-Alias-CE als nachfolgender, begrenzter Umsetzungsschritt.

## Verhältnis zu Issue #761

Das Issue wurde live gelesen. Es beschreibt denselben Problemkern aus #531, ist aber in wesentlichen Designpunkten auf einem anderen Stand. Die folgende Gegenüberstellung paraphrasiert seine Anforderungen; sie übernimmt diese nicht als zusätzliche Arbeitsfreigabe.

| Thema | Issue #761 | Aktueller Auftrag / Folgerung |
|---|---|---|
| Namen | Keine Severity aus Namen ableiten | Direkter Widerspruch: Heuristik ist ausdrücklich gewünscht. |
| Registrierung ohne Tag | Bisheriges Verhalten unverändert | Heuristik kann vorhandene Zuordnungen verändern; keine vollständige Verhaltenskompatibilität. |
| Explizites `Name:Tag` | Vorgesehen | Passt; Host-Wiederholung ist nun die gewählte Übergabe. |
| Historische Zuordnung | Alte Ereignisse auch nach Tag-Änderung eindeutig | Mit nicht persistiertem Override nicht allgemein erfüllbar. |
| TIL-Darstellung | Mehrere Wege erst analysieren | Alias in `Type`, keine neuen Tag-Felder oder synthetischen Präfixe gemäß neuer Richtung. |
| String-Alias-CE | Begrenzte printf-Form einschließlich `%s` | Technisch sinnvoll; ersetzt das pauschale Verbot aus Kapitel 44. |
| CE-Auswahl | Effektiver Tag plus Quellselektoren | Vereinbar mit Heuristik und Format-Fallback. |
| Konflikthinweis | Unterdrückbare Warnung | Vorschlag hier: nur mit `-verbose`; Entscheidung nötig. |
| Native `triceS`, Wire, Location | Bestehende Grenzen und ID/LI erhalten | Vereinbar; keine neue File-ID oder Target-Tag-Payload. |
| CLI-Namen | Verwendet `-ulabel` | Muss mit der separat geplanten Umbenennung abgestimmt werden. |

Empfohlene Reihenfolge:

1. Gemeinsames User Interface abstimmen. Vor Beginn späterer Codearbeit den Issue-Text an den vereinbarten Stand anpassen; insbesondere Heuristik, Historiengrenze, TIL und Diagnostik. Eine GitHub-Änderung ist nicht Teil dieses Auftrags.
2. Aliasidentität, Grundtypauflösung, Tag-Heuristik, Fallback und Overrides durch Build und Host umsetzen. Skalare CE nutzt dabei dieselbe Tag-Entscheidung. Die zulässigen String-Alias-Fälle ohne zusätzliche CE bleiben erhalten.
3. Printf-artige String-Alias-CE auf dieser Grundlage ergänzen, einschließlich Bind und Insert/Clean. Ein hardwarefreier C-Nachweis muss belegen, dass wirklich der Projekt-Formatter mit der erweiterten Argumentliste aufgerufen wird.
4. Anschließend gemeinsam integrieren und dokumentieren. Getrennte reviewbare Änderungen sind sinnvoll; zwei alternative öffentliche Tag-Semantiken sollten nicht zwischendurch veröffentlicht werden.

## Implementierung und technische Grenzen

### Was im aktuellen Code tatsächlich vorhanden ist

| Befund | Bedeutung | Quelle |
|---|---|---|
| `TriceFmt.Alias` hat `json:"-"`; Aliasauflösung setzt `Type` auf `trice` bzw. `triceS`. | Der bisherige Aliasname bleibt nicht als eigenes TIL-Feld erhalten. Alias in `Type` wäre eine echte Bedeutungsänderung. | [id.go](../../internal/id/id.go), [helper.go](../../internal/id/helper.go) |
| String-Alias-Quellargumente werden in `SAlias_Strg('...')SAlias_Strg` gespeichert. | Daraus lässt sich der String-Pfad unterscheiden; `%s` allein ist kein ausreichendes Unterscheidungsmerkmal. | [bindParse.go](../../internal/id/bindParse.go), [insertIDs.go](../../internal/id/insertIDs.go), [manage.go](../../internal/id/manage.go) |
| ID-Auswahl liest bisher den Präfix aus `Strg`. | Heuristik und Overrides müssen vor der ID-Bereichsauswahl wirksam werden. | [switchIDs.go](../../internal/id/switchIDs.go) |
| CE weist zusätzliche Argumente für Sonderfamilien zurück. | String-Alias-CE erfordert einen eigenen begrenzten Pfad vor dem Target-Formatter. | [contextEnrichment.go](../../internal/id/contextEnrichment.go) |
| Bind-CE rendert aktuell einen Aufruf von `TRICE_INSERT_<Typ>`. | Nur die Ablehnung zu entfernen würde nicht die printf-Stufe des Projekt-Wrappers erhalten. | [renderBindContext](../../internal/id/contextEnrichment.go) |
| `PRINTS_WITH_ID` formatiert in einen lokalen Buffer und sendet diesen. | Der in #761 beschriebene printf-Pfad existiert als konkretes Testbeispiel. | [triceConfig.h](../../_test/alias_dblB_de_tcobs_ua/triceConfig.h) |

Das erklärt das frühere Verbot: Die bestehende CE-Verarbeitung erwartet ein Trice-Format, während die String-Alias-Hülle eine ganze C-Argumentliste enthält. Ein Anhängen an diese Hülle oder an das spätere Hostformat `%s` erreicht den Target-Formatter nicht. Das ist eine Grenze der bisherigen Umsetzung, kein prinzipielles Verbot von String-Alias-CE.

### Gemeinsame Auflösung und Reihenfolge

Eine spätere Implementierung sollte Originalalias, Grundtyp, effektiven Tag und Herkunft der Klassifikation getrennt halten. Dieselbe Entscheidungslogik muss Instrumentierung und Host bedienen. Der Host darf den Aliasnamen nicht wie einen nativen Typ auf Bitbreiten oder Endbuchstaben untersuchen.

Für den Format-Fallback entsteht eine zusätzliche Bedingung gegenüber Kapitel 44: Der Host muss dieselbe statische Formatgrundlage wie der Build sehen. Skalare CE darf diese Grundlage nicht so umschreiben, dass später ein anderer Fallback entsteht. Die jetzige CE-Auswahl behält bekannte Tag-Präfixe bei und entfernt passende freie kleingeschriebene Selektoren. Diese Eigenschaft muss gezielt abgesichert werden. Bei String-Alias-CE muss die vollständige Hülle samt rekonstruierbarem erstem Literal erhalten bleiben.

Die Reihenfolge lautet: Registrierung prüfen, Aufruf und Grundtyp erkennen, Tag bestimmen, CE auswählen, effektive Argumente/Format prüfen, ID-Identität und Bereiche bestimmen, Änderungen veröffentlichen. Clean muss eigene Instrumentierung exakt entfernen können; Bind muss Quelltexte unverändert lassen. Die Sidecar-Anpassung darf den Projekt-Formatter nicht umgehen und CE-Ausdrücke nicht doppelt auswerten.

Mehrere reine CLI-Parser reichen nicht: TIL-Laden und Nachladen, ID-Wiederverwendung, Generatoren, lokale C-Logtabellen und sämtliche Host-Ausgaben müssen den neuen Alias-`Type` verstehen. Alte native Einträge behalten ihre bisherige Bedeutung; alte Tools können neue Alias-TILs nicht zwingend lesen. Es wird keine automatische Migration vorgeschlagen.

Für automatisch erkannte Tags ist die Identität aus Aliasname und unverändert erhaltener Formatgrundlage unter einem festen Heuristikvertrag nachvollziehbar. Für explizite Overrides gilt diese Ableitung gerade nicht. Ohne zusätzliche persistierte Information darf eine Umsetzung daher keine allgemeine Garantie „geänderter Override erzeugt historisch eindeutig anders klassifizierbare ID“ versprechen.

## Prüfung von TRICE_LEGACY_RPC_SUPPORT

Der Schalter ist aktiv und absichtlich vorhanden, kein bloßer veralteter Dokumentationsrest:

- [triceDefaultConfig.h](../../src/triceDefaultConfig.h) definiert ihn standardmäßig als `0` und markiert die F-Unterstützung ausdrücklich als deprecated. Der Kommentar erklärt, dass alter RPC-Generator und Hostoption entfernt wurden, die F-Implementierungen aber für Quell- und Wire-Kompatibilität bestehen bleiben.
- [trice.h](../../src/trice.h), [trice.c](../../src/trice.c) sowie `trice8.h`, `trice16.h`, `trice32.h` und `trice64.h` schalten damit tatsächlich F-Code frei.
- Mehrere Hardwarebeispiele aktivieren ihn, darunter `F030_inst`, `G0B1_inst`, `G0B1_features` und `L432_inst`. `PC_log` und `G0B1_log` setzen ihn ausdrücklich auf `0`.
- Viele `_test`-Konfigurationen aktivieren ihn. Die zentralen F-Fälle in [triceCheck.c](../../_test/testdata/triceCheck.c), weitere Target-Tests und ein [Bind-Integrationstest](../../internal/id/bindIntegration_test.go) sichern Legacy-Verhalten ab.
- Die aktuellen Begründungen stehen außerdem im [Implementierungsplan](Implementierungsplan.md). Historische Erwähnungen stehen im [Changelog](../../CHANGELOG.md) und in archivierten Entwürfen. Die Suche nach dem exakten Namen ergab keinen Treffer unter `docs/rm/`.

Alias-Unterstützung für bestehende Logging-Makros und Legacy-RPC/F-Unterstützung sind unterschiedliche Funktionen. #531/#761 erfordert keine Entfernung oder Umbenennung dieses Schalters. Ein mechanischer Wechsel von F nach C wäre falsch: Gestempelte C-Makros verlangen explizite Stempelargumente. Entfernung, Migration oder Überarbeitung der Beispiele wäre eine eigene Aufgabe. Archive bleiben unverändert.

## Spätere Tests und Abnahme

Diese Prüfungen sind ein Plan, keine in diesem Analyseauftrag ausgeführten Tests.

| Bereich | Wesentliche beobachtbare Ergebnisse |
|---|---|
| Heuristik | Alle positiven und negativen Muster; Ausschluss nur des überlappenden Treffers; unabhängiger zweiter Treffer; längster Treffer; Gleichstand; Schreibweisen |
| Priorität | Override vor Name vor Format vor `untagged`; explizites `untagged` stoppt; Custom-Tag-Registrierung; kein Tag aus Laufzeitparametern |
| CLI | Wiederholungen und Konflikte in beiden Reihenfolgen; ungenutzte gültige Aliase; doppelte Aliasarten; klare Fehler vor Seiteneffekten |
| Build und Host | Identische Tags ohne Override; gewünschte Abweichung bei vergessenem Override; Host-Override ändert keine Target-Route oder CE-Payload |
| TIL und Historie | Originalname und Hülle bleiben erhalten; native Alt-Einträge; fehlerhafte Hüllen; Rename und Grundtypwechsel; dokumentierte Override-Grenze |
| Ausgabe | Text, JSON/KV, Filter, Statistik und Visualisierung stimmen überein; Präfixschutz auch bei mehreren Ereignissen pro Zeile und mehrzeiligen Nachrichten |
| Skalare CE | Virtueller Tag plus Quellselektoren jeweils einmal; Grenzen für `%s`, Breiten, Argumentzahl und Float-Wrapping |
| String-Alias-CE | Integer, `%s`, Float nach Formatter-Regeln; tatsächlicher printf-Pfad; genau eine Auswertung zusätzlicher Ausdrücke; ein String-Ereignis ohne erfundene typisierte Felder |
| Nicht unterstützte Formen | Bedingung/Kanal vor Format, dynamischer Formatstring, direkte `triceS`-Weiterleitung; ohne CE unveränderte Nutzbarkeit |
| Roundtrip | Bind verändert keine Quelle; Insert/Clean stellt die ursprüngliche Quelle wieder her; Fehler hinterlassen keine Teiländerungen |
| Integration | Hardwarefreier C-Sender plus Hostdecoder, LI-Aufrufposition, Betrieb mit `-li off`, konsistentes TIL-Nachladen und relevante Generatoren |

Nach späteren fokussierten Tests ist `./scripts/testAll.sh` der reguläre Gesamtlauf. `full` bleibt einer begründeten abschließenden C-/Compilerprüfung vorbehalten. Bei Go-Änderungen gilt vor einem späteren Push die Coverage-Prüfung aus dem [Workflow](../../.github/workflows/coverage.yml), einschließlich Vergleich der abgedeckten Zeilen mit dem Zielbranch.

## Prüfstand dieses Dokuments

Durchgeführt: read-only Prüfung des lokalen Codes, des Diskussionsentwurfs, der aktiven Referenzdokumentation, aller mit `rg` gefundenen Verwendungen von `TRICE_LEGACY_RPC_SUPPORT` und des live abgerufenen Issue #761. Der erste `gh issue view` lieferte trotz Exitcode 0 keinen Inhalt; der Issue-Inhalt wurde anschließend erfolgreich über den GitHub-Connector gelesen. Keine Änderung am Issue.

Keine Builds oder Verhaltenstests ausgeführt, da keine Implementierung geändert wurde. Die Machbarkeit der String-Alias-CE ist aus den vorhandenen Verarbeitungspfaden begründet, aber noch nicht durch einen neuen C-Prototyp nachgewiesen. Als Ergebnis wird ausschließlich dieses Dokument angelegt; die vorhandene lokale Änderung an `scratchPad.md` bleibt unverändert.
