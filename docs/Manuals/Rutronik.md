
# (Nolte:)

Zu dem Info-Material:

Ein einfaches Tracing sollte mit dem (onboard) J-Link und einer eclipse-basierten IDE möglich sein. (via Breakpoints setzen und Variableninhalte spezieller Tracevariablen auslesen…)

Für erweitertes Tracing eines nicht modifizierten Projekts inklusive Live-Updates der Werte wird von ST folgende Soft- und Hardware empfohlen:

[STM32CubeMonitor](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-performance-and-debuggers/stm32cubemonitor.html)

[Segger J-TRACER Pro for Cortex-M](https://www.st.com/en/development-tools/j-trace-pro-for-cortex-m.html#product-details)

J-Link ist ja wahrscheinlich schon vorhanden und ist einen ersten Versuch wert, da die ST-Software gratis ist.

(Alternativ dazu kann man sich ein eigenes Tool für den virtuellen COM-Port stricken)

ST ist ebenfalls auf github aktiv: [https://github.com/STMicroelectronics](https://github.com/STMicroelectronics)

Es gibt [STM32Cube expansion packages](https://www.st.com/en/embedded-software/stm32cube-expansion-packages.html#products)

Die meisten packages sind kostenlos.