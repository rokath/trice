<!--

*) In ./examples/TriceABC funktionieren die *.sh Skripte sehr gut, allerdings wirken sie etwas abschreckend insbesondere für User, die keine ash-Experten sind. Lassen sich diese Skripte im Sinne der einfachen Lesarkeit verbessern ohne die Funktionalität zu verschlechteren? Zumindest mit ausführlichen Kommentaren. Auch ist es schwer, die generierten *.h und *.c Dateien zu finden. Wo sind die eigentlich? Das muss unbeding verbessert werden, damit User sich schnell zurechtfinden. Zumindest die README.md Files sollten darüber Aufschluss geben.

*) Die Bezeichnung MVP sollte verschwinden aus dem Inhalt der Datei docs/TriceBind/Trice_bind_90_MVP_User_Manual.md und geeignet ersetzt verden.

TODO:

- -sl Beispiele
- -ce Beispiele
- Trice Fretures
  - speed, space, tags, selection, routing, level, id, history, rtt, abc, vis, sl, ce, COBS/TCOBS, encryption, hs, ts, tsdelta, endianness, X0, li, small on wire, multi-targets, serial, udp in, tcp io, protection, bind, insert-cache, aliases, assert, self-contained


RM Demo Text zu lang
HW Tests


Zwischenfrage: Wie kommt go.mod ins scratchPad und wie kann scratchPad komplett für irgendwelche Tests ausgeschlossen werden? Es ist auf gut Deutsch einfach ein Schmierzettel Verzeichnis: 
--- FAIL: TestScratchpadModuleBoundary (0.00s)
    portability_test.go:160: 
        	Error Trace:	/home/runner/work/trice/trice/scripts/portability_test.go:160
        	Error:      	Received unexpected error:
        	            	open ../docs/scratchPad/go.mod: no such file or directory
        	Test:       	TestScratchpadModuleBoundary
FAIL
coverage: 0.0% of statements in ./...
FAIL	github.com/rokath/trice/scripts	46.680s
FAIL
Error: Process completed with exit code 1.



--- Nur antworten nichts machen!


---

