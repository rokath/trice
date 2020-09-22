STLink firmware upgrade applications

1. In folder Windows:

   Contains the application in its "historical" form: executable dedicated to Windows + dll.
   With all most recent firmwares (V1J13S3 for ST-Link/V1 boards, V2J37S7 for ST-Link/V2 boards,
   V2J37M26 for ST-Link/V2-1 boards, V3J7M2 for STLINK-V3 boards)

2. In folder AllPlatforms:

   Contains a new GUI based on Java, with native ST-Link libraries for running on Windows 32 bits,
   Windows 64bits, Linux 32bits, Linux 64bits and MacOS 64bits.
   
   The application requires the Java Runtime Environment 7u51 (or more recent) being installed.
   In platforms without .jar file extension association, the application can be launch by the command
   "java -jar STLinkUpgrade.jar".

   On Linux, the application relies on libusb-1.0, which must be installed separately. For
   instance on Ubuntu, through the command "sudo apt-get install libusb-1.0".

   On Linux, users must be granted with rights for accessing the ST-Link USB devices. To do
   that, it might be necessary to add rules into /etc/udev/rules.d. This can be achieved by
   running one of the package provided into the subdirectory StlinkRulesFilesForLinux.
