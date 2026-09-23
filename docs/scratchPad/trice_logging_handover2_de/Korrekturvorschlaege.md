# Vorschläge für bestehende Dokumentation

Nur vorbereitete Ersatztexte; UM und Go-Help wurden nicht geändert. Deutsch ist die Arbeitssprache, direkt übernehmbare Texte sind auftragsgemäß Englisch. Geplante Funktionen dürfen erst nach Umsetzung als vorhanden dokumentiert werden.

## 1. UM 45.2: Kontextanreicherung richtig benennen

Fundstelle: [Trice Structured Logging](../TriceUserManual.md#trice-structured-logging), einschließlich der abgeleiteten Untertitel.

Vorgeschlagener Titel:

> Automatic Context Enrichment

Vorgeschlagener Einstieg:

> Context enrichment automatically adds selected build-time, runtime, or host-side information to log events. It can extend ordinary text messages or structured events. Structured logging preserves individually accessible values; enrichment describes how additional values are supplied.
>
> This section is a design draft. The proposed enrichment options are not yet implemented.

Den vorhandenen Anker bei einer späteren Integration als Kompatibilitätsanker erhalten; Überschrift und Linkbeschriftungen getrennt korrigieren.

## 2. ulabel: keine Doppelpunktliste versprechen

Fundstellen: [flagUserLabel](../../internal/args/init.go) und [eigene Tags im UM](../TriceUserManual.md#possibility-to-create-new-tags-without-modifying-trice-tool-source).

Vorbereiteter Help-Ersatz für die heutige Dokumentation:

> Register an additional user tag. Repeat -ulabel to add multiple tags, for example: -ulabel motor -ulabel sensor. Do not use colon-separated tag lists. The current implementation does not reliably resolve these new tags in -pick or -ban.

**Erst nach M08/M09**, bei bestätigtem Bereich `0..999`:

> Register a user tag or set a tag group's priority: -ulabel name[:weight]. Repeat the option for multiple tags. Weights are integers from 0 to 999; higher values mean higher priority. New tags without a weight inherit the final INFO weight. A known name without a weight leaves its group unchanged. Explicit weights apply to all aliases; the last explicit assignment wins. Colon-separated tag lists are invalid. Tags are registered before filter selectors are resolved.

## 3. logLevel: heutiges und geplantes Verhalten trennen

Fundstelle: [logInit](../../internal/args/init.go). Heute ausdrücklich auf den [Einfluss auf Zusatzspalten](chapters/Log_Auswahl_DE.md#la-heute-loglevel) hinweisen.

**Erst nach M11/M13**, für das neue Verhalten:

> Show application events whose priority is at least the given weight or the weight of the given tag. Use all to show all application events or off to suppress them. Tag selection and the priority threshold both apply; -pick and -ban remain mutually exclusive. An accepted event retains its enabled metadata. Events without a recognized tag use the reserved untagged group. Tool diagnostics are independent of application filters.

## 4. Kontext: Konfiguration ändern und sicher zurücknehmen

Fundstelle: [45.2.7](../TriceUserManual.md#trice-structured-logging-cli-switches-usage-options), Absatz über unveränderte `stf`/`stv` und manuelles Cache-Löschen.

Anforderung für den Draft:

> The transformation must retain enough provenance to restore the original call after configuration changes or interrupted runs. Bind must preserve user-written sources. Insert/clean must not depend on the previous enrichment configuration remaining unchanged. Cache validity must follow the effective inputs.

Damit wird ein gewünschter Vertrag beschrieben, keine vorhandene Implementierung behauptet.

## 5. Beispiele: konkrete Fehler beheben

Fundstellen: [45.2.6](../TriceUserManual.md#trice-structured-logging-user-defined-values), [45.2.7](../TriceUserManual.md#trice-structured-logging-cli-switches-usage-options) und [45.2.8](../TriceUserManual.md#trice-structured-logging-level-specific-configuration).

- Nach `#undef TRICE_ETC` ist das behauptete leere Stringisierungsergebnis falsch. Ersatzhinweis: **“After undefining TRICE_ETC, XSTR(TRICE_ETC) produces the text TRICE_ETC, not an empty string.”**
- Der generierte C-Aufruf in 45.2.7 beendet das Stringliteral nach `\n` mit einem einfachen statt doppelten Anführungszeichen.
- JSON-Vorlagen haben unter anderem eine falsche Begrenzung bei `line` und ungesicherte Meldungs-/Einheitentexte. Die spätere Ausgabe muss korrekt serialisiert werden; nicht nur einzelne Anführungszeichen flicken.
- In 45.2.8 stehen überzählige Backticks; ungeschützte Expansionen zerlegen zusammengesetzte Argumente. Die neue Fassung braucht geprüfte Argumentübergabe.

Englischer Hinweis für noch nicht ausführbare Beispiele:

> Illustrative proposal; not an executable command for the current Trice release. A printf template alone does not ensure valid JSON. Machine-readable output must serialize typed values and escape strings correctly.

[M18](issues/M18_beispiele_pruefen.md) erklärt den Sinn des alten L23: Es geht um konkrete Fehler, nicht um eine zusätzliche Logging-Funktion.

## 6. Kosten der Kontextanreicherung einordnen

Fundstelle: Byte-Rechnung und TCOBS-Aussage in [45.2.7](../TriceUserManual.md#trice-structured-logging-cli-switches-usage-options).

> Static metadata and field names can remain in the dictionary. Additional runtime values still require acquisition, encoding, and transport. For equal values and bit widths, field names need not add target payload bytes. Actual code size, execution cost, and framed size depend on the configuration; no fixed compression ratio is guaranteed.

Die Hash-Aussage in [45.2.2](../TriceUserManual.md#trice-structured-logging-runtime-information) ergänzen:

> A hash cannot reconstruct the original runtime string. Use it only when an identifier or comparison value is sufficient.

## 7. Servereinsatz: zwei Aufgaben auseinanderhalten

Fundstelle: [Using Trice on Servers](../TriceUserManual.md#using-trice-on-servers).

> A server can ingest and analyze Trice streams from devices. Using Trice as the server application's own logger is a separate use case and needs evidence of a practical benefit. Claims about speed, energy use, storage, and additional compression require measurements with equivalent retained information.

Mehrere Wörterbücher lassen sich über ihre Quellenkennung unterscheiden. Daraus folgt allein weder ein Bedarf für breitere Target-IDs noch für einen neuen Serverlogger.

## 8. Zurückgestellte und verworfene Themen

Der [Parserentwurf](../TriceUserManual.md#improving-the-trice-tool-internal-parser) bleibt separat und zurückgestellt. Die in [45.1](../TriceUserManual.md#trice-log-level-control-specification-draft) vorgeschlagenen zusätzlichen Compilezeit-Level-Makros und dynamischen Target-Level-Regler werden für dieses Vorhaben verworfen. Bestehende UM-Texte werden jetzt nicht entfernt; diese Entscheidung ist bei einer später ausdrücklich beauftragten Integration zu berücksichtigen.
