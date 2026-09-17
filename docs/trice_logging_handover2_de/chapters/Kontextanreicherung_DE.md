<a id="ce-redaktion"></a>
**Redaktioneller Kommentar — nicht Teil des UM.** Dies ist die korrigierte, gekürzte Arbeitskopie der Ideen aus [UM 45.2](../../TriceUserManual.md#trice-structured-logging), keine Änderung des UM. Kontextanreicherung und strukturierte Felder sind unabhängig nutzbar. Statische Angaben im Wörterbuch sind der einfachste Einstieg; zusätzliche Laufzeitwerte benötigen Target-Instrumentierung und kosten Erfassung sowie Nutzdaten. Für Bind ist deren Einbau durch erzeugte Artefakte noch zu entwerfen; ein längerer Eintrag in `til.json` allein kann keine Task-ID erfassen.

| Weg | Vorteil | Noch zu klären |
|---|---|---|
| Statischer Kontext und Darstellungsformat im Wörterbuch | Bind kann Anwendercode unverändert lassen | Historische Formatvarianten und Logstellenzuordnung |
| Laufzeitkontext über erzeugte Bind-Artefakte | Gemeinsame Bedienung für normale und strukturierte Aufrufe | Makro-/Argumenterweiterung, Herkunft, unterstützte Typen |
| Temporäre Instrumentierung mit Insert/Clean | Bestehender Ablauf und sichtbarer instrumentierter Code beim Debuggen | Rücknahme trotz geänderter Konfiguration und Unterbrechung |

**Vorschlag zur ID-Identität:** Wiederverwendung verlangt dieselbe Logstelle und dieselbe wirksame Definition aus Typen, Argumentreihenfolge, Vorlage und statischen Kontextwerten. Eine geänderte `-stf`-Vorlage ergibt eine andere Definition. Beim Zurückwechseln darf die frühere ID derselben Stelle wiederverwendet werden, sofern sie zur aktuellen ID-Policy passt. Gleicher Text an einer anderen Stelle erhält eine eigene ID. `li.json` kann helfen, aber die aktuelle Dateizeile allein ist kein dauerhafter Herkunftsnachweis. Die Speicherung bleibt Gegenstand von [M20](../issues/M20_kontext_vertrag.md).

Die Spezialidee aus [45.2.6](../../TriceUserManual.md#trice-structured-logging-user-defined-values), lokale Makrowerte über Compilerdiagnosen einzusammeln, bleibt als spätere Option erhalten. Sie ist nicht der normale Bedienungsweg. Nach `#undef` ergibt die dortige Stringisierung den Token-Namen statt eines leeren Strings.

---

## <a id="kontextanreicherung"></a>Automatische Kontextanreicherung

Kontextanreicherung ergänzt Informationen, die du nicht an jeder Logstelle wiederholen möchtest: etwa Quellposition, Firmwarekennung oder Task-ID. Du legst fest, welche Informationen aufgenommen werden und wie sie erscheinen.

### Vorhanden: Quellposition und Zeitstempel anzeigen

Trice kann bereits Datei und Zeile aus den zugehörigen Ortsinformationen sowie Host- und Targetzeitstempel anzeigen. Dafür verwendest du die Optionen für [Quellposition und IDs](../../TriceUserManual.md#trice-id-management) und [Zeitstempel](../../TriceUserManual.md#trice-timestamps).

Das ist ein Teil des gewünschten Nutzens. Die allgemeine Ergänzung frei ausgewählter Build- und Laufzeitinformationen ist geplant. Die im alten UM-Entwurf genannten `-stf`/`-stv` sind Entwurfssyntax, keine hier zugesicherte vorhandene CLI.

### Geplant: Informationen auswählen

| Information | Woher sie kommt | Aufwand |
|---|---|---|
| Datei, Funktion, Firmware-/Buildkennung | Build und Wörterbuch | Keine zusätzlichen Werte pro Target-Ereignis nötig |
| Host-Sitzung, Empfangsquelle | Empfänger | Hostseitige Ergänzung |
| Task-ID, Core-ID, Laufzeitzähler | Target beim Logaufruf | Erfassen und übertragen |

Der Host kann einen nicht erfassten Laufzeitwert nicht nachträglich rekonstruieren. Ein Hash ersetzt den ursprünglichen Inhalt einer dynamischen Zeichenkette nicht.

Vorgesehener Ablauf:

1. Wähle für den Build statische Angaben und bei Bedarf zusätzliche Laufzeitwerte.
2. Begrenze teure Ergänzungen auf geeignete Logstellen oder Tag-Gruppen, beispielsweise Task-ID nur bei Fehlern. Die genaue Konfigurationssyntax ist noch offen.
3. Erzeuge die Bind-Artefakte beziehungsweise instrumentiere mit Insert und baue die Firmware.
4. Wähle auf dem Host die Darstellung der erfassten Informationen. Reine Host-Darstellung benötigt keinen neuen Firmware-Build.

Zusätzliche Ausdrücke werden pro Ereignis genau einmal ausgewertet. Sie müssen auch im jeweiligen Interrupt-/Fehlerkontext zulässig sein. Eine weitere Sensormessung nur für den Kontext kann deutlich teurer sein als der ursprüngliche Logaufruf.

### Geplant: Bind oder Insert/Clean verwenden

Bei Bind bleibt der selbst geschriebene Trice-Aufruf erhalten; Ergänzungen entstehen in generierten Artefakten und Wörterbucheinträgen. Statischer Kontext kann im Wörterbuch liegen. Für dynamischen Kontext muss der erzeugte Targetcode die zusätzlichen Werte tatsächlich erfassen.

Insert/Clean bleibt als Alternative vorgesehen. `clean` muss den ursprünglichen Aufruf auch dann wiederherstellen können, wenn sich die Kontextkonfiguration geändert hat. Dafür wird die ursprüngliche Form nachvollziehbar aufbewahrt; die Rücknahme darf nicht von unveränderten `-stf`-/`-stv`-Angaben abhängen.

Nach einem abgebrochenen Lauf muss erneutes Ausführen sicher fortsetzen oder einen verständlichen Fehler melden. Cache-Erneuerung richtet sich nach den wirksamen Eingaben; manuelles Cache-Löschen ist keine Voraussetzung für Korrektheit.

### Geplant: Konfiguration ändern und alte Logs weiter lesen

Eine veränderte Erfassung oder Build-Vorlage benötigt neue passende Artefakte und gegebenenfalls eine andere ID:

| Änderung | Verhalten im vorgeschlagenen Vertrag |
|---|---|
| Identische Logstelle und Definition erneut bearbeiten | ID beibehalten, sofern zur aktuellen Policy passend |
| Build-Vorlage, statischen Kontext oder Argumentlayout ändern | Andere Definition zuordnen; bisherige Zuordnung erhalten |
| Zu einer früheren Definition derselben Stelle zurückkehren | Historische ID wiederverwenden, wenn eindeutig und policykonform |
| Derselbe Text an einer anderen Logstelle | Eigene ID für deren Herkunft verwenden |
| Nur Host-Spalten anders anordnen | Erfassung und ID unverändert lassen |

Historische Einträge werden nicht überschrieben. Das passende Wörterbuch muss die vom jeweiligen Firmwarestand übertragene ID weiterhin richtig deuten. Reicht der ID-Bereich nicht aus, meldet das Werkzeug einen Fehler und darf keine historische ID für eine andere Definition verwenden.

### <a id="ce-zusammen"></a>Geplant: mit strukturiertem Logging kombinieren

Ohne strukturierte Felder kann Kontext als zusätzlicher Text erscheinen. Mit [strukturiertem Logging](Strukturiertes_Logging_DE.md#strukturiertes-logging) bleiben Nutzwerte und ergänzte Angaben getrennt auswertbar.

Beispiel des gemeinsamen Zielbilds, **keine vorhandene Ausgabeoption**:

```json
{
  "tag": "info",
  "fields": {"motor_id": 3, "temperature_c": 87},
  "ctx": {"task_id": 7, "uptime_us": 123456, "file": "motor.c"},
  "message": "Motor 3: 87 C"
}
```

`fields` enthält die ausdrücklich geloggten Werte; `ctx` die automatisch ergänzten Informationen. Die Task-ID wird nicht nochmals als manuelles Logargument benötigt. Kontext darf kein gleichnamiges Nutzfeld überschreiben.

Für dieselben übertragenen Werte und Bitbreiten benötigen Feldnamen keine zusätzlichen Target-Nutzdaten. Die zusätzlich erfassten Task- und Laufzeitwerte benötigen dagegen Platz und Rechenzeit. JSON ist nur eine mögliche Host-Ausgabe.
