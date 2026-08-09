

*) th@thX220iDebian:~/repos/trice$ ./trice_bindIDs_in_examples_and_test_folder.sh 
Trice ID workflow: bind
Sources: -src ./_test -src ./examples/exampleData -src ./examples/F030_inst/Core -src ./examples/G0B1_inst/Core -src ./examples/L432_inst/Core -src ./examples/DemoData_Trice 
TIL: ./demoTIL.json
LI: ./demoLI.json
LI root: .
ID options: -IDMin 13000 -IDMax 16383
Bind directory: ./build/triceIDs
th@thX220iDebian:~/repos/trice$ 


th@thX220iDebian:~/repos/trice$ ./trice_remigrateBindToClean_in_examples_and_test_folder.sh 
Trice ID workflow: remigrate
Sources: -src ./_test -src ./examples/exampleData -src ./examples/F030_inst/Core -src ./examples/G0B1_inst/Core -src ./examples/L432_inst/Core -src ./examples/DemoData_Trice 
TIL: ./demoTIL.json
LI: ./demoLI.json
LI root: .
ID options: -IDMin 13000 -IDMax 16383
Bind directory: ./build/triceIDs
th@thX220iDebian:~/repos/trice$ 

th@thX220iDebian:~/repos/trice$ ./trice_insertIDs_in_examples_and_test_folder.sh 
Trice ID workflow: insert
Sources: -src ./_test -src ./examples/exampleData -src ./examples/F030_inst/Core -src ./examples/G0B1_inst/Core -src ./examples/L432_inst/Core -src ./examples/DemoData_Trice 
TIL: ./demoTIL.json
LI: ./demoLI.json
LI root: .
ID options: -IDMin 13000 -IDMax 16383
Bind directory: ./build/triceIDs
th@thX220iDebian:~/repos/trice$ ./trice_cleanIDs_in_examples_and_test_folder.sh 
Trice ID workflow: clean
Sources: -src ./_test -src ./examples/exampleData -src ./examples/F030_inst/Core -src ./examples/G0B1_inst/Core -src ./examples/L432_inst/Core -src ./examples/DemoData_Trice 
TIL: ./demoTIL.json
LI: ./demoLI.json
LI root: .
ID options: -IDMin 13000 -IDMax 16383
Bind directory: ./build/triceIDs
th@thX220iDebian:~/repos/trice$ 

 --- Sollte jeweils silent sein im Normalfall

*) Inkonsistent, oder?:

th@thX220iDebian:~/repos/trice$ 
th@thX220iDebian:~/repos/trice$ git status
Auf Branch wip
Ihr Branch ist auf demselben Stand wie 'origin/wip'.

nichts zu committen, Arbeitsverzeichnis unverändert
th@thX220iDebian:~/repos/trice$ ./trice_insertIDs_in_examples_and_test_folder.sh 
Trice ID workflow: insert
Sources: -src ./_test -src ./examples/exampleData -src ./examples/F030_inst/Core -src ./examples/G0B1_inst/Core -src ./examples/L432_inst/Core -src ./examples/DemoData_Trice 
TIL: ./demoTIL.json
LI: ./demoLI.json
LI root: .
ID options: -IDMin 13000 -IDMax 16383
Bind directory: ./build/triceIDs
th@thX220iDebian:~/repos/trice$ ./trice_cleanIDs_in_examples_and_test_folder.sh 
Trice ID workflow: clean
Sources: -src ./_test -src ./examples/exampleData -src ./examples/F030_inst/Core -src ./examples/G0B1_inst/Core -src ./examples/L432_inst/Core -src ./examples/DemoData_Trice 
TIL: ./demoTIL.json
LI: ./demoLI.json
LI root: .
ID options: -IDMin 13000 -IDMax 16383
Bind directory: ./build/triceIDs
th@thX220iDebian:~/repos/trice$ git status
Auf Branch wip
Ihr Branch ist auf demselben Stand wie 'origin/wip'.

nichts zu committen, Arbeitsverzeichnis unverändert
th@thX220iDebian:~/repos/trice$ ./trice_bindIDs_in_examples_and_test_folder.sh 
Trice ID workflow: bind
Sources: -src ./_test -src ./examples/exampleData -src ./examples/F030_inst/Core -src ./examples/G0B1_inst/Core -src ./examples/L432_inst/Core -src ./examples/DemoData_Trice 
TIL: ./demoTIL.json
LI: ./demoLI.json
LI root: .
ID options: -IDMin 13000 -IDMax 16383
Bind directory: ./build/triceIDs
th@thX220iDebian:~/repos/trice$ ./trice_cleanIDs_in_examples_and_test_folder.sh 
Trice ID workflow: clean
Sources: -src ./_test -src ./examples/exampleData -src ./examples/F030_inst/Core -src ./examples/G0B1_inst/Core -src ./examples/L432_inst/Core -src ./examples/DemoData_Trice 
TIL: ./demoTIL.json
LI: ./demoLI.json
LI root: .
ID options: -IDMin 13000 -IDMax 16383
Bind directory: ./build/triceIDs
th@thX220iDebian:~/repos/trice$ git status
Auf Branch wip
Ihr Branch ist auf demselben Stand wie 'origin/wip'.

Änderungen, die nicht zum Commit vorgemerkt sind:
  (benutzen Sie "git add <Datei>...", um die Änderungen zum Commit vorzumerken)
  (benutzen Sie "git restore <Datei>...", um die Änderungen im Arbeitsverzeichnis zu verwerfen)
	geändert:       _test/alias_dblB_de_tcobs_ua/TargetActivity.c
	geändert:       _test/aliasassert_dblB_de_tcobs_ua/TargetActivity.c
	geändert:       _test/dblB_de_protect_tcobs_ua/TargetActivity.c
	geändert:       _test/modify_for_debug/TargetActivity.c
	geändert:       _test/ringB_de_protect_tcobs_ua/TargetActivity.c
	geändert:       _test/stackB_di_nopf_aux32_specific/TargetActivity.c
	geändert:       _test/testdata/triceCheck.c
	geändert:       _test/userprint_dblB_de_tcobs_ua/TargetActivity.c
	geändert:       demoLI.json
	geändert:       examples/DemoData_Trice/src/main.c
	geändert:       examples/F030_inst/Core/Src/stm32f0xx_it.c
	geändert:       examples/G0B1_inst/Core/Src/main.c
	geändert:       examples/G0B1_inst/Core/Src/stm32g0xx_it.c
	geändert:       examples/L432_inst/Core/Src/main.c
	geändert:       examples/L432_inst/Core/Src/stm32l4xx_it.c
	geändert:       examples/exampleData/triceExamples.c
	geändert:       examples/exampleData/triceLogDiagData.c

keine Änderungen zum Commit vorgemerkt (benutzen Sie "git add" und/oder "git commit -a")
th@thX220iDebian:~/repos/trice$ ./trice_remigrateBindToClean_in_examples_and_test_folder.sh 
Trice ID workflow: remigrate
Sources: -src ./_test -src ./examples/exampleData -src ./examples/F030_inst/Core -src ./examples/G0B1_inst/Core -src ./examples/L432_inst/Core -src ./examples/DemoData_Trice 
TIL: ./demoTIL.json
LI: ./demoLI.json
LI root: .
ID options: -IDMin 13000 -IDMax 16383
Bind directory: ./build/triceIDs
th@thX220iDebian:~/repos/trice$ git status
Auf Branch wip
Ihr Branch ist auf demselben Stand wie 'origin/wip'.

Änderungen, die nicht zum Commit vorgemerkt sind:
  (benutzen Sie "git add <Datei>...", um die Änderungen zum Commit vorzumerken)
  (benutzen Sie "git restore <Datei>...", um die Änderungen im Arbeitsverzeichnis zu verwerfen)
	geändert:       demoLI.json

keine Änderungen zum Commit vorgemerkt (benutzen Sie "git add" und/oder "git commit -a")
th@thX220iDebian:~/repos/trice$ git diff
diff --git a/demoLI.json b/demoLI.json
index e84c58f96..213570036 100644
--- a/demoLI.json
+++ b/demoLI.json
@@ -1,43 +1,43 @@
 {
        "13000": {
                "File": "examples/F030_inst/Core/Src/stm32f0xx_it.c",
-               "Line": 162
+               "Line": 163
        },
        "13001": {
                "File": "examples/F030_inst/Core/Src/stm32f0xx_it.c",
-               "Line": 168
+               "Line": 169
        },
...

*) Der Git commit State sollte sein wie nach trice bind (ohne ./build/triceIDs/ Folder)

*) triceCheck.c hatte für die trice insert Tests mehrere Trice Messages in einer Zeile an einigen Stellen um eben genau das zu testen. trice bind kann das noch nicht (MVP2 Feature). Trotzdem sollen diese Mehrfach Trices in einer Zeile mit trice insert getestet werden (später auch mit trice bind). Es ist allerdings nicht erforderlich diese Mehrfach Trices in einer Zeile in triceCheck.c und damit für alle PC-Konfigurrationen zu testen. Es ist ausreichend dafür einen separaten Test zu bauen für trice insert. Die spätere Eweiterbarkeit auf trice bind ist nett aber nicht nötig, dann dann kann auch ein spezieller MVP2 Test entstehen.

*) Die ./examples/PoC_... Ordner sind vermutlich nicht mehr erforderlich, ebenso sie beiden ..._PoC /..._PoCs Folder in ./docs/TriceBind. Sollten diese 4 Folder erhalten werden, etwa in ./docs/_legacy oder ./docs/obsolete? oder ist es besser diese zu entfernen?

*) In ./examples/TriceABC funktionieren die *.sh Skripte sehr gut, allerdings wirken sie etwas abschreckend insbesondere für User, die keine ash-Experten sind. Lassen sich diese Skripte im Sinne der einfachen Lesarkeit verbessern ohne die Funktionalität zu verschlechteren? Zumindest mit ausführlichen Kommentaren. Auch ist es schwer, die generierten *.h und *.c Dateien zu finden. Wo sind die eigentlich? Das muss unbeding verbessert werden, damit User sich schnell zurechtfinden. Zumindest die README.md Files sollten darüber Aufschluss geben.

*) Die Bezeichnung MVP sollte verschwinden aus dem Inhalt der Datei Trice_bind_User_Manual_MVP.md und geeignet ersetzt verden.

*) Versuche ein PoC zu eratellen, welches testet ob Compiler/Präprozessoren mit `__COUNTER__` mehrere Trice Messages in einer Zeile mit trice bind behandeln können.
