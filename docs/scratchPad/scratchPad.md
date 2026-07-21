
- Pipe-Tools werden erstmal nicht verfolgt. Das wären etwa:
  - TriceRx welches alles von tlog receive kann und in Pipe schreibt
  - TCOBSDecode, [TCOBSEncode], COBSDecode, [COBSEncode]
  - XTEADecode [XTEAEncode]
  - TREXDecode -> ASCII oder Binary
- tlog hat intern viel Wissen. Es nutzt aktuell nur til.json [li.json] und CLI Werte. TREX separat als JSON zu spezifizieren, würde etwa so aussehen: 
    - ./specs
      - Die TREX Spec beschreibt den physischen Aufbau, also genau die Trice Datenstrom BitOrdnung bis hin zu abstrakten Zahlenwerten und dem Payloadumfang. Sie umfasst nicht die Interpretation im Einzelnen.
        - TREX.json
          mit triceTargetTimeStampBits Info
          mit triceTypeX0CountedBuffer Info
      - TREXDefaultConfig.json umfasst die normale Datenstromkonfiguration (ohne Transport). Die Werte können projekt-spezifisch im Rahmen einer optionalen triceConfig.json überschrieben werden.
        - "TREXDefaultConfig.json":{
            "ByteOrderLittleEndian": true,
            "doubled16BitID": false,
            "defaultTRICEBitwidth": 32,
            "singleFraming": false,
            "TargetStampType": {
              "ts0": "none",
              "ts16": "uint16",
              "ts32": "uint32"
            }
          }
        Es ist zu überlegen statt TREXDefaultConfig.json einfach nur eine triceDefaultConfig.json zu haben, ähnlich triceDefaultConfig.h und dort etwa auch Transport-Spezifika defaults unterzubringen. Einerseits soll es kein Overdesign (viele Dateien) geben, andererseits soll die strukturelle Zuordnung gut erkennbar sein. 
      - projektspezifische triceConfig.h und triceConfig.json müssen matchen. triceConfig.h ist die Quelle der Wahrheit. triceConfig.json darf weitere projektspezifische Infos enthalten, etwa Transport-Spezifika oder ggf. Output-Anweisungen.
      - Um den TEX Datenstrom umfassend zu interpretieren, sind zwei weitere Dateien nötig: projektspezifisch til.json (ggf mit li.json) und trice-spezifisch triceTypes.json. Die IDs aus TREX führen über til.json zu den Trice-Namen und Formatstrings und triceTypes.json liefert Einzelheiten zu den Tricenamen. Grundsätzlich könnte triceTypes.json in die til.json hinein generiert werden, aber dann gibt es sehr viel Indormations-Duplikation, auch wenn nur generiert.
    - Dass Configwerte generell mittels CLI überschreibbar seinen sollen ist gegeben und erst einmal nebensächlich.
    - Die Projekt spezifische Ausgabekonverierung erfolgt dann Tool-Spezifisch:
      - Die normale ASCII Ausgabe Steuerung, etwa `-prefix` kann mit in die triceConfig.json
      - destinationA, destinationB, ... können je nach Umfang separat oder auch über die triceConfig.json abgebildet werden. 
    - tlog out Optionen (parallel)
      - ASCII ist der jetzige Log Output
      - TREX als binärer Output, der in ein andere binäres Format mittels Pipe Tool konvertiert werden kann.
      - Grafik-Tools spezifischer Output
- Nahziele:
  - til.json bleibt unverändert
  - tlog mit CLI bleibt unverändert
  - Geeignetes Open-Source Grafiktool finden, welches möglichst einfach ansgesteuert werden kann von tlog aus, auch wenn nur über ASCII. Es geht um eine Demonstration um zu zeigen, wie Grafiktools engebunden werden können.
  - tlog wird ggf. erweitert um einen Datenstrom für die Visualisierung einzelner selektierter Grafiktools zu erzeugen. Dessen Spezifika landen im Code und CLI, die Tool-Konfig-Spezifika ggf in einer Tool-JSON.
  - Kleines Datengenerator Executable, welches z.B. `TRice("vis: %d=sin(x), %d=cos(x)\n", x);` oder etwas spannenderes ausgibt mit einer einstellbaren Timestamp Basis.
  - Erweiterung von tlog für den Grafi-Demonstrator.
- Zweitziele:
  - Virtuelles Target welches die Datei _test/testdata/triceCheck.c abspielt und einen TREX-Datenstrom erzeugt, etwa einfach MULTI-Frame und TCOBS. Dieser kann mit tlog angeschaut werden.
  - TriceABC als unabhängige Verifikation sicherstellen. Bis jetzt ist unbewiesen ob TREX==ABC-TREX. Es könnte genügen ein binäres Logfile aus dem Bustraffik zu erzeugen, welches mit tlog angeschaut werden kann.
  - JSON-Specs: Eine TREX.json mit einer triceDefaultConfig.json plus triceTypes.json inklusive Projekt-spezifischer triceConfig.json + til.json mit optionaler li.json. 
  - Ein Generator-Tool könnte dann aus allen diesen JSON Files eine Projekt-Datenstrom-spezifische Schema-Datei generieren, die dann allein ausreicht für die vollständige Decodierung durch irgendein anderes Tool (eine Summen-Schema JSON, die dann zur Decodierung benutzt wird). Dann ist der Weg offen auch z.B. Python oder Rust zu nehmen. Ein AI-erzeugtes Programm sollte dann machbar sein. Vielleicht ist das auch schon der Weg zu triceLogC?
  - Ein triceLogC (in C/C++) Executable bauen, welches die von einem virtuellem Target ausgegebene triceCheck.c visualisiert. 

---

- ts32 kann sehr wohl mal ein float32 werden und aus uint16 könnte mal ein int16 werden. Insofern sind das konfigurierbare Werte.
- Regeln für variable Parameterzahlen gehören nicht in die physische Beschreibung von TREX. Parameterzahlen gibt es erst nach der ID-Auflösung über til.json. Zulässige Werte und Invarianten (was meinst Du dami?) eher auch nicht, vermute ich.
- triceDefaultConfig.h mit triceConfig.h beschreibt das Varget Verhalten. Insofern halte ich triceDefaultConfig.json mit triceConfig.json als Empfänger Konfiguration für ok.
- Ja, TREX, Framing und Transport sind verschiedene Ebenen, aber ließe das sich nicht auch in der trice[Default]Config.json sauber abbilden? Der User verantwortet die triceConfig.* alle beide. Wenn er es nicht richtig macht, ist es ein Fehler. Auch kann er einen AI Agenten checken lassen.
- CLI überschreibt immer ...Config.json, Punkt.
- TREX Decoder decoded und hat nie etwas mit Ausgabe zu tun.
- Korrektur (beides soll gehen):
  - `TRice("vis:demo:sin=%f,cos=%f\n", sinf(TimeStamp32), cosf(TimeStamp32));`
  - `trice("vis:demo:x=%f,sin=%f,cos=%f\n", x, sinf(x), cosf(x));`
  - Ich habe in Erinnerung inwieweit trice generate für Grafikausgabe benötigt wird. Reicht nicht "vis:" als Ausgabefilter?
  - Mit TREX meine ich ausdrücklich das decodierte binäre Trice Datenstromformat.
- Das bloße Mitschneiden des ABC Bustraffics und dessen testweise Ausgabe erscheint mir ausreichend. Genauerere Test sehe ich erst bei Bedarf, etwa Bug-Reports. Ich habe nicht unendlich viel Energie.
- triceSchema.json: Können diese Teil der Schema-Datei werden oder was sit hier üblich?:
  - präzise normative Beschreibungen
  - valide Binärbeispiele
  - erwartete Decodergebnisse
  - Grenzfall-Testvektoren
  - negative Testfälle
- normalisierte Datensätze als Zwischenergebnis und die Zweiteilung von triceLogC ist ok. Aber der einzige Vorteil, den ich sehe, ist dass normalisierte Datensätze auch andereitig verarbeitet werden könnten. Dass C-Code JSON Files als aufwändig sieht, kann ich nicht nachvollziehen. Es gibt bestimmt Libraries und wes geht nur um das initiale Lesen bei Programmstart. Dann stehen die Strukturen intern und der eigentliche Input Datenstrom ist binär. 

---

Ich sehe es eher so in der triceDefaultConfig.json (ts=TargetStamp!=TimeStamp):

{
  "TREX": {
    "TargetStamp": {
      "ts0": {
        "Type": "none"
      },
      "ts16": {
        "Type": "uint16",
        "Unit": "sec"
        "StepSize": 0.000001
      },
      "ts32": {
        "Type": "float32",
        "Unit": "sec"
        "StepSize": 1.0
      }
    }
  }
}

Dabei können Tool-spezifisch in der triceConfig.json noch weitere Felder hinzukommem, etwa für tlog "FormatString" oder ein Tool "Label" oder Werte überschrieben werden. Und die Bits bleiben in TREX.json.

TREX=TRiceEXtended ist immer das entframte decodierte Binärformat. Alles andere bekommt jeweils passende Namen. Davor liegt der Trice Binärdatenstrom, also inkusive Framing und Encoding.

---

Zusätzliche Feldnamen nur linear, keine Toool-Referenzen. Kann ggf. im Namen abgebildet werden.

Dass bei float32 die StepSize nicht konstant ist, ist klar. Dann wird dort z.B. "Einheitswert" genommen, also was dem Zahlenwert 1.0 entspricht, "Scale"? Auch z.B. "Offset" könnte spezifiziert werden.

ByteOrder ist ein Target Config Value (MCU spezifisch).

Ich möchte, dass Du dieses Diskussionsergebnis in einer Markdown Datei zusammenschreibst, aber erst, wenn Du keine Rückfragen mehr hast und ich dir dann das Go gebe.

---

Bei Scale bin ich mir nicht sicher. StepSize ist unmittelbar verständlicher für int Werte und die sind bei den TargetStamps der Regelfall. Mit Float32 ginge ja auch: Wert = (Rohwert + Offset) x Scale. statt  Wert = Rohwert x Scale + Offset. Was ist denn sinnvoll/üblich? Der Offset könnte ja auch extreme Werte annehmen. Ist meine Annahme, dass, wenn der Floatwert identisch zu "Scale" ist, genau eine Unit erreicht ist richtig? Aber wie mit Offset umgehen? Oder gibt es keine saubere universelle Lösung? Dann nehmen wir jetzt StepSize und kümmern uns um float32 wenn es denn gebraucht wird.

Wenn noch "offene Entscheidungen", benenne diese jetzt. 

Nachziele / Zweitziele zusammenfassen in ein prioritätsliste

---

Grafikdemonstrator ist eine klare Aufgabe und die Details werden separat erarbeitet.

trice("vis:[addr:]...", ...) - [addr] kann toolspezifisch ausgewertet werden

"ByteOrderLittleEndian": true - es ist de-facto ein boolean Wert

TargetStamp zunächst nur mit StepSize und ohne float32.

JSON Feldnamen werden mittels "Version" erweiterbar.

TREX.json umfasst die Bits- Beschreibung, das sollte jetzt klar sein

Die Einzelheiten der JSON Files sind zunächst nicht entscheidend, es geht hier zunächst um den Grobfahrplan.

TriceABC liefert einfach eine Datei mit dem Trice Binärdatenstrom, da alle alles sehen ist es quasi egal, wer diese datei schreibt. Vielleicht führen wir eine zusätzliche N_log Node ein, die diese Datei als trice.bin oder abc.bin schreibt neben abc.log.

Ich will noch nicht alle Details festlegen. Was an wichtigen Dingen ist noch unklar?

---




