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



--- Nur antworten nichts machen!


Abgearbeitet:


- Lies docs/_scratchPad/handsoff_discussion_531_alias_tag_mapping_DE.md. Dazu habe ich diese Ergänzungen/Meinung/Hinweise/Fragen:
  - Die Tag-Heuristik für Aliasnamen erweitern:
    - Zusätzlich zu den Validierungspattern gibt es Invalisierungspattern, einige mögliche Beispiele:
      - infra invalidiert inf
      - edba oder adbi invalisiert db
      - tran invalidiert tr Treffer
    - Bitte finde sinnvolle Invalidierungspattern (Dein Englisch ist besser)
    - Wenn keine Tag-Zuordnung aus dem Aliasnamen ermittelt werden kann, prüfe ob der Formatstring einen gültigen Tag hat. Etwa könnte printi("err:...",...) den ERROR Tag zugewiesen bekommen. 
    Nur wenn keine Zuodnung gefunden wird behandle den Alias als "untagged".
    Für Sonderfälle soll zB bind -alias MyWrn:err möglich sein. err kann hier vereinbarungsgemäß nicht in til.json übergeben werden. MyWrn würde beim Loggen dann als WARNING behandelt werden. Um dem zu begenen muss beim Logging dann tlog -alias MyWrn:err angegeben werden. Hier wird dann auf dem Host err zugewiesen. Bei Bind werden ggf die IDs entsprechend zugewiesen. Diese Info-Übergabe mittels CLI ist nur für Sonderfälle gedacht und kann versehentlich fehlbedient werden (Risiko). Der reale Fall wird durch die Tag-Heuristik normalerweise voll abgedeckt.
  - Was ist mit "Bestehende Doppelregistrierungsregel" genau gemeint? Kontext zur Aufgabe?
  - Irgendwo wird noch TRICE_LEGACY_RPC_SUPPORT benutzt/dokumentiert. Bite prüfen.
  - Issue \#761 ist jetzt eröffnet. Prüfe, inwieweit diese mit dieser Aufgabe kollidiert/verreinbar ist und ob beide Aufgaben besser in einem Rutsch oder getrennt gelöst werden und in welcher Reihenfolge dann am besten.
- Du sollst jetzt nichts umsetzen, sondern nur gründlich analysieren. Schreibe dein Ergebnis in in ein 531-761_Analyse.md Dokument. Trenne User-Interface von Implentierung/Testing. Zunächst will ich ein klares, leicht zu verstehendes User Interface abstimmen natürlich nur im Rahmen des Machbaren. 

-->

Paul Win-PC Log:


ms@PaulPCWin11 MINGW64 ~/repos/trice (wip)
$ ./scripts/testAll.sh
Preparing testAll: capturing tracked worktree state...
Starting testAll at Wed Oct 28 11:07:11 WEST 2026
Selection: full
Run directory: /c/Users/ms/repos/trice/temp/log/testAll.cQDHeg
Scope: native code, compiler and local consistency checks; external links and release snapshots run separately.
ID workflows: bind plus legacy insert/clean
Failure policy: continue after failures (--no-stop, default)
Progress scale: expected relative test work (hardware-independent; no ETA)
Pinned tool versions for this checkout:
  Go 1.27.3; Node.js 26.12.1; CMake 4.4.4
  GCC/G++ 16.2.0; Clang/Clang++/clangd 23.1.3; ARM GCC 15.2.1
  clang-format 23.1.3; Cppcheck 2.22.0; shfmt 3.14.1; ShellCheck 0.11.0
  GoReleaser 2.19.0; actionlint 1.7.12; jq 1.8.2
  markdownlint 0.50.0; mdtoc 1.0.3; stitchmd 0.9.0
  PDF/link checks: md-to-pdf 5.2.5; Puppeteer 25.16.0; lychee 0.24.2
  Managed Python 3.15.2
  Valid cached tools are reused; installations stay local to this worktree.
Preparing declared native tools; first-time downloads and builds can take several minutes. Log: /c/Users/ms/repos/trice/temp/log/testAll.cQDHeg/tools.log

Install pinned test tools and compiler bundles (Windows GCC 16.2.0 / Clang 23.1.3 bundle) locally under temp/tools? [y/N] y
[ 1/25 | ~  0.3%] (  0m  0s) _400_test_shell_format.sh: running
[ 1/25 | ~  0.3%] (  0m  5s) _400_test_shell_format.sh: PASS
[ 2/25 | ~  0.6%] (  0m  0s) _410_test_shellcheck.sh: |Selected shellcheck 0.11.0: C:\Users\ms\repos\trice\temp\tools\native\windows-amd64\2a22af315f3a0dce\bin\shellcheck.exe
[ 2/25 | ~  0.6%] (  0m  9s) _410_test_shellcheck.sh: PASS
[ 3/25 | ~  0.9%] (  0m  5s) _420_test_dsstore.sh: PASS
[ 4/25 | ~  1.2%] (  0m  0s) _430_test_clang_format.sh: running
[ 4/25 | ~  1.2%] (  0m  5s) _430_test_clang_format.sh: PASS
[ 5/25 | ~  1.5%] (  0m  0s) _440_test_manual_format.sh: running
[ 5/25 | ~  1.5%] (  0m 13s) _440_test_manual_format.sh: PASS
[ 6/25 | ~  1.7%] (  0m  4s) _450_test_markdownlint.sh: PASS
[ 7/25 | ~  2.0%] (  0m  0s) _460_test_goreleaser_config.sh: |Selected goreleaser 2.19.0: C:\Users\ms\repos\trice\temp\tools\native\windows-amd64\2a22af315f3a0dce\bin\goreleaser.exe
[ 7/25 | ~  2.0%] (  0m  1s) _460_test_goreleaser_config.sh: PASS
[ 8/25 | ~  2.3%] (  0m  0s) _470_test_actionlint.sh: |Selected actionlint 1.7.12: C:\Users\ms\repos\trice\temp\tools\native\windows-amd64\2a22af315f3a0dce\bin\actionlint.exe
[ 8/25 | ~  2.3%] (  0m  3s) _470_test_actionlint.sh: PASS
[ 9/25 | ~  3.2%] (  0m  6s) _480_test_build_trice_tool.sh: PASS
[10/25 | ~  3.4%] (  0m  0s) _490_test_runtime_prepare.sh: running
[10/25 | ~  3.4%] (  0m  8s) _490_test_runtime_prepare.sh: PASS
[11/25 | ~  4.0%] (  0m 17s) _500_test_bind.sh: PASS
[12/25 | ~  4.6%] (  0m 15s) _510_test_bind_workflows.sh: PASS
[13/25 | ~  5.4%] (  7m 33s) _515_test_logging_features.sh: PASS
[14/25 | ~  6.0%] (  0m  0s) _520_test_target_code_lint.sh: running
[14/25 | ~  6.0%] (  0m 28s) _520_test_target_code_lint.sh: PASS
[15/25 | ~  6.8%] ( 17m 40s) _540_test_go.sh: PASS
[16/25 | ~  7.7%] ( 18m 13s) _550_test_go_coverage.sh: PASS
[17/25 | ~  8.3%] (  0m 28s) _560_test_clang_insert.sh: PASS
[18/25 | ~  8.8%] (  0m 24s) _570_test_clang_bind.sh: PASS
[19/25 | ~ 10.0%] (  3m 33s) _580_test_gcc_off.sh: PASS
[20/25 | ~ 11.1%] ( 12m 51s) _590_test_gcc_insert.sh: PASS
[21/25 | ~ 12.2%] ( 19m 17s) _600_test_gcc_bind.sh: PASS
[22/25 | ~ 60.4%] ( 14m 14s) _620_test_l432_configs.sh: PASS
[23/25 | ~ 70.3%] ( 16m  2s) _630_test_pc_targets_insert.sh: PASS
[24/25 | ~ 80.2%] ( 17m 23s) _640_test_pc_targets_bind.sh: PASS
[25/25 | ~100.0%] ( 28m 23s) _645_test_pc_targets_clang.sh: PASS
Verifying tracked worktree state...
Result: PASS
Duration: 9592s
Finished at Wed Oct 28 13:46:51 WEST 2026

ms@PaulPCWin11 MINGW64 ~/repos/trice (wip)
