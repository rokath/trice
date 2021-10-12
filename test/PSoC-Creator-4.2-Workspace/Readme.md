# Steps

- Download [CY8CKIT-149 PSoC 4100S Plus Prototyping Kit.iso](https://www.cypress.com/documentation/development-kitsboards/cy8ckit-149-psoc-4100s-plus-prototyping-kit)
- Install & Open `PSoC Creator`
- Create Project from Target Device CY8CKIT-149 (PSoC 4100S Plus)
  - Use `Code Example` -> `Software_Transmit_UART`
  - Workspace name: `PSoC-Creator-4.2-Workspace` (avoid spaces)
  - Location: `C:\repos\trice\test` (avoid spaces)
  - Project name `UART`
- Select right hardware according `C:\repos\trice\test\PSoC_Creator_4.2_Workspace\CE195379_Software_Transmit_UART_PSoC3_4_5LP.pdf`
  - Project -> Context Menu -> Device Selector...
  - Select `CY8C4147AZI-S475`
    - Use device properties for filtering: CortexM0p CPU, 128KB Flash, 16KB RAM, 48 MHz, CapsSense=Y, LCD=N, ... to find the right device easier.
- compile, load & run
- Open git bash & enter `trice s` with and without plugged evaluation board to detect the right COM port
- Enter `trice l -p COM7 -baud 57600 -e CHAR` and the output should be similar to:
```
...
Software Transmit UART Component demo
Sending 254 as single byte hex: FE
Sending uint16 counter as two byte hex: 010F
Sending array: DataArray[13]

Software Transmit UART Component demo
Sending 254 as single byte hex: FE
Sending uint16 counter as two byte hex: 0110
Sending array: DataArray[13]
...
```
- Right click Source Files and add existing files
  - trice.c
  - ~~triceCheck.c~~
- Right click Header Files and add existing files
  - trice.h
  - triceConfig.h
 
- Copy template `_triceConfig.h` to file "C:\repos\trice\test\PSoC-Creator-4.2-Workspace\Software_Transmit_UART.cydsn\triceConfig.h"
- Add `trice.h` to compiler include path: Project -> Context -> Build Settings -> Compiler -> General -> Additional Include Directories: `..\..\..\pkg\src`
- Add `triceConfig.h` to compiler include path: Project -> Context -> Build Settings -> Compiler -> General -> Additional Include Directories: `.` (C:\repos\trice\test\PSoC-Creator-4.2-Workspace\Software_Transmit_UART.cydsn)
- Add `#include "trice.h"` to `main.c`
- In `main.c` replace code block
```
int main()
{
    uint16 cnt = 0;
    
    /* The array with characters to send */
    uint8 dataArray[DATAARR_LEN+1]={ 0x44u, 0x61u, 0x74u, 0x61u, 0x41u, 0x72u, 0x72u, 0x61u, 0x79u, 0x5Bu, 0x31u, 0x33u, 0x5Du, 0u };
    /*                                 D      a      t      a      A      r      r      a      y      [      1      3      ]   \0    */
    
    /* Start the SW_Tx_UART Component */
    SW_Tx_UART_Start();
    
    for(;;)
    {
        /* Transmit different data types through the UART */
        SW_Tx_UART_PutString("Software Transmit UART");
        SW_Tx_UART_PutString(" Component demo");
        SW_Tx_UART_PutCRLF();
        
        SW_Tx_UART_PutString("Sending 254 as single byte hex: ");
        SW_Tx_UART_PutHexByte((uint8)254u);
        SW_Tx_UART_PutCRLF();
        
        SW_Tx_UART_PutString("Sending uint16 counter as two byte hex: ");
        SW_Tx_UART_PutHexInt(cnt);
        SW_Tx_UART_PutCRLF();
        
        SW_Tx_UART_PutString("Sending array: ");
        SW_Tx_UART_PutArray(dataArray, DATAARR_LEN);
        SW_Tx_UART_PutCRLF();
        
        cnt++;
        
        CyDelay(SEND_INTERVAL);
        
        SW_Tx_UART_PutCRLF();
    }
}
```
with
```
int main()
{
    uint16 cnt = 0;
    
    /* The array with characters to send */
    uint8 dataArray[DATAARR_LEN+1]={ 0x44u, 0x61u, 0x74u, 0x61u, 0x41u, 0x72u, 0x72u, 0x61u, 0x79u, 0x5Bu, 0x31u, 0x33u, 0x5Du, 0u };
    /*                                 D      a      t      a      A      r      r      a      y      [      1      3      ]   \0    */
    
    /* Start the SW_Tx_UART Component */
    SW_Tx_UART_Start();
    
    for(;;)
    {
        /* Transmit different data types through the UART */
        TRICE0("\nSoftware Transmit UART Component demo\n");
        TRICE8("Sending 254 as single byte hex: %02X\n", 254);
        TRICE16("Sending uint16 counter as two byte hex: %x\n", cnt);
        TRICE_S("Sending array: %s\n", dataArray);
        cnt++; 
        CyDelay(SEND_INTERVAL);
    }
}
```

- Add Pre Build user command `trice u . -til ../../../til.json`: Project -> Context -> Build Settings -> User Commands -> Pre Build