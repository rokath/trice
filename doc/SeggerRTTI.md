# Segger RTTI connection
- If you use a Segger J-Link probe or you have a compatible ST-Link (board) and follow these instructions `https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/`, trices can be transmitted over the debug port.
  - This does not occupy any physical device port nedded for the app during developmemt.
  - The transfer can be done in background without occupying the target processor despite memcopy actions.
## Notes
- `Downloading RTT target package` from (https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/)[https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/].
- Read the f* manual `"C:\Program Files (x86)\SEGGER\JLink\Doc\Manuals\UM08001_JLink.pdf"`.
- Connect Segger J-Link probe or flash Segger J-Link Firmware on target board.
- Extract `"C:\Program Files (x86)\SEGGER\JLink\Samples\RTT\SEGGER_RTT_V672b.zip"` to target project.
- Add `SEGGER_RTTI.c` to target project
- Put into main(): `char* s = "Hi5!\n"; SEGGER_RTT_Write(0, s, strlen(s));`, compile & flash image
- Start `"C:\Program Files (x86)\SEGGER\JLink\JLinkRTTViewer.exe"` & connect.
- Reset target & `Hi5!` should be visible inside viewer window.
- Get & run `https://github.com/stalehd/jlinklogviewer`.
- Now `Hi5!` should be visible inside terminal window when resetting target board.
- Instead of `"C:\Program Files (x86)\SEGGER\JLink\JLinkRTTViewer.exe"` also `"C:\Program Files (x86)\SEGGER\JLink\JLink.exe"` could be started & then `https://github.com/stalehd/jlinklogviewer`.
- What also works: `"C:\Program Files (x86)\SEGGER\JLink\JLink.exe"` followed by a terminal program like TeraTerm connected to `localhost:19021`.
## Todo
- Checkout (https://github.com/deadsy/jaylink)[https://github.com/deadsy/jaylink].
- `"C:\Program Files (x86)\SEGGER\JLink\JMem.exe"` shows a memory dump.

