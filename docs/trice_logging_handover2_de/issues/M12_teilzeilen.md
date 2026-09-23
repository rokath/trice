# M12: Filterregeln für Teilzeilen und mehrzeilige Aufrufe festlegen

**Status:** Semantik für M13 festgelegt; Regressionen für den Ist-Zustand ergänzt. **Alt:** L13.

Ein Trice-Aufruf ist ein Ereignis, aber nicht unbedingt eine Ausgabezeile. `-pick`/`-ban` entscheidet derzeit vor dem Zeilenaufbau über einen ganzen Aufruf. `-logLevel` entscheidet dagegen erst beim Darstellen über einzelne Teile einer fertigen Zeile; der letzte Teil vor dem Suffix kann dabei die gesamte Zeile unterdrücken. Bei einem mehrzeiligen Aufruf kann die Fortsetzungszeile den ursprünglichen Tag verlieren. Die Tests halten dieses Verhalten bis zur Korrektur in M13 fest.

**Festgelegte Zielregel für M13:** Jeder Aufruf wird genau einmal nach Pick/Ban und Gewicht ausgewählt. Angenommene Teile werden bis zum nächsten angenommenen Zeilenumbruch zusammengesetzt. Verworfene Aufrufe tragen weder Text noch Zeilenumbrüche oder Metadaten bei. Ein mehrzeiliger Aufruf wird vollständig angenommen oder verworfen; vorhandene Fortsetzungseinrückung bleibt Teil seines Textes. Zusatzspalten einschließlich Host- und Targetstempel, Ort und ID gehören zum ersten angenommenen Ereignis der sichtbaren Zeile. Bei einem verworfenen Zeilenumbruch bleibt die angefangene Ausgabezeile offen. Am Ende einer gepufferten Eingabe wird ein offener Rest mit einem Ausgabe-Zeilenumbruch abgeschlossen. Dazu genügt der vorhandene Zeilenpuffer; M13 soll keine unbeschränkte zusätzliche Ereigniswarteschlange einführen.

`-addNL` ergänzt im Decoder derzeit bedingungslos einen Zeilenumbruch an den Formatstring jedes Aufrufs. Damit beendet es auch einen angenommenen Aufruf ohne eigenen Abschluss. Bei einem bereits mit `\n` endenden Formatstring entsteht eine zusätzliche Leerzeile mit der vorhandenen Fortsetzungseinrückung; dieses bestehende Verhalten wird für M13 beibehalten. Ohne Zusatzspalten beträgt die Einrückung derzeit 13 Leerzeichen. Ein verworfener Aufruf trägt auch seinen durch `-addNL` ergänzten Umbruch nicht zur Ausgabe bei. Ein vollständig leerer Formatstring ohne `-addNL` erzeugt heute keine Ausgabe; mit `-addNL` entsteht eine Leerzeile. Ein reiner Newline-Aufruf erzeugt eine Leerzeile, sofern er angenommen wird.

Die folgenden Textbeispiele verwenden `-color none`, keinen Hoststempel und keine Zusatzspalten. Jede mit Komma getrennte Angabe ist ein eigener Trice-Aufruf; `\n` bezeichnet einen Zeilenumbruch im Formatstring. „Ist“ ist durch die neuen Regressionen belegt. Die Zielspalte ist der für M13 festgelegte Vertrag und derzeit bewusst kein grüner Verhaltenstest.

| Eingabe und Filter | Ist-Ausgabe | Ziel für M13 |
|---|---|---|
| `msg:A`, `dbg:B\n`, `msg:C\n`; `-pick msg` | `AC\n` | `AC\n` |
| `msg:A`, `dbg:B\n`, `msg:C\n`; `-ban dbg` | `AC\n` | `AC\n` |
| `msg:A`, `dbg:B\n`, `msg:C\n`; `-logLevel info` | `C\n` | `AC\n` |
| `dbg:A\nB\n`; `-logLevel info` | zweite Zeile `B\n` mit bestehender Einrückung | keine Ausgabe |
| `msg:A`, `dbg:B\n`; `-pick msg` | `A\n` beim Eingabeende | `A\n` beim Eingabeende |
| `msg:A`, `dbg:B`, `msg:C`; `-pick msg -addNL` | `A\nC\n` | `A\nC\n` |
| `msg:A\n`; `-pick msg -addNL` | `A\n` und eine Zeile mit 13 Leerzeichen | unverändert |
| `msg:\n`; `-pick msg` | eine Leerzeile | eine Leerzeile |

Für `msg:A` mit ID 1 und Targetstempel 10, verworfenes `dbg:B\n` mit ID 2/Stempel 20 und `msg:C\n` mit ID 3/Stempel 30 stammen die Zusatzspalten der Ausgabezeile von A. Eine folgende Zeile mit Stempel 40 hat eine Differenz von 30 zu A. Weder B noch das angenommene C innerhalb derselben Ausgabezeile ändern die Differenzbasis. Erst das erste angenommene Ereignis einer neuen sichtbaren Zeile aktualisiert sie. Die Regression prüft Ort, ID, Host-/Targetstempel, Präfix/Suffix und die Differenz gemeinsam.

Wenn dagegen der erste Aufruf mit ID 1/Stempel 10 verworfen wird, beginnt die Ausgabezeile erst mit dem angenommenen Aufruf ID 2/Stempel 20. Dessen Metadaten erscheinen und seine erste Zeitdifferenz ist leer; der nächste angenommene Zeilenanfang mit Stempel 30 zeigt die Differenz 10. Auch diese Herkunft ist als Regression festgehalten.

## Abnahme und Tests

- [x] Erwartete Ausgaben für verschieden getaggte Teilaufrufe, mehrzeilige Aufrufe und fehlenden Abschluss am Eingabeende sind festgelegt.
- [x] `A` angenommen, `B\n` verworfen, `C\n` angenommen ergibt `AC\n`. Die Abweichung bei `-logLevel info` von heute `C\n` zum Ziel `AC\n` ist ausdrücklich Teil von M13.
- [x] Leere und reine Newline-Aufrufe, `-addNL`, verworfene erste und letzte Teile sowie Metadatenherkunft sind durch Ist-Regressionen und Zielregeln abgedeckt.
- [x] Zeitdifferenzen bleiben auf das vorherige sichtbare Ereignis am Zeilenanfang bezogen. M13 soll den vorhandenen begrenzten Zeilenpuffer verwenden.
- [x] Die neuen Tests beschreiben den heutigen Stand; M13 ändert die abweichenden Erwartungen zusammen mit der Filterimplementierung.

Strukturierte Ausgabe behält eine Ereignisgrenze je Aufruf und übernimmt keine aus Konsolenzeilen abgeleitete Gruppierung. Das ist ein Vertrag für die spätere strukturierte Ausgabe, keine neue Ausgabeform in M12.

**Quellen:** [WriteLine](../../../internal/emitter/lineTransformerANSI.go), [Zeilenaufbau und Ende der Eingabe](../../../internal/translator/translator.go).
