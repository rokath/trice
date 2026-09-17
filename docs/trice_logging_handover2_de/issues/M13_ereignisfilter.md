# M13: Tag-Auswahl und Gewicht einmal für das ganze Ereignis auswerten

**Status:** geplante Verhaltenskorrektur. **Alt:** L12 und Teil L11. **Voraussetzung:** M03, M08–M12.

Pick/Ban wird heute vor den Metadaten angewendet, `-logLevel` dagegen später auf einzelne Ausgabeteile. Eine erlaubte Warnung kann deshalb ihren Zeitstempel verlieren. Künftig entscheidet die Kombination aus Tag-Auswahl und Gewicht über das Ereignis samt eingeschalteter Zusatzspalten.

Regel: `(kein Pick oder Tag ausgewählt) UND (kein Ban oder Tag nicht verboten) UND (Gewicht >= Schwelle)`. Pick und Ban gemeinsam sind ungültig. `all` hebt nur die Gewichtsschwelle auf, `off` verwirft Anwendungsmeldungen. Beide ändern die Diagnose- und Rohaufzeichnungsregeln nicht.

## Abnahme und Tests

- [ ] Unterhalb, genau auf und oberhalb der Schwelle; Name/Alias/numerische Schwelle liefern passende Ergebnisse.
- [ ] Matrix aus ohne Tagfilter, Pick und Ban sowie jeweils `all`, `off` und normaler Schwelle. `-pick err:wrn -logLevel err` zeigt nur Fehler.
- [ ] Eine angenommene Meldung behält Host-/Targetstempel, Deltaspalten, Ort, ID, Präfix und Suffix; eine verworfene hinterlässt keine verwaisten Spalten.
- [ ] Farben `off`/`none`/`default` ändern nicht die Auswahl; lokale und entfernte Anzeige stimmen überein.
- [ ] Das [vollständige heutige Beispiel](../chapters/Log_Auswahl_DE.md#la-heute-loglevel) dient als Regression; weitere Fälle verwenden absichtlich niedrig gewichtete Darstellungstags.
- [ ] M12 deckt Teilzeilen ab; M10 schützt Diagnosen. Der Einfluss auf `-vis` wird ausdrücklich getestet: Im Zielbild gilt auch dort die gemeinsame Ereignisauswahl.

Eine Umstellung der Level-Auswahl für Visualisierung ist eine bewusste Verhaltensänderung. M17 behandelt erst danach eine mögliche Einsparung der Textformatierung.

**Quellen:** [Übersetzer](../../../internal/translator/translator.go), [ANSI-Filter](../../../internal/emitter/lineTransformerANSI.go).
