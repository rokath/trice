
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

*) NAMING:
`tlog` meint immer die `trice log` Funktionalität. `tlog` ist ein zusätzliches Executable, welches genau die `trice log` Funktionalität hat.

*) TARGETSTAMP (meist TIMESTAMP aber der Begriff sollte Target-Stamp oder nur Stamp sein um die Nicht-Einschränkung auf Zeit zu verdeutlichen):
Dadurch, dass ts im -vis String skaliert werden kann, sollte ts doch unproblematisch sein, oder? ts16 und ts32 sollen nicht vermischt verwendet werden. Durch Benutzung von explizit ts32 bzw. ts16 werden dann auch Fehler vermieden. Aber auch nur ts zu verwenden ist ok. Sobald ts auf ein -vis Log mit ts16 und auch mit ts32 matcht ist das ein harter Fehler. Es gibt aber den Sonderfall, dass ts16 die unteren Digits des letzten übertragenen ts32 sind. Das ist eine mögliche spezielle Optimierung, die im TriceUserManual.md #### 5.9.2. <a id="target-trice-stamps"></a>Target Trice Stamps gezeigt ist. Dieser Fall wird im Sinne der Einfachheit für -vis explizit ausgeschlossen. Die Unterscheidung von ts und ts_raw innerhalb von tlog erscheint unnötig, da tlog aktuell eine TargetStamp-Skalierung nicht unterstützt. Allerdings ist das ein nettes Feature und könnte nachgerüstet werden. Wenn das passiert, sollte das völlig unabhängig von der -vis Funktionalität sein. Schlussendlich entscheidet der User über die ts Bedeutung vollständig. Wenn das Plot-Tool nur x und y Werte z.B. für Lissajous Figuren braucht, könnte ts z.B. den x oder y Part übernehmen, obwohl das eine a-typische Benutzung wäre.

*) -pick, -ban, ;log=drop, ;log=keep:
Wenn z.B. -ban auf ein -vis matching Log passt, dass sieht -vis diesen Log nie. Insofern ist die -pick, -ban Filterung völlig unabhängig und damit unkritisch für die -vis Implementierung.

*) Mehrere Logs in einer Zeile:
Dieser Fall soll für die -vis Implementierung nicht erschwerend wirken. Wenn es hilft, wird gefordert, dass -vis Logs aus nur einem Log pro Zeile existieren. Andernfalls ignoriert -vis diese zum Beispiel.

*) Typisierte Messwerte:
Wenn Messwerte typisiert, auch nur kurz, existieren, kann diese Typinformation für -vis zeilenweise bewahrt werden, sofern sie für -vis benötigt wird. Wäre dann die angesprochene Sackgasse umgehbar? Bitte erläutese die Sackgassenproblematik.

*) Einfachheit:
Es soll keine Full-Featured Grafik-Tool Anbindung entstehen, insbesondere wenn der Coding-Aufwand dadurch unnötig groß wird. Wenn durch gewisse Einschränkungen zunächst vieles einfacher wird, dann gehe diesen Weg, mache aber die Einschränkungen deutlich.

*) TREX/CHAR/DUMP:
Es soll nur TREX unterstützt werden.

*) JSON:
In -vis='msg:printf(<go-fmt>, ...' kann <go-fmt> auch einen JSON Einzeiler enthalten, wenn das hilft.

*) Die wichtigste Änderung "-vis darf nicht auf dem bereits formatierten Logtext aufbauen":
Können die notwendigen Infos im der vorgelagerten Verarbeitung bewahrt werden, dort also nur einen kleinen Eingriff verursachen, und dann zusammen mit / parallel zu dem formatierten Text ausgewertet werden

*) Zu "Das hat drei Folgen":
1) Wenn Multi-Logs pro Zeile für -vis verboten werden, kann die Zeileninfo in globalen Variablen zwischengespeichert werden um sie für die -vis Ausgabe zu benutzen, ähnlich dem Stamp. Das ist zwar nicht schön, aber vielleicht im Sinne des geringstmöglichen Eingriffs machbar.
2) -pick/-ban muss darauf keine Rücksicht nehmen, wenn -ban auch auf -vis wirken darf.
3) Multi-Log Zeilen werden für -vis nicht erlaubt. D.h. sie dürfen vorkommen, aber der -vis Code nimmt darauf keine Rücksicht.

*) Typisierte interne Nachricht:
Diese könnte pro Trice Message als Strukt global gespeichert werden, vielleicht sogar asl Struct-Slice, um ggf. später mit Multi-Log Zeilen umgehen zu können. Wenn dann am Ende -vis aktiv wird, kann es darauf zugreifen.

*) Ja klar, ts sind nicht automatsch Mikrosekunden oder irgendeine andere Zeiteiheit:
Wrap-Erkennung wird nicht implementiert. Allerdings könnte darüber nachgedacht werden, wenn ein 16-Bit oder 32-Bit Stampwert auf nahe-Null springt, im 64-Bit Value +0x10000 bzw. +0x10000000 zu rechnen. Aber das gehört nicht ins MVP.

*) Transportumfang:
Datei und UDP ok für MVP. Was ist mit named Pipe? Sind die Win/Unix/Mac Unterschiede nicht in der Go Library gekapselt? Bitte erläutere "Sink-Vertrag Datensatz orientiert" an einem Beispiel.

*) Blockierung und Fehlerverhalten
Gib konkrtete Beispiele, wann das auftreten kann.

*) log=drop
Wenn es die Implementierung wesentlich vereinfacht, darf auch auf log=drop komplett verzichtet werden und der User muss dann explitit -ban:msg angeben um visualisierte Logs im normalen Trice Output zu unterdrücken. Halte ich aber nur für akzeptabel, wenn dadurch vieles wesentlch einfacher wird. Aber grundsätzlich sollte -ban:msg alle Tags msg völlig unsichtbar mache, auch für -vis.

*) Ausdrucks- und CLI-Syntax:
Ja, offen halten. printf=encoder habe ich verstanden. Etwa so: -vis='selector:encoder(...)@sink;filter'?

*) Das MVP sollte zunächst nur v0,...,v11 kennen und das zuätzliche Parsing auf name=% enfallen, damit es alles übersichtlicher wird. Diese Parsingfunktionalität muss dokumentiert und getestet werden, ist fehleranfällig und bringt wenig Mehrgewinn.

*) Erläutere weaum "file://out.csv" problematisch ist.

*) Ich möchte, dass Du mir konkret vorgibst, welche Textstellen der Issue gestrichen und gegen was ersetzt werden sollten. Außerdem eine Präzisierung, die unten in der Issue angefügt werden soll. Nicht als Comment sonder in die Issue mit hinein.

*) Bitte noch nichts machen jetzt, sondern nur mit mir reden um die Deteils abzustimmen, bevor Texte generiert werden.

---

*) "harter Fehler":
Eine zweite Stamp-Breite sollte nicht unerwartet auftreten können, da til.json von Anfang an verfügbar ist und gleich beim Start auf Konflickte gecheckt werden kann. In diesem Zusammenhang fällt mir auf, dass til.json die komplette History enthält und demzufolge alte Einträge stören könnten. Das sollte akzeptabel sein, da eine Fehlermeldung u.a. die betreffenden IDs enthält und der User ggf. die til.json manuell bereinigen kann, was mit einem Editor leicht geht. Wenn die til.json im laufenden Betrieb ge-updated wird und dann Konflikte auftreten, ist ein Ignorieren und deutliche Fehlermeldungen ausreichend. FAZIT: "harter Fehler" in diesem Zusammenhang einfach eine DEUTLICHE Warrnung bzgl. til.json beim Start und später bei jedem Auftreten.  

*) "stilles ignorieren von Multi-Log Zeilen:
Das ist völlig ok. für -vis sind Mult-Log Zeilen nicht da. Das muss nur in die Doku und bedarf keines Codes, oder nur sehr wenig.

*) Weglassen der named Pipes füe MVO ist ok.

*) "offene Entscheidungen":
1. DEUTLICHE Warnung und Regel abschalten
2. Sill ignorieren. Nur bei --verbose Switch Warnung ausgeben.
3. Ja, aber bei --verbose immer Warnung.
4. Ja.
5. Ja.

* Alle anderen Empfehlungen sind akzeptiert.

*) Bitte noch nichts machen jetzt, sondern nur mit mir reden um die Deteils abzustimmen, bevor Texte generiert werden.

