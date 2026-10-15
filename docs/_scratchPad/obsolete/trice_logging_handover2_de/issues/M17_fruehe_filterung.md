# M17: Frühe Hostfilterung messen und nur bei Nutzen umsetzen

**Status:** zurückgestellt, niedrige Priorität. Keine Voraussetzung für M19/M20. **Alt:** L17. Aktuelle Planung: [Implementierungsplan](../../../Implementierungsplan.md).

Die vorhandene Filtersemantik aus M10–M14 bleibt maßgeblich. Eine frühere Entscheidung könnte Textformatierung für verworfene Ereignisse vermeiden; ein ausreichender Nutzen ist noch nicht gemessen. Nach der Einführung strukturierter Records ist der dann tatsächliche Pfad zu messen, statt vorab eine zweite Architektur einzuführen.

## Bei späterem Auftrag

- Reproduzierbares Replay bei 0, 50, 90 und 100 Prozent verworfenen Ereignissen vergleichen: CPU, Allokationen und Durchsatz; Firmware-/TIL-Fixture und Messbedingungen festhalten.
- Text, strukturierte Records, Metadaten, Teilzeilen, Zeitdifferenzen, Visualisierung, Diagnosen und Statistik semantisch unverändert halten.
- Framing, Längen-/Integritätsprüfung, Cycle-/Stempelzustand und Rohaufzeichnung auch für verworfene Ereignisse weiterführen.
- Nach Wörterbuch- oder Labeländerung keine veralteten Filterentscheidungen verwenden; nicht ID-basierte Records gesondert betrachten.
- Ohne nachgewiesenen praktischen Nutzen bei Messergebnis und Entscheidung bleiben.

Der aktuelle Dokumentationsauftrag führt weder Benchmarks noch eine Optimierung aus. Targetprotokoll und Target-Level-Steuerung gehören nicht zu dieser Aufgabe.
