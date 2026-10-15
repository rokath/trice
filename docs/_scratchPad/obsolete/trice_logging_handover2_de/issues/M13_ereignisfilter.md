# M13: Tag-Auswahl und Gewicht einmal für das ganze Ereignis auswerten

**Status:** umgesetzt und getestet. **Alt:** L12 und Teil L11. **Voraussetzung:** M03, M08–M12.

Pick/Ban und `-logLevel` entscheiden gemeinsam vor dem Zeilenaufbau über einen ganzen Trice-Aufruf. Eine erlaubte Warnung behält deshalb auch ihre eingeschalteten Zusatzspalten. Vor M13 wirkte `-logLevel` erst auf einzelne Ausgabeteile und konnte diese getrennt unterdrücken.

Regel: `(kein Pick oder Tag ausgewählt) UND (kein Ban oder Tag nicht verboten) UND (Gewicht >= Schwelle)`. Pick und Ban gemeinsam sind ungültig. `all` hebt nur die Gewichtsschwelle auf, `off` verwirft Anwendungsmeldungen. Beide ändern die Diagnose- und Rohaufzeichnungsregeln nicht.

## Abnahme und Tests

- [x] Unterhalb, genau auf und oberhalb der Schwelle; Name/Alias/numerische Schwelle liefern passende Ergebnisse.
- [x] Matrix aus ohne Tagfilter, Pick und Ban sowie jeweils `all`, `off` und normaler Schwelle. `-pick err:wrn -logLevel err` zeigt nur Fehler.
- [x] Eine angenommene Meldung behält Host-/Targetstempel, Deltaspalten, Ort, ID, Präfix und Suffix; eine verworfene hinterlässt keine verwaisten Spalten.
- [x] Farben `off`/`none`/`default` ändern nicht die Auswahl; lokale und entfernte Anzeige stimmen überein.
- [x] Das [vollständige Beispiel](../chapters/Log_Auswahl_DE.md#la-heute-loglevel) dient als Regression; weitere Fälle verwenden absichtlich niedrig gewichtete Darstellungstags.
- [x] M12 deckt Teilzeilen ab; M10 schützt Diagnosen. Die gemeinsame Ereignisauswahl gilt auch für `-vis`.

Eine Umstellung der Level-Auswahl für Visualisierung ist eine bewusste Verhaltensänderung. M17 behandelt erst danach eine mögliche Einsparung der Textformatierung.

**Quellen:** [Übersetzer](../../../internal/translator/translator.go), [ANSI-Filter](../../../internal/emitter/lineTransformerANSI.go).
