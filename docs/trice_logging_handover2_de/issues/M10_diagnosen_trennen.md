# M10: Decoder- und Werkzeugfehler von Anwendungsfiltern trennen

**Status:** umgesetzt. **Alt:** L14.

Der TREX-Decoder kennzeichnet zusammenhängende Ausgabebereiche beim Erzeugen als Anwendungstext oder Werkzeugdiagnose. Der Übersetzer leitet nur Anwendungstext durch Pick/Ban, Level, Metadaten und Visualisierung. Diagnosen gehen direkt an die lokale Werkzeugausgabe.

Damit bleiben unbekannte IDs, ungültige COBS-/TCOBS-Frames, nicht unterstützte kurze Pakete, Type-X0-Fehler und Cycle-Hinweise bei `-pick`, `-ban` und `-logLevel off` beobachtbar. Eine zusammen mit einem Cycle-Hinweis dekodierte gültige Anwendungsmeldung bleibt ein eigener Bereich und unterliegt weiterhin den Anwendungsfiltern. Diagnosen werden nicht dem späteren `untagged` zugeordnet.

## Abnahme und Tests

- [x] Unbekannte ID, beschädigter/abgeschnittener Frame und unterstützte Cycle-/Integritätsfehler bleiben unter restriktiven Filtern sichtbar.
- [x] Fehler liefern keinen erfundenen normalen Datensatz und behalten gegebenenfalls ihren Fehlerstatus.
- [x] Künftige maschinenlesbare Ausgabe enthält keine hineingemischten Banner oder freien Warntexte.
- [x] Ziel der Diagnoseausgabe und Fehlerbehandlung bleiben dokumentiert; die Rohaufzeichnung bleibt bytegleich.

Der vorhandene Werkzeug-Writer und der Anwendungs-Line-Composer sind getrennte Parameter. Die heutige CLI richtet beide auf ihre lokale Textausgabe; eine künftige maschinenlesbare Anwendungsausgabe muss Diagnosen beim Werkzeug-Writer belassen. Nicht fatale Decoderdiagnosen setzen die Verarbeitung fort. Schreib- und Eingabefehler behalten ihre bisherige Fehlerbehandlung. Die Binäraufzeichnung liegt weiterhin vor dem Decoder und bleibt unverändert.

Die Benutzerbeschreibung steht unter [Decoder diagnostics](../../TriceTagsAndColor.md#decoder-diagnostics).

**Quelle:** [decodeAndComposeLoop](../../../internal/translator/translator.go).
