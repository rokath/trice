<!--

*) In ./examples/TriceABC funktionieren die *.sh Skripte sehr gut, allerdings wirken sie etwas abschreckend insbesondere für User, die keine ash-Experten sind. Lassen sich diese Skripte im Sinne der einfachen Lesarkeit verbessern ohne die Funktionalität zu verschlechteren? Zumindest mit ausführlichen Kommentaren. Auch ist es schwer, die generierten *.h und *.c Dateien zu finden. Wo sind die eigentlich? Das muss unbeding verbessert werden, damit User sich schnell zurechtfinden. Zumindest die README.md Files sollten darüber Aufschluss geben.

*) Die Bezeichnung MVP sollte verschwinden aus dem Inhalt der Datei docs/TriceBind/Trice_bind_90_MVP_User_Manual.md und geeignet ersetzt verden.

TODO:

- -sl Beispiele
- -ce Beispiele
- Trice Features
  - speed, space, tags, selection, routing, level, id, history, rtt, abc, vis, sl, ce, COBS/TCOBS, encryption, hs, ts, tsdelta, endianness, X0, li, small on wire, multi-targets, serial, udp in, tcp io, protection, bind, insert-cache, aliases, assert, self-contained


RM Demo Text zu lang
HW Tests


- Lies docs/_scratchPad/handsoff_discussion_531_alias_tag_mapping_DE.md. Dazu gabe ich diese Ergänzungen/Meinung/Hinweise/Fragen:
  - Die Tag-Heuristik für Aliasnamen erweitern:
    - Zusätzlich zu den Validierungspattern gibt es Invalisierungspattern, einige Beispiele:
      - infra invalidiert inf
      - edba oder adbi invalisiert db
      - tran invalidiert tr Treffer
    - Bitte finde sinnvolle Invalidierungspattern (Dein Englisch ist besser)
    - Wenn es trotzdem keine Tag-Zuordnung gibt, prüfe ob der Formatstring einen gültigen Tag hat. Etwa könnte printi("err:...",...) den ERROR Tag zugewiesen bekommen. 
    Nur wenn keine Zuodnung gefunden wird behandle den Alias als "untagged"
    Für Sonderfälle soll zB bind -alias MyWrn:err möglich sein. err kann vereinbarungsgemäß nicht in til.json übergeben werden. MyWrn würde beim Loggen dann als WARNING behandelt werden. Um dem zu begenen muss beim Logging dann tlog -alias MyWrn:err angegeben werden. Hier wird dann auf dem Host err zugewiesen. Bei Bind werden ggf die IDs entsprechend zugewiesen. Diese Info-Übergabe mittels CLI ist nur für Sonderfälle gedacht. Der reale Fall wird durch die Tag-Heuristik normalerweise voll abgedeckt.
  - Was ist mit "Bestehende Doppelregistrierungsregel" genau gemeint?
  - Irgendwo wird noch TRICE_LEGACY_RPC_SUPPORT benutzt/dokumentiert. Bite prüfen.
  - Issue \#761 ist jetzt eröffnet. Prüfe, inwieweit diese mit dieser Aufgabe kollidiert/verreinbar ist und ob beide Aufgaben besser in einem Rutsch oder getrennt gelöst werden und in welcher Reihenfolge dann am besten.
- Du sollst jetzt nichts umsetzen, sondern nur gründlich analysieren. Schreibe dein Ergebnis in in ein 531-761_Analyse.md Dokument. Trenne User-Interface von Implentierung/Testing. Zunächst will ich ein klares, leicht zu verstehendes User Interface abstimmen natürlich nur im Rahmen des Machbaren. 


--- Nur antworten nichts machen!


---

