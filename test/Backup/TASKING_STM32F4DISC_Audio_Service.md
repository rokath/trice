# TASKING_STM32F4DISC_Audio_Service

- For some reason this example does not work.
- Instrumentation: SEGGER_RTTD
- Tried to use Generic device with J-Link but no output
- Tried to use Evalboard with ST-Link
  - Use Segger Reflash to flash ST-Link
  - Use ST-Link Flash to flash ST-Link without Mass-Storage
  - Use TASKING to flash Firmware
  - Use ST-Link Flash to flash ST-Link with Mass-Storage
  - Use Segger Reflash to flash J-Link
  - Start J-Link viewer
  - trice log -device RTTD
  - RESET-Button: NO Output...
- About: [SeggerRTT.md](SeggerRTT.md)

![TASKING_STM32F4DISC_Audio_Service_0.PNG](./README.media/TASKING_STM32F4DISC_Audio_Service_0.PNG)
![TASKING_STM32F4DISC_Audio_Service_1.PNG](./README.media/TASKING_STM32F4DISC_Audio_Service_1.PNG)
- Project is now inside .workspace folder
- Delete Project inside the IDE but not the project itself
- Move the Project out of .workspace and rename it to TASKING_STM32F4DISC_Audio_Service
- Import existing project and **do not** copy it into workspace
