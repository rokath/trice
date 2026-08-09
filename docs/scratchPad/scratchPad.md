

*) triceCheck.c hatte für die trice insert Tests mehrere Trice Messages in einer Zeile an einigen Stellen um eben genau das zu testen. trice bind kann das noch nicht (MVP2 Feature). Trotzdem sollen diese Mehrfach Trices in einer Zeile mit trice insert getestet werden (später auch mit trice bind). Es ist allerdings nicht erforderlich diese Mehrfach Trices in einer Zeile in triceCheck.c und damit für alle PC-Konfigurrationen zu testen. Es ist ausreichend dafür einen separaten Test zu bauen für trice insert. Die spätere Eweiterbarkeit auf trice bind ist nett aber nicht nötig, dann dann kann auch ein spezieller MVP2 Test entstehen.

*) Die ./examples/PoC_... Ordner sind vermutlich nicht mehr erforderlich, ebenso sie beiden ..._PoC /..._PoCs Folder in ./docs/TriceBind. Sollten diese 4 Folder erhalten werden, etwa in ./docs/_legacy oder ./docs/obsolete? oder ist es besser diese zu entfernen?

*) In ./examples/TriceABC funktionieren die *.sh Skripte sehr gut, allerdings wirken sie etwas abschreckend insbesondere für User, die keine ash-Experten sind. Lassen sich diese Skripte im Sinne der einfachen Lesarkeit verbessern ohne die Funktionalität zu verschlechteren? Zumindest mit ausführlichen Kommentaren. Auch ist es schwer, die generierten *.h und *.c Dateien zu finden. Wo sind die eigentlich? Das muss unbeding verbessert werden, damit User sich schnell zurechtfinden. Zumindest die README.md Files sollten darüber Aufschluss geben.

*) Die Bezeichnung MVP sollte verschwinden aus dem Inhalt der Datei Trice_bind_User_Manual_MVP.md und geeignet ersetzt verden.

*) Erzeuge einen Branch bind_MVP2 aus Branch wip heraus, wechsle dorthin und baue einen MVP2 trice bind generator dort inklusive Tests, der nur die erweiterte Generatorfunktionalität hat, also noch keine check und diagnose Funktionen.