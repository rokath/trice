# M20: Kontextanreicherung für Bind und Insert/Clean zuverlässig spezifizieren

**Status:** offene Entwurfsaufgabe. **Alt:** L22; mit M19 abstimmen.

Der [Kontextentwurf](../chapters/Kontextanreicherung_DE.md) trennt statische Wörterbuchdaten, Hostkontext und zusätzlich erfasste Laufzeitwerte. Bind soll Anwendercode unverändert lassen. Insert/Clean bleibt eine zulässige Alternative, sofern die Rücknahme zuverlässig ist.

## Entscheidung und Abnahmekriterien

- [ ] Einen konkreten Weg zur Argumentergänzung bei Bind zeigen. Ein `til.json`-String allein erfasst keinen dynamischen Wert.
- [ ] Ursprüngliche Aufrufform und wirksame Konfiguration so nachweisen, dass Clean auch nach Konfigurationsänderung oder Abbruch korrekt arbeitet.
- [ ] ID-Fälle durchspielen: unverändert, andere `-stf`-Vorlage, Rückkehr zur alten Vorlage, gleicher Text an anderer Stelle, verschobene/duplizierte Logstelle.
- [ ] Vorschlag prüfen: historische ID nur für dieselbe Logstelle und Definition wiederverwenden, wenn policykonform; sonst passende andere ID, alte Zuordnung behalten. `li.json` allein nicht als unveränderliche Herkunft voraussetzen.
- [ ] Wörterbuch, generierte Artefakte und Firmware bei Fehlern konsistent halten; erschöpften Bereich, geteilte Wörterbücher und unterbrochene Läufe berücksichtigen.
- [ ] Cache anhand wirksamer Eingaben erneuern; wiederholte unveränderte Läufe bleiben stabil.
- [ ] Dynamische Ausdrücke genau einmal erfassen; zulässige Kontexte, Argumentzahl/Bitbreite und Ressourcenaufwand nennen. Keine pauschale Kostenfreiheit.
- [ ] Für normale und strukturierte Aufrufe denselben Kontextvertrag verwenden; Anwenderfelder und `ctx` dürfen sich nicht überschreiben.

Die Compilerdiagnostik-Idee für benutzerdefinierte statische Werte bleibt eine spätere Option im redaktionellen Vorspann. Die konkrete Syntax und Herkunftsspeicherung werden vor einer Implementierung abgestimmt.

**Quellen:** [bisheriger Enrichment-Draft](../../TriceUserManual.md#trice-structured-logging), [Bind](../../TriceUserManual.md#trice-bind), [ID-Vergabe](../../../internal/id/insertIDs.go).
