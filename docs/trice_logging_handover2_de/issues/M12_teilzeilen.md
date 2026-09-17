# M12: Filterregeln für Teilzeilen und mehrzeilige Aufrufe festlegen

**Status:** Semantikabnahme und Regressionen vor M13. **Alt:** L13.

Ein Trice-Aufruf ist ein Ereignis, aber nicht unbedingt eine Ausgabezeile. Der aktuelle `WriteLine`-Pfad berücksichtigt unter anderem den letzten relevanten Teil einer Zeile. Das soll vor einer früheren Filterentscheidung mit konkreten Beispielen festgehalten werden.

**Vorschlag:** Jeder Aufruf wird einzeln ausgewählt. Angenommene Teile werden bis zum nächsten angenommenen Zeilenumbruch zusammengesetzt. Ausgeblendete Teile einschließlich ihrer Zeilenumbrüche fehlen vollständig. Zusatzspalten gehören zum ersten angenommenen Ereignis der Ausgabezeile. Eingabeende gibt einen offenen Rest aus; `-addNL` beendet jeden angenommenen Aufruf. Ein mehrzeiliger Aufruf wird vollständig angenommen oder verworfen.

## Abnahme und Tests

- [ ] Erwartete Ausgaben für zwei verschieden getaggte Teilaufrufe, einen mehrzeiligen Aufruf und fehlenden Abschluss am Eingabeende festlegen.
- [ ] Beispiel: angenommenes `A`, verworfenes `B\n`, angenommenes `C\n` ergibt im Vorschlag `AC\n`; die Änderung gegenüber bisherigem Verhalten ausdrücklich abnehmen.
- [ ] Leere beziehungsweise reine Newline-Aufrufe, `-addNL`, unterdrückter erster/letzter Teil und Metadatenherkunft sind abgedeckt.
- [ ] Zeitdifferenzen beziehen sich weiterhin auf die vorher angezeigten passenden Targetstempel. Keine unbegrenzte zusätzliche Pufferung einführen.
- [ ] Bestandstests beschreiben den heutigen Stand; M13 stellt sie nach Freigabe auf die neuen Erwartungen um.

Strukturierte Ausgabe behält eine Ereignisgrenze je Aufruf und übernimmt keine aus Konsolenzeilen abgeleitete Gruppierung.

**Quellen:** [WriteLine](../../../internal/emitter/lineTransformerANSI.go), [Zeilenaufbau und Ende der Eingabe](../../../internal/translator/translator.go).
