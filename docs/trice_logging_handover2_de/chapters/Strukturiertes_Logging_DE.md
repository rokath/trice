<a id="sl-redaktion"></a>
**Redaktioneller Kommentar — nicht Teil des UM.** Der Entwurf erklärt den Nutzen anhand einer Messwertauswertung. Eine allgemeine strukturierte Trice-Schnittstelle ist noch nicht implementiert; vorhandene typisierte Visualisierungsdaten sind ein Anknüpfungspunkt. Vor einer verbindlichen API sind zwei Varianten zu vergleichen:

| Variante | Bedienung | Vorteil / Preis |
|---|---|---|
| A: benannte Platzhalter | `strice("info:Motor {motor_id}: {temperature_c} C", motor, temperature);` | Name steht bei der Verwendung; neue Syntax und sichere Typzuordnung erforderlich |
| B: vorhandene Aufrufe plus Feldzuordnung | `trice("info:Motor %d: %d C\n", motor, temperature);`, ergänzt um `arg0 → motor_id`, `arg1 → temperature_c` | Quellsyntax bleibt; separate Zuordnung muss Änderungen der Logstelle folgen |

Vorschlag: A als Bedienungsentwurf prüfen, B als Alternative behalten. Beide liefern dasselbe Datenmodell. Projektspezifische typisierte Wrapper können beide ergänzen; sie ersetzen nicht die Entscheidung. Wrapper können Quellposition, Codegröße und Argumentauswertung bei ausgeschaltetem Logging beeinflussen.

Ein konkreter Nutzen wäre ein Firmwaretest, der `temperature_c` über verschiedene Meldungen und Builds vergleicht, ohne Text zu parsen. [M19](../issues/M19_strukturierte_felder.md) verlangt zunächst ein nachvollziehbares Beispiel und die Entscheidung über Typen und Feldnamen. Die Parser-Neuorganisation aus UM 45.3 bleibt zurückgestellt.

Für einen Server, der Gerätedaten auswertet, reicht zunächst ein nutzbarer Host-Export. Einen nativen Serverlogger zu ersetzen ist eine andere Aufgabe mit Anforderungen an Parallelität, dynamische Daten und vorhandene Werkzeuge. Dafür wird hier kein neues System spezifiziert. Optionale numerische Feldschlüssel können auf dem Host entstehen; sie verlangen weder globale Target-IDs noch ein Target-Ereignis je Schlüssel.

Begriffliche Orientierung: [Go slog](https://pkg.go.dev/log/slog) trennt Meldung, Attribute und Ausgabe; [OpenTelemetry](https://opentelemetry.io/docs/specs/otel/logs/data-model/) trennt unter anderem Inhalt, Schweregrad und Attribute. Daraus folgt keine Pflicht, deren Datenmodell vollständig zu übernehmen.

---

## <a id="strukturiertes-logging"></a>Strukturiertes Logging

Beim strukturierten Logging bleiben Werte einzeln verfügbar. Ein Auswerteprogramm kann etwa `temperature_c > 80` prüfen, ohne den Satz „Motor 3: 87 C“ zu zerlegen. Ein lesbarer Meldungstext darf zusätzlich vorhanden sein.

JSON ist dafür nicht erforderlich. Auch ein binärer Datensatz mit bekanntem Schema kann strukturierte Informationen enthalten. Automatisch ergänzter Kontext ist ebenfalls optional.

### Vorhanden: ID und Werte als Grundlage

Trice überträgt bei gewöhnlichen ID-basierten Meldungen eine ID und die zugehörigen Werte. Das Wörterbuch liefert die Darstellung. Für die [Visualisierung](../../TriceUserManual.md#visualization-output-with--vis) gibt es einen begrenzten Pfad mit typisierten Zahlenwerten.

Eine allgemeine Benennung dieser Werte und der hier beschriebene Ausgabeablauf sind geplant.

### Geplant: Werte benennen und auswerten

Bedienungsentwurf mit Variante A, **keine derzeit aufrufbare API**:

```c
strice("info:Motor {motor_id}: {temperature_c} C", motor, temperature);
```

Du gibst jeden Wert einmal an. Die Vorlage nennt seine Bedeutung und liefert den lesbaren Text. Hier sind beide Werte als vorzeichenbehaftete 32-Bit-Zahlen vorgesehen; die verbindliche Syntax zur Festlegung beziehungsweise Prüfung der Typen ist noch offen.

Der Host könnte daraus erzeugen:

```json
{
  "tag": "info",
  "fields": {"motor_id": 3, "temperature_c": 87},
  "message": "Motor 3: 87 C"
}
```

Für die Auswertung verwendest du `fields.temperature_c`; für die Konsole `message`. Du kannst die Formulierung ändern, ohne die Feldabfrage ändern zu müssen. Eine zweite Meldungsart darf ebenfalls `temperature_c` liefern, sofern Typ, Einheit und Bedeutung zusammenpassen.

Der vorgesehene Ablauf ist:

1. Benenne die Werte, die du später einzeln auswerten möchtest.
2. Prüfe Namen, Typen, Einheiten und Argumentzuordnung beim Build.
3. Bewahre Werte unabhängig vom gerenderten Text auf.
4. Wähle auf dem Host lesbare Ausgabe oder eine maschinenlesbare Darstellung.

### Geplant: Tippfehler und Datenverluste vermeiden

Ein Wörterbuch kann gegen bereits geprüfte Namen und Typen vergleichen. Ein neuer Name wie `temperture_c` muss auffallen und geprüft werden; bloßes automatisches Eintragen in `til.json` erkennt keinen Tippfehler. Legitime neue Namen müssen ausdrücklich in den geprüften Feldbestand aufgenommen werden können. Die Bedienung dieser Freigabe ist noch festzulegen.

Der Build soll doppelte beziehungsweise widersprüchliche Felddefinitionen und eine falsche Zahl oder Art von Argumenten melden. Die Zuordnung bleibt unabhängig von der Formatierungsbreite. Ein auf eine Nachkommastelle gerundeter Konsolentext ersetzt nicht den ursprünglichen Zahlenwert.

Bei JSON-Ausgabe übernimmt ein Serializer Anführungszeichen, Backslashes und Steuerzeichen. Für sehr große Ganzzahlen und nicht endliche Gleitkommawerte braucht das Ausgabeformat eine ausdrückliche Regel; diese ist vor der Freigabe festzulegen. Ein frei formulierter printf-String ist keine Zusicherung gültigen JSONs.

### Geplant: mehrere Felder je Ereignis

Ein Aufruf bildet ein Ereignis, auch wenn sein Meldungstext Zeilenumbrüche enthält. Eine einzige ID darf mehrere Felder beschreiben:

```text
ID 4711: arg0 = motor_id, arg1 = temperature_c
ID 5822: arg0 = temperature_c, arg1 = fan_rpm
```

Der Host kann `temperature_c` in beiden Ereignistypen finden. Ein zusätzliches Target-Ereignis je Feld würde zusammengehörige Werte auseinanderreißen und ist nicht vorgesehen.

Die ID bezeichnet die Dekodierdefinition, nicht einen einzelnen Auftritt. Dieselbe ID kann tausendfach auftreten. Beim Zusammenführen mehrerer Geräte oder Wörterbücher wird zusätzlich die passende Quellen-/Wörterbuchkennung benötigt.

### Geplant: Kontext ergänzen und Meldungen auswählen

[Kontextanreicherung](Kontextanreicherung_DE.md#ce-zusammen) ergänzt denselben Datensatz, etwa um `ctx.task_id`. Benannte Nutzwerte funktionieren auch ohne Kontext; Kontext kann umgekehrt gewöhnliche Textmeldungen ergänzen.

Die [Tag- und Gewichtsauswahl](Log_Auswahl_DE.md#la-gewichte) gilt für das gesamte Ereignis. Ein anderes Ausgabeformat oder eine andere Farbe verändert die Auswahl nicht. Kontextfelder gehören zum angenommenen Ereignis und werden nicht nochmals einzeln gefiltert.

Unbekannte IDs oder beschädigte Daten dürfen keine scheinbar gültigen Datensätze mit erfundenen Nullwerten erzeugen. Werkzeugdiagnosen werden getrennt ausgegeben, damit ein maschinenlesbarer Datenstrom lesbar bleibt. Die binäre Rohaufzeichnung bleibt von diesen Ausgabefiltern unabhängig.
