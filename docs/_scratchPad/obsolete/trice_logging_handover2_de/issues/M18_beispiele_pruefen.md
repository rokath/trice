# M18: Beispiele bei der späteren Funktionsabnahme prüfen

**Status:** als eigenständige Aufgabe zurückgestellt; Bestandteil der späteren M19/M20-Abnahme. **Alt:** L23. Aktuelle Planung: [Implementierungsplan](../../../Implementierungsplan.md).

Die vorhandene M01–M16-Bedienung ist in das [User Manual](../../../../TriceUserManual.md) übernommen. Die früheren Kontextbeispiele im auskommentierten UM-Abschnitt sind historische Referenz. Die [aktuellen deutschen Entwürfe](../../../README.md) enthalten geplante Syntax und sind keine schon ausführbare CLI-Anleitung.

## Bei der Implementierung und UM-Übernahme

- Jeden Block eindeutig als vorhandene Bedienung, geplante Syntax oder Schema kennzeichnen.
- C-Beispiele mit dem vereinbarten Makro-/Wrapperumfang kompilieren; lokale Scopes, einmalige Auswertung und abgeschaltetes Logging prüfen.
- Shell-Quoting der Formatstrings und C-Ausdrücke prüfen.
- JSON und `kv` gegen den vereinbarten Exportvertrag testen, einschließlich Escaping, mehrfacher Feldnamen und numerischer Grenzfälle.
- Reale CLI-Beispiele mit passenden Fixtures und Wörterbüchern ausführen; keine geplante Funktion allein für einen Dokumenttest implementieren.
- Bei Funktionsabnahme verständliche englische Bedienung ins UM übernehmen; keine zusätzliche Sammelkopie oder allgemeine Testinfrastruktur nötig.

Die heute ergänzte Dokumentation wurde gegen Code und vorhandene Tests gelesen. Das ist keine nachträgliche Behauptung, alle bisherigen C-, Hardware- oder Shell-Beispiele ausgeführt zu haben.
