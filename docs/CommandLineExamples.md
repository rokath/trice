
# `trice` Command Line switches examples

(work in progress...)

## Current state

- No config file implemented yet. But all other commands are working.
- All following information about config files are just ideas.
- The config file can contain one or several subcommands and flags:
  - If exactly one subcommand in config file: No need to to be specified in the command line.
- The shortcut names are more for the command line and the long names more for the config files.
- Flags need to be associated to a subcommand inside config file.

## OPTIONS

- The **trice** command accepts one and only one sub-command (which can be inside the config file) followed by optional flags: `trice [s|subcommand [-f|flag [value]] [...]]`
- The flags start with a single dash and are boolean or need a value separated by space or `=`.
- Flags in square brackets `[]` are optional. If omitted they have a default value depending on the context.
- Some flags can occur several times. Check with `trice h` for multi-flags.
- Command line subcommand and flags are overwriting config file values.
- A subcommand in the command line is the selected subcommand in config files if there is any.
- You can place different subcommands with flags in a config file and specify only a subcommand in the command line for selection, like `trice u` and `trice l`.
- `trice l [...]` needs to be started only once for a development session. For example when running `trice u [...]` (in the toolchain), the still active trice logger detects til.json changes and re-reads the list automatically.

### `trice [...]`

Search for config files named *trice.conf* and use the settings there. The search goes on top down places. A value in a more down config overloads previous value settings. Command line values have highest precedence. The top down search places are:

- Directory of `trice` executable
- /
- /current/
- /current/path/
- /current/path/dir/

### `trice h|help [-c|config cfilename] [-i|idlist idfilename`

Display help and locations of config files named *trice.conf* and all list files *til.json* occurrences in top down config search. If *-c* and|or *-l* is specified, the appropriate filenames are used instead.

- The `-config` option is not implemented yet.
- The `-idlist` option is not implemented yet.

### `trice u|upd|update [-dry-run] [-i|idlist filename] [-src dirname -src filename ...] [-v|verbose]`

Update IDs in scource code and ID list.

- `-dry-run` Do nothing, just show the output.
- `-v|verbose` Show more output.
- `-l|list filename` Use filename as ID listfile. Default is *til.json* first found on the config file search places.
- `-src dirname -src filename ...` Scan all files in dirname and also filename. This is a multi parameter.

### `trice c|config filename [...]`

Use filename as config file and ignore all config files named *trice.conf*. If filename is *[path/]trice.conf* no other *trice.conf* is evaluated and it is searched only in the current directory or the specified path location. The config file must contain one and only one subcommand or a subcommand must be inside the command line.

### `trice v|version [-l|logfile off|filename]`

Show trice version. To log version information in a file use -lg filename. Default is off. This can be useful for automatic logging.

### `trice s|scan`

Scan for available COMports.

### `trice ds|displayserver [-l|logfile off|filename] [-color [off|alternate]] [-s|source IPaddr:IPport]`

Start as display server which can serve as destination for several trice instances.

- `-l|logfile off|filename`
  - Default filename is "2006-01-02_1504-05_trice.log" with the current time. To switch logging off set to "off".
- `-color [off|alternate]`
  - Switch color off or use alternate color set.

### `trice sd|shutdownServer [-r|remote IPaddr:IPport]`

Send control command to display server for self-shutdown.
This can be handy if display server is not visible or on a remote mashine.

### `trice l|log [-c|config off|filename] [-i|idlist filename] [-lg|logfile off|filename] [-ts off|UTCmicro] [-color off|alternate] [-f|format b|bare|w|wrap] [-k|key password [-show]] [-prefix off|string] [-postfix off|string] [-s|source COM|filename|RND|SIM|JLRTT|STRTT [-p|param string]] [-r|remote no|ds|displayserver|IPaddr:IPport] [-a|autostart cmd] [-write channel:file] [-show channel] [-hide channel]`

Log trice messages from -source and display them line by line. Default destination is current display.

- `-c|config off|filename` Default filename is *trice.conf*. It is searched on top down places. Last config overloads previous settings. Command line flags overload config settings and "off" ignores *trice.conf* files.
- `-l|logfile off|filename` Default filename is `2006-01-02_1504-05_trice.log` with current time. To switch logging off set to "off". When -remote switch is active the default is "off", because the destination logs then.
- `-ts off|UTCmicro` Default added timestamp is LOCmicro. Use this flag to change to UTC or to switch timestamps off.
- `-f|format b|bare|w|wrap` Default format is bare. That means a basic trice message is 4 bytes: 2 bytes ID and 2 bytes value. It is faster on the target, but cyclic sync messages are reccommended (**TRICE_SYNC**). The wrap format contains bare and 4 bytes additional control information. This allowes automatic host syncing and additional protocols on the same channel.
- `-k|key password [-show]` Decrypt encrypted trice messages if password is not "none". The switch -show displays the passphrase.
- `-prefix off|string` Add string as prefix to begin of each trice log line. It defaults to the source channel and can suppressed with "off".
- `-postfix string` Add string as postfix to end of each trice log line. It defaults to *\n*. The value *\n* was internally removed before, so the default value results to the original string.
- `-s|source COMn|filename|JLRTT|STRTT|RND|SIM [-p|param string]]` Use source as trice input. The switch -param is accordingly to source.
  - **COMn**: COM15 as example. The only possible parameter is the baud rate. *-param 115200* is default value.
  - **filename**:
    - **NOT IMPLEMENTED** When filename is an executable it is started with a random temporary logfile, internally read from. -param is accordingly to the executable command line.
    - When filename is a binary file this file is read and data interpreted according to -format switch.
    - **NOT IMPLEMENTED** When filename is an ASCII file, lines are diplayed. Can be used with logfiles. Default logging is off in that case.
  - **JLRTT** Starts the *JLinkRTTLogger* executable which is expected in the trice executable directory together with acompanying libraries. The -param string is according to *JLinkRTTLogger* description.
  - **STRTT** **NOT IMPLEMENTED** Starts the *STLinkRTTLogger* executable which is expected in the trice executable directory together with acompanying libraries. The -param string is according to *STLinkRTTLogger* description.
  - **RND**: random data are used as input and interpreted according to -format switch. The switch -param is used as control for the random generator. Default is total random data. Usable for testing.
    - *-param=wrap* generates valid wrap data with random payload.
  - **SIM**: inputDummy data are used as input and interpreted according to -format switch. The switch -param is used as control for the inputDummy generator. Usable for testing.
    - *-param=name* uses simulation dataset *name*.
- `-color off|alternate` Switch color off or use alternate color set. Ignored when switch -remote is used.
- `-r|remote no|ds|displayserver|IPaddr:IPport` Do not show loglines and send them to a displayserver instead. Default is no. localhost:61497.
- `-a|autostart cmd` Start `cmd` in background. Try `trice ds` to start displayserver automatically.
- `-write channel:file` **NOT IMPLEMENTED** Write all trice lines starting with *channel:* into file without line prefix, timestamp and *channel:*. This can be usable if you have parallel to normal trices for example JSON format lines for using **Chrome://tracing** to view your inline profiling data. This is a multi-parameter. Several channels can go in the same or different files.
- `-show channel` **NOT IMPLEMENTED** Display only trices starting with *channel:*. If this switch is not applied all channels are displayed. This is a multi-parameter. The same channel as parameter for -hide is an error.
- `-hide channel` **NOT IMPLEMENTED** Hide trices starting with *channel:*. If this switch is not applied all channels are displayed. This is a multi-parameter. The same channel as parameter for -show is an error.

## EXAMPLES

- Scan directories `../src`, `../lib/src` and `./` to update the IDs there and extend list file `../../../til.json`

```bash
trice u -i ../../../til.json -src ../src -src ../lib/src -src ./
```

- Log bare trice messages on COM3 8N1 115200 baud

```bash
trice log -i til.json -s=COM3
```

- Log wrap trice messages on COM3 8N1 9600 baud ans use default til.json

```bash
trice l -s COM3 -p="9600" -format wrap
```

- Start displayserver on ip 127.0.0.1 (locahost) and port 61497

```b
trice ds
```

- Log bare trice messages on SEGGER J-Link RTT channel 2 and display on display server

```b
trice l -r ds -JLRTT -p="-Device STM32F030R8 -if SWD -RTTChannel 2"
```

- Do an in *trice.conf* specified action

```b
trice
```

- Shutdown remote displayserver on IP 192.168.1.23 port 45678

```b
trice -sd -r 192.168.1.23:45678
```

- Play recorded wrap format binary file *log.bin*

```b
trice l -s log.bin -f w
```

- Start *JLinkRTTLogger* manually on Windows and use RTT channel 0.

```b
trice log -s="C:\\Program Files (x86)\\SEGGER\\JLink\\JLinkRTTLogger.exe" -p="-Device STM32F030R8 -if SWD -RTTChannel 0" 
```

- Start logging on COM7 8N1 with 38400 baud, start display server and send loglines there:

```b
trice l -s COM7 -p "38400" -a "trice ds"
```
