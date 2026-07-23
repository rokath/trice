# Spezifikationsentwurf: `til.json` Schema V2

**Selbstbeschreibende Interpretation binärer Trice-Datenströme**  
Entwurfsstand: 0.1 — 11. Juli 2026

## 1. Ziel

Eine `til.json` im Schema V2 soll zusammen mit einem binären Trice-Datenstrom ausreichen, um den Datenstrom ohne weitere Konfigurationsdateien oder nicht dokumentierte Target-Kenntnisse vollständig zu interpretieren. Bei mehreren Profilen oder historischen Konfigurationsständen dürfen `-profile` und `-stream-date` als reine Selektoren hinzukommen; sämtliche dadurch ausgewählten Decodierwerte stammen weiterhin aus der `til.json`.

„Vollständig interpretieren“ bedeutet hier:

1. den binären Eingang korrekt zu entframen und gegebenenfalls zu entschlüsseln,
2. die enthaltenen Trice-Datensätze strukturell korrekt zu dekodieren,
3. Trice-IDs über `Type` und `Strg` aufzulösen,
4. Target-Zeitstempel semantisch und textuell zu interpretieren,
5. daraus einen anderen Datenstrom erzeugen zu können, beispielsweise den heutigen ASCII-Ausgabestrom von `tlog` beziehungsweise `trice log`.

Nicht Ziel ist es, die physische oder logische Verbindung zum Gerät zu beschreiben. Kommunikationsparameter bleiben außerhalb der `til.json`. Ebenfalls nicht aus dem Binärstrom ableitbar sind die gewünschte Profilauswahl und, bei einem undatierten Archivlog, der historische Auswahlzeitpunkt. Ohne entsprechende CLI-Angabe gelten das Profil `default` und die aktuellen Werte.

Im Folgenden bezeichnet `tlog` den Log-/Decoderpfad des Trice-Tools, derzeit aufgerufen als `trice log`.

## 2. Grundlage und Abgrenzung

Der Entwurf berücksichtigt die im Abschnitt `l|log` von `docs/ref/trice-help-all.txt` vorhandenen Optionen. Für die Interpretation eines binären Trice-Datenstroms relevant sind insbesondere:

- `-encoding`
- `-packageFraming`
- `-singleFraming`
- `-doubled16BitID`
- `-triceEndianness`
- `-defaultTRICEBitwidth`
- `-password`
- `-ts`, `-ts0`, `-ts16`, `-ts32`
- `-typeX0`

Die bestehende Trice-/TREX-Bitstromspezifikation wird durch dieses Dokument nicht ersetzt. Schema V2 beschreibt die Konfigurationswerte, die ein Decoder zusätzlich zum Datenstrom benötigt.

## 3. Normative Begriffe

Die Begriffe **MUSS**, **DARF NICHT**, **SOLLTE** und **DARF** sind normativ zu verstehen.

- **V1** bezeichnet das bisherige `til.json`-Format mit den ID-Einträgen direkt auf der obersten JSON-Ebene.
- **V2** bezeichnet das in diesem Dokument definierte Format mit `SchemaVersion: 2`.
- **Profil** bezeichnet eine benannte Menge von Abweichungen der Streamkonfiguration. Profile wirken nicht auf die Trice-ID-Liste.
- **Current** bezeichnet den neuesten Wert eines Parameters.
- **HistoryUntil** enthält frühere Werte und den Zeitpunkt, bis zu dem der jeweilige Wert gültig war.

## 4. Grundentscheidungen

1. Bestehende V1-Dateien bleiben V1.
2. Es gibt keine automatische V1-zu-V2-Migration.
3. Neu angelegte `til.json`-Dateien werden als V2 angelegt.
4. Eine V2-Datei wird nie als V1 zurückgeschrieben.
5. Trice-IDs sind nicht profilabhängig.
6. Streamparameter werden parameterzentriert gespeichert. Ein Parameter enthält direkt die Profile, für die er vom Default abweicht.
7. Die Historie wird je Parameter geführt, nicht als vollständige Konfigurationskopie.
8. Kommunikationsparameter werden nicht gespeichert.
9. Explizit angegebene CLI-Werte sind Datenlieferanten für die V2-Einstellungen und werden als neue aktuelle Werte gespeichert.
10. Nicht explizit angegebene CLI-Defaults dürfen eine vorhandene V2-Datei nicht überschreiben.

## 5. Top-Level-Struktur

Eine V2-Datei hat folgende Grundform:

```json
{
  "SchemaVersion": 2,
  "Profiles": [
    "default"
  ],
  "StreamFormat": {
  },
  "Trices": {
  }
}
```

### 5.1 `SchemaVersion`

- `SchemaVersion` MUSS eine Ganzzahl sein.
- Für dieses Dokument MUSS der Wert `2` sein.
- Fehlt `SchemaVersion` in einer nichtleeren bisherigen TIL, wird die Datei als V1 behandelt.
- Eine unbekannte oder höhere Schema-Version MUSS mit einem eindeutigen Fehler abgelehnt werden.

### 5.2 `Profiles`

`Profiles` ist die vollständige Liste der zulässigen Profilnamen.

```json
"Profiles": [
  "default",
  "sensor-board",
  "motor-controller"
]
```

Regeln:

- `default` MUSS explizit enthalten sein.
- `default` ist reserviert und darf nicht umgedeutet werden.
- Profilnamen müssen eindeutig und nicht leer sein.
- Profilnamen sind case-sensitive.
- Hinzufügen, Umbenennen und Löschen von Profilen erfolgt zunächst ausschließlich durch manuelle Bearbeitung der `til.json`.
- `tlog` DARF ein unbekanntes Profil nicht automatisch anlegen.
- Ein mit `-profile` ausgewählter, nicht deklarierter Name MUSS einen Fehler verursachen.
- Parameter-Einträge für nicht deklarierte Profile sind ein Konsistenzfehler.

### 5.3 `Trices`

`Trices` enthält die bisherigen V1-ID-Einträge unverändert in ihrer bisherigen Bedeutung:

```json
"Trices": {
  "1000": {
    "Type": "TRICE32_1",
    "Strg": "temperature=%d\n"
  },
  "1001": {
    "Type": "TRICE16_2",
    "Strg": "x=%d y=%d\n"
  }
}
```

Die ID-Schlüssel sind dezimale JSON-Strings. `Type` und `Strg` behalten die bestehende Trice-Semantik. Profile wirken ausdrücklich nicht auf diesen Abschnitt.

## 6. Allgemeines Format eines zeitabhängigen Parameters

Jeder Streamparameter ist ein Objekt, dessen Schlüssel Profilnamen sind:

```json
"Framing": {
  "default": {
    "Current": "TCOBSv1",
    "HistoryUntil": {
      "2025-09-01T10:00:00Z": "none",
      "2026-04-18T08:30:00Z": "COBS"
    }
  },
  "motor-controller": {
    "Current": "COBS",
    "HistoryUntil": {
      "2026-06-01T00:00:00Z": null
    }
  }
}
```

### 6.1 Bedeutung von `HistoryUntil`

Die Zeitstempel sind exklusive obere Gültigkeitsgrenzen.

Das Beispiel für `default` bedeutet:

- vor `2025-09-01T10:00:00Z`: `none`
- ab `2025-09-01T10:00:00Z` und vor `2026-04-18T08:30:00Z`: `COBS`
- ab `2026-04-18T08:30:00Z`: `TCOBSv1`

Für einen Auswahlzeitpunkt `D` gilt:

1. Wähle den kleinsten `HistoryUntil`-Zeitpunkt `T` mit `D < T`.
2. Existiert ein solcher Zeitpunkt, ist der dort gespeicherte Wert gültig.
3. Existiert kein solcher Zeitpunkt, ist `Current` gültig.
4. Genau am Zeitpunkt `T` gilt bereits der danach eingeführte Wert.

### 6.2 Änderung eines Wertes

Wird ein Parameter zum Zeitpunkt `T` von `A` auf `B` geändert, wird atomar:

1. der bisherige `Current`-Wert als `HistoryUntil[T] = A` gespeichert,
2. `Current` auf `B` gesetzt.

Wird derselbe bereits explizit gespeicherte Wert erneut angegeben, wird kein Historieneintrag erzeugt. Eine Ausnahme ist die erstmalige Festlegung eines Nicht-Default-Profils: War der Wert bisher nur geerbt, erzeugt eine explizite CLI-Angabe einen eigenen Profilwert auch dann, wenn er momentan dem Default entspricht. Dadurch bleibt das Profil bei späteren Default-Änderungen bewusst auf diesem Wert fixiert.

Alle durch einen einzelnen CLI-Aufruf ausgelösten Änderungen verwenden denselben einmalig ermittelten UTC-Zeitstempel.

### 6.3 Profilvererbung und `null`

Fehlt ein Profil bei einem Parameter vollständig, erbt es den zeitabhängigen Wert von `default`.

Bei Nicht-Default-Profilen bedeutet der Timeline-Wert `null` ebenfalls: **den zu diesem Zeitpunkt gültigen Wert des Profils `default` verwenden**.

Damit lässt sich eine Profilabweichung historisch korrekt beginnen oder beenden, ohne die Default-Historie zu kopieren.

Beispiel:

```json
"motor-controller": {
  "Current": "COBS",
  "HistoryUntil": {
    "2026-06-01T00:00:00Z": null
  }
}
```

Bedeutung:

- vor Juni 2026: zeitabhängige Vererbung von `default`
- ab Juni 2026: explizit `COBS`

Für das Profil `default` ist `null` unzulässig. Es gibt keine Vererbung zwischen benutzerdefinierten Profilen und damit keine Vererbungsketten oder Zyklen.

## 7. UTC-Zeitformat und Datumsauswahl

### 7.1 Speicherung in der Datei

Historienzeitpunkte werden als RFC-3339-Zeitstempel in UTC gespeichert:

```text
YYYY-MM-DDTHH:MM:SSZ
```

Optional sind Sekundenbruchteile zulässig:

```text
YYYY-MM-DDTHH:MM:SS.sssssssssZ
```

Ein Writer SOLLTE die Schlüssel chronologisch aufsteigend ausgeben. Die JSON-Objektreihenfolge ist semantisch jedoch ohne Bedeutung.

### 7.2 Auswahl über das CLI

Der neue Schalter lautet in diesem Entwurf:

```text
-stream-date=<UTC-Zeitpunkt oder UTC-Präfix>
```

Ohne `-stream-date` wird `Current` verwendet.

Zulässige verkürzte Angaben sind beispielsweise:

```text
2026
2026-06
2026-06-15
2026-06-15T12
2026-06-15T12:30
2026-06-15T12:30:45Z
```

Fehlende Bestandteile werden mit dem frühestmöglichen Wert ergänzt. Damit gilt beispielsweise:

```text
2026-06  ==  2026-06-01T00:00:00Z
```

und:

```text
2026-06-15  ==  2026-06-15T00:00:00Z
```

Verkürzte Angaben sind UTC-Angaben. Vollständige gespeicherte Historienzeitpunkte werden immer kanonisch mit `Z` geschrieben.

## 8. Persistente Streamparameter

Alle folgenden Parameter MÜSSEN für `default` vorhanden sein. Nicht-Default-Profile enthalten nur Abweichungen.

### 8.1 `Encoding`

Zuordnung zum CLI:

```text
-encoding, -e
```

Aktuelle kanonische Werte:

```text
TREX
CHAR
DUMP
```

Für die in diesem Entwurf betrachtete binäre, ID-codierte Trice-Interpretation ist `TREX` der reguläre Wert. `CHAR` und `DUMP` bleiben als bestehende Eingangs-/Darstellungsmodi repräsentierbar, erzeugen jedoch nicht dieselbe typisierte ID-Auflösung.

### 8.2 `Framing`

Zuordnung zum CLI:

```text
-packageFraming, -pf
```

Aktuelle kanonische Werte:

```text
none
COBS
TCOBSv1
```

### 8.3 `PackageMode`

Zuordnung zum CLI:

```text
-singleFraming=true   -> single
-singleFraming=false  -> multi
```

Kanonische Werte:

```text
single
multi
```

`PackageMode` beschreibt, ob ein Frame höchstens einen oder mehrere Trice-Datensätze enthalten darf. Der Name vermeidet die irreführende Vermischung mit dem eigentlichen Framing-Verfahren.

### 8.4 `Doubled16BitID`

Zuordnung zum CLI:

```text
-doubled16BitID, -d16
```

Werttyp: Boolean.

Der Parameter bedeutet **verdoppelte 16-Bit-ID-Darstellung** und nicht mehrfach vergebene oder duplizierte Einträge in der Trice-ID-Liste.

### 8.5 `Endianness`

Zuordnung zum CLI:

```text
-triceEndianness
```

Kanonische Werte:

```text
littleEndian
bigEndian
```

### 8.6 `DefaultTRICEBitWidth`

Zuordnung zum CLI:

```text
-defaultTRICEBitwidth
```

Werttyp: Ganzzahl.

Zulässige Werte:

```text
8
16
32
64
```

Der Wert muss zur Target-Einstellung `TRICE_DEFAULT_PARAMETER_BIT_WIDTH` passen.

### 8.7 `Encryption`

Zuordnung zum CLI:

```text
-password, -pw
```

`Encryption` wird als zusammengehöriger Wert historisiert, damit Verfahren und Schlüssel nicht zeitlich auseinanderlaufen können:

```json
{
  "Method": "XTEA",
  "Key": "00112233445566778899AABBCCDDEEFF"
}
```

Zunächst definierte Verfahren:

```text
none
XTEA
```

Regeln:

- Für `XTEA` MUSS `Key` einen 128-Bit-Schlüssel als 32 Hexadezimalzeichen enthalten.
- Ein Writer SOLLTE Großbuchstaben ohne Trennzeichen und ohne `0x` verwenden.
- Für `none` darf `Key` fehlen oder ein Leerstring sein.
- Ein leerer oder fehlender Schlüssel bei `XTEA` ist ein Fehler.
- `tlog` speichert nicht zwingend die Passphrase. Wird `-password` verwendet, wird der daraus abgeleitete, tatsächlich zum Decodieren benötigte Schlüssel gespeichert.
- `-showKey` ist ein Hilfsschalter und kein persistenter Parameter.
- Ein explizites leeres Passwort setzt die Konfiguration auf `Method: "none"` und einen leeren beziehungsweise nicht vorhandenen Schlüssel.

Das Speichern des Decryption Keys in der TIL ist eine bewusste Komfortentscheidung für die Entwicklung. Wer den Schlüssel nicht speichern möchte, kann ihn leeren oder entfernen. Historische verschlüsselte Datenströme sind dann für die betroffenen Zeiträume nicht mehr allein mit dieser TIL decodierbar.

### 8.8 `TimestampFormat`

Zuordnung zum CLI:

```text
-ts
-ts0
-ts16
-ts32
```

Struktur:

```json
"TimestampFormat": {
  "ts0": {
  },
  "ts16": {
  },
  "ts32": {
  }
}
```

Die Werte sind die von `tlog` akzeptierten Formatstrings. Dazu gehören insbesondere:

- Leerstring zur Unterdrückung,
- `µs` beziehungsweise `us`,
- `ms`,
- benutzerdefinierte Formatstrings,
- bei `ts32` zusätzlich die bestehende `epoch...`-Notation.

`-ts` ist nur ein CLI-Kurzschalter. Er wird vor dem Speichern in die drei konkreten Werte `ts0`, `ts16` und `ts32` expandiert. Explizit angegebene `-ts0`, `-ts16` oder `-ts32` haben dabei Vorrang.

Die Target-Timestampformate werden gespeichert, weil der rohe 16- oder 32-Bit-Wert allein keine Einheit oder Epochensemantik enthält. Die Delta-Schalter `-ts0delta`, `-ts16delta` und `-ts32delta` bleiben dagegen Ausgabeoptionen: Sie verändern nicht die Bedeutung des Datenstroms und hängen zudem von Reihenfolge und Filterung der Ausgabe ab.

### 8.9 `TypeX0`

Zuordnung zum CLI:

```text
-typeX0
```

Der Wert verwendet zunächst die bestehende CLI-Syntax, beispielsweise:

```text
error
ignore
counted:ignore
all:ignore
counted:%s
```

Dieser Parameter gehört in die TIL, weil er bestimmt, wie Selector-0-/X0-Pakete semantisch behandelt werden. Für andere Encodings als `TREX` kann er wirkungslos sein.

## 9. Vollständiges Beispiel einer V2-Datei

Das folgende Beispiel ist gültiges JSON. Es zeigt drei Profile, sparse Profilabweichungen, Historien, zeitabhängige Default-Vererbung und einen gespeicherten XTEA-Schlüssel.

```json
{
  "SchemaVersion": 2,
  "Profiles": [
    "default",
    "sensor-board",
    "motor-controller"
  ],
  "StreamFormat": {
    "Encoding": {
      "default": {
        "Current": "TREX",
        "HistoryUntil": {}
      }
    },
    "Framing": {
      "default": {
        "Current": "TCOBSv1",
        "HistoryUntil": {
          "2025-09-01T10:00:00Z": "none",
          "2026-04-18T08:30:00Z": "COBS"
        }
      },
      "motor-controller": {
        "Current": "COBS",
        "HistoryUntil": {
          "2026-06-01T00:00:00Z": null
        }
      }
    },
    "PackageMode": {
      "default": {
        "Current": "multi",
        "HistoryUntil": {}
      },
      "motor-controller": {
        "Current": "single",
        "HistoryUntil": {}
      }
    },
    "Doubled16BitID": {
      "default": {
        "Current": false,
        "HistoryUntil": {}
      },
      "motor-controller": {
        "Current": true,
        "HistoryUntil": {
          "2026-03-01T00:00:00Z": null
        }
      }
    },
    "Endianness": {
      "default": {
        "Current": "littleEndian",
        "HistoryUntil": {}
      },
      "motor-controller": {
        "Current": "bigEndian",
        "HistoryUntil": {}
      }
    },
    "DefaultTRICEBitWidth": {
      "default": {
        "Current": 32,
        "HistoryUntil": {}
      },
      "sensor-board": {
        "Current": 16,
        "HistoryUntil": {}
      }
    },
    "Encryption": {
      "default": {
        "Current": {
          "Method": "none",
          "Key": ""
        },
        "HistoryUntil": {}
      },
      "sensor-board": {
        "Current": {
          "Method": "XTEA",
          "Key": "00112233445566778899AABBCCDDEEFF"
        },
        "HistoryUntil": {
          "2026-02-01T14:00:00Z": null,
          "2026-06-15T07:45:00Z": {
            "Method": "XTEA",
            "Key": "FFEEDDCCBBAA99887766554433221100"
          }
        }
      }
    },
    "TimestampFormat": {
      "ts0": {
        "default": {
          "Current": "time:            ",
          "HistoryUntil": {}
        }
      },
      "ts16": {
        "default": {
          "Current": "ms",
          "HistoryUntil": {
            "2026-03-01T12:00:00Z": "µs"
          }
        }
      },
      "ts32": {
        "default": {
          "Current": "ms",
          "HistoryUntil": {}
        },
        "sensor-board": {
          "Current": "epoch\"2006-01-02T15:04:05Z07:00\"",
          "HistoryUntil": {
            "2026-05-01T00:00:00Z": null
          }
        }
      }
    },
    "TypeX0": {
      "default": {
        "Current": "error",
        "HistoryUntil": {}
      },
      "sensor-board": {
        "Current": "counted:%s",
        "HistoryUntil": {}
      }
    }
  },
  "Trices": {
    "1000": {
      "Type": "TRICE32_1",
      "Strg": "temperature=%d\n"
    },
    "1001": {
      "Type": "TRICE16_2",
      "Strg": "x=%d y=%d\n"
    },
    "1002": {
      "Type": "TRICE_S",
      "Strg": "state=%s\n"
    }
  }
}
```

## 10. Auflösung eines Profils

Die Auflösung erfolgt für jeden Parameter einzeln.

Für Profil `P` und Zeitpunkt `D`:

1. Existiert kein Eintrag für `P`, löse `default` zum selben Zeitpunkt `D` auf.
2. Existiert ein Eintrag für `P`, löse dessen Timeline zum Zeitpunkt `D` auf.
3. Ergibt die Profil-Timeline `null`, löse `default` zum selben Zeitpunkt `D` auf.
4. Ein `null` aus `default` ist ungültig.
5. Nach Auflösung aller Parameter wird die Gesamtkonfiguration validiert.

Ohne CLI-Profil gilt:

```text
-profile=default
```

Beispiele:

```text
trice log ...
```

verwendet das aktuelle Profil `default`.

```text
trice log -profile=sensor-board ...
```

verwendet die aktuellen Werte von `sensor-board` und ansonsten die aktuellen Default-Werte.

```text
trice log -profile=sensor-board -stream-date=2026-06 ...
```

verwendet die am `2026-06-01T00:00:00Z` gültigen Sensor- und Default-Werte.

## 11. CLI als Datenlieferant und Persistenzquelle

### 11.1 Priorität

Für eine V2-Datei gilt folgende Priorität:

1. explizit auf der Kommandozeile angegebener schemarelevanter Wert,
2. aus Profil und Historie aufgelöster Wert aus der V2-Datei,
3. eingebaute Tool-Defaults ausschließlich beim erstmaligen Erzeugen einer neuen V2-Datei. Eine bereits vorhandene, unvollständige V2-Datei wird nicht still repariert, sondern als ungültig abgelehnt.

Ein gewöhnlicher CLI-Default ist nicht mit einer expliziten Benutzereingabe gleichzusetzen. Die Implementierung MUSS daher erkennen können, welche Flags tatsächlich in der Kommandozeile vorkamen.

### 11.2 Persistentes Update

Wird bei einer V2-Datei ohne `-stream-date` ein schemarelevanter CLI-Wert explizit angegeben, dann:

1. wird er kanonisiert,
2. gegen den aktuell explizit gespeicherten beziehungsweise geerbten Wert des ausgewählten Profils verglichen,
3. bei einer Änderung mit einem gemeinsamen aktuellen UTC-Zeitstempel historisiert,
4. als neuer `Current`-Wert gespeichert,
5. für den unmittelbar anschließenden Decoderlauf verwendet.

Für ein Nicht-Default-Profil, das bisher nur geerbt hat, wird beim ersten expliziten Override `null` als vorheriger Wert historisiert. Das gilt auch dann, wenn der explizit angegebene Wert momentan gleich dem geerbten Default-Wert ist: Die CLI-Angabe fixiert damit bewusst einen eigenen Profilwert. Nur die Wiederholung eines bereits explizit gespeicherten identischen Wertes bleibt ohne Änderung.

Beispiel:

```text
trice log -profile=sensor-board -packageFraming=COBS
```

legt bei Bedarf einen Profil-Eintrag unter `StreamFormat.Framing.sensor-board` an und macht `COBS` zum neuen aktuellen Wert dieses Profils.

### 11.3 Historische Auswahl ist read-only

Bei Verwendung von `-stream-date` wird die TIL nur gelesen. Schemarelevante Änderungsflags dürfen in diesem Modus nicht gleichzeitig als persistente Updates verwendet werden.

Zur Vermeidung stiller Fehlbedienung SOLL `tlog` die Kombination von `-stream-date` mit schemaändernden CLI-Flags ablehnen.

### 11.4 Spezielle CLI-Abbildungen

- `-ts` wird in `ts0`, `ts16` und `ts32` expandiert; explizite Einzelwerte gewinnen.
- `-singleFraming=true` speichert `single`.
- `-singleFraming=false` speichert `multi`.
- `-doubled16BitID=true|false` speichert einen Boolean.
- `-password=<Wert>` speichert die normalisierte Encryption-Konfiguration mit abgeleitetem Schlüssel.
- CLI-Aliase wie `-pf`, `-e`, `-d16` oder `-pw` erscheinen nicht im JSON; gespeichert wird nur der kanonische Parameter.

### 11.5 Atomare Dateiänderung

Ein Writer MUSS die gesamte geänderte JSON-Datei zunächst vollständig erzeugen und validieren. Danach wird sie atomar ersetzt, beispielsweise über eine temporäre Datei und Rename.

Unveränderte TIL-Bereiche, insbesondere `Trices`, unbekannte zulässige Zusatzfelder und Historien anderer Profile, dürfen nicht verloren gehen.

Alle zusammengehörigen Änderungen eines CLI-Aufrufs verwenden denselben Historienzeitpunkt.

## 12. Decoderverhalten

Für einen Decoderlauf wird die Streamkonfiguration einmal zu Beginn vollständig aufgelöst und validiert. Der binäre Datenstrom selbst wird während dieses Laufs als unverändert konfiguriert betrachtet.

Änderungen an `Trices` dürfen wie bisher dynamisch neu geladen werden, sofern `tlog` dies bereits unterstützt. Änderungen an der ausgewählten `StreamFormat`-Konfiguration sollen erst bei einem neuen Decoderlauf wirksam werden. Dadurch entsteht kein impliziter Streamformatwechsel mitten in einem Frame oder Log.

Die konzeptionelle Verarbeitung ist:

1. Bytes über einen beliebigen Transport beziehen.
2. Frames entsprechend `Framing` erkennen und decodieren.
3. Nutzdaten entsprechend `Encryption` entschlüsseln.
4. Paketinhalt entsprechend `PackageMode` in Trice-Datensätze zerlegen.
5. IDs, Werte und Typinformationen unter Beachtung von `Encoding`, `Endianness`, `Doubled16BitID` und `DefaultTRICEBitWidth` decodieren.
6. X0-Daten entsprechend `TypeX0` behandeln.
7. Trice-ID in `Trices` nachschlagen und mit `Type` und `Strg` formatieren.
8. Target-Zeitstempel entsprechend `TimestampFormat` interpretieren beziehungsweise für den ASCII-Ausgabestrom formatieren.
9. Das Ergebnis als ASCII oder in ein anderes Zielformat übersetzen.

Die exakten Algorithmen und die Protokolldetails bleiben durch die bestehende Trice-/TREX-Spezifikation definiert.

## 13. Nicht in `til.json` gespeicherte CLI-Optionen

### 13.1 Kommunikation und Datenquelle

Diese Parameter bestimmen, wie Bytes zu `tlog` gelangen, nicht wie der empfangene Bytestrom interpretiert wird:

- `-port`, `-p`
- `-args`
- `-baud`
- `-databits`
- `-parity`
- `-stopbits`
- TCP-/UDP-Endpunkte
- J-LINK-/ST-LINK-/RTT-Parameter
- `-exec`

Dieselben Bytes müssen unabhängig davon gleich decodiert werden, ob sie aus UART, RTT, TCP, UDP, einer Datei oder einem Testpuffer stammen.

### 13.2 Ausgabeziele und Laufzeitsteuerung

Nicht gespeichert werden unter anderem:

- `-logfile`
- `-binaryLogfile`
- `-tcp`
- `-displayserver`
- `-stat`, `-triceStat`, `-tagStat`
- `-debug`, `-showInputBytes`, `-verbose`

### 13.3 Reine Darstellung und Filterung

Nicht Bestandteil der Streaminterpretation sind unter anderem:

- `-hs` für den PC-Empfangszeitpunkt
- `-prefix`, `-suffix`
- `-color`
- `-showID`
- `-addNL`
- `-newlineIndent`
- `-unsigned`
- `-ts0delta`, `-ts16delta`, `-ts32delta`
- `-pick`, `-ban`, `-logLevel`, `-ulabel`
- `-noCycleCheck`
- `-dc`
- `-testTable`

Diese Optionen dürfen weiterhin den erzeugten ASCII-Strom verändern, sind aber keine Target- oder Codierungseigenschaften.

### 13.4 Location Information

`li.json`, `-locationInformation`, `-liFmt` und `-liPath` bleiben außerhalb des Schemas. Quellpfad und Zeilennummer sind nicht im binären Trice-Datenstrom enthalten und nicht zur inhaltlichen Decodierung erforderlich.

## 14. V1-/V2-Kompatibilität

### 14.1 Erkennung

- Nichtleere Datei ohne `SchemaVersion` und mit bisherigen Top-Level-ID-Einträgen: V1.
- Datei mit `SchemaVersion: 2`: V2.
- Nicht vorhandene, leere, nur aus Whitespace bestehende oder inhaltlich leere `{}`-Datei beim ersten Schreibzugriff eines V2-fähigen Tools: neue V2-Datei.

### 14.2 Verhalten bei V1

- V1 wird weiterhin gelesen.
- V1 wird weiterhin im bisherigen Format geschrieben.
- V1 wird nie automatisch umstrukturiert.
- Schema-V2-Streamwerte bleiben bei V1 reine Laufzeitwerte aus CLI beziehungsweise bisherigen Tool-Defaults.
- Es wird kein zusätzlicher Migrationscode benötigt.

### 14.3 Manuelle Konvertierung bei Bedarf

Eine selten benötigte Konvertierung kann halbmanuell erfolgen:

1. mit einem aktuellen Tool eine neue V2-Datei erzeugen,
2. die bisherigen V1-ID-Einträge unter `Trices` kopieren,
3. Profile und Streamparameter prüfen,
4. die neue Datei validieren und unter Versionskontrolle stellen.

### 14.4 Alte Tools mit V2

Ein altes Trice-Tool versteht die neue Top-Level-Struktur nicht zuverlässig. Die Verwendung einer V2-Datei setzt daher ein V2-fähiges Tool voraus. Das ist ein akzeptierter Kompatibilitätsbruch und wird durch ein Tool-Update gelöst.

V2-fähige Tools müssen unbekannte `SchemaVersion`-Werte eindeutig ablehnen und dürfen eine Datei nicht teilweise als andere Version interpretieren.

## 15. Validierung

Eine V2-Datei ist mindestens auf folgende Bedingungen zu prüfen:

1. `SchemaVersion` ist exakt `2`.
2. `Profiles` ist vorhanden, eindeutig und enthält `default`.
3. `StreamFormat` und `Trices` sind vorhanden.
4. Jeder Pflichtparameter besitzt einen nicht-nullbaren `default`-Eintrag.
5. Jeder Parameter-Profilschlüssel ist in `Profiles` deklariert.
6. Alle Historienzeitpunkte sind gültige UTC-Zeitstempel und innerhalb einer Timeline eindeutig.
7. Werte entsprechen Typ und Wertebereich des Parameters.
8. `DefaultTRICEBitWidth` ist 8, 16, 32 oder 64.
9. Ein XTEA-Schlüssel ist exakt 128 Bit lang.
10. Bei `Encryption.Method == XTEA` ist ein Schlüssel vorhanden.
11. Die aufgelöste Kombination ist mit den Decoderregeln vereinbar; insbesondere ist die derzeitige Einschränkung von Single-Package-Annahme und Encryption zu beachten.
12. `TypeX0` ist für die gewählte Implementierung syntaktisch gültig.
13. Jeder Trice-ID-Eintrag enthält gültige `Type`- und `Strg`-Werte.
14. Nach einer CLI-bedingten Änderung wird die komplette resultierende Datei vor dem Ersetzen erneut eingelesen und validiert.

Ein Fehler muss den betroffenen JSON-Pfad, Profilnamen und gegebenenfalls Historienzeitpunkt nennen.

## 16. Abnahmekriterien

Eine Implementierung dieses Entwurfs ist funktional ausreichend, wenn mindestens folgende Fälle automatisiert getestet sind:

1. Eine neu erzeugte TIL ist V2 und enthält alle Pflichtwerte für `default`.
2. Eine bestehende V1-TIL bleibt byte-strukturell V1 und wird nicht migriert.
3. `-profile` ohne Angabe verwendet `default`.
4. Ein unbekanntes Profil wird abgelehnt.
5. Ein fehlender Profilparameter erbt den zeitabhängigen Default-Wert.
6. Ein historisches `null` in einem Nicht-Default-Profil erbt den Default-Wert zum abgefragten Zeitpunkt.
7. Die Auswahl exakt an einer `HistoryUntil`-Grenze verwendet den neuen Wert.
8. `-stream-date=2026-06` wird als `2026-06-01T00:00:00Z` ausgewertet.
9. Ein expliziter CLI-Wert wird als neuer `Current`-Wert gespeichert und der alte Wert genau einmal historisiert.
10. Ein erstmals explizit gesetzter Nicht-Default-Profilwert wird auch bei Gleichheit mit dem momentan geerbten Default als eigener Wert angelegt und historisiert zuvor `null`.
11. Ein erneut angegebener identischer, bereits explizit gespeicherter CLI-Wert erzeugt keinen weiteren Historieneintrag.
12. Mehrere Änderungen eines Aufrufs erhalten denselben UTC-Zeitstempel.
13. Ein historischer XTEA-Schlüssel kann zur Decodierung eines alten Binärlogs ausgewählt werden.
14. Derselbe Binärdatenstrom ergibt unabhängig von UART-, RTT-, TCP- oder Datei-Zuführung dieselben decodierten Trice-Ereignisse.
15. Die ASCII-Ausgabe mit den aus V2 geladenen Target-Timestampformaten entspricht der Ausgabe bei äquivalent explizit gesetzten bisherigen CLI-Optionen.
16. Eine unbekannte höhere `SchemaVersion` wird ohne Teilinterpretation abgelehnt.

## 17. Zusammenfassung

Schema V2 erweitert die bestehende TIL von einer reinen ID-/Formatstring-Liste zu einer selbstbeschreibenden Decodierbeschreibung für binäre Trice-Datenströme.

Die wesentlichen Eigenschaften sind:

- bestehende V1-Dateien bleiben unverändert,
- neue Dateien verwenden `SchemaVersion: 2`,
- `Trices` behält die bestehende ID-Semantik,
- alle Target- und Stream-Codierungsparameter stehen in `StreamFormat`,
- Profile werden mit `-profile` ausgewählt und parameterweise gespeichert,
- `default` ist explizit und vollständig,
- Profilabweichungen bleiben sparse,
- Historien werden je Parameter mit UTC-Änderungszeitpunkt geführt,
- verkürzte Datumsangaben wie `2026-06` sind zulässig,
- explizite CLI-Werte aktualisieren die V2-Datei und werden damit zum neuen Default des gewählten Profils,
- Kommunikation, Host-Zeit, Filterung und reine Ausgabegestaltung bleiben außerhalb der TIL.

Damit kann ein Decoder oder Konverter aus einem Binärstrom und einer passenden V2-TIL eine vollständige semantische Trice-Ereignisfolge und daraus beispielsweise den heutigen ASCII-Ausgabestrom erzeugen.
