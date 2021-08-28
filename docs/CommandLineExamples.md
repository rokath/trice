
# `trice` Command Line switches

- The `trice` tool has many command line options, but is very easy to use with default values.
- No config file implemented yet. But the command history is usable for example inside the bash, simply enter CTRL-R and start typing `trice...` and you can select from the history.
- All commands are tested.

## Cheat sheet

- `trice h -all` shows all options of the current version.
- `trice ver` prints version information.
- `trice u` in the root of your project parses all source files for **TRICE** statements, adds automatically ID´s if needed and updates a file named **til.json** containing all ID´s with their format string information. To start simply generate an empty file named **til.json** in your project root. You can add `trice u` to your build process and need no further manual execution.
- `trice s` shows you all found serial ports for your convenience.
- `trice l -p COM18 -u` listens and displays trice logs on serial port COM18 at default baud rate 115200. It uses the **til.json** file. `-u` interprets hexadecimal (%x) and binary (%b) values as unsigned numbers. 
- `trice ds` starts a display server listening on default ip address *127.0.0.1:61487* or any specified value, so also on a remote device, lets say with ip address 192.168.1.200.
- `trice l -p COM18 -ds` sends the log strings to a display server with default ip address *127.0.0.1:61487* or any specified value, if for example `-ipa 192.168.1.200` the trice logs go to the remote device. You can start several trice log instances, all transmitting to the same display server.

## Further examples

- Scan directories `../src`, `../lib/src` and `./` to update the IDs there and extend list file `../../../til.json`

```bash
trice u -i ../../../til.json -src ../src -src ../lib/src -src ./
```

- Log flexL trice messages on COM3 8N1 115200 baud

```bash
trice log -i ./myProject/til.json -p=COM3
```

- Log wrap trice messages on COM3 8N1 9600 baud ans use default til.json

```bash
trice l -s COM3 -baud=9600 -format esc
```

- Start displayserver on ip 127.0.0.1 (locahost) and port 61497

```b
trice ds
```

- Log bare trice messages on SEGGER J-Link RTT channel 2 and display on display server

```b
trice l -ds -JLRTT -p="-Device STM32F030R8 -if SWD -RTTChannel 2"
```

- Shutdown remote display server on IP 192.168.1.23 port 45678

```b
trice -sd -r 192.168.1.23:45678
```
