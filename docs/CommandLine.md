
# `trice` Command Line switches
## Current state
- No config file implemented yet.
- All following information about config files are just ideas.
- The config file can contain no, one or several subcommands:
  - If exactly one subcommand in config file: No need to to be specified in the command line.
- The shortcut names are more for the command line and the long names more for the config files.

## OPTIONS
- The **trice** command accepts one and only one sub-command (which can be inside the config file) followed by optional parameters: `trice [subcommand [-p|param [value]] [...]]`
- The optional parameters start with a single dash and are boolean or need a value separated by space or `=`. 
- Parameters in square brackets `[]` are optional. If omitted they have a default value. 
- Parameter options [...] are overwriting config file values.
- A subcommand in the command line is the selected subcommand in config files if there is any.
- You can place different subcommands with parameters in a config file and specify only a subcommand in the command line for selection, like `trice u` and `trice l`.
- When adding `trice u [...]` to toolchain with a config file only `trice` needs to be started once for logging.

### `trice [...]`
Search for config files named *trice.conf* and use the settings there. The search goes  on top down places. A parameter in a more down config overloads previous settings. Command line values have highest precedence. The top down search places are:
- Directory of `trice` executable
- /
- /current/
- /current/path/
- /current/path/dir/

### `trice c|config filename [...]`
Use filename as config file and ignore all config files named *trice.conf*. If filename is *[path/]trice.conf* no other *trice.conf* is evaluated and it is searched only in the current directory or the specified path location. The config file must contain one and only one subcommand or a subcommand must be inside [...].

### `trice h|help [-c|config cfilename] [i|idlist idfilename`
Display help and locations of config files named *trice.conf* and all list files *til.json* occurences in top down config search. If *-c* and|or *-l* is specified, the appropriate filenames are used instead.

### `trice u|update [-dry-run] [-v|verbose] [-i|idlist filename] [-src dirname -src filename ...]`
Update IDs in scource code and ID list.
- `[-dry-run]` Do nothing, just show the output.
- `[-v|verbose]` Show more output.
- `[-l|list filename]` Use filename as ID listfile. Default is *til.json* first found on the config file search places.
- `[-src dirname -src filename ...]` Scan all files in dirname and also filename. This is a multi parameter.

###  `trice v|version [-lg|logfile off|filename]`
Show trice version. To log version information in a file use -lf filename. Default is off. This can be useful for automatic logging.

### `trice s|scan`
Scan for available COMports.

### `trice ds|displayserver [-lg|logfile off|filename] [-color [off|alternate]] [-s|source IPaddr:IPport]`
Start as display server which can serve as destination for several trice instances.
- `[-lg|logfile off|filename]`
    - Default filename is "2006-01-02_1504-05_trice.log" with the current time. To switch logging off set to "off".
- `[-color [off|alternate]]`
  - Switch color off or use alternate color set.

### `trice sd|shutdownServer [-r|remote IPaddr:IPport]`
Send control command to display server for self-shutdown.
This can be handy if display server is not visible or on a remote mashine.

### `trice l|log [-c|config off|filename] [-i|idlist filename] [-lg|logfile off|filename] [-color [off|alternate]] [-f|format b|bare|w|wrap] [-k|key password [-show]] [-prefix off|string][-postfix off|string] [-s|source COM|filename|RND|SIM|JLRTT|STRTT [-p|param string]] [-r|remote ds|displayserver|IPaddr:IPport] [-a|autostart cmd]`
Log trice messages from -source and display them line by line. Default destination is current display.
- `[-c|config off|filename]` Default filename is *trice.conf*. It is searched on top down places. Last config overloads previous settings. Command line params overload config settings and "off" ignores *trice.conf* files.
- `[-lg|logfile off|filename]` Default filename is `2006-01-02_1504-05_trice.log` with current time. To switch logging off set to "off". When -remote switch is active the default is "off", because the destination logs then.
- `[-f|format b|bare|w|wrap]` Default format is bare. That means a basic trice message is 4 bytes: 2 bytes ID and 2 bytes value. It is faster on the target, but cyclic sync messages are reccommended (**TRICE_SYNC**). The wrap format contains bare and 4 bytes additional control information. This allowes automatic host syncing and additional protocols on the same channel.
- `[-k|key password [-show]]` Decrypt encrypted trice messages if password is not "none". The switch -show displays the passphrase.
- `[-prefix off|string]` Add string as prefix to begin of each trice log line. It defaults to the source channel and can suppressed with "off".
- `[-postfix string]` Add string as postfix to end of each trice log line. It defaults to "".
- `[-s|source COMn|filename|JLRTT|STRTT|RND|SIM [-p|param string]]` Use source as trice input. The switch -param is accordingly to source.
  - **COMn**: COM15 as example. -param 115200 is default value
  - **filename**: 
    - When filename is an executable it is started with a random temporary logfile, internally read from. -param is accordingly to the executable commandline:
    - When filename is a binary file this file is read and data interpreted according to -format switch.
    - When filename is an ASCII file, lines are diplayed. Can be used with logfiles. Default logging is off in that case.
  - **JLRTT** Starts the *JLinkRTTLogger* executable which is expected in the trice executable directory together with acompanying libraries. The -param string is according to *JLinkRTTLogger* description.
  - **STRTT** Starts the *STLinkRTTLogger* executable which is expected in the trice executable directory together with acompanying libraries. The -param string is according to *STLinkRTTLogger* description.
  - **RND**: random data are used as input and interpreted according to -format switch. The switch -param is used as control for the random generator. Usable for testing.
  - **SIM**: inputDummy data are used as input and interpreted according to -format switch. The switch -param is used as control for the inputDummy generator. Usable for testing.
- `[-color [off|alternate]]` Switch color off or use alternate color set. Ignored when switch -remote is used.
- `[-r|remote ds|displayserver|IPaddr:IPport]` Do not show loglines and send them to a displayserver instead. Default is localhost:61497.
- `[-a|autostart cmd]` Start `cmd` in background. Try `trice ds` to start displayserver automatically.
## EXAMPLES:
- Scan directories `../src`, `../lib/src` and `./` to update the IDs there and extend list file `../../../til.json`
```
trice u -l ../../../til.json -src ../src -src ../lib/src -src ./
```
- Log bare trice messages on COM3 8N1 115200 baud
```
trice log -l til.json -s=COM3
```
- Log wrap trice messages on COM3 8N1 9600 baud
```
trice l -s COM3 -p="9600" -format wrap
```
- Start displayserver on ip 127.0.0.1 (locahost) and port 61497
```
trice ds
```
- Log bare trice messages on SEGGER J-Link RTT channel 2 and display on display server
```
trice l -r ds -JLRTT -p="-Device STM32F030R8 -if SWD -RTTChannel 2"
```
- Do an in *trice.conf* specified action
```
trice
```
- Shutdown remote displayserver on IP 192.168.1.23 port 45678
```
trice -sd -r 192.168.1.23:45678
```
- Play recorded wrap format binary file *log.bin* 
```
trice l -s log.bin -f w
```
- Start *JLinkRTTLogger* manually on Windows and use RTT channel 0. 
```
trice log -s="C:\\Program Files (x86)\\SEGGER\\JLink\\JLinkRTTLogger.exe" -p="-Device STM32F030R8 -if SWD -RTTChannel 0" 
```
- Start logging on COM7 8N1 with 38400 baud, start display server and send loglines there:
```
trice l -s COM7 -p "38400" -a "trice ds"
```
