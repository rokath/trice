
Die v3 habe ich jetzt durchgelesen und sie macht einen guten Eindruck, allerdings gibt es noch etwas Kärungsbedarf. Nachfolgend meine  Notizen, Klarstellungen und Fragen. Sie beziehen sich auf v3 und sollten den einzelnen Textabschnitten zuordenbar sein. Einiges wird sich wiederholen. Bitte werte diese Notzen aus, fasse sie geeignet in einem kleinen Punktekatalog zusammen und teile mir Deine kritische Meinung jeweils dazu mit. Noch keine Dokumenterstellung, nur mit mir hier reden.

- 1.: "`trice clean` wird in einem stabilen Bind-Projekt nicht mehr regelmäßig benötigt." --- gar nicht mehr.
- statt ⁠TRICE_FILE_KEY nimm ⁠TRICE_BIND_FILE_KEY, denn es könnte zukünftig zusätzlich andere TRICE_FILE_KEY Arten geben, oder?
- ⁠TRICE_BIND_FILE_KEY gleichzeitig als Sidecard Detektor verwenden. Einfach #ifdef ⁠TRICE_BIND_FILE_KEY aufrufen um die Existenz eines Sidecars festzustellen. Das kann aber schieflaufen, wenn includete Header Sidecars haben und eine C Datei IDs im Code. Das ist aber eher ein Sonderfall, den man vielleicht verbieten kann. Das ist im Sinne der Einfachheit und Lesbarkeit zu entscheiden und nicht der Universalität. TRICE_SOURCE_KIND erscheint mir doppelt gemoppelt: `#fdef TRICE_BIND_FILE_KEY` reicht doch, oder?
-⁠ ⁠TRICE_CLEAN wird für bind-only Projekte gar nicht mehr benötigt, stört aber nicht, sollte jedoch bei trice bind -check zukünftig als  optional zu entfernen angemerkt werden. Sourcen ohne TRICE_CLEAN zeigen trice Editor false positiv Fehler ohne build Verzeichnis, wenn sie frisch ausgecheckt sind, trice build also noch nicht aktiviert wurde, denke ich. Insofern ist ein TRICE_CLEAN auch für bind Projekte nicht ganz wertlos. Der User sollte entscheiden, aber kompetent augeklärt werden. ⁠TRICE_CLEAN hat keine Steuerfunktion. Es ist lediglich ein Status für trice insert/clean und bind(wenn Sidecar da aber nicht zugreifbar dann unterdrückt es false-positive Editor Warnungen) und kann optional in trice bind Projekten entfernt werden. Zumindest MUSS sein Zustand von trice bind auf 0 gehalten werden. TRICE_CLEAN aus triceConfig.h entfernen als Empfehlung mit Editor false-positive Markierung bei missing ./build/TriceIDs angeben, da es eigentlich nicht mehr benötigt wird in trice bind Projekten.
- ⁠"10.Das bestehende optionale TRICE_CLEAN weiterhin den Clean-/Off-Pfad auswählt" --- Sourcen ohne TRICE_CLEAN zeigen trice Editor false positiv Fehler ohne build Verzeichnis, wenn sie frisch ausgecheckt sind, trice build also noch nicht aktiviert wurde. Insofern ist ein TRICE_CLEAN auch für bind Projekte nicht ganz wertlos. TRICE_CLEAN ist ein Status, kein Setting!
- ⁠"Für Makrofamilien mit mindestens einem Kleinbuchstaben wird iD(...) verwendet." --- es wird natürlich im RAM eingefügt und nicht in die Files geschrieben imd ggf. lediglich in die Sidecars geschrieben. 
- ⁠"ein ID-freies Uppercase-Makro verwendet den mit -defaultStampSize gewählten Default," --- die Datei wird nicht verändert, sondern die Abbildung erfolgt über das Sidecar. Ein Sidecar Beispiel mit den ID-Varianten wäre hilfreich.
- ⁠"Neue schwer unterscheidbare Case-Varianten wie trRICE oder TrICE zur zusätzlichen Stamp-Codierung sind nicht Bestandteil des MVP." --- Das sind ohnehin keine User Interface Macros. 
- Es sollte irgendwie klar sein, was User-Makros sind und was nicht. Das aufzuklären kan das TriceUserManual und die triceCheck.c hilfreich sein. Vielleicht braucht es eine zusätzliche automatisch testbare Referenzliste. 
-⁠  ⁠"5.6 Kein klassischer Include-Guard" --- das ist akzeptiert aber bitte konkrete Beispiele aufzeigen für die Benennungen.
- ⁠5.7 Datei ohne aktuelle Trice-Stelle: Der einzige Vorteil, den Sidecar include nicht zu löschen ist die Position zu behalten, oder? Aber ein trice.h könnte auch noch dasein. Ich denke auch, der Sidecar include sollte bleiben und ggf. Später mit bind -check angemerkt werden wie auch trice.h, wenn keine Trice Stelle im File.
- ⁠6.3 Automatisches Einfügen: Statt "// trice-bind" etwas nehmen wie "// trice-bind: Must be in this file and should be last include and before any trice line. Damit sehen die Entwicker gleich, dass diese include Zeile nicht frei verschiebbar ist.
-⁠ ⁠Sollte nicht nach jeder letzten include Zeile der Sidecar includiert werden, wenn danach trice Zeilen kommen bevor neue Includezeilen folgen? Dann wäre ein mehrfach include des Sidecar eine Notwendigkeit, die stillschweigend akzeptiert, aber mit trice bind -check angezeigt wird. Wenn Conditionale Includes am Ende des Include-Blocks stehen, sollte die Sindcar Inclusion nach der Condition folgen. Da diese Sonderbedingungen für das automatische Einfügen recht komplex werden können, sollte der MVP v1 einfach die Zeile nach dem allerletzten Include als Sidecar-Position nehmen, selbst wenn innerhalb einer Condition und vorher trice Zeilen. Die anderen Regeln dem MVP v2 überlassen, aber spezifizieren, dann das sind aktuell keine relevanten Fälle.
- ⁠"Ein Header besitzt keinen für die einfache Heuristik eindeutig erkennbaren äußeren Include-Guard und die Einfügeposition wäre dadurch mehrdeutig." --- Das MVP sollte nicht in andere Header schauen müssen. Was genau ist das Problem mit Headern ohne include guard? Bitte Beispiel angeben. Falls wirklich relevant, dann für MVP v2 spezifizieren.
- ⁠"und der Build soll mit einem Compilerfehler abbrechen" - wie kann das garantiert werden? Ein User könnte einfach 2 trice Zeilen vertauscht haben. Ich denke trice bind muss zwingend laufen sobald eine Datei irgendwie geändert wird, auch wenn nur aus einem - ein + gemacht wurde.
-⁠ ⁠7.1. Cobra? Was soll das hier?
- ⁠7.2. "Dokumentation soll nicht alle gemeinsamen Optionen duplizieren, sondern nur die Abweichungen aufführen." --- Soll in der gleichen Form erfolgen. Keine Codeduplizierung. Ausgabe für bind voll ist ok. Die Anzeigeduplizierung ist erstmal ok, da der User gezielt nach insert oder bind schaut und dann dort sucht.
-  ⁠8.1. "Explizite IDs größer null und nicht unterstützte Konstruktionen projektweit diagnostizieren." Dateien mit expliziten IDs>0 sollten beim bind einfach ignoriert werden. Es könnte legacy Code geben der unverändert bleiben soll. Das braucht nicht einmal eine Warnung. Nur trice bind -check reportet das.
- ⁠"Verpflichtende diagnostische Schattenkopien unter ./TriceIDs sind nicht Bestandteil des MVP" --- das interessiert doch nicht in der spec, oder?
- ⁠Da upper Case only trices eher legacy Code oder Spezialfälle für mehr Speed sind, also selten verwendet werden, sollte im Code geprüft werden "#ifdef TRICE_BIND_HAS_TID_K73A915E9C4021B8_L12" und nicht "if TRICE_BIND_HAS_TID_K73A915E9C4021B8_L12 == 1". Damit werden die Sidecar Dateien entlastet, oder?
- Die Erläuterungen im Anhang C sind gut aber nicht klar genug. Am besten mit konkretem Beispiel:

```C
// module.c
#include "trice.h"
#include "trice_module_c_K73A915E9C4021B8.h" // trice-bind: keep in file and at position
void f(void){
    TRICE("Hello");
    TRICE(id(0), "world");
    trice("!\n");
}
```

Dazu den kompletten Sidecar angeben und die Makromechanik in trice.h und erläutern wie es geht. Etwa:

```C
#undef TRICE_BIND_FILE_KEY
#define TRICE_BIND_FILE_KEY K73A915E9C4021B8

// -defaultStampSize 16

#define TRICE_ID_K73A915E9C4021B8_L5 Id(12345u) // TRICE("Hello");
#define TRICE_BIND_HAS_TID_K73A915E9C4021B8_L6
#define TRICE_ID_K73A915E9C4021B8_L6 id(12346u) // TRICE(id(0), "world");
#define TRICE_ID_K73A915E9C4021B8_L7 iD(12347u) // trice("!\n");
```

- Die unbedingte ID Angabe im Sidecar macht diesen lesbarer, oder?
- 9.4. "Explizite IDs größer null sind im Bind-Projekt verboten und müssen deshalb im Bind-Targetpfad nicht unterstützt werden."  --- Ich vermute nur innerhalb von einer Datei darf man nicht explizite IDs mit Sidecar mischen. Vielleicht sollte trice bind Dateien mit expliziten IDs einfach nicht ändern sondern nur inspizieren. Fehler melden, etwa wenn trices mit IDs und ohne IDs zusammen vorkommen oder Sidecar und explizite IDs zusammen. Aber saubere explizite ID Dateien, die mit til.json matchen werden in li.json und til.json includiert. Aber eben nur wenn fehlerfrei. Diese dürfen dann keinen Header, der einen Sidecar hat includieren bzw. müssen dann explizit "#undef TRICE_BIND_FILE_KEY" nach den includes haben. Das aber soll der User selbst managen. Diese Mischprojekte sind nicht Ziel, aber für eine möglichst reibungsarme Migration möglich, aber eben mit etwas Handarbeit. Was meinst Du dazu?
- ⁠Target-PoCs soweit sinnvoll im Anhang so spezifizieren, dass sie generiert werden können.
- "Der Build benötigt ./build/triceIDs" --- bei Bedarf legt trice bind diesen Folder an.
- ⁠der häufige Bezug auf TRICE_CLEAN ist nicht nötig in der Spec
- ⁠"Die Library erzeugt aktiven Trice-Code über den lokal gewählten Bind-Pfad." --- was genau ist damit gemeint?
- ⁠"Die Sidecar-Includes müssen vor dem aktiven Inserted-Build entfernt werden, weil sie sonst weiterhin lokal den Bind-Dispatch auswählen würden." --- Es sollte im Zuge der bind Implementierung das trice insert Verhalten etwas erweitert werden, z.B. bei vorhandenen Sidecar die jeweilige Datei auslassen in MVP v1. MVP v2 könnte bei includiertem Sidecar dieses mit auswerten für die til.json Konsistenz. Dann könnte ein Projekt insert/clean/bind wechseld benutzen. Das zieht aber weitere Probleme nach sich, etwa wo trice insert das ./build/TriceIDs Verzeichnis sicher findet, müsste als mit einem -bindDir Schalter analog zu bind ausgestattett werden. Zu aufwändig und fehleranfällig und deswegen bestenfalls als MVP v2 Option festhalten. Lediglich das Vorhandensein des Sidecars excludiert die betreffende Datei von insert. Das Vorhandensein von IDs>0 excludiert eine Datei von trice bind. Eine cleaned Datei erhält mit bind natürlich einen Sidecar, was OK ist (Migration).
- ⁠mehrere bindbare Trice-Aufrufe auf derselben physischen Zeile, sollten im MVP v2 erlaubt sein wenn `__COUNTER__` vom Compiler angeboten wird. Dazu bitte ein spezielles PoC spezifizieren.
- Vielleicht viele Warnungen nur bei bind -check um die Bildumgebung nicht unnötig noisy zu machen:
  - mehrfache identische Einbindung desselben Sidecars in derselben Besitzerdatei,
  - vorhandene Sidecar-Include-Zeile für eine Datei ohne aktuelle Trice-Stelle,
  - ungewöhnlicher, aber eindeutig auswertbarer Include-Struktur
  - automatisch nicht sicher bestimmbarer Include-Position, sofern keine falsche Bindung erzeugt wurde,
  - Artefakte, die bei einer optionalen Bereinigung nicht entfernt werden konnten.
- Frage: "error: include/log.h:27:5: Trice call inside macro definition is not supported by bind MVP" ---  können trices innerhalb Macros erlaubt werden, wenn `__COUNTER__` verfügbar ist? Falls ja, dann auch ins PoC spezifizieren.
- Das MVP v2 könnte, wenn `__COUNTER__` verfügbar ist also diese Einschränkungen aufheben, richtig?:
  - mehrere Trices in einer Zeile
  - Trices in einer Makrodefinition wie:
  ```C
  #define LOG_ERROR(n) \
  switch(n) { \
    case 0: break; \
    case 1: triceS("err:no file\n"; break; \
    default: trice("err: unknown error number %d\n", n ); \
    }
  ```
- Wenn ein Sub Projekt mit IDs im Code mit einbezogen werden soll ohne dessen Code zu ändern, ist trice insert der versionierte Stand. Da dann IDs >0 in den Dateien stehen, fasst trice bind diese nicht an. Lediglich ein trice Clean gefolgt von trice insert bindet diese Dateien. Das ist dann entweder Fehlbedienung oder gewollt. IDs im Code haben immer Vorrang.
- ⁠"./scripts/testAll.sh full MUSS erfolgreich laufen." Ja, aber nur quick soll bei den Inbetriebnahme Tests benutzt werden. Der full Test läuft abhängig vom Rechner ggf. über eine Stunde und bringt für diesen MVP keinen Mehrwert gegenüber quick.
- "Der PoC in examples/PoC_bind_v2 wird auf den echten Generator umgestellt oder durch einen äquivalenten Generatorintegrationstest ergänzt." --- keine Umstellung, neuer unabhängiger PoC mit anderem Namen. Mehrere unabhängige PoCs sind ok zur Verifikation.
- ./testAll.sh muss aber auch noch erweitert werden auf zusätzlich trice bind. Also es braucht neben ./trice_insert...sh eine ./trice_bind...sh und die müssen die Demo Quellen abwechselnd anfassen. Dafür brauche ich eine sinnvolle Lösung ohne viel Codeduplizierung. Etwa eine triceCheck.c mit:

```C
#if TRICE_BIND_TEST
#include "triceCheck_c_K1234567812345678.h" // trice-bind: keep in file and at position
#endif
```

- Alle `#include sidcar` mit `#if TRICE_BIND ...` klammern ist nicht zielführend, denke ich.

- Alle MVP v2 Optionen in separaten Anhang und bestenfalls im MVP v1 Spec Text darauf referenzieren.
- Sprachlich vielleicht nur von MVP und MVPv2 sprechen?
- Alle v3 Festlegungen, zu denen nichts angemerkt wurde, gelten als akzeptiert.

- Der Umfang der Spec ist in etwa gut. Allerdings leidet die lesbarkeit durch die in-text Erkäuterungen. diese sollten alle in den Anhang und im Text nur ein Link dorthin stehen.
- Auch Implementation & Testing, Erweiterunge usw. in den Anhang, so dass die reine Spec kurz ist und damit gut lesbar. 

---

Zu 1.) Die TRICE_CLEAN Erläuterung bitte im Anhang

Zu 3.) Ich kann das noch nicht im Detail verstehen. Bitte erkläre anhand von Beispielen: a) Was nicht geht b) Was stattdessen geht.

Zu 4.) Die allermeisten Projekte verwenden ausschließlich Trices mit mindestens einem Lower-case. Zeilen wie `TRICE(id(0), "world");` sind die extreme Ausnahme. Dafür den Umfang aller Sidecars zu verdoppeln halte ich nicht für adäquat. Mir ist nicht klar, warum ein Compiler den Fall `#define TRICE_BIND_HAS_USER_TID_K73A915E9C4021B8_L6 [value]` von einem nicht definierten `TRICE_BIND_HAS_USER_TID_K73A915E9C4021B8_L6` nicht sicher unterscheiden können soll. Bitte erläutere oder mache einen praktikablen Vorschlag. Wenn es wirklich Instabilitäten bei irgendwelchen Compilern geben könnte, ist Dein Vorschlag akzeptiert.

zu 5.) Sollte `trice generate` erweitert werden um eine Liste mit allen User-Level Trice Makros erstellen zu können? Diese könnte in dem ./docs/ref Folder erzeugt werden und das UM verweist darauf. Ebenso kann die bind-spec sich dann darauf verbindlich beziehen.

Zu 9.) "Der Target-Dispatch gibt einem expliziten ID-Argument immer Vorrang." --- Wird lediglich als nicht verfolgte Option im Anhang erwähnt. `#undef TRICE_BIND_FILE_KEY` ist die Waffe der Wahl für solche seltenen Spezialfälle.

Zu 10.) A) Mehrere Trices in einer Zeile ist nicht unbedingt wichtig im MVP2, wäre aber nett gerade für generierten Code. B)
`#define LOG_ERROR(n) trice("error=%d", n)` wäre schon wichtig für das MVP2. Es wäre akzeptabel, wenn alle diese Aufrufstellen die gleiche ID bekämen und auch wenn die LocationInformation nicht konsistent ist. Könnte trice bind (MVP2) vielleicht alle LOG_ERROR(n) Aufrufe pro Datei bestimmen und im jeweiligen Sidecar vermerken? Beispiel:

```C
/// \file module.h
#define LOG_ERROR(n) trice("error=%d", n);
```

```C
/// \file module.c
#include "module.h"
#include "trice.h"
#include module_c_K73A915E9C4021B8.h
void fn(void){
   LOG_ERROR(0)
   LOG_ERROR(1) 
}
```

```C
/// \file module_c_K73A915E9C4021B8.h
/// \details generated by trice bind

#undef TRICE_BIND_FILE_KEY
#define TRICE_BIND_FILE_KEY K73A915E9C4021B8

// -defaultStampSize 16

#define TRICE_ID_K73A915E9C4021B8_L6 iD(12345u) // trice("error=%d", n);
#define TRICE_ID_K73A915E9C4021B8_L7 iD(12346u) // trice("error=%d", n);
```

Das wäre implementierbar ohne `__COUNTER__` zu verwenden und damit Compilerunabhängig.

Eine andere Möglichkeit, wäre, den clang oder gcc Präprozessor Output als basis für die Sidecar Erzeugung zu nehmen, auch wenn schlussendlich das target mit einem anderen Compiler übersetzt wird. Was meinst Du dazu?

apropo: `/// \file module_c_K73A915E9C4021B8.h` als erste Zeile im sidecar wäre doch ok, oder?

Zu 11.) Es reicht sogar wenn ./_testAll_10_PcTargetTests.sh (ohne option full) sauber läuft, während der Inbetriebnahmetests.


---

Zu 3.) Langsam wird es klarer. Bitte diese Erläuterung mit in den bind-spec Anhang aufnehmen.

Zu 4.) Das ist ein guter Weg. Allerdings sollten im Sinne der Lesbarkeit keine Zeilenumbrüche generiert werden, auch wenn die Zeilen recht lang werden können. MVP2 könnte bei Bedarf eine -maxLineLength Option bekommen. Die Erläuterungen bitte auch in den Anhang.

Zu 5.) Die Referenzlisten Erzeugung hat primär nichts mit trice bind zu tun. Sie gehört auch in den Anhang als Option. Zu dem Entwurf äußere ich mich hier erstmal nicht.

Zu 10.) Bitte komplet als Anhang, da nicht teil des MVP