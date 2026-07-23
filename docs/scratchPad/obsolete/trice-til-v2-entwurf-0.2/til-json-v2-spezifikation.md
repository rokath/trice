# `til.json` Schema V2 – Spezifikationsentwurf

**Entwurfsstand 0.2 – 13. Juli 2026**

## 1. Ziel

Ein **Trice-kompatibler Decoder** soll einen binären Trice-Datenstrom mit genau einer zugehörigen `til.json` V2 vollständig interpretieren und beispielsweise in ASCII oder einen anderen Datenstrom übersetzen können. Als zusätzliche Eingaben sind nur die Selektoren `-profile` und `-stream-date` zulässig.

**Trice-kompatibel** bedeutet: Der Decoder implementiert die festen, im Trice-Projekt definierten Verfahren und Strukturen – insbesondere TREX, COBS/TCOBS, gegebenenfalls XTEA, die Typsemantik aus `specs/triceTypes.json` und das feste Counted-Buffer-Format. V2 liefert dazu alle **projektspezifischen und veränderlichen** Decoderwerte. V2 ist keine Beschreibung der Algorithmen selbst.

V2 gilt zunächst ausschließlich für binäre TREX-Datenströme. Die CLI-Modi `CHAR` und `DUMP` gehören nicht zu diesem Schema. Innerhalb einer Aufzeichnung wird ein unveränderter Konfigurationsstand vorausgesetzt; `-stream-date` wählt diesen Stand vor dem Decodieren aus.

## 2. Abgrenzung

In V2 gehören Werte, ohne die derselbe TREX-Bytestrom unterschiedlich interpretiert oder dargestellt würde:

- Framing und Single-/Multi-Paketmodus,
- verdoppelte 16-Bit-IDs,
- Target-Endianness,
- Default-Bitbreite unqualifizierter Trice-Typen,
- Encryption einschließlich optional gespeichertem Password oder Key,
- Formate für `ts0`, `ts16` und `ts32`,
- Behandlung von `typeX0`,
- Trice-ID, `Type` und `Strg`.

Nicht hinein gehören:

- Port, `-args`, Baudrate, Datenbits, Parität, Stopbits, RTT-, TCP-, UDP- oder Dateiquelle,
- Filter, Farben, PC-Zeitstempel, Delta-Spalten, `-showID`, `-unsigned`, `-addNL`, `Prefix` und `Suffix`,
- `li.json` und andere volatile Location Information,
- feste Algorithmus- und Bitstromdefinitionen.

`Prefix` und `Suffix` beeinflussen nur die Host-Darstellung, nicht die Interpretation des binären Trice-Stroms. Eine spätere optionale Ausgabespezifikation bleibt möglich.

Structured Logging hat zunächst keinen Einfluss auf `StreamFormat`. Spätere stabile Feldnamen, Einheiten oder Datentypen können optional in den jeweiligen `Trices`-Einträgen ergänzt werden.

## 3. V1 und V2

- Fehlt `SchemaVersion`, ist die Datei V1.
- `SchemaVersion: 2` kennzeichnet V2.
- Bestehende V1-Dateien bleiben V1 und werden als V1 zurückgeschrieben.
- Neu angelegte TIL-Dateien werden V2.
- Eine automatische V1→V2-Migration ist nicht vorgesehen.
- V2 verweist nicht auf eine separate V1-Datei; IDs und Decoderwerte bleiben in einer Datei.
- `-profile` und `-stream-date` sind bei V1 nicht verfügbar.
- Unbekannte Schema-Versionen führen zum Fehler.
- V2 setzt ein aktualisiertes Trice-Tool voraus; alte Tools erhalten keine V2-Schreibkompatibilität.

## 4. Top-Level-Struktur

```json
{
  "SchemaVersion": 2,
  "Profiles": ["default"],
  "StreamFormat": {},
  "Trices": {}
}
```

### 4.1 Profile

- `Profiles` enthält alle gültigen Profilnamen einschließlich `default`.
- Ohne `-profile` wird `default` verwendet.
- Profile wirken nur auf `StreamFormat`, nicht auf `Trices`.
- Hinzufügen, Umbenennen und Löschen erfolgt zunächst manuell im Editor.
- Profilnamen werden ohne Beachtung der Groß-/Kleinschreibung verglichen; die Schreibweise aus `Profiles` bleibt erhalten.
- Namen, die sich nur in der Groß-/Kleinschreibung unterscheiden, sind unzulässig.
- Ein unbekannter CLI-Profilname führt zum Fehler.
- Parameterwerte für ein aus `Profiles` gelöschtes Profil werden beim Lesen ignoriert und beim nächsten Schreiben nicht mehr ausgegeben. Sie sind kein harter Konsistenzfehler.

### 4.2 Parameterzentrierte Ablage

Jeder Parameter enthält unmittelbar seine Profilwerte; eine zusätzliche Ebene `Profiles` gibt es nicht:

```json
"Framing": {
  "default": {
    "Current": "TCOBSv1",
    "HistoryUntil": {}
  },
  "motor": {
    "Current": "COBS",
    "HistoryUntil": {
      "2026-06-01T00:00:00Z": null
    }
  }
}
```

Fehlt ein Nicht-Default-Profil bei einem Parameter, gilt der zeitabhängige Wert von `default`.

### 4.3 Trice-Liste

Die bisherigen Einträge bleiben semantisch erhalten und liegen unter `Trices`:

```json
"Trices": {
  "1000": {
    "Type": "TRICE16_3",
    "Strg": "x=%d y=%d z=%d\n"
  }
}
```

Die Bedeutung von `Type` wird nicht mehr nur implizit aus dem Namen hergeleitet, sondern durch `specs/triceTypes.json` festgelegt.

## 5. Zeitabhängige Werte

Ein zeitabhängiger Wert besteht aus `Current` und `HistoryUntil`:

```json
{
  "Current": "TCOBSv1",
  "HistoryUntil": {
    "2025-09-01T10:00:00Z": "none",
    "2026-04-18T08:30:00Z": "COBS"
  }
}
```

Bedeutung:

- vor `2025-09-01T10:00:00Z`: `none`
- ab dort bis vor `2026-04-18T08:30:00Z`: `COBS`
- ab `2026-04-18T08:30:00Z`: `TCOBSv1`

Für einen Auswahlzeitpunkt `D` wird der Wert am kleinsten Zeitpunkt `T` mit `D < T` verwendet; existiert kein solches `T`, gilt `Current`. Genau an `T` gilt bereits der nachfolgende Wert.

`null` ist ausschließlich in `HistoryUntil` eines Nicht-Default-Profils zulässig und bedeutet: Bis zu diesem Zeitpunkt den jeweiligen Wert von `default` erben. `Current` und das Profil `default` dürfen nicht `null` sein.

Historienzeitpunkte werden vollständig als UTC/RFC-3339 gespeichert:

```text
2026-06-15T07:45:00Z
```

`-stream-date` akzeptiert auch UTC-Präfixe. Fehlende Bestandteile werden mit dem frühestmöglichen Wert ergänzt:

```text
2026       -> 2026-01-01T00:00:00Z
2026-06    -> 2026-06-01T00:00:00Z
2026-06-15 -> 2026-06-15T00:00:00Z
```

Ohne `-stream-date` gilt `Current`.

## 6. CLI als Datenlieferant

### 6.1 Neue V2-Datei

Beim erstmaligen Erzeugen werden die eingebauten Tool-Defaults verwendet. Explizite CLI-Werte ersetzen die jeweiligen Defaults. Alle initialen `HistoryUntil`-Objekte sind leer.

### 6.2 Vorhandene V2-Datei

1. Nicht explizit angegebene Flags beziehen ihren Wert ausschließlich aus V2.
2. Explizite CLI-Werte werden zuerst geparst, fachlich geprüft und kanonisiert.
3. Entspricht der Wert dem wirksamen V2-Wert, geschieht nichts.
4. Weicht er ab, fragt `tlog` für **jeden Parameter einzeln** nach.
5. Werden alle Änderungen bestätigt, werden die bisherigen Werte historisiert und die neuen Werte dauerhaft gespeichert.
6. Wird eine Frage abgelehnt, wird der gesamte Änderungsvorgang ohne Schreibzugriff abgebrochen.
7. Ist keine interaktive Rückfrage möglich, führt eine Abweichung zum Fehler.

Beispiel:

```text
Profil "motor", Parameter "Framing"
til.json: TCOBSv1
CLI:      COBS
Dauerhaft übernehmen? [y/N]
```

Ein nur für diesen Lauf gültiger Override ist nicht vorgesehen. Skripte müssen CLI und V2 widerspruchsfrei festlegen.

Alle durch einen Aufruf bestätigten Änderungen erhalten denselben UTC-Änderungszeitpunkt. `-stream-date` ist eine reine historische Auswahl und darf nicht mit ändernden Streamparametern kombiniert werden.

### 6.3 Kanonisierung

- CLI-Kurzformen `us`, `µs` und `ms` werden vor dem Speichern in den tatsächlich verwendeten Zeitformatwert expandiert.
- `-ts` wird vor dem Speichern auf `ts0`, `ts16` und `ts32` aufgelöst.
- `TCOBS` und `TCOBSv1` sind CLI-Synonyme; gespeichert wird `TCOBSv1`.
- `typeX0=ignore` wird als `counted:ignore` gespeichert.
- `-showKey` ist ein Hilfsbefehl und selbst kein persistenter Parameter.

## 7. Persistente Parameter

| V2-Pfad | CLI-/Konfigurationsquelle | Kanonische Bedeutung |
|---|---|---|
| `Framing` | `-packageFraming` | `none`, `COBS`, `TCOBSv1` |
| `PackageMode` | `-singleFraming` | `single` oder `multi` |
| `Doubled16BitID` | `-doubled16BitID` | Boolean |
| `Endianness` | `-triceEndianness` | `littleEndian` oder `bigEndian` |
| `DefaultTRICEBitWidth` | `-defaultTRICEBitwidth` | `8`, `16`, `32`, `64` |
| `Encryption.Method` | Decoder-/Target-Konfiguration | `none` oder `XTEA` |
| `Encryption.Password` | `-password` | optionaler Klartextwert |
| `Encryption.Key` | abgeleiteter oder direkt gepflegter Key | optional; für XTEA 32 Hex-Zeichen ohne Trenner |
| `TimestampFormat.ts0` | `-ts0`, gegebenenfalls `-ts` | expandierter Formatwert |
| `TimestampFormat.ts16` | `-ts16`, gegebenenfalls `-ts` | expandierter Formatwert |
| `TimestampFormat.ts32` | `-ts32`, gegebenenfalls `-ts` | expandierter Formatwert |
| `TypeX0` | `-typeX0` | kanonische Behandlungsregel |

Für Encryption gilt:

- `Password` und `Key` sind unabhängig optional. Ein leerer String bedeutet, dass aktuell kein Wert gespeichert ist; historische Werte dürfen dabei erhalten bleiben.
- Wird eines der Felder manuell vollständig entfernt, darf es nicht automatisch aus dem anderen rekonstruiert und erneut geschrieben werden.
- Sind beide vorhanden, muss der aus `Password` abgeleitete Key mit `Key` übereinstimmen.
- Fehlen bei `Method: XTEA` beide Werte, ist die Datei für eine automatische Entschlüsselung unvollständig und der Decoder muss klar abbrechen.
- Historische Schlüssel bleiben für historische Binärlogs erhalten. Da V2 Klartext-Secrets enthalten kann, ist die Datei entsprechend zu schützen.
- Spätere veränderliche Parameter wie XTEA-`Rounds` werden erst aufgenommen, wenn dazu reale Target-CONFIG- und CLI-Werte existieren.

Das feste Binärformat von `typeX0` ist nicht Teil dieses Parameters. `TypeX0` legt nur das Decoder-/Ausgabeverhalten fest.

## 8. Feste Trice-Spezifikationen

Feste Semantik wird einmalig im Repository gehalten. Diese Dateien gehören zur Trice-Decoder-Distribution beziehungsweise können in den Decoder eingebaut werden; sie sind keine zusätzlichen projektspezifischen Eingaben:

```text
specs/
    TREX.json
    triceTypes.json
    CountedBuffer.json
    testdata/
```

- `TREX.json` beschreibt feste, deklarative Record- und Bitfeldstrukturen, nicht den Algorithmus als JSON-Programm.
- `triceTypes.json` enthält eine vollständige, gut lesbare Tabelle aller gültigen TIL-Typnamen mit Art, Bitbreite und Parameteranzahl. Für Typen wie `TRICE_3` verweist der Eintrag auf `DefaultTRICEBitWidth`; bei `TRICE16_3` steht die Breite explizit auf `16`. Die lange Tabelle darf aus kompakten Build-Time-Regeln generiert werden; Decoder werten jedoch die expliziten Einträge aus und benötigen keinen Template-Interpreter.
- `CountedBuffer.json` definiert das derzeit feste Selector-0-Format: ein 16-Bit-Wort, dessen untere 14 Bit (`0x3FFF`) die Anzahl der folgenden Nutzbytes enthalten; die oberen zwei Selector-Bits sind `0`.
- COBS, TCOBS und XTEA werden nicht in JSON nachimplementiert. Ihre Bedeutung wird durch den Trice-Kontext, die vorhandenen Go-/C-Implementierungen und gemeinsame Referenzvektoren festgelegt.

Beispielhafte Einträge:

```json
{
  "TRICE16_3": {"Kind": "Fixed", "BitWidth": 16, "ParameterCount": 3},
  "TRICE_3": {"Kind": "Fixed", "BitWidthFrom": "DefaultTRICEBitWidth", "ParameterCount": 3}
}
```

`triceTypes.json` wird initial durch den Vergleich von Go-Code unter `internal/`, C-Code unter `src/` und `_test/testdata/triceCheck.c` erstellt. Widersprüche sind explizit zu entscheiden; keine Implementierung gewinnt automatisch. Nach der Einführung ist die eingecheckte Tabelle normativ und beide Implementierungen werden dagegen geprüft.

Kleine, manuell prüfbare Referenzvektoren liegen unter `specs/testdata`. Umfangreiche Logs und Regressionstests bleiben unter `_test/testdata`.

## 9. Validierung

Ein V2-Reader muss mindestens prüfen:

- `SchemaVersion == 2`,
- Vorhandensein und Eindeutigkeit von `default`,
- Eindeutigkeit aller Profilnamen ohne Beachtung der Groß-/Kleinschreibung,
- vollständige konkrete Defaultwerte für alle obligatorischen Parameter; `Password` und `Key` dürfen fehlen,
- gültige UTC-Historienzeitschlüssel,
- gültige kanonische Parameterwerte,
- zulässige Parameterkombinationen, insbesondere die aktuelle Unverträglichkeit von `single` und Encryption,
- bekannte `Type`-Namen gemäß `specs/triceTypes.json`,
- Konsistenz von Password und Key, falls beide vorhanden sind.

## 10. Test- und Referenzprojekte

### 10.1 Binäre Regressionstests

Mindestens folgende vier Konfigurationen werden als Binärlogs geprüft:

1. COBS + single
2. COBS + multi
3. TCOBS + single
4. TCOBS + multi

Jedes Log mischt innerhalb desselben Datenstroms:

- `ts0`, `ts16` und `ts32`,
- gewöhnliche Trice-Typen verschiedener Breiten und Parameterzahlen,
- `typeX0`/CountedBuffer.

Die Tests prüfen die wirksamen CLI-/V2-Einstellungen, nicht getrennte Logs je Timestamp-Typ. Weitere gezielte Fälle prüfen Endianness, verdoppelte 16-Bit-IDs, Default-Bitbreite, Profile und Historie.

### 10.2 `examples/tlog-c`

Ein kleines Host-Miniprojekt erzeugt `triceLogC`:

- Eingabe: binäre Datei,
- Framing: nur COBS und TCOBS,
- Paketmodus: single und multi,
- Ausgabe: einfache deterministische ASCII-Zeilen,
- Wiederverwendung der vorhandenen C-RX-Komponenten, insbesondere `src/triceRx.c`,
- kein Netzwerk, RTT, XTEA, Filter, Farbe, Location Information oder vollständiger Go-CLI-Nachbau.

`triceLogC` ist zunächst kein V2-JSON-Reader. Es dient als kleine, nachvollziehbare C-Referenz und als Binary→ASCII-Konverter für Demo und Tests.

Für jedes Testlog müssen übereinstimmen:

```text
erwartete Ausgabe == Go trice log == triceLogC
```

### 10.3 TriceABC

`examples/TriceAbc` soll denselben erzeugten Binärstrom wahlweise durch

- den internen C-Empfangspfad,
- das Go-Tool `trice log`,
- `triceLogC`

sichtbar machen können. Dadurch werden Fehler in der Demo-Erzeugung oder -Integration erkennbar. BcSim bleibt dabei protokollneutral.

## 11. Abnahmekriterien

V2 ist für die erste Umsetzung akzeptiert, wenn:

- neue TIL-Dateien V2 sind und bestehende V1-Dateien unverändert V1 bleiben,
- Profile, parameterweise Historie und verkürzte UTC-Auswahl funktionieren,
- explizite abweichende CLI-Werte einzeln bestätigt werden,
- alle persistierten Parameter ohne zusätzliche projektspezifische CLI-Werte wiederhergestellt werden,
- `triceTypes.json` keine implizite Typbreite oder Parameterzahl im Decoder voraussetzt,
- die vier gemischten COBS/TCOBS-Single/Multi-Logs in Go und C identisch decodiert werden,
- TriceABC über mindestens einen externen Decoderpfad gegengeprüft wird.
