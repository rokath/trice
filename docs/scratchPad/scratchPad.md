<!--

*) In ./examples/TriceABC funktionieren die *.sh Skripte sehr gut, allerdings wirken sie etwas abschreckend insbesondere für User, die keine ash-Experten sind. Lassen sich diese Skripte im Sinne der einfachen Lesarkeit verbessern ohne die Funktionalität zu verschlechteren? Zumindest mit ausführlichen Kommentaren. Auch ist es schwer, die generierten *.h und *.c Dateien zu finden. Wo sind die eigentlich? Das muss unbeding verbessert werden, damit User sich schnell zurechtfinden. Zumindest die README.md Files sollten darüber Aufschluss geben.

*) Die Bezeichnung MVP sollte verschwinden aus dem Inhalt der Datei docs/TriceBind/Trice_bind_90_MVP_User_Manual.md und geeignet ersetzt verden.

-->
---

*) UM Chapter 32: "Klassische Pufferlogs ohne benannte Felder bleiben als message verfügbar." Ergänze sinngemäß: "Sie werden nicht als strukturierte Werte angesehen.". Illustriere an einem Beispiel.

*) UM Chapter 32: "Das Textbeispiel zeigt den Meldungsinhalt ohne Metadaten. Die JSON und Key-Value Beispiele verwenden deaktivierte Host-Zeitstempel und keine weiteren Metadaten." Das ist nicht gewollt. Der User soll die volle Kontrolle behalten. Das was über CLI erlaubt ist soll auch umgesetzt werden: file, line, hs, id, ts. Hoststamp ist String entsprechend CLI Vorgabe. Prefix und Suffix sollten ignoriert werden. Target Stamp, jetzt unsigned int, soll verändert werden. Angezeigt werden ts16, ts32, ts16delta, ts32delta als Strings entsprechend der vorgegebenen Formatierung ohne ihre Tags aber mit zusätzlichem Text, wenn vorgegeben. Führende und folgende Leerzeichen werden entfernt. ts0 und ts0delta werden nicht angezeigt. Ist das sinnvoll und hinreichend genau spezifiziert?

*) UM Chapter 32: "Laufzeitstrings werden nicht noch einmal als C-Escapes oder als Tags interpretiert." - Bitte auch mit Beispiel.

*) UM Chapter 32: Dein -logFormat json gibt NDJSON aus, richtig?

*) UM Chapter 32: Sollte -logFormat seine Werte nicht besser case-neutral bekommen können?

*) UM Chapter 32: -logFormat kv ist für Skripte nicht gut verständlich. Zusätzlich -logFormat key-value erlauben, oder? 

*) "Structured formats require TREX;" steht in der -logFormat CLI Hilfe. TREX ist das default Drahtformat und hat mit dem Ausgabeformat nichts zu tun. Bitte einmali am Anfang des structure Logging chapteres klarestellen dass es nur für das defaut encoding TREX gilt und nicht für CHAR und DUMP. Und dann nicht weiter erwähnen, aalso auch nicht in der CLI Hilfe.

*) Erweitere _test/testdata/triceCheck.c nach den assert Zeilen um Zeilen die strukturiertes Logging beinhalten. Nimm für die Ausgabeformatierung die in den anderen Tests verwendeten CLI Settings an, damit das alles in einem Rutsch durchläuft. Du brauchst nicht alle Parameter-Counts in allen Bitbreiten durchsspielen. Nimm einen repräsentativen Set, der den Usern gleichzeitig als Anwenungsbeispiel dient. Zeige alle wichtigen Möglichkeiten einschließlich Bitbreiten, targetstamps und mit triceS und triceN und mixed %d {} sowie . und -> Operatoren. Verwende auch aFloat() und aDouble() beispielhaft. Fange nur an, wenn die Aufgabe ganz klar ist, ansonsten frage zurück.

*) Für die Lesbarkeit von ti.json und li.json wäre jeweilige Einzeiligkeit besser. etwa (mit Line vorne):

```json
	"13006": {"Line": 141, "File": "examples/G0B1_inst/Core/Src/main.c"},
	"13007": {"Line": 120, "File": "examples/G0B1_inst/Core/Src/stm32g0xx_it.c"},
````

Aber nur wenn das mit Standard-Lib Serialize/De-Serialize bzw. Standard-Lib Konvertern möglich ist. Auch darf es keine Probleme mit Legacy-Files bei Usern geben. Also z.B. gegebenes Format beibehalten, aber für neue Files einzeilenformat. Bitte Prüfe das, aber noch nichts machen.

*) UM Chapter 32: "JSON wird als JSON Lines ausgegeben: ein JSON-Objekt und ein abschließendes LF pro akzeptiertem Trice-Ereignis.". Ist das nicht genaugenommem NDJSON (Newline Delimited JSON)? Sollte das im UM klargestellt werden? Sollt der der CLI Wert angepasst werden?

*) UM Chapter 32.5: "Jeder Record enthält `tag` und `message`."
Bei trice("inf:Hi") entsteht "tag":"INFO" und "message":"Hi".
Bei trice("Inf:Hi") entsteht "tag":"INFO" und "Inf:message":"Hi".
Also "tag" erhält immer die kanonische Form und nicht was im Code steht. "message" ist genau die auch im text Format angezeigte Message (ohne Farbe).
Das sollte klargestellt werden und getestet sein.

*) Alle String-Werte bei -logFormat KV und JSON müssen von leading and trailing Spaces befreit sein.

*) "Ein Laufzeitstring wie `err:...` erzeugt keinen neuen Formatstring-Tag." - erläutere das im UM oder nimm es raus. Es ist eine tlog interne Behandlung, oder?

*) UM Chapter 32.7: "Ein erfolgreicher `bind`- oder `insert`-Lauf erzeugt `trice-fields.txt`. Bei `bind` liegt die Datei unter `-bindDir`, bei `insert` unter `-buildDir`; der Default ist jeweils `build/triceIDs`." -bindDir und -buildDir ist irritierend. Vermutlich ist es besser nur -buildDir als geeinsam nutzbaren Schalter zu haben.

*) UM Chapter 32.7.: "-dry-run veröffentlicht keine neue Datei und erhält ein vorhandenes Register." - Was ist mit Register gemeint? Bitte erläutern

*) Merge den Inhalt von docs/scratchPad/Strukturiertes_Logging_DE.md in das UM Chapter 32 und entferne docs/scratchPad/Strukturiertes_Logging_DE.md anchließend.

*) Überarbeite docs/scratchPad/Implementierungsplan.md derart, dass sofort klar ist, was noch zu tun ist und in welcher Reihenfolge. Alles was Abgearbeitet ist Am Ende in ein separates Haupt Chapter. DAS WERDE ICH NOCH NUTZEN FÜR REVIEW. CE wird zurückgestellt bis SL abgeschlossen ist. Und der PoC leitet dann die CE Implementierung ein. Darin gibt es noch bereits erledigte Infos, wie "Festgelegter M19-Vertrag", die im UM Chapter abgebildet sein sollten, und wenn, dann entfernt werden können. Es kommt darauf an, den Umfang der Rahmendokumente zu reduzieren - zuviel Leserauschen. Alle abgeabreiteten M-Nummern sollten nicht mehr erwähnt werden - außer es gibt triftige Gründe. Wenn Du z.B. von M17 sprichst, weiß ich nicht sofort was gemeint ist. Bitte immer eine Klammer mit Kurzüberschrift.

*) Aktuell sind einige Tests naturgemäß FAIL. Markdown und Link Fehler interessieren aktuell weniger. Manche Test waren vermutlich vorher FAIL. Das muss mit hoher Prio bereinigt werden. Es handelt sich um notwendige Anpassungen wg. Policy-Änderung, aber auch wirkliche Fehler könnten drin sein.

*) README.md und Implementierungsplan.md sind irgendwie doppelt gemoppelt und irritierend. Ich möchte nur eine einzige Datei statt 2. Am besten, in Folder scratchPad Ist nur noch das Chapter Kontextanreicherung mit Implementierungsplan und diesem scratchPad.md.

*) Du sollst scratchPad.md jetzt nicht anfassen. Aber seine Inhalte geordnet nach sinnvoller Reihenfolge als Aufgaben A1, A2, ... zusammen mit den noch nicht erledigten Tasks im Implementierungsplan.md update in Propmt Syntax auflisten.

*) Meine Anweisungen sind nicht in Stein gemeißelt, sondern ich führe gerne dazu einen Dialog mit Dir um schlussendlich gute Qualität des Ergebnisses zu erreichen.


