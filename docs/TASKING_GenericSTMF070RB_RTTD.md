# TASKING_GenericSTMF070RB_RTTD

- About: [SeggerRTT.md](SeggerRTT.md)

![TASKING_GenericSTMF070RB_RTTD.PNG](./README.media/TASKING_GenericSTMF070RB_RTTD.PNG)
- Start TASKING ARM Eclipse IDE with some workspace
- File -> New -> TASKING Arm Project -> Hello Worls C Project
- [ ] Uncheck "Use default location"
![TASKING_GenericsSTMF030R8_RTTD_NewProject.PNG](./README.media/TASKING_GenericsSTMF030R8_RTTD_NewProject.PNG)
- -> Next, select processor hardware & 
- [x] check "Add startup file(s) to the project"
- Select as target "Generic STMicroelectronics. This will allow you to 
- Right-Click on Project -> New -> Tasking Software Platform Document -> Generate Code
- Add Software Services and Device Stacks
- Needs ST-Link "STM32Debug+VCP" and **NOT** "STM32Debug+Mass storage+VCP" (change with ST-Link utility)
- Needs to ignore this:
![TASKING_NoST-LinkDrivers.PNG](./README.media/TASKING_NoST-LinkDrivers.PNG)
- ~190 clocks
  ![TASKING_GenericSTMF070RB_RTTDclocks.PNG](./README.media/TASKING_GenericSTMF070RB_RTTDclocks.PNG)