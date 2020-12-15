# TASKING_GenericSTMF030R8_RTTD

- About: [SeggerRTT.md](SeggerRTT.md)

![TASKING_GenericSTMF070RB_RTTD_NewProject.PNG](./README.media/TASKING_GenericSTMF070RB_RTTD_NewProject.PNG)
![TASKING_GenericsSTMF030R8_RTTD_J-Link.PNG](./README.media/TASKING_GenericsSTMF030R8_RTTD_J-Link.PNG)
- Build and check if Debug works:
![TASKING_GenericsSTMF030R8_RTTD_Debug.PNG](./README.media/TASKING_GenericsSTMF030R8_RTTD_Debug.PNG)
- Add New Source Folder SeggerRTT
- Import -> General -> Filesystem
  - Check "Create Top-Level Folder" and Advanced -> "Create Links in Workspace")
![TASKING_GenericsSTMF030R8_RTT_Import.PNG](./README.media/TASKING_GenericsSTMF030R8_RTT_Import.PNG)
- Add path 
![TASKING_GenericsSTMF030R8_RTT_Path.PNG](./README.media/TASKING_GenericsSTMF030R8_RTT_Path.PNG)
- Edit 
- Start Debug & J-Link Viewer and run:
![TASKING_GenericsSTMF030R8_RTTD_Debug2.PNG](./README.media/TASKING_GenericsSTMF030R8_RTTD_Debug2.PNG)
![J-LinkViewerHelloWorld.PNG](./README.media/J-LinkViewerHelloWorld.PNG)
- Add New Source Folder srcTrice.C
- Import -> General -> Filesystem
  - Check "Create Top-Level Folder" and Advanced -> "Create Links in Workspace")
![TASKING_GenericsSTMF030R8_RTTD_Trice.PNG](./README.media/TASKING_GenericsSTMF030R8_RTTD_Trice.PNG)
- specify in project settings:
  - TRICE_VARIANT=SEGGER_RTT
  - TRICE_PUSH=triceDirectWrite
![TASKING_GenericsSTMF030R8_RTTD_ok_IDE.PNG](./README.media/TASKING_GenericsSTMF030R8_RTTD_ok_IDE.PNG)
- ~230 clocks

  ![TASKING_GenericSTMF030R8_RTTD.PNG](./README.media/TASKING_GenericSTMF030R8_RTTD.PNG)

- ~190 clocks with time optimization

  ![TASKING_GenericSTMF030R8_RTTD_TimeOpt.PNG](./README.media/TASKING_GenericSTMF030R8_RTTD_TimeOpt.PNG)

