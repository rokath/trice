# Segger RTTI connection
- If you use a Segger J-Link probe or you have a compatible ST-Link (board) and follow these instructions `https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/`, trices can be transmitted over the debug port.
  - This does not occupy any physical device port nedded for the app during developmemt.
  - The transfer can be done in background without occupying the target processor despite memcopy actions.
## Segger J-Link SDK (~800 EUR)
- Issue: (https://forum.segger.com/index.php/Thread/6688-SOLVED-How-to-access-multiple-RTT-channels-from-Telnet/)[https://forum.segger.com/index.php/Thread/6688-SOLVED-How-to-access-multiple-RTT-channels-from-Telnet/]
  - Developer pack used to write your own program for the J-Link. Please be sure you agree to the terms of the associated license found on the Licensing Information tab before purchasing this SDK. You will benefit from six months of free email support from the time that this product is ordered.
  - (https://www.segger.com/products/debug-probes/j-link/technology/j-link-sdk/)[https://www.segger.com/products/debug-probes/j-link/technology/j-link-sdk/]

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

- Go to (https://libusb.info/)[https://libusb.info/] 
  - -> Downloads -> Latest Windows Binaries
  - extract `libusb-1.0.23` (or later version)
```
libusb-1.0.23\examples\bin64> .\listdevs.exe
2109:2811 (bus 2, device 8) path: 6
1022:145f (bus 1, device 0)
1022:43d5 (bus 2, device 0)
0a12:0001 (bus 2, device 1) path: 13
1366:0105 (bus 2, device 10) path: 5
```
- Repeat without connected Segger JLink
```
libusb-1.0.23\examples\bin64> .\listdevs.exe
2109:2811 (bus 2, device 8) path: 6
1022:145f (bus 1, device 0)
1022:43d5 (bus 2, device 0)
0a12:0001 (bus 2, device 1) path: 13
```  
- In this case `1366:0105 (bus 2, device 10) path: 5` is missing, so `vid=1366`, `did=0105` as example
- On Windows install WSL2. The real Linux kernel is needed for full USB access.
