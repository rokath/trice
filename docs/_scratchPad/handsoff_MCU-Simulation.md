# Handoff: MCU-Simulation mit LabWired für Trice

**Stand:** 13.11.2026  
**Thema:** LabWired Core als mögliche Laufzeitumgebung für Trice-Demos und Benchmarks

## Kurzfazit

LabWired ist ein aktiv entwickelter, noch wenig verbreiteter Open-Source-Simulator für kompilierte MCU-Firmware. Er führt ELF-Firmware auf simulierten CPUs und Peripherie-Modellen aus und kann unter anderem UART-Ausgaben, GPIO, Registerzustände und Testassertions erfassen.

Für Trice ist LabWired einen Prototyp wert: Eine Demo könnte Trice-Firmware ohne angeschlossenes Board ausführen und den Ablauf reproduzierbar machen. Für Aussagen über reale Laufzeitkosten, ISR-Latenz oder die Anzahl der CPU-Takte bleibt ein physisches Board notwendig. Simulationszyklen sind kein Ersatz für eine Messung auf Silizium.

## Projekt und Reife

- Hauptrepository: [w1ne/labwired-core](https://github.com/w1ne/labwired-core), MIT-lizenziert.
- Die Dokumentation nennt Cortex-M0+/M3/M4/M7/M33, RISC-V und ausgewählte Xtensa-Pfade. Die Abdeckung unterscheidet sich deutlich je nach MCU und Peripherie.
- Das Projekt hat CLI, versionierte Releases, CI-Tests, Beispiele, Debugging-Integration und ausführliche Dokumentation zu Modellgrenzen.
- Der von uns betrachtete Stand war v0.25.0. Für reproduzierbare Tests sollte eine konkrete Version gepinnt werden.
- Der niedrige Sternestand (vom Nutzer mit rund 124 Sternen beobachtet) bedeutet vor allem geringe Reichweite bzw. geringe Verbreitung. Er ist kein Beleg für Inaktivität. GitHub zeigte über 6.000 Commits; jüngste Releases und Beispiele belegten laufende Entwicklung.
- Ein kleiner Nutzerkreis bedeutet dennoch weniger unabhängige Praxiserfahrung und weniger externe Bestätigung als bei etablierten Emulatoren.

**Reife-Einschätzung:** Der technische Kern und die Entwicklungsinfrastruktur wirken substanziell; als breit bewährtes Standardwerkzeug ist LabWired noch nicht etabliert. Für Experimente und CI-Smoke-Tests plausibel, für produktkritische Hardwareaussagen nur ergänzend.

## Leistungsfähigkeit und Grenzen

- LabWired führt echte, kompilierte Firmware gegen MCU- und Boardmodelle aus; es ist nicht bloß ein Mock des Anwendungs-Codes.
- Dokumentierte Beobachtungen umfassen UART-Ausgabe, GPIO-/Bus-Aktivität, Registerwerte und Pass/Fail-Assertions.
- Das Projekt demonstriert beispielsweise 200.000 Simulationsschritte in rund 0,04 Sekunden. Das ist ein einzelner Projektlauf und kein unabhängiger, systematischer Benchmark.
- Performance-Optionen wie Instruction-Decode-Cache, Bus-Fast-Paths und gebündelte Peripheral-Ticks können die Ausführung beschleunigen. Ein größerer Peripheral-Tick-Abstand kann jedoch beeinflussen, wann Peripherieereignisse beobachtet werden.
- Modelle und Prüfstände sind nicht für alle MCUs gleich vollständig. „Chip gelistet“ heißt nicht, dass jede benötigte Peripherie korrekt abgebildet ist.
- Selbst dort, wo ein Modell mit echter Hardware verglichen wurde, gilt die Validierung nur für den dokumentierten Umfang. Modelltreue ist nicht automatisch vollständige Board- oder Timing-Treue.

## Relevanz für Trice

### Gute Demo-Ziele

1. Ein normales, kompiliertes Trice-Beispiel im Simulator starten.
2. Prüfen, dass Firmware startet und definierte UART-Ausgaben oder GPIO-Zustände erzeugt.
3. Trice-Ereignisse reproduzierbar ausführen und den resultierenden Datenstrom beziehungsweise die UART-Ausgabe untersuchen.
4. Eine kleine CI-Prüfung erstellen, die bei Regressionen reproduzierbar fehlschlägt.

Vor dem Ausbau muss praktisch geprüft werden, ob LabWired den Trice-Binärdatenstrom über UART vollständig und unverändert verfügbar macht und ob er sich anschließend mit dem Trice-Hostdecoder auswerten lässt.

### Nicht als alleiniger Beleg verwenden

LabWired sollte nicht allein dazu dienen, Trices typische Zykluskosten, ISR-Latenzen oder Auswirkungen auf Echtzeitverhalten zu quantifizieren. Ein Simulator kann den Ablauf und seine virtuellen Zyklen nützlich reproduzieren, aber reale Taktkosten müssen auf der Zielhardware gemessen werden.

### MCU-Auswahl

Trices vorhandenes Beispiel `F030_inst_uart` ist ein naheliegender Startpunkt, aber STM32F030 war in der geprüften LabWired-Boardliste nicht als unterstütztes Target nachgewiesen. Das Beispiel daher nicht als direkt lauffähig annehmen.

Als möglicher Pilot bietet sich ein MCU-Profil mit gut dokumentiertem UART- und Interruptpfad an. STM32L476RG ist in LabWired ein wichtiger Hardware-Referenzpfad; für Trice wäre dafür aber zunächst ein passendes minimales Firmware-Beispiel bzw. Port nötig. Vor der Auswahl die aktuelle Board- und Peripherie-Matrix prüfen.

## Vorschlag für einen kleinen Machbarkeitstest

- Eine konkrete LabWired-Version pinnen.
- Ein minimales Trice-Firmwareprojekt für ein tatsächlich gut abgedecktes Boardprofil bauen.
- Erst Boot und normale UART-Ausgabe verifizieren.
- Danach einen kurzen Trice-Datenstrom erzeugen und prüfen, ob Rohbytes unverändert erfasst und dekodiert werden können.
- Falls ein Interrupt beteiligt ist, zunächst nur Funktion und Ereignisfolge prüfen; Zyklus- oder Latenzaussagen separat auf echter Hardware messen.
- Ergebnis mit ELF, Boardmanifest, Testscript, Rohmitschnitt und dokumentiertem Modellumfang reproduzierbar ablegen.

## Quellen

- [LabWired Core auf GitHub](https://github.com/w1ne/labwired-core)
- [LabWired: Firmware mit CLI ausführen](https://docs.labwired.com/getting_started_firmware/)
- [LabWired: Fidelity und Aussagekraft eines erfolgreichen Tests](https://docs.labwired.com/fidelity/)
- [LabWired: Target-Support-Stufen](https://docs.labwired.com/target_support_rubric/)
- [LabWired: Performance-Architektur](https://docs.labwired.com/architecture/)
- [LabWired: Validierungsstatus der Boards](https://docs.labwired.com/boards/VALIDATION_STATUS/)
- [Trice-Repository](https://github.com/rokath/trice)
- [Trice-Beispiele und Hinweise zum F030-UART-Beispiel](https://rokath.github.io/trice/docs/rm/examples.html)
